#pragma once
#include <cstdint>
// Original x86 contracts; explicit second register argument is the fastcall bridge.
extern "C" {
void* __fastcall recovered_client_placeable_destructor_006836a0(void*, void*, std::uint32_t);
void __fastcall recovered_client_placeable_ai_update_006836c0(void*, void*);
std::uint32_t __fastcall recovered_client_placeable_apply_shadow_blob_00682c60(void*, void*);
std::uint32_t __fastcall recovered_client_placeable_is_hostile_006831b0(void*, void*);
std::uint8_t __fastcall recovered_client_placeable_get_reticle_point_00682ff0(void*, void*, void*);
float __fastcall recovered_client_placeable_get_can_see_height_00682f20(void*, void*);
void __fastcall recovered_client_placeable_unload_light_00682be0(void*, void*);
void __fastcall w3_placeable_destroy_00683200(void*, void*);
void __fastcall w3_placeable_animation_update_006832a0(void*, void*);
}
