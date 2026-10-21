// claude_savegui.cpp (lane worker savegui): the player side of saving a game from the in-game Options -> Save Game
// panel (CSWGuiSaveLoad in save mode), transcribed from the original swkotor.exe (read-only):
//   006CBB60 CSWGuiSaveLoad::HandleSaveButton(CSWGuiControl*)            thiscall RET 4   kotor_fastcall_006cbb60
//   006CB820 CSWGuiSaveLoad::PromptForSaveName(CSWGuiSaveLoadEntry*)     thiscall RET 4   kotor_fastcall_006cb820
//   006C8790 CSWGuiSaveLoad::WriteGame(CExoString const& name)           thiscall RET 4   kotor_fastcall_006c8790
//   006CAE70 CSWGuiSaveNamePanel::CSWGuiSaveNamePanel(CSWGuiManager*)    thiscall RET 4   kotor_fastcall_006cae70
//   006CC060 CSWGuiSaveNamePanel::~CSWGuiSaveNamePanel()                 thiscall RET 0   kotor_fastcall_006cc060
//   006CC040 CSWGuiSaveNamePanel scalar deleting destructor(uint)        thiscall RET 4   kotor_fastcall_006cc040
//   006C8410 CSWGuiSaveNamePanel::OnPanelAdded()                         thiscall RET 0   kotor_fastcall_006c8410
//   006C8460 CSWGuiSaveNamePanel::OnPanelRemoved()                       thiscall RET 0   kotor_fastcall_006c8460
//   006C8470 CSWGuiSaveNamePanel::HandleCancelButton(CSWGuiControl*)     thiscall RET 4   kotor_fastcall_006c8470
//   006C9D50 CSWGuiSaveNamePanel::HandleOkButton(CSWGuiControl*)         thiscall RET 4   kotor_fastcall_006c9d50
//   006CB040 CSWGuiSaveNamePanel::HandleInputEvent(int event, int value) thiscall RET 8   kotor_fastcall_006cb040
//   006C82F0 CSWGuiSaveGameEditBox::HandleKeyPress(int key)              thiscall RET 4   kotor_fastcall_006c82f0
//   006C9DD0 CSWGuiEditbox::~CSWGuiEditbox (jmp 0041C740)               thiscall RET 0   kotor_fastcall_006c9dd0
//   006CB0A0 CSWGuiSaveGameEditBox scalar deleting destructor(uint)      thiscall RET 4   kotor_fastcall_006cb0a0
//   007576D0 CSWGuiSaveNamePanel method table (27 slots)                                 kotor_savegui_methods_007576d0
//   007575B0 CSWGuiSaveGameEditBox method table (40 slots)                               kotor_savegui_editbox_methods_007575b0
//   00833BD0 CSWGuiSaveGameEditBox::HandleKeyPress 2-byte key buffer (.bss, used only there) kotor_savegui_key_00833bd0
// The 0796 SaveLoadHandlers::HandleSaveButton_006cbb60 member (registered by the CSWGuiSaveLoad constructor 006CC680 and
// PopulateGameList 006CC160 in save mode) forwards here. WriteGame sends CClientExoApp::SendSaveGameRequest 005ED670
// (camera_nested_providers -> CSWCMessage::SendPlayerToServerModule_SaveGame 006774E0, message 3/4) which the server
// handles in CSWSMessage::HandlePlayerToServerModuleMessage 00524800 sub 4 (w2_entry) -> SetSaveGameName ->
// CServerExoAppInternal::SaveGame 004B58A0 (claude_closeB_save; screenshot via StallEventSaveGame/DoSaveGameScreenShot).
// Conventions (W4): thiscall -> extern "C" __fastcall(self, unused_edx, stack args...); game heap only. No callee on
// these paths throws (operator new 006FA7E6 returns NULL), so the original EH states reduce to destructor order.
// Proven constants: [007439BC] = 358637BDh (1.0e-6f, the MB scale of the deficit message), event 27h = activate.
// Quirks kept: PromptForSaveName writes [panel+64h] through a NULL panel when the 690h allocation fails; the overwrite
// branch of HandleSaveButton tests (free/16K + need) >= need, i.e. it only reports "not enough space" on 64-bit wrap,
// and then stores need - (free/16K + need) (low dword) as the deficit; SaveNamePanel::HandleInputEvent never forwards to
// CSWGuiPanel::HandleInputEvent and ignores the event value; WriteGame uses the selected entry without a NULL check;
// ~CSWGuiSaveNamePanel does not restore the method table before destroying members (as the original).
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
using U8 = std::uint8_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
template <class T> T& at(void* object, U offset) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset); }
inline U8* bytes(void* p) { return static_cast<U8*>(p); }
inline float flt(U u) { float f; std::memcpy(&f, &u, 4); return f; }
constexpr U k_mb_scale_007439bc = 0x358637bdu;                                                  // 1.0e-6f
}

namespace kotor_reconstruction { struct StartupString; }
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }                    // [007A39FC] CAppManager*
namespace kotor_input_cells { extern void* volatile gui_manager_007a39f4; }                     // [007A39F4] CSWGuiManager*
namespace frame_control { extern unsigned resource_manager_007a39e8; }                          // [007A39E8] CExoResMan*
namespace kotor_menu_10_19 { struct String; struct Tlk { String* simple_0041e8f0(String* out, unsigned id); }; } // GetSimpleString [0037]
namespace tlk_core { void __fastcall ParseStr0041dd30(kotor_menu_10_19::Tlk*, void*, kotor_reconstruction::StartupString*); } // [0389]
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); } // CExoString() [menu20]
namespace tlk_file { struct File; }
namespace file_internal {
tlk_file::File* __fastcall exofile_construct_005e68d0(tlk_file::File*, void*, kotor_reconstruction::StartupString*, unsigned,
                                                     kotor_reconstruction::StartupString*);    // CExoFile::CExoFile [0230]
}
#ifndef W1R_MENU_RESOURCES_REF
#define W1R_MENU_RESOURCES_REF
namespace menu_resources { struct Ref { unsigned char bytes[16]; Ref* __thiscall construct_00406d80(const char* text); }; } // [0030]
#endif
namespace menu_resources { struct Manager { unsigned directory_00408800(kotor_reconstruction::StartupString* path); }; } // AddResourceDirectory [0027]
// Method-table members that the rebuild provides as C++ members (single inheritance -> 4-byte PMF entries).
namespace kotor_input_hover {
struct Control { void SetActive(int); Control* HitCheckMouse(int, int); };                     // [0391]
struct Navigable { void HandleMouseOver(int); void HandleInputEvent(int, int); };              // [0391]
}
namespace kotor_m1_saveload { struct RadioButtonLeaves { void HandleLMouseDown_00418680(); }; } // [0796]

extern "C" {
// ---------------- existing providers (c41 map, exact decorated arity) ----------------
void* __cdecl live_operator_new(unsigned);                                                     // 006FA7E6
void __cdecl live_free(void*);                                                                 // 006FA390
extern void* kotor_cell_tlk_007a3a08;                                                          // [007A3A08] CTlkTable*
void* __fastcall frame_LogStringCtor_005e5a90(void* s, void*, const char* text);              // CExoString(char*) [0459]
void __fastcall frame_LogStringDtor_005e5c20(void* s, void*);                                 // ~CExoString [0459]
void* __fastcall async_assign_string_005e5c50(void* self, void*, void* other);                // operator= [0199]
void* __fastcall WindowCopy005e5b00(void* self, void*, void* other);                          // CExoString(CExoString const&) [0078]
void __cdecl frame_LogFormat_005e5680(void* self, const char* format, ...);                   // CExoString::Format [0125]
U __fastcall file_opened_005e6a10(void* file, void*);                                         // CExoFile::FileOpened [0224]
U __fastcall exofile_size_005e6950(void* file, void*);                                        // CExoFile::GetSize [0232]
void __fastcall file_destroy_005e69e0(void* file, void*);                                     // ~CExoFile [0224]
U __fastcall gui_load_resman_free_disk_space_004067b0(void* resman, void*, void* path, std::uint64_t* out); // GetFreeDiskSpace [0395]
void* __fastcall kotor_m1_listbox_get_selected_control_0041a5d0(void* list, void*);           // CSWGuiListBox::GetSelectedControl [0791]
void* __fastcall frame_R4_005ed690(void* client, void*);                                      // CClientExoApp::GetInGameGui [0457]
void __fastcall kotor_fastcall_0041db50(void* tlk, void*, U token, void* text);               // CTlkTable::SetCustomToken [0583]
void __fastcall kotor_fastcall_00627130(void* box, void*, U allow);                           // CSWGuiMessageBox::SetAllowCancel [0596]
void __fastcall gui_load_messagebox_set_callback_00624a40(void* box, void*, U target, U handler, U param); // SetCallback [0298]
void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, U a, U b);         // CSWGuiManager::AddPanel [0298]
void* __fastcall kotor_fastcall_0040be00(void* manager, void*);                               // CSWGuiManager::PopModalPanel [0458]
U __fastcall kotor_fastcall_005ed670(void* client, void*, U number, void* dir, void* name);   // CClientExoApp::SendSaveGameRequest [nested]
void* __fastcall gui_load_panel_construct_0040b570(void* self, void*, void* manager);         // CSWGuiPanel::CSWGuiPanel [0296]
void* __fastcall gui_load_button_construct_0041c0f0(void* self, void*);                       // CSWGuiButton::CSWGuiButton [0296]
void* __fastcall kotor_m15c_editbox_construct_0041c6b0(void* self, void*);                    // CSWGuiEditbox::CSWGuiEditbox [0823]
void* __fastcall gui_load_label_construct_0041acd0(void* self, void*);                        // CSWGuiLabel::CSWGuiLabel [0296]
void __fastcall gui_load_panel_start_layout_0040a680(void* panel, void*, void* ref);          // CSWGuiPanel::StartLoadFromLayout [0293]
void __fastcall gui_load_panel_init_control_0040b930(void* panel, void*, void* control, void* tag, I flag); // InitControl [0293]
void __fastcall gui_load_panel_stop_layout_0040b8f0(void* panel, void*);                      // StopLoadFromLayout [0293]
void __fastcall gui_m48_panel_center_0040a600(void* panel, void*);                            // CSWGuiPanel::CenterPanel [0699]
void __fastcall gui_load_control_add_event_0041ab20(void* control, void*, I id, void* object, U func); // CSWGuiControl::AddEvent [0296]
void __fastcall gui_load_label_destruct_00419d10(void* self, void*);                          // ~CSWGuiLabel [0296]
void __fastcall kotor_m15c_editbox_destruct_0041c740(void* self, void*);                      // ~CSWGuiEditbox [0823]
void __fastcall gui_load_button_destruct_00419da0(void* self, void*);                         // ~CSWGuiButton [0296]
void __fastcall gui_load_panel_destruct_0040cf70(void* self, void*);                          // ~CSWGuiPanel [0296]
void __fastcall kotor_m15c_editbox_set_focus_00418660(void* self, void*);                     // CSWGuiEditbox::SetFocus [0823]
void __fastcall gui_load_panel_on_added_0040b870(void* self, void*);                          // CSWGuiPanel::OnPanelAdded [0296]
void __fastcall gui_m48_manager_store_mouse_0040a3d0(void* manager, void*);                   // StoreCurrentMousePosition [0699]
void __fastcall gui_m48_manager_move_mouse_0040c790(void* manager, void*, I x, I y);          // MoveMouseToPosition [0699]
void __fastcall gui_load_panel_on_removed_0040c170(void* self, void*);                        // CSWGuiPanel::OnPanelRemoved [0600]
void __fastcall gui_m48_manager_restore_mouse_0040c7d0(void* manager, void*);                 // RestoreOldMousePosition [0699]
void __fastcall kotor_m15c_edit_text_remove_last_char_004162e0(void* self, void*);            // CSWGuiEditText::RemoveLastChar [0823]
void __fastcall kotor_m15c_edit_text_add_new_char_004163a0(void* self, void*, const char* text); // CSWGuiEditText::AddNewChar [0823]
void __fastcall kotor_m15c_edit_text_set_text_00416490(void* self, void*, void* text);        // CSWGuiEditText::SetText [0823]
// ---- method-table slot providers (bound by the symbol the c41 image stores in 00757650 / 0073EAC8)
void __fastcall gui_load_panel_set_extent_0040a5a0(void*, void*, void*);                      // CSWGuiPanel::SetExtent
void __fastcall gui_load_panel_set_active_control_0040a630(void*, void*, void*, U);           // CSWGuiPanel::SetActiveControl
void __fastcall gui_load_slot_ret4_0060e760(void*, void*, U);                                 // void_return (RET 4)
U __fastcall gui_load_slot_zero8_00409eb0(void*, void*, U, U);                                // return_zero_0x8
void __fastcall gui_load_slot_ret_004015a0(void*, void*);                                     // void_return
void* __fastcall gui_load_slot_self_00641db0(void*, void*);                                   // self_return
U __fastcall gui_load_slot_zero_0063e7f0(void*, void*);                                       // return_zero
void __fastcall kotor_panel_draw_0040B760(void*, void*, U);                                   // CSWGuiPanel::Draw
void* __fastcall gui_load_panel_hit_check_0040b690(void*, void*, I, I);                       // CSWGuiPanel::HitCheckMouse
void* __fastcall gui_load_panel_active_control_0040b630(void*, void*);                        // CSWGuiPanel::GetActiveControl
void __fastcall gui_load_panel_button_a_0040b640(void*, void*, U);                            // OnAButtonPressed
void __fastcall gui_load_panel_button_b_0040b650(void*, void*, U);                            // OnBButtonPressed
void __fastcall gui_load_panel_button_x_0040b660(void*, void*, U);                            // OnXButtonPressed
void __fastcall gui_load_panel_button_y_0040b670(void*, void*, U);                            // OnYButtonPressed
void __fastcall gui_load_panel_button_black_0040b680(void*, void*, U);                        // OnBlackButtonPressed
void* __fastcall gui_load_panel_full_screen_bg_0040a900(void*, void*, void*);                 // GetFullScreenBG
void __fastcall gui_load_panel_reset_font_0040a9c0(void*, void*);                             // CSWGuiPanel::ResetFont
void __fastcall kotor_m15c_editbox_set_extent_00418450(void*, void*, void*);                  // CSWGuiEditbox::SetExtent
void __fastcall light_manager_void_return_8_005b5e90(void*, void*, U, U);                     // void_return (RET 8)
void __fastcall kotor_m15c_editbox_handle_lmouse_up_004186b0(void*, void*);                   // CSWGuiEditbox::HandleLMouseUp
void __fastcall gui_load_control_handle_rmouse_up_0041ac70(void*, void*);                     // CSWGuiControl::HandleRMouseUp
U __fastcall gui_scene_get_property_0x10_004064a0(void*, void*);                              // GetProperty0x10
void __fastcall kotor_m15c_editbox_draw_00418420(void*, void*, U);                            // CSWGuiEditbox::Draw
void __fastcall kotor_m15c_editbox_load_0041c7d0(void*, void*, void*, void*);                 // CSWGuiEditbox::Load
void __fastcall kotor_m15c_editbox_focus_change_0041a820(void*, void*, U);                    // HandleFocusChange
U __fastcall kotor_m15c_editbox_is_selectable_00418670(void*, void*);                         // GetIsSelectable
void __fastcall gui_load_control_rmouse_down_0041ac30(void*, void*);                          // CSWGuiControl::HandleRMouseDown
void __fastcall kotor_m15c_editbox_set_enabled_004184b0(void*, void*, U);                     // CSWGuiEditbox::SetEnabled
void __fastcall kotor_m15c_editbox_reset_font_004186d0(void*, void*);                         // CSWGuiEditbox::ReSetFont
void __fastcall gui_load_control_tooltip_00418a90(void*, void*);                              // CSWGuiControl::DisplayToolTip
void __fastcall kotor_m15c_editbox_initialize_004183b0(void*, void*, void*, U, U);            // CSWGuiEditbox::Initialize

// ---------------- bodies defined here ----------------
void __fastcall kotor_fastcall_006cbb60(void* self, void*, void* control);
void __fastcall kotor_fastcall_006cb820(void* self, void*, void* entry);
void __fastcall kotor_fastcall_006c8790(void* self, void*, void* name);
void* __fastcall kotor_fastcall_006cae70(void* self, void*, void* manager);
void __fastcall kotor_fastcall_006cc060(void* self, void*);
void* __fastcall kotor_fastcall_006cc040(void* self, void*, U flags);
void __fastcall kotor_fastcall_006c8410(void* self, void*);
void __fastcall kotor_fastcall_006c8460(void* self, void*);
void __fastcall kotor_fastcall_006c8470(void* self, void*, void* control);
void __fastcall kotor_fastcall_006c9d50(void* self, void*, void* control);
void __fastcall kotor_fastcall_006cb040(void* self, void*, I event, I value);
void __fastcall kotor_fastcall_006c82f0(void* self, void*, I key);
void __fastcall kotor_fastcall_006c9dd0(void* self, void*);
void* __fastcall kotor_fastcall_006cb0a0(void* self, void*, U flags);
char kotor_savegui_key_00833bd0[2];                                                            // [00833BD0] (.bss)
}

// ===================================================================================== method tables
// 007576D0 = CSWGuiPanel table 0073E010 except +00 006CC040, +3C 006CB040, +48 006C8410, +4C 006C8460.
#define SAVEGUI_PANEL_SLOTS(X)                                                                              \
    X(s00, &kotor_fastcall_006cc040) X(s04, &gui_load_panel_set_extent_0040a5a0)                            \
    X(s08, &gui_load_panel_set_active_control_0040a630) X(s0c, &gui_load_slot_ret4_0060e760)                \
    X(s10, &gui_load_slot_zero8_00409eb0) X(s14, &gui_load_slot_ret_004015a0) X(s18, &gui_load_slot_ret_004015a0) \
    X(s1c, &gui_load_slot_ret_004015a0) X(s20, &gui_load_slot_ret_004015a0) X(s24, &gui_load_slot_ret_004015a0) \
    X(s28, &gui_load_slot_self_00641db0) X(s2c, &gui_load_slot_zero_0063e7f0) X(s30, &gui_load_slot_zero_0063e7f0) \
    X(s34, &kotor_panel_draw_0040B760) X(s38, &gui_load_slot_ret4_0060e760) X(s3c, &kotor_fastcall_006cb040)  \
    X(s40, &gui_load_panel_hit_check_0040b690) X(s44, &gui_load_panel_active_control_0040b630)              \
    X(s48, &kotor_fastcall_006c8410) X(s4c, &kotor_fastcall_006c8460) X(s50, &gui_load_panel_button_a_0040b640) \
    X(s54, &gui_load_panel_button_b_0040b650) X(s58, &gui_load_panel_button_x_0040b660)                     \
    X(s5c, &gui_load_panel_button_y_0040b670) X(s60, &gui_load_panel_button_black_0040b680)                 \
    X(s64, &gui_load_panel_full_screen_bg_0040a900) X(s68, &gui_load_panel_reset_font_0040a9c0)
// 007575B0 = CSWGuiEditbox table 0073EAC8 except +00 006CB0A0, +9C 006C82F0 (checked slot by slot against the image).
#define SAVEGUI_EDITBOX_SLOTS(X)                                                                            \
    X(s00, &kotor_fastcall_006cb0a0) X(s04, &kotor_m15c_editbox_set_extent_00418450)                        \
    X(s08, &light_manager_void_return_8_005b5e90) X(s0c, &kotor_input_hover::Navigable::HandleMouseOver)     \
    X(s10, &gui_load_slot_zero8_00409eb0) X(s14, &gui_load_slot_ret_004015a0)                               \
    X(s18, &kotor_m1_saveload::RadioButtonLeaves::HandleLMouseDown_00418680)                                \
    X(s1c, &kotor_m15c_editbox_handle_lmouse_up_004186b0) X(s20, &gui_load_slot_ret_004015a0)               \
    X(s24, &gui_load_control_handle_rmouse_up_0041ac70) X(s28, &gui_load_slot_zero_0063e7f0)                \
    X(s2c, &gui_load_slot_zero_0063e7f0) X(s30, &gui_load_slot_self_00641db0)                               \
    X(s34, &gui_scene_get_property_0x10_004064a0) X(s38, &kotor_m15c_editbox_draw_00418420)                 \
    X(s3c, &kotor_input_hover::Navigable::HandleInputEvent) X(s40, &kotor_input_hover::Control::SetActive)  \
    X(s44, &kotor_input_hover::Control::HitCheckMouse) X(s48, &kotor_m15c_editbox_load_0041c7d0)            \
    X(s4c, &gui_load_slot_self_00641db0) X(s50, &gui_load_slot_zero_0063e7f0) X(s54, &gui_load_slot_zero_0063e7f0) \
    X(s58, &gui_load_slot_zero_0063e7f0) X(s5c, &gui_load_slot_zero_0063e7f0) X(s60, &gui_load_slot_zero_0063e7f0) \
    X(s64, &gui_load_slot_zero_0063e7f0) X(s68, &gui_load_slot_zero_0063e7f0) X(s6c, &gui_load_slot_zero_0063e7f0) \
    X(s70, &gui_load_slot_zero_0063e7f0) X(s74, &gui_load_slot_zero_0063e7f0) X(s78, &gui_load_slot_self_00641db0) \
    X(s7c, &kotor_m15c_editbox_focus_change_0041a820) X(s80, &kotor_m15c_editbox_is_selectable_00418670)    \
    X(s84, &gui_load_control_rmouse_down_0041ac30) X(s88, &kotor_m15c_editbox_set_enabled_004184b0)         \
    X(s8c, &kotor_m15c_editbox_reset_font_004186d0) X(s90, &gui_load_control_tooltip_00418a90)              \
    X(s94, &light_manager_void_return_8_005b5e90) X(s98, &kotor_m15c_editbox_initialize_004183b0)           \
    X(s9c, &kotor_fastcall_006c82f0)
#define SAVEGUI_FIELD(name, fn) decltype(fn) name;
#define SAVEGUI_VALUE(name, fn) fn,
struct SaveGuiPanelMethods { SAVEGUI_PANEL_SLOTS(SAVEGUI_FIELD) };
struct SaveGuiEditboxMethods { SAVEGUI_EDITBOX_SLOTS(SAVEGUI_FIELD) };
static_assert(sizeof(SaveGuiPanelMethods) == 27 * 4, "007576D0 slot count");
static_assert(sizeof(SaveGuiEditboxMethods) == 40 * 4, "007575B0 slot count");
extern "C" const SaveGuiPanelMethods kotor_savegui_methods_007576d0 = { SAVEGUI_PANEL_SLOTS(SAVEGUI_VALUE) };
extern "C" const SaveGuiEditboxMethods kotor_savegui_editbox_methods_007575b0 = { SAVEGUI_EDITBOX_SLOTS(SAVEGUI_VALUE) };
#undef SAVEGUI_VALUE
#undef SAVEGUI_FIELD
#undef SAVEGUI_EDITBOX_SLOTS
#undef SAVEGUI_PANEL_SLOTS

namespace {
using Str = kotor_reconstruction::StartupString;
inline Str* str(void* p) { return static_cast<Str*>(p); }
// One CExoString destroyed when its scope ends (the original EH states only destroy CExoString locals).
struct Scoped {
    alignas(4) unsigned char b[8]; bool live = false;
    void* p() { return b; }
    void* text(const char* t) { (void)frame_LogStringCtor_005e5a90(b, nullptr, t); live = true; return b; }
    void* empty() { (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(b), nullptr); live = true; return b; }
    void* copy(void* o) { (void)WindowCopy005e5b00(b, nullptr, o); live = true; return b; }
    void* adopt() { live = true; return b; }                                                 // constructed by a callee
    void end() { if (live) { live = false; frame_LogStringDtor_005e5c20(b, nullptr); } }
    ~Scoped() { end(); }
};
inline void* app() { return const_cast<void*>(kotor_menu_window_cells::manager_007a39fc); }
inline void* client() { return at<void*>(app(), 4); }
inline void* resman() { return ptr(frame_control::resource_manager_007a39e8); }
inline void* gui_manager() { return const_cast<void*>(kotor_input_cells::gui_manager_007a39f4); }
inline kotor_menu_10_19::Tlk* tlk() { return static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08); }
inline void s_assign(void* s, void* o) { (void)async_assign_string_005e5c50(s, nullptr, o); }
inline void* in_game_message_box() { return at<void*>(frame_R4_005ed690(client(), nullptr), 0x98); } // GetInGameGui()->+98h
inline void* games(void* save_load) { return bytes(save_load) + 0x934; }                    // CSWGuiSaveLoad listbox
inline void pop_with_flags(void* panel) {                                                     // [+44h] &= ~300h |= 400h; PopModalPanel
    at<U>(panel, 0x44) = (at<U>(panel, 0x44) & 0xfffffcffu) | 0x400u;
    (void)kotor_fastcall_0040be00(at<void*>(panel, 0x18), nullptr);
}
// CSWGuiMessageBox vslot +70h SetText(CExoString) by value: the caller builds the copy, the callee destroys it.
inline void box_set_text(void* box, void* text) {
    alignas(4) unsigned char by_value[8];
    (void)WindowCopy005e5b00(by_value, nullptr, text);
    U w0, w1; std::memcpy(&w0, by_value, 4); std::memcpy(&w1, by_value + 4, 4);
    using SetText = void(__thiscall*)(void*, U, U);
    reinterpret_cast<SetText>(at<void*>(at<void*>(box, 0), 0x70))(box, w0, w1);
}
inline void box_set_message_strref(void* box, U strref) {                                   // vslot +6Ch
    using SetMessage = void(__thiscall*)(void*, U);
    reinterpret_cast<SetMessage>(at<void*>(at<void*>(box, 0), 0x6c))(box, strref);
}
inline U fn(void (__fastcall* f)(void*, void*, void*)) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(f)); }

// 006CBCE0..006CBE1D / 006CBEDB..006CC018: not enough disk space. deficit = blocks of 16 KB still needed.
void report_disk_space(void* self, void* entry, U deficit) {
    void* const box = in_game_message_box();                                                  // edi
    at<U>(entry, 0x1d0) = deficit;                                                            // [entry+1D0h]
    const float megabytes = static_cast<float>(static_cast<double>(static_cast<I>(deficit << 14)) *
                                               static_cast<double>(flt(k_mb_scale_007439bc))); // fild / fmul dword / fstp dword
    Scoped message; message.empty();                                                          // state 7
    Scoped number; number.empty();                                                            // state 8
    frame_LogFormat_005e5680(number.p(), "%2.3f", static_cast<double>(megabytes));            // [00752FDC]
    kotor_fastcall_0041db50(tlk(), nullptr, 0, number.p());                                   // SetCustomToken(0, number)
    { Scoped text;
      (void)tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(text.p()), 0xbb75); text.adopt();
      s_assign(message.p(), text.p()); }                                                      // message = GetSimpleString(BB75h)
    tlk_core::ParseStr0041dd30(tlk(), nullptr, str(message.p()));
    kotor_fastcall_00627130(box, nullptr, 0);                                                 // SetAllowCancel(0)
    box_set_text(box, message.p());                                                           // vslot +70h
    gui_load_messagebox_set_callback_00624a40(box, nullptr, word(self), 0, 0);                // SetCallback(this, 0, 0)
    kotor_fastcall_0040bc70(at<void*>(self, 0x18), nullptr, box, 1, 1);                       // AddPanel(box, 1, 1)
    number.end();
    message.end();
}
}  // namespace

extern "C" {
// ===================================================================================== 006CBB60
// self = CSWGuiSaveLoad (+18h manager, +934h games listbox); entry = CSWGuiSaveLoadEntry (+1C4h bit0 existing save,
// +1D0h space deficit).
void __fastcall kotor_fastcall_006cbb60(void* self, void*, void* control) {
    if (at<U>(control, 0x4c) == 0) return;                                                    // 006CBB7E..BB8D
    U extra = 0;                                                                              // esi
    { Scoped options; options.text("OPTIONS:");                                               // 006CBB93..BB9C [007577EC]
      (void)static_cast<menu_resources::Manager*>(resman())->directory_00408800(str(options.p())); } // 006CBBA1..BBC0
    {
        Scoped mode; mode.text("r");                                                          // 006CBBC5..BBCE [00740CA4]
        Scoped path; path.text("OPTIONS:OPT");                                                // 006CBBD3..BBE4 [007577E0]
        alignas(4) unsigned char file[4];                                                     // [esp+0Ch] CExoFile
        (void)file_internal::exofile_construct_005e68d0(reinterpret_cast<tlk_file::File*>(file), nullptr,
                                                        str(path.p()), 0, str(mode.p()));     // 006CBBE9..BBFD
        path.end();                                                                           // 006CBC02..BC0B
        mode.end();                                                                           // 006CBC10..BC19
        if (file_opened_005e6a10(file, nullptr) == 0 || exofile_size_005e6950(file, nullptr) == 0) extra = 2; // 006CBC1E..BC38
        file_destroy_005e69e0(file, nullptr);                                                 // 006CBC3D..BC45
    }
    void* const entry = kotor_m1_listbox_get_selected_control_0041a5d0(games(self), nullptr); // 006CBC4A..BC50
    if (!entry) return;                                                                       // 006CBC55..BC5B
    const U need = extra + 0x640;                                                             // lea eax,[esi+640h]
    const std::int64_t need64 = static_cast<I>(need);                                         // cdq
    std::uint64_t free_bytes = 0;
    if ((at<U8>(entry, 0x1c4) & 1u) != 0) {                                                   // 006CBC61..BC6D overwrite
        { Scoped saves; saves.text("SAVES:");                                                 // 006CBC68..BC77 [00745174]
          (void)gui_load_resman_free_disk_space_004067b0(resman(), nullptr, saves.p(), &free_bytes); } // 006CBC7C..BCA1
        const std::int64_t total = static_cast<std::int64_t>(free_bytes >> 14) + need64;      // 006CBCA6..BCC6 shrd/shr/add/adc
        if (total >= need64) {                                                                // 006CBCC8..BCDA (jg / jl / jae)
            void* const box = in_game_message_box();                                          // 006CBE20..BE2E
            kotor_fastcall_00627130(box, nullptr, 1);                                         // 006CBE34..BE38 SetAllowCancel(1)
            box_set_message_strref(box, 0x637);                                               // 006CBE3D..BE46 vslot +6Ch
            gui_load_messagebox_set_callback_00624a40(box, nullptr, word(self), fn(&kotor_fastcall_006cb820),
                                                      word(entry));                           // 006CBE49..BE56
            kotor_fastcall_0040bc70(at<void*>(self, 0x18), nullptr, box, 1, 1);               // 006CBE5B..BE63
            return;
        }
        report_disk_space(self, entry, extra - static_cast<U>(total) + 0x640u);               // 006CBCE0..BE1D
        return;
    }
    { Scoped saves; saves.text("SAVES:");                                                     // 006CBE7C..BE80 [00745174]
      (void)gui_load_resman_free_disk_space_004067b0(resman(), nullptr, saves.p(), &free_bytes); } // 006CBE85..BEAA
    const std::uint64_t blocks = free_bytes >> 14;                                            // 006CBEAF..BEC2
    if (static_cast<std::int64_t>(blocks) >= need64) {                                        // 006CBEC5..BED5
        kotor_fastcall_006cb820(self, nullptr, entry);                                        // 006CC01B..C022
        return;
    }
    report_disk_space(self, entry, extra - static_cast<U>(blocks) + 0x640u);                  // 006CBEDB..C018
}

// ===================================================================================== 006CB820
// Message-box callback (overwrite confirm) and direct call: opens the CSWGuiSaveNamePanel with the entry's name.
void __fastcall kotor_fastcall_006cb820(void* self, void*, void* entry) {
    Scoped name; name.empty();                                                                // 006CB83C..B840 [esp+8]
    if (!entry) return;                                                                       // 006CB845..B853
    if ((at<U8>(entry, 0x1c4) & 1u) != 0) {                                                  // 006CB859..B860
        void* const box = in_game_message_box();                                              // 006CB862..B86F
        if ((at<U8>(box, 0x64) & 1u) == 0) return;                                            // 006CB875..B87D not confirmed
        s_assign(name.p(), bytes(entry) + 0x1d8);                                             // 006CB883..B88A entry save name
    } else {
        Scoped empty; empty.text("");                                                         // 006CB891..B89A [0073D71C]
        s_assign(name.p(), empty.p());                                                        // 006CB89F..B8A9
    }                                                                                         // 006CB8AE..B8B7
    void* const memory = live_operator_new(0x690);                                            // 006CB8BC..B8C6
    void* const panel = memory ? kotor_fastcall_006cae70(memory, nullptr, at<void*>(self, 0x18)) : nullptr; // 006CB8CD..B8E5
    at<void*>(panel, 0x64) = self;                                                            // 006CB8F7 (NULL panel faults, as the original)
    kotor_m15c_edit_text_set_text_00416490(bytes(panel) + 0x4d0, nullptr, name.p());          // 006CB8E7..B8FA editbox text
    kotor_fastcall_0040bc70(gui_manager(), nullptr, panel, 1, 1);                             // 006CB8FF..B90A AddPanel(panel, 1, 1)
}

// ===================================================================================== 006C8790
// self = CSWGuiSaveLoad (+68h next free game number); entry +1C8h game number (> 999 = the "new save" slot).
void __fastcall kotor_fastcall_006c8790(void* self, void*, void* name) {
    void* const entry = kotor_m1_listbox_get_selected_control_0041a5d0(games(self), nullptr); // 006C87AC..B7B7
    if (at<U>(entry, 0x1c8) > 0x3e7u) {                                                       // 006C87B9..B7C4
        const U next = at<U>(self, 0x68);
        if (next > 0x3e7u) {                                                                  // 006C87CA..B7D2 too many saves
            void* const box = in_game_message_box();                                          // 006C87D4..B7E1
            Scoped text;
            (void)tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(text.p()), 0xa5fa); text.adopt(); // 006C87E7..B7F7
            tlk_core::ParseStr0041dd30(tlk(), nullptr, str(text.p()));                       // 006C87FC..B80F
            kotor_fastcall_00627130(box, nullptr, 0);                                         // 006C8814..B818
            box_set_text(box, text.p());                                                      // 006C881D..B834 vslot +70h
            kotor_fastcall_0040bc70(at<void*>(self, 0x18), nullptr, box, 1, 1);               // 006C8837..B83F
            text.end();                                                                       // 006C8844 / 006C88DB..B8E3
            return;
        }
        at<U>(entry, 0x1c8) = next;                                                           // 006C884D
    }
    if ((at<U8>(entry, 0x1c4) & 1u) != 0) {                                                   // 006C8853..B85A
        void* const box = in_game_message_box();                                              // 006C885C..B869
        if ((at<U8>(box, 0x64) & 1u) == 0) return;                                            // 006C886F..B873
    }
    Scoped dir; dir.empty();                                                                  // 006C8875..B879
    frame_LogFormat_005e5680(dir.p(), "Game%d", static_cast<I>(at<U>(entry, 0x1c8) - 1u));  // 006C887E..B898 [00757574]
    (void)kotor_fastcall_005ed670(client(), nullptr, at<U>(entry, 0x1c8), dir.p(), name);    // 006C889D..B8BA SendSaveGameRequest
    pop_with_flags(self);                                                                     // 006C88BF..B8D2
    dir.end();                                                                                // 006C88D7..B8E3
}

// ===================================================================================== CSWGuiSaveNamePanel
// Layout (690h): CSWGuiPanel; +64h owning CSWGuiSaveLoad; +68h BTN_OK; +22Ch BTN_CANCEL; +3F0h EDITBOX
// (CSWGuiSaveGameEditBox, text +4D0h, value +548h, max length word +540h); +550h LBL_TITLE.
void* __fastcall kotor_fastcall_006cae70(void* self, void*, void* manager) {
    (void)gui_load_panel_construct_0040b570(self, nullptr, manager);                         // 006CAE92..AE97
    void* const ok = bytes(self) + 0x68;                                                      // edi
    void* const cancel = bytes(self) + 0x22c;                                                 // ebp
    void* const edit = bytes(self) + 0x3f0;                                                   // ebx
    void* const title = bytes(self) + 0x550;
    { const void* t = &kotor_savegui_methods_007576d0; std::memcpy(self, &t, 4); }           // 006CAEA9
    (void)gui_load_button_construct_0041c0f0(ok, nullptr);                                    // 006CAEAF
    (void)gui_load_button_construct_0041c0f0(cancel, nullptr);                                // 006CAEC1
    (void)kotor_m15c_editbox_construct_0041c6b0(edit, nullptr);                               // 006CAED3
    { const void* t = &kotor_savegui_editbox_methods_007575b0; std::memcpy(edit, &t, 4); }   // 006CAED8
    (void)gui_load_label_construct_0041acd0(title, nullptr);                                  // 006CAEE9
    at<void*>(self, 0x64) = nullptr;                                                          // 006CAEFC
    { menu_resources::Ref layout; (void)layout.construct_00406d80("savename");               // 006CAEEE..AF03 [007576C4]
      gui_load_panel_start_layout_0040a680(self, nullptr, &layout); }                         // 006CAF08..AF0F
    { Scoped tag; tag.text("BTN_OK");                                                         // 006CAF14..AF1D [0074FD9C]
      gui_load_panel_init_control_0040b930(self, nullptr, ok, tag.p(), 1); }                  // 006CAF22..AF3F
    { Scoped tag; tag.text("BTN_CANCEL");                                                     // 006CAF44..AF4D [0074FD90]
      gui_load_panel_init_control_0040b930(self, nullptr, cancel, tag.p(), 1); }              // 006CAF52..AF6F
    { Scoped tag; tag.text("EDITBOX");                                                        // 006CAF74..AF7D [007576BC]
      gui_load_panel_init_control_0040b930(self, nullptr, edit, tag.p(), 1); }                // 006CAF82..AF9F
    { Scoped tag; tag.text("LBL_TITLE");                                                      // 006CAFA4..AFAD [00751A20]
      gui_load_panel_init_control_0040b930(self, nullptr, title, tag.p(), 1); }               // 006CAFB2..AFD5
    at<U>(ok, 0x44) &= 0xfffffffbu;                                                           // 006CAFDA..AFE4
    at<U>(cancel, 0x44) &= 0xfffffffbu;                                                       // 006CAFE7..AFEE
    gui_load_panel_stop_layout_0040b8f0(self, nullptr);                                       // 006CAFF1
    gui_m48_panel_center_0040a600(self, nullptr);                                             // 006CAFF8
    gui_load_control_add_event_0041ab20(ok, nullptr, 0x27, self, fn(&kotor_fastcall_006c9d50));      // 006CAFFD..B007
    gui_load_control_add_event_0041ab20(cancel, nullptr, 0x27, self, fn(&kotor_fastcall_006c8470));  // 006CB00C..B016
    at<std::uint16_t>(self, 0x540) = 0x10;                                                    // 006CB020 editbox max length
    return self;
}
void __fastcall kotor_fastcall_006cc060(void* self, void*) {
    gui_load_label_destruct_00419d10(bytes(self) + 0x550, nullptr);                           // 006CC07D..C08B
    kotor_m15c_editbox_destruct_0041c740(bytes(self) + 0x3f0, nullptr);                       // 006CC090..C09B
    gui_load_button_destruct_00419da0(bytes(self) + 0x22c, nullptr);                          // 006CC0A0..C0AB
    gui_load_button_destruct_00419da0(bytes(self) + 0x68, nullptr);                           // 006CC0B0..C0B8
    gui_load_panel_destruct_0040cf70(self, nullptr);                                          // 006CC0BD..C0C7
}
void* __fastcall kotor_fastcall_006cc040(void* self, void*, U flags) {
    kotor_fastcall_006cc060(self, nullptr);                                                   // 006CC043
    if ((flags & 1u) != 0) live_free(self);                                                   // 006CC048..C055
    return self;                                                                              // 006CC058
}
void __fastcall kotor_fastcall_006c8410(void* self, void*) {
    kotor_m15c_editbox_set_focus_00418660(bytes(self) + 0x3f0, nullptr);                      // 006C8417..841D
    gui_load_panel_on_added_0040b870(self, nullptr);                                          // 006C8422..8424
    const I x = at<I>(self, 4), y = at<I>(self, 8);                                           // 006C8429..8436 extent (+0C/+10 dead)
    gui_m48_manager_store_mouse_0040a3d0(gui_manager(), nullptr);                             // 006C843D..8447
    gui_m48_manager_move_mouse_0040c790(gui_manager(), nullptr, x, y);                        // 006C844C..8454
}
void __fastcall kotor_fastcall_006c8460(void* self, void*) {
    gui_load_panel_on_removed_0040c170(self, nullptr);                                        // 006C8460
    gui_m48_manager_restore_mouse_0040c7d0(gui_manager(), nullptr);                           // 006C8465..846B
}
void __fastcall kotor_fastcall_006c8470(void* self, void*, void*) {
    pop_with_flags(self);                                                                     // 006C8470..8483
}
void __fastcall kotor_fastcall_006c9d50(void* self, void*, void*) {
    { Scoped name; name.copy(bytes(self) + 0x548);                                            // 006C9D6B..9D76 edit text value
      kotor_fastcall_006c8790(at<void*>(self, 0x64), nullptr, name.p()); }                    // 006C9D7B..9D9C WriteGame
    pop_with_flags(self);                                                                     // 006C9DA1..9DB4
}
void __fastcall kotor_fastcall_006cb040(void* self, void*, I event, I) {
    switch (event) {                                                                          // 006CB040..B053 (index bytes 006CB090)
    case 0x27: case 0x2d: kotor_fastcall_006c9d50(self, nullptr, nullptr); return;            // 006CB05A..B061
    case 0x28: case 0x2e: pop_with_flags(self); return;                                       // 006CB064..B07E
    default: return;                                                                          // 006CB07E (29h..2Ch, others)
    }
}

// ===================================================================================== CSWGuiSaveGameEditBox
void __fastcall kotor_fastcall_006c82f0(void* self, void*, I key) {
    if (static_cast<U>(key - 8) <= 0x77u) {                                                   // 006C82F0..82FA (index bytes 006C838C)
        switch (key) {
        case 0x08: case 0x7f:                                                                 // 006C830A
            kotor_m15c_edit_text_remove_last_char_004162e0(bytes(self) + 0xe0, nullptr);
            return;
        case 0x0a: case 0x0d: case 0x1b: {                                                    // 006C8336 / 006C8318
            void* const parent = at<void*>(self, 0x34);
            if (!parent) return;
            using Slot28 = void*(__thiscall*)(void*);
            void* const target = reinterpret_cast<Slot28>(at<void*>(at<void*>(parent, 0), 0x28))(parent);
            if (!target) return;
            using Input = void(__thiscall*)(void*, I, I);
            reinterpret_cast<Input>(at<void*>(at<void*>(target, 0), 0x3c))(target, key == 0x1b ? 0x28 : 0x27, 1);
            return;
        }
        default: break;                                                                       // index 3 -> 006C8354
        }
    }
    if (key < 0x20) return;                                                                   // 006C8354..8357 (signed)
    kotor_savegui_key_00833bd0[0] = static_cast<char>(key);                                   // 006C8359
    kotor_savegui_key_00833bd0[1] = 0;                                                        // 006C835F
    kotor_m15c_edit_text_add_new_char_004163a0(bytes(self) + 0xe0, nullptr, kotor_savegui_key_00833bd0); // 006C8366..8374
}
void __fastcall kotor_fastcall_006c9dd0(void* self, void*) {
    kotor_m15c_editbox_destruct_0041c740(self, nullptr);                                      // 006C9DD0 jmp
}
void* __fastcall kotor_fastcall_006cb0a0(void* self, void*, U flags) {
    kotor_fastcall_006c9dd0(self, nullptr);                                                   // 006CB0A3
    if ((flags & 1u) != 0) live_free(self);                                                   // 006CB0A8..B0B5
    return self;                                                                              // 006CB0B8
}
}  // extern "C"
