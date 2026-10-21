#pragma once
#include "590b90d687-sound_lower.h"
#include "9b88fdfd77-ftol2.h"
namespace table29_required { void __fastcall gui_priority_005d5ba0(void*,void*,std::uint32_t); }
namespace sound_volume29 {
void __fastcall priority_005dcd50(void*,void*,std::uint32_t);
}
namespace sound_volume29_required {
// Existing owners must provide these direct transports. No implementations here.
std::uint32_t __fastcall group_005d6ad0(void*,void*,void*);
float __fastcall gain_005d6b10(void*,void*,std::uint32_t,std::uint32_t);
std::uint32_t __fastcall predicate_006054e0(void*,void*,void*);
void __fastcall refresh_005dc560(void*,void*);
extern const float one_0073d764;
extern const float scale_0073ee00;
}
extern "C" __declspec(dllimport) void __stdcall AIL_set_stream_volume_levels(void*,float,float);
