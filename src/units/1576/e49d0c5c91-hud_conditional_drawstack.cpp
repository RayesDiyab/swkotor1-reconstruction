// A5 private reconstruction of original 00685330
// CSWGuiMainInterfaceChar::DrawStack — thiscall(float, U32, U32), RET 0xC.
//
// New TU only. No shared header, vtable table, provider or Draw TU is edited.
// Original span 00685330..00685540. The selected label is this+0xA48 when the
// third argument is nonzero, else this+0xB88. Label vtable slots:
//   +04 SetExtent -> 00417780; +0x38 Draw -> 00417750.
// The function has no x87 and no _ftol2. The original uses one SetExtent + one
// Draw per iteration, then restores the saved extent and closes the viewport.

#include <cstdint>
#include <cstring>

namespace {

struct Extent {
    std::uint32_t v[4];
};

using SetExtentFn = void (__fastcall*)(void*, void*, const Extent*);
using DrawFn = void (__fastcall*)(void*, void*, float);

inline void set_extent(void* object, const Extent* extent) {
    void** vtable = *reinterpret_cast<void***>(object);
    reinterpret_cast<SetExtentFn>(vtable[1])(object, nullptr, extent);
}

inline void draw(void* object, float delta) {
    void** vtable = *reinterpret_cast<void***>(object);
    reinterpret_cast<DrawFn>(vtable[0x38 / 4])(object, nullptr, delta);
}

}  // namespace

extern "C" void __stdcall kotor_AurGUIStartLayer_004591B0();
extern "C" unsigned __cdecl kotor_AurGUISetupViewport_004592F0(
    unsigned x, unsigned y, unsigned width, unsigned height,
    const void* color, unsigned depth_test_zero, unsigned alpha_bits);
extern "C" void __stdcall kotor_AurGUICloseViewport_00459580();
extern "C" void __stdcall kotor_AurGUIStopLayer_004592B0();
extern "C" const std::uint32_t scene3d_gui_scene_color_0078d3d8[3];

extern "C" void __fastcall kotor_fastcall_00685330(
    void* self, void*, float delta, std::uint32_t first, std::uint32_t second) {
    std::uint8_t* base = static_cast<std::uint8_t*>(self);

    Extent viewport_extent;
    std::memcpy(&viewport_extent, base + 0x66C, sizeof(viewport_extent));

    kotor_AurGUIStartLayer_004591B0();
    if (kotor_AurGUISetupViewport_004592F0(
            viewport_extent.v[0], viewport_extent.v[1],
            viewport_extent.v[2], viewport_extent.v[3],
            scene3d_gui_scene_color_0078d3d8, 0, 0x3F800000u) == 0) {
        kotor_AurGUIStopLayer_004592B0();
        return;
    }

    std::uint8_t count = *reinterpret_cast<const std::uint8_t*>(
        base + (second != 0 ? 0xEA0 : 0xEA1));
    if (first != 0) {
        if (count > 9) count = 9;
    } else {
        if (count > 5) count = 5;
    }

    std::uint8_t* label = base + (second != 0 ? 0xA48 : 0xB88);
    Extent saved;
    Extent current;
    std::memcpy(&saved, label + 4, sizeof(saved));
    current = saved;

    std::uint32_t step = 0;
    if (count < 5) {
        step = (first != 0) ? 10u : 5u;
        const int half = static_cast<int>(step >> 1);
        const int distance = half * (static_cast<int>(count) - 1);
        current.v[1] = (second != 0)
            ? static_cast<std::uint32_t>(static_cast<std::int32_t>(current.v[1]) - distance)
            : static_cast<std::uint32_t>(static_cast<std::int32_t>(current.v[1]) + distance);
    } else {
        current.v[1] = (second != 0)
            ? 1u
            : static_cast<std::uint32_t>(
                  static_cast<std::int32_t>(viewport_extent.v[3]) - (first != 0 ? 16 : 8) - 1);
        static const std::uint8_t kStep[5] = {10, 8, 7, 6, 5};
        step = (first != 0) ? kStep[count - 5] : 5u;
    }

    for (std::int32_t i = 0; i < static_cast<std::int32_t>(count); ++i) {
        set_extent(label, &current);
        draw(label, delta);

        current.v[1] = (second != 0)
            ? static_cast<std::uint32_t>(
                  static_cast<std::int32_t>(current.v[1]) + static_cast<std::int32_t>(step))
            : static_cast<std::uint32_t>(
                  static_cast<std::int32_t>(current.v[1]) - static_cast<std::int32_t>(step));

        if ((count == 7 || count == 8) && (i == 0 || i == 2)) {
            current.v[1] = (second != 0)
                ? static_cast<std::uint32_t>(static_cast<std::int32_t>(current.v[1]) - 1)
                : static_cast<std::uint32_t>(static_cast<std::int32_t>(current.v[1]) + 1);
        }
    }

    set_extent(label, &saved);
    kotor_AurGUICloseViewport_00459580();
    kotor_AurGUIStopLayer_004592B0();
}
