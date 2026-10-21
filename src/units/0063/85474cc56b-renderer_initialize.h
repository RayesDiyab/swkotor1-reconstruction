#pragma once
#include <cstddef>
#include <cstdint>
namespace kotor_renderer_init {
using U32 = std::uint32_t;
using I32 = std::int32_t;
using U16 = std::uint16_t;
using U8 = std::uint8_t;
struct DisplayMode { U8 bytes[156]; };
struct PixelFormat { U8 bytes[40]; };
struct Rect { U32 left, top, right, bottom; };
struct Renderer {
    U32 vtable_word;
    DisplayMode saved_mode;
    U32 untouched_a0, untouched_a4;
    U32 mode_changed, fullscreen_style, untouched_b0;
    void* context;
    I32 __thiscall Initialize(void* top_window, void* render_window, U32 width,
        U32 height, U8 display_bits, U32 frequency, I32 fullscreen,
        I32 requested_color_bits, I32 requested_depth_bits);
};
static_assert(sizeof(Renderer) == 0xb8 && offsetof(Renderer, saved_mode) == 4,
    "Saved x86 receiver extent");
static_assert(offsetof(Renderer, mode_changed) == 0xa8 &&
    offsetof(Renderer, fullscreen_style) == 0xac && offsetof(Renderer, context) == 0xb4,
    "Observed receiver writes");
using ConstructProgram = void* (__fastcall*)(void*, void*, char*, U32*);
using EnableProgram = void (__fastcall*)(void*, void*, const void*, U8);
struct ProgramBinding {
    char* nonzero_source;
    char* zero_source;
    char* const* dynamic_nonzero_source;
    U32* const* constructor_parameters;
    void** output;
    I32 second_enable_argument;
};
// Borrowed live bindings only. Integrator supplies canonical globals/resources.
struct SharedBindings {
    I32* initialized_0078e38c;
    U16* format_0078e388;
    I32* aa_007a6888;
    U16* color_007b9220;
    U16* depth_007b9224;
    U32* vertex_enum_0078d46c;
    I32* old_aa_0078d440;
    U8* vsync_0078d438;
    I32* programs_0078e5ec;
    ProgramBinding programs[10];
};
extern SharedBindings& shared;
extern const ConstructProgram construct_program_004a27d0;
extern const EnableProgram enable_program_004a24d0;
extern I32 (__stdcall* const choose_arb_007bb730)(void*, const I32*, const float*, U32, I32*, U32*);
extern I32 (__stdcall* const swap_interval_007bb538)(I32);
// Unresolved exact ABI providers; fixture implementations are explicitly synthetic.
extern I32 __stdcall EnumDisplaySettingsA_0073d3e4(const char*, U32, DisplayMode*);
extern I32 __stdcall ChangeDisplaySettingsA_0073d3e8(DisplayMode*, U32);
extern I32 __stdcall SetWindowLongA_0073d3ec(void*, I32, U32);
extern I32 __stdcall AdjustWindowRect_0073d3e0(Rect*, U32, I32);
extern I32 __stdcall SetWindowPos_0073d40c(void*, void*, I32, I32, U32, U32, U32);
extern I32 __stdcall SendMessageA_0073d428(void*, U32, U32, U32);
extern I32 __stdcall ShowWindow_0073d400(void*, I32);
extern void* __stdcall GetDC_0073d408(void*);
extern I32 __stdcall SetPixelFormat_0073d014(void*, I32, const PixelFormat*);
extern I32 __stdcall ChoosePixelFormat_0073d018(void*, const PixelFormat*);
extern I32 __stdcall DescribePixelFormat_0073d008(void*, I32, U32, PixelFormat*);
extern void* __stdcall wglCreateContext_0073d2b8(void*);
extern I32 __stdcall wglMakeCurrent_0073d2b0(void*, void*);
extern I32 __stdcall wglDeleteContext_0073d30c(void*);
extern I32 __stdcall ReleaseDC_0073d404(void*, void*);
extern void __stdcall glEnable_0073d390(U32);
extern void __stdcall glDisable_0073d22c(U32);
extern void __cdecl InitializeAntiAliasing_0042e040();
extern I32 __cdecl SetAntiAliasingMode_0044f2f0(I32);
extern void __cdecl InitExtensions_0044f070();
extern I32 __cdecl VertexProgramsAvailable_0045f770();
extern void __cdecl TrackMatrices_004a2400();
extern void __cdecl VoidReturn_004015a0(U32);
extern void __cdecl InitRenderSystem_00425c30();
extern void __cdecl TextureRefreshAll_00422360();
extern void __cdecl InitializeSoftShadows_00426cc0();
extern void __cdecl InitializeFrameBufferModifications_00427c90();
}
