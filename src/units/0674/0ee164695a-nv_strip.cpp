// nv_strip.cpp - lane-3d-model rev2: NvTriStrip members reached from AurStripify 00481140 (never executed at
// runtime: 007A69A8 has no writer; link-required). x87 intermediates as double (D30); EH unwind states cited.
// Inlined helpers of the original (NvStripInfo::IsInStrip / IsMarked / MarkTriangle, NvStripifier::FindOtherFace,
// VertexCache::InCache / AddEntry, vector::push_back) appear as local functions; the out-of-line copies keep their
// original addresses.
#include "3abdd0f0f1-nv_strip.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>

using namespace aurora_model;
using nv::Vec;
using nv::FaceInfo;
using nv::EdgeInfo;
using nv::StripInfo;
using nv::StripStartInfo;
using nv::Stripifier;
using nv::VertexCache;

namespace {
U count32(const Vec* v) { return v->first ? static_cast<U>(static_cast<I>(v->last - v->first) >> 2) : 0; }
U cap32(const Vec* v) { return v->first ? static_cast<U>(static_cast<I>(v->end - v->first) >> 2) : 0; }
template <class T> T* at32(const Vec* v, U i) { return ptr<T>(ptr<U>(v->first)[i]); }
// inlined vector<T*>::push_back: construct in place (0047C1A0) or _Insert_n(end(), 1, value) (0047DE10).
void push_back_inline(Vec* v, const U* value, U tag) {
    if (v->first != 0 && count32(v) < cap32(v)) {
        U* const at_end = ptr<U>(v->last);
        nv_ptr_uninit_fill_n_0047c1a0(at_end, 1, value, v, tag);
        v->last = addr(at_end + 1);
    } else {
        nv_ptr_vec_insert_n_0047de10(v, nullptr, ptr<U>(v->last), 1, value);
    }
}
// inlined NvStripInfo::IsInStrip.
bool in_strip(const StripInfo* s, const FaceInfo* f) {
    if (f == nullptr) return false;
    return s->experiment_id >= 0 ? f->test_strip_id == s->strip_id : f->strip_id == s->strip_id;
}
// inlined NvStripInfo::IsMarked.
bool is_marked(const StripInfo* s, const FaceInfo* f) {
    return f->strip_id >= 0 || (s->experiment_id >= 0 && f->experiment_id == s->experiment_id);
}
// inlined NvStripInfo::MarkTriangle.
void mark_triangle(const StripInfo* s, FaceInfo* f) {
    if (s->experiment_id >= 0) { f->experiment_id = s->experiment_id; f->test_strip_id = s->strip_id; }
    else { f->experiment_id = -1; f->strip_id = s->strip_id; }
}
// inlined FindEdgeInfo walk (FindStartPoint keeps it inline).
EdgeInfo* edge_walk(const Vec* edges, I v0, I v1) {
    for (EdgeInfo* e = at32<EdgeInfo>(edges, static_cast<U>(v0)); e != nullptr;) {
        if (e->v0 == v0) { if (e->v1 == v1) return e; e = e->next_v0; }
        else { if (e->v0 == v1) return e; e = e->next_v1; }
    }
    return nullptr;
}
// inlined FindOtherFace tail: the edge is dereferenced unconditionally (assert only in the original source).
FaceInfo* other_face(const EdgeInfo* e, const FaceInfo* f) { return e->face0 == f ? e->face1 : e->face0; }
// inlined VertexCache::InCache / AddEntry.
bool in_cache(const VertexCache* c, I entry) {
    for (I i = 0; i < c->num_entries; ++i) if (c->entries[i] == entry) return true;
    return false;
}
void add_entry(VertexCache* c, I entry) {
    for (I i = c->num_entries - 2; i >= 0; --i) c->entries[i + 1] = c->entries[i];
    c->entries[0] = entry;
}
}

// ======================================================================= vector<unsigned short> = / _Buy
// 0047DDC0 vector<unsigned short>::_Buy(count): first = last = end = 0; 0 -> false; > max -> _Xlen; else allocate.
U8 __fastcall nv_ushort_vec_buy_0047ddc0(Vec* self, void*, U count) {
    self->first = 0; self->last = 0; self->end = 0;                                 // 0047DDC7..0047DDD1
    if (count == 0) return 0;                                                       // 0047DDD4..0047DDD9
    if (count > 0x7FFFFFFFu) { nv_vector_xlen_0047d2b0(self, nullptr); }           // 0047DDDC..0047DDE5
    const U block = addr(live_operator_new(count * 2));                             // 0047DDF0..0047DDF5
    self->end = block + count * 2;                                                  // 0047DDFA..0047DDFF
    self->first = block;                                                            // 0047DE02
    self->last = block;                                                             // 0047DE05
    return 1;
}
// 0047E470 vector<unsigned short>::operator=(right).
Vec* __fastcall nv_ushort_vec_assign_0047e470(Vec* self, void*, const Vec* right) {
    if (self == right) return self;                                                 // 0047E47A..0047E47C
    const U rsize = right->first ? static_cast<U>(static_cast<I>(right->last - right->first) >> 1) : 0; // 0047E482..0047E492
    if (rsize == 0) {                                                               // 0047E494..0047E496
        if (self->first != 0) live_free(ptr<void>(self->first));                    // 0047E498..0047E4A0
        self->first = 0; self->last = 0; self->end = 0;                             // 0047E4A9..0047E4AF
        return self;
    }
    const U size = self->first ? static_cast<U>(static_cast<I>(self->last - self->first) >> 1) : 0; // 0047E4BA..0047E4CA
    if (rsize <= size) {                                                            // 0047E4CC..0047E4CE
        nv_ushort_copy_0047cc30(ptr<U16>(right->first), ptr<U16>(right->last), ptr<U16>(self->first)); // 0047E4D0..0047E4D3
        const U n = right->first ? static_cast<U>(static_cast<I>(right->last - right->first) >> 1) : 0; // 0047E4D8..0047E4FE
        self->last = self->first + n * 2;                                           // 0047E4E7 / 0047E500..0047E504
        return self;
    }
    const U capacity = self->first ? static_cast<U>(static_cast<I>(self->end - self->first) >> 1) : 0; // 0047E50F..0047E51C
    if (rsize <= capacity) {                                                        // 0047E51E..0047E520
        U16* const mid = ptr<U16>(right->first) + size;                             // 0047E522..0047E532
        nv_ushort_copy_0047cc30(ptr<U16>(right->first), mid, ptr<U16>(self->first)); // 0047E531..0047E537
        self->last = addr(nv_ushort_uninit_copy_0047be70(mid, ptr<U16>(right->last), ptr<U16>(self->last), self, addr(right))); // 0047E53C..0047E554
        return self;
    }
    live_free(ptr<void>(self->first));                                              // 0047E55F..0047E560 (EAX = first; free(0) included)
    const U n = right->first ? static_cast<U>(static_cast<I>(right->last - right->first) >> 1) : 0; // 0047E565..0047E578
    if (nv_ushort_vec_buy_0047ddc0(self, nullptr, n) != 0)                          // 0047E57A..0047E584
        self->last = addr(nv_ushort_vec_ucopy_0047cc60(self, nullptr, ptr<U16>(right->first), ptr<U16>(right->last), ptr<U16>(self->first))); // 0047E586..0047E599
    return self;
}

// ======================================================================= VertexCache
// 004A9680 VertexCache::VertexCache(size): entries = new int[size], all -1.
VertexCache* __fastcall nv_vertex_cache_construct_004a9680(VertexCache* self, void*, I size) {
    self->num_entries = size;                                                       // 004A9687
    self->entries = static_cast<I*>(live_operator_new(static_cast<U>(size) << 2));  // 004A968A..004A9696
    for (I i = 0; i < self->num_entries; ++i) self->entries[i] = -1;                // 004A969B..004A96B0
    return self;
}
// 004A96C0 VertexCache::~VertexCache: delete[] entries (free).
void __fastcall nv_vertex_cache_destruct_004a96c0(VertexCache* self, void*) {
    live_free(self->entries);                                                       // 004A96C0..004A96C3
}

// ======================================================================= NvStripInfo
// 0047BA70 NvStripInfo::IsInStrip(face).
U8 __fastcall nv_strip_is_in_strip_0047ba70(StripInfo* self, void*, const FaceInfo* face) {
    if (face == nullptr) return 0;                                                  // 0047BA70..0047BA7A
    if (self->experiment_id >= 0) return face->test_strip_id == self->strip_id ? 1 : 0; // 0047BA7D..0047BA91
    return face->strip_id == self->strip_id ? 1 : 0;                                // 0047BA94..0047BA9E
}
// 0047C8A0 NvStripInfo::Unique(faces, face): false once all three vertices occur in the vector.
U8 __fastcall nv_strip_unique_0047c8a0(StripInfo*, void*, Vec* faces, const FaceInfo* face) {
    bool b0 = false, b1 = false, b2 = false;                                        // 0047C8AB..0047C8B8
    for (U i = 0; i < count32(faces); ++i) {                                        // 0047C8C0..0047C940
        const FaceInfo* const f = at32<FaceInfo>(faces, i);
        if (!b0 && (f->v0 == face->v0 || f->v1 == face->v0 || f->v2 == face->v0)) b0 = true; // 0047C8D4..0047C8EB
        if (!b1 && (f->v0 == face->v1 || f->v1 == face->v1 || f->v2 == face->v1)) b1 = true; // 0047C8F0..0047C90C
        if (!b2 && (f->v0 == face->v2 || f->v1 == face->v2 || f->v2 == face->v2)) b2 = true; // 0047C911..0047C929
        if (b0 && b1 && b2) return 0;                                               // 0047C92B..0047C948
    }
    return 1;                                                                       // 0047C94F..0047C952
}
// 0047C960 NvStripInfo::SharesEdge(face, edgeInfos): any of the face's three edges touches a face in this strip.
U8 __fastcall nv_strip_shares_edge_0047c960(StripInfo* self, void*, const FaceInfo* face, Vec* edge_infos) {
    EdgeInfo* e = nv_find_edge_info_0047c230(edge_infos, face->v0, face->v1);      // 0047C968..0047C97A
    if (in_strip(self, e->face0)) return 1;                                         // 0047C97F..0047C9A3 (inline)
    if (in_strip(self, e->face1)) return 1;                                         // 0047C9A9..0047C9CA (inline)
    e = nv_find_edge_info_0047c230(edge_infos, face->v1, face->v2);                 // 0047C9CC..0047C9D2
    if (in_strip(self, e->face0)) return 1;                                         // 0047C9D7..0047C9FB (inline)
    if (nv_strip_is_in_strip_0047ba70(self, nullptr, e->face1) != 0) return 1;      // 0047C9FD..0047CA0A
    e = nv_find_edge_info_0047c230(edge_infos, face->v2, face->v0);                 // 0047CA0C..0047CA13
    if (nv_strip_is_in_strip_0047ba70(self, nullptr, e->face0) != 0) return 1;      // 0047CA18..0047CA2A
    if (nv_strip_is_in_strip_0047ba70(self, nullptr, e->face1) != 0) return 1;      // 0047CA2C..0047CA39
    return 0;                                                                       // 0047CA3B..0047CA3F (AL from the last call)
}
// 0047EB30 NvStripInfo::Combine(forward, backward): backward faces in reverse, then forward faces.
void __fastcall nv_strip_combine_0047eb30(StripInfo* self, void*, const Vec* forward, const Vec* backward) {
    for (I i = static_cast<I>(count32(backward)) - 1; i >= 0; --i)                  // 0047EB3B..0047EBBD
        push_back_inline(&self->faces, ptr<U>(backward->first) + i, addr(backward));
    const I n = static_cast<I>(count32(forward));                                   // 0047EBC3..0047EBDE
    for (I i = 0; i < n; ++i)                                                       // 0047EBE2..0047EC53
        push_back_inline(&self->faces, ptr<U>(forward->first) + i, static_cast<U>(n));
}

// ======================================================================= NvStripifier
// 0047C230 NvStripifier::FindEdgeInfo(edgeInfos, v0, v1) (static).
EdgeInfo* __cdecl nv_find_edge_info_0047c230(Vec* edge_infos, I v0, I v1) {
    return edge_walk(edge_infos, v0, v1);                                           // 0047C230..0047C26F
}
// 0047C4C0 NvStripifier::GetNextIndex(indices, face) (static): the face vertex not among the last two indices.
I __cdecl nv_get_next_index_0047c4c0(const Vec* indices, const FaceInfo* face) {
    const U n = indices->first ? static_cast<U>(static_cast<I>(indices->last - indices->first) >> 1) : 0; // 0047C4C0..0047C4D4
    const I v0 = ptr<U16>(indices->first)[n - 2];                                   // 0047C4D6
    const I v1 = ptr<U16>(indices->first)[n - 1];                                   // 0047C4DB
    const I f0 = face->v0, f1 = face->v1, f2 = face->v2;                            // 0047C4E5..0047C4EE
    if (f0 != v0 && f0 != v1) {                                                     // 0047C4EB..0047C4F5
        if ((f1 != v0 && f1 != v1) || (f2 != v0 && f2 != v1)) {                     // 0047C4F7..0047C505
            std::printf("GetNextIndex: Triangle doesn't have all of its vertices\n"); // 0047C507 (00743150)
            std::printf("GetNextIndex: Duplicate triangle probably got us derailed\n"); // 0047C511 (00743114)
        }
        return f0;
    }
    if (f1 != v0 && f1 != v1) {                                                     // 0047C524..0047C52A
        if ((f0 != v0 && f0 != v1) || (f2 != v0 && f2 != v1)) {                     // 0047C52C..0047C53A
            std::printf("GetNextIndex: Triangle doesn't have all of its vertices\n");
            std::printf("GetNextIndex: Duplicate triangle probably got us derailed\n");
        }
        return f1;
    }
    if (f2 != v0 && f2 != v1) {                                                     // 0047C559..0047C55F
        if ((f0 != v0 && f0 != v1) || (f1 != v0 && f1 != v1)) {                     // 0047C561..0047C56F
            std::printf("GetNextIndex: Triangle doesn't have all of its vertices\n");
            std::printf("GetNextIndex: Duplicate triangle probably got us derailed\n");
        }
        return f2;
    }
    std::printf("GetNextIndex: Duplicate triangle sent\n");                         // 0047C58E (007430EC)
    return -1;
}
// 0047C270 NvStripifier::AlreadyExists(face, faceInfos): same three vertices in the same order.
U8 __fastcall nv_already_exists_0047c270(Stripifier*, void*, const FaceInfo* face, Vec* face_infos) {
    for (U i = 0; i < count32(face_infos); ++i) {                                   // 0047C27B..0047C2AA
        const FaceInfo* const f = at32<FaceInfo>(face_infos, i);
        if (f->v0 == face->v0 && f->v1 == face->v1 && f->v2 == face->v2) return 1;  // 0047C290..0047C2AE
    }
    return 0;                                                                       // 0047C2B4..0047C2B6
}
// 0047BC40 NvStripifier::CalcNumHitsFace(cache, face).
I __fastcall nv_calc_num_hits_face_0047bc40(Stripifier*, void*, VertexCache* cache, const FaceInfo* face) {
    I hits = 0;                                                                     // 0047BC4E
    if (in_cache(cache, face->v0)) ++hits;                                          // 0047BC52..0047BC6E
    if (in_cache(cache, face->v1)) ++hits;                                          // 0047BC73..0047BC8E
    if (in_cache(cache, face->v2)) ++hits;                                          // 0047BC8F..0047BCB3
    return hits;
}
// 0047BB80 NvStripifier::UpdateCacheFace(cache, face).
void __fastcall nv_update_cache_face_0047bb80(Stripifier*, void*, VertexCache* cache, const FaceInfo* face) {
    if (!in_cache(cache, face->v0)) add_entry(cache, face->v0);                     // 0047BB84..0047BBC0
    if (!in_cache(cache, face->v1)) add_entry(cache, face->v1);                     // 0047BBC2..0047BBF6
    if (!in_cache(cache, face->v2)) add_entry(cache, face->v2);                     // 0047BBF8..0047BC30
}
// 0047C7A0 NvStripifier::NumNeighbors(face, edgeInfos): FindOtherFace != NULL for each edge (FindEdgeInfo called).
I __fastcall nv_num_neighbors_0047c7a0(Stripifier*, void*, FaceInfo* face, Vec* edge_infos) {
    I n = 0;                                                                        // 0047C7B5
    if (other_face(nv_find_edge_info_0047c230(edge_infos, face->v0, face->v1), face) != nullptr) n = 1; // 0047C7AC..0047C7D7
    if (other_face(nv_find_edge_info_0047c230(edge_infos, face->v1, face->v2), face) != nullptr) ++n; // 0047C7DF..0047C803
    if (other_face(nv_find_edge_info_0047c230(edge_infos, face->v2, face->v0), face) != nullptr) ++n; // 0047C804..0047C823
    return n;
}
// 0047C2C0 NvStripifier::FindStartPoint(faceInfos, edgeInfos): first face with more than one open edge, else -1.
I __fastcall nv_find_start_point_0047c2c0(Stripifier*, void*, Vec* face_infos, Vec* edge_infos) {
    for (U i = 0; i < count32(face_infos); ++i) {                                   // 0047C2C3..0047C3D8
        FaceInfo* const f = at32<FaceInfo>(face_infos, i);                          // 0047C2F4
        I ctr = 0;                                                                  // 0047C309
        if (other_face(edge_walk(edge_infos, f->v0, f->v1), f) == nullptr) ctr = 1; // 0047C2FB..0047C343 (inline walk)
        if (other_face(edge_walk(edge_infos, f->v1, f->v2), f) == nullptr) ++ctr;   // 0047C34B..0047C38A
        if (other_face(edge_walk(edge_infos, f->v2, f->v0), f) == nullptr) ++ctr;   // 0047C38B..0047C3C3
        if (ctr > 1) return static_cast<I>(i);                                      // 0047C3C4..0047C3E8
    }
    return -1;                                                                      // 0047C3EB..0047C3EE
}
// 0047C400 NvStripifier::FindGoodResetPoint(faceInfos, edgeInfos).
FaceInfo* __fastcall nv_find_good_reset_point_0047c400(Stripifier* self, void*, Vec* face_infos, Vec* edge_infos) {
    FaceInfo* result = nullptr;                                                     // 0047C40A
    const I num_faces = static_cast<I>(count32(face_infos));                        // 0047C403..0047C41C
    I start;
    if (self->first_time_reset_point != 0) {                                        // 0047C41F..0047C428
        start = nv_find_start_point_0047c2c0(self, nullptr, face_infos, edge_infos); // 0047C42A..0047C432
        self->first_time_reset_point = 0;                                           // 0047C437
    } else {
        start = static_cast<I>((static_cast<double>(num_faces) - 1.0) * self->mesh_jump); // 0047C43D..0047C44A (_ftol)
    }
    if (start == -1)                                                                // 0047C44F..0047C452
        start = static_cast<I>((static_cast<double>(num_faces) - 1.0) * self->mesh_jump); // 0047C454..0047C461
    I i = start;                                                                    // 0047C469
    do {
        FaceInfo* const f = at32<FaceInfo>(face_infos, static_cast<U>(i));          // 0047C470
        if (f->strip_id < 0) { result = f; break; }                                 // 0047C473..0047C486
        if (++i >= num_faces) i = 0;                                                // 0047C479..0047C47E
    } while (i != start);                                                           // 0047C480..0047C482
    const double jump = static_cast<double>(self->mesh_jump) + 0.1f;                // 0047C489..0047C48C (0073E798)
    self->mesh_jump = static_cast<float>(jump);                                     // 0047C492 (fst)
    if (jump > 1.0) self->mesh_jump = 0.05f;                                        // 0047C495..0047C4A4 (3D4CCCCD)
    return result;
}
// 0047C6D0 NvStripifier::CalcNumHitsStrip(cache, strip): hits / faces, unrounded (ST0).
double __fastcall nv_calc_num_hits_strip_0047c6d0(Stripifier*, void*, VertexCache* cache, StripInfo* strip) {
    I hits = 0, faces = 0;                                                          // 0047C6E4..0047C6E8
    for (U i = 0; i < count32(&strip->faces); ++i, ++faces) {                       // 0047C6F0..0047C784
        const FaceInfo* const f = at32<FaceInfo>(&strip->faces, i);
        if (in_cache(cache, f->v0)) ++hits;                                         // 0047C70E..0047C72E
        if (in_cache(cache, f->v1)) ++hits;                                         // 0047C732..0047C74E
        if (in_cache(cache, f->v2)) ++hits;                                         // 0047C752..0047C76E
    }
    return static_cast<double>(hits) / static_cast<double>(faces);                  // 0047C789..0047C790 (fild / fidiv)
}
// 0047C5B0 NvStripifier::UpdateCacheStrip(cache, strip).
void __fastcall nv_update_cache_strip_0047c5b0(Stripifier*, void*, VertexCache* cache, StripInfo* strip) {
    for (U i = 0; i < count32(&strip->faces); ++i) {                                // 0047C5BD..0047C6B7
        const FaceInfo* f = at32<FaceInfo>(&strip->faces, i);
        if (!in_cache(cache, f->v0)) add_entry(cache, f->v0);                       // 0047C5DE..0047C620
        f = at32<FaceInfo>(&strip->faces, i);
        if (!in_cache(cache, f->v1)) add_entry(cache, f->v1);                       // 0047C622..0047C668
        f = at32<FaceInfo>(&strip->faces, i);
        if (!in_cache(cache, f->v2)) add_entry(cache, f->v2);                       // 0047C66A..0047C6B0
    }
}
// 0047C830 NvStripifier::AvgStripSize(strips): total faces / strip count, unrounded (ST0).
double __fastcall nv_avg_strip_size_0047c830(Stripifier*, void*, const Vec* strips) {
    I size_accum = 0;                                                               // 0047C839..0047C83D
    const I num_strips = static_cast<I>(count32(strips));                           // 0047C835..0047C851
    for (I i = 0; i < num_strips; ++i)                                              // 0047C855..0047C87E
        size_accum += static_cast<I>(count32(&at32<StripInfo>(strips, static_cast<U>(i))->faces));
    return static_cast<double>(size_accum) / static_cast<double>(num_strips);       // 0047C885..0047C88E
}
// 0047CCE0 NvStripifier::FindTraversal(faceInfos, edgeInfos, strip, startInfo).
U8 __fastcall nv_find_traversal_0047cce0(Stripifier*, void*, Vec*, Vec* edge_infos, StripInfo* strip, StripStartInfo* start_info) {
    const I v = strip->start_info.to_v1 != 0 ? strip->start_info.start_edge->v1    // 0047CCE0..0047CCFA
                                              : strip->start_info.start_edge->v0;
    FaceInfo* untouched = nullptr;                                                  // 0047CD0C
    EdgeInfo* e = at32<EdgeInfo>(edge_infos, static_cast<U>(v));                    // 0047CCFD..0047CD04
    while (e != nullptr) {                                                          // 0047CD07..0047CD99
        FaceInfo* const face0 = e->face0;                                           // 0047CD20
        FaceInfo* const face1 = e->face1;                                           // 0047CD25
        if (face0 != nullptr && !in_strip(strip, face0) && face1 != nullptr && !is_marked(strip, face1)) { // 0047CD23..0047CD55
            untouched = face1;                                                      // 0047CD9D
            break;
        }
        if (face1 != nullptr && !in_strip(strip, face1) && face0 != nullptr && !is_marked(strip, face0)) { // 0047CD57..0047CD88
            untouched = face0;                                                      // 0047CDA3
            break;
        }
        e = e->v0 == v ? e->next_v0 : e->next_v1;                                   // 0047CD8A..0047CD94
    }
    start_info->start_face = untouched;                                             // 0047CDB1
    start_info->start_edge = e;                                                     // 0047CDB3
    if (e != nullptr) {                                                             // 0047CDB6
        if (nv_strip_shares_edge_0047c960(strip, nullptr, start_info->start_face, edge_infos) != 0) // 0047CDB8..0047CDC5
            start_info->to_v1 = e->v0 == v ? 1 : 0;                                 // 0047CDC7..0047CDCD
        else
            start_info->to_v1 = e->v1 == v ? 1 : 0;                                 // 0047CDDE..0047CDE4
    }
    return start_info->start_face != nullptr ? 1 : 0;                               // 0047CDD2..0047CDEE (untouched != 0)
}
// 0047F0D0 NvStripifier::CommitStrips(allStrips, strips, unused): strips become real, faces marked; a debug
// printf("BLEH") fires when the first face is (2302, 3215, 2603).
void __fastcall nv_commit_strips_0047f0d0(Stripifier*, void*, Vec* all_strips, const Vec* strips, U) {
    const I num_strips = static_cast<I>(count32(strips));                           // 0047F0D5..0047F0F2
    for (I i = 0; i < num_strips; ++i) {                                            // 0047F0F6..0047F20E
        StripInfo* strip = at32<StripInfo>(strips, static_cast<U>(i));              // 0047F110..0047F11A
        strip->experiment_id = -1;                                                  // 0047F11D
        const U value = addr(strip);                                                // 0047F12D ([esp+20h])
        push_back_inline(all_strips, &value, addr(strips));                         // 0047F124..0047F185
        const Vec* const faces = &at32<StripInfo>(strips, static_cast<U>(i))->faces; // 0047F18A..0047F193
        const I num_faces = static_cast<I>(count32(faces));                         // 0047F190..0047F1A3
        const FaceInfo* const first = at32<FaceInfo>(faces, 0);                     // 0047F1A6 (read even when empty)
        if (first->v0 == 0x8FE && first->v1 == 0xC8F && first->v2 == 0xA2B)         // 0047F1A8..0047F1C0
            std::printf("BLEH");                                                    // 0047F1C2..0047F1C7 (00743218)
        for (I j = 0; j < num_faces; ++j)                                           // 0047F1CF..0047F1FD
            mark_triangle(strip, at32<FaceInfo>(faces, static_cast<U>(j)));
    }
}

// 008302A4: set when an edge already carries two faces (BuildStripifyInfo, first edge only); no reader in the image.
volatile U8 nv_edge_overflow_008302a4 = 0;

namespace {
// inlined NvEdgeInfo(v0, v1): refcount 2, no faces, no links (operator new(1Ch), result unchecked when used).
EdgeInfo* new_edge(I v0, I v1) {
    EdgeInfo* const e = static_cast<EdgeInfo*>(live_operator_new(0x1C));
    if (e == nullptr) return nullptr;
    e->v0 = v0; e->v1 = v1;
    e->face0 = nullptr; e->face1 = nullptr;
    e->next_v0 = nullptr; e->next_v1 = nullptr;
    e->ref_count = 2;
    return e;
}
// one edge of BuildStripifyInfo: find (inline walk) or create-and-link, then attach the face.
void attach_edge(Vec* edges, I a, I b, FaceInfo* face, bool flag_overflow) {
    EdgeInfo* e = edge_walk(edges, a, b);
    if (e == nullptr) {
        e = new_edge(a, b);
        e->next_v0 = at32<EdgeInfo>(edges, static_cast<U>(a));
        e->next_v1 = at32<EdgeInfo>(edges, static_cast<U>(b));
        ptr<U>(edges->first)[a] = addr(e);
        ptr<U>(edges->first)[b] = addr(e);
        e->face0 = face;
    } else if (e->face1 != nullptr) {
        std::printf("BuildStripifyInfo: > 2 triangles on an edge... uncertain consequences\n"); // 007431D0
        if (flag_overflow) nv_edge_overflow_008302a4 = 1;
    } else {
        e->face1 = face;
    }
}
}

// 0047ED60 NvStripifier::BuildStripifyInfo(faceInfos, edgeInfos): one NvFaceInfo per distinct triangle of
// this->indices, edges created on demand and threaded into the per-vertex lists.
void __fastcall nv_build_stripify_info_0047ed60(Stripifier* self, void*, Vec* face_infos, Vec* edge_infos) {
    const I num_indices = self->indices.first                                       // 0047ED66..0047ED7D
        ? static_cast<I>(static_cast<I>(self->indices.last - self->indices.first) >> 1) : 0;
    nv_ptr_vec_reserve_0047daa0(face_infos, nullptr, static_cast<U>(num_indices));  // 0047ED7F..0047ED84
    nv_ptr_vec_resize_0047e630(edge_infos, nullptr, static_cast<U>(num_indices), 0); // 0047ED89..0047ED92
    for (I i = 0; i < num_indices; ++i) ptr<U>(edge_infos->first)[i] = 0;           // 0047ED97..0047EDAD
    const I num_triangles = num_indices / 3;                                        // 0047EDAF..0047EDBB
    U index = 0;                                                                    // 0047EDC5
    for (I t = 0; t < num_triangles; ++t) {                                         // 0047EDBD..0047F0B2
        const I v0 = ptr<U16>(self->indices.first)[index++];                        // 0047EDD1..0047EDD4
        const I v1 = ptr<U16>(self->indices.first)[index++];                        // 0047EDD8
        const I v2 = ptr<U16>(self->indices.first)[index++];                        // 0047EDE0
        FaceInfo* face = static_cast<FaceInfo*>(live_operator_new(0x18));           // 0047EDEB..0047EDF5
        if (face != nullptr) {                                                      // 0047EDFD..0047EE16 (NvFaceInfo(v0, v1, v2))
            face->v0 = v0; face->v1 = v1; face->v2 = v2;
            face->strip_id = -1; face->test_strip_id = -1; face->experiment_id = -1;
        }
        if (nv_already_exists_0047c270(self, nullptr, face, face_infos) != 0) continue; // 0047EE2B..0047EE40
        const U value = addr(face);                                                 // [esp+1Ch]
        push_back_inline(face_infos, &value, addr(face_infos));                     // 0047EE46..0047EEA7
        attach_edge(edge_infos, v0, v1, face, true);                                // 0047EEAC..0047EF5A
        attach_edge(edge_infos, v1, v2, face, false);                               // 0047EF5D..0047EFF9
        attach_edge(edge_infos, v2, v0, face, false);                               // 0047EFFC..0047F0A7
    }
}

namespace {
// inlined vector<unsigned short>::push_back: construct in place (0047C1D0) or _Insert_n(end(), 1, value) (0047D510).
void push_back16_inline(Vec* v, I value, U tag) {
    const U16 tmp = static_cast<U16>(value);                                        // the original passes &int (low word read)
    const U size = v->first ? static_cast<U>(static_cast<I>(v->last - v->first) >> 1) : 0;
    const U cap = v->first ? static_cast<U>(static_cast<I>(v->end - v->first) >> 1) : 0;
    if (v->first != 0 && size < cap) {
        U16* const at_end = ptr<U16>(v->last);
        nv_ushort_uninit_fill_n_0047c1d0(at_end, 1, &tmp, v, tag);
        v->last = addr(at_end + 1);
    } else {
        nv_ushort_vec_insert_n_0047d510(v, nullptr, ptr<U16>(v->last), 1, &tmp);
    }
}
U size16(const Vec* v) { return v->first ? static_cast<U>(static_cast<I>(v->last - v->first) >> 1) : 0; }
}

// 0047E720 NvStripifier::CreateStrips(allStrips, stripIndices, stripLengths) (KotOR variant: no stitching; one
// length word per strip). First face reordered so its unique vertex leads and the vertex shared with the third
// face trails; a leading duplicate is emitted when the first face is not clockwise for (v0, v1).
void __fastcall nv_create_strips_0047e720(Stripifier*, void*, const Vec* all_strips, Vec* strip_indices, Vec* strip_lengths) {
    if (all_strips->first == 0) return;                                             // 0047E72B..0047E732
    const I num_strips = static_cast<I>(static_cast<I>(all_strips->last - all_strips->first) >> 2); // 0047E738..0047E742
    if (num_strips == 0) return;                                                    // 0047E746
    nv_ushort_vec_resize_0047dd40(strip_lengths, nullptr, static_cast<U>(num_strips), 0); // 0047E74C..0047E752
    U prev_size = 0;                                                                // 0047E759 ([esp+14h])
    FaceInfo first{0, 0, 0, -1, -1, -1};                                            // [esp+28h] (0047E76B..0047E776: the -1 fields once)
    for (I i = 0; i < num_strips; ++i) {                                            // 0047E75D..0047EB1A
        StripInfo* const strip = at32<StripInfo>(all_strips, static_cast<U>(i));    // 0047E782..0047E791
        const I face_count = static_cast<I>(count32(&strip->faces));                // 0047E78C..0047E7A5 ([esp+60h])
        const FaceInfo* const f0 = at32<FaceInfo>(&strip->faces, 0);                // 0047E7A9..0047E7B6 (read even when empty)
        first.v0 = f0->v0; first.v1 = f0->v1; first.v2 = f0->v2;                    // 0047E7B0..0047E7CD
        if (face_count > 1) {                                                       // 0047E7BB..0047E7D1
            const FaceInfo* const f1 = at32<FaceInfo>(&strip->faces, 1);            // 0047E7D7
            I unique;                                                               // GetUniqueVertexInB(faces[1], &first), inline
            if (first.v0 != f1->v0 && first.v0 != f1->v1 && first.v0 != f1->v2) unique = first.v0; // 0047E7DA..0047E7EA
            else if (first.v1 != f1->v0 && first.v1 != f1->v1 && first.v1 != f1->v2) unique = first.v1; // 0047E7EE..0047E7FA
            else if (first.v2 != f1->v0 && first.v2 != f1->v1 && first.v2 != f1->v2) unique = first.v2; // 0047E7FC..0047E80A
            else unique = -1;                                                       // 0047E80E
            if (unique == first.v1) { const I t = first.v0; first.v0 = first.v1; first.v1 = t; } // 0047E811..0047E819
            else if (unique == first.v2) { const I t = first.v0; first.v0 = first.v2; first.v2 = t; } // 0047E81F..0047E82F
            if (face_count > 2) {                                                   // 0047E833..0047E838
                const FaceInfo* const f2 = at32<FaceInfo>(&strip->faces, 2);        // 0047E83A
                I shared;                                                           // GetSharedVertex, inline
                if (first.v0 == f2->v0 || first.v0 == f2->v1 || first.v0 == f2->v2) shared = first.v0; // 0047E83D..0047E84F
                else if (first.v1 == f2->v0 || first.v1 == f2->v1 || first.v1 == f2->v2) shared = first.v1; // 0047E851..0047E85B
                else if (first.v2 == f2->v0 || first.v2 == f2->v1 || first.v2 == f2->v2) shared = first.v2; // 0047E85D..0047E874
                else shared = -1;                                                   // 0047E86F
                if (shared == first.v1) { const I t = first.v1; first.v1 = first.v2; first.v2 = t; } // 0047E87A..0047E888
            }
        }
        {
            const FaceInfo* const f = at32<FaceInfo>(&strip->faces, 0);             // 0047E88C (IsCW(faces[0], v0, v1), inline)
            const I next = f->v0 == first.v0 ? f->v1 : (f->v1 == first.v0 ? f->v2 : f->v0);
            if (next != first.v1) {                                                 // 0047E8A1..0047E8A8
                const U16 lead = static_cast<U16>(first.v0);                        // [esp+20h]
                nv_ushort_vec_push_back_0047e5b0(strip_indices, nullptr, &lead);   // 0047E8AA..0047E8B5
            }
        }
        push_back16_inline(strip_indices, first.v0, addr(all_strips));             // 0047E8BA..0047E911
        push_back16_inline(strip_indices, first.v1, addr(all_strips));             // 0047E916..0047E96D
        push_back16_inline(strip_indices, first.v2, addr(all_strips));             // 0047E972..0047E9CD
        FaceInfo last = first;                                                      // 0047E9D2..0047E9DF (rep movsd, 6)
        for (I j = 1; j < face_count; ++j) {                                        // 0047E9E1..0047EAC4
            const FaceInfo* const f = at32<FaceInfo>(&strip->faces, static_cast<U>(j)); // 0047EA02..0047EA0D
            I unique;                                                               // GetUniqueVertexInB(&last, faces[j]), inline
            if (f->v0 != last.v0 && f->v0 != last.v1 && f->v0 != last.v2) unique = f->v0; // 0047EA10..0047EA20
            else if (f->v1 != last.v0 && f->v1 != last.v1 && f->v1 != last.v2) unique = f->v1; // 0047EA22..0047EA33
            else if (f->v2 != last.v0 && f->v2 != last.v1 && f->v2 != last.v2) unique = f->v2; // 0047EA35..0047EA44
            else continue;                                                          // 0047EA3A..0047EA42 -> 0047EAB5
            if (unique == -1) continue;                                             // 0047EA46..0047EA49
            push_back16_inline(strip_indices, unique, addr(all_strips));           // 0047EA4B..0047EAA6
            last.v0 = last.v1; last.v1 = last.v2; last.v2 = unique;                 // 0047EAAB..0047EAB1
        }
        const U size = size16(strip_indices);                                       // 0047EAD2..0047EAE2
        ptr<U16>(strip_lengths->first)[i] = static_cast<U16>(size - prev_size);     // 0047EAE4..0047EAF3
        prev_size = size16(strip_indices);                                          // 0047EAF7..0047EB0B
    }
}

namespace {
// inlined NvStripInfo deletion: ~vector (free faces block, zero the three pointers) then operator delete (free).
void delete_strip(StripInfo* strip) {
    if (strip == nullptr) return;
    if (strip->faces.first != 0) live_free(ptr<void>(strip->faces.first));
    strip->faces.first = 0; strip->faces.last = 0; strip->faces.end = 0;
    live_free(strip);
}
}

// 0047F220 NvStripifier::RemoveSmallStrips(allStrips, allBigStrips, faceList): strips shorter than
// minStripLength are dissolved into a face list that is re-ordered greedily by vertex-cache hits.
void __fastcall nv_remove_small_strips_0047f220(Stripifier* self, void*, Vec* all_strips, Vec* all_big_strips, Vec* face_list) {
    if (face_list->first != 0) live_free(ptr<void>(face_list->first));              // 0047F23B..0047F24E
    face_list->first = 0; face_list->last = 0; face_list->end = 0;                  // 0047F256..0047F25C
    if (all_big_strips->first != 0) live_free(ptr<void>(all_big_strips->first));    // 0047F25F..0047F26B
    all_big_strips->first = 0; all_big_strips->last = 0; all_big_strips->end = 0;   // 0047F277..0047F27D
    Vec temp{};                                                                     // 0047F280..0047F288 ([esp+18h]; EH state 0)
    for (U i = 0; i < count32(all_strips); ++i) {                                   // 0047F290..0047F3B7
        if (static_cast<I>(count32(&at32<StripInfo>(all_strips, i)->faces)) < self->min_strip_length) { // 0047F2B5..0047F2D5
            for (U j = 0; j < count32(&at32<StripInfo>(all_strips, i)->faces); ++j) // 0047F2DB..0047F366
                push_back_inline(&temp, ptr<U>(at32<StripInfo>(all_strips, i)->faces.first) + j, addr(face_list));
            delete_strip(at32<StripInfo>(all_strips, i));                           // 0047F36B..0047F397
        } else {
            nv_ptr_vec_push_back_0047ec60(all_big_strips, nullptr, ptr<U>(all_strips->first) + i); // 0047F3A9..0047F3B1
        }
    }
    U8* visited = nullptr;                                                          // 0047F3BE
    if (temp.first != 0) {                                                          // 0047F3BC..0047F3C2
        const U n = static_cast<U>(static_cast<I>(temp.last - temp.first) >> 2);    // 0047F3C4..0047F3C8
        if (n != 0) {                                                               // 0047F3CB..0047F3CD
            visited = static_cast<U8*>(live_operator_new(n));                       // 0047F3CF..0047F3D0 (thunk 006FC0D2)
            std::memset(visited, 0, n);                                             // 0047F3D5..0047F3EE
        }
    }
    VertexCache* cache = static_cast<VertexCache*>(live_operator_new(8));           // 0047F3F0..0047F3F2 (EH state 1)
    if (cache != nullptr) cache = nv_vertex_cache_construct_004a9680(cache, nullptr, self->cache_size); // 0047F3FE..0047F416
    U best_index = 0;                                                               // ebx (uninitialised in the original: [esp+40h])
    for (;;) {                                                                      // 0047F430..0047F512
        I best_hits = -1;                                                           // 0047F430..0047F433
        for (U i = 0; i < count32(&temp); ++i) {                                    // 0047F437..0047F477
            if (visited[i] != 0) continue;                                          // 0047F451..0047F459
            const I hits = nv_calc_num_hits_face_0047bc40(self, nullptr, cache, at32<FaceInfo>(&temp, i)); // 0047F45B..0047F469
            if (hits > best_hits) { best_hits = hits; best_index = i; }             // 0047F46E..0047F474
        }
        if (static_cast<double>(best_hits) == -1.0) break;                          // 0047F479..0047F48C (fild; fcomp 00740CBC)
        visited[best_index] = 1;                                                    // 0047F492..0047F49A
        nv_update_cache_face_0047bb80(self, nullptr, cache, at32<FaceInfo>(&temp, best_index)); // 0047F49E..0047F4AC
        push_back_inline(face_list, ptr<U>(temp.first) + best_index, addr(face_list)); // 0047F4B1..0047F50D
    }
    if (cache != nullptr) {                                                         // 0047F517..0047F51D
        nv_vertex_cache_destruct_004a96c0(cache, nullptr);                          // 0047F51F
        live_free(cache);                                                           // 0047F524..0047F529
    }
    live_free(visited);                                                             // 0047F531..0047F536 (delete[] thunk 006FC0CD)
    if (temp.first != 0) live_free(ptr<void>(temp.first));                          // 0047F53E..0047F543
}

aurora_model::List nv_face_list_008302a8 = {0, 0, 0};                               // .bss {data 008302A8, size 008302AC, capacity 008302B0}
volatile I nv_num_samples_00798a94 = 10;                                             // .data initial value 10; no writer in the image

// 00480EF0 NvStripifier::Stripify(inIndices, cacheSize, minStripLength, arg4, outStrips, outFaceList) (KotOR
// variant: no cache clamp; FindAllStrips takes 00798A94 and arg4; every NvFaceInfo* is kept in 008302A8 for
// AurStripify to delete).
void __fastcall nv_stripify_00480ef0(Stripifier* self, void*, const Vec* in_indices, I in_cache_size, I in_min_strip_length, U arg4, Vec* out_strips, Vec* out_face_list) {
    self->min_strip_length = in_min_strip_length;                                   // 00480F15..00480F1A
    self->mesh_jump = 0.0f;                                                         // 00480F24
    self->first_time_reset_point = 1;                                               // 00480F27
    self->cache_size = in_cache_size;                                               // 00480F2A
    nv_ushort_vec_assign_0047e470(&self->indices, nullptr, in_indices);             // 00480F21..00480F2D
    Vec all_face_infos{};                                                           // 00480F32..00480F3A ([esp+30h]; EH state 0)
    Vec all_edge_infos{};                                                           // 00480F42..00480F4A ([esp+10h]; EH state 1)
    nv_build_stripify_info_0047ed60(self, nullptr, &all_face_infos, &all_edge_infos); // 00480F4E..00480F5E
    Vec all_strips{};                                                               // 00480F63..00480F6B ([esp+20h]; EH state 2)
    nv_find_all_strips_00480490(self, nullptr, &all_strips, &all_face_infos, &all_edge_infos,
                                nv_num_samples_00798a94, static_cast<I>(arg4));     // 00480F6F..00480F90
    nv_split_up_strips_and_optimize_0047f560(self, nullptr, &all_strips, out_strips, &all_edge_infos, out_face_list); // 00480F95..00480FAB
    for (U i = 0; i < count32(&all_strips); ++i) {                                  // 00480FB0..00480FF3 (delete allStrips[i])
        StripInfo* const strip = at32<StripInfo>(&all_strips, i);
        if (strip == nullptr) continue;
        if (strip->faces.first != 0) live_free(ptr<void>(strip->faces.first));
        strip->faces.first = 0; strip->faces.last = 0; strip->faces.end = 0;
        live_free(strip);
    }
    for (U i = 0; i < count32(&all_edge_infos); ++i) {                              // 00480FF5..00481041
        for (EdgeInfo* info = at32<EdgeInfo>(&all_edge_infos, i); info != nullptr;) {
            EdgeInfo* const next = info->v0 == static_cast<I>(i) ? info->next_v0 : info->next_v1; // 00481020..0048102A
            if (--info->ref_count == 0) live_free(info);                            // 0048102D..00481032 (Unref)
            info = next;                                                            // 0048103A..0048103E
        }
    }
    nv_face_list_008302a8.size = 0;                                                 // 00481043
    for (U i = 0; i < count32(&all_face_infos); ++i) {                              // 0048104D..004810F0
        const U face = ptr<U>(all_face_infos.first)[i];                             // 00481074
        List* const l = &nv_face_list_008302a8;
        if (l->size == l->capacity) {                                               // 00481069..00481078 (inline Add, growth 2x / 16)
            const I cap = l->capacity ? l->capacity * 2 : 0x10;                     // 0048107A..00481087
            const U old = l->data;                                                  // 00481087
            l->capacity = cap;                                                      // 00481095
            l->data = addr(live_operator_new(static_cast<U>(cap) * 4));             // 0048108D..004810A5 (thunk 006FC0D2)
            for (I k = 0; k < l->size; ++k) ptr<U>(l->data)[k] = ptr<U>(old)[k];    // 004810AD..004810C8
            if (old != 0) live_free(ptr<void>(old));                                // 004810CA..004810CF (thunk 006FC0CD)
        }
        ptr<U>(l->data)[l->size] = face;                                            // 004810D7..004810E6
        l->size = l->size + 1;                                                      // 004810E9
    }
    if (all_strips.first != 0) live_free(ptr<void>(all_strips.first));              // 004810F5..004810FE
    if (all_edge_infos.first != 0) live_free(ptr<void>(all_edge_infos.first));      // 00481106..0048110B
    if (all_face_infos.first != 0) live_free(ptr<void>(all_face_infos.first));      // 00481113..00481118
}

// ======================================================================= static storage around NvStripInfo::Build
nv::Vec nv_scratch_indices_008302b4 = {0, 0, 0, 0};                                 // static WordVec scratchIndices (008302B4..008302C0)
volatile U nv_scratch_guard_008302c4 = 0;                                            // function-local static guard (bit 0)

// 0073BCD0 atexit callback: ~WordVec for scratchIndices (free the block, zero the three pointers).
void __cdecl nv_free_scratch_indices_0073bcd0() {
    if (nv_scratch_indices_008302b4.first != 0) live_free(ptr<void>(nv_scratch_indices_008302b4.first)); // 0073BCD0..0073BCDA
    nv_scratch_indices_008302b4.first = 0;                                          // 0073BCE2
    nv_scratch_indices_008302b4.last = 0;                                           // 0073BCEC
    nv_scratch_indices_008302b4.end = 0;                                            // 0073BCF6
}
// 0073BCB0 atexit callback: ~CExoArrayList for the 008302A8 face list (delete[] data via thunk 006FC0CD).
void __cdecl nv_dispose_faces_to_delete_0073bcb0() {
    if (nv_face_list_008302a8.data != 0) live_free(ptr<void>(nv_face_list_008302a8.data)); // 0073BCB0..0073BCBA
}
// 0073B0E0 (static initializer registering 0073BCB0) lives in nv_static_init.cpp (D66).

namespace {
// inlined FindOtherFace with the edge walk inline (Build keeps both inline after the first lookup).
FaceInfo* other_face_walk(const Vec* edges, I v0, I v1, const FaceInfo* f) { return other_face(edge_walk(edges, v0, v1), f); }
void push_scratch(I value, U tag) { push_back16_inline(&nv_scratch_indices_008302b4, value, tag); }
}

// 0047FD70 NvStripInfo::Build(edgeInfos, faceInfos): grow the strip forward from the start face, then backward,
// refusing faces whose three vertices are already present (Unique); Combine(forward, backward).
void __fastcall nv_strip_build_0047fd70(StripInfo* self, void*, Vec* edge_infos, Vec*) {
    if ((nv_scratch_guard_008302c4 & 1) == 0) {                                     // 0047FD88..0047FDA3
        nv_scratch_guard_008302c4 = nv_scratch_guard_008302c4 | 1;                  // 0047FDA5
        nv_scratch_indices_008302b4.first = 0;                                      // 0047FDB0
        nv_scratch_indices_008302b4.last = 0;                                       // 0047FDB6
        nv_scratch_indices_008302b4.end = 0;                                        // 0047FDBC
        std::atexit(&nv_free_scratch_indices_0073bcd0);                             // 0047FDAB..0047FDC2 (006FB332 _atexit)
    }
    nv_ushort_vec_resize_0047dd40(&nv_scratch_indices_008302b4, nullptr, 0, 0);     // 0047FDCE..0047FDD5
    Vec forward{};                                                                  // 0047FDDA..0047FDE2 ([esp+34h])
    Vec backward{};                                                                 // 0047FDEA..0047FDF2 ([esp+44h])
    nv_ptr_vec_push_back_0047ec60(&forward, nullptr, ptr<U>(addr(&self->start_info.start_face))); // 0047FDF6..0047FE00
    mark_triangle(self, self->start_info.start_face);                               // 0047FE05..0047FE1F
    const I v0 = self->start_info.to_v1 != 0 ? self->start_info.start_edge->v0 : self->start_info.start_edge->v1; // 0047FE22..0047FE31
    const I v1 = self->start_info.to_v1 != 0 ? self->start_info.start_edge->v1 : self->start_info.start_edge->v0; // 0047FE34..0047FE41
    {
        const U16 a = static_cast<U16>(v0);
        nv_ushort_vec_push_back_0047e5b0(&nv_scratch_indices_008302b4, nullptr, &a); // 0047FE44..0047FE56
        const U16 b = static_cast<U16>(v1);
        nv_ushort_vec_push_back_0047e5b0(&nv_scratch_indices_008302b4, nullptr, &b); // 0047FE5B..0047FE69
    }
    const I v2 = nv_get_next_index_0047c4c0(&nv_scratch_indices_008302b4, self->start_info.start_face); // 0047FE6E..0047FE76
    {
        const U16 c = static_cast<U16>(v2);
        nv_ushort_vec_push_back_0047e5b0(&nv_scratch_indices_008302b4, nullptr, &c); // 0047FE80..0047FE92
    }
    I nv0 = v1;
    I nv1 = v2;
    FaceInfo* next = other_face(nv_find_edge_info_0047c230(edge_infos, nv0, nv1), self->start_info.start_face); // 0047FE97..0047FEB6
    while (next != nullptr && !is_marked(self, next)) {                             // 0047FEB6..0047FEE1
        if (nv_strip_unique_0047c8a0(self, nullptr, &forward, next) == 0) break;    // 0047FEE7..0047FEF6
        const U value = addr(next);                                                 // [esp+10h]
        push_back_inline(&forward, &value, addr(edge_infos));                      // 0047FEFC..0047FF4B
        mark_triangle(self, next);                                                  // 0047FF50..0047FF6C
        nv0 = nv1;                                                                  // 0047FF75
        nv1 = nv_get_next_index_0047c4c0(&nv_scratch_indices_008302b4, next);       // 0047FF6F..0047FF77
        push_scratch(nv1, addr(edge_infos));                                        // 0047FF7C..0047FFE1
        next = other_face_walk(edge_infos, nv0, nv1, next);                         // 0047FFE6..00480020
    }
    Vec all{};                                                                      // 0048002C..00480036 ([esp+24h]; EH state 3)
    for (U i = 0; i < count32(&forward); ++i)                                       // 0048004B..004800C9
        push_back_inline(&all, ptr<U>(forward.first) + i, addr(edge_infos));
    nv_ushort_vec_resize_0047dd40(&nv_scratch_indices_008302b4, nullptr, 0, 0);     // 004800CB..004800D4
    push_scratch(v2, addr(edge_infos));                                             // 004800D9..0048013C
    push_scratch(v1, addr(edge_infos));                                             // 00480146..004801A4
    push_scratch(v0, addr(edge_infos));                                             // 004801AF..0048020D
    nv0 = v1;                                                                       // 0048014E
    nv1 = v0;                                                                       // 004801B7
    next = other_face_walk(edge_infos, nv0, nv1, self->start_info.start_face);      // 00480212..00480254
    while (next != nullptr && !is_marked(self, next)) {                             // 00480254..00480281
        if (nv_strip_unique_0047c8a0(self, nullptr, &all, next) == 0) break;        // 00480287..00480298
        const U value = addr(next);                                                 // [esp+10h]
        push_back_inline(&backward, &value, addr(edge_infos));                     // 0048029E..004802E9
        push_back_inline(&all, &value, addr(edge_infos));                          // 004802EE..00480341
        mark_triangle(self, next);                                                  // 00480346..00480366
        nv0 = nv1;                                                                  // 0048036F
        nv1 = nv_get_next_index_0047c4c0(&nv_scratch_indices_008302b4, next);       // 00480369..00480371
        push_scratch(nv1, addr(edge_infos));                                        // 00480376..004803DB
        next = other_face_walk(edge_infos, nv0, nv1, next);                         // 004803E0..0048041C
    }
    nv_strip_combine_0047eb30(self, nullptr, &forward, &backward);                  // 0048042C..0048043A
    if (all.first != 0) live_free(ptr<void>(all.first));                            // 0048043F..00480448
    if (backward.first != 0) live_free(ptr<void>(backward.first));                  // 00480450..00480455
    if (forward.first != 0) live_free(ptr<void>(forward.first));                    // 0048045D..00480466
}

namespace {
// inlined NvStripInfo(NvStripStartInfo(NULL, NULL, false), 0, -1) on operator new(28h); result unchecked when used.
StripInfo* new_strip() {
    StripInfo* const s = static_cast<StripInfo*>(live_operator_new(0x28));
    if (s == nullptr) return nullptr;
    s->start_info.start_face = nullptr;
    s->start_info.start_edge = nullptr;
    s->start_info.to_v1 = 0;
    s->faces.first = 0; s->faces.last = 0; s->faces.end = 0;
    s->strip_id = 0;
    s->experiment_id = -1;
    s->visited = 0;
    return s;
}
}

// 0047F560 NvStripifier::SplitUpStripsAndOptimize(allStrips, outStrips, edgeInfos, outFaceList) (KotOR variant:
// fixed 1000-face pieces). Pieces go through RemoveSmallStrips; the remaining strips are ordered greedily by
// vertex-cache hits starting from the strip with the fewest neighbours per face.
void __fastcall nv_split_up_strips_and_optimize_0047f560(Stripifier* self, void*, Vec* all_strips, Vec* out_strips, Vec* edge_infos, Vec* out_face_list) {
    Vec temp{};                                                                     // 0047F57C..0047F58A ([esp+40h]; EH state 0)
    for (U i = 0; i < count32(all_strips); ++i) {                                   // 0047F592..0047FA63
        const U size = count32(&at32<StripInfo>(all_strips, i)->faces);             // 0047F5BF..0047F5DB
        if (size > 1000) {                                                          // 0047F5DE..0047F5E3
            const U num_times = size / 1000;                                        // 0047F5E9..0047F614
            const U leftover = size % 1000;                                         // 0047F609..0047F63A
            U start = 0;                                                            // 0047F644
            for (U t = 0; t < num_times; ++t) {                                     // 0047F630..0047F781
                StripInfo* current = new_strip();                                   // 0047F650..0047F686
                for (U k = start; k < start + 1000; ++k)                            // 0047F68A..0047F716
                    push_back_inline(&current->faces, ptr<U>(at32<StripInfo>(all_strips, i)->faces.first) + k, addr(all_strips));
                const U value = addr(current);                                      // [esp+14h]
                push_back_inline(&temp, &value, addr(all_strips));                  // 0047F718..0047F76F
                start += 1000;                                                      // 0047F778
            }
            const U left_off = num_times * 1000;                                    // 0047F787..0047F78F
            if (leftover != 0) {                                                    // 0047F795..0047F797
                StripInfo* current = new_strip();                                   // 0047F79D..0047F7D3
                for (U k = 0; k < leftover; ++k)                                    // 0047F7D7..0047F875
                    push_back_inline(&current->faces, ptr<U>(at32<StripInfo>(all_strips, i)->faces.first) + left_off + k, addr(all_strips));
                const U value = addr(current);
                push_back_inline(&temp, &value, addr(all_strips));                  // 0047F877..0047F8DF
            }
        } else {
            StripInfo* current = new_strip();                                       // 0047F8FB..0047F936
            for (U j = 0; j < count32(&at32<StripInfo>(all_strips, i)->faces); ++j) // 0047F947..0047F9E7
                push_back_inline(&current->faces, ptr<U>(at32<StripInfo>(all_strips, i)->faces.first) + j, addr(all_strips));
            const U value = addr(current);
            push_back_inline(&temp, &value, addr(all_strips));                      // 0047F9EC..0047FA50
        }
    }
    Vec temp2{};                                                                    // 0047FA68..0047FA72 ([esp+30h]; EH state 4)
    nv_remove_small_strips_0047f220(self, nullptr, &temp, &temp2, out_face_list);   // 0047FA76..0047FA8E
    if (out_strips->first != 0) live_free(ptr<void>(out_strips->first));            // 0047FA93..0047FA9F
    out_strips->first = 0; out_strips->last = 0; out_strips->end = 0;               // 0047FAAD..0047FAB3
    if (temp2.first != 0) {                                                         // 0047FAA7..0047FAB6
        const U n = static_cast<U>(static_cast<I>(temp2.last - temp2.first) >> 2);  // 0047FABC..0047FAC7
        if (n != 0) {                                                               // 0047FACB
            VertexCache* cache = static_cast<VertexCache*>(live_operator_new(8));   // 0047FAD1..0047FAD3 (EH state 5)
            if (cache != nullptr) cache = nv_vertex_cache_construct_004a9680(cache, nullptr, self->cache_size); // 0047FADF..0047FAF7
            U first_index = 0;                                                      // 0047FB04
            float min_cost = 10000.0f;                                              // 0047FB08 (461C4000)
            for (U i = 0; i < n; ++i) {                                             // 0047FB10..0047FBB3
                StripInfo* const s = at32<StripInfo>(&temp2, i);                    // 0047FB1C..0047FB24
                I neighbors = 0;                                                    // 0047FB2C
                for (U j = 0; j < count32(&s->faces); ++j)                          // 0047FB30..0047FB5A
                    neighbors += nv_num_neighbors_0047c7a0(self, nullptr, at32<FaceInfo>(&s->faces, j), edge_infos);
                const double cost = static_cast<double>(neighbors) / static_cast<double>(count32(&s->faces)); // 0047FB5C..0047FB85 (unsigned fild fix-up 0073D770)
                if (cost < min_cost) {                                              // 0047FB87..0047FB90
                    min_cost = static_cast<float>(cost);                            // 0047FB96
                    first_index = i;                                                // 0047FB9A
                }
            }
            nv_update_cache_strip_0047c5b0(self, nullptr, cache, at32<StripInfo>(&temp2, first_index)); // 0047FBB8..0047FBC8
            push_back_inline(out_strips, ptr<U>(temp2.first) + first_index, addr(cache)); // 0047FBCD..0047FC1E
            at32<StripInfo>(&temp2, first_index)->visited = 1;                      // 0047FC23..0047FC25
            U best_index = addr(out_face_list);                                     // [esp+6Ch] (uninitialised in the original)
            for (;;) {                                                              // 0047FC30..0047FCEE
                double best = -1.0;                                                 // 0047FC30 (fld 00740CBC)
                for (U i = 0; i < n; ++i) {                                         // 0047FC36..0047FC6B
                    StripInfo* const s = at32<StripInfo>(&temp2, i);
                    if (s->visited != 0) continue;                                  // 0047FC3E..0047FC46
                    const double hits = nv_calc_num_hits_strip_0047c6d0(self, nullptr, cache, s); // 0047FC48..0047FC4E
                    const float stored = static_cast<float>(hits);                  // 0047FC53 (fst [esp+60h])
                    if (hits > best) {                                              // 0047FC57..0047FC5E
                        best_index = i;                                             // 0047FC62
                        best = stored;                                              // 0047FC60..0047FC66 (fld [esp+60h])
                    }
                }
                if (best == -1.0) break;                                            // 0047FC6D..0047FC78
                at32<StripInfo>(&temp2, best_index)->visited = 1;                   // 0047FC7A..0047FC88
                nv_update_cache_strip_0047c5b0(self, nullptr, cache, at32<StripInfo>(&temp2, best_index)); // 0047FC8C..0047FC90
                push_back_inline(out_strips, ptr<U>(temp2.first) + best_index, addr(cache)); // 0047FC95..0047FCE9
            }
            if (cache != nullptr) {                                                 // 0047FCF3..0047FCF5
                nv_vertex_cache_destruct_004a96c0(cache, nullptr);                  // 0047FCF7..0047FCF9
                live_free(cache);                                                   // 0047FCFE..0047FCFF
            }
        }
        live_free(ptr<void>(temp2.first));                                          // 0047FD09..0047FD0A
    }
    if (temp.first != 0) live_free(ptr<void>(temp.first));                          // 0047FD12..0047FD1B
}

// ======================================================================= std::vector<NvStripInfo*> element ctor/dtor
// 0047E620 vector<NvStripInfo*>::vector(): first = last = end = 0; returns this (eh vector constructor element).
void* __fastcall nv_strip_vec_construct_0047e620(void* self, void*) {
    Vec* const v = static_cast<Vec*>(self);
    v->first = 0; v->last = 0; v->end = 0;                                          // 0047E622..0047E62A
    return self;
}
// 0047D0F0 vector<NvStripInfo*>::~vector(): free the block, zero the three pointers.
void __fastcall nv_strip_vec_destruct_0047d0f0(void* self, void*) {
    Vec* const v = static_cast<Vec*>(self);
    if (v->first != 0) live_free(ptr<void>(v->first));                              // 0047D0F3..0047D0FB
    v->first = 0; v->last = 0; v->end = 0;                                          // 0047D103..0047D111
}

volatile I nv_experiments_per_sample_00798a98 = 6;                                   // .data initial value 6; no writer in the image

namespace {
// inlined NvStripInfo(NvStripStartInfo(face, edge, to_v1), strip_id, experiment_id) on operator new(28h).
StripInfo* new_strip_at(FaceInfo* face, EdgeInfo* edge, U8 to_v1, I strip_id, I experiment_id) {
    StripInfo* const s = static_cast<StripInfo*>(live_operator_new(0x28));
    if (s == nullptr) return nullptr;
    s->start_info.start_face = face;
    s->start_info.start_edge = edge;
    s->start_info.to_v1 = to_v1;
    s->faces.first = 0; s->faces.last = 0; s->faces.end = 0;
    s->strip_id = strip_id;
    s->experiment_id = experiment_id;
    s->visited = 0;
    return s;
}
}

// 00480490 NvStripifier::FindAllStrips(allStrips, allFaceInfos, allEdgeInfos, numSamples, commitArg) (KotOR
// variant: numSamples * 00798A98 experiment slots; commitArg forwarded to CommitStrips).
void __fastcall nv_find_all_strips_00480490(Stripifier* self, void*, Vec* all_strips, Vec* all_face_infos, Vec* all_edge_infos, I num_samples, I extra) {
    I experiment_id = 0;                                                            // 004804BA ([esp+24h])
    I strip_id = 0;                                                                 // 004804BE ([esp+1Ch])
    U8 done = 0;                                                                    // 004804C2 ([esp+1Bh])
    do {
        // PHASE 1: numSamples * 6 experiments
        const I slots = nv_experiments_per_sample_00798a98 * num_samples;           // 004804C6..004804CF
        unsigned char* const block = static_cast<unsigned char*>(live_operator_new(static_cast<U>(slots) * 16 + 4)); // 004804D2..004804DB (thunk 006FC0D2)
        Vec* experiments = nullptr;
        if (block != nullptr) {                                                     // 004804E9..004804F2
            put<I>(block, 0, slots);                                                // 00480505 (array cookie)
            crt_eh_vector_constructor_006fb55f(block + 4, 16, slots, &nv_strip_vec_construct_0047e620, &nv_strip_vec_destruct_0047d0f0); // 004804F4..00480507
            experiments = reinterpret_cast<Vec*>(block + 4);                        // 0048050C
        }
        I experiment_index = 0;                                                     // 0048052F ([esp+14h])
        nv::Tree reset_points{};                                                    // [esp+38h]
        reset_points.head = nv_tree_buy_head_0047c030();                            // 00480520..00480538
        reset_points.head->isnil = 1;                                               // 0048053C
        reset_points.head->parent = reset_points.head;                              // 00480540..00480544
        reset_points.head->left = reset_points.head;                                // 00480547..0048054B
        reset_points.head->right = reset_points.head;                               // 0048054D..00480551
        reset_points.size = 0;                                                      // 00480554 (EH state 1)
        for (I i = 0; i < num_samples; ++i) {                                       // 00480558..00480BCC
            FaceInfo* next_face = nv_find_good_reset_point_0047c400(self, nullptr, all_face_infos, all_edge_infos); // 00480571..00480586
            if (next_face == nullptr) { done = 1; break; }                          // 0048058A / 00480BD4
            const U key = addr(next_face);                                          // [esp+10h]
            nv::Node* found = nullptr;
            if (*nv_tree_find_0047ca80(&reset_points, nullptr, &found, &key) != reset_points.head) continue; // 00480590..004805A9
            nv::InsertResult inserted{};
            nv_tree_insert_0047dc80(&reset_points, nullptr, &inserted, &key);       // 004805AF..004805BD
            const I pairs[6][2] = {{next_face->v0, next_face->v1}, {next_face->v0, next_face->v1},
                                   {next_face->v1, next_face->v2}, {next_face->v1, next_face->v2},
                                   {next_face->v2, next_face->v0}, {next_face->v2, next_face->v0}};
            for (I e = 0; e < 6; ++e) {                                             // 004805C2..00480BB9 (six inline blocks)
                EdgeInfo* const edge = edge_walk(all_edge_infos, pairs[e][0], pairs[e][1]);
                StripInfo* const strip = new_strip_at(next_face, edge, (e & 1) == 0 ? 1 : 0, strip_id, experiment_id);
                ++strip_id; ++experiment_id;
                const U value = addr(strip);                                        // [esp+10h]
                push_back_inline(&experiments[experiment_index++], &value, addr(all_strips));
            }
        }
        // PHASE 2: build each experiment and the strips that follow it
        const I num_experiments = experiment_index;                                 // 00480BDB
        for (I i = 0; i < num_experiments; ++i) {                                   // 00480BE1..00480D33
            Vec* const set = &experiments[i];
            nv_strip_build_0047fd70(at32<StripInfo>(set, 0), nullptr, all_edge_infos, all_face_infos); // 00480BF6..00480C02
            const I exp_id = at32<StripInfo>(set, 0)->experiment_id;                // 00480C07..00480C0B
            StripInfo* iter = at32<StripInfo>(set, 0);                              // 00480C0E
            StripStartInfo start{nullptr, nullptr, 0};                              // 00480C10..00480C1A
            while (nv_find_traversal_0047cce0(self, nullptr, all_face_infos, all_edge_infos, iter, &start) != 0) { // 00480C1E..00480C35 / 00480D04..00480D1D
                iter = new_strip_at(start.start_face, start.start_edge, start.to_v1, strip_id, exp_id); // 00480C40..00480C8A
                ++strip_id;
                nv_strip_build_0047fd70(iter, nullptr, all_edge_infos, all_face_infos); // 00480C8C..00480CA2
                const U value = addr(iter);                                         // [esp+10h]
                push_back_inline(set, &value, addr(all_strips));                    // 00480CA7..00480CFF
            }
        }
        // PHASE 3: the most promising experiment
        I best_index = 0;                                                           // 00480D41
        double best_value = 0.0;                                                    // 00480D45..00480D49
        for (I i = 0; i < num_experiments; ++i) {                                   // 00480D4D..00480DA8
            const double avg = nv_avg_strip_size_0047c830(self, nullptr, &experiments[i]); // 00480D56..00480D5E
            const double count = static_cast<double>(count32(&experiments[i]));     // 00480D63..00480D81 (unsigned fild fix-up)
            const double value = avg + count * avg;                                 // 00480D87..00480D89
            if (value > best_value) { best_value = value; best_index = i; }         // 00480D8B..00480D9A
        }
        // PHASE 4: commit the best, delete the others
        nv_commit_strips_0047f0d0(self, nullptr, all_strips, &experiments[best_index], static_cast<U>(extra)); // 00480DAA..00480DC4
        for (I i = 0; i < num_experiments; ++i) {                                   // 00480DC9..00480E60
            if (i == best_index) continue;                                          // 00480DE2..00480DEA
            const I n = static_cast<I>(count32(&experiments[i]));                   // 00480DEC..00480E03
            for (I j = 0; j < n; ++j) {                                             // 00480E06..00480E44 (delete experiments[i][j])
                StripInfo* const s = at32<StripInfo>(&experiments[i], static_cast<U>(j));
                if (s == nullptr) continue;
                if (s->faces.first != 0) live_free(ptr<void>(s->faces.first));
                s->faces.first = 0; s->faces.last = 0; s->faces.end = 0;
                live_free(s);
            }
        }
        if (experiments != nullptr) {                                               // 00480E62..00480E68 (delete[] experiments)
            unsigned char* const cookie = reinterpret_cast<unsigned char*>(experiments) - 4;
            c2da_destroy_array_006fa852(experiments, 16, get<I>(cookie, 0), &nv_strip_vec_destruct_0047d0f0); // 00480E6A..00480E79
            live_free(cookie);                                                      // 00480E7E..00480E7F (thunk 006FC0CD)
        }
        nv::Node* erased = nullptr;
        nv_tree_erase_range_0047e0b0(&reset_points, nullptr, &erased, reset_points.head->left, reset_points.head); // 00480E87..00480EA3 (EH state -1)
        live_free(reset_points.head);                                               // 00480EA8..00480EAD
        reset_points.head = nullptr;                                                // 00480EB4
        reset_points.size = 0;                                                      // 00480EB8
    } while (done == 0);                                                            // 00480EBC..00480EC5
}
