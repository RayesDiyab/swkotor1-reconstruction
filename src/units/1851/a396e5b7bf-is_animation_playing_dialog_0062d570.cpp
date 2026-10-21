// Original 0062D570 CGuiInGame::IsAnimationPlayingInDialog.
// thiscall(gui), no stack args, RET 0. Loops over gui+0x198 count / gui+0x1A0
// ids, resolving each object and testing its creature animation id.
#include "e93cd573fa-dialog_camera_client_contracts.hpp"

extern "C" int __fastcall kotor_fastcall_0062d570(void* self, void*) {
    using namespace dlgcam;

    if (lw(self, 0x198) == 0) {
        return 0;
    }

    for (U32 i = 0;;) {
        // Original read order: [0x7A39FC], gui+0x1A0, [app+4], array[i].
        void* const app = kotor_menu_window_cells::manager_007a39fc;
        void* const ids = lp(self, 0x1A0);
        void* const client = lp(app, 4);
        const U32 id = lw(ids, i * 4);

        void* const object = kotor_fastcall_005ed580(client, nullptr, id);
        void* const creature = vcall<void*>(object, 0x2C);
        if (creature != nullptr) {
            const U16 word = lh(creature, 0x48);
            if (kotor_fastcall_0063c4a0(creature, nullptr, word) != 0) {
                return 1;
            }
        }

        const U32 next_count = lw(self, 0x198);
        ++i;
        if (i >= next_count) {
            return 0;
        }
    }
}
