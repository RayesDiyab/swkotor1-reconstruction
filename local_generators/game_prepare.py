#!/usr/bin/env python3
# D13 generator (committed). Produces game-prepare/prepare_image_data.cpp into an
# IGNORED local directory from the pinned original swkotor.exe; the generated file
# contains original .data/.rdata bytes and must NEVER be committed. Also writes
# IMAGE-CHECK.json (address, file offset, length, sha256 of every emitted value).
# Usage: python -I gen_prepare_image_data.py --exe <swkotor.exe> --out <ignored dir>
import argparse, hashlib, json, os, sys
PINNED_SHA256 = '9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435'
# (vsize, va, raw_offset, raw_size) of .text/.rdata/.data in the pinned image.
SECT = ((0x33c000, 0x401000, 0x1000, 0x33c000), (0x4fbfe, 0x73d000, 0x33d000, 0x50000),
        (0xa8498, 0x78d000, 0x38d000, 0x17000))

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--exe', required=True)
    ap.add_argument('--out', required=True)
    a = ap.parse_args()
    img = open(a.exe, 'rb').read()
    h = hashlib.sha256(img).hexdigest()
    if h != PINNED_SHA256:
        sys.exit('swkotor.exe sha256 %s != pinned %s' % (h, PINNED_SHA256))
    check = []

    def raw(va, n):
        for vs, va0, off, rs in SECT:
            if va0 <= va < va0 + vs:
                rel = va - va0
                b = bytes(img[off + rel + i] if rel + i < rs else 0 for i in range(n))
                return b, (off + rel if rel < rs else None), ('file' if rel + n <= rs else 'bss-zero')
        raise ValueError(hex(va))

    def record(field, va, b, off, src, emitted, evidence=''):
        check.append({'field': field, 'address': '%08X' % va, 'file_offset': None if off is None else '%X' % off,
                      'length': len(b), 'image_bytes_le': b.hex(), 'sha256': hashlib.sha256(b).hexdigest(),
                      'source': src, 'emitted': emitted, 'evidence': evidence})

    def num(field, va, n, kind='u'):
        b, off, src = raw(va, n)
        v = int.from_bytes(b, 'little')
        if kind == 'f':
            if v != 0: raise ValueError('nonzero float needs bit-exact emission at %x' % va)
            lit = '0.0f'
        else:
            lit = {'u': '0x%Xu' % v, 'u64': '0x%XULL' % v, 'i': '%d' % (v - (1 << 32) if v >= 1 << 31 else v), 'c': '%d' % v}[kind]
        record(field, va, b, off, src, lit)
        return lit

    def cstr_bytes(va):
        b, off, src = raw(va, 256)
        s = b.split(b'\0')[0] + b'\0'
        record('string %08X' % va, va, s, off, src, 'char[%d]' % len(s))
        return s

    def chars(b): return ', '.join('0x%02X' % c for c in b)

    W = [('quit', 0x7a39b8, 4, 'u'), ('argc', 0x833bfc, 4, 'u'), ('argv', 0x833c00, 4, 'u'), ('callback', 0x78e39c, 4, 'u'),
         ('priority_changed', 0x7a3ca4, 4, 'u'), ('priority_mode', 0x7a3ca0, 4, 'u'),
         ('width', 0x78d1d4, 4, 'u'), ('depth', 0x78d1e0, 1, 'c'), ('headlight', 0x7a3c50, 4, 'u'), ('frame_word', 0x7a3c5c, 4, 'u'),
         ('bias', 0x7b9274, 4, 'f'), ('client_ms', 0x7a3c7c, 4, 'f'), ('server_ms', 0x7a3c80, 4, 'f'), ('cap', 0x7a3c64, 4, 'f'),
         ('start', 0x7a39c8, 8, 'u64'), ('end', 0x7a39b0, 8, 'u64'), ('cap_start', 0x7a3c68, 8, 'u64'), ('cap_end', 0x7a3c70, 8, 'u64'),
         ('debug', 0x7a3a40, 4, 'u'), ('deferred', 0x7a3a48, 4, 'u'), ('position', 0x7a3a44, 4, 'u'), ('text', 0x7a3a4c, 1, 'c'),
         ('swapping', 0x7a3c9c, 4, 'u'), ('sleep_enabled', 0x7a3c58, 4, 'u'), ('sleep_ms', 0x78d1e8, 4, 'u'),
         ('mouse_trails', 0x7a3ca8, 4, 'i'), ('screen_saver', 0x7a3cac, 4, 'i')]
    SH = [0x78d454, 0x78d474, 0x78d45c, 0x78d458, 0x78d448, 0x78d44c]
    STR = [('graphics_version', 0x7a3cc4), ('title', 0x7a3ccc), ('caption_prefix', 0x7a3cdc)]
    RI = [('guard', 0x7a3a3c), ('mode', 0x78d1dc)]
    MI = [('movie_fullscreen', 0x7a3a10), ('movie_inactive', 0x7a3a18), ('focus', 0x78d1d0), ('last_tick', 0x7a3c4c),
          ('movie_window', 0x7a3cf8), ('flag', 0x8338f0)]
    # 007A3A08 (Tlk) is owned by lane-tlk (D44), 007A39EC/0074C5F8/00832894 by lane-sound (D25).
    CELLS = [('void*', 'kotor_cell_interface_007a39f0', 0x7a39f0, 4, '005F8B14'),
             ('void*', 'kotor_cell_tlk_client_007a3a0c', 0x7a3a0c, 4, '005F89BB'),
             ('std::uint32_t', 'kotor_cell_allow_windowed_007a3a30', 0x7a3a30, 4, '005F873F'),
             ('unsigned char', 'kotor_cell_logging_008328e1', 0x8328e1, 1, '005F88BA'),
             ('void*', 'kotor_cell_rules_007a3a28', 0x7a3a28, 4, '005F522A/005F5266')]
    # Original .rdata strings consumed by lane-prepare units (name, VA, consumer evidence).
    STRINGS = [(0x73d648, 'ReInitAurora 0040396A/00403AAC'), (0x73d6ac, 'ReInitAurora 00403955/00403A97'),
               (0x73d6c0, 'ReInitAurora 00403942'), (0x73d694, 'ReInitAurora 00403A89'),
               (0x73d6e0, 'ReInitAurora 0040389F (FindWindowA class)'),
               (0x73d634, 'initCinematicBackdrop 00401A8A'), (0x73d62c, 'initCinematicBackdrop 00401AC2'),
               (0x73d610, 'initCinematicBackdrop 00401B2B/00401B30'),
               (0x752db0, 'CSWGuiLoadScreen 0067A79A'), (0x752d78, 'CSWGuiLoadScreen 0067A910'),
               (0x748cb4, 'CSWRules 00552CFC'), (0x746f28, 'CSWRules 00552D33'), (0x746f38, 'CSWRules 00552D88'),
               (0x748ca8, 'CSWRules 00552F60'), (0x7451d4, 'CSWRules 00552F90'), (0x748a24, 'CSWRules::LoadDifficultyInfo 00550C92')]

    wm = ['    /* %-16s %08X */ %s' % (n, va, num('winmain.' + n, va, s, k)) for n, va, s, k in W]
    strs = ['    /* %-16s %08X loader image, before staticInit */ {%s, %s}' % (n, va, num('shell.%s.data' % n, va, 4), num('shell.%s.length' % n, va + 4, 4)) for n, va in STR]
    sh = ['    /* %08X */ %s' % (va, num('shell.enable_%06x' % va, va, 4)) for va in SH]
    ri = ['    /* %-6s %08X */ %s' % (n, va, num('reinit.' + n, va, 4)) for n, va in RI]
    mi = ['    /* %-16s %08X */ %s' % (n, va, num('message.' + n, va, 4)) for n, va in MI]
    rc, off, src = raw(0x73d684, 14); record('shell_constants.render_class', 0x73d684, rc, off, src, 'char[14]', '00403708..00403736')
    top = cstr_bytes(0x78d200)
    bs = num('shell_constants.blend_source', 0x73f270, 4); bd = num('shell_constants.blend_destination', 0x73f274, 4)
    cl = []
    for t, n, va, s, ev in CELLS:
        b, o, sr = raw(va, s); v = int.from_bytes(b, 'little')
        if t == 'void*' and v != 0: raise ValueError('nonzero pointer cell %x' % va)
        lit = 'nullptr' if t == 'void*' else '0x%Xu' % v
        record(n, va, b, o, sr, lit, ev)
        cl.append('%s %s = %s; // %08X (%s); %s' % (t, n, lit, va, sr, ev))
    sl = []
    for va, ev in STRINGS:
        s = cstr_bytes(va)
        sl.append('const char kotor_rdata_str_%08x[%d] = {%s}; // %s' % (va, len(s), chars(s), ev))

    out = '''// GENERATED - NEVER COMMIT (D13). Produced by game-prepare/tools/gen_prepare_image_data.py
// from swkotor.exe sha256 %s; every value's address, file offset,
// length and sha256 are in IMAGE-CHECK.json next to this file.
#include "game_prepare_producers.h"
#include "prepare_image_data.h"
#include <cstddef>
#include <cstdint>
// Positional initializers below are only valid for the D25 Initial layout
// (no disable_sound member): a header mismatch must fail to compile.
static_assert(offsetof(kotor_menu_live_views::WinMainInitial, priority_changed) == 16, "D25 WinMainInitial layout");
static_assert(sizeof(kotor_menu_live_views::ShellInitial) == 48, "ShellInitial layout");

extern "C" {
const kotor_menu_live_views::WinMainInitial kotor_image_winmain_initial = {
%s
};
const kotor_menu_live_views::ShellInitial kotor_image_shell_initial = {
%s,
%s
};
const kotor_menu_live_views::ReInitInitial kotor_image_reinit_initial = {
%s
};
const kotor_menu_live_views::MessageInitial kotor_image_message_initial = {
%s
};
kotor_window_shell::Constants kotor_shell_constants = {
    kotor_rdata_str_0078d200,
    {%s},
    %s, %s
};
%s
const char kotor_rdata_str_0078d200[%d] = {%s}; // InitGameApp 004040C0/004040F5 class name
%s
}
''' % (PINNED_SHA256, ',\n'.join(wm), ',\n'.join(strs), ',\n'.join(sh), ',\n'.join(ri), ',\n'.join(mi),
       chars(rc), bs, bd, '\n'.join(cl), len(top), chars(top), '\n'.join(sl))
    os.makedirs(a.out, exist_ok=True)
    open(os.path.join(a.out, 'prepare_image_data.cpp'), 'w', newline='\r\n').write(out)
    open(os.path.join(a.out, 'NEVER-COMMIT.txt'), 'w').write('Generated from original game bytes (D13). Do not commit, publish or copy into the repository.\n')
    open(os.path.join(a.out, '.gitignore'), 'w').write('*\n')
    json.dump({'exe_sha256': h, 'generator': os.path.abspath(__file__), 'values': check},
              open(os.path.join(a.out, 'IMAGE-CHECK.json'), 'w'), indent=1)
    print('generated', len(check), 'values')

if __name__ == '__main__':
    main()
