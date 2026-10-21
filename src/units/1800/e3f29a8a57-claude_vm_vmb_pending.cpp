// Lane vmB, file "pending": handlers whose callee is owned by another lane that has not landed yet.
// Link this object only once lane "levelup" provides @kotor_fastcall_0062dc00@12 (CGuiInGame::ShowLevelUpGUI).
// Original int __thiscall (int routine, int argc), RET 8 (verified) -> extern "C" __fastcall @16.
#include "54db204e3b-claude_vm_vmb_common.hpp"

extern "C" {
void* __fastcall frame_R4_005ed690(void* client, void*);                                   // CClientExoApp::GetInGameGui [0457]
U __fastcall kotor_fastcall_0062dc00(void* gui, void*, U flag);                           // CGuiInGame::ShowLevelUpGUI RET 4 [lane levelup]
}

using namespace vmb;

extern "C" {

// ===================================================================================================== 00543870
// ShowLevelUpGUI() [routine 265]: pushes CGuiInGame::ShowLevelUpGUI(1).  Callee owned by lane "levelup".
int __fastcall kotor_fastcall_00543870(void*, void*, int, unsigned) {
    const U shown = kotor_fastcall_0062dc00(frame_R4_005ed690(client_app(), nullptr), nullptr, 1); // 00543870..81
    return push_int(static_cast<I>(shown));                                              // 00543886..9B
}

}  // extern "C"
