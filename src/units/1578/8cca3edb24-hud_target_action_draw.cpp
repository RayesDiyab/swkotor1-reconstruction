// A5 private reconstruction of original 00685ED0
// CSWGuiTargetActionMenu::Draw — thiscall(float), RET 4.
//
// New TU only. No shared header, Draw TU, generator, vtable table, provider or
// A3/A4/A13 body is edited. The gate is byte [this+0x1AEC] bit0. The viewport
// fields are [+0x15AC..+0x15B8]; the three labels at +0x15CC/+0x170C/+0x184C and
// the PB_HEALTH progress bar at +0x198C all draw through [vtable+0x38]. The three
// action objects at +0x54+i*0x71C are drawn by the new 00685870 body.

#include <cstdint>

namespace {

using DrawFn = void (__fastcall*)(void*, void*, float);

inline void draw_child(void* object, float delta) {
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
extern "C" void __fastcall kotor_fastcall_00685870(void*, void*, float);

extern "C" void __fastcall kotor_fastcall_00685ed0(void* self, void*, float delta) {
    std::uint8_t* base = static_cast<std::uint8_t*>(self);

    if ((base[0x1AEC] & 1) == 0) {
        return;
    }

    kotor_AurGUIStartLayer_004591B0();

    const std::uint32_t left = *reinterpret_cast<const std::uint32_t*>(base + 0x15AC);
    const std::uint32_t top = *reinterpret_cast<const std::uint32_t*>(base + 0x15B0);
    const std::uint32_t right = *reinterpret_cast<const std::uint32_t*>(base + 0x15B4);
    const std::uint32_t bottom = *reinterpret_cast<const std::uint32_t*>(base + 0x15B8);

    if (kotor_AurGUISetupViewport_004592F0(
            left, top, right, bottom,
            scene3d_gui_scene_color_0078d3d8, 0, 0x3F800000u) == 0) {
        kotor_AurGUIStopLayer_004592B0();
        return;
    }

    draw_child(base + 0x184C, delta);
    draw_child(base + 0x170C, delta);
    draw_child(base + 0x15CC, delta);
    draw_child(base + 0x198C, delta);

    for (int i = 0; i < 3; ++i) {
        kotor_fastcall_00685870(base + 0x54 + i * 0x71C, nullptr, delta);
    }

    kotor_AurGUICloseViewport_00459580();
    kotor_AurGUIStopLayer_004592B0();
}
