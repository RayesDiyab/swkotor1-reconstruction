#include "d6da090c8d2fc425_trigger_geometry.hpp"
#include <cstring>
#include <new>
namespace kotor_q05{struct StructHandle;struct CResGFF{unsigned __thiscall GetElementType(StructHandle*);};}
extern "C"{unsigned __fastcall gff_list_count_00411940(void*,void*,void*);unsigned __fastcall gff_list_element_00411990(void*,void*,void*,void*,unsigned);float __fastcall gui_load_gff_float_00411d00(void*,void*,void*,const char*,unsigned*,float);}
namespace{using U=unsigned;void*ptr(U a){return reinterpret_cast<void*>(a);}U word(void*p){return reinterpret_cast<U>(p);}struct Native:recovered_trigger_geometry::Ports{
 U read(U a)override{U v;std::memcpy(&v,ptr(a),4);return v;}void write(U a,U v)override{std::memcpy(ptr(a),&v,4);}void free(U a)override{::operator delete(ptr(a));}U allocate(U n)override{return word(::operator new(n));}
 U count(U g,U l)override{return gff_list_count_00411940(ptr(g),nullptr,ptr(l));}U element(U g,U e,U l,U i)override{return gff_list_element_00411990(ptr(g),nullptr,ptr(e),ptr(l),i);}U type(U g,U e)override{return static_cast<kotor_q05::CResGFF*>(ptr(g))->GetElementType(static_cast<kotor_q05::StructHandle*>(ptr(e)));}
 U floating(U g,U e,U label,U status)override{const char*t=label==0x7499e4?"PointX":label==0x7499dc?"PointY":"PointZ";float v=gui_load_gff_float_00411d00(ptr(g),nullptr,ptr(e),t,static_cast<unsigned*>(ptr(status)),0.0f);U bits;std::memcpy(&bits,&v,4);return bits;}
 void translate(U s,U displacement,U x,U y,U z)override{U tx,ty,tz;
 __asm{
 mov esi,s
 fld dword ptr z
 fld dword ptr x
 fadd dword ptr [esi+90h]
 mov eax,dword ptr [esi+288h]
 add eax,displacement
 fstp dword ptr tx
 mov ecx,tx
 fld dword ptr y
 fadd dword ptr [esi+94h]
 fstp dword ptr ty
 mov edx,ty
 fadd dword ptr [esi+98h]
 mov dword ptr [eax],ecx
 mov dword ptr [eax+4],edx
 fstp dword ptr tz
 mov ecx,tz
 mov dword ptr [eax+8],ecx
 }
 }
};}
extern "C" void __fastcall recovered_trigger_geometry_0058d060(void*self,void*,void*gff,void*list){Native p;unsigned element,status;recovered_trigger_geometry::load(word(self),word(gff),word(list),word(&element),word(&status),p);}
