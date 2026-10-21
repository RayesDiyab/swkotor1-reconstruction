// Private A4 reconstruction of original 0068A310 CSWGuiMainInterface::UpdateIndicator.
//
// Original ABI: thiscall(float delta) RET 4; ECX=self, one 32-bit float argument,
// callee-cleaned. Reconstruction export: @kotor_fastcall_0068a310@12
// (dummy EDX, one stack float, generated RET 4). No SEH, no /GS cookie, no heap,
// no CResRef destructor. The two 16-byte CResRef locals are POD and are
// constructed by 00405ED0, assigned by 00406290 / 00406120.
//
// Selected providers used by exact symbol:
//   005ED580  @kotor_fastcall_005ed580@12
//   00405ED0  @kotor_fastcall_00405ed0@8
//   005ED8B0  @kotor_fastcall_005ed8b0@8
//   00635460  @kotor_fastcall_00635460@8
//   005EE0F0  @kotor_fastcall_005ee0f0@8
//   00423BC0  _kotor_cdecl_00423bc0
//   006FAE8C  _genuine_x87_ftol_006fae8c (real body at 0061DC20; cdecl ST0 -> EAX)
//   00406290  @unresolved_ref_assign_00406290@16 (real body despite the name)
//   00406120  @c2da_copy_00406120@16 -> ?assign_00406120@resource_ref_assignment...
//   0060FB20  @kotor_m15a_creature_get_server_creature_0060fb20@8
//   0063D4B0  @kotor_m15a_object_get_server_object_0063d4b0@8
//   0057CB80  @recovered_object_get_reputation_0057cb80@16
//   005EDE70  @kotor_fastcall_005ede70@8
//   00414C00  @gui_load_border_params_set_fill_00414c00@16
//
// Explicit dependency gates (missing selected bodies; called, never trapped,
// faked, null-guarded or skipped):
//   G1 006889C0 CSWGuiMainInterface::UpdateStickyTarget
//      expected export @kotor_fastcall_006889c0@8, thiscall RET 0.
//   G2 006859E0 CSWGuiTargetActionMenu::SetFriend
//      expected export @kotor_fastcall_006859e0@16, thiscall with two stack
//      args (friend, poison), RET 8.
// G8 type-8 client-object +0x13C is a runtime table gate: the original calls the
// selected vtable slot; this source does the same (no type-8 special case).
//
// Virtual slots: +0x0C AsSWCObject (twice, kept as two calls); +0x13C
// GetReticlePoint(Vector*) returning 0/1/2; +0x2C AsSWCCreature; +0x138
// IsHostile(); +0x14/+0x4C/+0x3C AsSWCDoor/AsSWCPlaceable/AsSWCTrigger; label
// +0x04 SetExtent(extent). Door/item +0x13C resolves to the selected 11-byte
// EDX-normalizing adapter, which tail-jumps to the real 0063D560; it is called
// normally, no rebind and no special case.
//
// Fault behavior is intentionally unchecked, matching the original: null
// app/client, null manager (+0x6C/+0x6E), null player character (+0x24),
// null GetServerObject results and null GetReputation receiver are not guarded.
//
// Source-only private proposal. Not built, linked, run or tested. No source
// ACCEPT is claimed; independent source review is required.
#include "8d1d1f9e87-w4_providers.hpp"

#include <cstdint>
#include <cstring>

using w4::U32;

// ------------------------------------------------------------------ providers
extern "C" {
void* __fastcall kotor_fastcall_005ed580(void*, void*, U32);
void* __fastcall kotor_fastcall_00405ed0(void*, void*);
void* __fastcall kotor_fastcall_005ed8b0(void*, void*);
void* __fastcall kotor_fastcall_00635460(void*, void*);
unsigned __fastcall kotor_fastcall_005ee0f0(void*, void*);
void __cdecl kotor_cdecl_00423bc0(void*, const void*, const void*);
int __cdecl genuine_x87_ftol_006fae8c();  // ST0 -> EAX, pops ST0, cdecl
void __fastcall unresolved_ref_assign_00406290(void*, void*, void*, const char*);
void __fastcall c2da_copy_00406120(void*, void*, void*, const void*);
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void*, void*);
void* __fastcall kotor_m15a_object_get_server_object_0063d4b0(void*, void*);
int __fastcall recovered_object_get_reputation_0057cb80(void*, void*, U32, int);
unsigned __fastcall kotor_fastcall_005ede70(void*, void*);
void __fastcall gui_load_border_params_set_fill_00414c00(void*, void*, const void*, int);

// G1 / G2: exact selected-body contracts (missing in 1565; real bodies required).
void __fastcall kotor_fastcall_006859e0(void*, void*, unsigned, unsigned);
void __fastcall kotor_fastcall_006889c0(void*, void*);
}

namespace {

struct ResRef16 {
    U32 a, b, c, d;
};
static_assert(sizeof(ResRef16) == 16, "16-byte CResRef local");

struct Vec3 {
    float x, y, z;
};

struct Extent {
    int x, y, w, h;
};

// Integer-based pointer arithmetic keeps the original unchecked null faults
// (e.g. a null player character yields address 0x24, not a C++ UB shortcut).
void* atv(void* p, U32 off) {
    return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(p) + off);
}

template <class T>
T* at(void* p, U32 off) {
    return reinterpret_cast<T*>(atv(p, off));
}

// Original 006FAE8C call contract: value on ST0, EAX = low dword, ST0 popped.
// The arithmetic feeding this helper is carried in double (53-bit), which is
// bit-identical to the original PC53 x87 sequence for these finite operations.
int ftol2_X87(double value) {
    int result;
    volatile double v = value;
    __asm {
        fld qword ptr [v]
        call genuine_x87_ftol_006fae8c
        mov result, eax
    }
    return result;
}

// 00406290: return-by-value CResRef literal assignment (this, hidden return,
// literal), RET 8. `scratch` receives the hidden return and is discarded.
void assign_literal(ResRef16& dst, ResRef16& scratch, const char* text) {
    unresolved_ref_assign_00406290(&dst, nullptr, &scratch, text);
}

// 00406120: return-by-value CResRef copy (this=dst, hidden return, src), RET 8.
// This is the original `tmpA = tmpB` used when the arrow replaces the reticle.
void copy_ref(ResRef16& dst, ResRef16& scratch, const ResRef16& src) {
    c2da_copy_00406120(&dst, nullptr, &scratch, &src);
}

}  // namespace

// ------------------------------------------------------------------ 0068A310
extern "C" void __fastcall kotor_fastcall_0068a310(void* self, void*, float delta) {
    // Frame-local state. All initialization happens before any forward jump, so
    // the original's early exits can be expressed as labels without skipping
    // non-trivial construction.
    ResRef16 tmpA;
    ResRef16 tmpB;
    ResRef16 scratch;
    Vec3 screen = {0.0f, 0.0f, 0.0f};
    Vec3 dist;
    Extent extent = {0, 0, 0, 0};
    unsigned resolved = 0;
    unsigned char result = 0;
    unsigned friendFlag = 1;
    unsigned poison = 0;
    int size = 0x40;
    float angle = 0.0f;
    int left = 0;
    int top = 0;

    // Original read order 0068A316..328: global manager cell, then self+0x64,
    // then [manager+4]. Keep it so the unchecked fault order is preserved.
    void* const appManager = w4::app_manager();
    const U32 targetId = w4::lw(self, 0x64);
    void* const client = w4::lp(appManager, 4);
    void* obj = kotor_fastcall_005ed580(client, nullptr, targetId); // 0068A328

    kotor_fastcall_00405ed0(&tmpA, nullptr);                      // 0068A351
    kotor_fastcall_00405ed0(&tmpB, nullptr);                      // 0068A35A

    // The original reads the manager and both signed screen extents BEFORE the
    // object-null branch (0068A361..0068A380). The volatile reads keep that
    // fault/read order; the copied values are the ones used by mode 2.
    void* const manager = w4::lp(self, 0x18);
    volatile int screenW_v = static_cast<int>(static_cast<std::int16_t>(w4::lh(manager, 0x6C)));
    volatile int screenH_v = static_cast<int>(static_cast<std::int16_t>(w4::lh(manager, 0x6E)));
    const int screenW = screenW_v;
    const int screenH = screenH_v;

    if (obj == nullptr) goto flags;                                // 0068A384
    if (w4::vcall<void*>(obj, 0x0C) == nullptr) goto flags;        // 0068A38E..93
    {
        void* const esi = w4::vcall<void*>(obj, 0x0C);             // 0068A399..A0
        void* const party = kotor_fastcall_005ed8b0(w4::client_app(), nullptr); // 0068A3A2..B7 re-read
        void* const pc = kotor_fastcall_00635460(party, nullptr);  // 0068A3BE
        resolved = 1;                                              // 0068A3D0 (S+0x50)
        result = w4::vcall<unsigned char, void*>(esi, 0x13C, &screen); // 0068A3D4
        if (result == 0) goto after_switch;                        // 0068A3E3

        switch (w4::lb(obj, 8)) {                                  // 0068A3E9
        case 5: {                                                  // creature (B4)
            void* const clientCreature = w4::vcall<void*>(esi, 0x2C);      // 0068A404
            void* const serverCreature =
                kotor_m15a_creature_get_server_creature_0060fb20(clientCreature, nullptr); // 0068A409
            const unsigned hostile = w4::vcall<unsigned>(esi, 0x138);       // 0068A414
            if (hostile || kotor_fastcall_005ee0f0(w4::client_app(), nullptr) != 0) { // 0068A41E..37 re-read
                friendFlag = 0;
            } else {
                friendFlag = 1;
            }
            poison = serverCreature != nullptr ? w4::lw(serverCreature, 0x9E0) : 0; // 0068A439..47

            kotor_cdecl_00423bc0(&dist, at<Vec3>(pc, 0x24), at<Vec3>(esi, 0x24)); // 0068A45C
            double sum = static_cast<double>(dist.z) * dist.z;     // 0068A461..65
            sum += static_cast<double>(dist.y) * dist.y;           // 0068A46C..74
            sum += static_cast<double>(dist.x) * dist.x;           // 0068A476..7E
            const float d = static_cast<float>(sum);               // 0068A480 fst dword
            if (!w4::jcc_jne(d, 900.0f, 0x01)) {                   // 0068A484..8F
                size = 0x10;
            } else if (!w4::jcc_jne(d, 25.0f, 0x41)) {             // 0068A49B..AA
                double s = w4::x87_fsqrt(static_cast<double>(d));   // 0068A4AC..B0 fsqrt
                s = s - 5.0;                                       // 0068A4B2 fsub qword
                s = s * 0.04;                                      // 0068A4B8 fmul qword
                s = s * 48.0;                                      // 0068A4BE fmul qword
                size = 0x40 - ftol2_X87(s);                        // 0068A4C4..D0
            }

            if (friendFlag != 0) {                                 // 0068A4D4..D8
                assign_literal(tmpA, scratch, "friendlyreticle2"); // 0068A4E9
                assign_literal(tmpB, scratch, "friendlyarrow");    // 0068A4EE/762
            } else if (kotor_fastcall_005ede70(w4::client_app(), nullptr) != 0) { // 0068A500..0F re-read
                assign_literal(tmpA, scratch, "combatreticle");    // 0068A526
                const float t = w4::lf(self, 0x5CB0);              // 0068A52B
                if (w4::jcc_jne(t, 0.0f, 0x41)) {                  // 0068A539..3C
                    // t <= 0 or NaN: sentinel/zero path (0068A5B5).
                    if (w4::lw(self, 0x5CB0) == 0xBF800000u) {     // 0068A5B5..BF
                        size += 0x40;
                        w4::sw(self, 0x5CB0, 0x3F000000u);         // 0.5f
                    }
                } else {
                    const double t2d = static_cast<double>(t) - static_cast<double>(delta);
                    w4::sf(self, 0x5CB0, static_cast<float>(t2d)); // 0068A551 fst dword
                    if (w4::jcc_jp(t2d, 0.0, 0x41)) {              // 0068A557..5C: t2 > 0 or NaN
                        double v = 0.5 - t2d;                      // 0068A57C..82
                        v = v + v;                                 // 0068A584 fadd st(0),st(0)
                        v = 1.0 - v;                               // 0068A586 fsubr
                        v = v * 64.0;                              // 0068A58C fmul
                        size += ftol2_X87(v);                      // 0068A592
                    } else {
                        w4::sw(self, 0x5CB0, 0u);                  // 0068A56C timer = 0
                    }
                }
                assign_literal(tmpB, scratch, "hostilearrow");     // 0068A5F8/762
            } else {
                assign_literal(tmpA, scratch, "hostilereticle2");  // 0068A5F3
                assign_literal(tmpB, scratch, "hostilearrow");     // 0068A5F8/762
            }
            break;
        }
        case 7:                                                    // trigger
        case 9:                                                    // placeable
        case 10: {                                                 // door (B5)
            void* const door = w4::vcall<void*>(esi, 0x14);        // 0068A60E
            void* const placeable = w4::vcall<void*>(esi, 0x4C);   // 0068A619
            void* const trigger = w4::vcall<void*>(esi, 0x3C);     // 0068A622
            if (door != nullptr && w4::lw(door, 0x110) != 0 && w4::lw(door, 0x114) != 0) {
                friendFlag = 0;                                    // 0068A62D..3F
            } else if (trigger != nullptr) {
                friendFlag = w4::vcall<unsigned>(trigger, 0x138) == 0 ? 1u : 0u; // 0068A641..56
            } else if (placeable == nullptr) {
                friendFlag = 1;                                    // 0068A658..5A
            } else if (w4::lw(placeable, 0x11C) != 0 && w4::lw(placeable, 0x124) != 0) {
                friendFlag = 0;                                    // 0068A65C..6E
            } else if (w4::vcall<unsigned>(placeable, 0x138) == 0) {
                friendFlag = 1;                                    // 0068A674..7C
            } else {
                void* const serverPc = kotor_m15a_object_get_server_object_0063d4b0(pc, nullptr); // 0068A682
                const U32 pcId = w4::lw(serverPc, 4);              // 0068A687 unchecked
                void* const serverPlaceable =
                    kotor_m15a_object_get_server_object_0063d4b0(placeable, nullptr); // 0068A68F
                const int rep =
                    recovered_object_get_reputation_0057cb80(serverPlaceable, nullptr, pcId, 0); // 0068A696
                friendFlag = rep > 10 ? 1u : 0u;                   // 0068A69B..A0
            }

            kotor_cdecl_00423bc0(&dist, at<Vec3>(pc, 0x24), at<Vec3>(esi, 0x24)); // 0068A6B3
            double sum = static_cast<double>(dist.z) * dist.z;     // 0068A6B8..BC
            sum += static_cast<double>(dist.y) * dist.y;           // 0068A6C3..CB
            sum += static_cast<double>(dist.x) * dist.x;           // 0068A6CD..D5
            const float d = static_cast<float>(sum);               // 0068A6D7 fst dword
            if (!w4::jcc_jne(d, 900.0f, 0x01)) {                   // 0068A6DB..E6
                size = 0x20;
            } else if (!w4::jcc_jne(d, 25.0f, 0x41)) {             // 0068A6F2..01
                double s = w4::x87_fsqrt(static_cast<double>(d));   // 0068A703..07 fsqrt
                s = s - 5.0;                                       // 0068A709
                s = s * 0.04;                                      // 0068A70F
                s = s * 32.0;                                      // 0068A715
                size = 0x40 - ftol2_X87(s);                        // 0068A71B..27
            }

            if (friendFlag != 0) {
                assign_literal(tmpA, scratch, "friendlyreticle2"); // 0068A746
                assign_literal(tmpB, scratch, "friendlyarrow");    // 0068A762
            } else {
                assign_literal(tmpA, scratch, "hostilereticle2");  // 0068A746
                assign_literal(tmpB, scratch, "hostilearrow");     // 0068A762
            }
            break;
        }
        default:
            // 0068A3F3: types 6/8/other with result != 0 go to the epilogue
            // without SetFriend, rect, flag writes or UpdateStickyTarget.
            return;
        }
    }

after_switch:
    kotor_fastcall_006859e0(atv(self, 0xBC), nullptr, friendFlag, poison); // 0068A777 (G2)
    if (result == 0) goto flags;                                   // 0068A782

    std::memcpy(&extent, atv(self, 0xBF70), sizeof(extent));       // 0068A788..A6
    angle = 0.0f;                                                  // 0068A79E
    {
        const int x0 = extent.x;
        const int y0 = extent.y;
        const int w0 = extent.w;
        const int h0 = extent.h;
        // Original 32-bit lea arithmetic; unsigned casts avoid signed-overflow UB.
        const int leftEdge = x0;
        const int leftMargin = static_cast<int>(static_cast<U32>(x0) + 0x20u);   // L+0x20
        const int rightEdge = static_cast<int>(static_cast<U32>(x0) + static_cast<U32>(w0) - 0x20u);
        const int topEdge = y0;
        const int topMargin = static_cast<int>(static_cast<U32>(y0) + 0x20u);   // y0+0x20
        const int bottom = static_cast<int>(static_cast<U32>(y0) + static_cast<U32>(h0) - 0x20u);

        if (result == 2) {                                         // 0068A7AA -> mode 2 (B12)
            copy_ref(tmpA, scratch, tmpB);                         // 0068A7C1: mode 2 always uses the arrow
            const int halfW = screenW / 2;                         // 0068A7C6..CD
            size = 0x20;                                           // 0068A7D3
            if (!w4::jcc_jne(halfW, screen.x, 0x41)) {             // 0068A7DB..E8
                left = rightEdge;                                  // 0068A7EA..FA
                angle = 180.0f;
            } else {
                left = leftEdge;                                   // 0068A800
            }
            if (w4::jcc_je(topMargin, screen.y, 0x41)) {           // 0068A804..18: y0+0x20 > sy
                top = bottom;                                      // 0068A979
            } else if (w4::jcc_jnp(bottom, screen.y, 0x05)) {      // 0068A81E..33: bottom < sy
                top = topEdge;                                     // 0068A868
            } else {
                const int halfH = screenH / 2;                     // 0068A835..3E
                if (!w4::jcc_jne(halfH, screen.y, 0x41)) {         // 0068A844..55
                    top = halfH - ftol2_X87(static_cast<double>(screen.y) + 16.0); // 0068A85B..66
                } else {
                    top = ftol2_X87(static_cast<double>(screen.y) - 16.0);         // 0068A8E3..F2
                }
            }
        } else {                                                   // 0068A871 -> mode 1 (B13)
            if (!w4::jcc_jne(leftMargin, screen.x, 0x41)) {        // 0068A871..85: sx < L+0x20
                copy_ref(tmpA, scratch, tmpB);                     // 0068A894
                size = 0x20;                                       // 0068A8A8
                left = leftEdge;                                   // 0068A8B0
                if (w4::jcc_je(topMargin, screen.y, 0x41)) {       // 0068A89D..BD
                    top = topEdge;                                 // 0068A868
                } else if (w4::jcc_jp(bottom, screen.y, 0x05)) {   // 0068A8BF..D4
                    top = ftol2_X87(static_cast<double>(screen.y) - 16.0); // 0068A8E3
                } else {
                    top = bottom;                                  // 0068A8D6..DA
                }
            } else if (w4::jcc_jp(rightEdge, screen.x, 0x05)) {    // 0068A8FB..910: MID @0068A986
                if (!w4::jcc_jne(topMargin, screen.y, 0x41)) {     // 0068A986..9A
                    copy_ref(tmpA, scratch, tmpB);                 // 0068A99C
                    size = 0x20;                                   // 0068A9BC
                    top = topEdge;                                 // 0068A9C4
                    angle = 270.0f;                                // 0068A9CD
                    left = ftol2_X87(static_cast<double>(screen.x) - 16.0); // 0068A9C8
                } else if (w4::jcc_jp(bottom, screen.y, 0x05)) {   // 0068A9DA..EF
                    const int half = size / 2;                     // 0068AA30..39
                    top = ftol2_X87(static_cast<double>(screen.y) - static_cast<double>(half)); // 0068AA47
                    left = ftol2_X87(static_cast<double>(screen.x) - static_cast<double>(half)); // 0068AA56
                } else {
                    copy_ref(tmpA, scratch, tmpB);                 // 0068A9F1
                    size = 0x20;                                   // 0068AA15
                    top = bottom;                                  // 0068AA1D
                    angle = 90.0f;                                 // 0068AA26
                    left = ftol2_X87(static_cast<double>(screen.x) - 16.0); // 0068AA21
                }
            } else {                                               // 0068A912: sx > R, right edge
                copy_ref(tmpA, scratch, tmpB);                     // 0068A912
                size = 0x20;                                       // 0068A93F
                left = rightEdge;                                  // 0068A93B
                angle = 180.0f;                                    // 0068A94B
                if (w4::jcc_je(topMargin, screen.y, 0x41)) {       // 0068A928..58
                    top = topEdge;                                 // 0068A868
                } else if (w4::jcc_jp(bottom, screen.y, 0x05)) {   // 0068A95E..73
                    top = ftol2_X87(static_cast<double>(screen.y) - 16.0); // 0068A8E3
                } else {
                    top = bottom;                                  // 0068A979
                }
            }
        }
    }

    extent.x = left;
    extent.y = top;
    extent.w = size;
    extent.h = size;
    w4::vcall<void, const void*>(atv(self, 0x5A2C), 0x04, &extent);  // 0068AA7E SetExtent
    w4::sf(self, 0x5AA4, angle);                                     // 0068AA8B
    gui_load_border_params_set_fill_00414c00(atv(self, 0x5A9C), nullptr, &tmpA, 0); // 0068AA98

flags:
    {
        const U32 f = w4::lw(self, 0x5A70);
        w4::sw(self, 0x5A70, f ^ ((((result != 0) ? 1u : 0u) << 1) ^ f) & 2u); // 0068AA9F..BF
        const U32 g = w4::lw(self, 0x1BA8);
        w4::sw(self, 0x1BA8, g ^ ((g ^ (resolved != 0 ? 1u : 0u)) & 1u));     // 0068AAC5..DA
    }
    kotor_fastcall_006889c0(self, nullptr);                          // 0068AAE0 (G1)
}
