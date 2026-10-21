#include "6b477b16ff-w3_trigger_render_internal.h"
#include "d8257ab55f-w3_trigger_numeric.h"
using namespace trigger_private;
namespace {
kotor_scene3d::Vector* bounds(kotor_scene3d::Vector* output,const kotor_reconstruction::ArrayMutations* source,bool maximum){
 void* const points=ld<void*>(source,0);const I count=ld<I>(source,4);
 U result[3]={ld<U>(points,0),ld<U>(points,4),ld<U>(points,8)};
 // The original reads point zero even for a zero/negative count and revisits it when count>0.
 for(I index=0;index<count;++index){
  for(U axis=0;axis<3;++axis){
   void* const point=at(points,static_cast<U>(index)*12u+axis*4u);
   const U status=trigger_numeric::compare(&result[axis],point)>>8;
   const U mask=status&5u;
   if(maximum?((status&0x41u)!=0):(mask==0||mask==5))result[axis]=ld<U>(point,0);
  }
 }
 for(U axis=0;axis<3;++axis)st<U>(output,axis*4u,result[axis]);return output;
}
}
extern "C" kotor_scene3d::Vector* __cdecl w3_trigger_vector_list_max_004aafc0(kotor_scene3d::Vector* output,const kotor_reconstruction::ArrayMutations* source){return bounds(output,source,true);}
extern "C" kotor_scene3d::Vector* __cdecl w3_trigger_vector_list_min_004ab060(kotor_scene3d::Vector* output,const kotor_reconstruction::ArrayMutations* source){return bounds(output,source,false);}
