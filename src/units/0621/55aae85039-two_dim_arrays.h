#pragma once
// lane-rules: CTwoDimArrays (0x12C allocation at 00552E8E; ctor 005BFB60 = lane-prepare).
// D1 form, C++ linkage (original bodies carry EH frames; D30-FINAL).
// Layout (observed offsets only): +00 C2DA** IPRP cost tables, +04 C2DA** IPRP param tables,
// +08 byte cost count, +09 byte param count, +0C..+128 C2DA* (72 table slots).
namespace kotor_rules {
// 005C4C20 CTwoDimArrays::Load2DArrays, RET, EAX 0/1 (consumer: lane-prepare 00552EC5, result ignored).
int __fastcall kotor_fastcall_005c4c20(void* self, void* unused_edx);
// 005C5070 CTwoDimArrays::UnLoad2DArrays, RET, EAX 1 (consumer: ~CSWRules 00551457).
int __fastcall kotor_fastcall_005c5070(void* self, void* unused_edx);
// 005BFCC0 CTwoDimArrays::~CTwoDimArrays, RET (consumer: ~CSWRules 00551468).
void __fastcall kotor_fastcall_005bfcc0(void* self, void* unused_edx);
// 005C4730 LoadIPRPCostTables / 005C49C0 LoadIPRPParamTables, RET, EAX 0/1.
int __fastcall kotor_fastcall_005c4730(void* self, void* unused_edx);
int __fastcall kotor_fastcall_005c49c0(void* self, void* unused_edx);
// 005C4940 UnLoadIPRPCostTables / 005C4B90 UnLoadIPRPParamTables, RET, EAX 1.
int __fastcall kotor_fastcall_005c4940(void* self, void* unused_edx);
int __fastcall kotor_fastcall_005c4b90(void* self, void* unused_edx);
}
