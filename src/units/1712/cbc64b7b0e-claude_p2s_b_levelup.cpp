// Lane worker "p2s-b" (fork levelup): 00527560 CSWSMessage::HandlePlayerToServerLevelUpMessage and its missing closure.
// Transcribed from the original swkotor.exe (read-only); statement comments cite original instruction addresses.
// Original ABI: thiscall(CSWSMessage* this; CSWSPlayer* player, BYTE minor) RET 8.
// Export: extern "C" unsigned __fastcall recovered_player_to_server_level_up_00527560(message, edx, player, minor) @16.
// Functions reconstructed here (no directly callable provider in the c16 map):
//   005CB120 CSWLevelStats::SetSkillRankChange(WORD, BYTE)           thiscall RET 8   -> @kotor_fastcall_005cb120@16
//   005CB150 CSWLevelStats::AddFeat(WORD)                            thiscall RET 4   -> @kotor_fastcall_005cb150@12
//   005CB270 CSWLevelStats::~CSWLevelStats()                         thiscall RET 0   -> @kotor_fastcall_005cb270@8
//   0056A560 CSWSMessage::SendServerToPlayerLevelUp_Confirmation(DWORD player, int ok) thiscall RET 8 -> @kotor_fastcall_0056a560@16
//   005AF950 CSWSCreatureStats::ValidateLevelUp(CSWLevelStats*)      thiscall RET 4   -> @kotor_fastcall_005af950@12
//   005AABF0 CSWSCreatureStats::LevelUp(CSWLevelStats*, int)         thiscall RET 8   -> @kotor_fastcall_005aabf0@16
//   005AA810 CSWSCreatureStats::AddFeat(WORD)                        thiscall RET 4   -> @kotor_fastcall_005aa810@12
#include "6bd5403233-claude_dialogue_common.hpp"

using namespace dlg;

namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth {
void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations* list, void*, unsigned count);  // CExoArrayList::Allocate
}
namespace kotor { namespace server_mainloop { unsigned creature_force_class_005be4a0(unsigned class_id); } }  // CSWClass::IsJedi (cdecl)

extern "C" {
extern void* kotor_cell_rules_007a3a28;                                   // [007A3A28] CSWRules*
// w2_message_native.obj
U __fastcall kotor_fastcall_004d6230(void* msg, void*, U bits);           // MessageReadOverflow RET 4
U __fastcall kotor_fastcall_004d6240(void* msg, void*, U bits);           // MessageReadUnderflow RET 4
U __fastcall kotor_fastcall_004d6780(void* msg, void*, U bits);           // ReadDWORD RET 4
U8 __fastcall kotor_fastcall_004d6720(void* msg, void*, U bits);          // ReadBYTE RET 4
U16 __fastcall kotor_fastcall_004d6750(void* msg, void*, U bits);         // ReadWORD RET 4
// players / creatures / stats
void* __fastcall kotor_fastcall_00561080(void* player, void*);           // CSWSPlayer::GetGameObject
void* __fastcall recovered_history_level_ctor_005cb1b0(void* level, void*);  // CSWLevelStats::CSWLevelStats (0963)
U8 __fastcall recovered_stats_level_005a5fd0(void* stats, void*, U flag);   // CSWSCreatureStats::GetLevel (AL)
U __fastcall recovered_can_level_up_005a6810(void* stats, void*);         // CSWSCreatureStats::CanLevelUp (w2_g07_native)
void __fastcall recovered_creature_feedback_004ede10(void* creature, void*, U id, void* data);  // SendFeedbackMessage
void* __fastcall recovered_event_construct_004d7540(void* event, void*);  // CScriptEvent::CScriptEvent
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);  // GetServerAIMaster
U __fastcall recovered_event_insert_004afdb0(void* ai, void*, U day, U tick, U source, U target, U kind, void* data); // AddEventAbsoluteTime
void* __fastcall server_get_module_004ae6b0(void* server, void*);          // CServerExoApp::GetModule
void __fastcall recovered_stats_refresh_005addc0(void* stats, void*);     // UpdateCombatInformation
U16 __fastcall w1x_creature_max_fp_004fd490(void* creature, void*);       // CSWSCreature::GetMaxForcePoints
U8 __fastcall recovered_creature_stats_str_005a6190(void* stats, void*);
U8 __fastcall recovered_creature_stats_dex_005a6550(void* stats, void*);
U8 __fastcall recovered_creature_stats_con_005a6250(void* stats, void*);
U8 __fastcall recovered_creature_stats_int_005a6310(void* stats, void*);
U8 __fastcall recovered_creature_stats_wis_005a63d0(void* stats, void*);
U8 __fastcall recovered_creature_stats_cha_005a6490(void* stats, void*);
void __fastcall w0886x_stats_adjust_spell_uses_005a6d80(void* stats, void*);  // AdjustSpellUsesPerDay
U __fastcall recovered_skill_rank_change_005cb0f0(void* level, void*, U skill); // CSWLevelStats::GetSkillRankChange (AL)
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, int id);  // CSWSpellArray::GetSpell
void __fastcall w1x_object_set_hp_004cc280(void* object, void*, U hp);    // CSWSObject::SetCurrentHitPoints
void* __fastcall kotor_rules_get_feat_00550c00(void* rules, void*, U feat);  // CSWRules::GetFeat (word)
void __fastcall kotor_m15_array_u16_add_unique_005a7670(void* list, void*, U value);  // CExoArrayList<ushort>::AddUnique
void __fastcall kotor_m15_array_u16_allocate_004d2fa0(void* list, void*, U capacity); // CExoArrayList<ushort>::Allocate
void __fastcall exo_array_list_destruct_0056b6f0(void* list, void*);      // CExoArrayList<ulong>::~CExoArrayList
int __fastcall net_layer_send_message_to_player_005d4cb0(void* net, void*, U player, const char* data, U size, U flags);

// exports of this file
void __fastcall kotor_fastcall_005cb120(void* self, void*, U skill, U value);
void __fastcall kotor_fastcall_005cb150(void* self, void*, U feat);
void __fastcall kotor_fastcall_005cb270(void* self, void*);
U __fastcall kotor_fastcall_0056a560(void* message, void*, U player, U ok);
U __fastcall kotor_fastcall_005af950(void* stats, void*, void* level);
void __fastcall kotor_fastcall_005aabf0(void* stats, void*, void* level, U flag);
void __fastcall kotor_fastcall_005aa810(void* stats, void*, U feat);
U __fastcall recovered_player_to_server_level_up_00527560(void* message, void*, void* player, U minor);
}

namespace {
constexpr U k_event_level_up_007441dc = 10;     // .rdata [007441DC] dword 0000000Ah
inline void* rules() { return kotor_cell_rules_007a3a28; }
// Inlined CExoArrayList<DWORD>::Add (operator new / copy / _free growth; cap 0 -> 10h, else doubled).
void inline_add_dword(void* list, U value) {
    if (rd(list, 4) == rd(list, 8)) {
        const U cap = rd(list, 8) != 0 ? rd(list, 8) * 2 : 0x10;
        void* const old = rp(list, 0);
        wr(list, 8, cap);
        wp(list, 0, live_operator_new(cap * 4));
        for (I i = 0; i < rdi(list, 4); ++i) wr(rp(list, 0), static_cast<U>(i) * 4, rd(old, static_cast<U>(i) * 4));
        if (old != nullptr) live_free(old);
    }
    wr(rp(list, 0), rd(list, 4) * 4, value);
    wr(list, 4, rd(list, 4) + 1);
}
// CExoArrayList<T*>::Add via CExoArrayList::Allocate 005E03C0.
void allocate_add_dword(void* list, U value) {
    if (rd(list, 4) == rd(list, 8)) {
        const U cap = rd(list, 8) != 0 ? rd(list, 8) * 2 : 0x10;
        menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr, cap);
    }
    wr(rp(list, 0), rd(list, 4) * 4, value);
    wr(list, 4, rd(list, 4) + 1);
}
// Class slot accessor used by CSWSCreatureStats (index < class count ? class byte : 0FFh).
inline U8 class_of(void* stats, U8 index) {
    return index < rd8(stats, 0x89) ? rd8(stats, 0xa7 + static_cast<U>(index) * 0x28) : static_cast<U8>(0xff);
}
}  // namespace

// 005CB120 CSWLevelStats::SetSkillRankChange(WORD skill, BYTE value) RET 8
extern "C" void __fastcall kotor_fastcall_005cb120(void* self, void*, U skill, U value) {
    const U16 count = rd8(rules(), 0xab);                                          // 005CB120..005CB125
    if (static_cast<U16>(skill) >= count) return;                                  // 005CB12D..005CB135 (jae)
    wr8(rp(self, 0x24), static_cast<U16>(skill), static_cast<U8>(value));          // 005CB137..005CB141
}

// 005CB150 CSWLevelStats::AddFeat(WORD feat) RET 4: CExoArrayList<WORD>::AddUnique on +18h
extern "C" void __fastcall kotor_fastcall_005cb150(void* self, void*, U feat) {
    void* const list = at(self, 0x18);
    I n = rdi(self, 0x1c);                                                         // 005CB150
    const U16 f = static_cast<U16>(feat);                                          // 005CB15C
    if (n > 0) {                                                                   // 005CB159..005CB161 (jle)
        U found = 0;
        U cursor = rd(list, 0);
        do {
            if (rd16(ptr(cursor), 0) == f) ++found;                                // 005CB165..005CB16A
            cursor += 2;
        } while (--n != 0);                                                        // 005CB16B..005CB16F
        if (found != 0) return;                                                    // 005CB171..005CB173
    }
    if (rd(list, 4) == rd(list, 8)) {                                              // 005CB175..005CB17B
        const U cap = rd(list, 8) != 0 ? rd(list, 8) * 2 : 0x10;                   // 005CB17D..005CB185
        kotor_m15_array_u16_allocate_004d2fa0(list, nullptr, cap);                 // 005CB18A..005CB18D
    }
    wr16(rp(list, 0), rd(list, 4) * 2, f);                                         // 005CB192..005CB197
    wr(list, 4, rd(list, 4) + 1);                                                  // 005CB19B
}

// 005CB270 CSWLevelStats::~CSWLevelStats()
extern "C" void __fastcall kotor_fastcall_005cb270(void* self, void*) {
    if (rp(self, 0x24) != nullptr) { live_free(rp(self, 0x24)); wr(self, 0x24, 0); }       // 005CB28F..005CB2A9
    if (rp(self, 0x18) != nullptr) { live_free(rp(self, 0x18)); wr(self, 0x18, 0); wr(self, 0x20, 0); } // 005CB2AC..005CB2BF
    wr(self, 0x1c, 0);                                                                     // 005CB2C2
    if (rp(self, 0) != nullptr) { live_free(rp(self, 0)); wr(self, 0, 0); wr(self, 8, 0); }  // 005CB2C5..005CB2D6
    wr(self, 4, 0);                                                                        // 005CB2DC
    void* const second = at(self, 0xc);
    if (rp(second, 0) != nullptr) { live_free(rp(second, 0)); wr(second, 0, 0); wr(second, 8, 0); } // 005CB2DF..005CB2F0
    wr(second, 4, 0);                                                                      // 005CB2F3
    live_free(rp(self, 0x18));                                                             // 005CB2F6..005CB2FF (member +18h dtor)
    wr(self, 0x18, 0);                                                                     // 005CB311
    c2da_destroy_array_006fa852(second, 0xc, 1, reinterpret_cast<void*>(&exo_array_list_destruct_0056b6f0)); // 005CB307..005CB318
    c2da_destroy_array_006fa852(self, 0xc, 1, reinterpret_cast<void*>(&exo_array_list_destruct_0056b6f0));   // 005CB31D..005CB32F
}

// 0056A560 CSWSMessage::SendServerToPlayerLevelUp_Confirmation(DWORD player, int ok) RET 8
extern "C" U __fastcall kotor_fastcall_0056a560(void* message, void*, U player, U ok) {
    char* const data = static_cast<char*>(live_operator_new(3));                   // 0056A56A..0056A573 / 0056A598..0056A59D
    wr(message, 0x4c, 0xffffffffu);                                                // 0056A57A / 0056A5A4
    if (data == nullptr) return 0;                                                 // 0056A581 / 0056A5AB
    data[0] = 0x50;                                                                // 'P'
    data[1] = 0x1d;
    data[2] = ok != 0 ? 1 : 2;                                                     // 0056A583..0056A58A / 0056A5B4..0056A5BB
    void* const net = vcall<void*>(server(), 0x10);                                // 0056A5C8..0056A5D4 GetNetLayer
    const U result = static_cast<U>(net_layer_send_message_to_player_005d4cb0(net, nullptr, player, data, 3, 0)); // 0056A5D7..0056A5D9
    live_free(data);                                                               // 0056A5DE..0056A5E1
    return result;
}

// 005AF950 CSWSCreatureStats::ValidateLevelUp(CSWLevelStats* level) RET 4
extern "C" U __fastcall kotor_fastcall_005af950(void* stats, void*, void* level) {
    const U can = recovered_can_level_up_005a6810(stats, nullptr);                 // 005AF969
    if (can == 0) return can;                                                      // 005AF96E..005AF981
    if (rd(stats, 0x6c) != 0) {                                                    // 005AF984..005AF989
        void* const module = server_get_module_004ae6b0(server(), nullptr);        // 005AF98B..005AF994
        if (module != nullptr) {                                                   // 005AF99B
            void* const memory = live_operator_new(0x34);                          // 005AF99F..005AF9A1
            void* event = nullptr;
            if (memory != nullptr) {                                               // 005AF9AD..005AF9B7
                try { event = recovered_event_construct_004d7540(memory, nullptr); } // 005AF9B9..005AF9BB (EH state 0)
                catch (...) { live_free(memory); throw; }
            }
            if (event != nullptr) {                                                // 005AF9C4..005AF9CE
                wr16(event, 0, 0x25);                                              // 005AF9D0
                const U target = rd(module, 0x20);                                 // 005AF9D5
                const U source = rd(rp(stats, 0x24), 4);                           // 005AF9D8..005AF9DB
                (void)recovered_event_insert_004afdb0(recovered_server_ai_master_004aed80(server(), nullptr), nullptr, 0, 0,
                                                      source, target, k_event_level_up_007441dc, event); // 005AF9DE..005AF9FB
            }
        }
    }
    kotor_fastcall_005aabf0(stats, nullptr, level, 1);                             // 005AFA01..005AFA0A
    recovered_stats_refresh_005addc0(stats, nullptr);                              // 005AFA0F..005AFA11
    return 1;                                                                      // 005AFA16..005AFA2A
}

// 005AA810 CSWSCreatureStats::AddFeat(WORD feat) RET 4
extern "C" void __fastcall kotor_fastcall_005aa810(void* stats, void*, U feat) {
    void* const info = kotor_rules_get_feat_00550c00(rules(), nullptr, feat);      // 005AA811..005AA820
    if (info == nullptr) return;                                                   // 005AA825..005AA829
    kotor_m15_array_u16_add_unique_005a7670(stats, nullptr, feat);                 // 005AA82F..005AA832
    const U8 uses = rd8(info, 0x33);                                               // 005AA837
    if (!(uses != 0) && static_cast<U>(uses) != 0xffffffffu) return;               // 005AA83C..005AA846 (test/ja; cmp -1 never equal)
    const U16 f = static_cast<U16>(feat);
    U found = 0;                                                                   // 005AA83A edx
    const I size = rdi(stats, 0x10);                                               // 005AA848 (read once)
    U16 i = 0;                                                                     // 005AA84B cx
    for (;;) {
        if (static_cast<I>(i) >= size) break;                                      // 005AA850..005AA855 (jge)
        if (rd16(rp(rp(stats, 0xc), static_cast<U>(i) * 4), 0) == f) found = 1;    // 005AA857..005AA863
        ++i;                                                                       // 005AA868
        if (found != 0) return;                                                    // 005AA869..005AA871
    }
    if (found != 0) return;                                                        // 005AA874..005AA876
    void* const entry = live_operator_new(4);                                      // 005AA878..005AA87A
    wr16(entry, 0, f);                                                             // 005AA884
    wr8(entry, 2, 0);                                                              // 005AA887
    allocate_add_dword(at(stats, 0xc), word(entry));                               // 005AA88B..005AA8B5
}

// 005AABF0 CSWSCreatureStats::LevelUp(CSWLevelStats* level, int record) RET 8
extern "C" void __fastcall kotor_fastcall_005aabf0(void* stats, void*, void* level, U flag) {
    U ebp = 1;                                                                     // 005AABFE
    U8 class_index = 0;                                                            // 005AAC04 [esp+13h]
    (void)vcall<U16>(rp(stats, 0x24), 0x98, 1u);                                   // 005AABF8..005AAC09 GetMaxHitPoints(1) (result unused)
    const U16 old_fp = w1x_creature_max_fp_004fd490(rp(stats, 0x24), nullptr);     // 005AAC0F..005AAC17
    if (flag != 0) allocate_add_dword(at(stats, 0x28), word(level));               // 005AAC1B..005AAC51
    U8 cl = 0;
    if (rd8(stats, 0x89) != 0) {                                                   // 005AAC54..005AAC5E (jbe)
        do {
            const U8 count = rd8(stats, 0x89);                                     // 005AAC60
            const U8 cls = cl < count ? rd8(stats, 0xa7 + static_cast<U>(cl) * 0x28) : static_cast<U8>(0xff); // 005AAC66..005AAC74
            if (cls == rd8(level, 0x2c)) {                                         // 005AAC7B..005AAC82
                ebp = 0;                                                           // 005AAC84
                class_index = cl;                                                  // 005AAC88
                const U8 lvl = cl < count ? rd8(stats, 0xa8 + static_cast<U>(cl) * 0x28) : static_cast<U8>(0); // 005AAC86..005AACA7
                if (cl < count) wr8(stats, 0xa8 + static_cast<U>(cl) * 0x28, static_cast<U8>(lvl + 1)); // 005AACA9..005AACB5
            }
            ++cl;                                                                  // 005AACC2
        } while (cl < rd8(stats, 0x89));                                           // 005AACBC..005AACC6
        if (ebp != 0) goto new_class;                                              // 005AACC8..005AACCA
    } else {
    new_class:
        const U8 bl = static_cast<U8>(rd8(stats, 0x89) + 1);                       // 005AACCC..005AACD2
        const U8 al = static_cast<U8>(bl - 1);                                     // 005AACD6..005AACD8
        wr8(stats, 0x89, bl);                                                      // 005AACDC
        class_index = al;                                                          // 005AACE2
        if (al < bl) wr8(stats, 0xa7 + static_cast<U>(al) * 0x28, rd8(level, 0x2c)); // 005AACDA..005AACF5
        if (al < rd8(stats, 0x89)) wr8(stats, 0xa8 + static_cast<U>(class_index) * 0x28, 1); // 005AAD02..005AAD12
    }
    {
        void* const creature = rp(stats, 0x24);                                    // 005AAD20
        const I hp = static_cast<I>(static_cast<std::int16_t>(rd16(creature, 0xe0))) + static_cast<I>(rd8(level, 0x2b)); // 005AAD23..005AAD2E
        wr(creature, 0xe0, static_cast<U>(hp));                                    // 005AAD30 (dword store over the word field: original quirk)
        wr16(stats, 0x122, static_cast<U16>(rd16(stats, 0x122) + rd8(level, 0x2d))); // 005AAD36..005AAD3B
    }
    // 005AAD42..005AAD4F: ability increase, jump table [005AB1BC] (0 STR .. 5 CHA)
    struct Ability { U count; U mod; U8(__fastcall* get)(void*, void*); bool adjust; };
    static const Ability abilities[6] = {
        {0xe9, 0xea, &recovered_creature_stats_str_005a6190, false},               // 005AAD56
        {0xeb, 0xec, &recovered_creature_stats_dex_005a6550, false},               // 005AAD98
        {0xed, 0xee, &recovered_creature_stats_con_005a6250, false},               // 005AADDA
        {0xef, 0xf0, &recovered_creature_stats_int_005a6310, true},                // 005AAE1C
        {0xf1, 0xf2, &recovered_creature_stats_wis_005a63d0, true},                // 005AAE58
        {0xf3, 0xf4, &recovered_creature_stats_cha_005a6490, true},                // 005AAE94
    };
    const U which = rd8(level, 0x2a);
    if (which <= 5) {
        const Ability& a = abilities[which];
        wr8(stats, a.count, static_cast<U8>(rd8(stats, a.count) + 1));             // inc base score
        const I v = static_cast<I>(a.get(stats, nullptr));                         // Get<Ability>Stat (AL, movzx)
        const I mod = v < 10 ? (v - 0xb) / 2 : (v - 0xa) / 2;                      // cmp al,0Ah / sub / cdq / sub / sar 1
        wr8(stats, a.mod, static_cast<U8>(mod));
        if (a.adjust) w0886x_stats_adjust_spell_uses_005a6d80(stats, nullptr);     // 005AAEC3..005AAEC5
    }
    // 005AAECA..005AAF67: skill ranks
    if (rd8(rules(), 0xab) != 0) {
        U8 bl = 0;
        do {
            if (static_cast<U8>(recovered_skill_rank_change_005cb0f0(level, nullptr, bl)) != 0) { // 005AAEE0..005AAEF0
                std::int8_t cur;                                                          // [esp+20h]
                if (bl >= rd8(rules(), 0xab)) cur = 0;                             // 005AAEF2..005AAF00
                else {
                    const I r = static_cast<I>(static_cast<std::int8_t>(rd8(rp(stats, 0x168), bl))); // 005AAF07..005AAF10
                    cur = r >= 0x7f ? static_cast<std::int8_t>(0x7f) : r <= -0x7f ? static_cast<std::int8_t>(-0x7f) : static_cast<std::int8_t>(r); // 005AAF14..005AAF2B
                }
                const U8 v = static_cast<U8>(static_cast<U8>(recovered_skill_rank_change_005cb0f0(level, nullptr, bl)) +
                                             static_cast<U8>(cur));               // 005AAF30..005AAF3A
                if (bl < rd8(rules(), 0xab)) wr8(rp(stats, 0x168), bl, v);         // 005AAF3E..005AAF55
            }
            ++bl;                                                                  // 005AAF63
        } while (bl < rd8(rules(), 0xab));                                         // 005AAF58..005AAF67
    }
    wr16(stats, 0x164, rd16(level, 0x28));                                         // 005AAF6D..005AAF75
    // 005AAF7C..005AAFA9: feats
    if (rd16(level, 0x1c) != 0) {
        U8 bl = 0;
        do {
            kotor_fastcall_005aa810(stats, nullptr, rd16(rp(level, 0x18), static_cast<U>(bl) * 2)); // 005AAF85..005AAF96
            ++bl;
        } while (static_cast<U16>(bl) < rd16(level, 0x1c));                       // 005AAF9F..005AAFA9
    }
    // 005AAFAB..005AB05C: spells removed (level list +0Ch)
    {
        U8 k = 0;                                                                  // [esp+20h]
        if (rd8(level, 0x10) != 0) {
            do {
                const U spell = rd(rp(level, 0xc), static_cast<U>(k) * 4);         // 005AAFC0..005AAFCA
                if (kotor_m15b_spell_array_get_spell_0059b6d0(rp(rules(), 0x8c), nullptr, static_cast<int>(spell)) != nullptr) {
                    void* const list = at(stats, 0x8c + static_cast<U>(class_index) * 0x28); // 005AAFE2..005AAFF1
                    const I size = rdi(list, 4);
                    I i = 0;
                    bool found = false;
                    if (size > 0) {                                                // 005AAFFA..005AAFFC
                        for (; i < size; ++i)
                            if (rd(rp(list, 0), static_cast<U>(i) * 4) == spell) { found = true; break; } // 005AAFFE..005AB00A
                    }
                    if (found && i >= 0) {                                         // 005AB00E..005AB010 (CExoArrayList::Remove inline)
                        const I n = rdi(list, 4);                                  // 005AB012
                        I j = 0;
                        if (n > 0) {
                            for (; j < size; ++j)
                                if (rd(rp(list, 0), static_cast<U>(j) * 4) == spell) break; // 005AB01B..005AB02A
                        }
                        wr(list, 4, static_cast<U>(n - 1));                        // 005AB02C..005AB031
                        for (; j < n - 1; ) {                                      // 005AB034 / 005AB045
                            wr(rp(list, 0), static_cast<U>(j) * 4, rd(rp(list, 0), static_cast<U>(j) * 4 + 4)); // 005AB036..005AB03F
                            ++j;
                            if (!(j < rdi(list, 4))) break;                        // 005AB041..005AB047
                        }
                    }
                }
                ++k;                                                               // 005AB049..005AB054
            } while (k < rd8(level, 0x10));                                        // 005AB04D..005AB05C
        }
    }
    // 005AB062..005AB17B: spells gained (level list +00h)
    {
        U8 k = 0;
        if (rd8(level, 4) != 0) {
            do {
                const U8 bl = class_index;                                         // 005AB077
                const U spell = rd(rp(level, 0), static_cast<U>(k) * 4);           // 005AB07B..005AB080
                if (bl < rd8(stats, 0x89) &&                                       // 005AB083..005AB089
                    kotor_m15b_spell_array_get_spell_0059b6d0(rp(rules(), 0x8c), nullptr, static_cast<int>(spell)) != nullptr &&
                    (rd(stats, 0x6c) == 0 ||                                       // 005AB0A9..005AB0AE
                     kotor::server_mainloop::creature_force_class_005be4a0(class_of(stats, bl)) != 0)) { // 005AB0B0..005AB0D7
                    void* const list = at(stats, 0x8c + static_cast<U>(bl) * 0x28); // 005AB0DD..005AB0E3
                    const I n = rdi(list, 4);
                    I i = 0;
                    bool present = false;
                    if (n > 0) {                                                   // 005AB0ED..005AB0F1
                        for (; i < n; ++i)
                            if (rd(rp(list, 0), static_cast<U>(i) * 4) == spell) { present = true; break; } // 005AB0F3..005AB0FF
                    }
                    if (!(present && i != -1)) inline_add_dword(list, spell);      // 005AB103..005AB165
                }
                ++k;                                                               // 005AB168..005AB173
            } while (k < rd8(level, 4));                                           // 005AB16C..005AB17B
        }
    }
    {
        void* const creature = rp(stats, 0x24);                                    // 005AB181
        const I hp = static_cast<std::int16_t>(vcall<U16>(creature, 0x98, 1u));    // 005AB184..005AB190 GetMaxHitPoints(1), movsx
        w1x_object_set_hp_004cc280(creature, nullptr, static_cast<U>(hp));         // 005AB193..005AB196
    }
    const U16 new_fp = w1x_creature_max_fp_004fd490(rp(stats, 0x24), nullptr);     // 005AB19B..005AB19E
    wr16(stats, 0x124, static_cast<U16>(rd16(stats, 0x124) + static_cast<U16>(new_fp - old_fp))); // 005AB1A3..005AB1AA
}

// 00527560 CSWSMessage::HandlePlayerToServerLevelUpMessage(CSWSPlayer* player, BYTE minor) RET 8
extern "C" U __fastcall recovered_player_to_server_level_up_00527560(void* message, void*, void* player, U minor) {
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                       // 00527578..00527599
    if (static_cast<U8>(minor) != 0) return 0;                                              // 0052759C..005275A4 -> 0052786D (esi 0)
    void* const creature = vcall<void*>(kotor_fastcall_00561080(player, nullptr), 0x30);    // 005275AA..005275B7 (no null check)
    if (creature == nullptr) return 0;                                                      // 005275BC..005275C6
    void* const memory = live_operator_new(0x30);                                           // 005275CB..005275CD
    void* level = nullptr;
    if (memory != nullptr) {                                                                // 005275D9..005275E3
        try { level = recovered_history_level_ctor_005cb1b0(memory, nullptr); }             // 005275E5..005275E7 (EH state 0)
        catch (...) { live_free(memory); throw; }
    }
    wr8(level, 0x2c, kotor_fastcall_004d6720(message, nullptr, 8));                         // 005275F2..00527603 class
    const U lvl = static_cast<U>(recovered_stats_level_005a5fd0(rp(creature, 0xa74), nullptr, 0)) + 1; // 00527606..00527616
    if (lvl % 4 == 0)                                                                       // 00527617..00527623 (and 80000003h; lvl > 0)
        wr8(level, 0x2a, kotor_fastcall_004d6720(message, nullptr, 8));                     // 00527625..0052762E ability
    {
        void* const row = ptr(rd(rules(), 0xac) + static_cast<U>(rd8(level, 0x2c)) * 0x1a0); // 00527631..00527641
        wr8(level, 0x2b, rd8(row, 0x54));                                                   // 00527647..0052764A hit die
        wr8(level, 0x2d, rd8(row, 0x55));                                                   // 0052764D..00527654 force die
    }
    const U mask = kotor_fastcall_004d6780(message, nullptr, 0x20);                         // 00527650..00527662 ebp
    if (rd8(rules(), 0xab) != 0) {                                                          // 0052765C..0052766E (jbe)
        U8 bl = 0;
        do {
            if ((mask & (1u << (bl & 31u))) != 0) {                                         // 00527670..0052767B (shl masks count)
                const U8 value = kotor_fastcall_004d6720(message, nullptr, 8);              // 0052767D..00527681
                kotor_fastcall_005cb120(level, nullptr, bl, value);                         // 00527686..0052768E
            }
            ++bl;                                                                           // 0052769F
        } while (bl < rd8(rules(), 0xab));                                                  // 00527693..005276A3
    }
    wr16(level, 0x28, kotor_fastcall_004d6750(message, nullptr, 0x10));                     // 005276A5..005276B2 skill points
    {
        U n = kotor_fastcall_004d6720(message, nullptr, 8);                                 // 005276AE..005276BD
        if (n != 0) {
            do kotor_fastcall_005cb150(level, nullptr, kotor_fastcall_004d6750(message, nullptr, 0x10)); // 005276C2..005276CE
            while (--n != 0);                                                               // 005276D3..005276D4
        }
    }
    {
        U n = kotor_fastcall_004d6720(message, nullptr, 8);                                 // 005276D6..005276E1
        if (n != 0) {
            do inline_add_dword(at(level, 0), kotor_fastcall_004d6780(message, nullptr, 0x20)); // 005276F0..00527761
            while (--n != 0);                                                               // 00527758..00527765
        }
    }
    {
        U n = kotor_fastcall_004d6720(message, nullptr, 8);                                 // 00527767..00527772
        if (n != 0) {
            do inline_add_dword(at(level, 0xc), kotor_fastcall_004d6780(message, nullptr, 0x20)); // 00527780..005277F3
            while (--n != 0);                                                               // 005277EA..005277F7
        }
    }
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                       // 005277F9..00527804 (level leaks: original)
    if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                       // 0052780A..00527815 (level leaks: original)
    const U ok = kotor_fastcall_005af950(rp(creature, 0xa74), nullptr, level);              // 0052781B..0052782B
    if (ok == 0) {                                                                          // 0052782D..0052782F
        kotor_fastcall_005cb270(level, nullptr);                                            // 00527831..00527833
        live_free(level);                                                                   // 00527838..00527839
    }
    const U result = kotor_fastcall_0056a560(message, nullptr, rd(player, 4), ok);          // 00527841..00527857
    if (recovered_can_level_up_005a6810(rp(creature, 0xa74), nullptr) != 0)                // 00527851..00527860
        recovered_creature_feedback_004ede10(creature, nullptr, 0xb, nullptr);              // 00527862..00527868
    return result;                                                                          // 0052786D..00527881
}
