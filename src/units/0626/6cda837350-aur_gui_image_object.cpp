// lane-gui-render: CAurGUIImage / CAurGUIImageInternal object family (8-byte object: +0 vptr,
// +4 Material*), its typed method table 0074183C (D21: constant struct of D1 fastcall pointers)
// and the base table 00741834, plus NewCAurGUIImage 0045A260 (the factory every border fill /
// corner / edge image comes from; lane-gui-load calls it as gui_load_new_aur_gui_image_0045a260).
// Unwind maps reproduced with RAII guards (FuncInfo 007634E0, 00763538, 0076355C); throwing
// extern "C" declarations are explicit noexcept(false) so /EHsc keeps the funclets.
#include "1277741e89-gui_render.h"
#include "d65319cf1e-texture_vtable.hpp"   // material_owner::Material (lane-texture-epoch E1, task-17 layout)
#include <cstring>
#include <intrin.h>

using gui_render::I32;
using gui_render::Word;
using material_owner::Material;
using Texture = font::Texture;

extern "C" {
// M40 providers: operator new 006FA7E6 / free 006FA390.
void* __cdecl c2da_directory_new_006fa7e6(std::uint32_t) noexcept(false);
void __cdecl live_free(void*);
// lane-3d-model model_material.cpp: Material::Material 0047B290 join.
void* __fastcall aurora_material_construct_0047b290(void* self, void* unused_edx) noexcept(false);
// CRT _purecall (original 006FB8DE), slot 0 of the abstract base table 00741834.
int __cdecl _purecall(void);
// This lane (aur_gui_image.cpp).
I32 __fastcall kotor_fastcall_0045a090(void*, void*);
I32 __fastcall kotor_fastcall_0045a0a0(void*, void*);
void __fastcall kotor_fastcall_004599b0(void*, void*, Word, Word, Word, Word, Word, Word, const void*, Word);
void __fastcall kotor_fastcall_00459920(void*, void*, I32, I32, I32, I32, Word, Word, const void*, Word);
void __fastcall kotor_fastcall_0045b290(void*, void*, Word, Word, Word, Word, Word, Word, const void*, Word);
void __fastcall kotor_fastcall_00459800(void*, void*, I32, I32, I32, I32, Word, Word, const void*, Word);
std::uint32_t __fastcall kotor_fastcall_0047af00(void* material, void*);
// Defined below.
void __fastcall kotor_fastcall_0045a0b0(void* self, void*) noexcept(false);
void* __fastcall kotor_fastcall_0045b970(void* self, void*, Word flags) noexcept(false);
void __fastcall kotor_fastcall_00459880(void* self, void*, const char* name) noexcept(false);
void __fastcall kotor_fastcall_0045a110(void* self, void*, Word alpha_bits) noexcept(false);
}

namespace kotor_gui_render_cells {
extern volatile I32 gui_buffer_used_007bb4d4;
extern volatile std::uint32_t gui_tid_007bb4d8;
extern void* volatile gui_material_007bb4dc;
}

// ---- typed method tables (D21) -----------------------------------------------------------------
struct AurGuiImageVTable {                         // 0074183C, 10 slots (+28 is the next datum)
    decltype(&kotor_fastcall_0045a0b0) s00;         // 0045A0B0 Draw()
    decltype(&kotor_fastcall_0045b970) s04;         // 0045B970 scalar deleting destructor
    decltype(&kotor_fastcall_00459880) s08;         // 00459880 SetTexture(name)
    decltype(&kotor_fastcall_0045a090) s0c;         // 0045A090 GetImageHeight
    decltype(&kotor_fastcall_0045a0a0) s10;         // 0045A0A0 GetImageWidth
    decltype(&kotor_fastcall_004599b0) s14;         // 004599B0 Draw (normalized)
    decltype(&kotor_fastcall_00459920) s18;         // 00459920 Draw (pixels)
    decltype(&kotor_fastcall_0045a110) s1c;         // 0045A110 Draw(alpha)
    decltype(&kotor_fastcall_0045b290) s20;         // 0045B290 DrawBuffered (normalized)
    decltype(&kotor_fastcall_00459800) s24;         // 00459800 DrawBuffered (pixels)
};
struct AurGuiImageBaseVTable {                     // 00741834: abstract CAurGUIImage (one slot)
    decltype(&_purecall) s00;                       // 006FB8DE
};
static_assert(sizeof(AurGuiImageVTable) == 0x28 && sizeof(AurGuiImageBaseVTable) == 4, "table sizes");
extern "C" const AurGuiImageVTable aur_gui_image_vtable_0074183c = {
    &kotor_fastcall_0045a0b0, &kotor_fastcall_0045b970, &kotor_fastcall_00459880,
    &kotor_fastcall_0045a090, &kotor_fastcall_0045a0a0, &kotor_fastcall_004599b0,
    &kotor_fastcall_00459920, &kotor_fastcall_0045a110, &kotor_fastcall_0045b290,
    &kotor_fastcall_00459800};
extern "C" const AurGuiImageBaseVTable aur_gui_image_base_vtable_00741834 = {&_purecall};

namespace {
void* P(const void* p, unsigned off) {
    void* v;
    _ReadWriteBarrier();
    std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4);
    _ReadWriteBarrier();
    return v;
}
void SP(void* p, unsigned off, const void* v) {
    _ReadWriteBarrier();
    std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4);
    _ReadWriteBarrier();
}
const AurGuiImageVTable* table_of(const void* self) { return static_cast<const AurGuiImageVTable*>(P(self, 0)); }
// 00459140 CAurGUIImage::~CAurGUIImage: mov [ecx],741834h; ret.
void base_destroy(void* self) { SP(self, 0, &aur_gui_image_base_vtable_00741834); }
struct BaseOnUnwind {      // unwind actions 00716950 / 00716990 -> 00459140
    void* self;
    ~BaseOnUnwind() { if (self != nullptr) base_destroy(self); }
};
struct FreeOnUnwind {      // unwind actions 00716958 / 00716970 / 0071697B -> free 006FA390
    void* block;
    ~FreeOnUnwind() { if (block != nullptr) live_free(block); }
};
void* new_material() noexcept(false) {   // new(4Ch); Material::Material (state 1 frees the block)
    void* block = c2da_directory_new_006fa7e6(0x4C);
    if (block == nullptr) return nullptr;
    FreeOnUnwind guard{block};
    void* material = aurora_material_construct_0047b290(block, nullptr);
    guard.block = nullptr;
    return material;
}
Texture* texture_of(void* self) { return static_cast<Material*>(P(self, 4))->textures[0].target; }
}

extern "C" {

// 00459140 CAurGUIImage::~CAurGUIImage (thiscall, RET).
void __fastcall kotor_fastcall_00459140(void* self, void*) { base_destroy(self); }

// 00459790 CAurGUIImageInternal::CAurGUIImageInternal() (thiscall, RET).
void* __fastcall kotor_fastcall_00459790(void* self, void*) noexcept(false) {
    BaseOnUnwind base{self};                                                  // state 0 (004597B1)
    SP(self, 0, &aur_gui_image_vtable_0074183c);                              // 004597B9
    void* const material = new_material();                                   // 004597AF..004597DD
    SP(self, 4, material);                                                    // 004597E3
    base.self = nullptr;
    return self;                                                              // 004597E6
}

// 004598A0 CAurGUIImageInternal::CAurGUIImageInternal(char const* name) (thiscall, RET 4).
void* __fastcall kotor_fastcall_004598a0(void* self, void*, const char* name) noexcept(false) {
    BaseOnUnwind base{self};                                                  // state 0 (004598C1)
    SP(self, 0, &aur_gui_image_vtable_0074183c);                              // 004598C9
    void* const material = new_material();                                   // 004598BF..004598EF
    SP(self, 4, material);                                                    // 00459901
    static_cast<Material*>(material)->initialize_textures_0047b560(name, nullptr, nullptr, nullptr);   // 004598EF..00459904
    base.self = nullptr;
    return self;                                                              // 0045990D
}

// 00459880 CAurGUIImageInternal::SetTexture(char const* name) (thiscall, RET 4).
void __fastcall kotor_fastcall_00459880(void* self, void*, const char* name) noexcept(false) {
    static_cast<Material*>(P(self, 4))->initialize_textures_0047b560(name, nullptr, nullptr, nullptr);   // 00459880..0045988E
}

// 0045B200 CAurGUIImageInternal::~CAurGUIImageInternal (thiscall, RET).
void __fastcall kotor_fastcall_0045b200(void* self, void*) noexcept(false) {
    using namespace kotor_gui_render_cells;
    BaseOnUnwind base{self};                                                  // state 0 (0045B227)
    SP(self, 0, &aur_gui_image_vtable_0074183c);                              // 0045B21E
    if (gui_tid_007bb4d8 == kotor_fastcall_0047af00(P(self, 4), nullptr)) {  // 0045B224..0045B23A
        kotor_cdecl_0045a170(kotor_gui_render_cells::gui_image_alpha_0078e43c);   // 0045B23C..0045B242
        gui_tid_007bb4d8 = 0;                                                 // 0045B24A
        gui_material_007bb4dc = nullptr;                                      // 0045B254
    }
    Material* const material = static_cast<Material*>(P(self, 4));          // 0045B25E
    if (material != nullptr) {                                                // 0045B261
        material->~Material();                                                // 0045B267 Material::~Material 0047B980
        live_free(material);                                                  // 0045B26C..0045B26D
    }
    base.self = nullptr;
    base_destroy(self);                                                       // 0045B279 (inlined 00459140)
}

// 0045B970 CAurGUIImageInternal scalar deleting destructor (thiscall, RET 4).
void* __fastcall kotor_fastcall_0045b970(void* self, void*, Word flags) noexcept(false) {
    kotor_fastcall_0045b200(self, nullptr);                                   // 0045B973
    if ((flags & 1u) != 0) live_free(self);                                   // 0045B978..0045B980
    return self;                                                              // 0045B988
}

// 0045A0B0 CAurGUIImageInternal::Draw() (thiscall, RET): own vtbl+18(0, 0, width, height, 0, 0,
// (-1,-1,-1), 1.0). The table pointer is read before the metric calls (0045A0BD).
void __fastcall kotor_fastcall_0045a0b0(void* self, void*) noexcept(false) {
    const AurGuiImageVTable* const table = table_of(self);                    // 0045A0BD
    const Word none[3] = {0xBF800000u, 0xBF800000u, 0xBF800000u};             // 0045A0CB..0045A0DB
    Texture* t = texture_of(self);                                            // 0045A0B8..0045A0BB
    const I32 height = texture_vtable::head(t)->common.s4c(t, nullptr);       // 0045A0E9
    const I32 width = texture_vtable::head(t)->common.s48(t, nullptr);        // 0045A0F1
    table->s18(self, nullptr, 0, 0, width, height, 0, 0, none, 0x3F800000u);  // 0045A0F5..0045A0FB
}

// 0045A110 CAurGUIImageInternal::Draw(float alpha) (thiscall, RET 4).
void __fastcall kotor_fastcall_0045a110(void* self, void*, Word alpha_bits) noexcept(false) {
    const AurGuiImageVTable* const table = table_of(self);                    // 0045A121
    const Word none[3] = {0xBF800000u, 0xBF800000u, 0xBF800000u};             // 0045A12B..0045A13B
    Texture* t = texture_of(self);                                            // 0045A11C..0045A11F
    const I32 height = texture_vtable::head(t)->common.s4c(t, nullptr);       // 0045A149
    const I32 width = texture_vtable::head(t)->common.s48(t, nullptr);        // 0045A151
    table->s18(self, nullptr, 0, 0, width, height, 0, 0, none, alpha_bits);   // 0045A155..0045A15B
}

// 0045A260 NewCAurGUIImage(char const* name), cdecl: new(8); name == 0 -> 00459790 else 004598A0;
// both EH states free the 8-byte block (unwind 00716970 / 0071697B).
void* __cdecl kotor_cdecl_0045a260(const char* name) noexcept(false) {
    void* const block = c2da_directory_new_006fa7e6(8);                       // 0045A27A..0045A27C
    if (block == nullptr) return nullptr;                                     // 0045A28C..0045A292 / 0045A2AB..0045A2B5
    FreeOnUnwind guard{block};
    void* const image = (name == nullptr) ? kotor_fastcall_00459790(block, nullptr)        // 0045A296
                                          : kotor_fastcall_004598a0(block, nullptr, name); // 0045A2BA
    guard.block = nullptr;
    return image;
}

}   // extern "C"

// lane-gui-load consumer names (gui_params.cpp / listbox_image.cpp) joined onto this owner.
#pragma comment(linker, "/alternatename:_gui_load_new_aur_gui_image_0045a260=_kotor_cdecl_0045a260")
