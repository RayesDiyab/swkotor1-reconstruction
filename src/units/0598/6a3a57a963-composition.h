#pragma once
#include "866a482a6a-menu_continuation.h"
// These declarations name required genuine original provider joins, not
// callbacks or provider definitions. No missing provider returns success here.
extern "C" {
void* __fastcall frame_GetCharacter_006346c0(void*,void*,int);
menu_continuation::WorldTimer* __fastcall frame_GetWorldTimer_005ed4f0(void*,void*);
void __fastcall frame_GetWorldTime_004ade40(menu_continuation::WorldTimer*,void*,unsigned*,unsigned*);
void __fastcall frame_GuiDispatch_006339c0(void*,void*);
void __fastcall frame_GuiDispatch_00632180(void*,void*);
void __fastcall frame_DialogUpdate_005fa5a0(menu_continuation::Client*,void*,float);
void __fastcall frame_DialogCheck_005f3ad0(menu_continuation::Client*,void*);
unsigned __fastcall frame_ContainsPanel_0040bd70(menu_continuation::GuiManager*,void*,void*);
void __fastcall frame_RePopulate_0062b050(menu_continuation::InGame*,void*);
void __fastcall frame_SelectedHilite_005f7500(menu_continuation::Client*,void*,float);
void __fastcall frame_UpdateDisableDialogue_0062f780(menu_continuation::InGame*,void*);
// Draw direct-provider joins, preserving their observed physical call shapes.
void __cdecl frame_RenderBoundary_0044ed90();
void __cdecl frame_SceneUpdate_00418300(float);
void __cdecl frame_SceneUpdate_00414a50(float);
void __fastcall frame_DrawCursor_0040ae80(menu_continuation::GuiManager*,void*,float) noexcept(false);
unsigned __fastcall frame_FindPopup_0040ab70(menu_continuation::GuiManager*,void*,void*);
void __fastcall frame_RemoveArray_006857a0(void*,void*,int);
void __cdecl frame_PushRender_004591b0();
unsigned __cdecl frame_Viewport_004592f0(int,int,int,int,float*,float,float);
void __cdecl frame_DrawFinish_00459580();
void __cdecl frame_PopRender_004592b0();
unsigned __fastcall frame_UnknownClientFlag_005ee230(void*,void*);
void __fastcall frame_RemovePanel_0040c830(menu_continuation::GuiManager*,void*,void*);
void __fastcall frame_DrawTail_0040bec0(menu_continuation::GuiManager*,void*,float) noexcept(false);
}
namespace frame_control {
using U32=menu_continuation::U32;
// All mutable cells are borrowed from the sole canonical owner.
extern U32 draw_tail_007a3d4c;
extern U32 end_timer_low_008328e8,end_timer_high_008328ec;
extern float draw_elapsed_00832914;
extern const float profile_scale_0073d708;
constexpr U32 next_suffix=0x00603db8;
U32 __fastcall ResumeMenuFrame_00603893(menu_continuation::Client*,void*,menu_continuation::Carry*);
U32 __fastcall ContinueAfterR2_00603b6d(menu_continuation::Client*,void*,menu_continuation::Carry*);
U32 __fastcall AfterFirstDraw_00603d26(menu_continuation::Client*,void*);
}
