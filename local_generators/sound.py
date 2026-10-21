# D13 local generator: extracts the original .rdata items the sound donors
# require from the user's own swkotor.exe and writes a LOCAL C++ unit plus a
# receipt (exe path+sha, VA, file offset, length, sha256). Never committed.
import argparse, hashlib, json, struct, os
ap = argparse.ArgumentParser()
ap.add_argument('--exe', required=True)
ap.add_argument('--out', required=True)
args = ap.parse_args()
EXE = args.exe
PIN = '9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435'
OUT = args.out
d = open(EXE, 'rb').read()
if hashlib.sha256(d).hexdigest() != PIN:
    raise ValueError('exe sha mismatch')
pe = struct.unpack_from('<I', d, 0x3c)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]; so = struct.unpack_from('<H', d, pe + 20)[0]
secs = []
for i in range(ns):
    o = pe + 24 + so + 40 * i; vs, va, rs, rp = struct.unpack_from('<IIII', d, o + 8); secs.append((va, max(vs, rs), rp))
def off(v):
    r = v - 0x400000
    for va, sz, rp in secs:
        if va <= r < va + sz: return rp + r - va
    raise ValueError(hex(v))
STRINGS = [('sound_stream_play29_required', 'format_0074c304', 0x74C304),
           ('sound_stream_play29_required', 'format_0074a808', 0x74A808),
           ('sound_stream_play29_required', 'format_0074a7f4', 0x74A7F4),
           ('sound_stream_play29_required', 'format_0074d33c', 0x74D33C)]
FLOATS = [('sound_volume29_required', 'one_0073d764', 0x73D764),
          ('sound_volume29_required', 'scale_0073ee00', 0x73EE00)]
os.makedirs(OUT, exist_ok=True)
lines = ['// GENERATED LOCALLY from the user\'s swkotor.exe (sha256 %s) by gen_sound_data.py.' % PIN,
         '// D13: original .rdata bytes; do not commit or publish.', '']
receipt = {'exe': EXE, 'exe_sha256': PIN, 'items': []}
for ns_, name, va in STRINGS:
    o = off(va); e = d.index(b'\0', o); raw = d[o:e + 1]
    body = ', '.join('0x%02x' % b for b in raw)
    lines.append('namespace %s { extern const char %s[%d] = { %s }; }' % (ns_, name, len(raw), body))
    receipt['items'].append({'symbol': '%s::%s' % (ns_, name), 'va': '%08X' % va, 'file_offset': o, 'length': len(raw), 'sha256': hashlib.sha256(raw).hexdigest(), 'text': raw[:-1].decode('latin1')})
for ns_, name, va in FLOATS:
    o = off(va); raw = d[o:o + 4]; bits = struct.unpack('<I', raw)[0]
    lines.append('namespace %s { extern const float %s = %rf; static_assert(sizeof(float) == 4, "x86"); } // bits %08X' % (ns_, name, struct.unpack('<f', raw)[0], bits))
    receipt['items'].append({'symbol': '%s::%s' % (ns_, name), 'va': '%08X' % va, 'file_offset': o, 'length': 4, 'sha256': hashlib.sha256(raw).hexdigest(), 'bits': '%08X' % bits})
BLOBS = [('kotor_sound_data', 'eax_presets_0074c610', 0x74C610, 24 * 0x70)]
for ns_, name, va, n in BLOBS:
    o = off(va); raw = d[o:o + n]
    body = ', '.join('0x%02x' % b for b in raw)
    lines.append('namespace %s { alignas(4) extern const unsigned char %s[%d] = { %s }; }' % (ns_, name, n, body))
    receipt['items'].append({'symbol': '%s::%s' % (ns_, name), 'va': '%08X' % va, 'file_offset': o, 'length': n, 'sha256': hashlib.sha256(raw).hexdigest()})
open(os.path.join(OUT, 'sound_data.cpp'), 'w').write('\n'.join(lines) + '\n')
json.dump(receipt, open(os.path.join(OUT, 'DATA-RECEIPT.json'), 'w'), indent=1)
print('Generated local sound data.')
