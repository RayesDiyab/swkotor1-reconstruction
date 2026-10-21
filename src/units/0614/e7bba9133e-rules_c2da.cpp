// lane-prepare: faithful bodies for C2DA::GetINTEntry 00413660 and
// CSWRules::CSWRules 00552C50 (swkotor.exe 9c10e045..., base 0x400000).
// Receivers are minimum addressed views of the caller's allocation; only the
// cited offsets are read/written. Table cell strings are the canonical 8-byte
// CExoString objects (kotor_reconstruction::StartupString layout; existing M41
// owner methods BorrowedEquals 005E5290, BorrowedLength 005E5790,
// WindowCStr 005E5670, kotor_menu20::String::as_int 005E5630 are reused
// through the same object-view convention as service-adapters/accepted.cpp).
#include "b09a95a028-rules_c2da.h"
#include "d6db6bea87-prepare_image_data.h"   // D13 generated .rdata strings
#include "c7e191db10-native_strings.h"    // WindowCStr005e5670
#include "66a5366142-menu_targets.h"      // kotor_menu20::String::as_int_005e5630 (CExoString::AsINT)
#include "7b8c81fcff-allocator_live.h"    // live_operator_new / live_free = 006FA7E6 / 006FA390
#include <cstring>
#include <cstdint>
#include <new>

namespace {
using kotor_rules_c2da::Word;
using kotor_reconstruction::StartupString;
inline Word rd32(const void* base, unsigned off) noexcept {
    Word v; std::memcpy(&v, static_cast<const unsigned char*>(base) + off, 4); return v;
}
inline void wr32(void* base, unsigned off, Word v) noexcept { std::memcpy(static_cast<unsigned char*>(base) + off, &v, 4); }
inline void wr16(void* base, unsigned off, std::uint16_t v) noexcept { std::memcpy(static_cast<unsigned char*>(base) + off, &v, 2); }
inline void wr8(void* base, unsigned off, unsigned char v) noexcept { static_cast<unsigned char*>(base)[off] = v; }
inline unsigned char* at(Word address) noexcept { return reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(address)); }
inline std::int32_t as_int(void* string) { return reinterpret_cast<kotor_menu20::String*>(string)->as_int_005e5630(); }
// [[this+38h]+row*4] + column*8 : the cell CExoString, re-read at every use
// exactly as 004136D4/00413703/00413715/00413727/0041373A/00413751/00413778 do.
inline StartupString* cell(void* table, int row, int column) noexcept {
    return reinterpret_cast<StartupString*>(at(rd32(at(rd32(table, 0x38)), static_cast<unsigned>(row) * 4u)) +
                                            static_cast<unsigned>(column) * 8u);
}
}

namespace kotor_rules_c2da {
int __fastcall ClientGetIntEntry00413660(void* table, void*, int row, client_services::String* column, int* value) {
    // Same 8-byte CExoString object (client_services::String view of a StartupString).
    return c2da_current_get_int_00413660(static_cast<c2da_current::Table*>(table), nullptr, row,
                                         std::launder(reinterpret_cast<StartupString*>(column)), value);
}
}

namespace {
// Allocation + constructor with the original EH-funclet behaviour: when the
// constructor unwinds, the 006FA7E6 block is released through 006FA390.
template<class Ctor> void* allocate_construct(Word size, Ctor ctor) {
    void* p = live_operator_new(size);
    if (!p) return nullptr;                       // cmp eax,ebx / je -> xor
    try { return ctor(p); }
    catch (...) { live_free(p); throw; }
}
menu_resources::Ref resref(const char* name) {
    menu_resources::Ref r;                        // stack CResRef slot (sub esp,10h; mov ecx,esp)
    r.construct_00406d80(name);                   // 00406D80
    return r;
}
void* new_c2da(const char* name) {                // states 0 / 8: allocation 54h + C2DA(CResRef, 0)
    return allocate_construct(0x54, [name](void* p) {
        return static_cast<void*>(c2da_current_construct_00413cc0(p, nullptr, resref(name), 0)); // lane-startup-misc donor
    });
}
int float_entry(void* table, int row, const char* column, float* value) {
    StartupString name(column);                   // 005E5A90; unwind state 1..4 destroys it
    return c2da_current_get_float_00413430(static_cast<c2da_current::Table*>(table), nullptr, row, &name, value); // lane-startup-misc // 00413430 RET 0Ch; then 005E5C20
}
}

namespace kotor_original {
void* __fastcall kotor_fastcall_00552c50(void* self, void*) {
    auto* b = static_cast<unsigned char*>(self);
    wr32(b, 0x00, static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(&kotor_vtable_00748bb4))); // 00552C78 CSWRules vtable (lane-rules typed table; slot0 = 00553020)
    wr32(b, 0x8C, 0); wr16(b, 0xA4, 0);            // 00552C7E/00552C84
    wr32(b, 0x90, 0); wr32(b, 0xA0, 0); wr32(b, 0x34, 0); // 00552C8B..00552C97
    std::memset(b + 0x38, 0, 0x15 * 4);            // 00552C9A..00552CA2 rep stosd 15h
    wr8(b, 0xA9, 0); wr32(b, 0xAC, 0); wr8(b, 0xAA, 0);    // 00552CA6..00552CB2
    wr32(b, 0xB0, 0); wr8(b, 0xAB, 0); wr32(b, 0xB4, 0);   // 00552CB8..00552CC4
    wr32(b, 0x94, 0); wr32(b, 0x98, 0); wr32(b, 0x9C, 0);  // 00552CCA..00552CD6

    void* ranges = new_c2da(kotor_rdata_str_00748cb4);             // 00552CA4/00552CDC, 00552CFC 00748CB4 "Ranges", 00552D08
    c2da_current_load_004143b0(static_cast<c2da_current::Table*>(ranges), nullptr);      // 00552D1D (called even when allocation failed)
    for (int row = 0; row < 6; ++row) {            // 00552D30..00552E7B
        if (row != 5) {
            wr32(b, 0x04 + row * 4u, 0);           // 00552D42
            float v;                               // [esp+14h]
            if (float_entry(ranges, row, kotor_rdata_str_00746f28, &v))           // 00552D33 00746F28 / 00552D7A
                std::memcpy(b + 0x04 + row * 4u, &v, 4);                 // 00552D80/00552D84
            wr32(b, 0x1C + row * 4u, 0);           // 00552D91
            if (float_entry(ranges, row, kotor_rdata_str_00746f38, &v))         // 00552D88 00746F38 / 00552DC9
                std::memcpy(b + 0x1C + row * 4u, &v, 4);                 // 00552DD3/00552DD7
        } else {
            wr32(b, 0x18, 0);                      // 00552DE4
            float v;                               // [esp+14h]
            if (float_entry(ranges, 0x13, kotor_rdata_str_00746f28, &v))          // 00552D33 00746F28, row 13h / 00552E1C
                std::memcpy(b + 0x18, &v, 4);                            // 00552E22/00552E26 (dword move)
            wr32(b, 0x30, 0);                      // 00552E32
            if (float_entry(ranges, 0x13, kotor_rdata_str_00746f38, &v))        // 00552E29 00746F38 / 00552E6A
                std::memcpy(b + 0x30, &v, 4);                            // 00552E70/00552E74
        }
    }
    if (ranges) { auto* t = static_cast<c2da_current::Table*>(ranges); static_cast<const c2da_current::TableSlot0*>(t->helper.methods)->deleting(t, nullptr, 1); } // typed slot0 (D41)            // 00552E81..00552E8C [vtable]+0 (deleting dtor)

    void* arrays = allocate_construct(0x12C, [](void* p) { return kotor_fastcall_005bfb60(p, nullptr); }); // 00552E8E/00552EAD
    wr32(b, 0xB8, static_cast<Word>(reinterpret_cast<std::uintptr_t>(arrays)));                         // 00552EBF
    kotor_rules::kotor_fastcall_005c4c20(arrays, nullptr);                                                             // 00552EC5

    void* spells = allocate_construct(8, [](void* p) {                  // 00552ECA/00552ECC
        return static_cast<void*>(::new(p) StartupString()); });       // 00552EE6 005B3190 (8-byte {0,0} ctor)
    wr32(b, 0x8C, static_cast<Word>(reinterpret_cast<std::uintptr_t>(spells)));  // 00552EF5
    kotor_rules::kotor_fastcall_0059ba20(spells, nullptr);                           // 00552EFB CSWSpellArray::Load
    kotor_rules::kotor_fastcall_005515e0(self, nullptr);                             // 00552F02 LoadFeatInfo

    void* items = allocate_construct(8, [](void* p) {                   // 00552F07/00552F09
        return static_cast<void*>(::new(p) StartupString()); });       // 00552F23 005B3190
    wr32(b, 0x34, static_cast<Word>(reinterpret_cast<std::uintptr_t>(items)));   // 00552F32
    kotor_rules::kotor_fastcall_005b31d0(items, nullptr);                            // 00552F35 CSWBaseItemArray::Load

    void* xp = new_c2da(kotor_rdata_str_00748ca8);               // 00552F3A/00552F3C, 00552F60 00748CA8 "EXPTABLE", 00552F6C
    c2da_current_load_004143b0(static_cast<c2da_current::Table*>(xp), nullptr);          // 00552F83
    for (int row = 0; row <= 0x14; ++row) {        // 00552F88..00552FD9 (jle 14h)
        int value;
        {
            StartupString column(kotor_rdata_str_007451d4); // 00552F90 007451D4 / 00552F99; state 9
            c2da_current_get_int_00413660(static_cast<c2da_current::Table*>(xp), nullptr, row, &column, &value); // 00552FB5 (lane-startup-misc)
        }                                          // 00552FC6 005E5C20
        wr32(b, 0x38 + row * 4u, static_cast<Word>(value));                 // 00552FCB/00552FCF
    }
    if (xp) { auto* t = static_cast<c2da_current::Table*>(xp); static_cast<const c2da_current::TableSlot0*>(t->helper.methods)->deleting(t, nullptr, 1); } // typed slot0 (D41)                     // 00552FDB..00552FE7
    kotor_rules::kotor_fastcall_00550590(self, nullptr);        // 00552FEB LoadSkillInfo
    kotor_rules::kotor_fastcall_0054f2c0(self, nullptr);        // 00552FF2 LoadClassInfo
    kotor_rules::kotor_fastcall_0054fec0(self, nullptr);        // 00552FF9 LoadRaceInfo
    kotor_fastcall_00550c50(self, nullptr);        // 00553000 LoadDifficultyInfo
    return self;                                   // 0055300A EAX=this
}
}
