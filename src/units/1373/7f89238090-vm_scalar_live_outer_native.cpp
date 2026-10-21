extern "C" void __cdecl live_free(void*); // 1118 single heap: CRT free -> game CRT heap _live_free 006FA390
#include "0b782d2ebb-vm_scalar_live.hpp"
#include <cstdlib>
#include <cstring>
#include <new>
extern "C" void __fastcall recovered_vm_stack_shrink_005d21f0(void*,void*,int);
namespace {using namespace recovered_vm_scalar;void*ptr(U a){return reinterpret_cast<void*>(a);}U word(void*p){return reinterpret_cast<U>(p);}struct Native:Ports{U read(U a)override{U v;std::memcpy(&v,ptr(a),4);return v;}U byte(U a)override{return *static_cast<unsigned char*>(ptr(a));}void write(U a,U v)override{std::memcpy(ptr(a),&v,4);}void write_byte(U a,U v)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(v);}U allocate(U n)override{return word(::operator new(n));}void release(U a)override{::live_free(ptr(a));}void copy(U a,U b,U n)override{std::memcpy(ptr(a),ptr(b),n);}void shrink(U a,U n)override{recovered_vm_stack_shrink_005d21f0(ptr(a),nullptr,static_cast<int>(n));}};}
extern "C" unsigned __fastcall recovered_vm_push_integer_005d1850(void*self,void*,unsigned value){Native p;return recovered_vm_scalar::push(p,word(self),3,value);}
extern "C" unsigned __fastcall recovered_vm_push_float_005d1880(void*self,void*,unsigned bits){Native p;return recovered_vm_scalar::push(p,word(self),4,bits);}
extern "C" unsigned __fastcall recovered_vm_push_object_005d1a10(void*self,void*,unsigned value){Native p;return recovered_vm_scalar::push(p,word(self),6,value);}
extern "C" unsigned __fastcall recovered_vm_pop_integer_005d24c0(void*self,void*,void*out){Native p;return recovered_vm_scalar::pop(p,word(self),3,word(out));}
extern "C" unsigned __fastcall recovered_vm_pop_float_005d2510(void*self,void*,void*out){Native p;return recovered_vm_scalar::pop(p,word(self),4,word(out));}

extern "C" unsigned __fastcall recovered_vm_pop_object_005d26a0(void*self,void*,void*out){Native p;return recovered_vm_scalar::pop(p,word(self),6,word(out));}
extern "C" unsigned __fastcall recovered_vm_outer_push_integer_005d1010(void*self,void*,unsigned value){Native p;return recovered_vm_push_integer_005d1850(ptr(p.read(word(self))),nullptr,value);}
extern "C" unsigned __fastcall recovered_vm_outer_push_object_005d10d0(void*self,void*,unsigned value){Native p;return recovered_vm_push_object_005d1a10(ptr(p.read(word(self))),nullptr,value);}
extern "C" unsigned __fastcall recovered_vm_outer_pop_object_005d10c0(void*self,void*,void*out){Native p;return recovered_vm_pop_object_005d26a0(ptr(p.read(word(self))),nullptr,out);}
