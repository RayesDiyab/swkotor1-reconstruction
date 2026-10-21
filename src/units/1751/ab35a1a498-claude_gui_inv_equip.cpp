// Lane W4 GUI successor (worker "gui-inv"), panel CSWGuiInGameEquip (construction / handlers: panels_equip.cpp,
// claude_equip_handlers.cpp, claude_equip_items*.cpp):
//   006B8E10 CSWGuiInGameEquip::OnPanelRemoved     thiscall() RET 0                 (slot trap replaced)
//   006BA3F0 CSWGuiInGameEquip::HandleInputEvent   thiscall(int event, int down) RET 8 (slot trap replaced)
//   006BB510 CSWGuiInGameEquip::Draw               thiscall(int) RET 4              (slot trap replaced)
//   006BB530 CSWGuiInGameEquip::OnPanelAdded       thiscall() RET 0                 (slot trap replaced)
// Callees with no rebuilt body, reconstructed here:
//   006B9970 CSWGuiInGameEquip::UpdateInventory()                                              RET 0
//   005A9E10 CSWSCreatureStats::GetTotalDamageRange(CSWSCreature*, CSWSItem*, int* min, int* max, int offhand,
//            int effects)                                                                      RET 18h
//   005A67D0 CSWSCreatureStats::GetWeaponSpecialization(CSWSItem*)                             RET 4
// Transcribed from the original swkotor.exe (read-only, capstone sweep); statement comments cite original PCs.
#include "0a92cb2116-claude_gui_inv.hpp"
using namespace gui_inv;

namespace menu_resources { struct Ref; }
namespace resource_ref_assignment {
menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*, void*, menu_resources::Ref* out, const menu_resources::Ref* source); // CResRef::operator=
}
namespace options_video { struct String; }
namespace client_movies { int __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*); } // operator!=(const char*)
namespace recovered_save_stats { unsigned short __cdecl maximum_hp_00647a80(void* stats); }        // CSWCCreatureStats::GetMaxHitPoints

extern "C" {
extern void* kotor_cell_rules_007a3a28;                                                          // [007A3A28] CSWRules*
extern const unsigned char kotor_m15d_color_007a23e4[12];                                        // [007A23E4]
// ---- existing providers (c24 map)
void* __fastcall c2da_default_ref_00405ed0(void* self, void*);                                   // CResRef::CResRef()
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, void* string);            // CResRef::CResRef(const CExoString&)
menu_resources::Ref* __fastcall kotor_fastcall_005556b0(void* item, void*, menu_resources::Ref* out); // CSWSItem::GetIcon
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, w4::I32 id); // CSWBaseItemArray::GetBaseItem
w4::U32 __fastcall recovered_stats_has_feat_005a6630(void* stats, void*, w4::U32 feat);           // CSWSCreatureStats::HasFeat
w4::U32 __fastcall kotor_m15e_slot010_zero_0063e7f0(void* self, void*);                           // 0063E7F0 return_zero
w4::U32 __fastcall kotor_fastcall_004f3fe0(void* creature, void*, w4::U32, w4::U32, w4::U32, w4::U32, w4::U32,
                                           w4::U32, w4::U32, w4::U32, w4::U32, w4::U32);      // GetTotalEffectBonus RET 28h
w4::U32 __fastcall recovered_attack_melee_005a7770(void* stats, void*, w4::U32 offhand, w4::U32 include, w4::U32, w4::U32); // GetMeleeAttackBonus
w4::U32 __fastcall recovered_attack_ranged_005a7b60(void* stats, void*, w4::U32 offhand, w4::U32 include, w4::U32); // GetRangedAttackBonus
w4::U32 __fastcall kotor_fastcall_005a60d0(void* stats, void*);                                   // GetBaseAttackBonus
w4::U32 __fastcall recovered_creature_armor_class_004ed1d0(void* creature, void*);                // CSWSCreature::GetArmorClass (AX)
w4::U32 __fastcall recovered_compare_item_00553cf0(void* item, void*, void* other);               // CSWSItem::CompareItem
// ---- equip panel (panels_equip.cpp, claude_equip_handlers.cpp, claude_equip_items*.cpp)
void __fastcall kotor_fastcall_006b5b60(void* self, void*, w4::U32 show);                         // UpdatePortraits
void __fastcall kotor_fastcall_006b7e30(void* self, void*);                                       // CloseDescription
void __fastcall kotor_fastcall_006b7590(void* self, void*, void* character);                      // SetCharacter
void* __fastcall kotor_fastcall_006b5600(void* self, void*, void* out, w4::I32 slot);             // GetIcon
w4::U32 __fastcall kotor_fastcall_005edf80(void* app, void*, w4::U32, w4::U32, w4::U32);          // ChangeCharacterToNextLivingPartyMember
void __fastcall kotor_fastcall_006b9470(void* self, void*, void* control);                        // OnEnterSlot
w4::I32 __fastcall kotor_fastcall_006b4c90(void* self, void*, w4::I32 mask);                      // GetSlotItemIdIndex
w4::U32 __fastcall kotor_fastcall_006b66a0(void* self, void*, w4::U32 id);                        // ItemExists
w4::U32 __fastcall kotor_fastcall_006b5760(void* self, void*, w4::U32 id, w4::U32 slot, w4::U32 clear);   // EquipItem(id)
w4::U32 __fastcall kotor_fastcall_006b5890(void* self, void*, void* copy, w4::U32 slot, w4::U32 clear);   // EquipItem(item)
void __fastcall kotor_fastcall_006b5910(void* self, void*, w4::U32 id, w4::U32 clear);            // UnequipItem
// ---- defined in this file
void __fastcall kotor_fastcall_006b8e10(void* self, void*);
void __fastcall kotor_fastcall_006ba3f0(void* self, void*, w4::U32 event, w4::U32 down);
void __fastcall kotor_fastcall_006bb510(void* self, void*, w4::U32 arg);
void __fastcall kotor_fastcall_006bb530(void* self, void*);
void __fastcall kotor_fastcall_006b9970(void* self, void*);
void __fastcall kotor_fastcall_005a9e10(void* stats, void*, void* creature, void* item, w4::I32* min, w4::I32* max,
                                        w4::U32 offhand, w4::U32 effects);
w4::U32 __fastcall kotor_fastcall_005a67d0(void* stats, void*, void* item);
}

namespace {
// .rdata 00756560: 9-entry equip slot table (stride 14h), column +0 = slot mask (ends at 00756614).
const U32 slot_masks[9] = {0x20, 0x10, 0x1, 0x80, 0x100, 0x2, 0x8, 0x200, 0x400};
inline void* base_item(void* item) { return kotor_m15b_item_get_base_item_005b4790(item, nullptr); }
inline void* server_creature(void* self) { return kotor_m15a_creature_get_server_creature_0060fb20(lp(self, 0x64), nullptr); }
inline void set_text(void* self, U32 off, Str& s) { gui_load_text_params_set_text_00415e00(at(self, off), nullptr, s.p()); }
inline void set_text(void* params, Str& s) { gui_load_text_params_set_text_00415e00(params, nullptr, s.p()); }
inline void set_color(void* params, const void* color) { gui_load_text_params_set_color_00414e10(params, nullptr, color); }
inline void* rules_base_items() { return lp(kotor_cell_rules_007a3a28, 0x34); }                  // [[007A3A28]+34h]
}

extern "C" {

// 005A67D0 CSWSCreatureStats::GetWeaponSpecialization(CSWSItem* item) RET 4: HasFeat(base item +B0h) when nonzero.
U32 __fastcall kotor_fastcall_005a67d0(void* stats, void*, void* item) {
    if (item == nullptr) return 0;                                                             // 005A67D1..005A67DA
    if (base_item(item) == nullptr) return 0;                                                  // 005A67DC..005A67E5
    const U16 feat = lh(base_item(item), 0xb0);                                                // 005A67E7..005A67EE
    if (feat == 0) return 0;                                                                   // 005A67F5..005A67F8
    return recovered_stats_has_feat_005a6630(stats, nullptr, feat);                            // 005A67FA..005A67FD
}

// 005A9E10 CSWSCreatureStats::GetTotalDamageRange(CSWSCreature* creature, CSWSItem* item, int* min, int* max,
// int offhand, int effects) RET 18h. Weapon: dice (+1Ch) .. dice*die (+1Dh) of the rules base item, plus the +EAh
// modifier byte for melee weapons (base item +1Ah == 0) and +2 for weapon specialization. Otherwise (no creature or no
// item): 1 .. 2/1 by creature size (+4F8h <= 2), with the modifier byte added when a creature was passed. Both results
// are clamped to >= 1. Quirk kept: return_zero (0063E7F0) is called twice on the creature (+24h) and its 0 result
// selects the 1/2 arm.
void __fastcall kotor_fastcall_005a9e10(void* stats, void*, void* creature, void* item, I32* min, I32* max,
                                        U32 offhand, U32 effects) {
    I32 low, high;                                                                             // edi / esi
    if (creature != nullptr && item != nullptr) {                                              // 005A9E10..005A9E24
        low = lb(kotor_m15b_base_item_array_get_base_item_005b31a0(rules_base_items(), nullptr, li(item, 0xc)), 0x1c); // 005A9E26..005A9E37
        high = static_cast<I32>(lb(kotor_m15b_base_item_array_get_base_item_005b31a0(rules_base_items(), nullptr, li(item, 0xc)), 0x1d)) * low; // 005A9E3B..005A9E53
        if (lb(base_item(item), 0x1a) == 0) {                                                  // 005A9E51..005A9E60
            const I32 modifier = static_cast<std::int8_t>(lb(stats, 0xea));                   // 005A9E62
            low += modifier; high += modifier;                                                 // 005A9E69..005A9E6B
        }
        if (kotor_fastcall_005a67d0(stats, nullptr, item) != 0) { low += 2; high += 2; }        // 005A9E6D..005A9E7C
    } else {
        void* owner = lp(stats, 0x24);
        (void)kotor_m15e_slot010_zero_0063e7f0(owner, nullptr);                                // 005A9E81..005A9E8E ([arg2] = +4F8h)
        const I32 size = li(lp(stats, 0x24), 0x4f8);                                           // 005A9E93..005A9E96
        low = 1;                                                                               // 005A9E9C
        U8 range;
        if (kotor_m15e_slot010_zero_0063e7f0(lp(stats, 0x24), nullptr) == 1)                  // 005A9E93..005A9EA8
            range = static_cast<U8>((size > 2 ? 1 : 0) * 2 + 4);                               // 005A9EAA..005A9EB2
        else
            range = static_cast<U8>((size <= 2 ? 1 : 0) + 1);                                  // 005A9EB8..005A9EC0
        high = range;                                                                          // 005A9EC1
        if (creature != nullptr) {                                                             // 005A9EC4..005A9ECA
            const I32 modifier = static_cast<std::int8_t>(lb(stats, 0xea));                   // 005A9ECC
            low = modifier + 1; high += modifier;                                              // 005A9ED3..005A9ED6
        }
    }
    if (effects != 0) {                                                                        // 005A9ED8..005A9EDE
        U32 extra = 0;                                                                         // [esp+14h] (creature argument slot)
        const I32 bonus = static_cast<I32>(kotor_fastcall_004f3fe0(lp(stats, 0x24), nullptr, 2, 0, ad(&extra), 0, 1, 0, 0,
                                                                   0xff, 0xff, offhand));     // 005A9EE0..005A9F0D (10 args)
        low += bonus + static_cast<I32>(extra);                                                // 005A9F12..005A9F19
        high += bonus;                                                                         // 005A9F1B
    }
    *min = low; *max = high;                                                                   // 005A9F1D..005A9F28
    if (*min < 1) *min = 1;                                                                    // 005A9F2A..005A9F32
    if (*max < 1) *max = 1;                                                                    // 005A9F38..005A9F3D
}

// 006B9970 CSWGuiInGameEquip::UpdateInventory() RET 0. Per slot: equipped item id cache +427Ch, slot icons
// (+10BCh + 140h*slot), weapon damage / attack labels (left 1B98h/1CD8h, right 1E18h/1F58h), then armour class
// (+2180h), vitality (+22C0h) and the compare-candidate marks (+42A4h slot mask, +42A8h item).
// Quirks kept: the item / server-item / id / two-hand / unarmed locals persist across slot iterations (a NULL creature
// reuses the previous slot's values; the first-iteration id is not initialized in the original and is 0 here); a
// two-handed main item (base +8 == 3) steps the slot index back once so slot 0 / 1 is processed twice; the vitality
// string is formatted from the server creature slots +9Ch / +98h.
void __fastcall kotor_fastcall_006b9970(void* self, void*) {
    menu_resources::Ref icon;                                                                  // [esp+B0h]
    c2da_default_ref_00405ed0(&icon, nullptr);                                                 // 006B9999..006B99A8
    void* server_item = nullptr;                                                               // [esp+1Ch]
    void* client_item = nullptr;                                                               // [esp+40h]
    U32 two_hand_done = 0;                                                                     // [esp+3Ch]
    U32 unarmed_seen = 0;                                                                      // [esp+24h]
    U32 equipped = 0;                                                                          // [esp+44h]
    {
        Str name;                                                                              // [esp+68h]
        void* text = kotor_fastcall_006b5600(self, nullptr, name.s, 0); name.live = true;      // 006B99AD..006B99BD GetIcon(0)
        menu_resources::Ref ref;                                                               // [esp+A0h]
        async_ref_construct_string_00406d60(&ref, nullptr, text);                              // 006B99C2..006B99D1
        gui_load_border_params_set_fill_00414c00(at(self, 0x10bc), nullptr, &ref, 0);          // 006B99D6..006B99E5
    }                                                                                          // 006B99EA..006B99F9
    sw(self, 0x10c8, 0x3f800000u);                                                             // 006B9A00 (1.0f)
    I32 slot = 0;                                                                              // ebx / [esp+20h]
    do {
        U32 main_hand = 0;                                                                     // ebp (006B9A10)
        bool have_item = false;
        void* creature = lp(self, 0x64);                                                       // 006B9A12..006B9A17
        if (creature != nullptr) {
            equipped = kotor_m15a_creature_get_equipped_item_id_0060ca80(creature, nullptr, slot_masks[slot]); // 006B9A19..006B9A29
            client_item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, equipped); // 006B9A2B..006B9A3F
            if (client_item != nullptr) {
                server_item = kotor_m15closure_00646c90(client_item, nullptr);                // 006B9A49..006B9A50
                have_item = server_item != nullptr;                                            // 006B9A5A..006B9A6C
            }
        } else {
            have_item = client_item != nullptr && server_item != nullptr;                      // 006B9A56..006B9A6C (stale)
        }
        bool check_enter = true;                                                               // -> 006BA02A
        if (have_item) {
            sw(self, 0x427c + static_cast<U32>(slot) * 4u, equipped);                          // 006B9A7A
            {
                menu_resources::Ref made, copy;                                                // [esp+D0h] / [esp+E0h]
                resource_ref_assignment::assign_00406120(&icon, nullptr, &copy,
                    kotor_fastcall_005556b0(server_item, nullptr, &made));                     // 006B9A72..006B9A96
            }
            const U32 two_hand = lb(base_item(server_item), 8) == 3 ? 1u : 0u;                 // 006B9A9B..006B9AB0 [esp+30h]
            void* label_a = nullptr; void* label_b = nullptr;                                  // [esp+2Ch] / [esp+14h]
            bool labels = true;
            if (slot == 0) { label_a = at(self, 0x1b98); label_b = at(self, 0x1cd8); main_hand = 1; } // 006B9ACF..006B9AE3
            else if (slot != 1) labels = false;                                                // 006B9AC7..006B9ACA -> 006B9D20
            if (labels) {
                if (two_hand != 0 && two_hand_done == 0) {                                     // 006B9AE8..006B9AF6
                    label_a = at(self, 0x1b98); label_b = at(self, 0x1cd8); main_hand = 1;     // 006B9AF8..006B9B0E
                    --slot; two_hand_done = 1;                                                 // 006B9B09..006B9B16
                }
                if (label_a == nullptr) { label_a = at(self, 0x1e18); label_b = at(self, 0x1f58); main_hand = 0; } // 006B9B1A..006B9B36
                void* fighter = server_creature(self);                                         // 006B9B38..006B9B44 edi
                void* stats = lp(fighter, 0xa74);
                Str damage; damage.empty();                                                    // 006B9B40..006B9B46 [esp+70h]
                Str attack; attack.empty();                                                    // 006B9B4B..006B9B5A [esp+60h]
                I32 low = 0, high = 0;                                                         // [esp+34h] / [esp+38h]
                U32 better = 0;                                                                // [esp+28h]
                kotor_fastcall_005a9e10(stats, nullptr, fighter, server_item, &low, &high, main_hand, 1); // 006B9B5F..006B9B8E
                if (low != 0) frame_LogFormat_005e5680(damage.s, "%d-%d", low, high);          // 006B9B93..006B9BAB (00756A10)
                I32 low_base = 0, high_base = 0;                                               // [esp+50h] / [esp+54h]
                kotor_fastcall_005a9e10(stats, nullptr, fighter, server_item, &low_base, &high_base, main_hand, 0); // 006B9BB3..006B9BD5
                if (low > low_base || high > high_base) better = 1;                            // 006B9BDA..006B9BEE
                if (slot == 1) {                                                               // 006B9BF6..006B9BF9
                    if (lb(base_item(server_item), 0x1b) == 4 || lb(base_item(server_item), 8) == 3) { // 006B9BFB..006B9C17
                        gui_load_border_params_set_fill_00414c00(at(self, 0x10bc), nullptr, &icon, 0); // 006B9C19..006B9C29
                        sw(self, 0x10c8, 0x3e800000u);                                         // 006B9C2E (0.25f)
                    }
                }
                void* damage_text = at(label_a, 0xe8);                                         // 006B9C38..006B9C40
                set_text(damage_text, damage);                                                 // 006B9C46..006B9C49
                set_color(damage_text, better != 0 ? static_cast<const void*>(kotor_m15d_color_007a23e4)
                                                   : static_cast<const void*>(borrowed_exit_color_007a23b4)); // 006B9C4E..006B9C64
                I32 bonus;
                if (lb(base_item(server_item), 0x1a) != 0)                                     // 006B9C69..006B9C7D
                    bonus = static_cast<I32>(recovered_attack_ranged_005a7b60(stats, nullptr, main_hand, 1, 0)); // 006B9C7F..006B9C84
                else
                    bonus = static_cast<I32>(recovered_attack_melee_005a7770(stats, nullptr, main_hand, 1, 0, two_hand)); // 006B9C8B..006B9C95
                frame_LogFormat_005e5680(attack.s, bonus > 0 ? "+%d" : "%d", bonus);           // 006B9C9A..006B9CB7 (00748EC0 / 0073D720)
                void* attack_text = at(label_b, 0xe8);                                         // 006B9CBC..006B9CC7
                set_text(attack_text, attack);                                                 // 006B9CC3..006B9CD0
                const I32 base_bonus = static_cast<I32>(kotor_fastcall_005a60d0(stats, nullptr)); // 006B9CD5..006B9CDB
                set_color(attack_text, bonus > base_bonus ? static_cast<const void*>(kotor_m15d_color_007a23e4)
                                                          : static_cast<const void*>(borrowed_exit_color_007a23b4)); // 006B9CE0..006B9CF2
            }                                                                                  // 006B9CF7..006B9D17 ~attack, ~damage
            if (two_hand == 0 || slot == 1)                                                    // 006B9D20..006B9D2B
                gui_load_border_params_set_fill_00414c00(at(self, 0x10bc + static_cast<U32>(slot) * 0x140u), nullptr, &icon, 0); // 006B9D31..006B9D48
        } else {
            if (slot == 1) {                                                                   // 006B9D52..006B9D55
                main_hand = 1; unarmed_seen = 1;                                               // 006B9D5E..006B9D63
                void* fighter = server_creature(self);                                         // 006B9D5B..006B9D6C edi
                if (fighter != nullptr) {                                                      // 006B9D70..006B9D72
                    void* stats = lp(fighter, 0xa74);
                    Str damage; damage.empty();                                                // 006B9D78..006B9D84 [esp+78h]
                    I32 low = 0, high = 0;                                                     // [esp+28h] / [esp+20h]
                    kotor_fastcall_005a9e10(stats, nullptr, fighter, nullptr, &low, &high, 0, 1); // 006B9D89..006B9DAA
                    if (low != 0) frame_LogFormat_005e5680(damage.s, "%d-%d", low, high);      // 006B9DAF..006B9DCA
                    set_text(self, 0x1f00, damage);                                            // 006B9DD2..006B9DDD
                    Str attack; attack.empty();                                                // 006B9DE2..006B9DF9 [esp+58h]
                    const I32 bonus = static_cast<I32>(recovered_attack_melee_005a7770(stats, nullptr, 0, 1, 0, 0)); // 006B9DEB..006B9E01
                    frame_LogFormat_005e5680(attack.s, bonus > 0 ? "+%d" : "%d", bonus);       // 006B9E06..006B9E21
                    set_text(self, 0x2040, attack);                                            // 006B9E29..006B9E34
                }                                                                              // 006B9E39..006B9E59 ~attack, ~damage
            }
            if (lw(self, 0x427c + static_cast<U32>(slot) * 4u) == 0x7f000000u && (two_hand_done != 0 || unarmed_seen != 0)) { // 006B9E5E..006B9E80
                check_enter = main_hand != 0;                                                  // 006BA026..006BA028
            } else {
                sw(self, 0x427c + static_cast<U32>(slot) * 4u, 0x7f000000u);                   // 006B9E86
                if (slot == 0 || slot == 1) {                                                  // 006B9E8D..006B9E99
                    const U32 first = slot == 0 ? 0x1c80u : 0x1f00u, second = slot == 0 ? 0x1dc0u : 0x2040u;
                    { Str blank(""); set_text(self, first, blank); }                           // 006B9F2F..006B9F71 / 006B9E9F..006B9EE1 (0073D71C)
                    set_color(at(self, first), borrowed_exit_color_007a23b4);                  // 006B9F76..006B9F7D / 006B9EE6..006B9EED
                    { Str blank(""); set_text(self, second, blank); }                          // 006B9F82..006B9FB8 / 006B9EF2..006B9F2A
                    set_color(at(self, second), borrowed_exit_color_007a23b4);                 // 006B9FBD..006B9FC4
                }
                Str name;                                                                      // [esp+A0h]
                void* text = kotor_fastcall_006b5600(self, nullptr, name.s, slot); name.live = true; // 006B9FC9..006B9FD4 GetIcon(slot)
                menu_resources::Ref ref;                                                       // [esp+C0h]
                async_ref_construct_string_00406d60(&ref, nullptr, text);                      // 006B9FD9..006B9FEC
                gui_load_border_params_set_fill_00414c00(at(self, 0x10bc + static_cast<U32>(slot) * 0x140u), nullptr, &ref, 0); // 006B9FF1..006BA008
            }                                                                                  // 006BA00D..006BA01F ~name
        }
        if (check_enter) {                                                                     // 006BA02A..006BA03B
            void* button = at(self, 0x68 + static_cast<U32>(slot) * 0x1c4u);
            if (lp(self, 0x1c) == button) kotor_fastcall_006b9470(self, nullptr, button);      // 006BA03D..006BA040 OnEnterSlot
        }
        ++slot;                                                                                // 006BA045..006BA04D
    } while (slot < 9);
    Str armour;                                                                                // [esp+98h]
    kotor_fastcall_005e5bc0(armour.s, nullptr,
        static_cast<std::int16_t>(recovered_creature_armor_class_004ed1d0(server_creature(self), nullptr))); armour.live = true; // 006BA053..006BA06D
    void* const armour_text = at(self, 0x2180);                                                // 006BA079 [esp+24h]
    set_text(armour_text, armour);                                                             // 006BA072..006BA08F
    Str vitality; vitality.empty();                                                            // 006BA094..006BA0A0 [esp+48h]
    void* const first = server_creature(self);                                                 // 006BA09D..006BA0A8 edi
    void* const second = server_creature(self);                                                // 006BA0AD..006BA0B2 ebp
    const I32 a = static_cast<std::int16_t>(vcall<U32, U32>(first, 0x98, 1));                 // 006BA0B7..006BA0C8
    const I32 b = static_cast<std::int16_t>(vcall<U32, U32>(second, 0x9c, 0));                // 006BA0C5..006BA0D6
    frame_LogFormat_005e5680(vitality.s, "%d/%d", b, a);                                       // 006BA0D9..006BA0E4 (0075616C)
    void* const vitality_text = at(self, 0x22c0);                                              // 006BA0EC [esp+44h]
    set_text(vitality_text, vitality);                                                         // 006BA0F2..006BA0FD
    void* const vitality_label = at(self, 0x22a8);                                             // 006BA108
    const I32 width = static_cast<I32>(vcall<U32>(vitality_label, 0x08));                     // 006BA102..006BA110
    if (width > static_cast<I32>(gui_load_text_font_height_00414ee0(vitality_label, nullptr))) { // 006BA113..006BA11E
        void* stats = lp(lp(self, 0x64), 0x2f8);                                               // 006BA120..006BA123
        const I32 current = static_cast<std::int16_t>(lh(stats, 0x4c));                       // 006BA129
        const I32 maximum = static_cast<std::int16_t>(recovered_save_stats::maximum_hp_00647a80(stats)); // 006BA12D..006BA132
        frame_LogFormat_005e5680(vitality.s, "%d/ %d", current, maximum);                      // 006BA135..006BA144 (0075645C)
        set_text(vitality_text, vitality);                                                     // 006BA14C..006BA153
    }
    if (lw(self, 0x42a4) != 0) {                                                               // 006BA158..006BA160
        bool clear = true;
        const U32 candidate = lw(self, 0x42a8);                                                // 006BA169 ebx
        U32 differs = candidate != kotor_m15a_creature_get_equipped_item_id_0060ca80(lp(self, 0x64), nullptr, lw(self, 0x42a4)) ? 1u : 0u; // 006BA166..006BA17C
        if (differs != 0) {                                                                    // 006BA17E..006BA180
            void* server = server_app();                                                       // 006BA186..006BA18C
            void* candidate_item = recovered_item_lookup_004ae760(server, nullptr, kotor_fastcall_004aea30(server, nullptr, candidate)); // 006BA18F..006BA1A5
            void* server2 = server_app();                                                      // 006BA1A7..006BA1AC
            const U32 current_id = kotor_m15a_creature_get_equipped_item_id_0060ca80(lp(self, 0x64), nullptr, lw(self, 0x42a4)); // 006BA19F..006BA1B3
            void* current_item = recovered_item_lookup_004ae760(server2, nullptr, kotor_fastcall_004aea30(server2, nullptr, current_id)); // 006BA1B8..006BA1C3
            bool proceed;
            if (current_item != nullptr) {                                                     // 006BA1C8..006BA1CA
                proceed = candidate_item != nullptr;                                           // 006BA1CC..006BA1CE
                if (proceed) differs = recovered_compare_item_00553cf0(current_item, nullptr, candidate_item) == 0 ? 1u : 0u; // 006BA1D4..006BA1E2
            } else {
                proceed = candidate_item != nullptr;                                           // 006BA1E5..006BA1E7
            }
            if (proceed && differs != 0) {                                                     // 006BA1ED..006BA1EF
                clear = false;
                Str dash("--");                                                                // 006BA1F5..006BA1FE (00756A0C)
                const I32 mask = li(self, 0x42a4);                                             // 006BA203
                auto not_blank = [&]() {                                                       // 006BA254..006BA268 / 006BA31C..006BA330 (0073D71C)
                    return client_movies::StringNotEqual_005e5390(static_cast<options_video::String*>(at(self, 0x1c80)), nullptr, "") != 0;
                };
                if (mask == 0x200 || mask == 0x80 || mask == 0x100 || mask == 0x400 || mask == 1 || mask == 8) { // 006BA2C8..006BA2FC / table 006BA3C8 index 0
                    set_text(self, 0x1f00, dash); set_text(self, 0x2040, dash);                // 006BA2FC..006BA317
                    if (not_blank()) { set_text(self, 0x1c80, dash); set_text(self, 0x1dc0, dash); } // 006BA332..006BA349
                    set_text(armour_text, dash);                                               // 006BA34E..006BA357
                    set_text(vitality_text, dash);                                             // 006BA35C..006BA365
                } else if (mask == 0x20) {                                                     // 006BA21A -> 006BA298
                    set_text(self, 0x2040, dash); set_text(self, 0x1c80, dash);                // 006BA298..006BA2B3
                    set_text(self, 0x1dc0, dash);                                              // 006BA2B8..006BA2C3 -> 006BA365
                } else if (mask == 2) {                                                        // table index 1 -> 006BA28A
                    set_text(armour_text, dash);                                               // 006BA28A..006BA293 -> 006BA365
                } else if (mask == 0x10) {                                                     // table index 2 -> 006BA234
                    set_text(self, 0x1f00, dash); set_text(self, 0x2040, dash);                // 006BA234..006BA24F
                    if (not_blank()) { set_text(self, 0x1c80, dash); set_text(self, 0x1dc0, dash); } // 006BA26E..006BA285
                }                                                                              // others: table index 3 / 006BA2DF / 006BA2FA
            }                                                                                  // 006BA36A..006BA376 / 006BA2DF..006BA2EB ~dash
        }
        if (clear) sw(self, 0x42a4, 0);                                                        // 006BA37D
    }
}                                                                                              // 006BA387..006BA3AA ~vitality, ~armour

// 006BB510 CSWGuiInGameEquip::Draw(int) RET 4.
void __fastcall kotor_fastcall_006bb510(void* self, void*, U32 arg) {
    kotor_fastcall_006b9970(self, nullptr);                                                    // 006BB511..006BB513 UpdateInventory
    kotor_panel_draw_0040B760(self, nullptr, arg);                                             // 006BB518..006BB51F CSWGuiPanel::Draw
}

// 006BB530 CSWGuiInGameEquip::OnPanelAdded() RET 0.
void __fastcall kotor_fastcall_006bb530(void* self, void*) {
    sb(self, 0x4274, 0xff);                                                                    // 006BB538
    sw(self, 0x3a78, 0xFFFFFFFEu); sw(self, 0x3c3c, 0xFFFFFFFEu);                              // 006BB533..006BB545
    kotor_fastcall_006b7590(self, nullptr,
        kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr));     // 006BB54B..006BB562 SetCharacter
    kotor_fastcall_006b5b60(self, nullptr, 1);                                                 // 006BB567..006BB56B UpdatePortraits(1)
    kotor_fastcall_006b9470(self, nullptr, lp(self, 0x1c));                                    // 006BB570..006BB576 OnEnterSlot
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 006BB57B..006BB57E
}

// 006B8E10 CSWGuiInGameEquip::OnPanelRemoved() RET 0.
void __fastcall kotor_fastcall_006b8e10(void* self, void*) {
    if ((lb(self, 0x4270) & 1u) != 0) kotor_fastcall_006b7e30(self, nullptr);                  // 006B8E14..006B8E1E CloseDescription
    gui_load_listbox_clear_items_00419a90(at(self, 0x30d8), nullptr);                          // 006B8E23..006B8E29
    for (I32 i = 0; i < li(self, 0x1b90); ++i) {                                               // 006B8E2E..006B8E65
        if (void* entry = lp(lp(self, 0x1b8c), static_cast<U32>(i) * 4u)) vcall<void*, U32>(entry, 0x00, 1);
        sw(lp(self, 0x1b8c), static_cast<U32>(i) * 4u, 0);
    }
    sw(self, 0x1b90, 0);                                                                       // 006B8E6F
    if (lp(self, 0x1b8c) != nullptr) {                                                         // 006B8E67..006B8E75
        live_free(lp(self, 0x1b8c));                                                           // 006B8E77..006B8E7D
        sw(self, 0x1b8c, 0); sw(self, 0x1b94, 0);                                              // 006B8E80..006B8E86
    }
    sw(self, 0x1b90, 0);                                                                       // 006B8E8C
    sw(self, 0x42a4, 0); sw(self, 0x42a8, 0x7f000000u);                                        // 006B8E93..006B8E99
    gui_load_panel_on_removed_0040c170(self, nullptr);                                         // 006B8EA3..006B8EA7
}

// 006BA3F0 CSWGuiInGameEquip::HandleInputEvent(int event, int down) RET 8. Index table 006BA768 (event - 28h), jump
// table 006BA750: 28h/2Eh/DFh -> 006BA41F, 2Dh -> 006BA581, 39h -> 006BA71E, 3Ah -> 006BA700, CEh -> 006BA5C3.
void __fastcall kotor_fastcall_006ba3f0(void* self, void*, U32 event, U32 down) {
    auto describing = [&]() { return (lb(self, 0x4270) & 1u) != 0; };                         // [this+4270h] bit 0
    auto leave = [&]() {                                                                       // 006BA54E..006BA577 / 006BA58E..006BA5B9
        if (frame_R4_0062cba0(ingame_gui(), nullptr, 0) != 0) kotor_fastcall_005eda60(client_app(), nullptr, 0, 1);
    };
    auto scroll = [&](U32 code) { vcall<void, U32, U32>(at(self, 0x33b8), 0x3c, code, 1); };   // LB_DESC [vtbl+3Ch]
    if (down != 0) {                                                                           // 006BA3F0..006BA3F9
        switch (event) {
        case 0x28: case 0x2e: case 0xdf:                                                       // 006BA41F
            if (!describing()) { leave(); break; }                                             // 006BA41F..006BA428 -> 006BA54E
            play_sound(self, 0);                                                               // 006BA42E..006BA431
            if (lw(self, 0x42ac) != 0) {                                                       // 006BA436..006BA43E
                const U32 chosen = lw(self, 0x42ac);
                const I32 index = kotor_fastcall_006b4c90(self, nullptr, li(self, 0x4278));    // 006BA444..006BA44D
                const U32 current = lw(self, 0x427c + static_cast<U32>(index) * 4u);           // 006BA452
                if (chosen != current) {                                                       // 006BA459..006BA45B
                    if (chosen == 0x7f000000u) {                                               // 006BA461..006BA467
                        kotor_fastcall_006b5910(self, nullptr, current, 1);                    // 006BA4A9..006BA4AC UnequipItem
                    } else if (kotor_fastcall_006b66a0(self, nullptr, chosen) != 0) {          // 006BA469..006BA471
                        kotor_fastcall_006b5760(self, nullptr, lw(self, 0x42ac), lw(self, 0x4278), 1); // 006BA473..006BA485
                    } else if (void* copy = lp(self, 0x42b0)) {                                // 006BA48C..006BA494
                        kotor_fastcall_006b5890(self, nullptr, copy, lw(self, 0x4278), 1);     // 006BA496..006BA4A2
                    }
                    if (lw(self, 0x4278) == 0x10 && lw(self, 0x42b4) != 0) {                   // 006BA4B1..006BA4C2
                        if (kotor_fastcall_006b66a0(self, nullptr, lw(self, 0x42b4)) != 0)     // 006BA4C4..006BA4CE
                            kotor_fastcall_006b5760(self, nullptr, lw(self, 0x42b4), 0x20, 0); // 006BA4D0..006BA4DD
                        else if (void* copy = lp(self, 0x42b8))                                // 006BA4E4..006BA4EC
                            kotor_fastcall_006b5890(self, nullptr, copy, 0x20, 0);             // 006BA4EE..006BA4F5
                    }
                }
            }
            sw(self, 0x42ac, 0);                                                               // 006BA502
            if (void* a = lp(self, 0x42b0)) { vcall<void*, U32>(a, 0x00, 1); sw(self, 0x42b0, 0); } // 006BA4FA..006BA514
            sw(self, 0x42b4, 0);                                                               // 006BA526
            if (void* b = lp(self, 0x42b8)) { vcall<void*, U32>(b, 0x00, 1); sw(self, 0x42b8, 0); } // 006BA51E..006BA538
            kotor_fastcall_006b7e30(self, nullptr);                                            // 006BA542..006BA544 CloseDescription
            break;
        case 0x2d:                                                                             // 006BA581
            if (!describing()) leave();                                                        // 006BA581..006BA5B9
            break;
        case 0x39:                                                                             // 006BA71E
            if (describing()) scroll(0x31);                                                    // 006BA71E..006BA737
            break;
        case 0x3a:                                                                             // 006BA700
            if (describing()) scroll(0x32);                                                    // 006BA700..006BA719
            break;
        case 0xce: {                                                                           // 006BA5C3
            if (describing()) { scroll(0x32); break; }                                         // 006BA5C3..006BA5CA -> 006BA709
            play_sound(self, 1);                                                               // 006BA5D0..006BA5D8
            bool chosen = false;                                                               // 006BA5FF ebp
            if ((lb(recovered_server_info_004aec90(server_app(), nullptr), 0x104) & 1u) != 0) { // 006BA5DD..006BA5F2
                for (I32 npc = static_cast<std::int8_t>(lb(self, 0x4274)) + 1; npc < 9; ++npc) { // 006BA5F8..006BA66F
                    if (chosen) break;                                                         // 006BA607..006BA609
                    if (kotor_fastcall_005636b0(party_table(), nullptr, npc) == 0) continue;    // 006BA60F..006BA627
                    void* app = client_app();                                                  // 006BA629..006BA631 edi
                    const U32 object = recovered_npc_object_00564700(party_table(), nullptr, npc, 0, 1); // 006BA634..006BA63F
                    void* character = pt(frame_R4_005ed5b0(app, nullptr, kotor_fastcall_005eda50(app, nullptr, object))); // 006BA644..006BA64F
                    if (character == nullptr) continue;                                        // 006BA654..006BA656
                    kotor_fastcall_006b7590(self, nullptr, character);                         // 006BA658..006BA65B SetCharacter
                    sb(self, 0x4274, static_cast<U8>(npc));                                    // 006BA660
                    chosen = true;                                                             // 006BA666
                }
                if (!chosen) {
                    sb(self, 0x4274, 0xff);                                                    // 006BA675
                    kotor_fastcall_006b7590(self, nullptr,
                        kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr)); // 006BA67C..006BA6B2
                }
            } else {
                kotor_fastcall_005edf80(client_app(), nullptr, 0, 0, 1);                       // 006BA686..006BA695
                kotor_fastcall_006b7590(self, nullptr,
                    kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr)); // 006BA69A..006BA6B2
            }
            for (U32 i = 0; i < 9; ++i) {                                                      // 006BA6B7..006BA6D3
                if (slot_masks[i] != lw(self, 0x4278)) continue;
                vcall<void, void*, U32>(self, 0x08, at(self, 0x68 + i * 0x1c4u), 0);           // 006BA6D5..006BA6E6
                break;
            }
            kotor_fastcall_006b9470(self, nullptr, lp(self, 0x1c));                            // 006BA6E9..006BA6EF OnEnterSlot
            kotor_fastcall_006b5b60(self, nullptr, 1);                                         // 006BA6F4..006BA6F8 UpdatePortraits(1)
            if (describing()) scroll(0x32);                                                    // 006BA700..006BA719
            break;
        }
        default:
            break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, down);                          // 006BA73A..006BA746
}

}  // extern "C"
