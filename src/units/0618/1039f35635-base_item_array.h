#pragma once
// lane-rules fork B: CSWBaseItemArray / CSWBaseItem (swkotor.exe 9c10e045..., base 0x400000).
// D1 form, C++ linkage (D30-FINAL: callers emit unwind cleanup around these calls).
#include "45d2335566-rules_common.h"

extern "C" {
// M41 window-native-strings.obj (identical to modern-ini-support/native_strings.h): 005E5670 CExoString::CStr.
char* __fastcall WindowCStr005e5670(kotor_reconstruction::StartupString*, void*);
}

namespace kotor_rules {
// 005B2EF0 CSWBaseItem::CSWBaseItem(), RET; EAX=this (005B2FAE). Element size 0xC8.
// Exactly the construct callback type of crt_eh_vector_constructor_006fb55f.
void* __fastcall kotor_fastcall_005b2ef0(void* self, void* unused_edx);
// 005B2FC0 CSWBaseItem::~CSWBaseItem(), RET. Exactly the destroy callback type of 006FB55F.
void __fastcall kotor_fastcall_005b2fc0(void* self, void* unused_edx);
// D24 typed form of 005B2FC0 for the 006FA852 destroy parameter (c2da_current::StringDestroy):
// same original body, receiver typed as the iterator's element view; pushed at 005B4741.
void __fastcall kotor_fastcall_005b2fc0_array_destroy(kotor_reconstruction::StartupString* self, void* unused_edx);
// 005B31D0 CSWBaseItemArray::Load(), RET; EAX not meaningful (last write is the 005E5C20
// call at 005B4638 followed by pops) -> void (consumer: lane-prepare 00552F35).
void __fastcall kotor_fastcall_005b31d0(void* self, void* unused_edx);
// 005B4730 CSWBaseItemArray::~CSWBaseItemArray(), RET (consumer: ~CSWRules 00551485).
void __fastcall kotor_fastcall_005b4730(void* self, void* unused_edx);
}
