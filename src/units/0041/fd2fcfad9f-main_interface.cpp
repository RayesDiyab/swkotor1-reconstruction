// Lane W4 G15: CSWGuiMainInterface per-frame update/hit-test and the leaves they reach.
// Transcribed from dis/00686BA0.asm, 006858E0, 00686A20, 00686B70, 00685F90, 006345E0, 0062B110, 0062EEB0, 00610AC0.
// x87: values kept on the FPU stack are `double`; every `fst/fstp dword` is an explicit float store (PC53).
#include "e7524694c6-hud_providers.hpp"
using namespace w4;

extern "C" {
// Absent leaves not yet reconstructed: explicit traps at their ORIGINAL addresses (W4 HUD-owned).
void __fastcall kotor_fastcall_00687700(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_fastcall_00684df0(void*, void*, int, int);
void __fastcall kotor_fastcall_00624ab0(void*, void*, std::uint32_t);
void __fastcall kotor_fastcall_0062b580(void*, void*, std::uint32_t);
void __fastcall kotor_fastcall_0062b5a0(void*, void*, std::uint32_t);
void __fastcall kotor_fastcall_00624a70(void*, void*, std::uint32_t);
}

// 00686BA0 CSWGuiMainInterface::Update(float) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_00686ba0(void* self, void*, float delta) {
    for (I32 i = li(self, 0x5CB8) - 1; i >= 0; --i) {                                     // 00686BA5..00686BC9
        void* child = lp(lp(self, 0x5CB4), static_cast<U32>(i) * 4u);
        if (child != nullptr) vcall<void, float>(child, 0xA0, delta);                       // 00686BBB..00686BC2
    }
    if (lw(self, 0x7724) != 0xBF800000u) {                                                  // 00686BCB..00686BD8
        const double t = static_cast<double>(lf(self, 0x7724)) - delta;                    // 00686BDE..00686BE4
        delta = static_cast<float>(t);                                                       // 00686BE8 fst [esp+10h]
        sf(self, 0x7724, static_cast<float>(t));                                            // 00686BEC
        if (jcc_jp(t, 0.0, 0x41)) {                                                         // 00686BF2..00686BFD -> 00686C46



            const double v = static_cast<double>(delta) / lf(self, 0x7720);                 // 00686C4C..00686C50 (float t)

            const float vf = static_cast<float>(v);                                         // 00686C56 fst [esp+10h]
            if (jcc_jp(v, 0.5, 0x05)) sf(self, 0x746C, 1.0f);                               // 00686C5A..00686C65, 00686C6F
            else sf(self, 0x746C, static_cast<float>(static_cast<double>(vf) + vf));        // 00686C67..00686C6F
        } else {
            sw(self, 0x7724, 0xBF800000u);                                                  // 00686BFF
            void* app = client_app();                                                       // 00686C05..00686C0A (ebx)
            const U32 id = kotor_m15_client_get_player_creature_id_005ed550(app, nullptr);  // 00686C0D..00686C0F
            const I32 index = recovered_party_index_006349c0(kotor_fastcall_005ed8b0(app, nullptr), nullptr, id); // 00686C14..00686C1E
            if (index >= 0) {                                                               // 00686C25..00686C27
                U8* party = static_cast<U8*>(kotor_fastcall_005ed8b0(app, nullptr));        // 00686C29..00686C2B
                kotor_fastcall_006345e0(party + static_cast<U32>(index) * 0x88u + 0x24u, nullptr, 0xBC50u); // 00686C30..00686C3F
            }
        }
    }
    void* creature = client_get_player_creature_005ed540(client_app(), nullptr);           // 00686C75..00686C8C (edi)
    {
        void* table = lane_config_required::GetPartyTable_004aee70(server_app(), nullptr);  // 00686C83..00686C8E
        const U32 bit = li(table, 0) > 0 ? 1u : 0u;                                         // 00686C93..00686C9F
        const U32 old = lw(self, 0xC3A4);
        sw(self, 0xC3A4, old ^ (((bit << 1) ^ old) & 2u));                                  // 00686CA2..00686CAD
    }
    {
        const U32 v = lw(lane_config_required::GetPartyTable_004aee70(server_app(), nullptr), 0x190); // 00686CB3..00686CC1
        const U32 old = lw(self, 0xC528);
        sw(self, 0xC528, old ^ ((v ^ old) & 1u));                                           // 00686CC7..00686CD8
    }
    {
        const U32 stealth = creature ? static_cast<U32>(kotor_fastcall_00610ac0(creature, nullptr)) : 0u; // 00686CD6..00686CE9
        const U32 old = lw(self, 0xC658);
        sw(self, 0xC658, old ^ (((stealth + stealth) ^ old) & 2u));                        // 00686CEB..00686CFF
    }
    {
        const U32 v = creature ? lw(creature, 0x194) : 0u;                                  // 00686CFD..00686D0F
        const U32 old = lw(self, 0xC7DC);
        sw(self, 0xC7DC, lw(self, 0xC7DC) ^ ((old ^ v) & 1u));                             // 00686D11..00686D24
    }
    {
        const U32 paused = kotor_fastcall_005edc10(client_app(), nullptr);                  // 00686D2A..00686D32
        const U32 old = lw(self, 0xC274);
        sw(self, 0xC274, lw(self, 0xC274) ^ ((old ^ paused) & 1u));                        // 00686D37..00686D4A
    }
    void* options = kotor_fastcall_005ed700(client_app(), nullptr);                        // 00686D50..00686D59
    const U32 bit = options ? ((~(lw(options, 0x14) >> 9)) & 1u) : 1u;                     // 00686D5E..00686D6F
    static const U32 fields[9] = {0xB050, 0xB214, 0xB3D8, 0xB59C, 0xB760, 0xB924, 0xBAE8, 0xBCAC, 0xBE70};
    for (U32 off : fields) sw(self, off, (lw(self, off) & 0xFFFFFFFDu) | ((bit & 1u) * 2u)); // 00686D74..00686E0D
}

// 006858E0 CSWGuiMainInterfaceAction::Update(float) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_006858e0(void* self, void*, float delta) {
    U8 count = lb(self, 0x714);                                                             // 006858E0
    if (count == 0) return;                                                                 // 006858E6..006858E8
    const double t = static_cast<double>(lf(self, 0x710)) - delta;                         // 006858EA..006858F0
    sf(self, 0x710, static_cast<float>(t));                                                 // 006858F4
    if (!jcc_jp(t, 0.0, 0x41)) {                                                            // 006858FA..00685905
        sb(self, 0x714, static_cast<U8>(count - 1));                                        // 00685907..00685909
        sw(self, 0x710, 0x3DCCCCCDu);                                                       // 0068590F
    }
    const U8 now = lb(self, 0x714);                                                         // 00685919
    double v = 1.0;                                                                         // 0068591F
    if (now != 0) {                                                                         // 00685925..00685927
        v = (static_cast<double>(bits_f(0x3DCCCCCDu)) - lf(self, 0x710)) * 10.0f;           // 0068592D..00685939
        if (now == 2) v = 1.0 - v;                                                          // 00685929, 0068593F..00685941
    }
    sf(self, 0x250, static_cast<float>(v));                                                 // 00685947
    sf(self, 0x2C4, static_cast<float>(v));                                                 // 0068594D
}

// 0062EEB0 CGuiInGame::UpdateStatus(BYTE kind, int value) (thiscall, RET 8).
extern "C" void __fastcall kotor_fastcall_0062eeb0(void* in_game, void*, U8 kind, U32 value) {
    void* summary = lp(in_game, 0xA8);                                                      // 0062EEB0
    if (summary == nullptr) return;                                                         // 0062EEB6..0062EEB8
    const I32 pending = li(summary, 0x78);                                                  // 0062EEBA
    if (pending > 0) { sw(summary, 0x78, static_cast<U32>(pending - 1)); return; }          // 0062EEBD..0062EEC5
    switch (kind) {                                                                         // 0062EEC8..0062EED2 (table 0062EF3C)
    case 0: sw(summary, 0x64, lw(summary, 0x64) | 0x11u); return;                          // 0062EED9
    case 1: kotor_fastcall_00624ab0(summary, nullptr, value); return;                      // 0062EEE0
    case 2: kotor_fastcall_0062b580(summary, nullptr, value); return;                      // 0062EEED
    case 3: kotor_fastcall_0062b5a0(summary, nullptr, value); return;                      // 0062EEFA
    case 4: case 5: kotor_fastcall_00624a70(summary, nullptr, value); return;              // 0062EF07
    case 7: sw(summary, 0x64, lw(summary, 0x64) | 0x05u); return;                          // 0062EF14
    case 8: sw(summary, 0x64, lw(summary, 0x64) | 0x09u); return;                          // 0062EF1B
    case 9: sw(summary, 0x64, lw(summary, 0x64) | 0x22u); return;                          // 0062EF22
    case 10: sw(summary, 0x64, lw(summary, 0x64) | 0x42u); return;                         // 0062EF29
    case 11: sw(summary, 0x64, lw(summary, 0x64) | 0x82u); return;                         // 0062EF30
    default: return;                                                                        // 6 and >11: 0062EF37
    }
}

// 00686A20 CSWGuiMainInterfaceStatus::Update(float) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_00686a20(void* self, void*, float delta) {
    if ((lb(self, 0x44) & 2) == 0) return;                                                  // 00686A23..00686A27
    if ((lb(self, 0x144) & 1) != 0 && !jcc_jne(delta, 0.0, 0x41)) {                        // 00686A29..00686A41
        if (lb(self, 0x148) != 0)                                                           // 00686A43..00686A4B
            kotor_fastcall_0062eeb0(pt(frame_R4_005ed690(client_app(), nullptr)), nullptr, 0x0A, 0); // 00686A4D..00686A60
        sw(self, 0x144, lw(self, 0x144) & 0xFFFFFFFEu);                                    // 00686A65
    }
    const double t = static_cast<double>(lf(self, 0x140)) - delta;                         // 00686A6C..00686A72
    sf(self, 0x140, static_cast<float>(t));                                                 // 00686A76
    if (jcc_jp(t, 0.0, 0x41)) return;                                                       // 00686A7C..00686A87
    sw(self, 0x44, lw(self, 0x44) & 0xFFFFFFFDu);                                           // 00686A89
}

// 00685F90 CSWGuiTargetActionMenu::HitCheckMouse(int x, int y) (thiscall, RET 8).
extern "C" U32 __fastcall kotor_fastcall_00685f90(void* self, void*, int x, int y) {
    U8* const s = static_cast<U8*>(self);
    U32 hit = 0;                                                                            // ebp
    if (lb(self, 0x1AEC) & 1) {                                                             // 00685F93..00685F9E
        y -= li(self, 0x15B0);                                                              // 00685FA4..00685FBA
        x -= li(self, 0x15AC);                                                              // 00685FAA..00685FC2
        if (vcall<U32, int, int>(s + 0x15CC, 0x44, x, y) != 0) {                            // 00685FBC..00685FD9
            hit = ad(s + 0x15CC);                                                           // 00685FDB
        } else if (vcall<U32, int, int>(s + 0x198C, 0x44, x, y) != 0) {                     // 00685FE3..00685FF6
            hit = ad(s + 0x198C);                                                           // 00685FF8
        } else {
            bool any = false;                                                               // 00686000..0068601C
            for (U32 i = 0; i < 3 && !any; ++i) any = lw(self, 4 + i * 0xC) != 0;
            if (any) {
                for (U32 i = 0; i < 3 && hit == 0; ++i)                                     // 00686022..00686048
                    hit = kotor_fastcall_00684df0(s + 0x54 + i * 0x71C, nullptr, x, y);
            }
        }
    }
    const U32 flags = lw(self, 0x1AEC);                                                     // 0068604C
    if ((flags & 2) && hit == 0) sw(self, 0x1AE0, 0x3F000000u);                             // 00686052..0068605A
    sw(self, 0x1AEC, flags ^ ((((hit != 0) ? 1u : 0u) << 1 ^ flags) & 2u));                 // 00686064..00686077
    return hit;                                                                             // 00686074
}

// 00686B70 CSWGuiMainInterface::HitCheckMouse(int x, int y) (thiscall, RET 8).
extern "C" U32 __fastcall kotor_fastcall_00686b70(void* self, void*, int x, int y) {
    const U32 hit = kotor_fastcall_00685f90(static_cast<U8*>(self) + 0xBC, nullptr, x, y);  // 00686B7D..00686B85
    if (hit != 0) return hit;                                                               // 00686B8A..00686B8C
    return gui_load_panel_hit_check_0040b690(self, nullptr, x, y);                          // 00686B8E..00686B92
}

// 0062B110 CGuiInGame::SetCombatMessage(msg) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_0062b110(void* in_game, void*, U32 message) {
    void* main_interface = lp(in_game, 0x90);                                               // 0062B110
    if (main_interface != nullptr) kotor_fastcall_00687700(main_interface, nullptr, message); // 0062B116..0062B11A
}

// 006345E0 CSWPartyMemberData::SetCombatMessage(msg) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_006345e0(void* member, void*, U32 message) {
    sw(member, 0x84, message);                                                              // 006345E7
    if (lw(member, 0) != kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr)) return; // 006345ED..006345FD
    kotor_fastcall_0062b110(pt(frame_R4_005ed690(client_app(), nullptr)), nullptr, lw(member, 0x84)); // 006345FF..00634616
}

// 00610AC0 CSWCCreature::IsStealthCapable() (thiscall, RET 0).
extern "C" int __fastcall kotor_fastcall_00610ac0(void* creature, void*) {
    if (kotor_m15a_object_get_server_object_0063d4b0(creature, nullptr) == nullptr) return 0;           // 00610AC3..00610ACA
    if (vcall<void*>(kotor_m15a_object_get_server_object_0063d4b0(creature, nullptr), 0x30) == nullptr) return 0; // 00610AD0..00610AE0
    void* server = kotor_m15a_object_get_server_object_0063d4b0(creature, nullptr) != nullptr           // 00610AE6..00610AEF
        ? vcall<void*>(kotor_m15a_object_get_server_object_0063d4b0(creature, nullptr), 0x30) : nullptr; // 00610AF1..00610B01
    void* area = recovered_object_area_004cb120(server, nullptr);                         // 00610B03..00610B05
    if (lw(area, 0x2B0) != 0) return 0;                                                    // 00610B0A..00610B12
    if (kotor_m15b_creature_stats_get_can_use_skill_006477e0(lp(creature, 0x2F8), nullptr, 2) == 0) return 0; // 00610B18..00610B27
    U32 stealth = 0;                                                                        // ebp
    if (void* item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, lw(creature, 0x248))) { // 00610B2D..00610B4A
        if (kotor_m15b_item_get_base_item_005b4790(item, nullptr) != nullptr)              // 00610B4C..00610B55
            stealth = lb(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0xAC) == 0x2C ? 1u : 0u; // 00610B57..00610B6C
    }
    if (void* item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, lw(creature, 0x258))) { // 00610B6E..00610B86
        if (kotor_m15b_item_get_base_item_005b4790(item, nullptr) != nullptr)              // 00610B88..00610B91
            stealth |= lb(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0xAC) == 0x2C ? 1u : 0u; // 00610B93..00610BA8
    }
    return stealth != 0 ? 1 : 0;                                                            // 00610BAB..00610BBA
}
