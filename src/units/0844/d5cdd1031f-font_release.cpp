// lane-window: CAurFont::~CAurFont 004A21D0 (__thiscall, RET 0) and AurTextureRelease 00420D70
// (cdecl, one word). Evidence: dumpbin /disasm swkotor.exe 004A21D0..004A22D8 (EH handler
// 00717E7B, state 0 = the +1C CAurTextureSafePointer member), 00420D70..00420DA7.
// Types: lane-texture-epoch E1 (font::Texture {vtable, registrations list +4, ...};
// texture_vtable::Head typed slots +00/+04). Font list 0083052C: kotor_aurora_window::fonts_0083052c
// (aurora_cleanup.cpp).
#include "283ae0c931-texture_vtable.hpp"   // lane-texture-epoch include root
#include "64c8bc673c-window_lists.h"
#include <cstdint>
#include <cstring>

using U32 = std::uint32_t; using I32 = std::int32_t;
extern "C" {
void __cdecl texture_release_00420d70(font::Texture* texture);
void __fastcall aurora_font_destroy_004a21d0(void* font, void*);
}

// 00420D70 AurTextureRelease(texture).
extern "C" void __cdecl texture_release_00420d70(font::Texture* t) {
    if (!t) return;                                                   // 00420D71..00420D77
    if (t->registrations.size != 0) return;                           // 00420D79..00420D7E ([esi+8])
    if (texture_vtable::head(t)->s04(t, nullptr) == nullptr) {        // 00420D80..00420D8B call [eax+4]
        (void)texture_vtable::head(t)->s00(t, nullptr, 1);            // 00420DA0..00420DA4 call [eax](1)
        return;
    }
    font::Texture* basic = texture_vtable::head(t)->s04(t, nullptr);  // 00420D8D..00420D8F (second call)
    if (basic) (void)texture_vtable::head(basic)->s00(basic, nullptr, 1); // 00420D92..00420D9C
}

namespace {
// Inline CAurTextureSafePointer release (004A2201..004A2207 / 004A2288..004A22C5): remove the
// cell's own address from the target's registration list (ArrayMutations::RemoveReverse32 == 00445080).
void unregister(font::Safe& cell) {
    if (font::Texture* target = cell.target)
        target->registrations.RemoveReverse32(static_cast<U32>(reinterpret_cast<std::uintptr_t>(&cell)));
}
struct SafeMemberDtor {                                               // EH state 0 member + final 004A2288 block
    font::Safe& cell;
    ~SafeMemberDtor() { unregister(cell); }
};
U32* font_words(U32 data) { return reinterpret_cast<U32*>(static_cast<std::uintptr_t>(data)); }
}

extern "C" void __fastcall aurora_font_destroy_004a21d0(void* self, void*) {
    unsigned char* f = static_cast<unsigned char*>(self);
    font::Safe& cell = *reinterpret_cast<font::Safe*>(f + 0x1c);     // 004A21F2 esi=&[edi+1C]
    SafeMemberDtor member{cell};
    font::Texture* texture = cell.target;                             // 004A21EF ebx
    unregister(cell);                                                 // 004A21F5..004A2207
    cell.target = nullptr;                                            // 004A220D
    texture_release_00420d70(texture);                                // 004A220C..004A221A
    unregister(cell);                                                 // 004A2218..004A2225
    cell.target = nullptr;                                            // 004A222A
    I32 index; std::memcpy(&index, f, 4);                             // 004A2230 eax=[edi]
    if (index < 0) return;                                            // 004A2232..004A2234
    auto& fonts = kotor_aurora_window::fonts_0083052c;
    for (I32 i = index + 1; i < fonts.size; ++i) {                    // 004A2236..004A2255
        I32* other = reinterpret_cast<I32*>(static_cast<std::uintptr_t>(font_words(fonts.data)[i]));
        --*other;                                                     // 004A2247..004A224A dec dword [font]
    }
    std::memcpy(&index, f, 4);                                        // 004A2257 reload
    const I32 last = fonts.size - 1;                                  // 004A2259
    fonts.size = last;                                                // 004A225C
    for (I32 i = index; i < last && i < fonts.size; ++i)              // 004A225A..004A2286
        font_words(fonts.data)[i] = font_words(fonts.data)[i + 1];
}                                                                     // 004A2288..004A22C5 member dtor (cell now null)
