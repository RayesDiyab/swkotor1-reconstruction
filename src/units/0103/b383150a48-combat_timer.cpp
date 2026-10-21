#include "e556d6f3cd-combat_timer.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// 004FAF20 CSWSCreature::UpdateCombatRoundTimer, 004D4C10 CSWSCombatRound::IncrementTimer, 004D2B50 GetAttack.
namespace w1::combat_timer {
U round_attack_004d2b50(U round, U index) {
  if (slt(index, 0)) index = 0;                                     // 004D2B54..004D2B58
  return round + index * 0x14c + 4;
}
void increment_timer_004d4c10(U r, U dt, U scratch, Ports& p) {
  U timer = p.read(r + 0x944) + dt;                                 // 004D4C2F..004D4C37
  p.write(r + 0x944, timer);
  U active = p.read(r + 0x934);
  if (active) {
    if (!slt(timer, p.read(r + 0x94c)) && p.read(r + 0x938)) {      // 004D4C4D..004D4C5D
      U head = p.read(p.read(p.read(r + 0x9b4) + 0xfc));
      if (head && p.read(head + 8) && p.read(p.read(head + 8)) == 0xf) return; // current action is a spell cast
      p.call(0x4d4620, r, {1});                                     // EndCombatRound(1)
      return;
    }
    bool end = false;
    if (!slt(timer, p.read(r + 0x94c))) {                           // 004D4CAD..004D4CD7
      U master = p.read(r + 0x9b8);
      if (!(master && !p.read(r + 0x9bc) && master)) {               // (the original re-tests [9B8])
        if (!p.read(r + 0x938)) end = true;
      }
    }
    if (end) { p.call(0x4d4620, r, {1}); return; }                  // 004D4CD7 -> 004D4C8A
    if (!slt(timer, p.read(r + 0x94c)) && p.read(r + 0x9b8) && !p.read(r + 0x9bc)) {   // 004D4CD9..004D4D03
      p.call(0x5b3190, scratch, {});                                // debug CExoString (discarded)
      U other = p.call(0x4ae750, server(p), {p.read(r + 0x9c0)});
      if (!other)
        p.call(0x5e5680, scratch, {0x7464e0, p.call(0x5e5670, p.read(r + 0x9b4) + 0x18, {})}); // CExoString::Format
      else
        p.call(0x5e5680, scratch, {0x746478, p.call(0x5e5670, p.read(r + 0x9b4) + 0x18, {}), p.read(other + 4),
                                   p.read(r + 0x944), p.read(r + 0x94c)});
      p.write(r + 0x9c0, kInvalidObject);
      p.call(0x4d4620, r, {1});
      p.call(0x5e5c20, scratch, {});
      return;
    }
  }
  if (slt(p.read(r + 0x944), 0)) {                                  // 004D4DBF..004D4E25 timer overflow
    p.call(0x5b3190, scratch + 8, {});
    p.call(0x5e5680, scratch + 8, {0x746418, p.call(0x5e5670, p.read(r + 0x9b4) + 0x18, {}), p.read(r + 0x944)});
    p.call(0x4d4620, r, {1});
    p.call(0x5e5c20, scratch + 8, {});
    return;
  }
  if (p.read(r + 0x9b8) && !p.read(r + 0x9bc) && p.read(r + 0x9c0) == p.read(p.read(r + 0x9b4) + 4)) { // 004D4E28..004D4E57
    p.write(r + 0x9b8, 0);
    p.write(r + 0x9c0, kInvalidObject);
  }
}
void update_combat_round_timer_004faf20(U c, U scratch, Ports& p) {
  if (!p.read(c + 0x9c8)) return;                                   // 004FAF25..004FAF2D
  if (p.call(0x4ae980, server(p), {2})) return;                     // GetPauseState(2)
  U r = p.read(c + 0x9c8);
  if (!p.read(r + 0x958)) { increment_timer_004d4c10(r, p.read(c + 0xcc), scratch, p); return; } // 004FB0D3
  if (p.read(r + 0x964)) { p.call(0x4d4e80, r, {p.read(c + 0xcc)}); return; }                 // 004FB0C3 DecrementPauseTimer
  U attack = round_attack_004d2b50(r, p.read8(r + 0x96c));
  S left = static_cast<S>(p.read(p.read(c + 0x9c8) + 0x960) - p.read(c + 0xcc));               // 004FAF7F..004FAF8D
  if (left > 0) {                                                   // 004FAF93..004FAFDE
    p.call(0x4d4e80, p.read(c + 0x9c8), {p.read(c + 0xcc)});
    if (!sgt(p.read(attack + 0xb0), 0)) return;
    if (!p.call(0x4d45e0, attack, {p.read(p.read(c + 0x9c8) + 0x960)})) return;  // IsSubAttackReady
    p.call(0x4d45b0, attack, {});                                   // ResolveTopSubAttackToAttack
    p.call(0x5b8050, c, {p.read(attack + 0x40), p.read(attack + 0xc)});          // SignalDamage
    p.call(0x4d32d0, attack, {});                                   // RemoveTopSubAttack
    return;
  }
  while (sgt(p.read(attack + 0xb0), 0)) {                           // 004FAFE3..004FB015
    p.call(0x4d45b0, attack, {});
    p.call(0x5b8050, c, {p.read(attack + 0x40), p.read(attack + 0xc)});
    p.call(0x4d32d0, attack, {});
  }
  U overflow = static_cast<U>(-left);                               // neg ebx
  p.call(0x4f1250, c, {overflow});                                  // ResetCombatAnimations
  U r2 = p.read(c + 0x9c8);
  if (p.read(r2 + 0x9bc)) {
    U partner = p.call(0x4ae770, server(p), {p.read(r2 + 0x9c4)});
    if (partner) p.call(0x4f1250, partner, {overflow});
  }
  U target = p.call(0x4ae750, server(p), {p.read(p.read(c + 0x9c8) + 0x9cc)});     // 004FB052..004FB06D
  if (!target || !p.vcall(target, 0x10, {})) return;
  U object = p.vcall(target, 0x10, {});
  if (!p.vcall(object, 0x94, {})) {                                 // not dead
    if (!p.vcall(object, 0x30, {})) return;
    if (!p.call(0x4ef890, p.vcall(object, 0x30, {}), {})) return;   // GetDeadTemp
  }
  p.call(0x4d4620, p.read(c + 0x9c8), {1});                         // EndCombatRound(1)
}
}
