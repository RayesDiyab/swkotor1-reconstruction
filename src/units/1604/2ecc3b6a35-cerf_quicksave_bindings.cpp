// Lane B import bindings.
//
// These are one-to-one symbol joins from the lane's local C-linkage views to the
// exact selected v5 provider symbols. They are import bindings, not composition
// aliases, define no body, and contain no numeric address. Every target was
// verified in EVIDENCE/reuse-registry.json and is a selected symbol in the frozen
// v5 MAP. No target is on another lane's new-body list.

#pragma comment(linker, "/alternatename:@cerf_startup_string_default@8=??0StartupString@kotor_reconstruction@@QAE@XZ")
#pragma comment(linker, "/alternatename:@cerf_startup_string_from_text@12=??0StartupString@kotor_reconstruction@@QAE@RBD@Z")
#pragma comment(linker, "/alternatename:@cerf_startup_string_destroy@8=??1StartupString@kotor_reconstruction@@QAE@XZ")
#pragma comment(linker, "/alternatename:@cerf_startup_string_assign@12=?Assign005e5c50@StartupString@kotor_reconstruction@@QAEPAU12@PAU12@@Z")
#pragma comment(linker, "/alternatename:@cerf_startup_string_borrowed_length@8=?BorrowedLength005e5790@StartupString@kotor_reconstruction@@QAEHXZ")
#pragma comment(linker, "/alternatename:_cerf_startup_format=?StartupFormat005e5680@kotor_reconstruction@@YAXPAUStartupString@1@PBDZZ")
#pragma comment(linker, "/alternatename:@cerf_gui_concat@16=?GuiConcat005e5d10@@YIPAUStartupString@kotor_reconstruction@@PAU12@PAX0PBU12@@Z")

#pragma comment(linker, "/alternatename:_cerf_construct_list_internal=?construct_list_internal_005e9aa0@recovered_server_create@@YAPAXPAX@Z")
#pragma comment(linker, "/alternatename:@cerf_destroy_list@8=?destroy_list_005e9ab0@base_ini_reconstruction@@YIXPAUInternal@kotor_list@@PAX@Z")
#pragma comment(linker, "/alternatename:@cerf_remove_head@8=?remove_head_005e9c80@base_ini_reconstruction@@YIPAXPAUInternal@kotor_list@@PAX@Z")
#pragma comment(linker, "/alternatename:@cerf_list_unwind@8=?ListUnwind005d86c0@kotor_sound_internal@@YIXPAX0@Z")
#pragma comment(linker, "/alternatename:@cerf_list_add_tail@12=?AddTail005e9dc0@ini_section@@YIPAUNode@base_ini_reconstruction@@PAUInternal@kotor_list@@PAX1@Z")
#pragma comment(linker, "/alternatename:_cerf_list_at_pos@4=@unresolved_list_at_pos_005e9b60@12")
#pragma comment(linker, "/alternatename:_cerf_list_next@4=?next_005e9b90@base_ini_reconstruction@@YIPAXPAUInternal@kotor_list@@PAXPAPAUNode@1@@Z")

#pragma comment(linker, "/alternatename:@cerf_exofile_construct@20=?exofile_construct_005e68d0@file_internal@@YIPAUFile@tlk_file@@PAU23@PAXPAUStartupString@kotor_reconstruction@@I2@Z")

// 006C8750's /arch:IA32 x87 conversion emits __ftol2; join it to the selected
// genuine x87 body (006FAE8C alias chain, 00637800 -> 0062A730).
#pragma comment(linker, "/alternatename:__ftol2=_genuine_x87_ftol_006fae8c")
