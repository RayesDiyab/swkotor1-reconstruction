// Lane worker "p2s-b" (journal fork): 005278D0 CSWSMessage::HandlePlayerToServerJournalMessage and its missing closure
// (world-journal module list, journal send messages, CWorldJournalEntry array helpers).
// Transcribed from the original swkotor.exe (read-only); statement comments cite original instruction addresses.
// Conventions: claude_dialogue_common.hpp. thiscall -> extern "C" __fastcall(self, edx, stack args...).
// Game heap only: operator new 006FA7E6 -> live_operator_new, _free 006FA390 -> live_free.
//
// Layouts used (from the instructions below):
//   CWorldJournalEntry (0x24): +0 CExoString, +8 CExoString, +10h/+14h dwords (world time pair), +18h dword,
//     +1Ch dword (index or strref), +20h bitfield (bit0: entry is strref-based).
//   CSWSModule +1CCh CExoArrayList<CWorldJournalEntry> {data, count(+1D0h), capacity(+1D4h)}.
//   CSWSPlayer +4Ch -> journal update block {+0 dword, +4 data (0xC-byte elements, dtor 0059EA20), +8 count,
//     +0Ch capacity, +10h full-update-pending}.
//   Creature journal (CSWSCreature::GetJournal): {+0 data (0x3C-byte entries), +4 count}.
#include "6bd5403233-claude_dialogue_common.hpp"

using namespace dlg;

extern "C" {
// w2_message_native.obj (real bodies; none in the c16 trap census)
U __fastcall kotor_fastcall_004d6230(void* msg, void*, U bits);                        // MessageReadOverflow RET 4
U __fastcall kotor_fastcall_004d6240(void* msg, void*, U bits);                        // MessageReadUnderflow RET 4
U __fastcall kotor_fastcall_004d6780(void* msg, void*, U bits);                        // ReadDWORD RET 4
U __fastcall kotor_fastcall_004d66f0(void* msg, void*);                                // ReadBOOL RET 0
void* __fastcall kotor_fastcall_004d67e0(void* msg, void*, void* result, U bits);      // ReadCExoString RET 8
void __fastcall kotor_fastcall_004d68a0(void* msg, void*, U size, U a, U b);           // CreateWriteMessage
void __fastcall kotor_fastcall_004d6340(void* msg, void*, U value, U bits);            // WriteDWORD
void __fastcall kotor_fastcall_004d6300(void* msg, void*, U value, U bits);            // WriteWORD
void __fastcall kotor_fastcall_004d68e0(void* msg, void*, U value);                    // WriteBOOL
void __fastcall kotor_fastcall_004d6960(void* msg, void*, const void* s, U bits);      // WriteCExoString
void __fastcall kotor_fastcall_005785c0(void* msg, void*, void* loc, U gender);        // WriteCExoLocStringServer
U __fastcall kotor_fastcall_004d6440(void* msg, void*, void** data, U* size);          // GetWriteMessage
U __fastcall kotor_fastcall_00567e60(void* msg, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage
// other real providers (c16 map)
void* __fastcall server_get_module_004ae6b0(void* server, void*);                      // CServerExoApp::GetModule
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                         // CServerExoApp::GetWorldTimer
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, U* a, U* b);           // CWorldTimer::GetWorldTime
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);           // CServerExoApp::GetSWSMessage
void* __fastcall recovered_server_player_list_004aed90(void* server, void*);           // CServerExoApp::GetPlayerList
void* __fastcall kotor_fastcall_00561080(void* player, void*);                         // CSWSPlayer::GetGameObject
void* __fastcall kotor_fastcall_004edb00(void* creature, void*);                       // CSWSCreature::GetJournal (dialogue_server)
int __fastcall async_string_equal_005e5200(const void* s, void*, const void* other);   // CExoString::operator==
void __fastcall world_journal_entry_destroy_004c4030(void* entry, void*);              // ~CWorldJournalEntry
}

namespace {
struct WorldJournalEntryArg { U w[9]; };   // CWorldJournalEntry passed by value (0x24 bytes; callee destroys it)
static_assert(sizeof(WorldJournalEntryArg) == 0x24, "CWorldJournalEntry by value");
constexpr U kEntry = 0x24;
inline U player_id(void* player) { return rd(player, 4); }
// CExoString temporary written by ReadCExoString (constructed by the callee), destroyed after the assignment.
struct ReadTemp { U a, b; };
}

// Exports of this file
extern "C" {
U __fastcall recovered_player_to_server_journal_005278d0(void* message, void*, void* player, U minor);
U __fastcall kotor_fastcall_00577b60(void* msg, void*, void* player);
U __fastcall kotor_fastcall_0056a450(void* msg, void*, void* player, U flag);
U __fastcall kotor_fastcall_0056a1a0(void* msg, void*, void* player, U index, void* s1, void* s2, U t1, U t2);
U __fastcall kotor_fastcall_0056a300(void* msg, void*, void* player, U index);
U __fastcall kotor_fastcall_0056a370(void* msg, void*, void* player, U strref);
I __fastcall kotor_fastcall_004c61d0(void* module, void*);
void __fastcall kotor_fastcall_004c7ed0(void* module, void*, U index, void* s1, void* s2, U t1, U t2);
void __fastcall kotor_fastcall_004c8170(void* module, void*, U index);
void __fastcall kotor_fastcall_004c8230(void* module, void*, U strref);
void* __fastcall kotor_fastcall_004c3fe0(void* entry, void*);
void* __fastcall kotor_fastcall_004c4080(void* entry, void*, const void* src);
void __fastcall kotor_fastcall_004c65e0(void* list, void*, WorldJournalEntryArg entry);
void __fastcall kotor_fastcall_004c5000(void* list, void*, U capacity);
void __fastcall kotor_fastcall_004c66b0(void* list, void*, I index);
}

// ---------------------------------------------------------------------------------------------------------------
// 005278D0 CSWSMessage::HandlePlayerToServerJournalMessage(CSWSPlayer* player, BYTE minor) RET 8
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall recovered_player_to_server_journal_005278d0(void* message, void*, void* player, U minor) {
    ExoString text_a;                                                                   // 005278F3 [esp+20h]
    ExoString text_b;                                                                   // 005278FE [esp+18h]
    void* module = server_get_module_004ae6b0(server(), nullptr);                       // 0052790B..0052791D edi
    switch (static_cast<U8>(minor)) {                                                   // 0052791F..0052792B table 00527B0C
    case 0x0a:                                                                          // 00527932 full update request
        wr(rp(player, 0x4c), 0, 1);                                                     // 00527932..00527938
        (void)kotor_fastcall_00577b60(message, nullptr, player);                        // 0052793E..00527941
        break;
    case 0x0b: {                                                                        // 0052797E clear the update list
        wr(rp(player, 0x4c), 0, 0);                                                     // 0052797E..00527984
        void* block = rp(player, 0x4c);                                                 // 00527986 esi
        const U data = rd(block, 4);                                                    // 00527989
        if (data != 0) {                                                                // 0052798C..0052798E
            c2da_destroy_array_006fa852(ptr(data), 0xc, rdi(ptr(data - 4), 0),
                                        reinterpret_cast<void*>(&kotor_fastcall_0059ea20)); // 00527990..0052799F
            live_free(ptr(data - 4));                                                   // 005279A4..005279AA
            wr(block, 4, 0);                                                            // 005279AD
            wr(block, 0xc, 0);                                                          // 005279B0
        }
        wr(block, 8, 0);                                                                // 005279B3
        break;
    }
    case 0x0d: {                                                                        // 005279B8 add world entry
        ReadTemp t1;
        str_assign(&text_a, kotor_fastcall_004d67e0(message, nullptr, &t1, 0x20));      // 005279B8..005279D0
        frame_LogStringDtor_005e5c20(&t1, nullptr);                                     // 005279D5..005279DE
        ReadTemp t2;
        str_assign(&text_b, kotor_fastcall_004d67e0(message, nullptr, &t2, 0x20));      // 005279E3..005279FB
        frame_LogStringDtor_005e5c20(&t2, nullptr);                                     // 00527A00..00527A09
        if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;               // 00527A0E..00527A19
        if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;               // 00527A1F..00527A2A
        if (module == nullptr) break;                                                   // 00527A30..00527A32
        U time_a, time_b;                                                               // [esp+10h] / [esp+0Ch]
        frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server(), nullptr), nullptr, &time_a, &time_b); // 00527A38..00527A52
        const I index = kotor_fastcall_004c61d0(module, nullptr);                       // 00527A6B..00527A6D
        kotor_fastcall_004c7ed0(module, nullptr, static_cast<U>(index), &text_b, &text_a, time_a, time_b); // 00527A57..00527A75
        break;
    }
    case 0x0e: {                                                                        // 00527A7F delete world entry
        const U id = kotor_fastcall_004d6780(message, nullptr, 0x20);                   // 00527A7F..00527A8A ebx
        const U by_strref = kotor_fastcall_004d66f0(message, nullptr);                  // 00527A88..00527A95 [esp+14h]
        if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;               // 00527A91..00527AA0
        if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;               // 00527AA2..00527AAD
        if (module == nullptr) break;                                                   // 00527AAF..00527AB1
        if (by_strref != 0) kotor_fastcall_004c8230(module, nullptr, id);               // 00527AB7..00527AC2
        else kotor_fastcall_004c8170(module, nullptr, id);                              // 00527ACC
        break;
    }
    default:                                                                            // 0x0c and outside 0Ah..0Eh
        break;
    }
    return 1;                                                                           // 00527946..0052797B (strings destroyed b, a)
}

// ---------------------------------------------------------------------------------------------------------------
// 00577B60 CSWSMessage::SendServerToPlayerJournalFullUpdate(CSWSPlayer* player) RET 4
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00577b60(void* msg, void*, void* player) {
    void* updates = nullptr;                                                            // 00577B70 [esp+0Ch] (E-1Ch)
    void* creature = nullptr;
    if (kotor_fastcall_00561080(player, nullptr) != nullptr)                            // 00577B6E..00577B7B
        creature = vcall<void*>(kotor_fastcall_00561080(player, nullptr), 0x30);        // 00577B7D..00577B88 AsSWSCreature
    if (creature == nullptr) return kotor_fastcall_0056a450(msg, nullptr, player, 1);   // 00577B8B..00577B9F
    void* journal = kotor_fastcall_004edb00(creature, nullptr);                         // 00577BA3..00577BAA edi
    if (journal == nullptr) return kotor_fastcall_0056a450(msg, nullptr, player, 1);    // 00577BAC..00577BC1
    void* block = rp(player, 0x4c);                                                     // 00577BC4
    U count;                                                                            // ebx
    U full;                                                                             // ebp
    U journal_count = 0;                                                                // [esp+1Ch] (E-10h)
    if (rd(block, 0x10) != 0) {                                                         // 00577BC7..00577BCA
        count = rd(journal, 4);                                                         // 00577BCC
        full = 1;                                                                       // 00577BCF
    } else {
        if (rdi(block, 8) <= 0) return kotor_fastcall_0056a450(msg, nullptr, player, 0); // 00577BD6..00577BEB
        updates = fa(block, 4);                                                         // 00577BEE..00577BF3
        if (updates == nullptr) return kotor_fastcall_0056a450(msg, nullptr, player, 0); // 00577BF1..00577BF7
        count = rd(updates, 4);                                                         // 00577BF9
        journal_count = rd(journal, 4);                                                 // 00577BFC..00577BFF
        full = 0;
    }
    U size = count * 0x1e + 0xc;                                                        // 00577C03..00577C12 [esp+30h] (E-8)
    kotor_fastcall_004d68a0(msg, nullptr, size, 0xffffffffu, 1);                        // 00577C08..00577C16
    kotor_fastcall_004d68e0(msg, nullptr, 1);                                           // 00577C1B..00577C1F
    kotor_fastcall_004d68e0(msg, nullptr, full);                                        // 00577C24..00577C27
    kotor_fastcall_004d6340(msg, nullptr, count, 0x20);                                 // 00577C2C..00577C31
    if (full != 0) {
        if (static_cast<I>(count) > 0) {                                                // 00577C3E..00577C40
            U off = 0;                                                                  // ebp
            U left = count;
            do {
                const U mask = (rd(rp(journal, 0), off + 0x2c) & 4) != 0 ? 0x8000u : 0x8100u; // 00577C50..00577C5E
                kotor_fastcall_004d6300(msg, nullptr, mask, 0x10);                      // 00577C63..00577C68
                kotor_fastcall_004d6960(msg, nullptr, fa(rp(journal, 0), off + 0x18), 0x20); // 00577C6D..00577C78
                kotor_fastcall_005785c0(msg, nullptr, fa(rp(journal, 0), off), 0);      // 00577C7D..00577C86
                kotor_fastcall_005785c0(msg, nullptr, fa(rp(journal, 0), off + 8), 0);  // 00577C8B..00577C96
                kotor_fastcall_004d6340(msg, nullptr, rd(rp(journal, 0), off + 0x10), 0x20); // 00577C9B..00577CA6
                kotor_fastcall_004d6340(msg, nullptr, rd(rp(journal, 0), off + 0x14), 0x20); // 00577CAB..00577CB6
                kotor_fastcall_004d6340(msg, nullptr, rd(rp(journal, 0), off + 0x20), 0x20); // 00577CBB..00577CC6
                kotor_fastcall_004d6340(msg, nullptr, rd(rp(journal, 0), off + 0x24), 0x20); // 00577CCB..00577CD6
                kotor_fastcall_004d6300(msg, nullptr, rd16(rp(journal, 0), off + 0x28), 0x10); // 00577CDB..00577CE9
                kotor_fastcall_004d68e0(msg, nullptr, rd(rp(journal, 0), off + 0x2c) & 1); // 00577CEE..00577CFA
                kotor_fastcall_004d6340(msg, nullptr, rd(rp(journal, 0), off + 0x30), 0x20); // 00577CFF..00577D0A
                kotor_fastcall_004d6340(msg, nullptr, rd(rp(journal, 0), off + 0x34), 0x20); // 00577D0F..00577D1A
                kotor_fastcall_004d6340(msg, nullptr, rd(rp(journal, 0), off + 0x38), 0x20); // 00577D1F..00577D2A
                void* e = rp(journal, 0);                                               // 00577D2F
                wr(e, off + 0x2c, rd(e, off + 0x2c) & 0xfffffffbu);                     // 00577D31..00577D40
                off += 0x3c;                                                            // 00577D3C
            } while (--left != 0);                                                      // 00577D3F..00577D42
        }
    } else if (static_cast<I>(count) > 0) {                                             // 00577D4D..00577D4F
        U uoff = 0;                                                                     // [esp+18h] (E-14h)
        U left = count;                                                                 // [esp+20h] (E-0Ch)
        do {
            const U flags = rd16(rp(updates, 0), uoff);                                 // 00577D60..00577D68 ebx
            if ((flags & 0x4000) != 0) {                                                // 00577D6C..00577D6F
                kotor_fastcall_004d6300(msg, nullptr, flags, 0x10);                     // 00577D71..00577D76
                kotor_fastcall_004d6960(msg, nullptr, fa(rp(updates, 0), uoff + 4), 0x20); // 00577D7B..00577D8A
            } else if (static_cast<I>(journal_count) > 0) {                             // 00577D94..00577D9C
                I j = 0;                                                                // ebp
                U joff = 0;                                                             // [esp+14h] (E-18h)
                bool found = false;
                for (;;) {
                    const void* tag = fa(rp(updates, 0), uoff + 4);                     // 00577DA6..00577DB0
                    if (async_string_equal_005e5200(fa(rp(journal, 0), joff + 0x18), nullptr, tag) != 0) { // 00577DB4..00577DC6
                        found = true;
                        break;
                    }
                    ++j;                                                                // 00577DD0
                    joff += 0x3c;                                                       // 00577DD1
                    if (j >= static_cast<I>(journal_count)) break;                      // 00577DD4..00577DDA
                }
                if (found) {
                    kotor_fastcall_004d6300(msg, nullptr, flags, 0x10);                 // 00577DE1..00577DE6
                    const U o = static_cast<U>(j) * 0x3c;                               // 00577DEB
                    kotor_fastcall_004d6960(msg, nullptr, fa(rp(journal, 0), o + 0x18), 0x20); // 00577DEE..00577DF9
                    if ((flags & 0x8001) != 0) kotor_fastcall_005785c0(msg, nullptr, fa(rp(journal, 0), o), 0);       // 00577DFE..00577E0F
                    if ((flags & 0x8002) != 0) kotor_fastcall_005785c0(msg, nullptr, fa(rp(journal, 0), o + 8), 0);   // 00577E14..00577E27
                    if ((flags & 0x8004) != 0) kotor_fastcall_004d6340(msg, nullptr, rd(rp(journal, 0), o + 0x10), 0x20); // 00577E2C..00577E3F
                    if ((flags & 0x8008) != 0) kotor_fastcall_004d6340(msg, nullptr, rd(rp(journal, 0), o + 0x14), 0x20); // 00577E44..00577E57
                    if ((flags & 0x8010) != 0) kotor_fastcall_004d6340(msg, nullptr, rd(rp(journal, 0), o + 0x20), 0x20); // 00577E5C..00577E6F
                    if ((flags & 0x8020) != 0) kotor_fastcall_004d6340(msg, nullptr, rd(rp(journal, 0), o + 0x24), 0x20); // 00577E74..00577E87
                    if ((flags & 0x8040) != 0) kotor_fastcall_004d6300(msg, nullptr, rd16(rp(journal, 0), o + 0x28), 0x10); // 00577E8C..00577EA2
                    if ((flags & 0x8080) != 0) kotor_fastcall_004d68e0(msg, nullptr, rd(rp(journal, 0), o + 0x2c) & 1); // 00577EA7..00577EBB
                    if ((flags & 0x8200) != 0) kotor_fastcall_004d6340(msg, nullptr, rd(rp(journal, 0), o + 0x30), 0x20); // 00577EC0..00577ED0 (test bh,82h)
                    if ((flags & 0x8400) != 0) kotor_fastcall_004d6340(msg, nullptr, rd(rp(journal, 0), o + 0x34), 0x20); // 00577ED5..00577EE5 (test bh,84h)
                    if ((flags & 0x8800) != 0) kotor_fastcall_004d6340(msg, nullptr, rd(rp(journal, 0), o + 0x38), 0x20); // 00577EEA..00577EFA (test bh,88h)
                }
            }
            uoff += 0xc;                                                                // 00577EFF..00577F0B
        } while (--left != 0);                                                          // 00577F0A..00577F13
    }
    void* data;                                                                         // [esp+28h] (E-4)
    if (kotor_fastcall_004d6440(msg, nullptr, &data, &size) == 0) return 0;             // 00577F19..00577F35
    if (updates != nullptr) {                                                           // 00577F38..00577F40
        const U d = rd(updates, 0);                                                     // 00577F42
        if (d != 0) {                                                                   // 00577F44..00577F46
            c2da_destroy_array_006fa852(ptr(d), 0xc, rdi(ptr(d - 4), 0),
                                        reinterpret_cast<void*>(&kotor_fastcall_0059ea20)); // 00577F48..00577F57
            live_free(ptr(d - 4));                                                      // 00577F5C..00577F62
            wr(updates, 0, 0);                                                          // 00577F65
            wr(updates, 8, 0);                                                          // 00577F67
        }
        wr(updates, 4, 0);                                                              // 00577F6A
    }
    wr(rp(player, 0x4c), 0x10, 0);                                                      // 00577F6D..00577F74
    return kotor_fastcall_00567e60(msg, nullptr, player_id(player), 0x1c, 9, data, size); // 00577F77..00577F97
}

// 0056A450 CSWSMessage::SendServerToPlayerJournalFullUpdateNotNeeded(CSWSPlayer* player, int flag) RET 8
extern "C" U __fastcall kotor_fastcall_0056a450(void* msg, void*, void* player, U flag) {
    if (flag != 0) {                                                                    // 0056A451..0056A45E
        kotor_fastcall_004d68a0(msg, nullptr, 0xc, 0xffffffffu, 1);                     // 0056A460..0056A462
        kotor_fastcall_004d68e0(msg, nullptr, 1);                                       // 0056A467..0056A46B
        kotor_fastcall_004d68e0(msg, nullptr, 1);                                       // 0056A470..0056A474
        kotor_fastcall_004d6340(msg, nullptr, 0, 0x20);                                 // 0056A479..0056A47F
    } else {
        kotor_fastcall_004d68a0(msg, nullptr, 4, 0xffffffffu, 1);                       // 0056A486..0056A488
        kotor_fastcall_004d68e0(msg, nullptr, 0);                                       // 0056A48D..0056A491
    }
    void* data;                                                                         // [esp+4]
    U size;                                                                             // flag argument slot
    if (kotor_fastcall_004d6440(msg, nullptr, &data, &size) == 0) return 0;             // 0056A496..0056A4AD
    return kotor_fastcall_00567e60(msg, nullptr, player_id(player), 0x1c, 9, data, size); // 0056A4B0..0056A4CF
}

// 0056A1A0 CSWSMessage::SendServerToPlayerJournalAddWorld (Ghidra: ExecuteCommandAddJournalWorldEntry)
//   (player, index, CExoString& s1, CExoString& s2, t1, t2) RET 18h
extern "C" U __fastcall kotor_fastcall_0056a1a0(void* msg, void*, void* player, U index, void* s1, void* s2, U t1, U t2) {
    const U len1 = async_string_length_005e5790(s1, nullptr);                           // 0056A1A5..0056A1B8 ebp
    const U len2 = async_string_length_005e5790(s2, nullptr);                           // 0056A1B2..0056A1BA
    U size = len2 + len1 + 0xc;                                                         // 0056A1C1 (stored in the s1 slot)
    kotor_fastcall_004d68a0(msg, nullptr, size, 0xffffffffu, 1);                        // 0056A1BF..0056A1CE
    kotor_fastcall_004d6340(msg, nullptr, index, 0x20);                                 // 0056A1D3..0056A1DC
    kotor_fastcall_004d6960(msg, nullptr, s1, 0x20);                                    // 0056A1E1..0056A1E6
    kotor_fastcall_004d6960(msg, nullptr, s2, 0x20);                                    // 0056A1EB..0056A1F0
    kotor_fastcall_004d6340(msg, nullptr, t1, 0x20);                                    // 0056A1F5..0056A1FE
    kotor_fastcall_004d6340(msg, nullptr, t2, 0x20);                                    // 0056A203..0056A20C
    void* data;                                                                         // [esp+10h]
    if (kotor_fastcall_004d6440(msg, nullptr, &data, &size) == 0) return 0;             // 0056A211..0056A22B
    return kotor_fastcall_00567e60(msg, nullptr, player_id(player), 0x1c, 1, data, size); // 0056A22E..0056A250
}

// 0056A300 CSWSMessage::SendServerToPlayerJournalDeleteWorld(CSWSPlayer* player, DWORD index) RET 8
extern "C" U __fastcall kotor_fastcall_0056a300(void* msg, void*, void* player, U index) {
    U size = 4;                                                                         // 0056A30C [esp+4]
    kotor_fastcall_004d68a0(msg, nullptr, 4, 0xffffffffu, 1);                           // 0056A304..0056A314
    kotor_fastcall_004d6340(msg, nullptr, index, 0x20);                                 // 0056A319..0056A322
    void* data;                                                                         // [esp+8]
    if (kotor_fastcall_004d6440(msg, nullptr, &data, &size) == 0) return 0;             // 0056A327..0056A340
    return kotor_fastcall_00567e60(msg, nullptr, player_id(player), 0x1c, 3, data, size); // 0056A343..0056A364
}

// 0056A370 CSWSMessage::SendServerToPlayerJournalDeleteWorldStrref(CSWSPlayer* player, DWORD strref) RET 8
extern "C" U __fastcall kotor_fastcall_0056a370(void* msg, void*, void* player, U strref) {
    U size = 4;                                                                         // 0056A37C [esp+4]
    kotor_fastcall_004d68a0(msg, nullptr, 4, 0xffffffffu, 1);                           // 0056A374..0056A384
    kotor_fastcall_004d6340(msg, nullptr, strref, 0x20);                                // 0056A389..0056A392
    void* data;                                                                         // [esp+8]
    if (kotor_fastcall_004d6440(msg, nullptr, &data, &size) == 0) return 0;             // 0056A397..0056A3B0
    return kotor_fastcall_00567e60(msg, nullptr, player_id(player), 0x1c, 4, data, size); // 0056A3B3..0056A3D4
}

// ---------------------------------------------------------------------------------------------------------------
// CSWSModule world journal
// ---------------------------------------------------------------------------------------------------------------
// 004C61D0 CSWSModule::GetWorldJournalIndexUnique() RET 0: smallest index >= 1 not used by a non-strref entry
extern "C" I __fastcall kotor_fastcall_004c61d0(void* module, void*) {
    const I count = rdi(module, 0x1d0);                                                 // 004C61D1 esi
    I candidate = 1;                                                                    // 004C61DB eax
    if (count <= 0) return candidate;                                                   // 004C61D9..004C61E0 (jle)
    void* data = rp(module, 0x1cc);                                                     // 004C61E3 edi
    I i = 0;                                                                            // 004C61D7 edx
    do {
        const U e = static_cast<U>(i) * kEntry;                                         // 004C61F0..004C61F8
        if ((rd8(data, e + 0x20) & 1) == 0 && rdi(data, e + 0x1c) == candidate) {     // 004C61F3..004C6200
            ++candidate;                                                                // 004C6202
            i = -1;                                                                     // 004C6203 restart
        }
        ++i;                                                                            // 004C6206
    } while (i < count);                                                                // 004C6207..004C6209
    return candidate;
}

// 004C7ED0 CSWSModule::AddWorldJournalEntry(index, CExoString& s1, CExoString& s2, t1, t2) RET 14h
extern "C" void __fastcall kotor_fastcall_004c7ed0(void* module, void*, U index, void* s1, void* s2, U t1, U t2) {
    void* msg = recovered_server_sws_message_004aedb0(server(), nullptr);               // 004C7EE8..004C7EFF edi
    // Local CWorldJournalEntry at [esp+14h]: the two CExoStrings are constructed, the dword at +18h and the upper
    // bits of +20h are never written (indeterminate stack contents in the original, as here).
    alignas(4) unsigned char entry[kEntry];
    (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(entry), nullptr);     // 004C7EFB..004C7F01
    (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(entry + 8), nullptr); // 004C7F06..004C7F12
    struct Strings {                                                                    // EH: ~CExoString(+8), ~CExoString(+0)
        unsigned char* e;
        ~Strings() { frame_LogStringDtor_005e5c20(e + 8, nullptr); frame_LogStringDtor_005e5c20(e, nullptr); }
    } strings{entry};
    wr(entry, 0x1c, index);                                                             // 004C7F2C
    str_assign(entry, s1);                                                              // 004C7F1F..004C7F30
    str_assign(entry + 8, s2);                                                          // 004C7F35..004C7F3E
    wr(entry, 0x14, t2);                                                                // 004C7F43..004C7F4B
    U bits;
    std::memcpy(&bits, entry + 0x20, 4);                                                // 004C7F4F (uninitialised local)
    wr(entry, 0x20, bits & 0xfffffffeu);                                                // 004C7F53..004C7F59
    wr(entry, 0x10, t1);                                                                // 004C7F47 / 004C7F68
    WorldJournalEntryArg copy;
    (void)kotor_fastcall_004c4080(&copy, nullptr, entry);                               // 004C7F56..004C7F6C copy into the arg area
    kotor_fastcall_004c65e0(fa(module, 0x1cc), nullptr, copy);                          // 004C7F71..004C7F77 (callee destroys it)
    if (msg == nullptr) return;                                                         // 004C7F7C..004C7F7E
    void* players = recovered_server_player_list_004aed90(server(), nullptr);           // 004C7F80..004C7F8D ebx
    if (players == nullptr) return;                                                     // 004C7F8F..004C7F91
    void* pos = rp(rp(players, 0), 0);                                                  // 004C7F93..004C7F99
    while (pos != nullptr) {                                                            // 004C7F9D / 004C7FE6
        void* player = unresolved_list_at_pos_005e9b60(rp(players, 0), nullptr, pos);   // 004C7FA0..004C7FA8
        if (player != nullptr && kotor_fastcall_00561080(player, nullptr) != nullptr)   // 004C7FAA..004C7FB7
            (void)kotor_fastcall_0056a1a0(msg, nullptr, player, index, s1, s2, t1, t2); // 004C7FB9..004C7FD1
        (void)unresolved_list_next_005e9b90(rp(players, 0), nullptr, &pos);            // 004C7FD6..004C7FDD
    }
}                                                                                       // 004C7FEA..004C801E

namespace {
// Shared body of 004C8170 / 004C8230: search from index == count (one past the last element: original quirk,
// it reads the slot after the end first) down to 0 for the first entry whose bit0 equals `strref_bit` and whose
// +1Ch equals `id`, and DelIndex it.
void delete_world_entry(void* module, U id, U strref_bit) {
    I i = rdi(module, 0x1d0);                                                           // 004C8185 / 004C8245
    if (i < 0) return;                                                                  // 004C818B..004C818D (jl)
    void* list = fa(module, 0x1cc);                                                     // 004C818F
    void* data = rp(list, 0);                                                           // 004C8195
    U p = static_cast<U>(i) * kEntry + 0x1c;                                            // 004C8197..004C819A
    do {
        if ((rd8(data, p + 4) & 1) == strref_bit && rd(data, p) == id) {               // 004C81A0..004C81AC
            kotor_fastcall_004c66b0(list, nullptr, i);                                  // 004C81B8..004C81B9
            return;
        }
        --i;                                                                            // 004C81AE
        p -= kEntry;                                                                    // 004C81AF
    } while (i >= 0);                                                                   // 004C81B2..004C81B4
}
}

// 004C8170 CSWSModule::DeleteWorldJournalEntry(DWORD index) RET 4
extern "C" void __fastcall kotor_fastcall_004c8170(void* module, void*, U index) {
    void* msg = recovered_server_sws_message_004aedb0(server(), nullptr);               // 004C8171..004C8183 ebp
    delete_world_entry(module, index, 0);                                               // 004C8185..004C81B9
    if (msg == nullptr) return;                                                         // 004C81BE..004C81C0
    void* players = recovered_server_player_list_004aed90(server(), nullptr);           // 004C81C2..004C81CF edi
    if (players == nullptr) return;                                                     // 004C81D1..004C81D3
    void* pos = rp(rp(players, 0), 0);                                                  // 004C81D5..004C81DB
    while (pos != nullptr) {                                                            // 004C81DF / 004C8217
        void* player = unresolved_list_at_pos_005e9b60(rp(players, 0), nullptr, pos);   // 004C81E1..004C81E9
        if (player != nullptr && kotor_fastcall_00561080(player, nullptr) != nullptr)   // 004C81EB..004C81F8
            (void)kotor_fastcall_0056a300(msg, nullptr, player, index);                 // 004C81FA..004C8202
        (void)unresolved_list_next_005e9b90(rp(players, 0), nullptr, &pos);            // 004C8207..004C820E
    }
}

// 004C8230 CSWSModule::DeleteWorldJournalEntryStrref(DWORD strref) RET 4
// (unlike 004C8170 the player list is fetched even without a message object; the message check is per player)
extern "C" void __fastcall kotor_fastcall_004c8230(void* module, void*, U strref) {
    void* msg = recovered_server_sws_message_004aedb0(server(), nullptr);               // 004C8231..004C8243 ebp
    delete_world_entry(module, strref, 1);                                              // 004C8245..004C8279
    void* players = recovered_server_player_list_004aed90(server(), nullptr);           // 004C827E..004C828B edi
    if (players == nullptr) return;                                                     // 004C828D..004C828F
    void* pos = rp(rp(players, 0), 0);                                                  // 004C8291..004C8297
    while (pos != nullptr) {                                                            // 004C829B / 004C82DA
        void* player = unresolved_list_at_pos_005e9b60(rp(players, 0), nullptr, pos);   // 004C82A0..004C82A8
        if (player != nullptr && kotor_fastcall_00561080(player, nullptr) != nullptr && msg != nullptr) // 004C82AA..004C82BB
            (void)kotor_fastcall_0056a370(msg, nullptr, player, strref);                // 004C82BD..004C82C5
        (void)unresolved_list_next_005e9b90(rp(players, 0), nullptr, &pos);            // 004C82CA..004C82D1
    }
}

// ---------------------------------------------------------------------------------------------------------------
// CWorldJournalEntry / CExoArrayList<CWorldJournalEntry>
// ---------------------------------------------------------------------------------------------------------------
// 004C3FE0 CWorldJournalEntry::CWorldJournalEntry() RET 0
extern "C" void* __fastcall kotor_fastcall_004c3fe0(void* entry, void*) {
    (void)kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(entry), nullptr);           // 004C3FF7..004C3FFD
    (void)kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(fa(entry, 8)), nullptr);    // 004C4002..004C400D
    return entry;                                                                       // 004C4016
}

// 004C4080 CWorldJournalEntry::CWorldJournalEntry(CWorldJournalEntry const&) RET 4
extern "C" void* __fastcall kotor_fastcall_004c4080(void* entry, void*, const void* src) {
    (void)WindowCopy005e5b00(entry, nullptr, src);                                      // 004C4098..004C40A3
    struct Guard {                                                                      // EH state 0: ~CExoString(+0)
        void* e; bool armed = true;
        ~Guard() { if (armed) frame_LogStringDtor_005e5c20(e, nullptr); }
    } guard{entry};
    (void)WindowCopy005e5b00(fa(entry, 8), nullptr, fa(src, 8));                        // 004C40A8..004C40B7
    guard.armed = false;
    wr(entry, 0x10, rd(src, 0x10));                                                     // 004C40BC..004C40BF
    wr(entry, 0x14, rd(src, 0x14));                                                     // 004C40C2..004C40C5
    wr(entry, 0x18, rd(src, 0x18));                                                     // 004C40C8..004C40CB
    wr(entry, 0x1c, rd(src, 0x1c));                                                     // 004C40CE..004C40D1
    const U mine = rd(entry, 0x20);                                                     // 004C40D7
    wr(entry, 0x20, mine ^ ((rd(src, 0x20) ^ mine) & 1));                               // 004C40D4..004C40E7 (bit0 only)
    return entry;                                                                       // 004C40EB
}

// 004C65E0 CExoArrayList<CWorldJournalEntry>::Add(CWorldJournalEntry) RET 24h (argument destroyed by the callee)
extern "C" void __fastcall kotor_fastcall_004c65e0(void* list, void*, WorldJournalEntryArg entry) {
    struct ArgDtor {                                                                    // EH states 0/1: ~(+8), then ~(+0)
        WorldJournalEntryArg* a;
        ~ArgDtor() { frame_LogStringDtor_005e5c20(&a->w[2], nullptr); frame_LogStringDtor_005e5c20(&a->w[0], nullptr); }
    } arg_dtor{&entry};
    const U cap = rd(list, 8);                                                          // 004C65F9
    if (rd(list, 4) == cap)                                                             // 004C65FC..004C6607
        kotor_fastcall_004c5000(list, nullptr, cap != 0 ? cap + cap : 0x10);            // 004C6609..004C6619
    const U n = rd(list, 4);                                                            // 004C661E
    void* e = fa(rp(list, 0), n * kEntry);                                              // 004C6621..004C662A
    wr(list, 4, n + 1);                                                                 // 004C6626..004C6627
    str_assign(e, &entry.w[0]);                                                         // 004C662D..004C6634
    str_assign(fa(e, 8), &entry.w[2]);                                                  // 004C6639..004C6641
    wr(e, 0x10, entry.w[4]);                                                            // 004C6646..004C664A
    wr(e, 0x14, entry.w[5]);                                                            // 004C664D..004C6651
    const U old = rd(e, 0x20);                                                          // 004C6658
    wr(e, 0x18, entry.w[6]);                                                            // 004C6654..004C665B
    wr(e, 0x1c, entry.w[7]);                                                            // 004C665E..004C6665
    wr(e, 0x20, old ^ ((old ^ entry.w[8]) & 1));                                        // 004C6662..004C6671
}                                                                                       // 004C6674..004C6691 arg strings destroyed

// 004C5000 CExoArrayList<CWorldJournalEntry>::Allocate(int capacity) RET 4
extern "C" void __fastcall kotor_fastcall_004c5000(void* list, void*, U capacity) {
    const U old = rd(list, 0);                                                          // 004C5023 edi
    wr(list, 8, capacity);                                                              // 004C5031
    void* block = live_operator_new(capacity * kEntry + 4);                             // 004C501F..004C5039 new[] with count cookie
    void* data = nullptr;                                                               // ebp
    if (block != nullptr) {                                                             // 004C5042..004C5048
        wr(block, 0, capacity);                                                         // 004C505B
        data = fa(block, 4);                                                            // 004C5055
        crt_eh_vector_constructor_006fb55f(data, kEntry, static_cast<I>(capacity),
                                           reinterpret_cast<void*>(&kotor_fastcall_004c3fe0),
                                           reinterpret_cast<void*>(&world_journal_entry_destroy_004c4030)); // 004C504A..004C505D
    }
    wp(list, 0, data);                                                                  // 004C5065
    for (I i = 0; i < rdi(list, 4); ++i) {                                              // 004C5062..004C50DD (count re-read)
        const U so = static_cast<U>(i) * kEntry;
        void* dst = fa(rp(list, 0), so);                                                // 004C5090..004C5094
        const void* src = ptr(old + so);                                                // edi - 10h
        str_assign(dst, src);                                                           // 004C5096..004C509C
        str_assign(fa(dst, 8), fa(src, 8));                                             // 004C50A1..004C50A8
        wr(dst, 0x10, rd(src, 0x10));                                                   // 004C50AD..004C50AF
        wr(dst, 0x14, rd(src, 0x14));                                                   // 004C50B2..004C50B5
        wr(dst, 0x18, rd(src, 0x18));                                                   // 004C50B8..004C50BB
        const U mine = rd(dst, 0x20);                                                   // 004C50C1
        wr(dst, 0x1c, rd(src, 0x1c));                                                   // 004C50BE..004C50C4
        wr(dst, 0x20, mine ^ ((rd(src, 0x20) ^ mine) & 1));                             // 004C50C7..004C50D1
    }
    if (old != 0) {                                                                     // 004C50E3..004C50E5
        c2da_destroy_array_006fa852(ptr(old), kEntry, rdi(ptr(old - 4), 0),
                                    reinterpret_cast<void*>(&world_journal_entry_destroy_004c4030)); // 004C50E7..004C50F6
        live_free(ptr(old - 4));                                                        // 004C50FB..004C5101
    }
}

// 004C66B0 CExoArrayList<CWorldJournalEntry>::DelIndex(int index) RET 4 (Ghidra folds it as CWorldJournalEntry::operator=)
// The vacated last slot is not destroyed (original behaviour).
extern "C" void __fastcall kotor_fastcall_004c66b0(void* list, void*, I index) {
    const I n = rdi(list, 4) - 1;                                                       // 004C66B3..004C66B6
    wr(list, 4, static_cast<U>(n));                                                     // 004C66C0
    for (I i = index; i < rdi(list, 4); ++i) {                                          // 004C66BE..004C66C3 / 004C6721..004C672E
        void* dst = fa(rp(list, 0), static_cast<U>(i) * kEntry);                        // 004C66D8..004C66DA
        const void* src = fa(dst, kEntry);                                              // 004C66DC
        str_assign(dst, src);                                                           // 004C66DF..004C66E2
        str_assign(fa(dst, 8), fa(src, 8));                                             // 004C66E7..004C66EE
        wr(dst, 0x10, rd(src, 0x10));                                                   // 004C66F3..004C66F6
        wr(dst, 0x14, rd(src, 0x14));                                                   // 004C66F9..004C66FC
        wr(dst, 0x18, rd(src, 0x18));                                                   // 004C66FF..004C6702
        wr(dst, 0x1c, rd(src, 0x1c));                                                   // 004C6705..004C670C
        const U mine = rd(dst, 0x20);                                                   // 004C6712
        wr(dst, 0x20, mine ^ ((rd(src, 0x20) ^ mine) & 1));                             // 004C670F..004C671E
    }
}
