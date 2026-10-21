// 0886-gates (Claude): faithful bodies for former required(0x...) gates of the 0886 cohort unit on the party,
// area-map and effect-duration paths (party_native.inc PartyUpdate/PartyPosition ports, effect_apply_native_faction.inc).
// Transcribed from the original swkotor.exe (read-only); addresses in comments are original instructions.
// ABI: original __thiscall members are exported as extern "C" __fastcall(self, unused_edx, stack args...):
//   004AE1B0 CWorldTimer::GetCalendarDayFromSeconds(float seconds)              RET 4
//   004AE200 CWorldTimer::GetTimeOfDayFromSeconds(float seconds)                RET 4
//   00503750 CGameEffect::SetExpiryTime(ulong day, ulong time)                  RET 8
//   004F7650 CSWSCreature::IsPartyVisible?(void) [funcs.json name]               RET 0
//   00578E00 CSWSAreaMap::GetMapPixelFromWorldCoord(Vector, int* x, int* y)      RET 14h
//   00578F10 CSWSAreaMap::GetMapExploredCoordFromPixel(int x, int y, int*, int*) RET 10h
//   00579000 CSWSAreaMap::SetMapPointExplored(int x, int y)                      RET 8
//   005790C0 CSWSAreaMap::SetPartyMemberWorldLocation(int index, Vector)         RET 10h
//   00579130 CSWSAreaMap::SetPartyMemberWorldOrientation(int index, Vector)      RET 10h
//   005792D0 CSWSAreaMap::SetWorldPointExplored(Vector, int radius)              RET 10h
// x87: the originals run at 53-bit precision; intermediates are double, float exactly where the original stores
// a dword (fstp dword) or loads one. __ftol2 is the existing x87 provider w2::ftol2_006fae8c.
// .rdata bound to proven values: [0073D770]=4294967296.0f [0073D700]=0.0f [0073D6FC]=1000.0f [0073E9AC]=0.5f
//   [00740CBC]=-1.0f [007455D4]=256.0f [00747748]=440.0f [0073E408]=180.0f [0073E40C]=90.0f [0073E404]=270.0f
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(a); }
inline U word(const void* p) { return reinterpret_cast<U>(p); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
inline float rf(U a) { float v; std::memcpy(&v, ptr(a), 4); return v; }
inline void wf(U a, float v) { std::memcpy(ptr(a), &v, 4); }
inline U rd8(U a) { return *static_cast<const unsigned char*>(ptr(a)); }
struct Vec { float x, y, z; };
// fild dword [x]; test; jge; fadd [0073D770]: unsigned dword as an exact double
inline double unsigned_dword(U value) { return static_cast<double>(value); }
}

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }    // [007A39FC] CAppManager*
namespace w2 { std::uint32_t ftol2_006fae8c(double value); }                     // 006FAE8C __ftol2 (w2_ftol_native)

extern "C" {
// ---------------- existing providers (c14 map, exact decorated arity) ----------------
void* __fastcall kotor_fastcall_005ed8b0(void* client, void*);                  // 005ED8B0 CClientExoApp::GetSWParty (0707)
void* __fastcall kotor_fastcall_004ae770(void* server, void*, U id);            // 004AE770 GetCreatureByGameObjectID (0595)
double __cdecl kotor_m15a_yaw_004aa0f0(const void* vector);                      // 004AA0F0 Yaw (naked, ST0 result)

// ---------------- bodies defined here ----------------
U __fastcall kotor_fastcall_004ae1b0(void* timer, void*, float seconds);
U __fastcall kotor_fastcall_004ae200(void* timer, void*, float seconds);
void __fastcall kotor_fastcall_00503750(void* effect, void*, U day, U time);
U __fastcall kotor_fastcall_004f7650(void* creature, void*);
U __fastcall kotor_fastcall_00578e00(void* map, void*, Vec world, I* x, I* y);
U __fastcall kotor_fastcall_00578f10(void* map, void*, I x, I y, I* out_x, I* out_y);
void __fastcall kotor_fastcall_00579000(void* map, void*, I x, I y);
void __fastcall kotor_fastcall_005790c0(void* map, void*, I index, Vec world);
void __fastcall kotor_fastcall_00579130(void* map, void*, I index, Vec direction);
void __fastcall kotor_fastcall_005792d0(void* map, void*, Vec world, I radius);
}

// ===================================================================================== 004AE1B0
// CWorldTimer::GetCalendarDayFromSeconds: whole days (length = unsigned [timer+40h], rounded to float) in seconds.
U __fastcall kotor_fastcall_004ae1b0(void* timer, void*, float seconds) {
    const float length = static_cast<float>(unsigned_dword(rd(word(timer) + 0x40)));  // 004AE1B1..004AE1C1 fstp dword [esp]
    if (seconds < length) return 0;                                                  // 004AE1C5..004AE1D7 (ordered less)
    double rest = seconds;                                                           // 004AE1DA
    U days = 0;                                                                      // 004AE1DE
    do {
        rest -= length;                                                              // 004AE1E0
        ++days;                                                                      // 004AE1E4
    } while (rest >= length);                                                        // 004AE1E5..004AE1EE (C0 clear; NaN exits)
    return days;                                                                     // 004AE1F0..004AE1F5
}

// ===================================================================================== 004AE200
// CWorldTimer::GetTimeOfDayFromSeconds: milliseconds of the remainder after whole days (__ftol2 of rest*1000).
U __fastcall kotor_fastcall_004ae200(void* timer, void*, float seconds) {
    if (seconds < 0.0f) return 0;                                                    // 004AE201..004AE215 ([0073D700])
    const float length = static_cast<float>(unsigned_dword(rd(word(timer) + 0x40)));  // 004AE218..004AE228
    double rest = seconds;                                                           // 004AE234
    if (!(seconds < length)) {                                                       // 004AE22C..004AE23D (jnp: ordered less skips)
        do { rest -= length; } while (rest >= length);                               // 004AE240..004AE24D
    }
    return w2::ftol2_006fae8c(rest * 1000.0);                                         // 004AE24F..004AE255 ([0073D6FC])
}

// ===================================================================================== 00503750
// CGameEffect::SetExpiryTime: +10h = day, +14h = time.
void __fastcall kotor_fastcall_00503750(void* effect, void*, U day, U time) {
    wr(word(effect) + 0x10, day);                                                    // 00503750..00503758
    wr(word(effect) + 0x14, time);                                                   // 00503754..0050375B
}

// ===================================================================================== 004F7650
// Party-update "hostile in sight" test: 1 when some perceived entry (list +920h/+924h, flag bit 0 of +4) names a
// creature whose +A88h is nonzero. The leading GetSWParty result is discarded (ECX is reloaded), as in the original.
U __fastcall kotor_fastcall_004f7650(void* creature, void*) {
    void* const app = kotor_menu_window_cells::manager_007a39fc;                     // 004F7650
    (void)kotor_fastcall_005ed8b0(*reinterpret_cast<void* const*>(static_cast<unsigned char*>(app) + 4), nullptr); // 004F7659..004F765C
    const U self = word(creature);
    for (I i = 0; i < static_cast<I>(rd(self + 0x924)); ++i) {                       // 004F7661..004F76A7 (count re-read)
        const U entry = rd(rd(self + 0x920) + 4u * static_cast<U>(i));               // 004F7670..004F7676
        if ((rd8(entry + 4) & 1) == 0) continue;                                     // 004F7679..004F767D
        void* const server = *reinterpret_cast<void* const*>(
            static_cast<unsigned char*>(kotor_menu_window_cells::manager_007a39fc) + 8); // 004F7681..004F7687
        void* const other = kotor_fastcall_004ae770(server, nullptr, rd(entry));     // 004F767F..004F768B
        if (other == nullptr) continue;                                              // 004F7690..004F7692
        if (rd(word(other) + 0xa88) != 0) return 1;                                  // 004F7694..004F769C -> 004F76AE
    }
    return 0;                                                                        // 004F76A9..004F76AD
}

// ===================================================================================== 00578E00
// CSWSAreaMap::GetMapPixelFromWorldCoord: north-axis mode [+10h] (0 none, 1 flip both, 2 swap+flip x, 3 swap+flip
// y), origin (+20h,+24h), scale (+18h,+1Ch); pixel = ftol(delta/scale + 0.5); valid range x 0..440, y 0..256.
U __fastcall kotor_fastcall_00578e00(void* map, void*, Vec world, I* x, I* y) {
    const U self = word(map);
    double u = world.x;                                                              // 00578E05 fld x
    float v = world.y;                                                               // 00578E01/00578E0C local = y
    const U mode = rd(self + 0x10);                                                  // 00578E10
    if (mode == 3) {                                                                 // 00578E13..00578E17
        v = world.x;                                                                 // 00578E19/00578E23
        u = static_cast<double>(world.y) * -1.0;                                     // 00578E1D..00578E27 ([00740CBC])
    }
    if (mode == 1) {                                                                 // 00578E2D..00578E30
        u = u * -1.0;                                                                // 00578E32
        v = static_cast<float>(static_cast<double>(v) * -1.0);                       // 00578E38..00578E42 fstp dword
    }
    if (mode == 2) {                                                                 // 00578E46..00578E49
        const float x_stored = static_cast<float>(u);                                // 00578E4B fstp dword [arg x]
        const float old_v = v;                                                       // 00578E4F fld local
        v = static_cast<float>(static_cast<double>(x_stored) * -1.0);                // 00578E53..00578E5D fstp dword local
        u = old_v;
    }
    *x = static_cast<I>(w2::ftol2_006fae8c((u - rf(self + 0x20)) / rf(self + 0x18) + 0.5));          // 00578E61..00578E7A
    *y = static_cast<I>(w2::ftol2_006fae8c((static_cast<double>(v) - rf(self + 0x24)) / rf(self + 0x1c) + 0.5)); // 00578E72..00578E91
    if (*x >= 0 && *x <= 0x1b8 && *y >= 0 && *y <= 0x100) return 1;                  // 00578E93..00578EB4 (signed)
    *x = -1;                                                                         // 00578EB7
    *y = -1;                                                                         // 00578EBE
    return 0;                                                                        // 00578EC4..00578EC8
}

// ===================================================================================== 00578F10
// Pixel (0..440, 0..256) -> explored-grid cell: ftol(x / (440/w) + 0.5), ftol(y / (256/h) + 0.5).
U __fastcall kotor_fastcall_00578f10(void* map, void*, I x, I y, I* out_x, I* out_y) {
    if (x > 0x1b8 || x < 0 || y > 0x100 || y < 0) {                                  // 00578F10..00578F2C
        *out_x = -1;                                                                 // 00578F78..00578F80
        *out_y = -1;                                                                 // 00578F86
        return 0;                                                                    // 00578F8C..00578F8E
    }
    const U self = word(map);
    const double cell_h = 256.0 / static_cast<double>(static_cast<I>(rd(self + 0xc))); // 00578F2E..00578F31 fdivr [007455D4]
    const double cell_w = 440.0 / static_cast<double>(static_cast<I>(rd(self + 8)));   // 00578F3B..00578F3E fdivr [00747748]
    *out_x = static_cast<I>(w2::ftol2_006fae8c(static_cast<double>(x) / cell_w + 0.5)); // 00578F37, 00578F44..00578F59
    *out_y = static_cast<I>(w2::ftol2_006fae8c(static_cast<double>(y) / cell_h + 0.5)); // 00578F51..00578F6E
    return 1;                                                                        // 00578F70..00578F75
}

// ===================================================================================== 00579000
// CSWSAreaMap::SetMapPointExplored: sets bit ((w+1)*cy + cx) of the explored bit array (+0 data, +4 dwords).
// Quirk kept: the word index is accepted when equal to the dword count (jg, not jge).
void __fastcall kotor_fastcall_00579000(void* map, void*, I x, I y) {
    if (x > 0x1b8 || x < 0) return;                                                  // 00579003..00579013
    if (y > 0x100 || y < 0) return;                                                  // 00579015..00579022
    I cell_y = 0, cell_x = 0;                                                        // 00579024..00579028 (argument slots)
    if (kotor_fastcall_00578f10(map, nullptr, x, y, &cell_x, &cell_y) == 0) return;  // 0057902C..00579041
    const U self = word(map);
    const I bit = (static_cast<I>(rd(self + 8)) + 1) * cell_y + cell_x;              // 00579043..00579050
    const I index = bit / 32;                                                        // 00579052..0057905D (cdq/and/add/sar)
    if (index > static_cast<I>(rd(self + 4))) return;                                // 0057905A..00579062
    const I shift = bit % 32;                                                        // 00579064..00579075
    const U cell = rd(self) + 4u * static_cast<U>(index);                            // 0057906A..0057906C
    wr(cell, rd(cell) | (1u << (static_cast<U>(shift) & 31u)));                      // 00579076..0057907D
}

// ===================================================================================== 005790C0
// CSWSAreaMap::SetPartyMemberWorldLocation: pixel position of party member `index` (< 3) into +28h/+34h.
void __fastcall kotor_fastcall_005790c0(void* map, void*, I index, Vec world) {
    const U self = word(map);
    if (rd(self + 4) == 0) return;                                                   // 005790C4..005790CC (jbe)
    if (index >= 3) return;                                                          // 005790CE..005790D5
    I px = 0, py = 0;                                                                // 005790D7..005790DB
    if (kotor_fastcall_00578e00(map, nullptr, world, &px, &py) == 0) return;         // 005790DF..0057910B
    wr(self + 0x28 + 4u * static_cast<U>(index), static_cast<U>(px));                // 0057910D..00579115
    wr(self + 0x34 + 4u * static_cast<U>(index), static_cast<U>(py));                // 00579111..00579119
}

// ===================================================================================== 00579130
// CSWSAreaMap::SetPartyMemberWorldOrientation: yaw (+180/+90/+270 for north-axis modes 1/2/3) into +40h[index].
void __fastcall kotor_fastcall_00579130(void* map, void*, I index, Vec direction) {
    const U self = word(map);
    if (rd(self + 4) == 0) return;                                                   // 00579136..0057913B
    if (index >= 3) return;                                                          // 0057913E..00579145
    const Vec local = direction;                                                     // 00579147..00579160
    double yaw = kotor_m15a_yaw_004aa0f0(&local);                                    // 00579157..00579164 (ST0)
    switch (rd(self + 0x10)) {                                                       // 00579169..00579176
    case 1: yaw += 180.0; break;                                                     // 0057919C ([0073E408])
    case 2: yaw += 90.0; break;                                                      // 0057918A ([0073E40C])
    case 3: yaw += 270.0; break;                                                     // 00579178 ([0073E404])
    default: break;
    }
    wf(self + 0x40 + 4u * static_cast<U>(index), static_cast<float>(yaw));          // 0057917E / 00579190 / 005791A2
}

// ===================================================================================== 005792D0
// CSWSAreaMap::SetWorldPointExplored: marks a diamond of radius cells around the world point's pixel.
void __fastcall kotor_fastcall_005792d0(void* map, void*, Vec world, I radius) {
    const U self = word(map);
    if (rd(self + 4) == 0) return;                                                   // 005792D9..005792E0
    I px = 0, py = 0;                                                                // 005792E6..005792EA
    if (kotor_fastcall_00578e00(map, nullptr, world, &px, &py) == 0) return;         // 005792EE..0057931A
    for (I dx = -radius; dx <= radius; ++dx) {                                       // 00579320..005793A3
        const I span = radius - (dx < 0 ? -dx : dx);                                 // 00579330..00579339 (cdq abs)
        for (I dy = -span; dy <= span; ++dy) {                                       // 0057933B..0057939A
            I x = (0x1b8 / static_cast<I>(rd(self + 8))) * dx + px;                  // 00579343..00579355
            I y = (0x100 / static_cast<I>(rd(self + 0xc))) * dy + py;                // 00579357..00579367
            if (x < 0) x = 0;                                                        // 00579369..0057936D
            if (y < 0) y = 0;                                                        // 0057936F..00579373
            if (x > 0x1b8) x = 0x1b8;                                                // 00579375..0057937D
            if (y > 0x100) y = 0x100;                                                // 00579382..00579389
            kotor_fastcall_00579000(map, nullptr, x, y);                             // 0057938E..00579392
        }
    }
}

// =====================================================================================================
//   00563A80 CSWPartyTable::SetPartyFollowState(int) [funcs.json name]  RET 4 (argument unused)
//   006369D0 CSWParty::FollowStateChanged()                              RET 0
extern "C" {
void __fastcall kotor_fastcall_006359c0(void* party, void*);                     // 006359C0 CSWParty::RecaulateFollowPoint
void __fastcall kotor_fastcall_006369d0(void* party, void*);
void __fastcall kotor_fastcall_00563a80(void* table, void*, U unused);
}

// FollowStateChanged: recompute the follow point, then remap the follow state (+28h, stride 88h, 3 members):
// 1->6, 2->9, 3->7, 4->8 (jump table 00636A18), other states unchanged.
void __fastcall kotor_fastcall_006369d0(void* party, void*) {
    kotor_fastcall_006359c0(party, nullptr);                                     // 006369D1..006369D3
    U cell = word(party) + 0x28;                                                 // 006369D8
    for (int left = 3; left != 0; --left, cell += 0x88) {                        // 006369DB, 00636A0E..00636A14
        switch (rd(cell)) {                                                      // 006369E1..006369E9 (ecx-1 > 3: ja)
        case 1: wr(cell, 6); break;                                              // 006369F0
        case 2: wr(cell, 9); break;                                              // 006369F8
        case 3: wr(cell, 7); break;                                              // 00636A00
        case 4: wr(cell, 8); break;                                              // 00636A08
        default: break;
        }
    }
}

// SetPartyFollowState: +E8h = 0, then the client party's FollowStateChanged.
void __fastcall kotor_fastcall_00563a80(void* table, void*, U) {
    wr(word(table) + 0xe8, 0);                                                   // 00563A80
    void* const client = *reinterpret_cast<void* const*>(
        static_cast<unsigned char*>(kotor_menu_window_cells::manager_007a39fc) + 4); // 00563A8A..00563A8F
    kotor_fastcall_006369d0(kotor_fastcall_005ed8b0(client, nullptr), nullptr);   // 00563A92..00563A99
}
