#pragma once
// lane-m15-c: CSWNameGen / CResHelper<CResLTR,0x07F4> / CResLTR (m15c_namegen.cpp). Internal to L15-C (the
// CSWGuiNameChargen panel embeds a CSWNameGen at +9A0h); declarations follow the D1 veneer convention.
#include "c53472623c7d6765_resources.h"          // menu_resources::Ref (16-byte CResRef)
#include "e1805047c9b7563b_startup_string.h"     // kotor_reconstruction::StartupString (CExoString)
#include <cstddef>
#include <cstdint>

namespace kotor_m15c_namegen {
// CSWNameGen (0x24): CResHelper<CResLTR,0x07F4> base (0x1C, same layout as lane-tpc ProcessedTexture / Txi)
// + loaded flag, cached race and flags (ctor 00711290, GetRandomName 007118C0).
struct NameGen {
    const void* volatile methods;           // +00 0075FC0C (helper) / 0075FC10 (name generator)
    volatile std::uint32_t requested;       // +04 (00711236)
    void* volatile resource;                // +08 CResLTR* (0x5C, 007121B0)
    menu_resources::Ref reference;          // +0C CResRef
    std::uint32_t loaded;                   // +1C (0071137A; 00711090)
    std::uint16_t race;                     // +20 (007112C5; 007120B4)
    unsigned char flags;                    // +22 (007112CF = 1; 007120C6)
    unsigned char pad23;
};
static_assert(sizeof(NameGen) == 0x24 && offsetof(NameGen, resource) == 8 && offsetof(NameGen, reference) == 0x0C &&
              offsetof(NameGen, loaded) == 0x1C && offsetof(NameGen, race) == 0x20 && offsetof(NameGen, flags) == 0x22,
              "CSWNameGen layout (00711290)");
}

extern "C" {
void* __fastcall kotor_m15c_res_ltr_construct_007121b0(void* self, void* unused_edx) noexcept(false);
void __fastcall kotor_m15c_res_ltr_destroy_007121f0(void* self, void* unused_edx) noexcept;
void* __fastcall kotor_m15c_res_ltr_deleting_destroy_00712980(void* self, void* unused_edx, std::uint32_t flags) noexcept;
std::uint32_t __fastcall kotor_m15c_res_ltr_freed_00712200(void* self, void* unused_edx);
std::uint32_t __fastcall kotor_m15c_res_ltr_serviced_00712410(void* self, void* unused_edx) noexcept(false);
void __fastcall kotor_m15c_ltr_helper_destroy_007110b0(kotor_m15c_namegen::NameGen* self, void* unused_edx) noexcept;
void* __fastcall kotor_m15c_ltr_helper_deleting_destroy_00711270(void* self, void* unused_edx, std::uint32_t flags) noexcept;
void __fastcall kotor_m15c_namegen_set_res_ref_00711110(kotor_m15c_namegen::NameGen* self, void* unused_edx,
    menu_resources::Ref* ref, std::int32_t request) noexcept(false);
kotor_m15c_namegen::NameGen* __fastcall kotor_m15c_namegen_construct_00711290(kotor_m15c_namegen::NameGen* self,
    void* unused_edx) noexcept(false);
void __fastcall kotor_m15c_namegen_destruct_007112e0(kotor_m15c_namegen::NameGen* self, void* unused_edx) noexcept;
void* __fastcall kotor_m15c_namegen_deleting_destroy_00712190(void* self, void* unused_edx, std::uint32_t flags) noexcept;
void __fastcall kotor_m15c_namegen_unload_00711090(kotor_m15c_namegen::NameGen* self, void* unused_edx) noexcept;
std::int32_t __fastcall kotor_m15c_namegen_load_00711340(kotor_m15c_namegen::NameGen* self, void* unused_edx,
    kotor_reconstruction::StartupString* name) noexcept(false);
// 00711390 GetRandomName(BYTE count) RET 8; 007118C0 GetRandomName(WORD race, BYTE flags, BYTE count) RET 10h.
// Both return CExoString by value through the hidden pointer (first stack word; D1-amendment a), EAX = out.
kotor_reconstruction::StartupString* __fastcall kotor_m15c_namegen_random_letters_00711390(
    kotor_m15c_namegen::NameGen* self, void* unused_edx, kotor_reconstruction::StartupString* out,
    std::uint32_t count) noexcept(false);
kotor_reconstruction::StartupString* __fastcall kotor_m15c_namegen_random_name_007118c0(
    kotor_m15c_namegen::NameGen* self, void* unused_edx, kotor_reconstruction::StartupString* out,
    std::uint32_t race, std::uint32_t flags, std::uint32_t count) noexcept(false);
}
