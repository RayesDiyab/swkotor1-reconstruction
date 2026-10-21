#pragma once
// Lane effects-itemprop: shared conventions for the CSWSItemPropertyHandler::Apply* / Remove* callbacks
// (004E5490..004EA9E0), reconstructed from the original swkotor.exe (read-only, capstone linear sweep).
//
// ABI (derived from the dispatchers, not from names):
//  * 004E5410 CSWSItemPropertyHandler::ApplyItemProperty(item, property, creature, slot, load_game) RET 14h:
//    bounds-checks property->type (WORD +0) < 3Ch, loads table [this+4][type] and calls it with ECX = this
//    (unchanged) and the five stack arguments in the same order -> every Apply callback is
//      thiscall(CSWSItem* item, CSWItemProperty* property, CSWSCreature* creature, U slot, U load_game) RET 14h
//    export: extern "C" unsigned __fastcall kotor_fastcall_<addr>(void* self, void*, void* item, void* property,
//            void* creature, unsigned slot, unsigned load_game)                         -> @kotor_fastcall_<addr>@28
//    (same shape as the resolved @recovered_property_effect_004e5c60@28 / @recovered_property_effect_004e60c0@28).
//  * 004E5450 CSWSItemPropertyHandler::RemoveItemProperty(item, property, creature, slot) RET 10h: table
//    [this+8][type], ECX = this, four stack arguments in order -> every Remove callback is
//      thiscall(CSWSItem* item, CSWItemProperty* property, CSWSCreature* creature, U slot) RET 10h
//    export: extern "C" unsigned __fastcall kotor_fastcall_<addr>(void* self, void*, void* item, void* property,
//            void* creature, unsigned slot)                                              -> @kotor_fastcall_<addr>@24
//  * Every callback returns EAX = 0 (xor eax,eax before ret).  The RET of every function was checked.
//    Exceptions: 004E9230 ApplyImprovedSavingThrow / 004E92E0 ApplyReducedSavingThrows are table "Remove"-range
//    addresses but are Apply callbacks (RET 14h); helpers 004E67D0 (RET 14h) and 004E9390 (RET 10h) are called
//    directly by their wrappers, not through the tables.
//
// Data (observed in the callbacks):
//  * CSWSItem: +0C base item id (CSWBaseItemArray::GetBaseItem index), +14 object id (effect creator / match key).
//  * CSWItemProperty: +0 WORD property type, +2 WORD subtype, +6 WORD cost value (row in the IPRP cost table),
//    +8 BYTE param1, +9 BYTE param1 value.
//  * CGameEffect (operator new 8Ch): +0/+4 64-bit id, +8 WORD type, +0A WORD flags (bits 0..2 duration type),
//    +18 creator id; integers via GetInteger/SetInteger.
//  * CSWSObject / CSWSCreature +124: CExoArrayList<CGameEffect*> applied effects (+0 data, +4 count).
//  * Duration type stored for item-property effects: WORD [007473BC] = 3 (equipped).
//  * CSWRules* [007A3A28]: +34 CSWBaseItemArray*, +AA BYTE "all racial types" value, +B8 CTwoDimArrays*.
//  * operator new failures are not checked before the effect is used (null dereference), exactly as the original;
//    the SEH frames around new+ctor only free the block if the constructor throws (no native throw path exists).
#include "842d6b1618-w5_t3.hpp"
#include <cstdint>

extern "C" {
using w5::U; using w5::I;
// ---- CGameEffect (exact decorated arity checked against the original RET) ----
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);         // CGameEffect::CGameEffect RET 4 [0886]
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);              // CGameEffect::GetInteger RET 4 [W1 b7]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);   // CGameEffect::SetInteger RET 8 [0886]
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);              // CGameEffect::SetCreator RET 4 [0886]
void __fastcall recovered_effect_copy_00504090(void* effect, void*, void* source, U skip);  // CGameEffect::CopyEffect RET 8 [1091]
// ---- CSWSObject ----
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b); // CSWSObject::ApplyEffect RET 0Ch [0886]
U __fastcall recovered_object_remove_effect_004d06c0(void* object, void*, U id_lo, U id_hi);   // CSWSObject::RemoveEffectById RET 8 [W1 b7]
// ---- items / rules ----
U __fastcall recovered_item_property_has_00555c90(void* item, void*, U type, U subtype);    // CSWSItem::GetPropertyByTypeExists RET 8 [1025]
U __fastcall kotor_fastcall_00554050(void* item, void*);                                   // CSWSItem::GetDamageFlags [claude_combat_attack_damagestats]
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, I id); // CSWBaseItemArray::GetBaseItem RET 4 [0820]
void* __fastcall kotor_fastcall_005c49a0(void* two_dim, void*, U index);                   // CTwoDimArrays::GetIPRPCostTable RET 4 [claude_equip_items2]
// C2DA::GetINTEntry 00413660 RET 0Ch is declared by w5_t3.hpp (c2da_current_get_int_00413660 [0376]).
// CExoString(const char*) 005E5A90 / ~CExoString 005E5C20 are declared by w5_t2.hpp (frame_LogStringCtor/Dtor [0459]).
}

namespace ipx {
using namespace w5;
struct ExoString { char* text; U length; };
static_assert(sizeof(ExoString) == 8, "CExoString is 8 bytes");

constexpr U16 k_duration_equipped_007473bc = 3;   // .rdata 007473BC (WORD)
const char k_value_00744bfc[] = "Value";          // .rdata 00744BFC

inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void* two_dim() { return at<void*>(rules(), 0xb8); }                       // [[007A3A28]+B8] CTwoDimArrays*
inline U racial_all() { return at<U8>(rules(), 0xaa); }                           // movzx byte [[007A3A28]+AA]
inline void* base_item(void* item) {                                              // GetBaseItem([[7A3A28]+34], item+0C)
    return kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(rules(), 0x34), nullptr, at<I>(item, 0xc));
}
// push 8Ch; call operator new; test eax; je -> null; push flag; call CGameEffect::CGameEffect
inline void* new_effect(U flag) {
    void* memory = live_operator_new(0x8c);
    return memory ? recovered_effect_construct_00503e40(memory, nullptr, flag) : nullptr;
}
// mov [e+8],type  ;  xor r,r; mov r16,[e+0A]; and r,FFF8; or r16,[007473BC]; mov [e+0A],r16
inline void set_type_equipped(void* e, U type) {
    at<U16>(e, 8) = static_cast<U16>(type);
    at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xfff8u) | k_duration_equipped_007473bc);
}
inline void set_int(void* e, U index, U value) { recovered_effect_integer_005036a0(e, nullptr, index, value); }
inline U get_int(void* e, U index) { return recovered_effect_integer_00503690(e, nullptr, index); }
inline void set_creator(void* e, void* item) { recovered_effect_creator_00503a00(e, nullptr, at<U>(item, 0x14)); }
inline void apply(void* creature, void* e, U a, U b) { recovered_effect_apply_004d14f0(creature, nullptr, e, a, b); }
inline void remove_by_id(void* creature, void* e) {                                // push [e+4]; push [e]; RemoveEffectById
    recovered_object_remove_effect_004d06c0(creature, nullptr, at<U>(e, 0), at<U>(e, 4));
}
inline U16 prop_type(void* p) { return at<U16>(p, 0); }
inline U16 prop_subtype(void* p) { return at<U16>(p, 2); }
inline U16 prop_cost(void* p) { return at<U16>(p, 6); }
// duration-type bits of the flags word: xor r,r; mov r8,[e+0A]; and r,7
inline U duration_bits(void* e) { return at<U8>(e, 0xa) & 7u; }
// CExoString s("Value"); table->GetINTEntry(row, s, &out); ~s   (the out cell is whatever the caller left in it: several callers never initialise it, and GetINTEntry leaves it untouched on a bad row; kept as an indeterminate local, as the original)
inline void cost_int(void* table, U row, I* out) {
    ExoString s;
    frame_LogStringCtor_005e5a90(&s, nullptr, k_value_00744bfc);
    c2da_current_get_int_00413660(table, nullptr, row, &s, out);
    frame_LogStringDtor_005e5c20(&s, nullptr);
}
}  // namespace ipx
