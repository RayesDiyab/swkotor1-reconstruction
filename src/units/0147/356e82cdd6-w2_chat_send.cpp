#include "0a2c4c7b5e-w2_chat_send.hpp"
#include "74be47154e-module_message.hpp"
#include <cstdint>
namespace w2::chat_send {
namespace sm = kotor::server_mainloop;
namespace msg = w2::msg;
namespace {
using I = std::int32_t;
constexpr Word kMajorChat = 9, kOne = 0x3f800000u, kBits = 0x20, kInvalid = 0x7f000000u;
constexpr Word kShoutNoSpeakerStrRef = 0x10782;                       // 0056DB24
Word server_of(Memory& m, Ports& x) { return m.read(x.app_007a39fc() + 8); }
// GetWriteMessage(&data, &size) then SendServerToPlayerMessage(player, 9, minor, data, size); 0 when the message is empty.
Word finish_and_send(Memory& m, msg::LocPorts& p, Word self, Word player, Word minor, Word s) {
    const Word data = s, size = s + 4;
    if (sm::message_finish_004d6440(m, self, data, size) == 0) return 0;
    return sm::message_send_00567e60(m, p, self, player, kMajorChat, minor, m.read(data), m.read(size));
}
void begin(Memory& m, msg::LocPorts& p, Word self, Word text, Word extra) {
    sm::message_begin_004d68a0(m, p, self, p.str_length_005e5790(text) + extra, 0xffffffffu, 1);   // CreateWriteMessage(len+extra, -1, 1)
}
// OBJECTID, CExoString, position x/y/z (FLOAT scale 1.0), first and last name (WriteCExoLocStringServer gender 0)
void write_speaker(Memory& m, msg::LocPorts& p, Ports& x, Word self, Word speaker_id, Word creature, Word text, Word s) {
    msg::write_object_id_00578570(m, p, self, speaker_id);
    msg::write_string_004d6960(m, p, self, text, kBits);
    msg::write_float_bits_004d6920(m, p, self, m.read(creature + 0x90), kOne, kBits);
    msg::write_float_bits_004d6920(m, p, self, m.read(creature + 0x94), kOne, kBits);
    msg::write_float_bits_004d6920(m, p, self, m.read(creature + 0x98), kOne, kBits);
    msg::write_locstring_005785c0(m, p, self, x.vcall0(creature, 0x8c), 0, s + 8);
    msg::write_locstring_005785c0(m, p, self, x.vcall0(creature, 0x90), 0, s + 8);
}
Word simple(Memory& m, msg::LocPorts& p, Word self, Word player, Word speaker, Word text, Word minor, Word s) {
    begin(m, p, self, text, 8);
    msg::write_object_id_00578570(m, p, self, speaker);
    msg::write_string_004d6960(m, p, self, text, kBits);
    return finish_and_send(m, p, self, player, minor, s);
}
Word named(Memory& m, msg::LocPorts& p, Ports& x, Word self, Word player, Word speaker, Word text, Word extra, Word minor, Word s) {
    const Word c = x.creature_by_id_004ae770(server_of(m, x), speaker);
    if (c == 0) return 0;                                              // before CreateWriteMessage
    begin(m, p, self, text, extra);
    write_speaker(m, p, x, self, speaker, c, text, s);
    return finish_and_send(m, p, self, player, minor, s);
}
}

Word talk_0056d7e0(Memory& m, msg::LocPorts& p, Ports&, Word self, Word player, Word speaker, Word text, Word s) { return simple(m, p, self, player, speaker, text, 1, s); }
Word whisper_0056dbf0(Memory& m, msg::LocPorts& p, Ports&, Word self, Word player, Word speaker, Word text, Word s) { return simple(m, p, self, player, speaker, text, 3, s); }
Word tell_0056d8c0(Memory& m, msg::LocPorts& p, Ports& x, Word self, Word player, Word speaker, Word text, Word s) { return named(m, p, x, self, player, speaker, text, 0x14, 4, s); }
Word party_0056d6e0(Memory& m, msg::LocPorts& p, Ports& x, Word self, Word player, Word speaker, Word text, Word s) { return named(m, p, x, self, player, speaker, text, 8, 6, s); }

Word server_tell_0056d9c0(Memory& m, msg::LocPorts& p, Ports&, Word self, Word player, Word text, Word s) {
    begin(m, p, self, text, 4);
    msg::write_string_004d6960(m, p, self, text, kBits);
    return finish_and_send(m, p, self, player, 5, s);
}

// Shout: CreateWriteMessage before the speaker check; a missing speaker writes OBJECT_INVALID, zero position and two
// fresh CExoLocStrings, the first with StrRef 10782h (both destroyed afterwards, second first).
Word shout_0056da30(Memory& m, msg::LocPorts& p, Ports& x, Word self, Word player, Word speaker, Word text, Word s) {
    const Word c = x.creature_by_id_004ae770(server_of(m, x), speaker);
    begin(m, p, self, text, 0x14);
    if (c != 0) {
        write_speaker(m, p, x, self, speaker, c, text, s);
    } else {
        const Word a = s + 0x20, b = s + 0x28;
        x.loc_construct_005e9e10(a);
        x.loc_construct_005e9e10(b);
        m.write(a + 4, kShoutNoSpeakerStrRef);
        msg::write_object_id_00578570(m, p, self, kInvalid);
        msg::write_string_004d6960(m, p, self, text, kBits);
        for (int i = 0; i < 3; ++i) msg::write_float_bits_004d6920(m, p, self, 0, kOne, kBits);
        msg::write_locstring_005785c0(m, p, self, a, 0, s + 8);
        msg::write_locstring_005785c0(m, p, self, b, 0, s + 8);
        x.loc_destroy_005ea000(b);
        x.loc_destroy_005ea000(a);
    }
    return finish_and_send(m, p, self, player, 2, s);
}

// 005BE900: Party chat to each faction member that has a client object; EAX = the member count re-read last.
Word faction_send_chat_005be900(Memory& m, msg::LocPorts& p, Ports& x, Word f, Word speaker, Word text, Word s) {
    const Word self = x.sws_message_004aedb0(server_of(m, x));
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(f + 4)); ++i) {
        const Word client = x.client_object_by_id_004ae910(server_of(m, x), m.read(m.read(f) + 4 * i));
        if (client != 0) (void)party_0056d6e0(m, p, x, self, m.read(client + 4), speaker, text, s);
    }
    return m.read(f + 4);
}
}
