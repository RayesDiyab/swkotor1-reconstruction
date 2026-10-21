// lane-m48-b composition joins (D9 /alternatename mechanism). Linker directives only; defines nothing.
// Each requested name is unresolved in the M47 prediction (m47-compose SYMBOL-PREDICTION.json); each target is the single
// body already staged in M47 at the same original address (m47-compose tools/symbols-m45.json defined_by). ABI checked
// from both mangled names / declarations: same convention, same argument words, same callee cleanup, same return.
//
// 005F1B20 CClientExoAppInternal::FadeAndStopStreamingSoundSource: fastcall(ECX,EDX,void*,U) @ RET 8 both (YIXPAX00I / YIXPAUMusicInternal..PAX1I).
#pragma comment(linker, "/alternatename:?FadeAndStop_005f1b20@menu_tables_required@@YIXPAX00I@Z=?gui_fade_005f1b20@table29_required@@YIXPAUMusicInternal@menu_normal_40_49@@PAX1I@Z")
// 005D5C50/005D5CD0/005D5D50 CExoSound::Set{Music,SoundEffect,Dialog}Volume: identical parameter lists (YIXPAX0MI / YIXPAX0M).
#pragma comment(linker, "/alternatename:?SoundMusicVolume_005d5c50@lane_config_required@@YIXPAX0MI@Z=?SoundSetMusicVolume005d5c50@kotor_sound_path@@YIXPAX0MI@Z")
#pragma comment(linker, "/alternatename:?SoundEffectVolume_005d5cd0@lane_config_required@@YIXPAX0M@Z=?SoundSetSoundEffectVolume005d5cd0@kotor_sound_path@@YIXPAX0M@Z")
#pragma comment(linker, "/alternatename:?SoundDialogVolume_005d5d50@lane_config_required@@YIXPAX0M@Z=?SoundSetDialogVolume005d5d50@kotor_sound_path@@YIXPAX0M@Z")
// 005D5A50..005D6200 CExoStreamingSoundSource members (lane-sound bodies; table29_required spellings): identical parameter lists.
#pragma comment(linker, "/alternatename:?StreamConstruct_005d5a50@menu_tables_required@@YIPAXPAX0@Z=?gui_sound_construct_005d5a50@table29_required@@YIPAXPAX0@Z")
#pragma comment(linker, "/alternatename:?StreamIsPlaying_005d5af0@menu_tables_required@@YIIPAX0@Z=?gui_playing_005d5af0@table29_required@@YIIPAX0@Z")
#pragma comment(linker, "/alternatename:?StreamPlay_005d5b10@menu_tables_required@@YIIPAX0PAURef@menu_resources@@IIII@Z=?gui_play_005d5b10@table29_required@@YIIPAX0PAURef@menu_resources@@IIII@Z")
#pragma comment(linker, "/alternatename:?StreamResume_005d5b30@menu_tables_required@@YIXPAX0@Z=?gui_resume_005d5b30@table29_required@@YIXPAX0@Z")
#pragma comment(linker, "/alternatename:?StreamSetLooping_005d5b40@menu_tables_required@@YIXPAX0I@Z=?gui_loop_005d5b40@table29_required@@YIXPAX0I@Z")
#pragma comment(linker, "/alternatename:?StreamSetPriority_005d5ba0@menu_tables_required@@YIXPAX0I@Z=?gui_priority_005d5ba0@table29_required@@YIXPAX0I@Z")
#pragma comment(linker, "/alternatename:?StreamDestroy_005d6200@menu_tables_required@@YIXPAX0@Z=?gui_sound_destroy_005d6200@table29_required@@YIXPAX0@Z")
// 005D6200 extern "C" fastcall(ECX,EDX) @8 spelling (routes-shutdown, startup46-shutdown-callees) -> same body.
#pragma comment(linker, "/alternatename:@kotor_fastcall_005d6200@8=?gui_sound_destroy_005d6200@table29_required@@YIXPAX0@Z")
// 005F9AF0 CClientExoAppInternal::StartMenuMusic(int,int,int): fastcall(ECX,EDX,U,U,U) @20 == YIXPAUMusicInternal..PAXIII (RET 0Ch).
#pragma comment(linker, "/alternatename:@frame_R4_005f9af0@20=?StartMenuMusic005f9af0@kotor_sound_path@@YIXPAUMusicInternal@menu_normal_40_49@@PAXIII@Z")
// 0044C1F0 FuncInterp: cdecl char*(char*) both (r4_lowers.h:122 / 3d model_console.cpp:28, D67 owner lane-3d-model).
#pragma comment(linker, "/alternatename:_kotor_cdecl_0044c1f0=_aurres_func_interp_0044c1f0")
// 007B8FC0 model gate byte: `volatile std::uint8_t` both (r4_lowers.h:45 namespace view / 3d model_instance.cpp:21 extern "C").
#pragma comment(linker, "/alternatename:?aurora_model_gate_007b8fc0@aurora_model@@3EC=_aurora_model_gate_007b8fc0")
