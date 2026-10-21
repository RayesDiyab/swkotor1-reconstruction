#pragma once
#include "93d73ae698-w1_ports.hpp"
// Server action queue and fade bookkeeping (W1-owned).
namespace w1::object_actions {
// 004CAC30 CSWSObject::AddActionNodeParameter(node, index, type, value), thiscall ret 0x10.
void add_action_node_parameter_004cac30(U object, U node, U index, U type, U value, Ports& p);
// 004CEA20 CSWSObject::AddAction(type, group (word; 0xFFFF new, 0xFFFE current), 13 x (param type, param value)),
// thiscall ret 0x70. `scratch` = 4-byte list position slot (the original reuses the type argument slot).
U add_action_004cea20(U object, U type, U group, const U (&params)[26], U scratch, Ports& p);
// 004CCB00 CSWSObject::AddActionToFront: as AddAction, queued at the head. thiscall ret 0x70.
U add_action_to_front_004ccb00(U object, U type, U group, const U (&params)[26], U scratch, Ports& p);
// 0057CB10 CSWSObject::AddDoCommandAction(situation), thiscall ret 4: 0 (situation deleted) when [+0xE8] == 0, else 1.
U add_do_command_action_0057cb10(U object, U situation, U scratch, Ports& p);
// 004CE8A0 CSWSObject::SetFadeOutTime(value), thiscall ret 4. `scratch` = 4-byte list position slot.
void set_fade_out_time_004ce8a0(U object, U value, U scratch, Ports& p);
}
