"""D13 local generator: extracts original .rdata tables used by the controller bodies from the user's own
swkotor.exe into an ignored local header (never committed/published). Records offsets/lengths/sha256."""
import argparse, hashlib, json, struct, sys
from pathlib import Path
parser = argparse.ArgumentParser(); parser.add_argument("--exe", type=Path, required=True); parser.add_argument("--out", type=Path, required=True); args = parser.parse_args()
EXE_SHA = "9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435"
EXE = args.exe; OUT = args.out
b = EXE.read_bytes()
if hashlib.sha256(b).hexdigest() != EXE_SHA: raise ValueError('Unsupported original executable')
pe = struct.unpack_from('<I', b, 0x3c)[0]; n = struct.unpack_from('<H', b, pe + 6)[0]; osz = struct.unpack_from('<H', b, pe + 20)[0]
sec = pe + 24 + osz
def off(va):
    rva = va - 0x400000
    for i in range(n):
        s = sec + 40 * i; vs, va_, rs, rp = struct.unpack_from('<IIII', b, s + 8)
        if va_ <= rva < va_ + rs: return rp + rva - va_
TABLES = [("cosine_lut_00743fe8", 0x743FE8, 32), ("sine_lut_00744070", 0x744070, 32), ("lowpass_kernel_0078e450", 0x78E450, 9)]
BYTES = [("lowpass_shift_0078e474_bytes", 0x78E474, 9)]
lines = ["#pragma once", "// GENERATED LOCALLY (D13) from the user's swkotor.exe sha256 %s. Do not commit or publish." % EXE_SHA, "#include <cstdint>"]
rec = {"exe": str(EXE), "exe_sha256": EXE_SHA, "tables": []}
for name, va, count in TABLES:
    o = off(va); raw = b[o:o + 4 * count]
    words = struct.unpack('<%dI' % count, raw)
    lines.append("constexpr std::uint32_t %s_bits[%d]={%s};" % (name, count, ",".join("0x%08Xu" % w for w in words)))
    rec["tables"].append({"name": name, "va": "%08X" % va, "file_offset": o, "length": 4 * count, "sha256": hashlib.sha256(raw).hexdigest()})
for name, va, count in BYTES:
    o = off(va); raw = b[o:o + count]
    lines.append("constexpr std::uint8_t %s[%d]={%s};" % (name, count, ",".join("0x%02Xu" % x for x in raw)))
    rec["tables"].append({"name": name, "va": "%08X" % va, "file_offset": o, "length": count, "sha256": hashlib.sha256(raw).hexdigest()})
OUT.parent.mkdir(parents=True, exist_ok=True)
OUT.write_text("\n".join(lines) + "\n")
(OUT.parent / "controller_rdata.json").write_text(json.dumps(rec, indent=1))
print("ok", OUT)
