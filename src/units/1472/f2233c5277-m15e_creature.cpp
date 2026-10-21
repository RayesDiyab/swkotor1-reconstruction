// lane-m15-e (L15-E, Milestone 1.5) unit m15e-creature: the CSWCCreature / CSWCObject slot bodies of tables
// 0074F5F8 / 007516C8 that the character-generation scenes call (lane-m15-f SLOT-AUDIT.json, 22 needed slots,
// second-level audit in lane-m15-e PACKET.json) and their direct-call closure, except the animation family
// (SetAnimation / AddFireForgetAnimation: unit m15e-anim) and the CSWCAnimBase family (unit m15e-animbase).
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only; every statement cites the original
// instruction(s) it reproduces. D1 fastcall veneers and typed tables (m15e.h).
// Original faulting accesses remain original accesses; bounded tests isolate them in subprocesses.
//
// Fidelity notes:
// * Hidden-return CResRef / CExoString temporaries are C++ locals of the observed 16 / 8 byte layouts; string
//   destructors (005E5C20) run in the original order at scope exit.
// * Unwind (D30-FINAL): the SEH frames of 0060E2F0 / 0063CCC0 / 00647B80 / 00647E80 / 00678EF0 / 006790D0 /
//   00406300 only destroy CExoString / CSWPortrait locals; C++ locals reproduce that cleanup.
#include "881e2a634f-m15e_internal.h"
#include <intrin.h>
#include "99e00ff93c-m15b_stats.h"       // L15-B CSWCCreatureStats / CSWPortrait owners
#include "7555a32042-m15a_object.h"      // L15-A CSWCObject members
#include "477243cc93-m15a_creature.h"    // L15-A CSWCCreature members
#include "36a720fede-scene3d.h"          // aurora_model::GobVTable (lane-3d-render vtables_gen.h)
#include <cstring>            // _strnicmp

namespace c2da_current { struct Table; }
using namespace kotor_m15e;

// ---------------------------------------------------------------------------------------- existing owners
extern "C" {
void __fastcall kotor_fastcall_00614a30(void*, void*); // existing W4 ForceAppearanceUpdate
void __fastcall kotor_fastcall_0060e6c0(void*, void*); // existing W4 ReAttachCamera
// CResRef (M60 map; extern "C" D1 symbols, declarations as lane-m15-a m15a_appearance.cpp)
int __fastcall c2da_not_equal_text_00406060(Ref*, void*, const char*);                        // 00406060 operator!=(char*)
Ref* __fastcall c2da_copy_00406120(Ref*, void*, Ref* out, Ref* source);                       // 00406120 operator=(CResRef) RET 8
Ref* __fastcall async_ref_assign_string_004061f0(Ref*, void*, Ref* out, String* src);         // 004061F0 operator=(CExoString) RET 8
Ref* __fastcall async_ref_construct_string_00406d60(Ref*, void*, String*);                    // 00406D60 CResRef(CExoString)
std::uint32_t __fastcall gui_load_resref_not_equal_00406dc0(const Ref*, void*, const Ref*);   // 00406DC0 operator!=(CResRef)
int __fastcall c2da_equal_text_004060d0(Ref*, void*, const char*);                            // 004060D0 operator==(char*)
void __fastcall c2da_ref_to_string_00405f70(Ref*, void*, String*);                            // 00405F70 CopyToString
// CExoString (M60 map)
std::uint32_t __fastcall async_string_length_005e5790(String*, void*);                       // 005E5790 GetLength
String* __fastcall c2da_assign_text_005e5140(String*, void*, const char*);                    // 005E5140 operator=(char*)
String* __fastcall async_assign_string_005e5c50(String*, void*, String*);                     // 005E5C50 operator=(CExoString)
char* __fastcall async_string_cstr_005e5670(String*, void*);                                  // 005E5670 CStr
String* __fastcall lower_construct_005e6080(String*, void*, String* out);                     // 005E6080 LowerCase (hidden return)
// C2DA (lane-tlk / startup-misc)
std::int32_t __fastcall c2da_current_get_string_00413270(c2da_current::Table* table, void*, std::int32_t row, String* column,
                                                         String* out);                        // 00413270 RET 0Ch
std::int32_t __fastcall c2da_current_get_int_entry_00413510(c2da_current::Table* table, void*, std::int32_t row, std::int32_t column,
                                                            std::int32_t* out);               // 00413510 RET 0Ch
// geometry (lane-world-los geometry_core_handover / lane-3d-render)
kotor_scene3d::Vector* __cdecl normalize_004ab180(kotor_scene3d::Vector* out, const kotor_scene3d::Vector* v);
kotor_scene3d::Quaternion* __cdecl rotation_arc_004ab630(kotor_scene3d::Quaternion* out, kotor_scene3d::Vector from,
                                                         kotor_scene3d::Vector to);
// client app / lists (M60 map)
void* __fastcall kotor_fastcall_005ed530(void* client, void*);                                // 005ED530 CClientExoApp::GetModule
void* __fastcall kotor_fastcall_005ed700(void* client, void*);                                // 005ED700 CClientExoApp::GetClientOptions
std::uint32_t __fastcall unresolved_list_next_005e9b90(void* list, void*, void** position);   // 005E9B90 CExoLinkedListInternal::GetNext
void __fastcall gui_load_slot_ret4_0060e760(void*, void*, std::uint32_t);                     // 0060E760 void_return (RET 4)
void __fastcall kotor_m15e_slot090_void0_004015a0(void*, void*);                                     // 004015A0 void_return (RET)
void* __fastcall packed_file_get_async_file_00644760(void* self, void*);                      // 00644760 GetProperty0x18 ([ecx+18h])
void* __fastcall kotor_fastcall_00641980(void* module, void*);                                // 00641980 CSWCModule::GetMiniGame
// rules cells (rules_image_data.h; written by CTwoDimArrays::Load2DArrays_* column caches)
extern void* kotor_cell_rules_007a3a28;                                                        // [007A3A28] CSWRules*
extern std::uint32_t kotor_rules_cell32_007a2124;                                              // [007A2124] appearance 2DA column
extern std::uint32_t kotor_rules_cell32_007a2118;                                              // [007A2118] "perspace" column
extern std::uint32_t kotor_rules_cell32_007a20d0;                                              // [007A20D0] "creperspace" column
char* __fastcall c2da_ref_00405fe0(Ref*, void*);                                               // 00405FE0 CResRef::GetResRefStr
int __cdecl format_006fadb0(char*, const char*, ...);                                          // 006FADB0 sprintf
std::int32_t __cdecl kotor_stricmp_0070acaf(const char*, const char*);                         // 0070ACAF __stricmp
void __fastcall c2da_005e9c10(void* list, void*, void* position);                             // 005E9C10 CExoLinkedListInternal::Remove RET 4
void __fastcall kotor_fastcall_005d6200(void*, void*);                                         // 005D6200 ~CExoStreamingSoundSource
void __fastcall cswc_object_set_id_00616fa0(void* self, void*, std::uint32_t value);          // 00616FA0 CSWCObject::SetId (lane-3d-bsp)
// forward declarations (this unit)
void __cdecl kotor_m15e_queued_callback_function_0063e510(void* gob, void* event, aurora_model::Part* user);
void* __fastcall kotor_m15e_slot00c_self_00641db0(void* self, void*);                               // 00641DB0 self_return (gui-load gui_objects.cpp)
}
namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction { void __fastcall destroy_list_005e9ab0(kotor_list::Internal*, void*); }   // 005E9AB0 (ini-list-cleanup.obj)
namespace menu_tables_m41 { namespace cells { extern const unsigned int invalid_object_00746644; } }     // .rdata 00746644 = 7F000000h
#include "9d80e826f0-m50_vtables.h"      // kotor_vtable_cgameobject_007457a0 (lane-m50-vtables)
String* __fastcall GuiConcat005e5d10(String*, void*, String*, const String*);                 // 005E5D10 operator+ (hidden return)
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }                  // [007A39FC] AppManager
namespace sound_volume29_required { unsigned __fastcall predicate_006054e0(void*, void*, void*); }  // 006054E0 return_true_4 (sound-lists.obj)

// .bss cell 0083395C: read only by 0060B9DA (CSWCCreature::SetPosition) and 0063F35D (CSWCObject::SetPosition);
// no instruction of the image writes it (full-text scan of the original disassembly), so it keeps its loader
// value 0. Single owner: lane-m15-e (no M60 definition).
extern "C" volatile std::uint32_t kotor_m15e_cell_0083395c = 0;

namespace {
const char kEmpty_0073d71c[] = "";                                   // .rdata 0073D71C
void* client_app() noexcept { return rp(kotor_menu_window_cells::manager_007a39fc, 4); }   // [[007A39FC]+4]
void* rules() noexcept { return kotor_cell_rules_007a3a28; }        // [007A3A28]
void* anim_base(const void* object) noexcept { return rp(object, 0x68); }   // [obj+68h]
void* stats_of(const void* creature) noexcept { return rp(creature, 0x2F8); }   // [creature+2F8h] CSWCLevelUpStats*
const aurora_model::GobVTable* gob_methods(const void* gob) noexcept {
    return *static_cast<const aurora_model::GobVTable* const*>(gob);
}
aurora_model::Gob* as_gob(void* gob) noexcept { return static_cast<aurora_model::Gob*>(gob); }
// Raw-word arguments of the lane-3d-render Gob table (vtables_gen.h raw-word contract): object pointer -> word.
U word_of(const void* p) noexcept { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
void* word_ptr(U w) noexcept { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(w)); }
// Slot +4Ch view of the item / monk-visual models freed by ~CSWCCreature (0061724Ch / 0061727B: [vtbl+4Ch](0)).
struct Slot4cView { const void* s000_048[0x13]; void (__fastcall* s04c)(void*, void*, U); };
static_assert(sizeof(void*) * 0x13 == 0x4C, "+4Ch");
// byte [00746634] (.rdata dword 08070605h): external-data type of a client creature, 5.
constexpr U kotor_m15e_external_data_type_00746634 = 5;
}  // namespace

// Existing selected provider contracts, independently checked against their owning headers.
#include "b0e1e980f2-m15_l0.h"
#include "5aec12c3b5-m15_dust.h"
#include "3c66185f25-w5_types.hpp"
namespace kotor_world_los { struct Vector; }
namespace options_video { struct String; }
namespace c2da_current { struct Table; }
namespace kotor_w3 { struct Vector3; }
namespace client_movies { I32 __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*); }
namespace base_ini_reconstruction {
void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*);
}
extern "C" {
void __fastcall object_vfx_006a0c70(void*, void*);
void __fastcall object_vfx_006a5000(void*, void*);
void __fastcall object_vfx_006a1810(void*, void*);
void __fastcall object_vfx_006a56f0(void*, void*, U);
void __fastcall object_vfx_006a0bf0(void*, void*);
void __fastcall kotor_fastcall_006116a0(void*, void*, U);
void __fastcall kotor_fastcall_00610550(void*, void*);
U __fastcall kotor_fastcall_0060d320(void*, void*);
U __fastcall kotor_fastcall_00617470(void*, void*, void*, U);
void* __fastcall kotor_fastcall_005ed580(void*, void*, U);
void* __fastcall frame_R4_005ed5b0(void*, void*, U);
void* __fastcall kotor_fastcall_005ed8b0(void*, void*);
void* __fastcall kotor_fastcall_00635460(void*, void*);
U __fastcall w5x_creature_is_debilitated_005b4880(void*, void*);
U __fastcall kotor_animation_sound_animation_parry_0063c4f0(void*, void*, U);
void __fastcall recovered_client_creature_play_sound_set_sound_00611470(void*, void*, U, U);
U __fastcall object_projectile_0060d730(void*, void*, U, void*, const kotor_scene3d::Vector*, const String*);
U __fastcall recovered_client_area_apply_fire_and_forget_visual_effect_00604c90(void*, void*, U, float, float, float);
I32 __fastcall c2da_current_get_int_00413660(c2da_current::Table*, void*, I32, String*, I32*);
I32 __cdecl crt_rand_006fc447();
String* __fastcall resman_string_from_int_005e5bc0(String*, void*, I32);
extern void* kotor_cell_sound_007a39ec;
extern U kotor_rules_cell32_007a2128, kotor_rules_cell32_007a21a4;
extern U kotor_rules_cell32_007a20ec, kotor_rules_cell32_007a219c, kotor_rules_cell32_007a21b0, kotor_rules_cell32_007a21a0;
I32 __fastcall c2da_current_get_string_entry_00413190(c2da_current::Table*, void*, I32, I32, String*);
void* __fastcall server_get_module_004ae6b0(void*, void*);
void* __fastcall module_get_area_004c30f0(void*, void*);
void __fastcall area_vfx_listener_position_005d63c0(void*, void*, kotor_w3::Vector3*);
void __fastcall area_vfx_client_announce_005edf60(void*, void*, U, U, const kotor_w3::Vector3*);
// New providers owned by this bounded closure. No fallback implementations.
void __fastcall kotor_fastcall_0061a2d0(void*, void*);
U __fastcall kotor_m15closure_0060dc50(void*, void*, String*, void*, U);
U __fastcall kotor_m15closure_0060de20(void*, void*, void*);
void __fastcall kotor_m15closure_006a6950(void*, void*);
U __fastcall kotor_m15closure_00604bf0(void*, void*, KotorM15VectorWords);
void* __fastcall kotor_m15closure_00646c90(void*, void*);
U __fastcall kotor_m15closure_00555690(void*, void*);
void __fastcall kotor_m15closure_005d64b0(void*, void*, U, float*, float*);
void __fastcall kotor_m15closure_0060ded0(void*, void*, U);
U __fastcall kotor_m15closure_005063c0(void*, void*, KotorM15VectorWords);
U __fastcall kotor_m15closure_00581930(void*, void*, KotorM15VectorWords);
U __fastcall w2x_area_room_004bb600(void*, void*, void*, int*);
kotor_world_los::Vector* __fastcall collision_mesh_world_to_local_00596a00(void*, void*,
    kotor_world_los::Vector*, const kotor_world_los::Vector*);
U __fastcall w5_surface_mesh_check_aabb_all_00581610(void*, void*, w5::Vector, w5::Vector, w5::SurfaceHit*);
}


namespace {
// Raw argument bridges preserve the canonical callee declarations and DWORD transport.
// Parents invoke these through correctly typed volatile pointers so live String EH remains visible.
__declspec(noinline) U __cdecl raw_terrain_words(void* area, const void* position) {
    U result;
    __asm {
        mov eax, position
        mov ecx, dword ptr [eax]
        mov edx, dword ptr [eax + 4]
        mov eax, dword ptr [eax + 8]
        push eax
        push edx
        push ecx
        mov ecx, area
        xor edx, edx
        call kotor_m15closure_00604bf0
        mov result, eax
    }
    return result;
}
__declspec(noinline) U __cdecl raw_sound_words(void* const* sound_cell, void* ref, const void* position,
    U z_offset_word, U sound_group, U pitch_word) {
    U result;
    __asm {
        push pitch_word
        push 0
        push 0
        push 0
        push sound_group
        push z_offset_word
        mov eax, position
        mov ecx, dword ptr [eax]
        mov edx, dword ptr [eax + 4]
        mov eax, dword ptr [eax + 8]
        push eax
        push edx
        push ecx
        push ref
        mov ecx, sound_cell
        mov ecx, dword ptr [ecx]
        xor edx, edx
        call kotor_m15_sound_play_3d_one_shot_005d5e10
        mov result, eax
    }
    return result;
}
__declspec(noinline) U __cdecl raw_area_vfx_words(void* object, U effect, const void* position) {
    U result;
    __asm {
        mov eax, position
        mov ecx, dword ptr [eax]
        mov edx, dword ptr [eax + 4]
        mov eax, dword ptr [eax + 8]
        push eax
        push edx
        push ecx
        push effect
        mov ecx, object
        mov ecx, dword ptr [ecx + 0x20]
        xor edx, edx
        call recovered_client_area_apply_fire_and_forget_visual_effect_00604c90
        mov result, eax
    }
    return result;
}
__declspec(noinline) U16 __cdecl raw_distance_compare(const kotor_scene3d::Vector* delta, const float* maximum) {
    const float two = 2.0f;
    U16 status;
    __asm {
        push delta
        call vector_magnitude_004ab100
        add esp, 4
        mov eax, maximum
        fld dword ptr [eax]
        fadd dword ptr two
        fxch st(1)
        fcompp
        fnstsw ax
        mov status, ax
    }
    return status;
}
}

extern "C" {
// ======================================================================================= small slot bodies
// 00617090 CSWCCreature::GetName (slot +70) -> CExoString by hidden return (RET 4).
void* __fastcall kotor_m15e_creature_get_name_00617090(void* self, void*, void* out_string) noexcept(false) {
    kotor_m15b_creature_stats_get_full_name_00647ce0(stats_of(self), nullptr, out_string);   // 00617091..006170A5 [this+2F8h]
    return out_string;                                                                        // 006170AA mov eax,esi
}

// 0060B9B0 CSWCCreature::SetPosition(const Vector*) (slot +8C) RET 4 (EAX = last callee's, never used).
void __fastcall kotor_m15e_creature_set_position_0060b9b0(void* self, void*, const float* position) {
    std::memcpy(at(self, 0x24), position, 12);                         // 0060B9B8..0060B9CA [this+24h] = *pos
    void* const area = rp(self, 0x20);                                 // 0060B9CD
    if (area != nullptr)                                               // 0060B9D0..0060B9D2
        sound_volume29_required::predicate_006054e0(area, nullptr, const_cast<float*>(position));   // 0060B9D4..0060B9D5 006054E0 return_true_4
    if (kotor_m15e_cell_0083395c != 0)                                 // 0060B9DA..0060B9E1 [0083395C]
        object_methods(self)->s02c_as_creature(self, nullptr);         // 0060B9E3..0060B9E7 [vtbl+2Ch] (result unused)
    void* const anim = anim_base(self);                                // 0060B9EA
    if (anim != nullptr && rd32(self, 0x50) == 0)                      // 0060B9ED..0060B9F6
        anim_methods(anim)->s068(anim, nullptr, position);             // 0060B9F8..0060B9FB anim +68h SetPosition
    object_methods(self)->s090_position_changed(self, nullptr);         // 0060B9FE..0060BA02 [vtbl+90h]
}

// 0063C110 CSWCObject::SetOrientation(const Vector*) (slot +88) RET 4.
void __fastcall kotor_m15e_object_set_orientation_0063c110(void* self, void*, const float* orientation) {
    kotor_scene3d::Vector normalized;                                  // [esp+8] (0063C119)
    const kotor_scene3d::Vector* const result =
        normalize_004ab180(&normalized, reinterpret_cast<const kotor_scene3d::Vector*>(orientation));   // 0063C11D..0063C121
    std::memcpy(at(self, 0x30), result, 12);                           // 0063C126..0063C136 [this+30h] = normalize(v)
    void* const anim = anim_base(self);                                // 0063C139
    if (anim != nullptr && rd32(self, 0x50) == 0)                      // 0063C13F..0063C148
        anim_methods(anim)->s064(anim, nullptr, orientation);          // 0063C14A..0063C14D anim +64h SetOrientation(Vector) (original pointer)
    object_methods(self)->s090_position_changed(self, nullptr);         // 0063C150..0063C154 [vtbl+90h]
}

// 005EDB00 CClientExoApp::GetMiniGame() RET -> CSWCModule::GetMiniGame of the client module, or 0.
void* __fastcall kotor_m15e_client_get_mini_game_005edb00(void* client, void*) {
    void* const module = packed_file_get_async_file_00644760(rp(client, 4), nullptr);   // 005EDB00..005EDB03 [[this+4]+18h]
    if (module == nullptr) return nullptr;                             // 005EDB08..005EDB0A / 005EDB13
    return kotor_fastcall_00641980(module, nullptr);                   // 005EDB0C..005EDB0E jmp
}

// 0063D820 CSWCObject::AttachToScene(scene) RET 4: tail jump to anim base +04h, or RET 4 (EAX = anim base 0).
U __fastcall kotor_m15e_object_attach_to_scene_0063d820(void* self, void*, void* scene) {
    void* const anim = anim_base(self);                                // 0063D820
    if (anim == nullptr) return 0;                                     // 0063D823..0063D825 / 0063D82C ret 4: EAX not written (D2: 0; callers ignore it)
    return anim_methods(anim)->s004(anim, nullptr, scene);             // 0063D827..0063D829 jmp [vtbl+4]
}

// 0060FB40 CSWCCreature::AttachToScene(scene) (slot +94) RET 4.
U __fastcall kotor_m15e_creature_attach_to_scene_0060fb40(void* self, void*, void* scene) {
    if (rd32(self, 0x3A4) != 0 &&                                      // 0060FB43..0060FB4B
        kotor_m15e_client_get_mini_game_005edb00(client_app(), nullptr) != nullptr)   // 0060FB4D..0060FB5C
        return kotor_m15e_object_attach_to_scene_0063d820(self, nullptr, nullptr);    // 0060FB5E..0060FB62 AttachToScene(0)
    return kotor_m15e_object_attach_to_scene_0063d820(self, nullptr, scene);          // 0060FB6B..0060FB72
}

// 00646650 CSWCObject::UnloadModel(BYTE part) (slot +7C) RET 4: tail jump to anim base +74h.
U __fastcall kotor_m15e_object_unload_model_00646650(void* self, void*, U part) {
    void* const anim = anim_base(self);                                // 00646650
    if (anim == nullptr) return 0;                                     // 00646653..00646655 / 0064665C
    return anim_methods(anim)->s074(anim, nullptr, part);              // 00646657..00646659 jmp [vtbl+74h]
}

// 00616FD0 CSWCObject::HideShadowBlob (slot +108) RET: tail jump to anim base +B0h.
U __fastcall kotor_m15e_object_hide_shadow_blob_00616fd0(void* self, void*) {
    void* const anim = anim_base(self);                                // 00616FD0
    if (anim == nullptr) return 0;                                     // 00616FD3..00616FD5 / 00616FDF
    return anim_methods(anim)->s0b0(anim, nullptr);                    // 00616FD7..00616FD9 jmp [vtbl+B0h]
}

// 0063D070 CSWCObject::RemoveShadowBlob RET (target of the slot +100 thunk 0060E480 `jmp 0063D070`).
void __fastcall kotor_m15e_object_remove_shadow_blob_0063d070(void* self, void*) {
    void* const anim = anim_base(self);                                // 0063D073
    if (anim == nullptr) return;                                       // 0063D076..0063D078 / 0063D098
    anim_methods(anim)->s0a8(anim, nullptr);                           // 0063D07A..0063D07C anim +A8h RemoveShadowBlob
    if (rd32(self, 0xD4) == 0) return;                                 // 0063D082..0063D08A
    void* const again = anim_base(self);                               // 0063D08C reload [esi+68h]
    anim_methods(again)->s0b8(again, nullptr);                         // 0063D08F..0063D092 jmp anim +B8h TurnOnShadows
}
// 0060E480 CSWCObject::RemoveShadowBlob (slot +100): `jmp 0063D070`.
void __fastcall kotor_m15e_object_remove_shadow_blob_0060e480(void* self, void*) {
    kotor_m15e_object_remove_shadow_blob_0063d070(self, nullptr);         // 0060E480
}

// 0060F150 CSWCCreature::SetQuededAnimationCallback (slot +118) RET.
U __fastcall kotor_m15e_creature_set_queued_animation_callback_0060f150(void* self, void*) {
    if (rd32(self, 0xF4) != 0) return rd32(self, 0xF4);               // 0060F153..0060F15B (EAX = [this+F4h])
    void* const gob = creature_methods(self)->object.s098_get_model(self, nullptr, 0xFF, 1);   // 0060F15D..0060F168 GetModel(FFh,1)
    void* const user = object_methods(self)->s00c_as_creature_or_self(self, nullptr);         // 0060F16E..0060F17D [vtbl+0Ch]
    const U handle = gob_methods(gob)->register_callback(as_gob(gob), nullptr,
        &kotor_m15e_queued_callback_function_0063e510, "*",            // 0060F186 push 63E510h / 0060F181 push 743890h ("*")
        static_cast<aurora_model::Part*>(user), 0x461C3C00u, 0);       // 0060F180 push eax / 0060F176 push 461C3C00h (10000.0f) / 0060F170 push 0
    st<U>(self, 0xF4, handle);                                         // 0060F191 [this+F4h] = handle
    return handle;                                                     // EAX = handle
}

// 0060F1A0 CSWCCreature::ClearQuededAnimationCallback (slot +11C) RET -> 1 when a callback was removed, else 0.
U __fastcall kotor_m15e_creature_clear_queued_animation_callback_0060f1a0(void* self, void*) {
    void* const gob = creature_methods(self)->object.s098_get_model(self, nullptr, 0xFF, 1);   // 0060F1A3..0060F1AC GetModel(FFh,1)
    const U handle = rd32(self, 0xF4);                                 // 0060F1B2
    if (handle == 0 || gob == nullptr) return 0;                       // 0060F1B8..0060F1BE / 0060F1D9
    gob_methods(gob)->s02c_gob_deregister_callback(as_gob(gob), nullptr, handle);   // 0060F1C0..0060F1C5 gob +2Ch
    st<U>(self, 0xF4, 0);                                              // 0060F1C8
    return 1;                                                          // 0060F1D2
}

// 0063D330 CSWCObject::QueuedCallback(gob, const char* event) RET 8: on a "DONE..." event, the queued animation
// [this+4Ah] (when set) becomes the current one if the event is "done<name of the current animation>".
void __fastcall kotor_m15e_object_queued_callback_0063d330(void* self, void*, void* gob, const char* event) noexcept(false) {
    (void)gob;
    if (ld<U16>(self, 0x4A) == 0) return;                              // 0063D34B..0063D350
    // 0063D353..0063D369 _strnicmp(event, "DONE", 4) (0070AD18). Bound to the UCRT _strnicmp: the original takes its
    // ___ascii_strnicmp path (0070B920) whenever the thread locale handle LC_CTYPE is 0, i.e. the "C" locale this
    // process keeps (setlocale 006FE34D runs only for language id 5, D79); for the "C" locale both compare bytewise
    // after ASCII A..Z -> a..z folding (equivalence precedent: lane-3d-render scene3d.cpp 00457DEE).
    if (_strnicmp(event, "DONE", 4) != 0) return;                      // 007516BC "DONE"
    char expected[64];                                                 // [esp+10h] (0063D38F; 0x40-byte frame buffer)
    {
        String name;                                                   // [esp+8] hidden return of anim +40h (EH state 0, 0063D381)
        void* const anim = anim_base(self);                            // 0063D36B
        String* const got = static_cast<String*>(anim_methods(anim)->s040(anim, nullptr, &name,
                                                                       ld<U16>(self, 0x48)));   // 0063D36E..0063D37C GetAnimationName([this+48h])
        format_006fadb0(expected, "done%s", async_string_cstr_005e5670(got, nullptr));   // 0063D37F..0063D399 sprintf(buf,"done%s",...)
    }                                                                  // 0063D3A1..0063D3AD ~CExoString
    const int different = kotor_stricmp_0070acaf(event, expected);     // 0063D3B2..0063D3B8 __stricmp
    if (different != 0) return;                                        // 0063D3C0..0063D3C2
    st<U16>(self, 0x48, ld<U16>(self, 0x4A));                          // 0063D3C4..0063D3C8 [48h] = [4Ah]
    st<U16>(self, 0x4A, static_cast<U16>(different));                  // 0063D3CC [4Ah] = ax (0)
}

// 0063E510 QueuedCallbackFunction(gob, event, user = CSWCObject*) __cdecl (registered by 0060F186 with event "*").
void __cdecl kotor_m15e_queued_callback_function_0063e510(void* gob, void* event, aurora_model::Part* user) {
    kotor_m15e_object_queued_callback_0063d330(user, nullptr, gob, static_cast<const char*>(event));   // 0063E510..0063E51E
}

// 006179C0 CSWCCreature::GetModel(BYTE part, int force) (slot +98) RET 8 -> Gob* of the part, or 0 without anim base.
void* __fastcall kotor_m15e_creature_get_model_006179c0(void* self, void*, U part, U force) noexcept(false) {
    void* const anim = anim_base(self);                                // 006179C3
    if (anim == nullptr) return nullptr;                               // 006179C6..006179CD
    if (force != 0 && rd32(self, 0x310) != 0 &&                        // 006179D0..006179E5 (jbe: unsigned > 0)
        static_cast<U8>(part) == 0xFF) {                               // 006179E7..006179EA
        // 006179EC..006179F8: check whether the existing body model is already present.
        // Only a missing body model executes the original rebuild/camera continuation.
        if (anim_methods(anim)->s008(anim, nullptr, 0xFF) == nullptr) {
            kotor_fastcall_00614a30(self, nullptr);                    // 006179FA..006179FC
            kotor_fastcall_0060e6c0(self, nullptr);                    // 00617A01..00617A03
        }
    }
    void* const anim2 = anim_base(self);                               // 00617A08 reload
    return anim_methods(anim2)->s008(anim2, nullptr, part);            // 00617A0B..00617A0E anim +08h GetModel(part)
}

// 006973B0 CSWCCreatureAppearance::GetHeight() RET -> ST0. HEAD_G part position z minus the creature's z when that
// is >= 0, else the appearance height [this+28h]; 0.0 without a body model.
double __fastcall kotor_m15e_appearance_get_height_006973b0(void* self, void*) noexcept(false) {
    float v[3];                                                        // [S+0..S+0Bh] Vector out (S+4/S+8 zeroed 006973BB/006973C3)
    float q[4];                                                        // [S+0Ch..S+1Bh] Quaternion out
    v[1] = 0.0f; v[2] = 0.0f;                                          // 006973BB / 006973C3 [S+4] = [S+8] = 0
    v[0] = 0.0f;                                                       // D2 (0069740B): [S+0] has no store before the gob call; value-initialised
    q[0] = 0.0f; q[1] = 1.0f; q[2] = 0.0f; q[3] = 0.0f;               // 006973CB [S+0Ch]=0, 006973EB [S+10h]=1.0, 006973E3 [S+14h]=0, 006973DB [S+18h]=0
                                                                       // 006973D3 [S+1Ch] = 0: dead store (never read)
    void* const creature = rp(self, 0x3C);                             // 006973B6 [this+3Ch]
    void* const gob = creature_methods(creature)->object.s098_get_model(creature, nullptr, 0xFF, 1);   // 006973B9..006973FA GetModel(FFh,1)
    if (gob == nullptr) return 0.0;                                    // 00697400..00697402 / 0069744F fld [0073D700] (0.0f)
    gob_methods(gob)->s098_gob_get_part_position(as_gob(gob), nullptr, word_of("HEAD_G"),   // 00697410 push 754A1Ch
        word_of(v), word_of(q));   // 0069740B / 00697406; 00697417 gob +98h
    if (v[2] == 0.0f) return static_cast<double>(fbits(rd32(self, 0x28)));   // 0069741D..0069742C (ZF, ordered) -> 00697447 fld [this+28h]
    const double diff = static_cast<double>(v[2]) - static_cast<double>(fbits(rd32(creature, 0x2C)));   // 00697431..00697435 fsub [creature+2Ch] (wide)
    if (diff >= 0.0) return diff;                                      // 00697438..00697443 fcom 0.0; test ah,5; jp (>= 0 or unordered)
    if (diff != diff) return diff;                                     //   unordered (NaN) also takes jp -> 00697455 with ST0 = diff
    return static_cast<double>(fbits(rd32(self, 0x28)));               // 00697445..00697447 fstp st(0); fld [this+28h]
}

// 006974D0 CSWCCreatureAppearance::GetHeadHeightOffset() RET -> ST0: CAMERAHOOK local z, else GetHeight().
double __fastcall kotor_m15e_appearance_get_head_height_offset_006974d0(void* self, void*) noexcept(false) {
    void* const creature = rp(self, 0x3C);                             // 006974D7 [this+3Ch]
    void* const gob = creature_methods(creature)->object.s098_get_model(creature, nullptr, 0xFF, 1);   // 006974DA..006974E3 GetModel(FFh,1)
    if (gob == nullptr) return 0.0;                                    // 006974EB..006974ED / 0069756A fld [0073D700]
    if (gob_methods(gob)->s10c_gob_get_part(as_gob(gob), nullptr, "CAMERAHOOK") == nullptr)   // 006974EF..006974F8 gob +10Ch GetPart(74F59Ch)
        return kotor_m15e_appearance_get_height_006973b0(self, nullptr);   // 006974FE..00697500 / 0069755D..0069755F
    float v[3] = {0.0f, 0.0f, 0.0f};                                   // [S+8..S+13h] 00697515/0069751D/00697525
    float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};                             // [S+14h..S+23h] 00697545 1.0, 0069753D/00697535/0069752D 0
    gob_methods(gob)->s0a0_gob_get_part_local_position(as_gob(gob), nullptr, word_of("CAMERAHOOK"),   // 0069750E push 74F59Ch
        word_of(v), word_of(q));   // 00697509 / 00697504; 0069754D gob +A0h
    return static_cast<double>(v[2]);                                  // 00697553 fld [S+10h]
}

// 0060F000 CSWCCreature::GetHeadHeightOffset (slot +D8): `mov ecx,[ecx+21Ch]; jmp 006974D0`.
double __fastcall kotor_m15e_creature_get_head_height_offset_0060f000(void* self, void*) noexcept(false) {
    return kotor_m15e_appearance_get_head_height_offset_006974d0(rp(self, 0x21C), nullptr);   // 0060F000..0060F006
}

// ======================================================================================= portraits
// 00406300 CResRef::operator+=(out, const CExoString& suffix) RET 8: this = lower(this + suffix) truncated to 16
// bytes; *out = *this; EAX = out.
Ref* __fastcall kotor_m15e_resref_append_00406300(Ref* self, void*, Ref* out, String* suffix) noexcept(false) {
    String text;                                                       // [esp+14h] 0040631E..00406327 (EH state 0)
    {
        char bytes[17];                                                // [esp+24h..34h] 0040632C..0040635A
        std::memcpy(bytes, self->bytes, 16);
        bytes[16] = '\0';                                              // 0040635A mov byte ptr [esp+38h],0
        c2da_assign_text_005e5140(&text, nullptr, bytes);              // 00406349..0040635F operator=(char*)
    }
    {
        String sum;                                                    // [esp+0Ch] hidden return (EH state 1)
        GuiConcat005e5d10(&text, nullptr, &sum, suffix);               // 00406364..00406371 text + suffix
        async_assign_string_005e5c50(&text, nullptr, &sum);            // 00406376..00406380 text = sum
    }                                                                  // 00406385..0040638E ~sum
    {
        String lower;                                                  // [esp+1Ch] hidden return (EH state 2)
        lower_construct_005e6080(&text, nullptr, &lower);              // 00406393..0040639C LowerCase
        async_assign_string_005e5c50(&text, nullptr, &lower);          // 004063A1..004063AB
    }                                                                  // 004063B0..004063B9 ~lower
    std::memset(self->bytes, 0, 16);                                   // 004063BE..004063CA
    if (static_cast<std::int32_t>(async_string_length_005e5790(&text, nullptr)) > 0) {   // 004063CD..004063D8
        std::uint32_t count;                                           // [esp+0Ch]
        if (async_string_length_005e5790(&text, nullptr) > 0x10u) count = 0x10;           // 004063DA..004063F0 (jbe: unsigned)
        else count = async_string_length_005e5790(&text, nullptr);     // 004063F2..004063FB
        std::memcpy(self->bytes, async_string_cstr_005e5670(&text, nullptr), count);       // 004063FF..0040641C rep movs
    }
    std::memcpy(out->bytes, self->bytes, 16);                          // 0040641E..00406436
    return out;                                                        // 0040644F mov eax,esi
}                                                                      // 00406439..00406445 ~text

// 00678EF0 CSWPortrait::GetPortrait(out, BYTE size) RET 8: resref + "T"/"S"/"M"/"L"/"H" for size 1..5, else the
// base resref; "" when the base resref is empty.
Ref* __fastcall kotor_m15e_portrait_get_portrait_00678ef0(void* self, void*, Ref* out, U size) noexcept(false) {
    Ref resref;                                                        // [esp+8] 00678F11..00678F2D copy of *this
    std::memcpy(resref.bytes, self, 16);
    const unsigned char* const first =
        static_cast<const unsigned char*>(kotor_m15e_slot00c_self_00641db0(&resref, nullptr));   // 00678F25..00678F31 self_return
    if (*first == 0) {                                                 // 00678F36..00678F39
        out->construct_00406d80(kEmpty_0073d71c);                      // 00678F3B..00678F45 CResRef("")
        return out;                                                    // 00678F4A
    }
    static const char* const kSuffix[5] = {"T", "S", "M", "L", "H"};   // 00746BB0 / 00746BAC / 00746BA8 / 00746BA4 / 00752CCC
    const U index = static_cast<U>(static_cast<U8>(size)) - 1u;       // 00678F61..00678F65 movzx; dec
    if (index <= 4u) {                                                 // 00678F66..00678F69 (ja 00679087); table 006790BC
        String suffix(kSuffix[index]);                                 // 00678F76.. CExoString(char*) (EH states 0..4)
        Ref copy;                                                      // [esp+40h..] hidden return of operator+= (dead)
        kotor_m15e_resref_append_00406300(&resref, nullptr, &copy, &suffix);   // 00678F84..00678F9D
    }                                                                  // 00679077..00679082 ~suffix
    std::memcpy(out->bytes, resref.bytes, 16);                         // 00679087..006790A4
    return out;                                                        // 0067908A mov eax,[ebp+8]
}

// 006790D0 CSWPortrait::SetPortraitId(WORD id) RET 4 -> 1 (resref set from portraits.2da) / 0 (id >= FFFEh).
U __fastcall kotor_m15e_portrait_set_portrait_id_006790d0(void* self, void*, U id) noexcept(false) {
    String value;                                                      // [esp+10h] 006790F3..006790F7 (EH state 0)
    String prefix;                                                     // [esp+18h] 006790FC..00679108 (EH state 1)
    const U16 row = static_cast<U16>(id);                              // 0067910D si = word [ebp+8]
    if (row >= 0xFFFEu) return 0;                                      // 00679111..0067911B (jb), 0067911D..0067913C
    void* const options = kotor_fastcall_005ed700(client_app(), nullptr);   // 00679152..0067915A GetClientOptions
    bool not_found;                                                    // bl (neg/sbb/inc: 1 when the lookup returned 0)
    if (ld<U8>(options, 0x72) == 0) {                                  // 0067915F..00679168
        {
            String column("LowGore");                                  // 0067916A..0067916F [esp+20h] (EH state 2), 0074EF78
            not_found = c2da_current_get_string_00413270(static_cast<c2da_current::Table*>(rp(rp(rules(), 0xB8), 0x38)), nullptr, row, &column,
                                                         &value) == 0; // 00679174..00679198 [[[007A3A28]+B8h]+38h]
        }                                                              // 006791A3..006791AE ~column
        if (not_found) {                                               // 006791B3..006791B5 (found -> 0067924F)
            String column("BaseResRef");                               // 006791BB..006791C4 (EH state 3), 00745E40
            not_found = c2da_current_get_string_00413270(static_cast<c2da_current::Table*>(rp(rp(rules(), 0xB8), 0x38)), nullptr, row, &column,
                                                         &value) == 0; // 006791C9..00679219 (shared tail 0067920F)
            if (not_found) {                                           // 0067922F..00679231
                c2da_assign_text_005e5140(&prefix, nullptr, kEmpty_0073d71c);   // 00679233..0067923C
                c2da_assign_text_005e5140(&value, nullptr, kEmpty_0073d71c);    // 00679241..0067924A
            }
        }                                                              // 0067921F..0067922A ~column
    } else {
        String column("BaseResRef");                                   // 006791E7..006791EC (EH state 4)
        not_found = c2da_current_get_string_00413270(static_cast<c2da_current::Table*>(rp(rp(rules(), 0xB8), 0x38)), nullptr, row, &column,
                                                     &value) == 0;     // 006791F1..00679219
        if (not_found) {                                               // 0067922F..00679231
            c2da_assign_text_005e5140(&prefix, nullptr, kEmpty_0073d71c);
            c2da_assign_text_005e5140(&value, nullptr, kEmpty_0073d71c);
        }
    }
    {
        String sum;                                                    // [esp+20h] hidden return (EH state 5)
        GuiConcat005e5d10(&prefix, nullptr, &sum, &value);             // 0067924F..0067925D prefix + value
        async_assign_string_005e5c50(&value, nullptr, &sum);           // 00679262..0067926C
    }                                                                  // 00679271..0067927A ~sum
    {
        Ref dead;                                                      // [esp+20h] hidden return of operator=(CExoString)
        async_ref_assign_string_004061f0(static_cast<Ref*>(self), nullptr, &dead, &value);   // 0067927F..0067928B
    }
    return 1;                                                          // 006792B5
}                                                                      // 00679290..006792AA ~prefix, ~value

// 00647B80 CSWCCreatureStats::SetPortraitId(WORD id) RET 4.
void __fastcall kotor_m15e_creature_stats_set_portrait_id_00647b80(void* self, void*, U id) noexcept(false) {
    alignas(4) unsigned char portrait[16];                             // [esp+8] CSWPortrait (EH state 0)
    kotor_m15b_portrait_construct_00678ec0(portrait, nullptr);         // 00647B9C..00647BA0
    if (kotor_m15e_portrait_set_portrait_id_006790d0(portrait, nullptr, id) != 0) {   // 00647BA5..00647BBD
        Ref text;                                                      // [esp+18h] hidden return of GetPortrait
        Ref* const got = kotor_m15e_portrait_get_portrait_00678ef0(portrait, nullptr, &text, 0);   // 00647BBF..00647BCA GetPortrait(0)
        Ref dead;                                                      // [esp+28h] hidden return of operator=
        c2da_copy_00406120(static_cast<Ref*>(at(self, 0x70)), nullptr, &dead, got);   // 00647BCF..00647BD8 [this+70h] = portrait
    }
    st<U16>(self, 0x11C, static_cast<U16>(id));                        // 00647BE1 [this+11Ch] = di
    kotor_m15e_slot090_void0_004015a0(portrait, nullptr);                     // 00647BDD..00647BF0 ~CSWPortrait (004015A0 `ret`)
}

// 00647E80 CSWCCreatureStats::GetPortrait(out, int variant) RET 8.
Ref* __fastcall kotor_m15e_creature_stats_get_portrait_00647e80(void* self, void*, Ref* out, U variant) noexcept(false) {
    String value;                                                      // [esp+10h] 00647EA3..00647EA7 (EH state 0)
    void* const portraits = rp(rp(rules(), 0xB8), 0x38);              // 00647EAC..00647EB7 edi
    const char* column = nullptr;
    if (portraits != nullptr && variant != 0) {                        // 00647EBA..00647ECF
        c2da_assign_text_005e5140(&value, nullptr, kEmpty_0073d71c);   // 00647ED5..00647EDE
        const std::int16_t alignment = ld<std::int16_t>(self, 0x80);   // 00647EE3 ax = [this+80h] (signed jg)
        if (alignment <= 0x0A) column = "BaseResRefVVVE";              // 00647EEA..00647EF0 751C44
        else if (alignment <= 0x14) column = "BaseResRefVVE";          // 00647F5F..00647F65 751C34
        else if (alignment <= 0x1E) column = "BaseResRefVE";           // 00647FD1..00647FDB 751C24
        else if (alignment <= 0x28) column = "BaseResRefE";            // 0064806C..00648072 751C18
    }
    if (column != nullptr) {
        bool not_found;
        {
            String name(column);                                       // [esp+18h] (EH states 1..4)
            not_found = c2da_current_get_string_00413270(static_cast<c2da_current::Table*>(portraits), nullptr, ld<U16>(self, 0x11C), &name,
                                                         &value) == 0; // 00647EFE..00647F17 (row = word [this+11Ch])
        }                                                              // ~name
        if (!not_found) {                                              // 00647F32..00647F34 je 006480DD
            async_ref_construct_string_00406d60(out, nullptr, &value); // 006480DD..006480E7 CResRef(CExoString)
            return out;
        }
    }
    std::memcpy(out->bytes, at(self, 0x70), 16);                       // 00647F3A / 00647FAF / 00648025 / 006480B8 copy [this+70h]
    return out;                                                        // 0064805B mov eax,esi
}                                                                      // 00648045..00648051 ~value

// 00616FF0 CSWCCreature::GetPortrait(out, int variant) (slot +148) RET 8.
void* __fastcall kotor_m15e_creature_get_portrait_00616ff0(void* self, void*, void* out_resref, U variant) noexcept(false) {
    void* const stats = stats_of(self);                                // 00616FF0
    Ref* const out = static_cast<Ref*>(out_resref);
    if (stats != nullptr) {                                            // 00616FF6..00616FFD
        kotor_m15e_creature_stats_get_portrait_00647e80(stats, nullptr, out, variant);   // 00616FFF..00617005
        return out;                                                    // 0061700A
    }
    out->construct_00406d80(kEmpty_0073d71c);                          // 00617010..00617017 CResRef("")
    return out;                                                        // 0061701C
}

// 0060D560 CSWCCreature::SetPortrait(const CResRef*) (slot +EC) RET 4.
void __fastcall kotor_m15e_creature_set_portrait_0060d560(void* self, void*, void* resref) noexcept(false) {
    Ref current;                                                       // [esp+8] hidden return
    void* const got = creature_methods(self)->s148_get_portrait(self, nullptr, &current, 0);   // 0060D56B..0060D577 [vtbl+148h](&tmp, 0)
    const U differs = gui_load_resref_not_equal_00406dc0(static_cast<const Ref*>(got), nullptr,
                                                         static_cast<const Ref*>(resref));   // 0060D57D..0060D57F
    if (differs == 0) return;
    {
        Ref dead;                                                      // [esp+8] hidden return of operator=
        c2da_copy_00406120(static_cast<Ref*>(at(stats_of(self), 0x70)), nullptr, &dead,
                           static_cast<Ref*>(resref));                 // 0060D588..0060D597 [[this+2F8h]+70h] = *resref
    }
    Ref variant;                                                       // [esp+8] hidden return
    void* const evil = creature_methods(self)->s148_get_portrait(self, nullptr, &variant, 1);   // 0060D59C..0060D5A7 [vtbl+148h](&tmp, 1)
    gui_load_slot_ret4_0060e760(self, nullptr, word_of(evil));         // 0060D5AD..0060D5B0 call 0060E760 (`ret 4`)
}

// 0060D5C0 CSWCCreature::SetPortraitId(WORD id) (slot +F4) RET 4.
void __fastcall kotor_m15e_creature_set_portrait_id_0060d5c0(void* self, void*, U id) noexcept(false) {
    void* const stats = stats_of(self);                                // 0060D5C6
    if (stats == nullptr) return;
    kotor_m15e_creature_stats_set_portrait_id_00647b80(stats, nullptr, id);   // 0060D5D0..0060D5D5
    Ref variant;                                                       // [esp+4] hidden return
    void* const evil = creature_methods(self)->s148_get_portrait(self, nullptr, &variant, 1);   // 0060D5DA..0060D5E5 [vtbl+148h](&tmp, 1)
    gui_load_slot_ret4_0060e760(self, nullptr, word_of(evil));         // 0060D5EB..0060D5EE call 0060E760 (`ret 4`)
}

// ======================================================================================= shadow / env map / VFX
// 0060E2F0 CSWCCreature::ApplyShadowBlob (slot +FC) RET -> anim base ApplyShadowBlob result, or 0 without anim base.
U __fastcall kotor_m15e_creature_apply_shadow_blob_0060e2f0(void* self, void*) noexcept(false) {
    String name;                                                       // [esp+10h] 0060E30D..0060E311 (EH state 0)
    if (anim_base(self) == nullptr) return 0;                          // 0060E316..0060E323 / 0060E325..0060E336
    std::int32_t size = 0;                                             // [esp+0Ch]
    const U16 row = ld<U16>(rp(self, 0x21C), 0x18);                    // 0060E34A..0060E350 [[this+21Ch]+18h]
    if (c2da_current_get_int_entry_00413510(static_cast<c2da_current::Table*>(rp(rp(rules(), 0xB8), 0x0C)), nullptr, row,
                                            static_cast<std::int32_t>(kotor_rules_cell32_007a2124), &size) == 0)   // 0060E354..0060E373
        size = 3;                                                      // 0060E37C
    const char* blob;
    switch (size) {                                                    // 0060E384..0060E38E table 0060E464 (value-1, 0..4)
    case 1: case 2: blob = "Fx_Shadow_S"; break;                       // 0060E395 push 74F3A0h
    case 4: case 5: blob = "Fx_Shadow_L"; break;                       // 0060E39C push 74F394h
    default: blob = "Fx_Shadow_M"; break;                              // 0060E3A3 push 74F388h (3 and out of range)
    }
    c2da_assign_text_005e5140(&name, nullptr, blob);                   // 0060E3A8..0060E3AC
    void* anim = anim_base(self);                                      // 0060E3B1
    const U applied = anim_methods(anim)->s0a4(anim, nullptr, &name);   // 0060E3B4..0060E3BB anim +A4h ApplyShadowBlob(&name)
    const kotor_scene3d::Vector up = {0.0f, 0.0f, 1.0f};               // [esp+18h..20h] 0060E3C6..0060E3D6
    kotor_scene3d::Vector normal;                                      // [this+D8h] ground normal (0060E3E3..0060E3FC)
    std::memcpy(&normal, at(self, 0xD8), 12);
    kotor_scene3d::Quaternion arc;                                     // [esp+24h] hidden return (0060E411)
    kotor_scene3d::Quaternion* const q = rotation_arc_004ab630(&arc, up, normal);   // 0060E419 RotationArc(&q, up, normal)
    anim = anim_base(self);                                            // 0060E3C1 / 0060E41E ecx = [esi+68h]
    anim_methods(anim)->s0b4(anim, nullptr, reinterpret_cast<const float*>(q));   // 0060E424..0060E425 anim +B4h SetShadowBlobOrientation
    if (rd32(self, 0xD4) == 0)                                         // 0060E42B..0060E433
        object_methods(self)->s108_hide_shadow_blob(self, nullptr);    // 0060E435..0060E439 [vtbl+108h]
    return applied;                                                    // 0060E454 mov eax,edi
}                                                                      // 0060E43F..0060E44B ~name

// 0063CCC0 CSWCObject::SetEnvMap(BYTE part, a2, a3, const CResRef* env) (slot +AC) RET 10h.
U __fastcall kotor_m15e_object_set_env_map_0063ccc0(void* self, void*, U part, U a2, U a3, void* resref) noexcept(false) {
    String text;                                                       // [esp+4] 0063CCDB..0063CCDF (EH state 0)
    if (anim_base(self) == nullptr) return 0;                          // 0063CCE4..0063CCF1 / 0063CCF3..0063CD04
    void* const module = kotor_fastcall_005ed530(client_app(), nullptr);   // 0063CD18..0063CD22 GetModule
    Ref* const env = static_cast<Ref*>(resref);                        // 0063CD27 edi = arg4
    if (c2da_equal_text_004060d0(env, nullptr, "DEFAULT") != 0) {      // 0063CD2B..0063CD3B (0073E304)
        if (module != nullptr) {                                       // 0063CD3D..0063CD3F
            void* const area = rp(module, 0x48);                       // 0063CD41 [module+48h]
            if (area != nullptr) {                                     // 0063CD44..0063CD46
                Ref copy;                                              // [esp+14h] copy of [area+BCh] (0063CD48..0063CD6E)
                std::memcpy(copy.bytes, at(area, 0xBC), 16);
                c2da_ref_to_string_00405f70(&copy, nullptr, &text);    // 0063CD61..0063CD72 CopyToString
            }
        }
        if (async_string_length_005e5790(&text, nullptr) == 0) return 0;   // 0063CD77..0063CD82 / 0063CD84..0063CD97
    } else {
        c2da_ref_to_string_00405f70(env, nullptr, &text);              // 0063CDAB..0063CDB2
    }
    Ref map;                                                           // [esp+14h] 0063CDB7..0063CDC0 CResRef(CExoString)
    async_ref_construct_string_00406d60(&map, nullptr, &text);
    void* const anim = anim_base(self);                                // 0063CDC5
    return anim_methods(anim)->s098(anim, nullptr, part, a2, a3, &map);   // 0063CDC8..0063CDDE anim +98h SetEnvMap(part, a2, a3, &map)
}                                                                      // 0063CDE4..0063CDF2 ~text

void __fastcall kotor_m15e_object_detach_visual_effects_0063e450(void* self, void*) {
    void* const list = rp(self, 0x6C);
    void* position = rp(list, 0);
    void* effect = position != nullptr ? rp(position, 8) : nullptr;
    if (position == nullptr) return;
    do {
        if (rp(effect, 0xC0) == nullptr && rp(effect, 0xA8) != nullptr)
            object_vfx_006a0c70(effect, nullptr);
        effect = word_ptr(unresolved_list_next_005e9b90(rp(self, 0x6C), nullptr, &position));
    } while (position != nullptr);
}
void __fastcall kotor_m15e_object_reattach_visual_effects_0063e4b0(void* self, void*) {
    void* const list = rp(self, 0x6C);
    void* position = rp(list, 0);
    void* effect = position != nullptr ? rp(position, 8) : nullptr;
    if (position == nullptr) return;
    do {
        if (rp(effect, 0xC0) == nullptr && rp(effect, 0xA8) != nullptr)
            object_vfx_006a5000(effect, nullptr);
        effect = word_ptr(unresolved_list_next_005e9b90(rp(self, 0x6C), nullptr, &position));
    } while (position != nullptr);
}
void __fastcall kotor_m15e_object_kill_all_visual_effects_0063dae0(void* self, void*) {
    while (rp(rp(self, 0x6C), 0) != nullptr) {
        void* const effect = base_ini_reconstruction::remove_head_005e9c80(
            static_cast<kotor_list::Internal*>(rp(self, 0x6C)), nullptr);
        if (effect != nullptr) {
            kotor_m15closure_006a6950(effect, nullptr);
            object_vfx_006a1810(effect, nullptr);
            live_free(effect);
        }
    }
}

// ======================================================================================= destruction
// 004CC320 CSWSObject::DetachFromClientObject RET: [this+224h] = 0.
void __fastcall kotor_m15e_server_object_detach_from_client_object_004cc320(void* self, void*) {
    st<U>(self, 0x224, 0);                                             // 004CC320
}

// 005F63A0 CClientExoAppInternal::RemoveObjectFromHitcheckIgnoreList(DWORD id) RET 4: removes and frees the first
// node of the list [this+190h] whose data's first dword equals id.
void __fastcall kotor_m15e_client_internal_remove_from_hitcheck_ignore_list_005f63a0(void* self, void*, U id) {
    void* const list = rp(self, 0x190);                                // 005F63A5 eax = [edi+190h]
    void* position = rp(list, 0);                                      // 005F63AB..005F63AD [esp+8] = [eax]
    void* const head = rp(list, 0);                                    // 005F63B1 mov eax,[eax]
    void* data = head != nullptr ? rp(head, 8) : nullptr;              // 005F63B3..005F63B7 node data [node+8]
    if (position == nullptr) return;                                   // 005F63BA..005F63BE
    for (;;) {
        if (data != nullptr && rd32(data, 0) == id) {                  // 005F63C5..005F63CB
            c2da_005e9c10(rp(self, 0x190), nullptr, position);         // 005F63EE..005F63F5 Remove(position)
            live_free(data);                                           // 005F63FA..005F63FB free(data)
            return;
        }
        data = reinterpret_cast<void*>(static_cast<std::uintptr_t>(
            unresolved_list_next_005e9b90(rp(self, 0x190), nullptr, &position)));   // 005F63CD..005F63D8 GetNext(&position) -> data
        if (position == nullptr) return;                               // 005F63DD..005F63E5
    }
}
// 005ED760 CClientExoApp::RemoveObjectFromHitcheckIgnoreList(id) RET 4: `mov ecx,[ecx+4]; jmp 005F63A0`.
void __fastcall kotor_m15e_client_remove_from_hitcheck_ignore_list_005ed760(void* client, void*, U id) {
    kotor_m15e_client_internal_remove_from_hitcheck_ignore_list_005f63a0(rp(client, 4), nullptr, id);   // 005ED760..005ED763
}
// 005ED6B0 CClientExoApp::GetClientAIMaster RET -> [[this+4]+0Ch].
void* __fastcall kotor_m15e_client_get_client_ai_master_005ed6b0(void* client, void*) {
    return rp(rp(client, 4), 0x0C);                                    // 005ED6B0..005ED6B3
}
// 00604810 CClientAIList::RemoveObject(CSWCObject*) RET 4 -> 1 when the object was not in the list, else 0.
U __fastcall kotor_m15e_client_ai_list_remove_object_00604810(void* self, void*, void* object) {
    void* const node = rp(object, 0x1C);                               // 00604812..00604816 [object+1Ch]
    if (node == nullptr) return 1;                                     // 00604819..00604828
    void** const cursor = static_cast<void**>(at(self, 8));            // 00604831 edi = this+8 (iteration position)
    if (*cursor == node) {                                             // 0060482B..00604834
        unresolved_list_next_005e9b90(rp(self, 4), nullptr, cursor);   // 00604836..0060483A GetNext(cursor)
        if (*cursor == nullptr) {                                      // 0060483F..00604842
            void* const head = rp(rp(self, 4), 0);                     // 00604844..00604847 [[this+4]]
            *cursor = head;                                            // 0060484B
            if (head == node) *cursor = nullptr;                       // 00604849..0060484F
        }
    }
    st<I32>(self, 0, ld<I32>(self, 0) - 1);                            // 00604855 dec [this]
    c2da_005e9c10(rp(self, 4), nullptr, node);                         // 00604857..0060485B Remove(node)
    st<U>(object, 0x1C, 0);                                            // 00604862
    return 0;                                                          // 0060486A
}
// 00604A30 CClientAIMaster::RemoveObject(CSWCObject*) RET 4: `add ecx,4; jmp 00604810`.
U __fastcall kotor_m15e_client_ai_master_remove_object_00604a30(void* self, void*, void* object) {
    return kotor_m15e_client_ai_list_remove_object_00604810(at(self, 4), nullptr, object);   // 00604A30..00604A33
}

// 005F1460 CClientExoAppInternal::ClearCachedCreatures(CSWCCreature*) RET 4.
void __fastcall kotor_m15e_client_internal_clear_cached_creatures_005f1460(void* self, void*, void* creature) {
    if (rp(self, 0x3C0) == creature) {                                 // 005F1460..005F146A
        st<U>(self, 0x3BC, 0x7F000000u);                               // 005F146C (OBJECT_INVALID)
        st<U>(self, 0x3C0, 0);                                         // 005F1476
    }
}
// 005EDF50 CClientExoApp::ClearCachedCreatures(creature) RET 4: `mov ecx,[ecx+4]; jmp 005F1460`.
void __fastcall kotor_m15e_client_clear_cached_creatures_005edf50(void* client, void*, void* creature) {
    kotor_m15e_client_internal_clear_cached_creatures_005f1460(rp(client, 4), nullptr, creature);   // 005EDF50..005EDF53
}

// 006145D0 CSWCCreature::ClearVisibilityList RET: CExoArrayList<void*> [this+360h]/[this+364h] emptied from the
// back (DelIndex inlined), each element freed.
void __fastcall kotor_m15e_creature_clear_visibility_list_006145d0(void* self, void*) {
    I32 index = ld<I32>(self, 0x364) - 1;                              // 006145D4..006145DA
    while (index >= 0) {                                               // 006145DB js / 00614628 jns
        const I32 count = ld<I32>(self, 0x364) - 1;                    // 006145E0 / 006145EF
        void* const element = rp(rp(self, 0x360), static_cast<unsigned>(index) * 4u);   // 006145E6..006145EC
        st<I32>(self, 0x364, count);                                   // 006145F6
        for (I32 k = index; k < ld<I32>(self, 0x364); ++k) {           // 006145F2..00614618
            void* const data = rp(self, 0x360);                        // 00614600
            st<void*>(data, static_cast<unsigned>(k) * 4u, rp(data, static_cast<unsigned>(k + 1) * 4u));   // 00614606..0061460D
        }
        if (element != nullptr) live_free(element);                    // 0061461A..0061461F
        --index;                                                       // 00614627
    }
}

// 00695D10 CSWCSpellCastVisual::UnloadConjureVisual RET: models [+0]/[+4]/[+8] detached (+50h(0,0,0)) and deleted.
// 00695D90 CSWCSpellCastVisual::UnloadCastVisual RET: the same for [+0Ch]/[+10h]/[+14h].
// The members are Aurora objects (gob table: +50h AttachToObject, +00 scalar deleting destructor); the char-gen
// creatures keep them 0 (ctor 00695CE0 zeroes the 0x30 block; only spell casting fills them).
namespace {
void unload_spell_model(void* visual, unsigned offset) noexcept(false) {
    void* const model = rp(visual, offset);                            // 00695D13 / 00695D36 / 00695D5C ...
    if (model == nullptr) return;
    gob_methods(model)->s050_gob_attach_to_object(model, nullptr, nullptr, nullptr, 0);   // [vtbl+50h](0,0,0)
    void* const again = rp(visual, offset);                            // reload
    if (again != nullptr) gob_methods(again)->s000_gob_destruct_gob(as_gob(again), nullptr, 1);   // [vtbl](1)
    st<void*>(visual, offset, nullptr);
}
}
void __fastcall kotor_m15e_spell_cast_visual_unload_conjure_visual_00695d10(void* self, void*) noexcept(false) {
    unload_spell_model(self, 0x00);                                    // 00695D13..00695D30
    unload_spell_model(self, 0x04);                                    // 00695D36..00695D55
    unload_spell_model(self, 0x08);                                    // 00695D5C..00695D7B
}
void __fastcall kotor_m15e_spell_cast_visual_unload_cast_visual_00695d90(void* self, void*) noexcept(false) {
    unload_spell_model(self, 0x0C);                                    // 00695D93..00695DB2
    unload_spell_model(self, 0x10);                                    // 00695DB9..00695DD8
    unload_spell_model(self, 0x14);                                    // 00695DDF..00695DFE
}
// 00695F20 CSWCSpellCastVisual::~CSWCSpellCastVisual RET.
void __fastcall kotor_m15e_spell_cast_visual_destruct_00695f20(void* self, void*) noexcept(false) {
    kotor_m15e_spell_cast_visual_unload_conjure_visual_00695d10(self, nullptr);   // 00695F23
    kotor_m15e_spell_cast_visual_unload_cast_visual_00695d90(self, nullptr);      // 00695F2B jmp
}

// 0063E800 CSWCObject::~CSWCObject RET.
void __fastcall kotor_m15e_object_destruct_0063e800(void* self, void*) noexcept(false) {
    st<const void*>(self, 0, &kotor_m15e_object_methods_007516c8);   // 0063E81F [this] = 007516C8
    kotor_m15e_client_remove_from_hitcheck_ignore_list_005ed760(client_app(), nullptr, rd32(self, 4));   // 0063E825..0063E83A
    kotor_m15e_object_kill_all_visual_effects_0063dae0(self, nullptr); // 0063E83F..0063E841
    kotor_m15e_client_ai_master_remove_object_00604a30(
        kotor_m15e_client_get_client_ai_master_005ed6b0(client_app(), nullptr), nullptr, self);   // 0063E846..0063E857
    if (rp(self, 0xC8) != nullptr) {                                   // 0063E85C..0063E866
        delete_object(rp(self, 0xC8));                                 // 0063E868..0063E86C [vtbl](1)
        st<void*>(self, 0xC8, nullptr);                                // 0063E86E
    }
    void* const stream = rp(self, 0xCC);                               // 0063E874
    if (stream != nullptr) {                                           // 0063E87A..0063E87C
        kotor_fastcall_005d6200(stream, nullptr);                      // 0063E87E..0063E880 ~CExoStreamingSoundSource
        live_free(stream);                                             // 0063E885..0063E886
        st<void*>(self, 0xCC, nullptr);                                // 0063E88E
    }
    if (rp(self, 0x5C) != nullptr) {                                   // 0063E894..0063E899
        live_free(rp(self, 0x5C));                                     // 0063E89B..0063E89C
        st<void*>(self, 0x5C, nullptr);                                // 0063E8A4
    }
    void* const server = rp(self, 0xF8);                               // 0063E8A7
    if (server != nullptr) {                                           // 0063E8AD..0063E8AF
        kotor_m15e_server_object_detach_from_client_object_004cc320(server, nullptr);   // 0063E8B1
        st<void*>(self, 0xF8, nullptr);                                // 0063E8B6
    }
    static_cast<String*>(at(self, 0xE8))->~StartupString();            // 0063E8BC..0063E8C7 ~CExoString [this+E8h]
    kotor_m15e_slot090_void0_004015a0(at(self, 0xA8), nullptr);               // 0063E8CC..0063E8D7 ~CSWPortrait [this+A8h] (004015A0 `ret`)
    live_free(rp(self, 0x70));                                         // 0063E8DC..0063E8E5 free([this+70h])
    st<void*>(self, 0x70, nullptr);                                    // 0063E8EA
    void* const list = rp(self, 0x6C);                                 // 0063E8ED
    if (list != nullptr) {                                             // 0063E8F3..0063E8F9
        base_ini_reconstruction::destroy_list_005e9ab0(static_cast<kotor_list::Internal*>(list), nullptr);   // 0063E8FB..0063E8FD
        live_free(list);                                               // 0063E902..0063E903
    }
    st<const void*>(self, 0, &kotor_vtable_cgameobject_007457a0);      // 0063E910 [this] = 007457A0 (CGameObject)
}

// 006170D0 CSWCCreature::~CSWCCreature RET.
void __fastcall kotor_m15e_creature_destruct_006170d0(void* self, void*) noexcept(false) {
    st<const void*>(self, 0, &kotor_m15e_creature_methods_0074f5f8);  // 006170EF [this] = 0074F5F8
    if (rp(self, 0x430) != nullptr) delete_object(rp(self, 0x430));    // 006170F5..0061710D rolling-footstep CExoSoundSource [vtbl](1)
    void* const abilities = rp(self, 0x3A0);                           // 0061710F
    if (abilities != nullptr) {                                        // 00617115..00617117
        live_free(rp(abilities, 0));                                   // 00617119..0061711C free([p])
        st<void*>(abilities, 0, nullptr);                              // 00617122 [p] = 0
        live_free(abilities);                                          // 00617121..00617124 free(p)
    }
    if (rp(self, 0x36C) != nullptr) {                                  // 0061712C..00617134 CSoundSet
        delete_object(rp(self, 0x36C));                                // 00617136..0061713A
        st<void*>(self, 0x36C, nullptr);                               // 0061713C
    }
    if (rp(self, 0x2F8) != nullptr) {                                  // 00617142..0061714A CSWCLevelUpStats (table 00751C64)
        void* const stats = rp(self, 0x2F8);
        (*static_cast<const KotorM15bStatsMethods* const*>(stats))->scalar_deleting_destructor(stats, nullptr, 1);   // 0061714C..00617150
        st<void*>(self, 0x2F8, nullptr);                               // 00617152
    }
    if (rp(self, 0x15C) != nullptr) {                                  // 00617158..00617160
        delete_object(rp(self, 0x15C));                                // 00617162..00617166
        st<void*>(self, 0x15C, nullptr);                               // 00617168
    }
    if (rp(self, 0x17C) != nullptr) {                                  // 0061716E..00617176 looping creature sound
        delete_object(rp(self, 0x17C));                               // 00617178..0061717C
        st<void*>(self, 0x17C, nullptr);                               // 0061717E
    }
    void* const visual = rp(self, 0x2F0);                              // 00617184 CSWCSpellCastVisual
    if (visual != nullptr) {                                           // 0061718A..0061718C
        kotor_m15e_spell_cast_visual_destruct_00695f20(visual, nullptr);   // 0061718E..00617190
        live_free(visual);                                             // 00617195..00617196
    }
    void* const appearance = rp(self, 0x21C);                          // 0061719E CSWCCreatureAppearance
    if (appearance != nullptr) {                                       // 006171A4..006171A6
        kotor_m15e_slot090_void0_004015a0(appearance, nullptr);               // 006171A8..006171AA ~CSWCCreatureAppearance (004015A0 `ret`)
        live_free(appearance);                                         // 006171AF..006171B0
    }
    if (rp(self, 0x1BC) != nullptr) live_free(rp(self, 0x1BC));        // 006171B8..006171C3
    kotor_m15e_slot090_void0_004015a0(self, nullptr);                         // 006171CB..006171CD call 004015A0 (inlined member dtor, `ret`)
    void* anim = anim_base(self);                                      // 006171D2
    if (anim != nullptr) {                                             // 006171D5..006171D7
        anim_methods(anim)->s078(anim, nullptr);                       // 006171D9..006171DB anim +78h UnloadModel()
        anim = anim_base(self);                                        // 006171DE
        if (anim != nullptr) anim_methods(anim)->s000(anim, nullptr, 1);   // 006171E1..006171E9 anim [vtbl](1)
        st<void*>(self, 0x68, nullptr);                                // 006171EB
    }
    if (rp(self, 0x2B8) != nullptr) live_free(rp(self, 0x2B8));        // 006171EE..006171F9 CSWCCombatAttackData (no dtor)
    if (rp(self, 0x2BC) != nullptr) live_free(rp(self, 0x2BC));        // 00617201..0061720C
    for (unsigned offset = 0xFC; offset < 0xFC + 0x0C * 4; offset += 4) {   // 00617215..00617232 (ebp = 0Ch)
        if (rp(self, offset) != nullptr) delete_object(rp(self, offset));   // 00617220..0061722A
        st<void*>(self, offset, nullptr);                              // 0061722C
    }
    if (rp(self, 0x12C) != nullptr) delete_object(rp(self, 0x12C));    // 00617234..00617242 (not cleared)
    for (unsigned offset = 0x160; offset < 0x160 + 4 * 4; offset += 4) {   // 00617244..0061726E (ebp = 4)
        void* const model = rp(self, offset);                          // 00617250
        if (model != nullptr) {                                        // 00617252..00617254
            (*static_cast<const Slot4cView* const*>(model))->s04c(model, nullptr, 0);   // 00617256..00617259 [vtbl+4Ch](0)
            void* const again = rp(self, offset);                      // 0061725C
            if (again != nullptr) delete_object(again);                // 0061725E..00617266
            st<void*>(self, offset, nullptr);                          // 00617268
        }
    }
    void* const monk = rp(self, 0x170);                                // 00617270
    if (monk != nullptr) {                                             // 00617276..00617279
        (*static_cast<const Slot4cView* const*>(monk))->s04c(monk, nullptr, 0);   // 0061727B..0061727E [vtbl+4Ch](0)
        void* const again = rp(self, 0x170);                           // 00617281
        if (again != nullptr) delete_object(again);                    // 00617287..0061728F (not cleared)
    }
    if (rp(self, 0x130) != nullptr) delete_object(rp(self, 0x130));    // 00617291..0061729F
    if (rp(self, 0x134) != nullptr) delete_object(rp(self, 0x134));    // 006172A1..006172AF
    void* const bark = rp(self, 0x30C);                                // 006172B1
    if (bark != nullptr) {                                             // 006172B7..006172B9
        static_cast<String*>(at(bark, 0x18))->~StartupString();        // 006172BB..006172BE ~CExoString [p+18h]
        live_free(bark);                                               // 006172C3..006172C4
        st<void*>(self, 0x30C, nullptr);                               // 006172CC
    }
    st<U>(self, 0x3C8, menu_tables_m41::cells::invalid_object_00746644);   // 006172D2..006172D8 [00746644] (7F000000h)
    kotor_m15e_creature_clear_visibility_list_006145d0(self, nullptr); // 006172DE..006172E0
    kotor_m15e_client_clear_cached_creatures_005edf50(client_app(), nullptr, self);   // 006172E5..006172EF
    live_free(rp(self, 0x360));                                        // 006172F4..006172FF
    st<void*>(self, 0x360, nullptr);                                   // 00617309
    kotor_m15e_object_destruct_0063e800(self, nullptr);                // 00617317 ~CSWCObject
}

// 00618280 CSWCCreature scalar deleting destructor (slot +00) RET 4.
void* __fastcall kotor_m15e_creature_scalar_deleting_destructor_00618280(void* self, void*, U flags) noexcept(false) {
    kotor_m15e_creature_destruct_006170d0(self, nullptr);              // 00618283
    if ((flags & 1u) != 0) live_free(self);                            // 00618288..00618290
    return self;                                                       // 00618298
}

// ======================================================================================= model load + callbacks
// Animation-event callbacks registered by RegisterCallbacks 0061AB40 (cdecl(gob, event, user data); the Gob's
// DoEvent 00486560 calls them for registered names).
//
// CDECL callbacks consume the third word, as the original Gob callback ABI does.
void __cdecl kotor_m15e_creature_footstep_event_0061a970(void*, void*, aurora_model::Part* user) {
    kotor_fastcall_0061a2d0(user, nullptr); // 0061A970 -> 0061A2D0
}
void __cdecl kotor_m15e_creature_hit_event_00617eb0(void*, void*, aurora_model::Part* attack_user) {
    void* const attack = attack_user;
    void* defender = nullptr;
    void* attacker = nullptr;
    if (attack != nullptr) {
        defender = kotor_fastcall_005ed580(client_app(), nullptr, rd32(attack, 0x14));
        attacker = kotor_fastcall_005ed580(client_app(), nullptr, rd32(attack, 4));
        if (attacker == nullptr || defender == nullptr) return;
    }
    // Null attack reaches the original null virtual dispatch; it is not silently ignored.
    if (object_methods(attacker)->s02c_as_creature(attacker, nullptr) == nullptr) return;
    void* const creature = object_methods(attacker)->s02c_as_creature(attacker, nullptr);
    if (rd32(attack, 0x20) != 0) return;
    const U8 result = ld<U8>(attack, 0x0C);
    bool parried = result == 1 || result == 2 || result == 3 || result == 5;
    if (!parried) {
        if (result == 8)
            parried = kotor_animation_sound_animation_parry_0063c4f0(defender, nullptr,
                kotor_m15a_object_get_current_animation_0063c060(defender, nullptr)) != 0;
        if (!parried && ld<U8>(attack, 0x0C) == 4)
            parried = kotor_animation_sound_animation_parry_0063c4f0(defender, nullptr,
                kotor_m15a_object_get_current_animation_0063c060(defender, nullptr)) != 0;
    }
    if (parried && !(result == 1 || result == 2 || result == 3 || result == 5)) {
        kotor_fastcall_00617470(creature, nullptr, attack, 0);
        I32 abort_on_parry;
        {
            String column("ABORTONPARRY");
            c2da_current_get_int_00413660(static_cast<c2da_current::Table*>(rp(rp(rules(), 0xB8), 0x0C)), nullptr,
                ld<U16>(rp(creature, 0x21C), 0x18), &column, &abort_on_parry);
        }
        if (abort_on_parry != 0) {
            kotor_fastcall_006116a0(creature, nullptr, 0);
            kotor_m15a_object_abort_fire_forget_animation_0063cad0(creature, nullptr, 0);
        }
        if (rd32(attack, 0x40) != 0)
            kotor_m15a_object_abort_fire_forget_animation_0063cad0(defender, nullptr, 0);
    } else if (result == 1 || result == 2 || result == 3 || result == 5) {
        kotor_fastcall_00617470(creature, nullptr, attack, 0);
        if (object_methods(defender)->s02c_as_creature(defender, nullptr) != nullptr) {
            void* const victim = object_methods(defender)->s02c_as_creature(defender, nullptr);
            if (kotor_fastcall_0060d320(victim, nullptr) != 0) {
                void* const server = kotor_m15a_creature_get_server_creature_0060fb20(victim, nullptr);
                if (server == nullptr || w5x_creature_is_debilitated_005b4880(server, nullptr) == 0) {
                    if (ld<U16>(attack, 0x1E) != 0 || ld<U8>(victim, 0x218) != 1) {
                        void* const anim = anim_base(defender);
                        anim_methods(anim)->s044(anim, nullptr, 0x283E, 1);
                    }
                }
            }
        }
        if (object_methods(defender)->s02c_as_creature(defender, nullptr) != nullptr) {
            void* const victim = object_methods(defender)->s02c_as_creature(defender, nullptr);
            if (ld<std::int16_t>(rp(victim, 0x2F8), 0x4C) > 0 && rd32(attack, 0x24) == 0) {
                void* const leader = kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr);
                if ((leader != nullptr && rd32(defender, 4) == rd32(leader, 4)) || crt_rand_006fc447() % 5 == 0) {
                    const U sound = crt_rand_006fc447() % 2 == 0 ? 14u : 13u;
                    recovered_client_creature_play_sound_set_sound_00611470(
                        object_methods(defender)->s02c_as_creature(defender, nullptr), nullptr, sound, 1);
                }
            }
        }
    }
    const U8 final_result = ld<U8>(attack, 0x0C);
    if (final_result != 1 && final_result != 2 && final_result != 3 && final_result != 5) return;
    if (object_methods(defender)->s02c_as_creature(defender, nullptr) != nullptr) {
        String hook("impact");
        kotor_scene3d::Vector direction;
        // FLD/FSub DWORD then FSTP DWORD for x/y/z, preserving the original precision boundary.
        direction.z = static_cast<float>(static_cast<double>(ld<float>(creature, 0x2C)) - ld<float>(defender, 0x2C));
        direction.y = static_cast<float>(static_cast<double>(ld<float>(creature, 0x28)) - ld<float>(defender, 0x28));
        direction.x = static_cast<float>(static_cast<double>(ld<float>(creature, 0x24)) - ld<float>(defender, 0x24));
        object_projectile_0060d730(object_methods(defender)->s02c_as_creature(defender, nullptr), nullptr,
            1, attack, &direction, &hook);
        return;
    }
    if (object_methods(defender)->s014(defender, nullptr) == 0 || rp(defender, 0x20) == nullptr) return;
    float position[3] = {0, 0, 0};
    float orientation[4] = {1, 0, 0, 0};
    void* const anim = anim_base(defender);
    void* const model = anim_methods(anim)->s008(anim, nullptr, 0xFF);
    gob_methods(model)->s098_gob_get_part_position(as_gob(model), nullptr, word_of("lookathook"), word_of(position), word_of(orientation));
    auto* volatile emit_vfx = &raw_area_vfx_words;
    emit_vfx(defender, 0xFA3, position);
    emit_vfx(defender, 0xFAB, position);
}
void __cdecl kotor_m15e_creature_hit_ground_event_0060b400(void*, void*, aurora_model::Part* user) {
    String sound;
    void* const creature = user;
    if (creature == nullptr) return;
    I32 row;
    c2da_current_get_int_entry_00413510(static_cast<c2da_current::Table*>(rp(rp(rules(), 0xB8), 0x0C)), nullptr,
        ld<U16>(rp(creature, 0x21C), 0x18), static_cast<I32>(kotor_rules_cell32_007a2128), &row);
    U surface = 0;
    if (rp(creature, 0x20) != nullptr) {
        auto* volatile terrain = &raw_terrain_words;
        surface = terrain(rp(creature, 0x20), at(creature, 0x24));
    }
    const char* column_name = nullptr;
    // Original index bytes0060B620 followed by targettable0060B60C.
    switch (surface - 1u) {
    case 0: case 2: case 13: column_name = "FallDirt"; break;
    case 3: case 4: case 8: case 14: column_name = "FallHard"; break;
    case 5: case 10: case 11: case 12: column_name = "FallWater"; break;
    case 9: column_name = "FallMetal"; break;
    default: break;
    }
    if (column_name != nullptr) {
        String column(column_name);
        c2da_current_get_string_00413270(static_cast<c2da_current::Table*>(rp(rp(rules(), 0xB8), 0x5C)), nullptr, row, &column, &sound);
    }
    Ref ref;
    ref.construct_00406d80(async_string_cstr_005e5670(&sound, nullptr));
    auto* volatile play_sound = &raw_sound_words;
    play_sound(&kotor_cell_sound_007a39ec, &ref, at(creature, 0x24), 0, 0, 0);
}
void __cdecl kotor_m15e_creature_swing_short_event_00610c90(void*, void*, aurora_model::Part* user) {
    String event("SwingShort");
    if (user == nullptr) return;
    void* const target = frame_R4_005ed5b0(client_app(), nullptr, rd32(user, 4));
    if (target != nullptr) kotor_m15closure_0060dc50(target, nullptr, &event, user, 3);
}
void __cdecl kotor_m15e_creature_swing_long_event_00610d10(void*, void*, aurora_model::Part* user) {
    String event("SwingLong");
    if (user == nullptr) return;
    void* const target = frame_R4_005ed5b0(client_app(), nullptr, rd32(user, 4));
    if (target != nullptr) kotor_m15closure_0060dc50(target, nullptr, &event, user, 3);
}
void __cdecl kotor_m15e_creature_swing_twirl_event_00610d90(void*, void*, aurora_model::Part* user) {
    String event("SwingTwirl");
    if (user == nullptr) return;
    void* const target = frame_R4_005ed5b0(client_app(), nullptr, rd32(user, 4));
    if (target != nullptr) kotor_m15closure_0060dc50(target, nullptr, &event, user, 3);
}
void __cdecl kotor_m15e_creature_hit_clash_event_00610e10(void*, void*, aurora_model::Part* user) {
    String event("Clash");
    if (user == nullptr) return;
    void* const target = frame_R4_005ed5b0(client_app(), nullptr, rd32(user, 4));
    if (target != nullptr) kotor_m15closure_0060dc50(target, nullptr, &event, user, 2);
}
void __cdecl kotor_m15e_creature_hit_contact_event_00610e90(void*, void*, aurora_model::Part* user) {
    if (user == nullptr) return;
    void* const target = frame_R4_005ed5b0(client_app(), nullptr, rd32(user, 4));
    if (target != nullptr) kotor_m15closure_0060de20(target, nullptr, user);
}
void __cdecl kotor_m15e_creature_hit_parry_event_00610ec0(void*, void*, aurora_model::Part* user) {
    String event("Parry");
    if (user == nullptr) return;
    void* const target = frame_R4_005ed5b0(client_app(), nullptr, rd32(user, 4));
    if (target != nullptr) kotor_m15closure_0060dc50(target, nullptr, &event, user, 2);
}
void __cdecl kotor_m15e_creature_blur_00616a00(void*, void*, aurora_model::Part* user) {
    kotor_fastcall_006116a0(user, nullptr, 1);
}
void __cdecl kotor_m15e_creature_unblur_00616a10(void*, void*, aurora_model::Part* user) {
    kotor_fastcall_006116a0(user, nullptr, 0);
}

// 0060E120 GetPersonalRadius(gob, event, float* out) __cdecl: appearance.2da "perspace" of the creature
// (external data of the gob) -> *out; *out untouched when the gob has no creature.
void __cdecl kotor_m15e_get_personal_radius_0060e120(void* gob, void* event, aurora_model::Part* data) {
    (void)event;
    float value = 0.0f;                                                // [esp] 0060E127 = 0
    void* const creature = word_ptr(gob_methods(gob)->s070_gob_get_external_data(as_gob(gob), nullptr));   // 0060E121..0060E12F gob +70h
    if (creature == nullptr) return;                                   // 0060E132..0060E134
    kotor_m15b_c2da_get_float_entry_00413350(rp(rp(rules(), 0xB8), 0x0C), nullptr,
        ld<U16>(rp(creature, 0x21C), 0x18), static_cast<std::int32_t>(kotor_rules_cell32_007a2118), &value);   // 0060E136..0060E15C
    std::memcpy(static_cast<void*>(data), &value, 4);                  // 0060E161..0060E169 fld/fstp dword [out]
}
// 0060E170 GetCreatureRadius(gob, event, float* out) __cdecl: appearance.2da column [007A20D0] -> *out.
void __cdecl kotor_m15e_get_creature_radius_0060e170(void* gob, void* event, aurora_model::Part* data) {
    (void)event;
    float value = 0.0f;                                                // 0060E177
    void* const creature = word_ptr(gob_methods(gob)->s070_gob_get_external_data(as_gob(gob), nullptr));   // 0060E171..0060E17F
    if (creature == nullptr) return;                                   // 0060E182..0060E184
    kotor_m15b_c2da_get_float_entry_00413350(rp(rp(rules(), 0xB8), 0x0C), nullptr,
        ld<U16>(rp(creature, 0x21C), 0x18), static_cast<std::int32_t>(kotor_rules_cell32_007a20d0), &value);   // 0060E186..0060E1AC
    std::memcpy(static_cast<void*>(data), &value, 4);                  // 0060E1B1..0060E1B9
}
// 0060E1C0 GetPath(gob, event, path* out) __cdecl: [out+4] = [out+8] = 0.
void __cdecl kotor_m15e_get_path_0060e1c0(void* gob, void* event, aurora_model::Part* data) {
    (void)gob; (void)event;
    st<U>(data, 4, 0);                                                 // 0060E1C6
    st<U>(data, 8, 0);                                                 // 0060E1C9
}

// 0061AB40 CSWCCreature::RegisterCallbacks RET: registers the 16 named callbacks on the body gob; handles stored at
// [this+3ECh..+42Ch] (+3F4h untouched).
void __fastcall kotor_m15e_creature_register_callbacks_0061ab40(void* self, void*) noexcept(false) {
    void* const anim = anim_base(self);                                // 0061AB43
    void* const gob = anim_methods(anim)->s008(anim, nullptr, 0xFF);   // 0061AB46..0061AB4E anim +08h GetModel(FFh)
    if (gob == nullptr) return;                                        // 0061AB51..0061AB55
    using Callback = void (__cdecl*)(void*, void*, aurora_model::Part*);
    auto reg = [&](Callback fn, const char* name, void* user, U time) -> U {
        return gob_methods(gob)->register_callback(as_gob(gob), nullptr, fn, name,
                                                   static_cast<aurora_model::Part*>(user), time, 0);   // push 0, time, user, name, fn
    };
    const U k50 = 0x42480000u, k10000 = 0x461C3C00u;                   // 50.0f / 10000.0f
    st<U>(self, 0x3EC, reg(&kotor_m15e_creature_footstep_event_0061a970, "snd_Footstep", self, k50));        // 0061AB5B..0061AB87 (74F838)
    st<U>(self, 0x3F0, reg(&kotor_m15e_creature_hit_event_00617eb0, "hit", rp(self, 0x2B8), k10000));       // 0061AB74..0061ABA6 (74F834, [this+2B8h])
    st<U>(self, 0x3F8, reg(&kotor_m15e_creature_hit_ground_event_0060b400, "snd_hitground", self, k10000)); // 0061AB99..0061ABC5 (74F824)
    st<U>(self, 0x3FC, reg(&kotor_m15e_creature_swing_short_event_00610c90, "SwingShort", self, k10000));  // 0061ABB8..0061ABE4 (74F48C)
    st<U>(self, 0x400, reg(&kotor_m15e_creature_swing_long_event_00610d10, "SwingLong", self, k10000));    // 0061ABD7..0061AC03 (74F498)
    st<U>(self, 0x404, reg(&kotor_m15e_creature_swing_twirl_event_00610d90, "SwingTwirl", self, k10000));  // 0061ABF6..0061AC22 (74F4A4)
    st<U>(self, 0x408, reg(&kotor_m15e_creature_hit_clash_event_00610e10, "Clash", self, k10000));         // 0061AC15..0061AC41 (74F4B0)
    st<U>(self, 0x40C, reg(&kotor_m15e_creature_hit_contact_event_00610e90, "Contact", self, k10000));     // 0061AC34..0061AC60 (74F81C)
    st<U>(self, 0x410, reg(&kotor_m15e_creature_hit_parry_event_00610ec0, "HitParry", self, k10000));      // 0061AC53..0061AC7F (74F810)
    st<U>(self, 0x414, reg(&kotor_m15e_creature_blur_00616a00, "blur_start", self, k10000));               // 0061AC72..0061AC9E (74F804)
    st<U>(self, 0x418, reg(&kotor_m15e_creature_unblur_00616a10, "blur_end", self, k10000));               // 0061AC91..0061ACBD (74F7F8)
    st<U>(self, 0x41C, reg(&kotor_m15e_creature_unblur_00616a10, "doneattack01", self, k10000));           // 0061ACB0..0061ACDC (74F7E8)
    st<U>(self, 0x420, reg(&kotor_m15e_creature_unblur_00616a10, "doneattack02", self, k10000));           // 0061ACCF..0061ACFB (74F7D8)
    st<U>(self, 0x424, reg(&kotor_m15e_get_personal_radius_0060e120, "GetPersonalRadius", self, k10000));  // 0061ACEE..0061AD1A (742F30)
    st<U>(self, 0x428, reg(&kotor_m15e_get_creature_radius_0060e170, "GetCreatureRadius", self, k10000));  // 0061AD0D..0061AD39 (742F1C)
    st<U>(self, 0x42C, reg(&kotor_m15e_get_path_0060e1c0, "GetPath", self, k10000));                       // 0061AD2C..0061AD4B (742F14)
}

// 0061B380 CSWCCreature::LoadModel(const CResRef*, BYTE part, BYTE anim_type) (slot +78) RET 0Ch -> 1 / 0.
// Only the low byte of the anim_type dword is used (0061B3E2 mov bl,[esp+144h]); the appearance builders pass a
// dword whose low byte is the type (lane-m15-a note, CreateBodyAppearance 00698CAF).
U __fastcall kotor_m15e_creature_load_model_0061b380(void* self, void*, void* resref, U part, U model_type) noexcept(false) {
    if (rp(self, 0x358) != nullptr) {                                  // 0061B39F..0061B3AA pending anim base
        void* const old = anim_base(self);                             // 0061B3AC
        if (old != nullptr) {                                          // 0061B3AF..0061B3B1
            anim_methods(old)->s000(old, nullptr, 1);                  // 0061B3B3..0061B3B7 [vtbl](1)
            st<void*>(self, 0x68, nullptr);                            // 0061B3B9
        }
        st<void*>(self, 0x68, rp(self, 0x358));                        // 0061B3BC..0061B3C8
        st<void*>(self, 0x358, nullptr);                               // 0061B3CB
        st<U>(self, 0x1F8, rd32(self, 0x35C));                         // 0061B3C2 / 0061B3D1
        st<U>(self, 0x35C, 0);                                         // 0061B3D7
    }
    const U8 type = static_cast<U8>(model_type);                       // 0061B3E2 bl
    void* anim = anim_base(self);                                      // 0061B3DD
    bool reuse = false;
    if (anim != nullptr) {                                             // 0061B3E0..0061B3E9
        if (ld<U8>(anim, 0xC4) == type) reuse = true;                  // 0061B3EB..0061B3F1 je 0061B5A7
        else {
            anim_methods(anim)->s000(anim, nullptr, 1);                // 0061B3FB..0061B3FF [vtbl](1)
            st<void*>(self, 0x68, nullptr);                            // 0061B401
        }
    }
    if (!reuse) {
        // 0061B404..0061B417 switch on the type byte (index table 0061B6E4, jump table 0061B6CC):
        //   0 -> CSWCAnimBase (F0h), 1 -> CSWCAnimBaseHead (1C4h), 2 -> CSWCAnimBaseWield (1D0h),
        //   3 -> CSWCAnimBaseHeadWield (220h), 0Bh -> CSWCAnimBaseTW (174h), 4..0Ah / > 0Bh -> 0061B5DD (return 0).
        // Char-gen creatures only reach type 3: every appearance they use comes from portraits.2da forpc rows
        // (AppearanceNumber/Appearance_S/Appearance_L, class-selection ctor 006DCA62..006DCB64; the portrait /
        // main char-gen creatures use the same rows) = appearance.2da rows 91..180, all modeltype "B", which
        // GetAnimType 00696D40 maps to 3 (00696DC2) and CreateBodyAppearance 00698CB5 routes to CreateBTypeBody.
        void* created = nullptr;
        switch (type) {
        case 0: {
            void* const memory = live_operator_new(0xF0);
            if (memory != nullptr) {
                void* object;
                // Original states0..4 own only the raw allocation during the constructor.
                try { object = kotor_m15e_anim_base_construct_0069dfb0(memory, nullptr); }
                catch (...) { live_free(memory); throw; }
                created = object;
            }
            break;
        }
        case 1: {
            void* const memory = live_operator_new(0x1C4);
            if (memory != nullptr) {
                void* object;
                // Original states0..4 own only the raw allocation during the constructor.
                try { object = kotor_m15e_anim_base_head_construct_0069bb80(memory, nullptr, 1); }
                catch (...) { live_free(memory); throw; }
                if (object != nullptr)
                    created = static_cast<unsigned char*>(object) + ld<I32>(rp(object, 0), 4);
            }
            break;
        }
        case 2: {
            void* const memory = live_operator_new(0x1D0);
            if (memory != nullptr) {
                void* object;
                // Original states0..4 own only the raw allocation during the constructor.
                try { object = kotor_m15e_anim_base_wield_construct_00699dd0(memory, nullptr, 1); }
                catch (...) { live_free(memory); throw; }
                if (object != nullptr)
                    created = static_cast<unsigned char*>(object) + ld<I32>(rp(object, 0), 4);
            }
            break;
        }
        case 3: {
            void* const memory = live_operator_new(0x220);
            if (memory != nullptr) {
                void* object;
                // Original states0..4 own only the raw allocation during the constructor.
                try { object = kotor_m15e_anim_base_head_wield_construct_00698ec0(memory, nullptr, 1); }
                catch (...) { live_free(memory); throw; }
                if (object != nullptr)
                    created = static_cast<unsigned char*>(object) + ld<I32>(rp(object, 0), 4);
            }
            break;
        }
        case 11: {
            void* const memory = live_operator_new(0x174);
            if (memory != nullptr) {
                void* object;
                // Original states0..4 own only the raw allocation during the constructor.
                try { object = kotor_m15e_anim_base_tw_construct_0069cbd0(memory, nullptr); }
                catch (...) { live_free(memory); throw; }
                created = object;
            }
            break;
        }
        default:
            return 0;                                                  // 0061B40A ja / table 5 -> 0061B5DD
        }
        st<void*>(self, 0x68, created);                                // 0061B55F
        anim = anim_base(self);                                        // 0061B562
        if (anim == nullptr) return 0;                                 // 0061B565..0061B567
        kotor_m15e_sw_anim_base_set_00698e30(anim, nullptr, 0, 0x44E74000u);   // 0061B569..0061B56F Set(0, 1850.0f)
        kotor_m15e_sw_anim_base_set_00698e30(anim_base(self), nullptr, 1, 0x45CE4000u);   // 0061B574..0061B57E Set(1, 6600.0f)
        kotor_m15e_sw_anim_base_set_00698e30(anim_base(self), nullptr, 2, 0x3F6CCCCDu);   // 0061B583..0061B58D Set(2, 0.925f)
        kotor_m15e_sw_anim_base_set_00698e30(anim_base(self), nullptr, 3, 0x40533333u);   // 0061B592..0061B59C Set(3, 3.3f)
        st<U>(self, 0x1F8, 0);                                         // 0061B5A1
    }
    anim = anim_base(self);                                            // 0061B5AE
    if (anim_methods(anim)->s00c(anim, nullptr, resref, part) == 0) {  // 0061B5A7..0061B5C1 anim +0Ch LoadModel(resref, part)
        char message[0x100];                                           // [esp+2Ch..12Bh] (sub esp,120h; formatted, never used)
        format_006fadb0(message, "CSWCCreature::LoadModel(): Failed to load creature model '%s'.",
                        c2da_ref_00405fe0(static_cast<Ref*>(resref), nullptr));   // 0061B5C3..0061B5D5 (0074F85C)
        return 0;                                                      // 0061B5DD
    }
    const U8 which = static_cast<U8>(part);                            // 0061B5F9.. bl
    if (which == 0xFF || which == 0xFE || which == 0xFD || which == 0xFC) {   // 0061B5F9..0061B60B
        void* const anim2 = anim_base(self);                           // 0061B611
        void* const gob = anim_methods(anim2)->s008(anim2, nullptr, part);   // 0061B614..0061B617 anim +08h GetModel(part)
        gob_methods(gob)->s074_gob_set_external_data(as_gob(gob), nullptr, word_of(self));   // 0061B61C..0061B621 gob +74h
        gob_methods(gob)->s07c_gob_set_external_data_type(as_gob(gob), nullptr,
                                                          kotor_m15e_external_data_type_00746634);   // 0061B624..0061B630 gob +7Ch (byte [00746634] = 5)
        if (which == 0xFF) {                                           // 0061B633..0061B636
            float v[3] = {0.0f, 0.0f, 0.0f};                           // [esp+10h..18h] 0061B65E..0061B66E
            float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};                     // [esp+1Ch..28h] 0061B656 / 0061B646..0061B63E
            kotor_m15e_creature_register_callbacks_0061ab40(self, nullptr);   // 0061B63C..0061B676
            if (gob_methods(gob)->s0a0_gob_get_part_local_position(as_gob(gob), nullptr, word_of("headconjure"),
                                                                    word_of(v), word_of(q)) != 0) {   // 0061B67B..0061B696 gob +A0h (0074F850)
                const double z = static_cast<double>(v[2]);             // 0061B698 fld [esp+18h]
                st<float>(self, 0xA4, static_cast<float>(z - z * static_cast<double>(0.125f)));   // 0061B6A1..0061B6AB fmul [0073F400]; fsubr; fstp
                return 1;                                              // 0061B69C
            }
            st<U>(self, 0xA4, 0x40066666u);                            // 0061B6B6 2.1f
        }
    }
    return 1;                                                          // 0061B6C0
}
// 006A6950..006A69D1 CSWCVisualEffectOnObject::Stop, no result.
void __fastcall kotor_m15closure_006a6950(void* self, void*) {
    const U8 kind = ld<U8>(self, 0xC6);
    U16 id;
    bool selected = true;
    if (kind == 1) id = ld<U16>(self, 0xA0);
    else if (kind == 2) id = ld<U16>(self, 0xA2);
    else if (kind == 3) id = ld<U16>(self, 0xA4);
    else { selected = false; id = 0; }
    if (selected && id != 0xFFFF) object_vfx_006a56f0(self, nullptr, id);
    object_vfx_006a0bf0(self, nullptr);
    void* const sound = rp(self, 0xEC);
    st<U8>(self, 0xC6, 0);
    st<U8>(self, 0xD9, 0);
    if (sound != nullptr) {
        kotor_m15_sound_source_stop_005d5a20(sound, nullptr);
        void* const again = rp(self, 0xEC);
        if (again != nullptr) delete_object(again);
        st<void*>(self, 0xEC, nullptr);
    }
}
// 0060DE20..0060DEC2: third stack word's payload is unused by the original.
U __fastcall kotor_m15closure_0060de20(void* self, void*, void*) {
    float position[3] = {0, 0, 0};
    float orientation[4] = {1, 0, 0, 0};
    void* const model = object_methods(self)->s098_get_model(self, nullptr, 0xFF, 1);
    const U found = gob_methods(model)->s098_gob_get_part_position(as_gob(model), nullptr,
        word_of("LightSaberHook"), word_of(position), word_of(orientation));
    if (found == 0) return 0;
    auto* volatile emit_vfx = &raw_area_vfx_words;
    emit_vfx(self, 0xFAB, position);
    return 1;
}
// 0063D700..0063D771 AcquireAnimationBase(CSWCObject* source), RET4; no logical result.
void __fastcall recovered_client_object_acquire_animation_base_0063d700(void* self, void*, void* source) {
    if (source == nullptr) return;
    void* const old = anim_base(self);
    if (old != nullptr) anim_methods(old)->s000(old, nullptr, 1);
    if (object_methods(source)->s02c_as_creature(source, nullptr) != nullptr)
        kotor_fastcall_00610550(object_methods(source)->s02c_as_creature(source, nullptr), nullptr);
    st<void*>(self, 0x68, anim_base(source));
    void* anim = anim_base(source);
    void* model = anim_methods(anim)->s008(anim, nullptr, 0xFF);
    gob_methods(model)->s074_gob_set_external_data(as_gob(model), nullptr, word_of(self));
    anim = anim_base(source);
    model = anim_methods(anim)->s008(anim, nullptr, 0xFF);
    gob_methods(model)->s07c_gob_set_external_data_type(as_gob(model), nullptr, ld<U8>(self, 8));
    st<void*>(source, 0x68, nullptr);
}
// 0060DC50..0060DE13: attack swing/clash/parry sound, RET0C.
U __fastcall kotor_m15closure_0060dc50(void* self, void*, String* event, void*, U count) {
    U row = 0;
    void* item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, rd32(self, 0x230));
    if (item != nullptr && kotor_m15b_item_get_base_item_005b4790(item, nullptr) != nullptr)
        row = ld<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0x8C);
    item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, rd32(self, 0x234));
    if (item != nullptr && kotor_m15b_item_get_base_item_005b4790(item, nullptr) != nullptr) {
        void* const server = kotor_m15closure_00646c90(item, nullptr);
        if (server != nullptr && kotor_m15closure_00555690(server, nullptr) == 1)
            row = ld<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0x8C);
    }
    const I32 index = crt_rand_006fc447() % static_cast<I32>(count);
    String sound;
    alignas(String) unsigned char column_storage[sizeof(String)];
    String* const column = reinterpret_cast<String*>(column_storage);
    resman_string_from_int_005e5bc0(column, nullptr, index);
    struct ConstructedString {
        String* value;
        ~ConstructedString() { value->~String(); }
    } column_lifetime{column};
    {
        alignas(String) unsigned char joined_storage[sizeof(String)];
        String* const joined = reinterpret_cast<String*>(joined_storage);
        String* const result = GuiConcat005e5d10(event, nullptr, joined, column);
        ConstructedString joined_lifetime{joined};
        async_assign_string_005e5c50(column, nullptr, result);
    }
    c2da_current_get_string_00413270(static_cast<c2da_current::Table*>(rp(rp(rules(), 0xB8), 0x60)), nullptr, static_cast<I32>(row), column, &sound);
    float pitch = 0;
    kotor_m15b_c2da_get_float_entry_00413350(rp(rp(rules(), 0xB8), 0x60), nullptr,
        static_cast<I32>(row), static_cast<I32>(kotor_rules_cell32_007a21a4), &pitch);
    Ref ref;
    async_ref_construct_string_00406d60(&ref, nullptr, &sound);
    auto* volatile play_sound = &raw_sound_words;
    return play_sound(&kotor_cell_sound_007a39ec, &ref, at(self, 0x24), 0x3FC00000u, 15, rd32(&pitch, 0));
}

// 00610550..0061058A: deregister17 original handlewords, including untouched+3F4.
void __fastcall kotor_fastcall_00610550(void* self, void*) {
    void* const anim = anim_base(self);
    void* const model = anim_methods(anim)->s008(anim, nullptr, 0xFF);
    if (model == nullptr) return;
    for (U offset = 0x3EC; offset != 0x430; offset += 4) {
        const U handle = rd32(self, offset);
        if (handle != 0) gob_methods(model)->s02c_gob_deregister_callback(as_gob(model), nullptr, handle);
    }
}
// 00555690..005556A9: CSWSItem doubleweapon baseitem IDs.
U __fastcall kotor_m15closure_00555690(void* self, void*) {
    const U kind = rd32(self, 0x0C);
    return kind == 8 || kind == 9 || kind == 10 ? 1u : 0u;
}
// 00646C90..00646CB1: clientitem serveritem, including repeated lookup.
void* __fastcall kotor_m15closure_00646c90(void* self, void*) {
    void* const object = at(self, 0x10);
    if (kotor_m15a_object_get_server_object_0063d4b0(object, nullptr) == nullptr) return nullptr;
    void* const server = kotor_m15a_object_get_server_object_0063d4b0(object, nullptr);
    using AsItem = void* (__fastcall*)(void*, void*);
    return ld<AsItem>(rp(server, 0), 0x38)(server, nullptr);
}
// 005D64B0..005D64D9: null wrapperinternal leavesboth outputwords untouched.
void __fastcall kotor_m15closure_005d64b0(void* self, void*, U group, float* first, float* second) {
    void* const internal = rp(self, 0);
    if (internal == nullptr) return;
    void* const entry = at(rp(internal, 0x4C), static_cast<U8>(group) * 24u);
    st<U>(first, 0, rd32(entry, 0x0C));
    st<U>(second, 0, rd32(entry, 8));
}
// 00604BF0..00604C37: the receiver is unused; server's current module area supplies terrain.
U __fastcall kotor_m15closure_00604bf0(void*, void*, KotorM15VectorWords) {
    // Canonical twelve-byte by-value ABI; retain incoming DWORDs without x87 materialization.
    const unsigned char* const position = static_cast<unsigned char*>(_AddressOfReturnAddress()) + 4;
    void* const server = rp(kotor_menu_window_cells::manager_007a39fc, 8);
    if (server == nullptr) return 0;
    void* const module = server_get_module_004ae6b0(server, nullptr);
    if (module == nullptr) return 0;
    void* const area = module_get_area_004c30f0(module, nullptr);
    if (area == nullptr) return 0;
    U result;
    __asm {
        mov eax, position
        mov ecx, dword ptr [eax]
        mov edx, dword ptr [eax + 4]
        mov eax, dword ptr [eax + 8]
        push eax
        push edx
        push ecx
        mov ecx, area
        xor edx, edx
        call kotor_m15closure_005063c0
        mov result, eax
    }
    return result;
}
// 0060DED0..0060DFFF: localplayer floorcreak chance, RET4, no logical result.
void __fastcall kotor_m15closure_0060ded0(void* self, void*, U flag) {
    if (static_cast<U8>(flag) != 0) return;
    void* const client = client_app();
    const U id = rd32(self, 4);
    if (kotor_m15_client_get_player_creature_id_005ed550(client, nullptr) != id) return;
    if (crt_rand_006fc447() % 100 >= 5) return;
    String sound;
    c2da_assign_text_005e5140(&sound, nullptr, "as_cv_florcreak");
    {
        alignas(String) unsigned char number_storage[sizeof(String)];
        String* const number = reinterpret_cast<String*>(number_storage);
        resman_string_from_int_005e5bc0(number, nullptr, crt_rand_006fc447() % 3 + 1);
        struct LiveString { String* p; ~LiveString() { p->~String(); } } number_lifetime{number};
        alignas(String) unsigned char joined_storage[sizeof(String)];
        String* const joined = reinterpret_cast<String*>(joined_storage);
        String* const result = GuiConcat005e5d10(&sound, nullptr, joined, number);
        LiveString joined_lifetime{joined};
        async_assign_string_005e5c50(&sound, nullptr, result);
    }
    Ref ref;
    async_ref_construct_string_00406d60(&ref, nullptr, &sound);
    auto* volatile play_sound = &raw_sound_words;
    play_sound(&kotor_cell_sound_007a39ec, &ref, at(self, 0x24), 0, 19, 0);
}
// 005063C0..005063FB: surface material atposition, original emptyroom returns0.
U __fastcall kotor_m15closure_005063c0(void* self, void*, KotorM15VectorWords) {
    unsigned char* const position = static_cast<unsigned char*>(_AddressOfReturnAddress()) + 4;
    void* const room = word_ptr(w2x_area_room_004bb600(self, nullptr, position, nullptr));
    if (room == nullptr) return 0;
    void* const mesh = rp(room, 0x3C);
    U result;
    __asm {
        mov eax, position
        mov ecx, dword ptr [eax]
        mov edx, dword ptr [eax + 4]
        mov eax, dword ptr [eax + 8]
        push eax
        push edx
        push ecx
        mov ecx, mesh
        xor edx, edx
        call kotor_m15closure_00581930
        mov result, eax
    }
    return result;
}
// 00581930..00581A0A: allmaterial vertical ray (distinctfromwalkcheckonly00581A10).
U __fastcall kotor_m15closure_00581930(void* self, void*, KotorM15VectorWords) {
    const unsigned char* const position = static_cast<unsigned char*>(_AddressOfReturnAddress()) + 4;
    alignas(float) unsigned char local_storage[12];
    collision_mesh_world_to_local_00596a00(self, nullptr,
        reinterpret_cast<kotor_world_los::Vector*>(local_storage),
        reinterpret_cast<const kotor_world_los::Vector*>(position));
    w5::SurfaceHit hit = {0xFFFFFFFFu, 0, 0, -1, {0, 0, 0}, 0};
    U from_words[3];
    U to_words[3];
    std::memcpy(from_words, local_storage, 8);
    std::memcpy(to_words, local_storage, 8);
    from_words[2] = 0x447A0000u; // +1000.0f raw DWORD
    to_words[2] = 0xC47A0000u;   // -1000.0f raw DWORD
    // Exact canonical fastcall ABI: hit, to.z/y/x, from.z/y/x; callee RET1C.
    __asm {
        lea eax, hit
        push eax
        lea eax, to_words
        push dword ptr [eax + 8]
        push dword ptr [eax + 4]
        push dword ptr [eax]
        lea eax, from_words
        push dword ptr [eax + 8]
        push dword ptr [eax + 4]
        push dword ptr [eax]
        mov ecx, self
        xor edx, edx
        call w5_surface_mesh_check_aabb_all_00581610
    }
    if (hit.face_c == -1) return 0;
    return rd32(rp(self, 0x64), static_cast<U>(hit.face_c) * 4u);
}
// 0060B6B0..0060B74D copyconstructor; no logical result. Preserve holes and load/store order under aliasing.
void __fastcall kotor_fastcall_0060b6b0(void* self, void*, void* source) {
    st<U8>(self, 0, ld<U8>(source, 0));
    st<U8>(self, 1, ld<U8>(source, 1));
    st<U>(self, 4, rd32(source, 4));
    st<U>(self, 0x14, rd32(source, 0x14));
    st<U16>(self, 8, ld<U16>(source, 8));
    st<U16>(self, 0x0A, ld<U16>(source, 0x0A));
    st<U8>(self, 0x0C, ld<U8>(source, 0x0C));
    st<U>(self, 0x10, rd32(source, 0x10));
    st<U16>(self, 0x1E, ld<U16>(source, 0x1E));
    st<U16>(self, 0x18, ld<U16>(source, 0x18));
    st<U16>(self, 0x1A, ld<U16>(source, 0x1A));
    st<U16>(self, 0x1C, ld<U16>(source, 0x1C));
    st<U>(self, 0x2C, rd32(source, 0x2C));
    st<U>(self, 0x30, rd32(source, 0x30));
    st<U>(self, 0x34, rd32(source, 0x34));
    st<U>(self, 0x38, rd32(source, 0x38));
    st<U>(self, 0x3C, rd32(source, 0x3C));
    st<U>(self, 0x44, rd32(source, 0x44));
    st<U>(self, 0x20, rd32(source, 0x20));
    st<U>(self, 0x24, rd32(source, 0x24));
    st<U8>(self, 0x28, ld<U8>(source, 0x28));
    st<U>(self, 0x40, rd32(source, 0x40));
}

// 0061A2D0..0061A663, RET0. Existing W4 and event callback share this one owner.
void __fastcall kotor_fastcall_0061a2d0(void* self, void*) {
    String column;
    String sound;
    float pitch = 0;
    void* const area = rp(self, 0x20);
    if (area == nullptr) return;
    I32 row;
    c2da_current_get_int_entry_00413510(static_cast<c2da_current::Table*>(rp(rp(rules(), 0xB8), 0x0C)), nullptr,
        ld<U16>(rp(self, 0x21C), 0x18), static_cast<I32>(kotor_rules_cell32_007a20ec), &row);
    if (c2da_current_get_string_entry_00413190(static_cast<c2da_current::Table*>(rp(rp(rules(), 0xB8), 0x58)), nullptr,
        row, static_cast<I32>(kotor_rules_cell32_007a219c), &sound) != 0) return;
    if (rd32(self, 0x194) == 1) row = 0;
    auto* volatile terrain = &raw_terrain_words;
    const U surface = terrain(area, at(self, 0x24));
    c2da_current_get_string_entry_00413190(static_cast<c2da_current::Table*>(rp(rp(rules(), 0xB8), 0x14)), nullptr,
        static_cast<I32>(surface), static_cast<I32>(kotor_rules_cell32_007a21b0), &column);
    if (client_movies::StringNotEqual_005e5390(reinterpret_cast<options_video::String*>(&column), nullptr, "") != 0) {
        alignas(String) unsigned char number_storage[sizeof(String)];
        String* const number = reinterpret_cast<String*>(number_storage);
        resman_string_from_int_005e5bc0(number, nullptr, crt_rand_006fc447() % 3);
        struct LiveString { String* p; ~LiveString() { p->~String(); } } number_lifetime{number};
        alignas(String) unsigned char joined_storage[sizeof(String)];
        String* const joined = reinterpret_cast<String*>(joined_storage);
        String* const result = GuiConcat005e5d10(&column, nullptr, joined, number);
        LiveString joined_lifetime{joined};
        async_assign_string_005e5c50(&column, nullptr, result);
    }
    c2da_current_get_string_00413270(static_cast<c2da_current::Table*>(rp(rp(rules(), 0xB8), 0x58)), nullptr, row, &column, &sound);
    kotor_m15b_c2da_get_float_entry_00413350(rp(rp(rules(), 0xB8), 0x58), nullptr,
        row, static_cast<I32>(kotor_rules_cell32_007a21a0), &pitch);
    const U id = rd32(self, 4); // cached before player lookup, as original0061A48C.
    U group;
    if (id == kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr)) group = 19;
    else {
        I32 appearance_type;
        c2da_current_get_int_entry_00413510(static_cast<c2da_current::Table*>(rp(rp(rules(), 0xB8), 0x0C)), nullptr,
            ld<U16>(rp(self, 0x21C), 0x18), static_cast<I32>(kotor_rules_cell32_007a2124), &appearance_type);
        group = appearance_type == 1 || appearance_type == 2 || appearance_type == 3 ? 20u : 18u;
    }
    float minimum;
    float maximum;
    kotor_m15closure_005d64b0(kotor_cell_sound_007a39ec, nullptr, group, &minimum, &maximum);
    alignas(float) unsigned char listener[12] = {};
    area_vfx_listener_position_005d63c0(kotor_cell_sound_007a39ec, nullptr,
        reinterpret_cast<kotor_w3::Vector3*>(listener));
    kotor_scene3d::Vector delta;
    delta.z = static_cast<float>(static_cast<double>(ld<float>(self, 0x2C)) - ld<float>(listener, 8));
    delta.y = static_cast<float>(static_cast<double>(ld<float>(self, 0x28)) - ld<float>(listener, 4));
    delta.x = static_cast<float>(static_cast<double>(ld<float>(self, 0x24)) - ld<float>(listener, 0));
    auto* volatile compare_distance = &raw_distance_compare;
    if ((compare_distance(&delta, &maximum) & 0x4100u) == 0) return;
    Ref ref;
    ref.construct_00406d80(async_string_cstr_005e5670(&sound, nullptr));
    auto* volatile play_sound = &raw_sound_words;
    play_sound(&kotor_cell_sound_007a39ec, &ref, at(self, 0x24), 0, group, rd32(&pitch, 0));
    if (row != 6 && row != 7) {
        if (surface == 1) {
            m15_dust_emit_00619e10(self, nullptr, 1, 0);
            st<U>(self, 0x300, 0);
        } else if (surface == 6 || surface == 11 || surface == 12 || surface == 13) {
            m15_dust_emit_00619e10(self, nullptr, 0, rd32(self, 0x300) == 0 ? 1u : 0u);
            st<U>(self, 0x300, 1);
        } else if (surface == 5) {
            kotor_m15closure_0060ded0(self, nullptr, 0);
        } else {
            st<U>(self, 0x300, 0);
        }
    }
    area_vfx_client_announce_005edf60(client_app(), nullptr, row, 0,
        reinterpret_cast<const kotor_w3::Vector3*>(at(self, 0x24)));
}

}
