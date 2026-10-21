// lane-rules fork D: faithful bodies for CSWRules::LoadRaceInfo 0054FEC0,
// CSWRules::LoadSkillInfo 00550590, CSWRules::LoadFeatInfo 005515E0 and the element
// methods CSWRace 005C7890/005C78D0/005C7AD0, CSWFeat 005CD700, CSWSkill 005CD8A0,
// element dtors 0059D7C0 / 004015A0 (typed, D24) - swkotor.exe 9c10e045..., base 0x400000.
// Receivers are byte-offset views; every write cites its original store.
// EH: each original unwind state is mirrored by C++ scope (RAII StartupString temporaries,
// allocate_construct for the state-0 allocation funclets).
// Null C2DA after a failed 54h allocation: the original continues with ECX=0 into
// 004143B0 and then dereferences [0+24h] (e.g. 0054FF25..0054FF37); this is reproduced
// as-is (no invented guard). Reachable only when 006FA7E6 returns NULL.
#include "22ceec0ec1-race_skill_feat.h"
#include <new>

namespace {
using namespace kotor_rules;

// Uninitialised 8-byte storage for an original hidden-return / in-place CExoString
// (005E5D10 output, 005E5BC0 target); destroyed (005E5C20) iff constructed.
struct Fresh {
    alignas(String) unsigned char storage[sizeof(String)];
    bool live = false;
    String* raw() noexcept { return reinterpret_cast<String*>(storage); }
    String* get() noexcept { return std::launder(raw()); }
    ~Fresh() { if (live) get()->~String(); }
    Fresh() = default;
    Fresh(const Fresh&) = delete;
    Fresh& operator=(const Fresh&) = delete;
};

// new C2DA(CResRef(CExoString const&), 0): 00406D60 builds the by-value CResRef in the
// outgoing argument slot (005C7B10..005C7B22).
Table* new_c2da_from_string(String* name) {
    return static_cast<Table*>(allocate_construct(0x54, [name](void* p) {
        Ref r;                                                          // sub esp,10h; mov ecx,esp
        async_ref_construct_string_00406d60(&r, nullptr, name);          // 005C7B1B
        return static_cast<void*>(c2da_current_construct_00413cc0(p, nullptr, r, 0)); // 005C7B22
    }));
}

// eh-vector "new T[n]" with a 4-byte count header: alloc(n*size+4); [p]=n;
// 006FB55F(p+4, size, n, ctor, dtor). Returns p+4 or 0 (allocation failure).
unsigned char* new_vector(Word count, Word size, void* (__fastcall* ctor)(void*, void*),
                          void (__fastcall* dtor)(void*, void*)) {
    void* p = live_operator_new(count * size + 4);
    if (!p) return nullptr;
    try {
        wr32(p, 0, count);
        crt_eh_vector_constructor_006fb55f(bytes(p) + 4, size, static_cast<std::int32_t>(count), ctor, dtor);
    } catch (...) { live_free(p); throw; }                              // state 1 funclet frees the block
    return bytes(p) + 4;
}

// Low-byte bitfield update observed as  edx=v<<k; edx^=word; edx&=1<<k; word^=edx.
void set_bit(void* base, unsigned off, unsigned bit, std::int32_t v) noexcept {
    const Word w = rd32(base, off);
    wr32(base, off, w ^ (((static_cast<Word>(v) << bit) ^ w) & (1u << bit)));
}
}

namespace kotor_rules {

// ===================== 004015A0 (typed D24 equivalent of a bare RET) =====================
void __fastcall kotor_fastcall_004015a0(void*, void*) {}                // 004015A0 ret

// ===================== 0059D7C0: add ecx,10h; jmp 005E5C20 =====================
void __fastcall kotor_fastcall_0059d7c0(void* self, void*) {
    std::launder(reinterpret_cast<String*>(bytes(self) + 0x10))->~String(); // 0059D7C0/0059D7C3
}

// ===================== 005C7890 CSWRace::CSWRace =====================
void* __fastcall kotor_fastcall_005c7890(void* self, void*) {
    wr32(self, 0x00, 0xFFFFFFFFu);                                      // 005C7895
    wr32(self, 0x0C, 0xFFFFFFFFu);                                      // 005C7897
    wr32(self, 0x10, 0xFFFFFFFFu);                                      // 005C789A
    wr32(self, 0x14, 0xFFFFFFFFu);                                      // 005C789D
    for (unsigned off = 0x18; off <= 0x1E; ++off) wr8(self, off, 0);   // 005C78A2..005C78B4
    wr32(self, 0x28, 0);                                                // 005C78B7
    wr8(self, 0x1F, 0xFF);                                              // 005C78BA
    wr32(self, 0x20, 0);                                                // 005C78BE
    return self;                                                        // 005C7890 mov eax,ecx
}

// ===================== 005C78D0 CSWRace::~CSWRace =====================
void __fastcall kotor_fastcall_005c78d0(void* self, void*) {
    if (void* feats = ptr_at<void>(self, 0x28)) {                       // 005C78D3..005C78D8
        live_free(feats);                                               // 005C78DB 006FA390
        wr32(self, 0x28, 0);                                            // 005C78E3
    }
}

// ===================== 005C7AD0 CSWRace::LoadFeatsTable(CExoString*) =====================
void __fastcall kotor_fastcall_005c7ad0(void* self, void*, String* table_name) {
    Table* table = new_c2da_from_string(table_name);                    // 005C7AF1..005C7B33 (state 0)
    if (!c2da_current_load_004143b0(table, nullptr)) {                  // 005C7B3F/005C7B46
        // Diagnostic text built and discarded (no output call in this build).
        String message;                                                 // 005C7B50 005B3190 (state 1)
        {
            String prefix(kotor_rules_str_0074bf54);                     // 005C7B55..005C7B66 (state 2)
            Fresh sum;
            GuiConcat005e5d10(&prefix, nullptr, sum.raw(), table_name); // 005C7B6B..005C7B78
            sum.live = true;                                            // state 3
            message.Assign005e5c50(sum.get());                          // 005C7B7D..005C7B87
        }                                                               // 005C7B95 (sum) / 005C7BA3 (prefix)
        {
            String suffix(kotor_rules_str_0074b328);                     // 005C7BA8..005C7BB1 (state 4)
            Fresh sum;
            GuiConcat005e5d10(&message, nullptr, sum.raw(), &suffix);   // 005C7BB6..005C7BC9
            sum.live = true;                                            // state 5
            message.Assign005e5c50(sum.get());                          // 005C7BCE..005C7BD8
        }                                                               // 005C7BE6 (sum) / 005C7BF4 (suffix)
    }                                                                   // 005C7C05 (message)
    if (void* old = ptr_at<void>(self, 0x28)) {                         // 005C7C0A..005C7C11
        live_free(old);                                                 // 005C7C14
        wr32(self, 0x28, 0);                                            // 005C7C1C
    }
    const std::uint16_t count = static_cast<std::uint16_t>(rd32(table, 0x24)); // 005C7C1F/005C7C22 ax = rows
    wr16(self, 0x24, count);                                            // 005C7C28
    put_ptr(self, 0x28, live_operator_new(static_cast<Word>(count) * 2u)); // 005C7C25..005C7C39
    for (std::int32_t row = 0; row < static_cast<std::int32_t>(rd16(self, 0x24)); ++row) { // 005C7C34/005C7C94..005C7C9F (live reload)
        wr16(ptr_at<void>(self, 0x28), static_cast<unsigned>(row) * 2u, 0); // 005C7C40/005C7C4C
        std::int32_t value;                                             // [esp+14h], always written by 00413660
        const std::int32_t found = int_entry(table, row, kotor_rules_str_0074bf48, &value); // 005C7C43..005C7C7F (state 6)
        if (found)                                                      // 005C7C84
            wr16(ptr_at<void>(self, 0x28), static_cast<unsigned>(row) * 2u,
                 static_cast<std::uint16_t>(value));                    // 005C7C88..005C7C90
    }
    delete_c2da(table);                                                 // 005C7CA1..005C7CA7 [vtbl+0](1), no null test
}

// ===================== 005CD700 CSWFeat::CSWFeat =====================
void* __fastcall kotor_fastcall_005cd700(void* self, void*) {
    c2da_default_ref_00405ed0(reinterpret_cast<Ref*>(bytes(self) + 0x18), nullptr); // 005CD703/005CD706
    wr32(self, 0x08, 0xFFFFFFFFu);                                      // 005CD715
    wr32(self, 0x0C, 0xFFFFFFFFu);                                      // 005CD718
    wr16(self, 0x38, 0xFFFF);                                           // 005CD71B
    wr16(self, 0x3A, 0xFFFF);                                           // 005CD71F
    wr16(self, 0x46, 0xFFFF);                                           // 005CD723
    wr8(self, 0x2C, 0);                                                 // 005CD727
    wr8(self, 0x2E, 0);                                                 // 005CD72A
    wr8(self, 0x2F, 0);                                                 // 005CD72D
    wr8(self, 0x30, 0);                                                 // 005CD730
    wr8(self, 0x31, 0);                                                 // 005CD733
    wr32(self, 0x3C, 0xFFFFFFFFu);                                      // 005CD738
    wr32(self, 0x40, 0xFFFFFFFFu);                                      // 005CD73B
    wr16(self, 0x44, 0xFFFF);                                           // 005CD73E (cx of ecx=-1)
    wr32(self, 0x04, 0xFFFFFFFFu);                                      // 005CD742
    wr32(self, 0x00, 0xFFFFFFFFu);                                      // 005CD745
    // 005CD747..005CD756: [+28h] = ([+28h] & ~0Fh) | 10h. Only byte +28h changes; the
    // original preserves bits 8..31 (prior heap contents) - reproduced by a byte update.
    wr8(self, 0x28, static_cast<unsigned char>((rd8(self, 0x28) & 0xF0u) | 0x10u));
    wr8(self, 0x32, 0);                                                 // 005CD74D
    wr8(self, 0x33, 0);                                                 // 005CD750
    return self;                                                        // 005CD759
}

// ===================== 005CD8A0 CSWSkill::CSWSkill =====================
void* __fastcall kotor_fastcall_005cd8a0(void* self, void*) {
    ::new (static_cast<void*>(bytes(self) + 0x10)) String();            // 005CD8A3/005CD8A6 005B3190
    // 005CD8AB..005CD8B4: [+1Ch] = ([+1Ch] & ~7) | 38h (byte +1Ch only; bits 8..31 preserved).
    wr8(self, 0x1C, static_cast<unsigned char>((rd8(self, 0x1C) & 0xF8u) | 0x38u));
    wr32(self, 0x00, 0xFFFFFFFFu);                                      // 005CD8B7
    wr32(self, 0x04, 0xFFFFFFFFu);                                      // 005CD8BD
    return self;                                                        // 005CD8C4
}

// ===================== 0054FEC0 CSWRules::LoadRaceInfo =====================
void __fastcall kotor_fastcall_0054fec0(void* rules, void*) {
    Table* table = new_c2da(kotor_rules_str_00748994);                  // 0054FEE7..0054FF23 (state 0)
    c2da_current_load_004143b0(table, nullptr);                         // 0054FF32 (result ignored)
    const unsigned char count = static_cast<unsigned char>(rd32(table, 0x24)); // 0054FF37/0054FF3A
    wr8(rules, 0xAA, count);                                            // 0054FF3D
    unsigned char* races = new_vector(count, 0x34, &kotor_fastcall_005c7890, &kotor_fastcall_005c78d0); // 0054FF43..0054FF7A (state 1)
    put_ptr(rules, 0xB0, races);                                        // 0054FF94 (0 on allocation failure)
    for (std::int32_t row = 0; row < static_cast<std::int32_t>(rd8(rules, 0xAA)); ++row) { // 0054FF7F..0054FF9A / 00550554..00550561
        const unsigned elem = static_cast<unsigned>(row) * 0x34u;       // ebx
        auto race = [&]() { return bytes(ptr_at<void>(rules, 0xB0)) + elem; }; // [ebp+B0h]+ebx re-read per store
        String feats_table;                                             // 0054FFA2 005B3190 (state 2)
        std::int32_t a, b, found;                                       // [esp+18h], [esp+14h] (always written by 00413660)
        int_entry(table, row, kotor_rules_str_00745eec, &a); wr32(race(), 0x00, static_cast<Word>(a)); // 0054FFAB..00550004
        int_entry(table, row, kotor_rules_str_00748988, &a); wr32(race(), 0x04, static_cast<Word>(a)); // 00550006..0055004B
        int_entry(table, row, kotor_rules_str_00748978, &a); wr32(race(), 0x08, static_cast<Word>(a)); // 0055004E..00550093
        int_entry(table, row, kotor_rules_str_0074896c, &a); wr32(race(), 0x0C, static_cast<Word>(a)); // 00550096..005500DB
        int_entry(table, row, kotor_rules_str_007488c0, &a); wr32(race(), 0x10, static_cast<Word>(a)); // 005500DE..0055012C
        found = int_entry(table, row, kotor_rules_str_00748960, &a);    // 0055012F..00550167
        if (found) wr32(race(), 0x14, static_cast<Word>(a));            // 0055016C..00550180
        found = int_entry(table, row, kotor_rules_str_00748958, &b);    // 00550183..005501BB
        if (found) wr8(race(), 0x1F, static_cast<unsigned char>(b));    // 005501C0..005501D4
        found = int_entry(table, row, kotor_rules_str_0074894c, &b);    // 005501D7..0055020F
        if (found) wr32(race(), 0x20, b != 0 ? 1u : 0u);                // 00550214..0055022F setne
        int_entry(table, row, kotor_rules_str_00748940, &b); wr8(race(), 0x18, static_cast<unsigned char>(b)); // 00550232..00550280
        int_entry(table, row, kotor_rules_str_00748934, &b); wr8(race(), 0x19, static_cast<unsigned char>(b)); // 00550283..005502C8
        int_entry(table, row, kotor_rules_str_00748928, &b); wr8(race(), 0x1A, static_cast<unsigned char>(b)); // 005502CB..00550310
        int_entry(table, row, kotor_rules_str_0074891c, &b); wr8(race(), 0x1B, static_cast<unsigned char>(b)); // 00550313..00550358
        int_entry(table, row, kotor_rules_str_00748910, &b); wr8(race(), 0x1C, static_cast<unsigned char>(b)); // 0055035B..005503A0
        int_entry(table, row, kotor_rules_str_00748904, &b); wr8(race(), 0x1D, static_cast<unsigned char>(b)); // 005503A3..005503E8
        int_entry(table, row, kotor_rules_str_007488f8, &b); wr8(race(), 0x1E, static_cast<unsigned char>(b)); // 005503EB..00550439
        int_entry(table, row, kotor_rules_str_007488f4, &b); wr32(race(), 0x2C, static_cast<Word>(b));         // 0055043C..0055048A
        int_entry(table, row, kotor_rules_str_00746efc, &b); wr16(race(), 0x30, static_cast<std::uint16_t>(b)); // 0055048D..005504DC
        found = string_entry(table, row, kotor_rules_str_00748884, &feats_table); // 005504E0..00550521
        if (found)                                                      // 00550526..0055052C
            kotor_fastcall_005c7ad0(race(), nullptr, &feats_table);     // 0055052E..0055053B
    }                                                                   // 0055054F 005E5C20 (feats_table)
    delete_c2da(table);                                                 // 00550567..0055056D [vtbl+0](1), no null test
}

// ===================== 00550590 CSWRules::LoadSkillInfo =====================
void __fastcall kotor_fastcall_00550590(void* rules, void*) {
    Table* table = new_c2da(kotor_rules_str_00748a1c);                  // 005505B2..005505F0 (state 0)
    c2da_current_load_004143b0(table, nullptr);                         // 005505FF
    const unsigned char count = static_cast<unsigned char>(rd32(table, 0x24)); // 00550604/00550607
    wr8(rules, 0xAB, count);                                            // 0055060A
    unsigned char* skills = new_vector(count, 0x20, &kotor_fastcall_005cd8a0, &kotor_fastcall_0059d7c0); // 00550610..00550647 (state 1)
    put_ptr(rules, 0xB4, skills);                                       // 00550652
    for (std::int32_t row = 0; row < static_cast<std::int32_t>(rd8(rules, 0xAB)); ++row) { // 0055064C..00550667 / 00550B2D..00550B3A
        const unsigned elem = static_cast<unsigned>(row) * 0x20u;       // edi
        auto skill = [&]() { return bytes(ptr_at<void>(rules, 0xB4)) + elem; };
        String text;                                                    // 00550670 005B3190 [esp+18h]
        std::int32_t v;                                                 // [esp+14h]
        int_entry(table, row, kotor_rules_str_00745eec, &v); wr32(skill(), 0x00, static_cast<Word>(v)); // 00550679..005506C9
        int_entry(table, row, kotor_rules_str_007488c0, &v); wr32(skill(), 0x04, static_cast<Word>(v)); // 005506CB..00550710
        string_entry(table, row, kotor_rules_str_007488b8, &text);      // 00550713..00550747 (result ignored)
        std::launder(reinterpret_cast<String*>(skill() + 0x10))->Assign005e5c50(&text); // 0055074C..0055075C
        int_entry(table, row, kotor_rules_str_00748a10, &v); set_bit(skill(), 0x1C, 1, v); // 00550761..005507B3
        string_entry(table, row, kotor_rules_str_00748a04, &text);      // 005507B6..005507EA (result ignored)
        wr32(skill(), 0x18, 0);                                         // 00550800
        if (text.BorrowedEquals005e5290(kotor_rules_str_007487dc))      // 005507F7..0055080E
            wr32(skill(), 0x18, 0);                                     // 00550818
        else if (text.BorrowedEquals005e5290(kotor_rules_str_007487d8)) // 00550824..00550834
            wr32(skill(), 0x18, 1);                                     // 0055083E
        else if (text.BorrowedEquals005e5290(kotor_rules_str_007487d4)) // 0055084A..0055085A
            wr32(skill(), 0x18, 2);                                     // 00550864
        else if (text.BorrowedEquals005e5290(kotor_rules_str_007487d0)) // 0055086D..0055087D
            wr32(skill(), 0x18, 3);                                     // 00550887
        else if (text.BorrowedEquals005e5290(kotor_rules_str_007487cc)) // 00550890..005508A0
            wr32(skill(), 0x18, 4);                                     // 005508AA
        else if (text.BorrowedEquals005e5290(kotor_rules_str_007487c8)) // 005508B3..005508C3
            wr32(skill(), 0x18, 5);                                     // 005508CD
        int_entry(table, row, kotor_rules_str_007489f0, &v); set_bit(skill(), 0x1C, 2, v); // 005508D4..0055092B
        int_entry(table, row, kotor_rules_str_007489dc, &v); set_bit(skill(), 0x1C, 3, v); // 0055092E..0055098D
        int_entry(table, row, kotor_rules_str_007489d0, &v); wr32(skill(), 0x08, static_cast<Word>(v)); // 00550984..005509CC
        int_entry(table, row, kotor_rules_str_007489c8, &v); wr32(skill(), 0x0C, static_cast<Word>(v)); // 005509CF..00550A14
        int_entry(table, row, kotor_rules_str_007489b8, &v); set_bit(skill(), 0x1C, 0, v); // 00550A17..00550A73
        int_entry(table, row, kotor_rules_str_007489ac, &v); set_bit(skill(), 0x1C, 4, v); // 00550A6A..00550AC9
        int_entry(table, row, kotor_rules_str_007489a0, &v); set_bit(skill(), 0x1C, 5, v); // 00550AC0..00550B1A
    }                                                                   // 00550B28 005E5C20 (text)
    delete_c2da(table);                                                 // 00550B40..00550B46
}

// ===================== 005515E0 CSWRules::LoadFeatInfo =====================
void __fastcall kotor_fastcall_005515e0(void* rules, void*) {
    Table* table = new_c2da(kotor_rules_str_00748ca0);                  // 00551605..00551643 (state 0)
    c2da_current_load_004143b0(table, nullptr);                         // 00551652
    const std::uint16_t count = static_cast<std::uint16_t>(rd32(table, 0x24)); // 00551657/0055165A
    wr16(rules, 0xA4, count);                                           // 0055165D
    unsigned char* feats = new_vector(count, 0x48, &kotor_fastcall_005cd700, &kotor_fastcall_004015a0); // 00551664..0055169D (state 1)
    put_ptr(rules, 0x90, feats);                                        // 005516B7
    put_ptr(rules, 0xA0, live_operator_new(static_cast<Word>(rd16(rules, 0xA4)) * 2u)); // 005516A2..005516D0
    wr16(rules, 0xA6, 0);                                               // 005516D6
    for (std::int32_t row = 0; row < static_cast<std::int32_t>(rd16(rules, 0xA4)); ++row) { // 005516C9..005516DD / 00552023..00552030
        const unsigned elem = static_cast<unsigned>(row) * 0x48u;       // ebx
        auto feat = [&]() { return bytes(ptr_at<void>(rules, 0x90)) + elem; };
        String icon;                                                    // 005516E3 005B3190 [esp+50h] (state 2)
        String label(kotor_rules_str_00745eec);                         // 005516EC..00551700 [esp+40h] (state 3)
        if (c2da_current_column_index_00413100(table, nullptr, &label) == -1) // 00551705..0055171C
            GuiAssign005e5140(&label, nullptr, kotor_rules_str_00748ca0); // 0055171E..00551727
        std::int32_t v18;                                               // [esp+18h]
        if (!c2da_current_get_int_00413660(table, nullptr, row, &label, &v18)) { // 0055172C..00551740
            wr32(feat(), 0x28, rd32(feat(), 0x28) & 0xFFFFFFEFu);       // 00551FEC..00551FFB
            continue;                                                   // 00551FFE/0055200F (label, icon)
        }
        {
            const std::uint16_t n = rd16(rules, 0xA6);                  // 00551746
            wr16(ptr_at<void>(rules, 0xA0), static_cast<unsigned>(n) * 2u, static_cast<std::uint16_t>(row)); // 0055174D/00551753
            wr16(rules, 0xA6, static_cast<std::uint16_t>(n + 1));       // 00551761
            wr32(feat(), 0x08, static_cast<Word>(v18));                 // 00551757..00551776
        }
        int_entry(table, row, kotor_rules_str_007488c0, &v18);          // 0055176A..005517AA (state 4)
        wr32(feat(), 0x0C, static_cast<Word>(v18));                     // 005517AF..005517C7
        if (string_entry(table, row, kotor_rules_str_007488b8, &icon)) { // 005517BB..0055180A (state 5)
            Ref incoming;                                               // [esp+100h]
            incoming.construct_00406d80(WindowCStr005e5670(&icon, nullptr)); // 0055180C..0055181D
            Ref discarded;                                              // [esp+110h] (hidden-return slot)
            c2da_copy_00406120(reinterpret_cast<Ref*>(feat() + 0x18), nullptr, &discarded, &incoming); // 00551822..0055183D
        }
        std::int32_t v;                                                 // [esp+10h]
        std::int32_t found;
        int_entry(table, row, kotor_rules_str_007489d0, &v); wr32(feat(), 0x10, static_cast<Word>(v));          // 00551842..00551890 (6)
        int_entry(table, row, kotor_rules_str_007489c8, &v); wr32(feat(), 0x14, static_cast<Word>(v));          // 00551893..005518E1 (7)
        int_entry(table, row, kotor_rules_str_00748c90, &v); wr8(feat(), 0x2C, static_cast<unsigned char>(v));  // 005518E4..00551932 (8)
        int_entry(table, row, kotor_rules_str_00748c80, &v); wr8(feat(), 0x2D, static_cast<unsigned char>(v));  // 00551935..0055197A (9)
        int_entry(table, row, kotor_rules_str_00748c78, &v); wr8(feat(), 0x2E, static_cast<unsigned char>(v));  // 0055197D..005519CB (A)
        int_entry(table, row, kotor_rules_str_00748c70, &v); wr8(feat(), 0x2F, static_cast<unsigned char>(v));  // 005519CE..00551A1C (B)
        int_entry(table, row, kotor_rules_str_00748c68, &v); wr8(feat(), 0x30, static_cast<unsigned char>(v));  // 00551A1F..00551A64 (C)
        int_entry(table, row, kotor_rules_str_00748c60, &v); wr8(feat(), 0x31, static_cast<unsigned char>(v));  // 00551A67..00551AB5 (D)
        found = int_entry(table, row, kotor_rules_str_00748c54, &v);    // 00551AB8..00551AF9 (E)
        if (found) wr16(feat(), 0x38, static_cast<std::uint16_t>(v));   // 00551AFE..00551B13
        found = int_entry(table, row, kotor_rules_str_00748c48, &v);    // 00551B17..00551B58 (F)
        if (found) wr16(feat(), 0x3A, static_cast<std::uint16_t>(v));   // 00551B5D..00551B72
        {
            String column;                                              // 00551B76/00551B7A [esp+30h] (state 10h)
            for (std::int32_t k = 0; k < 5; ++k) {                      // 00551B87 / 00551C3E..00551C4A ([esp+14h])
                GuiAssign005e5140(&column, nullptr, kotor_rules_str_00748c3c); // 00551B90..00551B99
                {
                    Fresh number;                                       // [esp+E8h]
                    resman_string_from_int_005e5bc0(number.raw(), nullptr, k); // 00551B9E..00551BAA
                    number.live = true;                                 // state 11h
                    Fresh sum;                                          // [esp+F8h]
                    GuiConcat005e5d10(&column, nullptr, sum.raw(), number.get()); // 00551BAF..00551BCB
                    sum.live = true;                                    // state 12h
                    column.Assign005e5c50(sum.get());                   // 00551BD0..00551BDD
                }                                                       // 00551BF1 (sum) / 00551C05 (number)
                if (c2da_current_get_int_00413660(table, nullptr, row, &column, &v) && // 00551C0A..00551C1E
                    static_cast<unsigned char>(k) < 5)                  // 00551C26..00551C2F cmp cl,5 / jae
                    wr16(feat(), 0x3C + static_cast<unsigned>(static_cast<unsigned char>(k)) * 2u,
                         static_cast<std::uint16_t>(v));                // 00551C20/00551C31..00551C39
            }
            found = int_entry(table, row, kotor_rules_str_00748c30, &v); // 00551C50..00551C91 (13h)
            if (found) wr16(feat(), 0x46, static_cast<std::uint16_t>(v)); // 00551C96..00551CAB
            found = int_entry(table, row, kotor_rules_str_00748c24, &v); // 00551CAF..00551CE7 (14h)
            if (found) wr32(feat(), 0x00, static_cast<Word>(v));        // 00551CEC..00551D00
            int_entry(table, row, kotor_rules_str_00748c14, &v);        // 00551D02..00551D36 (15h)
            set_bit(feat(), 0x28, 0, v == 1 ? 1 : 0);                   // 00551D3B..00551D62 sete
            int_entry(table, row, kotor_rules_str_00748c08, &v);        // 00551D53..00551D94 (16h) value unused
            wr8(feat(), 0x32, 0);                                       // 00551D99..00551DAD
            int_entry(table, row, kotor_rules_str_007489dc, &v);        // 00551DA1..00551DE2 (17h)
            set_bit(feat(), 0x28, 1, v);                                // 00551DE7..00551E01
            found = int_entry(table, row, kotor_rules_str_00746d30, &v); // 00551E04..00551E45 (18h)
            if (found) wr32(feat(), 0x04, static_cast<Word>(v));        // 00551E4A..00551E5E
            found = int_entry(table, row, kotor_rules_str_00748bfc, &v); // 00551E61..00551EA2 (19h)
            if (found) wr8(feat(), 0x33, static_cast<unsigned char>(v)); // 00551EA7..00551EBB
            found = int_entry(table, row, kotor_rules_str_00748bf0, &v); // 00551EBE..00551EF6 (1Ah)
            wr8(feat(), 0x34, found ? static_cast<unsigned char>(v) : 0xFFu); // 00551EFB..00551F16
            found = int_entry(table, row, kotor_rules_str_00748be4, &v); // 00551F1A..00551F52 (1Bh)
            if (found) set_bit(feat(), 0x28, 2, v);                     // 00551F57..00551F7C
            int_entry(table, row, kotor_rules_str_00748bd8, &v);        // 00551F7F..00551FB3 (1Ch)
            set_bit(feat(), 0x28, 3, v);                                // 00551FB8..00551FD6
        }                                                               // 00551FE5 005E5C20 (column)
    }                                                                   // 0055200A (label) / 0055201E (icon)
    delete_c2da(table);                                                 // 00552036..0055203D

    Table* masters = new_c2da(kotor_rules_str_00748bcc);                // 0055203F..0055207E (state 1Dh)
    c2da_current_load_004143b0(masters, nullptr);                       // 0055208D
    const signed char n = static_cast<signed char>(rd32(masters, 0x24)); // 00552092/00552095 al
    wr8(rules, 0xA8, static_cast<unsigned char>(n));
    put_ptr(rules, 0x94, live_operator_new(static_cast<Word>(static_cast<std::int32_t>(n)) * 4u)); // 0055209B..005520B2 movsx
    put_ptr(rules, 0x98, live_operator_new(static_cast<Word>(static_cast<std::int32_t>(static_cast<signed char>(rd8(rules, 0xA8)))) * 4u)); // 005520A7..005520CA
    {
        const std::int32_t refs_count = static_cast<signed char>(rd8(rules, 0xA8)); // 005520BD
        void* refs = live_operator_new(static_cast<Word>(refs_count) << 4); // 005520C4..005520D0 (state 1Eh)
        if (refs) {                                                     // 005520DE/005520EB
            for (std::int32_t i = 0; i < refs_count; ++i)               // 005520ED..0055210C
                c2da_default_ref_00405ed0(reinterpret_cast<Ref*>(bytes(refs) + static_cast<unsigned>(i) * 16u), nullptr); // 005520F7..005520FB
        }
        put_ptr(rules, 0x9C, refs);                                     // 00552110/00552121
    }
    String name;                                                        // 00552112..00552127 [esp+48h] (state 1Fh)
    for (std::int32_t row = 0; row < static_cast<signed char>(rd8(rules, 0xA8)); ++row) { // 0055212C..00552141 / 00552234..00552241
        std::int32_t v;                                                 // [esp+1Ch]
        int_entry(masters, row, kotor_rules_str_0073e3c4, &v);          // 00552150..00552184 (20h)
        wr32(ptr_at<void>(rules, 0x94), static_cast<unsigned>(row) * 4u, static_cast<Word>(v)); // 00552189..0055219C
        int_entry(masters, row, kotor_rules_str_00748bc0, &v);          // 00552193..005521CA (21h)
        wr32(ptr_at<void>(rules, 0x98), static_cast<unsigned>(row) * 4u, static_cast<Word>(v)); // 005521CF..005521D9
        string_entry(masters, row, kotor_rules_str_00748bb8, &name);    // 005521DC..00552210 (22h) result ignored
        Ref discarded;                                                  // [esp+110h] hidden-return slot
        unresolved_ref_assign_00406290(reinterpret_cast<Ref*>(bytes(ptr_at<void>(rules, 0x9C)) + static_cast<unsigned>(row) * 16u),
                                       nullptr, &discarded, WindowCStr005e5670(&name, nullptr)); // 00552215..0055222F
    }
    delete_c2da(masters);                                               // 00552247..0055224E
}                                                                       // 0055225F 005E5C20 (name)
}
