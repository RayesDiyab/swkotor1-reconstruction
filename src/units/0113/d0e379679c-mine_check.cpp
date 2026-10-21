#include "8fe93ed262-mine_check.hpp"
#include "4a8ca74ca0-leaves.hpp"
#include "1a847f7684-w1_x87.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// 004FA390 CSWSCreature::UpdateMineCheck, 0058C8A0 CSWSTrigger::CalculateNearestPoint, 00550B60 CSWRules::RollDice.
namespace w1::mine_check {
namespace {
inline U sx8(U v) { return static_cast<U>(static_cast<S>(static_cast<signed char>(v & 0xff))); }
void read3(U a, U out[3], Ports& p) { out[0] = p.read(a); out[1] = p.read(a + 4); out[2] = p.read(a + 8); }
// Adds the detecting side to `list`: every party member when the creature is party-controlled, otherwise itself.
void mark_detected(U c, U list, U e, Ports& p) {
  if (p.read(e + 0x1c) == 1) {                                      // [esp+28] party-controlled flag ([creature+A88])
    U party = p.read(e + 4);
    if (!sgt(p.read(party), 0)) return;
    for (U i = 0; slt(i, p.read(p.read(e + 4))); ++i) {
      U character = p.call(0x6346c0, p.read(e + 4), {i});          // CSWParty::GetCharacter
      U id = p.call(0x4aea30, p.read(e + 0x10), {p.read(character + 4)});   // ClientToServerObjectId
      p.call(0x6449d0, list, {id});                                  // CExoArrayList<unsigned long>::AddUnique
    }
  } else {
    p.call(0x6449d0, list, {p.read(c + 4)});
  }
}
bool already_detected(U list, U id, Ports& p) {                     // counted scan (all entries visited)
  U n = p.read(list + 4), hits = 0;
  if (!sgt(n, 0)) return false;
  for (U cell = p.read(list); n; --n, cell += 4) if (p.read(cell) == id) ++hits;
  return hits != 0;
}
}
U roll_dice_00550b60(U count, U sides, Ports& p) {
  U total = 0;
  for (U n = count & 0xff; static_cast<S>(n) > 0; --n)             // 00550B62..00550B80
    total += static_cast<U>(static_cast<S>(p.call(0x6fc447, 0, {})) % static_cast<S>(sides & 0xff)) + 1;
  return total & 0xffff;                                            // mov ax, si
}
void nearest_point_0058c8a0(U t, U in, U out, U f, Ports& p) {
  p.write(f + 0x28, 0); p.write(f + 0x2c, 0); p.write(f + 0x30, 0);
  U r[3] = {0, 0, 0};
  if (p.read(t + 0x2bc)) {                                          // radial trigger
    p.write(f + 0x1c, 0); p.write(f + 0x20, 0); p.write(f + 0x24, 0);
    p.call(0x5e5a90, f + 0x10, {0x746f28});                          // CExoString("PrimaryRange")
    U table = p.read(p.read(p.global(kRules) + 0xb8) + 0xc4);
    U ok = p.call(0x413430, table, {0x11, f + 0x10, f + 8});         // C2DA::GetFLOATEntry(17, ...)
    p.call(0x5e5c20, f + 0x10, {});
    if (!ok) p.write(f + 8, 0x3f800000);                            // 1.0f
    U pin[3], pos[3], d[3];
    read3(in, pin, p); read3(t + 0x90, pos, p);
    if (x87::trigger_radial_far(pin, pos, d)) {
      p.write(f + 0x1c, d[0]); p.write(f + 0x20, d[1]); p.write(f + 0x24, d[2]);
      U n = p.call(0x4ab180, 0, {f + 0x58, f + 0x1c});              // normalize(out, in)
      U nv[3]; read3(n, nv, p);
      x87::trigger_radial_point(nv, p.read(f + 8), pos, r);
    } else {
      r[0] = pin[0]; r[1] = pin[1]; r[2] = pin[2];
    }
  } else {                                                          // polygon trigger: nearest point on the outline
    U n = p.read(t + 0x294);
    U best = 0x4cbebc20;                                            // 1.0e8f
    if (sgt(n, 0)) {
      U idx = p.read(t + 0x298), verts = p.read(t + 0x288);
      U pin[3]; read3(in, pin, p);
      for (U i = 0; slt(i, n); ++i) {
        U next = (i == n - 1) ? 0 : i + 1;
        U a[3], b[3];
        read3(verts + p.read(idx + i * 4) * 12, a, p);
        read3(verts + p.read(idx + next * 4) * 12, b, p);
        x87::trigger_edge_nearest(pin, a, b, best, r);
      }
    }
  }
  p.write(out, r[0]); p.write(out + 4, r[1]); p.write(out + 8, r[2]);
}
void update_mine_check_004fa390(U c, U e, Ports& p) {
  // e+N == [esp+N] after `push ebp` (004FA3A8).
  for (U o : {0x48u, 0x4cu, 0x50u, 0x3cu, 0x40u, 0x44u}) p.write(e + o, 0);
  U timer = p.call(0x4ae830, server(p), {p.read(c + 4)});
  p.call(0x4ade40, timer, {e + 0x28, e + 0x2c});
  p.call(0x4ae460, p.call(0x4aede0, server(p), {}), {p.read(e + 0x28), p.read(e + 0x2c), p.read(c + 0x374), p.read(c + 0x378), e + 0x38, e + 0x30});
  U mode = p.read8(c + 0x4d0);
  bool fast = (p.read(c + 0xd4) == 0x2710 && mode != 1) || mode != 0;   // 004FA434..004FA452
  U interval = fast ? 0x64 : 0xbb8;
  p.write(e + 0xc, fast ? 0x43c80000u : 0x41100000u);               // 400.0f / 9.0f (squared range)
  if (p.read(e + 0x30) < interval) return;                          // 004FA470..004FA474
  p.write(ai_master(p) + 0x78, 1);
  U area = p.call(0x4cb120, c, {});
  p.write(e + 0x34, area);
  if (area) {
    p.write(e + 0x10, server(p));
    p.write(e + 4, p.call(0x5ed8b0, p.read(p.global(kAppManager) + 4), {}));
    p.write(e + 0x3c, p.read(c + 0x90)); p.write(e + 0x40, p.read(c + 0x94)); p.write(e + 0x44, p.read(c + 0x98));
    p.write(e + 0x1c, p.read(c + 0xa88));
    p.write(e + 0x24, 0);
    if (sgt(p.read(area + 0x130), 0)) {
      U pos[3]; read3(e + 0x3c, pos, p);
      do {
        U obj = p.call(0x4ae750, p.read(e + 0x10), {p.read(p.read(area + 0x12c) + p.read(e + 0x24) * 4)});
        p.write(e + 0x18, obj);
        if (obj) {
          U kind = p.read(c + 0xd4), m = p.read8(c + 0x4d0);       // 004FA521..004FA580 detection roll
          bool searching = (kind == 0x2710 && m != 1) || m == 1 || m == 2;
          U roll = roll_dice_00550b60(1, 10, p);
          p.write(e + 8, searching ? roll + 0xa : roll);
          if (p.call(0x4cc2b0, c, {})) p.vcall(p.call(0x4cc2b0, c, {}), 0x2c, {});   // GetClientObject()->vtbl+2C
          U stats = p.read(c + 0xa74);
          if (p.vcall(obj, 0x40, {})) {                             // ---- trigger (004FA59D..004FA784)
            U trig = p.vcall(obj, 0x40, {});
            nearest_point_0058c8a0(trig, e + 0x3c, e + 0x48, e + 0x60, p);
            U list = trig + 0x2a8;
            p.write(e + 0x14, list);
            if (!already_detected(list, p.read(c + 4), p) && p.read(trig + 0x2c0) == 1) {
              U q[3]; read3(e + 0x48, q, p);
              if (x87::mine_within_trigger(pos, q, p.read(e + 0xc)) &&
                  !sge(p.call(0x57cb80, trig, {p.read(c + 4), 0}), 0x5a) &&
                  p.read(trig + 0x2b8) != p.read(p.read(c + 0xa74) + 0x78)) {
                U total = sx8(p.call(0x5aa570, p.read(c + 0xa74), {3, 0, 0})) + p.read(e + 8);
                if (!(slt(total, p.read(trig + 0x2d8)) && p.read(trig + 0x2c8) != 1)) {
                  if (!p.read(trig + 0x2c8)) {                      // first detection: broadcast
                    U mem = p.call(0x6fa7e6, 0, {0x34});
                    U msg = mem ? leaves::message_data_construct_004d69f0(mem, p) : 0;
                    p.call(0x4d7780, msg, {0, p.read(c + 4)});
                    p.call(0x4d7780, msg, {1, p.read(trig + 4)});
                    p.call(0x4d76b0, msg, {0, p.read(e + 8)});
                    p.call(0x4d76b0, msg, {1, sx8(p.call(0x5aa570, p.read(c + 0xa74), {3, 0, 0}))});
                    p.call(0x4d76b0, msg, {2, p.read(trig + 0x2d8)});
                    p.call(0x4ec840, c, {msg});                     // BroadcastMineDetectionData
                    if (msg) { leaves::message_data_destroy_004d6a40(msg, p); p.call(0x6fa390, 0, {msg}); }
                  }
                  mark_detected(c, p.read(e + 0x14), e, p);
                }
              }
            }
          }
          obj = p.read(e + 0x18);
          if (p.vcall(obj, 0x18, {})) {                             // ---- door (004FA789..004FA8D8)
            U door = p.vcall(obj, 0x18, {});
            U list = door + 0x2dc;
            if (!already_detected(list, p.read(c + 4), p) && p.read(door + 0x300) == 1) {
              U dp[3]; read3(door + 0x90, dp, p);
              if (x87::mine_within_object(pos, dp, p.read(e + 0xc)) &&
                  !sge(p.call(0x57cb80, p.vcall(obj, 0x18, {}), {p.read(c + 4), 0}), 0x5a) &&
                  p.read(p.vcall(obj, 0x18, {}) + 0x2b8) != p.read(p.read(c + 0xa74) + 0x78)) {
                U dc = p.read8(door + 0x2ed);
                U total = sx8(p.call(0x5aa570, p.read(c + 0xa74), {3, 0, 0})) + p.read(e + 8);
                if (!(slt(total, dc) && p.read8(door + 0x2f4) != 1)) mark_detected(c, list, e, p);
              }
            }
          }
          if (p.vcall(obj, 0x48, {})) {                             // ---- placeable (004FA8DC..004FAA26)
            U pl = p.vcall(obj, 0x48, {});
            U list = pl + 0x318;
            if (!already_detected(list, p.read(c + 4), p) && p.read(pl + 0x284) == 1) {
              U pp[3]; read3(pl + 0x90, pp, p);
              if (x87::mine_within_object(pos, pp, p.read(e + 0xc)) &&
                  !sge(p.call(0x57cb80, p.vcall(obj, 0x48, {}), {p.read(c + 4), 0}), 0x5a) &&
                  p.read(p.vcall(obj, 0x48, {}) + 0x23c) != p.read(p.read(c + 0xa74) + 0x78)) {
                U dc = p.read8(pl + 0x276);
                U total = sx8(p.call(0x5aa570, p.read(c + 0xa74), {3, 0, 0})) + p.read(e + 8);
                if (!(slt(total, dc) && p.read8(pl + 0x28c) != 1)) mark_detected(c, list, e, p);
              }
            }
          }
          (void)stats;
        }
        p.write(e + 0x24, p.read(e + 0x24) + 1);                    // 004FAA2B..004FAA40
      } while (slt(p.read(e + 0x24), p.read(p.read(e + 0x34) + 0x130)));
    }
  }
  p.write(c + 0x374, p.read(e + 0x28));                             // 004FAA48..004FAA56
  p.write(c + 0x378, p.read(e + 0x2c));
}
}
