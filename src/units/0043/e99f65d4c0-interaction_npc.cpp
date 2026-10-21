// Original NPC click -> client input sender; queued conversation belongs to existing W1/server owners.
#include "db870c74e4-w5_t3.hpp"
#include "c9be7d3663-w5_server_env.hpp"
#include <cstring>
using namespace w5;
namespace kotor_input_required {
void* __fastcall get_in_game_gui_005ed690(void*, void*);
void* __fastcall get_character_006346c0(void*, void*, U);
}
extern "C" {
void* __fastcall kotor_fastcall_005ed6f0(void*, void*);
U __fastcall kotor_fastcall_0062f9a0(void*, void*);
void __fastcall kotor_fastcall_0062ec60(void*, void*, U);
void __fastcall kotor_fastcall_004d68a0(void*, void*, U, U, U);
void __fastcall kotor_fastcall_004d68e0(void*, void*, U);
U __fastcall kotor_fastcall_004d6440(void*, void*, void**, U*);
U __fastcall recovered_client_send_00677410(void*, void*, U, U, void*, U);
void __fastcall w5_object_clear_all_actions_004ccd80(void*, void*, U);
// [00677EA0,00677F22),130 bytes; original thiscall(targetID),RET4/native@12.
U __fastcall kotor_fastcall_00677ea0(void* message, void*, U target) {
    if (at<U>(kotor_input_required::get_in_game_gui_005ed690(client_app(), nullptr), 0xb4) != 0) return 1;
    kotor_fastcall_004d68a0(message, nullptr, 4, 0xffffffffu, 1);
    kotor_fastcall_004d68e0(message, nullptr, target);
    void* bytes; U size;
    if (kotor_fastcall_004d6440(message, nullptr, &bytes, &size) == 0) return 0;
    return recovered_client_send_00677410(message, nullptr, 6, 8, bytes, size);
}
// [0060F620,0060F77A),346 bytes; unused actionID/actor still require RET8/native@16.
void __fastcall kotor_fastcall_0060f620(void* target, void*, U, void*) {
    if (kotor_fastcall_0062f9a0(kotor_input_required::get_in_game_gui_005ed690(client_app(), nullptr), nullptr) == 0) return;
    if (at<U>(kotor_input_required::get_in_game_gui_005ed690(client_app(), nullptr), 0xb4) != 0) return;
    if (at<U>(target, 0x138) != 0) return;
    if (kotor_m15a_object_get_server_object_0063d4b0(target, nullptr) == nullptr) return;
    w5_object_clear_all_actions_004ccd80(kotor_m15a_object_get_server_object_0063d4b0(target, nullptr), nullptr, 1);
    void* pc = kotor_input_required::get_character_006346c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, 0);
    if (pc != nullptr && at<U>(kotor_m15a_object_get_server_object_0063d4b0(target, nullptr), 0x21c) != 0 &&
        at<U>(kotor_m15a_object_get_server_object_0063d4b0(target, nullptr), 0xe8) != 0) {
        float pc_position[3], target_position[3], direction[3];
        std::memcpy(pc_position, static_cast<unsigned char*>(pc) + 0x24, sizeof(pc_position));
        std::memcpy(target_position, static_cast<unsigned char*>(target) + 0x24, sizeof(target_position));
        // Original FLD/FSUB/FSTP for each component, preserving subnormal and rounding behavior.
        __asm {
            fld dword ptr pc_position[0]
            fsub dword ptr target_position[0]
            fstp dword ptr direction[0]
            fld dword ptr pc_position[4]
            fsub dword ptr target_position[4]
            fstp dword ptr direction[4]
            fld dword ptr pc_position[8]
            fsub dword ptr target_position[8]
            fstp dword ptr direction[8]
        }
        if (kotor_m15a_object_get_server_object_0063d4b0(target, nullptr) != nullptr)
            w1x_object_set_orientation_004cd170(kotor_m15a_object_get_server_object_0063d4b0(target, nullptr), nullptr, direction);
    }
    const U id = at<U>(target, 4);
    (void)kotor_fastcall_00677ea0(kotor_fastcall_005ed6f0(client_app(), nullptr), nullptr, id);
    kotor_fastcall_0062ec60(kotor_input_required::get_in_game_gui_005ed690(client_app(), nullptr), nullptr, 1);
}
}
