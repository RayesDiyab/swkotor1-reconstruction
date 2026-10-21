// Original Stick settings constructor and Combat behavior family. Runtime remains held.
#include "391fdbaa0d-camera_restore_lowers.h"
#include "1cbf675c6d-camera_check_position.h"
#include "0cdbed471d-w4_providers.hpp"
#include "fe0daa935e-scene3d.h"
#include "64303ae048-construct3d.h"
#include "9963eab06a-vtables_gen.h"
#include "423f318b3b-vtables2_gen.h"
#include "ead2dcbdd7-m15e.h"
#include "3df78882af-startup_string.h"
#include <cstring>
#include <initializer_list>
using U=std::uint32_t; using B=std::uint8_t;
using V=kotor_scene3d::Vector; using Q=kotor_scene3d::Quaternion;
using String=kotor_reconstruction::StartupString;
struct W4RK4State;
namespace w4cam { extern void* const camera_stick_vtable_0075165c[8]; extern U dialog_camera_debug_008327ec; }
namespace kotor_input_cells_required { extern volatile int word_008338f0; }
namespace kotor_input_cells { extern void* volatile gui_manager_007a39f4; }
namespace options_video { struct Table; struct String; }
namespace menu_tables_required { int __fastcall C2daFloatEntry_00413430(options_video::Table*,void*,int,options_video::String*,float*); }
extern "C" {
extern void* kotor_cell_rules_007a3a28;
void* __fastcall kotor_fastcall_006d10b0(void*,void*);
W4RK4State* __fastcall kotor_fastcall_006d1190(void*,void*,W4RK4State*,W4Vector,float,U,U,U,U);
void __fastcall kotor_fastcall_006d1030(void*,void*,float);
void __fastcall kotor_fastcall_00638070(void*,void*,int);
void __fastcall aurora_texture_safe_pointer_destroy_00422380(void*,void*);
void* __fastcall kotor_fastcall_005ed8b0(void*,void*);
void* __fastcall kotor_fastcall_00635460(void*,void*);
void* __fastcall kotor_fastcall_005ed580(void*,void*,U);
void* __fastcall kotor_fastcall_005ed560(void*,void*);
U __fastcall kotor_fastcall_005edd80(void*,void*);
double __fastcall kotor_fastcall_0061d5b0(void*,void*);
double __fastcall kotor_fastcall_0061d5e0(void*,void*);
double __fastcall kotor_fastcall_0061d610(void*,void*);
double __fastcall kotor_fastcall_0061d640(void*,void*);
double __fastcall kotor_m15e_appearance_get_height_006973b0(void*,void*) noexcept(false);
double __fastcall kotor_m15e_appearance_get_head_height_offset_006974d0(void*,void*) noexcept(false);
V* __fastcall vector_subtract_assign_004a9770(V*,void*,const V*);
float __cdecl kotor_m15a_yaw_004aa0f0(const void*);
double __cdecl quaternion_yaw_004a9f40(const Q*);
Q* __cdecl yaw_pitch_roll_004acac0(Q*,float,float,float);
void __fastcall gui_m48_manager_store_mouse_0040a3d0(void*,void*);
void __fastcall gui_m48_manager_restore_mouse_0040c7d0(void*,void*);
void __fastcall kotor_fastcall_005ee170(void*,void*,U);
void __fastcall kotor_fastcall_005ee180(void*,void*,U);
U __fastcall gui_m48_client_get_input_class_005eda70(void*,void*);
}
namespace camera_restore {
const kotor_scene3d_construct::BehaviorBaseSlots camera_combat_vtable_00757bdc={
 kotor_fastcall_006d18c0,behavior_void_return_004015a0,kotor_fastcall_006d1fc0,
 behavior_void_return_4_0060e760,kotor_fastcall_006d14d0,behavior_null_return_0063e7f0,
 behavior_return_zero_0063e7f0,behavior_return_zero_0063e7f0};
}
namespace {
template<class T> T rd(const void* p,U o) {
 T v;
 if constexpr(sizeof(T)==1) {
  B raw=*reinterpret_cast<const B volatile*>(static_cast<const B*>(p)+o);std::memcpy(&v,&raw,1);
 } else {
  static_assert(sizeof(T)%4==0,"Original DWORD carrier");
  for(U i=0;i<sizeof(T);i+=4) {
   U raw=*reinterpret_cast<const U volatile*>(static_cast<const B*>(p)+o+i);
   std::memcpy(reinterpret_cast<B*>(&v)+i,&raw,4);
  }
 }
 return v;
}
template<class T> void wr(void* p,U o,T v) {
 static_assert(sizeof(T)%4==0,"Original DWORD carrier");
 for(U i=0;i<sizeof(T);i+=4) {
  U raw;std::memcpy(&raw,reinterpret_cast<const B*>(&v)+i,4);
  *reinterpret_cast<U volatile*>(static_cast<B*>(p)+o+i)=raw;
 }
}
void* at(void* p,U o) {return static_cast<B*>(p)+o;}
void* ptr(const void* p,U o) {return rd<void*>(p,o);}
U word(const void* p) {return static_cast<U>(reinterpret_cast<std::uintptr_t>(p));}
void* from_word(U v) {return reinterpret_cast<void*>(static_cast<std::uintptr_t>(v));}
float* fp(void* p,U o) {return static_cast<float*>(at(p,o));}
template<class F,class... A> auto opaque(F f,A... a) {F volatile target=f;return target(a...);}
void* app() {return ptr(kotor_menu_window_cells::manager_007a39fc,4);}
const aurora_model::GobVTable* gob_vt(void* p) {return *static_cast<const aurora_model::GobVTable* const volatile*>(p);}
aurora_model::Gob* gob(void* p) {return static_cast<aurora_model::Gob*>(p);}
const kotor_scene3d::CameraVTable* cam_vt(void* p) {return *static_cast<const kotor_scene3d::CameraVTable* const volatile*>(p);}
V* get_position(void* p,V* out) {return static_cast<V*>(from_word(gob_vt(p)->s064_gob_get_position(gob(p),nullptr,word(out))));}
Q* get_orientation(void* p,Q* out) {return static_cast<Q*>(from_word(gob_vt(p)->s068_gob_get_orientation(gob(p),nullptr,word(out))));}
void set_orientation(void* p,Q q,Q* out) {
 (void)gob_vt(p)->s060_gob_set_orientation(gob(p),nullptr,word(out),rd<U>(&q,0),rd<U>(&q,4),rd<U>(&q,8),rd<U>(&q,12));
}
void set_position(void* p,V v,V* out) {(void)gob_vt(p)->s05c_gob_set_position(gob(p),nullptr,out,v);}
void* actor_view(void* p) {
 const auto* methods=*static_cast<const KotorM15ObjectMethods_007516c8* const volatile*>(p);
 return methods->s00c_as_creature_or_self(p,nullptr);
}
struct ConstructorUnwind {
 void* self;unsigned active=0;
 ~ConstructorUnwind() {
  if(active>=3)aurora_texture_safe_pointer_destroy_00422380(at(self,0x18),nullptr);
  if(active>=2)aurora_texture_safe_pointer_destroy_00422380(at(self,0x14),nullptr);
  if(active>=1)kotor_fastcall_006db4e0(self,nullptr);
 }
};
// Explicit x87 graphs keep the original intermediate order and DWORD stores.
void axis_z(const Q* q,V* out) {
 const float one=1.0f;
 __asm {
  mov eax,q
  mov edx,out
  fld dword ptr[eax+12]
  fmul dword ptr[eax+4]
  fld dword ptr[eax+8]
  fmul dword ptr[eax]
  faddp st(1),st(0)
  fadd st(0),st(0)
  fstp dword ptr[edx]
  fld dword ptr[eax+12]
  fmul dword ptr[eax+8]
  fld dword ptr[eax+4]
  fmul dword ptr[eax]
  fsubp st(1),st(0)
  fadd st(0),st(0)
  fstp dword ptr[edx+4]
  fld dword ptr[eax+8]
  fmul dword ptr[eax+8]
  fld dword ptr[eax+4]
  fmul dword ptr[eax+4]
  faddp st(1),st(0)
  fadd st(0),st(0)
  fsubr one
  fstp dword ptr[edx+8]
 }
}
void axis_y(const Q* q,V* out) {
 const float one=1.0f;
 __asm {
  mov eax,q
  mov edx,out
  fld dword ptr[eax+8]
  fmul dword ptr[eax+4]
  fld dword ptr[eax+12]
  fmul dword ptr[eax]
  fsubp st(1),st(0)
  fadd st(0),st(0)
  fstp dword ptr[edx]
  fld dword ptr[eax+12]
  fmul dword ptr[eax+12]
  fld dword ptr[eax+4]
  fmul dword ptr[eax+4]
  faddp st(1),st(0)
  fadd st(0),st(0)
  fsubr one
  fstp dword ptr[edx+4]
  fld dword ptr[eax+12]
  fmul dword ptr[eax+8]
  fld dword ptr[eax+4]
  fmul dword ptr[eax]
  faddp st(1),st(0)
  fadd st(0),st(0)
  fstp dword ptr[edx+8]
 }
}
void add_word(float* out,const float* a,const float* b) {
 __asm {
  mov eax,a
  mov edx,b
  fld dword ptr[eax]
  fadd dword ptr[edx]
  mov eax,out
  fstp dword ptr[eax]
 }
}
void difference(const V* a,const V* b,V* out) {
 __asm {
  mov eax,a
  mov edx,b
  mov ecx,out
  fld dword ptr[eax]
  fsub dword ptr[edx]
  fstp dword ptr[ecx]
  fld dword ptr[eax+4]
  fsub dword ptr[edx+4]
  fstp dword ptr[ecx+4]
  fld dword ptr[eax+8]
  fsub dword ptr[edx+8]
  fstp dword ptr[ecx+8]
 }
}
template<class F> void st0_word(F target,void* self,float* out) {
 __asm {
  mov ecx,self
  xor edx,edx
  call target
  mov eax,out
  fstp dword ptr[eax]
 }
}
void yaw_word(const V* v,float* out) {
 __asm {
  push v
  call kotor_m15a_yaw_004aa0f0
  add esp,4
  mov eax,out
  fstp dword ptr[eax]
 }
}
void options_values(void* self,U a,U b,U c) {
 st0_word(kotor_fastcall_0061d5e0,opaque(kotor_fastcall_005ed700,app(),nullptr),fp(self,a));
 st0_word(kotor_fastcall_0061d610,opaque(kotor_fastcall_005ed700,app(),nullptr),fp(self,b));
 st0_word(kotor_fastcall_0061d640,opaque(kotor_fastcall_005ed700,app(),nullptr),fp(self,c));
}
void scaled_back(const V* direction,float* distance,const V* origin,V* out,bool ground) {
 if(ground) {
  __asm {
  mov eax,distance
  fld dword ptr[eax]
  fchs
  mov eax,direction
  fld dword ptr[eax]
  fmul st(0),st(1)
  fld dword ptr[eax+4]
  fmul st(0),st(2)
  mov edx,out
  fstp dword ptr[edx+4]
  fxch st(1)
  fmul dword ptr[eax+8]
  fstp dword ptr[edx+8]
  mov ecx,origin
  fld dword ptr[ecx]
  fadd st(0),st(1)
  fstp dword ptr[edx]
  fstp st(0)
  fld dword ptr[ecx+4]
  fadd dword ptr[edx+4]
  fstp dword ptr[edx+4]
  fld dword ptr[edx+8]
  fstp dword ptr[edx+8]
 }
  return;
 }
 __asm {
  mov eax,distance
  fld dword ptr[eax]
  fchs
  mov eax,direction
  fld dword ptr[eax]
  fmul st(0),st(1)
  fld dword ptr[eax+4]
  fmul st(0),st(2)
  mov edx,out
  fstp dword ptr[edx+4]
  fld dword ptr[eax+8]
  fmul st(0),st(2)
  fstp dword ptr[edx+8]
  mov ecx,origin
  fadd dword ptr[ecx]
  fstp dword ptr[edx]
  fstp st(0)
  fld dword ptr[edx+4]
  fadd dword ptr[ecx+4]
  fstp dword ptr[edx+4]
 }
 if(!ground) {
  __asm {
  mov edx,out
  mov ecx,origin
  fld dword ptr[edx+8]
  fadd dword ptr[ecx+8]
  fstp dword ptr[edx+8]
 }
 }
}
}

extern "C" void* __fastcall kotor_fastcall_0063ad60(void* self,void*,void* settings) {
 ConstructorUnwind guard{self};
 (void)opaque(kotor_fastcall_0049f110,self,nullptr,ptr(settings,8));guard.active=1;
 wr(self,0,static_cast<const void*>(w4cam::camera_stick_vtable_0075165c));
 wr(self,0x14,U(0));guard.active=2;wr(self,0x18,U(0));
 for(U o:{0x34u,0x30u,0x2cu})wr(self,o,U(0));wr(self,0x28,U(0x3f800000));
 for(U o=0x38;o<=0x58;o+=4)wr(self,o,U(0));guard.active=3;
 for(U o:{0x74u,0x78u,0x7cu})wr(self,o,U(0));
 (void)opaque(kotor_fastcall_006d10b0,at(self,0xb4),nullptr);
 for(U o:{0x128u,0x12cu,0x130u})wr(self,o,U(0));
 (void)opaque(safe_pointer_assign_00447fa0,at(self,0x14),nullptr,ptr(settings,12));
 wr(self,0x1c,rd<U>(settings,8));wr(self,0x68,rd<U>(settings,0x14));wr(self,0x6c,rd<U>(settings,0x18));
 wr(self,0x5c,U(0xffffffff));opaque(kotor_fastcall_00638070,self,nullptr,rd<int>(settings,0x10));
 opaque(behavior_add_dependency_0049f130,self,nullptr,ptr(self,0x14));
 wr(self,0x128,U(0));wr(self,0x12c,U(0));wr(self,0x108,rd<U>(self,0x11c));
 wr(self,0x124,U(0));wr(self,0x130,U(0));wr(self,0x118,U(0x42c80000));
 for(U o:{0x104u,0x100u,0x10cu})wr(self,o,U(0));
 Q q;V axis,normalized;float yaw;
 (void)get_orientation(ptr(self,0x14),&q);axis_z(&q,&axis);
 V* norm=opaque(normalize_004ab180,&normalized,&axis);yaw_word(norm,&yaw);
 Q temp;Q* result=opaque(yaw_pitch_roll_004acac0,&temp,yaw,rd<float>(self,0x11c),0.0f);
 wr(self,0x28,*result);Q orientation_out;set_orientation(ptr(self,0x10),q,&orientation_out);
 V position;(void)get_position(ptr(self,0x14),&position);const float one_half=1.5f;
 add_word(&position.z,&position.z,&one_half);axis_y(&q,&axis);
 (void)opaque(vector_subtract_assign_004a9770,&position,nullptr,&axis);
 for(U o:{0x50u,0x44u,0x38u})wr(self,o,rd<U>(&position,0));
 for(U o:{0x54u,0x48u,0x3cu})wr(self,o,rd<U>(&position,4));
 wr(self,0x60,U(1));wr(self,0x64,U(1));
 for(U o:{0x90u,0x94u,0xa4u,0xacu})wr(self,o,U(0));
 for(U o:{0x58u,0x4cu,0x40u})wr(self,o,rd<U>(&position,8));
 wr(self,0x80,U(0x7f000000));wr(self,0x98,U(0xbf800000));
 wr(self,0xb0,U(kotor_input_cells_required::word_008338f0));guard.active=0;return self;
}

extern "C" void* __fastcall kotor_fastcall_006d12b0(void* self,void*,void* owner) {
 ConstructorUnwind guard{self};(void)opaque(kotor_fastcall_0049f110,self,nullptr,owner);guard.active=1;
 wr(self,0,static_cast<const void*>(&camera_restore::camera_combat_vtable_00757bdc));
 for(U o:{0x1cu,0x20u,0x24u})wr(self,o,U(0));
 (void)opaque(kotor_fastcall_006d10b0,at(self,0x44),nullptr);
 for(U o:{0x18u,0x14u,0x28u,0x2cu})wr(self,o,U(0));
 auto* table=static_cast<options_video::Table*>(ptr(ptr(kotor_cell_rules_007a3a28,0xb8),0x74));
 const char* labels[]={"DISTANCE","HEIGHT","PITCH","ROTATION","SPEED","VIEWANGLE"};
 const U offsets[]={0x38,0x40,0x30,0x34,0x3c};float view_angle;
 for(U i=0;i<6;++i) {
  String column(labels[i]);
  (void)opaque(menu_tables_required::C2daFloatEntry_00413430,table,nullptr,8,
      reinterpret_cast<options_video::String*>(&column),i<5?fp(self,offsets[i]):&view_angle);
 }
 void* camera=from_word(gob_vt(ptr(self,0x10))->s004_gob_return_zero(gob(ptr(self,0x10)),nullptr));
 cam_vt(camera)->s044_camera_set_view_angle(camera,nullptr,view_angle);
 options_values(self,0x84,0x88,0x8c);
 wr(self,0x90,U(kotor_input_cells_required::word_008338f0));guard.active=0;return self;
}

extern "C" U __fastcall kotor_fastcall_006d14d0(void*,void*) {return 0x1070;}
extern "C" void* __fastcall kotor_fastcall_006d18c0(void* self,void*,U flags) {
 kotor_fastcall_006db4e0(self,nullptr);if(flags&1)live_free(self);return self;
}
extern "C" void __fastcall kotor_fastcall_006d1870(void* self,void*,U raw_turn) {
 void* manager=kotor_menu_window_cells::manager_007a39fc;
 U enabled=U(kotor_input_cells_required::word_008338f0);
 void* options=opaque(kotor_fastcall_005ed700,ptr(manager,4),nullptr);
 if(rd<B>(options,8)&2)enabled=enabled==0?1u:0u;
 wr(self,0x28,enabled?raw_turn:0u);
}

extern "C" void __fastcall kotor_fastcall_006d22c0(void* self,void*) {
 void* character=opaque(kotor_fastcall_00635460,opaque(kotor_fastcall_005ed8b0,app(),nullptr),nullptr);
 void* cached_app=app();U target_id=opaque(kotor_fastcall_005edd80,cached_app,nullptr);
 void* target=opaque(kotor_fastcall_005ed580,cached_app,nullptr,target_id);
 (void)opaque(kotor_fastcall_005ed560,app(),nullptr);
 if(!character||!target||!actor_view(target))return;
 V initial=rd<V>(character,0x24);V target_position=rd<V>(actor_view(target),0x24);
 V direction;difference(&target_position,&initial,&direction);float yaw;yaw_word(&direction,&yaw);
 opaque(vector_normalize_004ab130,&direction,nullptr);
 V desired;scaled_back(&direction,fp(self,0x38),&initial,&desired,false);
 V checked;(void)opaque(camera_check_position_006d18e0,self,nullptr,&checked,character,&desired,&initial);
 Q orientation;(void)opaque(yaw_pitch_roll_004acac0,&orientation,yaw,rd<float>(self,0x30),0.0f);
 Q orientation_out;set_orientation(ptr(self,0x10),orientation,&orientation_out);
 V position_out;set_position(ptr(self,0x10),checked,&position_out);
}

extern "C" double __fastcall kotor_fastcall_006d16f0(void* self,void*,const V*,float yaw,float,U* flag) {
 Q orientation;(void)get_orientation(ptr(self,0x10),&orientation);unsigned short status;
 __asm {
  lea eax,orientation
  push eax
  call quaternion_yaw_004a9f40
  fcomp yaw
  add esp,4
  fnstsw ax
  mov status,ax
 }
 // TEST AH,44 / JNP: skip only the ordered-equal arm.
 const B masked=B((status>>8)&0x44);
 if(w4::parity_even(masked)) {
  void* options=opaque(kotor_fastcall_005ed700,app(),nullptr);
  __asm {
  mov ecx,options
  xor edx,edx
  call kotor_fastcall_0061d5b0
  fstp st(0)
 }
 }
 void* options=opaque(kotor_fastcall_005ed700,app(),nullptr);
 __asm {
  mov ecx,options
  xor edx,edx
  call kotor_fastcall_0061d5b0
  fstp st(0)
 }
 double result;
 __asm {
  fld yaw
  mov eax,flag
  mov dword ptr[eax],0
  fstp qword ptr result
 }
 return result; // Exact widened float carrier; the original loads yaw before writing flag.
}

extern "C" V* __fastcall kotor_fastcall_006d1760(void* self,void*,V* out,const V* desired,float duration,float delta) {
 V old_position;V* returned=get_position(ptr(self,0x10),&old_position);
 V displacement;
 const float minimum_duration=w4::bits_f(0x3a83126f); // 0073D708
 const float minimum_distance_squared=w4::bits_f(0x3c23d70a); // 0073DFEC
 bool interpolate=false;
 __asm {
  mov eax,desired
  mov edx,returned
  fld dword ptr[eax+8]
  fsub dword ptr[edx+8]
  fld dword ptr[eax+4]
  fsub dword ptr[edx+4]
  fld dword ptr[eax]
  fsub dword ptr[edx]
  fstp displacement.x
  fst displacement.y
  fld st(1)
  fstp displacement.z
  fld duration
  fcomp minimum_duration
  fnstsw ax
  test ah,41h
  jne no_duration
  // Keep unrounded y/z on ST0/ST1 as the original does; only x was popped.
  fld displacement.x
  fmul displacement.x
  fld st(1)
  fmul st(0),st(2)
  faddp st(1),st(0)
  fld st(2)
  fmul st(0),st(3)
  faddp st(1),st(0)
  fstp st(2)
  fstp st(0)
  fcom minimum_distance_squared
  fnstsw ax
  test ah,41h
   jne no_distance
  fsqrt
  fdiv duration
  fmul delta
  fstp duration
  mov interpolate,1
   jmp distance_done
  no_duration:
  fstp st(0)
  no_distance:
  fstp st(0)
  distance_done:
 }
 if(!interpolate) {std::memcpy(out,desired,12);return out;}
 V normalized;V* n=opaque(normalize_004ab180,&normalized,&displacement);
 V scaled;
 __asm {
  mov eax,n
  fld duration
  fmul dword ptr[eax]
  fstp scaled.x
  fld duration
  fmul dword ptr[eax+4]
  fstp scaled.y
  fld duration
  fmul dword ptr[eax+8]
  fstp scaled.z
 }
 V current;V* r=get_position(ptr(self,0x10),&current);
 add_word(&old_position.x,&scaled.x,&r->x);add_word(&old_position.y,&scaled.y,&r->y);add_word(&old_position.z,&scaled.z,&r->z);
 std::memcpy(out,&old_position,12);return out;
}

extern "C" V* __fastcall kotor_fastcall_006d14e0(void* self,void*,V* out,void* character,void*,float delta,float* yaw) {
 void* camera=opaque(kotor_fastcall_005ed560,app(),nullptr);
 float turn=rd<float>(self,0x28);unsigned short status;const float zero=0;
 __asm {
  fld turn
  fcomp zero
  fnstsw ax
  mov status,ax
 }
 V initial=rd<V>(character,0x24);
 if(!w4::parity_even(B((status>>8)&0x44))) {
  W4Vector input{0,rd<float>(self,0x2c),0};alignas(float) B state_bytes[24];
  auto* result=opaque(kotor_fastcall_006d1190,at(self,0x44),nullptr,
      reinterpret_cast<W4RK4State*>(state_bytes),input,delta,U(1),U(0),U(0),U(0));
  std::memcpy(&turn,static_cast<B*>(static_cast<void*>(result))+0x10,4);
 } else {
  opaque(kotor_fastcall_006d1030,at(self,0x44),nullptr,turn);wr(self,0x2c,U(0));
 }
 Q camera_orientation;
 Q* orientation=static_cast<Q*>(from_word(cam_vt(camera)->s024_camera_get_orientation(camera,nullptr,word(&camera_orientation))));
 Q delta_orientation;Q* delta_ptr=opaque(yaw_pitch_roll_004acac0,&delta_orientation,turn,0.0f,0.0f);
 Q combined;(void)opaque(quaternion_multiply_004a9ac0,&combined,delta_ptr,orientation);
 __asm {
  lea eax,combined
  push eax
  call quaternion_yaw_004a9f40
  mov eax,yaw
  fstp dword ptr[eax]
  add esp,4
 }
 V direction;axis_y(&combined,&direction);wr(&direction,8,U(0));
 opaque(vector_normalize_004ab130,&direction,nullptr);
 V desired;scaled_back(&direction,fp(self,0x38),&initial,&desired,true);
 void* appearance=ptr(character,0x21c);float height;const float* z=fp(character,0x2c);const float* extra=fp(self,0x40);
 __asm {
  mov ecx,appearance
  xor edx,edx
  call kotor_m15e_appearance_get_head_height_offset_006974d0
  mov eax,z
  fadd dword ptr[eax]
  mov eax,extra
  fadd dword ptr[eax]
  fstp height
 }
 wr(&desired,8,rd<U>(&height,0));std::memcpy(out,&desired,12);return out;
}

extern "C" U __fastcall kotor_fastcall_006d1fc0(void* self,void*,U raw_delta) {
 if(!opaque(behavior_control_0049f270,self,nullptr,raw_delta))return 0;
 options_values(self,0x84,0x88,0x8c);
 void* character=opaque(kotor_fastcall_00635460,opaque(kotor_fastcall_005ed8b0,app(),nullptr),nullptr);
 void* cached_app=app();U target_id=opaque(kotor_fastcall_005edd80,cached_app,nullptr);
 void* target=opaque(kotor_fastcall_005ed580,cached_app,nullptr,target_id);
 if(w4cam::dialog_camera_debug_008327ec&&ptr(self,0x10)&&character) {
  V scratch[4];for(U i=0;i<4;++i) {
   V* result=get_position(ptr(self,0x10),&scratch[i]);
   if(i==3) {
    V delta;const V* origin=static_cast<const V*>(at(character,0x24));
    __asm {
  mov eax,result
  mov edx,origin
  fld dword ptr[eax+8]
  fsub dword ptr[edx+8]
  fld dword ptr[eax+4]
  fsub dword ptr[edx+4]
  fld dword ptr[eax]
  fsub dword ptr[edx]
  fstp delta.x
  fstp delta.y
  fstp delta.z
  lea eax,delta
  push eax
  call vector_magnitude_004ab100
  fstp st(0)
  add esp,4
 }
   }
  }
 }
 void* options_app=app();U enabled=U(kotor_input_cells_required::word_008338f0);
 void* options=opaque(kotor_fastcall_005ed700,options_app,nullptr);
 if(rd<B>(options,8)&2)enabled=enabled==0?1u:0u;
 U old=rd<U>(self,0x90);
 if(enabled) {
  if(!old) {
   (void)opaque(gui_m48_manager_store_mouse_0040a3d0,kotor_input_cells::gui_manager_007a39f4,nullptr);
   wr(self,0x90,enabled);opaque(kotor_fastcall_005ee170,app(),nullptr,U(0x20));
  }
 } else if(old) {
  (void)opaque(gui_m48_manager_restore_mouse_0040c7d0,kotor_input_cells::gui_manager_007a39f4,nullptr);
  wr(self,0x90,U(0));opaque(kotor_fastcall_005ee180,app(),nullptr,U(0x20));
 }
 if(opaque(gui_m48_client_get_input_class_005eda70,app(),nullptr)==2) {
  wr(self,0x90,U(0));opaque(kotor_fastcall_005ee180,app(),nullptr,U(0x20));
 }
 if(!target)target=character;
 if(!character||!target||!actor_view(target))return 1;
 V initial=rd<V>(character,0x24);void* appearance=ptr(character,0x21c);
 __asm {
  mov ecx,appearance
  xor edx,edx
  call kotor_m15e_appearance_get_height_006973b0
  fadd initial.z
  fstp initial.z
 }
 float delta,yaw;std::memcpy(&delta,&raw_delta,sizeof delta);V desired;
 void* view=actor_view(target);
 (void)opaque(kotor_fastcall_006d14e0,self,nullptr,&desired,character,view,delta,&yaw);
 V checked;(void)opaque(camera_check_position_006d18e0,self,nullptr,&checked,character,&desired,&initial);
 U duration_bits=0;
 // Original returns ST0 unchanged yaw and writes zero through the fourth argument.
 __asm {
  lea eax,duration_bits
  push eax
  push raw_delta
  push yaw
  lea eax,checked
  push eax
  mov ecx,self
  xor edx,edx
  call kotor_fastcall_006d16f0
  fstp yaw
 }
 float duration;std::memcpy(&duration,&duration_bits,sizeof duration);
 (void)opaque(kotor_fastcall_006d1760,self,nullptr,&initial,&checked,duration,delta);
 float pitch;add_word(&pitch,fp(self,0x30),fp(self,0x18));
 float final_yaw;add_word(&final_yaw,&yaw,fp(self,0x14));
 Q orientation;(void)opaque(yaw_pitch_roll_004acac0,&orientation,final_yaw,pitch,0.0f);
 Q orientation_out;set_orientation(ptr(self,0x10),orientation,&orientation_out);
 V position_out;set_position(ptr(self,0x10),initial,&position_out);return 1;
}
