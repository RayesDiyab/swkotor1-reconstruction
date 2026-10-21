// M44 composition-time integrator joins (claude-takeover m44-compose). Linker directives only:
// this translation unit defines no code and no data. Each /alternatename resolves an otherwise
// undefined consumer name onto the single selected provider of the same original address
// (ABI-identical spellings; D9 mechanism). No thunk, no cast, no second body.
//
// D50: lane-r4-lowers names of cells/functions owned elsewhere (claude-takeover/lane-r4-lowers PACKET
//      alternatename_joins_required). frame_LogFormat is variadic cdecl: alias only, never a forwarder.
#pragma comment(linker, "/alternatename:?app_manager_007a39fc@menu_continuation@@3PAUAppManager@1@A=?manager_007a39fc@kotor_menu_window_cells@@3RAXA")
#pragma comment(linker, "/alternatename:?exo_base_007a39e0@menu_continuation@@3PAUExoBase@1@A=?base_007a39e0@dependencies@kotor_menu_10_19@@3PAXA")
#pragma comment(linker, "/alternatename:?frame_delta_0078e574@menu_continuation@@3MA=?delta_t_0078e574@texture_cells@@3MC")
#pragma comment(linker, "/alternatename:?resource_manager_007a39e8@frame_control@@3IA=?resource_manager_007a39e8@kotor_resource_demand@@3RAUManager@menu_resources@@A")
#pragma comment(linker, "/alternatename:?sound_manager_007a39ec@frame_control@@3IA=_kotor_cell_sound_007a39ec")
#pragma comment(linker, "/alternatename:_frame_LogFormat_005e5680=?StartupFormat005e5680@kotor_reconstruction@@YAXPAUStartupString@1@PBDZZ")
// D38 G2 / D36 handover: 0061FA00 ShowMouse single provider = lane-r4-lowers adopted show_mouse_unpauseable.cpp
//      (__fastcall(ECX receiver, EDX unused, int), RET 4 on both spellings).
#pragma comment(linker, "/alternatename:@kotor_fastcall_0061fa00@12=?ShowMouse0061fa00@kotor_game_prepare@@YIXPAX0H@Z")
// 0040BD70 CSWGuiManager::PanelExists single provider = lane-r4-lowers @frame_ContainsPanel_0040bd70@12
//      (__fastcall(ECX manager, EDX unused, panel*) -> int, RET 4 on both spellings); integrator to confirm (CONFLICTS).
#pragma comment(linker, "/alternatename:?PanelExists_0040bd70@lane_config_required@@YIHPAX00@Z=@frame_ContainsPanel_0040bd70@12")
// D46/D22: 007A39F4 GUI manager pointer single owner lane-input (kotor_input_cells, void* volatile both spellings).
#pragma comment(linker, "/alternatename:?gui_manager_007a39f4@kotor_menu_window_cells@@3RAXA=?gui_manager_007a39f4@kotor_input_cells@@3RAXA")
// D47: 005ED700 GetOptions single provider = M41 kotor_menu20::ClientApp::get_options_005ed700 (thiscall, no stack args == fastcall(ECX,EDX) @8; D1/D9).
#pragma comment(linker, "/alternatename:@kotor_fastcall_005ed700@8=?get_options_005ed700@ClientApp@kotor_menu20@@QAEPAXXZ")
// D18: 005E5BC0 CExoString(int) single provider = lane-resman resman_string_from_int (fastcall @12 both).
#pragma comment(linker, "/alternatename:@kotor_fastcall_005e5bc0@12=@resman_string_from_int_005e5bc0@12")
// D23/D44: 0041E550 CTlkTable::Fetch single provider = lane-tlk (fastcall @20 both).
#pragma comment(linker, "/alternatename:@gui_load_tlk_fetch_0041e550@20=@tlk_fetch_0041e550@20")
// Single existing body, ABI-identical spellings (integrator to confirm; CONFLICTS composer-joins):
// 004B25D0 list ctor: thiscall, no stack args, returns this == fastcall(ECX,EDX) returning void*, no stack args -> lane-menu-tables body.
#pragma comment(linker, "/alternatename:?Construct004b25d0@ListWrapper@kotor_input_hover@@QAEPAU12@XZ=?ClientListConstruct_004b25d0@menu_tables_m41@@YIPAXPAX0@Z")
// 00445080 RemoveReverse32-style list remove: fastcall @12 both -> lane-texture-epoch body.
#pragma comment(linker, "/alternatename:@kotor_fastcall_00445080@12=@texture_list_remove_00445080@12")
// 006FB55F eh vector constructor iterator: stdcall 5 dwords (@20) both -> lane-menu-tables body.
#pragma comment(linker, "/alternatename:_crt_eh_vector_constructor_006fb55f@20=?VectorConstruct_006fb55f@menu_tables_m41@@YGXPAXIIP6IPAX00@ZP6IX00@Z@Z")
// 0044C740 AurResGet / 0044C8F0 AurResFree: cdecl (caller cleanup) -> lane-aurres bodies (routes passes 0 for the size pointer; return of release ignored).
#pragma comment(linker, "/alternatename:_kotor_cdecl_0044c740=_aurres_get_0044c740")
#pragma comment(linker, "/alternatename:_kotor_cdecl_0044c8f0=_aurres_release_0044c8f0")
// 00462A70/004630B0/00463260/00463320/00463470 texture controller ctors: fastcall @12 both -> lane-texture-epoch bodies (lane-font procedure_bindings consumer).
#pragma comment(linker, "/alternatename:@procedure_base_ctor_00462a70@12=@texture_controller_ctor_00462a70@12")
#pragma comment(linker, "/alternatename:@procedure_water_ctor_004630b0@12=@water_controller_ctor_004630b0@12")
#pragma comment(linker, "/alternatename:@procedure_wave_ctor_00463260@12=@wave_controller_ctor_00463260@12")
#pragma comment(linker, "/alternatename:@procedure_arturo_ctor_00463320@12=@arturo_controller_ctor_00463320@12")
#pragma comment(linker, "/alternatename:@procedure_cycle_ctor_00463470@12=@cycle_controller_ctor_00463470@12")
// D59: 0059EA30 single provider = lane-menu-tables vector callback body (fastcall(ECX,EDX) @8 both spellings).
#pragma comment(linker, "/alternatename:@gui_load_module_entry_destruct_0059ea30@8=?LiveContentDestroy_0059ea30@menu_tables_m41@@YIXPAX0@Z")
