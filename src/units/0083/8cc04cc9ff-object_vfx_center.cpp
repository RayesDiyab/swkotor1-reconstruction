// Original CSWCAnimBase::GetCenterPoint, AL result, RET4. Canonical renderer
// carriers/providers retain their owners; raw frame offsets are original ESP.
#include "309ecfaddb-scene3d.h"
#include "cf2f594520-object_vfx_center.h"
#include <cstdint>
#include <cstring>
using U=std::uint32_t;using I=std::int32_t;using B=std::uint8_t;
using kotor_scene3d::Vector;using kotor_scene3d::Quaternion;
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;}
extern "C" {
void* __fastcall kotor_fastcall_005ed560(void*,void*);
void* __fastcall kotor_fastcall_005eda40(void*,void*);
Vector* __fastcall quaternion_zdir_0043e250(const Quaternion*,void*,Vector*);
void __stdcall vector_constructor_iterator_00401130(void*,U,U,void*(__fastcall*)(void*,void*));
void* __fastcall vector_default_construct_00409e70(void*,void*);
}
namespace {
template<class T>T rd(const void*p,U o){T v;std::memcpy(&v,static_cast<const B*>(p)+o,sizeof(v));return v;}
template<class T>void wr(void*p,U o,T v){std::memcpy(static_cast<B*>(p)+o,&v,sizeof(v));}
void*ptr(const void*p,U o){return rd<void*>(p,o);}
U word(const void*p){return static_cast<U>(reinterpret_cast<std::uintptr_t>(p));}
template<class F>F slot(void*p,U o){return rd<F>(ptr(p,0),o);}
template<class F,class...A>auto invoke(F f,A...a){F volatile target=f;return target(a...);}
Vector*vec(B*f,U off){return reinterpret_cast<Vector*>(f+off);}
Quaternion*quat(B*f,U off){return reinterpret_cast<Quaternion*>(f+off);}
using GobOutput=U(__fastcall*)(aurora_model::Gob*,void*,U);
using GobBounds=U(__fastcall*)(aurora_model::Gob*,void*,U,U);
using CameraPosition=Vector*(__fastcall*)(void*,void*,Vector*);
using CameraOrientation=U(__fastcall*)(void*,void*,U);
using CameraScreen=U(__fastcall*)(void*,void*,U,U,U,U,U);
void negate_direction(B*f,const Vector*direction){__asm {
 mov ecx,f
 mov eax,direction
 fld dword ptr [eax]
 fchs
 fstp dword ptr [ecx+04ch]
 fld dword ptr [eax+4]
 fchs
 fstp dword ptr [ecx+050h]
 fld dword ptr [eax+8]
 fchs
 fstp dword ptr [ecx+054h]
}}
void translated_corner(B*f,Vector*corner,const Vector*rotated){__asm {
 mov ecx,f
 mov eax,rotated
 fld dword ptr [ecx+064h]
 fadd dword ptr [eax]
 fstp dword ptr [ecx+00ch]
 fld dword ptr [ecx+068h]
 fadd dword ptr [eax+4]
 fstp dword ptr [ecx+010h]
 fld dword ptr [ecx+06ch]
 fadd dword ptr [eax+8]
 mov esi,corner
 mov edx,dword ptr [ecx+00ch]
 mov eax,dword ptr [ecx+010h]
 mov dword ptr [esi],edx
 mov dword ptr [esi+4],eax
 fstp dword ptr [ecx+014h]
 mov edx,dword ptr [ecx+014h]
 mov dword ptr [esi+8],edx
}}
// Match FCOMP/FNSTSW/mask/parity exactly. Unordered updates min and max;
// equal values also copy the original raw DWORD from the transformed corner.
void bounds_update(B*f,const Vector*corner){__asm {
 mov ecx,f
 mov esi,corner
 fld dword ptr [ecx+03ch]
 fcomp dword ptr [esi]
 fnstsw ax
 test ah,5
 jnp min_y
 mov eax,dword ptr [esi]
 mov dword ptr [ecx+03ch],eax
min_y:
 fld dword ptr [ecx+040h]
 fcomp dword ptr [esi+4]
 fnstsw ax
 test ah,5
 jnp min_z
 mov eax,dword ptr [esi+4]
 mov dword ptr [ecx+040h],eax
min_z:
 fld dword ptr [ecx+044h]
 fcomp dword ptr [esi+8]
 fnstsw ax
 test ah,5
 jnp max_x
 mov eax,dword ptr [esi+8]
 mov dword ptr [ecx+044h],eax
max_x:
 fld dword ptr [ecx+018h]
 fcomp dword ptr [esi]
 fnstsw ax
 test ah,041h
 je max_y
 mov eax,dword ptr [esi]
 mov dword ptr [ecx+018h],eax
max_y:
 fld dword ptr [ecx+01ch]
 fcomp dword ptr [esi+4]
 fnstsw ax
 test ah,041h
 je max_z
 mov eax,dword ptr [esi+4]
 mov dword ptr [ecx+01ch],eax
max_z:
 fld dword ptr [ecx+020h]
 fcomp dword ptr [esi+8]
 fnstsw ax
 test ah,041h
 je bounds_done
 mov eax,dword ptr [esi+8]
 mov dword ptr [ecx+020h],eax
bounds_done:
}}
B center_and_depth(B*f){
 const U half=0x3f000000;B result=1;
 __asm {
 mov ecx,f
 fld dword ptr [ecx+018h]
 fsub dword ptr [ecx+03ch]
 fld dword ptr [ecx+01ch]
 fsub dword ptr [ecx+040h]
 fld dword ptr [ecx+020h]
 fsub dword ptr [ecx+044h]
 fstp dword ptr [ecx+014h]
 fxch st(1)
 fmul dword ptr half
 fstp dword ptr [ecx+018h]
 fmul dword ptr half
 fld dword ptr [ecx+014h]
 fmul dword ptr half
 fld dword ptr [ecx+018h]
 fadd dword ptr [ecx+03ch]
 fstp dword ptr [ecx+018h]
 fxch st(1)
 fadd dword ptr [ecx+040h]
 fstp dword ptr [ecx+01ch]
 fadd dword ptr [ecx+044h]
 fst dword ptr [ecx+020h]
 fmul dword ptr [ecx+054h]
 fld dword ptr [ecx+050h]
 fmul dword ptr [ecx+01ch]
 faddp st(1),st(0)
 fld dword ptr [ecx+04ch]
 fmul dword ptr [ecx+018h]
 faddp st(1),st(0)
 fld dword ptr [ecx+060h]
 fmul dword ptr [ecx+054h]
 fld dword ptr [ecx+05ch]
 fmul dword ptr [ecx+050h]
 faddp st(1),st(0)
 fld dword ptr [ecx+04ch]
 fmul dword ptr [ecx+058h]
 faddp st(1),st(0)
 fcompp
 fnstsw ax
 test ah,5
 jnp depth_done
 mov result,2
depth_done:
 }
 return result;
}
void screen_scale(I extent,const float*coordinate,float*out){__asm {
 fild dword ptr extent
 mov eax,coordinate
 fmul dword ptr [eax]
 mov eax,out
 fstp dword ptr [eax]
}}
}
extern "C" B __fastcall object_vfx_center_0069f1e0(void*self,void*,void*raw_out){
 float*out=static_cast<float*>(raw_out);
 void*camera=invoke(kotor_fastcall_005ed560,ptr(kotor_menu_window_cells::manager_007a39fc,4),nullptr);
 if(!camera)return 0;
 auto*g=static_cast<aurora_model::Gob*>(ptr(self,0xb8));if(!g)return 0;
 // Original E4 local bytes plus three saved registers produce a F0-byte
 // scratch span below the caller frame. Quaternion at E0 writes through EF.
 alignas(4) B frame[0xf0];
 for(U off=0x24;off<0x3c;off+=4)wr<U>(frame,off,0);
 slot<GobOutput>(g,0x64)(g,nullptr,word(vec(frame,0x64)));
 g=static_cast<aurora_model::Gob*>(ptr(self,0xb8));
 slot<GobOutput>(g,0x68)(g,nullptr,word(quat(frame,0xe0)));
 slot<CameraPosition>(camera,0x20)(camera,nullptr,vec(frame,0x58));
 // The original pre-pushes zdir's output, then camera+24 consumes only its
 // own Quaternion output (RET4), leaving one word for the following zdir.
 U orientation=slot<CameraOrientation>(camera,0x24)(camera,nullptr,word(quat(frame,0xd0)));
 Vector*direction=invoke(quaternion_zdir_0043e250,reinterpret_cast<const Quaternion*>(static_cast<std::uintptr_t>(orientation)),nullptr,vec(frame,0x0c));
 negate_direction(frame,direction);
 g=static_cast<aurora_model::Gob*>(ptr(self,0xb8));
 slot<GobBounds>(g,0xec)(g,nullptr,word(vec(frame,0x24)),word(vec(frame,0x30)));
 invoke(vector_constructor_iterator_00401130,static_cast<void*>(frame+0x70),12u,8u,&vector_default_construct_00409e70);
 const U axes[8][3]={{0x24,0x28,0x2c},{0x30,0x34,0x38},{0x24,0x28,0x38},{0x24,0x34,0x2c},
                    {0x30,0x28,0x2c},{0x24,0x34,0x38},{0x30,0x28,0x38},{0x30,0x34,0x2c}};
 for(U i=0;i<8;++i)for(U j=0;j<3;++j)wr<U>(frame,0x70+i*12+j*4,rd<U>(frame,axes[i][j]));
 for(U j=0;j<3;++j){wr<U>(frame,0x3c+j*4,0x7f7fffff);wr<U>(frame,0x18+j*4,0xff7fffff);}
 for(U i=0;i<8;++i){
  Vector*corner=vec(frame,0x70+i*12);
  Vector*rotated=invoke(quaternion_rotate_004a9bb0,vec(frame,0xd0),static_cast<const Quaternion*>(quat(frame,0xe0)),static_cast<const Vector*>(corner));
  translated_corner(frame,corner,rotated);bounds_update(frame,corner);
 }
 B result=center_and_depth(frame);
 U screen=slot<CameraScreen>(camera,0x28)(camera,nullptr,word(vec(frame,0xd0)),rd<U>(frame,0x18),rd<U>(frame,0x1c),rd<U>(frame,0x20),1);
 std::memcpy(frame+0x18,reinterpret_cast<const void*>(static_cast<std::uintptr_t>(screen)),sizeof(Vector));
 void*gui=invoke(kotor_fastcall_005eda40,ptr(kotor_menu_window_cells::manager_007a39fc,4),nullptr);
 screen_scale(static_cast<I>(rd<std::int16_t>(gui,0x6c)),reinterpret_cast<const float*>(frame+0x18),out);
 screen_scale(static_cast<I>(rd<std::int16_t>(gui,0x6e)),reinterpret_cast<const float*>(frame+0x1c),out+1);
 return result;
}
