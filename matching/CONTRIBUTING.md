# Contributing byte-identical code

This guide is for people and AI coding agents adding to the byte-identical
decompilation in `matching/`. It is the original-toolchain build described
in [STRUCTURE.md](../STRUCTURE.md); the general rules in
[CONTRIBUTING.md](../CONTRIBUTING.md) apply as well.

## 1. Setup

You need three things that this repository does not ship:

- **Visual Studio .NET 2002 (MSVC 7.0, `cl` 13.00.9466)** from your own
  licensed copy. Use the `Vc7` folder, which contains `BIN\CL.EXE`,
  `INCLUDE` and `LIB`. Later versions (2003 and up) produce different code,
  and no other version will match.
- **Your purchased GOG 1.03 `swkotor.exe`**, SHA-256
  `9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435`.
  Keep it outside the checkout.
- **Python 3 with `pefile`**: `python -m pip install pefile==2024.8.26`.

Point the tools at the first two once per shell:

```powershell
$env:KOTOR_MSVC7 = "C:\Tools\msvc7"          # folder with BIN, INCLUDE, LIB
$env:KOTOR_ORIGINAL_EXE = "D:\Games\Star Wars - KotOR\swkotor.exe"
```

On Linux or macOS the compiler runs under Wine (`KOTOR_WINE` names the
launcher), and paths are handled for you.

**Check the setup.** The result should equal the number of MATCH rows in
`matching/status.csv`, which was 5,251 / 5,755 when this guide was written:

```powershell
python .\matching\tools\matchcmp.py -q matching\src
```

## 2. Pick work

```powershell
python .\matching\tools\todo.py                      # smallest functions with no source yet
python .\matching\tools\todo.py --range 0x5E0000 0x620000 -n 50
python .\matching\tools\todo.py --near               # attempts closest to matching
```

Work in class clusters (a constructor, destructor and their methods
together), since they share headers. When several people or agents work at
once, give each one bucket so nobody edits the same files:

| Bucket | Address range | Bucket | Address range |
| --- | --- | --- | --- |
| d | 0x00400000–0x0041FFFF | f | 0x00580000–0x005CFFFF |
| g | 0x00420000–0x0044FFFF | j | 0x005D0000–0x005DFFFF |
| h | 0x00450000–0x0047FFFF | c | 0x005E0000–0x0061FFFF |
| e | 0x00480000–0x0049FFFF | n | 0x00620000–0x0063FFFF |
| i | 0x004A0000–0x004CFFFF | m | 0x00640000–0x0067FFFF |
| a | 0x004D0000–0x0051FFFF | k | 0x00680000–0x0069FFFF |
| b | 0x00520000–0x0057FFFF | l | 0x006A0000–0x006EFFFF |

Your sources go in `matching/src/<bucket>/` and your headers in
`matching/include/<bucket>/`. Copy declarations you need from other buckets
into your own headers; don't edit another bucket's files. Before annotating
an address, check `matching/status.csv` to make sure nobody has it already.
Functions at 0x006F0000 and up are mostly the C runtime, which is the stock
`LIBCMT.LIB` (`matching/crt_libcmt.csv`). Those need no source.

## 3. Match one function

1. **Understand it.** `todo.py --address 0x005B2EF0` prints the
   modern-toolchain version from `src/`. It's play-tested and cites the
   original instruction addresses, so read it for what the function does,
   member offsets, callees and exception-handling notes. Its raw-offset
   style (`wr32(self, 0x04, 0)`) never compiles to the original bytes, so
   don't paste it. Write a stub with the right name and run `matchcmp -v` to
   see the original disassembly.
2. **Write it in 2002 style:**
   - real classes with members at their original offsets, using padding
     arrays where a member is unknown
   - virtual functions in vtable order
   - callees declared but not defined
   - `__thiscall` member functions
   - nothing past C++98: no `nullptr`, `auto`, `<cstdint>` or `constexpr`.
     MSVC 7.0 rejects them.
3. **Annotate** the definition:
   ```cpp
   // FUNCTION: SWKOTOR 0x005B2EF0
   CSWBaseItem::CSWBaseItem() { ... }
   ```
   Add `SYMBOL: <decorated name>` when the name alone is ambiguous
   (templates, overloads, compiler-generated functions).
4. **Iterate** with
   `python .\matching\tools\matchcmp.py -v matching\src\<bucket>\<file>.cpp`
   until it prints `OK`. Judge progress by the "diff insns" count, and see
   the source-shape notes in [README.md](README.md).
5. **Timebox** each function to about 20 minutes or 15 compiles. If it still
   doesn't match, keep the best attempt annotated (it counts as NONMATCH) and
   move on. Most remaining misses are register-allocation or stack-slot ties
   that a later pass can revisit.

## 4. Before you commit

1. Re-run the whole track and refresh the report:
   ```powershell
   python .\matching\tools\matchcmp.py -q --report matching\status.csv matching\src
   ```
   - The MATCH count must not go down.
   - Fix any "annotated twice" line before committing.
2. Optionally refresh the bridge map:
   `python .\matching\tools\symbolmap.py`.
3. Commit the sources, headers and `matching/status.csv` together. Don't
   commit `matching/assets/progress.svg`: CI redraws it after the merge.
4. Never commit `swkotor.exe`, compiler files, object files or anything
   extracted from the game. Don't edit `src/` in a matching change.

## Reviewing a matching change

- The diff touches only `matching/`, and `status.csv` was regenerated.
- Re-run `matchcmp.py -q` on the changed folders. Every function the change
  claims as matched must print `OK` with your own exe and compiler.
- No address is annotated twice, and the total MATCH count did not drop.
