#include "214484301a-nearest.hpp"
#include "1a847f7684-w1_x87.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// 004FC4C0 CSWSCreature::GetNearestObjects and 004F2C30 CSWSCreature::ValidNearestObjectType.
namespace w1::nearest {
namespace {
constexpr U kBehind = 0xc0800000u;     // [007471BC] -4.0f
constexpr U kZero = 0;                 // [0073D700] 0.0f
constexpr U kFull = 0x43b40000u;       // [00741868] 360.0f
constexpr U kLimit = 0x43b38000u;      // [007471B8] 359.0f
constexpr double kCone = 0.866;        // [007471B0] qword 3FEBB645A1CAC083
constexpr U kTypeTrigger = 7, kTypePlaceable = 9, kTypeDoor = 10;   // jump table 004F2DBC indexed by (type - 5)
x87::X80 rx(U a, Ports& p) { x87::X80 r; for (U i = 0; i < 10; ++i) r.b[i] = static_cast<unsigned char>(p.read8(a + i)); return r; }
bool bitsf_less(U a, U b) { return x87::x80_less_than_float(x87::x80_from_float(a), b); }   // fld dword; fcomp dword (C0 alone)
void append_word(U list, U value, Ports& p) {                      // inlined CExoArrayList<T>::Add
  U alloc = p.read(list + 8);
  if (p.read(list + 4) == alloc) p.call(0x5e03c0, list, {alloc ? alloc * 2 : 0x10});
  p.write(p.read(list) + p.read(list + 4) * 4, value);
  p.write(list + 4, p.read(list + 4) + 1);
}
// One candidate (identical in the backward 004FC6CE..004FCA6C and forward 004FCABB..004FCE68 halves).
void consider(U self, U id, U range_flagged, U range_other, U cone, U all, U e, Ports& p) {
  U obj = p.call(0x4ae750, server(p), {id});                       // CServerExoApp::GetGameObject
  if (!valid_nearest_object_type_004f2c30(self, obj, e + 0x58, p)) return;
  U pos = p.vcall(obj, 0x10, {}) + 0x90;
  U d[3] = {x87::sub_float(p.read(pos), p.read(self + 0x90)), x87::sub_float(p.read(pos + 4), p.read(self + 0x94)),
            x87::sub_float(p.read(pos + 8), p.read(self + 0x98))};
  p.write(e + 0x14, d[0]); p.write(e + 0x18, d[1]); p.write(e + 0x1c, d[2]);
  U dist2;
  if (!x87::nearest_in_range(d[0], d[1], d[2], p.read(e + 0x58) ? range_flagged : range_other, dist2)) return;
  p.write(e + 0x10, dist2);
  p.call_st0(0x4aa0f0, 0, {e + 0x14}, e + 0xb0);                   // Yaw(&d) -> ST0
  x87::X80 yaw = rx(e + 0xb0, p);
  U angle;
  x87::X80 rel = x87::nearest_wrap_angle(x87::x80_sub_float(yaw, p.read(e + 0x7c)), kZero, kFull, kLimit, angle);
  p.write(e + 0x20, angle);
  // all: sorted by relative yaw (strictly-less search on the extended value), parallel float lists D (angle) / E (dist2)
  U n = p.read(e + 0x38), i = 0;
  while (slt(i, n) && !x87::x80_less_than_float(rel, p.read(p.read(e + 0x34) + i * 4))) ++i;
  if (slt(i, n)) {
    float_list_insert_004f78c0(e + 0x34, angle, i, p);
    float_list_insert_004f78c0(e + 0x40, dist2, i, p);
    p.call(0x616940, all, {id, i});                                 // CExoArrayList<pointer>::Insert
  } else {
    append_word(e + 0x34, angle, p); append_word(e + 0x40, dist2, p); append_word(all, id, p);
  }
  // cone: in front of the creature and inside the 0.866 cosine cone seen from 4 m behind it, sorted by dist2
  U orient[3] = {p.read(self + 0x9c), p.read(self + 0xa0), p.read(self + 0xa4)};
  U raw[3] = {d[0], d[1], d[2]};
  if ((x87::dot_status_float(raw, orient, kZero) >> 8) & 1) return;  // 004FC922..004FC92D (below zero or unordered)
  pos = p.vcall(obj, 0x10, {}) + 0x90;
  p.write(e + 0x14, x87::sub_float(p.read(pos), p.read(e + 0xac)));
  p.write(e + 0x18, x87::sub_float(p.read(pos + 4), p.read(e + 0x78)));
  p.write(e + 0x1c, x87::sub_float(p.read(pos + 8), p.read(e + 0x74)));
  p.call(0x4ab130, e + 0x14, {});                                  // Vector::Normalize
  U nd[3] = {p.read(e + 0x14), p.read(e + 0x18), p.read(e + 0x1c)};
  if ((x87::dot_status_double(nd, orient, kCone) >> 8) & 0x41) return;   // 004FC9B1..004FC9BC
  n = p.read(e + 0x2c); i = 0;
  while (slt(i, n) && !(bitsf_less(dist2, p.read(p.read(e + 0x28) + i * 4)))) ++i;
  if (slt(i, n)) {
    float_list_insert_004f78c0(e + 0x28, dist2, i, p);
    p.call(0x616940, cone, {id, i});
  } else {
    append_word(e + 0x28, dist2, p); append_word(cone, id, p);
  }
}
}
void float_list_insert_004f78c0(U list, U value, U index, Ports& p) {
  U alloc = p.read(list + 8), old = p.read(list + 4);
  if (old == alloc) p.call(0x5e03c0, list, {alloc ? alloc * 2 : 0x10});
  p.write(p.read(list) + p.read(list + 4) * 4, x87::load_store_float(value));
  p.write(list + 4, p.read(list + 4) + 1);
  for (U i = old; sgt(i, index); --i) p.write(p.read(list) + i * 4, p.read(p.read(list) + i * 4 - 4));
  p.write(p.read(list) + index * 4, x87::load_store_float(value));
}
U valid_nearest_object_type_004f2c30(U self, U obj, U out, Ports& p) {
  if (!obj) return 0;
  p.write(out, 0);
  switch (p.read8(obj + 8)) {
  case type::creature: {                                            // 004F2C63
    U c = p.vcall(obj, 0x30, {});
    if (p.vcall(c, 0x94, {})) return 0;
    if (p.call(0x4ef890, c, {})) return 0;                          // GetDeadTemp
    U el = p.call(0x517a20, self, {p.read(c + 4)});                 // GetVisibleListElement
    if (!el || !(p.read8(el + 4) & 1))
      if (!p.call(0x4f1fd0, self, {c, 0})) return 0;                // DoSpotDetection
    if ((p.call(0x57cd90, self, {p.read(c + 4)}) & 0xff) != 2) return 1;   // GetAIStateReputation: hostile only flags
    p.write(out, 1);
    return 1;
  }
  case kTypePlaceable: {                                            // 004F2CD5
    U pl = p.vcall(obj, 0x48, {});
    if (!pl || !p.read(pl + 0x328)) return 0;
    if (p.read(pl + 0x340) && sle(p.call(0x57cb80, pl, {p.read(self + 4), 0}), 10)) { p.write(out, 1); return 1; }
    p.write(out, 0);
    return 1;
  }
  case kTypeDoor: {                                                 // 004F2D2E
    U d = p.vcall(obj, 0x18, {});
    if (!d || p.read8(d + 0x2cc) || p.read(d + 0x3c0)) return 0;
    return 1;
  }
  case kTypeTrigger: {                                              // 004F2D59
    U t = p.vcall(obj, 0x40, {});
    if (!t || !p.read(t + 0x2bc)) return 0;
    U id = p.read(self + 4);
    if (p.call(0x4af240, t + 0x2a8, {id})) return 1;                // CExoArrayList::Count (contains)
    if (sge(p.call(0x57cb80, t, {id, 0}), 0x5a)) return 1;
    return p.read(t + 0x2b8) == p.read(p.read(self + 0xa74) + 0x78) ? 1u : 0u;
  }
  default: return 0;                                                // 004F2DAE (item, type 8, others)
  }
}
void get_nearest_objects_004fc4c0(U self, U range_flagged, U range_other, U cone, U all, U e, Ports& p) {
  p.write(e + 0x24, self);
  U area = p.call(0x4cb120, self, {});                              // CSWSObject::GetArea
  p.write(e + 0x80, area);
  U n = p.read(area + 0x194), idx = 0;
  if (sgt(n, 0)) {
    U list = p.read(area + 0x190);
    while (p.read(list + idx * 4) != p.read(self + 4)) { ++idx; if (!slt(idx, n)) break; }
  }
  p.write(e + 0x4c, idx);
  if (U data = p.read(all)) { p.call(0x6fa390, 0, {data}); p.write(all, 0); p.write(all + 8, 0); }
  p.write(all + 4, 0);
  if (sge(idx, p.read(area + 0x194))) return;
  for (U o : {0x28u, 0x2cu, 0x30u, 0x34u, 0x38u, 0x3cu, 0x40u, 0x44u, 0x48u, 0x14u, 0x18u, 0x1cu}) p.write(e + o, 0);
  p.call(0x4ab180, 0, {e + 0xa0, self + 0x9c});                    // normalize(out, &orientation)
  U o[3] = {p.read(e + 0xa0), p.read(e + 0xa4), p.read(e + 0xa8)}, pos[3] = {p.read(self + 0x90), p.read(self + 0x94), p.read(self + 0x98)}, b[3];
  x87::nearest_behind_point(o, pos, kBehind, b);
  p.write(e + 0xac, b[0]); p.write(e + 0x78, b[1]); p.write(e + 0x74, b[2]);
  p.call_st0(0x4aa0f0, 0, {e + 0xa0}, e + 0xb0);                   // Yaw(&o) -> ST0, fstp dword
  x87::X80 yaw = rx(e + 0xb0, p);
  p.write(e + 0x7c, x87::x80_to_float(yaw));
  p.write(e + 0x84, 1); p.write(e + 0x54, 1);
  for (U k = 1;; ++k) {
    p.write(e + 0x50, k);
    if (p.read(e + 0x84)) {                                         // backward
      U j = idx - k;
      if (static_cast<S>(j) < 0) p.write(e + 0x84, 0);
      else if (U id = p.read(p.read(area + 0x190) + j * 4); id != p.read(self + 4)) consider(self, id, range_flagged, range_other, cone, all, e, p);
    } else if (!p.read(e + 0x54)) {
      break;
    }
    if (!p.read(e + 0x54)) continue;                                // 004FCA79
    U j = k + idx;                                                  // forward
    if (!slt(j, p.read(area + 0x194))) { p.write(e + 0x54, 0); continue; }
    if (U id = p.read(p.read(area + 0x190) + j * 4); id != p.read(self + 4)) consider(self, id, range_flagged, range_other, cone, all, e, p);
  }
  p.call(0x6fa390, 0, {p.read(e + 0x28)});                         // locals' destructors
  p.call(0x6fa390, 0, {p.read(e + 0x40)});
  p.call(0x6fa390, 0, {p.read(e + 0x34)});
}
}
