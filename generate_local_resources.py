"""Generate private build inputs from a separately purchased, hash-pinned GOG game.

Outputs contain original game material. Keep the complete output directory local
and ignored; never add it to a source snapshot or upload it. This tool does not
grant redistribution rights. It never executes the game or its DLLs.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import runpy
import struct
import subprocess
import sys

ROOT = Path(__file__).resolve().parent
HELPERS = ROOT / "local_generators"
EXE_SHA = "9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435"
DLLS = {
    "Mss32.dll": "858d6066bab4445f56c2d1713f16bfa742fb43a149c6afcb83460efe5bdec7da",
    "binkw32.dll": "2d0ae23a6175dc7b635c402a5e7e9542e923c0d1c376a8c5ef876ca0d5959d23",
}
RENDERER_GROUPS = {
    "mesh": ("mesh-spec.json", "kotor_original_renderer_data.generated.h"),
    "vp-data": ("vp-data-spec.json", "kotor_original_renderer_data.generated.h"),
    "vp-rdata": ("vp-rdata-spec.json", "kotor_original_renderer_data.generated.h"),
    "epoch": ("epoch-spec.json", "kotor_renderer_programs_original_data.generated.h"),
    "window": ("window-spec.json", "kotor_window_original_data.generated.h"),
}


def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def disjoint(a: Path, b: Path) -> bool:
    return not (a.is_relative_to(b) or b.is_relative_to(a))


def helper(name: str, arguments: list[str], log: Path) -> None:
    # Helpers are reviewed source in this candidate, not paths from the game.
    run = subprocess.run([sys.executable, "-I", str(HELPERS / name), *arguments],
                         cwd=log.parent, capture_output=True, text=True, timeout=120)
    log.write_text(run.stdout + run.stderr, encoding="utf-8")
    if run.returncode:
        raise RuntimeError(f"Generator {name} failed; inspect local {log.name}")


def original_resources(executable: Path, output: Path) -> None:
    import pefile  # Installed separately; no third-party source is vendored here.

    flags = {1: 0x1010, 3: 0x1010, 12: 0x1030, 14: 0x1030, 16: 0x0030}
    data = struct.pack("<II", 0, 32) + struct.pack("<HHHH", 0xFFFF, 0, 0xFFFF, 0) + bytes(16)
    with pefile.PE(str(executable)) as pe:
        for kind in pe.DIRECTORY_ENTRY_RESOURCE.entries:
            if kind.id not in flags:
                continue
            for item in kind.directory.entries:
                if item.id is None or not 0 <= item.id <= 65535:
                    raise ValueError("Unsupported named/out-of-range original resource")
                for language in item.directory.entries:
                    record = language.data.struct
                    blob = pe.get_data(record.OffsetToData, record.Size)
                    if len(blob) != record.Size:
                        raise ValueError("Truncated original resource")
                    header = struct.pack("<HHHH", 0xFFFF, kind.id, 0xFFFF, item.id)
                    header += struct.pack("<IHHII", 0, flags[kind.id], language.id, 0, 0)
                    entry = struct.pack("<II", len(blob), 8 + len(header)) + header + blob
                    data += entry + bytes(-len(entry) % 4)
    output.write_bytes(data)


def resource_pairs(executable: Path, output: Path) -> None:
    import pefile

    entries = json.loads((HELPERS / "resource-pairs-spec.json").read_text(encoding="utf-8"))
    if len(entries) != 88:
        raise ValueError("Expected 88 ordered resource pairs")
    lines = ['#pragma section(".rdata$BRT", read)',
             '// Local ignored original-backed data; do not publish.',
             '#include "baseinternal.h"', '#include "warning_runtime.h"',
             'namespace base_resource_table {']
    with pefile.PE(str(executable)) as pe:
        for index, item in enumerate(entries):
            address = int(item["original_address"], 16)
            size = item["size"]
            if item["index"] != index or type(item["id"]) is not int or not 0 <= item["id"] <= 65535:
                raise ValueError("Invalid resource-pair ordering/ID")
            if type(size) is not int or not 1 <= size <= 4096:
                raise ValueError("Invalid resource literal size")
            data = pe.get_data(address - pe.OPTIONAL_HEADER.ImageBase, size)
            if (len(data) != size or data[-1:] != b'\0' or b'\0' in data[:-1]
                    or any(b >= 128 for b in data)
                    or hashlib.sha256(data).hexdigest() != item["span_sha256"]):
                raise ValueError("Original resource literal span mismatch")
            if index < 87:
                symbol = f"resource_literal_{address:08x}"
                if item["canonical_symbol"] != "base_resource_table::" + symbol or item["new_definition"] is not True:
                    raise ValueError("Invalid resource literal owner")
                lines.append('extern const char ' + symbol + '[] = {' + ', '.join(f'0x{b:02x}' for b in data) + '};')
            elif (item["canonical_symbol"] != 'base_ini_reconstruction::warning_empty_0073d71c'
                  or item["new_definition"] is not False or data != b'\0' or address != 0x73D71C):
                raise ValueError("Invalid shared empty-string owner")
    lines.append('extern const ResourcePair canonical_resource_pairs[88] = {')
    lines.extend('    {' + str(item['id']) + 'u, ' + item['canonical_symbol'] + '},' for item in entries)
    lines.extend(['};', '__declspec(allocate(".rdata$BRT")) const ResourcePair (&resource_pairs)[88] = canonical_resource_pairs;', '}'])
    output.write_bytes(('\n'.join(lines) + '\n').encode('ascii'))


def extra_data(executable: Path, output: Path) -> None:
    import pefile

    groups = json.loads((HELPERS / 'extra-data-spec.json').read_text(encoding='utf-8'))
    with pefile.PE(str(executable)) as pe:
        for group in groups:
            destination = (output / group['output']).resolve()
            if not destination.is_relative_to(output):
                raise ValueError('Invalid generated output path')
            lines = ['// Generated locally from supported original; never publish.', *group['prefix']]
            for item in group['objects']:
                if (not re.fullmatch(r'[A-Za-z_][A-Za-z_0-9]*', item['name'])
                        or item['type'] not in ('char', 'unsigned char')
                        or item.get('storage', 'extern const') not in ('const', 'constexpr', 'extern const')
                        or type(item['size']) is not int or not 0 < item['size'] <= 4096
                        or item.get('align') not in (None, 4, 8)):
                    raise ValueError('Invalid generated object metadata')
                data = pe.get_data(int(item['va'], 16) - pe.OPTIONAL_HEADER.ImageBase, item['size'])
                if len(data) != item['size'] or (item['type'] == 'char' and any(b >= 128 for b in data)):
                    raise ValueError('Truncated or invalid original span')
                alignment = f"alignas({item['align']}) " if item.get('align') else ''
                values = ','.join(f'0x{b:02x}' for b in data)
                lines.append(f"{alignment}{item.get('storage', 'extern const')} {item['type']} {item['name']}[{item['size']}] = {{" + values + '};')
            destination.parent.mkdir(parents=True, exist_ok=True)
            destination.write_text('\n'.join([*lines, *group['suffix']]) + '\n', encoding='ascii')


def class_choices(executable: Path, output: Path) -> None:
    import pefile

    with pefile.PE(str(executable)) as pe:
        data = pe.get_data(0x007A2684 - pe.OPTIONAL_HEADER.ImageBase, 48)
    if len(data) != 48:
        raise ValueError('Truncated class-choice table')
    lines = ['// Original-backed local data; never publish.', '#include "m15a_gui.h"',
             'extern "C" KotorM15aClassChoice kotor_m15a_class_choices_007a2684[6] = {']
    lines.extend('    {' + ', '.join(str(n) + 'u' for n in record) + '},'
                 for record in struct.iter_unpack('<BBBBI', data))
    output.write_text('\n'.join([*lines, '};']) + '\n', encoding='ascii')


def upgrade_slots(executable: Path, output: Path) -> None:
    import pefile

    lines = ['// Local original-backed data; never publish.', '#include "claude_upgrade.hpp"', 'namespace upgrade {']
    records = []
    with pefile.PE(str(executable)) as pe:
        base = pe.OPTIONAL_HEADER.ImageBase
        table = pe.get_data(0x00756FB0 - base, 16 * 12)
        if len(table) != 16 * 12:
            raise ValueError("Truncated upgrade table")
        for i, (kind, pointer, strref) in enumerate(struct.iter_unpack('<iII', table)):
            data = pe.get_data(pointer - base, 256)
            end = data.find(b'\0')
            if end < 0 or any(b >= 128 for b in data[:end]):
                raise ValueError("Invalid upgrade icon string")
            values = ', '.join(f'0x{b:02x}' for b in data[:end + 1])
            lines.append(f'static const char icon_{i}[] = {{' + values + '};')
            records.append(f'    {{{kind}, icon_{i}, {strref}u}},')
    lines.extend(['extern const SlotInfo k_slots_00756fb0[16] = {', *records, '};', '}'])
    output.write_text('\n'.join(lines) + '\n', encoding='ascii')


def texture_literal(executable: Path, output: Path) -> None:
    import pefile

    with pefile.PE(str(executable)) as pe:
        data = pe.get_data(0x0073EE34 - pe.OPTIONAL_HEADER.ImageBase, 12)
    if len(data) != 12 or data[-1:] != b'\0' or any(b >= 128 for b in data):
        raise ValueError("Invalid original texture literal")
    values = ', '.join(f'0x{b:02x}' for b in data)
    output.write_text('// Local original-backed data; never publish.\nnamespace texture_cells {\n'
                      'extern const char norm_cube_map_0073ee34[12] = {' + values + '};\n}\n', encoding='ascii')


def import_libraries(game: Path, output: Path, librarian: Path) -> list[dict[str, str]]:
    import pefile

    if librarian.name.lower() != "lib.exe" or not librarian.is_file():
        raise ValueError("Pass an installed MSVC x86 lib.exe via --lib-exe")
    if any(k.upper() in {"CL", "_CL_", "LINK", "_LINK_", "LIB_FLAGS"} and v
           for k, v in os.environ.items()):
        raise ValueError("Inherited tool option overrides are not permitted")
    results = []
    for filename, expected in DLLS.items():
        dll = (game / filename).resolve(strict=True)
        if not dll.is_relative_to(game) or sha(dll) != expected:
            raise ValueError(f"Unsupported or redirected original DLL: {filename}")
        with pefile.PE(str(dll)) as pe:
            if pe.FILE_HEADER.Machine != 0x14C:
                raise ValueError("Expected x86 middleware DLL")
            names = [s.name.decode("ascii") for s in pe.DIRECTORY_ENTRY_EXPORT.symbols if s.name]
        if not names or len(names) != len(set(names)):
            raise ValueError("Empty or duplicate DLL exports")
        lines = [f"LIBRARY {filename.lower()}", "EXPORTS"]
        for name in names:
            if not re.fullmatch(r"[A-Za-z_?@$][A-Za-z0-9_?@$]*", name):
                raise ValueError("Unsupported DLL export name")
            stdcall = re.fullmatch(r"_([A-Za-z0-9_]+@\d+)", name)
            lines.append(f"  {stdcall[1]} EXPORTAS {name}" if stdcall else f"  {name}")
        stem = Path(filename).stem.lower()
        definition, library = output / f"{stem}.def", output / f"{stem}.lib"
        definition.write_text("\n".join(lines) + "\n", encoding="ascii")
        command = [str(librarian), "/nologo", f"/def:{definition}", "/machine:x86", f"/out:{library}"]
        run = subprocess.run(command, cwd=output, capture_output=True, text=True, timeout=120)
        (output / f"{stem}.log").write_text(run.stdout + run.stderr, encoding="utf-8")
        if run.returncode or not library.is_file() or library.stat().st_size == 0:
            raise RuntimeError(f"Import library generation failed for {filename}")
        results.append({"dll": filename, "dll_sha256": expected,
                        "library_sha256": sha(library), "lib_exe_sha256": sha(librarian)})
    return results


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--game-dir", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True, help="New private output outside this source tree and game install")
    parser.add_argument("--rules-src", type=Path, help="Reviewed rules source directory, read only")
    parser.add_argument("--renderer-spec", type=Path, help="Reviewed renderer address specification; default is the six-object GL specification")
    parser.add_argument("--lib-exe", type=Path, help="Optional installed MSVC x86 lib.exe; enables middleware import generation")
    args = parser.parse_args()
    game = args.game_dir.resolve(strict=True)
    out = args.out.resolve()
    executable = (game / "swkotor.exe").resolve(strict=True)
    if not game.is_dir() or not executable.is_relative_to(game) or sha(executable) != EXE_SHA:
        raise ValueError("The original executable does not match the supported GOG release")
    if not disjoint(out, game) or not disjoint(out, ROOT):
        raise ValueError("Output must be outside and separate from the installation and source tree")
    if out.exists():
        raise ValueError("Output must be a new directory; existing files are never replaced")
    rules = args.rules_src.resolve(strict=True) if args.rules_src else None
    if rules and (not rules.is_dir() or not disjoint(out, rules)):
        raise ValueError("Rules source must be a separate input directory")
    spec = (args.renderer_spec or HELPERS / "renderer-spec.json").resolve(strict=True)
    # Validate declarative names before they are emitted as C++ tokens.
    entries = json.loads(spec.read_text(encoding="utf-8"))
    for item in entries:
        if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", item["name"]):
            raise ValueError("Invalid renderer symbol")
        if item["kind"] not in {"bytes", "cstring"} or not re.fullmatch(r"[0-9a-fA-F]{8}", item["va"]):
            raise ValueError("Invalid renderer extraction specification")
        if "size" in item and (type(item["size"]) is not int or not 0 < item["size"] <= 4042752):
            raise ValueError("Invalid extraction size")
    if not disjoint(out, spec):
        raise ValueError("Output overlaps an input")
    # Fail before producing outputs if the optional parser dependency is absent.
    import pefile
    out.mkdir(parents=True)
    (out / ".gitignore").write_text("*\n", encoding="ascii")
    (out / "NEVER-PUBLISH.txt").write_text("Original game-derived build inputs: keep this directory private.\n", encoding="ascii")
    for name in ("resources", "renderer", "game-prepare", "sound", "rules", "imports", "resource-pairs", "controller", "texture", "upgrade", "class-choice", "pow", *RENDERER_GROUPS):
        (out / name).mkdir()
    original_resources(executable, out / "resources" / "swkotor-original.res")
    helper("renderer.py", [str(spec), str(out / "renderer"), str(executable)], out / "renderer.log")
    for group, (spec_name, header_name) in RENDERER_GROUPS.items():
        helper("renderer.py", [str(HELPERS / spec_name), str(out / group), str(executable)], out / f"{group}.log")
        generated = out / group / "kotor_original_renderer_data.generated.h"
        if header_name != generated.name:
            generated.rename(out / group / header_name)
    resource_pairs(executable, out / "resource-pairs" / "resource_pairs.cpp")
    texture_literal(executable, out / "texture" / "texture_literal.cpp")
    upgrade_slots(executable, out / "upgrade" / "upgrade_slots.cpp")
    class_choices(executable, out / "class-choice" / "class_choices.cpp")
    extra_data(executable, out)
    # run_path loads the reviewed helper without writing a source-tree bytecode cache.
    pow_generator = runpy.run_path(str(HELPERS / "pow_constants.py"))["pow_constants"]
    pow_generator(executable, out / "pow" / "generated_constants.inc", HELPERS / "pow-constants-spec.json")
    (out / "diagnostics").mkdir()
    diagnostic_generator = runpy.run_path(str(HELPERS / "diagnostic_literals.py"))["diagnostic_literals"]
    diagnostic_generator(executable, out / "diagnostics" / "diagnostic_literals.cpp")
    warning_locale = runpy.run_path(str(HELPERS / "warning_locale.py"))["warning_locale"]
    warning_locale(executable, out / "warning-locale", HELPERS / "warning-locale-spec.json")
    helper("controller.py", ["--exe", str(executable), "--out", str(out / "controller" / "controller_rdata.h")], out / "controller.log")
    helper("game_prepare.py", ["--exe", str(executable), "--out", str(out / "game-prepare")], out / "game-prepare.log")
    helper("sound.py", ["--exe", str(executable), "--out", str(out / "sound")], out / "sound.log")
    if rules:
        helper("rules.py", ["--exe", str(executable), "--src", str(rules), "--out", str(out / "rules")], out / "rules.log")
    imports = import_libraries(game, out / "imports", args.lib_exe.resolve(strict=True)) if args.lib_exe else []
    if sha(executable) != EXE_SHA:
        raise ValueError("Original executable changed during generation")
    receipt = {"exe_sha256": EXE_SHA, "pefile_version": pefile.__version__,
               "rules_generated": bool(rules), "import_libraries": imports,
               "scope": "Resources, GL/mesh/VP/epoch/window/controller renderer data, resource-pair descriptors, texture/upgrade data, preparation data, sound data, optional rules data/import libraries; not full dependency closure",
               "generators": {p.name: sha(p) for p in HELPERS.iterdir() if p.is_file()},
               "files": {str(p.relative_to(out)): sha(p) for p in out.rglob("*") if p.is_file()}}
    (out / "generation-receipt.json").write_text(json.dumps(receipt, indent=2), encoding="utf-8")
    print("Generated private local inputs; do not publish the output directory.")


if __name__ == "__main__":
    try:
        main()
    except (ValueError, RuntimeError, OSError, ImportError, KeyError, subprocess.SubprocessError) as exc:
        print(f"Generation stopped: {exc}", file=sys.stderr)
        sys.exit(1)
