// lane-3d-bsp: BSP visibility callbacks reached only through address immediates (gate G1, assigned to this lane by
// the coordinator): NodeHas* BSPSearch predicates, CollectBins (BSPTraverse sphere callback), ProcessParts
// (BSPTraverseVolumeApprox callback), the *Outside frustum tests, AddPartToMeshBuckets, AurClearActiveBins,
// SphereAbovePlane, CSWCObject::SetId (identical-code-folded field setter) and GrassSortTris (qsort comparator),
// plus the EAX-entry adapter of core's ConstructCameraFadePlane 0046B7C0 (D21 naked function).
// Evidence: lane-3d-bsp/d/<ADDR>.txt; instruction addresses cited per statement.
// D30-FINAL: double intermediates in original operand order, float only at the original `fstp dword`.
// Unwind: EH frames of 0046D3B0 (handler 00716E40) and 004A7690 (007181F0) are cited, not emitted (all callees are
// extern "C"; the two local list copies are released on the normal path exactly as the original does).
#include "c0efdcfcef-bsp_callbacks.h"

namespace {
using namespace kotor_scene3d_bsp;

constexpr U kZero_0073d700 = 0x00000000u;   // .rdata 0.0f
constexpr U kOne_0073d764 = 0x3F800000u;    // .rdata 1.0f / immediate 3F800000h
inline double zero() { return Wb(kZero_0073d700); }
inline void* node_data(void* node) { return field<void*>(node, 0x58); }
inline void* camera() { return *bsp_cells.current_camera_007bb4e8; }
inline const Plane* plane_at(const PlaneList* planes, I index) { return planes->data + index; }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }

// Shared by 0046A6B0 / 0046A7E0 / 0046A850: box (min at +lo, max at +hi) against the planes, starting at a cached
// plane index. `cache_store_rotated` selects 0046A6B0's (cache+i)%count store; the other two store the raw i.
I box_outside(void* part, const PlaneList* planes, U lo, U hi, volatile I& cache, bool cache_store_rotated) {
    for (I index = 0; index < planes->count; ++index) {                       // loop / ecx=[edi+4] re-read
        const I slot = static_cast<I>(static_cast<U>(cache) + static_cast<U>(index)) % planes->count; // cdq; idiv
        const double above = box_above_plane_004aaf50(static_cast<const Vector*>(at(part, lo)),
                                                      static_cast<const Vector*>(at(part, hi)), plane_at(planes, slot));
        if (above > zero()) {                                                  // test ah,41h; je
            if (cache_store_rotated)
                cache = static_cast<I>(static_cast<U>(cache) + static_cast<U>(index)) % planes->count; // 0046A708..0046A71E
            else
                cache = index;                                                 // 0046A839 / 0046A8A9 (raw index)
            return 1;
        }
    }
    return 0;
}

// The per-gob distance update shared by both gob loops of ProcessParts (0046D444..0046D4B4 / 0046D50D..0046D57D).
void update_gob_distance(void* gob) {
    aurora_model::Gob* g = as_gob(gob);
    gob_slots(g).s178_gob_enable_in_view_volume(g, nullptr);                   // call [edx+178h]
    Vector rotated;
    const Vector* turned = quaternion_rotate_004a9bb0(&rotated, static_cast<const Quaternion*>(at(gob, 0x84)),
                                                      static_cast<const Vector*>(at(gob, 0x13C)));
    const double z = W(field<float>(gob, 0x80)) + W(turned->z);              // kept on the x87 stack (no store)
    const float y = S(W(field<float>(gob, 0x7C)) + W(turned->y));            // fstp [esp+1Ch]
    const float x = S(W(field<float>(gob, 0x78)) + W(turned->x));            // fstp [esp+38h] / [esp+20h]
    void* view = camera();
    Vector delta;
    delta.x = S(W(field<float>(view, 0x7C)) - W(x));                         // fld [cam+7C]; fsub
    delta.y = S(W(field<float>(view, 0x80)) - W(y));                         // fld [cam+80]; fsub
    delta.z = S(W(field<float>(view, 0x84)) - z);                            // fsubr [cam+84]
    field<float>(gob, 0x104) = S(vector_magnitude_004ab100(&delta));          // fstp [esi+104h]
}
} // namespace

extern "C" {

// ---------------------------------------------------------------- BSPSearch predicates (node, item) -> count / bool
static I count_in(void* node, U list_offset, void* item) {
    return count_of(list_at(node_data(node), list_offset), item);
}
I __cdecl node_has_aabb_0046a660(void* node, void* part) {                     // 0046A660
    if (node == nullptr) return 0;                                             // 0046A664..0046A668 (EAX = 0)
    return field<void*>(node_data(node), 0x6C) == part ? 1 : 0;                // 0046A669..0046A67A sete
}
I __cdecl node_has_light_0046b340(void* node, void* part) {                    // 0046B340
    if (node == nullptr) return 0;
    return count_in(node, 0x00, part);                                         // 0046B349..0046B36C
}
I __cdecl node_has_emitter_0046b370(void* node, void* part) {                  // 0046B370
    if (node == nullptr) return 0;
    return count_in(node, 0x24, part);                                         // 0046B379..0046B39C
}
I __cdecl node_has_projection_0046b3a0(void* node, void* part) {               // 0046B3A0
    if (node == nullptr) return 0;
    if (count_in(node, 0x30, part) != 0) return 1;                             // 0046B3A9..0046B3CD
    if (count_in(node, 0x3C, part) != 0) return 1;                             // 0046B3CF..0046B3ED
    return 0;                                                                  // 0046B3EF..0046B3F3
}
I __cdecl node_has_trimesh_0046b400(void* node, void* part) {                  // 0046B400
    if (node == nullptr) return 0;
    return count_in(node, 0x18, part);
}
I __cdecl node_has_trigger_0046b430(void* node, void* part) {                  // 0046B430
    if (node == nullptr) return 0;
    return count_in(node, 0x54, part);
}
I __cdecl node_has_other_0046b460(void* node, void* part) {                    // 0046B460
    if (node == nullptr) return 0;
    return count_in(node, 0x48, part);
}
I __cdecl node_has_gob_0046b490(void* node, void* gob) {                       // 0046B490
    if (node == nullptr) return 0;
    return count_in(node, 0x0C, gob);
}

// 0046A610 AurClearActiveBins
void __cdecl aur_clear_active_bins_0046a610() {
    bsp_cells.active_bins_007fbf9c->size = 0;                                  // 0046A612
    bsp_cells.old_active_bins_007fbffc->size = 0;                              // 0046A617
}

// 004A9710 SphereAbovePlane(center, radius, plane) -> ST0 = dot(center, n) + d - radius
double __cdecl sphere_above_plane_004a9710(const Vector* center, float radius, const Plane* plane) {
    return ((W(center->z) * W(plane->c) + W(center->y) * W(plane->b)) + W(center->x) * W(plane->a)) +
           W(plane->d) - W(radius);                                            // 004A9718..004A972F
}

// 00616FA0 CSWCObject::SetId (folded setter): [this+4] = value, RET 4
void __fastcall cswc_object_set_id_00616fa0(void* self, void*, U value) {
    field<U>(self, 4) = value;                                                 // 00616FA0..00616FA4
}

// 0046A6B0 PartOutside (trimesh box +4C/+58, cache 00827F4C)
I __cdecl part_outside_trimesh_0046a6b0(void* part, const PlaneList* planes) {
    return box_outside(part, planes, 0x4C, 0x58, bsp_trimesh_plane_cache_00827f4c, true);
}
// 0046A7E0 PartOutside (projection box +54/+60, cache 00827F50 = raw index)
I __cdecl part_outside_projection_0046a7e0(void* part, const PlaneList* planes) {
    return box_outside(part, planes, 0x54, 0x60, bsp_projection_plane_cache_00827f50, false);
}
// 0046A850 PartOutside (trigger box +5C/+68, cache 00827F54 = raw index)
I __cdecl part_outside_trigger_0046a850(void* part, const PlaneList* planes) {
    return box_outside(part, planes, 0x5C, 0x68, bsp_trigger_plane_cache_00827f54, false);
}

// 0046A730 PartOutside (emitter: world position sphere of radius [+D0])
I __cdecl part_outside_emitter_0046a730(void* part, const PlaneList* planes) {
    Vector position = {bits_to_float(0), bits_to_float(0), bits_to_float(0)};                     // 0046A74A..0046A75A
    Quaternion orientation = {bits_to_float(kOne_0073d764), bits_to_float(0), bits_to_float(0), bits_to_float(0)}; // 0046A762..0046A77A
    slots<PartSlots>(part).get_world(part, nullptr, &position, &orientation);                      // 0046A739..0046A782
    for (I index = 0; index < planes->count; ++index) {                        // 0046A789..0046A7C3
        if (sphere_above_plane_004a9710(&position, field<float>(part, 0xD0), plane_at(planes, index)) > zero()) // 0046A794..0046A7B8
            return 1;                                                          // 0046A7CF
    }
    return 0;                                                                  // 0046A7C5
}

// 0046A8C0 GobOutside(gob, planes)
I __cdecl gob_outside_0046a8c0(void* gob, const PlaneList* planes) {
    aurora_model::Gob* g = as_gob(gob);
    if (gob_slots(g).s180_gob_is_in_view_volume(g, nullptr) != 0) return 1;   // 0046A8C8..0046A8DF
    for (I index = 0; index < planes->count; ++index) {                        // 0046A8E1..0046A962
        if (bsp_gob_cull_disabled_007fbf20 != 0) continue;                     // 0046A8F2..0046A8F9
        Vector rotated;                                                        // [esp+1Ch]
        const Vector* turned = quaternion_rotate_004a9bb0(&rotated, static_cast<const Quaternion*>(at(gob, 0x84)),
                                                          static_cast<const Vector*>(at(gob, 0x13C))); // 0046A8FB..0046A90E
        Vector centre;                                                         // [esp+10h]
        centre.z = S(W(field<float>(gob, 0x80)) + W(turned->z));               // 0046A913..0046A919 / 0046A940
        centre.y = S(W(field<float>(gob, 0x7C)) + W(turned->y));               // 0046A91E..0046A927 / 0046A93C
        centre.x = S(W(field<float>(gob, 0x78)) + W(turned->x));               // 0046A92C..0046A930 / 0046A938
        if (sphere_above_plane_004a9710(&centre, field<float>(gob, 0x138), plane_at(planes, index)) > zero()) // 0046A944..0046A957
            return 1;                                                          // 0046A96E
    }
    return 0;                                                                  // 0046A964
}

// 0046A980 BinOutside(bin, planes): sphere centre +2C, radius +8
I __cdecl bin_outside_0046a980(void* bin, const PlaneList* planes) {
    for (I index = 0; index < planes->count; ++index) {                        // 0046A988..0046A9C2
        if (sphere_above_plane_004a9710(static_cast<const Vector*>(at(bin, 0x2C)), field<float>(bin, 8),
                                        plane_at(planes, index)) > zero())     // 0046A997..0046A9B7
            return 1;                                                          // 0046A9CB
    }
    return 0;
}

// 0046BDF0 AddPartToMeshBuckets(part)
void __cdecl add_part_to_mesh_buckets_0046bdf0(void* part) {
    if (void* controller = slots<PartQuerySlots>(part).query_4c(part, nullptr)) {               // 0046BDF5..0046BDFE
        slots<ControllerSlots>(controller).update(controller, nullptr, *callback_cells.frame_time_0078e574); // 0046BE00..0046BE0B
    }
    aurora_model::MdlNode* mesh = aurora_node_as_trimesh_0043e400(field<aurora_model::MdlNode*>(part, 4), nullptr); // 0046BE0E..0046BE11
    if (field<U8>(mesh, 0x186) != 0) {                                         // 0046BE16..0046BE1E
        add_with_growth(bsp_cells.part_list_00827f40, part);                   // 0046BE20..0046BE53
        return;
    }
    const U tid = kotor_fastcall_0047af00(field<void*>(part, 0x44), nullptr);  // 0046BE5B..0046BE5F
    add_with_growth(&bsp_cells.mesh_buckets_008194e0[tid], part);              // 0046BE64..0046BEAE
    add_with_growth(bsp_cells.mesh_shadow_bucket_007fbfb4, part);              // 0046BEA0..0046BEDA
}

// 0046CAE0 CollectBins(node, center, radius_sq): BSPTraverse sphere callback; activates near grass bins.
void __cdecl collect_bins_0046cae0(void* node, const Vector* center, float radius_sq) {
    void* data = node_data(node);                                              // 0046CAE9
    const I count = list_at(data, 0x60)->size;                                 // 0046CAEC..0046CAF3 (read once)
    for (I index = 0; index < count; ++index) {                                // 0046CAF7..0046CBE9
        void* bin = elements(list_at(data, 0x60))[index];                      // 0046CB03..0046CB08
        Vector delta = {bits_to_float(0), bits_to_float(0), bits_to_float(0)}; // 0046CB0B..0046CB1B
        delta.x = S(W(center->x) - W(field<float>(bin, 0x2C)));                // 0046CB06..0046CB42
        delta.y = S(W(center->y) - W(field<float>(bin, 0x30)));                // 0046CB2E..0046CB53
        delta.z = S(W(center->z) - W(field<float>(bin, 0x34)));                // 0046CB38..0046CB57
        double distance = vector_magnitude_004ab100(&delta) - W(field<float>(bin, 8)); // 0046CB5B..0046CB60
        if (distance < zero()) distance = zero();                              // 0046CB66..0046CB75 test ah,5; jp
        const double square = distance * distance;                             // 0046CB7B..0046CB7D
        const float stored = S(square);                                        // 0046CB7F fst [esp+30h]
        if (!(square < W(radius_sq))) continue;                                // 0046CB83..0046CB8C test ah,5; jp
        *bsp_cells.bins_visited_007fbf2c = *bsp_cells.bins_visited_007fbf2c + 1; // 0046CB8E..0046CBA2
        add_with_growth(bsp_cells.active_bins_007fbf9c, bin);                  // 0046CB94..0046CBD4
        cswc_object_set_id_00616fa0(bin, nullptr, float_to_bits(stored));      // 0046CBCD..0046CBDD
    }
}

// 0046D3B0 ProcessParts(node, scene, planes): BSPTraverseVolumeApprox callback; fills the render buckets.
void __cdecl process_parts_0046d3b0(void* node, void*, PlaneList* planes) {
    if (node == nullptr) return;                                               // 0046D3CC..0046D3CE
    void* data = node_data(node);                                              // 0046D3D5
    if (data == nullptr) return;                                               // 0046D3D8..0046D3DE
    if (*callback_cells.draw_aabb_007fbf5c != 0 && field<void*>(data, 0x6C) != nullptr) // 0046D3E4..0046D3F2
        add_part_to_mesh_buckets_0046bdf0(field<void*>(data, 0x6C));           // 0046D3F4..0046D3F5
    if (bsp_ignore_tile_hidden_0078e5ac == 0) {                                // 0046D3FD..0046D404
        void* tile = field<void*>(data, 0x70);                                 // 0046D406
        if (tile != nullptr && field<U8>(tile, 0x90) != 0) return;            // 0046D409..0046D415
    }
    for (I index = 0; index < list_at(data, 0x0C)->size; ++index) {           // 0046D41B..0046D4CB (gobs +0C)
        void* gob = elements(list_at(data, 0x0C))[index];                      // 0046D430..0046D433
        if (gob_outside_0046a8c0(gob, planes) != 0) {                          // 0046D436..0046D442
            *bsp_cells.stat_007fbf44 = *bsp_cells.stat_007fbf44 + 1;           // 0046D4BF
            continue;
        }
        update_gob_distance(gob);                                              // 0046D444..0046D4B4
    }
    PointerList room_objects;                                                  // [esp+30h] (EH state 0)
    int_list_copy_construct_0046d250(&room_objects, nullptr, list_at(data, 0x74)); // 0046D4D1..0046D4D9
    const I object_count = room_objects.size;                                  // 0046D4DE
    for (I index = 0; index < object_count; ++index) {                         // 0046D4E4..0046D591
        void* gob = elements(&room_objects)[index];                            // 0046D4F0..0046D4F8
        if (gob_outside_0046a8c0(gob, planes) != 0) {                          // 0046D4FB..0046D507
            *bsp_cells.stat_007fbf44 = *bsp_cells.stat_007fbf44 + 1;           // 0046D588
            continue;
        }
        update_gob_distance(gob);                                              // 0046D50D..0046D57D
    }
    for (I index = 0; index < list_at(data, 0x24)->size; ++index) {           // 0046D597..0046D644 (emitters +24)
        void* emitter = elements(list_at(data, 0x24))[index];                  // 0046D5A4..0046D5AB
        if (part_outside_emitter_0046a730(emitter, planes) != 0) continue;     // 0046D5AE..0046D5BA
        add_inline_growth(bsp_cells.emitter_bucket_007fbf78, emitter);         // 0046D5C0..0046D638
    }
    PointerList room_emitters;                                                 // [esp+14h] (EH state 1)
    int_list_copy_construct_0046d250(&room_emitters, nullptr, list_at(data, 0x8C)); // 0046D64A..0046D659
    const I emitter_count = room_emitters.size;                                // 0046D65E
    for (I index = 0; index < emitter_count; ++index) {                        // 0046D662..0046D73A
        void* emitter = elements(&room_emitters)[index];                       // 0046D671..0046D679
        if (part_outside_emitter_0046a730(emitter, planes) != 0) continue;     // 0046D67C..0046D688
        if (count_of(bsp_cells.emitter_bucket_007fbf78, emitter) != 0) continue; // 0046D68E..0046D6AE
        add_inline_growth(bsp_cells.emitter_bucket_007fbf78, emitter);         // 0046D6B4..0046D731
    }
    void* tile = field<void*>(data, 0x70);                                     // 0046D744
    if (tile != nullptr && W(field<float>(tile, 0x94)) < Wb(kOne_0073d764) && field<U8>(tile, 0x90) == 0) { // 0046D747..0046D766
        field<float>(tile, 0x94) = S(Wb(*callback_cells.frame_time_0078e574) / W(*callback_cells.fade_time_0078e5a0) +
                                     W(field<float>(tile, 0x94)));            // 0046D768..0046D77A
        tile = field<void*>(data, 0x70);                                       // 0046D780
        if (W(field<float>(tile, 0x94)) > Wb(kOne_0073d764))                   // 0046D783..0046D794 test ah,41h; jne
            field<U>(tile, 0x94) = kOne_0073d764;                              // 0046D796
    }
    if (*callback_cells.draw_aabb_007fbf5c == 0) {                             // 0046D7A0..0046D7AB
        for (I index = 0; index < list_at(data, 0x18)->size; ++index) {       // 0046D7AD..0046D7DD (trimeshes +18)
            void* mesh = elements(list_at(data, 0x18))[index];                 // 0046D7B6..0046D7BD
            if (part_outside_trimesh_0046a6b0(mesh, planes) == 0)              // 0046D7C0..0046D7CC
                add_part_to_mesh_buckets_0046bdf0(mesh);                       // 0046D7CE..0046D7CF
        }
    }
    for (I index = 0; index < list_at(data, 0x30)->size; ++index) {       // 0046D7DF..0046D89C (projections +30)
        void* projector = elements(list_at(data, 0x30))[index];                // 0046D7F0..0046D7F7
        if (part_outside_projection_0046a7e0(projector, planes) != 0) {        // 0046D7FA..0046D806
            *bsp_cells.stat_007fbf38 = *bsp_cells.stat_007fbf38 + 1;           // 0046D890
            continue;
        }
        add_inline_growth(bsp_cells.static_shadow_bucket_007fbf84, projector); // 0046D80C..0046D888
    }
    for (I index = 0; index < list_at(data, 0x54)->size; ++index) {           // 0046D8A2..0046D981 (triggers +54)
        void* trigger = elements(list_at(data, 0x54))[index];                  // 0046D8B0..0046D8B7
        if (part_outside_trigger_0046a850(trigger, planes) != 0) continue;     // 0046D8BA..0046D8C6
        if (count_of(bsp_cells.trigger_bucket_0080aa74, trigger) != 0) continue; // 0046D8CC..0046D8ED
        add_inline_growth(bsp_cells.trigger_bucket_0080aa74, trigger);         // 0046D8F3..0046D975
    }
    for (I index = 0; index < list_at(data, 0x3C)->size; ++index) {           // 0046D987..0046DA4A (+3C)
        void* beam = elements(list_at(data, 0x3C))[index];                     // 0046D994..0046D99B
        if (part_outside_projection_0046a7e0(beam, planes) != 0) {             // 0046D99E..0046D9AA
            *bsp_cells.stat_007fbf3c = *bsp_cells.stat_007fbf3c + 1;           // 0046DA3E
            continue;
        }
        add_inline_growth(bsp_cells.beam_bucket_007fbf6c, beam);               // 0046D9B0..0046DA36
    }
    for (I index = 0; index < list_at(data, 0x60)->size; ++index) {           // 0046DA50..0046DB16 (bins +60)
        void* bin = elements(list_at(data, 0x60))[index];                      // 0046DA60..0046DA67
        if (bin_outside_0046a980(bin, planes) != 0) continue;                  // 0046DA6A..0046DA76
        add_inline_growth(bsp_cells.bin_bucket_0080aa68, bin);                 // 0046DA7C..0046DB05
        *bsp_cells.bins_collected_007fbf28 = *bsp_cells.bins_collected_007fbf28 + 1; // 0046DAFE..0046DB0B
    }
    if (room_emitters.data != 0) live_free(elements(&room_emitters));         // 0046DB1C..0046DB2D
    if (room_objects.data != 0) live_free(elements(&room_objects));           // 0046DB35..0046DB46
}

// 004A7690 GrassSortTris(a, b): qsort comparator over 18h-byte CAurTriangleListElem; unsigned key at +10.
int __cdecl grass_sort_tris_004a7690(const void* a, const void* b) {
    alignas(4) unsigned char left[0x18];                                       // [esp+8]
    alignas(4) unsigned char right[0x18];                                      // [esp+20h]
    triangle_list_elem_copy_construct_004a7340(left, nullptr, a);             // 004A76AA..004A76B3 (list part)
    std::memcpy(left + 0x0C, static_cast<const unsigned char*>(a) + 0x0C, 12); // 004A76B8..004A76C9
    triangle_list_elem_copy_construct_004a7340(right, nullptr, b);            // 004A76CD..004A76DC
    std::memcpy(right + 0x0C, static_cast<const unsigned char*>(b) + 0x0C, 12); // 004A76E1..004A76F2
    U left_key; U right_key;
    std::memcpy(&left_key, left + 0x10, 4);                                    // 004A76F6
    std::memcpy(&right_key, right + 0x10, 4);                                  // 004A76EB
    const int result = left_key < right_key ? -1 : (left_key > right_key ? 1 : 0); // 004A76FA..004A7730 jae/jbe
    triangle_list_elem_destruct_004a72d0(right, nullptr);                      // 004A7706 / 004A7732 / 004A775E
    triangle_list_elem_destruct_004a72d0(left, nullptr);                       // 004A7717 / 004A7743 / 004A776F
    return result;
}

// Original-convention entry of core's ConstructCameraFadePlane 0046B7C0 (scene in EAX, 0046B7C7 mov esi,eax; RET 0),
// forwarding to the agreed reconstruction convention cdecl(scene). D21 naked function.
__declspec(naked) void construct_camera_fade_plane_eax_entry_0046b7c0() {
    __asm {
        push eax
        call construct_camera_fade_plane_0046b7c0
        add esp, 4
        ret
    }
}

} // extern "C"
