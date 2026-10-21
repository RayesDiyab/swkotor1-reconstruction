// nv_strip.hpp - lane-3d-model rev2: NvTriStrip objects as laid out in swkotor.exe (offsets observed in the
// functions listed below; names follow the NvTriStrip source the code was built from) and the declarations of
// the NvStripifier / NvStripInfo / VertexCache members and AurStripify.
#pragma once
#include "5b3fc79ff6-nv_stl.hpp"

namespace nv {
struct FaceInfo {                 // NvFaceInfo (0x18): 0047C270, 0047C4C0, 0047F1E2..0047F1F7
    I v0, v1, v2;                 // +00 / +04 / +08
    I strip_id;                   // +0C
    I test_strip_id;              // +10
    I experiment_id;              // +14
};
static_assert(sizeof(FaceInfo) == 0x18, "NvFaceInfo");
struct EdgeInfo {                 // NvEdgeInfo (0x1C; operator new(1Ch) 0047EEE2): 0047C230, 0047CCE0
    I ref_count;                  // +00
    FaceInfo* face0;              // +04
    FaceInfo* face1;              // +08
    I v0, v1;                     // +0C / +10
    EdgeInfo* next_v0;            // +14
    EdgeInfo* next_v1;            // +18
};
static_assert(sizeof(EdgeInfo) == 0x1C, "NvEdgeInfo");
struct StripStartInfo {           // NvStripStartInfo (0x0C): 0047CCE0
    FaceInfo* start_face;         // +00
    EdgeInfo* start_edge;         // +04
    U8 to_v1;                     // +08
};
static_assert(sizeof(StripStartInfo) == 0x0C, "NvStripStartInfo");
struct StripInfo {                // NvStripInfo (0x28; operator new(28h) 004805F2)
    StripStartInfo start_info;    // +00
    Vec faces;                    // +0C (vector<NvFaceInfo*>)
    I strip_id;                   // +1C
    I experiment_id;              // +20
    U8 visited;                   // +24
};
static_assert(sizeof(StripInfo) == 0x28, "NvStripInfo");
struct Stripifier {               // NvStripifier: 0047C400, 00480EF0
    Vec indices;                  // +00 (vector<unsigned short>)
    I cache_size;                 // +10
    I min_strip_length;           // +14
    float mesh_jump;              // +18
    U8 first_time_reset_point;    // +1C
};
struct VertexCache {              // VertexCache (8): 004A9680
    I* entries;                   // +00
    I num_entries;                // +04
};
static_assert(sizeof(VertexCache) == 8, "VertexCache");
}

// vector<unsigned short>::operator= / _Buy (used by NvStripifier::Stripify)
nv::Vec* __fastcall nv_ushort_vec_assign_0047e470(nv::Vec* self, void* unused_edx, const nv::Vec* right);
U8 __fastcall nv_ushort_vec_buy_0047ddc0(nv::Vec* self, void* unused_edx, aurora_model::U count);
// VertexCache
nv::VertexCache* __fastcall nv_vertex_cache_construct_004a9680(nv::VertexCache* self, void* unused_edx, aurora_model::I size);
void __fastcall nv_vertex_cache_destruct_004a96c0(nv::VertexCache* self, void* unused_edx);
// NvStripInfo
U8 __fastcall nv_strip_is_in_strip_0047ba70(nv::StripInfo* self, void* unused_edx, const nv::FaceInfo* face);
U8 __fastcall nv_strip_unique_0047c8a0(nv::StripInfo* self, void* unused_edx, nv::Vec* faces, const nv::FaceInfo* face);
U8 __fastcall nv_strip_shares_edge_0047c960(nv::StripInfo* self, void* unused_edx, const nv::FaceInfo* face, nv::Vec* edge_infos);
void __fastcall nv_strip_combine_0047eb30(nv::StripInfo* self, void* unused_edx, const nv::Vec* forward, const nv::Vec* backward);
void __fastcall nv_strip_build_0047fd70(nv::StripInfo* self, void* unused_edx, nv::Vec* edge_infos, nv::Vec* face_infos);
// NvStripifier (static members are __cdecl; the others are thiscall members whose this may go unused)
nv::EdgeInfo* __cdecl nv_find_edge_info_0047c230(nv::Vec* edge_infos, aurora_model::I v0, aurora_model::I v1);
aurora_model::I __cdecl nv_get_next_index_0047c4c0(const nv::Vec* indices, const nv::FaceInfo* face);
U8 __fastcall nv_already_exists_0047c270(nv::Stripifier* self, void* unused_edx, const nv::FaceInfo* face, nv::Vec* face_infos);
aurora_model::I __fastcall nv_calc_num_hits_face_0047bc40(nv::Stripifier* self, void* unused_edx, nv::VertexCache* cache, const nv::FaceInfo* face);
void __fastcall nv_update_cache_face_0047bb80(nv::Stripifier* self, void* unused_edx, nv::VertexCache* cache, const nv::FaceInfo* face);
aurora_model::I __fastcall nv_num_neighbors_0047c7a0(nv::Stripifier* self, void* unused_edx, nv::FaceInfo* face, nv::Vec* edge_infos);
aurora_model::I __fastcall nv_find_start_point_0047c2c0(nv::Stripifier* self, void* unused_edx, nv::Vec* face_infos, nv::Vec* edge_infos);
nv::FaceInfo* __fastcall nv_find_good_reset_point_0047c400(nv::Stripifier* self, void* unused_edx, nv::Vec* face_infos, nv::Vec* edge_infos);
double __fastcall nv_calc_num_hits_strip_0047c6d0(nv::Stripifier* self, void* unused_edx, nv::VertexCache* cache, nv::StripInfo* strip);
void __fastcall nv_update_cache_strip_0047c5b0(nv::Stripifier* self, void* unused_edx, nv::VertexCache* cache, nv::StripInfo* strip);
double __fastcall nv_avg_strip_size_0047c830(nv::Stripifier* self, void* unused_edx, const nv::Vec* strips);
U8 __fastcall nv_find_traversal_0047cce0(nv::Stripifier* self, void* unused_edx, nv::Vec* face_infos, nv::Vec* edge_infos, nv::StripInfo* strip, nv::StripStartInfo* start_info);
void __fastcall nv_commit_strips_0047f0d0(nv::Stripifier* self, void* unused_edx, nv::Vec* all_strips, const nv::Vec* strips, aurora_model::U unused3);
void __fastcall nv_build_stripify_info_0047ed60(nv::Stripifier* self, void* unused_edx, nv::Vec* face_infos, nv::Vec* edge_infos);
void __fastcall nv_remove_small_strips_0047f220(nv::Stripifier* self, void* unused_edx, nv::Vec* all_strips, nv::Vec* all_big_strips, nv::Vec* face_list);
void __fastcall nv_split_up_strips_and_optimize_0047f560(nv::Stripifier* self, void* unused_edx, nv::Vec* all_strips, nv::Vec* out_strips, nv::Vec* edge_infos, nv::Vec* out_face_list);
void __fastcall nv_find_all_strips_00480490(nv::Stripifier* self, void* unused_edx, nv::Vec* all_strips, nv::Vec* all_face_infos, nv::Vec* all_edge_infos, aurora_model::I num_samples, aurora_model::I extra);
void __fastcall nv_create_strips_0047e720(nv::Stripifier* self, void* unused_edx, const nv::Vec* all_strips, nv::Vec* strip_indices, nv::Vec* strip_lengths);
void __fastcall nv_stripify_00480ef0(nv::Stripifier* self, void* unused_edx, const nv::Vec* in_indices, aurora_model::I in_cache_size, aurora_model::I in_min_strip_length, aurora_model::U max_index, nv::Vec* out_strips, nv::Vec* out_face_list);
// CRT array helpers (006FB55F authored in model_texture_lists.cpp; 006FA852 provided by the c2da owner)
using NvElementCtor = void* (__fastcall*)(void* self, void* unused_edx);
using NvElementDtor = void (__fastcall*)(void* self, void* unused_edx);
extern "C" {
void __stdcall crt_eh_vector_constructor_006fb55f(void* first, aurora_model::U size, aurora_model::I count, NvElementCtor construct, NvElementDtor destroy);
void __stdcall c2da_destroy_array_006fa852(void* first, aurora_model::U size, aurora_model::I count, NvElementDtor destroy);
}
void* __fastcall nv_strip_vec_construct_0047e620(void* self, void* unused_edx);
void __fastcall nv_strip_vec_destruct_0047d0f0(void* self, void* unused_edx);
extern volatile aurora_model::I nv_experiments_per_sample_00798a98;
void __cdecl nv_dispose_faces_to_delete_0073bcb0();
void __cdecl nv_static_init_faces_to_delete_0073b0e0();
// cells: NvFaceInfo* list kept for AurStripify (008302A8 data / AC size / B0 capacity; exit-time destructor
// 0073BCB0 belongs to the static-teardown table) and the experiment count (00798A94, .data 10).
extern aurora_model::List nv_face_list_008302a8;
extern volatile aurora_model::I nv_num_samples_00798a94;
// AurStripify (called from MdlNodeTriMesh::BuildVertexIndices 0043C53F)
void __cdecl aurora_stripify_00481140(aurora_model::List* triangles, aurora_model::List* strips);
