#pragma once
// lane-m46-3dcore fork B: shared view header for the Gob vtable 00741268 slot bodies (m46_gob_*.cpp).
// Include order: scene3d.h (canonical, lane-3d-render) + lane-3d-model headers + lane-3d-construct behavior slots,
// then the lowers below that have NO declaration in the M45 stage (new undefined externals, exact original ABI
// from the callee's own `ret N` / caller `add esp`; cited per line).
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "6c81d8874d-scene3d.h"
#include "7ee14b5ec3-model_vtables.hpp"     // lane-3d-model aurora_vt::VTable_00740ae8 (Part base table type), path_info table
#include "5d11c14380-model_load.hpp"        // aurora_find_model_00464110, aurres_* (lane-aurres), MaxTree delete, no-delete cell
#include "a7e200e217-model_instance.hpp"    // aurora_gobs_007a8a58, aurora_gob_count_007a8a18, CreateInstanceTreeR, PartInitMaterial
#include "516b075ab1-construct3d.h"         // lane-3d-construct BehaviorBaseSlots (CAurBehavior tables 00743864..)
#include "7a7bf89a27-m47_aurora_object_vtable.h"   // lane-m47-3d: typed CAurObject table 00741108 (D21; definition m47_gob2.cpp)

namespace m46_gob {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::U8; using kotor_scene3d::U16;
using kotor_scene3d::Vector; using kotor_scene3d::Quaternion; using kotor_scene3d::PointerList;
using kotor_scene3d::field; using kotor_scene3d::at;
using kotor_scene3d::bits_to_float; using kotor_scene3d::float_to_bits;
using PartTable = aurora_vt::VTable_00740ae8;   // Part base table type; the slots used here (+00 +04 +0C +1C +2C +38
                                                // +64 +6C +78) have the same SlotN type in every Part-family table
                                                // listed in model_vtables.hpp (generated from the pinned image).
using BehaviorTable = kotor_scene3d_construct::BehaviorBaseSlots;

// CAurTexture table view (offsets/types identical to lane-texture-epoch texture_vtable.hpp CommonSlots; local view to
// avoid that include chain - same precedent as lane-3d-anim anim3d.h TextureSlots / lane-3d-bsp triangle_bin.h).
struct TextureSlots {
    const void* s00[0x10 / 4];
    U8 (__fastcall* is_invalid)(void*, void*);                    // +10 texture_is_invalid_004205d0 (AL; 004486F5)
    const void* s14[(0x34 - 0x14) / 4];
    U8 (__fastcall* is_txi_loaded)(void*, void*);                 // +34 texture_is_txi_loaded_0041ec50 (AL; 004481B2)
    const void* s38[(0x90 - 0x38) / 4];
    U (__fastcall* get_txi_size)(void*, void*);                   // +90 texture_get_txi_size_0041ed20 (004482EB)
    void* (__fastcall* get_txi_data)(void*, void*);               // +94 texture_get_txi_data_0041ec90 (004482DF)
    const void* s98[(0xA0 - 0x98) / 4];
    U (__fastcall* set_txi_data)(void*, void*, void*, I);         // +A0 texture_set_txi_data_0041ecb0 (RET 8; 00448211)
    U (__fastcall* set_txi_loaded)(void*, void*, U);              // +A4 texture_set_txi_loaded_0041ec70 (RET 4; 004481DC)
};
static_assert(sizeof(void*) == 4 && offsetof(TextureSlots, set_txi_loaded) == 0xA4, "CAurTexture +A4");
// Objects at Gob+174 / Gob+178: only their deleting destructor (+00, `push 1; call [vtbl]`, 0044B527 / 0044B53D) is
// used; no published table exists for their class -> minimal view of that one slot.
struct DeletingSlot { void* (__fastcall* scalar_delete)(void*, void*, U); };

template <class T> inline const T* table(void* object) { return *static_cast<const T* const volatile*>(object); }
inline const aurora_model::GobVTable* gob_vt(void* gob) { return table<aurora_model::GobVTable>(gob); }
inline const kotor_scene3d::SceneVTable* scene_vt(void* scene) { return table<kotor_scene3d::SceneVTable>(scene); }
inline aurora_model::Gob* as_gob(void* gob) { return static_cast<aurora_model::Gob*>(gob); }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
template <class T> inline T* ptr(U w) { return reinterpret_cast<T*>(static_cast<std::uintptr_t>(w)); }
// D30-FINAL: W = x87 stack value (double), S = original fstp dword rounding.
inline double W(float v) { return static_cast<double>(v); }
inline float S(double v) { return static_cast<float>(v); }
}

extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::Vector; using kotor_scene3d::PointerList;
// ---- existing M45 providers whose declaring header is not on this unit's include path (identical declarations)
char* __cdecl resource_next_0044bfa0();                      // lane-aurres AurResGetNextLine (aurres.hpp / font_txi.hpp)
char* __cdecl firstword_00463530(char*);                      // font-txi-readers (font_txi.hpp)
void __cdecl parse_cstr_00463590(char*, char*);               // font-txi-readers (font_txi.hpp)
void __cdecl texture_release_00420d70(void* texture);         // AurTextureRelease: lane-window font_release.cpp spelling
// ---- new undefined externals (no provider in M45; owner noted)
I __cdecl sphere_intersect_004abdd0(const Vector* start, const Vector* end, const Vector* center, U radius_bits); // 0043ECCD add esp,1Ch (with 3 pushes before)
U __cdecl gl_render_set_up_imposters_00432160(U a, U b);       // 004404C0 add esp,8; EAX -> Gob+30
void __cdecl part_set_fog_00440550(void* part, U enable);      // 00440876 (push 0/1 dword; callee reads the byte)
U __cdecl part_set_can_down_sample_004434a0(void* part, U flag);   // 0044380C (EAX returned by 004437F0)
void __cdecl find_all_light_parts_00445ae0(void* part, PointerList* out);   // 00445E64
void __cdecl find_all_materials_00445ec0(void* part, PointerList* out);     // 00446563
void __cdecl find_all_tri_mesh_parts_004461d0(void* part, PointerList* out);   // 00446695
void __cdecl get_all_named_textures_00446ab0(void* part, PointerList* out, U name);   // 00446E49
U __cdecl aur_texture_set_attributes_00422ce0(void* texture, U attributes);   // 00446E6E (EAX 0/1; lane-m47-3d m47_gob2_tex.cpp)
U __stdcall enum_parts_004474d0(void* part, U (__cdecl* callback)(void* part), U out);   // D75: typed PartEnumerator (RET 0Ch unchanged)   // RET 0Ch (004477A5); 004477BE
U __cdecl part_has_material_0043ee40(void* part);              // used only as a callback address (00448174 / 00448604)
void __fastcall material_replace_texture_0047b890(void* material, void*, const char* a0, const char* a1);   // RET 8
void __fastcall material_replace_texture_0047b7e0(void* material, void*, const char* a0, const char* a1, U a2,
    const U* a3, I a4);                                         // RET 14h
void __fastcall exo_array_list_remove_00445080(PointerList* list, void*, void* value);   // identical to lane-3d-bsp bsp.h (M45 join -> texture_list_remove_00445080)
void __cdecl scene_remove_from_scene_manager_00455540(void* gob);   // 0044B513
void __fastcall safe_pointer_list_destruct_004487c0(void* list, void*);   // SafePointerList<CAurObject>::~ (0044B64C, ECX=gob+4)
kotor_scene3d::Quaternion* __cdecl quaternion_normalize_004a9d60(kotor_scene3d::Quaternion* out, kotor_scene3d::Quaternion q);   // 0043F3F5 add esp,14h
// ---- original cells referenced ONLY by the Gob slot bodies (image-wide byte search: 0078E338 -> 0043F0AC only;
//      007A8A3C -> 0044673C/00446786/004467A0/0044691F/00446969/00446983 only). Storage: m46_gob_tree.cpp.
extern volatile float gob_sphere_scale_0078e338;               // .data 3FACCCCD (1.35f)
extern std::FILE* volatile aurora_texture_log_007a8a3c;       // BSS; no writer in the image (always null)
}
namespace font { struct Texture; }
namespace texture_join { void __fastcall set_environment_00421480(font::Texture*, void*, std::uint32_t); }   // M45 texture-epoch-cleanup-slots
