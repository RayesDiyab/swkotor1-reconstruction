// Lane worker "s2p-small" (fork d): client server-to-player DebugInfo message (major 20h of the client dispatcher
// CSWCMessage::HandleServerToPlayerMessage 0066A640). Transcribed from the original swkotor.exe (GOG K1, read-only
// static disassembly); statement comments cite original instruction addresses; B+x = original frame slot at ESP just
// after the prologue register pushes.
//   0066A410 CSWCMessage::HandleServerToPlayerDebugInfo(BYTE minor)            thiscall RET 4  -> @12 export
//   00653850 CSWCMessage::HandleServerToPlayerDebugInfo_Placeable(DWORD id)    thiscall RET 4  -> @12
//   00652F20 CSWCMessage::HandleServerToPlayerDebugInfo_Item(DWORD id)         thiscall RET 4  -> @12
//   00663840 CSWCMessage::HandleServerToPlayerDebugInfo_Creature(DWORD id)     thiscall RET 4  -> @12
//   00648800 CSWCCreatureStats::GetSpellsPerDayLeft(BYTE class, BYTE level)    thiscall RET 8  -> @16
//   006495A0 CSWCCreatureStats::GetKnownSpell(BYTE class, BYTE level, BYTE i)  thiscall RET 0Ch -> @20
// The handlers print the received debug record into the in-game message buffer (CGuiInGame::AppendToMsgBuffer).
// Original quirks kept: every CExoString(int)/CStr pair whose result the compiler kept but never used is still
// performed (allocation traffic identical); CResRef text is passed to CExoString(char*) unterminated (00663DF3);
// the Item handler leaks the WORD array and the ITEMPROPS C2DA on its failure exits; the Creature handler leaks the
// repository string array on its failure exits; GetINTEntry results are used even when the lookup failed.
#include "6bd5403233-claude_dialogue_common.hpp"

using namespace dlg;

namespace kotor_reconstruction { struct StartupString; }
// 005E5D10 CExoString::operator+(CExoString const&) const (0093-ini-string-priority.obj)
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
                                                                  kotor_reconstruction::StartupString* out,
                                                                  const kotor_reconstruction::StartupString* rhs);
namespace recovered_save_stats {
unsigned short maximum_hp_00647a80(void* stats);                                   // 00647A80 GetMaxHitPoints (0859)
unsigned short spell_count_006487d0(void* stats, unsigned cls, unsigned level);    // 006487D0 GetNumberKnownSpells (0859)
}
namespace kotor { namespace server_mainloop { unsigned creature_force_class_005be4a0(unsigned cls); } }   // 005BE4A0 IsJedi (cdecl)

extern "C" {
// ---- message readers (w2_message_native.obj)
U __fastcall kotor_fastcall_004d6230(void* msg, void*, U bits);                   // MessageReadOverflow RET 4
U __fastcall kotor_fastcall_00692630(void* msg, void*);                           // ReadOBJECTIDClient RET
float __fastcall kotor_fastcall_004d67b0(void* msg, void*, U scale_bits, U bits);  // ReadFLOAT RET 8
U __fastcall kotor_fastcall_004d6780(void* msg, void*, U bits);                   // ReadDWORD RET 4
U8 __fastcall kotor_fastcall_004d6720(void* msg, void*, U bits);                  // ReadBYTE RET 4
U16 __fastcall kotor_fastcall_004d6750(void* msg, void*, U bits);                 // ReadWORD RET 4
void* __fastcall kotor_fastcall_004d67e0(void* msg, void*, void* out, U bits);    // ReadCExoString RET 8
void* __fastcall kotor_fastcall_004d6180(void* msg, void*, void* out, U bits);    // ReadCResRef RET 8
void* __fastcall kotor_fastcall_00692660(void* msg, void*, void* out, void* resref, U flag);   // ReadCExoLocStringClient RET 0Ch
// ---- client app / objects
void* __fastcall kotor_fastcall_005ed580(void* app, void*, U id);                 // GetGameObject RET 4 (0701)
void* __fastcall frame_R4_005ed5b0(void* app, void*, U id);                       // GetCreatureByGameObjectID (0457)
void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void* app, void*, U id);   // GetItemByGameObjectID (0813)
void* __fastcall recovered_client_get_placeable_by_id_005ed5f0(void* app, void*, U id) noexcept(false);   // (1204-04)
void __fastcall feedback_append_0062b5c0(void* gui, void*, void* text, U color, U flag);   // AppendToMsgBuffer RET 0Ch
void* __fastcall feedback_item_name_00646e70(void* item, void*, void* out, U flag);   // CSWCItem::GetName RET 8
void* __fastcall recovered_client_item_get_active_property_006466a0(void* item, void*, U index);   // RET 4 (1204-00)
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, int id);   // RET 4 (0820)
void* __fastcall kotor_fastcall_005b4690(void* base_item, void*, void* out);      // CSWBaseItem::GetNameText RET 4
void* __fastcall feedback_subrace_00647660(void* stats, void*, void* out);        // 00647660 GetRaceString RET 4
void* __fastcall kotor_m15b_creature_stats_get_classes_string_00648470(void* stats, void*, void* out, U flag);   // RET 8
U16 __fastcall kotor_m15b_creature_stats_get_armor_class_00647720(void* stats, void*, U flag);   // RET 4 (0819)
U8 __fastcall kotor_m15b_creature_stats_get_class_00647730(void* stats, void*, U index);        // RET 4 (0819)
void* __fastcall kotor_m15b_class_get_name_text_005be0e0(void* cls, void*, void* out) noexcept(false);   // RET 4
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, U id);   // RET 4 (0820)
void* __fastcall feedback_spell_name_0059b940(void* spell, void*, void* out);     // CSWSpell::GetSpellNameText RET 4
// ---- strings / resrefs / 2DA / TLK
void* __fastcall frame_LogAssign_005e5140(void* s, void*, const char* text);       // CExoString::operator=(char*)
const char* __fastcall frame_LogCStr_005e5670(void* s, void*);                     // CExoString::CStr
void* __fastcall resman_string_from_int_005e5bc0(void* s, void*, int value);       // CExoString(int) (0266)
void* __fastcall kotor_m15e_slot00c_self_00641db0(void* self, void*);              // 00641DB0 return this
int __cdecl format_006fadb0(char* out, const char* format, ...);                   // sprintf (0310)
int __fastcall c2da_current_get_string_00413270(void* table, void*, int row, void* column, void* out);   // RET 0Ch
int __fastcall c2da_current_get_int_00413660(void* table, void*, int row, void* column, int* out);       // RET 0Ch
int __fastcall c2da_current_load_004143b0(void* table, void*);                    // C2DA::Load2DArray
void __fastcall c2da_current_unload_004139e0(void* table, void*);                  // C2DA::Unload2DArray
U __fastcall tlk_fetch_0041e550(void* tlk, void*, U strref, void* out, U flag);    // CTlkTable::Fetch RET 0Ch
extern void* kotor_cell_tlk_007a3a08;                                              // [007A3A08] CTlkTable*
extern void* kotor_cell_rules_007a3a28;                                            // [007A3A28] CSWRules*
}

namespace {
struct ResRefArg { unsigned char bytes[16]; };     // CResRef by value
}
extern "C" void* __fastcall c2da_current_construct_00413cc0(void* self, void*, ResRefArg name, int mode) noexcept(false);   // C2DA(CResRef, int) RET 14h

extern "C" {
// this unit
U __fastcall kotor_fastcall_0066a410(void* self, void*, unsigned minor) noexcept(false);
U __fastcall kotor_fastcall_00653850(void* self, void*, U id) noexcept(false);
U __fastcall kotor_fastcall_00652f20(void* self, void*, U id) noexcept(false);
U __fastcall kotor_fastcall_00663840(void* self, void*, U id) noexcept(false);
U8 __fastcall kotor_fastcall_00648800(void* stats, void*, U cls, U level) noexcept;
U __fastcall kotor_fastcall_006495a0(void* stats, void*, U cls, U level, U index) noexcept;
}

namespace {
constexpr U kMessageColor = 0x10000000u;          // immediate (00653AA6 ...)
constexpr U8 kTlkFlag_0073ecd0 = 0;               // .rdata 0073ECD0 (byte 00)
constexpr U kOneBits = 0x3f800000u;               // immediate 1.0f
const char kStars_00751e04[] = "******************";
const char kLabel_00748610[] = "LABEL";
const char kName_00751dfc[] = "Name: ";
const char kTag_00751df0[] = "    Tag: ";
const char kAppearance4_00751e90[] = "    Appearance: ";
const char kHardness_00751e84[] = "Hardness: ";
const char kHp_00751e78[] = "    HP: ";
const char kFortitude_00751e68[] = "FortitudeSave: ";
const char kReflex_00751e54[] = "    ReflexSave: ";
const char kWill_00751e44[] = "    WillSave: ";
const char kTemplate_00751e30[] = "TemplateResRef: ";
const char kFaction_00751e24[] = "Faction: ";
const char kItemProps_00751e18[] = "ITEMPROPS";
const char kBaseType_00751de0[] = "    BaseType: ";
const char kWeight_00751dd4[] = "Weight: ";
const char kAc_00751dc8[] = "    AC: ";
const char kActive_00751db4[] = "ActiveProperties: ";
const char kStringRef_00751da8[] = "STRINGREF";
const char kPassive_00751d94[] = "PassiveProperties: ";
const char kIndent_00751c54[] = "    ";
const char kFirstName_00752118[] = "FirstName: ";
const char kLastName_00752108[] = "    LastName: ";
const char kConversation_007520f4[] = "Conversation File: ";
const char kAppearance_007520e4[] = "Appearance: ";
const char kRace_007520d8[] = "    Race: ";
const char kClass_007520cc[] = "    Class: ";
const char kAiLevel_007520bc[] = "    AI Level: ";
const char kCurrentHp_007520b0[] = "CurrentHP: ";
const char kMaxHp_007520a4[] = "    MaxHP: ";
const char kPosFmt_00752090[] = "%.2f, %.2f, %.2f";
const char kAreaName_00752084[] = "Area Name: ";
const char kAreaTag_00752074[] = "    Area Tag: ";
const char kPos_00752068[] = "    Pos: ";
const char kEquipped_00752058[] = "EquippedItems: ";
const char kRepository_00751f68[] = "RepositoryItems: ";
const char kKnownSpells_00751f58[] = "KnownSpells: ";
const char kLevel_00751f48[] = "        Level: ";
const char kSpellsPerDay_00751f2c[] = "        SpellsPerDayLeft: ";
const char kSpellIndent_00751f1c[] = "            ";
const char kChallenge_00751f08[] = "ChallengeRating: ";
const char kFloatFmt_0074e894[] = "%.2f";
// equipped-slot labels by mask bit (bits 11..13 are not printed)
struct SlotLabel { U bit; const char* text; };
const SlotLabel kSlots[] = {
    {0, "    Head: "},        {1, "    Chest: "},        {2, "    Boots: "},     {3, "    Arms: "},        // 0075204C 00752040 00752034 00752028
    {4, "    RightHand: "},   {5, "    LeftHand: "},     {6, "    Cloak: "},     {8, "    RightForeArm: "},   // 00752018 00752008 00751FFC 00751FE8
    {7, "    LeftForeArm: "}, {9, "    Neck: "},         {10, "    Belt: "},     {14, "    CWeapon1: "},       // 00751FD4 00751FC8 00751FBC 00751FAC
    {15, "    CWeapon2: "},   {16, "    CWeapon3: "},    {17, "    CArmor: "},                               // 00751F9C 00751F8C 00751F7C
};

inline void* rules() { return kotor_cell_rules_007a3a28; }
inline kotor_reconstruction::StartupString* ss(void* s) { return static_cast<kotor_reconstruction::StartupString*>(s); }
inline const kotor_reconstruction::StartupString* ss(const void* s) {
    return static_cast<const kotor_reconstruction::StartupString*>(s);
}
// a CExoString constructed by a callee into caller storage (hidden return); destroyed at scope end
struct Made {
    U data = 0, length = 0;
    bool live = false;
    Made() = default;
    Made(const Made&) = delete;
    Made& operator=(const Made&) = delete;
    void* p() { return this; }
    void* own(void* r) { live = true; return r; }
    ~Made() { if (live) frame_LogStringDtor_005e5c20(this, nullptr); }
};
// line = line + rhs (operator+ into a temporary, operator=, temporary destroyed)
void cat(void* line, const void* rhs) {
    Made sum;
    sum.own(GuiConcat005e5d10(ss(line), nullptr, ss(sum.p()), ss(rhs)));
    str_assign(line, &sum);
}
void cat_text(void* line, const char* text) {           // CExoString tmp(text); line = line + tmp
    ExoString tmp(text);
    cat(line, &tmp);
}
void cat_int(void* line, int value) {                   // CExoString tmp(value); line = line + tmp
    Made tmp;
    tmp.own(resman_string_from_int_005e5bc0(tmp.p(), nullptr, value));
    cat(line, &tmp);
}
void dead_int(int value) {                              // CExoString(value).CStr() with the result unused
    Made tmp;
    tmp.own(resman_string_from_int_005e5bc0(tmp.p(), nullptr, value));
    (void)frame_LogCStr_005e5670(tmp.p(), nullptr);
}
void set(void* line, const char* text) { (void)frame_LogAssign_005e5140(line, nullptr, text); }
void append(void* gui, void* text) { feedback_append_0062b5c0(gui, nullptr, text, kMessageColor, 0); }
void append_text(void* gui, const char* text) {         // CExoString tmp(text); AppendToMsgBuffer(tmp)
    ExoString tmp(text);
    append(gui, &tmp);
}
void read_string(void* msg, void* dst) {                // dst = ReadCExoString(32)
    Made tmp;
    tmp.own(kotor_fastcall_004d67e0(msg, nullptr, tmp.p(), 0x20));
    str_assign(dst, &tmp);
}
// an eh-vector-constructed CExoString[n] (destroyed with the CRT vector destructor iterator)
struct StringArray {
    void* first;
    I count;
    StringArray(void* storage, I n) : first(storage), count(n) {
        crt_eh_vector_constructor_006fb55f(first, 8, count, reinterpret_cast<void*>(&kotor_menu20::ConstructString005b3190),
                                           reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));
    }
    StringArray(const StringArray&) = delete;
    StringArray& operator=(const StringArray&) = delete;
    ~StringArray() { c2da_destroy_array_006fa852(first, 8, count, reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20)); }
    void* at_index(U i) const { return ptr(word(first) + 8 * i); }
};
}  // namespace

// ================================================================================================================
// 00648800 CSWCCreatureStats::GetSpellsPerDayLeft(BYTE class, BYTE level) RET 8
extern "C" U8 __fastcall kotor_fastcall_00648800(void* stats, void*, U cls, U level) noexcept {
    const U8 c = static_cast<U8>(cls);                                     // 00648800
    const U8 l = static_cast<U8>(level);                                   // 00648808
    if (c >= 2 || l >= 1) return 0;                                        // 00648804..00648813 (jae / jb)
    return rd8(stats, (static_cast<U>(c) << 5) + l + 0xec);                // 00648816..00648821
}

// 006495A0 CSWCCreatureStats::GetKnownSpell(BYTE class, BYTE level, BYTE index) RET 0Ch
extern "C" U __fastcall kotor_fastcall_006495a0(void* stats, void*, U cls, U level, U index) noexcept {
    const U8 c = static_cast<U8>(cls);                                     // 006495A0
    if (c >= 2) return 0xffffffffu;                                        // 006495A4..006495BD
    const U list = word(stats) + (static_cast<U>(c) << 5) + 0xd4;          // 006495A8..006495AE
    const U8 l = static_cast<U8>(level);                                   // 006495B5
    if (l >= 1) return 0xffffffffu;                                        // 006495B9..006495BD
    const U entry = list + static_cast<U>(l) * 12;                         // 006495C5..006495CB CExoArrayList<DWORD>
    const I i = static_cast<U8>(index);                                    // 006495CE movzx
    if (i >= rdi(ptr(entry), 4)) return 0xffffffffu;                       // 006495D3..006495DB (signed jl)
    return rd(rp(ptr(entry), 0), static_cast<U>(i) * 4);                   // 006495DE..006495E0
}

// ================================================================================================================
// 0066A410 HandleServerToPlayerDebugInfo(BYTE minor) RET 4 (the minor is not examined)
extern "C" U __fastcall kotor_fastcall_0066a410(void* self, void*, unsigned) noexcept(false) {
    const U id = kotor_fastcall_00692630(self, nullptr);                   // 0066A412..0066A41D esi
    if (kotor_fastcall_004d6230(self, nullptr, 1) != 0) return 0;         // 0066A419..0066A426
    void* object = kotor_fastcall_005ed580(client(), nullptr, id);         // 0066A428..0066A431
    if (object != nullptr) {                                               // 0066A436
        switch (rd8(object, 8)) {                                          // 0066A43A object type
        case 5: return kotor_fastcall_00663840(self, nullptr, id);         // 0066A465 creature
        case 6: return kotor_fastcall_00652f20(self, nullptr, id);         // 0066A458 item
        case 9: return kotor_fastcall_00653850(self, nullptr, id);         // 0066A44B placeable
        default: return 0;                                                 // 0066A495
        }
    }
    void* module = kotor_fastcall_005ed530(client(), nullptr);             // 0066A472..0066A47B
    if (rd(rp(module, 0x48), 0x104) != id) return 0;                       // 0066A480..0066A489 (area id)
    return 1;                                                              // 0066A48B
}

// ================================================================================================================
// 00653850 HandleServerToPlayerDebugInfo_Placeable(DWORD id) RET 4
extern "C" U __fastcall kotor_fastcall_00653850(void* self, void*, U id) noexcept(false) {
    void* const msg = self;                                                // esi
    ResRef template_ref;                                                   // B+68
    resref_default(&template_ref);                                         // 00653873..00653877
    ExoString line;                                                        // B+18 (state 0)
    ExoString text;                                                        // B+20 (state 1)
    ExoString name;                                                        // B+38 (state 2)
    ExoString tag;                                                         // B+30 (state 3)
    ExoString unused_40;                                                   // B+40 (state 4)
    ExoString faction;                                                     // B+28 (state 5)
    {
        ResRef scratch;                                                    // B+58
        Made loc;                                                          // B+48
        loc.own(kotor_fastcall_00692660(msg, nullptr, loc.p(), kotor_fastcall_00405ed0(&scratch, nullptr), 0));   // 006538DF..006538F9
        str_assign(&name, &loc);                                           // 006538FE..0065390B
    }                                                                      // 00653910..0065391B
    read_string(msg, &tag);                                                // 00653920..0065394B
    const U8 hardness = kotor_fastcall_004d6720(msg, nullptr, 8);          // 00653950..0065395D B+12
    U hit_points = kotor_fastcall_004d6750(msg, nullptr, 0x10);            // 00653959..0065396A B+14 (dword store)
    const U8 fortitude = kotor_fastcall_004d6720(msg, nullptr, 8);         // 00653966..00653977 B+13
    const U8 reflex = kotor_fastcall_004d6720(msg, nullptr, 8);            // 00653973..00653984 B+10
    const U8 will = kotor_fastcall_004d6720(msg, nullptr, 8);              // 00653980..0065398D B+11
    {
        ResRef read;                                                       // B+58
        ResRef scratch;                                                    // B+48
        void* got = kotor_fastcall_004d6180(msg, nullptr, &read, 0x10);    // 00653991..0065399A
        (void)ref_assign_copy_00406120(&template_ref, nullptr, &scratch, got);   // 0065399F..006539A9
    }
    read_string(msg, &faction);                                            // 006539AE..006539D9
    if (kotor_fastcall_004d6230(msg, nullptr, 1) != 0) return 0;          // 006539DE..006539E9
    void* placeable = recovered_client_get_placeable_by_id_005ed5f0(client(), nullptr, id);   // 006539EB..006539F8 edi
    if (placeable == nullptr) return 0;                                    // 006539FF..00653A01
    void* gui = in_game_gui();                                             // 00653A03..00653A0C esi
    if (gui == nullptr) return 0;                                          // 00653A13..00653A15
    append_text(gui, kStars_00751e04);                                     // 00653A96..00653ACA
    {
        ExoString column(kLabel_00748610);                                 // 00653ACF..00653AD8
        (void)c2da_current_get_string_00413270(rp(rp(rules(), 0xb8), 0x2c), nullptr, rd8(placeable, 0x10c), &column,
                                               &text);                     // 00653ADD..00653B08
    }
    set(&line, kName_00751dfc);                                            // 00653B1D..00653B26
    cat(&line, &name);                                                     // 00653B2B..00653B5B
    cat_text(&line, kTag_00751df0);                                        // 00653B60..00653BB7
    cat(&line, &tag);                                                      // 00653BBC..00653BEC
    cat_text(&line, kAppearance4_00751e90);                                // 00653BF1..00653C48
    cat(&line, &text);                                                     // 00653C4D..00653C7D
    append(gui, &line);                                                    // 00653C82..00653C90
    (void)frame_LogCStr_005e5670(&name, nullptr);                          // 00653C95..00653C99
    (void)frame_LogCStr_005e5670(&tag, nullptr);                           // 00653C9E..00653CA2
    (void)frame_LogCStr_005e5670(&text, nullptr);                          // 00653CA7..00653CAB
    set(&line, kHardness_00751e84);                                        // 00653CB0..00653CB9
    cat_int(&line, hardness);                                              // 00653CBE..00653D12
    cat_text(&line, kHp_00751e78);                                         // 00653D17..00653D6E
    hit_points = static_cast<U>(static_cast<I>(static_cast<std::int16_t>(hit_points)));   // 00653D73..00653D7D movsx -> B+14
    cat_int(&line, static_cast<int>(hit_points));                          // 00653D81..00653DCB
    append(gui, &line);                                                    // 00653DD0..00653DDE
    dead_int(hardness);                                                    // 00653DE3..00653E07
    dead_int(static_cast<int>(hit_points));                                // 00653E0C..00653E34
    set(&line, kFortitude_00751e68);                                       // 00653E39..00653E42
    cat_int(&line, fortitude);                                             // 00653E47..00653E9B
    cat_text(&line, kReflex_00751e54);                                     // 00653EA0..00653EF7
    cat_int(&line, reflex);                                                // 00653EFC..00653F50
    cat_text(&line, kWill_00751e44);                                       // 00653F55..00653FAC
    cat_int(&line, will);                                                  // 00653FB1..00654005
    append(gui, &line);                                                    // 0065400A..00654018
    dead_int(fortitude);                                                   // 0065401D..00654041
    dead_int(reflex);                                                      // 00654046..0065406F
    dead_int(will);                                                        // 00654074..0065409D
    c2da_ref_to_string_00405f70(&template_ref, nullptr, &text);      // 006540A2..006540AB
    set(&line, kTemplate_00751e30);                                        // 006540B0..006540B9
    cat(&line, &text);                                                     // 006540BE..006540EE
    append(gui, &line);                                                    // 006540F3..00654101
    (void)frame_LogCStr_005e5670(&text, nullptr);                          // 00654106..0065410A
    set(&line, kFaction_00751e24);                                         // 0065410F..00654118
    cat(&line, &faction);                                                  // 0065411D..0065414D
    append(gui, &line);                                                    // 00654152..00654160
    (void)frame_LogCStr_005e5670(&faction, nullptr);                       // 00654165..00654169
    append_text(gui, kStars_00751e04);                                     // 0065416E..006541A2
    return 1;                                                              // 006541A7..00654226
}

// ================================================================================================================
// 00652F20 HandleServerToPlayerDebugInfo_Item(DWORD id) RET 4
extern "C" U __fastcall kotor_fastcall_00652f20(void* self, void*, U id) noexcept(false) {
    void* const msg = self;                                                // esi
    ExoString text;                                                        // B+54 (state 0)
    ResRef unused_ref;                                                     // B+5C
    resref_default(&unused_ref);                                           // 00652F4E..00652F56
    ExoString base_type;                                                   // B+3C (state 2)
    ExoString line;                                                        // B+2C (state 3)
    ExoString tag;                                                         // B+34 (state 4)
    read_string(msg, &tag);                                                // 00652F88..00652FB9
    const I count = static_cast<I>(kotor_fastcall_004d6780(msg, nullptr, 0x20));   // 00652FBE..00652FC9 B+10
    U16* passive = nullptr;                                                // B+18
    if (count > 0) {                                                       // 00652FC7..00652FD1 (signed)
        passive = static_cast<U16*>(live_operator_new(static_cast<U>(count) * 2));   // 00652FD3..00652FE1
        if (passive == nullptr) return 0;                                  // 00652FE5
    }
    for (I i = 0; i < count; ++i)                                          // 00652FEB..0065300B
        passive[i] = kotor_fastcall_004d6750(msg, nullptr, 0x10);
    if (kotor_fastcall_004d6230(msg, nullptr, 1) != 0) return 0;          // 0065300D..00653018 (array leaked)
    void* props = nullptr;                                                 // B+14
    {
        void* mem = live_operator_new(0x54);                               // 0065301E..00653025 (EH state 6)
        if (mem != nullptr) {
            ResRefArg name;
            resref_from(&name, kItemProps_00751e18);                       // 00653037..00653047 CResRef("ITEMPROPS")
            try {
                props = c2da_current_construct_00413cc0(mem, nullptr, name, 0);   // 0065304C..00653053
            } catch (...) {
                live_free(mem);                                            // state 6 unwind: free
                throw;
            }
        }
    }
    if (props == nullptr) return 0;                                        // 00653065..0065306B
    if (c2da_current_load_004143b0(props, nullptr) == 0) return 0;         // 0065306D..00653076 (table + array leaked)
    void* item = kotor_m15_client_get_item_by_id_005ed5a0(client(), nullptr, id);   // 00653078..00653084 esi
    if (item == nullptr) return 0;                                         // 0065308B..0065308D
    void* gui = in_game_gui();                                             // 0065308F..00653097 edi
    if (gui == nullptr) return 0;                                          // 0065309C..006530A0
    append_text(gui, kStars_00751e04);                                     // 006530F3..00653124
    {
        Made name;                                                         // B+44
        void* base = kotor_m15b_base_item_array_get_base_item_005b31a0(rp(rules(), 0x34), nullptr, rdi(item, 0xc));   // 00653129..0065313A
        name.own(kotor_fastcall_005b4690(base, nullptr, name.p()));        // 0065313F..00653141
        str_assign(&base_type, &name);                                     // 00653146..00653153
    }                                                                      // 00653158..00653160
    set(&line, kName_00751dfc);                                            // 00653165..0065316E
    {
        Made item_name;                                                    // B+24
        item_name.own(feedback_item_name_00646e70(item, nullptr, item_name.p(), 0));   // 00653173..0065317C
        cat(&line, &item_name);                                            // 00653181..006531AA
    }                                                                      // 006531B8..006531C0
    cat_text(&line, kTag_00751df0);                                        // 006531C5..00653216
    cat(&line, &tag);                                                      // 0065321B..00653248
    cat_text(&line, kBaseType_00751de0);                                   // 0065324D..0065329E
    cat(&line, &base_type);                                                // 006532A3..006532D0
    append(gui, &line);                                                    // 006532D5..006532E3
    {
        Made item_name;                                                    // B+44
        item_name.own(feedback_item_name_00646e70(item, nullptr, item_name.p(), 0));   // 006532E8..006532F1
        (void)frame_LogCStr_005e5670(item_name.p(), nullptr);              // 006532F6..006532FD
    }                                                                      // 00653302..0065330A
    (void)frame_LogCStr_005e5670(&tag, nullptr);                           // 0065330F..00653313
    (void)frame_LogCStr_005e5670(&base_type, nullptr);                     // 00653318..0065331C
    set(&line, kWeight_00751dd4);                                          // 00653321..0065332A
    cat_int(&line, 0);                                                     // 0065332F..00653379
    cat_text(&line, kAc_00751dc8);                                         // 0065337E..006533CF
    cat_int(&line, rdi(item, 0x10c));                                      // 006533D4..00653423
    append(gui, &line);                                                    // 00653428..00653436
    dead_int(0);                                                           // 0065343B..0065345A
    dead_int(rdi(item, 0x10c));                                            // 0065345F..00653483
    set(&line, kActive_00751db4);                                          // 00653488..00653491
    cat_int(&line, rdi(item, 0x120));                                      // 00653496..006534E5
    append(gui, &line);                                                    // 006534EA..006534F8
    dead_int(rdi(item, 0x120));                                            // 006534FD..00653521
    int strref;                                                            // B+20 (GetINTEntry result, unchecked)
    for (I i = 0; i < rdi(item, 0x120); ++i) {                             // 00653526..00653624 (count re-read)
        void* property = recovered_client_item_get_active_property_006466a0(item, nullptr, static_cast<U>(i));   // 00653540..00653547
        if (property == nullptr) continue;                                 // 0065354C..00653552
        {
            ExoString column(kStringRef_00751da8);                         // 00653558..00653561
            (void)c2da_current_get_int_00413660(props, nullptr, rd16(property, 0), &column, &strref);   // 00653566..00653587
        }                                                                  // 0065358C..00653594
        (void)tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, static_cast<U>(strref), &text, kTlkFlag_0073ecd0);   // 00653599..006535B2
        set(&line, kIndent_00751c54);                                      // 006535B7..006535C0
        cat(&line, &text);                                                 // 006535C5..006535F2
        append(gui, &line);                                                // 006535F7..00653605
        (void)frame_LogCStr_005e5670(&text, nullptr);                      // 0065360A..0065360E
    }
    set(&line, kPassive_00751d94);                                         // 0065362A..00653633
    cat_int(&line, count);                                                 // 00653638..00653685
    append(gui, &line);                                                    // 0065368A..00653698
    dead_int(count);                                                       // 0065369D..006536BB
    for (I i = 0; i < count; ++i) {                                        // 006536C0..00653790
        {
            ExoString column(kStringRef_00751da8);                         // 006536D0..006536D9
            (void)c2da_current_get_int_00413660(props, nullptr, passive[i], &column, &strref);   // 006536DE..006536FD
        }                                                                  // 00653702..0065370A
        (void)tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, static_cast<U>(strref), &text, kTlkFlag_0073ecd0);   // 0065370F..00653728
        set(&line, kIndent_00751c54);                                      // 0065372D..00653736
        cat(&line, &text);                                                 // 0065373B..00653768
        append(gui, &line);                                                // 0065376D..0065377B
        (void)frame_LogCStr_005e5670(&text, nullptr);                      // 00653780..00653784
    }
    append_text(gui, kStars_00751e04);                                     // 00653796..006537C7
    c2da_current_unload_004139e0(props, nullptr);                          // 006537CC..006537D2
    vcall<void>(props, 0x00, 1u);                                          // 006537D7..006537DD scalar deleting destructor
    if (passive != nullptr) live_free(passive);                            // 006537DF..006537ED _free
    return 1;                                                              // 006537F0..00653841
}

// ================================================================================================================
// 00663840 HandleServerToPlayerDebugInfo_Creature(DWORD id) RET 4
extern "C" U __fastcall kotor_fastcall_00663840(void* self, void*, U id) noexcept(false) {
    void* const msg = self;                                                // esi
    alignas(4) unsigned char equipped_storage[0x12 * 8];                   // B+A0 CExoString[18]
    StringArray equipped(equipped_storage, 0x12);                          // 00663864..0066387C (state 0)
    ExoString line;                                                        // B+10 (state 1)
    ExoString text;                                                        // B+54 (state 2)
    ExoString tag;                                                         // B+4C (state 3)
    ExoString faction;                                                     // B+40 (state 4)
    ResRef conversation;                                                   // B+138
    resref_default(&conversation);                                         // 006638C6..006638D5
    ExoString area_tag;                                                    // B+5C (state 5)
    read_string(msg, &tag);                                                // 006638E3..00663917
    read_string(msg, &faction);                                            // 0066391C..00663947
    {
        ResRef read;                                                       // B+6C
        ResRef scratch;                                                    // B+150
        void* got = kotor_fastcall_004d6180(msg, nullptr, &read, 0x10);    // 0066394C..00663955
        (void)ref_assign_copy_00406120(&conversation, nullptr, &scratch, got);   // 0066395A..0066396A
    }
    const U current_hp = kotor_fastcall_004d6750(msg, nullptr, 0x10);      // 0066396F..0066397C B+94
    const U max_hp = kotor_fastcall_004d6750(msg, nullptr, 0x10);          // 00663978..00663988 B+7C
    const U armor = kotor_fastcall_004d6750(msg, nullptr, 0x10);           // 0066398C..00663999 B+84
    const U ai_level = kotor_fastcall_004d6780(msg, nullptr, 0x20);        // 00663995..006639AE B+30
    const float challenge = kotor_fastcall_004d67b0(msg, nullptr, kOneBits, 0x20);   // 006639A5..006639B7 B+9C
    read_string(msg, &area_tag);                                           // 006639BE..006639E9
    const U mask = kotor_fastcall_004d6780(msg, nullptr, 0x20);            // 006639EE..006639FD B+28
    U equipped_count = 0;                                                  // B+38
    for (U bit = 0; bit < 0x12; ++bit) {                                   // 00663A01..00663A65
        if ((mask & (1u << bit)) == 0) continue;                           // 00663A10..00663A1D
        read_string(msg, equipped.at_index(bit));                          // 00663A1F..00663A48
        ++equipped_count;                                                  // 00663A4D..00663A56
    }
    const U repository_count = kotor_fastcall_004d6780(msg, nullptr, 0x20);   // 00663A67..00663A74 B+2C
    unsigned char* repository = nullptr;                                   // B+48
    if (repository_count != 0) {                                           // 00663A72..00663A80 (unsigned)
        void* block = live_operator_new(repository_count * 8 + 4);         // 00663A86..00663A8E
        if (block != nullptr) {                                            // 00663A9A..00663AA4 (EH state 10)
            wr(block, 0, repository_count);                                // 00663AB5 element count header
            try {
                crt_eh_vector_constructor_006fb55f(at(block, 4), 8, static_cast<I>(repository_count),
                                                   reinterpret_cast<void*>(&kotor_menu20::ConstructString005b3190),
                                                   reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));   // 00663AAA..00663ABD
            } catch (...) {
                live_free(block);                                          // state 10 unwind: free
                throw;
            }
            repository = reinterpret_cast<unsigned char*>(at(block, 4));        // 00663AB7..00663AC2
        }
        if (repository == nullptr) return 0;                              // 00663ACC..00663AD9 (-> 00665476)
        for (U i = 0; i < repository_count; ++i)                           // 00663AE3..00663B2A
            read_string(msg, repository + 8 * i);
    }
    if (kotor_fastcall_004d6230(msg, nullptr, 1) != 0) return 0;          // 00663B2C..00663B37 (repository leaked)
    void* creature = frame_R4_005ed5b0(client(), nullptr, id);             // 00663B3D..00663B4A B+8C
    if (creature == nullptr) return 0;                                     // 00663B4F..00663B58
    void* appearance = rp(creature, 0x21c);                                // 00663B5E B+64
    void* stats = rp(creature, 0x2f8);                                     // 00663B66 edi
    void* area = rp(creature, 0x20);                                       // 00663B6C B+34
    if (appearance == nullptr || stats == nullptr || area == nullptr) return 0;   // 00663B64..00663B87
    void* gui = in_game_gui();                                             // 00663B8D..00663B96 esi
    if (gui == nullptr) return 0;                                          // 00663B9B..00663C07
    append_text(gui, kStars_00751e04);                                     // 00663C0C..00663C40
    set(&line, kFirstName_00752118);                                       // 00663C45..00663C4E
    cat(&line, at(stats, 0x14));                                           // 00663C53..00663C82 (first name)
    cat_text(&line, kLastName_00752108);                                   // 00663C87..00663CDE
    cat(&line, at(stats, 0x1c));                                           // 00663CE3..00663D12 (last name)
    cat_text(&line, kTag_00751df0);                                        // 00663D17..00663D6E
    cat(&line, &tag);                                                      // 00663D73..00663DA3
    append(gui, &line);                                                    // 00663DA8..00663DB6
    (void)frame_LogCStr_005e5670(at(stats, 0x14), nullptr);                // 00663DBB..00663DBE
    (void)frame_LogCStr_005e5670(at(stats, 0x1c), nullptr);                // 00663DC3..00663DC6
    (void)frame_LogCStr_005e5670(&tag, nullptr);                           // 00663DCB..00663DCF
    set(&line, kConversation_007520f4);                                    // 00663DD4..00663DDD
    cat_text(&line, static_cast<const char*>(kotor_m15e_slot00c_self_00641db0(&conversation, nullptr)));   // 00663DE2..00663E41
    append(gui, &line);                                                    // 00663E46..00663E54
    (void)kotor_m15e_slot00c_self_00641db0(&conversation, nullptr);        // 00663E59..00663E60
    {
        ExoString column(kLabel_00748610);                                 // 00663E65..00663E6E
        (void)c2da_current_get_string_00413270(rp(rp(rules(), 0xb8), 0xc), nullptr, rd16(appearance, 0x18), &column,
                                               &text);                     // 00663E73..00663E9F
    }                                                                      // 00663EA4..00663EAF
    set(&line, kAppearance_007520e4);                                      // 00663EB4..00663EBD
    cat(&line, &text);                                                     // 00663EC2..00663EF2
    cat_text(&line, kRace_007520d8);                                       // 00663EF7..00663F4E
    {
        Made race;                                                         // B+20
        race.own(feedback_subrace_00647660(stats, nullptr, race.p()));     // 00663F53..00663F5A
        cat(&line, &race);                                                 // 00663F5F..00663F94
    }                                                                      // 00663F99..00663FA4
    cat_text(&line, kClass_007520cc);                                      // 00663FA9..00664000
    {
        Made classes;                                                      // B+20
        classes.own(kotor_m15b_creature_stats_get_classes_string_00648470(stats, nullptr, classes.p(), 0x20));   // 00664005..0066400E
        cat(&line, &classes);                                              // 00664013..00664048
    }                                                                      // 0066404D..00664058
    append(gui, &line);                                                    // 0066405D..0066406B
    (void)frame_LogCStr_005e5670(&text, nullptr);                          // 00664070..00664074
    {
        Made race;                                                         // B+18
        race.own(feedback_subrace_00647660(stats, nullptr, race.p()));     // 00664079..00664080
        (void)frame_LogCStr_005e5670(race.p(), nullptr);                   // 00664085..0066408F
    }                                                                      // 00664094..0066409F
    {
        Made classes;                                                      // B+18
        classes.own(kotor_m15b_creature_stats_get_classes_string_00648470(stats, nullptr, classes.p(), 0x20));   // 006640A4..006640AD
        (void)frame_LogCStr_005e5670(classes.p(), nullptr);                // 006640B2..006640BC
    }                                                                      // 006640C1..006640CC
    set(&line, kFaction_00751e24);                                         // 006640D1..006640DA
    cat(&line, &faction);                                                  // 006640DF..0066410F
    cat_text(&line, kAiLevel_007520bc);                                    // 00664114..0066416B
    cat_int(&line, static_cast<int>(ai_level));                            // 00664170..006641C3
    append(gui, &line);                                                    // 006641C8..006641D6
    (void)frame_LogCStr_005e5670(&faction, nullptr);                       // 006641DB..006641DF
    {
        Made unused;                                                       // B+18 (CExoString(int) without CStr)
        unused.own(resman_string_from_int_005e5bc0(unused.p(), nullptr, static_cast<int>(ai_level)));   // 006641E4..006641ED
    }                                                                      // 006641F2..006641F6
    set(&line, kCurrentHp_007520b0);                                       // 006641FB..00664204
    cat_int(&line, static_cast<std::int16_t>(current_hp));                 // 00664209..00664260 movsx
    cat_text(&line, kMaxHp_007520a4);                                      // 00664265..006642BC
    cat_int(&line, static_cast<std::int16_t>(max_hp));                     // 006642C1..00664315 movsx
    cat_text(&line, kAc_00751dc8);                                         // 0066431A..00664371
    cat_int(&line, static_cast<std::int16_t>(armor));                      // 00664376..006643CD movsx
    append(gui, &line);                                                    // 006643D2..006643E0
    dead_int(static_cast<std::int16_t>(rd16(stats, 0x4c)));                // 006643E5..0066440D
    dead_int(static_cast<std::int16_t>(recovered_save_stats::maximum_hp_00647a80(stats)));   // 00664412..00664440
    dead_int(static_cast<std::int16_t>(kotor_m15b_creature_stats_get_armor_class_00647720(stats, nullptr, 0)));   // 00664445..00664475
    char buffer[0x44];                                                     // B+160
    {
        float pos[3];
        std::memcpy(pos, at(creature, 0x24), 12);                          // 0066447A..00664489
        (void)format_006fadb0(buffer, kPosFmt_00752090, static_cast<double>(pos[0]), static_cast<double>(pos[1]),
                              static_cast<double>(pos[2]));               // 0066448C..006644D6 (fld dword / fstp qword)
    }
    set(&line, kAreaName_00752084);                                        // 006644D9..006644E2
    void* const area_name = at(area, 0x17c);                               // 006644E7..006644F0 B+34
    cat(&line, area_name);                                                 // 006644F4..00664520
    cat_text(&line, kAreaTag_00752074);                                    // 00664525..0066457C
    cat(&line, &area_tag);                                                 // 00664581..006645B1
    cat_text(&line, kPos_00752068);                                        // 006645B6..0066460D
    cat_text(&line, buffer);                                               // 00664612..0066466C
    append(gui, &line);                                                    // 00664671..0066467F
    (void)frame_LogCStr_005e5670(area_name, nullptr);                      // 00664684..00664688
    set(&line, kEquipped_00752058);                                        // 0066468D..00664696
    cat_int(&line, static_cast<int>(equipped_count));                      // 0066469B..006646EE
    append(gui, &line);                                                    // 006646F3..00664701
    dead_int(static_cast<int>(equipped_count));                            // 00664706..0066472E
    for (const SlotLabel& s : kSlots) {                                    // 00664733..00664D92
        if ((mask & (1u << s.bit)) == 0) continue;
        set(&line, s.text);
        cat(&line, equipped.at_index(s.bit));
        append(gui, &line);
        (void)frame_LogCStr_005e5670(equipped.at_index(s.bit), nullptr);
    }
    set(&line, kRepository_00751f68);                                      // 00664D97..00664DA0
    cat_int(&line, static_cast<int>(repository_count));                    // 00664DA5..00664DF8
    append(gui, &line);                                                    // 00664DFD..00664E0B
    dead_int(static_cast<int>(repository_count));                          // 00664E10..00664E38
    for (U i = 0; i < repository_count; ++i) {                             // 00664E3D..00664ED3 (unsigned)
        void* entry = repository + 8 * i;                                  // B+38
        set(&line, kIndent_00751c54);                                      // 00664E60..00664E69
        cat(&line, entry);                                                 // 00664E6E..00664E9E
        append(gui, &line);                                                // 00664EA3..00664EB1
        (void)frame_LogCStr_005e5670(entry, nullptr);                      // 00664EB6..00664EBA
    }
    set(&line, kKnownSpells_00751f58);                                     // 00664ED5..00664EDE
    append(gui, &line);                                                    // 00664EE3..00664EF1
    for (U8 cls = 0; cls < rd8(stats, 0x32); ++cls) {                      // 00664EF6..006652DE (byte, unsigned)
        void* const klass = ptr(word(rp(rules(), 0xac)) +
                                static_cast<U>(kotor_m15b_creature_stats_get_class_00647730(stats, nullptr, cls)) * 0x1a0);   // 00664F06..00664F29
        if (klass == nullptr) continue;                                    // 00664F2D
        set(&line, kClass_007520cc);                                       // 00664F33..00664F3C
        {
            Made name;                                                     // B+20
            name.own(kotor_m15b_class_get_name_text_005be0e0(klass, nullptr, name.p()));   // 00664F41..00664F4A
            cat(&line, &name);                                             // 00664F4F..00664F84
        }                                                                  // 00664F89..00664F94
        append(gui, &line);                                                // 00664F99..00664FA7
        {
            Made name;                                                     // B+8C
            name.own(kotor_m15b_class_get_name_text_005be0e0(klass, nullptr, name.p()));   // 00664FAC..00664FB8
            (void)frame_LogCStr_005e5670(name.p(), nullptr);               // 00664FBD..00664FC7
        }                                                                  // 00664FCC..00664FDA
        const U16 known = recovered_save_stats::spell_count_006487d0(stats, cls, 0);   // 00664FDF..00664FE8 B+34
        if (known < 1) continue;                                           // 00664FED..00664FF5
        set(&line, kLevel_00751f48);                                       // 00664FFB..00665004
        cat_int(&line, 0);                                                 // 00665009..00665062
        append(gui, &line);                                                // 00665067..00665075
        dead_int(0);                                                       // 0066507A..006650A5
        if (kotor::server_mainloop::creature_force_class_005be4a0(
                kotor_m15b_creature_stats_get_class_00647730(stats, nullptr, cls)) != 0) {   // 006650AA..006650C4
            set(&line, kSpellsPerDay_00751f2c);                            // 006650CA..006650D3
            cat_int(&line, kotor_fastcall_00648800(stats, nullptr, cls, 0));   // 006650D8..00665138
            append(gui, &line);                                            // 0066513D..0066514B
            dead_int(kotor_fastcall_00648800(stats, nullptr, cls, 0));     // 00665150..0066518B
        }
        for (U j = 0; j < known; ++j) {                                    // 00665190..006652C9 (unsigned)
            const U spell_id = kotor_fastcall_006495a0(stats, nullptr, cls, 0, j);   // 006651B0..006651BE B+30
            if (kotor_m15b_spell_array_get_spell_0059b6d0(rp(rules(), 0x8c), nullptr, spell_id) == nullptr) continue;   // 006651C3..006651DB
            set(&line, kSpellIndent_00751f1c);                             // 006651E1..006651EA
            {
                Made spell_name;                                           // B+150
                void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(rp(rules(), 0x8c), nullptr, spell_id);   // 006651EF..00665208
                spell_name.own(feedback_spell_name_0059b940(spell, nullptr, spell_name.p()));   // 0066520D..0066520F
                cat(&line, &spell_name);                                   // 00665214..0066524F
            }                                                              // 00665254..00665262
            append(gui, &line);                                            // 00665267..00665275
            {
                Made spell_name;                                           // B+6C
                void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(rp(rules(), 0x8c), nullptr, spell_id);   // 0066527A..0066528F
                spell_name.own(feedback_spell_name_0059b940(spell, nullptr, spell_name.p()));   // 00665294..00665296
                (void)frame_LogCStr_005e5670(spell_name.p(), nullptr);     // 0066529B..006652A5
            }                                                              // 006652AA..006652B5
        }
    }
    (void)format_006fadb0(buffer, kFloatFmt_0074e894, static_cast<double>(challenge));   // 006652E4..00665303
    set(&line, kChallenge_00751f08);                                       // 00665306..0066530F
    cat_text(&line, buffer);                                               // 00665314..0066536E
    append(gui, &line);                                                    // 00665373..00665381
    append_text(gui, kStars_00751e04);                                     // 00665386..006653BA
    if (repository != nullptr) {                                           // 006653BF..006653C5
        void* const block = repository - 4;
        c2da_destroy_array_006fa852(repository, 8, rdi(block, 0), reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));   // 006653C7..006653D6
        live_free(block);                                                  // 006653DB..006653DC _free
    }
    return 1;                                                              // 006653E4..00665473 (strings, array destroyed)
}
