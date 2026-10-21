// Action-input provider completion: five exact original-address definitions.
// Source-only; no compile/link/native evidence is claimed by this file.
// Owner addresses: 00688790, 0068AD20, 0066DC50, 006865B0, 00688820.
#include "c80e970c1c-action_input_declarations.hpp"

namespace kotor_input_required {

namespace aid = action_input_detail;

// 00688790 [00688790,006887C4): CSWGuiMainInterface::ClearOneAction.
// If the action-queue flag is clear, return. Otherwise call the selected
// show_tutorial_window provider; on a zero result tail-call the genuine
// 006880C0 provider with ECX=self. No null guards are added.
void __fastcall clear_one_action_00688790(void* self, void*) {
    if ((aid::ld8(aid::addr(self, 0x6D14)) & 0x02) == 0) return;
    kotor_input_hover::AppManager* mgr =
        static_cast<kotor_input_hover::AppManager*>(kotor_menu_window_cells::manager_007a39fc);
    void* client = mgr->client;
    const int shown = ::show_tutorial_window_005edf40(
        client, nullptr, 0x20u, 0x7F000000u, 0x7F000000u, 0u);
    if (shown != 0) return;
    ::kotor_fastcall_006880c0(self, nullptr);
}

// 0068AD20 [0068AD20,0068AD51): CSWGuiMainInterface::DoTargetAction.
// The second stack argument is dead; the original always pushes the constant 1.
// The embedded target-action menu is formed by lea self+0xBC.
void __fastcall do_target_action_0068ad20(void* self, void*, U32 slot, U32) {
    if (static_cast<aid::I32>(slot) < 0 || static_cast<aid::I32>(slot) >= 3) return;
    if (kotor_input_cells_required::word_008338f8 != 0)
        ::kotor_fastcall_006887d0(self, nullptr);
    void* target_menu = aid::addr(self, 0xBC);
    ::kotor_fastcall_00689610(target_menu, nullptr, slot, 1u);
}

// 0066DC50 [0066DC50,0066DC73): per-element fire invoker.
// The count is cached in EBX, the array pointer is reloaded from [self+0x74]
// on every iteration, and 00674090 is called with ECX=element. No null guards.
void __fastcall fire_0066dc50(void* self, void*) {
    const aid::I32 count = aid::ld32s(aid::addr(self, 0x78));
    for (aid::I32 i = 0; i < count; ++i) {
        void** array = reinterpret_cast<void**>(
            static_cast<std::uintptr_t>(aid::ld32(aid::addr(self, 0x74))));
        void* element = array[i];
        ::kotor_fastcall_00674090(element, nullptr);
    }
}

// 006865B0 [006865B0,00686676): CSWGuiTargetActionMenu::SelectNextAction.
// Slot 0..2; three 12-byte vector headers at +0x00/+0x0C/+0x18; entries stride
// 0x38 with the compared id at +8; selected-id cell is 4*(slot+3*(category+3)).
// count<=1 is a no-op. The incoming slot stack slot is reused as scratch in the
// original; the C++ local next/base achieves the same observable behavior.
void __fastcall select_next_action_006865b0(void* self, void*, U32 slot) {
    if (static_cast<aid::I32>(slot) < 0 || static_cast<aid::I32>(slot) >= 3) return;
    aid::U8* row = aid::addr(self, 12u * slot);
    const aid::I32 count = aid::ld32s(aid::addr(row, 4u));
    if (count == 0) return;

    // Original exercises the party/player chain; the result is discarded.
    (void)::kotor_fastcall_00635460(::kotor_fastcall_005ed8b0(aid::app(), nullptr), nullptr);

    const std::int8_t category = aid::ld8s(aid::addr(self, 0x1AEA));
    const aid::U32 table_index =
        slot + static_cast<aid::U32>(3 * (static_cast<aid::I32>(category) + 3));
    aid::U8* cell = aid::addr(self, 4u * table_index);
    const aid::U32 selected = aid::ld32(cell);

    aid::U8* next = nullptr;
    if (selected != 0xFFFFFFFFu) {
        const aid::I32 loop_count = aid::ld32s(aid::addr(row, 4u));
        if (loop_count > 0) {
            aid::U8* base = reinterpret_cast<aid::U8*>(
                static_cast<std::uintptr_t>(aid::ld32(row)));
            for (aid::I32 i = 0; i < loop_count; ++i) {
                aid::U8* entry_id = aid::addr(
                    base, static_cast<aid::U32>(i) * 0x38u + 8u);
                if (aid::ld32(entry_id) == selected) {
                    if (i + 1 < loop_count)
                        next = aid::addr(base, static_cast<aid::U32>(i + 1) * 0x38u);
                    else
                        next = base;
                    break;
                }
            }
        }
    }

    const aid::I32 count_now = aid::ld32s(aid::addr(row, 4u));
    if (count_now <= 1) return;
    if (next == nullptr) {
        aid::U8* base = reinterpret_cast<aid::U8*>(
            static_cast<std::uintptr_t>(aid::ld32(row)));
        next = aid::addr(base, 0x38u);
    }
    const aid::U32 next_id = aid::ld32(aid::addr(next, 8u));
    aid::st32(cell, next_id);

    void* mgr = ::kotor_fastcall_005eda40(aid::app(), nullptr);
    ::kotor_fastcall_0040a140(mgr, nullptr, 7u);
}

// 00688820 [00688820,006888DF): CSWGuiMainInterface::SelectNextAction.
// Slot 0..3; six 12-byte vectors at +0x74; selected ids at +0x1BAC; count at
// +0x78+12*slot; entries stride 0x38 with the compared id at +8. count<=1 is a
// no-op. The incoming slot stack slot is reused as scratch in the original.
void __fastcall select_next_action_00688820(void* self, void*, U32 slot) {
    if (static_cast<aid::I32>(slot) < 0 || static_cast<aid::I32>(slot) >= 4) return;
    aid::U8* row = aid::addr(self, 0x78u + 12u * slot);
    const aid::I32 count = aid::ld32s(row);
    if (count == 0) return;

    // Original exercises the party/player chain; the result is discarded.
    (void)::kotor_fastcall_00635460(::kotor_fastcall_005ed8b0(aid::app(), nullptr), nullptr);

    const aid::U32 selected = aid::ld32(aid::addr(self, 0x1BACu + 4u * slot));
    aid::U8* next = nullptr;
    if (selected != 0xFFFFFFFFu) {
        const aid::I32 loop_count = aid::ld32s(row);
        if (loop_count > 0) {
            aid::U8* base = reinterpret_cast<aid::U8*>(
                static_cast<std::uintptr_t>(
                    aid::ld32(aid::addr(self, 0x74u + 12u * slot))));
            for (aid::I32 i = 0; i < loop_count; ++i) {
                aid::U8* entry_id = aid::addr(
                    base, static_cast<aid::U32>(i) * 0x38u + 8u);
                if (aid::ld32(entry_id) == selected) {
                    if (i + 1 < loop_count)
                        next = aid::addr(base, static_cast<aid::U32>(i + 1) * 0x38u);
                    else
                        next = base;
                    break;
                }
            }
        }
    }

    const aid::I32 count_now = aid::ld32s(row);
    if (count_now <= 1) return;
    if (next == nullptr) {
        aid::U8* base = reinterpret_cast<aid::U8*>(
            static_cast<std::uintptr_t>(
                aid::ld32(aid::addr(self, 0x74u + 12u * slot))));
        next = aid::addr(base, 0x38u);
    }
    const aid::U32 next_id = aid::ld32(aid::addr(next, 8u));
    aid::st32(aid::addr(self, 0x1BACu + 4u * slot), next_id);

    void* mgr = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(aid::ld32(aid::addr(self, 0x18u))));
    ::kotor_fastcall_0040a140(mgr, nullptr, 7u);
}

} // namespace kotor_input_required
