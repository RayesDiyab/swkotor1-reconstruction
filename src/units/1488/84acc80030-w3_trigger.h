#pragma once
#include <cstdint>
namespace kotor_w3 {struct Vector3;}
extern "C" {
void* __fastcall recovered_client_trigger_scalar_destructor_00691ca0(void*,void*,std::uint32_t) noexcept(false);
void __fastcall recovered_client_trigger_ai_update_00692410(void*,void*) noexcept(false);
std::uint8_t __fastcall recovered_client_trigger_get_reticle_point_00691b00(void*,void*,void*) noexcept(false);
void __fastcall w3_trigger_006912f0(void*,void*) noexcept(false);
void __fastcall w3_trigger_00691560(void*,void*) noexcept(false);
std::uint8_t __fastcall w3_trigger_006916b0(void*,void*,kotor_w3::Vector3*) noexcept(false);
void __fastcall w3_trigger_00691970(void*,void*) noexcept(false);
void __fastcall w3_trigger_006920b0(void*,void*) noexcept(false);
std::uint32_t __fastcall w3_trigger_00691a30(void*,void*,kotor_w3::Vector3*,kotor_w3::Vector3*,kotor_w3::Vector3*) noexcept(false);
}
