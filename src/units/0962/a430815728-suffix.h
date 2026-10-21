#pragma once
#include "f50185569c-composition.h"
#include "dc6cf5303a-ftol2.h"
namespace frame_control {
// Preserve existing Carry unchanged. This separate original stack local was
// initialized at00602EDB; admitted prefix paths must retain its actual value.
struct SuffixState { U32 stack20_00603e74; U32 mainloop_eax; U32 return_ready; };
extern U32 sound_manager_007a39ec,snapshot_enabled_007a2284;
extern float sound_elapsed_00832918;
extern const float milliseconds_0073d6fc;
extern U32 log_request_008328d8[2]; // borrowed CExoString object, not a pointer cell
U32 __fastcall ContinueSuffix_00603db8(menu_continuation::Client*,void*,SuffixState*);
U32 __fastcall ContinueAfterR2ThroughSuffix(menu_continuation::Client*,void*,menu_continuation::Carry*,SuffixState*);
}
extern "C" {
void __fastcall frame_UpdateSound_005f5370(menu_continuation::Client*,void*);
void __cdecl frame_AurSnapshot_004211a0(void*,unsigned);
void __fastcall frame_SetAutoPaused_005f3f10(menu_continuation::Client*,void*,unsigned,unsigned);
void* __fastcall frame_GetServerArea_00605640(void*,void*);
unsigned __fastcall frame_GetPauseState_004ae980(void*,void*,unsigned);
unsigned __fastcall frame_TogglePauseRequest_00677800(void*,void*);
void __fastcall frame_SetPaused_0062def0(menu_continuation::InGame*,void*,unsigned,unsigned);
unsigned __fastcall frame_ShowTutorial_0062f4a0(menu_continuation::InGame*,void*,unsigned,unsigned,unsigned,unsigned);
unsigned __fastcall frame_LogIsEmpty_005e58a0(void*,void*);
}
