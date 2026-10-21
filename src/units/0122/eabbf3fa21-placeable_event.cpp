#include "14b14dc5e6-placeable_event.hpp"
#include "4a8ca74ca0-leaves.hpp"
#include "1a847f7684-w1_x87.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// G05: 00587BA0 CSWSPlaceable::EventHandler (vtable +0x78).
namespace w1::placeable_event {
namespace {
constexpr U kTimedEvent = 1, kRemoveFromArea = 4, kApplyEffect = 5, kOpenInventory = 7, kSpellCastAt = 8, kForward7C = 9,
            kSignalEvent = 10, kDestroyObject = 11, kRemoveEffect = 14, kDamaged = 15, kFeedbackMessage = 22, kAcquireItem = 25;
constexpr U kScriptOk = 0, kTypeArea = 4;
constexpr U kDebugEventStrings = 0x8327e4;
void run_script(U script, U id, Ports& p) { p.call(0x5d0fc0, p.global(kVirtualMachine), {script, id, 1}); }
U get_int(U m, U i, Ports& p) { return leaves::message_get_integer_004d6480(m, i, p); }
U get_obj(U m, U i, Ports& p) { return leaves::message_get_object_004d64c0(m, i, p); }
void release_inventory_list(U o, Ports& p) {                        // 00587FFD..0058801E / 00588190..005881B1
  if (p.read(o + 0x318)) { p.call(0x6fa390, 0, {p.read(o + 0x318)}); p.write(o + 0x318, 0); p.write(o + 0x320, 0); }
  p.write(o + 0x31c, 0);
}
// 00587F23..00588425: script event subtypes (word value indexes the bytes at 00588670, which select the jump table 00588634).
void signal_event(U o, U caller, U day, U ms, U ev, U e, Ports& p) {
  U id = p.read(o + 4);
  switch (p.read16(ev)) {
  case 0x00: p.write(o + 0x348, day); p.write(o + 0x34c, ms); run_script(o + 0x2b4, id, p); break;   // 0058802E heartbeat
  case 0x02: {                                                      // 00588371 attacked
    p.write(o + 0x1d8, get_obj(ev, 0, p));
    p.write(o + 0x1dc, get_int(ev, 0, p));
    p.write(o + 0x1e0, get_int(ev, 1, p));
    if (p.read(o + 0x1e0)) {
      p.call(0x4cc0c0, o, {p.read(o + 0x1d8), 0});                  // SetLastHostileActor
      U cr = p.call(0x4ae770, server(p), {p.read(o + 0x1d8)});
      if (cr) {
        p.call(0x4fbbe0, cr, {kInvalidObject});                     // BroadcastCombatStateToParty
        p.call(0x4f3580, cr, {2});                                  // SetExcitedState
        p.call(0x4f5f70, cr, {});                                   // RemoveCombatInvisibilityEffects
      }
    }
    run_script(o + 0x2dc, id, p);
    break;
  }
  case 0x04: p.write(o + 0x160, caller); p.call(0x4cc0c0, o, {caller, 0}); run_script(o + 0x29c, id, p); break;   // 00587F74
  case 0x07: {                                                      // 005881C1 conversation
    p.write(o + 0x17c, caller);
    p.write(o + 0x198, get_int(ev, 1, p));
    U s1 = p.call(0x4d7360, ev, {e + 0x10, 0});
    p.call(0x5e5c50, o + 0x184, {s1});
    p.call(0x5e5c20, e + 0x10, {});
    U s2 = p.call(0x4d7360, ev, {e + 0x18, 0});
    p.call(0x4aefd0, server(p), {s2});                              // SetLastConversation
    p.call(0x5e5c20, e + 0x18, {});
    p.write(o + 0x180, get_int(ev, 2, p));
    p.call(0x4af010, server(p), {1});                               // SetActionConversation
    p.call(0x4af030, server(p), {get_int(ev, 3, p)});               // SetIgnoreConversationStartRange
    p.call(0x4cfa50, o, {});                                        // ClearMatchedExpressionStrings
    U i = 1;
    if (sgt(get_int(ev, 0, p), i)) {
      do {
        U s = p.call(0x4d7360, ev, {e + 0x20, i});
        p.call(0x4cf9b0, o, {s});                                   // AddMatchedExpressionString
        p.call(0x5e5c20, e + 0x20, {});
        ++i;
      } while (slt(i, get_int(ev, 0, p)));
    }
    U dialog = o + 0x304;                                           // 005882C8..005882F4
    if (p.call(0x5e58a0, dialog, {}) || p.call(0x5e5290, dialog, {0x73df6c}))
      p.call(0x5e5140, dialog, {0x7471d8});                         // "k_hen_dialogue01"
    run_script(dialog, id, p);
    break;
  }
  case 0x0a: p.vcall(o, 0x7c, {0x2758}); p.write(o + 0x154, caller); run_script(o + 0x2a4, id, p); break;   // 00587F96
  case 0x0b: p.write(o + 0x150, get_int(ev, 0, p)); run_script(o + 0x2fc, id, p); break;                   // 005883F7
  case 0x16: p.write(o + 0x350, caller); run_script(o + 0x2d4, id, p); break;                             // 00587F41
  case 0x17: p.write(o + 0x354, caller); run_script(o + 0x294, id, p); break;                             // 00587F59
  case 0x18:                                                        // 00587FBD used
    if (p.read(o + 0x278)) { p.write(o + 0x278, 0); p.write(o + 0x360, caller); run_script(o + 0x2ac, id, p); }
    if (sgt(p.read(o + 0x31c), 0)) release_inventory_list(o, p);
    break;
  case 0x19:                                                        // 00588053
    if (p.read(o + 0x328)) { p.write(o + 0x358, caller); run_script(o + 0x2f4, id, p); }
    break;
  case 0x1a: {                                                      // 00588079 trap triggered / disarm attempt
    U force = get_int(ev, 0, p);
    U cr = p.call(0x4ae770, server(p), {caller});
    if (cr && p.call(0x5a6960, p.read(cr + 0xa74), {5, 0})) break;                 // GetEffectImmunity([00746910], 0)
    if (!force) {
      if (sge(p.call(0x57cb80, o, {caller, 0}), 0x5a)) break;                      // GetReputation >= 90
      if (p.read(o + 0x23c) == p.read(p.read(cr + 0xa74) + 0x78)) break;           // same faction (cr may be 0: as the original)
    }
    if (!p.read(o + 0x278)) break;
    if (cr) leaves::send_feedback_004ede10(cr, 0x52, 0, p);
    p.write(o + 0x35c, caller);
    run_script(o + 0x2e4, id, p);
    if (p.read(o + 0x288)) {                                        // 00588128..0058817F: one-shot: leave the area list
      p.write(o + 0x278, 0);
      if (p.call(0x4cb120, o, {}) && p.call(0x4cb120, o, {}) + 0x12c) {
        U oid = p.read(o + 4);
        if (p.call(0x4af240, p.call(0x4cb120, o, {}) + 0x12c, {oid}) == 1) {
          U oid2 = p.read(o + 4);
          p.call(0x66dcc0, p.call(0x4cb120, o, {}) + 0x12c, {oid2});
        }
      }
    }
    if (sgt(p.read(o + 0x31c), 0)) release_inventory_list(o, p);
    break;
  }
  case 0x1b:                                                        // 00588305 disturbed
    p.write(o + 0x164, caller);
    p.write(o + 0x178, get_int(ev, 0, p));
    p.write(o + 0x174, get_obj(ev, 0, p));
    run_script(o + 0x2bc, id, p);
    break;
  case 0x1c: p.write(o + 0x364, caller); run_script(o + 0x2c4, id, p); break;   // 0058833E
  case 0x1d: p.write(o + 0x368, caller); run_script(o + 0x2ec, id, p); break;   // 00588356
  default: break;
  }
  p.call(0x4d7590, ev, {});                                         // ~CScriptEvent
  p.call(0x6fa390, 0, {ev});
}
}
void event_handler_00587ba0(U o, U code, U caller, U data, U day, U ms, U e, Ports& p) {
  // e+N == [esp+N] after `push ebp` (00587C4D); +0x38..+0x48 are the five argument slots.
  p.write(e + 0x38, code); p.write(e + 0x3c, caller); p.write(e + 0x40, data); p.write(e + 0x44, day); p.write(e + 0x48, ms);
  if (code == kFeedbackMessage) {                                   // 00587BBF..00587C43
    if (!data) return;
    U cr = p.call(0x4ae770, server(p), {caller});
    if (p.read8(data) == 1) {
      if (cr) p.call(0x4ec760, cr, {data});                         // BroadcastSavingThrowData
    } else if (cr) {
      leaves::send_feedback_004ede10(cr, get_int(data, 0, p), data, p);
      return;
    }
    leaves::message_data_destroy_004d6a40(data, p);
    p.call(0x6fa390, 0, {data});
    return;
  }
  if (code == kDamaged) {                                           // 00587C46..00587ED3
    U cr = 0;
    p.write(e + 0x38, 0);                                           // weapon
    if (data) {
      U mem = p.call(0x6fa7e6, 0, {0x34});
      p.write(e + 0x40, mem);
      U msg = mem ? leaves::message_data_construct_004d69f0(mem, p) : 0;
      p.call(0x4d7780, msg, {0, caller});
      p.call(0x4d7780, msg, {1, p.read(o + 4)});
      if (p.global(kDebugEventStrings)) p.call(0x4d6ae0, msg, {0, data + 0x78});
      cr = p.call(0x4ae770, server(p), {caller});
      if (cr) {
        p.call(0x5b5e90, cr, {msg, data});                          // void_return (8)
        p.call(0x4f5f70, cr, {});
      }
      if (msg) { leaves::message_data_destroy_004d6a40(msg, p); p.call(0x6fa390, 0, {msg}); }
      p.write16(o + 0x16c, p.read16(data + 0x64));
      p.write8(o + 0x16e, p.read8(data + 0x3b));
      U slot = 0;
      switch (p.read8(data + 0x3a)) {
      case 1: case 6: slot = 0x10; break;
      case 2: slot = 0x20; break;
      case 7: case 8: slot = 8; break;
      case 3: slot = 0x4000; break;
      case 4: slot = 0x8000; break;
      case 5: slot = 0x10000; break;
      default: break;
      }
      if (slot) p.write(e + 0x38, p.call(0x5a4c20, p.read(cr + 0xa2c), {slot}));   // GetItemInSlot
    } else {
      cr = caller;                                                  // 00587DB9: the original reuses the caller id here
    }
    p.write(o + 0x15c, caller);
    p.call(0x4cc0c0, o, {caller, 0});
    if (cr) {
      p.call(0x4fbbe0, cr, {kInvalidObject});
      p.call(0x4f5f70, cr, {});
      U weapon = p.read(e + 0x38);
      p.write(cr + 0x170, weapon ? p.read(weapon + 0x14) : kInvalidObject);
    }
    run_script(o + 0x2cc, p.read(o + 4), p);                        // OnDamaged
    if (p.read(o + 0x278) != 1 || !cr) return;
    U a[3] = {p.read(cr + 0x90), p.read(cr + 0x94), p.read(cr + 0x98)}, b[3] = {p.read(o + 0x90), p.read(o + 0x94), p.read(o + 0x98)};
    if (!x87::mine_within_trigger(a, b, 0x41800000u)) return;       // 00587E35..00587E7A: within 4 m ([007455D0] 16.0f)
    U mem = p.call(0x6fa7e6, 0, {0x34});
    p.write(e + 0x3c, mem);
    U ev = mem ? p.call(0x4d7540, mem, {}) : 0;                      // CScriptEvent
    p.write16(ev, 0x1a);
    p.call(0x4b08d0, ai_master(p), {0, 0, caller, p.read(o + 4), kSignalEvent, ev});   // AddEventDeltaTime
    return;
  }
  if (code == kAcquireItem) {                                       // 00587ED8..00587F12
    U item = p.call(0x4ae760, server(p), {caller});
    p.write(e + 0x40, item);
    if (item) p.call(0x584b10, o, {e + 0x40, kInvalidObject, 1});   // CSWSPlaceable::AcquireItem
    return;
  }
  if (code == kSignalEvent) { signal_event(o, caller, day, ms, data, e, p); return; }
  if (code == kDestroyObject) {                                     // 0058842A..00588457
    p.call(0x587770, o, {});                                        // ClosePlaceableForAllPlayers
    p.call(0x4ce220, o, {});                                        // SpawnBodyBag
    p.call(0x584950, o, {});                                        // CSWSPlaceable::RemoveFromArea
    if (o) p.vcall(o, 0x00, {1});
    return;
  }
  if (code == kRemoveFromArea) { p.call(0x584950, o, {}); return; }
  if (code == kApplyEffect) { if (data) p.call(0x4d14f0, o, {data, 0, 0}); return; }
  if (code == kRemoveEffect) { if (data) p.call(0x4d06c0, o, {p.read(data), p.read(data + 4)}); return; }
  if (code == kForward7C) { if (data && !p.vcall(o, 0x94, {})) p.vcall(o, 0x7c, {data}); return; }
  if (code == kTimedEvent) { if (data) p.call(0x5d0fd0, p.global(kVirtualMachine), {data, p.read(o + 4), 1}); return; }
  if (code == kSpellCastAt) {                                       // 0058851A..005885F5
    U spell = data;
    if (!spell || p.read(spell + 4) != caller) { p.call(0x6fa390, 0, {spell}); return; }
    p.write(o + 0x1c0, p.read(spell));
    p.write(o + 0x1bc, p.read(spell + 8));
    p.call(0x64b4f0, o, {spell + 0x10});                            // SetSpellTargetPosition
    U target = p.read(spell + 8);
    if (target != kInvalidObject) {
      U array = p.read(server_internal(p) + 0x1005c);
      if ((p.call(0x4d8230, array, {target, e + 0x3c}) & 0xff) == kScriptOk) {
        U t = p.read(e + 0x3c);
        if (p.read8(t + 8) > kTypeArea) {
          U mine = p.call(0x4cb120, o, {});
          if (mine == p.call(0x4cb120, p.read(e + 0x3c), {})) p.call(0x64b4f0, o, {p.read(e + 0x3c) + 0x90});
        }
      }
    }
    p.vcall(o, 0xbc, {p.read(spell)});
    p.write(o + 0x14c, p.read(spell));
    run_script(spell + 0x1c, p.read(o + 4), p);
    p.vcall(o, 0xbc, {0xffffffffu});
    p.call(0x6fa390, 0, {spell});
    return;
  }
  if (code == kOpenInventory) {                                     // 005885F7..00588619
    if (!data) return;
    p.call(0x587420, o, {p.read(data), 0});                         // CSWSPlaceable::OpenInventory
    p.call(0x6fa390, 0, {data});
  }
}
}
