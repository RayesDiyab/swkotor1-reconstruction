// stale-stubs lane, group G5 "client" (Claude): faithful bodies for former trap stubs of
//   main_interface_ctor.obj (CSWGuiMainInterface button/event handlers, PMF targets registered by the 0068C100 ctor) and
//   0838-play-handoff-m15f-traps.obj (CSWGuiTutorialBox::Perform* message-box callbacks registered by 006AA900),
// plus the missing originals their bodies call. Transcribed from the original swkotor.exe (read-only); every comment
// address is an original instruction. No original address is dereferenced: [007A39FC] is the rebuilt
// kotor_menu_window_cells::manager_007a39fc cell (CAppManager*: +4 client, +8 server).
// ABI (all verified against the original RET):
//   event handlers   thiscall(CSWGuiControl* control)                        RET 4  -> __fastcall(self, edx, control) @12
//   00686680 CSWGuiTargetActionMenu::SelectPrevAction(int slot)              RET 4
//   006888E0 CSWGuiMainInterface::SelectPrevPersonalAction(int slot)          RET 4
//   00616900 CSWCCreature::PerformCombatFeat(ushort feat, CSWCCreature*)       RET 8
//   0060EF10 CSWCCreature::PerformCastSpell(7 stack words)                    RET 1Ch
//   006167D0 CSWCCreature::PerformUseHostileItem(id, CSWCCreature*)            RET 8
//   0060EE60 CSWCCreature::PerformUseItem(4 stack words)                      RET 10h
//   Perform* CSWGuiTutorialBox callbacks: members of kotor_m15_l0::TutorialBoxHandlers (L15-0 PMF contract), RET 4.
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
inline unsigned char* at(void* base, U off) { return static_cast<unsigned char*>(base) + off; }
inline U ld(void* base, U off) { U v; std::memcpy(&v, at(base, off), 4); return v; }
inline I lds(void* base, U off) { I v; std::memcpy(&v, at(base, off), 4); return v; }
inline void st(void* base, U off, U v) { std::memcpy(at(base, off), &v, 4); }
inline void* lp(void* base, U off) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(ld(base, off))); }
}

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }    // [007A39FC] CAppManager*
namespace {
inline void* client() { return lp(kotor_menu_window_cells::manager_007a39fc, 4); }  // mov ecx,[[007A39FC]+4]
}

namespace kotor_input_required {
void* __fastcall get_in_game_gui_005ed690(void* client, void*);                     // 005ED690 GetInGameGui (0457)
void __fastcall show_solo_mode_query_0062e550(void* gui, void*, U arg);             // 0062E550 RET 4
void __fastcall party_menu_stealth_0060f4b0(void* creature, void*, U a, U b);       // 0060F4B0 RET 8
void* __fastcall get_character_006346c0(void* party, void*, U index);               // 006346C0 RET 4 (0457)
void __fastcall select_next_action_006865b0(void* menu, void*, U slot);             // 006865B0 RET 4
void __fastcall select_next_action_00688820(void* mi, void*, U slot);               // 00688820 RET 4
void __fastcall player_flourish_weapons_005ede90(void* client, void*);              // 005EDE90
}

extern "C" {
// ---------------- existing providers (c17 map, exact decorated arity) ----------------
U __fastcall kotor_fastcall_0062c9b0(void* gui, void*, U panel);                    // 0062C9B0 ShowSWInGameGui RET 4
void __fastcall kotor_fastcall_005eda60(void* client, void*, U cls, U flag);        // 005EDA60 SetInputClass
U __fastcall show_tutorial_window_005edf40(void* client, void*, U id, U a, U b, U c); // 005EDF40 ShowTutorialWindow
void __fastcall kotor_fastcall_006880c0(void* mi, void*);                           // 006880C0 OnCombatYButton
void __fastcall kotor_fastcall_006887d0(void* mi, void*);                           // 006887D0 ClearAllActions
U __fastcall kotor_fastcall_005edc10(void* client, void*);                          // 005EDC10 GetPausedByCombat
void __fastcall kotor_fastcall_005edc20(void* client, void*, U a, U b, U c);        // 005EDC20 SetPausedByCombat
U __fastcall kotor_fastcall_005edef0(void* client, void*);                          // 005EDEF0 GetAutoPaused
void __fastcall kotor_fastcall_005edee0(void* client, void*, U a, U b);             // 005EDEE0 SetAutoPaused
void* __fastcall client_get_player_creature_005ed540(void* client, void*);          // 005ED540 GetPlayerCreature
U __fastcall kotor_fastcall_00610ac0(void* creature, void*);                        // 00610AC0 IsStealthCapable
void* __fastcall kotor_fastcall_005ed8b0(void* client, void*);                      // 005ED8B0 GetSWParty
void* __fastcall kotor_fastcall_00635460(void* party, void*);                       // 00635460 GetPlayerCharacter
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*); // 0060FB20
U __fastcall recovered_can_level_up_005a6810(void* stats, void*);                   // 005A6810 CanLevelUp
U __fastcall recovered_creature_incapacitated_004ef890(void* creature, void*);      // 004EF890 GetDeadTemp
void __fastcall kotor_fastcall_005edc30(void* client, void*, U index, U flag);      // 005EDC30 ChangeCharacter
void* __fastcall kotor_fastcall_005ed700(void* client, void*);                      // 005ED700 GetClientOptions
void* __fastcall kotor_fastcall_005eda40(void* client, void*);                      // 005EDA40 GetSWGuiManager
void __fastcall kotor_fastcall_0040a140(void* manager, void*, U sound);             // 0040A140 PlayGuiSound RET 4
void* __fastcall frame_R4_005ed5b0(void* client, void*, U id);                      // 005ED5B0 GetCreatureByGameObjectID
void* __fastcall kotor_fastcall_005ed6f0(void* client, void*);                      // 005ED6F0 GetSWCMessage
U __fastcall kotor_fastcall_00677a90(void* msg, void*, U id, U feat);               // 00677A90 SendInput_Attack RET 8
U __fastcall kotor_fastcall_006776a0(void* msg, void*, U, U, U, U, U, U, U);        // 006776A0 SendInput_CastSpell RET 1Ch
U __fastcall kotor_fastcall_00610a10(void* creature, void*, U mode);                // 00610A10 SetCombatMode RET 4
U __fastcall kotor_fastcall_0060f590(void* creature, void*, U a, U b);              // 0060F590 PartyMenuUseItem RET 8
U __fastcall kotor_fastcall_00677bd0(void* msg, void*, U, U, U, U, U);              // 00677BD0 SendInput_UseItem RET 14h
void* __fastcall recovered_client_get_door_by_id_005ed600(void* client, void*, U id);      // 005ED600
void* __fastcall recovered_client_get_placeable_by_id_005ed5f0(void* client, void*, U id); // 005ED5F0
void __fastcall kotor_fastcall_00683e90(void* door, void*, U a, void* creature);    // 00683E90 MenuActionBash RET 8
void __fastcall kotor_fastcall_006826a0(void* plc, void*, U a, void* creature);     // 006826A0 BashPlaceable RET 8
void __fastcall kotor_fastcall_00616800(void* target, void*, U a, void* creature);  // 00616800 ActionMenuAttack RET 8

// ---------------- bodies defined here ----------------
void __fastcall kotor_fastcall_006883c0(void* self, void*, void* control);
void __fastcall kotor_fastcall_0068b050(void* self, void*, void* control);
void __fastcall kotor_fastcall_0068b0a0(void* self, void*, void* control);
void __fastcall kotor_fastcall_00688590(void* self, void*, void* control);
void __fastcall kotor_fastcall_00688610(void* self, void*, void* control);
void __fastcall kotor_fastcall_00688640(void* self, void*, void* control);
void __fastcall kotor_fastcall_00688690(void* self, void*, void* control);
void __fastcall kotor_fastcall_006884b0(void* self, void*, void* control);
void __fastcall kotor_fastcall_00688520(void* self, void*, void* control);
void __fastcall kotor_fastcall_0068af70(void* self, void*, void* control);
void __fastcall kotor_fastcall_0068afe0(void* self, void*, void* control);
void __fastcall kotor_fastcall_00686680(void* menu, void*, U slot);
void __fastcall kotor_fastcall_006888e0(void* self, void*, U slot);
U __fastcall kotor_fastcall_00616900(void* creature, void*, U feat, void* combat_creature);
U __fastcall kotor_fastcall_0060ef10(void* creature, void*, U a1, U a2, U a3, U a4, U a5, U a6, U a7);
U __fastcall kotor_fastcall_006167d0(void* creature, void*, U a1, void* target);
U __fastcall kotor_fastcall_0060ee60(void* creature, void*, U a1, U a2, U a3, U a4);
}

// ===================================================================================== main interface handlers
extern "C" {
// 006883C0 CSWGuiMainInterface::OnMenuButtonPressed: map the pressed button (control->vtable[58h]()) to an in-game
// panel index and show it; on success switch the input class.
void __fastcall kotor_fastcall_006883c0(void* self, void*, void* control) {
    if (ld(control, 0x4C) == 0) return;                                               // 006883C3..006883CC
    using VFn = void* (__fastcall*)(void*, void*);
    void* const pressed = reinterpret_cast<VFn>(ld(lp(control, 0), 0x58))(control, nullptr);   // 006883D2..006883D4
    U panel;
    if (pressed == at(self, 0xB00C)) panel = 0;                                       // 006883D7..006883E3
    else if (pressed == at(self, 0xB1D0)) panel = 1;                                  // 006883E5..006883F4
    else if (pressed == at(self, 0xB394)) panel = 2;                                  // 006883F6..00688405
    else if (pressed == at(self, 0xB558)) panel = 3;                                  // 00688407..00688416
    else if (pressed == at(self, 0xB71C)) panel = 4;                                  // 00688418..00688427
    else if (pressed == at(self, 0xB8E0)) panel = 5;                                  // 00688429..00688438
    else if (pressed == at(self, 0xBAA4)) panel = 6;                                  // 0068843A..00688442 -> 00688453
    else if (pressed == at(self, 0x6098)) panel = 6;                                  // 00688444..00688453
    else panel = 7;                                                                   // 0068844C
    void* const gui = kotor_input_required::get_in_game_gui_005ed690(client(), nullptr);   // 00688458..00688462
    if (kotor_fastcall_0062c9b0(gui, nullptr, panel) == 0) return;                    // 00688467..00688470
    kotor_fastcall_005eda60(client(), nullptr, 2, 1);                                 // 00688472..0068847E
}

// 0068B050 CSWGuiMainInterface::OnClearOneButtonPressed.
void __fastcall kotor_fastcall_0068b050(void* self, void*, void* control) {
    if (ld(control, 0x4C) == 0) return;                                               // 0068B057..0068B05C
    if ((*at(self, 0x6D14) & 2) == 0) return;                                         // 0068B05E..0068B065
    if (show_tutorial_window_005edf40(client(), nullptr, 0x20, 0x7F000000u, 0x7F000000u, 0) != 0) return;  // 0068B067..0068B085
    kotor_fastcall_006880c0(self, nullptr);                                           // 0068B087..0068B089
}

// 0068B0A0 CSWGuiMainInterface::OnClearAllButtonPressed (ECX=self is passed through unchanged).
void __fastcall kotor_fastcall_0068b0a0(void* self, void*, void* control) {
    if (ld(control, 0x4C) == 0) return;                                               // 0068B0A4..0068B0A9
    kotor_fastcall_006887d0(self, nullptr);                                           // 0068B0AB
}

// 00688590 CSWGuiMainInterface::OnPauseButtonPressed: toggle combat pause (reason 4), clear auto-pause, tutorial 6.
void __fastcall kotor_fastcall_00688590(void*, void*, void* control) {
    if (ld(control, 0x4C) == 0) return;                                               // 00688594..00688599
    const U paused = kotor_fastcall_005edc10(client(), nullptr);                      // 0068859B..006885A4
    kotor_fastcall_005edc20(client(), nullptr, paused == 0 ? 1u : 0u, 4, 0);          // 006885A9..006885BD
    if (kotor_fastcall_005edef0(client(), nullptr) != 0)                              // 006885C2..006885D2
        kotor_fastcall_005edee0(client(), nullptr, 0, 0);                             // 006885D4..006885E1
    (void)show_tutorial_window_005edf40(client(), nullptr, 6, 0x7F000000u, 0x7F000000u, 0);  // 006885E6..006885FC
}

// 00688610 CSWGuiMainInterface::OnSoloButtonPressed.
void __fastcall kotor_fastcall_00688610(void*, void*, void* control) {
    if (ld(control, 0x4C) == 0) return;                                               // 00688614..00688619
    void* const gui = kotor_input_required::get_in_game_gui_005ed690(client(), nullptr);   // 0068861B..00688626
    kotor_input_required::show_solo_mode_query_0062e550(gui, nullptr, 0);            // 00688624, 0068862B..0068862D
}

// 00688640 CSWGuiMainInterface::OnStealthButtonPressed.
void __fastcall kotor_fastcall_00688640(void*, void*, void* control) {
    if (ld(control, 0x4C) == 0) return;                                               // 00688644..00688649
    void* const player = client_get_player_creature_005ed540(client(), nullptr);      // 0068864B..00688654
    if (kotor_fastcall_00610ac0(player, nullptr) == 0) return;                        // 00688659..00688662
    void* const party = kotor_fastcall_005ed8b0(client(), nullptr);                   // 00688664..00688671
    void* const pc = kotor_fastcall_00635460(party, nullptr);                         // 00688676..00688678
    kotor_input_required::party_menu_stealth_0060f4b0(pc, nullptr, 0, 0);             // 0068866D..0068866F, 0068867D..0068867F
}

// 00688690 CSWGuiMainInterface::OnCharacterClicked: portrait 1 opens the level-up (character) or the equipment
// panel; portraits 2/3 switch the controlled party member.
void __fastcall kotor_fastcall_00688690(void* self, void*, void* control) {
    if (ld(control, 0x4C) == 0) return;                                               // 00688697..0068869C
    const I index = lds(control, 0x58) - 1;                                           // 006886A2..006886A5
    if (index == 0) {                                                                 // 006886A6..006886A8
        void* const c = kotor_input_required::get_character_006346c0(kotor_fastcall_005ed8b0(client(), nullptr), nullptr, 0); // 006886AA..006886BB
        if (c == nullptr) return;                                                     // 006886C0..006886C2
        void* const server = kotor_m15a_creature_get_server_creature_0060fb20(c, nullptr);   // 006886C8..006886CA
        if (server == nullptr) return;                                                // 006886CF..006886D1
        if (recovered_can_level_up_005a6810(lp(server, 0xA74), nullptr) != 0) {        // 006886D7..006886E4
            st(self, 0xB3E0, 1);                                                      // 006886EF
            kotor_fastcall_006883c0(self, nullptr, at(self, 0xB394));                 // 006886E6..006886F9
            return;
        }
        st(self, 0xB058, 1);                                                          // 0068870B
        kotor_fastcall_006883c0(self, nullptr, at(self, 0xB00C));                     // 00688702..00688715
        return;
    }
    if (index >= 3) return;                                                           // 0068871E..00688721 (signed)
    I member = index;                                                                 // 00688723
    if (lds(kotor_fastcall_005ed8b0(client(), nullptr), 0) < 3) member = 1;           // 00688725..00688737 (signed)
    void* const c = kotor_input_required::get_character_006346c0(kotor_fastcall_005ed8b0(client(), nullptr), nullptr,
                                                                 static_cast<U>(member));  // 0068873C..0068874E
    if (c == nullptr) return;                                                         // 00688753..00688757
    if (kotor_m15a_creature_get_server_creature_0060fb20(c, nullptr) == nullptr) return;  // 00688759..00688762
    void* const server = kotor_m15a_creature_get_server_creature_0060fb20(c, nullptr);    // 00688764..00688766 (re-read)
    if (recovered_creature_incapacitated_004ef890(server, nullptr) != 0) return;      // 0068876B..00688774
    kotor_fastcall_005edc30(client(), nullptr, static_cast<U>(member), 1);            // 00688776..00688782
}
}

namespace {
// shared head of the four arrow handlers (006884B0/00688520/0068AF70/0068AFE0): option bit 0x8000 sets the auto-pause
// (1, reason 7), then tutorial window 5.
void arrow_head() {
    void* const options = kotor_fastcall_005ed700(client(), nullptr);                 // 006884C2..006884CA
    if ((ld(options, 0x14) & 0x8000u) != 0)                                           // 006884CF..006884D4 (test ch,ch; jns)
        kotor_fastcall_005edee0(client(), nullptr, 1, 7);                             // 006884D6..006884E3
    (void)show_tutorial_window_005edf40(client(), nullptr, 5, 0x7F000000u, 0x7F000000u, 0);  // 006884E8..006884FF
}
}

extern "C" {
// 006884B0 CSWGuiMainInterface::OnTargetUpArrowPressed: SelectPrevAction(target menu at +BCh, control[58h]-4).
void __fastcall kotor_fastcall_006884b0(void* self, void*, void* control) {
    if (ld(control, 0x4C) == 0) return;                                               // 006884B7..006884BC
    const U id = ld(control, 0x58);                                                   // 006884BF
    arrow_head();
    kotor_fastcall_00686680(at(self, 0xBC), nullptr, id - 4);                         // 00688504..0068850E
}
// 00688520 CSWGuiMainInterface::OnTargetDownArrowPressed: SelectNextAction(target menu, control[58h]-4).
void __fastcall kotor_fastcall_00688520(void* self, void*, void* control) {
    if (ld(control, 0x4C) == 0) return;                                               // 00688527..0068852C
    const U id = ld(control, 0x58);                                                   // 0068852F
    arrow_head();                                                                     // 00688532..0068856F
    kotor_input_required::select_next_action_006865b0(at(self, 0xBC), nullptr, id - 4);   // 00688574..0068857E
}
// 0068AF70 CSWGuiMainInterface::OnActionUpArrowPressed: SelectPrevPersonalAction(self, control[58h]).
void __fastcall kotor_fastcall_0068af70(void* self, void*, void* control) {
    if (ld(control, 0x4C) == 0) return;                                               // 0068AF77..0068AF7C
    const U id = ld(control, 0x58);                                                   // 0068AF7F
    arrow_head();                                                                     // 0068AF82..0068AFBF
    kotor_fastcall_006888e0(self, nullptr, id);                                       // 0068AFC4..0068AFC7
}
// 0068AFE0 CSWGuiMainInterface::OnActionDownArrowPressed: 00688820(self, control[58h]).
void __fastcall kotor_fastcall_0068afe0(void* self, void*, void* control) {
    if (ld(control, 0x4C) == 0) return;                                               // 0068AFE7..0068AFEC
    const U id = ld(control, 0x58);                                                   // 0068AFEF
    arrow_head();                                                                     // 0068AFF2..0068B02F
    kotor_input_required::select_next_action_00688820(self, nullptr, id);            // 0068B034..0068B037
}

// 00686680 CSWGuiTargetActionMenu::SelectPrevAction(slot): slot 0..2; vector header (data,count) at 12*slot; entry
// stride 38h, id at +8; selected-id cell 4*(slot+3*(category+3)) with category = signed byte [+1AEAh].
// The loop bound is the count cached before the search (ECX); the wrap uses that cached count; the fallback and the
// final count are re-read. The party/player call result is discarded as in the original.
void __fastcall kotor_fastcall_00686680(void* menu, void*, U slot) {
    if (static_cast<I>(slot) < 0 || static_cast<I>(slot) >= 3) return;               // 00686686..00686693
    unsigned char* const row = at(menu, 12u * slot);                                  // 0068669A..0068669E
    if (ld(row, 4) == 0) return;                                                      // 006866A1..006866A6
    (void)kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client(), nullptr), nullptr);   // 006866AC..006866BC
    const I category = static_cast<signed char>(*at(menu, 0x1AEA));                   // 006866C1
    void* const cell = at(menu, 4u * (slot + 3u * static_cast<U>(category + 3)));     // 006866C8..006866D0
    const U selected = ld(cell, 0);                                                   // 006866D3
    unsigned char* prev = nullptr;                                                    // 006866D5
    if (selected != 0xFFFFFFFFu) {                                                    // 006866D7..006866DE
        const I count = lds(row, 4);                                                  // 006866E0
        if (count > 0) {                                                              // 006866E5..006866E7
            unsigned char* const base = static_cast<unsigned char*>(lp(row, 0));      // 006866E9
            for (I i = 0; i < count; ++i) {                                           // 006866F0..006866FA
                if (ld(base, static_cast<U>(i) * 0x38u + 8u) == selected) {
                    prev = i > 0 ? base + static_cast<U>(i) * 0x38u - 0x38u           // 006866FE..00686705
                                 : base + static_cast<U>(count) * 0x38u - 0x38u;      // 0068670B..0068670E
                    break;
                }
            }
        }
    }
    const I count_now = lds(row, 4);                                                  // 00686712
    if (count_now <= 1) return;                                                       // 00686715..00686718 (signed)
    if (prev == nullptr)                                                              // 0068671A..0068671C
        prev = static_cast<unsigned char*>(lp(row, 0)) + static_cast<U>(count_now) * 0x38u - 0x38u;   // 0068671E..00686724
    st(cell, 0, ld(prev, 8));                                                         // 00686728..0068672F
    kotor_fastcall_0040a140(kotor_fastcall_005eda40(client(), nullptr), nullptr, 7);  // 00686731..00686743
}

// 006888E0 CSWGuiMainInterface::SelectPrevPersonalAction(slot): slot 0..3; count at +78h+12*slot (re-read by the
// loop compare), data at +74h+12*slot (cached at search start), selected id at +1BACh+4*slot; wrap uses the count
// cached before the loop; sound through the manager at [self+18h].
void __fastcall kotor_fastcall_006888e0(void* self, void*, U slot) {
    if (static_cast<I>(slot) < 0 || static_cast<I>(slot) >= 4) return;               // 006888E6..006888F4
    unsigned char* const count_cell = at(self, 4u * (3u * slot + 0x1Eu));             // 006888FA..006888FF
    if (ld(count_cell, 0) == 0) return;                                               // 00688902..00688905
    (void)kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client(), nullptr), nullptr);   // 0068890B..0068891C
    const U selected = ld(self, 0x1BACu + 4u * slot);                                 // 00688921
    unsigned char* prev = nullptr;                                                    // 00688928
    if (selected != 0xFFFFFFFFu) {                                                    // 0068892A..0068892D
        const I count = lds(count_cell, 0);                                           // 0068892F..00688935
        if (count > 0) {                                                              // 00688939
            unsigned char* const base = static_cast<unsigned char*>(lp(self, 0x74u + 12u * slot));   // 0068893B..00688942
            for (I i = 0; i < lds(count_cell, 0); ++i) {                              // 00688950..0068895A
                if (ld(base, static_cast<U>(i) * 0x38u + 8u) == selected) {
                    prev = i > 0 ? base + static_cast<U>(i) * 0x38u - 0x38u           // 0068895E..00688969
                                 : base + static_cast<U>(count) * 0x38u - 0x38u;      // 0068896F..0068897A
                    break;
                }
            }
        }
    }
    const I count_now = lds(count_cell, 0);                                           // 0068897E
    if (count_now <= 1) return;                                                       // 00688980..00688984 (signed)
    if (prev == nullptr)                                                              // 00688986..00688988
        prev = static_cast<unsigned char*>(lp(self, 0x74u + 12u * slot)) + static_cast<U>(count_now) * 0x38u - 0x38u; // 0068898A..00688994
    st(self, 0x1BACu + 4u * slot, ld(prev, 8));                                       // 00688998..0068899B
    kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 7);                              // 006889A2..006889A7
}

// ===================================================================================== CSWCCreature perform helpers
// 00616900 CSWCCreature::PerformCombatFeat(feat, combat_creature): send Attack(id=[this+4], feat) when a message
// object exists; then SetCombatMode(combat_creature, 1) when it is non-null. Returns the last EAX (send result,
// null message, or SetCombatMode result).
U __fastcall kotor_fastcall_00616900(void* creature, void*, U feat, void* combat_creature) {
    U result = 0;
    void* const msg = kotor_fastcall_005ed6f0(client(), nullptr);                     // 00616900..0061690B
    result = static_cast<U>(reinterpret_cast<std::uintptr_t>(msg));
    if (msg != nullptr)                                                               // 00616910..00616912
        result = kotor_fastcall_00677a90(msg, nullptr, ld(creature, 4), feat);        // 00616914..0061691F
    if (combat_creature != nullptr)                                                   // 00616924..0061692B
        result = kotor_fastcall_00610a10(combat_creature, nullptr, 1);                // 0061692D..0061692F
    return result;                                                                    // 00616934 RET 8
}

// 0060EF10 CSWCCreature::PerformCastSpell(a1..a7): SendInput_CastSpell(msg, a1, a4, a5, a6, a7, a3, a2).
U __fastcall kotor_fastcall_0060ef10(void*, void*, U a1, U a2, U a3, U a4, U a5, U a6, U a7) {
    void* const msg = kotor_fastcall_005ed6f0(client(), nullptr);                     // 0060EF10..0060EF18
    if (msg == nullptr) return 0;                                                     // 0060EF1D..0060EF1F (EAX=0)
    return kotor_fastcall_006776a0(msg, nullptr, a1, a4, a5, a6, a7, a3, a2);         // 0060EF21..0060EF46
}

// 006167D0 CSWCCreature::PerformUseHostileItem(a1, target): SetCombatMode(target, 1) if target; PartyMenuUseItem(a1, target).
U __fastcall kotor_fastcall_006167d0(void* creature, void*, U a1, void* target) {
    if (target != nullptr) (void)kotor_fastcall_00610a10(target, nullptr, 1);         // 006167D1..006167E0
    return kotor_fastcall_0060f590(creature, nullptr, a1,
                                   static_cast<U>(reinterpret_cast<std::uintptr_t>(target)));   // 006167E5..006167ED
}

// 0060EE60 CSWCCreature::PerformUseItem(a1..a4): SendInput_UseItem(msg, a1, a2, (int)a2 >> 16, a3, a4).
U __fastcall kotor_fastcall_0060ee60(void*, void*, U a1, U a2, U a3, U a4) {
    void* const msg = kotor_fastcall_005ed6f0(client(), nullptr);                     // 0060EE60..0060EE68
    if (msg == nullptr) return 0;                                                     // 0060EE6D..0060EE6F
    return kotor_fastcall_00677bd0(msg, nullptr, a1, a2, static_cast<U>(static_cast<I>(a2) >> 16), a3, a4); // 0060EE71..0060EE8D
}
}

// ===================================================================================== CSWGuiTutorialBox::Perform*
// Fields of CSWGuiTutorialBox: +984h actor id, +988h target id / item / feat byte, +98Ch action word/dword,
// +990h "already paused" flag. Every callback first unpauses (SetPausedByCombat(0,2,0)) unless +990h is set,
// then clears +990h.
namespace kotor_input_hover { struct Control; }
namespace kotor_m15_l0 {
struct TutorialBoxHandlers {
    void __thiscall PerformCombatFeat_006aa260(kotor_input_hover::Control*);
    void __thiscall PerformForcePower_006aa2c0(kotor_input_hover::Control*);
    void __thiscall PerformUseGrenade_006aa350(kotor_input_hover::Control*);
    void __thiscall PerformSetMine_006aa3d0(kotor_input_hover::Control*);
    void __thiscall PerformPlayerFlourishWeapons_006aa440(kotor_input_hover::Control*);
    void __thiscall PerformBash_006aa480(kotor_input_hover::Control*);
    void __thiscall PerformAttack_006aa520(kotor_input_hover::Control*);
    void __thiscall PerformNonCombatXButton_006aa590(kotor_input_hover::Control*);
    void __thiscall PerformCombatYButton_006aa5d0(kotor_input_hover::Control*);
};
}

namespace {
void unpause_once(void* box) {
    if (ld(box, 0x990) == 0) kotor_fastcall_005edc20(client(), nullptr, 0, 2, 0);    // test [esi+990h]; push 0,2,0
    st(box, 0x990, 0);                                                                // mov [esi+990h],0
}
void* creature_by_id(U id) { return frame_R4_005ed5b0(client(), nullptr, id); }
}

namespace kotor_m15_l0 {
// 006AA260 PerformCombatFeat.
void __thiscall TutorialBoxHandlers::PerformCombatFeat_006aa260(kotor_input_hover::Control*) {
    void* const box = this;
    void* const c = creature_by_id(ld(box, 0x984));                                   // 006AA260..006AA273
    unpause_once(box);                                                                // 006AA27A..006AA299
    if (c == nullptr) return;                                                         // 006AA2A3
    std::uint16_t feat; std::memcpy(&feat, at(box, 0x98C), 2);                                  // 006AA2A5..006AA2A7 (word)
    (void)kotor_fastcall_00616900(c, nullptr, feat, c);                               // 006AA2AE..006AA2B2
}
// 006AA2C0 PerformForcePower: cast with a zero target vector at [esp+..] (12-byte local).
void __thiscall TutorialBoxHandlers::PerformForcePower_006aa2c0(kotor_input_hover::Control*) {
    void* const box = this;
    void* const c = creature_by_id(ld(box, 0x984));                                   // 006AA2C0..006AA2D6
    unpause_once(box);                                                                // 006AA2DD..006AA2FC
    if (c == nullptr) return;                                                         // 006AA306
    U vec[3]; vec[0] = 0; vec[1] = 0;                                                 // 006AA31D..006AA325
    vec[2] = 0;                                                                       // 006AA339
    (void)kotor_fastcall_0060ef10(c, nullptr, ld(box, 0x98C), ld(c, 4),
                                  static_cast<U>(reinterpret_cast<std::uintptr_t>(vec)),
                                  static_cast<U>(*at(box, 0x988)), 0, 0, 0);          // 006AA308..006AA341
}
// 006AA350 PerformUseGrenade.
void __thiscall TutorialBoxHandlers::PerformUseGrenade_006aa350(kotor_input_hover::Control*) {
    void* const box = this;
    void* const c = creature_by_id(ld(box, 0x984));                                   // 006AA350..006AA364
    void* const target = creature_by_id(ld(box, 0x988));                              // 006AA369..006AA37A
    unpause_once(box);                                                                // 006AA381..006AA3A0
    if (c == nullptr) return;                                                         // 006AA3AA
    (void)kotor_fastcall_006167d0(c, nullptr, ld(box, 0x98C), target);               // 006AA3AC..006AA3B6
}
// 006AA3D0 PerformSetMine: PerformUseItem([988h], 0, 7F000000h, creature+24h).
void __thiscall TutorialBoxHandlers::PerformSetMine_006aa3d0(kotor_input_hover::Control*) {
    void* const box = this;
    void* const c = creature_by_id(ld(box, 0x984));                                   // 006AA3D0..006AA3E3
    unpause_once(box);                                                                // 006AA3EA..006AA409
    if (c == nullptr) return;                                                         // 006AA413
    (void)kotor_fastcall_0060ee60(c, nullptr, ld(box, 0x988), 0, 0x7F000000u,
                                  static_cast<U>(reinterpret_cast<std::uintptr_t>(at(c, 0x24))));   // 006AA415..006AA429
}
// 006AA440 PerformPlayerFlourishWeapons.
void __thiscall TutorialBoxHandlers::PerformPlayerFlourishWeapons_006aa440(kotor_input_hover::Control*) {
    unpause_once(this);                                                               // 006AA443..006AA460
    kotor_input_required::player_flourish_weapons_005ede90(client(), nullptr);       // 006AA46A..006AA473
}
// 006AA480 PerformBash: door (when [98Ch] != 0) or placeable by id [988h].
void __thiscall TutorialBoxHandlers::PerformBash_006aa480(kotor_input_hover::Control*) {
    void* const box = this;
    void* const c = creature_by_id(ld(box, 0x984));                                   // 006AA480..006AA493
    unpause_once(box);                                                                // 006AA49A..006AA4B9
    if (c == nullptr) return;                                                         // 006AA4C3
    const U id = ld(box, 0x988);                                                      // 006AA4CD
    if (ld(box, 0x98C) != 0) {                                                        // 006AA4C5..006AA4D4
        void* const door = recovered_client_get_door_by_id_005ed600(client(), nullptr, id);   // 006AA4D6..006AA4DF
        if (door == nullptr) return;                                                  // 006AA4E4..006AA4E6
        kotor_fastcall_00683e90(door, nullptr, 0, c);                                 // 006AA4E8..006AA4ED
        return;
    }
    void* const plc = recovered_client_get_placeable_by_id_005ed5f0(client(), nullptr, id);    // 006AA4F7..006AA4FF
    if (plc == nullptr) return;                                                       // 006AA504..006AA506
    kotor_fastcall_006826a0(plc, nullptr, 0, c);                                      // 006AA508..006AA50D
}
// 006AA520 PerformAttack: target creature by id [988h] -> ActionMenuAttack(0, actor) on the target (ECX=target).
void __thiscall TutorialBoxHandlers::PerformAttack_006aa520(kotor_input_hover::Control*) {
    void* const box = this;
    void* const c = creature_by_id(ld(box, 0x984));                                   // 006AA520..006AA533
    unpause_once(box);                                                                // 006AA53A..006AA559
    if (c == nullptr) return;                                                         // 006AA563
    void* const target = creature_by_id(ld(box, 0x988));                              // 006AA565..006AA575
    if (target == nullptr) return;                                                    // 006AA57A..006AA57C
    kotor_fastcall_00616800(target, nullptr, 0, c);                                   // 006AA57E..006AA583
}
// 006AA590 PerformNonCombatXButton: the GetInGameGui result is discarded.
void __thiscall TutorialBoxHandlers::PerformNonCombatXButton_006aa590(kotor_input_hover::Control*) {
    unpause_once(this);                                                               // 006AA593..006AA5B0
    (void)kotor_input_required::get_in_game_gui_005ed690(client(), nullptr);         // 006AA5BA..006AA5C3
}
// 006AA5D0 PerformCombatYButton: main interface [gui+90h] -> OnCombatYButton.
void __thiscall TutorialBoxHandlers::PerformCombatYButton_006aa5d0(kotor_input_hover::Control*) {
    unpause_once(this);                                                               // 006AA5D3..006AA5F0
    void* const gui = kotor_input_required::get_in_game_gui_005ed690(client(), nullptr);   // 006AA5FA..006AA603
    kotor_fastcall_006880c0(lp(gui, 0x90), nullptr);                                  // 006AA608..006AA60E
}
}
