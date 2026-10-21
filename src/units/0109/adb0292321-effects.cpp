#include "fde3237102-effects.hpp"
#include "1a847f7684-w1_x87.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// 004D1730 CSWSObject::UpdateEffectList, 004D06C0 RemoveEffectById, 00503590 ~CGameEffect, 004CF3F0 RemoveEffectTarget,
// 0066DCC0 CExoArrayList<int>::remove, 005036C0/005036D0 Get/SetFloat, 00503770 GetExpiryTime.
namespace w1::effects {
namespace {
constexpr U kEffectRegenerate = 7;      // [00746698]
constexpr U kEffectPoison = 35;         // [00746708]
constexpr U kEffectDisease = 5;         // [00746690]
constexpr U kEffectHeal = 39;           // [00746718] (type of the effect created by a regeneration tick)
constexpr U kDurationInstant = 0;       // WORD [007473B8]
constexpr U kDurationTemporary = 1;     // WORD [007473C0]
constexpr U kEventPoisonExpired = 14;   // [007441EC]
U gi(U e, U i, Ports& p) { return p.read(p.read(e + 0x34) + i * 4); }   // 00503690 CGameEffect::GetInteger
void si(U e, U i, U v, Ports& p) { p.call(0x5036a0, e, {i, v}); }       // 005036A0 CGameEffect::SetInteger (linked)
}
U effect_float_005036c0(U e, U i, Ports& p) { return p.read(e + i * 4 + 0x38); }
void effect_set_float_005036d0(U e, U i, U v, Ports& p) { p.write(e + i * 4 + 0x38, x87::x80_to_float(x87::x80_from_float(v))); }
void effect_expiry_00503770(U e, U day_out, U ms_out, Ports& p) {
  p.write(day_out, p.read(e + 0x10));
  p.write(ms_out, p.read(e + 0x14));
}
void int_list_remove_0066dcc0(U list, U value, Ports& p) {
  U count = p.read(list + 4), i = 0;
  if (sgt(count, 0)) {                                              // 0066DCC6..0066DCDF
    for (U cell = p.read(list); p.read(cell) != value; cell += 4) {
      ++i;
      if (!slt(i, p.read(list + 4))) break;
    }
  }
  U last = count - 1;                                               // 0066DCE3..0066DCE8 (count read before the scan)
  p.write(list + 4, last);
  for (; slt(i, last); ) {                                          // 0066DCF0..0066DD01
    U data = p.read(list);
    p.write(data + i * 4, p.read(data + i * 4 + 4));
    ++i;
    if (!slt(i, p.read(list + 4))) break;
  }
}
void effect_destroy_00503590(U e, Ports& p) {
  if (U linked = p.read(e + 0x28)) {                                // 005035AF..005035CC
    effect_destroy_00503590(linked, p);
    p.call(0x6fa390, 0, {linked});
    p.write(e + 0x28, 0);
  }
  if (U linked = p.read(e + 0x2c)) {                                // 005035CF..005035E6
    effect_destroy_00503590(linked, p);
    p.call(0x6fa390, 0, {linked});
    p.write(e + 0x2c, 0);
  }
  if (U ints = p.read(e + 0x34)) { p.call(0x6fa390, 0, {ints}); p.write(e + 0x34, 0); }   // 005035E9..005035F9
  p.call(0x6fa852, 0, {e + 0x48, 8, 6, 0x5e5c20});                  // ~CExoString x6 at +0x48
}
void remove_effect_target_004cf3f0(U creator, U owner_id, Ports& p) {
  U owner = p.call(0x4ae750, server(p), {owner_id});
  if (!owner || !p.vcall(owner, 0x10, {})) return;                  // 004CF402..004CF416
  if (!sgt(p.read(creator + 0x140), 0)) return;                     // 004CF418..004CF420
  U object = p.vcall(owner, 0x10, {});
  U count = p.read(object + 0x128);
  bool found = false;                                               // owner still carries an effect from this creator?
  for (U i = 0; i < count; ++i) {                                   // 004CF440..004CF45A (unsigned)
    if (found) return;
    if (p.read(p.read(p.read(object + 0x124) + i * 4) + 0x18) == p.read(creator + 4)) found = true;
  }
  if (found) return;
  U n = p.read(creator + 0x140), hits = 0;                          // 004CF460..004CF47D
  if (!sgt(n, 0)) return;
  for (U cell = p.read(creator + 0x13c); n; --n, cell += 4) if (p.read(cell) == owner_id) ++hits;
  if (hits) int_list_remove_0066dcc0(creator + 0x13c, owner_id, p);
}
U remove_effect_by_id_004d06c0(U o, U id_lo, U id_hi, Ports& p) {
  U removed = 0;
  U aim = ai_master(p);
  U i = 0;
  if (!sgt(p.read(o + 0x128), 0)) return 0;                         // 004D06E2..004D06E8
  do {                                                              // 004D06F0..004D07FF
    U e = p.read(p.read(o + 0x124) + i * 4);
    if (p.read(e) != id_lo || p.read(e + 4) != id_hi) { ++i; continue; }
    U creator = p.call(0x4ae750, server(p), {p.read(e + 0x18)});
    U listener = p.read(aim + 0x58);
    if (p.vcall(listener, 0xc, {o, e}) != 1) { ++i; continue; }     // CServerAIMaster::OnEffectRemoved == [00746678]
    U count = p.read(o + 0x128), k = 0;                             // 004D073D..004D0788 remove from array
    if (sgt(count, 0)) {
      for (U cell = p.read(o + 0x124); p.read(cell) != e; cell += 4) {
        ++k;
        if (!slt(k, p.read(o + 0x128))) break;
      }
    }
    p.write(o + 0x128, count - 1);
    for (; slt(k, count - 1); ) {
      U data = p.read(o + 0x124);
      p.write(data + k * 4, p.read(data + k * 4 + 4));
      ++k;
      if (!slt(k, p.read(o + 0x128))) break;
    }
    if (p.read8(o + 8) == type::creature) p.vcall(o, 0xd8, {});     // 004D078A..004D0799
    if (p.read8(o + 8) == type::creature) p.vcall(o, 0xdc, {e});    // 004D079F..004D07AE
    effect_destroy_00503590(e, p);
    p.call(0x6fa390, 0, {e});
    removed = 1;
    if (creator && p.vcall(creator, 0x10, {}))                      // 004D07C4..004D07E5
      remove_effect_target_004cf3f0(p.vcall(creator, 0x10, {}), p.read(o + 4), p);
  } while (slt(i, p.read(o + 0x128)));
  return removed;
}
void update_effect_list_004d1730(U o, U day, U ms, U e, Ports& p) {
  // e+N == [esp+N] after `push edi` (004D178D); e+0x4C/e+0x50 are the argument slots (day / reused "restart" flag).
  p.write(e + 0x4c, day); p.write(e + 0x50, ms);
  p.write(e + 0x1c, 0); p.write(e + 0x18, 0);
  U timer = p.call(0x4ae830, server(p), {p.read(o + 4)});           // 004D1763 GetActiveTimer
  p.write(e + 0x14, timer);
  if (!p.read(o + 0x128)) return;
  p.write(e + 0x10, 0);
  do {
    p.write(e + 0x50, 0);
    U eff = p.read(p.read(o + 0x124) + p.read(e + 0x10) * 4);
    U type = p.read16(eff + 8);
    if (type == kEffectRegenerate) {                                // 004D17A1..004D1978
      U a2 = gi(eff, 2, p), a3 = gi(eff, 3, p);
      p.call(0x4ae460, p.read(e + 0x14), {p.read(e + 0x4c), ms, a2, a3, e + 0x1c, e + 0x18});
      bool tick = false;
      if (p.vcall(o, 0x30, {})) {
        U cr = p.vcall(o, 0x30, {});
        if (!p.vcall(cr, 0x94, {}) && !p.call(0x4ef890, p.vcall(o, 0x30, {}), {})) {
          if (gi(eff, 4, p) == 0x36) {                              // force-point regeneration
            U stats = p.read(p.vcall(o, 0x30, {}) + 0xa74);
            U maxfp = p.call(0x4fd490, p.vcall(o, 0x30, {}), {});
            tick = static_cast<std::int16_t>(p.read16(stats + 0x124)) < static_cast<std::int16_t>(maxfp & 0xffff);
          } else {
            U self = p.vcall(o, 0x30, {});
            U other = p.vcall(o, 0x30, {});
            U cur = p.vcall(self, 0x9c, {1});
            U mx = p.vcall(other, 0x98, {1});
            tick = static_cast<std::int16_t>(cur & 0xffff) < static_cast<std::int16_t>(mx & 0xffff);
          }
        }
      } else {
        U cur = p.vcall(o, 0x9c, {1});
        U mx = p.vcall(o, 0x98, {1});
        tick = static_cast<std::int16_t>(cur & 0xffff) < static_cast<std::int16_t>(mx & 0xffff) && !p.vcall(o, 0x94, {});
      }
      if (tick && p.read(e + 0x18) > gi(eff, 1, p)) {               // 004D18C1..004D18CE elapsed > interval
        U mem = p.call(0x6fa7e6, 0, {0x8c});
        U heal = mem ? p.call(0x503e40, mem, {1}) : 0;              // CGameEffect::CGameEffect(1)
        p.write16(heal + 8, kEffectHeal);
        si(heal, 0, gi(eff, 0, p), p);
        si(heal, 1, gi(eff, 4, p), p);
        p.write16(heal + 0xa, (p.read16(heal + 0xa) & 0xfff8) | kDurationInstant);
        p.call(0x503a00, heal, {p.read(eff + 0x18)});               // SetCreator
        p.call(0x4d14f0, o, {heal, 0, 0});                          // ApplyEffect(heal, 0, 0)
        si(eff, 2, p.read(e + 0x4c), p);
        si(eff, 3, ms, p);
        p.write(e + 0x50, 1);
      }
    } else if (type == kEffectPoison) {                             // 004D197D..004D1AF8
      U a1 = gi(eff, 1, p);
      p.write(e + 0x24, gi(eff, 2, p));
      p.write(e + 0x20, gi(eff, 5, p));
      p.call(0x4ae460, p.read(e + 0x14), {p.read(e + 0x4c), ms, a1, p.read(e + 0x24), e + 0x34, e + 0x28});
      if (p.read(e + 0x28) > p.read(e + 0x20) * 1000) {             // poison duration over
        U id = p.read(o + 4);
        p.call(0x4b08d0, ai_master(p), {0, 0, id, id, kEventPoisonExpired, eff});
      } else {
        p.write(e + 0x2c, gi(eff, 3, p));
        p.write(e + 0x24, gi(eff, 4, p));
        U a6 = gi(eff, 6, p);
        p.call(0x4ae460, p.read(e + 0x14), {p.read(e + 0x4c), ms, p.read(e + 0x2c), p.read(e + 0x24), e + 0x38, e + 0x30});
        if (p.read(e + 0x30) > a6 * 1000) {
          U gui = p.call(0x5ed690, p.read(p.global(kAppManager) + 4), {});
          if (p.read(gui + 0xb4)) {
            si(eff, 5, a6 + p.read(e + 0x20), p);                    // conversation running: extend
          } else {
            U f = effect_float_005036c0(eff, 0, p);
            U a7 = gi(eff, 7, p);
            U a0 = gi(eff, 0, p);
            p.call(0x4ee770, p.vcall(o, 0x30, {}), {a0, eff, a7, f}); // ApplyPoisonDamage
            effect_set_float_005036d0(eff, 0, x87::add_float(effect_float_005036c0(eff, 0, p), 0x3f800000), p);
          }
          si(eff, 3, p.read(e + 0x4c), p);
          si(eff, 4, ms, p);
        }
      }
    } else if (type == kEffectDisease) {                            // 004D1AFA..004D1B4B
      U a3 = gi(eff, 3, p), a4 = gi(eff, 4, p);
      if (!slt(p.call(0x4adfa0, p.read(e + 0x14), {p.read(e + 0x4c), ms, a3, a4}), 0) && p.vcall(o, 0x30, {})) {
        p.call(0x4f6790, p.vcall(o, 0x30, {}), {eff, p.read(e + 0x4c), ms});   // ApplyDiseasePayload
        p.write(e + 0x50, 1);
      }
    }
    // 004D1B53: expiry of temporary effects
    if ((p.read8(eff + 0xa) & 7) == kDurationTemporary) {
      effect_expiry_00503770(eff, e + 0x1c, e + 0x18, p);
      if (p.call(0x4adfa0, p.read(e + 0x14), {p.read(e + 0x4c), ms, p.read(e + 0x1c), p.read(e + 0x18)}) == 1) {
        remove_effect_by_id_004d06c0(o, p.read(eff), p.read(eff + 4), p);
        p.write(e + 0x10, 0);                                       // restart the scan
        continue;
      }
    }
    p.write(e + 0x10, p.read(e + 0x10) + 1);
    if (p.read(e + 0x50)) p.write(e + 0x10, 0);
  } while (p.read(e + 0x10) < p.read(o + 0x128));                   // unsigned jb
}
void remove_effect_004d05c0(U o, U e, Ports& p) {
  U aim = ai_master(p);
  if (p.vcall(p.read(aim + 0x58), 0xc, {o, e}) != 1) return;       // 004AF100 OnEffectRemoved == [00746678]
  U count = p.read(o + 0x128), data = p.read(o + 0x124), k = 0xffffffffu;
  for (U i = 0; slt(i, count); ++i) if (p.read(data + i * 4) == e) { k = i; break; }   // or eax,-1 when absent
  p.write(o + 0x128, count - 1);
  if (k == 0xffffffffu && slt(k, count - 1)) p.call(0x4d062f, 0, {});   // ORIGINAL BUG: effect absent -> writes data[-1]
  if (slt(k, count - 1)) {                                          // 004D0618..004D063A (k = -1 shifts into data[-1])
    do { U d = p.read(o + 0x124); p.write(d + k * 4, p.read(d + k * 4 + 4)); ++k; } while (slt(k, p.read(o + 0x128)));
  }
  if (U creator = p.call(0x4ae750, server(p), {p.read(e + 0x18)}))
    if (p.vcall(creator, 0x10, {})) remove_effect_target_004cf3f0(p.vcall(creator, 0x10, {}), p.read(o + 4), p);
  if (p.read8(o + 8) == type::creature) p.vcall(o, 0xdc, {e});
  effect_destroy_00503590(e, p);
  p.call(0x6fa390, 0, {e});
  if (p.read8(o + 8) == type::creature) p.vcall(o, 0xd8, {});
}
void remove_effect_by_creator_004d0820(U o, U creator_id, Ports& p) {
  if (!sgt(p.read(o + 0x128), 0)) return;
  for (U i = 0; slt(i, p.read(o + 0x128)); ++i) {                   // no index fix-up after a removal (original)
    U e = p.read(p.read(o + 0x124) + i * 4);
    if (p.read(e + 0x18) == creator_id) p.call(0x4d05c0, o, {e});
  }
}
}

