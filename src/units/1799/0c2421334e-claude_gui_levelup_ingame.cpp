// Lane W4 GUI successor (worker "levelup"): CGuiInGame::ShowLevelUpGUI.
//   0062DC00 CGuiInGame::ShowLevelUpGUI(int flag)   thiscall RET 4, returns int (EAX 0/1)
//            -> extern "C" w4::U32 __fastcall kotor_fastcall_0062dc00(void* gui, void*, w4::U32 flag)  = @12
// Caller: VM handler 00543870 (srcw5 claude_vm_vmB_pending.cpp) with flag 1.
// Transcribed from the original swkotor.exe linear sweep; each statement carries its original PCs.
#include "6030a9ee65-claude_gui_char.hpp"

extern "C" {
void* __fastcall kotor_fastcall_005ed6f0(void* app, void*);                                         // CClientExoApp::GetSWCMessage
w4::U32 __fastcall frame_TogglePauseRequest_00677800(void* message, void*);                         // SendPlayerToServerInput_TogglePauseRequest
w4::U32 __fastcall kotor_fastcall_006b0bb0(void* self, void*, w4::U32 flag);                        // CSWGuiInGameCharacter::ShowLevelUpGUI (claude_gui_char_character.cpp)
extern const std::uint32_t kotor_rdata_sound_mode_0074c5fc;                                         // .rdata 0074C5FC (= 4)
w4::U32 __fastcall kotor_fastcall_0062dc00(void* gui, void*, w4::U32 flag);
}

using namespace gchar;

extern "C" w4::U32 __fastcall kotor_fastcall_0062dc00(void* gui, void*, w4::U32 flag) {
    if (lw(gui, 0x108) == 0) return 0;                                                          // 0062DC18..0062DC21
    if (lw(ingame_gui(), 0x10C) == 0) return 0;                                                 // 0062DC27..0062DC3C ([GetInGameGui()+10Ch])
    if (flag != 0) {                                                                            // 0062DC42..0062DC48
        if (lw(gui, 0xB38) == 0)                                                                // 0062DC4A..0062DC52 (not already paused)
            (void)frame_TogglePauseRequest_00677800(kotor_fastcall_005ed6f0(client_app(), nullptr), nullptr); // 0062DC54..0062DC64
        frame_R4_005d5e80(pt(frame_control::sound_manager_007a39ec), nullptr, kotor_rdata_sound_mode_0074c5fc); // 0062DC69..0062DC76
    } else if (kotor_fastcall_005ee190(client_app(), nullptr) != 0) {                           // 0062DC7D..0062DC8C GetCharacterChangeInProgress
        return 0;                                                                               // 0062DCFF..0062DD11
    }
    if (lw(gui, 0x14) == 0) {                                                                   // 0062DC8E..0062DC93 character panel not created yet
        void* raw = live_operator_new(0x59F8);                                                  // 0062DC95..0062DC9F
        void* panel = nullptr;                                                                  // 0062DCBF (null when the allocation failed)
        if (raw != nullptr) {                                                                   // 0062DCA6..0062DCB0
            auto guard = on_throw([&]() { live_free(raw); });                                   // EH state 0 (0072956B): free the block if the ctor throws
            panel = kotor_fastcall_006b0e40(raw, nullptr, lp(gui, 0x38));                      // 0062DCB2..0062DCB8 CSWGuiInGameCharacter(manager [this+38h])
            guard.armed = false;                                                                // 0062DCC1 state -1
        }
        sw(gui, 0x14, ad(panel));                                                               // 0062DCC9
    }
    (void)kotor_fastcall_006b0bb0(lp(gui, 0x14), nullptr, flag);                                // 0062DCCC..0062DCD0 (result discarded; panel re-read, unchecked)
    gui_m48_client_set_input_class_005eda60(client_app(), nullptr, 2, 1);                       // 0062DCD5..0062DCE2 SetInputClass(2, 1)
    return 1;                                                                                   // 0062DCE7..0062DCFC
}
