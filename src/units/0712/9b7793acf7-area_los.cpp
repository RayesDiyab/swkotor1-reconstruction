// lane-world-los: CSWSArea::ClearLineOfSight 0050C330 and its area/room/surface-mesh closure.
// Evidence: lane-world-los\asm\<ADDR>.asm (dumpbin of the GOG swkotor.exe); offsets cited per instruction.
// Unwind (D30-FINAL): 0050C330 registers SEH state 0 for its local CExoArrayList (handler 0071BBA8 frees
// the data). No callee in this closure throws on any reachable path (no STL / throwing new / CRT throwers;
// virtual targets are original engine methods), so the extern "C" body frees the list explicitly at the
// original point (0050C671..0050C681) and no unwind-only cleanup is lost.
#include "dc46124e28-world_los.h"

namespace {
using namespace kotor_world_los;

// [[007A39FC]+8] CServerExoApp* (0050C3C2..0050C3CF, 004BBE20..004BBE29, 005066F8..00506700).
inline void* server_app() { return view<void*>(kotor_menu_window_cells::manager_007a39fc, 8); }

// 0050C3E5..0050C41C (and 0050C43B..0050C472): add an object to the local exclusion list when its
// vtbl+10 slot returns nonzero; growth = capacity ? capacity*2 : 10h via CExoArrayList::Allocate 005E03C0.
void add_if_slot_10(List* list, void* object) {
    if (object == nullptr) return;                                                // 0050C3E1..0050C3E3
    GameObject* const game_object = static_cast<GameObject*>(object);
    if (game_object->vtbl->slot_10(game_object, nullptr) == 0) return;            // 0050C3E5..0050C3EE
    const I capacity = list->capacity;                                            // 0050C3F0
    if (list->size == capacity) {                                                 // 0050C3F4..0050C3F8
        const U grown = capacity != 0 ? static_cast<U>(capacity) + static_cast<U>(capacity) : 0x10u; // 0050C3FA..0050C402
        menu_list_growth::allocate_005e03c0(list, nullptr, grown);               // 0050C407..0050C40C
    }
    reinterpret_cast<void**>(static_cast<std::uintptr_t>(list->data))[list->size] = object; // 0050C411..0050C419
    list->size = list->size + 1;                                                  // 0050C41C inc dword
}
}

extern "C" {

// ---------------------------------------------------------------- 0050C330 CSWSArea::ClearLineOfSight (RET 20h)
I __fastcall area_clear_line_of_sight_0050c330(void* area, void*, Vector* from, Vector* to, Vector* hit_out,
    U* blocker_out, U source_id, U target_id, I param7, I param8) {
    Vector room_hit;                                                              // [esp+30h..38h] = 0 (0050C362..0050C374)
    room_hit.x = 0.0f; room_hit.y = 0.0f; room_hit.z = 0.0f;
    U room_mask = k_invalid_object_7f000000;                                      // [esp+20h] (0050C37C); room arg 4, never read by 005797A0
    float best = bits_to_float(0x7F7FFFFFu);                                      // [esp+1Ch] FLT_MAX (0050C38A)
    I result = 1;                                                                 // [esp+10h] (0050C392)
    // 0050C351..0050C3B0: x87 compare from.z < to.z (fcomp; test ah,5; jp). Ordered-less -> (hi,lo)=(to.z,from.z),
    // otherwise (incl. unordered) (from.z,to.z). Raw DWORD copies.
    U z_hi, z_lo;
    if (from->z < to->z) { z_lo = float_to_bits(from->z); z_hi = float_to_bits(to->z); }
    else { z_lo = float_to_bits(to->z); z_hi = float_to_bits(from->z); }
    List exclusions = {0, 0, 0};                                                  // [esp+24h..2Ch] (0050C3B4..0050C3BE), SEH state 0
    add_if_slot_10(&exclusions, kotor_fastcall_004ae750(server_app(), nullptr, source_id)); // 0050C3C2..0050C41C
    add_if_slot_10(&exclusions, kotor_fastcall_004ae750(server_app(), nullptr, target_id)); // 0050C420..0050C472

    // 0050C4B2..0050C556: CWalkHitInfo local [esp+6Ch..B0h] (0x48 bytes): all zero except +40 = 7F000000.
    unsigned char walk_hit[0x48];
    std::memset(walk_hit, 0, sizeof walk_hit);
    view<U>(walk_hit, 0x40) = k_invalid_object_7f000000;                         // 0050C556 [esp+ACh]
    // 0050C476..0050C561: NoNonWalkPolysDetailed(from, to, z_lo, z_hi, 0.001f (3A83126F), 7F000000, blocker_out,
    //   1, &exclusions, param7, &walk_hit, 0).
    const U open = area_no_nonwalk_polys_detailed_00506650(area, nullptr, from, to, bits_to_float(z_lo),
        bits_to_float(z_hi), bits_to_float(0x3A83126Fu), k_invalid_object_7f000000, blocker_out, 1, &exclusions,
        param7, walk_hit, 0);
    bool scan_rooms = true;
    if (open == 0) {                                                              // 0050C566..0050C568
        // 0050C56A..0050C589: hit_out = walk_hit +4 (raw DWORDs); result = 0; param8 == 0 -> skip rooms.
        view<U>(hit_out, 0) = view<U>(walk_hit, 4);
        view<U>(hit_out, 4) = view<U>(walk_hit, 8);
        view<U>(hit_out, 8) = view<U>(walk_hit, 0xC);
        result = 0;                                                               // 0050C58C
        if (param8 == 0) scan_rooms = false;                                      // 0050C582 / 0050C590
    }
    if (scan_rooms) {
        // 0050C596..0050C66B: rooms [area+230h], stride 4Ch, count [area+22Ch] re-read per iteration.
        U offset = 0;
        for (I index = 0; index < view<I>(area, 0x22C); ++index, offset += 0x4C) {
            void* const room = static_cast<unsigned char*>(view<void*>(area, 0x230)) + offset; // 0050C5ED..0050C5F3
            if (room_clear_line_of_sight_005797a0(room, nullptr, *from, *to, &room_hit, &room_mask) != 0) continue; // 0050C5B0..0050C5FC
            // 0050C5FE..0050C627: |room_hit - from|^2 in x87 width: (dx*dx + dy*dy) + dz*dz.
            const double dz = static_cast<double>(room_hit.z) - static_cast<double>(from->z);
            const double dy = static_cast<double>(room_hit.y) - static_cast<double>(from->y);
            const double dx = static_cast<double>(room_hit.x) - static_cast<double>(from->x);
            const double distance = (dx * dx + dy * dy) + dz * dz;
            if (distance < static_cast<double>(best)) {                           // 0050C629..0050C632 (ordered less)
                best = static_cast<float>(distance);                              // 0050C638 fstp dword
                view<U>(hit_out, 0) = float_to_bits(room_hit.x);                 // 0050C634..0050C650 raw DWORDs
                view<U>(hit_out, 4) = float_to_bits(room_hit.y);
                view<U>(hit_out, 8) = float_to_bits(room_hit.z);
                result = 0;                                                       // 0050C653
            }
        }
    }
    live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(exclusions.data))); // 0050C671..0050C681 (state -1, 006FA390)
    return result;                                                                // 0050C68D
}

// ---------------------------------------------------------------- 00506650 CSWSArea::NoNonWalkPolysDetailed (RET 30h)
// Static objects: count +48, bounding boxes +4C (18h each: min +0, max +0C), object ids +50.
U __fastcall area_no_nonwalk_polys_detailed_00506650(void* area, void*, Vector* a, Vector* b, float f3, float f4,
    float f5, U object_id, U* out_id, I flag8, List* list, I flag10, void* walk_hit, I check_doors) {
    U result = 1;                                                                 // 00506660 / 00506668
    (void)aurora_vector_equal_004aa980(b, a);                                     // 0050665F..0050666C (EAX discarded)
    const I count = view<I>(area, 0x48);                                         // 00506671
    if (count <= 0) return result;                                                // 00506677..00506679 -> 005068F5
    // 0050667F..005066E0: box of the segment widened by f5; min side rounded to float at fstp dword.
    float x_hi_raw, x_lo_raw, y_hi_raw, y_lo_raw;
    if (a->x < b->x) { x_hi_raw = b->x; x_lo_raw = a->x; } else { x_hi_raw = a->x; x_lo_raw = b->x; } // 0050667F..0050669F
    if (a->y < b->y) { y_hi_raw = b->y; y_lo_raw = a->y; } else { y_hi_raw = a->y; y_lo_raw = b->y; } // 0050669F..005066B7
    const float x_lo = static_cast<float>(static_cast<double>(x_lo_raw) - static_cast<double>(f5)); // 005066C0..005066C4 [esp+38h]
    const float y_lo = static_cast<float>(static_cast<double>(y_lo_raw) - static_cast<double>(f5)); // 005066C8..005066CC [esp+3Ch]
    const float x_hi = static_cast<float>(static_cast<double>(x_hi_raw) + static_cast<double>(f5)); // 005066D0..005066D8 [esp+14h]
    const float y_hi = static_cast<float>(static_cast<double>(y_hi_raw) + static_cast<double>(f5)); // 005066DC..005066E0 [esp+18h]
    Area* const self = static_cast<Area*>(area);
    U box_offset = 0;                                                             // [esp+48h] (arg-1 slot reused)
    for (I index = 0; index < view<I>(area, 0x48); ++index, box_offset += 0x18) { // 005068D0..005068E1 (count re-read)
        if (check_doors != 0) {                                                   // 005066F0..005066F6
            GameObject* const object = static_cast<GameObject*>(
                kotor_fastcall_004ae750(server_app(), nullptr, view<U*>(area, 0x50)[index])); // 005066F8..00506707
            if (object != nullptr) {                                              // 0050670C..0050670E
                void* const door = object->vtbl->as_door_18(object, nullptr);    // 00506710..00506714
                if (door == nullptr) continue;                                    // 00506717..00506719
                if (view<U8>(door, 0x2CC) != 0) continue;                        // 0050671F..00506727
            }
        }
        const unsigned char* const box = static_cast<const unsigned char*>(view<void*>(area, 0x4C)) + box_offset; // 0050672D..00506738
        const float min_x = cview<float>(box, 0), min_y = cview<float>(box, 4), min_z = cview<float>(box, 8);
        const float max_x = cview<float>(box, 0xC), max_y = cview<float>(box, 0x10), max_z = cview<float>(box, 0x14);
        // 00506731..005067E3: x / y overlap (test ah,5 = ordered less; test ah,41h/je = ordered greater).
        if (x_lo < min_x && x_hi < min_x) continue;
        if (x_hi > max_x && x_lo > max_x) continue;
        if (y_lo < min_y && y_hi < min_y) continue;
        if (y_hi > max_y && y_lo > max_y) continue;
        // 005067E9..00506827: z overlap against [f3, f4].
        if (f3 < min_z && f4 < min_z) continue;
        if (f4 > max_z && f3 > max_z) continue;
        // 0050682D..00506842: area vtbl+4(id, list) nonzero -> skip.
        if (self->vtbl->slot_04(self, nullptr, view<U*>(area, 0x50)[index], list) != 0) continue;
        // 00506848..00506878: per-object test (ECX = area, ignored by the stdcall callee).
        if (area_no_nonwalk_polys_in_static_object_004bbe20(view<U*>(area, 0x50)[index], a, b, f3, f4, f5,
                object_id, flag8, walk_hit) != 0) continue;
        const U blocker = view<U*>(area, 0x50)[index];                           // 0050687A..00506883
        *out_id = blocker;                                                        // 0050688A
        if (walk_hit != nullptr) view<U>(walk_hit, 0x40) = blocker;              // 0050687D..0050688E
        if (flag10 == 1) {                                                        // 00506891..00506896
            result &= static_cast<U>(self->vtbl->no_nonwalk_polys_in_doors_18(self, nullptr, a, b, f3, f4, f5,
                object_id, out_id, flag8, list, 1));                              // 00506898..005068C2
        } else {
            result = 0;                                                           // 005068C8
        }
    }
    return result;                                                                // 005068E7
}

// ---------------------------------------------------------------- 004BBE20 CSWSArea::NoNoneWalkPolysInStaticObject (stdcall, RET 24h)
I __stdcall area_no_nonwalk_polys_in_static_object_004bbe20(U object_id, Vector* a, Vector* b, float f4, float f5,
    float f6, U, I, void* walk_hit) {
    GameObject* const object = static_cast<GameObject*>(kotor_fastcall_004ae750(server_app(), nullptr, object_id)); // 004BBE20..004BBE2D
    if (object == nullptr) return 0;                                              // 004BBE34..004BBE36
    const U8 type = object_type(object);                                          // 004BBE38 movzx [ecx+8]
    if (type == 9) {                                                              // 004BBE3C..004BBE3F -> 004BBE76
        void* const placeable = object->vtbl->as_placeable_48(object, nullptr);  // 004BBE93..004BBE96
        return placeable_no_nonwalk_polys_00585350(placeable, nullptr, a, b, &f4, &f5, &f6, walk_hit); // 004BBE99..004BBE9B
    }
    if (type == 10) {                                                             // 004BBE41..004BBE42 -> 004BBE49
        void* const door = object->vtbl->as_door_18(object, nullptr);            // 004BBE49..004BBE4B
        return door_no_nonwalk_polys_in_door_005890f0(door, nullptr, a, b, &f4, &f5, &f6, walk_hit); // 004BBE4E..004BBE6E
    }
    return 0;                                                                     // 004BBE44
}

// ---------------------------------------------------------------- 00585350 CSWSPlaceable::NoNonWalkPolys (RET 18h)
// Collision mesh at +3A0; position +90 -> +3C0 (mesh+20); orientation +428 -> +3D8 (mesh+38); tail jmp 005972B0
// with the three float pointers replaced by their values (005853A0..005853AA writes back into the arg slots).
I __fastcall placeable_no_nonwalk_polys_00585350(void* placeable, void*, Vector* a, Vector* b, float* f3,
    float* f4, float* f5, void* walk_hit) {
    copy_vector(offset_ptr(placeable, 0x3C0), offset_ptr(placeable, 0x90));                       // 00585351..0058536A
    std::memcpy(offset_ptr(placeable, 0x3D8), offset_ptr(placeable, 0x428), 16);                  // 0058536D..0058538C
    const float v5 = *f5;                                                         // 00585390..00585396
    const float v4 = *f4;                                                         // 0058539A..005853A0
    const float v3 = *f3;                                                         // 005853A4..005853AA
    return collision_mesh_no_nonwalk_polys_005972b0(offset_ptr(placeable, 0x3A0), nullptr, a, b, v3, v4, v5, walk_hit); // 005853AE..005853B4
}

// ---------------------------------------------------------------- 005890F0 CSWSDoor::NoNonWalkPolysInDoor (RET 18h)
// Collision mesh pointer table +32C indexed by byte +2CC (re-read at each use); null -> EAX 0.
I __fastcall door_no_nonwalk_polys_in_door_005890f0(void* door, void*, Vector* a, Vector* b, float* f3,
    float* f4, float* f5, void* walk_hit) {
    void* const mesh = view<void*>(door, 0x32C + 4u * view<U8>(door, 0x2CC));   // 005890F0..005890F7
    if (mesh == nullptr) return 0;                                                // 005890FE..00589102
    copy_vector(offset_ptr(mesh, 0x20), offset_ptr(door, 0x90));                                  // 00589106..0058911D
    void* const mesh_again = view<void*>(door, 0x32C + 4u * view<U8>(door, 0x2CC)); // 00589120..00589127
    std::memcpy(offset_ptr(mesh_again, 0x38), offset_ptr(door, 0x3B0), 16);                       // 0058912F..00589151
    const float v5 = *f5;                                                         // 00589147..00589154
    const float v4 = *f4;                                                         // 00589156..0058915E
    const float v3 = *f3;                                                         // 00589160..00589168
    void* const mesh_third = view<void*>(door, 0x32C + 4u * view<U8>(door, 0x2CC)); // 0058916A..00589171
    return collision_mesh_no_nonwalk_polys_005972b0(mesh_third, nullptr, a, b, v3, v4, v5, walk_hit); // 0058917F
}

// ---------------------------------------------------------------- 004BB570 CWalkHitInfo::Initialize (RET 1Ch)
void __fastcall walk_hit_info_initialize_004bb570(void* info, void*, I p1, const Vector* v2, const Vector* v3,
    const Vector* v4, const Vector* v5, U p6, const Vector* v7) {
    view<I>(info, 0) = p1;                                                       // 004BB574
    copy_vector(offset_ptr(info, 0x04), v2);                                              // 004BB57B..004BB58B
    copy_vector(offset_ptr(info, 0x10), v3);                                              // 004BB592..004BB5A2
    copy_vector(offset_ptr(info, 0x1C), v4);                                              // 004BB5A9..004BB5B9
    copy_vector(offset_ptr(info, 0x28), v5);                                              // 004BB5C0..004BB5D0
    view<U>(info, 0x44) = p6;                                                    // 004BB5DB
    copy_vector(offset_ptr(info, 0x34), v7);                                              // 004BB5DE..004BB5EE
}

// ---------------------------------------------------------------- 005797A0 CSWSRoom::ClearLineOfSight (RET 20h)
// Room: +38 nonzero -> clear; +3C CSWRoomSurfaceMesh*. Argument 4 is not read.
I __fastcall room_clear_line_of_sight_005797a0(void* room, void*, Vector from, Vector to, Vector* hit_out, U*) {
    if (view<U>(room, 0x38) != 0) return 1;                                      // 005797A6..005797B6
    SurfaceHitInfo hit;                                                           // [esp+4..23h]
    surface_hit_info_construct_00581320(&hit, nullptr, 0xFFFFFFFFu);              // 005797B9..005797BF
    surface_hit_info_clear_all_00580780(&hit, nullptr, 0xFFFFFFFFu);              // 005797C4..005797CA
    const U blocked = surface_mesh_check_aabb_line_of_sight_005814d0(view<void*>(room, 0x3C), nullptr, from, to, &hit); // 005797CF..00579809
    if (blocked != 0 && hit_out != nullptr) copy_vector(hit_out, &hit.point_10);  // 0057980E..0057982B
    return blocked == 0 ? 1 : 0;                                                  // 0057982E..00579836 sete
}

// ---------------------------------------------------------------- 00580780 CSWRoomSurfaceMeshHitInfo::ClearAll (RET 4)
void __fastcall surface_hit_info_clear_all_00580780(SurfaceHitInfo* info, void*, U mask) {
    info->mask_0 = mask;                                                          // 00580787
    info->face_c = -1;                                                            // 0058078B
    info->field_4 = 0;                                                            // 00580792
    info->hit_8 = 0;                                                              // 00580795
    info->bit_1c = 0;                                                             // 00580798
    view<U>(&info->point_10, 0) = 0;                                             // 005807A0
    view<U>(&info->point_10, 4) = 0;                                             // 005807A2
    view<U>(&info->point_10, 8) = 0;                                             // 005807B1 (stack temp 005807A5..AD is dead)
}

// ---------------------------------------------------------------- 00581320 CSWRoomSurfaceMeshHitInfo ctor (RET 4, EAX = this)
SurfaceHitInfo* __fastcall surface_hit_info_construct_00581320(SurfaceHitInfo* info, void*, U mask) {
    view<U>(&info->point_10, 0) = 0;                                             // 0058132B (Vector ctor)
    view<U>(&info->point_10, 4) = 0;                                             // 0058132E
    view<U>(&info->point_10, 8) = 0;                                             // 00581331
    info->mask_0 = mask;                                                          // 00581334
    view<U>(&info->point_10, 0) = 0;                                             // 00581336
    view<U>(&info->point_10, 4) = 0;                                             // 0058133B
    info->face_c = -1;                                                            // 0058133E
    info->field_4 = 0;                                                            // 00581345
    info->hit_8 = 0;                                                              // 00581348
    info->bit_1c = 0;                                                             // 0058134B
    view<U>(&info->point_10, 8) = 0;                                             // 0058135A (stack temp 0058134E..56 is dead)
    return info;
}

// ---------------------------------------------------------------- 00580920 CSWRoomSurfaceMesh::CheckAABBNode (RET 10h)
// Mesh: +54 vertices (Vector[]), +60 face vertex indices (3 x U per face), +64 face material (U per face),
// +AC AABB nodes (2Ch each: min +0, max +0C, face +18 (-1 = interior), child-order bits +20, children +24/+28).
I __fastcall surface_mesh_check_aabb_node_00580920(void* mesh, void*, I node, Vector* from, Vector* to,
    SurfaceHitInfo* hit) {
    const U node_offset = static_cast<U>(node) * 0x2Cu;                           // 00580929 imul ebx,ebx,2Ch
    unsigned char* node_ptr = static_cast<unsigned char*>(view<void*>(mesh, 0xAC)) + node_offset; // 00580933..0058093E
    if (box_intersect_004aa610(from, to, reinterpret_cast<Vector*>(node_ptr),
            reinterpret_cast<Vector*>(node_ptr + 0xC)) == 0)                      // 00580940..00580951
        return 0;
    node_ptr = static_cast<unsigned char*>(view<void*>(mesh, 0xAC)) + node_offset; // 0058095F..00580969 (reloaded)
    const I face = view<I>(node_ptr, 0x18);                                      // 00580965
    if (face == -1) {                                                             // 0058096B..0058096E -> 00580A70
        I first, second;
        if ((hit->field_4 & view<U>(node_ptr, 0x20)) != 0) {                     // 00580A70..00580A81
            first = surface_mesh_check_aabb_node_00580920(mesh, nullptr, view<I>(node_ptr, 0x24), from, to, hit); // 00580A83..00580A87
            second = surface_mesh_check_aabb_node_00580920(mesh, nullptr, view<I>(node_ptr, 0x28), from, to, hit); // 00580A8C..00580A9D
        } else {
            first = surface_mesh_check_aabb_node_00580920(mesh, nullptr, view<I>(node_ptr, 0x28), from, to, hit); // 00580AB2..00580AB6
            second = surface_mesh_check_aabb_node_00580920(mesh, nullptr, view<I>(node_ptr, 0x24), from, to, hit); // 00580ABB..00580ACC
        }
        return static_cast<I>(static_cast<U>(second) + static_cast<U>(first));     // 00580AA9 / 00580AD8 add eax,ecx
    }
    // 00580974..00580997: bit = 1 << (material[face] & 31); not in hit mask -> 0.
    const U bit = 1u << (view<U*>(mesh, 0x64)[face] & 31u);
    if ((bit & hit->mask_0) == 0) return 0;
    Vector triangle[3];                                                           // [esp+1Ch..3Fh]
    vector_constructor_iterator_00401130(triangle, 0xC, 3, &vector_default_construct_00409e70); // 00580999..005809A7
    Vector out;                                                                   // [esp+10h..18h] = 0 (005809B2..005809C2)
    view<U>(&out, 0) = 0; view<U>(&out, 4) = 0; view<U>(&out, 8) = 0;
    const U* const indices = view<U*>(mesh, 0x60) + static_cast<U>(face) * 3u;  // 005809AC / 005809CA
    const unsigned char* const vertices = static_cast<const unsigned char*>(view<void*>(mesh, 0x54)); // 005809AF
    copy_vector(&triangle[0], vertices + indices[0] * 12u);                       // 005809CD..005809E6
    copy_vector(&triangle[1], vertices + indices[1] * 12u);                       // 005809EA..00580A04
    copy_vector(&triangle[2], vertices + indices[2] * 12u);                       // 00580A08..00580A2E
    // 00580A1B..00580A39: polyhit(triangle, 3, from, to, to, &out).
    if (polyhit_004ab9d0(triangle, 3, from, to, to, &out) != 0) {                 // 00580A41..00580A43
        hit->face_c = face;                                                       // 00580A50
        hit->bit_1c = bit;                                                        // 00580A58
        return 1;
    }
    return 0;                                                                     // 00580A62 ([esp+4Ch] = 0 from 00580989)
}

// ---------------------------------------------------------------- 005814D0 CSWRoomSurfaceMesh::CheckAABBLineOfSight (RET 1Ch)
U __fastcall surface_mesh_check_aabb_line_of_sight_005814d0(void* mesh, void*, Vector from, Vector to,
    SurfaceHitInfo* hit) {
    if (view<U>(mesh, 0xB0) == 0) return 0;                                      // 005814D0..005814DC
    hit->mask_0 = view<U>(mesh, 0xD8);                                           // 005814DF / 005814EB
    const I hits = surface_mesh_check_aabb_node_00580920(mesh, nullptr, view<I>(mesh, 0xD4), &from, &to, hit); // 005814E6..005814FE
    if (hits > 0) {                                                               // 00581503..00581505 (signed)
        view<U>(&hit->point_10, 0) = float_to_bits(to.x);                        // 00581507..00581512 (`to` as left by the callees)
        view<U>(&hit->point_10, 4) = float_to_bits(to.y);                        // 00581518
        hit->hit_8 = 1;                                                           // 0058151B
        view<U>(&hit->point_10, 8) = float_to_bits(to.z);                        // 00581522
    }
    return hit->hit_8;                                                            // 00581525
}

// ---------------------------------------------------------------- 00401130 `eh vector constructor iterator` (stdcall, RET 10h)
void __stdcall vector_constructor_iterator_00401130(void* base, U element_size, U count,
    void* (__fastcall* construct)(void*, void*)) {
    if (static_cast<I>(count - 1u) < 0) return;                                   // 00401130..00401135 dec eax; js
    unsigned char* element = static_cast<unsigned char*>(base);
    U remaining = count;                                                          // 00401147 lea edi,[eax+1]
    do {
        (void)construct(element, nullptr);                                        // 00401150..00401152 call ebx (ECX = element)
        element += element_size;                                                  // 00401154
    } while (--remaining != 0);                                                   // 00401156..00401157
}

// ---------------------------------------------------------------- 00409E70 Vector::Vector() (thiscall, RET 0, EAX = this)
void* __fastcall vector_default_construct_00409e70(void* vector, void*) {
    view<U>(vector, 0) = 0;                                                      // 00409E74
    view<U>(vector, 4) = 0;                                                      // 00409E76
    view<U>(vector, 8) = 0;                                                      // 00409E79
    return vector;
}

} // extern "C"
