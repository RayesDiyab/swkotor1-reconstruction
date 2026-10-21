#include "7dd862643f-creature_ai.hpp"
#include "1a847f7684-w1_x87.hpp"
#include "4a8ca74ca0-leaves.hpp"
#include "5ecaebb99c-perception.hpp"
#include "e556d6f3cd-combat_timer.hpp"
#include "fde3237102-effects.hpp"
#include "65dd6aca7d-run_actions.hpp"
#include "8fe93ed262-mine_check.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
namespace w1::creature_ai {
namespace {
// BSS cells read by AIUpdate. No instruction of the original .text ever stores a nonzero value into them
// (xref scan: only AIUpdate's own clears, plus client reads of 008327EC/008327F0), so retail value is 0.
constexpr U kDebugFullHeal = 0x8327f8;     // read/cleared 004FE211/004FE22E
constexpr U kDebugDistance = 0x832838;     // read 004FE23D
constexpr U kDebugStatusDump = 0x8327e8;   // read 004FE895
constexpr U kDebugWatchA = 0x8327ec;       // read 004FE925
constexpr U kDebugWatchB = 0x8327f0;       // read 004FE92D
constexpr U kDebugZeroForce = 0x832800;    // read/cleared 004FE866/004FE872
constexpr U kStealthTimerEnabled = 0x7a1b28; // .data, initial 1, written by the cheat message handler
constexpr U kRegenColumnHp = 0x7a21a8;     // .data cells (rules column indices)
constexpr U kRegenColumnFp = 0x7a21ac;
inline U sx16(U v) { return static_cast<U>(static_cast<S>(static_cast<std::int16_t>(v & 0xffff))); }
U get_area(U o, Ports& p) { return p.call(0x4cb120, o, {}); }
}
U active_timer_004ae830(U server_app, U id, Ports& p) {
  U internal = p.read(server_app + 4);                              // 004AE830 mov ecx,[ecx+4]
  if (id == kInvalidObject) return p.read(internal + 0x10048);      // 004B6C44..004B6C4D
  U flags = p.read8(internal + 0x10078), two = flags & 2;
  if (!two && !(flags & 1)) return p.read(internal + 0x10048);      // 004B6C55..004B6C5E
  if (two) {                                                        // 004B6C60..004B6C95
    if (p.call(0x4b48e0, internal, {id}) == 1) return p.read(internal + 0x10050);
    return p.read(internal + 0x10048);
  }
  if (!(flags & 1)) return 0;                                       // 004B6C64..004B6C68 (unreachable form kept)
  if (p.call(0x4b48e0, internal, {id}) == 1) return p.read(internal + 0x1004c); // 004B6C6A..004B6C7E
  return p.read(internal + 0x10048);
}
void excited_timer_004ed110(U c, Ports& p) {
  U t = p.read(c + 0x384);
  if (!t) return;                                                   // 004ED119
  U dt = p.read(c + 0xcc);
  if (t > dt) { p.write(c + 0x384, t - dt); return; }               // 004ED124/004ED17D
  U id = p.read(c + 4);
  p.write(c + 0x384, 0);                                            // 004ED12B
  U client_object = p.call(0x4ae910, server(p), {id});              // 004ED13F GetClientObjectByObjectId
  if (!get_area(c, p) || !client_object) return;                    // 004ED148..004ED153
  if (!p.read(get_area(c, p) + 0x208)) return;                      // 004ED157..004ED164
  p.vcall(p.read(get_area(c, p) + 0x208), 0x10, {0});               // 004ED168..004ED177
}
void compute_ai_state_004f1460(U c, U scratch, Ports& p) {
  p.write8(c + 0x9f2, 0);                                           // 004F146A
  p.write(c + 0x9f4, kInvalidObject);                               // 004F1471
  U position = p.read(p.read(c + 0xfc));                            // 004F1464/004F147B
  p.write(scratch, position);
  if (position) {
    do {
      U action = p.call(0x5e9b60, p.read(c + 0xfc), {position});    // 004F1497 GetAtPos
      U kind = p.read(action);
      U target = kind == 0xc ? p.read(action + 0x38) : kInvalidObject; // 004F149C..004F14A8
      compute_ai_state_on_action_004ebb70(c, kind, target, p);         // 004F14AF ComputeAIStateOnAction
      p.call(0x5e9b90, p.read(c + 0xfc), {scratch});                // 004F14BF GetNext
      position = p.read(scratch);
    } while (position);
  }
  U current = p.read16(c + 0x80);                                   // 004F14CC movzx
  if (current != 0xffffffffu)                                       // cmp eax,-1 (never equal after movzx)
    compute_ai_state_on_action_004ebb70(c, current, p.read(c + 0x84), p); // 004F14D8..004F14E2
}
void compute_ai_state_on_action_004ebb70(U c, U type, U target, Ports& p) {
  U state = p.read8(c + 0x9f2);
  U simple = 0;                                                     // 004EBB74..004EBE8F: action -> AI state
  switch (type) {
  case 0x08: simple = 0x0c; break;  case 0x0b: simple = 0x0d; break;  case 0x07: simple = 0x0a; break;
  case 0x09: simple = 0x0b; break;  case 0x14: simple = 0x0f; break;  case 0x15: simple = 0x10; break;
  case 0x19: simple = 0x11; break;  case 0x1a: simple = 0x12; break;  case 0x1b: simple = 0x13; break;
  case 0x1c: simple = 0x14; break;  case 0x1d: simple = 0x15; break;  case 0x22: simple = 0x16; break;
  case 0x23: simple = 0x17; break;  case 0x26: simple = 0x18; break;  case 0x27: simple = 0x19; break;
  case 0x28: simple = 0x1a; break;  case 0x29: simple = 0x1b; break;  case 0x2a: simple = 0x1c; break;
  case 0x36: simple = 0x1d; break;  case 0x38: simple = 0x1e; break;
  default: break;
  }
  if (simple) {
    if (state < 0xa) { p.write8(c + 0x9f2, simple); p.write(c + 0x9f4, kInvalidObject); }
    return;
  }
  if (type == 0x0f || type == 0x2e) {                               // 004EBE92 spell casting
    if (state < 0xa) { p.write8(c + 0x9f2, 0x0e); p.write(c + 0x9f4, kInvalidObject); }
    return;
  }
  if (type == 0x0c) {                                               // 004EBEA8 attack
    if (state == 0 || state == 1) { p.write(c + 0x9f4, target); p.write8(c + 0x9f2, 2); }
    return;
  }
  if ((type == 1 || type == 0x33) && state == 0) {                  // 004EBED0..004EBEEB movement
    p.write8(c + 0x9f2, 1); p.write(c + 0x9f4, kInvalidObject);
  }
  if (type == 0x32) {                                               // 004EBEF1..004EBF0A counterspell
    U s = p.read8(c + 0x9f2);
    if (s == 0 || s == 1) { p.write(c + 0x9f4, kInvalidObject); p.write8(c + 0x9f2, 2); }
  }
}
U adjust_hp_accumulator_005a5c70(U stats, U amount, Ports& p) {
  U acc = p.read(stats + 0x1ac);
  U ax = x87::accumulator_adjust(acc, amount);
  p.write(stats + 0x1ac, acc);
  return ax;
}
U adjust_fp_accumulator_005a5cc0(U stats, U amount, Ports& p) {
  U acc = p.read(stats + 0x1b0);
  U ax = x87::accumulator_adjust(acc, amount);
  p.write(stats + 0x1b0, acc);
  return ax;
}
void set_stealth_xp_00506aa0(U area, U value, Ports& p) {
  U cap = p.read(area + 0x2b4);
  p.write(area + 0x2b8, value > cap ? cap : value);                 // 00506AAA..00506AB7 (unsigned)
}
U party_index_006349c0(U party, U id, Ports& p) {
  if (id == kInvalidObject) return 0xffffffffu;                     // 006349C5
  U count = p.read(party);
  if (!sgt(count, 0)) return 0xffffffffu;                           // 006349D1
  U slot = party + 0x24;
  for (U i = 0;;) {
    if (p.read(slot) == id) return i;                               // 006349D8
    ++i; slot += 0x88;
    if (!slt(i, count)) return 0xffffffffu;                         // 006349DC..006349E7
  }
}

void spawn_heartbeat_perception_004eb6e0(U c, U f, Ports& p) {
  const U mode = p.read(f + 0x48);                                  // edi = argument
  for (U o : {0x10u, 0x14u, 0x1cu, 0x18u, 0x24u, 0x20u}) p.write(f + o, 0); // 004EB704..004EB718
  U aim = ai_master(p);                                             // 004EB720 (ebp)
  auto now = [&]() {                                                // GetActiveTimer + GetWorldTime(&day f+10, &ms f+14)
    U timer = active_timer_004ae830(server(p), p.read(c + 4), p);
    p.call(0x4ade40, timer, {f + 0x10, f + 0x14});
  };
  auto since = [&](U day_off, U ms_off, U out_day, U out_ms) {      // SubtractWorldTimes(now, stamp, out)
    U wt = p.call(0x4aede0, server(p), {});
    p.call(0x4ae460, wt, {p.read(f + 0x10), p.read(f + 0x14), p.read(c + day_off), p.read(c + ms_off), out_day, out_ms});
  };
  now();
  since(0x350, 0x354, f + 0x1c, f + 0x18);                          // 004EB74A..004EB77B
  if (!p.read(c + 0x34c)) {                                         // 004EB780 spawn script
    p.call(0x5d0fc0, p.global(kVirtualMachine), {c + 0x270, p.read(c + 4), 1});
    if (p.read(c + 0xa24) == kInvalidObject && !p.read(c + 0xa88)) p.write(c + 0x34c, 1); // 004EB7A2..004EB7B6
  }
  bool heartbeat_due = (mode == 1 && p.read(f + 0x18) >= p.read(c + 0x358)) || !p.read(c + 0x34c); // 004EB7C0..004EB7DB
  if (heartbeat_due) {
    U skips = p.read(c + 0x394) + 1;                                // 004EB7E1..004EB7ED
    p.write(c + 0x394, skips);
    bool skip = p.read(c + 0x78) == 0 && skips < (p.read(c + 0x358) >> 6) && p.read(c + 0x34c);
    if (!skip) {
      p.write(c + 0x394, 0);                                        // 004EB810..004EB816
      bool run = !(p.read(c + 0x34c) && !p.read(c + 0x354));        // 004EB81C..004EB824
      if (run && !p.vcall(c, 0x94, {}) && !p.call(0x63e7f0, c, {})) { // 004EB82A..004EB845
        p.call(0x5d0fc0, p.global(kVirtualMachine), {c + 0x230, p.read(c + 4), 1}); // heartbeat script
        U row = 0; bool extra = true;
        switch (p.read8(c + 0x8ed)) {                               // 004EB863..004EB887
        case 1: row = 3; break;
        case 2: row = 2; break;
        case 11: row = 1; break;
        default: extra = false; break;
        }
        if (extra) {
          p.write8(f + 0x48, row);
          p.call(0x5b3190, f + 0x28, {});                           // CExoString() result
          p.call(0x5e5a90, f + 0x30, {0x746bb8});                   // CExoString("SCRIPTNAME")
          U table = p.read(p.read(p.global(kRules) + 0xb8) + 0x84);
          p.call(0x413270, table, {p.read8(f + 0x48), f + 0x30, f + 0x28}); // C2DA::GetCExoStringEntry
          p.call(0x5e5c20, f + 0x30, {});                           // ~CExoString(column)
          p.call(0x5d0fc0, p.global(kVirtualMachine), {f + 0x28, p.read(c + 4), 1});
          p.write(c + 0xe8, 0);                                     // 004EB8FC
          p.call(0x5e5c20, f + 0x28, {});                           // ~CExoString(result)
        }
        leaves::clean_invitations_00518660(c, f + 0x4c, p);         // 004EB911 (own 0x34-byte frame at f+4c)
      }
      p.write(c + 0x350, p.read(f + 0x10));                          // 004EB916..004EB924
      p.write(c + 0x354, p.read(f + 0x14));
      U r = p.call(0x6fc447, 0, {});
      p.write(c + 0x358, static_cast<U>(static_cast<S>(r) % 0x4b0 + 0xbb8)); // 004EB92A..004EB93D
    }
  }
  now();                                                            // 004EB943..004EB961
  since(0x360, 0x364, f + 0x1c, f + 0x18);                          // 004EB966..004EB997
  {                                                                 // 004EB99C..004EB9F4 visible list refresh
    bool refresh;
    if (!p.read(c + 0x34c)) refresh = true;
    else if (p.read(c + 0xa88)) refresh = false;
    else {
      U paused = p.read(c + 0x9d4), elapsed = p.read(f + 0x18);
      if (paused) refresh = mode == 1 && elapsed >= p.read(c + 0x35c);
      else refresh = mode == 1 && elapsed >= 0xfa0;
    }
    if (refresh) {
      p.write(f + 0x80 + 0x2c, 1); perception::update_visible_0051b050(c, f + 0x80, p); // UpdateVisibleList(1)
      U r = p.call(0x6fc447, 0, {});
      p.write(c + 0x35c, static_cast<U>(static_cast<S>(r) % 0x190 + 0x12c));
    }
  }
  since(0x388, 0x38c, f + 0x24, f + 0x20);                          // 004EB9FA..004EBA2C
  {
    U spawned = p.read(c + 0x34c);
    bool due;
    if (!spawned) due = true;
    else {
      U paused = p.read(c + 0x9d4), elapsed = p.read(f + 0x20);
      if (paused) due = mode == 1 && elapsed >= 0xc8;
      else due = mode == 1 && elapsed >= 0xfa0;
    }
    if (due) {
      U n = p.read(c + 0x390) + 1;                                  // 004EBA73..004EBA92
      p.write(c + 0x390, n);
      if (!(p.read(c + 0x78) == 0 && slt(n, 0x96) && spawned)) {
        U r = p.call(0x6fc447, 0, {});
        p.write(c + 0x390, static_cast<U>(static_cast<S>(r) % 0x32)); // 004EBA94..004EBAA7
        if (p.read(c + 0x38c) || !p.read(c + 0x34c))
          { p.write(f + 0x80 + 0x2c, p.read(c + 0xa88) ? 0u : 2u); perception::update_visible_0051b050(c, f + 0x80, p); } // 004EBAB7..004EBAC6
        p.write(c + 0x388, p.read(f + 0x10));
        p.write(c + 0x38c, p.read(f + 0x14));
        if (get_area(c, p) && !p.read(get_area(c, p) + 0x128) && p.read(c + 0x78))
          p.call(0x4b08a0, aim, {c, 0});                            // 004EBAFE..004EBB02
      }
    }
  }
  if (!p.read(c + 0x34c)) p.write(c + 0x34c, 1);                    // 004EBB07..004EBB0F
}
void ai_update_004fe210(U c, U f, Ports& p) {
  // --- debug full-heal (004FE210..004FE238)
  if (p.global(kDebugFullHeal) && p.read(c + 0xa88)) {
    p.set_global(kDebugFullHeal, 0);
    p.call(0x4cc280, c, {1});
  }
  // --- debug distance-to-player probe (004FE23D..004FE2A7); result discarded
  if (p.global(kDebugDistance)) {
    U srv = server(p);
    U player = p.call(0x4ae770, srv, {p.call(0x4aea40, srv, {})});
    p.call(0x5e5670, c + 0x18, {});
    if (player) {
      p.write(f + 0x38, x87::sub_float(p.read(c + 0x90), p.read(player + 0x90)));
      p.write(f + 0x3c, x87::sub_float(p.read(c + 0x94), p.read(player + 0x94)));
      p.write(f + 0x40, x87::sub_float(p.read(c + 0x98), p.read(player + 0x98)));
      p.call(0x4ab100, 0, {f + 0x38});                              // vector_magnitude (cdecl), fstp st(0)
    }
  }
  // --- clocks (004FE2AA..004FE2E0)
  std::uint64_t now = p.call64(0x5e9260, p.read(p.global(0x7a39e0) + 4), {});
  U now_lo = static_cast<U>(now), now_hi = static_cast<U>(now >> 32);
  U srv = server(p);
  p.write(f + 0x28, srv);
  p.write(f + 0x18, ai_master(p));
  p.write(f + 0x2c, p.call(0x4aede0, srv, {}));                     // GetWorldTimer
  // --- AI level promotion (004FE2DA..004FE301)
  if ((p.read(p.read(c + 0xa74) + 0x6c) == 1 || p.read(c + 0xa88)) && p.read(c + 0x78) != 4)
    p.call(0x4b08a0, p.read(f + 0x18), {c, 4});                     // SetAILevel
  { U frame = f + 0x44; p.write(frame + 0x48, 1); spawn_heartbeat_perception_004eb6e0(c, frame, p); } // 004FE30A
  // --- object time step (004FE30F..004FE36E)
  p.write(f + 0x10, 0); p.write(f + 0x14, 0); p.write(f + 0x30, 0); p.write(f + 0x20, 0);
  U active = active_timer_004ae830(srv, p.read(c + 4), p);          // 004FE331 GetActiveTimer
  p.call(0x4ade40, active, {f + 0x10, f + 0x14});                   // GetWorldTime(&day,&ms)
  p.call(0x4ae460, p.read(f + 0x2c),
         {p.read(f + 0x10), p.read(f + 0x14), p.read(c + 0xa8), p.read(c + 0xac), f + 0x30, f + 0x20});
  p.write(c + 0xcc, p.read(f + 0x20));
  combat_timer::update_combat_round_timer_004faf20(c, f + 0x114, p);  // 004FE374 UpdateCombatRoundTimer (own 0x10 scratch)
  excited_timer_004ed110(c, p);                                     // 004FE37B
  effects::update_effect_list_004d1730(c, p.read(f + 0x10), p.read(f + 0x14), f + 0x124, p); // 004FE38C UpdateEffectList (own 0x54 frame)
  run_actions::run_actions_0057f4a0(c, p.read(f + 0x10), p.read(f + 0x14), now_lo, now_hi, f + 0x178, p); // 004FE39F RunActions (own 0x40 frame)
  p.write(c + 0xa8, p.read(f + 0x10));
  p.write(c + 0xac, p.read(f + 0x14));
  // --- ground snap (004FE3BA..004FE53A)
  if (get_area(c, p)) {
    U pos = c + 0x90;
    p.write(f + 0x38, p.read(pos)); p.write(f + 0x3c, p.read(pos + 4)); p.write(f + 0x40, p.read(pos + 8));
    p.write(f + 0x1c, 0);
    p.write(f + 0x40, p.call(0x4bc380, get_area(c, p), {pos, 1, f + 0x1c})); // ComputeHeight -> fstp [f+40]
    if (!p.read(f + 0x1c) && p.read(c + 0xa88)) {
      U id = p.read(c + 4);
      if (id != p.call(0x4aea40, server(p), {})) {
        U client = p.read(p.global(kAppManager) + 4);
        if (!p.read(p.call(0x5ed690, client, {}) + 0xb4) &&
            !p.read(p.call(0x5ed690, p.read(p.global(kAppManager) + 4), {}) + 0xc0)) {
          client = p.read(p.global(kAppManager) + 4);
          U cid = p.call(0x5eda50, client, {p.read(c + 4)});        // ServerToClientObjectId
          U index = party_index_006349c0(p.call(0x5ed8b0, client, {}), cid, p);
          if (index != 0xffffffffu) {
            U entry = p.call(0x5ed8b0, p.read(p.global(kAppManager) + 4), {}) + index * 0x88 + 0x24;
            if (entry) {
              U src = entry + 0x18;
              p.write(f + 0x38, p.read(src)); p.write(f + 0x3c, p.read(src + 4)); p.write(f + 0x40, p.read(src + 8));
              U area = get_area(c, p);
              if (p.call(0x4be860, area, {f + 0x38, 0x40a00000u, p.read(c + 0x340), 1, f + 0x38})) // ComputeSafeLocation
                p.write(f + 0x40, p.call(0x4bc380, get_area(c, p), {pos, 1, f + 0x1c}));
            }
          }
        }
      }
    }
    U mode = p.read(c + 0xa8c);
    if (mode == 4 || mode == 5) p.write(f + 0x40, x87::add_float(p.read(c + 0xa94), p.read(f + 0x40)));
    p.call(0x4cd1a0, c, {f + 0x38, 1, 0, 0});                       // 004FE53A SetPosition
  }
  compute_ai_state_004f1460(c, f + 0x04, p);                        // 004FE541 (its own [esp+4] slot)
  if (get_area(c, p)) mine_check::update_mine_check_004fa390(c, f + 0x1b8, p);   // 004FE553 UpdateMineCheck (own 0xC4 frame)
  // --- combat state timers (004FE558..004FE5C5)
  if (p.read(c + 0x4e0) == 1) {
    if (p.read8(c + 0xac0) == 1 && !(p.read8(c + 0xa00) & 1)) {
      U v = p.read(c + 0x9fc);
      p.write(c + 0x9fc, v & 0xfffffffeu);
      if (v & 1) p.call(0x50ee50, c, {0, 0});                       // SetStealthMode
    }
    U t = p.read(c + 0x4e4);
    if (sgt(t, 0)) p.write(c + 0x4e4, t - p.read(c + 0xcc));
    else p.call(0x4f2610, c, {0, p.read8(c + 0xac0)});              // SetCombatState
  }
  { U v = p.read(c + 0xab0);                                        // 004FE5C5..004FE5E7
    if (v) { U dt = p.read(c + 0xcc); p.write(c + 0xab0, dt < v ? v - dt : 0); } }
  if (p.read(c + 0x4e0) == 1) {                                     // 004FE5ED..004FE621
    if (!sgt(p.read(c + 0x52c), 0)) { p.vcall(c, 0x94, {}); p.write(c + 0x52c, 0xbb8); }
    p.write(c + 0x52c, p.read(c + 0x52c) - p.read(c + 0xcc));
  }
  // --- regeneration (004FE627..004FE891)
  if (!p.vcall(c, 0x94, {}) && !p.call(0x4ef890, c, {}) && p.read(c + 0xa88)) {
    U row = (!p.read(c + 0x4e0) || p.read8(c + 0xac0) == 2) ? 1 : 0;
    U table = p.read(p.read(p.global(kRules) + 0xb8) + 0xa4);
    p.write(f + 0x1c, 0);
    p.call(0x413350, table, {row, p.global(kRegenColumnHp), f + 0x1c}); // C2DA::GetFLOATEntry
    S maxhp = static_cast<S>(sx16(p.vcall(c, 0x98, {1})));
    p.write(f + 0x24, static_cast<U>(maxhp));
    U rate = p.read(f + 0x1c);
    bool use_min = x87::regen_below_minimum(rate, maxhp);
    S scale = maxhp;
    if (!use_min) { scale = static_cast<S>(sx16(p.vcall(c, 0x98, {1}))); p.write(f + 0x24, static_cast<U>(scale)); }
    U amount = x87::regen_amount(rate, scale, use_min, p.read(c + 0xcc));
    p.write(f + 0x24, amount);                                      // 004FE725 fstp [esp+28] (== f+24)
    U stats = p.read(c + 0xa74);
    S current = static_cast<S>(sx16(p.vcall(c, 0x9c, {1})));        // GetCurrentHitPoints
    S hp = current + static_cast<S>(sx16(adjust_hp_accumulator_005a5c70(stats, p.read(f + 0x24), p)));
    p.write(f + 0x24, static_cast<U>(hp));
    U hp_float = x87::int_to_float(hp);
    p.write(f + 0x24, hp_float);
    S max2 = static_cast<S>(sx16(p.vcall(c, 0x98, {1})));
    p.write(f + 0x34, static_cast<U>(max2));
    U value;
    if (x87::int_less_than_float(max2, hp_float)) {
      S max3 = static_cast<S>(sx16(p.vcall(c, 0x98, {1})));
      p.write(f + 0x34, static_cast<U>(max3));
      value = static_cast<U>(max3);                                 // fild/__ftol2 of an int is exact
    } else {
      value = x87::ftol_float(hp_float);
    }
    p.call(0x4cc280, c, {value});                                   // 004FE796 SetCurrentHitPoints
    U fp_bonus = p.read16(p.read(c + 0xa74) + 0x126);
    S fp_cap = static_cast<S>(sx16(p.call(0x4fd490, c, {}))) + static_cast<S>(sx16(fp_bonus));
    p.write(f + 0x34, static_cast<U>(fp_cap));
    if (static_cast<std::int16_t>(p.call(0x4fd490, c, {}) & 0xffff) > 0) {
      p.write(f + 0x24, 0);
      p.call(0x413350, table, {row, p.global(kRegenColumnFp), f + 0x24});
      U fp_rate = p.read(f + 0x24);
      bool fp_min = x87::regen_below_minimum(fp_rate, fp_cap);
      U fp_amount = x87::regen_amount(fp_rate, fp_cap, fp_min, p.read(c + 0xcc));
      p.write(f + 0x34, fp_amount);
      U st = p.read(c + 0xa74);
      U ax = adjust_fp_accumulator_005a5cc0(st, fp_amount, p);
      U cx = (p.read16(st + 0x126) + p.read16(st + 0x124)) & 0xffff;
      S value_fp = static_cast<S>(sx16(ax)) + static_cast<S>(sx16(cx));
      if (p.global(kDebugZeroForce)) { p.set_global(kDebugZeroForce, 0); value_fp = 0; }
      if (fp_cap < value_fp) value_fp = fp_cap;
      p.write16(p.read(c + 0xa74) + 0x124, static_cast<U>(value_fp) & 0xffff);
    }
  }
  // --- debug status dump (004FE895..004FE8C1); results discarded
  if (p.global(kDebugStatusDump)) {
    p.call(0x5e5670, c + 0x18, {});
    p.vcall(c, 0x9c, {0});
    p.vcall(c, 0x98, {1});
    p.call(0x4fd490, c, {});
  }
  // --- party leader probe (004FE8C6..004FE95C); only feeds the debug watch
  U leader = 0;
  U client = p.read(p.global(kAppManager) + 4);
  if (client && p.call(0x5ed8b0, client, {}) &&
      p.call(0x6346c0, p.call(0x5ed8b0, p.read(p.global(kAppManager) + 4), {}), {0})) {
    U character = p.call(0x6346c0, p.call(0x5ed8b0, p.read(p.global(kAppManager) + 4), {}), {0});
    leader = p.read(character + 4);
    if (leader) leader = p.call(0x4aea30, server(p), {leader});     // ClientToServerObjectId
  }
  if ((p.global(kDebugWatchA) || p.global(kDebugWatchB)) && p.read(c + 4) == leader) {
    U cl = p.read(p.global(kAppManager) + 4);
    if (p.call(0x5ed530, cl, {}))
      p.call(0x5e5670, p.call(0x5ed530, p.read(p.global(kAppManager) + 4), {}) + 0x38, {});
  }
  // --- misc countdowns (004FE961..004FE9CB)
  if (p.read(c + 0x534) == 1) {
    U v = p.read(c + 0x538);
    if (!sgt(v, 0)) { p.write(c + 0x534, 0); p.write(c + 0x538, 0); }
    else p.write(c + 0x538, v - p.read(c + 0xcc));
  }
  { U a = p.read(c + 0xd4);
    if (a == 0x2714 || a == 0x2767 || a == 0x2766) {
      U v = p.read(c + 0x530);
      if (!sgt(v, 0)) p.write(c + 0x530, 0x5dc); else p.write(c + 0x530, v - p.read(c + 0xcc));
    } }
  // --- paused action-queue refresh (004FE9D1..004FEA3B)
  if (p.read(c + 0x9d4) == 1) {
    U pause_timer = p.call(0x4aee00, p.read(f + 0x28), {});        // GetPauseTimer(server)
    p.call(0x4ade40, pause_timer, {f + 0x28, f + 0x24});            // day->f+28, ms->f+24
    p.call(0x4ae460, p.read(f + 0x2c),
           {p.read(f + 0x28), p.read(f + 0x24), p.read(c + 0x3a0), p.read(c + 0x3a4), f + 0x1c, f + 0x34});
    if (p.read(f + 0x34) >= 0x12c) {
      p.call(0x4f6f30, c, {});                                      // UpdateActionQueue
      p.write(c + 0x3a0, p.read(f + 0x28));
      p.write(c + 0x3a4, p.read(f + 0x24));
    }
  }
  // --- eight countdown slots (004FEA41..004FEA6C)
  for (U slot = c + 0x9a8, n = 8; n; --n, slot += 4) {
    U v = p.read(slot);
    if (!v) continue;
    U dt = p.read(c + 0xcc);
    p.write(slot, v > dt ? v - dt : 0);
  }
  if (p.global(kStealthTimerEnabled) == 1) {                          // 004FEA6E..004FEA96
    U v = p.read(c + 0x398), dt = p.read(c + 0xcc);
    p.write(c + 0x398, v > dt ? v - dt : 0x1f40);
  }
  if (p.read(c + 0x344) == 1) {                                     // 004FEAA0..004FEAB3
    p.call(0x5addc0, p.read(c + 0xa74), {});                        // UpdateCombatInformation
    p.write(c + 0x344, 0);
  }
  // --- AI level demotion (004FEAB9..004FEAF9)
  if (p.read(c + 0x4e0) == 1) {
    U lvl = p.read(c + 0x78);
    if (lvl == 1 || lvl == 0) p.call(0x4b08a0, p.read(f + 0x18), {c, 2});
  } else if (p.read(c + 0x78) == 2 && get_area(c, p)) {
    U level = p.read(get_area(c, p) + 0x128) == 0 ? 0 : 1;
    p.call(0x4b08a0, p.read(f + 0x18), {c, level});
  }
  // --- stealth XP (004FEAFE..004FEB7D)
  if (!get_area(c, p)) return;
  if (!p.read(get_area(c, p) + 0x2c0)) { p.write(c + 0xaa4, 0); return; }
  U v = p.read(c + 0xaa4);
  if (!v) return;
  U dt = p.read(c + 0xcc);
  if (v > dt && v != 0xffffffffu) { p.write(c + 0xaa4, v - dt); return; }
  U have = p.read(get_area(c, p) + 0x2b8), sub = p.read(get_area(c, p) + 0x2bc);
  U xp = have < sub ? 0 : have - p.read(get_area(c, p) + 0x2bc);
  set_stealth_xp_00506aa0(get_area(c, p), xp, p);
  p.write(c + 0xaa4, 0);
}
}
