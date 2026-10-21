#include "6b477b16ff-w3_trigger_render_internal.h"
#include "d8257ab55f-w3_trigger_numeric.h"
using namespace trigger_render;
extern "C" I __cdecl polyhit_004ab9d0(const kotor_scene3d::Vector*,I,const kotor_scene3d::Vector*,const kotor_scene3d::Vector*,kotor_scene3d::Vector*,kotor_scene3d::Vector*);
namespace {
const kotor_scene3d::Vector* cv(const void* p){return static_cast<const kotor_scene3d::Vector*>(p);}
kotor_scene3d::Vector* mv(void* p){return static_cast<kotor_scene3d::Vector*>(p);}
}
extern "C" void __fastcall w3_trigger_part_triangulate_0049df10(void* self,void*,List indices){
 ListCleanup indices_cleanup{&indices};
 while(ld<I>(&indices,4)>2){
  const U count=ld<U>(&indices,4);U minimum=0x49742400u,pivot=0xffffffffu;
  for(U i=0;signed_word(i)<signed_word(count);++i){
   const U candidate=ld<U>(ld<void*>(self,0x74),element(&indices,i)*12u);
   if(((trigger_numeric::compare(&minimum,&candidate)>>8)&0x41u)==0){pivot=i;minimum=candidate;}
  }
  const U previous=static_cast<U>(signed_word(pivot+count-1u)%signed_word(count));
  const U next=static_cast<U>(signed_word(pivot+count+1u)%signed_word(count));
  const U pivot_vertex=element(&indices,pivot),next_vertex=element(&indices,next);
  U previous_vertex=element(&indices,previous);
  List triangle{};ListCleanup triangle_cleanup{&triangle};U vertex[3];
  copy_vertex(self,previous_vertex,vertex);append_vector(&triangle,vertex);
  copy_vertex(self,pivot_vertex,vertex);append_vector(&triangle,vertex);
  copy_vertex(self,next_vertex,vertex);append_vector(&triangle,vertex);
  for(U i=0;signed_word(i)<ld<I>(&triangle,4);++i)st<U>(ld<void*>(&triangle,0),i*12u+8u,0);
  U selected=0xffffffffu,best_x=0x49742400u;
  for(U i=0;signed_word(i)<signed_word(count);++i){
   const U index=element(&indices,i);
   if(index==previous_vertex||index==pivot_vertex||index==next_vertex)continue;
   U from[3],to[3],hit[3],normal[3]={0,0,0};copy_vertex(self,index,from);
   to[0]=from[0];to[1]=from[1];to[2]=0xc47a0000u;from[2]=0x447a0000u;
   if(!polyhit_004ab9d0(cv(ld<void*>(&triangle,0)),3,cv(from),cv(to),mv(hit),mv(normal)))continue;
   const U mask=(trigger_numeric::compare(&from[0],&best_x)>>8)&5u;
   if(mask==1u||mask==4u){selected=i;best_x=from[0];}
  }
  U marker=0xffffffffu;
  if(signed_word(selected)>=0){
   previous_vertex=element(&indices,selected);
   if(pivot==count-1u)append_word(&indices,previous_vertex);
   else{
    const U insertion=pivot+1u;reserve_words(&indices);
    void* const data=ld<void*>(&indices,0);st<U>(data,count*4u,previous_vertex);st<U>(&indices,4,count+1u);
    for(U i=count;signed_word(i)>signed_word(insertion);--i)st<U>(data,i*4u,ld<U>(data,(i-1u)*4u));
    st<U>(data,insertion*4u,previous_vertex);marker=previous_vertex;
   }
  }else erase_word(&indices,pivot);
  copy_vertex(self,previous_vertex,vertex);append_vector(list(self,0xd0),vertex);
  copy_vertex(self,pivot_vertex,vertex);append_vector(list(self,0xd0),vertex);
  copy_vertex(self,next_vertex,vertex);append_vector(list(self,0xd0),vertex);
  if(signed_word(marker)>=0){
   U position=0;while(signed_word(position)<ld<I>(&indices,4)&&element(&indices,position)!=marker)++position;
   if(signed_word(position)<ld<I>(&indices,4)){
    List split{};ListCleanup split_cleanup{&split};const U remove_position=position+1u;
    for(;;){
     const U value=element(&indices,remove_position);append_word(&split,value);erase_word(&indices,remove_position);
     if(value==marker)break;
    }
    List copy;int_list_copy_construct_0046d250(&copy,nullptr,&split);
    w3_trigger_part_triangulate_0049df10(self,nullptr,copy);
   }
  }
 }
}
