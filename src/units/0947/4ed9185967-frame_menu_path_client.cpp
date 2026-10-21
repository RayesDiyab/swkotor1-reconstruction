// lane-m48-traps fork f-frame: per-frame client bodies reached on the main menu (D83).
// Faithful original control flow; only regions PROVEN unreachable under menu state call the shared D83 trap
// (kotor_unreachable_trap, lane include/). Every gate is registered in f-frame/FORK.json with its cell, initial
// value, initial writer and all writers. Evidence: dumpbin /disasm of swkotor.exe at the cited addresses.
//
// Menu-state premises (FORK.json "premises"):
//  P1 party count [CSWParty+0] == 0: ctor 00636170 stores 0 at 00636471; the only increment is
//     CSWParty::AddCharacter 0063672F, whose callers (SetPlayerCreature 005F2BB0, client UnloadModule 005F82CE,
//     CrownFutureLeader 005FB786, CSWCCreature::SetInParty 0060EF7C) all need a module/server/creature context.
//  P2 no client creature exists: CSWCCreature ctor 00616A20 is called only from server->client message handlers
//     (0064C24E, 0064C56D/706/95D, 00650E5D) and in-game/chargen GUI ctors (006B2087, 006DC984, 006E9183); the client
//     game-object lookup 005F1180 therefore returns 0 for every id (and client+0x20 = 7F000000, ctor 005FC216).
//  P3 CGuiInGame+0x108 == 0: ctor 0063008D (ebx = 0, 0062FEED); set to 1 only by CGuiInGame::Initialize 0063328F
//     <- InitializeInGameGUI 005F1413 <- CClientExoApp::InitializeInGameGUI 005ED733 <- HandleServerToPlayerModule 00668575.
//  P4 AppManager+8 (server) == 0 on the menu (ctor 00401181; only CreateServer 00401406, callers OnModulePicked /
//     SaveLoad::LoadGame / ClassSelection::BeginPlayGame).
#include <cstdint>
#include <cstring>
#include "82db854ff2-kotor_unreachable_trap.h"

namespace {
using U32 = std::uint32_t;
inline U32 lw(const void* o, U32 off) { U32 v; std::memcpy(&v, static_cast<const unsigned char*>(o) + off, 4); return v; }
[[maybe_unused]] inline void sw(void* o, U32 off, U32 v) { std::memcpy(static_cast<unsigned char*>(o) + off, &v, 4); }
[[maybe_unused]] inline void* lp(const void* o, U32 off) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(lw(o, off))); }
}

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }   // 007A39FC CAppManager* (M40 owner)

extern "C" {
// Existing composed-link providers (exact symbols; declarations as in their consumer headers).
unsigned __fastcall frame_R4_005ed5b0(void*, void*, unsigned);              // CClientExoApp::GetCreatureByGameObjectID (RET 4)
void* __fastcall frame_GetCharacter_006346c0(void*, void*, int);            // CSWParty::GetCharacter (RET 4)
void __fastcall kotor_fastcall_0062b000(void*, void*, U32, U32);            // CGuiInGame::SetMainInterfaceTarget (RET 8)

// Defined here.
void* __fastcall kotor_fastcall_00635460(void* party, void*);
void __fastcall kotor_fastcall_005f9c60(void* self, void*, void* object, U32, U32, U32);
void __fastcall frame_DialogUpdate_005fa5a0(void* self, void*, float);
void __fastcall frame_DialogCheck_005f3ad0(void* self, void*);
int __fastcall frame_R4_0062cba0(void* in_game, void*, unsigned);
void __fastcall frame_SetPaused_0062def0(void* in_game, void*, unsigned, unsigned);
void __fastcall frame_R4_00637050(void* party, void*, float);
void __cdecl frame_AurSnapshot_004211a0(void*, unsigned);
unsigned __fastcall client_internal_handle_message_005ee700(void*, void*, U32, unsigned char*, U32, U32);
unsigned __fastcall recovered_client_message_005ee700(void*, void*, U32, unsigned char*, U32, U32);
void __fastcall client_internal_shut_down_to_main_menu_005fcbd0(void*, void*);
}

namespace {
void* client_app() { return lp(kotor_menu_window_cells::manager_007a39fc, 4); }   // [[007A39FC]+4]
}

// 00635460 CSWParty::GetPlayerCharacter (thiscall, RET 0).
extern "C" void* __fastcall kotor_fastcall_00635460(void* party, void*) {
    if (static_cast<std::int32_t>(lw(party, 0)) <= 0) return nullptr;                 // 00635460 cmp [ecx],0; jle 00635478 (xor eax,eax)
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(
        frame_R4_005ed5b0(client_app(), nullptr, lw(party, 0x24))));                   // 00635465..00635472 GetCreatureByGameObjectID([party+24h])
}

// 005F9C60 CClientExoAppInternal::ShowObject(CSWCObject* object, int, int, int) (thiscall, RET 10h).
// Lane W4: trap definition removed; single owner src/party/selection.cpp (kotor_fastcall_005f9c60).
extern "C" void __fastcall kotor_fastcall_005f9c60(void* self, void*, void* object, U32, U32, U32);

// 005FA5A0 CClientExoAppInternal::DoPassiveSelection(float) (thiscall, RET 4).
// Lane W4: trap definition removed; single owner src/party/selection.cpp (frame_DialogUpdate_005fa5a0).
extern "C" void __fastcall frame_DialogUpdate_005fa5a0(void* self, void*, float);

// 005F3AD0 CClientExoAppInternal::UpdateCombatMode() (thiscall, RET 0).
// Lane W4: trap definition removed; single owner src/party/selection.cpp (frame_DialogCheck_005f3ad0).
extern "C" void __fastcall frame_DialogCheck_005f3ad0(void* self, void*);

// 0062CBA0 CGuiInGame::HideSWInGameGui(int) (thiscall, RET 4; returns 0/1).
// Lane W4: trap definition removed; single owner src/hud/ingame_gui.cpp (frame_R4_0062cba0).
extern "C" int __fastcall frame_R4_0062cba0(void* in_game, void*, unsigned);

// 0062DEF0 CGuiInGame::SetPaused(int, byte) (thiscall, RET 8).
// Lane W4: trap definition removed; single owner src/hud/ingame_gui.cpp (frame_SetPaused_0062def0).
extern "C" void __fastcall frame_SetPaused_0062def0(void* in_game, void*, unsigned, unsigned);

// ---- D83 whole-function traps (callers proven unreachable from WinMain -> first menu frame -> menu idle) ----

// 00637050 CSWParty::Update(float) (thiscall, RET 4). Sole caller MainLoop 0060339A, gated at 0060337D..00603391 by
// party != 0 && module(+18h) != 0 && module+48h (area) != 0; area is 0 on the menu (CSWCModule ctor 0064400F; only
// CSWCModule::LoadModule 0063F660 writes non-zero).
// Lane W4: trap definition removed; single owner src/party/party_update.cpp (frame_R4_00637050).
extern "C" void __fastcall frame_R4_00637050(void*, void*, float);

// 004211A0 AurSnapshot (cdecl). Sole caller MainLoop 00603E66, gated at 00603E2D..00603E35 by client+150h != 0
// (007A2284 is .data 1, never written). client+150h: ctor 005FC2FB = 0, MainLoop 00603E6E = 0, and set to 1 only by
// PlayBackInputEvents 00622762 (event playback, active only with the "-epf" command-line switch, 004017A1/004017E8).
// Lane snapshot: also set by the screenshot input event 50h (00621572; Print Screen, bound by SetEventDescriptions
// 005EED13..005EED65 only when [Game Options] EnableScreenShot=1). Trap definition removed; faithful body in
// srcw5 actions/claude_snapshot.cpp (frame_AurSnapshot_004211a0).
extern "C" void __cdecl frame_AurSnapshot_004211a0(void*, unsigned);

// 005EE700 CClientExoAppInternal::HandleMessage(id, message, length, ignore) (thiscall, RET 10h). Callers:
// CClientExoApp::HandleMessage 005ED790 (vtable 0074E6DC; invoked by the net layer for server->player frames) and
// PlayBackInputEvents 006225C4 (-epf playback only). Server->player frames are produced only by CSWSMessage senders
// (server object, absent: P4) or a net session started by CNetLayer::StartConnectToSession 005D5040 (callers
// OnModulePicked 0067BA91, SaveLoad::LoadGame 006CB3A1/006CB5BC, ClassSelection::BeginPlayGame 006DBEDE).
extern "C" unsigned __fastcall client_internal_handle_message_005ee700(void* self, void*, U32 id, unsigned char* data, U32 length, U32 ignore) {
    return recovered_client_message_005ee700(self,nullptr,id,data,length,ignore);
}

// 005FCBD0 CClientExoAppInternal::ShutDownToMainMenu() (thiscall, RET 0). Callers: StallServerCode 00602CDE gated by
// client+264h != 0 (ctor 005FC44D = 0; set only by CSWCMessage::HandleServerToPlayerLoadBar 0064EE8A, reached through
// HandleMessage above) and CClientExoApp::ShutDownToMainMenu 005ED890 (vtable 0074E6EC; net-session teardown, session
// never started on the menu - see HandleMessage).
// Lane closeC: trap definition removed; faithful body in srcw4 src/gui/claude_closeC_gui.cpp
// (client_internal_shut_down_to_main_menu_005fcbd0). Death / quit-to-menu / net teardown reach it in play.

