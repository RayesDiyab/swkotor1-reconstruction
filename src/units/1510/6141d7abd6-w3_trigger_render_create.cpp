#include "6b477b16ff-w3_trigger_render_internal.h"
#include "f973c751a2-w3_trigger_storage.h"
using namespace trigger_render;
extern "C" {
kotor_scene3d::Vector* __cdecl vector_min_004aae70(kotor_scene3d::Vector*,kotor_scene3d::Vector,kotor_scene3d::Vector);
kotor_scene3d::Vector* __cdecl vector_max_004aaee0(kotor_scene3d::Vector*,kotor_scene3d::Vector,kotor_scene3d::Vector);
I __cdecl scene_bsp_add_part_0046e290(void*,void*,aurora_model::MaxTree*);
}
namespace {
struct PendingAllocation {void* value;~PendingAllocation() noexcept(false){if(value)live_free(value);}};
void epsilon_add(const void* input,void* output){const U epsilon=0x3c23d70au;const U* constant=&epsilon;
 __asm{
  mov ecx,input
  fld dword ptr [ecx]
  mov edx,constant
  fadd dword ptr [edx]
  mov ecx,output
  fstp dword ptr [ecx]
 }
}
kotor_scene3d::Vector vector_value(const void* source){kotor_scene3d::Vector result;std::memcpy(&result,source,12);return result;}
void copy_three(void* destination,const void* source){
 st<U>(destination,0,ld<U>(source,0));st<U>(destination,4,ld<U>(source,4));st<U>(destination,8,ld<U>(source,8));
}
void release_hit(void* info){
 if(!info)return;
 if(void* reference=ld<void*>(info,0x38))static_cast<List*>(at(reference,4))->RemoveReverse32(word(at(info,0x38)));
 if(void* data=ld<void*>(info,8))live_free(data);
 live_free(info);
}
}
extern "C" void* __cdecl recovered_aur_part_trigger_create_0049e630(void* scene,void* points,U count,U flag,void* orientation,void* trigger){
 void* part=nullptr;
 {PendingAllocation memory{live_operator_new(0xdc)};
  if(memory.value)part=w3_trigger_part_construct_0049db30(memory.value,nullptr,static_cast<aurora_model::MdlNode*>(static_cast<void*>(w3_trigger_node_008304c0)));
  memory.value=nullptr;
 }
 void* info=nullptr;
 {PendingAllocation memory{live_operator_new(0x64)};
  if(memory.value)info=aurora_hit_info_construct_004578f0(memory.value,nullptr,0,0xbfffffffu,0);
  memory.value=nullptr;
 }
 void* point=points;
 for(U remaining=count;signed_word(remaining)>0;--remaining,point=at(point,12)){
  if(!flag){
   U from[3]={ld<U>(point,0),ld<U>(point,4),0x447a0000u};
   U to[3]={from[0],from[1],0xc1300000u};
   aurora_hit_info_clear_output_00456a60(info,nullptr);st<U>(info,0,1);
   if(!ld<const kotor_scene3d::SceneVTable*>(scene,0)->s038_scene_hit_check(scene,nullptr,vector_value(from),vector_value(to),static_cast<kotor_scene3d::Vector*>(point),info)){
    epsilon_add(&to[0],&to[0]);epsilon_add(&to[1],&to[1]);epsilon_add(&from[0],&from[0]);epsilon_add(&from[1],&from[1]);
    ld<const kotor_scene3d::SceneVTable*>(scene,0)->s038_scene_hit_check(scene,nullptr,vector_value(from),vector_value(to),static_cast<kotor_scene3d::Vector*>(point),info);
   }
   epsilon_add(at(point,8),at(point,8));
  }
  U value[3]={ld<U>(point,0),ld<U>(point,4),ld<U>(point,8)};append_vector(list(part,0x74),value);
 }
 for(U axis=0;axis<3;++axis)st<U>(part,0x68+axis*4u,0xc7c35000u);
 for(U axis=0;axis<3;++axis)st<U>(part,0x5c+axis*4u,0x47c35000u);
 for(U index=0;signed_word(index)<ld<I>(part,0x78);++index){
  kotor_scene3d::Vector output;
  const void* result=vector_min_004aae70(&output,vector_value(at(part,0x5c)),vector_value(at(ld<void*>(part,0x74),index*12u)));
  copy_three(at(part,0x5c),result);
  result=vector_max_004aaee0(&output,vector_value(at(part,0x68)),vector_value(at(ld<void*>(part,0x74),index*12u)));
  copy_three(at(part,0x68),result);
 }
 for(U offset=0;offset<16;offset+=4)st<U>(part,0x4c+offset,ld<U>(orientation,offset));
 st<void*>(part,0xa4,trigger);
 List indices{};ListCleanup indices_cleanup{&indices};
 for(U index=0;signed_word(index)<signed_word(count);++index)append_word(&indices,index);
 List copy;int_list_copy_construct_0046d250(&copy,nullptr,&indices);
 w3_trigger_part_triangulate_0049df10(part,nullptr,copy);
 if(ld<void*>(scene,0xa0))scene_bsp_add_part_0046e290(scene,part,nullptr);
 else w3_trigger_part_add_scene_0046d130(scene,part);
 release_hit(info);
 return part;
}
