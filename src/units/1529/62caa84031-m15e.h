#pragma once
// lane-m15-e (L15-E, Milestone 1.5): CSWCCreature / CSWCObject virtual slots (vtables 0074F5F8 / 007516C8)
// and their call closure, scoped by lane-m15-f SLOT-AUDIT.json (22 needed slots; the other slots are D83 traps).
// Original: swkotor.exe GOG K1 (image base 0x400000); static disassembly only, nothing executed.
//
// Consumers (L15-A ctor/SetAppearance/CGPauseCycle, L15-C/L15-D panels) include this header (header root:
// claude-takeover\lane-m15-e\src) to call creature/object virtuals through the typed tables and to store the
// table pointers (CSWCCreature ctor 00616A51 stores &kotor_m15e_creature_methods_0074f5f8; CSWCObject ctor
// 0063E562 stores &kotor_m15e_object_methods_007516c8). Never store the raw original addresses.
//
// Conventions (same as lane-m15-0 / lane-m15-b; DECISIONS D1/D21/D24/D41):
//  * every original __thiscall entry is an extern "C" __fastcall(Self*, void* unused_edx, <original stack
//    words>) veneer with identical callee cleanup (RET N noted per entry); callers pass nullptr for EDX and
//    no callee reads EDX;
//  * struct returns use the original hidden-return pointer as the first stack argument, returned in EAX
//    (D1-amendment (a));
//  * x87 ST0 returns are typed float;
//  * a vtable is a typed constant struct of such pointers; an object's first word holds the address of its
//    typed table (object-pointer conversion only, D41); unauthored slots hold D83 traps (m15e_traps.cpp),
//    _purecall slots hold the existing owner packed_purecall_006fb8de through a typed adapter (D24).
#include <cstdint>

namespace kotor_m15e {
using U = std::uint32_t;
}

// =============================================================================================== tables
// CSWCObject vtable 007516C8 (82 slots, +000..+144). Slot types are the original ABI of the targets of BOTH
// tables, with the concrete +118 return channel selected per table below.
template<class QueuedCallbackResult>
struct KotorM15ObjectMethodSlots {
    void* (__fastcall* s000_scalar_deleting_destructor)(void* self, void* edx, kotor_m15e::U flags);   // RET 4, EAX=this
    void (__fastcall* s004_set_id)(void* self, void* edx, kotor_m15e::U id);                          // 00616FA0 RET 4
    void (__fastcall* s008)(void* self, void* edx, kotor_m15e::U a, kotor_m15e::U b);                // RET 8
    void* (__fastcall* s00c_as_creature_or_self)(void* self, void* edx);                              // 00641DB0 self_return
    kotor_m15e::U (__fastcall* s010)(void*, void*);  kotor_m15e::U (__fastcall* s014)(void*, void*);  // 0063E7F0
    kotor_m15e::U (__fastcall* s018)(void*, void*);  kotor_m15e::U (__fastcall* s01c)(void*, void*);
    kotor_m15e::U (__fastcall* s020)(void*, void*);  kotor_m15e::U (__fastcall* s024)(void*, void*);
    kotor_m15e::U (__fastcall* s028)(void*, void*);
    void* (__fastcall* s02c_as_creature)(void* self, void* edx);                                      // creature: self; object: 0
    kotor_m15e::U (__fastcall* s030)(void*, void*);  kotor_m15e::U (__fastcall* s034)(void*, void*);
    kotor_m15e::U (__fastcall* s038)(void*, void*);  kotor_m15e::U (__fastcall* s03c)(void*, void*);
    kotor_m15e::U (__fastcall* s040)(void*, void*);  kotor_m15e::U (__fastcall* s044)(void*, void*);
    kotor_m15e::U (__fastcall* s048)(void*, void*);  kotor_m15e::U (__fastcall* s04c)(void*, void*);
    kotor_m15e::U (__fastcall* s050)(void*, void*);  kotor_m15e::U (__fastcall* s054)(void*, void*);
    kotor_m15e::U (__fastcall* s058)(void*, void*);  kotor_m15e::U (__fastcall* s05c)(void*, void*);
    kotor_m15e::U (__fastcall* s060)(void*, void*);  kotor_m15e::U (__fastcall* s064)(void*, void*);
    kotor_m15e::U (__fastcall* s068)(void*, void*);  kotor_m15e::U (__fastcall* s06c)(void*, void*);
    void* (__fastcall* s070_get_name)(void* self, void* edx, void* out_string);                       // RET 4 (CExoString hidden return)
    void (__fastcall* s074_ai_update)(void* self, void* edx);                                // RET 0
    kotor_m15e::U (__fastcall* s078_load_model)(void* self, void* edx, void* resref, kotor_m15e::U part, kotor_m15e::U model_type);   // RET 0Ch
    kotor_m15e::U (__fastcall* s07c_unload_model)(void* self, void* edx, kotor_m15e::U part);         // RET 4
    void (__fastcall* s080_unload_model)(void* self, void* edx);                             // RET 0
    void (__fastcall* s084_set_area)(void* self, void* edx, void* area);                     // RET 4
    void (__fastcall* s088_set_orientation)(void* self, void* edx, const float* vector3);    // RET 4
    void (__fastcall* s08c_set_position)(void* self, void* edx, const float* vector3);       // RET 4
    void (__fastcall* s090_position_changed)(void* self, void* edx);                                  // 004015A0 RET 0 (EAX unused)
    kotor_m15e::U (__fastcall* s094_attach_to_scene)(void* self, void* edx, void* scene);             // RET 4
    void* (__fastcall* s098_get_model)(void* self, void* edx, kotor_m15e::U part, kotor_m15e::U force);   // RET 8 -> Gob*
    kotor_m15e::U (__fastcall* s09c_set_animation)(void* self, void* edx, kotor_m15e::U animation, kotor_m15e::U a2, kotor_m15e::U a3);  // RET 0Ch
    void (__fastcall* s0a0_add_fire_forget_animation)(void* self, void* edx, kotor_m15e::U animation, kotor_m15e::U a2);       // RET 8
    kotor_m15e::U (__fastcall* s0a4_hilite)(void*, void*, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U);   // RET 10h
    kotor_m15e::U (__fastcall* s0a8_unhilite)(void*, void*, kotor_m15e::U);                         // RET 4
    kotor_m15e::U (__fastcall* s0ac_set_env_map)(void* self, void* edx, kotor_m15e::U part, kotor_m15e::U a2, kotor_m15e::U a3, void* resref);   // RET 10h
    kotor_m15e::U (__fastcall* s0b0_set_env_map)(void*, void*, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U);              // RET 0Ch
    kotor_m15e::U (__fastcall* s0b4_add_visual_effect)(void*, void*, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U,
                                                       kotor_m15e::U, kotor_m15e::U, kotor_m15e::U);                      // RET 1Ch
    kotor_m15e::U (__fastcall* s0b8_delete_visual_effect)(void*, void*, kotor_m15e::U);             // RET 4
    kotor_m15e::U (__fastcall* s0bc_kill_all_visual_effects)(void*, void*);
    kotor_m15e::U (__fastcall* s0c0_refresh_visual_effects)(void*, void*, kotor_m15e::U);           // RET 4
    void (__fastcall* s0c4_update_visual_effects_list)(void*, void*);
    void (__fastcall* s0c8_stop_all_visual_effects)(void*, void*);
    kotor_m15e::U (__fastcall* s0cc_has_visual_effect_applied)(void*, void*, kotor_m15e::U);        // RET 4
    void* (__fastcall* s0d0_get_camera_follow_part)(void*, void*);
    kotor_m15e::U (__fastcall* s0d4_get_following_body)(void*, void*);
    double (__fastcall* s0d8_get_head_height_offset)(void* self, void* edx);                         // ST0 (x87 wide value, D30-FINAL)
    void (__fastcall* s0dc_activate_hard_coded_visual)(void*, void*, kotor_m15e::U, kotor_m15e::U);   // RET 8
    kotor_m15e::U (__fastcall* s0e0_detach_visual_effects)(void* self, void* edx);
    kotor_m15e::U (__fastcall* s0e4_reattach_visual_effects)(void* self, void* edx);
    void (__fastcall* s0e8_update_pop_up_bubble)(void*, void*);
    void (__fastcall* s0ec_set_portrait)(void* self, void* edx, void* resref);             // RET 4
    void* (__fastcall* s0f0_get_portrait)(void* self, void* edx, void* out_resref, kotor_m15e::U a2);   // RET 8
    void (__fastcall* s0f4_set_portrait_id)(void* self, void* edx, kotor_m15e::U id);       // RET 4
    std::uint16_t (__fastcall* s0f8_get_portrait_id)(void*, void*);
    kotor_m15e::U (__fastcall* s0fc_apply_shadow_blob)(void* self, void* edx);
    kotor_m15e::U (__fastcall* s100_remove_shadow_blob)(void* self, void* edx);
    void (__fastcall* s104_show_shadow_blob)(void*, void*);
    kotor_m15e::U (__fastcall* s108_hide_shadow_blob)(void* self, void* edx);
    void (__fastcall* s10c_set_ground_normal)(void*, void*, const float*);                 // RET 4
    void (__fastcall* s110_turn_on_shadows)(void*, void*);
    void (__fastcall* s114_turn_off_shadows)(void*, void*);
    QueuedCallbackResult (__fastcall* s118_set_queued_animation_callback)(void* self, void* edx);
    kotor_m15e::U (__fastcall* s11c_clear_queued_animation_callback)(void* self, void* edx);
    kotor_m15e::U (__fastcall* s120_bark_string)(void*, void*, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U);   // RET 10h
    kotor_m15e::U (__fastcall* s124_bark_bubble)(void*, void*, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U);   // RET 10h
    kotor_m15e::U (__fastcall* s128_lip_sync)(void*, void*, kotor_m15e::U);                          // RET 4
    void (__fastcall* s12c_sync_position)(void*, void*, const float*, kotor_m15e::U, float);   // RET 0Ch
    void (__fastcall* s130_sync_orientation)(void*, void*, const float*);                  // RET 4
    void (__fastcall* s134_sync_animation)(void*, void*, kotor_m15e::U, kotor_m15e::U);     // RET 8
    kotor_m15e::U (__fastcall* s138_is_hostile)(void*, void*);
    std::uint8_t (__fastcall* s13c_get_reticle_point)(void*, void*, void*);                 // RET 4
    double (__fastcall* s140_return_1_5)(void*, void*);                                              // ST0
    double (__fastcall* s144_get_sticky_mouse_threshold)(void*, void*);                              // ST0 (x87 wide)
};
// Original +118 differs: base004015A0 returns no value; creature0060F150 returns a callback handle.
using KotorM15ObjectMethods_007516c8 = KotorM15ObjectMethodSlots<void>;
using KotorM15CreatureObjectMethods = KotorM15ObjectMethodSlots<kotor_m15e::U>;
static_assert(sizeof(KotorM15CreatureObjectMethods) == 82 * 4, "creature common prefix size");
static_assert(sizeof(KotorM15ObjectMethods_007516c8) == 82 * 4, "CSWCObject vtable 007516C8..0075180F");

// CSWCCreature vtable 0074F5F8 (88 slots): the 82 CSWCObject slots + 6.
struct KotorM15CreatureMethods_0074f5f8 {
    KotorM15CreatureObjectMethods object;
    void* (__fastcall* s148_get_portrait)(void* self, void* edx, void* out_resref, kotor_m15e::U a2);   // RET 8
    void* (__fastcall* s14c_get_current_way_point)(void*, void*);
    void* (__fastcall* s150_get_end_way_point)(void*, void*);
    void* (__fastcall* s154_get_start_way_point)(void*, void*);
    kotor_m15e::U (__fastcall* s158_request_model)(void*, void*, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U);   // RET 0Ch
    kotor_m15e::U (__fastcall* s15c_move_to_next_way_point)(void*, void*);
};
static_assert(sizeof(KotorM15CreatureMethods_0074f5f8) == 88 * 4, "CSWCCreature vtable 0074F5F8..0074F757");

extern "C" {
extern const KotorM15CreatureMethods_0074f5f8 kotor_m15e_creature_methods_0074f5f8;   // stored by CSWCCreature ctor 00616A51 / dtor 006170EF
extern const KotorM15ObjectMethods_007516c8 kotor_m15e_object_methods_007516c8;       // stored by CSWCObject ctor 0063E562 / dtor 0063E800

// ======================================================================================= entry points
// Every slot body below is also reachable through the tables; direct calls use these names.
// CSWCCreature (ECX = creature)
void* __fastcall kotor_m15e_creature_scalar_deleting_destructor_00618280(void* self, void*, kotor_m15e::U flags) noexcept(false);
void __fastcall kotor_m15e_creature_destruct_006170d0(void* self, void*) noexcept(false);
void* __fastcall kotor_m15e_creature_get_name_00617090(void* self, void*, void* out_string) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_creature_load_model_0061b380(void* self, void*, void* resref, kotor_m15e::U part, kotor_m15e::U model_type) noexcept(false);
void __fastcall kotor_m15e_creature_set_position_0060b9b0(void* self, void*, const float* position);
kotor_m15e::U __fastcall kotor_m15e_creature_attach_to_scene_0060fb40(void* self, void*, void* scene);
void* __fastcall kotor_m15e_creature_get_model_006179c0(void* self, void*, kotor_m15e::U part, kotor_m15e::U force) noexcept(false);
void __fastcall kotor_m15e_creature_add_fire_forget_animation_006183d0(void* self, void*, kotor_m15e::U animation, kotor_m15e::U a2) noexcept(false);
double __fastcall kotor_m15e_creature_get_head_height_offset_0060f000(void* self, void*) noexcept(false);
void __fastcall kotor_m15e_creature_set_portrait_0060d560(void* self, void*, void* resref) noexcept(false);
void __fastcall kotor_m15e_creature_set_portrait_id_0060d5c0(void* self, void*, kotor_m15e::U id) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_creature_apply_shadow_blob_0060e2f0(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_creature_set_queued_animation_callback_0060f150(void* self, void*);
kotor_m15e::U __fastcall kotor_m15e_creature_clear_queued_animation_callback_0060f1a0(void* self, void*);
void* __fastcall kotor_m15e_creature_get_portrait_00616ff0(void* self, void*, void* out_resref, kotor_m15e::U a2) noexcept(false);
// CSWCObject (ECX = object)
kotor_m15e::U __fastcall kotor_m15e_object_unload_model_00646650(void* self, void*, kotor_m15e::U part);
void __fastcall kotor_m15e_object_set_orientation_0063c110(void* self, void*, const float* orientation);
kotor_m15e::U __fastcall kotor_m15e_object_set_animation_0063e930(void* self, void*, kotor_m15e::U animation, kotor_m15e::U a2, kotor_m15e::U a3) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_object_set_env_map_0063ccc0(void* self, void*, kotor_m15e::U part, kotor_m15e::U a2, kotor_m15e::U a3, void* resref) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_object_detach_visual_effects_0063e450(void* self, void*);
kotor_m15e::U __fastcall kotor_m15e_object_reattach_visual_effects_0063e4b0(void* self, void*);
kotor_m15e::U __fastcall kotor_m15e_object_remove_shadow_blob_0063d070(void* self, void*);
kotor_m15e::U __fastcall kotor_m15e_object_hide_shadow_blob_00616fd0(void* self, void*);
void __fastcall kotor_m15e_object_destruct_0063e800(void* self, void*) noexcept(false);
}
