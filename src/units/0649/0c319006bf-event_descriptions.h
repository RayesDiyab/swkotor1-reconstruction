#pragma once
// lane-prepare: CClientExoAppInternal::SetEventDescriptions 005EEB10
// (client_services::Dependencies::event_descriptions_005eeb10, D1 form).
// Callee naming epoch (agreed with lane-routes kotor_original_calls.h):
//   original __thiscall -> extern "C" __fastcall kotor_fastcall_<addr>(self, unused EDX, stack words...)
// Every kotor_fastcall_* below is a STILL-MISSING genuine provider (link UNDEF);
// stack-word counts are the callee RET immediates observed in swkotor.exe.
#include "c534281817-startup_string.h"
#include <cstdint>

extern "C" {
// CExoInput outer wrappers (mov ecx,[ecx+4]; jmp internal):
void __fastcall kotor_fastcall_005df600(void* input, void* edx);                       // ClearAllInputEventDescriptions -> 005E2F70, RET
void __fastcall kotor_fastcall_005df460(void* input, void* edx, std::uint32_t id, std::uint32_t type,
                                        std::uint32_t a, std::uint32_t b, std::uint32_t c); // CreateNewEvent -> 005E0E20, RET 14h
void __fastcall kotor_fastcall_005df470(void* input, void* edx, std::uint32_t id, std::uint32_t context); // AddEvent -> 005E0FA0, RET 8
void __fastcall kotor_fastcall_005df480(void* input, void* edx, std::uint32_t id, float magnitude, float exponent); // ScaleEvent -> 005E11D0, RET 0Ch
void __fastcall kotor_fastcall_005df490(void* input, void* edx, std::uint32_t id, std::int32_t wait, std::int32_t rate); // SetEventRepeatable -> 005E0520, RET 0Ch
int  __fastcall kotor_fastcall_005df6f0(void* input, void* edx, std::uint32_t id);     // EventExists, RET 4
// CClientExoApp::GetClientOptions 005ED700: mov eax,[ecx+4]; mov eax,[eax+4]; RET.
void* __fastcall kotor_fastcall_005ed700(void* client, void* edx);
// C2DA lookups by (CExoString row label, CExoString column), RET 0Ch.
int __fastcall kotor_fastcall_00414110(void* table, void* edx, kotor_reconstruction::StartupString* row,
                                       kotor_reconstruction::StartupString* column, std::int32_t* out);   // GetINTEntry
int __fastcall kotor_fastcall_00413fa0(void* table, void* edx, kotor_reconstruction::StartupString* row,
                                       kotor_reconstruction::StartupString* column, float* out);          // GetFLOATEntry
// CExoString(int) 005E5BC0: constructs *self from the decimal value, RET 4, EAX=self.
kotor_reconstruction::StartupString* __fastcall kotor_fastcall_005e5bc0(kotor_reconstruction::StartupString* self,
                                                                        void* edx, std::int32_t value);
// CExoString::AsINT 005E5630 (atoi of CStr; 0 for null data), RET.
// Rules cell 007A3A28 (single owner: lane-prepare prepare_image_data.cpp).
extern void* kotor_cell_rules_007a3a28;
}

namespace kotor_game_prepare {
// 005EEB10, RET (no stack arguments). ECX = CClientExoAppInternal*.
void __fastcall SetEventDescriptions005eeb10(void* self, void* edx);
}
