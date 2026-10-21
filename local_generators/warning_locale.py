"""Read warning and locale data only from the supported, locally supplied game."""
from pathlib import Path
import hashlib
import json
import struct

EXE_SHA = "9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435"

def warning_locale(executable: Path, output: Path, spec_path: Path) -> None:
    import pefile
    if hashlib.sha256(executable.read_bytes()).hexdigest() != EXE_SHA:
        raise ValueError("Unsupported original executable")
    groups = {"warning": ["namespace base_ini_reconstruction {"], "locale": ["namespace kotor_locale {"]}
    with pefile.PE(str(executable), fast_load=True) as pe:
        for item in json.loads(spec_path.read_text(encoding="utf-8")):
            count, width = item["count"], item["width"]
            size = count * width
            data = bytes(size) if item["bss"] else pe.get_data(item["address"] - pe.OPTIONAL_HEADER.ImageBase, size)
            if len(data) != size:
                raise ValueError("Truncated original data")
            values = struct.unpack("<" + ("H" if width == 2 else "B") * count, data)
            prefix = item["prefix"]
            if item["group"] == "warning":
                prefix = "extern " + prefix
            groups[item["group"]].append(f"{prefix} {item['name']}[{count}] = {{{','.join(map(str, values))}}};")
    if hashlib.sha256(executable.read_bytes()).hexdigest() != EXE_SHA:
        raise ValueError("Executable changed during generation")
    output.mkdir(parents=True, exist_ok=True)
    for group, lines in groups.items():
        lines.append("}")
        extension = "cpp" if group == "warning" else "inc"
        (output / f"{group}_data.{extension}").write_text("\n".join(lines) + "\n", encoding="utf-8")
