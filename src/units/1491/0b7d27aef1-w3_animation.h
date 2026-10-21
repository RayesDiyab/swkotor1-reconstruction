#pragma once
#include <cstdint>
extern "C" {
void* __fastcall w3_anim_door_scalar_006e59a0(void*,void*,std::uint32_t);
void* __fastcall w3_anim_placeable_scalar_006e5180(void*,void*,std::uint32_t);
void* __fastcall w3_anim_trigger_scalar_006e6020(void*,void*,std::uint32_t);
void __fastcall w3_anim_door_destruct_006e56c0(void*,void*);
void __fastcall w3_anim_placeable_destruct_006e4e70(void*,void*);
void __fastcall w3_anim_trigger_destruct_006e5f60(void*,void*);
void* __fastcall w3_anim_placeable_name_006e51a0(void*,void*,void*,std::uint32_t);
void* __fastcall w3_anim_trigger_name_006e6040(void*,void*,void*,std::uint32_t);
std::uint32_t __fastcall w3_anim_placeable_set_006e4f30(void*,void*,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t);
std::uint16_t __fastcall w3_anim_placeable_convert_006e53f0(void*,void*,std::uint32_t);
std::uint16_t __fastcall w3_anim_trigger_convert_006e6100(void*,void*,std::uint32_t);
void* __fastcall w3_area_position_0062ec20(void*,void*,void*);
void __cdecl w3_bullet_die_00670b20(void*);
}
