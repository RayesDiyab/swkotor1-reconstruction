// lane-gui-render rev2: frame-1 GUI text path (D62).
//   NewCAurGUIString 0045BDF0 -> CAurGUIStringInternal ctor 0045B990 / dtors 0045BC70 / 0045BE80,
//   the complete typed method table 00741878 (32 slots, D21), its small slot bodies, and
//   CSWGuiText::Initialize 00417310 + CSWGuiTextParams::operator= 004171E0 (DrawPCBG label path).
// Object CAurGUIStringInternal (0x48, 0045BE09 push 48h): +00 vptr, +04/+08 position, +0C/+10 size
// (ints), +14 char* text, +18 SafePointer<CAurTexture> font, +1C Vector colour, +28 CExoArrayList<int>
// (wrapped line starts), +34 CExoArrayList<int> (line lengths), +40 float scale, +44 short cursor,
// +46 byte alignment.
// D30-FINAL: x87 wide intermediates are double; float only at FSTP m32; _ftol is the M40 port.
// Unwind: the ctor and GetIdealPixelHeight reproduce their FuncInfo maps with RAII guards; the
// destructor body has no throwing callee (free/release/remove) and carries no guards.
#include "881508e8f6-gui_render.h"
#include "6ce637f12f-gui_cells.h"
#include "5fa24cf659-texture_vtable.hpp"   // font::Texture/Safe, texture::FontInfo, texture_vtable::head (E1)
#include "ac41715f26-growth.h"             // menu_list_growth::allocate_005e03c0 (M40 viewport-list-growth.obj)
#include <cstring>
#include <intrin.h>

using gui_render::I32;
using gui_render::Word;
using List = kotor_reconstruction::ArrayMutations;
using Texture = font::Texture;

namespace gui_wrap_draw {   // M40 gui-wrap.obj / gui-draw.obj (slot +4C WrapStrings, +7C Draw)
void __fastcall wrap_0045a2f0(void*, void*, unsigned int);
void __fastcall draw_0045a850(void*, void*, unsigned int);
}
namespace kotor_renderer_init {
void __stdcall glDisable_0073d22c(std::uint32_t);
void __stdcall glEnable_0073d390(std::uint32_t);
extern void __stdcall glBegin_0073d2d0(std::uint32_t);
extern void __stdcall glEnd_0073d2c4();
extern void __stdcall glColor4ubv_0073d2e8(const std::uint8_t*);
extern void __stdcall glVertex4f_0073d324(float, float, float, float);
}
namespace kotor_renderer_cells {   // lane-gui-draw writers gl_enum_cells.cpp
extern std::uint32_t enable_0078d448;   // GL_BLEND
extern std::uint32_t enable_0078d458;   // GL_TEXTURE_2D
}

extern "C" {
void* __cdecl c2da_directory_new_006fa7e6(std::uint32_t) noexcept(false);   // M40 (006FA7E6)
void __cdecl live_free(void*);                                                // M40 (006FA390)
unsigned __int64 __cdecl unresolved_ftol2_006fae8c();                         // M40 (_ftol 006FAE8C)
void __fastcall texture_list_remove_00445080(List*, void*, std::uint32_t);   // lane-texture-epoch
void __cdecl texture_release_00420d70(Texture*);                              // lane-window font_release.cpp
void __fastcall aurora_texture_safe_pointer_destroy_00422380(void* self, void*);   // lane-3d-model
void __fastcall exo_array_list_free_0047abc0(void* list, void*);              // lane-3d-render
List* __fastcall int_list_copy_construct_0046d250(List* list, void*, const List* source) noexcept(false);   // consumers: lane-3d-*; MISSING body
List* __fastcall int_list_assign_00438bb0(List* list, void*, const List* source) noexcept(false);          // MISSING (CExoArrayList<int>::operator= 00438BB0)
void __fastcall gui_load_resref_copy_to_string_00405fb0(const void* ref, void*, char* out);   // lane-gui-load
void __fastcall gui_load_text_params_set_text_object_00415f70(void*, void*, void*) noexcept(false);   // lane-gui-load
void __fastcall gui_load_text_wrap_00414e90(void*, void*) noexcept(false);    // lane-gui-load
const char* __fastcall WindowCStr005e5670(const void* string, void*);         // M40 window-native-strings.obj
void* __fastcall async_assign_string_005e5c50(void* self, void*, const void* source) noexcept(false);   // M40 current-assign-join.obj
std::uint32_t __fastcall aurora_txi_size_00712fc0(void*, void*);              // lane-tpc (00712FC0 mov eax,[ecx+38h])
}
// font::reference_00423a60 / font::assign_00447fa0: task-33 font.hpp (via texture_vtable.hpp).

// ---- slot bodies (declarations for the typed table) --------------------------------------------
extern "C" {
void __fastcall kotor_fastcall_0045bbd0(void* self, void*) noexcept(false);
void* __fastcall kotor_fastcall_0045be80(void* self, void*, Word flags) noexcept(false);
void* __fastcall kotor_fastcall_0045bae0(void* self, void*, void* out);
void __fastcall kotor_fastcall_0045bb00(void* self, void*, const void* color);
std::uint32_t __fastcall kotor_fastcall_0045bb20(void* self, void*);
void __fastcall kotor_fastcall_00458f10(void* self, void*, Word size) noexcept(false);
void __fastcall kotor_fastcall_00458f70(void* self, void*, const char* text) noexcept(false);
void __fastcall kotor_fastcall_0045bb30(void* self, void*, Word alignment);
std::uint8_t __fastcall kotor_fastcall_005d5fe0(void* self, void*);
void __fastcall kotor_fastcall_0045bb40(void* self, void*, Word vertical);
std::uint32_t __fastcall kotor_fastcall_0045bb60(void* self, void*);
void __fastcall kotor_fastcall_0045bb70(void* self, void*, Word horizontal);
std::uint32_t __fastcall kotor_fastcall_0045bb90(void* self, void*);
void __fastcall kotor_fastcall_0045bba0(void* self, void*, Word index);
std::uint16_t __fastcall kotor_fastcall_0045bbb0(void* self, void*);
void __fastcall kotor_fastcall_0045bbc0(void* self, void*);
char* __fastcall kotor_fastcall_0045bc60(void* self, void*);
void __fastcall kotor_fastcall_0045bd70(void* self, void*, const char* name) noexcept(false);
I32 __fastcall kotor_fastcall_00459610(void* self, void*);
I32 __fastcall kotor_fastcall_0045b7a0(void* self, void*, I32 lines) noexcept(false);
void __fastcall kotor_fastcall_00458fe0(void* self, void*, float* x, float* y);
void __fastcall kotor_fastcall_0045bbe0(void* self, void*, I32* x, I32* y);
void __fastcall kotor_fastcall_00459030(void* self, void*, Word x_bits, Word y_bits);
void __fastcall kotor_fastcall_0045bc00(void* self, void*, I32 x, I32 y);
void __fastcall kotor_fastcall_00459090(void* self, void*, float* w, float* h);
void __fastcall kotor_fastcall_0045bc20(void* self, void*, I32* w, I32* h);
void __fastcall kotor_fastcall_004590e0(void* self, void*, Word w_bits, Word h_bits);
void __fastcall kotor_fastcall_0045bc40(void* self, void*, I32 w, I32 h);
void __fastcall kotor_fastcall_00459630(void* self, void*, Word, Word, Word, Word);
}

struct AurGuiStringVTable {                        // 00741878, 32 slots (+80 is 007418F8, the camera table)
    decltype(&kotor_fastcall_0045bbd0) s00;   // 0045BBD0 Draw()
    decltype(&kotor_fastcall_0045be80) s04;   // 0045BE80 scalar deleting destructor
    decltype(&kotor_fastcall_0045bae0) s08;   // 0045BAE0 GetColor
    decltype(&kotor_fastcall_0045bb00) s0c;   // 0045BB00 SetColor
    decltype(&kotor_fastcall_0045bb20) s10;   // 0045BB20 GetStringSize
    decltype(&kotor_fastcall_00458f10) s14;   // 00458F10 SetStringSize
    decltype(&kotor_fastcall_00458f70) s18;   // 00458F70 SetString
    decltype(&kotor_fastcall_0045bb30) s1c;   // 0045BB30 SetAlignment
    decltype(&kotor_fastcall_005d5fe0) s20;   // 005D5FE0 GetProperty0x46 (GetAlignment)
    decltype(&kotor_fastcall_0045bb40) s24;   // 0045BB40 SetAlignmentVertical
    decltype(&kotor_fastcall_0045bb60) s28;   // 0045BB60 GetAlignmentVertical
    decltype(&kotor_fastcall_0045bb70) s2c;   // 0045BB70 SetAlignmentHorizontal
    decltype(&kotor_fastcall_0045bb90) s30;   // 0045BB90 GetAlignmentHorizontal
    decltype(&kotor_fastcall_0045bba0) s34;   // 0045BBA0 SetCursorIndex
    decltype(&kotor_fastcall_0045bbb0) s38;   // 0045BBB0 GetCursorIndex
    decltype(&kotor_fastcall_0045bbc0) s3c;   // 0045BBC0 DisableCursor
    decltype(&kotor_fastcall_0045bc60) s40;   // 0045BC60 GetFontTexture
    decltype(&kotor_fastcall_0045bd70) s44;   // 0045BD70 SetFontTexture
    decltype(&kotor_fastcall_00459610) s48;   // 00459610 GetFontPixelHeight
    decltype(&gui_wrap_draw::wrap_0045a2f0) s4c;   // 0045A2F0 WrapStrings (M40)
    decltype(&kotor_fastcall_0045b7a0) s50;   // 0045B7A0 GetIdealPixelHeight
    decltype(&aurora_txi_size_00712fc0) s54;  // 00712FC0 GetProperty0x38 (lane-tpc body)
    decltype(&kotor_fastcall_00458fe0) s58;   // 00458FE0 GetPositionBoundingRect(float*, float*)
    decltype(&kotor_fastcall_0045bbe0) s5c;   // 0045BBE0 GetPositionBoundingRect(int*, int*)
    decltype(&kotor_fastcall_00459030) s60;   // 00459030 (named Get...) sets position from floats
    decltype(&kotor_fastcall_0045bc00) s64;   // 0045BC00 SetPositionBoundingRect(int, int)
    decltype(&kotor_fastcall_00459090) s68;   // 00459090 GetSizeBoundingRect(float*, float*)
    decltype(&kotor_fastcall_0045bc20) s6c;   // 0045BC20 GetSizeBoundingRect(int*, int*)
    decltype(&kotor_fastcall_004590e0) s70;   // 004590E0 SetSizeBoundingRect(float, float)
    decltype(&kotor_fastcall_0045bc40) s74;   // 0045BC40 SetSizeBoundingRect(int, int)
    decltype(&kotor_fastcall_00459630) s78;   // 00459630 DrawBoundingRect (RET 10h)
    decltype(&gui_wrap_draw::draw_0045a850) s7c;   // 0045A850 Draw(alpha) (M40)
};
static_assert(sizeof(AurGuiStringVTable) == 0x80, "00741878 has 32 slots");
extern "C" const AurGuiStringVTable aur_gui_string_vtable_00741878 = {
    &kotor_fastcall_0045bbd0, &kotor_fastcall_0045be80, &kotor_fastcall_0045bae0, &kotor_fastcall_0045bb00,
    &kotor_fastcall_0045bb20, &kotor_fastcall_00458f10, &kotor_fastcall_00458f70, &kotor_fastcall_0045bb30,
    &kotor_fastcall_005d5fe0, &kotor_fastcall_0045bb40, &kotor_fastcall_0045bb60, &kotor_fastcall_0045bb70,
    &kotor_fastcall_0045bb90, &kotor_fastcall_0045bba0, &kotor_fastcall_0045bbb0, &kotor_fastcall_0045bbc0,
    &kotor_fastcall_0045bc60, &kotor_fastcall_0045bd70, &kotor_fastcall_00459610, &gui_wrap_draw::wrap_0045a2f0,
    &kotor_fastcall_0045b7a0, &aurora_txi_size_00712fc0, &kotor_fastcall_00458fe0, &kotor_fastcall_0045bbe0,
    &kotor_fastcall_00459030, &kotor_fastcall_0045bc00, &kotor_fastcall_00459090, &kotor_fastcall_0045bc20,
    &kotor_fastcall_004590e0, &kotor_fastcall_0045bc40, &kotor_fastcall_00459630, &gui_wrap_draw::draw_0045a850};

struct AurGuiImageBaseVTable;   // aur_gui_image_object.cpp: abstract base table 00741834 (CAurGUI*)
extern "C" const AurGuiImageBaseVTable aur_gui_image_base_vtable_00741834;

namespace {
unsigned char* B(void* p) { return static_cast<unsigned char*>(p); }
template <class T> T get(const void* p, unsigned off) {
    T v;
    _ReadWriteBarrier();
    std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof v);
    _ReadWriteBarrier();
    return v;
}
template <class T> void put(void* p, unsigned off, T v) {
    _ReadWriteBarrier();
    std::memcpy(static_cast<unsigned char*>(p) + off, &v, sizeof v);
    _ReadWriteBarrier();
}
const AurGuiStringVTable* table_of(const void* self) { return get<const AurGuiStringVTable*>(self, 0); }
Texture* font_of(const void* self) { return get<Texture*>(self, 0x18); }
List* list_at(void* self, unsigned off) { return reinterpret_cast<List*>(B(self) + off); }
I32 ftol_006fae8c(double value) {
    I32 result;
    __asm {
        fld qword ptr value
        call unresolved_ftol2_006fae8c
        mov result, eax
    }
    return result;
}
Word x87_move(Word bits) {   // FLD m32 / FSTP m32
    Word result;
    __asm {
        fld dword ptr bits
        fstp dword ptr result
    }
    return result;
}
std::uint32_t c_strlen(const char* s) { std::uint32_t n = 0; while (s[n] != 0) ++n; return n; }   // inline REPNE-free loops
void c_strcpy(char* d, const char* s) { std::size_t i = 0; do { d[i] = s[i]; } while (s[i++] != 0); }
float F(Word bits) { float f; std::memcpy(&f, &bits, 4); return f; }
void viewport_dims(I32& w, I32& h) {   // MOVSX [idx*10+007B946C] / [idx*10+007B946E]
    const short index = kotor_renderer_cells::viewport_index_007b9460;
    std::int16_t ww, hh;
    std::memcpy(&ww, &kotor_renderer_cells::viewport_frame_words_007b9468[static_cast<unsigned>(static_cast<int>(index)) * 5u + 2u], 2);
    std::memcpy(&hh, &kotor_renderer_cells::viewport_frame_words_007b9468[static_cast<unsigned>(static_cast<int>(index)) * 5u + 3u], 2);
    w = ww; h = hh;
}
// SafePointer<CAurTexture> reset (inline form at 0045B9EC..0045B9FC / 0045BDA4..0045BDB4 / 0045BD48..0045BD52).
void safe_unregister(void* self) {
    Texture* const target = font_of(self);
    if (target != nullptr)
        texture_list_remove_00445080(&target->registrations, nullptr,
                                     static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(B(self) + 0x18)));
}
void list_dtor_inline(void* self, unsigned off) {   // CExoArrayList dtor inlined: free(data) if non-null
    void* const data = get<void*>(self, off);
    if (data != nullptr) live_free(data);
}
struct BaseOnUnwind { void* s; ~BaseOnUnwind() { if (s) put(s, 0, &aur_gui_image_base_vtable_00741834); } };   // 007169E0 -> 00459140
struct SafeOnUnwind { void* s; ~SafeOnUnwind() { if (s) aurora_texture_safe_pointer_destroy_00422380(B(s) + 0x18, nullptr); } };   // 007169E8
struct ListOnUnwind { void* list; ~ListOnUnwind() { if (list) exo_array_list_free_0047abc0(list, nullptr); } };      // 007169F3 / 007169FE / 007169B0..
const texture::FontInfo* font_info(void* self) {   // [+18]->vtbl+38 (texture GetFontInfo)
    Texture* const t = font_of(self);
    return texture_vtable::head(t)->common.s38(t, nullptr);
}
}

extern "C" {

// 0045B990 CAurGUIStringInternal::CAurGUIStringInternal(text, font, Vector colour, float scale), RET 18h.
void* __fastcall kotor_fastcall_0045b990(void* self, void*, const char* text, const char* font_name,
                                         Word color_x, Word color_y, Word color_z, Word scale_bits) noexcept(false) {
    put(self, 0, &aur_gui_string_vtable_00741878);                         // 0045B9B5
    BaseOnUnwind base{self};                                               // state 0 (0045B9BB)
    put<void*>(self, 0x18, nullptr);                                       // 0045B9BF
    SafeOnUnwind safe{self};
    put<Word>(self, 0x1C, 0); put<Word>(self, 0x20, 0); put<Word>(self, 0x24, 0);   // 0045B9C2..0045B9C8
    put<Word>(self, 0x2C, 0); put<Word>(self, 0x30, 0); put<Word>(self, 0x28, 0);   // 0045B9CB..0045B9D1
    ListOnUnwind list_a{B(self) + 0x28};
    put<Word>(self, 0x38, 0); put<Word>(self, 0x3C, 0); put<Word>(self, 0x34, 0);   // 0045B9D7..0045B9DD
    ListOnUnwind list_b{B(self) + 0x34};                                   // state 3 (0045B9E5)
    if (font_name == nullptr) {                                            // 0045B9E3..0045B9EA
        safe_unregister(self);                                             // 0045B9EC..0045B9F7
        put<void*>(self, 0x18, nullptr);                                   // 0045B9FC
    } else {
        Texture* const t = font::reference_00423a60(font_name, nullptr);   // 0045BA01..0045BA08
        font::assign_00447fa0(reinterpret_cast<font::Safe*>(B(self) + 0x18), nullptr, t);   // 0045BA0B..0045BA0E
    }
    char* const copy = static_cast<char*>(c2da_directory_new_006fa7e6(c_strlen(text) + 1));   // 0045BA13..0045BA2B
    put(self, 0x14, copy);                                                 // 0045BA33
    c_strcpy(copy, text);                                                  // 0045BA36..0045BA48
    const std::uint32_t length = c_strlen(text);                           // 0045BA4A..0045BA5C
    List* const lines = list_at(self, 0x34);
    if (lines->size == lines->capacity) {                                  // 0045BA57..0045BA63
        const std::int32_t cap = lines->capacity;
        menu_list_growth::allocate_005e03c0(lines, nullptr, cap != 0 ? static_cast<Word>(cap + cap) : 8u);   // 0045BA65..0045BA75
    }
    put<std::uint32_t>(reinterpret_cast<void*>(static_cast<std::uintptr_t>(lines->data)),
                       static_cast<unsigned>(lines->size) * 4u, length);  // 0045BA7A..0045BA83
    lines->size = lines->size + 1;                                         // 0045BA86..0045BA8E
    put<Word>(self, 0x1C, color_x);                                        // 0045BA95
    put<Word>(self, 0x20, color_y);                                        // 0045BA9C
    put<Word>(self, 0x24, color_z);                                        // 0045BAA0
    put<Word>(self, 4, 0); put<Word>(self, 8, 0); put<Word>(self, 0xC, 0); put<Word>(self, 0x10, 0);   // 0045BAA7..0045BAB0
    put<std::uint16_t>(self, 0x44, 0xFFFFu);                               // 0045BAB3
    put<std::uint8_t>(self, 0x46, 9);                                      // 0045BAB9
    put<Word>(self, 0x40, scale_bits);                                     // 0045BABD
    list_b.list = nullptr; list_a.list = nullptr; safe.s = nullptr; base.s = nullptr;
    return self;
}

// 0045BDF0 NewCAurGUIString(text, font, Vector const* colour, float scale), cdecl: new(48h); colour
// copied by value into the ctor arguments (0045BE29..0045BE3C); EH state 0 frees the block (00716A20).
void* __cdecl kotor_cdecl_0045bdf0(const char* text, const char* font_name, const void* color,
                                   Word scale_bits) noexcept(false) {
    void* const block = c2da_directory_new_006fa7e6(0x48);                 // 0045BE07..0045BE09
    if (block == nullptr) return nullptr;                                  // 0045BE15..0045BE1F / 0045BE60
    struct Free { void* b; ~Free() { if (b) live_free(b); } } guard{block};
    void* const object = kotor_fastcall_0045b990(block, nullptr, text, font_name, get<Word>(color, 0),
                                                 get<Word>(color, 4), get<Word>(color, 8), scale_bits);   // 0045BE21..0045BE4B
    guard.b = nullptr;
    return object;
}

// 0045BC70 CAurGUIStringInternal::~CAurGUIStringInternal (thiscall, RET).
void __fastcall kotor_fastcall_0045bc70(void* self, void*) {
    put(self, 0, &aur_gui_string_vtable_00741878);                         // 0045BC90
    Texture* const font = font_of(self);                                   // 0045BC96
    if (font != nullptr)                                                   // 0045BCA0..0045BCB0
        texture_list_remove_00445080(&font->registrations, nullptr,
                                     static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(B(self) + 0x18)));
    put<void*>(self, 0x18, nullptr);                                       // 0045BCB6
    texture_release_00420d70(font);                                        // 0045BCB5..0045BCB8 (also for null)
    safe_unregister(self);                                                 // 0045BCBD..0045BCCA (re-read, now null)
    put<void*>(self, 0x18, nullptr);                                       // 0045BCCF
    live_free(get<void*>(self, 0x14));                                     // 0045BCD1..0045BCD5
    put<void*>(self, 0x14, nullptr);                                       // 0045BCDA
    if (get<void*>(self, 0x34) != nullptr) {                               // 0045BCDD..0045BCE5
        live_free(get<void*>(self, 0x34));
        put<void*>(self, 0x34, nullptr);                                   // 0045BCF0
    }
    put<Word>(self, 0x3C, 0); put<Word>(self, 0x38, 0);                    // 0045BCF3..0045BCF6
    if (get<void*>(self, 0x28) != nullptr) {                               // 0045BCF9..0045BCFE
        live_free(get<void*>(self, 0x28));
        put<void*>(self, 0x28, nullptr);                                   // 0045BD09
    }
    put<Word>(self, 0x30, 0); put<Word>(self, 0x2C, 0);                    // 0045BD0C..0045BD0F
    put<Word>(self, 4, 0); put<Word>(self, 8, 0); put<Word>(self, 0xC, 0); put<Word>(self, 0x10, 0);   // 0045BD12..0045BD1B
    list_dtor_inline(self, 0x34);                                          // 0045BD1E..0045BD2B (member dtor)
    list_dtor_inline(self, 0x28);                                          // 0045BD33..0045BD40
    safe_unregister(self);                                                 // 0045BD48..0045BD52 (Safe dtor inline)
    put(self, 0, &aur_gui_image_base_vtable_00741834);                     // 0045BD5C (base dtor inline)
}

// 0045BE80 scalar deleting destructor (slot +04), RET 4.
void* __fastcall kotor_fastcall_0045be80(void* self, void*, Word flags) noexcept(false) {
    kotor_fastcall_0045bc70(self, nullptr);                                // 0045BE83
    if ((flags & 1u) != 0) live_free(self);                                // 0045BE88..0045BE90
    return self;
}

// 0045BBD0 Draw(): own vtbl+7C(1.0f).
void __fastcall kotor_fastcall_0045bbd0(void* self, void*) noexcept(false) {
    table_of(self)->s7c(self, nullptr, 0x3F800000u);
}
// 0045BAE0 GetColor(Vector* out), RET 4: copies +1C..+24, EAX = out.
void* __fastcall kotor_fastcall_0045bae0(void* self, void*, void* out) {
    put<Word>(out, 0, get<Word>(self, 0x1C)); put<Word>(out, 4, get<Word>(self, 0x20)); put<Word>(out, 8, get<Word>(self, 0x24));
    return out;
}
// 0045BB00 SetColor(Vector const*), RET 4.
void __fastcall kotor_fastcall_0045bb00(void* self, void*, const void* color) {
    put<Word>(self, 0x1C, get<Word>(color, 0)); put<Word>(self, 0x20, get<Word>(color, 4)); put<Word>(self, 0x24, get<Word>(color, 8));
}
// 0045BB20 GetStringSize: strlen(+14).
std::uint32_t __fastcall kotor_fastcall_0045bb20(void* self, void*) { return c_strlen(get<const char*>(self, 0x14)); }
// 00458F10 SetStringSize(short n), RET 4: grow (copy into new(n+1), free old) or truncate at n.
void __fastcall kotor_fastcall_00458f10(void* self, void*, Word size) noexcept(false) {
    char* const old = get<char*>(self, 0x14);                              // 00458F14
    const std::int16_t length = static_cast<std::int16_t>(c_strlen(old));  // 00458F17..00458F2C
    const std::int16_t n = static_cast<std::int16_t>(size);                // 00458F27
    if (n <= length) {                                                     // 00458F2E..00458F34 (16-bit signed)
        old[static_cast<I32>(n)] = 0;                                      // 00458F66
        return;
    }
    char* const grown = static_cast<char*>(c2da_directory_new_006fa7e6(static_cast<std::uint32_t>(static_cast<I32>(n) + 1)));   // 00458F36..00458F38
    c_strcpy(grown, get<const char*>(self, 0x14));                         // 00458F3D..00458F4F
    live_free(get<void*>(self, 0x14));                                     // 00458F51..00458F55
    put(self, 0x14, grown);                                                // 00458F5D
}
// 00458F70 SetString(char const*), RET 4: reallocate only when longer (unsigned compare), then copy.
void __fastcall kotor_fastcall_00458f70(void* self, void*, const char* text) noexcept(false) {
    const std::uint32_t wanted = c_strlen(text);                           // 00458F75..00458F87
    if (wanted > c_strlen(get<const char*>(self, 0x14))) {                 // 00458F89..00458FA0 (jbe)
        live_free(get<void*>(self, 0x14));                                 // 00458FA2..00458FA3
        put(self, 0x14, c2da_directory_new_006fa7e6(c_strlen(text) + 1)); // 00458FA8..00458FC3
    }
    c_strcpy(get<char*>(self, 0x14), text);                                // 00458FC6..00458FD8
}
// 0045BB30 SetAlignment(byte), RET 4.
void __fastcall kotor_fastcall_0045bb30(void* self, void*, Word alignment) { put<std::uint8_t>(self, 0x46, static_cast<std::uint8_t>(alignment)); }
// 005D5FE0 GetProperty0x46: mov al,[ecx+46h]; ret.
std::uint8_t __fastcall kotor_fastcall_005d5fe0(void* self, void*) { return get<std::uint8_t>(self, 0x46); }
// 0045BB40 SetAlignmentVertical(byte): [+46] = ([+46] & 7) | arg.
void __fastcall kotor_fastcall_0045bb40(void* self, void*, Word v) {
    put<std::uint8_t>(self, 0x46, static_cast<std::uint8_t>((get<std::uint8_t>(self, 0x46) & 7u) | (v & 0xFFu)));
}
// 0045BB60 GetAlignmentVertical: [+46] & 38h (full EAX).
std::uint32_t __fastcall kotor_fastcall_0045bb60(void* self, void*) { return get<std::uint8_t>(self, 0x46) & 0x38u; }
// 0045BB70 SetAlignmentHorizontal(byte): [+46] = ([+46] & 38h) | arg.
void __fastcall kotor_fastcall_0045bb70(void* self, void*, Word h) {
    put<std::uint8_t>(self, 0x46, static_cast<std::uint8_t>((get<std::uint8_t>(self, 0x46) & 0x38u) | (h & 0xFFu)));
}
// 0045BB90 GetAlignmentHorizontal: [+46] & 7.
std::uint32_t __fastcall kotor_fastcall_0045bb90(void* self, void*) { return get<std::uint8_t>(self, 0x46) & 7u; }
// 0045BBA0 SetCursorIndex(short), RET 4.
void __fastcall kotor_fastcall_0045bba0(void* self, void*, Word index) { put<std::uint16_t>(self, 0x44, static_cast<std::uint16_t>(index)); }
// 0045BBB0 GetCursorIndex: AX = [+44].
std::uint16_t __fastcall kotor_fastcall_0045bbb0(void* self, void*) { return get<std::uint16_t>(self, 0x44); }
// 0045BBC0 DisableCursor: [+44] = FFFFh.
void __fastcall kotor_fastcall_0045bbc0(void* self, void*) { put<std::uint16_t>(self, 0x44, 0xFFFFu); }
// 0045BC60 GetFontTexture: null font -> 0, else tail-jump texture vtbl+40 (GetName).
char* __fastcall kotor_fastcall_0045bc60(void* self, void*) {
    Texture* const t = font_of(self);
    if (t == nullptr) return nullptr;
    return texture_vtable::head(t)->common.s40(t, nullptr);
}
// 0045BD70 SetFontTexture(char const*), RET 4.
void __fastcall kotor_fastcall_0045bd70(void* self, void*, const char* name) noexcept(false) {
    Texture* const old = font_of(self);                                    // 0045BD78
    if (old != nullptr) {                                                  // 0045BD7A..0045BD7C
        texture_list_remove_00445080(&old->registrations, nullptr,
                                     static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(B(self) + 0x18)));   // 0045BD84..0045BD88
        put<void*>(self, 0x18, nullptr);                                   // 0045BD8E
        texture_release_00420d70(old);                                     // 0045BD94
    }
    if (name == nullptr) {                                                 // 0045BD9C..0045BDA2
        safe_unregister(self);                                             // 0045BDA4..0045BDAE
        put<void*>(self, 0x18, nullptr);                                   // 0045BDB4
    } else {
        Texture* const t = font::reference_00423a60(name, nullptr);        // 0045BDC6..0045BDC9
        font::assign_00447fa0(reinterpret_cast<font::Safe*>(B(self) + 0x18), nullptr, t);   // 0045BDD1..0045BDD4
    }
    put<Word>(self, 0x2C, 0);                                              // 0045BDBB / 0045BDDB
}
// 00459610 GetFontPixelHeight: _ftol(info.fontheight * 100.0 + 0.5).
I32 __fastcall kotor_fastcall_00459610(void* self, void*) {
    const texture::FontInfo* const info = font_info(self);                 // 00459610..00459615
    return ftol_006fae8c(static_cast<double>(info->fontheight) * static_cast<double>(kotor_renderer_cells::hundred_00741838) + 0.5);   // 00459618..00459627
}
// 0045B7A0 GetIdealPixelHeight(int max_lines), RET 4. Wraps on copies of both line lists, restores them.
I32 __fastcall kotor_fastcall_0045b7a0(void* self, void*, I32 lines) noexcept(false) {
    const double hundred = static_cast<double>(kotor_renderer_cells::hundred_00741838);   // 00741838
    if (lines == -1 && get<I32>(self, 0x38) != 0) {                        // 0045B7BD..0045B7D0
        const texture::FontInfo* const info = font_info(self);             // 0045B7D2..0045B7D7
        const double h = (static_cast<double>(info->spacing_b) + static_cast<double>(info->fontheight)) *
                         static_cast<double>(F(get<Word>(self, 0x40))) * hundred;   // 0045B7DA..0045B7E3
        return ftol_006fae8c(h * static_cast<double>(get<I32>(self, 0x38)) + 0.5);  // 0045B7E9..0045B7F2
    }
    List first, second;   // [esp+1Ch]/[esp+10h] (lines == -1) or [esp+10h]/[esp+1Ch] (else)
    int_list_copy_construct_0046d250(&first, nullptr, list_at(self, 0x28));   // 0045B7FC..0045B804 / 0045B89A..0045B8A2
    ListOnUnwind g1{&first};
    int_list_copy_construct_0046d250(&second, nullptr, list_at(self, 0x34));  // 0045B809..0045B819 / 0045B8A7..0045B8B7
    ListOnUnwind g2{&second};
    table_of(self)->s4c(self, nullptr, static_cast<unsigned int>(lines));  // 0045B81E..0045B829 / 0045B8BC..0045B8C6
    const std::int16_t wrapped = get<std::int16_t>(self, 0x38);            // 0045B82C / 0045B8C9
    int_list_assign_00438bb0(list_at(self, 0x34), nullptr, &second);       // 0045B830..0045B837 / 0045B8CD..0045B8D4
    int_list_assign_00438bb0(list_at(self, 0x28), nullptr, &first);        // 0045B83C..0045B843 / 0045B8D9..0045B8E0
    const texture::FontInfo* const info = font_info(self);                 // 0045B848..0045B84D / 0045B8E5..0045B8EA
    I32 result;
    if (lines == -1) {
        const double h = (static_cast<double>(info->spacing_b) + static_cast<double>(info->fontheight)) *
                         static_cast<double>(F(get<Word>(self, 0x40))) * hundred;   // 0045B850..0045B860
        result = ftol_006fae8c(static_cast<double>(wrapped) * h + 0.5);    // 0045B866..0045B872
    } else {
        const I32 t = ftol_006fae8c((static_cast<double>(info->spacing_b) + static_cast<double>(info->fontheight)) * hundred + 0.5);   // 0045B8ED..0045B8FF
        const I32 u = ftol_006fae8c(static_cast<double>(t) * static_cast<double>(F(get<Word>(self, 0x40))) + 0.5);                    // 0045B904..0045B915
        result = static_cast<I32>(wrapped) * u;                            // 0045B91A..0045B91D
    }
    g2.list = nullptr; g1.list = nullptr;
    // Inline list destructors, original order: lines == -1 frees [esp+10h] (second) then [esp+1Ch]
    // (first); else frees [esp+1Ch] (second) then [esp+10h] (first). Both free `second` first.
    if (second.data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(second.data)));   // 0045B87B..0045B889 / 0045B920..0045B92E
    if (first.data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(first.data)));     // 0045B93A..0045B947
    return result;
}
// 00458FE0 GetPositionBoundingRect(float* x, float* y), RET 8: [+4]/vw, [+8]/vh (FILD/FIDIV/FSTP m32).
void __fastcall kotor_fastcall_00458fe0(void* self, void*, float* x, float* y) {
    I32 vw, vh;
    viewport_dims(vw, vh);
    *x = static_cast<float>(static_cast<double>(get<I32>(self, 4)) / static_cast<double>(vw));
    viewport_dims(vw, vh);
    *y = static_cast<float>(static_cast<double>(get<I32>(self, 8)) / static_cast<double>(vh));
}
// 0045BBE0 GetPositionBoundingRect(int* x, int* y), RET 8.
void __fastcall kotor_fastcall_0045bbe0(void* self, void*, I32* x, I32* y) { *x = get<I32>(self, 4); *y = get<I32>(self, 8); }
// 00459030 position from normalized floats, RET 8: [+4] = _ftol(vw * x), [+8] = _ftol(vh * y).
void __fastcall kotor_fastcall_00459030(void* self, void*, Word x_bits, Word y_bits) {
    I32 vw, vh;
    viewport_dims(vw, vh);
    put<I32>(self, 4, ftol_006fae8c(static_cast<double>(vw) * static_cast<double>(F(x_bits))));
    viewport_dims(vw, vh);
    put<I32>(self, 8, ftol_006fae8c(static_cast<double>(vh) * static_cast<double>(F(y_bits))));
}
// 0045BC00 SetPositionBoundingRect(int, int), RET 8.
void __fastcall kotor_fastcall_0045bc00(void* self, void*, I32 x, I32 y) { put<I32>(self, 4, x); put<I32>(self, 8, y); }
// 00459090 GetSizeBoundingRect(float*, float*), RET 8.
void __fastcall kotor_fastcall_00459090(void* self, void*, float* w, float* h) {
    I32 vw, vh;
    viewport_dims(vw, vh);
    *w = static_cast<float>(static_cast<double>(get<I32>(self, 0xC)) / static_cast<double>(vw));
    viewport_dims(vw, vh);
    *h = static_cast<float>(static_cast<double>(get<I32>(self, 0x10)) / static_cast<double>(vh));
}
// 0045BC20 GetSizeBoundingRect(int*, int*), RET 8.
void __fastcall kotor_fastcall_0045bc20(void* self, void*, I32* w, I32* h) { *w = get<I32>(self, 0xC); *h = get<I32>(self, 0x10); }
// 004590E0 SetSizeBoundingRect(float, float), RET 8.
void __fastcall kotor_fastcall_004590e0(void* self, void*, Word w_bits, Word h_bits) {
    I32 vw, vh;
    viewport_dims(vw, vh);
    put<I32>(self, 0xC, ftol_006fae8c(static_cast<double>(vw) * static_cast<double>(F(w_bits))));
    viewport_dims(vw, vh);
    put<I32>(self, 0x10, ftol_006fae8c(static_cast<double>(vh) * static_cast<double>(F(h_bits))));
}
// 0045BC40 SetSizeBoundingRect(int, int), RET 8.
void __fastcall kotor_fastcall_0045bc40(void* self, void*, I32 w, I32 h) { put<I32>(self, 0xC, w); put<I32>(self, 0x10, h); }

// 00459630 DrawBoundingRect (4 unread stack args), RET 10h: GL_LINE_LOOP ([0073F358] = 2) in colour
// FFE5A566 through (L,T) (L,B) (R,B) (R,T) (L,B) (the original's fifth vertex).
void __fastcall kotor_fastcall_00459630(void* self, void*, Word, Word, Word, Word) {
    namespace gl = kotor_renderer_init;
    if (kotor_renderer_cells::busy_007bb4d0 != 0) return;                  // 00459630..0045963D
    gl::glDisable_0073d22c(kotor_renderer_cells::disable_0078d45c);        // 00459643..00459652
    gl::glDisable_0073d22c(kotor_renderer_cells::enable_0078d458);         // 00459654..0045965B
    gl::glDisable_0073d22c(kotor_renderer_cells::enable_0078d448);         // 0045965D..00459664
    I32 vw, vh;
    viewport_dims(vw, vh);                                                 // 00459666..00459695
    const I32 y = get<I32>(self, 8), x = get<I32>(self, 4);
    const I32 bottom_px = get<I32>(self, 0x10) + y;                        // 00459684..0045968B
    const I32 right_px = get<I32>(self, 0xC) + x;                          // 004596A5..004596A8
    const float top = static_cast<float>(1.0 - static_cast<double>(y) / static_cast<double>(vh));            // 00459687..004596B0
    const float bottom = static_cast<float>(1.0 - static_cast<double>(bottom_px) / static_cast<double>(vh)); // 004596B4..004596CA
    const float left = static_cast<float>(static_cast<double>(x) / static_cast<double>(vw));                 // 004596D0..004596DE
    const float right = static_cast<float>(static_cast<double>(right_px) / static_cast<double>(vw));         // 004596E2..004596E8
    gl::glBegin_0073d2d0(2u);                                              // 004596BC..004596EE (.rdata 0073F358)
    const Word color = 0xFFE5A566u;                                        // 004596F9
    gl::glColor4ubv_0073d2e8(reinterpret_cast<const std::uint8_t*>(&color));   // 00459701
    gl::glVertex4f_0073d324(left, top, 0.0f, 1.0f);                        // 00459715..0045971E
    gl::glVertex4f_0073d324(left, bottom, 0.0f, 1.0f);                     // 00459724..0045972D
    gl::glVertex4f_0073d324(right, bottom, 0.0f, 1.0f);                    // 00459733..0045973C
    gl::glVertex4f_0073d324(right, top, 0.0f, 1.0f);                       // 00459742..0045974B
    gl::glVertex4f_0073d324(left, bottom, 0.0f, 1.0f);                     // 0045974D..00459756
    gl::glEnd_0073d2c4();                                                  // 00459758
    gl::glEnable_0073d390(kotor_renderer_cells::disable_0078d45c);         // 0045975E..0045976B
    gl::glEnable_0073d390(kotor_renderer_cells::enable_0078d448);          // 0045976D..00459774
    gl::glEnable_0073d390(kotor_renderer_cells::enable_0078d458);          // 00459776..0045977C
}

// 004171E0 CSWGuiTextParams::operator=(CSWGuiTextParams const&), thiscall RET 4, EAX = this.
void* __fastcall kotor_fastcall_004171e0(void* self, void*, const void* source) noexcept(false) {
    async_assign_string_005e5c50(self, nullptr, source);                   // 004171E7..004171EA (CExoString at +0)
    put<Word>(self, 8, get<Word>(source, 8));                              // 004171EF..004171F2
    c_strcpy(reinterpret_cast<char*>(B(self) + 0x3C),
             reinterpret_cast<const char*>(static_cast<const unsigned char*>(source) + 0x3C));   // 004171F5..00417208
    for (unsigned i = 0; i != 12; i += 4) put<Word>(self, 0x2C + i, get<Word>(source, 0x1C + i));   // 0041720A..0041721F
    for (unsigned i = 0; i != 12; i += 4) put<Word>(self, 0x1C + i, get<Word>(source, 0x1C + i));   // 00417222..00417235
    const Word old38 = get<Word>(self, 0x38);                              // 00417232
    const Word mixed = old38 ^ ((get<Word>(self, 0x38) ^ get<Word>(source, 0x38)) & 0x3Fu);   // 00417238..00417243
    put<Word>(self, 0x38, mixed);                                          // 00417245
    put<Word>(self, 0x28, x87_move(get<Word>(source, 0x28)));              // 00417248..0041724B (FLD/FSTP m32)
    const Word final38 = get<Word>(self, 0x38) ^ ((mixed ^ get<Word>(source, 0x38)) & 0x40u);   // 0041724E..0041725C
    void* const text_object = get<void*>(self, 0x50);                      // 00417256
    put<Word>(self, 0x38, final38);                                        // 00417261
    gui_load_text_params_set_text_object_00415f70(self, nullptr, text_object);   // 0041725E..00417264
    return self;
}

// 00417310 CSWGuiText::Initialize(CSWGuiExtent const*, CSWGuiTextParams const*, float scale), thiscall
// RET 0Ch (called from Label::Initialize 004176E0 at 00417737 on the DrawPCBG label, frame 1).
void __fastcall kotor_fastcall_00417310(void* text, void*, const void* extent, void* text_params,
                                        Word scale_bits) noexcept(false) {
    void* const old = get<void*>(text, 0x14);                              // 00417317
    if (old != nullptr) table_of(old)->s04(old, nullptr, 1);               // 0041731A..00417323 (vtbl+4, 1)
    char font_name[0x14];                                                  // [esp+0Ch], 20 bytes
    gui_load_resref_copy_to_string_00405fb0(static_cast<const unsigned char*>(text_params) + 0x0C, nullptr, font_name);   // 00417326..00417332
    const char* const string = WindowCStr005e5670(text_params, nullptr);   // 00417345..00417347
    void* const created = kotor_cdecl_0045bdf0(string, font_name,
                                               static_cast<const unsigned char*>(text_params) + 0x1C, scale_bits);   // 00417337..0041734D
    put(text, 0x14, created);                                              // 00417359
    kotor_fastcall_004171e0(B(text) + 0x18, nullptr, text_params);         // 00417355..0041735C
    for (unsigned i = 0; i != 16; i += 4) put<Word>(text, 4 + i, get<Word>(extent, i));   // 00417361..0041737D
    if (get<void*>(text, 0x14) == nullptr) return;                         // 00417380..00417385
    void* s = get<void*>(text, 0x14);
    table_of(s)->s64(s, nullptr, get<I32>(text, 4), get<I32>(text, 8));    // 00417387..00417390 SetPositionBoundingRect
    s = get<void*>(text, 0x14);
    table_of(s)->s74(s, nullptr, get<I32>(text, 0xC), get<I32>(text, 0x10));   // 00417393..004173A0 SetSizeBoundingRect
    s = get<void*>(text, 0x14);
    table_of(s)->s1c(s, nullptr, get<std::uint8_t>(text_params, 0x38) & 0x3Fu);   // 004173A3..004173B3 SetAlignment
    gui_load_text_wrap_00414e90(text, nullptr);                            // 004173B6..004173B8
}

}   // extern "C"
