// Lane W4 G15: CSWGuiInGameEquip (006BA980), built unconditionally by CGuiInGame::Initialize (00632860), with the
// helpers its constructor runs: ShowDescription (006B7680) -> ShowCantEquipMessage (006B59F0) / UpdatePortraits
// (006B5B60). Transcribed from dis/c_006BA980.asm, dis/006B5B60.asm and the odis listings cited per line.
#include "78d2073e39-gui_panels.hpp"
#include "60d326f6b8-claude_handlers.hpp"
using namespace w4gui;

namespace kotor_reconstruction { struct StartupString; }
using SS = kotor_reconstruction::StartupString;
SS* __fastcall GuiConcat005e5d10(SS* self, void*, SS* out, const SS* rhs);                       // CExoString::operator+
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); }   // CServerExoApp::GetPartyTable
extern "C" {
extern const unsigned char borrowed_exit_color_007a23b4[12];                                     // [007A23B4] input-cells
extern const unsigned char kotor_m1_color_007a2408[12];                                          // [007A2408] m1-saveload
void __fastcall gui_load_border_params_set_pulsing_00414d00(void*, void*, U32, U32, U32);
void __fastcall gui_load_border_params_set_fill_00414c00(void* params, void*, const void* ref, I32) noexcept(false);
void __fastcall gui_load_listbox_clear_items_00419a90(void* listbox, void*);
void* __fastcall kotor_fastcall_0041a5a0(void* listbox, void*, I32 index);                       // CSWGuiListBox::GetControl
void* __fastcall kotor_fastcall_005edeb0(void* app, void*, void* out, U32 id);                   // CClientExoApp::GetGUIString
void* __fastcall client_get_player_creature_005ed540(void* app, void*);                          // CClientExoApp::GetPlayerCreature
U32 __fastcall kotor_fastcall_005eda50(void* app, void*, U32 id);                                // ServerToClientObjectId
void* __fastcall kotor_fastcall_005ed8b0(void* app, void*);                                     // CClientExoApp::GetSWParty
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, U32 index);                     // CSWParty::GetCharacter
// server leaves (src/gui/pending_leaves.cpp: link only if no owner provides them)
void* __fastcall recovered_server_info_004aec90(void* server, void*);                                   // CServerExoApp::GetServerInfo
U32 __fastcall kotor_fastcall_005636b0(void* table, void*, I32 index);                           // CSWPartyTable::GetIsNPCAvailable
U8 __fastcall kotor_fastcall_005636d0(void* table, void*);                                       // CSWPartyTable::GetNumNPCAvailable
U32 __fastcall recovered_npc_object_00564700(void* table, void*, I32 index, U32, U32);                // CSWPartyTable::GetNPCObject
}

namespace {
void* party_table() { return lane_config_required::GetPartyTable_004aee70(server_app(), nullptr); }
// creature vtable +148h GetPortrait(CResRef* out, int) -> CResRef*; the result feeds SetFillImage(ref, keep).
void portrait_fill(void* self, U32 params, void* creature, I32 keep) {
    unsigned char ref[16];
    void* r = vcall<void*, void*, U32>(creature, 0x148, ref, 1);
    gui_load_border_params_set_fill_00414c00(at(self, params), nullptr, r, keep);
}
// CSWGuiInGameEquip +1B8C entries (owned, deleted): 006B8888..006B88EC.
void __fastcall equip_entries_release(void* self, void*) {
    for (I32 i = 0; i < li(self, 0x1B90); ++i) {
        if (void* entry = lp(lp(self, 0x1B8C), static_cast<U32>(i) * 4u)) vcall<void*, U32>(entry, 0x00, 1);
        sw(lp(self, 0x1B8C), static_cast<U32>(i) * 4u, 0);
    }
    zero(self, 0x1B90);
    if (lp(self, 0x1B8C) != nullptr) { live_free(lp(self, 0x1B8C)); zero(self, 0x1B8C); zero(self, 0x1B94); }
    zero(self, 0x1B90);
}
void set_bit1(void* self, U32 off, U32 on) { sw(self, off, (lw(self, off) & 0xFFFFFFFDu) | ((on & 1u) << 1)); }
// One candidate NPC slot (body shared by both scan loops, 006B5C07..006B5CED / 006B5E1F..006B5F00).
void portrait_candidate(void* self, I32 slot, U32& first, U32& second) {
    if (slot == static_cast<std::int8_t>(lb(self, 0x4274))) return;
    if (kotor_fastcall_005636b0(party_table(), nullptr, slot) == 0) return;
    void* app = client_app();
    const U32 object = recovered_npc_object_00564700(party_table(), nullptr, slot, 0, 1);
    void* creature = pt(frame_R4_005ed5b0(app, nullptr, kotor_fastcall_005eda50(app, nullptr, object)));
    if (creature == nullptr) return;
    if (first != 0) {
        portrait_fill(self, 0x3C64, creature, 0); portrait_fill(self, 0x3CD8, creature, 0);
        sw(self, 0x3C3C, static_cast<U32>(slot)); second = 1;
    } else {
        portrait_fill(self, 0x3AA0, creature, 0); portrait_fill(self, 0x3B14, creature, 0);
        sw(self, 0x3A78, static_cast<U32>(slot)); first = 1;
    }
}
}

extern "C" {

// 006B59F0 CSWGuiInGameEquip::ShowCantEquipMessage(int show, int gui_string) (thiscall, RET 8).
void __fastcall kotor_fastcall_006b59f0(void* self, void*, U32 show, U32 gui_string) {
    const U32 hidden = show == 0 ? 1u : 0u;                                                    // 006B5A15..006B5A1B
    for (U32 off : {0x1BDCu, 0x1E5Cu, 0x20DCu, 0x1D1Cu, 0x1F9Cu, 0x221Cu, 0x25DCu, 0x271Cu, 0x2ADCu, 0x2C1Cu})
        set_bit1(self, off, hidden);                                                           // 006B5A1E..006B5AC5
    if (show != 0) {                                                                           // 006B5AC3..006B5ACB
        U32 text[2];
        kotor_fastcall_005edeb0(client_app(), nullptr, text, gui_string);                     // 006B5ACD..006B5ADF
        { auto w4_guard_0 = ::w4gui::on_throw([&]() { frame_LogStringDtor_005e5c20(text, nullptr); });  gui_load_text_params_set_text_00415e00(at(self, 0x4218), nullptr, text); w4_guard_0.armed = false; }
        frame_LogStringDtor_005e5c20(text, nullptr);                                           // 006B5AF8..006B5B04
    }
    set_bit1(self, 0x4174, show);                                                              // 006B5B09..006B5B1F
    vcall<void, U32>(at(self, 0x3698), 0x88, hidden);                                          // 006B5B19..006B5B28
    gui_load_text_params_set_color_00414e10(at(self, 0x3804), nullptr,
        show != 0 ? static_cast<const void*>(kotor_m1_color_007a2408) : static_cast<const void*>(borrowed_exit_color_007a23b4)); // 006B5B2E..006B5B43
}

// 006B5B60 CSWGuiInGameEquip::UpdatePortraits(int show) (thiscall, RET 4).
void __fastcall kotor_fastcall_006b5b60(void* self, void*, U32 show) {
    if (show == 0) {                                                                           // 006B5B6C..006B5BA7
        for (U32 off : {0x3A64u, 0x3C28u, 0x3DECu, 0x3FB0u}) sw(self, off, lw(self, off) & 0xFFFFFFFDu);
        return;
    }
    if ((lb(recovered_server_info_004aec90(server_app(), nullptr), 0x104) & 1u) != 0) {               // 006B5BAA..006B5BBE
        U32 first = 0, second = 0;                                                             // [esp+58] / ebp
        std::int8_t cur = static_cast<std::int8_t>(lb(self, 0x3A78));                          // 006B5BC4..006B5BDA
        if (static_cast<U8>(cur) == 0xFE) cur = static_cast<std::int8_t>(lb(self, 0x4274) + 1);
        const I32 start = cur;                                                                 // [esp+10]
        for (I32 slot = start; slot < 9 && second == 0; ++slot) portrait_candidate(self, slot, first, second); // 006B5BDD..006B5CF9
        bool scan_low = true;
        if (static_cast<std::int8_t>(lb(self, 0x4274)) != -1) {                                // 006B5CFF..006B5D0A
            if (second != 0) scan_low = false;                                                 // 006B5D10..006B5D12
            else if (first == 0) {                                                             // 006B5D18..006B5D86
                portrait_fill(self, 0x3AA0, client_get_player_creature_005ed540(client_app(), nullptr), 0);
                portrait_fill(self, 0x3B14, client_get_player_creature_005ed540(client_app(), nullptr), 0);
                sw(self, 0x3A78, 0xFFFFFFFFu); first = 1;
            } else {                                                                           // 006B5D88..006B5DED
                portrait_fill(self, 0x3C64, client_get_player_creature_005ed540(client_app(), nullptr), 0);
                portrait_fill(self, 0x3CD8, client_get_player_creature_005ed540(client_app(), nullptr), 0);
                sw(self, 0x3C3C, 0xFFFFFFFFu); second = 1;
                scan_low = false;
            }
        } else if (second != 0) {                                                              // 006B5DF2..006B5DF4
            scan_low = false;
        }
        if (scan_low)
            for (I32 slot = 0; slot < start && second == 0; ++slot) portrait_candidate(self, slot, first, second); // 006B5DFA..006B5F0F
        set_bit1(self, 0x3A64, first);                                                         // 006B5F15..006B5F2B
        set_bit1(self, 0x3C28, second);                                                        // 006B5F31..006B5F44
        set_bit1(self, 0x3DEC, kotor_fastcall_005636d0(party_table(), nullptr) > 2 ? 1u : 0u); // 006B5F4A..006B5F78
        set_bit1(self, 0x3FB0, kotor_fastcall_005636d0(party_table(), nullptr) > 2 ? 1u : 0u); // 006B5F7E..006B5FAD
        return;
    }
    void* party = kotor_fastcall_005ed8b0(client_app(), nullptr);                              // 006B5FBC..006B5FC5
    if (li(party, 0) > 1) {                                                                    // 006B5FCC..006B5FDA
        sw(self, 0x3A64, lw(self, 0x3A64) | 2u);
        portrait_fill(self, 0x3AA0, frame_GetCharacter_006346c0(party, nullptr, 1), 0);        // 006B5FDF..006B6007
        portrait_fill(self, 0x3B14, frame_GetCharacter_006346c0(party, nullptr, 1), 0);        // 006B600C..006B602E
    } else {
        sw(self, 0x3A64, lw(self, 0x3A64) & 0xFFFFFFFDu);                                      // 006B6035..006B6037
    }
    if (li(party, 0) > 2) {                                                                    // 006B603D..006B6040
        sw(self, 0x3C28, lw(self, 0x3C28) | 2u);
        portrait_fill(self, 0x3C64, frame_GetCharacter_006346c0(party, nullptr, 2), 0);        // 006B604B..006B6073
        portrait_fill(self, 0x3CD8, frame_GetCharacter_006346c0(party, nullptr, 2), 0);        // 006B6078..006B609A
    } else {
        sw(self, 0x3C28, lw(self, 0x3C28) & 0xFFFFFFFDu);                                      // 006B60A9..006B60B2
    }
}

// 006B7680 CSWGuiInGameEquip::ShowDescription(int show) (thiscall, RET 4).
void __fastcall kotor_fastcall_006b7680(void* self, void*, U32 show) {
    static const U32 toggled[22] = {0xAC, 0x1090, 0x270, 0x11D0, 0x434, 0x1310, 0x5F8, 0x1450, 0x7BC, 0x1590, 0x980,
                                    0x16D0, 0xB44, 0x1810, 0xD08, 0x1950, 0xECC, 0x1A90, 0x285C, 0x299C, 0x249C, 0x2D5C};
    kotor_fastcall_006b59f0(self, nullptr, 0, 0xFFFFFFFFu);                                    // 006B769C..006B76A3
    kotor_fastcall_006b5b60(self, nullptr, show == 0 ? 1u : 0u);                               // 006B76A8..006B76B6
    sw(self, 0x33FC, show != 0 ? (lw(self, 0x33FC) | 2u) : (lw(self, 0x33FC) & 0xFFFFFFFDu));  // 006B76C3..006B76D0 / 006B77E6..006B77F3
    {
        U32 text[2];
        kotor_fastcall_005edeb0(client_app(), nullptr, text, show != 0 ? 0x62Du : 0x62Eu);      // 006B76D6..006B76E9 / 006B77F9..006B780B
        { auto w4_guard_1 = ::w4gui::on_throw([&]() { frame_LogStringDtor_005e5c20(text, nullptr); });  gui_load_text_params_set_text_00415e00(at(self, 0x39C8), nullptr, text); w4_guard_1.armed = false; }
        frame_LogStringDtor_005e5c20(text, nullptr);
    }
    for (U32 off : toggled) sw(self, off, show != 0 ? (lw(self, off) & 0xFFFFFFFDu) : (lw(self, off) | 2u)); // 006B770F..006B779A / 006B7835..006B78C0
    sw(self, 0x36DC, show != 0 ? (lw(self, 0x36DC) | 2u) : (lw(self, 0x36DC) & 0xFFFFFFFDu));  // 006B77A0 / 006B78C6
    const U32 alpha = show != 0 ? 0x3F800000u : 0u;                                            // ebp: 1.0f / 0
    for (I32 i = 0; i < li(self, 0x3378); ++i) {                                               // 006B77A6..006B77DF / 006B78CC..006B78FE
        void* control = kotor_fastcall_0041a5a0(at(self, 0x30D8), nullptr, i);
        sw(vcall<void*>(control, 0x58), 0x8C, alpha);
    }
}

// ======================= 006BA980 CSWGuiInGameEquip (0x42BC, table 007569A0) =======================
void* __fastcall kotor_fastcall_006ba980(void* self, void*, void* manager) {
    // .rdata 00756564: slot-name column of the 9-entry equip slot table (stride 14h).
    static const char* const slots[9] = {"INV_WEAP_L", "INV_WEAP_R", "INV_HEAD", "INV_ARM_L", "INV_ARM_R",
                                         "INV_BODY", "INV_HANDS", "INV_IMPLANT", "INV_BELT"};
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 006BA9AB
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_equip());                                                                // 006BA9CB (007569A0)
    vector_member(at(self, 0x68), 0x1C4, 9, &gui_load_button_construct_0041c0f0, &gui_load_button_destruct_00419da0, u); // 006BA9B0..006BA9D1
    vector_member(at(self, 0x104C), 0x140, 9, &gui_load_label_construct_0041acd0, &gui_load_label_destruct_00419d10, u); // 006BA9D6..006BA9F3
    zero(self, 0x1B90); zero(self, 0x1B94); zero(self, 0x1B8C);                                // 006BA9F8..006BAA04
    u.push(&equip_entries_release, self);
    for (U32 i = 0; i < 17; ++i) label(self, 0x1B98 + i * 0x140, u);                           // 006BAA0A..006BAB15
    listbox(self, 0x30D8, u); listbox(self, 0x33B8, u);                                        // 006BAB1A..006BAB35
    for (U32 off : {0x3698u, 0x385Cu, 0x3A20u, 0x3BE4u, 0x3DA8u, 0x3F6Cu}) button(self, off, u); // 006BAB3A..006BAB95
    label(self, 0x4130, u);                                                                    // 006BABA5
    zero(self, 0x64);                                                                          // 006BABC1
    sw(self, 0x4270, lw(self, 0x4270) & 0xFFFFFFFEu);                                          // 006BABAA..006BABC4
    sb(self, 0x4274, 0xFF);                                                                    // 006BABCA
    start_layout(self, "equip");                                                               // 006BABB3..006BABDD
    {
        ExoString btn("BTN_");                                                                 // 006BABE2..006BABEB [esp+38]
        ExoString lbl("LBL_");                                                                 // 006BABF0..006BABFE [esp+30]
        for (U32 i = 0; i < 9; ++i) {                                                          // 006BAC09..006BAD4E
            void* slot = at(self, 0x68 + i * 0x1C4);
            {
                ExoString suffix(slots[i]); U32 name[2];
                GuiConcat005e5d10(static_cast<SS*>(static_cast<void*>(btn.storage)), nullptr, static_cast<SS*>(static_cast<void*>(name)),
                                  static_cast<SS*>(static_cast<void*>(suffix.storage)));    // 006BAC30..006BAC53
                gui_load_panel_init_control_0040b930(self, nullptr, slot, name, 1);            // 006BAC58..006BAC63
                frame_LogStringDtor_005e5c20(name, nullptr);                                   // 006BAC68..006BAC71
            }                                                                                  // 006BAC76..006BAC7E
            {
                ExoString suffix(slots[i]); U32 name[2];
                GuiConcat005e5d10(static_cast<SS*>(static_cast<void*>(lbl.storage)), nullptr, static_cast<SS*>(static_cast<void*>(name)),
                                  static_cast<SS*>(static_cast<void*>(suffix.storage)));    // 006BAC83..006BACA6
                gui_load_panel_init_control_0040b930(self, nullptr, at(self, 0x104C + i * 0x140), name, 1); // 006BACAB..006BACBA
                frame_LogStringDtor_005e5c20(name, nullptr);                                   // 006BACBF..006BACC8
            }                                                                                  // 006BACCD..006BACD5
            gui_load_control_add_event_0041ab20(slot, nullptr, 0x27, self, fn(&kotor_fastcall_006b8eb0)); // 006BACDA..006BACE4
            gui_load_control_add_event_0041ab20(slot, nullptr, 0, self, fn(&kotor_fastcall_006b9470));    // 006BACE9..006BACF3
            sw(slot, 0x58, i);                                                                 // 006BACF8..006BAD08
            gui_load_border_params_set_pulsing_00414d00(at(slot, 0xF4), nullptr, 1, 0, 1);     // 006BACFC..006BAD0B
            sw(self, 0x427C + i * 4, 0x7F000000u);                                             // 006BAD10..006BAD1C
        }
        static const char* const names[17] = {
            "LBL_ATKL", "LBL_TOHITL", "LBL_ATKR", "LBL_TOHITR", "LBL_DEF", "LBL_VITALITY", nullptr, "LBL_PORTRAIT",
            "LBL_ATTACK_INFO", "LBL_DEF_INFO", "LBL_TXTBAR", "LBL_PORT_BORD", "LBL_TOHIT", "LBL_DAMAGE", "LBL_SLOTNAME",
            "LBL_TITLE", "LBL_SELECTTITLE"};
        for (U32 i = 0; i < 6; ++i) init_control(self, 0x1B98 + i * 0x140, names[i]);          // 006BAD54..006BAE5D
        init_control(self, 0x30D8, "LB_ITEMS");                                                // 006BAE92
        init_control(self, 0x33B8, "LB_DESC");                                                 // 006BAEC7
        for (U32 i = 7; i < 17; ++i) init_control(self, 0x1B98 + i * 0x140, names[i]);         // 006BAEFC..006BB0D9
        init_control(self, 0x3698, "BTN_EQUIP");                                               // 006BB10E
        init_control(self, 0x385C, "BTN_BACK");                                                // 006BB143
        init_control(self, 0x3A20, "BTN_CHANGE1");                                             // 006BB178
        init_control(self, 0x3BE4, "BTN_CHANGE2");                                             // 006BB1AD
        init_control(self, 0x3DA8, "BTN_CHARLEFT");                                            // 006BB1E2
        init_control(self, 0x3F6C, "BTN_CHARRIGHT");                                           // 006BB217
        init_control(self, 0x4130, "LBL_CANTEQUIP");                                           // 006BB24C
        stop_layout(self);                                                                     // 006BB281..006BB283
        for (U32 off : {0x1C80u, 0x1F00u, 0x1DC0u, 0x2040u, 0x2180u}) {                        // 006BB288..006BB373 (0073D71C)
            ExoString empty(""); gui_load_text_params_set_text_00415e00(at(self, off), nullptr, empty.storage);
        }
        void* proto = lp(self, 0x3684);                                                        // 006BB378 (listbox +33B8 prototype)
        vcall<void, const void*, void*, float>(at(self, 0x2318), 0x98, at(proto, 4), proto, 1.0f); // 006BB37E..006BB394
        kotor_fastcall_006b7680(self, nullptr, 0);                                             // 006BB39A..006BB39E ShowDescription(0)
        struct Ev { U32 off; const void* fn; };
        const Ev events[4] = {{0x3698, fn(&kotor_fastcall_006b9160)}, {0x385C, on_b()},
                              {0x3A20, fn(&kotor_fastcall_006ba820)}, {0x3BE4, fn(&kotor_fastcall_006ba820)}}; // 006BB3A3..006BB418
        for (const Ev& e : events) { sw(self, e.off + 0x44, lw(self, e.off + 0x44) & 0xFFFFFFFBu); add_event(self, e.off, 0x27, self, e.fn); }
        sw(self, 0x36DC, lw(self, 0x36DC) & 0xFFFFFFFDu);                                      // 006BB41D (BTN_EQUIP)
        sw(self, 0x3A44, 0x9725); sw(self, 0x3A50, 0xCE);                                      // 006BB421..006BB429 (BTN_CHANGE1 +24/+30)
        sw(self, 0x3C08, 0x9725);                                                              // 006BB436
        const Ev arrows[2] = {{0x3DA8, fn(&kotor_fastcall_006b60f0)}, {0x3F6C, fn(&kotor_fastcall_006b6370)}}; // 006BB430..006BB46F
        for (const Ev& e : arrows) { sw(self, e.off + 0x44, lw(self, e.off + 0x44) & 0xFFFFFFFBu); add_event(self, e.off, 0x27, self, e.fn); }
        sw(self, 0x3FF4, 0x43340000u); sw(self, 0x4068, 0x43340000u);                          // 006BB474..006BB485 (180.0f)
        vcall<void, U32>(at(self, 0x30D8), 0x88, 0);                                           // 006BB479..006BB490 (edi=0 pushed earlier)
        sw(self, 0x4174, lw(self, 0x4174) & 0xFFFFFFFDu);                                      // 006BB496
        vcall<void, void*, U32>(self, 0x08, at(self, 0x93C), 0);                               // 006BB49D..006BB4A9
        sw(self, 0x42A4, 0); sw(self, 0x42A8, 0x7F000000u);                                    // 006BB4B0..006BB4B6
        for (U32 off : {0x42ACu, 0x42B0u, 0x42B4u, 0x42B8u}) zero(self, off);                  // 006BB4C0..006BB4D2
    }                                                                                          // 006BB4AC..006BB4EB ~lbl, ~btn
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_006b8840(void* self, void*) {
    set_vptr(self, vt_equip());                                                                // 006B8862
    gui_load_listbox_clear_items_00419a90(at(self, 0x33B8), nullptr);                          // 006B8878
    gui_load_listbox_clear_items_00419a90(at(self, 0x30D8), nullptr);                          // 006B8883
    equip_entries_release(self, nullptr);                                                      // 006B8888..006B88EC
    if (void* a = lp(self, 0x42B0)) { vcall<void*, U32>(a, 0x00, 1); zero(self, 0x42B0); }     // 006B88F2..006B8902
    if (void* b = lp(self, 0x42B8)) { vcall<void*, U32>(b, 0x00, 1); zero(self, 0x42B8); }     // 006B8908..006B8918
    gui_load_label_destruct_00419d10(at(self, 0x4130), nullptr);
    for (U32 off : {0x3F6Cu, 0x3DA8u, 0x3BE4u, 0x3A20u, 0x385Cu, 0x3698u}) gui_load_button_destruct_00419da0(at(self, off), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x33B8), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x30D8), nullptr);
    for (U32 i = 17; i-- > 0;) gui_load_label_destruct_00419d10(at(self, 0x1B98 + i * 0x140), nullptr);
    live_free(lp(self, 0x1B8C)); zero(self, 0x1B8C);                                           // 006B8D9C..006B8DC3
    vector_destruct(at(self, 0x104C), 0x140, 9, &gui_load_label_destruct_00419d10);
    vector_destruct(at(self, 0x68), 0x1C4, 9, &gui_load_button_destruct_00419da0);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006b9450(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006b8840, flags); }
}  // extern "C"
