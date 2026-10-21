// Lane W4 GUI successor (worker "upgrade", fork B1): CSWGuiUpgrade (0x2FB8, method table 00757298), part 1.
//   006C6B60 CSWGuiUpgrade::CSWGuiUpgrade(CSWGuiManager*)        thiscall RET 4 (returns this)
//   006C4AD0 ~CSWGuiUpgrade                                      thiscall RET
//   006C6100 scalar deleting destructor (flags)                  thiscall RET 4   (slot +00)
//   006C25F0 OnPanelRemoved                                      thiscall RET     (slot +4C)
//   006C2F80 ShowItems(int show)                                 thiscall RET 4
//   006C32E0 SetDescription(const CExoString&)                   thiscall RET 4
//   006C33A0 Draw(float)                                         thiscall RET 4   (slot +34)
//   006C3460 GetArmorModelAndTextureResRef(row, model variation, texture variation, CExoString* model,
//            CExoString* texture)                                thiscall RET 14h
//   006C3630 Update3DView                                        thiscall RET
//   006C3AA0 UpdateItems                                         thiscall RET
//   006C4D70 OnPanelAdded                                        thiscall RET     (slot +48)
// Slot +3C HandleInputEvent 006C6A80 and the event handlers (006C3C30, 006C5510, 006C6190, 006C6500, ...) are in
// claude_upgrade_panel2.cpp. Transcribed from the original swkotor.exe linear sweep; each statement carries its PCs.
//
// Layout (constructor): +64 4 x BTN_UPGRADE4n (0x1C4), +774 3 x BTN_UPGRADE3n, +CC0 4 x LBL_UPGRADE4n (0x140),
// +11C0 3 x LBL_UPGRADE3n, +1580 LB_ITEMS, +1860 LB_DESC, +1B40 LB_DESC_LS, +1E20 LBL_TITLE, +1F60 description
// label, +20A0 label, +21E0 LBL_DESCBG, +2320 LBL_DESCBG_LS, +2460 LBL_PROPERTY, +25A0 LBL_SLOTNAME,
// +26E0 LBL_LSSLOTNAME, +2820 LBL_UPGRADES, +2960 LBL_UPGRADE_COUNT, +2AA0 BTN_ASSEMBLE, +2C64 3D_MODEL /
// +2CF4 3D_MODEL_LS (inline CSWGui3DSceneView: control 5Ch + CSWGuiScene; room = view+70h, camera = view+74h),
// +2D84 BTN_BACK, +2F48 bit0 items list open, +2F4C..: see claude_upgrade.hpp.
//
// Quirks kept: GetArmorModelAndTextureResRef patches the model variation letter into BOTH column names ("ModelA" /
// "TEXA") and formats the texture name in place from its own CStr(); Update3DView passes the item+264h variation
// byte through `movzx dx, al`, so the upper half of the pushed dword is the upper half of the item pointer; Draw hands
// slot +60h of the model the address of an uninitialised 16-byte frame slot; UpdateItems looks up the base item of
// each upgrade item and discards it.
#include "52a797cc9c-claude_upgrade.hpp"

namespace kotor_reconstruction { struct StartupString; }
namespace directory_list {
kotor_reconstruction::StartupString* __fastcall LowerCase005e6080(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out);                                                  // CExoString::LowerCase
}
namespace resource_ref_assignment {                                                               // CResRef::operator=(const CResRef&)
menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*, void*, menu_resources::Ref* out, const menu_resources::Ref* source);
}
namespace menu_resources {
struct Manager { unsigned __thiscall exists_00408bc0(Ref* ref, unsigned type, unsigned* out); };  // CExoResMan::Exists
}
namespace kotor_resource_demand { extern menu_resources::Manager* volatile resource_manager_007a39e8; }  // [007A39E8]

extern "C" {
// ---- panel / controls / scene
void* __fastcall gui_load_control_construct_0041aa80(void* self, void*) noexcept(false);        // CSWGuiControl()
void __fastcall gui_load_control_destruct_00419a20(void* self, void*);                         // ~CSWGuiControl
extern const unsigned char gui_load_scene_view_methods_00752e30[];                             // CSWGui3DSceneView table
void* __fastcall gui_scene_construct_004174b0(void* scene, void*) noexcept(false);             // CSWGuiScene()
void __fastcall gui_load_scene_view_destruct_0067ac50(void* view, void*);                      // ~CSWGui3DSceneView
void* __fastcall aurora_gui_scene_add_model_name_00417620(void* scene, void*, void* name, w4::I32 index); // CSWGuiScene::AddModel
void* __fastcall kotor_m15_gui_scene_get_model_00416750(void* scene, void*, w4::I32 index);    // CSWGuiScene::GetModel RET 4
w4::U32 __fastcall kotor_fastcall_00416710(void* scene, void*, w4::I32 index, w4::U32 destroy); // CSWGuiScene::RemoveModel RET 8
void __fastcall gui_load_nav_set_move_to_00417660(void* nav, void*, w4::U32 direction, void* control); // SetMoveToControl RET 8
void __fastcall gui_m48_panel_set_to_destroy_00624a00(void* panel, void*, w4::U32 flag);       // CSWGuiPanel::setToDestroy RET 4
w4::I32 __fastcall gui_load_text_font_height_00414ee0(void* text, void*);                       // CSWGuiText::GetFontHeight
void __fastcall gui_m48_listbox_add_controls_0041c2c0(void* listbox, void*, void** controls, w4::U32 count,
                                                     w4::U32, w4::U32, w4::U32);               // CSWGuiListBox::AddControls
void __fastcall gui_load_listbox_set_active_control_0041c160(void* listbox, void*, void* control, w4::U32); // SetActiveControl
// ---- strings / resources / 2DA
w4::I32 __fastcall async_string_length_005e5790(void* string, void*);                          // CExoString::GetLength
unsigned __fastcall frame_LogIsEmpty_005e58a0(void* string, void*);                            // CExoString::IsEmpty
w4::I32 __fastcall exostring_compare_no_case_005e6450(void* string, void*, const void* other);  // CExoString::CompareNoCase
void* __fastcall c2da_default_ref_00405ed0(void* self, void*);                                 // CResRef::CResRef()
void* __fastcall c2da_ref_to_string_00405f70(void* ref, void*, void* out);                     // CResRef::CopyToString
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, void* string);          // CResRef(const CExoString&)
menu_resources::Ref* __fastcall unresolved_ref_assign_00406290(menu_resources::Ref*, void*, menu_resources::Ref* out, const char* text); // operator=(const char*)
void* __fastcall c2da_current_construct_00413cc0(void* self, void*, menu_resources::Ref name, w4::I32 mode) noexcept(false); // C2DA::C2DA
w4::I32 __fastcall c2da_current_load_004143b0(void* table, void*);                             // C2DA::Load2DArray
w4::I32 __fastcall c2da_current_get_int_00413660(void* table, void*, w4::I32 row, void* column, w4::I32* out); // GetINTEntry RET 0Ch
w4::I32 __fastcall c2da_current_get_string_00413270(void* table, void*, w4::I32 row, void* column, void* out); // GetCExoStringEntry RET 0Ch
// ---- math
void* __cdecl yaw_pitch_roll_004acac0(void* out, float yaw, float pitch, float roll);           // YawPitchRoll
void* __cdecl quaternion_multiply_004a9ac0(void* out, const void* a, const void* b);            // operator*(Quaternion, Quaternion)
// ---- server items / party
void* __fastcall recovered_item_construct_005530a0(void* item, void*, w4::U32 id);             // CSWSItem::CSWSItem
w4::U32 __fastcall kotor_fastcall_0055d950(void* item, void*, void* source);                   // CSWSItem::CopyItem
w4::U32 __fastcall recovered_item_template_005608b0(void* item, void*, void* resref);          // CSWSItem::LoadFromTemplate
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*) noexcept(false);    // CSWItem::GetBaseItem
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, w4::I32 id) noexcept(false); // CSWBaseItemArray::GetBaseItem
void* __fastcall recovered_base_item_get_model_resref_005b30e0(void* base, void*, void* out, w4::U32 variation, w4::U32 type); // GetModelResRef RET 0Ch
menu_resources::Ref* __fastcall kotor_fastcall_005556b0(void* item, void*, menu_resources::Ref* out); // CSWSItem::GetIcon
void* __fastcall kotor_fastcall_0055f340(void* item, void*, void* out);                        // CSWSItem::GetPropertyDescription
void* __fastcall recovered_party_repository_00563340(void* table, void*);                      // CSWPartyTable::GetPartyItems
w4::U32 __fastcall recovered_find_item_with_tag_00555ed0(void* repository, void*, void* tag);  // CItemRepository::FindItemWithTag
}

using namespace gchar;
using SS = kotor_reconstruction::StartupString;

namespace {
inline SS* ss(void* p) { return static_cast<SS*>(p); }
inline void or_flag(void* self, U32 off, U32 v) { sw(self, off, lw(self, off) | v); }
inline void and_flag(void* self, U32 off, U32 v) { sw(self, off, lw(self, off) & v); }
inline void copy12(void* dst, const void* src) { std::memcpy(dst, src, 12); }
inline U8 type_of(void* self) { return lb(self, 0x2F4C); }
inline void* item_of(void* self) { return lp(self, 0x2F54); }
// CExoString() as an eh_vector_constructor_iterator element constructor (005B3190).
void* __fastcall string_ctor(void* s, void*) { exo_default(s); return s; }
// Inlined ~CExoArrayList<CSWSItem*>: free(data), data = 0.
void __fastcall item_list_dtor(void* list, void*) { live_free(lp(list, 0)); sw(list, 0, 0); }
// "Remove panel" flag update shared by OnPanelAdded's bail-out path (006C530A..006C5332).
void mark_removed(void* self) {
    const U32 f = lw(self, 0x44) & 0xFFFFFEFFu;
    sw(self, 0x44, f);
    if ((f & 0x600u) != 0x400u) sw(self, 0x44, (f & 0xFFFFFBFFu) | 0x200u);
}
// operator new(0x29C) + CSWSItem(0x7F000000): the raw block is freed when the constructor throws.
void* new_item() {
    void* raw = live_operator_new(0x29C);
    if (raw == nullptr) return nullptr;
    auto g = on_throw([&]() { live_free(raw); });
    void* item = recovered_item_construct_005530a0(raw, nullptr, 0x7F000000u);
    g.armed = false;
    return item;
}
// operator new(0x54) + C2DA(CResRef(name), 0).
void* new_2da(const char* name) {
    void* raw = live_operator_new(0x54);
    if (raw == nullptr) return nullptr;
    auto g = on_throw([&]() { live_free(raw); });
    menu_resources::Ref ref; ref.construct_00406d80(name);
    void* table = c2da_current_construct_00413cc0(raw, nullptr, ref, 0);
    g.armed = false;
    return table;
}

// ======================= method table 00757298 (27 slots; base CSWGuiPanel 0073E010) =======================
const Override ovr_upgrade[] = {
    {0x00, fn(&kotor_fastcall_006c6100)},   // 006C6100 scalar deleting destructor
    {0x34, fn(&kotor_fastcall_006c33a0)},   // 006C33A0 Draw
    {0x3C, fn(&kotor_fastcall_006c6a80)},   // 006C6A80 HandleInputEvent (claude_upgrade_panel2.cpp)
    {0x48, fn(&kotor_fastcall_006c4d70)},   // 006C4D70 OnPanelAdded
    {0x4C, fn(&kotor_fastcall_006c25f0)},   // 006C25F0 OnPanelRemoved
};
void* table_upgrade[27];
bool built_upgrade = false;
const void* vt_upgrade() {
    if (!built_upgrade) { vt_build(table_upgrade, 27, gui_load_panel_methods_0073e010, 27, ovr_upgrade, 5); built_upgrade = true; }
    return table_upgrade;
}

// Inline CSWGui3DSceneView construction (CSWGuiControl, vptr 00752E30, CSWGuiScene at +5Ch).
void scene_view(void* view, Unwind& u) {
    gui_load_control_construct_0041aa80(view, nullptr);
    set_vptr(view, gui_load_scene_view_methods_00752e30);
    { auto g = on_throw([&]() { gui_load_control_destruct_00419a20(view, nullptr); });
      gui_scene_construct_004174b0(at(view, 0x5C), nullptr); g.armed = false; }
    u.push(&gui_load_scene_view_destruct_0067ac50, view);
}
inline void init_named(void* self, U32 off, Str& name) {
    gui_load_panel_init_control_0040b930(self, nullptr, at(self, off), name.p(), 1);
}
}  // namespace

extern "C" {
// ======================= 006C6B60 CSWGuiUpgrade::CSWGuiUpgrade(CSWGuiManager*) RET 4 =======================
void* __fastcall kotor_fastcall_006c6b60(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 006C6B86..006C6B8B
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);                                 // state 0
    set_vptr(self, vt_upgrade());                                                              // 006C6BAD (00757298)
    vector_member(at(self, 0x64), 0x1C4, 4, &gui_load_button_construct_0041c0f0, &gui_load_button_destruct_00419da0, u);  // 006C6B90..006C6BB3
    vector_member(at(self, 0x774), 0x1C4, 3, &gui_load_button_construct_0041c0f0, &gui_load_button_destruct_00419da0, u); // 006C6BB8..006C6BD5
    vector_member(at(self, 0xCC0), 0x140, 4, &gui_load_label_construct_0041acd0, &gui_load_label_destruct_00419d10, u);   // 006C6BDA..006C6BF7
    vector_member(at(self, 0x11C0), 0x140, 3, &gui_load_label_construct_0041acd0, &gui_load_label_destruct_00419d10, u);  // 006C6BFC..006C6C19
    listbox(self, 0x1580, u); listbox(self, 0x1860, u); listbox(self, 0x1B40, u);             // 006C6C1E..006C6C49
    for (U32 off : {0x1E20u, 0x1F60u, 0x20A0u, 0x21E0u, 0x2320u, 0x2460u, 0x25A0u, 0x26E0u, 0x2820u, 0x2960u})
        label(self, off, u);                                                                   // 006C6C4E..006C6CE9
    button(self, 0x2AA0, u);                                                                   // 006C6CEE..006C6CFB
    scene_view(at(self, 0x2C64), u);                                                           // 006C6D00..006C6D24
    scene_view(at(self, 0x2CF4), u);                                                           // 006C6D29..006C6D4D
    button(self, 0x2D84, u);                                                                   // 006C6D52..006C6D5D
    sw(self, 0x2F60, 0); sw(self, 0x2F64, 0); sw(self, 0x2F5C, 0);                              // 006C6D62..006C6D70 CExoArrayList
    u.push(&item_list_dtor, at(self, 0x2F5C));
    sw(self, 0x2F6C, 0); sw(self, 0x2F70, 0); sw(self, 0x2F68, 0);                              // 006C6D76..006C6D82 CExoArrayList
    u.push(&item_list_dtor, at(self, 0x2F68));
    vector_member(at(self, 0x2F84), 8, 4, &string_ctor, &frame_LogStringDtor_005e5c20, u);     // 006C6D88..006C6DA2 state 19h
    Str name; name.empty();                                                                    // 006C6DA7..006C6DB0 [esp+28] state 1Ah
    sw(self, 0x2FA8, ad(new_2da("upgrade")));                                                  // 006C6DB7..006C6DFA (0075721C)
    sw(self, 0x2FAC, ad(new_2da("upcrystals")));                                               // 006C6DF4..006C6E41 (0075741C)
    c2da_current_load_004143b0(lp(self, 0x2FA8), nullptr);                                     // 006C6E37..006C6E47
    c2da_current_load_004143b0(lp(self, 0x2FAC), nullptr);                                     // 006C6E4C..006C6E52
    start_layout(self, "upgrade");                                                             // 006C6E57..006C6E6C (0075721C)
    init_control(self, 0x1E20, "LBL_TITLE");                                                   // 006C6E71..006C6EA1 (00751A20)
    init_control(self, 0x1580, "LB_ITEMS");                                                    // 006C6EA6..006C6ED6 (00751A14)
    init_control(self, 0x1860, "LB_DESC");                                                     // 006C6EDB..006C6F0B (00755D6C)
    sb(self, 0x1B21, 1);                                                                       // 006C6F19
    init_control(self, 0x1B40, "LB_DESC_LS");                                                  // 006C6F10..006C6F47 (00757410)
    sb(self, 0x1B21, 1);                                                                       // 006C6F55
    init_control(self, 0x21E0, "LBL_DESCBG");                                                  // 006C6F4C..006C6F83 (00757404)
    init_control(self, 0x2320, "LBL_DESCBG_LS");                                               // 006C6F88..006C6FB8 (007573F4)
    init_control(self, 0x2C64, "3D_MODEL");                                                    // 006C6FBD..006C6FED (007573E8)
    init_control(self, 0x2CF4, "3D_MODEL_LS");                                                 // 006C6FF2..006C7022 (007573DC)
    init_control(self, 0x25A0, "LBL_SLOTNAME");                                                // 006C7027..006C7057 (00756A44)
    init_control(self, 0x26E0, "LBL_LSSLOTNAME");                                              // 006C705C..006C708C (007573CC)
    init_control(self, 0x2820, "LBL_UPGRADES");                                                // 006C7091..006C70C1 (007573BC)
    init_control(self, 0x2960, "LBL_UPGRADE_COUNT");                                           // 006C70C6..006C70F6 (007573A8)
    init_control(self, 0x2460, "LBL_PROPERTY");                                                // 006C70FB..006C712B (00757398)
    init_control(self, 0x2AA0, "BTN_ASSEMBLE");                                                // 006C7130..006C715A (00757388)
    init_control(self, 0x2D84, "BTN_BACK");                                                    // 006C715F..006C718F (007548AC)
    void* assemble = at(self, 0x2AA0);
    sb(self, 0x2AF4, 0x10);                                                                    // 006C719A
    add_event(self, 0x2AA0, 0x27, self, fn(&kotor_fastcall_006c6190));                         // 006C7194..006C71A5 OnAssemble
    add_event(self, 0x2AA0, 0x2D, self, fn(&kotor_fastcall_006c6190));                         // 006C71AA..006C71B4
    sw(assemble, 0x44, lw(assemble, 0x44) & 0xFFFFFFFBu);                                      // 006C71B9
    for (U32 i = 0; i < 4; ++i) {                                                              // 006C71BD..006C7350
        if (i < 3) {                                                                           // 006C71E0..006C71E3
            frame_LogFormat_005e5680(name.p(), "LBL_UPGRADE3%d", i + 1);                       // 006C71E9..006C71FE (00757378)
            init_named(self, 0x11C0 + i * 0x140, name);                                        // 006C71FA..006C7211
            frame_LogFormat_005e5680(name.p(), "BTN_UPGRADE3%d", i + 1);                       // 006C7216..006C722B (00757368)
            void* b = at(self, 0x774 + i * 0x1C4);                                             // edi
            init_named(self, 0x774 + i * 0x1C4, name);                                         // 006C722E..006C7238
            gui_load_control_add_event_0041ab20(b, nullptr, 0x27, self, fn(&kotor_fastcall_006c6500)); // 006C723D..006C7247 OnSlotSelected
            gui_load_control_add_event_0041ab20(b, nullptr, 0x2D, self, fn(&kotor_fastcall_006c6500)); // 006C724C..006C7256
            gui_load_control_add_event_0041ab20(b, nullptr, 0, self, fn(&kotor_fastcall_006c3c30));    // 006C725B..006C7265 OnEnterSlot
            sw(b, 0x58, i);                                                                    // 006C726A..006C7272
            gui_load_border_params_set_pulsing_00414d00(at(b, 0xF4), nullptr, 1, 0, 1);        // 006C726E..006C727D
        }
        frame_LogFormat_005e5680(name.p(), "LBL_UPGRADE4%d", i + 1);                           // 006C7282..006C729F (00757358)
        init_named(self, 0xCC0 + i * 0x140, name);                                             // 006C72A2..006C72AC
        frame_LogFormat_005e5680(name.p(), "BTN_UPGRADE4%d", i + 1);                           // 006C72B1..006C72C5 (00757348)
        void* b = at(self, 0x64 + i * 0x1C4);                                                  // edi-710h
        init_named(self, 0x64 + i * 0x1C4, name);                                              // 006C72C8..006C72D8
        gui_load_control_add_event_0041ab20(b, nullptr, 0x27, self, fn(&kotor_fastcall_006c6500)); // 006C72DD..006C72EB
        gui_load_control_add_event_0041ab20(b, nullptr, 0x2D, self, fn(&kotor_fastcall_006c6500)); // 006C72F0..006C72FE
        gui_load_control_add_event_0041ab20(b, nullptr, 0, self, fn(&kotor_fastcall_006c3c30));    // 006C7303..006C7311
        sw(b, 0x58, i);                                                                        // 006C7316..006C7326
        gui_load_border_params_set_pulsing_00414d00(at(b, 0xF4), nullptr, 1, 0, 1);            // 006C731A..006C732C
    }
    stop_layout(self);                                                                         // 006C7356..006C7358
    for (U32 off : {0x2620u, 0x2760u, 0x29E0u, 0x28A0u, 0x24E0u, 0x1634u})
        copy12(at(self, off), borrowed_exit_color_007a23b4);                                   // 006C735D..006C741F
    {
        const U32 v = lw(self, 0x1E0C);                                                        // 006C7414
        vcall<void, U32, U32, U32>(at(self, 0x1F60), 0x98, v + 4u, v, 0x3F800000u);           // 006C741A..006C7433
    }
    {
        void* proto = lp(self, 0x184C);                                                        // 006C7439
        copy12(at(proto, 0x90), borrowed_exit_color_007a23b4);                                 // 006C743F..006C745C
        copy12(at(proto, 0x104), borrowed_enter_color_007a23c0);                               // 006C745F..006C7481
        gui_load_border_params_set_pulsing_00414d00(at(proto, 0xF4), nullptr, 1, 1, 0);        // 006C747C..006C748C
    }
    sw(self, 0x2F48, lw(self, 0x2F48) & 0xFFFFFFFEu);                                          // 006C7491..006C74AF
    sb(self, 0x2F4C, 0);                                                                       // 006C749C
    sw(self, 0x2F54, 0); sw(self, 0x2F58, 0);                                                  // 006C74A3..006C74A9
    sw(self, 0x2F74, 0); sw(self, 0x2F78, 0); sw(self, 0x2F7C, 0); sw(self, 0x2F80, 0);        // 006C74B5..006C74C7
    if (kotor_cell_do3dgui_0078d1e4 != 0) {                                                    // 006C74CD..006C74D3
        for (U32 view : {0x2C64u, 0x2CF4u}) {                                                  // 006C74D9..006C7552 / 006C755F..006C75D8
            void* room = lp(self, view + 0x70);
            const float position[3] = {0.0f, 0.0f, 0.0f};                                      // [esp+18]
            const float orientation[4] = {0.0f, 0.0f, 0.0f, 1.0f};                             // [esp+30]
            vcall<void, const char*, const void*, const void*>(room, 0x70, "gui3D_room", position, orientation); // (00753038)
            Str model("upgitem_light");                                                        // (00757338) state 2Dh / 2Eh
            aurora_gui_scene_add_model_name_00417620(at(self, view + 0x5C), nullptr, model.p(), -1); // result unused
        }
    }
    void* back = at(self, 0x2D84);
    sw(back, 0x44, lw(back, 0x44) & 0xFFFFFFFBu);                                              // 006C75E5..006C75FC
    add_event(self, 0x2D84, 0x27, self, on_b());                                               // 006C75F1..006C75FF (00624BB0)
    u.dismiss();
    return self;                                                                               // 006C7604..006C7625 (~name)
}

// ======================= 006C4AD0 ~CSWGuiUpgrade =======================
void __fastcall kotor_fastcall_006c4ad0(void* self, void*) {
    set_vptr(self, vt_upgrade());                                                              // 006C4AF0
    gui_load_listbox_clear_items_00419a90(at(self, 0x1580), nullptr);                          // 006C4AF6..006C4B06
    gui_load_listbox_clear_items_00419a90(at(self, 0x1860), nullptr);                          // 006C4B0B..006C4B13
    if (void* t = lp(self, 0x2FA8)) { vcall<void*, U32>(t, 0x00, 1); sw(self, 0x2FA8, 0); }    // 006C4B18..006C4B2A
    if (void* t = lp(self, 0x2FAC)) { vcall<void*, U32>(t, 0x00, 1); sw(self, 0x2FAC, 0); }    // 006C4B30..006C4B40
    vector_destruct(at(self, 0x2F84), 8, 4, &frame_LogStringDtor_005e5c20);                    // 006C4B46..006C4B5B
    live_free(lp(self, 0x2F68)); sw(self, 0x2F68, 0);                                          // 006C4B60..006C4B71
    live_free(lp(self, 0x2F5C)); sw(self, 0x2F5C, 0);                                          // 006C4B77..006C4B91
    gui_load_button_destruct_00419da0(at(self, 0x2D84), nullptr);                              // 006C4B8B..006C4B9C
    gui_load_scene_view_destruct_0067ac50(at(self, 0x2CF4), nullptr);                          // 006C4BA1..006C4BAC
    gui_load_scene_view_destruct_0067ac50(at(self, 0x2C64), nullptr);                          // 006C4BB1..006C4BBC
    gui_load_button_destruct_00419da0(at(self, 0x2AA0), nullptr);                              // 006C4BC1..006C4BCC
    for (U32 off : {0x2960u, 0x2820u, 0x26E0u, 0x25A0u, 0x2460u, 0x2320u, 0x21E0u, 0x20A0u, 0x1F60u, 0x1E20u})
        gui_load_label_destruct_00419d10(at(self, off), nullptr);                              // 006C4BD1..006C4C6C
    gui_load_listbox_destruct_0041b060(at(self, 0x1B40), nullptr);                             // 006C4C71..006C4C7C
    gui_load_listbox_destruct_0041b060(at(self, 0x1860), nullptr);                             // 006C4C81..006C4C88
    gui_load_listbox_destruct_0041b060(at(self, 0x1580), nullptr);                             // 006C4C8D..006C4C94
    vector_destruct(at(self, 0x11C0), 0x140, 3, &gui_load_label_destruct_00419d10);            // 006C4C99..006C4CB1
    vector_destruct(at(self, 0xCC0), 0x140, 4, &gui_load_label_destruct_00419d10);             // 006C4CB6..006C4CCE
    vector_destruct(at(self, 0x774), 0x1C4, 3, &gui_load_button_destruct_00419da0);            // 006C4CD3..006C4CEB
    vector_destruct(at(self, 0x64), 0x1C4, 4, &gui_load_button_destruct_00419da0);             // 006C4CF0..006C4D04
    gui_load_panel_destruct_0040cf70(self, nullptr);                                           // 006C4D09..006C4D13
}

// 006C6100 scalar deleting destructor RET 4.
void* __fastcall kotor_fastcall_006c6100(void* self, void*, w4::U32 flags) {
    return deleting(self, &kotor_fastcall_006c4ad0, flags);                                    // 006C6100..006C611B
}

// ======================= 006C25F0 OnPanelRemoved =======================
void __fastcall kotor_fastcall_006c25f0(void* self, void*) {
    void* original = lp(self, 0x2F58);                                                         // 006C25F6
    sb(self, 0x2F4C, 0);                                                                       // 006C2600
    sw(self, 0x2F54, 0);                                                                       // 006C2606
    if (original != nullptr) { vcall<void*, U32>(original, 0x00, 1); sw(self, 0x2F58, 0); }   // 006C260C..006C2614
    for (U32 i = 0; i < 4; ++i) {                                                              // 006C261A..006C2637
        if (void* item = lp(self, 0x2F74 + i * 4)) { vcall<void*, U32>(item, 0x00, 1); sw(self, 0x2F74 + i * 4, 0); }
    }
    sw(self, 0x2F60, 0);                                                                       // 006C2639 removed list size
    sw(self, 0x2F6C, 0);                                                                       // 006C263F added list size
    gui_load_panel_on_removed_0040c170(self, nullptr);                                         // 006C2645..006C264B (jmp)
}

// ======================= 006C2F80 ShowItems(int show) RET 4 =======================
void __fastcall kotor_fastcall_006c2f80(void* self, void*, w4::U32 show) {
    const U32 clear = 0xFFFFFFFDu;
    if (show != 0) {                                                                           // 006C2F80..006C2F8B
        for (U32 off : {0x2CA8u, 0x2D38u, 0x25E4u, 0x2724u, 0x29A4u, 0x2864u, 0x24A4u, 0x2AE4u})
            and_flag(self, off, clear);                                                        // 006C2F91..006C2FC8
        for (U32 k = 0; k < 4; ++k) {                                                          // 006C2FCE..006C300C
            and_flag(self, 0xA8 + k * 0x1C4, clear);
            and_flag(self, 0xD04 + k * 0x140, clear);
            if (k < 3) { and_flag(self, 0x7B8 + k * 0x1C4, clear); and_flag(self, 0x1204 + k * 0x140, clear); }
        }
        or_flag(self, 0x15C4, 2);                                                              // 006C300E..006C3018
        return;
    }
    if (type_of(self) == 1) {                                                                  // 006C3023..006C302B
        for (U32 off : {0x2CA8u, 0x25E4u, 0x29A4u, 0x2864u, 0x24A4u, 0x2224u, 0x18A4u}) and_flag(self, off, clear); // 006C3031..006C3062
        for (U32 off : {0x2D38u, 0x2724u, 0x2AE4u, 0x2364u, 0x1B84u}) or_flag(self, off, 2);   // 006C3068..006C308D
        for (U32 k = 0; k < 4; ++k) {                                                          // 006C3093..006C30CE
            and_flag(self, 0xA8 + k * 0x1C4, clear);
            and_flag(self, 0xD04 + k * 0x140, clear);
            if (k < 3) { or_flag(self, 0x7B8 + k * 0x1C4, 2); or_flag(self, 0x1204 + k * 0x140, 2); }
        }
        vcall<void, void*, U32>(self, 0x08, at(self, 0x774), 0);                               // 006C30D0..006C30DD SetActiveControl
        and_flag(self, 0x15C4, clear);                                                         // 006C32CA..006C32D4
        return;
    }
    for (U32 off : {0x2CA8u, 0x25E4u, 0x29A4u, 0x2864u, 0x24A4u, 0x2AE4u, 0x2224u, 0x18A4u}) or_flag(self, off, 2); // 006C30E5..006C311C
    for (U32 off : {0x2D38u, 0x2724u, 0x2364u, 0x1B84u}) and_flag(self, off, clear);           // 006C3122..006C3141
    U32 three_slots = 0;                                                                       // ebx 006C314E
    switch (type_of(self)) {                                                                   // 006C3147..006C315C
    case 4:                                                                                    // 006C3162 armor
        and_flag(self, 0xA8, clear); and_flag(self, 0xD04, clear);
        or_flag(self, 0x26C, 2); or_flag(self, 0xE44, 2); or_flag(self, 0x430, 2); or_flag(self, 0xF84, 2);
        and_flag(self, 0x5F4, clear); and_flag(self, 0x10C4, clear);
        vcall<void, void*, U32>(self, 0x08, at(self, 0x228), 0);                               // 006C31A2..006C31B6
        break;
    case 3:                                                                                    // 006C31BE melee
        for (U32 off : {0xA8u, 0xD04u, 0x26Cu, 0xE44u, 0x430u, 0xF84u, 0x5F4u, 0x10C4u}) and_flag(self, off, clear);
        vcall<void, void*, U32>(self, 0x08, at(self, 0x64), 0);                                // 006C31F0..006C3205
        three_slots = 1;                                                                       // 006C3208
        break;
    case 2:                                                                                    // 006C320F ranged
        for (U32 off : {0xA8u, 0xD04u, 0x26Cu, 0xE44u, 0x430u, 0xF84u, 0x5F4u, 0x10C4u}) or_flag(self, off, 2);
        vcall<void, void*, U32>(self, 0x08, at(self, 0x64), 0);                                // 006C324C..006C3261
        break;
    default: break;
    }
    const U32 bit = (three_slots & 1u) * 2u;                                                   // 006C3264..006C326F
    for (U32 off : {0x7B8u, 0x1204u, 0x97Cu, 0x1344u, 0xB40u, 0x1484u})
        sw(self, off, (lw(self, off) & clear) | bit);                                          // 006C3264..006C32C4
    and_flag(self, 0x15C4, clear);                                                             // 006C32CA..006C32D4
}

// ======================= 006C32E0 SetDescription(const CExoString&) RET 4 =======================
void __fastcall kotor_fastcall_006c32e0(void* self, void*, void* text) {
    gui_load_text_params_set_text_00415e00(at(self, 0x2048), nullptr, text);                   // 006C32EC..006C32F3
    void* listbox = type_of(self) == 1 ? at(self, 0x1B40) : at(self, 0x1860);                  // 006C32F8..006C3307
    gui_load_listbox_clear_items_00419a90(listbox, nullptr);                                   // 006C330D..006C330F
    U32 extent[4];
    std::memcpy(extent, at(self, 0x1F64), 16);                                                 // 006C3314..006C3333
    U32 height;
    if (async_string_length_005e5790(text, nullptr) > 0)                                       // 006C3331..006C3344
        height = vcall<U32>(at(self, 0x2030), 0x08);                                           // 006C3346..006C3348
    else
        height = static_cast<U32>(gui_load_text_font_height_00414ee0(at(self, 0x2030), nullptr)); // 006C334D
    extent[3] = height;                                                                        // 006C3358
    void* label = at(self, 0x1F60);                                                            // 006C335C
    vcall<void, void*>(label, 0x04, extent);                                                   // 006C3352..006C3369 SetExtent
    void* single = label;                                                                      // 006C337B (argument slot reused)
    gui_m48_listbox_add_controls_0041c2c0(listbox, nullptr, &single, 1, 0, 0, 0);              // 006C336C..006C337F
    gui_load_listbox_set_active_control_0041c160(listbox, nullptr, label, 0);                  // 006C3384..006C3389
}

// ======================= 006C33A0 Draw(float delta) RET 4 =======================
void __fastcall kotor_fastcall_006c33a0(void* self, void*, w4::U32 delta_bits) {
    if (kotor_cell_do3dgui_0078d1e4 != 0) {                                                    // 006C33A0..006C33AF
        void* scene = type_of(self) == 1 ? at(self, 0x2D50) : at(self, 0x2CC0);                // 006C33B5..006C33C6
        void* model = kotor_m15_gui_scene_get_model_00416750(scene, nullptr, 1);               // 006C33BC..006C33CC
        if (model != nullptr && vcall<U32, I32>(model, 0x108, -1) == 0) {                      // 006C33D3..006C33E5
            alignas(4) unsigned char rotation[16];                                             // [esp+0Ch]
            alignas(4) unsigned char current[16];                                              // [esp+1Ch]
            alignas(4) unsigned char product[16];                                              // [esp+2Ch]
            alignas(4) unsigned char unused[16];                                               // [esp+3Ch] (never written here)
            const float yaw = static_cast<float>(static_cast<double>(bits_f(delta_bits)) * -70.0); // 006C33E7..006C33F8 fmul [007570D8]
            void* r = yaw_pitch_roll_004acac0(rotation, yaw, 0.0f, 0.0f);                      // 006C33EB..006C3403
            void* o = vcall<void*, void*>(model, 0x68, current);                               // 006C3406..006C340E
            const void* q = quaternion_multiply_004a9ac0(product, o, r);                       // 006C3411..006C3417
            struct Q4 { float v[4]; } value;
            std::memcpy(&value, q, 16);                                                        // 006C341C..006C3434 (by value)
            reinterpret_cast<void(__thiscall*)(void*, void*, Q4)>(vslot(model, 0x60))(model, unused, value); // 006C3437..006C343E
        }
    }
    kotor_panel_draw_0040B760(self, nullptr, delta_bits);                                      // 006C3441..006C3448
}

// ======================= 006C3460 GetArmorModelAndTextureResRef RET 14h =======================
void __fastcall kotor_fastcall_006c3460(void* self, void*, w4::U32 row, w4::U32 model_variation, w4::U32 texture_variation,
                                        void* model_out, void* texture_out) {
    (void)self;
    char texture_column[5];                                                                    // [esp] "TEXA" (00754A30)
    char model_column[7];                                                                      // [esp+8] "ModelA" (00754A38)
    std::memcpy(model_column, "ModelA", 7);                                                    // 006C3478..006C34A9
    std::memcpy(texture_column, "TEXA", 5);                                                    // 006C348E..006C34AD
    I32 v = static_cast<I32>(model_variation);                                                 // 006C3497..006C34BF
    if (v > 10) v = 10;
    else if (v < 1) v = 1;
    I32 tex = 1;                                                                               // 006C34CB..006C34D9 (arg slot)
    if (static_cast<I32>(texture_variation) >= 1) tex = static_cast<I32>(texture_variation);
    const char letter = static_cast<char>(static_cast<U8>(v) + 0x40);                          // 006C34DD
    model_column[5] = letter;                                                                  // 006C34E8
    texture_column[3] = letter;                                                                // 006C34EC
    void* table = lp(lp(kotor_cell_rules_007a3a28, 0xB8), 0x0C);                               // 006C34F5..006C3504
    bool failed;
    {
        Str column(model_column);                                                              // 006C34DF..006C34F0 state 0
        failed = c2da_current_get_string_00413270(table, nullptr, static_cast<I32>(row), column.p(), model_out) == 0; // 006C350B..006C352B
    }                                                                                          // 006C3535
    if (failed) GuiAssign005e5140(model_out, nullptr, "");                                     // 006C353A..006C3545 (0073D71C)
    I32 found;
    {
        Str column(texture_column);                                                            // 006C354A..006C3553 state 1
        found = c2da_current_get_string_00413270(lp(lp(kotor_cell_rules_007a3a28, 0xB8), 0x0C), nullptr,
                                                 static_cast<I32>(row), column.p(), texture_out); // 006C3558..006C3577
    }                                                                                          // 006C358A
    if (found == 0) { GuiAssign005e5140(texture_out, nullptr, ""); return; }                   // 006C358F..006C3614
    frame_LogFormat_005e5680(texture_out, "%s%02d", frame_LogCStr_005e5670(texture_out, nullptr), tex); // 006C3595..006C35AB (00754A28)
    {
        menu_resources::Ref ref;                                                               // [esp+24]
        async_ref_construct_string_00406d60(&ref, nullptr, texture_out);                       // 006C35AE..006C35B3
        if (kotor_resource_demand::resource_manager_007a39e8->exists_00408bc0(&ref, 3, nullptr) != 0) return; // 006C35B8..006C35CE
    }
    {
        menu_resources::Ref ref;                                                               // [esp+34]
        async_ref_construct_string_00406d60(&ref, nullptr, texture_out);                       // 006C35D0..006C35D5
        if (kotor_resource_demand::resource_manager_007a39e8->exists_00408bc0(&ref, 0xBBF, nullptr) != 0) return; // 006C35DA..006C35F3
    }
    frame_LogFormat_005e5680(texture_out, "%s%02d", frame_LogCStr_005e5670(model_out, nullptr), 1); // 006C35F5..006C360A
}

// ======================= 006C3630 Update3DView =======================
void __fastcall kotor_fastcall_006c3630(void* self, void*) {
    if (kotor_cell_do3dgui_0078d1e4 == 0) return;                                              // 006C3654..006C365B
    void* view = at(self, 0x2C64);                                                             // 006C3661..006C3667 [esp+10]
    if (type_of(self) == 1) view = at(self, 0x2CF4);                                           // 006C366B..006C367A
    void* scene = at(view, 0x5C);                                                              // 006C367E..006C368B [esp+14] / edi
    kotor_fastcall_00416710(scene, nullptr, 1, 1);                                             // 006C3685..006C368F RemoveModel(1, 1)
    Str model; model.empty();                                                                  // 006C3694..006C3698 [esp+30] state 0
    Str texture; texture.empty();                                                              // 006C369D..006C36AC [esp+28] state 1
    void* base = kotor_m15b_item_get_base_item_005b4790(item_of(self), nullptr);               // 006C36B1..006C36BF esi
    void* item = item_of(self);                                                                // 006C36C4 edx
    if (type_of(self) == 4) {                                                                  // 006C36CC..006C36D3
        kotor_fastcall_006c3460(self, nullptr, 0x89, lb(base, 0xAF), lb(item, 0x266), model.p(), texture.p()); // 006C36D5..006C36FA
    } else {
        menu_resources::Ref ref;                                                               // [esp+70]
        const U32 variation = (ad(item) & 0xFFFF0000u) | lb(item, 0x264);                      // 006C3701..006C3707 movzx dx, al
        void* r = recovered_base_item_get_model_resref_005b30e0(base, nullptr, &ref, variation, 0x6D); // 006C370B..006C371A ('m')
        c2da_ref_to_string_00405f70(r, nullptr, model.p());                                    // 006C371F..006C3721 CopyToString
    }
    Str camera_hook; camera_hook.empty();                                                      // 006C3726..006C372A [esp+18] state 2
    Str rotate_hook; rotate_hook.empty();                                                      // 006C372F..006C373B [esp+20] state 3
    frame_LogFormat_005e5680(camera_hook.p(), "camerahook%d", static_cast<U32>(lb(base, 0xAC))); // 006C3740..006C375B (007570F4)
    frame_LogFormat_005e5680(rotate_hook.p(), "rotatehook%d", static_cast<U32>(lb(base, 0xAC))); // 006C3760..006C3772 (007570E4)
    const U8 kind = lb(base, 0xAC);                                                            // 006C3777
    bool suffix = false;
    if (kind == 0) suffix = li(item_of(self), 0x0C) == 1;                                      // 006C3780 / 006C3851..006C385B
    else if (kind == 4) suffix = li(item_of(self), 0x0C) == 0x14;                              // 006C3788..006C379A
    if (suffix) {                                                                              // 006C37A0..006C3914 / 006C3861..
        for (Str* s : {&camera_hook, &rotate_hook}) {
            Str a("a");                                                                        // (0073D5D4)
            Str sum;
            GuiConcat005e5d10(ss(s->p()), nullptr, ss(sum.p()), ss(a.p())); sum.live = true;   // operator+
            async_assign_string_005e5c50(s->p(), nullptr, sum.p());                            // operator=
        }
    }
    void* camera = lp(view, 0x74);                                                             // 006C3919..006C391D esi
    void* room = kotor_m15_gui_scene_get_model_00416750(scene, nullptr, 0);                    // 006C3920..006C3924 edi
    if (vcall<U32, const char*>(room, 0x10C, frame_LogCStr_005e5670(camera_hook.p(), nullptr)) == 0) // 006C392B..006C3941
        GuiAssign005e5140(camera_hook.p(), nullptr, "camerahook");                             // 006C3943..006C394C (0074F42C)
    {
        const char* hook = frame_LogCStr_005e5670(camera_hook.p(), nullptr);                   // 006C3953..006C3959
        void* base_model = kotor_m15_gui_scene_get_model_00416750(scene, nullptr, 0);          // 006C395E..006C3965
        vcall<void, void*, const char*, U32>(camera, 0x74, base_model, hook, 0);               // 006C396A..006C396D
    }
    vcall<void, U32>(camera, 0x44, 0x41B5CED9u);                                               // 006C3970..006C3979 (22.726f)
    void* added = aurora_gui_scene_add_model_name_00417620(scene, nullptr, model.p(), -1);     // 006C397C..006C3987
    if (added != nullptr) {                                                                    // 006C398C..006C3990
        if (frame_LogIsEmpty_005e58a0(texture.p(), nullptr) == 0)                              // 006C3996..006C39A1
            vcall<void, const char*, const char*>(added, 0x34, "", frame_LogCStr_005e5670(texture.p(), nullptr)); // 006C39A3..006C39B6
        if (vcall<U32, const char*>(room, 0x10C, frame_LogCStr_005e5670(rotate_hook.p(), nullptr)) != 0) { // 006C39B9..006C39CF
            vcall<void, void*, const char*, U32>(added, 0x50, room, frame_LogCStr_005e5670(rotate_hook.p(), nullptr), 0); // 006C39D1..006C39E2
            vcall<void, const char*, U32, U32, U32>(room, 0x18, "rotate", 0x3F800000u, 0, 0);  // 006C39E5..006C39F7 (007546F0)
        }
        if (lw(kotor_m15b_item_get_base_item_005b4790(item_of(self), nullptr), 0x90) != 0)     // 006C39FA..006C3A0D
            vcall<void, const char*, U32, U32, U32>(added, 0x18, "powered", 0x3F800000u, 0, 0); // 006C3A0F..006C3A21 (007570DC)
        if (type_of(self) == 4)                                                                // 006C3A24..006C3A2B
            vcall<void, const char*, U32, U32, U32>(added, 0x18, "neutral", 0x3F800000u, 0, 0); // 006C3A2D..006C3A3F (007485D0)
    }
}                                                                                              // 006C3A42..006C3A84 ~rotate ~camera ~texture ~model

// ======================= 006C3AA0 UpdateItems =======================
void __fastcall kotor_fastcall_006c3aa0(void* self, void*) {
    menu_resources::Ref icon; c2da_default_ref_00405ed0(&icon, nullptr);                       // 006C3ABD..006C3AC5 [esp+28]
    Str unused; unused.empty();                                                                // 006C3ACA..006C3ACE [esp+20] state 0
    const U32 index = static_cast<U32>(type_of(self)) - 1u;                                    // 006C3AD3..006C3ADC
    U32 first = 0;                                                                             // esi
    void* labels;                                                                              // edx
    switch (index) {                                                                           // 006C3ADD..006C3AE6 (table 006C3C0C)
    case 0: case 2: labels = at(self, 0x11C0); break;                                          // 006C3AED
    case 1: labels = at(self, 0xCC0); break;                                                   // 006C3AFA
    case 3: first = 1; labels = at(self, 0xCC0); break;                                        // 006C3AF5
    default: labels = self; break;                                                             // 006C3B02
    }
    U8 count = 0;                                                                              // 006C3B06
    switch (index) {                                                                           // 006C3B08..006C3B0D (table 006C3C1C)
    case 0: case 2: count = 3; break;                                                          // 006C3B14
    case 1: count = 4; break;                                                                  // 006C3B18
    case 3: count = 2; break;                                                                  // 006C3B1C
    default: break;
    }
    const I32 end = static_cast<I32>(count) + static_cast<I32>(first);                         // 006C3B1E..006C3B21
    for (I32 i = static_cast<I32>(first); i < end; ++i) {                                      // 006C3B23..006C3BB7
        void* fill = static_cast<U8*>(labels) + 0x70 + i * 0x140;                              // ebp
        void* item = lp(self, 0x2F74 + static_cast<U32>(i) * 4);
        if (item != nullptr) {                                                                 // 006C3B40..006C3B44
            kotor_m15b_base_item_array_get_base_item_005b31a0(lp(kotor_cell_rules_007a3a28, 0x34), nullptr, li(item, 0x0C)); // 006C3B46..006C3B53 (unused)
            menu_resources::Ref fetched, discarded;                                            // [esp+14] / [esp+40]
            menu_resources::Ref* r = kotor_fastcall_005556b0(lp(self, 0x2F74 + static_cast<U32>(i) * 4), nullptr, &fetched); // 006C3B58..006C3B5F
            resource_ref_assignment::assign_00406120(&icon, nullptr, &discarded, r);           // 006C3B64..006C3B6E
        } else {
            const char* text = upgrade::slot_info(type_of(self), static_cast<U32>(i)).icon;     // 006C3B75..006C3B87 [00756FB4]
            menu_resources::Ref discarded;                                                     // [esp+50]
            unresolved_ref_assign_00406290(&icon, nullptr, &discarded, text);                  // 006C3B8E..006C3B98
        }
        gui_load_border_params_set_fill_00414c00(fill, nullptr, &icon, 0);                     // 006C3B9D..006C3BA6
    }
    {
        Str description;                                                                       // [esp+10] state 1
        void* r = kotor_fastcall_0055f340(item_of(self), nullptr, description.p()); description.live = true; // 006C3BBE..006C3BC9
        kotor_fastcall_006c32e0(self, nullptr, r);                                             // 006C3BCE..006C3BD6
    }                                                                                          // 006C3BDB..006C3BE4
}                                                                                              // 006C3BE9..006C3BF5 ~unused

// ======================= 006C4D70 OnPanelAdded =======================
void __fastcall kotor_fastcall_006c4d70(void* self, void*) {
    if (lp(self, 0x2F54) == nullptr) { mark_removed(self); return; }                           // 006C4D93..006C4D9E / 006C530A
    {
        void* copy = new_item();                                                               // 006C4DA4..006C4DCD state 0
        sw(self, 0x2F58, ad(copy));                                                            // 006C4DE0
        kotor_fastcall_0055d950(copy, nullptr, lp(self, 0x2F54));                              // 006C4DCD..006C4DE6 CopyItem
    }
    I32 first = 0;                                                                             // [esp+18]
    void* labels = nullptr;                                                                    // [esp+14]
    const U32 index = static_cast<U32>(type_of(self)) - 1u;                                    // 006C4DEB..006C4DF2
    switch (index) {                                                                           // 006C4DF8..006C4E04 (table 006C5348)
    case 0: {                                                                                  // 006C4E0B lightsaber
        Str column; column.empty();                                                            // [esp+20] state 1
        Str value; value.empty();                                                              // [esp+30] state 2
        Str resref; resref.empty();                                                            // [esp+28] state 3
        const U8 kind = lb(kotor_m15b_item_get_base_item_005b4790(lp(self, 0x2F54), nullptr), 0xAC); // 006C4E33..006C4E42
        const char* name;
        if (kind == 0x27) name = "DoubleMdlVar";                                               // 006C4EA6 (00757310)
        else if (kind == 0x28) name = "ShortMdlVar";                                           // 006C4E9F (00757320)
        else if (kind == 0x29) name = "LongMdlVar";                                            // 006C4E98 (0075732C)
        else { gui_m48_panel_set_to_destroy_00624a00(self, nullptr, 0); return; }              // 006C4E54..006C4E97
        GuiAssign005e5140(column.p(), nullptr, name);                                          // 006C4EAB..006C4EAF
        for (I32 row = 0; row < li(lp(self, 0x2FAC), 0x24); ++row) {                           // 006C4EB4..006C4F3F
            if (c2da_current_get_string_00413270(lp(self, 0x2FAC), nullptr, row, column.p(), value.p()) == 0) continue; // 006C4EC3..006C4EDB
            if (exostring_compare_no_case_005e6450(value.p(), nullptr, at(lp(self, 0x2F54), 0x28)) == 0) continue; // 006C4EDD..006C4EF2
            I32 found;
            { Str t("Template");                                                               // (00757304) state 4
              found = c2da_current_get_string_00413270(lp(self, 0x2FAC), nullptr, row, t.p(), resref.p()); } // 006C4F02..006C4F2A
            if (found == 0) continue;                                                          // 006C4F2F..006C4F31
            void* crystal = new_item();                                                        // 006C4F43..006C4F6D state 5
            sw(self, 0x2F78, ad(crystal));                                                     // 006C4F7D
            menu_resources::Ref ref;                                                           // [esp+50]
            async_ref_construct_string_00406d60(&ref, nullptr, resref.p());                    // 006C4F6D..006C4F83
            recovered_item_template_005608b0(lp(self, 0x2F78), nullptr, &ref);                 // 006C4F88..006C4F93 LoadFromTemplate
            break;
        }
        if (lp(self, 0x2F78) == nullptr) { gui_m48_panel_set_to_destroy_00624a00(self, nullptr, 0); return; } // 006C4F98..006C4FEA
        labels = at(self, 0x11C0);                                                             // 006C4FEB..006C4FF5
        break;                                                                                 // 006C4FF9..006C5022 (~resref ~value ~column)
    }
    case 2: labels = at(self, 0x11C0); break;                                                  // 006C5024 melee
    case 1:                                                                                    // 006C5030 ranged
        gui_load_nav_set_move_to_00417660(at(self, 0x228), nullptr, 1, at(self, 0x64));        // 006C5030..006C503C
        gui_load_nav_set_move_to_00417660(at(self, 0x3EC), nullptr, 3, at(self, 0x5B0));       // 006C5041..006C504F
        labels = at(self, 0xCC0);                                                              // 006C5054..006C505A
        break;
    case 3:                                                                                    // 006C5060 armor
        gui_load_nav_set_move_to_00417660(at(self, 0x228), nullptr, 1, at(self, 0x3EC));       // 006C5060..006C5071
        gui_load_nav_set_move_to_00417660(at(self, 0x3EC), nullptr, 3, at(self, 0x228));       // 006C5076..006C507A
        first = 1;                                                                             // 006C5085
        labels = at(self, 0xCC0);                                                              // 006C507F..006C508D
        break;
    default: mark_removed(self); return;                                                       // 006C4DFE -> 006C530A
    }
    kotor_fastcall_006c2f80(self, nullptr, 0);                                                 // 006C5091..006C5095 ShowItems(0)
    U8 count = 0;                                                                              // 006C50A1
    switch (static_cast<U32>(type_of(self)) - 1u) {                                            // 006C509A..006C50A8 (table 006C5358)
    case 0: case 2: count = 3; break;                                                          // 006C50AF (bl)
    case 1: count = 4; break;                                                                  // 006C50B3
    case 3: count = 2; break;                                                                  // 006C50B7
    default: break;
    }
    const I32 end = static_cast<I32>(count) + first;                                           // 006C50B9..006C50C4 ebp
    Str tmpl; tmpl.empty();                                                                    // 006C50C0..006C50C6 [esp+38] state 6
    for (I32 row = 0; row < li(lp(self, 0x2FA8), 0x24); ++row) {                               // 006C50CB..006C52CB
        I32 upgrade_type;                                                                      // [esp+1C]
        { Str c("UpgradeType");                                                                // (00748DB8) state 7
          c2da_current_get_int_00413660(lp(self, 0x2FA8), nullptr, row, c.p(), &upgrade_type); } // 006C50F0..006C5125
        { Str c("Template");                                                                   // (00757304) state 8
          c2da_current_get_string_00413270(lp(self, 0x2FA8), nullptr, row, c.p(), tmpl.p()); } // 006C512A..006C515F
        I32 slot = first;                                                                      // 006C5164 edi
        if (slot >= end) continue;                                                             // 006C5168..006C516A
        for (; slot < end; ++slot) {                                                           // 006C5180..006C51A2
            if (lw(self, 0x2F74 + static_cast<U32>(slot) * 4) != 0) continue;                  // 006C5180..006C5183
            if (upgrade_type == upgrade::slot_info(type_of(self), static_cast<U32>(slot)).upgrade_type) break; // 006C5185..006C519A [00756FB0]
        }
        if (slot >= end) continue;                                                             // 006C51A4 / 006C51A9..006C51AB
        if (type_of(self) != 1) {                                                              // 006C51B1..006C51B8
            {
                Str lower;                                                                     // [esp+48] state 9
                SS* r = directory_list::LowerCase005e6080(ss(tmpl.p()), nullptr, ss(lower.p())); lower.live = true; // 006C51BA..006C51C3
                async_assign_string_005e5c50(at(self, 0x2F84 + static_cast<U32>(slot) * 8), nullptr, r); // 006C51C8..006C51D8
            }                                                                                  // 006C51DD..006C51E6
            sw(labels, 0x7C + static_cast<U32>(slot) * 0x140, 0x3F800000u);                    // 006C51EB..006C51F5 alpha 1.0f
        }
        const U8 key = static_cast<U8>(row);                                                   // bl
        sb(self, 0x2FA4 + static_cast<U32>(slot), key);                                        // 006C5200
        bool create = key == 0xFF;                                                             // 006C51FD / 006C5207
        if (!create) create = (lw(lp(self, 0x2F54), 0x294) & (1u << (key & 31u))) != 0;       // 006C5209..006C521E
        if (create) {                                                                          // 006C5220
            void* upgrade_item = new_item();                                                   // 006C5220..006C524A state 0Ah
            sw(self, 0x2F74 + static_cast<U32>(slot) * 4, ad(upgrade_item));                   // 006C525B
            menu_resources::Ref ref;                                                           // [esp+60]
            async_ref_construct_string_00406d60(&ref, nullptr, tmpl.p());                      // 006C524A..006C5262
            recovered_item_template_005608b0(lp(self, 0x2F74 + static_cast<U32>(slot) * 4), nullptr, &ref); // 006C5267..006C5273
            continue;
        }
        if (type_of(self) == 1) continue;                                                      // 006C527A..006C5281
        void* repository = recovered_party_repository_00563340(party_table(), nullptr);        // 006C5283..006C529A
        if (recovered_find_item_with_tag_00555ed0(repository, nullptr, at(self, 0x2F84 + static_cast<U32>(slot) * 8)) == 0x7F000000u) // 006C529F..006C52AB
            sw(labels, 0x7C + static_cast<U32>(slot) * 0x140, 0x3E800000u);                    // 006C52AD..006C52B7 alpha 0.25f
    }
    kotor_fastcall_006c3630(self, nullptr);                                                    // 006C52D1..006C52D3 Update3DView
    kotor_fastcall_006c3aa0(self, nullptr);                                                    // 006C52D8..006C52DA UpdateItems
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 006C52DF..006C52E1
}                                                                                              // 006C52E6..006C52F2 ~tmpl
}  // extern "C"
