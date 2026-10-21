// Original 00511130..005111BC (0x90 = 144 bytes including the 3-byte NOP pad)
// CSWSCreature::AIActionPartyFollowLeader.
//
// Transcribed from the read-only original disassembly (00511130.asm) and the
// full swkotor.exe disassembly.  Private reconstruction source only; this file
// has not been compiled, linked or run by the author.
//
// Control flow (every branch below is present in the original):
//   00511133  [this+0xA88] == 0                         -> 3
//   0051113D  [this+0x4C0] == 0                         -> 3
//   00511149  vtable[0x94]() == 1                       -> 3
//   00511156  0063E7F0(this) == 1                       -> 3
//   00511162  004EF890(this) == 1                       -> 3
//   0051116C  [this+0x340] == 0                         -> 3
//   00511176  ([this+0x9F0] & 0x02) == 0                -> 3
//   00511181  area = 004CB120(this)
//   00511186  area == 0                                 -> 1
//   0051118A  manager = [0x7A39FC]; server = [manager+8]
//   00511194  object_id = [this+4]                      (cached before call)
//   00511197  player_id = 004AEA40(server)
//   0051119C  player_id == object_id                    -> 3
//   005111A1  [this+0xA8C] = 3
//   005111AB  return 1
//   005111B4  return 3
//
// The single stack argument (the action node) is never read.
#include "39ded69227-aiaction_party_follow_leader_00511130.hpp"

// Selected global data cell.  The V4 symbol is
//   ?manager_007a39fc@kotor_menu_window_cells@@3RAXA  (0053-window-shared-cells.obj)
// and is declared with the same volatile pointer type used by the accepted
// reconstruction sources.  The original loads this cell without a null guard;
// the dereference below intentionally preserves that unchecked-null quirk.
namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
}

namespace {

template <typename T>
T read_at(const void* base, std::size_t offset) {
    T value;
    std::memcpy(&value, static_cast<const unsigned char*>(base) + offset, sizeof(value));
    return value;
}

}  // namespace

extern "C" unsigned __fastcall w5_action_party_follow_leader_00511130(
    void* self_raw, void* /*edx_unused*/, void* /*node*/) {
    using namespace kotor_00511130;
    CSWSCreature* const self = static_cast<CSWSCreature*>(self_raw);

    // 00511133..0051113B
    if (self->field_A88 == nullptr) return kActionFailed_00745DC4;

    // 0051113D..00511145
    if (self->field_4C0 == nullptr) return kActionFailed_00745DC4;

    // 00511147..00511152: virtual [this]->vtable+0x94 (CSWSCreature::GetDead)
    if (call_get_dead_slot_94(self) == 1u) return kActionFailed_00745DC4;

    // 00511154..0051115E: 0063E7F0 is exactly xor eax,eax; ret
    if (kotor_m15e_slot010_zero_0063e7f0(self, nullptr) == 1u) return kActionFailed_00745DC4;

    // 00511160..0051116A: 004EF890 CSWSCreature::GetDeadTemp
    if (recovered_creature_incapacitated_004ef890(self, nullptr) == 1u)
        return kActionFailed_00745DC4;

    // 0051116C..00511174
    if (self->field_340 == nullptr) return kActionFailed_00745DC4;

    // 00511176..0051117D
    if ((self->flags_9F0 & 0x02u) == 0u) return kActionFailed_00745DC4;

    // 0051117F..00511188: 004CB120 CSWSCreature/ServerObject GetArea.
    if (recovered_object_area_004cb120(self, nullptr) != nullptr) {
        // 0051118A..00511190: original is an unchecked [0x7A39FC] -> [manager+8].
        void* const manager = kotor_menu_window_cells::manager_007a39fc;
        void* const server = read_at<void*>(manager, 8);

        // 00511194: the object id is loaded into edi BEFORE the player query.
        // Keep the cache: the comparison must use the pre-call value.
        const std::uint32_t object_id = self->object_id;

        // 00511197: 004AEA40 CSWSCreature/CServerExoApp GetPlayerCreatureId.
        const unsigned player_id = kotor_fastcall_004aea40(server, nullptr);

        // 0051119C..0051119F
        if (player_id == object_id) return kActionFailed_00745DC4;

        // 005111A1..005111AB
        self->field_A8C = 3;
    }

    // 005111AB..005111B3: in progress (area == 0 falls through here too).
    return kActionInProgress_00745DBC;
}
