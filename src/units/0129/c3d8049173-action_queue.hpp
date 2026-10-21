#pragma once
#include "83a9f72867-w1_ports.hpp"
// CSWSObject action-queue group helpers (shared by perception, RunActions and the event handlers).
namespace w1::action_queue {
// 0057A240 CSWSObject::GetAcceptableAction(type), thiscall ret 4 (does not touch the object).
U acceptable_action_0057a240(U type);
// 004CC690 CSWSObject::GetNumActionGroups, thiscall ret. `scratch` = 4-byte list position slot.
U num_groups_004cc690(U object, U scratch, Ports& p);
// 004CC5C0 CSWSObject::GetPositionByGroupIndex(index), thiscall ret 4.
U position_by_group_index_004cc5c0(U object, U index, U scratch, Ports& p);
// 004CC530 CSWSObject::GetActionByGroupId(WORD group, node** out), thiscall ret 8. Returns AX (action id or 0xFFFF).
U action_by_group_id_004cc530(U object, U group, U out, U scratch, Ports& p);
// 004CC4D0 CSWSObjectActionNode::~CSWSObjectActionNode, thiscall ret.
void action_node_destroy_004cc4d0(U node, Ports& p);
// 004CC6F0 CSWSObject::RemoveGroup(WORD group), thiscall ret 4.
void remove_group_004cc6f0(U object, U group, U scratch, Ports& p);
// 004F61D0 CSWSCreature::ClearHostileActionsVersus(CSWSObject* target), thiscall ret 4. `scratch` = 12 bytes.
void clear_hostile_004f61d0(U creature, U target, U scratch, Ports& p);
}
