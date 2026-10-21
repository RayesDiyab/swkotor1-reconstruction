#include "a73d34dc7b6860bc_module_load.hpp"
namespace kotor::server_mainloop {
Word module_load_004c9050(Memory& m,ModuleLoadPorts& p,Word self,Word name,Word use_saved,Word scratch){
 auto s=[scratch](Word x){return scratch+x;};auto res=[&](){return m.read(self+8);};auto server=[&](){return m.read(m.read(0x7a39fc)+8);};
 if(module_start_prefix_004c9050(m,p,self,name,scratch)==ModulePrefixResult::OriginalReturn1)return 1;
 module_identity_004c919b(m,p,self,name,scratch);module_time_004c9597(m,p,self,use_saved,scratch);
 auto failure=[&](bool extra){p.resource_release_00409b80(res());if(extra)p.destroy_005e5c20(s(0x18));p.destroy_005e5c20(s(0x64));p.destroy_005e5c20(s(0x40));return Word(3);};
 p.list_004118c0(res(),s(0x6c),s(0x10),0x745af8);m.write(s(0x38),p.count_00411940(res(),s(0x6c)));
 for(Word i=0;i<m.read(s(0x38));i++){
  p.element_00411990(res(),s(0x14),s(0x6c),i);
  Word item=p.allocate(12);if(item)p.loc_empty_005e9e10(item);if(!item)return failure(false);
  Word fallback=p.loc_empty_005e9e10(s(0x8c));
  Word text=p.loc_read_00411fd0(res(),s(0x94),s(0x14),0x745ae8,s(0xc),fallback);
  p.loc_assign_005e9e70(item,text);p.loc_destroy_005ea000(s(0x94));p.loc_destroy_005ea000(s(0x8c));
  Word value=p.signed_00411c90(res(),s(0x14),0x745ad8,s(0xc),0);m.write(item+8,value);
  p.append_005e9dc0(m.read(self+0x28),item);
 }
 p.list_004118c0(res(),s(0x6c),s(0x10),0x745ac4);m.write(s(0x38),p.count_00411940(res(),s(0x6c)));
 for(Word i=0;i<m.read(s(0x38));i++){
  p.element_00411990(res(),s(0x14),s(0x6c),i);
  Word item=p.allocate(20);if(item)p.ref_empty_00405ed0(item);if(!item)return failure(false);
  p.ref_literal_00406d80(s(0x28),0x73d71c);
  Word ref=p.ref_read_00411e10(res(),s(0x48),s(0x14),0x745ab4,s(0xc),s(0x28));p.ref_assign_00406120(item,s(0x9c),ref);
  Word value=p.word_00411c20(res(),s(0x14),0x745aa8,s(0xc),0);m.write(item+0x10,value);p.append_005e9dc0(m.read(self+0x2c),item);
 }
 p.empty_005b3190(s(0x18));
 const Word scripts[][2]={{0x745a98,0xb0},{0x745a84,0xb8},{0x745a70,0xd0},{0x745a5c,0xd8},{0x745a4c,0xe0},{0x745a38,0xe8},{0x745a24,0xf0},{0x745a14,0xc0},{0x745a04,0xc8},{0x7459f4,0xf8},{0x7459e4,0x100},{0x7459d0,0x108},{0x7459c0,0x110},{0x7459b0,0x118},{0x745d94,0x120}};
 for(const auto& entry:scripts){p.ref_literal_00406d80(s(0x28),0x73d71c);Word ref=p.ref_read_00411e10(res(),s(0x48),s(0x10),entry[0],s(0xc),s(0x28));p.ref_string_00405f70(ref,s(0x18));p.assign_005e5c50(self+entry[1],s(0x18));}
 p.list_004118c0(res(),s(0x6c),s(0x10),0x7459a0);m.write(s(0x38),p.count_00411940(res(),s(0x6c)));
 // Original asks for element zero even if list count was zero.
 p.element_00411990(res(),s(0x14),s(0x6c),0);p.ref_literal_00406d80(s(0x28),0x73d71c);
 Word ref=p.ref_read_00411e10(res(),s(0x48),s(0x14),0x745994,s(0xc),s(0x28));p.ref_assign_00406120(self+0x30,s(0x9c),ref);
 if(m.read(self+0x1c8)){
  m.write(self+0x40,p.word_00411c20(res(),s(0x14),0x744c24,s(0xc),0x7f000000));
  if(m.read(self+0x1c8)){
   Wide value=p.wide_00411d70(res(),s(0x10),0x745c84,s(0xc),0);m.write(0x7a1b40,static_cast<Word>(value));m.write(0x7a1b44,static_cast<Word>(value>>32));
   Word id=p.word_00411c20(res(),s(0x10),0x745c74,s(0xc),0);Word objects=m.read(s(0x5c));m.write(objects+0xc,id);
   id=p.word_00411c20(res(),s(0x10),0x745c64,s(0xc),0);m.write(objects+0x10,id);
   id=p.word_00411c20(res(),s(0x10),0x745c54,s(0xc),0);m.write(objects+4,id);
   id=p.word_00411c20(res(),s(0x10),0x745c44,s(0xc),0);m.write(objects+8,id);
   p.list_004118c0(res(),s(0x6c),s(0x10),0x745320);
   for(Word i=0;i<p.count_00411940(res(),s(0x6c));i++){
    Word item=p.allocate(0x1c);m.write(s(0x5c),item);
    if(item){p.empty_005b3190(item);p.loc_empty_005e9e10(item+8);p.loc_empty_005e9e10(item+0x10);}if(!item)return failure(true);
    p.element_00411990(res(),s(0x14),s(0x6c),i);p.literal_005e5a90(s(0x20),0x73d71c);
    Word text=p.string_00411ec0(res(),s(0x8c),s(0x14),0x745888,s(0xc),s(0x20));p.assign_005e5c50(item,text);p.destroy_005e5c20(s(0x8c));p.destroy_005e5c20(s(0x20));
    Word fallback=p.loc_empty_005e9e10(s(0x9c));text=p.loc_read_00411fd0(res(),s(0x94),s(0x14),0x745d20,s(0xc),fallback);p.loc_assign_005e9e70(item+8,text);p.loc_destroy_005ea000(s(0x94));p.loc_destroy_005ea000(s(0x9c));
    fallback=p.loc_empty_005e9e10(s(0x28));text=p.loc_read_00411fd0(res(),s(0x48),s(0x14),0x745d10,s(0xc),fallback);p.loc_assign_005e9e70(item+0x10,text);p.loc_destroy_005ea000(s(0x48));p.loc_destroy_005ea000(s(0x28));
    Word flag=p.byte_00411a60(res(),s(0x14),0x745874,s(0xc),0)&255;m.write(item+0x18,flag);p.append_005e9dc0(m.read(self+0x48),item);
   }
   p.list_004118c0(res(),s(0x6c),s(0x10),0x745988);
   for(Word i=0;i<p.count_00411940(res(),s(0x6c));i++){
    p.element_00411990(res(),s(0x14),s(0x6c),i);Word id2=p.word_00411c20(res(),s(0x14),0x745974,s(0xc),0);
    p.literal_005e5a90(s(0x5c),0x73d71c);Word text=p.string_00411ec0(res(),s(0x28),s(0x14),0x745964,s(0xc),s(0x5c));p.assign_005e5c50(s(0x64),text);p.destroy_005e5c20(s(0x28));p.destroy_005e5c20(s(0x5c));p.talk_0041db50(m.read(0x7a3a08),id2,s(0x64));
   }
   p.variables_0059aa80(self+0x8c,res(),s(0x10));p.effects_0059b0f0(self+0x9c,res(),s(0x10));
   Word app=server(),resource=res();p.messages_load_004b0a00(p.messages_004aed80(app),resource,s(0x10));
  }
 }
 Word app=server(),internal=m.read(app+4);
 if(internal){
  Word faction=m.read(internal+0x10054);m.write(s(0x5c),1);p.party_begin_005638d0(p.party_004aee70(app));
  p.ref_literal_00406d80(s(0x28),0x745d8c);
  if(p.resource_exists_00408c00(m.read(0x7a39e8),s(0x28),0x7f6,2)){
   Word allocated=p.allocate(0xa0),resource=0;
   if(allocated){p.ref_literal_00406d80(s(0x28),0x745d8c);resource=p.gff_construct_00410630(allocated,0x7f6,0x745854,s(0x28));}
   if(resource){
    if(m.read(resource+0x94)){
     p.root_00411240(resource,s(0x20));
     if(p.list_004118c0(resource,s(0x48),s(0x20),0x745848)&&p.faction_load_0052b5c0(faction,resource,s(0x48))){
      if(p.list_004118c0(resource,s(0x9c),s(0x20),0x745840)){p.reputation_load_0052bbe0(faction,resource,s(0x9c));}m.write(s(0x5c),0);
     }
    }
    p.resource_destroy(resource,1);
   }
  }
  p.party_end_00563950(p.party_004aee70(server()));
  if(m.read(s(0x5c))){if(p.faction_check_0052b490(faction,0))p.faction_a_0052b9e0(faction);else p.faction_b_0052bce0(faction);}
 }
 p.finish_004c8c70(self,res(),s(0x10),1);
 Word manager=m.read(0x7a39fc),day=m.read(s(0x38));m.write(m.read(manager+0x14)+8,0);manager=m.read(0x7a39fc);m.write(m.read(manager+0x14)+0xc,day);
 p.message_date_0056c910(p.message_service_004aedb0(server()),0,day);m.write(self+0x1d8,0);
 p.destroy_005e5c20(s(0x18));p.destroy_005e5c20(s(0x64));p.destroy_005e5c20(s(0x40));return 0;
}
}
