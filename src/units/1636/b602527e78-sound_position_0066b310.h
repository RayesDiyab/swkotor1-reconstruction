#pragma once
// 0066B310 CSWCSoundObject::SetPosition.
//
// Original: __thiscall, ECX = CSWCSoundObject*, one stack argument
// float[3]*, RET 4, void. Body 0066B310..0066B352; next function 0066B360.
#include "133115ec0d-r4_sound_corpse_abi.h"

extern "C" {

// Selected 00426990 (original 0063F340), @12 RET4. It copies the three floats
// to self+0x24 and runs the area-vfx position path; it dereferences position.
void __fastcall area_vfx_base_set_position_0063f340(void* self, void* unused_edx,
                                                    const void* position) noexcept(false);

// Selected 004770C0 (original 005D59E0), @16 RET8. The original pushes 0 for the
// trailing stack word; the selected provider types it as float. 0.0f has the
// same four zero bytes and adds nothing to the second float.
void __fastcall kotor_m15_sound_source_set_position_005d59e0(void* source, void* unused_edx,
                                                             const void* position,
                                                             float z_offset) noexcept(false);

// Selected 00477760 (original 005D62C0), @12 RET4.
void __fastcall kotor_m15_stream_source_set_position_005d62c0(void* source, void* unused_edx,
                                                              const void* position) noexcept(false);

// 0066B310. ECX = self, one stack argument, RET4, void. The original returns no
// meaningful EAX and adds no null guard on position.
void __fastcall kotor_sound_object_set_position_0066b310(void* self, void* unused_edx,
                                                         const void* position) noexcept(false);

} // extern "C"
