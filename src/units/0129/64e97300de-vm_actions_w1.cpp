#include "d76b9ee790-vm_actions_w1.hpp"
#include "c3d8049173-action_queue.hpp"
#include "b15126a110-object_actions.hpp"
#include "c34e3002b9-nearest.hpp"
#include "c574626fdd-w1_x87.hpp"
#include <cstring>
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// W1 tier 6: NWScript engine-command (ACTION) bodies claimed in lane-w7/OWNERSHIP.md, plus small server leaves they reach.
// ACTION ABI: __thiscall(CSWVirtualMachineCommands* this, int routine, int argc), RET 8; this+4 = owner valid flag,
// this+8 = script owner object id. Return 0, -2001 (pop failure) or -2000 (push failure).
namespace w1::vm_actions {
namespace {
constexpr U kPopFailed = 0xfffff82fu, kPushFailed = 0xfffff830u;
constexpr U kScriptOk = 0;                 // [0074666C]
constexpr U kSignalEvent = 10, kDestroyObject = 11;   // [007441DC] [007441E0]
U vm(Ports& p) { return p.global(kVirtualMachine); }
U client(Ports& p) { return p.read(p.global(kAppManager) + 4); }
bool pop_object(U slot, Ports& p) { return p.call(0x5d10c0, vm(p), {slot}) != 0; }
bool pop_int(U slot, Ports& p) { return p.call(0x5d1000, vm(p), {slot}) != 0; }
bool pop_float(U slot, Ports& p) { return p.call(0x5d1020, vm(p), {slot}) != 0; }
U push_object(U id, Ports& p) { return p.call(0x5d10d0, vm(p), {id}) ? 0u : kPushFailed; }   // neg/sbb/and 7D0/add -2000
// fld dword v; fcomp dword 0.0f; test ah,5; jp -> falls through only for C0 alone (v < 0, ordered): a single compare of a
// loaded single, so the C++ comparison is exact (-0.0 and NaN are not below).
bool below_zero(U v) { float f; std::memcpy(&f, &v, 4); return f < 0.0f; }
U object_array(Ports& p) { return p.read(server_internal(p) + 0x1005c); }   // GetObjectArray 004AED70
bool lookup(U id, U slot, Ports& p) { return (p.call(0x4d8230, object_array(p), {id, slot}) & 0xff) == kScriptOk; }
}
// ---- leaves
void set_delete_no_fade_004ce920(U o, U value, U scratch, Ports& p) {
  U srv = server(p);
  U player = p.call(0x4ae910, srv, {p.call(0x4aea40, srv, {})});
  if (player) {                                                     // 004CE942..004CE979 (as 004CE8A0, entry +0x148)
    U head = p.read(p.read(p.read(player + 0xc)));
    p.write(scratch, head);
    for (U pos = head; pos; pos = p.read(scratch)) {
      U entry = p.call(0x5e9b60, p.read(p.read(player + 0xc)), {pos});
      if (p.read(entry + 0x2c) == p.read(o + 4)) { p.write(entry + 0x148, value); return; }
      p.call(0x5e9b90, p.read(p.read(player + 0xc)), {scratch});
    }
  }
  p.write(o + 0x204, value);                                        // 004CE97B
}
U party_table_npc_id_005638a0(U table, U id, Ports& p) {
  if (id == kInvalidObject) return 0xffffffffu;                     // 005638A4..005638AF
  for (U i = 0; slt(i, 9); ++i) if (p.read(table + 0xc + i * 4) == id) return i;
  return 0xffffffffu;
}
void party_table_clear_member_005639e0(U table, U index, Ports& p) {
  if (sge(index, 0) && slt(index, 9)) p.write(table + 0xc + index * 4, kInvalidObject);   // 005639E4..005639ED
}
void creature_cancel_combat_004fdaa0(U c, U flag, U scratch, Ports& p) {
  p.call(0x4ccd80, c, {1});                                         // ClearAllActions
  p.call(0x4f2610, c, {0, 1});                                      // SetCombatState
  p.call(0x4cc0c0, c, {kInvalidObject, 1});                         // SetLastHostileActor
  action_queue::clear_hostile_004f61d0(c, 0, scratch, p);                    // ClearHostileActionsVersus (W1)
  p.call(0x4d4620, p.read(c + 0x9c8), {flag});                      // CSWSCombatRound::EndCombatRound
  p.write(p.read(c + 0x9c8) + 0x9cc, kInvalidObject);
  if (p.call(0x4cc2b0, c, {})) {                                    // 004FDAEE..004FDB14
    U cc = p.vcall(p.call(0x4cc2b0, c, {}), 0x2c, {});              // AsSWCCreature
    if (cc) p.call(0x610a10, cc, {0});                              // CSWCCreature::SetCombatMode (tail call)
  }
}
// ---- ACTIONs
U get_last_perceived_0053b230(U s, U e, Ports& p) {
  p.write(e, s);                                                    // the local starts as ECX (push ecx)
  p.call(0x4d8230, object_array(p), {p.read(s + 8), e});            // status ignored
  U result = kInvalidObject;
  if (p.vcall(p.read(e), 0x30, {})) result = p.read(p.vcall(p.read(e), 0x30, {}) + 0x900);   // AsSWSCreature: last perceived
  return push_object(result, p);
}
U get_party_member_by_index_0053c870(U, U e, Ports& p) {
  if (!pop_int(e, p)) return kPopFailed;
  U result = kInvalidObject;
  U ch = p.call(0x6346c0, p.call(0x5ed8b0, client(p), {}), {p.read(e)});   // CSWParty::GetCharacter
  if (ch) result = p.call(0x4aea30, server(p), {p.read(ch + 4)});   // ClientToServerObjectId
  return push_object(result, p);
}
U signal_event_005439d0(U s, U e, Ports& p) {
  // e+4 target (popped object), e+8 event (engine structure type 1), e+0x14 / e+0x18 lookup scratch
  if (!pop_object(e + 4, p)) return kPopFailed;
  if (!p.call(0x5d10a0, vm(p), {1, e + 8})) return kPopFailed;      // StackPopEngineStructure(EVENT)
  U ev = p.read(e + 8);
  U caller = kInvalidObject;
  if (lookup(p.read(s + 8), e + 0x14, p)) caller = p.read(s + 8);   // 00543A29..00543A3D
  if (lookup(p.read(e + 4), e + 0x18, p))                           // 00543A40..00543A88 (else the event is not queued)
    p.call(0x4b08d0, ai_master(p), {0, 0, caller, p.read(e + 4), kSignalEvent, ev});   // AddEventDeltaTime
  return 0;
}
U event_user_defined_00535a90(U, U e, Ports& p) {
  U mem = p.call(0x6fa7e6, 0, {0x34});
  U ev = mem ? p.call(0x4d7540, mem, {}) : 0;                        // CScriptEvent
  if (!pop_int(e, p)) return kPopFailed;                            // (the event is not freed: original)
  p.write16(ev, 0xb);
  p.call(0x4d76b0, ev, {0, p.read(e)});                             // SetInteger(0, number)
  if (!p.call(0x5d10b0, vm(p), {1, ev})) return kPushFailed;        // StackPushEngineStructure copies it
  p.call(0x4d7590, ev, {});
  p.call(0x6fa390, 0, {ev});
  return 0;
}
namespace {
U door_action(U s, U e, bool open, Ports& p) {
  if (!pop_object(e + 8, p)) return kPopFailed;
  if (p.read(s + 4) != 1 || !lookup(p.read(s + 8), e + 4, p)) return 0;
  U o = p.read(e + 4);
  if (!p.call(0x4cb120, o, {})) return 0;                           // GetArea
  if (open) {
    if (p.read(p.vcall(o, 0x10, {}) + 0xe8) != 1) return 0;         // commandable (open only)
    p.call(0x4cfac0, p.vcall(o, 0x10, {}), {p.read(e + 8), 0});     // AddOpenDoorAction (W5)
  } else {
    p.call(0x4cfb20, p.vcall(o, 0x10, {}), {p.read(e + 8), 0});     // AddCloseDoorAction (W5)
  }
  return 0;
}
}
U action_open_door_00540260(U s, U e, Ports& p) { return door_action(s, e, true, p); }
U action_close_door_0052f4f0(U s, U e, Ports& p) { return door_action(s, e, false, p); }
U cancel_combat_00546510(U, U argc, U e, Ports& p) {
  // 0054651E/00546526: [esp] (object) = OBJECT_INVALID, [esp+4] (flag) = 0.  The flag
  // default must be 0: a one-argument CancelCombat(oCreature) passes 0 to 004FDAA0 and
  // EndCombatRound(0) then skips the VM/CreateNewAttackActions branches.
  p.write(e, kInvalidObject); p.write(e + 4, 0);
  if (!pop_object(e, p)) return kPopFailed;                         // the object
  if (sge(argc, 2) && !pop_int(e + 4, p)) return kPopFailed;        // optional flag (default 0)
  U cr = p.call(0x4ae770, server(p), {p.read(e)});
  if (cr) creature_cancel_combat_004fdaa0(cr, p.read(e + 4), e + 0xc, p);
  return 0;
}
U destroy_object_0052ff20(U s, U argc, U e, Ports& p) {
  // e == [esp] after `sub esp,14h`: +0 object, +0xC delay (2nd argument, float), +8 no-fade (3rd, int),
  // +4 fade delay (4th, float), +0x10 this; +0x14 list-position scratch for the fade leaves.
  p.write(e + 0x10, s); p.write(e + 0xc, 0); p.write(e + 8, 0); p.write(e + 4, 0);
  if (!pop_object(e, p)) return kPopFailed;
  if (sge(argc, 2) && !pop_float(e + 0xc, p)) return kPopFailed;
  if (sge(argc, 3) && !pop_int(e + 8, p)) return kPopFailed;
  if (sge(argc, 4) && !pop_float(e + 4, p)) return kPopFailed;
  U target = p.read(e);
  U pc = p.call(0x635460, p.call(0x5ed8b0, client(p), {}), {});      // CSWParty::GetPlayerCharacter
  if (pc && p.call(0x4aea30, server(p), {p.read(pc + 4)}) == target) {   // 0052FFD3..005300AB: destroying the controlled PC
    U old_id = p.read(pc + 4);
    if (sgt(p.read(p.call(0x5ed8b0, client(p), {})), 0)) {
      for (U i = 0;;) {
        p.call(0x5edc30, client(p), {0xfffffffeu, 1});              // CClientExoApp::ChangeCharacter(-2, 1)
        U next = p.call(0x635460, p.call(0x5ed8b0, client(p), {}), {});
        if (next) {
          U srv = server(p);
          if (p.call(0x4ae910, srv, {p.call(0x4aea30, srv, {p.read(next + 4)})})) {
            p.call(0x5ed780, client(p), {p.read(next + 4)});        // SetPlayerCreature
            p.call(0x5ed720, client(p), {});                        // AttachPlayerGameObjectToArea
            break;
          }
        }
        i = (i + 1) & 0xff;
        if (!slt(i, p.read(p.call(0x5ed8b0, client(p), {})))) break;
      }
    }
    p.call(0x6349f0, p.call(0x5ed8b0, client(p), {}), {old_id});    // CSWParty::RemoveCharacter
  }
  U srv = server(p);
  U obj = p.call(0x4ae750, srv, {target});
  if (obj && p.vcall(obj, 0x10, {})) {                              // 005300C5..00530104
    U fade_ms = x87::ftol_mul_float(x87::x80_from_float(p.read(e + 4)), 0x447a0000u);   // __ftol2(fade * 1000.0f [0073D6FC])
    set_delete_no_fade_004ce920(p.vcall(obj, 0x10, {}), p.read(e + 8), e + 0x14, p);
    object_actions::set_fade_out_time_004ce8a0(p.vcall(obj, 0x10, {}), fade_ms, e + 0x14, p);
  }
  U table = server_internal(p) + 0x1b770;                           // GetPartyTable (address)
  party_table_clear_member_005639e0(table, party_table_npc_id_005638a0(table, target, p), p);
  U delay_ms = x87::ftol_mul_float(x87::x80_from_float(p.read(e + 0xc)), 0x447a0000u);   // delay * 1000
  p.call(0x4b08d0, ai_master(p), {0, delay_ms, p.read(s + 8), target, kDestroyObject, 1});   // AddEventDeltaTime
  return 0;
}

namespace {
// 00540703..005409BB: script ANIMATION_* (0..0x78) -> creature animation id for a creature owner, from the byte index at
// 00540D10 and the jump table at 00540C44 (0 = default 0x2710 via 00540AE1; 1 = the MODELTYPE test at 00540919).
const U kCreatureAnimation[0x79] = {0, 0x2744, 0x272e, 0x2730, 0x2731, 0x2736, 0x2737, 0x2738, 0x2739, 0x273a, 0x274b, 0x274c, 0x2749, 0x274a, 0x2788, 0x2789, 0x278a, 0x278b, 0x278c, 0x2786, 0x2786, 0x2727, 0x2799, 0x279b, 0x27a7, 0x27a8, 0x2716, 0x27aa, 0x27ab, 0x27b0, 0x27ac, 0x27b3, 0x27b4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x2745, 0x2746, 0x2747, 0x2748, 0x2732, 0x2733, 0x272d, 0x272c, 0x273c, 0x273c, 0x273c, 0, 1, 0x278d, 0x278e, 0x278f, 0x27a6, 0x2791, 0x2792, 0x279e, 0x27af};
// 00540A39..00540A4B: placeable animations 200..213 (jump table 00540D8C).
const U kPlaceableAnimation[14] = {0x2759, 0x275a, 0x275b, 0x275c, 0x277a, 0x277b, 0x277c, 0x277e, 0x277f, 0x2780, 0x2781, 0x2782, 0x2783, 0x2784};
}
U play_animation_00540550(U s, U routine, U e, Ports& p) {
  // e == [esp] after `push ebx` (00540568): +4 animation, +0x14 speed (float), +0xC duration (float), +8 owner object,
  // +0x18 owner-found flag (left as found when not set: an uninitialised local in the original), +0x10 the int 1 parameter,
  // +0x18/+0x20 CExoStrings for the MODELTYPE test.
  if (!pop_int(e + 4, p) || !pop_float(e + 0x14, p) || !pop_float(e + 0xc, p)) return kPopFailed;
  if (p.read(s + 4) == 1 && lookup(p.read(s + 8), e + 8, p)) p.write(e + 0x18, 1);
  U gui = p.call(0x5ed690, client(p), {});                          // 005405FF..005406C9 stop the GUI-held animation on this object
  if (p.read(gui + 0xb4)) {
    U cid = p.call(0x5eda50, client(p), {p.read(s + 8)});
    U a = p.read(p.call(0x5ed690, client(p), {}) + 0x170);
    U b = p.read(p.call(0x5ed690, client(p), {}) + 0x174);
    for (U which : {a, b}) {
      if (cid != which) continue;
      U o = p.call(0x5ed580, client(p), {which});                  // CClientExoApp::GetGameObject
      if (o && p.vcall(o, 0xc, {})) p.vcall(p.vcall(o, 0xc, {}), 0x9c, {0x2710, 0, 0});
    }
  }
  const U fallback = 0x2710;
  U raw = p.read(e + 4);
  if (p.read(e + 0x18) != 1) return 0;                              // 005406D0..005406DF
  U o = p.read(e + 8);
  if (p.vcall(o, 0x30, {})) {                                       // creature
    U v = raw <= 0x78 ? kCreatureAnimation[raw] : 0;
    if (v == 1) {                                                   // 00540919: appearance MODELTYPE "B" or "F" -> 0x2756
      p.call(0x5b3190, e + 0x18, {});
      p.call(0x5e5a90, e + 0x20, {0x747c2c});                       // "MODELTYPE"
      U table = p.read(p.read(p.global(kRules) + 0xb8) + 0xc);
      p.call(0x413270, table, {p.read16(p.vcall(o, 0x30, {}) + 0xa60), e + 0x20, e + 0x18});   // GetCExoStringEntry
      p.call(0x5e5c20, e + 0x20, {});
      bool special = p.call(0x5e58c0, e + 0x18, {0x747c28}) || p.call(0x5e58c0, e + 0x18, {0x73ece0});
      p.write(e + 4, special ? 0x2756 : fallback);
      p.call(0x5e5c20, e + 0x18, {});
    } else {
      p.write(e + 4, v ? v : fallback);
    }
  } else if (p.vcall(o, 0x48, {})) {                                // placeable
    U i = raw - 200;
    p.write(e + 4, i <= 0xd ? kPlaceableAnimation[i] : fallback);
  }
  if (below_zero(p.read(e + 0xc))) {                                // 00540AE5..00540AF4: duration < 0.0f [0073D700], ordered
    if (!slt(raw, 0) && !sgt(raw, 0x20)) {                          // looping ids 0..0x20: play directly
      U obj = p.call(0x4ae750, server(p), {p.read(s + 8)});
      if (obj && p.vcall(obj, 0x10, {})) p.vcall(p.vcall(obj, 0x10, {}), 0x7c, {p.read(e + 4)});
    }
    return 0;
  }
  if (!p.read(p.vcall(o, 0x10, {}) + 0xe8)) return 0;               // 00540B49..00540B5A commandable
  p.write(e + 0x10, 1);
  U params[26] = {1, e + 4, 2, e + 0x14, 2, e + 0xc, 1, e + 0x10};  // animation, speed, duration, 1
  if (routine == 0x12c) {                                           // 300 PlayAnimation: replace the queue head
    p.call(0x4ccd80, p.vcall(o, 0x10, {}), {1});                    // ClearAllActions
    object_actions::add_action_to_front_004ccb00(p.vcall(o, 0x10, {}), 6, 0xffff, params, e + 0x24, p);
  } else {                                                          // 40 ActionPlayAnimation
    object_actions::add_action_004cea20(p.vcall(o, 0x10, {}), 6, 0xffff, params, e + 0x24, p);
  }
  return 0;
}
// ---- tier 7: vector ACTIONs
U set_lock_orientation_00610950(U cc, U target, U force, Ports& p) {
  U srv = server(p);
  if (!srv) return 0;                                               // 0061095B..0061095D
  U cr = p.call(0x4ae770, srv, {p.call(0x4aea30, srv, {p.read(cc + 4)})});   // GetCreatureByGameObjectID(ClientToServer(id))
  if (!cr) return 0;
  if (!force && (p.vcall(cr, 0x94, {}) || p.call(0x4ef890, cr, {}))) return 0;   // dead / dead-temp creatures keep their lock
  p.write(cc + 0x2f4, target);                                      // 006109A3
  return 0;
}
U get_position_0053cae0(U, U e, Ports& p) {
  // e == [esp+4] after `sub esp,14h; push esi`: +0 object id, +4 object, +8..+0x10 the default vector (0,0,0)
  p.write(e + 8, 0); p.write(e + 0xc, 0); p.write(e + 0x10, 0);
  if (!pop_object(e, p)) return kPopFailed;
  U v = e + 8;
  if (lookup(p.read(e), e + 4, p)) {
    U o = p.read(e + 4);
    if (o && p.read8(o + 8) > type::area) v = o + 0x90;             // 0053CB4B..0053CB56 (jbe: unsigned)
  }
  return p.call(0x5d1050, vm(p), {p.read(v), p.read(v + 4), p.read(v + 8)}) ? 0u : kPushFailed;   // StackPushVector (by value)
}
U set_facing_00542830(U s, U routine, U e, Ports& p) {
  // e == esp after the four pushes: +0x10 object, +0x14..+0x1C facing, +0x20..+0x28 point, +0x2C..+0x34 delta,
  // +0x38..+0x40 normalized delta, +0x48 the routine argument slot (reused for the popped angle)
  for (U o = 0x14; o <= 0x28; o += 4) p.write(e + o, 0);
  bool set = false;
  if (routine == 10) {                                              // SetFacing(float degrees)
    if (!pop_float(e + 0x48, p)) return kPopFailed;
    U f[2]; x87::facing_from_angle(p.read(e + 0x48), 0x3c8efa35u, f);   // * [007415F8] = pi/180
    p.write(e + 0x14, f[0]); p.write(e + 0x18, f[1]); p.write(e + 0x1c, 0);
    set = true;
  } else if (routine == 0x8f) {                                     // SetFacingPoint(vector)
    if (!p.call(0x5d1040, vm(p), {e + 0x20})) return kPopFailed;     // StackPopVector
  }
  if (!lookup(p.read(s + 8), e + 0x10, p)) return 0;
  U o = p.read(e + 0x10);
  if (p.read8(o + 8) <= type::area) return 0;
  if (routine == 0x8f) {
    U pt[3] = {p.read(e + 0x20), p.read(e + 0x24), p.read(e + 0x28)}, pos[3] = {p.read(o + 0x90), p.read(o + 0x94), p.read(o + 0x98)}, d[3];
    if (x87::facing_point_delta(pt, pos, d)) {
      p.write(e + 0x2c, d[0]); p.write(e + 0x30, d[1]); p.write(e + 0x34, d[2]);
      U n = p.call(0x4ab180, 0, {e + 0x38, e + 0x2c});               // normalize(out, in)
      p.write(e + 0x14, p.read(n)); p.write(e + 0x18, p.read(n + 4)); p.write(e + 0x1c, p.read(n + 8));
      set = true;
    } else {
      p.write(e + 0x2c, d[0]); p.write(e + 0x30, d[1]); p.write(e + 0x34, d[2]);
    }
  }
  if (p.read(s + 4) != 1 || !set) return 0;                         // 005429CE..005429D6
  U cid = p.call(0x5eda50, client(p), {p.read(o + 4)});             // ServerToClientObjectId
  U co = p.call(0x5ed580, client(p), {cid});                        // CClientExoApp::GetGameObject
  if (co && p.vcall(co, 0x2c, {})) set_lock_orientation_00610950(p.vcall(co, 0x2c, {}), kInvalidObject, 0, p);
  p.vcall(o, 0xc0, {kInvalidObject});                               // 00542A1F..00542A2A: server object vtable +0xC0 (OBJECT_INVALID)
  p.call(0x4cd170, o, {e + 0x14});                                  // CSWSObject::SetOrientation
  return 0;
}
// ---- tier 7: 0054B550 ExecuteCommandGetNearestObject (38 GetNearestCreature, 226 GetNearestCreatureToLocation,
// 227 GetNearestObject, 228 GetNearestObjectToLocation, 229 GetNearestObjectByTag)
U has_spell_effect_applied_004cdac0(U o, U spell, Ports& p) {
  if (spell == 0xffffffffu) return 0;                               // 004CDAC5..004CDAC9
  U n = p.read(o + 0x128), data = p.read(o + 0x124);
  for (U i = 0; slt(i, n); ++i) if (p.read(p.read(data + i * 4) + 0x1c) == spell) return 1;
  return 0;
}
namespace {
// The creature criteria (0054BA19..0054BC8A). Returns esi as the original leaves it at 0054BC8C (1 = accepted).
U creature_criteria(U c, U e, Ports& p) {
  U ok = 0;                                                         // 0054B9DA xor esi,esi
  U type = p.read(e + 0xa8), value = p.read(e + 0x70);
  p.write(e + 0x3c, type);
  for (U j = 0;;) {
    p.write(e + 0x20, j);
    if (j == 1) { type = p.read(e + 0x54); value = p.read(e + 0x5c); p.write(e + 0x3c, type); }
    else if (j == 2) { type = p.read(e + 0x4c); value = p.read(e + 0x58); p.write(e + 0x3c, type); }
    bool next = false;                                              // straight to the counter (0054BB50)
    if (type == 0xffffffffu || value == 0xffffffffu) next = true;
    else if (type > 7) return 0;                                    // ja 0054BC8A
    else switch (type) {
    case 0: {                                                       // racial type; 0054BA85 compares the FIRST value ([esp+70])
      U first = p.read(e + 0x70), any = p.read8(p.global(kRules) + 0xaa);
      if (first == any) { ok = 1; next = true; break; }
      if (first == any + 1) return 0;
      ok = p.read16(p.read(c + 0xa74) + 0xdc) == value ? 1u : 0u;
      break;
    }
    case 1: ok = p.read(p.read(c + 0xa74) + 0x6c) == value ? 1u : 0u; break;
    case 2: {                                                       // class: GetClass(0..2) (AL) against the value
      U stats = p.read(c + 0xa74), num = p.read8(stats + 0x89);
      bool hit = (p.call(0x5a4e90, stats, {0}) & 0xff) == value ||
                 (sgt(num, 1) && (p.call(0x5a4e90, stats, {1}) & 0xff) == value) ||
                 (sgt(num, 2) && (p.call(0x5a4e90, stats, {2}) & 0xff) == value);
      if (!hit) return 0;
      ok = 1; next = true;
      break;
    }
    case 3: {                                                       // reputation towards the centre object ([esp+1C])
      if (value > 2) break;                                         // 0054BB92..0054BB95: esi unchanged
      U centre = p.read(e + 0x1c);
      if (!centre) p.call(0x54bb27, 0, {});                        // ORIGINAL BUG: *ToLocation never writes [esp+1C] (stale read)
      U centre_id = p.read(centre + 4);                             // 0054BB27..0054BB2B (read before the vcall)
      U rep = p.call(0x57cb80, p.vcall(c, 0x10, {}), {centre_id, 0});
      if (value == 0) ok = sge(rep, 0x5a) ? 1u : 0u;
      else if (value == 1) ok = sle(rep, 0xa) ? 1u : 0u;
      else { if (sle(rep, 0xa) || sge(rep, 0x5a)) return 0; ok = 1; next = true; }
      break;
    }
    case 4: break;                                                  // 0054BB48: esi as left by the previous criterion
    case 5: ok = p.call(0x4cdac0, c, {value}); break;               // HasSpellEffectApplied
    case 6: ok = p.call(0x4cdac0, c, {value}) ? 0u : 1u; break;
    default: {                                                      // 7: perception of the candidate by the centre creature
      U centre = p.read(e + 0x1c);
      if (!centre) p.call(0x54bbeb, 0, {});                        // ORIGINAL BUG: *ToLocation never writes [esp+1C] (stale read)
      if (!p.vcall(centre, 0x30, {})) break;                        // 0054BBF4: esi unchanged
      U seen_want = 2, heard_want = 2;                              // bl / [esp+13]; table 0054C0B4
      switch (value) {
      case 0: seen_want = 1; heard_want = 1; break;
      case 1: seen_want = 0; heard_want = 0; break;
      case 2: seen_want = 0; heard_want = 1; break;
      case 3: seen_want = 1; heard_want = 0; break;
      case 4: heard_want = 0; break;
      case 5: heard_want = 1; break;
      case 6: seen_want = 0; break;
      case 7: seen_want = 1; break;
      default: break;
      }
      p.write8(e + 0x13, heard_want);
      U el = p.call(0x517a20, p.vcall(centre, 0x30, {}), {p.read(c + 4)});   // GetVisibleListElement
      U seen = el ? p.read8(el + 4) & 1 : 0, heard = el ? (p.read8(el + 4) >> 1) & 1 : 0;
      if (seen_want != 2 && seen != seen_want) return 0;
      if (heard_want != 2 && heard != heard_want) return 0;
      ok = 1; next = true;
      break;
    }
    }
    if (!next && !ok) return 0;                                     // 0054BB48 -> 0054BC8C (esi = 0)
    if (++j >= 3) return ok;                                        // 0054BB50..0054BB5C
  }
}
bool type_mask_matches(U t, U mask) {                               // 0054BCC4..0054BD4A
  static const U bits[][2] = {{0x1, 5}, {0x2, 6}, {0x4, 7}, {0x8, 0xa}, {0x10, 0xb}, {0x20, 0xc}, {0x40, 9}, {0x80, 0xe}, {0x100, 0xd}, {0x200, 0x10}};
  for (auto& b : bits) if ((mask & b[0]) && t == b[1]) return true;
  return (mask & 0x7fff) == 0x7fff;
}
U abs_s(U v) { S s = static_cast<S>(v); return static_cast<U>(s < 0 ? -s : s); }
}
U get_nearest_object_0054b550(U, U routine, U argc, U e, Ports& p) {
  // e == esp after the prologue pushes (frame 0xC0). +0x14 step, +0x18 index, +0x1C centre object, +0x20 criterion counter
  // then candidate dist2, +0x24 tag, +0x2C candidate, +0x30 lower-case tag, +0x38 area list, +0x3C criterion type,
  // +0x40/+0x44 low/high end reached, +0x48 location, +0x4C/+0x58 criterion 3, +0x50 object id, +0x54/+0x5C criterion 2,
  // +0x60 nth, +0x64..+0x6C delta, +0x70 criterion-1 value, +0x74..+0x7C centre, +0x80 result ids, +0x8C result dist2,
  // +0x98 candidate id / lower-case temporary, +0xA0 type mask, +0xA4 object array, +0xA8 criterion-1 type, +0xAC delta out.
  p.write(e + 0x54, 0xffffffffu); p.write(e + 0x5c, 0xffffffffu); p.write(e + 0x4c, 0xffffffffu); p.write(e + 0x58, 0xffffffffu);
  U result = kInvalidObject;
  p.call(0x5b3190, e + 0x24, {});                                   // CExoString tag
  U array = object_array(p);
  p.write(e + 0xa4, array); p.write(e + 0x48, 0);
  p.call(0x5b3190, e + 0x30, {});
  auto fail_pop = [&]() { p.call(0x5e5c20, e + 0x30, {}); p.call(0x5e5c20, e + 0x24, {}); return kPopFailed; };
  bool object_centred = routine == 0x26 || routine == 0xe3 || routine == 0xe5;
  if (routine == 0x26 || routine == 0xe2) {
    if (!pop_int(e + 0xa8, p) || !pop_int(e + 0x70, p)) return fail_pop();
  } else if (routine == 0xe3 || routine == 0xe4) {
    if (!pop_int(e + 0xa0, p)) return fail_pop();
  } else if (routine == 0xe5) {
    if (!p.call(0x5d1080, vm(p), {e + 0x24})) return fail_pop();    // StackPopString
    U lower = p.call(0x5e6080, e + 0x24, {e + 0x98});               // LowerCase -> temporary
    p.call(0x5e5c50, e + 0x30, {lower});
    p.call(0x5e5c20, e + 0x98, {});
  }
  if (object_centred) { if (!pop_object(e + 0x50, p)) return fail_pop(); }
  else if (routine == 0xe2 || routine == 0xe4) { if (!p.call(0x5d10a0, vm(p), {2, e + 0x48})) return fail_pop(); }   // location
  if (!pop_int(e + 0x60, p)) return fail_pop();
  if (sgt(argc, 4) && (routine == 0x26 || routine == 0xe2)) {
    if (!pop_int(e + 0x54, p) || !pop_int(e + 0x5c, p) || !pop_int(e + 0x4c, p) || !pop_int(e + 0x58, p)) return fail_pop();
  }
  bool scan = false;
  U list = 0, index = 0;
  if (!object_centred) {                                            // 0054B71B: centre = the location's position
    U loc = p.read(e + 0x48);
    p.write(e + 0x74, p.read(loc)); p.write(e + 0x78, p.read(loc + 4)); p.write(e + 0x7c, p.read(loc + 8));
    U pc = p.call(0x4b2820, server(p), {});                         // GetPlayerCreature
    U area = pc ? p.call(0x4cb120, pc, {}) : 0;
    if (area) {
      list = area + 0x190; p.write(e + 0x38, list);
      if (p.call(0x506f20, area, {e + 0x18, p.read(loc)})) { index = p.read(e + 0x18); scan = true; }   // GetFirstObjectIndiceByX
    }
  } else if (lookup(p.read(e + 0x50), e + 0x1c, p)) {
    U c = p.read(e + 0x1c);
    if (p.read8(c + 8) > type::area) {
      p.write(e + 0x74, p.read(c + 0x90)); p.write(e + 0x78, p.read(c + 0x94)); p.write(e + 0x7c, p.read(c + 0x98));
      U area = p.call(0x4cb120, c, {});
      list = area + 0x190; p.write(e + 0x38, list);
      if (area && p.call(0x506f20, area, {e + 0x18, p.read(c + 0x90)})) {
        index = p.read(e + 0x18);
        U id = p.read(e + 0x50);
        while (slt(index, p.read(list + 4)) && p.read(p.read(list) + index * 4) != id) p.write(e + 0x18, ++index);
        scan = slt(index, p.read(list + 4)) && p.read(p.read(list) + index * 4) == id;
      }
    }
  }
  if (scan) {
    for (U o = 0x80; o <= 0x94; o += 4) p.write(e + o, 0);          // result id / dist2 lists
    U step = 1;
    p.write(e + 0x44, 0); p.write(e + 0x40, 0);
    for (;;) {
      if (p.read(e + 0x44) && p.read(e + 0x40)) break;              // 0054B948..0054B956
      p.write(e + 0x14, step);
      U at = step + index;
      if (slt(at, 0)) { p.write(e + 0x40, 1); step = abs_s(step); continue; }
      if (!slt(at, p.read(list + 4))) { p.write(e + 0x44, 1); step = 0u - abs_s(step); continue; }
      U id = p.read(p.read(list) + at * 4);
      p.write(e + 0x98, id);
      if (!p.call(0x4af240, e + 0x80, {id})) {                      // not yet in the result list
        bool accept = false;
        if (lookup(id, e + 0x2c, p)) {
          U cand = p.read(e + 0x2c);
          if (routine == 0x26 || routine == 0xe2) {
            U c = p.vcall(cand, 0x30, {}) ? p.vcall(cand, 0x30, {}) : 0;
            if (c) {
              U ok = creature_criteria(c, e, p);
              bool dead = p.vcall(c, 0x94, {}) == 1 || p.call(0x63e7f0, c, {}) == 1 || p.call(0x4ef890, c, {}) == 1;
              accept = !dead && ok;
            }
          } else if (routine == 0xe3 || routine == 0xe4) {
            accept = type_mask_matches(p.read8(cand + 8), p.read(e + 0xa0));
          } else if (routine == 0xe5) {
            accept = p.call(0x5e5200, cand + 0x18, {e + 0x30}) != 0; // CExoString::operator==
          }
          if (accept) {                                             // 0054BD6D..0054BE5A
            U d = p.call(0x423bc0, 0, {e + 0xac, cand + 0x90, e + 0x74});   // out = a - b
            U dv[3] = {p.read(d), p.read(d + 4), p.read(d + 8)};
            p.write(e + 0x64, dv[0]); p.write(e + 0x68, dv[1]); p.write(e + 0x6c, dv[2]);
            U dist2 = x87::sum_sq_xzy(dv[0], dv[1], dv[2]);
            p.write(e + 0x20, dist2);
            U n = p.read(e + 0x90), nth = p.read(e + 0x60);
            if (sle(nth, 0)) p.call(0x54bde1, 0, {});                // ORIGINAL BUG: nth <= 0 reads dist[nth-1] (before the array)
            if (!slt(n, nth) &&
                x87::delta_sq_above(p.read(cand + 0x90), p.read(e + 0x74), p.read(p.read(e + 0x8c) + (nth - 1) * 4))) {
              U st = p.read(e + 0x14);                              // 0054BDEE..0054BE1B: that side is exhausted
              if (sgt(st, 0)) { p.write(e + 0x44, 1); p.write(e + 0x14, 0u - abs_s(st)); }
              else { p.write(e + 0x40, 1); p.write(e + 0x14, abs_s(st)); }
            }
            U i = 0;
            while (slt(i, n) && !x87::float_below_or_unordered(dist2, p.read(p.read(e + 0x8c) + i * 4))) ++i;
            nearest::float_list_insert_004f78c0(e + 0x8c, dist2, i, p);
            p.call(0x616940, e + 0x80, {p.read(e + 0x98), i});
          }
        }
        index = p.read(e + 0x18); step = p.read(e + 0x14); list = p.read(e + 0x38);   // 0054BE5F
      }
      // 0054BE6B..0054BEBE: alternate sides while both are open, otherwise keep walking outwards
      S st = static_cast<S>(step), a = static_cast<S>(abs_s(step));
      if ((a & 1) == (st > 0 ? 1 : 0) && !p.read(e + 0x44) && !p.read(e + 0x40)) step = 0u - step;
      else step = static_cast<U>(st + st / a);
    }
    U nth = p.read(e + 0x60);
    if (sle(nth, 0)) p.call(0x54bed7, 0, {});                        // ORIGINAL BUG: nth <= 0 reads ids[nth-1] (before the array)
    result = sgt(nth, p.read(e + 0x84)) ? kInvalidObject : p.read(p.read(e + 0x80) + (nth - 1) * 4);   // 0054BEC3..0054BEDD
    p.call(0x6fa390, 0, {p.read(e + 0x8c)});
    p.call(0x6fa390, 0, {p.read(e + 0x80)});
  }
  if (U cr = p.call(0x4ae770, server(p), {result}); cr && routine != 0xe5)   // 0054BF0F..0054BF3C (results unused)
    if (!p.vcall(cr, 0x94, {})) p.call(0x4ef890, cr, {});
  if (!p.call(0x5d10d0, vm(p), {result})) {                         // StackPushObject
    p.call(0x5e5c20, e + 0x30, {}); p.call(0x5e5c20, e + 0x24, {});
    return kPushFailed;
  }
  if (U loc = p.read(e + 0x48)) { p.call(0x4015a0, loc, {}); p.call(0x6fa390, 0, {loc}); }   // location dtor + free
  p.call(0x5e5c20, e + 0x30, {}); p.call(0x5e5c20, e + 0x24, {});
  return 0;
}
}
