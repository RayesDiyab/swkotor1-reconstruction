#!/usr/bin/env python3
# D13 generator (committable). Scans the lane-rules sources for references to
# kotor_rules_str_XXXXXXXX (original .rdata/.data C strings) and emits:
#   <out>/rules_image_data.h          declarations only (no original bytes)
#   <out>/rules_image_data.cpp        definitions with original bytes (NEVER COMMIT)
#   <out>/IMAGE-CHECK.json            address, file offset, length, sha256 per value
# Usage: python -I gen_rules_image_data.py --exe <swkotor.exe> --src <lane src dir> --out <ignored local dir>
import argparse, hashlib, json, os, re, sys
PINNED_SHA256 = '9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435'
# (vsize, va, raw_offset, raw_size) of .text/.rdata/.data in the pinned image.
SECT = ((0x33c000, 0x401000, 0x1000, 0x33c000), (0x4fbfe, 0x73d000, 0x33d000, 0x50000),
        (0xa8498, 0x78d000, 0x38d000, 0x17000))
NAME = re.compile(r'kotor_rules_str_([0-9a-f]{8})')
CELL = re.compile(r'kotor_rules_cell(8|16|32)_([0-9a-f]{8})')


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--exe', required=True)
    ap.add_argument('--src', required=True)
    ap.add_argument('--out', required=True)
    a = ap.parse_args()
    img = open(a.exe, 'rb').read()
    h = hashlib.sha256(img).hexdigest()
    if h != PINNED_SHA256:
        sys.exit('swkotor.exe sha256 %s != pinned %s' % (h, PINNED_SHA256))
    uses = {}
    cells = {}
    for fn in sorted(os.listdir(a.src)):
        if not fn.endswith(('.cpp', '.h')) or fn == 'rules_image_data.h':
            continue
        for m in NAME.finditer(open(os.path.join(a.src, fn), encoding='utf-8').read()):
            uses.setdefault(int(m.group(1), 16), set()).add(fn)
        for m in CELL.finditer(open(os.path.join(a.src, fn), encoding='utf-8').read()):
            cells.setdefault((int(m.group(2), 16), int(m.group(1))), set()).add(fn)

    def cstr(va):
        for vs, va0, off, rs in SECT[1:]:
            if va0 <= va < va0 + vs:
                rel = va - va0
                if rel >= rs:
                    raise ValueError('%08X in bss' % va)
                end = img.index(b'\0', off + rel)
                return img[off + rel:end + 1], off + rel
        raise ValueError('%08X not in .rdata/.data' % va)

    check, decl, defs = [], [], []
    for va in sorted(uses):
        b, off = cstr(va)
        check.append({'name': 'kotor_rules_str_%08x' % va, 'address': '%08X' % va, 'file_offset': '%X' % off,
                      'length': len(b), 'sha256': hashlib.sha256(b).hexdigest(), 'consumers': sorted(uses[va])})
        decl.append('extern const char kotor_rules_str_%08x[%d];  // %s' % (va, len(b), ', '.join(sorted(uses[va]))))
        defs.append('const char kotor_rules_str_%08x[%d] = {%s};' % (va, len(b), ', '.join('0x%02X' % c for c in b)))
    for (va, bits), fns in sorted(cells.items()):
        n = bits // 8
        for vs, va0, off, rs in SECT[2:]:
            if va0 <= va < va0 + vs:
                rel = va - va0
                b = bytes(img[off + rel + i] if rel + i < rs else 0 for i in range(n))
                src = 'file' if rel + n <= rs else 'bss-zero'
                fo = '%X' % (off + rel) if rel < rs else None
                break
        else:
            raise ValueError('%08X not in .data' % va)
        v = int.from_bytes(b, 'little')
        t = {8: 'std::uint8_t', 16: 'std::uint16_t', 32: 'std::uint32_t'}[bits]
        nm = 'kotor_rules_cell%d_%08x' % (bits, va)
        check.append({'name': nm, 'address': '%08X' % va, 'file_offset': fo, 'length': n, 'source': src,
                      'sha256': hashlib.sha256(b).hexdigest(), 'consumers': sorted(fns)})
        decl.append('extern %s %s;  // %s' % (t, nm, ', '.join(sorted(fns))))
        defs.append('%s %s = 0x%Xu;' % (t, nm, v))
    hdr = ('#pragma once\n// GENERATED declarations (no original bytes) by tools/gen_rules_image_data.py.\n'
           '// Definitions exist ONLY in the locally generated, never-committed rules_image_data.cpp (D13).\n'
           '#include <cstdint>\nextern "C" {\n' + '\n'.join(decl) + '\n}\n')
    os.makedirs(a.out, exist_ok=True)
    hp = os.path.join(a.out, 'rules_image_data.h')
    if not os.path.exists(hp) or open(hp, encoding='utf-8').read() != hdr:
        tmp = hp + '.%d.tmp' % os.getpid()
        open(tmp, 'w', newline='\n').write(hdr)
        os.replace(tmp, hp)
    os.makedirs(a.out, exist_ok=True)
    cpp = ('// GENERATED - NEVER COMMIT (D13). tools/gen_rules_image_data.py from swkotor.exe sha256 %s.\n'
           '#include "rules_image_data.h"\nextern "C" {\n%s\n}\n') % (h, '\n'.join(defs))
    tmp = os.path.join(a.out, 'rules_image_data.cpp.%d.tmp' % os.getpid())
    open(tmp, 'w', newline='\n').write(cpp)
    os.replace(tmp, os.path.join(a.out, 'rules_image_data.cpp'))
    tmp = os.path.join(a.out, 'IMAGE-CHECK.json.%d.tmp' % os.getpid())
    json.dump({'exe_sha256': h, 'values': check}, open(tmp, 'w'), indent=1)
    os.replace(tmp, os.path.join(a.out, 'IMAGE-CHECK.json'))
    open(os.path.join(a.out, 'NEVER-COMMIT'), 'w').write('D13 generated original bytes\n')
    print('strings', len(check))


if __name__ == '__main__':
    main()
