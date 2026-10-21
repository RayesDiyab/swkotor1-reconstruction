// lane-m47-core: CGuiInGame teardown / dialog-animation buffers and small CClientExoAppInternal leaves:
//   0062C310 CGuiInGame::ShutDown (+ 0062BA30 ClearRepliesData)          [R4 entry router, routes-shutdown]
//   00630620 CGuiInGame::InitializeDialogAnimationData (+ 0062B960 StoreLastDialogAnimationData) [r4-lowers]
//   005F2C60 CClientExoAppInternal::IndicateObject (+ 0062B000 CGuiInGame::SetMainInterfaceTarget,
//            006855F0 CSWGuiMainInterface::SetTarget)                    [gui-load mainmenu]
//   005F4A00 CClientExoAppInternal::CancelMovie (+ 0070C1E0 CExoMoviePlayer::CancelMovie,
//            00404C40 CExoMoviePlayerInternal::CancelMovie)              [routes-window-leaves]
// Link-required; not reached on the main-menu first frame (no in-game GUI / movie cancel there).
// Evidence: lane dis/<addr>.asm. Every owned pointer released here is either a polymorphic object deleted through its
// slot 0 with flag 1 (`mov reg,[ecx]; push 1; call [reg]`) or a raw block freed with 006FA390 (live_free); the CExoString
// array at +118h is released with eh_vector_destructor_iterator 006FA852 (8-byte stride, dtor 005E5C20) and the cookie
// block freed (array delete[]).
#include "39c670fc66-allocator_live.h"
#include "f9f7ab0530-startup_string.h"
#include <cstdint>
#include <cstring>

namespace {
using U32 = std::uint32_t;
U32 w(const void* p, U32 off) { U32 v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
void sw(void* p, U32 off, U32 v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4); }
void* pw(const void* p, U32 off) { void* v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
using DeletingSlot = void* (__fastcall*)(void*, void*, U32);
// `mov ecx,[this+off]; test; je; mov eax,[ecx]; push 1; call [eax]; mov [this+off],0`
void delete_member(void* self, U32 off) {
    if (void* const object = pw(self, off)) {
        DeletingSlot slot; std::memcpy(&slot, pw(object, 0), sizeof(slot));
        (void)slot(object, nullptr, 1u);
        sw(self, off, 0);
    }
}
// `mov eax,[this+off]; test; je; push eax; call 006FA390; mov [this+off],0`
void free_member(void* self, U32 off) {
    if (void* const block = pw(self, off)) { live_free(block); sw(self, off, 0); }
}
}

extern "C" {
void __stdcall c2da_destroy_array_006fa852(void*, U32, U32, void (__fastcall*)(kotor_reconstruction::StartupString*, void*)); // task10-c2da-array-normal
void __fastcall c2da_current_string_destroy_005e5c20(kotor_reconstruction::StartupString*, void*);                         // startup-misc

// 0062BA30 CGuiInGame::ClearRepliesData.
__declspec(noinline) void __fastcall kotor_fastcall_0062ba30(void* self, void*) {
    if (unsigned char* const strings = static_cast<unsigned char*>(pw(self, 0x118))) {       // 0062BA34..3F
        const U32 count = w(strings - 4, 0);                                                 // 0062BA41 cookie
        c2da_destroy_array_006fa852(strings, 8u, count, &c2da_current_string_destroy_005e5c20); // 0062BA47..50
        live_free(strings - 4);                                                               // 0062BA55..56
        sw(self, 0x118, 0);                                                                   // 0062BA5E
    }
    for (U32 off = 0x11C; off <= 0x150; off += 4) free_member(self, off);                    // 0062BA64..BBC
    for (int i = 0; i < static_cast<int>(w(self, 0x114)); ++i) {                              // 0062BBC8..C10 (count reloaded)
        if (w(pw(self, 0x154), static_cast<U32>(i) * 4u) == 0) continue;                      // 0062BBD0..D9
        void* const first = pw(pw(self, 0x158), static_cast<U32>(i) * 4u);                    // 0062BBDB..E1
        void* const second = pw(pw(self, 0x15C), static_cast<U32>(i) * 4u);                  // 0062BBE6..EC
        if (first) live_free(first);                                                          // 0062BBEF..F7
        if (second) live_free(second);                                                        // 0062BBFA..04
    }
    free_member(self, 0x158);                                                                 // 0062BC13..26
    free_member(self, 0x15C);                                                                 // 0062BC2C..3F
    free_member(self, 0x154);                                                                 // 0062BC45..58
    sw(self, 0x114, 0);                                                                       // 0062BC5F
}

// 0062C310 CGuiInGame::ShutDown (only when [+108h] != 0).
__declspec(noinline) void __fastcall frame_R4_0062c310(void* self, void*) {
    if (w(self, 0x108) == 0) return;                                                          // 0062C313..1E
    sw(self, 0x38, 0);                                                                        // 0062C329
    static const U32 panels[] = {0x4C, 0x50, 0x54, 0x58, 0x70, 0x74, 0x5C, 0x08, 0x60, 0x64, 0x68,
                                 0x40, 0x44, 0x48, 0x6C, 0x78, 0x7C, 0x8C, 0x94, 0xA8};       // 0062C324..473 (order)
    for (U32 off : panels) delete_member(self, off);
    free_member(self, 0xAC);                                                                  // 0062C479..8C
    delete_member(self, 0x84);                                                                // 0062C492..A2
    delete_member(self, 0x90);                                                                // 0062C4A8..B8
    for (U32 off = 0x0C; off < 0x2C; off += 4) delete_member(self, off);                      // 0062C4C0..DA (8 entries)
    delete_member(self, 0x80);                                                                // 0062C4DC..EE
    free_member(self, 0x190);                                                                 // 0062C4F4..507
    free_member(self, 0x194);                                                                 // 0062C50D..520
    free_member(self, 0x1A0);                                                                 // 0062C526..539
    free_member(self, 0x1A4);                                                                 // 0062C53F..552
    if (w(self, 0x1A8) != 0) {                                                                // 0062C558..5E
        live_free(pw(self, 0x1A0));                                                           // 0062C560..67 (+1A0h, as read)
        sw(self, 0x1A0, 0);                                                                   // 0062C56F
    }
    free_member(self, 0x1AC);                                                                 // 0062C575..588
    sw(self, 0x18C, 0);                                                                       // 0062C590
    kotor_fastcall_0062ba30(self, nullptr);                                                   // 0062C58E..596
    sw(self, 0x198, 0);                                                                       // 0062C5A3
    free_member(self, 0x1A0);                                                                 // 0062C59B..5B4
    free_member(self, 0x1A4);                                                                 // 0062C5BA..5CD
    sw(self, 0x19C, 0);                                                                       // 0062C5DB
    free_member(self, 0x1A8);                                                                 // 0062C5D3..5EC
    free_member(self, 0x1AC);                                                                 // 0062C5F2..605
    sw(self, 0xB38, 0); sw(self, 0xB3C, 0); sw(self, 0x108, 0);                               // 0062C60B..617
}

// 0062B960 CGuiInGame::StoreLastDialogAnimationData: copy current (+198h count, +1A0h dwords, +1A4h words) to last.
__declspec(noinline) void __fastcall kotor_fastcall_0062b960(void* self, void*) noexcept(false) {
    sw(self, 0x19C, 0);                                                                       // 0062B96B
    free_member(self, 0x1A8);                                                                 // 0062B963..980
    free_member(self, 0x1AC);                                                                 // 0062B98A..99D
    const U32 count = w(self, 0x198);                                                         // 0062B9A7
    sw(self, 0x19C, count);                                                                   // 0062B9AF
    if (count != 0) {                                                                         // 0062B9B5
        void* const words = live_operator_new(count << 2);                                    // 0062B9B7..BB
        std::memcpy(static_cast<unsigned char*>(self) + 0x1A8, &words, 4);                    // 0062B9C9
        void* const halves = live_operator_new(w(self, 0x198) << 1);                          // 0062B9C0..CF
        std::memcpy(static_cast<unsigned char*>(self) + 0x1AC, &halves, 4);                   // 0062B9D7
    }
    for (int i = 0; i < static_cast<int>(w(self, 0x198)); ++i) {                              // 0062B9DD..A1F (count reloaded)
        sw(pw(self, 0x1A8), static_cast<U32>(i) * 4u, w(pw(self, 0x1A0), static_cast<U32>(i) * 4u)); // 0062B9F0..FF
        std::uint16_t half; std::memcpy(&half, static_cast<unsigned char*>(pw(self, 0x1A4)) + i * 2, 2); // 0062BA02..08
        std::memcpy(static_cast<unsigned char*>(pw(self, 0x1AC)) + i * 2, &half, 2);           // 0062BA0C..12
    }
}
// 00630620 CGuiInGame::InitializeDialogAnimationData(count) RET 4; EAX 1 when both buffers were allocated.
__declspec(noinline) void __fastcall kotor_fastcall_00630620(void* self, void*, std::uint32_t count) {
    kotor_fastcall_0062b960(self, nullptr);                                                   // 00630626
    sw(self, 0x198, 0);                                                                       // 00630633
    free_member(self, 0x1A0);                                                                 // 0063062B..644
    free_member(self, 0x1A4);                                                                 // 0063064A..65D
    if (count == 0) return;                                                                   // 00630663..69 (EAX 0)
    sw(self, 0x198, count);                                                                   // 0063066B
    void* const words = live_operator_new(count << 2);                                        // 00630671..75
    std::memcpy(static_cast<unsigned char*>(self) + 0x1A0, &words, 4);                        // 00630683 (before 2nd new)
    void* const halves = live_operator_new(w(self, 0x198) << 1);                              // 0063067A..89
    std::memcpy(static_cast<unsigned char*>(self) + 0x1A4, &halves, 4);                       // 00630699
    if (words != nullptr && halves != nullptr) return;                                        // 00630697..A3 (EAX 1)
    live_free(pw(self, 0x1A0));                                                               // 006306AF..B0 (fields not cleared)
    live_free(pw(self, 0x1A4));                                                               // 006306B5..BC
}

// 006855F0 CSWGuiMainInterface::SetTarget(id, unused) RET 8: on change, [+5CB0h] = -1.0f.
__declspec(noinline) void __fastcall kotor_fastcall_006855f0(void* self, void*, U32 id, U32) {
    if (w(self, 0x64) == id) return;                                                          // 006855F0..F7
    sw(self, 0x64, id);                                                                       // 006855F9
    sw(self, 0x5CB0, 0xBF800000u);                                                            // 006855FC
}
// 0062B000 CGuiInGame::SetMainInterfaceTarget(id, b) RET 8: tail JMP 006855F0 on [+90h] when present.
__declspec(noinline) void __fastcall kotor_fastcall_0062b000(void* self, void*, U32 id, U32 b) {
    if (void* const main_interface = pw(self, 0x90)) kotor_fastcall_006855f0(main_interface, nullptr, id, b); // 0062B000..0A
}
// 005F2C60 CClientExoAppInternal::IndicateObject(object, b) RET 8: object id (or 7F000000h) to the in-game GUI (+40h).
__declspec(noinline) void __fastcall kotor_fastcall_005f2c60(void* self, void*, std::uint32_t object, std::uint32_t b) {
    const U32 id = object != 0 ? w(reinterpret_cast<void*>(static_cast<std::uintptr_t>(object)), 4) : 0x7F000000u; // 005F2C60..7F
    kotor_fastcall_0062b000(pw(self, 0x40), nullptr, id, b);                                  // 005F2C6B..83 (tail JMP)
}

// 00404C40 CExoMoviePlayerInternal::CancelMovie(a, b) RET 8.
__declspec(noinline) void __fastcall kotor_fastcall_00404c40(void* self, void*, U32 a, U32 b) {
    if (b != 0 || (w(self, 0x30) == 1 && w(self, 0x9C) != 0)) {                              // 00404C40..5A
        sw(self, 0x14, 1);                                                                    // 00404C5C
        sw(self, 0x18, a);                                                                    // 00404C5F..63
        return;
    }
    if (w(self, 0x30) == 1) { sw(self, 0x18, a); return; }                                   // 00404C4D..5A -> 00404C5F
    sw(self, 0x18, a);                                                                        // 00404C69..6D
}
// 0070C1E0 CExoMoviePlayer::CancelMovie: mov ecx,[ecx]; jmp 00404C40.
__declspec(noinline) void __fastcall kotor_fastcall_0070c1e0(void* self, void*, U32 a, U32 b) {
    kotor_fastcall_00404c40(pw(self, 0), nullptr, a, b);
}
// 005F4A00 CClientExoAppInternal::CancelMovie: movie player at +12Ch, CancelMovie(1, 0).
__declspec(noinline) void __fastcall kotor_fastcall_005f4a00(void* self, void*) {
    if (void* const player = pw(self, 0x12C)) kotor_fastcall_0070c1e0(player, nullptr, 1u, 0u); // 005F4A00..0E (push 0; push 1)
}
}
