#include "deb5c50775-renderer_initialize.h"
#include "3d785914bb-allocator_live.h"
#include <cstring>
#include <cstdint>
namespace kotor_renderer_init {
template<class T> static T Load(const U8* p) { T v; std::memcpy(&v, p, sizeof(v)); return v; }
template<class T> static void Store(U8* p, T v) { std::memcpy(p, &v, sizeof(v)); }
I32 __thiscall Renderer::Initialize(void* top_window, void* window, U32 width,
    U32 height, U8 bits, U32 frequency, I32 fullscreen, I32 color, I32 depth) {
    (void)top_window; // First stack argument is never read in the original body.
    if (*shared.initialized_0078e38c != 0) {
        *shared.initialized_0078e38c = 0;
        InitializeAntiAliasing_0042e040();
    }
    *shared.format_0078e388 = 0xffffu;
    if (fullscreen != 0) {
        U32 index = 0;
        DisplayMode mode{};
        Store<U16>(mode.bytes + 36, 156);
        while (EnumDisplaySettingsA_0073d3e4(nullptr, index, &mode) != 0) {
            if (Load<U32>(mode.bytes + 108) == width && Load<U32>(mode.bytes + 112) == height &&
                Load<U32>(mode.bytes + 104) == bits && Load<U32>(mode.bytes + 120) == frequency) {
                if (ChangeDisplaySettingsA_0073d3e8(&mode, 2) != 0) return 0;
                std::memcpy(saved_mode.bytes, mode.bytes, sizeof(mode));
                (void)ChangeDisplaySettingsA_0073d3e8(&mode, 4);
                mode_changed = 1;
                break;
            }
            ++index;
            std::memset(mode.bytes, 0, sizeof(mode));
            Store<U16>(mode.bytes + 36, 156);
        }
    }
    const bool use_popup = fullscreen != 0 && mode_changed != 0;
    const U32 style = use_popup ? 0x82000000u : 0x02cf0000u;
    fullscreen_style = use_popup ? 1u : 0u;
    (void)SetWindowLongA_0073d3ec(window, -16, style);
    Rect rect{0, 0, width, height};
    (void)AdjustWindowRect_0073d3e0(&rect, style, 0);
    const U32 outer_width = rect.right - rect.left;
    const U32 outer_height = rect.bottom - rect.top;
    // lane-m52-precheck fix: hWndInsertAfter is the local [esp+14h] = -1 (HWND_TOPMOST) when the popup style is
    // used (0044DBF4) or -2 (HWND_NOTOPMOST) otherwise (0044DC11). At 0044DC66 `mov edx,[esp+18h]` executes after
    // `push 120h` (0044DC5F), so it reads that local, not `this` ([esp+18h] before the push, 0044DADD).
    void* const insert_after = reinterpret_cast<void*>(static_cast<std::intptr_t>(use_popup ? -1 : -2));
    (void)SetWindowPos_0073d40c(window, insert_after, 0, 0, outer_width, outer_height, 0x120); // 0044DC5F..0044DC72
    (void)SendMessageA_0073d428(window, 5, 0, ((outer_height & 0xffffu) << 16) | (outer_width & 0xffffu));
    (void)ShowWindow_0073d400(window, 1);
    void* dc = GetDC_0073d408(window);
    if (!dc) return 0;
    *shared.color_007b9220 = 32;
    *shared.depth_007b9224 = 24;
    bool fallback = false;
    if (*shared.aa_007a6888 != 0) {
        if (SetAntiAliasingMode_0044f2f0(*shared.aa_007a6888) == 0) fallback = true;
        else if (*shared.aa_007a6888 != 0) {
            const I32 attributes[20] = {0x2001, 1, 0x2003, 0x2027, 0x2014, color,
                0x201b, 8, 0x2022, depth > 24 ? 24 : depth, 0x2011, 1,
                0x2041, 1, 0x2042, *shared.aa_007a6888, 0x2023, 8, 0, 0};
            const float floats[2] = {0.0f, 0.0f};
            I32 format;
            U32 count;
            if (choose_arb_007bb730(dc, attributes, floats, 1, &format, &count) == 0 || count == 0) fallback = true;
            else {
                // Original passes an uninitialized 40-byte descriptor here.
                // Its contents are unspecified; synthetic tests never read them.
                PixelFormat unspecified;
                if (SetPixelFormat_0073d014(dc, format, &unspecified) == 0) fallback = true;
                *shared.color_007b9220 = static_cast<U16>(color);
                *shared.depth_007b9224 = 24;
            }
        }
    }
    if (fallback || *shared.aa_007a6888 == 0) {
        PixelFormat pfd{};
        Store<U16>(pfd.bytes, 40);
        Store<U16>(pfd.bytes + 2, 1);
        Store<U32>(pfd.bytes + 4, 0x25);
        pfd.bytes[9] = 32;
        pfd.bytes[16] = 8;
        pfd.bytes[23] = 24;
        pfd.bytes[24] = 8;
        const I32 format = ChoosePixelFormat_0073d018(dc, &pfd);
        if (format == 0 || SetPixelFormat_0073d014(dc, format, nullptr) == 0) {
            (void)ReleaseDC_0073d404(window, dc);
            return 0;
        }
        (void)DescribePixelFormat_0073d008(dc, format, 40, &pfd);
        *shared.color_007b9220 = pfd.bytes[9];
        *shared.depth_007b9224 = pfd.bytes[23];
    }
    context = wglCreateContext_0073d2b8(dc);
    if (!context) {
        (void)ReleaseDC_0073d404(window, dc);
        return 0;
    }
    if (wglMakeCurrent_0073d2b0(dc, context) == 0) {
        // Original failed-current path rereads +B4 and deletes unconditionally.
        (void)wglDeleteContext_0073d30c(context);
        (void)ReleaseDC_0073d404(window, dc);
        return 0; // Preserve the live word, including provider changes.
    }
    InitExtensions_0044f070();
    if (VertexProgramsAvailable_0045f770() != 0) TrackMatrices_004a2400();
    VoidReturn_004015a0(0x01000000);
    InitRenderSystem_00425c30();
    TextureRefreshAll_00422360();
    InitializeSoftShadows_00426cc0();
    InitializeFrameBufferModifications_00427c90();
    if (VertexProgramsAvailable_0045f770() != 0) {
        glEnable_0073d390(*shared.vertex_enum_0078d46c);
        for (unsigned i = 0; i < 10; ++i) {
            void* program = live_operator_new(0x201c);
            if (program) {
                const auto& binding = shared.programs[i];
                char* source = *shared.programs_0078e5ec != 0 ?
                    (binding.dynamic_nonzero_source ? *binding.dynamic_nonzero_source : binding.nonzero_source) : binding.zero_source;
                program = construct_program_004a27d0(program, nullptr, source, *binding.constructor_parameters);
            }
            *shared.programs[i].output = program;
            // Even a null allocation still reaches Enable(this=null,0,arg).
            enable_program_004a24d0(program, nullptr, nullptr, static_cast<U8>((i == 1 || i == 2 || i == 4) ? 1 : 0));
        }
        glDisable_0073d22c(*shared.vertex_enum_0078d46c);
    }
    if (*shared.aa_007a6888 > 0 && !fallback) glEnable_0073d390(0x809d);
    else if (*shared.old_aa_0078d440 > 0) glDisable_0073d22c(0x809d);
    (void)swap_interval_007bb538(*shared.vsync_0078d438 != 0 ? 1 : 0);
    return 1; // No success ReleaseDC in the observed body.
}
}
