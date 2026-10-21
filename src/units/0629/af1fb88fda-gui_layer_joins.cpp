// lane-gui-render: joins of lane-routes' R4 draw.obj consumer names onto the
// single existing owners in lane-gui-draw writers (M30 r5 layers.cpp, Task22
// producer; task-6 pulse donor). No definitions, no second bodies.
// ABI identity per pair:
//  - 004591B0 / 004592B0 / 00459580: no stack arguments, plain RET; the owners'
//    __stdcall@0 and R4's __cdecl() are the same machine contract.
//  - 004592F0: both __cdecl with 7 stack DWORDs (R4 types args 6/7 as float, the
//    owner as DWORD: identical stack words), EAX result.
//  - 00414A50: both __cdecl(float), RET (owner GuiPulseUpdate00414a50).
// Mechanism as accepted in D9 (/alternatename join).
#pragma comment(linker, "/alternatename:_frame_PushRender_004591b0=_kotor_AurGUIStartLayer_004591B0@0")
#pragma comment(linker, "/alternatename:_frame_PopRender_004592b0=_kotor_AurGUIStopLayer_004592B0@0")
#pragma comment(linker, "/alternatename:_frame_DrawFinish_00459580=_kotor_AurGUICloseViewport_00459580@0")
#pragma comment(linker, "/alternatename:_frame_Viewport_004592f0=_kotor_AurGUISetupViewport_004592F0")
#pragma comment(linker, "/alternatename:_frame_SceneUpdate_00414a50=_GuiPulseUpdate00414a50")
// lane-gui-load method-table names (gui_vtables.obj / mainmenu.obj) onto this lane's bodies.
#pragma comment(linker, "/alternatename:@gui_load_border_draw_004168c0@12=@kotor_fastcall_004168c0@12")
#pragma comment(linker, "/alternatename:@gui_load_label_draw_00417750@12=@kotor_fastcall_00417750@12")
#pragma comment(linker, "/alternatename:@gui_load_button_draw_00417ab0@12=@kotor_fastcall_00417ab0@12")
#pragma comment(linker, "/alternatename:@gui_load_panel_draw_0040b760@12=@kotor_panel_draw_0040B760@12")
#pragma comment(linker, "/alternatename:@gui_load_label_initialize_004176e0@24=@kotor_fastcall_004176e0@24")
// D49 canonical epoch names used by DrawPCBG onto lane-gui-load's authored bodies (single owner each).
#pragma comment(linker, "/alternatename:@kotor_fastcall_0041acd0@8=@gui_load_label_construct_0041acd0@8")
#pragma comment(linker, "/alternatename:@kotor_fastcall_00417050@8=@gui_load_text_params_construct_00417050@8")
#pragma comment(linker, "/alternatename:@kotor_fastcall_00415000@40=@gui_load_border_params_construct_00415000@40")
// lane-3d-emitter (emitter.h, extern "C" cdecl) consumer name of 00431CC0 onto this lane's body.
#pragma comment(linker, "/alternatename:_gl_render_objects_vtc_quad_up_00431cc0=_kotor_cdecl_00431cc0")
// rev2 (D62/D59 review fixes):
// 0040AA00 single provider lane-gui-load; this lane's Panel::Draw call name joined to it.
#pragma comment(linker, "/alternatename:@kotor_panel_extent_0040AA00@12=@gui_load_panel_offset_extent_0040aa00@12")
// 00414CD0 single provider lane-gui-render (D62); lane-gui-load's consumer name joined here.
#pragma comment(linker, "/alternatename:@gui_load_border_params_dim_00414cd0@8=@kotor_fastcall_00414cd0@8")
// 0059EA30 single provider lane-menu-tables (D59).
#pragma comment(linker, "/alternatename:@kotor_fastcall_0059ea30@8=?LiveContentDestroy_0059ea30@menu_tables_m41@@YIXPAX0@Z")
// GR-2: lane-gui-load consumer name of 0059EA30 (MainMenu dtor array), joined directly to the same owner.
#pragma comment(linker, "/alternatename:@gui_load_module_entry_destruct_0059ea30@8=?LiveContentDestroy_0059ea30@menu_tables_m41@@YIXPAX0@Z")
// lane-gui-load consumer name of 0045BDF0 joined onto this lane's NewCAurGUIString.
#pragma comment(linker, "/alternatename:_gui_load_new_aur_gui_string_0045bdf0=_kotor_cdecl_0045bdf0")
// The translation unit must not be empty for the pinned flags (/W4 /WX).
namespace gui_render_joins { extern const int present = 1; }
