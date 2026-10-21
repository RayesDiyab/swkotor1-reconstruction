#pragma once
#include "d981b3615c-key_lookup.h"
// Required original dependency contracts. This packet does not define them.
// ECX=list internal, EDX unused, one stack word, RET4, payload in full EAX.
extern "C" uint32_t __fastcall unresolved_list_at_pos_005e9b60(void*,void*,void*);
extern "C" uint32_t __fastcall unresolved_list_next_005e9b90(void*,void*,void**);
// ECX=table, EDX unused, resource/type DWORDs on stack, RET8, entry pointer EAX.
extern "C" uint32_t __fastcall unresolved_find_key_0040ec50(
    void*,void*,menu_resources::Ref*,uint32_t);
static_assert(sizeof(void*)==4&&sizeof(uint32_t)==4,"verified x86 carrier");
