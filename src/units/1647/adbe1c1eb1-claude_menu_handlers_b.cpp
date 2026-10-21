// Lane W4 G15 successor (worker menus-b): in-game GUI event handlers of CSWGuiInGameAbilities,
// CSWGuiInGameInventory and CSWGuiInGameCharacter that were W4_GUI_TRAP_HANDLER stubs, plus the missing callees
// they reach: CSWGuiInGameInventory::UpdatePortraits (006B2DC0) and CSWGuiInGameCharacter::UpdatePortraits (006AEE30).
// Transcribed from the original swkotor.exe disassembly (capstone linear sweep, addresses cited per statement).
//
// The three Switch{Left,Right} pairs and the two OnChangeCharacter bodies are the same machine code per class with only
// the member offsets changed (verified by an offset-normalised diff of the listings); they are expressed once over a
// per-class offset layout.
#include "78d2073e39-gui_panels.hpp"
using namespace w4gui;

namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); }   // CServerExoApp::GetPartyTable
extern "C" {
extern const unsigned char borrowed_enter_color_007a23c0[12];                                   // [007A23C0] hilight colour
void __fastcall gui_load_border_params_set_fill_00414c00(void* params, void*, const void* ref, I32) noexcept(false); // SetFillImage
void* __fastcall client_get_player_creature_005ed540(void* app, void*);                          // CClientExoApp::GetPlayerCreature
U32 __fastcall kotor_fastcall_005eda50(void* app, void*, U32 id);                                // ServerToClientObjectId
void* __fastcall kotor_fastcall_005ed8b0(void* app, void*);                                     // CClientExoApp::GetSWParty
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, U32 index);                     // CSWParty::GetCharacter
void* __fastcall recovered_server_info_004aec90(void* server, void*);                            // CServerExoApp::GetServerInfo
U32 __fastcall kotor_fastcall_005636b0(void* table, void*, I32 index);                           // CSWPartyTable::GetIsNPCAvailable
U8 __fastcall kotor_fastcall_005636d0(void* table, void*);                                       // CSWPartyTable::GetNumNPCAvailable
U32 __fastcall recovered_npc_object_00564700(void* table, void*, I32 index, U32, U32);           // CSWPartyTable::GetNPCObject
void __fastcall kotor_fastcall_0040a140(void* manager, void*, U32 sound);                       // CSWGuiManager::PlayGuiSound
void* __cdecl yaw_pitch_roll_004acac0(void* out_quaternion, float yaw, float pitch, float roll); // YawPitchRoll (returns out)
void* __cdecl quaternion_rotate_004a9bb0(void* out_vector, const void* quaternion, const void* v); // operator*(Quaternion, Vector)
// 005EDF80 CClientExoApp::ChangeCharacterToNextLivingPartyMember(int,int,int) RET 0Ch: owned by claude_equip_handlers.cpp.
U32 __fastcall kotor_fastcall_005edf80(void* app, void*, U32, U32, U32);
// defined below
void __fastcall kotor_fastcall_006b2dc0(void* self, void*);
void __fastcall kotor_fastcall_006aee30(void* self, void*);
}

namespace {
// Per-class member offsets of the two change-character portrait buttons (A = BTN_CHANGE1, B = BTN_CHANGE2) and the
// signed byte index of the displayed party member (-1 = player). Each button: +44 flags, +58 slot index,
// +80 / +F4 portrait border params (the CResRef inside each params block is at +40).
struct Layout { U32 a, b, current, right; };
constexpr Layout abilities_layout = {0x3860, 0x3A24, 0x3F71, 0x3DAC};   // 006AE2B7 / 006AE2EC / 006ADF83 / BTN_CHARRIGHT
constexpr Layout inventory_layout = {0x16B0, 0x1874, 0x1DE0, 0x1BFC};
constexpr Layout character_layout = {0x4B2C, 0x4CF0, 0x59F0, 0x5078};
inline U32 slot(U32 btn) { return btn + 0x58; }
inline U32 fill1(U32 btn) { return btn + 0x80; }
inline U32 fill2(U32 btn) { return btn + 0xF4; }
inline U32 flags(U32 btn) { return btn + 0x44; }

void* party_table() { return lane_config_required::GetPartyTable_004aee70(server_app(), nullptr); }
// creature vtable +148h GetPortrait(CResRef* out, int) -> CResRef*; the result feeds SetFillImage(ref, keep).
void portrait_fill(void* self, U32 params, void* creature, I32 keep) {
    unsigned char ref[16];
    void* r = vcall<void*, void*, U32>(creature, 0x148, ref, 1);
    gui_load_border_params_set_fill_00414c00(at(self, params), nullptr, r, keep);
}
// Both portrait params of one button from one creature, keep = 0 (every call site pushes 0 / ebp == 0).
void portrait_pair(void* self, U32 btn, void* creature) {
    portrait_fill(self, fill1(btn), creature, 0);
    portrait_fill(self, fill2(btn), creature, 0);
}
std::int8_t current(void* self, const Layout& l) { return static_cast<std::int8_t>(lb(self, l.current)); }
// Creature of party-table NPC slot i when it is not the displayed member and is available, else null
// (GetIsNPCAvailable -> GetNPCObject(i, 0, 1) -> ServerToClientObjectId -> GetCreatureByGameObjectID).
void* npc_candidate(void* self, const Layout& l, I32 i) {
    if (i == current(self, l)) return nullptr;
    if (kotor_fastcall_005636b0(party_table(), nullptr, i) == 0) return nullptr;
    void* app = client_app();
    const U32 object = recovered_npc_object_00564700(party_table(), nullptr, i, 0, 1);
    return pt(frame_R4_005ed5b0(app, nullptr, kotor_fastcall_005eda50(app, nullptr, object)));
}
void set_bit1(void* self, U32 off, U32 on) { sw(self, off, (lw(self, off) & 0xFFFFFFFDu) | ((on & 1u) << 1)); }

// 006AC3E0 / 006B2FD0 / 006AF450 OnSwitchLeft(CSWGuiControl*) (thiscall, RET 4).
void switch_left(void* self, void* control, const Layout& l) {
    if (lw(control, 0x4C) == 0) return;                                                      // 006AC3E0..006AC3EF
    const U8 first = lb(self, slot(l.a));                                                 // 006AC3F6 bl
    U32 found = 0;                                                                           // ebp
    if (first == 0xFE) return;                                                               // 006AC3FF..006AC403
    gui_load_border_params_set_fill_00414c00(at(self, fill1(l.b)), nullptr, at(self, fill1(l.a) + 0x40), 0); // 006AC409..006AC417
    gui_load_border_params_set_fill_00414c00(at(self, fill2(l.b)), nullptr, at(self, fill2(l.a) + 0x40), 0); // 006AC41C..006AC42A
    sw(self, slot(l.b), lw(self, slot(l.a)));                                          // 006AC42F..006AC435
    const I32 start = static_cast<std::int8_t>(first);                                       // 006AC43B [esp+34]
    for (I32 i = start - 1; i >= 0; --i) {                                                   // 006AC43E..006AC4FA
        if (found != 0) return;                                                              // 006AC450..006AC452
        if (void* c = npc_candidate(self, l, i)) {                                           // 006AC458..006AC4B0
            portrait_pair(self, l.a, c);                                                     // 006AC4B2..006AC4E9
            sw(self, slot(l.a), static_cast<U32>(i)); found = 1;                          // 006AC4EE..006AC4F4
        }
    }
    if (found != 0) return;                                                                  // 006AC500..006AC502
    if (current(self, l) != -1 && static_cast<std::int8_t>(lb(self, slot(l.b))) != -1) { // 006AC50C..006AC51F
        portrait_fill(self, fill1(l.a), client_get_player_creature_005ed540(client_app(), nullptr), 0); // 006AC521..006AC549
        portrait_fill(self, fill2(l.a), client_get_player_creature_005ed540(client_app(), nullptr), 0); // 006AC54E..006AC576
        sw(self, slot(l.a), 0xFFFFFFFFu);                                                 // 006AC57D
        return;
    }
    for (I32 i = 8; i > start; --i) {                                                        // 006AC58B..006AC647
        if (found != 0) return;                                                              // 006AC598..006AC59A
        if (void* c = npc_candidate(self, l, i)) {                                           // 006AC5A0..006AC5F7
            portrait_pair(self, l.a, c);                                                     // 006AC5F9..006AC630
            sw(self, slot(l.a), static_cast<U32>(i)); found = 1;                          // 006AC635..006AC63B
        }
    }
}

// 006AC660 / 006B3250 / 006AF6D0 OnSwitchRight(CSWGuiControl*) (thiscall, RET 4).
void switch_right(void* self, void* control, const Layout& l) {
    if (lw(control, 0x4C) == 0) return;                                                      // 006AC660..006AC66F
    const U8 first = lb(self, slot(l.b));                                                 // 006AC676 bl
    U32 found = 0;                                                                           // ebp
    if (first == 0xFE) return;                                                               // 006AC67F..006AC683
    gui_load_border_params_set_fill_00414c00(at(self, fill1(l.a)), nullptr, at(self, fill1(l.b) + 0x40), 0); // 006AC689..006AC697
    gui_load_border_params_set_fill_00414c00(at(self, fill2(l.a)), nullptr, at(self, fill2(l.b) + 0x40), 0); // 006AC69C..006AC6AA
    sw(self, slot(l.a), lw(self, slot(l.b)));                                          // 006AC6AF..006AC6B5
    const I32 start = static_cast<std::int8_t>(first);                                       // 006AC6BB [esp+34]
    for (I32 i = start + 1; i < 9; ++i) {                                                    // 006AC6BE..006AC77D
        if (found != 0) return;                                                              // 006AC6D0..006AC6D2
        if (void* c = npc_candidate(self, l, i)) {                                           // 006AC6D8..006AC730
            portrait_pair(self, l.b, c);                                                     // 006AC732..006AC769
            sw(self, slot(l.b), static_cast<U32>(i)); found = 1;                          // 006AC76E..006AC774
        }
    }
    if (found != 0) return;                                                                  // 006AC783..006AC785
    // Unlike OnSwitchLeft, only the displayed member is tested here (no check of the other button's slot).
    if (current(self, l) != -1) {                                                            // 006AC78F..006AC79A
        portrait_fill(self, fill1(l.b), client_get_player_creature_005ed540(client_app(), nullptr), 0); // 006AC79C..006AC7C4
        portrait_fill(self, fill2(l.b), client_get_player_creature_005ed540(client_app(), nullptr), 0); // 006AC7C9..006AC7F1
        sw(self, slot(l.b), 0xFFFFFFFFu);                                                 // 006AC7F8
        return;
    }
    for (I32 i = 0; i < start; ++i) {                                                        // 006AC806..006AC8BF
        if (found != 0) return;                                                              // 006AC810..006AC812
        if (void* c = npc_candidate(self, l, i)) {                                           // 006AC818..006AC86F
            portrait_pair(self, l.b, c);                                                     // 006AC871..006AC8A8
            sw(self, slot(l.b), static_cast<U32>(i)); found = 1;                          // 006AC8AD..006AC8B3
        }
    }
}

// Shared head of OnChangeCharacter (006AF350 / 006B2EC0, same code at 006ADC80): returns true when the server-info
// party-select branch ran (portrait slots updated in place), false when the client path changed character.
// `changed` is the ChangeCharacterToNextLivingPartyMember result on the client path.
bool change_character_head(void* self, void* control, const Layout& l, U32& changed) {
    if ((lb(recovered_server_info_004aec90(server_app(), nullptr), 0x104) & 1u) != 0) {     // 006AF369..006AF37E
        const U8 index = lb(control, 0x58);                                                  // 006AF384 bl
        void* creature;
        if (index == 0xFF) {                                                                 // 006AF387..006AF38A
            creature = client_get_player_creature_005ed540(client_app(), nullptr);          // 006AF38C..006AF395
        } else {
            void* app = client_app();                                                        // 006AF39C..006AF3A4
            const U32 object = recovered_npc_object_00564700(party_table(), nullptr, static_cast<std::int8_t>(index), 0, 1); // 006AF3A7..006AF3B6
            creature = pt(frame_R4_005ed5b0(app, nullptr, kotor_fastcall_005eda50(app, nullptr, object))); // 006AF3BB..006AF3C6
        }
        if (creature != nullptr) {                                                           // 006AF3CB..006AF3CD
            if ((lw(self, flags(l.right)) & 2u) != 0) {                                      // 006AF3CF..006AF3D6
                if (control == at(self, l.a)) {                                              // 006AF3D8..006AF3E0
                    sw(self, slot(l.a), lw(self, slot(l.b)));                          // 006AF3E2..006AF3E8
                }
            } else {
                sw(self, slot(l.a), 0xFFFFFFFEu);                                         // 006AF3F6
            }
            sb(self, l.current, index);                                                      // 006AF3EE / 006AF400
        }
        return true;
    }
    const U32 step = control == at(self, l.b) ? 2u : 1u;                                     // 006AF35E / 006AF408..006AF412
    changed = kotor_fastcall_005edf80(client_app(), nullptr, 0, 0, step);                    // 006AF417..006AF424
    return false;
}

// 006B2DC0 / 006AEE30 shared tail: the client (non party-select) path showing party members 1 and 2.
void portraits_from_party(void* self, const Layout& l) {
    void* party = kotor_fastcall_005ed8b0(client_app(), nullptr);                            // 006AF24A..006AF253
    if (li(party, 0) > 1) {                                                                  // 006AF25A..006AF262
        sw(self, flags(l.a), lw(self, flags(l.a)) | 2u);                                     // 006AF264..006AF271
        portrait_fill(self, fill1(l.a), frame_GetCharacter_006346c0(party, nullptr, 1), 0);  // 006AF26D..006AF296
        portrait_fill(self, fill2(l.a), frame_GetCharacter_006346c0(party, nullptr, 1), 0);  // 006AF29B..006AF2BE
    } else {
        sw(self, flags(l.a), lw(self, flags(l.a)) & 0xFFFFFFFDu);                            // 006AF2C5
    }
    if (li(party, 0) > 2) {                                                                  // 006AF2CB..006AF2CE
        sw(self, flags(l.b), lw(self, flags(l.b)) | 2u);                                     // 006AF2D0
        portrait_fill(self, fill1(l.b), frame_GetCharacter_006346c0(party, nullptr, 2), 0);  // 006AF2D7..006AF2FA
        portrait_fill(self, fill2(l.b), frame_GetCharacter_006346c0(party, nullptr, 2), 0);  // 006AF2FF..006AF322
    } else {
        sw(self, flags(l.b), lw(self, flags(l.b)) & 0xFFFFFFFDu);                            // 006AF32F..006AF338
    }
}

// One candidate slot of CSWGuiInGameCharacter::UpdatePortraits (006AEE88..006AEF7D / 006AF0A0..006AF190).
void update_candidate(void* self, const Layout& l, I32 i, U32& first, U32& second) {
    void* c = npc_candidate(self, l, i);
    if (c == nullptr) return;
    if (first != 0) {
        portrait_pair(self, l.b, c); sw(self, slot(l.b), static_cast<U32>(i)); second = 1;   // 006AEEF4..006AEF34
    } else {
        portrait_pair(self, l.a, c); sw(self, slot(l.a), static_cast<U32>(i)); first = 1;    // 006AEF3B..006AEF7D
    }
}
void rotate_character(void* self, float yaw) {
    void* object = lp(self, 0x59E4);                                                         // 006AED97
    void* table = lp(object, 0);                                                             // 006AED9D (vtable read before the calls)
    unsigned char q[16]; unsigned char v[12];                                                // [esp+14] / [esp+08]
    void* rq = yaw_pitch_roll_004acac0(q, yaw, 0.0f, 0.0f);                                  // 006AED9F..006AEDB6
    void* rv = quaternion_rotate_004a9bb0(v, rq, at(object, 0x30));                          // 006AEDB9..006AEDC4
    reinterpret_cast<void(__thiscall*)(void*, void*)>(lp(table, 0x88))(lp(self, 0x59E4), rv); // 006AEDC4..006AEDCE
}
}  // namespace

extern "C" {
// 006B2DC0 CSWGuiInGameInventory::UpdatePortraits() (thiscall, RET 0).
void __fastcall kotor_fastcall_006b2dc0(void* self, void*) {
    portraits_from_party(self, inventory_layout);                                            // 006B2DC0..006B2EBD
}

// 006AEE30 CSWGuiInGameCharacter::UpdatePortraits() (thiscall, RET 0).
void __fastcall kotor_fastcall_006aee30(void* self, void*) {
    const Layout& l = character_layout;
    if ((lb(recovered_server_info_004aec90(server_app(), nullptr), 0x104) & 1u) == 0) {      // 006AEE3D..006AEE4D
        portraits_from_party(self, l);                                                       // 006AF24A..006AF344
        return;
    }
    U32 first = 0, second = 0;                                                               // [esp+10] / ebp
    U8 cur = lb(self, slot(l.a));                                                         // 006AEE53
    if (cur == 0xFE) cur = static_cast<U8>(lb(self, l.current) + 1);                         // 006AEE5B..006AEE69
    const I32 start = static_cast<std::int8_t>(cur);                                         // 006AEE6B [esp+14]
    for (I32 i = start; i < 9 && second == 0; ++i) update_candidate(self, l, i, first, second); // 006AEE6E..006AEF89
    bool scan_low = true;
    if (current(self, l) != -1) {                                                            // 006AEF8F..006AEF9A
        if (second != 0) scan_low = false;                                                   // 006AEFA0..006AEFA2
        else if (first == 0) {                                                               // 006AEFA8..006AF016
            portrait_fill(self, fill1(l.a), client_get_player_creature_005ed540(client_app(), nullptr), 0);
            portrait_fill(self, fill2(l.a), client_get_player_creature_005ed540(client_app(), nullptr), 0);
            sw(self, slot(l.a), 0xFFFFFFFFu); first = 1;
        } else {                                                                             // 006AF018..006AF07D
            portrait_fill(self, fill1(l.b), client_get_player_creature_005ed540(client_app(), nullptr), 0);
            portrait_fill(self, fill2(l.b), client_get_player_creature_005ed540(client_app(), nullptr), 0);
            sw(self, slot(l.b), 0xFFFFFFFFu); second = 1;
            scan_low = false;
        }
    } else if (second != 0) {                                                                // 006AF082..006AF084
        scan_low = false;
    }
    if (scan_low)
        for (I32 i = 0; i < start && second == 0; ++i) update_candidate(self, l, i, first, second); // 006AF08A..006AF19F
    set_bit1(self, flags(l.a), first);                                                       // 006AF1A5..006AF1BB
    set_bit1(self, flags(l.b), second);                                                      // 006AF1C1..006AF1D4
    set_bit1(self, 0x4EF8, kotor_fastcall_005636d0(party_table(), nullptr) > 2 ? 1u : 0u);   // 006AF1DA..006AF208 (BTN_CHARLEFT +44)
    set_bit1(self, flags(l.right), kotor_fastcall_005636d0(party_table(), nullptr) > 2 ? 1u : 0u); // 006AF20E..006AF23D
}

// ---- CSWGuiInGameAbilities
void __fastcall kotor_fastcall_006ac3e0(void* self, void*, void* control) { switch_left(self, control, abilities_layout); }
void __fastcall kotor_fastcall_006ac660(void* self, void*, void* control) { switch_right(self, control, abilities_layout); }

// 006AC8D0 CSWGuiInGameAbilities::SetButtonColor(CSWGuiControl*) (thiscall, RET 4; control unused): highlight the
// text of the button of the current ability mode ([CGuiInGame+BC0]: 0 skills, 1 powers, 2 feats).
void __fastcall kotor_fastcall_006ac8d0(void* self, void*, void*) {
    void* gui = pt(frame_R4_005ed690(client_app(), nullptr));                                // 006AC8D0..006AC8DB
    switch (lb(gui, 0xBC0)) {                                                                // 006AC8E0..006AC8F0
    case 0: gui_load_text_params_set_color_00414e10(at(self, 0x3084), nullptr, borrowed_enter_color_007a23c0); break; // 006AC91A..006AC925 (BTN_SKILLS)
    case 1: gui_load_text_params_set_color_00414e10(at(self, 0x2EC0), nullptr, borrowed_enter_color_007a23c0); break; // 006AC906..006AC911 (BTN_POWERS)
    case 2: gui_load_text_params_set_color_00414e10(at(self, 0x2CFC), nullptr, borrowed_enter_color_007a23c0); break; // 006AC8F2..006AC8FD (BTN_FEATS)
    default: break;                                                                          // 006AC8F0 -> 006AC92A
    }
}

// ---- CSWGuiInGameInventory
void __fastcall kotor_fastcall_006b2fd0(void* self, void*, void* control) { switch_left(self, control, inventory_layout); }
void __fastcall kotor_fastcall_006b3250(void* self, void*, void* control) { switch_right(self, control, inventory_layout); }

// 006B2EC0 CSWGuiInGameInventory::OnChangeCharacter(CSWGuiControl*) (thiscall, RET 4).
void __fastcall kotor_fastcall_006b2ec0(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                      // 006B2EC5..006B2ED3
    U32 changed = 0;
    if (!change_character_head(self, control, inventory_layout, changed)) {                  // 006B2ED9..006B2F94
        if (changed == 0) return;                                                            // 006B2F99..006B2F9B
        kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 1);                                 // 006B2F9D..006B2FA2 PlayGuiSound(1)
    }
    sw(self, 0x1DE4, lw(self, 0x1DE4) | 1u);                                                 // 006B2FA7..006B2FB2
    kotor_fastcall_006b2dc0(self, nullptr);                                                  // 006B2FB0..006B2FB8 UpdatePortraits
}

// ---- CSWGuiInGameCharacter
void __fastcall kotor_fastcall_006af450(void* self, void*, void* control) { switch_left(self, control, character_layout); }
void __fastcall kotor_fastcall_006af6d0(void* self, void*, void* control) { switch_right(self, control, character_layout); }

// 006AED90 / 006AEDE0 CSWGuiInGameCharacter::RotateCharacter{Left,Right}(CSWGuiControl*) (thiscall, RET 4; control
// unused): object [this+59E4] vtable +88h is called with YawPitchRoll(-/+10, 0, 0) * object+30h.
void __fastcall kotor_fastcall_006aed90(void* self, void*, void*) { rotate_character(self, bits_f(0xC1200000u)); }  // -10.0f
void __fastcall kotor_fastcall_006aede0(void* self, void*, void*) { rotate_character(self, bits_f(0x41200000u)); }  // +10.0f

// 006AF350 CSWGuiInGameCharacter::OnChangeCharacter(CSWGuiControl*) (thiscall, RET 4).
void __fastcall kotor_fastcall_006af350(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                      // 006AF355..006AF363
    U32 changed = 0;
    if (!change_character_head(self, control, character_layout, changed)) {                  // 006AF369..006AF424
        if (changed == 0) return;                                                            // 006AF429..006AF42B
        kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 1);                                 // 006AF42D..006AF432 PlayGuiSound(1)
    }
    // Original quirk kept: on the client path PlayGuiSound(1) runs twice (006AF432 falls through to 006AF43C).
    kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 1);                                    // 006AF437..006AF43C
    kotor_fastcall_006aee30(self, nullptr);                                                  // 006AF441..006AF443 UpdatePortraits
}
}  // extern "C"
