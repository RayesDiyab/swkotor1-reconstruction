#include "7879f51aba-menu_continuation.h"
namespace menu_continuation {
U32 __thiscall Client::ContinueToFirstDraw_00603b6d(Carry* carry) {
 // Carry is deliberately unchanged here; later gates still require all words.
 (void)carry;
 I64 ticks=app_manager_007a39fc->app->client->timer->GetSnapshotTimeDifference_004adc80();
 float elapsed;
 __asm {
  fild qword ptr ticks
  fmul dword ptr scale_007439bc
  fstp dword ptr elapsed
 }
 UpdateHostileHilite_005f7640(elapsed);
 I64 high_resolution=exo_base_007a39e0->timers->GetHighResolutionTimer_005e9260();
 InGame* pending_receiver=in_game;
 high_resolution_low_008328f0=static_cast<U32>(high_resolution);
 high_resolution_high_008328f4=static_cast<U32>(static_cast<std::uint64_t>(high_resolution)>>32);
 if(pending_receiver->GetPendingStatusSummary_0062ef70()!=0 && input_mode==0 && field_288==0 &&
    in_game->field_b98==0 && in_game->field_b4==0 &&
    in_game->IsGlobalFading_0062ac60()==0 && in_game->IsGlobalFadeObscuring_0062ded0()==0) {
  // Inline x87 predicates retain the original unordered/parity behavior.
  I32 show_status=0;
  __asm {
   mov ecx,this
   fld dword ptr [ecx+370h]
   fcomp dword ptr zero_0073d700
   fnstsw ax
   test ah,44h
   jp countdown_nonzero
   mov dword ptr [ecx+370h],3e800000h
   jmp status_done
  countdown_nonzero:
   fld dword ptr frame_delta_0078e574
   fcomp dword ptr threshold_0073edf0
   fnstsw ax
   test ah,5
   jp countdown_compare
   fld dword ptr [ecx+370h]
   fsub dword ptr frame_delta_0078e574
   fstp dword ptr [ecx+370h]
  countdown_compare:
   fld dword ptr [ecx+370h]
   fcomp dword ptr zero_0073d700
   fnstsw ax
   test ah,41h
   jp status_done
   mov show_status,1
  status_done:
  }
  if(show_status) {
   InGame* show_receiver=in_game;
   status_countdown=0.0f;
   show_receiver->ShowStatusSummary_0062ef90();
  }
 } else {
  __asm {
   mov ecx,this
   fld dword ptr [ecx+370h]
   fcomp dword ptr zero_0073d700
   fnstsw ax
   test ah,44h
   jnp status_clear_done
   mov dword ptr [ecx+370h],0
  status_clear_done:
  }
 }
 gui->Update_0040ce70(frame_delta_0078e574);
 gui->saved_delta(frame_delta_0078e574);
 ticks=app_manager_007a39fc->app->client->timer->GetSnapshotTimeDifference_004adc80();
 I32 show_mouse=0;
 __asm {
  fild qword ptr ticks
  fmul dword ptr scale_007439bc
  fstp dword ptr frame_delta_0078e574
  mov ecx,this
  fld dword ptr [ecx+3dch]
  fcomp dword ptr zero_0073d700
  fnstsw ax
  test ah,41h
  jne mouse_done
  fld dword ptr [ecx+3dch]
  fsub dword ptr frame_delta_0078e574
  fst dword ptr [ecx+3dch]
  fcomp dword ptr zero_0073d700
  fnstsw ax
  test ah,41h
  jp mouse_done
  mov show_mouse,1
 mouse_done:
 }
 if(show_mouse) ShowMouse_0061fa00(0);
 gui->Draw_0040cc50(frame_delta_0078e574);
 return 0x00603d26; // explicit stopping frontier, not historical EAX
}
}
