#include "00cddcd8db-menu_draw.h"
#include <cstring>
#include <intrin.h>

namespace {
using Word = std::uint32_t;
static_assert(sizeof(void*) == 4 && sizeof(Word) == 4, "x86 carrier required");

// memcpy accesses object representations without introducing a competing class
// view. Compiler barriers retain the original load/store order, including the
// extent output's permitted aliasing with receiver fields. Ordinary aligned,
// nonconcurrent game objects are the domain; MMIO is not a claimed domain.
template<class T> T load(const void* object, unsigned offset) {
    T value;
    _ReadWriteBarrier();
    std::memcpy(&value, static_cast<const unsigned char*>(object) + offset, sizeof value);
    _ReadWriteBarrier();
    return value;
}
void store(void* object, unsigned offset, Word value) {
    _ReadWriteBarrier();
    std::memcpy(static_cast<unsigned char*>(object) + offset, &value, sizeof value);
    _ReadWriteBarrier();
}
Word signed_word(std::int16_t value) {
    return static_cast<Word>(static_cast<std::int32_t>(value));
}
bool positive(Word value) { return value != 0 && (value & 0x80000000u) == 0; }
bool greater_signed(Word a, Word b) { return (a ^ 0x80000000u) > (b ^ 0x80000000u); }
Word half_signed_toward_zero(Word value) {
    const Word biased = value + (value >> 31);
    return (biased >> 1) | (biased & 0x80000000u);
}
Word dimension(void* panel, unsigned offset) {
    return signed_word(load<std::int16_t>(load<void*>(panel, 0x18), offset));
}
// lane-gui-render revision (decision D1): original thiscall slots are viewed as
// the ABI-identical __fastcall(Self*, void* unused_edx, DWORD) form, RET 4.
using DrawSlot = void (__fastcall*)(void*, void*, Word);
void invoke_draw(void* object, unsigned slot, Word delta_bits) {
    void* table = load<void*>(object, 0);
    load<DrawSlot>(table, slot)(object, nullptr, delta_bits);
}
}

// 0040AA00 CSWGuiPanel::GetExtentAccountingForPanelOffset: owned by lane-gui-load (D62,
// @gui_load_panel_offset_extent_0040aa00@12); the name used below is joined to it in
// gui_layer_joins.cpp. Body withdrawn here (rev2).

extern "C" void __fastcall kotor_panel_draw_0040B760(
    void* receiver, void* ignored_edx, Word delta_bits) {
    (void)ignored_edx;
    if ((load<std::uint8_t>(receiver, 0x44) & 0x80u) == 0) return;
    if ((load<Word>(receiver, 4) & 0x80000000u) != 0) return;
    if ((load<Word>(receiver, 8) & 0x80000000u) != 0) return;
    if (!positive(load<Word>(receiver, 0x10))) return;
    if (!positive(load<Word>(receiver, 0x0c))) return;
    void* manager = load<void*>(receiver, 0x18);
    const Word manager_width = signed_word(load<std::int16_t>(manager, 0x6c));
    const Word width = load<Word>(receiver, 0x0c);
    const Word left = load<Word>(receiver, 4);
    if (greater_signed(width + left, manager_width)) return;
    const Word top = load<Word>(receiver, 8);
    const Word height = load<Word>(receiver, 0x10);
    if (greater_signed(height + top, signed_word(load<std::int16_t>(manager, 0x6e)))) return;
    Word extent[4] = {left, load<Word>(receiver, 8), load<Word>(receiver, 0x0c), load<Word>(receiver, 0x10)};
    const Word count = load<Word>(receiver, 0x24); // cached before every callback
    kotor_panel_extent_0040AA00(receiver, nullptr, extent);
    kotor_AurGUIStartLayer_004591B0();
    if (kotor_AurGUISetupViewport_004592F0(extent[0], extent[1], extent[2], extent[3],
        static_cast<unsigned char*>(receiver) + 0x50, 0, load<Word>(receiver, 0x4c)) != 0) {
        invoke_draw(load<void*>(receiver, 0x5c), 0x0c, delta_bits);
        if (positive(count)) {
            for (Word index = 0; index < count; ++index) {
                void* array = load<void*>(receiver, 0x20); // reload after callbacks
                void* control = load<void*>(array, index * 4);
                if (control && (load<std::uint8_t>(control, 0x44) & 2u) != 0)
                    invoke_draw(control, 0x38, delta_bits);
            }
        }
        kotor_AurGUICloseViewport_00459580();
    }
    kotor_AurGUIStopLayer_004592B0();
}

// 0067AC10 CSWGuiMainMenu::Draw: owned by lane-gui-load (gui_load_mainmenu_draw_0067ac10, mainmenu.cpp);
// withdrawn from this copy (lane-gui-render revision 2) to keep one body per address.
