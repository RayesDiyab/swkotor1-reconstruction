#pragma once
#include <cstdint>
namespace kotor_reconstruction { struct StartupString; }
namespace menu_resources { struct Ref; }
extern "C" {
void __fastcall object_projectile_bark_006a9920(void*,void*,std::uint32_t,
    kotor_reconstruction::StartupString*,menu_resources::Ref*,std::uint32_t);
void __fastcall bark_stream_set_005d5bb0(void*,void*,std::uint32_t);
void __fastcall bark_stream_internal_set_005dbb90(void*,void*,std::uint32_t);
std::uint8_t __fastcall kotor_fastcall_00674b40(void*,void*);
// Same word transport as the existing W4 table/slot declarations.
void __fastcall kotor_fastcall_006a9fd0(void*,void*,std::uint32_t);
void __fastcall kotor_fastcall_006a9f90(void*,void*);
}
