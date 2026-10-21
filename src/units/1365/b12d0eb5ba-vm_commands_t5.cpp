#include "6bd21b2ac4-vm_commands_t5.hpp"
namespace kotor::vm_commands_t5 {
namespace {
Word sws_message_004aedb0(Memory& m,Word server){return m.read(m.read(server+4)+0x10010);}   // inlined
I32 push_status(Word ok){return ok?0:kPushFailed;}
}
I32 get_commandable_00536e40(Memory& m,Ports& p,Word implementer,Word argc){
 Word id=m.read(implementer+8);
 if(static_cast<I32>(argc)>=1&&!p.pop_object(&id))return kPopFailed;
 Word o=0;
 const Word st=p.get_game_object_004d8230(p.object_array_004aed70(p.server_app()),id,&o)&0xff;
 return push_status(p.push_integer(st==0?m.read(o+0xe8):o));   // not found: the cleared out-cell (0) is pushed
}
I32 get_player_restrict_mode_0053cf70(Memory& m,Ports& p){
 Word id=kInvalid,value=0;
 if(!p.pop_object(&id))return kPopFailed;
 const Word c=p.get_creature_by_id_004ae770(p.server_app(),id);
 if(c&&m.read(c+0xa88)&&p.get_module_004ae6b0(p.server_app())&&p.module_get_area_004c30f0(p.get_module_004ae6b0(p.server_app())))
  value=m.read(p.module_get_area_004c30f0(p.get_module_004ae6b0(p.server_app()))+0x2b0);
 return push_status(p.push_integer(value));
}
I32 get_listen_pattern_number_0053bd20(Memory& m,Ports& p,Word implementer){
 Word o=0,value=0;
 if((p.get_game_object_004d8230(p.object_array_004aed70(p.server_app()),m.read(implementer+8),&o)&0xff)==0&&o)value=m.read(o+0x198);
 return push_status(p.push_integer(value));
}
I32 get_blocking_door_00536a80(Memory& m,Ports& p,Word implementer){
 const Word c=p.get_creature_by_id_004ae770(p.server_app(),m.read(implementer+8));
 return push_status(p.push_object(c?m.read(c+0x33c):kInvalid));
}
I32 get_spell_cast_0053d110(Memory& m,Ports& p,Word implementer,Word routine){
 Word o=0;
 const bool found=(p.get_game_object_004d8230(p.object_array_004aed70(p.server_app()),m.read(implementer+8),&o)&0xff)==0;
 auto field=[&](Word def,Word off)->Word{
  if(!found)return def;
  const Word type=m.read8(o+8);
  Word as=0;
  if(type==5)as=p.object_virtual(o,0x30);
  else if(type==9)as=p.object_virtual(o,0x48);
  else if(type==10)as=p.object_virtual(o,0x18);
  else return def;
  return m.read(as+off);
 };
 if(routine==0xf5)return push_status(p.push_object(field(kInvalid,0x1d8)));      // 245 GetLastSpellCaster
 if(routine==0xf6)return push_status(p.push_integer(field(0xffffffffu,0x1dc)));  // 246 GetLastSpell (+1DC, default -1)
 if(routine==0x1a7)return push_status(p.push_integer(field(0,0x1e0)));           // 423 GetLastSpellHarmful (+1E0, default 0)
 return 0;
}
I32 get_name_0053c080(Memory& m,Ports& p,Word s){
 p.string_default_005b3190(s);
 Word id=0;
 if(!p.pop_object(&id)){p.string_destroy_005e5c20(s);return kPopFailed;}
 Word o=0;
 if((p.get_game_object_004d8230(p.object_array_004aed70(p.server_app()),id,&o)&0xff)==0&&m.read8(o+8)>4){   // [00746633] = 4
  const Word first_obj=p.object_virtual(o,0x10),second_obj=p.object_virtual(o,0x10);
  const Word last=p.object_virtual(first_obj,0x90);       // evaluated first, as in the original
  const Word first=p.object_virtual(second_obj,0x8c);
  (void)p.get_locstring_server_00578870(0xffffffffu,first,last,s,0);
 }
 const Word ok=p.push_string(s);
 p.string_destroy_005e5c20(s);
 return push_status(ok);
}
I32 speak_string_00544090(Memory& m,Ports& p,Word implementer,Word routine,Word s,Word empty,Word copy,Word mode){
 p.string_default_005b3190(s);
 Word volume=0;
 if(!p.pop_string(s)||!p.pop_integer(&volume)){p.string_destroy_005e5c20(s);return kPopFailed;}
 if(m.read(implementer+4)==1){
  Word o=0;
  if((p.get_game_object_004d8230(p.object_array_004aed70(p.server_app()),m.read(implementer+8),&o)&0xff)==0){
   Word v=1;
   if(volume==1)v=3;else if(volume==2)v=2;else if(volume==3)v=0xd;else if(volume==4)v=0xe;
   m.write(mode,v);
   if(o){
    if(routine==0xdd){                                      // 221 SpeakString: chat message to the players
     p.string_literal_005e5a90(empty,"");                    // [0073D71C]
     p.string_copy_005e5b00(copy,s);                         // by-value argument, destroyed by the callee
     p.send_chat_00571960(sws_message_004aedb0(m,p.server_app()),m.read(mode),m.read(implementer+8),copy,0xffffffffu,empty);
     p.string_destroy_005e5c20(empty);
    }else if(m.read(o+0xe8)){                                // 39 ActionSpeakString: queue action 0x0E when commandable
     const Word params[26]={4,s,1,mode};
     (void)p.add_action_004cea20(o,0xe,0xffff,params);
    }
   }
  }
 }
 p.string_destroy_005e5c20(s);
 return 0;
}
I32 get_attempted_attack_target_00536860(Memory& m,Ports& p,Word implementer){
 const Word c=p.get_creature_by_id_004ae770(p.server_app(),m.read(implementer+8));
 return push_status(p.push_object(c?m.read(c+0x50c):kInvalid));
}
I32 get_last_perception_0053b2a0(Memory& m,Ports& p,Word implementer,Word routine){
 Word off=0;
 if(routine==0x101)off=0x904;else if(routine==0x102)off=0x90c;else if(routine==0x103)off=0x908;else if(routine==0x105)off=0x910;
 Word value=0,o=0;
 if(off&&(p.get_game_object_004d8230(p.object_array_004aed70(p.server_app()),m.read(implementer+8),&o)&0xff)==0&&p.object_virtual(o,0x30))
  value=m.read(p.object_virtual(o,0x30)+off);
 return push_status(p.push_integer(value));
}
I32 get_last_speaker_0053b410(Memory& m,Ports& p,Word implementer){
 Word o=0,value=kInvalid;
 if((p.get_game_object_004d8230(p.object_array_004aed70(p.server_app()),m.read(implementer+8),&o)&0xff)==0&&p.object_virtual(o,0x30))
  value=m.read(p.object_virtual(o,0x30)+0x17c);
 return push_status(p.push_object(value));
}
Word aq_action_id_by_id_0057a2b0(Word type,Word flag){
 static const unsigned short table[63]={0x0,0xffff,0xffff,0xffff,0xffff,0xffff,0x1,0xffff,0x2,0xffff,0xffff,0x3,0xffff,0xffff,0x4,0xffff,0xffff,0xffff,0xffff,0x5,0x6,0xffff,0xffff,0x7,0x8,0x9,0xa,0xb,0xc,0x24,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xd,0xe,0xf,0x10,0x11,0x12,0xffff,0xffff,0x13,0xffff,0xffff,0xffff,0x1f,0xffff,0xffff,0xffff,0x22,0x23,0x21,0xffff,0xffff,0xffff,0xffff,0x26,0xffff,0x27};
 const Word t=type&0xffff;
 if(flag)return t==0xb?0x1b:t==0x1e?0x18:0xffff;
 return (t>=1&&t<=63)?table[t-1]:0xffff;
}
I32 get_current_action_00549040(Memory& m,Ports& p,Word pos){
 Word result=0xffff,id=0;
 if(!p.pop_object(&id))return kPopFailed;
 const Word o=p.get_game_object_004ae750(p.server_app(),id);
 if(o&&p.object_virtual(o,0x10)){
  const Word obj=p.object_virtual(o,0x10);
  const Word list=obj+0xfc;
  const Word internal=m.read(list);
  const Word head=m.read(internal);
  Word action=head?m.read(head+8):0;
  m.write(pos,head);
  if(!action)result=0xfffe;
  else{
   const Word group=m.read16(action+0x6c);
   while(action&&m.read16(action+0x6c)==group){
    if(p.acceptable_action_0057a240(obj,m.read(action)))result=aq_action_id_by_id_0057a2b0(m.read16(action),0)&0xffff;
    action=p.list_next_005e9b90(m.read(list),pos);
   }
  }
 }
 // [0083281C] debug branch (GetCreatureByGameObjectID + CExoString::CStr of the creature tag, result unused): no effect.
 return push_status(p.push_integer(result));
}
I32 get_ability_score_005361b0(Memory& m,Ports& p){
 Word id=0,ability=0,value=0;
 if(!p.pop_object(&id)||!p.pop_integer(&ability))return kPopFailed;
 Word o=0;
 if((p.get_game_object_004d8230(p.object_array_004aed70(p.server_app()),id,&o)&0xff)==0&&p.object_virtual(o,0x30)&&ability<=5)   // [00746838..0074684C] = 0..5
  value=p.creature_stat(m.read(p.object_virtual(o,0x30)+0xa74),ability)&0xff;
 return push_status(p.push_integer(value));
}
I32 get_attempted_spell_target_005368b0(Memory& m,Ports& p,Word implementer){
 const Word c=p.get_creature_by_id_004ae770(p.server_app(),m.read(implementer+8));
 return push_status(p.push_object(c?m.read(c+0x524):kInvalid));
}
namespace {
Word object_get_area_004cb120(Memory& m,Ports& p,Word self){   // CSWSObject::GetArea: area id +8C -> AsArea (vtbl+28)
 Word o=0;
 (void)p.get_game_object_004d8230(p.object_array_004aed70(p.server_app()),m.read(self+0x8c),&o);
 return o?p.object_virtual(o,0x28):0;
}
}
void set_last_hostile_actor_004cc0c0(Memory& m,Ports& p,Word self,Word actor,Word force){
 if(force==1){m.write(self+0x158,actor);return;}
 if(m.read(self+0xf8)==1){m.write(self+0x158,kInvalid);return;}
 const Word o=p.get_game_object_004ae750(p.server_app(),actor);
 if(!o)return;                                                    // (original: unchanged)
 Word c=0;
 const Word type=m.read8(o+8);
 if(type==11)c=p.get_creature_by_id_004ae770(p.server_app(),m.read(p.object_virtual(o,0x50)+0x248));   // [0074663A] trap: its creator
 else if(type==5&&m.read(o+4)!=m.read(self+4))c=p.object_virtual(o,0x30);                              // [00746634] creature, not self
 if(!c||p.object_virtual(c,0x94)==1||p.dead_temp_004ef890(c)==1){m.write(self+0x158,kInvalid);return;}
 if(p.object_virtual(self,0x30)&&object_get_area_004cb120(m,p,self)&&m.read(m.read(c+0xa74)+0x78)==m.read(m.read(p.object_virtual(self,0x30)+0xa74)+0x78))
  return;                                                         // same faction: unchanged
 m.write(self+0x158,m.read(c+4));
}
I32 get_last_hostile_actor_0053af40(Memory& m,Ports& p){
 Word id=kInvalid,result=kInvalid;
 if(!p.pop_object(&id))return kPopFailed;
 Word o=p.get_game_object_004ae750(p.server_app(),id);
 if(o&&p.object_virtual(o,0x10)&&(o=p.object_virtual(o,0x10))!=0){
  bool clear=false;
  if(m.read(o+0xf8)==1)clear=m.read(o+0x158)!=kInvalid;
  else{
   result=m.read(o+0x158);
   const Word a=p.get_game_object_004ae750(p.server_app(),result);
   Word c=0;bool look=false;
   if(a){
    const Word type=m.read8(a+8);
    if(type==11){result=m.read(p.object_virtual(a,0x50)+0x248);look=true;}   // trap: its creator
    else if(type==5)look=true;
   }
   if(look)c=p.get_creature_by_id_004ae770(p.server_app(),result);
   if(!c||p.object_virtual(c,0x94)==1||p.return_zero_0063e7f0(c)==1)clear=true;
   else if(p.dead_temp_004ef890(c))clear=true;
   else return push_status(p.push_object(result));                  // living hostile actor
  }
  if(clear)set_last_hostile_actor_004cc0c0(m,p,o,kInvalid,1);
  result=kInvalid;
 }
 return push_status(p.push_object(result));
}
I32 get_attack_target_00536800(Memory& m,Ports& p){
 Word id=0;
 if(!p.pop_object(&id))return kPopFailed;
 const Word c=p.get_creature_by_id_004ae770(p.server_app(),id);
 return push_status(p.push_object(c?m.read(c+0x504):kInvalid));
}
I32 get_is_dead_0053ecb0(Memory&,Ports& p){
 Word id=0,dead=1;
 if(!p.pop_object(&id))return kPopFailed;
 const Word o=p.get_game_object_004ae750(p.server_app(),id);
 if(o){
  if(p.object_virtual(o,0x30))
   dead=(p.object_virtual(p.object_virtual(o,0x30),0x94)||p.return_zero_0063e7f0(p.object_virtual(o,0x30))||p.dead_temp_004ef890(p.object_virtual(o,0x30)))?1:0;
  else dead=p.object_virtual(o,0x94);
 }
 return push_status(p.push_integer(dead));
}
I32 get_last_damager_0053ad40(Memory& m,Ports& p,Word implementer){
 const Word o=p.get_game_object_004ae750(p.server_app(),m.read(implementer+8));
 // Original quirk: when the object or its AsSWSObject is missing, the register holds the stack slot of the
 // initial `push ecx`, i.e. the implementer pointer itself, and +160 is read from the implementer.
 const Word base=(o&&p.object_virtual(o,0x10))?p.object_virtual(o,0x10):implementer;
 return push_status(p.push_object(base?m.read(base+0x160):kInvalid));
}
I32 object_to_string_00540170(Memory&,Ports& p,Word tmp,Word s){
 Word id=0;
 if(!p.pop_object(&id))return kPopFailed;
 p.string_default_005b3190(tmp);
 p.string_format_hex_005e5680(tmp,id);
 p.string_literal_005e5a90(s,reinterpret_cast<const char*>(static_cast<std::uintptr_t>(p.string_cstr_005e5670(tmp))));
 const Word ok=p.push_string(s);
 p.string_destroy_005e5c20(s);
 p.string_destroy_005e5c20(tmp);
 return push_status(ok);
}
I32 ship_build_00543840(Memory&,Ports& p){
 p.ship_build_flag_00832834(1);
 return push_status(p.push_integer(1));
}
I32 get_object_visibility_0053c780(Memory& m,Ports& p,Word routine){
 Word target=0,source=0,value=0;
 if(!p.pop_object(&target)||!p.pop_object(&source))return kPopFailed;
 Word o=0;
 if((p.get_game_object_004d8230(p.object_array_004aed70(p.server_app()),source,&o)&0xff)==0&&p.object_virtual(o,0x30)){
  if(routine==0x121||routine==0x122){
   const Word e=p.visible_element_00517a20(p.object_virtual(o,0x30),target);
   if(e)value=(routine==0x121?m.read8(e+4):m.read8(e+4)>>1)&1;   // 289 seen (bit 0), 290 heard (bit 1)
  }
 }
 return push_status(p.push_integer(value));
}
I32 get_emotions_00537ce0(Memory& m,Ports& p,Word implementer,Word routine,Word argc){
 Word source=kInvalid,target=kInvalid,value=0;
 if(!p.pop_object(&target))return kPopFailed;
 if(m.read(implementer+4)==1)source=m.read(implementer+8);
 if(static_cast<I32>(argc)>1&&!p.pop_object(&source))return kPopFailed;
 Word o=0;
 if((p.get_game_object_004d8230(p.object_array_004aed70(p.server_app()),target,&o)&0xff)==0){
  const Word self=p.object_virtual(o,0x10);
  if(self){
   if(routine==0xeb){                                                  // 235 GetIsEnemy
    if(p.object_virtual(o,0x48)&&!m.read(p.object_virtual(o,0x48)+0x340))value=0;   // placeable without +340: never an enemy
    else value=static_cast<I32>(p.get_reputation_0057cb80(self,source,0))<=10?1:0;
   }else if(routine==0xec)value=static_cast<I32>(p.get_reputation_0057cb80(self,source,0))>=90?1:0;   // 236 GetIsFriend
   else if(routine==0xed){const I32 r=static_cast<I32>(p.get_reputation_0057cb80(self,source,0));value=(r>10&&r<90)?1:0;}   // 237
  }
 }
 return push_status(p.push_integer(value));
}
I32 get_party_ai_style_005459a0(Memory& m,Ports& p){
 const Word party=m.read(p.server_app()+4)+0x1b770;   // CServerExoApp::GetPartyTable 004AEE70 (inlined)
 return push_status(p.push_integer(m.read(party+0xe4)));
}
I32 get_npc_ai_style_005459d0(Memory& m,Ports& p){
 Word id=0,value=0,o=0;
 if(!p.pop_object(&id))return kPopFailed;
 if((p.get_game_object_004d8230(p.object_array_004aed70(p.server_app()),id,&o)&0xff)==0&&p.object_virtual(o,0x30))
  value=m.read16(m.read(p.object_virtual(o,0x30)+0xa74)+0x13a);   // CSWSCreatureStats +13A (word)
 return push_status(p.push_integer(value));
}
Word creature_user_actions_pending_00518b30(Memory& m,Ports& p,Word creature,Word pos){
 auto internal=[&]{return m.read(m.read(m.read(creature+0x9c8)+0x9b0));};
 const Word head=m.read(internal());
 m.write(pos,head);
 if(!head)return 0;
 Word e=p.list_at_pos_005e9b60(internal(),head);
 auto living_creature=[&](Word id)->Word{                       // the object, or 0 when it is a dead creature
  if(id==kInvalid)return 0;
  const Word o=p.get_game_object_004ae750(p.server_app(),id);
  return o;
 };
 while(e){
  Word first=living_creature(m.read(e+0x14));
  if(first&&p.object_virtual(first,0x30)&&p.object_virtual(p.object_virtual(first,0x30),0x94))first=0;
  Word second=living_creature(m.read(e+0x44));
  if(second&&p.object_virtual(second,0x30)&&p.object_virtual(p.object_virtual(second,0x30),0x94))first=0;   // original clears ESI (the FIRST), not EDI
  if(m.read(e+0x84)&&first&&second)return 1;
  e=p.list_next_005e9b90(internal(),pos);
 }
 return 0;
}
I32 get_user_actions_pending_0053e1b0(Memory& m,Ports& p,Word implementer,Word pos){
 Word o=0,value=0;
 if((p.get_game_object_004d8230(p.object_array_004aed70(p.server_app()),m.read(implementer+8),&o)&0xff)==0&&p.object_virtual(o,0x30))
  value=creature_user_actions_pending_00518b30(m,p,p.object_virtual(o,0x30),pos);
 return push_status(p.push_integer(value));
}
}
