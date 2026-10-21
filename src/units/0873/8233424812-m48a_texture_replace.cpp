// lane-m48-a: texture-side callees of Material::ReplaceTexture (lane-m47-3d m47_gob2_tex.cpp 0047B7E0/0047B890):
//   004235F0 AurTextureReplaceSingleReference (cdecl, 6 DWORDs, EAX 0/1)
//   00421750 AurTextureSetLayered            (cdecl, 4 DWORDs, EAX unspecified; consumer void)
//   0041FD00 CAurTextureBasic::SetScheme     (thiscall(layers, count) RET 8) - callee of 00421750
// Evidence: swkotor.exe GOG K1 dumpbin /disasm (lane-m48-a/d/004235F0.txt, d/00421750.txt, d/0041FD00.txt).
// Dispatch: CAurTexture slots +04/+08/+2C through texture_vtable::Head (D21/D41, same pattern as m47_gob2_tex.cpp:75).
// Ref (font::Texture) is the first member of the standard-layout Basic: Basic* views are object-pointer conversions.
// Allocation: 0041FD15 call 006FA7E6 operator new -> kotor_menu_10_19::dependencies::allocate_006fa7e6 (joined onto the
// live SBH allocator). No callee here throws (texture_vtable slot bodies, get/assign/release are non-throwing).
#include <cstdint>
#include <cstring>
#include "d3ac7e1daf-texture_vtable.hpp"
#include "63b483a8b0-texture_join.h"
#include "a14ee97e1a-texture_cells.hpp"
#include "636973bdee-font.hpp"

namespace kotor_menu_10_19 { namespace dependencies { void* allocate_006fa7e6(unsigned int); } } // 006FA7E6 (joined)

extern "C" {
// ---- existing providers (declarations identical to their M47 consumers)
void __cdecl texture_release_00420d70(void* texture);                       // lane-window font_release.cpp
extern volatile std::uint8_t aur_texture_mode_dynamic_0078d400;             // core46-gl-fb (m46_gl_decls.h:49)
texture::U __fastcall texture_list_remove_00445080(texture::List*, void*, texture::U); // texture-epoch texture_x86
__declspec(noinline) std::int32_t __cdecl kotor_stricmp_0070acaf(const char*, const char*); // M40 provider (0070ACAF)
}

namespace {
using texture::U; using texture::I; using texture::Basic; using font::Texture;
inline Basic* as_basic(Texture* ref) { return reinterpret_cast<Basic*>(ref); }
// The two-step "referenced basic" query used at 0042175C..00421766 / 00423603..00423611 / 00423750..0042375B.
inline Texture* referenced_basic(Texture* texture) {
    Texture* const referenced = texture_vtable::head(texture)->common.s08(texture, nullptr); // call [eax+8]
    return texture_vtable::head(referenced)->s04(referenced, nullptr);                       // call [edx+4]
}
inline int compare(const char* a, const char* b) { return kotor_stricmp_0070acaf(a, b); } // call 0070ACAF (_stricmp)
} // namespace

extern "C" {
// 0041FD00 CAurTextureBasic::SetScheme(layers, count): 256-entry DWORD table at [this+3C] (allocated 0x400 bytes on
// first use), entry i = layers[i] for i < count (signed) else 0; word [this+CC] = count. EAX unspecified.
void __fastcall texture_basic_set_scheme_0041fd00(Basic* self, void* /*unused_edx*/, const U* layers, I count) {
    if (self->components == nullptr)                                           // 0041FD08..0041FD0E
        self->components = static_cast<U*>(kotor_menu_10_19::dependencies::allocate_006fa7e6(0x400u)); // 0041FD10..0041FD1D
    for (I index = 0; index < 0x100; ++index)                                  // 0041FD69..0041FE5B (unrolled x8, [esi+3C] reloaded per store)
        self->components[index] = (index < count) ? layers[index] : 0u;        // jge -> xor
    self->component_count = static_cast<std::int16_t>(count);                  // 0041FE62 mov word ptr [esi+0CCh],dx
}                                                                              // 0041FE6F ret 8

// 00421750 AurTextureSetLayered(texture, name, layers, count).
void __cdecl aur_texture_set_layered_00421750(void* texture_word, const char* name, const U* layers, I count) {
    if (texture_word == nullptr) return;                                       // 00421750..00421756
    Basic* const basic = as_basic(referenced_basic(static_cast<Texture*>(texture_word))); // 0042175C..00421769 esi
    if (name != nullptr && *name != '\0') {                                    // 0042176B..00421776
        char* out = basic->derived;                                            // 00421778 lea edx,[esi+98h] (inline strcpy)
        for (const char* in = name;; ++in, ++out) { *out = *in; if (*in == '\0') break; } // 00421780..00421788
    }
    texture_basic_set_scheme_0041fd00(basic, nullptr, layers, count);         // 0042178A..00421796
    basic->f70 = 4;                                                            // 004217AE mov dword ptr [esi+70h],4
    const U hash = texture_hash_0041f4d0(basic->derived,                       // 0042179B..004217B7 push [esi+3C], movzx [esi+CC], esi+98
        static_cast<I>(static_cast<std::uint16_t>(basic->component_count)), basic->components);
    basic->hash = static_cast<std::uint16_t>(hash);                            // 004217C1 mov word ptr [esi+0D0h],ax
    texture_resource::remove_ordered_00421650(basic, nullptr);                                 // 004217BF..004217C8
    texture_add_ordered_00421560(basic, nullptr);                              // 004217CD..004217CF
    texture_join::add_unique_0046bd10(&texture_cells::aur_refresh_textures_007a4770, nullptr,
                                      reinterpret_cast<Texture*>(basic));     // 004217D4..004217DA (ECX = 007A4770)
}

// 004235F0 AurTextureReplaceSingleReference(texture, old_name, new_name, layered_name, layers, count) -> 0 / 1.
U __cdecl aur_texture_replace_single_reference_004235f0(void* texture_word, const char* old_name, const char* new_name,
                                                        const char* layered_name, const U* layers, I count) {
    Texture* const texture = static_cast<Texture*>(texture_word);
    if (texture == nullptr) return 0;                                          // 004235F4..00423602
    Texture* const old_basic = referenced_basic(texture);                      // 00423603..00423614 edi
    if (old_basic == nullptr) return 0;                                        // 00423616..00423618
    Basic* const old_view = as_basic(old_basic);
    if (old_name != nullptr && compare(old_name, old_view->name) != 0) return 0; // 0042361E..00423635
    const std::uint8_t environment = texture_vtable::head(texture)->common.s2c(texture, nullptr); // 0042363B..00423642 -> [esp+13h]
    if (old_view->d3 == 0) aur_texture_mode_dynamic_0078d400 = 0;             // 00423646..00423650
    bool released = false;                                                     // 0042365B xor bl,bl
    Texture* fresh;
    if (layered_name != nullptr) {                                             // 00423657..0042365F
        char base_name[32];                                                    // [esp+14h] (0x20 bytes)
        char* out = base_name;                                                 // 00423661..00423678 inline strcpy of [edi+78h]
        for (const char* in = old_view->name;; ++in, ++out) { *out = *in; if (*in == '\0') break; }
        if (compare(layered_name, old_view->derived) == 0) {                   // 0042367A..00423690 (jne skips the release)
            Texture* const held = texture->reference.target;                   // 00423692 eax = [esi+10h]
            if (held != nullptr)                                               // 00423698..0042369A
                texture_list_remove_00445080(&held->registrations, nullptr,   // 0042369C..004236A0 ecx = eax+4, push esi+10h
                    static_cast<U>(reinterpret_cast<std::uintptr_t>(&texture->reference)));
            texture->reference.target = nullptr;                               // 004236A6 mov dword ptr [esi],0
            texture_release_00420d70(old_basic);                               // 004236A5..004236AC push edi
            released = true;                                                   // 004236B4 mov bl,1
        }
        fresh = font::get_00423490(base_name, nullptr, layered_name, const_cast<U*>(layers), static_cast<U>(count)); // 004236B6..00423746
    } else if (old_name != nullptr) {                                          // 004236CE..004236D0
        if (compare(old_name, old_view->name) != 0) {                          // 004236D2..004236E1
            aur_texture_mode_dynamic_0078d400 = 1;                             // 00423720
            return 0;                                                          // 00423727..00423730
        }
        Texture* const held = texture->reference.target;                       // 004236E3
        if (held != nullptr)                                                   // 004236E9..004236EB
            texture_list_remove_00445080(&held->registrations, nullptr,       // 004236ED..004236F1
                static_cast<U>(reinterpret_cast<std::uintptr_t>(&texture->reference)));
        texture->reference.target = nullptr;                                   // 004236F7
        texture_release_00420d70(old_basic);                                   // 004236F6..004236FD
        released = true;                                                       // 00423714 mov bl,1
        fresh = font::get_00423490(old_name, nullptr, new_name, const_cast<U*>(layers), static_cast<U>(count)); // 00423702..00423716
    } else {
        fresh = font::get_00423490(old_view->name, nullptr, new_name, const_cast<U*>(layers), static_cast<U>(count)); // 00423731..00423746
    }
    Texture* const fresh_basic = referenced_basic(fresh);                     // 0042374E..0042375B (no null test of fresh)
    if (fresh_basic != nullptr) {                                              // 0042375E..00423760
        as_basic(fresh_basic)->flags[2] = environment;                         // 00423768 mov [eax+0DAh],cl
        if (environment != 0)                                                  // 00423766..0042376E
            texture_join::add_unique_0046bd10(&texture_cells::aur_refresh_textures_007a4770, nullptr, fresh_basic); // 00423770..00423776
    }
    Texture* const fresh_query = texture_vtable::head(fresh)->s04(fresh, nullptr); // 0042377B..0042377F ebp
    aur_texture_mode_dynamic_0078d400 = 1;                                     // 00423786
    if (!released && fresh_query == texture_vtable::head(old_basic)->s04(old_basic, nullptr)) return 0; // 0042378D..00423798
    font::assign_00447fa0(&texture->reference, nullptr, fresh);               // 0042379A..004237A2 (ecx = texture+10h)
    if (!released) texture_release_00420d70(old_basic);                       // 004237A7..004237AC
    if (layers != nullptr && as_basic(fresh_query)->flags[8] != 0) return 0;  // 004237B4..004237C4 [ebp+0E0h]
    return 1;                                                                  // 004237CD
}
} // extern "C"
