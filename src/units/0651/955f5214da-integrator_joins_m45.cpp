// M45 composition-time integrator joins (claude-takeover m45-compose). Linker directives only:
// this translation unit defines no code and no data. Each /alternatename resolves a consumer name whose
// duplicate body/storage was removed from a composer-adapted copy onto the single selected provider
// (D59 / D65 conditions C2-C3; D9 mechanism; ABI-identical spellings; no thunk, no cast).
// C2 0044C980 dofile -> lane-aurres (D59): cdecl char*(const char*) both.
#pragma comment(linker, "/alternatename:_kotor_cdecl_0044c980=_aurres_dofile_0044c980")
// C2 00401280 CAppManager::DestroyServer -> lane-routes lifecycle body: fastcall(ECX,EDX) @8 both.
#pragma comment(linker, "/alternatename:@frame_R4_00401280@8=@kotor_fastcall_00401280@8")
// C2 00404DD0 GetProperty0x30 -> existing M43 tpc-lowers provider: fastcall @8 both.
#pragma comment(linker, "/alternatename:@kotor_fastcall_00404dd0@8=@c2da_current_resource_data_00404dd0@8")
// C2 005DF6E0 CExoInput::GetMouseMode -> existing M40 input-snapshot raw-AL getter (thiscall no stack args == fastcall(ECX,EDX)).
#pragma comment(linker, "/alternatename:@kotor_fastcall_005df6e0@8=?GetMouseMode@InputView@kotor_reconstruction@@QAEEXZ")
// C3 single storage (lane-prepare route_cells names -> existing owners; same width and image initial values).
#pragma comment(linker, "/alternatename:_kotor_cell_headlight_0078e388=?format_0078e388@kotor_renderer_gl_cells@@3GA")
#pragma comment(linker, "/alternatename:_kotor_cell_vbo_write_only_0078e398=?renderer_flag_0078e398@kotor_renderer_gl_cells@@3EA")
#pragma comment(linker, "/alternatename:_kotor_cell_vbo_disabled_007b927d=?vertex_buffer_disabled_007b927d@kotor_renderer_gl_cells@@3EA")
#pragma comment(linker, "/alternatename:_kotor_cell_resource_table_007b8fd8=_aurres_stack_007b8fd8")
#pragma comment(linker, "/alternatename:_kotor_cell_dofile_message_007b8fe8=_aurres_dofile_message_007b8fe8")
// C2 00414CD0 CSWGuiBorderParams::GetBorderDim -> lane-gui-render (D62): fastcall(ECX,EDX) @8 both.
#pragma comment(linker, "/alternatename:@gui_load_border_params_dim_00414cd0@8=@kotor_fastcall_00414cd0@8")
// ---- 3D single-body joins (D65 rule / D50 / D55 / D68); duplicate bodies removed from composer copies.
// 004AB100 vector magnitude -> lane-3d-render (D50): cdecl float(const float*) both.
#pragma comment(linker, "/alternatename:_kotor_cdecl_004ab100=_vector_magnitude_004ab100")
// 00444FE0 fastcall(ECX,EDX,int) @12 both
#pragma comment(linker, "/alternatename:@int_list_add_00444fe0@12=?add_00444fe0@gui_viewport_list@@YIXPAUArrayMutations@kotor_reconstruction@@PAXH@Z")
// 004064A0 fastcall @8 both
#pragma comment(linker, "/alternatename:@gui_scene_get_property_0x10_004064a0@8=@gui_load_slot_prop10_004064a0@8")
// 00445020 fastcall @16 both
#pragma comment(linker, "/alternatename:@aurora_list_insert_00445020@16=@list_insert_00445020@16")
// 00671C00 fastcall @12 both
#pragma comment(linker, "/alternatename:@aurora_list_add16_00671c00@12=@gui_load_array_add_00671c00@12")
// 004AB180 cdecl both (D55)
#pragma comment(linker, "/alternatename:_vector_normalized_004ab180=_normalize_004ab180")
// 0044DA60 cdecl void() both
#pragma comment(linker, "/alternatename:_aur_enable_head_light_0044da60=_kotor_cdecl_0044da60")
// 0044DA80 cdecl void() both
#pragma comment(linker, "/alternatename:_aur_disable_head_light_0044da80=_kotor_cdecl_0044da80")
// 005B5E90 fastcall @16 both; original leaves EAX undefined (D68)
#pragma comment(linker, "/alternatename:@camera_gob_void_return_8_005b5e90@16=@gui_load_slot_ret8_005b5e90@16")
// 005B5E90 fastcall @16 both (D68)
#pragma comment(linker, "/alternatename:@light_manager_void_return_8_005b5e90@16=@gui_load_slot_ret8_005b5e90@16")
// 0063E7C0 fastcall @12 both (D68)
#pragma comment(linker, "/alternatename:@aurora_return_false_4_0063e7c0@12=@packed_return_false_0063e7c0@12")
// 00798B68 data cell, volatile U both, image value 1 (D68; owner choice recorded for integrator)
#pragma comment(linker, "/alternatename:_emitter_render_mode_00798b68=_gate_00798b68")
