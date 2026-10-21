#include "be45c700cb-vm_commands_t3.hpp"
#include "91826d361f-vm_execute_x87.inc"
#include <cstring>
namespace kotor::vm_commands_t3 {
namespace {
I32 si(Word x){I32 y;std::memcpy(&y,&x,4);return y;}
}
void effect_set_object_id_005036f0(Memory& m,Word effect,Word index,Word id){m.write(effect+0x78+4*index,id);}
I32 effect_complex_005313f0(Memory& m,Ports& p,Word implementer,I32 routine,Word argc){
 const Word allocation=p.allocate_006fa7e6(0x8c);
 const Word e=allocation?p.effect_construct_00503e40(allocation,1):0;
 p.write16(e+8,0);                                         // [0074667C]
 Word a=0,b=0;
 auto type=[&](Word t){p.write16(e+8,t);};
 auto seti=[&](Word i,Word v){p.effect_set_integer_005036a0(e,i,v);};
 switch(routine){
 case 51:type(101);break;                                  // EffectAssuredHit
 case 130:type(18);break;                                  // EffectEntangle
 case 133:                                                 // EffectDeath(int nSpectacularDeath=FALSE,int nDisplayFeedback=TRUE)
  a=0;b=1;
  if(si(argc)>=1&&!p.pop_integer(&a))return kPopFailed;
  if(si(argc)>=2&&!p.pop_integer(&b))return kPopFailed;
  type(19);seti(0,a);seti(1,b);break;
 case 134:type(20);break;                                  // EffectKnockdown
 case 148:type(8);seti(0,5);break;                         // EffectParalyze
 case 154:type(8);seti(0,6);break;                         // EffectSleep
 case 157:type(8);seti(0,1);break;                         // EffectConfused
 case 158:type(8);seti(0,2);break;                         // EffectFrightened
 case 161:type(8);seti(0,4);break;                         // EffectStunned
 case 164:{                                                // EffectRegenerate(int,float)
  if(!p.pop_integer(&b))return kPopFailed;
  if(!p.pop_float(&a))return kPopFailed;
  const Word ms=p.ftol_mul_1000_x87(a);
  type(7);seti(0,b);seti(1,ms);break;
 }
 case 252:a=0;if(!p.pop_integer(&a))return kPopFailed;type(104);seti(0,a);break; // EffectAssuredDeflection
 case 270:type(1);break;                                   // EffectHaste
 case 402:type(105);a=kInvalid;if(!p.pop_object(&a))return kPopFailed;effect_set_object_id_005036f0(m,e,0,a);break; // EffectForceResisted
 case 420:type(106);break;                                 // EffectForceFizzle
 case 460:type(51);if(!p.pop_integer(&a))return kPopFailed;break; // EffectDispelMagicAll (value unused)
 case 463:if(!p.pop_integer(&a))return kPopFailed;type(62);seti(0,a);seti(1,0);break; // EffectDisguise
 case 467:type(64);break;                                  // EffectTimeStop
 case 473:type(52);if(!p.pop_integer(&a))return kPopFailed;break; // EffectDispelMagicBest
 case 754:type(8);seti(0,8);seti(1,1);break;               // EffectCutSceneHorrified
 case 755:type(8);seti(0,5);seti(1,1);break;               // EffectCutSceneParalyze
 case 756:type(8);seti(0,4);seti(1,1);break;               // EffectCutSceneStunned
 default:break;
 }
 p.write16(e+0xa,(m.read16(e+0xa)&0xffef)|8);
 const Word self=m.read(implementer+8);
 Word object=0;
 const Word status=p.get_game_object_004d8230(p.object_array_004aed70(p.server_app()),self,&object);
 if((status&0xff)==0&&object)p.effect_set_creator_00503a00(e,m.read(implementer+8));
 if(!p.push_engine_structure(0,e))return kPushFailed;     // original leaks e on this path
 p.effect_destroy_00503590(e);p.free_006fa390(e);
 return 0;
}
I32 create_item_on_object_0052f570(Memory& m,Ports& p,Word argc,Frame f){
 const Word name=f.base,ref=f.base+8,object_cell=f.base+0x18,item_cell=f.base+0x1c;
 p.string_default_005b3190(name);
 Word target=0,count=1;
 if(!p.pop_string(name)||!p.pop_object(&target)||(si(argc)>=3&&!p.pop_integer(&count))){p.string_destroy_005e5c20(name);return kPopFailed;}
 Word result=kInvalid;
 Word object=0;
 const Word status=p.get_game_object_004d8230(p.object_array_004aed70(p.server_app()),target,&object);
 m.write(object_cell,object);
 if((status&0xff)==0){
  p.resref_from_chars_00406d80(ref,p.string_cstr_005e5670(name));
  const Word allocation=p.allocate_006fa7e6(0x29c);
  const Word item=allocation?p.item_construct_005530a0(allocation,kInvalid):0;
  m.write(item_cell,item);
  if(!p.item_load_template_005608b0(item,ref)){if(item)p.item_delete_virtual(item);}
  else{
   m.write(item+0x288,m.read(item+0x288)|8);
   const Word base=p.base_item_005b31a0(m.read(p.rules()+0x34),m.read(item+0xc));
   if(base&&si(count)>si(m.read16(base+0x24)))count=m.read16(base+0x24);
   if(si(count)<1){if(item)p.item_delete_virtual(item);}
   else{
    m.write(item+0x28c,count&0xffff);
    bool acquired=true;
    if(const Word c=p.object_virtual(object,0x30)){
     p.creature_acquire_item_005158e0(c,item_cell,kInvalid,kInvalid,1);
     if(m.read(p.object_virtual(object,0x30)+0xa88))p.gui_update_status_0062eeb0(p.in_game_gui_005ed690(p.client_app()),7,0);
    }else if(const Word pl=p.object_virtual(object,0x48))p.placeable_acquire_item_00584b10(pl,item_cell,kInvalid,1);
    else if(const Word st=p.object_virtual(object,0x64))p.store_acquire_item_005c7610(st,m.read(item_cell));
    else{acquired=false;if(m.read(item_cell))p.item_delete_virtual(m.read(item_cell));m.write(item_cell,0);}
    if(acquired&&m.read(item_cell))result=m.read(m.read(item_cell)+0x14);
   }
  }
 }
 const Word ok=p.push_object(result);
 p.string_destroy_005e5c20(name);
 return ok?0:kPushFailed;
}
I32 reveal_map_00547240(Memory& m,Ports& p,Frame f){
 const Word v=f.base;Word n=0;
 m.write(v,0);m.write(v+4,0);m.write(v+8,0);
 if(!p.pop_vector(v)||!p.pop_integer(&n))return kPopFailed;
 const Word module=p.get_module_004ae6b0(p.server_app());
 if(module){
  const Word map=m.read(module+0x218);
  if(map){
   if(n==0xffffffff)p.map_entire_explored_00578fa0(map);
   else p.map_world_point_explored_005792d0(map,m.read(v),m.read(v+4),m.read(v+8),n);
  }
 }
 return 0;
}
I32 is_available_npc_0053ed70(Memory&,Ports& p,I32 routine){
 Word npc=0;
 if(!p.pop_integer(&npc))return kPopFailed;
 Word result=0;
 if(routine==0x2b8)result=p.npc_available_005636b0(p.party_table_004aee70(p.server_app()),npc);
 else if(routine==0x2c5)result=p.npc_selectable_005637c0(p.party_table_004aee70(p.server_app()),npc)&0xff;
 return p.push_integer(result)?0:kPushFailed;
}
I32 item_activated_0053f220(Memory& m,Ports& p,I32 routine,Frame f){
 const Word module=p.get_module_004ae6b0(p.server_app());
 if(routine==0x1b9){                                       // GetItemActivatedTargetLocation
  const Word loc=f.base,d=f.base+0x18,n=f.base+0x24;
  for(Word o=0;o<0x18;o+=4)m.write(loc+o,0);               // CScriptLocation 004CA7A0
  if(module){
   const Word c=p.get_creature_by_id_004ae770(p.server_app(),m.read(module+0x17c));
   m.write(loc,m.read(module+0x188));m.write(loc+4,m.read(module+0x18c));m.write(loc+8,m.read(module+0x190));
   if(c){
    const Word pos=c+0x90;
    if(!p.vector_equal_004aa980(pos,loc)){
     m.write(d+8,w6_x87::binary(0x15,m.read(loc+8),m.read(pos+8)));
     m.write(d+4,w6_x87::binary(0x15,m.read(loc+4),m.read(pos+4)));
     m.write(d,w6_x87::binary(0x15,m.read(loc),m.read(pos)));
     p.vector_normalize_004ab180(n,d);
     m.write(loc+0xc,m.read(n));m.write(loc+0x10,m.read(n+4));m.write(loc+0x14,m.read(n+8));
    }
   }
  }
  return p.push_engine_structure(2,loc)?0:kPushFailed;
 }
 Word value=kInvalid;
 if(module){
  if(routine==0x1b7)value=m.read(module+0x178);
  else if(routine==0x1b8)value=m.read(module+0x17c);
  else if(routine==0x1ba)value=m.read(module+0x184);
 }
 return p.push_object(value)?0:kPushFailed;
}
}
