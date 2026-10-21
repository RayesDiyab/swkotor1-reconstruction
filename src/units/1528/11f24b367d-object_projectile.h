#pragma once
#include <cstdint>
namespace kotor_reconstruction { struct StartupString; }
extern "C" {
// CSWC projectile storage is 0x1c8 bytes. Original constructor RET 0.
void* __fastcall object_projectile_006d57b0(void* self, void* dummy);
// Original RET 4; duration is transported as an unchanged DWORD.
std::uint32_t __fastcall object_projectile_006d2c30(void* self, void* dummy, std::uint32_t duration);
void __fastcall object_projectile_006d41b0(void* self, void* dummy, std::uint32_t effect);
std::uint32_t __fastcall object_projectile_006d4250(void* self, void* dummy, std::uint32_t animation);
// Original RET 8; string is a pointer to canonical eight-byte storage.
void __fastcall object_projectile_006d42d0(void* self, void* dummy, std::uint32_t target,
                                        kotor_reconstruction::StartupString* name);
void __fastcall object_projectile_006d6ee0(void* self, void* dummy, std::uint32_t mode);
}
