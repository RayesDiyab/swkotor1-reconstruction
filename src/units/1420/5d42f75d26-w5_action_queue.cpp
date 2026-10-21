// W5 action-queue producers: CSWSCreature::AddDriveAction (004EB510) over W1's CSWSObject::AddAction (004CEA20). Node layout and the runner contract: ACTION-ABI.md.
#include "71303376b8-w5_action_queue.hpp"
#include <intrin.h>

using namespace w5;

extern "C" {

// 004CAC30 AddActionNodeParameter and 004CEA20 AddAction are W1's (ownership ruling; lane-w1 tier5b
// @recovered_object_add_action_node_parameter_004cac30@24 / @recovered_object_add_action_004cea20@120).
// w5_add_action_body keeps the W5 producers' (type, group, pairs[26]) shape and calls W1's export with the
// 28 stack dwords of the original thiscall RET 70h.
void __cdecl w5_add_action_body(void* object, U type, U group_word, const U* q) {
    auto v = [&](int i) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(q[i])); };
    recovered_object_add_action_004cea20(object, nullptr, type, group_word,
        q[0], v(1), q[2], v(3), q[4], v(5), q[6], v(7), q[8], v(9), q[10], v(11), q[12], v(13),
        q[14], v(15), q[16], v(17), q[18], v(19), q[20], v(21), q[22], v(23), q[24], v(25));
}

// 004EB510 AddDriveAction(word group, Vector* clientPosition, word heading10, byte a, byte flags, int mode):
// thiscall RET 18h. EAX 0 when the creature has no area, else 1.
U __fastcall w5_add_drive_action_004eb510(void* creature, void*, U group, const Vector* position, U heading,
    U arg4, U flags, U mode) {
    if (recovered_object_area_004cb120(creature, nullptr) == nullptr) return 0;   // 004EB514..004EB51F
    // [esp+7] = 1 is the top byte of the pushed-ECX slot; the parameter reads the DWORD at that address,
    // whose upper three bytes are the low bytes of this call's return address (original quirk). The
    // consumer (AIActionDrive) only tests the DWORD for nonzero.
    const U ra = static_cast<U>(reinterpret_cast<std::uintptr_t>(_ReturnAddress()));
    U first_run = 1u | (ra << 8);
    if (at<U>(creature, 0x8e8) != 0) flags &= 0xfffdu;                     // 004EB522..004EB531 (dword and)
    U x, y; std::memcpy(&x, &position->x, 4); std::memcpy(&y, &position->y, 4);
    const U pairs[26] = {2, reinterpret_cast<U>(&x), 2, reinterpret_cast<U>(&y), 1, reinterpret_cast<U>(&heading),
        1, reinterpret_cast<U>(&arg4), 1, reinterpret_cast<U>(&flags), 1, reinterpret_cast<U>(&mode),
        1, reinterpret_cast<U>(&first_run), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    w5_add_action_body(creature, 0x33, group, pairs);                      // 004EB58A AddAction(33h, ...)
    return 1;
}
}
