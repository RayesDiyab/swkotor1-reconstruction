#include "f5b66431ee-creature_event.hpp"
#include "7dd862643f-creature_ai.hpp"
#include "4a8ca74ca0-leaves.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// G04: 004FECE0 CSWSCreature::EventHandler (creature vtable +0x78).
namespace w1::creature_event {
namespace {
// .rdata event codes ([007441B8..00744220]; names follow the handled behaviour) and object types ([00746633..0074663A]).
constexpr U kTimedEvent = 1, kRemoveFromArea = 4, kApplyEffect = 5, kSpellCastAt = 8, kOnMeleeAttacked = 9,
            kSignalEvent = 10, kDestroyObject = 11, kRemoveEffect = 14, kOnDamaged = 15, kForcedAction = 18,
            kSpellImpact = 19, kIgnored20 = 20, kBroadcastProjectile = 21, kFeedbackMessage = 22, kRumble = 27;
constexpr U kEffectSpecial = 40;           // [0074671C]
constexpr U kScriptOk = 0;                 // [0074666C] CGameObjectArray::GetGameObject success code
constexpr U kTypeArea = 4, kTypeCreature = 5, kTypeTrigger = 7, kTypePlaceable = 9, kTypeDoor = 10, kTypeAreaOfEffect = 11;
constexpr U kDebugEventStrings = 0x8327e4; // BSS debug flag
U vm(Ports& p) { return p.global(kVirtualMachine); }
U client(Ports& p) { return p.read(p.global(kAppManager) + 4); }
void run_script(U script, U c, Ports& p) { p.call(0x5d0fc0, vm(p), {script, p.read(c + 4), 1}); }
void free_mem(U mem, Ports& p) { p.call(0x6fa390, 0, {mem}); }
U get_int(U msg, U i, Ports& p) { return leaves::message_get_integer_004d6480(msg, i, p); }
U get_obj(U msg, U i, Ports& p) { return leaves::message_get_object_004d64c0(msg, i, p); }
U new_message(Ports& p) {
  U mem = p.call(0x6fa7e6, 0, {0x34});
  return mem ? leaves::message_data_construct_004d69f0(mem, p) : 0;
}
void destroy_script_event(U ev, Ports& p) { p.call(0x4d7590, ev, {}); free_mem(ev, p); }   // 004FF927 + 004FFA68
// 004FF482..004FF92E: script event (CScriptEvent*) by subtype.
void signal_event(U c, U caller, U ev, U e, Ports& p) {
  switch (p.read16(ev)) {
  case 1: {                                                         // 004FF4A1 perception
    p.write(c + 0x900, get_obj(ev, 0, p));
    if (get_int(ev, 0, p) == 1) { p.write(c + 0x90c, 1); p.write(c + 0x908, 0); p.write(c + 0x904, 0); p.write(c + 0x910, 0); }
    else if (get_int(ev, 0, p) == 0) { p.write(c + 0x904, 1); p.write(c + 0x90c, 0); p.write(c + 0x908, 0); p.write(c + 0x910, 0); }
    else if (get_int(ev, 0, p) == 3) { p.write(c + 0x904, 0); p.write(c + 0x90c, 0); p.write(c + 0x908, 0); p.write(c + 0x910, 1); }
    else if (get_int(ev, 0, p) == 2) { p.write(c + 0x90c, 0); p.write(c + 0x908, 1); p.write(c + 0x904, 0); p.write(c + 0x910, 0); }
    if (p.read(c + 0xa88)) {                                        // 004FF53A..004FF56C party-controlled: only party members
      U id = p.call(0x5eda50, client(p), {p.read(c + 4)});
      if (!p.call(0x634620, p.call(0x5ed8b0, client(p), {}), {id})) break;
    }
    run_script(c + 0x238, c, p);
    break;
  }
  case 2: {                                                         // 004FF72B attacked
    U obj = p.call(0x4ae750, server(p), {get_obj(ev, 0, p)});
    U attacker = kInvalidObject;
    if (obj) {
      U kind = p.read8(obj + 8);
      if (kind == kTypeCreature) attacker = get_obj(ev, 0, p);
      else if (kind == kTypeAreaOfEffect) {
        U cr = p.call(0x4ae770, server(p), {p.read(p.vcall(obj, 0x50, {}) + 0x248)});
        if (cr) attacker = p.read(cr + 4);
      }
    }
    p.write(c + 0x1d8, attacker);
    p.write(c + 0x1dc, get_int(ev, 0, p));
    p.write(c + 0x1e0, get_int(ev, 1, p));
    if (p.read(c + 0x1e0) == 1) p.call(0x4cc0c0, c, {p.read(c + 0x1d8), 0});   // SetLastHostileActor
    if (!p.vcall(c, 0x94, {}) && !p.call(0x4ef890, c, {})) run_script(c + 0x240, c, p);
    U cr = p.call(0x4ae770, server(p), {p.read(c + 0x1d8)});
    if (!cr) break;
    if (p.read(c + 0x1e0)) {                                        // 004FF82F..004FF8AD
      p.call(0x4fbbe0, cr, {p.read(c + 4)});                        // BroadcastCombatStateToParty
      p.write(cr + 0x4dc, p.read(cr + 0xa88) ? 0u : 1u);
      p.call(0x4f3580, cr, {2});                                    // SetExcitedState
      p.call(0x4f5f70, cr, {});                                     // RemoveCombatInvisibilityEffects
      p.call(0x4fbbe0, c, {p.read(cr + 4)});
      p.write(c + 0x4dc, p.read(c + 0xa88) ? 0u : 1u);
      p.call(0x4f3580, c, {1});
      p.call(0x5c5630, ai_master(p), {p.read(c + 4), p.read(c + 0x1d8), 0});   // void_return_0xc
    } else {
      p.call(0x5c5630, ai_master(p), {p.read(c + 4), p.read(c + 0x1d8), 3});
    }
    break;
  }
  case 7: {                                                         // 004FF5C6 conversation
    p.write(c + 0x17c, caller);
    p.write(c + 0x198, get_int(ev, 1, p));
    U s1 = p.call(0x4d7360, ev, {e + 0x18, 0});                     // GetString(out, 0)
    p.call(0x5e5c50, c + 0x184, {s1});                              // CExoString::operator=
    p.call(0x5e5c20, e + 0x18, {});
    U s2 = p.call(0x4d7360, ev, {e + 0x20, 0});
    p.call(0x4aefd0, server(p), {s2});                              // SetLastConversation
    p.call(0x5e5c20, e + 0x20, {});
    p.write(c + 0x180, get_int(ev, 2, p));
    p.call(0x4af010, server(p), {1});                               // SetActionConversation
    p.call(0x4af030, server(p), {get_int(ev, 3, p)});               // SetIgnoreConversationStartRange
    p.call(0x4cfa50, c, {});                                        // ClearMatchedExpressionStrings
    U i = 1;
    if (sgt(get_int(ev, 0, p), i)) {
      do {
        U s = p.call(0x4d7360, ev, {e + 0x28, i});
        p.call(0x4cf9b0, c, {s});                                   // AddMatchedExpressionString
        p.call(0x5e5c20, e + 0x28, {});
        ++i;
      } while (slt(i, get_int(ev, 0, p)));
    }
    U dialog = c + 0x268;                                           // 004FF6CC..004FF6F4
    if (p.call(0x5e58a0, dialog, {}) || p.call(0x5e5290, dialog, {0x73df6c}))   // IsEmpty || == "default"
      p.call(0x5e5140, dialog, {0x7471d8});                         // = "k_hen_dialogue01"
    p.call(0x5e5670, dialog, {});                                   // CStr (unused)
    run_script(dialog, c, p);
    break;
  }
  case 11:                                                          // 004FF70C
    p.write(c + 0x150, get_int(ev, 0, p));
    run_script(c + 0x288, c, p);
    break;
  case 27:                                                          // 004FF8E0 disturbed
    p.write(c + 0x164, caller);
    p.call(0x4cc0c0, c, {caller, 0});
    p.write(c + 0x178, get_int(ev, 0, p));
    p.write(c + 0x174, get_obj(ev, 0, p));
    run_script(c + 0x258, c, p);
    break;
  case 31:                                                          // 004FF591 blocked
    if (p.call(0x4ae910, server(p), {p.read(c + 4)})) break;        // GetClientObjectByObjectId
    p.write(c + 0x33c, caller);
    run_script(c + 0x290, c, p);
    break;
  default: break;
  }
  destroy_script_event(ev, p);
}
}
void event_handler_004fece0(U c, U code, U caller, U data, U e, Ports& p) {
  // e+N == [esp+N] after the prologue (0x20 locals + 4 pushes + EH frame); +0x40/+0x44/+0x48 are the code/caller/data slots.
  p.write(e + 0x40, code); p.write(e + 0x44, caller); p.write(e + 0x48, data);
  if (!p.read(c + 0x34c)) {                                         // 004FECFD..004FED0B
    U f = e + 0x54; p.write(f + 0x48, 0); creature_ai::spawn_heartbeat_perception_004eb6e0(c, f, p);
  }
  if (code == kApplyEffect) {                                       // 004FED14..004FED5B
    if (!data) return;
    U special = p.read16(data + 8) == kEffectSpecial ? 1u : 0u;
    if (special) p.write(c + 0x148, 1);
    p.call(0x4d14f0, c, {data, 0, special});                        // ApplyEffect
    if (special) p.write(c + 0x148, 0);
    return;
  }
  if (code == kRemoveEffect) {                                      // 004FED60..004FED82
    if (data) p.call(0x4d06c0, c, {p.read(data), p.read(data + 4)});   // RemoveEffectById
    return;
  }
  if (code == kRemoveFromArea) {                                    // 004FED87..004FEDA3
    if (data) p.call(0x4f0fd0, c, {data});
    return;
  }
  if (code == kOnMeleeAttacked) {                                   // 004FEDA8..004FEDF4
    if (!data || p.vcall(c, 0x94, {}) || p.call(0x63e7f0, c, {}) || p.call(0x4ef890, c, {})) return;
    p.vcall(c, 0x7c, {data});
    return;
  }
  if (code == kTimedEvent) {                                        // 004FEDF9..004FEE1F
    if (data) p.call(0x5d0fd0, vm(p), {data, p.read(c + 4), 1});    // RunScriptSituation (W6)
    return;
  }
  if (code == kIgnored20) return;
  if (code == kBroadcastProjectile) {                               // 004FEE30..004FEED8
    if (!data) return;
    U cr1 = p.call(0x4ae770, server(p), {caller});
    U cr2 = p.call(0x4ae770, server(p), {p.read(data + 0xc)});
    if (cr1 && cr2) {
      U obj = p.call(0x4ae750, server(p), {p.read(data + 0x74)});
      if (obj && obj - 0x10) {
        U base = p.call(0x5b4790, obj - 0x10, {});                  // CSWItem::GetBaseItem
        p.call(0x4cdb00, c, {caller, p.read(cr2 + 4), cr1 + 0x90, cr2 + 0x90, p.read16(data + 0x10), p.read8(data + 0x5c), 2,
                             p.read8(base + 0x8e), 0, 0});          // BroadcastSafeProjectile
      }
    }
    p.call(0x4d4480, data, {});                                     // ~CSWSCombatAttackData
    free_mem(data, p);
    return;
  }
  if (code == kFeedbackMessage) {                                   // 004FEEDD..004FEFDB
    if (!data) return;
    U cr = p.call(0x4ae770, server(p), {caller});
    U kind = p.read8(data);
    if (kind == 1) {
      p.call(0x4ec760, c, {data});                                  // BroadcastSavingThrowData
      if (cr) p.call(0x4ec760, cr, {data});
      leaves::message_data_destroy_004d6a40(data, p);
      free_mem(data, p);
      return;
    }
    if (kind == 3) {
      if (cr) leaves::send_feedback_004ede10(cr, get_int(data, 0, p), data, p);
      return;
    }
    if (kind == 4) { leaves::send_feedback_004ede10(c, get_int(data, 0, p), data, p); return; }
    U mem = p.call(0x6fa7e6, 0, {0x34});                            // 004FEF6D copy for the other side
    p.write(e + 0x48, mem);
    U copy = mem ? leaves::message_data_construct_004d69f0(mem, p) : 0;
    p.call(0x4d6ee0, data, {copy});                                 // CopyTo
    leaves::send_feedback_004ede10(c, get_int(data, 0, p), data, p);
    if (cr) { leaves::send_feedback_004ede10(cr, get_int(copy, 0, p), copy, p); return; }
    if (copy) { leaves::message_data_destroy_004d6a40(copy, p); free_mem(copy, p); }
    return;
  }
  if (code == kOnDamaged) {                                         // 004FEFE0..004FF187
    U attacker = p.call(0x4ae770, server(p), {caller});
    U weapon = 0;
    if (data) {
      U mem = p.call(0x6fa7e6, 0, {0x34});
      p.write(e + 0x40, mem);
      U msg = mem ? leaves::message_data_construct_004d69f0(mem, p) : 0;
      p.call(0x4d7780, msg, {0, caller});
      p.call(0x4d7780, msg, {1, p.read(c + 4)});
      if (p.global(kDebugEventStrings)) p.call(0x4d6ae0, msg, {0, data + 0x78});
      if (msg) { leaves::message_data_destroy_004d6a40(msg, p); free_mem(msg, p); }
      p.write16(c + 0x16c, p.read16(data + 0x64));
      p.write8(c + 0x16e, p.read8(data + 0x3b));
      U slot = 0;
      switch (p.read8(data + 0x3a)) {                               // 004FF09C..004FF0DC attack kind -> equipment slot
      case 1: case 6: slot = 0x10; break;
      case 2: slot = 0x20; break;
      case 7: case 8: slot = 8; break;
      case 3: slot = 0x4000; break;
      case 4: slot = 0x8000; break;
      case 5: slot = 0x10000; break;
      default: break;
      }
      if (slot) weapon = p.call(0x5a4c20, p.read(attacker + 0xa2c), {slot});   // GetItemInSlot (attacker may be 0: as the original)
    }
    p.write(c + 0x15c, caller);                                     // 004FF0EB
    p.call(0x4cc0c0, c, {caller, 0});                               // SetLastHostileActor
    if (attacker) {
      p.call(0x4fbbe0, attacker, {p.read(c + 4)});
      p.call(0x4f5f70, attacker, {});
      p.write(attacker + 0x170, weapon ? p.read(weapon + 0x14) : kInvalidObject);
    }
    p.call(0x4fbbe0, c, {caller});
    p.write(c + 0x4dc, p.read(c + 0xa88) ? 0u : 1u);
    if (p.vcall(c, 0x94, {}) || p.call(0x4ef890, c, {})) return;
    run_script(c + 0x248, c, p);
    return;
  }
  if (code == kSpellCastAt || code == kSpellImpact) {               // 004FF957..004FFA68
    U spell = data;
    if (!spell || p.read(spell + 4) != caller) { free_mem(spell, p); return; }
    p.write(c + 0x1c0, p.read(spell));
    p.write(c + 0x1bc, p.read(spell + 8));
    p.call(0x64b4f0, c, {spell + 0x10});                            // SetSpellTargetPosition
    p.write(c + 0x95c, p.read(spell + 0xc));
    U ebx = 0;
    U target = p.read(spell + 8);
    if (target != kInvalidObject) {
      U array = p.read(server_internal(p) + 0x1005c);               // GetObjectArray
      if ((p.call(0x4d8230, array, {target, e + 0x48}) & 0xff) == kScriptOk) {
        U o = p.read(e + 0x48);
        if (p.read8(o + 8) > kTypeArea) {
          ebx = p.call(0x4cb120, o, {});
          if (p.call(0x4cb120, c, {}) == ebx) p.call(0x64b4f0, c, {p.read(e + 0x48) + 0x90});
          ebx = 0;                                                  // 004FF9F5: both outcomes of 004FF9E1 reach xor ebx, ebx
        }
      }
    }
    p.vcall(c, 0xbc, {p.read(spell)});
    p.write(c + 0x14c, p.read(spell));
    if (code == kSpellImpact) {
      p.write(c + 0x964, 1);
      p.write(c + 0x968, p.read(spell + 0x28));
      p.call(0x4cb9e0, c, {ebx});                                   // CalculateLastSpellProjectileTime
    }
    run_script(spell + 0x1c, c, p);
    p.vcall(c, 0xbc, {0xffffffffu});
    p.write(c + 0x14c, 0xffffffffu);
    p.write(c + 0x964, 0);
    p.write(c + 0x968, 0xffffffffu);
    free_mem(spell, p);
    return;
  }
  if (code == kDestroyObject) {                                     // 004FF1A4..004FF438
    if (p.read(c + 0x9d4) == 1) p.call(0x5e5670, c + 0x18, {});
    if (!p.read(c + 0xec)) p.call(0x5e5670, c + 0x18, {});
    if (p.read(c + 0xa88)) p.call(0x5e5670, c + 0x18, {});
    if (p.read(c + 0x9d4) || !p.read(c + 0xec) || p.read(c + 0xa88)) return;
    if (p.vcall(c, 0x94, {})) {                                     // dead: leave a body bag for the client creature
      U id = p.call(0x5eda50, client(p), {p.read(c + 4)});
      U cl = client(p);
      U bag = p.call(0x5eda50, cl, {p.call(0x4ce220, c, {})});      // SpawnBodyBag
      U cc = p.call(0x5ed5b0, client(p), {id});
      if (cc) p.write(cc + 0x3dc, bag);
    }
    if (!data) {                                                    // 004FF25B..004FF2CC fade delay from appearance.2da
      p.call(0x5e5a90, e + 0x10, {0x7471ec});                       // CExoString("FadeDelayOnDeath")
      U table = p.read(p.read(p.global(kRules) + 0xb8) + 0xc);
      U ok = p.call(0x413660, table, {p.read16(c + 0xa60), e + 0x10, e + 0x48});   // C2DA::GetINTEntry
      p.call(0x5e5c20, e + 0x10, {});
      if (ok) { p.call(0x4ce8a0, c, {p.read(e + 0x48)}); p.call(0x4ce9a0, c, {0}); }   // SetFadeOutTime, SetKeepCorpse(0)
      else p.call(0x4ce9a0, c, {1});
    }
    p.call(0x4f0fd0, c, {0});                                       // RemoveFromArea
    if (p.call(0x4cb120, c, {})) {                                  // 004FF2DA..004FF3E4 drop from mine detection lists
      U area = p.call(0x4cb120, c, {});
      U list = area + 0x12c;
      p.write(e + 0x48, 0);
      if (sgt(p.read(area + 0x130), 0)) {
        U i = 0;
        do {
          U obj = p.call(0x4ae750, server(p), {p.read(p.read(list) + i * 4)});
          if (obj) {
            U kind = p.read8(obj + 8), slot = 0, off = 0;
            if (kind == kTypeTrigger) { slot = 0x40; off = 0x2a8; }
            else if (kind == kTypeDoor) { slot = 0x18; off = 0x2dc; }
            else if (kind == kTypePlaceable) { slot = 0x48; off = 0x318; }
            if (slot) {
              U id = p.read(c + 4);
              if (p.call(0x4af240, p.vcall(obj, slot, {}) + off, {id})) {        // CExoArrayList::Count(id)
                U id2 = p.read(c + 4);
                p.call(0x66dcc0, p.vcall(obj, slot, {}) + off, {id2});          // CExoArrayList<int>::remove
              }
            }
          }
          i = p.read(e + 0x48) + 1;
          p.write(e + 0x48, i);
        } while (slt(i, p.read(area + 0x130)));
      }
    }
    if (!p.read(c + 0xa28)) {                                       // 004FF3EA..004FF426 encounter bookkeeping
      U enc = p.call(0x4ae7f0, server(p), {p.read(c + 0xa24)});
      if (enc) {
        p.call(0x594310, enc, {p.read(p.read(c + 0xa74) + 0x84), 0});   // RemoveFromActiveCreatureCount(float, 0)
        p.write(c + 0xa28, 1);
      }
    }
    p.vcall(c, 0x00, {1});                                          // scalar deleting destructor
    return;
  }
  if (code == kForcedAction) {                                      // 004FF43D..004FF471
    U act = data;
    if (p.call(0x4f1ef0, c, {0, p.read16(act + 4)}) == 1 && p.read(act) == 1)   // CancelAction
      p.call(0x4edba0, c, {act});                                   // ForceMoveToPoint
    p.call(0x4015a0, act, {});                                      // void_return (trivial destructor)
    free_mem(act, p);
    return;
  }
  if (code == kSignalEvent) {                                       // 004FF476
    signal_event(c, caller, data, e, p);
    return;
  }
  if (code == kRumble) p.call(0x5ede40, client(p), {data});         // PlayRumblePattern
}
}
