#include "b818a80e8adc7f69_ref_array_growth.hpp"
#include <cstring>
#include <new>
namespace menu_resources{struct Ref;}
namespace resource_ref_assignment{menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*,void*,menu_resources::Ref*,const menu_resources::Ref*);}
extern "C" void* __fastcall c2da_default_ref_00405ed0(void*,void*);
namespace{using U=unsigned;void*ptr(U a){return reinterpret_cast<void*>(a);}U word(void*p){return reinterpret_cast<U>(p);}struct Native:recovered_ref_array_growth::Ports{U read(U a)override{U v;std::memcpy(&v,ptr(a),4);return v;}void write(U a,U v)override{std::memcpy(ptr(a),&v,4);}U allocate(U n)override{return word(::operator new(n));}void release(U a)override{::operator delete(ptr(a));}void construct(U a)override{(void)c2da_default_ref_00405ed0(ptr(a),nullptr);}void assign(U a,U out,U src)override{(void)resource_ref_assignment::assign_00406120(static_cast<menu_resources::Ref*>(ptr(a)),nullptr,static_cast<menu_resources::Ref*>(ptr(out)),static_cast<const menu_resources::Ref*>(ptr(src)));}};}
extern "C" void __fastcall recovered_ref_array_grow_00506e20(void*self,void*,unsigned capacity){Native p;unsigned scratch[4];recovered_ref_array_growth::grow(word(self),capacity,word(scratch),p);}
