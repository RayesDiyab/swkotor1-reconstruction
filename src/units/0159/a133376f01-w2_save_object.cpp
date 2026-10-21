#include "cb64dd2119-w2_save_object.hpp"
#include <cstdint>
namespace w2::save_object {
namespace {
using I = std::int32_t;
constexpr Word kInvalid = 0x7f000000u;   // [00746644]
// .rdata labels used below (addresses of the original)
enum : Word {
    kVarTable = 0x0074a11c, kName = 0x00745eec, kType = 0x00745e38, kValue = 0x00744bfc, kSWVarTable = 0x0074a14c,
    kBitArray = 0x0074a140, kVariable = 0x0074a134, kByteArray = 0x0074a128, kCommandable = 0x00745f08,
    kEffectList = 0x00745eac, kId = 0x00747434, kSubType = 0x0074742c, kDuration = 0x00747420, kSkipOnLoad = 0x00747414,
    kExpireDay = 0x00747408, kExpireTime = 0x007473fc, kCreatorId = 0x007473f0, kSpellId = 0x00746d30,
    kIsExposed = 0x007473e4, kNumIntegers = 0x007473d8, kIntList = 0x00746604, kFloatList = 0x007465e8,
    kStringList = 0x007465b4, kObjectList = 0x0074660c, kOrientX = 0x00745df0, kOrientY = 0x00745de0,
    kOrientZ = 0x00745dd0, kPosX = 0x0074423c, kPosY = 0x00744230, kPosZ = 0x00744224,
};
}

const char* save_object_label(Word a) {
    switch (a) {
    case kVarTable: return "VarTable"; case kName: return "Name"; case kType: return "Type"; case kValue: return "Value";
    case kSWVarTable: return "SWVarTable"; case kBitArray: return "BitArray"; case kVariable: return "Variable";
    case kByteArray: return "ByteArray"; case kCommandable: return "Commandable"; case kEffectList: return "EffectList";
    case kId: return "Id"; case kSubType: return "SubType"; case kDuration: return "Duration"; case kSkipOnLoad: return "SkipOnLoad";
    case kExpireDay: return "ExpireDay"; case kExpireTime: return "ExpireTime"; case kCreatorId: return "CreatorId";
    case kSpellId: return "SpellId"; case kIsExposed: return "IsExposed"; case kNumIntegers: return "NumIntegers";
    case kIntList: return "IntList"; case kFloatList: return "FloatList"; case kStringList: return "StringList";
    case kObjectList: return "ObjectList"; case kOrientX: return "OrientationX"; case kOrientY: return "OrientationY";
    case kOrientZ: return "OrientationZ"; case kPosX: return "PositionX"; case kPosY: return "PositionY"; case kPosZ: return "PositionZ";
    default: return nullptr;
    }
}

// 004CEC50: effects, script vars, SW vars, action queue, then BYTE "Commandable" = [object+E8h]. EAX = the BYTE write.
Word save_object_state_004cec50(Memory& m, Ports& p, Word o, Word gff, Word st, Word s) {
    (void)save_effect_list_004cc9d0(m, p, o, gff, st, s);
    (void)save_var_table_0059adb0(m, p, o + 0x100, gff, st, s);
    (void)sw_var_table_save_0059b250(m, p, o + 0x110, gff, st, s);
    (void)p.save_action_queue_004cc7e0(o, gff, st);
    return p.gff_byte_00412620(gff, st, m.read8(o + 0xe8), kCommandable);
}

// 004CC9D0: list "EffectList" only when [object+128h] > 0 (signed); one element (type 2) per effect at [[+124h]+4i].
// EAX on return is [object+128h] as last read (both paths).
Word save_effect_list_004cc9d0(Memory& m, Ports& p, Word o, Word gff, Word st, Word s) {
    if (static_cast<I>(m.read(o + 0x128)) <= 0) return m.read(o + 0x128);
    const Word list = s, elem = s + 0x18;   // original: list local, element struct in the arg-2 slot
    (void)p.gff_add_list_00412450(gff, list, st, kEffectList);
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(o + 0x128)); ++i) {
        (void)p.gff_add_list_element_004124e0(gff, elem, list, 2);
        (void)save_game_effect_00503790(m, p, m.read(m.read(o + 0x124) + 4 * i), gff, elem, s + 0x20);
    }
    return m.read(o + 0x128);
}

// 00503790: scalar header, IntList (only when NumIntegers > 0), FloatList (4), StringList (6), ObjectList (4).
Word save_game_effect_00503790(Memory& m, Ports& p, Word e, Word gff, Word st, Word s) {
    (void)p.gff_dword64_00412800(gff, st, m.read(e), m.read(e + 4), kId);
    (void)p.gff_word_004126c0(gff, st, m.read16(e + 8), kType);
    (void)p.gff_word_004126c0(gff, st, m.read16(e + 0x0a), kSubType);
    (void)p.gff_float_00412870(gff, st, m.read(e + 0x0c), kDuration);
    (void)p.gff_byte_00412620(gff, st, m.read(e + 0x88), kSkipOnLoad);   // full dword pushed; the writer stores the low byte
    (void)p.gff_dword_00412760(gff, st, m.read(e + 0x10), kExpireDay);
    (void)p.gff_dword_00412760(gff, st, m.read(e + 0x14), kExpireTime);
    (void)p.gff_dword_00412760(gff, st, m.read(e + 0x18), kCreatorId);
    (void)p.gff_dword_00412760(gff, st, m.read(e + 0x1c), kSpellId);
    (void)p.gff_int_004127b0(gff, st, m.read(e + 0x20), kIsExposed);
    (void)p.gff_int_004127b0(gff, st, m.read(e + 0x30), kNumIntegers);
    const Word list = s, elem = s + 0x18, str = s + 0x20;
    if (static_cast<I>(m.read(e + 0x30)) > 0) {                             // 00503888 JLE
        (void)p.gff_add_list_00412450(gff, list, st, kIntList);
        for (Word k = 0; k < m.read(e + 0x30); ++k) {                        // unsigned JB
            (void)p.gff_add_list_element_004124e0(gff, elem, list, 3);
            (void)p.gff_int_004127b0(gff, elem, m.read(m.read(e + 0x34) + 4 * k), kValue);
        }
    }
    (void)p.gff_add_list_00412450(gff, list, st, kFloatList);
    for (Word k = 0; k < 4; ++k) {
        (void)p.gff_add_list_element_004124e0(gff, elem, list, 4);
        (void)p.gff_float_00412870(gff, elem, m.read(e + 0x38 + 4 * k), kValue);
    }
    (void)p.gff_add_list_00412450(gff, list, st, kStringList);
    for (Word k = 0; k < 6; ++k) {
        (void)p.gff_add_list_element_004124e0(gff, elem, list, 5);
        (void)p.gff_string_00412970(gff, elem, effect_copy_string_00503700(m, p, e, str, k), kValue);
        p.str_destroy_005e5c20(str);
    }
    (void)p.gff_add_list_00412450(gff, list, st, kObjectList);
    Word r = 0;
    for (Word k = 0; k < 4; ++k) {
        (void)p.gff_add_list_element_004124e0(gff, elem, list, 6);
        r = p.gff_dword_00412760(gff, elem, m.read(e + 0x78 + 4 * k), kValue);
    }
    return r;                                                                // EAX = last WriteFieldDWORD
}

// 00503700: out = CExoString(effect strings[index]) (8-byte entries at +48h). Returns out.
Word effect_copy_string_00503700(Memory&, Ports& p, Word e, Word out, Word index) {
    p.str_copy_005e5b00(out, e + 0x48 + 8 * index);
    return out;
}

// 0059ADB0: list "VarTable" of {Name, Type, Value}; entries are 10h bytes {CExoString name, type, value}.
// Type 1 INT, 2 FLOAT, 3 STRING, 4 OBJECT (default 7F000000h), 5 LOCATION (struct "Value", type 1); other types: no Value.
Word save_var_table_0059adb0(Memory& m, Ports& p, Word t, Word gff, Word st, Word s) {
    const Word elem = s, list = s + 0x08, sub = s + 0x20, str = s + 0x28, loc = s + 0x30, tmp = s + 0x48;
    p.location_construct_004ca7a0(loc);                                      // 0059ADD1
    Word r = p.gff_add_list_00412450(gff, list, st, kVarTable);
    if (static_cast<I>(m.read(t + 4)) <= 0) return r;                       // 0059ADFD: EAX = AddList's result
    Word i = 0;
    for (;;) {
        const Word entry = m.read(t) + 0x10 * i;
        (void)p.gff_add_list_element_004124e0(gff, elem, list, 0);
        (void)p.gff_string_00412970(gff, elem, entry, kName);
        (void)p.gff_dword_00412760(gff, elem, m.read(m.read(t) + 0x10 * i + 8), kType);
        const Word e2 = m.read(t) + 0x10 * i;                                // re-read (0059AE4B)
        switch (m.read(e2 + 8)) {
        case 1: { const Word v = p.var_match_0059a390(t, e2, 1, 0);
                  (void)p.gff_int_004127b0(gff, elem, v != 0 ? m.read(v + 0x0c) : 0, kValue); break; }
        case 2: { const Word v = p.var_match_0059a390(t, e2, 2, 0);
                  (void)p.gff_float_00412870(gff, elem, v != 0 ? m.read(v + 0x0c) : 0, kValue); break; }
        case 3: { (void)p.gff_string_00412970(gff, elem, p.var_string_0059a590(t, str, e2), kValue);
                  p.str_destroy_005e5c20(str); break; }
        case 4: { const Word v = p.var_match_0059a390(t, e2, 4, 0);
                  (void)p.gff_dword_00412760(gff, elem, v != 0 ? m.read(v + 0x0c) : kInvalid, kValue); break; }
        case 5: { const Word got = var_get_location_0059a650(m, p, t, tmp, e2, s + 0x60);
                  for (Word k = 0; k < 0x18; k += 4) m.write(loc + k, m.read(got + k));
                  (void)p.gff_add_struct_to_struct_004125b0(gff, sub, elem, kValue, 1);
                  (void)save_location_004ca8e0(m, p, loc, gff, sub); break; }
        default: break;                                                      // 0059AE58 JA: no Value field
        }
        ++i;
        if (static_cast<I>(i) >= static_cast<I>(m.read(t + 4))) return i;  // EAX = the arg-1 counter slot
    }
}

// 0059A650: local CScriptLocation (zeroed), filled from the matched type-5 variable's location pointer; copied to out.
Word var_get_location_0059a650(Memory& m, Ports& p, Word t, Word out, Word name, Word s) {
    p.location_construct_004ca7a0(s);
    const Word v = p.var_match_0059a390(t, name, 5, 0);
    if (v != 0) { const Word src = m.read(v + 0x0c); for (Word k = 0; k < 0x18; k += 4) m.write(s + k, m.read(src + k)); }
    for (Word k = 0; k < 0x18; k += 4) m.write(out + k, m.read(s + k));
    return out;
}

// 004CA8E0: FLOAT OrientationX/Y/Z (+0C/+10/+14), PositionX/Y/Z (+0/+4/+8). Returns 1.
Word save_location_004ca8e0(Memory& m, Ports& p, Word l, Word gff, Word st) {
    (void)p.gff_float_00412870(gff, st, m.read(l + 0x0c), kOrientX);
    (void)p.gff_float_00412870(gff, st, m.read(l + 0x10), kOrientY);
    (void)p.gff_float_00412870(gff, st, m.read(l + 0x14), kOrientZ);
    (void)p.gff_float_00412870(gff, st, m.read(l), kPosX);
    (void)p.gff_float_00412870(gff, st, m.read(l + 4), kPosY);
    (void)p.gff_float_00412870(gff, st, m.read(l + 8), kPosZ);
    return 1;
}

// 0059B250: struct "SWVarTable" (type 0) with list BitArray (3 DWORD "Variable") and ByteArray (8 BYTE "Variable").
Word sw_var_table_save_0059b250(Memory& m, Ports& p, Word t, Word gff, Word st, Word s) {
    const Word sub = s, list = s + 0x08, elem = s + 0x20;
    (void)p.gff_add_struct_to_struct_004125b0(gff, sub, st, kSWVarTable, 0);
    (void)p.gff_add_list_00412450(gff, list, sub, kBitArray);
    for (Word k = 0; k < 3; ++k) {
        (void)p.gff_add_list_element_004124e0(gff, elem, list, 0);
        (void)p.gff_dword_00412760(gff, elem, m.read(t + 4 * k), kVariable);
    }
    (void)p.gff_add_list_00412450(gff, list, sub, kByteArray);
    Word r = 0;
    for (Word k = 0; k < 8; ++k) {
        (void)p.gff_add_list_element_004124e0(gff, elem, list, 0);
        r = p.gff_byte_00412620(gff, elem, m.read8(t + 0x0c + k), kVariable);
    }
    return r;
}
}
