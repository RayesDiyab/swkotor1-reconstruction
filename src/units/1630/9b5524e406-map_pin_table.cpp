// 00754598 CSWCMapPin canonical method table -- private reconstruction data owner.
//
// Original identity: 00754598..007546E0, 0x148 bytes, 82 dwords.  007546DC is the
// last slot; 007546E0 is the first byte after the table (ASCII "k_sup_gohawk").
// The table is NOT a copy or alias of the reconstructed CSWCObject table 007516C8:
// map-pin differs at slots 0, 29, 30, 31 and 32.  Slot 0 is the map-pin scalar
// deleting destructor 00692610; slot 35 (+0x8C) is the selected real SetPosition
// body @area_vfx_base_set_position_0063f340@12 @ 0x00426990, never the W3 trap
// @kotor_m15e_trap_0063f340@12 @ 0x00539240.
//
// Every initializer is the address of a named selected symbol (or of the authored
// 00692610 body).  There is no numeric original address, no storage placed at the
// original VA, no raw pointer transplant, no per-object copy and no base-table
// alias.  The table relocates normally through COFF relocations.  MAP-PIN-TABLE-
// INVENTORY.json is the exact 82-row target -> selected-symbol/ABI/status record.
#include "05d8ea4633-map_pin_table.h"
#include "cd5f8e5e6a-map_pin_dtor.h"

// Selected providers are declared with their exact project decoration.  The
// declarations below are only address bindings for this data object; calls are
// made through the original vtable slot contract, not through these prototypes.

extern "C" void __fastcall cswc_object_set_id_00616fa0(void* self, void* unused_edx, void* a1);
extern "C" void __fastcall gui_load_slot_ret8_005b5e90(void* self, void* unused_edx, void* a1, void* a2);
extern "C" void* __fastcall gob_self_return_00641db0(void* self, void* unused_edx);
extern "C" int __fastcall slot_return_zero_0063e7f0_a0(void* self, void* unused_edx);
extern "C" void* __fastcall area_vfx_base_name_0063e730(void* self, void* unused_edx, void* a1);
extern "C" void __fastcall gui_load_slot_ret_004015a0(void* self, void* unused_edx);
// Slot 30: selected C++ symbol in an anonymous namespace; a linker alias gives the
// table a C-fastcall spelling without defining a wrapper or a second body.
extern "C" unsigned int __fastcall mp_slot30_ai_zero_0066b360(
    void* self, void* unused_edx, void* a1, void* a2, unsigned int a3);
#pragma comment(linker, "/alternatename:@mp_slot30_ai_zero_0066b360@20=?ai_zero_0066b360@?A0xA65B3CB@@YIIPAX000I@Z")

extern "C" void __fastcall gui_load_slot_ret4_0060e760(void* self, void* unused_edx, void* a1);
extern "C" void __fastcall recovered_client_object_set_area_0063f570(void* self, void* unused_edx, void* a1);
extern "C" void __fastcall kotor_m15e_object_set_orientation_0063c110(void* self, void* unused_edx, void* a1);
extern "C" void __fastcall area_vfx_base_set_position_0063f340(void* self, void* unused_edx, void* a1);
extern "C" void __fastcall kotor_m15e_object_attach_to_scene_0063d820(void* self, void* unused_edx, void* a1);
extern "C" void* __fastcall recovered_client_object_get_model_0063c080(void* self, void* unused_edx, void* a1, void* a2);
extern "C" void __fastcall kotor_m15e_object_set_animation_0063e930(void* self, void* unused_edx, void* a1, void* a2, void* a3);
extern "C" void __fastcall kotor_m15e_object_add_fire_forget_animation_0063d8c0(void* self, void* unused_edx, void* a1, void* a2);
extern "C" void __fastcall area_vfx_base_hilite_0063cb00(void* self, void* unused_edx, void* a1, void* a2, void* a3, void* a4);
extern "C" void __fastcall area_vfx_base_unhilite_0063cb30(void* self, void* unused_edx, void* a1);
extern "C" void __fastcall kotor_m15e_object_set_env_map_0063ccc0(void* self, void* unused_edx, void* a1, void* a2, void* a3, void* a4);
extern "C" void __fastcall recovered_client_object_set_env_map_0063cb70(void* self, void* unused_edx, void* a1, void* a2, void* a3);
extern "C" int __fastcall object_vfx_0063d970(void* self, void* unused_edx, void* a1, void* a2, void* a3, void* a4, void* a5, void* a6, void* a7);
extern "C" void __fastcall object_vfx_0063f400(void* self, void* unused_edx, void* a1);
extern "C" void __fastcall kotor_m15e_object_kill_all_visual_effects_0063dae0(void* self, void* unused_edx);
extern "C" void __fastcall object_vfx_0063db20(void* self, void* unused_edx, void* a1);
extern "C" void __fastcall object_vfx_0063f490(void* self, void* unused_edx);
extern "C" void __fastcall object_projectile_base_0063daa0(void* self, void* unused_edx);
extern "C" void __fastcall object_vfx_0063ddf0(void* self, void* unused_edx, void* a1);
extern "C" void __fastcall object_projectile_base_0063e760(void* self, void* unused_edx);
extern "C" int __fastcall input_desc_return_true_0063e7a0(void* self, void* unused_edx);
extern "C" float __fastcall kotor_fastcall_00616fe0(void* self, void* unused_edx);
extern "C" void __fastcall object_vfx_0063db90(void* self, void* unused_edx, void* a1, void* a2);
extern "C" void __fastcall kotor_m15e_object_detach_visual_effects_0063e450(void* self, void* unused_edx);
extern "C" void __fastcall kotor_m15e_object_reattach_visual_effects_0063e4b0(void* self, void* unused_edx);
extern "C" void __fastcall object_projectile_base_0063cfd0(void* self, void* unused_edx, void* a1);
extern "C" void* __fastcall object_projectile_base_0063e770(void* self, void* unused_edx, void* a1, void* a2);
extern "C" void __fastcall object_projectile_base_0063d140(void* self, void* unused_edx, void* a1);
extern "C" unsigned short __fastcall object_projectile_base_0063e790(void* self, void* unused_edx);
extern "C" void __fastcall kotor_m15e_object_remove_shadow_blob_0063d070(void* self, void* unused_edx);
extern "C" void __fastcall recovered_client_object_show_shadow_blob_00616fb0(void* self, void* unused_edx);
extern "C" void __fastcall kotor_m15e_object_hide_shadow_blob_00616fd0(void* self, void* unused_edx);
extern "C" void __fastcall object_projectile_base_0063d0a0(void* self, void* unused_edx, void* a1);
extern "C" void __fastcall recovered_client_object_turn_on_shadows_0060b310(void* self, void* unused_edx);
extern "C" void __fastcall recovered_client_object_turn_off_shadows_0060b320(void* self, void* unused_edx);
extern "C" void __fastcall object_projectile_base_0063e7b0(void* self, void* unused_edx, void* a1, void* a2, void* a3, void* a4);
extern "C" void __fastcall object_projectile_base_0060f560(void* self, void* unused_edx, void* a1, void* a2, void* a3, void* a4);
extern "C" int __fastcall aurora_return_false_4_0063e7c0(void* self, void* unused_edx, void* a1);
extern "C" void __fastcall object_projectile_base_0063e7d0(void* self, void* unused_edx, void* a1, void* a2, void* a3);
extern "C" void __fastcall object_projectile_base_0063e7e0(void* self, void* unused_edx, void* a1);
extern "C" void __fastcall object_projectile_base_0063d3f0(void* self, void* unused_edx, void* a1, void* a2);
extern "C" unsigned long __fastcall kotor_fastcall_0063d560(void* self, void* unused_edx, void* a1);
extern "C" float __fastcall kotor_fastcall_0063d780(void* self, void* unused_edx);

#pragma warning(push)
#pragma warning(disable: 4152 4191)  // canonical table stores function addresses as const void* relocations
extern "C" const void* const recovered_client_map_pin_methods_00754598[82] = {
    /* 00 0x00754598 */ reinterpret_cast<const void*>(&recovered_client_map_pin_scalar_delete_00692610),  // original 0x00692610
    /* 01 0x0075459C */ reinterpret_cast<const void*>(&cswc_object_set_id_00616fa0),  // original 0x00616FA0
    /* 02 0x007545A0 */ reinterpret_cast<const void*>(&gui_load_slot_ret8_005b5e90),  // original 0x005B5E90
    /* 03 0x007545A4 */ reinterpret_cast<const void*>(&gob_self_return_00641db0),  // original 0x00641DB0
    /* 04 0x007545A8 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 05 0x007545AC */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 06 0x007545B0 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 07 0x007545B4 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 08 0x007545B8 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 09 0x007545BC */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 10 0x007545C0 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 11 0x007545C4 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 12 0x007545C8 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 13 0x007545CC */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 14 0x007545D0 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 15 0x007545D4 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 16 0x007545D8 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 17 0x007545DC */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 18 0x007545E0 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 19 0x007545E4 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 20 0x007545E8 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 21 0x007545EC */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 22 0x007545F0 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 23 0x007545F4 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 24 0x007545F8 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 25 0x007545FC */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 26 0x00754600 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 27 0x00754604 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 28 0x00754608 */ reinterpret_cast<const void*>(&area_vfx_base_name_0063e730),  // original 0x0063E730
    /* 29 0x0075460C */ reinterpret_cast<const void*>(&gui_load_slot_ret_004015a0),  // original 0x004015A0
    /* 30 0x00754610 */ reinterpret_cast<const void*>(&mp_slot30_ai_zero_0066b360),  // original 0x0066B360
    /* 31 0x00754614 */ reinterpret_cast<const void*>(&gui_load_slot_ret4_0060e760),  // original 0x0060E760
    /* 32 0x00754618 */ reinterpret_cast<const void*>(&gui_load_slot_ret_004015a0),  // original 0x004015A0
    /* 33 0x0075461C */ reinterpret_cast<const void*>(&recovered_client_object_set_area_0063f570),  // original 0x0063F570
    /* 34 0x00754620 */ reinterpret_cast<const void*>(&kotor_m15e_object_set_orientation_0063c110),  // original 0x0063C110
    /* 35 0x00754624 */ reinterpret_cast<const void*>(&area_vfx_base_set_position_0063f340),  // original 0x0063F340
    /* 36 0x00754628 */ reinterpret_cast<const void*>(&gui_load_slot_ret_004015a0),  // original 0x004015A0
    /* 37 0x0075462C */ reinterpret_cast<const void*>(&kotor_m15e_object_attach_to_scene_0063d820),  // original 0x0063D820
    /* 38 0x00754630 */ reinterpret_cast<const void*>(&recovered_client_object_get_model_0063c080),  // original 0x0063C080
    /* 39 0x00754634 */ reinterpret_cast<const void*>(&kotor_m15e_object_set_animation_0063e930),  // original 0x0063E930
    /* 40 0x00754638 */ reinterpret_cast<const void*>(&kotor_m15e_object_add_fire_forget_animation_0063d8c0),  // original 0x0063D8C0
    /* 41 0x0075463C */ reinterpret_cast<const void*>(&area_vfx_base_hilite_0063cb00),  // original 0x0063CB00
    /* 42 0x00754640 */ reinterpret_cast<const void*>(&area_vfx_base_unhilite_0063cb30),  // original 0x0063CB30
    /* 43 0x00754644 */ reinterpret_cast<const void*>(&kotor_m15e_object_set_env_map_0063ccc0),  // original 0x0063CCC0
    /* 44 0x00754648 */ reinterpret_cast<const void*>(&recovered_client_object_set_env_map_0063cb70),  // original 0x0063CB70
    /* 45 0x0075464C */ reinterpret_cast<const void*>(&object_vfx_0063d970),  // original 0x0063D970
    /* 46 0x00754650 */ reinterpret_cast<const void*>(&object_vfx_0063f400),  // original 0x0063F400
    /* 47 0x00754654 */ reinterpret_cast<const void*>(&kotor_m15e_object_kill_all_visual_effects_0063dae0),  // original 0x0063DAE0
    /* 48 0x00754658 */ reinterpret_cast<const void*>(&object_vfx_0063db20),  // original 0x0063DB20
    /* 49 0x0075465C */ reinterpret_cast<const void*>(&object_vfx_0063f490),  // original 0x0063F490
    /* 50 0x00754660 */ reinterpret_cast<const void*>(&object_projectile_base_0063daa0),  // original 0x0063DAA0
    /* 51 0x00754664 */ reinterpret_cast<const void*>(&object_vfx_0063ddf0),  // original 0x0063DDF0
    /* 52 0x00754668 */ reinterpret_cast<const void*>(&object_projectile_base_0063e760),  // original 0x0063E760
    /* 53 0x0075466C */ reinterpret_cast<const void*>(&input_desc_return_true_0063e7a0),  // original 0x0063E7A0
    /* 54 0x00754670 */ reinterpret_cast<const void*>(&kotor_fastcall_00616fe0),  // original 0x00616FE0
    /* 55 0x00754674 */ reinterpret_cast<const void*>(&object_vfx_0063db90),  // original 0x0063DB90
    /* 56 0x00754678 */ reinterpret_cast<const void*>(&kotor_m15e_object_detach_visual_effects_0063e450),  // original 0x0063E450
    /* 57 0x0075467C */ reinterpret_cast<const void*>(&kotor_m15e_object_reattach_visual_effects_0063e4b0),  // original 0x0063E4B0
    /* 58 0x00754680 */ reinterpret_cast<const void*>(&gui_load_slot_ret_004015a0),  // original 0x004015A0
    /* 59 0x00754684 */ reinterpret_cast<const void*>(&object_projectile_base_0063cfd0),  // original 0x0063CFD0
    /* 60 0x00754688 */ reinterpret_cast<const void*>(&object_projectile_base_0063e770),  // original 0x0063E770
    /* 61 0x0075468C */ reinterpret_cast<const void*>(&object_projectile_base_0063d140),  // original 0x0063D140
    /* 62 0x00754690 */ reinterpret_cast<const void*>(&object_projectile_base_0063e790),  // original 0x0063E790
    /* 63 0x00754694 */ reinterpret_cast<const void*>(&input_desc_return_true_0063e7a0),  // original 0x0063E7A0
    /* 64 0x00754698 */ reinterpret_cast<const void*>(&kotor_m15e_object_remove_shadow_blob_0063d070),  // original 0x0063D070
    /* 65 0x0075469C */ reinterpret_cast<const void*>(&recovered_client_object_show_shadow_blob_00616fb0),  // original 0x00616FB0
    /* 66 0x007546A0 */ reinterpret_cast<const void*>(&kotor_m15e_object_hide_shadow_blob_00616fd0),  // original 0x00616FD0
    /* 67 0x007546A4 */ reinterpret_cast<const void*>(&object_projectile_base_0063d0a0),  // original 0x0063D0A0
    /* 68 0x007546A8 */ reinterpret_cast<const void*>(&recovered_client_object_turn_on_shadows_0060b310),  // original 0x0060B310
    /* 69 0x007546AC */ reinterpret_cast<const void*>(&recovered_client_object_turn_off_shadows_0060b320),  // original 0x0060B320
    /* 70 0x007546B0 */ reinterpret_cast<const void*>(&gui_load_slot_ret_004015a0),  // original 0x004015A0
    /* 71 0x007546B4 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 72 0x007546B8 */ reinterpret_cast<const void*>(&object_projectile_base_0063e7b0),  // original 0x0063E7B0
    /* 73 0x007546BC */ reinterpret_cast<const void*>(&object_projectile_base_0060f560),  // original 0x0060F560
    /* 74 0x007546C0 */ reinterpret_cast<const void*>(&aurora_return_false_4_0063e7c0),  // original 0x0063E7C0
    /* 75 0x007546C4 */ reinterpret_cast<const void*>(&object_projectile_base_0063e7d0),  // original 0x0063E7D0
    /* 76 0x007546C8 */ reinterpret_cast<const void*>(&object_projectile_base_0063e7e0),  // original 0x0063E7E0
    /* 77 0x007546CC */ reinterpret_cast<const void*>(&object_projectile_base_0063d3f0),  // original 0x0063D3F0
    /* 78 0x007546D0 */ reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),  // original 0x0063E7F0
    /* 79 0x007546D4 */ reinterpret_cast<const void*>(&kotor_fastcall_0063d560),  // original 0x0063D560
    /* 80 0x007546D8 */ reinterpret_cast<const void*>(&kotor_fastcall_00616fe0),  // original 0x00616FE0
    /* 81 0x007546DC */ reinterpret_cast<const void*>(&kotor_fastcall_0063d780),  // original 0x0063D780
};
#pragma warning(pop)

static_assert(sizeof(recovered_client_map_pin_methods_00754598) == 328,
              "CSWCMapPin table must be 82 dwords / 328 bytes");
static_assert(sizeof(recovered_client_map_pin_methods_00754598) / sizeof(void*) == 82,
              "CSWCMapPin table must have 82 slots");
