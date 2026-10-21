// Private reconstruction contract for original 00511130
// CSWSCreature::AIActionPartyFollowLeader.
//
// Evidence (read-only):
//   original body  : swkotor.exe file offset 0x111130, 0x90 = 144 bytes
//                    sha256 4b2bae5e7ddf4e2407e41f7d84479e6992587fa31e642ee46402f2cd443515c8
//   original disasm: ...\disasm\00511130.asm (header "len=900" is a disassembly
//                    window, not the function size; M2-LANES.json records 0x90)
//   next function  : 005111C0 CSWSCreature::ComputeTotalWalkDistance
//
// ABI: thiscall (ecx = this), one stack argument (the action node), RET 4.
// Modeled as __fastcall(self, unused edx, node) => decorated @12 and RET 4,
// which is the established W5 action-body contract.  The action node is not
// read by the original body.
//
// No x87 / floating point / allocation / exception path exists in this body.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace kotor_00511130 {

// Exact field offsets read by 00511130.  Names are deliberately neutral: only
// the offsets and widths are observed facts, the semantic names are not.
struct CSWSCreature {
    void** vtable;                 // +0x000  mov eax,[esi]
    std::uint32_t object_id;       // +0x004  mov edi,[esi+4]
    std::byte pad_008[0x340 - 0x008];
    void* field_340;               // +0x340  mov eax,[esi+0x340]; test eax,eax
    std::byte pad_344[0x4C0 - 0x344];
    void* field_4C0;               // +0x4C0  mov eax,[esi+0x4C0]; test eax,eax
    std::byte pad_4C4[0x9F0 - 0x4C4];
    std::uint8_t flags_9F0;        // +0x9F0  test byte [esi+0x9F0],2
    std::byte pad_9F1[0xA88 - 0x9F1];
    void* field_A88;               // +0xA88  mov eax,[esi+0xA88]; test eax,eax
    std::int32_t field_A8C;        // +0xA8C  mov dword [esi+0xA8C],3
};

static_assert(sizeof(void*) == 4, "original is 32-bit x86");
static_assert(offsetof(CSWSCreature, vtable) == 0x000, "observed +0x000");
static_assert(offsetof(CSWSCreature, object_id) == 0x004, "observed +0x004");
static_assert(offsetof(CSWSCreature, field_340) == 0x340, "observed +0x340");
static_assert(offsetof(CSWSCreature, field_4C0) == 0x4C0, "observed +0x4C0");
static_assert(offsetof(CSWSCreature, flags_9F0) == 0x9F0, "observed +0x9F0");
static_assert(offsetof(CSWSCreature, field_A88) == 0xA88, "observed +0xA88");
static_assert(offsetof(CSWSCreature, field_A8C) == 0xA8C, "observed +0xA8C");
static_assert(sizeof(CSWSCreature) == 0xA90, "last observed field ends at 0xA90");

// .rdata dwords in the original image; read-only, never written:
//   00745DBC = 1  ACTION_IN_PROGRESS
//   00745DC4 = 3  ACTION_FAILED
// 005111AB loads [00745DBC]; 005111B4 loads [00745DC4].  Because both cells
// are immutable, a constexpr value is behaviorally identical.  If a future
// data owner exports the cells, replace these two constants with the exported
// symbols without changing the control flow.
inline constexpr unsigned kActionInProgress_00745DBC = 1u;
inline constexpr unsigned kActionFailed_00745DC4 = 3u;

// [this] + 0x94 is CSWSCreature::GetDead (original 004EF820).  The selected V4
// reconstructed CSWSCreature table (0963-r2-fix.obj, creature_table at VA
// 0x00883DA0) stores 0x00598580 (creature_dead, the faithful 004EF820 body) in
// slot +0x94.  The call uses ecx = this and returns the dead flag in eax.
using GetDeadSlot = unsigned(__fastcall*)(void* self, void* unused_edx);

inline unsigned call_get_dead_slot_94(CSWSCreature* self) {
    // memcpy keeps the slot read exact and avoids a function-pointer/data-pointer
    // reinterpret_cast; MSVC lowers this to a single load at /O2.
    GetDeadSlot fn = nullptr;
    std::memcpy(&fn, &self->vtable[0x94 / 4], sizeof(fn));
    return fn(self, nullptr);
}

}  // namespace kotor_00511130

// Selected real providers from the V4 map.  See RECEIPT.json for the exact
// VA/object/hash pins and closure.
extern "C" {
unsigned __fastcall kotor_m15e_slot010_zero_0063e7f0(void*, void*);
unsigned __fastcall recovered_creature_incapacitated_004ef890(void*, void*);
void* __fastcall recovered_object_area_004cb120(void*, void*);
unsigned __fastcall kotor_fastcall_004aea40(void*, void*);

// Canonical W5 action-body symbol for original 00511130.  Decorated:
//   @w5_action_party_follow_leader_00511130@12   (RET 4)
// If the integration uses a distinct resolver symbol, rename this one
// definition; the body and ABI must not change.
unsigned __fastcall w5_action_party_follow_leader_00511130(void* self, void* edx_unused,
                                                           void* node);
}
