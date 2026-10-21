#pragma once
#include "8c1ebff04be0d29d_material.h"
#include "564c91413881b15e_texture.hpp" // E1 epoch
namespace texture_join {
using Ref=font::Texture; using List=kotor_reconstruction::ArrayMutations;using Word=std::uint32_t;
// Task7 shared-storage owner and Main6 must bind the SAME live nominal cells.
extern font::Safe& canonical_null_007a477c;
extern List& canonical_environment_007a4770;
Ref* __cdecl get_null_004230c0();
Ref* __fastcall referenced_004205c0(Ref*,void*);
// ABI support: dispatches actual slot08 with ECX and full EAX, no stub/table.
Ref* __fastcall call_slot08(Ref*,void*);
void __fastcall add_unique_0046bd10(List*,void*,Ref*);
// Raw DWORD stack carrier preserves the original low-byte boolean representation.
// The true tail-call path overwrites the incoming caller argument cell.
void __fastcall set_environment_00421480(Ref*,void*,Word);
}
