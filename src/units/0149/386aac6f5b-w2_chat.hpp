#pragma once
// W2 tier 6: CSWSMessage::SendServerToPlayerChatMessage 00571960.
// thiscall (BYTE mode, OBJECTID speaker, CExoString text (by value, destroyed by the callee), OBJECTID target,
// DWORD unused) RET 18h; EAX = success.
// Jump table 00571E7C (mode-1): 1 talk (broadcast 1000.0 + per-player talk within 1000.0), 2 shout, 3 whisper (3.0),
// 4 tell, 5 server tell, 6 party, 7..12 none (0), 13 broadcast 35.0 flag 1, 14 broadcast 1000.0 flag 0.
#include "1e16cfcf3d-server_mainloop.hpp"
namespace w2::chat {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;

struct Ports {
    virtual ~Ports() = default;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    virtual Word app_007a39fc() = 0;
    virtual Word player_list_004aed90(Word server) = 0;                 // W1 @recovered_server_player_list_004aed90@8
    virtual Word object_array_004aed70(Word server) = 0;
    virtual Word object_lookup_004d8230(Word array, Word id, Word out) = 0;   // AL status (0 = found)
    virtual Word object_area_004cb120(Word object) = 0;
    virtual Word vcall0(Word object, Word slot) = 0;
    virtual Word client_object_by_id_004ae910(Word server, Word id) = 0;
    virtual Word player_game_object_00561080(Word player) = 0;
    virtual Word list_at_pos_005e9b60(Word list, Word pos) = 0;
    virtual void list_next_005e9b90(Word list, Word pos_slot) = 0;
    virtual Word creature_faction_00513fc0(Word creature) = 0;
    virtual Word chat_talk_0056d7e0(Word msg, Word player_id, Word speaker, Word text) = 0;
    virtual Word chat_whisper_0056dbf0(Word msg, Word player_id, Word speaker, Word text) = 0;
    virtual Word chat_tell_0056d8c0(Word msg, Word player_id, Word speaker, Word text) = 0;
    virtual Word chat_server_tell_0056d9c0(Word msg, Word player_id, Word text) = 0;
    virtual Word chat_shout_0056da30(Word msg, Word player_id, Word speaker, Word text) = 0;
    virtual Word faction_send_chat_005be900(Word faction, Word speaker, Word text) = 0;
    virtual Word broadcast_dialog_004cd760(Word object, Word text, Word range_bits, Word flag) = 0;
    virtual void str_destroy_005e5c20(Word str) = 0;
};

// text: address of the by-value CExoString argument (destroyed before return). s: >= 0x20 bytes scratch.
Word send_chat_00571960(Memory&, Ports&, Word msg, Word mode, Word speaker, Word text, Word target, Word s);
}
