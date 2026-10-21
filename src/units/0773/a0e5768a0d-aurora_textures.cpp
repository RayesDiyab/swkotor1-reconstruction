// lane-m47-core: texture CResHelper instances TGA (0003), PLT (0006), DDS (07F1), 4PC (080B) and their
// instance-specific members (Load / Unload / ReadTextureHeader / GetPtrToResHelperInfo / attribute getters),
// plus CAuroraProcessedTexture::ReadProcessedTextureHeader 0070F590 (lane-tpc helper type).
// Template members follow aurora_family_impl.h (TXI shape, tools/norm.py); evidence lane dis/<addr>.asm.
// D2 (labelled): stack outputs of an attribute getter that returns early are read uninitialised by the
// original at 0070EC44.., 0070ECA0.., 0070ED9F.., 0070DAE4.., 0070DB6F..; value-initialised to 0 here.
// D30-FINAL note: header floats copied by fld/fstp in the original (00710456/00710482, 00710836/0071084E,
// 0070F5C0/0070F5E7) are copied as float values here (identical bits for every non-signalling value).
#include "76452bc32b-aurora_family_impl.h"

using namespace aurora_family_impl;
using font_callback_read::Texture;
using font_callback_read::LayeredTexture;
using font_callback_read::CompressedTexture;
using font_callback_read::Texture16;
using font_callback_read::ProcessedTexture;

namespace {
template <class T> T header_value(const unsigned char* header, unsigned offset) { return load<T>(header, offset); }
}

extern "C" {
// ======================================================================= TGA 0003 (CResTGA 4Ch via 007129A0)
__declspec(noinline) void __fastcall aurora_texture_base_destroy_0070edd0(Texture* self, void*) noexcept {
    base_destroy(self, &aurora_texture_helper_methods_0075fb54);                 // 0070EDD0..2F (table 0075FB54)
}
__declspec(noinline) void __fastcall aurora_texture_destroy_0070f030(Texture* self, void*) noexcept {
    self->methods = &aurora_texture_methods_0075fb58;                            // 0070F030
    aurora_texture_base_destroy_0070edd0(self, nullptr);                         // 0070F036 JMP
}
__declspec(noinline) void* __fastcall aurora_texture_base_deleting_destroy_0070ef80(void* self, void*, Word flags) noexcept {
    return deleting<Texture>(self, flags, &aurora_texture_base_destroy_0070edd0); // 0070EF80
}
__declspec(noinline) void* __fastcall aurora_texture_deleting_destroy_0070f040(void* self, void*, Word flags) noexcept {
    return deleting<Texture>(self, flags, &aurora_texture_destroy_0070f030);     // 0070F040
}
__declspec(noinline) void __fastcall aurora_texture_set_ref_0070ee30(Texture* self, void*, menu_resources::Ref* incoming,
                                                                     Signed flag) noexcept(false) {
    set_ref(self, incoming, flag, 0x0003u, 0x4cu, &aurora_tga_resource_ctor_007129a0); // 0070EE30 (push 3 x2 0070EEF7/2F, push 4Ch 0070EF05)
}
__declspec(noinline) __declspec(safebuffers) Texture* __fastcall aurora_texture_construct_0070efa0(
    Texture* self, void*, menu_resources::Ref incoming, Signed mode) noexcept(false) {
    return construct(self, &incoming, mode, &aurora_texture_helper_methods_0075fb54, &aurora_texture_methods_0075fb58,
                     &aurora_texture_set_ref_0070ee30, &aurora_texture_base_destroy_0070edd0);   // 0070EFA0..F020 (0073A978)
}
// 0070EBB0 CAuroraTexture::GetTextureAttrib(w, h, bits) RET 0Ch.
__declspec(noinline) Signed __fastcall aurora_texture_attrib_0070ebb0(Texture* self, void*, Word* width, Word* height,
                                                                      unsigned char* bits) {
    if (!demanded(self->resource)) return 0;                                     // 0070EBB3..C2 / 0070EBDF
    return static_cast<Signed>(aurora_tga_attrib_007129f0(self->resource, nullptr, width, height, bits)); // 0070EBC4..D6
}
// 0070EBF0 CAuroraTexture::Load: Demand; attrib; 8/24/32-bit only; tag 0003 after the pixels.
__declspec(noinline) Signed __fastcall aurora_texture_load_0070ebf0(Texture* self, void*) noexcept(false) {
    if (!c2da_current_resource_demand_00409b10(self->resource, nullptr)) return 0; // 0070EBF6..00 -> 0070EC3D
    Word width = 0, height = 0; unsigned char bits = 0;                          // D2 (0070EC44 reads)
    (void)aurora_tga_attrib_007129f0(self->resource, nullptr, &width, &height, &bits); // 0070EC02..14 (EAX ignored)
    if (bits == 8 || bits == 0x18 || bits == 0x20) {                             // 0070EC19..27
        if (unsigned char* const data = static_cast<unsigned char*>(
                static_cast<void*>(aurora_txi_pointer_00712fa0(self->resource, nullptr)))) {   // 0070EC29..33 ([+44h])
            unsigned char* const tag = data + (Word{bits} >> 3) * width * height + 4u;          // 0070EC44..56
            store<Texture*>(tag, 2, self);                                       // 0070EC5A
            store<std::uint16_t>(tag, 0, 0x0003);                                // 0070EC5D
            return 1;
        }
    }
    c2da_release_demand_00409b80(self->resource, nullptr);                       // 0070EC35..38
    return 0;
}
// 0070EC70 CAuroraTexture::Unload: when demanded, swap R/B of every pixel back, then release the demand.
__declspec(noinline) void __fastcall aurora_texture_unload_0070ec70(Texture* self, void*) noexcept(false) {
    if (!demanded(self->resource)) return;                                       // 0070EC76..85
    Word width = 0, height = 0; unsigned char bits = 0;                          // D2 (0070ECA0 reads)
    (void)aurora_tga_attrib_007129f0(self->resource, nullptr, &width, &height, &bits); // 0070EC89..9B
    const Word step = Word{bits} >> 3;                                           // 0070ECA0 / 0070ECB1
    Word count = width * height;                                                 // 0070ECA5..AC
    unsigned char* pixel = static_cast<unsigned char*>(static_cast<void*>(
        aurora_txi_pointer_00712fa0(self->resource, nullptr)));                  // 0070ECA9..B4
    for (; count != 0; --count) {                                                // 0070ECB9..CD (unsigned, JBE)
        const unsigned char first = pixel[0];
        pixel[0] = pixel[2];
        pixel[2] = first;
        pixel += step;
    }
    c2da_release_demand_00409b80(self->resource, nullptr);                       // 0070ECCF..D2
}
// 0070ECE0 CAuroraTexture::ReadTextureHeader(int* w, int* h, int* bytes) RET 0Ch: raw 12h-byte TGA header.
__declspec(noinline) Signed __fastcall aurora_texture_read_header_0070ece0(Texture* self, void*, Signed* width, Signed* height,
                                                                           Signed* bytes_per_pixel) noexcept(false) {
    void* const resource = self->resource;                                       // 0070ECE0
    unsigned char header[0x14];                                                  // [esp] 14h local, 12h read
    if (!resource || !resman_res_read_raw_00409b20(resource, nullptr, 0x12, reinterpret_cast<char*>(header))) return 0; // 0070ECE6..F8
    *width = header_value<std::uint16_t>(header, 0x0c);                          // 0070ED02..10
    *height = header_value<std::uint16_t>(header, 0x0e);                         // 0070ED0B..1B
    *bytes_per_pixel = static_cast<Signed>(Word{header[0x10]} >> 3);             // 0070ED12..24
    return 1;
}
// 0070ED40 CAuroraTexture::GetPtrToResHelperInfo: pixels + (bits>>3)*w*h + 4, or 0.
__declspec(noinline) void* __fastcall aurora_texture_info_0070ed40(Texture* self, void*) {
    if (!demanded(self->resource)) return nullptr;                               // 0070ED46..55 / 0070EDBB
    unsigned char* const data = demanded(self->resource)
        ? static_cast<unsigned char*>(static_cast<void*>(aurora_txi_pointer_00712fa0(self->resource, nullptr)))
        : nullptr;                                                               // 0070ED57..75
    Word width = 0, height = 0; unsigned char bits = 0;                          // D2 (0070ED9F reads)
    if (demanded(self->resource))
        (void)aurora_tga_attrib_007129f0(self->resource, nullptr, &width, &height, &bits); // 0070ED77..9A
    return data + (Word{bits} >> 3) * width * height + 4u;                       // 0070ED9F..B1
}
__declspec(noinline) Texture* __cdecl aurora_texture_target_0070e130(String* name) noexcept(false) {
    return getter<Texture>(name, &aurora_texture_construct_0070efa0, &aurora_texture_info_0070ed40,
                           &aurora_texture_load_0070ebf0, 0x0003);               // 0070E130..213 (info 0070ED40, tag 3)
}

// ======================================================================= PLT 0006 (CResPLT 44h via 00710B50)
__declspec(noinline) void __fastcall aurora_layered_base_destroy_0070db90(LayeredTexture* self, void*) noexcept {
    base_destroy(self, &aurora_layered_helper_methods_0075fb4c);                 // 0070DB90 (table 0075FB4C)
}
__declspec(noinline) void __fastcall aurora_layered_destroy_0070ddf0(LayeredTexture* self, void*) noexcept {
    self->methods = &aurora_layered_methods_0075fb50;                            // 0070DDF0
    aurora_layered_base_destroy_0070db90(self, nullptr);
}
__declspec(noinline) void* __fastcall aurora_layered_base_deleting_destroy_0070dd40(void* self, void*, Word flags) noexcept {
    return deleting<LayeredTexture>(self, flags, &aurora_layered_base_destroy_0070db90);
}
__declspec(noinline) void* __fastcall aurora_layered_deleting_destroy_0070de00(void* self, void*, Word flags) noexcept {
    return deleting<LayeredTexture>(self, flags, &aurora_layered_destroy_0070ddf0);
}
__declspec(noinline) void __fastcall aurora_layered_set_ref_0070dbf0(LayeredTexture* self, void*, menu_resources::Ref* incoming,
                                                                     Signed flag) noexcept(false) {
    set_ref(self, incoming, flag, 0x0006u, 0x44u, &aurora_plt_resource_ctor_00710b50); // 0070DBF0 (6, 44h)
}
__declspec(noinline) __declspec(safebuffers) LayeredTexture* __fastcall aurora_layered_construct_0070dd60(
    LayeredTexture* self, void*, menu_resources::Ref incoming, Signed mode) noexcept(false) {
    return construct(self, &incoming, mode, &aurora_layered_helper_methods_0075fb4c, &aurora_layered_methods_0075fb50,
                     &aurora_layered_set_ref_0070dbf0, &aurora_layered_base_destroy_0070db90);   // 0070DD60 (0073A8D8)
}
// 0070DA60 CAuroraLayeredTexture::GetTextureAttrib RET 0Ch -> CResPLT 00710B90.
__declspec(noinline) Signed __fastcall aurora_layered_attrib_0070da60(LayeredTexture* self, void*, Word* width, Word* height,
                                                                      Word* third) {
    if (!demanded(self->resource)) return 0;                                     // 0070DA63..72 / 0070DA8F
    return static_cast<Signed>(aurora_plt_attrib_00710b90(self->resource, nullptr, width, height, third)); // 0070DA74..86
}
// 0070DAA0 CAuroraLayeredTexture::Load: tag 0006 after w*h 16-bit cells.
__declspec(noinline) Signed __fastcall aurora_layered_load_0070daa0(LayeredTexture* self, void*) noexcept(false) {
    if (!c2da_current_resource_demand_00409b10(self->resource, nullptr)) return 0; // 0070DAA6..B0 -> 0070DADD
    Word width = 0, height = 0, third = 0;                                       // D2 (0070DAE4 reads)
    (void)aurora_plt_attrib_00710b90(self->resource, nullptr, &width, &height, &third); // 0070DAB2..C4
    unsigned char* const data = static_cast<unsigned char*>(static_cast<void*>(
        aurora_processed_pointer_00712f90(self->resource, nullptr)));            // 0070DAC9..CC ([+3Ch])
    if (!data) {                                                                 // 0070DAD1..D3
        c2da_release_demand_00409b80(self->resource, nullptr);                   // 0070DAD5..D8
        return 0;
    }
    unsigned char* const tag = data + width * height * 2u + 4u;                  // 0070DAE4..ED
    store<LayeredTexture*>(tag, 2, self);                                        // 0070DAF1
    store<std::uint16_t>(tag, 0, 0x0006);                                        // 0070DAF4
    return 1;
}
// 0070DB10 CAuroraLayeredTexture::GetPtrToResHelperInfo: pixels + w*h*2 + 4, or 0.
__declspec(noinline) void* __fastcall aurora_layered_info_0070db10(LayeredTexture* self, void*) {
    if (!demanded(self->resource)) return nullptr;                               // 0070DB16..25 / 0070DB82
    unsigned char* const data = demanded(self->resource)
        ? static_cast<unsigned char*>(static_cast<void*>(aurora_processed_pointer_00712f90(self->resource, nullptr)))
        : nullptr;                                                               // 0070DB27..45
    Word width = 0, height = 0, third = 0;                                       // D2 (0070DB6F reads)
    if (demanded(self->resource))
        (void)aurora_plt_attrib_00710b90(self->resource, nullptr, &width, &height, &third); // 0070DB47..6A
    return data + width * height * 2u + 4u;                                      // 0070DB6F..78
}
__declspec(noinline) LayeredTexture* __cdecl aurora_layered_target_0070e310(String* name) noexcept(false) {
    return getter<LayeredTexture>(name, &aurora_layered_construct_0070dd60, &aurora_layered_info_0070db10,
                                  &aurora_layered_load_0070daa0, 0x0006);        // 0070E310 (info 0070DB10, tag 6)
}

// ======================================================================= DDS 07F1 (CResDDS 38h via 00710EA0)
__declspec(noinline) void __fastcall aurora_compressed_base_destroy_007104d0(CompressedTexture* self, void*) noexcept {
    base_destroy(self, &aurora_compressed_helper_methods_0075fb84);              // 007104D0 (table 0075FB84)
}
__declspec(noinline) void __fastcall aurora_compressed_destroy_00710740(CompressedTexture* self, void*) noexcept {
    self->methods = &aurora_compressed_methods_0075fb88;                         // 00710740
    aurora_compressed_base_destroy_007104d0(self, nullptr);
}
__declspec(noinline) void* __fastcall aurora_compressed_base_deleting_destroy_00710690(void* self, void*, Word flags) noexcept {
    return deleting<CompressedTexture>(self, flags, &aurora_compressed_base_destroy_007104d0);
}
__declspec(noinline) void* __fastcall aurora_compressed_deleting_destroy_00710750(void* self, void*, Word flags) noexcept {
    return deleting<CompressedTexture>(self, flags, &aurora_compressed_destroy_00710740);
}
__declspec(noinline) void __fastcall aurora_compressed_set_ref_00710530(CompressedTexture* self, void*, menu_resources::Ref* incoming,
                                                                        Signed flag) noexcept(false) {
    set_ref(self, incoming, flag, 0x07f1u, 0x38u, &aurora_dds_resource_ctor_00710ea0); // 00710530 (7F1h, 38h)
}
__declspec(noinline) __declspec(safebuffers) CompressedTexture* __fastcall aurora_compressed_construct_007106b0(
    CompressedTexture* self, void*, menu_resources::Ref incoming, Signed mode) noexcept(false) {
    return construct(self, &incoming, mode, &aurora_compressed_helper_methods_0075fb84, &aurora_compressed_methods_0075fb88,
                     &aurora_compressed_set_ref_00710530, &aurora_compressed_base_destroy_007104d0); // 007106B0 (0073AAF8)
}
__declspec(noinline) Signed __fastcall aurora_compressed_load_00710400(CompressedTexture* self, void*) noexcept(false) {
    return load_tagged(self, 0x1au, 0x07f1);                                     // 00710400..2A (sub eax,1Ah)
}
// 007103C0 CAuroraCompressedTexture::GetCompressedTextureAttrib RET 14h -> CResDDS 00710EE0.
__declspec(noinline) Signed __fastcall aurora_compressed_attrib_007103c0(CompressedTexture* self, void*, Word* width,
        Word* height, unsigned char* format, Word* size, float* alpha) {
    if (!demanded(self->resource)) return 0;                                     // 007103C3..D2 / 007103F9
    return static_cast<Signed>(aurora_dds_attrib_00710ee0(self->resource, nullptr, width, height, format, size, alpha)); // 007103D4..F0
}
// 00710430 CAuroraCompressedTexture::ReadTextureHeader(int*,int*,int*,int*,float*) RET 14h: raw 14h-byte header.
__declspec(noinline) Signed __fastcall aurora_compressed_read_header_00710430(CompressedTexture* self, void*, Signed* a,
        Signed* b, Signed* c, Signed* d, float* e) noexcept(false) {
    void* const resource = self->resource;                                       // 00710430
    unsigned char header[0x14];
    if (!resource || !resman_res_read_raw_00409b20(resource, nullptr, 0x14, reinterpret_cast<char*>(header))) return 0; // 00710436..48
    *a = header_value<Signed>(header, 0);                                        // 0071045A..62
    *b = header_value<Signed>(header, 4);                                        // 00710464..6C
    *c = header_value<Signed>(header, 8);                                        // 0071046E..76
    *d = header_value<Signed>(header, 0x0c);                                     // 00710478..80
    *e = header_value<float>(header, 0x10);                                      // 00710456 fld / 00710482 fstp
    return 1;
}
__declspec(noinline) void* __fastcall aurora_compressed_info_00710490(CompressedTexture* self, void*) {
    return info_displaced(self, 0x1au);                                          // 00710490..CE (add eax,-1Ah)
}
__declspec(noinline) CompressedTexture* __cdecl aurora_compressed_target_0070e400(String* name) noexcept(false) {
    return getter<CompressedTexture>(name, &aurora_compressed_construct_007106b0, &aurora_compressed_info_00710490,
                                     &aurora_compressed_load_00710400, 0x07f1);  // 0070E400 (info 00710490, tag 7F1h)
}

// ======================================================================= 4PC 080B (CRes4PC 38h via 00710F90)
__declspec(noinline) void __fastcall aurora_texture16_base_destroy_007108b0(Texture16* self, void*) noexcept {
    base_destroy(self, &aurora_texture16_helper_methods_0075fb8c);               // 007108B0 (table 0075FB8C)
}
__declspec(noinline) void __fastcall aurora_texture16_destroy_00710b20(Texture16* self, void*) noexcept {
    self->methods = &aurora_texture16_methods_0075fb90;                          // 00710B20
    aurora_texture16_base_destroy_007108b0(self, nullptr);
}
__declspec(noinline) void* __fastcall aurora_texture16_base_deleting_destroy_00710a70(void* self, void*, Word flags) noexcept {
    return deleting<Texture16>(self, flags, &aurora_texture16_base_destroy_007108b0);
}
__declspec(noinline) void* __fastcall aurora_texture16_deleting_destroy_00710b30(void* self, void*, Word flags) noexcept {
    return deleting<Texture16>(self, flags, &aurora_texture16_destroy_00710b20);
}
__declspec(noinline) void __fastcall aurora_texture16_set_ref_00710910(Texture16* self, void*, menu_resources::Ref* incoming,
                                                                       Signed flag) noexcept(false) {
    set_ref(self, incoming, flag, 0x080bu, 0x38u, &aurora_4pc_resource_ctor_00710f90); // 00710910 (80Bh, 38h)
}
__declspec(noinline) __declspec(safebuffers) Texture16* __fastcall aurora_texture16_construct_00710a90(
    Texture16* self, void*, menu_resources::Ref incoming, Signed mode) noexcept(false) {
    return construct(self, &incoming, mode, &aurora_texture16_helper_methods_0075fb8c, &aurora_texture16_methods_0075fb90,
                     &aurora_texture16_set_ref_00710910, &aurora_texture16_base_destroy_007108b0); // 00710A90 (0073AB38)
}
__declspec(noinline) Signed __fastcall aurora_texture16_load_007107e0(Texture16* self, void*) noexcept(false) {
    return load_tagged(self, 0x16u, 0x080b);                                     // 007107E0..0A (sub eax,16h)
}
// 00710770 CAurora16BitTexture::GetTextureAttrib RET 10h -> CRes4PC 00710FD0.
__declspec(noinline) Signed __fastcall aurora_texture16_attrib_00710770(Texture16* self, void*, Word* width, Word* height,
                                                                        float* alpha, Signed* format) {
    if (!demanded(self->resource)) return 0;                                     // 00710773..82 / 007107A4
    return static_cast<Signed>(aurora_4pc_attrib_00710fd0(self->resource, nullptr, width, height, alpha, format)); // 00710784..9B
}
// 00710810 CAurora16BitTexture::ReadTextureHeader(int*,int*,float*,int*) RET 10h: raw 10h-byte header.
__declspec(noinline) Signed __fastcall aurora_texture16_read_header_00710810(Texture16* self, void*, Signed* a, Signed* b,
        float* c, Signed* d) noexcept(false) {
    void* const resource = self->resource;                                       // 00710810
    unsigned char header[0x10];
    if (!resource || !resman_res_read_raw_00409b20(resource, nullptr, 0x10, reinterpret_cast<char*>(header))) return 0; // 00710816..28
    *a = header_value<Signed>(header, 0);                                        // 0071083A..42
    *b = header_value<Signed>(header, 4);                                        // 00710844..4C
    *c = header_value<float>(header, 8);                                         // 00710836 fld / 0071084E fstp
    *d = header_value<Signed>(header, 0x0c);                                     // 00710850..58
    return 1;
}
__declspec(noinline) void* __fastcall aurora_texture16_info_00710870(Texture16* self, void*) {
    return info_displaced(self, 0x16u);                                          // 00710870..AE (add eax,-16h)
}
__declspec(noinline) Texture16* __cdecl aurora_texture16_target_0070e220(String* name) noexcept(false) {
    return getter<Texture16>(name, &aurora_texture16_construct_00710a90, &aurora_texture16_info_00710870,
                             &aurora_texture16_load_007107e0, 0x080b);           // 0070E220 (info 00710870, tag 80Bh)
}

// ======================================================================= TPC header (lane-tpc helper type)
// 0070F590 CAuroraProcessedTexture::ReadProcessedTextureHeader(w, h, encoding, compressed, image, alpha, mips) RET 1Ch.
__declspec(noinline) Signed __fastcall aurora_processed_read_header_0070f590(ProcessedTexture* self, void*, Signed* width,
        Signed* height, char* encoding, Signed* compressed, Signed* image, float* alpha, char* mips) noexcept(false) {
    void* const resource = self->resource;                                       // 0070F590
    unsigned char header[0x80];
    if (!resource || !resman_res_read_raw_00409b20(resource, nullptr, 0x80, reinterpret_cast<char*>(header))) return 0; // 0070F599..AE
    Signed w = header_value<std::uint16_t>(header, 8);                           // 0070F5BB (EAX)
    Signed h = header_value<std::uint16_t>(header, 0x0a);                        // 0070F5D4 (ECX)
    *width = w;                                                                  // 0070F5D2
    *height = h;                                                                 // 0070F5D9
    *alpha = header_value<float>(header, 4);                                     // 0070F5C0 fld / 0070F5E7 fstp
    const unsigned char levels = header[0x0d];                                   // 0070F5E3 (BL)
    *mips = static_cast<char>(levels);                                           // 0070F5F0
    const unsigned char flags = header[0x0c];                                    // 0070F5F2
    if (flags & 1u) *encoding = 1;                                               // 0070F5F6..04
    else if (flags & 2u) *encoding = 3;                                          // 0070F609..0E
    else if (flags & 4u) *encoding = 4;                                          // 0070F613..18 (else unchanged)
    const Signed packed = header_value<Signed>(header, 0);                       // 0070F61B
    if (packed != 0) {                                                           // 0070F61F..21
        *image = 0;                                                              // 0070F669..77
        *compressed = packed;                                                    // 0070F67D
        return 1;
    }
    *image = 0;                                                                  // 0070F623..2A (EDX = 0)
    *compressed = 0;                                                             // 0070F62C..33
    for (Signed level = levels; level > 0; --level) {                            // 0070F639..55 (signed JLE)
        *image += static_cast<Signed>(static_cast<signed char>(*encoding)) * h * w; // 0070F641..53 (movsx, imul)
        w >>= 1;                                                                 // 0070F64E SAR
        h >>= 1;                                                                 // 0070F650 SAR
    }
    return 1;
}
}
