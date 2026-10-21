#include "5ecaebb99c-perception.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// 00501950 CSWSCreature::GetInvisible, 00503690 CGameEffect::GetInteger, 0057CD90 GetAIStateReputation,
// 004FDA20 CSWSCreature::PacifyCreature.
namespace w1::perception {
namespace {
constexpr U kEffectInvisibility = 47;   // [00746738]
constexpr U kEffectLast = 63;           // [00746778] (also the sanctuary-style effect checked below)
}
U effect_integer_00503690(U effect, U index, Ports& p) { return p.read(p.read(effect + 0x34) + index * 4); }
U ai_state_reputation_0057cd90(U object, U other_id, Ports& p, U scratch) {
  p.call(0x4cb120, object, {});                                     // 0057CD94 GetArea (result unused)
  U array = p.read(server_internal(p) + 0x1005c);                   // GetObjectArray (inline accessor 004AED70)
  p.call(0x4d8230, array, {other_id, scratch});                     // CGameObjectArray::GetGameObject(id, &out)
  U other = p.read(scratch);
  if (!other || !p.vcall(other, 0x10, {})) return 0;                // 0057CDB7..0057CDC6
  U self_id = p.read(object + 4);
  U as_object = p.vcall(p.read(scratch), 0x10, {});                 // 0057CDD1..0057CDD4 (push 0 stays for GetReputation)
  U rep = p.call(0x57cb80, as_object, {self_id, 0});                // CSWSObject::GetReputation(id, 0)
  if (sge(rep, 0x5a)) return 1;                                     // 0057CDDE..0057CDE3
  if (sle(rep, 0xa)) return 2;                                      // 0057CDEA..0057CDEF
  return 0;
}
void pacify_004fda20(U c, U other_id, Ports& p) {
  U other = p.call(0x4ae770, server(p), {other_id});                // 004FDA31
  if (!other) return;
  U id = p.read(c + 4);
  if (p.read(other + 0x50c) != id && p.read(other + 0x504) != id && p.read(other + 0x524) != id &&
      p.read(other + 0x528) != id && p.read(other + 0x158) != id) return; // 004FDA3C..004FDA65
  p.call(0x4f2610, other, {0, 1});                                  // SetCombatState(0, 1)
  if (p.read(other + 0x158) == p.read(c + 4)) p.call(0x4cc0c0, other, {kInvalidObject, 1}); // SetLastHostileActor
  p.call(0x4f61d0, other, {c});                                     // ClearHostileActionsVersus(this)
}
U invisible_00501950(U c, U viewer_object, U scratch, Ports& p) {
  if (!p.vcall(viewer_object, 0x30, {})) return 0;                  // 0050195F..00501966
  U viewer = p.vcall(viewer_object, 0x30, {});                      // AsSWSCreature
  U vstats = p.read(viewer + 0xa74);
  const U match = p.read16(vstats + 0xdc);                          // ebx, saved at [esp+14] and restored per iteration (005019B2)
  U target_match = static_cast<U>(static_cast<S>(static_cast<std::int16_t>(p.read16(vstats + 0x17e)))); // [esp+1c]
  U i = p.read16(p.read(c + 0xa74) + 0x140);                        // first effect index of interest
  if (!slt(i, p.read(c + 0x128))) return 0;                         // 00501996..005019A4
  for (;;) {
    U effect = p.read(p.read(c + 0x124) + i * 4);
    U type = p.read16(effect + 8);
    if (sgt(type, kEffectLast)) return 0;                           // 005019CD..005019D5
    auto source_matches = [&]() {
      if (match != effect_integer_00503690(effect, 1, p) &&
          p.read8(p.global(kRules) + 0xaa) != effect_integer_00503690(effect, 1, p)) return false;
      if (target_match != effect_integer_00503690(effect, 3, p) && effect_integer_00503690(effect, 3, p)) return false;
      return true;
    };
    if (type == kEffectInvisibility) {                              // 005019D7..00501A80
      if (source_matches()) {
        U kind = effect_integer_00503690(effect, 0, p);
        if (kind == 1 || effect_integer_00503690(effect, 0, p) == 4)
          return (p.read8(viewer + 0x8ec) & 5) ? 0 : 1;             // 00501BA9
        if (effect_integer_00503690(effect, 0, p) != 2) return 1;
        return (p.read8(viewer + 0x8ec) & 6) ? 0 : 1;
      }
    } else if (type == kEffectLast) {                               // 00501A83..00501B81
      if (ai_state_reputation_0057cd90(viewer, p.read(c + 4), p, scratch) == 2 && source_matches()) {
        U elem = visible_element_00517a20(viewer, p.read(c + 4), p);
        U state = elem ? (p.read8(elem + 4) >> 2) & 3 : 0;
        if (elem && state == 1) return ((~p.read8(viewer + 0x8ec) & 0xff) >> 2) & 1;  // 00501BC5..00501BDA
        if (elem && state == 2) return 0;
        if (!elem || state == 0) {
          U dc = effect_integer_00503690(effect, 4, p);
          U saved = p.call(0x5b92b0, viewer, {3, dc, 0, kInvalidObject, 1, 0, 0}) & 0xff; // SavingThrowRoll
          if (!saved) {
            pacify_004fda20(c, p.read(viewer + 4), p);              // 00501BE0..00501BF9
            add_visible_0051b370(viewer, p.read(c + 4), 0, 0, 1, 0, p);
            return 1;
          }
          add_visible_0051b370(viewer, p.read(c + 4), 0, 0, 2, 0, p);
        }
      }
    }
    ++i;                                                            // 00501B86..00501B97
    if (!slt(i, p.read(c + 0x128))) return 0;
  }
}
}
