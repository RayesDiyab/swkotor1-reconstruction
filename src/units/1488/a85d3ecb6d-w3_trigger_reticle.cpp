#include "a43a781a46-w3_trigger_internal.h"
#include "d8257ab55f-w3_trigger_numeric.h"
#include "cc436f6baa-m15e_animbase.h"
#include "94d6ff964a-scene3d.h"
#include "85efbde20b-model_vtables.hpp"
using namespace trigger_private;
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;}
extern "C" {
void* __fastcall kotor_fastcall_005ed560(void*,void*);
void* __fastcall kotor_fastcall_005eda40(void*,void*);
kotor_scene3d::Vector* __fastcall quaternion_zdir_0043e250(const kotor_scene3d::Quaternion*,void*,kotor_scene3d::Vector*);
}
namespace {
void* client(){return ld<void*>(const_cast<void*>(kotor_menu_window_cells::manager_007a39fc),4);}
const kotor_scene3d::CameraVTable* camera_table(void* p){return ld<const kotor_scene3d::CameraVTable*>(p,0);}
void* project(void* camera,const void* point,kotor_scene3d::Vector* storage){
 const U x=ld<U>(point,0),y=ld<U>(point,4),z=ld<U>(point,8);
 return ptr(camera_table(camera)->s028_camera_map_to_screen(camera,nullptr,word(storage),x,y,z,1));
}
bool parity_even(U mask){return mask==0||mask==5;}
}
extern "C" std::uint8_t __fastcall w3_trigger_006916b0(void* self,void*,Vec* output) noexcept(false){
 void* const camera=kotor_fastcall_005ed560(client(),nullptr);
 if(!camera)return 0;
 kotor_scene3d::Vector camera_position;
 camera_table(camera)->s020_camera_get_position(camera,nullptr,&camera_position);
 kotor_scene3d::Quaternion quaternion;
 const void* const quaternion_result=ptr(camera_table(camera)->s024_camera_get_orientation(camera,nullptr,word(&quaternion)));
 trigger_numeric::Direction direction;trigger_numeric::negative_forward(quaternion_result,&direction);
 U minimum[3]={0x7f7fffff,0x7f7fffff,0x7f7fffff};U maximum[3]={0xff7fffff,0xff7fffff,0xff7fffff};
 const I count=ld<I>(self,0x130);
 if(count>0){
  void* const vertices=ld<void*>(self,0x134);U displacement=0;
  for(I i=0;i<count;++i,displacement+=12){
   void* const vertex=at(vertices,displacement);
   if(parity_even((trigger_numeric::compare(&minimum[0],vertex)>>8)&5u))minimum[0]=ld<U>(vertex,0);
   if(((trigger_numeric::compare(at(vertex,4),&minimum[1])>>8)&0x41u)!=0)trigger_numeric::float_copy(at(vertex,4),&minimum[1]);
   if(parity_even((trigger_numeric::compare(&minimum[2],at(vertex,8))>>8)&5u))minimum[2]=ld<U>(vertex,8);
   if(((trigger_numeric::compare(&maximum[0],vertex)>>8)&0x41u)!=0)maximum[0]=ld<U>(vertex,0);
   if(parity_even((trigger_numeric::compare(at(vertex,4),&maximum[1])>>8)&5u))trigger_numeric::float_copy(at(vertex,4),&maximum[1]);
   if(((trigger_numeric::compare(&maximum[2],at(vertex,8))>>8)&0x41u)!=0)maximum[2]=ld<U>(vertex,8);
  }
 }
 trigger_numeric::Center center;trigger_numeric::make_center(minimum,maximum,&center);
 const std::uint8_t side=trigger_numeric::center_side(&direction,&center,&camera_position);
 kotor_scene3d::Vector screen_storage;void* const mapped=project(camera,center.xyz,&screen_storage);
 U mapped_copy[3];for(U d=0;d<3;++d)mapped_copy[d]=ld<U>(mapped,d*4);
 void* const manager=kotor_fastcall_005eda40(client(),nullptr);
 trigger_numeric::scale_coordinate(ld<std::int16_t>(manager,0x6c),&mapped_copy[0],&output->x);
 trigger_numeric::scale_coordinate(ld<std::int16_t>(manager,0x6e),&mapped_copy[1],&output->y);
 // Original leaves the caller's Z untouched in the fallback.
 return side;
}
extern "C" std::uint8_t __fastcall recovered_client_trigger_get_reticle_point_00691b00(void* self,void*,void* output_word) noexcept(false){
 Vec* const output=static_cast<Vec*>(output_word);
 void* const animation_base=ld<void*>(self,0x68);
 void* const model=ld<const KotorM15eAnimBaseVtbl*>(animation_base,0)->s008(animation_base,nullptr,0xff);
 if(!model)return w3_trigger_006916b0(self,nullptr,output);
 void* const part=ld<const aurora_model::GobVTable*>(model,0)->s10c_gob_get_part(static_cast<aurora_model::Gob*>(model),nullptr,"lookathook");
 if(!part)return w3_trigger_006916b0(self,nullptr,output);
 kotor_scene3d::Quaternion part_orientation;
 ld<const aurora_vt::VTable_00740ae8*>(part,0)->s00c(part,nullptr,word(output),word(&part_orientation));
 void* camera=kotor_fastcall_005ed560(client(),nullptr);
 kotor_scene3d::Quaternion camera_orientation;kotor_scene3d::Vector forward_storage;
 const auto* orientation=static_cast<const kotor_scene3d::Quaternion*>(ptr(camera_table(camera)->s024_camera_get_orientation(camera,nullptr,word(&camera_orientation))));
 const auto* forward=quaternion_zdir_0043e250(orientation,nullptr,&forward_storage);
 kotor_scene3d::Vector negative_forward;trigger_numeric::negative_vector(forward,&negative_forward);
 kotor_scene3d::Vector camera_position;
 const auto* camera_position_result=camera_table(camera)->s020_camera_get_position(camera,nullptr,&camera_position);
 const std::uint8_t side=trigger_numeric::point_side(&negative_forward,output,camera_position_result);
 camera=kotor_fastcall_005ed560(client(),nullptr);
 kotor_scene3d::Vector screen_storage;void* const mapped=project(camera,output,&screen_storage);
 // Sequential DWORD copies from the returned pointer, before either manager call.
 for(U d=0;d<3;++d)st<U>(output,d*4,ld<U>(mapped,d*4));
 void* manager=kotor_fastcall_005eda40(client(),nullptr);
 trigger_numeric::scale_coordinate(ld<std::int16_t>(manager,0x6c),&output->x,&output->x);
 manager=kotor_fastcall_005eda40(client(),nullptr);
 trigger_numeric::scale_coordinate(ld<std::int16_t>(manager,0x6e),&output->y,&output->y);
 return side;
}
