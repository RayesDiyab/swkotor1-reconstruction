#include "0d8d5e7c97-door_event.hpp"
#include "4a8ca74ca0-leaves.hpp"
#include "5ecaebb99c-perception.hpp"
#include "1a847f7684-w1_x87.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// G05: 0058B850 CSWSDoor::EventHandler (vtable +0x78) and 005890D0 CSWSDoor::GetIsLinked.
namespace w1::door_event {
namespace {
constexpr U kTimedEvent = 1, kApplyEffect = 5, kCloseDoor = 6, kOpenDoor = 7, kForward7C = 9, kSignalEvent = 10, kDestroyObject = 11,
            kDamaged = 15, kFeedbackMessage = 22;
constexpr U kDebugEventStrings = 0x8327e4;
U client(Ports& p) { return p.read(p.global(kAppManager) + 4); }
void run_script(U script, U id, Ports& p) { p.call(0x5d0fc0, p.global(kVirtualMachine), {script, id, 1}); }
U get_int(U m, U i, Ports& p) { return leaves::message_get_integer_004d6480(m, i, p); }
U get_obj(U m, U i, Ports& p) { return leaves::message_get_object_004d64c0(m, i, p); }
void release_list(U d, Ports& p) {                                  // 0058BD12..0058BD31 / 0058BE79..0058BE98
  if (p.read(d + 0x2dc)) { p.call(0x6fa390, 0, {p.read(d + 0x2dc)}); p.write(d + 0x2dc, 0); p.write(d + 0x2e4, 0); }
  p.write(d + 0x2e0, 0);
}
// 0058C0BE..0058C426: subtype 0x1E (transition click).
void transition(U d, U ev, U e, Ports& p) {
  p.write(d + 0x324, get_obj(ev, 0, p));
  U cr = p.call(0x4ae770, server(p), {get_obj(ev, 0, p)});
  if (door_is_linked_005890d0(d, p)) {
    U area = p.call(0x4cb120, d, {});
    if (!cr || p.read(p.call(0x5ed690, client(p), {}) + 0xb4)) {   // 0058C35C
      if (area) p.call(0x506b30, area, {0, 0});
      return;
    }
    if (!area || !sgt(p.call(0x5e5790, d + 0x390, {}), 0)) return;  // destination module name
    if (p.read(area + 0x2c4)) {                                     // 0058C13D..0058C1E3 pending transition
      p.write8(e + 0x58, p.read8(area + 0x2c9));
      if (get_int(ev, 0, p) != p.read8(e + 0x58)) return;
      if (!p.vcall(cr, 0x94, {}) && !p.call(0x4ef890, cr, {})) {
        U srv = server(p);
        p.call(0x4aecc0, srv, {1});                                 // SetMoveToModulePending
        p.call(0x4aecd0, srv, {d + 0x390});                         // SetMoveToModuleString
        p.call(0x4aed30, srv, {d + 0x388});                         // SetMoveToModuleStartWaypoint
      } else {
        p.call(0x62ac40, p.call(0x5ed690, client(p), {}), {});      // StopGlobalFade
      }
      p.call(0x506b30, area, {0, 0});
      return;
    }
    if (!p.read(cr + 0x9d4) &&                                      // 0058C1E8..0058C20D leader only
        !perception::party_leader_00563a00(server_internal(p) + 0x1b770, p.read(cr + 4), p)   /* GetPartyTable returns the address */) return;
    if (p.call(0x635350, p.call(0x5ed8b0, client(p), {}), {})) {    // IsPartyTogether
      U mem = p.call(0x6fa7e6, 0, {0x34});                          // 0058C230..0058C315
      p.write(e + 0x58, mem);
      U ev2 = mem ? p.call(0x4d7540, mem, {}) : 0;
      U id = p.call(0x506ac0, area, {});                            // GetNextTransitionPendingID
      p.write8(e + 0x58, id);
      p.write16(ev2, 0x1e);
      p.call(0x4d7780, ev2, {0, get_obj(ev, 0, p)});
      p.call(0x4d76b0, ev2, {0, p.read8(e + 0x58)});
      p.call(0x4b08d0, ai_master(p), {0, 500, p.read(d + 4), p.read(d + 4), kSignalEvent, ev2});
      p.write(e + 0x38, 0); p.write(e + 0x3c, 0); p.write(e + 0x40, 0);
      p.call(0x62abf0, p.call(0x5ed690, client(p), {}), {0, 0, 0x3f000000u, e + 0x38});   // StartGlobalFade
      p.call(0x506b30, area, {1, p.read(e + 0x58)});
      p.call(0x4ae9a0, server(p), {2, 0});                          // SetPauseState
      return;
    }
    p.call(0x5e5a90, e + 0x10, {0x7499c4});                         // "k_trg_transfail"
    run_script(e + 0x10, p.read(cr + 4), p);
    p.call(0x5e5c20, e + 0x10, {});
    return;
  }
  U script = d + 0x288;                                             // 0058C374..0058C421 not linked: OnClick
  if ((p.call(0x5e58a0, script, {}) || p.call(0x5e5290, script, {0x73df6c})) && door_is_linked_005890d0(d, p) == 1)
    p.call(0x5e5140, script, {0x7499b0});                           // "NW_G0_Transition"
  U player = p.call(0x4ae910, server(p), {get_obj(ev, 0, p)});
  if (player && p.read16(d + 0x3ac)) {
    p.call(0x5e5a90, e + 0x18, {0x73d71c});
    p.call(0x560cd0, player, {p.read16(d + 0x3ac), e + 0x18});      // SetAreaTransitionBMP
    p.call(0x5e5c20, e + 0x18, {});
  }
  run_script(script, p.read(d + 4), p);
}
// 0058BB95..0058C436: script event subtypes (the word indexes the bytes at 0058C5C8, which select the table at 0058C594).
void signal_event(U d, U caller, U day, U ms, U ev, U e, Ports& p) {
  U id = p.read(d + 4);
  switch (p.read16(ev)) {
  case 0x00: p.write(d + 0x30c, day); p.write(d + 0x310, ms); run_script(d + 0x250, id, p); break;   // 0058BD3C heartbeat
  case 0x02: {                                                      // 0058C013 attacked
    p.write(d + 0x1d8, get_obj(ev, 0, p));
    p.write(d + 0x1dc, get_int(ev, 0, p));
    p.write(d + 0x1e0, get_int(ev, 1, p));
    if (p.read(d + 0x1e0)) {
      p.call(0x4cc0c0, d, {p.read(d + 0x1d8), 0});
      U cr = p.call(0x4ae770, server(p), {p.read(d + 0x1d8)});
      if (cr) { p.call(0x4fbbe0, cr, {kInvalidObject}); p.call(0x4f3580, cr, {2}); p.call(0x4f5f70, cr, {}); }
    }
    run_script(d + 0x268, id, p);
    break;
  }
  case 0x04: p.write(d + 0x160, caller); p.call(0x4cc0c0, d, {caller, 0}); run_script(d + 0x238, id, p); break;   // 0058BC90
  case 0x07: {                                                      // 0058BEA3 conversation
    p.write(d + 0x17c, caller);
    p.write(d + 0x198, get_int(ev, 1, p));
    U s1 = p.call(0x4d7360, ev, {e + 0x20, 0});
    p.call(0x5e5c50, d + 0x184, {s1});
    p.call(0x5e5c20, e + 0x20, {});
    U s2 = p.call(0x4d7360, ev, {e + 0x28, 0});
    p.call(0x4aefd0, server(p), {s2});
    p.call(0x5e5c20, e + 0x28, {});
    p.write(d + 0x180, get_int(ev, 2, p));
    p.call(0x4af010, server(p), {1});
    p.call(0x4af030, server(p), {get_int(ev, 3, p)});
    p.call(0x4cfa50, d, {});
    U i = 1;
    if (sgt(get_int(ev, 0, p), i)) {
      do {
        U s = p.call(0x4d7360, ev, {e + 0x30, i});
        p.call(0x4cf9b0, d, {s});
        p.call(0x5e5c20, e + 0x30, {});
        ++i;
      } while (slt(i, get_int(ev, 0, p)));
    }
    U dialog = d + 0x290;                                           // 0058BFA9..0058BFCA (no IsEmpty test here)
    if (p.call(0x5e5290, dialog, {0x73df6c})) p.call(0x5e5140, dialog, {0x7471d8});
    run_script(dialog, id, p);
    break;
  }
  case 0x0a: p.write(d + 0x154, caller); run_script(d + 0x240, id, p); break;                 // 0058BCB6
  case 0x0b: p.write(d + 0x150, get_int(ev, 0, p)); run_script(d + 0x280, id, p); break;     // 0058C09D
  case 0x18:                                                        // 0058BCD2 opened
    if (p.read(d + 0x2e8)) { p.write(d + 0x2e8, 0); p.write(d + 0x328, caller); run_script(d + 0x248, id, p); }
    if (sgt(p.read(d + 0x2e0), 0)) release_list(d, p);
    break;
  case 0x1a: {                                                      // 0058BD62 trap triggered
    U force = get_int(ev, 0, p);
    U cr = p.call(0x4ae770, server(p), {caller});
    if (cr && p.call(0x5a6960, p.read(cr + 0xa74), {5, 0})) break;
    if (!force) {
      if (sge(p.call(0x57cb80, d, {caller, 0}), 0x5a)) break;
      if (p.read(d + 0x2b8) == p.read(p.read(cr + 0xa74) + 0x78)) break;
    }
    if (!p.read(d + 0x2e8)) break;
    if (cr) leaves::send_feedback_004ede10(cr, 0x52, 0, p);
    p.write(d + 0x324, caller);
    run_script(d + 0x270, id, p);
    if (p.read(d + 0x304)) {
      p.write(d + 0x2e8, 0);
      if (p.call(0x4cb120, d, {}) && p.call(0x4cb120, d, {}) + 0x12c) {
        U oid = p.read(d + 4);
        if (p.call(0x4af240, p.call(0x4cb120, d, {}) + 0x12c, {oid}) == 1) {
          U oid2 = p.read(d + 4);
          p.call(0x66dcc0, p.call(0x4cb120, d, {}) + 0x12c, {oid2});
        }
      }
    }
    if (sgt(p.read(d + 0x2e0), 0)) release_list(d, p);
    break;
  }
  case 0x1c: p.write(d + 0x314, caller); run_script(d + 0x258, id, p); break;   // 0058BFDB
  case 0x1d: p.write(d + 0x318, caller); run_script(d + 0x278, id, p); break;   // 0058BFF7
  case 0x1e: transition(d, ev, e, p); break;
  case 0x22: {                                                      // 0058BBB3 failed to open
    p.write(d + 0x324, caller);
    run_script(d + 0x298, id, p);
    if (!p.read(d + 0x2c4)) break;
    U list = p.read(d + 0xfc);                                      // a queued open action (type 0x14) of this door: no message
    p.write(e + 0x58, p.read(list));
    for (U pos = p.read(list); pos; pos = p.read(e + 0x58)) {
      U node = p.call(0x5e9b60, p.read(d + 0xfc), {pos});
      if (node && p.read(node) == 0x14 && p.read(node + 0x38) == p.read(d + 4)) { pos = 0; goto done; }
      p.call(0x5e9b90, p.read(d + 0xfc), {e + 0x58});
    }
    {
      U cr = p.call(0x4ae770, server(p), {caller});
      if (!cr) break;
      U mem = p.call(0x6fa7e6, 0, {0x34});
      p.write(e + 0x5c, mem);
      U msg = mem ? leaves::message_data_construct_004d69f0(mem, p) : 0;
      p.call(0x4d7780, msg, {0, p.read(d + 4)});
      leaves::send_feedback_004ede10(cr, 0xd, msg, p);              // "locked"
    }
  done:
    break;
  }
  default: break;
  }
  p.call(0x4d7590, ev, {});
  p.call(0x6fa390, 0, {ev});
}
}
U door_is_linked_005890d0(U d, Ports& p) {
  U v = p.read8(d + 0x384);                                         // 005890D0..005890E6
  return (v == 1 || v == 2) ? 1u : 0u;
}
void event_handler_0058b850(U d, U code, U caller, U data, U day, U ms, U e, Ports& p) {
  // e+N == [esp+N] after `push ebp` (0058B8FD); +0x54..+0x64 are the five argument slots.
  p.write(e + 0x54, code); p.write(e + 0x58, caller); p.write(e + 0x5c, data); p.write(e + 0x60, day); p.write(e + 0x64, ms);
  if (code == kFeedbackMessage) {                                   // 0058B86F..0058B8F3 (same as the placeable)
    if (!data) return;
    U cr = p.call(0x4ae770, server(p), {caller});
    if (p.read8(data) == 1) {
      if (cr) p.call(0x4ec760, cr, {data});
    } else if (cr) {
      leaves::send_feedback_004ede10(cr, get_int(data, 0, p), data, p);
      return;
    }
    leaves::message_data_destroy_004d6a40(data, p);
    p.call(0x6fa390, 0, {data});
    return;
  }
  if (code == kDamaged) {                                           // 0058B8F6..0058BB84
    U cr = 0;
    p.write(e + 0x54, 0);
    if (data) {
      U mem = p.call(0x6fa7e6, 0, {0x34});
      p.write(e + 0x5c, mem);
      U msg = mem ? leaves::message_data_construct_004d69f0(mem, p) : 0;
      p.call(0x4d7780, msg, {0, caller});
      p.call(0x4d7780, msg, {1, p.read(d + 4)});
      if (p.global(kDebugEventStrings)) p.call(0x4d6ae0, msg, {0, data + 0x78});
      cr = p.call(0x4ae770, server(p), {caller});
      if (cr) { p.call(0x5b5e90, cr, {msg, data}); p.call(0x4f5f70, cr, {}); }
      if (msg) { leaves::message_data_destroy_004d6a40(msg, p); p.call(0x6fa390, 0, {msg}); }
      p.write16(d + 0x16c, p.read16(data + 0x64));
      p.write8(d + 0x16e, p.read8(data + 0x3b));
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
      if (slot) p.write(e + 0x54, p.call(0x5a4c20, p.read(cr + 0xa2c), {slot}));
    } else {
      cr = caller;                                                  // 0058BA69: the original reuses the caller id
    }
    p.write(d + 0x15c, caller);
    p.call(0x4cc0c0, d, {caller, 0});
    if (cr) {
      p.call(0x4fbbe0, cr, {kInvalidObject});
      p.call(0x4f5f70, cr, {});
      U weapon = p.read(e + 0x54);
      p.write(cr + 0x170, weapon ? p.read(weapon + 0x14) : kInvalidObject);
    }
    run_script(d + 0x260, p.read(d + 4), p);                        // OnDamaged
    if (p.read(d + 0x2e8) != 1 || !cr) return;
    U a[3] = {p.read(cr + 0x90), p.read(cr + 0x94), p.read(cr + 0x98)}, b[3] = {p.read(d + 0x90), p.read(d + 0x94), p.read(d + 0x98)};
    if (!x87::mine_within_trigger(a, b, 0x41800000u)) return;       // within 4 m ([007455D0] 16.0f)
    U mem = p.call(0x6fa7e6, 0, {0x34});
    p.write(e + 0x58, mem);
    U ev = mem ? p.call(0x4d7540, mem, {}) : 0;
    p.write16(ev, 0x1a);
    p.call(0x4b08d0, ai_master(p), {0, 0, caller, p.read(d + 4), kSignalEvent, ev});
    return;
  }
  if (code == kSignalEvent) { signal_event(d, caller, day, ms, data, e, p); return; }
  if (code == kTimedEvent) { if (data) p.call(0x5d0fd0, p.global(kVirtualMachine), {data, p.read(d + 4), 1}); return; }
  if (code == kOpenDoor) { p.call(0x589c70, d, {caller}); return; }   // CSWSDoor::OpenDoor
  if (code == kCloseDoor) {                                         // 0058C47F..0058C4DE
    p.write(d + 0x320, caller);
    p.call(0x589600, d, {0, 1});                                    // SetOpenState
    U linked = p.call(0x589580, d, {});                             // GetLinkedObject
    if (linked && p.vcall(linked, 0x18, {})) p.call(0x589600, p.vcall(linked, 0x18, {}), {0, 1});
    run_script(d + 0x230, p.read(d + 4), p);                        // OnClosed
    return;
  }
  if (code == kApplyEffect) { if (data) p.call(0x4d14f0, d, {data, 0, 0}); return; }
  if (code == kDestroyObject) {                                     // 0058C505..0058C557
    if (!p.read(d + 0xec)) return;
    if (!door_is_linked_005890d0(d, p)) {
      p.call(0x58b490, d, {});                                      // CSWSDoor::RemoveFromArea
      p.vcall(d, 0x00, {1});
      return;
    }
    if (p.call(0x4cb120, d, {})) p.call(0x58fec0, p.call(0x4cb120, d, {}), {p.read(d + 4)});   // RemoveStaticBoundingBox
    p.call(0x589600, d, {3, 1});                                    // SetOpenState(destroyed)
    return;
  }
  if (code == kForward7C) { if (data && !p.vcall(d, 0x94, {})) p.vcall(d, 0x7c, {data}); }
}
}
