#pragma once
#include "a43a781a46-w3_trigger_internal.h"
#include "eccf27eea7-w3_trigger_render.h"
#include "c5812aaca1-model_instance.hpp"
namespace trigger_render {
using namespace trigger_private;using List=kotor_reconstruction::ArrayMutations;
inline I signed_word(U u){I i;std::memcpy(&i,&u,4);return i;}
inline List* list(void* p,U offset){return static_cast<List*>(at(p,offset));}
}
extern "C" {
void __fastcall vector_list_allocate_00421310(trigger_render::List*,void*,trigger_private::I);
void __cdecl live_free(void*);
void* __cdecl live_operator_new(std::size_t);
}
namespace trigger_render {
inline void append_vector(List* target,const U* value){
 const U capacity=ld<U>(target,8);
 if(ld<U>(target,4)==capacity)vector_list_allocate_00421310(target,nullptr,signed_word(capacity?capacity*2u:8u));
 const U size=ld<U>(target,4);void* const item=at(ld<void*>(target,0),size*12u);
 st<U>(item,0,value[0]);st<U>(item,4,value[1]);st<U>(item,8,value[2]);st<U>(target,4,size+1u);
}
}

namespace trigger_render {
struct ListCleanup {
 List* value;
 ~ListCleanup() noexcept(false){if(void* p=ld<void*>(value,0))live_free(p);}
};
inline void reserve_words(List* target){
 const U count=ld<U>(target,4),capacity=ld<U>(target,8);
 if(count!=capacity)return;
 const U grown=capacity?capacity*2u:8u;void* const old=ld<void*>(target,0);
 st<U>(target,8,grown);void* const data=live_operator_new(grown*4u);st<void*>(target,0,data);
 for(U i=0;signed_word(i)<signed_word(count);++i)st<U>(data,i*4u,ld<U>(old,i*4u));
 if(old)live_free(old);
}
inline void append_word(List* target,U value){reserve_words(target);const U n=ld<U>(target,4);st<U>(ld<void*>(target,0),n*4u,value);st<U>(target,4,n+1u);}
inline U element(const List* target,U index){return ld<U>(ld<void*>(target,0),index*4u);}
inline void erase_word(List* target,U index){
 const U last=ld<U>(target,4)-1u;st<U>(target,4,last);
 for(U i=index;signed_word(i)<signed_word(last);++i)st<U>(ld<void*>(target,0),i*4u,element(target,i+1u));
}
inline void copy_vertex(void* self,U index,U* value){
 void* const point=at(ld<void*>(self,0x74),index*12u);
 value[0]=ld<U>(point,0);value[1]=ld<U>(point,4);value[2]=ld<U>(point,8);
}
}
