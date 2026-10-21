// Lane worker "upgrade": NWScript engine command ShowUpgradeScreen, reconstructed from
// the original swkotor.exe (read-only, capstone).
//   00543990 CSWVirtualMachineCommands::ExecuteCommandShowUpgradeScreen(int routine, int argc) thiscall RET 8
//            -> extern "C" int __fastcall kotor_fastcall_00543990(self, edx, routine, argc)  @16
// Pops one object (the item, OBJECT_INVALID = choose from the party) and hands it to CGuiInGame::ShowUpgradeScreen
// 0062E760 (srcw4 gui claude_upgrade_sel.cpp). The popped id is passed unconverted (server id, as the original does).
// Pop failure -> -2001 (0xFFFFF82F); otherwise 0. Original statement PCs in comments.
#include "54db204e3b-claude_vm_vmb_common.hpp"

extern "C" {
void* __fastcall frame_R4_005ed690(void* client, void*);                                   // CClientExoApp::GetInGameGui [0457]
void __fastcall kotor_fastcall_0062e760(void* gui, void*, U item_id);                       // CGuiInGame::ShowUpgradeScreen RET 4 [claude_upgrade_sel]
int __fastcall kotor_fastcall_00543990(void*, void*, int, unsigned);
}

using namespace vmb;

extern "C" {

// ===================================================================================================== 00543990
int __fastcall kotor_fastcall_00543990(void*, void*, int, unsigned) {
    U item = 0;                                                                                  // [esp] (push ecx slot)
    if (pop_object(&item) == 0) return k_pop_failed;                                            // 00543990..005439AB
    kotor_fastcall_0062e760(frame_R4_005ed690(client_app(), nullptr), nullptr, item);           // 005439AE..005439C3
    return 0;                                                                                    // 005439C8..005439CB
}

}  // extern "C"
