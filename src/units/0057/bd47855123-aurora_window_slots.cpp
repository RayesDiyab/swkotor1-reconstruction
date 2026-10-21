// lane-window: CAurInternalGL native slot bodies + typed table 00741518 / base table 007414F0.
// Evidence: dumpbin /disasm swkotor.exe (GOG, sha256 9c10e045...1435) ranges cited per line.
// Donors reused (task-22, 2026-10-02): renderer-factory-slots-r1/source/slots.cpp
// (0044D6D0/0044D6E0/0044D970/0044D980, bodies unchanged), renderer-context-restore-r1/
// source/restore.cpp (0044D8D0/0044D900; gamma dispatch re-expressed through the typed
// table instead of an integer->__thiscall pointer cast), renderer-adoption-packet-r1/
// source/gamma.cpp (0044D990; cells re-bound to kotor_menu_window_cells, gdi call direct).
#include "c117bb715c-aurora_window_slots.h"
#include "9d61fbd0c6-initializer_slot.h"
#include "03e67ad358-window_shared_cells.h"
#include "6542d604d6-allocator_live.h"
#include <windows.h>
#include <GL/gl.h>
#include <cstring>
// Existing fatal CRT provider; same int (__cdecl*)() BaseVTable ABI.
extern "C" int __cdecl _purecall(void);

using namespace kotor_renderer_init;

// ---------------- borrowed providers (declarations only) ----------------
extern "C" {
// 0044E3B0 InitEditor (+10): defined in aurora_editor.cpp.
std::int32_t __fastcall genuine_editor_0044e3b0(Renderer*, void*, std::uint32_t, std::uint32_t,
    std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
// 004A2470 ResetVertexPrograms (cdecl, no args) - name already requested by lane-3d-render.
void __cdecl vertex_program_manager_reset_004a2470();
// 004A2830 CAurVertexProgram::~CAurVertexProgram (__thiscall, RET 0).
void __fastcall vertex_program_destroy_004a2830(void* program, void*);
// 00432180 renderer GL resource Shutdown(release) (cdecl, one word; caller pops at 0044EBB5).
void __cdecl render_shutdown_00432180(std::uint32_t release);
// Original 006FAE8C/006FC1F0 x87 transports (consumed only from inline asm below).
void __cdecl genuine_x87_ftol_006fae8c();   // M41 renderer-x87-ftol-alias.obj
void __cdecl genuine_x87_pow_006fc1f0();    // lane-font (task-8 pow_entry donor)
// 0044EC60 cleanup callees (cdecl).
const char* __cdecl aurora_untrace_0044d250(const char* name);   // untrace(0): 0044EC83 push 0 (EAX result ignored)
void __cdecl aurora_strings_free_0044d1e0();
void __cdecl aurora_free_font_0044cc60();
void __cdecl aurora_delete_all_fonts_004a22e0();
}
// Program output cells (task-22 renderer-adoption-packet-r1 program_cells.cpp names).
namespace renderer_program_cells {
extern void* output_00827FEC; extern void* output_00827FF0; extern void* output_00827FF4;
extern void* output_00827FF8; extern void* output_00827FFC; extern void* output_00828014;
extern void* output_0083037C; extern void* output_00830380; extern void* output_00830388;
extern void* output_0083038C;
}
// Program text cells 0083028C/00830290 (lane-renderer-bodies renderer_gl_loader_cells.cpp).
namespace kotor_renderer_gl_cells {
extern char* first_program_0083028c;
extern char* second_program_00830290;
extern std::uint32_t width_0078d1d4;   // lane-renderer-bodies renderer_gl_cells.cpp (see open gate)
}

namespace kotor_aurora_window {
void* output_00830384 = nullptr;
}

namespace {
template<class T> T read(const void* p) { T v; std::memcpy(&v, p, sizeof v); return v; }
template<class T> void write(void* p, T v) { std::memcpy(p, &v, sizeof v); }
U32 field(const DisplayMode& d, unsigned n) { return read<U32>(d.bytes + n); }
U32 address(const void* p) { return static_cast<U32>(reinterpret_cast<std::uintptr_t>(p)); }
// Runtime table of the receiver ([ecx]); object-pointer conversion only (D24).
const renderer_adoption::VTable* table_of(const Renderer* self) {
    return reinterpret_cast<const renderer_adoption::VTable*>(static_cast<std::uintptr_t>(self->vtable_word));
}
// gamma constants: 0073F3F0 = 1/255 (double 0x3F70101010101010), 007414E8 = 255.0 (0x406FE00000000000)
const std::uint64_t gamma_ratio_bits_0073f3f0 = 0x3f70101010101010ull;
const std::uint64_t gamma_scale_bits_007414e8 = 0x406fe00000000000ull;
}

// ---------------- +04 0044D6D0 ----------------
extern "C" U32 __fastcall aurora_fullscreen_0044d6d0(Renderer* p, void*) {
    return p->fullscreen_style;                                     // 0044D6D0 mov eax,[ecx+0ACh]
}
// ---------------- +28 0044D970 / +2C 0044D980 ----------------
extern "C" void* __fastcall aurora_get_context_0044d970(Renderer* p, void*) {
    return p->context;                                              // 0044D970 mov eax,[ecx+0B4h]
}
extern "C" I32 __stdcall aurora_make_current_0044d980(void* dc, void* rc) {
    return wglMakeCurrent_0073d2b0(dc, rc);                         // 0044D980 jmp [0073D2B0]
}
// ---------------- +0C 0044D6E0 GetValidMode (task-22 slots.cpp body) ----------------
extern "C" U32 __stdcall aurora_valid_mode_0044d6e0(U32* width, U32* height, U8* color, U32* frequency, U32 flag) {
    DisplayMode selected{}; write<U16>(selected.bytes + 36, 156);  // 0044D6EF..0044D72B rep stos, dmSize=9Ch
    bool found = false; U32 index = 0;
    for (;;) {
        DisplayMode current{}; write<U16>(current.bytes + 36, 156); // 0044D7D1..0044D7F1
        if (!EnumDisplaySettingsA_0073d3e4(nullptr, index, &current)) break; // 0044D730/0044D7FF
        const U32 w = field(current, 108), h = field(current, 112), c = field(current, 104), f = field(current, 120);
        const U32 sw = field(selected, 108), sh = field(selected, 112), sc = field(selected, 104), sf = field(selected, 120);
        const bool bounds = w <= read<U32>(width) && h <= read<U32>(height) && c <= read<U8>(color) && f <= read<U32>(frequency); // 0044D74A..0044D77D (ja: unsigned)
        if (bounds && w >= sw && h >= sh && c >= sc && (f >= sf || w != sw || h != sh || c != sc)) { // 0044D786..0044D7B8
            selected = current; found = true;                        // 0044D7BA rep movs; 0044D7CC flag=1
        }
        ++index;                                                     // 0044D7EC inc eax
    }
    if (!found) return 0;                                            // 0044D813 -> 0044D826 xor al,al (EAX was 0 from Enum)
    const U32 os = static_cast<U32>(ChangeDisplaySettingsA_0073d3e8(&selected, 2)); // 0044D815 CDS_TEST
    if (os) return os & 0xffffff00u;                                 // 0044D826 xor al,al keeps upper EAX
    if (static_cast<U8>(flag) != 0) {                                // 0044D831 mov al,[ebp+18h]
        const U32 w = field(selected, 108), h = field(selected, 112); const U8 c = static_cast<U8>(field(selected, 104)); const U32 f = field(selected, 120);
        write(width, w); write(height, h); write(color, c); write(frequency, f); // 0044D849..0044D866
        return (static_cast<U32>(reinterpret_cast<std::uintptr_t>(color)) & 0xffffff00u) | 1u; // 0044D868 mov al,1 (EAX=color ptr)
    }
    return field(selected, 108) == read<U32>(width) && field(selected, 112) == read<U32>(height) &&
           field(selected, 104) == read<U8>(color) && field(selected, 120) == read<U32>(frequency) ? 1u : 0u; // 0044D873..0044D8BC
}
// ---------------- +24 0044D990 ResetGammaRamp ----------------
extern "C" U32 __fastcall genuine_gamma_0044d990(Renderer* self, void*, U32 gamma_bits, U32 keep) {
    if (keep == 0) self->untouched_b0 = gamma_bits;                  // 0044D990..0044D9A5
    U16 ramp[3 * 256];                                               // 0044D994 sub esp,604h (600h ramp)
    for (unsigned channel = 0; channel < 3; ++channel) {             // 0044D9B2 ebx=3
        for (I32 index = 0; index < 256; ++index) {                  // 0044D9C0..0044D9FF
            U32 integer;
            __asm {
                fild index                                           ; 0044D9C6
                fmul qword ptr gamma_ratio_bits_0073f3f0             ; 0044D9CA
                fld dword ptr gamma_bits                             ; 0044D9D0
                call genuine_x87_pow_006fc1f0                        ; 0044D9D7
                fmul qword ptr gamma_scale_bits_007414e8             ; 0044D9DC
                call genuine_x87_ftol_006fae8c                       ; 0044D9E2
                mov integer, eax
            }
            ramp[channel * 256 + static_cast<unsigned>(index)] =
                static_cast<U16>(((integer & 255u) << 8) | static_cast<U32>(index)); // 0044D9E7..0044D9F7 ch=al, or esi
        }
    }
    void* dc = GetDC_0073d408(kotor_menu_window_cells::render_007a39d8); // 0044DA04..0044DA0B
    if (!dc) return 0;                                               // 0044DA16..0044DA1E (EAX=0)
    (void)::SetDeviceGammaRamp(static_cast<HDC>(dc), ramp);          // 0044DA27 [0073D020]; result ignored
    return 1;                                                        // 0044DA2D; DC is not released (original)
}
// ---------------- +18 0044D8D0 / +1C 0044D900 ----------------
extern "C" U32 __fastcall aurora_restore_desktop_0044d8d0(Renderer* self, void*) {
    (void)table_of(self)->gamma(self, nullptr, 0x3f800000u, 1);      // 0044D8D3..0044D8DC call [eax+24h](1.0f,1)
    const U32 mode_changed = self->mode_changed;                     // 0044D8DF
    if (!mode_changed) return 0;                                     // 0044D8E8 (EAX = 0)
    return static_cast<U32>(ChangeDisplaySettingsA_0073d3e8(nullptr, 0)); // 0044D8EA..0044D8EE
}
extern "C" U32 __fastcall aurora_restore_display_0044d900(Renderer* self, void*) {
    (void)table_of(self)->gamma(self, nullptr, self->untouched_b0, 1); // 0044D903..0044D910
    if (!self->mode_changed) return 0;                               // 0044D913..0044D91B
    const I32 test = ChangeDisplaySettingsA_0073d3e8(&self->saved_mode, 2); // 0044D924..0044D92A (this+4)
    if (test != 0) return static_cast<U32>(test);                    // 0044D92E jne -> 0044D935
    return static_cast<U32>(ChangeDisplaySettingsA_0073d3e8(&self->saved_mode, 4)); // 0044D930..0044D933
}
// ---------------- +14 0044E9E0 Reshape ----------------
extern "C" void __fastcall genuine_reshape_0044e9e0(Renderer* self, void*, U32 width, U32 height) {
    if (!self->context) return;                                      // 0044E9E0..0044E9E8
    self->untouched_a0 = width;                                      // 0044E9F4
    self->untouched_a4 = height;                                     // 0044E9FA
    kotor_renderer_gl_cells::width_0078d1d4 = width;                 // 0044EA00
    kotor_menu_window_cells::height_0078d1d8 = height;               // 0044EA06
    const U32 h = height ? height : 1u;                              // 0044EA0B..0044EA0D
    const U32 w = width ? width : 1u;                                // 0044EA12..0044EA16
    ::glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h)); // 0044EA1B..0044EA21 [0073D2BC]
}
// ---------------- +20 0044EA30 ShutDown ----------------
namespace {
void release_program(void*& cell) {                                  // 0044EA3A.. pattern (x11)
    if (void* program = cell) {
        vertex_program_destroy_004a2830(program, nullptr);           // call 004A2830 (ecx=program)
        live_free(program);                                          // call 006FA390
        cell = nullptr;                                              // mov [cell],esi(0)
    }
}
}
extern "C" void __fastcall genuine_shutdown_0044ea30(Renderer* self, void*, U32 release, U32 restore) {
    vertex_program_manager_reset_004a2470();                         // 0044EA35
    release_program(renderer_program_cells::output_0083037C);        // 0044EA3A
    release_program(renderer_program_cells::output_00830380);        // 0044EA5C
    release_program(kotor_aurora_window::output_00830384);           // 0044EA7C
    release_program(renderer_program_cells::output_00827FEC);        // 0044EA9C
    release_program(renderer_program_cells::output_00827FF0);        // 0044EABC
    release_program(renderer_program_cells::output_00827FF4);        // 0044EADC
    release_program(renderer_program_cells::output_00827FF8);        // 0044EAFC
    release_program(renderer_program_cells::output_00827FFC);        // 0044EB1C
    release_program(renderer_program_cells::output_00830388);        // 0044EB3C
    release_program(renderer_program_cells::output_0083038C);        // 0044EB5C
    release_program(renderer_program_cells::output_00828014);        // 0044EB7C
    if (release) ::renderer_void_004015a0();                         // 0044EBA1..0044EBA5 (no argument pushed)
    render_shutdown_00432180(release);                               // 0044EBAA..0044EBB5 (always)
    if (char* text = kotor_renderer_gl_cells::first_program_0083028c) { live_free(text); kotor_renderer_gl_cells::first_program_0083028c = nullptr; } // 0044EBB0..0044EBC5
    if (char* text = kotor_renderer_gl_cells::second_program_00830290) { live_free(text); kotor_renderer_gl_cells::second_program_00830290 = nullptr; } // 0044EBCB..0044EBDD
    if (self->context) {                                             // 0044EBE3
        (void)wglMakeCurrent_0073d2b0(nullptr, nullptr);             // 0044EBEB..0044EBED
        (void)wglDeleteContext_0073d30c(self->context);              // 0044EBF3..0044EBFA (reloaded)
        self->context = nullptr;                                     // 0044EC00
    }
    if (self->mode_changed) {                                        // 0044EC06
        if (restore) (void)ChangeDisplaySettingsA_0073d3e8(nullptr, 0); // 0044EC12..0044EC18
        self->mode_changed = 0;                                      // 0044EC1E
        self->fullscreen_style = 0;                                  // 0044EC24
    }
    if (release) (void)table_of(self)->gamma(self, nullptr, 0x3f800000u, 0); // 0044EC2A..0044EC3A [edx+24h](1.0f,0)
    HWND window = kotor_menu_window_cells::render_007a39d8;          // 0044EC3D
    if (window && restore) {                                         // 0044EC42..0044EC48
        (void)::DestroyWindow(window);                               // 0044EC4B [0073D3FC]
        kotor_menu_window_cells::render_007a39d8 = nullptr;          // 0044EC51
    }
}
// ---------------- 0044EC60 / +00 0044ED40 / base 0044DA40 ----------------
namespace {
// EH state 0 (FuncInfo 0076309C via 00716578): unwind runs 0044DAA0 (base table store).
// Normal exit performs the same store at 0044ECB3; the guard expresses both.
struct BaseTableOnExit {
    Renderer* self;
    ~BaseTableOnExit() { self->vtable_word = address(&kotor_aurora_window::base_vtable_007414f0); }
};
}
extern "C" void __fastcall aurora_destroy_0044ec60(Renderer* self, void*) {
    self->vtable_word = address(&kotor_aurora_window::native_vtable_00741518); // 0044EC7D
    BaseTableOnExit guard{self};                                     // 0044EC85 state 0
    (void)aurora_untrace_0044d250(nullptr);                              // 0044EC83..0044EC92 push 0
    aurora_strings_free_0044d1e0();                                  // 0044EC95
    aurora_free_font_0044cc60();                                     // 0044EC9A
    aurora_delete_all_fonts_004a22e0();                              // 0044EC9F
    genuine_shutdown_0044ea30(self, nullptr, 1, 1);                  // 0044ECA4..0044ECAA
}
extern "C" U32 __fastcall genuine_delete_0044ed40(Renderer* self, void*, U32 flags) {
    aurora_destroy_0044ec60(self, nullptr);                          // 0044ED43
    if (static_cast<U8>(flags) & 1u) live_free(self);                // 0044ED48..0044ED55
    return address(self);                                            // 0044ED58 mov eax,esi; RET 4
}
extern "C" U32 __fastcall aurora_base_delete_0044da40(Renderer* self, void*, U32 flags) {
    self->vtable_word = address(&kotor_aurora_window::base_vtable_007414f0); // 0044DA48
    if (static_cast<U8>(flags) & 1u) live_free(self);                // 0044DA40 test [esp+4],1; 0044DA51
    return address(self);                                            // 0044DA59; RET 4
}

// ---------------- tables ----------------
namespace kotor_aurora_window {
extern const renderer_adoption::VTable native_vtable_00741518 = {
    &genuine_delete_0044ed40,                     // +00 0044ED40
    &aurora_fullscreen_0044d6d0,                  // +04 0044D6D0
    &aurora_context_adoption::initialize_slot,    // +08 0044DAB0 (existing M41 transport -> Renderer::Initialize)
    &aurora_valid_mode_0044d6e0,                  // +0C 0044D6E0
    &genuine_editor_0044e3b0,                     // +10 0044E3B0 (aurora_editor.cpp)
    &genuine_reshape_0044e9e0,                    // +14 0044E9E0
    &aurora_restore_desktop_0044d8d0,             // +18 0044D8D0
    &aurora_restore_display_0044d900,             // +1C 0044D900
    &genuine_shutdown_0044ea30,                   // +20 0044EA30
    &genuine_gamma_0044d990,                      // +24 0044D990
    &aurora_get_context_0044d970,                 // +28 0044D970
    &aurora_make_current_0044d980,                // +2C 0044D980
};
extern const BaseVTable base_vtable_007414f0 = {
    &aurora_base_delete_0044da40,                 // 007414F0 -> 0044DA40
    {&_purecall, &_purecall, &_purecall, &_purecall, &_purecall, &_purecall, &_purecall, &_purecall, &_purecall}, // 006FB8DE x9
};
}
static_assert(sizeof(kotor_aurora_window::BaseVTable) == 40, "007414F0..00741517: ten native words");
