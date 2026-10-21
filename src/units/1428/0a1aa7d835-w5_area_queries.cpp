// W5 t3: every-frame area geometry queries the client frame reaches (W4 MainLoop analysis): GetRoomIndex 004BB730
// and its room checks 00579560 / 00579610, GetNormalOfPoint 004AEB70 / 004B4FF0 / 00579840, and
// CSWSCreature::SetVisibilityList 0051AFD0 (ruling: W5). Transcribed from the original (read-only).
#include "842d6b1618-w5_t3.hpp"
#include "4f622be2f2-w5_area_direct_line.hpp"

using namespace w5;

namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned int); } // CExoArrayList::Allocate

extern "C" {
void* __fastcall surface_hit_info_construct_00581320(void* self, void*, U mask);   // CSWRoomSurfaceMeshHitInfo ctor (0526)

namespace {
const float k_ray_half_0073d6fc = 1000.0f;            // .rdata 0073D6FC
inline Vector vec_from_bits(U x, U y, U z) { Vector v; std::memcpy(&v.x, &x, 4); std::memcpy(&v.y, &y, 4); std::memcpy(&v.z, &z, 4); return v; }
}

// 00579560 CSWSRoom::CheckAABBWalkable(from, to, hit*): mesh (+3C) walkable ray; a hit point is moved to world space
// by the room offset (+4..+C) and the mesh position (+20..+28). RET 1Ch, EAX 0/1.
U __fastcall w5_room_check_aabb_walkable_00579560(void* room, void*, Vector from, Vector to, SurfaceHit* hit) {
    void* mesh = at<void*>(room, 0x3c);
    if (w5_adapt_check_aabb_walkable_00581530(mesh, nullptr, from, to, hit) == 0) return 0;   // 00579599..A8
    const Vector* off = reinterpret_cast<const Vector*>(static_cast<unsigned char*>(room) + 4);
    const Vector* pos = reinterpret_cast<const Vector*>(static_cast<unsigned char*>(mesh) + 0x20);
    const double x = static_cast<double>(hit->point_10.x) + off->x;                   // 005795AA..B0 (stays in ST)
    const double y = static_cast<double>(hit->point_10.y) + off->y;                   // 005795B6..BC (stays in ST)
    const float z = static_cast<float>(static_cast<double>(hit->point_10.z) + off->z); // 005795C4..CA fstp [18]
    const float wx = static_cast<float>(x + pos->x);                                   // 005795CE..D2
    const float wy = static_cast<float>(y + pos->y);                                   // 005795D6..D9
    const float wz = static_cast<float>(static_cast<double>(z) + pos->z);             // 005795DD..EE
    hit->point_10.x = wx; hit->point_10.y = wy; hit->point_10.z = wz;                 // 005795E4..F9
    return 1;
}
// 00579610 CSWSRoom::CheckAABBAll(from, to, hit*): mesh (+3C) all-material ray, no transform. RET 1Ch.
U __fastcall w5_room_check_aabb_all_00579610(void* room, void*, Vector from, Vector to, SurfaceHit* hit) {
    return w5_surface_mesh_check_aabb_all_00581610(at<void*>(room, 0x3c), nullptr, from, to, hit) != 0 ? 1u : 0u; // 00579614..5B
}

// 004BB730 CSWSArea::GetRoomIndex(Vector* point, int all): index of the first room (+230, +22C count, 4Ch each)
// whose mesh a vertical ray at the point (z +-1000) hits; -1 when none. RET 8.
// Export name is the one W4's client_update.cpp imports.
I __fastcall kotor_fastcall_004bb730(void* area, void*, const Vector* point, I all) {
    U x, y; std::memcpy(&x, &point->x, 4); std::memcpy(&y, &point->y, 4);
    const float up = static_cast<float>(static_cast<double>(point->z) + k_ray_half_0073d6fc);    // 004BB737..46
    const float down = static_cast<float>(static_cast<double>(point->z) - k_ray_half_0073d6fc);  // 004BB74A..60
    const Vector to = vec_from_bits(x, y, bits(down));                                  // [esp+0]
    const Vector from = vec_from_bits(x, y, bits(up));                                  // [esp+0C]
    SurfaceHit hit;                                                                     // [esp+18]
    surface_hit_info_construct_00581320(&hit, nullptr, 0xffffffffu);                   // 004BB764..75
    const I count = at<I>(area, 0x22c);                                                 // 004BB77A
    if (count <= 0) return -1;                                                          // 004BB780..8A -> 004BB84D (EDI = -1)
    for (I i = 0; i < at<I>(area, 0x22c); ++i) {                                        // 004BB7A0..3A (count re-read)
        void* room = at<unsigned char*>(area, 0x230) + i * 0x4c;
        const U r = all != 0 ? w5_room_check_aabb_all_00579610(room, nullptr, from, to, &hit)        // 004BB7A8..DF
                             : w5_room_check_aabb_walkable_00579560(room, nullptr, from, to, &hit);  // 004BB7E6..1D
        if (r != 0) return i;                                                           // 004BB822..28
    }
    return -1;                                                                          // 004BB840
}

// 00579840 CSWSRoom::GetNormalOfPoint(Vector* out, int face): mesh (+3C) face normals (+68); (0,0,1) otherwise. RET 8.
Vector* __fastcall w5_room_get_normal_of_point_00579840(void* room, void*, Vector* out, I face) {
    const Vector* normals = face >= 0 ? at<const Vector*>(at<void*>(room, 0x3c), 0x68) : nullptr;   // 00579840..53
    if (normals != nullptr) { copy_vector(out, normals + face); return out; }           // 00579855..72
    *out = vec_from_bits(0, 0, 0x3f800000u);                                            // 00579875..86
    return out;
}
// 004B4FF0 CServerExoAppInternal::GetNormalOfPoint(Vector* out, Vector* point): RET 8.
Vector* __fastcall w5_server_internal_get_normal_of_point_004b4ff0(void* internal, void*, Vector* out, Vector* point) {
    (void)internal;
    I face;                                                                             // [esp] (written by GetRoom)
    void* area = module_get_area_004c30f0(server_get_module_004ae6b0(server_app(), nullptr), nullptr); // 004B4FF1..500A
    void* room = reinterpret_cast<void*>(static_cast<std::uintptr_t>(
        w2x_area_room_004bb600(area, nullptr, point, &face)));                          // 004B5011
    if (room == nullptr) { *out = vec_from_bits(0, 0, 0x3f800000u); return out; }       // 004B5016..4C
    return w5_room_get_normal_of_point_00579840(room, nullptr, out, face);              // 004B501A..2C
}
// 004AEB70 CServerExoApp::GetNormalOfPoint(Vector* out, Vector* point): internal (+4), returns out. RET 8.
// Export name is the one W4's creature_ai.cpp imports.
Vector* __fastcall kotor_fastcall_004aeb70(void* server, void*, Vector* out, Vector* point) {
    w5_server_internal_get_normal_of_point_004b4ff0(at<void*>(server, 4), nullptr, out, point);   // 004AEB70..7E
    return out;                                                                         // 004AEB83
}

// 0051AFD0 CSWSCreature::SetVisibilityList(CExoArrayList<ulong>* list): replaces the +920 list (data/count/alloc)
// with a copy of `list` (null = cleared). RET 4.
void __fastcall kotor_fastcall_0051afd0(void* creature, void*, void* list) {
    unsigned char* own = static_cast<unsigned char*>(creature) + 0x920;
    if (at<void*>(own, 0) != nullptr) {                                                 // 0051AFD0..E3
        live_free(at<void*>(own, 0));                                                   // 0051AFE5..EB _free
        at<void*>(own, 0) = nullptr; at<U>(own, 8) = 0;                                 // 0051AFEE..F0
    }
    at<U>(own, 4) = 0;                                                                  // 0051AFF9
    if (list == nullptr) return;                                                        // 0051AFF7..FC
    for (I i = 0; i < at<I>(list, 4); ++i) {                                            // 0051AFFE..39 (count re-read)
        const U value = at<U*>(list, 0)[i];                                             // 0051B006..08
        if (at<I>(own, 4) == at<I>(own, 8)) {                                           // 0051B00B..11
            const I alloc = at<I>(own, 8);
            menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(own), nullptr,
                                                static_cast<unsigned>(alloc != 0 ? alloc + alloc : 0x10));   // 0051B013..23
        }
        at<U*>(own, 0)[at<I>(own, 4)] = value;                                          // 0051B028..2D
        at<I>(own, 4) += 1;                                                             // 0051B030
    }
}
}  // extern "C"
