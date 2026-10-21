// lane-m46-3dcore fork C2: Scene vtable bucket / shadow / face bodies of swkotor.exe (GOG K1, image base 00400000).
//   00450970 Scene::DoStaticMeshBeams        (Scene vtbl+108)
//   00452CC0 Scene::AddFacesToBins           (Scene vtbl+128, RET 0Ch)
//   004531A0 Scene::ListSpecialPartsR        (RET 4; recursion the compiler unrolled five levels deep)
//   00453950 Scene::DoGobShadows             (Scene vtbl+F8)
//   00453BB0 Scene::DoDynamicMeshProjections (Scene vtbl+10C)
//   00455A40 Scene::DoCapBucket              (Scene vtbl+EC)
//   00455B60 Scene::DoFadeBuckets            (Scene vtbl+F4)
//   00456250 Scene::DoStaticMeshShadows      (Scene vtbl+104)
// Evidence: dumpbin /disasm of the pinned swkotor.exe (local d/ADDR.txt); every behaviour cites its instructions.
// Conventions: D1 (thiscall -> fastcall, EDX ignored), raw-word Scene slot declarations of vtables_gen.h kept exactly,
// D24/D41 typed object-pointer views of receiver vtables (vtable reloaded per call, no function-pointer cast),
// D30-FINAL (x87 wide values are double, float only at original `fstp dword`, frndint+fistp = round-to-nearest via
// cvtsd2si), M41 allocator family (006FA7E6 -> live_operator_new, 006FA390 -> live_free).
// EAX of the void-like Scene slots: where the original leaves EAX as an unrelated callee/free result the body returns
// 0 (D2-style labelled deviation, "D2:EAX" below); callers in the Scene render path ignore it (RenderSinglePass
// 004514F0 / Render 004512D0 never read EAX after these slots).
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <gl/GL.h>
#include <cmath>
#include <cstddef>
#include <emmintrin.h>
#include "11b317539a-bsp.h"
#include "8079180b4a-m46_scene_externs_c2.h"

namespace {
using namespace kotor_scene3d;
using kotor_scene3d_bsp::bsp_cells;
using kotor_scene3d_bsp::W;
using kotor_scene3d_bsp::S;
using kotor_scene3d_bsp::Wb;
using kotor_scene3d_bsp::elements;
using kotor_scene3d_bsp::list_at;
using kotor_scene3d_bsp::PartSlots;
using kotor_scene3d_bsp::LightManagerSlots;

// ---- typed object-pointer views of receiver vtables (D24). The table pointer is reloaded at every call.
template<class Slots> inline const Slots& view(void* object) {
    return *static_cast<const Slots*>(*static_cast<void* const volatile*>(object));
}
// Part family draw slots (+60 prepare, +64 draw(flag)); +64 = scene3d.h part_draw_004714b0 type.
struct PartDrawSlots {
    const void* o00[24];                                         // +00..+5C
    void (__fastcall* prepare_60)(void* self, void*);            // +60 (00455ACD, 00455D1D)
    U (__fastcall* draw_64)(void* self, void*, U flag);          // +64 RET 4 (0045099C, 00455AD6, 00455E0C)
};
static_assert(offsetof(PartDrawSlots, prepare_60) == 0x60 && offsetof(PartDrawSlots, draw_64) == 0x64, "Part draw");
// LightManager slots used here (table 00743958; member types as construct3d.h LightManagerVTable +08 / +0C).
struct LightManagerC2Slots {
    const void* o00[2];                                          // +00..+04
    void (__fastcall* enable_lights_dynamic_only)(void* self, void*);   // +08 004A4480 (00455BAE)
    void (__fastcall* add_light)(void* self, void*, void* light);       // +0C 004A39F0 RET 4 (004531CB)
};
static_assert(offsetof(LightManagerC2Slots, add_light) == 0x0C, "LightManager +0C");
// Dynamic mesh projector entries of bucket 007FBFB4 (no published table): +5C project, +80 range test.
struct ProjectorSlots {
    const void* o00[23];                                         // +00..+58
    void (__fastcall* project_5c)(void* self, void*, Vector* light_local, U radius_bits, Plane* plane, U flag); // 00453D9A (4 args)
    const void* o60[8];                                          // +60..+7C
    U (__fastcall* in_range_80)(void* self, void*, Vector light_position, U radius_bits);                     // 00453CBE (4 dwords)
};
static_assert(offsetof(ProjectorSlots, project_5c) == 0x5C && offsetof(ProjectorSlots, in_range_80) == 0x80, "proj");
// Object at Gob+188 (no published table): +14 query, no stack arguments (00453ACC).
struct Gob188Slots {
    const void* o00[5];                                          // +00..+10
    U (__fastcall* query_14)(void* self, void*);                 // +14
};
static_assert(offsetof(Gob188Slots, query_14) == 0x14, "Gob+188 +14");

inline void* ptr_of(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
inline U word_of(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline U* words(const PointerList* list) { return reinterpret_cast<U*>(static_cast<std::uintptr_t>(list->data)); }
// x87 `frndint ; fistp dword` under the original control word (RC nearest): cvtsd2si (MXCSR nearest) gives the same
// integer, including the integer-indefinite 80000000h for NaN / out-of-range values.
inline I fistp_nearest(double value) { return _mm_cvtsd_si32(_mm_set_sd(value)); }

// .rdata words read by the original through memory operands (values from the pinned image).
constexpr U kZero_0073d700 = 0x00000000;           // 0.0f
constexpr U kOne_0073d764 = 0x3F800000;            // 1.0f
constexpr U kPlaneLift_007415bc = 0x3D4CCCCD;      // 0.05f
constexpr U kThird_007415f4 = 0x3EAAAAAB;          // 0.33333334f
constexpr U kMilli = 0x3A83126F;                   // 0.001f immediate (00452EDF / 00452EFA / 00452F0D)
constexpr U kFogEndBias_0073e798 = 0x3DCCCCCD;     // 0.1f
constexpr U kMeshResetValue_00740a70 = 4;          // .rdata 00740A70
constexpr U kGlFront_0073f2ec = 0x0404;            // GL_FRONT
constexpr U kGlEmission_0073f30c = 0x1600;         // GL_EMISSION (glColorMaterial mode)
constexpr U kGlAmbient_0073f2fc = 0x1200;
constexpr U kGlDiffuse_0073f300 = 0x1201;
constexpr U kGlEmission_0073f304 = 0x1600;
constexpr U kGlColorArray_0073f3a0 = 0x8076;
constexpr U kGlColorMaterial = 0x0B57;             // immediate (00455BD3 / 004561F4)

void part_prepare(void* part) { view<PartDrawSlots>(part).prepare_60(part, nullptr); }
void part_draw(void* part, U flag) { view<PartDrawSlots>(part).draw_64(part, nullptr, flag); }
void* light_manager_of(void* scene) { return field<void*>(scene, 0x80); }

// Local CExoArrayList<int> of DoFadeBuckets ([esp+30h] data, [esp+34h] size, [esp+38h] capacity): count loop and the
// inlined Add (capacity stored before operator new, element copy, free of the old buffer when non-null).
struct LocalList { U* data; I size; I capacity; };
void local_add_unique(LocalList& list, U value) {
    if (list.size > 0) {                                       // 00455E8D test ebx,ebx / jle
        I matches = 0;
        for (I n = 0; n < list.size; ++n) if (list.data[n] == value) ++matches;   // 00455EA0..00455EA9
        if (matches != 0) return;                              // 00455EAB / 00455EAD
    }
    if (list.size == list.capacity) {                          // 00455EB3 cmp ebx,[esp+38h]
        const I capacity = list.capacity != 0 ? list.capacity + list.capacity : 8;   // 00455EC1..00455EC9
        list.capacity = capacity;                              // 00455ECE (before new)
        U* fresh = static_cast<U*>(live_operator_new(static_cast<U>(capacity) << 2));   // 00455ED6
        for (I n = 0; n < list.size; ++n) fresh[n] = list.data[n];                   // 00455EE6..00455EF9
        if (list.data != nullptr) live_free(list.data);         // 00455EFB..00455F00
        list.data = fresh;                                     // 00455F08
    }
    list.data[list.size] = value;                              // 00455F10
    list.size = list.size + 1;                                 // 00455F13 / 00455F14
}

// One part of a fade bucket: 00455CE0..00455F13 (first pass) == 00455F60..004561A3 (bucket 0 pass).
// scratch: [esp+48h] fog colour (3 floats) contiguous with the material colour [esp+54h] = {1,1,1,1}; glFogfv reads
// 4 floats, so its 4th float is the statically known 1.0f of [esp+54h] (D54: exact known word, not a D43 zero).
void fade_part(void* scene, void* part, float* scratch, LocalList& seen) {
    void* object = field<void*>(part, 0x80);                   // 00455CE9
    const U saved = field<U>(part, 0x84);                      // 00455CF2 / 00455CFE [esp+18h]
    const double fade = W(field<float>(object, 0x50)) * W(field<float>(part, 0x84));   // 00455CEF / 00455CF8
    field<float>(part, 0x84) = S(fade);                        // 00455D02 fst dword
    if (fade > Wb(kZero_0073d700)) {                           // 00455D08..00455D13 test ah,41h / jne skip
        part_prepare(part);                                    // 00455D1D call [edx+60h]
        void* owner = field<void*>(part, 0x80);                // 00455D20
        if (owner != nullptr && W(field<float>(owner, 0x94)) < Wb(kOne_0073d764)) {   // 00455D26..00455D3F test ah,5 / jp
            kotor_renderer_init::glEnable_0073d390(*cells.gl_fog_0078d450);   // 00455D45..00455D4B
            const double alpha = W(field<float>(owner, 0x94));               // 00455D57
            const float square = S(alpha * alpha);                           // 00455D62..00455D6F fstp [esp+1Ch]
            const float* colour = static_cast<const float*>(at(scene, 0x74)); // 00455D64 lea edi,[ebp+74h]
            scratch[2] = S(W(square) * W(colour[2]));                        // 00455D75..00455D79 -> [esp+50h]
            scratch[1] = S(W(square) * W(colour[1]));                        // 00455D7C..00455D80 -> [esp+4Ch]
            scratch[0] = S(W(square) * W(colour[0]));                        // 00455D83..00455D87 -> [esp+48h]
            glFogfv(*cells.gl_fog_color_0078d490, scratch);                  // 00455D95 glFogfv(78D490, esp+48h)
            glFogf(*cells.gl_fog_density_0078d48c,
                   S(W(field<float>(scene, 0x70)) + W(field<float>(scene, 0x70))));     // 00455D9B..00455DB0
            glFogf(*cells.gl_fog_start_0078d484,
                   S(W(field<float>(owner, 0x94)) * W(field<float>(scene, 0x64))));     // 00455DBC..00455DD5
            glFogf(*cells.gl_fog_end_0078d488,
                   S(W(field<float>(owner, 0x94)) * W(field<float>(scene, 0x68)) + Wb(kFogEndBias_0073e798))); // 00455DE1..00455E00
            part_draw(part, 0);                                // 00455E06..00455E0C call [eax+64h](0)
            if (field<U>(scene, 0x60) == 0)                    // 00455E0F..00455E14
                kotor_renderer_init::glDisable_0073d22c(*cells.gl_fog_0078d450);    // 00455E16..00455E1D
            // Restore the scene fog (raw DWORDs pushed, 00455E23..00455E62); glFogfv reads Scene+74..+80.
            glFogf(*cells.gl_fog_density_0078d48c, field<float>(scene, 0x70));
            glFogf(*cells.gl_fog_start_0078d484, field<float>(scene, 0x64));
            glFogf(*cells.gl_fog_end_0078d488, field<float>(scene, 0x68));
            glFogfv(*cells.gl_fog_color_0078d490, static_cast<const float*>(at(scene, 0x74)));   // 00455E68..00455E70
        } else {
            part_draw(part, 0);                                // 00455E7C..00455E82
        }
    }
    field<U>(part, 0x84) = saved;                              // 00455E85..00455E8F restore raw DWORD
    local_add_unique(seen, field<U>(part, 0x80));              // 00455E97 / 00455EB5 (no null test)
}
} // namespace

extern "C" {

// 00450970 Scene::DoStaticMeshBeams (Scene vtbl+108; `this` unused). EAX: the gate (0) or the last re-read size of
// bucket 007FBF6C (0045097F / 0045099F), returned faithfully.
U __fastcall scene_do_static_mesh_beams_00450970(void*, void*) {
    const U gate = static_mesh_beams_0078e3d0;                 // 00450970 mov eax,[0078E3D0]
    if (gate == 0) return gate;                                // 00450975 / 00450977
    set_beaming_00471450();                                    // 0045097A
    PointerList* beams = bsp_cells.beam_bucket_007fbf6c;
    I size = beams->size;                                      // 0045097F
    for (I index = 0; index < size;) {                         // 00450986..004509A7
        part_draw(elements(beams)[index], 0);                  // 00450990..0045099C [data+i*4]->vtbl+64(0)
        size = beams->size;                                    // 0045099F (re-read)
        ++index;
    }
    return static_cast<U>(size);
}

// 00452CC0 Scene::AddFacesToBins(Part* part, const Vector* origin, CExoArrayList<CExoArrayList<CAurTriangleBin*>>* grid)
// (Scene vtbl+128, RET 0Ch). Stack words are raw U (vtables_gen.h contract). EAX = final face index (0045309F / 00452D4F).
U __fastcall scene_add_faces_to_bins_00452cc0(void* scene, void*, U part_word, U origin_word, U grid_word) {
    void* part = ptr_of(part_word);
    const Vector* origin = static_cast<const Vector*>(ptr_of(origin_word));
    PointerList* grid = static_cast<PointerList*>(ptr_of(grid_word));
    void* node = aurora_node_as_trimesh_0043e400(field<aurora_model::MdlNode*>(part, 4), nullptr);   // 00452CEA..00452CEE
    Vector position{0.0f, 0.0f, 0.0f};                         // 00452D01..00452D11 [esp+40h..48h] = 0
    Quaternion orientation{1.0f, 0.0f, 0.0f, 0.0f};            // 00452D19..00452D3A (w=1.0f at [esp+7Ch])
    view<PartSlots>(part).get_world(part, nullptr, &position, &orientation);   // 00452D42..00452D49 vtbl+0C
    I face_index = 0;                                          // 00452D4F / 00452D53 [esp+20h]
    U face_offset = 0;                                         // 00452D5D [esp+1Ch] (stride 20h)
    for (; face_index < field<I>(node, 0x5C); ++face_index, face_offset += 0x20u) {   // 00452D4C / 00453094..004530AD
        unsigned char* face = field<unsigned char*>(node, 0x58) + face_offset;     // 00452D61..00452D6C
        if (!aur_grass_material_in_use_004a6840(*reinterpret_cast<const U*>(face + 0x10)))   // 00452D68..00452D7D
            continue;
        void* model = aurora_tree_as_model_0043e1c0(field<aurora_model::MaxTree*>(node, 8), nullptr);   // 00452D83..00452D86
        const std::uintptr_t base = reinterpret_cast<std::uintptr_t>(
            aurora_pool_lock_00423f10(field<U>(model, 0xAC), 0, 0, 8));            // 00452D8B..00452D98
        const U stride = field<U>(node, 0x14C);                // 00452D9D
        const U offset194 = field<U>(node, 0x194);             // 00452DA3
        const U offset154 = field<U>(node, 0x154);             // 00452DA9
        auto vertex = [&](U index_offset) -> const float* {    // movsx [face+..]; imul stride; + [154] + [194] + base
            const I index = *reinterpret_cast<const std::int16_t*>(face + index_offset);
            const U offset = static_cast<U>(index) * stride + offset154 + offset194;
            return reinterpret_cast<const float*>(base + offset);
        };
        const float* v0 = vertex(0x1A);                        // 00452DC7..00452DED raw DWORD copies
        const float* v1 = vertex(0x1C);                        // 00452DF1..00452DFC
        const float* v2 = vertex(0x1E);                        // 00452E1A..00452E29
        Vector sum;
        std::memcpy(&sum, v0, sizeof sum);                     // [esp+40h..48h] = vertex 0
        sum.x = S(W(sum.x) + W(v1[0]));                        // 00452DFE / 00452E00
        sum.y = S(W(sum.y) + W(v1[1]));                        // 00452E04..00452E0B
        sum.z = S(W(sum.z) + W(v1[2]));                        // 00452E0F..00452E16
        sum.x = S(W(sum.x) + W(v2[0]));                        // 00452E1E..00452E2D
        sum.y = S(W(sum.y) + W(v2[1]));                        // 00452E31..00452E38
        const double z_sum = W(sum.z) + W(v2[2]);              // 00452E3C..00452E40 (kept on the x87 stack)
        Vector centre;
        centre.z = S(z_sum * Wb(kThird_007415f4));             // 00452E43 -> 00452E75 [esp+6Ch]
        centre.y = S(W(sum.y) * Wb(kThird_007415f4));          // 00452E49..00452E4D -> 00452E69 [esp+68h]
        centre.x = S(W(sum.x) * Wb(kThird_007415f4));          // 00452E53..00452E5D [esp+64h]
        Vector rotated_storage;
        const Vector* rotated = quaternion_rotate_004a9bb0(&rotated_storage, &orientation, &centre);   // 00452E81..00452E96
        const double world_z = W(position.z) + W(rotated->z);  // 00452E9B..00452EA2 (fst keeps ST0)
        const double world_y = W(position.y) + W(rotated->y);  // 00452EA6..00452EAA
        const double world_x = W(position.x) + W(rotated->x);  // 00452EAD..00452EB1
        const float wx = S(world_x);                           // 00452EB3 fstp [esp+7Ch]
        const float wy = S(world_y);                           // 00452EC3 fstp [esp+80h]
        const float wz = S(world_z);                           // 00452EA2 fst / 00452ED1 fstp
        const float rz = S(round_004a96e0(wz, bits_to_float(kMilli)));   // 00452EDF..00452EF2
        const float ry = S(round_004a96e0(wy, bits_to_float(kMilli)));   // 00452EFA..00452F05
        const float rx = S(round_004a96e0(wx, bits_to_float(kMilli)));   // 00452F0D..00452F18
        (void)rz;   // stored to [esp+6Ch] / [esp+58h] (00452F37 / 00452F4A) and never read again by the original
        const double spacing = W(*bsp_cells.tile_step_0078e428);         // 00452F44 / 00452F7E fdiv [0078E428]
        // CRT floor 006FC750 -> UCRT floor (exact IEEE operation, same result for every input; precedent lane-3d-bsp).
        const float column_floor = S(std::floor((W(rx) - W(origin->x)) / spacing));   // 00452F20..00452F56
        I column = fistp_nearest(W(column_floor));             // 00452F65..00452F6F frndint / fistp
        const float row_floor = S(std::floor((W(ry) - W(origin->y)) / spacing));      // 00452F71..00452F8C
        I row = fistp_nearest(W(row_floor));                   // 00452F9B..00452FA5
        const I column_max = grid->size - 1;                   // 00452FAA..00452FB1
        const I column_low = column < column_max ? column : column_max;   // 00452FB2..00452FB8
        if (column_low < 0) column = 0;                        // 00452FBA..00452FBE
        else if (column >= column_max) column = column_max;    // 00452FC2..00452FC6
        PointerList* rows = reinterpret_cast<PointerList*>(static_cast<std::uintptr_t>(grid->data));
        const I row_max = rows[column].size - 1;               // 00452FCC..00452FD8
        const I row_low = row < row_max ? row : row_max;       // 00452FDF..00452FE5
        if (row_low < 0) row = 0;                              // 00452FE7..00452FEB
        else if (row >= row_max) row = row_max;                // 00452FEF..00452FF3
        if (words(&rows[column])[row] == 0) {                  // 00452FF9..00453002
            void* storage = live_operator_new(0x70);           // 00453004..00453006
            void* bin = storage != nullptr ? triangle_bin_construct_004a82b0(storage, nullptr) : nullptr;   // 00453012..0045302A
            rows = reinterpret_cast<PointerList*>(static_cast<std::uintptr_t>(grid->data));               // 00453030..00453033
            words(&rows[column])[row] = word_of(bin);          // 0045302C..0045304A
        }
        void* model_again = aurora_tree_as_model_0043e1c0(field<aurora_model::MaxTree*>(node, 8), nullptr);   // 0045304D..00453054
        aurora_pool_unlock_00423f30(field<U>(model_again, 0xAC));    // 00453059..00453060
        rows = reinterpret_cast<PointerList*>(static_cast<std::uintptr_t>(grid->data));                     // 00453077
        triangle_bin_add_triangle_004a7860(ptr_of(words(&rows[column])[row]), nullptr, face, part, node);   // 0045306E..00453084
        field<U8>(scene, 0xDB) = 1;                            // 00453089..0045308D
    }
    return static_cast<U>(face_index);
}

// 004531A0 Scene::ListSpecialPartsR(Part* part), RET 4. The original body is this recursion with the first five levels
// inlined by the compiler (00453212 / 00453280 / 004532F0 / 00453360 / 004533D0 / 00453440 levels, then the real
// recursive call 004534A9); the per-level operations, null-child skips and child-count re-reads are identical.
void __fastcall scene_list_special_parts_r_004531a0(void* scene, void*, void* part) {
    if (part == nullptr) return;                               // 004531A9 / 004531AD
    void* light = view<PartSlots>(part).as_light(part, nullptr);   // 004531B3..004531B9 vtbl+24
    if (light != nullptr) {                                    // 004531BE
        void* lights = light_manager_of(scene);                // 004531C2
        view<LightManagerC2Slots>(lights).add_light(lights, nullptr, light);   // 004531C8..004531CB vtbl+0C
        scene_bsp_add_part_0046e290(scene, light, nullptr);    // 004531CE..004531D2 (push 0, light, scene)
    }
    void* emitter = view<PartSlots>(part).as_emitter(part, nullptr);   // 004531DA..004531DF vtbl+34
    if (emitter != nullptr) {                                  // 004531E4
        scene_bsp_add_part_0046e290(scene, emitter, nullptr);  // 004531E8..004531EC
        exo_array_list_add_unique_0046bd10(list_at(scene, 0x94), nullptr, word_of(emitter));   // 004531F4..004531FB
    }
    for (I index = 0; index < field<I>(part, 0x34); ++index)   // 00453200..0045320B / 0045352E..0045353C (re-read)
        scene_list_special_parts_r_004531a0(scene, nullptr, elements(list_at(part, 0x30))[index]);   // 00453212..00453215
}

// 00453950 Scene::DoGobShadows (Scene vtbl+F8). D2:EAX (original: free() result or 0).
U __fastcall scene_do_gob_shadows_00453950(void* scene, void*) {
    PointerList lights;                                        // [esp+8h] data / [esp+0Ch] size / [esp+10h] capacity
    lights.data = 0; lights.size = 0; lights.capacity = 0;     // 0045396B..00453973
    exo_array_list_assign_0046bc80(&lights, nullptr,
        light_manager_get_shadow_lights_004a4b10(light_manager_of(scene), nullptr, 0));   // 00453977..0045398C
    if (lights.size == 0) {                                    // 00453991 / 00453995
        if (lights.data != 0) live_free(ptr_of(lights.data));  // 00453997..004539AC
        return 0;
    }
    void** light_data = elements(&lights);                     // 004539C5 ebp (loaded once)
    I gob_index = *cells.gob_bucket_count_007fbfd0;            // 004539CA
    if (gob_index != 0) {                                      // 004539D0 / 004539D2
        do {
            --gob_index;                                       // 004539E5
            if (*cells.animations_0078e3b8 != 0 && *cells.interleaved_007b92f4 != 0) {   // 004539E0..004539F0
                if (gob_index >= *cells.gob_bucket_count_007fbfd0) continue;          // 004539F2..004539F8
                void* gob = (*cells.gob_bucket_007fbfcc)[gob_index];                   // 004539FE..00453A03
                if (field<U>(gob, 0x188) != 0)                                         // 00453A06..00453A0C
                    view<aurora_model::GobVTable>(gob).animate(static_cast<aurora_model::Gob*>(gob), nullptr,
                        *cells.frame_time_0078e574);           // 00453A0E..00453A16 vtbl+14(frame time)
                if (gob_index >= *cells.gob_bucket_count_007fbfd0) continue;          // 00453A19..00453A1F
            }
            void* gob = (*cells.gob_bucket_007fbfcc)[gob_index];                       // 00453A25..00453A2B
            if (field<U16>(gob, 0xC2) == 0) continue;          // 00453A2E..00453A35
            if (field<U8>(gob, 0x166) == 0) continue;          // 00453A3B..00453A41
            if (*cells.shadow_distance_check_0078e3cc != 0) {  // 00453A47..00453A4D
                void* active = *cells.active_scene_007b92a0;   // 00453A52
                Vector delta;
                delta.z = S(W(field<float>(active, 0xB0)) - W(field<float>(gob, 0x80)));   // 00453A58..00453A61
                delta.y = S(W(field<float>(active, 0xAC)) - W(field<float>(gob, 0x7C)));   // 00453A64..00453A6A
                delta.x = S(W(field<float>(active, 0xA8)) - W(field<float>(gob, 0x78)));   // 00453A6D..00453A77
                if (vector_magnitude_004ab100(&delta) > W(*cells.shadow_distance_0078e3c8)) continue;   // 00453A86..00453A99 test ah,41h / je
            }
            gob = (*cells.gob_bucket_007fbfcc)[gob_index];     // 00453A9F..00453AA5
            void* render = field<void*>(gob, 0x58);            // 00453AA8
            if (render != nullptr && ((field<U8>(render, 0x50) >> 2) & 1u) == 0) {   // 00453AAB..00453AB8
                void* object = field<void*>(gob, 0x188);       // 00453ABA
                if (object == nullptr) continue;               // 00453AC0..00453AC2
                if (view<Gob188Slots>(object).query_14(object, nullptr) == 0) continue;   // 00453AC8..00453AD1
            }
            for (I index = 0; index < lights.size; ++index) {  // 00453AD7..00453B75 (size re-read)
                void* light = light_data[index];               // 00453AE5
                U radius = field<U>(light, 0x58);              // 00453AEC / 00453AF5
                if (W(field<float>(light, 0x60)) > Wb(kZero_0073d700)) radius = field<U>(light, 0x60);   // 00453AE9..00453B03
                void* current = (*cells.gob_bucket_007fbfcc)[gob_index];               // 00453B0B..00453B17
                Vector delta;
                delta.z = S(W(field<float>(light, 0x9C)) - W(field<float>(current, 0x80)));   // 00453B11..00453B1A
                delta.y = S(W(field<float>(light, 0x98)) - W(field<float>(current, 0x7C)));   // 00453B26..00453B2C
                delta.x = S(W(field<float>(light, 0x94)) - W(field<float>(current, 0x78)));   // 00453B2F..00453B31
                if (vector_magnitude_004ab100(&delta) < Wb(radius))                      // 00453B44..00453B55 test ah,5 / jp
                    project_dynamic_shadow_0044ff50((*cells.gob_bucket_007fbfcc)[gob_index], light);   // 00453B57..00453B66
            }
        } while (gob_index != 0);                              // 00453B7B / 00453B7D
    }
    if (light_data != nullptr) live_free(light_data);          // 00453B84..00453B91
    return 0;
}

// 00453BB0 Scene::DoDynamicMeshProjections (Scene vtbl+10C). D2:EAX (original: free() result or list data 0).
U __fastcall scene_do_dynamic_mesh_projections_00453bb0(void* scene, void*) {
    PointerList lights;                                        // [esp+18h] data / [esp+1Ch] size / [esp+20h] capacity
    lights.data = 0; lights.size = 0; lights.capacity = 0;     // 00453BD1..00453BD9
    void* camera = *cells.current_camera_007bb4e8;             // 00453BDD
    const Plane* planes = field<PlaneList>(camera, 0x1F8).data;   // 00453BE2 [camera+1F8] (plane 1 at +10h)
    U plane_words[3];
    std::memcpy(plane_words, &planes[1].a, sizeof plane_words);    // 00453BE8 / 00453C01 / 00453C08 raw copies
    const float plane_distance = S(W(planes[1].d) + Wb(kPlaneLift_007415bc));   // 00453BEB..00453C04 [esp+24h]
    exo_array_list_assign_0046bc80(&lights, nullptr,
        light_manager_get_shadow_lights_004a4b10(light_manager_of(scene), nullptr, 0));   // 00453BF4..00453C25
    void** light_data = elements(&lights);                     // 00453C2E edi
    if (lights.size > 0) {                                     // 00453C34..00453C3A
        PointerList* projectors = bsp_cells.mesh_shadow_bucket_007fbfb4;
        I projector_count = projectors->size;                  // 00453C40 ebp (read once; refreshed at 00453DA3)
        for (I light_index = 0; light_index < lights.size; ++light_index) {   // 00453C46 / 00453DBC..00453DC7
            void* light = light_data[light_index];             // 00453C46
            U radius = field<U>(light, 0x58);                  // 00453C4C / 00453C55
            if (W(field<float>(light, 0x60)) > Wb(kZero_0073d700)) radius = field<U>(light, 0x60);   // 00453C49..00453C63
            if (!(Wb(radius) > W(projection_min_radius_0078e3d4)))           // 00453C67..00453C76 test ah,41h / je
                radius = float_to_bits(projection_min_radius_0078e3d4);      // 00453C78..00453C7D raw copy
            if (projector_count <= 0) continue;                // 00453C81..00453C83
            const Vector* light_position = static_cast<const Vector*>(at(light, 0x94));   // 00453C8D edi
            for (I index = 0; index < projector_count;) {      // 00453C93..00453DAC
                void* projector = elements(projectors)[index]; // 00453C93..00453C9E [esp+28h]
                if (view<ProjectorSlots>(projector).in_range_80(projector, nullptr, *light_position, radius) != 0) {   // 00453CA2..00453CC6
                    glPushMatrix();                            // 00453CCC call [0073D300]
                    void* current = elements(projectors)[index];                   // 00453CD2..00453CD8
                    float matrix[16];                          // [esp+74h]
                    make_matrix4x4_004aa9c0(matrix, static_cast<const Vector*>(at(current, 0x08)),
                        static_cast<const Quaternion*>(at(current, 0x14)));        // 00453CDB..00453CE8
                    glMultMatrixf(matrix);                     // 00453CF0..00453CF5 call [0073D25C]
                    Plane plane;                               // [esp+2Ch]: normal raw copy, distance [esp+24h]
                    std::memcpy(&plane.a, plane_words, sizeof plane_words);        // 00453CFB..00453D18
                    plane.d = plane_distance;                  // 00453D0B / 00453D1C
                    current = elements(projectors)[index];     // 00453D13..00453D20
                    plane_transform_004ab4c0(&plane, nullptr, static_cast<const Vector*>(at(current, 0x08)),
                        static_cast<const Quaternion*>(at(current, 0x14)));        // 00453D23..00453D2F
                    current = elements(projectors)[index];     // 00453D37..00453D3D
                    Vector local;                              // [esp+4Ch]
                    local.z = S(W(light_position->z) - W(field<float>(current, 0x10)));   // 00453D34..00453D40
                    local.y = S(W(light_position->y) - W(field<float>(current, 0x0C)));   // 00453D46..00453D4D
                    local.x = S(W(light_position->x) - W(field<float>(current, 0x08)));   // 00453D51..00453D53
                    Quaternion inverse;                        // [esp+64h]
                    const Quaternion* conjugate = quaternion_negate_w_004a9b50(&inverse,
                        static_cast<const Quaternion*>(at(current, 0x14)));        // 00453D61..00453D6D
                    Vector rotated;                            // [esp+58h]
                    quaternion_rotate_004a9bb0(&rotated, conjugate, &local);       // 00453D75..00453D7B
                    void* target = elements(projectors)[index];                    // 00453D80..00453D85
                    view<ProjectorSlots>(target).project_5c(target, nullptr, &rotated, radius, &plane, 0);   // 00453D88..00453D9A
                    glPopMatrix();                             // 00453D9D call [0073D2FC]
                }
                projector_count = projectors->size;            // 00453DA3 (re-read)
                ++index;                                       // 00453DA9
            }
        }
    }
    if (light_data != nullptr) live_free(light_data);          // 00453DCD..00453DDD
    return 0;
}

// 00455A40 Scene::DoCapBucket (Scene vtbl+EC; `this` unused). D2:EAX (original: free() result or 0).
U __fastcall scene_do_cap_bucket_00455a40(void*, void*) {
    PointerList ordering;                                      // [esp+0Ch]
    int_list_copy_construct_0046d250(&ordering, nullptr, aur_texture_get_ordering_00421970());   // 00455A5B..00455A65
    PointerList* buckets = bsp_cells.cap_buckets_007fc008;     // 12-byte {data,size,capacity} entries (007FC008/0C)
    for (I k = 0; k < ordering.size; ++k) {                    // 00455A6A..00455AEB
        PointerList* bucket = &buckets[words(&ordering)[k]];   // 00455A80..00455A8A lea esi,[eax+eax*2]; shl 2
        if (bucket->size == 0) continue;                       // 00455A8D..00455A95
        void* material = field<void*>(elements(bucket)[0], 0x44);   // 00455A97..00455A9F
        material_bind_texture0_0047adf0(material, nullptr);   // 00455AA4
        material_set_blending_mode_0047af70(material, nullptr, 0);   // 00455AA9..00455AAD
        for (I j = 0; j < bucket->size; ++j) {                 // 00455AB2..00455AE2 (size re-read)
            void* part = elements(bucket)[j];                  // 00455AC0..00455AC6
            part_prepare(part);                                // 00455ACD vtbl+60
            part_draw(part, 0);                                // 00455AD0..00455AD6 vtbl+64(0)
        }
    }
    PointerList* bucket0 = &buckets[0];                        // 007FC008 / 007FC00C
    for (I j = 0; j < bucket0->size; ++j) {                    // 00455AEE..00455B2D
        void* part = elements(bucket0)[j];                     // 00455B02..00455B08
        if (j == 0) material_bind_texture0_0047adf0(field<void*>(part, 0x44), nullptr);   // 00455B00 / 00455B0D..00455B10
        part_prepare(part);                                    // 00455B15..00455B19
        part_draw(part, 0);                                    // 00455B1C..00455B22
    }
    if (ordering.data != 0) live_free(ptr_of(ordering.data));  // 00455B2F..00455B43
    return 0;
}

// 00455B60 Scene::DoFadeBuckets (Scene vtbl+F4). D2:EAX (original: free() result or 0).
U __fastcall scene_do_fade_buckets_00455b60(void* scene, void*) {
    LocalList seen{nullptr, 0, 0};                             // 00455B7C..00455B8A [esp+30h..38h]
    PointerList ordering;                                      // [esp+3Ch]
    int_list_copy_construct_0046d250(&ordering, nullptr, aur_texture_get_ordering_00421970());   // 00455B92..00455B9C
    void* lights = light_manager_of(scene);                    // 00455BA1
    view<LightManagerC2Slots>(lights).enable_lights_dynamic_only(lights, nullptr);   // 00455BA7..00455BAE vtbl+08
    kotor_renderer_init::glEnable_0073d390(*cells.gl_blend_0078d448);               // 00455BB1..00455BBE
    glColorMaterial(kGlFront_0073f2ec, kGlEmission_0073f30c);  // 00455BC0..00455BCD
    kotor_renderer_init::glEnable_0073d390(kGlColorMaterial);  // 00455BD3..00455BD8
    if (*cells.black_fog_008305e0 != 0)                        // 00455BDA..00455BE1
        kotor_renderer_init::glEnableClientState_0073d260(kGlColorArray_0073f3a0);   // 00455BE3..00455BEA
    // [esp+48h..53h] fog colour scratch, [esp+54h] material colour {1,1,1,1}, [esp+64h] emission {0,0,0,1}.
    float scratch[11] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};   // 00455C08..00455C40
    glMaterialfv(kGlFront_0073f2ec, kGlAmbient_0073f2fc, scratch + 3);      // 00455BF0..00455C48
    glMaterialfv(kGlFront_0073f2ec, kGlDiffuse_0073f300, scratch + 3);      // 00455C4A..00455C5C
    glMaterialfv(kGlFront_0073f2ec, kGlEmission_0073f304, scratch + 7);     // 00455C5E..00455C70
    PointerList* buckets = bsp_cells.fade_buckets_0080aa80;    // {data 0080AA80, size 0080AA84} x 5000
    for (I k = 0; k < ordering.size; ++k) {                    // 00455C72..00455F42
        PointerList* bucket = &buckets[words(&ordering)[k]];   // 00455C84..00455C99 [esp+1Ch] offset
        if (bucket->size == 0) continue;                       // 00455C91..00455C9D
        void* material = field<void*>(elements(bucket)[0], 0x44);   // 00455CA3..00455CAB
        material_bind_texture0_0047adf0(material, nullptr);   // 00455CB0
        material_set_blending_mode_0047af70(material, nullptr, 0);   // 00455CB5..00455CB9
        for (I j = 0; j < bucket->size; ++j)                   // 00455CBE..00455CD0 / 00455F18..00455F2D (size re-read)
            fade_part(scene, elements(bucket)[j], scratch, seen);   // 00455CE0..00455CE6
    }
    PointerList* bucket0 = &buckets[0];                        // 0080AA80 / 0080AA84
    for (I j = 0; j < bucket0->size; ++j) {                    // 00455F48..004561B9
        void* part = elements(bucket0)[j];                     // 00455F62..00455F68
        if (j == 0) material_bind_texture0_0047adf0(field<void*>(part, 0x44), nullptr);   // 00455F60 / 00455F6D..00455F70
        fade_part(scene, part, scratch, seen);                 // 00455F75..004561A3
    }
    for (I n = 0; n < seen.size; ++n) {                        // 004561BF..004561E0 (no null test in the original)
        void* object = ptr_of(seen.data[n]);
        field<U>(object, 0x50) = field<U>(object, 0x54);       // 004561C5..004561CB
        field<U>(object, 0x8C) = kMeshResetValue_00740a70;     // 004561CE..004561DA
    }
    kotor_renderer_init::glDisableClientState_0073d264(kGlColorArray_0073f3a0);   // 004561E2..004561E8
    kotor_renderer_init::glDisable_0073d22c(kGlColorMaterial); // 004561EE..004561F9
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_blend_0078d448);           // 004561FB..00456202
    if (ordering.data != 0) live_free(ptr_of(ordering.data));  // 00456204..00456212
    if (seen.data != nullptr) live_free(seen.data);            // 0045621A..00456227
    return 0;
}

// 00456250 Scene::DoStaticMeshShadows (Scene vtbl+104). D2:EAX (original: free() result or bucket size).
U __fastcall scene_do_static_mesh_shadows_00456250(void* scene, void*) {
    void* lights = light_manager_of(scene);                    // 00456268
    const U first_light = field<U>(lights, 0xA4);              // 0045626F esi
    PointerList shadow_lights;                                 // [esp+8h]
    int_list_copy_construct_0046d250(&shadow_lights, nullptr,
        light_manager_get_shadow_lights_004a4b10(lights, nullptr, 1));   // 00456276..00456282
    U* data = words(&shadow_lights);                           // 0045628C edi
    if (shadow_lights.size == 1 && first_light != data[0]) {   // 00456287..0045629C
        live_free(data);                                       // 0045629E / 004562D8..004562D9 (no null test)
        return 0;
    }
    PointerList* bucket = bsp_cells.static_shadow_bucket_007fbf84;
    for (I index = 0; index < bucket->size; ++index)           // 004562A8..004562CA (size re-read)
        part_draw(elements(bucket)[index], 0);                 // 004562B3..004562BF vtbl+64(0)
    if (data != nullptr) live_free(data);                      // 004562CC..004562D9
    return 0;
}

} // extern "C"
