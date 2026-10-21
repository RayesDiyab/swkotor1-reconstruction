extern "C" void __cdecl live_free(void*); // 1118 single heap: CRT free -> game CRT heap _live_free 006FA390
#include "a5c9572aa2-vm_stack_view.hpp"
#include "3df78882af-startup_string.h"
#include <cstdlib>
#include <cstring>
namespace{using U=unsigned;void*ptr(U a){return reinterpret_cast<void*>(a);}U word(void*p){return reinterpret_cast<U>(p);}struct Native:recovered_vm_stack_view::Ports{U read(U a)override{U v;std::memcpy(&v,ptr(a),4);return v;}void write(U a,U b)override{std::memcpy(ptr(a),&b,4);}std::uint8_t byte(U a)override{return *static_cast<std::uint8_t*>(ptr(a));}void destroy_string(U a)override{static_cast<kotor_reconstruction::StartupString*>(ptr(a))->~StartupString();}void release(U a)override{::live_free(ptr(a));}void destroy_engine(U a,std::int32_t type,U payload)override{using F=void(__thiscall*)(void*,std::int32_t,U);reinterpret_cast<F>(read(read(a)+0x18))(ptr(a),type,payload);}};}
extern "C" void __fastcall recovered_vm_stack_clear_005d2140(void*self,void*){Native p;recovered_vm_stack_view::clear(word(self),p);}
extern "C" void __fastcall recovered_vm_stack_shrink_005d21f0(void*self,void*,int requested){Native p;recovered_vm_stack_view::shrink(word(self),requested,p);}

extern "C" unsigned __fastcall recovered_vm_script_delete_005d44f0(void*,void*,void*descriptor){Native p;return recovered_vm_stack_view::delete_script(word(descriptor),p);}
