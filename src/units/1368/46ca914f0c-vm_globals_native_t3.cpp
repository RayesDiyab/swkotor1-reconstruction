// W6 t3 native adapter: Global* ACTION bodies (CSWVirtualMachineCommands handlers, thiscall RET 8).
// Providers: VM stack (1196/1200 + W6 t3 engine push/pop), CExoString (0199/0459), FindIdentifierIndex 00529030
// (0855 recovered_global_catalog, bound with the same string providers as 0857's Native ports).
// MEMORY: live_free only (location copies returned by StackPopEngineStructure).
#include "8b63c5db7f-vm_globals.hpp"
#include "9f6f5e8482-global_catalog.hpp"
#include "5a827be7df-vm_execute_native_exports.hpp"
#include <cctype>
#include <cstring>
extern "C" {
void __cdecl live_free(void*);
unsigned __fastcall recovered_vm_push_string_005d1930(void*,void*,void*);
unsigned __fastcall recovered_vm_pop_string_005d25e0(void*,void*,void*);
void* __fastcall async_assign_string_005e5c50(void*,void*,void*);
void __fastcall frame_LogStringDtor_005e5c20(void*,void*);
const char* __fastcall frame_LogCStr_005e5670(void*,void*);
unsigned __fastcall async_string_length_005e5790(void*,void*);
}
namespace kotor_reconstruction{struct StartupString;}
namespace directory_list{char __fastcall At005e5600(kotor_reconstruction::StartupString*,void*,std::int32_t);}
namespace ini_comparison{std::int32_t __fastcall CompareNC005e58c0(kotor_reconstruction::StartupString*,void*,const char*);}
// Game singleton cells as linked symbols (the reconstruction does not place them at their original addresses):
//   007A39FC kotor_menu_window_cells::manager_007a39fc (0053), 007A3A00 frame_control::message_manager_007a3a00 (0456),
//   007A3A28 kotor_cell_rules_007a3a28 (0421)
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace frame_control { extern unsigned message_manager_007a3a00; }
extern "C" void* kotor_cell_rules_007a3a28;
namespace {
using kotor::vm_globals::Word;using I32=std::int32_t;
void* ptr(Word a){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a));}
Word word(const void* p){return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p));}
struct NativeMemory final:kotor::server_mainloop::Memory{
 Word read(Word a)override{Word v;std::memcpy(&v,ptr(a),4);return v;}
 void write(Word a,Word v)override{std::memcpy(ptr(a),&v,4);}
 Word read16(Word a)override{std::uint16_t v;std::memcpy(&v,ptr(a),2);return v;}
 void write16(Word a,Word v)override{const std::uint16_t x=static_cast<std::uint16_t>(v);std::memcpy(ptr(a),&x,2);}
 Word read8(Word a)override{return *static_cast<const unsigned char*>(ptr(a));}
};
using recovered_global_catalog::String;
// Same provider bindings as 0857 global_catalog_native.cpp Native (lookup subset).
struct Catalog final:recovered_global_catalog::Ports{
 int length(String*s)override{return static_cast<int>(async_string_length_005e5790(s,nullptr));}
 signed char character(String*s,int i)override{return static_cast<signed char>(directory_list::At005e5600(reinterpret_cast<kotor_reconstruction::StartupString*>(s),nullptr,i));}
 int uppercase(int c)override{return std::toupper(c);}
 const char* text(String*s)override{return frame_LogCStr_005e5670(s,nullptr);}
 int equal_nc(String*s,const char*t)override{return ini_comparison::CompareNC005e58c0(reinterpret_cast<kotor_reconstruction::StartupString*>(s),nullptr,t);}
};
struct NativeGlobals final:kotor::vm_globals::Ports{
 NativeMemory m;
 Word vm_internal()override{return m.read(frame_control::message_manager_007a3a00);}
 Word global_table()override{return m.read(m.read(word(kotor_menu_window_cells::manager_007a39fc)+8)+4)+0x100fc;} // 004AEE60
 I32 find_identifier_00529030(Word t,Word name)override{Catalog c;return recovered_global_catalog::find_00529030(ptr(t),static_cast<String*>(ptr(name)),c);}
 Word pop_string_005d25e0(Word vm,Word d)override{return recovered_vm_pop_string_005d25e0(ptr(vm),nullptr,ptr(d));}
 Word push_string_005d1930(Word vm,Word s)override{return recovered_vm_push_string_005d1930(ptr(vm),nullptr,ptr(s));}
 Word push_engine_structure_005d19c0(Word vm,Word type,Word value)override{return recovered_vm_push_engine_structure_005d19c0(ptr(vm),nullptr,static_cast<int>(type),value);}
 Word pop_engine_structure_005d2630(Word vm,Word type,Word* out)override{return recovered_vm_pop_engine_structure_005d2630(ptr(vm),nullptr,static_cast<int>(type),out);}
 void string_default_005b3190(Word s)override{std::memset(ptr(s),0,8);}
 void string_destroy_005e5c20(Word s)override{frame_LogStringDtor_005e5c20(ptr(s),nullptr);}
 void string_assign_005e5c50(Word d,Word s)override{(void)async_assign_string_005e5c50(ptr(d),nullptr,ptr(s));}
 void free_006fa390(Word a)override{live_free(ptr(a));}
};
struct Frame{alignas(4) unsigned char bytes[48]={};kotor::vm_globals::Scratch s{word(bytes),word(bytes+8),word(bytes+16),word(bytes+40)};};
}
namespace vg=kotor::vm_globals;
extern "C" {
int __fastcall recovered_vm_get_global_string_00538ed0(void*,void*,int,unsigned){NativeGlobals p;Frame f;return vg::get_global_string_00538ed0(p.m,p,f.s);}
int __fastcall recovered_vm_set_global_string_00542c60(void*,void*,int,unsigned){NativeGlobals p;Frame f;return vg::set_global_string_00542c60(p.m,p,f.s);}
int __fastcall recovered_vm_get_global_location_00538cd0(void*,void*,int,unsigned){NativeGlobals p;Frame f;return vg::get_global_location_00538cd0(p.m,p,f.s);}
int __fastcall recovered_vm_set_global_location_00542a50(void*,void*,int,unsigned){NativeGlobals p;Frame f;return vg::set_global_location_00542a50(p.m,p,f.s);}
}
