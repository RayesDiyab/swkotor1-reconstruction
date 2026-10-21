#pragma once
// W7 tier t5 exports (ACTION ABI as in vm_actions_t1.hpp).
#include <cstdint>
extern "C" {
int __fastcall recovered_vm_action_move_to_object_0053fb00(void*, void*, int, unsigned);   // 22 ActionMoveToObject, 383 ActionForceMoveToObject
int __fastcall recovered_vm_set_party_leader_00545a70(void*, void*, int, unsigned);        // 13
int __fastcall recovered_vm_set_global_fade_in_00546010(void*, void*, int, unsigned);      // 719
int __fastcall recovered_vm_set_global_fade_out_005460d0(void*, void*, int, unsigned);     // 720
int __fastcall recovered_creature_add_move_to_point_action_004f8b60(void*, void*, std::uint32_t group, const float* point, std::uint32_t area_id,
    std::uint32_t target_id, std::uint32_t run, std::uint32_t range, std::uint32_t a6, std::uint32_t a7, std::uint32_t a8, std::uint32_t a9,
    std::uint32_t a10, std::uint32_t a11, std::uint32_t a12, const float* a13, std::uint32_t a14, std::uint32_t a15, std::uint32_t a16);
}
