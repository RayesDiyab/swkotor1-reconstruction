// W7 tier t6: PlayMovie (733), SetReturnStrref (152), ActionStartConversation (204), GiveXPToCreature (393) and
// GetHasSkill (286), reconstructed from the original swkotor.exe (read-only, capstone 5.0.7).
// Native x86 translation unit. Live 1123 providers: CExoInput::ClearEvents 005DF4E0 (0417), CExoSoundInternal::
// SetSoundMode 005D5E80 (0709), CClientExoApp::HideLoadScreen 005EDA90 (0813), CClientExoApp::PlayMovie 005EDB30
// (0809), CExoString ctor/dtor/assign/compare, CExoLinkedList AddHead/GetNext, CGameObjectArray::GetGameObject,
// GetInGameGui / GetSWParty / CSWParty::GetCharacter, CServerAIMaster::SetAILevel (1070), AddPanel / SetVisible /
// SetFade, ShowMouse (0461); W1 tier5b CSWSObject::AddAction.
// Imports with named-trap fallbacks (w7_missing_t6.cpp), per the t6 review rulings (t7): W4 ResetDriveAcceleration
// 005EDBA0, UpdateStatus 0062EEB0, SetGlobalDialogState 0062EC60, CSWCObject::ClearAllActions 0063D470 and
// AddToDialogNonDependantList 00632300 (with its IsOnDialogNonDependantList 0062F840); W5 CSWSObject::ClearAllActions
// 004CCD80; W2 t6 GetCanUseSkill 005AF880 and CSWPartyTable::DistributeExperience 005653A0.
// W7 leaf: CSWPartyTable::SetReturnStrref 00563AB0.
#include "bea6449dcb-vm_actions_t1.hpp"
#include "a0b503eff7-vm_actions_t6.hpp"
#include <cstdint>
#include <cstring>
#if !defined(_M_IX86)
#error W7 ACTION bodies are native x86 (32-bit guest pointers)
#endif
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }
namespace frame_control { extern unsigned message_manager_007a3a00; }
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; extern void* volatile input_007a39e4; }
namespace table29_required { void __fastcall gui_sound_mode_005d5e80(void*, void*, unsigned); }   // 0709 CExoSoundInternal::SetSoundMode
extern "C" {
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long address, const char* reason);
void __fastcall frame_LogStringDtor_005e5c20(void*, void*);
void* __fastcall frame_LogStringCtor_005e5a90(void*, void*, const char*);
extern void* kotor_cell_sound_007a39ec;                                                       // 0673 g_pExoSound
void __fastcall kotor_fastcall_005df4e0(void*, void*);                                        // CExoInput::ClearEvents
void __fastcall kotor_m15_client_hide_load_screen_005eda90(void*, void*, unsigned, unsigned);
void __fastcall kotor_fastcall_005edb30(void*, void*, void*, void*, unsigned, unsigned);      // CClientExoApp::PlayMovie
unsigned __fastcall recovered_vm_outer_pop_object_005d10c0(void*, void*, void*);
unsigned __fastcall recovered_vm_outer_push_integer_005d1010(void*, void*, unsigned);
unsigned char __fastcall kotor_fastcall_004d8230(void*, void*, unsigned, void**);            // CGameObjectArray::GetGameObject
unsigned __fastcall kotor_fastcall_004aea30(void*, void*, unsigned);                        // ClientToServerObjectId
void* __fastcall kotor_fastcall_004ae750(void*, void*, unsigned);                            // CServerExoApp::GetGameObject
void* __fastcall frame_R4_005ed690(void*, void*);                                            // CClientExoApp::GetInGameGui
void* __fastcall kotor_fastcall_005ed8b0(void*, void*);                                      // CClientExoApp::GetSWParty
void* __fastcall frame_GetCharacter_006346c0(void*, void*, unsigned);                       // CSWParty::GetCharacter
unsigned __fastcall recovered_ai_level_004b08a0(void*, void*, void*, unsigned);              // CServerAIMaster::SetAILevel (1070)
int __fastcall recovered_object_add_action_004cea20(void*, void*, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned,
    unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned,
    unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned);              // W1 (tier5b / tier6b)
// other lanes' exports (owner names; named-trap fallbacks in w7_missing_t6.cpp until they are linked)
void __fastcall kotor_fastcall_005edba0(void*, void*);                                        // W4 ResetDriveAcceleration
void __fastcall kotor_fastcall_0062eeb0(void*, void*, std::uint8_t, std::uint32_t);           // W4 t1 CGuiInGame::UpdateStatus
void __fastcall w5_object_clear_all_actions_004ccd80(void*, void*, unsigned);                // W5 CSWSObject::ClearAllActions
void __fastcall kotor_fastcall_0063d470(void*, void*);                                       // W4 CSWCObject::ClearAllActions
void __fastcall kotor_fastcall_0062ec60(void*, void*, unsigned);                             // W4 CGuiInGame::SetGlobalDialogState
void __fastcall kotor_fastcall_00632300(void*, void*, void*);                                // W4 AddToDialogNonDependantList
unsigned __fastcall recovered_stats_can_use_skill_005af880(void*, void*, unsigned);          // W2 t6
void __fastcall recovered_party_distribute_experience_005653a0(void*, void*, unsigned, unsigned); // W2 t6 DistributeExperience
void __fastcall w7_missing_reset_drive_acceleration_005edba0(void*, void*);
void __fastcall w7_missing_update_status_0062eeb0(void*, void*, std::uint8_t, std::uint32_t);
void __fastcall w7_missing_clear_all_actions_004ccd80(void*, void*, unsigned);
void __fastcall w7_missing_client_clear_all_actions_0063d470(void*, void*);
void __fastcall w7_missing_set_global_dialog_state_0062ec60(void*, void*, unsigned);
void __fastcall w7_missing_add_to_dialog_list_00632300(void*, void*, void*);
unsigned __fastcall w7_missing_can_use_skill_005af880(void*, void*, unsigned);
void __fastcall w7_missing_distribute_experience_005653a0(void*, void*, unsigned, unsigned);
}
#pragma comment(linker, "/alternatename:@kotor_fastcall_005edba0@8=@w7_missing_reset_drive_acceleration_005edba0@8")
#pragma comment(linker, "/alternatename:@kotor_fastcall_0062eeb0@16=@w7_missing_update_status_0062eeb0@16")
#pragma comment(linker, "/alternatename:@w5_object_clear_all_actions_004ccd80@12=@w7_missing_clear_all_actions_004ccd80@12")
#pragma comment(linker, "/alternatename:@kotor_fastcall_0063d470@8=@w7_missing_client_clear_all_actions_0063d470@8")
#pragma comment(linker, "/alternatename:@kotor_fastcall_0062ec60@12=@w7_missing_set_global_dialog_state_0062ec60@12")
#pragma comment(linker, "/alternatename:@kotor_fastcall_00632300@12=@w7_missing_add_to_dialog_list_00632300@12")
#pragma comment(linker, "/alternatename:@recovered_stats_can_use_skill_005af880@12=@w7_missing_can_use_skill_005af880@12")
#pragma comment(linker, "/alternatename:@recovered_party_distribute_experience_005653a0@16=@w7_missing_distribute_experience_005653a0@16")
namespace {
using U = std::uint32_t;
void* ptr(U a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
void* client() { return ptr(rd(word(kotor_menu_window_cells::manager_007a39fc) + 4)); }
U server() { return rd(word(kotor_menu_window_cells::manager_007a39fc) + 8); }
void* vm() { return ptr(frame_control::message_manager_007a3a00); }
U pop_int(U* v) { return recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, v); }
U pop_obj(U* v) { return recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, v); }
struct Str { char* d = nullptr; U n = 0; };                                                    // CExoString
void str_dtor(Str* s) { frame_LogStringDtor_005e5c20(s, nullptr); }
void* object_array() { return ptr(rd(rd(server() + 4) + 0x1005c)); }                          // GetObjectArray 004AED70
// CGameObjectArray::GetGameObject; 0 unless status 0
U game_object(U id) { void* o = nullptr; return kotor_fastcall_004d8230(object_array(), nullptr, id, &o) == 0 ? word(o) : 0u; }
U vcall0(U obj, U slot) { using F = U(__fastcall*)(void*, void*); return reinterpret_cast<F>(ptr(rd(rd(obj) + slot)))(ptr(obj), nullptr); }
U party_table() { return rd(server() + 4) + 0x1b770; }                                        // CServerExoApp::GetPartyTable 004AEE70
void* in_game_gui() { return frame_R4_005ed690(client(), nullptr); }
}
extern "C" {
// 00540ED0 PlayMovie(string movie): clear input, reset drive acceleration, sound mode [0074C5F8] (3), hide the load
// screen (0, 1), play, then sound mode [0074C5EC] (0). Always returns 0 after a successful pop.
int __fastcall recovered_vm_play_movie_00540ed0(void*, void*, int, unsigned) {
  Str name;
  (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&name), nullptr);
  if (!recovered_vm_outer_pop_string_005d1080(vm(), nullptr, &name)) { str_dtor(&name); return -2001; }
  kotor_fastcall_005df4e0(kotor_menu_window_cells::input_007a39e4, nullptr);
  kotor_fastcall_005edba0(client(), nullptr);
  table29_required::gui_sound_mode_005d5e80(kotor_cell_sound_007a39ec, nullptr, 3);
  kotor_m15_client_hide_load_screen_005eda90(client(), nullptr, 0, 1);
  // 00540F7D: the second argument is the address of the (never written) first frame dword; zero-initialised here
  U frame0 = 0;
  kotor_fastcall_005edb30(client(), nullptr, &name, &frame0, 0, 0);
  table29_required::gui_sound_mode_005d5e80(kotor_cell_sound_007a39ec, nullptr, 0);
  str_dtor(&name);
  return 0;
}

// 00563AB0 CSWPartyTable::SetReturnStrref(a, b, c): +100 / +104 / +108 (W7 leaf, t6)
void __fastcall recovered_party_set_return_strref_00563ab0(void* table, void*, U a, U b, U c) {
  wr(word(table) + 0x100, a); wr(word(table) + 0x104, b); wr(word(table) + 0x108, c);
}

// 005434C0 SetReturnStrref(int bShow, int srStringRef, int srReturnQueryStrRef = 0)
int __fastcall recovered_vm_set_return_strref_005434c0(void*, void*, int, unsigned argc) {
  U a = 0, b = 0, c = 0;
  if (!pop_int(&a) || !pop_int(&b)) return -2001;
  if (static_cast<int>(argc) > 2 && !pop_int(&c)) return -2001;                                // 005434F3: signed argc > 2
  recovered_party_set_return_strref_00563ab0(ptr(party_table()), nullptr, a, b, c);
  return 0;
}

// 0053E750 GiveXPToCreature(object oCreature, int nXpAmount): xp > 0 and a creature -> (player-controlled +A88:
// CGuiInGame::UpdateStatus(2, xp)), then CSWPartyTable::DistributeExperience(xp, 1) on the party table
int __fastcall recovered_vm_give_xp_to_creature_0053e750(void*, void*, int, unsigned) {
  U id = 0, xp = 0;
  if (!pop_obj(&id) || !pop_int(&xp)) return -2001;
  if (static_cast<std::int32_t>(xp) <= 0) return 0;
  const U o = game_object(id);
  if (!o) return 0;
  const U c = vcall0(o, 0x30);                                                                 // AsSWSCreature
  if (!c) return 0;
  if (rd(c + 0xa88)) kotor_fastcall_0062eeb0(in_game_gui(), nullptr, 2, xp);
  if (party_table()) recovered_party_distribute_experience_005653a0(ptr(party_table()), nullptr, xp, 1);
  return 0;
}

// 00539380 GetHasSkill(int nSkill, object oCreature): 1 when CSWSCreatureStats::GetCanUseSkill(+A74, nSkill) == 1
int __fastcall recovered_vm_get_has_skill_00539380(void*, void*, int, unsigned) {
  U skill = 0, id = 0, result = 0;
  if (!pop_int(&skill) || !pop_obj(&id)) return -2001;
  void* o = nullptr;
  if (kotor_fastcall_004d8230(object_array(), nullptr, id, &o) == 0) {
    // 005393E5..005393E7: the original unconditionally dereferences [o] and
    // calls AsSWSCreature through its vtable. A status-0/null object is a real
    // original null-deref fault, not a substitute trap; preserve it (vcall0
    // reads [o] and faults before any return).
    if (vcall0(word(o), 0x30)) {
      const U c = vcall0(word(o), 0x30);
      if (recovered_stats_can_use_skill_005af880(ptr(rd(c + 0xa74)), nullptr, skill) == 1) result = 1;
    }
  }
  return recovered_vm_outer_push_integer_005d1010(vm(), nullptr, result) ? 0 : -2000;
}

// 0052D5B0 ActionStartConversation(object oObjectToConverse, string sDialogResRef = "", int bPrivateConversation = 0,
// int nConversationType, int bIgnoreStartRange, string sNameObjectToIgnore1..6, int bUseLeader = 0)
// Quirks kept: every pop failure returns -2000; nConversationType is popped and unused; bIgnoreStartRange is popped
// and then forced to 1 (0052D69E); the six names are popped only when argc > 5.
int __fastcall recovered_vm_action_start_conversation_0052d5b0(void* self, void*, int, unsigned argc) {
  const int n = static_cast<int>(argc);
  Str dialog; (void)frame_LogStringCtor_005e5a90(&dialog, nullptr, "");
  U target = 0, priv = 0, type = 1, ignore = 0, one = 1, use_leader = 0;
  if (!pop_obj(&target) || (n >= 2 && !recovered_vm_outer_pop_string_005d1080(vm(), nullptr, &dialog)) ||
      (n >= 3 && !pop_int(&priv)) || (n >= 4 && !pop_int(&type)) || (n >= 5 && !pop_int(&ignore))) {
    str_dtor(&dialog); return -2000;
  }
  (void)type;
  ignore = 1;
  Str temp; (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&temp), nullptr);
  bool ok = true;
  if (n > 5)
    for (int i = 0; ok && i < 6; ++i) {
      ok = recovered_vm_outer_pop_string_005d1080(vm(), nullptr, &temp) != 0;
      if (ok) kotor_fastcall_00632300(in_game_gui(), nullptr, &temp);
    }
  if (ok && n >= 12) ok = pop_int(&use_leader) != 0;
  if (!ok) { str_dtor(&temp); str_dtor(&dialog); return -2000; }
  wr(word(in_game_gui()) + 0xbd8, use_leader ? 1u : 0u);
  if (void* pc = frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client(), nullptr), nullptr, 0)) {
    kotor_fastcall_0063d470(pc, nullptr);
    void* so = kotor_fastcall_004ae750(ptr(server()), nullptr, kotor_fastcall_004aea30(ptr(server()), nullptr, rd(word(pc) + 4)));
    if (so && vcall0(word(so), 0x30)) w5_object_clear_all_actions_004ccd80(ptr(vcall0(word(so), 0x30)), nullptr, 1);
  }
  if (rd(word(in_game_gui()) + 0xb4) == 0 && rd(word(self) + 4) == 1) {
    void* found = nullptr;
    const bool status_ok = kotor_fastcall_004d8230(object_array(), nullptr, rd(word(self) + 8), &found) == 0;
    // closeC: 0052D819..0052D823 read [found+E8] unconditionally once the status is 0; a null found object faults on
    // that read exactly as the original does (former explicit trap removed).
    const U s = word(found);
    if (status_ok && *static_cast<const volatile U*>(ptr(s + 0xe8)) != 0) {
      if (rd(s + 0x78) == 0) (void)recovered_ai_level_004b08a0(ptr(rd(rd(server() + 4) + 0x10044)), nullptr, ptr(s), 1);
      kotor_fastcall_0062ec60(in_game_gui(), nullptr, 1);
      U invalid = 0x7f000000u;
      (void)recovered_object_add_action_004cea20(ptr(s), nullptr, 0x18, 0xffff, 3, word(&target), 4, word(&dialog), 1, word(&priv),
          1, word(&one), 1, word(&ignore), 3, word(&invalid), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }
  str_dtor(&temp); str_dtor(&dialog);
  return 0;
}
}
