// Lane W4 G15 (enter-handlers): in-game GUI "enter" (event id 0) handlers registered during panel construction, plus
// the CSWGuiScriptSelect::OnScriptSelected (0x27) handler, and the callees they reach that had no rebuilt provider.
// Transcribed from linear-sweep listings of swkotor.exe (claude-runs/tools/win.py <VA>); PCs cited per statement.
//
//   006E9FE0 CSWGuiScriptSelect::OnEnter                 thiscall(CSWGuiControl*) RET 4
//   006E9E70 CSWGuiScriptSelect::OnScriptSelected        thiscall(CSWGuiControl*) RET 4
//   006BF5B0 CSWGuiPartySelection::OnEnter               thiscall(CSWGuiControl*) RET 4
//   006AD180 CSWGuiInGameAbilities::OnEnterSkill         thiscall(CSWGuiControl*) RET 4
//   006AB220 CSWGuiInGameOptions::OnEnterSlot            thiscall(CSWGuiControl*) RET 4
// Callees reconstructed here (no rebuilt body existed):
//   006E9D50 CSWGuiScriptSelect::SetDescription(int strref)              RET 4
//   006AB000 CSWGuiInGameOptions::SetDescription(int strref)             RET 4
//   006ABD20 CSWGuiInGameAbilities::UpdateDescriptionListBox(int)        RET 4
//   006BECC0 CSWGuiPartySelection::SetAddRemoveLabel(int)                RET 4
//   005A53B0 CSWSCreatureStats::GetClassString(CExoString*, unsigned char) RET 8
//   005A4C60 CSWSCreatureStats::GetFullName(CExoString*)                 RET 4
// Panel field offsets are those of panels_character.cpp / panels_menus.cpp (same layouts).
#include "78d2073e39-gui_panels.hpp"
using namespace w4gui;

namespace kotor_reconstruction { struct StartupString; }
using SS = kotor_reconstruction::StartupString;
SS* __fastcall GuiConcat005e5d10(SS* self, void*, SS* out, const SS* rhs);                       // CExoString::operator+
namespace kotor_menu_10_19 {
struct String;
struct Tlk { String* __thiscall simple_0041e8f0(String* out, unsigned strref); };                 // CTlkTable::GetSimpleString
}
namespace menu_resources {
struct Manager { unsigned __thiscall exists_00408bc0(Ref* ref, unsigned type, unsigned* out); };  // CExoResMan::Exists
}
namespace kotor_resource_demand { extern menu_resources::Manager* volatile resource_manager_007a39e8; }  // [007A39E8]
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); }  // CServerExoApp::GetPartyTable
struct Ref16 { unsigned char bytes[16]; };

extern "C" {
// ---- server / party / creature
void* __fastcall kotor_fastcall_004ae770(void* server, void*, unsigned id);                       // CServerExoApp::GetCreatureByGameObjectID
unsigned __fastcall recovered_party_npc_available_005636b0(void* party, void*, unsigned index);    // CSWPartyTable::GetIsNPCAvailable
unsigned __fastcall recovered_npc_object_00564700(void* party, void*, unsigned index, unsigned create, unsigned flag); // CSWPartyTable::GetNPCObject
unsigned char __fastcall recovered_stats_level_005a5fd0(void* stats, void*, unsigned flag);       // CSWSCreatureStats::GetLevel
unsigned __cdecl recovered_get_locstring_server_00578870(void* player, void* loc, void* loc2, void* out, unsigned gender); // CSWSMessage::GetLocStringServer
void* __fastcall kotor_m15b_class_get_name_text_005be0e0(void* cls, void*, void* out) noexcept(false);   // CSWClass::GetNameText
void* __fastcall kotor_m15b_skill_get_description_text_005cd970(void* skill, void*, void* out) noexcept(false); // CSWSkill::GetDescriptionText
extern void* kotor_cell_rules_007a3a28;                                                           // [007A3A28] CSWRules*
// ---- gui manager / controls
void* __fastcall kotor_fastcall_0040be00(void* gui, void*);                                       // CSWGuiManager::PopModalPanel
void* __fastcall kotor_m1_listbox_get_selected_control_0041a5d0(void* listbox, void*);            // CSWGuiListBox::GetSelectedControl
void __fastcall gui_load_listbox_clear_items_00419a90(void* listbox, void*);                      // CSWGuiListBox::ClearItems
void __fastcall gui_m48_listbox_add_controls_0041c2c0(void* listbox, void*, void** controls, I32 count,
                                                     U32 suppress, U32 restore, U32 variable) noexcept(false); // CSWGuiListBox::AddControls
void __fastcall gui_load_listbox_set_active_control_0041c160(void* listbox, void*, void* control, U32); // CSWGuiListBox::SetActiveControl
std::int32_t __fastcall gui_load_text_font_height_00414ee0(void* text, void*);                   // CSWGuiText::GetFontHeight
void __fastcall gui_load_border_params_set_fill_00414c00(void* params, void*, const void* ref, I32) noexcept(false); // SetFillImage
// ---- strings / resources / tlk
std::int32_t __fastcall async_string_length_005e5790(void* self, void*);                          // CExoString::GetLength
void* __fastcall kotor_fastcall_005e5bc0(void* self, void*, std::int32_t value) noexcept(false);  // CExoString::CExoString(int)
U32 __fastcall tlk_fetch_0041e550(void* tlk, void*, U32 strref, void* str_res, U32 flag) noexcept(false); // CTlkTable::Fetch
void* __fastcall c2da_default_ref_00405ed0(void* self, void*);                                   // CResRef::CResRef()
void* __fastcall c2da_ref_to_string_00405f70(void* ref, void*, void* out);                        // CResRef::CopyToString
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, void* text);               // CResRef(const CExoString&)
// ---- .data colours (3 floats each), bound by their rebuilt storage
extern const unsigned char kotor_m1_color_007a2378[12];                                           // [007A2378] (1, 1, 1)
extern const unsigned char kotor_m1_color_007a2384[12];                                           // [007A2384] (.35, .35, .35)
extern const unsigned char borrowed_exit_color_007a23b4[12];                                      // [007A23B4] (0, .66, .98)
extern const unsigned char kotor_m15d_color_007a23d8[12];                                         // [007A23D8] (.74, .11, 0)
extern const unsigned char kotor_m15d_color_007a23e4[12];                                         // [007A23E4] (.28, .92, .11)
extern const unsigned char kotor_m1_color_007a2408[12];                                           // [007A2408] (0, .33, .49)
}

namespace {
// .rdata byte 0073ECD0 (image value 00), CTlkTable::Fetch's third argument at every SetDescription site.
const unsigned char rdata_0073ecd0 = 0x00;
SS* ss(void* s) { return static_cast<SS*>(s); }
// A stack CExoString: destroyed at scope end (normally or while unwinding) once it has been constructed, so
// reverse declaration order reproduces the original destructor order and EH state cleanup.
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
// CTlkTable::Fetch result {CExoString text; CResRef sound; float length} (28 bytes, [esp+1Ch..37h] in 006E9D50).
struct StrRes { U32 text[2]; Ref16 sound; U32 length; };
static_assert(sizeof(StrRes) == 28, "STR_RES frame");
// Shared tail of the three description routines: `extent` (already copied from `label`+4) gets the new height,
// `label` is re-added as the only item of `listbox` and made its active item. `measured` selects CSWGuiText
// slot 08h (text height) vs CSWGuiText::GetFontHeight on the label's text (+D0h).
void readd_description(void* listbox, void* label, I32* extent, bool measured) {
    void* text = at(label, 0xD0);
    extent[3] = measured ? vcall<I32>(text, 0x08) : gui_load_text_font_height_00414ee0(text, nullptr);
    set_extent(label, extent);                                                                   // [label]+04 SetExtent
    void* items[1] = {label};
    gui_m48_listbox_add_controls_0041c2c0(listbox, nullptr, items, 1, 0, 0, 0);
    gui_load_listbox_set_active_control_0041c160(listbox, nullptr, label, 0);
}
}  // namespace

extern "C" {
// =====================================================================================================
// 005A53B0 CSWSCreatureStats::GetClassString(CExoString* out, unsigned char index) RET 8. Returns out.
void* __fastcall kotor_fastcall_005a53b0(void* stats, void*, void* out, U32 index) {
    const U8 i = static_cast<U8>(index);                                                         // 005A53B1 [esp+0Ch] (byte)
    if (i < lb(stats, 0x89)) {                                                                   // 005A53B5..005A53C4 jae (unsigned)
        const U32 cls = static_cast<U32>(lb(stats, static_cast<U32>(i) * 0x28u + 0xA7u)) * 0x1A0u   // 005A53CC..005A53E0
                        + lw(kotor_cell_rules_007a3a28, 0xAC);                                   // 005A53C6 / 005A53DA..005A53E6
        if (cls != 0) {                                                                          // 005A53E8 je
            kotor_m15b_class_get_name_text_005be0e0(pt(cls), nullptr, out);                      // 005A53EA..005A53EF
            return out;                                                                          // 005A53F4..005A53F8
        }
    }
    frame_LogStringCtor_005e5a90(out, nullptr, "");                                              // 005A53FB..005A5406 (0073D71C "")
    return out;                                                                                  // 005A540B..005A540F
}

// 005A4C60 CSWSCreatureStats::GetFullName(CExoString* out) RET 4. Returns out.
void* __fastcall kotor_fastcall_005a4c60(void* stats, void*, void* out) {
    Str name; exo_default(name.p()); name.born();                                                // 005A4C7B..005A4C87 [esp+8] (state 0)
    recovered_get_locstring_server_00578870(pt(0xFFFFFFFFu), at(stats, 0x34), at(stats, 0x3C), name.p(), 0); // 005A4C8C..005A4CAE
    WindowCopy005e5b00(out, nullptr, name.p());                                                  // 005A4CB1..005A4CB8 CExoString(const&)
    return out;                                                                                  // 005A4CBD..005A4CCE ~CExoString(name)
}

// =====================================================================================================
// 006E9D50 CSWGuiScriptSelect::SetDescription(int strref) RET 4.
void __fastcall kotor_fastcall_006e9d50(void* self, void*, U32 strref) {
    StrRes res;                                                                                  // [esp+1Ch]
    exo_default(res.text);                                                                       // 006E9D6C..006E9D70 (state 0)
    auto guard = on_throw([&]() { frame_LogStringDtor_005e5c20(res.text, nullptr); });
    c2da_default_ref_00405ed0(&res.sound, nullptr);                                              // 006E9D75..006E9D81 (state 1)
    tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, strref, &res, rdata_0073ecd0);          // 006E9D86..006E9DA6
    gui_load_text_params_set_text_00415e00(at(self, 0x718), nullptr, res.text);                  // 006E9DAB..006E9DB6 (LBL +630 text)
    void* listbox = at(self, 0x350);                                                             // 006E9DBB (LB_DESC)
    gui_load_listbox_clear_items_00419a90(listbox, nullptr);                                     // 006E9DC1..006E9DC3
    I32 extent[4]; std::memcpy(extent, at(self, 0x634), 16);                                     // 006E9DC8..006E9DE9 [esp+0Ch]
    const bool measured = async_string_length_005e5790(res.text, nullptr) > 0;                   // 006E9DE1..006E9DFA
    readd_description(listbox, at(self, 0x630), extent, measured);                               // 006E9DF4..006E9E3B
    guard.armed = false;
    frame_LogStringDtor_005e5c20(res.text, nullptr);                                             // 006E9E40..006E9E4C
}

// 006E9FE0 CSWGuiScriptSelect::OnEnter(CSWGuiControl*) RET 4.
void __fastcall kotor_fastcall_006e9fe0(void* self, void*, void* control) {
    if (lw(control, 0x4C) != 0) {                                                                // 006E9FE0..006E9FE9
        const U32 index = lw(control, 0x50);                                                     // 006E9FEB
        kotor_fastcall_006e9d50(self, nullptr, lw(lp(self, 0x64), index * 8u));                  // 006E9FEE..006E9FF8 (tail jmp)
    }
}                                                                                                // 006E9FFD ret 4

// 006E9E70 CSWGuiScriptSelect::OnScriptSelected(CSWGuiControl*) RET 4.
void __fastcall kotor_fastcall_006e9e70(void* self, void*, void*) {
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, lw(self, 0xC3C));            // 006E9E73..006E9E84
    if (creature != nullptr) {                                                                   // 006E9E8B..006E9E8D
        void* selected = kotor_m1_listbox_get_selected_control_0041a5d0(at(self, 0x70), nullptr); // 006E9E8F..006E9E92 (unchecked)
        const U32 index = lw(selected, 0x50);                                                    // 006E9E97
        const U16 state = lh(lp(self, 0x64), index * 8u + 4u);                                   // 006E9E9A..006E9EA3 AISTATE
        std::memcpy(static_cast<U8*>(lp(creature, 0xA74)) + 0x13A, &state, 2);                   // 006E9E9D / 006E9EA8 (stats +13Ah)
    }
    kotor_fastcall_0040be00(lp(self, 0x18), nullptr);                                            // 006E9EAF..006E9EB2 PopModalPanel
    U32 flags = lw(self, 0x44) & 0xFFFFFEFFu;                                                    // 006E9EB7..006E9EBA
    sw(self, 0x44, flags);                                                                       // 006E9ECD
    if ((flags & 0x600u) != 0x400u) {                                                            // 006E9EBF..006E9ED0
        flags = (flags & 0xFFFFFBFFu) | 0x200u;                                                  // 006E9ED2..006E9ED7
        sw(self, 0x44, flags);                                                                   // 006E9EDC
    }
}

// =====================================================================================================
// 006AB000 CSWGuiInGameOptions::SetDescription(int strref) RET 4.
void __fastcall kotor_fastcall_006ab000(void* self, void*, U32 strref) {
    void* params = at(self, 0x1270);                                                             // 006AB020 (LBL +1188 text params)
    gui_load_text_params_set_strref_00415e50(params, nullptr, strref);                           // 006AB026..006AB029
    Str text; WindowCopy005e5b00(text.p(), nullptr, params); text.born();                        // 006AB02E..006AB040 [esp+8] (state 0)
    void* listbox = at(self, 0x1908);                                                            // 006AB038 (LB_DESC)
    gui_load_listbox_clear_items_00419a90(listbox, nullptr);                                     // 006AB03E..006AB048
    I32 extent[4]; std::memcpy(extent, at(self, 0x118C), 16);                                    // 006AB04D..006AB06E [esp+10h]
    const bool measured = async_string_length_005e5790(text.p(), nullptr) > 0;                   // 006AB066..006AB079
    readd_description(listbox, at(self, 0x1188), extent, measured);                              // 006AB07F..006AB0C1
    const bool scroll = static_cast<I32>(lb(listbox, 0x2C0)) + li(listbox, 0x2B4) > li(listbox, 0x298) // 006AB0C6..006AB0DE jle
                        && li(listbox, 0x2A0) > 0                                                // 006AB0E0..006AB0E8 jle
                        && (lw(listbox, 0x2BC) & 0x100u) == 0;                                   // 006AB0EA..006AB0F3 test ah,1
    const U32 old = lw(self, 0x180C);                                                            // 006AB0FE (LBL +17C8 flags)
    sw(self, 0x180C, old ^ (((scroll ? 1u : 0u) * 2u ^ old) & 2u));                              // 006AB104..006AB114
}                                                                                                // 006AB11A..006AB122 ~CExoString(text)

// 006AB220 CSWGuiInGameOptions::OnEnterSlot(CSWGuiControl*) RET 4.
void __fastcall kotor_fastcall_006ab220(void* self, void*, void* control) {
    I32 slot = -1;                                                                               // 006AB226 or esi,-1
    for (U32 i = 0; i < 8 && slot == -1; ++i)                                                    // 006AB229..006AB245
        if (control == at(self, 0x64 + i * 0x1C4)) slot = static_cast<I32>(i);
    static const U32 strrefs[8] = {0xA53C, 0xA53D, 0xBB86, 0xBD99, 0xBDBC, 0xBB8A, 0xBB8C, 0xA53E}; // jump table 006AB2D0 -> 006AB253..006AB2BC
    if (static_cast<U32>(slot) > 7u) return;                                                     // 006AB247..006AB24A ja -> 006AB2CB
    kotor_fastcall_006ab000(self, nullptr, strrefs[slot]);                                       // 006AB24C (tail jmp 006AB000)
}

// =====================================================================================================
// 006ABD20 CSWGuiInGameAbilities::UpdateDescriptionListBox(int measured) RET 4.
void __fastcall kotor_fastcall_006abd20(void* self, void*, I32 measured) {
    void* listbox = at(self, 0x33BC);                                                            // 006ABD27 (LB_DESC)
    gui_load_listbox_clear_items_00419a90(listbox, nullptr);                                     // 006ABD2D..006ABD2F
    I32 extent[4]; std::memcpy(extent, at(self, 0x2054), 16);                                    // 006ABD34..006ABD57 [esp+0Ch]
    readd_description(listbox, at(self, 0x2050), extent, measured != 0);                         // 006ABD4D..006ABDA6
}

// 006AD180 CSWGuiInGameAbilities::OnEnterSkill(CSWGuiControl*) RET 4.
void __fastcall kotor_fastcall_006ad180(void* self, void*, void* control) {
    const U8 index = static_cast<U8>(lw(control, 0x50));                                         // 006AD19E..006AD1A1
    sb(self, 0x3F70, index);                                                                     // 006AD1AB
    const U8* entry = static_cast<const U8*>(lp(self, 0x64)) + static_cast<U32>(index) * 12u;    // 006AD1B1..006AD1BB
    void* skill = lp(entry, 0);                                                                  // 006AD1BE..006AD1C0 [esp+10]
    const I32 rank = li(entry, 4);                                                               // 006AD1B7 ebx -> [esp+0Ch]
    const I32 bonus = li(entry, 8);                                                              // 006AD1C5 edi
    Str bonus_text; exo_default(bonus_text.p()); bonus_text.born();                              // 006AD1C8..006AD1D0 [esp+14] (state 0)
    U32 color[3] = {0, 0, 0};                                                                    // 006AD1DF..006AD1EF [esp+34]
    if (bonus > 0) {                                                                             // 006AD1D5 / 006AD1FC jle
        {
            Str number; kotor_fastcall_005e5bc0(number.p(), nullptr, bonus); number.born();      // 006AD1F7..006AD202 [esp+1C]
            Str plus; frame_LogStringCtor_005e5a90(plus.p(), nullptr, "+"); plus.born();         // 006AD207..006AD215 [esp+2C] (00748ED4)
            Str sum; GuiConcat005e5d10(ss(plus.p()), nullptr, ss(sum.p()), ss(number.p())); sum.born(); // 006AD21A..006AD22B [esp+24]
            async_assign_string_005e5c50(bonus_text.p(), nullptr, sum.p());                      // 006AD230..006AD23A
        }                                                                                        // 006AD23F..006AD264 ~sum, ~plus, ~number
        std::memcpy(color, kotor_m15d_color_007a23e4, 12);                                       // 006AD269..006AD282 green
    } else if (bonus < 0) {                                                                      // 006AD28B jge
        {
            Str number; kotor_fastcall_005e5bc0(number.p(), nullptr, bonus); number.born();      // 006AD28D [esp+1C]
            async_assign_string_005e5c50(bonus_text.p(), nullptr, number.p());                   // 006AD292..006AD2A0
        }                                                                                        // 006AD2A5..006AD2AE
        std::memcpy(color, kotor_m15d_color_007a23d8, 12);                                       // 006AD2B3..006AD2CC red
    } else {
        {
            Str number; kotor_fastcall_005e5bc0(number.p(), nullptr, bonus); number.born();      // 006AD2D2 [esp+1C]
            async_assign_string_005e5c50(bonus_text.p(), nullptr, number.p());                   // 006AD2D7..006AD2E5
        }                                                                                        // 006AD2EA..006AD2F3
        std::memcpy(color, borrowed_exit_color_007a23b4, 12);                                    // 006AD2F8..006AD311
    }
    {
        Str number; kotor_fastcall_005e5bc0(number.p(), nullptr, rank); number.born();           // 006AD315..006AD31A [esp+1C]
        gui_load_text_params_set_text_00415e00(at(self, 0x23B8), nullptr, number.p());           // 006AD31F..006AD32F LBL_RANKVAL
    }                                                                                            // 006AD334..006AD33D
    gui_load_text_params_set_text_00415e00(at(self, 0x2638), nullptr, bonus_text.p());           // 006AD342..006AD34F LBL_BONUSVAL
    gui_load_text_params_set_color_00414e10(at(self, 0x2638), nullptr, color);                   // 006AD354..006AD35B
    {
        const I32 total = static_cast<I32>(static_cast<U32>(bonus) + static_cast<U32>(rank));    // 006AD360..006AD364 add edi,ecx (wraps)
        Str number; kotor_fastcall_005e5bc0(number.p(), nullptr, total); number.born();          // 006AD366..006AD36B [esp+1C]
        gui_load_text_params_set_text_00415e00(at(self, 0x28B8), nullptr, number.p());           // 006AD370..006AD380 LBL_TOTALVAL
    }                                                                                            // 006AD385..006AD38E
    gui_load_text_params_set_strref_00415e50(at(self, 0x29F8), nullptr, lw(skill, 0));           // 006AD393..006AD3A0 LBL_NAME
    {
        Str desc;
        void* text = kotor_m15b_skill_get_description_text_005cd970(skill, nullptr, desc.p()); desc.born(); // 006AD3A5..006AD3B1 [esp+2C]
        gui_load_text_params_set_text_00415e00(at(self, 0x2138), nullptr, text);                // 006AD3B3..006AD3BF
        const I32 has = async_string_length_005e5790(text, nullptr) > 0 ? 1 : 0;                 // 006AD3C4..006AD3CF setg
        kotor_fastcall_006abd20(self, nullptr, has);                                             // 006AD3D2..006AD3D5
    }                                                                                            // 006AD3DA..006AD3E3
}                                                                                                // 006AD3E8..006AD3F4 ~CExoString(bonus_text)

// =====================================================================================================
// 006BECC0 CSWGuiPartySelection::SetAddRemoveLabel(int add) RET 4.
void __fastcall kotor_fastcall_006becc0(void* self, void*, I32 add) {
    void* accept_text = at(self, 0x3A24);                                                        // BTN_ACCEPT (+38B8) text params
    if (add != 0) {                                                                              // 006BECC0..006BECD0
        gui_load_text_params_set_strref_00415e50(at(self, 0x319C), nullptr, 0x9637);             // 006BECD2..006BECD7
        gui_load_text_params_set_strref_00415e50(accept_text, nullptr, 0x9637);                  // 006BECDC..006BECE9
        if (li(self, 0x68) >= 2) {                                                               // 006BECEE..006BECF2 jl
            gui_load_text_params_set_color_00414e10(accept_text, nullptr, kotor_m1_color_007a2408); // 006BECF4..006BECFB
            std::memcpy(at(self, 0x3948), kotor_m1_color_007a2384, 12);                          // 006BED00..006BED1D
            return;                                                                              // 006BED21
        }
    } else {
        gui_load_text_params_set_strref_00415e50(at(self, 0x319C), nullptr, 0x9638);             // 006BED24..006BED29
        gui_load_text_params_set_strref_00415e50(accept_text, nullptr, 0x9638);                  // 006BED2E..006BED3B
    }
    gui_load_text_params_set_color_00414e10(accept_text, nullptr, borrowed_exit_color_007a23b4); // 006BED40..006BED47
    std::memcpy(at(self, 0x3948), kotor_m1_color_007a2378, 12);                                  // 006BED4C..006BED69
}

// 006BF5B0 CSWGuiPartySelection::OnEnter(CSWGuiControl*) RET 4.
void __fastcall kotor_fastcall_006bf5b0(void* self, void*, void* control) {
    void* server = server_app();                                                                 // 006BF5CE..006BF5D6 edi
    void* party = lane_config_required::GetPartyTable_004aee70(server, nullptr);                 // 006BF5DB..006BF5DD ebx
    void* data = lp(control, 0x58);                                                              // 006BF5E2..006BF5E7 [esp+10] CSWGuiPartySelectionData
    if (recovered_party_npc_available_005636b0(party, nullptr, lw(data, 0x450)) == 0) {          // 006BF5EA..006BF5FE
        sw(self, 0x2D38, lw(self, 0x2D38) & 0xFFFFFFFDu);                                        // 006BF9C3..006BF9D0 LBL_3D
        sw(self, 0x2AB8, lw(self, 0x2AB8) & 0xFFFFFFFDu);                                        // 006BF9D6 LBL_NPC_NAME
        sw(self, 0x2BF8, lw(self, 0x2BF8) & 0xFFFFFFFDu);                                        // 006BF9DC..006BF9EF LBL_NPC_LEVEL
        gui_load_text_params_set_color_00414e10(at(self, 0x319C), nullptr, kotor_m1_color_007a2408); // 006BF9E4..006BF9F5
        kotor_fastcall_006becc0(self, nullptr, 1);                                               // 006BF9FA..006BF9FE
        if (void* previous = lp(self, 0x3A7C)) {                                                 // 006BFA03..006BFA0B
            vcall<void, U32>(previous, 0x40, 0);                                                 // 006BFA0D..006BFA11
            zero(self, 0x3A7C);                                                                  // 006BFA14
        }
        return;                                                                                  // 006BFA1E..006BFA2F
    }
    const U32 id = recovered_npc_object_00564700(party, nullptr, lw(data, 0x450), 0, 1);         // 006BF604..006BF615
    void* creature = kotor_fastcall_004ae770(server, nullptr, id);                               // 006BF61A..006BF61D
    if (creature != nullptr) {                                                                   // 006BF622..006BF626
        Str portrait; exo_default(portrait.p()); portrait.born();                                // 006BF62C..006BF630 [esp+28] (state 0)
        {
            Ref16 ref;                                                                           // [esp+40]
            void* resref = vcall<void*, void*>(creature, 0xC8, &ref);                            // 006BF635..006BF64B CSWSCreature::GetPortrait
            c2da_ref_to_string_00405f70(resref, nullptr, portrait.p());                          // 006BF637..006BF653 CopyToString
        }
        {
            Str suffix; frame_LogStringCtor_005e5a90(suffix.p(), nullptr, "3"); suffix.born();   // 006BF658..006BF661 [esp+20] (00753064) state 1
            Str sum; GuiConcat005e5d10(ss(portrait.p()), nullptr, ss(sum.p()), ss(suffix.p())); sum.born(); // 006BF666..006BF679 [esp+18]
            async_assign_string_005e5c50(portrait.p(), nullptr, sum.p());                        // 006BF67E..006BF68C
        }                                                                                        // 006BF691..006BF6A8 ~sum, ~suffix
        if (static_cast<std::int16_t>(lh(lp(creature, 0xA74), 0x17E)) < 0x32) {                 // 006BF6AD..006BF6BB jge (signed word)
            Str candidate;                                                                       // [esp+20]
            {
                Str suffix; frame_LogStringCtor_005e5a90(suffix.p(), nullptr, "e"); suffix.born(); // 006BF6C1..006BF6CA [esp+18] (00756C84)
                GuiConcat005e5d10(ss(portrait.p()), nullptr, ss(candidate.p()), ss(suffix.p())); candidate.born(); // 006BF6CF..006BF6E2
            }                                                                                    // 006BF6E7..006BF6F0 ~suffix
            menu_resources::Ref ref;                                                             // [esp+40]
            async_ref_construct_string_00406d60(&ref, nullptr, candidate.p());                   // 006BF6F5..006BF6FE
            unsigned found = kotor_resource_demand::resource_manager_007a39e8->exists_00408bc0(&ref, 3, nullptr); // 006BF703..006BF712
            if (found == 0) {                                                                    // 006BF717..006BF719
                async_ref_construct_string_00406d60(&ref, nullptr, candidate.p());               // 006BF71B..006BF724
                found = kotor_resource_demand::resource_manager_007a39e8->exists_00408bc0(&ref, 0xBBF, nullptr); // 006BF729..006BF73B
            }
            if (found != 0) async_assign_string_005e5c50(portrait.p(), nullptr, candidate.p());  // 006BF740..006BF74D
        }                                                                                        // 006BF752..006BF75B ~candidate
        {
            menu_resources::Ref ref;                                                             // [esp+40]
            async_ref_construct_string_00406d60(&ref, nullptr, portrait.p());                    // 006BF760..006BF769
            gui_load_border_params_set_fill_00414c00(at(self, 0x2D64), nullptr, &ref, 0);         // 006BF76E..006BF77B LBL_3D fill
        }
        Str level_text; exo_default(level_text.p()); level_text.born();                          // 006BF780..006BF784 [esp+30] (state 6)
        {
            void* stats = lp(creature, 0xA74);                                                   // 006BF789
            const U32 last = static_cast<U8>(lb(stats, 0x89) - 1u);                              // 006BF78F..006BF797 dec al (byte wrap)
            Str class_name;                                                                      // [esp+40]
            void* class_text = kotor_fastcall_005a53b0(stats, nullptr, class_name.p(), last); class_name.born(); // 006BF799..006BF7A9 [esp+14]
            Str simple;                                                                          // [esp+38]
            void* simple_text = static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08)->simple_0041e8f0(
                reinterpret_cast<kotor_menu_10_19::String*>(simple.p()), 0x7D9A);                // 006BF7AD..006BF7C2 [esp+20]
            simple.born();
            void* stats_again = lp(creature, 0xA74);                                             // 006BF7CB..006BF7D1 [esp+18]
            const char* class_c = frame_LogCStr_005e5670(class_text, nullptr);                   // 006BF7D5..006BF7DE
            const U8 level = recovered_stats_level_005a5fd0(stats_again, nullptr, 0);            // 006BF7E3..006BF7EA
            const char* simple_c = frame_LogCStr_005e5670(simple_text, nullptr);                 // 006BF7EF..006BF7F7
            frame_LogFormat_005e5680(level_text.p(), "%s %d %s", simple_c, static_cast<U32>(level), class_c); // 006BF7FC..006BF80C (00756C78)
        }                                                                                        // 006BF80F..006BF826 ~simple, ~class_name
        {
            Str full;                                                                            // [esp+40]
            void* name = kotor_fastcall_005a4c60(lp(creature, 0xA74), nullptr, full.p()); full.born(); // 006BF82B..006BF836
            gui_load_text_params_set_text_00415e00(at(self, 0x2B5C), nullptr, name);             // 006BF83B..006BF847 LBL_NPC_NAME
        }                                                                                        // 006BF84C..006BF855
        gui_load_text_params_set_text_00415e00(at(self, 0x2C9C), nullptr, level_text.p());       // 006BF85A..006BF865 LBL_NPC_LEVEL
        sw(self, 0x2D38, lw(self, 0x2D38) | 2u);                                                 // 006BF86A
        sw(self, 0x2AB8, lw(self, 0x2AB8) | 2u);                                                 // 006BF870
        sw(self, 0x2BF8, lw(self, 0x2BF8) | 2u);                                                 // 006BF876
        const void* color = (li(self, 0x68) < 2 && (lb(data, 0x448) & 1u) != 0)                 // 006BF87C..006BF88C
                                ? static_cast<const void*>(borrowed_exit_color_007a23b4)         // 006BF88E
                                : static_cast<const void*>(kotor_m1_color_007a2408);             // 006BF895
        gui_load_text_params_set_color_00414e10(at(self, 0x319C), nullptr, color);               // 006BF89A..006BF8A0
        kotor_fastcall_006becc0(self, nullptr, lw(control, 0x1C4) == 0 ? 1 : 0);                 // 006BF8A5..006BF8B8
    } else {                                                                                     // 006BF8DC / 006BF8E1
        {
            Str text; frame_LogStringCtor_005e5a90(text.p(), nullptr, "I am broken"); text.born(); // 006BF8E1..006BF8EA [esp+30] (00756C6C)
            gui_load_text_params_set_text_00415e00(at(self, 0x2B5C), nullptr, text.p());         // 006BF8EF..006BF902
        }                                                                                        // 006BF907..006BF912
        {
            Str text; frame_LogStringCtor_005e5a90(text.p(), nullptr, "So very very broken"); text.born(); // 006BF917..006BF920 [esp+20] (00756C58)
            gui_load_text_params_set_text_00415e00(at(self, 0x2C9C), nullptr, text.p());         // 006BF925..006BF938
        }                                                                                        // 006BF93D..006BF945
        sw(self, 0x2AB8, lw(self, 0x2AB8) | 2u);                                                 // 006BF94A..006BF957
        sw(self, 0x2BF8, lw(self, 0x2BF8) | 2u);                                                 // 006BF95D
        sw(self, 0x2D38, lw(self, 0x2D38) & 0xFFFFFFFDu);                                        // 006BF963..006BF977
        gui_load_text_params_set_color_00414e10(at(self, 0x319C), nullptr, kotor_m1_color_007a2408); // 006BF96C..006BF97D
        kotor_fastcall_006becc0(self, nullptr, 1);                                               // 006BF982..006BF986
    }
    if (void* previous = lp(self, 0x3A7C)) vcall<void, U32>(previous, 0x40, 0);                  // 006BF98F..006BF99D
    sw(self, 0x3A7C, ad(data));                                                                  // 006BF9A0
    vcall<void, U32>(data, 0x40, 1);                                                             // 006BF9A6..006BF9AC
}
}  // extern "C"
