#include "e5205c2ce5-effect_handlers.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
namespace w1::effect_handlers {
U on_apply_assured_hit_004d83a0(U, U o, U effect, U, Ports& p) {
  if (!o || !p.vcall(o, 0x30, {})) return 1;                        // 004D83A5..004D83B8: creatures only (AsSWSCreature)
  U c = p.vcall(o, 0x30, {});                                       // 004D83BA..004D83BE (called again)
  if (!effect || p.read(c + 0x8d4)) return 1;                       // 004D83C5..004D83D1: already assured -> not applied
  p.write(c + 0x8d4, 1);                                            // 004D83D3 m_bAssuredHit
  return 0;
}
U on_remove_assured_hit_004d83f0(U, U o, U, Ports& p) {
  if (o && p.vcall(o, 0x30, {})) p.write(p.vcall(o, 0x30, {}) + 0x8d4, 0);   // 004D83F5..004D840B
  return 1;
}
}
