#pragma once
// lane-rules: typed CSWRules vtable 00748BB4 (D21/D41). The original .rdata table has a
// single slot (00748BB4 -> 00553020; 00748BB8 is string data, not a slot). Consumers
// store its ADDRESS into the object (00552C78 / 0055144F) - an object-pointer conversion
// (D24), never a function-pointer cast - and call the slot only through this struct.
#include <cstdint>
namespace kotor_rules {
struct CSWRulesMethods {
    // slot 0: 00553020 CSWRules scalar deleting destructor, __thiscall RET 4 -> D1 form.
    void* (__fastcall* deleting_00553020)(void* self, void* unused_edx, std::uint32_t flags);
};
static_assert(sizeof(CSWRulesMethods) == 4, "one observed slot");
// 00553020 CSWRules::`scalar deleting destructor', RET 4, EAX = this.
void* __fastcall kotor_fastcall_00553020(void* self, void* unused_edx, std::uint32_t flags);
// 00551440 CSWRules::~CSWRules, RET.
void __fastcall kotor_fastcall_00551440(void* self, void* unused_edx);
}
extern "C" const kotor_rules::CSWRulesMethods kotor_vtable_00748bb4;
