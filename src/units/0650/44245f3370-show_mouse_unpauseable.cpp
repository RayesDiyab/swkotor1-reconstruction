// OFFER from lane-prepare to lane-r4-lowers (default owner of 0061FA00/0062B410).
// Names match lane-routes kotor_original_calls.h consumers. Not compiled into
// lane-prepare's stage. 005DF5B0/005E0930 are lane-routes bodies (window_leaves.cpp).
#include "2ff1299369-game_prepare_producers.h"
#include "f9de668f9a-gui_mouse.h"
#include "df72b7ab87-window_shared_cells.h"
#include <cstring>
// lane-r4-lowers change: 008338F4 has ONE storage, kotor_input_cells_required::word_008338f4
// (declared by lane-input client_input_event.h, defined by lane-r4-lowers r4_cells.cpp);
// the offer's separate kotor_cell_mouse_button_008338f4 definition is removed.
namespace kotor_input_cells_required { extern volatile std::int32_t word_008338f4; }
namespace {
using Word = std::uint32_t;
inline Word get32(const void* p, unsigned off) noexcept { Word v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
inline void put32(void* p, unsigned off, Word v) noexcept { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4); }
inline void* ptr(Word w) noexcept { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(w)); }
inline float getf(const void* p, unsigned off) noexcept { float v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
}
extern "C" {
void __fastcall kotor_fastcall_005e00b0(void* internal, void*);   // MISSING CExoInputInternal::ShowMouse
void __fastcall kotor_fastcall_005df5b0(void* input, void*, std::uint32_t); // lane-routes window_leaves.cpp
// 005DF5F0 CExoInput::ShowMouse: mov ecx,[ecx+4]; jmp 005E00B0.
void __fastcall kotor_fastcall_005df5f0(void* input, void*) { kotor_fastcall_005e00b0(ptr(get32(input, 4)), nullptr); }
// 0040B290 CSWGuiManager::ShowSoftwareMouse: if [+24]==0 jmp 0040B060 (same ECX); else RET.
void __fastcall kotor_fastcall_0040b290(void* gui, void*) {
    if (get32(gui, 0x24) == 0) static_cast<gui_mouse::Gui*>(gui)->activate_0040b060(); // 0040B290..0040B297
}
// 0061FA00 CClientExoAppInternal::ShowMouse(int flags) RET 4.
}
namespace kotor_game_prepare {
void __fastcall ShowMouse0061fa00(void* internal, void*, int flags) {
    const Word mask = get32(internal, 0x3D8) & ~static_cast<Word>(flags); // 0061FA07..0061FA11
    put32(internal, 0x3D8, mask);                                        // 0061FA13
    if (mask != 0) return;                                               // 0061FA19 jne -> RET 4
    if (flags & 0x4A) put32(internal, 0x3DC, 0x3F000000u);               // 0061FA1B..0061FA1F (0.5f)
    if (flags & 0x14) put32(internal, 0x3DC, 0xBF800000u);               // 0061FA29..0061FA2D (-1.0f)
    // 0061FA37..0061FA48: fcomp [0073D700]=0.0f; test ah,41h; jp -> return when
    // greater or unordered; continue when less or equal.
    const float v = getf(internal, 0x3DC);
    if (!(v < 0.0f || v == 0.0f)) return;
    kotor_fastcall_005df5f0(kotor_menu_window_cells::input_007a39e4, nullptr);   // 0061FA4A/0061FA50
    void* gui = ptr(get32(internal, 0x274));                              // 0061FA55
    if (gui && !(*static_cast<const unsigned char*>(ptr(get32(internal, 4))) & 2)) // 0061FA5F..0061FA65
        kotor_fastcall_0040b290(gui, nullptr);                            // 0061FA67
    const Word arg = static_cast<Word>(kotor_input_cells_required::word_008338f4);                    // 0061FA6C
    kotor_fastcall_005df5b0(kotor_menu_window_cells::input_007a39e4, nullptr, arg); // 0061FA72..0061FA7C tail
}
}
namespace kotor_game_prepare {
// 0062B410 CGuiInGame::SetGlobalUnpauseableState(int) RET 4: counter at +D8.
void __fastcall SetGlobalUnpauseableState0062b410(void* in_game, void*, client_services::Word enable) {
    if (enable == 1) { put32(in_game, 0xD8, get32(in_game, 0xD8) + 1); return; } // 0062B410..0062B424
    const Word n = get32(in_game, 0xD8);                                           // 0062B427
    if (n != 0) put32(in_game, 0xD8, n - 1);                                       // 0062B42F..0062B432 (jbe: unsigned)
}
}
