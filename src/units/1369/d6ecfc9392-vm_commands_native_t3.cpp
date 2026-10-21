// W6 t3 native adapter for the assigned ACTION bodies (EffectComplex, CreateItemOnObject, RevealMap,
// IsAvailableNPC, ItemActivated). Live providers are bound by linked symbol. A callee with no live provider
// is an import with an /alternatename fallback to a named trap at its original address. Handler ABI:
// int __fastcall (implementer, edx, routine, argc) == original thiscall RET 8.
#include "be45c700cb-vm_commands_t3.hpp"
#include "5a827be7df-vm_execute_native_exports.hpp"
#include "91826d361f-vm_execute_x87.inc"
#include <cmath>
#include <cstring>
extern "C" {
void* __fastcall recovered_object_array_004aed70(void*,void*);   // W2 t5 CServerExoApp::GetObjectArray
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long address,const char* reason);
unsigned __fastcall recovered_vm_pop_integer_005d24c0(void*,void*,void*);
unsigned __fastcall recovered_vm_pop_float_005d2510(void*,void*,void*);
unsigned __fastcall recovered_vm_pop_object_005d26a0(void*,void*,void*);
unsigned __fastcall recovered_vm_pop_string_005d25e0(void*,void*,void*);
unsigned __fastcall recovered_vm_push_integer_005d1850(void*,void*,unsigned);
unsigned __fastcall recovered_vm_push_object_005d1a10(void*,void*,unsigned);
void* __fastcall server_get_module_004ae6b0(void*,void*);
unsigned char __fastcall kotor_fastcall_004d8230(void*,void*,unsigned,void**);
void* __fastcall kotor_fastcall_004ae770(void*,void*,unsigned);
int __cdecl aurora_vector_equal_004aa980(const void*,const void*);
void* __cdecl normalize_004ab180(void*,const void*);
void* __fastcall recovered_effect_construct_00503e40(void*,void*,unsigned);
void __fastcall recovered_effect_integer_005036a0(void*,void*,unsigned,unsigned);
void __fastcall recovered_effect_creator_00503a00(void*,void*,unsigned);
void __fastcall recovered_effect_destroy_00503590(void*,void*);
void __fastcall frame_LogStringDtor_005e5c20(void*,void*);
const char* __fastcall WindowCStr005e5670(void*,void*);
void* __fastcall recovered_item_construct_005530a0(void*,void*,unsigned);
unsigned __fastcall recovered_item_template_005608b0(void*,void*,void*);
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void*,void*,int);
// imports; recovered_party_* are W2 w2_party_native bodies, w6_import_* bind real bodies via /alternatename below [stale-stubs: trap fallbacks removed]
unsigned __fastcall recovered_party_npc_available_005636b0(void*,void*,unsigned);
unsigned __fastcall recovered_party_npc_selectable_005637c0(void*,void*,unsigned);
void __fastcall w6_import_map_world_point_explored_005792d0(void*,void*,unsigned,unsigned,unsigned,unsigned);
void __fastcall w6_import_map_entire_explored_00578fa0(void*,void*);
void* __fastcall frame_R4_005ed690(void*,void*);   // 0457 live CClientExoApp::GetInGameGui
void __fastcall w6_import_gui_update_status_0062eeb0(void*,void*,unsigned,unsigned);
}
#pragma comment(linker, "/alternatename:@w6_import_map_world_point_explored_005792d0@24=@kotor_fastcall_005792d0@24")   // claude_0886_bodies_party SetWorldPointExplored(Vector,int) RET 10h
#pragma comment(linker, "/alternatename:@w6_import_map_entire_explored_00578fa0@8=@kotor_fastcall_00578fa0@8")   // claude_stale_bodies_w6 SetEntireMapExplored RET 0
#pragma comment(linker, "/alternatename:@w6_import_gui_update_status_0062eeb0@16=@kotor_fastcall_0062eeb0@16")   // W4 hud main_interface CGuiInGame::UpdateStatus RET 8
// @recovered_effect_destroy_00503590@8: W1 provider; fallback trap is declared once in vm_situation_native_t2.cpp
namespace menu_resources{struct Ref{Ref* construct_00406d80(const char*);};} // 0030 CResRef::CResRef(char const*)
// Game singleton cells as linked symbols (the reconstruction does not place them at their original addresses):
//   007A39FC kotor_menu_window_cells::manager_007a39fc (0053), 007A3A00 frame_control::message_manager_007a3a00 (0456),
//   007A3A28 kotor_cell_rules_007a3a28 (0421)
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace frame_control { extern unsigned message_manager_007a3a00; }
extern "C" void* kotor_cell_rules_007a3a28;
namespace {
using kotor::vm_commands_t3::Word;using I32=std::int32_t;
void* ptr(Word a){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a));}
Word word(const void* p){return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p));}
struct NativeMemory final:kotor::server_mainloop::Memory{
 Word read(Word a)override{Word v;std::memcpy(&v,ptr(a),4);return v;}
 void write(Word a,Word v)override{std::memcpy(ptr(a),&v,4);}
 Word read16(Word a)override{std::uint16_t v;std::memcpy(&v,ptr(a),2);return v;}
 void write16(Word a,Word v)override{const std::uint16_t x=static_cast<std::uint16_t>(v);std::memcpy(ptr(a),&x,2);}
 Word read8(Word a)override{return *static_cast<const unsigned char*>(ptr(a));}
};
Word thiscall0(Word object,Word offset){NativeMemory m;using F=void*(__thiscall*)(void*);return word(reinterpret_cast<F>(m.read(m.read(object)+offset))(ptr(object)));}
struct NativeCommands final:kotor::vm_commands_t3::Ports{
 NativeMemory m;
 Word vm(){return m.read(frame_control::message_manager_007a3a00);}
 void write8(Word a,Word v)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(v);}
 void write16(Word a,Word v)override{m.write16(a,v);}
 Word pop_integer(Word* o)override{return recovered_vm_pop_integer_005d24c0(ptr(vm()),nullptr,o);}
 Word pop_float(Word* o)override{return recovered_vm_pop_float_005d2510(ptr(vm()),nullptr,o);}
 Word pop_object(Word* o)override{return recovered_vm_pop_object_005d26a0(ptr(vm()),nullptr,o);}
 Word pop_string(Word d)override{return recovered_vm_pop_string_005d25e0(ptr(vm()),nullptr,ptr(d));}
 Word pop_vector(Word o)override{return recovered_vm_pop_vector_005d2560(ptr(vm()),nullptr,ptr(o));}
 Word push_integer(Word v)override{return recovered_vm_push_integer_005d1850(ptr(vm()),nullptr,v);}
 Word push_object(Word v)override{return recovered_vm_push_object_005d1a10(ptr(vm()),nullptr,v);}
 Word push_engine_structure(Word t,Word v)override{return recovered_vm_push_engine_structure_005d19c0(ptr(vm()),nullptr,static_cast<int>(t),v);}
 Word server_app()override{return m.read(word(kotor_menu_window_cells::manager_007a39fc)+8);}
 Word object_array_004aed70(Word s)override{return word(recovered_object_array_004aed70(ptr(s),nullptr));}
 Word client_app()override{return m.read(word(kotor_menu_window_cells::manager_007a39fc)+4);}
 Word get_module_004ae6b0(Word s)override{return word(server_get_module_004ae6b0(ptr(s),nullptr));}
 Word get_game_object_004d8230(Word a,Word id,Word* out)override{void* o=nullptr;const Word st=kotor_fastcall_004d8230(ptr(a),nullptr,id,&o);*out=word(o);return st;}
 Word get_creature_by_id_004ae770(Word s,Word id)override{return word(kotor_fastcall_004ae770(ptr(s),nullptr,id));}
 Word party_table_004aee70(Word s)override{return m.read(s+4)+0x1b770;} // 004AEE70 (inlined leaf)
 Word npc_available_005636b0(Word t,Word n)override{return recovered_party_npc_available_005636b0(ptr(t),nullptr,n);}
 Word npc_selectable_005637c0(Word t,Word n)override{return recovered_party_npc_selectable_005637c0(ptr(t),nullptr,n);}
 void map_world_point_explored_005792d0(Word map,Word x,Word y,Word z,Word v)override{w6_import_map_world_point_explored_005792d0(ptr(map),nullptr,x,y,z,v);}
 void map_entire_explored_00578fa0(Word map)override{w6_import_map_entire_explored_00578fa0(ptr(map),nullptr);}
 Word vector_equal_004aa980(Word a,Word b)override{return static_cast<Word>(aurora_vector_equal_004aa980(ptr(a),ptr(b)));}
 void vector_normalize_004ab180(Word out,Word in)override{(void)normalize_004ab180(ptr(out),ptr(in));}
 Word allocate_006fa7e6(Word n)override{return word(live_operator_new(n));}
 void free_006fa390(Word a)override{live_free(ptr(a));}
 Word effect_construct_00503e40(Word s,Word f)override{return word(recovered_effect_construct_00503e40(ptr(s),nullptr,f));}
 void effect_set_integer_005036a0(Word e,Word i,Word v)override{recovered_effect_integer_005036a0(ptr(e),nullptr,i,v);}
 void effect_set_creator_00503a00(Word e,Word id)override{recovered_effect_creator_00503a00(ptr(e),nullptr,id);}
 void effect_destroy_00503590(Word e)override{recovered_effect_destroy_00503590(ptr(e),nullptr);}
 Word ftol_mul_1000_x87(Word f)override{
  const float k=1000.0f;double prod=0;__asm{fld dword ptr f}__asm{fmul dword ptr k}__asm{fstp qword ptr prod}
  return w6_x87::ftol2_low32(prod);
 }
 void string_default_005b3190(Word s)override{std::memset(ptr(s),0,8);}
 void string_destroy_005e5c20(Word s)override{frame_LogStringDtor_005e5c20(ptr(s),nullptr);}
 Word string_cstr_005e5670(Word s)override{return word(WindowCStr005e5670(ptr(s),nullptr));}
 void resref_from_chars_00406d80(Word r,Word c)override{(void)static_cast<menu_resources::Ref*>(ptr(r))->construct_00406d80(static_cast<const char*>(ptr(c)));}
 Word item_construct_005530a0(Word s,Word id)override{return word(recovered_item_construct_005530a0(ptr(s),nullptr,id));}
 Word item_load_template_005608b0(Word i,Word r)override{return recovered_item_template_005608b0(ptr(i),nullptr,ptr(r));}
 void item_delete_virtual(Word i)override{using F=void*(__thiscall*)(void*,unsigned);(void)reinterpret_cast<F>(m.read(m.read(i)))(ptr(i),1);}
 Word base_item_005b31a0(Word a,Word b)override{return word(kotor_m15b_base_item_array_get_base_item_005b31a0(ptr(a),nullptr,static_cast<int>(b)));}
 Word object_virtual(Word o,Word off)override{return thiscall0(o,off);}
 void creature_acquire_item_005158e0(Word c,Word cell,Word a,Word b,Word f)override{recovered_creature_acquire_item_005158e0(ptr(c),nullptr,ptr(cell),a,b,f);}
 void placeable_acquire_item_00584b10(Word p,Word cell,Word a,Word f)override{recovered_placeable_acquire_item_00584b10(ptr(p),nullptr,ptr(cell),a,f);}
 void store_acquire_item_005c7610(Word s,Word i)override{recovered_store_acquire_item_005c7610(ptr(s),nullptr,ptr(i));}
 Word in_game_gui_005ed690(Word c)override{return word(frame_R4_005ed690(ptr(c),nullptr));}
 void gui_update_status_0062eeb0(Word g,Word a,Word b)override{w6_import_gui_update_status_0062eeb0(ptr(g),nullptr,a,b);}
 Word rules()override{return word(kotor_cell_rules_007a3a28);}
};
struct Frame{alignas(4) unsigned char bytes[0x40]={};kotor::vm_commands_t3::Frame f{word(bytes)};};
}
namespace vc=kotor::vm_commands_t3;
extern "C" {
int __fastcall recovered_vm_effect_complex_005313f0(void* implementer,void*,int routine,unsigned argc){NativeCommands p;return vc::effect_complex_005313f0(p.m,p,word(implementer),routine,argc);}
int __fastcall recovered_vm_create_item_on_object_0052f570(void*,void*,int,unsigned argc){NativeCommands p;Frame f;return vc::create_item_on_object_0052f570(p.m,p,argc,f.f);}
int __fastcall recovered_vm_reveal_map_00547240(void*,void*,int,unsigned){NativeCommands p;Frame f;return vc::reveal_map_00547240(p.m,p,f.f);}
int __fastcall recovered_vm_is_available_npc_0053ed70(void*,void*,int routine,unsigned){NativeCommands p;return vc::is_available_npc_0053ed70(p.m,p,routine);}
int __fastcall recovered_vm_item_activated_0053f220(void*,void*,int routine,unsigned){NativeCommands p;Frame f;return vc::item_activated_0053f220(p.m,p,routine,f.f);}
void __fastcall recovered_effect_set_object_id_005036f0(void* e,void*,unsigned index,unsigned id){NativeMemory m;vc::effect_set_object_id_005036f0(m,word(e),index,id);}
}
