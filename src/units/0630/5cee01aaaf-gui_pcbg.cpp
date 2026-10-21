// lane-gui-render: CSWGuiManager::DrawPCBG 0040AE80 (R4 consumer name
// @frame_DrawCursor_0040ae80@12). Reached on the first menu frame: AddPanel
// 0040BC70(menu, 2, 1) from 005FCB0E stores flags bit3 (0040BCCD..0040BCDF:
// (2<<2)&0x18 = 8) and increments byte [mgr+70] (0040BCE4), which
// CSWGuiManager::Draw tests at 0040CC78..0040CC85.
#include "881508e8f6-gui_render.h"
#include <cstring>
#include <intrin.h>

using gui_render::I32;
using gui_render::Word;

extern "C" {
// M40 providers.
void* __cdecl c2da_directory_new_006fa7e6(std::uint32_t) noexcept(false);   // m39-directory-allocator.obj
void __cdecl live_free(void*);                                                // live-policy.obj (006FA390)
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, void* string) noexcept(false);   // ref-bodies.obj
// lane-gui-load declared name (MISSING there): SetFillImage(CResRef const&, int), RET 8.
void __fastcall gui_load_border_params_set_fill_00414c00(void* params, void*, const void* ref,
                                                          std::int32_t force) noexcept(false);
// MISSING native providers (thiscall originals, D1 fastcall forms). The C++
// originals may throw (MSVC EH frames in the image), so throwing declarations are
// explicit: /EHsc would otherwise assume extern "C" never throws and drop the
// unwind actions of FuncInfo 007608B4.
void* __fastcall kotor_fastcall_0041acd0(void* self, void*) noexcept(false);             // RET
void* __fastcall kotor_fastcall_00417050(void* self, void*) noexcept(false);       // RET
void* __fastcall kotor_fastcall_00415000(void* self, void*, void* corner,
        void* edge, void* fill, Word a4, Word a5, Word a6_bits, const void* color, Word a8) noexcept(false);   // RET 20h
// 005E5C20 CExoString::~CExoString native provider: lane-r4-lowers r4_log_joins.cpp.
void __fastcall frame_LogStringDtor_005e5c20(void* self, void*);               // RET
// 0059EA30: lane-menu-tables (D59), joined by name (unwind target of state 1).
void __fastcall kotor_fastcall_0059ea30(void* self, void*);
// CResRef::CResRef() 00405ED0 (zero 4 DWORDs, EAX=this); joined to the D9 adopter.
void* __fastcall kotor_fastcall_00405ed0(void* self, void*) noexcept(false);
}
// 0059EA30 (jmp 005E5C20): single provider lane-menu-tables (D59); joined in gui_layer_joins.cpp.

// lane-gui-load gui_params.cpp: Border::Initialize 00415190 (RET 8), Border::GetInnerExtent 00415240 (RET 4).
extern "C" void __fastcall gui_load_border_initialize_00415190(void* border, void*, const void* extent,
                                                               const void* params) noexcept(false);
extern "C" void* __fastcall gui_load_border_inner_extent_00415240(void* border, void*, void* out) noexcept(false);
// CSWGuiText::Initialize 00417310: this lane, aur_gui_string.cpp (rev2).
extern "C" void __fastcall kotor_fastcall_00417310(void* text, void*, const void* extent, void* text_params,
                                                    Word scale_bits) noexcept(false);

// 004176E0 CSWGuiLabel::Initialize(extent*, TextParams*, BorderParams*, float scale) (vtable 73E5B8 +9C),
// thiscall RET 10h.
extern "C" void __fastcall kotor_fastcall_004176e0(void* label, void*, const void* extent, void* text_params,
                                                    const void* border_params, Word scale_bits) noexcept(false) {
    unsigned char* const self = static_cast<unsigned char*>(label);
    for (unsigned offset = 0; offset != 16; offset += 4) {                     // 004176EB..00417703 extent -> +04
        Word word;
        std::memcpy(&word, static_cast<const unsigned char*>(extent) + offset, 4);
        std::memcpy(self + 4 + offset, &word, 4);
    }
    gui_load_border_initialize_00415190(self + 0x5C, nullptr, self + 4, border_params);   // 00417706..00417711
    I32 inner[4];                                                              // [esp+8] local extent
    gui_load_border_inner_extent_00415240(self + 0x5C, nullptr, inner);       // 00417716..0041771D
    kotor_fastcall_00417310(self + 0xD0, nullptr, inner, text_params, scale_bits);   // 00417722..00417737
}

// D9 accepted adopter of 00405ED0 (thiscall, no stack args == fastcall@8 here).
#pragma comment(linker, "/alternatename:@kotor_fastcall_00405ed0@8=?default_00405ed0@ResRef@kotor_menu_10_19@@QAEPAU12@XZ")

namespace {
Word W(const void* p, unsigned off) {
    Word v;
    _ReadWriteBarrier();
    std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4);
    _ReadWriteBarrier();
    return v;
}
void* P(const void* p, unsigned off) {
    void* v;
    _ReadWriteBarrier();
    std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4);
    _ReadWriteBarrier();
    return v;
}
void SW(void* p, unsigned off, Word v) {
    _ReadWriteBarrier();
    std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4);
    _ReadWriteBarrier();
}
void SP(void* p, unsigned off, void* v) {
    _ReadWriteBarrier();
    std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4);
    _ReadWriteBarrier();
}
I32 S16(const void* p, unsigned off) {   // MOVSX r32, word
    std::int16_t v;
    std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 2);
    return v;
}
// Label vtable 73E5B8 +9C CSWGuiLabel::Initialize 004176E0 (RET 10h).
using InitializeSlot = void (__fastcall*)(void* label, void* unused_edx, const void* extent,
                                          void* text_params, void* border_params, Word scale_bits);
// Unwind map of FuncInfo 007608B4: state 0 -> 00714730 free([ebp-114h]) via 006FA390;
// state 1 -> 0071473E CSWGuiTextParams dtor 0059EA30 on the TextParams local.
struct FreeOnUnwind {
    void* block;
    ~FreeOnUnwind() { if (block != nullptr) live_free(block); }
};
struct TextParamsOnUnwind {
    void* params;
    ~TextParamsOnUnwind() { if (params != nullptr) kotor_fastcall_0059ea30(params, nullptr); }
};
// Frame-local objects (sizes from the original frame: CResRef 0x10 at esp+24/34/44/54;
// CSWGuiTextParams esp+64..B7 and CSWGuiBorderParams esp+B8..10B, 0x54 each; inferred).
struct alignas(4) ResRefStorage { unsigned char bytes[0x10]; };
struct alignas(4) ParamsStorage { unsigned char bytes[0x54]; };
}

// 0040AE80 CSWGuiManager::DrawPCBG(float delta), thiscall RET 4.
extern "C" void __fastcall frame_DrawCursor_0040ae80(void* manager, void*, Word delta_bits) noexcept(false) {
    kotor_AurGUIStartLayer_004591B0();                                         // 0040AE9E
    float color[3] = {0.0f, 0.0f, 0.0f};                                       // 0040AEBD..0040AECD
    if (kotor_AurGUISetupViewport_004592F0(0, 0, static_cast<Word>(S16(manager, 0x6C)),
            static_cast<Word>(S16(manager, 0x6E)), color, 0, 0x3F800000u) != 0) {   // 0040AEA3..0040AEDF
        if (P(manager, 0x7C) == nullptr) {                                     // 0040AEE5..0040AEEA
            void* memory = c2da_directory_new_006fa7e6(0x140);                 // 0040AEF0..0040AEF5
            void* label = nullptr;
            if (memory != nullptr) {                                           // 0040AF01
                FreeOnUnwind guard{memory};                                    // state 0 (0040AF03)
                label = kotor_fastcall_0041acd0(memory, nullptr);         // 0040AF12
                guard.block = nullptr;                                         // state -1 (0040AF27)
            }
            SP(manager, 0x7C, label);                  // 0040AF1B
            ResRefStorage fill_name;                                           // [esp+34h]
            async_ref_construct_string_00406d60(&fill_name, nullptr,
                static_cast<unsigned char*>(manager) + 0x74);                  // 0040AF1F..0040AF32
            ParamsStorage text_params;                                         // [esp+64h]
            kotor_fastcall_00417050(&text_params, nullptr);         // 0040AF37..0040AF3B
            TextParamsOnUnwind text_guard{&text_params};                       // state 1 (0040AF9D)
            const I32 width = S16(manager, 0x6C);                              // 0040AF42
            const I32 height = S16(manager, 0x6E);                             // 0040AF46
            I32 extent[4] = {0, 0, width, height};                             // 0040AF56/0040AF7C/0040AF84/0040AF8C
            color[0] = 1.0f; color[1] = 1.0f; color[2] = 1.0f;                 // 0040AF64..0040AF74
            InitializeSlot initialize;                                         // 0040AF90 vtbl read
            std::memcpy(&initialize, static_cast<const unsigned char*>(P(P(manager, 0x7C), 0)) + 0x9C,
                        sizeof initialize);
            ResRefStorage edge_name, corner_name;                              // [esp+54h], [esp+44h]
            void* edge = kotor_fastcall_00405ed0(&edge_name, nullptr);       // 0040AFAC
            void* corner = kotor_fastcall_00405ed0(&corner_name, nullptr);   // 0040AFB6
            ParamsStorage border_params;                                       // [esp+B8h]
            void* border = kotor_fastcall_00415000(&border_params, nullptr, corner,
                edge, &fill_name, 0, 0, 0x3F800000u, color, 0);                // 0040AF4A..0040AFC3
            initialize(P(manager, 0x7C), nullptr, extent, &text_params, border, 0x3F800000u);   // 0040AFC8..0040AFD6
            text_guard.params = nullptr;                                       // state -1 (0040AFE0)
            frame_LogStringDtor_005e5c20(&text_params, nullptr);                // 0040AFDC..0040AFEB
        }
        ResRefStorage fill_name;                                               // [esp+24h]
        async_ref_construct_string_00406d60(&fill_name, nullptr,
            static_cast<unsigned char*>(manager) + 0x74);                      // 0040AFF1..0040AFF9
        gui_load_border_params_set_fill_00414c00(
            static_cast<unsigned char*>(P(manager, 0x7C)) + 0x70, nullptr, &fill_name, 0);   // 0040AFFE..0040B00B
        void* label = P(manager, 0x7C);                                        // 0040B010
        SW(label, 0x8C, (W(label, 0x8C) & 0xFFFFFFFEu) | 2u);                  // 0040B013..0040B022 fill style 2
        label = P(manager, 0x7C);                                              // 0040B025
        gui_render::DeltaSlot draw;
        std::memcpy(&draw, static_cast<const unsigned char*>(P(label, 0)) + 0x38, sizeof draw);
        draw(label, nullptr, delta_bits);                                      // 0040B028..0040B032
        kotor_AurGUICloseViewport_00459580();                                  // 0040B035
    }
    kotor_AurGUIStopLayer_004592B0();                                          // 0040B03A
}
