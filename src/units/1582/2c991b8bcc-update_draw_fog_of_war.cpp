// Original 00688100 — CSWGuiMainInterface::UpdateAndDrawFogOfWar
// Full entry-to-return reconstruction.
//
// Span 00688100..006883B9 (0x2BC); thiscall ECX=this, one const Extent* stack
// arg, RET 4. Export @kotor_fastcall_00688100@12.
//
// B0 returns before Flush for map null (00688119), zero map count (00688126)
// and null GetMapData result (00688146). Only the normal/empty-bounds path runs
// the bit loop and reaches FlushBuffer at 006883A3.
//
// x87: sx=440/cellsX, sy=256/cellsY, sx4/sy4, invSx/invSy; K and bounds use the
// selected real __ftol2 route (_genuine_x87_ftol_006fae8c ->
// _unresolved_ftol2_006fae8c). No SSE cvttss2si, no 006FAE8C direct binding.

#include <cstdint>

namespace {

struct Extent {
    std::int32_t left;
    std::int32_t top;
    std::int32_t width;
    std::int32_t height;
};

std::uint32_t load_u32(const void* base, std::uintptr_t offset) {
    return *reinterpret_cast<const std::uint32_t*>(
        reinterpret_cast<std::uintptr_t>(base) + offset);
}

std::int32_t load_i32(const void* base, std::uintptr_t offset) {
    return *reinterpret_cast<const std::int32_t*>(
        reinterpret_cast<std::uintptr_t>(base) + offset);
}

extern "C" int __cdecl kotor_ftol2_006fae8c();
#pragma comment(linker, "/alternatename:_kotor_ftol2_006fae8c=_genuine_x87_ftol_006fae8c")

__declspec(noinline) std::int32_t x87_ftol(double value) {
    std::int32_t result;
    __asm {
        fld qword ptr [value]
        call kotor_ftol2_006fae8c
        mov result, eax
    }
    return result;
}

using DrawFogFn = void (__fastcall*)(void*, void*, float, float, float, float,
                                     int, int, const float*, float);

void draw_fog_quad(void* image, float u0, float v0, float du, float dv,
                   const float* color, float alpha) {
    void** const vtable = *reinterpret_cast<void***>(image);
    const DrawFogFn draw =
        reinterpret_cast<DrawFogFn>(vtable[0x20 / sizeof(void*)]);
    draw(image, nullptr, u0, v0, du, dv, 0, 0, color, alpha);
}

}  // namespace

namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
}

extern "C" void* __fastcall server_get_module_004ae6b0(void* app, void*);
extern "C" void* __fastcall kotor_fastcall_00579090(
    void* map, void*, std::uint32_t* out1, std::uint32_t* out2,
    std::uint32_t* out3);
extern "C" void __cdecl kotor_cdecl_0045a170(float alpha);
extern "C" float kotor_gui_image_alpha_0078e43c;
#pragma comment(linker, "/alternatename:_kotor_gui_image_alpha_0078e43c=?gui_image_alpha_0078e43c@kotor_gui_render_cells@@3IC")

extern "C" void __fastcall kotor_fastcall_00688100(
    void* self, void*, const Extent* extent) {
    void* const app = kotor_menu_window_cells::manager_007a39fc;
    void* const module = server_get_module_004ae6b0(
        *reinterpret_cast<void**>(
            reinterpret_cast<std::uintptr_t>(app) + 0x08), nullptr);
    void* const map = *reinterpret_cast<void**>(
        reinterpret_cast<std::uintptr_t>(module) + 0x218);
    if (map == nullptr) {
        return;  // 00688119 -> 006883B5, no Flush
    }
    if (load_u32(map, 0x04) == 0) {
        return;  // 00688126 -> 006883B5, no Flush
    }

    std::uint32_t map_count = 0;
    std::uint32_t cells_x_u = 0;
    std::uint32_t cells_y_u = 0;
    void* const bits = kotor_fastcall_00579090(
        map, nullptr, &map_count, &cells_x_u, &cells_y_u);
    (void)map_count;  // original writes out1 and never reads it
    if (bits == nullptr) {
        return;  // 00688146 -> 006883B5, no Flush
    }

    const std::int32_t cells_x = static_cast<std::int32_t>(cells_x_u);
    const std::int32_t cells_y = static_cast<std::int32_t>(cells_y_u);
    const float sx = 440.0f / static_cast<float>(cells_x);
    const float sy = 256.0f / static_cast<float>(cells_y);
    const float sx4 = sx * 4.0f;
    const float sy4 = sy * 4.0f;
    // The original keeps these reciprocals and products in x87 PC_53; use
    // double, not a stored float, so the ftol inputs keep 53-bit precision.
    const double inv_sx = 1.0 / static_cast<double>(sx);
    const double inv_sy = 1.0 / static_cast<double>(sy);

    const std::int32_t k = x87_ftol(
        static_cast<double>(sx4) * inv_sx * -0.5);
    const std::int32_t left_cell = x87_ftol(
        static_cast<double>(extent->left) * inv_sx);
    const std::int32_t top_cell = x87_ftol(
        static_cast<double>(extent->top) * inv_sy);

    std::int32_t x0 = k - left_cell;
    std::int32_t x1 = x87_ftol(
        static_cast<double>(load_i32(self, 0x6088)) * inv_sx) -
        k - left_cell;
    std::int32_t y0 = k - top_cell;
    std::int32_t y1 = x87_ftol(
        static_cast<double>(load_i32(self, 0x608C)) * inv_sy) -
        k - top_cell;

    if (y0 < 0) y0 = 0;
    if (y1 > cells_y) y1 = cells_y;
    if (x0 < 0) x0 = 0;
    if (x1 > cells_x) x1 = cells_x;

    std::int32_t row_bits = (cells_x + 1) * y0;
    float y_pix = static_cast<float>(extent->top) + static_cast<float>(y0) * sy;
    const float x_off = static_cast<float>(x0) * sx;

    if (y0 <= y1) {
        std::int32_t rows = y1 - y0 + 1;
        float x_pix = static_cast<float>(extent->left) + x_off;

        while (rows != 0) {
            std::int32_t bit = row_bits;
            if (x0 <= x1) {
                float col_pix = x_pix;
                for (std::int32_t col = x0; col <= x1; ++col, ++bit) {
                    const std::uint32_t mask = 1u << (bit & 31);
                    const std::uint32_t word =
                        static_cast<const std::uint32_t*>(bits)[bit >> 5];
                    if ((word & mask) == 0) {
                        const float u0 =
                            (col_pix - sx4 * 0.5f) /
                            static_cast<float>(load_i32(self, 0x6088));
                        const float v0 =
                            (y_pix - sy4 * 0.5f) /
                            static_cast<float>(load_i32(self, 0x608C));
                        const float du =
                            sx4 / static_cast<float>(load_i32(self, 0x6088));
                        const float dv =
                            sy4 / static_cast<float>(load_i32(self, 0x608C));
                        static const float kNoColor[3] = {
                            -1.0f, -1.0f, -1.0f};
                        void* const image = *reinterpret_cast<void**>(
                            reinterpret_cast<std::uintptr_t>(self) + 0xC8C8);
                        draw_fog_quad(image, u0, v0, du, dv, kNoColor, 1.0f);
                    }
                    col_pix += sx;
                }
            }
            y_pix += sy;
            row_bits += cells_x + 1;
            --rows;
        }
    }

    kotor_cdecl_0045a170(kotor_gui_image_alpha_0078e43c);
}
