#include "2e1ef3b500-server_queries.hpp"
#include "1a847f7684-w1_x87.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
namespace w1::server_queries {
namespace { constexpr U kHealthThreshold = 0x7a1b38; }   // .data float 0.2f (0963 cell recovered_health_threshold_007a1b38)
U server_player_list_004aed90(U a, Ports& p) { return p.read(p.read(a + 4) + 0x10064); }   // 004AED90
U server_info_004aec90(U a, Ports& p) { return p.read(p.read(a + 4) + 0x10004); }          // 004AEC90
U server_is_injured_004aee40(U a, U client_id, U scratch, Ports& p) {
  U id = p.call(0x4b1f70, p.read(a + 4), {client_id});            // CServerExoAppInternal::ClientToServerObjectId
  return internal_is_injured_004b1d20(p.read(a + 4), id, scratch, p);
}
U internal_is_injured_004b1d20(U internal, U id, U scratch, Ports& p) {
  p.write(scratch, 0);
  if ((p.call(0x4d8230, p.read(internal + 0x1005c), {id, scratch}) & 0xff) != 0) return 0;   // 004B1D39..004B1D44
  U o = p.read(scratch);
  if (!o || p.read8(o + 8) != 5) return 0;                         // creature
  U c = p.vcall(o, 0x30, {});
  return c ? creature_is_injured_004eff30(c, p) : 0u;
}
U creature_is_injured_004eff30(U c, Ports& p) {
  if (p.read(c + 0xac4)) return 0;                                 // 004EFF36..004EFF3E
  S cur = static_cast<short>(p.vcall(c, 0x9c, {0}) & 0xffff);      // current hit points (movsx ax)
  S max = static_cast<short>(p.vcall(c, 0x98, {1}) & 0xffff);      // maximum
  return x87::hp_ratio_below(cur, max, p.global(kHealthThreshold)) ? 1u : 0u;   // ratio < 0.2
}
void object_delete_dialog_004cc330(U o, Ports& p) {
  if (U d = p.read(o + 0x34)) { p.vcall(d, 0x00, {1}); p.write(o + 0x34, 0); }   // scalar deleting destructor
  p.write(o + 0x214, 0);
}
U creature_following_leader_004f7750(U c, Ports& p) {
  U list = p.read(c + 0xfc);
  U head = p.read(list);
  if (!head) return 0;
  U node = p.call(0x5e9b60, list, {head});                         // GetAtPos
  return node && p.read(node) == 0x3d ? 1u : 0u;                   // ACTION_FOLLOWLEADER
}
}
