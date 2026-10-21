#include "5ecaebb99c-perception.hpp"
#include "1a847f7684-w1_x87.hpp"
#include <cstring>
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
namespace w1::perception {
namespace {
U range(U c, U f, U field, U column_literal, Ports& p) {
  p.write(f + 4, p.read(c + field));                                // 004EFC8B..004EFC91 default from creature
  if (!p.read(c + 0x4e0)) return p.read(f + 4);                     // 004EFC95..004EFC9D
  U srv = server(p);
  if (p.call(0x4ae770, srv, {p.call(0x4aea40, srv, {})}) == c) return p.read(f + 4); // 004EFC9F..004EFCBB player
  p.call(0x5e5a90, f + 8, {column_literal});                         // CExoString("PrimaryRange"/"SecondaryRange")
  U table = p.read(p.read(p.global(kRules) + 0xb8) + 0xc4);
  p.call(0x413430, table, {0x12, f + 8, f + 4});                    // C2DA::GetFLOATEntry(row 18, column, &out)
  p.call(0x5e5c20, f + 8, {});
  return p.read(f + 4);                                             // fld dword [esp+4]
}
inline float fl(U b) { float f; std::memcpy(&f, &b, 4); return f; }
inline U bits(float f) { U b; std::memcpy(&b, &f, 4); return b; }
}
U spot_distance_004efc70(U c, U f, Ports& p) { return range(c, f, 0x914, 0x746f28, p); }
U listen_distance_004efd20(U c, U f, Ports& p) { return range(c, f, 0x918, 0x746f38, p); }
U visible_element_00517a20(U c, U id, Ports& p) {
  U count = p.read(c + 0x924);
  if (!sgt(count, 0)) return 0;                                     // 00517A2D
  U data = p.read(c + 0x920);
  for (U i = 0; slt(i, count); ++i)                                 // 00517A40..00517A4C
    if (p.read(p.read(data + i * 4)) == id) return p.read(data + i * 4);
  return 0;
}
void remove_visible_00517960(U c, U id, Ports& p) {
  if (!sgt(p.read(c + 0x924), 0)) return;                           // 0051796B
  for (U i = p.read(c + 0x924) - 1; sge(i, 0); --i) {               // 0051796E..005179CA
    U element = p.read(p.read(c + 0x920) + i * 4);
    if (p.read(element) != id) continue;
    U count = p.read(c + 0x924) - 1;
    p.write(c + 0x924, count);
    for (U k = i; slt(k, count); ) {                                // 005179A2..005179BA
      U data = p.read(c + 0x920);
      p.write(data + k * 4, p.read(data + k * 4 + 4));
      ++k;
      if (!slt(k, p.read(c + 0x924))) break;
    }
    if (element) p.call(0x6fa390, 0, {element});                    // 005179C0 free
  }
}
void update_visible_0051b050(U c, U f, Ports& p) {
  const U mode = p.read8(f + 0x2c);
  U area = p.call(0x4cb120, c, {});
  p.write(f + 0x18, area);
  if (!area) { p.write(c + 0xaa0, 0); return; }                     // 0051B066 -> 0051B34E
  U srv = server(p);
  p.write(f + 0x14, srv);
  (void)p.read(server_internal(p) + 0x1005c);                       // 0051B07A GetObjectArray (unused)
  p.write(ai_master(p) + 0x70, 1);                                  // 0051B081..0051B086
  if (!p.read(c + 0x9d4) && p.vcall(c, 0x94, {}) == 1) return;      // 0051B08D..0051B0A4
  U g = f + 0x40;
  U spot = spot_distance_004efc70(c, g, p);
  U listen = listen_distance_004efd20(c, g, p);
  U r = (fl(listen) <= fl(spot)) ? spot_distance_004efc70(c, g, p) : listen_distance_004efd20(c, g, p); // 0051B0BC..0051B0D0
  p.write(f + 0x10, r);
  U r2 = bits(fl(r) * fl(r));                                       // 0051B0DF..0051B0E7 (single product)
  p.write(f + 0x20, r2);
  auto within = [&](U other) {
    return x87::within_squared(p.read(c + 0x90), p.read(c + 0x94), p.read(c + 0x98),
                               p.read(other + 0x90), p.read(other + 0x94), p.read(other + 0x98), p.read(f + 0x20));
  };
  if (mode == 1) {                                                  // party members only
    if (p.read(c + 0xa88)) { p.write(c + 0xaa0, 0); return; }        // 0051B0F1..0051B0F9
    U party = p.call(0x5ed8b0, p.read(p.global(kAppManager) + 4), {});
    p.write(f + 0x18, party);
    if (!party) { p.write(c + 0xaa0, 0); return; }
    p.write(f + 0x2c, 0);
    if (!sgt(p.read(party), 0)) { p.write(c + 0xaa0, 0); return; }   // 0051B118..0051B123
    U slot = party + 0x24;
    for (U i = 0;;) {
      U id = p.call(0x4aea30, p.read(f + 0x14), {p.read(slot)});    // ClientToServerObjectId
      U other = p.call(0x4ae770, p.read(f + 0x14), {id});
      if (other) {
        bool known = visible_element_00517a20(c, id, p) != 0;       // 0051B176
        if (known || within(other)) p.call(0x502ac0, c, {other});   // DoPerceptionUpdateOnCreature (own frame)
      }
      i = p.read(f + 0x2c) + 1; slot += 0x88; p.write(f + 0x2c, i);
      if (!slt(i, p.read(p.read(f + 0x18)))) break;                 // 0051B1B0..0051B1C7
    }
    p.write(c + 0xaa0, 0);
    return;
  }
  auto wanted = [&](U other) { return mode == 0 || (mode == 2 && !p.read(other + 0xa88)); };
  U count = p.read(c + 0x924);                                      // 0051B1E1..0051B238 existing visible list
  if (sgt(count, 0)) {
    for (U k = count - 1; sge(k, 0); --k) {
      U id = p.read(p.read(p.read(c + 0x920) + k * 4));
      if (id == p.read(c + 4)) continue;
      U other = p.call(0x4ae770, srv, {id});
      if (!other) { remove_visible_00517960(c, id, p); continue; }
      if (wanted(other)) p.call(0x502ac0, c, {other});
    }
    area = p.read(f + 0x18);
  }
  p.write(f + 0x24, bits(fl(p.read(c + 0x90)) - fl(p.read(f + 0x10))));   // 0051B23E..0051B24C minX
  p.write(f + 0x10, bits(fl(r) + fl(p.read(c + 0x90))));                  // 0051B250..0051B262 maxX (reuses slot)
  if (!p.call(0x506f20, area, {f + 0x1c, p.read(f + 0x24)})) return;       // GetFirstObjectIndiceByX
  for (U index = p.read(f + 0x1c); slt(index, p.read(area + 0x194)); ) {   // 0051B273..0051B348
    U id = p.read(p.read(area + 0x190) + index * 4);
    if (id != p.read(c + 4)) {
      U other = p.call(0x4ae770, p.read(f + 0x14), {id});
      if (other) {
        if (fl(p.read(other + 0x90)) > fl(p.read(f + 0x10))) break;      // 0051B2A9..0051B2B8: exits only on ordered x > maxX (NaN continues)
        bool known = visible_element_00517a20(c, id, p) != 0;
        if (!known && within(other) && wanted(other)) p.call(0x502ac0, c, {other});
      }
      area = p.read(f + 0x18);
    }
    index = p.read(f + 0x1c) + 1;
    p.write(f + 0x1c, index);
  }
  p.write(c + 0xaa0, 0);
}
}
