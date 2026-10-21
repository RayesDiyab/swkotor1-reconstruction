// lane-m47-3d fork gob2 (texture-side Gob-tree callees): Material::ReplaceTexture 0047B7E0 / 0047B890 and
// AurTextureSetAttributes 00422CE0 (swkotor.exe GOG K1, image base 00400000). Every statement cites the original
// instruction(s) (d/<addr>.txt, local disassembly). Consumer ABIs: M46 stage 3d/core46/m46_gob_externs.h
// (extern "C"; decoration depends only on name, convention and stack bytes).
//
// Dispatch (D21/D41): CAurTexture slots +04/+08 through the published texture_vtable::Head (lane-texture-epoch).
// Allocator family: none (no allocation here). Throws: no callee here throws (D30-FINAL unwind note).
#include <cstdint>
#include <cstring>
#include "283ae0c931-texture_vtable.hpp"   // texture_vtable::Head (+04 basic query, +08 referenced), texture::init_00422af0
#include "49236907d8-texture_join.h"       // texture_join::add_unique_0046bd10
#include "708974864c-texture_cells.hpp"    // texture_cells::aur_refresh_textures_007a4770
#include "69638710e8-font.hpp"             // font::Safe / font::Texture, font::reference_00423a60, font::assign_00447fa0

extern "C" {
// ---- existing providers (identical declarations to the M46 consumer headers)
// core46-gl-fb (m46_gl_decls.h:24): CAurTexture::IsNull 00420600, thiscall, AL result.
std::uint8_t __fastcall aur_texture_is_null_00420600(void* texture, void* unused_edx);
// lane-window font_release.cpp body (m46_gob_externs.h:62 spelling): AurTextureRelease 00420D70, cdecl.
void __cdecl texture_release_00420d70(void* texture);
// ---- NEW undefined externals (no body in M46 or any lane; texture area, not authored by this fork)
// 004235F0 AurTextureReplaceSingleReference: cdecl, 6 DWORDs (0047B850 / 0047B8BA add esp,18h), EAX result tested.
texture::U __cdecl aur_texture_replace_single_reference_004235f0(void* texture, const char* old_name,
    const char* new_name, const char* layered_name, const texture::U* layer_data, texture::I layer_count);
// 00421750 AurTextureSetLayered: cdecl, 4 DWORDs (0047B862 / 0047B87F add esp,10h), result unused.
void __cdecl aur_texture_set_layered_00421750(void* texture, const char* name, const texture::U* layer_data,
    texture::I layer_count);

// Material::ReplaceTexture(char*, char*, char*, int*, int) 0047B7E0: __thiscall RET 14h (0047B869 / 0047B886).
// Material+00 is its SafePointer<CAurTexture> (ECX = [esi] at 0047B7E5, SafePointer::operator= with ECX = esi at
// 0047B80B). Consumer m46_gob_externs.h types the third stack word as U; it is the replacement name (pushed to
// 00423A60 at 0047B801) - same DWORD, header diff parts/gob2-m46_gob_externs.h.diff.
void __fastcall material_replace_texture_0047b7e0(void* material, void* /*unused_edx*/, const char* old_name,
    const char* new_name, const char* replacement, const texture::U* layer_data, texture::I layer_count) {
    font::Safe* const slot = static_cast<font::Safe*>(material);           // 0047B7E3 mov esi,ecx
    if (aur_texture_is_null_00420600(slot->target, nullptr) != 0) {       // 0047B7E5..0047B7EF (ECX = [esi], no null test)
        if (replacement == nullptr) return;                                // 0047B7F1..0047B7F7 je 0047B882
        font::Texture* const old = slot->target;                           // 0047B7FD mov edi,[esi]
        font::assign_00447fa0(slot, nullptr, font::reference_00423a60(replacement, nullptr)); // 0047B7FF..0047B80D (push 0; push name)
        texture_release_00420d70(old);                                     // 0047B812..0047B818
    }
    // 0047B81B..0047B835: inline strlen of new_name (the length is stored into the dead third-argument cell).
    if (new_name != nullptr && std::strlen(new_name) != 0) {
        // 0047B837..0047B850: push p5, p4, new, new, old, [esi]
        if (aur_texture_replace_single_reference_004235f0(slot->target, old_name, new_name, new_name, layer_data,
                                                          layer_count) == 0)
            return;                                                        // 0047B853..0047B855 je 0047B882
        aur_texture_set_layered_00421750(slot->target, new_name, layer_data, layer_count); // 0047B857..0047B862
        return;                                                            // 0047B865..0047B869
    }
    aur_texture_set_layered_00421750(slot->target, new_name, layer_data, layer_count);     // 0047B86C..0047B87F
}                                                                          // 0047B882..0047B886 ret 14h

// Material::ReplaceTexture(char*, char*) 0047B890: __thiscall RET 8 (0047B8BE / 0047B8ED). The first path leaves
// 004235F0's EAX in EAX at the RET; the consumer declares void (result unused by every caller).
void __fastcall material_replace_texture_0047b890(void* material, void* /*unused_edx*/, const char* name,
    const char* alternate) {
    font::Safe* const slot = static_cast<font::Safe*>(material);           // 0047B891 mov esi,ecx
    font::Texture* const current = slot->target;                           // 0047B893 mov ecx,[esi]
    if (current != nullptr && aur_texture_is_null_00420600(current, nullptr) == 0) { // 0047B895..0047B8A0
        (void)aur_texture_replace_single_reference_004235f0(slot->target, name, alternate, nullptr, nullptr, 0);
                                                                           // 0047B8A2..0047B8BA (edx = [esi] re-read; push 0,0,0,alt,name,tex)
        return;                                                            // 0047B8BD ret 8
    }
    if (alternate == nullptr) return;                                      // 0047B8C1..0047B8C7
    font::Texture* const old = slot->target;                               // 0047B8CA mov edi,[esi]
    font::assign_00447fa0(slot, nullptr, font::reference_00423a60(name, alternate)); // 0047B8CC..0047B8DD
    texture_release_00420d70(old);                                         // 0047B8E2..0047B8E8
}

// AurTextureSetAttributes(CAurTexture*, char*) 00422CE0: __cdecl, EAX 0 / 1. The consumer (m46_gob_externs.h:69)
// declares void (its caller 00446E6E ignores EAX); the original EAX is returned here (same symbol, header diff).
texture::U __cdecl aur_texture_set_attributes_00422ce0(font::Texture* texture, const char* attributes) {
    if (texture == nullptr) return 0;                                      // 00422CE0..00422CEA
    font::Texture* const referenced = texture_vtable::head(texture)->common.s08(texture, nullptr); // 00422CEB..00422CEE call [eax+8]
    font::Texture* const basic = texture_vtable::head(referenced)->s04(referenced, nullptr);       // 00422CF1..00422CF5 call [edx+4]
    // 00422CFA..00422D01 CAurTextureBasic::Init(this = basic, attributes); EAX dropped. Ref is the first member of
    // the standard-layout Basic (pointer-interconvertible; same precedent as 3d/construct/texture_get.cpp:48).
    (void)texture::init_00422af0(reinterpret_cast<texture::Basic*>(basic), nullptr, attributes);
    texture_join::add_unique_0046bd10(&texture_cells::aur_refresh_textures_007a4770, nullptr, basic); // 00422D06..00422D0C (ECX = 007A4770)
    return 1;                                                              // 00422D11 mov eax,1
}
}
