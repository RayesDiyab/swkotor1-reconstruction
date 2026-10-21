// W6 t5 native adapter: handler ABI int __fastcall(implementer, edx, routine, argc) == original thiscall RET 8.
// Live providers bound by symbol; a callee without a callable live provider is an import whose /alternatename fallback
// is a named trap at its original address. Singleton cells are bound by symbol.
#include "6bd21b2ac4-vm_commands_t5.hpp"
#include "5a827be7df-vm_execute_native_exports.hpp"
#include <cstring>
extern "C" {
void* __fastcall recovered_object_array_004aed70(void*,void*);   // W2 t5 CServerExoApp::GetObjectArray
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long address,const char* reason);
unsigned __fastcall recovered_vm_pop_integer_005d24c0(void*,void*,void*);
unsigned __fastcall recovered_vm_pop_object_005d26a0(void*,void*,void*);
unsigned __fastcall recovered_vm_pop_string_005d25e0(void*,void*,void*);
unsigned __fastcall recovered_vm_push_integer_005d1850(void*,void*,unsigned);
unsigned __fastcall recovered_vm_push_object_005d1a10(void*,void*,unsigned);
unsigned __fastcall recovered_vm_push_string_005d1930(void*,void*,void*);
unsigned char __fastcall kotor_fastcall_004d8230(void*,void*,unsigned,void**);
void* __fastcall kotor_fastcall_004ae770(void*,void*,unsigned);
void* __fastcall server_get_module_004ae6b0(void*,void*);
void* __fastcall module_get_area_004c30f0(void*,void*);
void __fastcall frame_LogStringCtor_005e5a90(void*,void*,const char*);
void* __fastcall WindowCopy005e5b00(void*,void*,const void*);
void __fastcall frame_LogStringDtor_005e5c20(void*,void*);
const char* __fastcall WindowCStr005e5670(void*,void*);                                  // 0078 CExoString::CStr
// CSWSVirtualMachineCommands ShipBuild flag (original .data 00832834, zero-initialised; written by 00543840, never read)
unsigned recovered_ship_build_flag_00832834=0;
unsigned __cdecl recovered_get_locstring_server_00578870(void* player,void* loc,void* loc2,void* out,unsigned gender);   // W2 t4b
unsigned __fastcall recovered_object_add_action_004cea20(void*,void*,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,
  unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned);   // W1
void* __fastcall kotor_fastcall_004ae750(void*,void*,unsigned);
int __fastcall recovered_creature_incapacitated_004ef890(void*,void*);   // 0963 CSWSCreature::GetDeadTemp
unsigned __fastcall gui_load_slot_zero_0063e7f0(void*,void*);           // 0296 return_zero
void* __fastcall recovered_creature_visible_element_00517a20(void*,void*,unsigned);          // W1
int __fastcall recovered_object_get_reputation_0057cb80(void*,void*,unsigned,unsigned);      // W1
unsigned __fastcall recovered_object_acceptable_action_0057a240(void*,void*,unsigned);   // W1
void* __fastcall unresolved_list_next_005e9b90(void*,void*,void*);
void* __fastcall unresolved_list_at_pos_005e9b60(void*,void*,void*);                     // 0168 live CExoLinkedListInternal::GetAtPos                       // 0168 live CExoLinkedListInternal::GetNext
// CSWSCreatureStats::Get<ABILITY>Stat: imports; /alternatename defaults bind the W2 recovered_creature_stats_* bodies (w2_g07_native) [stale-stubs: trap defaults removed]
unsigned char __fastcall w6_import_creature_stats_str_005a6190(void*,void*);
unsigned char __fastcall w6_import_creature_stats_dex_005a6550(void*,void*);
unsigned char __fastcall w6_import_creature_stats_con_005a6250(void*,void*);
unsigned char __fastcall w6_import_creature_stats_int_005a6310(void*,void*);
unsigned char __fastcall w6_import_creature_stats_wis_005a63d0(void*,void*);
unsigned char __fastcall w6_import_creature_stats_cha_005a6490(void*,void*);
// CSWSMessage::SendServerToPlayerChatMessage(mode, id, CExoString by value, player, CExoString*) RET 18h: W2 recovered_send_chat_00571960 (w2_chat_native)
void __fastcall w6_import_send_chat_00571960(void*,void*,unsigned,unsigned,unsigned,unsigned,unsigned,void*);
}
#pragma comment(linker, "/alternatename:@w6_import_send_chat_00571960@32=@recovered_send_chat_00571960@32")
#pragma comment(linker, "/alternatename:@w6_import_creature_stats_str_005a6190@8=@recovered_creature_stats_str_005a6190@8")
#pragma comment(linker, "/alternatename:@w6_import_creature_stats_dex_005a6550@8=@recovered_creature_stats_dex_005a6550@8")
#pragma comment(linker, "/alternatename:@w6_import_creature_stats_con_005a6250@8=@recovered_creature_stats_con_005a6250@8")
#pragma comment(linker, "/alternatename:@w6_import_creature_stats_int_005a6310@8=@recovered_creature_stats_int_005a6310@8")
#pragma comment(linker, "/alternatename:@w6_import_creature_stats_wis_005a63d0@8=@recovered_creature_stats_wis_005a63d0@8")
#pragma comment(linker, "/alternatename:@w6_import_creature_stats_cha_005a6490@8=@recovered_creature_stats_cha_005a6490@8")
namespace kotor_reconstruction { struct StartupString; void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); }   // CExoString::Format
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace frame_control { extern unsigned message_manager_007a3a00; }
namespace {
using kotor::vm_commands_t5::Word;
void* ptr(Word a){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a));}
Word word(const void* p){return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p));}
struct NativeMemory final:kotor::server_mainloop::Memory{
 Word read(Word a)override{Word v;std::memcpy(&v,ptr(a),4);return v;}
 void write(Word a,Word v)override{std::memcpy(ptr(a),&v,4);}
 Word read16(Word a)override{std::uint16_t v;std::memcpy(&v,ptr(a),2);return v;}
 void write16(Word a,Word v)override{const std::uint16_t x=static_cast<std::uint16_t>(v);std::memcpy(ptr(a),&x,2);}
 Word read8(Word a)override{return *static_cast<const unsigned char*>(ptr(a));}
};
struct NativeT5 final:kotor::vm_commands_t5::Ports{
 NativeMemory m;
 Word vm(){return m.read(frame_control::message_manager_007a3a00);}
 Word pop_integer(Word* o)override{return recovered_vm_pop_integer_005d24c0(ptr(vm()),nullptr,o);}
 Word pop_object(Word* o)override{return recovered_vm_pop_object_005d26a0(ptr(vm()),nullptr,o);}
 Word pop_string(Word d)override{return recovered_vm_pop_string_005d25e0(ptr(vm()),nullptr,ptr(d));}
 Word push_integer(Word v)override{return recovered_vm_push_integer_005d1850(ptr(vm()),nullptr,v);}
 Word push_object(Word v)override{return recovered_vm_push_object_005d1a10(ptr(vm()),nullptr,v);}
 Word push_string(Word s)override{return recovered_vm_push_string_005d1930(ptr(vm()),nullptr,ptr(s));}
 Word server_app()override{return m.read(word(kotor_menu_window_cells::manager_007a39fc)+8);}
 Word object_array_004aed70(Word s)override{return word(recovered_object_array_004aed70(ptr(s),nullptr));}
 Word get_game_object_004d8230(Word a,Word id,Word* out)override{void* o=nullptr;const Word st=kotor_fastcall_004d8230(ptr(a),nullptr,id,&o);*out=word(o);return st;}
 Word get_creature_by_id_004ae770(Word s,Word id)override{return word(kotor_fastcall_004ae770(ptr(s),nullptr,id));}
 Word get_module_004ae6b0(Word s)override{return word(server_get_module_004ae6b0(ptr(s),nullptr));}
 Word module_get_area_004c30f0(Word mod)override{return word(::module_get_area_004c30f0(ptr(mod),nullptr));}
 Word object_virtual(Word o,Word off)override{using F=void*(__thiscall*)(void*);return word(reinterpret_cast<F>(m.read(m.read(o)+off))(ptr(o)));}
 Word get_locstring_server_00578870(Word pl,Word a,Word b,Word out,Word g)override{return recovered_get_locstring_server_00578870(ptr(pl),ptr(a),ptr(b),ptr(out),g);}
 void send_chat_00571960(Word msg,Word mode,Word id,Word s,Word pl,Word extra)override{w6_import_send_chat_00571960(ptr(msg),nullptr,mode,id,m.read(s),m.read(s+4),pl,ptr(extra));}
 Word add_action_004cea20(Word o,Word t,Word g,const Word (&q)[26])override{
  return recovered_object_add_action_004cea20(ptr(o),nullptr,t,g,q[0],q[1],q[2],q[3],q[4],q[5],q[6],q[7],q[8],q[9],q[10],q[11],q[12],q[13],q[14],q[15],q[16],q[17],q[18],q[19],q[20],q[21],q[22],q[23],q[24],q[25]);}
 Word get_game_object_004ae750(Word s,Word id)override{return word(kotor_fastcall_004ae750(ptr(s),nullptr,id));}
 Word acceptable_action_0057a240(Word o,Word t)override{return recovered_object_acceptable_action_0057a240(ptr(o),nullptr,t);}
 Word list_next_005e9b90(Word in,Word pos)override{return word(unresolved_list_next_005e9b90(ptr(in),nullptr,ptr(pos)));}
 Word visible_element_00517a20(Word c,Word id)override{return word(recovered_creature_visible_element_00517a20(ptr(c),nullptr,id));}
 Word get_reputation_0057cb80(Word o,Word id,Word f)override{return static_cast<Word>(recovered_object_get_reputation_0057cb80(ptr(o),nullptr,id,f));}
 Word dead_temp_004ef890(Word c)override{return static_cast<Word>(recovered_creature_incapacitated_004ef890(ptr(c),nullptr));}
 Word return_zero_0063e7f0(Word c)override{return gui_load_slot_zero_0063e7f0(ptr(c),nullptr);}
 Word list_at_pos_005e9b60(Word in,Word pos)override{return word(unresolved_list_at_pos_005e9b60(ptr(in),nullptr,ptr(pos)));}
 Word creature_stat(Word st,Word a)override{
  using F=unsigned char(__fastcall*)(void*,void*);
  static const F f[6]={&w6_import_creature_stats_str_005a6190,&w6_import_creature_stats_dex_005a6550,&w6_import_creature_stats_con_005a6250,
                       &w6_import_creature_stats_int_005a6310,&w6_import_creature_stats_wis_005a63d0,&w6_import_creature_stats_cha_005a6490};
  return f[a](ptr(st),nullptr);}
 void string_default_005b3190(Word s)override{std::memset(ptr(s),0,8);}
 void string_literal_005e5a90(Word s,const char* t)override{frame_LogStringCtor_005e5a90(ptr(s),nullptr,t);}
 void string_copy_005e5b00(Word d,Word s)override{(void)WindowCopy005e5b00(ptr(d),nullptr,ptr(s));}
 void string_destroy_005e5c20(Word s)override{frame_LogStringDtor_005e5c20(ptr(s),nullptr);}
 void string_format_hex_005e5680(Word out,Word v)override{kotor_reconstruction::StartupFormat005e5680(static_cast<kotor_reconstruction::StartupString*>(ptr(out)),"%x",v);}
 Word string_cstr_005e5670(Word s)override{return word(WindowCStr005e5670(ptr(s),nullptr));}
 void ship_build_flag_00832834(Word v)override{recovered_ship_build_flag_00832834=v;}
};
}
namespace v5=kotor::vm_commands_t5;
extern "C" {
int __fastcall recovered_vm_get_commandable_00536e40(void* self,void*,int,unsigned argc){NativeT5 p;return v5::get_commandable_00536e40(p.m,p,word(self),argc);}
int __fastcall recovered_vm_get_player_restrict_mode_0053cf70(void*,void*,int,unsigned){NativeT5 p;return v5::get_player_restrict_mode_0053cf70(p.m,p);}
int __fastcall recovered_vm_get_listen_pattern_number_0053bd20(void* self,void*,int,unsigned){NativeT5 p;return v5::get_listen_pattern_number_0053bd20(p.m,p,word(self));}
int __fastcall recovered_vm_get_blocking_door_00536a80(void* self,void*,int,unsigned){NativeT5 p;return v5::get_blocking_door_00536a80(p.m,p,word(self));}
int __fastcall recovered_vm_get_spell_cast_0053d110(void* self,void*,int routine,unsigned){NativeT5 p;return v5::get_spell_cast_0053d110(p.m,p,word(self),static_cast<Word>(routine));}
int __fastcall recovered_vm_get_name_0053c080(void*,void*,int,unsigned){NativeT5 p;alignas(4) unsigned char s[8]={};return v5::get_name_0053c080(p.m,p,word(s));}
int __fastcall recovered_vm_get_attempted_attack_target_00536860(void* self,void*,int,unsigned){NativeT5 p;return v5::get_attempted_attack_target_00536860(p.m,p,word(self));}
int __fastcall recovered_vm_get_last_perception_0053b2a0(void* self,void*,int routine,unsigned){NativeT5 p;return v5::get_last_perception_0053b2a0(p.m,p,word(self),static_cast<Word>(routine));}
int __fastcall recovered_vm_get_last_speaker_0053b410(void* self,void*,int,unsigned){NativeT5 p;return v5::get_last_speaker_0053b410(p.m,p,word(self));}
int __fastcall recovered_vm_get_current_action_00549040(void*,void*,int,unsigned){NativeT5 p;alignas(4) unsigned char pos[4]={};return v5::get_current_action_00549040(p.m,p,word(pos));}
int __fastcall recovered_vm_get_ability_score_005361b0(void*,void*,int,unsigned){NativeT5 p;return v5::get_ability_score_005361b0(p.m,p);}
// CSWSObject::GetAQActionIDByID(ushort type, int flag) RET 8 (this unused); returns AX
unsigned short __fastcall recovered_object_aq_action_id_by_id_0057a2b0(void*,void*,unsigned type,int flag){return static_cast<unsigned short>(v5::aq_action_id_by_id_0057a2b0(type&0xffff,flag!=0));}
int __fastcall recovered_vm_get_attempted_spell_target_005368b0(void* self,void*,int,unsigned){NativeT5 p;return v5::get_attempted_spell_target_005368b0(p.m,p,word(self));}
int __fastcall recovered_vm_get_last_hostile_actor_0053af40(void*,void*,int,unsigned){NativeT5 p;return v5::get_last_hostile_actor_0053af40(p.m,p);}
// CSWSObject::SetLastHostileActor(ulong actor, int force) RET 8
void __fastcall recovered_object_set_last_hostile_actor_004cc0c0(void* self,void*,unsigned actor,int force){NativeT5 p;v5::set_last_hostile_actor_004cc0c0(p.m,p,word(self),actor,static_cast<Word>(force));}
int __fastcall recovered_vm_get_attack_target_00536800(void*,void*,int,unsigned){NativeT5 p;return v5::get_attack_target_00536800(p.m,p);}
int __fastcall recovered_vm_get_is_dead_0053ecb0(void*,void*,int,unsigned){NativeT5 p;return v5::get_is_dead_0053ecb0(p.m,p);}
int __fastcall recovered_vm_get_last_damager_0053ad40(void* self,void*,int,unsigned){NativeT5 p;return v5::get_last_damager_0053ad40(p.m,p,word(self));}
int __fastcall recovered_vm_object_to_string_00540170(void*,void*,int,unsigned){NativeT5 p;alignas(4) unsigned char t[8]={},s[8]={};return v5::object_to_string_00540170(p.m,p,word(t),word(s));}
int __fastcall recovered_vm_ship_build_00543840(void*,void*,int,unsigned){NativeT5 p;return v5::ship_build_00543840(p.m,p);}
int __fastcall recovered_vm_get_object_visibility_0053c780(void*,void*,int routine,unsigned){NativeT5 p;return v5::get_object_visibility_0053c780(p.m,p,static_cast<Word>(routine));}
int __fastcall recovered_vm_get_emotions_00537ce0(void* self,void*,int routine,unsigned argc){NativeT5 p;return v5::get_emotions_00537ce0(p.m,p,word(self),static_cast<Word>(routine),argc);}
int __fastcall recovered_vm_get_party_ai_style_005459a0(void*,void*,int,unsigned){NativeT5 p;return v5::get_party_ai_style_005459a0(p.m,p);}
int __fastcall recovered_vm_get_npc_ai_style_005459d0(void*,void*,int,unsigned){NativeT5 p;return v5::get_npc_ai_style_005459d0(p.m,p);}
int __fastcall recovered_vm_get_user_actions_pending_0053e1b0(void* self,void*,int,unsigned){NativeT5 p;alignas(4) unsigned char pos[4]={};return v5::get_user_actions_pending_0053e1b0(p.m,p,word(self),word(pos));}
// CSWSCreature::GetUserActionsPending() RET 0
int __fastcall recovered_creature_user_actions_pending_00518b30(void* self,void*){NativeT5 p;alignas(4) unsigned char pos[4]={};return static_cast<int>(v5::creature_user_actions_pending_00518b30(p.m,p,word(self),word(pos)));}
int __fastcall recovered_vm_speak_string_00544090(void* self,void*,int routine,unsigned){
 NativeT5 p;alignas(4) unsigned char s[8]={},empty[8]={},copy[8]={};alignas(4) unsigned char mode[4]={};
 return v5::speak_string_00544090(p.m,p,word(self),static_cast<Word>(routine),word(s),word(empty),word(copy),word(mode));}
}
