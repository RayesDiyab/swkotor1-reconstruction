#include "6b477b16ff-w3_trigger_render_internal.h"
using namespace trigger_private;
extern "C" {
struct W3TriggerPartMethods;
extern const W3TriggerPartMethods w3_trigger_part_methods_007436d0;
void __cdecl scene_remove_from_scene_manager_004554b0(void*);
void __fastcall part_destruct_00440140(void*,void*);
}
namespace {
struct BaseCleanup {void* self;~BaseCleanup() noexcept(false){part_destruct_00440140(self,nullptr);}};
struct ArrayCleanup {void* self;U offset;~ArrayCleanup() noexcept(false){void* const data=ld<void*>(self,offset);if(data)live_free(data);}};
}
extern "C" void __fastcall w3_trigger_part_destruct_0049d8e0(void* self,void*){
 st<const void*>(self,0,&w3_trigger_part_methods_007436d0);
 BaseCleanup base{self};ArrayCleanup points{self,0x74},walls{self,0x80},top{self,0x8c},bottom{self,0x98},triangles{self,0xd0};
 // Original state5 is active before SceneRemove. Normal/unwind cleanup frees fields without clearing.
 scene_remove_from_scene_manager_004554b0(self);
}
extern "C" void* __fastcall w3_trigger_part_scalar_0049db10(void* self,void*,U flags){
 w3_trigger_part_destruct_0049d8e0(self,nullptr);if(flags&1u)live_free(self);return self;
}
