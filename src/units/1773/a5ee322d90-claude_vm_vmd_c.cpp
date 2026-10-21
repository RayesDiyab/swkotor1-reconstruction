// Lane VM package vmD, part C: NWScript engine-command bodies CSWVirtualMachineCommands::ExecuteCommand* (vectors,
// faction queries, party/area/item setters, journal quest XP, feats, camera mode), reconstructed from the original
// swkotor.exe (read-only, capstone sweep).  Addresses in the comments are original instructions.
//  * Handler: int __thiscall ExecuteCommandX(int routine, int argc) RET 8
//    -> extern "C" int __fastcall kotor_fastcall_<addr>(void* self, void*, int routine, unsigned argc)  (@16).
//  * Return: 0 / -2001 (pop failure) / -2000 (push failure, neg/sbb/and 7D0h/add F830h).
//  * CSWVirtualMachineCommands +8 = OBJECT_SELF id.  [0074666C] byte 0 = CGameObjectArray "found".
//  * Missing callees reconstructed here (no C-linkage provider in the c31 map), section at the end:
//      00506A10 CSWSArea::GetClientArea, 00506AE0 CSWSArea::SetFogColor, 005ED5C0 CClientExoApp::GetAreaByGameObjectID,
//      005F11D0 CClientExoAppInternal::GetAreaByGameObjectID, 00605690 CSWCArea::AttachmentFromServerArea,
//      005BEA80 CSWSFaction::GetWeakestMember, 005BEE70 CSWSFaction::GetLeastDamagedMember,
//      005BF1A0 CSWSFaction::GetAverageXP, 005BF020 CSWSFaction::GetAverageReputation,
//      004ECEE0 CSWSCreature::UseFeat, 004CE680 CSWSObject::GetHasFeatEffectApplied,
//      0056C6D0 CSWSMessage::SendServerToPlayerCamera_SetMode.
#include "061872cb05-claude_vm_actions_common.hpp"

namespace menu_resources { struct Ref { char bytes[16]; Ref* __thiscall construct_00406d80(const char*); }; } // CResRef(char*) [0030]
namespace w2 { std::uint32_t ftol2_006fae8c(double value); }                                   // __ftol2 low dword [w2_ftol_native]

extern "C" {
// ---- server app / objects ----
void* __fastcall kotor_fastcall_004ae780(void* server, void*, U id);                        // GetAreaByGameObjectID [0530]
void* __fastcall recovered_placeable_lookup_004ae7a0(void* server, void*, U id);            // GetPlaceableByGameObjectID [0886]
U __fastcall kotor_fastcall_004aea40(void* server, void*);                                  // GetPlayerCreatureId [0595]
unsigned __fastcall recovered_load_from_save_game_004af050(void* server, void*);            // GetLoadFromSaveGame [w2_party_native]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                // GetSWSMessage [creature_destructor_r2]
void* __fastcall combat_reuse_faction_00513fc0(void* creature, void*);                      // CSWSCreature::GetFaction [w1_native_b7]
void __fastcall w2x_faction_add_member_005bfa70(void* faction, void*, U id, U flag);        // CSWSFaction::AddMember [0886]
U __fastcall w5_object_add_use_object_action_0057c810(void* object, void*, U target_id);    // AddUseObjectAction RET 4 [1206-14]
U __fastcall recovered_client_area_set_fog_color_00605090(void* area, void*, const void* colour); // CSWCArea::SetFogColor [1204-01]
void* __fastcall kotor_fastcall_0052b3b0(void* manager, void*, I faction);                  // CFactionManager::GetFaction [claude_vm_actions_world]
unsigned char __fastcall recovered_stats_level_005a5fd0(void* stats, void*, U arg);         // CSWSCreatureStats::GetLevel RET 4 [0963]
U __fastcall recovered_stats_has_feat_005a6630(void* stats, void*, U feat);                 // CSWSCreatureStats::HasFeat RET 4 [0963]
void* __fastcall kotor_rules_get_feat_00550c00(void* rules, void*, unsigned short feat);    // CSWRules::GetFeat RET 4 [0428]
unsigned __fastcall kotor_fastcall_004d38b0(void*, void*, unsigned, unsigned, unsigned, unsigned, unsigned); // CSWSCombatRound::AddAttackAction RET 14h [round_queue]
I __fastcall recovered_object_get_reputation_0057cb80(void* object, void*, U id, U flag);   // CSWSObject::GetReputation RET 8 [w1_native_b7]
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U a, U b);            // CSWMessage::CreateWriteMessage RET 0Ch [w2_message_native]
void __fastcall kotor_fastcall_004d62c0(void* message, void*, U value, U bits);             // CSWMessage::WriteBYTE RET 8 [w2_message_native]
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);           // CSWMessage::GetWriteMessage RET 8 [w2_message_native]
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage RET 14h
U __cdecl aurora_vector_equal_004aa980(const void* left, const void* right);                // operator==(Vector, Vector) [0446]
// ---- GFF ----
void* __fastcall gui_load_gff_construct_00410630(void* gff, void*, unsigned type, const char* ext, const void* ref); // CResGFF::CResGFF [0289]
U __fastcall kotor_fastcall_00411240(void* gff, void*, void* out_struct);                   // CResGFF::GetTopLevelStruct [claude_dialogue_start]
U __fastcall gff_list_004118c0(void*, void*, void* list, void* s, const char* label);       // CResGFF::GetList [0291]
U __fastcall gff_list_count_00411940(void*, void*, void* list);                             // CResGFF::GetListCount [0291]
U __fastcall gff_list_element_00411990(void*, void*, void* out, void* list, U index);       // CResGFF::GetListElement [0291]
void* __fastcall gui_load_gff_string_00411ec0(void*, void*, void* out, void* s, const char* label, U* ok, const void* d); // ReadFieldCExoString [0292]
U __fastcall gui_load_gff_dword_00411c20(void*, void*, void* s, const char* label, U* ok, U d); // ReadFieldDWORD [0292]
void* __fastcall frame_LogStringCtor_005e5a90(void* string, void*, const char* text);       // CExoString(char const*) [0459]
U __fastcall async_string_equal_005e5200(void* string, void*, void* other);                 // CExoString::operator== [0188]
// ---- globals ----
extern void* kotor_cell_rules_007a3a28;                                                     // [007A3A28] CSWRules* [0421]
// ---- defined in this file (missing callees) ----
void* __fastcall kotor_fastcall_00506a10(void* area, void*);
void __fastcall kotor_fastcall_00506ae0(void* area, void*, const Vector* colour);
void* __fastcall kotor_fastcall_005ed5c0(void* client, void*, U id);
void* __fastcall kotor_fastcall_005f11d0(void* internal, void*, U id);
void __fastcall kotor_fastcall_00605690(void* client_area, void*, void* server_area);
U __fastcall kotor_fastcall_005bea80(void* faction, void*, U target, U must_see);
U __fastcall kotor_fastcall_005bee70(void* faction, void*, U target, U must_see);
I __fastcall kotor_fastcall_005bf1a0(void* faction, void*);
I __fastcall kotor_fastcall_005bf020(void* faction, void*, U target);
U __fastcall kotor_fastcall_004ecee0(void* creature, void*, U feat, U a2, U target, U a4);
U __fastcall kotor_fastcall_004ce680(void* object, void*, U feat);
U __fastcall kotor_fastcall_0056c6d0(void* message, void*, void* player, U mode);
}

using namespace vmx;

namespace {
inline U self_id(void* self) { return at<U>(self, 8); }
inline U rd(const void* p, U off) { return *reinterpret_cast<const U*>(static_cast<const char*>(p) + off); }
inline void* rp(const void* p, U off) { return ptr(rd(p, off)); }
inline void* sapp() { return server_app(); }
inline int push_result(U ok) { return ok ? 0 : k_push_failed; }          // neg/sbb/and 7D0h/add F830h
inline void* creature(U id) { return kotor_fastcall_004ae770(sapp(), nullptr, id); }
// [[[7A39FC]+8]+4] CServerExoAppInternal, +10054h CFactionManager (both null-checked by the callers)
inline void* faction_manager_or_null() {
    void* internal = rp(sapp(), 4);
    if (internal == nullptr) return nullptr;
    return rp(internal, 0x10054);
}
struct LocalString {                         // CExoString on the handler frame
    ExoString s;
    LocalString() { exo_default(&s); }
    explicit LocalString(const char* text) { (void)frame_LogStringCtor_005e5a90(&s, nullptr, text); }
    LocalString(const LocalString&) = delete;
    LocalString& operator=(const LocalString&) = delete;
    ~LocalString() { exo_destroy(&s); }
};
struct GffList { U s; char label[16]; };     // CResList (20 bytes)
static_assert(sizeof(GffList) == 20, "CResList");
struct ResRef { char bytes[16]; };

constexpr const char* k_global_007486d0 = "Global";
constexpr const char* k_jrl_007486c8 = "JRL ";
constexpr const char* k_categories_007486bc = "Categories";
constexpr const char* k_empty_0073d71c = "";
constexpr const char* k_tag_00747a28 = "Tag";
constexpr const char* k_xp_007451d4 = "XP";
}  // namespace

extern "C" {

// ===================================================================================================== 00545390
// vector VectorNormalize(vector vVector) [routine 137]
int __fastcall kotor_fastcall_00545390(void*, void*, int, unsigned) {
    Vector v{0, 0, 0}, zero_result{0, 0, 0}, scratch;                                    // 0054539F..005453C7
    if (!recovered_vm_outer_pop_vector_005d1040(vm(), nullptr, &v)) return k_pop_failed;  // 005453CF..E1
    scratch.x = 0; scratch.y = 0; scratch.z = 0;                                          // 005453EE..FE
    Vector out;
    if (aurora_vector_equal_004aa980(&v, &scratch) == 0) {                                // 00545406..10
        const Vector* n = normalize_004ab180(&scratch, &v);                              // 00545412..2C (out = scratch)
        out = *n;
    } else {
        out = zero_result;                                                               // 0054542E..36
    }
    return push_result(recovered_vm_outer_push_vector_005d1050(vm(), nullptr, out));    // 0054543A..64
}

// ===================================================================================================== 00545120
// vector Vector(float x=0.0f, float y=0.0f, float z=0.0f) [routine 142] (argc unchecked: always three pops)
int __fastcall kotor_fastcall_00545120(void*, void*, int, unsigned) {
    Vector v{0, 0, 0};                                                                   // 0054512E..3E
    if (!recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &v.x)) return k_pop_failed; // 00545146..4D
    if (!recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &v.y)) return k_pop_failed; // 0054514F..61
    if (!recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &v.z)) return k_pop_failed; // 00545163..75
    return push_result(recovered_vm_outer_push_vector_005d1050(vm(), nullptr, v));      // 00545177..AC
}

// ===================================================================================================== 0053A5C0
// object GetItemPossessor(object oItem) [routine 29]; a container item's possessor is followed one level up
int __fastcall kotor_fastcall_0053a5c0(void*, void*, int, unsigned) {
    U id = 0;
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id)) return k_pop_failed; // 0053A5C9..DF
    U result = k_invalid;                                                                // 0053A5F1 [00746644]
    void* object = nullptr;
    if (lookup(id, &object) && vcall<void*>(object, 0x38) != nullptr)                    // 0053A5F7..1C AsSWSItem
        result = rd(vcall<void*>(object, 0x38), 0x268);                                  // 0053A61E..27
    void* possessor = kotor_fastcall_004ae750(sapp(), nullptr, result);                  // 0053A62D..3C
    if (possessor != nullptr && vcall<void*>(possessor, 0x38) != nullptr)                // 0053A63E..4B
        result = rd(vcall<void*>(possessor, 0x38), 0x268);                               // 0053A64D..54
    return push_result(recovered_vm_outer_push_object_005d10d0(vm(), nullptr, result));  // 0053A65A..79
}

// ===================================================================================================== 0052DF40
// void AddToParty(object oPC, object oPartyLeader) [routine 572]
int __fastcall kotor_fastcall_0052df40(void*, void*, int, unsigned) {
    U pc = 0, leader = 0;
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &pc)) return k_pop_failed;     // 0052DF49..55
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &leader)) return k_pop_failed; // 0052DF57..73
    void* lead = creature(leader);                                                       // 0052DF76..85
    void* member = creature(pc);                                                         // 0052DF8A..9A
    if (lead != nullptr && rd(lead, 0x9d4) != 0 && member != nullptr && rd(member, 0x9d4) != 0) { // 0052DF9F..B9
        void* faction = combat_reuse_faction_00513fc0(lead, nullptr);                    // 0052DFBB..C4
        if (faction != nullptr) w2x_faction_add_member_005bfa70(faction, nullptr, pc, 0); // 0052DFC6..CF
    }
    return 0;                                                                            // 0052DFD4
}

// ===================================================================================================== 0052CC70
// void ActionInteractObject(object oPlaceable) [routine 329]
int __fastcall kotor_fastcall_0052cc70(void* self, void*, int, unsigned) {
    U id = 0;
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id)) return k_pop_failed; // 0052CC72..8F
    void* placeable = recovered_placeable_lookup_004ae7a0(sapp(), nullptr, id);          // 0052CC92..A6
    if (placeable != nullptr) {                                                          // 0052CCA8..AA
        void* actor = creature(self_id(self));                                           // 0052CCAC..BD
        if (actor != nullptr) (void)w5_object_add_use_object_action_0057c810(actor, nullptr, rd(placeable, 4)); // 0052CCC1..C7
    }
    return 0;                                                                            // 0052CCCD
}

// ===================================================================================================== 00546D30
// void SetStealthXPEnabled(int bEnabled) [routine 482]; the player creature is fetched before the pop
int __fastcall kotor_fastcall_00546d30(void*, void*, int, unsigned) {
    void* server = sapp();
    void* player = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 00546D31..44
    I value = 0;
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &value)) return k_pop_failed; // 00546D49..66
    if (player != nullptr) {                                                             // 00546D69..6B
        void* area = recovered_object_area_004cb120(player, nullptr);                    // 00546D6D..76
        if (area != nullptr) at<I>(area, 0x2c0) = value;                                 // 00546D78..7C
    }
    return 0;
}

// ===================================================================================================== 005469A0
// void SetItemNonEquippable(object oItem, int bNonEquippable) [routine 266]: bit 6 of item +288h
int __fastcall kotor_fastcall_005469a0(void*, void*, int, unsigned) {
    U id = 0; I flag = 0;
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id)) return k_pop_failed;    // 005469A9..B5
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &flag)) return k_pop_failed; // 005469B7..D3
    void* item = recovered_item_lookup_004ae760(sapp(), nullptr, id);                    // 005469D6..E4
    if (item != nullptr) {                                                               // 005469E9..EB
        const U old = at<U>(item, 0x288);
        at<U>(item, 0x288) = old ^ (((static_cast<U>(flag) << 6) ^ old) & 0x40u);        // 005469ED..01
    }
    return 0;
}

// ===================================================================================================== 005367B0
// int GetAreaUnescapable() [routine 15]; quirk: no null checks on the player creature or its area
int __fastcall kotor_fastcall_005367b0(void*, void*, int, unsigned) {
    void* server = sapp();
    void* player = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 005367B0..C3
    void* area = recovered_object_area_004cb120(player, nullptr);                        // 005367C8..CA
    return push_result(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, at<I>(area, 0x2ac))); // 005367CF..F0
}

// ===================================================================================================== 00539820
// object GetInventoryDisturbItem() [routine 353]
int __fastcall kotor_fastcall_00539820(void* self, void*, int, unsigned) {
    U result = k_invalid;                                                                // 0053982F [00746644]
    void* object = kotor_fastcall_004ae750(sapp(), nullptr, self_id(self));              // 00539820..36
    if (object != nullptr) {                                                             // 0053983B..3D
        void* as = vcall<void*>(object, 0x10);                                           // 0053983F..43
        if (as != nullptr) result = rd(as, 0x174);                                       // 00539846..4A
    }
    return push_result(recovered_vm_outer_push_object_005d10d0(vm(), nullptr, result));  // 00539850..6B
}

// ===================================================================================================== 0053B730
// int GetLoadFromSaveGame() [routine 251]
int __fastcall kotor_fastcall_0053b730(void*, void*, int, unsigned) {
    const U value = recovered_load_from_save_game_004af050(sapp(), nullptr);             // 0053B730..38
    return push_result(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, static_cast<I>(value))); // 0053B73D..57
}

// ===================================================================================================== 005478B0
// void SetAreaFogColor(object oArea, float fRed, float fGreen, float fBlue) [routine 746]
int __fastcall kotor_fastcall_005478b0(void*, void*, int, unsigned) {
    U id = 0;
    Vector colour{0, 0, 0};                                                              // 005478BE..CE
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id)) return k_pop_failed;     // 005478D6..DD
    if (!recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &colour.x)) return k_pop_failed; // 005478DF..F1
    if (!recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &colour.y)) return k_pop_failed; // 005478F3..05
    if (!recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &colour.z)) return k_pop_failed; // 00547907..23
    void* area = kotor_fastcall_004ae780(sapp(), nullptr, id);                           // 00547926..35
    if (area != nullptr) {                                                               // 0054793A..3E
        kotor_fastcall_00506ae0(area, nullptr, &colour);                                 // 00547940..47
        void* client_area = kotor_fastcall_00506a10(area, nullptr);                      // 0054794C..4E
        if (client_area != nullptr)                                                      // 00547953..55
            (void)recovered_client_area_set_fog_color_00605090(client_area, nullptr, &colour); // 00547957..5E
    }
    return 0;                                                                            // 00547963
}

// ===================================================================================================== 0053A680
// int GetJournalQuestExperience(string szPlotID) [routine 384]: "XP" of the Categories entry whose Tag matches,
// read from a fresh CResGFF("Global", JRL).
// Quirks kept: a failed operator new(A0h) leaves a null CResGFF that is dereferenced (+94h); when the GFF has no
// resource (+94h == 0) the CResGFF object is leaked (the delete at 0053A824 is skipped).
int __fastcall kotor_fastcall_0053a680(void*, void*, int, unsigned) {
    LocalString plot;                                                                    // 0053A6A2..A6 [esp+24h]
    U xp = 0;                                                                            // 0053A6BC [esp+18h]
    U ok = 1;                                                                            // 0053A6C0 [esp+10h]
    if (!recovered_vm_outer_pop_string_005d1080(vm(), nullptr, &plot.s)) return k_pop_failed; // 0053A6C8..E7
    void* memory = live_operator_new(0xa0);                                              // 0053A6EC..F6
    void* gff = nullptr;
    if (memory != nullptr) {                                                             // 0053A6FF..06
        ResRef ref;
        (void)reinterpret_cast<menu_resources::Ref*>(&ref)->construct_00406d80(k_global_007486d0); // 0053A708..11
        gff = gui_load_gff_construct_00410630(memory, nullptr, 0x808, k_jrl_007486c8, &ref); // 0053A716..2C
    }
    if (*static_cast<volatile U*>(static_cast<void*>(static_cast<char*>(gff) + 0x94)) != 0) { // 0053A732..3D
        U top = 0;
        (void)kotor_fastcall_00411240(gff, nullptr, &top);                               // 0053A743..4A
        GffList categories{};
        (void)gff_list_004118c0(gff, nullptr, &categories, &top, k_categories_007486bc); // 0053A74F..60
        const I count = static_cast<I>(gff_list_count_00411940(gff, nullptr, &categories)); // 0053A765..73
        for (I index = 0; index < count; ++index) {                                      // 0053A777, 0053A7F9..00 (signed)
            U element = 0;
            (void)gff_list_element_00411990(gff, nullptr, &element, &categories, static_cast<U>(index)); // 0053A780..8D
            U equal = 0;
            {
                LocalString fallback(k_empty_0073d71c);                                  // 0053A792..9B [esp+2Ch]
                LocalString tag;                                                         // [esp+34h] (hidden return)
                void* read = gui_load_gff_string_00411ec0(gff, nullptr, &tag.s, &element, k_tag_00747a28, &ok, &fallback.s); // 0053A7A0..C2
                equal = async_string_equal_005e5200(read, nullptr, &plot.s);             // 0053A7C7..D3
            }                                                                            // 0053A7D8..F0 (tag, then fallback)
            if (equal != 0) {                                                            // 0053A7F5..F7
                xp = gui_load_gff_dword_00411c20(gff, nullptr, &element, k_xp_007451d4, &ok, 0); // 0053A808..20
                break;
            }
        }
        vcall<void>(gff, 0, 1u);                                                         // 0053A824..2A scalar deleting dtor
    }
    return push_result(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, static_cast<I>(xp))); // 0053A82C..5D
}

// ===================================================================================================== 00538A40
// object GetFactionWeakestMember(object oFactionMember=OBJECT_SELF, int bMustBeVisible=TRUE) [routine 181]
// Quirk: the CFactionManager::GetFaction result is not null-checked.
int __fastcall kotor_fastcall_00538a40(void*, void*, int, unsigned argc) {
    U member = k_invalid;                                                                // 00538A54
    U result = k_invalid;                                                                // 00538A4E (esi)
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &member)) return k_pop_failed; // 00538A58..6A
    I must_see = 1;                                                                      // 00538A72
    if (argc == 2 &&                                                                     // 00538A6D..7A
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &must_see)) return k_pop_failed; // 00538A7C..8E
    void* c = creature(member);                                                          // 00538A90..9E
    if (c != nullptr) {                                                                  // 00538AA3..A5
        const I faction_id = at<I>(rp(c, 0xa74), 0x78);                                  // 00538AA7..BB
        void* manager = faction_manager_or_null();                                       // 00538AAD..C8
        if (manager != nullptr) {
            void* faction = kotor_fastcall_0052b3b0(manager, nullptr, faction_id);       // 00538ACA..D7
            result = kotor_fastcall_005bea80(faction, nullptr, member, static_cast<U>(must_see)); // 00538ADC..E3
        }
    }
    return push_result(recovered_vm_outer_push_object_005d10d0(vm(), nullptr, result));  // 00538AE5..03
}

// ===================================================================================================== 005385A0
// object GetFactionLeastDamagedMember(object oFactionMember=OBJECT_SELF, int bMustBeVisible=TRUE) [routine 184]
int __fastcall kotor_fastcall_005385a0(void*, void*, int, unsigned argc) {
    U member = k_invalid;                                                                // 005385B4
    U result = k_invalid;                                                                // 005385AE (esi)
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &member)) return k_pop_failed; // 005385B8..CA
    I must_see = 1;                                                                      // 005385D2
    if (argc == 2 &&                                                                     // 005385CD..DA
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &must_see)) return k_pop_failed; // 005385DC..EE
    void* c = creature(member);                                                          // 005385F0..FE
    if (c != nullptr) {                                                                  // 00538603..05
        const I faction_id = at<I>(rp(c, 0xa74), 0x78);                                  // 00538607..1B
        void* manager = faction_manager_or_null();                                       // 0053860D..28
        if (manager != nullptr) {
            void* faction = kotor_fastcall_0052b3b0(manager, nullptr, faction_id);       // 0053862A..37
            result = kotor_fastcall_005bee70(faction, nullptr, member, static_cast<U>(must_see)); // 0053863C..43
        }
    }
    return push_result(recovered_vm_outer_push_object_005d10d0(vm(), nullptr, result));  // 00538645..63
}

// ===================================================================================================== 005382E0
// int GetFactionAverageXP(object oFactionMember=OBJECT_SELF) [routine 190]; -1 when no faction is found
int __fastcall kotor_fastcall_005382e0(void*, void*, int, unsigned) {
    I result = -1;                                                                       // 005382ED
    U member = k_invalid;                                                                // 005382F0
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &member)) return k_pop_failed; // 005382F8..08
    void* c = creature(member);                                                          // 0053830B..19
    if (c != nullptr) {                                                                  // 0053831E..20
        const I faction_id = at<I>(rp(c, 0xa74), 0x78);                                  // 00538322..28
        void* manager = faction_manager_or_null();                                       // 0053832B..43
        if (manager != nullptr)
            result = kotor_fastcall_005bf1a0(kotor_fastcall_0052b3b0(manager, nullptr, faction_id), nullptr); // 00538345..54
    }
    return push_result(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, result)); // 00538356..72
}

// ===================================================================================================== 00538230
// int GetFactionAverageReputation(object oSourceFactionMember, object oTarget) [routine 186]; -1 when no faction
int __fastcall kotor_fastcall_00538230(void*, void*, int, unsigned) {
    I result = -1;                                                                       // 0053823F
    U member = 0, target = 0;
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &member)) return k_pop_failed; // 0053823A..54
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &target)) return k_pop_failed; // 00538257..69
    void* c = creature(member);                                                          // 0053826B..79
    if (c != nullptr) {                                                                  // 0053827E..80
        const I faction_id = at<I>(rp(c, 0xa74), 0x78);                                  // 00538282..96
        void* manager = faction_manager_or_null();                                       // 00538288..A3
        if (manager != nullptr)
            result = kotor_fastcall_005bf020(kotor_fastcall_0052b3b0(manager, nullptr, faction_id), nullptr, target); // 005382A5..B9
    }
    return push_result(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, result)); // 005382BB..D9
}

// ===================================================================================================== 0052D8F0
// void ActionUseFeat(int nFeat, object oTarget) [routine 287] -> CSWSCreature::UseFeat(feat, 0, target, area +120h)
int __fastcall kotor_fastcall_0052d8f0(void* self, void*, int, unsigned) {
    I feat = 0; U target = 0;
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &feat)) return k_pop_failed;  // 0052D8F4..08
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &target)) return k_pop_failed; // 0052D90A..27
    void* object = nullptr;
    if (lookup(self_id(self), &object) && vcall<void*>(object, 0x30) != nullptr) {       // 0052D92A..5B AsSWSCreature
        void* area = recovered_object_area_004cb120(vcall<void*>(object, 0x30), nullptr); // 0052D95D..6F
        if (area != nullptr) {
            void* area2 = recovered_object_area_004cb120(vcall<void*>(object, 0x30), nullptr); // 0052D971..7C
            const U area_word = rd(area2, 0x120);                                        // 0052D981..8E
            (void)kotor_fastcall_004ecee0(vcall<void*>(object, 0x30), nullptr, static_cast<U>(feat), 0, target, area_word); // 0052D991..A1
        }
    }
    return 0;                                                                            // 0052D9A6
}

// ===================================================================================================== 00539230
// int GetHasFeatEffect(int nFeat, object oObject=OBJECT_SELF) [routine 543]
int __fastcall kotor_fastcall_00539230(void*, void*, int, unsigned) {
    I result = 0;                                                                        // 0053923F
    I feat = 0; U id = 0;
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &feat)) return k_pop_failed; // 0053923A..53
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id)) return k_pop_failed;    // 00539256..68
    void* object = nullptr;
    if (lookup(id, &object) && vcall<void*>(object, 0x10) != nullptr)                    // 0053926A..9C
        result = static_cast<I>(kotor_fastcall_004ce680(object, nullptr, static_cast<U>(feat))); // 0053929E..AC (raw object)
    return push_result(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, result)); // 005392AE..CC
}

// ===================================================================================================== 00542640
// void SetCameraMode(object oPlayer, int nCameraMode) [routine 504]
int __fastcall kotor_fastcall_00542640(void*, void*, int, unsigned) {
    U id = 0; I mode = 0;
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id)) return k_pop_failed;    // 00542649..55
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &mode)) return k_pop_failed; // 00542657..73
    void* player = kotor_fastcall_004ae910(sapp(), nullptr, id);                         // 00542676..85
    if (player != nullptr) {                                                             // 0054268A..8E
        void* message = recovered_server_sws_message_004aedb0(sapp(), nullptr);          // 00542690..99
        if (message != nullptr) (void)kotor_fastcall_0056c6d0(message, nullptr, player, static_cast<U>(mode)); // 0054269E..AA
    }
    return 0;                                                                            // 005426AF
}

// =====================================================================================================
// Missing callees (no callable provider in the c31 map)
// =====================================================================================================

// 00605690 CSWCArea::AttachmentFromServerArea(CSWSArea*) RET 4: +27Ch = server area
void __fastcall kotor_fastcall_00605690(void* client_area, void*, void* server_area) {
    at<void*>(client_area, 0x27c) = server_area;                                         // 00605690..9A
}

// 005F11D0 CClientExoAppInternal::GetAreaByGameObjectID(id) RET 4: object array +14h, type byte +8 == 4 -> vcall +24h
void* __fastcall kotor_fastcall_005f11d0(void* internal, void*, U id) {
    void* object = nullptr;                                                              // 005F11DE
    if (static_cast<unsigned char>(kotor_fastcall_004d8230(rp(internal, 0x14), nullptr, id, &object)) == 0 && // 005F11D5..F1 [0074666C]
        object != nullptr &&                                                             // 005F11F3..F9
        at<unsigned char>(object, 8) == 4)                                               // 005F11FB..04 [00746633]
        return vcall<void*>(object, 0x24);                                               // 005F1206..0C AsSWCArea
    return nullptr;                                                                      // 005F120F..12
}

// 005ED5C0 CClientExoApp::GetAreaByGameObjectID(id) RET 4: tail jump through +4 (internal)
void* __fastcall kotor_fastcall_005ed5c0(void* client, void*, U id) {
    return kotor_fastcall_005f11d0(rp(client, 4), nullptr, id);                          // 005ED5C0..C3
}

// 00506A10 CSWSArea::GetClientArea() RET 0: cached at +2D0h, resolved through the client app once
void* __fastcall kotor_fastcall_00506a10(void* area, void*) {
    if (rd(area, 0x2d0) == 0) {                                                          // 00506A13..1B
        void* client = client_app();                                                     // 00506A1D..23 [[7A39FC]+4]
        const U client_id = kotor_fastcall_005eda50(client, nullptr, rd(area, 0x120));   // 00506A26..2F
        void* client_area = kotor_fastcall_005ed5c0(client, nullptr, client_id);         // 00506A34..37
        at<void*>(area, 0x2d0) = client_area;                                            // 00506A3E
        if (client_area != nullptr) kotor_fastcall_00605690(client_area, nullptr, area); // 00506A45..4A
    }
    return rp(area, 0x2d0);                                                              // 00506A4F..56
}

// 00506AE0 CSWSArea::SetFogColor(const Vector&) RET 4: +8Ch = (B<<16)|(G<<8)|R, each byte = __ftol2(c*255)
void __fastcall kotor_fastcall_00506ae0(void* area, void*, const Vector* colour) {
    const double scale = 255.0;                                                          // [0073EDE4] float 255.0
    const U blue = w2::ftol2_006fae8c(static_cast<double>(colour->z) * scale);           // 00506AE6..F2
    const U green = w2::ftol2_006fae8c(static_cast<double>(colour->y) * scale);          // 00506AF7..04
    const U red = w2::ftol2_006fae8c(static_cast<double>(colour->x) * scale);            // 00506B09..16
    U packed = (blue & 0xffu) << 8;                                                      // 00506B00..02 (xor ebx; mov bh,al)
    packed = (packed | (green & 0xffu)) << 8;                                            // 00506B11..13
    packed |= red & 0xffu;                                                               // 00506B1B..1E
    at<U>(area, 0x8c) = packed;                                                          // 00506B20
}

// 005BEA80 CSWSFaction::GetWeakestMember(target, must_see) RET 8: lowest GetLevel(0) member (strictly below 999);
// with a valid target and must_see the member must have a clear line of sight to the target.
U __fastcall kotor_fastcall_005bea80(void* faction, void*, U target, U must_see) {
    I best = 0x3e7;                                                                      // 005BEA93
    U result = 0x7f000000u;                                                              // 005BEA8E..9B
    const I count = at<I>(faction, 4);
    if (count <= 0) return 0x7f000000u;                                                  // 005BEA8C..A3 (jle, eax preset)
    for (I index = 0; index < at<I>(faction, 4); ++index) {                              // 005BEB88..92 (count re-read, signed)
        void* member = creature(static_cast<U*>(rp(faction, 0))[index]);                 // 005BEAB0..BE
        if (member == nullptr) continue;                                                 // 005BEAC5..C7
        const I level = static_cast<I>(recovered_stats_level_005a5fd0(rp(member, 0xa74), nullptr, 0)); // 005BEACD..DA
        if (level >= best) continue;                                                     // 005BEADD..E1 (jge)
        if (target == 0x7f000000u || must_see == 0) {                                    // 005BEAE7..FC
            best = level; result = static_cast<U*>(rp(faction, 0))[index];               // 005BEBA6..B3
            continue;
        }
        Vector hit{0, 0, 0};                                                             // 005BEB08..18
        U scratch;
        void* other = creature(target);                                                  // 005BEB20..29
        if (other == nullptr || recovered_object_area_004cb120(member, nullptr) == nullptr) continue; // 005BEB2B..38
        if (area_clear_line_of_sight_0050c330(recovered_object_area_004cb120(member, nullptr), nullptr,
                static_cast<char*>(member) + 0x90, static_cast<char*>(other) + 0x90, &hit, &scratch,
                0x7f000000u, 0x7f000000u, 0, 0) == 1) {                                  // 005BEB3A..71
            result = static_cast<U*>(rp(faction, 0))[index];                             // 005BEB73..80
            best = level;
        }
    }
    return result;                                                                       // 005BEB98
}

// 005BEE70 CSWSFaction::GetLeastDamagedMember(target, must_see) RET 8: smallest (vcall 98h(1) - vcall 9Ch(0)) among
// members that are not dead (vcall 94h) and not GetDeadTemp.  Quirk: the member creature is not null-checked.
U __fastcall kotor_fastcall_005bee70(void* faction, void*, U target, U must_see) {
    I best = 0x3e7;                                                                      // 005BEE83
    U result = 0x7f000000u;                                                              // 005BEE7E..8B
    const I count = at<I>(faction, 4);
    if (count <= 0) return 0x7f000000u;                                                  // 005BEE7C..93
    for (I index = 0; index < at<I>(faction, 4); ++index) {                              // 005BEFA3..AD
        void* member = creature(static_cast<U*>(rp(faction, 0))[index]);                 // 005BEEA0..B4
        if (vcall<U>(member, 0x94) != 0) continue;                                       // 005BEEB6..C2 GetDead
        if (recovered_creature_incapacitated_004ef890(member, nullptr) != 0) continue;   // 005BEEC8..D1 GetDeadTemp
        const I max_hp = static_cast<I>(vcall<short>(member, 0x98, 1u));                 // 005BEED7..E9 movsx
        const I cur_hp = static_cast<I>(vcall<short>(member, 0x9c, 0u));                 // 005BEEE3..F2 movsx
        const I damage = max_hp - cur_hp;                                                // 005BEEF5
        if (damage >= best) continue;                                                    // 005BEEF7..FB (jge)
        if (target == 0x7f000000u || must_see == 0) {                                    // 005BEF01..16
            result = static_cast<U*>(rp(faction, 0))[index]; best = damage;              // 005BEFC1..CF
            continue;
        }
        Vector hit{0, 0, 0};                                                             // 005BEF22..32
        U scratch;
        void* other = creature(target);                                                  // 005BEF3A..43
        if (other == nullptr || recovered_object_area_004cb120(member, nullptr) == nullptr) continue; // 005BEF45..52
        if (area_clear_line_of_sight_0050c330(recovered_object_area_004cb120(member, nullptr), nullptr,
                static_cast<char*>(member) + 0x90, static_cast<char*>(other) + 0x90, &hit, &scratch,
                0x7f000000u, 0x7f000000u, 0, 0) == 1) {                                  // 005BEF54..8B
            result = static_cast<U*>(rp(faction, 0))[index];                             // 005BEF8D..97
            best = damage;                                                               // 005BEF9B
        }
    }
    return result;                                                                       // 005BEFB3
}

// 005BF1A0 CSWSFaction::GetAverageXP() RET 0: sum(stats +68h) / count (signed idiv), 0 for an empty faction
I __fastcall kotor_fastcall_005bf1a0(void* faction, void*) {
    I sum = 0;                                                                           // 005BF1A8
    for (I index = 0; index < at<I>(faction, 4); ++index) {                              // 005BF1AA..D6
        void* member = creature(static_cast<U*>(rp(faction, 0))[index]);                 // 005BF1B0..BE
        if (member != nullptr) sum += at<I>(rp(member, 0xa74), 0x68);                    // 005BF1C3..CD
    }
    const I count = at<I>(faction, 4);                                                   // 005BF1D8
    if (count <= 0) return 0;                                                            // 005BF1DB..ED
    return sum / count;                                                                  // 005BF1DF..E3
}

// 005BF020 CSWSFaction::GetAverageReputation(target) RET 4: mean of target->GetReputation(member, 0) clamped 0..100.
// Quirk: the idiv by the member count runs even for a null target or an empty faction (divide error at count 0).
I __fastcall kotor_fastcall_005bf020(void* faction, void*, U target) {
    I sum = 0;                                                                           // 005BF032
    void* object = creature(target);                                                     // 005BF020..39
    if (object != nullptr) {                                                             // 005BF03D
        for (I index = 0; index < at<I>(faction, 4); ++index)                            // 005BF03F..67
            sum += recovered_object_get_reputation_0057cb80(object, nullptr, static_cast<U*>(rp(faction, 0))[index], 0); // 005BF050..5F
    }
    volatile I count = at<I>(faction, 4);                                                // 005BF06D [edi+4]
    I average = sum / count;                                                             // 005BF06A..6D
    if (average < 0) return 0;                                                           // 005BF073..79
    if (average > 0x64) average = 0x64;                                                  // 005BF07C..81
    return average;
}

// 004ECEE0 CSWSCreature::UseFeat(feat, a2, target, a4) RET 10h: follows the successor chain (CFeat +0) while the
// creature has it; feats 8,11,17-20,26,28-31,53,77,81-83,91,92 queue a combat-round attack action (returns 1);
// feats 2, 25, 54 toggle the creature's +8E0h mode word.  Quirk: CSWRules::GetFeat results are not null-checked.
U __fastcall kotor_fastcall_004ecee0(void* creature_object, void*, U feat, U, U target, U) {
    U used = 0;                                                                          // 004ECEF1
    if (recovered_stats_has_feat_005a6630(rp(creature_object, 0xa74), nullptr, feat) == 0) return 0; // 004ECEE8..FA
    U current = feat & 0xffffu;                                                          // 004ECF00
    U chosen;
    for (;;) {
        chosen = current;                                                                // 004ECF0A
        const void* info = kotor_rules_get_feat_00550c00(kotor_cell_rules_007a3a28, nullptr,
                                                         static_cast<unsigned short>(current)); // 004ECF03..0C
        const I next = *static_cast<const I*>(info);                                     // 004ECF11
        if (next <= 0) break;                                                            // 004ECF13..15
        current = static_cast<U>(next);
        if (recovered_stats_has_feat_005a6630(rp(creature_object, 0xa74), nullptr, current) == 0) break; // 004ECF17..25
    }
    const U id = chosen & 0xffffu;                                                       // 004ECF27
    switch (id) {                                                                        // 004ECF2A..39 (index bytes 004ECF98)
    case 8: case 11: case 17: case 18: case 19: case 20: case 26: case 28: case 29: case 30: case 31:
    case 53: case 77: case 81: case 82: case 83: case 91: case 92:
        (void)kotor_fastcall_004d38b0(rp(creature_object, 0x9c8), nullptr, target, 0, id, 0x5dc, 1); // 004ECF40..55
        used = 1;                                                                        // 004ECF5A
        break;
    default:
        break;
    }
    if (id == 2 || id == 0x19 || id == 0x36) {                                           // 004ECF5F..6C
        const unsigned short old = at<unsigned short>(creature_object, 0x8e0);
        at<unsigned short>(creature_object, 0x8e0) =
            static_cast<unsigned short>((old != 0 ? 0u : 0xffffffffu) & chosen);         // 004ECF6E..7D
    }
    return used;                                                                         // 004ECF84..8A
}

// 004CE680 CSWSObject::GetHasFeatEffectApplied(feat) RET 4: any applied effect (+124h array, +128h count) whose +1Ch
// equals CFeat +4 (word).  The original's "== -1" test on the zero-extended word can never hit.
U __fastcall kotor_fastcall_004ce680(void* object, void*, U feat) {
    const void* info = kotor_rules_get_feat_00550c00(kotor_cell_rules_007a3a28, nullptr, static_cast<unsigned short>(feat)); // 004CE680..8E
    if (info == nullptr) return 0;                                                       // 004CE693..98
    const I spell = static_cast<I>(*reinterpret_cast<const unsigned short*>(static_cast<const char*>(info) + 4)); // 004CE69C
    if (spell == -1) return 0;                                                           // 004CE6A0..A3
    const I count = at<I>(object, 0x128);                                                // 004CE6A5
    void** effects = static_cast<void**>(rp(object, 0x124));                             // 004CE6B1
    for (I index = 0; index < count; ++index)                                            // 004CE6AB..C4
        if (at<I>(effects[index], 0x1c) == spell) return 1;                              // 004CE6B7..BC
    return 0;
}

// 0056C6D0 CSWSMessage::SendServerToPlayerCamera_SetMode(CSWSPlayer*, BYTE mode) RET 8: message 10h/02h, one byte
U __fastcall kotor_fastcall_0056c6d0(void* message, void*, void* player, U mode) {
    void* data = nullptr; U size = 0;
    kotor_fastcall_004d68a0(message, nullptr, 1, 0xffffffffu, 1);                        // 0056C6D4..DC
    kotor_fastcall_004d62c0(message, nullptr, mode, 8);                                  // 0056C6E1..EA
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;          // 0056C6EF..08
    return kotor_fastcall_00567e60(message, nullptr, rd(player, 4), 0x10, 2, data, size); // 0056C70B..2C
}

}  // extern "C"
