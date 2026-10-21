"""D13 extractor: read-only extraction of original .data/.rdata objects from the
user's own pinned swkotor.exe into a LOCAL generated header.

usage: python extract_original_data.py SPEC.json OUT_DIR [EXE]
SPEC: [{"name": "arb_vertex_0078f600", "va": "0078f600", "kind": "cstring"|"bytes", "size": N?}]
  cstring: size = strlen+1 (NUL included), verified NUL inside the raw section.
  bytes:   exactly `size` bytes (must be raw-initialized, not loader-zero tail).
Writes OUT_DIR/kotor_original_renderer_data.generated.h (contains original bytes;
NEVER commit/publish) and OUT_DIR/EXTRACT-MANIFEST.json (offsets, lengths,
sha256 of each object; no data). The exe is opened read-only and never executed.
"""
import hashlib, json, os, struct, sys
PINNED_EXE = None
PINNED_SHA = "9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435"
BASE = 0x400000


def sections(b):
    pe = struct.unpack_from('<I', b, 0x3c)[0]
    ns = struct.unpack_from('<H', b, pe + 6)[0]
    opt = struct.unpack_from('<H', b, pe + 20)[0]
    out = []
    for i in range(ns):
        o = pe + 24 + opt + 40 * i
        name = b[o:o + 8].rstrip(b'\0').decode()
        vs, va, rs, ro = struct.unpack_from('<IIII', b, o + 8)
        out.append((name, va, vs, ro, rs))
    return out


def locate(secs, va):
    rva = va - BASE
    for name, sva, vs, ro, rs in secs:
        if sva <= rva < sva + max(vs, rs):
            off = rva - sva
            if off >= rs:
                raise SystemExit(f'{va:08x} lies in the loader-zero tail of {name}; nothing to extract')
            return name, ro + off, ro + rs
    raise SystemExit(f'{va:08x} outside every section')


def main():
    spec_path, out_dir = sys.argv[1], sys.argv[2]
    exe = sys.argv[3]
    with open(exe, 'rb') as f:
        b = f.read()
    exe_sha = hashlib.sha256(b).hexdigest()
    if exe_sha != PINNED_SHA:
        raise SystemExit(f'exe sha256 {exe_sha} != pinned {PINNED_SHA}')
    secs = sections(b)
    spec = json.load(open(spec_path))
    os.makedirs(out_dir, exist_ok=True)
    lines = ['// LOCAL GENERATED FILE (decision D13). Contains original swkotor.exe bytes.',
             '// Never commit or publish. Regenerate with tools/extract_original_data.py.',
             f'// exe sha256 {exe_sha}']
    manifest = {'exe_path': exe, 'exe_sha256': exe_sha, 'spec': os.path.abspath(spec_path), 'objects': []}
    for item in spec:
        va = int(item['va'], 16)
        sec, off, end = locate(secs, va)
        if item['kind'] == 'cstring':
            nul = b.index(b'\0', off, end)
            data = b[off:nul + 1]
            if 'size' in item and item['size'] != len(data):
                raise SystemExit(f"{item['name']}: size {len(data)} != spec {item['size']}")
            parts = [chr(c) if 32 <= c < 127 and chr(c) not in '"\\?' else f'\\{c:03o}' for c in data[:-1]]
            chunks = ['"' + ''.join(parts[i:i + 256]) + '"' for i in range(0, len(parts), 256)] or ['""']
            body = '\n    '.join(chunks)
            lines.append(f'KOTOR_ORIGINAL_CSTRING({item["name"]}, {len(data)},\n    {body})')
        elif item['kind'] == 'bytes':
            n = item['size']
            if off + n > end:
                raise SystemExit(f"{item['name']}: extends past raw data")
            data = b[off:off + n]
            lines.append(f'KOTOR_ORIGINAL_BYTES({item["name"]}, {n}, {{{",".join(str(x) for x in data)}}})')
        else:
            raise SystemExit('unknown kind')
        manifest['objects'].append({'name': item['name'], 'va': f'{va:08X}', 'section': sec,
                                    'file_offset': f'0x{off:X}', 'length': len(data),
                                    'sha256': hashlib.sha256(data).hexdigest(), 'kind': item['kind']})
    with open(os.path.join(out_dir, 'kotor_original_renderer_data.generated.h'), 'w', newline='\n') as f:
        f.write('\n'.join(lines) + '\n')
    json.dump(manifest, open(os.path.join(out_dir, 'EXTRACT-MANIFEST.json'), 'w'), indent=1)
    print('extracted', len(manifest['objects']))


if __name__ == '__main__':
    main()
