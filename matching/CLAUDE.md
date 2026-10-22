# Byte-identical decompilation (matching/)

You are in the original-toolchain build: C++ that Visual Studio .NET 2002
(MSVC 7.0) compiles to the exact bytes of the GOG 1.03 `swkotor.exe`. The
modern-toolchain build in `src/` is separate, and matching work never edits
it. Background: [../STRUCTURE.md](../STRUCTURE.md).

Follow [CONTRIBUTING.md](CONTRIBUTING.md) for setup, picking work, the
per-function loop and the pre-commit checks. Rules:

- Edit only `matching/`. Working alone, add to the existing file for a
  class. Working in parallel with other agents, stay inside your assigned
  bucket (`matching/src/<bucket>/`, `matching/include/<bucket>/`).
- Set `KOTOR_MSVC7` and `KOTOR_ORIGINAL_EXE` in the same command as each
  tool run, or pass `--msvc`/`--exe`, because your shell may not keep them.
- Name functions and classes from `matching/symbols.csv` and
  `matching/include` before anything else. The tool cannot catch a wrong
  name.
- A function counts as done only when
  `python matching/tools/matchcmp.py <file>` prints `OK` for it. Never claim
  a match the tool hasn't confirmed.
- Write C++98-era code. Read `python matching/tools/todo.py --address <addr>`
  for semantics, but don't copy its raw-offset style.
- Before committing, regenerate `matching/status.csv` (`--report`) and run
  `matching/tools/symbolmap.py`. The MATCH count must not drop, no address
  may be annotated twice, and there should be no new multi-address symbols.
- Never commit the original exe, compiler files or extracted game data.
