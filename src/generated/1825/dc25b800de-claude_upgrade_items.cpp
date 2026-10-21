// Lane W4 GUI successor (worker "upgrade", fork C): the CSWSItem helpers the upgrade workbench reaches that had no
// provider in the c41 candidate map:
//   005541C0 CSWSItem::GetUpgradableType(C2DA* upgrade_2da)                  thiscall RET 4   -> BYTE (AL)
//   0055F510 CSWSItem::GetKeyedPropertyString(CExoString* out, BYTE key)      thiscall RET 8   -> out
//   0055E180 CSWSItem::AddProperty(WORD type, WORD subtype, WORD cost_value, BYTE param_value,
//            CExoArrayList<CExoString>* list)                                thiscall RET 14h (this unused)
//   005562D0 CSWSItem::GetSortedPropertyStrings(CExoArrayList<CExoString>* list, CExoString* out)
//                                                                            thiscall RET 8   (this unused)
// Transcribed from swkotor.exe (read-only); each statement carries its original PCs.
//
// CSWSItem property records (0x1C bytes, see claude_equip_items2.cpp): +0 WORD type, +2 WORD subtype, +6 WORD cost
// value, +9 BYTE param value, +11h BYTE upgrade slot. Active list count +248h / data +250h, passive +24Ch / +254h.
// CExoArrayList<CExoString> = {CExoString* data (new[] with count word at data-4), int size, int capacity}.
//
// Quirks kept: GetUpgradableType reads byte +11h through the inline accessor without a null test (the accessor's
// null result for a negative offset / out-of-range index would fault, as in the original); AddProperty leaks the
// subtype 2DA when Load2DArray fails and then skips adding the string; GetSortedPropertyStrings consumes the list by
// shifting elements down with operator= and decrementing the size (the vacated tail element is not destroyed).
#include "52a797cc9c-claude_upgrade.hpp"

namespace directory_list {
std::int32_t __fastcall Less005e5410(kotor_reconstruction::StartupString* self, void*,
                                     kotor_reconstruction::StartupString* other);                 // CExoString::operator<
}

extern "C" {
// ---- 2DA / resources
w4::I32 __fastcall c2da_current_get_int_00413660(void* table, void*, w4::I32 row, void* column, w4::I32* out);     // C2DA::GetINTEntry RET 0Ch
w4::I32 __fastcall c2da_current_get_string_00413270(void* table, void*, w4::I32 row, void* column, void* out);     // C2DA::GetCExoStringEntry RET 0Ch
void* __fastcall c2da_current_construct_00413cc0(void* self, void*, menu_resources::Ref name, w4::I32 mode) noexcept(false); // C2DA::C2DA
w4::I32 __fastcall c2da_current_load_004143b0(void* table, void*);                                                 // C2DA::Load2DArray
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, void* string);                             // CResRef(const CExoString&)
void* __fastcall kotor_fastcall_005c49a0(void* two_dim, void*, w4::U32 index);                                   // CTwoDimArrays::GetIPRPCostTable
void* __fastcall kotor_fastcall_005c4c00(void* two_dim, void*, w4::U32 index);                                   // CTwoDimArrays::GetIPRPParamTable
// ---- CExoArrayList<CExoString>::Add(CExoString) by value (callee destroys the argument) RET 8
void __fastcall claude_exo_string_list_add_004059a0(void* list, void*, gchar::ExoVal value);
// ---- defined here
void __fastcall kotor_fastcall_0055e180(void* self, void*, w4::U32 type, w4::U32 subtype, w4::U32 cost, w4::U32 param, void* list);
void __fastcall kotor_fastcall_005562d0(void* self, void*, void* list, void* out);
}

using namespace gchar;

namespace {
// .rdata literals
const char k_upgrade_type_00748db8[] = "UpgradeType";                                              // 00748DB8
const char k_name_00745eec[] = "Name";                                                             // 00745EEC
const char k_subtype_00748df8[] = "SubTypeResRef";                                                 // 00748DF8
const char k_cost_00748de8[] = "CostTableResRef";                                                  // 00748DE8
const char k_param_00748dd8[] = "Param1ResRef";                                                    // 00748DD8
const char k_colon_0073e2a0[] = ":";                                                               // 0073E2A0
const char k_space_0073d5d8[] = " ";                                                               // 0073D5D8
const char k_comma_00748668[] = ",";                                                               // 00748668
const char k_plus_00748f34[] = " +";                                                               // 00748F34
const char k_sep_00748dd4[] = ", ";                                                                // 00748DD4
const char k_colon_nl_00748e94[] = ":\n";                                                          // 00748E94
const char k_nl_0073d5cc[] = "\n";                                                                 // 0073D5CC

using SS = kotor_reconstruction::StartupString;
inline SS* sp(void* p) { return static_cast<SS*>(p); }
inline void* rules2() { return kotor_cell_rules_007a3a28; }
inline void* two_dim() { return lp(rules2(), 0xB8); }                                              // CSWRules +B8h CTwoDimArrays*
inline void* item_property_def() { return lp(two_dim(), 0x34); }                                   // [[rules+B8h]+34h]
inline void assign(void* dst, void* src) { async_assign_string_005e5c50(dst, nullptr, src); }      // CExoString::operator=
// out = left + right into a fresh temporary (operator+ 005E5D10).
inline void concat(Str& out, void* left, void* right) {
    GuiConcat005e5d10(sp(left), nullptr, out.ss(), sp(right)); out.live = true;
}
// Inline record accessor (NULL for a negative byte offset or an out-of-range index; count re-read each time).
inline U8* record(void* item, U32 count_off, U32 data_off, I32 index, I32 off) {
    if (off < 0 || index >= li(item, count_off)) return nullptr;
    return static_cast<U8*>(lp(item, data_off)) + off;
}
// Unchecked byte read through a possibly-null accessor result (faults like the original).
inline U8 raw_byte(const U8* p, U32 off) {
    return *reinterpret_cast<const volatile U8*>(static_cast<std::uintptr_t>(ad(p) + off));
}
// UpgradeType -> upgradable type (jump tables 00554370/00554380 and 0055438C/0055439C: 0 -> 1, 1..3 -> 3, 4..7 -> 2,
// 8..9 -> 4; above 9 (unsigned ja) leaves the result unchanged).
inline U8 map_type(U32 value, U8 current) {
    static const U8 index[10] = {0, 1, 1, 1, 2, 2, 2, 2, 3, 3};                                   // byte table
    static const U8 result[4] = {1, 3, 2, 4};                                                      // 00554276/7E/7A/82
    if (value > 9) return current;
    return result[index[value]];
}
// One property list pass of GetUpgradableType (005541E4..00554290 / 00554296..00554350).
U8 scan_upgradable(void* item, void* table, U32 count_off, U32 data_off, U8 result) {
    I32 off = 0;
    for (I32 i = 0; i < li(item, count_off); ++i, off += 0x1C) {                                   // 005541E8 / 00554284..00554290
        if (raw_byte(record(item, count_off, data_off, i, off), 0x11) == 0xFF) continue;           // 005541F2..0055420A
        I32 value;                                                                                 // [esp+10h] (always written)
        {
            Str column(k_upgrade_type_00748db8);                                                   // 0055420C..00554215 [esp+14h]
            const U8 row = raw_byte(record(item, count_off, data_off, i, off), 0x11);              // 0055421A..00554234
            c2da_current_get_int_00413660(table, nullptr, row, column.p(), &value);                // 00554237..00554249
        }                                                                                          // 0055424E..0055425A
        result = map_type(static_cast<U32>(value), result);                                        // 0055425F..00554282
    }
    return result;
}
// The " +" / ", " composition of a type-5 property (0055F5CA..0055F82B / 0055F920..0055FB81):
// s = s + GUI(177h) + " +" + cost + ", " + GUI(7A99h) + " +" + cost; temporaries destroyed newest first.
void compose_type5(Str& s, U32 cost) {
    Str a(k_plus_00748f34);                                                                        // 0055F5CA..0055F5D3
    Str b(k_sep_00748dd4);                                                                         // 0055F5D8..0055F5E9
    Str c(k_plus_00748f34);                                                                        // 0055F5EE..0055F5FF
    Str n1(static_cast<I32>(cost));                                                                // 0055F604..0055F618
    Str g1; gui_string(g1, 0x7A99);                                                                // 0055F61F..0055F63D
    Str n2(static_cast<I32>(cost));                                                                // 0055F644..0055F658
    Str g2; gui_string(g2, 0x177);                                                                 // 0055F65F..0055F67A
    Str t1, t2, t3, t4, t5, t6, t7;
    concat(t1, s.p(), g2.p());                                                                     // 0055F67F..0055F691
    concat(t2, t1.p(), c.p());                                                                     // 0055F696..0055F6AA
    concat(t3, t2.p(), n2.p());                                                                    // 0055F6AF..0055F6C2
    concat(t4, t3.p(), b.p());                                                                     // 0055F6C7..0055F6DB
    concat(t5, t4.p(), g1.p());                                                                    // 0055F6E0..0055F6F3
    concat(t6, t5.p(), a.p());                                                                     // 0055F6F8..0055F70C
    concat(t7, t6.p(), n1.p());                                                                    // 0055F711..0055F720
    assign(s.p(), t7.p());                                                                         // 0055F725..0055F732
}   // 0055F737..0055F82B: ~t7 ~t6 ~t5 ~t4 ~t3 ~t2 ~t1 ~g2 ~n2 ~g1 ~n1 ~c ~b ~a
// Stack CExoArrayList<CExoString> of GetKeyedPropertyString ([esp+3Ch]): destroyed as delete[] (vector dtor + free).
struct StringList {
    U32 data = 0; I32 size = 0; I32 capacity = 0;
    StringList() = default; StringList(const StringList&) = delete; StringList& operator=(const StringList&) = delete;
    ~StringList() {
        if (data == 0) return;                                                                     // 0055FC5D..0055FC6B
        const U32 count = lw(pt(data - 4u), 0);                                                    // 0055FC6D
        w4gui::vector_destruct(pt(data), 8, count, &frame_LogStringDtor_005e5c20);                 // 0055FC73..0055FC7C eh_vector_destructor_iterator
        live_free(pt(data - 4u));                                                                  // 0055FC81..0055FC87
    }
};
static_assert(sizeof(StringList) == 12, "CExoArrayList<CExoString>");
// Pass by value: copy-construct into the 8-byte argument (005E5B00); the callee destroys it.
inline void list_add(void* list, void* string) {
    ExoVal value;
    WindowCopy005e5b00(value.w, nullptr, string);                                                  // CExoString(const&)
    claude_exo_string_list_add_004059a0(list, nullptr, value);                                     // 004059A0
}
// One property list pass of GetKeyedPropertyString (0055F54A..0055F8A4 / 0055F8AA..0055FBFA).
void keyed_pass(void* item, U32 count_off, U32 data_off, U8 key, StringList& list) {
    I32 off = 0;                                                                                   // [esp+10h]
    for (I32 i = 0; i < li(item, count_off); ++i, off += 0x1C) {                                   // [esp+14h]
        U8* p = record(item, count_off, data_off, i, off);                                         // 0055F570..0055F586
        if (p == nullptr) continue;                                                                // 0055F588..0055F58A
        if (lb(p, 0x11) != key) continue;                                                          // 0055F590..0055F59A
        const U16 type = lh(p, 0);                                                                 // 0055F5A0
        if (type == 5) {                                                                           // 0055F5A3..0055F5A7
            Str s; s.empty();                                                                      // 0055F5AD..0055F5B1 [esp+18h]
            if (lh(p, 6) != 0) compose_type5(s, lh(p, 6));                                         // 0055F5B6..0055F5C4
            list_add(&list, s.p());                                                                // 0055F838..0055F84F
        } else {                                                                                   // 0055F860 ~s
            kotor_fastcall_0055e180(item, nullptr, type, lh(p, 2), lh(p, 6), lb(p, 9), &list);     // 0055F867..0055F887
        }
    }
}
}  // namespace

extern "C" {
// 005541C0 CSWSItem::GetUpgradableType(C2DA* table) RET 4 -> BYTE: the type of the last property (active list, then
// passive list) whose upgrade slot is set; 0 when none.
w4::U8 __fastcall kotor_fastcall_005541c0(void* item, void*, void* table) {
    U8 result = 0;                                                                                 // 005541E4 bl
    result = scan_upgradable(item, table, 0x248, 0x250, result);                                   // 005541DD..00554290
    result = scan_upgradable(item, table, 0x24C, 0x254, result);                                   // 00554296..00554350
    return result;                                                                                 // 0055435D
}

// 0055E180 CSWSItem::AddProperty(type, subtype, cost value, param value, list) RET 14h: builds
// "<Name>:" [" <subtype name>"] [","] [" <cost name>"] [","] [" <param name>"] from itempropdef.2da and the subtype /
// cost / param tables and appends it to the list.
void __fastcall kotor_fastcall_0055e180(void*, void*, w4::U32 type_arg, w4::U32 subtype_arg, w4::U32 cost_arg,
                                        w4::U32 param_arg, void* list) {
    const U16 type = static_cast<U16>(type_arg);
    const U16 subtype = static_cast<U16>(subtype_arg);
    Str text; text.empty();                                                                        // 0055E1A1..0055E1A5 [esp+38h] state 0
    Str subtype_ref; subtype_ref.empty();                                                          // 0055E1AC..0055E1B4 [esp+40h] state 1
    I32 name = 0;                                                                                  // [esp+18h] 0055E1CE
    I32 subtype_name = -1;                                                                         // [esp+24h] 0055E1D2
    I32 has_param = -1;                                                                            // [esp+1Ch] 0055E1D6
    I32 cost_table;                                                                                // [esp+14h] (always written by GetINTEntry)
    I32 param_table;                                                                               // [esp+20h]
    I32 found_subtype;                                                                             // edi
    void* table = item_property_def();                                                             // 0055E1DF..0055E1EE
    I32 name_found;
    {
        Str column(k_name_00745eec);                                                               // 0055E1C1..0055E1DA
        name_found = c2da_current_get_int_00413660(table, nullptr, type, column.p(), &name);       // 0055E1F1..0055E203
    }                                                                                              // 0055E20E..0055E21A ~column
    if (name_found == 0) return;                                                                   // 0055E208..0055E225 -> 0055E227 ~subtype_ref, 0055E90D ~text
    {
        Str column(k_subtype_00748df8);                                                            // 0055E23E..0055E247
        found_subtype = c2da_current_get_string_00413270(item_property_def(), nullptr, type, column.p(), subtype_ref.p()); // 0055E24C..0055E275
    }
    {
        Str column(k_cost_00748de8);                                                               // 0055E280..0055E289
        c2da_current_get_int_00413660(item_property_def(), nullptr, type, column.p(), &cost_table); // 0055E28E..0055E2AE
    }
    {
        Str column(k_param_00748dd8);                                                              // 0055E2C0..0055E2C9
        has_param = c2da_current_get_int_00413660(item_property_def(), nullptr, type, column.p(), &param_table); // 0055E2CE..0055E2F9
    }
    I32 param_flag = has_param;                                                                    // esi
    {
        Str g; gui_string(g, static_cast<U32>(name));                                              // 0055E306..0055E318
        assign(text.p(), g.p());                                                                   // 0055E31D..0055E327
    }                                                                                              // 0055E32C..0055E334
    bool add = true;
    if (found_subtype == 0 && param_flag == 0 && cost_table == 0) goto append;                     // 0055E339..0055E347
    {
        Str colon(k_colon_0073e2a0);                                                               // 0055E34D..0055E356
        Str t; concat(t, text.p(), colon.p());                                                     // 0055E35B..0055E36E
        assign(text.p(), t.p());                                                                   // 0055E373..0055E37D
    }                                                                                              // 0055E382..0055E398
    if (found_subtype != 0) {                                                                      // 0055E39D..0055E39F
        void* raw = live_operator_new(0x54);                                                       // 0055E3A5..0055E3AC
        void* sub = nullptr;
        if (raw != nullptr) {                                                                      // 0055E3B5..0055E3BC
            auto guard = w4gui::on_throw([&]() { live_free(raw); });                               // EH state 9
            menu_resources::Ref ref;
            async_ref_construct_string_00406d60(&ref, nullptr, subtype_ref.p());                   // 0055E3BE..0055E3CE
            sub = c2da_current_construct_00413cc0(raw, nullptr, ref, 0);                           // 0055E3D3..0055E3D5
            guard.armed = false;
        }
        if (c2da_current_load_004143b0(sub, nullptr) == 0) { add = false; goto append; }           // 0055E3E0..0055E3ED (table leaked)
        const U16 row = subtype;                                                                   // 0055E409
        {
            Str column(k_name_00745eec);                                                           // 0055E3F3..0055E404
            c2da_current_get_int_00413660(sub, nullptr, row, column.p(), &subtype_name);           // 0055E40D..0055E41F
        }
        I32 value;                                                                                 // [esp+18h]
        bool cost_override;
        {
            Str column(k_cost_00748de8);                                                           // 0055E431..0055E43A
            const I32 r = c2da_current_get_int_00413660(sub, nullptr, row, column.p(), &value);    // 0055E43F..0055E451
            cost_override = r != 0 && value != 0;                                                  // 0055E456..0055E466 [esp+13h]
        }
        if (cost_override) cost_table = value;                                                     // 0055E478..0055E484
        {
            Str column(k_param_00748dd8);                                                          // 0055E488..0055E491
            const I32 r = c2da_current_get_int_00413660(sub, nullptr, row, column.p(), &value);    // 0055E496..0055E4B1
            if (r != 0) { has_param = 1; param_table = value; }                                    // 0055E4BC..0055E4C8 (after ~column)
        }
        if (sub != nullptr) vcall<void, U32>(sub, 0x00, 1);                                        // 0055E4CC..0055E4D5 delete
        if (type == 0x0B && (subtype == 0 || subtype == 1 || subtype == 2)) {                      // 0055E4D7..0055E4F4
            Str space(k_space_0073d5d8);                                                           // 0055E4FA..0055E503
            Str g; gui_string(g, 0x9698);                                                          // 0055E508..0055E520
            Str t1; concat(t1, text.p(), space.p());                                               // 0055E527..0055E53A
            Str t2; concat(t2, t1.p(), g.p());                                                     // 0055E53F..0055E54C
            assign(text.p(), t2.p());                                                              // 0055E551..0055E55B
        } else {                                                                                   // 0055E560..0055E621 ~t2 ~t1 ~g ~space
            Str space(k_space_0073d5d8);                                                           // 0055E58A..0055E593
            Str g; gui_string(g, static_cast<U32>(subtype_name));                                  // 0055E598..0055E5AF
            Str t1; concat(t1, text.p(), space.p());                                               // 0055E5B4..0055E5C9
            Str t2; concat(t2, t1.p(), g.p());                                                     // 0055E5CE..0055E5DB
            assign(text.p(), t2.p());                                                              // 0055E5E0..0055E5EA
        }
        if (has_param == 0 && cost_table == 0) goto append;                                        // 0055E626..0055E634
        {
            Str comma(k_comma_00748668);                                                           // 0055E63A..0055E643
            Str t; concat(t, text.p(), comma.p());                                                 // 0055E648..0055E65B
            assign(text.p(), t.p());                                                               // 0055E660..0055E66A
        }                                                                                          // 0055E66F..0055E685
        param_flag = has_param;                                                                    // 0055E68A
    }
    if (cost_table != 0) {                                                                         // 0055E68E..0055E694
        void* cost = kotor_fastcall_005c49a0(two_dim(), nullptr, static_cast<U32>(cost_table));    // 0055E69A..0055E6A7
        I32 cost_name;                                                                             // [esp+24h] (reused slot)
        {
            Str column(k_name_00745eec);                                                           // 0055E6AC..0055E6BF
            c2da_current_get_int_00413660(cost, nullptr, static_cast<U16>(cost_arg), column.p(), &subtype_name); // 0055E6C4..0055E6DA
            cost_name = subtype_name;
        }
        {
            Str space(k_space_0073d5d8);                                                           // 0055E6EC..0055E6F5
            Str g; gui_string(g, static_cast<U32>(cost_name));                                     // 0055E6FA..0055E712
            Str t1; concat(t1, text.p(), space.p());                                               // 0055E717..0055E72C
            Str t2; concat(t2, t1.p(), g.p());                                                     // 0055E731..0055E73E
            assign(text.p(), t2.p());                                                              // 0055E743..0055E74D
        }                                                                                          // 0055E752..0055E784 ~t2 ~t1 ~g ~space
        if (param_flag == 0) goto append;                                                          // 0055E789..0055E78B
        {
            Str comma(k_comma_00748668);                                                           // 0055E791..0055E79A
            Str t; concat(t, text.p(), comma.p());                                                 // 0055E79F..0055E7B2
            assign(text.p(), t.p());                                                               // 0055E7B7..0055E7C1
        }                                                                                          // 0055E7C6..0055E7DC
    }
    if (param_flag == 0) goto append;                                                              // 0055E7E1..0055E7E3
    {
        void* param = kotor_fastcall_005c4c00(two_dim(), nullptr, static_cast<U32>(param_table));  // 0055E7E9..0055E7FA
        I32 param_name;                                                                            // [esp+1Ch]
        {
            Str column(k_name_00745eec);                                                           // 0055E7FF..0055E812
            c2da_current_get_int_00413660(param, nullptr, static_cast<U8>(param_arg), column.p(), &param_name); // 0055E817..0055E82D
        }
        Str space(k_space_0073d5d8);                                                               // 0055E83F..0055E848 [esp+28h]
        Str g; gui_string(g, static_cast<U32>(param_name));                                        // 0055E84D..0055E865
        Str t1; concat(t1, text.p(), space.p());                                                   // 0055E86A..0055E87F
        Str t2; concat(t2, t1.p(), g.p());                                                         // 0055E884..0055E891
        assign(text.p(), t2.p());                                                                  // 0055E896..0055E8A0
    }                                                                                              // 0055E8A5..0055E8D7 ~t2 ~t1 ~g ~space
append:
    if (add) list_add(list, text.p());                                                             // 0055E8DC..0055E8F2
}                                                                                                  // 0055E8F7..0055E911 ~subtype_ref ~text

// 005562D0 CSWSItem::GetSortedPropertyStrings(list, out) RET 8: out = GUI(609h) + ":\n", then repeatedly appends the
// smallest remaining string + "\n" and removes it from the list (shift down by operator=, size - 1).
void __fastcall kotor_fastcall_005562d0(void*, void*, void* list, void* out) {
    if (li(list, 4) == 0) return;                                                                  // 005562F0..005562F9
    {
        Str head(k_colon_nl_00748e94);                                                             // 005562FF..00556308 [esp+10h]
        Str g; gui_string(g, 0x609);                                                               // 0055630D..00556327
        Str t; concat(t, g.p(), head.p());                                                         // 0055632C..0055633D
        assign(out, t.p());                                                                        // 00556342..0055634B
    }                                                                                              // 00556350..00556378 ~t ~g ~head
    while (li(list, 4) != 0) {                                                                     // 0055637D..00556382 / 0055646A..0055646F
        I32 best = 0;                                                                              // edi
        U32 best_off = 0;                                                                          // [esp+10h]
        for (I32 k = 1; k < li(list, 4); ++k) {                                                    // 00556390..005563CA
            U8* data = static_cast<U8*>(lp(list, 0));
            if (directory_list::Less005e5410(sp(data + k * 8), nullptr, sp(data + best_off)) != 0) { // 005563A2..005563B5
                best = k; best_off = static_cast<U32>(k) * 8u;                                     // 005563B7..005563C0
            }
        }
        {
            Str nl(k_nl_0073d5cc);                                                                 // 005563CC..005563D5 [esp+18h]
            Str t1; concat(t1, out, static_cast<U8*>(lp(list, 0)) + best * 8);                     // 005563DA..005563F2
            Str t2; concat(t2, t1.p(), nl.p());                                                    // 005563F7..00556408
            assign(out, t2.p());                                                                   // 0055640D..00556415
        }                                                                                          // 0055641A..00556442 ~t2 ~t1 ~nl
        sw(list, 4, lw(list, 4) - 1u);                                                             // 00556447..0055644F
        for (I32 k = best; k < li(list, 4); ++k) {                                                 // 0055644D..00556468
            U8* data = static_cast<U8*>(lp(list, 0));
            assign(data + k * 8, data + k * 8 + 8);                                                // 00556454..0055645D
        }
    }
}

// 0055F510 CSWSItem::GetKeyedPropertyString(CExoString* out, BYTE key) RET 8 -> out: the sorted description of every
// property (active, then passive) whose upgrade slot byte equals key.
void* __fastcall kotor_fastcall_0055f510(void* item, void*, void* out, w4::U32 key_arg) {
    const U8 key = static_cast<U8>(key_arg);                                                       // [esp+C4h]
    StringList list;                                                                               // [esp+3Ch] 0055F53E..0055F546 state 1
    keyed_pass(item, 0x248, 0x250, key, list);                                                     // 0055F54A..0055F8A4
    keyed_pass(item, 0x24C, 0x254, key, list);                                                     // 0055F8AA..0055FBFA
    Str sorted; sorted.empty();                                                                    // 0055FC00..0055FC07 [esp+84h] state 20h
    kotor_fastcall_005562d0(item, nullptr, &list, sorted.p());                                     // 0055FC0C..0055FC23
    WindowCopy005e5b00(out, nullptr, sorted.p());                                                  // 0055FC28..0055FC39 CExoString(const&)
    return out;                                                                                    // 0055FC3E..0055FC58 ~sorted; 0055FC5D..0055FC87 list delete[]
}
}
