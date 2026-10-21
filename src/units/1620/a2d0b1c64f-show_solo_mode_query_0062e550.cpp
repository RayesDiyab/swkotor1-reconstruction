// 0062E550 — CGuiInGame::ShowSoloModeQuery (owned export).
// Original 378-byte body. Three early null returns happen before EBP/ESI/EDI are used.
// +0xB4 is read from the 005ED690 GetInGameGui result, not from self.
#include "78e79b69b7-party_selection_decls.h"

namespace kotor_input_required {

void __fastcall show_solo_mode_query_0062e550(void* self, void* /*unused_edx*/,
                                               U32 text_id) {
    using namespace party_selection;

    if (at<U32>(self, 0x108) == 0) return;             // 0062E55B -> 0062E6C6
    if (at<void*>(self, 0x38) == nullptr) return;      // 0062E566
    if (at<void*>(self, 0x8C) == nullptr) return;      // 0062E574

    void* const party = kotor_fastcall_005ed8b0(client_app(), nullptr);  // 0062E585
    int multi = 0;
    if (party != nullptr) {                            // 0062E58E
        // 0062E594 cmp edx,1 / 0062E597 setg: signed greater-than, not unsigned.
        const std::int32_t party_count =
            static_cast<std::int32_t>(at<U32>(party, 0));
        multi = (party_count > 1) ? 1 : 0;
    }

    void* const pc = get_player_creature_005ed540(client_app(), nullptr);  // 0062E5A5
    void* server_creature = nullptr;
    if (pc != nullptr) {                               // 0062E5AE
        server_creature = kotor_m15a_creature_get_server_creature_0060fb20(
            pc, nullptr);                                  // 0062E5B2
    }

    void* server_area = nullptr;
    if (kotor_fastcall_005ed530(client_app(), nullptr) != nullptr) {  // 0062E5C3
        // Original calls GetModule a second time at 0062E5D5 and dereferences
        // the second result at +0x48 without a null check.
        void* const module = kotor_fastcall_005ed530(client_app(), nullptr);
        void* const area = at<void*>(module, 0x48);     // 0062E5DA
        if (area != nullptr) {                         // 0062E5DF
            server_area = frame_GetServerArea_00605640(area, nullptr);  // 0062E5E1
        }
    }

    void* const gui = get_in_game_gui_005ed690(client_app(), nullptr);  // 0062E5F1
    if (at<U32>(gui, 0xB4) != 0) goto hide;            // 0062E5FE (GetInGameGui result)
    if (server_area != nullptr && at<U32>(server_area, 0x2C4) != 0) goto hide;  // 0062E610
    if (kotor_m15_client_get_area_not_ready_005edad0(client_app(), nullptr) != 0) {
        goto hide;                                     // 0062E625
    }

    if (server_creature != nullptr) {                  // 0062E62D
        if (vcall<U32>(server_creature, 0x94) != 0) goto hide;  // 0062E63B
        if (recovered_creature_incapacitated_004ef890(server_creature, nullptr) != 0) goto hide;  // 0062E646
    }

    if (multi == 0) goto hide;                         // 0062E64A

    if (at<void*>(self, 0xB38) == nullptr) {           // 0062E654
        void* const message = kotor_fastcall_005ed6f0(client_app(), nullptr);  // 0062E65E
        (void)frame_TogglePauseRequest_00677800(message, nullptr);             // 0062E665
    }

    kotor_nested_006c24a0(at<void*>(self, 0x8C), nullptr, text_id);  // 0062E675

    // Original re-reads [self+0x8C]/[self+0x38] for ContainsPanel (0062E67A/680)
    // and again for AddPanel (0062E68D/693).
    if (frame_ContainsPanel_0040bd70(
            at<void*>(self, 0x38), nullptr, at<void*>(self, 0x8C)) == 0) {  // 0062E684
        kotor_fastcall_0040bc70(
            at<void*>(self, 0x38), nullptr, at<void*>(self, 0x8C), 1, 1);   // 0062E69B
    }
    gui_m48_client_set_input_class_005eda60(client_app(), nullptr, 2, 1);  // 0062E6AD
    return;

hide:
    kotor_fastcall_0040a140(at<void*>(self, 0x38), nullptr, 2);        // 0062E6BE
}

}  // namespace kotor_input_required
