// Owner-authorized extraction of two existing bodies; exact body text retained.
#include "1f3fbe79c1-w4_providers.hpp"
extern "C" {
void __fastcall kotor_fastcall_004af030(void*,void*,w4::U32);
void __fastcall kotor_fastcall_0040bc70(void*,void*,void*,std::uint32_t,std::uint32_t);
void __fastcall kotor_m15_panel_set_visible_0040b9d0(void*,void*,unsigned);
void __fastcall kotor_m15_fade_set_fade_006244e0(void*,void*,unsigned,float,float,const void*);
}
using namespace w4;
namespace { void* in_game() { return pt(frame_R4_005ed690(client_app(), nullptr)); } }

extern "C" void __fastcall kotor_fastcall_0062ec60(void* self, void*, U32 state) {
    sw(self, 0xB4, state);                                                                   // 0062EC6C
    if (state != 0) return;                                                                  // 0062EC72
    sw(self, 0xC0, 0);                                                                       // 0062EC78
    kotor_fastcall_004af030(server_app(), nullptr, 0);                                       // 0062EC7E..0062EC87
    void* gui = in_game();                                                                   // 0062EC8C..0062EC95
    if (lw(gui, 0xD8) > 0) sw(gui, 0xD8, lw(gui, 0xD8) - 1);                                 // 0062EC9A..0062ECA5
    if (lw(self, 0xB98) != 0) {                                                              // 0062ECAB..0062ECB3
        sw(self, 0xB98, 0);                                                                  // 0062ECBA
        U32 black[3] = {0, 0, 0};                                                            // 0062ECC4..0062ECD4
        if (lw(self, 0x6C) != 0) {                                                           // 0062ECB5, 0062ECDC
            kotor_fastcall_0040bc70(lp(self, 0x38), nullptr, lp(self, 0x6C), 4, 1);          // 0062ECDE..0062ECE6
            kotor_m15_panel_set_visible_0040b9d0(lp(self, 0x6C), nullptr, 1);                 // 0062ECEB..0062ECF0
            kotor_m15_fade_set_fade_006244e0(lp(self, 0x6C), nullptr, 1, 0.0f, 1.0f, black); // 0062ECF5..0062ED06
        }
    }
    kotor_fastcall_0061fa00(lp(client_app(), 4), nullptr, 0x10);                             // 0062ED0B..0062ED18 ShowMouse
}

extern "C" void __fastcall kotor_fastcall_004af030(void* server, void*, std::uint32_t ignore) {
    void* internal; std::memcpy(&internal, static_cast<unsigned char*>(server) + 4, 4);       // 004AF030
    std::memcpy(static_cast<unsigned char*>(internal) + 0x1B914, &ignore, 4);                // 004AF033..004AF037
}
