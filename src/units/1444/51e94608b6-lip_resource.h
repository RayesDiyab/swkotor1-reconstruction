#pragma once
#include "983aa0b277-startup_string.h"
#include <cstdint>
namespace menu_resources { struct Ref; }
extern "C" {
void* __fastcall lip_construct_0070c4d0(void*,void*);
void __fastcall lip_destroy_0070c520(void*,void*);
void* __fastcall lip_deleting_0070c670(void*,void*,std::uint32_t);
float __fastcall lip_duration_0070c280(void*,void*);
std::uint32_t __fastcall lip_frame_id_0070c290(void*,void*,std::uint32_t);
float __fastcall lip_frame_time_0070c2c0(void*,void*,std::uint32_t);
// Original callee owns/destructs an eight-byte String stack argument; RET8.
std::uint32_t __fastcall lip_load_0070c590(void*,void*,kotor_reconstruction::StartupString);
void __fastcall lip_set_ref_0070c350(void*,void*,menu_resources::Ref*,std::int32_t);
void* __fastcall lip_resource_construct_0070c690(void*,void*);
}
