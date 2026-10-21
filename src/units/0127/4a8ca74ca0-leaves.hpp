#pragma once
#include "93d73ae698-w1_ports.hpp"
// Shared W1 leaves: CSWCCMessageData ctor/dtor, feedback messages, party invitation bookkeeping.
namespace w1::leaves {
// 004D69F0 CSWCCMessageData::CSWCCMessageData, thiscall ret; returns this.
U message_data_construct_004d69f0(U self, Ports& p);
// 004D6A40 CSWCCMessageData::~CSWCCMessageData, thiscall ret.
void message_data_destroy_004d6a40(U self, Ports& p);
// 004D6480 CSWCCMessageData::GetInteger(index), thiscall ret 4: [+4][index] when index < [+8] (signed), else 0.
U message_get_integer_004d6480(U self, U index, Ports& p);
// 004D64C0 CSWCCMessageData::GetObjectID(index), thiscall ret 4: [+0x28][index] when index < [+0x2C] (signed), else INVALID.
U message_get_object_004d64c0(U self, U index, Ports& p);
// 004EDE10 CSWSCreature::SendFeedbackMessage(WORD id, CSWCCMessageData* data), thiscall ret 8.
void send_feedback_004ede10(U creature, U id, U data, Ports& p);
// 004EF520 CSWSCreature::SetInvitedToParty(int), thiscall ret 4.
void set_invited_004ef520(U creature, U value, Ports& p);
// 00518660 CSWSCreature::CleanInvitationLists, thiscall ret. `frame` = 0x34-byte scratch (frame+N == [esp+N]).
void clean_invitations_00518660(U creature, U frame, Ports& p);
}
