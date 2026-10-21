#include "12e99d71d0-trigger_event.hpp"
#include "4a8ca74ca0-leaves.hpp"
#include "5ecaebb99c-perception.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// G05: 0058F140 CSWSTrigger::EventHandler (vtable +0x78).
namespace w1::trigger_event {
namespace {
constexpr U kTimedEvent = 1, kSignalEvent = 10, kDestroyObject = 11;
U client(Ports& p) { return p.read(p.global(kAppManager) + 4); }
void run_script(U script, U object_id, Ports& p) { p.call(0x5d0fc0, p.global(kVirtualMachine), {script, object_id, 1}); }
U get_int(U m, U i, Ports& p) { return leaves::message_get_integer_004d6480(m, i, p); }
U get_obj(U m, U i, Ports& p) { return leaves::message_get_object_004d64c0(m, i, p); }
// 0058F1EF..0058F525: subtype 0x0C (enter / click): area transition, mine, or OnClick script.
void on_enter(U t, U ev, U e, Ports& p) {
  U value = get_int(ev, 0, p);                                      // ebx (OnEnterMine argument)
  p.write(t + 0x29c, get_obj(ev, 0, p));
  U cr = p.call(0x4ae770, server(p), {get_obj(ev, 0, p)});
  if (!p.read(t + 0x2b4)) {                                         // 0058F4E4: not a transition
    if (p.read(t + 0x2bc)) p.call(0x58d570, t, {value});            // CSWSTrigger::OnEnterMine
    else run_script(t + 0x24c, p.read(t + 4), p);
    p.call(0x4ce8a0, t, {1});
    return;
  }
  U area = p.call(0x4cb120, t, {});
  auto cancel = [&] {                                               // 0058F4C7..0058F4DF
    if (area) p.call(0x506b30, area, {0, 0});                       // CSWSArea::SetTransitionPending
    p.call(0x4ce8a0, t, {1});
  };
  if (!cr || p.read(p.call(0x5ed690, client(p), {}) + 0xb4)) { cancel(); return; }   // no creature or GUI busy
  if (!area || !sgt(p.call(0x5e5790, t + 0x238, {}), 0)) { p.call(0x4ce8a0, t, {1}); return; }   // no destination module
  if (p.read(area + 0x2c4)) {                                       // 0058F288..0058F330: a transition is pending
    p.write8(e + 0x3c, p.read8(area + 0x2c9));
    if (get_int(ev, 1, p) != p.read8(e + 0x3c)) { p.call(0x4ce8a0, t, {1}); return; }
    if (!p.vcall(cr, 0x94, {}) && !p.call(0x4ef890, cr, {})) {
      U srv = server(p);
      p.call(0x4aecc0, srv, {1});                                   // SetMoveToModulePending
      p.call(0x4aecd0, srv, {t + 0x238});                           // SetMoveToModuleString
      p.call(0x4aed30, srv, {t + 0x230});                           // SetMoveToModuleStartWaypoint
    } else {
      p.call(0x62ac40, p.call(0x5ed690, client(p), {}), {});        // CGuiInGame::StopGlobalFade
    }
    p.call(0x506b30, area, {0, 0});
    p.call(0x4ce8a0, t, {1});
    return;
  }
  if (!p.read(cr + 0x9d4) &&                                        // 0058F335..0058F35A: only the party leader
      !perception::party_leader_00563a00(server_internal(p) + 0x1b770, p.read(cr + 4), p)   /* GetPartyTable returns the address */) {
    p.call(0x4ce8a0, t, {1});
    return;
  }
  if (p.call(0x635350, p.call(0x5ed8b0, client(p), {}), {})) {      // CSWParty::IsPartyTogether
    U mem = p.call(0x6fa7e6, 0, {0x34});                            // 0058F37D..0058F477: delayed transition event
    p.write(e + 0x3c, mem);
    U ev2 = mem ? p.call(0x4d7540, mem, {}) : 0;                     // CScriptEvent::CScriptEvent
    U id = p.call(0x506ac0, area, {});                              // CSWSArea::GetNextTransitionPendingID
    p.write8(e + 0x3c, id);
    p.write16(ev2, 0xc);
    p.call(0x4d7780, ev2, {0, get_obj(ev, 0, p)});
    p.call(0x4d76b0, ev2, {0, get_int(ev, 0, p)});
    p.call(0x4d76b0, ev2, {1, p.read8(e + 0x3c)});
    p.call(0x4b08d0, ai_master(p), {0, 500, p.read(t + 4), p.read(t + 4), kSignalEvent, ev2});   // AddEventDeltaTime
    p.write(e + 0x10, 0); p.write(e + 0x14, 0); p.write(e + 0x18, 0);
    p.call(0x62abf0, p.call(0x5ed690, client(p), {}), {0, 0, 0x3f000000u, e + 0x10});   // StartGlobalFade(0, 0, 0.5f, &black)
    p.call(0x506b30, area, {1, p.read(e + 0x3c)});                  // SetTransitionPending(1, id slot)
    p.call(0x4ae9a0, server(p), {2, 0});                            // SetPauseState
    p.call(0x4ce8a0, t, {1});
    return;
  }
  p.call(0x5e5a90, e + 0x10, {0x7499c4});                           // 0058F47C..0058F4C2: "k_trg_transfail"
  run_script(e + 0x10, p.read(cr + 4), p);
  p.call(0x5e5c20, e + 0x10, {});
  p.call(0x4ce8a0, t, {1});
}
}
void event_handler_0058f140(U t, U code, U caller, U data, U e, Ports& p) {
  // e+N == [esp+N] after `push edi` (0058F1E8); +0x34/+0x38/+0x3C are the code, caller and data argument slots.
  p.write(e + 0x34, code); p.write(e + 0x38, caller); p.write(e + 0x3c, data);
  if (code == kTimedEvent) {                                        // 0058F15F..0058F194
    if (data) p.call(0x5d0fd0, p.global(kVirtualMachine), {data, p.read(t + 4), 1});   // RunScriptSituation (W6)
    return;
  }
  if (code == kDestroyObject) {                                     // 0058F197..0058F1CB
    if (!p.read(t + 0xec)) return;
    p.call(0x58ee30, t, {});                                        // CSWSTrigger::RemoveFromArea
    p.vcall(t, 0x00, {1});
    return;
  }
  if (code != kSignalEvent) return;
  U ev = data;
  switch (p.read16(ev)) {
  case 0x0c: on_enter(t, ev, e, p); break;
  case 0x0d:                                                        // 0058F530
    p.write(t + 0x2a0, get_obj(ev, 0, p));
    run_script(t + 0x254, p.read(t + 4), p);
    break;
  case 0x0b:                                                        // 0058F557: script runs as the caller
    p.write(t + 0x2f8, get_int(ev, 0, p));
    run_script(t + 0x25c, caller, p);
    break;
  case 0x18:                                                        // 0058F57F
    p.write(t + 0x2a4, caller);
    run_script(t + 0x26c, p.read(t + 4), p);
    break;
  case 0x1e: {                                                      // 0058F5A5..0058F65E transition click
    p.write(t + 0x29c, get_obj(ev, 0, p));
    U script = t + 0x274;
    bool fallback = p.call(0x5e58a0, script, {}) || p.call(0x5e5290, script, {0x73df6c});   // IsEmpty || == "default"
    if (fallback && p.read(t + 0x2b4) == 1) p.call(0x5e5140, script, {0x7499b0});   // = "NW_G0_Transition"
    U player = p.call(0x4ae910, server(p), {get_obj(ev, 0, p)});
    if (player && p.read16(t + 0x308)) {
      p.call(0x5e5a90, e + 0x1c, {0x73d71c});                       // CExoString("")
      p.call(0x560cd0, player, {p.read16(t + 0x308), e + 0x1c});    // CSWSPlayer::SetAreaTransitionBMP
      p.call(0x5e5c20, e + 0x1c, {});
    }
    run_script(script, p.read(t + 4), p);
    break;
  }
  default: break;
  }
  p.call(0x4d7590, ev, {});                                         // 0058F663 ~CScriptEvent, free
  p.call(0x6fa390, 0, {ev});
}
}
