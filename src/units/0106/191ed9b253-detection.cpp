#include "5ecaebb99c-perception.hpp"
#include "1a847f7684-w1_x87.hpp"
#include <cstring>
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// Stealth detection: 004FB4B0 DoListenDetection, 004F1FD0 DoSpotDetection and their leaves
// 004FB0F0 MaxAttackRange, 004F1310 DesiredAttackRange, 004EDAE0 GetDetectMode, 004CAE60 AnimationStationary,
// 004EFB20 GetClientCreature.
namespace w1::perception {
namespace {
inline float fl(U b) { float f; std::memcpy(&f, &b, 4); return f; }
inline U bits(float f) { U b; std::memcpy(&b, &f, 4); return b; }
inline U sx8(U v) { return static_cast<U>(static_cast<S>(static_cast<signed char>(v & 0xff))); }
inline S half(U v) { return static_cast<S>(v) / 2; }            // cdq; sub eax,edx; sar eax,1
x87::X80 rx(U a, Ports& p) { x87::X80 r; for (U i = 0; i < 10; ++i) r.b[i] = static_cast<unsigned char>(p.read8(a + i)); return r; }
constexpr U kHeadOffset = 0x3f800000;                           // [0073D764] = 1.0f (spot)
constexpr U kEyeOffset = 0x3fc00000;                            // [0073F414] = 1.5f (listen)
// GetSkillRank(stats, skill, versus object, 0) via the object's +0x10 accessor (AsSWSObject), AL sign-extended.
U skill_rank(U stats, U skill, U versus_creature, Ports& p) {
  U versus = p.vcall(versus_creature, 0x10, {});
  return sx8(p.call(0x5aa570, stats, {skill, versus, 0}));
}
}
U detect_mode_004edae0(U c, Ports& p) {
  if (p.read(c + 0xd4) == 0x2710 && p.read8(c + 0x4d0) != 1) return 2;      // 004EDAE0..004EDAF5
  return p.read8(c + 0x4d0);
}
U animation_stationary_004cae60(U animation, Ports&) {
  switch (animation & 0xffff) {                                     // 004CAE60..004CAEB7
  case 0x2712: case 0x2713: case 0x2714: case 0x275e: case 0x275f: case 0x2765: case 0x2764: case 0x2767:
  case 0x2766: case 0x276d: case 0x276e: case 0x2795: return 0;
  default: return 1;
  }
}
U client_creature_004efb20(U c, Ports& p) {
  if (!p.call(0x4cc2b0, c, {})) return 0;                           // 004EFB23..004EFB2A GetClientObject
  U client = p.call(0x4cc2b0, c, {});
  return p.vcall(client, 0x2c, {});                                 // jmp [edx+2c]
}
x87::X80 desired_attack_range_004f1310(U c, U id, U flag, U scratch, Ports& p) {
  p.write(scratch + 0xc, 0);                                        // 004F1325 [esp+0x10] (result slot)
  U other = p.call(0x4ae750, server(p), {id});
  if (!other) return x87::x80_from_float(p.read(scratch + 0xc));    // 004F1336 -> 004F143F
  if (p.vcall(other, 0x30, {})) {                                   // creature target
    U target = p.vcall(other, 0x30, {});
    if (flag)                                                       // 004F135C..004F137A
      return x87::x80_add_two_floats(p.read(p.read(target + 0x340) + 8), p.read(p.read(c + 0x340) + 8), 0x3f333333);
    U table = p.read(p.read(p.global(kRules) + 0xb8) + 0xc);        // appearance 2DA
    p.write(scratch + 0x20, 0);
    p.call(0x413350, table, {p.read16(c + 0xa60), p.global(0x7a210c), scratch + 0x20});
    p.write(scratch + 0xc, x87::add_float(p.read(scratch + 0x20), 0x3fcccccd)); // fstp [esp+10] == result slot; + [00746F48] = 1.6f
    p.call(0x413350, table, {p.read16(target + 0xa60), p.global(0x7a210c), scratch + 0x20});
    return x87::x80_add_float(x87::x80_from_float(p.read(scratch + 0x20)), p.read(scratch + 0xc)); // 004F13E2..004F13E7
  }
  U positioned;                                                     // 004F13F2..004F140F placeable / door
  if (p.vcall(other, 0x18, {})) positioned = p.vcall(other, 0x18, {});
  else if (p.vcall(other, 0x48, {})) positioned = p.vcall(other, 0x48, {});
  else return x87::x80_from_float(kEyeOffset);                      // 004F144C fld [0073F414]
  U pos = positioned + 0x90;
  p.write(scratch + 0x10, p.read(pos)); p.write(scratch + 0x14, p.read(pos + 4)); p.write(scratch + 0x18, p.read(pos + 8));
  p.call(0x4ee440, c, {id, scratch + 0x10, scratch + 0x0c, 0});     // GetUseRange(id, &pos, &range, 0)
  return x87::x80_from_float(p.read(scratch + 0xc));                // 004F143F fld [esp+0xc]
}
x87::X80 max_attack_range_004fb0f0(U c, U id, U a2, U a3, U scratch, Ports& p) {
  U item = p.call(0x5a4c20, p.read(c + 0xa2c), {0x10});             // GetItemInSlot(right hand)
  if (item && p.read8(p.call(0x5b4790, item, {}) + 0x1a)) {         // ranged weapon
    if (a3) return x87::x80_from_float(0x41a00000);                 // [007455D8] = 20.0f
    U range = p.read(p.call(0x5b4790, item, {}) + 0x34);
    if (fl(range) == 0.0f) return x87::x80_from_float(0x41f00000);  // 004FB13C..004FB14C: equal to 0 -> [007455DC] = 30.0f
    return x87::x80_from_float(range);
  }
  return x87::x80_add_float(desired_attack_range_004f1310(c, id, a2, scratch, p), 0x3f000000); // + [0073E9AC] = 0.5f
}
U listen_detection_004fb4b0(U c, U target, U flag, U e, Ports& p) {
  // e+N == [esp+N] after `push edi` (004FB4B5); the later `push ebx` is folded into the offsets below.
  for (U o : {0x2cu, 0x30u, 0x34u, 0x20u, 0x24u, 0x28u}) p.write(e + o, 0);
  p.write(e + 0x10, 1);
  S score = 0;                                                      // ebp
  if (p.read(target + 0x8c8)) return 0;                             // 004FB4BA..004FB4FE
  p.write(e + 0x38, p.read(c + 0x90)); p.write(e + 0x3c, p.read(c + 0x94));
  p.write(e + 0x40, x87::add_float(p.read(c + 0x98), kEyeOffset));
  p.write(e + 0x44, p.read(target + 0x90)); p.write(e + 0x48, p.read(target + 0x94));
  p.write(e + 0x4c, x87::add_float(p.read(target + 0x98), kEyeOffset));
  p.write(e + 0x14, x87::sub_float(p.read(e + 0x38), p.read(e + 0x44)));
  p.write(e + 0x18, x87::sub_float(p.read(e + 0x3c), p.read(e + 0x48)));
  p.write(e + 0x1c, x87::sub_float(p.read(e + 0x40), p.read(e + 0x4c)));
  U flags = p.read8(c + 0x8ec);
  bool check_attack = flag || (flags & 0x10) || ((flags & 8) && !(flags & 2));      // 004FB57E..004FB590
  if (check_attack) {
    x87::X80 r = max_attack_range_004fb0f0(c, p.read(target + 4), 0, 0, e + 0x94, p);
    unsigned short sw = x87::cmp_r2_sum(r, p.read(e + 0x1c), p.read(e + 0x18), p.read(e + 0x14));
    if ((((sw >> 8) & 5) == 1) || (((sw >> 8) & 5) == 4)) return 0;               // jnp: r*r < sum
    U el = visible_element_00517a20(c, p.read(target + 4), p);
    if (el && (p.read8(el + 4) & 0xc) == 4) return 0;
  }
  {
    U listen = listen_distance_004efd20(c, e + 0x70, p);
    unsigned short sw = x87::cmp_r2_sum(x87::x80_from_float(listen), p.read(e + 0x1c), p.read(e + 0x18), p.read(e + 0x14));
    U m = (sw >> 8) & 5;
    if (!(m == 0 || m == 5)) return 0;                              // jp continue
  }
  // 004FB623: two line-of-sight rays (mine->target, target->mine); blockers in e+0x64 / e+0x0c
  U area = p.call(0x4cb120, c, {});
  p.call(0x50c330, area, {e + 0x38, e + 0x44, e + 0x2c, e + 0x64, kInvalidObject, kInvalidObject, 0, 0});
  area = p.call(0x4cb120, c, {});
  p.call(0x50c330, area, {e + 0x44, e + 0x38, e + 0x20, e + 0x0c, kInvalidObject, kInvalidObject, 0, 0});
  U b1 = p.read(e + 0x64), b2 = p.read(e + 0x0c);
  bool skip_distance = false;
  if (b1 == b2) {
    if (b1 != kInvalidObject) { p.write(e + 0x60, 0x3e23d70b); skip_distance = true; }   // 004FB6A7 [esp+64] (after push ebx) == e+0x60: 0.16f
  } else {
    if (b1 != kInvalidObject) {                                     // 004FB6BB..004FB6F7
      U o = p.call(0x4ae750, server(p), {b1});
      if (o && p.vcall(o, 0x10, {})) {
        U pos = p.vcall(o, 0x10, {}) + 0x90;
        p.write(e + 0x2c, p.read(pos)); p.write(e + 0x30, p.read(pos + 4)); p.write(e + 0x34, p.read(pos + 8));
      }
    }
    if (b2 != kInvalidObject) {                                     // 004FB6FF..004FB745 (looks up blocker 1 again)
      U o = p.call(0x4ae750, server(p), {b1});
      if (o && p.vcall(o, 0x10, {})) {
        U pos = p.vcall(o, 0x10, {}) + 0x90;
        p.write(e + 0x20, p.read(pos)); p.write(e + 0x24, p.read(pos + 4)); p.write(e + 0x28, p.read(pos + 8));
      }
    }
  }
  bool to_skill = false;
  if (!skip_distance) {                                             // 004FB749..004FB7A4
    p.write(e + 0x60, x87::listen_blocker_sum(p.read(e + 0x20), p.read(e + 0x24), p.read(e + 0x28),
                                               p.read(e + 0x2c), p.read(e + 0x30), p.read(e + 0x34)));
    if (!(fl(p.read(e + 0x60)) > 0.0f)) to_skill = true;            // test ah,41 / jne 004FB7D8
  }
  if (!to_skill) {                                                  // 004FB7A6
    U ar = p.call(0x4cb120, c, {});
    if ((p.read8(ar + 4) & 1) || (p.read8(p.call(0x4cb120, c, {}) + 4) & 2)) {
      U obj = p.read(p.call(0x4cb120, c, {}) + 0x1b0);              // 004FB873..004FB8BD
      p.write(obj + 0x74, p.read(c + 0x90));
      p.write(obj + 0x78, x87::x80_to_float(x87::x80_from_float(p.read(c + 0x94))));
      p.write(obj + 0x5c, p.read(target + 0x90));
      p.write(obj + 0x60, x87::x80_to_float(x87::x80_from_float(p.read(target + 0x94))));
      U a2 = p.call(0x4cb120, c, {});
      if (p.call(0x6054e0, a2, {obj}) != 1) return 0;               // return_true_4
      score = -2;
    } else {
      score = static_cast<S>(x87::ftol_sqrt_float(p.read(e + 0x60))) * -5;
    }
  }
  // 004FB7D8: skills
  if (!(skill_rank(p.read(target + 0xa74), 2, c, p) & 0xff)) return 1;
  U mode = p.read8(target + 0x4d1);
  if (!mode) return 1;
  if (mode == 1) score -= static_cast<S>(skill_rank(p.read(target + 0xa74), 2, c, p)) + static_cast<S>(p.read8(target + 0x91d));
  U dm = detect_mode_004edae0(c, p);
  if (dm == 1 || detect_mode_004edae0(c, p) == 2) score += static_cast<S>(skill_rank(p.read(c + 0xa74), 3, target, p));
  else score += half(skill_rank(p.read(c + 0xa74), 3, target, p));
  score += static_cast<S>(p.read(p.call(0x4cb120, c, {}) + 0x18c) + p.read8(c + 0x91f)); // 004FB8EC..004FB906
  client_creature_004efb20(c, p);
  if (p.read(c + 0x4e0)) score -= 10;
  if (animation_stationary_004cae60(p.read(target + 0xd4), p)) score -= 5;
  if (animation_stationary_004cae60(p.read(c + 0xd4), p)) score += 5;
  p.call(0x423bc0, 0, {e + 0x50, c + 0x90, target + 0x90});         // Vector out = mine - target (004FB957)
  p.call_st0(0x4ab100, 0, {e + 0x50}, e + 0x88);                    // vector_magnitude -> ST0
  score += static_cast<S>(x87::ftol_mul_float(rx(e + 0x88, p), 0xbeaaaaab));      // * [007471AC] = -1/3
  switch (p.read(target + 0x4f8)) {                                 // 004FB972..004FB99B
  case 1: score -= 8; break;
  case 2: score -= 4; break;
  case 4: score += 4; break;
  case 5: score += 8; break;
  default: break;
  }
  score += static_cast<S>(p.call(0x63e7c0, p.read(c + 0xa74), {target}));          // return_false_4
  if (score <= 0) p.write(e + 0x10, 0);                             // 004FB9B2 [esp+14] (after push ebx) == e+0x10
  return p.read(e + 0x10);                                          // 1 (initialised at 004FB4F6) or 0
}
U spot_detection_004f1fd0(U c, U target, U flag, U e, Ports& p) {
  // e+N == [esp+N] after the last prologue push; e+0x60 / e+0x64 are the argument slots (reused as locals).
  p.write(e + 0x60, target); p.write(e + 0x64, flag);
  p.write(e + 0x2c, p.read(c + 0x90)); p.write(e + 0x30, p.read(c + 0x94));
  p.write(e + 0x34, x87::add_float(p.read(c + 0x98), kHeadOffset));
  p.write(e + 0x38, p.read(target + 0x90)); p.write(e + 0x3c, p.read(target + 0x94));
  p.write(e + 0x40, x87::add_float(p.read(target + 0x98), kHeadOffset));
  S score = 0; U ebp = 0;                                           // esi / ebp
  p.write(e + 0x28, 1);
  for (U o : {0x10u, 0x14u, 0x18u, 0x1cu, 0x20u, 0x24u}) p.write(e + o, 0);
  U flags = p.read8(c + 0x8ec);
  if ((flags & 0x10) || ((flags & 8) && !(flags & 2)) || p.read(e + 0x64)) return 0;   // 004F2040..004F207C -> 004F20D4
  p.write(e + 0x64, x87::sub_float(p.read(e + 0x34), p.read(e + 0x40)));
  p.write(e + 0x60, x87::sub_float(p.read(e + 0x30), p.read(e + 0x3c)));
  p.write(e + 0x38, x87::sub_float(p.read(e + 0x2c), p.read(e + 0x38)));
  {
    U spot = spot_distance_004efc70(c, e + 0x70, p);
    unsigned short sw = x87::cmp_r2_sum(x87::x80_from_float(spot), p.read(e + 0x38), p.read(e + 0x60), p.read(e + 0x64));
    U m = (sw >> 8) & 5;
    if (!(m == 0 || m == 5)) return 0;                              // jp 004F20DB
  }
  if (p.read8(c + 0x8ec) & 4) return 1;                             // 004F20DB..004F20E3
  if (!(skill_rank(p.read(target + 0xa74), 2, c, p) & 0xff)) return 1;
  U mode = p.read8(target + 0x4d1);
  if (!mode) return 1;
  if (mode == 1) {                                                  // 004F2121..004F2168
    score = -static_cast<S>(skill_rank(p.read(target + 0xa74), 2, c, p)) - static_cast<S>(p.read8(target + 0x91c));
    p.write(e + 0x10, skill_rank(p.read(target + 0xa74), 2, c, p));
  }
  U dm = detect_mode_004edae0(c, p);
  if (dm == 1 || detect_mode_004edae0(c, p) == 2) {                 // 004F21D2..004F2213
    score += static_cast<S>(skill_rank(p.read(c + 0xa74), 3, target, p));
    ebp = skill_rank(p.read(c + 0xa74), 3, target, p);
  } else {                                                          // 004F2180..004F21CE
    score += half(skill_rank(p.read(c + 0xa74), 3, target, p));
    ebp = static_cast<U>(half(skill_rank(p.read(c + 0xa74), 3, target, p)));
  }
  score += static_cast<S>(p.read8(c + 0x91e));
  client_creature_004efb20(c, p);
  U mine_client = p.call(0x4ae910, server(p), {p.read(c + 4)});
  U their_client = p.call(0x4ae910, server(p), {p.read(target + 4)});
  if (!mine_client && their_client) {                               // 004F2252..004F2324 facing check
    p.call(0x423bc0, 0, {e + 0x2c, target + 0x90, c + 0x90});       // dir = target - mine
    if (x87::sumsq_not_above_double(p.read(e + 0x2c), p.read(e + 0x30), p.read(e + 0x34), 0.0)) {
      p.write(e + 0x2c, p.read(c + 0x9c)); p.write(e + 0x30, p.read(c + 0xa0)); p.write(e + 0x34, p.read(c + 0xa4));
    } else {
      U n = p.call(0x4ab180, 0, {e + 0x38, e + 0x2c});              // normalize(out, in)
      p.write(e + 0x2c, p.read(n)); p.write(e + 0x30, p.read(n + 4)); p.write(e + 0x34, p.read(n + 8));
    }
    U f = p.call(0x4ab180, 0, {e + 0x38, c + 0x9c});                // normalize(own orientation)
    if (x87::dot_not_above(p.read(e + 0x2c), p.read(e + 0x30), p.read(e + 0x34), p.read(f), p.read(f + 4), p.read(f + 8), 0xbf34fdf4)) {
      score -= 5; p.write(e + 0x20, 0xfffffffbu);                   // facing away: [00746FFC] = -0.707f
    }
  }
  if (p.read(c + 0x4e0)) { score -= 10; p.write(e + 0x24, 0xfffffff6u); }
  if (animation_stationary_004cae60(p.read(target + 0xd4), p)) { score -= 5; p.write(e + 0x18, 5); }
  if (animation_stationary_004cae60(p.read(c + 0xd4), p)) { score += 5; p.write(e + 0x14, 5); }
  p.call(0x423bc0, 0, {e + 0x44, c + 0x90, target + 0x90});         // mine - target
  p.call_st0(0x4ab100, 0, {e + 0x44}, e + 0x80);
  U penalty;
  if (x87::mul_above_ftol(rx(e + 0x80, p), 0x3eaaaaab, 0x40000000, penalty)) {     // * 1/3 above 2.0
    score -= static_cast<S>(penalty);
    p.write(e + 0x1c, static_cast<U>(-static_cast<S>(penalty)));
  }
  if (score <= 0) { p.write(e + 0x28, 0); return 0; }
  U mem = p.call(0x6fa7e6, 0, {0x34});                              // 004F23DB message data
  U msg = mem ? p.call(0x4d69f0, mem, {}) : 0;
  p.call(0x4d7780, msg, {0, p.read(c + 4)});
  p.call(0x4d7780, msg, {1, p.read(target + 4)});
  p.call(0x4d76b0, msg, {0, 1});
  p.call(0x4d76b0, msg, {1, p.read8(c + 0x91e)});
  p.call(0x4d76b0, msg, {2, p.read8(target + 0x91c)});
  p.call(0x4d76b0, msg, {3, ebp});
  p.call(0x4d76b0, msg, {4, p.read(e + 0x10)});
  p.call(0x4d76b0, msg, {5, p.read(e + 0x14)});
  p.call(0x4d76b0, msg, {6, p.read(e + 0x18)});
  p.call(0x4d76b0, msg, {7, p.read(e + 0x1c)});
  p.call(0x4d76b0, msg, {8, p.read(e + 0x20)});
  p.call(0x4d76b0, msg, {9, 0});
  p.call(0x4d76b0, msg, {0xa, p.read(e + 0x24)});
  if (p.call(0x4ae910, server(p), {p.read(c + 4)})) p.call(0x4ec7a0, c, {msg});          // BroadcastStealthDetectionData
  else if (p.call(0x4ae910, server(p), {p.read(target + 4)})) p.call(0x4ec7a0, target, {msg});
  if (msg) {
    p.call(0x4d6a40, msg, {});                                      // ~CSWCCMessageData
    p.call(0x6fa390, 0, {msg});
  }
  return p.read(e + 0x28);
}
}
