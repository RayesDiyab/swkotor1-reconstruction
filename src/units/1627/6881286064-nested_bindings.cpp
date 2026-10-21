// Binding-only nested-provider joins for the party-selection lane.
// No function/data body, storage, cell, placeholder or numeric-address alias is defined here.
// The friendly LHS names are the ones this lane calls; nested-providers defines both names,
// so the joins resolve directly. Each line is the one reviewed /alternatename per original.
#include "78e79b69b7-party_selection_decls.h"

// 00687FD0: 13-byte faithful thunk (mov eax,[007A39FC]; mov ecx,[eax+4]; jmp 005ED8B0).
#pragma comment(linker, "/alternatename:@kotor_thunk_00687fd0@8=@kotor_fastcall_00687fd0@8")

// 005F4D60: 66-byte PlayCharacterSelectSound, incoming ECX ignored, one stack arg, RET4.
#pragma comment(linker, "/alternatename:@kotor_nested_005f4d60@12=@kotor_fastcall_005f4d60@12")

// 006C24A0: 147-byte SetQueryText, ECX=query, one stack U32, RET4.
#pragma comment(linker, "/alternatename:@kotor_nested_006c24a0@12=@kotor_fastcall_006c24a0@12")
