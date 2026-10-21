#include "1568f48cf6-object_state.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
namespace w1::object_state {
U object_get_dead_004cb810(U o, Ports& p) {
  if (p.vcall(o, 0x30, {}) && p.read(p.vcall(o, 0x30, {}) + 0x9d4)) {   // 004CB813..004CB82B
    U hp = p.vcall(p.vcall(o, 0x30, {}), 0x9c, {0}) & 0xffff;
    return static_cast<short>(hp) <= -10 ? 1u : 0u;                     // cmp ax, FFF6; setle
  }
  U hp;
  if (p.vcall(o, 0x30, {})) hp = p.vcall(p.vcall(o, 0x30, {}), 0x9c, {0}) & 0xffff;   // 004CB84D..004CB865
  else hp = p.vcall(o, 0x9c, {0}) & 0xffff;                            // 004CB877
  return static_cast<short>(hp) <= 0 ? 1u : 0u;                        // test ax, ax; setle
}
U creature_get_dead_004ef820(U c, Ports& p) {
  U count = p.read(p.call(0x5ed8b0, p.read(p.global(kAppManager) + 4), {}));   // 004EF82E..004EF839
  for (U i = 0; slt(i, count); ++i) {
    U character = p.call(0x6346c0, p.call(0x5ed8b0, p.read(p.global(kAppManager) + 4), {}), {i});
    U id = p.call(0x5eda50, p.read(p.global(kAppManager) + 4), {p.read(c + 4)});
    if (character && p.read(character + 4) == id) return 0;           // 004EF86A..004EF889
  }
  return object_get_dead_004cb810(c, p);                              // 004EF87E tail call
}
U object_current_hp_004caec0(U o, U base_only, Ports& p) {
  if (base_only == 1) return p.read16(o + 0xdc);                       // 004CAEC7 mov ax, [ecx+DC]
  return p.read(o + 0xe4) + p.read(o + 0xdc);                         // 004CAED1..004CAED7
}
}
