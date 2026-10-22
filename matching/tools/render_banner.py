#!/usr/bin/env python3
"""Render matching/assets/progress.svg, the project's progress tracker.

The top half restates the playable build's state from STATUS.md (edit
PLAYABLE when it changes). The bottom half measures the matching track from
matching/status.csv (written by matchcmp.py --report) against
matching/functions.csv, the executable's function starts ("kind" eh marks
compiler-generated EH funclets, which are excluded). Annotated addresses
missing from that list are added to it. Function sizes run from each start
to the next, so byte figures include alignment padding.

The change badge compares matching/status.csv with its previous committed
revision. CI reruns this on every push that changes those files.
"""

from __future__ import annotations

import argparse
import bisect
import csv
import json
import subprocess
from html import escape
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUTPUT = ROOT / "matching" / "assets" / "progress.svg"

TEXT_START, TEXT_END = 0x00401000, 0x0073D000   # .text of the 1.0.3.0 exe
CELL_BYTES = 0x1000
MAP_COLUMNS = 46

# (milestone, detail, state) from STATUS.md; state is done, open or todo.
PLAYABLE = [
    ("Builds", "VS2022 MSVC + clang-cl", "done"),
    ("Endar Spire", "completed in manual play", "done"),
    ("Taris", "apartment reached", "open"),
    ("Minigames", "not yet integrated", "todo"),
    ("Rest of game", "untested", "todo"),
]

BG, BORDER = "#090909", "#343434"
TEXT, MUTED, DIM = "#f4f4f5", "#c9c9c9", "#8a8a8a"
MATCH_COLOR, NEAR_COLOR, PLAY_COLOR = "#5be3a8", "#c9963a", "#6ba8ff"
LIB_COLOR = "#4aa8d8"
LIBRARY = ROOT / "matching" / "crt_libcmt.csv"   # stock LIBCMT.LIB functions
TODO_COLOR, CG_COLOR, PENDING = "#272727", "#4a3f6b", "#3a3a3a"
STATE_COLOR = {"done": PLAY_COLOR, "open": NEAR_COLOR, "todo": PENDING}


def read_status(text: str) -> dict[int, str]:
    return {int(r["address"], 16): r["status"] for r in csv.DictReader(text.splitlines())}


def previous_status(report: Path, current: dict[int, str]) -> dict[int, str] | None:
    """The most recent committed revision of the report that differs from now."""
    try:
        relative = report.resolve().relative_to(ROOT).as_posix()
        history = subprocess.run(["git", "log", "-n", "3", "--format=%H", "--", relative],
                                 cwd=ROOT, capture_output=True, text=True, check=True).stdout.split()
        for rev in history:
            text = subprocess.run(["git", "show", f"{rev}:{relative}"], cwd=ROOT,
                                  capture_output=True, text=True, check=True).stdout
            old = read_status(text)
            if old != current:
                return old
    except (OSError, ValueError, KeyError, subprocess.CalledProcessError):
        pass
    return None


def load_functions(status: dict[int, str], table: Path) -> list[tuple[int, int, str]]:
    """(start, size, category): M byte-identical, L identical to the stock
    VS.NET 2002 LIBCMT.LIB (linked, no source needed), N attempt, T to do, CG."""
    library = set()
    if LIBRARY.exists():
        with LIBRARY.open(encoding="utf-8") as source:
            library = {int(r["address"], 16) for r in csv.DictReader(source)}
    with table.open(encoding="utf-8") as source:
        levels = {int(r["address"], 16): "CG" if r.get("kind") == "eh" else ""
                  for r in csv.DictReader(source)}
    for address in status:
        levels.setdefault(address, "")
    starts = sorted(levels)
    functions = []
    for index, start in enumerate(starts):
        end = starts[index + 1] if index + 1 < len(starts) else TEXT_END
        state = status.get(start)
        category = ("M" if state == "MATCH" else "CG" if levels[start] == "CG"
                    else "L" if start in library else "N" if state else "T")
        functions.append((start, max(0, min(end, TEXT_END) - start), category))
    return functions


def byte_totals(functions, low=0, high=1 << 32) -> dict[str, int]:
    totals = dict.fromkeys(("M", "L", "N", "T", "CG"), 0)
    for start, size, category in functions:
        overlap = min(start + size, high) - max(start, low)
        if overlap > 0:
            totals[category] += overlap
    return totals


def mix(color_a: str, color_b: str, t: float) -> str:
    a = [int(color_a[i:i + 2], 16) for i in (1, 3, 5)]
    b = [int(color_b[i:i + 2], 16) for i in (1, 3, 5)]
    return "#" + "".join(f"{round(x + (y - x) * t):02x}" for x, y in zip(a, b))


def cell_color(totals: dict) -> str:
    game = totals["M"] + totals["L"] + totals["N"] + totals["T"]
    if game == 0:
        return CG_COLOR if totals["CG"] else "#161616"
    if totals["L"] > totals["M"]:
        return mix("#173444", LIB_COLOR, totals["L"] / game)
    if totals["M"]:
        return mix("#1f4a3a", MATCH_COLOR, totals["M"] / game)
    if totals["N"]:
        return mix(TODO_COLOR, NEAR_COLOR, 0.6)
    return TODO_COLOR


def stacked_bar(add, x0, y0, width, height, shares) -> None:
    add(f'<rect x="{x0}" y="{y0}" width="{width}" height="{height}" rx="{height / 2}" fill="{TODO_COLOR}"/>')
    x = x0
    for fraction, color in shares:
        w = width * fraction
        if w > 0:
            add(f'<rect x="{x:.2f}" y="{y0}" width="{max(w, 1.5):.2f}" height="{height}" fill="{color}"/>')
        x += w
    add(f'<rect x="{x0}" y="{y0}" width="{width}" height="{height}" rx="{height / 2}" '
        f'fill="none" stroke="{BG}" stroke-width="2"/>')


def heading(add, y, text) -> None:
    add(f'<text x="23" y="{y}" font-size="13" font-weight="550" fill="#dedede">{escape(text)}</text>')


def render(functions, units: int, old_status: dict[int, str] | None) -> str:
    counts = {c: sum(1 for f in functions if f[2] == c) for c in ("M", "L", "N", "T", "CG")}
    matched, library, near = counts["M"], counts["L"], counts["N"]
    total = matched + library + near + counts["T"]
    code = byte_totals(functions, TEXT_START, TEXT_END)
    game_bytes = code["M"] + code["L"] + code["N"] + code["T"]
    match_pct = matched / total * 100
    byte_pct = code["M"] / game_bytes * 100 if game_bytes else 0.0

    parts: list[str] = []
    add = parts.append
    add('<text x="23" y="34" font-size="15" font-weight="550" fill="#dedede">'
        'Star Wars: Knights of the Old Republic reconstruction — GOG 1.03 swkotor.exe 1.0.3.0</text>')
    add(f'<text x="23" y="54" font-size="12" fill="{DIM}">Unofficial. Bring your own purchased game; '
        f'no original code or assets are distributed.</text>')
    badge, badge_fg, badge_bg = "Tracking", "#a1a1aa", "#222225"
    if old_status is not None:
        diff = matched - sum(1 for v in old_status.values() if v == "MATCH")
        if diff > 0:
            badge, badge_fg, badge_bg = f"+{diff:,} matching", "#33db73", "#062514"
        elif diff < 0:
            badge, badge_fg, badge_bg = f"{diff:,} matching", "#fca5a5", "#371517"
        else:
            badge = "No change"
    badge_w = max(112, 30 + len(badge) * 8.0)
    badge_x = 938 - badge_w
    add(f'<rect x="{badge_x:.1f}" y="17" width="{badge_w:.1f}" height="27" rx="14" fill="{badge_bg}"/>')
    add(f'<text x="{badge_x + badge_w / 2:.1f}" y="35" font-size="13" font-weight="600" '
        f'text-anchor="middle" fill="{badge_fg}">{badge}</text>')

    # Playable build.
    heading(add, 90, f"Playable build (src/) — {units:,} source units, links swkotor-reconstructed.exe")
    step_w = 916 / len(PLAYABLE)
    for i, (title, detail, state) in enumerate(PLAYABLE):
        x0, y0 = 23 + i * step_w, 104
        add(f'<circle cx="{x0 + 9:.1f}" cy="{y0 + 9}" r="9" fill="{STATE_COLOR[state]}"/>')
        mark = "✓" if state == "done" else "…" if state == "open" else ""
        add(f'<text x="{x0 + 9:.1f}" y="{y0 + 13}" font-size="11" font-weight="700" '
            f'text-anchor="middle" fill="{BG}">{mark}</text>')
        add(f'<text x="{x0 + 24:.1f}" y="{y0 + 8}" font-size="12" font-weight="600" fill="{TEXT}">'
            f'{escape(title)}</text>')
        add(f'<text x="{x0 + 24:.1f}" y="{y0 + 23}" font-size="11" fill="{MUTED}">{escape(detail)}</text>')
    add(f'<line x1="23" y1="158" x2="939" y2="158" stroke="{BORDER}" stroke-width="1"/>')

    # Matching track.
    heading(add, 186, "Matching track (matching/) — byte-identical rebuild with MSVC 7.0 "
                      "(Visual Studio .NET 2002, cl 13.00.9466)")
    metrics = [
        (23, f"{match_pct:.1f}%", "of functions byte-identical",
         f"{matched:,} / {total:,} functions · +{library:,} stock LIBCMT"),
        (333, f"{byte_pct:.1f}%", "of code bytes byte-identical", "weighted by function size"),
        (643, f"{near:,}", "attempts not yet identical", "kept for later passes"),
    ]
    for x, value, label, detail in metrics:
        color = NEAR_COLOR if x == 643 else MATCH_COLOR
        add(f'<text x="{x}" y="228" font-size="34" font-weight="700" fill="{color}">{value}</text>')
        add(f'<text x="{x}" y="250" font-size="13" fill="{MUTED}">{escape(label)}</text>')
        add(f'<text x="{x}" y="266" font-size="12" fill="{DIM}">{escape(detail)}</text>')
    stacked_bar(add, 23, 282, 916, 14, [(matched / total, MATCH_COLOR), (library / total, LIB_COLOR),
                                        (near / total, NEAR_COLOR)])

    map_top = 336
    add(f'<text x="23" y="{map_top - 10}" font-size="12" fill="{MUTED}">'
        f'Address map of .text — each cell is 4 KiB of code</text>')
    key = [(MATCH_COLOR, "byte-identical"), (LIB_COLOR, "stock LIBCMT"), (NEAR_COLOR, "attempt"),
           (CG_COLOR, "compiler-generated"), (TODO_COLOR, "not started")]
    kx = 939
    for color, text in reversed(key):
        kx -= len(text) * 6.0 + 30
        add(f'<rect x="{kx:.1f}" y="{map_top - 19}" width="10" height="10" rx="2" fill="{color}"/>')
        add(f'<text x="{kx + 14:.1f}" y="{map_top - 10}" font-size="11" fill="{DIM}">{text}</text>')
    cells = (TEXT_END - TEXT_START) // CELL_BYTES
    rows = (cells + MAP_COLUMNS - 1) // MAP_COLUMNS
    left, pitch_x, pitch_y, size_y = 86, 18.6, 12, 10
    starts = [f[0] for f in functions]
    for index in range(cells):
        row, column = divmod(index, MAP_COLUMNS)
        if column == 0:
            add(f'<text x="{left - 8}" y="{map_top + row * pitch_y + 9}" font-size="10" '
                f'text-anchor="end" fill="{DIM}" font-family="Consolas, Menlo, monospace">'
                f'0x{TEXT_START + index * CELL_BYTES:08X}</text>')
        low = TEXT_START + index * CELL_BYTES
        first = max(0, bisect.bisect_right(starts, low) - 1)
        last = bisect.bisect_left(starts, low + CELL_BYTES)
        totals = byte_totals(functions[first:last], low, low + CELL_BYTES)
        add(f'<rect x="{left + column * pitch_x:.1f}" y="{map_top + row * pitch_y}" '
            f'width="{pitch_x - 2:.1f}" height="{size_y}" rx="2" fill="{cell_color(totals)}"/>')
    foot = map_top + rows * pitch_y + 22
    add(f'<text x="23" y="{foot}" font-size="12" fill="{DIM}">Byte-identical: compiled with MSVC 7.0 and '
        f'compared by matchcmp.py. Stock LIBCMT: identical to VS.NET 2002 LIBCMT.LIB. '
        f'Excludes {counts["CG"]:,} compiler-generated EH funclets.</text>')
    height = foot + 18

    desc = (f"Playable build reaches Taris after completing the Endar Spire. Matching track: "
            f"{matched:,} of {total:,} functions ({match_pct:.1f}%) and {byte_pct:.1f}% of code bytes "
            f"rebuild byte-identical with MSVC 7.0; {library:,} more are identical to the stock "
            f"VS.NET 2002 LIBCMT.LIB.")
    body = "\n    ".join(parts)
    return f'''<svg xmlns="http://www.w3.org/2000/svg" width="960" height="{height}" viewBox="0 0 960 {height}" role="img" aria-labelledby="card-title card-desc">
  <title id="card-title">KOTOR reconstruction progress</title>
  <desc id="card-desc">{escape(desc)}</desc>
  <rect x="1" y="1" width="958" height="{height - 2}" rx="14" fill="{BG}" stroke="{BORDER}" stroke-width="1.5"/>
  <g font-family="-apple-system, BlinkMacSystemFont, Segoe UI, Arial, sans-serif">
    {body}
  </g>
</svg>
'''


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--report", type=Path, default=ROOT / "matching" / "status.csv",
                        help="matchcmp.py --report CSV")
    parser.add_argument("--function-table", type=Path, default=ROOT / "matching" / "functions.csv",
                        help="CSV of function starts (address, kind)")
    parser.add_argument("--out", type=Path, default=OUTPUT)
    args = parser.parse_args()
    manifest = json.loads((ROOT / "build-manifest.json").read_text(encoding="utf-8"))
    units = len(manifest["targets"]["game"]["units"])
    status = read_status(args.report.read_text(encoding="utf-8"))
    functions = load_functions(status, args.function_table)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    content = render(functions, units, previous_status(args.report, status))
    if not args.out.exists() or args.out.read_text(encoding="utf-8") != content:
        args.out.write_text(content, encoding="utf-8")
    print(f"Banner: {sum(1 for f in functions if f[2] == 'M'):,} byte-identical functions -> {args.out}")


if __name__ == "__main__":
    main()
