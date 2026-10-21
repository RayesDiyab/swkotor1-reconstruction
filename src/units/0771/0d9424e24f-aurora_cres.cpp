// lane-m47-core: CRes subclasses of the Aurora resource helpers: CResMDL (005CEA50.., table 0074C404),
// CResVIS (00710D50.., 0075FBA8), CResMDX (00710E10.., 0075FBD0), CResPLT (00710B50.., 0075FB94),
// CResDDS (00710EA0.., 0075FBE4), CRes4PC (00710F90.., 0075FBF8), CResTGA (007129A0.., 0075FC48).
// Evidence: lane dis/<addr>.asm (dumpbin /disasm swkotor.exe); tables read from .rdata (tools/rd.py):
//   0074C404 = {005CEA90, 00406490, 0063E7F0, 00710E60, 005D4C60}
//   0075FB94 = {00710CA0, 00406490, 0063E7F0, 00710BD0, 00710CC0}
//   0075FBA8 = {00710D90, 00406490, 0063E7F0, 00710E60, 005D4C60}
//   0075FBD0 = {00710E80, 00406490, 0063E7F0, 00710E60, 005D4C60}
//   0075FBE4 = {00710F70, 00406490, 0063E7F0, 00711010, 00710F30}
//   0075FBF8 = {00711070, 00406490, 0063E7F0, 00711010, 00711030}
//   0075FC48 = {00712C90, 00406490, 0063E7F0, 00712A30, 00712CB0}
// ctor/dtor/deleting shapes are identical to the reviewed CResTXI instance (lane-tpc tpc_txi.cpp 00710DB0/
// 00710DE0/00710DF0) except the zeroed field range, the +20h/+24h one-word and the table (tools/norm.py).
// Allocation family: 006FA7E6 = live_operator_new, 006FA390 = live_free (allocator_live.h).
#include "f34237b809-aurora_family.h"

using namespace aurora_family;
using tpc_lower::Word;

extern "C" {
const ResourceMethods aurora_mdl_resource_methods_0074c404{
    &aurora_mdl_resource_deleting_destroy_005cea90, &c2da_resource_constant_word_00406490,
    &c2da_resource_constant_word_0063e7f0, &aurora_txi_resource_freed_00710e60, &aurora_generic_resource_serviced_005d4c60};
const ResourceMethods aurora_vis_resource_methods_0075fba8{
    &aurora_vis_resource_deleting_destroy_00710d90, &c2da_resource_constant_word_00406490,
    &c2da_resource_constant_word_0063e7f0, &aurora_txi_resource_freed_00710e60, &aurora_generic_resource_serviced_005d4c60};
const ResourceMethods aurora_mdx_resource_methods_0075fbd0{
    &aurora_mdx_resource_deleting_destroy_00710e80, &c2da_resource_constant_word_00406490,
    &c2da_resource_constant_word_0063e7f0, &aurora_txi_resource_freed_00710e60, &aurora_generic_resource_serviced_005d4c60};
const ResourceMethods aurora_plt_resource_methods_0075fb94{
    &aurora_plt_resource_deleting_destroy_00710ca0, &c2da_resource_constant_word_00406490,
    &c2da_resource_constant_word_0063e7f0, &aurora_plt_resource_freed_00710bd0, &aurora_plt_resource_serviced_00710cc0};
const ResourceMethods aurora_dds_resource_methods_0075fbe4{
    &aurora_dds_resource_deleting_destroy_00710f70, &c2da_resource_constant_word_00406490,
    &c2da_resource_constant_word_0063e7f0, &aurora_dds_resource_freed_00711010, &aurora_dds_resource_serviced_00710f30};
const ResourceMethods aurora_4pc_resource_methods_0075fbf8{
    &aurora_4pc_resource_deleting_destroy_00711070, &c2da_resource_constant_word_00406490,
    &c2da_resource_constant_word_0063e7f0, &aurora_dds_resource_freed_00711010, &aurora_4pc_resource_serviced_00711030};
const ResourceMethods aurora_tga_resource_methods_0075fc48{
    &aurora_tga_resource_deleting_destroy_00712c90, &c2da_resource_constant_word_00406490,
    &c2da_resource_constant_word_0063e7f0, &aurora_tga_resource_freed_00712a30, &aurora_tga_resource_serviced_00712cb0};
}

namespace {
void zero_words(void* self, unsigned first, unsigned last) {   // dword stores of 0 at first..last inclusive
    for (unsigned offset = first; offset <= last; offset += 4) store(self, offset, Word{0});
}
template <const ResourceMethods* Table> void* deleting(void* self, Word flags, void (__fastcall* destroy)(void*, void*) noexcept) {
    destroy(self, nullptr);
    if ((flags & 1u) != 0u) live_free(self);                   // test byte [esp+8],1 / call 006FA390
    return self;
}
// Uniform tail of every OnResourceServiced failure path that releases the decoded state: call [vtbl+0Ch]
// (OnResourceFreed) on this (00710D12..16, 00712E59..5D); the slot's EAX is ignored.
void call_freed_slot(void* self) {
    const void* const methods = load<const void*>(self, 0);
    tpc_lower::ResourceCallback slot;
    std::memcpy(&slot, static_cast<const unsigned char*>(methods) + 0x0C, sizeof(slot));
    (void)slot(self, nullptr);
}
unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
}

extern "C" {
// ======================================================================= CResMDL (0x34)
// 005CEA50: base ctor 00406460; zero +28/+2C/+30; table 0074C404; +20 = 1.
__declspec(noinline) void* __fastcall aurora_mdl_resource_ctor_005cea50(void* self, void*) {
    (void)c2da_resource_base_ctor_00406460(self, nullptr);                       // 005CEA53
    zero_words(self, 0x28, 0x30);                                                // 005CEA5A..60
    store<const void*>(self, 0, &aurora_mdl_resource_methods_0074c404);          // 005CEA63
    store(self, 0x20, Word{1});                                                  // 005CEA69
    return self;
}
// 005CEA80: table store, JMP 00409D40.
__declspec(noinline) void __fastcall aurora_mdl_resource_destroy_005cea80(void* self, void*) noexcept {
    store<const void*>(self, 0, &aurora_mdl_resource_methods_0074c404);
    c2da_resource_base_destroy_00409d40(self, nullptr);
}
// 005CEA90: slot0 of 0074C404.
__declspec(noinline) void* __fastcall aurora_mdl_resource_deleting_destroy_005cea90(void* self, void*, Word flags) noexcept {
    return deleting<&aurora_mdl_resource_methods_0074c404>(self, flags, &aurora_mdl_resource_destroy_005cea80);
}

// ======================================================================= CResVIS (0x34)
// 00710D50: base ctor; zero +28..+30; table 0075FBA8; +20 = 1.
__declspec(noinline) void* __fastcall aurora_vis_resource_ctor_00710d50(void* self, void*) {
    (void)c2da_resource_base_ctor_00406460(self, nullptr);
    zero_words(self, 0x28, 0x30);
    store<const void*>(self, 0, &aurora_vis_resource_methods_0075fba8);
    store(self, 0x20, Word{1});
    return self;
}
// 00710D80 / 00710D90.
__declspec(noinline) void __fastcall aurora_vis_resource_destroy_00710d80(void* self, void*) noexcept {
    store<const void*>(self, 0, &aurora_vis_resource_methods_0075fba8);
    c2da_resource_base_destroy_00409d40(self, nullptr);
}
__declspec(noinline) void* __fastcall aurora_vis_resource_deleting_destroy_00710d90(void* self, void*, Word flags) noexcept {
    return deleting<&aurora_vis_resource_methods_0075fba8>(self, flags, &aurora_vis_resource_destroy_00710d80);
}

// ======================================================================= CResMDX (0x34)
// 00710E10: base ctor; zero +28..+30; table 0075FBD0; +20 = 1.
__declspec(noinline) void* __fastcall aurora_mdx_resource_ctor_00710e10(void* self, void*) {
    (void)c2da_resource_base_ctor_00406460(self, nullptr);
    zero_words(self, 0x28, 0x30);
    store<const void*>(self, 0, &aurora_mdx_resource_methods_0075fbd0);
    store(self, 0x20, Word{1});
    return self;
}
// 00710E40 / 00710E80.
__declspec(noinline) void __fastcall aurora_mdx_resource_destroy_00710e40(void* self, void*) noexcept {
    store<const void*>(self, 0, &aurora_mdx_resource_methods_0075fbd0);
    c2da_resource_base_destroy_00409d40(self, nullptr);
}
__declspec(noinline) void* __fastcall aurora_mdx_resource_deleting_destroy_00710e80(void* self, void*, Word flags) noexcept {
    return deleting<&aurora_mdx_resource_methods_0075fbd0>(self, flags, &aurora_mdx_resource_destroy_00710e40);
}

// ======================================================================= CResDDS (0x38)
// 00710EA0: base ctor; zero +28..+34; table 0075FBE4; +20 = 1.
__declspec(noinline) void* __fastcall aurora_dds_resource_ctor_00710ea0(void* self, void*) {
    (void)c2da_resource_base_ctor_00406460(self, nullptr);                       // 00710EA3
    zero_words(self, 0x28, 0x34);                                                // 00710EAA..B3
    store<const void*>(self, 0, &aurora_dds_resource_methods_0075fbe4);          // 00710EB6
    store(self, 0x20, Word{1});                                                  // 00710EBC
    return self;
}
// 00710ED0 / 00710F70.
__declspec(noinline) void __fastcall aurora_dds_resource_destroy_00710ed0(void* self, void*) noexcept {
    store<const void*>(self, 0, &aurora_dds_resource_methods_0075fbe4);
    c2da_resource_base_destroy_00409d40(self, nullptr);
}
__declspec(noinline) void* __fastcall aurora_dds_resource_deleting_destroy_00710f70(void* self, void*, Word flags) noexcept {
    return deleting<&aurora_dds_resource_methods_0075fbe4>(self, flags, &aurora_dds_resource_destroy_00710ed0);
}
// 00711010 CResDDS::OnResourceFreed (also CRes4PC slot +0C): zero +28..+34, return 1.
__declspec(noinline) Word __fastcall aurora_dds_resource_freed_00711010(void* self, void*) {
    zero_words(self, 0x28, 0x34);                                                // 00711012..1B
    return 1u;
}
// 00710F30 CResDDS::OnResourceServiced: header pointer +34 = data, payload +30 = data+14h.
__declspec(noinline) Word __fastcall aurora_dds_resource_serviced_00710f30(void* self, void*) {
    unsigned char* const data = load<unsigned char*>(self, 0x10);                // 00710F30
    if (data == nullptr) return 0;                                               // 00710F33..35 -> 00710F52
    if (load<Word>(self, 0x28) != 0) return 1;                                   // 00710F37..43
    if (load<void*>(self, 0x34) != nullptr) return 0;                            // 00710F44..49
    if (load<void*>(self, 0x30) != nullptr) return 0;                            // 00710F4B..50
    store<unsigned char*>(self, 0x34, data);                                     // 00710F55
    store<unsigned char*>(self, 0x30, data + 0x14);                              // 00710F58..5B
    store(self, 0x28, Word{1});                                                  // 00710F63
    return 1;
}
// 00710EE0 CResDDS::GetDDSAttrib(w, h, byte, size, alpha) RET 14h: copies header words (raw dword moves).
__declspec(noinline) Word __fastcall aurora_dds_attrib_00710ee0(void* self, void*, Word* width, Word* height,
                                                                unsigned char* format, Word* size, float* alpha) {
    if (load<void*>(self, 0x34) == nullptr) return 0;                            // 00710EE0..E7 (EAX = 0)
    *width = load<Word>(load<void*>(self, 0x34), 0);                             // 00710EEA..F0
    *height = load<Word>(load<void*>(self, 0x34), 4);                            // 00710EF2..FC
    *format = load<unsigned char>(load<void*>(self, 0x34), 8);                   // 00710EFE..08
    *size = load<Word>(load<void*>(self, 0x34), 0x0C);                           // 00710F0A..14
    const Word bits = load<Word>(load<void*>(self, 0x34), 0x10);                 // 00710F16..1D (integer move)
    std::memcpy(alpha, &bits, 4);                                                // 00710F20
    return 1;
}

// ======================================================================= CRes4PC (0x38)
// 00710F90: base ctor; zero +28..+34; table 0075FBF8; +20 = 1.
__declspec(noinline) void* __fastcall aurora_4pc_resource_ctor_00710f90(void* self, void*) {
    (void)c2da_resource_base_ctor_00406460(self, nullptr);
    zero_words(self, 0x28, 0x34);
    store<const void*>(self, 0, &aurora_4pc_resource_methods_0075fbf8);
    store(self, 0x20, Word{1});
    return self;
}
// 00710FC0 / 00711070.
__declspec(noinline) void __fastcall aurora_4pc_resource_destroy_00710fc0(void* self, void*) noexcept {
    store<const void*>(self, 0, &aurora_4pc_resource_methods_0075fbf8);
    c2da_resource_base_destroy_00409d40(self, nullptr);
}
__declspec(noinline) void* __fastcall aurora_4pc_resource_deleting_destroy_00711070(void* self, void*, Word flags) noexcept {
    return deleting<&aurora_4pc_resource_methods_0075fbf8>(self, flags, &aurora_4pc_resource_destroy_00710fc0);
}
// 00711030 CRes4PC::OnResourceServiced: as 00710F30 with payload = data+10h.
__declspec(noinline) Word __fastcall aurora_4pc_resource_serviced_00711030(void* self, void*) {
    unsigned char* const data = load<unsigned char*>(self, 0x10);
    if (data == nullptr) return 0;                                               // 00711033..35
    if (load<Word>(self, 0x28) != 0) return 1;                                   // 00711037..43
    if (load<void*>(self, 0x34) != nullptr) return 0;                            // 00711044..49
    if (load<void*>(self, 0x30) != nullptr) return 0;                            // 0071104B..50
    store<unsigned char*>(self, 0x34, data);                                     // 00711055
    store<unsigned char*>(self, 0x30, data + 0x10);                              // 00711058..5B
    store(self, 0x28, Word{1});                                                  // 00711063
    return 1;
}
// 00710FD0 CRes4PC::Get4PCAttrib(w, h, float*, int*) RET 10h (raw dword moves).
__declspec(noinline) Word __fastcall aurora_4pc_attrib_00710fd0(void* self, void*, Word* width, Word* height,
                                                                float* alpha, Signed* format) {
    if (load<void*>(self, 0x34) == nullptr) return 0;                            // 00710FD0..D7
    *width = load<Word>(load<void*>(self, 0x34), 0);                             // 00710FDA..E0
    *height = load<Word>(load<void*>(self, 0x34), 4);                            // 00710FE2..EC
    const Word bits = load<Word>(load<void*>(self, 0x34), 8);                    // 00710FEE..F4
    std::memcpy(alpha, &bits, 4);                                                // 00710FF8
    *format = load<Signed>(load<void*>(self, 0x34), 0x0C);                       // 00710FFA..11004
    return 1;
}

// ======================================================================= CResPLT (0x44)
// 00710B50: base ctor; zero +28..+40; table 0075FB94; +24 = 1.
__declspec(noinline) void* __fastcall aurora_plt_resource_ctor_00710b50(void* self, void*) {
    (void)c2da_resource_base_ctor_00406460(self, nullptr);                       // 00710B53
    zero_words(self, 0x28, 0x40);                                                // 00710B5A..6C
    store<const void*>(self, 0, &aurora_plt_resource_methods_0075fb94);          // 00710B6F
    store(self, 0x24, Word{1});                                                  // 00710B75
    return self;
}
// 00710B80 / 00710CA0.
__declspec(noinline) void __fastcall aurora_plt_resource_destroy_00710b80(void* self, void*) noexcept {
    store<const void*>(self, 0, &aurora_plt_resource_methods_0075fb94);
    c2da_resource_base_destroy_00409d40(self, nullptr);
}
__declspec(noinline) void* __fastcall aurora_plt_resource_deleting_destroy_00710ca0(void* self, void*, Word flags) noexcept {
    return deleting<&aurora_plt_resource_methods_0075fb94>(self, flags, &aurora_plt_resource_destroy_00710b80);
}
// 00710BD0 CResPLT::OnResourceFreed: free the decoded pixels (+3C) when +28 says they were allocated; zero +28..+40.
__declspec(noinline) Word __fastcall aurora_plt_resource_freed_00710bd0(void* self, void*) {
    if (load<Word>(self, 0x28) != 0) {                                           // 00710BD3..DB
        if (void* const pixels = load<void*>(self, 0x3c)) live_free(pixels);     // 00710BDD..E5 006FA390
    }
    zero_words(self, 0x28, 0x40);                                                // 00710BED..FF
    return 1u;
}
// 00710C10 CResPLT::ReadRLEPLT: decode the 1-byte RLE stream at +3C into a new buffer of +34 bytes.
__declspec(noinline) Word __fastcall aurora_plt_read_rle_00710c10(void* self, void*) noexcept(false) {
    const unsigned char* const source = load<const unsigned char*>(self, 0x3c);  // 00710C17 (EBP)
    unsigned char* const out = static_cast<unsigned char*>(live_operator_new(load<Word>(self, 0x34))); // 00710C14..1B
    store<unsigned char*>(self, 0x3c, out);                                      // 00710C25
    if (out == nullptr) return 0;                                                // 00710C23..2C (EAX = 0)
    Word in = 0, at = 0;                                                         // 00710C30..32
    while (at < load<Word>(self, 0x34)) {                                        // 00710C34..36 / 00710C80..83 (unsigned)
        const unsigned char control = source[in++];                              // 00710C40..4D
        const Word end = (control & 0x7fu) + at + 1u;                            // 00710C4A / 00710C50
        if ((control & 0x80u) != 0) {                                            // 00710C47 / 00710C4E
            const unsigned char value = source[in++];                            // 00710C56..59
            while (at < end) bytes(load<void*>(self, 0x3c))[at++] = value;       // 00710C5A..69 (+3C reloaded)
        } else {
            while (at < end) bytes(load<void*>(self, 0x3c))[at++] = source[in++]; // 00710C6D..7E
        }
    }
    store(self, 0x28, Word{1});                                                  // 00710C87..8C
    return 1;
}
// 00710CC0 CResPLT::OnResourceServiced.
__declspec(noinline) Word __fastcall aurora_plt_resource_serviced_00710cc0(void* self, void*) noexcept(false) {
    unsigned char* const data = load<unsigned char*>(self, 0x10);                // 00710CC3
    if (data == nullptr) return 0;                                               // 00710CC6..C8
    if (load<Word>(self, 0x30) != 0) return 1;                                   // 00710CCA..CF -> 00710D41
    if (load<void*>(self, 0x40) != nullptr) return 0;                            // 00710CD1..D6
    if (load<void*>(self, 0x3c) != nullptr) return 0;                            // 00710CD8..DD
    store<unsigned char*>(self, 0x40, data);                                     // 00710CDF
    if (load<Word>(data, 0) != 0x20544c50u) return 0;                            // 00710CE2..E8 ("PLT ")
    const Word size = load<Word>(self, 0x18);                                    // 00710CEF
    if (load<Word>(data, 0x0c) != 0) {                                           // 00710CEA..F2
        store(self, 0x38, size - 0x18u);                                         // 00710CF4..F7
        store(self, 0x2c, Word{1});                                              // 00710CFF
        store<unsigned char*>(self, 0x3c, data + 0x18);                          // 00710CFA / 00710D06
        if (aurora_plt_read_rle_00710c10(self, nullptr) == 0) {                  // 00710D09..10
            call_freed_slot(self);                                               // 00710D12..16 [vtbl+0Ch]
            return 0;                                                            // 00710D19
        }
    } else {
        store(self, 0x2c, Word{0});                                              // 00710D23
        store(self, 0x28, Word{0});                                              // 00710D2A
        store(self, 0x34, size - 0x18u);                                         // 00710D1D / 00710D31 (add ecx,-18h)
        store(self, 0x38, size - 0x18u);                                         // 00710D34
        store<unsigned char*>(self, 0x3c, data + 0x18);                          // 00710D20 / 00710D37
    }
    store(self, 0x30, Word{1});                                                  // 00710D3A
    return 1;
}
// 00710B90 CResPLT::GetPLTAttrib(w, h, layers) RET 0Ch: header (+40) words +10, +14, +8.
__declspec(noinline) Word __fastcall aurora_plt_attrib_00710b90(void* self, void*, Word* width, Word* height, Word* third) {
    if (load<void*>(self, 0x40) == nullptr) return 0;                            // 00710B90..97 (EAX = 0)
    *width = load<Word>(load<void*>(self, 0x40), 0x10);                          // 00710B9A..A1
    *height = load<Word>(load<void*>(self, 0x40), 0x14);                         // 00710BA3..AD
    *third = load<Word>(load<void*>(self, 0x40), 0x08);                          // 00710BAF..B9
    return 1;
}

// ======================================================================= CResTGA (0x4C)
// 007129A0: base ctor; zero +28..+48; table 0075FC48; +24 = 1.
__declspec(noinline) void* __fastcall aurora_tga_resource_ctor_007129a0(void* self, void*) {
    (void)c2da_resource_base_ctor_00406460(self, nullptr);                       // 007129A3
    zero_words(self, 0x28, 0x48);                                                // 007129AA..C2
    store<const void*>(self, 0, &aurora_tga_resource_methods_0075fc48);          // 007129C5
    store(self, 0x24, Word{1});                                                  // 007129CB
    return self;
}
// 007129E0 / 00712C90.
__declspec(noinline) void __fastcall aurora_tga_resource_destroy_007129e0(void* self, void*) noexcept {
    store<const void*>(self, 0, &aurora_tga_resource_methods_0075fc48);
    c2da_resource_base_destroy_00409d40(self, nullptr);
}
__declspec(noinline) void* __fastcall aurora_tga_resource_deleting_destroy_00712c90(void* self, void*, Word flags) noexcept {
    return deleting<&aurora_tga_resource_methods_0075fc48>(self, flags, &aurora_tga_resource_destroy_007129e0);
}
// 00712A30 CResTGA::OnResourceFreed: free decoded pixels (+44) when +28; zero +28..+48.
__declspec(noinline) Word __fastcall aurora_tga_resource_freed_00712a30(void* self, void*) {
    if (load<Word>(self, 0x28) != 0) {                                           // 00712A33..3B
        if (void* const pixels = load<void*>(self, 0x44)) live_free(pixels);     // 00712A3D..45
    }
    zero_words(self, 0x28, 0x48);                                                // 00712A4D..62
    return 1u;
}
// 00712A70 CResTGA::ReadColorMappedRLETGA: 1-byte RLE of +3C... source = old +44, output +38 bytes (+0Ah slack).
__declspec(noinline) Word __fastcall aurora_tga_read_color_mapped_rle_00712a70(void* self, void*) noexcept(false) {
    const unsigned char* const source = load<const unsigned char*>(self, 0x44);  // 00712A77 (EBP)
    unsigned char* const out = static_cast<unsigned char*>(live_operator_new(load<Word>(self, 0x38) + 0x0au)); // 00712A74..7E
    store<unsigned char*>(self, 0x44, out);                                      // 00712A88
    if (out == nullptr) return 0;                                                // 00712A86..8F
    Word in = 0, at = 0;                                                         // 00712A93..95
    if (load<Word>(self, 0x38) != 0) {                                           // 00712A97..99
        do {
            const unsigned char control = source[in++];                          // 00712AA0..AD
            const Word end = (control & 0x7fu) + at + 1u;                        // 00712AB0
            if ((control & 0x80u) != 0) {
                const unsigned char value = source[in++];                        // 00712AB6..B9
                while (at < end) bytes(load<void*>(self, 0x44))[at++] = value;   // 00712ABA..C9
            } else {
                while (at < end) bytes(load<void*>(self, 0x44))[at++] = source[in++]; // 00712ACD..DE
            }
        } while (at < load<Word>(self, 0x38));                                   // 00712AE0..E3
    }
    store(self, 0x28, Word{1});                                                  // 00712AE7..EC
    return 1;
}
// 00712B00 CResTGA::ReadUnmappedRLETGA: 24/32-bit pixel RLE (header byte +10h selects), other depths fail.
__declspec(noinline) Word __fastcall aurora_tga_read_unmapped_rle_00712b00(void* self, void*) noexcept(false) {
    const unsigned char* const source = load<const unsigned char*>(self, 0x44);  // 00712B07 (EDI)
    unsigned char* const out = static_cast<unsigned char*>(live_operator_new(load<Word>(self, 0x38) + 0x0au)); // 00712B03..0E
    store<unsigned char*>(self, 0x44, out);                                      // 00712B18
    if (out == nullptr) return 0;                                                // 00712B16..1F
    const unsigned char depth = load<unsigned char>(load<void*>(self, 0x48), 0x10); // 00712B20..23
    Word in = 0, at = 0;                                                         // 00712B27..29
    Word stride;
    if (depth == 0x18) stride = 3;                                               // 00712B2B..2F
    else if (depth == 0x20) stride = 4;                                          // 00712BC9..CC
    else return 0;                                                               // 00712C89..8F (+28 untouched)
    if (load<Word>(self, 0x38) != 0) {                                           // 00712B35..3A / 00712BD2..D7
        do {
            const unsigned char control = source[in++];                          // 00712B40..53 / 00712BE0..ED
            const Word end = at + stride * ((control & 0x7fu) + 1u);             // 00712B4D..51 / 00712BF0
            if ((control & 0x80u) != 0) {                                        // run: repeat one pixel
                while (at < end) {                                               // 00712B5A..82 / 00712BF6..2E
                    for (Word k = 0; k < stride; ++k) bytes(load<void*>(self, 0x44))[at + k] = source[in + k];
                    at += stride;
                }
                in += stride;                                                    // 00712B84 / 00712C30
            } else {                                                             // raw: copy pixels in sequence
                while (at < end) {                                               // 00712B89..B5 / 00712C35..71
                    for (Word k = 0; k < stride; ++k) bytes(load<void*>(self, 0x44))[at + k] = source[in + k];
                    at += stride; in += stride;
                }
            }
        } while (at < load<Word>(self, 0x38));                                   // 00712BB7..BA / 00712C73..76
    }
    store(self, 0x28, Word{1});                                                  // 00712BBE..C4 / 00712C7E..84
    return 1;
}
// 00712CB0 CResTGA::OnResourceServiced: parse the 18-byte TGA header and locate the colour map / pixels.
// Image types (header +2): 1 -> 00712D12, 2/3 -> 00712D7F, 9 -> 00712DAE, 10 -> 00712E02, else 00712E67
// (index table 00712E88 = {0,1,1,4,4,4,4,4,2,3}, jump table 00712E74).
__declspec(noinline) Word __fastcall aurora_tga_resource_serviced_00712cb0(void* self, void*) noexcept(false) {
    unsigned char* const header = load<unsigned char*>(self, 0x10);              // 00712CB3
    if (header == nullptr) return 0;                                             // 00712CB9..BB
    if (load<Word>(self, 0x34) != 0) return 1;                                   // 00712CBD..C9
    if (load<void*>(self, 0x48) != nullptr || load<void*>(self, 0x44) != nullptr ||
        load<void*>(self, 0x40) != nullptr) return 0;                            // 00712CCA..D7
    store<unsigned char*>(self, 0x48, header);                                   // 00712CDE
    store<unsigned char*>(self, 0x40, header + header[0] + 0x12);                // 00712CE1..E8 (id length + 18)
    const Word map_length = load<std::uint16_t>(header, 5);                      // 00712CEF
    const Word map_bytes = (Word{header[7]} >> 3) * map_length;                  // entry bits >> 3 * length
    const Word size = load<Word>(self, 0x18);
    Word decoded = 1;
    switch (header[2]) {                                                         // 00712CEB..0B
    case 1:                                                                      // 00712D12
        store(self, 0x38, size - map_bytes - header[0] - 0x12u);                 // 00712D16..2C
        store<unsigned char*>(self, 0x44, load<unsigned char*>(self, 0x40) + map_bytes); // 00712D2F..3C
        store(self, 0x2c, Word{1});                                              // 00712D3F
        break;
    case 2: case 3:                                                              // 00712D7F (no +2C store)
        store(self, 0x38, size - map_bytes - header[0] - 0x12u);                 // 00712D83..99
        store<unsigned char*>(self, 0x44, load<unsigned char*>(self, 0x40) + map_bytes); // 00712D9C..A9
        break;
    case 9:                                                                      // 00712DAE
        store(self, 0x3c, size - map_bytes - header[0] - 0x12u);                 // 00712DB2..C8
        store(self, 0x38, (Word{header[0x10]} >> 3) * load<std::uint16_t>(header, 0x0e) *
                              load<std::uint16_t>(header, 0x0c));                // 00712DCB..E0
        store<unsigned char*>(self, 0x44, load<unsigned char*>(self, 0x40) + map_bytes); // 00712DE3..F2
        store(self, 0x2c, Word{1});                                              // 00712DF5
        store(self, 0x30, Word{1});                                              // 00712DF8
        decoded = aurora_tga_read_color_mapped_rle_00712a70(self, nullptr);      // 00712DFB
        break;
    case 10:                                                                     // 00712E02 (no +2C store)
        store(self, 0x3c, size - map_bytes - header[0] - 0x12u);                 // 00712E06..1C
        store(self, 0x38, (Word{header[0x10]} >> 3) * load<std::uint16_t>(header, 0x0e) *
                              load<std::uint16_t>(header, 0x0c));                // 00712E1F..34
        store<unsigned char*>(self, 0x44, load<unsigned char*>(self, 0x40) + map_bytes); // 00712E37..46
        store(self, 0x30, Word{1});                                              // 00712E49
        decoded = aurora_tga_read_unmapped_rle_00712b00(self, nullptr);          // 00712E4C
        break;
    default:                                                                     // 00712E67
        store(self, 0x40, Word{0});                                              // 00712E69
        store(self, 0x48, Word{0});                                              // 00712E6C
        return 0;
    }
    if (decoded == 0) {                                                          // 00712E51..53
        call_freed_slot(self);                                                   // 00712E59..5D [vtbl+0Ch]
        return 0;                                                                // 00712E63
    }
    const unsigned char* const parsed = load<const unsigned char*>(self, 0x48);  // 00712D42
    if (parsed[2] != 3) {                                                        // 00712D45..49: swap R/B except grey
        const Word step = Word{parsed[0x10]} >> 3;                               // 00712D4B / 00712D5D
        Word count = Word{load<std::uint16_t>(parsed, 0x0e)} * load<std::uint16_t>(parsed, 0x0c); // 00712D4F..57
        unsigned char* pixel = load<unsigned char*>(self, 0x44);                 // 00712D5A
        for (; count != 0; --count) {                                            // 00712D60..73
            const unsigned char first = pixel[0];
            pixel[0] = pixel[2];
            pixel[2] = first;
            pixel += step;
        }
    }
    store(self, 0x34, Word{1});                                                  // 00712D75 (EBP = 1)
    return 1;
}
// 007129F0 CResTGA::GetTGAAttrib(w, h, bits) RET 0Ch: header (+48) words +0C/+0E zero-extended, byte +10.
__declspec(noinline) Word __fastcall aurora_tga_attrib_007129f0(void* self, void*, Word* width, Word* height, unsigned char* bits) {
    if (load<void*>(self, 0x48) == nullptr) return 0;                            // 007129F0..F7 (EAX = 0)
    *width = load<std::uint16_t>(load<void*>(self, 0x48), 0x0c);                 // 007129FA..A02
    *height = load<std::uint16_t>(load<void*>(self, 0x48), 0x0e);                // 00712A04..0F
    *bits = load<unsigned char>(load<void*>(self, 0x48), 0x10);                  // 00712A11..1B
    return 1;
}
}
