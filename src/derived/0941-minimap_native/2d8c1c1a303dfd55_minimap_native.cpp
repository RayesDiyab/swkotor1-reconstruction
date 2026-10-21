#include "a9beba76e8c9e9c7_minimap.hpp"
#include <cstring>
#include <new>
extern "C" void __cdecl genuine_x87_ftol_006fae8c();
namespace {using namespace recovered_minimap;
static void* pointer(U a){return reinterpret_cast<void*>(a);}static float floating(U u){float f;std::memcpy(&f,&u,4);return f;}static U word(float f){U u;std::memcpy(&u,&f,4);return u;}
struct Native final:Ports{
 U read(U a,U o)override{U v;std::memcpy(&v,static_cast<char*>(pointer(a))+o,4);return v;}void write(U a,U o,U v)override{std::memcpy(static_cast<char*>(pointer(a))+o,&v,4);}
 I height(I width)override{const float scale=floating(0x3f14f209);I result;__asm{fild width}__asm{fmul scale}__asm{call genuine_x87_ftol_006fae8c}__asm{mov result,eax}return result;}
 U reciprocal(I divisor)override{const float one=1.0f;float v;__asm{fild divisor}__asm{fdivr one}__asm{fstp v}return word(v);}
 U allocate(U n)override{return reinterpret_cast<U>(::operator new(n));}void clear(U a,U n)override{std::memset(pointer(a),0,n);}
 void mapping(U self,const U*a)override{
 float x1=floating(a[3]),y1=floating(a[4]),x2=floating(a[5]),y2=floating(a[6]);
 // For ordinary finite inputs these exact sign/axis changes match x87 * -1.
 if(a[2]==3){float old=x1;x1=-y1;y1=old;old=x2;x2=-y2;y2=old;}
 else if(a[2]==1){x1=-x1;y1=-y1;x2=-x2;y2=-y2;}
 else if(a[2]==2){float old=x1;x1=y1;y1=-old;old=x2;x2=y2;y2=-old;}
 const U dxword=a[9]-a[7],dyword=a[10]-a[8];I delta_x,delta_y,mx,my;std::memcpy(&delta_x,&dxword,4);std::memcpy(&delta_y,&dyword,4);std::memcpy(&mx,a+7,4);std::memcpy(&my,a+8,4);
 float sx,sy,ox,oy;
 // Keep X slope extended for X offset. Original Y offset uses rounded F32 slope.
 __asm{fld x1}__asm{fld x2}__asm{fsub st(0),st(1)}__asm{fidiv delta_x}__asm{fst sx}
 __asm{fld y2}__asm{fsub y1}__asm{fidiv delta_y}__asm{fstp sy}
 __asm{fild mx}__asm{fmul st(0),st(1)}__asm{fsubr st(0),st(2)}__asm{fstp ox}__asm{fstp st(0)}__asm{fstp st(0)}
 __asm{fild my}__asm{fmul sy}__asm{fsubr y1}__asm{fstp oy}
 write(self,0x18,word(sx));write(self,0x1c,word(sy));write(self,0x20,word(ox));write(self,0x24,word(oy));
 }
};}
extern "C" void __fastcall recovered_area_map_setup_00578c60(void*self,void*,unsigned enabled,unsigned resolution,unsigned axis,unsigned x1,unsigned y1,unsigned x2,unsigned y2,unsigned mx1,unsigned my1,unsigned mx2,unsigned my2,unsigned divisor){const unsigned a[]={enabled,resolution,axis,x1,y1,x2,y2,mx1,my1,mx2,my2,divisor};Native p;recovered_minimap::setup(reinterpret_cast<unsigned>(self),a,p);}
