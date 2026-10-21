// W5 successor: CSWSObject::AIActionDoCommand 0057B530 (thiscall(CSWSObjectActionNode*) RET 4).
// Transcribed from the original swkotor.exe (read-only); addresses in comments are original instructions.
#include "842d6b1618-w5_t3.hpp"

using namespace w5;

namespace frame_control { extern unsigned message_manager_007a3a00; }              // [007A3A00] CVirtualMachine*
extern "C" {
unsigned __fastcall recovered_creature_incapacitated_004ef890(void* creature, void*);  // CSWSCreature::GetDeadTemp
unsigned __fastcall kotor_m15e_slot010_zero_0063e7f0(void* object, void*);             // 0063E7F0 return_zero
unsigned __fastcall recovered_vm_run_script_situation_005d0fd0(void* vm, void*, unsigned situation,
                                                               unsigned object_id, unsigned flag); // RunScriptSituation
}

namespace {
constexpr unsigned k_action_complete_00745dc0 = 2;   // .rdata 00745DC0
constexpr unsigned k_action_failed_00745dc4 = 3;     // .rdata 00745DC4
}

extern "C" unsigned __fastcall w5_action_do_command_0057b530(void* self, void*, void* node) {
    unsigned dead = 0;                                                                   // 0057B536 edi
    if (vcall<void*>(self, 0x30) != nullptr)                                             // 0057B533..0057B53D AsSWSCreature
        dead = recovered_creature_incapacitated_004ef890(vcall<void*>(self, 0x30), nullptr); // 0057B53F..0057B54D
    if (vcall<unsigned>(self, 0x94) == 1) return k_action_failed_00745dc4;               // 0057B54F..0057B55C
    if (kotor_m15e_slot010_zero_0063e7f0(self, nullptr) == 1) return k_action_failed_00745dc4; // 0057B55E..0057B568
    if (dead == 1) return k_action_failed_00745dc4;                                      // 0057B56A..0057B56D
    unsigned situation;
    std::memcpy(&situation, static_cast<unsigned char*>(node) + 0x38, 4);                // 0057B576
    unsigned id;
    std::memcpy(&id, static_cast<unsigned char*>(self) + 4, 4);                          // 0057B56F
    recovered_vm_run_script_situation_005d0fd0(reinterpret_cast<void*>(frame_control::message_manager_007a3a00),
                                               nullptr, situation, id, 1);               // 0057B579..0057B583
    const unsigned zero = 0;
    std::memcpy(static_cast<unsigned char*>(node) + 0x38, &zero, 4);                     // 0057B588
    return k_action_complete_00745dc0;                                                   // 0057B58F
}
