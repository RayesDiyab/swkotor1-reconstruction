// lane-window: DeactivateRenderWindow 00401D90, ActivateRenderWindow 00401E00,
// CClientOptions::GetGammaSetting 0061D590 and the render-active cell 007A3A38.
// Evidence: dumpbin /disasm swkotor.exe (GOG) 00401D90..00401E83, 0061D590..0061D5AF.
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Pinned MSVC x86 only
#endif
#include "7648bf18bf-renderer_adoption.h"
#include "369787b919-window_shared_cells.h"
#include "a767de49a4-services.h"        // client_services::Client::input_005eda10 (M41 client-services.obj)
#include "66a5366142-menu_targets.h"    // kotor_menu20::ClientApp::get_options_005ed700 (M41 service-menu20-4.obj)
#include <cstdint>
#include <cstring>

using U32 = std::uint32_t;

extern "C" {
// 007A3A38 render-window-active word: .data image 00000000 (file offset 0x3A3A38).
// Writers: 00401DF2 (0), 00401E0E (1). Readers: 00401D90, 00401E00, 005F6F59, 005F713D,
// MainLoop 00602FE8/00603040. No other owner exists.
volatile U32 kotor_cell_render_active_007a3a38 = 0;
// Still-missing genuine providers (declarations only).
// 004AE980 CServerExoApp::GetPauseState -> 004B1D80 (name shared with frame R4 suffix.h).
unsigned __fastcall frame_GetPauseState_004ae980(void*, void*, unsigned);
// 004AE9A0 CServerExoApp::SetPauseState(type, state) -> 004B8110 (__thiscall, RET 8).
void __fastcall kotor_fastcall_004ae9a0(void* server, void*, U32 type, U32 state);
extern void* kotor_cell_sound_007a39ec;   // lane-sound sound_owners.cpp (D25)
// Exported entries (lane-routes kotor_original_calls.h names).
void __cdecl kotor_cdecl_00401d90();
void __cdecl kotor_cdecl_00401e00();
double __fastcall kotor_fastcall_0061d590(const void* options, void*);
}
namespace table29_required {   // lane-sound donor (task-29 sound-mode-closure-r2), 005D5E80
void __fastcall gui_sound_mode_005d5e80(void* outer, void* unused_edx, std::uint32_t mode);
}
namespace sound_mode29_required {   // lane-sound sound_owners.cpp views (.rdata 0074C600/0074C604)
extern const volatile std::uint32_t* const word_0074c600;
extern const volatile std::uint32_t* const word_0074c604;
}

namespace {
void* pointer_at(const void* object, unsigned offset) { void* v; std::memcpy(&v, static_cast<const unsigned char*>(object) + offset, 4); return v; }
void store_word(void* object, unsigned offset, U32 v) { std::memcpy(static_cast<unsigned char*>(object) + offset, &v, 4); }
U32 word_at(const void* object, unsigned offset) { U32 v; std::memcpy(&v, static_cast<const unsigned char*>(object) + offset, 4); return v; }
// CAppManager (0x20 allocation at 004042B9): +4 client, +8 server, +1C saved pause state.
void* manager() { return kotor_menu_window_cells::manager_007a39fc; }
}

// 0061D590 (__thiscall, RET 0, ST0 result): [+20] - byte[+18]*0.01f*([+20]-[+1C]).
// Original x87 runs at the CRT 53-bit precision control, so double arithmetic is the same
// operation sequence; the value stays unrounded in ST0 (double return) as in the original.
extern "C" double __fastcall kotor_fastcall_0061d590(const void* options, void*) {
    const unsigned char* o = static_cast<const unsigned char*>(options);
    float high, low; std::memcpy(&high, o + 0x20, 4); std::memcpy(&low, o + 0x1c, 4);
    const int level = o[0x18];                                       // 0061D591 movzx eax,byte [ecx+18h]
    const float scale_0073dfec = 0.00999999977648258209228515625f;   // 0073DFEC bits 3C23D70A
    double range = static_cast<double>(high) - static_cast<double>(low); // 0061D595..0061D598
    double factor = static_cast<double>(level) * static_cast<double>(scale_0073dfec); // 0061D59F..0061D5A3
    double product = factor * range;                                 // 0061D5A9 fmulp
    return static_cast<double>(high) - product;                      // 0061D5AB fsubr [ecx+20h]
}

// 00401D90 DeactivateRenderWindow (cdecl, no args).
extern "C" void __cdecl kotor_cdecl_00401d90() {
    if (kotor_cell_render_active_007a3a38 == 0) return;             // 00401D90..00401D97
    void* app = manager();                                           // 00401D99
    if (void* server = pointer_at(app, 8)) {                         // 00401D9E..00401DA3
        const U32 paused = frame_GetPauseState_004ae980(server, nullptr, 2); // 00401DA5..00401DA7
        store_word(manager(), 0x1c, paused);                         // 00401DAE..00401DB4 (manager reloaded)
        if (paused == 0)                                             // 00401DAC test / 00401DB7 jne
            kotor_fastcall_004ae9a0(pointer_at(manager(), 8), nullptr, 2, 1); // 00401DB9..00401DC6
    }
    static_cast<client_services::Client*>(pointer_at(manager(), 4))->input_005eda10(0, 0); // 00401DCB..00401DD7
    if (void* sound = kotor_cell_sound_007a39ec)                     // 00401DDC..00401DE4
        table29_required::gui_sound_mode_005d5e80(sound, nullptr, *sound_mode29_required::word_0074c600); // 00401DE6..00401DED
    kotor_cell_render_active_007a3a38 = 0;                           // 00401DF2
}

// 00401E00 ActivateRenderWindow (cdecl, no args).
extern "C" void __cdecl kotor_cdecl_00401e00() {
    if (kotor_cell_render_active_007a3a38 != 0) return;             // 00401E00..00401E07
    void* app = manager();                                           // 00401E09
    kotor_cell_render_active_007a3a38 = 1;                           // 00401E0E
    if (void* server = pointer_at(app, 8)) {                         // 00401E18..00401E1D
        if (word_at(app, 0x1c) == 0)                                 // 00401E1F..00401E24
            kotor_fastcall_004ae9a0(server, nullptr, 2, 0);          // 00401E26..00401E2A
        app = manager();                                             // 00401E2F reload
    }
    static_cast<client_services::Client*>(pointer_at(app, 4))->input_005eda10(1, 0); // 00401E34..00401E3B
    if (void* sound = kotor_cell_sound_007a39ec)                     // 00401E40..00401E48
        table29_required::gui_sound_mode_005d5e80(sound, nullptr, *sound_mode29_required::word_0074c604); // 00401E4A..00401E50
    // 00401E55..00401E7F: aurora table read before the option calls; aurora reloaded as receiver.
    void* client = pointer_at(manager(), 4);                         // 00401E55/00401E61
    void* aurora_first = kotor_menu_window_cells::aurora_007a39f8;   // 00401E5B
    const renderer_adoption::VTable* table = reinterpret_cast<const renderer_adoption::VTable*>(
        static_cast<std::uintptr_t>(static_cast<kotor_renderer_init::Renderer*>(aurora_first)->vtable_word)); // 00401E65 mov esi,[edx]
    // 00401E67 push 0 is not consumed by 005ED700 (RET 0); it is the second ResetGammaRamp word.
    void* options = static_cast<kotor_menu20::ClientApp*>(client)->get_options_005ed700(); // 00401E69
    const float gamma = static_cast<float>(kotor_fastcall_0061d590(options, nullptr)); // 00401E70; 00401E7C fstp dword
    U32 gamma_bits; std::memcpy(&gamma_bits, &gamma, 4);
    auto* aurora = static_cast<kotor_renderer_init::Renderer*>(kotor_menu_window_cells::aurora_007a39f8); // 00401E76 reload
    (void)table->gamma(aurora, nullptr, gamma_bits, 0);              // 00401E7F call [esi+24h] (pops gamma + the 0)
}
