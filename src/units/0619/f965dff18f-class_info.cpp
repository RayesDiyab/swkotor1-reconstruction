// lane-rules fork C: CSWRules::LoadClassInfo 0054F2C0, CSWRules::GetFeat 00550C00 and the
// CSWClass subtree 005BCAC0..005BE6CD (swkotor.exe GOG K1 sha256 9c10e045..., base 0x400000).
// D1 form (__thiscall -> __fastcall(self, unused EDX, stack args)), C++ linkage so the
// original EH unwind states (temporary CExoStrings, state-0 allocation funclets) are
// emitted as RAII / try (D30-FINAL). Receivers are byte-offset views of the original
// allocations; only cited offsets are touched.
//
// Observed quirks reproduced on purpose (address-cited below):
//  * C2DA::Load2DArray is called on the table pointer even when the 54h allocation
//    failed, and several bodies call the deleting destructor without a null test
//    (0054F332/0054FE98, 005BD15F/005BD445, 005BD72F/005BD8E1, 005BE528/005BE6B3).
//    The original faults on that path; the reconstruction performs the same calls.
//  * "Can't load" diagnostics are built into temporary CExoStrings and discarded
//    (the original's report call is compiled out); the string operations and their
//    allocations are reproduced.
//  * LoadSpellKnownTable leaves array slots for zero-count rows uninitialised
//    (005BDDA4 skips the store); ~CSWClass later frees every non-zero slot (005BCBE1).
#include "b71ed2c913-class_info.h"

namespace kotor_rules {
namespace {
// Output carrier for a hidden-return CExoString built by 005E5D10 (operator+): the
// callee constructs into the storage; destroyed (005E5C20) when it goes out of scope,
// i.e. in the original's unwind-state order.
struct Fresh {
    alignas(4) unsigned char storage[sizeof(String)];
    bool live = false;
    String* slot() noexcept { return reinterpret_cast<String*>(storage); }
    String* get() noexcept { return std::launder(reinterpret_cast<String*>(storage)); }
    Fresh() = default;
    Fresh(const Fresh&) = delete;
    Fresh& operator=(const Fresh&) = delete;
    ~Fresh() { if (live) get()->~StartupString(); }
};
String* concat(Fresh& out, String* left, String* right) {
    String* r = GuiConcat005e5d10(left, nullptr, out.slot(), right);   // 005E5D10 (constructs out)
    out.live = true;
    return r;
}
// " 2DA file" suffix used by every discarded "Can't load" message.
// Pattern A: s = String(prefix) + name; s = s + " 2DA file".
void discarded_message_a(const char* prefix, String* name) {
    String s;                                              // 005B3190
    {
        String t(prefix);                                  // 005E5A90
        Fresh c;
        s.Assign005e5c50(concat(c, &t, name));             // 005E5D10 / 005E5C50
    }                                                      // 005E5C20 x2 (c, then t)
    {
        String t(kotor_rules_str_0074b328);                // " 2DA file"
        Fresh c;
        s.Assign005e5c50(concat(c, &s, &t));
    }
}                                                          // 005E5C20 (s)
// Pattern B: s = String(prefix); s = s + " 2DA file".
void discarded_message_b(const char* prefix) {
    String s;
    {
        String t(prefix);
        s.Assign005e5c50(&t);
    }
    {
        String t(kotor_rules_str_0074b328);
        Fresh c;
        s.Assign005e5c50(concat(c, &s, &t));
    }
}
// Pattern C: s = String(prefix).
void discarded_message_c(const char* prefix) {
    String s;
    {
        String t(prefix);
        s.Assign005e5c50(&t);
    }
}
// new C2DA(CResRef(CExoString), 0): 006FA7E6 / 00406D60 / 00413CC0, state-0 funclet frees.
Table* new_c2da_from(String* name) {
    return static_cast<Table*>(allocate_construct(0x54, [name](void* p) {
        Ref r;                                             // sub esp,10h; mov ecx,esp
        async_ref_construct_string_00406d60(&r, nullptr, name);  // 00406D60
        return static_cast<void*>(c2da_current_construct_00413cc0(p, nullptr, r, 0));
    }));
}
// GetINTEntry with a column CExoString object.
std::int32_t int_entry_s(Table* t, std::int32_t row, String* column, std::int32_t* out) {
    return c2da_current_get_int_00413660(t, nullptr, row, column, out);
}
// GetINTEntry with column = prefix + suffix (005E5A90 suffix, 005E5D10, unwind order c, t).
std::int32_t int_entry_cat(Table* t, std::int32_t row, String* prefix, const char* suffix, std::int32_t* out) {
    String s(suffix);
    Fresh c;
    concat(c, prefix, &s);
    return int_entry_s(t, row, c.get(), out);
}
unsigned char* cls(void* rules, std::int32_t index) noexcept {    // [rules+ACh] + index*1A0h (re-read per use)
    return ptr_at<unsigned char>(rules, 0xAC) + static_cast<Word>(index) * 0x1A0u;
}
}

// ---------------------------------------------------------------------------
// 00550C00 CSWRules::GetFeat
void* __fastcall kotor_fastcall_00550c00(void* rules, void*, std::uint16_t feat) {
    if (feat >= rd16(rules, 0xA4)) return nullptr;                          // 00550C00..00550C0C jae -> xor
    unsigned char* entry = ptr_at<unsigned char>(rules, 0x90) + static_cast<Word>(feat) * 0x48u; // 00550C0E..00550C1A
    if (rd8(entry, 0x28) & 0x10) return entry;                              // 00550C1D/00550C21
    return nullptr;                                                         // 00550C23
}

// ---------------------------------------------------------------------------
// 005BCAC0 CSWClass::CSWClass (store order as observed)
void* __fastcall kotor_fastcall_005bcac0(void* self, void*) {
    unsigned char* b = bytes(self);
    ::new (b + 0x10) String();                                              // 005BCAC4/005BCAC7 005B3190
    wr32(b, 0x00, 0xFFFFFFFFu); wr32(b, 0x08, 0xFFFFFFFFu); wr32(b, 0x0C, 0xFFFFFFFFu); // 005BCACC..005BCAD4
    std::memset(b + 0x18, 0, 0x0F * 4);                                     // 005BCAD7..005BCAE1 rep stosd
    for (unsigned i = 0; i < 0x14; ++i) {                                   // 005BCAE3..005BCAFD (eax = +183h + i)
        b[0x128 + i] = 0; b[0x183 + i] = 0; b[0x13C + i] = 0; b[0x150 + i] = 0;
    }
    wr32(b, 0x17B, 0); wr16(b, 0x17F, 0);                                   // 005BCB01/005BCB07
    std::memset(b + 0x57, 0, 0x0F * 4);                                     // 005BCB0E..005BCB16
    std::memset(b + 0x93, 0, 0x0F * 4);                                     // 005BCB18..005BCB23
    std::memset(b + 0xCF, 0, 0x0F * 4);                                     // 005BCB25..005BCB30
    for (unsigned o = 0; o < 0x14; o += 4) wr32(b, 0x114 + o, 0);           // 005BCB34..005BCB45
    for (unsigned o = 0; o < 0x14; o += 4) wr32(b, 0x128 + o, 0);           // 005BCB48..005BCB59
    wr32(b, 0x164, 0); wr32(b, 0x110, 0); wr32(b, 0x174, 0); wr32(b, 0x10C, 0); // 005BCB5D..005BCB6F
    b[0x54] = 0; wr16(b, 0x168, 0); wr32(b, 0x198, 0); wr32(b, 0x16C, 0);   // 005BCB75..005BCB85
    wr16(b, 0x170, 0); wr32(b, 0x19C, 0);                                   // 005BCB8B/005BCB92
    b[0x17A] = 0; b[0x181] = 0; b[0x182] = 0;                               // 005BCB98..005BCBA4
    return self;                                                            // 005BCBAA
}

// 005BCBB0 CSWClass::~CSWClass (EH state 0 = member CExoString +10h)
void __fastcall kotor_fastcall_005bcbb0(void* self, void*) {
    unsigned char* b = bytes(self);
    if (rd32(b, 0x110) != 0) {                                              // 005BCBCE..005BCBDC
        for (unsigned o = 0; o < 0x50; o += 4) {                            // 005BCBDF..005BCC06
            void* p = ptr_at<void>(ptr_at<unsigned char>(b, 0x110), o);     // 005BCBE1/005BCBE7 (re-read)
            if (p) { live_free(p); wr32(ptr_at<unsigned char>(b, 0x110), o, 0); } // 005BCBEE..005BCBFD
        }
        live_free(ptr_at<void>(b, 0x110)); wr32(b, 0x110, 0);               // 005BCC08..005BCC17
    }
    const unsigned owned[4] = {0x164u, 0x16Cu, 0x10Cu, 0x174u};
    for (unsigned off : owned) {                                            // 005BCC1E..005BCC7C
        if (void* p = ptr_at<void>(b, off)) { live_free(p); wr32(b, off, 0); }
    }
    std::launder(reinterpret_cast<String*>(b + 0x10))->~StartupString();   // 005BCC82..005BCC8D
}

// ---------------------------------------------------------------------------
// 005BCDA0 CSWClass::LoadAttackBonusTable(CExoString* table), RET 4
void __fastcall kotor_fastcall_005bcda0(void* self, void*, String* name) {
    String unused;                                                          // 005BCDC5/005BCDC9 [esp+30h] (state 0, never used)
    Table* t = new_c2da_from(name);                                         // 005BCDD0..005BCE07 (state 1 funclet)
    if (!c2da_current_load_004143b0(t, nullptr))                            // 005BCE10/005BCE17
        discarded_message_a(kotor_rules_str_0074b334, name);                // 005BCE1D..005BCED0
    for (std::int32_t row = 0; row < 0x3C; ++row) {                         // 005BCED5..005BCF2E
        std::int32_t v;                                                     // [esp+10h], always written by 00413660
        const std::int32_t found = int_entry(t, row, kotor_rules_str_0074b324, &v); // "BAB" 005BCEE0..005BCF10
        bytes(self)[0x18 + row] = found ? static_cast<unsigned char>(v) : 0; // 005BCF1D..005BCF26 sete/dec/and
    }
    if (t) delete_c2da(t);                                                  // 005BCF30..005BCF3A
}                                                                           // 005BCF48 ~unused

// 005BCF70 CSWClass::LoadFeatGain(CExoString* prefix), RET 4
void __fastcall kotor_fastcall_005bcf70(void* self, void*, String* prefix) {
    Table* t = new_c2da(kotor_rules_str_0074b3a0);                          // "featgain" 005BCF91..005BCFC8
    if (!c2da_current_load_004143b0(t, nullptr)) {                          // 005BCFD4/005BCFDB
        String discarded(kotor_rules_str_0074b370);                         // 005BCFDD..005BCFEF (constructed and destroyed)
    }
    Fresh reg;                                                              // [esp+28h]
    {
        String s(kotor_rules_str_0074b368);                                 // "_REG" 005BCFF4
        concat(reg, prefix, &s);                                            // 005BD019
    }                                                                       // 005BD027
    Fresh bon;                                                              // [esp+20h]
    {
        String s(kotor_rules_str_0074b360);                                 // "_BON" 005BD02C
        concat(bon, prefix, &s);                                            // 005BD04B
    }                                                                       // 005BD059
    for (std::int32_t row = 0; row < 0x14; ++row) {                         // 005BD05E..005BD0A6
        std::int32_t v;                                                     // [esp+0Ch]
        if (int_entry_s(t, row, reg.get(), &v))                             // 005BD060..005BD074
            bytes(self)[0x150 + row] = static_cast<unsigned char>(v);       // 005BD076/005BD07A
        if (int_entry_s(t, row, bon.get(), &v))                             // 005BD081..005BD095
            bytes(self)[0x13C + row] = static_cast<unsigned char>(v);       // 005BD097/005BD09B
    }
    if (t) delete_c2da(t);                                                  // 005BD0A8..005BD0B2
}                                                                           // 005BD0BD ~bon, 005BD0CE ~reg

// 005BD0F0 CSWClass::LoadFeatsTable(CExoString* prefix, CSWRules* rules), RET 8.
// Entry array +16Ch (8-byte entries: WORD feat, BYTE +2 granted level, BYTE +3 list kind, DWORD +4 recommended);
// WORD +170h = row count.
void __fastcall kotor_fastcall_005bd0f0(void* self, void*, String* prefix, void* rules) {
    unsigned char* b = bytes(self);
    Table* t = new_c2da(kotor_rules_str_00748ca0);                          // "Feat" 005BD111..005BD154 ([esp+10h])
    if (!c2da_current_load_004143b0(t, nullptr))                            // 005BD15F/005BD166 (no null test)
        discarded_message_b(kotor_rules_str_0074b3c8);                      // 005BD16C..005BD201
    if (void* old = ptr_at<void>(b, 0x16C)) { live_free(old); wr32(b, 0x16C, 0); } // 005BD206..005BD21B
    const Word rows = rd32(t, 0x24);                                        // 005BD221
    wr16(b, 0x170, static_cast<std::uint16_t>(rows));                       // 005BD22B
    wr32(b, 0x16C, static_cast<Word>(reinterpret_cast<std::uintptr_t>(
        live_operator_new(static_cast<Word>(static_cast<std::uint16_t>(rows)) * 8u)))); // 005BD224..005BD242 (no null test)
    Word out = 0;                                                           // edi: committed-entry byte offset
    for (std::int32_t row = 0; row < static_cast<std::int32_t>(rd16(b, 0x170)); ++row) { // 005BD23A/005BD431..005BD43B
        unsigned char* a = ptr_at<unsigned char>(b, 0x16C);
        wr16(a, static_cast<Word>(row) * 8u, 0);                            // 005BD250..005BD256 (entry[row])
        ptr_at<unsigned char>(b, 0x16C)[static_cast<Word>(row) * 8u + 2] = 0; // 005BD262
        ptr_at<unsigned char>(b, 0x16C)[static_cast<Word>(row) * 8u + 3] = 0; // 005BD26D
        wr16(ptr_at<unsigned char>(b, 0x16C), out, 0);                      // 005BD278 (entry at cursor)
        ptr_at<unsigned char>(b, 0x16C)[out + 2] = 0;                       // 005BD284
        ptr_at<unsigned char>(b, 0x16C)[out + 3] = 0;                       // 005BD28F
        wr16(ptr_at<unsigned char>(b, 0x16C), out, static_cast<std::uint16_t>(row)); // 005BD29A
        const std::uint16_t feat = rd16(ptr_at<unsigned char>(b, 0x16C), out);       // 005BD2A4..005BD2A6
        if (!kotor_fastcall_00550c00(rules, nullptr, feat)) continue;      // 005BD2AA..005BD2B5
        std::int32_t v;                                                     // [esp+14h]
        int_entry_cat(t, row, prefix, kotor_rules_str_0074b3c0, &v);        // "_List" 005BD2BB..005BD312 (result unused)
        switch (static_cast<unsigned char>(v)) {                            // 005BD317..005BD321 jump table 005BD464
        case 0: ptr_at<unsigned char>(b, 0x16C)[out + 3] = 1; break;        // 005BD328
        case 1: ptr_at<unsigned char>(b, 0x16C)[out + 3] = 3; break;        // 005BD335
        case 2: ptr_at<unsigned char>(b, 0x16C)[out + 3] = 2; break;        // 005BD342
        case 3: ptr_at<unsigned char>(b, 0x16C)[out + 3] = 0; break;        // 005BD34F
        case 4: continue;                                                   // 005BD431: entry not committed
        default: break;                                                     // 005BD31F ja 005BD35A
        }
        int_entry_cat(t, row, prefix, kotor_rules_str_0074b3b4, &v);        // "_Granted" 005BD35A..005BD3B1
        ptr_at<unsigned char>(b, 0x16C)[out + 2] = static_cast<unsigned char>(v); // 005BD3B6..005BD3C9
        int_entry_cat(t, row, prefix, kotor_rules_str_0074b3ac, &v);        // "_Recom" 005BD3C0..005BD41B
        wr32(ptr_at<unsigned char>(b, 0x16C), out + 4, static_cast<Word>(v)); // 005BD420..005BD42A
        out += 8;                                                           // 005BD42E
    }
    delete_c2da(t);                                                         // 005BD441..005BD44B (no null test)
}

// 005BD480 CSWClass::LoadSavingThrowTable(CExoString* table), RET 4
void __fastcall kotor_fastcall_005bd480(void* self, void*, String* name) {
    Table* t = new_c2da_from(name);                                         // 005BD4A1..005BD4DE
    if (!c2da_current_load_004143b0(t, nullptr))                            // 005BD4EA/005BD4F1
        discarded_message_a(kotor_rules_str_0074b418, name);                // 005BD4F7..005BD5B0
    for (std::int32_t row = 0; row < 0x3C; ++row) {                         // 005BD5B5..005BD68D
        std::int32_t v;                                                     // [esp+0Ch]
        int_entry(t, row, kotor_rules_str_0074b40c, &v);                    // "FortSave" 005BD5C0..005BD5F4
        bytes(self)[0x57 + row] = static_cast<unsigned char>(v);            // 005BD5F9/005BD606
        int_entry(t, row, kotor_rules_str_0074b400, &v);                    // "WillSave" 005BD5FD..005BD635
        bytes(self)[0x93 + row] = static_cast<unsigned char>(v);            // 005BD63A/005BD647
        int_entry(t, row, kotor_rules_str_0074b3f8, &v);                    // "RefSave" 005BD63E..005BD679
        bytes(self)[0xCF + row] = static_cast<unsigned char>(v);            // 005BD67E/005BD682
    }
    if (t) delete_c2da(t);                                                  // 005BD693..005BD69D
}

// 005BD6C0 CSWClass::LoadSkillsTable(CExoString* prefix), RET 4.
// Entry array +164h (12-byte entries: WORD +0 row, DWORD +4 class skill, DWORD +8 recommended or -1);
// WORD +168h = row count.
void __fastcall kotor_fastcall_005bd6c0(void* self, void*, String* prefix) {
    unsigned char* b = bytes(self);
    Table* t = new_c2da(kotor_rules_str_0074b488);                          // "skills" 005BD6E1..005BD724 ([esp+10h])
    if (!c2da_current_load_004143b0(t, nullptr))                            // 005BD72F/005BD736 (no null test)
        discarded_message_c(kotor_rules_str_0074b454);                      // 005BD738..005BD77C
    if (void* old = ptr_at<void>(b, 0x164)) { live_free(old); wr32(b, 0x164, 0); } // 005BD781..005BD796
    const Word rows = rd32(t, 0x24);                                        // 005BD79C
    wr16(b, 0x168, static_cast<std::uint16_t>(rows));                       // 005BD79F
    wr32(b, 0x164, static_cast<Word>(reinterpret_cast<std::uintptr_t>(
        live_operator_new(static_cast<Word>(static_cast<std::uint16_t>(rows)) * 12u)))); // 005BD7A6..005BD7C0 (no null test)
    for (std::int32_t row = 0; row < static_cast<std::int32_t>(rd16(b, 0x168)); ++row) { // 005BD7B8/005BD8CA..005BD8D7
        const Word e = static_cast<Word>(row) * 12u;                        // ebx
        wr16(ptr_at<unsigned char>(b, 0x164), e, static_cast<std::uint16_t>(row)); // 005BD7D0/005BD7D6
        std::int32_t v;                                                     // [esp+14h]
        int_entry_cat(t, row, prefix, kotor_rules_str_0074b44c, &v);        // "_Class" 005BD7DA..005BD831
        wr32(ptr_at<unsigned char>(b, 0x164), e + 4, static_cast<Word>(v)); // 005BD836..005BD849 (unconditional)
        const std::int32_t found = int_entry_cat(t, row, prefix, kotor_rules_str_0074b444, &v); // "_Reco" 005BD840..005BD89F
        wr32(ptr_at<unsigned char>(b, 0x164), e + 8, found ? static_cast<Word>(v) : 0xFFFFFFFFu); // 005BD8A4..005BD8C2
    }
    delete_c2da(t);                                                         // 005BD8DD..005BD8E7 (no null test)
}

// 005BD900 CSWClass::LoadSpellGainTable(CExoString* column), RET 4
void __fastcall kotor_fastcall_005bd900(void* self, void*, String* column) {
    if (column->BorrowedLength005e5790() < 1) return;                       // 005BD924..005BD930 jl -> 005BDBC3
    Table* levels = new_c2da(kotor_rules_str_0074b520);                     // "CLS_SPGN_JEDI" 005BD936..005BD970 ([esp+14h])
    if (!c2da_current_load_004143b0(levels, nullptr))                       // 005BD978/005BD97F
        discarded_message_b(kotor_rules_str_0074b4e8);                      // 005BD985..005BDA1A
    Table* gain = new_c2da(kotor_rules_str_0074b4d8);                       // "ClassPowerGain" 005BDA1F..005BDA5D ([esp+0Ch], state 5)
    if (!c2da_current_load_004143b0(gain, nullptr))                         // 005BDA65/005BDA74
        discarded_message_b(kotor_rules_str_0074b4a0);                      // 005BDA7A..005BDB0E
    unsigned char* count = bytes(self) + 0x114;                             // 005BDB13..005BDB19 esi
    for (std::int32_t row = 0; row < 0x14; ++row, ++count) {                // 005BDB17/005BDB9C..005BDBA1
        std::int32_t v;                                                     // [esp+18h]
        int_entry(levels, row, kotor_rules_str_0074b490, &v);               // "NumSpellLevels" 005BDB20..005BDB56
        count[0] = static_cast<unsigned char>(v);                           // 005BDB5B/005BDB61
        if (count[0] == 0) continue;                                        // 005BDB5F/005BDB63
        for (std::int32_t i = 0; i < static_cast<std::int32_t>(count[0]); ++i) { // 005BDB65..005BDB9A (byte reloaded)
            if (int_entry_s(gain, row, column, &v))                         // 005BDB70..005BDB85
                count[0x14] = static_cast<unsigned char>(v);                // 005BDB87/005BDB8B
            else
                count[0x14] = 0xFF;                                         // 005BDB90
        }
    }
    if (gain) delete_c2da(gain);                                            // 005BDBA7..005BDBB3
    if (levels) delete_c2da(levels);                                        // 005BDBB5..005BDBC1
}

// 005BDBE0 CSWClass::LoadSpellKnownTable(CExoString* table), RET 4
void __fastcall kotor_fastcall_005bdbe0(void* self, void*, String* name) {
    unsigned char* b = bytes(self);
    String column;                                                          // 005BDC03/005BDC07 [esp+28h] (state 0)
    Table* t = new_c2da_from(name);                                         // 005BDC0C..005BDC4E ([esp+10h])
    if (!c2da_current_load_004143b0(t, nullptr))                            // 005BDC5F/005BDC66
        discarded_message_a(kotor_rules_str_0074b540, name);                // 005BDC6C..005BDD1F
    if (rd32(b, 0x110) != 0) {                                              // 005BDD24..005BDD2C
        for (unsigned o = 0; o < 0x50; o += 4) {                            // 005BDD2E..005BDD59
            if (void* p = ptr_at<void>(ptr_at<unsigned char>(b, 0x110), o)) {
                live_free(p); wr32(ptr_at<unsigned char>(b, 0x110), o, 0);  // 005BDD3D..005BDD4C
            }
        }
        live_free(ptr_at<void>(b, 0x110)); wr32(b, 0x110, 0);               // 005BDD5B..005BDD6A
    }
    put_ptr(b, 0x110, live_operator_new(0x50));                             // 005BDD74..005BDD7B (no null test)
    for (std::int32_t row = 0; row < 0x14; ++row) {                         // 005BDD81..005BDE45 (edi = +114h + row)
        const unsigned char n = b[0x114 + row];                             // 005BDDA0
        if (n == 0) continue;                                               // 005BDDA2/005BDDA4 (slot left as allocated)
        wr32(ptr_at<unsigned char>(b, 0x110), static_cast<Word>(row) * 4u,
             static_cast<Word>(reinterpret_cast<std::uintptr_t>(live_operator_new(n)))); // 005BDDAA..005BDDBD
        for (std::int32_t level = 0; level < static_cast<std::int32_t>(b[0x114 + row]); ++level) { // 005BDDC5..005BDE2E
            kotor_reconstruction::StartupFormat005e5680(&column, kotor_rules_str_0074b530, level); // "SpellLevel%d" 005BDDD0..005BDDDB
            std::int32_t v;                                                 // [esp+1Ch]
            if (int_entry_s(t, row, &column, &v))                           // 005BDDE0..005BDDFF
                ptr_at<unsigned char>(ptr_at<unsigned char>(b, 0x110), static_cast<Word>(row) * 4u)[level] =
                    static_cast<unsigned char>(v);                          // 005BDE01..005BDE12
            else
                ptr_at<unsigned char>(ptr_at<unsigned char>(b, 0x110), static_cast<Word>(row) * 4u)[level] = 0; // 005BDE17..005BDE24
        }
    }
    if (t) delete_c2da(t);                                                  // 005BDE4B..005BDE57
}                                                                           // 005BDE65 ~column

// 005BDE80 CSWClass::LoadArmorClassTable(CExoString* column), RET 4
void __fastcall kotor_fastcall_005bde80(void* self, void*, String* column) {
    unsigned char* b = bytes(self);
    Table* t = new_c2da(kotor_rules_str_0074b5a0);                          // "ACBonus" 005BDE9C..005BDED3
    if (!c2da_current_load_004143b0(t, nullptr))                            // 005BDEDE/005BDEE5
        discarded_message_b(kotor_rules_str_0074b56c);                      // 005BDEEB..005BDF80
    if (void* old = ptr_at<void>(b, 0x10C)) { live_free(old); wr32(b, 0x10C, 0); } // 005BDF85..005BDF9A
    put_ptr(b, 0x10C, live_operator_new(0x15));                             // 005BDFA0..005BDFAE (no null test)
    for (std::int32_t row = 0; row <= 0x14; ++row) {                        // 005BDFB4..005BDFE3 (jle 14h)
        std::int32_t v;                                                     // [esp+10h]
        if (int_entry_s(t, row, column, &v))                                // 005BDFB4..005BDFC4
            ptr_at<unsigned char>(b, 0x10C)[row] = static_cast<unsigned char>(v); // 005BDFC6..005BDFD0
        else
            ptr_at<unsigned char>(b, 0x10C)[row] = 0;                       // 005BDFD5..005BDFDB
    }
    if (t) delete_c2da(t);                                                  // 005BDFE5..005BDFF0
}

// 005BE4C0 CSWClass::LoadSpellsTable, RET.
// Entry array +174h (16-byte entries: WORD +0 row, DWORD +4 light, DWORD +8 dark, BYTE +0Ch user type);
// WORD +178h = row count.
void __fastcall kotor_fastcall_005be4c0(void* self, void*) {
    unsigned char* b = bytes(self);
    Table* t = new_c2da(kotor_rules_str_0074b5f4);                          // "spells" 005BE4E2..005BE51D
    if (!c2da_current_load_004143b0(t, nullptr))                            // 005BE528/005BE52F (no null test)
        discarded_message_c(kotor_rules_str_0074b5c0);                      // 005BE531..005BE575
    if (void* old = ptr_at<void>(b, 0x174)) { live_free(old); wr32(b, 0x174, 0); } // 005BE57A..005BE58D
    const Word rows = rd32(t, 0x24);                                        // 005BE593
    wr16(b, 0x178, static_cast<std::uint16_t>(rows));                       // 005BE596
    put_ptr(b, 0x174, live_operator_new(static_cast<Word>(static_cast<std::uint16_t>(rows)) << 4)); // 005BE59D..005BE5B5
    for (std::int32_t row = 0; row < static_cast<std::int32_t>(rd16(b, 0x178)); ++row) { // 005BE5AE/005BE6A0..005BE6AD
        const Word e = static_cast<Word>(row) * 16u;                        // ebp
        wr16(ptr_at<unsigned char>(b, 0x174), e, static_cast<std::uint16_t>(row)); // 005BE5C1/005BE5C7
        std::int32_t v;                                                     // [esp+14h]
        int_entry(t, row, kotor_rules_str_0074b5b4, &v);                    // "Light_Recom" 005BE5CB..005BE5FF
        wr32(ptr_at<unsigned char>(b, 0x174), e + 4, static_cast<Word>(v)); // 005BE604..005BE60E
        int_entry(t, row, kotor_rules_str_0074b5a8, &v);                    // "Dark_Recom" 005BE612..005BE646
        wr32(ptr_at<unsigned char>(b, 0x174), e + 8, static_cast<Word>(v)); // 005BE64B..005BE65E
        int_entry(t, row, kotor_rules_str_0074a1fc, &v);                    // "UserType" 005BE655..005BE68D
        ptr_at<unsigned char>(b, 0x174)[e + 0x0C] = static_cast<unsigned char>(v); // 005BE692..005BE69C
    }
    delete_c2da(t);                                                         // 005BE6B3..005BE6B9 (no null test)
}

// ---------------------------------------------------------------------------
// 0054F2C0 CSWRules::LoadClassInfo. CSWRules: BYTE +A9h class count, +ACh CSWClass[] (new[] with
// count cookie, 1A0h stride, ctor 005BCAC0 / dtor 005BCBB0 via 006FB55F).
void __fastcall kotor_fastcall_0054f2c0(void* rules, void*) {
    Table* t = new_c2da(kotor_rules_str_007488ec);                          // "CLASSES" 0054F2E7..0054F323 (edi; state 0 funclet)
    c2da_current_load_004143b0(t, nullptr);                                 // 0054F332 (result ignored, no null test)
    const unsigned char count = static_cast<unsigned char>(rd32(t, 0x24));  // 0054F337..0054F33A
    wr8(rules, 0xA9, count);                                                // 0054F33D
    const Word n = count;                                                   // esi = movzx
    void* block = allocate_construct(n * 0x1A0u + 4u, [n](void* p) {        // 0054F343..0054F368 (state 1 funclet)
        wr32(p, 0, n);                                                      // 0054F37E [eax] = count
        crt_eh_vector_constructor_006fb55f(bytes(p) + 4, 0x1A0, static_cast<std::int32_t>(n),
                                           &kotor_fastcall_005bcac0, &kotor_fastcall_005bcbb0); // 0054F36A..0054F380
        return p;
    });
    put_ptr(rules, 0xAC, block ? bytes(block) + 4 : nullptr);               // 0054F375/0054F39A (ebx: eax+4 or 0)
    for (std::int32_t i = 0; i < static_cast<std::int32_t>(rd8(rules, 0xA9)); ++i) { // 0054F385..0054F3A0 / 0054FE82..0054FE92
        String text;                                                        // 0054F3B0/0054F3B4 [esp+1Ch] (state 2)
        std::int32_t v;                                                     // [esp+24h]/[esp+2Ch]/[esp+28h]/[esp+18h]
        int_entry(t, i, kotor_rules_str_00745eec, &v);                      // "Name" 0054F3B9..0054F401
        wr32(cls(rules, i), 0x00, static_cast<Word>(v));                    // 0054F406..0054F412
        int_entry(t, i, kotor_rules_str_007488dc, &v);                      // "SkillPointBase" 0054F414..0054F448
        cls(rules, i)[0x56] = static_cast<unsigned char>(v);                // 0054F44D..0054F459
        int_entry(t, i, kotor_rules_str_007488d4, &v);                      // "Lower" 0054F45C..0054F499
        wr32(cls(rules, i), 0x04, static_cast<Word>(v));                    // 0054F49E..0054F4AA
        int_entry(t, i, kotor_rules_str_007488cc, &v);                      // "Plural" 0054F4AD..0054F4E1
        wr32(cls(rules, i), 0x08, static_cast<Word>(v));                    // 0054F4E6..0054F4F2
        int_entry(t, i, kotor_rules_str_007488c0, &v);                      // "Description" 0054F4F5..0054F532
        wr32(cls(rules, i), 0x0C, static_cast<Word>(v));                    // 0054F537..0054F543
        string_entry(t, i, kotor_rules_str_007488b8, &text);                // "Icon" 0054F546..0054F57A (result unused)
        reinterpret_cast<String*>(cls(rules, i) + 0x10)->Assign005e5c50(&text); // 0054F57F..0054F58F 005E5C50
        int_entry(t, i, kotor_rules_str_007488b0, &v);                      // "HitDie" 0054F594..0054F5D1
        cls(rules, i)[0x54] = static_cast<unsigned char>(v);                // 0054F5D6..0054F5E2
        int_entry(t, i, kotor_rules_str_007488a4, &v);                      // "ForceDie" 0054F5E5..0054F619
        cls(rules, i)[0x55] = static_cast<unsigned char>(v);                // 0054F61E..0054F62A
        if (string_entry(t, i, kotor_rules_str_00748890, &text))            // "AttackBonusTable" 0054F62D..0054F679
            kotor_fastcall_005bcda0(cls(rules, i), nullptr, &text);         // 0054F67B..0054F688
        if (string_entry(t, i, kotor_rules_str_00748884, &text))            // "FeatsTable" 0054F68D..0054F6D9
            kotor_fastcall_005bd0f0(cls(rules, i), nullptr, &text, rules);  // 0054F6DB..0054F6E9 (push ebp; push &text)
        if (string_entry(t, i, kotor_rules_str_00748870, &text))            // "SavingThrowTable" 0054F6EE..0054F731
            kotor_fastcall_005bd480(cls(rules, i), nullptr, &text);         // 0054F733..0054F740
        if (string_entry(t, i, kotor_rules_str_00748864, &text))            // "SkillsTable" 0054F745..0054F791
            kotor_fastcall_005bd6c0(cls(rules, i), nullptr, &text);         // 0054F793..0054F7A0
        if (string_entry(t, i, kotor_rules_str_00748854, &text))            // "SpellGainTable" 0054F7A5..0054F7F1
            kotor_fastcall_005bd900(cls(rules, i), nullptr, &text);         // 0054F7F3..0054F800
        if (string_entry(t, i, kotor_rules_str_00748844, &text))            // "SpellKnownTable" 0054F805..0054F851
            kotor_fastcall_005bdbe0(cls(rules, i), nullptr, &text);         // 0054F853..0054F860
        if (string_entry(t, i, kotor_rules_str_00748838, &text))            // "FeatGain" 0054F865..0054F8A8
            kotor_fastcall_005bcf70(cls(rules, i), nullptr, &text);         // 0054F8AA..0054F8B7
        if (string_entry(t, i, kotor_rules_str_00748824, &text))            // "ArmorClassColumn" 0054F8BC..0054F8FF
            kotor_fastcall_005bde80(cls(rules, i), nullptr, &text);         // 0054F901..0054F90E
        kotor_fastcall_005be4c0(cls(rules, i), nullptr);                    // 0054F913..0054F91B (unconditional)
        if (int_entry(t, i, kotor_rules_str_00748818, &v))                  // "PlayerClass" 0054F920..0054F963
            wr32(cls(rules, i), 0x198, v != 0 ? 1u : 0u);                   // 0054F965..0054F978 setne
        if (int_entry(t, i, kotor_rules_str_0074880c, &v))                  // "SpellCaster" 0054F97E..0054F9C1
            wr32(cls(rules, i), 0x19C, v != 0 ? 1u : 0u);                   // 0054F9C3..0054F9D6
        if (int_entry(t, i, kotor_rules_str_007487fc, &v))                  // "AlignRestrict" 0054F9DC..0054FA28
            cls(rules, i)[0x181] = static_cast<unsigned char>(v);           // 0054FA2A..0054FA36
        if (int_entry(t, i, kotor_rules_str_007487ec, &v))                  // "AlignRstrctType" 0054FA3C..0054FA88
            cls(rules, i)[0x182] = static_cast<unsigned char>(v);           // 0054FA8A..0054FA96
        if (string_entry(t, i, kotor_rules_str_007487e0, &text)) {          // "PrimaryAbil" 0054FA9C..0054FAE8
            const char* const abilities[6] = {                       // 0054FAEE..0054FBBD compare chain
                kotor_rules_str_007487dc, kotor_rules_str_007487d8, kotor_rules_str_007487d4,
                kotor_rules_str_007487d0, kotor_rules_str_007487cc, kotor_rules_str_007487c8};
            for (unsigned char k = 0; k < 6; ++k) {
                if (text.BorrowedEquals005e5290(abilities[k])) {            // 005E5290 (first match wins)
                    cls(rules, i)[0x17A] = k;                               // 0054FB08/2E/54/77/9A/BD
                    break;
                }
            }
        }
        const unsigned offsets[6] = {0x17B, 0x17C, 0x17D, 0x17E, 0x17F, 0x180};
        const char* const stats[6] = {kotor_rules_str_007487c4, kotor_rules_str_007487c0, kotor_rules_str_007487bc,
                                      kotor_rules_str_007487b8, kotor_rules_str_007487b4, kotor_rules_str_007487b0};
        for (unsigned k = 0; k < 6; ++k) {                                  // 0054FBC4..0054FDB6 (unconditional stores)
            int_entry(t, i, stats[k], &v);                                  // "Str".."Cha"
            cls(rules, i)[offsets[k]] = static_cast<unsigned char>(v);
        }
        {
            String level_column;                                            // 0054FDBC/0054FDC0 [esp+30h] (state 1Eh)
            for (std::int32_t k = 1; k <= 0x14; ++k) {                      // 0054FDCD..0054FE57 (jle 14h)
                kotor_reconstruction::StartupFormat005e5680(&level_column, kotor_rules_str_007487a0, k); // "EffCRLvl%02d" 0054FDD5..0054FDE9
                String column(WindowCStr005e5670(&level_column, nullptr));  // 0054FDEC..0054FDFD (state 1Fh)
                int_entry_s(t, i, &column, &v);                             // 0054FE02..0054FE1A
                cls(rules, i)[0x183 + static_cast<unsigned char>(k)] = static_cast<unsigned char>(v); // 0054FE33..0054FE44
            }                                                               // 0054FE2E ~column
        }                                                                   // 0054FE69 ~level_column
    }                                                                       // 0054FE7D ~text
    delete_c2da(t);                                                         // 0054FE98..0054FE9E (no null test)
}
}
