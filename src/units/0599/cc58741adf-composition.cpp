#include "6a3a57a963-composition.h"
#include <cstring>
namespace frame_control {
static U32 word(const void* p,U32 off){U32 v;std::memcpy(&v,static_cast<const unsigned char*>(p)+off,4);return v;}
static void* pointer(const void* p,U32 off){return reinterpret_cast<void*>(word(p,off));}
U32 __fastcall ResumeMenuFrame_00603893(menu_continuation::Client* self,void*,menu_continuation::Carry* carry){
 using namespace menu_continuation;
 const U32 character=reinterpret_cast<U32>(frame_GetCharacter_006346c0(pointer(self,0x270),nullptr,0));
 void* app=pointer(app_manager_007a39fc,4);void* saved_module=pointer(self,0x18);



 carry->saved_ebp_006038ab=character;

 WorldTimer* timer=frame_GetWorldTimer_005ed4f0(app,nullptr);
 frame_GetWorldTime_004ade40(timer,nullptr,&carry->world_time_local_2c,&carry->world_time_local_28);
 if(saved_module&&word(saved_module,0x48))return 0x006038d6;
 InGame* game=self->in_game;
 if(game&&word(game,0x188)!=0x7f000000&&word(app_manager_007a39fc,8))return 0x00603ac5;
 if(game){frame_GuiDispatch_006339c0(game,nullptr);frame_GuiDispatch_00632180(self->in_game,nullptr);}
 const U32 input=self->input_mode;
 if((input==0||input==4)&&self->field_288==0&&self->in_game->IsGlobalFading_0062ac60()==0&&self->in_game->IsGlobalFadeObscuring_0062ded0()==0){
  frame_DialogUpdate_005fa5a0(self,nullptr,frame_delta_0078e574);frame_DialogCheck_005f3ad0(self,nullptr);
 }
 return ContinueAfterR2_00603b6d(self,nullptr,carry);
}
U32 __fastcall ContinueAfterR2_00603b6d(menu_continuation::Client* self,void*,menu_continuation::Carry* carry){
 const U32 frontier=self->ContinueToFirstDraw_00603b6d(carry);
 if(frontier!=0x00603d26)return frontier;
 return AfterFirstDraw_00603d26(self,nullptr);
}
U32 __fastcall AfterFirstDraw_00603d26(menu_continuation::Client* self,void*){
 using namespace menu_continuation;
 if(frame_ContainsPanel_0040bd70(self->gui,nullptr,pointer(self->in_game,0x90))==0)frame_RePopulate_0062b050(self->in_game,nullptr);
 frame_delta_0078e574=self->gui->saved_delta();
 I64 ticks=exo_base_007a39e0->timers->GetHighResolutionTimer_005e9260();
 end_timer_high_008328ec=static_cast<U32>(static_cast<std::uint64_t>(ticks)>>32);
 const U32 previous=high_resolution_low_008328f0;end_timer_low_008328e8=static_cast<U32>(ticks);
 const U32 raw_difference=static_cast<U32>(ticks)-previous;
 __asm{fild raw_difference}
 __asm{fmul dword ptr profile_scale_0073d708}
 __asm{fstp dword ptr draw_elapsed_00832914}
 ticks=exo_base_007a39e0->timers->GetHighResolutionTimer_005e9260();
 high_resolution_low_008328f0=static_cast<U32>(ticks);
 const float saved_delta=frame_delta_0078e574;
 high_resolution_high_008328f4=static_cast<U32>(static_cast<std::uint64_t>(ticks)>>32);
 frame_SelectedHilite_005f7500(self,nullptr,saved_delta);
 frame_UpdateDisableDialogue_0062f780(self->in_game,nullptr);
 return next_suffix; // Explicit boundary; never an original MainLoop result.
}
}
