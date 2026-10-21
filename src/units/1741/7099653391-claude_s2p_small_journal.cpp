// Lane worker "s2p-small" (journal fork): 0066A370 CSWCMessage::HandleServerToPlayerJournalMessage, its ten
// sub-handlers and the CClientExoApp journal accessors. The journal containers are in claude_s2p_small_journal2.cpp.
// Transcribed from the original swkotor.exe (read-only); statement comments cite original instruction addresses.
// Conventions: claude_dialogue_common.hpp. thiscall -> extern "C" __fastcall(self, edx, stack args...).
#include "6bd5403233-claude_dialogue_common.hpp"

using namespace dlg;

namespace {
struct SJournalEntryArg { U w[15]; };       // SJournalEntry passed by value (0x3C bytes; the callee destroys it)
static_assert(sizeof(SJournalEntryArg) == 0x3c, "SJournalEntry by value");
}

extern "C" {
// w2_message_native.obj (real bodies)
U __fastcall kotor_fastcall_004d6230(void* msg, void*, U bits);                        // MessageReadOverflow RET 4
U __fastcall kotor_fastcall_004d6240(void* msg, void*, U bits);                        // MessageReadUnderflow RET 4
U __fastcall kotor_fastcall_004d6780(void* msg, void*, U bits);                        // ReadDWORD RET 4
U8 __fastcall kotor_fastcall_004d6720(void* msg, void*, U bits);                       // ReadBYTE RET 4
U16 __fastcall kotor_fastcall_004d6750(void* msg, void*, U bits);                      // ReadWORD RET 4
U __fastcall kotor_fastcall_004d66f0(void* msg, void*);                                // ReadBOOL RET 0
void* __fastcall kotor_fastcall_004d67e0(void* msg, void*, void* result, U bits);      // ReadCExoString RET 8
void* __fastcall kotor_fastcall_00692660(void* msg, void*, void* result, void* sound, U* strref); // ReadCExoLocStringClient RET 0Ch
// other real providers (c19 map)
U __fastcall kotor_fastcall_005ed4c0(void* client, void*);                             // CClientExoApp::GetClientLanguage
U __fastcall exo_locstring_add_005ea340(void* loc, void*, U language, void* text, U gender); // CExoLocString::AddString
void* __fastcall kotor_fastcall_005c53e0(void* entry, void*);                          // SJournalEntry()
void* __fastcall kotor_fastcall_005c5560(void* dst, void*, const void* src);           // SJournalEntry(const&)
void __fastcall kotor_fastcall_005c5500(void* entry, void*);                           // ~SJournalEntry
// claude_s2p_small_journal2.cpp
void __fastcall kotor_fastcall_00675db0(void* journal, void*);                         // CSWCJournal::EnableSort
void __fastcall kotor_fastcall_00675dc0(void* journal, void*);                         // CSWCJournal::DisableSort
void __fastcall kotor_fastcall_00676ba0(void* journal, void*);                         // CSWCJournal::CallFullUpdateComplete
void __fastcall kotor_fastcall_00676b00(void* journal, void*);                         // CSWCJournal::ClearAll
void __fastcall kotor_fastcall_006760f0(void* journal, void*, const void* tag);        // CSWCJournal::Remove
void __fastcall kotor_fastcall_00676260(void* journal, void*, const void* tag, U picture); // SetJournalPicture
void __fastcall kotor_fastcall_00676610(void* journal, void*, SJournalEntryArg entry, U flags); // CSWCJournal::Add
void __fastcall kotor_fastcall_00676c20(void* journal, void*, U deferrable, U completed, const void* text); // JournalUpdated
void __fastcall kotor_fastcall_00677300(void* wj, void*, U index, const void* s1, const void* s2, U t1, U t2);
void __fastcall kotor_fastcall_00677130(void* wj, void*, U strref1, U strref2, U t1, U t2);
void __fastcall kotor_fastcall_00676f00(void* wj, void*, U index);
void __fastcall kotor_fastcall_00676e60(void* wj, void*, U strref);
void __fastcall kotor_fastcall_00676fa0(void* wj, void*);
}

// Exports of this file
extern "C" {
unsigned __fastcall kotor_fastcall_0066a370(void* self, void*, unsigned minor);
U __fastcall kotor_fastcall_006632a0(void* msg, void*);
U __fastcall kotor_fastcall_00651ed0(void* msg, void*);
U __fastcall kotor_fastcall_00651fb0(void* msg, void*);
U __fastcall kotor_fastcall_006520a0(void* msg, void*);
U __fastcall kotor_fastcall_00652210(void* msg, void*);
U __fastcall kotor_fastcall_00652290(void* msg, void*);
U __fastcall kotor_fastcall_006522e0(void* msg, void*);
U __fastcall kotor_fastcall_00652330(void* msg, void*);
U __fastcall kotor_fastcall_00652380(void* msg, void*);
U __fastcall kotor_fastcall_006526e0(void* msg, void*);
void* __fastcall kotor_fastcall_005ed320(void* client, void*);
void* __fastcall kotor_fastcall_005ed330(void* client, void*);
void __fastcall kotor_fastcall_005ed340(void* client, void*, U deferrable, U completed, const void* text);
void __fastcall kotor_fastcall_005eea00(void* internal, void*, U deferrable, U completed, const void* text);
}

namespace {
inline U overflow(void* m) { return kotor_fastcall_004d6230(m, nullptr, 1); }
inline U underflow(void* m) { return kotor_fastcall_004d6240(m, nullptr, 1); }
inline U read_dword(void* m) { return kotor_fastcall_004d6780(m, nullptr, 0x20); }
// ReadCExoString into a temporary, assign, destroy the temporary
inline void read_string_into(void* m, void* dst) {
    struct Temp { U a, b; ~Temp() { frame_LogStringDtor_005e5c20(this, nullptr); } } t;  // constructed by the callee
    str_assign(dst, kotor_fastcall_004d67e0(m, nullptr, &t, 0x20));
}
// ReadCExoLocStringClient(&tmp, &resref, nullptr) -> CExoLocString::AddString(client language, tmp, 0); ~tmp
inline void read_loc_into(void* m, void* loc) {
    struct Temp { U a, b; ~Temp() { frame_LogStringDtor_005e5c20(this, nullptr); } } t;
    ResRef sound;
    resref_default(&sound);
    void* const text = kotor_fastcall_00692660(m, nullptr, &t, &sound, nullptr);
    (void)exo_locstring_add_005ea340(loc, nullptr, kotor_fastcall_005ed4c0(client(), nullptr), text, 0);
}
struct JournalEntry {                         // SJournalEntry local: ctor 005C53E0, dtor 005C5500
    alignas(4) unsigned char raw[0x3c];
    JournalEntry() { (void)kotor_fastcall_005c53e0(this, nullptr); }
    JournalEntry(const JournalEntry&) = delete;
    JournalEntry& operator=(const JournalEntry&) = delete;
    ~JournalEntry() { kotor_fastcall_005c5500(this, nullptr); }
};
inline SJournalEntryArg copy_entry(const void* src) {
    SJournalEntryArg a;
    (void)kotor_fastcall_005c5560(&a, nullptr, src);
    return a;
}
}  // namespace

// ===============================================================================================================
// CClientExoApp accessors
// ===============================================================================================================
// 005ED320 GetQuestJournal / 005ED330 GetWorldJournal RET 0 (CClientExoAppInternal +38h / +3Ch)
extern "C" void* __fastcall kotor_fastcall_005ed320(void* c, void*) { return rp(rp(c, 4), 0x38); }   // 005ED320..005ED323
extern "C" void* __fastcall kotor_fastcall_005ed330(void* c, void*) { return rp(rp(c, 4), 0x3c); }   // 005ED330..005ED333
// 005EEA00 CClientExoAppInternal::JournalUpdated(a, b, CExoString const&) RET 0Ch
extern "C" void __fastcall kotor_fastcall_005eea00(void* in, void*, U a, U b, const void* text) {
    void* const journal = rp(in, 0x38);                                                 // 005EEA00
    if (journal == nullptr) return;                                                     // 005EEA03..005EEA05 / 005EEA0C
    kotor_fastcall_00676c20(journal, nullptr, a, b, text);                              // 005EEA07 (tail jump)
}
// 005ED340 CClientExoApp::JournalUpdated(a, b, CExoString const&) RET 0Ch
extern "C" void __fastcall kotor_fastcall_005ed340(void* c, void*, U a, U b, const void* text) {
    kotor_fastcall_005eea00(rp(c, 4), nullptr, a, b, text);                             // 005ED340..005ED343 (tail jump)
}

// ===============================================================================================================
// 0066A370 CSWCMessage::HandleServerToPlayerJournalMessage(BYTE minor) RET 4
// ===============================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_0066a370(void* self, void*, unsigned minor) {
    switch (static_cast<U8>(minor)) {                                                   // 0066A370..0066A37B table 0066A3D8
    case 1: return kotor_fastcall_006520a0(self, nullptr);                              // 0066A39A AddWorld
    case 2: return kotor_fastcall_00652210(self, nullptr);                              // 0066A3A2 AddWorldStrref
    case 3: return kotor_fastcall_00652290(self, nullptr);                              // 0066A3AA DeleteWorld
    case 4: return kotor_fastcall_006522e0(self, nullptr);                              // 0066A3B2 DeleteWorldStrref
    case 5: return kotor_fastcall_00652330(self, nullptr);                              // 0066A3BA DeleteWorldAll
    case 6: return kotor_fastcall_006632a0(self, nullptr);                              // 0066A382 AddQuest
    case 7: return kotor_fastcall_00651ed0(self, nullptr);                              // 0066A38A RemoveQuest
    case 8: return kotor_fastcall_00651fb0(self, nullptr);                              // 0066A392 SetQuestPicture
    case 9: return kotor_fastcall_00652380(self, nullptr);                              // 0066A3C2 FullQuestUpdate
    case 12: return kotor_fastcall_006526e0(self, nullptr);                             // 0066A3CA Updated
    default: return 0;                                                                  // 0066A3D2 (also minors 10, 11)
    }
}

// 006632A0 HandleServerToPlayerJournalMessage_AddQuest() RET 0
extern "C" U __fastcall kotor_fastcall_006632a0(void* m, void*) {
    void* const journal = kotor_fastcall_005ed320(client(), nullptr);                   // 006632B8..006632CE edi
    JournalEntry e;                                                                     // 006632CA..006632D0 [esp+24h]
    if (overflow(m) != 0) return 0;                                                     // 006632D7..006632F9
    read_loc_into(m, e.raw);                                                            // 0066330D..0066334B name
    read_loc_into(m, e.raw + 8);                                                        // 00663350..0066338E text
    wr(e.raw, 0x10, read_dword(m));                                                     // 00663393..006633A0
    wr(e.raw, 0x14, read_dword(m));                                                     // 0066339C..006633B2
    read_string_into(m, e.raw + 0x18);                                                  // 006633A9..006633D2 tag
    wr(e.raw, 0x20, read_dword(m));                                                     // 006633D7..006633E4
    wr(e.raw, 0x24, read_dword(m));                                                     // 006633E0..006633F1
    wr16(e.raw, 0x28, kotor_fastcall_004d6750(m, nullptr, 0x10));                       // 006633ED..006633FC
    const U completed = kotor_fastcall_004d66f0(m, nullptr);                            // 006633FA..00663401
    wr(e.raw, 0x2c, (completed & 1) | (rd(e.raw, 0x2c) & 0xfffffffeu) | 4);             // 00663406..00663419
    wr(e.raw, 0x30, read_dword(m));                                                     // 00663415..00663426
    wr(e.raw, 0x34, read_dword(m));                                                     // 00663422..00663433
    wr(e.raw, 0x38, read_dword(m));                                                     // 0066342F..0066344F
    kotor_fastcall_00676610(journal, nullptr, copy_entry(e.raw), 0x8000);               // 0066343C..0066345D
    return underflow(m) != 0 ? 0u : 1u;                                                 // 00663462..0066349A
}

// 00651ED0 HandleServerToPlayerJournalMessage_RemoveQuest() RET 0
extern "C" U __fastcall kotor_fastcall_00651ed0(void* m, void*) {
    ExoString tag;                                                                      // 00651EEC..00651EF0
    void* const journal = kotor_fastcall_005ed320(client(), nullptr);                   // 00651EF5..00651F05 edi
    if (overflow(m) != 0) return 0;                                                     // 00651F0A..00651F2B
    read_string_into(m, &tag);                                                          // 00651F3D..00651F63
    kotor_fastcall_006760f0(journal, nullptr, &tag);                                    // 00651F68..00651F6F
    return underflow(m) != 0 ? 0u : 1u;                                                 // 00651F74..00651FA7
}

// 00651FB0 HandleServerToPlayerJournalMessage_SetQuestPicture() RET 0
extern "C" U __fastcall kotor_fastcall_00651fb0(void* m, void*) {
    void* const journal = kotor_fastcall_005ed320(client(), nullptr);                   // 00651FC8..00651FDD edi
    ExoString tag;                                                                      // 00651FD9..00651FDF
    if (overflow(m) != 0) return 0;                                                     // 00651FE4..0065200B
    read_string_into(m, &tag);                                                          // 0065201D..00652043
    const U picture = read_dword(m);                                                    // 00652048..0065204C
    kotor_fastcall_00676260(journal, nullptr, &tag, picture);                           // 00652051..00652059
    return underflow(m) != 0 ? 0u : 1u;                                                 // 0065205E..00652091
}

// 006520A0 HandleServerToPlayerJournalMessage_AddWorld() RET 0
extern "C" U __fastcall kotor_fastcall_006520a0(void* m, void*) {
    ExoString s1;                                                                       // 006520C4..006520C8 [esp+18h]
    ExoString s2;                                                                       // 006520CD..006520D9 [esp+10h]
    void* const wj = kotor_fastcall_005ed330(client(), nullptr);                        // 006520DE..006520F4 ebx
    if (overflow(m) != 0) return 0;                                                     // 006520F0..0065211E
    const U index = read_dword(m);                                                      // 00652133..00652145 edi
    read_string_into(m, &s1);                                                           // 0065213C..00652164
    read_string_into(m, &s2);                                                           // 00652169..0065218F
    const U t1 = read_dword(m);                                                         // 00652194..006521A1 ebp
    const U t2 = read_dword(m);                                                         // 0065219D..006521A3
    kotor_fastcall_00677300(wj, nullptr, index, &s1, &s2, t1, t2);                      // 006521A8..006521B7
    return underflow(m) != 0 ? 0u : 1u;                                                 // 006521BC..00652203
}

// 00652210 HandleServerToPlayerJournalMessage_AddWorldStrref() RET 0
extern "C" U __fastcall kotor_fastcall_00652210(void* m, void*) {
    void* const wj = kotor_fastcall_005ed330(client(), nullptr);                        // 00652211..00652225 [esp+8]
    if (overflow(m) != 0) return 0;                                                     // 00652221..00652236
    const U a = read_dword(m);                                                          // 0065223A..00652247 edi
    const U b = read_dword(m);                                                          // 00652243..00652252 ebx
    const U c = read_dword(m);                                                          // 0065224E..0065225D ebp
    const U d = read_dword(m);                                                          // 00652259..0065225F
    kotor_fastcall_00677130(wj, nullptr, a, b, c, d);                                   // 00652264..0065226C
    return underflow(m) != 0 ? 0u : 1u;                                                 // 00652271..00652284 (neg/sbb/inc)
}

// 00652290 HandleServerToPlayerJournalMessage_DeleteWorld() RET 0
extern "C" U __fastcall kotor_fastcall_00652290(void* m, void*) {
    void* const wj = kotor_fastcall_005ed330(client(), nullptr);                        // 00652290..006522A5 edi
    if (overflow(m) != 0) return 0;                                                     // 006522A1..006522B4
    kotor_fastcall_00676f00(wj, nullptr, read_dword(m));                                // 006522B5..006522C1
    return underflow(m) != 0 ? 0u : 1u;                                                 // 006522C6..006522D6
}

// 006522E0 HandleServerToPlayerJournalMessage_DeleteWorldStrref() RET 0
extern "C" U __fastcall kotor_fastcall_006522e0(void* m, void*) {
    void* const wj = kotor_fastcall_005ed330(client(), nullptr);                        // 006522E0..006522F5 edi
    if (overflow(m) != 0) return 0;                                                     // 006522F1..00652304
    kotor_fastcall_00676e60(wj, nullptr, read_dword(m));                                // 00652305..00652311
    return underflow(m) != 0 ? 0u : 1u;                                                 // 00652316..00652326
}

// 00652330 HandleServerToPlayerJournalMessage_DeleteWorldAll() RET 0
extern "C" U __fastcall kotor_fastcall_00652330(void* m, void*) {
    void* const wj = kotor_fastcall_005ed330(client(), nullptr);                        // 00652330..00652345 edi
    if (overflow(m) != 0) return 0;                                                     // 00652341..00652354
    (void)kotor_fastcall_004d6720(m, nullptr, 8);                                       // 00652355..00652359 (value unused)
    kotor_fastcall_00676fa0(wj, nullptr);                                               // 0065235E..00652360
    return underflow(m) != 0 ? 0u : 1u;                                                 // 00652365..00652375
}

// 00652380 HandleServerToPlayerJournalMessage_FullQuestUpdate() RET 0
extern "C" U __fastcall kotor_fastcall_00652380(void* m, void*) {
    void* const journal = kotor_fastcall_005ed320(client(), nullptr);                   // 006523A1..006523B8 ebp
    JournalEntry e;                                                                     // 006523B4..006523BA [esp+54h] (reused per entry)
    if (overflow(m) != 0) {                                                             // 006523BF..006523D5
        kotor_fastcall_00676ba0(journal, nullptr);                                      // 006523D7..006523D9
        return 0;                                                                       // 006523DE..00652409
    }
    if (kotor_fastcall_004d66f0(m, nullptr) != 0) {                                     // 0065240A..00652415
        kotor_fastcall_00675dc0(journal, nullptr);                                      // 0065241B
        const U full = kotor_fastcall_004d66f0(m, nullptr);                             // 00652420..00652427 edi
        if (full != 0) kotor_fastcall_00676b00(journal, nullptr);                       // 00652429..0065242F
        I count = static_cast<I>(read_dword(m));                                        // 00652434..00652438
        if (count > 0) {                                                                // 0065243D..0065243F (jle)
            do {                                                                        // 00652450..00652688 [esp+14h]
                const U flags = kotor_fastcall_004d6750(m, nullptr, 0x10);              // 00652450..00652462 ebx
                const U8 lo = static_cast<U8>(flags);                                   // bl
                const U8 hi = static_cast<U8>(flags >> 8);                              // bh
                read_string_into(m, e.raw + 0x18);                                      // 00652459..00652487 tag
                if (hi & 0x40) {                                                        // 0065248C..0065248F
                    kotor_fastcall_006760f0(journal, nullptr, e.raw + 0x18);            // 00652678..0065267F
                    continue;                                                           // 00652684
                }
                const bool all = full != 0 || (hi & 0x80) != 0;                         // test edi / test bh,bh js
                if (all || (lo & 1)) read_loc_into(m, e.raw);                           // 00652495..006524E9
                if (all || (lo & 2)) read_loc_into(m, e.raw + 8);                       // 006524EE..00652542
                if (all || (lo & 4)) wr(e.raw, 0x10, read_dword(m));                    // 00652547..0065255F
                if (all || (lo & 8)) wr(e.raw, 0x14, read_dword(m));                    // 00652563..00652579
                if (all || (lo & 0x10)) wr(e.raw, 0x20, read_dword(m));                 // 0065257D..00652593
                if (all || (lo & 0x20)) wr(e.raw, 0x24, read_dword(m));                 // 00652597..006525AD
                if (all || (lo & 0x40)) wr16(e.raw, 0x28, kotor_fastcall_004d6750(m, nullptr, 0x10)); // 006525B1..006525C7
                if (all || (lo & 0x80)) {                                               // 006525CC..006525D4
                    const U b = kotor_fastcall_004d66f0(m, nullptr);                    // 006525D6..006525D8
                    const U old = rd(e.raw, 0x2c);
                    wr(e.raw, 0x2c, old ^ ((b ^ old) & 1));                             // 006525DD..006525ED
                }
                if (full != 0 || (hi & 0x82)) wr(e.raw, 0x30, read_dword(m));           // 006525EB..00652606
                if (full != 0 || (hi & 0x84)) wr(e.raw, 0x34, read_dword(m));           // 0065260D..0065261F
                if (full != 0 || (hi & 0x88)) wr(e.raw, 0x38, read_dword(m));           // 00652626..00652636
                U bits = rd(e.raw, 0x2c);                                               // 00652640
                bits = (hi & 1) ? (bits & 0xfffffffbu) : (bits | 4);                    // 0065263D..0065264E
                wr(e.raw, 0x2c, bits);                                                  // 00652663 (before the copy)
                kotor_fastcall_00676610(journal, nullptr, copy_entry(e.raw), flags);    // 00652651..00652671
            } while (--count != 0);                                                     // 00652684..00652688
        }
        kotor_fastcall_00675db0(journal, nullptr);                                      // 0065268E..00652690
    }
    kotor_fastcall_00676ba0(journal, nullptr);                                          // 00652695..00652697
    return underflow(m) != 0 ? 0u : 1u;                                                 // 0065269C..006526DB
}

// 006526E0 HandleServerToPlayerJournalMessage_Updated() RET 0
extern "C" U __fastcall kotor_fastcall_006526e0(void* m, void*) {
    (void)kotor_fastcall_005ed330(client(), nullptr);                                   // 006526F8..00652703 (result unused)
    ExoString text;                                                                     // 00652708..0065270C [esp+4]
    if (overflow(m) != 0) return 0;                                                     // 00652711..00652737
    {
        struct Temp { U a, b; ~Temp() { frame_LogStringDtor_005e5c20(this, nullptr); } } t;
        ResRef sound;
        resref_default(&sound);                                                         // 0065274A..00652750
        str_assign(&text, kotor_fastcall_00692660(m, nullptr, &t, &sound, nullptr));    // 00652755..0065276C
    }                                                                                   // 00652771..0065277A
    const U a = kotor_fastcall_004d66f0(m, nullptr);                                    // 0065277F..00652786 edi
    const U b = kotor_fastcall_004d66f0(m, nullptr);                                    // 00652786..0065278A
    kotor_fastcall_005ed340(client(), nullptr, a, b, &text);                            // 0065278F..0065279F
    return underflow(m) != 0 ? 0u : 1u;                                                 // 006527A4..006527DB
}
