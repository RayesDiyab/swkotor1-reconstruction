// lane-m48-traps (main): lane-config required providers (declared in lane-config/lane_config_tables.cpp,
// namespace lane_config_required; repeated here exactly). Evidence: lane-m48-traps/TRAPS.json.
#include "5907a9dfa7-kotor_unreachable_trap.h"
#include <cstdint>
#include <cstring>

namespace lane_config_required {
void __fastcall AutoLevelUpNPCs_00563b90(void*, void*);
void __fastcall InGameReset_0062f5f0(void*, void*);
}

// transition lane (review F2): live providers of the member-loop callees.
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }   // [007A39FC] (0053-window-shared-cells)
extern "C" {
// 004AE770 CServerExoApp::GetCreatureByGameObjectID(id) thiscall RET 4 (0595-core47-server-app).
void* __fastcall kotor_fastcall_004ae770(void* server_app, void*, std::uint32_t object_id);
// 005A6810 CSWSCreatureStats::CanLevelUp() thiscall RET (w2_g07_native -> g07 body).
std::int32_t __fastcall recovered_can_level_up_005a6810(void* stats, void*);
// 005B27E0 CSWSCreatureStats::AutoLevelup(int) thiscall RET 4 (claude_levelup_rules).
void __fastcall kotor_fastcall_005b27e0(void* stats, void*, std::int32_t flag);
}

namespace lane_config_required {
// 00563B90..00563BE5 CSWPartyTable::AutoLevelUpNPCs (thiscall, RET). Full body (the member-loop trap is removed).
//   edi = this; ebp = i = 0; if ([this] <= 0) return (jle, signed).
//   ebx = this+4 walks one dword per iteration: npc = [this + 0Ch + [ebx]*4] (the index read is [ebx], i.e. the
//   i-th dword of the index array at this+4, not i itself); creature = [[007A39FC]+8]->GetCreatureByGameObjectID(npc);
//   if (creature && CanLevelUp(creature+A74 stats)) AutoLevelup(stats, 1); the count [this] is re-read every
//   iteration (00563BD7) and compared signed (jl 00563BDF).
void __fastcall AutoLevelUpNPCs_00563b90(void* self, void*) {
    unsigned char* const table = static_cast<unsigned char*>(self);
    std::int32_t count;
    std::memcpy(&count, table, sizeof count);                                  // 00563B94
    if (count <= 0) return;                                                    // 00563B98..00563B9A jle 00563BE3
    std::int32_t i = 0;                                                        // 00563B96 ebp
    const unsigned char* index_slot = table + 4;                               // 00563B9E lea ebx,[edi+4]
    do {
        unsigned char* const manager = static_cast<unsigned char*>(kotor_menu_window_cells::manager_007a39fc);  // 00563BA1
        std::int32_t index;
        std::memcpy(&index, index_slot, sizeof index);                         // 00563BA6 edx = [ebx]
        void* server_app;
        std::memcpy(&server_app, manager + 8, sizeof server_app);              // 00563BA8 ecx = [eax+8]
        std::uint32_t object_id;
        std::memcpy(&object_id, table + 0x0c + index * 4, sizeof object_id);   // 00563BAB [edi+edx*4+0Ch]
        unsigned char* const creature =
            static_cast<unsigned char*>(kotor_fastcall_004ae770(server_app, nullptr, object_id));   // 00563BAF..00563BB0
        if (creature != nullptr) {                                             // 00563BB7..00563BB9
            void* stats;
            std::memcpy(&stats, creature + 0xa74, sizeof stats);               // 00563BBB
            if (recovered_can_level_up_005a6810(stats, nullptr) != 0) {        // 00563BC1..00563BC8
                std::memcpy(&stats, creature + 0xa74, sizeof stats);           // 00563BCA (re-read)
                kotor_fastcall_005b27e0(stats, nullptr, 1);                    // 00563BD0..00563BD2
            }
        }
        std::memcpy(&count, table, sizeof count);                              // 00563BD7 (re-read)
        ++i;                                                                   // 00563BD9
        index_slot += 4;                                                       // 00563BDA
    } while (i < count);                                                       // 00563BDD..00563BDF jl
}
// 0062F5F0 CGuiInGame::ResetInterfaceForSize (thiscall, RET) - fullscreen lane 2026-10-20: full body (the whole-function
// trap is removed). Only caller CClientExoAppInternal::SetVideoMode 005F1A6B (client+40h != 0), i.e. Options > Graphics >
// Resolution (OnResolutionChosen 006DF713 -> CClientExoApp::SetVideoMode 005ED8D0) while the in-game GUI exists.
// The three panels that bake the screen size into their layout are rebuilt, the five centred ones re-centred:
//   +90h  CSWGuiMainInterface (operator new C8CCh, ctor 0068C100(GetSWGuiManager))     0062F609..0062F65B
//   +40h  CSWGuiDialogCinematic (operator new 1E00h, ctor 006A8B40(GetSWGuiManager))   0062F661..0062F6AE
//   CenterPanel 0040A600 on +58h, +70h, +74h, +5Ch                                    0062F6B1..0062F6DC
//   +98h  CSWGuiMessageBox (operator new 984h, ctor 00626DF0(GetSWGuiManager))        0062F6E1..0062F731
//   CenterPanel 0040A600 on +54h, +9Ch, +A0h                                          0062F737..0062F75E
// Each rebuild runs only when the old panel exists: its deleting destructor (vslot +00) with 1, then the allocation
// (operator new 006FA7E6 = game heap); a null allocation stores 0. The manager is re-read through
// [[007A39FC]+4] -> CClientExoApp::GetSWGuiManager 005EDA40 for every rebuild. EH states 0/1/2 (FuncInfo via 007293A1)
// free the allocation if the constructor throws (operator delete 006FA390 = live_free) and continue unwinding.
extern "C" void* __cdecl live_operator_new(std::uint32_t size);                          // 006FA7E6 (0004-live-policy)
extern "C" void __cdecl live_free(void* block);                                           // 006FA390 (0004-live-policy)
extern "C" {
void* __fastcall kotor_fastcall_005eda40(void* client_app, void*);                       // CClientExoApp::GetSWGuiManager (0417)
void* __fastcall kotor_fastcall_0068c100(void* self, void*, void* manager);               // CSWGuiMainInterface ctor (main_interface_ctor)
void* __fastcall kotor_fastcall_006a8b40(void* self, void*, void* manager);               // CSWGuiDialogCinematic ctor (1205-24-w4t1b_panels_dialog)
void __fastcall gui_m48_panel_center_0040a600(void* panel, void*);                       // CSWGuiPanel::CenterPanel (0699-m48b-gui-controls)
}
}   // namespace lane_config_required (reopened below)
namespace kotor_original { void* __fastcall kotor_fastcall_00626df0(void* self, void*, void* manager); }   // CSWGuiMessageBox ctor (0699)
namespace lane_config_required {
namespace {
using Ctor = void* (__fastcall*)(void*, void*, void*);
using DeletingDestructor = void* (__fastcall*)(void*, void*, std::uint32_t);
void* slot_at(void* gui, unsigned offset) { void* v; std::memcpy(&v, static_cast<unsigned char*>(gui) + offset, sizeof v); return v; }
void store_slot(void* gui, unsigned offset, void* v) { std::memcpy(static_cast<unsigned char*>(gui) + offset, &v, sizeof v); }
void* gui_manager() {                                                                     // [[007A39FC]+4] -> 005EDA40
    void* client_app;
    std::memcpy(&client_app, static_cast<unsigned char*>(kotor_menu_window_cells::manager_007a39fc) + 4, sizeof client_app);
    return kotor_fastcall_005eda40(client_app, nullptr);
}
void rebuild(void* gui, unsigned offset, std::uint32_t size, Ctor construct) {
    void* const old = slot_at(gui, offset);
    if (!old) return;                                                                     // test ecx,ecx / je
    DeletingDestructor destroy;
    std::memcpy(&destroy, *static_cast<void**>(old), sizeof destroy);                     // mov eax,[ecx]
    (void)destroy(old, nullptr, 1);                                                       // push 1; call [eax]
    void* const memory = live_operator_new(size);                                         // push size; call 006FA7E6
    void* created = nullptr;
    if (memory) {
        try {
            created = construct(memory, nullptr, gui_manager());                          // GetSWGuiManager; push eax; ctor
        } catch (...) {
            live_free(memory);                                                            // EH state unwind: operator delete
            throw;
        }
    }
    store_slot(gui, offset, created);                                                     // mov [esi+off],eax (0 when null)
}
void center(void* gui, unsigned offset) {
    if (void* const panel = slot_at(gui, offset)) gui_m48_panel_center_0040a600(panel, nullptr);
}
}
void __fastcall InGameReset_0062f5f0(void* gui, void*) {
    rebuild(gui, 0x90, 0xC8CC, &kotor_fastcall_0068c100);                                 // 0062F609..0062F65B
    rebuild(gui, 0x40, 0x1E00, &kotor_fastcall_006a8b40);                                 // 0062F661..0062F6AE
    center(gui, 0x58);                                                                    // 0062F6B1..0062F6B8
    center(gui, 0x70);                                                                    // 0062F6BD..0062F6C4
    center(gui, 0x74);                                                                    // 0062F6C9..0062F6D0
    center(gui, 0x5C);                                                                    // 0062F6D5..0062F6DC
    rebuild(gui, 0x98, 0x984, &kotor_original::kotor_fastcall_00626df0);                  // 0062F6E1..0062F731
    center(gui, 0x54);                                                                    // 0062F737..0062F73E
    center(gui, 0x9C);                                                                    // 0062F743..0062F74D
    center(gui, 0xA0);                                                                    // 0062F752..0062F75E
}
}
