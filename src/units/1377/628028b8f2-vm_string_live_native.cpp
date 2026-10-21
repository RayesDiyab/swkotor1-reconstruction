extern "C" void __cdecl live_free(void*); // 1118 single heap: CRT free -> game CRT heap _live_free 006FA390
#include "8115216e1f-vm_string_live.hpp"
#include "0e5bf9dd42-startup_string.h"
#include "cac93e7c05-native_strings.h"
#include <cstdlib>
#include <cstring>
#include <new>
extern "C" void __fastcall recovered_vm_stack_shrink_005d21f0(void*,void*,int);
namespace {using namespace recovered_vm_scalar;void*ptr(U a){return reinterpret_cast<void*>(a);}U word(void*p){return reinterpret_cast<U>(p);}struct Native:recovered_vm_string::Ports{U read(U a)override{U v;std::memcpy(&v,ptr(a),4);return v;}U byte(U a)override{return *static_cast<unsigned char*>(ptr(a));}void write(U a,U v)override{std::memcpy(ptr(a),&v,4);}void write_byte(U a,U v)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(v);}U allocate(U n)override{return word(::operator new(n));}void release(U a)override{::live_free(ptr(a));}void copy(U a,U b,U n)override{std::memcpy(ptr(a),ptr(b),n);}void shrink(U a,U n)override{recovered_vm_stack_shrink_005d21f0(ptr(a),nullptr,static_cast<int>(n));}U chars(U a)override{return word(WindowCStr005e5670(static_cast<kotor_reconstruction::StartupString*>(ptr(a)),nullptr));}U construct(U a,U b)override{return word(new(ptr(a))kotor_reconstruction::StartupString(static_cast<const char*>(ptr(b))));}void assign(U a,U b)override{(void)static_cast<kotor_reconstruction::StartupString*>(ptr(a))->Assign005e5c50(static_cast<kotor_reconstruction::StartupString*>(ptr(b)));}};}
extern "C" unsigned __fastcall recovered_vm_push_string_005d1930(void*self,void*,void*source){Native p;return recovered_vm_string::push(p,word(self),word(source));}
extern "C" unsigned __fastcall recovered_vm_pop_string_005d25e0(void*self,void*,void*destination){Native p;return recovered_vm_string::pop(p,word(self),word(destination));}
