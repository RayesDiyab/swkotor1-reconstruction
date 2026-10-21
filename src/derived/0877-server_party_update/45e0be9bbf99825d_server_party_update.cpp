#include "27c1ace21ba1e40f_server_party_update.hpp"
namespace kotor::server_mainloop {
static VectorWords vector_at(Memory& m,Word a){return {m.read(a),m.read(a+4),m.read(a+8)};}
}
namespace kotor::server_mainloop {
void party_update_004b6da0(Memory& m,PartyUpdatePorts& p,Word self,Word scratch){
 const auto client=[&](){return m.read(m.read(0x007a39fc)+4);};
 const auto server=[&](){return m.read(m.read(0x007a39fc)+8);};
 const auto s=[&](Word off){return scratch+off;};
 const auto copy=[&](Word dst,Word src){const auto v=vector_at(m,src);m.write(dst,v.x);m.write(dst+4,v.y);m.write(dst+8,v.z);};
 if(p.cinematic_005edc60(client()))return;
 if(p.busy_005edad0(client()))return;
 const Word now=Word(p.divide_006fb030(p.high_time_005e9260(m.read(m.read(0x007a39e0)+4)),1000));
 const Word elapsed=now-m.read(self+0x100cc);m.write(self+0x100cc,now);
 const Word count=m.read(p.party_005ed8b0(client()));
 bool any=false,all=true;
 for(Word i=0;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(count);++i){
  const Word entry=p.character_006346c0(p.party_005ed8b0(client()),i);
  if(!entry)continue;
  const Word c=p.creature_0060fb20(entry);m.write(s(0x1c),c);
  if(c&&p.incapacitated_004ef890(c))any=true;else all=false;
 }
 if(all&&count)p.all_down_005edc40(client());
 bool safe=any;
 if(any){
  const Word scan=m.read(self+0x100d0)+elapsed;m.write(self+0x100d0,scan);
  if(scan>1000){
   const Word last=m.read(s(0x1c));m.write(self+0x100d0,0);
   const Word area=p.area_004cb120(last),array=p.object_array_004aed70(server());
   if(area&&array){
    p.first_005071b0(area,s(0x1c));
    do {
     p.lookup_004d8230(array,m.read(s(0x1c)),s(0x14));
     const Word object=m.read(s(0x14));
     if(object&&p.virtual_creature_30(object)&&m.read(p.virtual_creature_30(object)+0xa88)==0){
      const Word c=p.virtual_creature_30(object);
      if(static_cast<std::int32_t>(p.relationship_0057cb80(c,m.read(last+4),0))<=10&&p.hostile_004f7650(c))safe=false;
     }
     if(!p.next_005071e0(area,s(0x1c)))break;
    }while(safe);
   }
  }
 }
 m.write(self+0x100d4,safe?m.read(self+0x100d4)+elapsed:0);
 if(m.read(self+0x100d4)<=5000)return;
 const Word first=p.creature_0060fb20(p.character_006346c0(p.party_005ed8b0(client()),0));
 copy(s(0x34),first+0x90);
 for(Word i=0;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(count);++i){
  const Word c=p.creature_0060fb20(p.character_006346c0(p.party_005ed8b0(client()),i));
  if(!c)continue;
  if(p.incapacitated_004ef890(c)){
   copy(s(0x28),c+0x90);
   p.safe_position_004aeb60(server(),s(0x28),0x40a00000,m.read(c+4),0,s(0x28));
   p.set_position_004cd1a0(c,s(0x28),1,1,0);
   const Word raw=p.allocate_006fa7e6(0x8c);
   const Word effect=raw?p.effect_ctor_00503e40(raw,1):0;
   const Word flags=m.read16(effect+0xa);
   m.write16(effect+8,m.read16(0x74668c));
   m.write16(effect+0xa,(flags&0xfff8)|m.read16(0x7473b8));
   p.apply_effect_004d14f0(c,effect,0,0);m.write(c+0xf0,1);
  }
  if(m.read(p.party_table_004aee70(server())+0x190))continue;
  if(!p.distance_exceeds_004b7120(c+0x90,s(0x34)))continue;
  copy(s(0x40),p.party_005ed8b0(client())+i*0x88+0x4c);
  p.safe_position_004aeb60(server(),s(0x40),0x40a00000,m.read(c+4),0,s(0x40));
  p.set_position_004cd1a0(c,s(0x40),1,1,0);
 }
}
}
