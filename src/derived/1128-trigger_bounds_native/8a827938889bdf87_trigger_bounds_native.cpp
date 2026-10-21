#include "27a36b75249a4636_trigger_bounds.hpp"
#include <cstring>
namespace{using U=unsigned;void*ptr(U a){return reinterpret_cast<void*>(a);}U word(void*p){return reinterpret_cast<U>(p);}unsigned short compare(U a,U b){unsigned short status;__asm{fld dword ptr a
 fcomp dword ptr b
 fnstsw status
 }return status;}
struct Native:recovered_trigger_bounds::Ports{U read(U a)override{U v;std::memcpy(&v,ptr(a),4);return v;}void write(U a,U v)override{std::memcpy(ptr(a),&v,4);}bool equal(U a,U b)override{U f=compare(a,b)&0x4400;return f==0x4000||f==0x400;}bool less(U a,U b)override{U f=compare(a,b)&0x500;return f==0x100||f==0x400;}bool greater(U a,U b)override{return(compare(a,b)&0x4100)==0;}};}
extern "C" void __fastcall recovered_trigger_bounds_0058cc60(void*self,void*,void*x0,void*y0,void*x1,void*y1){Native p;recovered_trigger_bounds::bounds(word(self),word(x0),word(y0),word(x1),word(y1),p);}
