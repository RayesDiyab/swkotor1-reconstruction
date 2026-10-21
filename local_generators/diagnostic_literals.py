"""Generate original diagnostic literals locally; generated output must stay private."""
from pathlib import Path
import hashlib

EXE_SHA = "9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435"
LITERALS = (
    ("diagnostic_source_0074e208", 0x0074E208, 92),
    ("diagnostic_empty_0074e360", 0x0074E360, 128),
    ("diagnostic_position_0074e2a4", 0x0074E2A4, 128),
    ("diagnostic_at_pos_0074e264", 0x0074E264, 64),
)


def diagnostic_literals(executable: Path, output: Path) -> None:
    import pefile
    if hashlib.sha256(executable.read_bytes()).hexdigest() != EXE_SHA:
        raise ValueError("Unsupported original executable")
    lines = ["// Generated original game data: keep local and ignored.",
             "namespace base_ini_reconstruction {"]
    with pefile.PE(str(executable), fast_load=True) as pe:
        for symbol, address, bound in LITERALS:
            data = pe.get_data(address - pe.OPTIONAL_HEADER.ImageBase, bound)
            terminator = data.find(b"\0")
            if terminator < 1 or any(byte >= 128 for byte in data[:terminator]):
                raise ValueError("Invalid bounded original diagnostic literal")
            data = data[:terminator + 1]
            values = ",".join(str(byte) for byte in data)
            lines.append(f"extern const char {symbol}[] = {{{values}}};")
    if hashlib.sha256(executable.read_bytes()).hexdigest() != EXE_SHA:
        raise ValueError("Executable changed during generation")
    lines.append("}")
    output.write_text("\n".join(lines) + "\n", encoding="utf-8")
