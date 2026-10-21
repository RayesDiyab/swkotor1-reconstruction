// Lane "dialogue-start", fork A: server dialogue flow (reply handling, entry/reply node publication, journal,
// plot XP, dialog delays, item unequip for dialog). Transcribed from the original swkotor.exe (read-only);
// statement comments cite the original instruction addresses.
// Conventions: see claude_dialogue_common.hpp. thiscall -> extern "C" __fastcall(self, edx, args...).
#include "6bd5403233-claude_dialogue_common.hpp"

using namespace dlg;

// ---------------------------------------------------------------------------------------------------------------
// Additional providers (c05 map, real bodies; checked against the c05 trap census)
// ---------------------------------------------------------------------------------------------------------------
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString* out, const char* format, ...); }
extern "C" {
extern void* kotor_cell_rules_007a3a28;                       // [007A3A28] CSWRules* (game-prepare cell)
extern void* kotor_cell_tlk_007a3a08;                         // [007A3A08] CTlkTable* (tlk-cells)
extern volatile std::uint32_t kotor_cell_disable_sound_00832894;  // [00832894] dword (sound-owners)
double __cdecl ceil(double);                                  // 006FC880 _ceil == libucrt ceil (same MSVC CRT routine)
void* __fastcall recovered_party_journal_005633c0(void* party, void*);                     // GetPartyJournal
dlg::U __fastcall c2da_current_get_int_00413660(void* table, void*, dlg::U row, void* col, dlg::I* out); // C2DA::GetINTEntry
void __fastcall recovered_party_distribute_experience_005653a0(void* party, void*, dlg::U xp, dlg::U flag);
void __fastcall kotor_fastcall_0062eeb0(void* gui, void*, std::uint8_t kind, std::uint32_t value); // CGuiInGame::UpdateStatus
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                             // GetWorldTimer
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, dlg::U* day, dlg::U* time);
void __fastcall kotor_fastcall_004adf50(void* timer, void*, dlg::U day, dlg::U time, dlg::U add_day, dlg::U add_time,
                                        dlg::U* out_day, dlg::U* out_time);                 // AddWorldTimes
dlg::U __fastcall recovered_world_time_difference_004ae460(void* timer, void*, dlg::U day, dlg::U time, dlg::U last_day,
                                                           dlg::U last_time, dlg::U* out_day, dlg::U* out_time); // SubtractWorldTimes
dlg::U __fastcall exo_locstring_get_005ea130(void* loc, void*, dlg::U language, void* out, dlg::U gender);
dlg::U __fastcall exo_locinternal_get_005ec9c0(void* internal, void*, dlg::U id, void* out);
dlg::U __fastcall kotor_fastcall_005ea240(void* loc, void*, dlg::U language, void* out, dlg::U gender); // GetStringLoc
dlg::U __fastcall tlk_fetch_0041e550(void* tlk, void*, dlg::U strref, void* out, dlg::U flag);
void* __fastcall exo_locinternal_construct_005ecaf0(void* internal, void*);
void __fastcall exo_locinternal_clear_005ecb10(void* internal, void*);
dlg::U __fastcall kotor_m15e_slot010_zero_0063e7f0(void* self, void*);                      // 0063E7F0 return_zero
void* __fastcall c2da_005e9d70(void* list, void*, void* data);                              // CExoLinkedListInternal::AddHead
void* __fastcall kotor_fastcall_005ed580(void* client, void*, dlg::U id);                  // CClientExoApp::GetGameObject
void __fastcall kotor_fastcall_00614630(void* creature, void*, void* a, void* b);          // CSWCCreature::HideEquippedItems
void __fastcall kotor_fastcall_00614670(void* creature, void*);                            // CSWCCreature::ShowEquippedItems
void* __fastcall kotor_fastcall_004ae920(void* server, void*, dlg::U id, dlg::U flag);      // GetClientObjectByPlayerId
void* __fastcall kotor_fastcall_00561080(void* player, void*);                             // CSWSPlayer::GetGameObject
void* __fastcall combat_reuse_faction_00513fc0(void* creature, void*);                     // CSWSCreature::GetFaction
void* __fastcall kotor_fastcall_004ae770(void* server, void*, dlg::U id);                  // GetCreatureByGameObjectID
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);               // GetSWSMessage
int __fastcall recovered_localized_different_005ea030(void* loc, void*, const void* other); // CExoLocString::operator!=
void* __fastcall lower_construct_005e6080(void* s, void*, void* out);                      // CExoString::LowerCase
void __fastcall kotor_fastcall_005c5500(void* entry, void*);                               // SJournalEntry::~SJournalEntry
void* __fastcall tlk_token_construct_0041d910(void* update, void*);                        // CSWSPlayerJournalQuestUpdates()
void __fastcall kotor_fastcall_004d68a0(void* msg, void*, dlg::U size, dlg::U a, dlg::U b); // CreateWriteMessage
void __fastcall kotor_fastcall_005785c0(void* msg, void*, void* loc, dlg::U gender);       // WriteCExoLocStringServer
void __fastcall kotor_fastcall_004d6340(void* msg, void*, dlg::U value, dlg::U bits);      // WriteDWORD
void __fastcall kotor_fastcall_004d6960(void* msg, void*, const void* s, dlg::U bits);     // WriteCExoString
void __fastcall kotor_fastcall_004d6300(void* msg, void*, dlg::U value, dlg::U bits);      // WriteWORD
void __fastcall kotor_fastcall_004d68e0(void* msg, void*, dlg::U value);                   // WriteBOOL
dlg::U __fastcall kotor_fastcall_004d6440(void* msg, void*, void** data, dlg::U* size);    // GetWriteMessage
dlg::U __fastcall kotor_fastcall_00567e60(void* msg, void*, dlg::U player, dlg::U major, dlg::U minor, void* data,
                                          dlg::U size);                                     // SendServerToPlayerMessage
void* __fastcall kotor_fastcall_005ed8b0(void* client, void*);                             // CClientExoApp::GetSWParty
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, dlg::U index);            // CSWParty::GetCharacter
dlg::U __fastcall kotor_fastcall_0062f840(void* gui, void*, void* name);                   // IsOnDialogNonDependantList
void* __fastcall frame_LogAssign_005e5140(void* s, void*, const char* text);              // CExoString::operator=(char*)
dlg::U __fastcall kotor_fastcall_00411240(void* gff, void*, void* out_struct);             // (start.cpp) GetTopLevelStruct
// delivered dialogue objects
dlg::U __fastcall kotor_fastcall_004cb5c0(void* object, void*, void* name, dlg::U flag);   // D5 GetNearestObjectByName
int __fastcall kotor_fastcall_005a0a40(void* dialog, void*, void* object, dlg::U flag);    // D3 RunEndConversationScript
// fork B1 (claude_dialogue_client.cpp)
int __fastcall kotor_fastcall_0062e100(void* gui, void*, dlg::U player, dlg::U reply, dlg::U entry, void* text,
                                       dlg::U speaker, dlg::U8 gender, dlg::I a7);           // HandleDialogReplyChosen
dlg::U __fastcall kotor_fastcall_00631d80(void* gui, void*, dlg::U a1, dlg::U a2, dlg::U a3, dlg::U a4, dlg::U a5,
    dlg::U a6, void* a7, dlg::U a8, const dlg::U* a9, const dlg::U16* a10, dlg::U a11, dlg::U a12, dlg::U a13, dlg::U a14,
    dlg::U a15, dlg::U a16, dlg::U a17, dlg::U a18, dlg::U a19, dlg::U a20, dlg::U a21, dlg::U a22, dlg::U a23); // HandleDialogEntry
dlg::U __fastcall kotor_fastcall_00634340(void* gui, void*, dlg::U a1, dlg::U a2, dlg::U a3, void* a4, dlg::U a5,
    dlg::U a6, dlg::U a7, dlg::U a8, dlg::U a9, dlg::U a10, dlg::U a11, dlg::U a12, dlg::U a13, dlg::U a14, dlg::U a15,
    dlg::U a16, dlg::U a17, dlg::U a18, dlg::U a19, dlg::U a20, dlg::U a21, dlg::U a22, dlg::U a23, dlg::U a24,
    dlg::U a25);                                                                             // HandleDialogReplies
}

// Exports of this file
extern "C" {
void __fastcall kotor_fastcall_004cb1a0(void* object, void*, float delay);
dlg::U __fastcall kotor_fastcall_004cb480(void* object, void*, dlg::U player, dlg::U reply, dlg::U flag, dlg::U entry,
                                          dlg::U skip_delay);
void* __fastcall kotor_fastcall_004edb00(void* creature, void*);
void __fastcall kotor_fastcall_004ef910(void* creature, void*, void* unused);
void __fastcall kotor_fastcall_00585460(void* placeable, void*, void* unused);
void __fastcall kotor_fastcall_005666e0(void* party, void*, dlg::U row, dlg::I percent);
void __fastcall kotor_fastcall_0059ef00(void* dialog, void*, void* quest, dlg::U entry, dlg::U player);
void* __fastcall kotor_fastcall_0059f140(void* dst, void*, const void* src);
void* __fastcall kotor_fastcall_0059f320(void* dialog, void*, void* out, void* loc, dlg::U gender);
void __fastcall kotor_fastcall_0059f3c0(void* dialog, void*, dlg::I plot_index, float percent);
float __fastcall kotor_fastcall_0059ff50(void* dialog, void*, void* object, void* text, dlg::U min_delay, dlg::U has_links,
                                         dlg::U a5);
void* __fastcall kotor_fastcall_005a0360(void* dst, void*, const void* src);
dlg::U __fastcall kotor_fastcall_005a03c0(void* dialog, void*, dlg::U player, void* object, dlg::U reply, dlg::U force,
                                          dlg::U entry, dlg::U skip_delay);
void __fastcall kotor_fastcall_005a0ba0(void* dialog, void*, dlg::U id);
void __fastcall kotor_fastcall_005a0c80(void* dialog, void*);
void __fastcall kotor_fastcall_005a0d10(void* dialog, void*, dlg::U id);
void __fastcall kotor_fastcall_005a0dd0(void* dialog, void*);
dlg::U __fastcall kotor_fastcall_005a13d0(void* dialog, void*, dlg::U player, dlg::U owner, dlg::U speaker, dlg::U listener,
                                          dlg::U target, dlg::U gender, void* object, void* entry, dlg::U* status);
dlg::U __fastcall kotor_fastcall_005a1c00(void* dialog, void*, dlg::U player, dlg::U id, dlg::U gender, void* replies,
                                          dlg::I active, dlg::I appended, dlg::U entry, dlg::U flag);
dlg::I __fastcall kotor_fastcall_005c5850(void* journal, void*, void* tag);
dlg::I __fastcall kotor_fastcall_005c58a0(void* journal, void*, void* tag);
void __fastcall kotor_fastcall_005c5a40(void* journal, void*, void* tag, dlg::U state, dlg::U flag);
void __fastcall kotor_fastcall_005c63f0(void* journal, void*, void* tag, dlg::U date, dlg::U flag);
void __fastcall kotor_fastcall_005c6650(void* journal, void*, void* tag, dlg::U time, dlg::U flag);
void* __fastcall kotor_fastcall_005c53e0(void* entry, void*);
void* __fastcall kotor_fastcall_005c5470(void* dst, void*, const void* src);
void* __fastcall kotor_fastcall_005c5560(void* dst, void*, const void* src);
void __fastcall kotor_fastcall_005c5680(void* list, void*, dlg::U count);
void __fastcall kotor_fastcall_00527490(void* list, void*, dlg::U count);
void __fastcall kotor_fastcall_0056a020(void* msg, void*, void* player, void* tag, dlg::U state, dlg::U priority,
    dlg::U picture, dlg::U end, dlg::U date, dlg::U time, void* name, void* text, dlg::U plot, dlg::U planet, dlg::U xp);
void __fastcall kotor_fastcall_0056a4e0(void* msg, void*, void* player, dlg::U a, dlg::U end, void* entry);
dlg::U __cdecl kotor_cdecl_00578a20(dlg::U player, void* loc, void* out, float* seconds, dlg::U gender);
dlg::U __fastcall kotor_fastcall_005ea050(void* loc, void*, dlg::U language, void* out, dlg::U gender);
float __fastcall kotor_fastcall_005ea270(void* loc, void*, dlg::U language, dlg::U gender);
dlg::U __fastcall kotor_fastcall_005ee0d0(void* client, void*, dlg::U mask);
void __fastcall kotor_fastcall_0060e210(void* creature, void*);
void __fastcall kotor_fastcall_0060e230(void* creature, void*);
dlg::U __fastcall kotor_fastcall_004ae820(void* server, void*);
dlg::U __fastcall kotor_fastcall_005e9eb0(void* loc, void*, dlg::U index, dlg::I* language, void* out, U8* gender);
dlg::U __fastcall kotor_fastcall_005ec950(void* internal, void*, dlg::U index, dlg::I* language, void* out);
void* __fastcall kotor_fastcall_005e9f80(void* loc, void*, const void* src);
dlg::U __fastcall kotor_fastcall_005ecbb0(void* internal, void*, const void* src);
}


// CExoString::operator==(char const*) 005E5290 -> 0077 StartupString::BorrowedEquals005e5290 (real body)
namespace kotor_reconstruction {
struct StartupString { void* data; dlg::U length; int __thiscall BorrowedEquals005e5290(const char*); };
}

namespace {
inline void* party_table() { return lane_config_required::GetPartyTable_004aee70(server(), nullptr); }
inline U ftol(double v) { return w2::ftol2_006fae8c(v); }
inline int str_equals(void* s, const char* t) {
    return reinterpret_cast<kotor_reconstruction::StartupString*>(s)->BorrowedEquals005e5290(t);
}
inline int str_nc_s(void* s, const void* o) { return exostring_compare_no_case_005e6450(s, nullptr, o); }
struct CopiedString {                         // CExoString(CExoString const&) local
    void* data = nullptr;
    U length = 0;
    explicit CopiedString(const void* src) { (void)WindowCopy005e5b00(this, nullptr, src); }
    CopiedString(const CopiedString&) = delete;
    CopiedString& operator=(const CopiedString&) = delete;
    ~CopiedString() { frame_LogStringDtor_005e5c20(this, nullptr); }
};
struct ResultString {                         // CExoString storage constructed by a hidden-return callee
    void* data = nullptr;
    U length = 0;
    ResultString() = default;
    ResultString(const ResultString&) = delete;
    ResultString& operator=(const ResultString&) = delete;
    ~ResultString() { frame_LogStringDtor_005e5c20(this, nullptr); }
};
struct ResultLocString {                      // CExoLocString storage constructed by a hidden-return callee
    U a = 0, b = 0;
    ResultLocString() = default;
    ResultLocString(const ResultLocString&) = delete;
    ResultLocString& operator=(const ResultLocString&) = delete;
    ~ResultLocString() { exo_locstring_destroy_005ea000(this, nullptr); }
};
struct GffList { U s; char label[16]; };      // CResList (20 bytes)
static_assert(sizeof(GffList) == 20, "CResList");
struct JournalEntry {                         // SJournalEntry local (0x3C): ctor 005C53E0, dtor 005C5500
    alignas(4) unsigned char raw[0x3c];
    JournalEntry() { (void)kotor_fastcall_005c53e0(this, nullptr); }
    JournalEntry(const JournalEntry&) = delete;
    JournalEntry& operator=(const JournalEntry&) = delete;
    ~JournalEntry() { kotor_fastcall_005c5500(this, nullptr); }
};
struct RawJournalEntry { alignas(4) unsigned char raw[0x3c]; };   // by-value argument storage
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }   // AsSWSCreature (vtable +30h)
}  // namespace

// ===============================================================================================================
// Small leaves
// ===============================================================================================================
// 004CB1A0 CSWSObject::SetDialogDelay(float seconds) RET 4: +38h/+3Ch = world time (+40h/+44h now) + seconds*1000 ms
extern "C" void __fastcall kotor_fastcall_004cb1a0(void* self, void*, float seconds) {
    frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server(), nullptr), nullptr,
                                reinterpret_cast<U*>(at(self, 0x40)), reinterpret_cast<U*>(at(self, 0x44))); // 004CB1A1..004CB1BE
    const I k = 1000;                                                    // fild [00744174] (.rdata dword 000003E8)
    double scale = static_cast<double>(k);                               // 004CB1C3
    if (k < 0) scale += 4294967296.0;                                    // 004CB1DD..004CB1E3 (fadd [0073D770])
    const U ms = ftol(scale * static_cast<double>(seconds));             // 004CB1E9..004CB1ED
    kotor_fastcall_004adf50(kotor_fastcall_004aede0(server(), nullptr), nullptr, rd(self, 0x40), rd(self, 0x44), 0, ms,
                            reinterpret_cast<U*>(at(self, 0x38)), reinterpret_cast<U*>(at(self, 0x3c))); // 004CB1F2..004CB204
}

// 004CB480 CSWSObject::ReplyDialog(player, reply, flag, entry, skip) RET 14h
extern "C" U __fastcall kotor_fastcall_004cb480(void* self, void*, U player, U reply, U flag, U entry, U skip) {
    void* d = rp(self, 0x34);
    if (d == nullptr) return 0;                                                              // 004CB483..004CB48D
    if (kotor_fastcall_005a03c0(d, nullptr, player, self, reply, flag, entry, skip) == 0) {  // 004CB490..004CB4B1
        void* d2 = rp(self, 0x34);
        if (d2 != nullptr) {                                                                 // 004CB4B3..004CB4B8
            (void)kotor_fastcall_005a0a40(d2, nullptr, self, 0);                             // 004CB4BA..004CB4BC
            wr(self, 0x38, 0); wr(self, 0x3c, 0); wr(self, 0x48, 0); wr(self, 0x214, 1);     // 004CB4C1..004CB4D6
        }
    }
    return 1;                                                                                // 004CB4E0
}

// 004EDB00 CSWSCreature::GetJournal(): GetPartyTable()->GetPartyJournal() (this unused)
extern "C" void* __fastcall kotor_fastcall_004edb00(void*, void*) {
    return recovered_party_journal_005633c0(party_table(), nullptr);                         // 004EDB00..004EDB0F
}

// 004EF910 CSWSCreature::OnEndDialog(unused) RET 4: RunScript(+298h, id, 1)
extern "C" void __fastcall kotor_fastcall_004ef910(void* self, void*, void*) {
    (void)frame_R4_005d0fc0(vm(), nullptr, at(self, 0x298), rd(self, 4), 1);                // 004EF910..004EF923
}

// 00585460 CSWSPlaceable::OnEndDialog(unused) RET 4: RunScript(+30Ch, id, 1); +43Ch = 0
extern "C" void __fastcall kotor_fastcall_00585460(void* self, void*, void*) {
    (void)frame_R4_005d0fc0(vm(), nullptr, at(self, 0x30c), rd(self, 4), 1);                // 00585463..00585476
    wr(self, 0x43c, 0);                                                                      // 0058547B
}

// 005EE0D0 CClientExoApp::GetModuleResourceLoaded(BYTE mask) RET 4: (internal+498h & mask) != 0
extern "C" U __fastcall kotor_fastcall_005ee0d0(void* client_app, void*, U mask) {
    const U8 v = static_cast<U8>(rd8(rp(client_app, 4), 0x498) & static_cast<U8>(mask));    // 005EE0D0..005EE0D9
    return v != 0 ? 1u : 0u;                                                                 // 005EE0DD..005EE0E3
}

// 0060E210 / 0060E230 CSWCCreature::Hide/ShowEquippedHeadItem(): model (+68h) vtable +88h / +8Ch; +3D0h = 1 / 0
extern "C" void __fastcall kotor_fastcall_0060e210(void* self, void*) {
    void* m = rp(self, 0x68);
    if (m != nullptr) { vcall<void>(m, 0x88); wr(self, 0x3d0, 1); }                          // 0060E213..0060E222
}
extern "C" void __fastcall kotor_fastcall_0060e230(void* self, void*) {
    void* m = rp(self, 0x68);
    if (m != nullptr) { vcall<void>(m, 0x8c); wr(self, 0x3d0, 0); }                          // 0060E233..0060E242
}

// 004AE820 CServerExoApp::GetModuleLanguage(): tail-jump to 0063E7F0 return_zero with ECX = internal
extern "C" U __fastcall kotor_fastcall_004ae820(void* server_app, void*) {
    return kotor_m15e_slot010_zero_0063e7f0(rp(server_app, 4), nullptr);                     // 004AE820..004AE823
}

// 005EC950 CExoLocStringInternal::GetString(index, int* language, CExoString& out) RET 0Ch: the index-th node
extern "C" U __fastcall kotor_fastcall_005ec950(void* self, void*, U index, I* language, void* out) {
    if (index >= rd(self, 4)) return 0;                                                      // 005EC958..005EC961
    void* cursor = rp(rp(self, 0), 0);                                                       // 005EC964..005EC968
    void* data = cursor ? rp(cursor, 8) : nullptr;                                           // 005EC96C..005EC972
    for (U i = 0; data != nullptr; ++i) {                                                    // 005EC976..005EC993
        if (i == index) {
            *language = rdi(data, 0);                                                        // 005EC99D..005EC9A6
            str_assign(out, at(data, 4));                                                    // 005EC9A8..005EC9AD
            return 1;
        }
        data = unresolved_list_next_005e9b90(rp(self, 0), nullptr, &cursor);                 // 005EC984..005EC98B
    }
    return 0;
}

// 005E9EB0 CExoLocString::GetString(index, int* language, CExoString& out, BYTE* gender) RET 10h
extern "C" U __fastcall kotor_fastcall_005e9eb0(void* self, void*, U index, I* language, void* out, U8* gender) {
    const U r = kotor_fastcall_005ec950(rp(self, 0), nullptr, index, language, out);         // 005E9EB0..005E9EC2
    *gender = static_cast<U8>(*language % 2);                                                // 005E9EC9..005E9EDB (signed remainder)
    *language = *language / 2;                                                               // 005E9EDD..005E9EE4 (signed, toward 0)
    return r;
}

// 005ECBB0 CExoLocStringInternal::Assign(CExoLocStringInternal const&) RET 4
extern "C" U __fastcall kotor_fastcall_005ecbb0(void* self, void*, const void* src) {
    exo_locinternal_clear_005ecb10(self, nullptr);                                           // 005ECBCC
    void* cursor = rp(rp(src, 0), 0);                                                        // 005ECBD5..005ECBDA
    void* data = cursor ? rp(cursor, 8) : nullptr;                                           // 005ECBDE..005ECBE4
    while (data != nullptr) {                                                                // 005ECBE7..005ECC54
        void* node = live_operator_new(0xc);                                                 // 005ECBF0..005ECBF2
        if (node == nullptr) return 0;                                                       // 005ECC18..005ECC22 -> 005ECC76
        (void)kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(static_cast<void*>(at(node, 4))),
                                                    nullptr);                                // 005ECC0C..005ECC0F
        wr(node, 0, rd(data, 0));                                                            // 005ECC24..005ECC2D
        str_assign(at(node, 4), at(data, 4));                                                // 005ECC26..005ECC2F
        wr(self, 4, rd(self, 4) + 1);                                                        // 005ECC34..005ECC3B
        (void)resource_append_005e9dc0(rp(self, 0), nullptr, node);                          // 005ECC37..005ECC3E
        data = unresolved_list_next_005e9b90(rp(src, 0), nullptr, &cursor);                  // 005ECC43..005ECC4B
    }
    wr(self, 8, rd(src, 8));                                                                 // 005ECC56..005ECC59
    return 1;
}

// 005E9F80 CExoLocString::CExoLocString(CExoLocString const&) RET 4
extern "C" void* __fastcall kotor_fastcall_005e9f80(void* self, void*, const void* src) {
    void* mem = live_operator_new(0xc);                                                      // 005E9F98..005E9F9C
    void* internal = mem ? exo_locinternal_construct_005ecaf0(mem, nullptr) : nullptr;       // 005E9FA8..005E9FBD
    wp(self, 0, internal);                                                                   // 005E9FCD
    if (self != src) {                                                                       // 005E9FC3..005E9FCF
        (void)kotor_fastcall_005ecbb0(internal, nullptr, rp(src, 0));                         // 005E9FD1..005E9FD6
        wr(self, 4, rd(src, 4));                                                             // 005E9FDB..005E9FDE
    }
    return self;
}

// 005EA050 CExoLocString::GetString(language, CExoString& out, BYTE gender) RET 0Ch
extern "C" U __fastcall kotor_fastcall_005ea050(void* self, void*, U language, void* out, U gender) {
    const U8 g = static_cast<U8>((language != 0 ? 0xffu : 0u) & gender);                    // 005EA073..005EA080
    const U gender_slot = (gender & 0xffffff00u) | g;                                        // 005EA082 byte store into arg slot
    if (exo_locinternal_get_005ec9c0(rp(self, 0), nullptr, g + language * 2, out) != 0) return 1; // 005EA086..005EA096
    struct { ExoString text; ResRef sound; float length; } tmp;                             // 005EA0B0..005EA0C5 [esp+8] 0x1C-byte Fetch result
    resref_default(&tmp.sound);                                                              // 005EA0B9..005EA0C5 (CResRef at +8)
    U r = 0;
    if (kotor_cell_tlk_007a3a08 != nullptr &&                                                // 005EA0CA..005EA0DA
        tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, rd(self, 4), &tmp, gender_slot) != 0) { // 005EA0DC..005EA0F1 (lea esp+8)
        str_assign(out, &tmp.text);                                                          // 005EA0F3..005EA0FA
        r = 1;
    }
    return r;
}

// 005EA270 CExoLocString::GetSpokenTimeEstimate(language, gender) RET 8 -> float (ST0)
extern "C" float __fastcall kotor_fastcall_005ea270(void* self, void*, U language, U gender) {
    ExoString text;                                                                          // 005EA28B..005EA28F
    const U8 g = static_cast<U8>((language != 0 ? 0xffu : 0u) & gender);                    // 005EA29C..005EA2AD
    (void)kotor_fastcall_005ea050(self, nullptr, g + language * 2, &text, 0);                // 005EA2A3..005EA2BD
    if (async_string_length_005e5790(&text, nullptr) == 0) return 0.0f;                      // 005EA2C6..005EA2D2, [0073D700]
    const I len = static_cast<I>(async_string_length_005e5790(&text, nullptr));             // 005EA2D4..005EA2DD
    return static_cast<float>(static_cast<double>(len) * static_cast<double>(0.11f)         // fmul [0074E3E0]
                              + static_cast<double>(1.0f));                                  // fadd [0073D764]; fstp dword
}

// 0059F320 CSWSDialog::GetExoStringFromLocString(CExoString* out, CExoLocString* loc, BYTE gender) RET 0Ch
extern "C" void* __fastcall kotor_fastcall_0059f320(void*, void*, void* out, void* loc, U gender) {
    const U pc = kotor_fastcall_004aea40(server(), nullptr);                                 // 0059F340..0059F349
    ExoString tmp;                                                                           // 0059F34E..0059F354
    const U language = kotor_fastcall_004ae810(server(), nullptr, pc);                      // 0059F359..0059F375
    (void)exo_locstring_get_005ea130(loc, nullptr, language, &tmp, gender);                  // 0059F37A..0059F37F
    (void)WindowCopy005e5b00(out, nullptr, &tmp);                                            // 0059F384..0059F38F
    return out;
}

// 0059F140 CSWSDialogBase::operator=(CSWSDialogBase const&) RET 4
extern "C" void* __fastcall kotor_fastcall_0059f140(void* dst, void*, const void* src) {
    const char* s = static_cast<const char*>(src);
    str_assign(dst, s);                                                                      // 0059F145..0059F14C
    (void)recovered_localized_assign_005e9e70(at(dst, 8), nullptr, s + 8);                   // 0059F151..0059F158
    resref_assign(at(dst, 0x10), s + 0x10);                                                  // 0059F15D..0059F169
    for (U o = 0x20; o <= 0x30; o += 4) wr(dst, o, rd(src, o));                              // 0059F16E..0059F191
    resref_assign(at(dst, 0x34), s + 0x34);                                                  // 0059F189..0059F198
    str_assign(at(dst, 0x44), s + 0x44);                                                     // 0059F19D..0059F1A4
    str_assign(at(dst, 0x4c), s + 0x4c);                                                     // 0059F1A9..0059F1B0
    for (U o = 0x54; o <= 0x64; o += 4) wr(dst, o, rd(src, o));                              // 0059F1B5..0059F1D0
    wr8(dst, 0x68, rd8(src, 0x68));                                                          // 0059F1D3..0059F1D6
    for (U o = 0x6c; o <= 0x7c; o += 4) wr(dst, o, rd(src, o));                              // 0059F1D9..0059F1F8
    wr8(dst, 0x80, rd8(src, 0x80));                                                          // 0059F1FB..0059F201
    return dst;
}

// 005A0360 CSWSDialogNode::operator=(CSWSDialogNode const&) RET 4 (null src -> null camera source: original fault)
extern "C" void* __fastcall kotor_fastcall_005a0360(void* dst, void*, const void* src) {
    (void)kotor_fastcall_0059f140(dst, nullptr, src);                                        // 005A0362..005A036A
    const void* sc = src != nullptr ? static_cast<const char*>(src) + 0x84 : nullptr;        // 005A036F..005A037B
    void* dc = at(dst, 0x84);                                                                // 005A037D
    str_assign(dc, sc);                                                                      // 005A0383..005A0386
    for (U o = 8; o <= 0x14; o += 4) wr(dc, o, rd(sc, o));                                   // 005A038B..005A03A0
    wr16(dc, 0x18, rd16(sc, 0x18));                                                          // 005A03A3..005A03A7
    wr(dc, 0x1c, rd(sc, 0x1c)); wr(dc, 0x20, rd(sc, 0x20));                                  // 005A03AB..005A03B4
    return dst;
}

// 006FC880 _ceil is the MSVC CRT ceil (SSE2 path when [00834314] and default MXCSR/x87 masks, x87 path otherwise);
// bound to the toolchain CRT ceil (libucrt ceil_pentium4 / ceil_default), the same routine family; exact results.

// 005666E0 CSWPartyTable::GivePlotXP(row, percent) RET 8
extern "C" void __fastcall kotor_fastcall_005666e0(void* self, void*, U row, I percent) {
    I base = 0;                                                                              // 005666FD [esp] = 0
    void* table = rp(rp(kotor_cell_rules_007a3a28, 0xb8), 0xec);                            // 005666F8..0056670C
    if (table == nullptr) return;                                                            // 00566712..00566717
    {
        ExoString column("XP");                                                              // 0056671D..00566726 [007451D4]
        (void)c2da_current_get_int_00413660(table, nullptr, row, &column, &base);           // 0056672B..00566744
    }                                                                                        // 00566749..00566755
    const I product = static_cast<I>(static_cast<U>(base) * static_cast<U>(percent));        // 0056675A..0056675E imul
    const I xp = static_cast<I>(ftol(ceil(static_cast<double>(product) * static_cast<double>(0.01f)))); // fmul [0073DFEC]
    if (xp <= 0) return;                                                                     // 00566786..00566788
    recovered_party_distribute_experience_005653a0(self, nullptr, static_cast<U>(xp), 1);    // 0056678A..0056678F
    kotor_fastcall_0062eeb0(in_game_gui(), nullptr, 2, static_cast<U>(xp));                 // 00566794..005667A7
}

// 0059F3C0 CSWSDialog::GiveDialogPlotXP(plot index, float percent) RET 8
extern "C" void __fastcall kotor_fastcall_0059f3c0(void*, void*, I plot, float percent) {
    if (plot == -1) return;                                                                  // 0059F3C5..0059F3C8
    if (!(percent > 0.0f)) return;                                                           // 0059F3CA..0059F3D9 fcomp 0; test ah,41h
    if (party_table() == nullptr) return;                                                    // 0059F3DB..0059F3EA
    const U xp = ftol(ceil(static_cast<double>(percent) * static_cast<double>(100.0f)));     // 0059F3EC..0059F40E fmul [00741838]
    kotor_fastcall_005666e0(party_table(), nullptr, static_cast<U>(plot), static_cast<I>(xp)); // 0059F413..0059F41E
}

// 00578A20 CSWSMessage::GetLocStringServerTime(player, CExoLocString*, CExoString* out, float* seconds, BYTE gender)
// static cdecl (caller pops 14h). Quirk: the gender dword handed to CTlkTable::Fetch has only its low byte written by
// CExoLocString::GetString(index) (upper bytes are stale stack in the original; 0 here).
extern "C" U __cdecl kotor_cdecl_00578a20(U player, void* loc, void* out, float* seconds, U gender) {
    ExoString s1;                                                                            // 00578A39..00578A3D
    ExoString s2;                                                                            // 00578A42..00578A4E
    struct TlkResult {                                                                       // STR_RES-like {text, sound, length}
        ExoString text; ResRef sound; float length;
        TlkResult() { resref_default(&sound); }                                              // 00578A53..00578A6A
    } res;
    (void)frame_LogAssign_005e5140(out, nullptr, "");                                        // 00578A6F..00578A7F
    *reinterpret_cast<U*>(seconds) = 0;                                                      // 00578A84..00578A88
    if (server() == nullptr) return 0;                                                       // 00578A8E..00578A99
    I language = static_cast<I>(kotor_fastcall_004ae810(server(), nullptr, player));         // 00578ADA..00578AE7
    const U module_language = kotor_fastcall_004ae820(server(), nullptr);                    // 00578AEB..00578AF3
    if (kotor_fastcall_005ea240(loc, nullptr, static_cast<U>(language), out, gender) != 0) return 1;  // 00578AF8..00578B12
    if (language != 0 && kotor_fastcall_005ea240(loc, nullptr, 0, out, gender) != 0) return 1;       // 00578B14..00578B29
    if (module_language != static_cast<U>(language) && module_language != 0 &&
        kotor_fastcall_005ea240(loc, nullptr, module_language, out, gender) != 0) return 1;           // 00578B2B..00578B41
    U gender_slot = 0;                                                                       // [esp+14h]
    if (kotor_fastcall_005e9eb0(loc, nullptr, 0, &language, out, reinterpret_cast<U8*>(&gender_slot)) != 0) return 1; // 00578B43..00578B59
    const I strref = rdi(loc, 4);                                                            // 00578B5B
    if (strref == -1) return 1;                                                              // 00578B5E..00578B61
    (void)tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, static_cast<U>(strref), &res, gender_slot); // 00578B63..00578B74
    str_assign(out, &res.text);                                                              // 00578B79..00578B80
    *seconds = static_cast<float>(static_cast<double>(res.length) + static_cast<double>(*seconds)); // 00578B85..00578B8F
    return 1;
}

// 0059FF50 CSWSDialog::SetDialogDelay(object, CExoLocString* text, min delay, has links, flag) RET 14h -> float
extern "C" float __fastcall kotor_fastcall_0059ff50(void* d, void*, void* object, void* text, U min_delay, U has_links,
                                                    U a5) {
    ExoString s;                                                                             // 0059FF6B..0059FF6F
    float delay = 0.0f;                                                                      // 0059FF77 [esp+4] = 0
    if (rd(rp(d, 0x64), 0) == 0 && has_links != 0) {                                        // 0059FF74..0059FF92
        (void)kotor_cdecl_00578a20(0xffffffffu, text, &s, &delay, rd8(d, 0x78));             // 0059FF94..0059FFAB
        if (delay < 0.15f) {                                                                 // 0059FFB0..0059FFC2 fcomp [0074A604]; test ah,5
            const I len = static_cast<I>(async_string_length_005e5790(&s, nullptr));        // 0059FFC4..0059FFCD
            delay = static_cast<float>(static_cast<double>(len) * static_cast<double>(0.1125f)); // fmul [0074A600]
        }
    }
    if (rd(in_game_gui(), 0xc0) == 0 && a5 == 0) {                                          // 0059FFDF..0059FFFE
        if (delay < 0.01f) delay = bitsf(0x3c23d70au);                                       // 005A0000..005A0011 fcomp [0073DFEC]
    }
    if (ftol(static_cast<double>(delay)) < min_delay) {                                      // 005A0019..005A0028 (unsigned jae)
        double t = static_cast<double>(static_cast<I>(min_delay));                           // 005A002C..005A0030 fild
        if (static_cast<I>(min_delay) < 0) t += 4294967296.0;                                // 005A0034..005A0036
        delay = static_cast<float>(t);                                                       // 005A003C
    }
    kotor_fastcall_004cb1a0(object, nullptr, delay);                                         // 005A0040..005A0049
    return delay;                                                                            // 005A004E..005A0067
}

// ===============================================================================================================
// Reply handling
// ===============================================================================================================
// 005A03C0 CSWSDialog::HandleReply(player, object, reply index, force, entry index, skip delay) RET 18h
// Quirks kept: the reply camera-listener lookup (OWNER/PLAYER/GetSpeaker) is computed and discarded (the GUI call is
// passed ServerToClient(+7Ch)); every SetDialogDelay result is popped (fstp st0).
extern "C" U __fastcall kotor_fastcall_005a03c0(void* d, void*, U player, void* object, U reply, U force, U entry,
                                                U skip_delay) {
    void* const capp = client();                                                             // 005A03DE..005A03F0 [esp+10h]
    if (force == 0) {                                                                        // 005A03E6..005A03F4
        if (rdi(d, 0x74) != -1) return 1;                                                    // 005A03F6..005A03FA
        if (entry != rd(d, 0x70)) return 1;                                                  // 005A0400..005A0406
    }
    void* cursor = rp(rp(d, 0x64), 0);                                                       // 005A040C..005A0411
    void* p = cursor ? rp(cursor, 8) : nullptr;                                              // 005A0415..005A041B
    while (p != nullptr) {                                                                   // 005A041E..005A043E
        if (rd(p, 0) == player) break;                                                       // 005A0426..005A042B
        p = unresolved_list_next_005e9b90(rp(d, 0x64), nullptr, &cursor);                    // 005A042D..005A0435
    }
    if (p != nullptr) {
        if (force != 0) {                                                                    // 005A0443..005A0445
            void* o = nullptr;
            if (!get_object(rd(p, 4), &o)) return 1;                                         // 005A044B..005A046F
            if (as_creature(o) != nullptr) {                                                 // 005A0471..005A0480
                recovered_set_activity_004f00a0(as_creature(o), nullptr, 4, 0, 0);          // 005A0482..005A048F
                return 1;
            }
            (void)kotor_fastcall_004cb4f0(o, nullptr);                                       // 005A04AD StopDialog
            return 1;
        }
        wr(d, 0x7c, rd(p, 4));                                                               // 005A04CB..005A04CE
        wr8(d, 0x78, rd8(p, 8));                                                             // 005A04D1..005A04D4
    }
    if (rd(rp(d, 0x64), 0) != 0 && p == nullptr) return 1;                                   // 005A04D7..005A04E1
    void* const ent = at(rp(d, 8), rd(d, 0x70) * 0xb0);                                      // 005A04E3..005A04E9
    if (reply >= rd(ent, 0xa8)) return 0;                                                    // 005A04EF..005A04F9
    {
        void* o = nullptr;
        if (!get_object(rd(d, 0x7c), &o)) return 0;                                          // 005A04FF..005A0523
    }
    wr(d, 0x74, reply);                                                                      // 005A0535
    const U ri = rd(rp(at(rp(d, 8), rd(d, 0x70) * 0xb0), 0xac), reply * 0x18 + 0x10);       // 005A0529..005A0542
    if (rd(rp(d, 0x64), 0) != 0) {                                                           // 005A0546..005A0551
        cursor = rp(rp(d, 0x64), 0);                                                         // 005A0557
        void* q = cursor ? rp(cursor, 8) : nullptr;                                          // 005A055B..005A0565
        while (q != nullptr) {                                                               // 005A0568..005A06D7
            if (rd(q, 0) != player) {                                                        // 005A0580..005A0585
                const U li = rd(rp(at(rp(d, 8), rd(d, 0x70) * 0xb0), 0xac), rd(d, 0x74) * 0x18 + 0x10);
                CopiedString listener(at(rp(d, 0x10), li * 0xb4 + 0x84));                    // 005A058B..005A05BD
                if (dlg::str_not_equal(&listener, "") != 0) {                                // 005A05C2..005A05DA
                    if (dlg::str_compare_nc(&listener, "OWNER") == 0) {                      // 005A05DC..005A05EC
                        if (dlg::str_compare_nc(&listener, "PLAYER") != 0)                   // 005A05EE..005A05FE
                            (void)kotor_fastcall_004aea40(server(), nullptr);                // 005A0600..005A0609 (discarded)
                        else
                            (void)kotor_fastcall_0059fc30(d, nullptr, object, &listener);    // 005A0610..005A061B (discarded)
                    }
                }
                ResultString text;
                (void)kotor_fastcall_0059f320(d, nullptr, &text, at(rp(d, 0x10), ri * 0xb4 + 8), rd8(d, 0x78)); // 005A0620..005A0635
                const U a7 = rd(d, 0x6c);                                                    // 005A063C
                const U8 g = rd8(d, 0x78);                                                   // 005A0644
                const U speaker = kotor_fastcall_005eda50(capp, nullptr, rd(d, 0x7c));      // 005A063F..005A0653
                (void)kotor_fastcall_0062e100(frame_R4_005ed690(capp, nullptr), nullptr, rd(q, 0), reply, entry, &text,
                                              speaker, g, static_cast<I>(a7));               // 005A0658..005A0670
            }                                                                                // 005A0675..005A068F
            void* rep = at(rp(d, 0x10), ri * 0xb4);                                          // 005A0698..005A06A1
            kotor_fastcall_0059ef00(d, nullptr, at(rep, 0x4c), rd(rep, 0x54), rd(q, 0));     // 005A069B..005A06AB
            rep = at(rp(d, 0x10), ri * 0xb4);
            kotor_fastcall_0059f3c0(d, nullptr, rdi(rep, 0x58), rdf(rep, 0x5c));             // 005A06B0..005A06C1
            q = unresolved_list_next_005e9b90(rp(d, 0x64), nullptr, &cursor);                // 005A06C6..005A06CE
        }
    }
    if (rd(rp(d, 0x64), 0) != 0) wr(d, 0x98, rd(d, 0x7c));                                  // 005A06E1..005A06EC
    if (skip_delay == 0) {                                                                   // 005A06F2..005A06F7
        void* rep = at(rp(d, 0x10), ri * 0xb4);                                              // 005A06FD..005A070C
        const U has_links = rd(rep, 0xa8) != 0 ? 1u : 0u;
        if ((rd8(rep, 0x20) & 2) != 0) {                                                     // 005A0708..005A0711
            (void)kotor_fastcall_0059ff50(d, nullptr, object, at(rep, 8), rd(rep, 0x24), has_links, 1); // 005A07A4..005A07C7
        } else if (rdi(rep, 0x28) != -1) {                                                   // 005A0717..005A071B
            (void)kotor_fastcall_0059ff50(d, nullptr, object, at(rep, 8), 0, has_links, 1);  // 005A0788..005A07C7
        } else if (rd(in_game_gui(), 0xc0) != 0) {                                           // 005A071D..005A0733
            kotor_fastcall_004cb1a0(object, nullptr, 0.0f);                                  // 005A077C..005A0781
        } else {
            const U8 g = rd8(d, 0x78);
            const U language = kotor_fastcall_004ae810(server(), nullptr, rd(d, 0x7c));      // 005A0735..005A075A
            const float est = kotor_fastcall_005ea270(at(rp(d, 0x10), ri * 0xb4 + 8), nullptr, language, g); // 005A075F..005A0767
            const U ms = ftol(static_cast<double>(est));                                     // 005A076C
            (void)kotor_fastcall_0059ff50(d, nullptr, object, at(rp(d, 0x10), ri * 0xb4 + 8), ms, has_links, 1); // 005A0771..005A07C7
        }
        if (rd(in_game_gui(), 0xc0) == 0)                                                    // 005A07C9..005A07DF
            wr(object, 0x5c, rd(rp(d, 0x10), ri * 0xb4 + 0x20));                             // 005A07E1..005A07EB
        else
            wr(object, 0x5c, 0);                                                             // 005A07F0..005A07F3
    }
    (void)kotor_fastcall_0059ed70(d, nullptr, object, at(rp(d, 0x10), ri * 0xb4 + 0x10));    // 005A07FA..005A0810 RunScript
    for (U k = 0; k < rd(rp(d, 0x10), ri * 0xb4 + 0xa8); ++k) {                              // 005A0815..005A08DD
        void* link = at(rp(rp(d, 0x10), ri * 0xb4 + 0xac), k * 0x14);
        void* e = at(rp(d, 8), rd(link, 0x10) * 0xb0);                                       // 005A0830..005A084C
        bool try_script = true;
        if (dlg::str_not_equal(e, "") != 0) {                                                // 005A0847..005A0855
            if (str_nc_s(at(object, 0x18), e) == 0) {                                        // 005A0857..005A087E
                link = at(rp(rp(d, 0x10), ri * 0xb4 + 0xac), k * 0x14);
                e = at(rp(d, 8), rd(link, 0x10) * 0xb0);
                if (kotor_fastcall_0059fc30(d, nullptr, object, e) == nullptr) try_script = false; // 005A0880..005A08A7
            }
        }
        if (try_script) {
            link = at(rp(rp(d, 0x10), ri * 0xb4 + 0xac), k * 0x14);
            if (kotor_fastcall_0059ec90(d, nullptr, object, link) != 0)                      // 005A08A9..005A08C3
                return kotor_fastcall_004cd460(object, nullptr, rdi(link, 0x10));            // 005A0904..005A091D
        }
    }
    wr(d, 0x6c, 1); wr(d, 0x60, 1);                                                          // 005A08E3..005A08EB
    return 0;                                                                                // 005A08EE
}

// ===============================================================================================================
// Journal
// ===============================================================================================================
// 005C5850 CSWSJournal::GetQuestEntryXP(CExoString const& tag) RET 4 / 005C58A0 GetState RET 4
extern "C" I __fastcall kotor_fastcall_005c5850(void* j, void*, void* tag) {
    const I n = rdi(j, 4);
    for (I i = 0; i < n; ++i)                                                                // 005C585A..005C587C
        if (str_nc_s(at(rp(j, 0), static_cast<U>(i) * 0x3c + 0x18), tag)) return rdi(rp(j, 0), static_cast<U>(i) * 0x3c + 0x38);
    return 0;
}
extern "C" I __fastcall kotor_fastcall_005c58a0(void* j, void*, void* tag) {
    const I n = rdi(j, 4);
    for (I i = 0; i < n; ++i)                                                                // 005C58AA..005C58CC
        if (str_nc_s(at(rp(j, 0), static_cast<U>(i) * 0x3c + 0x18), tag)) return rdi(rp(j, 0), static_cast<U>(i) * 0x3c + 0x20);
    return 0;
}

// 005C53E0 SJournalEntry::SJournalEntry()
extern "C" void* __fastcall kotor_fastcall_005c53e0(void* e, void*) {
    (void)exo_locstring_construct_005e9e10(e, nullptr);                                     // 005C53FE
    (void)exo_locstring_construct_005e9e10(at(e, 8), nullptr);                              // 005C5403..005C540E
    wr(e, 0x10, 0xffffffffu); wr(e, 0x14, 0xffffffffu);                                     // 005C5413..005C5421
    (void)kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(static_cast<void*>(at(e, 0x18))), nullptr);
    wr(e, 0x20, 0xffffffffu); wr(e, 0x24, 0xffffffffu);                                     // 005C5430..005C5433
    wr(e, 0x34, 0xffffffffu);                                                                // 005C5439
    wr(e, 0x2c, rd(e, 0x2c) & 0xfffffff8u);                                                  // 005C5429..005C543C (3 bitfields cleared)
    wr16(e, 0x28, 0xffff);                                                                   // 005C543F
    wr(e, 0x30, 0); wr(e, 0x38, 0);                                                          // 005C5445..005C544C
    return e;
}
namespace {
inline void copy_journal_scalars(void* dst, const void* src) {
    wr(dst, 0x20, rd(src, 0x20)); wr(dst, 0x24, rd(src, 0x24)); wr16(dst, 0x28, rd16(src, 0x28));
    wr(dst, 0x2c, (rd(dst, 0x2c) & ~7u) | (rd(src, 0x2c) & 7u));                             // three single-bit copies
    wr(dst, 0x30, rd(src, 0x30)); wr(dst, 0x34, rd(src, 0x34)); wr(dst, 0x38, rd(src, 0x38));
}
}  // namespace
// 005C5470 SJournalEntry::operator=(SJournalEntry const&) RET 4
extern "C" void* __fastcall kotor_fastcall_005c5470(void* dst, void*, const void* src) {
    (void)recovered_localized_assign_005e9e70(dst, nullptr, src);                            // 005C5479
    (void)recovered_localized_assign_005e9e70(at(dst, 8), nullptr, static_cast<const char*>(src) + 8); // 005C547E..005C5485
    wr(dst, 0x10, rd(src, 0x10)); wr(dst, 0x14, rd(src, 0x14));                              // 005C548A..005C549A
    str_assign(at(dst, 0x18), static_cast<const char*>(src) + 0x18);                         // 005C5493..005C549D
    copy_journal_scalars(dst, src);                                                          // 005C54A2..005C54F3
    return dst;
}
// 005C5560 SJournalEntry::SJournalEntry(SJournalEntry const&) RET 4
extern "C" void* __fastcall kotor_fastcall_005c5560(void* dst, void*, const void* src) {
    (void)kotor_fastcall_005e9f80(dst, nullptr, src);                                        // 005C557E..005C5583
    (void)kotor_fastcall_005e9f80(at(dst, 8), nullptr, static_cast<const char*>(src) + 8);   // 005C5588..005C5597
    wr(dst, 0x10, rd(src, 0x10)); wr(dst, 0x14, rd(src, 0x14));                              // 005C559C..005C55B1
    (void)WindowCopy005e5b00(at(dst, 0x18), nullptr, static_cast<const char*>(src) + 0x18);  // 005C55A5..005C55B4
    copy_journal_scalars(dst, src);                                                          // 005C55B9..005C560D
    return dst;
}
// 005C5680 CExoArrayList<SJournalEntry>::Allocate(n) RET 4
extern "C" void __fastcall kotor_fastcall_005c5680(void* list, void*, U n) {
    void* const old = rp(list, 0);                                                           // 005C56A4
    wr(list, 8, n);                                                                          // 005C56AA
    void* const block = live_operator_new(n * 0x3c + 4);                                     // 005C569D..005C56AD
    void* arr = nullptr;
    if (block != nullptr) {                                                                  // 005C56BB..005C56C1
        wr(block, 0, n); arr = at(block, 4);                                                 // 005C56CE..005C56D4
        crt_eh_vector_constructor_006fb55f(arr, 0x3c, static_cast<I>(n), reinterpret_cast<void*>(&kotor_fastcall_005c53e0),
                                           reinterpret_cast<void*>(&kotor_fastcall_005c5500)); // 005C56C3..005C56D6
    }
    wp(list, 0, arr);                                                                        // 005C56DE
    for (I i = 0; i < rdi(list, 4); ++i)                                                     // 005C56E0..005C5706
        (void)kotor_fastcall_005c5470(at(rp(list, 0), static_cast<U>(i) * 0x3c), nullptr, static_cast<char*>(old) + i * 0x3c);
    if (old != nullptr) {                                                                    // 005C5708..005C570A
        c2da_destroy_array_006fa852(old, 0x3c, rdi(static_cast<char*>(old) - 4, 0),
                                    reinterpret_cast<void*>(&kotor_fastcall_005c5500));      // 005C570C..005C571B
        live_free(static_cast<char*>(old) - 4);                                              // 005C5720..005C5721
    }
}
// 00527490 CExoArrayList<CSWSPlayerJournalQuestUpdates>::Allocate(n) RET 4 (element {WORD flags; CExoString tag})
extern "C" void __fastcall kotor_fastcall_00527490(void* list, void*, U n) {
    void* const old = rp(list, 0);                                                           // 005274AF
    wr(list, 8, n);                                                                          // 005274BC
    void* const block = live_operator_new(n * 0xc + 4);                                      // 005274B1..005274BF
    void* arr = nullptr;
    if (block != nullptr) {                                                                  // 005274CB..005274D3
        wr(block, 0, n); arr = at(block, 4);
        crt_eh_vector_constructor_006fb55f(arr, 0xc, static_cast<I>(n), reinterpret_cast<void*>(&tlk_token_construct_0041d910),
                                           reinterpret_cast<void*>(&kotor_fastcall_0059ea20)); // 005274D5..005274E8
    }
    wp(list, 0, arr);                                                                        // 005274F0
    for (I i = 0; i < rdi(list, 4); ++i) {                                                   // 005274F2..00527523
        char* dst = static_cast<char*>(rp(list, 0)) + i * 0xc;
        wr16(dst, 0, rd16(old, static_cast<U>(i) * 0xc));                                    // 00527504..00527512
        str_assign(dst + 4, static_cast<char*>(old) + i * 0xc + 4);                          // 0052750A..00527515
    }
    if (old != nullptr) {                                                                    // 00527525..00527527
        c2da_destroy_array_006fa852(old, 0xc, rdi(static_cast<char*>(old) - 4, 0),
                                    reinterpret_cast<void*>(&kotor_fastcall_0059ea20));      // 00527529..00527538
        live_free(static_cast<char*>(old) - 4);                                              // 0052753D..0052753E
    }
}
namespace {
// 005C5920 CExoArrayList<SJournalEntry>::AddAndDispose(SJournalEntry by value) RET 3Ch. ABI adaptation: the by-value
// argument is passed by address (file-local helper; only this file calls it); the callee still destroys it.
void journal_add_and_dispose_005c5920(void* list, void* entry) {
    if (rd(list, 4) == rd(list, 8)) {                                                        // 005C5938..005C5946
        const U a = rd(list, 8);
        kotor_fastcall_005c5680(list, nullptr, a != 0 ? a + a : 0x10);                       // 005C5948..005C5958
    }
    const U c = rd(list, 4);
    void* slot = at(rp(list, 0), c * 0x3c);                                                  // 005C595D..005C5967
    wr(list, 4, c + 1);                                                                      // 005C5969..005C596A
    (void)kotor_fastcall_005c5470(slot, nullptr, entry);                                     // 005C596D..005C5972
    kotor_fastcall_005c5500(entry, nullptr);                                                 // 005C5977..005C5983
}
// 005C59A0 CExoArrayList<CSWSPlayerJournalQuestUpdates>::Add(by value {WORD, CExoString}) RET 0Ch. Same adaptation.
void quest_updates_add_005c59a0(void* list, U16 flags, void* tag_by_value) {
    if (rd(list, 4) == rd(list, 8)) {                                                        // 005C59B8..005C59C6
        const U a = rd(list, 8);
        kotor_fastcall_00527490(list, nullptr, a != 0 ? a + a : 0x10);                       // 005C59C8..005C59D8
    }
    const U c = rd(list, 4);
    char* slot = static_cast<char*>(rp(list, 0)) + c * 0xc;                                  // 005C59DD..005C59E6
    wr(list, 4, c + 1);                                                                      // 005C59E5..005C59E9
    wr16(slot, 0, flags);                                                                    // 005C59EC..005C59F5
    str_assign(slot + 4, tag_by_value);                                                      // 005C59F1..005C59FC
    frame_LogStringDtor_005e5c20(tag_by_value, nullptr);                                     // 005C5A01..005C5A0D
}
// CSWSJournal::SetDate 005C63F0 / SetTime 005C6650 share one body (field +10h/+14h, update flag bit 4/8)
void journal_set_date_time(void* j, void* tag, U value, U flag, U field, U16 bit) {
    JournalEntry J;                                                                          // 005C640E..005C6412
    ExoString lowered;                                                                       // 005C6419..005C6421 [esp+1Ch]
    U found = 0;                                                                             // edi
    const I n = rdi(j, 4);
    I i = 0;
    for (; i < n; ++i)                                                                       // 005C642B..005C646B
        if (str_nc_s(at(rp(j, 0), static_cast<U>(i) * 0x3c + 0x18), tag)) break;
    if (i < n) {
        wr(rp(j, 0), static_cast<U>(i) * 0x3c + field, value);                               // 005C65EE..005C65F8
        found = 1;                                                                           // 005C65FC
    } else {
        { ResultString low; (void)lower_construct_005e6080(tag, nullptr, &low); str_assign(at(&J, 0x18), &low); } // 005C646D..005C6493
        wr(&J, field, value);                                                                // 005C6498..005C64AD
        RawJournalEntry tmp;
        (void)kotor_fastcall_005c5560(&tmp, nullptr, &J);                                    // 005C649C..005C64B1
        journal_add_and_dispose_005c5920(j, &tmp);                                           // 005C64B6..005C64B8
    }
    void* player = kotor_fastcall_004ae910(server(), nullptr, kotor_fastcall_004aea40(server(), nullptr)); // 005C64BD..005C64CF
    if (player == nullptr) return;                                                           // 005C64D4..005C64D6
    void* u = rp(player, 0x4c);                                                              // 005C64DC
    if (rd(u, 0x10) != 0) return;                                                            // 005C64DF..005C64E4
    void* ul = at(u, 4);                                                                     // 005C64EA
    if (ul == nullptr) return;                                                               // 005C64ED..005C64EF
    const I cnt = rdi(ul, 4);                                                                // 005C64F5..005C64FC
    I k = 0;
    for (; k < cnt; ++k)                                                                     // 005C64F8..005C6527
        if (str_nc_s(at(rp(ul, 0), static_cast<U>(k) * 0xc + 4), tag)) break;
    if (k < cnt) {
        char* el = static_cast<char*>(rp(ul, 0)) + k * 0xc;                                  // 005C6606..005C6609
        if (found == 0) {
            wr16(el, 0, 0x8000);                                                             // 005C6610..005C6613
        } else {
            el[1] = static_cast<char>(static_cast<U8>(el[1]) & 0xbf);                        // 005C661B..005C661E
            el[0] = static_cast<char>(static_cast<U8>(el[0]) | static_cast<U8>(bit));        // 005C6626..005C662B
        }
        if (flag != 0) el[1] = static_cast<char>(static_cast<U8>(el[1]) | 1);                // 005C662E..005C663F
        return;
    }
    { ResultString low; (void)lower_construct_005e6080(tag, nullptr, &low); str_assign(&lowered, &low); } // 005C6529..005C654F
    U16 w = found != 0 ? bit : 0x8000;                                                       // 005C6554..005C6562
    if (flag != 0) w = static_cast<U16>(w | 0x100);                                          // 005C6568..005C6576
    struct { void* data; U length; } arg;                                                    // by-value CExoString copy
    (void)WindowCopy005e5b00(&arg, nullptr, &lowered);                                       // 005C657B..005C658F
    quest_updates_add_005c59a0(ul, w, &arg);                                                 // 005C6594..005C6596
}
}  // namespace
extern "C" void __fastcall kotor_fastcall_005c63f0(void* j, void*, void* tag, U date, U flag) {
    journal_set_date_time(j, tag, date, flag, 0x10, 4);
}
extern "C" void __fastcall kotor_fastcall_005c6650(void* j, void*, void* tag, U time, U flag) {
    journal_set_date_time(j, tag, time, flag, 0x14, 8);
}

// 0056A020 CSWSMessage::SendServerToPlayerJournalAddQuest(13 args) RET 34h
extern "C" void __fastcall kotor_fastcall_0056a020(void* msg, void*, void* player, void* tag, U state, U priority,
    U picture, U end, U date, U time, void* name, void* text, U plot, U planet, U xp) {
    void* data = nullptr;                                                                    // [esp+8] (push ecx slot)
    U size = async_string_length_005e5790(tag, nullptr) + 0x16;                              // 0056A029..0056A032
    kotor_fastcall_004d68a0(msg, nullptr, size, 0xffffffffu, 1);                             // 0056A030..0056A03E
    kotor_fastcall_005785c0(msg, nullptr, name, 0);                                          // 0056A043..0056A04C
    kotor_fastcall_005785c0(msg, nullptr, text, 0);                                          // 0056A051..0056A05A
    kotor_fastcall_004d6340(msg, nullptr, date, 0x20);                                       // 0056A05F..0056A068
    kotor_fastcall_004d6340(msg, nullptr, time, 0x20);                                       // 0056A06D..0056A076
    kotor_fastcall_004d6960(msg, nullptr, tag, 0x20);                                        // 0056A07B..0056A080
    kotor_fastcall_004d6340(msg, nullptr, state, 0x20);                                      // 0056A085..0056A08E
    kotor_fastcall_004d6340(msg, nullptr, priority, 0x20);                                   // 0056A093..0056A09C
    kotor_fastcall_004d6300(msg, nullptr, picture, 0x10);                                    // 0056A0A1..0056A0AA
    kotor_fastcall_004d68e0(msg, nullptr, end);                                              // 0056A0AF..0056A0B6
    kotor_fastcall_004d6340(msg, nullptr, plot, 0x20);                                       // 0056A0BB..0056A0C4
    kotor_fastcall_004d6340(msg, nullptr, planet, 0x20);                                     // 0056A0C9..0056A0D2
    kotor_fastcall_004d6340(msg, nullptr, xp, 0x20);                                         // 0056A0D7..0056A0E0
    if (kotor_fastcall_004d6440(msg, nullptr, &data, &size) == 0) return;                    // 0056A0E5..0056A0F8
    (void)kotor_fastcall_00567e60(msg, nullptr, rd(player, 4), 0x1c, 6, data, size);         // 0056A100..0056A118
}

// 0056A4E0 CSWSMessage::SendServerToPlayerJournalUpdated(player, a, end, SJournalEntry*) RET 10h
extern "C" void __fastcall kotor_fastcall_0056a4e0(void* msg, void*, void* player, U a, U end, void* entry) {
    void* data = nullptr;                                                                    // [esp+8]
    U size = 8;                                                                              // 0056A4EC [esp+4] = 8
    kotor_fastcall_004d68a0(msg, nullptr, 8, 0xffffffffu, 1);                                // 0056A4E4..0056A4F4
    kotor_fastcall_005785c0(msg, nullptr, entry, 0);                                         // 0056A4F9..0056A502
    kotor_fastcall_004d68e0(msg, nullptr, a);                                                // 0056A507..0056A50E
    kotor_fastcall_004d68e0(msg, nullptr, end);                                              // 0056A513..0056A51A
    if (kotor_fastcall_004d6440(msg, nullptr, &data, &size) == 0) return;                    // 0056A51F..0056A532
    (void)kotor_fastcall_00567e60(msg, nullptr, rd(player, 4), 0x1c, 0xc, data, size);       // 0056A53B..0056A553
}

// 005C5A40 CSWSJournal::SetState(CExoString const& tag, state, flag) RET 0Ch
// Quirk: for a quest not yet in the journal with no pending update record, the update flags word at [esp+54h] is
// uninitialised stack in the original (only bit 8000h is OR-ed in); it starts at 0 here.
extern "C" void __fastcall kotor_fastcall_005c5a40(void* j, void*, void* tag, U state, U flag) {
    JournalEntry J;                                                                          // 005C5A66..005C5A6A [esp+6Ch]
    U found = 0;                                                                             // [esp+3Ch]
    U okread = 1;                                                                            // [esp+1Ch]
    void* const msg = recovered_server_sws_message_004aedb0(server(), nullptr);              // 005C5A7F..005C5A99 [esp+44h]
    ExoString A;                                                                             // [esp+58h]
    ExoLocString L;                                                                          // [esp+48h]
    ExoString B;                                                                             // [esp+60h]
    U flags = flag != 0 ? 0x100u : 0u;                                                       // 005C5AC4..005C5ADF [esp+10h]
    U not_matched = 1;                                                                       // [esp+50h]
    U16 update_word = 0;                                                                     // [esp+54h] (see quirk)
    U idx = 0;                                                                               // [esp+40h]
    {
        const I n = rdi(j, 4);
        I i = 0;
        for (; i < n; ++i)                                                                   // 005C5AE7..005C5B1A
            if (str_nc_s(at(rp(j, 0), static_cast<U>(i) * 0x3c + 0x18), tag)) break;
        if (i < n) {
            idx = static_cast<U>(i); found = 1;                                              // 005C5BD3..005C5BD7
        } else {
            { ResultString low; (void)lower_construct_005e6080(tag, nullptr, &low); str_assign(at(&J, 0x18), &low); } // 005C5B1C..005C5B4A
            wr(&J, 0x20, state);                                                             // 005C5B4F..005C5B63
            RawJournalEntry tmp;
            (void)kotor_fastcall_005c5560(&tmp, nullptr, &J);                                // 005C5B52..005C5B6A
            journal_add_and_dispose_005c5920(j, &tmp);                                       // 005C5B6F..005C5B71
            idx = rd(j, 4) - 1;                                                              // 005C5B76..005C5B7A
            flags |= 0x8000;                                                                 // 005C5B7E
        }
    }
    void* const mem = live_operator_new(0xa0);                                               // 005C5B86..005C5B8B
    void* gff = nullptr;
    if (mem != nullptr) {                                                                    // 005C5B99..005C5BA3
        ResRef r; resref_from(&r, "Global");                                                 // 005C5BA5..005C5BB1 [007486D0]
        gff = gui_load_gff_construct_00410630(mem, nullptr, 0x808, "JRL ", &r);              // 005C5BB6..005C5BCA
    }
    if (rd(gff, 0x94) == 0) { vcall<void>(gff, 0, 1u); return; }                             // 005C5BE3..005C5C4F
    auto E = [&]() { return at(rp(j, 0), idx * 0x3c); };
    U top = 0;
    (void)kotor_fastcall_00411240(gff, nullptr, &top);                                       // 005C5C54..005C5C59
    GffList cats{};
    (void)gff_list_004118c0(gff, nullptr, &cats, &top, "Categories");                        // 005C5C5E..005C5C72
    const I ncat = static_cast<I>(gff_list_count_00411940(gff, nullptr, &cats));             // 005C5C77..005C5C8A
    for (I c = 0; c < ncat; ++c) {                                                           // 005C5C86..005C5D45
        U cs = 0;
        (void)gff_list_element_00411990(gff, nullptr, &cs, &cats, static_cast<U>(c));        // 005C5C94..005C5CA4
        {
            ExoString fallback("");                                                          // 005C5CA9..005C5CB2
            ResultString out;
            (void)gui_load_gff_string_00411ec0(gff, nullptr, &out, &cs, "Tag", &okread, &fallback); // 005C5CB7..005C5CDA
            str_assign(&B, &out);                                                            // 005C5CDF..005C5CEC
        }                                                                                    // 005C5CF1..005C5D0E
        if (okread == 0 || dlg::str_not_equal(&B, "") == 0 || str_nc_s(tag, &B) == 0) continue; // 005C5D13..005C5D3C
        // ---- matched category (005C5D50) ----
        not_matched = 0;                                                                     // 005C5D54
        int diff;
        {
            ExoLocString fallback;                                                           // 005C5D50..005C5D5C
            ResultLocString out;
            (void)recovered_gff_localized_00411fd0(gff, nullptr, &out, &cs, "Name", &okread, &fallback); // 005C5D61..005C5D80
            void* e = E();
            (void)recovered_localized_assign_005e9e70(&L, nullptr, &out);                    // 005C5D85..005C5D9E
            diff = recovered_localized_different_005ea030(&L, nullptr, e);                   // 005C5DA3..005C5DA5
        }                                                                                    // 005C5DAA..005C5DCB
        if (diff != 0) { flags |= 1; (void)recovered_localized_assign_005e9e70(E(), nullptr, &L); } // 005C5DD0..005C5DEC
        const U priority = gui_load_gff_dword_00411c20(gff, nullptr, &cs, "Priority", &okread, 0); // 005C5DF1..005C5E04
        if (priority != rd(E(), 0x24)) { flags |= 0x20; wr(E(), 0x24, priority); }          // 005C5E09..005C5E1C
        if (rd(E(), 0x20) != state) { flags |= 0x10; wr(E(), 0x20, state); }                // 005C5E1E..005C5E30
        const U16 picture = recovered_gff_word_00411b40(gff, nullptr, &cs, "Picture", &okread, 0); // 005C5E32..005C5E45
        if (picture != rd16(E(), 0x28)) { flags |= 0x40; wr16(E(), 0x28, picture); }        // 005C5E4A..005C5E5C
        const I plot = gui_load_gff_int_00411c90(gff, nullptr, &cs, "PlotIndex", &okread, 0); // 005C5E5F..005C5E72
        if (plot != rdi(E(), 0x30)) { flags |= 0x200; wr(E(), 0x30, static_cast<U>(plot)); } // 005C5E77..005C5E89
        const I planet = gui_load_gff_int_00411c90(gff, nullptr, &cs, "PlanetID", &okread, 0); // 005C5E8B..005C5E9E
        if (planet != rdi(E(), 0x34)) { flags |= 0x400; wr(E(), 0x34, static_cast<U>(planet)); } // 005C5EA3..005C5EB5
        GffList entries{};
        (void)gff_list_004118c0(gff, nullptr, &entries, &cs, "EntryList");                   // 005C5EB7..005C5ECB
        const I ne = static_cast<I>(gff_list_count_00411940(gff, nullptr, &entries));        // 005C5ED0..005C5EE1
        I m = 0;
        U es = 0;
        for (; m < ne; ++m) {                                                                // 005C5EE5..005C5F38
            (void)gff_list_element_00411990(gff, nullptr, &es, &entries, static_cast<U>(m)); // 005C5EF3..005C5F07
            if (gui_load_gff_dword_00411c20(gff, nullptr, &es, "ID", &okread, 0) == state) break; // 005C5F0C..005C5F27
        }
        if (m < ne) {
            {
                ExoLocString fallback;                                                       // 005C5F3F..005C5F43
                ResultLocString out;
                (void)recovered_gff_localized_00411fd0(gff, nullptr, &out, &es, "Text", &okread, &fallback); // 005C5F48..005C5F67
                void* e8 = at(E(), 8);
                (void)recovered_localized_assign_005e9e70(&L, nullptr, &out);                // 005C5F6C..005C5F80
                diff = recovered_localized_different_005ea030(&L, nullptr, e8);              // 005C5F85..005C5F87
            }                                                                                // 005C5F8C..005C5FAD
            if (diff != 0) { flags |= 2; (void)recovered_localized_assign_005e9e70(at(E(), 8), nullptr, &L); } // 005C5FB2..005C5FD0
            const float pct = gui_load_gff_float_00411d00(gff, nullptr, &es, "XP_Percentage", &okread, 0.0f); // 005C5FD5..005C5FED
            if (okread != 0) {                                                               // 005C5FF1..005C5FF7
                I base = 0;                                                                  // 005C6002
                void* table = rp(rp(kotor_cell_rules_007a3a28, 0xb8), 0xec);                // 005C5FFD..005C6016
                {
                    ExoString column("XP");                                                  // 005C601A..005C6023
                    (void)c2da_current_get_int_00413660(table, nullptr, rd(E(), 0x30), &column, &base); // 005C6028..005C6045
                }                                                                            // 005C604A..005C6056
                const U x = ftol(static_cast<double>(base) * static_cast<double>(pct) + static_cast<double>(0.5f)); // 005C605B..005C6069 fadd [0073E9AC]
                if (x != rd(E(), 0x38)) { flags |= 0x800; wr(E(), 0x38, x); }               // 005C606E..005C6084
            }
            const U end = recovered_gff_word_00411b40(gff, nullptr, &es, "End", &okread, 0); // 005C6086..005C60A0
            const U v = rd(E(), 0x2c);
            if (end != (v & 1)) wr(E(), 0x2c, v ^ ((v ^ end) & 1));                           // 005C60A3..005C60BB
            flags |= 0x80;                                                                   // 005C60BD
        }
        // 005C60C5
        {
            const U v = rd(E(), 0x2c);
            const U c4 = flag == 0 ? 4u : 0u;
            wr(E(), 0x2c, v ^ ((c4 ^ v) & 4));                                               // 005C60C5..005C60E3
        }
        void* const player = kotor_fastcall_004ae910(server(), nullptr, kotor_fastcall_004aea40(server(), nullptr)); // 005C60E5..005C60FC
        if (player != nullptr) {                                                             // 005C6101..005C6107
            void* const u = rp(player, 0x4c);                                                // 005C610D
            if (rd(u, 0) != 0) {                                                             // 005C6110..005C6116
                ResultString low; (void)lower_construct_005e6080(tag, nullptr, &low);        // 005C6118..005C6120
                void* e = E();
                kotor_fastcall_0056a020(msg, nullptr, player, &low, state, rd(e, 0x24), rd16(e, 0x28), rd(e, 0x2c) & 1,
                                        rd(e, 0x10), rd(e, 0x14), e, at(e, 8), rd(e, 0x30), rd(e, 0x34), rd(e, 0x38)); // 005C6125..005C616D
            } else if (rd(u, 0x10) == 0) {                                                   // 005C6188..005C618B
                void* const ul = at(u, 4);                                                   // 005C6191..005C619A
                if (ul != nullptr) {
                    const I cnt = rdi(ul, 4);                                                // 005C61A0..005C61A9
                    I k = 0;
                    for (; k < cnt; ++k)                                                     // 005C61AF..005C61F1
                        if (str_nc_s(at(rp(ul, 0), static_cast<U>(k) * 0xc + 4), tag)) break;
                    if (k < cnt) {
                        char* el = static_cast<char*>(rp(ul, 0)) + k * 0xc;                  // 005C6202..005C6224
                        if (found == 0) {
                            wr16(el, 0, 0x8000);                                             // 005C6212..005C6217
                        } else {
                            el[1] = static_cast<char>(static_cast<U8>(el[1]) & 0xbf);        // 005C6227
                            wr16(el, 0, static_cast<U16>(rd16(el, 0) | static_cast<U16>(flags))); // 005C6230..005C6237
                        }
                    } else {
                        if (found == 0) update_word = static_cast<U16>(update_word | 0x8000); // 005C61F3..005C61FB
                        else update_word = static_cast<U16>(flags);                          // 005C623C..005C6241
                        { ResultString low; (void)lower_construct_005e6080(tag, nullptr, &low); str_assign(&A, &low); } // 005C6246..005C6271
                        struct { void* data; U length; } arg;
                        (void)WindowCopy005e5b00(&arg, nullptr, &A);                         // 005C6276..005C628F
                        quest_updates_add_005c59a0(ul, update_word, &arg);                   // 005C6294..005C6298
                    }
                }
            }
            if (flag == 0)                                                                   // 005C629D..005C62A2
                kotor_fastcall_0056a4e0(msg, nullptr, player, 1, rd(E(), 0x2c) & 1, E());    // 005C62A4..005C62BC
        }
        break;                                                                               // -> 005C62C1
    }
    vcall<void>(gff, 0, 1u);                                                                 // 005C62C1..005C62C7
    if (not_matched != 0) {                                                                  // 005C62C9..005C62CF
        void* const player = kotor_fastcall_004ae910(server(), nullptr, kotor_fastcall_004aea40(server(), nullptr)); // 005C62D5..005C62E8
        if (player != nullptr && rp(player, 0x4c) != nullptr && rd(rp(player, 0x4c), 0) != 0) { // 005C62EF..005C62FD
            ResultString low; (void)lower_construct_005e6080(tag, nullptr, &low);            // 005C62FF..005C6307
            void* e = E();
            kotor_fastcall_0056a020(msg, nullptr, player, &low, state, rd(e, 0x24), rd16(e, 0x28), rd(e, 0x2c) & 1,
                                    rd(e, 0x10), rd(e, 0x14), e, at(e, 8), rd(e, 0x30), rd(e, 0x34), rd(e, 0x38)); // 005C630C..005C6354
        }
    }
}

// 0059EF00 CSWSDialog::AddJournalEntry(CExoString const& quest, entry, player id) RET 0Ch
// Quirk kept: GetFaction is called on the (possibly null) controlled creature.
extern "C" void __fastcall kotor_fastcall_0059ef00(void*, void*, void* quest, U entry, U player_id) {
    if (frame_LogIsEmpty_005e58a0(quest, nullptr) != 0) return;                              // 0059EF08..0059EF11
    void* c = kotor_fastcall_004ae920(server(), nullptr, player_id, 0);                      // 0059EF17..0059EF29
    if (c == nullptr || vcall<void*>(c, 4) == nullptr) return;                               // 0059EF30..0059EF41
    void* o = kotor_fastcall_00561080(vcall<void*>(c, 4), nullptr);                          // 0059EF47..0059EF50
    if (o == nullptr || as_creature(o) == nullptr) return;                                   // 0059EF55..0059EF68
    void* const me = as_creature(o);                                                         // 0059EF6E..0059EF75
    U day = 0, time = 0;                                                                     // [esp+1Ch] (arg slot) / [esp+10h]
    frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server(), nullptr), nullptr, &day, &time); // 0059EF77..0059EF90
    auto apply = [&](void* cre, bool distribute) {
        if (kotor_fastcall_005c58a0(kotor_fastcall_004edb00(cre, nullptr), nullptr, quest) >= static_cast<I>(entry)) return; // signed jge
        kotor_fastcall_005c63f0(kotor_fastcall_004edb00(cre, nullptr), nullptr, quest, day, 0);
        kotor_fastcall_005c6650(kotor_fastcall_004edb00(cre, nullptr), nullptr, quest, time, 0);
        kotor_fastcall_005c5a40(kotor_fastcall_004edb00(cre, nullptr), nullptr, quest, entry, 0);
        const I xp = kotor_fastcall_005c5850(kotor_fastcall_004edb00(cre, nullptr), nullptr, quest);
        if (xp == 0) return;
        if (distribute && party_table() != nullptr)
            recovered_party_distribute_experience_005653a0(party_table(), nullptr, static_cast<U>(xp), 1);
        kotor_fastcall_0062eeb0(in_game_gui(), nullptr, 2, static_cast<U>(xp));
    };
    if (me != nullptr) apply(me, true);                                                      // 0059EF95..0059F047
    void* faction = combat_reuse_faction_00513fc0(me, nullptr);                              // 0059F04C..0059F04E
    if (faction == nullptr) return;                                                          // 0059F053..0059F055
    const I count = rdi(faction, 4);                                                         // 0059F05B [esp+14h]
    void* const members = rp(faction, 0);                                                    // 0059F05E [esp+24h]
    if (members == nullptr || count <= 0) return;                                            // 0059F060..0059F074
    for (I i = 0; i < count; ++i) {                                                          // 0059F07C..0059F12D
        void* cre = kotor_fastcall_004ae770(server(), nullptr, rd(members, static_cast<U>(i) * 4));
        if (cre == nullptr || cre == me) continue;                                           // 0059F092..0059F09E
        apply(cre, false);                                                                   // 0059F0A4..0059F121
    }
}

// ===============================================================================================================
// Unequip / re-equip for dialog (lists +114h items, +118h head items; nodes hold a heap DWORD client creature id)
// ===============================================================================================================
namespace {
void* client_creature_of_server_id(U id) {
    void* c = kotor_fastcall_005ed580(client(), nullptr, kotor_fastcall_005eda50(client(), nullptr, id));
    if (c == nullptr || vcall<void*>(c, 0x2c) == nullptr) return nullptr;
    return vcall<void*>(c, 0x2c);
}
bool listed(void* d, U list_off, U id) {
    void* pos = rp(rp(d, list_off), 0);
    while (pos != nullptr) {
        void* data = unresolved_list_at_pos_005e9b60(rp(d, list_off), nullptr, pos);
        if (rd(data, 0) == id) return true;
        (void)unresolved_list_next_005e9b90(rp(d, list_off), nullptr, &pos);
    }
    return false;
}
void reequip(void* d, U list_off, void (__fastcall* show)(void*, void*)) {
    void* pos = rp(rp(d, list_off), 0);
    while (pos != nullptr) {
        void* data = unresolved_list_at_pos_005e9b60(rp(d, list_off), nullptr, pos);
        if (data != nullptr) {
            void* c = kotor_fastcall_005ed580(client(), nullptr, rd(data, 0));
            if (c != nullptr && vcall<void*>(c, 0x2c) != nullptr) show(vcall<void*>(c, 0x2c), nullptr);
            live_free(data);
        }
        (void)unresolved_list_next_005e9b90(rp(d, list_off), nullptr, &pos);
    }
}
}  // namespace
// 005A0BA0 CSWSDialog::UnequipItemsForDialog(server id) RET 4
extern "C" void __fastcall kotor_fastcall_005a0ba0(void* d, void*, U id) {
    if (id == kInvalid) return;                                                              // 005A0BA7..005A0BAF
    void* cc = client_creature_of_server_id(id);                                             // 005A0BB5..005A0BF4
    if (cc == nullptr) return;
    if (listed(d, 0x114, rd(cc, 4))) return;                                                 // 005A0BF6..005A0C39
    void* node = live_operator_new(4);                                                       // 005A0C3B..005A0C3D
    if (node == nullptr) return;                                                             // 005A0C45..005A0C47
    wr(node, 0, rd(cc, 4));                                                                  // 005A0C49..005A0C4C
    (void)c2da_005e9d70(rp(d, 0x114), nullptr, node);                                        // 005A0C4E..005A0C55 AddHead
    ResRef first; resref_default(&first);                                                    // 005A0C5A..005A0C5E
    ResRef second; resref_default(&second);                                                  // 005A0C63..005A0C68
    kotor_fastcall_00614630(cc, nullptr, &second, &first);                                   // 005A0C6D..005A0C70
}
// 005A0C80 CSWSDialog::ReEquipItemsForDialog() (data freed, list nodes kept: original behaviour)
extern "C" void __fastcall kotor_fastcall_005a0c80(void* d, void*) { reequip(d, 0x114, &kotor_fastcall_00614670); }
// 005A0D10 CSWSDialog::UnequipHItemForDialog(server id) RET 4
extern "C" void __fastcall kotor_fastcall_005a0d10(void* d, void*, U id) {
    if (id == kInvalid) return;                                                              // 005A0D14..005A0D1C
    void* cc = client_creature_of_server_id(id);                                             // 005A0D22..005A0D59
    if (cc == nullptr) return;
    if (listed(d, 0x118, rd(cc, 4))) return;                                                 // 005A0D5B..005A0D99
    void* node = live_operator_new(4);                                                       // 005A0D9B..005A0D9D
    if (node == nullptr) return;
    wr(node, 0, rd(cc, 4));                                                                  // 005A0DA9..005A0DAC
    (void)c2da_005e9d70(rp(d, 0x118), nullptr, node);                                        // 005A0DAE..005A0DB5
    kotor_fastcall_0060e210(cc, nullptr);                                                    // 005A0DBA..005A0DBC
}
// 005A0DD0 CSWSDialog::ReEquipHItemForDialog()
extern "C" void __fastcall kotor_fastcall_005a0dd0(void* d, void*) { reequip(d, 0x118, &kotor_fastcall_0060e230); }

// ===============================================================================================================
// Entry / reply node publication
// ===============================================================================================================
namespace {
// Participant resolution shared by both node senders (005A1465..005A157B / 005A2170..005A2256)
U participant_id(void* d, void* name, void* lookup_object, bool* nondependent_written) {
    if (nondependent_written) *nondependent_written = false;
    if (str_equals(name, "PLAYER")) return rd(rp(d, 0x64), 0) != 0 ? rd(d, 0x7c) : kInvalid;
    if (str_equals(name, "OWNER")) return rd(d, 0x80);
    U slot = 0;
    if (str_equals(name, "NPC1")) slot = 1;
    else if (str_equals(name, "NPC2")) slot = 2;
    if (slot != 0) {
        void* c = frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client(), nullptr), nullptr, slot);
        if (c == nullptr) return kInvalid;
        return kotor_fastcall_004aea30(server(), nullptr, rd(c, 4));
    }
    if (nondependent_written) *nondependent_written = true;
    return kotor_fastcall_004cb5c0(lookup_object, nullptr, name, 0);
}
// "Error: dialogue can't find object '%s'!" formatted and discarded (CExoString passed by value: data, length)
void participant_error(void* name) {
    ExoString message;
    kotor_reconstruction::StartupFormat005e5680(reinterpret_cast<kotor_reconstruction::StartupString*>(&message),
                                                "Error: dialogue can't find object '%s'!", rd(name, 0), rd(name, 4));
}
// Pseudo-random camera angle step: returns the table value at the old index (+A8h + 4*[+9Ch])
U step_camera_angle(void* d) {
    const U old = rd(d, 0x9c);
    const U value = rd(d, 0xa8 + old * 4);
    if (rd(d, 0xa4) != 0) wr(d, 0x9c, rd(d, 0xa0) + old); else wr(d, 0x9c, old - rd(d, 0xa0));
    if (rdi(d, 0x9c) >= 0x13) wr(d, 0x9c, 0);
    if (rdi(d, 0x9c) < 0) wr(d, 0x9c, 0x12);
    return value;
}
inline void* heap(U bytes) { return live_operator_new(bytes); }
inline void release(void* p) { if (p != nullptr) live_free(p); }
}  // namespace

// 005A13D0 CSWSDialog::SendDialogEntryNode(player, owner, speaker, listener, target, gender, object, entry, status*)
// RET 24h; always returns 1. Quirks kept: the early status-10 exits leak the three participant arrays; the
// non-dependant flag array is only written for named participants (others are uninitialised heap, as originally);
// the entry's camera angle is rewritten in place when 0.
extern "C" U __fastcall kotor_fastcall_005a13d0(void* d, void*, U player, U owner, U speaker, U listener, U target,
                                                U gender, void* object, void* entry, U* status) {
    void* const capp = client();                                                             // 005A13E8..005A1401 [esp+30h]
    const I n = rdi(entry, 0x60);                                                            // 005A13F4
    U* ids = nullptr; U16* anims = nullptr; U* nondep = nullptr;                              // [esp+10h] [esp+14h] [esp+20h]
    if (n != 0) {                                                                            // 005A1415
        ids = static_cast<U*>(heap(static_cast<U>(n) * 4));                                  // 005A1417..005A141F
        anims = static_cast<U16*>(heap(static_cast<U>(n) * 2));                              // 005A1424..005A142D
        nondep = static_cast<U*>(heap(static_cast<U>(n) * 4));                               // 005A1432..005A1437
    }
    for (I i = 0; i < n; ++i) {                                                              // 005A1445..005A162B
        void* an = at(rp(entry, 0x64), static_cast<U>(i) * 0xc);
        bool named = false;
        const U id = participant_id(d, an, object, &named);
        if (named) {
            an = at(rp(entry, 0x64), static_cast<U>(i) * 0xc);
            nondep[i] = kotor_fastcall_0062f840(in_game_gui(), nullptr, an) == 0 ? 1u : 0u;  // 005A154F..005A1578
        }
        if (kotor_fastcall_004ae750(server(), nullptr, id) != nullptr) {                     // 005A157B..005A158B
            ids[i] = kotor_fastcall_005eda50(capp, nullptr, id);                             // 005A158D..005A159B
            anims[i] = rd16(rp(entry, 0x64), static_cast<U>(i) * 0xc + 8);                   // 005A159D..005A15AD
        } else {
            participant_error(at(rp(entry, 0x64), static_cast<U>(i) * 0xc));                 // 005A15B3..005A15DA
            ids[i] = kInvalid; anims[i] = 0;                                                 // 005A15EE..005A15FB
        }
    }
    auto present = [&](U id) -> bool {                                                       // 005A1641..005A170F pattern
        void* o = kotor_fastcall_004ae750(server(), nullptr, id);
        if (o == nullptr || vcall<void*>(o, 0x10) == nullptr) return false;
        *status = 0;
        void* c = kotor_fastcall_005ed580(client(), nullptr, kotor_fastcall_005eda50(client(), nullptr, id));
        if (c == nullptr || vcall<void*>(c, 0xc) == nullptr) return false;
        *status = 0;
        return true;
    };
    if (listener != kInvalid && !present(listener)) { *status = 10; return 1; }              // 005A1631..005A16A1, 005A1776
    if (speaker != kInvalid && !present(speaker)) { *status = 10; return 1; }                // 005A16A7..005A170F, 005A1785
    if (target != kInvalid && !present(target)) { *status = 10; return 1; }                  // 005A1715..005A1774
    U day = 0, time = 0;
    frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server(), nullptr), nullptr, &day, &time); // 005A1792..005A17AF
    U elapsed = 0;                                                                           // [esp+74h] (arg slot reused)
    U out_day = 0, out_time = 0;
    (void)recovered_world_time_difference_004ae460(kotor_fastcall_004aede0(server(), nullptr), nullptr, day, time,
                                                   rd(d, 0x124), rd(d, 0x128), &out_day, &out_time); // 005A17B4..005A17E6
    wr(d, 0x124, day); wr(d, 0x128, time);                                                   // 005A17F3..005A1801
    const U remaining = rd(d, 0x120);
    if (remaining > 0) {                                                                     // 005A17EB..005A1807
        if (remaining <= out_time) { wr(d, 0x120, 0); elapsed = 1; }                         // 005A1809..005A1817
        else wr(d, 0x120, remaining - out_time);                                             // 005A1821..005A1823
    }
    bool missing = false;
    for (I i = 0; i < n; ++i) {                                                              // 005A1829..005A18D4
        if (nondep[i] == 0) continue;                                                        // 005A1845..005A1849
        const U cid = ids[i];
        void* o = kotor_fastcall_004ae750(server(), nullptr, kotor_fastcall_004aea30(server(), nullptr, cid)); // 005A184B..005A1866
        if ((o == nullptr || vcall<void*>(o, 0x10) == nullptr) && elapsed == 0) { missing = true; break; } // 005A186B..005A1880
        *status = 0;                                                                         // 005A1882..005A1886
        void* c = kotor_fastcall_005ed580(client(), nullptr, cid);                           // 005A188C..005A1896
        if ((c == nullptr || vcall<void*>(c, 0xc) == nullptr) && elapsed == 0) { missing = true; break; } // 005A189B..005A18B0
        *status = 0;                                                                         // 005A18B2..005A18B6
    }
    if (missing) {
        *status = 10;                                                                        // 005A18DC..005A18EC
        if (rd(d, 0x120) == 0) wr(d, 0x120, 0x5dc);                                          // 005A18F2..005A1900
    }
    if (missing || *status == 10) {                                                          // 005A190A..005A190D
        release(ids); release(anims); release(nondep);                                       // 005A190F..005A1943
        return 1;
    }
    const U angle = step_camera_angle(d);                                                    // 005A194B..005A1997
    if (rd(entry, 0x8c) == 0) wr(entry, 0x8c, rd(d, 0xf4) != 0 ? 2u : angle);                // 005A19A1..005A19BD
    wr(d, 0xf4, 0);                                                                          // 005A19D3
    ResultString text;
    (void)kotor_fastcall_0059f320(d, nullptr, &text, at(entry, 8), gender);                  // 005A19C3..005A19D9
    if (rd(d, 0x40) != 0) {                                                                  // 005A19DE..005A19E9
        kotor_fastcall_005a0ba0(d, nullptr, speaker);                                        // 005A19EB..005A19F2
        kotor_fastcall_005a0ba0(d, nullptr, listener);                                       // 005A19F7..005A19FE
        kotor_fastcall_005a0ba0(d, nullptr, target);                                         // 005A1A03..005A1A0A
        for (I i = 0; i < n; ++i)                                                            // 005A1A0F..005A1A40
            kotor_fastcall_005a0ba0(d, nullptr, kotor_fastcall_004aea30(server(), nullptr, ids[i]));
    }
    if (rd(d, 0x44) != 0) {                                                                  // 005A1A48..005A1A4D
        kotor_fastcall_005a0d10(d, nullptr, speaker);
        kotor_fastcall_005a0d10(d, nullptr, listener);
        kotor_fastcall_005a0d10(d, nullptr, target);
        for (I i = 0; i < n; ++i)                                                            // 005A1A73..005A1AA1
            kotor_fastcall_005a0d10(d, nullptr, kotor_fastcall_004aea30(server(), nullptr, ids[i]));
    }
    U sound = rd(entry, 0x2c);                                                               // 005A1AAA
    if (kotor_cell_disable_sound_00832894 == 1) sound = 1;                                   // 005A1AA3..005A1AAF
    const U8 exists = rd8(entry, 0x80);                                                      // 005A1AB4
    if ((exists & 2) != 0 || (exists & 1) == 0) sound = 1;                                   // 005A1ABA..005A1ACF
    const U a5 = kotor_fastcall_005eda50(capp, nullptr, target);                             // 005A1B4F..005A1B51
    const U a4 = kotor_fastcall_005eda50(capp, nullptr, listener);                           // 005A1B57..005A1B61
    const U a3 = kotor_fastcall_005eda50(capp, nullptr, speaker);                            // 005A1B66..005A1B71
    const U a2 = kotor_fastcall_005eda50(capp, nullptr, owner);                              // 005A1B76..005A1B81
    void* const cam = at(entry, 0x84);
    (void)kotor_fastcall_00631d80(frame_R4_005ed690(capp, nullptr), nullptr, player, a2, a3, a4, a5, gender, &text,
        static_cast<U>(n), ids, anims, rd(cam, 8), rd(cam, 0xc), rd(cam, 0x10), rd(cam, 0x14), rd(entry, 0x20),
        rd16(cam, 0x18), rd(cam, 0x1c), rd(cam, 0x20), rd8(entry, 0x68), word(at(entry, 0x6c)), rd(entry, 0x7c),
        rd(entry, 0x78), sound);                                                             // 005A1AD4..005A1B98 (23 args)
    release(ids); release(anims); release(nondep);                                           // 005A1B9D..005A1BC9
    return 1;
}

// 005A1C00 CSWSDialog::SendDialogReplyNode(player, id, gender, replies, active, appended, entry, flag) RET 20h
// Returns 0 on mixed empty/non-empty reply texts (GUI error string), else 1. Quirks kept: the PLAYER listener
// lookup result is discarded (listener stays invalid); the unequip passes are fed client ids; the owner object is
// fetched without checking the lookup status (its out slot starts null here, uninitialised stack originally).
extern "C" U __fastcall kotor_fastcall_005a1c00(void* d, void*, U player, U id, U gender, void* replies, I active,
                                                I appended, U entry, U flag) {
    const I total = active + appended;                                                       // 005A1C21..005A1C30
    U has_empty = 0, has_text = 0;                                                           // [esp+20h], ebx
    for (I i = 0; i < total; ++i) {                                                          // 005A1C3D..005A1CCB
        ExoString s;                                                                         // 005A1C50..005A1C54
        const U language = kotor_fastcall_004ae810(server(), nullptr, player);               // 005A1C59..005A1C7A
        (void)exo_locstring_get_005ea130(at(replies, static_cast<U>(i) * 0xb4 + 8), nullptr, language, &s, gender); // 005A1C7F..005A1C82
        if (str_equals(&s, "")) has_empty = 1; else has_text = 1;                            // 005A1C87..005A1CA3
    }
    if (has_text != 0 && has_empty != 0) {                                                   // 005A1CCD..005A1CD7
        ExoString message("CONVERSATION ERROR: Last Conversation Node Contains Either an END NODE or CONTINUE NODE.  "
                          "Please contact a Designer!");                                     // 005A1CD9..005A1CE2 [0074A658]
        void* gui = in_game_gui();                                                           // 005A1CE7..005A1CFC
        wr(gui, 0xb14, 1);                                                                   // 005A1D0C
        str_assign(at(gui, 0xb18), &message);                                                // 005A1D01..005A1D12
        return 0;                                                                            // 005A1D2B
    }
    void* owner = nullptr;                                                                   // [esp+94h]
    (void)kotor_fastcall_004d8230(object_array(), nullptr, rd(d, 0x80), &owner);              // 005A1D44..005A1D66 (status ignored)
    void* const owner_obj = vcall<void*>(owner, 0x10);                                       // 005A1D6B..005A1D7F [esp+6Ch]
    const U t = static_cast<U>(total);
    void* const strings_block = heap(t * 8 + 4);                                             // 005A1D77..005A1D83
    char* strings = nullptr;                                                                 // [esp+48h]
    if (strings_block != nullptr) {
        wr(strings_block, 0, t); strings = at(strings_block, 4);                             // 005A1DA9..005A1DAF
        crt_eh_vector_constructor_006fb55f(strings, 8, total,
            reinterpret_cast<void*>(&kotor_menu20::ConstructString005b3190),
            reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));                        // 005A1D9E..005A1DB1
    }
    U* const A1 = static_cast<U*>(heap(t * 4));                                              // link index     [esp+38h]
    U* const A2 = static_cast<U*>(heap(t * 4));                                              // listener id    [esp+2Ch]
    U* const A3 = static_cast<U*>(heap(t * 4));                                              // camera angle   [esp+64h]
    U* const A4 = static_cast<U*>(heap(t * 4));                                              // camera id      [esp+90h]
    U* const A5 = static_cast<U*>(heap(t * 4));                                              // cam height     [esp+7Ch]
    U* const A6 = static_cast<U*>(heap(t * 4));                                              // target height  [esp+5Ch]
    U* const A7 = static_cast<U*>(heap(t * 4));                                              // wait flags     [esp+8Ch]
    U16* const A8 = static_cast<U16*>(heap(t * 2));                                          // cam animation  [esp+74h]
    U* const A9 = static_cast<U*>(heap(t * 4));                                              // fov            [esp+84h]
    U* const A10 = static_cast<U*>(heap(t * 4));                                             // video effect   [esp+60h]
    U8* const A11 = static_cast<U8*>(heap(t));                                               // fade type      [esp+68h]
    char* A12 = static_cast<char*>(heap(t * 12));                                            // fade colour    [esp+50h]
    if (A12 != nullptr)                                                                      // 005A1E6D..005A1E8C
        for (U i = 0; i < t; ++i) { wr(A12, i * 12, 0); wr(A12, i * 12 + 4, 0); wr(A12, i * 12 + 8, 0); }
    U* const B1 = static_cast<U*>(heap(t * 4));                                              // fade delay     [esp+20h]
    U* const B2 = static_cast<U*>(heap(t * 4));                                              // fade length    [esp+24h]
    U* const B3 = static_cast<U*>(heap(t * 4));                                              // anim counts    [esp+40h]
    U* const B4 = static_cast<U*>(heap(t * 4));                                              // anim id arrays (ebx)
    U* const B5 = static_cast<U*>(heap(t * 4));                                              // anim word arrays [esp+58h]
    const U angle = step_camera_angle(d);                                                    // 005A1EC8..005A1F22
    void* const capp = client();                                                             // 005A1F2E..005A1F36 [esp+54h]
    for (I i = 0; i < total; ++i) {                                                          // 005A1F3A..005A2440
        void* rep = at(replies, static_cast<U>(i) * 0xb4);
        void* cam = at(rep, 0x84);                                                           // esi
        {
            ResultString txt;
            (void)kotor_fastcall_0059f320(d, nullptr, &txt, at(rep, 8), gender);             // 005A2010..005A2024
            str_assign(strings + i * 8, &txt);                                               // 005A2029..005A203C
        }                                                                                    // 005A2041..005A2053
        A1[i] = rd(rep, 0xb0);                                                               // 005A2058..005A2069
        U listener_id = kInvalid;                                                            // 005A2072 [esp+70h]
        if (owner_obj != nullptr && dlg::str_not_equal(cam, "") != 0) {                      // 005A206C..005A208E
            if (dlg::str_compare_nc(cam, "OWNER") != 0) {                                    // 005A2090..005A209E
                listener_id = kotor_fastcall_005eda50(capp, nullptr, rd(d, 0x80));          // 005A20A0..005A20FB
            } else if (dlg::str_compare_nc(cam, "PLAYER") != 0) {                            // 005A20AC..005A20BA
                (void)kotor_fastcall_004ae910(server(), nullptr, kotor_fastcall_004aea40(server(), nullptr)); // 005A20BC..005A20D9 (discarded)
            } else {
                void* sp = kotor_fastcall_0059fc30(d, nullptr, owner_obj, cam);              // 005A20DB..005A20E5
                if (sp != nullptr) listener_id = kotor_fastcall_005eda50(capp, nullptr, rd(sp, 4)); // 005A20EA..005A20FB
            }
        }
        const U na = rd(rep, 0x60);                                                          // 005A20FF
        B3[i] = na; B4[i] = 0; B5[i] = 0;                                                    // 005A2102..005A211A
        if (na != 0) {                                                                       // 005A2121
            B4[i] = word(heap(na * 4));                                                      // 005A2123..005A2134
            B5[i] = word(heap(na * 2));                                                      // 005A2130..005A2143
        }
        for (I j = 0; j < static_cast<I>(na); ++j) {                                         // 005A214D..005A2327
            void* an = at(rp(rep, 0x64), static_cast<U>(j) * 0xc);
            const U oid = participant_id(d, an, owner_obj, nullptr);
            if (kotor_fastcall_004ae750(server(), nullptr, oid) != nullptr) {                // 005A2256..005A226A
                wr(ptr(B4[i]), static_cast<U>(j) * 4, kotor_fastcall_005eda50(capp, nullptr, oid)); // 005A226C..005A2280
                wr16(ptr(B5[i]), static_cast<U>(j) * 2, rd16(rp(rep, 0x64), static_cast<U>(j) * 0xc + 8)); // 005A2283..005A229A
            } else {
                participant_error(at(rp(rep, 0x64), static_cast<U>(j) * 0xc));               // 005A22A0..005A22D4
                wr(ptr(B4[i]), static_cast<U>(j) * 4, kInvalid);                             // 005A22D9..005A22E3
                wr16(ptr(B5[i]), static_cast<U>(j) * 2, 0);                                  // 005A22EA..005A22ED
            }
        }
        if (rd(cam, 8) == 0) wr(cam, 8, rd(d, 0xf8) != 0 ? 2u : angle);                      // 005A232D..005A2352
        A2[i] = listener_id;                                                                 // 005A2355..005A235D
        A3[i] = rd(cam, 8); A4[i] = rd(cam, 0xc); A5[i] = rd(cam, 0x10); A6[i] = rd(cam, 0x14); // 005A2360..005A2391
        A7[i] = rd(rep, 0x20);                                                               // 005A2394..005A239E
        A8[i] = rd16(cam, 0x18);                                                             // 005A23A1..005A23AD
        A9[i] = rd(cam, 0x1c); A10[i] = rd(cam, 0x20);                                       // 005A23B1..005A23C8
        A11[i] = rd8(rep, 0x68);                                                             // 005A23CB..005A23D2
        wr(A12, static_cast<U>(i) * 12, rd(rep, 0x6c)); wr(A12, static_cast<U>(i) * 12 + 4, rd(rep, 0x70));
        wr(A12, static_cast<U>(i) * 12 + 8, rd(rep, 0x74));                                  // 005A23D5..005A23F3
        B1[i] = rd(rep, 0x7c); B2[i] = rd(rep, 0x78);                                        // 005A23F6..005A240A
    }
    wr(d, 0xf8, 0);                                                                          // 005A244F
    if (rd(d, 0x40) != 0 && total > 0) {                                                     // 005A2446..005A2461
        for (I k = 0; k < total; ++k) {                                                      // 005A2481..005A24C9
            kotor_fastcall_005a0ba0(d, nullptr, A2[k]);
            for (I m = 0; m < static_cast<I>(B3[k]); ++m) kotor_fastcall_005a0ba0(d, nullptr, rd(ptr(B4[k]), static_cast<U>(m) * 4));
        }
    }
    if (rd(d, 0x44) != 0 && total > 0) {                                                     // 005A24CB..005A24DC
        for (I k = 0; k < total; ++k) {                                                      // 005A2500..005A254A
            kotor_fastcall_005a0d10(d, nullptr, A2[k]);
            for (I m = 0; m < static_cast<I>(B3[k]); ++m) kotor_fastcall_005a0d10(d, nullptr, rd(ptr(B4[k]), static_cast<U>(m) * 4));
        }
    }
    const U sid = kotor_fastcall_005eda50(capp, nullptr, id);                                // 005A25DE..005A25E1
    (void)kotor_fastcall_00634340(frame_R4_005ed690(capp, nullptr), nullptr, player, sid, gender, strings,
        word(A1), word(A2), word(A3), word(A4), word(A5), word(A6), word(A7), word(A8), word(A9), word(A10), word(A11),
        word(A12), word(B1), word(B2), static_cast<U>(active), static_cast<U>(appended), entry, word(B3), word(B4),
        word(B5), flag);                                                                     // 005A254C..005A25F4 (25 args)
    release(A1); release(A2);                                                                // 005A25F9..005A2614
    if (strings != nullptr) {                                                                // 005A2617..005A263A
        c2da_destroy_array_006fa852(strings, 8, rdi(strings - 4, 0), reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));
        live_free(strings - 4);
    }
    release(A3); release(A4); release(A5); release(A6); release(A7); release(A8); release(A9); release(A10);
    release(A11); release(A12); release(B1); release(B2);                                    // 005A263C..005A270E
    for (I i = 0; i < total; ++i) { release(ptr(B4[i])); release(ptr(B5[i])); }              // 005A2711..005A275F
    release(B4); release(B5); release(B3);                                                   // 005A2761..005A278D
    return 1;
}
