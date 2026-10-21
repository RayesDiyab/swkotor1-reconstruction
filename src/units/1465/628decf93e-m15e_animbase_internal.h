#pragma once
// lane-m15-e (L15-E) fork A: internal contract of the CSWCAnimBase family units (m15e_animbase*.cpp).
// Original: swkotor.exe GOG K1 (image base 0x400000); static disassembly only (work/hw.asm, head.asm, tw.asm,
// ab1.asm, ab2.asm, ab3.asm, wield.asm).
//
// Object model (MSVC virtual inheritance, reproduced as raw byte views; all offsets from the original code):
//  CSWCAnimBaseHeadWield (0x220, ctor 00698EC0): +000 vbptr (00754BF0 = {0, 8, 17Ch, 1C8h}), +004 vtordisp of the
//   virtual base, +008 CSWCAnimBaseTW virtual base (0x174, vfptr first), +17C CSWCAnimBaseHead (vbptr 00754BE8 =
//   {0, -174h}), +1C8 CSWCAnimBaseWield (vbptr 00754BE0 = {0, -1C0h}). [creature+68h] = the TW subobject.
//  "this" conventions of the original methods (each documented per function):
//   - CSWCAnimBase / CSWCAnimBaseTW methods: this = the TW/AnimBase object (vfptr at +0);
//   - CSWCAnimBaseHeadWield methods: this = TW subobject (obj+8), vbptr at [this-8];
//   - CSWCAnimBaseHead methods: this = head+50h, vbptr at [this-50h];
//   - CSWCAnimBaseWield methods: this = wield+5Ch, vbptr at [this-5Ch] (ReAttachWieldedObjects: this = wield).
//  NV thunks (vtable entries of the virtual base's tables) do "sub ecx,[ecx-4]" (vtordisp) then jmp.
// Field map (CSWCAnimBase, this-relative): +04..+10 CSWAnimBase::Set values; +14 (20 bytes) texture attribute
//  block; +28/+29 bytes; +2C CResRef texture; +3C flag; +40 CResRef env map; +50 flag; +54 5x Vector hilite
//  colours; +90 5 hilite flags; +A4 5 flags; +B8 body CAurObject*; +BC shadow blob CAurObject*; +C0 alpha;
//  +C4 model type byte; +C5/+C6/+C7 bytes; +C8 byte; +CC; +D0; +D4 shadow shown; +D8; +DC; +E0 Quaternion.
// TW adds: +F0 (20 bytes) / +104 / +105, +106 (20 bytes) / +11A / +11B, +11C CResRef, +12C, +130 CResRef, +140,
//  +144 CResRef, +154, +158 CResRef, +168, +16C tail CAurObject*, +170 wings CAurObject*.
// Head (head-base relative): +04 (20 bytes) / +18 / +19, +1C CResRef, +2C, +30 CResRef env, +40, +44 head
//  CAurObject*, +48 head item id. Wield (wield-base relative): +04/+14 CResRef, +24/+2C CExoString hook names,
//  +34/+38 dummy CAurObject*, +3C right item id, +40 left item id, +44 wield limited, +48 hidden, +4C, +50,
//  +54 CExoSoundSource*.
#include <cstddef>
#include <cstdio>
#include <initializer_list>
#include <new>
#include <cstdint>
#include <cstring>
#include "bd9f6ce7e8-m15e_animbase.h"
#include "fe0daa935e-scene3d.h"            // aurora_model::GobVTable (lane-3d-render, D42), kotor_scene3d::Vector/Quaternion
#include "0996de89b6-startup_string.h"     // kotor_reconstruction::StartupString (CExoString 005B3190/005E5A90/005E5C20 ...)
#include "e880d614d4-resources.h"          // menu_resources::Ref (CResRef 16 bytes) + construct_00406d80
#include "f6553584c7-allocator_live.h"     // live_free (006FA390)
#include "c272944ff0-kotor_unreachable_trap.h"
#include "21be0cd701-m15_l0.h"             // L15-0: GetItemByGameObjectID 005ED5A0, CExoSoundSource Play/Stop

namespace c2da_current {struct Table;}

// ---------------------------------------------------------------------------------- existing owners (M60)
extern "C" {
void __fastcall gui_load_resref_copy_to_string_00405fb0(const menu_resources::Ref* ref, void* edx, char* out);            // 00405FB0 RET 4
void __fastcall c2da_ref_to_string_00405f70(menu_resources::Ref*, void*, menu_resources::String*);         // 00405F70 RET 4
menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*, void*);                       // 00405ED0 CResRef()
menu_resources::Ref* __fastcall c2da_copy_00406120(menu_resources::Ref*, void*, menu_resources::Ref*, menu_resources::Ref*);   // 00406120 operator=(CResRef) (D48)
menu_resources::Ref* __fastcall unresolved_ref_assign_00406290(menu_resources::Ref*, void*, menu_resources::Ref*, const char*); // 00406290
std::uint32_t __fastcall gui_load_resref_not_equal_00406dc0(const menu_resources::Ref*, void*, const menu_resources::Ref*); // 00406DC0 RET 4
std::int32_t __fastcall c2da_current_get_string_entry_00413190(c2da_current::Table* table, void* unused_edx, std::int32_t row,
    std::int32_t column, kotor_reconstruction::StartupString* out);                                       // 00413190 RET 0Ch
aurora_model::Gob* __cdecl aurora_new_aur_object_00449cc0(const char* name, const char* gob_name, std::FILE* mdl, std::FILE* mdx);
kotor_scene3d::Quaternion* __cdecl yaw_pitch_roll_004acac0(kotor_scene3d::Quaternion* out, float yaw, float pitch, float roll);
kotor_scene3d::Quaternion* __fastcall quaternion_construct_004ac960(kotor_scene3d::Quaternion* self, void* unused_edx,
    kotor_scene3d::Vector axis, float angle);                                                             // 004AC960 RET 10h
char* __cdecl c2da_current_strlwr_007135df(char*);                                                        // 007135DF _strlwr
int __cdecl format_006fadb0(char*, const char*, ...);                                                     // 006FADB0 sprintf
kotor_reconstruction::StartupString* __fastcall GuiAssign005e5140(kotor_reconstruction::StartupString*, void*, const char*); // 005E5140
char* __fastcall WindowCStr005e5670(kotor_reconstruction::StartupString*, void*);                         // 005E5670
kotor_reconstruction::StartupString* __fastcall WindowCopy005e5b00(kotor_reconstruction::StartupString*, void*,
    const kotor_reconstruction::StartupString* volatile);                                                 // 005E5B00 (D9)
void* __fastcall kotor_fastcall_005ed530(void* app, void* unused_edx);                                    // 005ED530 GetModule
std::uint32_t __fastcall frame_R4_005ed5b0(void* client, void* unused_edx, std::uint32_t id);             // 005ED5B0
void* __fastcall kotor_m15e_slot00c_self_00641db0(void* self, void* unused_edx);                               // 00641DB0 self_return
float __cdecl kotor_m15a_yaw_004aa0f0(const void* vector);
// L15-B (lane-m15-b/src/m15b_stats.h, verbatim): 005B4790 CSWItem::GetBaseItem() RET 0.
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* self, void*);                                                // L15-A 004AA0F0 Yaw
}
namespace kotor_reconstruction {
void __cdecl StartupFormat005e5680(StartupString*, const char*, ...);                                     // 005E5680 Format
}
namespace kotor_sound_path {
unsigned __fastcall StringIsEmpty005e58a0(kotor_reconstruction::StartupString*, void*);                   // 005E58A0
}
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString*, void*,
    kotor_reconstruction::StartupString*, const kotor_reconstruction::StartupString*);                    // 005E5D10
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }                             // [007A39FC]
namespace kotor_world_los { extern kotor_scene3d::Vector up_007a19b0; }                                   // [007A19B0]
extern "C" {
extern void* kotor_cell_rules_007a3a28;                    // [007A3A28] CSWRules* (lane-prepare)
extern std::uint32_t kotor_rules_cell32_007a2174;          // [007A2174] animations.2da "name" column index (lane-rules)
}
namespace kotor_input_hover {                              // input-hover/hover_path.h (verbatim): 005D5930 owner m48b-gui-input
struct SoundSource {
    unsigned char opaque00[4];
    void* internal;                                        // +0x04 (005D5930)
    void __thiscall Play();                                // 005D5930
};
}

namespace kotor_m15e_ab {
using U = std::uint32_t;
using I = std::int32_t;
using U8 = std::uint8_t;
using kotor_reconstruction::StartupString;
using menu_resources::Ref;
using kotor_scene3d::Vector;
using kotor_scene3d::Quaternion;

template <class T> inline T ld(const void* base, std::ptrdiff_t off) noexcept {
    T v; std::memcpy(&v, static_cast<const unsigned char*>(base) + off, sizeof(T)); return v;
}
template <class T> inline void st(void* base, std::ptrdiff_t off, T v) noexcept {
    std::memcpy(static_cast<unsigned char*>(base) + off, &v, sizeof(T));
}
inline unsigned char* at(void* base, std::ptrdiff_t off) noexcept { return static_cast<unsigned char*>(base) + off; }
inline void* pv(const void* base, std::ptrdiff_t off) noexcept { return ld<void*>(base, off); }
// Entry k of the vbtable whose pointer is stored at [base+vbptr_off] (two loads, as the original: mov r,[vbptr]; mov r,[r+4k]).
inline I vbe(const void* base, std::ptrdiff_t vbptr_off, int k) noexcept { return ld<const I*>(base, vbptr_off)[k]; }
inline U wd(const void* p) noexcept { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline U fbits(float f) noexcept { U u; std::memcpy(&u, &f, 4); return u; }
inline float bitsf(U u) noexcept { float f; std::memcpy(&f, &u, 4); return f; }
inline aurora_model::Gob* gob(void* p) noexcept { return static_cast<aurora_model::Gob*>(p); }
inline const aurora_model::GobVTable* gvt(const void* g) noexcept { return ld<const aurora_model::GobVTable*>(g, 0); }
inline const KotorM15eAnimBaseVtbl* avt(const void* a) noexcept { return ld<const KotorM15eAnimBaseVtbl*>(a, 0); }
inline void* client_app() noexcept { return pv(const_cast<void*>(kotor_menu_window_cells::manager_007a39fc), 4); }   // [[007A39FC]+4]
inline Ref* ref_at(void* base, std::ptrdiff_t off) noexcept { return reinterpret_cast<Ref*>(at(base, off)); }
inline StartupString* str_at(void* base, std::ptrdiff_t off) noexcept { return reinterpret_cast<StartupString*>(at(base, off)); }
// Generic first-slot view of an object whose table's slot 0 is a scalar deleting destructor (RET 4).
struct ScalarDtorView { void* (__fastcall* s000)(void* self, void* edx, U flags); };
inline void delete_object(void* o) noexcept(false) { ld<const ScalarDtorView*>(o, 0)->s000(o, nullptr, 1); }
// 00406290 CResRef::operator=(char const*) into a hidden-return temporary (trivial 16-byte storage, D9).
inline void ref_assign_text(Ref* target, const char* text) noexcept(false) { Ref tmp; unresolved_ref_assign_00406290(target, nullptr, &tmp, text); }
// Original .rdata literals.
constexpr char kEmpty_0073d71c[] = "";
constexpr float kZero_0073d700 = 0.0f;
constexpr float kOne_0073d764 = 1.0f;
constexpr float kThousand_0073d6fc = 1000.0f;
}  // namespace kotor_m15e_ab

// --------------------------------------------------------------------------- family entry points (this lane)
extern "C" {
// tables (m15e_animbase_tables.cpp)
struct KotorM15eSwAnimBaseVtbl { void* (__fastcall* s000)(void* self, void* edx, kotor_m15e::U flags); };
extern const KotorM15eSwAnimBaseVtbl kotor_m15e_sw_anim_base_vtable_00754ae4;
extern const KotorM15eAnimBaseVtbl kotor_m15e_anim_base_head_wield_vtable_00754ae8;
extern const KotorM15eAnimBaseVtbl kotor_m15e_anim_base_wield_vtable_00754c08;
extern const KotorM15eAnimBaseVtbl kotor_m15e_anim_base_head_vtable_00754d48;
extern const KotorM15eAnimBaseVtbl kotor_m15e_anim_base_tw_vtable_00754e58;
extern const KotorM15eAnimBaseVtbl kotor_m15e_anim_base_vtable_00754f60;
extern const std::int32_t kotor_m15e_anim_base_vbtables_00754be0[8];   // 00754BE0 / 00754BE8 / 00754BF0 (overlapping)
extern const std::int32_t kotor_m15e_anim_base_wield_vbtable_00754d00[2];
extern const std::int32_t kotor_m15e_anim_base_head_vbtable_00754e40[2];

// CSWAnimBase (core unit)
void* __fastcall kotor_m15e_sw_anim_base_construct_00698e00(void* self, void*) noexcept(false);
void __fastcall kotor_m15e_sw_anim_base_destruct_00698e20(void* self, void*) noexcept(false);
// CSWCAnimBase (this = object)
kotor_m15e::U __fastcall kotor_m15e_anim_base_attach_to_scene_0069cd80(void* self, void*, void* scene) noexcept(false);
void* __fastcall kotor_m15e_anim_base_get_model_0069cda0(void* self, void*, kotor_m15e::U part) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_replace_texture_0069cdc0(void* self, void*, kotor_m15e::U from, kotor_m15e::U to, kotor_m15e::U keep) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_replace_texture_0069ce50(void* self, void*, kotor_m15e::U part, kotor_m15e::U from, kotor_m15e::U to, kotor_m15e::U keep) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_replace_texture_0069cef0(void* self, void*, kotor_m15e::U part, kotor_m15e::U from, kotor_m15e::U to,
    kotor_m15e::U count, kotor_m15e::U attrs, kotor_m15e::U keep, kotor_m15e::U first) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_restore_texture_0069d100(void* self, void*, kotor_m15e::U part) noexcept(false);
float __fastcall kotor_m15e_anim_base_get_animation_time_0069d180(void* self, void*, kotor_m15e::U animation) noexcept(false);
float __fastcall kotor_m15e_anim_base_get_animation_length_0069d250(void* self, void*, kotor_m15e::U animation) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_get_overlay_animation_playing_0069d320(void* self, void*, kotor_m15e::U animation) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_set_orientation_0069d5d0(void* self, void*, const float* vector3) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_set_orientation_0069d640(void* self, void*, const float* quaternion) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_set_position_0069d690(void* self, void*, const float* vector3) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_unload_model_0069d740(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_unload_model_0069d770(void* self, void*, kotor_m15e::U part) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_set_env_map_0069d830(void* self, void*, kotor_m15e::U part, kotor_m15e::U a2, kotor_m15e::U a3, void* resref) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_animation_exists_0069d910(void* self, void*, kotor_m15e::U animation) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_apply_shadow_blob_0069da00(void* self, void*, void* name_string) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_remove_shadow_blob_0069da70(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_show_shadow_blob_0069dac0(void* self, void*, kotor_m15e::U force) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_hide_shadow_blob_0069db10(void* self, void*) noexcept(false);
void __fastcall kotor_m15e_anim_base_set_shadow_blob_orientation_0069db50(void* self, void*, const float* quaternion) noexcept(false);
void __fastcall kotor_m15e_anim_base_turn_on_shadows_0069dc10(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_enable_distortion_0069de50(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_disable_distortion_0069de70(void* self, void*) noexcept(false);
void __fastcall kotor_m15e_anim_base_destruct_0069e140(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_load_model_0069e1f0(void* self, void*, void* resref, kotor_m15e::U part) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_set_overlay_animation_0069e420(void* self, void*, void* gob, kotor_m15e::U animation, kotor_m15e::U flag) noexcept(false);
void* __fastcall kotor_m15e_anim_base_get_animation_name_0069e620(void* self, void*, void* out_string, kotor_m15e::U animation) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_set_alpha_0069eb20(void* self, void*, kotor_m15e::U alpha, kotor_m15e::U part) noexcept(false);
void __fastcall kotor_m15e_anim_base_refresh_hilite_0069ec00(void* self, void*) noexcept(false);
std::uint16_t __fastcall kotor_m15e_anim_base_server_to_client_animation_0069f650(void* self, void*, kotor_m15e::U animation) noexcept(false);
// CSWCAnimBaseTW (this = object)
void* __fastcall kotor_m15e_anim_base_tw_get_model_0069bcb0(void* self, void*, kotor_m15e::U part) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_tw_load_model_0069bce0(void* self, void*, void* resref, kotor_m15e::U part) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_tw_replace_texture_0069be20(void* self, void*, kotor_m15e::U from, kotor_m15e::U to, kotor_m15e::U keep) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_tw_replace_texture_0069bf30(void* self, void*, kotor_m15e::U part, kotor_m15e::U from, kotor_m15e::U to, kotor_m15e::U keep) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_tw_replace_texture_0069c090(void* self, void*, kotor_m15e::U part, kotor_m15e::U from, kotor_m15e::U to,
    kotor_m15e::U count, kotor_m15e::U attrs, kotor_m15e::U keep, kotor_m15e::U first) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_tw_restore_texture_0069c4c0(void* self, void*, kotor_m15e::U part) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_tw_unload_model_0069c5d0(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_tw_unload_model_0069c640(void* self, void*, kotor_m15e::U part) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_tw_set_env_map_0069c790(void* self, void*, kotor_m15e::U part, kotor_m15e::U a2, kotor_m15e::U a3, void* resref) noexcept(false);
void __fastcall kotor_m15e_anim_base_tw_destruct_0069cd10(void* self, void*) noexcept(false);
// CSWCAnimBaseHead (this = head + 50h)
void* __fastcall kotor_m15e_anim_base_head_get_model_0069b060(void* self, void*, kotor_m15e::U part) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_load_model_0069b0a0(void* self, void*, void* resref, kotor_m15e::U part) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_replace_texture_0069b1c0(void* self, void*, kotor_m15e::U from, kotor_m15e::U to, kotor_m15e::U keep) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_replace_texture_0069b250(void* self, void*, kotor_m15e::U part, kotor_m15e::U from, kotor_m15e::U to, kotor_m15e::U keep) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_replace_texture_0069b310(void* self, void*, kotor_m15e::U part, kotor_m15e::U from, kotor_m15e::U to,
    kotor_m15e::U count, kotor_m15e::U attrs, kotor_m15e::U keep, kotor_m15e::U first) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_restore_texture_0069b570(void* self, void*, kotor_m15e::U part) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_unload_model_0069b610(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_unload_model_0069b650(void* self, void*, kotor_m15e::U part) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_set_env_map_0069b720(void* self, void*, kotor_m15e::U part, kotor_m15e::U a2, kotor_m15e::U a3, void* resref) noexcept(false);
void __fastcall kotor_m15e_anim_base_head_turn_on_shadows_0069b810(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_enable_distortion_0069b950(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_disable_distortion_0069b9c0(void* self, void*) noexcept(false);
void __fastcall kotor_m15e_anim_base_head_destruct_0069b030(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_thunk_unload_model_0069bb00(void* vbase, void*) noexcept(false);
// CSWCAnimBaseWield (this = wield + 5Ch unless noted)
void __fastcall kotor_m15e_anim_base_wield_set_wield_limited_00699fc0(void* self, void*, kotor_m15e::U limited) noexcept(false);
void __fastcall kotor_m15e_anim_base_wield_show_wielded_items_0069a010(void* self, void*) noexcept(false);
void __fastcall kotor_m15e_anim_base_wield_turn_on_shadows_0069a170(void* self, void*) noexcept(false);
void* __fastcall kotor_m15e_anim_base_wield_get_item_aur_object_0069a650(void* self, void*, kotor_m15e::U item_id, kotor_m15e::U part) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_wield_item_is_valid_0069a680(void* self, void*, kotor_m15e::U item_id) noexcept(false);
void __fastcall kotor_m15e_anim_base_wield_hide_wielded_items_0069a6b0(void* self, void*, void* right_ref, void* left_ref) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_wield_enable_distortion_0069a8f0(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_wield_disable_distortion_0069a980(void* self, void*) noexcept(false);
void* __fastcall kotor_m15e_anim_base_wield_get_model_0069aa90(void* self, void*, kotor_m15e::U part) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_wield_wield_0069ab80(void* self, void*, kotor_m15e::U item_id, void* hook_name, kotor_m15e::U hand, kotor_m15e::U scale) noexcept(false);
void __fastcall kotor_m15e_anim_base_wield_reattach_wielded_objects_0069af10(void* wield, void*) noexcept(false);   // this = wield base
kotor_m15e::U __fastcall kotor_m15e_anim_base_wield_load_model_0069afe0(void* self, void*, void* resref, kotor_m15e::U part) noexcept(false);
void __fastcall kotor_m15e_anim_base_wield_destruct_00699ed0(void* self, void*) noexcept(false);
// CSWCItem::GetEquippedBy 00646630 (RET 0): [item+160h] == 7F000000h -> 0, else GetCreatureByGameObjectID.
std::uint32_t __fastcall kotor_m15e_item_get_equipped_by_00646630(void* item, void*) noexcept(false);
}

