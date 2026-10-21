// Lane W4 G15: CSWGuiInGameCharacter (006B0E40) and CSWGuiScriptSelect (006EA000, built by it), with destructors and
// the small CSWGuiSliderReadOnly / CSWGuiSpeedButton destructor thunks their method tables need.
// Transcribed from dis/c_006B0E40.asm and the compact.py / odis listings cited per line.
#include "78d2073e39-gui_panels.hpp"
#include "60d326f6b8-claude_handlers.hpp"
using namespace w4gui;

namespace kotor_input_hover { struct Control; }
namespace kotor_m15_l0 { struct PanelEventThunks15 { void __thiscall OnYButtonPressed_00644720(kotor_input_hover::Control*); }; }
namespace directory_list { struct TimeList; void __fastcall AllocateTimes004c4f50(TimeList* list, void*, int count); }  // CExoArrayList<>::Allocate
namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations* list, void*, unsigned count); }
struct Ref16 { unsigned char bytes[16]; };
extern "C" {
void* __fastcall gui_load_slider_construct_0041baa0(void* self, void*) noexcept(false);        // CSWGuiSlider::CSWGuiSlider
void __fastcall gui_m48_slider_destruct_0041bb40(void* self, void*);                           // CSWGuiSlider::~CSWGuiSlider
void* __fastcall gui_load_control_construct_0041aa80(void* self, void*) noexcept(false);
void __fastcall gui_load_control_destruct_00419a20(void* self, void*);
extern const unsigned char gui_load_scene_view_methods_00752e30[];
void* __fastcall gui_scene_construct_004174b0(void* scene, void*) noexcept(false);
void __fastcall gui_load_scene_view_destruct_0067ac50(void* view, void*);
void* __fastcall aurora_gui_scene_add_model_name_00417620(void* scene, void*, void* name, I32 index);
void* __fastcall kotor_m15a_creature_construct_00616a20(void* self, void*) noexcept(false);    // CSWCCreature::CSWCCreature
void* __fastcall gui_load_button_toggle_construct_0041ca30(void*, void*) noexcept(false);
void __fastcall gui_load_listbox_clear_items_00419a90(void* listbox, void*);
void __fastcall kotor_fastcall_0041c1d0(void* listbox, void*, void* list, U32, U32, U32);     // CSWGuiListBox::AddControls
void* __fastcall c2da_current_construct_00413cc0(void* self, void*, Ref16 name, std::int32_t);
std::int32_t __fastcall c2da_current_load_004143b0(void* table, void*);
std::int32_t __fastcall c2da_current_get_int_00413660(void* table, void*, I32 row, void* column, I32* out);
extern volatile U32 kotor_cell_do3dgui_0078d1e4;
void* __cdecl live_operator_new(std::size_t);
}


extern "C" {
// ---- method-table thunks
void __fastcall kotor_fastcall_006af940(void* self, void*) { gui_m48_slider_destruct_0041bb40(self, nullptr); }    // jmp 0041BB40
void* __fastcall kotor_fastcall_006af950(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006af940, flags); }
void __fastcall kotor_fastcall_00644a30(void* self, void*) { gui_load_button_destruct_00419da0(self, nullptr); }    // jmp 00419DA0
void* __fastcall kotor_fastcall_0067efa0(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_00644a30, flags); }


// 006E9F10 CSWGuiScriptSelect::CreateOption(int strref, handler, CSWGuiControl* proto) (thiscall, RET 0Ch).
void* __fastcall kotor_fastcall_006e9f10(void* self, void*, U32 strref, const void* handler, void* proto) {
    I32 extent[4] = {0, 0, 0, 0};                                                              // 006E9F35..006E9F41 [esp+14]
    void* raw = live_operator_new(0x2B4);                                                      // 006E9F2E..006E9F45
    void* toggle = raw;
    if (raw != nullptr) {
        { auto w4_guard_0 = ::w4gui::on_throw([&]() { live_free(raw); });  gui_load_button_toggle_construct_0041ca30(raw, nullptr); w4_guard_0.armed = false; }  // 006E9F5B
    }
    vcall<void, void*, void*>(toggle, 0xA0, at(proto, 4), proto);                              // 006E9F66..006E9F7B
    gui_load_text_params_set_strref_00415e50(at(toggle, 0x16C), nullptr, strref);              // 006E9F81..006E9F8C
    extent[3] = vcall<I32>(proto, 0x34);                                                       // 006E9F91..006E9F98
    vcall<void, void*>(toggle, 0x04, extent);                                                  // 006E9F9C..006E9FA5 SetExtent
    gui_load_control_add_event_0041ab20(toggle, nullptr, 0, self, handler);                    // 006E9FA8..006E9FB1
    gui_load_control_add_event_0041ab20(toggle, nullptr, 0x27, self, fn(&kotor_fastcall_006e9e70)); // 006E9FB6..006E9FC0
    return toggle;
}

// ======================= 006EA000 CSWGuiScriptSelect (0xC40, table 007590A8) =======================
void* __fastcall kotor_fastcall_006ea000(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 006EA02B
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_script_select());                                                        // 006EA035 (007590A8)
    zero(self, 0x68); zero(self, 0x6C); zero(self, 0x64);                                      // 006EA03F..006EA045
    void* states = listbox(self, 0x70, u); listbox(self, 0x350, u);                            // 006EA04F..006EA05F
    void* desc = label(self, 0x630, u); label(self, 0x770, u);                                 // 006EA071..006EA081
    void* back = button(self, 0x8B0, u); void* accept = button(self, 0xA74, u);                // 006EA091..006EA0A1
    zero(self, 0xC38);                                                                         // 006EA0B4
    start_layout(self, "ScriptSelect");                                                        // 006EA0A6..006EA0CA
    init_control(self, 0x70, "LST_AIState");                                                   // 006EA0CF
    init_control(self, 0x350, "LB_DESC");                                                      // 006EA102
    init_control(self, 0x770, "LBL_TITLE");                                                    // 006EA138
    init_control(self, 0x8B0, "BTN_Back");                                                     // 006EA175
    init_control(self, 0xA74, "BTN_Accept");                                                   // 006EA1AB
    stop_layout(self);                                                                         // 006EA1E1..006EA1E3
    void* proto = lp(self, 0x61C);                                                             // 006EA1E8 (listbox +350 prototype)
    vcall<void, const void*, void*, float>(desc, 0x98, at(proto, 4), proto, 1.0f);             // 006EA1EE..006EA1FC
    struct { void** data = nullptr; I32 size = 0; I32 allocated = 0; } options;                // [esp+44]
    void* tag = lp(self, 0x33C);                                                               // 006EA210 (listbox +70 prototype)
    void* table = live_operator_new(0x54);                                                     // 006EA216..006EA221
    if (table != nullptr) {
        menu_resources::Ref ref; ref.construct_00406d80("aiscripts");                          // 006EA238..006EA248
        Ref16 by_value; std::memcpy(by_value.bytes, ref.bytes, 16);
        { auto w4_guard_1 = ::w4gui::on_throw([&]() { live_free(table); });  table = c2da_current_construct_00413cc0(table, nullptr, by_value, 0); w4_guard_1.armed = false; }
    }
    { auto w4_guard_2 = ::w4gui::on_throw([&]() { live_free(options.data); }); 
        c2da_current_load_004143b0(table, nullptr);                                            // 006EA25A..006EA261
        I32 rows = li(table, 0x24);                                                            // 006EA266..006EA29C
        if (rows != 0) menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&options), nullptr, static_cast<unsigned>(rows));
        else if (options.data != nullptr) { live_free(options.data); options.data = nullptr; options.allocated = 0; }
        options.size = rows;
        rows = li(table, 0x24);                                                                // 006EA2A0..006EA2D5
        if (rows != 0) directory_list::AllocateTimes004c4f50(reinterpret_cast<directory_list::TimeList*>(at(self, 0x64)), nullptr, rows);
        else if (lp(self, 0x64) != nullptr) { live_free(lp(self, 0x64)); zero(self, 0x64); zero(self, 0x6C); }
        sw(self, 0x68, static_cast<U32>(rows));
        for (I32 row = 0; row < li(table, 0x24); ++row) {                                      // 006EA2D8..006EA3CB
            I32 name = 0, description = 0, state = 0;                                          // [esp+14] [esp+18] [esp+1C]
            { ExoString c("NAME_STRREF"); c2da_current_get_int_00413660(table, nullptr, row, c.storage, &name); }
            { ExoString c("DESCRIPTION_STRREF"); c2da_current_get_int_00413660(table, nullptr, row, c.storage, &description); }
            { ExoString c("AISTATE"); c2da_current_get_int_00413660(table, nullptr, row, c.storage, &state); }
            options.data[row] = kotor_fastcall_006e9f10(self, nullptr, static_cast<U32>(name), fn(&kotor_fastcall_006e9fe0), tag); // 006EA389..006EA3AE
            sw(lp(self, 0x64), static_cast<U32>(row) * 8u, static_cast<U32>(description));    // 006EA3B0..006EA3B7
            sw(lp(self, 0x64), static_cast<U32>(row) * 8u + 4u, static_cast<U32>(state));      // 006EA3BA..006EA3C1
        }
        vcall<void*, U32>(table, 0x00, 1);                                                     // 006EA3D1..006EA3D7 delete
        kotor_fastcall_0041c1d0(states, nullptr, &options, 1, 0, 0);                           // 006EA3D9..006EA3E9
        sw(back, 0x44, lw(back, 0x44) & 0xFFFFFFFBu);                                          // 006EA3EE..006EA409
        gui_load_control_add_event_0041ab20(back, nullptr, 0x27, self, on_b());                // 006EA3FA..006EA40C
        sw(accept, 0x44, lw(accept, 0x44) & 0xFFFFFFFBu);                                      // 006EA411..006EA427
        gui_load_control_add_event_0041ab20(accept, nullptr, 0x27, self, on_accept());         // 006EA41D..006EA42A
        vcall<void, U32>(states, 0xA4, 1);                                                     // 006EA42F..006EA435
        gui_load_panel_set_active_control_0040a630(self, nullptr, states, 0);                  // 006EA43B..006EA440
        w4_guard_2.armed = false; }
    live_free(options.data);                                                                   // 006EA445..006EA44F
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_006e9ac0(void* self, void*) {
    set_vptr(self, vt_script_select());                                                        // 006E9ADE
    gui_load_listbox_clear_items_00419a90(at(self, 0x350), nullptr);                           // 006E9AF4
    gui_load_button_destruct_00419da0(at(self, 0xA74), nullptr);
    gui_load_button_destruct_00419da0(at(self, 0x8B0), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0x770), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0x630), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x350), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x70), nullptr);
    live_free(lp(self, 0x64)); zero(self, 0x64);                                               // 006E9B52..006E9B65
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006e9ef0(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006e9ac0, flags); }

// ======================= 006B0E40 CSWGuiInGameCharacter (0x59F8, table 00756100) =======================
void* __fastcall kotor_fastcall_006b0e40(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 006B0E6B
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_character());                                                            // 006B0E7B (00756100)
    for (U32 i = 0; i < 43; ++i) label(self, 0x64 + i * 0x140, u);                             // 006B0E81..006B1121 (43 labels +64h..+34E4h; t1 review)
    vector_member(at(self, 0x3624), 0x140, 10, &gui_load_label_construct_0041acd0, &gui_load_label_destruct_00419d10, u); // 006B1126..006B1143
    label(self, 0x42A4, u); label(self, 0x43E4, u);                                            // 006B1148..006B1163
    set_vptr(label(self, 0x4524, u), vt_blackened_label());                                    // 006B1168..006B117A (00753CC0)
    set_vptr(label(self, 0x4664, u), vt_blackened_label());                                    // 006B1180..006B1192
    for (U32 off : {0x47A4u, 0x4968u, 0x4B2Cu, 0x4CF0u, 0x4EB4u, 0x5078u, 0x523Cu, 0x5400u}) button(self, off, u); // 006B1198..006B1213
    void* slider = at(self, 0x55C4);
    gui_load_slider_construct_0041baa0(slider, nullptr); u.push(&gui_m48_slider_destruct_0041bb40, slider);  // 006B1218..006B1225
    set_vptr(slider, vt_slider_read_only());                                                   // 006B122A (00756060)
    void* view = at(self, 0x577C);                                                             // 006B1230..006B1254 CSWGui3DSceneView (inlined)
    gui_load_control_construct_0041aa80(view, nullptr);
    set_vptr(view, gui_load_scene_view_methods_00752e30);
    { auto w4_guard_3 = ::w4gui::on_throw([&]() { gui_load_control_destruct_00419a20(view, nullptr); });  gui_scene_construct_004174b0(at(view, 0x5C), nullptr); w4_guard_3.armed = false; }
    u.push(&gui_load_scene_view_destruct_0067ac50, view);
    void* rotate = button(self, 0x580C, u);                                                    // 006B1259..006B1266
    set_vptr(rotate, vt_speed_button());                                                       // 006B126B (007533C8)
    sw(rotate, 0x1C4, 0x3E4CCCCDu); sw(rotate, 0x1C8, 0x3F000000u);                            // 006B1271..006B127B (0.2f / 0.5f)
    sb(self, 0x59F0, 0xFF);                                                                    // 006B1294
    start_layout(self, "character");                                                           // 006B1287..006B12A7
    static const char* const stats[44] = {
        "LBL_NAME", "LBL_CLASS", "LBL_CLASS1", "LBL_CLASS2", "LBL_LEVEL", "LBL_LEVEL1", "LBL_LEVEL2", "LBL_FORTITUDE", "LBL_REFLEX",
        "LBL_WILL", "LBL_FORTITUDE_STAT", "LBL_REFLEX_STAT", "LBL_WILL_STAT", "LBL_EXPERIENCE", "LBL_EXPERIENCE_STAT", "LBL_NEXT_LEVEL",
        "LBL_NEEDED_XP", "LBL_DEFENSE_STAT", "LBL_FORCE_STAT", "LBL_VITALITY_STAT", "LBL_DEFENSE", "LBL_FORCE", "LBL_VITALITY",
        "LBL_STR", "LBL_STRENGTH", "LBL_STR_MOD", "LBL_WIS", "LBL_WISDOM", "LBL_WIS_MOD", "LBL_CHA", "LBL_CHARISMA", "LBL_CHA_MOD",
        "LBL_INT", "LBL_INTELLIGENCE", "LBL_INT_MOD", "LBL_CON", "LBL_CONSTITUTION", "LBL_CON_MOD", "LBL_DEX", "LBL_DEXTERITY",
        "LBL_DEX_MOD", nullptr, nullptr, nullptr};
    for (U32 i = 0; i < 41; ++i) init_control(self, 0x64 + i * 0x140, stats[i]);              // 006B12AC..006B1AF1
    init_control(self, 0x577C, "LBL_3DCHAR");                                                  // 006B1B26
    init_control(self, 0x33A4, "LBL_BEVEL");                                                   // 006B1B5B
    init_control(self, 0x34E4, "LBL_BEVEL2");                                                  // 006B1B90
    init_control(self, 0x55C4, "SLD_ALIGN");                                                   // 006B1BC5
    init_control(self, 0x43E4, "LBL_ADORN");                                                   // 006B1BFA
    init_control(self, 0x42A4, "LBL_MORE");                                                    // 006B1C2F
    init_control(self, 0x4524, "LBL_LIGHT");                                                   // 006B1C64
    init_control(self, 0x4664, "LBL_DARK");                                                    // 006B1C99
    init_control(self, 0x47A4, "BTN_AUTO");                                                    // 006B1CCE
    init_control(self, 0x4968, "BTN_LEVELUP");                                                 // 006B1D03
    init_control(self, 0x4B2C, "BTN_CHANGE1");                                                 // 006B1D38
    init_control(self, 0x4CF0, "BTN_CHANGE2");                                                 // 006B1D6D
    init_control(self, 0x523C, "BTN_EXIT");                                                    // 006B1DA2
    init_control(self, 0x5400, "BTN_SCRIPTS");                                                 // 006B1DD7
    init_control(self, 0x580C, "BTN_3DCHAR");                                                  // 006B1E0C
    init_control(self, 0x4EB4, "BTN_CHARLEFT");                                                // 006B1E3B
    init_control(self, 0x5078, "BTN_CHARRIGHT");                                               // 006B1E70
    U32 name[2]; exo_default(name); u.push(&frame_LogStringDtor_005e5c20, name);               // 006B1EA5..006B1EA9 [esp+1C]
    for (I32 i = 1; i <= 10; ++i) {                                                            // 006B1EB9..006B1F06
        void* good = at(self, 0x3624 + static_cast<U32>(i - 1) * 0x140);
        frame_LogFormat_005e5680(name, "LBL_GOOD%d", i);
        gui_load_panel_init_control_0040b930(self, nullptr, good, name, 1);
        sw(good, 0x44, lw(good, 0x44) & 0xFFFFFFFDu);                                          // 006B1EEE..006B1EF2
    }
    stop_layout(self);                                                                         // 006B1F08..006B1F0A
    sw(self, 0x4EF8, lw(self, 0x4EF8) & 0xFFFFFFFBu);                                          // 006B1F0F..006B1F26
    add_event(self, 0x4EB4, 0x27, self, fn(&kotor_fastcall_006af450));                         // 006B1F1B..006B1F29
    sw(self, 0x50BC, lw(self, 0x50BC) & 0xFFFFFFFBu);                                          // 006B1F2E..006B1F45
    add_event(self, 0x5078, 0x27, self, fn(&kotor_fastcall_006af6d0));                         // 006B1F3A..006B1F48
    sw(self, 0x5100, 0x43340000u); sw(self, 0x5174, 0x43340000u);                              // 006B1F4D..006B1F58 (180.0f)
    sw(self, 0x42E8, lw(self, 0x42E8) & 0xFFFFFFFDu);                                          // 006B1F5E..006B1F6C
    void* camera = lp(self, 0x57F0); void* room = lp(self, 0x57EC);                            // 006B1F72..006B1F8A
    gui_load_control_add_event_0041ab20(rotate, nullptr, 0x16256, self, fn(&kotor_fastcall_006aed90)); // 006B1F67..006B1F8E
    gui_load_control_add_event_0041ab20(rotate, nullptr, 0x16257, self, fn(&kotor_fastcall_006aede0)); // 006B1F93..006B1FA0
    sw(self, 0x59D0, 0x3DCCCCCDu); sw(self, 0x59D4, 0x3DCCCCCDu);                              // 006B1FA5..006B1FB0 (0.1f)
    if (kotor_cell_do3dgui_0078d1e4 != 0) {                                                    // 006B1FB6..006B1FBD
        const float position[3] = {0.0f, 0.0f, 0.0f}, orientation[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        vcall<void, const char*, const void*, const void*>(room, 0x70, "gui3D_room", position, orientation); // 006B1FC3..006B2010
        void* light;
        { ExoString model("charrec_light"); light = aurora_gui_scene_add_model_name_00417620(at(self, 0x57D8), nullptr, model.storage, -1); } // 006B2013..006B2043
        if (light != nullptr) {                                                                // 006B2048..006B204A
            vcall<void, void*, const char*, U32>(camera, 0x74, light, "camerahook", 1);        // 006B204C..006B205A
            vcall<void, U32>(camera, 0x44, 0x41B5CED9u);                                       // 006B205D..006B2068
        }
    }
    {
        void* raw = live_operator_new(0x44C);                                                  // 006B206B..006B2070
        void* creature = nullptr;
        if (raw != nullptr) {
            { auto w4_guard_4 = ::w4gui::on_throw([&]() { live_free(raw); });  creature = kotor_m15a_creature_construct_00616a20(raw, nullptr); w4_guard_4.armed = false; } // 006B2085..006B2087
        }
        sw(self, 0x59E4, ad(creature)); zero(self, 0x59E8); sw(self, 0x59EC, 0xFFFFFFFFu);     // 006B209A..006B20AA
    }
    {
        void* raw = live_operator_new(0xC40);                                                  // 006B2090..006B20B4
        void* select = nullptr;
        if (raw != nullptr) {
            { auto w4_guard_5 = ::w4gui::on_throw([&]() { live_free(raw); });  select = kotor_fastcall_006ea000(raw, nullptr, lp(self, 0x18)); w4_guard_5.armed = false; } // 006B20C9..006B20CF
        }
        sw(self, 0x59F4, ad(select));                                                          // 006B20D8
    }
    sw(self, 0x4B0C, lw(self, 0x4B0C) | 0x40u); sw(self, 0x4948, lw(self, 0x4948) | 0x40u);   // 006B20DE..006B20F1
    struct Ev { U32 off; const void* fn; };
    const Ev events[6] = {{0x47A4, pmf_code(&kotor_m15_l0::PanelEventThunks15::OnYButtonPressed_00644720)}, {0x4968, on_accept()},
                          {0x4B2C, fn(&kotor_fastcall_006af350)}, {0x4CF0, fn(&kotor_fastcall_006af350)}, {0x523C, on_b()}, {0x5400, on_x()}};
    for (const Ev& e : events) {                                                               // 006B20F7..006B21B6
        sw(self, e.off + 0x44, lw(self, e.off + 0x44) & 0xFFFFFFFBu);
        add_event(self, e.off, 0x27, self, e.fn);
    }
    sw(self, 0x5608, lw(self, 0x5608) & 0xFFFFFFFBu);                                          // 006B21BB (SLD_ALIGN)
    sw(self, 0x4B50, 0x9725); sw(self, 0x4B5C, 0xCE);                                          // 006B21C2..006B21CA (BTN_CHANGE1 +24/+30)
    sw(self, 0x4D14, 0x9725);                                                                  // 006B21D5
    u.dismiss();
    frame_LogStringDtor_005e5c20(name, nullptr);                                               // 006B21D1..006B21DF
    return self;
}
void __fastcall kotor_fastcall_006af970(void* self, void*) {
    set_vptr(self, vt_character());                                                            // 006AF98D
    if (void* select = lp(self, 0x59F4)) vcall<void*, U32>(select, 0x00, 1);                   // 006AF993..006AF9A9
    if (void* creature = lp(self, 0x59E4)) vcall<void*, U32>(creature, 0x00, 1);               // 006AF9AB..006AF9B9
    gui_load_button_destruct_00419da0(at(self, 0x580C), nullptr);
    gui_load_scene_view_destruct_0067ac50(at(self, 0x577C), nullptr);
    gui_m48_slider_destruct_0041bb40(at(self, 0x55C4), nullptr);
    for (U32 off : {0x5400u, 0x523Cu, 0x5078u, 0x4EB4u, 0x4CF0u, 0x4B2Cu, 0x4968u, 0x47A4u}) gui_load_button_destruct_00419da0(at(self, off), nullptr);
    for (U32 off : {0x4664u, 0x4524u, 0x43E4u, 0x42A4u}) gui_load_label_destruct_00419d10(at(self, off), nullptr);
    vector_destruct(at(self, 0x3624), 0x140, 10, &gui_load_label_destruct_00419d10);
    for (U32 i = 43; i-- > 0;) gui_load_label_destruct_00419d10(at(self, 0x64 + i * 0x140), nullptr); // 43 labels (t1 review)
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006b2200(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006af970, flags); }
}  // extern "C"
