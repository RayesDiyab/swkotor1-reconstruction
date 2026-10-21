#include "5ecaebb99c-perception.hpp"
#include "1a847f7684-w1_x87.hpp"
#include <cstring>
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// 00502AC0 CSWSCreature::DoPerceptionUpdateOnCreature and small leaves. Locals are kept in the original stack
// slots: h+N == [esp+N] after the last prologue push (push ebp at 00502C8C); the pre-ebp-push slots are h+4+N.
namespace w1::perception {
namespace {
inline float fl(U b) { float f; std::memcpy(&f, &b, 4); return f; }
inline U bits(float f) { U b; std::memcpy(&b, &f, 4); return b; }
// CScriptEvent perception notification (00503120..0050317E and its five siblings).
void perception_event(U c, U h, U target_id, U kind, Ports& p) {
  U mem = p.call(0x6fa7e6, 0, {0x34});                              // operator new(0x34)
  U ev = mem ? p.call(0x4d7540, mem, {}) : 0;                       // CScriptEvent::CScriptEvent
  p.write16(ev, 1);                                                 // mov word [edi], 1
  p.call(0x4d76b0, ev, {0, kind});                                  // SetInteger(0, kind)
  p.call(0x4d7780, ev, {0, target_id});                             // SetObjectID(0, target)
  p.call(0x4b08d0, p.read(h + 0x24), {0, 0, target_id, p.read(c + 4), 10, ev}); // AddEventDeltaTime(.., [007441DC]=10, ev)
}
}
U check_visible_005179e0(U c, U id, Ports& p) {
  U count = p.read(c + 0x924);
  if (!sgt(count, 0)) return 0;                                     // 005179EC
  U data = p.read(c + 0x920);
  for (U i = 0; slt(i, count); ++i)                                 // 005179F8..00517A04
    if (p.read(p.read(data + i * 4)) == id) return 1;
  return 0;
}
void add_visible_0051b370(U c, U id, U a2, U a3, U a4, U a5, Ports& p) {
  if (check_visible_005179e0(c, id, p)) return;                    // 0051B379..0051B380
  U e = p.call(0x6fa7e6, 0, {8});                                   // 0051B389 operator new(8)
  if (e) { p.write(e, kInvalidObject); p.write8(e + 4, p.read8(e + 4) & 0xe0); } // 0051B395..0051B3A1
  U b = (((((a5 & 1) << 2) | (a4 & 3)) << 1) | (a3 & 1)) & 0xff;    // 0051B3AA..0051B3C8
  b = ((b << 1) | (p.read8(e + 4) & 0xe0) | (a2 & 1)) & 0xff;       // 0051B3CA..0051B3D8
  U list = c + 0x920;
  p.write(e, id);                                                   // 0051B3E0
  p.write8(e + 4, b);                                               // 0051B3E2
  U cap = p.read(list + 8);
  if (p.read(list + 4) == cap) p.call(0x5e03c0, list, {cap ? cap + cap : 0x10}); // CExoArrayList::Allocate
  p.write(p.read(list) + p.read(list + 4) * 4, e);                  // 0051B402..0051B407
  p.write(list + 4, p.read(list + 4) + 1);                          // 0051B40A
}
U stealth_detection_00500c00(U c, U target, U los, U spot_out, U listen_out, U flag, Ports& p) {
  if (!target) { p.write(spot_out, 0); p.write(listen_out, 0); return 0; }       // 00500C0D..00500C20
  if (p.call(0x4cb120, target, {}) != p.call(0x4cb120, c, {})) {   // 00500C23..00500C52
    p.write(spot_out, 0); p.write(listen_out, 0); return 0;
  }
  U heard = p.call(0x4fb4b0, c, {target, flag});                    // 00500C5D DoListenDetection
  U seen = 0;
  if (los) seen = p.call(0x4f1fd0, c, {target, flag});              // 00500C72 DoSpotDetection
  p.write(listen_out, heard);                                       // 00500C87
  p.write(spot_out, seen);                                          // 00500C89
  return (seen || heard) ? 1 : 0;
}
U party_member_00634620(U party, U id, Ports& p) {
  U count = p.read(party);
  if (!sgt(count, 0)) return 0;                                     // 00634627
  for (U i = 0, slot = party + 0x24; slt(i, count); ++i, slot += 0x88)
    if (p.read(slot) == id) return 1;                               // 00634630..0063463D
  return 0;
}
U party_leader_00563a00(U table, U id, Ports& p) {
  if (id == kInvalidObject) return 0;                               // 00563A07
  for (U i = 0; slt(i, 9); ++i)                                     // 00563A0F..00563A1D
    if (p.read(table + 0xc + i * 4) == id) return p.read(table + 0xec) == i ? 1 : 0; // 00563A25..00563A2F
  return 0;
}
void do_perception_00502ac0(U c, U target, U h, Ports& p) {
  for (U o : {0x48u, 0x4cu, 0x50u, 0x3cu, 0x40u, 0x44u, 0x54u, 0x58u, 0x5cu}) p.write(h + 4 + o, 0); // 00502AE1..00502B21
  p.write(h + 4 + 0x18, 0); p.write(h + 4 + 0x1c, 0); p.write(h + 4 + 0xc, 0);                       // 00502B2F..00502B37
  p.write(h + 4 + 0x10, server(p));                                 // 00502B3B
  p.write(h + 4 + 0x20, ai_master(p));                              // 00502B3F..00502B51
  {
    U timer = p.call(0x4ae830, server(p), {kInvalidObject});        // 00502B60 GetActiveTimer(INVALID)
    p.call(0x4ade40, timer, {h + 4 + 0x24, h + 4 + 0x28});          // GetWorldTime(&day, &ms)
    U wt = p.call(0x4aede0, server(p), {});
    p.call(0x4ae460, wt, {p.read(h + 4 + 0x24), p.read(h + 4 + 0x28), p.read(c + 0x37c), p.read(c + 0x380),
                          h + 4 + 0x34, h + 4 + 0x30});             // 00502B9E SubtractWorldTimes
  }
  if (p.read(h + 4 + 0x30) >= 0x4e20) {                             // 00502BA3 perception roll refresh
    auto roll = [&](U mod) { return static_cast<U>(static_cast<S>(p.call(0x6fc447, 0, {})) % static_cast<S>(mod)); };
    if (p.read8(c + 0x4d1) == 1) {                                  // 00502BB1..00502BE1
      p.write8(c + 0x91c, (roll(10) + 0xb) & 0xff);
      p.write8(c + 0x91d, (roll(10) + 0xb) & 0xff);
    }
    U kind = p.read(c + 0xd4), mode = p.read8(c + 0x4d0);          // 00502BE7..00502C17
    U mod = (kind == 0x2710 || mode == 1 || mode == 2) ? 0x14 : 0xa;
    p.write8(c + 0x91e, (roll(mod) + 1) & 0xff);
    U r = roll(mod);
    p.write(c + 0x380, p.read(h + 4 + 0x28));                       // 00502C5C..00502C60
    p.write8(c + 0x91f, (r + 1) & 0xff);
    p.write(c + 0x37c, p.read(h + 4 + 0x24));                       // 00502C6E..00502C72
  }
  U area = p.call(0x4cb120, c, {});                                 // 00502C7A
  if (!area) return;                                                // 00502C83
  if (p.read(target + 4) == p.read(c + 4)) return;                  // 00502C91..00502C96
  U g = h + 0x70;                                                   // range-getter frames
  U spot = spot_distance_004efc70(c, g, p);
  U listen = listen_distance_004efd20(c, g, p);
  p.write(h + 0x18, (fl(listen) <= fl(spot)) ? spot_distance_004efc70(c, g, p) : listen_distance_004efd20(c, g, p));
  U tid = p.read(target + 4);                                       // ebx
  p.write(h + 0x4c, p.read(c + 0x90)); p.write(h + 0x50, p.read(c + 0x94));
  p.write(h + 0x54, bits(fl(p.read(c + 0x98)) + 1.5f));            // + [0073F414] = 1.5f
  p.write(h + 0x40, p.read(target + 0x90)); p.write(h + 0x44, p.read(target + 0x94));
  p.write(h + 0x48, bits(fl(p.read(target + 0x98)) + 1.5f));
  p.write(h + 0x30, tid);
  U los;
  if (p.call(0x4cb120, target, {}) != area) {
    los = 0;                                                        // 00502D2D -> 00502EB2
  } else {
    if (p.read(c + 4) == p.call(0x4aea40, p.read(h + 0x14), {})) {  // 00502D34..00502D42
      los = 1;
    } else if (!x87::perception_range_open(p.read(h + 0x4c), p.read(h + 0x50), p.read(h + 0x54),
                                           p.read(h + 0x40), p.read(h + 0x44), p.read(h + 0x48), p.read(h + 0x18))) {
      los = 0;                                                      // 00502D8A
    } else {
      U r = p.call(0x50c330, area, {h + 0x4c, h + 0x40, h + 0x58, h + 0x3c, p.read(c + 4), tid, 1, 0}); // ClearLineOfSight
      los = r == 1 ? 1 : 0;                                         // 00502DB2..00502DB9
    }
    U module = p.call(0x4ae6b0, server(p), {});                     // 00502DC3 GetModule
    p.write(h + 0x14, module);
    p.write(h + 0x18, 0);
    if (sgt(p.read(module + 0x140), 0)) {                           // 00502DDC..00502E63 player in this area?
      for (;;) {
        U id = p.read(p.read(p.read(h + 0x14) + 0x13c) + p.read(h + 0x18) * 4);
        U player = p.call(0x4ae770, server(p), {id});
        if (player && p.call(0x4cb120, c, {}) && p.call(0x4cb120, player, {})) {
          U mine = p.read(p.call(0x4cb120, c, {}) + 0x120);
          U theirs = p.read(p.call(0x4cb120, player, {}) + 0x120);
          if (mine == theirs) { p.write(h + 0x10, 1); break; }
        }
        U i = p.read(h + 0x18) + 1;
        p.write(h + 0x18, i);
        if (!slt(i, p.read(p.read(h + 0x14) + 0x140))) break;
      }
    }
    U flags = p.read8(c + 0x8ec);                                   // 00502E6B..00502EAE
    bool forced = (flags & 0x10) || ((flags & 8) && !(flags & 2));
    if (forced) p.write(h + 0x10, 1);
    else if (p.read(h + 0x10) == 1)
      p.write(h + 0x10, p.call(0x501950, target, {p.vcall(c, 0x10, {})}) & 1);   // GetInvisible
  }
  // ---- 00502EB2: existing visible-list element
  U e = visible_element_00517a20(c, tid, p);
  if (e) {
    U v = p.read(h + 0x10);
    if (v != ((p.read8(e + 4) >> 4) & 1)) {                         // 00502EC4..00502EF8
      if (v == 1) { p.call(0x4f61d0, c, {target}); v = p.read(h + 0x10); }      // ClearHostileActionsVersus
      U b = p.read8(e + 4);
      p.write8(e + 4, b ^ ((((v << 4) & 0xff) ^ b) & 0x10));
    }
    U al = p.read8(e + 4);
    p.write(h + 0x14, (al >> 1) & 1);
    p.write(h + 0x18, al & 1);
    if (los) {                                                      // 00502F13..00502FD4
      U cl = al & 1;
      if (!cl) {
        stealth_detection_00500c00(c, target, los, h + 0x18, h + 0x14, p.read(h + 0x10), p); // 00502F41
        al = p.read8(e + 4);
        if (!(al & 2) && p.read(h + 0x14)) p.write(h + 0x1c, 1);    // 00502F49..00502F55
        cl = al & 1;
        if (!cl && p.read(h + 0x18)) p.write(h + 0x1c, 1);          // 00502F5D..00502F6C
      }
      if (cl && p.read(h + 0x10)) { p.write(h + 0x18, 0); p.write(h + 0x20, 1); } // 00502F74..00502F88
      U heard = (al >> 1) & 1;
      if (heard && p.read(target + 0x8c8)) { p.write(h + 0x14, 0); p.write(h + 0x20, 1); } // 00502F90..00502FAC
      if (p.read(h + 0x10) && (cl || heard)) p.write(h + 0x20, 1);  // 00502FB4..00502FCC
    } else {                                                        // 00502FD9..0050301D
      if (al & 1) { p.write(h + 0x18, 0); p.write(h + 0x20, 1); }
      stealth_detection_00500c00(c, target, 0, h + 0x18, h + 0x14, p.read(h + 0x10), p);
      if (p.read(h + 0x14)) p.write(h + 0x1c, 1); else p.write(h + 0x20, 1);
    }
  } else if (los) {                                                 // 0050301F..00503047
    stealth_detection_00500c00(c, target, los, h + 0x18, h + 0x14, p.read(h + 0x10), p);
    if (p.read(h + 0x18) || p.read(h + 0x14)) p.write(h + 0x1c, 1);
  } else {                                                          // 00503049..0050306E
    stealth_detection_00500c00(c, target, 0, h + 0x18, h + 0x14, p.read(h + 0x10), p);
    if (p.read(h + 0x14)) p.write(h + 0x1c, 1);
  }
  // ---- 00503076: party members always perceive each other
  p.write(h + 0x30, p.call(0x5eda50, p.read(p.global(kAppManager) + 4), {tid}));
  U self_client = p.call(0x5eda50, p.read(p.global(kAppManager) + 4), {p.read(c + 4)});
  U party = p.call(0x5ed8b0, p.read(p.global(kAppManager) + 4), {});
  if (party && party_member_00634620(party, self_client, p) && party_member_00634620(party, p.read(h + 0x30), p)) {
    p.write(h + 0x10, 0); p.write(h + 0x18, 1); p.write(h + 0x14, 1); p.write(h + 0x1c, 1); // 005030CC..005030E1
  } else if (p.read(h + 0x20)) {                                    // 005030EA..00503229 perception lost
    U el = visible_element_00517a20(c, tid, p);
    if (el) {
      if ((p.read8(el + 4) & 2) && !p.read(h + 0x14) && !p.read(c + 0xaa0)) perception_event(c, h, tid, 1, p);
      if ((p.read8(el + 4) & 1) && !p.read(h + 0x18) && !p.read(c + 0xaa0)) perception_event(c, h, tid, 3, p);
      U b = ((((p.read(h + 0x14) & 1) << 1) | (p.read8(el + 4) & 0xfc)) | (p.read(h + 0x18) & 1)) & 0xff;
      p.write8(el + 4, b);                                          // 005031FE..00503219
      if (!(b & 3) && !p.read(h + 0x10)) remove_visible_00517960(c, tid, p);
    }
  }
  // ---- 0050322E: perception gained
  U gained = p.read(h + 0x1c);
  p.write(h + 0x30, 0);
  if (!gained) return;
  U el = visible_element_00517a20(c, tid, p);
  if (!el) {                                                        // 00503254..0050335E
    add_visible_0051b370(c, tid, p.read(h + 0x18), p.read(h + 0x14), 0, p.read(h + 0x10), p);
    if (p.read(h + 0x14) && !p.read(c + 0xaa0)) perception_event(c, h, tid, 0, p);
    if (!p.read(h + 0x18) || p.read(c + 0xaa0)) return;
    perception_event(c, h, tid, 2, p);
  } else {                                                          // 00503363..00503477
    U al = p.read8(el + 4);
    if (!(al & 1) && p.read(h + 0x18)) {
      p.write8(el + 4, al | 1);
      p.write(h + 0x30, 1);
      if (!p.read(c + 0xaa0)) perception_event(c, h, tid, 2, p);
    }
    al = p.read8(el + 4);
    if (!(al & 2) && p.read(h + 0x14)) {
      p.write8(el + 4, al | 2);
      if (!p.read(c + 0xaa0)) perception_event(c, h, tid, 0, p);
    }
    if (!p.read(h + 0x30)) return;
  }
  // ---- 00503479: seen a player-side creature -> stealth XP hold
  if (!p.read(p.read(target + 0xa74) + 0x6c)) {
    U table = server_internal(p) + 0x1b770;                         // GetPartyTable (inline accessor 004AEE70)
    if (!party_leader_00563a00(table, p.read(target + 4), p)) return;
  }
  p.write(c + 0xaa4, 0x1770);
}
}
