# Byte-identical decompilation (matching/)

You are in the original-toolchain build: C++ that Visual Studio .NET 2002
(MSVC 7.0) compiles to the exact bytes of the GOG 1.03 `swkotor.exe`. The
modern-toolchain build in `src/` is separate, and matching work never edits
it. Background: [../STRUCTURE.md](../STRUCTURE.md).

Follow [CONTRIBUTING.md](CONTRIBUTING.md) for setup, picking work, the
per-function loop and the pre-commit checks. Rules:

- Edit only `matching/src/<bucket>/` and `matching/include/<bucket>/` for
  the address range you are working on, plus `matching/status.csv`.
- A function counts as done only when
  `python matching/tools/matchcmp.py <file>` prints `OK` for it. Never claim
  a match the tool hasn't confirmed.
- Write C++98-era code. Read `python matching/tools/todo.py --address <addr>`
  for semantics, but don't copy its raw-offset style.
- Regenerate `matching/status.csv` with `--report` before committing. The
  MATCH count must not drop, and no address may be annotated twice.
- Never commit the original exe, compiler files or extracted game data.
