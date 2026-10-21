#include "15afee75a2da877c_server_load_request.hpp"
#include <initializer_list>
namespace kotor::server_mainloop {
Word load_module_004b95b0(Memory& m,LoadPorts& p,Word self,Word name,Word player,Word scratch) {
 const auto s=[&](Word offset){return scratch+offset;};
 const auto app=[&](){return m.read(0x007a39fcu);};
 const auto manager=[&](){return m.read(0x007a39e8u);};
 const auto aliases=[&](){return m.read(m.read(0x007a39e0u)+0xcu);};
 const auto cleanup=[&](){for(Word off:{0x4cu,0x68u,0x34u,0x44u,0x28u,0x10u})p.destroy_005e5c20(s(off));};
 const auto append=[&](Word destination,Word right,Word temporary){
  const Word result=p.concat_005e5d10(destination,temporary,right);
  p.assign_005e5c50(destination,result);p.destroy_005e5c20(temporary);
 };
 const auto resolve=[&](Word destination,Word type,Word temporary){
  const Word receiver=aliases();
  const Word result=p.resolve_005e68a0(receiver,temporary,destination,type);
  p.assign_005e5c50(destination,result);p.destroy_005e5c20(temporary);
 };
 const Word old_module=p.module_004b14f0(self);
 for(Word off:{0x10u,0x28u,0x44u,0x34u,0x68u,0x4cu})p.empty_005b3190(s(off));
 p.timestamp_005e9410(m.read(m.read(0x007a39e0u)+8u),s(0x4c));
 (void)p.buffer_005e5670(s(0x4c));(void)p.buffer_005e5670(name);
 if(m.read(self+0x100b4u)!=1u)m.write(self+0x100b4u,1u);
 if(old_module!=0)p.unload_004b9240(self);
 p.literal_005e5a90(s(0x20),0x0073d8f8u);
 const bool missing=p.directory_exists_004068c0(manager(),s(0x20))==0;
 p.destroy_005e5c20(s(0x20));
 if(missing) {
  p.literal_005e5a90(s(0x20),0x0073d8f8u);
  p.create_directory_00409460(manager(),s(0x20),1,1);
  p.destroy_005e5c20(s(0x20));
 }
 p.set_literal_005e5140(s(0x28),0x0073d8f8u);
 p.literal_005e5a90(s(0x20),0x0073d8f4u);
 const Word slash=p.find_005e5eb0(name,s(0x20),0);
 p.destroy_005e5c20(s(0x20));
 if(slash==0xffffffffu) {
  append(s(0x28),name,s(0x18));p.assign_005e5c50(s(0x44),name);
 } else {
  const Word count=p.length_005e5790(name)-slash-1u;
  const Word result=p.right_005e6170(name,s(0x18),count);
  p.assign_005e5c50(s(0x44),result);p.destroy_005e5c20(s(0x18));
  append(s(0x28),s(0x44),s(0x18));
 }
 Word type=0xffffu;
 // Repeated branch bodies share operations only; providers and global receivers
 // are reloaded at each original call. No resource lookup result is invented.
 const auto exists=[&](Word candidate,Word ref){
  p.resref_00406d60(ref,name);
  return p.resource_exists_00408bc0(manager(),ref,candidate,0)!=0;
 };
 const auto select=[&](Word prefix,Word candidate){
  p.set_literal_005e5140(s(0x10),prefix);append(s(0x10),name,s(0x18));
  p.assign_005e5c50(s(0x34),s(0x10));
  resolve(s(0x10),candidate,s(0x18));resolve(s(0x28),candidate,s(0x18));
  type=candidate;p.estimate_004b5f90(self,s(0x34),type);
 };
 if(p.include_save_004b20e0(self,name)!=0) {
  p.add_game_005638d0(p.party_004aee70(m.read(app()+8u)));
  if(exists(0xbc1u,s(0x54)))select(0x0074514cu,0xbc1u);
  else if(exists(0x809u,s(0x54)))select(0x0074514cu,0x809u);
  p.remove_game_00563950(p.party_004aee70(m.read(app()+8u)));
 }
 if(type==0xffffu) {
  p.literal_005e5a90(s(0x20),0x007454a0u);
  p.add_directory_00408800(manager(),s(0x20));p.destroy_005e5c20(s(0x20));
  if(exists(0x80eu,s(0x54)))select(0x007454a0u,0x80eu);
  p.literal_005e5a90(s(0x20),0x007454a0u);
  p.remove_directory_004088d0(manager(),s(0x20));p.destroy_005e5c20(s(0x20));
 }
 if(type==0xffffu) {
  p.literal_005e5a90(s(0x20),0x0073d90cu);
  p.add_directory_00408800(manager(),s(0x20));p.destroy_005e5c20(s(0x20));
  if(exists(0x7dbu,s(0x54)))select(0x0073d90cu,0x7dbu);
  else if(exists(0xbbau,s(0x54)))select(0x0073d90cu,0xbbau);
  p.literal_005e5a90(s(0x20),0x0073d90cu);
  p.remove_directory_004088d0(manager(),s(0x20));p.destroy_005e5c20(s(0x20));
 }
 if(type==0xffffu) {
  p.empty_005b3190(s(0x20));
  Word index=1;
  if(m.read8(0x0073ecd2u)>1u) do {
   p.format_005e5680(s(0x20),0x007453c0u,index);
   const Word alias=p.alias_005e6890(aliases(),s(0x20),0);
   if(p.length_005e5790(alias)!=0) {
    p.literal_005e5a90(s(0x78),0x00745494u);
    append(s(0x20),s(0x78),s(0xa8));p.destroy_005e5c20(s(0x78));
    p.add_directory_00408800(manager(),s(0x20));
    Word candidate=0xffffu;
    if(exists(0x7dbu,s(0xc8)))candidate=0x7dbu;
    else if(exists(0xbbau,s(0xd8)))candidate=0xbbau;
    if(candidate!=0xffffu) {
     const Word literal=candidate==0x7dbu?s(0x70):s(0x18);
     const Word temporary=candidate==0x7dbu?s(0xb0):s(0xc0);
     p.literal_005e5a90(literal,0x0073d8f4u);
     const Word result=p.concat_005e5d10(s(0x20),temporary,literal);
     p.assign_005e5c50(s(0x10),result);p.destroy_005e5c20(temporary);p.destroy_005e5c20(literal);
     append(s(0x10),name,candidate==0x7dbu?s(0x98):s(0x90));
     p.assign_005e5c50(s(0x34),s(0x10));
     resolve(s(0x10),candidate,candidate==0x7dbu?s(0xb8):s(0xa0));
     resolve(s(0x28),candidate,candidate==0x7dbu?s(0x88):s(0x54));
     type=candidate;p.estimate_004b5f90(self,s(0x34),type);
    }
    p.remove_directory_004088d0(manager(),s(0x20));
    if(type!=0xffffu)break;
   }
   ++index;
  } while(index<m.read8(0x0073ecd2u));
  p.destroy_005e5c20(s(0x20));
 }
 if(type==0xffffu) {
  p.load_reply_0056cb80(m.read(self+0x10010u),player,1,3);cleanup();return 0;
 }
 p.literal_005e5a90(s(0x18),0x0073d8f8u);
 const Word space_status=p.free_space_004067b0(manager(),s(0x18),s(0x80));
 p.destroy_005e5c20(s(0x18));
 if(space_status==1u) {
  const Word allocation=p.allocate_006fa7e6(4);
  Word file=0;
  if(allocation!=0) {
   p.literal_005e5a90(s(0x18),0x0073d5b0u);
   file=p.file_005e68d0(allocation,s(0x34),type,s(0x18));
   p.destroy_005e5c20(s(0x18));
  }
  const Word size=p.file_size_005e6950(file); // deliberately called even with zero
  const Word high=(size&0x80000000u)!=0?0xffffffffu:0u;
  if(file!=0){p.file_destroy_005e69e0(file);p.free_006fa390(file);}
  const Word available_high=m.read(s(0x84));
  if(high>available_high || (high==available_high && size>m.read(s(0x80)))) {
   p.load_reply_0056cb80(m.read(self+0x10010u),player,1,1);cleanup();return 0;
  }
 }
 p.assign_005e5c50(m.read(app()+0x14u)+0x18u,s(0x44));
 p.assign_005e5c50(m.read(app()+0x14u)+0x20u,s(0x10));
 p.assign_005e5c50(m.read(app()+0x14u)+0x28u,s(0x28));
 m.write(m.read(app()+0x14u)+0x30u,type);
 const Word state=m.read(app()+0x14u);
 const Word kind=(type==0x809u || type==0xbc1u)?3u:1u;
 m.write(state,1);m.write(state+4u,kind);m.write(state+0x10u,1);m.write(state+0x38u,0);
 p.stall_0056c8b0(p.messages_004aedb0(m.read(app()+8u)),kind);
 m.write(m.read(app()+0x14u)+8u,0);m.write(m.read(app()+0x14u)+0xcu,0);
 p.assign_005e5c50(m.read(app()+0x14u)+0x18u,s(0x44));
 cleanup();return 1;
}
}

