#include "386aac6f5b-w2_chat.hpp"
#include <cstdint>
#include <cstring>
namespace w2::chat {
namespace {
using I = std::int32_t;
constexpr Word kRange1000 = 0x447a0000u;   // [00747444] / [00747458] 1000.0f
constexpr Word kRange250 = 0x437a0000u;    // [00747440] 250.0f
constexpr Word kRange3 = 0x40400000u;      // [00747448] 3.0f
constexpr Word kRange35 = 0x420c0000u;     // [00747454] 35.0f
constexpr Word kShoutTarget = 0xfffffff7u; // [0074C52C]
float f32(Word b) { float v; std::memcpy(&v, &b, 4); return v; }
Word server(Memory& m, Ports& p) { return m.read(p.app_007a39fc() + 8); }
// GetObjectArray()->GetGameObject(id, &out) == [0074666C] (0): returns the object or 0
Word lookup(Memory& m, Ports& p, Word id, Word out) {
    if ((p.object_lookup_004d8230(p.object_array_004aed70(server(m, p)), id, out) & 0xffu) != 0) return 0;
    return m.read(out);
}
enum class Send { Talk, Whisper };
// 00571B26 / 00571CA2: per-player loop. Squared distance in x87 double from float-stored deltas; sends when
// range^2 >= d^2 (FCOMPP C0 clear; NaN skips). Stops as soon as a send fails (esi != 1).
Word players_in_range(Memory& m, Ports& p, Word msg, Word players, Word speaker, Word text, Word area, const float (&pos)[3],
                      Word range_bits, Send kind, Word esi, Word slot) {
    const Word list = m.read(players);
    m.write(slot, m.read(list));
    while (m.read(slot) != 0 && esi == 1) {
        const Word pl = p.list_at_pos_005e9b60(list, m.read(slot));
        if (p.player_game_object_00561080(pl) != 0) {
            const Word go = p.player_game_object_00561080(pl);
            const float dz = f32(m.read(go + 0x98)) - pos[2];
            const float dy = f32(m.read(go + 0x94)) - pos[1];
            const float dx = f32(m.read(go + 0x90)) - pos[0];
            if (p.object_area_004cb120(p.player_game_object_00561080(pl)) == area) {
                const double d2 = (static_cast<double>(dx) * dx + static_cast<double>(dy) * dy) + static_cast<double>(dz) * dz;
                const double r = f32(range_bits), r2 = r * r;
                if (!(r2 < d2) && r2 == r2 && d2 == d2) {
                    const Word r_ = kind == Send::Talk ? p.chat_talk_0056d7e0(msg, m.read(pl + 4), speaker, text)
                                                       : p.chat_whisper_0056dbf0(msg, m.read(pl + 4), speaker, text);
                    esi = r_ & 1u;
                }
            }
        }
        p.list_next_005e9b90(list, slot);
    }
    return esi;
}
}

Word send_chat_00571960(Memory& m, Ports& p, Word msg, Word mode, Word speaker, Word text, Word target, Word s) {
    const Word players = p.player_list_004aed90(server(m, p));
    if (players == 0) { p.str_destroy_005e5c20(text); return 0; }         // 0057199B
    Word esi = 1;
    const Word obj_slot = s, pos_slot = s + 4, out = s + 8;
    switch (static_cast<std::uint8_t>(mode)) {                            // 005719C4 movzx; dec; cmp 0Dh; ja
    case 1: case 3: {
        const bool whisper = static_cast<std::uint8_t>(mode) == 3;
        float pos[3] = {0.0f, 0.0f, 0.0f}; Word area = 0;
        const Word obj = lookup(m, p, speaker, obj_slot);
        Word src = 0;
        if (obj != 0) src = whisper ? p.vcall0(obj, 0x30) : obj;          // whisper: AsCreature must be non-null
        if (obj == 0 || src == 0) { esi = 0; }
        else {
            if (whisper) src = p.vcall0(obj, 0x30);                       // 00571C57 second AsCreature
            area = p.object_area_004cb120(src);
            if (whisper) src = p.vcall0(obj, 0x30);                       // 00571C6B third AsCreature
            for (int k = 0; k < 3; ++k) pos[k] = f32(m.read(src + 0x90 + 4 * static_cast<Word>(k)));
            (void)p.broadcast_dialog_004cd760(obj, text, whisper ? kRange3 : kRange1000, 0);
        }
        esi = players_in_range(m, p, msg, players, speaker, text, area, pos, whisper ? kRange3 : kRange1000,
                               whisper ? Send::Whisper : Send::Talk, esi, pos_slot);
        break;
    }
    case 2: {                                                             // 00571A3A shout
        esi = p.chat_shout_0056da30(msg, kShoutTarget, speaker, text) & 1u;
        const Word obj = lookup(m, p, speaker, out);
        if (obj != 0) (void)p.broadcast_dialog_004cd760(obj, text, kRange250, 0);
        break;
    }
    case 4: {                                                             // 005719F1 tell: speaker's own player too
        const Word c = p.client_object_by_id_004ae910(server(m, p), speaker);
        if (c != 0 && target != m.read(c + 4)) esi = p.chat_tell_0056d8c0(msg, m.read(c + 4), speaker, text) & 1u;
        esi &= p.chat_tell_0056d8c0(msg, target, speaker, text);
        break;
    }
    case 5:                                                               // 005719DB server tell (result ignored)
        (void)p.chat_server_tell_0056d9c0(msg, target, text);
        break;
    case 6: {                                                             // 00571E14 party
        const Word c = p.client_object_by_id_004ae910(server(m, p), speaker);
        if (c == 0) p.trap(0x00571e29, "W2: party chat from an object without a client player (original dereferences null)");
        const Word go = p.player_game_object_00561080(c);
        if (go == 0) p.trap(0x00571e32, "W2: party chat player without a game object (original dereferences null)");
        const Word f = p.creature_faction_00513fc0(p.vcall0(go, 0x30));
        if (f != 0) (void)p.faction_send_chat_005be900(f, speaker, text);
        break;
    }
    case 13: case 14: {                                                   // 00571D81 / 00571DD0
        const Word obj = lookup(m, p, speaker, obj_slot);
        if (obj != 0) (void)p.broadcast_dialog_004cd760(obj, text, static_cast<std::uint8_t>(mode) == 13 ? kRange35 : kRange1000,
                                                       static_cast<std::uint8_t>(mode) == 13 ? 1u : 0u);
        break;
    }
    default:                                                              // 0, 7..12, > 14: 00571E4F
        esi = 0;
        break;
    }
    p.str_destroy_005e5c20(text);
    return esi;
}
}
