// M46 composition-time integrator joins (claude-takeover m46-compose). Linker directives only; defines nothing.
// Consumer spellings with no definition are joined to the single staged body of the same original address
// (D9 mechanism; ABI-identical spellings per the lane packets' joins lists; requested names verified undefined).
#pragma comment(linker, "/alternatename:_gl_blend_func_indexed_00401670=_gl_blend_func_00401670")   // cdecl(U,U) both (lane-m46-3dcore body)
#pragma comment(linker, "/alternatename:?arb_grass_program_0078feb8@kotor_trimesh_data@@3PADA=?arb_flat_vp_0078feb8@kotor_vp_data@@3PADA")   // char[1095] D13 cell (lane-3d-vp owner)
#pragma comment(linker, "/alternatename:@exo_array_list_assign_0046bc80@12=@texture_list_assign_0046bc80@12")   // fastcall @12 (D54 spelling pick, startup-misc)
#pragma comment(linker, "/alternatename:@exo_array_list_remove_00445080@12=@texture_list_remove_00445080@12")   // fastcall @12 (texture-epoch)
#pragma comment(linker, "/alternatename:@matrix_identity_00437af0@8=@matrix_identity_construct_00437af0@8")      // fastcall @8 (3d-model)
#pragma comment(linker, "/alternatename:@int_list_assign_00438bb0@12=?exo_int_list_assign_00438bb0@@YIPAUArrayMutations@kotor_reconstruction@@PAU12@PAXPBU12@@Z")   // fastcall @12, C++ spelling
#pragma comment(linker, "/alternatename:_expand_reference_parts_00454470=_aurora_expand_reference_parts_00454470")   // cdecl (3d-model)
// ---- single-body joins (address-level duplicates removed from composer copies; earliest-staged owner keeps the body)
// 00405ED0 fastcall(ECX,EDX) @8 both
#pragma comment(linker, "/alternatename:?ref_default_element_00405ed0@kotor_item_drop_sounds@@YIPAXPAX0@Z=@c2da_default_ref_00405ed0@8")
// 005C5630 fastcall @20 both
#pragma comment(linker, "/alternatename:?client_slot00_005c5630@kotor_client_exo_app@@YIXPAX0III@Z=@gui_scene_void_return_0c_005c5630@20")
// 00425250 cdecl(U) both
#pragma comment(linker, "/alternatename:_gl_render_activate_texture_stage_00425250=_gl_render_activate_client_texture_stage_00425250")
// 004AC960 fastcall @24 both
#pragma comment(linker, "/alternatename:@quaternion_from_axis_angle_004ac960@24=@quaternion_construct_004ac960@24")
// 0078E614 volatile U32, image 1, both
#pragma comment(linker, "/alternatename:?gate_0078e614@kotor_vp_cells@@3IC=_scene3d_gate_0078e614")
// 007FBF78 12-byte PointerList, zero image, both
#pragma comment(linker, "/alternatename:_emitter_bucket_007fbf78=_scene3d_emitter_bucket_007fbf78")
// 007B9374 volatile U32, BSS 0, both
#pragma comment(linker, "/alternatename:_shadow_volume_programs_007b9374=_project_shadow_gate_007b9374")
