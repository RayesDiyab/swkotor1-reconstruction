#pragma once
#include <cstdint>
// Declarations preserved from table29's provider_contracts.h. No nominal
// class or provider table is copied from its historical epoch headers.
namespace table29_required {
void* __fastcall gui_sound_construct_005d5a50(void*,void*);
void __fastcall gui_sound_destroy_005d6200(void*,void*);
}
namespace table29_canonical_required { extern void* sound_007a39ec; }
namespace sound_lifecycle29_required {
// Requires the one genuine mutable word owner. No storage is defined here.
extern volatile std::uint32_t* const word_00832894;
// Genuine lower contracts. These declarations do not claim lower closure or
// assign a nominal type to the unaligned reference subobject at internal+9.
void* __fastcall construct_005dbbe0(void*,void*,void*);
void __fastcall destroy_005dbca0(void*,void*);
}
