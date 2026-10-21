// Lane W4 G15 successor (worker menus-b, part 2): the remaining CSWGuiInGameAbilities event handlers and every callee
// they reach that had no rebuilt provider. Transcribed from linear-sweep listings of swkotor.exe; PCs cited per line.
//
//   006AD4B0 CSWGuiInGameAbilities::OnAbilitySelectionChanged(CSWGuiControl*)   thiscall RET 4
//   006ADA70 CSWGuiInGameAbilities::OnFeatsButtonPressed(CSWGuiControl*)        thiscall RET 4
//   006ADAA0 CSWGuiInGameAbilities::OnPowersButtonPressed(CSWGuiControl*)       thiscall RET 4
//   006ADAD0 CSWGuiInGameAbilities::OnSkillsButtonPressed(CSWGuiControl*)       thiscall RET 4
//   006ADC80 CSWGuiInGameAbilities::OnChangeCharacter(CSWGuiControl*)           thiscall RET 4
// Callees reconstructed here:
//   006AD560 CSWGuiInGameAbilities::UpdateView()                                RET 0
//   006ACCE0 CSWGuiInGameAbilities::OnEnterPower(int spell)                     RET 4
//   006AD410 CSWGuiInGameAbilities::OnEnterFeat(int feat)                       RET 4
//   006ABDC0 CSWGuiInGameAbilities::SetDescription(int strref)                  RET 4
//   006ABEC0 CSWGuiInGameAbilities::UpdatePortraits()                           RET 0
//   006ADB00 CSWGuiInGameAbilities::SetCharacter(CSWCCreature*)                 RET 4
//   006ABCE0 CSWGuiInGameAbilities::CreatePowerList()                           RET 0
//   006AD960 CSWGuiInGameAbilities::CreateSkillList()                           RET 0
//   006CE0F0 CSWGuiSkillFlowChart::CreatePowerChart(CSWSCreatureStats*, int)    RET 8
//   006CE370 CSWGuiSkillFlowChart::CreateFeatChart(CSWSCreatureStats*)          RET 4
//   006CD590 CSWGuiSkillFlowChart::BuildSortedPowerList(CExoArrayList<int>*)    RET 4 (ECX unused)
//   006CD7E0 CSWGuiSkillFlowChart::AddPowerSet(int, int, int)                   RET 0Ch
//   005CD800 CSWFeat::GetDescriptionText(CExoString* out)                       RET 4, returns out
//   00647510 CSWSpell::GetSlot(int)                                             RET 4
//   006474D0 CSWSpell::GetPriority(int)                                         RET 4
#include "78d2073e39-gui_panels.hpp"
using namespace w4gui;

namespace kotor_reconstruction { struct StartupString; struct ArrayMutations; }
using SS = kotor_reconstruction::StartupString;
SS* __fastcall GuiConcat005e5d10(SS* self, void*, SS* out, const SS* rhs);                       // CExoString::operator+
namespace kotor_menu_10_19 {
struct String;
struct Tlk { String* __thiscall simple_0041e8f0(String* out, unsigned strref); };                 // CTlkTable::GetSimpleString
}
namespace options_video { struct String; }
namespace client_movies { int __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*); } // operator!=(char*)
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned); } // CExoArrayList::Allocate
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); } // CServerExoApp::GetPartyTable
namespace kotor { namespace server_mainloop { unsigned __cdecl creature_force_class_005be4a0(unsigned cls); } }  // CSWClass::IsJedi (cdecl)
namespace resource_ref_assignment {
menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref* self, void*, menu_resources::Ref* out, const menu_resources::Ref* src); // CResRef::operator=(const CResRef&)
}

extern "C" {
// ---- rules / stats / spells
extern void* kotor_cell_rules_007a3a28;                                                          // [007A3A28] CSWRules*
void* __fastcall kotor_rules_get_feat_00550c00(void* rules, void*, U32 feat);                    // CSWRules::GetFeat (RET 4, word)
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, U32 id);         // CSWSpellArray::GetSpell
U32 __fastcall kotor_fastcall_0059b6a0(void* spell, void*, U32 index);                           // CSWSpell::GetSubRadialSpell
U32 __fastcall recovered_stats_has_feat_005a6630(void* stats, void*, U32 feat);                  // CSWSCreatureStats::HasFeat
U32 __fastcall recovered_stats_has_spell_005a6e70(void* stats, void*, U32 cls, U32 spell, U32 flag); // CSWSCreatureStats::HasSpell
U32 __fastcall kt_get_class_005a4e90(void* stats, void*, U32 index);                              // CSWSCreatureStats::GetClass (AL)
U32 __fastcall w1x_creature_skill_rank_005aa570(void* stats, void*, I32 skill, I32 target, I32 base); // CSWSCreatureStats::GetSkillRank (AL)
U32 __fastcall kotor_m15b_class_is_feat_useable_005be370(void* cls, void*, U32 feat);             // CSWClass::IsFeatUseable
U8 __fastcall kotor_m15b_creature_stats_get_class_00647730(void* stats, void*, U32 index);         // CSWCCreatureStats::GetClass
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);        // CSWCCreature::GetServerCreature
U32 __fastcall c2da_current_get_float_00413430(void* table, void*, I32 row, void* column, float* out); // C2DA::GetFLOATEntry
// ---- client / party
void* __fastcall client_get_player_creature_005ed540(void* app, void*);                          // CClientExoApp::GetPlayerCreature
U32 __fastcall kotor_fastcall_005eda50(void* app, void*, U32 id);                                // ServerToClientObjectId
void* __fastcall kotor_fastcall_005ed8b0(void* app, void*);                                     // CClientExoApp::GetSWParty
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, U32 index);                     // CSWParty::GetCharacter
void* __fastcall kotor_fastcall_00635460(void* party, void*);                                   // CSWParty::GetPlayerCharacter
void* __fastcall recovered_server_info_004aec90(void* server, void*);                            // CServerExoApp::GetServerInfo
U32 __fastcall kotor_fastcall_005636b0(void* table, void*, I32 index);                           // CSWPartyTable::GetIsNPCAvailable
U8 __fastcall kotor_fastcall_005636d0(void* table, void*);                                       // CSWPartyTable::GetNumNPCAvailable
U32 __fastcall recovered_npc_object_00564700(void* table, void*, I32 index, U32, U32);           // CSWPartyTable::GetNPCObject
U32 __fastcall kotor_fastcall_005edf80(void* app, void*, U32, U32, U32);                         // ChangeCharacterToNextLivingPartyMember (claude_equip_handlers.cpp)
int __fastcall show_tutorial_window_005edf40(void* app, void*, U32, U32, U32, U32);              // CClientExoApp::ShowTutorialWindow
void __fastcall kotor_fastcall_0040a140(void* manager, void*, U32 sound);                       // CSWGuiManager::PlayGuiSound
// ---- gui
void __fastcall gui_load_border_params_set_fill_00414c00(void* params, void*, const void* ref, I32) noexcept(false); // SetFillImage
void __fastcall gui_load_image_params_set_image_00415900(void* params, void*, void* ref, U32) noexcept(false); // CSWGuiImageParams::SetImage
void __fastcall gui_load_listbox_clear_items_00419a90(void* listbox, void*);                      // CSWGuiListBox::ClearItems
void* __fastcall kotor_m1_listbox_get_selected_control_0041a5d0(void* listbox, void*);            // CSWGuiListBox::GetSelectedControl
void __fastcall gui_load_listbox_set_selected_0041c040(void* listbox, void*, I32 index, U32 flag); // CSWGuiListBox::SetSelectedControl
void __fastcall kotor_fastcall_0041c1d0(void* listbox, void*, void* list, U32, U32, U32);       // CSWGuiListBox::AddControls
void __fastcall gui_load_event_list_allocate_004186e0(void* list, void*, U32 size) noexcept(false); // CExoArrayList<12-byte>::Allocate
void __fastcall kotor_fastcall_006abd20(void* self, void*, I32 measured);                        // UpdateDescriptionListBox (claude_enter_handlers.cpp)
void __fastcall kotor_m15d_skill_entry_set_skill_006acb40(void* entry, void*, U32 id, U32 type) noexcept(false); // CSWGuiInGameSkillEntry::SetSkill
I32 __fastcall kotor_m15d_skill_flow_hit_check_006cd1c0(void* flow, void*, I32 x, I32 y);         // CSWGuiSkillFlow::SkillHitCheckMouse
void* __fastcall kotor_m15d_skill_flow_construct_006cccc0(void* self, void*) noexcept(false);    // CSWGuiSkillFlow::CSWGuiSkillFlow
void __fastcall kotor_m15d_chart_clear_006cd240(void* chart, void*);                             // CSWGuiSkillFlowChart::ClearChart
void __fastcall kotor_m15d_chart_set_skill_status_all_006cd2e0(void* chart, void*, U32 status);  // SetSkillStatusAll
void __fastcall kotor_m15d_chart_build_sorted_feat_list_006cd960(void* chart, void*, void* list) noexcept(false); // BuildSortedFeatList
void __fastcall kotor_m15d_chart_add_feat_set_006cda80(void* chart, void*, I32, I32, I32) noexcept(false); // AddFeatSet
void __fastcall kotor_m15d_chart_set_selected_skill_006cdc00(void* chart, void*, I32 skill);     // SetSelectedSkill
I32 __fastcall kotor_m15d_chart_set_top_left_006cdd10(void* chart, void*);                       // SetTopLeft
void __fastcall w1x_pointer_list_insert_00616940(void* list, void*, U32 value, U32 index);       // CExoArrayList<>::Insert
// ---- strings / tlk / resources
std::int32_t __fastcall async_string_length_005e5790(void* self, void*);                          // CExoString::GetLength
void* __fastcall kotor_fastcall_005e5bc0(void* self, void*, std::int32_t value) noexcept(false);  // CExoString::CExoString(int)
U32 __fastcall tlk_fetch_0041e550(void* tlk, void*, U32 strref, void* str_res, U32 flag) noexcept(false); // CTlkTable::Fetch
void* __fastcall c2da_default_ref_00405ed0(void* self, void*);                                   // CResRef::CResRef()
menu_resources::Ref* __fastcall unresolved_ref_assign_00406290(menu_resources::Ref*, void*, menu_resources::Ref* out, const char*); // CResRef::operator=(char*)
void __cdecl genuine_x87_ftol_006fae8c();                                                        // 006FAE8C __ftol2 (ST0 -> EDX:EAX)
// ---- .data colours (3 floats each), bound by their rebuilt storage
extern const unsigned char borrowed_exit_color_007a23b4[12];                                      // [007A23B4]
extern const unsigned char borrowed_enter_color_007a23c0[12];                                     // [007A23C0]
extern const unsigned char kotor_m15d_color_007a23d8[12];                                         // [007A23D8]
extern const unsigned char kotor_m15d_color_007a23e4[12];                                         // [007A23E4]
// defined below (forward)
void __fastcall kotor_fastcall_006ad560(void* self, void*);
void __fastcall kotor_fastcall_006acce0(void* self, void*, U32 spell);
void __fastcall kotor_fastcall_006ad410(void* self, void*, U32 feat);
void __fastcall kotor_fastcall_006abdc0(void* self, void*, U32 strref);
void __fastcall kotor_fastcall_006abec0(void* self, void*);
void __fastcall kotor_fastcall_006adb00(void* self, void*, void* creature);
void __fastcall kotor_fastcall_006abce0(void* self, void*);
void __fastcall kotor_fastcall_006ad960(void* self, void*);
void __fastcall kotor_fastcall_006ce0f0(void* chart, void*, void* stats, U32 all);
void __fastcall kotor_fastcall_006ce370(void* chart, void*, void* stats);
void __fastcall kotor_fastcall_006cd590(void* chart, void*, void* list);
void __fastcall kotor_fastcall_006cd7e0(void* chart, void*, U32 a, U32 b, U32 c);
void* __fastcall kotor_fastcall_005cd800(void* feat, void*, void* out);
I32 __fastcall kotor_fastcall_00647510(void* spell, void*, U32 index);
U32 __fastcall kotor_fastcall_006474d0(void* spell, void*, U32 index);
}

namespace {
// .rdata 0073ECD0 (image byte 00): CTlkTable::Fetch's flag at both Fetch sites here.
const unsigned char rdata_0073ecd0 = 0x00;
// .rdata 0073E9AC (image bits 3F000000 = 0.5f): OnEnterPower rounding addend.
const float rdata_0073e9ac = 0.5f;
// .rdata strings (image contents).
const char rdata_0073d71c[] = "";          // 0073D71C
const char rdata_0073d720[] = "%d";        // 0073D720
const char rdata_00748ec0[] = "+%d";       // 00748EC0
const char rdata_0073d5d8[] = " ";         // 0073D5D8
const char rdata_0074bc3c[] = "EvilCost";  // 0074BC3C
const char rdata_0074bc30[] = "GoodCost";  // 0074BC30

void* rules() { return kotor_cell_rules_007a3a28; }
void* spells() { return lp(rules(), 0x8C); }                                                     // [CSWRules+8C] CSWSpellArray*
void* in_game_gui() { return pt(frame_R4_005ed690(client_app(), nullptr)); }                    // [[007A39FC]+4]->GetInGameGui
kotor_menu_10_19::Tlk* tlk() { return static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08); }
SS* ss(void* s) { return static_cast<SS*>(s); }
U32 sub0(void* spell) { return kotor_fastcall_0059b6a0(spell, nullptr, 0); }                      // GetSubRadialSpell(0)
U32 mod10(void* spell) { const U32 base = (sub0(spell) / 10u) * 10u; return sub0(spell) - base; }   // two calls, unsigned /10
I32 div1000(void* spell) { return static_cast<I32>(sub0(spell)) / 1000; }                          // signed /1000 (imul 10624DD3h)

// A stack CExoString destroyed at scope end (normally or while unwinding) once constructed.
struct Str {
    U32 s[2];
    bool live = false;
    Str() = default;
    Str(const Str&) = delete;
    Str& operator=(const Str&) = delete;
    ~Str() { if (live) frame_LogStringDtor_005e5c20(s, nullptr); }
    void born() { live = true; }
    void* p() { return s; }
};
struct Ref16 { unsigned char bytes[16]; };
// CTlkTable::Fetch result {CExoString text; CResRef sound; float length} (28 bytes).
struct StrRes { U32 text[2]; Ref16 sound; U32 length; };
static_assert(sizeof(StrRes) == 28, "STR_RES frame");
// CExoArrayList<T> header {data, count, allocated}.
struct List { void* data; I32 count; I32 allocated; };
static_assert(sizeof(List) == 12, "CExoArrayList");
kotor_reconstruction::ArrayMutations* am(List* l) { return reinterpret_cast<kotor_reconstruction::ArrayMutations*>(l); }

// ---- change-character portraits (same machine code as CSWGuiInGameCharacter, abilities offsets)
struct Layout { U32 a, b, current, left, right; };
constexpr Layout abilities = {0x3860, 0x3A24, 0x3F71, 0x3BE8, 0x3DAC};   // BTN_CHANGE1/2, +3F71, BTN_CHARLEFT/RIGHT
inline U32 slot(U32 btn) { return btn + 0x58; }
inline U32 fill1(U32 btn) { return btn + 0x80; }
inline U32 fill2(U32 btn) { return btn + 0xF4; }
inline U32 flags(U32 btn) { return btn + 0x44; }
void* party_table() { return lane_config_required::GetPartyTable_004aee70(server_app(), nullptr); }
void portrait_fill(void* self, U32 params, void* creature, I32 keep) {
    unsigned char ref[16];
    void* r = vcall<void*, void*, U32>(creature, 0x148, ref, 1);                                 // GetPortrait(out, 1)
    gui_load_border_params_set_fill_00414c00(at(self, params), nullptr, r, keep);
}
void portrait_pair(void* self, U32 btn, void* creature) {
    portrait_fill(self, fill1(btn), creature, 0);
    portrait_fill(self, fill2(btn), creature, 0);
}
std::int8_t current(void* self, const Layout& l) { return static_cast<std::int8_t>(lb(self, l.current)); }
void* npc_candidate(void* self, const Layout& l, I32 i) {
    if (i == current(self, l)) return nullptr;
    if (kotor_fastcall_005636b0(party_table(), nullptr, i) == 0) return nullptr;
    void* app = client_app();
    const U32 object = recovered_npc_object_00564700(party_table(), nullptr, i, 0, 1);
    return pt(frame_R4_005ed5b0(app, nullptr, kotor_fastcall_005eda50(app, nullptr, object)));
}
void set_bit1(void* self, U32 off, U32 on) { sw(self, off, (lw(self, off) & 0xFFFFFFFDu) | ((on & 1u) << 1)); }
void update_candidate(void* self, const Layout& l, I32 i, U32& first, U32& second) {
    void* c = npc_candidate(self, l, i);
    if (c == nullptr) return;
    if (first != 0) { portrait_pair(self, l.b, c); sw(self, slot(l.b), static_cast<U32>(i)); second = 1; }
    else { portrait_pair(self, l.a, c); sw(self, slot(l.a), static_cast<U32>(i)); first = 1; }
}

// Skill of the selected cell of a CSWGuiSkillFlowChart (0 rows -> -1): [[flows + row*4] + col*128h + 178h].
I32 chart_selected_skill(void* chart) {
    if (li(chart, 4) <= 0) return -1;                                                           // jle -> or eax,-1
    void* flow = lp(lp(chart, 0), static_cast<U32>(lb(chart, 0xD)) * 4u);
    return li(flow, static_cast<U32>(lb(chart, 0xC)) * 0x128u + 0x178u);
}
}  // namespace

extern "C" {
// =====================================================================================================
// 005CD800 CSWFeat::GetDescriptionText(CExoString* out) RET 4: tlk(+0Ch) text copied to out.
void* __fastcall kotor_fastcall_005cd800(void* feat, void*, void* out) {
    StrRes res;                                                                                  // [esp+8]
    exo_default(res.text);                                                                       // 005CD81B..005CD827 (state 0)
    auto guard = on_throw([&]() { frame_LogStringDtor_005e5c20(res.text, nullptr); });
    c2da_default_ref_00405ed0(&res.sound, nullptr);                                              // 005CD82C..005CD838 (state 1)
    tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, lw(feat, 0xC), &res, rdata_0073ecd0);   // 005CD83D..005CD85C
    WindowCopy005e5b00(out, nullptr, res.text);                                                  // 005CD861..005CD86C
    guard.armed = false;
    frame_LogStringDtor_005e5c20(res.text, nullptr);                                             // 005CD871..005CD882
    return out;                                                                                  // 005CD88B
}

// 006474D0 CSWSpell::GetPriority(int index) RET 4: sub - (sub / 10) * 10 (unsigned).
U32 __fastcall kotor_fastcall_006474d0(void* spell, void*, U32 index) {
    const U32 base = (kotor_fastcall_0059b6a0(spell, nullptr, index) / 10u) * 10u;                // 006474D3..006474F3
    return kotor_fastcall_0059b6a0(spell, nullptr, index) - base;                                // 006474F0..006474FC
}

// 00647510 CSWSpell::GetSlot(int index) RET 4: (sub - (sub / 1000) * 1000) / 10 (signed second divide).
I32 __fastcall kotor_fastcall_00647510(void* spell, void*, U32 index) {
    const U32 base = (kotor_fastcall_0059b6a0(spell, nullptr, index) / 1000u) * 1000u;            // 00647513..0064752E
    const I32 rest = static_cast<I32>(kotor_fastcall_0059b6a0(spell, nullptr, index) - base);    // 0064752D..0064753D
    return rest / 10;                                                                            // 0064753F..00647552
}

// =====================================================================================================
// 006CD590 CSWGuiSkillFlowChart::BuildSortedPowerList(CExoArrayList<int>* list) RET 4 (ECX unused): every spell
// with +140h == 1 and a name (+8), inserted before the first entry that sorts after it.
void __fastcall kotor_fastcall_006cd590(void*, void*, void* list_ptr) {
    List* list = static_cast<List*>(list_ptr);
    for (I32 id = 0; id < li(spells(), 0); ++id) {                                               // 006CD590..006CD5A9 / 006CD7B4..006CD7C9
        void* sp = kotor_m15b_spell_array_get_spell_0059b6d0(spells(), nullptr, static_cast<U32>(id)); // 006CD5B6..006CD5BC
        if (sp == nullptr || lb(sp, 0x140) != 1 || lw(sp, 8) == 0) continue;                    // 006CD5BE..006CD5D8
        bool inserted = false;
        for (I32 j = 0; j < list->count; ++j) {                                                  // 006CD5DE..006CD5E9 / 006CD75D..006CD76B
            void* other = kotor_m15b_spell_array_get_spell_0059b6d0(spells(), nullptr,
                                                                    lw(list->data, static_cast<U32>(j) * 4u)); // 006CD5F0..006CD602
            if (other == nullptr) continue;                                                      // 006CD607..006CD60B
            bool before = false;
            {
                const I32 pa = static_cast<I32>(mod10(sp));                                      // 006CD611..006CD63C
                const I32 po = static_cast<I32>(mod10(other));                                   // 006CD638..006CD65F
                if (po > pa) before = true;                                                      // 006CD661..006CD663 jg
            }
            if (!before) {
                const U32 pa = mod10(sp);                                                        // 006CD669..006CD694
                const U32 po = mod10(other);                                                     // 006CD690..006CD6B7
                if (po != pa) continue;                                                          // 006CD6B9..006CD6BB
                const I32 ka = div1000(sp);                                                      // 006CD6C1..006CD6E1
                const I32 ko = div1000(other);                                                   // 006CD6DD..006CD6F9
                if (ko < ka) before = true;                                                      // 006CD6FB..006CD6FD jl
                else {
                    const I32 ka2 = div1000(sp);                                                 // 006CD703..006CD723
                    const I32 ko2 = div1000(other);                                              // 006CD71F..006CD73B
                    if (ko2 != ka2) continue;                                                    // 006CD73D..006CD73F
                    const I32 so = kotor_fastcall_00647510(other, nullptr, 0);                   // 006CD741..006CD745
                    const I32 sa = kotor_fastcall_00647510(sp, nullptr, 0);                      // 006CD74A..006CD750
                    if (so > sa) before = true;                                                  // 006CD755..006CD757 jg
                }
            }
            if (before) {
                w1x_pointer_list_insert_00616940(list, nullptr, static_cast<U32>(id), static_cast<U32>(j)); // 006CD785..006CD795
                inserted = true;
                break;
            }
        }
        if (inserted) continue;                                                                  // 006CD79A
        if (list->count == list->allocated)                                                      // 006CD775..006CD77B
            menu_list_growth::allocate_005e03c0(am(list), nullptr,
                                                list->allocated != 0 ? static_cast<unsigned>(list->allocated) * 2u : 0x10u); // 006CD77D..006CD7A4
        sw(list->data, static_cast<U32>(list->count) * 4u, static_cast<U32>(id));                 // 006CD7A9..006CD7AE
        ++list->count;                                                                           // 006CD7B1
    }
}

// 006CD7E0 CSWGuiSkillFlowChart::AddPowerSet(int a, int b, int c) RET 0Ch: new CSWGuiSkillFlow (0x47C) with the
// three spells' icons (+10h), extent {0, 0, F2h, 28h}, appended to the chart.
void __fastcall kotor_fastcall_006cd7e0(void* chart, void*, U32 a, U32 b, U32 c) {
    menu_resources::Ref icon;                                                                    // [esp+24]
    c2da_default_ref_00405ed0(&icon, nullptr);                                                   // 006CD7FE..006CD802
    void* memory = live_operator_new(0x47C);                                                     // 006CD807..006CD811
    void* flow = nullptr;                                                                        // 006CD82D xor ebp,ebp
    if (memory != nullptr) {                                                                     // 006CD818..006CD820
        auto guard = on_throw([&]() { live_free(memory); });                                     // EH state 0
        flow = kotor_m15d_skill_flow_construct_006cccc0(memory, nullptr);                        // 006CD822..006CD829
        guard.armed = false;
    }
    const U32 ids[3] = {a, b, c};                                                                // 006CD82F..006CD84B [esp+14..1C]
    for (U32 k = 0; k < 3; ++k) {                                                                // 006CD850..006CD8B7
        void* sp = kotor_m15b_spell_array_get_spell_0059b6d0(spells(), nullptr, ids[k]);         // 006CD850..006CD860
        menu_resources::Ref discarded;                                                           // [esp+34] / [esp+44]
        if (sp != nullptr)                                                                       // 006CD865..006CD867
            resource_ref_assignment::assign_00406120(&icon, nullptr, &discarded,
                                                     static_cast<const menu_resources::Ref*>(at(sp, 0x10))); // 006CD869..006CD876
        else
            unresolved_ref_assign_00406290(&icon, nullptr, &discarded, rdata_0073d71c);          // 006CD87D..006CD88B
        U8* cell = static_cast<U8*>(flow) + static_cast<U32>(static_cast<U8>(k)) * 0x128u;      // 006CD890..006CD899 (null flow: original fault)
        sw(cell, 0x178, ids[k]);                                                                 // 006CD8A8
        gui_load_image_params_set_image_00415900(cell + 0xF8, nullptr, &icon, 0);                // 006CD89B..006CD8AE
    }
    const I32 extent[4] = {0, 0, 0xF2, 0x28};                                                    // 006CD8BC..006CD8D5 [esp+14]
    vcall<void, const void*>(flow, 0x04, extent);                                                // 006CD8B9..006CD8DD SetExtent
    if (li(chart, 4) == li(chart, 8)) {                                                          // 006CD8E0..006CD8E6 inline growth
        const I32 capacity = li(chart, 8) != 0 ? li(chart, 8) * 2 : 0x10;                        // 006CD8E8..006CD8F5
        void* old = lp(chart, 0);                                                                // 006CD8F5
        sw(chart, 8, static_cast<U32>(capacity));                                                // 006CD8FF
        sw(chart, 0, ad(live_operator_new(static_cast<U32>(capacity) * 4u)));                    // 006CD8F7..006CD90A
        for (I32 i = 0; i < li(chart, 4); ++i) sw(lp(chart, 0), static_cast<U32>(i) * 4u, lw(old, static_cast<U32>(i) * 4u)); // 006CD90F..006CD923
        if (old != nullptr) live_free(old);                                                      // 006CD925..006CD92F
    }
    sw(lp(chart, 0), static_cast<U32>(li(chart, 4)) * 4u, ad(flow));                              // 006CD932..006CD93B
    sw(chart, 4, lw(chart, 4) + 1u);                                                             // 006CD93E..006CD943
}

// 006CE0F0 CSWGuiSkillFlowChart::CreatePowerChart(CSWSCreatureStats* stats, int all) RET 8: one flow per root power
// (sub-radial % 10 == 0) the creature has (or every one when `all`), with up to two successors in the same
// thousand-group and slot: priority 1 (single) and priority 2 (dual).
void __fastcall kotor_fastcall_006ce0f0(void* chart, void*, void* stats, U32 all) {
    List list = {nullptr, 0, 0};                                                                 // [esp+28] (EH state 0)
    auto guard = on_throw([&]() { live_free(list.data); });
    kotor_m15d_chart_clear_006cd240(chart, nullptr);                                             // 006CE124
    kotor_fastcall_006cd590(chart, nullptr, &list);                                              // 006CE129..006CE130
    U32* const data = static_cast<U32*>(list.data);                                              // 006CE135 edi (read once)
    U32 more = 1;                                                                                // [esp+20]
    for (I32 i = 0; more != 0; ++i) {                                                            // 006CE150..006CE15B / 006CE322..006CE33E
        if (i >= list.count) break;
        const U32 root = data[i];                                                                // 006CE161..006CE165 [[esp+10]]
        void* sp = kotor_m15b_spell_array_get_spell_0059b6d0(spells(), nullptr, root);           // 006CE167..006CE173
        if (sp == nullptr) continue;                                                             // 006CE178..006CE17C
        const U32 base = (sub0(sp) / 10u) * 10u;                                                 // 006CE182..006CE1A0 [esp+1C]
        if (static_cast<I32>(sub0(sp) - base) > 0) { more = 0; continue; }                       // 006CE19D..006CE1BB
        if (all == 0 && recovered_stats_has_spell_005a6e70(stats, nullptr, 0, root, 0) == 0) continue; // 006CE1C0..006CE1DE
        I32 single = -1, dual = -1;                                                              // [esp+1C] / [esp+18]
        for (I32 j = i + 1; j < list.count; ++j) {                                               // 006CE1E4..006CE1F5 / 006CE2F7..006CE302
            if (single != -1 && dual != -1) break;                                               // 006CE200..006CE20A
            const U32 next = static_cast<U32*>(list.data)[j];                                    // [edi+ebx*4] (edi re-read each pass)
            if (all == 0 && recovered_stats_has_spell_005a6e70(stats, nullptr, 0, next, 1) == 0) continue; // 006CE210..006CE22B
            void* succ = kotor_m15b_spell_array_get_spell_0059b6d0(spells(), nullptr, next);     // 006CE231..006CE240
            if (succ == nullptr) continue;                                                       // 006CE245..006CE249
            const I32 ga = div1000(sp);                                                          // 006CE24F..006CE26F
            const I32 gs = div1000(succ);                                                        // 006CE26B..006CE287
            if (gs != ga) continue;                                                              // 006CE289..006CE28B
            const I32 slot_s = kotor_fastcall_00647510(succ, nullptr, 0);                        // 006CE28D..006CE291
            const I32 slot_r = kotor_fastcall_00647510(sp, nullptr, 0);                          // 006CE296..006CE29C
            if (slot_s != slot_r) continue;                                                      // 006CE2A1..006CE2A3
            const U32 sbase = (sub0(succ) / 10u) * 10u;                                          // 006CE2A5..006CE2C3
            if (sub0(succ) - sbase == 1u) {                                                      // 006CE2BF..006CE2CF
                single = static_cast<I32>(static_cast<U32*>(list.data)[j]);                      // 006CE2D1..006CE2D8
            } else if (kotor_fastcall_006474d0(succ, nullptr, 0) == 2u) {                        // 006CE2DE..006CE2EA
                dual = static_cast<I32>(static_cast<U32*>(list.data)[j]);                        // 006CE2EC..006CE2F3
            }
        }
        kotor_fastcall_006cd7e0(chart, nullptr, root, static_cast<U32>(single), static_cast<U32>(dual)); // 006CE308..006CE31D
    }
    guard.armed = false;
    live_free(list.data);                                                                        // 006CE344..006CE34D
}

// 006CE370 CSWGuiSkillFlowChart::CreateFeatChart(CSWSCreatureStats* stats) RET 4 (in-game variant): one flow per
// root feat the creature has, with up to two owned-or-useable successors (single / dual prerequisite).
void __fastcall kotor_fastcall_006ce370(void* chart, void*, void* stats) {
    List list = {nullptr, 0, 0};                                                                 // [esp+28] (EH state 0)
    auto guard = on_throw([&]() { live_free(list.data); });
    const U8 cls_id = static_cast<U8>(kt_get_class_005a4e90(stats, nullptr,
                                                            static_cast<U8>(lb(stats, 0x89) - 1u))); // 006CE3A0..006CE3B1
    void* cls = static_cast<U8*>(lp(rules(), 0xAC)) + static_cast<U32>(cls_id) * 0x1A0u;        // 006CE3B6..006CE3CF [esp+20]
    kotor_m15d_chart_clear_006cd240(chart, nullptr);                                             // 006CE3CD..006CE3D3
    kotor_m15d_chart_build_sorted_feat_list_006cd960(chart, nullptr, &list);                     // 006CE3D8..006CE3DF
    const U16* const data = static_cast<const U16*>(list.data);                                  // 006CE3E4 ebx (read once)
    U32 more = 1;                                                                                // [esp+1C]
    for (I32 i = 0; more != 0; ++i) {                                                            // 006CE400..006CE409 / 006CE523..006CE535
        if (i >= list.count) break;
        const U16 root = data[i];                                                                // [ebp] ([esp+10] pointer)
        void* feat = kotor_rules_get_feat_00550c00(rules(), nullptr, root);                      // 006CE40F..006CE41C
        if (feat == nullptr) continue;                                                           // 006CE421..006CE423
        if (lh(feat, 0x38) != 0xFFFFu) { more = 0; continue; }                                   // 006CE429..006CE439
        if (recovered_stats_has_feat_005a6630(stats, nullptr, root) == 0) continue;              // 006CE43E..006CE450
        I32 single = -1, dual = -1;                                                              // [esp+14] / ebp
        for (I32 j = i + 1; j < list.count; ++j) {                                               // 006CE456..006CE463 / 006CE4F7..006CE4FE
            if (single != -1 && dual != -1) break;                                               // 006CE470..006CE47A
            const U16 next = data[j];                                                            // [ebx+esi*2]
            if (recovered_stats_has_feat_005a6630(stats, nullptr, next) == 0) continue;          // 006CE480..006CE492
            void* succ = kotor_rules_get_feat_00550c00(rules(), nullptr, next);                  // 006CE494..006CE4A1
            if (succ == nullptr) continue;                                                       // 006CE4A6..006CE4AA
            if (lh(succ, 0x38) != root) continue;                                                // 006CE4AC..006CE4B7
            if (kotor_m15b_class_is_feat_useable_005be370(cls, nullptr, next) == 0 &&
                recovered_stats_has_feat_005a6630(stats, nullptr, next) == 0) continue;          // 006CE4B9..006CE4DF
            if (lh(succ, 0x3A) == 0xFFFFu) single = next;                                        // 006CE4E1..006CE4ED
            else dual = next;                                                                    // 006CE4F3
        }
        kotor_m15d_chart_add_feat_set_006cda80(chart, nullptr, root, single, dual);              // 006CE504..006CE516
    }
    guard.armed = false;
    live_free(list.data);                                                                        // 006CE53B..006CE544 (ebx)
}

// =====================================================================================================
// 006ABDC0 CSWGuiInGameAbilities::SetDescription(int strref) RET 4.
void __fastcall kotor_fastcall_006abdc0(void* self, void*, U32 strref) {
    StrRes res;                                                                                  // [esp+4]
    exo_default(res.text);                                                                       // 006ABDDB..006ABDDF (state 0)
    auto guard = on_throw([&]() { frame_LogStringDtor_005e5c20(res.text, nullptr); });
    c2da_default_ref_00405ed0(&res.sound, nullptr);                                              // 006ABDE4..006ABDF0 (state 1)
    tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, strref, &res, rdata_0073ecd0);          // 006ABDF5..006ABE15
    gui_load_text_params_set_text_00415e00(at(self, 0x2138), nullptr, res.text);                 // 006ABE1A..006ABE25
    kotor_fastcall_006abd20(self, nullptr, async_string_length_005e5790(res.text, nullptr) > 0 ? 1 : 0); // 006ABE2A..006ABE3D
    guard.armed = false;
    frame_LogStringDtor_005e5c20(res.text, nullptr);                                             // 006ABE42..006ABE4E
}

// 006AD410 CSWGuiInGameAbilities::OnEnterFeat(int feat) RET 4 (feat pointer unchecked, as in the original).
void __fastcall kotor_fastcall_006ad410(void* self, void*, U32 feat_id) {
    void* feat = kotor_rules_get_feat_00550c00(rules(), nullptr, feat_id);                       // 006AD428..006AD438
    {
        Str desc;                                                                                // [esp+0C]
        void* text = kotor_fastcall_005cd800(feat, nullptr, desc.p()); desc.born();              // 006AD43D..006AD446 (state 0)
        gui_load_text_params_set_text_00415e00(at(self, 0x2138), nullptr, text);                 // 006AD44B..006AD45C
        kotor_fastcall_006abd20(self, nullptr, async_string_length_005e5790(text, nullptr) > 0 ? 1 : 0); // 006AD461..006AD472
    }                                                                                            // 006AD477..006AD483
    gui_load_text_params_set_strref_00415e50(at(self, 0x29F8), nullptr, lw(feat, 8));            // 006AD488..006AD492 LBL_NAME
}

// 006ACCE0 CSWGuiInGameAbilities::OnEnterPower(int spell) RET 4 (spell pointer unchecked, as in the original).
void __fastcall kotor_fastcall_006acce0(void* self, void*, U32 spell_id) {
    void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(spells(), nullptr, spell_id);        // 006ACCFE..006ACD15
    I32 bonus = 0;                                                                               // ebx
    const U8 alignment = lb(spell, 0x14F);                                                       // 006ACD22..006ACD28 [esp+13]
    void* server = kotor_m15a_creature_get_server_creature_0060fb20(lp(self, 0x3F74), nullptr);  // 006ACD1A..006ACD2C
    const U8 tier = static_cast<U8>(static_cast<std::int16_t>(lh(lp(server, 0xA74), 0x17E)) / 10); // 006ACD31..006ACD53 [esp+12]
    Str column; exo_default(column.p()); column.born();                                          // 006ACD4F..006ACD57 [esp+3C] (state 0)
    Str bonus_name; exo_default(bonus_name.p()); bonus_name.born();                              // 006ACD5C..006ACD64 [esp+34] (state 1)
    Str bonus_text; exo_default(bonus_text.p()); bonus_text.born();                              // 006ACD69..006ACD72 [esp+2C] (state 2)
    Str prefix; exo_default(prefix.p()); prefix.born();                                          // 006ACD77..006ACD80 [esp+24] (state 3)
    U32 color[3] = {0, 0, 0};                                                                    // 006ACD90..006ACD98 [esp+5C]
    if (alignment == 0x45) GuiAssign005e5140(column.p(), nullptr, rdata_0074bc3c);              // 006ACD85..006ACDB2 'E'
    else if (alignment == 0x47) GuiAssign005e5140(column.p(), nullptr, rdata_0074bc30);         // 006ACDA5..006ACDB2 'G'
    if (client_movies::StringNotEqual_005e5390(static_cast<options_video::String*>(column.p()), nullptr, rdata_0073d71c) != 0) { // 006ACDB7..006ACDC7
        void* table = lp(lp(rules(), 0xB8), 0xD8);                                               // 006ACDC9..006ACDD5
        float cost;                                                                              // [esp+1C] (left as is when the lookup fails)
        c2da_current_get_float_00413430(table, nullptr, static_cast<I32>(tier), column.p(), &cost); // 006ACDDB..006ACDED
        const U32 base = lb(spell, 0x14E);                                                       // 006ACDF2..006ACDF9 [esp+14]
        I32 rounded;
        __asm {
            fild dword ptr base
            fmul dword ptr cost
            fadd dword ptr rdata_0073e9ac
            call genuine_x87_ftol_006fae8c
            mov rounded, eax
        }                                                                                        // 006ACDFD..006ACE0B
        bonus = static_cast<I32>(static_cast<U32>(rounded) - lb(spell, 0x14E));                  // 006ACE10..006ACE19
    }
    if (tier > 5) {                                                                              // 006ACE1B..006ACE21 jbe (unsigned)
        Str t; void* r = tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(t.p()), 0xA3A3); t.born(); // 006ACE23..006ACE38 (state 4)
        async_assign_string_005e5c50(prefix.p(), nullptr, r);                                    // 006ACE5B..006ACE60
    } else if (tier < 5) {                                                                       // 006ACE3F jae
        Str t; void* r = tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(t.p()), 0xA3A2); t.born(); // 006ACE41..006ACE56 (state 5)
        async_assign_string_005e5c50(prefix.p(), nullptr, r);                                    // 006ACE5B..006ACE60
    }                                                                                            // 006ACE65..006ACE6E ~t
    if (bonus < 0) {                                                                             // 006ACE73..006ACE76
        frame_LogFormat_005e5680(bonus_text.p(), rdata_0073d720, bonus);                         // 006ACE7C..006ACE86
        std::memcpy(color, kotor_m15d_color_007a23e4, 12);                                       // 006ACE8B..006ACEB0
        Str space; frame_LogStringCtor_005e5a90(space.p(), nullptr, rdata_0073d5d8); space.born(); // 006ACEA3..006ACEB4 (state 6)
        Str name; void* r = tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(name.p()), 0xBB10); name.born(); // 006ACEB9..006ACED3 (7)
        Str first; GuiConcat005e5d10(ss(prefix.p()), nullptr, ss(first.p()), ss(space.p())); first.born(); // 006ACED7..006ACEEA (8)
        Str whole; GuiConcat005e5d10(ss(first.p()), nullptr, ss(whole.p()), ss(r)); whole.born();    // 006ACEEF..006ACF00 (9)
        async_assign_string_005e5c50(bonus_name.p(), nullptr, whole.p());                        // 006ACF05..006ACF0F
    } else if (bonus > 0) {                                                                      // 006ACF47 jle
        frame_LogFormat_005e5680(bonus_text.p(), rdata_00748ec0, bonus);                         // 006ACF4D..006ACF57
        std::memcpy(color, kotor_m15d_color_007a23d8, 12);                                       // 006ACF5C..006ACF81
        Str space; frame_LogStringCtor_005e5a90(space.p(), nullptr, rdata_0073d5d8); space.born(); // 006ACF74..006ACF85 (state A)
        Str name; void* r = tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(name.p()), 0xBB10); name.born(); // 006ACF8A..006ACFA4 (B)
        Str first; GuiConcat005e5d10(ss(prefix.p()), nullptr, ss(first.p()), ss(space.p())); first.born(); // 006ACFA8..006ACFBB (C)
        Str whole; GuiConcat005e5d10(ss(first.p()), nullptr, ss(whole.p()), ss(r)); whole.born();    // 006ACFC0..006ACFD1 (D)
        async_assign_string_005e5c50(bonus_name.p(), nullptr, whole.p());                        // 006ACFD6..006ACFE0
    } else {
        frame_LogFormat_005e5680(bonus_text.p(), rdata_0073d720, bonus);                         // 006AD015..006AD01F
        std::memcpy(color, borrowed_exit_color_007a23b4, 12);                                    // 006AD024..006AD050
        Str name; void* r = tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(name.p()), 0xBB10); name.born(); // 006AD03C..006AD05E (E)
        async_assign_string_005e5c50(bonus_name.p(), nullptr, r);                                // 006AD059..006AD063
    }                                                                                            // ~whole, ~first, ~name, ~space (006ACF14..006AD071)
    gui_load_text_params_set_text_00415e00(at(self, 0x2638), nullptr, bonus_text.p());           // 006AD076..006AD081 LBL_BONUSVAL
    gui_load_text_params_set_color_00414e10(at(self, 0x2638), nullptr, color);                   // 006AD086..006AD091
    gui_load_text_params_set_text_00415e00(at(self, 0x24F8), nullptr, bonus_name.p());           // 006AD096..006AD0A1 LBL_BONUS
    {
        Str number; kotor_fastcall_005e5bc0(number.p(), nullptr, static_cast<I32>(lb(spell, 0x14E))); number.born(); // 006AD0A6..006AD0B2 (F)
        gui_load_text_params_set_text_00415e00(at(self, 0x23B8), nullptr, number.p());           // 006AD0B7..006AD0C7 LBL_RANKVAL
    }                                                                                            // 006AD0CC..006AD0D5
    {
        Str number;
        void* r = kotor_fastcall_005e5bc0(number.p(), nullptr,
                                          static_cast<I32>(lb(spell, 0x14E) + static_cast<U32>(bonus))); number.born(); // 006AD0DA..006AD0E8 (10h)
        gui_load_text_params_set_text_00415e00(at(self, 0x28B8), nullptr, r);                    // 006AD0ED..006AD0F9 LBL_TOTALVAL
    }                                                                                            // 006AD0FE..006AD107
    gui_load_text_params_set_strref_00415e50(at(self, 0x29F8), nullptr, lw(spell, 8));           // 006AD10C..006AD116 LBL_NAME
    kotor_fastcall_006abdc0(self, nullptr, lw(spell, 0xC));                                      // 006AD11B..006AD121
}                                                                                                // 006AD126..006AD15C ~prefix ~bonus_text ~bonus_name ~column

// 006AD560 CSWGuiInGameAbilities::UpdateView() RET 0.
void __fastcall kotor_fastcall_006ad560(void* self, void*) {
    List list = {nullptr, 0, 0};                                                                 // [esp+18] (EH state 0)
    auto guard = on_throw([&]() { live_free(list.data); });
    {
        Str empty; frame_LogStringCtor_005e5a90(empty.p(), nullptr, rdata_0073d71c); empty.born(); // 006AD58C..006AD599 (state 1)
        gui_load_text_params_set_text_00415e00(at(self, 0x2138), nullptr, empty.p());            // 006AD59E..006AD5AE
        kotor_fastcall_006abd20(self, nullptr, async_string_length_005e5790(empty.p(), nullptr) > 0 ? 1 : 0); // 006AD5B3..006AD5C6
    }                                                                                            // 006AD5CB..006AD5D3
    static const U32 info[7] = {0x2A94, 0x21D4, 0x2314, 0x2454, 0x2594, 0x26D4, 0x2814};         // LBL_INFOBG, SKILLRANK..TOTALVAL +44
    void* listbox = at(self, 0x30DC);
    U32 tutorial = 0;
    switch (lb(in_game_gui(), 0xBC0)) {                                                          // 006AD5D8..006AD5FD
    case 2: {                                                                                    // feats
        gui_load_text_params_set_color_00414e10(at(self, 0x3084), nullptr, borrowed_exit_color_007a23b4);  // 006AD603..006AD60E
        gui_load_text_params_set_color_00414e10(at(self, 0x2CFC), nullptr, borrowed_enter_color_007a23c0); // 006AD613..006AD61E
        gui_load_text_params_set_color_00414e10(at(self, 0x2EC0), nullptr, borrowed_exit_color_007a23b4);  // 006AD623..006AD62E
        for (U32 off : info) sw(self, off, lw(self, off) & 0xFFFFFFFDu);                         // 006AD633..006AD677
        void* chart = at(self, 0x3F88);
        kotor_fastcall_0041c1d0(listbox, nullptr, chart, 0, 0, 0);                               // 006AD666..006AD68C
        gui_load_listbox_set_selected_0041c040(listbox, nullptr, lb(self, 0x3F95), 0);            // 006AD691..006AD69E
        kotor_fastcall_006ad410(self, nullptr, static_cast<U32>(chart_selected_skill(chart)));    // 006AD6A3..006AD6CA
        tutorial = 0x25;                                                                         // 006AD6CF..006AD6DA
        break;
    }
    case 1: {                                                                                    // powers
        gui_load_text_params_set_color_00414e10(at(self, 0x3084), nullptr, borrowed_exit_color_007a23b4);  // 006AD6E1..006AD6EC
        gui_load_text_params_set_color_00414e10(at(self, 0x2CFC), nullptr, borrowed_exit_color_007a23b4);  // 006AD6F1..006AD6FC
        gui_load_text_params_set_color_00414e10(at(self, 0x2EC0), nullptr, borrowed_enter_color_007a23c0); // 006AD701..006AD70C
        gui_load_text_params_set_strref_00415e50(at(self, 0x2278), nullptr, 0x9317);             // 006AD711..006AD71C
        gui_load_text_params_set_strref_00415e50(at(self, 0x2778), nullptr, 0xA3B1);             // 006AD721..006AD72C
        for (U32 off : info) sw(self, off, lw(self, off) | 2u);                                  // 006AD731..006AD775
        void* chart = at(self, 0x3F78);
        kotor_fastcall_0041c1d0(listbox, nullptr, chart, 0, 0, 0);                               // 006AD764..006AD78A
        gui_load_listbox_set_selected_0041c040(listbox, nullptr, lb(self, 0x3F85), 0);            // 006AD78F..006AD79C
        kotor_fastcall_006acce0(self, nullptr, static_cast<U32>(chart_selected_skill(chart)));    // 006AD7A1..006AD7C8
        tutorial = 0x26;                                                                         // 006AD7CD..006AD7D8
        break;
    }
    case 0: {                                                                                    // skills
        gui_load_text_params_set_color_00414e10(at(self, 0x3084), nullptr, borrowed_enter_color_007a23c0); // 006AD7DF..006AD7EA
        gui_load_text_params_set_color_00414e10(at(self, 0x2CFC), nullptr, borrowed_exit_color_007a23b4);  // 006AD7EF..006AD7FA
        gui_load_text_params_set_color_00414e10(at(self, 0x2EC0), nullptr, borrowed_exit_color_007a23b4);  // 006AD7FF..006AD80A
        gui_load_text_params_set_strref_00415e50(at(self, 0x2278), nullptr, 0x62B);              // 006AD80F..006AD81A
        gui_load_text_params_set_strref_00415e50(at(self, 0x24F8), nullptr, 0x7D81);             // 006AD81F..006AD82A
        gui_load_text_params_set_strref_00415e50(at(self, 0x2778), nullptr, 0xA3B0);             // 006AD82F..006AD83A
        for (U32 off : info) sw(self, off, lw(self, off) | 2u);                                  // 006AD83F..006AD870
        I32 n = li(self, 0x68);                                                                  // 006AD876..006AD87E
        if (n >= 10) n = 10;
        for (I32 i = 0; i < n; ++i) {                                                            // 006AD883..006AD8D4
            void* entry = at(self, 0x70 + static_cast<U32>(i) * 0x310u);
            vcall<void, U32>(entry, 0x40, 0);                                                    // 006AD890..006AD895
            if (list.count == list.allocated)                                                    // 006AD898..006AD8A0
                menu_list_growth::allocate_005e03c0(am(&list), nullptr,
                                                    list.allocated != 0 ? static_cast<unsigned>(list.allocated) * 2u : 0x10u); // 006AD8A2..006AD8B4
            sw(list.data, static_cast<U32>(list.count) * 4u, ad(entry));                          // 006AD8B9..006AD8C1
            ++list.count;                                                                        // 006AD8C4..006AD8D0
        }
        kotor_fastcall_0041c1d0(listbox, nullptr, &list, 0, 1, 0);                               // 006AD8D6..006AD8E7
        gui_load_listbox_set_selected_0041c040(listbox, nullptr, lb(self, 0x3F70), 0);            // 006AD8EC..006AD8F7
        tutorial = 0x24;                                                                         // 006AD8FC..006AD907
        break;
    }
    default: break;                                                                              // 006AD5FD -> 006AD917
    }
    if (tutorial != 0)
        show_tutorial_window_005edf40(client_app(), nullptr, tutorial, 0x7F000000u, 0x7F000000u, 0); // 006AD909..006AD912
    vcall<void, void*, U32>(self, 0x08, nullptr, 0);                                             // 006AD917..006AD91D
    vcall<void, void*, U32>(self, 0x08, listbox, 0);                                             // 006AD920..006AD92C
    guard.armed = false;
    live_free(list.data);                                                                        // 006AD92F..006AD93C
}

// 006AD960 CSWGuiInGameAbilities::CreateSkillList() RET 0: +64 list of {CSWSkill*, rank, bonus} and the skill
// entries +70 (stride 310h) for every rules skill (no bound against the 10 entries, as in the original).
void __fastcall kotor_fastcall_006ad960(void* self, void*) {
    const I32 count = lb(rules(), 0xAB);                                                         // 006AD963..006AD969
    sw(self, 0x68, 0);                                                                           // 006AD976
    void* server = kotor_m15a_creature_get_server_creature_0060fb20(lp(self, 0x3F74), nullptr);  // 006AD979..006AD988
    U8* entry = static_cast<U8*>(at(self, 0x70));                                                // 006AD99B [esp+10]
    for (I32 i = 0; i < count; ++i) {                                                            // 006AD98D..006AD995 / 006ADA56..006ADA59
        const U32 skill = lw(rules(), 0xB4) + static_cast<U32>(static_cast<U16>(i)) * 32u;       // 006AD9B0..006AD9C4
        if (skill == 0) continue;                                                                // 006AD9C8
        const I32 rank = static_cast<std::int8_t>(w1x_creature_skill_rank_005aa570(lp(server, 0xA74), nullptr, i, 0, 1)); // 006AD9CE..006AD9E9
        const I32 total = static_cast<std::int8_t>(w1x_creature_skill_rank_005aa570(lp(server, 0xA74), nullptr, i, 0, 0)); // 006AD9DE..006AD9F4
        const I32 bonus = total - rank;                                                          // 006AD9FD
        List* list = static_cast<List*>(at(self, 0x64));
        if (list->count == list->allocated)                                                      // 006AD9F1..006ADA01
            gui_load_event_list_allocate_004186e0(list, nullptr,
                                                  list->allocated != 0 ? static_cast<U32>(list->allocated) * 2u : 0x10u); // 006ADA03..006ADA13
        U8* item = static_cast<U8*>(list->data) + static_cast<U32>(list->count) * 12u;           // 006ADA18..006ADA20
        sw(item, 0, skill); sw(item, 4, static_cast<U32>(rank)); sw(item, 8, static_cast<U32>(bonus)); // 006ADA23..006ADA2C
        ++list->count;                                                                           // 006ADA2F
        kotor_m15d_skill_entry_set_skill_006acb40(entry, nullptr, static_cast<U32>(i), 0);       // 006ADA32..006ADA3B
        entry += 0x310;                                                                          // 006ADA48..006ADA4E
    }
    sb(self, 0x3F70, 0);                                                                         // 006ADA60
}

// 006ABCE0 CSWGuiInGameAbilities::CreatePowerList() RET 0.
void __fastcall kotor_fastcall_006abce0(void* self, void*) {
    if (lp(self, 0x3F74) == nullptr) return;                                                     // 006ABCE3..006ABCEB
    void* server = kotor_m15a_creature_get_server_creature_0060fb20(lp(self, 0x3F74), nullptr);  // 006ABCED
    void* chart = at(self, 0x3F78);
    kotor_fastcall_006ce0f0(chart, nullptr, lp(server, 0xA74), 0);                               // 006ABCF2..006ABD03
    kotor_m15d_chart_set_skill_status_all_006cd2e0(chart, nullptr, 6);                           // 006ABD08..006ABD0C
    kotor_m15d_chart_set_top_left_006cdd10(chart, nullptr);                                      // 006ABD11..006ABD14 (tail jmp)
}

// 006ADB00 CSWGuiInGameAbilities::SetCharacter(CSWCCreature* creature) RET 4.
void __fastcall kotor_fastcall_006adb00(void* self, void*, void* creature) {
    sw(self, 0x3F74, ad(creature));                                                              // 006ADB06..006ADB0C
    if (creature == nullptr) return;                                                             // 006ADB0A..006ADB12
    portrait_fill(self, 0x1F80, creature, 0);                                                    // 006ADB18..006ADB30 LBL_PORTRAIT
    gui_load_listbox_clear_items_00419a90(at(self, 0x30DC), nullptr);                            // 006ADB35..006ADB3B
    if (void* current_creature = lp(self, 0x3F74)) {                                             // 006ADB40..006ADB48
        void* server = kotor_m15a_creature_get_server_creature_0060fb20(current_creature, nullptr); // 006ADB4B
        void* chart = at(self, 0x3F88);
        kotor_fastcall_006ce370(chart, nullptr, lp(server, 0xA74));                              // 006ADB50..006ADB5F
        kotor_m15d_chart_set_skill_status_all_006cd2e0(chart, nullptr, 6);                       // 006ADB64..006ADB68
        kotor_m15d_chart_set_top_left_006cdd10(chart, nullptr);                                  // 006ADB6D..006ADB6F
    }
    kotor_fastcall_006ad960(self, nullptr);                                                      // 006ADB75..006ADB77
    void* stats = lp(lp(self, 0x3F74), 0x2F8);                                                   // 006ADB7C..006ADB82
    const U8 cls = kotor_m15b_creature_stats_get_class_00647730(stats, nullptr,
                                                                static_cast<U8>(lb(stats, 0x32) - 1u)); // 006ADB88..006ADB90
    if (kotor::server_mainloop::creature_force_class_005be4a0(cls) != 0) {                       // 006ADB95..006ADBA3
        kotor_fastcall_006abce0(self, nullptr);                                                  // 006ADBA5..006ADBA7
        if (li(self, 0x3F7C) > 0) sw(self, 0x2D98, lw(self, 0x2D98) | 2u);                       // 006ADBAC..006ADBC1 (BTN_POWERS +44)
    } else {
        sw(self, 0x2D98, lw(self, 0x2D98) & 0xFFFFFFFDu);                                        // 006ADBD3
        if (lb(in_game_gui(), 0xBC0) == 1) sb(in_game_gui(), 0xBC0, 0);                          // 006ADBDA..006ADBFE
    }
    kotor_fastcall_006ad560(self, nullptr);                                                      // 006ADBC7 / 006ADC05..006ADC07
}

// 006ABEC0 CSWGuiInGameAbilities::UpdatePortraits() RET 0 (same code as 006AEE30 with the abilities offsets).
void __fastcall kotor_fastcall_006abec0(void* self, void*) {
    const Layout& l = abilities;
    if ((lb(recovered_server_info_004aec90(server_app(), nullptr), 0x104) & 1u) == 0) {      // 006ABECD..006ABEDD
        void* party = kotor_fastcall_005ed8b0(client_app(), nullptr);
        if (li(party, 0) > 1) {
            sw(self, flags(l.a), lw(self, flags(l.a)) | 2u);
            portrait_fill(self, fill1(l.a), frame_GetCharacter_006346c0(party, nullptr, 1), 0);
            portrait_fill(self, fill2(l.a), frame_GetCharacter_006346c0(party, nullptr, 1), 0);
        } else {
            sw(self, flags(l.a), lw(self, flags(l.a)) & 0xFFFFFFFDu);
        }
        if (li(party, 0) > 2) {
            sw(self, flags(l.b), lw(self, flags(l.b)) | 2u);
            portrait_fill(self, fill1(l.b), frame_GetCharacter_006346c0(party, nullptr, 2), 0);
            portrait_fill(self, fill2(l.b), frame_GetCharacter_006346c0(party, nullptr, 2), 0);
        } else {
            sw(self, flags(l.b), lw(self, flags(l.b)) & 0xFFFFFFFDu);
        }
        return;
    }
    U32 first = 0, second = 0;
    U8 cur = lb(self, slot(l.a));
    if (cur == 0xFE) cur = static_cast<U8>(lb(self, l.current) + 1);
    const I32 start = static_cast<std::int8_t>(cur);
    for (I32 i = start; i < 9 && second == 0; ++i) update_candidate(self, l, i, first, second);
    bool scan_low = true;
    if (current(self, l) != -1) {
        if (second != 0) scan_low = false;
        else if (first == 0) {
            portrait_fill(self, fill1(l.a), client_get_player_creature_005ed540(client_app(), nullptr), 0);
            portrait_fill(self, fill2(l.a), client_get_player_creature_005ed540(client_app(), nullptr), 0);
            sw(self, slot(l.a), 0xFFFFFFFFu); first = 1;
        } else {
            portrait_fill(self, fill1(l.b), client_get_player_creature_005ed540(client_app(), nullptr), 0);
            portrait_fill(self, fill2(l.b), client_get_player_creature_005ed540(client_app(), nullptr), 0);
            sw(self, slot(l.b), 0xFFFFFFFFu); second = 1;
            scan_low = false;
        }
    } else if (second != 0) {
        scan_low = false;
    }
    if (scan_low)
        for (I32 i = 0; i < start && second == 0; ++i) update_candidate(self, l, i, first, second);
    set_bit1(self, flags(l.a), first);
    set_bit1(self, flags(l.b), second);
    set_bit1(self, flags(l.left), kotor_fastcall_005636d0(party_table(), nullptr) > 2 ? 1u : 0u);
    set_bit1(self, flags(l.right), kotor_fastcall_005636d0(party_table(), nullptr) > 2 ? 1u : 0u);
}

// =====================================================================================================
// 006AD4B0 CSWGuiInGameAbilities::OnAbilitySelectionChanged(CSWGuiControl*) RET 4 (control unused).
void __fastcall kotor_fastcall_006ad4b0(void* self, void*, void*) {
    void* selected = kotor_m1_listbox_get_selected_control_0041a5d0(at(self, 0x30DC), nullptr); // 006AD4B4..006AD4BF
    if (lb(in_game_gui(), 0xBC0) == 1) {                                                         // 006AD4C1..006AD4D5
        const I32 skill = kotor_m15d_skill_flow_hit_check_006cd1c0(selected, nullptr, li(self, 0x315C), li(self, 0x3160)); // 006AD4D7..006AD4E7
        kotor_m15d_chart_set_selected_skill_006cdc00(at(self, 0x3F78), nullptr, skill);          // 006AD4EE..006AD4F5
        kotor_fastcall_006acce0(self, nullptr, static_cast<U16>(skill));                         // 006AD4FA..006AD500 (movzx di)
        return;
    }
    if (lb(in_game_gui(), 0xBC0) == 2) {                                                         // 006AD50A..006AD51F
        const I32 skill = kotor_m15d_skill_flow_hit_check_006cd1c0(selected, nullptr, li(self, 0x315C), li(self, 0x3160)); // 006AD521..006AD531
        kotor_m15d_chart_set_selected_skill_006cdc00(at(self, 0x3F88), nullptr, skill);          // 006AD538..006AD53F
        kotor_fastcall_006ad410(self, nullptr, static_cast<U32>(skill));                         // 006AD544..006AD547
    }
}

// 006ADA70 / 006ADAA0 / 006ADAD0 On{Feats,Powers,Skills}ButtonPressed(CSWGuiControl*) RET 4 (control unused).
void __fastcall kotor_fastcall_006ada70(void* self, void*, void*) {
    sb(in_game_gui(), 0xBC0, 2);                                                                 // 006ADA70..006ADA82
    kotor_fastcall_006ad560(self, nullptr);                                                      // 006ADA89
}
void __fastcall kotor_fastcall_006adaa0(void* self, void*, void*) {
    sb(in_game_gui(), 0xBC0, 1);                                                                 // 006ADAA0..006ADAB2
    kotor_fastcall_006ad560(self, nullptr);                                                      // 006ADAB9
}
void __fastcall kotor_fastcall_006adad0(void* self, void*, void*) {
    sb(in_game_gui(), 0xBC0, 0);                                                                 // 006ADAD0..006ADAE2
    kotor_fastcall_006ad560(self, nullptr);                                                      // 006ADAE9
}

// 006ADC80 CSWGuiInGameAbilities::OnChangeCharacter(CSWGuiControl*) RET 4.
void __fastcall kotor_fastcall_006adc80(void* self, void*, void* control) {
    const Layout& l = abilities;
    if (lw(control, 0x4C) == 0) return;                                                          // 006ADC85..006ADC93
    if ((lb(recovered_server_info_004aec90(server_app(), nullptr), 0x104) & 1u) != 0) {      // 006ADC99..006ADCAE
        const U8 index = lb(control, 0x58);                                                      // 006ADCB4 bl
        void* creature;
        if (index == 0xFF) {                                                                     // 006ADCB7..006ADCBA
            creature = client_get_player_creature_005ed540(client_app(), nullptr);              // 006ADCBC..006ADCC5
        } else {
            void* app = client_app();                                                            // 006ADCCC..006ADCD4
            const U32 object = recovered_npc_object_00564700(party_table(), nullptr, static_cast<std::int8_t>(index), 0, 1); // 006ADCD7..006ADCE6
            creature = pt(frame_R4_005ed5b0(app, nullptr, kotor_fastcall_005eda50(app, nullptr, object))); // 006ADCEB..006ADCF6
        }
        if (creature != nullptr) {                                                               // 006ADCFB..006ADCFD
            if ((lw(self, flags(l.right)) & 2u) != 0) {                                          // 006ADD03..006ADD0A
                if (control == at(self, l.a)) sw(self, slot(l.a), lw(self, slot(l.b)));         // 006ADD0C..006ADD1C
            } else {
                sw(self, slot(l.a), 0xFFFFFFFEu);                                                // 006ADD2A
            }
            sb(self, l.current, index);                                                          // 006ADD22 / 006ADD34
            kotor_fastcall_006adb00(self, nullptr, creature);                                    // 006ADD81..006ADD84 SetCharacter
        }
    } else {
        const U32 step = control == at(self, l.b) ? 2u : 1u;                                     // 006ADC8E / 006ADD3C..006ADD46
        if (kotor_fastcall_005edf80(client_app(), nullptr, 0, 0, step) == 0) return;             // 006ADD4B..006ADD60
        kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 1);                                     // 006ADD62..006ADD67 PlayGuiSound(1)
        void* player = kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr); // 006ADD6C..006ADD7C
        kotor_fastcall_006adb00(self, nullptr, player);                                          // 006ADD81..006ADD84 SetCharacter
    }
    kotor_fastcall_006abec0(self, nullptr);                                                      // 006ADD89..006ADD8B UpdatePortraits
}
}  // extern "C"
