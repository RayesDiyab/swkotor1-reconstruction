// Lane W4 G15 (equip-items, part 2): CSWGuiInGameEquip::OnItemSelected (006B7920, item entry event 0) and everything it
// reaches without a callable provider:
//   equip panel: SetDescription 006B56B0, GetSlotItemIdIndex 006B4C90, CopyItem 006B65E0, ItemExists 006B66A0,
//                EquipItem(id) 006B5760, EquipItem(item) 006B5890, UnequipItem 006B5910;
//   CSWSItem:    GetPropertyDescription 0055F340 and its description builders AddFeatRequirements 00556490,
//                AddDamageProperties 00556DE0, AddRangeProperties 005543B0, AddCriticalThreatProperties 00558950,
//                AddOnHitProperties 00558C10, AddWeaponSizeProperties 005544F0, AddAttackModifierProperties 0055E930,
//                AddDefenceProperties 005599D0, AddMiscellaneousProperties 0055A510, AddAttackBonusStrings 005584C0,
//                GetDamageTypeString 005545D0, GetPropertyStrings 00554E00, GetPropertyDamageRange 00555380,
//                GetPropertyChanceDurationDC 005554D0, GetExistsHigherValuedProperty 00555610, CopyItem 0055D950;
//   CTwoDimArrays: GetIPRPCostTable 005C49A0, GetIPRPParamTable 005C4C00.
// Transcribed from swkotor.exe (read-only). Statement comments cite the original instruction ranges.
//
// CExoString composition: every "desc = desc + a + b + ..." in the originals is a chain of operator+ (005E5D10) into
// fresh temporaries, then operator= (005E5C50) into desc, then the temporaries destroyed newest first; chain() below
// reproduces exactly that call sequence. Literal/GUI-string operands are built first, in the original order.
//
// CSWSItem property records (0x1C bytes): +0 WORD type, +2 WORD subtype, +6 WORD cost value, +9 BYTE param value,
// +11h BYTE upgrade slot (FFh = always active, else active iff bit set in item+294h). Passive list: count +24Ch,
// data +254h; active list: count +248h, data +250h. The originals' inline accessor returns NULL when the byte offset is
// negative or the index is out of range.
#include "78d2073e39-gui_panels.hpp"
using namespace w4gui;

namespace kotor_reconstruction {
struct StartupString { int __thiscall BorrowedEquals005e5290(const char*); };                  // CExoString::operator==(const char*)
}
using SS = kotor_reconstruction::StartupString;
SS* __fastcall GuiConcat005e5d10(SS* self, void*, SS* out, const SS* rhs);                       // CExoString::operator+
namespace base_ini_reconstruction {
SS* __fastcall left_005e5f70(SS* self, void*, SS* out, int count);                               // CExoString::Left
SS* __fastcall right_005e6170(SS* self, void*, SS* out, int count);                              // CExoString::Right
}
namespace options_video { struct String; }
namespace client_movies { int __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*); } // operator!=(const char*)
namespace w2 { unsigned __cdecl ftol2_006fae8c(double value); }                                  // __ftol2 (low dword)
extern "C" {
extern void* kotor_cell_rules_007a3a28;                                                          // [007A3A28] CSWRules*
// strings
U32 __fastcall async_string_length_005e5790(void* string, void*);                               // CExoString::GetLength
void* __fastcall kotor_fastcall_005e5bc0(void* self, void*, I32 value);                          // CExoString::CExoString(int)
I32 __fastcall find_005e5f20(void* string, void*, U32 ch, I32 start);                            // CExoString::Find(char, int)
int __cdecl kotor_sscanf_006fae08(const char* buffer, const char* format, ...);                 // sscanf
void* __fastcall kotor_fastcall_005edeb0(void* app, void*, void* out, U32 id) noexcept(false);  // CClientExoApp::GetGUIString
U32 __fastcall kotor_fastcall_005ed4c0(void* app, void*);                                        // CClientExoApp::GetClientLanguage
U32 __fastcall exo_locstring_get_005ea130(void* loc, void*, U32 language, void* out, U32 gender); // CExoLocString::GetString
U32 __fastcall recovered_localized_assign_005e9e70(void* self, void*, void* source);             // CExoLocString::operator=
// 2DA
I32 __fastcall c2da_current_get_int_00413660(void* table, void*, I32 row, void* column, I32* out);       // C2DA::GetINTEntry
I32 __fastcall c2da_current_get_string_00413270(void* table, void*, I32 row, void* column, void* out);   // C2DA::GetCExoStringEntry
void* __fastcall c2da_current_construct_00413cc0(void* self, void*, menu_resources::Ref name, I32 mode) noexcept(false); // C2DA::C2DA
I32 __fastcall c2da_current_load_004143b0(void* table, void*);                                   // C2DA::Load2DArray
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, void* string);            // CResRef::CResRef(const CExoString&)
// items / rules
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*) noexcept(false);      // CSWItem::GetBaseItem
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, I32 id) noexcept(false); // CSWBaseItemArray::GetBaseItem
U16 __fastcall kotor_fastcall_005b30c0(void* base, void*, U32 index);                            // CSWBaseItem::GetRequiredFeat (claude_equip_items.cpp)
U32 __fastcall recovered_item_property_has_00555c90(void* item, void*, U32 type, U32 subtype);   // CSWSItem::GetPropertyByTypeExists
U32 __fastcall recovered_item_property_query_005539c0(void* item, void*, void** out, U32 type, U32 subtype); // GetPropertyByType
U32 __fastcall kotor_fastcall_00555a60(void* item, void*, U8 index);                             // GetUsedActivePropertyUsesLeft
void* __fastcall recovered_item_construct_005530a0(void* self, void*, U32 id);                   // CSWSItem::CSWSItem(DWORD)
U32 __fastcall recovered_compare_item_00553cf0(void* item, void*, void* other);                  // CSWSItem::CompareItem
void* __fastcall server_get_module_004ae6b0(void* server, void*);                                // CServerExoApp::GetModule
void __fastcall recovered_module_register_tag_004c7de0(void* module, void*, void* tag, U32 id);  // CSWSModule::AddObjectToLookupTable
void* __fastcall recovered_object_array_004aed70(void* server, void*);                           // CServerExoApp::GetObjectArray
U8 __fastcall kotor_fastcall_004d8230(void* array, void*, U32 id, void** out);                   // CGameObjectArray::GetGameObject
U32 __fastcall kotor_fastcall_004aea30(void* server, void*, U32 id);                             // ClientToServerObjectId
void* __fastcall recovered_item_lookup_004ae760(void* server, void*, U32 id);                    // CServerExoApp::GetItemByGameObjectID
void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void* app, void*, U32 id);             // CClientExoApp::GetItemByGameObjectID
U32 __fastcall kotor_m15a_creature_get_equipped_item_id_0060ca80(void* creature, void*, U32 slot); // CSWCCreature::GetEquippedItemID
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);        // CSWCCreature::GetServerCreature
void* __fastcall recovered_creature_inventory_004ef770(void* creature, void*, U32 flag);          // CSWSCreature::GetItemRepository
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, U32 index);                // CItemRepository::ItemListGetItem
void __fastcall w5_object_clear_all_actions_004ccd80(void* object, void*, U32 flag);              // CSWSObject::ClearAllActions
void __fastcall recovered_combat_round_remove_all_actions_004d3770(void* round, void*);          // CSWSCombatRound::RemoveAllActions
U32 __fastcall recovered_object_equip_004f0420(void* creature, void*, void* item, U32 slot, U32, U32, U32, U32, U32); // AddEquipItemActions
U32 __fastcall recovered_object_unequip_004f06d0(void* creature, void*, void* item, U32, U32, U32, U32); // AddUnequipActions
U32 __fastcall kotor_fastcall_004fde40(void* creature, void*, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32); // AddAttackActions
void __fastcall kotor_m15a_item_power_item_00647130(void* item, void*, U32, U32, U32, U32);      // CSWCItem::PowerItem
// GUI
void __fastcall gui_load_text_params_set_text_00415e00(void* params, void*, void* string) noexcept(false); // SetText
void __fastcall gui_load_listbox_clear_items_00419a90(void* listbox, void*);                     // CSWGuiListBox::ClearItems
I32 __fastcall gui_load_text_font_height_00414ee0(void* text, void*);                            // CSWGuiText::GetFontHeight
void __fastcall gui_m48_listbox_add_controls_0041c2c0(void* listbox, void*, void** controls, I32 count, U32, U32, U32) noexcept(false); // AddControls
void __fastcall gui_load_listbox_set_active_control_0041c160(void* listbox, void*, void* control, U32); // SetActiveControl
void __fastcall kotor_fastcall_0040a140(void* manager, void*, U32 sound);                        // CSWGuiManager::PlayGuiSound
void __fastcall kotor_fastcall_006b59f0(void* self, void*, U32 show, U32 gui_string);           // ShowCantEquipMessage (panels_equip.cpp)
}

namespace {
// ---------------------------------------------------------------- CExoString helpers
struct Str {
    U32 s[2];
    bool live;
    enum Raw { raw };
    Str() : live(true) { exo_default(s); }                                                     // 005B3190
    explicit Str(const char* text) : live(true) { frame_LogStringCtor_005e5a90(s, nullptr, text); } // 005E5A90
    explicit Str(Raw) : live(false) {}
    ~Str() { if (live) frame_LogStringDtor_005e5c20(s, nullptr); }                             // 005E5C20
    Str(const Str&) = delete;
    Str& operator=(const Str&) = delete;
};
void gui(Str& out, U32 id) { kotor_fastcall_005edeb0(client_app(), nullptr, out.s, id); out.live = true; }
const char* cstr(const void* s) { return frame_LogCStr_005e5670(const_cast<void*>(s), nullptr); }
void assign(void* dst, const void* src) { async_assign_string_005e5c50(dst, nullptr, const_cast<void*>(src)); }
SS* ss(const void* p) { return static_cast<SS*>(const_cast<void*>(p)); }
// desc = desc + p[0] + p[1] + ... ; temporaries destroyed newest first.
void chain(void* desc, std::initializer_list<const void*> parts) {
    Str t[10] = {Str(Str::raw), Str(Str::raw), Str(Str::raw), Str(Str::raw), Str(Str::raw),
                 Str(Str::raw), Str(Str::raw), Str(Str::raw), Str(Str::raw), Str(Str::raw)};
    U32 n = 0;
    const void* left = desc;
    for (const void* part : parts) {
        GuiConcat005e5d10(ss(left), nullptr, ss(t[n].s), ss(part)); t[n].live = true;
        left = t[n].s; ++n;
    }
    assign(desc, left);
    for (U32 i = n; i-- > 0;) { frame_LogStringDtor_005e5c20(t[i].s, nullptr); t[i].live = false; }
}
void* rules() { return kotor_cell_rules_007a3a28; }
void* two_dim() { return lp(rules(), 0xB8); }                                                  // CSWRules +B8h CTwoDimArrays*
void* base_of(void* item) { return kotor_m15b_item_get_base_item_005b4790(item, nullptr); }
void* base_by_id(void* item) { return kotor_m15b_base_item_array_get_base_item_005b31a0(lp(rules(), 0x34), nullptr, li(item, 0x0C)); }
// Volatile byte read: the originals dereference possibly-NULL records here without a guard.
// Reads a stack word the original reads before anything has stored to it (see AddMiscellaneousProperties).
__declspec(noinline) U32 stale_word(const U32* slot) { return *reinterpret_cast<const volatile U32*>(slot); }
U8 lbv(const void* p, U32 off) { return *reinterpret_cast<const volatile U8*>(static_cast<const U8*>(p) + off); }
I32 get_int(void* table, I32 row, const char* column, I32* out) {
    Str c(column);
    return c2da_current_get_int_00413660(table, nullptr, row, c.s, out);
}
// Inline record accessors (NULL on negative offset / index out of range).
void* passive_at(void* item, I32 i, I32 off) {
    if (off < 0 || i >= li(item, 0x24C)) return nullptr;
    return static_cast<U8*>(lp(item, 0x254)) + off;
}
void* active_at(void* item, I32 i, I32 off) {
    if (off < 0 || i >= li(item, 0x248)) return nullptr;
    return static_cast<U8*>(lp(item, 0x250)) + off;
}
bool upgrade_active(void* item, const void* p) {
    const U8 slot = lbv(p, 0x11);
    return slot == 0xFF || (lw(item, 0x294) & (1u << (slot & 31u))) != 0;
}
U32 exists(void* item, U32 type) { return recovered_item_property_has_00555c90(item, nullptr, type, 0); }
U32 damage_flags(void* item) {                                                                 // base item +0Ch, default 8
    void* base = base_by_id(item);
    if (base == nullptr || lw(base, 0x0C) == 0) return 8;
    return lw(base, 0x0C);
}
}  // namespace

extern "C" {
// ---------------------------------------------------------------- CTwoDimArrays
// 005C49A0 CTwoDimArrays::GetIPRPCostTable(BYTE) RET 4.
void* __fastcall kotor_fastcall_005c49a0(void* self, void*, U32 index) {
    const U8 i = static_cast<U8>(index);                                                       // 005C49A0
    if (!(i < lb(self, 8))) return nullptr;                                                    // 005C49A5..005C49AD (jae)
    if (lp(self, 0) == nullptr) return nullptr;                                                // 005C49AF..005C49B3
    return lp(lp(self, 0), static_cast<U32>(i) * 4u);                                          // 005C49B5..005C49B8
}
// 005C4C00 CTwoDimArrays::GetIPRPParamTable(BYTE) RET 4.
void* __fastcall kotor_fastcall_005c4c00(void* self, void*, U32 index) {
    const U8 i = static_cast<U8>(index);
    if (!(i < lb(self, 9))) return nullptr;                                                    // 005C4C05..005C4C0D
    if (lp(self, 4) == nullptr) return nullptr;                                                // 005C4C0F..005C4C14
    return lp(lp(self, 4), static_cast<U32>(i) * 4u);                                          // 005C4C16..005C4C19
}

// ---------------------------------------------------------------- CSWSItem leaves
// 00555610 CSWSItem::GetExistsHigherValuedProperty(WORD type, WORD subtype, WORD value) RET 0Ch.
U32 __fastcall kotor_fastcall_00555610(void* self, void*, U32 type, U32 subtype, U32 value) {
    const I32 count = li(self, 0x24C);                                                         // 00555616
    if (count <= 0) return 0;                                                                  // 0055561E..00555620
    I32 off = 0;
    for (I32 i = 0; i < count; ++i, off += 0x1C) {                                             // 00555624..00555675
        void* p = passive_at(self, i, off);
        if (p == nullptr) continue;                                                            // 00555634..00555636
        if (!upgrade_active(self, p)) continue;                                                // 00555638..0055564D
        if (lh(p, 0) != static_cast<U16>(type)) continue;                                      // 0055564F..00555657
        if (lh(p, 2) != static_cast<U16>(subtype)) continue;                                   // 00555659..00555662
        if (lh(p, 6) > static_cast<U16>(value)) return 1;                                      // 00555664..0055566D (ja)
    }
    return 0;
}

// 00555380 CSWSItem::GetPropertyDamageRange(WORD row, int* count, int* total) RET 0Ch (iprp_damagecost: NumDice/Die).
void __fastcall kotor_fastcall_00555380(void*, void*, U32 row_arg, I32* first, I32* second) {
    I32 dice = 0, die = 0;                                                                     // [esp+8] / [esp+0Ch]
    const I32 row = static_cast<U16>(row_arg);                                                 // 005553C3
    void* table = lp(two_dim(), 0xB0);                                                         // 005553B8..005553C8
    const bool found = get_int(table, row, "NumDice", &dice) == 1;                             // 0055539A..005553FD
    if (!found) { *first = row; *second = row; return; }                                       // 005554AA..005554B8
    if (dice == 0) { *first = 1; *second = row; return; }                                      // 0055540A..00555420
    get_int(lp(two_dim(), 0xB0), row, "Die", &die);                                            // 00555435..0055547B
    *first = dice;                                                                             // 0055548F
    *second = die * dice;                                                                      // 0055548C..00555496
}

// 005554D0 CSWSItem::GetPropertyChanceDurationDC(WORD cost, BYTE row, int* chance, int* rounds, int* dc) RET 14h.
// The shared output local is not initialized (a failed lookup forwards whatever it holds, original behavior).
void __fastcall kotor_fastcall_005554d0(void*, void*, U32 cost, U32 row_arg, I32* chance, I32* rounds, I32* dc) {
    I32 value;                                                                                 // [esp+4]
    const I32 row = static_cast<U8>(row_arg);                                                  // 00555502
    get_int(lp(two_dim(), 0xCC), row, "EffectChance", &value);                                 // 005554E9..00555533
    *chance = value;                                                                           // 00555538..00555540
    get_int(lp(two_dim(), 0xCC), row, "DurationRounds", &value);                               // 00555542..00555588
    *rounds = value;                                                                           // 0055558D..00555595
    {
        Str c("Value");                                                                        // 00555597..005555A0
        void* table = kotor_fastcall_005c49a0(two_dim(), nullptr, 0x19);                       // 005555A5..005555CB
        c2da_current_get_int_00413660(table, nullptr, static_cast<U16>(cost), c.s, &value);    // 005555D0..005555D2
    }
    *dc = value;                                                                               // 005555E8..005555F0
}

// 00554E00 CSWSItem::GetPropertyStrings(WORD type, WORD subtype, WORD cost, BYTE param, CExoString* name,
// CExoString* sub, CExoString* cost_name, CExoString* param_name) RET 20h.
void __fastcall kotor_fastcall_00554e00(void* self, void*, U32 type_arg, U32 subtype_arg, U32 cost_arg, U32 param_arg,
                                        void* name, void* sub, void* cost_name, void* param_name) {
    (void)self;
    const I32 type = static_cast<U16>(type_arg);                                               // 00554E41 (esi)
    I32 strref = -1;                                                                           // [esp+10h]
    I32 table_index;                                                                           // [esp+14h]
    void* defs = lp(two_dim(), 0x34);                                                          // itempropdef
    if (get_int(defs, type, "Name", &strref) == 0) return;                                     // 00554E24..00554E79 / 00555364
    { Str g(Str::raw); gui(g, static_cast<U32>(strref)); assign(name, g.s); }                  // 00554E7F..00554EAF
    Str sub_ref;                                                                               // [esp+28h]
    Str param_ref;                                                                             // [esp+30h]
    bool found;
    { Str c("SubTypeResRef"); found = c2da_current_get_string_00413270(lp(two_dim(), 0x34), nullptr, type, c.s, sub_ref.s) == 1; } // 00554ECE..00554F19
    if (found) {
        void* raw = live_operator_new(0x54);                                                   // 00554F26..00554F2D
        void* table = nullptr;
        if (raw != nullptr) {
            auto g = on_throw([&]() { live_free(raw); });
            menu_resources::Ref ref; async_ref_construct_string_00406d60(&ref, nullptr, sub_ref.s); // 00554F3F..00554F4F
            table = c2da_current_construct_00413cc0(raw, nullptr, ref, 0);                     // 00554F54..00554F56
            g.armed = false;
        }
        if (c2da_current_load_004143b0(table, nullptr) == 0) return;                           // 00554F61..00554F83 (table leaked)
        get_int(table, static_cast<U16>(subtype_arg), "Name", &strref);                        // 00554F88..00554FBA
        { Str g(Str::raw); gui(g, static_cast<U32>(strref)); assign(sub, g.s); }               // 00554FBF..00554FED
        if (table != nullptr) vcall<void*, U32>(table, 0x00, 1);                               // 00554FF2..00554FFC
    }
    const U16 cost = static_cast<U16>(cost_arg);                                               // 00554FFE
    if (cost != 0xFFFF) {                                                                      // 00555002..00555007
        if (get_int(lp(two_dim(), 0x34), type, "CostTableResRef", &table_index) == 1) {        // 0055500D..00555059
            void* table = kotor_fastcall_005c49a0(two_dim(), nullptr, static_cast<U32>(table_index)); // 0055505F..0055506F
            { Str c("Name"); c2da_current_get_int_00413660(table, nullptr, cost, c.s, &strref); }  // 00555074..005550A7
            Str g(Str::raw); gui(g, static_cast<U32>(strref)); assign(cost_name, g.s);         // 005550AC..005550DA
        }
    }
    const U8 param = static_cast<U8>(param_arg);
    if (param != 0xFF) {                                                                       // 005550DF..005550E3
        if (get_int(lp(two_dim(), 0x34), type, "Param1ResRef", &table_index) == 1) {           // 005550E9..0055513A
            { Str c("Name");
              void* table = kotor_fastcall_005c4c00(two_dim(), nullptr, static_cast<U32>(table_index)); // 0055514A..0055516F
              c2da_current_get_int_00413660(table, nullptr, param, c.s, &strref); }            // 00555174..00555184
            Str g(Str::raw); gui(g, static_cast<U32>(strref)); assign(param_name, g.s);        // 00555189..005551B7
        } else {
            bool sub_found;
            { Str c("SubTypeResRef"); sub_found = c2da_current_get_string_00413270(lp(two_dim(), 0x34), nullptr, type, c.s, sub_ref.s) == 1; } // 005551C1..00555203
            if (sub_found) {
                void* raw = live_operator_new(0x54);                                           // 00555210..00555217
                void* table = nullptr;
                if (raw != nullptr) {
                    auto g = on_throw([&]() { live_free(raw); });
                    menu_resources::Ref ref; async_ref_construct_string_00406d60(&ref, nullptr, sub_ref.s); // 00555229..00555239
                    table = c2da_current_construct_00413cc0(raw, nullptr, ref, 0);             // 0055523E..00555240
                    g.armed = false;
                }
                if (c2da_current_load_004143b0(table, nullptr) == 0) {                         // 0055524B..00555259
                    if (table != nullptr) vcall<void*, U32>(table, 0x00, 1);                   // 0055525B..00555269
                } else {
                    if (get_int(table, static_cast<U16>(subtype_arg), "Param1ResRef", &table_index) == 1) { // 00555270..005552B2
                        { Str c("Name");
                          void* ptable = kotor_fastcall_005c4c00(two_dim(), nullptr, static_cast<U32>(table_index)); // 005552C6..005552EB
                          c2da_current_get_int_00413660(ptable, nullptr, param, c.s, &strref); }  // 005552F0..00555300
                        Str g(Str::raw); gui(g, static_cast<U32>(strref)); assign(param_name, g.s); // 00555305..00555334
                    }
                    if (table != nullptr) vcall<void*, U32>(table, 0x00, 1);                   // 00555339..00555343
                }
            }
        }
    }
}                                                                                              // 00555345..0055535F (~param_ref, ~sub_ref)

// 005545D0 CSWSItem::GetDamageTypeString(DWORD flags, CExoString* out) RET 8.
void __fastcall kotor_fastcall_005545d0(void*, void*, U32 flags, void* out) {
    I32 n = 0;                                                                                 // edi
    if ((flags & 7u) != 0) {                                                                   // 005545FC..005545FE
        Str g(Str::raw); gui(g, 0x9698); assign(out, g.s); n = 1;                              // 00554600..00554630
    }
    static const U32 bits[11] = {0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000};
    static const U32 refs[11] = {0x9699, 0x969A, 0x969B, 0x969C, 0x969D, 0x969E, 0x969F, 0x96A0, 0x96A1, 0x96A2, 0xA3AF};
    for (U32 k = 0; k < 11; ++k) {                                                             // 00554635..00554DE4
        if ((flags & bits[k]) == 0) continue;
        if (n > 0) { Str sep(", "); chain(out, {sep.s}); }
        Str g(Str::raw); gui(g, refs[k]); chain(out, {g.s});
        ++n;
    }
}

// 005544F0 CSWSItem::AddWeaponSizeProperties(CExoString* desc) RET 4.
void __fastcall kotor_fastcall_005544f0(void* self, void*, void* desc) {
    if (lb(base_of(self), 0x1B) != 2) return;                                                  // 00554508..00554512
    Str nl("\n\n");                                                                            // 00554519..00554522
    Str g(Str::raw); gui(g, 0xA4A1);                                                           // 00554527..00554541
    chain(desc, {g.s, nl.s});                                                                  // 00554546..0055459F
}

// 005543B0 CSWSItem::AddRangeProperties(CExoString* desc) RET 4.
void __fastcall kotor_fastcall_005543b0(void* self, void*, void* desc) {
    Str text;                                                                                  // [esp+4]
    if (lb(base_of(self), 0x1A) == 0) return;                                                  // 005543D4..005543F0
    const U32 range = w2::ftol2_006fae8c(static_cast<double>(lf(base_of(self), 0x34)));       // 005543F6..00554400
    {
        Str g(Str::raw); gui(g, 0x96A5);                                                       // 00554405..0055441A
        frame_LogFormat_005e5680(text.s, "%s: %dm\n", cstr(g.s), range);                       // 0055441F..0055443C
    }
    Str nl("\n");                                                                              // 0055444D..00554456
    chain(desc, {text.s, nl.s});                                                               // 0055445B..005544BD
}

// 005584C0 CSWSItem::AddAttackBonusStrings(WORD type, CExoString* desc) RET 8. The running total is not reset between
// property groups (original).
void __fastcall kotor_fastcall_005584c0(void* self, void*, U32 type, void* desc) {
    U32 total = 0;                                                                             // [esp+1Ch]
    Str name, sub, cost, param, unused;                                                        // [esp+60h] [28h] [50h] [30h] [B0h]
    if (exists(self, type) == 0) return;                                                       // 0055853F..00558555
    I32 count = li(self, 0x24C);                                                               // 0055855B
    if (count <= 0) return;
    I32 off = 0;
    for (I32 i = 0; i < count; ++i, off += 0x1C, count = li(self, 0x24C)) {                    // 00558580..005588D2
        void* p = passive_at(self, i, off);
        if (p == nullptr || !upgrade_active(self, p)) continue;                                // 00558580..005585B9
        if (static_cast<U32>(lh(p, 0)) != type) continue;                                      // 005585BF..005585D0
        bool earlier = false;
        U8* q = static_cast<U8*>(lp(self, 0x254));
        for (I32 j = 0; j < count; ++j, q += 0x1C) {                                           // 005585D6..00558639
            if (j < 0 || q == nullptr || !upgrade_active(self, q)) continue;
            if (static_cast<U32>(lh(q, 0)) != type || lh(p, 2) != lh(q, 2)) continue;          // 0055860B..0055861B
            if (j < i) { earlier = true; break; }                                              // 0055861D..00558621
            total += lh(q, 6);                                                                 // 00558627..0055862B
        }
        if (earlier) continue;
        kotor_fastcall_00554e00(self, nullptr, lh(p, 0), lh(p, 2), lh(p, 6), 0xFF, name.s, sub.s, cost.s, param.s); // 0055863B..00558667
        Str nl("\n"), sp1(" "), sp2(" "), plus("+");                                           // 0055866C..005586B7 [38h] [48h] [58h] [40h]
        Str g(Str::raw); gui(g, 0x573);                                                        // 005586BC..005586DA [A0h]
        Str number(Str::raw); kotor_fastcall_005e5bc0(number.s, nullptr, static_cast<I32>(total)); number.live = true; // 005586E1..005586F5
        chain(desc, {plus.s, number.s, sp2.s, g.s, sp1.s, sub.s, nl.s});                       // 005586FA..00558898
    }
}

// 00558950 CSWSItem::AddCriticalThreatProperties(CExoString* desc) RET 4.
void __fastcall kotor_fastcall_00558950(void* self, void*, void* desc) {
    Str text;                                                                                  // [esp+38h]
    {
        Str colon(":\n");                                                                      // 00558977..00558988
        Str g(Str::raw); gui(g, 0x96AB);                                                       // 0055898D..005589A4
        chain(desc, {g.s, colon.s});                                                           // 005589A9..00558A15
    }
    I32 threat = lb(base_of(self), 0x1E);                                                      // 00558A1A..00558A27
    const I32 multiplier = lb(base_of(self), 0x1F);                                            // 00558A2B..00558A3B
    if (exists(self, 0x1C) != 0) threat = threat + threat;                                     // 00558A36..00558A4B
    I32 low = 0, high = 0;                                                                     // [esp+10h] / [esp+50h]
    bool bonus = false;
    if (exists(self, 0x31) != 0) {                                                             // 00558A4F..00558A63
        I32 count = li(self, 0x24C);
        I32 off = 0;
        for (I32 i = 0; i < count; ++i, off += 0x1C, count = li(self, 0x24C)) {                // 00558A69..00558B10
            void* p = passive_at(self, i, off);
            if (p == nullptr || !upgrade_active(self, p)) continue;
            if (lh(p, 0) != 0x31) continue;                                                    // 00558AAB..00558AB4
            const U32 value = lh(p, 6);
            if (kotor_fastcall_00555610(self, nullptr, 0x31, lh(p, 2), value) != 0) continue;  // 00558AB6..00558ACC
            I32 a, b;
            kotor_fastcall_00555380(self, nullptr, value, &a, &b);                             // 00558ACE..00558ADB
            low += a; high += b;                                                               // 00558AE0..00558AF8
        }
        bonus = low != 0 || high != 0;                                                         // 00558B16..00558B24
    }
    if (!bonus)
        frame_LogFormat_005e5680(text.s, "%d-20,x%d\n", 0x15 - threat, multiplier);            // 00558B26..00558B46
    else
        frame_LogFormat_005e5680(text.s, "%d-20,x%d +%d-%d\n", 0x15 - threat, multiplier, low, high); // 00558B4B..00558B71
    Str nl("\n");                                                                              // 00558B74..00558B7D
    chain(desc, {text.s, nl.s});                                                               // 00558B82..00558BD2
}

// 0055E930 CSWSItem::AddAttackModifierProperties(CExoString* desc) RET 4.
void __fastcall kotor_fastcall_0055e930(void* self, void*, void* desc) {
    Str text;                                                                                  // [esp+10h]
    I32 modifier = 0;                                                                          // ebp
    if (exists(self, 0x26) != 0 || exists(self, 8) != 0 || exists(self, 0x29) != 0 || exists(self, 5) != 0) { // 0055E959..0055E997
        I32 count = li(self, 0x24C);
        I32 off = 0;
        for (I32 i = 0; i < count; ++i, off += 0x1C, count = li(self, 0x24C)) {                // 0055E99D..0055EA19
            void* p = passive_at(self, i, off);
            if (p == nullptr || !upgrade_active(self, p)) continue;
            const U16 t = lh(p, 0);
            if (t == 5 || t == 0x26) modifier += lh(p, 6);                                     // 0055E9DE..0055E9F1
            if (t == 8 || t == 0x29) modifier -= lh(p, 6);                                     // 0055E9F3..0055EA03
        }
        if (modifier != 0) {                                                                   // 0055EA1B..0055EA7D
            {
                Str g(Str::raw); gui(g, 0x96A6);                                               // 0055EA1F..0055EA32 / 0055EA7F..0055EA91
                frame_LogFormat_005e5680(text.s, modifier < 0 ? "%s: -%d\n" : "%s: +%d\n", cstr(g.s), modifier); // 0055EA37..0055EA54 / 0055EA96..0055EAB3
            }
            chain(desc, {text.s});                                                             // 0055EA65..0055EAEB
        }
    }
    if (exists(self, 6) != 0 || exists(self, 7) != 0 || exists(self, 0x27) != 0 || exists(self, 0x28) != 0) { // 0055EAF0..0055EB2B
        if (modifier == 0) {                                                                   // 0055EB31..0055EB33
            Str colon(": ");                                                                   // 0055EB39..0055EB42
            Str g(Str::raw); gui(g, 0x96A6);                                                   // 0055EB47..0055EB5E
            chain(desc, {g.s, colon.s});                                                       // 0055EB63..0055EBC9
        }
        kotor_fastcall_005584c0(self, nullptr, 6, desc);                                       // 0055EBCE..0055EBD3
        kotor_fastcall_005584c0(self, nullptr, 7, desc);                                       // 0055EBD8..0055EBDC
        kotor_fastcall_005584c0(self, nullptr, 0x27, desc);                                    // 0055EBE1..0055EBE6
        kotor_fastcall_005584c0(self, nullptr, 0x28, desc);                                    // 0055EBEB..0055EBF0
    }
    Str nl("\n");                                                                              // 0055EBF5..0055EBFE
    chain(desc, {nl.s});                                                                       // 0055EC03..0055EC3D
}

// 00556490 CSWSItem::AddFeatRequirements(CExoString* desc) RET 4. The 0x39 pass dereferences an out-of-range record
// unguarded (original); the feat-name strref local is not initialized before the first lookup (original).
void __fastcall kotor_fastcall_00556490(void* self, void*, void* desc) {
    Str feat_name, left, right;                                                                // [esp+14h] [5Ch] [54h]
    bool header = false;                                                                       // [esp+20h]
    I32 strref;                                                                                // [esp+2Ch]
    I32 pick = 5;                                                                              // [esp+10h]
    for (I32 i = 0; lb(base_of(self), 0x80) != 0 && i < static_cast<I32>(lb(base_of(self), 0x80)); ++i) { // 005564FD..00556570
        if (kotor_fastcall_005b30c0(base_of(self), nullptr, static_cast<U32>(i)) == 6 && pick == 5) pick = 6;   // 00556510..0055652C
        if (kotor_fastcall_005b30c0(base_of(self), nullptr, static_cast<U32>(i)) == 4 && (pick == 5 || pick == 6)) pick = 4; // 00556534..00556557
    }
    auto add_feat = [&](I32 feat) {
        if (!header) {                                                                         // 005565CD..005565D3 / 005569BB..005569C1
            Str colon(":\n");
            Str g(Str::raw); gui(g, 0x9688);
            chain(desc, {g.s, colon.s});
            header = true;
        }
        get_int(lp(two_dim(), 0xF8), feat, "NAME", &strref);                                   // 0055669C..005566E5 / 00556A89..00556AD1
        { Str g(Str::raw); gui(g, static_cast<U32>(strref)); assign(feat_name.s, g.s); }       // 005566EA..00556725
        const I32 pos = find_005e5f20(feat_name.s, nullptr, 0x3A, 0);                          // 0055672A..00556732
        if (pos != -1) {                                                                       // 00556737..0055673C
            { Str t(Str::raw); base_ini_reconstruction::left_005e5f70(ss(feat_name.s), nullptr, ss(t.s), pos); t.live = true; assign(left.s, t.s); }  // 00556742..00556774
            { const I32 n = static_cast<I32>(async_string_length_005e5790(feat_name.s, nullptr)) - pos - 1;
              Str t(Str::raw); base_ini_reconstruction::right_005e6170(ss(feat_name.s), nullptr, ss(t.s), n); t.live = true; assign(right.s, t.s); } // 00556779..005567B7
            Str dash(" - ");                                                                   // 005567BC..005567C5
            Str c1(Str::raw); WindowCopy005e5b00(c1.s, nullptr, right.s); c1.live = true;      // 005567CA..005567DE
            Str c2(Str::raw); WindowCopy005e5b00(c2.s, nullptr, left.s); c2.live = true;       // 005567E5..005567F9
            Str t1(Str::raw); GuiConcat005e5d10(ss(c2.s), nullptr, ss(t1.s), ss(dash.s)); t1.live = true; // 005567FE..00556812
            Str t2(Str::raw); GuiConcat005e5d10(ss(t1.s), nullptr, ss(t2.s), ss(c1.s)); t2.live = true;   // 00556817..00556827
            assign(feat_name.s, t2.s);                                                         // 0055682C..00556839
        }
        Str nl("\n");                                                                          // 00556898..005568A1
        chain(desc, {feat_name.s, nl.s});                                                      // 005568A6..0055691B
    };
    const I32 desc_count = lb(base_of(self), 0x80);                                            // 0055657C..0055658C
    if (desc_count != 0) {
        for (I32 j = 0; j < static_cast<I32>(lb(base_of(self), 0x80)); ++j) {                  // 00556592..0055693F
            const I32 feat = kotor_fastcall_005b30c0(base_of(self), nullptr, static_cast<U32>(j)); // 00556592..005565A5
            if (feat <= 0) continue;                                                           // 005565A8..005565AE
            if ((feat == 5 || feat == 6 || feat == 4) && feat != pick) continue;               // 005565B4..005565C7
            add_feat(feat);
        }
    }
    if (exists(self, 0x39) != 0) {                                                             // 00556945..00556952
        I32 count = li(self, 0x24C);
        I32 off = 0;
        for (I32 k = 0; k < count; ++k, off += 0x1C, count = li(self, 0x24C)) {                // 00556958..00556D2C
            void* p = passive_at(self, k, off);                                                // 00556972..00556986 (no NULL check)
            const U8 slot = lbv(p, 0x11);                                                      // 00556986
            if (slot != 0xFF && (lw(self, 0x294) & (1u << (slot & 31u))) == 0) continue;       // 00556989..0055699B
            if (lh(p, 0) != 0x39) continue;                                                    // 005569A1..005569A5
            const I32 feat = lh(p, 2);                                                         // 005569AB..005569B5
            if (feat <= 0) continue;
            add_feat(feat);
        }
    }
    if (header) { Str nl("\n"); chain(desc, {nl.s}); }                                         // 00556D32..00556D8D
}

// 00556DE0 CSWSItem::AddDamageProperties(CExoString* desc) RET 4.
void __fastcall kotor_fastcall_00556de0(void* self, void*, void* desc) {
    Str text;                                                                                  // [esp+C4h]
    {
        Str colon(": ");                                                                       // 00556E12..00556E26
        Str g(Str::raw); gui(g, 0x7A99);                                                       // 00556E2B..00556E48
        chain(desc, {g.s, colon.s});                                                           // 00556E4D..00556EC1
    }
    if (exists(self, 0x1F) != 0) {                                                             // 00556ED2..00556EDF
        Str zero("0\n"); chain(desc, {zero.s});                                                // 00556EE1..00556F2D / 0055721A
    } else {
        I32 low = lb(base_by_id(self), 0x1C);                                                  // 00556F32..00556F44 [esp+28h]
        I32 high = static_cast<I32>(lb(base_by_id(self), 0x1D)) * low;                         // 00556F48..00556F68 [esp+18h]
        Str type_name;                                                                         // [esp+38h]
        kotor_fastcall_005545d0(self, nullptr, damage_flags(self), type_name.s);               // 00556F71..00556FA3
        if (client_movies::StringNotEqual_005e5390(reinterpret_cast<options_video::String*>(type_name.s), nullptr, "") != 0) { // 00556FA8..00556FB8
            Str sep(", "); chain(desc, {type_name.s, sep.s});                                  // 00556FBE..00557030
        }
        if (exists(self, 0xF) != 0 || exists(self, 5) != 0 || exists(self, 0xB) != 0) {       // 00557041..0055706A
            I32 count = li(self, 0x24C);
            I32 off = 0;
            for (I32 i = 0; i < count; ++i, off += 0x1C, count = li(self, 0x24C)) {            // 00557070..00557190
                void* p = passive_at(self, i, off);
                if (p == nullptr || !upgrade_active(self, p)) continue;
                if (lh(p, 0) == 5) { low += lh(p, 6); high += lh(p, 6); }                      // 005570D5..005570EF
                if (lh(p, 0) == 0xF) { low -= lh(p, 6); high -= lh(p, 6); }                    // 005570F3..0055710D
                if (lh(p, 0) == 0xB) {                                                         // 00557111..00557115
                    if (((damage_flags(self) >> (lh(p, 2) & 31u)) & 1u) == 0) continue;        // 00557117..00557140
                    I32 a, b;
                    kotor_fastcall_00555380(self, nullptr, lh(p, 6), &a, &b);                  // 00557142..00557153
                    low += a; high += b;                                                       // 00557158..00557170
                }
            }
        }
        frame_LogFormat_005e5680(text.s, "%d-%d\n\n", low, high);                              // 00557196..005571B2
        Str copy(cstr(text.s));                                                                // 005571B5..005571C6
        chain(desc, {copy.s});                                                                 // 005571CB..00557205
    }                                                                                          // 00557216 (~type_name)
    Str scratch, name, sub, cost, param, unused;                                               // [A4h] [D4h] [BCh] [84h] [B4h] [120h]
    I32 last = -1;                                                                             // [esp+10h]
    // Damage bonus groups 0x0C / 0x0D: totals over every property of the type, then one line from the last match.
    auto bonus_group = [&](U32 type) {
        I32 low = 0, high = 0;
        bool any = false;                                                                      // set for every active record (original)
        if (exists(self, type) == 0) return;
        I32 count = li(self, 0x24C);
        I32 off = 0;
        for (I32 i = 0; i < count; ++i, off += 0x1C, count = li(self, 0x24C)) {
            void* p = passive_at(self, i, off);
            if (p == nullptr || !upgrade_active(self, p)) continue;
            if (lh(p, 0) == type) {
                last = i;
                I32 a, b;
                kotor_fastcall_00555380(self, nullptr, lh(p, 6), &a, &b);
                low += a; high += b;
            }
            any = true;
        }
        if (!any) return;
        void* p = (last >= 0 && last < li(self, 0x24C)) ? static_cast<U8*>(lp(self, 0x254)) + last * 0x1C : nullptr;
        const U8 param_value = lbv(p, 9);                                                      // (NULL when no match: original)
        kotor_fastcall_00554e00(self, nullptr, lh(p, 0), lh(p, 2), 0xFFFF, param_value, name.s, sub.s, cost.s, param.s);
        if (low == high) frame_LogFormat_005e5680(cost.s, "+%d", low);
        else frame_LogFormat_005e5680(cost.s, "+%d-%d", low, high);
        Str nl("\n\n"), sp1(" "), sp2(" "), colon(": ");
        Str g(Str::raw); gui(g, 0x573);
        chain(desc, {param.s, colon.s, cost.s, sp2.s, g.s, sp1.s, sub.s, nl.s});
    };
    bonus_group(0x0C);                                                                         // 005572A9..00557641
    bonus_group(0x0D);                                                                         // 00557652..005579E8 ([esp+10h] not reset)
    // 0x10: one line per record ("name: +N (cost)").
    char word[0x2C];                                                                           // [esp+130h] (sscanf %s target, unbounded: original)
    I32 parsed;                                                                                // [esp+DCh] (not initialized: original)
    if (exists(self, 0x10) != 0) {                                                             // 005579F9..00557A06
        I32 count = li(self, 0x24C);
        I32 off = 0;
        for (I32 i = 0; i < count; ++i, off += 0x1C, count = li(self, 0x24C)) {                // 00557A0C..00557D9B
            void* p = passive_at(self, i, off);
            if (p == nullptr || !upgrade_active(self, p)) continue;
            Str a, b, c, d, e;                                                                 // [60h] [68h] [48h] [94h] [18h]
            if (lh(p, 0) != 0x10) continue;                                                    // 00557ABE..00557AD1
            if (kotor_fastcall_00555610(self, nullptr, 0x10, lh(p, 2), lh(p, 6)) != 0) continue; // 00557AD7..00557AF7
            kotor_fastcall_00554e00(self, nullptr, 0x10, lh(p, 2), lh(p, 6), 0xFF, a.s, b.s, c.s, d.s); // 00557AFD..00557B26
            kotor_sscanf_006fae08(cstr(b.s), "%s +%d", word, &parsed);                         // 00557B2B..00557B4A
            frame_LogFormat_005e5680(scratch.s, "%d", parsed);                                 // 00557B4F..00557B69
            { Str plus(": +"); chain(desc, {a.s, plus.s, scratch.s}); }                        // 00557B6C..00557C0E
            frame_LogFormat_005e5680(scratch.s, "%s", lp(c.s, 0), lw(c.s, 4));                 // 00557C1F..00557C3B (CExoString by value)
            Str close(")\n\n"), open(" (");                                                    // 00557C3E..00557C60
            chain(desc, {open.s, c.s, close.s});                                               // 00557C65..00557CC4
        }
    }
    // 0x0B: elemental damage bonuses not native to the weapon, physical types (0..2) grouped; "type: +lo-hi kind[, ...]".
    bool header = false;                                                                       // [esp+50h]
    I32 low = 0, high = 0;                                                                     // [esp+28h] / [esp+18h]
    if (exists(self, 0xB) != 0) {                                                              // 00557DA1..00557DBB
        I32 count = li(self, 0x24C);
        I32 off = 0;
        for (I32 i = 0; i < count; ++i, off += 0x1C, count = li(self, 0x24C)) {                // 00557DC1..005583A1
            void* p = passive_at(self, i, off);
            if (p == nullptr || !upgrade_active(self, p)) continue;
            Str a, b, c, d, e;                                                                 // [38h] [48h] [68h] [60h] [20h]
            if (lh(p, 0) != 0xB) continue;                                                     // 00557E76..00557E82
            if (((damage_flags(self) >> (lh(p, 2) & 31u)) & 1u) != 0) continue;                // 00557E88..00557EB3
            kotor_fastcall_00554e00(self, nullptr, 0xB, lh(p, 2), lh(p, 6), 0xFF, a.s, b.s, c.s, d.s); // 00557EB9..00557EDE
            if (!header) { Str colon(": "); chain(desc, {a.s, colon.s}); }                     // 00557EE3..00557F73
            bool earlier = false;
            I32 qcount = li(self, 0x24C);
            if (qcount > 0) {
                I32 qoff = 0;
                for (I32 j = 0; j < qcount; ++j, qoff += 0x1C, qcount = li(self, 0x24C)) {     // 00557FA8..0055809C
                    void* q = passive_at(self, j, qoff);                                       // (no NULL check: original)
                    const U32 pt = lh(p, 2), qt = *reinterpret_cast<const volatile U16*>(static_cast<U8*>(q) + 2);
                    if (!upgrade_active(self, q)) continue;                                    // 00557FDB..00557FF7
                    if (lh(q, 0) != 0xB) continue;                                             // 00558004..00558008
                    if (pt != qt && !((pt <= 2) && (qt <= 2))) continue;                       // 0055800A..00558031
                    if (j < i) { earlier = true; break; }                                      // 00558033..0055803E
                    I32 x, y;
                    kotor_fastcall_00555380(self, nullptr, lh(q, 6), &x, &y);                  // 00558044..00558055
                    low += x;                                                                  // 0055805A..0055806A
                    high += (x == y) ? x : y;                                                  // 00558068..00558076
                }
            }
            if (earlier) continue;
            if (low == high) frame_LogFormat_005e5680(scratch.s, "+%d", low);                  // 005580A2..005580C4
            else frame_LogFormat_005e5680(scratch.s, "+%d-%d", low, high);                     // 005580C6..005580DA
            if (lh(p, 2) <= 2) { Str g(Str::raw); gui(g, 0x9698); assign(b.s, g.s); }          // 005580DD..00558124
            if (!header) {                                                                     // 00558138..00558141
                Str sp(" "); chain(desc, {scratch.s, sp.s, b.s});                              // 00558147..005581F6
                header = true;                                                                 // 00558207
            } else {
                Str sp(" "), sep(", "); chain(desc, {sep.s, scratch.s, sp.s, b.s});            // 00558214..00558317
            }
            low = 0; high = 0;                                                                 // 00558328..0055832C
        }
        if (header) { Str nl("\n\n"); chain(desc, {nl.s}); }                                   // 005583A7..005583FD
    }
}

// 00558C10 CSWSItem::AddOnHitProperties(CExoString* desc) RET 4.
void __fastcall kotor_fastcall_00558c10(void* self, void*, void* desc) {
    if (exists(self, 0x20) == 0) return;                                                       // 00558C34..00558C45
    Str name, sub, cost, param, unused, text;                                                  // [30h] [10h] [28h] [1Ch] [218h] [60h]
    I32 count = li(self, 0x24C);
    I32 off = 0;
    for (I32 i = 0; i < count; ++i, off += 0x1C, count = li(self, 0x24C)) {                    // 00558CAF..0055991F
        void* p = passive_at(self, i, off);
        if (p == nullptr || !upgrade_active(self, p)) continue;
        if (lh(p, 0) != 0x20) continue;                                                        // 00558D14..00558D1B
        kotor_fastcall_00554e00(self, nullptr, 0x20, lh(p, 2), 0xFFFF, 0xFF, name.s, sub.s, cost.s, param.s); // 00558D21..00558D49
        const U32 kind = lh(p, 2);                                                             // 00558D4E
        if (kind > 0xA) continue;                                                              // 00558D52..00558D55
        static const U8 group[11] = {0, 0, 0, 0, 0, 0, 1, 2, 1, 1, 3};                         // 005599B8 -> 005599A8
        switch (group[kind]) {
        case 0: {                                                                              // 00558D69: chance/duration/DC effects
            { Str sp(" "), colon(": "); Str g(Str::raw); gui(g, 0x96B7); chain(desc, {g.s, colon.s, sub.s, sp.s}); } // 00558D69..00558EB5
            I32 chance, rounds, dc;                                                            // [54h] [38h] [24h]
            kotor_fastcall_005554d0(self, nullptr, lh(p, 6), lb(p, 9), &chance, &rounds, &dc); // 00558EBA..00558EDE
            const I32 rounds3 = rounds * 3;                                                    // 00558EE3..00558F00
            {
                Str g1(Str::raw); gui(g1, 0xBFEF);                                             // 00558EE7..00558F09
                Str g2(Str::raw); gui(g2, 0xBB06);                                             // 00558F0D..00558F30
                Str g3(Str::raw); gui(g3, 0x96AE);                                             // 00558F34..00558F56
                const char* s1 = cstr(g1.s); const char* s2 = cstr(g2.s); const char* s3 = cstr(g3.s); // 00558F5A..00558F80
                frame_LogFormat_005e5680(text.s, "%d%% %s, %d %s, %s %d\n", chance, s3, rounds3, s2, s1, dc); // 00558F85..00558F9A
            }
            Str nl("\n"); chain(desc, {text.s, nl.s});                                         // 00558FD8..0055905A
            break;
        }
        case 1: {                                                                              // 0055905F
            { Str sp(" "), colon(": "); Str g(Str::raw); gui(g, 0x96B7); chain(desc, {g.s, colon.s, sub.s, sp.s}); } // 0055905F..005591A6
            kotor_fastcall_00554e00(self, nullptr, lh(p, 0), lh(p, 2), lh(p, 6), lb(p, 9), name.s, sub.s, cost.s, param.s); // 005591B9..005591EB
            Str nl("\n\n"), sp(" ");                                                           // 005591F0..00559215
            chain(desc, {param.s, sp.s, cost.s, nl.s});                                        // 0055921A..00559304
            break;
        }
        case 2: {                                                                              // 00559310
            { Str colon(": "); Str g(Str::raw); gui(g, 0x96B7); chain(desc, {g.s, colon.s}); } // 00559310..005593C7
            kotor_fastcall_00554e00(self, nullptr, lh(p, 0), lh(p, 2), lh(p, 6), lb(p, 9), name.s, sub.s, cost.s, param.s); // 005593DA..0055940E
            I32 dc;                                                                            // [esp+24h] (not initialized: original)
            get_int(lp(two_dim(), 0x90), lb(p, 9), "DC_SAVE", &dc);                            // 00559413..0055945C
            Str nl("\n\n"), sp1(" "), sp2(" ");                                                // 00559461..0055949F [A0h] [90h] [80h]
            Str number(Str::raw); kotor_fastcall_005e5bc0(number.s, nullptr, dc); number.live = true; // 005594A4..005594B8
            Str g(Str::raw); gui(g, 0xBFEF);                                                   // 005594BF..005594E2
            chain(desc, {param.s, sp2.s, g.s, sp1.s, number.s, nl.s});                         // 005594E6..0055966B
            break;
        }
        default: {                                                                             // 00559677
            { Str sp(" "), colon(": "); Str g(Str::raw); gui(g, 0x96B7); chain(desc, {g.s, colon.s, sub.s, sp.s}); } // 00559677..005597BE
            kotor_fastcall_00554e00(self, nullptr, lh(p, 0), lh(p, 2), lh(p, 6), lb(p, 9), name.s, sub.s, cost.s, param.s); // 005597D1..00559803
            Str nl("\n\n"), sep(", ");                                                         // 00559808..0055982D
            chain(desc, {sep.s, cost.s, nl.s});                                                // 00559832..005598EC
            break;
        }
        }
    }
}

// 005599D0 CSWSItem::AddDefenceProperties(CExoString* desc) RET 4.
void __fastcall kotor_fastcall_005599d0(void* self, void*, void* desc) {
    Str text;                                                                                  // [esp+78h]
    I32 defence = lb(base_of(self), 0x8A);                                                     // 00559A01..00559A1B [esp+0Ch]
    if (exists(self, 1) != 0) {                                                                // 00559A16..00559A26
        I32 count = li(self, 0x24C);
        I32 off = 0;
        for (I32 i = 0; i < count; ++i, off += 0x1C, count = li(self, 0x24C)) {                // 00559A2C..00559AC7
            void* p = passive_at(self, i, off);
            if (p == nullptr || !upgrade_active(self, p)) continue;
            if (lh(p, 0) != 1) continue;                                                       // 00559A75..00559A79
            if (lb(base_of(self), 0x8B) != 0 &&
                kotor_fastcall_00555610(self, nullptr, lh(p, 0), lh(p, 2), lh(p, 6)) != 0) continue; // 00559A7B..00559AA9
            defence += lh(p, 6);                                                               // 00559AAB..00559AAF
        }
    }
    if (exists(self, 0x14) != 0) {                                                             // 00559ACD..00559ADA
        void* p = nullptr;                                                                     // [esp+10h]
        recovered_item_property_query_005539c0(self, nullptr, &p, 0x14, 0);                    // 00559ADC..00559AE7
        if (p != nullptr && upgrade_active(self, p) &&
            kotor_fastcall_00555610(self, nullptr, lh(p, 0), lh(p, 2), lh(p, 6)) == 0)         // 00559AEC..00559B26
            defence -= lh(p, 6);                                                               // 00559B28..00559B31
    }
    if (defence != 0) {                                                                        // 00559B35..00559B3E
        { Str colon(": "); Str g(Str::raw); gui(g, 0x96C1); chain(desc, {g.s, colon.s}); }     // 00559B44..00559BEF
        frame_LogFormat_005e5680(text.s, "%d\n", defence);                                     // 00559BF4..00559C08
        Str nl("\n"); chain(desc, {text.s, nl.s});                                             // 00559C0B..00559C89
    }
    if (lb(base_of(self), 0xAC) != 0xF) {                                                      // 00559C8E..00559C9D
        const U8 dex = lb(base_of(self), 0xAD);                                                // 00559CA3..00559CAA
        if (dex != 0xFF) {                                                                     // 00559CB0..00559CB3
            Str nl("\n\n"), plus(": +");                                                       // 00559CB9..00559CD8
            Str number(Str::raw); kotor_fastcall_005e5bc0(number.s, nullptr, dex); number.live = true; // 00559CDD..00559CED
            Str g(Str::raw); gui(g, 0xA4A0);                                                   // 00559CF4..00559D0F
            chain(desc, {g.s, plus.s, number.s, nl.s});                                        // 00559D14..00559DD0
        }
    }
    Str name, sub, cost, param, unused;                                                        // [60h] [40h] [38h] [70h] [118h]
    if (exists(self, 2) != 0 || exists(self, 3) != 0 || exists(self, 4) != 0 || exists(self, 0xE) != 0 || exists(self, 0x11) != 0) { // 00559E58..00559EA5
        I32 count = li(self, 0x24C);
        I32 off = 0;
        for (I32 i = 0; i < count; ++i, off += 0x1C, count = li(self, 0x24C)) {                // 00559EAB..0055A484
            void* p = passive_at(self, i, off);
            if (p == nullptr || !upgrade_active(self, p)) continue;
            const U16 t = lh(p, 0);
            if (t == 2 || t == 3 || t == 4) {                                                  // 00559F04..00559F17
                if (kotor_fastcall_00555610(self, nullptr, t, lh(p, 2), lh(p, 6)) == 0) {      // 00559F1D..00559F3B
                    kotor_fastcall_00554e00(self, nullptr, t, lh(p, 2), lh(p, 6), 0xFF, name.s, sub.s, cost.s, param.s); // 00559F41..00559F67
                    Str nl("\n\n"), sp1(" "), sp2(" "), plus("+");                             // 00559F6C..00559FBD [68h] [50h] [80h] [88h]
                    Str g(Str::raw); gui(g, 0x573);                                            // 00559FC2..00559FDF
                    chain(desc, {plus.s, cost.s, sp2.s, g.s, sp1.s, sub.s, nl.s});             // 00559FE4..0055A1A1
                }
            }
            const U16 u = lh(p, 0);                                                            // 0055A1A6..0055A1AA
            if (u == 0xE || u == 0x11) {                                                       // 0055A1AD..0055A1B7
                if (kotor_fastcall_00555610(self, nullptr, u, lh(p, 2), lh(p, 6)) == 0) {      // 0055A1BD..0055A1DF
                    kotor_fastcall_00554e00(self, nullptr, u, lh(p, 2), lh(p, 6), 0xFF, name.s, sub.s, cost.s, param.s); // 0055A1E5..0055A20B
                    Str nl("\n\n"), sp1(" "), sp2(" "), colon(": ");                           // 0055A210..0055A25B [20h] [28h] [30h] [58h]
                    Str g(Str::raw); gui(g, 0x573);                                            // 0055A260..0055A27D
                    chain(desc, {name.s, colon.s, cost.s, sp2.s, g.s, sp1.s, sub.s, nl.s});    // 0055A282..0055A463
                }
            }
        }
    }
}

// 0055A510 CSWSItem::AddMiscellaneousProperties(CExoString* desc) RET 4.
void __fastcall kotor_fastcall_0055a510(void* self, void*, void* desc) {
    Str name, sub, cost, param, unused, text;                                                  // [1Ch] [2Ch] [34h] [54h] [2E0h] [4Ch]
    char word[0x2C];                                                                           // [esp+2F0h] (sscanf %s target, unbounded: original)
    I32 parsed;                                                                                // [esp+48h] (not initialized: original)
    auto strings = [&](void* p, U32 subtype, U32 value) {
        kotor_fastcall_00554e00(self, nullptr, lh(p, 0), subtype, value, 0xFF, name.s, sub.s, cost.s, param.s);
    };
    auto higher = [&](void* p) { return kotor_fastcall_00555610(self, nullptr, lh(p, 0), lh(p, 2), lh(p, 6)) != 0; };
    // ---- 0x0A activated-charge properties (active list).
    if (exists(self, 0xA) != 0) {                                                              // 0055A59A..0055A5B2
        I32 count = li(self, 0x248);
        // ebx is loaded once from the [esp+74h] stack slot before the loop and only rewritten by the known cost
        // values; any other value reuses it (original). The slot holds no initialized value at this point.
        U32 stale_slot[1];                                                                     // [esp+74h]
        U32 uses = stale_word(stale_slot);                                                     // 0055A5CE
        I32 off = 0;
        for (I32 i = 0; i < count; ++i, off += 0x1C, count = li(self, 0x248)) {                // 0055A5E0..0055A9B3
            void* p = active_at(self, i, off);
            if (p == nullptr || !upgrade_active(self, p)) continue;                            // 0055A5E0..0055A61B
            if (lh(p, 0) != 0xA) continue;                                                     // 0055A621..0055A625
            const U32 value = lh(p, 6);                                                        // 0055A62B
            if (value == 1) {                                                                  // 0055A62F..0055A632
                Str nl("\n\n"); Str g(Str::raw); gui(g, 0x30F); chain(desc, {g.s, nl.s});     // 0055A638..0055A6DA
                continue;
            }
            switch (value - 1u) {                                                              // 0055A6DF..0055A6E5 (table 0055CA8C)
            case 6: case 12: uses = 0; break;
            case 0: case 7: case 13: case 14: case 15: case 16: case 17: uses = 1; break;
            case 8: uses = 2; break;
            case 9: uses = 3; break;
            case 10: uses = 4; break;
            case 11: uses = 5; break;
            case 1: uses = static_cast<U32>(li(self, 0x25C) / 5); break;                       // 0055A713..0055A729
            case 2: uses = static_cast<U32>(li(self, 0x25C) / 4); break;                       // 0055A72D..0055A73B
            case 3: uses = static_cast<U32>(li(self, 0x25C) / 3); break;                       // 0055A740..0055A754
            case 4: case 5: uses = lw(self, 0x25C); break;                                     // 0055A758
            default: break;                                                                    // 0055A6E3 (ja): ebx unchanged
            }
            { Str colon(": "); Str g(Str::raw); gui(g, 0xA3AB); chain(desc, {g.s, colon.s}); } // 0055A75E..0055A80F
            if (uses == 0) {                                                                   // 0055A820..0055A822
                Str nl("\n\n"); Str g(Str::raw); gui(g, 0xA3AC); chain(desc, {g.s, nl.s});     // 0055A828..0055A8DD
            } else {
                const U32 left = static_cast<U16>(kotor_fastcall_00555a60(self, nullptr, static_cast<U8>(i))); // 0055A8E2..0055A8EE
                frame_LogFormat_005e5680(text.s, "%d/%d\n", left, uses);                       // 0055A8F1..0055A902
                Str nl("\n"); chain(desc, {text.s, nl.s});                                     // 0055A905..0055A986
            }
        }
    }
    // ---- 0x18 list ("name: a, b, c\n\n").
    if (exists(self, 0x18) != 0) {                                                             // 0055A9B9..0055A9C7
        I32 n = 0;                                                                             // ebx
        I32 count = li(self, 0x24C);
        I32 off = 0;
        for (I32 i = 0; i < count; ++i, off += 0x1C, count = li(self, 0x24C)) {                // 0055A9CD..0055AB86
            void* p = passive_at(self, i, off);
            if (p == nullptr || !upgrade_active(self, p)) continue;
            if (lh(p, 0) != 0x18) continue;                                                    // 0055AA22..0055AA29
            strings(p, lh(p, 2), 0xFFFF);                                                      // 0055AA2F..0055AA55
            if (n == 0) { Str colon(": "); chain(desc, {name.s, colon.s}); }                   // 0055AA5A..0055AAD4
            else if (n > 0) { Str sep(", "); chain(desc, {sep.s}); }                           // 0055AAD6..0055AB20
            chain(desc, {sub.s});                                                              // 0055AB31..0055AB55
            ++n;                                                                               // 0055AB69
        }
        Str nl("\n\n"); chain(desc, {nl.s});                                                   // 0055AB8C..0055ABD4
    }
    // ---- saving throw bonuses/penalties 0x1A 0x1B / 0x21 0x22.
    if (exists(self, 0x1A) != 0 || exists(self, 0x21) != 0 || exists(self, 0x1B) != 0 || exists(self, 0x22) != 0) { // 0055ABE5..0055AC24
        I32 n = 0;                                                                             // [esp+10h]
        { Str colon(": "); Str g(Str::raw); gui(g, 0xA3CF); chain(desc, {g.s, colon.s}); }     // 0055AC2A..0055ACC9
        I32 count = li(self, 0x24C);
        I32 off = 0;
        for (I32 i = 0; i < count; ++i, off += 0x1C, count = li(self, 0x24C)) {                // 0055ACDA..0055B046
            void* p = passive_at(self, i, off);
            if (p == nullptr || !upgrade_active(self, p)) continue;
            if (higher(p)) continue;                                                           // 0055AD45..0055AD62
            strings(p, lh(p, 2), lh(p, 6));                                                    // 0055AD68..0055AD97
            const U32 t = static_cast<U32>(lh(p, 0)) - 0x1Au;                                  // 0055AD9C..0055ADA5
            if (t > 8) continue;
            static const U8 group[9] = {0, 0, 2, 2, 2, 2, 2, 1, 1};                            // 0055CAE0 -> 0055CAD4
            if (group[t] == 0) {                                                               // 0055ADB9
                if (n > 0) { Str sep(", "); chain(desc, {sep.s}); }                            // 0055ADB9..0055AE09
                Str plus(" +"); chain(desc, {sub.s, plus.s, cost.s});                          // 0055AE1A..0055AEC0
                ++n;                                                                           // 0055B026
            } else if (group[t] == 1) {                                                        // 0055AEC5
                if (n > 0) { Str sep(", "); chain(desc, {sep.s}); }                            // 0055AEC5..0055AF1B
                kotor_sscanf_006fae08(cstr(cost.s), "%s -%d", word, &parsed);                  // 0055AF2C..0055AF48
                frame_LogFormat_005e5680(text.s, "%d", parsed);                                // 0055AF4D..0055AF61
                Str minus(" -"); chain(desc, {sub.s, minus.s, text.s});                        // 0055AF64..0055B015
                ++n;                                                                           // 0055B026
            }
        }
        Str nl("\n\n"); chain(desc, {nl.s});                                                   // 0055B04C..0055B094
    }
    // ---- 0x24 / 0x15 bonuses / penalties.
    if (exists(self, 0x24) != 0 || exists(self, 0x15) != 0) {                                  // 0055B0A5..0055B0C8
        I32 n = 0;                                                                             // [esp+10h]
        { Str colon(": "); Str g(Str::raw); gui(g, 0xA3D0); chain(desc, {g.s, colon.s}); }     // 0055B0CE..0055B16D
        I32 count = li(self, 0x24C);
        I32 off = 0;
        for (I32 i = 0; i < count; ++i, off += 0x1C, count = li(self, 0x24C)) {                // 0055B17E..0055B577
            void* p = passive_at(self, i, off);
            if (p == nullptr || !upgrade_active(self, p)) continue;
            if (higher(p)) continue;                                                           // 0055B1E3..0055B200
            strings(p, lh(p, 2), lh(p, 6));                                                    // 0055B206..0055B235
            const U16 t = lh(p, 0);                                                            // 0055B23A..0055B24C
            if (t == 0x24) {
                if (higher(p)) continue;                                                       // 0055B252..0055B26A
                if (n > 0) { Str sep(", "); chain(desc, {sep.s}); }                            // 0055B270..0055B2C0
                strings(p, lh(p, 2), lh(p, 6));                                                // 0055B2D1..0055B2FA
                Str sp(" "); chain(desc, {sub.s, sp.s, cost.s});                               // 0055B2FF..0055B3A5
                ++n;                                                                           // 0055B557
            } else if (t == 0x15) {
                if (higher(p)) continue;                                                       // 0055B3AA..0055B3C2
                if (n > 0) { Str sep(", "); chain(desc, {sep.s}); }                            // 0055B3C8..0055B41E
                strings(p, lh(p, 2), lh(p, 6));                                                // 0055B42F..0055B458
                kotor_sscanf_006fae08(cstr(cost.s), "%s -%d", word, &parsed);                  // 0055B45D..0055B479
                frame_LogFormat_005e5680(text.s, "-%d", parsed);                               // 0055B47E..0055B492
                Str sp(" "); chain(desc, {sub.s, sp.s, text.s});                               // 0055B495..0055B546
                ++n;                                                                           // 0055B557
            }
        }
        Str nl("\n\n"); chain(desc, {nl.s});                                                   // 0055B57D..0055B5C5
    }
    // ---- remaining single-line properties (table 0055CB18 -> 0055CAEC).
    I32 count = li(self, 0x24C);
    I32 off = 0;
    for (I32 i = 0; i < count; ++i, off += 0x1C, count = li(self, 0x24C)) {                    // 0055B5D6..0055CA01
        void* p = passive_at(self, i, off);
        if (p == nullptr || !upgrade_active(self, p)) continue;                                // 0055B5F4..0055B637
        const U32 t = lh(p, 0);                                                                // 0055B63D..0055B644
        if (t > 0x38) continue;                                                                // 0055B647..0055B64A
        switch (t) {
        case 0x09: {                                                                           // 0055B65E
            strings(p, lh(p, 2), 0xFFFF);
            Str nl("\n\n"), colon(": "); chain(desc, {name.s, colon.s, sub.s, nl.s});          // 0055B68D..0055B7A8
            break;
        }
        case 0x00: {                                                                           // 0055B7AD
            if (higher(p)) break;
            strings(p, lh(p, 2), lh(p, 6));
            Str nl("\n\n"), colon(": "); chain(desc, {sub.s, colon.s, cost.s, nl.s});          // 0055B800..0055B91B
            break;
        }
        case 0x23: case 0x36: {                                                                // 0055B920
            if (higher(p)) break;
            strings(p, lh(p, 2), lh(p, 6));
            Str nl("\n\n"), colon(": "); chain(desc, {name.s, colon.s, cost.s, nl.s});         // 0055B973..0055BA8E
            break;
        }
        case 0x19: {                                                                           // 0055BA93
            if (higher(p)) break;
            strings(p, lh(p, 2), lh(p, 6));
            kotor_sscanf_006fae08(cstr(cost.s), "%s %d", word, &parsed);                       // 0055BAE6..0055BB02
            frame_LogFormat_005e5680(text.s, "%d", parsed);                                    // 0055BB07..0055BB1B
            Str nl("\n\n"), plus(": +"); Str g(Str::raw); gui(g, 0xA3CA);                      // 0055BB1E..0055BB65
            chain(desc, {g.s, plus.s, text.s, nl.s});                                          // 0055BB6A..0055BC6B
            break;
        }
        case 0x13: {                                                                           // 0055BC70
            if (higher(p)) break;
            strings(p, lh(p, 2), lh(p, 6));
            kotor_sscanf_006fae08(cstr(cost.s), "%s -%d", word, &parsed);                      // 0055BCC3..0055BCDF
            frame_LogFormat_005e5680(text.s, "-%d", parsed);                                   // 0055BCE4..0055BCF8
            Str nl("\n\n"), colon(": "); chain(desc, {sub.s, colon.s, text.s, nl.s});          // 0055BCFB..0055BE16
            break;
        }
        case 0x1D: case 0x2F: case 0x32: {                                                     // 0055BE1B
            strings(p, 0xFFFF, 0xFFFF);
            Str nl("\n\n"); chain(desc, {name.s, nl.s});                                       // 0055BE46..0055BED1
            break;
        }
        case 0x37: {                                                                           // 0055BED6
            if (higher(p)) break;
            strings(p, 0xFFFF, lh(p, 6));                                                      // 0055BEF6..0055BF22
            frame_LogFormat_005e5680(text.s, "%d", static_cast<U32>(lh(p, 6)));                // 0055BF27..0055BF3F
            Str left, right;                                                                   // [5Ch] [C0h]
            const I32 pos = find_005e5f20(name.s, nullptr, 0x3A, 0);                           // 0055BF5F..0055BF6F
            if (pos != -1) {                                                                   // 0055BF74..0055BF79
                { Str t(Str::raw); base_ini_reconstruction::left_005e5f70(ss(name.s), nullptr, ss(t.s), pos); t.live = true; assign(left.s, t.s); } // 0055BF7F..0055BFA3
                { const I32 n = static_cast<I32>(async_string_length_005e5790(name.s, nullptr)) - pos - 1;
                  Str t(Str::raw); base_ini_reconstruction::right_005e6170(ss(name.s), nullptr, ss(t.s), n); t.live = true; assign(right.s, t.s); } // 0055BFB7..0055BFEA
                Str colon(": ");                                                               // 0055BFFE..0055C00A
                Str c(Str::raw); WindowCopy005e5b00(c.s, nullptr, left.s); c.live = true;      // 0055C00F..0055C023
                Str t(Str::raw); GuiConcat005e5d10(ss(c.s), nullptr, ss(t.s), ss(colon.s)); t.live = true; // 0055C028..0055C042
                assign(name.s, t.s);                                                           // 0055C047..0055C054
            }
            Str nl("\n\n"), plus("+"); chain(desc, {name.s, plus.s, text.s, nl.s});            // 0055C095..0055C1D5
            break;
        }
        case 0x38: {                                                                           // 0055C1DA
            if (higher(p)) break;
            strings(p, 0xFFFF, lh(p, 6));                                                      // 0055C1FE..0055C224
            Str left, right;                                                                   // [24h] [A4h]
            const I32 pos = find_005e5f20(name.s, nullptr, 0x3A, 0);                           // 0055C246..0055C256
            if (pos != -1) {                                                                   // 0055C25B..0055C260
                { Str t(Str::raw); base_ini_reconstruction::left_005e5f70(ss(name.s), nullptr, ss(t.s), pos); t.live = true; assign(left.s, t.s); } // 0055C266..0055C28A
                { const I32 n = static_cast<I32>(async_string_length_005e5790(name.s, nullptr)) - pos - 1;
                  Str t(Str::raw); base_ini_reconstruction::right_005e6170(ss(name.s), nullptr, ss(t.s), n); t.live = true; assign(right.s, t.s); } // 0055C29E..0055C2D1
                Str colon(": ");                                                               // 0055C2E5..0055C2F1
                Str c(Str::raw); WindowCopy005e5b00(c.s, nullptr, left.s); c.live = true;      // 0055C2F6..0055C30A
                Str t(Str::raw); GuiConcat005e5d10(ss(c.s), nullptr, ss(t.s), ss(colon.s)); t.live = true; // 0055C30F..0055C329
                assign(name.s, t.s);                                                           // 0055C32E..0055C33B
            }
            kotor_sscanf_006fae08(cstr(cost.s), "%s -%d", word, &parsed);                      // 0055C37C..0055C398
            frame_LogFormat_005e5680(text.s, "%d", parsed);                                    // 0055C39D..0055C3B1
            Str nl("\n\n"), minus(" -"); chain(desc, {name.s, minus.s, text.s, nl.s});         // 0055C3B4..0055C4F4
            break;
        }
        case 0x12: {                                                                           // 0055C4F9
            if (higher(p)) break;
            strings(p, lh(p, 2), 0xFFFF);                                                      // 0055C519..0055C543
            I32 chance, rounds, dc;                                                            // [74h] [ACh] [14h]
            kotor_fastcall_005554d0(self, nullptr, lh(p, 6), lb(p, 9), &chance, &rounds, &dc); // 0055C548..0055C574
            const I32 rounds3 = rounds * 3;                                                    // 0055C579..0055C599
            {
                Str g1(Str::raw); gui(g1, 0xBB06);                                             // 0055C580..0055C5A5
                Str g2(Str::raw); gui(g2, 0x96AE);                                             // 0055C5A9..0055C5CC
                const char* s1 = cstr(g1.s); const char* s2 = cstr(g2.s);                      // 0055C5D0..0055C5E7
                frame_LogFormat_005e5680(text.s, "%d%% %s, %d%s ", chance, s2, rounds3, s1);   // 0055C5EC..0055C604
            }
            Str nl("\n\n"), sp(" "), colon(": ");                                              // 0055C62F..0055C66A [6Ch] [84h] [94h]
            Str g3(Str::raw); gui(g3, 0x573);                                                  // 0055C66F..0055C692
            Str g4(Str::raw); gui(g4, 0xA3AA);                                                 // 0055C694..0055C6B2
            chain(desc, {g4.s, colon.s, text.s, g3.s, sp.s, sub.s, nl.s});                     // 0055C6B7..0055C865
            break;
        }
        case 0x2B: case 0x2C: {                                                                // 0055C86E
            strings(p, lh(p, 2), 0xFFFF);
            Str nl("\n\n"), colon(": "); Str g(Str::raw); gui(g, 0xA3CE);                      // 0055C89D..0055C8DE
            chain(desc, {g.s, colon.s, sub.s, nl.s});                                          // 0055C8E3..0055C9D4
            break;
        }
        default: break;                                                                        // 0055C9E5
        }
    }
}

// 0055F340 CSWSItem::GetPropertyDescription(CExoString* out) RET 4 -> out.
void* __fastcall kotor_fastcall_0055f340(void* self, void*, void* out) {
    Str desc;                                                                                  // [esp+0Ch]
    const U8 kind = lb(base_of(self), 0xAC);                                                   // 0055F37F..0055F38B
    if (kind != 0x2E && lb(base_of(self), 0xAC) != 6) {                                        // 0055F38B..0055F3A6
        kotor_fastcall_00556490(self, nullptr, desc.s);                                        // 0055F3A8..0055F3AF
        if (lb(base_of(self), 9) != 0) {                                                       // 0055F3B4..0055F3BE
            kotor_fastcall_00556de0(self, nullptr, desc.s);                                    // 0055F3C0..0055F3C7
            kotor_fastcall_005543b0(self, nullptr, desc.s);                                    // 0055F3CC..0055F3D3
            kotor_fastcall_00558950(self, nullptr, desc.s);                                    // 0055F3D8..0055F3DF
            kotor_fastcall_00558c10(self, nullptr, desc.s);                                    // 0055F3E4..0055F3EB
            kotor_fastcall_005544f0(self, nullptr, desc.s);                                    // 0055F3F0..0055F3F7
        }
        kotor_fastcall_0055e930(self, nullptr, desc.s);                                        // 0055F3FC..0055F403
        kotor_fastcall_005599d0(self, nullptr, desc.s);                                        // 0055F408..0055F40F
        kotor_fastcall_0055a510(self, nullptr, desc.s);                                        // 0055F414..0055F41B
    }
    Str text;                                                                                  // [esp+14h]
    exo_locstring_get_005ea130(at(self, 0x270), nullptr, kotor_fastcall_005ed4c0(client_app(), nullptr), text.s, 0); // 0055F420..0055F44A
    if (static_cast<SS*>(static_cast<void*>(text.s))->BorrowedEquals005e5290("") != 0) {      // 0055F44F..0055F45F
        Str g(Str::raw); gui(g, 0x7DAC); assign(text.s, g.s);                                  // 0055F461..0055F490
    }
    { Str t(Str::raw); GuiConcat005e5d10(ss(desc.s), nullptr, ss(t.s), ss(text.s)); t.live = true; assign(text.s, t.s); } // 0055F495..0055F4C0
    WindowCopy005e5b00(out, nullptr, text.s);                                                  // 0055F4C5..0055F4D0
    return out;                                                                                // 0055F4D5..0055F4FD (~text, ~desc)
}

// 0055D950 CSWSItem::CopyItem(CSWSItem* source) RET 4. A previously owned container repository is overwritten without
// being released (original).
U32 __fastcall kotor_fastcall_0055d950(void* self, void*, void* source) {
    if (source == nullptr) return 0;                                                           // 0055D96E..0055D973
    if (lp(source, 0x26C) != nullptr && lw(lp(source, 0x26C), 0x10) != 0) return 0;          // 0055D975..0055D986
    recovered_localized_assign_005e9e70(at(self, 0x270), nullptr, at(source, 0x270));          // 0055D98B..0055D998
    recovered_localized_assign_005e9e70(at(self, 0x278), nullptr, at(source, 0x278));          // 0055D99D..0055D9AA
    recovered_localized_assign_005e9e70(at(self, 0x280), nullptr, at(source, 0x280));          // 0055D9AF..0055D9BC
    assign(at(self, 0x28), at(source, 0x28));                                                  // 0055D9C1..0055D9CC
    sw(self, 0x268, lw(source, 0x268));                                                        // 0055D9D1..0055D9D7
    if (lp(source, 0x26C) != nullptr) {                                                        // 0055D9DD..0055D9E3
        void* repo = live_operator_new(0x18);                                                  // 0055D9E5..0055D9EC
        if (repo != nullptr) {                                                                 // 0055D9EF..0055D9F1 (CItemRepository(owner) inlined)
            sw(repo, 0x10, 0); sw(repo, 0x14, 0); sw(repo, 0x0C, 0);
            sw(repo, 0x00, lw(self, 0x14));
            sw(repo, 0x08, lw(repo, 0x08) & 0xFFFFFFFEu);
            sw(repo, 0x04, 0);                                                                 // 0055D9F3..0055DA0A
        }
        sw(self, 0x26C, ad(repo));                                                             // 0055DA19
    }
    sb(self, 0x264, lb(source, 0x264));                                                        // 0055DA1F..0055DA25
    sw(self, 0x248, lw(source, 0x248));                                                        // 0055DA2B..0055DA33
    if (li(source, 0x248) > 0) {                                                               // 0055DA39
        sw(self, 0x250, ad(live_operator_new(lw(source, 0x248) * 0x1Cu)));                     // 0055DA3B..0055DA44
        for (I32 k = 0; k < li(self, 0x248); ++k)                                              // 0055DA4A..0055DA83
            std::memcpy(static_cast<U8*>(lp(self, 0x250)) + k * 0x1C, static_cast<U8*>(lp(source, 0x250)) + k * 0x1C, 0x1C);
    }
    sw(self, 0x24C, lw(source, 0x24C));                                                        // 0055DA87..0055DA8F
    if (li(source, 0x24C) > 0) {                                                               // 0055DA95
        sw(self, 0x254, ad(live_operator_new(lw(source, 0x24C) * 0x1Cu)));                     // 0055DA97..0055DAA0
        for (I32 k = 0; k < li(self, 0x24C); ++k)                                              // 0055DAA6..0055DAE3
            std::memcpy(static_cast<U8*>(lp(self, 0x254)) + k * 0x1C, static_cast<U8*>(lp(source, 0x254)) + k * 0x1C, 0x1C);
    }
    sw(self, 0x260, lw(source, 0x260));                                                        // 0055DAE5..0055DAF1
    sw(self, 0x258, lw(source, 0x258));                                                        // 0055DAF7..0055DAFD
    sw(self, 0x25C, lw(source, 0x25C));                                                        // 0055DB03..0055DB0F
    sw(self, 0x244, lw(source, 0x244));                                                        // 0055DB15..0055DB1B
    sw(self, 0x288, (lw(self, 0x288) & ~4u) | (lw(source, 0x288) & 4u));                       // 0055DAEB..0055DB2C
    sw(self, 0x28C, lw(source, 0x28C));                                                        // 0055DB32..0055DB38
    sw(self, 0x290, lw(source, 0x290));                                                        // 0055DB3E..0055DB44
    sw(self, 0x288, (lw(self, 0x288) & ~1u) | (lw(source, 0x288) & 1u));                       // 0055DB4A..0055DB5B
    sw(self, 0x294, lw(source, 0x294));                                                        // 0055DB61..0055DB6D
    sb(self, 0x265, lb(source, 0x265));                                                        // 0055DB73..0055DB7F
    sb(self, 0x266, lb(source, 0x266));                                                        // 0055DB85..0055DB8B
    sw(self, 0x288, (lw(self, 0x288) & ~0x40u) | (lw(source, 0x288) & 0x40u));                 // 0055DB91..0055DB9C
    sw(self, 0x0C, lw(source, 0x0C));                                                          // 0055DBA2..0055DBA5
    for (U32 k = 4; k <= 9; ++k) sb(self, k, lb(source, k));                                   // 0055DBA8..0055DBC9
    recovered_module_register_tag_004c7de0(server_get_module_004ae6b0(server_app(), nullptr), nullptr, at(self, 0x28), lw(self, 0x14)); // 0055DBCC..0055DBE4
    return 1;                                                                                  // 0055DBE9
}

// ---------------------------------------------------------------- CSWGuiInGameEquip
// 006B4C90 CSWGuiInGameEquip::GetSlotItemIdIndex(int slot_mask) RET 4 (index into +427Ch; 0Ah when unknown).
I32 __fastcall kotor_fastcall_006b4c90(void*, void*, I32 mask) {
    switch (mask) {                                                                            // 006B4C94..006B4D12 (tables 006B4D30 / 006B4D1C)
    case 0x20: return 0;
    case 0x10: return 1;
    case 0x01: return 2;
    case 0x80: return 3;
    case 0x100: return 4;
    case 0x02: return 5;
    case 0x08: return 6;
    case 0x200: return 7;
    case 0x400: return 8;
    default: return 0xA;
    }
}

// 006B66A0 CSWGuiInGameEquip::ItemExists(DWORD client_id) RET 4.
U32 __fastcall kotor_fastcall_006b66a0(void*, void*, U32 id) {
    void* object;                                                                              // the argument slot (original)
    const U32 server_id = kotor_fastcall_004aea30(server_app(), nullptr, id);                  // 006B66A0..006B66AD
    if (kotor_fastcall_004d8230(recovered_object_array_004aed70(server_app(), nullptr), nullptr, server_id, &object) != 0) return 0; // 006B66B2..006B66D3 ([0074666C] = 0)
    return vcall<void*>(object, 0x38) != nullptr ? 1u : 0u;                                    // 006B66D5..006B66E4 (AsSWSItem)
}

// 006B65E0 CSWGuiInGameEquip::CopyItem(DWORD client_id) RET 4 -> new CSWSItem copy or NULL. A failed allocation still
// copies into NULL (original).
void* __fastcall kotor_fastcall_006b65e0(void*, void*, U32 id) {
    void* object = nullptr;                                                                    // [esp+4]
    void* copy = nullptr;                                                                      // esi
    const U32 server_id = kotor_fastcall_004aea30(server_app(), nullptr, id);                  // 006B65F6..006B660A
    if (kotor_fastcall_004d8230(recovered_object_array_004aed70(server_app(), nullptr), nullptr, server_id, &object) != 0) return nullptr; // 006B660F..006B6630
    if (vcall<void*>(object, 0x38) == nullptr) return nullptr;                                 // 006B6632..006B663D
    void* raw = live_operator_new(0x29C);                                                      // 006B663F..006B6644
    if (raw != nullptr) {                                                                      // 006B6650..006B6656
        auto g = on_throw([&]() { live_free(raw); });                                          // EH state 0
        copy = recovered_item_construct_005530a0(raw, nullptr, 0x7F000000u);                   // 006B6658..006B665F
        g.armed = false;
    }
    kotor_fastcall_0055d950(copy, nullptr, vcall<void*>(object, 0x38));                       // 006B6666..006B667A
    return copy;
}

// AddAttackActions(target, 0,0,0,0,0,0, 1, 2719h, 5DCh, 1, 0, 0, 4, 0) re-issued after an equipment change in combat.
namespace { void resume_attack(void* creature) {
    if (lw(creature, 0x4E0) == 0) return;
    const U32 target = lw(lp(creature, 0x9C8), 0x9CC);
    if (target == 0x7F000000u) return;
    kotor_fastcall_004fde40(creature, nullptr, target, 0, 0, 0, 0, 0, 0, 1, 0x2719, 0x5DC, 1, 0, 0, 4, 0);
} }

// 006B5760 CSWGuiInGameEquip::EquipItem(DWORD client_id, DWORD slot, int clear_actions) RET 0Ch.
U32 __fastcall kotor_fastcall_006b5760(void* self, void*, U32 id, U32 slot, U32 clear) {
    void* object = nullptr;                                                                    // [esp+8]
    const U32 server_id = kotor_fastcall_004aea30(server_app(), nullptr, id);                  // 006B5761..006B577A
    if (kotor_fastcall_004d8230(recovered_object_array_004aed70(server_app(), nullptr), nullptr, server_id, &object) != 0) return 0; // 006B577F..006B57A0
    if (vcall<void*>(object, 0x38) == nullptr) return 0;                                       // 006B57A6..006B57B1
    void* creature = kotor_m15a_creature_get_server_creature_0060fb20(lp(self, 0x64), nullptr); // 006B57B7..006B57BA
    if (creature == nullptr) return 0;                                                         // 006B57BF..006B57C3
    void* base = kotor_m15b_base_item_array_get_base_item_005b31a0(lp(rules(), 0x34), nullptr, li(vcall<void*>(object, 0x38), 0x0C)); // 006B57C9..006B57DE
    if ((lw(base, 4) & slot) == 0) return 0;                                                   // 006B57E3..006B57EA
    if (clear != 0) {                                                                          // 006B57F1..006B57F7
        w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);                            // 006B57F9..006B57FD
        recovered_combat_round_remove_all_actions_004d3770(lp(creature, 0x9C8), nullptr);      // 006B5802..006B5808
    }
    recovered_object_equip_004f0420(creature, nullptr, vcall<void*>(object, 0x38), slot, 0, 1, 1, 0, clear == 0 ? 1u : 0u); // 006B580D..006B582A
    resume_attack(creature);                                                                   // 006B582F..006B5872
    return 1;
}

// 006B5890 CSWGuiInGameEquip::EquipItem(CSWSItem* copy, DWORD slot, int clear_actions) RET 0Ch: equips the inventory
// item matching the copy.
U32 __fastcall kotor_fastcall_006b5890(void* self, void*, void* copy, U32 slot, U32 clear) {
    void* creature = kotor_m15a_creature_get_server_creature_0060fb20(lp(self, 0x64), nullptr); // 006B5898..006B589C
    if (creature == nullptr) return 0;                                                         // 006B58A1..006B58A3
    void* repository = recovered_creature_inventory_004ef770(creature, nullptr, 1);            // 006B58A5..006B58A9
    for (U32 i = 0; i < lw(repository, 0x10); ++i) {                                           // 006B58AE..006B58DC (jb)
        void* item = w2x_inventory_item_005560b0(repository, nullptr, i);                      // 006B58C0..006B58C3
        if (recovered_compare_item_00553cf0(copy, nullptr, item) != 0)                         // 006B58C8..006B58D4
            return kotor_fastcall_006b5760(self, nullptr, lw(item, 0x14), slot, clear);        // 006B58E8..006B58FA
    }
    return 0;
}

// 006B5910 CSWGuiInGameEquip::UnequipItem(DWORD client_id, int clear_actions) RET 8.
void __fastcall kotor_fastcall_006b5910(void* self, void*, U32 id, U32 clear) {
    void* item = recovered_item_lookup_004ae760(server_app(), nullptr, kotor_fastcall_004aea30(server_app(), nullptr, id)); // 006B5910..006B5931
    if (item == nullptr) return;                                                               // 006B5936..006B593A
    void* creature = kotor_m15a_creature_get_server_creature_0060fb20(lp(self, 0x64), nullptr); // 006B5940..006B5943
    if (creature == nullptr) return;                                                           // 006B5948..006B594C
    if (clear != 0) {                                                                          // 006B5952..006B5958
        w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);                            // 006B595A..006B595E
        recovered_combat_round_remove_all_actions_004d3770(lp(creature, 0x9C8), nullptr);      // 006B5963..006B5969
    }
    recovered_object_unequip_004f06d0(creature, nullptr, item, 0x7F000000u, 0, 1, 1);          // 006B596E..006B597C
    if (void* client_item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, id)) // 006B5981..006B5992
        kotor_m15a_item_power_item_00647130(client_item, nullptr, 0, 0, 0, 0);                 // 006B5994..006B599E
    resume_attack(creature);                                                                   // 006B59A3..006B59E5
}

// 006B56B0 CSWGuiInGameEquip::SetDescription(CExoString* text) RET 4.
void __fastcall kotor_fastcall_006b56b0(void* self, void*, void* text) {
    gui_load_text_params_set_text_00415e00(at(self, 0x2400), nullptr, text);                   // 006B56BC..006B56C3
    void* listbox = at(self, 0x33B8);                                                          // 006B56C8 (LB_DESC)
    gui_load_listbox_clear_items_00419a90(listbox, nullptr);                                   // 006B56CE..006B56D0
    I32 extent[4];                                                                             // [esp+0Ch]
    std::memcpy(extent, at(self, 0x231C), 16);                                                 // 006B56D5..006B56F4
    if (static_cast<I32>(async_string_length_005e5790(text, nullptr)) > 0)                     // 006B56F2..006B5705 (jle)
        extent[3] = vcall<I32>(at(self, 0x23E8), 0x08);                                        // 006B5707..006B5709
    else
        extent[3] = gui_load_text_font_height_00414ee0(at(self, 0x23E8), nullptr);             // 006B570E
    void* label = at(self, 0x2318);                                                            // 006B571D
    vcall<void, const void*>(label, 0x04, extent);                                             // 006B5713..006B572A SetExtent
    void* controls[1] = {label};                                                               // the argument slot (original)
    gui_m48_listbox_add_controls_0041c2c0(listbox, nullptr, controls, 1, 0, 0, 0);             // 006B572D..006B5740
    gui_load_listbox_set_active_control_0041c160(listbox, nullptr, label, 0);                  // 006B5745..006B574A
}

// 006B7920 CSWGuiInGameEquip::OnItemSelected(CSWGuiControl* entry) RET 4 (item entry event 0).
void __fastcall kotor_fastcall_006b7920(void* self, void*, void* entry) {
    if (lw(entry, 0x4C) == 0) return;                                                          // 006B7942..006B794B
    if ((lb(self, 0x33FC) & 2u) == 0) { vcall<void, U32>(entry, 0x40, 0); return; }            // 006B7951..006B7972
    const U32 id = lw(entry, 0x1C4);                                                           // 006B7975 [esp+10h]
    U32 equipped = kotor_m15a_creature_get_equipped_item_id_0060ca80(lp(self, 0x64), nullptr, lw(self, 0x4278)); // 006B797B..006B7994 [esp+14h]
    if (id != 0x7F000000u) {                                                                   // 006B798E..006B7998
        void* item = recovered_item_lookup_004ae760(server_app(), nullptr, kotor_fastcall_004aea30(server_app(), nullptr, id)); // 006B799E..006B79BC
        if (item != nullptr) {                                                                 // 006B79BE..006B79C0
            Str text;                                                                          // [esp+18h]
            { Str t(Str::raw); kotor_fastcall_0055f340(item, nullptr, t.s); t.live = true; assign(text.s, t.s); } // 006B79C6..006B79FB
            if (client_movies::StringNotEqual_005e5390(reinterpret_cast<options_video::String*>(text.s), nullptr, "") != 0) // 006B7A00..006B7A10
                kotor_fastcall_006b56b0(self, nullptr, text.s);                                // 006B7A12..006B7A19
            else { Str g(Str::raw); gui(g, 0x7DAC); kotor_fastcall_006b56b0(self, nullptr, g.s); } // 006B7A27..006B7A55
        } else if (id == lw(self, 0x42AC) && lp(self, 0x42B0) != nullptr) {                    // 006B7A63..006B7A7C
            bool has;
            { Str t(Str::raw); kotor_fastcall_0055f340(lp(self, 0x42B0), nullptr, t.s); t.live = true;
              has = client_movies::StringNotEqual_005e5390(reinterpret_cast<options_video::String*>(t.s), nullptr, "") != 0; } // 006B7A7E..006B7AA6
            if (has) { Str t(Str::raw); kotor_fastcall_0055f340(lp(self, 0x42B0), nullptr, t.s); t.live = true; kotor_fastcall_006b56b0(self, nullptr, t.s); } // 006B7AAF..006B7AD3
            else { Str g(Str::raw); gui(g, 0x7DAC); kotor_fastcall_006b56b0(self, nullptr, g.s); } // 006B7AD5..006B7B01
        } else {
            Str g(Str::raw); gui(g, 0x7DAC); kotor_fastcall_006b56b0(self, nullptr, g.s);      // 006B7AD5..006B7B01
        }
    } else {
        Str empty(""); kotor_fastcall_006b56b0(self, nullptr, empty.s);                        // 006B7B03..006B7B30
    }
    if ((lb(self, 0x311C) & 8u) == 0) return;                                                  // 006B7B35..006B7B3C
    if (kotor_m15a_creature_get_server_creature_0060fb20(lp(self, 0x64), nullptr) == nullptr) return; // 006B7B42..006B7B4C
    kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 0);                                       // 006B7B52..006B7B57
    const U8 state = lb(entry, 0x398);                                                         // 006B7B5C..006B7B5F
    if (state > 3) return;                                                                     // 006B7B65..006B7B6B
    if (state >= 2) {                                                                          // 006B7DEE (table 006B7E20)
        kotor_fastcall_006b59f0(self, nullptr, 1, state == 2 ? 0x9632u : 0xA51Fu);             // 006B7DEE..006B7E07
        return;
    }
    kotor_fastcall_006b59f0(self, nullptr, 0, 0xFFFFFFFFu);                                    // 006B7B78..006B7B7D
    if (id == equipped) return;                                                                // 006B7B82..006B7B8C
    if (lw(self, 0x42AC) == 0) {                                                               // 006B7B92..006B7B9A
        const I32 index = kotor_fastcall_006b4c90(self, nullptr, li(self, 0x4278));            // 006B7B9C..006B7BA5
        void* old = lp(self, 0x42B0);                                                          // 006B7BAA
        sw(self, 0x42AC, lw(self, 0x427C + static_cast<U32>(index) * 4u));                     // 006B7BB2..006B7BB9
        if (old != nullptr) vcall<void*, U32>(old, 0x00, 1);                                   // 006B7BBF..006B7BC5
        sw(self, 0x42B0, ad(kotor_fastcall_006b65e0(self, nullptr, lw(self, 0x42AC))));        // 006B7BC7..006B7BD5
    }
    if (id == 0x7F000000u) {                                                                   // 006B7BDB..006B7BE1 / 006B7D5B
        kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 0xB);                                 // 006B7D5B..006B7D65
        U32 clear = 1;                                                                         // edi
        if (lw(self, 0x4278) == 0x10 && lw(self, 0x427C) != 0x7F000000u) {                     // 006B7D6A..006B7D7E
            sw(self, 0x42B4, lw(self, 0x427C));                                                // 006B7D80..006B7D88
            if (void* old = lp(self, 0x42B8)) vcall<void*, U32>(old, 0x00, 1);                 // 006B7D8E..006B7D93
            sw(self, 0x42B8, ad(kotor_fastcall_006b65e0(self, nullptr, lw(self, 0x42B4))));    // 006B7D95..006B7DAE
            kotor_fastcall_006b5910(self, nullptr, lw(self, 0x427C), 1);                       // 006B7DA3..006B7DB4
            clear = 0;                                                                         // 006B7DB9
        }
        kotor_fastcall_006b5910(self, nullptr, equipped, clear);                               // 006B7DBB..006B7DBF
    } else {
        void* item = nullptr;                                                                  // edi
        if (lw(self, 0x4278) == 0x10) {                                                        // 006B7BE7..006B7BEE
            item = recovered_item_lookup_004ae760(server_app(), nullptr, kotor_fastcall_004aea30(server_app(), nullptr, id)); // 006B7BF0..006B7C0D
            if (item != nullptr && lb(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0x1B) == 4 &&
                lw(self, 0x427C) != 0x7F000000u) {                                             // 006B7C0F..006B7C2B
                sw(self, 0x42B4, lw(self, 0x427C));                                            // 006B7C35
                if (void* old = lp(self, 0x42B8)) vcall<void*, U32>(old, 0x00, 1);             // 006B7C2D..006B7C41
                sw(self, 0x42B8, ad(kotor_fastcall_006b65e0(self, nullptr, lw(self, 0x42B4)))); // 006B7C43..006B7C51
            }
        }                                                                                      // (006B7C59: other slots keep edi = equipped id, unused)
        kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 0xA);                                 // 006B7C5D..006B7C62
        if (void* client_item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, equipped)) // 006B7C67..006B7C7A
            kotor_m15a_item_power_item_00647130(client_item, nullptr, 0, 0, 0, 0);             // 006B7C7C..006B7C82
        if (kotor_fastcall_006b66a0(self, nullptr, id) != 0)                                   // 006B7C87..006B7C95
            kotor_fastcall_006b5760(self, nullptr, id, lw(self, 0x4278), 1);                   // 006B7C97..006B7CA7
        else if (id == lw(self, 0x42AC) && lp(self, 0x42B0) != nullptr)                        // 006B7CAE..006B7CC2
            kotor_fastcall_006b5890(self, nullptr, lp(self, 0x42B0), lw(self, 0x4278), 1);     // 006B7CC4..006B7CD0
        if (lw(self, 0x4278) == 0x10 && item != nullptr &&
            lb(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0x1B) < 4 && lw(self, 0x42B4) != 0) { // 006B7CD5..006B7D03
            if (kotor_fastcall_006b66a0(self, nullptr, lw(self, 0x42B4)) != 0)                 // 006B7D09..006B7D13
                kotor_fastcall_006b5760(self, nullptr, lw(self, 0x42B4), 0x20, 0);             // 006B7D15..006B7D21
            else if (void* copy = lp(self, 0x42B8))                                            // 006B7D28..006B7D30
                kotor_fastcall_006b5890(self, nullptr, copy, 0x20, 0);                         // 006B7D32..006B7D38
            void* old = lp(self, 0x42B8);                                                      // 006B7D3D
            sw(self, 0x42B4, 0);                                                               // 006B7D45
            if (old != nullptr) { vcall<void*, U32>(old, 0x00, 1); sw(self, 0x42B8, 0); }      // 006B7D4B..006B7D53
        }
    }
    sw(self, 0x42A4, lw(self, 0x4278));                                                        // 006B7DC4..006B7DCE
    sw(self, 0x42A8, id);                                                                      // 006B7DD4
}
}  // extern "C"
