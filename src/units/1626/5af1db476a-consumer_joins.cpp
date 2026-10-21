// consumer_joins.cpp
// Binding-only translation unit.  No function body, storage, stub, trap,
// wrapper or numeric-address alias is defined here.  It emits only the
// reviewed /alternatename joins from the exact original-address names used by
// consumer lanes to this lane's genuine @kotor_nested_* / @kotor_thunk_*
// definitions.  The @kotor_nested_* definitions remain the accepted bodies;
// these joins add no code and change no body hash.
//
// Consumers: action-input imports @kotor_fastcall_00674090@8,
// @kotor_fastcall_006880c0@8 and records @kotor_fastcall_004d37b0@8;
// ui-save-state expects the 005F4460/005ED670 exact names; party-selection
// imports the friendly @kotor_nested_* names and keeps its own reverse joins.
// /alternatename is inert when the left-hand symbol is already defined, so
// these lines cannot override a genuine definition.
#include "7d9ceabe3d-nested_providers.h"

// 005F4460: 651 B, ECX=this, RET0.
#pragma comment(linker, "/alternatename:@kotor_fastcall_005f4460@8=@kotor_nested_005f4460@8")

// 00674090: 55 B, ECX=this, RET0.
#pragma comment(linker, "/alternatename:@kotor_fastcall_00674090@8=@kotor_nested_00674090@8")

// 005ED670: 8 B client->internal adjust, 3 stack args, RET 0xC.
#pragma comment(linker, "/alternatename:@kotor_fastcall_005ed670@20=@kotor_nested_005ed670@20")

// 006C24A0: 147 B, ECX=query, one stack U32, RET4.
#pragma comment(linker, "/alternatename:@kotor_fastcall_006c24a0@12=@kotor_nested_006c24a0@12")

// 00687FD0: 13 B A->B thunk into accepted 005ED8B0.
#pragma comment(linker, "/alternatename:@kotor_fastcall_00687fd0@8=@kotor_thunk_00687fd0@8")

// 005F4D60: 66 B, incoming ECX ignored, one stack arg, RET4.
#pragma comment(linker, "/alternatename:@kotor_fastcall_005f4d60@12=@kotor_nested_005f4d60@12")

// 006880C0: 64 B, ECX ignored, RET0.
#pragma comment(linker, "/alternatename:@kotor_fastcall_006880c0@8=@kotor_nested_006880c0@8")

// 004D37B0: 45 B, ECX=server subobject, RET0, EAX 0/1.
#pragma comment(linker, "/alternatename:@kotor_fastcall_004d37b0@8=@kotor_nested_004d37b0@8")

// 005EEA90: 20 B guard on [C+0x14C], 3 stack args, RET 0xC.
#pragma comment(linker, "/alternatename:@kotor_fastcall_005eea90@20=@kotor_nested_005eea90@20")

// 006774E0: 118 B message builder, 3 stack args, RET 0xC.
#pragma comment(linker, "/alternatename:@kotor_fastcall_006774e0@20=@kotor_nested_006774e0@20")
