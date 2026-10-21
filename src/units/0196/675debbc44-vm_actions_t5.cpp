// W7 tier t5: ActionMoveToObject / ActionForceMoveToObject (22/383), SetPartyLeader (13), SetGlobalFadeIn/Out
// (719/720), and the unowned CSWSCreature::AddMoveToPointAction 004F8B60, reconstructed from the original swkotor.exe
// (read-only, capstone 5.0.7). Native x86 translation unit. MEMORY: only live_operator_new / live_free.
// Imports owned by other lanes (each with a named-trap fallback in w7_missing_t5.cpp, selected by /alternatename only
// when the owner's export is absent from the link):
//   W1 tier5b   @recovered_object_add_action_004cea20@120             CSWSObject::AddAction
//   W5 t3       @w5_creature_get_use_range_004ee440@24                 CSWSCreature::GetUseRange
//   W5 t3       @w5_creature_set_lock_orientation_to_object_004f34a0@16 (declared in vm_actions_t4.cpp)
//   W2 t4       @recovered_npc_object_00564700@20                      CSWPartyTable::GetNPCObject (hard import since t6)
//   W4 t1       @kotor_fastcall_00635480@12                            CSWParty::SetLeader
// Live 1123 providers: CClientExoApp::GetCreatureByGameObjectID 005ED5B0 (0457), CSWCCreature::GetServerCreature
// 0060FB20 (0816), ServerToClientObjectId 005EDA50 (0453), GetPartyTable 004AEE70 (0575), CGameEffect ctor 00503E40
// and CSWSObject::ApplyEffect 004D14F0 (0886), GetDeadTemp 004EF890 (0963), GetInGameGui 005ED690 (0457),
// CGuiInGame::StartGlobalFade 0062ABF0 (0814), CSWSObject::GetArea 004CB120 (1060), CResRef::CResRef 00405ED0.
#include "73dbba025f-vm_actions_t1.hpp"
#include "6cdf8f62ff-vm_actions_t2.hpp"
#include "d09f202cd1-vm_actions_t4.hpp"
#include "9be0bd2506-vm_actions_t5.hpp"
#include <cstring>
#if !defined(_M_IX86)
#error W7 ACTION bodies are native x86 (32-bit guest pointers)
#endif
namespace frame_control { extern unsigned message_manager_007a3a00; }
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void*, void*); }                     // 0575
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long address, const char* reason);
unsigned __fastcall recovered_vm_outer_push_integer_005d1010(void*, void*, unsigned);
unsigned __fastcall recovered_vm_outer_pop_object_005d10c0(void*, void*, void*);
unsigned char __fastcall kotor_fastcall_004d8230(void*, void*, unsigned, void**);
void* __fastcall recovered_object_area_004cb120(void*, void*);
void* __fastcall c2da_default_ref_00405ed0(void*, void*);                                // CResRef::CResRef()
unsigned __fastcall frame_R4_005ed5b0(void*, void*, unsigned);                          // CClientExoApp::GetCreatureByGameObjectID
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void*, void*);        // CSWCCreature::GetServerCreature
unsigned __fastcall kotor_fastcall_005eda50(void*, void*, unsigned);                    // ServerToClientObjectId
void* __fastcall recovered_effect_construct_00503e40(void*, void*, unsigned);
void __fastcall recovered_effect_apply_004d14f0(void*, void*, void*, unsigned, unsigned);
int __fastcall recovered_creature_incapacitated_004ef890(void*, void*);                 // GetDeadTemp
void* __fastcall frame_R4_005ed690(void*, void*);                                       // CClientExoApp::GetInGameGui
void __fastcall kotor_m15_ingame_start_global_fade_0062abf0(void*, void*, unsigned, unsigned, unsigned, const void*);
// imports owned by other lanes
int __fastcall recovered_object_add_action_004cea20(void*, void*, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned,
    unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned,
    unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned);
unsigned __fastcall w5_creature_get_use_range_004ee440(void*, void*, unsigned, void*, float*, unsigned);
unsigned __fastcall w7_missing_get_use_range_004ee440(void*, void*, unsigned, void*, float*, unsigned);
void __fastcall w5_creature_set_lock_orientation_to_object_004f34a0(void*, void*, unsigned, unsigned);
unsigned __fastcall recovered_npc_object_00564700(void*, void*, unsigned, unsigned, unsigned);
void __fastcall kotor_fastcall_00635480(void*, void*, int);
void __fastcall w7_missing_party_set_leader_00635480(void*, void*, int);
}
#pragma comment(linker, "/alternatename:@w5_creature_get_use_range_004ee440@24=@w7_missing_get_use_range_004ee440@24")
#pragma comment(linker, "/alternatename:@kotor_fastcall_00635480@12=@w7_missing_party_set_leader_00635480@12")

namespace {
using U = std::uint32_t;
using I = std::int32_t;
constexpr U kInvalidObject = 0x7f000000u;
constexpr I kPopFailed = -2001, kPushFailed = -2000;
void* ptr(U a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
std::uint16_t rd16(U a) { std::uint16_t v; std::memcpy(&v, ptr(a), 2); return v; }
void wr16(U a, U v) { const std::uint16_t x = static_cast<std::uint16_t>(v); std::memcpy(ptr(a), &x, 2); }
U fbits(float f) { U u; std::memcpy(&u, &f, 4); return u; }
float fval(U u) { float f; std::memcpy(&f, &u, 4); return f; }
void* vm() { return ptr(frame_control::message_manager_007a3a00); }
U app() { return word(kotor_menu_window_cells::manager_007a39fc); }
U server() { return rd(app() + 8); }
U object_array_004aed70(U s) { return rd(rd(s + 4) + 0x1005c); }
U vcall(U object, U slot) { using F = U(__thiscall*)(void*); return reinterpret_cast<F>(rd(rd(object) + slot))(ptr(object)); }
bool lookup(U id, U& object) { void* out = nullptr; const bool ok = kotor_fastcall_004d8230(ptr(object_array_004aed70(server())), nullptr, id, &out) == 0; object = word(out); return ok; }
U party_005ed8b0(U client) { return rd(rd(client + 4) + 0x270); }   // CClientExoApp::GetSWParty (mov eax,[ecx+4]; mov eax,[eax+270])
int add_action(U object, U type, U group, const U (&pairs)[26]) {
  return recovered_object_add_action_004cea20(ptr(object), nullptr, type, group, pairs[0], pairs[1], pairs[2], pairs[3], pairs[4], pairs[5], pairs[6],
      pairs[7], pairs[8], pairs[9], pairs[10], pairs[11], pairs[12], pairs[13], pairs[14], pairs[15], pairs[16], pairs[17], pairs[18], pairs[19],
      pairs[20], pairs[21], pairs[22], pairs[23], pairs[24], pairs[25]);
}
}

extern "C" {
// 004F8B60 CSWSCreature::AddMoveToPointAction (17 stack words, RET 44h). Same packing as its twin
// AddMoveToPointActionToFront 004F8A50 (W5 t3), but appended with CSWSObject::AddAction (action 1).
int __fastcall recovered_creature_add_move_to_point_action_004f8b60(void* creature, void*, U group, const float* point, U area_id, U target_id,
    U run, U range, U a6, U a7, U a8, U a9, U a10, U a11, U a12, const float* a13, U a14, U a15, U a16) {
  const U c = word(creature);
  U flags = (a7 + a7) | run;                                                  // 004F8B71..77
  if (!(fval(a6) == 0.0f)) flags |= 4;                                        // FCOMP 0.0 / test ah,44h / jnp (NaN: set)
  flags |= ((((a9 & 7u) << 1) | a11) << 3);                                   // 004F8B83..95
  if (a10) flags |= (a10 & 3u) << 7;                                          // 004F8B97..A5
  flags |= ((((a16 & 1u) << 1) | (a12 & 1u)) << 9);                           // 004F8BA7..C2
  wr(c + 0xa8c, 2);
  const U pathfinder = rd(c + 0x340);
  wr(pathfinder + 0x26c, 0); wr(pathfinder + 0x270, 0); wr(pathfinder + 0x274, 0);
  const U pairs[26] = {2, word(&point[0]), 2, word(&point[1]), 2, word(&point[2]), 3, word(&area_id), 3, word(&target_id),
                       1, word(&flags), 2, word(&range), 1, word(&a8), 2, word(&a6), 2, word(&a13[0]), 2, word(&a13[1]),
                       1, word(&a14), 1, word(&a15)};
  (void)add_action(c, 1, group, pairs);
  w5_creature_set_lock_orientation_to_object_004f34a0(creature, nullptr, kInvalidObject, 0);
  return 1;
}

// 0053FB00 ActionMoveToObject (22) / ActionForceMoveToObject (383)
// (object target, int run = FALSE, float range = 1.0, [383: float timeout = 30.0])
int __fastcall recovered_vm_action_move_to_object_0053fb00(void* self, void*, int routine, unsigned argc) {
  U target_id = 0, run = 0, range = fbits(1.0f), timeout = 0, use_range = fbits(0.5f);
  if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &target_id)) return kPopFailed;
  if (static_cast<I>(argc) >= 2 && !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &run)) return kPopFailed;
  if (static_cast<I>(argc) >= 3 && !recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &range)) return kPopFailed;
  if (routine == 0x17f) {
    if (static_cast<I>(argc) >= 4) { if (!recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &timeout)) return kPopFailed; }
    else timeout = fbits(30.0f);
  }
  const U s = word(self);
  U owner = 0, target = 0;
  if (rd(s + 4) != 1 || !lookup(rd(s + 8), owner) || !lookup(target_id, target)) return 0;
  if (!owner) kotor_unreachable_trap(0x0053fc18, "ActionMoveToObject: owner lookup succeeded with a null object");
  if (!vcall(owner, 0x30) || !rd(vcall(owner, 0x30) + 0xe8)) return 0;
  float use_point[3] = {0.0f, 0.0f, 0.0f};
  float out_range = fval(use_range);
  (void)w5_creature_get_use_range_004ee440(ptr(vcall(owner, 0x30)), nullptr, target_id, use_point, &out_range, 0);
  use_range = fbits(out_range);
  if (!(fval(range) > fval(use_range))) range = use_range;                    // FCOMP; test ah,41h: <=, == or NaN -> use range
  if (!target) kotor_unreachable_trap(0x0053fc90, "ActionMoveToObject: target lookup succeeded with a null object");
  float position[3]; std::memcpy(position, ptr(target + 0x90), 12);
  const U area = word(recovered_object_area_004cb120(ptr(target), nullptr));
  alignas(4) unsigned char resref[16]; (void)c2da_default_ref_00405ed0(resref, nullptr);   // constructed, unused (original)
  if (!area) return 0;
  const float zero[3] = {0.0f, 0.0f, 0.0f};
  U one = 1;   // 0053FCFD: the routine argument slot is overwritten with 1 and passed by address below
  (void)recovered_creature_add_move_to_point_action_004f8b60(ptr(vcall(owner, 0x30)), nullptr, 0xffff, position, rd(area + 0x120), target_id, run,
      range, timeout, 0, 0, 0, 0, 0, 0, zero, 0, 0, 0);
  const U p17[26] = {3, word(&target_id), 1, word(&run), 2, word(&range), 2, word(&range), 1, word(&one)};
  (void)add_action(owner, 0x11, 0xfffe, p17);
  if (vcall(owner, 0x48) && rd(vcall(owner, 0x48) + 0x33c)) {
    const U p19[26] = {3, word(&target_id)};
    (void)add_action(owner, 0x13, 0xfffe, p19);
  }
  return 0;
}

// 00545A70 SetPartyLeader(int npc). NPC_PLAYER (-1): every party member that is player-controlled (+A88), in the
// party (+9D4) and dead (vtbl+94 or GetDeadTemp) is resurrected (CGameEffect type [0074668C] applied, +F0 = 1); the
// leader becomes index -3. Otherwise the NPC's party slot (GetNPCObject -> client id) becomes leader. Pushes 1 when
// SetLeader was called, else 0.
int __fastcall recovered_vm_set_party_leader_00545a70(void*, void*, int, unsigned) {
  const U client = rd(app() + 4), srv = rd(app() + 8);
  const U party = party_005ed8b0(client);
  U npc = 0, result = 0;
  if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &npc)) return kPopFailed;
  int leader = -1; bool set = false;
  if (static_cast<I>(npc) == -1) {
    for (I i = 0; i < static_cast<I>(rd(party)); ++i) {
      const U cc = frame_R4_005ed5b0(ptr(rd(app() + 4)), nullptr, rd(party + 0x24 + static_cast<U>(i) * 0x88));
      if (!cc || !kotor_m15a_creature_get_server_creature_0060fb20(ptr(cc), nullptr)) continue;
      const U c = word(kotor_m15a_creature_get_server_creature_0060fb20(ptr(cc), nullptr));
      if (rd(c + 0xa88) != 1 || rd(c + 0x9d4) != 1) continue;
      if (vcall(c, 0x94) != 1 && recovered_creature_incapacitated_004ef890(ptr(c), nullptr) != 1) continue;
      void* block = live_operator_new(0x8c);
      if (!block) kotor_unreachable_trap(0x00545b86, "SetPartyLeader: CGameEffect allocation failed (original writes through null)");
      const U e = word(recovered_effect_construct_00503e40(block, nullptr, 1));
      wr16(e + 8, 4);                                                           // [0074668C] effect type
      wr16(e + 0xa, (rd16(e + 0xa) & 0xfff8u) | 0u);                            // | [007473B8] (0)
      recovered_effect_apply_004d14f0(ptr(c), nullptr, ptr(e), 0, 0);
      wr(c + 0xf0, 1);
    }
    leader = -3; set = true;
  } else {
    const U object = recovered_npc_object_00564700(lane_config_required::GetPartyTable_004aee70(ptr(srv), nullptr), nullptr, npc, 0, 1);
    const U id = kotor_fastcall_005eda50(ptr(client), nullptr, object);   // 00545BE5 mov ecx,esi (CClientExoApp)
    for (I i = 0; i < static_cast<I>(rd(party)); ++i) if (rd(party + 0x24 + static_cast<U>(i) * 0x88) == id) { leader = i; set = true; break; }
  }
  if (set) { kotor_fastcall_00635480(ptr(party), nullptr, leader); result = 1; }
  return recovered_vm_outer_push_integer_005d1010(vm(), nullptr, result) ? 0 : kPushFailed;
}

namespace {
// 00546010 / 005460D0: pops fWait, fLength, then r, g, b; CGuiInGame::StartGlobalFade(in, wait, length, &rgb)
int global_fade(U mode) {
  U wait = 0, length = 0, rgb[3] = {0, 0, 0};
  if (!recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &wait) || !recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &length) ||
      !recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &rgb[0]) || !recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &rgb[1]) ||
      !recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &rgb[2]))
    return kPopFailed;
  const U color[3] = {rgb[0], rgb[1], rgb[2]};
  kotor_m15_ingame_start_global_fade_0062abf0(frame_R4_005ed690(ptr(rd(app() + 4)), nullptr), nullptr, mode, wait, length, color);
  return 0;
}
}
int __fastcall recovered_vm_set_global_fade_in_00546010(void*, void*, int, unsigned) { return global_fade(1); }
int __fastcall recovered_vm_set_global_fade_out_005460d0(void*, void*, int, unsigned) { return global_fade(0); }
}
