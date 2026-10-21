// lane-m47-3d fork gob1 (texture-side unit): GetAllNamedTextures 00446AB0 (consumer core46-gob-tree, declaration
// m46_gob_externs.h:42) plus the two callees it/its sibling walker need that no M46 object defines:
// Material::SetDownSampling 0047AF10 (called by PartSetCanDownSample 004434A0, m47_gob1.cpp) and AurTextureMessage
// 00422D20 (called by 0047AF10). Separate unit because the texture-epoch include chain (texture_vtable.hpp) is not on
// the core46 roots; the m46_gob_externs.h declaration is reproduced verbatim below (same decorated symbol).
// Evidence: dumpbin /disasm of swkotor.exe (GOG K1, image base 00400000); instruction addresses cited per behaviour.
// Texture virtual calls use the published typed tables texture_vtable::head (lane-texture-epoch; +08 GetReferenced,
// +04 basic query, +40 GetName) and the Part table aurora_vt::VTable_00740ae8 (+2C) - D21/D41, no fn-pointer casts.
#include "283ae0c931-texture_vtable.hpp"   // texture_vtable::Head, texture_cells::aur_refresh_textures_007a4770,
                                // texture_parse_field_00422390 (extern "C"), texture_join::add_unique_0046bd10
#include "7ee14b5ec3-model_vtables.hpp"    // aurora_vt::VTable_00740ae8 (Part family, +2C Slot0)
#include <cstdint>

namespace {
using U = std::uint32_t; using I = std::int32_t; using U8 = std::uint8_t;
using List = kotor_reconstruction::ArrayMutations;
using Ref = font::Texture;
template <class T> inline T* ptr(U w) { return reinterpret_cast<T*>(static_cast<std::uintptr_t>(w)); }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
template <class T> inline T& field(void* object, U offset) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset); }
inline const aurora_vt::VTable_00740ae8* part_table(void* part) {   // mov eax,[part]: live table reload per call
    return *static_cast<const aurora_vt::VTable_00740ae8* const volatile*>(part);
}
}

extern "C" {
// ---- consumer declaration (verbatim from stage 3d/core46/m46_gob_externs.h:42; PointerList == ArrayMutations)
void __cdecl get_all_named_textures_00446ab0(void* part, kotor_reconstruction::ArrayMutations* out, U name);
// ---- existing M46 providers (identical declarations to their owners)
__declspec(noinline) std::int32_t __cdecl kotor_stricmp_0070acaf(const char*, const char*);   // font_txi.hpp:78
int __cdecl format_006fadb0(char*, const char*, ...);                                          // m46_scene_externs_c1.h:21 (tpc-joins)
// ---- authored here (new names)
U __cdecl aur_texture_message_00422d20(Ref* texture, const char* text);
void __fastcall material_set_down_sampling_0047af10(void* material, void* unused_edx, I value);
}

namespace {
// One level of GetAllNamedTextures (00446AB3..00446AFA, repeated inline for levels 1..6):
//   mesh = part->+2C; if mesh { t = Material(mesh+44h)->textures[0]; r = t->GetReferenced(+08); if (r->+04 != 0 &&
//   stricmp(r->GetName(+40), name) == 0) AddUnique(out, r) } ; then children.
void named_level(void* part, List* out, U name, int depth) {
    const U mesh = part_table(part)->s02c(part, nullptr);                       // 00446ABE call [eax+2Ch]
    if (mesh != 0) {                                                            // 00446AC1..00446AC3
        void* const material = field<void*>(ptr<void>(mesh), 0x44);             // 00446AC5 mov ecx,[eax+44h]
        Ref* const texture = field<Ref*>(material, 0x00);                       // 00446AC8 mov ecx,[ecx] (textures[0])
        Ref* const referenced = texture_vtable::head(texture)->common.s08(texture, nullptr);   // 00446ACA..00446ACC call [edx+8]
        if (texture_vtable::head(referenced)->s04(referenced, nullptr) != nullptr) {          // 00446AD1..00446ADA call [eax+4]
            const char* const own = texture_vtable::head(referenced)->common.s40(referenced, nullptr);   // 00446AE0..00446AE5 call [edx+40h]
            if (kotor_stricmp_0070acaf(own, ptr<const char>(name)) == 0)       // 00446AE2/00446AE8..00446AF3 (stricmp(own, name))
                texture_join::add_unique_0046bd10(out, nullptr, referenced);    // 00446AF5..00446AFA
        }
    }
    List* const kids = &field<List>(part, 0x30);                               // +30 data, +34 count (re-read per iteration)
    for (I i = 0; i < kids->size; ++i) {
        void* const c = ptr<void>(ptr<const U>(kids->data)[i]);                 // 00446B17..00446B1A / 00446D68..00446D6C
        if (depth == 6) get_all_named_textures_00446ab0(c, out, name);          // 00446D60..00446D79 (direct recursive call)
        else named_level(c, out, name, depth + 1);
    }
}
}

extern "C" {
// 00446AB0 GetAllNamedTextures(Part*, CExoArrayList<CAurTexture*>*, char*) cdecl, plain RET; levels 0..6 inlined by the
// original compiler, level-6 children call 00446AB0 again (00446D71) - same visit order and callee sequence.
void __cdecl get_all_named_textures_00446ab0(void* part, List* out, U name) { named_level(part, out, name, 0); }

// 00422D20 AurTextureMessage(CAurTexture*, char*) cdecl: null -> 0 (00422D24..00422D2A); r = t->+08 (00422D2E);
// b = r->+04 (00422D35); b null -> 0 (00422D3A..00422D3F, EAX = b); b->ParseField(text) 00422390 (00422D40..00422D47);
// AddUnique(007A4770 refresh list, b) (00422D4C..00422D52); return 1 (00422D57).
U __cdecl aur_texture_message_00422d20(Ref* texture, const char* text) {
    if (texture == nullptr) return 0;                                           // 00422D24..00422D2A
    Ref* const referenced = texture_vtable::head(texture)->common.s08(texture, nullptr);   // 00422D2B..00422D2E
    Ref* const basic = texture_vtable::head(referenced)->s04(referenced, nullptr);         // 00422D31..00422D38
    if (basic == nullptr) return 0;                                             // 00422D3A..00422D3F
    // Basic begins with its Ref prefix (first member, standard layout): pointer-interconvertible (texture_slots.cpp precedent).
    (void)texture_parse_field_00422390(reinterpret_cast<texture::Basic*>(basic), nullptr, text);   // 00422D40..00422D47
    texture_join::add_unique_0046bd10(&texture_cells::aur_refresh_textures_007a4770, nullptr, basic);         // 00422D4C..00422D52 (ecx=7A4770)
    return 1;                                                                   // 00422D57
}

// 0047AF10 Material::SetDownSampling(int value) thiscall RET 4 (0047AF3E) -> D1 fastcall(self, edx unused, value).
// sprintf(buf[14h], "downsamplemax %d" 007430AC, value) (0047AF18..0047AF25); AurTextureMessage([this+0] =
// textures[0] target, buf) (0047AF2A..0047AF32); result discarded.
void __fastcall material_set_down_sampling_0047af10(void* material, void*, I value) {
    static constexpr char kFormat_007430ac[] = "downsamplemax %d";             // .rdata 007430AC
    char buffer[0x14];                                                          // sub esp,14h; buffer at [esp+8] after 2 pushes
    (void)format_006fadb0(buffer, kFormat_007430ac, value);                     // 0047AF18..0047AF25
    (void)aur_texture_message_00422d20(field<Ref*>(material, 0x00), buffer);    // 0047AF2A..0047AF32
}
}
