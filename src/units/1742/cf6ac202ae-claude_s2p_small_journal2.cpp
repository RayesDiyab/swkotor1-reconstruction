// Lane worker "s2p-small" (journal fork): client quest/world journal containers used by
// 0066A370 CSWCMessage::HandleServerToPlayerJournalMessage (see claude_s2p_small_journal.cpp).
// CSWCJournal (Add/Remove/SetJournalPicture/ClearAll/sort), CSWCSortedJournalArrayList sort + comparators,
// CSWCWorldJournal (AddEntry/AddEntryStrRef/Delete*/SortList + comparators), SJournalEntry::Update,
// CExoArrayList<SJournalEntry>::Remove/AddAndDispose, CSWCJournal::JournalUpdated + pending-update list.
// Transcribed from the original swkotor.exe (read-only); statement comments cite original instruction addresses.
// Conventions: claude_dialogue_common.hpp. thiscall -> extern "C" __fastcall(self, edx, stack args...).
// Game heap only: operator new 006FA7E6 -> live_operator_new, _free 006FA390 -> live_free.
//
// Layouts (from the instructions below):
//   SJournalEntry (0x3C): +0 CExoLocString name, +8 CExoLocString text, +10h/+14h date/time, +18h CExoString tag,
//     +20h state, +24h planet, +28h WORD picture, +2Ch bitfield (bit0 completed, bit2 updated), +30h, +34h priority, +38h.
//   CSWCSortedJournalArrayList (0x20): +0 CExoArrayList<SJournalEntry>, +0Ch CExoArrayList<int> sorted index,
//     +18h sort mode, +1Ch dirty.
//   CSWCJournal: +0 list (active), +20h list (completed), +40h flags (bit0 sort enabled),
//     +44h CExoArrayList<CSWCJournalPendingUpdate> (element 0xC: CExoString, dword).
//   CSWCWorldJournal: +0 CExoArrayList<CWorldJournalEntry> (0x24 elements), +0Ch CExoArrayList<int> sorted index,
//     +18h BYTE sort mode, +1Ch dirty bitfield.
#include "6bd5403233-claude_dialogue_common.hpp"

using namespace dlg;

namespace kotor_reconstruction { struct ArrayMutations; void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); }
namespace directory_list { int __fastcall Less005e5410(kotor_reconstruction::StartupString*, void*, kotor_reconstruction::StartupString*); }
namespace alias_add { int __fastcall Greater005e5490(kotor_reconstruction::StartupString*, void*, kotor_reconstruction::StartupString*); }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned); }

namespace {
struct SJournalEntryArg { U w[15]; };       // SJournalEntry passed by value (0x3C bytes; the callee destroys it)
static_assert(sizeof(SJournalEntryArg) == 0x3c, "SJournalEntry by value");
struct PendingUpdateArg { U w[3]; };        // CSWCJournalPendingUpdate by value {CExoString, dword}
static_assert(sizeof(PendingUpdateArg) == 0xc, "CSWCJournalPendingUpdate by value");
struct WorldJournalEntryArg { U w[9]; };    // CWorldJournalEntry by value (0x24 bytes; callee destroys it)
static_assert(sizeof(WorldJournalEntryArg) == 0x24, "CWorldJournalEntry by value");
}

extern "C" {
// real providers (c19 map)
int __fastcall async_string_equal_005e5200(const void* s, void*, const void* other);      // CExoString::operator==
void* __fastcall kotor_fastcall_005c53e0(void* entry, void*);                              // SJournalEntry() (dialogue_server)
void* __fastcall kotor_fastcall_005c5470(void* dst, void*, const void* src);               // SJournalEntry::operator=
void* __fastcall kotor_fastcall_005c5560(void* dst, void*, const void* src);               // SJournalEntry(const&)
void __fastcall kotor_fastcall_005c5500(void* entry, void*);                               // ~SJournalEntry (0703)
void __fastcall kotor_fastcall_005c5680(void* list, void*, U count);                       // CExoArrayList<SJournalEntry>::Allocate
void* __fastcall kotor_fastcall_004c4080(void* entry, void*, const void* src);             // CWorldJournalEntry(const&)
void __fastcall kotor_fastcall_004c65e0(void* list, void*, WorldJournalEntryArg entry);    // CExoArrayList<CWorldJournalEntry>::Add
void __fastcall kotor_fastcall_004c66b0(void* list, void*, I index);                       // CExoArrayList<CWorldJournalEntry>::DelIndex
void __fastcall world_journal_entry_destroy_004c4030(void* entry, void*);                  // ~CWorldJournalEntry
void* __fastcall frame_GetWorldTimer_005ed4f0(void* client, void*);                        // CClientExoApp::GetWorldTimer
I __fastcall recovered_event_time_compare_004adfa0(void* timer, void*, U d1, U t1, U d2, U t2); // CWorldTimer::CompareWorldTimes
U __fastcall kotor_fastcall_005ed4c0(void* client, void*);                                 // CClientExoApp::GetClientLanguage
dlg::U __fastcall exo_locstring_get_005ea130(void* loc, void*, dlg::U language, void* out, dlg::U gender); // GetString
extern void* kotor_cell_tlk_007a3a08;                                                      // [007A3A08] CTlkTable*
dlg::U __fastcall tlk_fetch_0041e550(void* tlk, void*, dlg::U strref, void* out, dlg::U flag); // CTlkTable::Fetch
void* __fastcall frame_LogAssign_005e5140(void* s, void*, const char* text);              // CExoString::operator=(char*)
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);            // CSWCCMessageData()
void __fastcall recovered_event_string_set_004d6ae0(void* data, void*, U index, const void* s); // CSWCCMessageData::SetString
void __fastcall recovered_client_add_feedback_message_005ed360(void* client, void*, U type, void* data, void* s); // AddFeedbackMessage
void __fastcall kotor_fastcall_0062eeb0(void* gui, void*, std::uint8_t kind, std::uint32_t value); // CGuiInGame::UpdateStatus
void __fastcall kotor_fastcall_0059ea30(void* s, void*);                                   // ~CExoString (element dtor thunk)
void* __fastcall kotor_fastcall_0059ea90(void* s, void*);                                  // CExoString() (element ctor thunk)
}

// Exports of this file
extern "C" {
// [00833A94] CSWGuiInGameJournal "journal updated" cell (.bss, initial 0). No rebuilt storage existed; defined here.
// Written by CSWCJournal::JournalUpdated (00676D86), read/cleared by CSWGuiInGameJournal::OnPanelAdded (00645E38).
volatile U kotor_cell_journal_updated_00833a94 = 0;

void __fastcall kotor_fastcall_00675db0(void* journal, void*);
void __fastcall kotor_fastcall_00675dc0(void* journal, void*);
void __fastcall kotor_fastcall_00676530(void* list, void*);
I __fastcall kotor_fastcall_00675ed0(void* list, void*, I a, I b);
I __fastcall kotor_fastcall_00675f10(void* list, void*, I a, I b);
I __fastcall kotor_fastcall_00676050(void* list, void*, I a, I b);
I __fastcall kotor_fastcall_006760a0(void* list, void*, I a, I b);
void __fastcall kotor_fastcall_00676ba0(void* journal, void*);
void __fastcall kotor_fastcall_00676b00(void* journal, void*);
void __fastcall kotor_fastcall_006760f0(void* journal, void*, const void* tag);
void __fastcall kotor_fastcall_00676260(void* journal, void*, const void* tag, U picture);
void __fastcall kotor_fastcall_00676610(void* journal, void*, SJournalEntryArg entry, U flags);
void __fastcall kotor_fastcall_005c5740(void* entry, void*, SJournalEntryArg src, U flags);
void __fastcall kotor_fastcall_005c5640(void* list, void*, I index);
void __fastcall kotor_fastcall_005c5920(void* list, void*, SJournalEntryArg entry);
void __fastcall kotor_fastcall_00676c20(void* journal, void*, U deferrable, U completed, const void* text);
void __fastcall kotor_fastcall_006764a0(void* list, void*, PendingUpdateArg update);
void __fastcall kotor_fastcall_00675de0(void* list, void*, U capacity);
U __fastcall kotor_fastcall_005ed970(void* client, void*);
I __fastcall kotor_fastcall_00676370(void* wj, void*, I a, I b, U flag);
I __fastcall kotor_fastcall_006763d0(void* wj, void*, I a, I b, U flag);
void __fastcall kotor_fastcall_00677000(void* wj, void*);
void __fastcall kotor_fastcall_00677300(void* wj, void*, U index, const void* s1, const void* s2, U t1, U t2);
void __fastcall kotor_fastcall_00677130(void* wj, void*, U strref1, U strref2, U t1, U t2);
void __fastcall kotor_fastcall_00676f00(void* wj, void*, U index);
void __fastcall kotor_fastcall_00676e60(void* wj, void*, U strref);
void __fastcall kotor_fastcall_00676fa0(void* wj, void*);
}

namespace {
constexpr U kEntry = 0x3c;
constexpr U kWorld = 0x24;
inline int str_eq(const void* s, const void* other) { return async_string_equal_005e5200(s, nullptr, other); }
inline kotor_reconstruction::StartupString* ss(void* s) { return static_cast<kotor_reconstruction::StartupString*>(s); }
struct JournalEntry {                         // SJournalEntry local: ctor 005C53E0, dtor 005C5500 (inlined in the original)
    alignas(4) unsigned char raw[0x3c];
    JournalEntry() { (void)kotor_fastcall_005c53e0(this, nullptr); }
    JournalEntry(const JournalEntry&) = delete;
    JournalEntry& operator=(const JournalEntry&) = delete;
    ~JournalEntry() { kotor_fastcall_005c5500(this, nullptr); }
};
struct EntryArgDtor {                         // the by-value SJournalEntry argument, destroyed by the callee
    void* p;
    ~EntryArgDtor() { kotor_fastcall_005c5500(p, nullptr); }
};
inline SJournalEntryArg copy_entry(const void* src) {   // SJournalEntry(const&) into the outgoing argument area
    SJournalEntryArg a;
    (void)kotor_fastcall_005c5560(&a, nullptr, src);
    return a;
}
// Inline CExoArrayList<int> growth used by CSWCJournal::Add (operator new / copy / _free).
// list: {data +0, count +4, capacity +8}
void index_list_grow(void* l) {
    const U cap = rd(l, 8);
    if (rd(l, 4) != cap) return;                                                        // 0067680C / 00676A08 (jne)
    const U n = cap != 0 ? cap + cap : 0x10;                                            // 00676815..0067681F / 00676A0D..00676A15
    U* const old = static_cast<U*>(rp(l, 0));                                           // 0067698D / 00676A1A edi
    wr(l, 8, n);                                                                        // 00676998 / 00676A1D
    void* const fresh = live_operator_new(n * 4);                                       // 00676990..0067699B / 00676A20..00676A24
    wp(l, 0, fresh);                                                                    // 006769A3 / 00676A2C
    for (I i = 0; i < rdi(l, 4); ++i)                                                   // 006769A9..006769BF (count re-read)
        wr(rp(l, 0), static_cast<U>(i) * 4, old[i]);
    if (old != nullptr) live_free(old);                                                 // 006769C1..006769C6 / 00676A51..00676A56
}
inline void index_list_append(void* l, U value) {
    wr(rp(l, 0), rd(l, 4) * 4, value);                                                  // 006769CE..006769D4 / 00676A5E..00676A64
    wr(l, 4, rd(l, 4) + 1);                                                             // 006769D7 / 00676A67
}
// Selection sort of the sorted-index array shared by 00676530 and 00677000.
template <class Cmp> void selection_sort(void* list, U index_offset, I n, Cmp cmp) {
    for (I i = 0; i < n;) {                                                             // 0067656C..006765F9
        I best = i;                                                                     // [esp+18h]
        const I next = i + 1;
        for (I j = next; j < n; ++j) {
            U* idx = static_cast<U*>(rp(list, index_offset));
            if (cmp(static_cast<I>(idx[best]), static_cast<I>(idx[j])) < 0) best = j;  // call [esp+18h]; jge
        }
        U* idx = static_cast<U*>(rp(list, index_offset));
        const U t = idx[i];                                                             // 006765DB..006765F6 swap
        idx[i] = idx[best];
        static_cast<U*>(rp(list, index_offset))[best] = t;
        i = next;
    }
}
}  // namespace

// ===============================================================================================================
// SJournalEntry / CExoArrayList<SJournalEntry>
// ===============================================================================================================
// 005C5740 SJournalEntry::Update(SJournalEntry src by value, WORD flags) RET 40h
extern "C" void __fastcall kotor_fastcall_005c5740(void* self, void*, SJournalEntryArg src, U flags) {
    void* const s = &src;
    EntryArgDtor dtor{s};                                                               // 005C5821..005C5830
    str_assign(fa(self, 0x18), fa(s, 0x18));                                            // 005C5759..005C5769 (always)
    const U8 lo = static_cast<U8>(flags);                                               // bl
    const U8 hi = static_cast<U8>(flags >> 8);                                          // bh
    const bool all = (hi & 0x80) != 0;                                                  // test bh,bh / js
    if ((lo & 1) || all) (void)recovered_localized_assign_005e9e70(self, nullptr, s);   // 005C5772..005C5782
    if ((lo & 2) || all) (void)recovered_localized_assign_005e9e70(fa(self, 8), nullptr, fa(s, 8)); // 005C5787..005C5798
    if ((lo & 4) || all) wr(self, 0x10, rd(s, 0x10));                                   // 005C579D..005C57AA
    if ((lo & 8) || all) wr(self, 0x14, rd(s, 0x14));                                   // 005C57AD..005C57BA
    if ((lo & 0x10) || all) wr(self, 0x20, rd(s, 0x20));                                // 005C57BD..005C57CA
    if ((lo & 0x20) || all) wr(self, 0x24, rd(s, 0x24));                                // 005C57CD..005C57DA
    // flag 40h (picture, +28h) is never applied here (original behaviour).
    if ((lo & 0x80) || all) {                                                           // 005C57DD..005C57F4
        const U b = rd(self, 0x2c);
        wr(self, 0x2c, b ^ ((b ^ rd(s, 0x2c)) & 1));
    }
    if (hi & 0x82) wr(self, 0x30, rd(s, 0x30));                                         // 005C57F7..005C5800
    if (hi & 0x84) wr(self, 0x34, rd(s, 0x34));                                         // 005C5803..005C580C
    if (hi & 0x88) wr(self, 0x38, rd(s, 0x38));                                         // 005C580F..005C5818
    wr(self, 0x2c, rd(self, 0x2c) | 4);                                                 // 005C581B..005C5825
}

// 005C5640 CExoArrayList<SJournalEntry>::Remove(int index) RET 4
extern "C" void __fastcall kotor_fastcall_005c5640(void* list, void*, I index) {
    wr(list, 4, rd(list, 4) - 1);                                                       // 005C5648..005C5650
    for (I i = index; i < rdi(list, 4); ++i)                                            // 005C564E..005C5677 (count re-read)
        (void)kotor_fastcall_005c5470(fa(rp(list, 0), static_cast<U>(i) * kEntry), nullptr,
                                      fa(rp(list, 0), static_cast<U>(i) * kEntry + kEntry)); // 005C5660..005C5669
}

// 005C5920 CExoArrayList<SJournalEntry>::AddAndDispose(SJournalEntry by value) RET 3Ch
extern "C" void __fastcall kotor_fastcall_005c5920(void* list, void*, SJournalEntryArg entry) {
    EntryArgDtor dtor{&entry};                                                          // 005C5977..005C5983
    if (rd(list, 4) == rd(list, 8)) {                                                   // 005C5938..005C5946
        const U a = rd(list, 8);
        kotor_fastcall_005c5680(list, nullptr, a != 0 ? a + a : 0x10);                  // 005C5948..005C5958
    }
    const U c = rd(list, 4);
    void* slot = fa(rp(list, 0), c * kEntry);                                           // 005C595D..005C5967
    wr(list, 4, c + 1);                                                                 // 005C5969..005C596A
    (void)kotor_fastcall_005c5470(slot, nullptr, &entry);                               // 005C596D..005C5972
}

// ===============================================================================================================
// CSWCSortedJournalArrayList
// ===============================================================================================================
// 00675ED0 SortItemsByDate(int a, int b) RET 8
extern "C" I __fastcall kotor_fastcall_00675ed0(void* list, void*, I a, I b) {
    void* const data = rp(list, 0);                                                     // 00675EDF..00675EE1
    void* const ea = fa(data, static_cast<U>(a) * kEntry);
    void* const eb = fa(data, static_cast<U>(b) * kEntry);
    void* const timer = frame_GetWorldTimer_005ed4f0(client(), nullptr);               // 00675EE3..00675F00
    return recovered_event_time_compare_004adfa0(timer, nullptr, rd(ea, 0x10), rd(ea, 0x14), rd(eb, 0x10),
                                                 rd(eb, 0x14));                         // 00675EF0..00675F07
}

// 00675F10 SortItemsByName(int a, int b) RET 8
extern "C" I __fastcall kotor_fastcall_00675f10(void* list, void*, I a, I b) {
    ExoString name_a;                                                                   // 00675F2D..00675F31 [esp+14h]
    ExoString name_b;                                                                   // 00675F38..00675F40 [esp+0Ch]
    const U lang = kotor_fastcall_005ed4c0(client(), nullptr);                         // 00675F45..00675F52 edi
    (void)exo_locstring_get_005ea130(fa(rp(list, 0), static_cast<U>(a) * kEntry), nullptr, lang, &name_a, 0); // 00675F57..00675F6B
    (void)exo_locstring_get_005ea130(fa(rp(list, 0), static_cast<U>(b) * kEntry), nullptr, lang, &name_b, 0); // 00675F70..00675F80
    if (directory_list::Less005e5410(ss(&name_a), nullptr, ss(&name_b)) != 0) return 1; // 00675F85..00675FB7
    if (alias_add::Greater005e5490(ss(&name_a), nullptr, ss(&name_b)) != 0) return -1;  // 00675FCE..00676000
    return 0;                                                                           // 00676031
}

// 00676050 SortItemsByPlanet(int a, int b) RET 8 (+24h unsigned, ties by date)
extern "C" I __fastcall kotor_fastcall_00676050(void* list, void*, I a, I b) {
    const U pa = rd(rp(list, 0), static_cast<U>(a) * kEntry + 0x24);                  // 0067605D..00676062
    const U pb = rd(rp(list, 0), static_cast<U>(b) * kEntry + 0x24);                  // 00676066..0067606B
    if (pa > pb) return -1;                                                             // 0067606F..00676075 (jbe)
    if (pa < pb) return 1;                                                              // 0067607C..00676080 (jae)
    return kotor_fastcall_00675ed0(list, nullptr, a, b);                                // 00676089..0067608B
}

// 006760A0 SortItemsByPriority(int a, int b) RET 8 (+34h signed, ties by date)
extern "C" I __fastcall kotor_fastcall_006760a0(void* list, void*, I a, I b) {
    const I pa = rdi(rp(list, 0), static_cast<U>(a) * kEntry + 0x34);                  // 006760AD..006760B2
    const I pb = rdi(rp(list, 0), static_cast<U>(b) * kEntry + 0x34);                  // 006760B6..006760BB
    if (pa > pb) return 1;                                                              // 006760BF..006760C5 (jle)
    if (pa < pb) return -1;                                                             // 006760CE..006760D2 (jge)
    return kotor_fastcall_00675ed0(list, nullptr, a, b);                                // 006760D9..006760DB
}

// 00676530 CSWCSortedJournalArrayList::SortList() RET 0
extern "C" void __fastcall kotor_fastcall_00676530(void* list, void*) {
    using Cmp = I(__fastcall*)(void*, void*, I, I);
    Cmp cmp;
    const U mode = rd(list, 0x18);                                                      // 00676537
    if (mode == 0) cmp = &kotor_fastcall_00675ed0;                                      // 0067653C..0067653E
    else if (mode == 1) cmp = &kotor_fastcall_00675f10;                                 // 00676548..0067654D
    else if (mode == 2) cmp = &kotor_fastcall_00676050;                                 // 00676557..0067655A
    else cmp = &kotor_fastcall_006760a0;                                                // 00676564
    const I n = rdi(list, 4);                                                           // 0067656C..00676573 (cached)
    selection_sort(list, 0xc, n, [&](I x, I y) { return cmp(list, nullptr, x, y); });   // 0067657D..006765F9
    wr(list, 0x1c, 1);                                                                  // 006765FD
}

// ===============================================================================================================
// CSWCJournal
// ===============================================================================================================
// 00675DB0 CSWCJournal::EnableSort / 00675DC0 DisableSort RET 0
extern "C" void __fastcall kotor_fastcall_00675db0(void* j, void*) { wr(j, 0x40, rd(j, 0x40) | 1); }          // 00675DB0
extern "C" void __fastcall kotor_fastcall_00675dc0(void* j, void*) { wr(j, 0x40, rd(j, 0x40) & 0xfffffffeu); } // 00675DC0

// 00676BA0 CSWCJournal::CallFullUpdateComplete() RET 0
extern "C" void __fastcall kotor_fastcall_00676ba0(void* j, void*) {
    kotor_fastcall_00676530(fa(j, 0x20), nullptr);                                      // 00676BA3..00676BA6
    kotor_fastcall_00676530(j, nullptr);                                                // 00676BAB..00676BAE (tail jump)
}

// 00676B00 CSWCJournal::ClearAll() RET 0
extern "C" void __fastcall kotor_fastcall_00676b00(void* j, void*) {
    const auto clear = [](void* list) {                                                 // 00676B04..00676B2F / 00676B4B..00676B75
        void* const data = rp(list, 0);
        if (data != nullptr) {
            c2da_destroy_array_006fa852(data, kEntry, rdi(data, static_cast<U>(-4)), reinterpret_cast<void*>(&kotor_fastcall_005c5500));
            live_free(fa(data, static_cast<U>(-4)));
            wr(list, 0, 0);
            wr(list, 8, 0);
        }
        wr(list, 4, 0);
        void* const idx = rp(list, 0xc);                                                // 00676B32..00676B48 / 00676B78..00676B8E
        if (idx != nullptr) {
            live_free(idx);
            wr(list, 0xc, 0);
            wr(list, 0x14, 0);
        }
        wr(list, 0x10, 0);
    };
    clear(j);                                                                           // active list (+0)
    clear(fa(j, 0x20));                                                                 // completed list (+20h)
}

// 006760F0 CSWCJournal::Remove(CExoString const& tag) RET 4
extern "C" void __fastcall kotor_fastcall_006760f0(void* j, void*, const void* tag) {
    const I n2 = rdi(j, 0x24);                                                          // 006760F6 ebp
    for (I k = 0; k < n2; ++k) {                                                        // 006760FA..0067611D
        if (!str_eq(fa(rp(j, 0x20), static_cast<U>(k) * kEntry + 0x18), tag)) continue;
        kotor_fastcall_005c5640(fa(j, 0x20), nullptr, k);                               // 00676156..0067615A
        I bound = n2;                                                                   // ebp (stale until a removal)
        I c = 0;
        do {                                                                            // 0067615F..006761AE
            if (rdi(rp(j, 0x2c), static_cast<U>(c) * 4) == k) {                         // 00676161..00676167
                wr(j, 0x30, rd(j, 0x30) - 1);                                           // 00676169..00676173
                for (I e = c; e < rdi(j, 0x30); ++e)                                    // 0067616F..00676192
                    wr(rp(j, 0x2c), static_cast<U>(e) * 4, rd(rp(j, 0x2c), static_cast<U>(e) * 4 + 4));
                bound = rdi(j, 0x30);                                                   // 00676194
            }
            if (c != bound) {                                                           // 00676197..00676199
                const I v = rdi(rp(j, 0x2c), static_cast<U>(c) * 4);
                if (v > k) wr(rp(j, 0x2c), static_cast<U>(c) * 4, static_cast<U>(v - 1)); // 0067619B..006761A9
            }
            ++c;
        } while (c < bound);
        wr(j, 0x3c, 1);                                                                 // 006761B1
        return;
    }
    const I n1 = rdi(j, 4);                                                             // 0067611F..00676126 edi / [esp+10h]
    for (I k = 0; k < n1; ++k) {                                                        // 00676130..0067614C
        if (!str_eq(fa(rp(j, 0), static_cast<U>(k) * kEntry + 0x18), tag)) continue;
        wr(j, 4, rd(j, 4) - 1);                                                         // 006761BF..006761C9
        for (I e = k; e < rdi(j, 4); ++e)                                               // 006761C5..006761EA (count re-read)
            (void)kotor_fastcall_005c5470(fa(rp(j, 0), static_cast<U>(e) * kEntry), nullptr,
                                          fa(rp(j, 0), static_cast<U>(e) * kEntry + kEntry));
        I bound = n1;                                                                   // edi
        I c = 0;
        do {                                                                            // 006761F0..00676243
            if (rdi(rp(j, 0xc), static_cast<U>(c) * 4) == k) {                          // 006761F2..006761F8
                wr(j, 0x10, rd(j, 0x10) - 1);                                           // 006761FA..00676204
                for (I e = c; e < rdi(j, 0x10); ++e)                                    // 006761FE..00676222
                    wr(rp(j, 0xc), static_cast<U>(e) * 4, rd(rp(j, 0xc), static_cast<U>(e) * 4 + 4));
                bound = rdi(j, 0x10);                                                   // 00676224..0067622B
            }
            if (c != bound) {                                                           // 0067622D..0067622F
                const I v = rdi(rp(j, 0xc), static_cast<U>(c) * 4);
                if (v > k) wr(rp(j, 0xc), static_cast<U>(c) * 4, static_cast<U>(v - 1)); // 00676231..0067623E
            }
            ++c;
        } while (c < bound);
        wr(j, 0x1c, 1);                                                                 // 00676245
        return;
    }
}

// 00676260 CSWCJournal::SetJournalPicture(CExoString const& tag, WORD picture) RET 8
extern "C" void __fastcall kotor_fastcall_00676260(void* j, void*, const void* tag, U picture) {
    const I n1 = rdi(j, 4);                                                             // 00676265
    for (I k = 0; k < n1; ++k) {                                                        // 0067626F..0067628B
        if (str_eq(fa(rp(j, 0), static_cast<U>(k) * kEntry + 0x18), tag)) {
            wr16(rp(j, 0), static_cast<U>(k) * kEntry + 0x28, static_cast<U16>(picture)); // 006762BC..006762C6
            void* const e = fa(rp(j, 0), static_cast<U>(k) * kEntry);
            wr(e, 0x2c, rd(e, 0x2c) | 4);                                               // 006762CB..006762D7
            wr(j, 0x1c, 1);                                                             // 006762D9
            return;
        }
    }
    const I n2 = rdi(j, 0x24);                                                          // 0067628D
    for (I k = 0; k < n2; ++k) {                                                        // 00676296..006762B3
        if (str_eq(fa(rp(j, 0x20), static_cast<U>(k) * kEntry + 0x18), tag)) {
            wr16(rp(j, 0x20), static_cast<U>(k) * kEntry + 0x28, static_cast<U16>(picture)); // 006762E6..006762F3
            void* const e = fa(rp(j, 0x20), static_cast<U>(k) * kEntry);
            wr(e, 0x2c, rd(e, 0x2c) | 4);                                               // 006762F8..006762FF
            wr(j, 0x3c, 1);                                                             // 00676302
            return;
        }
    }
}

// 00676610 CSWCJournal::Add(SJournalEntry entry by value, WORD flags) RET 40h
extern "C" void __fastcall kotor_fastcall_00676610(void* j, void*, SJournalEntryArg entry, U flags) {
    void* const arg = &entry;
    EntryArgDtor arg_dtor{arg};                                                         // 00676AAF..00676ADD (destroyed last)
    const U completed = rd(arg, 0x2c) & 1;                                              // 0067662E..0067663D edi / [esp+18h]
    const I n2 = rdi(j, 0x24);                                                          // 00676635 [esp+14h]
    const I n1 = rdi(j, 4);                                                             // 00676638 ebx
    U found = 0;                                                                        // [esp+10h]
    JournalEntry moved;                                                                 // 00676640..00676654 [esp+28h]
    ExoString message;                                                                  // 00676659..00676662 [esp+20h]
    const void* const tag = fa(arg, 0x18);                                              // [esp+8Ch]
    // Take an entry with the same tag out of the other list (completed: +20h list, else the +0 list).
    if (completed == 1) {
        for (I k = 0; k < n2; ++k) {                                                    // 00676671..006766D4
            if (found != 0) break;                                                      // 00676680..00676686
            void* const e = fa(rp(j, 0x20), static_cast<U>(k) * kEntry);
            if (str_eq(fa(e, 0x18), tag)) {                                             // 0067668C..006766A2
                (void)kotor_fastcall_005c5470(moved.raw, nullptr, fa(rp(j, 0x20), static_cast<U>(k) * kEntry)); // 006766A4..006766AE
                kotor_fastcall_006760f0(j, nullptr, tag);                               // 006766B3..006766BD
                found = 1;                                                              // 006766C2
            }
        }
    } else {
        for (I k = 0; k < n1; ++k) {                                                    // 006766D8..0067672C
            if (found != 0) break;                                                      // 006766E2..006766E8
            void* const e = fa(rp(j, 0), static_cast<U>(k) * kEntry);
            if (str_eq(fa(e, 0x18), tag)) {                                             // 006766EA..006766FF
                (void)kotor_fastcall_005c5470(moved.raw, nullptr, fa(rp(j, 0), static_cast<U>(k) * kEntry)); // 00676701..0067670A
                kotor_fastcall_006760f0(j, nullptr, tag);                               // 0067670F..00676719
                found = 1;                                                              // 0067671E
            }
        }
    }
    if (found != 0) {                                                                   // 0067672E..00676734
        kotor_fastcall_005c5740(moved.raw, nullptr, copy_entry(arg), flags);            // 00676736..00676758
        (void)kotor_fastcall_005c5470(arg, nullptr, moved.raw);                         // 0067675D..00676766
    }
    // Update an existing entry with the same tag in the destination list.
    if (completed == 1) {
        for (I k = 0; k < n1; ++k) {                                                    // 00676778..006767A1
            if (!str_eq(fa(rp(j, 0), static_cast<U>(k) * kEntry + 0x18), tag)) continue;
            SJournalEntryArg c = copy_entry(arg);                                       // 00676824..00676840
            kotor_fastcall_005c5740(fa(rp(j, 0), static_cast<U>(k) * kEntry), nullptr, c, flags); // 00676845..0067684C
            if (rd(j, 0x40) & 1) kotor_fastcall_00676530(j, nullptr);                   // 00676851..00676859
            return;                                                                     // 0067685E..006768AF -> 00676AC8
        }
    } else {
        for (I k = 0; k < n2; ++k) {                                                    // 006768B4..006768F1
            if (!str_eq(fa(rp(j, 0x20), static_cast<U>(k) * kEntry + 0x18), tag)) continue;
            SJournalEntryArg c = copy_entry(arg);                                       // 006768F8..00676914
            kotor_fastcall_005c5740(fa(rp(j, 0x20), static_cast<U>(k) * kEntry), nullptr, c, flags); // 00676919..00676920
            if (rd(j, 0x40) & 1) kotor_fastcall_00676530(fa(j, 0x20), nullptr);         // 00676925..0067692D
            return;                                                                     // 00676932..00676983 -> 00676AC8
        }
    }
    // Not present: a partial update (flags bit 15 clear) of an unknown entry formats a diagnostic that is never used.
    if (found == 0 && (flags & 0x8000) == 0)                                            // 006767A3..006767B4
        kotor_reconstruction::StartupFormat005e5680(ss(&message),
            "Journal Crash! Partial Update on non-existant entry; plot:%s flags:%i",    // .rdata 00752C18
            WindowCStr005e5670(const_cast<void*>(tag), nullptr), static_cast<int>(flags & 0xffff)); // 006767B6..006767D6
    if (completed == 1) {                                                               // 006767D9..006767EC
        const U index = rd(j, 4);                                                       // 006767F2 ebx
        kotor_fastcall_005c5920(j, nullptr, copy_entry(arg));                           // 006767F5..00676804
        index_list_grow(fa(j, 0xc));                                                    // 00676809..006769C6
        index_list_append(fa(j, 0xc), index);                                           // 006769CE..006769D7
        if (rd(j, 0x40) & 1) kotor_fastcall_00676530(j, nullptr);                       // 006769DA..006769E6 -> 00676A72
    } else {
        const U index = rd(j, 0x24);                                                    // 006769EB ebp
        kotor_fastcall_005c5920(fa(j, 0x20), nullptr, copy_entry(arg));                 // 006769EE..00676A00
        index_list_grow(fa(j, 0x2c));                                                   // 00676A05..00676A56
        index_list_append(fa(j, 0x2c), index);                                          // 00676A5E..00676A67
        if (rd(j, 0x40) & 1) kotor_fastcall_00676530(fa(j, 0x20), nullptr);            // 00676A6A..00676A72
    }
}

// ===============================================================================================================
// Journal update notification
// ===============================================================================================================
// 005ED970 CClientExoApp::GetLoadBarState() RET 0
extern "C" U __fastcall kotor_fastcall_005ed970(void* client_app, void*) { return rd(rp(client_app, 4), 0x18c); } // 005ED970..005ED973

// 00675DE0 CExoArrayList<CSWCJournalPendingUpdate>::Allocate(int capacity) RET 4
extern "C" void __fastcall kotor_fastcall_00675de0(void* list, void*, U capacity) {
    void* const old = rp(list, 0);                                                      // 00675E00 ebx
    wr(list, 8, capacity);                                                              // 00675E11
    void* const block = live_operator_new(capacity * 0xc + 4);                          // 00675E02..00675E14
    void* arr = nullptr;
    if (block != nullptr) {                                                             // 00675E22..00675E28
        wr(block, 0, capacity);                                                         // 00675E3B
        arr = fa(block, 4);                                                             // 00675E35
        crt_eh_vector_constructor_006fb55f(arr, 0xc, static_cast<I>(capacity), reinterpret_cast<void*>(&kotor_fastcall_0059ea90),
                                           reinterpret_cast<void*>(&kotor_fastcall_0059ea30)); // 00675E2A..00675E3D
    }
    wp(list, 0, arr);                                                                   // 00675E45
    for (I i = 0; i < rdi(list, 4); ++i) {                                              // 00675E47..00675E86 (count re-read)
        void* const dst = fa(rp(list, 0), static_cast<U>(i) * 0xc);
        const void* const src = fa(old, static_cast<U>(i) * 0xc);
        str_assign(dst, src);                                                           // 00675E60..00675E67
        wr(dst, 8, rd(src, 8));                                                         // 00675E6C..00675E73
    }
    if (old != nullptr) {                                                               // 00675E8C..00675E8E
        c2da_destroy_array_006fa852(old, 0xc, rdi(old, static_cast<U>(-4)), reinterpret_cast<void*>(&kotor_fastcall_0059ea30)); // 00675E90..00675E9F
        live_free(fa(old, static_cast<U>(-4)));                                         // 00675EA4..00675EA5
    }
}

// 006764A0 CExoArrayList<CSWCJournalPendingUpdate>::Add(CSWCJournalPendingUpdate by value) RET 0Ch
extern "C" void __fastcall kotor_fastcall_006764a0(void* list, void*, PendingUpdateArg update) {
    struct ArgDtor { void* s; ~ArgDtor() { frame_LogStringDtor_005e5c20(s, nullptr); } } arg_dtor{&update.w[0]}; // 00676500..0067650C
    const U cap = rd(list, 8);
    if (rd(list, 4) == cap)                                                             // 006764B9..006764C7
        kotor_fastcall_00675de0(list, nullptr, cap != 0 ? cap + cap : 0x10);            // 006764C9..006764D9
    const U n = rd(list, 4);                                                            // 006764DE
    void* const slot = fa(rp(list, 0), n * 0xc);                                        // 006764E1..006764EA
    wr(list, 4, n + 1);                                                                 // 006764E6..006764E7
    str_assign(slot, &update.w[0]);                                                     // 006764ED..006764F4
    wr(slot, 8, update.w[2]);                                                           // 006764F9..006764FD
}

// 00676C20 CSWCJournal::JournalUpdated(int deferrable, int completed, CExoString const& text) RET 0Ch
extern "C" void __fastcall kotor_fastcall_00676c20(void* j, void*, U deferrable, U completed, const void* text) {
    if (kotor_fastcall_005ed970(client(), nullptr) != 0 && deferrable != 0) {          // 00676C3F..00676C53
        ExoString s;                                                                    // 00676C55..00676C59
        str_assign(&s, text);                                                           // 00676C5E..00676C6F
        PendingUpdateArg arg;
        (void)WindowCopy005e5b00(&arg.w[0], nullptr, &s);                               // 00676C74..00676C8C
        arg.w[2] = completed;                                                           // 00676C74 / 00676C88..00676C95
        kotor_fastcall_006764a0(fa(j, 0x44), nullptr, arg);                             // 00676C98..00676C9B
        return;                                                                         // 00676CA0..00676CC1
    }
    void* const memory = live_operator_new(0x34);                                       // 00676CC4..00676CCB
    void* const data = memory != nullptr ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr; // 00676CCE..00676CE9
    recovered_event_string_set_004d6ae0(data, nullptr, 0, text);                        // 00676CEB..00676CFB (null data faults)
    {
        ExoString empty("");                                                            // 00676D00..00676D09 (.rdata 0073D71C "")
        recovered_client_add_feedback_message_005ed360(client(), nullptr, 0xb8, data, &empty); // 00676D0E..00676D29
    }                                                                                   // 00676D2E..00676D36
    kotor_fastcall_0062eeb0(in_game_gui(), nullptr, 0, 0);                              // 00676D3B..00676D4E
    kotor_fastcall_0062eeb0(in_game_gui(), nullptr, completed != 0 ? 0xb : 0xa, 0);     // 00676D53..00676D7C
    kotor_cell_journal_updated_00833a94 = 1;                                            // 00676D86
}

// ===============================================================================================================
// CSWCWorldJournal
// ===============================================================================================================
// 00676370 SortItemsByDate(int a, int b, int no_name) RET 0Ch
extern "C" I __fastcall kotor_fastcall_00676370(void* wj, void*, I a, I b, U flag) {
    void* const timer = frame_GetWorldTimer_005ed4f0(client(), nullptr);               // 00676370..006763A4
    void* const ea = fa(rp(wj, 0), static_cast<U>(a) * kWorld);                        // 0067638E..00676391
    void* const eb = fa(rp(wj, 0), static_cast<U>(b) * kWorld);                        // 00676388..0067638B
    const I r = recovered_event_time_compare_004adfa0(timer, nullptr, rd(ea, 0x10), rd(ea, 0x14), rd(eb, 0x10),
                                                      rd(eb, 0x14));                   // 00676394..006763AB
    if (r == 0 && flag == 0) return kotor_fastcall_006763d0(wj, nullptr, a, b, 1);     // 006763B0..006763C2
    return r;
}

// 006763D0 SortItemsByName(int a, int b, int no_date) RET 0Ch (compares the +8 string)
extern "C" I __fastcall kotor_fastcall_006763d0(void* wj, void*, I a, I b, U flag) {
    const U oa = static_cast<U>(a) * kWorld;                                            // 006763E2..006763EC edi
    const U ob = static_cast<U>(b) * kWorld;                                            // 006763DC..006763E5 esi
    if (directory_list::Less005e5410(ss(fa(rp(wj, 0), oa + 8)), nullptr, ss(fa(rp(wj, 0), ob + 8))) != 0) return 1; // 006763DF..00676404
    if (alias_add::Greater005e5490(ss(fa(rp(wj, 0), oa + 8)), nullptr, ss(fa(rp(wj, 0), ob + 8))) != 0) return -1; // 00676408..00676421
    if (flag != 0) return 0;                                                            // 00676425..0067645F
    void* const timer = frame_GetWorldTimer_005ed4f0(client(), nullptr);               // 0067642D..0067644B
    void* const data = rp(wj, 0);
    return recovered_event_time_compare_004adfa0(timer, nullptr, rd(data, oa + 0x10), rd(data, oa + 0x14),
                                                 rd(data, ob + 0x10), rd(data, ob + 0x14)); // 00676435..00676452
}

// 00677000 CSWCWorldJournal::SortList() RET 0
extern "C" void __fastcall kotor_fastcall_00677000(void* wj, void*) {
    using Cmp = I(__fastcall*)(void*, void*, I, I, U);
    const Cmp cmp = rd8(wj, 0x18) != 0 ? &kotor_fastcall_006763d0 : &kotor_fastcall_00676370; // 00677007..00677016
    const I n = rdi(wj, 0x10);                                                          // 0067701E..00677025 (cached)
    selection_sort(wj, 0xc, n, [&](I x, I y) { return cmp(wj, nullptr, x, y, 0); });    // 0067702F..006770AB
}

namespace {
// Shared tail of AddEntry / AddEntryStrRef: append the entry copy, add its index to the sorted index, sort, dirty.
void world_add(void* wj, const void* entry) {
    const U index = rd(wj, 4);                                                          // 0067736B / 0067725A
    WorldJournalEntryArg copy;
    (void)kotor_fastcall_004c4080(&copy, nullptr, entry);                               // 0067737F..0067738F / 00677246..00677265
    kotor_fastcall_004c65e0(wj, nullptr, copy);                                         // 00677394..00677396 (callee destroys it)
    void* const idx = fa(wj, 0xc);
    const U cap = rd(idx, 8);
    if (rd(idx, 4) == cap)                                                              // 0067739B..006773A6
        menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(idx), nullptr,
                                            cap != 0 ? cap + cap : 0x10);              // 006773A8..006773B8
    wr(rp(idx, 0), rd(idx, 4) * 4, index);                                              // 006773BD..006773C2
    wr(idx, 4, rd(idx, 4) + 1);                                                         // 006773C5
    kotor_fastcall_00677000(wj, nullptr);                                               // 006773C8..006773CA
    wr(wj, 0x1c, rd(wj, 0x1c) | 1);                                                     // 006773CF
}
struct WorldEntryLocal {                     // local CWorldJournalEntry: strings at +0/+8; dwords +18h/+20h never initialised
    alignas(4) unsigned char raw[0x24];
    WorldEntryLocal() {
        (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(raw), nullptr);
        (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(raw + 8), nullptr);
    }
    WorldEntryLocal(const WorldEntryLocal&) = delete;
    WorldEntryLocal& operator=(const WorldEntryLocal&) = delete;
    ~WorldEntryLocal() { frame_LogStringDtor_005e5c20(raw + 8, nullptr); frame_LogStringDtor_005e5c20(raw, nullptr); }
};
}  // namespace

// 00677300 CSWCWorldJournal::AddEntry(index, CExoString const& s1, CExoString const& s2, t1, t2) RET 14h
extern "C" void __fastcall kotor_fastcall_00677300(void* wj, void*, U index, const void* s1, const void* s2, U t1, U t2) {
    WorldEntryLocal e;                                                                  // 0067731D..00677332 [esp+0Ch]
    wr(e.raw, 0x1c, index);                                                             // 0067733B..0067734C
    str_assign(e.raw, s1);                                                              // 00677337..00677350
    str_assign(e.raw + 8, s2);                                                          // 00677355..0067735E
    wr(e.raw, 0x14, t2);                                                                // 00677363..0067736E
    U bits;
    std::memcpy(&bits, e.raw + 0x20, 4);                                                // 00677372 (uninitialised local)
    wr(e.raw, 0x20, bits & 0xfffffffeu);                                                // 00677376..0067737C
    wr(e.raw, 0x10, t1);                                                                // 00677367 / 0067738B
    world_add(wj, e.raw);                                                               // 0067736B..006773CF
}                                                                                       // 006773D3..006773F0

// 00677130 CSWCWorldJournal::AddEntryStrRef(strref1, strref2, t1, t2) RET 10h
extern "C" void __fastcall kotor_fastcall_00677130(void* wj, void*, U strref1, U strref2, U t1, U t2) {
    ExoString name;                                                                     // 0067714E..00677152 [esp+2Ch]
    ResRef sound1;                                                                      // [esp+34h]
    resref_default(&sound1);                                                            // 00677157..00677163
    ExoString text;                                                                     // 0067716D..00677175 [esp+10h]
    ResRef sound2;                                                                      // [esp+18h]
    resref_default(&sound2);                                                            // 0067717A..00677183
    if (tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, strref1, &name, kByte0073ecd0) == 0) // 00677188..006771AC
        (void)frame_LogAssign_005e5140(&name, nullptr, "");                             // 006771AE..006771B7 (.rdata 0073D71C)
    if (tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, strref2, &text, kByte0073ecd0) == 0) // 006771BC..006771DF
        (void)frame_LogAssign_005e5140(&text, nullptr, "");                             // 006771E1..006771EA
    WorldEntryLocal e;                                                                  // 006771EF..00677201 [esp+48h]
    wr(e.raw, 0x1c, strref1);                                                           // 00677214 (strref2 is not stored)
    str_assign(e.raw, &name);                                                           // 00677206..00677218
    str_assign(e.raw + 8, &text);                                                       // 0067721D..00677226
    U bits;
    std::memcpy(&bits, e.raw + 0x20, 4);                                                // 0067722B (uninitialised local)
    wr(e.raw, 0x10, t1);                                                                // 0067722F / 00677240
    wr(e.raw, 0x20, bits | 1);                                                          // 00677244 / 00677253
    wr(e.raw, 0x14, t2);                                                                // 00677236 / 0067725E
    world_add(wj, e.raw);                                                               // 0067725A..006772A5
}                                                                                       // 006772A8..006772DD

// 00676F00 CSWCWorldJournal::DeleteEntry(DWORD index) RET 4 (iterates the sorted index; non-strref entries)
extern "C" void __fastcall kotor_fastcall_00676f00(void* wj, void*, U index) {
    I bound = rdi(wj, 4);                                                               // 00676F03 edx (entry count)
    for (I k = 0; k < bound; ++k) {                                                     // 00676F07..00676F85
        const I at = rdi(rp(wj, 0xc), static_cast<U>(k) * 4);                          // 00676F10..00676F13
        void* const e = fa(rp(wj, 0), static_cast<U>(at) * kWorld);                     // 00676F16..00676F20
        if ((rd8(e, 0x20) & 1) != 0 || rd(e, 0x1c) != index) continue;                  // 00676F1B..00676F2D
        kotor_fastcall_004c66b0(wj, nullptr, at);                                       // 00676F2F..00676F32
        const I removed = rdi(rp(wj, 0xc), static_cast<U>(k) * 4);                      // 00676F37..00676F3A ebp
        wr(wj, 0x10, rd(wj, 0x10) - 1);                                                 // 00676F3D..00676F47
        for (I e2 = k; e2 < rdi(wj, 0x10); ++e2)                                        // 00676F41..00676F62 (count re-read)
            wr(rp(wj, 0xc), static_cast<U>(e2) * 4, rd(rp(wj, 0xc), static_cast<U>(e2) * 4 + 4));
        bound = rdi(wj, 0x10);                                                          // 00676F64
        for (I c = 0; c < bound; ++c) {                                                 // 00676F67..00676F80
            const I v = rdi(rp(wj, 0xc), static_cast<U>(c) * 4);
            if (v > removed) wr(rp(wj, 0xc), static_cast<U>(c) * 4, static_cast<U>(v - 1)); // 00676F70..00676F7B
        }
    }
    wr(wj, 0x1c, rd(wj, 0x1c) | 1);                                                     // 00676F88..00676F8F
}

// 00676E60 CSWCWorldJournal::DeleteEntryStrRef(DWORD strref) RET 4 (iterates the entries; strref entries)
extern "C" void __fastcall kotor_fastcall_00676e60(void* wj, void*, U strref) {
    I bound = rdi(wj, 4);                                                               // 00676E65 edi
    for (I k = 0; k < bound; ++k) {                                                     // 00676E6A..00676EE6
        void* const e = fa(rp(wj, 0), static_cast<U>(k) * kWorld);                     // 00676E71..00676E77
        if ((rd8(e, 0x20) & 1) == 0 || rd(e, 0x1c) != strref) continue;                 // 00676E79..00676E85
        kotor_fastcall_004c66b0(wj, nullptr, k);                                        // 00676E87..00676E8A
        I c = 0;
        if (bound > 0) {                                                                // 00676E8F..00676E93
            do {                                                                        // 00676E95..00676EDE
                if (rdi(rp(wj, 0xc), static_cast<U>(c) * 4) == k) {                     // 00676E95..00676E9B
                    wr(wj, 0x10, rd(wj, 0x10) - 1);                                     // 00676E9D..00676EA7
                    for (I e2 = c; e2 < rdi(wj, 0x10); ++e2)                            // 00676EA1..00676EC2
                        wr(rp(wj, 0xc), static_cast<U>(e2) * 4, rd(rp(wj, 0xc), static_cast<U>(e2) * 4 + 4));
                    bound = rdi(wj, 0x10);                                              // 00676EC4
                }
                if (c != bound) {                                                       // 00676EC7..00676EC9
                    const I v = rdi(rp(wj, 0xc), static_cast<U>(c) * 4);
                    if (v > k) wr(rp(wj, 0xc), static_cast<U>(c) * 4, static_cast<U>(v - 1)); // 00676ECB..00676ED9
                }
                ++c;
            } while (c < bound);
        }
    }
    wr(wj, 0x1c, rd(wj, 0x1c) | 1);                                                     // 00676EE9..00676EF0
}

// 00676FA0 CSWCWorldJournal::DeleteAllEntries() RET 0
extern "C" void __fastcall kotor_fastcall_00676fa0(void* wj, void*) {
    void* const data = rp(wj, 0);                                                       // 00676FA4
    if (data != nullptr) {                                                              // 00676FA8..00676FAA
        c2da_destroy_array_006fa852(data, kWorld, rdi(data, static_cast<U>(-4)),
                                    reinterpret_cast<void*>(&world_journal_entry_destroy_004c4030)); // 00676FAC..00676FBC
        live_free(fa(data, static_cast<U>(-4)));                                        // 00676FC1..00676FC2
        wr(wj, 0, 0);                                                                   // 00676FCA
        wr(wj, 8, 0);                                                                   // 00676FCC
    }
    wr(wj, 4, 0);                                                                       // 00676FD0
    void* const idx = rp(wj, 0xc);                                                      // 00676FD3
    if (idx != nullptr) {                                                               // 00676FD6..00676FD8
        live_free(idx);                                                                 // 00676FDA..00676FDB
        wr(wj, 0xc, 0);                                                                 // 00676FE3
        wr(wj, 0x14, 0);                                                                // 00676FE6
    }
    wr(wj, 0x10, 0);                                                                    // 00676FE9
    wr(wj, 0x1c, rd(wj, 0x1c) | 1);                                                     // 00676FEC
}
