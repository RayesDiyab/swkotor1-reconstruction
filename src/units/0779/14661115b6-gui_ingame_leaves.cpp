// lane-m47-core: in-game GUI leaves referenced by the R4 entry router / suffix / lowers and the main menu:
//   0062AA00 CGuiInGame::SetSWGuiStatus (+ 00624A00 CSWGuiPanel flag setter), 00627260 CSWGuiMessageBox::
//   DisplayDeathMessage (+ 00625200 UpdateControls, 00625A60 OnDeath, 005EDE10 CClientExoApp::SetEndGamePending),
//   00627130 CSWGuiMessageBox::SetAllowCancel, 0062F4A0 CGuiInGame::ShowTutorialWindow (+ 006AA0D0
//   CSWGuiTutorialBox::SetTutorialInstigator), 006247A0 CSWGuiFade::ResetFade (+ 006246C0 CSWGuiFade::SetMsg).
// Link-required; none is reached on the main-menu first frame (no in-game GUI exists there; R4 no-player path D15).
// Evidence: lane dis/<addr>.asm; jump table 0062AB18 = {0062AA1C, 0062AA21, 0062AA2A, 0062AA35}.
// ABI: D1 form of every original __thiscall; consumer declarations: frame-gui entry_providers.h / suffix.h,
// r4-lowers r4_lowers.h, gui-load mainmenu.h.
#include "f9f7ab0530-startup_string.h"
#include "f43a5a7d3b-startup_views.h"
#include "a0f716d4dd-menu_targets.h"
#include "5c6f651f13-window_shared_cells.h"
#include <cstdint>
#include <cstring>

using kotor_reconstruction::StartupString;
namespace tlk_core { void __fastcall ParseStr0041dd30(kotor_menu_10_19::Tlk* self, void* unused_edx, StartupString* text); }
extern "C" {
extern void* kotor_cell_tlk_007a3a08;                                                     // lane-tlk tlk_cells (D44)
void __fastcall kotor_fastcall_0040bc70(void* gui, void*, void* panel, std::uint32_t, std::uint32_t) noexcept(false); // AddPanel
unsigned __fastcall frame_ContainsPanel_0040bd70(void* gui, void*, void* panel);          // PanelExists
void __fastcall frame_RemovePanel_0040c830(void* gui, void*, void* panel);                // RemovePanel
void __fastcall kotor_fastcall_0040bd20(void* gui, void*, void* panel);                   // SendPanelToBack
void* __fastcall kotor_fastcall_005eda40(void* client, void*);                            // GetSWGuiManager
unsigned __int64 __fastcall kotor_fastcall_004adc70(const void* timer, void*);           // CWorldTimer::GetSnapshotTime
void __fastcall gui_load_text_params_set_text_00415e00(void*, void*, const StartupString*) noexcept(false);
const char* __cdecl c2da_empty_0073d71c();
}

namespace {
using U32 = std::uint32_t;
U32 w(const void* p, U32 off) { U32 v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
void sw(void* p, U32 off, U32 v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4); }
void* pw(const void* p, U32 off) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(w(p, off))); }
unsigned char& b8(void* p, U32 off) { return static_cast<unsigned char*>(p)[off]; }
void* client() { return pw(kotor_menu_window_cells::manager_007a39fc, 4); }               // [[007A39FC]+4]
// Original bit-field merge `x ^ ((x ^ v) & mask)` (e.g. 0062520B..10).
U32 merge(U32 x, U32 v, U32 mask) { return x ^ ((x ^ v) & mask); }
}

extern "C" {
// ---------------------------------------------------------------- panels
// 00624A00 CSWGuiPanel::(set-to-destroy) (flag) RET 4: bit 8 = flag; unless (flags & 600h) == 400h: -400h, +200h.
__declspec(noinline) void __fastcall kotor_fastcall_00624a00(void* panel, void*, U32 flag) {
    U32 flags = merge(w(panel, 0x44), flag << 8, 0x100u);                         // 00624A00..13
    sw(panel, 0x44, flags);
    if ((flags & 0x600u) != 0x400u) sw(panel, 0x44, (flags & 0xFFFFFBFFu) | 0x200u); // 00624A16..30
}
// 0062AA00 CGuiInGame::SetSWGuiStatus(status, flag) RET 8.
__declspec(noinline) void __fastcall frame_R4_0062aa00(void* self, void*, unsigned status, unsigned flag) {
    switch (status) {                                                             // 0062AA07..15 (status-1 > 3 -> return)
    case 1: if (w(self, 0x34) != 3) sw(self, 0x34, 1); break;                    // 0062AA1C / 0062AA39..3B
    case 2: sw(self, 0x34, 2); break;                                             // 0062AA21
    case 3: if (w(self, 0x34) != 2) sw(self, 0x34, 3); break;                    // 0062AA2A..30
    case 4: if (w(self, 0x34) != 2) sw(self, 0x34, 1); break;                    // 0062AA35..3B
    default: return;                                                              // 0062AA0F -> 0062AB12
    }
    if (w(self, 0x108) == 0 || pw(self, 0x38) == nullptr) return;                // 0062AA42..55
    if (w(self, 0x34) == 1) {                                                     // 0062AA5B..5F
        if (!frame_ContainsPanel_0040bd70(pw(self, 0x38), nullptr, pw(self, 0x90))) {   // 0062AA61..6F
            kotor_fastcall_0040bc70(pw(self, 0x38), nullptr, pw(self, 0x90), 4u, 1u);    // 0062AA71..7F
            kotor_fastcall_0040bd20(pw(self, 0x38), nullptr, pw(self, 0x90));            // 0062AA84..8E
        }
        if (w(self, 0xB3C) == 0 || pw(self, 0x7c) == nullptr) return;            // 0062AA93..A2
        if (frame_ContainsPanel_0040bd70(pw(self, 0x38), nullptr, pw(self, 0x7c))) return; // 0062AAA4..AF
        kotor_fastcall_0040bc70(pw(self, 0x38), nullptr, pw(self, 0x7c), 4u, 1u);       // 0062AAB1..BC
        kotor_fastcall_0040bd20(pw(self, 0x38), nullptr, pw(self, 0x7c));               // 0062AAC1..C8
        return;
    }
    if (frame_ContainsPanel_0040bd70(pw(self, 0x38), nullptr, pw(self, 0x90)))   // 0062AAD1..DF
        frame_RemovePanel_0040c830(pw(self, 0x38), nullptr, pw(self, 0x90));     // 0062AAE1..EB
    if (flag == 0) return;                                                        // 0062AAF0..F6
    if (!frame_ContainsPanel_0040bd70(pw(self, 0x38), nullptr, pw(self, 0x7c))) return; // 0062AAF8..06
    kotor_fastcall_00624a00(pw(self, 0x7c), nullptr, 0u);                        // 0062AB08..0D
}

// ---------------------------------------------------------------- message box
// 00625200 CSWGuiMessageBox::UpdateControls: three button-state bits from [this+64h].
__declspec(noinline) void __fastcall kotor_fastcall_00625200(void* box, void*) {
    sw(box, 0x338, merge(w(box, 0x338), w(box, 0x64) >> 1, 2u));                 // 00625200..12
    const U32 both = ((w(box, 0x64) & 2u) != 0 && (w(box, 0x64) & 4u) != 0) ? 1u : 0u; // 00625218..2C
    sw(box, 0x4FC, merge(w(box, 0x4FC), both + both, 2u));                       // 0062522C..3C
    sw(box, 0x1F8, merge(w(box, 0x1F8), w(box, 0x64) >> 3, 2u));                 // 00625242..55
}
// 00627130 CSWGuiMessageBox::SetAllowCancel(allow) RET 4.
__declspec(noinline) void __fastcall kotor_fastcall_00627130(void* box, void*, std::uint32_t allow) {
    sw(box, 0x64, merge(w(box, 0x64), allow + allow, 2u));                        // 00627134..4D
    b8(box, 0x60) = allow == 0 ? 0x02 : 0x0F;                                     // 00627142..50 (sete/dec/and 0Dh/add 2)
    kotor_fastcall_00625200(box, nullptr);                                        // 00627153
}
// 005EDE10 CClientExoApp::SetEndGamePending(a, b, c) RET 0Ch: [[this+4]+1A8h/374h/1ACh] (raw dwords).
__declspec(noinline) void __fastcall kotor_fastcall_005ede10(void* app, void*, U32 a, U32 b, U32 c) {
    void* const internal = pw(app, 4);                                            // 005EDE10
    sw(internal, 0x1A8, a);                                                       // 005EDE1B
    sw(internal, 0x374, b);                                                       // 005EDE25
    sw(internal, 0x1AC, c);                                                       // 005EDE2B
}
// 00625A60 CSWGuiMessageBox::OnDeath(CSWGuiControl*) RET 4 (ECX unused): SetEndGamePending(1, 0, 1).
__declspec(noinline) void __fastcall kotor_fastcall_00625a60(void*, void*, void*) {
    kotor_fastcall_005ede10(client(), nullptr, 1u, 0u, 1u);                       // 00625A60..6E (push 1; push 0; push 1)
}
// 00627260 CSWGuiMessageBox::DisplayDeathMessage.
__declspec(noinline) void __fastcall frame_R4_00627260(void* box, void*) {
    sw(box, 0x64, w(box, 0x64) & 0xFFFFFFFDu);                                    // 00627263
    b8(box, 0x60) = 2;                                                            // 00627267
    kotor_fastcall_00625200(box, nullptr);                                        // 0062726B
    using SetTextSlot = void (__fastcall*)(void*, void*, U32);                    // [vtbl+6Ch](strref), RET 4
    SetTextSlot slot; std::memcpy(&slot, static_cast<const unsigned char*>(pw(box, 0)) + 0x6c, sizeof(slot));
    slot(box, nullptr, 0xA56Fu);                                                  // 00627270..77
    sw(box, 0x6c, static_cast<U32>(reinterpret_cast<std::uintptr_t>(box)));      // 00627282 handler object
    void (__fastcall* const handler)(void*, void*, void*) = &kotor_fastcall_00625a60;
    std::memcpy(static_cast<unsigned char*>(box) + 0x68, &handler, 4);            // 00627285 handler 00625A60
    sw(box, 0x70, 0);                                                             // 0062728C
    kotor_fastcall_0040bc70(pw(box, 0x18), nullptr, box, 1u, 1u);                 // 0062727A..93
}

// ---------------------------------------------------------------- tutorial
// 006AA0D0 CSWGuiTutorialBox::SetTutorialInstigator(a, b, c) RET 0Ch.
__declspec(noinline) void __fastcall kotor_fastcall_006aa0d0(void* box, void*, U32 a, U32 b, U32 c) {
    sw(box, 0x984, a); sw(box, 0x988, b); sw(box, 0x98C, c);                      // 006AA0D8..E8
}
// 0062F4A0 CGuiInGame::ShowTutorialWindow(byte id, a, b, c) RET 10h; EAX 1 when shown.
__declspec(noinline) unsigned __fastcall frame_ShowTutorial_0062f4a0(void* self, void*, unsigned tutorial, unsigned a,
                                                                      unsigned b, unsigned c) {
    kotor_menu20::ClientApp* const app = static_cast<kotor_menu20::ClientApp*>(client());
    if ((b8(app->get_options_005ed700(), 0x14) & 2u) == 0) return 0;              // 0062F4A0..B5
    const unsigned char id = static_cast<unsigned char>(tutorial);               // 0062F4BB (BL)
    if (id >= 0x2B) return 0;                                                     // 0062F4BF..C2
    const unsigned char index = static_cast<unsigned char>(id >> 3);             // 0062F4C8..CA
    const unsigned bit = static_cast<unsigned>(id - static_cast<unsigned char>(index << 3)); // 0062F4CF..D6
    if (((1u << bit) & b8(self, 0xBA8 + index)) != 0) return 0;                  // 0062F4D8..F0
    b8(self, 0xBA8 + index) = static_cast<unsigned char>(b8(self, 0xBA8 + index) | static_cast<unsigned char>(1u << bit)); // 0062F4F6..1A
    kotor_fastcall_006aa0d0(pw(self, 0xA0), nullptr, a, b, c);                    // 0062F516..2D
    if (!frame_ContainsPanel_0040bd70(kotor_fastcall_005eda40(client(), nullptr), nullptr, pw(self, 0xA0)))  // 0062F532..50
        kotor_fastcall_0040bc70(kotor_fastcall_005eda40(client(), nullptr), nullptr, pw(self, 0xA0), 1u, 1u); // 0062F552..6C
    return 1;
}

// ---------------------------------------------------------------- fade
// 006246C0 CSWGuiFade::SetMsg(strref) RET 4: TLK string (parsed) or "" into the text params at +15Ch.
__declspec(noinline) void __fastcall kotor_fastcall_006246c0(void* fade, void*, U32 strref) noexcept(false) {
    StartupString text;                                                           // 006246DB..DF (state 0)
    void* const params = static_cast<unsigned char*>(fade) + 0x15C;
    if (strref != 0) {                                                            // 006246E4..F2
        alignas(StartupString) unsigned char storage[sizeof(StartupString)];      // hidden-return temporary [esp+0Ch]
        auto* const tlk = static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08);
        kotor_menu_10_19::String* const fetched = tlk->simple_0041e8f0(
            reinterpret_cast<kotor_menu_10_19::String*>(storage), strref);       // 006246F4..700
        struct Temp { StartupString* s; ~Temp() { s->~StartupString(); } } temp{reinterpret_cast<StartupString*>(storage)}; // state 1
        text.Assign005e5c50(reinterpret_cast<StartupString*>(fetched));           // 00624705..0F
    }                                                                             // 00624714..1D ~temp
    if (strref != 0) {
        tlk_core::ParseStr0041dd30(static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08), nullptr, &text); // 00624722..2D
        gui_load_text_params_set_text_00415e00(params, nullptr, &text);           // 00624732..3D
    } else {
        StartupString empty(c2da_empty_0073d71c());                               // 00624744..4D (state 2)
        gui_load_text_params_set_text_00415e00(params, nullptr, &empty);          // 00624752..62
    }                                                                             // 00624767..70 ~empty
}                                                                                 // 00624775..81 ~text
// 006247A0 CSWGuiFade::ResetFade.
__declspec(noinline) void __fastcall kotor_fastcall_006247a0(void* fade, void*) {
    sw(fade, 0x50, 0); sw(fade, 0x54, 0); sw(fade, 0x58, 0);                     // 006247AB..B5
    sw(fade, 0x4C, 0);                                                            // 006247BB
    kotor_fastcall_006246c0(fade, nullptr, 0u);                                   // 006247B8..CA
    sw(fade, 0x64, 0); sw(fade, 0x68, 0); sw(fade, 0x6C, 0); sw(fade, 0x70, 0);  // 006247CF..D8
    void* const timer = pw(pw(client(), 4), 0x2C);                               // 006247DB..E7
    const std::int64_t now = static_cast<std::int64_t>(kotor_fastcall_004adc70(timer, nullptr)); // 006247EA
    sw(fade, 0x1B4, static_cast<U32>(now / 1000));                                // 006247EF..FD (__alldiv by 3E8h)
}
}
