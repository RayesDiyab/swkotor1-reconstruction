// lane-m48-b fork GUI composition joins (D9 /alternatename mechanism). Linker directives only; defines nothing.
// Each target is a single existing body at the same original address; ABI equality noted per line
// (D1: thiscall member with N stack words == extern "C" __fastcall(ECX, EDX unused, N words), callee cleanup N*4).
//
// 0040C150 CSWGuiManager::RemoveToolTip: thiscall() RET == @8 (core47 gui_manager_leaves.cpp).
#pragma comment(linker, "/alternatename:?RemoveToolTip@Manager@kotor_input_hover@@QAEXXZ=@kotor_fastcall_0040c150@8")
// 0040AA00 CSWGuiPanel::GetExtentAccountingForPanelOffset(Extent*): thiscall(1) RET 4 == @12 (gui-load owner, D62).
#pragma comment(linker, "/alternatename:?GetExtentAccountingForPanelOffset@Panel@kotor_input_hover@@QAEXPAUExtent@2@@Z=@gui_load_panel_offset_extent_0040aa00@12")
// 0041A5E0 CSWGuiListBox::HitCheckMouseLocal(int*): thiscall(1 word) RET 4, EAX control pointer (lane-input reads it
// as the int word) == @12 gui-load body.
#pragma comment(linker, "/alternatename:?HitCheckMouseLocal@ListBox@kotor_input_hover@@QAEHH@Z=@gui_load_listbox_hit_local_0041a5e0@12")
// 0041C400 CSWGuiListBox::HandleMouseMove: thiscall() RET == @8 (gui_m48_controls.cpp).
#pragma comment(linker, "/alternatename:?HandleMouseMove@ListBox@kotor_input_hover@@QAEXXZ=@gui_load_listbox_handle_mouse_move_0041c400@8")
// 0040B690 CSWGuiPanel::HitCheckMouse(int,int): thiscall(2) RET 8, EAX control == @16 (lane-input body, input_m48.cpp).
#pragma comment(linker, "/alternatename:@gui_load_panel_hit_check_0040b690@16=?HitCheckMouse@Panel@kotor_input_hover@@QAEPAUControl@2@HH@Z")
// 0040C1E0 CSWGuiManager::HandleMouseMove(int,int): thiscall(2) RET 8 == @16 (lane-input body, input_m48.cpp).
#pragma comment(linker, "/alternatename:@kotor_fastcall_0040c1e0@16=?HandleMouseMove@Manager@kotor_input_hover@@QAEXHH@Z")
// 00405150 CExoArrayList<unsigned long>::SetSize(int): lane-input spelling (fastcall WordArray*,void*,int) -> sibling fork
// CLIENT's single body @kotor_fastcall_00405150@12 (fastcall void*,void*,int; RET 4).
#pragma comment(linker, "/alternatename:?WordArraySetSize00405150@kotor_input_hover@@YIXPAUWordArray@1@PAXH@Z=@kotor_fastcall_00405150@12")
// 005EDA70 CClientExoApp::GetInputClass: C spelling used by gui_m48_controls.cpp (fastcall ECX,EDX @8, EAX word)
// -> lane-input member extracted in input_m48.cpp (thiscall, no stack args, RET).
#pragma comment(linker, "/alternatename:@gui_m48_client_get_input_class_005eda70@8=?GetInputClass@ClientApp@kotor_input_hover@@QAEIXZ")
