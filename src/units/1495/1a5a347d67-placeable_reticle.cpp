#include "5b8ffa03d2-placeable_internal.h"
using namespace placeable_private;
namespace trigger_numeric {
void negative_vector(const void*,void*);
void scale_coordinate(I,const void*,void*);
}
extern "C" {
void* __fastcall kotor_fastcall_005ed560(void*,void*);
void* __fastcall kotor_fastcall_005eda40(void*,void*);
void* __fastcall quaternion_zdir_0043e250(const void*,void*,void*);
B __fastcall kotor_fastcall_0063d560(void*,void*,void*);
}
namespace {
B point_side(const void* direction,const void* point,const void* camera){std::uint16_t status;
 __asm {
  mov ecx,direction
  mov edx,point
  fld dword ptr [ecx+8]
  fmul dword ptr [edx+8]
  fld dword ptr [ecx+4]
  fmul dword ptr [edx+4]
  faddp st(1),st(0)
  fld dword ptr [ecx]
  fmul dword ptr [edx]
  faddp st(1),st(0)
  mov edx,camera
  fld dword ptr [ecx+8]
  fmul dword ptr [edx+8]
  fld dword ptr [ecx+4]
  fmul dword ptr [edx+4]
  faddp st(1),st(0)
  fld dword ptr [ecx]
  fmul dword ptr [edx]
  faddp st(1),st(0)
  fcompp
  fnstsw ax
  mov status,ax
 }
 const U mask=(status>>8)&5u;return mask==1||mask==4?1:2;
}
}
extern "C" B __fastcall recovered_client_placeable_get_reticle_point_00682ff0(void* self,void*,void* output){
 void* server=nullptr;
 if(kotor_m15a_object_get_server_object_0063d4b0(self,nullptr)){
  void* const owner=kotor_m15a_object_get_server_object_0063d4b0(self,nullptr);
  server=slot<Ptr>(owner,0x48)(owner,nullptr);
 }
 if(!server||!ptr(server,0x44C))return kotor_fastcall_0063d560(self,nullptr,output);
 using Model=void*(__fastcall*)(void*,void*,U);
 using Part=void*(__fastcall*)(void*,void*,const char*);
 void* const animation_base=ptr(self,0x68);
 void* const model=slot<Model>(animation_base,8)(animation_base,nullptr,0xFF);
 if(!model)return kotor_fastcall_0063d560(self,nullptr,output);
 void* const part=slot<Part>(model,0x10C)(model,nullptr,"rootdummy");
 if(!part)return kotor_fastcall_0063d560(self,nullptr,output);
 using Transform=void(__fastcall*)(void*,void*,void*,void*);
 float part_orientation[4]={1,0,0,0};slot<Transform>(part,0x0C)(part,nullptr,output,part_orientation);
 void* const camera=kotor_fastcall_005ed560(client(),nullptr);
 using Out=void*(__fastcall*)(void*,void*,void*);
 float camera_orientation[4],forward[3],negative[3],camera_position[3];
 void* const orientation=slot<Out>(camera,0x24)(camera,nullptr,camera_orientation);
 void* const direction=quaternion_zdir_0043e250(orientation,nullptr,forward);
 trigger_numeric::negative_vector(direction,negative);
 void* const position=slot<Out>(camera,0x20)(camera,nullptr,camera_position);
 const B side=point_side(negative,output,position);
 using Project=void*(__fastcall*)(void*,void*,void*,U,U,U,U);
 float screen[3];
 void* const mapped=slot<Project>(camera,0x28)(camera,nullptr,screen,rd<U>(output,0),rd<U>(output,4),rd<U>(output,8),1);
 for(U i=0;i<3;++i)wr<U>(output,i*4,rd<U>(mapped,i*4));
 void* manager=kotor_fastcall_005eda40(client(),nullptr);
 trigger_numeric::scale_coordinate(rd<std::int16_t>(manager,0x6C),output,output);
 manager=kotor_fastcall_005eda40(client(),nullptr);
 trigger_numeric::scale_coordinate(rd<std::int16_t>(manager,0x6E),at(output,4),at(output,4));
 return side;
}
