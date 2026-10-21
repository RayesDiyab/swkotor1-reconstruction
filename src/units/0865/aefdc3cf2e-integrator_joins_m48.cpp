// M48 composer joins (single providers after the address-level double-owner check). Linker directives only; defines nothing.
// 00624A00 thiscall(1) RET4 == fastcall @12; body core47 gui_ingame_leaves.cpp (earliest staged)
#pragma comment(linker, "/alternatename:@gui_m48_panel_set_to_destroy_00624a00@12=@kotor_fastcall_00624a00@12")
// 00625200 thiscall() == fastcall @8; body core47
#pragma comment(linker, "/alternatename:@gui_m48_message_box_update_controls_00625200@8=@kotor_fastcall_00625200@8")
// 005EDA60 thiscall(2) RET8 == fastcall @16; body lane-m48-b client_status_summary.cpp
#pragma comment(linker, "/alternatename:@gui_m48_client_set_input_class_005eda60@16=@kotor_fastcall_005eda60@16")
// 00401510 cdecl(void) void; body lane-m48-a winmain_callback_cell.cpp
#pragma comment(linker, "/alternatename:_kotor_cdecl_00401510=_winmain_message_pump_00401510")
