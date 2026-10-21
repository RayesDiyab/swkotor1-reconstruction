#include "666c149404-vm_commands_t4.hpp"
namespace kotor::vm_commands_t4 {
namespace {
Word party_table_004aee70(Memory& m,Word server){return m.read(server+4)+0x1b770;}           // inlined
Word base_item(Memory& m,Ports& p,Word base){return p.base_item_005b31a0(m.read(p.rules()+0x34),base);}
}
I32 apply_effect_to_object_0052e570(Memory& m,Ports& p){
 const Word array=p.object_array_004aed70(p.server_app());
 Word duration_type=0,effect=0,target=0,duration=0;
 if(!p.pop_integer(&duration_type)||!p.pop_engine_structure(0,&effect)||!p.pop_object(&target)||!p.pop_float(&duration))return kPopFailed;
 // (a failure after the effect pop leaks that copy, as in the original)
 const Word flags=m.read16(effect+0xa);
 if(duration_type==0)p.write16(effect+0xa,flags&0xfff8);              // DURATION_TYPE_INSTANT: and byte [+A],F8
 else if(duration_type==1){p.write16(effect+0xa,(flags&0xfff9)|1);m.write(effect+0xc,duration);} // TEMPORARY
 else p.write16(effect+0xa,(flags&0xfffa)|2);                          // PERMANENT
 Word object=0;
 if((p.get_game_object_004d8230(array,target,&object)&0xff)!=0||!object){p.effect_destroy_00503590(effect);p.free_006fa390(effect);return 0;}
 Word creator=0;
 if((p.get_game_object_004d8230(array,m.read(effect+0x18),&creator)&0xff)==0&&creator&&m.read8(creator+8)==5){
  if(m.read(p.object_virtual(creator,0x30)+0x964))m.write(effect+0x24,m.read(p.object_virtual(creator,0x30)+0x968));
 }
 if(!p.object_virtual(object,0x10))return 0;                          // original leaks the effect here
 if(m.read16(effect+8)==40)m.write(p.object_virtual(object,0x10)+0x148,1); // [0074671C]
 p.effect_apply_004d14f0(p.object_virtual(object,0x10),effect,0,1);
 m.write(p.object_virtual(object,0x10)+0x148,0);
 return 0;
}
I32 add_available_npc_by_template_0052dcd0(Memory& m,Ports& p,Word s){
 p.string_default_005b3190(s);
 Word npc=0;
 if(!p.pop_integer(&npc)||!p.pop_string(s)){p.string_destroy_005e5c20(s);return kPopFailed;}
 const Word result=p.party_add_npc_005645f0(party_table_004aee70(m,p.server_app()),npc,s);
 const Word ok=p.push_integer(result);
 p.string_destroy_005e5c20(s);
 return ok?0:kPushFailed;
}
I32 set_area_unescapable_005424e0(Memory& m,Ports& p){
 Word value=0;
 if(!p.pop_integer(&value))return kPopFailed;
 const Word module=p.get_module_004ae6b0(p.server_app());
 if(module){const Word area=p.module_get_area_004c30f0(module);if(area)m.write(area+0x2ac,value);}
 return 0;
}
I32 is_object_party_member_0053f160(Memory& m,Ports& p){
 Word id=0;
 if(!p.pop_object(&id))return kPopFailed;
 const Word c=p.get_creature_by_id_004ae770(p.server_app(),id);
 return p.push_integer(c?m.read(c+0xa88):0)?0:kPushFailed;
}
Word creature_acquire_item_005158e0(Memory& m,Ports& p,Word self,Word cell,Word container,Word repository_owner,Word flag){
 (void)p.object_array_004aed70(p.server_app());                        // result unused by the original
 Word possessor=m.read(m.read(cell)+0x268);
 Word repository=0,edi=0;
 if(possessor!=container){
  edi=p.get_item_004ae760(p.server_app(),possessor);
  if(!edi||m.read(edi+0x268)!=container)return 0;
  possessor=m.read(m.read(cell)+0x268);
 }
 if(repository_owner==kInvalid)repository=p.creature_item_repository_004ef770(self,1);
 else{edi=p.get_item_004ae760(p.server_app(),repository_owner);if(edi)repository=m.read(edi+0x26c);}
 const Word previous=p.get_game_object_004ae750(p.server_app(),possessor);
 if(previous){
  const Word type=m.read8(previous+8);
  if(type==5)p.creature_remove_item_00510ef0(p.object_virtual(previous,0x30),m.read(cell),1,1,0);
  else if(type==9)p.placeable_remove_item_00584ac0(p.object_virtual(previous,0x48),m.read(cell));
  else if(type==6&&m.read(p.object_virtual(previous,0x38)+0x26c))p.repository_remove_item_00555fd0(m.read(p.object_virtual(previous,0x38)+0x26c),m.read(cell));
 }else if(edi){
  if(m.read(base_item(m,p,m.read(m.read(cell)+0xc))+0x70)&&m.read(base_item(m,p,m.read(edi+0xc))+0x70))return 0;
 }
 const Word result=p.repository_add_item_0055d330(repository,cell,1,flag,1);
 if(result!=1)return result;
 if(!previous&&m.read(cell))p.item_remove_from_area_005538a0(m.read(cell));
 if(m.read(cell))p.item_set_possessor_00553210(m.read(cell),repository_owner==kInvalid?m.read(self+4):repository_owner,1,flag,kInvalid);
 return result;
}
Word placeable_acquire_item_00584b10(Memory& m,Ports& p,Word self,Word cell,Word container,Word flag){
 const Word array=p.object_array_004aed70(p.server_app());
 const Word repository=m.read(self+0x36c);
 Word object=0,previous_id=kInvalid;
 if(m.read(m.read(cell)+0x268)!=container)return 0;
 if((p.get_game_object_004d8230(array,container,&object)&0xff)==0&&object){
  const Word type=m.read8(object+8);previous_id=m.read(object+4);
  if(type==5)p.creature_remove_item_00510ef0(p.object_virtual(object,0x30),m.read(cell),1,flag,cell); // 4th arg: the item cell pointer (as pushed)
  else if(type==9)p.placeable_remove_item_00584ac0(p.object_virtual(object,0x48),m.read(cell));
  else if(type==6){
   if(m.read(base_item(m,p,m.read(p.object_virtual(object,0x38)+0xc))+0x70)==1)
    p.repository_remove_item_00555fd0(m.read(p.object_virtual(object,0x38)+0x26c),m.read(cell));
  }
 }else p.item_remove_from_area_005538a0(m.read(cell));
 if(!repository)return 0;
 (void)p.repository_add_item_0055d330(repository,cell,1,0,1);
 if(m.read(cell))p.item_set_possessor_00553210(m.read(cell),m.read(self+4),1,1,previous_id);
 return 1;
}
Word store_acquire_item_005c7610(Memory& m,Ports& p,Word self,Word cell){
 const Word item=m.read(cell);
 if(!item||m.read(item+0x108))return 0;
 p.item_set_identified_00554170(item,1);
 const Word owner=p.get_game_object_004ae750(p.server_app(),m.read(m.read(cell)+0x268));
 if(owner){
  if(const Word c=p.object_virtual(owner,0x30))p.creature_remove_item_00510ef0(c,m.read(cell),1,1,0);
  else if(p.object_virtual(owner,0x38)){
   const Word c2=p.get_creature_by_id_004ae770(p.server_app(),m.read(p.object_virtual(owner,0x38)+0x268));
   if(c2)p.creature_remove_item_00510ef0(c2,m.read(cell),1,1,0);
  }
 }
 const Word found=p.get_item_004ae760(p.server_app(),p.repository_find_item_with_tag_00555ed0(m.read(self+0x230),m.read(cell)+0x28));
 if(found&&p.item_compare_00553cf0(found,m.read(cell))){
  if(!(m.read(found+0x288)&4))m.write(found+0x28c,m.read(found+0x28c)+1);
  if(m.read(cell))p.item_delete_virtual(m.read(cell));
  return 1;
 }
 p.item_set_possessor_00553210(m.read(cell),m.read(self+4),1,1,kInvalid);
 return p.store_add_item_to_inventory_005c70c0(self,cell);
}
}
