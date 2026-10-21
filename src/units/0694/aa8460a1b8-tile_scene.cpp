// lane-3d-bsp fork B: faithful bodies for GLRender::BindTexture, TileData ctor/dtor, ProcessTilePart,
// SpawnPartsForTile, Scene::SpawnTile / SetTileFlames / SetSingleTileFlame / DeleteTile / SetCurrentRoom /
// DoDistortionBuckets / DoBlurBuckets / DoTriggerBucket, the CExoArrayList<CExoArrayList<CAurTriangleBin*>>
// allocate / SetSize pair and InitGrassBins (swkotor.exe GOG K1, base 00400000; local disassembly d/<ADDR>.txt).
//
// D30-FINAL: values the original keeps on the x87 stack are double; float only at the original `fstp dword`.
// CRT: floor 006FC750 / ceil 006FC880 -> UCRT floor/ceil (both exact IEEE operations: identical results for every
// finite, infinite and NaN input, D35 equivalence note); __ftol2 006FAE8C -> ftol2() below (truncation toward zero,
// 0x80000000 "integer indefinite" when out of range); `frndint; fistp dword` -> fistp32() (round to nearest even
// under the CRT-startup RC=nearest control word 007092C0, 0x80000000 when out of range); sprintf 006FADB0 -> UCRT
// sprintf (same as lane-3d-room). eh_vector_constructor_iterator 006FB55F is expressed as its loop (005A76D0 cannot
// throw); eh_vector_destructor_iterator 006FA852 -> strict-link c2da_destroy_array_006fa852.
// Unwind: original EH states (handlers 00715C8A, 00715EDC, 00716673, 00716688, 007166AB) are cited, not emitted:
// every callee is extern "C" and live_operator_new returns null instead of throwing (D30-FINAL).
// Scene vtable slot bodies use the vtables_gen.h raw-word declarations; EAX is returned as the original leaves it
// where determinable, otherwise 0 with a "D2-EAX" label (the original EAX there is a callee's undefined residue).
#include "558510f9c9-tile_scene.h"
#include <cmath>
#include <cstdio>
#include <cstring>

namespace {
using namespace kotor_scene3d_bsp;

inline void* ptr(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* elem(void* list_object, U list_offset, I index) { return elements(list_at(list_object, list_offset))[index]; }

// __ftol2 006FAE8C: truncation toward zero of ST0; out-of-range / NaN -> 0x80000000 (x87 integer indefinite).
inline I ftol2(double v) {
    if (v > -2147483649.0 && v < 2147483648.0) return static_cast<I>(v);
    return static_cast<I>(0x80000000u);
}
// frndint + fistp dword (RC nearest): round half to even; out-of-range / NaN -> 0x80000000.
inline I fistp32(double v) {
    const double r = std::nearbyint(v);
    if (r >= -2147483648.0 && r < 2147483648.0) return static_cast<I>(r);
    return static_cast<I>(0x80000000u);
}

constexpr U kNinety_0073e40c = 0x42B40000;      // 90.0f
constexpr U kGlKeep_0073f2c8 = 0x1E00;          // GL_KEEP
constexpr U kGlReplace_0073f2d0 = 0x1E01;       // GL_REPLACE
constexpr U kGlAlways_0073f2bc = 0x0207;        // GL_ALWAYS
constexpr U kGlMaxAnisotropy = 0x84FE;          // 00420467 push 84FEh
constexpr char kEmpty_0073d71c[] = "";          // 00454A9F push 73D71Ch
constexpr char kSuffixA_0073d5d4[2] = {'a', 0}; // 00454A7B mov cx,word ptr [0073D5D4] (0x0061)
constexpr char kTileFormat_007415fc[] = "(%d,%d)";   // 00454AEF push 7415FCh
constexpr char kMl1_007415d4[4] = {'m', 'l', '1', 0};
constexpr char kMl2_007415d0[4] = {'m', 'l', '2', 0};
constexpr char kSl1_007415cc[4] = {'s', 'l', '1', 0};
constexpr char kSl2_007415c8[4] = {'s', 'l', '2', 0};

// strcpy byte loop + strlen scan + one DWORD store of the 4-byte suffix (004528B8..004528E4 and the three repeats).
inline void name_with_suffix(char* buffer, const char* name, const char (&suffix)[4]) {
    std::size_t index = 0;
    char c;
    do { c = name[index]; buffer[index] = c; ++index; } while (c != 0);
    char* end = buffer;
    while (*end != 0) ++end;
    std::memcpy(end, suffix, 4);
}

void tile_scene_add_inline(void* object, U list_offset, void* value) { add_inline_growth(list_at(object, list_offset), value); }

// Shared body of the two neighbour passes of Scene::SpawnTile (00454F36..004550BE / 00455126..0045531E).
// `inline_lists` selects the inlined operator-new growth on +5C (second pass) instead of the 005E03C0 owner.
void link_neighbour(void* tile, void* neighbour, bool inline_lists) {
    if (neighbour == nullptr || neighbour == tile) return;                    // 00454F2E / 00454F36
    if (inline_lists) {
        tile_scene_add_inline(tile, 0x5C, neighbour);                          // 0045512E..0045518D
        tile_scene_add_inline(neighbour, 0x5C, tile);                          // 00455190..004551F8
    } else {
        add_with_growth(list_at(tile, 0x5C), neighbour);                       // 00454F3E..00454F69
        add_with_growth(list_at(neighbour, 0x5C), tile);                       // 00454F6C..00454F97
    }
    for (I index = 0; index < list_at(tile, 0x80)->size; ++index)             // 00454F9A..0045502E (+84 re-read)
        tile_scene_add_inline(neighbour, 0x74, elem(tile, 0x80, index));      // value read first (00454FB6 [esp+58h])
    for (I index = 0; index < list_at(neighbour, 0x80)->size; ++index)        // 00455030..004550BE
        tile_scene_add_inline(tile, 0x74, elem(neighbour, 0x80, index));
}
} // namespace

extern "C" {

// 00420440 GLRender::BindTexture(target_index, texture), cdecl.
void __cdecl gl_render_bind_texture_00420440(U target_index, U texture) {
    using namespace kotor_renderer_init;
    glBindTexture_0073d240(*bsp_cells.gl_enum_table_0078d448[target_index], texture);           // 00420445..00420453
    glTexParameteri_0073d23c(*bsp_cells.gl_enum_table_0078d448[target_index], kGlMaxAnisotropy,   // 0042045F re-read
                             *bsp_cells.anisotropy_007a685c);                                     // 00420459..0042046D
    bsp_cells.current_tid_007a683c[*bsp_cells.current_stage_007a6898] = texture;                  // 00420473..00420478
}

// 00444E20 TileData::~TileData, thiscall RET 0 (EH 00715C8A states 4..0 cited).
void __fastcall tile_data_destruct_00444e20(void* tile, void*) {
    void* owned = field<void*>(tile, 0x24);                                                       // 00444E3D
    if (owned != nullptr) slots<DeletingSlot>(owned).destroy(owned, nullptr, 1);                  // 00444E4C..00444E50
    if (void* p = elements(list_at(tile, 0x80))) live_free(p);                                    // 00444E52..00444E62
    if (void* p = elements(list_at(tile, 0x74))) live_free(p);                                    // 00444E6A..00444E77
    if (void* p = elements(list_at(tile, 0x68))) live_free(p);                                    // 00444E7F..00444E8C
    if (void* p = elements(list_at(tile, 0x5C))) live_free(p);                                    // 00444E94..00444EA1
    if (void* p = elements(list_at(tile, 0x18))) live_free(p);                                    // 00444EA9..00444EB9
}

// 004473B0 TileData::TileData, thiscall RET 0, EAX = this (store order of the original kept).
void* __fastcall tile_data_construct_004473b0(void* tile, void*) {
    static const U kZeroFirst[] = {0x1C, 0x20, 0x18, 0x28, 0x2C, 0x30, 0x38, 0x3C, 0x40, 0x44, 0x48, 0x4C, 0x60, 0x64,
                                   0x5C, 0x6C, 0x70, 0x68, 0x78, 0x7C, 0x74, 0x84, 0x88, 0x80, 0x24, 0x00, 0x04, 0x08,
                                   0x0C, 0x10, 0x14, 0x34, 0x58};                                 // 004473CC..00447434
    for (U offset : kZeroFirst) field<U>(tile, offset) = 0;
    field<U8>(tile, 0x90) = 0;                                                                    // 00447437
    field<U>(tile, 0x50) = 0x3F800000;                                                            // 00447442 1.0f
    field<U>(tile, 0x54) = 0x3F800000;                                                            // 00447445
    field<U>(tile, 0x94) = 0x3F800000;                                                            // 00447448
    for (U offset = 0x38; offset <= 0x4C; offset += 4) field<U>(tile, offset) = 0x49742400;      // 0044744E..004474B3 1e6f
    field<U>(tile, 0x8C) = 4;                                                                     // 004474A9
    return tile;
}

// 0044FC20 Scene::SetTileFlames(Vector* position, CAurObject* first, CAurObject* second), Scene vtbl+2C, RET 0Ch.
U __fastcall scene_set_tile_flames_0044fc20(void* scene, void*, U position, U first, U second) {
    void* tile = scene_bsp_get_tile_data_0046a680(scene, static_cast<const Vector*>(ptr(position)));   // 0044FC29..0044FC2B
    scene_slots(scene).s11c_scene_set_single_tile_flame(scene, nullptr, first, word(at(tile, 0x10)),
                                                        field<U>(tile, 0x08));                     // 0044FC34..0044FC46
    if (first != 0) field<void*>(ptr(first), 0x170) = tile;                                       // 0044FC4C..0044FC50
    const U result = scene_slots(scene).s11c_scene_set_single_tile_flame(scene, nullptr, second, word(at(tile, 0x14)),
                                                                         field<U>(tile, 0x0C));    // 0044FC56..0044FC67
    if (second != 0) field<void*>(ptr(second), 0x170) = tile;                                     // 0044FC6D..0044FC71
    return result;                                                                                // EAX = slot +11C result
}

// 0044FCA0 Scene::DeleteTile(Vector* position), Scene vtbl+34, RET 4.
U __fastcall scene_delete_tile_0044fca0(void* scene, void*, U position) {
    scene_bsp_remove_tiledata_0046d2f0(scene, static_cast<const Vector*>(ptr(position)));        // 0044FCA4..0044FCA6
    return 0;   // D2-EAX: original returns the residue EAX of SceneBSPRemoveTiledata (void)
}

// 004503A0 Scene::DoDistortionBuckets, Scene vtbl+E4, RET 0.
U __fastcall scene_do_distortion_buckets_004503a0(void*, void*) {
    using namespace kotor_renderer_init;
    glEnable_0073d390(*bsp_cells.gl_stencil_test_0078d470);                                      // 004503A0..004503A9
    glColorMask_0073d358(0, 0, 0, 0);                                                             // 004503B5..004503BD
    glStencilOp_0073d270(kGlKeep_0073f2c8, kGlKeep_0073f2c8, kGlReplace_0073f2d0);              // 004503BF..004503CD
    glStencilMask_0073d278(1);                                                                    // 004503D9..004503DB
    glStencilFunc_0073d298(kGlAlways_0073f2bc, 1, 0xFF);                                          // 004503DD..004503EB
    PointerList* bucket = bsp_cells.distortion_bucket_007fbfc0;
    for (I index = 0; index < bucket->size; ++index) {                                            // 004503F1..00450417
        void* gob = elements(bucket)[index];                                                      // 00450400..00450405
        gob_slots(gob).s010_gob_render(gob, nullptr, 0);                                          // 00450408..0045040C (Gob::Render(0))
    }
    glColorMask_0073d358(1, 1, 1, 1);                                                             // 00450419..00450421
    glStencilMask_0073d278(0xFF);                                                                 // 00450423..00450428
    glDisable_0073d22c(*bsp_cells.gl_stencil_test_0078d470);                                     // 0045042A..00450430
    return 0;   // D2-EAX: residue of glDisable
}

// 004504D0 Scene::DoBlurBuckets, Scene vtbl+DC, RET 0.
U __fastcall scene_do_blur_buckets_004504d0(void* scene, void*) {
    PointerList* bucket = bsp_cells.blur_bucket_007fbf90;
    I size = bucket->size;                                                                        // 004504D0
    *bsp_cells.gob_bucket_snapshot_007b92ec = size;                                               // 004504DD
    if (size > 0) {                                                                               // 004504E2
        I index = 0;
        do {
            (*bsp_cells.callback_0078e39c)();                                                     // 004504E5
            void* gob = elements(bucket)[index];                                                  // 004504EB..004504F0
            const U radius_bits = field<U>(gob, 0x138);                                           // 004504F3
            void* lights = field<void*>(scene, 0x80);
            slots<LightManagerSlots>(lights).setup_lights_for_this_gob(lights, nullptr, at(gob, 0x78), radius_bits); // 004504F9..00450506
            gob = elements(bucket)[index];                                                        // 00450509..0045050F
            lights = field<void*>(scene, 0x80);
            slots<LightManagerSlots>(lights).prioritize_lights(lights, nullptr, static_cast<const Vector*>(at(gob, 0x78)), 0); // 00450512..00450520
            lights = field<void*>(scene, 0x80);
            slots<LightManagerSlots>(lights).enable_highest_priority_lights(lights, nullptr, 3);  // 00450522..0045052C
            gob = elements(bucket)[index];                                                        // 0045052F..00450535
            gob_slots(gob).s14c_gob_render_blur(as_gob(gob), nullptr);                           // 00450538..0045053A
            size = bucket->size;                                                                  // 00450540
            ++index;                                                                              // 00450545
        } while (index < size);                                                                   // 00450546..00450548
    }
    return static_cast<U>(size);                                                                  // EAX = last [007FBF94] read
}

// 00450AC0 Scene::DoTriggerBucket, Scene vtbl+114, RET 0 (deferred: not on the menu path).
U __fastcall scene_do_trigger_bucket_00450ac0(void*, void*) {
    using namespace kotor_renderer_init;
    if (*bsp_cells.gate_0078e614 == 0) return 0;                                                  // 00450AC0..00450AC7 (EAX = gate = 0)
    glDisable_0073d22c(*bsp_cells.gl_enum_table_0078d448[0]);                                     // 00450AC9..00450AD6 (0078D448)
    glDisable_0073d22c(*bsp_cells.gl_texture_2d_0078d458);                                        // 00450AD8..00450ADF
    glDisable_0073d22c(*bsp_cells.gl_lighting_0078d45c);                                          // 00450AE1..00450AE8
    PointerList* bucket = bsp_cells.trigger_bucket_0080aa74;
    for (I index = 0; index < bucket->size; ++index) {                                            // 00450AEA..00450B21
        void* trigger = elements(bucket)[index];                                                  // 00450B00..00450B05
        if (field<U8>(trigger, 0xA8) == 0)                                                        // 00450B08..00450B10
            slots<PartDrawSlot>(trigger).draw(trigger, nullptr, 0);                               // 00450B12..00450B16
    }
    glEnable_0073d390(*bsp_cells.gl_enum_table_0078d448[0]);                                      // 00450B23..00450B2F
    glEnable_0073d390(*bsp_cells.gl_texture_2d_0078d458);                                         // 00450B31..00450B38
    glEnable_0073d390(*bsp_cells.gl_lighting_0078d45c);                                           // 00450B3A..00450B41
    return 0;   // D2-EAX: residue of glEnable
}

// 004528B0 ProcessTilePart(TileData*, Part*, name), cdecl, AL.
bool __cdecl process_tile_part_004528b0(void* tile, void* part, const char* name) {
    char buffer[0x40];                                                                            // [esp+10h] (esp0-40h)
    name_with_suffix(buffer, name, kMl1_007415d4);                                                // 004528B8..004528E4
    aurora_model::MdlNode* node = field<aurora_model::MdlNode*>(part, 0x04);                     // 004528E1
    aurora_model::MaxTree* tree = field<aurora_model::MaxTree*>(node, 0x08);                     // 004528E6
    const I name_index = field<I>(node, 0x04);                                                    // 004528F3
    const char* node_name;
    if (aurora_tree_as_model_0043e1c0(tree, nullptr) != nullptr) {                                // 004528E9..004528F6
        void* model = aurora_tree_as_model_0043e1c0(field<aurora_model::MaxTree*>(node, 0x08), nullptr); // 004528F0..004528F8
        node_name = static_cast<const char**>(field<void*>(model, 0xB8))[name_index];             // 004528FD..00452908
    } else {
        void* animation = aurora_tree_as_animation_0043e1d0(field<aurora_model::MaxTree*>(node, 0x08), nullptr); // 0045290D
        void* model = field<void*>(animation, 0x84);                                              // 00452912
        node_name = static_cast<const char**>(field<void*>(model, 0xB8))[name_index];             // 00452918..00452923
    }
    if (kotor_stricmp_0070acaf(node_name, buffer) == 0) {                                          // 00452926..00452936
        field<void*>(tile, 0x00) = slots<PartSlots>(part).as_light(part, nullptr);                // 00452938..00452947
        return true;
    }
    name_with_suffix(buffer, name, kMl2_007415d0);                                                // 00452950..0045297B
    if (kotor_stricmp_0070acaf(aurora_node_translate_name_00438a00(field<aurora_model::MdlNode*>(part, 0x04), nullptr),
                               buffer) == 0) {                                                    // 0045297D..0045298D
        field<void*>(tile, 0x04) = slots<PartSlots>(part).as_light(part, nullptr);                // 0045298F..0045299E
        return true;
    }
    name_with_suffix(buffer, name, kSl1_007415cc);                                                // 004529A8..004529D6
    if (kotor_stricmp_0070acaf(aurora_node_translate_name_00438a00(field<aurora_model::MdlNode*>(part, 0x04), nullptr),
                               buffer) == 0) {                                                    // 004529D8..004529E8
        field<void*>(tile, 0x08) = part;                                                          // 004529F0
        return true;
    }
    name_with_suffix(buffer, name, kSl2_007415c8);                                                // 004529FB..00452A26
    if (kotor_stricmp_0070acaf(aurora_node_translate_name_00438a00(field<aurora_model::MdlNode*>(part, 0x04), nullptr),
                               buffer) == 0) {                                                    // 00452A28..00452A38
        field<void*>(tile, 0x0C) = part;                                                          // 00452A40
        return true;
    }
    return false;                                                                                 // 00452A4E
}

// 00452C90 Scene::SetCurrentRoom(int index), Scene vtbl+7C, RET 4.
U __fastcall scene_set_current_room_00452c90(void* scene, void*, U index_word) {
    const I index = static_cast<I>(index_word);
    if (index >= 0 && index < field<I>(scene, 0xCC)) {                                            // 00452C94..00452C9E
        void* room = elements(list_at(scene, 0xC8))[index];                                       // 00452CA0..00452CA6
        field<void*>(scene, 0xD4) = room;                                                         // 00452CA9
        return word(room);                                                                        // EAX = room
    }
    field<void*>(scene, 0xD4) = nullptr;                                                          // 00452CB2
    return index_word;                                                                            // EAX = index
}

// 00454A20 SpawnPartsForTile(name, position*, orientation*, rotation, skip_node_lookup, parts*), cdecl.
void* __cdecl spawn_parts_for_tile_00454a20(const char* name, const Vector* position, const Quaternion* orientation,
                                            I texture_rotate, I instance, PointerList* parts) {
    aurora_model::MaxTree* model = aurora_find_model_00464110(name);                              // 00454A2B..00454A2C
    if (model == nullptr) return nullptr;                                                         // 00454A36..00454A41
    aur_texture_set_mode_static_004204b0();                                                       // 00454A42
    char buffer[0x40];                                                                            // [esp+10h]
    aurora_model::Gob* gob = nullptr;                                                             // 00454A4F ebp = 0
    aurora_model::Part* tree = nullptr;
    bool found = false;
    if (instance == 0) {                                                                          // 00454A51..00454A53
        const char* model_name = static_cast<const char*>(at(model, 0x08));
        std::size_t index = 0;
        char c;
        do { c = model_name[index]; buffer[index] = c; ++index; } while (c != 0);                 // 00454A55..00454A68
        char* end = buffer;
        while (*end != 0) ++end;                                                                  // 00454A6A..00454A76
        std::memcpy(end, kSuffixA_0073d5d4, 2);                                                   // 00454A7B / 00454A88 (word store)
        if (find_node_00452000(field<aurora_model::MdlNode*>(model, 0x28), buffer) != nullptr) {  // 00454A78..00454A95
            gob = aurora_new_aur_object_00449cc0(name, kEmpty_0073d71c, nullptr, nullptr);       // 00454A97..00454AA5
            tree = field<aurora_model::Part*>(gob, 0x6C);                                         // 00454AAC
            found = true;
        }
    }
    if (!found) {
        tree = aurora_create_instance_tree_00449410(field<aurora_model::MdlNode*>(model, 0x28), nullptr); // 00454AB4..00454ABA
        const I y = ftol2(std::floor(W(position->y)));                                            // 00454ABF..00454ACF
        const I x = ftol2(std::floor(W(position->x)));                                            // 00454AD4..00454AE5
        std::sprintf(buffer, kTileFormat_007415fc, x, y);                                         // 00454AE6..00454AF5
        aurora_part_init_material_00442d70(nullptr, tree, buffer);                               // 00454AFA..00454B02
    }
    set_part_texture_rotate_00452500(tree, texture_rotate);                                       // 00454B0A..00454B10
    expand_reference_parts_00454470(tree);                                                        // 00454B15..00454B16
    aurora_collapse_part_tree_00453f10(gob, tree, parts, *position, *orientation, nullptr, nullptr); // 00454B1B..00454B60
    aur_texture_set_mode_dynamic_004204c0();                                                      // 00454B68
    return gob;                                                                                   // 00454B6F EAX = ebp
}

// 00454C80 Scene::SpawnTile(name, Vector* position, int rotation, flag), Scene vtbl+24, RET 10h; EAX = Gob.
U __fastcall scene_spawn_tile_00454c80(void* scene, void*, U name_word, U position_word, U rotation_word, U flag) {
    const char* name = static_cast<const char*>(ptr(name_word));
    const Vector* position = static_cast<const Vector*>(ptr(position_word));
    const I rotation = static_cast<I>(rotation_word);
    Quaternion orientation;                                                                       // [esp+44h] (esp0-1Ch)
    yaw_pitch_roll_004acac0(&orientation, S(static_cast<double>(rotation) * Wb(kNinety_0073e40c)), // 00454C98..00454CB7
                            bits_to_float(0), bits_to_float(0));
    PointerList parts;                                                                            // [esp+24h] (esp0-40h)
    parts.size = 0; parts.capacity = 0; parts.data = 0;                                           // 00454CBC..00454CC4
    void* gob = spawn_parts_for_tile_00454a20(name, position, &orientation, rotation, static_cast<I>(flag), &parts); // 00454CC8..00454CEA
    void* memory = live_operator_new(0x98);                                                       // 00454CF1..00454CFD
    void* tile = memory != nullptr ? tile_data_construct_004473b0(memory, nullptr) : nullptr;     // 00454D09..00454D1D
    field<U>(tile, 0x28) = float_to_bits(position->x);                                            // 00454D21..00454D38 raw copies
    field<U>(tile, 0x2C) = float_to_bits(position->y);
    field<U>(tile, 0x30) = float_to_bits(position->z);
    field<U>(tile, 0x34) = flag;                                                                  // 00454D3B
    if (gob != nullptr) process_static_object_0044fac0(gob, scene, tile, position, &orientation);  // 00454D3E..00454D4D
    process_static_parts_00454b80(&parts, scene, tile, name, flag);                              // 00454D55..00454D66
    // static-part bounds: first part answering vtbl+2C seeds +38/+44 (00454D6B..00454DEC), the rest widen them.
    PointerList* statics = list_at(tile, 0x18);
    I index = 0;
    if (statics->size > 0) {                                                                      // 00454D73..00454D75
        for (;;) {                                                                                // 00454D80..00454D95
            void* part = elements(statics)[index];
            if (slots<PartSlots>(part).as_trimesh(part, nullptr) != nullptr) break;
            ++index;
            if (!(index < statics->size)) break;
        }
        void* part = elements(statics)[index];                                                    // 00454D97 (index may equal size: original reads it)
        if (slots<PartSlots>(part).as_trimesh(part, nullptr) != nullptr) {                         // 00454D9F..00454DA4
            part = elements(statics)[index];
            void* mesh = slots<PartSlots>(part).as_trimesh(part, nullptr);                        // 00454DAA..00454DB5
            std::memcpy(at(tile, 0x38), at(mesh, 0x4C), 12);                                      // 00454DB8..00454DCA
            part = elements(statics)[index];
            mesh = slots<PartSlots>(part).as_trimesh(part, nullptr);                              // 00454DCD..00454DD8
            std::memcpy(at(tile, 0x44), at(mesh, 0x58), 12);                                      // 00454DDB..00454DEC
            for (++index; index < statics->size; ++index) {                                       // 00454DEF..00454EB7
                part = elements(statics)[index];
                void* other = slots<PartSlots>(part).as_trimesh(part, nullptr);                   // 00454E00..00454E0B
                if (other == nullptr) continue;
                Vector out;
                const Vector* minimum = vector_min_004aae70(&out, field<Vector>(tile, 0x38), field<Vector>(other, 0x4C)); // 00454E15..00454E49
                std::memcpy(at(tile, 0x38), minimum, 12);                                         // 00454E4E..00454E5D
                Vector out2;
                const Vector* maximum = vector_max_004aaee0(&out2, field<Vector>(tile, 0x44), field<Vector>(other, 0x58)); // 00454E60..00454E98
                std::memcpy(at(tile, 0x44), maximum, 12);                                         // 00454E9D..00454EAE
            }
        }
    }
    scene_bsp_add_tile_data_0046ed00(scene, tile, *position);                                     // 00454EBD..00454EDC
    // neighbours along x (counter -1, +1; 00454EE4..004550CE): 005E03C0 growth on +5C.
    for (I counter = -1; counter <= 1; counter += 2) {
        const float offset = S(static_cast<double>(counter) * W(bsp_tile_spacing_0078e424));     // 00454EF0..00454F03
        Vector probe;                                                                             // [esp+20h] (esp0-34h)
        probe.x = S(W(offset) + W(position->x));                                                  // 00454F0D..00454F18
        probe.y = S(W(position->y));                                                              // 00454F0A / 00454F1C
        probe.z = S(W(position->z));                                                              // 00454F07 / 00454F20
        link_neighbour(tile, scene_bsp_get_tile_data_0046a680(scene, &probe), false);            // 00454F24..004550BE
    }
    // neighbours along y (004550D4..0045532E): inlined operator-new growth on +5C.
    for (I counter = -1; counter <= 1; counter += 2) {
        Vector probe;
        probe.x = position->x;                                                                    // 004550E8 / 004550F4 raw copy
        const float offset = S(static_cast<double>(counter) * W(bsp_tile_spacing_0078e424));     // 004550E0..004550F8 (stored over the name slot)
        probe.y = S(W(offset) + W(position->y));                                                  // 004550FF..0045510C
        probe.z = S(W(position->z));                                                              // 004550FC / 00455110
        link_neighbour(tile, scene_bsp_get_tile_data_0046a680(scene, &probe), true);             // 00455114..0045531E
    }
    if (void* data = elements(&parts)) live_free(data);                                           // 00455334..00455345
    return word(gob);                                                                             // 00455351 EAX = [esp+64h] (gob, 00454CF6)
}

// 00455370 Scene::SetSingleTileFlame(CAurObject* object, Gob** slot, Part* part), Scene vtbl+11C, RET 0Ch.
U __fastcall scene_set_single_tile_flame_00455370(void* scene, void*, U object_word, U slot_word, U part_word) {
    void* object = ptr(object_word);
    void** slot = static_cast<void**>(ptr(slot_word));
    PointerList lights;                                                                           // [esp+10h]
    lights.size = 0; lights.capacity = 0; lights.data = 0;                                        // 00455390..00455398
    if (*slot == object) return 0;   // 004553A7..004553AD; D2-EAX (SEH-frame residue)
    if (*slot != nullptr) {                                                                       // 004553B3
        gob_collect_lights_00447180(*slot, nullptr, &lights);                                     // 004553B7..004553BC
        for (I index = 0; index < lights.size; ++index)                                           // 004553C1..004553E8
            update_static_lighting_0046ca30(scene, elements(&lights)[index]);                     // 004553D0..004553D9
        void* previous = *slot;                                                                   // 004553EA
        if (previous != nullptr) {
            slots<DeletingSlot>(previous).destroy(previous, nullptr, 1);                          // 004553F1..004553F5
            *slot = nullptr;                                                                      // 004553F7
        }
    }
    void* part = ptr(part_word);
    if (object != nullptr && part != nullptr) {                                                   // 004553FE..00455408
        std::memcpy(at(object, 0x78), at(part, 0x08), 12);                                        // 0045540A..0045541D
        std::memcpy(at(object, 0x84), at(part, 0x14), 16);                                        // 00455420..0045543F
        gob_set_scene_0043f200(object, nullptr, scene);                                           // 0045543C..00455442
        gob_collect_lights_00447180(object, nullptr, &lights);                                    // 00455447..0045544E (list not reset)
        for (I index = 0; index < lights.size; ++index)                                           // 00455453..00455478
            update_static_lighting_0046ca30(scene, elements(&lights)[index]);                     // 00455460..00455469
        *slot = object;                                                                           // 0045547A
    }
    void* data = elements(&lights);                                                               // 0045547D
    if (data == nullptr) return 0;                                                                // EAX = list data (0)
    live_free(data);                                                                              // 0045548D..0045548E
    return 0;   // D2-EAX: residue of _free
}

// 004555B0 CExoArrayList<CExoArrayList<CAurTriangleBin*>>::allocate(int), thiscall RET 4 (EH 007166AB cited).
void __fastcall bin_lists_allocate_004555b0(PointerList* list, void*, I size) {
    unsigned char* previous = static_cast<unsigned char*>(static_cast<void*>(elements(list)));    // 004555CF
    list->capacity = size;                                                                        // 004555DC
    void* memory = live_operator_new(static_cast<U>(size) * 12u + 4u);                            // 004555D1..004555DF
    unsigned char* first = nullptr;
    if (memory != nullptr) {                                                                      // 004555ED..004555F3
        field<I>(memory, 0) = size;                                                               // 00455606 count cookie
        first = static_cast<unsigned char*>(memory) + 4;                                          // 00455600
        for (I index = 0; index < size; ++index) {                                                // 006FB55F loop with ctor 005A76D0
            PointerList* element = reinterpret_cast<PointerList*>(first + index * 12);
            element->size = 0; element->capacity = 0; element->data = 0;                          // 005A76D2..005A76D8
        }
    }
    list->data = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(first));             // 00455610
    for (I index = 0; index < list->size; ++index)                                                // 00455612..00455638
        exo_array_list_assign_0046bc80(reinterpret_cast<PointerList*>(static_cast<unsigned char*>(static_cast<void*>(elements(list))) + index * 12),
                                       nullptr, reinterpret_cast<PointerList*>(previous + index * 12)); // 00455622..0045562A
    if (previous != nullptr) {                                                                    // 0045563A
        c2da_destroy_array_006fa852(previous, 0x0C, *reinterpret_cast<std::int32_t*>(previous - 4),
                                    exo_array_list_free_0047abc0);                                // 0045563E..0045564D
        live_free(previous - 4);                                                                  // 00455652..00455653
    }
}

// 00456A00 CExoArrayList<CExoArrayList<CAurTriangleBin*>>::SetSize(int), thiscall RET 4.
void __fastcall bin_lists_set_size_00456a00(PointerList* list, void*, I size) {
    if (size != 0) {                                                                              // 00456A05..00456A0A
        bin_lists_allocate_004555b0(list, nullptr, size);                                         // 00456A42..00456A45
        list->size = size;                                                                        // 00456A4A
        return;
    }
    unsigned char* data = static_cast<unsigned char*>(static_cast<void*>(elements(list)));        // 00456A0C
    if (data != nullptr) {                                                                        // 00456A10
        c2da_destroy_array_006fa852(data, 0x0C, *reinterpret_cast<std::int32_t*>(data - 4),
                                    exo_array_list_free_0047abc0);                                // 00456A12..00456A22
        live_free(data - 4);                                                                      // 00456A27..00456A28
        list->data = 0;                                                                           // 00456A30
    }
    list->size = 0;                                                                               // 00456A33
    list->capacity = 0;                                                                           // 00456A36
}

// 00456E00 InitGrassBins(Vector minimum, Vector maximum, CExoArrayList<CExoArrayList<CAurTriangleBin*>>* bins), cdecl.
void __cdecl init_grass_bins_00456e00(Vector minimum, Vector maximum, PointerList* bins) {
    bins->size = 0;                                                                               // 00456E13
    const float ceil_x = S(std::ceil((W(maximum.x) - W(minimum.x)) / W(*bsp_cells.tile_step_0078e428))); // 00456E03..00456E27
    const I columns = fistp32(W(ceil_x));                                                         // 00456E36..00456E40
    const float ceil_y = S(std::ceil((W(maximum.y) - W(minimum.y)) / W(*bsp_cells.tile_step_0078e428)));                      // 00456E42..00456E5B
    const I rows = fistp32(W(ceil_y));                                                            // 00456E6A..00456E74
    if (columns < 0 || rows < 0) return;                                                          // 00456E76..00456E86
    bin_lists_set_size_00456a00(bins, nullptr, columns);                                          // 00456E8C..00456E8F
    for (I column = 0; column < columns; ++column) {                                              // 00456E94..00456F17
        PointerList* inner = reinterpret_cast<PointerList*>(
            static_cast<unsigned char*>(static_cast<void*>(elements(bins))) + column * 12);
        inner->size = 0;                                                                          // 00456EA6..00456EA8
        inner = reinterpret_cast<PointerList*>(
            static_cast<unsigned char*>(static_cast<void*>(elements(bins))) + column * 12);       // 00456EB0..00456EB2
        if (rows == 0) {                                                                          // 00456EB4..00456EB8
            if (void* data = elements(inner)) { live_free(data); inner->data = 0; }               // 00456EBA..00456EC9
            inner->capacity = 0;                                                                  // 00456ECB
        } else {
            menu_list_growth::allocate_005e03c0(inner, nullptr, static_cast<U>(rows));            // 00456ED4..00456ED7
        }
        inner->size = rows;                                                                       // 00456EDC
        for (I row = 0; row < rows; ++row) {                                                      // 00456EDF..00456F03
            PointerList* cell = reinterpret_cast<PointerList*>(
                static_cast<unsigned char*>(static_cast<void*>(elements(bins))) + column * 12);
            elements(cell)[row] = nullptr;                                                        // 00456EF0..00456EF5
        }
    }
}

} // extern "C"
