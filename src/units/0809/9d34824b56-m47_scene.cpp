// lane-m47-3d fork "scene": faithful bodies for the scene/wind/light/triangle-bin names left unresolved by M46
// (m46-compose/SYMBOL-PREDICTION.json) and referenced by core46-scene-c1 / core46-scene-c2 / core46-math.
//   00454940 Scene::SetVisibility                 004A6050 WindManager::GetGlobalWind
//   00458BE0 Scene::~Scene (body)                 004A5C80 WindManager::pointSourceWindVector
//   004237E0 AurTextureReplaceTileReferences      004A4B10 LightManager::GetShadowLights
//   004AB4C0 Plane::Transform                     004A7860 CAurTriangleBin::AddTriangle
//   004A82B0 CAurTriangleBin::CAurTriangleBin     00471450 SetBeaming
//   004A77C0 CExoArrayList<CAurTriangleListElem>::Add
// plus their unowned callees (authored here, see parts/scene.json "authored_callees"):
//   004A73F0 CExoArrayList<CAurTriangleListElem>::allocate, 004A7220 / 004A7240 CAurTriangleListElem ctors,
//   0046C380 SceneBSPGetTileTextures, 0046C060 SceneBSPGetObjectTextures, 0046C850 DeleteNonGobParts,
//   004ACED0 BSPNode::~BSPNode, 00452A60 CAurRoom::~CAurRoom, 004A6710 WindManager::~WindManager,
//   004A6570 SafePointerList<WindManager>::~SafePointerList, 00457820 CAurScene::~CAurScene,
//   004A2BC0 LightManager::~LightManager, 004A8350 CAurTriangleBin::~CAurTriangleBin, 0046B1E0 nodedata::~nodedata.
// Evidence: dumpbin /disasm of the pinned GOG swkotor.exe (local lane-m47-3d/d/ADDR.txt); every behaviour cites the
// instruction addresses. Conventions: D1 (__thiscall -> __fastcall(self, unused EDX, stack...), EDX never read),
// symbols/ABIs exactly as the consumer declarations (m46_scene_externs_c1.h / _c2.h, scene3d.h, m46_math.cpp,
// m46_camera_externs.h), typed receiver tables reloaded per call (D21/D41; published aurora_vt / SceneVTable /
// LightManagerVTable, local single-purpose views where no table is published - precedent m46_gob_externs.h),
// D30-FINAL (x87 wide values are double, float only where the original stores `fstp dword`; frndint+fistp =
// round-to-nearest cvtsd2si; _ftol2 through the genuine port 006FAE8C), allocator family 006FA7E6 / 006FA390 ->
// live_operator_new / live_free (M41, stage precedent), D2 labels where the original leaves EAX undefined.
// EH frames (C++ unwind states) of the original are cited; no callee on these paths throws (D30-FINAL unwind rule).
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <emmintrin.h>
#include "11b317539a-bsp.h"
#include "74db077be7-m46_scene_externs_c1.h"
#include "8079180b4a-m46_scene_externs_c2.h"
#include "7ee14b5ec3-model_vtables.hpp"
#include "9219c31d0e-m47_aur_scene_vtable.h"   // [rev2] typed CAurScene table 00741610

// ---------------------------------------------------------------- existing M46 providers (exact spellings)
namespace kotor_renderer_init {
extern void __stdcall glBlendFunc_0073d38c(U32, U32);                    // aurora-windows-gl-imports
extern void __stdcall glColor4fv_0073d35c(const float*);                 // aurora-windows-gl-imports
}
namespace kotor_renderer_gl {   // renderer-bodies render_pool.h:106 (identical declaration; in the M46 unresolved set)
void __fastcall allocate_00437d60(kotor_reconstruction::ArrayMutations* self, void* unused_edx, std::uint32_t capacity);
}
namespace kotor_vp_cells { extern volatile std::uint32_t gate_0078e608; }   // model46-m46-orphan-cells (.data 1)
namespace font { struct Texture; }
namespace texture_join {   // texture-epoch-cleanup-slots CExoArrayList::AddUnique 0046BD10 (RET 4)
void __fastcall add_unique_0046bd10(kotor_reconstruction::ArrayMutations*, void*, font::Texture*);
}
namespace kotor_scene3d_construct { struct LightManagerVTable; extern const LightManagerVTable light_manager_vtable_00743958; }
namespace menu_tables_m41 {   // menu-tables-d1-bodies: `eh vector constructor iterator` 006FB55F port (stdcall)
void __stdcall VectorConstruct_006fb55f(void*, unsigned, unsigned, void* (__fastcall*)(void*, void*), void (__fastcall*)(void*, void*));
}
namespace gui_viewport_list {   // viewport-add: CExoArrayList<int>::Add 00444FE0 (D58 single provider, RET 4)
void __fastcall add_00444fe0(kotor_reconstruction::ArrayMutations*, void*, int);
}
extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::Vector; using kotor_scene3d::Quaternion;
using kotor_scene3d::Plane; using kotor_scene3d::PointerList;
void __cdecl genuine_x87_ftol_006fae8c();                                 // renderer-x87-ftol-alias (ST0 -> EDX:EAX)
float __cdecl cosine_lut_004aadf0(float);                                 // texture-epoch-controller_base (ST0)
float __cdecl sine_lut_004aae30(float);                                   // texture-epoch-controller_base (ST0)
Vector* __fastcall vector_add_assign_004a9740(Vector* self, void*, const Vector* other);   // 3d-model-model-skin, RET 4
void __cdecl aur_texture_set_mode_static_004204b0();                     // core46-gl-fb
void __cdecl aur_texture_set_mode_dynamic_004204c0();                    // core46-gl-fb
extern volatile std::uint8_t aur_texture_mode_dynamic_0078d400;           // core46-gl-fb (.data byte 1)
void* __fastcall assign_00447fa0(void* safe_pointer, void* unused_edx, void* object);   // 3d-model-model-material, RET 4
Vector* __cdecl vector_min_004aae70(Vector* out, Vector a, Vector b);     // model46-f1-part-core
Vector* __cdecl vector_max_004aaee0(Vector* out, Vector a, Vector b);     // model46-f1-part-core
void* __fastcall triangle_list_elem_copy_construct_004a7340(void* self, void*, const void* source);   // core46-math, RET 4
kotor_reconstruction::ArrayMutations* __fastcall float_list_assign_00438a50(kotor_reconstruction::ArrayMutations* self, void*,
    const kotor_reconstruction::ArrayMutations* source);                  // core46-math, RET 4
void __fastcall triangle_list_elem_destruct_004a72d0(void* self, void*);   // core46-math, RET 0
void* __cdecl find_00420ac0(const char*, const char*, const std::uint32_t*, std::int32_t);   // texture-epoch-texture
void* __cdecl get_00423490(const char*, void*, const char*, void*, U);   // 3d-construct-texture-get (AurTextureGet)
void __cdecl texture_release_00420d70(void* texture);                     // AurTextureRelease (lane-window spelling; M46 unresolved set)
PointerList* __fastcall texture_list_assign_0046bc80(PointerList* self, void*, const PointerList* source);   // startup-misc-texture, RET 4
void __fastcall scene_void_return_004015a0(void*, void*);                 // 3d-render-vtables (004015A0 void_return)
void __fastcall exo_array_list_free_0047abc0(void* list, void*);          // scene3d.h (also here for clarity)
void __stdcall c2da_destroy_array_006fa852(void* first, std::uint32_t size, std::int32_t count,
                                           void (__fastcall* destroy)(void*, void*));   // task10-c2da-array-normal (triangle_bin.h spelling)
void __fastcall aurora_texture_safe_pointer_destroy_00422380(void* self, void* unused_edx);   // 3d-model-model-texture-lists
void __cdecl aur_clear_active_bins_0046a610();                           // 3d-bsp-bsp-callbacks (bsp_callbacks.h)
}

// ---------------------------------------------------------------- definitions authored here (declarations)
extern "C" {
// 004A5C80 WindManager::pointSourceWindVector(Vector* out, Vector point): thiscall RET 10h, EAX = out
// (consumer declaration m46_camera_externs.h; the Vector is passed as three float stack words).
void* __fastcall wind_manager_point_source_wind_vector_004a5c80(void* wind, void* unused_edx, void* out,
                                                                float x, float y, float z);
// 004A77C0 Add(CAurTriangleListElem by value) RET 18h (consumer declaration m46_math.cpp: 18h-byte POD).
struct M47TriangleListElem { kotor_reconstruction::ArrayMutations corners; U face_0c; U part_10; U count_14; };
void __fastcall triangle_list_add_004a77c0(PointerList* list, void* unused_edx, M47TriangleListElem elem);
void __fastcall triangle_list_allocate_004a73f0(PointerList* list, void* unused_edx, I count);
void* __fastcall triangle_list_elem_default_construct_004a7220(void* self, void* unused_edx);
void* __fastcall triangle_list_elem_construct_004a7240(void* self, void* unused_edx, kotor_reconstruction::ArrayMutations list,
                                                       void* face, void* part, I count);
void __cdecl scene_bsp_get_tile_textures_0046c380(void* scene, const Vector* position, PointerList* out);
void __cdecl scene_bsp_get_object_textures_0046c060(void* part, PointerList* out);
void __cdecl delete_non_gob_parts_0046c850(void* node, void* context);
void __fastcall bsp_node_destruct_004aced0(void* node, void*);
void __fastcall aur_room_destruct_00452a60(void* room, void*);
void __fastcall wind_manager_destruct_004a6710(void* wind, void*);
void __fastcall wind_safe_pointer_list_destruct_004a6570(void* list, void*);
void __fastcall aur_scene_destruct_00457820(void* list, void*);
void __fastcall light_manager_destruct_004a2bc0(void* lights, void*);
void __fastcall triangle_bin_destruct_004a8350(void* bin, void*);
void __fastcall node_data_destruct_0046b1e0(void* data, void*);
// CAurScene class table 00741610 (written by Scene::~Scene before the base destructor, 00458E52): [rev2] typed
// table owned by lane-m47-3d rev2 m47_aur_scene_vtable.cpp (declared in m47_aur_scene_vtable.h, included above).

// ---- original cells / function statics referenced only by the bodies of this unit (image-wide absolute-address
//      byte search, tools/xref.py; .data cold values from the pinned image). Storage below.
extern volatile I shadow_light_max_007a1938;          // .data 1 (004A4BDF, 004A4C54, 004A4CE6, 004A4E34, 004A4E4A)
extern volatile float shadow_light_fade_time_007a1948;   // .data 0.3f (004A4D3A, 004A4E7C)
extern volatile U beam_stencil_mask_0078e69c;          // .data 1 (00471497 only)
extern float beam_alpha_0078e6a0;                      // .data 0.2f (0047147E; also written by CSWCArea day/night code
                                                       // 006058E3/006059E3/00606564/00608F3B - non-3D, binds here)
extern Vector beam_color_0078e8a0;                     // .data (1,1,1) (00471483 only)
extern Vector add_triangle_v0_00831eb0;               // AddTriangle function statics (BSS) + guard bits 1/2/4
extern Vector add_triangle_v1_00831ea4;
extern Vector add_triangle_v2_00831e98;
extern volatile U add_triangle_guard_00831ebc;
extern Vector plane_transform_normal_00831f54;        // Plane::Transform function statics (BSS) + guard bits 1/2
extern Vector plane_transform_point_00831f48;
extern volatile U plane_transform_guard_00831f60;
extern Quaternion global_wind_rotation_00831e2c;      // GetGlobalWind function static (BSS) + guard bit 1
extern volatile U global_wind_rotation_guard_00831e3c;
}

namespace {
using namespace kotor_scene3d;
using kotor_reconstruction::ArrayMutations;
using PartTable = aurora_vt::VTable_00740ae8;   // Part base table; slots +00 / +0C / +2C have one type in every
                                                // Part-family table (model_vtables.hpp, generated from the image)

constexpr float kZero = 0.0f;       // [0073D700]
constexpr float kOne = 1.0f;        // [0073D764]
constexpr float kHalf = 0.5f;       // [0073E9AC]
constexpr float kThousand = 1000.0f;   // [0073D6FC] 0x447A0000
constexpr U kGlSrcAlpha_0073f270 = 0x302u;   // .rdata blend-factor table entries
constexpr U kGlOne_0073f278 = 0x1u;

inline double W(float v) { return static_cast<double>(v); }
inline float S(double v) { return static_cast<float>(v); }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
template<class T> inline T* ptr(U w) { return reinterpret_cast<T*>(static_cast<std::uintptr_t>(w)); }
inline U& word_at(U address) { return *reinterpret_cast<U*>(static_cast<std::uintptr_t>(address)); }
inline float x87_float_copy(float v) { return static_cast<float>(static_cast<double>(v)); }   // fld dword / fstp dword
// frndint (RC nearest) + fistp dword: cvtsd2si under the default MXCSR nearest mode; out of range -> 80000000h.
inline I x87_round_store(double v) { return _mm_cvtsd_si32(_mm_set_sd(v)); }
// `fld dword ptr [p]; call 006FAE8C` (genuine __ftol2 port) -> low DWORD.
inline I x87_ftol_float(const float* value) {
    I r;
    __asm {
        mov ecx, value
        fld dword ptr [ecx]
        call genuine_x87_ftol_006fae8c
        mov r, eax
    }
    return r;
}
// `shl eax,2; cdq; xor eax,edx; sub eax,edx; and 8000000Fh; jns; dec; or 0FFFFFFF0h; inc` (004A6113..004A6129).
inline I wind_cell(I v) {
    const U shifted = static_cast<U>(v) << 2;
    const U sign = (shifted & 0x80000000u) ? 0xFFFFFFFFu : 0u;
    const I magnitude = static_cast<I>((shifted ^ sign) - sign);
    return magnitude % 16;
}
// Inline CExoArrayList<T*>::Add with the 005E03C0 growth (cap ? cap*2 : 8): count re-read after the growth call.
inline void append_word(PointerList* list, U value) {
    if (list->size == list->capacity) {
        const U capacity = (list->capacity != 0) ? static_cast<U>(list->capacity) * 2u : 8u;
        menu_list_growth::allocate_005e03c0(list, nullptr, capacity);
    }
    word_at(list->data + static_cast<U>(list->size) * 4u) = value;
    list->size = list->size + 1;
}
// Inline SafePointer unregister idiom (00458DDF..00458E16 / 0042390B..00423952 / 00457855..00457895 /
// 004A65A5..004A65E4): search the target's registration list backwards for `value`, store count-1 unconditionally,
// then shift the tail down from the found index (data and count re-read per pass). When the value is absent the
// original shifts from index -1 (writes data[-1]); reproduced with raw address arithmetic.
inline void unregister(void* target, U data_offset, U count_offset, U value) {
    const I last = field<I>(target, count_offset) - 1;
    I i = last;
    if (i >= 0) {
        U at_index = field<U>(target, data_offset) + static_cast<U>(i) * 4u;
        while (word_at(at_index) != value) {
            --i; at_index -= 4u;
            if (i < 0) break;
        }
    }
    field<I>(target, count_offset) = last;
    if (i < last) {
        do {
            const U data = field<U>(target, data_offset);
            word_at(data + static_cast<U>(i) * 4u) = word_at(data + static_cast<U>(i) * 4u + 4u);
            ++i;
        } while (i < field<I>(target, count_offset));
    }
}
// Deleting destructor slot +00 (`push 1; call [vtbl]`) of objects whose class table is not published: one-slot view.
struct DeletingSlot { void* (__fastcall* scalar_delete)(void*, void*, U); };
inline void delete_object(void* object) {
    (*static_cast<const DeletingSlot* const volatile*>(object))->scalar_delete(object, nullptr, 1u);
}
inline const PartTable* part_vt(void* part) { return *static_cast<const PartTable* const volatile*>(part); }
// CAurTexture table view (+00 deleting dtor, +04 base-object getter, +10 is_invalid AL): offsets/types identical to
// lane-texture-epoch texture_vtable.hpp CommonSlots (local view, precedent m46_gob_externs.h TextureSlots).
struct TextureSlots {
    void* (__fastcall* scalar_delete)(void*, void*, U);     // +00 (004239A6 / 004239AE push 1)
    void* (__fastcall* base)(void*, void*);                 // +04 (00423828 / 0042398E / 00423999)
    const void* s08[2];
    std::uint8_t (__fastcall* is_invalid)(void*, void*);    // +10 (00423860, AL)
};
static_assert(offsetof(TextureSlots, is_invalid) == 0x10, "CAurTexture +10");
inline const TextureSlots* texture_vt(void* t) { return *static_cast<const TextureSlots* const volatile*>(t); }
inline void free_if(U data) { if (data != 0u) live_free(ptr<void>(data)); }
}

extern "C" {
// ================================================================ cells / statics storage (see declarations)
volatile I shadow_light_max_007a1938 = 1;
volatile float shadow_light_fade_time_007a1948 = 0.3f;     // .data 3E99999A
volatile U beam_stencil_mask_0078e69c = 1u;
float beam_alpha_0078e6a0 = 0.2f;                          // .data 3E4CCCCD
Vector beam_color_0078e8a0 = {1.0f, 1.0f, 1.0f};
Vector add_triangle_v0_00831eb0 = {0.0f, 0.0f, 0.0f};
Vector add_triangle_v1_00831ea4 = {0.0f, 0.0f, 0.0f};
Vector add_triangle_v2_00831e98 = {0.0f, 0.0f, 0.0f};
volatile U add_triangle_guard_00831ebc = 0u;
Vector plane_transform_normal_00831f54 = {0.0f, 0.0f, 0.0f};
Vector plane_transform_point_00831f48 = {0.0f, 0.0f, 0.0f};
volatile U plane_transform_guard_00831f60 = 0u;
Quaternion global_wind_rotation_00831e2c = {0.0f, 0.0f, 0.0f, 0.0f};
volatile U global_wind_rotation_guard_00831e3c = 0u;

// ================================================================ 00454940 Scene::SetVisibility
// Original: __stdcall(room, visible_room, flag) RET 0Ch (004549D4 / 00454A11); ECX is not read. Consumer declaration
// is the D1 form (scene in ECX ignored).
U __fastcall scene_set_visibility_00454940(void*, void*, void* room, void* visible_room, U flag) {
    if (flag != 0u) {                                                   // 00454940..0045494D
        PointerList* rooms = &field<PointerList>(room, 0x5C);          // 0045495A lea esi,[ebx+5Ch]
        I found = 0;
        const I count = field<I>(room, 0x60);                           // 00454953
        for (I k = 0; k < count; ++k)                                   // 00454963..0045496E (count down)
            if (word_at(rooms->data + static_cast<U>(k) * 4u) == word(visible_room)) ++found;
        if (count > 0 && found != 0) return 0u;                         // 00454970 jne 00454A0E (EAX = 0 after the loop)
        append_word(rooms, word(visible_room));                         // 00454978..0045499D
        PointerList* back = &field<PointerList>(visible_room, 0x5C);   // 004549A8 lea esi,[edi+5Ch]
        append_word(back, word(room));                                  // 004549A0..004549CF (no duplicate test)
        return static_cast<U>(back->size);                              // 004549CA..004549CF EAX = new count
    }
    const I count = field<I>(room, 0x60);                               // 004549D7
    if (count <= 0) return static_cast<U>(count);                       // 004549E1 jle 00454A0E (EAX = count)
    I found = 0;
    for (I k = 0; k < count; ++k)                                       // 004549F0..004549F9
        if (word_at(field<U>(room, 0x5C) + static_cast<U>(k) * 4u) == word(visible_room)) ++found;
    if (found == 0) return 0u;                                          // 004549FD (EAX = 0 after the loop)
    texture_list_remove_00445080(&field<PointerList>(room, 0x5C), nullptr, word(visible_room));   // 004549FF..00454A00
    texture_list_remove_00445080(&field<PointerList>(visible_room, 0x5C), nullptr, word(room));   // 00454A05..00454A09
    return 0u;   // D2: EAX = 00445080's leftover (not reproducible); the only caller (004569B7) ignores it.
}

// ================================================================ 004A5C80 WindManager::pointSourceWindVector
// thiscall(out, Vector point) RET 10h (004A5E2C), EAX = out (004A5E0D). Point sources: CExoArrayList at +0C (data) /
// +10 (count), 18h-byte records {+00 position, +0C radius, +14 strength} (004A5CD0..004A5DD4).
void* __fastcall wind_manager_point_source_wind_vector_004a5c80(void* wind, void*, void* out, float x, float y, float z) {
    Vector sum;                                                         // 004A5C8E..004A5C9E [esp+24h..2Ch] = 0
    std::memset(&sum, 0, sizeof(sum));
    Vector d;                                                           // 004A5CA6..004A5CB6 [esp+18h..20h] = 0
    std::memset(&d, 0, sizeof(d));
    if (field<I>(wind, 0x10) > 0) {                                     // 004A5C87..004A5CBE
        for (I i = 0; i < field<I>(wind, 0x10); ++i) {                  // 004A5DFC..004A5E05 (count re-read)
            const float* src = ptr<const float>(field<U>(wind, 0x0C) + static_cast<U>(i) * 0x18u);   // 004A5CD0..004A5CDA
            const double dx = W(x) - W(src[0]);                         // 004A5CD3..004A5CD7 (kept wide)
            d.x = S(dx);                                                // 004A5CDD fst dword
            d.y = S(W(y) - W(src[1]));                                  // 004A5CE5..004A5CEC
            d.z = S(W(z) - W(src[2]));                                  // 004A5CF0..004A5D03
            float a = S(dx < W(kZero) ? -dx : dx);                      // 004A5D0B..004A5D1E (fchs only when below 0)
            const double ay = W(d.y) < W(kZero) ? -W(d.y) : W(d.y);     // 004A5D22..004A5D37
            if (ay > W(a)) {                                            // 004A5D39..004A5D42 (test ah,41h)
                if (W(d.y) < W(kZero)) a = S(-W(d.y));                  // 004A5D44..004A5D5B
                else std::memcpy(&a, &d.y, 4);                          // 004A5D61..004A5D65 raw DWORD copy
            }
            const double az = W(d.z) < W(kZero) ? -W(d.z) : W(d.z);     // 004A5D69..004A5D6D fabs
            if (W(a) < az) {                                            // 004A5D6F..004A5D7A (fcompp, test ah,5 jp)
                if (W(d.z) < W(kZero)) a = S(-W(d.z));                  // 004A5D7C..004A5D93
                else std::memcpy(&a, &d.z, 4);                          // 004A5D99..004A5D9D
            }
            if (W(a) < W(src[3])) {                                     // 004A5DA1..004A5DAD (a < radius)
                Vector n;
                const Vector* np = normalize_004ab180(&n, &d);          // 004A5DAF..004A5DB9
                const double s = (W(src[3]) - W(a)) / W(src[3]) * W(src[5]);   // 004A5DBE..004A5DD4
                const float sf = S(s);                                  // 004A5DD7 fst dword
                Vector t;
                t.z = S(s * W(np->z));                                  // 004A5DDB (wide s)
                t.y = S(W(sf) * W(np->y));                              // 004A5DDE..004A5DE2
                t.x = S(W(sf) * W(np->x));                              // 004A5DE5..004A5DE9
                vector_add_assign_004a9740(&sum, nullptr, &t);          // 004A5DF7
            }
        }
    }
    std::memcpy(out, &sum, sizeof(sum));                                // 004A5E0D..004A5E25 raw copies
    return out;
}

// ================================================================ 004A6050 WindManager::GetGlobalWind
// thiscall(Vector* out, Vector point, float scale) RET 14h (004A6107 / 004A6430), EAX = out. `last` is the raw stack
// DWORD of the float scale (consumer declaration).
Vector* __fastcall wind_manager_get_global_wind_004a6050(void* wind, void*, Vector* out, Vector point, U last) {
    const Vector* global = &field<Vector>(wind, 0x18);                  // 004A605B lea ebx,[esi+18h]
    if (W(global->x) == W(kZero) && W(global->y) == W(kZero) && W(global->z) == W(kZero)) {   // 004A6058..004A6092 (test ah,44h jp)
        Vector v;
        const Vector* r = static_cast<const Vector*>(
            wind_manager_point_source_wind_vector_004a5c80(wind, nullptr, &v, point.x, point.y, point.z));   // 004A6094..004A60B4
        if (W(r->x) == W(kZero) && W(r->y) == W(kZero) && W(r->z) == W(kZero)) {   // 004A60BB..004A60E8
            std::memcpy(out, global, sizeof(Vector));                   // 004A60EA..004A60FD raw copies of the zero vector
            return out;
        }
    }
    const I ix = wind_cell(x87_ftol_float(&point.x));                   // 004A610A..004A6129 -> ebp
    const I iy = wind_cell(x87_ftol_float(&point.y));                   // 004A612A..004A6149 -> edi
    float angle_a = 0.0f;                                               // 004A6157 [esp+14h] = 0
    float angle_b = 0.0f;                                               // 004A615F [esp+18h] = 0
    if ((global_wind_rotation_guard_00831e3c & 1u) == 0u) {             // 004A614A..004A6167
        global_wind_rotation_guard_00831e3c = global_wind_rotation_guard_00831e3c | 1u;   // 004A6169
        global_wind_rotation_00831e2c.z = 0.0f;                         // 004A616F [00831E38]
        global_wind_rotation_00831e2c.y = 0.0f;                         // 004A6179 [00831E34]
        global_wind_rotation_00831e2c.x = 0.0f;                         // 004A6183 [00831E30]
        global_wind_rotation_00831e2c.w = 1.0f;                         // 004A618D [00831E2C]
    }
    const float* grid0 = field<const float*>(wind, 0x830);              // 004A619D
    const float* grid1 = field<const float*>(wind, 0x834);              // 004A61A9
    const I index = ix * 16 + iy;                                       // 004A61AF..004A61B6
    const double sample = (W(kOne) - W(field<float>(wind, 0x840))) * W(grid0[index]) +
                          W(grid1[index]) * W(field<float>(wind, 0x840));   // 004A6197..004A61D2
    const float sample_f = S(sample);                                   // 004A61DD fst [esp+10h]
    const double magnitude = sample * W(kotor_scene3d::bits_to_float(last));   // 004A61E1 (wide sample)
    const float vx = S(magnitude * W(global->x));                       // 004A61ED..004A61F3
    const float vy = S(magnitude * W(global->y));                       // 004A61F7..004A61FC
    const float vz = S(magnitude * W(global->z));                       // 004A6200..004A6203
    Vector pw;                                                          // original: the point's own argument slot
    const Vector* p = static_cast<const Vector*>(
        wind_manager_point_source_wind_vector_004a5c80(wind, nullptr, &pw, point.x, point.y, point.z));   // 004A61BF..004A6207
    const float sx = S(W(vx) + W(p->x));                                // 004A620C..004A6212
    const float sy = S(W(vy) + W(p->y));                                // 004A6216..004A621D
    const float sz = S(W(vz) + W(p->z));                                // 004A6221..004A6228
    const float k1 = field<float>(wind, 0x838);
    if (!(W(k1) == W(kZero))) {                                         // 004A622C..004A623D (test ah,44h jnp)
        const double v = W(sample_f) * W(k1);                           // 004A623F..004A6243 (stored float sample)
        angle_a = S(((v + v) - W(k1)) * W(kHalf));                      // 004A6249..004A6257
    }
    const float k2 = field<float>(wind, 0x83C);
    if (!(W(k2) == W(kZero))) {                                         // 004A625B..004A626C
        const float* g0 = field<const float*>(wind, 0x830);             // 004A6274
        const float* g1 = field<const float*>(wind, 0x834);             // 004A6280
        const I index2 = iy * 16 + ix;                                  // 004A6286..004A628B
        const double sample2 = (W(kOne) - W(field<float>(wind, 0x840))) * W(g0[index2]) +
                               W(g1[index2]) * W(field<float>(wind, 0x840));   // 004A626E..004A629A
        const double v = sample2 * W(k2);                               // 004A629C
        angle_b = S(((v + v) - W(k2)) * W(kHalf));                      // 004A62A2..004A62B0
    }
    const float cos_a = cosine_lut_004aadf0(angle_a);                   // 004A62B4..004A62BE
    const float sin_a = sine_lut_004aae30(angle_a);                     // 004A62C2..004A62C8
    const float cos_b = cosine_lut_004aadf0(angle_b);                   // 004A62CC..004A62D6
    const float sin_b = sine_lut_004aae30(angle_b);                     // 004A62DA..004A62DB (table value kept in ST0)
    Quaternion& q = global_wind_rotation_00831e2c;
    q.w = S(W(cos_b) * W(cos_a));                                       // 004A62E0..004A62EF
    q.x = S(W(cos_a) * W(sin_b));                                       // 004A62F5..004A62FB
    q.y = S(W(sin_b) * W(sin_a));                                       // 004A6301..004A6305
    q.z = S(W(cos_b) * W(sin_a));                                       // 004A630B..004A6313
    const double xx = W(q.x) * W(q.x);                                  // 004A6319..004A631F
    const double yy = W(q.y) * W(q.y);                                  // 004A6325..004A632B
    const double zz = W(q.z) * W(q.z);                                  // 004A6331..004A6337
    const double xy = W(q.y) * W(q.x);                                  // 004A633D..004A6343
    const float xz = S(W(q.z) * W(q.x));                                // 004A6349..004A6355
    const float xw = S(W(q.x) * W(q.w));                                // 004A6359..004A6365
    const float yz = S(W(q.z) * W(q.y));                                // 004A6369..004A6375
    const float yw = S(W(q.y) * W(q.w));                                // 004A6379..004A6385
    const double zw = W(q.z) * W(q.w);                                  // 004A6389..004A638F
    {
        const double s = zz + yy;                                       // 004A6395..004A6397
        double r = (W(kOne) - (s + s)) * W(sx);                         // 004A6399..004A63A1
        const double t = xy - zw;                                       // 004A63A5..004A63A7
        r = r + (t + t) * W(sy);                                        // 004A63A9..004A63AF
        const double u = W(yw) + W(xz);                                 // 004A63B1..004A63B5
        r = r + (u + u) * W(sz);                                        // 004A63B9..004A63BF
        out->x = S(r);                                                  // 004A63C1
    }
    {
        const double s = zz + xx;                                       // 004A63C3..004A63C5
        double r = (W(kOne) - (s + s)) * W(sy);                         // 004A63C7..004A63CF
        const double t = W(yz) - W(xw);                                 // 004A63D3..004A63D7
        r = r + (t + t) * W(sz);                                        // 004A63DB..004A63E3
        const double u = zw + xy;                                       // 004A63E6..004A63E9
        r = r + (u + u) * W(sx);                                        // 004A63EB..004A63F1
        out->y = S(r);                                                  // 004A63F5
    }
    {
        const double s = yy + xx;                                       // 004A63FA
        double r = (W(kOne) - (s + s)) * W(sz);                         // 004A63FC..004A6404
        const double t = W(yz) + W(xw);                                 // 004A6408..004A640C
        r = r + (t + t) * W(sy);                                        // 004A6410..004A6416
        const double u = W(xz) - W(yw);                                 // 004A6418..004A641C
        r = r + (u + u) * W(sx);                                        // 004A6420..004A6426
        out->z = S(r);                                                  // 004A6428
    }
    return out;                                                         // EAX = [esp+38h] (004A62E8)
}

// ================================================================ 004AB4C0 Plane::Transform
// thiscall(const Vector* translation, const Quaternion* rotation) RET 8 (004AB62C). Function statics N 00831F54 /
// P 00831F48 (guard 00831F60 bits 1/2).
void __fastcall plane_transform_004ab4c0(Plane* plane, void*, const Vector* translation, const Quaternion* rotation) {
    U guard = plane_transform_guard_00831f60;                           // 004AB4C0
    if ((guard & 1u) == 0u) {                                           // 004AB4C8..004AB4ED
        guard |= 1u; plane_transform_guard_00831f60 = guard;
        std::memset(&plane_transform_normal_00831f54, 0, sizeof(Vector));
    }
    if ((guard & 2u) == 0u) {                                           // 004AB4F7..004AB517
        guard |= 2u; plane_transform_guard_00831f60 = guard;
        std::memset(&plane_transform_point_00831f48, 0, sizeof(Vector));
    }
    Quaternion conj;
    const Quaternion* c = quaternion_negate_w_004a9b50(&conj, rotation);   // 004AB521..004AB52C
    Vector rotated;
    const Vector* r = quaternion_rotate_004a9bb0(&rotated, c, reinterpret_cast<const Vector*>(plane));   // 004AB525..004AB53A
    U normal_x;
    std::memcpy(&normal_x, &r->x, 4);                                   // 004AB53F ebx
    std::memcpy(&plane_transform_normal_00831f54, r, sizeof(Vector));   // 004AB541..004AB553 raw copies
    const double nx = -W(plane->a);                                     // 004AB558..004AB55A
    const double ny = -W(plane->b);                                     // 004AB55F..004AB566
    const float nz = S(-W(plane->c));                                   // 004AB56C..004AB578 fstp [esp+14h]
    const float dist = plane->d;                                        // 004AB55C / 004AB568 (copied into the arg slot)
    const float px = S(nx * W(dist));                                   // 004AB57F..004AB584
    const double py = ny * W(dist);                                     // 004AB588
    const double pz = W(nz) * W(dist);                                  // 004AB58C..004AB590
    Vector point;
    point.x = S(W(px) - W(translation->x));                             // 004AB594..004AB59A
    point.y = S(py - W(translation->y));                                // 004AB59E..004AB5A3
    point.z = S(pz - W(translation->z));                                // 004AB5A7..004AB5AF
    Quaternion conj2;
    const Quaternion* c2 = quaternion_negate_w_004a9b50(&conj2, rotation);   // 004AB5AA..004AB5B3
    Vector rotated2;
    const Vector* r2 = quaternion_rotate_004a9bb0(&rotated2, c2, &point);   // 004AB5BB..004AB5C1
    std::memcpy(&plane_transform_point_00831f48, r2, sizeof(Vector));   // 004AB5C6..004AB5DA
    std::memcpy(&plane->a, &normal_x, 4);                               // 004AB5E2
    std::memcpy(&plane->b, &plane_transform_normal_00831f54.y, 4);      // 004AB5E4..004AB5EA
    std::memcpy(&plane->c, &plane_transform_normal_00831f54.z, 4);      // 004AB5ED..004AB5F3
    const Vector& N = plane_transform_normal_00831f54;
    const Vector& P = plane_transform_point_00831f48;
    const double dot = (W(P.z) * W(N.z) + W(P.y) * W(N.y)) + W(P.x) * W(N.x);   // 004AB5F6..004AB61F
    plane->d = S(-dot);                                                 // 004AB621..004AB624
}

// ================================================================ 004A4B10 LightManager::GetShadowLights
// thiscall(int mode) RET 4 (004A4F08), EAX = this+98h (the shadow-light CExoArrayList, 004A4EF5). EH frame 00718098
// (state 0 = the local copy list; its storage is freed on the normal path at 004A4EE7).
PointerList* __fastcall light_manager_get_shadow_lights_004a4b10(void* lights, void*, U mode) {
    PointerList* shadow = &field<PointerList>(lights, 0x98);
    shadow->size = 0;                                                   // 004A4B37
    if (mode == 2u) {                                                   // 004A4B33..004A4B3D
        I best = 6;                                                     // 004A4B48
        void* chosen = nullptr;                                         // ebp = 0 (004A4B31)
        const I count = field<I>(lights, 0x0C);                         // 004A4B43
        if (count <= 0) return shadow;                                  // 004A4B4D
        const U data = field<U>(lights, 0x08);                          // 004A4B53 (read once)
        for (I k = 0; k < count; ++k) {                                 // 004A4B58..004A4B94
            void* light = ptr<void>(word_at(data + static_cast<U>(k) * 4u));
            if (field<U>(light, 0x5C) == 0u || field<U>(light, 0x68) != 0u) continue;   // 004A4B5A..004A4B66
            const bool far = !(W(field<float>(light, 0x58)) < W(kThousand));   // 004A4B68..004A4B76 (test ah,5 jp)
            if (!far && field<U>(light, 0x70) != 0u) continue;          // 004A4B78..004A4B7D
            const I priority = field<I>(field<void*>(light, 0x04), 0x90);   // 004A4B7F..004A4B82
            if (priority < best) { best = priority; chosen = light; }   // 004A4B88..004A4B8E
        }
        if (chosen != nullptr) {                                        // 004A4B96
            append_word(shadow, word(chosen));                          // 004A4B9E..004A4BD1
            field<void*>(lights, 0xA4) = chosen;                        // 004A4BD4
        }
        return shadow;
    }
    if (shadow_light_max_007a1938 > 0) {                                // 004A4BDF..004A4BE5
        I k = 0;
        for (;;) {
            if (k >= field<I>(lights, 0x54)) break;                     // 004A4BE7..004A4BEA
            void* light = ptr<void>(word_at(field<U>(lights, 0x50) + static_cast<U>(k) * 4u));   // 004A4BEF..004A4BF2
            bool take;
            if (mode == 1u && field<U>(light, 0x70) == 0u) take = false;   // 004A4BEC..004A4BFC
            else take = !(W(field<float>(light, 0x80)) > W(kOne));     // 004A4BFE..004A4C0F (test ah,41h je)
            if (take) {
                append_word(shadow, word(light));                       // 004A4C11..004A4C4F
                if (mode == 1u) break;                                  // 004A4C4C..004A4C52
            }
            ++k;                                                        // 004A4C60
            if (!(shadow->size < shadow_light_max_007a1938)) break;     // 004A4C54..004A4C63
        }
    }
    ArrayMutations previous;                                            // 004A4C65..004A4C76 {0,0,0}, EH state 0
    previous.data = 0u; previous.size = 0; previous.capacity = 0;
    texture_list_assign_0046bc80(&previous, nullptr,
        &field<PointerList>(lights, mode == 1u ? 0xB8u : 0xACu));       // 004A4C73..004A4C90
    const I previous_count = previous.size;                             // 004A4C9B ebx
    const U previous_data = previous.data;                              // 004A4C9F ebp
    for (I j = 0; j < shadow->size; ++j) {                              // 004A4CA3..004A4CE4
        void* light = ptr<void>(word_at(shadow->data + static_cast<U>(j) * 4u));
        I found = 0;
        for (I m = 0; m < previous_count; ++m)
            if (word_at(previous_data + static_cast<U>(m) * 4u) == word(light)) ++found;
        if (previous_count <= 0 || found == 0) field<U>(light, 0x90) = 0u;   // 004A4CD1 (fade starts at 0.0f)
    }
    if (shadow_light_max_007a1938 > 0) {                                // 004A4CE6..004A4CF3
        I i = 0;
        for (;;) {
            if (i >= previous_count) break;                             // 004A4D00..004A4D02
            void* light = ptr<void>(word_at(previous_data + static_cast<U>(i) * 4u));   // 004A4D08..004A4D0C
            I found = 0;
            const I count = shadow->size;                               // 004A4D0F
            for (I m = 0; m < count; ++m)                               // 004A4D1B..004A4D2A
                if (word_at(shadow->data + static_cast<U>(m) * 4u) == word(light)) ++found;
            if (count <= 0 || found == 0) {
                const float dt = kotor_scene3d::bits_to_float(*cells.frame_time_0078e574);
                double t = W(dt) / W(shadow_light_fade_time_007a1948);  // 004A4D34..004A4D3A
                t = t + t;                                              // 004A4D40
                double v = W(field<float>(light, 0x90)) - t;            // 004A4D42 fsubr
                if (W(kZero) > v) v = W(kZero);                         // 004A4D48..004A4D59 (unordered keeps v)
                field<float>(light, 0x90) = S(v);                       // 004A4D5F fst
                if (v > W(kZero)) {                                     // 004A4D65..004A4D70 (wide v)
                    const I old_count = shadow->size;                   // 004A4D76 edi
                    if (old_count == shadow->capacity) {                // 004A4D7C..004A4D84
                        const I capacity = (shadow->capacity != 0) ? shadow->capacity * 2 : 8;   // 004A4D86..004A4D8E
                        const U old = shadow->data;                     // 004A4D93
                        shadow->capacity = capacity;                    // 004A4DA1 (before the allocation)
                        shadow->data = word(live_operator_new(static_cast<U>(capacity) * 4u));   // 004A4DA7..004A4DB2
                        for (I m = 0; m < shadow->size; ++m)            // 004A4DBB..004A4DD6
                            word_at(shadow->data + static_cast<U>(m) * 4u) = word_at(old + static_cast<U>(m) * 4u);
                        if (old != 0u) live_free(ptr<void>(old));       // 004A4DD8..004A4DDD
                    }
                    word_at(shadow->data + static_cast<U>(shadow->size) * 4u) = word(light);   // 004A4DE5..004A4DF1
                    shadow->size = shadow->size + 1;                    // 004A4DF4..004A4DFD
                    for (I m = old_count; m > 0; --m)                   // 004A4E03..004A4E22
                        word_at(shadow->data + static_cast<U>(m) * 4u) = word_at(shadow->data + static_cast<U>(m - 1) * 4u);
                    word_at(shadow->data) = word(light);                // 004A4E24..004A4E32
                }
            }
            ++i;                                                        // 004A4E3D
            if (!(i < shadow_light_max_007a1938)) break;                // 004A4E34..004A4E44
        }
    }
    if (shadow->size > shadow_light_max_007a1938) shadow->size = shadow_light_max_007a1938;   // 004A4E4A..004A4E57
    for (I c = 0; c < shadow->size; ++c) {                              // 004A4E5D..004A4EB1
        const float dt = kotor_scene3d::bits_to_float(*cells.frame_time_0078e574);
        void* light = ptr<void>(word_at(shadow->data + static_cast<U>(c) * 4u));
        double v = W(dt) / W(shadow_light_fade_time_007a1948) + W(field<float>(light, 0x90));   // 004A4E70..004A4E85
        if (W(kOne) < v) v = W(kOne);                                   // 004A4E8B..004A4E9C (unordered keeps v)
        field<float>(light, 0x90) = S(v);                               // 004A4EA2
    }
    texture_list_assign_0046bc80(&field<PointerList>(lights, mode == 1u ? 0xB8u : 0xACu), nullptr, shadow);   // 004A4EB3..004A4ED6
    free_if(previous_data);                                             // 004A4EDB..004A4EE8 (state -1)
    return shadow;                                                      // 004A4EF5
}

// ================================================================ 00471450 SetBeaming (cdecl, plain RET 004714A8)
void __cdecl set_beaming_00471450() {
    kotor_renderer_init::glBlendFunc_0073d38c(kGlSrcAlpha_0073f270, kGlOne_0073f278);   // 00471450..0047145D
    kotor_renderer_init::glColorMask_0073d358(1u, 1u, 1u, 1u);         // 00471463..0047146B
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_lighting_0078d45c);   // 00471471..00471478
    kotor_renderer_init::glColor4fv_0073d35c(homogenize_004aaab0(&beam_color_0078e8a0, &beam_alpha_0078e6a0));   // 0047147E..00471491
    if (beam_stencil_mask_0078e69c != 0u)                               // 00471497..0047149E
        kotor_renderer_init::glStencilMask_0073d278(0u);                // 004714A0..004714A2
}
} // extern C

// ================================================================ CExoArrayList<CAurTriangleListElem> (18h elements)
// 004A7220 CAurTriangleListElem::CAurTriangleListElem(): thiscall RET 0, EAX = this (004A7220..004A7235).
void* __fastcall triangle_list_elem_default_construct_004a7220(void* self, void*) {
    field<U>(self, 0x04) = 0u; field<U>(self, 0x08) = 0u; field<U>(self, 0x00) = 0u;   // 004A7224..004A722A
    field<U>(self, 0x0C) = 0u; field<U>(self, 0x10) = 0u; field<U>(self, 0x14) = 0u;   // 004A722C..004A7232
    return self;
}

// 004A7240 CAurTriangleListElem::CAurTriangleListElem(CExoArrayList<float> list, face, part, count): thiscall RET 18h,
// EAX = this (004A72BA). The by-value list is assigned (00438A50) and then destroyed by the callee (004A72A3..004A72AE,
// EH frame 00718190 state 0).
void* __fastcall triangle_list_elem_construct_004a7240(void* self, void*, ArrayMutations list, void* face, void* part, I count) {
    field<U>(self, 0x04) = 0u; field<U>(self, 0x08) = 0u; field<U>(self, 0x00) = 0u;   // 004A7265..004A7273
    float_list_assign_00438a50(&field<ArrayMutations>(self, 0x00), nullptr, &list);    // 004A7279..004A7283
    field<I>(self, 0x14) = count;                                       // 004A7288 / 004A7294
    field<void*>(self, 0x0C) = face;                                    // 004A728C / 004A729D
    field<void*>(self, 0x10) = part;                                    // 004A7290 / 004A72A0
    free_if(list.data);                                                 // 004A7297..004A72AE (argument destructor)
    return self;
}

// 004A73F0 CExoArrayList<CAurTriangleListElem>::allocate(int count): thiscall RET 4 (004A74E4). new[] with a count
// cookie (operator new 006FA7E6 of count*18h+4; `eh vector constructor iterator` 006FB55F with 004A7220 / 004A72D0),
// element-wise assignment (00438A50 + three words), `eh vector destructor iterator` 006FA852 + free of the old block.
void __fastcall triangle_list_allocate_004a73f0(PointerList* list, void*, I count) {
    const U old = list->data;                                           // 004A7413 / 004A741D
    list->capacity = count;                                             // 004A7421 (before the allocation)
    void* block = live_operator_new(static_cast<U>(count) * 0x18u + 4u);   // 004A740F..004A7424
    U first = 0u;                                                       // 004A7430 ebp = 0
    if (block != nullptr) {                                             // 004A7432..004A7438
        first = word(block) + 4u;                                       // 004A7445 lea ebp,[eax+4]
        field<I>(block, 0) = count;                                     // 004A744B cookie
        menu_tables_m41::VectorConstruct_006fb55f(ptr<void>(first), 0x18u, static_cast<U>(count),
            &triangle_list_elem_default_construct_004a7220, &triangle_list_elem_destruct_004a72d0);   // 004A743A..004A744D
    }
    list->data = first;                                                 // 004A7455
    for (I k = 0; k < list->size; ++k) {                                // 004A7457..004A74AB (count re-read)
        const U source = old + static_cast<U>(k) * 0x18u;
        const U target = list->data + static_cast<U>(k) * 0x18u;        // 004A7480..004A7484 (data re-read)
        float_list_assign_00438a50(ptr<ArrayMutations>(target), nullptr, ptr<ArrayMutations>(source));   // 004A7486..004A748C
        word_at(target + 0x0Cu) = word_at(source + 0x0Cu);              // 004A7491..004A7494
        word_at(target + 0x10u) = word_at(source + 0x10u);              // 004A7497..004A7499
        word_at(target + 0x14u) = word_at(source + 0x14u);              // 004A749C..004A749F
    }
    if (old != 0u) {                                                    // 004A74B1..004A74B3
        c2da_destroy_array_006fa852(ptr<void>(old), 0x18u, field<I>(ptr<void>(old - 4u), 0),
                                    &triangle_list_elem_destruct_004a72d0);   // 004A74B5..004A74C4
        live_free(ptr<void>(old - 4u));                                 // 004A74C9..004A74CA
    }
}

// 004A77C0 CExoArrayList<CAurTriangleListElem>::Add(CAurTriangleListElem by value): thiscall RET 18h (004A784F).
// The count is incremented before the slot is assigned (004A7806..004A7807); the callee destroys its argument
// (004A783A, EH frame 00718208 state 0).
void __fastcall triangle_list_add_004a77c0(PointerList* list, void*, M47TriangleListElem elem) {
    if (list->size == list->capacity) {                                 // 004A77D9..004A77E7
        const I capacity = (list->capacity != 0) ? list->capacity + list->capacity : 8;   // 004A77E9..004A77F1
        triangle_list_allocate_004a73f0(list, nullptr, capacity);       // 004A77F6..004A77F9
    }
    const I index = list->size;                                         // 004A77FE
    list->size = index + 1;                                             // 004A7806..004A7807
    const U slot = list->data + static_cast<U>(index) * 0x18u;          // 004A7801..004A780A
    float_list_assign_00438a50(ptr<ArrayMutations>(slot), nullptr, &elem.corners);   // 004A780D..004A7814
    word_at(slot + 0x0Cu) = elem.face_0c;                               // 004A7819..004A781D
    word_at(slot + 0x10u) = elem.part_10;                               // 004A7820..004A7824
    word_at(slot + 0x14u) = elem.count_14;                              // 004A7827..004A782F
    triangle_list_elem_destruct_004a72d0(&elem, nullptr);               // 004A782B..004A783A
}

// ================================================================ 004A82B0 CAurTriangleBin::CAurTriangleBin
// thiscall RET 0, EAX = this (004A8336). EH frame 0071828C. Offsets: triangle_bin.h byte view.
void* __fastcall triangle_bin_construct_004a82b0(void* self, void*) {
    for (U o = 0x14u; o <= 0x34u; o += 4u) field<U>(self, o) = 0u;      // 004A82CB..004A82E3 (+14..+34 in order)
    field<U>(self, 0x4C) = 0u; field<U>(self, 0x50) = 0u; field<U>(self, 0x48) = 0u;   // 004A82E7..004A82ED
    field<U>(self, 0x54) = 0u;                                          // 004A82F0..004A82F3 SafePointer target
    field<U>(self, 0x5C) = 0u; field<U>(self, 0x60) = 0u; field<U>(self, 0x58) = 0u;   // 004A82F5..004A82FB
    field<U>(self, 0x68) = 0u; field<U>(self, 0x6C) = 0u; field<U>(self, 0x64) = 0u;   // 004A82FE..004A8304
    field<U>(self, 0x0C) = 0u;                                          // 004A8307
    field<std::uint8_t>(self, 0x02) = 0u;                               // 004A830A
    const U target = field<U>(self, 0x54);                              // 004A830D SafePointer::operator=(0) inline
    if (target != 0u)                                                   // 004A830F..004A8311
        texture_list_remove_00445080(&field<PointerList>(ptr<void>(target), 4), nullptr, word(&field<U>(self, 0x54)));   // 004A8313..004A8317
    field<U>(self, 0x54) = 0u;                                          // 004A8320
    field<U>(self, 0x38) = 0u; field<U>(self, 0x3C) = 0u;               // 004A8322..004A8325
    field<U>(self, 0x40) = 0u; field<U>(self, 0x44) = 0u;               // 004A8328..004A832B
    field<I>(self, 0x10) = -1;                                          // 004A832F
    return self;
}

// ================================================================ 004A7860 CAurTriangleBin::AddTriangle
// thiscall(face, part, trimesh node) RET 0Ch (004A81A4). EH frame 00718230 (state 0 = local CExoArrayList<float>,
// state 1 = temporary CAurTriangleListElem); the local list storage is freed on the normal path (004A8189).
// Function statics V0 00831EB0 / V1 00831EA4 / V2 00831E98, guard 00831EBC bits 1/2/4.
void __fastcall triangle_bin_add_triangle_004a7860(void* bin, void*, void* face, void* part, void* node) {
    U guard = add_triangle_guard_00831ebc;                              // 004A7878
    if ((guard & 1u) == 0u) { guard |= 1u; add_triangle_guard_00831ebc = guard; std::memset(&add_triangle_v0_00831eb0, 0, 12); }   // 004A787D..004A78A5
    if ((guard & 2u) == 0u) { guard |= 2u; add_triangle_guard_00831ebc = guard; std::memset(&add_triangle_v1_00831ea4, 0, 12); }   // 004A78AF..004A78CF
    if ((guard & 4u) == 0u) { guard |= 4u; add_triangle_guard_00831ebc = guard; std::memset(&add_triangle_v2_00831e98, 0, 12); }   // 004A78D9..004A78F9
    const U vertices = field<U>(node, 0x198);                           // 004A7903..004A790A esi
    if (field<I>(bin, 0x10) == -1) {                                    // 004A7910..004A7918
        field<I>(bin, 0x10) = field<I>(face, 0x10);                     // 004A791A..004A7921
        aur_texture_set_mode_static_004204b0();                         // 004A7924
        const U record = word(*cells.grass_material_table_00831e44) + static_cast<U>(field<I>(bin, 0x10)) * 0x54u;   // 004A792D..004A7939
        void* texture = aur_texture_get_reference_00423a60(ptr<const char>(record), 0u);   // 004A793B..004A793D
        assign_00447fa0(at(bin, 0x54), nullptr, texture);               // 004A7949..004A794D
        aur_texture_set_mode_dynamic_004204c0();                        // 004A7952
    }
    std::memcpy(&add_triangle_v0_00831eb0, ptr<void>(vertices + static_cast<U>(static_cast<I>(field<std::int16_t>(face, 0x1A))) * 12u), 12);   // 004A795B..004A797B
    std::memcpy(&add_triangle_v1_00831ea4, ptr<void>(vertices + static_cast<U>(static_cast<I>(field<std::int16_t>(face, 0x1C))) * 12u), 12);   // 004A7980..004A799E
    std::memcpy(&add_triangle_v2_00831e98, ptr<void>(vertices + static_cast<U>(static_cast<I>(field<std::int16_t>(face, 0x1E))) * 12u), 12);   // 004A79A3..004A79C9
    Vector position;                                                    // 004A79D8..004A79E8 = 0
    std::memset(&position, 0, sizeof(position));
    Quaternion orientation;                                             // 004A79F0..004A7A08 = (1,0,0,0)
    std::memset(&orientation, 0, sizeof(orientation));
    orientation.w = 1.0f;
    part_vt(part)->s00c(part, nullptr, word(&position), word(&orientation));   // 004A7A10..004A7A13 Part::GetWorld
    Vector r;
    const Vector* r0 = quaternion_rotate_004a9bb0(&r, &orientation, &add_triangle_v0_00831eb0);   // 004A7A16..004A7A25
    const float v0x = S(W(position.x) + W(r0->x));                      // 004A7A3D..004A7A43
    const float v0y = S(W(position.y) + W(r0->y));                      // 004A7A36..004A7A51
    const float v0z = S(W(position.z) + W(r0->z));                      // 004A7A2A..004A7A62
    add_triangle_v0_00831eb0.x = v0x; add_triangle_v0_00831eb0.y = v0y; add_triangle_v0_00831eb0.z = v0z;   // 004A7A4B / 004A7A5D / 004A7A70
    Vector r_1;
    const Vector* r1 = quaternion_rotate_004a9bb0(&r_1, &orientation, &add_triangle_v1_00831ea4);   // 004A7A2E..004A7A76
    const float v1x = S(W(position.x) + W(r1->x));                      // 004A7A89..004A7A8F
    const float v1y = S(W(position.y) + W(r1->y));                      // 004A7A82..004A7A9D
    const float v1z = S(W(position.z) + W(r1->z));                      // 004A7A7B..004A7AA5
    add_triangle_v1_00831ea4.x = v1x; add_triangle_v1_00831ea4.y = v1y; add_triangle_v1_00831ea4.z = v1z;   // 004A7A97 / 004A7AB6 / 004A7AC2
    Vector r_2;
    const Vector* r2 = quaternion_rotate_004a9bb0(&r_2, &orientation, &add_triangle_v2_00831e98);   // 004A7AAD..004A7AC7
    const double v2z_wide = W(position.z) + W(r2->z);                   // 004A7ACC..004A7AD0 (kept on the stack)
    const float v2y = S(W(position.y) + W(r2->y));                      // 004A7AD3..004A7ADA fst
    const float v2x = S(W(position.x) + W(r2->x));                      // 004A7ADE..004A7AE4
    const float v2z = S(v2z_wide);                                      // 004A7AFD fst
    add_triangle_v2_00831e98.x = v2x; add_triangle_v2_00831e98.y = v2y; add_triangle_v2_00831e98.z = v2z;   // 004A7AEC / 004A7B0B / 004A7B24
    Vector edge2;                                                       // V2 - V1 (004A7B05..004A7B3E)
    edge2.x = S(W(v2x) - W(add_triangle_v1_00831ea4.x));
    edge2.y = S(W(v2y) - W(add_triangle_v1_00831ea4.y));
    edge2.z = S(v2z_wide - W(add_triangle_v1_00831ea4.z));              // wide z of V2 (004A7B05)
    Vector edge1;                                                       // V1 - V0 (004A7B42..004A7B6E)
    edge1.x = S(W(add_triangle_v1_00831ea4.x) - W(add_triangle_v0_00831eb0.x));
    edge1.y = S(W(add_triangle_v1_00831ea4.y) - W(add_triangle_v0_00831eb0.y));
    edge1.z = S(W(add_triangle_v1_00831ea4.z) - W(add_triangle_v0_00831eb0.z));
    Vector cross;
    const Vector* c = vector_cross_004ab210(&cross, &edge1, &edge2);    // 004A7B72
    const U record = word(*cells.grass_material_table_00831e44) + static_cast<U>(field<I>(face, 0x10)) * 0x54u;   // 004A7B83..004A7B93
    const float area = S(vector_magnitude_004ab100(c) * W(kHalf) * W(field<float>(ptr<void>(record), 0x20)));   // 004A7B78..004A7BA5
    const I count = x87_round_store(W(area));                           // 004A7BA9..004A7BB3 frndint + fistp
    if (count == 0) return;                                             // 004A7BB5..004A7BB9
    if (field<std::uint8_t>(node, 0x184) != 0u && kotor_vp_cells::gate_0078e608 != 0u) {   // 004A7BBF..004A7BD4
        field<std::uint8_t>(bin, 0x02) = static_cast<std::uint8_t>(field<std::uint8_t>(bin, 0x02) | 8u);   // 004A7BDA
    } else if (field<std::uint8_t>(part, 0x7C) == 1u) {                 // 004A7BE0..004A7BEB
        const std::uint8_t flags = field<std::uint8_t>(bin, 0x02);      // 004A7BF1
        if ((flags & 8u) == 0u) field<std::uint8_t>(bin, 0x02) = static_cast<std::uint8_t>(flags | 0x10u);   // 004A7BF4..004A7C01
    }
    ArrayMutations corners;                                             // 004A7C04..004A7C0C {0,0,0}, EH state 0
    corners.data = 0u; corners.size = 0; corners.capacity = 0;
    const U words[9] = {                                                // raw DWORDs of the statics (mov edx,[...])
        float_to_bits(add_triangle_v0_00831eb0.x), float_to_bits(add_triangle_v0_00831eb0.y), float_to_bits(add_triangle_v0_00831eb0.z),
        float_to_bits(add_triangle_v1_00831ea4.x), float_to_bits(add_triangle_v1_00831ea4.y), float_to_bits(add_triangle_v1_00831ea4.z),
        float_to_bits(add_triangle_v2_00831e98.x), float_to_bits(add_triangle_v2_00831e98.y), float_to_bits(add_triangle_v2_00831e98.z)};
    kotor_renderer_gl::allocate_00437d60(&corners, nullptr, 8u);        // 004A7C15..004A7C23 (unconditional first growth)
    for (U k = 0; k < 9u; ++k) {                                        // 004A7C28..004A7E1D (nine fld/fstp dword stores)
        if (k != 0u && corners.size == corners.capacity) {
            const U capacity = (corners.capacity != 0) ? static_cast<U>(corners.capacity) * 2u : 8u;
            kotor_renderer_gl::allocate_00437d60(&corners, nullptr, capacity);
        }
        const float value = x87_float_copy(bits_to_float(words[k]));    // fld dword [esp+1Ch]; fstp dword [ebp+eax*4]
        std::memcpy(ptr<void>(corners.data + static_cast<U>(corners.size) * 4u), &value, 4);
        corners.size = corners.size + 1;
    }
    M47TriangleListElem temp;                                           // [esp+44h] (state 1)
    {
        ArrayMutations list_arg;                                        // copy constructed in the argument area (004A7E3A..004A7E45)
        triangle_list_elem_copy_construct_004a7340(&list_arg, nullptr, &corners);
        triangle_list_elem_construct_004a7240(&temp, nullptr, list_arg, face, part, count);   // 004A7E30..004A7E4E
    }
    {
        M47TriangleListElem arg;                                        // 004A7E55..004A7E7D: list copy + three words
        triangle_list_elem_copy_construct_004a7340(&arg.corners, nullptr, &temp.corners);   // 004A7E5E..004A7E69
        arg.face_0c = temp.face_0c; arg.part_10 = temp.part_10; arg.count_14 = temp.count_14;
        triangle_list_add_004a77c0(&field<PointerList>(bin, 0x48), nullptr, arg);   // 004A7E80..004A7E87 (callee destroys)
    }
    triangle_list_elem_destruct_004a72d0(&temp, nullptr);               // 004A7E8C..004A7E94 (state 0)
    field<I>(bin, 0x0C) = field<I>(bin, 0x0C) + count;                  // 004A7E99..004A7EA6
    const Vector* points[3] = {&add_triangle_v0_00831eb0, &add_triangle_v1_00831ea4, &add_triangle_v2_00831e98};
    U first = 0u;
    if ((field<std::uint8_t>(bin, 0x02) & 2u) == 0u) {                  // 004A7EA9..004A7EB6
        std::memcpy(at(bin, 0x14), points[0], 12);                      // 004A7EB8..004A7ECF minimum = V0
        std::memcpy(at(bin, 0x20), points[0], 12);                      // 004A7ED2..004A7EF0 maximum = V0
        field<std::uint8_t>(bin, 0x02) = static_cast<std::uint8_t>(field<std::uint8_t>(bin, 0x02) | 2u);   // 004A7EF3..004A7EF7
        first = 1u;                                                     // 004A7EFB jmp 004A7FB3
    }
    for (U k = first; k < 3u; ++k) {                                    // 004A7F00..004A811C
        Vector tmp;
        const Vector* mn = vector_min_004aae70(&tmp, field<Vector>(bin, 0x14), *points[k]);
        std::memcpy(at(bin, 0x14), mn, 12);
        Vector tmp2;
        const Vector* mx = vector_max_004aaee0(&tmp2, field<Vector>(bin, 0x20), *points[k]);
        std::memcpy(at(bin, 0x20), mx, 12);
    }
    const double z_sum = W(field<float>(bin, 0x1C)) + W(field<float>(bin, 0x28));   // 004A8123..004A8129 (kept wide)
    const float y_sum = S(W(field<float>(bin, 0x18)) + W(field<float>(bin, 0x24)));  // 004A8131..004A813F
    const float x_sum = S(W(field<float>(bin, 0x14)) + W(field<float>(bin, 0x20)));  // 004A8143..004A8149
    const float cz = S(z_sum * W(kHalf));                               // 004A814D
    const float cy = S(W(y_sum) * W(kHalf));                            // 004A8153..004A8157
    const float cx = S(W(x_sum) * W(kHalf));                            // 004A815D..004A8161
    field<float>(bin, 0x2C) = cx; field<float>(bin, 0x30) = cy; field<float>(bin, 0x34) = cz;   // 004A8167..004A8184
    free_if(corners.data);                                              // 004A812F / 004A8187..004A818A (state -1)
}

// ================================================================ 0046C060 SceneBSPGetObjectTextures (cdecl)
// Pre-order walk of a Part tree: each part's material texture ([[material+44h]]) is appended. The compiler unrolled
// the recursion: level 0 appends inline (005E03C0 growth, 0046C082..0046C0AC), nested levels 1..8 append through
// CExoArrayList<int>::Add 00444FE0 (0046C0E6 .. 0046C28D) after a null test of the child, and the children of level 8
// re-enter 0046C060 (0046C2AC). Child lists: data +30, count +34 (re-read per pass).
namespace {
void object_textures(void* part, PointerList* out, int depth) {
    void* material = ptr<void>(part_vt(part)->s02c(part, nullptr));    // call [vtbl+2Ch]
    if (material != nullptr) {
        const U texture = word_at(field<U>(material, 0x44));            // mov ecx,[eax+44h]; mov edx,[ecx]
        if (depth == 0) append_word(out, texture);
        else gui_viewport_list::add_00444fe0(out, nullptr, static_cast<int>(texture));
    }
    for (I i = 0; i < field<I>(part, 0x34); ++i) {
        void* child = ptr<void>(word_at(field<U>(part, 0x30) + static_cast<U>(i) * 4u));
        if (depth == 8) scene_bsp_get_object_textures_0046c060(child, out);   // 0046C2A0..0046C2AC
        else if (child != nullptr) object_textures(child, out, depth + 1);
    }
}
}
void __cdecl scene_bsp_get_object_textures_0046c060(void* part, PointerList* out) {
    if (part == nullptr) return;                                        // 0046C064..0046C06A
    object_textures(part, out, 0);
}

// ================================================================ 0046C380 SceneBSPGetTileTextures (cdecl)
void __cdecl scene_bsp_get_tile_textures_0046c380(void* scene, const Vector* position, PointerList* out) {
    void* root = field<void*>(scene, 0xA0);                             // 0046C380..0046C384
    void* tile = nullptr;                                               // 0046C393 edi = 0
    if (root != nullptr) {
        void* node = bsp_fit_sphere_004ad060(root, position, 0.0f);     // 0046C397..0046C39E (radius DWORD 0)
        tile = field<void*>(field<void*>(node, 0x58), 0x70);            // 0046C3A3..0046C3A6
    }
    out->size = 0;                                                      // 0046C3B2
    if (tile == nullptr) return;                                        // 0046C3B5
    for (I i = 0; i < field<I>(tile, 0x1C); ++i) {                      // 0046C3B7..0046C402
        void* p = ptr<void>(word_at(field<U>(tile, 0x18) + static_cast<U>(i) * 4u));   // 0046C3C0..0046C3C3
        void* material = ptr<void>(part_vt(p)->s02c(p, nullptr));       // 0046C3C6..0046C3C8
        if (material != nullptr) append_word(out, word_at(field<U>(material, 0x44)));   // 0046C3CF..0046C3F9
    }
    void* object = field<void*>(tile, 0x24);                            // 0046C405
    if (object != nullptr) scene_bsp_get_object_textures_0046c060(field<void*>(object, 0x6C), out);   // 0046C40C..0046C411
}

// ================================================================ 004237E0 AurTextureReplaceTileReferences (cdecl)
// (scene, const Vector* position, const char* new_name, const char* old_name) -> 1 replaced / 0. EH frame 00715810
// (state 0/1 = the two local lists, freed on every normal path).
U __cdecl aur_texture_replace_tile_references_004237e0(void* scene, U position, U new_name, U old_name) {
    void* found = find_00420ac0(ptr<const char>(old_name), ptr<const char>(old_name), nullptr, 0);   // 004237EE..00423803
    if (found == nullptr) return 0u;                                    // 0042380B..00423820
    void* basic = texture_vt(found)->base(found, nullptr);              // 00423821..0042382B ebx
    if (field<std::uint8_t>(basic, 0xD3) == 0u) aur_texture_mode_dynamic_0078d400 = 0u;   // 0042382D..00423837
    void* fresh = get_00423490(ptr<const char>(new_name), nullptr, nullptr, nullptr, 0u);   // 0042383E..00423847
    aur_texture_mode_dynamic_0078d400 = 1u;                             // 0042384E
    if (texture_vt(fresh)->is_invalid(fresh, nullptr) != 0u) {          // 00423855..00423865
        texture_release_00420d70(fresh);                                // 00423867..00423868
        return 0u;                                                      // 0042386D -> 00423A3E
    }
    ArrayMutations tiles;  tiles.data = 0u; tiles.size = 0; tiles.capacity = 0;        // 00423872..0042387A
    ArrayMutations matched; matched.data = 0u; matched.size = 0; matched.capacity = 0; // 00423880..0042388C
    scene_bsp_get_tile_textures_0046c380(scene, ptr<const Vector>(position), &tiles);   // 00423890..004238A4
    const I tile_count = tiles.size;                                    // 004238A9 esi
    const U tile_data = tiles.data;                                     // 004238AD edx
    I hit = -1;
    for (I i = 0; i < tile_count; ++i) {                                // 004238B4..004238CF
        const U e = word_at(tile_data + static_cast<U>(i) * 4u);
        if (e != 0u && word_at(e + 0x10u) == word(basic)) { hit = i; break; }   // 004238C3..004238CA
    }
    if (hit < 0) {                                                      // 004238D1 -> 00423A16
        free_if(matched.data);
        free_if(tiles.data);
        return 0u;
    }
    texture_join::add_unique_0046bd10(&matched, nullptr, ptr<font::Texture>(word_at(tile_data + static_cast<U>(hit) * 4u)));   // 004238D6..004238DE
    if (matched.size <= 0) {                                            // 004238E3..004238E9 -> 00423A0E
        free_if(matched.data);
        free_if(tiles.data);
        return 0u;
    }
    for (I j = 0; j < matched.size; ++j) {                              // 00423904..004239BF (count re-read)
        const U e = word_at(matched.data + static_cast<U>(j) * 4u);     // 00423904..00423908
        const U safe = e + 0x10u;                                       // 00423910 lea edi,[eax+10h]
        const U target = word_at(safe);                                 // 0042390B
        if (target != 0u) unregister(ptr<void>(target), 0x04u, 0x08u, safe);   // 00423913..00423952
        word_at(safe) = word(fresh);                                    // 00423954..00423958
        append_word(&field<PointerList>(fresh, 0x04), safe);            // 004238FD / 0042395A..00423980
        if (field<U>(basic, 0x08) == 0u) {                              // 00423983..00423988 no registrations left
            if (texture_vt(basic)->base(basic, nullptr) != nullptr) {   // 0042398A..00423995
                void* inner = texture_vt(basic)->base(basic, nullptr); // 00423997..00423999
                if (inner != nullptr) texture_vt(inner)->scalar_delete(inner, nullptr, 1u);   // 0042399C..004239A6
            } else {
                texture_vt(basic)->scalar_delete(basic, nullptr, 1u);   // 004239AA..004239AE
            }
        }
    }
    free_if(matched.data);                                              // 004239C7..004239D5
    free_if(tiles.data);                                                // 004239DD..004239EE
    return 1u;                                                          // 004239F9
}

// ================================================================ destructor chain of Scene::~Scene
// 004ACED0 BSPNode::~BSPNode: thiscall RET 0 (EH frame 00718321 states 2/1/0).
void __fastcall bsp_node_destruct_004aced0(void* node, void*) {
    void* front = field<void*>(node, 0x10);                             // 004ACEEE
    if (front != nullptr) { bsp_node_destruct_004aced0(front, nullptr); live_free(front); }   // 004ACEFD..004ACF05
    void* back = field<void*>(node, 0x14);                              // 004ACF0D
    if (back != nullptr) { bsp_node_destruct_004aced0(back, nullptr); live_free(back); }      // 004ACF14..004ACF1C
    exo_array_list_free_0047abc0(at(node, 0x4C), nullptr);              // 004ACF24..004ACF2C
    exo_array_list_free_0047abc0(at(node, 0x40), nullptr);              // 004ACF31..004ACF39
    exo_array_list_free_0047abc0(at(node, 0x34), nullptr);              // 004ACF3E..004ACF49
}

// 00452A60 CAurRoom::~CAurRoom: thiscall RET 0 (EH frame 007165CE); base BSPNode at +0.
void __fastcall aur_room_destruct_00452a60(void* room, void*) {
    void* owned = field<void*>(room, 0x74);                             // 00452A7D
    if (owned != nullptr) delete_object(owned);                         // 00452A80..00452A90
    for (I i = 0; i < field<I>(room, 0x6C); ++i) {                      // 00452A92..00452AB6 (count re-read)
        void* entry = ptr<void>(word_at(field<U>(room, 0x68) + static_cast<U>(i) * 4u));
        if (entry != nullptr) delete_object(entry);                     // 00452AA6..00452AAE
    }
    free_if(field<U>(room, 0x68));                                      // 00452AB8..00452AC6
    free_if(field<U>(room, 0x5C));                                      // 00452ACE..00452ADB
    bsp_node_destruct_004aced0(room, nullptr);                          // 00452AE3..00452AED
}

// 004A6570 SafePointerList<WindManager>::~SafePointerList: thiscall RET 0 (EH frame 007180F8). Each registered
// SafePointer is unhooked from its target's list (WindManager+0 data / +4 count) and cleared, last to first.
void __fastcall wind_safe_pointer_list_destruct_004a6570(void* list, void*) {
    for (I k = field<I>(list, 0x04) - 1; k >= 0; --k) {                 // 004A658E..004A65ED
        const U safe = word_at(field<U>(list, 0x00) + static_cast<U>(k) * 4u);   // 004A65A0..004A65A2
        const U target = word_at(safe);                                 // 004A65A5
        if (target != 0u) unregister(ptr<void>(target), 0x00u, 0x04u, safe);   // 004A65A7..004A65E4
        word_at(safe) = 0u;                                             // 004A65E7
    }
    const U data = field<U>(list, 0x00);                                // 004A65F1
    if (data != 0u) { live_free(ptr<void>(data)); field<U>(list, 0x00) = 0u; }   // 004A65F5..004A6600
    field<U>(list, 0x08) = 0u; field<U>(list, 0x04) = 0u;               // 004A660B..004A6612
    free_if(field<U>(list, 0x00));                                      // 004A6606 / 004A6622..004A6625 member dtor
}

// 004A6710 WindManager::~WindManager: thiscall RET 0 (EH frame 00718143).
void __fastcall wind_manager_destruct_004a6710(void* wind, void*) {
    free_if(field<U>(wind, 0x84C));                                     // 004A672D..004A6740
    const U sources = field<U>(wind, 0x0C);                             // 004A6748
    if (sources != 0u) {                                                // 004A674B..004A6752 delete[] (18h records)
        c2da_destroy_array_006fa852(ptr<void>(sources), 0x18u, field<I>(ptr<void>(sources - 4u), 0),
                                    &scene_void_return_004015a0);       // 004A6754..004A6764 (element dtor 004015A0)
        live_free(ptr<void>(sources - 4u));                             // 004A6769..004A676A
    }
    wind_safe_pointer_list_destruct_004a6570(wind, nullptr);            // 004A6773..004A677D
}

// 00457820 CAurScene::~CAurScene (ECX = scene+4): thiscall RET 0 (EH frame 00716778). SafePointerList<CAurObject>
// teardown: targets keep their registrations at +4 (data) / +8 (count).
void __fastcall aur_scene_destruct_00457820(void* list, void*) {
    for (I k = field<I>(list, 0x04) - 1; k >= 0; --k) {                 // 0045783E..0045789E
        const U safe = word_at(field<U>(list, 0x00) + static_cast<U>(k) * 4u);   // 00457850..00457852
        const U target = word_at(safe);                                 // 00457855
        if (target != 0u) unregister(ptr<void>(target), 0x04u, 0x08u, safe);   // 00457857..00457895
        word_at(safe) = 0u;                                             // 00457898
    }
    const U data = field<U>(list, 0x00);                                // 004578A2
    if (data != 0u) { live_free(ptr<void>(data)); field<U>(list, 0x00) = 0u; }   // 004578A4..004578B1
    field<U>(list, 0x08) = 0u; field<U>(list, 0x04) = 0u;               // 004578BC..004578C3
    free_if(field<U>(list, 0x00));                                      // 004578B7 / 004578D3..004578D6
}

// 004A2BC0 LightManager::~LightManager: thiscall RET 0 (EH frame 00717F6E states 11h..0). Restores the class table
// 00743958, then frees the storage of the nineteen member lists in reverse member order.
void __fastcall light_manager_destruct_004a2bc0(void* lights, void*) {
    field<const void*>(lights, 0x00) = &kotor_scene3d_construct::light_manager_vtable_00743958;   // 004A2BDD
    static const U kLists[] = {0xE8u, 0xDCu, 0xD0u, 0xC4u, 0xB8u, 0xACu, 0x98u, 0x8Cu, 0x80u, 0x74u,
                               0x68u, 0x5Cu, 0x50u, 0x44u, 0x38u, 0x2Cu, 0x20u, 0x14u, 0x08u};   // 004A2BE3..004A2D8B
    for (U offset : kLists) free_if(field<U>(lights, offset));
}

// 004A8350 CAurTriangleBin::~CAurTriangleBin: thiscall RET 0 (EH frame 007182CC states 3..0).
void __fastcall triangle_bin_destruct_004a8350(void* bin, void*) {
    if ((field<std::uint8_t>(bin, 0x02) & 1u) != 0u) {                  // 004A836F..004A837E arrays built
        live_free(field<void*>(bin, 0x3C));                             // 004A8380..004A8384
        field<U>(bin, 0x3C) = 0u;                                       // 004A838D
        live_free(field<void*>(bin, 0x38));                             // 004A8389..004A8390
        field<std::uint8_t>(bin, 0x02) = static_cast<std::uint8_t>(field<std::uint8_t>(bin, 0x02) ^ 1u);   // 004A8395..004A83A6
        field<U>(bin, 0x38) = 0u; field<U>(bin, 0x40) = 0u; field<U>(bin, 0x44) = 0u;   // 004A839D..004A83A3
    }
    const U safe = word(at(bin, 0x54));                                 // 004A83A9 lea edi,[esi+54h]
    void* texture = field<void*>(bin, 0x54);                            // 004A83AD ebp
    if (texture != nullptr) texture_list_remove_00445080(&field<PointerList>(texture, 4), nullptr, safe);   // 004A83B1..004A83B9
    field<U>(bin, 0x54) = 0u;                                           // 004A83BF
    texture_release_00420d70(texture);                                  // 004A83BE..004A83C1 (also for a null texture)
    const U textures = field<U>(bin, 0x64);                             // 004A83C6
    if (textures != 0u) {                                               // 004A83CC..004A83D3 delete[] SafePointer<CAurTexture>
        c2da_destroy_array_006fa852(ptr<void>(textures), 4u, field<I>(ptr<void>(textures - 4u), 0),
                                    &aurora_texture_safe_pointer_destroy_00422380);   // 004A83D5..004A83E4
        live_free(ptr<void>(textures - 4u));                            // 004A83E9..004A83EA
    }
    free_if(field<U>(bin, 0x58));                                       // 004A83F2..004A8400
    void* member = field<void*>(bin, 0x54);                             // 004A8408 SafePointer member dtor
    if (member != nullptr) texture_list_remove_00445080(&field<PointerList>(member, 4), nullptr, safe);   // 004A840A..004A8412
    const U triangles = field<U>(bin, 0x48);                            // 004A8417
    if (triangles != 0u) {                                              // 004A841A..004A8424 delete[] CAurTriangleListElem
        c2da_destroy_array_006fa852(ptr<void>(triangles), 0x18u, field<I>(ptr<void>(triangles - 4u), 0),
                                    &triangle_list_elem_destruct_004a72d0);   // 004A8426..004A8435
        live_free(ptr<void>(triangles - 4u));                           // 004A843A..004A843B
    }
}

// 0046B1E0 nodedata::~nodedata: thiscall RET 0 (EH frame 00716D67 states 0Bh..0).
void __fastcall node_data_destruct_0046b1e0(void* data, void*) {
    void* tile = field<void*>(data, 0x70);                              // 0046B1FE
    if (tile != nullptr) { tile_data_destruct_00444e20(tile, nullptr); live_free(tile); }   // 0046B20D..0046B215
    static const U kLists[] = {0x8Cu, 0x80u, 0x74u, 0x60u, 0x54u, 0x48u, 0x3Cu, 0x30u, 0x24u, 0x18u, 0x0Cu, 0x00u};   // 0046B21D..0046B319
    for (U offset : kLists) free_if(field<U>(data, offset));
}

// 0046C850 DeleteNonGobParts (BSPTraverse callback, cdecl(node, context), plain RET 0046CA2A). Deletes, last to first,
// the parts of the node data lists whose owner word +40h is 0; for the four object lists the model reference of
// [[part+4]+8] is released first (0043E1C0 / 0043E210); then the triangle bins, then the node data itself.
// Deviation (labelled): for lists +30 and +3C the original loads [entry+40h] before its null test (0046C867 /
// 0046C897) - a null entry would fault there; the C++ tests the entry first (identical for every non-null entry).
void __cdecl delete_non_gob_parts_0046c850(void* node, void*) {
    void* data = field<void*>(node, 0x58);                              // 0046C855
    static const U kPartLists[2][2] = {{0x30u, 0x34u}, {0x3Cu, 0x40u}};
    for (const auto& l : kPartLists) {                                  // 0046C859..0046C882 / 0046C884..0046C8B2
        for (I k = field<I>(data, l[1]); k != 0;) {
            void* part = ptr<void>(word_at(field<U>(data, l[0]) + static_cast<U>(k - 1) * 4u));
            --k;
            if (part != nullptr && field<U>(part, 0x40) == 0u) delete_object(part);
            field<I>(data, l[1]) = field<I>(data, l[1]) - 1;
        }
    }
    static const U kObjectLists[4][2] = {{0x00u, 0x04u}, {0x18u, 0x1Cu}, {0x24u, 0x28u}, {0x48u, 0x4Cu}};
    for (const auto& l : kObjectLists) {                                // 0046C8B4..0046C9DF
        for (I k = field<I>(data, l[1]); k != 0;) {
            void* object = ptr<void>(word_at(field<U>(data, l[0]) + static_cast<U>(k - 1) * 4u));
            --k;
            if (field<U>(object, 0x40) == 0u) {
                aurora_model::MaxTree* model = aurora_tree_as_model_0043e1c0(
                    field<aurora_model::MaxTree*>(field<void*>(object, 0x04), 0x08), nullptr);   // 0046C8CE..0046C8D4
                if (model != nullptr) aurora_model_del_ref_0043e210(model, nullptr, model);   // 0046C8D9..0046C8E0
                void* again = ptr<void>(word_at(field<U>(data, l[0]) + static_cast<U>(k) * 4u));   // 0046C8E5..0046C8E7
                if (again != nullptr) delete_object(again);             // 0046C8EA..0046C8F2
            }
            field<I>(data, l[1]) = field<I>(data, l[1]) - 1;
        }
    }
    for (I k = field<I>(data, 0x64); k != 0;) {                         // 0046C9E1..0046CA15
        void* bin = ptr<void>(word_at(field<U>(data, 0x60) + static_cast<U>(k - 1) * 4u));
        --k;
        if (bin != nullptr) { triangle_bin_destruct_004a8350(bin, nullptr); live_free(bin); }   // 0046C9F8..0046CA04
        field<I>(data, 0x64) = field<I>(data, 0x64) - 1;
    }
    node_data_destruct_0046b1e0(data, nullptr);                         // 0046CA18..0046CA1A
    live_free(data);                                                    // 0046CA1F..0046CA20
}

// ================================================================ 00458BE0 Scene::~Scene (body)
// thiscall RET 0 (00458E6C). EH frame 00716917 (states 7..0) cited; no callee throws on these paths.
void __fastcall scene_destruct_00458be0(void* scene, void*) {
    field<const void*>(scene, 0x00) = &scene_vtable_00741708;           // 00458BFD
    while (field<I>(scene, 0x58) > 0)                                   // 00458C03..00458C28 (count re-read)
        gob_set_scene_0043f200(ptr<void>(word_at(field<U>(scene, 0x54) + static_cast<U>(field<I>(scene, 0x58) - 1) * 4u)),
                               nullptr, nullptr);                       // 00458C12..00458C1E Gob::SetScene(0)
    gob_set_scene_0043f200(at(scene, 0xF8), nullptr, nullptr);          // 00458C2C..00458C34
    texture_list_remove_00445080(cells.scene_list_007b9398, nullptr, word(scene));   // 00458C39..00458C3F
    void* lights = field<void*>(scene, 0x80);                           // 00458C44
    if (lights != nullptr) { light_manager_destruct_004a2bc0(lights, nullptr); live_free(lights); }   // 00458C4E..00458C56
    void* bsp = field<void*>(scene, 0xA0);                              // 00458C5E
    if (bsp != nullptr) { bsp_node_destruct_004aced0(bsp, nullptr); live_free(bsp); }   // 00458C68..00458C70
    aur_clear_active_bins_0046a610();                                   // 00458C78
    if (field<I>(scene, 0xCC) > 0) {                                    // 00458C7D..00458C85
        do {
            const I count = field<I>(scene, 0xCC);                      // 00458C90
            bsp_traverse_004acea0(ptr<void>(word_at(field<U>(scene, 0xC8) + static_cast<U>(count - 1) * 4u)),
                                  &delete_non_gob_parts_0046c850, nullptr);   // 00458C96..00458CA8
            void* room = ptr<void>(word_at(field<U>(scene, 0xC8) + static_cast<U>(field<I>(scene, 0xCC) - 1) * 4u));   // 00458CAD..00458CB9
            if (room != nullptr) { aur_room_destruct_00452a60(room, nullptr); live_free(room); }   // 00458CC0..00458CCC
            field<I>(scene, 0xCC) = field<I>(scene, 0xCC) - 1;          // 00458CD4..00458CDF
        } while (field<I>(scene, 0xCC) > 0);                            // 00458CE5
    }
    void* wind = field<void*>(scene, 0x84);                             // 00458CE7
    if (wind != nullptr) { wind_manager_destruct_004a6710(wind, nullptr); live_free(wind); }   // 00458CF1..00458CF9
    const U safe = word(at(scene, 0xA4));                               // 00458D09 lea ebp,[esi+0A4h]
    void* texture = field<void*>(scene, 0xA4);                          // 00458D01
    if (texture != nullptr) {                                           // 00458D07..00458D0F SafePointer::operator=(0)
        texture_list_remove_00445080(&field<PointerList>(texture, 4), nullptr, safe);   // 00458D17..00458D1B
        field<U>(scene, 0xA4) = 0u;                                     // 00458D21
        texture_release_00420d70(texture);                              // 00458D20..00458D28
    }
    void* noise = field<void*>(scene, 0xE4);                            // 00458D30
    if (noise != nullptr) { field<U>(scene, 0xE4) = 0u; texture_release_00420d70(noise); }   // 00458D3A..00458D45
    void* distortion = field<void*>(scene, 0xE8);                       // 00458D4D
    if (distortion != nullptr) { field<U>(scene, 0xE8) = 0u; texture_release_00420d70(distortion); }   // 00458D57..00458D62
    for (I k = 0; k < 5000; ++k) cells.mesh_buckets_008194e0[k].size = 0;   // 00458D6A..00458D7E ([8194E4 + 0Ch*k] = 0, k < 5000)
    gob_destruct_0044b2f0(static_cast<aurora_model::Gob*>(at(scene, 0xF8)), nullptr);   // 00458D80..00458D8B
    free_if(field<U>(scene, 0xEC));                                     // 00458D90..00458DA0
    free_if(field<U>(scene, 0xC8));                                     // 00458DA8..00458DB8
    free_if(field<U>(scene, 0xBC));                                     // 00458DC0..00458DD0
    const U target = field<U>(scene, 0xA4);                             // 00458DD8 SafePointer member dtor (inline)
    if (target != 0u) unregister(ptr<void>(target), 0x04u, 0x08u, safe);   // 00458DDB..00458E16
    free_if(field<U>(scene, 0x94));                                     // 00458E18..00458E2A
    free_if(field<U>(scene, 0x54));                                     // 00458E32..00458E3F
    { const m47_aur_scene::AurSceneVTable* own = &aur_scene_vtable_00741610;   // 00458E52 CAurScene table (typed, rev2)
      std::memcpy(scene, &own, sizeof own); }
    aur_scene_destruct_00457820(at(scene, 0x04), nullptr);              // 00458E47..00458E58
}

