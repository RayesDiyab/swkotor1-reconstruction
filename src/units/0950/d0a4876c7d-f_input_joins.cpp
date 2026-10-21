// lane-m48-traps fork f-input: linker joins (D1/D58 precedent, as lane-m48-b gui/src/m48_gui_joins.cpp). Each left name is the
// lane-input donor spelling referenced by this fork's objects; each right name is the single existing composed-link owner of
// the same original address. Every pair is the same original __thiscall body: member thiscall (ECX=this, callee-cleaned
// stack args) is ABI-identical to __fastcall(ECX, unused EDX, same stack args) (D1); the stack-argument byte counts match
// the decorated @N of the right names. No body, no address credit.
#pragma comment(linker, "/alternatename:?GetActive@Input@kotor_input_hover@@QAEHXZ=@kotor_fastcall_005df510@8")  // 005DF510 routes-window-leaves
#pragma comment(linker, "/alternatename:?BufferRaw@InputInternal@kotor_input_hover@@QAEXIIH@Z=@kotor_fastcall_005e08c0@20")  // 005E08C0 lane-m48-b client-input-buffer
#pragma comment(linker, "/alternatename:?RemovePanel@Manager@kotor_input_hover@@QAEXPAUPanel@2@@Z=@frame_RemovePanel_0040c830@12")  // 0040C830
#pragma comment(linker, "/alternatename:?file_write_string_005e6a70@kotor_input_required@@YIXPAX0PAUStartupString@kotor_reconstruction@@@Z=@frame_LogWriteString_005e6a70@12")  // 005E6A70 CExoFile::Write(CExoString&)
#pragma comment(linker, "/alternatename:?remove_resource_image_file_00408830@kotor_input_required@@YIXPAUManager@menu_resources@@PAXPAUStartupString@kotor_reconstruction@@@Z=@kotor_fastcall_00408830@12")  // 00408830 CExoResMan::RemoveResourceImageFile
#pragma comment(linker, "/alternatename:?get_capture_event_play_back_005ed480@kotor_input_required@@YIHPAX0@Z=@frame_R4_005ed480@8")  // 005ED480
#pragma comment(linker, "/alternatename:?get_character_006346c0@kotor_input_required@@YIPAXPAX0I@Z=@frame_GetCharacter_006346c0@12")  // 006346C0 CSWParty::GetCharacter
#pragma comment(linker, "/alternatename:?get_in_game_gui_005ed690@kotor_input_required@@YIPAXPAX0@Z=@frame_R4_005ed690@8")  // 005ED690
#pragma comment(linker, "/alternatename:?get_player_creature_005ed540@kotor_input_required@@YIPAXPAX0@Z=@client_get_player_creature_005ed540@8")  // 005ED540
#pragma comment(linker, "/alternatename:?get_player_creature_005f1420@kotor_input_required@@YIPAXPAX0@Z=@kotor_fastcall_005f1420@8")  // 005F1420
#pragma comment(linker, "/alternatename:?get_snapshot_time_004adc70@kotor_input_required@@YI_KPAX0@Z=@kotor_fastcall_004adc70@8")  // 004ADC70 CWorldTimer::GetSnapshotTime (EDX:EAX)
#pragma comment(linker, "/alternatename:?get_snapshot_time_difference_004adc80@kotor_input_required@@YIHPAX0@Z=?GetSnapshotTimeDifference_004adc80@WorldTimer@menu_continuation@@QAE_JXZ")  // 004ADC80 (EAX consumed, FILD 00622890)
#pragma comment(linker, "/alternatename:?get_sw_party_005ed8b0@kotor_input_required@@YIPAXPAX0@Z=@kotor_fastcall_005ed8b0@8")  // 005ED8B0
#pragma comment(linker, "/alternatename:?get_world_timer_005ed4f0@kotor_input_required@@YIPAXPAX0@Z=@frame_GetWorldTimer_005ed4f0@8")  // 005ED4F0
