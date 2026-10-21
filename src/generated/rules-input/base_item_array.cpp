// lane-rules fork B: CSWBaseItem::CSWBaseItem 005B2EF0, CSWBaseItem::~CSWBaseItem 005B2FC0,
// CSWBaseItemArray::Load 005B31D0, CSWBaseItemArray::~CSWBaseItemArray 005B4730
// (swkotor.exe GOG K1 sha256 9c10e045..., base 0x400000). D1 form, C++ linkage.
// CSWBaseItemArray view: +0 count (I32), +4 element pointer (0xC8-byte CSWBaseItem
// elements, count-prefixed eh-vector block). Only cited offsets are touched.
#include "base_item_array.h"
#include <cstring>
#include <new>

namespace kotor_rules {
namespace {
constexpr Word kItemSize = 0xC8;                       // 005B324C imul 0C8h / 005B3282 push 0C8h / 005B4613
inline String* string_at(void* base, unsigned off) noexcept {
    return std::launder(reinterpret_cast<String*>(bytes(base) + off));
}
inline Ref* ref_at(void* base, unsigned off) noexcept {
    return std::launder(reinterpret_cast<Ref*>(bytes(base) + off));
}
}

// ---- 005B2EF0 CSWBaseItem::CSWBaseItem ----
// EH states 0..2 (handler 0072232A) cover the four CExoString members; their
// constructor 005B3190 is two plain stores (005B3190..005B319F) and cannot throw,
// so the unwind paths are unreachable and no try is emitted.
void* __fastcall kotor_fastcall_005b2ef0(void* self, void*) {
    c2da_default_ref_00405ed0(ref_at(self, 0x4C), nullptr);   // 005B2F0A/005B2F11 CResRef()
    c2da_default_ref_00405ed0(ref_at(self, 0x5C), nullptr);   // 005B2F16/005B2F19
    ::new (bytes(self) + 0x94) String();                       // 005B2F1E/005B2F24 005B3190
    ::new (bytes(self) + 0x9C) String();                       // 005B2F2B/005B2F35 (state 0)
    ::new (bytes(self) + 0xA4) String();                       // 005B2F3A/005B2F45 (state 1)
    ::new (bytes(self) + 0xBC) String();                       // 005B2F4A/005B2F55 (state 2)
    wr32(self, 0x04, 0);                                       // 005B2F5A
    wr8(self, 0x18, 0);                                        // 005B2F5D
    wr32(self, 0x00, 0xFFFFFFFFu);                             // 005B2F60
    wr32(self, 0x3C, 0); wr32(self, 0x40, 0); wr32(self, 0x44, 0); // 005B2F68..005B2F70
    wr32(self, 0x48, 0);                                       // 005B2F77
    wr8(self, 0x78, 0);                                        // 005B2F7A
    wr32(self, 0x7C, 0);                                       // 005B2F7D
    wr8(self, 0x80, 0);                                        // 005B2F80
    wr8(self, 0x88, 0);                                        // 005B2F86
    wr16(self, 0xB0, 0);                                       // 005B2F8C
    wr16(self, 0xB2, 0);                                       // 005B2F93
    wr32(self, 0xB8, 0);                                       // 005B2F9A
    wr8(self, 0xAF, 1);                                        // 005B2FA0
    wr8(self, 0xAD, 0xFF);                                     // 005B2FA7
    return self;                                               // 005B2FAE mov eax,esi
}

// ---- 005B2FC0 CSWBaseItem::~CSWBaseItem ----
void __fastcall kotor_fastcall_005b2fc0(void* self, void*) {
    if (void* table = ptr_at<void>(self, 0x7C))                // 005B2FDD..005B2FEA (not cleared afterwards)
        live_free(table);                                      // 005B2FEC/005B2FED 006FA390
    string_at(self, 0xBC)->~StartupString();                   // 005B2FF5/005B3000 005E5C20
    string_at(self, 0xA4)->~StartupString();                   // 005B3005/005B3010
    string_at(self, 0x9C)->~StartupString();                   // 005B3015/005B3020
    string_at(self, 0x94)->~StartupString();                   // 005B3025/005B3033
}
// D24: same body, typed for c2da_destroy_array_006fa852's destroy parameter.
void __fastcall kotor_fastcall_005b2fc0_array_destroy(kotor_reconstruction::StartupString* self, void*) {
    kotor_fastcall_005b2fc0(static_cast<void*>(self), nullptr);
}

// ---- 005B4730 CSWBaseItemArray::~CSWBaseItemArray ----
void __fastcall kotor_fastcall_005b4730(void* self, void*) {
    unsigned char* const first = ptr_at<unsigned char>(self, 0x04);           // 005B4733
    if (!first) return;                                                       // 005B4736/005B4738
    // delete[] of the count-prefixed block: count at [first-4] (005B473A/005B473E).
    c2da_destroy_array_006fa852(first, kItemSize, rd32(first - 4, 0),          // 005B4741..005B474D push 5B2FC0/count/0C8h/first
                                &kotor_fastcall_005b2fc0_array_destroy);
    live_free(first - 4);                                                     // 005B4752/005B4753 006FA390
    wr32(self, 0x04, 0);                                                      // 005B475B
    wr32(self, 0x00, 0);                                                      // 005B4762
}

// ---- 005B31D0 CSWBaseItemArray::Load ----
// EH handler 00722610: state 0 = C2DA allocation, 1 = item-array allocation,
// 2 = local CExoString [esp+20h], 3..39h = per-call column temporaries,
// 33h = row-local CExoString [esp+30h]. Modelled by allocate_construct, a try
// around the vector construction, and RAII String objects.
void __fastcall kotor_fastcall_005b31d0(void* self, void*) {
    // 005B31F7..005B3231: new C2DA(CResRef("baseitems"), 0) (state 0); EDI = result or 0.
    Table* const table = new_c2da(kotor_rules_str_0074b294);                  // 005B321E push 74B294h
    c2da_current_load_004143b0(table, nullptr);                               // 005B3242 (also when allocation failed)
    const Word rows = rd32(table, 0x24);                                      // 005B3247 [edi+24h]
    wr32(self, 0x00, rows);                                                   // 005B3256
    unsigned char* items = nullptr;                                           // ebx = 0 (005B3207)
    if (void* block = live_operator_new(rows * kItemSize + 4)) {              // 005B324A..005B3259 (DWORD wrap)
        items = static_cast<unsigned char*>(block) + 4;                       // 005B327F
        wr32(block, 0, rows);                                                 // 005B3288 [eax]=count
        try {                                                                 // state 1
            crt_eh_vector_constructor_006fb55f(items, kItemSize, static_cast<std::int32_t>(rows), // 005B3274..005B328A
                                               &kotor_fastcall_005b2ef0, &kotor_fastcall_005b2fc0);
        } catch (...) { live_free(block); throw; }
    }
    put_ptr(self, 0x04, items);                                               // 005B329E
    String value;                                                             // 005B328F/005B32A1 005B3190 (state 2)
    std::int32_t number;   // [esp+10h]: always written by 00413660 before each read (default path 00413796)
    float real;            // [esp+2Ch]: always written by 00413430 before each read (004134F1 / 00413478 / 004134C7 / 004134D3)
    Word offset = 0;                                                          // 005B32BE ebx
    for (std::int32_t row = 0; row < static_cast<std::int32_t>(rd32(self, 0x00)); ++row, offset += kItemSize) { // 005B32A6..005B32B8, 005B460F..005B461B
        // [ebp+4]+ebx is re-read before every store, as the original does.
        auto item = [self, offset]() noexcept { return ptr_at<unsigned char>(self, 0x04) + offset; };
        int_entry(table, row, kotor_rules_str_00745eec, &number);  wr32(item(), 0x00, static_cast<Word>(number)); // 005B32C0..005B330B
        int_entry(table, row, kotor_rules_str_0074b284, &number);  wr32(item(), 0x04, static_cast<Word>(number)); // 005B330D..005B334F
        int_entry(table, row, kotor_rules_str_0074b274, &number);  wr32(item(), 0x6C, static_cast<Word>(number)); // 005B3352..005B339D
        int_entry(table, row, kotor_rules_str_0074b268, &number);  wr8(item(), 0x18, static_cast<unsigned char>(number)); // 005B33A0..005B33E2
        string_entry(table, row, kotor_rules_str_0074b25c, &value);                                   // 005B33E5..005B3422
        // strncpy(item+3Ch, CStr(value), 0Bh): 006FAB80 is the MSVC CRT strncpy (copy up to 11
        // bytes, stop at NUL, NUL-pad the rest, no forced terminator); UCRT strncpy has the
        // identical ISO C contract for these non-overlapping inputs (D35 equivalence).
        std::strncpy(reinterpret_cast<char*>(item() + 0x3C), WindowCStr005e5670(&value, nullptr), 0x0B); // 005B3427..005B343C
        int_entry(table, row, kotor_rules_str_0074b24c, &number);  wr32(item(), 0x74, static_cast<unsigned char>(number)); // 005B3444..005B3487 movzx
        int_entry(table, row, kotor_rules_str_0074b240, &number);  wr32(item(), 0x48, static_cast<Word>(number)); // 005B348A..005B34D5
        {
            string_entry(table, row, kotor_rules_str_0074b230, &value);                               // 005B34D8..005B350C
            Ref built, discarded;                                                                      // [esp+218h] / [esp+1F8h]
            Ref* const source = async_ref_construct_string_00406d60(&built, nullptr, &value);          // 005B3511..005B351D
            c2da_copy_00406120(ref_at(item(), 0x5C), nullptr, &discarded, source);                     // 005B3522..005B3533
        }
        {
            string_entry(table, row, kotor_rules_str_0074b224, &value);                               // 005B3538..005B3575
            Ref built, discarded;                                                                      // [esp+208h] / [esp+228h]
            Ref* const source = async_ref_construct_string_00406d60(&built, nullptr, &value);          // 005B357A..005B3586
            c2da_copy_00406120(ref_at(item(), 0x4C), nullptr, &discarded, source);                     // 005B358B..005B359C
        }
        int_entry(table, row, kotor_rules_str_0074b218, &number);  wr32(item(), 0x70, static_cast<Word>(number)); // 005B35A1..005B35EC
        int_entry(table, row, kotor_rules_str_00748a60, &number);  wr8(item(), 0x08, static_cast<unsigned char>(number)); // 005B35EF..005B363A
        int_entry(table, row, kotor_rules_str_0074b20c, &number);  wr8(item(), 0x09, static_cast<unsigned char>(number)); // 005B363D..005B3688
        int_entry(table, row, kotor_rules_str_00746a20, &number);  wr32(item(), 0x0C, static_cast<Word>(number)); // 005B368B..005B36D6
        int_entry(table, row, kotor_rules_str_0074b200, &number);  wr8(item(), 0x1B, static_cast<unsigned char>(number)); // 005B36D9..005B3724
        if (int_entry(table, row, kotor_rules_str_0074b1f0, &number))                                 // 005B3727..005B3776
            wr8(item(), 0x1A, static_cast<unsigned char>(number));                                    // 005B3778..005B377E
        else
            wr8(item(), 0x1A, 0);                                                                     // 005B3783/005B3785
        float_entry(table, row, kotor_rules_str_0074b1e0, &real);  wrf(item(), 0x34, real);  // 005B3789..005B37D4 (dword move of the stored float)
        float_entry(table, row, kotor_rules_str_0074b1d0, &real);  wrf(item(), 0x38, real);  // 005B37D7..005B3822
        int_entry(table, row, kotor_rules_str_0074b1c4, &number);  wr32(item(), 0x10, static_cast<Word>(number)); // 005B3825..005B3870
        int_entry(table, row, kotor_rules_str_0074b1b8, &number);  wr32(item(), 0x14, static_cast<Word>(number)); // 005B3873..005B38BE
        int_entry(table, row, kotor_rules_str_00747038, &number);  wr8(item(), 0x1C, static_cast<unsigned char>(number)); // 005B38C1..005B390C
        int_entry(table, row, kotor_rules_str_0074b1ac, &number);  wr8(item(), 0x1D, static_cast<unsigned char>(number)); // 005B390F..005B395A
        int_entry(table, row, kotor_rules_str_0074b1a0, &number);  wr8(item(), 0x1E, static_cast<unsigned char>(number)); // 005B395D..005B39A8
        int_entry(table, row, kotor_rules_str_0074b194, &number);  wr8(item(), 0x1F, static_cast<unsigned char>(number)); // 005B39AB..005B39F6
        float_entry(table, row, kotor_rules_str_0074b188, &real);  wrf(item(), 0x20, real);  // 005B39F9..005B3A44
        int_entry(table, row, kotor_rules_str_0074b17c, &number);  wr16(item(), 0x24, static_cast<std::uint16_t>(number)); // 005B3A47..005B3A93
        wr32(item(), 0xC4, 0x3F800000u);                          // 005B3A97..005B3AA5 (1.0f bits, before the next column string)
        int_entry(table, row, kotor_rules_str_007488c0, &number);  wr32(item(), 0x28, static_cast<Word>(number)); // 005B3A9C..005B3AE8
        if (int_entry(table, row, kotor_rules_str_0074b16c, &number))                                 // 005B3AEB..005B3B37
            wr8(item(), 0x78, static_cast<unsigned char>(number));                                    // 005B3B39..005B3B42
        int_entry(table, row, kotor_rules_str_0074b160, &number);  wr8(item(), 0x2D, static_cast<unsigned char>(number)); // 005B3B45..005B3B90
        int_entry(table, row, kotor_rules_str_0074b154, &number);  wr8(item(), 0x2C, static_cast<unsigned char>(number)); // 005B3B93..005B3BDE
        int_entry(table, row, kotor_rules_str_0074b148, &number);  wr8(item(), 0x2E, static_cast<unsigned char>(number)); // 005B3BE1..005B3C2C
        int_entry(table, row, kotor_rules_str_00748b30, &number);  wr8(item(), 0x8D, static_cast<unsigned char>(number)); // 005B3C2F..005B3C71
        int_entry(table, row, kotor_rules_str_0074b138, &number);  wr8(item(), 0x19, static_cast<unsigned char>(number)); // 005B3C77..005B3CB9
        if (!int_entry(table, row, kotor_rules_str_0074b128, &number))                                // 005B3CBC..005B3D05 (neg/sbb/inc)
            number = -1;                                                                              // 005B3D07 [esp+10h]=-1
        wr32(item(), 0x84, static_cast<Word>(number));                                                // 005B3D0F..005B3D18
        int_entry(table, row, kotor_rules_str_0074b11c, &number);  wr8(item(), 0x2F, static_cast<unsigned char>(number)); // 005B3D1E..005B3D60
        int_entry(table, row, kotor_rules_str_0074b10c, &number);  wr8(item(), 0x30, static_cast<unsigned char>(number)); // 005B3D63..005B3DAE
        int_entry(table, row, kotor_rules_str_0074b0fc, &number);  wr8(item(), 0x88, static_cast<unsigned char>(number)); // 005B3DB1..005B3DFC
        int_entry(table, row, kotor_rules_str_0074b0f0, &number);  wr8(item(), 0x89, static_cast<unsigned char>(number)); // 005B3E02..005B3E4D
        int_entry(table, row, kotor_rules_str_0074b0e8, &number);  wr8(item(), 0x8A, static_cast<unsigned char>(number)); // 005B3E53..005B3E9E
        if (int_entry(table, row, kotor_rules_str_0074b0dc, &number))                                 // 005B3EA4..005B3EF3
            wr8(item(), 0x8B, static_cast<unsigned char>(number));                                    // 005B3EF5..005B3EFB
        else
            wr8(item(), 0x8B, 0xFF);                                                                  // 005B3F03/005B3F05
        int_entry(table, row, kotor_rules_str_0074b0cc, &number);  wr8(item(), 0x8C, static_cast<unsigned char>(number)); // 005B3F0C..005B3F57
        int_entry(table, row, kotor_rules_str_0074b0bc, &number);  wr8(item(), 0x8E, static_cast<unsigned char>(number)); // 005B3F5D..005B3FA8
        int_entry(table, row, kotor_rules_str_0074b0b0, &number);  wr32(item(), 0x90, static_cast<Word>(number)); // 005B3FAE..005B3FF9
        string_entry(table, row, kotor_rules_str_0074b0a4, &value);  string_at(item(), 0x94)->Assign005e5c50(&value); // 005B3FFF..005B4051
        string_entry(table, row, kotor_rules_str_0074b094, &value);  string_at(item(), 0xA4)->Assign005e5c50(&value); // 005B4056..005B40A8
        string_entry(table, row, kotor_rules_str_0074b088, &value);  string_at(item(), 0x9C)->Assign005e5c50(&value); // 005B40AD..005B40FF
        int_entry(table, row, kotor_rules_str_0074b07c, &number);  wr8(item(), 0xAC, static_cast<unsigned char>(number)); // 005B4104..005B414F
        if (!int_entry(table, row, kotor_rules_str_0074b06c, &number))                                // 005B4155..005B41A7
            number = 0;                                                                               // 005B41A9
        wr8(item(), 0xB4, static_cast<unsigned char>(number));                                        // 005B41B1..005B41BA
        string_entry(table, row, kotor_rules_str_0074b064, &value);                                   // 005B41C0..005B41FD
        wr8(item(), 0xAF, 1);                                                                         // 005B4202..005B4213
        {
            // UpperCase into a hidden-return temporary [esp+1F0h] that is destroyed at once;
            // the result is not used (005B421A..005B4226).
            alignas(String) unsigned char upper_storage[sizeof(String)];
            String* const upper = alias_add::Upper005e6360(&value, nullptr, reinterpret_cast<String*>(upper_storage)); // 005B420E..005B421A
            upper->~StartupString();                                                                  // 005B421F/005B4226
        }
        if (value.BorrowedLength005e5790() == 1 &&                                                    // 005B422B..005B4237
            directory_list::At005e5600(&value, nullptr, 0) >= 0x41 &&                                 // 005B4239..005B4246 (signed char jl)
            directory_list::At005e5600(&value, nullptr, 0) <= 0x4A) {                                 // 005B4248..005B4255 (jg)
            const char c = directory_list::At005e5600(&value, nullptr, 0);                            // 005B4257..005B425D
            wr8(item(), 0xAF, static_cast<unsigned char>(c - 0x40));                                  // 005B4262..005B4269
        }
        {
            String name;                                                                              // 005B426F/005B4273 (state 33h)
            unsigned char found = 0;                                                                  // 005B4280 [esp+17h]
            for (std::int32_t i = 0; i < 5; ++i) {                                                    // 005B4285..005B430A
                kotor_reconstruction::StartupFormat005e5680(&name, kotor_rules_str_0074b058, i);      // 005B4290..005B42A4
                if (int_entry(table, row, WindowCStr005e5670(&name, nullptr), &number))               // 005B42A7..005B42F8
                    ++found;                                                                          // 005B42FA
            }
            if (found > 0) {                                                                          // 005B430C..005B4312 (jbe)
                unsigned char* const target = item();                                                 // 005B4318..005B431D [esp+18h]
                if (void* old = ptr_at<void>(target, 0x7C)) {                                         // 005B4321..005B432A
                    live_free(old);                                                                   // 005B432C/005B432D
                    wr32(target, 0x7C, 0);                                                            // 005B433D
                }
                void* const list = live_operator_new(static_cast<Word>(found) * 2u);                  // 005B4344..005B434A
                put_ptr(target, 0x7C, list);                                                          // 005B4358
                wr8(target, 0x80, list ? found : 0);                                                  // 005B435B..005B4365
                unsigned char stored = 0;                                                             // 005B436B
                for (std::int32_t i = 0; i < 5; ++i) {                                                // 005B4370..005B441D
                    kotor_reconstruction::StartupFormat005e5680(&name, kotor_rules_str_0074b058, i);  // 005B4380..005B4394
                    if (int_entry(table, row, WindowCStr005e5670(&name, nullptr), &number)) {         // 005B4397..005B43E8
                        unsigned char* const entry = item();                                          // 005B43EA..005B43F8
                        if (stored < rd8(entry, 0x80))                                                // 005B43ED..005B43FC (jae)
                            wr16(ptr_at<unsigned char>(entry, 0x7C), static_cast<unsigned>(stored) * 2u, // 005B43FE..005B4409
                                 static_cast<std::uint16_t>(number));
                        ++stored;                                                                     // 005B440D (also when out of range)
                    }
                }
            }
            if (int_entry(table, row, kotor_rules_str_0074b04c, &number))                             // 005B4423..005B446F
                wr16(item(), 0xB0, static_cast<std::uint16_t>(number));                               // 005B4471..005B447B
            if (int_entry(table, row, kotor_rules_str_0074b044, &number))                             // 005B4482..005B44CE
                wr16(item(), 0xB2, static_cast<std::uint16_t>(number));                               // 005B44D0..005B44DA
            if (int_entry(table, row, kotor_rules_str_0074b038, &number))                             // 005B44E1..005B452D
                wr32(item(), 0xB8, static_cast<Word>(number));                                        // 005B452F..005B4538
            if (string_entry(table, row, kotor_rules_str_0074b02c, &value))                           // 005B453E..005B458A
                string_at(item(), 0xBC)->Assign005e5c50(&value);                                      // 005B458C..005B459C
            if (int_entry(table, row, kotor_rules_str_0074b020, &number))                             // 005B45A1..005B45ED
                wr8(item(), 0xAD, static_cast<unsigned char>(number));                                // 005B45EF..005B45F8
        }                                                                                             // 005B45FE..005B460A ~name (state 2)
    }
    delete_c2da(table);                    // 005B4621..005B4627 call [vtbl+0](1); no null test in the original
}                                          // 005B4629..005B4638 ~value (state -1)
}
