// W6 t1 native adapter for owner 1194 (vm_execute_live). Self-contained TU replacing the 0886-local
// vm_execute_native_strings.inc: binds every vm_execute_live::Ports slot to the EXISTING live providers
// by their linked symbols (1196/1198/1200/1187/1195 VM stack, 0078/0459/0093/0125/0286 CExoString,
// 0528/0529 vector helpers, 0004 allocator). Exports (unchanged names/ABI):
//   @recovered_vm_execute_file_005d4270@12, @recovered_vm_execute_command_0052c0d0@16
// New exports:
//   _recovered_vm_action_unimplemented  (named ACTION trap for the 0854 vm_required_* stubs)
// MEMORY: allocation only through live_operator_new / live_free (006FA7E6 / 006FA390 owners).
#include "1d7398170f-vm_execute_live.hpp"
#include "0bbae016e8-vm_scalar_live.hpp"
#include "5a827be7df-vm_execute_native_exports.hpp"
#include <cstdio>
#include <cstring>
#include <exception>
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long address,const char* reason);
unsigned __fastcall recovered_vm_push_integer_005d1850(void*,void*,unsigned);
unsigned __fastcall recovered_vm_push_float_005d1880(void*,void*,unsigned);
unsigned __fastcall recovered_vm_push_object_005d1a10(void*,void*,unsigned);
unsigned __fastcall recovered_vm_pop_integer_005d24c0(void*,void*,void*);
unsigned __fastcall recovered_vm_pop_float_005d2510(void*,void*,void*);
unsigned __fastcall recovered_vm_push_string_005d1930(void*,void*,void*);
unsigned __fastcall recovered_vm_pop_string_005d25e0(void*,void*,void*);
void __fastcall recovered_vm_copy_stack_005d1c40(void*,void*,int,int);
void __fastcall recovered_vm_stack_shrink_005d21f0(void*,void*,int);
const char* __fastcall WindowCStr005e5670(void*,void*);
void* __fastcall frame_LogStringCtor_005e5a90(void*,void*,const char*);
void __fastcall frame_LogStringDtor_005e5c20(void*,void*);
int __fastcall exostring_compare_no_case_005e6450(void*,void*,const void*);
struct W6Vector{float x,y,z;};
W6Vector* __cdecl vector_component_mult_00423bf0(W6Vector* out,const W6Vector* v,float scale);
W6Vector* __cdecl vector_divide_00437a90(W6Vector* out,const W6Vector* v,float divisor);
}
namespace kotor_reconstruction{struct StartupString;void __cdecl StartupFormat005e5680(StartupString*,const char*,...);}
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString*,void*,kotor_reconstruction::StartupString*,const kotor_reconstruction::StartupString*);
#include "91826d361f-vm_execute_x87.inc"
namespace {
using kotor::vm_execute_live::Word;using I32=std::int32_t;
void* ptr(Word a){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a));}
Word word(const void* p){return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p));}
struct NativeMemory final:kotor::server_mainloop::Memory{
 Word read(Word a)override{Word v;std::memcpy(&v,ptr(a),4);return v;}
 void write(Word a,Word v)override{std::memcpy(ptr(a),&v,4);}
 Word read16(Word a)override{std::uint16_t v;std::memcpy(&v,ptr(a),2);return v;}
 void write16(Word a,Word v)override{const std::uint16_t x=static_cast<std::uint16_t>(v);std::memcpy(ptr(a),&x,2);}
 Word read8(Word a)override{return *static_cast<const unsigned char*>(ptr(a));}
};
// Same contract as the 1196 Native ports (vm_scalar_live_outer_native.cpp); used only for AddToTopOfStack 005D15A0.
struct NativeScalar final:recovered_vm_scalar::Ports{
 Word read(Word a)override{Word v;std::memcpy(&v,ptr(a),4);return v;}
 Word byte(Word a)override{return *static_cast<unsigned char*>(ptr(a));}
 void write(Word a,Word v)override{std::memcpy(ptr(a),&v,4);}
 void write_byte(Word a,Word v)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(v);}
 Word allocate(Word n)override{return word(live_operator_new(n));}
 void release(Word a)override{live_free(ptr(a));}
 void copy(Word a,Word b,Word n)override{std::memcpy(ptr(a),ptr(b),n);}
 void shrink(Word a,Word n)override{recovered_vm_stack_shrink_005d21f0(ptr(a),nullptr,static_cast<int>(n));}
};
Word vtable_slot(Word object,Word offset){NativeMemory m;return m.read(m.read(object)+offset);}
float as_float(Word v){float f;std::memcpy(&f,&v,4);return f;}
}
extern "C" {
__declspec(noreturn) void __cdecl recovered_vm_action_unimplemented(unsigned handler){
 kotor_unreachable_trap(handler,"VM ACTION engine command body not bound");
}
}
namespace {
struct NativeVmExecute final:kotor::vm_execute_live::Ports{
 [[noreturn]] void stop(Word address,const char* what){kotor_unreachable_trap(address,what);}
 Word push(Word a,Word b,Word kind)override{if(kind==3)return recovered_vm_push_integer_005d1850(ptr(a),nullptr,b);if(kind==4)return recovered_vm_push_float_005d1880(ptr(a),nullptr,b);return recovered_vm_push_object_005d1a10(ptr(a),nullptr,b);}
 Word pop(Word a,Word*b,Word kind)override{return kind==3?recovered_vm_pop_integer_005d24c0(ptr(a),nullptr,b):recovered_vm_pop_float_005d2510(ptr(a),nullptr,b);}
 void trim(Word a,I32 b)override{recovered_vm_stack_shrink_005d21f0(ptr(a),nullptr,b);}
 void copy(Word a,I32 b,I32 c)override{recovered_vm_copy_stack_005d1c40(ptr(a+0x1ac),nullptr,b,c);}
 Word push_string(Word a,Word b)override{return recovered_vm_push_string_005d1930(ptr(a),nullptr,ptr(b));}
 Word pop_string(Word a,Word b)override{return recovered_vm_pop_string_005d25e0(ptr(a),nullptr,ptr(b));}
 void byte(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}
 void string_default_005b3190(Word a)override{NativeMemory m;m.write(a,0);m.write(a+4,0);}
 void string_chars_005e5a90(Word a,Word b)override{frame_LogStringCtor_005e5a90(ptr(a),nullptr,b==0x73d71c?"":static_cast<const char*>(ptr(b)));}
 Word string_cstr_005e5670(Word a)override{return word(WindowCStr005e5670(ptr(a),nullptr));}
 void string_destroy_005e5c20(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
 Word string_concat_005e5d10(Word a,Word b,Word c)override{return word(GuiConcat005e5d10(static_cast<kotor_reconstruction::StartupString*>(ptr(a)),nullptr,static_cast<kotor_reconstruction::StartupString*>(ptr(b)),static_cast<const kotor_reconstruction::StartupString*>(ptr(c))));}
 void string_format_005e5680(Word a,Word b,Word c)override{if(b!=0x73d5d0)stop(b,"CExoString::Format literal other than 0073D5D0");kotor_reconstruction::StartupFormat005e5680(static_cast<kotor_reconstruction::StartupString*>(ptr(a)),"%s",static_cast<const char*>(ptr(c)));}
 Word allocate_006fa7e6(Word a)override{return word(live_operator_new(a));}
 void free_006fa390(Word a)override{live_free(ptr(a));}
 bool divisor_is_zero_x87(Word a)override{return w6_x87::is_zero(a);}
 // Nest-safe: a command body may run a nested script (ExecuteScript/AssignCommand...) whose ACTIONs
 // overwrite the global context; restore the enclosing context when this command returns.
 I32 action_slot2(Word a,I32 b,Word c)override{using F=int(__thiscall*)(void*,int,unsigned);const I32 s=reinterpret_cast<F>(vtable_slot(a,8))(ptr(a),b,c);return s;}
 void error_slot4(Word a,Word b,Word c)override{using F=void(__thiscall*)(void*,void*,unsigned);reinterpret_cast<F>(vtable_slot(a,0x10))(ptr(a),ptr(b),c);}
 void add_to_top_005d15a0(Word stack,Word type)override{NativeScalar p;recovered_vm_scalar::add(p,stack,type);}
 Word compare_no_case_005e6450(Word a,Word b)override{return static_cast<Word>(exostring_compare_no_case_005e6450(ptr(a),nullptr,ptr(b)));}
 Word implementer_equal_slot7(Word a,Word type,Word top,Word lower)override{using F=int(__thiscall*)(void*,int,unsigned,unsigned);return static_cast<Word>(reinterpret_cast<F>(vtable_slot(a,0x1c))(ptr(a),static_cast<int>(type),top,lower));}
 Word implementer_copy_slot8(Word a,Word type,Word value)override{using F=unsigned(__thiscall*)(void*,int,unsigned);return reinterpret_cast<F>(vtable_slot(a,0x20))(ptr(a),static_cast<int>(type),value);}
 Word float_binary_x87(Word op,Word l,Word r)override{return w6_x87::binary(op,l,r);}
 Word float_compare_x87(Word op,Word l,Word r)override{return w6_x87::compare(op,l,r);}
 Word float_negate_x87(Word v)override{return w6_x87::negate(v);}
 Word int_to_float_x87(Word v)override{return w6_x87::from_int(v);}
 Word float_reload_x87(Word v)override{return w6_x87::reload(v);}
 void vector_component_mult_00423bf0(Word out,Word v,Word s)override{(void)::vector_component_mult_00423bf0(static_cast<W6Vector*>(ptr(out)),static_cast<const W6Vector*>(ptr(v)),as_float(s));}
 void vector_divide_00437a90(Word out,Word v,Word s)override{(void)::vector_divide_00437a90(static_cast<W6Vector*>(ptr(out)),static_cast<const W6Vector*>(ptr(v)),as_float(s));}
};
struct NativeVmDispatch final:kotor::vm_execute_live::DispatchPorts{I32 invoke(Word function,Word self,I32 routine,Word count)override{using F=int(__thiscall*)(void*,int,unsigned);return reinterpret_cast<F>(function)(ptr(self),routine,count);}};
}
extern "C" int __fastcall recovered_vm_execute_file_005d4270(void* self,void*,unsigned initial){
 NativeMemory m;NativeVmExecute p;
 alignas(4) unsigned char scratch[60]={};
 kotor::vm_execute_live::Runtime r{m,word(self),p,{word(scratch),word(scratch+8),word(scratch+16),word(scratch+24),word(scratch+36),word(scratch+48)}};
 try{return kotor::vm_execute_live::run_script_file_005d4270(r,initial);}
 catch(const kotor::vm_execute_live::CoverageStop& e){p.stop(e.address?e.address:0x005d2bd0,e.what());}
}
extern "C" int __fastcall recovered_vm_execute_command_0052c0d0(void* self,void*,int routine,unsigned count){NativeMemory m;NativeVmDispatch p;return kotor::vm_execute_live::execute_command_0052c0d0(m,p,word(self),routine,count);}

// t3: VM stack engine-structure entry points (internal and CVirtualMachine wrappers 005D10A0/005D10B0).
namespace {
struct EngineCall{NativeMemory m;NativeVmExecute p;alignas(4) unsigned char scratch[60]={};kotor::vm_execute_live::Runtime r;
 explicit EngineCall(Word vm):r{m,vm,p,{word(scratch),word(scratch+8),word(scratch+16),word(scratch+24),word(scratch+36),word(scratch+48)}}{}};
}
extern "C" unsigned __fastcall recovered_vm_push_engine_structure_005d19c0(void* vm,void*,int type,unsigned value){EngineCall c(word(vm));return kotor::vm_execute_live::push_engine_structure_005d19c0(c.r,static_cast<Word>(type),value);}
extern "C" unsigned __fastcall recovered_vm_pop_engine_structure_005d2630(void* vm,void*,int type,void* out){EngineCall c(word(vm));Word v=0;const Word ok=kotor::vm_execute_live::pop_engine_structure_005d2630(c.r,static_cast<Word>(type),v);if(ok)std::memcpy(out,&v,4);return ok;}
extern "C" unsigned __fastcall recovered_vm_outer_push_engine_structure_005d10b0(void* outer,void*,int type,unsigned value){NativeMemory m;return recovered_vm_push_engine_structure_005d19c0(ptr(m.read(word(outer))),nullptr,type,value);}
extern "C" unsigned __fastcall recovered_vm_outer_pop_engine_structure_005d10a0(void* outer,void*,int type,void* out){NativeMemory m;return recovered_vm_pop_engine_structure_005d2630(ptr(m.read(word(outer))),nullptr,type,out);}
extern "C" unsigned __fastcall recovered_vm_pop_vector_005d2560(void* vm,void*,void* out){EngineCall c(word(vm));const Word t=c.r.scratch.vector_b;std::memcpy(ptr(t),out,12);const Word ok=kotor::vm_execute_live::pop_vector_005d2560(c.r,t);std::memcpy(out,ptr(t),12);return ok;} // unpopped components keep the caller's values
// t4: CVirtualMachine vector wrappers 005D1040/005D1050 (requested by W1 for ACTIONs 27/143) and internal 005D18B0
extern "C" unsigned __fastcall recovered_vm_push_vector_005d18b0(void* vm,void*,unsigned x,unsigned y,unsigned z){EngineCall c(word(vm));return kotor::vm_execute_live::push_vector_005d18b0(c.r,x,y,z);}
extern "C" unsigned __fastcall recovered_vm_outer_pop_vector_005d1040(void* outer,void*,void* out){NativeMemory m;return recovered_vm_pop_vector_005d2560(ptr(m.read(word(outer))),nullptr,out);}
extern "C" unsigned __fastcall recovered_vm_outer_push_vector_005d1050(void* outer,void*,unsigned x,unsigned y,unsigned z){NativeMemory m;return recovered_vm_push_vector_005d18b0(ptr(m.read(word(outer))),nullptr,x,y,z);}
