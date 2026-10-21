#include "cf5dc17c21-object_vfx_mirv.h"
#include "e65f8674fd-object_projectile.h"
#include "a4bf971e06-startup_string.h"
#include "86c40a8afc-resources.h"
#include "cca80f7c35-m15e.h"
#include "b5c61133a1-scene3d.h"
#include <cstring>

using U=std::uint32_t;using H=std::uint16_t;using B=std::uint8_t;
using String=kotor_reconstruction::StartupString;
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;}
extern "C" {
void* __fastcall kotor_fastcall_005ed580(void*,void*,U);
void* __fastcall kotor_fastcall_005ed530(void*,void*);
String* __fastcall GuiAssign005e5140(String*,void*,const char*);
menu_resources::Ref* __fastcall async_ref_construct_string_00406d60(menu_resources::Ref*,void*,String*);
U __cdecl aurora_vector_equal_004aa980(const void*,const void*);
float* __cdecl kotor_cdecl_00423bc0(float*,const float*,const float*);
void* __cdecl live_operator_new(U);
void __cdecl live_free(void*);
void __cdecl genuine_x87_ftol_006fae8c();
}
namespace {
template<class T>T rd(const void*p,U o){T v;std::memcpy(&v,static_cast<const B*>(p)+o,sizeof(v));return v;}
template<class T>void wr(void*p,U o,T v){std::memcpy(static_cast<B*>(p)+o,&v,sizeof(v));}
void* ptr(const void*p,U o){return rd<void*>(p,o);}
template<class F>F slot(void*p,U o){return rd<F>(ptr(p,0),o);}
template<class F,class...A>auto invoke(F f,A...a){F volatile target=f;return target(a...);}
U word(const void*p){return static_cast<U>(reinterpret_cast<std::uintptr_t>(p));}
U output_word(void*p){return static_cast<U>(reinterpret_cast<std::uintptr_t>(p));}
using ObjectMethods=KotorM15ObjectMethods_007516c8;
using View=decltype(ObjectMethods::s00c_as_creature_or_self);
using Delete=decltype(ObjectMethods::s000_scalar_deleting_destructor);
using Load=decltype(ObjectMethods::s078_load_model);
using Position=decltype(ObjectMethods::s08c_set_position);
using Orientation=decltype(ObjectMethods::s088_set_orientation);
using Scene=decltype(ObjectMethods::s094_attach_to_scene);
using Model=decltype(ObjectMethods::s098_get_model);
using PartPosition=decltype(aurora_model::GobVTable::s098_gob_get_part_position);
void* client(){return ptr(kotor_menu_window_cells::manager_007a39fc,4);}
void* view(void*self,U offset){
 void*manager=kotor_menu_window_cells::manager_007a39fc;
 U id=rd<U>(self,offset);
 void*object=invoke(kotor_fastcall_005ed580,ptr(manager,4),nullptr,id);
 return object?slot<View>(object,0x0c)(object,nullptr):nullptr;
}
void copy_vector(void*destination,const void*source){
 wr<U>(destination,0,rd<U>(source,0));
 wr<U>(destination,4,rd<U>(source,4));
 wr<U>(destination,8,rd<U>(source,8));
}
// 006A44F3..006A453E: keep both cdecl argument areas and raw ST0 alive.
U logarithmic_duration(Vector*delta,const float*from_position,const float*to_position){
 const U three=0x40400000u,two=0x40000000u,thousand=0x447a0000u;
 U result;
 __asm {
  push to_position
  push from_position
  push delta
  call kotor_cdecl_00423bc0
  push eax
  call vector_magnitude_004ab100
  fld st(0)
  fldln2
  add esp,16
  fxch st(1)
  fyl2x
  fmul dword ptr three
  fadd dword ptr two
  fdivr st(0),st(1)
  fmul dword ptr thousand
  call genuine_x87_ftol_006fae8c
  fstp st(0)
  mov result,eax
 }
 return result;
}
}
extern "C" {
std::uint64_t __fastcall object_vfx_duration_0063d010(void*self,void*,const void*point,U alternate){
 Vector delta;U result_lo,result_hi;
 const U speed42=0x42280000u,speed37=0x42140000u,thousand=0x447a0000u;
 __asm {
  mov eax,point
  mov ecx,self
  fld dword ptr [eax]
  fsub dword ptr [ecx+24h]
  fstp dword ptr delta.x
  fld dword ptr [eax+4]
  fsub dword ptr [ecx+28h]
  fstp dword ptr delta.y
  fld dword ptr [eax+8]
  fsub dword ptr [ecx+2ch]
  fstp dword ptr delta.z
  lea eax,delta
  push eax
  call vector_magnitude_004ab100
  add esp,4
  cmp alternate,0
  je duration_default_speed
  fld dword ptr speed37
  jmp duration_divide
 duration_default_speed:
  fld dword ptr speed42
 duration_divide:
  fxch st(1)
  fdiv st(0),st(1)
  fmul dword ptr thousand
  call genuine_x87_ftol_006fae8c
  fstp st(0)
  mov result_lo,eax
  mov result_hi,edx
 }
 return (static_cast<std::uint64_t>(result_hi)<<32)|result_lo;
}
void __fastcall object_vfx_006a4240(void*self,void*,U selector){
 Quaternion orientation{1.0f,0.0f,0.0f,0.0f};
 Vector position{0.0f,0.0f,0.0f};
 String model_name;
 void*source=view(self,0x40);
 void*target=view(self,0x44);
 if(!source||!target)return;
 void*raw=invoke(live_operator_new,0x1c8u);void*projectile=nullptr;
 try{if(raw)projectile=invoke(object_projectile_006d57b0,raw,nullptr);}
 catch(...){invoke(live_free,raw);throw;}
 invoke(object_projectile_006d41b0,projectile,nullptr,0x154u);
 U kind=static_cast<H>(selector+0xfc7cu);
 if(kind==0)invoke(GuiAssign005e5140,&model_name,nullptr,"vpr_magmisl");
 else if(kind==1||kind==2){
  invoke(GuiAssign005e5140,&model_name,nullptr,kind==1?"vpr_arofire":"vpr_aroacid");
  wr<B>(projectile,0x1a1,2);
 }
 alignas(4) menu_resources::Ref reference;
 invoke(async_ref_construct_string_00406d60,&reference,nullptr,&model_name);
 if(!slot<Load>(projectile,0x78)(projectile,nullptr,&reference,255,0)){
  slot<Delete>(projectile,0)(projectile,nullptr,1);return;
 }
 void*model=slot<Model>(target,0x98)(target,nullptr,255,1);
 if(!slot<PartPosition>(model,0x98)(static_cast<aurora_model::Gob*>(model),nullptr,
    word("handconjure"),output_word(&position),output_word(&orientation)))
  copy_vector(&position,static_cast<B*>(target)+0x24);
 slot<Position>(projectile,0x8c)(projectile,nullptr,&position.x);
 slot<Orientation>(projectile,0x88)(projectile,nullptr,reinterpret_cast<const float*>(static_cast<B*>(target)+0x30));
 void*module=invoke(kotor_fastcall_005ed530,client(),nullptr);
 if(module){void*area=ptr(module,0x48);if(area)
  slot<Scene>(projectile,0x94)(projectile,nullptr,ptr(area,0x184));}
 invoke(object_projectile_006d4250,projectile,nullptr,1u);
 Vector zero{0.0f,0.0f,0.0f};
 if(invoke(aurora_vector_equal_004aa980,static_cast<const void*>(static_cast<B*>(self)+0x34),static_cast<const void*>(&zero))){
  copy_vector(static_cast<B*>(projectile)+0x114,static_cast<B*>(source)+0x24);
  String empty("");
  invoke(object_projectile_006d42d0,projectile,nullptr,rd<U>(source,4),&empty);
 }else copy_vector(static_cast<B*>(projectile)+0x114,static_cast<B*>(self)+0x34);
 if(kind==2){
  U duration=static_cast<U>(invoke(object_vfx_duration_0063d010,projectile,nullptr,
      static_cast<const void*>(static_cast<B*>(source)+0x24),0u));
  invoke(object_projectile_006d2c30,projectile,nullptr,duration<<1);
  invoke(object_projectile_006d6ee0,projectile,nullptr,1u);
 }else if(kind==0||kind==1){
  Vector delta;
  U duration=invoke(logarithmic_duration,&delta,reinterpret_cast<const float*>(static_cast<B*>(source)+0x24),
      reinterpret_cast<const float*>(static_cast<B*>(target)+0x24));
  invoke(object_projectile_006d2c30,projectile,nullptr,duration);
  invoke(object_projectile_006d6ee0,projectile,nullptr,4u);
 }
}
}
