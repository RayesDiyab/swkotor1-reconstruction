// 0060F4B0 — CSWCCreature::PartyMenuStealth (owned export).
// Original 91-byte body, two RET 8 exits. Both stack arguments are ignored (original quirk).
// [party_table+0x190] is from GetPartyTable; the party count is [GetSWParty result+0].
#include "78e79b69b7-party_selection_decls.h"

namespace kotor_input_required {

void __fastcall party_menu_stealth_0060f4b0(void* self, void* /*unused_edx*/,
                                             U32 /*unused_a*/, U32 /*unused_b*/) {
    using namespace party_selection;

    void* const table = lane_config_required::GetPartyTable_004aee70(server_app(), nullptr);
    if (at<U32>(table, 0x190) != 0) {                  // 0060F4C8
        goto perform_skill;
    }

    {
        void* const party = kotor_fastcall_005ed8b0(client_app(), nullptr);
        if (at<U32>(party, 0) == 1) {                  // 0060F4DB
            goto perform_skill;
        }
    }

    {
        void* const gui = get_in_game_gui_005ed690(client_app(), nullptr);
        show_solo_mode_query_0062e550(gui, nullptr, 1);  // 0060F4EF (text id 1)
    }
    return;

perform_skill:
    kotor_fastcall_0060ee00(self, nullptr, 2, 0, at<void*>(self, 4));  // 0060F502
}

}  // namespace kotor_input_required
