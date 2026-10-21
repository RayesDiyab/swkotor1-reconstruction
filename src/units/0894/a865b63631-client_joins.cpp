// lane-m48-b fork CLIENT joins (D9 /alternatename mechanism). Linker directives only; defines nothing.
// 005D5E80 CExoSound::SetSoundMode single body = task-29 sound-mode-closure-r2 donor (sound/donors/donor-sound-mode/
// sound_mode.cpp, ?gui_sound_mode_005d5e80@table29_required@@YIXPAX0I@Z): fastcall(outer ECX, unused EDX, mode) RET 4.
// menu_tables_required spelling: identical parameter list (YIXPAX0I).
#pragma comment(linker, "/alternatename:?SoundSetMode_005d5e80@menu_tables_required@@YIXPAX0I@Z=?gui_sound_mode_005d5e80@table29_required@@YIXPAX0I@Z")
// frame-r4 menu_r2 spelling: extern "C" fastcall, 12 argument bytes (ECX, EDX, one stack word) == RET 4; void.
#pragma comment(linker, "/alternatename:@frame_R4_005d5e80@12=?gui_sound_mode_005d5e80@table29_required@@YIXPAX0I@Z")
