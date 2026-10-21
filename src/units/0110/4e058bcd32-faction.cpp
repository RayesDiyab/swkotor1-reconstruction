#include "9b6e07d27f-faction.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// Reputation: 0057CB80 CSWSObject::GetReputation and the faction leaves it reaches.
namespace w1::faction {
namespace {
constexpr U kNeutral = 0x32, kFriendly = 0x64;
constexpr U kReputationOverride = 0x7a2068;   // .data dword, -1 unless a debug command sets it
constexpr U kTypeAreaOfEffect = 11;           // [0074663A]
U object_array(Ports& p) { return p.read(server_internal(p) + 0x1005c); }
bool lookup(U id, U slot, Ports& p) { return (p.call(0x4d8230, object_array(p), {id, slot}) & 0xff) == 0; }
}
U is_npc_faction_0052b280(U mgr, U f, Ports& p) { return sge(f, 1) && sle(f, p.read(mgr + 0x1c)) ? 1u : 0u; }
U npc_faction_reputation_0052b420(U mgr, U a, U b, Ports& p) {
  for (;;) {                                                        // 0052B430..0052B460: put the valid faction first
    U n = p.read(mgr + 0x1c);
    bool a_ok = sge(a, 1) && sle(a, n), b_ok = sge(b, 1) && sle(b, n);
    if (!a_ok && !b_ok) return kNeutral;                            // 0052B43C..0052B446
    if (!a_ok && b_ok) { U t = a; a = b; b = t; continue; }         // 0052B45A..0052B460 swap
    break;
  }
  S idx = static_cast<S>(p.read(mgr + 0x1c) * b + a - 1);           // 0052B462..0052B468
  U table = p.read(mgr + 0x20);
  if (idx < 0 || !slt(static_cast<U>(idx), p.read(table + 4))) return kNeutral;
  return p.read(p.read(table) + static_cast<U>(idx) * 4);
}
U creature_reputation_00513ff0(U c, U other, U other_faction, Ports& p) {
  U own = p.read(p.read(c + 0xa74) + 0x78);
  U cr = p.call(0x4ae770, server(p), {other});
  U party = p.read(c + 0xa88);
  if (party && cr && p.read(cr + 0xa88)) return kFriendly;          // both party members
  if (party) own = 0;                                               // 00514038..0051403C
  if (cr && p.read(cr + 0xa88)) other_faction = 0;                  // 0051403E..0051404C
  return npc_faction_reputation_0052b420(p.read(server_internal(p) + 0x10054), other_faction, own, p);
}
U get_reputation_0057cb80(U o, U target, U arg2, U e, Ports& p) {
  // e+0x10 target faction, e+0x14 own faction, e+0x18 lookup slot, e+0x20 the target argument slot (reused for the object)
  U override = p.global(kReputationOverride);
  if (override != 0xffffffffu) return override;                     // 0057CB80..0057CB8E
  p.write(e + 0x20, target); p.write(e + 0x24, arg2);
  if (!lookup(target, e + 0x20, p)) return kNeutral;
  U mgr = p.read(server_internal(p) + 0x10054);                     // CFactionManager
  if (!mgr) return kNeutral;
  U t = p.read(e + 0x20);
  U subject = target;
  if (p.read8(t + 8) == kTypeAreaOfEffect && p.vcall(t, 0x50, {})) subject = p.read(p.vcall(t, 0x50, {}) + 0x248);   // AoE: its creator
  if (p.vcall(o, 0x50, {})) {                                       // 0057CC03..0057CC58 this is an AoE: ask its creator
    U creator = p.read(p.vcall(o, 0x50, {}) + 0x248);
    if (!lookup(creator, e + 0x18, p)) return kNeutral;
    return get_reputation_0057cb80(p.vcall(p.read(e + 0x18), 0x10, {}), subject, p.read(e + 0x24), e + 0x28, p);
  }
  if (!p.call(0x4ae950, server(p), {subject, e + 0x10})) return kNeutral;          // GetFactionOfObject
  if (!p.call(0x4ae950, server(p), {p.read(o + 4), e + 0x14})) return kNeutral;
  if (subject == p.read(o + 4)) return kFriendly;
  if (!is_npc_faction_0052b280(mgr, p.read(e + 0x10), p) && is_npc_faction_0052b280(mgr, p.read(e + 0x14), p) == 1)   // 0057CCB0..0057CCE2
    return get_reputation_0057cb80(p.vcall(p.read(e + 0x20), 0x10, {}), p.read(o + 4), 0, e + 0x28, p);
  U rep;
  if (p.vcall(o, 0x30, {})) {
    rep = creature_reputation_00513ff0(p.vcall(o, 0x30, {}), subject, p.read(e + 0x10), p);
  } else if (p.vcall(o, 0x48, {}) || p.vcall(o, 0x5c, {}) || p.vcall(o, 0x18, {}) || p.vcall(o, 0x40, {})) {
    rep = npc_faction_reputation_0052b420(mgr, p.read(e + 0x10), p.read(e + 0x14), p);
  } else {
    return 0;                                                       // 0057CD6D with ebx = 0
  }
  if (slt(rep, 0)) return 0;                                        // 0057CD51..0057CD68 clamp to 0..100
  return sgt(rep, kFriendly) ? kFriendly : rep;
}
}
