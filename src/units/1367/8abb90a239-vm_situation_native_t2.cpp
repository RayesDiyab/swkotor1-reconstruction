// W6 t2b native adapter: implementer slots 4/6/7/8 and script-situation entry points.
// Every Ports slot binds an EXISTING live provider by its linked symbol (strong references, no trap fallbacks):
//   @recovered_vm_read_script_file_005d2260@12 -> 0886 ReadScriptFile export
//   @recovered_effect_destroy_00503590@8       -> W1 CGameEffect::~CGameEffect (w1_native_b7)
// CopyScriptEvent 004D77F0 is W6-owned since t2b (portable, vm_situation.cpp).
// MEMORY: live_operator_new / live_free only.
#include "7acd3f8ce2-vm_situation.hpp"
#include "6bff443bba-array_iterator.h"
#include "3b1e255cd0-vm_construct.hpp"
#include "5a827be7df-vm_execute_native_exports.hpp"
#include <cstring>
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long address,const char* reason);
void __fastcall recovered_vm_stack_clear_005d2140(void*,void*);
unsigned __fastcall recovered_vm_script_delete_005d44f0(void*,void*,void*);
void* __fastcall async_assign_string_005e5c50(void*,void*,void*);
int __fastcall async_string_equal_005e5200(void*,void*,void*);
void __fastcall frame_LogStringDtor_005e5c20(void*,void*);
void* __fastcall GuiAssign005e5140(void*,void*,const char*);
void* __fastcall recovered_effect_construct_00503e40(void*,void*,unsigned);
void __fastcall recovered_effect_copy_00504090(void*,void*,void*,unsigned);
void* __fastcall recovered_event_construct_004d7540(void*,void*);
void __fastcall recovered_event_destroy_004d7590(void*,void*);
int __cdecl aurora_vector_equal_004aa980(const void*,const void*);
// imports with trap fallbacks
int __fastcall recovered_vm_read_script_file_005d2260(void*,void*,void*);
void __fastcall recovered_effect_destroy_00503590(void*,void*);
void __fastcall kotor_fastcall_00405080(void*,void*,int);
void __fastcall c2da_current_string_destroy_005e5c20(void*,void*);
}
namespace kotor_reconstruction{struct ArrayMutations;}
namespace menu_list_growth{void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*,void*,unsigned);} // 0098, CExoArrayList::Allocate
namespace {
using kotor::vm_situation::Word;using I32=std::int32_t;
void* ptr(Word a){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a));}
Word word(const void* p){return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p));}
struct NativeMemory final:kotor::server_mainloop::Memory{
 Word read(Word a)override{Word v;std::memcpy(&v,ptr(a),4);return v;}
 void write(Word a,Word v)override{std::memcpy(ptr(a),&v,4);}
 Word read16(Word a)override{std::uint16_t v;std::memcpy(&v,ptr(a),2);return v;}
 void write16(Word a,Word v)override{const std::uint16_t x=static_cast<std::uint16_t>(v);std::memcpy(ptr(a),&x,2);}
 Word read8(Word a)override{return *static_cast<const unsigned char*>(ptr(a));}
};
struct NativeSituation final:kotor::vm_situation::Ports{
 void write8(Word a,Word v)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(v);}
 Word allocate_006fa7e6(Word n)override{return word(live_operator_new(n));}
 void free_006fa390(Word a)override{live_free(ptr(a));}
 void stack_clear_005d2140(Word s)override{recovered_vm_stack_clear_005d2140(ptr(s),nullptr);}
 void string_default_005b3190(Word s)override{std::memset(ptr(s),0,8);}
 void string_assign_005e5c50(Word d,Word s)override{(void)async_assign_string_005e5c50(ptr(d),nullptr,ptr(s));}
 void string_destroy_005e5c20(Word s)override{frame_LogStringDtor_005e5c20(ptr(s),nullptr);}
 Word string_not_equal_005e5310(Word a,Word b)override{return async_string_equal_005e5200(ptr(a),nullptr,ptr(b))?0u:1u;}
 // CVirtualMachineScript::CVirtualMachineScript 005D1740, transcribed directly (the 0851 frame_005d1740 port path only
 // ever used its two string slots; the six unused Ports slots were traps) [stale-stubs]
 void script_construct_005d1740(Word s)override{
  std::memset(static_cast<char*>(ptr(s))+0x18,0,8);                        // 005D175A..005D1764 CExoString::CExoString (005B3190)
  GuiAssign005e5140(static_cast<char*>(ptr(s))+0x18,nullptr,"");           // 005D176B..005D1776 CExoString::operator=("") (005E5140)
  static const unsigned zero_offsets[8]={0U,4U,8U,12U,16U,20U,32U,36U};
  for(unsigned o:zero_offsets){const Word z=0;std::memcpy(static_cast<char*>(ptr(s))+o,&z,4);}   // 005D177F..005D1793
 }
 Word implementer_copy_slot8(Word impl,Word type,Word value)override{NativeMemory m;using F=unsigned(__thiscall*)(void*,int,unsigned);return reinterpret_cast<F>(m.read(m.read(impl)+0x20))(ptr(impl),static_cast<int>(type),value);}
 I32 read_script_file_005d2260(Word vm,Word name)override{return recovered_vm_read_script_file_005d2260(ptr(vm),nullptr,ptr(name));}
 void delete_script_005d44f0(Word,Word d)override{(void)recovered_vm_script_delete_005d44f0(nullptr,nullptr,ptr(d));}
 I32 run_script_file_005d4270(Word vm,Word pc)override{return recovered_vm_execute_file_005d4270(ptr(vm),nullptr,pc);}
 Word effect_construct_00503e40(Word s,Word f)override{return word(recovered_effect_construct_00503e40(ptr(s),nullptr,f));}
 void effect_copy_00504090(Word s,Word src,Word f)override{recovered_effect_copy_00504090(ptr(s),nullptr,ptr(src),f);}
 void effect_destroy_00503590(Word s)override{recovered_effect_destroy_00503590(ptr(s),nullptr);}
 Word event_construct_004d7540(Word s)override{return word(recovered_event_construct_004d7540(ptr(s),nullptr));}
 void list_allocate_005e03c0(Word l,Word n)override{menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(ptr(l)),nullptr,n);}
 void string_list_allocate_00405080(Word l,Word n)override{kotor_fastcall_00405080(ptr(l),nullptr,static_cast<int>(n));}
 void string_array_destroy_006fa852(Word d,Word n)override{c2da_destroy_array_006fa852(ptr(d),8,n,&c2da_current_string_destroy_005e5c20);}
 void event_destroy_004d7590(Word s)override{recovered_event_destroy_004d7590(ptr(s),nullptr);}
 Word vector_equal_004aa980(Word a,Word b)override{return static_cast<Word>(aurora_vector_equal_004aa980(ptr(a),ptr(b)));}
};
Word internal_of(void* outer){NativeMemory m;return m.read(word(outer));}
}
namespace vs=kotor::vm_situation;
extern "C" {
// ---- command implementer slots (CSWVirtualMachineCommands vtable 007450E4) ----
// slot 4 (ReportError 005B5E90, `ret 8`): bound to the live 0296 provider @gui_load_slot_ret8_005b5e90@16 (t3b); no W6 copy.
void __fastcall recovered_vm_commands_destroy_engine_00548360(void*,void*,int type,unsigned value){NativeMemory m;NativeSituation p;vs::destroy_game_defined_structure_00548360(m,p,static_cast<Word>(type),value);}
int __fastcall recovered_vm_commands_equal_engine_0052c510(void*,void*,int type,unsigned a,unsigned b){NativeMemory m;NativeSituation p;return static_cast<int>(vs::get_equal_game_defined_structure_0052c510(m,p,static_cast<Word>(type),a,b));}
unsigned __fastcall recovered_vm_commands_copy_engine_0052c140(void*,void*,int type,unsigned source){NativeMemory m;NativeSituation p;return vs::copy_game_defined_structure_0052c140(m,p,static_cast<Word>(type),source);}
// ---- script situations: CVirtualMachineInternal ----
int __fastcall recovered_vm_internal_stack_pop_command_005d4380(void* vm,void*,void* out){NativeMemory m;NativeSituation p;return static_cast<int>(vs::stack_pop_command_005d4380(m,p,word(vm),word(out)));}
void __fastcall recovered_vm_internal_delete_script_situation_005d48e0(void*,void*,void* s){NativeMemory m;NativeSituation p;vs::delete_script_situation_005d48e0(m,p,word(s));}
int __fastcall recovered_vm_internal_run_script_situation_005d4ad0(void* vm,void*,void* s,unsigned self,unsigned flag){NativeMemory m;NativeSituation p;return static_cast<int>(vs::run_script_situation_005d4ad0(m,p,word(vm),word(s),self,flag));}
// ---- CVirtualMachine wrappers (this = CVirtualMachine*, [this] = CVirtualMachineInternal*) ----
int __fastcall recovered_vm_stack_pop_command_005d10e0(void* outer,void*,void* out){return recovered_vm_internal_stack_pop_command_005d4380(ptr(internal_of(outer)),nullptr,out);}
void __fastcall recovered_vm_delete_script_situation_005d1110(void*,void*,void* s){recovered_vm_internal_delete_script_situation_005d48e0(nullptr,nullptr,s);}
int __fastcall recovered_vm_run_script_situation_005d0fd0(void* outer,void*,void* s,unsigned self,unsigned flag){return recovered_vm_internal_run_script_situation_005d4ad0(ptr(internal_of(outer)),nullptr,s,self,flag);}
// W5's declared import name for 005D1110 (w5_foreign_traps.cpp alternatename target)
void __fastcall w5_import_vm_delete_script_situation_005d1110(void* a,void* b,void* s){recovered_vm_delete_script_situation_005d1110(a,b,s);}
}

// Native facade only: reuse the existing1194a constructor and this unit's real ports.
extern "C" void* __fastcall recovered_talent_construct_004ca9e0(void* self, void*) {
 NativeMemory m; NativeSituation p;
 return ptr(vs::talent_construct_004ca9e0(m, p, word(self)));
}
