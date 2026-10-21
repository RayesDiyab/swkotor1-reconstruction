#include "2bce37c0e5-player_construct.hpp"
#include "b2575b6f75-player_lowers.hpp"
#include <cstring>
#include <new>
#if !defined(_M_IX86)
#error Connection player bridge requires x86
#endif
namespace menu_tables_m41::cells{extern const unsigned int invalid_object_00746644;}
namespace recovered_server_create{void* construct_list_internal_005e9aa0(void*);}
namespace kotor_list{struct Internal;}
namespace base_ini_reconstruction{void* __fastcall remove_head_005e9c80(kotor_list::Internal*,void*);}
extern "C" {

extern void* kotor_cell_rules_007a3a28;
void* __fastcall kotor_m15_combat_information_construct_00552280(void*,void*);
void __fastcall GuiAssign005e5140(void*,void*,const char*);
void* __fastcall c2da_default_ref_00405ed0(void*,void*);
void* __fastcall unresolved_ref_assign_00406290(void*,void*,void*,const char*);
void __cdecl live_free(void*);
void __fastcall recovered_last_update_destroy_005ca6a0(void*,void*);        // CLastUpdateObject::~CLastUpdateObject [w2_g07_native]
void __fastcall kotor_fastcall_005cb040(void*,void*);                       // ~CSWCreatureStatsUpdate [claude_p2s_a_gui]
void __fastcall recovered_player_last_destroy_005cad70(void*,void*);        // ~CSWSPlayerLastUpdateObject [w2_g07_native]
void __fastcall kotor_fastcall_0059ea20(void*,void*);                       // ~CSWSPlayerJournalQuestUpdates [claude_dialogue_start]
void __stdcall c2da_destroy_array_006fa852(void* first,unsigned size,int count,void* destroy); // eh vector destructor iterator [0186]
void __fastcall frame_LogStringDtor_005e5c20(void*,void*);                  // ~CExoString
}
namespace base_ini_reconstruction{void __fastcall destroy_list_005e9ab0(kotor_list::Internal*,void*);}   // ~CExoLinkedList [0091]
namespace {
using Delete=void*(__fastcall*)(void*,void*,unsigned);using Get=void*(__fastcall*)(void*,void*);using Flag=unsigned(__fastcall*)(void*,void*);
void* __fastcall base_delete(void*,void*,unsigned);void* __fastcall short_delete(void*,void*,unsigned);
void* __fastcall full_delete(void*,void*,unsigned);
void* __fastcall zero(void*,void*){return nullptr;}void* __fastcall identity(void*s,void*){return s;}unsigned __fastcall one(void*,void*){return 1;}
struct BaseTable{Delete destroy;Get none;Flag flag;};struct ShortTable{Delete destroy;Get none,self;};struct FullTable{Delete destroy;Get self;};
const BaseTable base_table{base_delete,zero,one};const ShortTable short_table{short_delete,zero,identity};const FullTable full_table{full_delete,identity};
static_assert(sizeof(BaseTable)==12&&sizeof(ShortTable)==12&&sizeof(FullTable)==8,"Original player vtable widths");
void table(void*s,const void*t){std::memcpy(s,&t,4);}
// closeC: 005CA900 CSWSPlayerLUOInventory::~CSWSPlayerLUOInventory (thiscall RET 0): reset 18 slots (+0 invalid, +48h -1),
// drain the three lists +90h/+98h/+9Ch (RemoveHead + free), +94h = invalid, then destroy/free +9Ch, +98h, +90h.
void*& field(void*s,unsigned o){return *reinterpret_cast<void**>(static_cast<unsigned char*>(s)+o);}
unsigned& word(void*s,unsigned o){return *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(s)+o);}
void drain(void*s,unsigned o){                                              // 005CA93E..6E (inlined ClearContainer 005CA4A0)
 if(*static_cast<void**>(field(s,o))==nullptr)return;
 do{live_free(base_ini_reconstruction::remove_head_005e9c80(static_cast<kotor_list::Internal*>(field(s,o)),nullptr));}while(*static_cast<void**>(field(s,o))!=nullptr);
}
void destroy_list(void*s,unsigned o){void*l=field(s,o);if(l){base_ini_reconstruction::destroy_list_005e9ab0(static_cast<kotor_list::Internal*>(l),nullptr);live_free(l);}}
void __fastcall luo_inventory_destroy_005ca900(void*s,void*){
 for(unsigned i=0;i<0x12;++i){word(s,i*4)=menu_tables_m41::cells::invalid_object_00746644;word(s,0x48+i*4)=0xffffffffu;} // 005CA925..3C
 drain(s,0x90);drain(s,0x98);drain(s,0x9c);                                 // 005CA93E..CE
 word(s,0x94)=menu_tables_m41::cells::invalid_object_00746644;              // 005CA9D0..DF
 destroy_list(s,0x9c);destroy_list(s,0x98);destroy_list(s,0x90);            // 005CA9D7..3B (fields not cleared, as original)
}
// closeC: 00563040 CSWSPlayer::~CSWSPlayer (thiscall RET 0).
void player_destroy_00563040(void*s){
 table(s,&full_table);                                                      // 00563060 [esi]=7490ACh
 if(void*l=field(s,0x10)){if(void*in=*static_cast<void**>(l)){base_ini_reconstruction::destroy_list_005e9ab0(static_cast<kotor_list::Internal*>(in),nullptr);live_free(in);}live_free(l);} // 00563066..93
 field(s,0x10)=nullptr;                                                     // 0056309B
 if(field(s,0xc)!=nullptr){                                                 // 00563096..9E
  while(**static_cast<void***>(field(s,0xc))!=nullptr){                     // 005630A0..CD
   void*o=base_ini_reconstruction::remove_head_005e9c80(*static_cast<kotor_list::Internal**>(field(s,0xc)),nullptr);
   if(o){recovered_last_update_destroy_005ca6a0(o,nullptr);live_free(o);}
  }
  if(void*l=field(s,0xc)){if(void*in=*static_cast<void**>(l)){base_ini_reconstruction::destroy_list_005e9ab0(static_cast<kotor_list::Internal*>(in),nullptr);live_free(in);}live_free(l);} // 005630CF..F2
  field(s,0xc)=nullptr;                                                     // 005630F5
 }
 if(void*u=field(s,0x50)){if(void*st=field(u,8)){kotor_fastcall_005cb040(st,nullptr);live_free(st);}live_free(u);} // 005630F8..1C
 field(s,0x50)=nullptr;                                                     // 00563124
 if(void*q=field(s,0x4c)){                                                  // 0056311F..56
  if(void*a=field(q,4)){unsigned char*block=static_cast<unsigned char*>(a)-4;
   c2da_destroy_array_006fa852(a,0xc,*reinterpret_cast<int*>(block),reinterpret_cast<void*>(&kotor_fastcall_0059ea20));live_free(block);}
  field(q,4)=nullptr;live_free(q);
 }
 field(s,0x4c)=nullptr;                                                     // 0056315E
 if(void*v=field(s,0x54)){if(void*inv=field(v,0)){luo_inventory_destroy_005ca900(inv,nullptr);live_free(inv);}live_free(v);} // 00563159..7F
 void*p58=field(s,0x58);field(s,0x54)=nullptr;live_free(p58);field(s,0x58)=nullptr;  // 00563182..96
 if(void*w=field(s,0x5c)){recovered_player_last_destroy_005cad70(w,nullptr);live_free(w);} // 0056318E..A8
 field(s,0x5c)=nullptr;                                                     // 005631AE
 frame_LogStringDtor_005e5c20(static_cast<unsigned char*>(s)+0x18,nullptr);// 005631AB..B5
 table(s,&base_table);                                                      // 005631C4 005CDA30 ~CSWSClient: [ecx]=74C324h
}
// closeC: 005631E0 CSWSPlayer scalar deleting destructor (thiscall RET 4).
void* __fastcall full_delete(void*s,void*,unsigned flags){player_destroy_00563040(s);if(flags&1)live_free(s);return s;}void* __fastcall base_delete(void*s,void*,unsigned flags){table(s,&base_table);if(flags&1)::operator delete(s);return s;}void* __fastcall short_delete(void*s,void*,unsigned flags){table(s,&short_table);table(s,&base_table);if(flags&1)::operator delete(s);return s;}
struct Lower:recovered_connection_player::LowerPorts{
void*allocate(unsigned n)override{return ::operator new(n);}void*list_internal(void*p)override{return recovered_server_create::construct_list_internal_005e9aa0(p);}void*combat_information(void*p)override{return kotor_m15_combat_information_construct_00552280(p,nullptr);}unsigned invalid_object()override{return menu_tables_m41::cells::invalid_object_00746644;}unsigned char skill_count()override{return static_cast<unsigned char*>(kotor_cell_rules_007a3a28)[0xab];}void*pointer(void*s,unsigned o)override{void*p;std::memcpy(&p,static_cast<unsigned char*>(s)+o,4);return p;}void*remove_head(void*p)override{return base_ini_reconstruction::remove_head_005e9c80(static_cast<kotor_list::Internal*>(p),nullptr);}void free(void*p)override{::operator delete(p);}};
struct Native:recovered_connection_player::Ports{Lower lower;
unsigned vtable(unsigned a)override{const void*p=nullptr;switch(a){case 0x0074c324:p=&base_table;break;case 0x00749438:p=&short_table;break;default:p=&full_table;break;}/* closeC: player_construct.cpp passes only 0074C324/00749438/007490AC (audited); 007490AC is the default arm */return reinterpret_cast<unsigned>(p);}unsigned invalid_object()override{return menu_tables_m41::cells::invalid_object_00746644;}void*allocate(unsigned n)override{return ::operator new(n);}void string_construct(void*p)override{std::memset(p,0,8);}void string_assign_empty(void*p)override{GuiAssign005e5140(p,nullptr,"");}void ref_construct(void*p)override{c2da_default_ref_00405ed0(p,nullptr);}void ref_assign_empty(void*p,void*out)override{unresolved_ref_assign_00406290(p,nullptr,out,"");}void*list_construct_005611f0(void*p)override{return recovered_connection_player::list_wrapper_005611f0(p,lower);}void*last_update_00560b30(void*p)override{return recovered_connection_player::last_wrapper_00560b30(p,lower);}void*update_wrapper_00560c20(void*p)override{return recovered_connection_player::equipment_wrapper_00560c20(p,lower);}};
}
extern "C" void* __fastcall recovered_connection_player_00562c70(void*s,void*,unsigned id){Native p;return recovered_connection_player::player_00562c70(s,id,p);}
extern "C" void* __fastcall recovered_connection_player_005802f0(void*s,void*,unsigned id){Native p;return recovered_connection_player::short_player_005802f0(s,id,p);}
