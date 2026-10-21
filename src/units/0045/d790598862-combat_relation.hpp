#pragma once
extern "C" {
void __fastcall kotor_fastcall_004fbbe0(void*, void*, unsigned);
void __fastcall kotor_fastcall_004ec8e0(void*, void*);
void __fastcall kotor_fastcall_004f3580(void*, void*, unsigned);
unsigned __fastcall kotor_fastcall_00569ec0(void*, void*, void*, unsigned, unsigned);
// Native wrappers only: existing 1039 faction and W2 G07 member-list bodies retain ownership.
void* __fastcall combat_reuse_faction_00513fc0(void*, void*);
unsigned __fastcall combat_reuse_members_005be780(void*, void*, void**);
void* __cdecl combat_reuse_message_004aedb0();
}
