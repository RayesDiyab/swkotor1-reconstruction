// A5 private reconstruction of original 00685870
// CSWGuiMainInterfaceAction::Draw — thiscall(float), RET 4.
//
// New TU only. No shared header, Draw TU, generator, vtable table, provider or
// A3/A4/A13 body is edited. The action object contains four 0x1C4-spaced base
// CSWGuiButton children at +0/+0x1C4/+0x388/+0x54C; each child's +0x44 flag bit1
// controls whether its own [vtable+0x38] Draw is called. The original order is
// +0, +0x388, +0x54C, +0x1C4.

#include <cstdint>

namespace {

using DrawFn = void (__fastcall*)(void*, void*, float);

inline void draw_child(void* object, float delta) {
    void** vtable = *reinterpret_cast<void***>(object);
    reinterpret_cast<DrawFn>(vtable[0x38 / 4])(object, nullptr, delta);
}

}  // namespace

extern "C" void __fastcall kotor_fastcall_00685870(void* self, void*, float delta) {
    std::uint8_t* base = static_cast<std::uint8_t*>(self);

    if (base[0x044] & 2) draw_child(self, delta);
    if (base[0x3CC] & 2) draw_child(base + 0x388, delta);
    if (base[0x590] & 2) draw_child(base + 0x54C, delta);
    if (base[0x208] & 2) draw_child(base + 0x1C4, delta);
}
