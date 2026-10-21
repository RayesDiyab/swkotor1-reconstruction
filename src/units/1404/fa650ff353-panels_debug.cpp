// Lane W4 G15: the four debug panels CGuiInGame::Initialize constructs unconditionally (00632860):
// CSWGuiCreateItemMenu (006D0620), CSWGuiLoadModuleDebugMenu (006CFA70), CSWGuiPowersFeatsSkillsDebugMenu (006CF5A0)
// and CSWGuiCreateItemSubMenu (006BDC60), their destructors, CreateOption builders and the leaf
// CSWBaseItem::GetNameText (005B4690). Transcribed from dis/ann_006CFA70.asm and the ann.py listings cited per line.
#include "37dfeb87ca-gui_panels.hpp"
using namespace w4gui;

namespace kotor_reconstruction { struct StartupString; struct ArrayMutations; }
namespace options_video { struct String; }
using SS = kotor_reconstruction::StartupString;
SS* __fastcall GuiConcat005e5d10(SS* self, void*, SS* out, const SS* rhs);                       // CExoString::operator+
namespace startup_rand { SS* __fastcall SubString005e6270(SS* self, void*, SS* out, int start, int count); }
namespace directory_list { SS* __fastcall LowerCase005e6080(SS* self, void*, SS* out); }
namespace client_movies { int __fastcall StringFind_005e5eb0(options_video::String* self, void*, options_video::String* what, int start); }
namespace f200_ports { void __fastcall Enumerate005e6640(void* base, void*, void* out, SS* dir, unsigned type, unsigned, unsigned); }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations* list, void*, unsigned count); }
namespace kotor_sound_internal {
void* __fastcall StringListConstruct005e9530(void* self, void*);                                   // CExoStringList::CExoStringList
void __fastcall StringListDestroy005e9860(void* self, void*);                                      // CExoStringList::~CExoStringList
}
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }              // [007A39E0] CExoBase*
namespace kotor_window_shell { struct Context; extern Context* bound_context; }                     // globals->graphics_version_7a3cc4 (+0)
extern "C" void* kotor_cell_rules_007a3a28;                                                        // [007A3A28] CSWRules*

extern "C" {
std::int32_t __fastcall kotor_fastcall_005e5500(void* self, void*, void* other);                   // CExoString::operator<=
std::int32_t __fastcall async_string_equal_005e5200(void* self, void*, void* other);                // CExoString::operator==
std::int32_t __fastcall async_string_length_005e5790(void* self, void*);                            // CExoString::GetLength
void* __fastcall kotor_fastcall_005e6890(void* aliases, void*, void* name, std::uint32_t);          // CExoAliasList::GetAliasPath
void __fastcall kotor_fastcall_0041c1d0(void* listbox, void*, void* list, U32, U32, U32);          // CSWGuiListBox::AddControls
void* __fastcall kotor_fastcall_0041a5a0(void* listbox, void*, I32 index);                          // CSWGuiListBox::GetControl
void __fastcall gui_load_listbox_clear_items_00419a90(void* listbox, void*);                        // CSWGuiListBox::ClearItems
std::int32_t __fastcall gui_load_border_params_dim_00414cd0(void* border, void*);                   // CSWGuiBorderParams::GetBorderDim
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, U32 index); // CSWBaseItemArray::GetBaseItem
U32 __fastcall tlk_fetch_0041e550(void* tlk, void*, U32 strref, void* str_res, U32 flag) noexcept(false); // CTlkTable::Fetch
void* __fastcall c2da_default_ref_00405ed0(void* self, void*);                                      // CResRef::CResRef()
std::int32_t __fastcall c2da_current_load_004143b0(void* table, void*);                             // C2DA::Load2DArray
std::int32_t __fastcall c2da_current_get_string_00413270(void* table, void*, I32 row, void* column, void* out); // C2DA::GetCExoStringEntry
void* __cdecl live_operator_new(std::size_t);
}
struct Ref16 { unsigned char bytes[16]; };
extern "C" void* __fastcall c2da_current_construct_00413cc0(void* self, void*, Ref16 name, std::int32_t); // C2DA::C2DA(CResRef, int)

namespace {
void* graphics_version_7a3cc4() { return *reinterpret_cast<void**>(kotor_window_shell::bound_context); }
SS* ss(void* s) { return static_cast<SS*>(s); }
// "Build: " + [007A3CC4] into `out` (temporary "Build: " destroyed right after the concat).
void build_label_text(U32* out) {
    ExoString prefix("Build: ");
    GuiConcat005e5d10(ss(prefix.storage), nullptr, ss(out), static_cast<const SS*>(graphics_version_7a3cc4()));
}
// CExoArrayList<T> {data, size, allocated} with T = 4-byte pointer (CExoArrayList::Allocate / Add inlined).
struct PtrList { void** data = nullptr; I32 size = 0; I32 allocated = 0; };
void list_add(PtrList& list, void* value) {                                                          // 006D009F..006D0105
    if (list.size == list.allocated) {
        list.allocated = list.allocated != 0 ? list.allocated * 2 : 0x10;
        void** old = list.data;
        list.data = static_cast<void**>(live_operator_new(static_cast<std::size_t>(list.allocated) * 4u));
        for (I32 i = 0; i < list.size; ++i) list.data[i] = old[i];
        if (old != nullptr) live_free(old);
    }
    list.data[list.size++] = value;
}
// CExoArrayList<CExoString> destroy (new[]-allocated: count at data-4, element 8 bytes).
struct StringList { U8* data = nullptr; I32 size = 0; I32 allocated = 0; };
void string_list_release(StringList& list) {                                                         // 006D0165..006D0198
    if (list.data != nullptr) {
        const U32 count = lw(list.data - 4, 0);
        for (U32 i = count; i-- > 0;) frame_LogStringDtor_005e5c20(list.data + i * 8u, nullptr);
        live_free(list.data - 4);
    }
    list.data = nullptr;
}
// Shared listbox/label debug layout prologue ("debug": LB_OPTIONS +64, LBL_BUILD +344).
void debug_layout(void* self) {
    start_layout(self, "debug");
    init_control(self, 0x64, "LB_OPTIONS");
    init_control(self, 0x344, "LBL_BUILD");
    stop_layout(self);
}
// CreateOption body shared by 006CEA90 / 006D0490 / 006CF890 (instruction-identical).
void* create_option(void* panel, void* text, const void* handler, U32 tag) {
    void* raw = live_operator_new(0x1C4);                                                            // 006CEAAE..006CEAC7
    void* button = raw;
    if (raw != nullptr) {
        { auto w4_guard_0 = ::w4gui::on_throw([&]() { live_free(raw); });  gui_load_button_construct_0041c0f0(raw, nullptr); w4_guard_0.armed = false; }  // 006CEADD
    }
    I32 extent[4] = {0, 0, 0, 0};                                                                    // 006CEAB7..006CEAC3
    vcall<void, void*, U32>(button, 0x98, extent, tag);                                              // 006CEAE4..006CEAFA Initialize
    gui_load_text_params_set_text_00415e00(at(button, 0x16C), nullptr, text);                        // 006CEB00..006CEB0B
    const I32 a = gui_load_border_params_dim_00414cd0(at(button, 0x80), nullptr);                    // 006CEB10..006CEB1E
    const I32 b = gui_load_border_params_dim_00414cd0(at(button, 0xF4), nullptr);                    // 006CEB23..006CEB27
    const I32 d = gui_load_border_params_dim_00414cd0(b > a ? at(button, 0xF4) : at(button, 0x80), nullptr); // 006CEB2C..006CEB34
    extent[3] = vcall<I32>(at(button, 0x154), 0x08) + d * 2;                                         // 006CEB39..006CEB4D
    vcall<void, void*>(button, 0x04, extent);                                                        // 006CEB51..006CEB5A SetExtent
    gui_load_control_add_event_0041ab20(button, nullptr, 0x27, panel, handler);                      // 006CEB5D..006CEB6B
    gui_load_control_add_event_0041ab20(button, nullptr, 0x2D, panel, handler);                      // 006CEB70..006CEB76
    return button;
}
void finish_options(void* self, PtrList& list, bool direct_active) {
    void* box = at(self, 0x64);
    kotor_fastcall_0041c1d0(box, nullptr, &list, 1, 0, 0);                                            // AddControls
    sw(box, 0x2BC, lw(box, 0x2BC) | 0x40u);
    if (direct_active) gui_load_panel_set_active_control_0040a630(self, nullptr, box, 0);            // SetActiveControl
    else vcall<void, void*, U32>(self, 0x08, box, 0);
}
}  // namespace

extern "C" {
#define W4_TRAP_HANDLER(addr, name) \
    void __fastcall kotor_fastcall_##addr(void*, void*, void*) { kotor_unreachable_trap(0x##addr##u, name); }
W4_TRAP_HANDLER(006cf3d0, "CSWGuiPowersFeatsSkillsDebugMenu::OnAddPowerPicked")
W4_TRAP_HANDLER(006cf020, "CSWGuiPowersFeatsSkillsDebugMenu::OnAddSkillPicked")
W4_TRAP_HANDLER(006cf3f0, "CSWGuiPowersFeatsSkillsDebugMenu::OnAddFeatPicked")
// 006D05A0 CSWGuiCreateItemMenu::Back: trap removed (lane closeC); faithful body in src/gui/claude_closeC_gui_debug.cpp.
void __fastcall kotor_fastcall_006d05a0(void*, void*, void*);
W4_TRAP_HANDLER(006d05e0, "CSWGuiCreateItemMenu::ItemSubTypeMenu")
// 006CF9D0 CSWGuiLoadModuleDebugMenu::OnModulePicked: trap removed (lane closeC); faithful body in src/gui/claude_closeC_gui_debug.cpp.
void __fastcall kotor_fastcall_006cf9d0(void*, void*, void*);
#undef W4_TRAP_HANDLER

// ---------------------------------------------------------------- CreateOption (thiscall(CExoString*, handler, tag) RET 0Ch)
void* __fastcall kotor_fastcall_006cea90(void* self, void*, void* text, const void* handler, U32 tag) { return create_option(self, text, handler, tag); }
void* __fastcall kotor_fastcall_006d0490(void* self, void*, void* text, const void* handler, U32 tag) { return create_option(self, text, handler, tag); }
void* __fastcall kotor_fastcall_006cf890(void* self, void*, void* text, const void* handler, U32 tag) { return create_option(self, text, handler, tag); }

// ---------------------------------------------------------------- 005B4690 CSWBaseItem::GetNameText(CExoString* out) RET 4
void* __fastcall kotor_fastcall_005b4690(void* item, void*, void* out) {
    struct StrRes { U32 text[2]; Ref16 sound; } res;                                                 // [esp+8] / [esp+10]
    exo_default(res.text);
    { auto w4_guard_1 = ::w4gui::on_throw([&]() { frame_LogStringDtor_005e5c20(res.text, nullptr); }); 
        c2da_default_ref_00405ed0(&res.sound, nullptr);                                              // 005B46C8
        static const unsigned char rdata_0073ecd0 = 0x00;                                            // .rdata byte 0073ECD0 (image value 00)
        tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, lw(item, 0), &res, rdata_0073ecd0);     // 005B46CD..005B46EB
        WindowCopy005e5b00(out, nullptr, res.text);                                                  // 005B46F0..005B46FB
        w4_guard_1.armed = false; }
    frame_LogStringDtor_005e5c20(res.text, nullptr);                                                 // 005B4700..005B4711
    return out;
}

// ---------------------------------------------------------------- CSWGuiPowersFeatsSkillsDebugMenu (0x490, table 00757A78)
// 006CF410 CreateMainPowersFeatsSkillsMenu() RET 0.
void __fastcall kotor_fastcall_006cf410(void* self, void*) {
    zero(self, 0x484);                                                                              // 006CF435
    gui_load_listbox_clear_items_00419a90(at(self, 0x64), nullptr);                                  // 006CF43B
    PtrList list;
    const U32 tag = lw(self, 0x330);                                                                 // 006CF44C
    menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&list), nullptr, 3); // 006CF452..006CF45C
    list.size = 3;                                                                                   // 006CF46A
    { auto w4_guard_2 = ::w4gui::on_throw([&]() { live_free(list.data); }); 
        { ExoString s("Add Power"); list.data[0] = kotor_fastcall_006cea90(self, nullptr, s.storage, fn(&kotor_fastcall_006cf3d0), tag); }
        { ExoString s("Add Skill"); list.data[1] = kotor_fastcall_006cea90(self, nullptr, s.storage, fn(&kotor_fastcall_006cf020), tag); }
        { ExoString s("Add Feat");  list.data[2] = kotor_fastcall_006cea90(self, nullptr, s.storage, fn(&kotor_fastcall_006cf3f0), tag); }
        finish_options(self, list, false);                                                           // 006CF52F..006CF554
        void* box = at(self, 0x64);
        zero(self, 0x48C);                                                                           // 006CF563
        void* control = kotor_fastcall_0041a5a0(box, nullptr, li(self, 0x484));                      // 006CF557..006CF569
        vcall<void, void*, U32>(box, 0x08, control, 0);                                              // 006CF56E..006CF571
        w4_guard_2.armed = false; }
    live_free(list.data);                                                                            // 006CF574..006CF581
}
void* __fastcall kotor_fastcall_006cf5a0(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                       // 006CF5C7
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_pfs_debug());                                                                  // 006CF5D9 (00757A78)
    listbox(self, 0x64, u); label(self, 0x344, u);                                                   // 006CF5DF..006CF5F1
    debug_layout(self);                                                                              // 006CF5F8..006CF676
    U32 build[2]; build_label_text(build);                                                           // 006CF67B..006CF6A8
    u.push(&frame_LogStringDtor_005e5c20, build);
    gui_load_text_params_set_text_00415e00(at(self, 0x42C), nullptr, build);                         // 006CF6AD..006CF6B8
    kotor_fastcall_006cf410(self, nullptr);                                                          // 006CF6BF
    gui_m48_panel_center_0040a600(self, nullptr);                                                    // 006CF6C6
    u.dismiss();
    frame_LogStringDtor_005e5c20(build, nullptr);                                                    // 006CF6CB..006CF6D3
    return self;
}
void __fastcall kotor_fastcall_006cf710(void* self, void*) {
    gui_load_label_destruct_00419d10(at(self, 0x344), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x64), nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006cf6f0(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006cf710, flags); }

// ---------------------------------------------------------------- CSWGuiCreateItemSubMenu (0x4BC, table 00756B30)
void* __fastcall kotor_fastcall_006bdc60(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                       // 006BDC87
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_create_item_sub_menu());                                                       // 006BDC97 (00756B30)
    listbox(self, 0x64, u); label(self, 0x344, u);                                                   // 006BDC9D..006BDCAF
    zero(self, 0x48C); zero(self, 0x490); zero(self, 0x488);                                         // 006BDCB4..006BDCC0
    kotor_sound_internal::StringListConstruct005e9530(at(self, 0x494), nullptr); u.push(&kotor_sound_internal::StringListDestroy005e9860, at(self, 0x494));
    kotor_sound_internal::StringListConstruct005e9530(at(self, 0x4A8), nullptr); u.push(&kotor_sound_internal::StringListDestroy005e9860, at(self, 0x4A8));
    zero(self, 0x484);                                                                               // 006BDCF4
    debug_layout(self);                                                                              // 006BDCE6..006BDD6D
    U32 build[2]; build_label_text(build);                                                           // 006BDD72..006BDD9F
    u.push(&frame_LogStringDtor_005e5c20, build);
    gui_load_text_params_set_text_00415e00(at(self, 0x42C), nullptr, build);                         // 006BDDA4..006BDDAF
    gui_m48_panel_center_0040a600(self, nullptr);                                                    // 006BDDB6
    u.dismiss();
    frame_LogStringDtor_005e5c20(build, nullptr);                                                    // 006BDDBB..006BDDC4
    return self;
}
void __fastcall kotor_fastcall_006bde00(void* self, void*) {
    kotor_sound_internal::StringListDestroy005e9860(at(self, 0x4A8), nullptr);                      // 006BDE1D..006BDE2B
    kotor_sound_internal::StringListDestroy005e9860(at(self, 0x494), nullptr);                      // 006BDE30..006BDE3B
    live_free(lp(self, 0x488));                                                                      // 006BDE40..006BDE4C
    zero(self, 0x488);                                                                               // 006BDE5A
    gui_load_label_destruct_00419d10(at(self, 0x344), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x64), nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006bdde0(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006bde00, flags); }

// ---------------------------------------------------------------- CSWGuiCreateItemMenu (0x488, table 00757B68)
void* __fastcall kotor_fastcall_006d0620(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                       // 006D064B
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_create_item_menu());                                                           // 006D065D (00757B68)
    listbox(self, 0x64, u); label(self, 0x344, u);                                                   // 006D0663..006D0675
    zero(self, 0x484);                                                                               // 006D0688
    debug_layout(self);                                                                              // 006D067A..006D070B
    U32 build[2]; build_label_text(build);                                                           // 006D0710..006D073D
    u.push(&frame_LogStringDtor_005e5c20, build);
    gui_load_text_params_set_text_00415e00(at(self, 0x42C), nullptr, build);                         // 006D0742..006D074D
    PtrList list;                                                                                    // [esp+5C]
    void* items = lp(kotor_cell_rules_007a3a28, 0x34);                                               // 006D0760..006D0766
    const I32 count = li(items, 0);                                                                  // 006D0769
    const U32 tag = lw(self, 0x330);                                                                 // 006D076B
    { auto w4_guard_3 = ::w4gui::on_throw([&]() { live_free(list.data); }); 
        if (count + 1 != 0)                                                                          // 006D0771..006D077F
            menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&list), nullptr, static_cast<unsigned>(count + 1));
        list.size = count + 1;                                                                       // 006D0794
        { ExoString s("Back"); list.data[0] = kotor_fastcall_006d0490(self, nullptr, s.storage, fn(&kotor_fastcall_006d05a0), tag); } // 006D078B..006D07CA
        for (I32 i = 0; i < count; ++i) {                                                            // 006D07CF..006D0942
            U32 name[2]; exo_default(name);                           // 006D07E6..006D07EA
            { auto w4_guard_4 = ::w4gui::on_throw([&]() { frame_LogStringDtor_005e5c20(name, nullptr); }); 
                void* item = kotor_m15b_base_item_array_get_base_item_005b31a0(lp(kotor_cell_rules_007a3a28, 0x34), nullptr, static_cast<U32>(i));
                if (lw(item, 0) != 0) {                                                              // 006D0803..006D0806
                    U32 text[2];
                    kotor_fastcall_005b4690(kotor_m15b_base_item_array_get_base_item_005b31a0(lp(kotor_cell_rules_007a3a28, 0x34), nullptr, static_cast<U32>(i)), nullptr, text);
                    async_assign_string_005e5c50(name, nullptr, text);                               // 006D0823..006D082D
                    frame_LogStringDtor_005e5c20(text, nullptr);                                     // 006D0832..006D08F7
                } else {
                    void* table = live_operator_new(0x54);                                           // 006D083B..006D083D
                    if (table != nullptr) {
                        menu_resources::Ref ref; ref.construct_00406d80("BASEITEMS");                // 006D085F..006D0864
                        Ref16 by_value; std::memcpy(by_value.bytes, ref.bytes, 16);
                        { auto w4_guard_5 = ::w4gui::on_throw([&]() { live_free(table); });  table = c2da_current_construct_00413cc0(table, nullptr, by_value, 0); w4_guard_5.armed = false; }
                    }
                    c2da_current_load_004143b0(table, nullptr);                                      // 006D0876..006D087D
                    U32 label_text[2]; exo_default(label_text);       // 006D0882..006D0886
                    {
                        ExoString column("Label");                                                   // 006D088B..006D0899
                        c2da_current_get_string_00413270(table, nullptr, i, column.storage, label_text); // 006D089E..006D08B3
                    }
                    frame_LogFormat_005e5680(name, "No StrRef: %s", label_text[0], label_text[1]); // 006D08C6..006D08DA (CExoString by value)
                    if (table != nullptr) vcall<void*, U32>(table, 0x00, 1);                         // 006D08E2..006D08EC
                    frame_LogStringDtor_005e5c20(label_text, nullptr);                               // 006D08EE..006D08F7
                }
                list.data[i + 1] = kotor_fastcall_006d0490(self, nullptr, name, fn(&kotor_fastcall_006d05e0), tag); // 006D08FC..006D0921
        w4_guard_4.armed = false; }
            frame_LogStringDtor_005e5c20(name, nullptr);                                             // 006D091D..006D0928
        }
        finish_options(self, list, true);                                                            // 006D0948..006D0971
        gui_m48_panel_center_0040a600(self, nullptr);                                                // 006D0976..006D0978
        w4_guard_3.armed = false; }
    live_free(list.data);                                                                            // 006D097D..006D0987
    u.dismiss();
    frame_LogStringDtor_005e5c20(build, nullptr);                                                    // 006D098F..006D09A0
    return self;
}
void __fastcall kotor_fastcall_006d09e0(void* self, void*) {
    gui_load_label_destruct_00419d10(at(self, 0x344), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x64), nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006d09c0(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006d09e0, flags); }

// ---------------------------------------------------------------- CSWGuiLoadModuleDebugMenu (0x48C, table 00757AE8)
void* __fastcall kotor_fastcall_006cfa70(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                       // 006CFAA1
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_load_module_debug());                                                          // 006CFAB2 (00757AE8)
    listbox(self, 0x64, u); label(self, 0x344, u);                                                   // 006CFAB8..006CFACD
    zero(self, 0x484);                                                                               // 006CFAE3
    debug_layout(self);                                                                              // 006CFAD2..006CFB6B
    U32 build[2]; build_label_text(build);                                                           // 006CFB70..006CFBA9 ([esp+70])
    u.push(&frame_LogStringDtor_005e5c20, build);
    gui_load_text_params_set_text_00415e00(at(self, 0x42C), nullptr, build);                         // 006CFBAE..006CFBB9
    PtrList list;                                                                                    // [esp+1C]
    const U32 tag = lw(self, 0x330);                                                                 // 006CFBCA..006CFBDC [esp+64]
    struct Locals {
        U32 last[2], cur_a[2], cur_b[2], unused[2], alias[2];
        StringList rims, mods;
    } l{};
    exo_default(l.last);   u.push(&frame_LogStringDtor_005e5c20, l.last);    // 006CFBE0 [esp+5C]
    exo_default(l.cur_a);  u.push(&frame_LogStringDtor_005e5c20, l.cur_a);   // 006CFBF1 [esp+28]
    exo_default(l.cur_b);  u.push(&frame_LogStringDtor_005e5c20, l.cur_b);   // 006CFC02 [esp+30]
    exo_default(l.unused); u.push(&frame_LogStringDtor_005e5c20, l.unused);  // 006CFC5A [esp+90]
    struct ListGuard {
        Locals& l; void** data; bool armed = true;
        ~ListGuard() { if (armed) { live_free(*data); string_list_release(l.mods); string_list_release(l.rims); } }
    } lists{l, reinterpret_cast<void**>(&list.data)};
    void* base = kotor_menu_10_19::dependencies::base_007a39e0;
    { ExoString dir("MODULES:"); f200_ports::Enumerate005e6640(base, nullptr, &l.mods, ss(dir.storage), 0x7DB, 0, 1); }  // 006CFC5F..006CFCA6 [esp+50]
    { ExoString dir("MODULES:"); f200_ports::Enumerate005e6640(kotor_menu_10_19::dependencies::base_007a39e0, nullptr, &l.rims, ss(dir.storage), 0xBBA, 0, 1); } // 006CFCAB..006CFCEA [esp+44]
    exo_default(l.alias); u.push(&frame_LogStringDtor_005e5c20, l.alias);    // 006CFCEF [esp+38]
    static const unsigned char rdata_0073ecd2 = 0x07;                                                // .rdata byte 0073ECD2 (image value 07)
    for (U32 n = 1; n < rdata_0073ecd2; ++n) {                                                       // 006CFCF8..006CFDE9
        frame_LogFormat_005e5680(l.alias, "live%d", n);                                              // 006CFD12..006CFD1D
        void* path = kotor_fastcall_005e6890(lp(kotor_menu_10_19::dependencies::base_007a39e0, 0xC), nullptr, l.alias, 0); // 006CFD22..006CFD34
        if (async_string_length_005e5790(path, nullptr) <= 0) continue;                              // 006CFD39..006CFD42
        {
            ExoString suffix(":modules");                                                            // 006CFD48..006CFD51
            U32 joined[2];
            GuiConcat005e5d10(ss(l.alias), nullptr, ss(joined), ss(suffix.storage));                 // 006CFD56..006CFD6C
            async_assign_string_005e5c50(l.alias, nullptr, joined);                                  // 006CFD71..006CFD7E
            frame_LogStringDtor_005e5c20(joined, nullptr);                                           // 006CFD83..006CFD8F
        }
        f200_ports::Enumerate005e6640(kotor_menu_10_19::dependencies::base_007a39e0, nullptr, &l.mods, ss(l.alias), 0x7DB, 0, 1); // 006CFDA5..006CFDBD
        f200_ports::Enumerate005e6640(kotor_menu_10_19::dependencies::base_007a39e0, nullptr, &l.rims, ss(l.alias), 0xBBA, 0, 1); // 006CFDC2..006CFDDA
    }
    const I32 rim_count = l.rims.size, mod_count = l.mods.size;                                      // 006CFDEF..006CFDFB
    I32 i = 0, j = 0;                                                                                // [esp+14] / [esp+18]
    auto is_s_rim = [](U32* s) {
        ExoString pattern("_s.rim");
        return client_movies::StringFind_005e5eb0(reinterpret_cast<options_video::String*>(s), nullptr,
                                                  reinterpret_cast<options_video::String*>(pattern.storage), 0) != -1;
    };
    auto set_text = [](U32* dst, const char* text) { ExoString t(text); async_assign_string_005e5c50(dst, nullptr, t.storage); };
    auto strip_ext = [](U32* s) {
        U32 sub[2];
        startup_rand::SubString005e6270(ss(s), nullptr, ss(sub), 0, async_string_length_005e5790(s, nullptr) - 4);
        async_assign_string_005e5c50(s, nullptr, sub);
        frame_LogStringDtor_005e5c20(sub, nullptr);
    };
    for (;;) {                                                                                       // 006CFE00
        if (i < rim_count) {                                                                         // 006CFE00..006CFE08
            async_assign_string_005e5c50(l.cur_a, nullptr, l.rims.data + i * 8);                     // 006CFE51..006CFE5D
            if (is_s_rim(l.cur_a)) { ++i; continue; }                                                // 006CFE62..006CFEA5
        } else {
            if (j >= mod_count) break;                                                               // 006CFE0A..006CFE12
            set_text(l.cur_a, "zzzzzzzz.zzz");                                                       // 006CFE18..006CFE4A
        }
        if (j < mod_count) {                                                                         // 006CFEAE..006CFEB2
            async_assign_string_005e5c50(l.cur_b, nullptr, l.mods.data + j * 8);                     // 006D0012..006D001E
            if (is_s_rim(l.cur_b)) { ++i; continue; }                                                // 006D0023..006D006A (original increments i)
        } else {
            set_text(l.cur_b, "zzzzzzzz.zzz");                                                       // 006CFEB8..006CFEEA
        }
        strip_ext(l.cur_a);                                                                          // 006CFEEF..006CFF2F
        strip_ext(l.cur_b);                                                                          // 006CFF34..006CFF74
        std::int32_t a_first;
        {
            U32 low_b[2], low_a[2];
            directory_list::LowerCase005e6080(ss(l.cur_b), nullptr, ss(low_b));                     // 006CFF79..006CFF85
            directory_list::LowerCase005e6080(ss(l.cur_a), nullptr, ss(low_a));                     // 006CFF8C..006CFFA0
            a_first = kotor_fastcall_005e5500(low_a, nullptr, low_b);                                // 006CFFA5..006CFFB0
            frame_LogStringDtor_005e5c20(low_a, nullptr);                                            // 006CFFB5..006CFFC6
            frame_LogStringDtor_005e5c20(low_b, nullptr);                                            // 006CFFCB..006CFFDA
        }
        if (a_first != 0) {                                                                          // 006CFFDF..006CFFE1
            async_assign_string_005e5c50(l.last, nullptr, l.cur_a);                                  // 006CFFE7..006CFFF0
            ++i;                                                                                     // 006CFFF5..006D0003
            if (async_string_equal_005e5200(l.cur_a, nullptr, l.cur_b) != 0) ++j;                    // 006CFFF9..006D0010, 006D007D
        } else {
            async_assign_string_005e5c50(l.last, nullptr, l.cur_b);                                  // 006D006F..006D0078
            ++j;                                                                                     // 006D007D
        }
        void* option = kotor_fastcall_006cf890(self, nullptr, l.last, fn(&kotor_fastcall_006cf9d0), tag); // 006D0081..006D0092
        list_add(list, option);                                                                      // 006D0097..006D0109
    }
    finish_options(self, list, true);                                                                // 006D010E..006D0134
    gui_m48_panel_center_0040a600(self, nullptr);                                                    // 006D0139..006D013B
    lists.armed = false;
    u.dismiss();
    frame_LogStringDtor_005e5c20(l.alias, nullptr);                                                  // 006D0140..006D014C
    frame_LogStringDtor_005e5c20(l.unused, nullptr);                                                 // 006D0151..006D0160
    string_list_release(l.mods);                                                                     // 006D0165..006D0198
    string_list_release(l.rims);                                                                     // 006D0192..006D01C7
    frame_LogStringDtor_005e5c20(l.cur_b, nullptr);                                                  // 006D01C3..006D01D3
    frame_LogStringDtor_005e5c20(l.cur_a, nullptr);                                                  // 006D01D8..006D01E4
    frame_LogStringDtor_005e5c20(l.last, nullptr);                                                   // 006D01E9..006D01F5
    live_free(list.data);                                                                            // 006D01FA..006D0207
    frame_LogStringDtor_005e5c20(build, nullptr);                                                    // 006D020F..006D021F
    return self;
}
void __fastcall kotor_fastcall_006d0260(void* self, void*) {
    gui_load_label_destruct_00419d10(at(self, 0x344), nullptr);
    gui_load_listbox_destruct_0041b060(at(self, 0x64), nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_006d0240(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006d0260, flags); }
}  // extern "C"
