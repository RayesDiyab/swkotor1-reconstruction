// Exact original combat-round action construction and insertion; real list/heap owners only.
#include "eb237338e8-w5_server_env.hpp"
#include "0a6194515c-w5_action_queue.hpp"
#include <cstring>
using namespace w5;
namespace kotor_combat_round_cells {
// Sole real scalar owner, zero-initialized like the original PE loader; storage gate fccedd42.
volatile std::int32_t word_008327dc = 0;
}
extern "C" {
void* __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
void* __fastcall recovered_linked_list_add_before_005e9ad0(void*, void*, void*, void*);
// [004D1D50,004D1DC8),120bytes,RET0/native@8; preserve untouched padding/fields.
void __fastcall kotor_fastcall_004d1d50(void* action, void*) {
    at<U>(action, 0) = 0; at<U>(action, 8) = 0; at<U16>(action, 4) = 0;
    at<U8>(action, 0x10) = 0; at<U>(action, 0x0c) = 0;
    at<U>(action, 0x14) = 0x7f000000; at<U>(action, 0x18) = 1; at<U>(action, 0x1c) = 0;
    at<U>(action, 0x20) = 0x7f000000;
    for (U field = 0x24; field <= 0x34; field += 4) at<U>(action, field) = 0;
    at<U>(action, 0x44) = 0x7f000000; at<U>(action, 0x48) = 0; at<U>(action, 0x4c) = 0;
    at<U8>(action, 0x50) = 0; at<U>(action, 0x54) = 0; at<U>(action, 0x58) = 0;
    at<U>(action, 0x5c) = 0; at<U8>(action, 0x60) = 0;
    at<U>(action, 0x64) = 0x7f000000; at<U>(action, 0x68) = 0; at<U>(action, 0x6c) = 0;
    at<U16>(action, 0x70) = 0; at<U16>(action, 0x72) = 0;
    at<U>(action, 0x74) = 0; at<U>(action, 0x78) = 0; at<U>(action, 0x7c) = 4;
    at<U>(action, 0x80) = 0; at<U>(action, 0x84) = 0;
}
// [004D2FF0,004D3003),19bytes,RET0/native@8.
void* __fastcall kotor_fastcall_004d2ff0(void* action, void*) {
    at<U>(action, 0x38) = 0; at<U>(action, 0x3c) = 0; at<U>(action, 0x40) = 0;
    kotor_fastcall_004d1d50(action, nullptr);
    return action;
}
// [004D3660,004D3762),258bytes,RET8/native@16; four-entry capacity and signed stable ordering.
void __fastcall kotor_fastcall_004d3660(void* round, void*, void* action, I index) {
    void* wrapper = at<void*>(round, 0x9b0);
    if (wrapper == nullptr || at<U>(at<void*>(wrapper, 0), 8) >= 4) {
        if (action != nullptr) live_free(action);
        return;
    }
    void* position = at<void*>(at<void*>(wrapper, 0), 0);
    if (index == -1) {
        while (position != nullptr) {
            void* current = unresolved_list_at_pos_005e9b60(at<void*>(at<void*>(round, 0x9b0), 0), nullptr, position);
            if (at<I>(action, 0) < at<I>(current, 0)) {
                (void)recovered_linked_list_add_before_005e9ad0(at<void*>(at<void*>(round, 0x9b0), 0), nullptr, action, position);
                return;
            }
            (void)unresolved_list_next_005e9b90(at<void*>(at<void*>(round, 0x9b0), 0), nullptr, &position);
        }
    } else {
        I current = 0;
        while (position != nullptr && current < index) {
            (void)unresolved_list_next_005e9b90(at<void*>(at<void*>(round, 0x9b0), 0), nullptr, &position);
            ++current;
        }
        if (position != nullptr) {
            (void)recovered_linked_list_add_before_005e9ad0(at<void*>(at<void*>(round, 0x9b0), 0), nullptr, action, position);
            return;
        }
    }
    (void)resource_append_005e9dc0(at<void*>(at<void*>(round, 0x9b0), 0), nullptr, action);
}
// [004D38B0,004D39A5),245bytes,RET14/native@28; unused fourth argument retained.
void __fastcall kotor_fastcall_004d38b0(void* round, void*, U target_id, U mode, U arg3, U, U arg5) {
    void* target = kotor_fastcall_004ae750(server_app(), nullptr, target_id);
    if (target == nullptr || vcall<void*>(target, 0x10) == nullptr ||
        vcall<U>(vcall<void*>(target, 0x10), 0x94) != 0) return;
    if (vcall<void*>(target, 0x18) != nullptr && at<U8>(vcall<void*>(target, 0x18), 0x2cc) != 0) return;
    void* memory = live_operator_new(0x88);
    void* action = memory != nullptr ? kotor_fastcall_004d2ff0(memory, nullptr) : nullptr;
    if (static_cast<U16>(mode) != 0) {
        at<U8>(action, 0x10) = 0x0b;
        at<U>(action, 0x5c) = static_cast<U16>(mode);
    } else at<U8>(action, 0x10) = 1;
    at<U>(action, 0x84) = arg3;
    at<U16>(action, 4) = 0x2719;
    const U wrapped = static_cast<U>(kotor_combat_round_cells::word_008327dc) + 3000u;
    I signed_value;
    std::memcpy(&signed_value, &wrapped, 4);
    at<I>(action, 8) = signed_value / 2; // original ADD/CDQ/SUB/SAR: wrap then signed truncation
    at<U>(action, 0x0c) = arg5;
    at<U>(action, 0x14) = target_id;
    kotor_fastcall_004d3660(round, nullptr, action, -1);
    void* creature = at<void*>(round, 0x9b4);
    if (at<U>(creature, 0x50c) == 0x7f000000u) at<U>(creature, 0x50c) = target_id;
}
}
