// claude_closeB_load_a.cpp (lane worker closeB, load fork): saved-game load gates of the 0886 unit, part A, transcribed
// from the original swkotor.exe (read-only):
//   00411AD0 CResGFF::ReadFieldCHAR(CResStruct*, char* label, int* ok, char default)   RET 10h  kotor_fastcall_00411ad0
//   00552350 CCombatInformation::LoadData(CResGFF*, CResStruct* root)                  RET 8, EAX 0/1 kotor_fastcall_00552350
//            (+ closeB_combat_info_load_tail_0055239d: the body from 0055239D on, for the 0886 CreatureCombatLoad port)
//   00578FC0 CSWSAreaMap::LoadSavedAreaMapData(DWORD* data, ulong count, int resX, int resY) RET 10h kotor_fastcall_00578fc0
//   004D5120 CSWSCombatRound::LoadCombatRound(CResGFF*, CResStruct*)                   RET 8, EAX 0/1 kotor_fastcall_004d5120
//   004D2450 CSWSCombatAttackData::LoadData(CResGFF*, CResStruct*)                     RET 8, EAX 1   kotor_fastcall_004d2450
//   004D1E90 CSWSCombatRoundAction::LoadData(CResGFF*, CResStruct*)                    RET 8, EAX 1   kotor_fastcall_004d1e90
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, stack args...); game heap only. The GFF status
// out-parameter of the readers is a stack slot the original reuses (an argument slot); here it is a local, which is
// unobservable. CResStruct/CResList handles get 10h-byte locals.
#include <cstdint>
#include <cstring>
#include <initializer_list>

namespace {
using U = std::uint32_t;
using U16 = std::uint16_t;
using U8 = std::uint8_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(a); }
inline U word(const void* p) { return reinterpret_cast<U>(p); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline U rd8(U a) { return *static_cast<U8*>(ptr(a)); }
inline U rd16(U a) { U16 v; std::memcpy(&v, ptr(a), 2); return v; }
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
inline void wr8(U a, U v) { *static_cast<U8*>(ptr(a)) = static_cast<U8>(v); }
inline void wr16(U a, U v) { const U16 x = static_cast<U16>(v); std::memcpy(ptr(a), &x, 2); }
inline void wrf(U a, float v) { std::memcpy(ptr(a), &v, 4); }
}

namespace kotor_q05 { struct StructHandle; struct FieldRecord; struct CResGFF { FieldRecord* __thiscall GetField(StructHandle*, unsigned); }; }
extern "C" void* kotor_cell_rules_007a3a28;                                                // [007A3A28] CSWRules*

extern "C" {
void* __cdecl live_operator_new(unsigned);                                                 // 006FA7E6
void __cdecl live_free(void*);                                                             // 006FA390
U __fastcall gff_label_00411630(void* gff, void*, void* st, const char* label);           // 00411630 GetFieldByLabel [0291]
U __fastcall gui_load_gff_struct_00411a10(void* gff, void*, void* out, void* parent, const char* label); // 00411A10 GetStructFromStruct [0292]
U __fastcall gff_list_004118c0(void* gff, void*, void* out, void* parent, const char* label); // 004118C0 GetList [0291]
U __fastcall gff_list_count_00411940(void* gff, void*, void* list);                      // 00411940 GetListCount [0291]
U __fastcall gff_list_element_00411990(void* gff, void*, void* out, void* list, U index); // 00411990 GetListElement [0291]
unsigned char __fastcall gui_load_gff_byte_00411a60(void* gff, void*, void* st, const char* label, U* ok, unsigned char def); // 00411A60 [0292]
unsigned short __fastcall recovered_gff_word_00411b40(void* gff, void*, void* st, const char* label, U* ok, unsigned short def); // 00411B40 (0886)
short __fastcall kotor_fastcall_00411bb0(void* gff, void*, void* st, const char* label, U* ok, short def); // 00411BB0 ReadFieldSHORT [p2s_b_charlist]
U __fastcall gui_load_gff_dword_00411c20(void* gff, void*, void* st, const char* label, U* ok, U def); // 00411C20 [0292]
int __fastcall gui_load_gff_int_00411c90(void* gff, void*, void* st, const char* label, U* ok, int def); // 00411C90 [0292]
float __fastcall gui_load_gff_float_00411d00(void* gff, void*, void* st, const char* label, U* ok, float def); // 00411D00 [0292]
void* __fastcall gui_load_gff_string_00411ec0(void* gff, void*, void* out, void* st, const char* label, U* ok, void* def); // 00411EC0 [0292]
void* __fastcall frame_LogStringCtor_005e5a90(void* s, void*, const char* text);          // 005E5A90 CExoString(char*) [0459]
void __fastcall frame_LogStringDtor_005e5c20(void* s, void*);                             // 005E5C20 ~CExoString [0459]
void* __fastcall async_assign_string_005e5c50(void* s, void*, void* other);               // 005E5C50 CExoString::operator= [0188]
void* __fastcall resource_append_005e9dc0(void* list, void*, void* data);                 // 005E9DC0 CExoLinkedListInternal::AddTail [0264]
void __fastcall kotor_m15_array_u16_allocate_004d2fa0(void* list, void*, U size);         // 004D2FA0 CExoArrayList<ushort>::Allocate [0811]
void __fastcall kotor_fastcall_004d1d50(void* action, void*);                             // 004D1D50 CSWSCombatRoundAction::ClearData [round_queue]

char __fastcall kotor_fastcall_00411ad0(void* gff, void*, void* st, const char* label, U* ok, char def);
U __fastcall closeB_combat_info_load_tail_0055239d(void* info, void*, void* gff, void* combat, void* root);
U __fastcall kotor_fastcall_00552350(void* info, void*, void* gff, void* root);
void __fastcall kotor_fastcall_00578fc0(void* map, void*, void* data, U count, U resx, U resy);
U __fastcall kotor_fastcall_004d5120(void* round, void*, void* gff, void* st);
U __fastcall kotor_fastcall_004d2450(void* attack, void*, void* gff, void* st);
U __fastcall kotor_fastcall_004d1e90(void* action, void*, void* gff, void* st);
}

namespace {
U rules() { return word(kotor_cell_rules_007a3a28); }
// CExoArrayList<T*>::Add inline growth (capacity 0 -> 10h, else double; copy, free old)
void array_add(U list, U value) {
    if (rd(list + 4) == rd(list + 8)) {
        const U cap = rd(list + 8) != 0 ? rd(list + 8) * 2 : 0x10;
        const U old = rd(list);
        wr(list + 8, cap);
        wr(list, word(live_operator_new(cap * 4)));
        for (U i = 0; static_cast<int>(i) < static_cast<int>(rd(list + 4)); ++i) wr(rd(list) + i * 4, rd(old + i * 4));
        if (old != 0) live_free(ptr(old));
    }
    wr(rd(list) + rd(list + 4) * 4, value);
    wr(list + 4, rd(list + 4) + 1);
}
// CExoString temp read: CExoString def(""); dst = ReadFieldCExoString(st, label, ok, def)
void read_string(void* gff, void* st, const char* label, U* ok, U dst) {
    alignas(4) unsigned char def[8], out[8];
    frame_LogStringCtor_005e5a90(def, nullptr, "");                                      // [0073D71C] ""
    void* const text = gui_load_gff_string_00411ec0(gff, nullptr, out, st, label, ok, def);
    async_assign_string_005e5c50(ptr(dst), nullptr, text);
    frame_LogStringDtor_005e5c20(out, nullptr);
    frame_LogStringDtor_005e5c20(def, nullptr);
}
}

// ===================================================================================== 00411AD0
// ReadFieldCHAR: field type 1 (CHAR) -> *ok = 1, byte at +8; else *ok = 0, default. Quirk kept: with a NULL struct or
// label the result is the low byte of the ok-pointer argument (the original returns [esp+14h], not the default).
char __fastcall kotor_fastcall_00411ad0(void* gff, void*, void* st, const char* label, U* ok, char def) {
    if (st == nullptr || label == nullptr) {                                             // 00411AD2..00411AE2
        *ok = 0;                                                                         // 00411B22..00411B27
        return static_cast<char>(word(ok) & 0xff);                                       // 00411B2D
    }
    const U index = gff_label_00411630(gff, nullptr, st, label);                         // 00411AE4..00411AE6
    const U field = word(static_cast<kotor_q05::CResGFF*>(gff)->GetField(
        static_cast<kotor_q05::StructHandle*>(st), index));                              // 00411AEB..00411AEF
    if (field != 0 && rd(field) == 1) {                                                  // 00411AF4..00411AFB
        *ok = 1;                                                                         // 00411B02
        return static_cast<char>(rd8(field + 8));                                        // 00411B08
    }
    *ok = 0;                                                                             // 00411B0F..00411B18
    return def;                                                                          // 00411B13
}

// ===================================================================================== 00552350
U __fastcall closeB_combat_info_load_tail_0055239d(void* info, void*, void* gff, void* combat, void* root) {
    const U s = word(info);
    U ok = 0;
    wr8(s + 0, gui_load_gff_byte_00411a60(gff, nullptr, combat, "NumAttacks", &ok, static_cast<unsigned char>(rd8(s))));   // 0055239D..005523C0
    wr8(s + 1, static_cast<U8>(kotor_fastcall_00411ad0(gff, nullptr, combat, "OnHandAttackMod", &ok, static_cast<char>(rd8(s + 1))))); // ..005523F0
    wr8(s + 2, static_cast<U8>(kotor_fastcall_00411ad0(gff, nullptr, combat, "OnHandDamageMod", &ok, static_cast<char>(rd8(s + 2))))); // ..00552410
    wr8(s + 3, static_cast<U8>(kotor_fastcall_00411ad0(gff, nullptr, combat, "OffHandAttackMod", &ok, static_cast<char>(rd8(s + 3))))); // ..0055242E
    wr8(s + 4, static_cast<U8>(kotor_fastcall_00411ad0(gff, nullptr, combat, "OffHandDamageMod", &ok, static_cast<char>(rd8(s + 4))))); // ..0055245F
    wr8(s + 5, gui_load_gff_byte_00411a60(gff, nullptr, combat, "ForceResistance", &ok, static_cast<unsigned char>(rd8(s + 5))));  // ..0055247B
    { const unsigned char v = gui_load_gff_byte_00411a60(gff, nullptr, combat, "ArcaneSpellFail", &ok, static_cast<unsigned char>(rd8(s + 6))); // 00552478..00552498
      (void)v; }                                                                         // quirk kept: ArcaneSpellFail result is discarded (+6 unchanged)
    wr8(s + 7, gui_load_gff_byte_00411a60(gff, nullptr, combat, "ArmorCheckPen", &ok, static_cast<unsigned char>(rd8(s + 7))));   // 0055249D..005524CB
    wr8(s + 8, gui_load_gff_byte_00411a60(gff, nullptr, combat, "UnarmedDamDice", &ok, static_cast<unsigned char>(rd8(s + 8))));  // ..005524EB
    wr8(s + 9, gui_load_gff_byte_00411a60(gff, nullptr, combat, "UnarmedDamDie", &ok, static_cast<unsigned char>(rd8(s + 9))));   // ..00552509
    wr8(s + 0xa, gui_load_gff_byte_00411a60(gff, nullptr, combat, "OnHandCritRng", &ok, static_cast<unsigned char>(rd8(s + 0xa)))); // ..0055253A
    wr8(s + 0xb, gui_load_gff_byte_00411a60(gff, nullptr, combat, "OnHandCritMult", &ok, static_cast<unsigned char>(rd8(s + 0xb)))); // ..00552553
    wr(s + 0xc, gui_load_gff_byte_00411a60(gff, nullptr, combat, "OffHandWeaponEq", &ok, static_cast<unsigned char>(rd(s + 0xc)))); // 00552556..0055257E (movzx)
    wr8(s + 0x10, gui_load_gff_byte_00411a60(gff, nullptr, combat, "OffHandCritRng", &ok, static_cast<unsigned char>(rd8(s + 0x10)))); // ..00552598
    wr8(s + 0x11, gui_load_gff_byte_00411a60(gff, nullptr, combat, "OffHandCritMult", &ok, static_cast<unsigned char>(rd8(s + 0x11)))); // ..005525BD
    wr(s + 0x30, gui_load_gff_dword_00411c20(gff, nullptr, combat, "LeftEquip", &ok, rd(s + 0x30)));   // 005525C0..005525DA
    wr(s + 0x2c, gui_load_gff_dword_00411c20(gff, nullptr, combat, "RightEquip", &ok, rd(s + 0x2c)));  // 005525DD..00552600
    read_string(gff, combat, "LeftString", &ok, s + 0x3c);                               // 005525F7..00552657
    read_string(gff, combat, "RightString", &ok, s + 0x34);                              // 0055265C..005526B6
    wr8(s + 0x44, gui_load_gff_byte_00411a60(gff, nullptr, combat, "DamageDice", &ok, 0)); // 005526BB..005526D5
    wr8(s + 0x45, gui_load_gff_byte_00411a60(gff, nullptr, combat, "DamageDie", &ok, 0));  // 005526D3..005526F7
    alignas(4) unsigned char list[0x14] = {}, element[0x10] = {};
    // AttackList / DamageList are read from the creature root (the second stack argument), not from CombatInfo.
    if (gff_list_004118c0(gff, nullptr, list, root, "AttackList") == 1) {               // 005526EE..0055270A
        U i = 0;
        if (static_cast<int>(gff_list_count_00411940(gff, nullptr, list)) > 0) {         // 00552710..00552726 (jle)
            do {
                gff_list_element_00411990(gff, nullptr, element, list, i);               // 00552730..00552741
                const U mod = word(live_operator_new(5));                                // 00552746..0055274D
                if (mod != 0) {                                                          // 00552750..00552752
                    wr8(mod, 0); wr8(mod + 1, 0); wr8(mod + 2, 0); wr8(mod + 3, 0);      // 00552754..0055275F
                    wr8(mod + 4, rd8(rules() + 0xaa));                                   // 00552763..0055276F
                }
                wr8(mod, static_cast<U8>(kotor_fastcall_00411ad0(gff, nullptr, element, "Modifier", &ok, 0))); // 00552778..005527A3 (NULL dereferenced unchecked)
                wr8(mod + 2, gui_load_gff_byte_00411a60(gff, nullptr, element, "WeaponWield", &ok, 0));       // 00552790..005527AD
                wr8(mod + 3, gui_load_gff_byte_00411a60(gff, nullptr, element, "VersusGoodEvil", &ok, 0));    // 005527AB..005527C6
                wr8(mod + 4, gui_load_gff_byte_00411a60(gff, nullptr, element, "VersusRace", &ok,
                                                        static_cast<unsigned char>(rd8(rules() + 0xaa))));    // 005527C9..005527F4
                array_add(s + 0x14, mod);                                                // 005527F7..00552828 (Allocate 005E03C0 growth)
                ++i;                                                                     // 0055282B
            } while (static_cast<int>(i) < static_cast<int>(gff_list_count_00411940(gff, nullptr, list))); // 00552833..0055283A
        }
    }
    if (gff_list_004118c0(gff, nullptr, list, root, "DamageList") == 1) {               // 00552840..00552859
        U i = 0;
        if (static_cast<int>(gff_list_count_00411940(gff, nullptr, list)) > 0) {         // 0055285F..00552873
            do {
                gff_list_element_00411990(gff, nullptr, element, list, i);               // 00552880..0055288D
                const U mod = word(live_operator_new(5));                                // 00552892..00552899
                if (mod != 0) {
                    wr8(mod, 0); wr8(mod + 1, 0); wr8(mod + 2, 0); wr8(mod + 3, 0);      // 005528A0..005528AB
                    wr8(mod + 4, rd8(rules() + 0xaa));                                   // 005528AF..005528BB
                }
                wr8(mod, static_cast<U8>(kotor_fastcall_00411ad0(gff, nullptr, element, "Modifier", &ok, 0))); // 005528C4..005528E3
                wr8(mod + 1, gui_load_gff_byte_00411a60(gff, nullptr, element, "ModifierType", &ok, 0));      // 005528DC..0055290A
                wr8(mod + 2, gui_load_gff_byte_00411a60(gff, nullptr, element, "WeaponWield", &ok, 0));       // 005528F7..00552914
                wr8(mod + 3, gui_load_gff_byte_00411a60(gff, nullptr, element, "VersusGoodEvil", &ok, 0));    // 00552912..0055292D
                wr8(mod + 4, gui_load_gff_byte_00411a60(gff, nullptr, element, "VersusRace", &ok,
                                                        static_cast<unsigned char>(rd8(rules() + 0xaa))));    // 00552930..0055295B
                array_add(s + 0x20, mod);                                                // 0055295E..005529CC (inline growth)
                ++i;
            } while (static_cast<int>(i) < static_cast<int>(gff_list_count_00411940(gff, nullptr, list))); // 005529C4..005529DA
        }
    }
    return 1;                                                                            // 005529E7
}
U __fastcall kotor_fastcall_00552350(void* info, void*, void* gff, void* root) {
    alignas(4) unsigned char combat[0x10] = {};
    if (gui_load_gff_struct_00411a10(gff, nullptr, combat, root, "CombatInfo") == 0) return 0; // 00552372..0055239A
    return closeB_combat_info_load_tail_0055239d(info, nullptr, gff, combat, root);      // 0055239D..
}

// ===================================================================================== 00578FC0
void __fastcall kotor_fastcall_00578fc0(void* map, void*, void* data, U count, U resx, U resy) {
    const U s = word(map);
    const U size = rd(s + 4);                                                            // 00578FC2
    if (size == 0) return;                                                               // 00578FC5..00578FC7 (jbe)
    if (resx != rd(s + 8) || resy != rd(s + 0xc) || count != size) return;               // 00578FC9..00578FE2
    std::memcpy(ptr(rd(s)), data, count * 4);                                            // 00578FE4..00578FEB rep movsd
}

// ===================================================================================== 004D1E90
U __fastcall kotor_fastcall_004d1e90(void* action, void*, void* gff, void* st) {
    const U s = word(action);
    U ok = 0;
    wr(s, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "ActionTimer", &ok, 0)));       // 004D1E9B..004D1EC0
    wr16(s + 4, recovered_gff_word_00411b40(gff, nullptr, st, "Animation", &ok, 0));                 // ..004D1ED6
    wr(s + 8, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "AnimationTime", &ok, 0))); // ..004D1EE1
    wr(s + 0xc, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "NumAttacks", &ok, 0)));  // ..004D1F05
    wr8(s + 0x10, gui_load_gff_byte_00411a60(gff, nullptr, st, "ActionType", &ok, 0));              // ..004D1F1C
    wr(s + 0x14, gui_load_gff_dword_00411c20(gff, nullptr, st, "Target", &ok, 0));                   // ..004D1F26
    wr(s + 0x18, gui_load_gff_byte_00411a60(gff, nullptr, st, "Retargettable", &ok, 0));            // ..004D1F4A (movzx)
    wr(s + 0x1c, gui_load_gff_dword_00411c20(gff, nullptr, st, "InventorySlot", &ok, 0));            // ..004D1F57
    wr(s + 0x20, gui_load_gff_dword_00411c20(gff, nullptr, st, "TargetRepository", &ok, 0));         // ..004D1F6C
    return 1;
}

// ===================================================================================== 004D2450
U __fastcall kotor_fastcall_004d2450(void* attack, void*, void* gff, void* st) {
    const U s = word(attack);
    U ok = 0;
    wr8(s, gui_load_gff_byte_00411a60(gff, nullptr, st, "AttackGroup", &ok, 0));                    // 004D2474..004D2499
    wr16(s + 8, recovered_gff_word_00411b40(gff, nullptr, st, "AnimationLength", &ok, 0));           // ..004D24AF
    wr(s + 0x18, gui_load_gff_dword_00411c20(gff, nullptr, st, "MissedBy", &ok, 0));                 // ..004D24BA
    wr8(s + 0x5c, gui_load_gff_byte_00411a60(gff, nullptr, st, "AttackResult", &ok, 0));            // ..004D24DE
    wr(s + 0xc, gui_load_gff_dword_00411c20(gff, nullptr, st, "ReactObject", &ok, 0));               // ..004D24F5
    wr16(s + 0x10, recovered_gff_word_00411b40(gff, nullptr, st, "ReaxnDelay", &ok, 0));             // ..004D24FF
    wr16(s + 0x12, recovered_gff_word_00411b40(gff, nullptr, st, "ReaxnAnimation", &ok, 0));         // ..004D2524
    wr16(s + 0x14, recovered_gff_word_00411b40(gff, nullptr, st, "ReaxnAnimLength", &ok, 0));        // ..004D2539
    wr8(s + 0x3c, gui_load_gff_byte_00411a60(gff, nullptr, st, "Concealment", &ok, 0));             // ..004D2547
    wr16(s + 0x64, recovered_gff_word_00411b40(gff, nullptr, st, "AttackType", &ok, 0));             // ..004D256B
    wr8(s + 0x3b, gui_load_gff_byte_00411a60(gff, nullptr, st, "AttackMode", &ok, 0));              // ..004D2583
    wr(s + 0x40, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "RangedAttack", &ok, 0))); // ..004D258D
    wr(s + 0x44, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "SneakAttack", &ok, 0)));  // ..004D25B1
    wr8(s + 0x3a, gui_load_gff_byte_00411a60(gff, nullptr, st, "WeaponAttackType", &ok, 0));        // ..004D25C8
    wrf(s + 0x68, gui_load_gff_float_00411d00(gff, nullptr, st, "RangedTargetX", &ok, 0.0f));        // ..004D25D0 fstp dword
    wrf(s + 0x6c, gui_load_gff_float_00411d00(gff, nullptr, st, "RangedTargetY", &ok, 0.0f));        // ..004D25E7
    wrf(s + 0x70, gui_load_gff_float_00411d00(gff, nullptr, st, "RangedTargetZ", &ok, 0.0f));        // ..004D25FE
    alignas(4) unsigned char list[0x14] = {}, element[0x10] = {};
    if (gff_list_004118c0(gff, nullptr, list, st, "DamageList") == 1) {                 // 004D2601..004D2616
        U i = 0;
        if (gff_list_count_00411940(gff, nullptr, list) != 0) {                          // 004D2618..004D2628 (jbe)
            U slot = s + 0x1c;                                                           // 004D262A..004D262D
            do {
                if (gff_list_element_00411990(gff, nullptr, element, list, i) == 1)      // 004D2631..004D2646
                    wr16(slot, static_cast<U16>(kotor_fastcall_00411bb0(gff, nullptr, element, "DamageValue", &ok, 0))); // 004D2648..004D2664
                slot += 2;                                                               // 004D2667..004D2672 (no bound: quirk kept)
                ++i;
            } while (i < gff_list_count_00411940(gff, nullptr, list));                   // 004D2676..004D2681 (jb)
        }
    }
    wr(s + 0x48, gui_load_gff_byte_00411a60(gff, nullptr, st, "KillingBlow", &ok, 0));              // 004D2683..004D26A6
    wr(s + 0x4c, gui_load_gff_byte_00411a60(gff, nullptr, st, "CoupDeGrace", &ok, 0));              // ..004D26C3
    wr(s + 0x50, gui_load_gff_byte_00411a60(gff, nullptr, st, "CriticalThreat", &ok, 0));           // ..004D26DD
    wr(s + 0x58, gui_load_gff_byte_00411a60(gff, nullptr, st, "AttackDeflected", &ok, 0));          // ..004D26F4
    wr(s + 0x74, gui_load_gff_dword_00411c20(gff, nullptr, st, "AmmoItem", &ok, 0));                 // ..004D2708
    read_string(gff, st, "AttackDebugText", &ok, s + 0x78);                              // 004D26FF..004D275B
    read_string(gff, st, "DamageDebugText", &ok, s + 0x80);                              // 004D2760..004D27B9
    return 1;
}

// ===================================================================================== 004D5120
U __fastcall kotor_fastcall_004d5120(void* round, void*, void* gff, void* st) {
    const U s = word(round);
    U ok = 0;
    wr(s + 0x934, gui_load_gff_byte_00411a60(gff, nullptr, st, "RoundStarted", &ok, static_cast<unsigned char>(rd8(s + 0x934)))); // 004D512C..004D5153
    if (rd(s + 0x934) == 0) return 0;                                                    // 004D5151..004D5164
    wr(s + 0x938, gui_load_gff_byte_00411a60(gff, nullptr, st, "SpellCastRound", &ok, static_cast<unsigned char>(rd8(s + 0x938)))); // ..004D5191
    wr(s + 0x978, gui_load_gff_byte_00411a60(gff, nullptr, st, "DeflectArrow", &ok, static_cast<unsigned char>(rd8(s + 0x978))));   // ..004D51B5
    wr(s + 0x97c, gui_load_gff_byte_00411a60(gff, nullptr, st, "WeaponSucks", &ok, static_cast<unsigned char>(rd8(s + 0x97c))));    // ..004D51DE
    wr(s + 0x9ac, gui_load_gff_dword_00411c20(gff, nullptr, st, "DodgeTarget", &ok, rd(s + 0x9ac)));                                 // ..004D5200
    wr(s + 0x98c, gui_load_gff_dword_00411c20(gff, nullptr, st, "NewAttackTarget", &ok, rd(s + 0x98c)));                             // ..004D520B
    wr(s + 0x958, gui_load_gff_byte_00411a60(gff, nullptr, st, "RoundPaused", &ok, static_cast<unsigned char>(rd8(s + 0x958))));    // ..004D5243
    wr(s + 0x95c, gui_load_gff_dword_00411c20(gff, nullptr, st, "RoundPausedBy", &ok, rd(s + 0x95c)));                               // ..004D5256
    wr(s + 0x964, gui_load_gff_byte_00411a60(gff, nullptr, st, "InfinitePause", &ok, static_cast<unsigned char>(rd8(s + 0x964))));  // ..004D5283
    wr(s + 0x960, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "PauseTimer", &ok, static_cast<int>(rd(s + 0x960))))); // ..004D529F
    wr(s + 0x964, 0);                                                                    // 004D52A5 quirk kept: InfinitePause overwritten with 0
    wr(s + 0x9b8, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "Engaged", &ok, 0)));   // 004D5291..004D52BE
    wr(s + 0x9bc, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "Master", &ok, 0)));    // ..004D52CE
    wr(s + 0x9c0, gui_load_gff_dword_00411c20(gff, nullptr, st, "MasterID", &ok, 0x7f000000u));      // ..004D52FA
    wr(s + 0x944, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "Timer", &ok, static_cast<int>(rd(s + 0x944)))));        // ..004D5305
    wr(s + 0x94c, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "RoundLength", &ok, static_cast<int>(rd(s + 0x94c))))); // ..004D532B
    wr(s + 0x950, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "OverlapAmount", &ok, static_cast<int>(rd(s + 0x950))))); // ..004D5357
    wr(s + 0x954, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "BleedTimer", &ok, static_cast<int>(rd(s + 0x954)))));  // ..004D5362
    wr8(s + 0x96c, gui_load_gff_byte_00411a60(gff, nullptr, st, "CurrentAttack", &ok, static_cast<unsigned char>(rd8(s + 0x96c))));  // ..004D5383
    wr16(s + 0x930, recovered_gff_word_00411b40(gff, nullptr, st, "AttackID", &ok, static_cast<unsigned short>(rd16(s + 0x930)))); // ..004D53B1
    wr8(s + 0x974, gui_load_gff_byte_00411a60(gff, nullptr, st, "AttackGroup", &ok, static_cast<unsigned char>(rd8(s + 0x974))));    // ..004D53C7
    wr(s + 0x980, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "ParryIndex", &ok, static_cast<int>(rd(s + 0x980)))));  // ..004D53ED
    wr(s + 0x984, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "NumAOOs", &ok, static_cast<int>(rd(s + 0x984)))));     // ..004D5419
    wr(s + 0x988, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "NumCleaves", &ok, static_cast<int>(rd(s + 0x988)))));  // ..004D5424
    wr(s + 0x990, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "OnHandAttacks", &ok, static_cast<int>(rd(s + 0x990))))); // ..004D544A
    wr(s + 0x994, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "OffHandAttacks", &ok, static_cast<int>(rd(s + 0x994))))); // ..004D5476
    wr(s + 0x9a0, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "AdditAttacks", &ok, static_cast<int>(rd(s + 0x9a0)))));  // ..004D5481
    (void)gui_load_gff_int_00411c90(gff, nullptr, st, "EffectAttacks", &ok, static_cast<int>(rd(s + 0x9a4)));                       // 004D5487..004D549B
    wr(s + 0x9a4, 0);                                                                    // 004D54AE quirk kept: EffectAttacks discarded, +9A4 = 0
    wr8(s + 0x9a8, gui_load_gff_byte_00411a60(gff, nullptr, st, "ParryActions", &ok, static_cast<unsigned char>(rd8(s + 0x9a8))));   // ..004D54D5
    wr(s + 0x998, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "OffHandTaken", &ok, static_cast<int>(rd(s + 0x998))))); // ..004D54E0
    wr(s + 0x99c, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "ExtraTaken", &ok, static_cast<int>(rd(s + 0x99c)))));   // ..004D550C
    alignas(4) unsigned char list[0x14] = {}, element[0x10] = {};
    if (gff_list_004118c0(gff, nullptr, list, st, "AttackList") == 1 &&                  // 004D54FF..004D551A
        static_cast<int>(gff_list_count_00411940(gff, nullptr, list)) > 0) {             // 004D551C..004D552A
        U attack = s + 4;                                                                // 004D552C..004D552F
        U i = 0;
        do {
            if (gff_list_element_00411990(gff, nullptr, element, list, i) == 1)          // 004D5533..004D5548
                (void)kotor_fastcall_004d2450(ptr(attack), nullptr, gff, element);       // 004D554A..004D5554
            attack += 0x14c;                                                             // 004D5559..004D5567
            ++i;
        } while (static_cast<int>(i) < static_cast<int>(gff_list_count_00411940(gff, nullptr, list))); // 004D556B..004D5576
    }
    for (const U member : {0x918u, 0x924u}) {                                            // SpecAttackList / SpecAttackIdList
        const char* const list_label = member == 0x918u ? "SpecAttackList" : "SpecAttackIdList";
        const char* const field_label = member == 0x918u ? "SpecialAttack" : "SpecialAttackId";
        if (gff_list_004118c0(gff, nullptr, list, st, list_label) != 1) continue;        // 004D5578..004D558D / 004D5636..004D564B
        const U count = gff_list_count_00411940(gff, nullptr, list);                     // 004D5593..004D55A1 / 004D5651..004D565F
        const U array = s + member;                                                      // 004D55A5 / 004D5663
        if (count == 0) {
            if (rd(array) != 0) {                                                        // 004D55AD..004D55B2
                live_free(ptr(rd(array)));                                               // 004D55B4..004D55B5
                wr(array, 0); wr(array + 8, 0);                                          // 004D55BD..004D55C4
            }
        } else {
            kotor_m15_array_u16_allocate_004d2fa0(ptr(array), nullptr, count);          // 004D55CD..004D55D0
        }
        wr(array + 4, count);                                                            // 004D55D9 / 004D5697
        // quirk kept: both loops are bounded by the SpecAttackList size (+91C)
        for (U i = 0; static_cast<int>(i) < static_cast<int>(rd(s + 0x91c)); ++i) {    // 004D55DC..004D5634 / 004D569A..004D56F2
            if (gff_list_element_00411990(gff, nullptr, element, list, i) == 1) {        // 004D55E8..004D55FD
                const U slot = rd(s + member) + i * 2;                                   // 004D55FF..004D561B
                wr16(slot, recovered_gff_word_00411b40(gff, nullptr, element, field_label, &ok, 0)); // 004D5605..004D5628
            }
        }
    }
    if (gff_list_004118c0(gff, nullptr, list, st, "SchedActionList") == 1 &&             // 004D56F4..004D5709
        static_cast<int>(gff_list_count_00411940(gff, nullptr, list)) > 0) {             // 004D570B..004D571B
        U i = 0;
        do {
            if (gff_list_element_00411990(gff, nullptr, element, list, i) == 1) {        // 004D5720..004D5735
                U action = word(live_operator_new(0x88));                                // 004D5737..004D5745
                if (action != 0) {                                                       // 004D5748..004D574A
                    wr(action + 0x38, 0); wr(action + 0x3c, 0); wr(action + 0x40, 0);    // 004D574C..004D5752
                    kotor_fastcall_004d1d50(ptr(action), nullptr);                       // 004D5755 (ECX preserved -> EBX)
                }
                (void)kotor_fastcall_004d1e90(ptr(action), nullptr, gff, element);       // 004D575C..004D5764 (NULL unchecked)
                (void)resource_append_005e9dc0(ptr(rd(rd(s + 0x9b0))), nullptr, ptr(action)); // 004D5769..004D5772
            }
            ++i;
        } while (static_cast<int>(i) < static_cast<int>(gff_list_count_00411940(gff, nullptr, list))); // 004D5777..004D5786
    }
    return 1;                                                                            // 004D578B
}
