#include "8512140f7c-combat_state.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// Tier 9: 004F2610 CSWSCreature::SetCombatState, 004D3770 CSWSCombatRound::RemoveAllActions, and the CSWSRules
// constructor / destructors (0057A1F0, 0057A210, 0057A220).
namespace w1::combat_state {
namespace {
constexpr U kRulesServerVtable = 0x7493e0;   // CSWSRules vtable (one slot: 0057A220); bound to W1's reconstructed table
constexpr U kFactionNoCombat = 5;            // 004F2637: faction +0xC == 5 never enters combat
}
void combat_round_remove_all_actions_004d3770(U round, Ports& p) {
  U list = p.read(round + 0x9b0);
  if (!list) return;                                                // 004D3779
  while (p.read(p.read(list))) {                                    // 004D377D..004D37A3: head position non-null
    U node = p.call(0x5e9c80, p.read(list), {});                     // CExoLinkedListInternal::RemoveHead
    if (node) p.call(0x6fa390, 0, {node});
    list = p.read(round + 0x9b0);
  }
}
void set_combat_state_004f2610(U c, U state, U flag, U e, Ports& p) {
  // e: +0x20 / +0x28 the two CExoStrings of the debug text block.
  U faction = p.call(0x513fc0, c, {});                              // CSWSCreature::GetFaction
  if (p.read(faction + 0xc) == kFactionNoCombat) state = 0;         // 004F2632..004F263E (the argument slot is zeroed too)
  U flag8 = flag & 0xff;
  if (p.read(c + 0x4e0) != state && state == 1 && flag8 == 1 && !p.read(c + 0xa88) && !p.read(c + 0x9d4)) {
    S r = static_cast<S>(p.call(0x6fc447, 0, {}));                  // rand() % 10 < 3 (idiv, signed)
    if (r % 10 < 3) p.call(0x4ec8e0, c, {});                         // CSWSCreature::BroadcastBattleCry
  }
  p.write(c + 0x4e0, state);
  if (state) {                                                      // 004F268F..004F26B0
    p.write(c + 0x4e4, 0x1f40);
    if (p.read8(c + 0xac0) != 1) p.write8(c + 0xac0, flag8);
  } else {                                                          // 004F26B5..004F2862: leaving combat
    p.write(c + 0x4e4, 0); p.write8(c + 0xac0, 0); p.write(c + 0x4dc, 1);
    p.write(c + 0x540, 0xffffffffu); p.write(c + 0x544, 0xffffffffu); p.write(c + 0x53c, kInvalidObject);
    p.call(0x4cc0c0, c, {kInvalidObject, 1});                       // CSWSObject::SetLastHostileActor
    p.write(c + 0x548, 0xffffffffu); p.write(c + 0x54c, 0xffffffffu); p.write(c + 0x550, 0); p.write(c + 0x554, 0);
    p.call(0x4d37e0, p.read(c + 0x9c8), {});                        // CSWSCombatRound::ClearAllAttacks
    p.call(0x4d3770, p.read(c + 0x9c8), {});                        // CSWSCombatRound::RemoveAllActions
    U round = p.read(c + 0x9c8);
    p.write(c + 0x50c, kInvalidObject); p.write(c + 0x504, kInvalidObject); p.write(c + 0x524, kInvalidObject); p.write(c + 0x528, kInvalidObject);
    p.write(round + 0x9cc, kInvalidObject);
    round = p.read(c + 0x9c8);
    if (p.read(round + 0x934)) {                                    // 004F2749..004F2860: round still active
      p.call(0x5b3190, e + 0x28, {});                               // CExoString (debug text)
      p.call(0x5b3190, e + 0x20, {});                               // CExoString (target name)
      U target = p.call(0x4ae750, server(p), {p.read(round + 0x9c0)});
      if (target && p.vcall(target, 0x10, {})) {
        p.call(0x5e5c50, e + 0x20, {p.vcall(target, 0x10, {}) + 0x18});
        if (p.call(0x5e58a0, e + 0x20, {})) p.call(0x5e5140, e + 0x20, {0x73ee04});   // "NULL"
      }
      U a9bc = p.read(round + 0x9bc), a9b8 = p.read(round + 0x9b8), a94c = p.read(round + 0x94c), a9c0 = p.read(round + 0x9c0), a944 = p.read(round + 0x944);
      U name = p.call(0x5e5670, e + 0x20, {});
      U tag = p.call(0x5e5670, c + 0x18, {});
      // CExoString::Format(&text, "WARNING:  %x %s %d %d %d %d %x %s %x", ...); the text is never used (debug build remnant)
      p.call(0x5e5680, 0, {e + 0x28, 0x747000, a9bc, tag, a944, p.read(c + 4), a94c, a9b8, a9c0, name, target});
      p.call(0x4d4620, round, {0});                                 // CSWSCombatRound::EndCombatRound
      p.call(0x5e5c20, e + 0x20, {}); p.call(0x5e5c20, e + 0x28, {});
    }
  }
  // 004F2867..004F28AB: the client creature mirrors the state when the flag byte is 1
  if (U co = p.call(0x4cc2b0, c, {}); co && p.vcall(co, 0x2c, {}) && flag8 == 1) {
    U cc = p.call(0x4cc2b0, c, {}) ? p.vcall(p.call(0x4cc2b0, c, {}), 0x2c, {}) : 0;
    p.call(0x6182a0, cc, {state});                                  // CSWCCreature::SetCombatState
  }
}
U rules_server_construct_0057a1f0(U self, Ports& p) {
  p.call(0x552c50, self, {});                                       // CSWRules::CSWRules (0423)
  p.write(self, p.global(kRulesServerVtable));                      // 0057A1F8
  return self;
}
void rules_server_destroy_0057a210(U self, Ports& p) {
  p.write(self, p.global(kRulesServerVtable));                      // 0057A210
  p.call(0x551440, self, {});                                       // CSWRules::~CSWRules (0430), tail jump
}
U rules_server_scalar_destroy_0057a220(U self, U flags, Ports& p) {
  rules_server_destroy_0057a210(self, p);
  if (flags & 1) p.call(0x6fa390, 0, {self});
  return self;
}
}
