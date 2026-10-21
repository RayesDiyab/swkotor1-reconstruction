#pragma once
// W2 tier 6 (fork D): GFF save of the creature combat round, listen data and party-follow info.
//   004D3EC0 CSWSCombatRound::SaveCombatRound(CResGFF*, CResStruct*) RET 8 -> 1
//   004D2210 CSWSCombatAttackData::SaveData(CResGFF*, CResStruct*) RET 8 -> 1
//   004D1DD0 CSWSCombatRoundAction::SaveData(CResGFF*, CResStruct*) RET 8 -> 1
//   004CCA50 CSWSObject::SaveListenData(CResGFF*, CResStruct*) RET 8 -> EAX = expression count (no explicit return value)
//   004EAF70 CSWSCreaturePartyFollowInfo::Save(CResGFF*, CResStruct*) RET 8 -> EAX = result of the last WriteFieldBYTE
// CResStruct is 4 bytes (struct index), CResList 20 bytes (index + 16-byte label), as written by the 0869 owners.
#include "1e16cfcf3d-server_mainloop.hpp"
namespace w2::save_combat {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;

// GFF writer identities: the original callee address selects the CResGFF::WriteField* overload.
enum : Word {
    kByte = 0x00412620, kChar = 0x00412670, kWord = 0x004126c0, kShort = 0x00412710, kDword = 0x00412760,
    kInt = 0x004127b0, kFloat = 0x00412870, kString = 0x00412970, kVector = 0x00412d30,
};

struct Ports {
    virtual ~Ports() = default;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    // CResGFF::WriteField<writer>(struct*, value, label) RET 0Ch. value: the dword pushed (zero-extended BYTE/WORD,
    // raw float bits for FLOAT, the object address for CExoString / Vector).
    virtual Word write_field(Word writer, Word gff, Word structure, Word value, const char* label) = 0;
    virtual Word add_list(Word gff, Word out_list, Word parent, const char* label) = 0;          // 00412450
    virtual Word add_list_element(Word gff, Word out_struct, Word list, Word type) = 0;          // 004124E0
    virtual Word list_at_pos_005e9b60(Word internal, Word pos) = 0;                              // CExoLinkedListInternal::GetAtPos
    virtual void list_next_005e9b90(Word internal, Word pos_slot) = 0;                           // GetNext(&pos)
};

// scratch: >= 0x20 bytes (list at +0, element struct at +14h, linked-list position at +18h; nested saves at +20h)
Word save_combat_round_004d3ec0(Memory&, Ports&, Word round, Word gff, Word structure, Word scratch);
Word attack_data_save_004d2210(Memory&, Ports&, Word attack, Word gff, Word structure, Word scratch);
Word round_action_save_004d1dd0(Memory&, Ports&, Word action, Word gff, Word structure);
Word save_listen_data_004cca50(Memory&, Ports&, Word object, Word gff, Word parent, Word scratch);
Word follow_info_save_004eaf70(Memory&, Ports&, Word info, Word gff, Word structure);
}
