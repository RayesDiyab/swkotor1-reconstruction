// Lane W4 G15: in-game menu panels built by CGuiInGame::Initialize (00632860) that only assemble generic CSWGui*
// widgets from their .gui layout: ctor / complete dtor / scalar deleting dtor per class. Transcribed from
// build/hudtools/compact.py listings of each address (dis/ann_<ADDR>.asm).
#include "78d2073e39-gui_panels.hpp"
#include "60d326f6b8-claude_handlers.hpp"
using namespace w4gui;

extern "C" {
extern const unsigned char borrowed_enter_color_007a23c0[12];                                // [007A23C0] hilight colour
void __fastcall gui_load_listbox_clear_items_00419a90(void* listbox, void*);                  // CSWGuiListBox::ClearItems
void __fastcall gui_load_border_params_set_pulsing_00414d00(void*, void*, U32, U32, U32);   // CSWGuiBorderParams::SetPulsingAlpha
void* __fastcall gui_load_label_hilight_construct_0041b5c0(void*, void*) noexcept(false);    // CSWGuiLabelHilight
void __fastcall gui_m48_label_hilight_destruct_0041b610(void*, void*);
}

namespace kotor_reconstruction { struct StartupString; }
using SS = kotor_reconstruction::StartupString;
SS* __fastcall GuiConcat005e5d10(SS* self, void*, SS* out, const SS* rhs);                    // CExoString::operator+
namespace kotor_menu_10_19 {
struct String;
struct Tlk { String* __thiscall simple_0041e8f0(String* out, unsigned strref); };              // CTlkTable::GetSimpleString
}
namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction {
void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*);                           // CExoLinkedListInternal::RemoveHead
void __fastcall destroy_list_005e9ab0(kotor_list::Internal*, void*);                            // ~CExoLinkedListInternal
}
namespace recovered_server_create { void* __cdecl construct_list_internal_005e9aa0(void*); }   // CExoLinkedListInternal()
namespace kotor_m15_l0 { struct PanelEventThunks15 { void __thiscall OnYButtonPressed_00644720(kotor_input_hover::Control*); }; }
extern "C" {
extern const unsigned char borrowed_exit_color_007a23b4[12];                                  // [007A23B4] input-cells
}
namespace kotor_input_hover { struct ListWrapper { ListWrapper* __thiscall Construct004b25d0(); }; }   // CExoLinkedList<>()
extern "C" {
void* __fastcall gui_load_control_construct_0041aa80(void* self, void*) noexcept(false);     // CSWGuiControl::CSWGuiControl
void __fastcall gui_load_control_destruct_00419a20(void* self, void*);                       // CSWGuiControl::~CSWGuiControl
void* __fastcall gui_load_image_construct_00415a30(void* self, void*) noexcept(false);       // CSWGuiImage::CSWGuiImage
void __fastcall gui_load_image_destruct_00415ad0(void* self, void*);                         // CSWGuiImage::~CSWGuiImage
void* __fastcall gui_load_image_params_construct_00414d30(void* self, void*, void* resref, U32, U32, U32, U32, float alpha, const void* color); // CSWGuiImageParams
void __fastcall gui_load_image_initialize_00416e90(void* image, void*, const void* extent, void* params); // CSWGuiImage::Initialize
void* __fastcall gui_load_text_params_construct_00417050(void* self, void*);                 // CSWGuiTextParams::CSWGuiTextParams
void* __fastcall gui_load_border_params_construct_00415000(void* self, void*, void* edge, void* corner, void* fill,
                                                          U32 fill_style, U32 dimension, float alpha, const void* color, U32 pulsing); // CSWGuiBorderParams
void* __fastcall c2da_default_ref_00405ed0(void* self, void*);                              // CResRef::CResRef()
void* __cdecl kotor_cdecl_0045a260(const char* name);                                        // NewCAurGUIImage
extern void* kotor_cell_rules_007a3a28;                                                      // [007A3A28] CSWRules*
extern volatile U32 kotor_cell_do3dgui_0078d1e4;
void* __fastcall gui_m48_skill_entry_construct_006acc50(void* self, void*) noexcept(false); // CSWGuiInGameSkillEntry
void __fastcall gui_m48_skill_entry_destruct_006aba80(void* self, void*);
void* __fastcall kotor_m15d_chart_construct_006cd510(void* self, void*);                     // CSWGuiSkillFlowChart
void __fastcall kotor_m15d_chart_destruct_006cd530(void* self, void*);
void __fastcall kotor_m1_radio_button_destruct_006be360(void* self, void*);                 // CSWGuiRadioButton::~CSWGuiRadioButton
void __fastcall gui_load_slot_ret4_0060e760(void*, void*, U32);                              // 0060E760 `ret 4` (void_return)                                            // [0078D1E4] 3D GUI enabled
extern const unsigned char gui_load_scene_view_methods_00752e30[];                          // CSWGui3DSceneView table
void* __fastcall gui_scene_construct_004174b0(void* scene, void*) noexcept(false);           // CSWGuiScene::CSWGuiScene
void __fastcall gui_load_scene_view_destruct_0067ac50(void* view, void*);                    // CSWGui3DSceneView::~
void* __fastcall aurora_gui_scene_add_model_name_00417620(void* scene, void*, void* name, I32 index); // CSWGuiScene::AddModel
std::int32_t __fastcall c2da_current_get_string_00413270(void* table, void*, I32 row, void* column, void* out); // C2DA::GetCExoStringEntry
std::int32_t __fastcall c2da_current_get_int_00413660(void* table, void*, I32 row, void* column, I32* out);    // C2DA::GetINTEntry
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, void* text);          // CResRef(const CExoString&)
void* __fastcall async_ref_assign_string_004061f0(void* ref, void*, void* returned, void* text); // CResRef::operator=(const CExoString&)
void __fastcall gui_load_border_params_set_fill_00414c00(void* params, void*, const void* ref, I32) noexcept(false); // SetFillImage
}
namespace w4 {
// [007A23F0] map note colour (0.98, 0.45, 0): .data, never written; read here and by 006029C5 / 006877E3. W4-owned.
extern const float map_note_color_007a23f0[3] = {0.98f, 0.45f, 0.0f};
}
// [00833A90] journal sort mode (.bss, 0), also read/written by save/load at 004F1838/00563480/00563DAE. ONE cell:
// the C-linkage definition `recovered_journal_sort_order_00833a90` in w2_ifo_native.obj (save/load owner) is the
// storage; the W4 name `w4::journal_sort_00833a90` (still referenced by claude_gui_journal / claude_loadgame) is a
// linker alias of it, so every reader and writer of the original cell shares the same 4 bytes.
extern "C" volatile U32 recovered_journal_sort_order_00833a90;                              // w2_ifo_native.obj
#pragma comment(linker, "/alternatename:?journal_sort_00833a90@w4@@3IC=_recovered_journal_sort_order_00833a90")
namespace w4 { extern volatile U32 journal_sort_00833a90; }                                  // = [00833A90] (alias)
namespace {
// [007A2474] .data strref table for the four journal sort modes (image values).
const U32 journal_sort_strref_007a2474[4] = {0x7DAD, 0x7DAE, 0x7DAF, 0x7DB0};
SS* ss(void* s) { return static_cast<SS*>(s); }
void tlk_simple(U32* out, U32 strref) {
    static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08)->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(out), strref);
}
// CSWGuiQuestItem +64 CExoLinkedList: ~CExoLinkedListInternal + free (006D29A4..006D29BD).
void __fastcall quest_item_list_release(void* self, void*) {
    if (void* internal = lp(self, 0x64)) {
        base_ini_reconstruction::destroy_list_005e9ab0(static_cast<kotor_list::Internal*>(internal), nullptr);
        live_free(internal);
    }
}
// CSWGuiMapHider +238 CExoLinkedList: ~CExoLinkedListInternal + free (0069435F..0069437B).
void __fastcall map_hider_notes_release(void* self, void*) {
    if (void* internal = lp(self, 0x238)) {
        base_ini_reconstruction::destroy_list_005e9ab0(static_cast<kotor_list::Internal*>(internal), nullptr);
        live_free(internal);
    }
}
// new CSWGuiImage (0x54; block freed if the ctor throws; null when operator new returns null).
void* new_image() {
    void* raw = live_operator_new(0x54);
    if (raw == nullptr) return nullptr;
    { auto w4_guard_0 = ::w4gui::on_throw([&]() { live_free(raw); }); void* w4_result_1 = gui_load_image_construct_00415a30(raw, nullptr); w4_guard_0.armed = false; return w4_result_1; }
}
void* planets_2da() { return lp(lp(kotor_cell_rules_007a3a28, 0xB8), 0x104); }              // [[[7A3A28]+B8]+104] planets.2da
// CSWGuiStore item entry arrays (+74 / +68 CExoArrayList data): free + clear (006C1600..006C1628).
void __fastcall store_lists_release(void* self, void*) {
    live_free(lp(self, 0x74)); zero(self, 0x74);
    live_free(lp(self, 0x68)); zero(self, 0x68);
}
// CSWGuiInGameAbilities +64 CExoArrayList data: free + clear (006ABCA9..006ABCBD).
void __fastcall abilities_list_release(void* self, void*) { live_free(lp(self, 0x64)); zero(self, 0x64); }
// tlk(head) + sep + tlk(tail): constructed straight into `out` (assign=false) or assigned to it (assign=true).
// Temporaries are fetched / destroyed in the original order (tail first; destroyed joined, head, tail, sep).
void tlk_join(U32* out, U32 head_ref, const char* sep, U32 tail_ref, bool assign) {
    ExoString separator(sep);
    U32 tail[2], head[2], joined[2];
    tlk_simple(tail, tail_ref);
    tlk_simple(head, head_ref);
    GuiConcat005e5d10(ss(head), nullptr, ss(joined), ss(separator.storage));
    if (!assign) {
        GuiConcat005e5d10(ss(joined), nullptr, ss(out), ss(tail));
    } else {
        U32 full[2];
        GuiConcat005e5d10(ss(joined), nullptr, ss(full), ss(tail));
        async_assign_string_005e5c50(out, nullptr, full);
        frame_LogStringDtor_005e5c20(full, nullptr);
    }
    frame_LogStringDtor_005e5c20(joined, nullptr);
    frame_LogStringDtor_005e5c20(head, nullptr);
    frame_LogStringDtor_005e5c20(tail, nullptr);
}
// [00756444] .rdata strref table for the six inventory filter modes (image values).
const U32 inventory_filter_strref_00756444[6] = {0xA35E, 0xA4B5, 0xA35A, 0xA35D, 0xA35B, 0xA35C};
// CSWGuiInGameInventory +1DC0 entries (owned, deleted) / +1DCC array: 006B4108..006B417E.
void __fastcall inventory_lists_release(void* self, void*) {
    for (I32 i = 0; i < li(self, 0x1DC4); ++i) {
        if (void* entry = lp(lp(self, 0x1DC0), static_cast<U32>(i) * 4u)) vcall<void*, U32>(entry, 0x00, 1);
        sw(lp(self, 0x1DC0), static_cast<U32>(i) * 4u, 0);
    }
    zero(self, 0x1DC4);
    live_free(lp(self, 0x1DCC)); zero(self, 0x1DCC);
    live_free(lp(self, 0x1DC0)); zero(self, 0x1DC0);
}
// Listbox prototype item (listbox +2CC): border colour <- [007A23C0], SetPulsingAlpha(1, 1, 0).
void proto_hilight(void* proto) {
    std::memcpy(at(proto, 0x164), borrowed_enter_color_007a23c0, 12);
    gui_load_border_params_set_pulsing_00414d00(at(proto, 0x154), nullptr, 1, 1, 0);
}
}

extern "C" {
// ======================= 00626400 CSWGuiInGameMessages (0xAF4, table 0074FD18) =======================
void* __fastcall kotor_fastcall_00626400(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 00626427
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_messages());                                                             // 00626439 (0074FD18)
    void* messages = listbox(self, 0x64, u); void* dialog = listbox(self, 0x344, u);           // 0062643F..0062644F
    label(self, 0x624, u); void* show = button(self, 0x76C, u); void* exit = button(self, 0x930, u);  // 00626461..00626483
    I32 extent[4] = {0, 0, 0, 0};                                                              // 00626498..006264A4 [esp+24]
    start_layout(self, "messages");                                                            // 0062648A..006264B4
    init_control(self, 0x64, "LB_MESSAGES");                                                   // 006264B9
    init_control(self, 0x624, "LBL_MESSAGES");                                                 // 006264E9
    init_control(self, 0x344, "LB_DIALOG");                                                    // 00626519
    init_control(self, 0x76C, "BTN_SHOW");                                                     // 0062654F
    init_control(self, 0x930, "BTN_EXIT");                                                     // 0062657F
    stop_layout(self);                                                                         // 006265B5
    for (U32 k = 0; k < 2; ++k) {                                                              // 006265BC..00626738
        void* proto = lp(self, k == 0 ? 0x330u : 0x610u);
        proto_hilight(proto);
        void* items = new_vector(0x40, 0x1B4, &gui_load_label_hilight_construct_0041b5c0, &gui_m48_label_hilight_destruct_0041b610);
        sw(self, k == 0 ? 0x764u : 0x768u, ad(items));                                         // 0062663D / 006266F8
        for (U32 off = 0; off < 0x6D00; off += 0x1B4)                                          // vcall +A0 Initialize(extent, proto)
            vcall<void, void*, void*>(at(lp(self, k == 0 ? 0x764u : 0x768u), off), 0xA0, extent, proto);
    }
    sw(messages, 0x2BC, lw(messages, 0x2BC) | 0x400u);                                         // 0062673A..0062674F
    gui_load_control_add_event_0041ab20(show, nullptr, 0x27, self, on_x());                    // 00626740..00626755
    sw(show, 0x44, lw(show, 0x44) & 0xFFFFFFFBu);                                              // 0062675A..00626769
    gui_load_control_add_event_0041ab20(exit, nullptr, 0x27, self, on_b());                    // 00626764..00626777
    sw(exit, 0x44, lw(exit, 0x44) & 0xFFFFFFFBu);                                              // 0062677C
    vcall<void, U32>(messages, 0xA4, 1);                                                       // 0062677F..00626785
    vcall<void, U32>(dialog, 0xA4, 1);                                                         // 0062678B..00626799
    gui_load_panel_set_active_control_0040a630(self, nullptr, messages, 0);                    // 0062679F..006267A4
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_00626820(void* self, void*) {
    set_vptr(self, vt_messages());                                                             // 0062683F
    gui_load_listbox_clear_items_00419a90(at(self, 0x344), nullptr);                           // 00626855
    gui_load_listbox_clear_items_00419a90(at(self, 0x64), nullptr);                            // 0062685F
    delete_vector(lp(self, 0x768));                                                            // 00626864..00626886
    delete_vector(lp(self, 0x764));                                                            // 00626889..006268AB
    gui_load_button_destruct_00419da0(at(self, 0x930), nullptr);
    gui_load_button_destruct_00419da0(at(self, 0x76C), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0x624), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x344), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x64), nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_00628240(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_00626820, flags); }

// ======================= 006B6DC0 CSWGuiContainer (0x101C, table 007567E0) =======================
void* __fastcall kotor_fastcall_006b6dc0(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 006B6DEB
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_container());                                                            // 006B6DFB (007567E0)
    for (U32 off : {0x70u, 0x1B0u, 0x2F0u, 0x430u, 0x570u, 0x6B0u}) label(self, off, u);       // 006B6E01..006B6E51
    void* items = listbox(self, 0x7F0, u);                                                     // 006B6E63
    void* ok = button(self, 0xAD0, u); void* cancel = button(self, 0xC94, u); void* give = button(self, 0xE58, u); // 006B6E75..006B6E95
    start_layout(self, "container");                                                           // 006B6E9A..006B6EB4
    init_control(self, 0x70, "LBL_MESSAGE");                                                   // 006B6EB9
    init_control(self, 0x1B0, "LBL_BUTTON");                                                   // 006B6EEC
    init_control(self, 0x2F0, "LBL_OK");                                                       // 006B6F22
    init_control(self, 0x7F0, "LB_ITEMS");                                                     // 006B6F58
    init_control(self, 0x430, "LBL_B");                                                        // 006B6F88
    init_control(self, 0x6B0, "LBL_X");                                                        // 006B6FBE
    init_control(self, 0x570, "LBL_CANCEL");                                                   // 006B6FF4
    init_control(self, 0xAD0, "BTN_OK");                                                       // 006B702A
    init_control(self, 0xE58, "BTN_GIVEITEMS");                                                // 006B705A
    init_control(self, 0xC94, "BTN_CANCEL");                                                   // 006B7090
    stop_layout(self);                                                                         // 006B70C6
    sw(self, 0x6C, (lw(self, 0x6C) & 0xFFFFFFF9u) | 1u);                                       // 006B70CD..006B70D6
    sw(items, 0x2BC, lw(items, 0x2BC) | 0x200u);                                               // 006B70D9
    vcall<void, U32>(items, 0x88, 0);                                                          // 006B70E3..006B70E9
    sb(self, 0xAB1, 1);                                                                        // 006B70EF
    sw(ok, 0x44, lw(ok, 0x44) & 0xFFFFFFFBu);                                                  // 006B70F6..006B7106
    gui_load_control_add_event_0041ab20(ok, nullptr, 0x27, self, on_accept());                 // 006B70F9..006B710D
    sw(give, 0x44, lw(give, 0x44) & 0xFFFFFFFBu);                                              // 006B7112..006B7128
    gui_load_control_add_event_0041ab20(give, nullptr, 0x27, self, on_x());                    // 006B711E..006B712B
    sw(cancel, 0x44, lw(cancel, 0x44) & 0xFFFFFFFBu);                                          // 006B7130..006B713E
    gui_load_control_add_event_0041ab20(cancel, nullptr, 0x27, self, on_b());                  // 006B7141..006B7149
    gui_m48_panel_center_0040a600(self, nullptr);                                              // 006B714E..006B7150
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_006b8050(void* self, void*) {
    for (U32 off : {0xE58u, 0xC94u, 0xAD0u}) gui_load_button_destruct_00419da0(at(self, off), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x7F0), nullptr);
    for (U32 off : {0x6B0u, 0x570u, 0x430u, 0x2F0u, 0x1B0u, 0x70u}) gui_load_label_destruct_00419d10(at(self, off), nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006b8030(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006b8050, flags); }

// ======================= 006272A0 CSWGuiStatusSummary (0x1B44, table 0074FF68) =======================
void* __fastcall kotor_fastcall_006272a0(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 006272CB
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_status_summary());                                                       // 006272DE (0074FF68)
    label(self, 0x80, u); label(self, 0x1C0, u);                                               // 006272E4..006272F4
    vector_member(at(self, 0x300), 0x140, 9, &gui_load_label_construct_0041acd0, &gui_load_label_destruct_00419d10, u);  // 006272F9..00627316
    vector_member(at(self, 0xE40), 0x140, 9, &gui_load_label_construct_0041acd0, &gui_load_label_destruct_00419d10, u);  // 0062731B..00627338
    void* ok = button(self, 0x1980, u);                                                        // 0062734A
    start_layout(self, "statussummary");                                                       // 00627351..0062736A
    static const char* const names[9][2] = {
        {"LBL_JOURNAL", "LBL_JOURNAL_DESC"}, {"LBL_CREDITS", "LBL_CREDITS_DESC"}, {"LBL_XP", "LBL_XP_DESC"},
        {"LBL_STEALTH", "LBL_STEALTH_DESC"}, {"LBL_DARKSIDE", "LBL_DARKSIDE_DESC"}, {"LBL_LIGHTSIDE", "LBL_LIGHTSIDE_DESC"},
        {"LBL_NETSHIFT", "LBL_NETSHIFT_DESC"}, {"LBL_RECEIVED", "LBL_RECEIVED_DESC"}, {"LBL_LOST", "LBL_LOST_DESC"}};
    for (U32 i = 0; i < 9; ++i) {                                                              // 0062736F..006276F4
        init_control(self, 0x300 + i * 0x140, names[i][0]);
        init_control(self, 0xE40 + i * 0x140, names[i][1]);
    }
    init_control(self, 0x1980, "BTN_OK");                                                      // 00627729
    stop_layout(self);                                                                         // 00627758
    sw(ok, 0x44, lw(ok, 0x44) & 0xFFFFFFFBu);                                                  // 0062775F..0062776B
    gui_load_control_add_event_0041ab20(ok, nullptr, 0x27, self, on_accept());                 // 00627762..00627772
    const U32 flags64 = lw(self, 0x64);                                                        // 00627777
    zero(self, 0x6C); sb(self, 0x68, 0); sb(self, 0x69, 0);                                    // 0062777C..00627782
    zero(self, 0x70); zero(self, 0x74); zero(self, 0x78);                                      // 00627785..0062778B
    sw(self, 0x44, lw(self, 0x44) | 1u);                                                       // 0062778E..0062779A
    sw(self, 0x64, flags64 & 0xFFFFFE00u);                                                     // 00627791, 0062779D
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_00628500(void* self, void*) {
    gui_load_button_destruct_00419da0(at(self, 0x1980), nullptr);
    vector_destruct(at(self, 0xE40), 0x140, 9, &gui_load_label_destruct_00419d10);
    vector_destruct(at(self, 0x300), 0x140, 9, &gui_load_label_destruct_00419d10);
    gui_load_label_destruct_00419d10(at(self, 0x1C0), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0x80), nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006284e0(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_00628500, flags); }

// ======================= 00627980 CSWGuiInGameMenu (0x1C24, table 00750148) =======================
void* __fastcall kotor_fastcall_00627980(void* self, void*, void* manager) {
    static const U32 hilights[8] = {0x64, 0x218, 0x3CC, 0x580, 0x734, 0x8E8, 0xA9C, 0xC50};
    static const U32 buttons[8] = {0xE04, 0xFC8, 0x118C, 0x1350, 0x1514, 0x16D8, 0x189C, 0x1A60};
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 006279AB
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_ingame_menu());                                                          // 006279BB (00750148)
    for (U32 off : hilights) {                                                                 // 006279C1..00627A31
        gui_load_label_hilight_construct_0041b5c0(at(self, off), nullptr); u.push(&gui_m48_label_hilight_destruct_0041b610, at(self, off));
    }
    for (U32 off : buttons) button(self, off, u);                                              // 00627A43..00627AB3
    start_layout(self, "top");                                                                 // 00627ABA..00627AD3
    static const char* const hilight_names[8] = {"LBLH_EQU", "LBLH_INV", "LBLH_CHA", "LBLH_MAP", "LBLH_ABI", "LBLH_JOU", "LBLH_OPT", "LBLH_MSG"};
    static const char* const button_names[8] = {"BTN_EQU", "BTN_INV", "BTN_CHAR", "BTN_MAP", "BTN_ABI", "BTN_JOU", "BTN_OPT", "BTN_MSG"};
    for (U32 i = 0; i < 8; ++i) init_control(self, hilights[i], hilight_names[i]);             // 00627AD8..00627C48
    for (U32 i = 0; i < 8; ++i) init_control(self, buttons[i], button_names[i]);               // 00627C7D..00627DEA
    stop_layout(self);                                                                         // 00627E1F
    for (U32 off : hilights) add_event(self, off, 0x35, self, fn(&kotor_fastcall_00624c00));   // 00627E26..00627EB8
    for (U32 off : hilights) add_event(self, off, 0x36, self, fn(&kotor_fastcall_00624c30));   // 00627EBD..00627F4C
    static void (__fastcall* const pressed[8])(void*, void*, void*) = {
        &kotor_fastcall_00624cf0, &kotor_fastcall_00624d10, &kotor_fastcall_00624d30, &kotor_fastcall_00624d50,
        &kotor_fastcall_00624d70, &kotor_fastcall_00624d90, &kotor_fastcall_00624db0, &kotor_fastcall_00624dd0};
    for (U32 i = 0; i < 8; ++i) add_event(self, buttons[i], 0x27, self, fn(pressed[i]));       // 00627F51..00627FE2
    for (U32 off : buttons) sw(self, off + 0x44, lw(self, off + 0x44) & 0xFFFFFFFBu);          // 00627FE7..0062801F
    gui_load_panel_set_active_control_0040a630(self, nullptr, at(self, 0xE04), 0);             // 0062801D..00628028
    sw(self, 0x44, lw(self, 0x44) | 0x60u);                                                    // 0062802D
    static const U32 strings[8][2] = {{0xBC5B, 0xD1}, {0xBC5C, 0xD2}, {0xBC61, 0xD3}, {0xBC5D, 0xD7},
                                      {0xBC60, 0xD4}, {0xBC5A, 0xD6}, {0xBC5E, 0xD8}, {0xBC5F, 0xD5}};   // 00628031..006280C0
    for (U32 i = 0; i < 8; ++i) { sw(self, buttons[i] + 0x24, strings[i][0]); sw(self, buttons[i] + 0x30, strings[i][1]); }
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_00628100(void* self, void*) {
    for (U32 off : {0x1A60u, 0x189Cu, 0x16D8u, 0x1514u, 0x1350u, 0x118Cu, 0xFC8u, 0xE04u}) gui_load_button_destruct_00419da0(at(self, off), nullptr);
    for (U32 off : {0xC50u, 0xA9Cu, 0x8E8u, 0x734u, 0x580u, 0x3CCu, 0x218u, 0x64u}) gui_m48_label_hilight_destruct_0041b610(at(self, off), nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006280e0(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_00628100, flags); }

// ======================= 006D26C0 CSWGuiQuestItem (0xA70, table 00757C20) =======================
// 006D2460 CExoLinkedList<CSWGuiInGameItemEntry>::CExoLinkedList(): new CExoLinkedListInternal (12 bytes) -> [this].
void* __fastcall kotor_fastcall_006d2460(void* self, void*) {
    void* raw = live_operator_new(0xC);                                                        // 006D2477..006D247B
    void* internal = nullptr;
    if (raw != nullptr) internal = recovered_server_create::construct_list_internal_005e9aa0(raw); // 006D2493..006D2495
    sw(self, 0, ad(internal));                                                                 // 006D249A / 006D24B4
    return self;
}
void* __fastcall kotor_fastcall_006d26c0(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 006D26EB
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_quest_item());                                                           // 006D26FB (00757C20)
    kotor_fastcall_006d2460(at(self, 0x64), nullptr); u.push(&quest_item_list_release, self);  // 006D2701
    listbox(self, 0x68, u); void* title = label(self, 0x348, u);                               // 006D270E..006D271E
    void* items = listbox(self, 0x488, u); label(self, 0x768, u); void* back = button(self, 0x8A8, u);  // 006D2730..006D2752
    zero(self, 0xA6C);                                                                         // 006D2765
    start_layout(self, "QuestItem");                                                           // 006D2757..006D277B
    init_control(self, 0x488, "LB_ITEMS");                                                     // 006D2780
    init_control(self, 0x68, "LB_ITEM_DESCRIPTION");                                           // 006D27B0
    init_control(self, 0x768, "LBL_TITLE");                                                    // 006D27E3
    init_control(self, 0x8A8, "BTN_BACK");                                                     // 006D2819
    void* proto = lp(self, 0x334);                                                             // 006D2849 (listbox +68 prototype)
    vcall<void, const void*, void*, float>(title, 0x98, at(proto, 4), proto, 1.0f);             // 006D284F..006D2865 Initialize
    stop_layout(self);                                                                         // 006D286B..006D286D
    sw(items, 0x2BC, lw(items, 0x2BC) | 0x20u);                                                // 006D2872..006D2884
    { ExoString empty(""); gui_load_text_params_set_text_00415e00(at(self, 0x430), nullptr, empty.storage); }  // 006D287B..006D28AD (0073D71C)
    sw(back, 0x44, lw(back, 0x44) & 0xFFFFFFFBu);                                              // 006D28B2..006D28C2
    gui_load_control_add_event_0041ab20(back, nullptr, 0x27, self, on_b());                    // 006D28B5..006D28C5
    gui_load_panel_set_active_control_0040a630(self, nullptr, items, 0);                       // 006D28CA..006D28CF
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_006d28f0(void* self, void*) {
    set_vptr(self, vt_quest_item());                                                           // 006D290F
    gui_load_listbox_clear_items_00419a90(at(self, 0x488), nullptr);                           // 006D2925
    gui_load_listbox_clear_items_00419a90(at(self, 0x68), nullptr);                            // 006D292F
    while (lw(lp(self, 0x64), 0) != 0) {                                                       // 006D2934..006D295A
        void* entry = base_ini_reconstruction::remove_head_005e9c80(static_cast<kotor_list::Internal*>(lp(self, 0x64)), nullptr);
        if (entry != nullptr) vcall<void*, U32>(entry, 0x00, 1);                               // 006D2948..006D2952
    }
    gui_load_button_destruct_00419da0(at(self, 0x8A8), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0x768), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x488), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0x348), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x68), nullptr);
    quest_item_list_release(self, nullptr);                                                    // 006D29A4..006D29BD
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006d2c00(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006d28f0, flags); }

// ======================= 00644A40 CSWGuiInGameJournal (0xFC8, table 00751960) =======================
void* __fastcall kotor_fastcall_00644a40(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 00644A6B
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_journal());                                                              // 00644A7B (00751960)
    label(self, 0x64, u); listbox(self, 0x1A4, u); void* desc = label(self, 0x484, u);         // 00644A81..00644AA1
    void* items = listbox(self, 0x5C4, u);                                                     // 00644AB3
    void* quest = button(self, 0x8A4, u); void* swap = button(self, 0xA68, u);                 // 00644AC5..00644AD5
    void* sort = button(self, 0xC2C, u); void* exit = button(self, 0xDF0, u);                  // 00644AE5..00644AF5
    zero(self, 0xFC0); zero(self, 0xFC4); zero(self, 0xFBC); zero(self, 0xFB8);                // 00644AFA..00644B18
    void* raw = live_operator_new(0xA70);                                                      // 00644B0E..00644B1E
    void* quest_item = nullptr;
    if (raw != nullptr) {
        { auto w4_guard_1 = ::w4gui::on_throw([&]() { live_free(raw); });  quest_item = kotor_fastcall_006d26c0(raw, nullptr, lp(self, 0x18)); w4_guard_1.armed = false; }
    }
    sw(self, 0xFB4, ad(quest_item));                                                           // 00644B50
    start_layout(self, "journal");                                                             // 00644B42..00644B62
    init_control(self, 0x64, "LBL_TITLE");                                                     // 00644B67
    init_control(self, 0x5C4, "LB_ITEMS");                                                     // 00644B9D
    init_control(self, 0x1A4, "LBL_ITEM_DESCRIPTION");                                         // 00644BD0
    init_control(self, 0x8A4, "BTN_QUESTITEMS");                                               // 00644C09
    init_control(self, 0xA68, "BTN_SWAPTEXT");                                                 // 00644C3C
    init_control(self, 0xC2C, "BTN_SORT");                                                     // 00644C75
    init_control(self, 0xDF0, "BTN_EXIT");                                                     // 00644CAE
    stop_layout(self);                                                                         // 00644CE7..00644CE9
    void* proto = lp(self, 0x470);                                                             // 00644CEE (listbox +1A4 prototype)
    vcall<void, const void*, void*, float>(desc, 0x98, at(proto, 4), proto, 1.0f);              // 00644CF4..00644D0A
    void* item_proto = lp(self, 0x890);                                                        // 00644D10 (listbox +5C4 prototype)
    std::memcpy(at(item_proto, 0x90), borrowed_exit_color_007a23b4, 12);                       // 00644D16..00644D33
    std::memcpy(at(item_proto, 0x104), borrowed_enter_color_007a23c0, 12);                     // 00644D36..00644D57
    gui_load_border_params_set_pulsing_00414d00(at(item_proto, 0xF4), nullptr, 1, 1, 0);       // 00644D53..00644D62
    sw(items, 0x2BC, lw(items, 0x2BC) | 0x20u);                                                // 00644D67
    sw(quest, 0x44, lw(quest, 0x44) & 0xFFFFFFFBu);                                            // 00644D6E..00644D7A
    gui_load_control_add_event_0041ab20(quest, nullptr, 0x27, self, on_x());                   // 00644D71..00644D81
    sw(swap, 0x44, lw(swap, 0x44) & 0xFFFFFFFBu);                                              // 00644D86..00644DA1
    gui_load_control_add_event_0041ab20(swap, nullptr, 0x27, self,
        pmf_code(&kotor_m15_l0::PanelEventThunks15::OnYButtonPressed_00644720));               // 00644D92..00644DA4
    sw(sort, 0x44, lw(sort, 0x44) & 0xFFFFFFFBu);                                              // 00644DA9..00644DBF
    gui_load_control_add_event_0041ab20(sort, nullptr, 0x27, self, fn(&kotor_fastcall_00644730)); // 00644DB5..00644DC2
    sw(exit, 0x44, lw(exit, 0x44) & 0xFFFFFFFBu);                                              // 00644DC7..00644DDD
    gui_load_control_add_event_0041ab20(exit, nullptr, 0x27, self, on_b());                    // 00644DD3..00644DE0
    vcall<void, U32>(items, 0xA4, 1);                                                          // 00644DE5..00644DEB
    gui_load_panel_set_active_control_0040a630(self, nullptr, items, 0);                       // 00644DF1..00644DF6
    // Sort button text: tlk(0xA646) + " " + tlk(sort strref[(sort + 1) % 4]).
    I32 next = static_cast<I32>(w4::journal_sort_00833a90) + 1;                                // 00644DFB..00644E07
    if (next >= 4) next = 0;
    U32 text[2];                                                                               // [esp+18]
    {
        ExoString space(" ");                                                                  // 00644E09..00644E12 (0073D5D8)
        U32 name[2], prefix[2], joined[2];                                                     // [esp+38] [esp+30] [esp+28]
        tlk_simple(name, journal_sort_strref_007a2474[next]);                                  // 00644E17..00644E2F
        tlk_simple(prefix, 0xA646);                                                            // 00644E36..00644E4B
        GuiConcat005e5d10(ss(prefix), nullptr, ss(joined), ss(space.storage));                 // 00644E50..00644E61
        GuiConcat005e5d10(ss(joined), nullptr, ss(text), ss(name));                            // 00644E66..00644E73
        frame_LogStringDtor_005e5c20(joined, nullptr);                                         // 00644E78..00644E81
        frame_LogStringDtor_005e5c20(prefix, nullptr);                                         // 00644E86..00644E8F
        frame_LogStringDtor_005e5c20(name, nullptr);                                           // 00644E94..00644E9D
    }                                                                                          // 00644EA4..00644EAC ~space
    gui_load_text_params_set_text_00415e00(at(sort, 0x16C), nullptr, text);                    // 00644EB1..00644EBC (+D98)
    // Swap button strref / title text follow CGuiInGame +BC4 bit 0.
    const bool flag = (lb(pt(frame_R4_005ed690(client_app(), nullptr)), 0xBC4) & 1u) != 0;  // 00644EC1..00644EDB
    gui_load_text_params_set_strref_00415e50(at(swap, 0x16C), nullptr, flag ? 0x7DB2u : 0x7DB1u); // 00644EE1 / 00644FA7 (+BD4)
    {
        ExoString dash(" - ");                                                                 // 00644EEB / 00644FB1 (00748E98)
        U32 name[2], head[2], joined[2], full[2];
        tlk_simple(name, journal_sort_strref_007a2474[w4::journal_sort_00833a90]);             // 00644EF9..00644F17 / 00644FBF..00644FDD
        tlk_simple(head, flag ? 0x7DB1u : 0x7DB2u);                                            // 00644F1E..00644F33 / 00644FE4..00644FF9
        GuiConcat005e5d10(ss(head), nullptr, ss(joined), ss(dash.storage));                    // 00644F38..00644F49 / 00644FFE..0064500F
        GuiConcat005e5d10(ss(joined), nullptr, ss(full), ss(name));                            // 00644F4E..00644F5B / 00645014..00645021
        async_assign_string_005e5c50(text, nullptr, full);                                     // 00644F60..00644F6A / 00645026..00645030
        frame_LogStringDtor_005e5c20(full, nullptr);
        frame_LogStringDtor_005e5c20(joined, nullptr);
        frame_LogStringDtor_005e5c20(head, nullptr);
        frame_LogStringDtor_005e5c20(name, nullptr);                                           // .. 00645068
    }                                                                                          // 0064506D..00645075 ~dash
    gui_load_text_params_set_text_00415e00(at(self, 0x14C), nullptr, text);                    // 0064507A..00645085 (LBL_TITLE)
    { ExoString empty(""); gui_load_text_params_set_text_00415e00(at(self, 0x56C), nullptr, empty.storage); }  // 0064508A..006450B5 (0073D71C)
    frame_LogStringDtor_005e5c20(text, nullptr);                                               // 006450BA..006450C3
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_00644850(void* self, void*) {
    set_vptr(self, vt_journal());                                                              // 0064486F
    gui_load_listbox_clear_items_00419a90(at(self, 0x1A4), nullptr);                           // 00644885
    gui_load_listbox_clear_items_00419a90(at(self, 0x5C4), nullptr);                           // 00644892
    if (void* quest_item = lp(self, 0xFB4)) vcall<void*, U32>(quest_item, 0x00, 1);            // 00644897..006448A5
    live_free(lp(self, 0xFBC)); zero(self, 0xFBC);                                             // 006448A7..006448C1
    for (U32 off : {0xDF0u, 0xC2Cu, 0xA68u, 0x8A4u}) gui_load_button_destruct_00419da0(at(self, off), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x5C4), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0x484), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x1A4), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0x64), nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006450e0(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_00644850, flags); }

// ======================= 00693F60 CSWGuiMapHider (control, 0x248, table 00754738) =======================
// 00692E30 CSWGuiMapHider::ClearMapNotes() RET 0.
void __fastcall kotor_fastcall_00692e30(void* self, void*) {
    while (lw(lp(self, 0x238), 0) != 0) {                                                      // 00692E33..00692E6C
        void* head = lp(lp(self, 0x238), 0);
        live_free(head != nullptr ? lp(head, 8) : nullptr);                                    // 00692E46..00692E50
        base_ini_reconstruction::remove_head_005e9c80(static_cast<kotor_list::Internal*>(lp(self, 0x238)), nullptr); // 00692E55..00692E5E
    }
    zero(self, 0x23C);                                                                         // 00692E6E
}
void* __fastcall kotor_fastcall_00693f60(void* self, void*) {
    gui_load_control_construct_0041aa80(self, nullptr);                                        // 00693F85
    Unwind u; u.push(&gui_load_control_destruct_00419a20, self);
    set_vptr(self, vt_map_hider());                                                            // 00693F96 (00754738)
    void* marker = button(self, 0x68, u);                                                      // 00693F9C
    zero(self, 0x230); zero(self, 0x234); zero(self, 0x22C);                                   // 00693FA1..00693FAD
    reinterpret_cast<kotor_input_hover::ListWrapper*>(at(self, 0x238))->Construct004b25d0();    // 00693FB3..00693FC1
    u.push(&map_hider_notes_release, self);
    kotor_fastcall_00692e30(self, nullptr);                                                    // 00693FC8..00693FD1
    zero(self, 0x244);                                                                         // 00693FDB
    sw(self, 0x5C, ad(kotor_cdecl_0045a260("blackdot")));                                      // 00693FD6..00693FE8 (00754034)
    sw(self, 0x60, ad(new_image()));                                                           // 00693FE6..00694017
    sw(self, 0x64, ad(new_image()));                                                           // 0069400E..0069403D
    float white[3] = {1.0f, 1.0f, 1.0f};                                                       // [esp+10]
    if (lp(self, 0x60) != nullptr) {                                                           // 00694040..0069404A
        menu_resources::Ref ref; ref.construct_00406d80("mm_barrow");                          // 0069404C..00694055
        const I32 extent[4] = {0, 0, 0x20, 0x20};                                              // [esp+1C]
        unsigned char params[0x60];                                                            // [esp+A4] CSWGuiImageParams
        void* p = gui_load_image_params_construct_00414d30(params, nullptr, &ref, 0, 0, 0, 0x12, 1.0f, white); // 00694067..006940A2
        gui_load_image_initialize_00416e90(lp(self, 0x60), nullptr, extent, p);                // 006940A7..006940B0
        sw(lp(self, 0x60), 0x4C, lw(lp(self, 0x60), 0x4C) | 1u);                               // 006940B5..006940C1
    }
    if (lp(self, 0x64) != nullptr) {                                                           // 006940C4..006940C7
        menu_resources::Ref ref; ref.construct_00406d80("lbl_mapcircle");                      // 006940CD..006940D6
        const I32 extent[4] = {0, 0, 0x10, 0x10};
        unsigned char params[0x60];
        void* p = gui_load_image_params_construct_00414d30(params, nullptr, &ref, 0, 0, 0, 0x12, 1.0f, white); // 006940E8..00694123
        gui_load_image_initialize_00416e90(lp(self, 0x64), nullptr, extent, p);                // 00694128..00694131
        std::memcpy(at(lp(self, 0x64), 0x40), map_note_color_007a23f0, 12);                    // 00694136..00694153
    }
    {
        menu_resources::Ref target_a; target_a.construct_00406d80("whitetarget");              // 00694156..0069415F [esp+54]
        menu_resources::Ref target_b; target_b.construct_00406d80("whitetarget");              // 00694164..0069416D [esp+44]
        unsigned char text[0x58];                                                              // [esp+A4] CSWGuiTextParams
        void* text_params = gui_load_text_params_construct_00417050(text, nullptr);           // 00694172..0069417E
        { auto w4_guard_2 = ::w4gui::on_throw([&]() { frame_LogStringDtor_005e5c20(text, nullptr); }); 
            const float color_a[3] = {1.0f, 1.0f, 1.0f}, color_b[3] = {1.0f, 1.0f, 1.0f};     // [esp+10] / [esp+1C] (006941B2..006941DA)
            const I32 extent[4] = {0, 0, 0x14, 0x14};                                          // [esp+2C] (00694194..006941E6)
            menu_resources::Ref none_a, none_b, none_c, none_d;                                // [esp+84] [esp+94] [esp+64] [esp+74]
            unsigned char border_a[0x74], border_b[0x74];                                      // [esp+F8] / [esp+14C]
            c2da_default_ref_00405ed0(&none_a, nullptr);                                       // 006941A3..006941EA
            c2da_default_ref_00405ed0(&none_b, nullptr);                                       // 006941F0..006941F7
            void* first = gui_load_border_params_construct_00415000(border_a, nullptr, &none_b, &none_a, &target_a, 2, 0, 1.0f, color_a, 0); // 006941FD..00694204
            c2da_default_ref_00405ed0(&none_c, nullptr);                                       // 0069421D..00694224
            c2da_default_ref_00405ed0(&none_d, nullptr);                                       // 0069422A..00694231
            void* second = gui_load_border_params_construct_00415000(border_b, nullptr, &none_d, &none_c, &target_b, 2, 0, 1.0f, color_b, 0); // 00694237..0069423E
            vcall<void, const void*, void*, void*, void*>(marker, 0x9C, extent, text_params, second, first);   // 00694243..0069424D Initialize
        w4_guard_2.armed = false; }
        frame_LogStringDtor_005e5c20(text, nullptr);                                           // 00694253..00694262
    }
    std::memcpy(at(self, 0xF8), borrowed_enter_color_007a23c0, 12);                            // 00694267..00694283 (button +90)
    std::memcpy(at(self, 0x16C), borrowed_enter_color_007a23c0, 12);                           // 00694286..006942A3 (button +104)
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_006942d0(void* self, void*) {
    set_vptr(self, vt_map_hider());                                                            // 006942EF
    kotor_fastcall_00692e30(self, nullptr);                                                    // 006942FD
    if (void* dot = lp(self, 0x5C)) { vcall<void*, U32>(dot, 0x04, 1); zero(self, 0x5C); }    // 00694302..00694312
    if (void* image = lp(self, 0x60)) { vcall<void*, U32>(image, 0x00, 1); zero(self, 0x60); } // 00694315..00694322
    if (void* image = lp(self, 0x64)) { vcall<void*, U32>(image, 0x00, 1); zero(self, 0x64); } // 00694325..00694332
    for (I32 i = 0; i < li(self, 0x230); ++i)                                                  // 00694335..0069435D
        if (void* note = lp(lp(self, 0x22C), static_cast<U32>(i) * 4u)) vcall<void*, U32>(note, 0x00, 1);
    map_hider_notes_release(self, nullptr);                                                    // 0069435F..0069437B
    live_free(lp(self, 0x22C)); zero(self, 0x22C);                                             // 0069437E..00694395
    gui_load_button_destruct_00419da0(at(self, 0x68), nullptr);                                // 0069439F
    gui_load_control_destruct_00419a20(self, nullptr);                                         // 006943AE
}
void* __fastcall kotor_fastcall_00695160(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006942d0, flags); }

// ======================= 00694D50 CSWGuiInGameMap (0x10DC, table 00754830) =======================
void* __fastcall kotor_fastcall_00694d50(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 00694D7B
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_ingame_map());                                                           // 00694D8B (00754830)
    for (U32 off : {0x64u, 0x1A4u, 0x2E4u, 0x424u}) label(self, off, u);                       // 00694D91..00694DC1
    for (U32 off : {0x564u, 0x728u, 0x8ECu, 0xAB0u, 0xC74u}) button(self, off, u);             // 00694DD3..00694E13
    void* hider = at(self, 0xE38);
    kotor_fastcall_00693f60(hider, nullptr); u.push(&kotor_fastcall_006942d0, hider);          // 00694E25
    void* image = at(self, 0x1080);
    gui_load_image_construct_00415a30(image, nullptr); u.push(&gui_load_image_destruct_00415ad0, image); // 00694E35
    start_layout(self, "map");                                                                 // 00694E3A..00694E54
    init_control(self, 0x64, "LBL_Map", 0);                                                    // 00694E59
    init_control(self, 0x2E4, "LBL_MapNote");                                                  // 00694E8C
    init_control(self, 0x1A4, "LBL_Area");                                                     // 00694EC2
    init_control(self, 0x424, "LBL_COMPASS");                                                  // 00694EF8
    init_control(self, 0x564, "BTN_RETURN");                                                   // 00694F2E
    init_control(self, 0x728, "BTN_PRTYSLCT");                                                 // 00694F5E
    init_control(self, 0x8EC, "BTN_EXIT");                                                     // 00694F94
    init_control(self, 0xAB0, "BTN_UP");                                                       // 00694FCA
    init_control(self, 0xC74, "BTN_DOWN");                                                     // 00695000
    stop_layout(self);                                                                         // 00695036..00695038
    { const I32 e[4] = {0, 0, 0x200, 0x100}; set_extent(image, e); }                           // 0069503D..00695068
    { const I32 e[4] = {0, 0, 0x1B8, 0x100}; set_extent(hider, e); }                           // 0069506B..0069508E
    gui_m48_panel_add_control_0040b9a0(self, nullptr, hider);                                  // 00695091..00695094
    sw(hider, 0x44, lw(hider, 0x44) & 0xFFFFFFFDu);                                            // 00695099
    sw(self, 0x10D8, lw(self, 0x10D8) | 3u);                                                   // 0069509D
    zero(self, 0x10D4);                                                                        // 006950A4
    struct Ev { U32 off; const void* fn; };
    const Ev events[5] = {{0x564, on_x()}, {0x728, on_accept()}, {0x8EC, on_b()},
                          {0xAB0, fn(&kotor_fastcall_006927b0)}, {0xC74, fn(&kotor_fastcall_006927c0)}};   // 006950AE..0069513D
    for (const Ev& e : events) {
        sw(self, e.off + 0x44, lw(self, e.off + 0x44) & 0xFFFFFFFBu);
        add_event(self, e.off, 0x27, self, e.fn);
    }
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_00695bf0(void* self, void*) {
    gui_load_image_destruct_00415ad0(at(self, 0x1080), nullptr);
    kotor_fastcall_006942d0(at(self, 0xE38), nullptr);
    for (U32 off : {0xC74u, 0xAB0u, 0x8ECu, 0x728u, 0x564u}) gui_load_button_destruct_00419da0(at(self, off), nullptr);
    for (U32 off : {0x424u, 0x2E4u, 0x1A4u, 0x64u}) gui_load_label_destruct_00419d10(at(self, off), nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_00695bd0(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_00695bf0, flags); }

// ======================= 00695180 CSWGuiInGameGalaxyMap (0x2550, table 00754910) =======================
void* __fastcall kotor_fastcall_00695180(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 006951AE
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_galaxy_map());                                                           // 006951D3 (00754910)
    vector_member(at(self, 0x64), 0x1C4, 0x10, &gui_load_button_construct_0041c0f0, &gui_load_button_destruct_00419da0, u); // 006951B3..006951D9
    label(self, 0x1CA4, u); label(self, 0x1DE4, u);                                            // 006951EE..00695201
    void* accept = button(self, 0x1F24, u); void* back = button(self, 0x20E8, u);              // 00695214..00695227
    for (U32 off : {0x22ACu, 0x233Cu}) {                                                       // 00695240..00695285 CSWGui3DSceneView (inlined ctor)
        void* view = at(self, off);
        gui_load_control_construct_0041aa80(view, nullptr);
        set_vptr(view, gui_load_scene_view_methods_00752e30);
        { auto w4_guard_3 = ::w4gui::on_throw([&]() { gui_load_control_destruct_00419a20(view, nullptr); });  gui_scene_construct_004174b0(at(view, 0x5C), nullptr); w4_guard_3.armed = false; }
        u.push(&gui_load_scene_view_destruct_0067ac50, view);
    }
    for (U32 i = 0; i < 0x10; ++i) c2da_default_ref_00405ed0(at(self, 0x23D4 + i * 0x18), nullptr);  // 00695292..006952B3
    U32 icon[2], model[2], tag[2];                                                             // [esp+3C] [esp+44] [esp+50]
    exo_default(icon); u.push(&frame_LogStringDtor_005e5c20, icon);                            // 006952B5..006952B9
    exo_default(model); u.push(&frame_LogStringDtor_005e5c20, model);                          // 006952BE..006952CA
    exo_default(tag); u.push(&frame_LogStringDtor_005e5c20, tag);                              // 006952CF..006952DB
    sw(self, 0x254C, 0xFFFFFFFFu);                                                             // 006952F1
    start_layout(self, "galaxymap");                                                           // 006952E0..00695307
    init_control(self, 0x1DE4, "LBL_DESC");                                                    // 0069530C
    init_control(self, 0x1CA4, "LBL_PLANETNAME");                                              // 00695348
    init_control(self, 0x233C, "3D_PlanetModel");                                              // 0069537E
    init_control(self, 0x22AC, "3D_PlanetDisplay");                                            // 006953BA
    init_control(self, 0x1F24, "BTN_ACCEPT");                                                  // 006953F6
    init_control(self, 0x20E8, "BTN_BACK");                                                    // 00695432
    for (I32 i = 0; i < 0x10; ++i) {                                                           // 0069546E..0069550B
        void* planet = at(self, 0x64 + static_cast<U32>(i) * 0x1C4);
        I32 found;
        { ExoString column("GUITag"); found = c2da_current_get_string_00413270(planets_2da(), nullptr, i, column.storage, tag); } // 00695480..006954CA
        if (found != 0) {                                                                      // 006954CF..006954D5
            gui_load_panel_init_control_0040b930(self, nullptr, planet, tag, 1);               // 006954D7..006954E1
            sw(planet, 0x44, lw(planet, 0x44) & 0xFFFFFFFBu);                                  // 006954E6..006954F6
            gui_load_control_add_event_0041ab20(planet, nullptr, 0x27, self, fn(&kotor_fastcall_006935a0)); // 006954E9..006954F9
            sw(planet, 0x58, static_cast<U32>(i));                                             // 006954FE
        }
    }
    stop_layout(self);                                                                         // 00695511..00695513
    sw(back, 0x44, lw(back, 0x44) & 0xFFFFFFFBu);                                              // 00695518..00695533
    gui_load_control_add_event_0041ab20(back, nullptr, 0x27, self, on_b());                    // 00695524..00695536
    sw(accept, 0x44, lw(accept, 0x44) & 0xFFFFFFFBu);                                          // 0069553B..00695551
    gui_load_control_add_event_0041ab20(accept, nullptr, 0x27, self, on_accept());             // 00695547..00695554
    const float position[3] = {0.0f, 0.0f, 0.0f};                                              // [esp+1C] (006955A3..006955B3)
    const float orientation[4] = {0.0f, 0.0f, 0.0f, 1.0f};                                     // [esp+2C] (00695583..0069559B)
    if (kotor_cell_do3dgui_0078d1e4 != 0) {                                                    // 00695559..0069556C
        void* camera = lp(self, 0x2320);                                                       // 00695560
        vcall<void, const char*, const void*, const void*>(lp(self, 0x231C), 0x70, "gui3D_room", position, orientation); // 00695566..006955BB
        void* planet_model;
        { ExoString name("galaxy"); planet_model = aurora_gui_scene_add_model_name_00417620(at(self, 0x2308), nullptr, name.storage, -1); } // 006955BE..006955F4
        if (planet_model != nullptr) {                                                         // 006955F9..006955FB
            vcall<void, const char*, float, U32, U32>(planet_model, 0x18, "default", 1.0f, 0, 0); // 006955FD..0069560F
            vcall<void, void*, const char*, U32>(camera, 0x74, planet_model, "camerahook", 0);   // 00695612..0069561E
            vcall<void, U32>(camera, 0x44, 0x41B5CED9u);                                       // 00695621..0069562A (22.726f)
        }
    }
    vcall<void, const char*, const void*, const void*>(lp(self, 0x23AC), 0x70, "gui3D_room", position, orientation); // 0069562D..0069567C
    for (I32 i = 0; i < 0x10; ++i) {                                                           // 0069567F..0069583E
        void* planet = at(self, 0x64 + static_cast<U32>(i) * 0x1C4);
        U8* record = static_cast<U8*>(at(self, 0x23D0 + static_cast<U32>(i) * 0x18));
        I32 name_strref = 0, desc_strref = 0;                                                  // [esp+4C] / [esp+58]
        { ExoString column("Icon"); c2da_current_get_string_00413270(planets_2da(), nullptr, i, column.storage, icon); }   // 00695690..006956D6
        { ExoString column("Model"); c2da_current_get_string_00413270(planets_2da(), nullptr, i, column.storage, model); } // 006956DB..00695721
        { ExoString column("Name"); c2da_current_get_int_00413660(planets_2da(), nullptr, i, column.storage, &name_strref); }        // 00695726..0069576B
        { ExoString column("Description"); c2da_current_get_int_00413660(planets_2da(), nullptr, i, column.storage, &desc_strref); } // 00695770..006957B5
        { menu_resources::Ref ref; async_ref_construct_string_00406d60(&ref, nullptr, icon);    // 006957BA..006957C3
          gui_load_border_params_set_fill_00414c00(at(planet, 0x80), nullptr, &ref, 0); }      // 006957C8..006957D5
        { menu_resources::Ref ref; async_ref_construct_string_00406d60(&ref, nullptr, icon);    // 006957DA..006957E3
          gui_load_border_params_set_fill_00414c00(at(planet, 0xF4), nullptr, &ref, 0); }      // 006957E8..006957F5
        vcall<void, U32>(planet, 0x40, 0);                                                     // 006957FA..00695800
        sw(record - 4, 0, static_cast<U32>(name_strref));                             // 00695803..0069580F
        sw(record, 0, static_cast<U32>(desc_strref));                                          // 00695816
        menu_resources::Ref returned;                                                          // [esp+88]
        async_ref_assign_string_004061f0(record + 4, nullptr, &returned, model);               // 00695812..00695824
    }
    sb(self, 0x48, 3);                                                                         // 00695848
    u.dismiss();
    frame_LogStringDtor_005e5c20(tag, nullptr);                                                // 00695844..00695854
    frame_LogStringDtor_005e5c20(model, nullptr);                                              // 00695859..00695865
    frame_LogStringDtor_005e5c20(icon, nullptr);                                               // 0069586A..00695876
    return self;
}
void __fastcall kotor_fastcall_006958c0(void* self, void*) {
    gui_load_scene_view_destruct_0067ac50(at(self, 0x233C), nullptr);
    gui_load_scene_view_destruct_0067ac50(at(self, 0x22AC), nullptr);
    gui_load_button_destruct_00419da0(at(self, 0x20E8), nullptr);
    gui_load_button_destruct_00419da0(at(self, 0x1F24), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0x1DE4), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0x1CA4), nullptr);
    vector_destruct(at(self, 0x64), 0x1C4, 0x10, &gui_load_button_destruct_00419da0);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006958a0(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006958c0, flags); }

// ======================= 006AB2F0 CSWGuiInGameOptions (0x1DAC, table 00755DE0) =======================
void* __fastcall kotor_fastcall_006ab2f0(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 006AB31B
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_ingame_options());                                                       // 006AB33D (00755DE0)
    vector_member(at(self, 0x64), 0x1C4, 8, &gui_load_button_construct_0041c0f0, &gui_load_button_destruct_00419da0, u); // 006AB320..006AB343
    button(self, 0xE84, u);                                                                    // 006AB353
    for (U32 off : {0x1048u, 0x1188u, 0x12C8u, 0x1408u, 0x1548u, 0x1688u, 0x17C8u}) label(self, off, u); // 006AB363..006AB3C5
    listbox(self, 0x1908, u); void* exit = button(self, 0x1BE8, u);                            // 006AB3D5..006AB3E5
    start_layout(self, "optionsingame");                                                       // 006AB3EC..006AB405
    static const char* const names[8] = {"BTN_LOADGAME", "BTN_SAVEGAME", "BTN_GAMEPLAY", "BTN_FEEDBACK",
                                         "BTN_AUTOPAUSE", "BTN_GRAPHICS", "BTN_SOUND", "BTN_QUIT"};
    for (U32 i = 0; i < 8; ++i) init_control(self, 0x64 + i * 0x1C4, names[i]);                // 006AB40A..006AB57A
    init_control(self, 0x1048, "LBL_TITLE");                                                   // 006AB5AF
    init_control(self, 0x1908, "LB_DESC");                                                     // 006AB5E4
    init_control(self, 0x1BE8, "BTN_EXIT");                                                    // 006AB619
    stop_layout(self);                                                                         // 006AB655..006AB657
    void* proto = lp(self, 0x1BD4);                                                            // 006AB65C (listbox +1908 prototype)
    vcall<void, const void*, void*, float>(at(self, 0x1188), 0x98, at(proto, 4), proto, 1.0f); // 006AB662..006AB670
    static void (__fastcall* const handlers[8])(void*, void*, void*) = {
        &kotor_fastcall_006aaaf0, &kotor_fastcall_006aab80, &kotor_fastcall_006aac10, &kotor_fastcall_006aac90,
        &kotor_fastcall_006aad10, &kotor_fastcall_006aad90, &kotor_fastcall_006aae10, &kotor_fastcall_006ab1b0};
    for (U32 i = 0; i < 8; ++i) add_event(self, 0x64 + i * 0x1C4, 0x27, self, fn(handlers[i]));  // 006AB676..006AB706
    for (U32 i = 0; i < 8; ++i) {                                                              // 006AB70B..006AB73D
        add_event(self, 0x64 + i * 0x1C4, 0, self, fn(&kotor_fastcall_006ab220));
        gui_load_border_params_set_pulsing_00414d00(at(self, 0x158 + i * 0x1C4), nullptr, 1, 1, 0);
    }
    sw(exit, 0x44, lw(exit, 0x44) & 0xFFFFFFFBu);                                              // 006AB73F..006AB756
    gui_load_control_add_event_0041ab20(exit, nullptr, 0x27, self, on_b());                    // 006AB74B..006AB759
    vcall<void, void*, U32>(self, 0x08, at(self, 0x64), 0);                                    // 006AB75E..006AB768
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_006ab7b0(void* self, void*) {
    gui_load_button_destruct_00419da0(at(self, 0x1BE8), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x1908), nullptr);
    for (U32 off : {0x17C8u, 0x1688u, 0x1548u, 0x1408u, 0x12C8u, 0x1188u, 0x1048u}) gui_load_label_destruct_00419d10(at(self, off), nullptr);
    gui_load_button_destruct_00419da0(at(self, 0xE84), nullptr);
    vector_destruct(at(self, 0x64), 0x1C4, 8, &gui_load_button_destruct_00419da0);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006ab790(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006ab7b0, flags); }

// ======================= 006BFA40 CSWGuiPartySelection (0x3A80, table 00756D28) =======================
// 006BED70 CSWGuiPartySelectionData (CSWGuiRadioButton + 2 labels, 0x454, table 00756BB8 = copy of 007566D8).
void* __fastcall kotor_fastcall_006bed70(void* self, void*) {
    gui_load_button_construct_0041c0f0(self, nullptr);                                         // 006BED8D
    Unwind u; u.push(&kotor_m1_radio_button_destruct_006be360, self);
    set_vptr(self, vt_party_selection_data());                                                 // 006BED94 (00756BB8)
    zero(self, 0x1C4);                                                                         // 006BED9A
    label(self, 0x1C8, u); label(self, 0x308, u);                                              // 006BEDAA..006BEDBA
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_006be420(void* self, void*) {
    gui_load_label_destruct_00419d10(at(self, 0x308), nullptr);                                // 006BE43D..006BE44B
    gui_load_label_destruct_00419d10(at(self, 0x1C8), nullptr);                                // 006BE450..006BE45B
    kotor_m1_radio_button_destruct_006be360(self, nullptr);                                    // 006BE46A..006BE470 (inlined)
}
// 006bf3b0 CSWGuiPartySelection::OnDone: body in claude_gui_char_*.cpp (declared in claude_handlers.hpp)
void* __fastcall kotor_fastcall_006bfa40(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 006BFA6B
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_party_selection());                                                      // 006BFA79 (00756D28)
    exo_default(at(self, 0x74)); u.push(&frame_LogStringDtor_005e5c20, at(self, 0x74));        // 006BFA7F
    vector_member(at(self, 0x7C), 0x454, 9, &kotor_fastcall_006bed70, &kotor_fastcall_006be420, u); // 006BFA84..006BFA9E
    label(self, 0x2770, u); button(self, 0x28B0, u);                                           // 006BFAAE..006BFABE
    label(self, 0x2A74, u); label(self, 0x2BB4, u); label(self, 0x2CF4, u);                    // 006BFACE..006BFAEE
    vector_member(at(self, 0x2E34), 0x140, 7, &gui_load_label_construct_0041acd0, &gui_load_label_destruct_00419d10, u); // 006BFAF3..006BFB10
    button(self, 0x36F4, u); button(self, 0x38B8, u);                                          // 006BFB20..006BFB30
    U32 name[2]; exo_default(name); u.push(&frame_LogStringDtor_005e5c20, name);               // 006BFB35..006BFB3E [esp+10]
    zero(self, 0x6C);                                                                          // 006BFB51
    start_layout(self, "partyselection");                                                      // 006BFB43..006BFB60
    for (U32 i = 0; i < 9; ++i) {                                                              // 006BFB65..006BFC66
        void* npc = at(self, 0x7C + i * 0x454);
        frame_LogFormat_005e5680(name, "LBL_NA%d", i);
        gui_load_panel_init_control_0040b930(self, nullptr, at(npc, 0x1C8), name, 1);
        frame_LogFormat_005e5680(name, "LBL_CHAR%d", i);
        gui_load_panel_init_control_0040b930(self, nullptr, at(npc, 0x308), name, 1);
        frame_LogFormat_005e5680(name, "BTN_NPC%d", i);
        gui_load_panel_init_control_0040b930(self, nullptr, npc, name, 1);
        gui_load_control_add_event_0041ab20(npc, nullptr, 0x27, self, fn(&kotor_fastcall_006bf2a0)); // 006BFBD7..006BFBE1
        gui_load_control_add_event_0041ab20(npc, nullptr, 0x2D, self, fn(&kotor_fastcall_006bf2a0)); // 006BFBE6..006BFBF0
        gui_load_control_add_event_0041ab20(npc, nullptr, 0, self, fn(&kotor_fastcall_006bf5b0));    // 006BFBF5..006BFBFF
        gui_load_control_add_event_0041ab20(npc, nullptr, 1, self, fn(&gui_load_slot_ret4_0060e760)); // 006BFC04..006BFC0E
        sw(npc, 0x58, ad(npc));                                                                // 006BFC13
        std::memcpy(at(npc, 0x104), borrowed_enter_color_007a23c0, 12);                        // 006BFC16..006BFC37
        gui_load_border_params_set_pulsing_00414d00(at(npc, 0xF4), nullptr, 1, 1, 0);          // 006BFC33..006BFC42
        sw(npc, 0x450, i);                                                                     // 006BFC50
        sw(npc, 0x448, lw(npc, 0x448) & 0xFFFFFFFBu);                                          // 006BFC47..006BFC56
    }
    init_control(self, 0x2770, "LBL_COUNT");                                                   // 006BFC6C
    init_control(self, 0x2A74, "LBL_NPC_NAME");                                                // 006BFCA2
    init_control(self, 0x2BB4, "LBL_NPC_LEVEL");                                               // 006BFCD8
    init_control(self, 0x2CF4, "LBL_3D");                                                      // 006BFD0E
    static const char* const labels[7] = {"LBL_TITLE", "LBL_AVAILABLE", "LBL_BEVEL_L", "LBL_BEVEL_R", "LBL_BEVEL_M"};
    for (U32 i = 0; i < 5; ++i) init_control(self, (i == 0 ? 0x2E34u : 0x31F4u + (i - 1) * 0x140u), labels[i]);  // 006BFD44..006BFE1C
    init_control(self, 0x28B0, "BTN_DONE");                                                    // 006BFE52
    init_control(self, 0x36F4, "BTN_BACK");                                                    // 006BFE88
    init_control(self, 0x38B8, "BTN_ACCEPT");                                                  // 006BFEBE
    stop_layout(self);                                                                         // 006BFEF4..006BFEF6
    void* done = at(self, 0x28B0); void* back = at(self, 0x36F4); void* accept = at(self, 0x38B8);
    sw(self, 0x34EC, 0x43340000u);                                                             // 006BFF05 (180.0f)
    gui_load_control_add_event_0041ab20(done, nullptr, 0x27, self, fn(&kotor_fastcall_006bf3b0)); // 006BFEFB..006BFF0F
    gui_load_control_add_event_0041ab20(done, nullptr, 0x2D, self, fn(&kotor_fastcall_006bf3b0)); // 006BFF14..006BFF1E
    sw(back, 0x44, lw(back, 0x44) & 0xFFFFFFFBu);                                              // 006BFF23..006BFF2F
    gui_load_control_add_event_0041ab20(back, nullptr, 0x27, self, on_b());                    // 006BFF26..006BFF36
    sw(accept, 0x44, lw(accept, 0x44) & 0xFFFFFFFBu);                                          // 006BFF3B..006BFF56
    gui_load_control_add_event_0041ab20(accept, nullptr, 0x27, self, fn(&kotor_fastcall_006bf2a0)); // 006BFF47..006BFF59
    zero(self, 0x3A7C);                                                                        // 006BFF60
    sw(done, 0x44, lw(done, 0x44) & 0xFFFFFFFBu);                                              // 006BFF66
    zero(self, 0x70);                                                                          // 006BFF6D
    u.dismiss();
    frame_LogStringDtor_005e5c20(name, nullptr);                                               // 006BFF69..006BFF75
    return self;
}
void __fastcall kotor_fastcall_006bffb0(void* self, void*) {
    gui_load_button_destruct_00419da0(at(self, 0x38B8), nullptr);
    gui_load_button_destruct_00419da0(at(self, 0x36F4), nullptr);
    vector_destruct(at(self, 0x2E34), 0x140, 7, &gui_load_label_destruct_00419d10);
    for (U32 off : {0x2CF4u, 0x2BB4u, 0x2A74u}) gui_load_label_destruct_00419d10(at(self, off), nullptr);
    gui_load_button_destruct_00419da0(at(self, 0x28B0), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0x2770), nullptr);
    vector_destruct(at(self, 0x7C), 0x454, 9, &kotor_fastcall_006be420);
    frame_LogStringDtor_005e5c20(at(self, 0x74), nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006bff90(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006bffb0, flags); }

// ======================= 006C1C00 CSWGuiStore (0x2280, table 00756E38) =======================
// 006c1b00 CSWGuiStore::OnControlXButton: body in claude_gui_char_*.cpp (declared in claude_handlers.hpp)
// 006C1330 CSWGuiStore::DeleteItemEntries() RET 0.
void __fastcall kotor_fastcall_006c1330(void* self, void*) {
    gui_load_listbox_clear_items_00419a90(at(self, 0x1760), nullptr);                          // 006C1334..006C133A
    gui_load_listbox_clear_items_00419a90(at(self, 0x1480), nullptr);                          // 006C133F..006C1345
    for (I32 i = 0; i < li(self, 0x6C); ++i) {                                                 // 006C134A..006C1373
        if (void* entry = lp(lp(self, 0x68), static_cast<U32>(i) * 4u)) vcall<void*, U32>(entry, 0x00, 1);
        sw(lp(self, 0x68), static_cast<U32>(i) * 4u, 0);
    }
    zero(self, 0x6C);                                                                          // 006C1375
    for (I32 i = 0; i < li(self, 0x78); ++i) {                                                 // 006C137C..006C13A5
        if (void* entry = lp(lp(self, 0x74), static_cast<U32>(i) * 4u)) vcall<void*, U32>(entry, 0x00, 1);
        sw(lp(self, 0x74), static_cast<U32>(i) * 4u, 0);
    }
    zero(self, 0x78);                                                                          // 006C13A8
}
void* __fastcall kotor_fastcall_006c1c00(void* self, void*, void* manager) {
    static const U32 labels[16] = {0x80, 0x1C0, 0x300, 0x440, 0x580, 0x6C0, 0x800, 0x940, 0xA80,
                                   0xBC0, 0xD00, 0xE40, 0xF80, 0x10C0, 0x1200, 0x1340};
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 006C1C2B
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_store());                                                                // 006C1C32 (00756E38)
    for (U32 off : {0x6Cu, 0x70u, 0x68u, 0x78u, 0x7Cu, 0x74u}) zero(self, off);                // 006C1C3C..006C1C4B
    u.push(&store_lists_release, self);
    for (U32 i = 0; i < 16; ++i) label(self, labels[i], u);                                    // 006C1C59..006C1D49
    void* shop = listbox(self, 0x1480, u); void* inv = listbox(self, 0x1760, u); listbox(self, 0x1A40, u); // 006C1D5B..006C1D7D
    void* cancel = button(self, 0x1D20, u); void* examine = button(self, 0x1EE4, u); button(self, 0x20A8, u); // 006C1D8D..006C1DAD
    zero(self, 0x226C);                                                                        // 006C1DC0
    start_layout(self, "store");                                                               // 006C1DB2..006C1DD6
    init_control(self, 0x1340, "LBL_BUYSELL");                                                 // 006C1DDB
    init_control(self, 0x1480, "LB_SHOPITEMS");                                                // 006C1E11
    init_control(self, 0x1760, "LB_INVITEMS");                                                 // 006C1E41
    init_control(self, 0x10C0, "LBL_CREDITS");                                                 // 006C1E71
    init_control(self, 0x1200, "LBL_CREDITS_VALUE");                                           // 006C1EA7
    init_control(self, 0x1A40, "LB_DESCRIPTION");                                              // 006C1EDD
    init_control(self, 0xA80, "LBL_COST");                                                     // 006C1F13
    init_control(self, 0xBC0, "LBL_COST_VALUE");                                               // 006C1F50
    init_control(self, 0xD00, "LBL_STOCK");                                                    // 006C1F86
    init_control(self, 0xE40, "LBL_STOCK_VALUE");                                              // 006C1FBC
    init_control(self, 0x1D20, "BTN_Cancel");                                                  // 006C1FF2
    init_control(self, 0x1EE4, "BTN_Examine");                                                 // 006C2028
    init_control(self, 0x20A8, "BTN_Accept");                                                  // 006C205E
    stop_layout(self);                                                                         // 006C2094..006C2096
    void* proto = lp(self, 0x1D0C);                                                            // 006C209B (listbox +1A40 prototype)
    vcall<void, const void*, void*, float>(at(self, 0x940), 0x98, at(proto, 4), proto, 1.0f);  // 006C20A1..006C20B7
    sw(shop, 0x2BC, lw(shop, 0x2BC) | 0x20u);                                                  // 006C20BD..006C20C2
    sw(inv, 0x2BC, lw(inv, 0x2BC) | 0x20u);                                                    // 006C20C8..006C20D2
    gui_load_panel_set_active_control_0040a630(self, nullptr, shop, 0);                        // 006C20D0..006C20DB
    sw(inv, 0x44, lw(inv, 0x44) & 0xFFFFFFFDu);                                                // 006C20E0
    zero(self, 0x64); zero(self, 0x2274); zero(self, 0x2270);                                  // 006C20EC..006C20F5
    sw(cancel, 0x44, lw(cancel, 0x44) & 0xFFFFFFFBu);                                          // 006C20FB..006C2109
    gui_load_control_add_event_0041ab20(cancel, nullptr, 0x27, self, on_b());                  // 006C20FE..006C210C
    sw(examine, 0x44, lw(examine, 0x44) & 0xFFFFFFFBu);                                        // 006C2111..006C2128
    gui_load_control_add_event_0041ab20(examine, nullptr, 0x27, self, fn(&kotor_fastcall_006c1b00)); // 006C211D..006C212B
    sw(self, 0x20EC, lw(self, 0x20EC) & 0xFFFFFFFBu);                                          // 006C2130 (BTN_Accept)
    vcall<void, U32>(inv, 0xA4, 1);                                                            // 006C2137..006C213D
    vcall<void, U32>(shop, 0xA4, 1);                                                           // 006C2143..006C2149
    sb(self, 0x48, 1);                                                                         // 006C2154
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_006c1470(void* self, void*) {
    set_vptr(self, vt_store());                                                                // 006C148D
    kotor_fastcall_006c1330(self, nullptr);                                                    // 006C149B
    for (U32 off : {0x20A8u, 0x1EE4u, 0x1D20u}) gui_load_button_destruct_00419da0(at(self, off), nullptr);
    for (U32 off : {0x1A40u, 0x1760u, 0x1480u}) gui_load_listbox_destruct_0041b060(at(self, off), nullptr);
    for (U32 off : {0x1340u, 0x1200u, 0x10C0u, 0xF80u, 0xE40u, 0xD00u, 0xBC0u, 0xA80u, 0x940u, 0x800u, 0x6C0u, 0x580u,
                    0x440u, 0x300u, 0x1C0u, 0x80u}) gui_load_label_destruct_00419d10(at(self, off), nullptr);
    store_lists_release(self, nullptr);                                                        // 006C1600..006C1628
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006c1820(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006c1470, flags); }

// ======================= 006ADDA0 CSWGuiInGameAbilities (0x3F98, table 00755E50) =======================
void* __fastcall kotor_fastcall_006adda0(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 006ADDCB
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_abilities());                                                            // 006ADDD2 (00755E50)
    zero(self, 0x68); zero(self, 0x6C); zero(self, 0x64);                                      // 006ADDDC..006ADDE2
    u.push(&abilities_list_release, self);
    vector_member(at(self, 0x70), 0x310, 10, &gui_m48_skill_entry_construct_006acc50, &gui_m48_skill_entry_destruct_006aba80, u); // 006ADDE5..006ADDFF
    for (U32 i = 0; i < 10; ++i) label(self, 0x1F10 + i * 0x140, u);                           // 006ADE0F..006ADE9F
    for (U32 off : {0x2B90u, 0x2D54u, 0x2F18u}) button(self, off, u);                          // 006ADEAF..006ADECF
    void* ability = listbox(self, 0x30DC, u); listbox(self, 0x33BC, u);                        // 006ADEE1..006ADEF1
    for (U32 off : {0x369Cu, 0x3860u, 0x3A24u, 0x3BE8u, 0x3DACu}) button(self, off, u);        // 006ADF01..006ADF41
    for (U32 off : {0x3F78u, 0x3F88u}) {                                                       // 006ADF46..006ADF61
        kotor_m15d_chart_construct_006cd510(at(self, off), nullptr); u.push(&kotor_m15d_chart_destruct_006cd530, at(self, off));
    }
    U32 scratch[2]; exo_default(scratch); u.push(&frame_LogStringDtor_005e5c20, scratch);      // 006ADF66..006ADF6F [esp+30]
    sb(self, 0x3F71, 0xFF);                                                                    // 006ADF83
    start_layout(self, "abilities");                                                           // 006ADF76..006ADF96
    init_control(self, 0x1F10, "LBL_PORTRAIT");                                                // 006ADF9B
    init_control(self, 0x33BC, "LB_DESC");                                                     // 006ADFD0
    init_control(self, 0x30DC, "LB_ABILITY");                                                  // 006AE005
    init_control(self, 0x2410, "LBL_BONUS");                                                   // 006AE03B
    init_control(self, 0x2550, "LBL_BONUSVAL");                                                // 006AE070
    init_control(self, 0x2190, "LBL_SKILLRANK");                                               // 006AE0A5
    init_control(self, 0x22D0, "LBL_RANKVAL");                                                 // 006AE0DA
    init_control(self, 0x2690, "LBL_TOTAL");                                                   // 006AE10F
    init_control(self, 0x27D0, "LBL_TOTALVAL");                                                // 006AE144
    init_control(self, 0x2910, "LBL_NAME");                                                    // 006AE179
    init_control(self, 0x2A50, "LBL_INFOBG");                                                  // 006AE1AE
    init_control(self, 0x2B90, "BTN_FEATS");                                                   // 006AE1E3
    init_control(self, 0x2D54, "BTN_POWERS");                                                  // 006AE218
    init_control(self, 0x2F18, "BTN_SKILLS");                                                  // 006AE24D
    init_control(self, 0x369C, "BTN_EXIT");                                                    // 006AE282
    init_control(self, 0x3860, "BTN_CHANGE1");                                                 // 006AE2B7
    init_control(self, 0x3A24, "BTN_CHANGE2");                                                 // 006AE2EC
    init_control(self, 0x3BE8, "BTN_CHARLEFT");                                                // 006AE321
    init_control(self, 0x3DAC, "BTN_CHARRIGHT");                                               // 006AE356
    stop_layout(self);                                                                         // 006AE38B..006AE38D
    sw(self, 0x3C2C, lw(self, 0x3C2C) & 0xFFFFFFFBu);                                          // 006AE392..006AE3AD
    add_event(self, 0x3BE8, 0x27, self, fn(&kotor_fastcall_006ac3e0));                         // 006AE39E..006AE3B0
    sw(self, 0x3DF0, lw(self, 0x3DF0) & 0xFFFFFFFBu);                                          // 006AE3B5..006AE3CB
    add_event(self, 0x3DAC, 0x27, self, fn(&kotor_fastcall_006ac660));                         // 006AE3C1..006AE3CE
    void* proto = lp(self, 0x33A8);                                                            // 006AE3D3 (listbox +30DC prototype)
    sw(self, 0x3E34, 0x43340000u); sw(self, 0x3EA8, 0x43340000u);                              // 006AE3D9..006AE3E4 (180.0f)
    std::memcpy(at(proto, 0x164), borrowed_enter_color_007a23c0, 12);                          // 006AE3EA..006AE40B
    gui_load_border_params_set_pulsing_00414d00(at(proto, 0x154), nullptr, 1, 1, 0);           // 006AE407..006AE416
    const U32 width = lw(ability, 0x294) - 2u * lb(ability, 0x2C0);                            // 006AE41B..006AE430
    for (U32 i = 0; i < 10; ++i) {                                                             // 006AE450..006AE498
        void* entry = at(self, 0x70 + i * 0x310);
        vcall<void, void*, void*, void*, U32>(entry, 0xA8, at(proto, 0xD0), at(proto, 0x5C), at(proto, 0x140), width); // Initialize
        gui_load_control_add_event_0041ab20(entry, nullptr, 0, self, fn(&kotor_fastcall_006ad180));
    }
    void* desc_proto = lp(self, 0x3688);                                                       // 006AE49A (listbox +33BC prototype)
    vcall<void, const void*, void*, float>(at(self, 0x2050), 0x98, at(desc_proto, 4), desc_proto, 1.0f); // 006AE4A0..006AE4B6
    gui_load_control_add_event_0041ab20(ability, nullptr, 0x1F8, self, fn(&kotor_fastcall_006ad4b0)); // 006AE4BC..006AE4C9
    vcall<void, U32>(ability, 0xA4, 1);                                                        // 006AE4CE..006AE4D4
    add_event(self, 0x2B90, 0x27, self, fn(&kotor_fastcall_006ada70));                         // 006AE4DA..006AE4EA
    add_event(self, 0x2B90, 1, self, fn(&kotor_fastcall_006ac8d0));                            // 006AE4EF..006AE4F9
    add_event(self, 0x2D54, 0x27, self, fn(&kotor_fastcall_006adaa0));                         // 006AE4FE..006AE50E
    add_event(self, 0x2D54, 1, self, fn(&kotor_fastcall_006ac8d0));                            // 006AE513..006AE51D
    add_event(self, 0x2F18, 0x27, self, fn(&kotor_fastcall_006adad0));                         // 006AE522..006AE532
    add_event(self, 0x2F18, 1, self, fn(&kotor_fastcall_006ac8d0));                            // 006AE537..006AE541
    add_event(self, 0x369C, 0x27, self, on_b());                                               // 006AE546..006AE554
    add_event(self, 0x3860, 0x27, self, fn(&kotor_fastcall_006adc80));                         // 006AE559..006AE569
    add_event(self, 0x3A24, 0x27, self, fn(&kotor_fastcall_006adc80));                         // 006AE56E..006AE57C
    for (U32 off : {0x2BD4u, 0x2D98u, 0x2F5Cu, 0x36E0u, 0x38A4u, 0x3A68u})                    // 006AE581..006AE5A3
        sw(self, off, lw(self, off) & 0xFFFFFFFBu);
    sw(self, 0x3884, 0x9725); sw(self, 0x3890, 0xCE);                                          // 006AE5A9..006AE5B1 (BTN_CHANGE1 +24/+30)
    sw(self, 0x3A48, 0x9725);                                                                  // 006AE5BC
    zero(self, 0x3F74);                                                                        // 006AE5C2
    u.dismiss();
    frame_LogStringDtor_005e5c20(scratch, nullptr);                                            // 006AE5B8..006AE5D1
    return self;
}
void __fastcall kotor_fastcall_006abaf0(void* self, void*) {
    set_vptr(self, vt_abilities());                                                            // 006ABB0F
    gui_load_listbox_clear_items_00419a90(at(self, 0x33BC), nullptr);                          // 006ABB25
    gui_load_listbox_clear_items_00419a90(at(self, 0x30DC), nullptr);                          // 006ABB32
    kotor_m15d_chart_destruct_006cd530(at(self, 0x3F88), nullptr);
    kotor_m15d_chart_destruct_006cd530(at(self, 0x3F78), nullptr);
    for (U32 off : {0x3DACu, 0x3BE8u, 0x3A24u, 0x3860u, 0x369Cu}) gui_load_button_destruct_00419da0(at(self, off), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x33BC), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x30DC), nullptr);
    for (U32 off : {0x2F18u, 0x2D54u, 0x2B90u}) gui_load_button_destruct_00419da0(at(self, off), nullptr);
    for (U32 i = 10; i-- > 0;) gui_load_label_destruct_00419d10(at(self, 0x1F10 + i * 0x140), nullptr);
    vector_destruct(at(self, 0x70), 0x310, 10, &gui_m48_skill_entry_destruct_006aba80);
    abilities_list_release(self, nullptr);                                                     // 006ABCA9..006ABCBD
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006acc30(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006abaf0, flags); }

// ======================= 006B34C0 CSWGuiInGameInventory (0x1DE8, table 007564E0) =======================
void* __fastcall kotor_fastcall_006b34c0(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 006B34EB
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_inventory());                                                            // 006B34F9 (007564E0)
    for (U32 off : {0x64u, 0x1A4u, 0x2E4u, 0x424u}) label(self, off, u);                       // 006B34FF..006B352F
    void* items = listbox(self, 0x564, u); listbox(self, 0x844, u);                            // 006B353F..006B354F
    for (U32 off : {0xB24u, 0xC64u, 0xDA4u, 0xEE4u, 0x1024u}) label(self, off, u);             // 006B355F..006B359F
    for (U32 off : {0x1164u, 0x1328u, 0x14ECu, 0x16B0u, 0x1874u, 0x1A38u, 0x1BFCu}) button(self, off, u); // 006B35AF..006B3611
    for (U32 off : {0x1DC4u, 0x1DC8u, 0x1DC0u, 0x1DD0u, 0x1DD4u, 0x1DCCu}) zero(self, off);    // 006B3616..006B3634
    u.push(&inventory_lists_release, self);
    sw(self, 0x1DE4, lw(self, 0x1DE4) & 0xFFFFFFFCu);                                          // 006B363A..006B3655
    sw(self, 0x1DD8, 0x7F000000u);                                                             // 006B365B
    sb(self, 0x1DE0, 0xFF);                                                                    // 006B3665
    start_layout(self, "inventory");                                                           // 006B3645..006B3678
    init_control(self, 0x844, "LB_DESCRIPTION");                                               // 006B367D
    init_control(self, 0x564, "LB_ITEMS");                                                     // 006B36B5
    init_control(self, 0x1A4, "LBL_INV");                                                      // 006B36ED
    init_control(self, 0x2E4, "LBL_CREDITS");                                                  // 006B3725
    init_control(self, 0x424, "LBL_CREDITS_VALUE");                                            // 006B375D
    init_control(self, 0xEE4, "LBL_VIT");                                                      // 006B3795
    init_control(self, 0x1024, "LBL_DEF");                                                     // 006B37CD
    init_control(self, 0xC64, "LBL_BGSTATS");                                                  // 006B3805
    init_control(self, 0xB24, "LBL_BGPORT");                                                   // 006B383D
    init_control(self, 0xDA4, "LBL_PORT");                                                     // 006B3875
    init_control(self, 0x1164, "BTN_EXIT");                                                    // 006B38AD
    init_control(self, 0x1328, "BTN_USEITEM");                                                 // 006B38E5
    init_control(self, 0x14EC, "BTN_QUESTITEMS");                                              // 006B391D
    init_control(self, 0x16B0, "BTN_CHANGE1");                                                 // 006B3955
    init_control(self, 0x1874, "BTN_CHANGE2");                                                 // 006B3987
    stop_layout(self);                                                                         // 006B39BF..006B39C1
    sw(self, 0x1A7C, lw(self, 0x1A7C) & 0xFFFFFFFBu);                                          // 006B39C6..006B39DD
    add_event(self, 0x1A38, 0x27, self, fn(&kotor_fastcall_006b2fd0));                         // 006B39D2..006B39E0
    sw(self, 0x1C40, lw(self, 0x1C40) & 0xFFFFFFFBu);                                          // 006B39E5..006B39FC
    add_event(self, 0x1BFC, 0x27, self, fn(&kotor_fastcall_006b3250));                         // 006B39F1..006B39FF
    sw(self, 0x1C84, 0x43340000u); sw(self, 0x1CF8, 0x43340000u);                              // 006B3A07..006B3A12 (180.0f)
    void* proto = lp(self, 0xB10);                                                             // 006B3A18 (listbox +844 prototype)
    vcall<void, const void*, void*, float>(at(self, 0x64), 0x98, at(proto, 4), proto, 1.0f);   // 006B3A04..006B3A2B
    sw(items, 0x2BC, lw(items, 0x2BC) | 0x20u);                                                // 006B3A31..006B3A42
    gui_load_panel_set_active_control_0040a630(self, nullptr, items, 0);                       // 006B3A40..006B3A4B
    // Quest-items button: tlk(0xA577) + " " + tlk(filter strref[(filter + 1) % 6]).
    void* gui = pt(frame_R4_005ed690(client_app(), nullptr));                                 // 006B3A50..006B3A58
    U32 next = static_cast<U32>(lb(gui, 0xBC1)) + 1u;                                          // 006B3A5D..006B3A6E
    if (static_cast<I32>(next) >= 6) next = 0;
    U32 text[2];                                                                               // [esp+18]
    tlk_join(text, 0xA577, " ", inventory_filter_strref_00756444[next], false);               // 006B3A76..006B3B2E
    gui_load_text_params_set_text_00415e00(at(self, 0x1658), nullptr, text);                   // 006B3B33..006B3B3E (BTN_QUESTITEMS)
    gui = pt(frame_R4_005ed690(client_app(), nullptr));                                        // 006B3B51..006B3B5E
    tlk_join(text, 0x7DAB, " - ", inventory_filter_strref_00756444[lb(gui, 0xBC1)], true);     // 006B3B43..006B3C28
    gui_load_text_params_set_text_00415e00(at(self, 0x28C), nullptr, text);                    // 006B3C2D..006B3C38 (LBL_INV)
    struct Ev { U32 off; const void* fn; };
    const Ev events[5] = {{0x1164, on_b()}, {0x1328, on_accept()}, {0x14EC, on_x()},
                          {0x16B0, fn(&kotor_fastcall_006b2ec0)}, {0x1874, fn(&kotor_fastcall_006b2ec0)}};   // 006B3C3D..006B3CCC
    for (const Ev& e : events) {
        sw(self, e.off + 0x44, lw(self, e.off + 0x44) & 0xFFFFFFFBu);
        add_event(self, e.off, 0x27, self, e.fn);
    }
    sw(self, 0x16D4, 0x9725); sw(self, 0x16E0, 0xCE);                                          // 006B3CD1..006B3CD9 (BTN_CHANGE1 +24/+30)
    sw(self, 0x1898, 0x9725);                                                                  // 006B3CE4
    u.dismiss();
    frame_LogStringDtor_005e5c20(text, nullptr);                                               // 006B3CE0..006B3CEE
    return self;
}
void __fastcall kotor_fastcall_006b40c0(void* self, void*) {
    set_vptr(self, vt_inventory());                                                            // 006B40E2
    gui_load_listbox_clear_items_00419a90(at(self, 0x844), nullptr);                           // 006B40F8
    gui_load_listbox_clear_items_00419a90(at(self, 0x564), nullptr);                           // 006B4103
    inventory_lists_release(self, nullptr);                                                    // 006B4108..006B417E
    for (U32 off : {0x1BFCu, 0x1A38u, 0x1874u, 0x16B0u, 0x14ECu, 0x1328u, 0x1164u}) gui_load_button_destruct_00419da0(at(self, off), nullptr);
    for (U32 off : {0x1024u, 0xEE4u, 0xDA4u, 0xC64u, 0xB24u}) gui_load_label_destruct_00419d10(at(self, off), nullptr);  // inlined ~CSWGuiLabel
    gui_load_listbox_destruct_0041b060(at(self, 0x844), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x564), nullptr);
    for (U32 off : {0x424u, 0x2E4u, 0x1A4u, 0x64u}) gui_load_label_destruct_00419d10(at(self, off), nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006b4b90(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006b40c0, flags); }
}  // extern "C"
