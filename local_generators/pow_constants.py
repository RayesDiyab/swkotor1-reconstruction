"""Generate ignored power-function constants from the hash-pinned local game."""
from pathlib import Path
import hashlib
import json

EXE_SHA = '9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435'

def pow_constants(executable: Path, output: Path, spec: Path) -> None:
    import pefile
    if hashlib.sha256(executable.read_bytes()).hexdigest() != EXE_SHA:
        raise ValueError('Unsupported original executable')
    records = json.loads(spec.read_text(encoding='utf-8'))
    lines = ['// Generated original game data. Keep local and ignored; never publish.']
    with pefile.PE(str(executable), fast_load=True) as pe:
        if len(records) != 12:
            raise ValueError('Expected 12 constant objects')
        for record in records:
            symbol, address, size = record['symbol'], record['va'], record['size']
            if symbol != f'task8_pow_const_{address:08x}' or not 0 < size <= 16000:
                raise ValueError('Invalid constant specification')
            payload = pe.get_data(address - pe.OPTIONAL_HEADER.ImageBase, size)
            if len(payload) != size or hashlib.sha256(payload).hexdigest() != record['sha256']:
                raise ValueError('Original constant bytes do not match specification')
            qualifier = 'const ' if record['const'] else ''
            lines.append(f'extern "C" __declspec(align(16)) {qualifier}unsigned char {symbol}[{size}]={{')
            lines.append(','.join(f'0x{byte:02x}' for byte in payload))
            lines.append('};\n')
    if hashlib.sha256(executable.read_bytes()).hexdigest() != EXE_SHA:
        raise ValueError('Executable changed during generation')
    output.write_text('\n'.join(lines), encoding='utf-8', newline='\n')

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--game-dir', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    game = args.game_dir.resolve(strict=True)
    out = args.out.resolve()
    if out.exists() or out.is_relative_to(game) or game.is_relative_to(out):
        raise ValueError('Output must be a new directory separate from the installed game')
    out.mkdir(parents=True)
    (out / '.gitignore').write_text('*\n', encoding='ascii')
    (out / 'pow').mkdir()
    pow_constants(game / 'swkotor.exe', out / 'pow/generated_constants.inc', Path(__file__).with_name('pow-constants-spec.json'))
