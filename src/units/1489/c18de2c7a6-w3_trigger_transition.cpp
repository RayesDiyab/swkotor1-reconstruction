#include "a43a781a46-w3_trigger_internal.h"
#include "d8257ab55f-w3_trigger_numeric.h"
#include "0b7d27aef1-w3_animation.h"
#include "777e6bd68b-area_region_glue.h"
#include "cca80f7c35-m15e.h"
#include "94d6ff964a-scene3d.h"
#include "c5812aaca1-model_instance.hpp"
using namespace trigger_private;
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;}
extern "C" {
U __fastcall frame_R4_005ed690(void*,void*);
void* __fastcall client_get_player_creature_005ed540(void*,void*);
void* __fastcall kotor_fastcall_005ed560(void*,void*);
void* __fastcall frame_GetServerArea_00605640(void*,void*);
void __fastcall frame_R4_0062b020(void*,void*,U,U*);
float* __cdecl kotor_cdecl_00423bc0(float*,const float*,const float*);
kotor_scene3d::Vector* __fastcall quaternion_ydir_0044f420(const kotor_scene3d::Quaternion*,void*,kotor_scene3d::Vector*);
}
namespace {
void* client(){return ld<void*>(const_cast<void*>(kotor_menu_window_cells::manager_007a39fc),4);}
struct alignas(4) HitStorage {unsigned char bytes[0x64];};
struct HitCleanup {void* value;~HitCleanup() noexcept(false){chit_info_destruct_00456b30(value,nullptr);}};
// The X product is rounded to float before the addition. The Y product remains extended.
void segment_end(const void* direction,const void* position,void* destination){
 const float eight=8.0f;const float* factor=&eight;float x_product;float* product=&x_product;
 __asm{
  mov ecx,direction
  mov edx,factor
  fld dword ptr [ecx+4]
  fmul dword ptr [edx]
  fld dword ptr [ecx]
  fmul dword ptr [edx]
  mov edx,product
  fstp dword ptr [edx]
  mov ecx,position
  fadd dword ptr [ecx+4]
  fld dword ptr [edx]
  fadd dword ptr [ecx]
  mov edx,destination
  fstp dword ptr [edx]
  fstp dword ptr [edx+4]
 }
 st<U>(destination,8,0);
}
void raised_height(const void* source,void* destination){const float one=1.0f;const float* constant=&one;
 __asm{
  mov ecx,source
  fld dword ptr [ecx]
  mov edx,constant
  fadd dword ptr [edx]
  mov ecx,destination
  fstp dword ptr [ecx]
 }
}
bool farther(const void* target_delta,const void* from,const void* hit){std::uint16_t status;
 __asm{
  mov ecx,hit
  mov edx,from
  fld dword ptr [ecx]
  fsub dword ptr [edx]
  fld dword ptr [ecx+4]
  fsub dword ptr [edx+4]
  fld dword ptr [ecx+8]
  fsub dword ptr [edx+8]
  fld st(0)
  fmul st(0),st(1)
  fld st(2)
  fmul st(0),st(3)
  faddp st(1),st(0)
  fld st(3)
  fmul st(0),st(4)
  faddp st(1),st(0)
  mov ecx,target_delta
  fld dword ptr [ecx+8]
  fmul dword ptr [ecx+8]
  fld dword ptr [ecx+4]
  fmul dword ptr [ecx+4]
  faddp st(1),st(0)
  fld dword ptr [ecx]
  fmul dword ptr [ecx]
  faddp st(1),st(0)
  fcompp
  fstp st(0)
  fnstsw ax
  fstp st(0)
  mov status,ax
  fstp st(0)
 }
 return ((status>>8)&0x41u)==0;
}
}
extern "C" void __fastcall w3_trigger_006920b0(void* self,void*) noexcept(false){
 void* const owner=ptr(frame_R4_005ed690(client(),nullptr));
 void* const player=client_get_player_creature_005ed540(client(),nullptr);
 if(!owner||!player||!ld<U>(self,0x10c))return;
 kotor_scene3d::Vector direction={0,0,0};
 if(ld<U>(owner,0xc1c)){
  void* const camera=kotor_fastcall_005ed560(client(),nullptr);
  kotor_scene3d::Quaternion quaternion;
  const auto* table=ld<const kotor_scene3d::CameraVTable*>(camera,0);
  const auto* q=static_cast<const kotor_scene3d::Quaternion*>(ptr(table->s024_camera_get_orientation(camera,nullptr,word(&quaternion))));
  kotor_scene3d::Vector forward;
  const auto* returned=quaternion_ydir_0044f420(q,nullptr,&forward);
  std::memcpy(&direction,returned,12);st<U>(&direction,8,0);
  kotor_scene3d::Vector normalized;
  const auto* result=normalize_004ab180(&normalized,&direction);
  std::memcpy(&direction,result,12);
 }else std::memcpy(&direction,at(player,0x30),12);
 void* const player_position=at(player,0x24);
 Vec from;std::memcpy(&from,player_position,12);
 Vec end;segment_end(&direction,player_position,&end);st<U>(&from,8,0);
 Vec hit={0,0,0};
 if(!w3_trigger_00691a30(self,nullptr,&from,&end,&hit))return;
 void* const model=ld<const KotorM15ObjectMethods_007516c8*>(player,0)->s098_get_model(player,nullptr,0xfe,1);
 if(model){
  kotor_scene3d::Vector position;
  const void* result=ptr(ld<const aurora_model::GobVTable*>(model,0)->s064_gob_get_position(static_cast<aurora_model::Gob*>(model),nullptr,word(&position)));
  trigger_numeric::float_copy(at(result,8),&from.z);
 }else raised_height(at(player,0x2c),&from.z);
 std::memcpy(&hit.z,&from.z,4);
 void* const area=ld<void*>(self,0x20);
 if(area){
  HitStorage info;aurora_hit_info_construct_004578f0(&info,nullptr,0,0xbfffffffu,0);
  HitCleanup cleanup{&info};
  void* const scene=ld<void*>(area,0x184);
  void* const server_area=frame_GetServerArea_00605640(area,nullptr);
  st<U>(&info,0,1);
  const auto* table=ld<const kotor_scene3d::SceneVTable*>(scene,0);
  kotor_scene3d::Vector scene_from,scene_hit;
  std::memcpy(&scene_from,&from,12);std::memcpy(&scene_hit,&hit,12);
  if(table->s038_scene_hit_check(scene,nullptr,scene_from,scene_hit,nullptr,&info))return;
  // Both nominal vector families are the same three-float native image. Keep original storage
  // across this mutable native boundary so downstream callbacks can change the final candidate.
  static_assert(sizeof(Vec)==sizeof(kotor_scene3d::Vector)&&alignof(Vec)==alignof(kotor_scene3d::Vector),"shared native vector image");
  auto* const region_from=reinterpret_cast<kotor_scene3d::Vector*>(&from);
  auto* const region_hit=reinterpret_cast<kotor_scene3d::Vector*>(&hit);
  if(server_area&&!recovered_area_region_005086d0(server_area,nullptr,region_from,region_hit,from.z,from.z,0.0f,0x7f000000u,nullptr,1))return;
 }
 kotor_scene3d::Vector area_position;
 const void* const result=w3_area_position_0062ec20(owner,nullptr,&area_position);
 kotor_scene3d::Vector delta;kotor_cdecl_00423bc0(&delta.x,static_cast<const float*>(result),static_cast<const float*>(player_position));
 if(ld<U>(owner,0xc18)!=0x7f000000u&&!farther(&delta,&from,&hit))return;
 frame_R4_0062b020(owner,nullptr,ld<U>(self,4),reinterpret_cast<U*>(&hit));
}
