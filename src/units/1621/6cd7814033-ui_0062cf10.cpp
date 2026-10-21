// 0062CF10 — CGuiInGame::SwitchToSWInGameGui (303 bytes, 0x0062CF10..0x0062D03F).
//
// Original-byte reference: EVIDENCE/function-0062CF10.disasm.txt.
// Exact accepted 0769 import:
//   ?switch_to_sw_in_game_gui_0062cf10@kotor_input_required@@YIXPAX0I@Z
//
// ABI: __fastcall(ECX=CGuiInGame*, EDX unused, 1 stack U32 index), RET 4, void.
// EBP is untouched; EBX/ESI/EDI are compiler-saved.  The two original epilogue
// entries differ only in whether EDI is popped; source-level returns are equivalent.
// Quirks preserved: manager is not null-guarded where the original dereferences it;
// self+0x2C is stored unconditionally immediately after UpdateCreatedInGameGUI;
// AddPanel's source order is (panel, 2, 1).
#include "1f393e8651-ui_save_state_types.h"

using kotor_reconstruction::StartupString;

namespace kotor_input_required {

void __fastcall switch_to_sw_in_game_gui_0062cf10(
    void* self, void*, std::uint32_t index_raw) {
    std::int32_t const index = static_cast<std::int32_t>(index_raw);

    // Early exits: signed jl, signed jge, equality with the current index, and the
    // required non-null self+0x108 gate.
    if (index < 0) {
        return;
    }
    if (index >= 8) {
        return;
    }
    if (index_raw == ui_load_at<std::uint32_t>(self, 0x2C)) {
        return;
    }
    if (ui_load_at<std::uint32_t>(self, 0x108) == 0u) {
        return;
    }

    void* const app = kotor_menu_window_cells::manager_007a39fc;
    void* const internal = ui_load_at<void*>(app, 4);
    void* const manager = kotor_fastcall_005eda40(internal, nullptr);

    std::uint32_t const old_index = ui_load_at<std::uint32_t>(self, 0x2C);

    if (w4::ingame_gui_single_panel_007a2370 != 0u &&
        ui_load_at<std::uint32_t>(self, 0x30) != 0u && manager != nullptr) {
        void* const panel =
            ui_load_at<void*>(self, 0x0C + static_cast<std::size_t>(old_index) * 4u);
        frame_RemovePanel_0040c830(manager, nullptr, panel);
    }

    std::uint32_t new_index;
    if (ui_load_at<std::uint32_t>(self, 0x30) != 0u && manager != nullptr) {
        new_index = index_raw;
    } else {
        new_index = 0xFFFFFFFFu;
    }

    // The original reloads [self+0x2C] at 0x62CF9E for the Update call; the earlier
    // cached value is only the RemovePanel index.
    load_screen_update_created_in_game_gui_0062b1e0(
        self, nullptr, ui_load_at<std::uint32_t>(self, 0x2C), new_index);

    // Unconditional store immediately after the call (0062CFAE).
    ui_store_at<std::uint32_t>(self, 0x2C, index_raw);

    if (ui_load_at<std::uint32_t>(self, 0x30) != 0u && manager != nullptr) {
        void* const panel =
            ui_load_at<void*>(self, 0x0C + static_cast<std::size_t>(index_raw) * 4u);
        kotor_fastcall_0040bc70(manager, nullptr, panel, 2u, 1u);

        void* const back = ui_load_at<void*>(
            self, 0x0C + static_cast<std::size_t>(
                              ui_load_at<std::uint32_t>(self, 0x2C)) * 4u);
        kotor_fastcall_0040bd20(manager, nullptr, back);

        void* const menu = ui_load_at<void*>(self, 0x08);
        kotor_fastcall_00624bd0(
            menu, nullptr, ui_load_at<std::uint32_t>(self, 0x2C), 1u);

        // Only reached when the panel block is taken: 0x62CFB1/0x62CFB5 jump to
        // 0x62D02B (skipping this VM block) when self+0x30==0 or manager==0.
        // The original then gates on the zero BSS outer-VM pointer.
        void* const vm = reinterpret_cast<void*>(
            static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00));
        if (vm != nullptr) {
            StartupString script("k_pend_screenchg");
            // The original reloads the cell after constructing the temporary.
            void* const vm_now = reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00));
            // 005D0FC0 JOIN GATE (not closure): this reference is the selected 36-byte
            // @frame_R4_005d0fc0@20 frame wrapper at v5 0073B840, not the genuine
            // 16-byte thunk. The genuine thunk is load-screen-deps-owned; expected
            // original-address export @kotor_fastcall_005d0fc0@20 (confirm with the
            // load-screen lane). This lane authors no new body/alias.
            frame_R4_005d0fc0(vm_now, nullptr, &script, 0x7F000000u, 0u);
        }
    }
}

}  // namespace kotor_input_required
