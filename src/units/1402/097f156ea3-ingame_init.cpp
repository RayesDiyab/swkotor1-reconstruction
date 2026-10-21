// Lane W4 G15: CGuiInGame::Initialize 00632860 and its client wrappers 005ED730 / 005F1410, CClientExoApp::GetLoadStep
// 005EDFF0. Transcribed from dis/00632860.asm. Panel constructors are W4 bodies (src/gui/panels_*.cpp) or the
// existing runtime1115 constructors (MessageBox, SkillInfoBox, TutorialBox, Fade).
#include "37dfeb87ca-gui_panels.hpp"
using namespace w4gui;

namespace {
// `new T(manager)` with the original EH: operator delete (_free 006FA390 -> live_free) if the constructor throws.
using Ctor = void* (__fastcall*)(void*, void*, void*);
void* make(U32 size, Ctor ctor, void* manager) {
    void* memory = live_operator_new(size);                                      // call 006FA7E6
    if (memory == nullptr) return nullptr;                                       // test eax,eax; je -> xor eax,eax
    { auto w4_guard_0 = ::w4gui::on_throw([&]() { live_free(memory); }); void* w4_result_1 = ctor(memory, nullptr, manager); w4_guard_0.armed = false; return w4_result_1; }
}
void* manager_of(void* in_game) { return lp(in_game, 0x38); }

// Load-bar step 1..4: progress = trunc(v1 + float(v0 + float(v3 * scale))) with v = GetLoadStep(3/0/1).
void load_bar(float scale) {
    void* app = client_app();                                                    // esi = [[007A39FC]+4]
    const U32 v3 = kotor_fastcall_005edff0(app, nullptr, 3);
    const float f1 = static_cast<float>(static_cast<double>(static_cast<I32>(v3)) * scale);   // fild; fmul; fstp
    const U32 v0 = kotor_fastcall_005edff0(app, nullptr, 0);
    const float f2 = static_cast<float>(static_cast<double>(static_cast<I32>(v0)) + f1);       // fild; fadd; fstp
    const U32 v1 = kotor_fastcall_005edff0(app, nullptr, 1);
    const I32 progress = static_cast<I32>(static_cast<double>(static_cast<I32>(v1)) + f2);     // fild; fadd; __ftol2
    kotor_m15_client_set_load_bar_progress_005edd40(app, nullptr, progress, 1);
    kotor_cdecl_00401c10(bits_f(0x3D088889u), 0, 0);                            // UpdateScreen(1/30, 0, 0)
}
// Final load-bar step (00633214..00633280): trunc(v3 + float(v1 + float(v0))).
void load_bar_final() {
    void* app = client_app();
    const U32 v0 = kotor_fastcall_005edff0(app, nullptr, 0);
    const float f1 = static_cast<float>(static_cast<I32>(v0));
    const U32 v1 = kotor_fastcall_005edff0(app, nullptr, 1);
    const float f2 = static_cast<float>(static_cast<double>(static_cast<I32>(v1)) + f1);
    const U32 v3 = kotor_fastcall_005edff0(app, nullptr, 3);
    const I32 progress = static_cast<I32>(static_cast<double>(static_cast<I32>(v3)) + f2);
    kotor_m15_client_set_load_bar_progress_005edd40(app, nullptr, progress, 1);
    kotor_cdecl_00401c10(bits_f(0x3D088889u), 0, 0);
}
}  // namespace

extern "C" {
// 005EDFF0 CClientExoApp::GetLoadStep(int) (thiscall, RET 4; returns AL).
U8 __fastcall kotor_fastcall_005edff0(void* app, void*, U32 which) {
    return lb(lp(app, 4), 0x3E4 + which);                                       // 005EDFF0..005EDFF7
}

// 00632860 CGuiInGame::Initialize() (thiscall, RET 0; returns in_game+108h).
U32 __fastcall kotor_fastcall_00632860(void* g, void*) {
    if (lw(g, 0x108) != 0) return lw(g, 0x108);                                 // 0063287B..00632883
    if (lw(g, 0x38) == 0) sw(g, 0x38, ad(kotor_fastcall_005eda40(client_app(), nullptr))); // 00632889..0063289D
    sw(g, 0x58, ad(make(0x488, &kotor_fastcall_006d0620, manager_of(g))));      // 006328A0..006328DA CSWGuiCreateItemMenu
    sw(g, 0x70, ad(make(0x48C, &kotor_fastcall_006cfa70, manager_of(g))));      // 006328D1..0063290D CSWGuiLoadModuleDebugMenu
    sw(g, 0x74, ad(make(0x490, &kotor_fastcall_006cf5a0, manager_of(g))));      // 00632904..00632937 CSWGuiPowersFeatsSkillsDebugMenu
    load_bar(bits_f(0x3E4CCCCDu));                                              // 0063293A..006329B1 (0.2f)
    sw(g, 0x5C, ad(make(0x4BC, &kotor_fastcall_006bdc60, manager_of(g))));      // 006329B6..006329EB CSWGuiCreateItemSubMenu
    sw(g, 0x50, ad(make(0x988, &kotor_fastcall_006ce9c0, manager_of(g))));      // 006329E2..00632A1E CSWGuiExamine
    sw(g, 0x4C, ad(make(0x1CC, &kotor_fastcall_006a9770, manager_of(g))));      // 00632A15..00632A51 CSWGuiBarkBubble
    sw(g, 0x54, ad(make(0x101C, &kotor_fastcall_006b6dc0, manager_of(g))));     // 00632A48..00632A84 CSWGuiContainer
    sw(g, 0x40, ad(make(0x1E00, &kotor_fastcall_006a8b40, manager_of(g))));     // 00632A7B..00632AAE CSWGuiDialogCinematic
    if (ingame_gui_single_panel_007a2370 != 0)                                  // 00632AB1..00632ABC
        sw(g, 0x44, ad(make(0x3620, &kotor_fastcall_006a8eb0, manager_of(g)))); // 00632ABE..00632AEE CSWGuiDialogComputer
    sw(g, 0x48, ad(make(0x2080, &kotor_fastcall_006a95f0, manager_of(g))));     // 00632AF1..00632B26 CSWGuiDialogComputerCamera
    sw(g, 0x60, ad(make(0xA4, &kotor_fastcall_006a8930, manager_of(g))));       // 00632B1D..00632B50 CSWGuiDialogLetterbox
    if (lw(g, 0x98) == 0)                                                       // 00632B53..00632B5F
        sw(g, 0x98, ad(make(0x984, &kotor_original::kotor_fastcall_00626df0, manager_of(g)))); // CSWGuiMessageBox
    if (lw(g, 0x9C) == 0)                                                       // 00632B97..00632B9F
        sw(g, 0x9C, ad(make(0x24E8, &kotor_original::kotor_fastcall_006ce7c0, manager_of(g)))); // CSWGuiSkillInfoBox
    if (lw(g, 0xA0) == 0)                                                       // 00632BD7..00632BDF
        sw(g, 0xA0, ad(make(0x998, &kotor_original::kotor_fastcall_006aa100, manager_of(g)))); // CSWGuiTutorialBox
    if (lw(g, 0xA4) == 0) {                                                     // 00632C17..00632C1F
        void* box = make(0x984, &kotor_original::kotor_fastcall_00626df0, manager_of(g));     // 00632C21..00632C46
        if (box != nullptr) set_vptr(box, kotor_vtable_007513f8);               // 00632C4B (CSWGuiControllerLossBox table)
        sw(g, 0xA4, ad(box));                                                   // 00632C59
    }
    load_bar(bits_f(0x3ECCCCCDu));                                              // 00632C5F..00632CD2 (0.4f)
    sw(g, 0x64, ad(make(0xA4, &kotor_fastcall_006a8930, manager_of(g))));       // 00632CD7..00632D0C CSWGuiDialogLetterbox
    sw(g, 0x68, ad(make(0xA4, &kotor_fastcall_006a8930, manager_of(g))));       // 00632D03..00632D3F CSWGuiDialogLetterbox
    sw(g, 0x6C, ad(make(0x1B8, &gui_fade_construct_00624810, manager_of(g))));  // 00632D36..00632D72 CSWGuiFade
    sw(g, 0x08, ad(make(0x1C24, &kotor_fastcall_00627980, manager_of(g))));     // 00632D69..00632DA5 CSWGuiInGameMenu
    sw(g, 0x7C, ad(make(0x4B0, &kotor_fastcall_006c03b0, manager_of(g))));      // 00632D9C..00632DD8 CSWGuiInGamePause
    sw(g, 0x8C, ad(make(0xFC8, &kotor_fastcall_006c2270, manager_of(g))));      // 00632DCF..00632E0B CSWGuiInGameSoloModeQuery
    sw(g, 0x94, ad(make(0x438, &kotor_fastcall_006c7d50, manager_of(g))));      // 00632E02..00632E38 CSWGuiInGameAreaTransition
    if (ingame_gui_single_panel_007a2370 != 0) {                                // 00632E3E..00632E49
        sw(g, 0x78, ad(make(0x3A80, &kotor_fastcall_006bfa40, manager_of(g)))); // CSWGuiPartySelection
        sw(g, 0x84, ad(make(0x2280, &kotor_fastcall_006c1c00, manager_of(g)))); // CSWGuiStore
        sw(g, 0x0C, ad(make(0x42BC, &kotor_fastcall_006ba980, manager_of(g)))); // CSWGuiInGameEquip
        sw(g, 0x10, ad(make(0x1DE8, &kotor_fastcall_006b34c0, manager_of(g)))); // CSWGuiInGameInventory
    }
    load_bar(bits_f(0x3F19999Au));                                              // 00632F1E..00632F91 (0.6f)
    if (ingame_gui_single_panel_007a2370 != 0)                                  // 00632F96..00632FA0
        sw(g, 0x14, ad(make(0x59F8, &kotor_fastcall_006b0e40, manager_of(g)))); // CSWGuiInGameCharacter
    void* summary = make(0x1B44, &kotor_fastcall_006272a0, manager_of(g));      // 00632FD5..00632FFF CSWGuiStatusSummary
    sw(g, 0xA8, ad(summary));                                                   // 0063300D
    if (void* saved = lp(g, 0xAC)) std::memcpy(static_cast<U8*>(summary) + 0x64, saved, 24);   // 00633001..0063301E rep movsd x6
    sw(g, 0x90, ad(make(0xC8CC, &kotor_fastcall_0068c100, manager_of(g))));     // 00633021..0063304D CSWGuiMainInterface
    load_bar(bits_f(0x3F4CCCCDu));                                              // 00633053..006330CA (0.8f)
    if (ingame_gui_single_panel_007a2370 != 0) {                                // 006330CF..006330D9
        sw(g, 0x24, ad(make(0x10DC, &kotor_fastcall_00694d50, manager_of(g)))); // CSWGuiInGameMap
        sw(g, 0x18, ad(make(0x3F98, &kotor_fastcall_006adda0, manager_of(g)))); // CSWGuiInGameAbilities
        sw(g, 0x20, ad(make(0xFC8, &kotor_fastcall_00644a40, manager_of(g))));  // CSWGuiInGameJournal
        sw(g, 0x28, ad(make(0x1DAC, &kotor_fastcall_006ab2f0, manager_of(g)))); // CSWGuiInGameOptions
        sw(g, 0x1C, ad(make(0xAF4, &kotor_fastcall_00626400, manager_of(g))));  // CSWGuiInGameMessages
        sw(g, 0x80, ad(make(0x2550, &kotor_fastcall_00695180, manager_of(g)))); // CSWGuiInGameGalaxyMap
    }
    load_bar_final();                                                           // 00633214..00633280
    sw(g, 0x3C, lw(g, 0x40));                                                   // 00633285..0063328C
    sw(g, 0x108, 1);                                                            // 0063328F
    return lw(g, 0x108);                                                        // 0063329A
}

// 005F1410 CClientExoAppInternal::InitializeInGameGUI(): mov ecx,[ecx+40h]; jmp 00632860.
U32 __fastcall kotor_fastcall_005f1410(void* client, void*) { return kotor_fastcall_00632860(lp(client, 0x40), nullptr); }
// 005ED730 CClientExoApp::InitializeInGameGUI(): mov ecx,[ecx+4]; jmp 005F1410.
U32 __fastcall kotor_fastcall_005ed730(void* app, void*) { return kotor_fastcall_005f1410(lp(app, 4), nullptr); }
}
