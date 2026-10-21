// W6 t4 native adapter: ApplyEffectToObject, AddAvailableNPCByTemplate, SetAreaUnescapable, IsObjectPartyMember
// (handler ABI int __fastcall(implementer, edx, routine, argc) == thiscall RET 8) and the AcquireItem trio with their
// original thiscall ABIs. Live providers are bound by symbol. A callee with no callable live provider is an import
// whose /alternatename fallback is a named trap at its original address. Singleton cells are bound by symbol.
#include "666c149404-vm_commands_t4.hpp"
#include "5a827be7df-vm_execute_native_exports.hpp"
#include <cstring>
extern "C" {
void* __fastcall recovered_object_array_004aed70(void*,void*);   // W2 t5 CServerExoApp::GetObjectArray
void __cdecl live_free(void*);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long address,const char* reason);
unsigned __fastcall recovered_vm_pop_integer_005d24c0(void*,void*,void*);
unsigned __fastcall recovered_vm_pop_float_005d2510(void*,void*,void*);
unsigned __fastcall recovered_vm_pop_object_005d26a0(void*,void*,void*);
unsigned __fastcall recovered_vm_pop_string_005d25e0(void*,void*,void*);
unsigned __fastcall recovered_vm_push_integer_005d1850(void*,void*,unsigned);
unsigned char __fastcall kotor_fastcall_004d8230(void*,void*,unsigned,void**);
void* __fastcall kotor_fastcall_004ae750(void*,void*,unsigned);
void* __fastcall recovered_item_lookup_004ae760(void*,void*,unsigned);
void* __fastcall kotor_fastcall_004ae770(void*,void*,unsigned);
void* __fastcall server_get_module_004ae6b0(void*,void*);
void* __fastcall module_get_area_004c30f0(void*,void*);
void __fastcall recovered_effect_apply_004d14f0(void*,void*,void*,unsigned,unsigned);
void __fastcall recovered_effect_destroy_00503590(void*,void*);
void __fastcall frame_LogStringDtor_005e5c20(void*,void*);
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void*,void*,int);
void* __fastcall recovered_creature_inventory_004ef770(void*,void*,unsigned);
unsigned __fastcall recovered_inventory_insert_0055d330(void*,void*,void**,unsigned,unsigned,unsigned);
void __fastcall recovered_item_remove_area_005538a0(void*,void*);
void __fastcall recovered_item_owner_00553210(void*,void*,unsigned,unsigned,unsigned,unsigned);
// imports: W2 w2_party_native / 0886 recovered_* bodies (strong references) [stale-stubs: trap fallbacks removed]
unsigned __fastcall recovered_party_add_npc_005645f0(void*,void*,int,void*);
unsigned __fastcall recovered_creature_remove_item_00510ef0(void*,void*,void*,unsigned,unsigned,unsigned);
unsigned __fastcall recovered_placeable_remove_item_00584ac0(void*,void*,void*);
unsigned __fastcall recovered_remove_item_00555fd0(void*,void*,void*);   // W2 t4b export
unsigned __fastcall recovered_find_item_with_tag_00555ed0(void*,void*,void*);   // W2 t4b export
unsigned __fastcall recovered_compare_item_00553cf0(void*,void*,void*);   // assigned to W2 (adapting 1159); expected name
void __fastcall recovered_item_set_identified_00554170(void*,void*,unsigned);
unsigned __fastcall recovered_store_add_item_005c70c0(void*,void*,void*);
}
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace frame_control { extern unsigned message_manager_007a3a00; }
extern "C" void* kotor_cell_rules_007a3a28;
namespace {
using kotor::vm_commands_t4::Word;using I32=std::int32_t;
void* ptr(Word a){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a));}
Word word(const void* p){return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p));}
struct NativeMemory final:kotor::server_mainloop::Memory{
 Word read(Word a)override{Word v;std::memcpy(&v,ptr(a),4);return v;}
 void write(Word a,Word v)override{std::memcpy(ptr(a),&v,4);}
 Word read16(Word a)override{std::uint16_t v;std::memcpy(&v,ptr(a),2);return v;}
 void write16(Word a,Word v)override{const std::uint16_t x=static_cast<std::uint16_t>(v);std::memcpy(ptr(a),&x,2);}
 Word read8(Word a)override{return *static_cast<const unsigned char*>(ptr(a));}
};
struct NativeT4 final:kotor::vm_commands_t4::Ports{
 NativeMemory m;
 Word vm(){return m.read(frame_control::message_manager_007a3a00);}
 void write16(Word a,Word v)override{m.write16(a,v);}
 Word pop_integer(Word* o)override{return recovered_vm_pop_integer_005d24c0(ptr(vm()),nullptr,o);}
 Word pop_float(Word* o)override{return recovered_vm_pop_float_005d2510(ptr(vm()),nullptr,o);}
 Word pop_object(Word* o)override{return recovered_vm_pop_object_005d26a0(ptr(vm()),nullptr,o);}
 Word pop_string(Word d)override{return recovered_vm_pop_string_005d25e0(ptr(vm()),nullptr,ptr(d));}
 Word pop_engine_structure(Word t,Word* o)override{return recovered_vm_pop_engine_structure_005d2630(ptr(vm()),nullptr,static_cast<int>(t),o);}
 Word push_integer(Word v)override{return recovered_vm_push_integer_005d1850(ptr(vm()),nullptr,v);}
 Word server_app()override{return m.read(word(kotor_menu_window_cells::manager_007a39fc)+8);}
 Word object_array_004aed70(Word s)override{return word(recovered_object_array_004aed70(ptr(s),nullptr));}
 Word get_game_object_004d8230(Word a,Word id,Word* out)override{void* o=nullptr;const Word st=kotor_fastcall_004d8230(ptr(a),nullptr,id,&o);*out=word(o);return st;}
 Word get_game_object_004ae750(Word s,Word id)override{return word(kotor_fastcall_004ae750(ptr(s),nullptr,id));}
 Word get_item_004ae760(Word s,Word id)override{return word(recovered_item_lookup_004ae760(ptr(s),nullptr,id));}
 Word get_creature_by_id_004ae770(Word s,Word id)override{return word(kotor_fastcall_004ae770(ptr(s),nullptr,id));}
 Word get_module_004ae6b0(Word s)override{return word(server_get_module_004ae6b0(ptr(s),nullptr));}
 Word module_get_area_004c30f0(Word mod)override{return word(::module_get_area_004c30f0(ptr(mod),nullptr));}
 Word party_add_npc_005645f0(Word party,Word npc,Word s)override{return recovered_party_add_npc_005645f0(ptr(party),nullptr,static_cast<int>(npc),ptr(s));}
 Word object_virtual(Word o,Word off)override{using F=void*(__thiscall*)(void*);return word(reinterpret_cast<F>(m.read(m.read(o)+off))(ptr(o)));}
 void effect_apply_004d14f0(Word o,Word e,Word a,Word b)override{recovered_effect_apply_004d14f0(ptr(o),nullptr,ptr(e),a,b);}
 void effect_destroy_00503590(Word e)override{recovered_effect_destroy_00503590(ptr(e),nullptr);}
 void free_006fa390(Word a)override{live_free(ptr(a));}
 void string_default_005b3190(Word s)override{std::memset(ptr(s),0,8);}
 void string_destroy_005e5c20(Word s)override{frame_LogStringDtor_005e5c20(ptr(s),nullptr);}
 Word rules()override{return word(kotor_cell_rules_007a3a28);}
 Word base_item_005b31a0(Word a,Word b)override{return word(kotor_m15b_base_item_array_get_base_item_005b31a0(ptr(a),nullptr,static_cast<int>(b)));}
 Word creature_item_repository_004ef770(Word c,Word sh)override{return word(recovered_creature_inventory_004ef770(ptr(c),nullptr,sh));}
 Word repository_add_item_0055d330(Word r,Word cell,Word a,Word b,Word c)override{return recovered_inventory_insert_0055d330(ptr(r),nullptr,static_cast<void**>(ptr(cell)),a,b,c);}
 void repository_remove_item_00555fd0(Word r,Word i)override{(void)recovered_remove_item_00555fd0(ptr(r),nullptr,ptr(i));}
 Word repository_find_item_with_tag_00555ed0(Word r,Word t)override{return recovered_find_item_with_tag_00555ed0(ptr(r),nullptr,ptr(t));}
 void creature_remove_item_00510ef0(Word c,Word i,Word a,Word b,Word d)override{(void)recovered_creature_remove_item_00510ef0(ptr(c),nullptr,ptr(i),a,b,d);}
 void placeable_remove_item_00584ac0(Word p,Word i)override{(void)recovered_placeable_remove_item_00584ac0(ptr(p),nullptr,ptr(i));}
 void item_remove_from_area_005538a0(Word i)override{recovered_item_remove_area_005538a0(ptr(i),nullptr);}
 void item_set_possessor_00553210(Word i,Word o,Word a,Word b,Word prev)override{recovered_item_owner_00553210(ptr(i),nullptr,o,a,b,prev);}
 void item_set_identified_00554170(Word i,Word v)override{recovered_item_set_identified_00554170(ptr(i),nullptr,v);}
 Word item_compare_00553cf0(Word i,Word o)override{return recovered_compare_item_00553cf0(ptr(i),nullptr,ptr(o));}
 void item_delete_virtual(Word i)override{using F=void*(__thiscall*)(void*,unsigned);(void)reinterpret_cast<F>(m.read(m.read(i)))(ptr(i),1);}
 Word store_add_item_to_inventory_005c70c0(Word s,Word cell)override{return recovered_store_add_item_005c70c0(ptr(s),nullptr,ptr(cell));}
};
}
namespace v4=kotor::vm_commands_t4;
extern "C" {
int __fastcall recovered_vm_apply_effect_to_object_0052e570(void*,void*,int,unsigned){NativeT4 p;return v4::apply_effect_to_object_0052e570(p.m,p);}
int __fastcall recovered_vm_add_available_npc_by_template_0052dcd0(void*,void*,int,unsigned){NativeT4 p;alignas(4) unsigned char s[8]={};return v4::add_available_npc_by_template_0052dcd0(p.m,p,word(s));}
int __fastcall recovered_vm_set_area_unescapable_005424e0(void*,void*,int,unsigned){NativeT4 p;return v4::set_area_unescapable_005424e0(p.m,p);}
int __fastcall recovered_vm_is_object_party_member_0053f160(void*,void*,int,unsigned){NativeT4 p;return v4::is_object_party_member_0053f160(p.m,p);}
// CSWSCreature::AcquireItem(CSWSItem** item, ulong container, ulong repository_owner, int flag) RET 10
unsigned __fastcall recovered_creature_acquire_item_005158e0(void* self,void*,void* cell,unsigned container,unsigned owner,unsigned flag){NativeT4 p;return v4::creature_acquire_item_005158e0(p.m,p,word(self),word(cell),container,owner,flag);}
// CSWSPlaceable::AcquireItem(CSWSItem** item, ulong container, int flag) RET C
unsigned __fastcall recovered_placeable_acquire_item_00584b10(void* self,void*,void* cell,unsigned container,unsigned flag){NativeT4 p;return v4::placeable_acquire_item_00584b10(p.m,p,word(self),word(cell),container,flag);}
// CSWSStore::AcquireItem(CSWSItem* item) RET 4 -- the by-value parameter slot is the item cell AddItemToInventory receives
unsigned __fastcall recovered_store_acquire_item_005c7610(void* self,void*,void* item){NativeT4 p;void* cell=item;return v4::store_acquire_item_005c7610(p.m,p,word(self),word(&cell));}
}
