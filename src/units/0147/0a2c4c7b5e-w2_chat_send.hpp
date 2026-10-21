#pragma once
// W2 tier 6: server -> client chat senders (major 09h, framing via SendServerToPlayerMessage 00567E60).
//   0056D7E0 SendServerToPlayerChat_Talk       (player, speaker, CExoString*) RET 0Ch  minor 01: OBJECTID, CExoString
//   0056DA30 SendServerToPlayerChat_Shout      (player, speaker, CExoString*) RET 0Ch  minor 02: OBJECTID, CExoString, FLOAT x3, LocString x2
//   0056DBF0 SendServerToPlayerChat_Whisper    (player, speaker, CExoString*) RET 0Ch  minor 03: OBJECTID, CExoString
//   0056D8C0 SendServerToPlayerChat_Tell       (player, speaker, CExoString*) RET 0Ch  minor 04: as Shout (speaker must exist)
//   0056D9C0 SendServerToPlayerChat_ServerTell (player, CExoString*)          RET 8    minor 05: CExoString
//   0056D6E0 SendServerToPlayerChat_Party      (player, speaker, CExoString*) RET 0Ch  minor 06: as Tell
//   005BE900 CSWSFaction::SendChatMessage      (speaker, CExoString*)         RET 8    Party to every faction member with a client
// EAX: 0 when the speaker creature is missing (Tell / Party) or GetWriteMessage fails, else the send result.
// Message writers / framing are the existing W2 + 0898 owners (w2_message, module_message).
#include "913ae857a3-w2_message.hpp"
namespace w2::chat_send {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;

struct Ports {
    virtual ~Ports() = default;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    virtual Word app_007a39fc() = 0;
    virtual Word creature_by_id_004ae770(Word server, Word id) = 0;
    virtual Word client_object_by_id_004ae910(Word server, Word id) = 0;
    virtual Word sws_message_004aedb0(Word server) = 0;
    virtual Word vcall0(Word object, Word slot) = 0;                 // thiscall [[object]+slot](object): 8Ch first / 90h last name
    virtual void loc_construct_005e9e10(Word loc) = 0;
    virtual void loc_destroy_005ea000(Word loc) = 0;
};

// s: >= 0x30 bytes of scratch (GetWriteMessage outputs, Shout's two CExoLocStrings, WriteCExoLocStringServer locals).
Word talk_0056d7e0(Memory&, w2::msg::LocPorts&, Ports&, Word msg, Word player, Word speaker, Word text, Word s);
Word whisper_0056dbf0(Memory&, w2::msg::LocPorts&, Ports&, Word msg, Word player, Word speaker, Word text, Word s);
Word tell_0056d8c0(Memory&, w2::msg::LocPorts&, Ports&, Word msg, Word player, Word speaker, Word text, Word s);
Word server_tell_0056d9c0(Memory&, w2::msg::LocPorts&, Ports&, Word msg, Word player, Word text, Word s);
Word shout_0056da30(Memory&, w2::msg::LocPorts&, Ports&, Word msg, Word player, Word speaker, Word text, Word s);
Word party_0056d6e0(Memory&, w2::msg::LocPorts&, Ports&, Word msg, Word player, Word speaker, Word text, Word s);
Word faction_send_chat_005be900(Memory&, w2::msg::LocPorts&, Ports&, Word faction, Word speaker, Word text, Word s);
}
