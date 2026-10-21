// lane-prepare: CSWRules::LoadDifficultyInfo 00550C50 and CTwoDimArrays::CTwoDimArrays
// 005BFB60 (swkotor.exe 9c10e045..., base 0x400000). D1 form. C2DA family is
// lane-startup-misc's (D38) / lane-tlk's 00413510 (D44); the C2DA virtual
// deleting destructor is reached through its published typed slot table
// (Helper::methods -> c2da_current::TableSlot0, D41).
#include "b09a95a028-rules_c2da.h"
#include "d6db6bea87-prepare_image_data.h"   // D13 generated .rdata strings
#include "7b8c81fcff-allocator_live.h"
#include <cstring>
#include <new>
#include <initializer_list>

extern "C" {
// lane-tlk c2da_rowcol.cpp (identical declaration, tlk_core.h): C2DA::GetINTEntry(row, column index), RET 0Ch.
std::int32_t __fastcall c2da_current_get_int_entry_00413510(c2da_current::Table* self, void* unused_edx,
    std::int32_t row, std::int32_t column, std::int32_t* out);
}

namespace {
using Word = std::uint32_t;
inline void put32(void* p, unsigned off, Word v) noexcept { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4); }
}

namespace kotor_original {
// 00550C50 CSWRules::LoadDifficultyInfo (EH frame 0071D64B: state 0 frees the
// 54h allocation if C2DA::C2DA unwinds).
void __fastcall kotor_fastcall_00550c50(void* rules, void*) {
    c2da_current::Table* table = nullptr;                                  // 00550C7E ebp=0
    if (void* p = live_operator_new(0x54)) {                               // 00550C6C/00550C70
        try {
            menu_resources::Ref ref;                                       // 00550C89 sub esp,10h; mov ecx,esp
            ref.construct_00406d80(kotor_rdata_str_00748a24);              // 00550C92 push 00748A24 / 00550C97
            table = c2da_current_construct_00413cc0(p, nullptr, ref, 0);   // 00550C88 push ebp(0) / 00550C9E
        } catch (...) { live_free(p); throw; }                             // state 0 funclet
    }
    c2da_current_load_004143b0(table, nullptr);                            // 00550CAF (also when allocation failed)
    unsigned char* row_base = static_cast<unsigned char*>(rules) + 0xBC;   // 00550CB9
    for (std::int32_t column = 1; column <= 5; ++column) {                 // 00550CB4 edi=1 .. 00550CEA jle
        for (std::int32_t row = 0; row < 6; ++row) {                       // 00550CC0 esi=0 .. 00550CE1
            // D2: [esp+10h] is not written by the original when the lookup fails;
            // the result is masked by 'neg/sbb/and' (00550CD4..00550CD8) either way.
            std::int32_t value = 0;
            const std::int32_t found = c2da_current_get_int_entry_00413510(table, nullptr, row, column, &value); // 00550CC2..00550CCB
            row_base[row] = found ? static_cast<unsigned char>(value) : 0; // 00550CD0..00550CDA
        }
        row_base += 6;                                                     // 00550CE4
    }
    if (table)                                                             // 00550CEC
        static_cast<const c2da_current::TableSlot0*>(table->helper.methods)->deleting(table, nullptr, 1); // 00550CF0..00550CF7 call [vtbl+0](1)
}
}

extern "C" {
// 005BFB60 CTwoDimArrays::CTwoDimArrays (0x12C allocation at 00552E8E): zeroes the
// cited fields in the original store order; EAX=this (005BFB60 mov eax,ecx).
void* __fastcall kotor_fastcall_005bfb60(void* self, void*) {
    auto* b = static_cast<unsigned char*>(self);
    for (unsigned off : {0x0Cu, 0x10u, 0x14u, 0x84u, 0x88u, 0x18u, 0x8Cu, 0x1Cu, 0x20u, 0x90u, 0x94u, 0x48u, 0x98u,
                         0x9Cu, 0xA8u, 0x24u, 0x28u, 0x2Cu, 0xA0u, 0xA4u, 0x4Cu, 0x30u, 0xACu, 0xB0u, 0xB4u, 0xB8u,
                         0xBCu, 0xC0u, 0xC4u, 0x34u, 0x00u})
        put32(b, off, 0);                                                  // 005BFB64..005BFBF1
    b[0x08] = 0;                                                           // 005BFBF3
    put32(b, 0x04, 0);                                                     // 005BFBF6
    b[0x09] = 0;                                                           // 005BFBF9
    for (unsigned off : {0x50u, 0x54u, 0x58u, 0x5Cu, 0x60u, 0x64u, 0x68u, 0xC8u, 0xCCu, 0xD0u, 0x70u, 0xD4u, 0x38u,
                         0x74u, 0xE0u, 0xE4u, 0xE8u, 0xECu, 0x3Cu, 0x78u, 0x40u, 0xF4u, 0xF8u, 0xFCu, 0x100u, 0x104u,
                         0x108u, 0x10Cu, 0x44u, 0x114u, 0x110u, 0x7Cu, 0x80u, 0x118u, 0x11Cu, 0x120u, 0x124u, 0x128u})
        put32(b, off, 0);                                                  // 005BFBFC..005BFCAD
    return self;                                                           // 005BFCB3
}
}
