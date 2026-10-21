#include "386b2d6488-object_ai.hpp"
#include "fde3237102-effects.hpp"
#include "65dd6aca7d-run_actions.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// G05: CSWSPlaceable::AIUpdate 005849D0, CSWSDoor::AIUpdate 005889C0, CSWSTrigger::AIUpdate 0058D760 (vtable +0x70).
namespace w1::object_ai {
namespace {
// Shared head: high-resolution start time, world time now (e+0x10 day, e+0xC ms), elapsed since the heartbeat stamp
// (e+0x14 ms, e+0x18 days).
void begin(U o, U stamp_day, U stamp_ms, U e, U& lo, U& hi, Ports& p) {
  std::uint64_t t = p.call64(0x5e9260, p.read(p.global(0x7a39e0) + 4), {});   // CExoTimers::GetHighResolutionTimer
  lo = static_cast<U>(t); hi = static_cast<U>(t >> 32);
  p.call(0x4ade40, p.call(0x4aede0, server(p), {}), {e + 0x10, e + 0xc});     // GetWorldTime(day*, ms*)
  p.call(0x4ae460, p.call(0x4aede0, server(p), {}),
         {p.read(e + 0x10), p.read(e + 0xc), p.read(o + stamp_day), p.read(o + stamp_ms), e + 0x18, e + 0x14});
}
// Placeable / door body (identical code, different offsets).
void heartbeat_object(U o, U stamp_day, U stamp_ms, U script, U e, Ports& p) {
  U lo, hi;
  begin(o, stamp_day, stamp_ms, e, lo, hi, p);
  if (p.read(e + 0x14) >= 0x1770) {                                 // 6000 ms
    if (p.read(o + stamp_ms) && !p.vcall(o, 0x94, {}))              // stamp set and not dead
      p.call(0x5d0fc0, p.global(kVirtualMachine), {o + script, p.read(o + 4), 1});   // OnHeartbeat
    p.write(o + stamp_day, p.read(e + 0x10));
    p.write(o + stamp_ms, p.read(e + 0xc));
  }
  effects::update_effect_list_004d1730(o, p.read(e + 0x10), p.read(e + 0xc), e + 0x1c, p);
  run_actions::run_actions_0057f4a0(o, p.read(e + 0x10), p.read(e + 0xc), lo, hi, e + 0x70, p);
}
}
void placeable_ai_update_005849d0(U o, U e, Ports& p) { heartbeat_object(o, 0x348, 0x34c, 0x2b4, e, p); }   // 005849D0..00584AB7
void door_ai_update_005889c0(U o, U e, Ports& p) { heartbeat_object(o, 0x30c, 0x310, 0x250, e, p); }        // 005889C0..00588AA7
void trigger_ai_update_0058d760(U t, U e, Ports& p) {
  U lo, hi;
  begin(t, 0x304, 0x300, e, lo, hi, p);                             // 0058D760..0058D7CA (stamp day +0x304, ms +0x300)
  if (p.read(e + 0x14) >= 0x1770) {                                 // 0058D7CF..0058D7FF: heartbeat, unconditional
    p.call(0x5d0fc0, p.global(kVirtualMachine), {t + 0x244, p.read(t + 4), 1});
    p.write(t + 0x304, p.read(e + 0x10));
    p.write(t + 0x300, p.read(e + 0xc));
  }
  if (p.read(t + 0x2bc)) {                                          // 0058D805..0058D8A4 trap visibility state for the player
    U pc = p.call(0x4ae770, server(p), {p.call(0x4aea40, server(p), {})});   // GetCreatureByGameObjectID(GetPlayerCreatureId())
    bool detected = p.read(t + 0x2c8) != 0;
    if (!detected && pc) {
      detected = p.read(t + 0x2b8) == p.read(p.read(pc + 0xa74) + 0x78) ||          // same faction
                 sge(p.call(0x57cb80, t, {p.read(pc + 4), 0}), 0x5a) ||          // GetReputation >= 90 (friendly)
                 p.call(0x4af240, t + 0x2a8, {p.read(pc + 4)}) != 0;            // already in the detected list
    }
    U state = detected ? 0x27a0u : 0x279fu;
    if (p.read(t + 0xd4) != state) p.vcall(t, 0x7c, {state});
  }
  run_actions::run_actions_0057f4a0(t, p.read(e + 0x10), p.read(e + 0xc), lo, hi, e + 0x70, p);   // 0058D8A5..0058D8B3
}
}
