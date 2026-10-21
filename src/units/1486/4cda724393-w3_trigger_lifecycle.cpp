// /EHs without EHc: allthree original unwindstates remain active across C-linkage calls.
#include "a43a781a46-w3_trigger_internal.h"
#include "cca80f7c35-m15e.h"
#include "cc436f6baa-m15e_animbase.h"
#include "e1fb1def77-startup_string.h"
#include "0b7d27aef1-w3_animation.h"
using namespace trigger_private;
namespace c2da_current {using String=kotor_reconstruction::StartupString;}
extern "C" {
void __fastcall c2da_current_string_destroy_005e5c20(c2da_current::String*,void*);
void __cdecl live_free(void*);
extern const void* const recovered_client_trigger_methods_00754440[82];
}
namespace {
struct BaseCleanup {void* self;~BaseCleanup() noexcept(false){kotor_m15e_object_destruct_0063e800(self,nullptr);}};
struct StringCleanup {void* self;~StringCleanup() noexcept(false){c2da_current_string_destroy_005e5c20(static_cast<c2da_current::String*>(self),nullptr);}};
}
extern "C" void __fastcall w3_trigger_006912f0(void* self,void*) noexcept(false){
 BaseCleanup base{self};StringCleanup string100{at(self,0x100)};StringCleanup string140{at(self,0x140)};
 st<const void*>(self,0,recovered_client_trigger_methods_00754440);
 void* model=ld<void*>(self,0x68);
 if(model){
  ld<const KotorM15eAnimBaseVtbl*>(model,0)->s078(model,nullptr);
  model=ld<void*>(self,0x68);
  if(model)ld<const KotorM15eAnimBaseVtbl*>(model,0)->s000(model,nullptr,1);
  st<void*>(self,0x68,nullptr);
 }
 void* vertices=ld<void*>(self,0x134);
 if(vertices){live_free(vertices);st<void*>(self,0x134,nullptr);}
 void* bullet=ld<void*>(self,0x138);
 if(bullet){w3_bullet_die_00670b20(bullet);st<void*>(self,0x138,nullptr);}
 // Automatic cleanup order is +140,+100,base, including propagation from earliercalls.
}
extern "C" void* __fastcall recovered_client_trigger_scalar_destructor_00691ca0(void* self,void*,U flags) noexcept(false){
 w3_trigger_006912f0(self,nullptr);if(flags&1u)live_free(self);return self;
}
