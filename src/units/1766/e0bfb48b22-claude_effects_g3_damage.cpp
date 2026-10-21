// Effects group 3 (effects-g3), damage part: the Endar combat path of the server effect dispatcher
// (0886 NativeEffectDispatch -> 0854 ai_resolve):
//   004DFA40 CSWSEffectListHandler::OnApplyDamage(CSWSObject*, CGameEffect*, int load_game)  thiscall RET 0Ch, EAX 1
//   004E0750 CSWSEffectListHandler::OnApplyHeal  (CSWSObject*, CGameEffect*, int load_game)  thiscall RET 0Ch, EAX 1
// plus every callee of theirs without a callable provider in the c27 candidate map:
//   004F3830 CSWSCreature::DoDamage(int damage, int no_floaty)              RET 8
//   004CCF80 CSWSObject::DoDamage(int damage)                               RET 4  (only a file-local 0963 body exists)
//   005B7D10 CSWSCreature::ResolveDamageShields(CSWSCreature* attacker)     RET 4
//   004EC400 CSWSCreature::BroadcastDamageDataToParty(CSWCCMessageData*, int) RET 8
//   004CCD30 CSWSObject::DeleteCurrentAIAction()                            RET 0
//   004EFEF0 CSWSCreature::SetAmbientAnimationState(unsigned char)         RET 4  (shared with the death part)
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are original instructions.
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision -> double
// intermediates, float exactly where the original stores a dword; live_operator_new / live_free only.
// The original keeps EH frames only for the CExoString locals / operator-new temporaries; nothing on these paths
// throws in the game, and no catch is reproduced (same as the other W5 combat files).
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"

using namespace w5;

namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction {   // 005E9C80 CExoLinkedListInternal::RemoveHead [0091-ini-list-cleanup]
void* __fastcall remove_head_005e9c80(kotor_list::Internal* list, void*);
}
namespace w2 { std::uint32_t ftol2_006fae8c(double value); }                       // 006FAE8C __ftol2 low dword [w2_ftol_native]
namespace frame_control { extern unsigned message_manager_007a3a00; }               // .data 007A3A00 g_pVirtualMachine [0456]

extern "C" {
// ---------------- imports: existing providers (c27 candidate map; object in brackets) ----------------
extern void* kotor_cell_rules_007a3a28;                                                          // [007A3A28] CSWRules*
extern unsigned recovered_debug_cell_008327e4;                                                   // .bss 008327E4 debug-combat flag [w1 b7]
extern U recovered_effect_rank_scratch_008327e0;                                                 // .bss 008327E0 fistp scratch [0886]
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);                    // CGameEffect::GetInteger [w1 b7]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);        // CGameEffect::SetInteger [0886]
void* __fastcall recovered_effect_copy_string_00503700(void* effect, void*, void* out, U index); // CopyStringFromList [w2 save]
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);               // CGameEffect::CGameEffect [0886]
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);                    // CGameEffect::SetCreator [0886]
void __fastcall recovered_effect_resize_00503630(void* effect, void*, U count);                  // SetNumIntegers [1091]
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b);    // CSWSObject::ApplyEffect [0886]
U __fastcall recovered_object_remove_effect_004d06c0(void* object, void*, U id_low, U id_high);  // RemoveEffectById [w1 b7]
U __fastcall recovered_object_set_last_hostile_actor_004cc0c0(void* object, void*, U id, U flag); // SetLastHostileActor [1194w]
void __fastcall w1x_object_set_hp_004cc280(void* object, void*, U hp);                           // SetCurrentHitPoints [0886]
void __fastcall w5_object_clear_all_actions_004ccd80(void* object, void*, U flag);               // ClearAllActions [W5 t3 drive]
void __fastcall recovered_action_node_destroy_004cc4d0(void* node, void*);                       // ~CSWSObjectActionNode [w1 b7]
U __fastcall recovered_creature_incapacitated_004ef890(void* creature, void*);                   // GetDeadTemp [0963]
U __fastcall w5x_creature_is_debilitated_005b4880(void* creature, void*);                        // IsDebilitated [0886]
U __fastcall w1x_creature_max_fp_004fd490(void* creature, void*);                                // GetMaxForcePoints (AX) [0886]
void __fastcall recovered_creature_feedback_004ede10(void* creature, void*, U message, void* data); // SendFeedbackMessage [w1 b7]
void __fastcall w5_creature_auto_close_gui_panels_004eded0(void* creature, void*, U flag);       // AutoCloseGUIPanels [pf drive]
U __fastcall kotor_fastcall_004f2de0(void* creature, void*, float range, U target, U exclude);   // GetNearestEnemy [nearest_enemy]
U __fastcall kotor_fastcall_004eddd0(void* creature, void*, U spell, U cls, U a3, U a4, U a5);   // DecrementSpellReadyCount [combat_spell]
void __fastcall kotor_fastcall_004fbbe0(void* creature, void*, U target);                        // BroadcastCombatStateToParty [combat_relation]
void* __fastcall combat_reuse_faction_00513fc0(void* creature, void*);                           // CSWSCreature::GetFaction [w1 b7]
I __fastcall combat_reuse_members_005be780(void* faction, void*, U** list);                      // CSWSFaction::GetMemberList [w1 b7]
void* __fastcall kotor_fastcall_00561080(void* player, void*);                                   // CSWSPlayer::GetGameObject [0595]
U __fastcall kt_get_class_005a4e90(void* stats, void*, U index);                                 // CSWSCreatureStats::GetClass
U __fastcall kotor_fastcall_0059b650(void* spell, void*, U cls);                                 // CSWSpell::GetSpellLevel (AL) [combat_spell]
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, int id);         // CSWSpellArray::GetSpell [0820]
U __fastcall recovered_rules_roll_dice_00550b60(void* rules, void*, U count, U sides);           // CSWRules::RollDice (AX) [w1 b7]
void __fastcall kotor_fastcall_004d28f0(void* round, void*, U paused, U id);                     // SetRoundPaused [w5 combat actions]
void __fastcall kotor_fastcall_004d2920(void* round, void*, U time, U flag);                     // SetPauseTimer [w5 combat actions]
void __fastcall recovered_combat_round_advance_004d3440(void* round, void*, U time, U flag);     // DecrementRoundLength [clear9]
void* __fastcall recovered_combat_round_attack_004d2b50(void* round, void*, U index);            // CSWSCombatRound::GetAttack [w1 b7]
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);                   // CSWCCMessageData() [w1 b7]
void __fastcall recovered_message_data_destroy_004d6a40(void* data, void*);                      // ~CSWCCMessageData [w1 b7]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);           // CSWCCMessageData::SetInteger [1017]
void __fastcall recovered_event_string_set_004d6ae0(void* data, void*, U index, const void* s);  // CSWCCMessageData::SetString [0886]
void __fastcall kotor_fastcall_004d6ee0(void* data, void*, void* out);                           // CSWCCMessageData::CopyTo [combat round]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                     // GetSWSMessage
U __fastcall recovered_cc_message_00568570(void* message, void*, U player, U type, void* data, U a4); // SendServerToPlayerCCMessage
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                       // GetServerAIMaster
U __fastcall kotor_fastcall_004aea40(void* server, void*);                                       // CServerExoApp::GetPlayerCreatureId [0595]
U __fastcall kotor_fastcall_004ae9f0(void* server, void*, U option);                             // GetDifficultyOption [combat damagestats]
U __fastcall recovered_party_table_leader_00563a00(void* table, void*, U id);                    // CSWPartyTable::GetIsLeader [w1 b7]
U __fastcall recovered_party_member_00563740(void* table, void*, U id);                          // CSWPartyTable::GetIsMember [1124]
void __fastcall frame_R4_005d0fc0(void* vm, void*, unsigned* script, unsigned id, unsigned flag); // CVirtualMachine::RunScript [0457]
U __fastcall kotor_m15_client_get_player_creature_id_005ed550(void* client, void*);              // CClientExoApp::GetPlayerCreatureId
U __fastcall kotor_fastcall_005edd80(void* client, void*);                                       // CClientExoApp::GetLastTarget [w5 player control]
void __fastcall kotor_fastcall_005edea0(void* client, void*, U id, U kind, I value);             // CClientExoApp::AddFloatyText [w1 floaty]
U __fastcall recovered_party_index_006349c0(void* party, void*, U id);                           // CSWParty::GetIndex [w1 b7]
void __fastcall w5_door_open_door_00589c70(void* door, void*, U opener);                         // CSWSDoor::OpenDoor [W5 t2]
void __fastcall w5_placeable_open_inventory_00587420(void* placeable, void*, U opener, U animate); // CSWSPlaceable::OpenInventory
int __cdecl crt_rand_006fc447();                                                                 // _rand [0479]
void* __fastcall frame_LogStringCtor_005e5a90(void* string, void*, const char* text);            // CExoString(const char*) [0459]
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);                               // ~CExoString [0459]

// ---------------- exports reconstructed in this file ----------------
U __fastcall kotor_fastcall_004dfa40(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004e0750(void* self, void*, void* object, void* effect, U load_game);
void __fastcall kotor_fastcall_004f3830(void* creature, void*, I damage, U no_floaty);
void __fastcall kotor_fastcall_004ccf80(void* object, void*, I damage);
void __fastcall kotor_fastcall_005b7d10(void* creature, void*, void* attacker);
void __fastcall kotor_fastcall_004ec400(void* creature, void*, void* data, U flag);
void __fastcall kotor_fastcall_004ccd30(void* object, void*);
void __fastcall kotor_fastcall_004efef0(void* creature, void*, U state);
}

namespace {
constexpr U k_event_timed_007441d0 = 7;           // .rdata 007441D0
constexpr U k_event_script_007441d8 = 9;          // .rdata 007441D8
constexpr U k_event_queue_007441dc = 10;          // .rdata 007441DC
constexpr U16 k_subtype_magical_007473b8 = 0;     // .rdata 007473B8 (word, OR-ed into the low 3 subtype bits)
constexpr U16 k_subtype_supernatural_007473c0 = 1; // .rdata 007473C0 (word)
constexpr U8 k_type_placeable_00746638 = 9;       // .rdata 00746638 (byte)
constexpr U8 k_type_door_00746639 = 10;           // .rdata 00746639 (byte)
constexpr I k_effect_temp_hp_007466b8 = 0x0f;     // .rdata 007466B8
constexpr I k_effect_damage_shield_00746770 = 0x3d; // .rdata 00746770
constexpr U16 k_effect_damage_00746714 = 0x26;    // .rdata 00746714 (word)
constexpr U16 k_effect_visual_007466f4 = 0x1e;    // .rdata 007466F4 (word)
constexpr U k_log_scale_bits_00745ff8 = 0x40549a78u;  // .rdata 00745FF8 (3.321928f)
const float k_party_range_sq_007455bc = 900.0f;   // .rdata 007455BC
const char k_multiplier_00747028[] = "MULTIPLIER"; // .rdata 00747028
const char k_num_dice_00747038[] = "NumDice";     // .rdata 00747038
const char k_die_00747034[] = "Die";              // .rdata 00747034

// 00832878: .data cell beyond the raw data (loader zero) with no writer in the image (only readers 004DFAD4 and
// 005BB4CC); a file-local zero cell keeps the original mutable read (same as claude_combat_attack_damage.cpp).
volatile U g_cell_00832878 = 0;
// 007A1B30: initialized .data dword = 1 with a single reader (004F38F1) and no writer in the image.
volatile I g_damage_multiplier_007a1b30 = 1;

struct Str { char* text; U length; };              // CExoString (8 bytes)
static_assert(sizeof(Str) == 8, "CExoString is 8 bytes");

inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* ptr(U v) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(v)); }
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void* stats_of(void* c) { return at<void*>(c, 0xa74); }                // CSWSCreature +A74 CSWSCreatureStats*
inline void* round_of(void* c) { return at<void*>(c, 0x9c8); }                // CSWSCreature +9C8 CSWSCombatRound*
inline U id_of(void* o) { return at<U>(o, 4); }
inline I eff_int(void* e, U i) { return static_cast<I>(recovered_effect_integer_00503690(e, nullptr, i)); }
inline void eff_set(void* e, U i, I v) { recovered_effect_integer_005036a0(e, nullptr, i, static_cast<U>(v)); }
inline void* ai() { return recovered_server_ai_master_004aed80(server_app(), nullptr); }
inline void add_event(U day, U ms, U caller, U target, U type, void* event) {
    recovered_event_enqueue_004b08d0(ai(), nullptr, day, ms, caller, target, type, event);
}
inline void* party_table() { return static_cast<unsigned char*>(at<void*>(server_app(), 4)) + 0x1b770; }  // inline view of 004AEE70
inline U return_zero(void* creature) { return kotor_m15e_slot010_zero_0063e7f0(creature, nullptr); }     // 0063E7F0
inline void animate(void* object, U animation) { vcall<void>(object, 0x7c, animation); }                  // vtbl+7C
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }                                       // vtbl+30
inline U get_dead(void* o) { return vcall<U>(o, 0x94); }                                                  // vtbl+94

// operator new(8Ch) + CGameEffect(1); type word +8, subtype word +A low 3 bits := 007473B8
void* new_effect(U16 type) {
    void* memory = live_operator_new(0x8c);
    void* e = memory != nullptr ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr;
    at<U16>(e, 8) = type;
    at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xfff8) | k_subtype_magical_007473b8);
    return e;
}
void* new_message_data() {                    // operator new(34h) + CSWCCMessageData(); unchecked like the original
    void* memory = live_operator_new(0x34);
    return memory != nullptr ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr;
}
void delete_message_data(void* data) {
    if (data == nullptr) return;
    recovered_message_data_destroy_004d6a40(data, nullptr);
    live_free(data);
}
void* new_script_event() {
    void* memory = live_operator_new(0x34);
    return memory != nullptr ? recovered_event_construct_004d7540(memory, nullptr) : nullptr;
}

// fldlg2; fild [x]; fyl2x; fmul dword [00745FF8]; fstp dword; fld; frndint; fistp dword [008327E0]
// (005B7E5A..7C): round(log2(x)) through the float; 0 -> integer indefinite.
I log2_index(I x) {
    I value = x;
    float scaled;
    const float scale = flt(k_log_scale_bits_00745ff8);
    __asm {
        fldlg2
        fild dword ptr [value]
        fyl2x
        fmul dword ptr [scale]
        fstp dword ptr [scaled]
        fld dword ptr [scaled]
        frndint
        fistp dword ptr [recovered_effect_rank_scratch_008327e0]
    }
    return static_cast<I>(recovered_effect_rank_scratch_008327e0);
}

// The resist chain shared by both OnApplyDamage arms (004DFC0A..80 / 004E034B..BE): vtbl+AC DoDamageImmunity,
// vtbl+A8 DoDamageResistance, and vtbl+A4 DoDamageReduction only when (flags & 4007h) != 0 (its third argument
// is then the computed 0; the 6 sentinel is never passed).
I resist_chain(void* object, void* attacker, I total, I flags) {
    const I reduction = (flags & 0x4007) != 0 ? 0 : 6;                                      // and/neg/sbb/and/add
    const I immune = vcall<I>(object, 0xac, attacker, total, flags, 0u, 0u);
    I result = vcall<I>(object, 0xa8, attacker, immune, flags, 0u, 0u, 0u);
    if (reduction != 6) result = vcall<I>(object, 0xa4, attacker, result, reduction, 0u, 0u);
    return result;
}
}  // namespace

// ============================================================================================================
// 004DFA40 CSWSEffectListHandler::OnApplyDamage(CSWSObject* object, CGameEffect* effect, int load_game) RET 0Ch
// Integers 0..14 per-type damage, 15/18 read and unused, 16 pause time, 17 damage flags, 19 "already resisted",
// 20 "no damage shields", 21 (only when more than 21 integers) "no damage broadcast".
// ============================================================================================================
extern "C" U __fastcall kotor_fastcall_004dfa40(void*, void*, void* object, void* effect, U load_game)
{
    Str text;                                                                             // [esp+34]
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&text), nullptr); // 004DFA5C..60
    void* const obj = object;                                                             // ESI
    U dead_temp = 0;                                                                      // EDI
    if (obj != nullptr && as_creature(obj) != nullptr)                                    // 004DFA6E..82
        dead_temp = recovered_creature_incapacitated_004ef890(as_creature(obj), nullptr); // 004DFA84..92
    if (get_dead(obj) != 0 || dead_temp != 0) goto done;                                  // 004DFA94..A8 (obj unchecked)
    {
    I dmg[15];                                                                            // [esp+3C]
    I total = 0;                                                                          // [esp+88]
    for (U i = 0; i < 15; ++i) {                                                          // 004DFAC0..F9
        dmg[i] = eff_int(effect, i);
        if ((at<U>(obj, 0xf8) != 0 || g_cell_00832878 != 0) && dmg[i] > -1) dmg[i] = 0;  // 004DFAC8..E1
        if (i == 14) total = dmg[14];                                                     // 004DFAE5..EE
    }
    if (total == 0 && at<U>(obj, 0xf8) == 0) goto done;                                   // 004DFAFB..0A
    (void)eff_int(effect, 0xf);                                                           // 004DFB10..14
    const I pause_time = eff_int(effect, 0x10);                                           // [esp+24]
    const I flags = eff_int(effect, 0x11);                                                // EBX
    (void)eff_int(effect, 0x12);                                                          // 004DFB2F..35
    const I resisted = eff_int(effect, 0x13);                                             // [esp+14]
    const I no_shields = eff_int(effect, 0x14);                                           // [esp+20]
    I no_broadcast = 0;                                                                   // [esp+1C]
    if (at<I>(effect, 0x30) > 0x15) no_broadcast = eff_int(effect, 0x15);                 // 004DFB54..69 (jle)
    if (recovered_debug_cell_008327e4 != 0) {                                             // 004DFB6D..A0
        Str copy;                                                                         // [esp+2C]
        void* r = recovered_effect_copy_string_00503700(effect, nullptr, &copy, 0);
        async_assign_string_005e5c50(&text, nullptr, r);
        frame_LogStringDtor_005e5c20(&copy, nullptr);
    }
    const U creator = at<U>(effect, 0x18);                                                // EBP / [esp+10]
    void* const creature = as_creature(obj);                                              // 004DFBA8..B5
    if (creature != nullptr) {
        void* const attacker_object = game_object(creator);                               // [esp+28] 004DFBBB..D4
        void* attacker = nullptr;                                                         // [esp+8C]
        if (attacker_object != nullptr) attacker = as_creature(attacker_object);          // 004DFBE3..EC
        const U zero = return_zero(creature);                                             // [esp+2C] 004DFBF3..FA
        if (resisted == 0) {                                                              // 004DFBFE..C98
            total = resist_chain(creature, attacker, total, flags);
            for (U i = 0; i < 15; ++i)
                if (dmg[i] > -1) dmg[i] = total;                                          // 004DFC80..98
        }
        at<U>(creature, 0x160) = creator;                                                 // 004DFC9A..9E
        bool hostile_known = false;
        if (at<U>(creature, 0x9e0) != 0 && at<I>(creature, 0x128) > 0) {                 // 004DFCA4..BE
            void** list = at<void**>(creature, 0x124);                                    // EBP (loaded once)
            for (I k = 0; k < at<I>(creature, 0x128); ++k) {                              // 004DFCC6..F6
                void* e = list[k];
                if (at<U16>(e, 8) == 0x23 && at<U>(e, 0) == at<U>(effect, 0) && at<U>(e, 4) == at<U>(effect, 4)) {
                    hostile_known = true;
                    break;
                }
            }
        }
        if (!hostile_known) recovered_object_set_last_hostile_actor_004cc0c0(creature, nullptr, creator, 0); // 004DFCF8..D01
        for (U i = 0; i < 15; ++i) at<I*>(creature, 0x168)[i] = dmg[i];                  // 004DFD06..23
        if (no_shields == 0 && resisted != 0) kotor_fastcall_005b7d10(creature, nullptr, attacker); // 004DFD25..3F
        if (total > 0) {                                                                  // 004DFD44..4D (jle)
            void* client = client_app();                                                  // EDI
            const U creator_client = kotor_fastcall_005eda50(client, nullptr, creator);
            const I damage = total;                                                       // EBX
            U no_floaty = 0;
            if (kotor_m15_client_get_player_creature_id_005ed550(client, nullptr) == creator_client) { // 004DFD6C..7A
                kotor_fastcall_005edea0(client, nullptr, kotor_fastcall_005eda50(client, nullptr, id_of(creature)), 0, damage);
                no_floaty = 1;
            }
            kotor_fastcall_004f3830(creature, nullptr, damage, no_floaty);                // 004DFD92..9B
            const U animation = at<U>(creature, 0xd4);                                    // 004DFDA0..B9
            if (animation == 0x271f || animation == 0x2720 || animation == 0x2af8) {
                void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(at<void*>(rules(), 0x8c), nullptr,
                                                                        at<I>(creature, 0x1c0)); // 004DFDBF..D1
                if (spell != nullptr && at<U>(spell, 0x134) == 1) {                       // 004DFDD8..E7
                    const I roll = static_cast<I>(recovered_rules_roll_dice_00550b60(rules(), nullptr, 1, 0x14) & 0xffff);
                    (void)kotor_fastcall_004f2de0(creature, nullptr, 4.0f, k_object_invalid, k_object_invalid); // 004DFDFC..10
                    I level;
                    const U8 cls = at<U8>(creature, 0x1d4);
                    if (cls != 0xff) {                                                    // 004DFE15..44
                        // mov byte [esp+88], al: the class byte replaces the low byte of the damage slot
                        total = static_cast<I>((static_cast<U>(total) & 0xffffff00u) | cls);
                        const U class_id = kt_get_class_005a4e90(stats_of(creature), nullptr, static_cast<U>(total));
                        level = static_cast<I>(kotor_fastcall_0059b650(spell, nullptr, class_id) & 0xff);
                    } else {
                        level = at<U8>(spell, 0x3f);                                      // 004DFE46
                    }
                    I dc = level + damage + 10;                                           // 004DFE4A..53
                    if (dc < 1) dc = 1;
                    if (roll < dc) {                                                      // 004DFE58..5A (jge)
                        kotor_fastcall_004ccd30(creature, nullptr);                       // 004DFE60..62
                        animate(creature, 0x271e);                                        // 004DFE67..70
                        const U8 cls2 = at<U8>(creature, 0x1d4);
                        const U spell_id = at<U>(creature, 0x1c0);
                        total = static_cast<I>((static_cast<U>(total) & 0xffffff00u) | cls2);   // [esp+88] low byte
                        (void)kotor_fastcall_004eddd0(creature, nullptr, spell_id, static_cast<U>(total), 0, 0, 0xff);
                        void* e = new_effect(k_effect_visual_007466f4);                   // 004DFE9F..F0 (type 1Eh)
                        recovered_effect_creator_00503a00(e, nullptr, id_of(creature));   // 004DFEF4..F8
                        recovered_effect_apply_004d14f0(creature, nullptr, e, load_game, 0); // 004DFEFD..0A
                    }
                }
            }
        }
        if (resisted == 0 && w5x_creature_is_debilitated_005b4880(creature, nullptr) == 0) { // 004DFF0F..24
            if (dmg[7] > 0 || dmg[9] > 0) {                                               // 004DFF26..34 (jg)
                const U animation = at<U>(creature, 0xd4);
                if (animation == 0x2710 || animation == 0x2711) {                         // 004DFF3D..4F
                    if (at<U>(round_of(creature), 0x958) == 0) {                          // 004DFF51..5F
                        kotor_fastcall_004d28f0(round_of(creature), nullptr, 1, creator);
                        kotor_fastcall_004d2920(round_of(creature), nullptr, static_cast<U>(pause_time), 0);
                        recovered_combat_round_advance_004d3440(round_of(creature), nullptr, static_cast<U>(pause_time), 0);
                    }
                    animate(creature, 0x2727);                                            // 004DFF8D..96
                }
            } else {
                animate(creature, 0x283e);                                                // 004DFF36..96
            }
        }
        (void)kotor_fastcall_004ae910(server_app(), nullptr, id_of(creature));            // 004DFF99..A6 (discarded)
        if (at<U>(creature, 0x9d4) == 0 && get_dead(creature) == 0 &&                     // 004DFFAB..E9
            recovered_creature_incapacitated_004ef890(creature, nullptr) == 0 &&
            recovered_party_table_leader_00563a00(party_table(), nullptr, id_of(creature)) == 0)
            frame_R4_005d0fc0(ptr(frame_control::message_manager_007a3a00), nullptr,
                              &at<unsigned>(creature, 0x250), id_of(creature), 1);        // 004DFFEB..FE
        if (no_broadcast == 0) {                                                          // 004E0003..09
            if (attacker_object != nullptr) {                                             // 004E000F..17
                void* data = new_message_data();                                          // 004E001D..43
                recovered_event_object_004d7780(data, nullptr, 0, creator);
                recovered_event_object_004d7780(data, nullptr, 1, id_of(creature));
                for (U i = 0; i < 15; ++i) recovered_event_integer_004d76b0(data, nullptr, i, static_cast<U>(dmg[i]));
                if (recovered_debug_cell_008327e4 != 0) recovered_event_string_set_004d6ae0(data, nullptr, 0, &text);
                kotor_fastcall_004ec400(creature, nullptr, data, 0);                      // 004E009A..9F
                if (attacker != nullptr &&
                    at<U>(stats_of(creature), 0x78) != at<U>(stats_of(attacker), 0x78))  // 004E00A4..C1
                    kotor_fastcall_004ec400(attacker, nullptr, data, 0);
                delete_message_data(data);                                                // 004E00CD..49
            } else {
                void* data = new_message_data();                                          // 004E00DF..05
                recovered_event_object_004d7780(data, nullptr, 0, id_of(creature));
                for (U i = 0; i < 15; ++i) recovered_event_integer_004d76b0(data, nullptr, i, static_cast<U>(dmg[i]));
                kotor_fastcall_004ec400(creature, nullptr, data, 1);                      // 004E0133..38
                delete_message_data(data);
            }
        }
        if (zero == 0 && return_zero(creature) != 0) {                                    // 004E0151..66
            const U8 state = at<U8>(creature, 0x4c4);
            if (state != 3 && state != 4 && state != 0xe) {                               // 004E016C..7C
                if (crt_rand_006fc447() % 2 != 0) {                                       // 004E017E..91
                    kotor_fastcall_004efef0(creature, nullptr, 3);
                    animate(creature, 0x2710);
                } else {
                    kotor_fastcall_004efef0(creature, nullptr, 4);
                    animate(creature, 0x2710);
                }
            }
            at<U>(creature, 0xe8) = 0;                                                    // 004E01BB
            void* module = server_get_module_004ae6b0(server_app(), nullptr);             // 004E01C5..CD
            const U module_target = at<U>(module, 0x20);
            void* event = new_script_event();                                             // 004E01D8..FE
            at<U16>(event, 0) = 0x20;
            add_event(0, 0, id_of(creature), module_target, k_event_queue_007441dc, event); // 004E0200..2D
            w5_creature_auto_close_gui_panels_004eded0(creature, nullptr, 0);             // 004E0232..36
            goto done;
        }
        if (get_dead(creature) == 0 && recovered_creature_incapacitated_004ef890(creature, nullptr) == 0) // 004E0240..57
            goto done;
        void* death = new_effect(0x13);                                                   // 004E025D..B3 (type 13h)
        recovered_effect_creator_00503a00(death, nullptr, creator);                       // 004E02B7
        if (attacker != nullptr) {                                                        // 004E02BC..DF
            void* attack = recovered_combat_round_attack_004d2b50(round_of(attacker), nullptr,
                                                                  at<U8>(round_of(attacker), 0x96c));
            if (at<U>(attack, 0x40) == 0) eff_set(death, 0, 1);                           // 004E02E1..E7
        }
        eff_set(death, 1, 1);                                                             // 004E02EC..F2
        recovered_effect_apply_004d14f0(creature, nullptr, death, load_game, 0);          // 004E02F7..04
        goto done;
    }
    // ---------------- not a creature: placeables and doors only (004E030E..) ----------------
    {
        const U8 type = at<U8>(obj, 8);
        if (type != k_type_placeable_00746638 && type != k_type_door_00746639) goto done; // 004E030E..1F
        void* const attacker = kotor_fastcall_004ae770(server_app(), nullptr, creator);   // [esp+8C] 004E0329..38
        I amount;                                                                         // EBP
        if (resisted == 0) {                                                              // 004E033F..45
            const I reduction = (flags & 0x4007) != 0 ? 0 : 6;
            const I immune = vcall<I>(obj, 0xac, attacker, total, flags, 0u, 0u);
            total = vcall<I>(obj, 0xa8, attacker, immune, flags, 0u, 0u, 0u);             // [esp+88]
            amount = reduction != 6 ? vcall<I>(obj, 0xa4, attacker, total, reduction, 0u, 0u) : total;
            const U animation = at<U>(obj, 0xd4);                                         // 004E03C5..D9
            if (animation == 0x2726 || animation == 0x275c) {
                add_event(0, 0, at<U>(effect, 0x18), id_of(obj), k_event_script_007441d8, ptr(animation)); // 004E03DB..FF
                animate(obj, 0x271e);                                                     // 004E0404..0D
            }
        } else {
            amount = total;                                                               // 004E0412
        }
        void* client = client_app();                                                      // EBX
        const U creator_client = kotor_fastcall_005eda50(client, nullptr, creator);       // [esp+88]
        if (kotor_m15_client_get_player_creature_id_005ed550(client, nullptr) == creator_client) // 004E0431..48
            kotor_fastcall_005edea0(client, nullptr, kotor_fastcall_005eda50(client, nullptr, id_of(obj)), 0, amount);
        vcall<void>(obj, 0xa0, amount);                                                   // 004E0460..65 DoDamage
        void* event = new_script_event();                                                 // 004E046B..93
        at<U16>(event, 0) = 4;                                                            // 004E04A2
        recovered_event_integer_004d76b0(event, nullptr, 0, static_cast<U>(amount));      // 004E0495..A7
        add_event(0, 0, at<U>(effect, 0x18), id_of(obj), k_event_queue_007441dc, event);  // 004E04AC..D0
        void* data = new_message_data();                                                  // 004E04D5..FD
        recovered_event_object_004d7780(data, nullptr, 0, creator);                       // 004E04FF..10
        recovered_event_object_004d7780(data, nullptr, 1, id_of(obj));
        for (U i = 0; i < 15; ++i) recovered_event_integer_004d76b0(data, nullptr, i, static_cast<U>(dmg[i]));
        if (recovered_debug_cell_008327e4 != 0) recovered_event_string_set_004d6ae0(data, nullptr, 0, &text);
        if (attacker != nullptr) {                                                        // 004E054F..6D
            kotor_fastcall_004fbbe0(attacker, nullptr, k_object_invalid);
            kotor_fastcall_004ec400(attacker, nullptr, data, 0);
        }
        delete_message_data(data);                                                        // 004E0572..83
        if (get_dead(obj) != 1) goto done;                                                // 004E0586..97
        if (vcall<void*>(obj, 0x18) != nullptr) {                                         // 004E059D..AA AsSWSDoor
            at<U>(obj, 0xf8) = 1;
            at<U>(vcall<void*>(obj, 0x18), 0x2c4) = 0;
            w5_door_open_door_00589c70(vcall<void*>(obj, 0x18), nullptr, creator);       // 004E05BB..C5
        } else if (vcall<void*>(obj, 0x48) != nullptr) {                                  // 004E05CF..D4 AsSWSPlaceable
            if (at<U>(vcall<void*>(obj, 0x48), 0x324) != 0) {                             // 004E05DA..E7
                at<U>(obj, 0xf8) = 1;
                at<U>(vcall<void*>(obj, 0x48), 0x260) = 0;                                // 004E05ED..FC
                U* holder = static_cast<U*>(live_operator_new(4));                        // 004E05FA..07
                if (holder != nullptr) {                                                  // 004E0614..16
                    const U player = kotor_fastcall_004aea40(server_app(), nullptr);
                    *holder = player;
                    add_event(0, 0, player, id_of(obj), k_event_script_007441d8, ptr(0x275b)); // 004E061F..42
                    add_event(0, 1000, *holder, id_of(obj), k_event_timed_007441d0, holder);   // 004E0647..6B
                } else {
                    w5_placeable_open_inventory_00587420(vcall<void*>(obj, 0x48), nullptr,
                                                         kotor_fastcall_004aea40(server_app(), nullptr), 1); // 004E0675..86
                }
            } else {
                void* death = new_effect(0x13);                                           // 004E0690..E1
                recovered_effect_creator_00503a00(death, nullptr, creator);               // 004E06CA..E5 (EBP)
                eff_set(death, 0, 0);
                eff_set(death, 1, 1);
                recovered_effect_apply_004d14f0(obj, nullptr, death, load_game, 0);       // 004E06FE..0A
            }
        }
        if (attacker != nullptr) w5_object_clear_all_actions_004ccd80(attacker, nullptr, 1); // 004E070F..1C
    }
    }
done:
    frame_LogStringDtor_005e5c20(&text, nullptr);                                         // 004E0721..30
    return 1;
}

// ============================================================================================================
// 004E0750 CSWSEffectListHandler::OnApplyHeal(CSWSObject* object, CGameEffect* effect, int load_game) RET 0Ch
// Integer 0 = amount, integer 1 = 36h heals force points instead of hit points.
// ============================================================================================================
extern "C" U __fastcall kotor_fastcall_004e0750(void*, void*, void* object, void* effect, U load_game)
{
    if (as_creature(object) == nullptr) return 1;                                         // 004E0767..74
    void* const creature = as_creature(object);                                           // 004E077A..89
    if (effect == nullptr) return 1;                                                      // 004E0783..8B
    I amount = eff_int(effect, 0);                                                        // EBX
    if (eff_int(effect, 1) == 0x36) {                                                     // 004E079A..A8
        void* stats = stats_of(creature);
        if (stats == nullptr) return 1;                                                   // 004E07AE..B6
        const I current = static_cast<short>(static_cast<U16>(at<U16>(stats, 0x126) + at<U16>(stats, 0x124))); // movsx
        const I maximum = static_cast<short>(static_cast<U16>(w1x_creature_max_fp_004fd490(creature, nullptr)));
        if (current + amount > maximum) amount = maximum - current;                       // 004E07D7..E0 (jle)
        at<U16>(stats_of(creature), 0x124) = static_cast<U16>(amount + current);          // 004E07E2..ED
        void* data = new_message_data();                                                  // 004E07EB..17
        recovered_event_object_004d7780(data, nullptr, 0, id_of(creature));
        recovered_event_integer_004d76b0(data, nullptr, 0, static_cast<U>(amount));
        recovered_creature_feedback_004ede10(creature, nullptr, 0xe4, data);              // 004E0838..40
        return 1;
    }
    if (get_dead(creature) != 0) return 1;                                                // 004E084A..56
    if (recovered_creature_incapacitated_004ef890(creature, nullptr) != 0) return 1;      // 004E085C..65
    const U zero = return_zero(creature);                                                 // [esp+20] 004E086B..78
    const I hp = static_cast<short>(static_cast<U16>(vcall<U>(creature, 0x9c, 1u))) + amount;   // vtbl+9C GetCurrentHitPoints
    if (hp > static_cast<short>(static_cast<U16>(vcall<U>(creature, 0x98, 1u)))) {        // vtbl+98 GetMaxHitPoints (jle)
        const I current = static_cast<short>(static_cast<U16>(vcall<U>(creature, 0x9c, 1u)));
        amount = static_cast<short>(static_cast<U16>(vcall<U>(creature, 0x98, 1u))) - current; // 004E089A..B8
    }
    void* client = client_app();                                                          // EDI
    const U client_id = kotor_fastcall_005eda50(client, nullptr, id_of(creature));        // EBP
    if (kotor_fastcall_005edd80(client, nullptr) == client_id ||                          // 004E08D3..DA
        recovered_party_index_006349c0(kotor_fastcall_005ed8b0(client, nullptr), nullptr, client_id) != 0xffffffffu)
        kotor_fastcall_005edea0(client, nullptr, client_id, 1, amount);                   // 004E08F0..F6
    w1x_object_set_hp_004cc280(creature, nullptr,
        static_cast<U>(static_cast<short>(static_cast<U16>(vcall<U>(creature, 0x9c, 1u))) + amount)); // 004E08FB..0F
    void* data = new_message_data();                                                      // EDI 004E0914..39
    void* copy = new_message_data();                                                      // EBP 004E093B..68
    recovered_event_object_004d7780(data, nullptr, 0, id_of(creature));                   // 004E096A..7A
    recovered_event_integer_004d76b0(data, nullptr, 0, static_cast<U>(amount));           // 004E097F..84
    kotor_fastcall_004d6ee0(data, nullptr, copy);                                         // 004E0989..8C
    recovered_creature_feedback_004ede10(creature, nullptr, 0x97, data);                  // 004E0991..99
    void* healer = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(effect, 0x18));   // 004E099E..AF
    if (healer != nullptr && healer != creature) {                                        // 004E09B4..BA
        recovered_creature_feedback_004ede10(healer, nullptr, 0x97, copy);
    } else {
        delete_message_data(copy);                                                        // 004E09CB..DC
    }
    // remove every supernatural (subtype bits 1) effect of type 54h; the list is sorted by type (ja -> stop)
    for (I i = 0; i < at<I>(creature, 0x128);) {                                          // 004E09DF..2C
        void* e = at<void**>(creature, 0x124)[i];
        const U16 type = at<U16>(e, 8);
        if (type > 0x54) break;
        if (type == 0x54 && static_cast<U16>(at<U8>(e, 0xa) & 7) == k_subtype_supernatural_007473c0) {
            recovered_object_remove_effect_004d06c0(creature, nullptr, at<U>(e, 0), at<U>(e, 4));
            i = 0;                                                                        // restart from the head
            continue;
        }
        ++i;
    }
    if (zero == 1 && return_zero(creature) == 0) {                                        // 004E0A2E..3E
        void* e = new_effect(4);                                                          // 004E0A40..93 (type 4)
        recovered_effect_apply_004d14f0(creature, nullptr, e, load_game, 0);              // 004E0A97..99
    }
    return 1;
}

// ============================================================================================================
// 004F3830 CSWSCreature::DoDamage(int damage, int no_floaty) RET 8
// ============================================================================================================
extern "C" void __fastcall kotor_fastcall_004f3830(void* creature, void*, I damage, U no_floaty)
{
    I amount;                                                                             // EBX
    void* table = party_table();                                                          // 004F383E..56
    const bool party = (table != nullptr && recovered_party_member_00563740(table, nullptr, id_of(creature)) != 0) ||
                       at<U>(creature, 0x9d4) != 0;                                       // 004F385B..76
    if (party) {
        const U difficulty = at<U8>(kotor_fastcall_005ed700(client_app(), nullptr), 4);  // 004F3878..86
        Str column;                                                                       // [esp+1C]
        frame_LogStringCtor_005e5a90(&column, nullptr, k_multiplier_00747028);            // 004F388A..93
        float multiplier;                                                                 // [esp+18]
        void* table2da = at<void*>(at<void*>(rules(), 0xb8), 0x7c);                       // 004F3898..A4
        const U found = c2da_current_get_float_00413430(table2da, nullptr, difficulty, &column, &multiplier);
        frame_LogStringDtor_005e5c20(&column, nullptr);                                   // 004F38BD..CB
        if (found != 0)
            amount = static_cast<I>(w2::ftol2_006fae8c(static_cast<double>(damage) * static_cast<double>(multiplier))); // 004F38D4..E1
        else
            amount = damage;
    } else {
        amount = damage;                                                                  // 004F38E5
    }
    if (at<U>(creature, 0xa88) == 0 && g_damage_multiplier_007a1b30 > 1)                  // 004F38E9..F9
        amount = g_damage_multiplier_007a1b30 * amount;
    if (static_cast<short>(at<U16>(creature, 0xe4)) > 0) {                                // 004F3900..07 (word, jle)
        I absorbed = 0;                                                                   // [esp+10]
        unsigned char* list = static_cast<unsigned char*>(as_creature(creature)) + 0x124; // [esp+14]
        if (at<I>(list, 4) > 0) {                                                         // 004F3919..2A
            I i = at<U16>(stats_of(creature), 0x12c);                                     // [esp+34]
            if (i < at<I>(list, 4)) {                                                     // 004F393D..43
                do {
                    void* e = at<void**>(list, 0)[i];                                     // 004F3945..47
                    if (static_cast<I>(at<U16>(e, 8)) > k_effect_temp_hp_007466b8) break; // 004F394A..56 (jg)
                    if (static_cast<I>(at<U16>(e, 8)) == k_effect_temp_hp_007466b8) {
                        I value = eff_int(e, 0);                                          // 004F3960..69
                        if (value > amount) {                                             // jle
                            value -= amount;
                            amount = 0;
                            eff_set(e, 0, value);                                         // 004F3971..77
                            absorbed += value;
                        } else {
                            const U low = at<U>(e, 0), high = at<U>(e, 4);
                            amount -= value;                                              // 004F398E
                            recovered_object_remove_effect_004d06c0(as_creature(creature), nullptr, low, high);
                        }
                    }
                    ++i;                                                                  // 004F39A2..AC
                } while (i < at<I>(list, 4));
            }
        }
        at<I>(creature, 0xe4) = absorbed;                                                 // 004F39AE..B2 (dword)
    }
    if (kotor_fastcall_004ae9f0(server_app(), nullptr, 5) == 1 && at<U>(creature, 0x9d4) == 1) { // 004F39B8..D4
        if (static_cast<short>(static_cast<U16>(vcall<U>(creature, 0x9c, 1u))) - amount < 1) { // 004F39D6..EB (jge)
            amount = static_cast<short>(static_cast<U16>(vcall<U>(creature, 0x9c, 1u))) - 1;
            if (amount < 0) amount = 0;                                                   // dec / jns
        }
    }
    void* client = client_app();                                                          // ESI
    const U client_id = kotor_fastcall_005eda50(client, nullptr, id_of(creature));        // EDI
    if (kotor_m15_client_get_player_creature_id_005ed550(client, nullptr) == client_id && no_floaty == 0)
        kotor_fastcall_005edea0(client, nullptr, client_id, 0, amount);                   // 004F3A2A..30
    kotor_fastcall_004ccf80(creature, nullptr, amount);                                   // 004F3A35..38
}

// ============================================================================================================
// 004CCF80 CSWSObject::DoDamage(int damage) RET 4: +DC = current HP - damage, floored at 1 when +200 (plot) is set
// ============================================================================================================
extern "C" void __fastcall kotor_fastcall_004ccf80(void* object, void*, I damage)
{
    const I hp = static_cast<short>(static_cast<U16>(vcall<U>(object, 0x9c, 1u))) - damage;   // 004CCF83..9A
    at<I>(object, 0xdc) = hp;                                                             // 004CCF9E
    if (at<U>(object, 0x200) != 0 && hp < 1) at<I>(object, 0xdc) = 1;                     // 004CCF9C..AB
}

// ============================================================================================================
// 005B7D10 CSWSCreature::ResolveDamageShields(CSWSCreature* attacker) RET 4
// For every damage-shield effect (type 3Dh) of this creature: damage = int0 + (int1 > 5 ? RollDice(NumDice, Die of
// 2DA row int1) : int1) of damage type log2(int2), applied to the attacker with a 1Eh visual effect.
// ============================================================================================================
extern "C" void __fastcall kotor_fastcall_005b7d10(void* creature, void*, void* attacker)
{
    unsigned char* list = static_cast<unsigned char*>(creature) + 0x124;                  // [esp+14]
    I i = at<U16>(stats_of(creature), 0x156);                                             // [esp+C]
    if (attacker == nullptr) return;                                                      // 005B7D4D
    if (i >= at<I>(list, 4)) return;                                                      // 005B7D53..56 (jge)
    do {
        void* e = at<void**>(list, 0)[i];                                                 // 005B7D65..67
        if (static_cast<I>(at<U16>(e, 8)) > k_effect_damage_shield_00746770) break;       // jg
        if (static_cast<I>(at<U16>(e, 8)) == k_effect_damage_shield_00746770) {
            const I base = eff_int(e, 0);                                                 // EBX
            I dice = eff_int(e, 1);                                                       // ESI
            const I type_bits = eff_int(e, 2);                                            // [esp+20]
            if (dice > 5) {                                                               // 005B7DA1..A8 (jle)
                I num_dice;                                                               // [esp+14]
                I die;                                                                    // [esp+18]
                void* table = at<void*>(at<void*>(rules(), 0xb8), 0xb0);
                Str column;
                frame_LogStringCtor_005e5a90(&column, nullptr, k_num_dice_00747038);      // 005B7DAE..B7
                c2da_current_get_int_00413660(table, nullptr, static_cast<U>(dice), &column, &num_dice);
                frame_LogStringDtor_005e5c20(&column, nullptr);
                Str column2;
                frame_LogStringCtor_005e5a90(&column2, nullptr, k_die_00747034);          // 005B7DF7..00
                table = at<void*>(at<void*>(rules(), 0xb8), 0xb0);
                c2da_current_get_int_00413660(table, nullptr, static_cast<U>(dice), &column2, &die);
                frame_LogStringDtor_005e5c20(&column2, nullptr);
                dice = static_cast<I>(recovered_rules_roll_dice_00550b60(rules(), nullptr, static_cast<U>(num_dice),
                                                                         static_cast<U>(die)) & 0xffff);
            }
            const I damage = dice + base;                                                 // EBP 005B7E5C
            const I index = log2_index(type_bits);                                        // EBX 005B7E5A..7C
            void* shield = new_effect(k_effect_damage_00746714);                          // 005B7E82..CF (type 26h)
            recovered_effect_creator_00503a00(shield, nullptr, id_of(creature));          // 005B7ED3..DD
            recovered_effect_resize_00503630(shield, nullptr, 0x15);                      // 005B7EE2..E6
            for (I j = 0; j < 15; ++j) {                                                  // 005B7EEB..06
                if (j == index) eff_set(shield, static_cast<U>(index), damage);
                else eff_set(shield, static_cast<U>(j), -1);
            }
            eff_set(shield, 0x10, 1000);                                                  // 005B7F08..11
            recovered_effect_apply_004d14f0(attacker, nullptr, shield, 0, 0);             // 005B7F16..1F
            void* visual = new_effect(k_effect_visual_007466f4);                          // 005B7F24..75 (type 1Eh)
            recovered_effect_creator_00503a00(visual, nullptr, id_of(creature));          // 005B7F79..83
            eff_set(visual, 0, 0);                                                        // 005B7F88..8E
            recovered_effect_apply_004d14f0(attacker, nullptr, visual, 0, 0);             // 005B7F93..9C
        }
        ++i;                                                                              // 005B7FAE..B8
    } while (i < at<I>(list, 4));
}

// ============================================================================================================
// 004EC400 CSWSCreature::BroadcastDamageDataToParty(CSWCCMessageData* data, int self_only) RET 8
// Sends the CC message (type 2 when self_only, else 3) to every faction member's player whose game object is in
// the same area within 30 m (squared distance <= 900, NaN excluded).
// ============================================================================================================
extern "C" void __fastcall kotor_fastcall_004ec400(void* creature, void*, void* data, U flag)
{
    U* members;                                                                           // [esp+8]
    combat_reuse_members_005be780(combat_reuse_faction_00513fc0(creature, nullptr), nullptr, &members); // 004EC403..11
    for (I i = 0; i < at<I>(combat_reuse_faction_00513fc0(creature, nullptr), 4); ++i) { // 004EC416..F1
        void* player = kotor_fastcall_004ae910(server_app(), nullptr, members[i]);        // 004EC430..46
        if (player == nullptr) continue;
        void* object = kotor_fastcall_00561080(player, nullptr);                          // 004EC450..57
        if (object == nullptr) continue;
        if (at<U>(creature, 0x8c) != at<U>(object, 0x8c)) continue;                       // 004EC45F..6B
        const double dx = static_cast<double>(at<float>(object, 0x90)) - static_cast<double>(at<float>(creature, 0x90));
        const double dy = static_cast<double>(at<float>(object, 0x94)) - static_cast<double>(at<float>(creature, 0x94));
        const double dz = static_cast<double>(at<float>(object, 0x98)) - static_cast<double>(at<float>(creature, 0x98));
        const double distance = (dz * dz + dy * dy) + dx * dx;                            // 004EC491..9F
        if (!(distance <= static_cast<double>(k_party_range_sq_007455bc))) continue;      // fcomp / test ah,41h / jp
        const U player_id = at<U>(player, 4);                                             // 004EC4BA
        recovered_cc_message_00568570(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr,
                                      player_id, flag != 0 ? 2u : 3u, data, 0);          // 004EC4B4..E1
    }
}

// ============================================================================================================
// 004CCD30 CSWSObject::DeleteCurrentAIAction() RET 0
// ============================================================================================================
extern "C" void __fastcall kotor_fastcall_004ccd30(void* object, void*)
{
    void* head = at<void*>(at<void*>(object, 0xfc), 0);                                   // 004CCD33..3D
    if (head == nullptr) return;
    void* node = head != nullptr ? at<void*>(head, 8) : nullptr;                          // 004CCD3F..45
    if (vcall<U>(object, 0x74, node, 1u) != 1) return;                                    // 004CCD48..55 vtbl+74
    void* removed = base_ini_reconstruction::remove_head_005e9c80(
        static_cast<kotor_list::Internal*>(at<void*>(object, 0xfc)), nullptr);           // 004CCD57..5D
    if (removed == nullptr) return;
    recovered_action_node_destroy_004cc4d0(removed, nullptr);                             // 004CCD68..6A
    live_free(removed);                                                                   // 004CCD6F..75
}

// ============================================================================================================
// 004EFEF0 CSWSCreature::SetAmbientAnimationState(unsigned char state) RET 4
// ============================================================================================================
extern "C" void __fastcall kotor_fastcall_004efef0(void* creature, void*, U state)
{
    const U8 value = static_cast<U8>(state);                                              // DL
    if (value == 4 || value == 3) {                                                       // 004EFEF4..FC
        const U animation = at<U>(creature, 0xd4);
        if (animation == 0x279b || animation == 0x2785 || animation == 0x28a0) {          // 004EFEFE..17
            at<U8>(creature, 0x4c4) = 0xe;
            return;
        }
    }
    at<U8>(creature, 0x4c4) = value;                                                      // 004EFF24
}
