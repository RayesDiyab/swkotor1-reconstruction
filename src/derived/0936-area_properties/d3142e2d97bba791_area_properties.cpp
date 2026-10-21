#include "4b76230000801caf_area_properties.hpp"
namespace kotor::server_mainloop {
//00508C74 through00508E92; zero return is the original localized-allocation failure exit.
Word area_properties_header(Memory&m,AreaPropertyPorts&p,Word self,Word root,Word scratch){
 auto s=[scratch](Word o){return scratch+o;};auto resource=[&](){return m.read(self+0x108);};
 Word value=p.scalar(0x411c90,resource(),root,0x73e9c0,s(0x10),0);m.write(m.read(self+0x160),value);
 value=p.scalar(0x411c90,resource(),root,0x747a7c,s(0x10),0);m.write(m.read(self+0x160)+4,value);
 value=p.scalar(0x411c20,resource(),root,0x747a74,s(0x10),0);m.write(m.read(self+0x160)+8,value);
 p.literal(s(0x24),0x73d71c);Word text=p.string(resource(),s(0x30),root,0x747a68,s(0x10),s(0x24));p.assign(self+0x148,text);p.destroy(s(0x30));p.destroy(s(0x24));
 if(p.list(resource(),s(0x98),root,0x747a58)==1){
  Word count=p.count(resource(),s(0x98));m.write(s(0x1c),count);m.write(s(0x18),0);
  for(Word index=0;static_cast<std::int32_t>(index)<static_cast<std::int32_t>(m.read(s(0x1c)));){
   p.element(resource(),s(0x14),s(0x98),index);Word localized=p.allocate(12);if(localized)p.localized_construct(localized);else{p.release(resource());return 0;}
   Word fallback=p.localized_construct(s(0x40));Word read=p.localized(resource(),s(0x30),s(0x14),0x745ae8,s(0x10),fallback);p.localized_assign(localized,read);p.localized_destroy(s(0x30));p.localized_destroy(s(0x40));
   m.write(localized+8,p.scalar(0x411c90,resource(),s(0x14),0x745ad8,s(0x10),0));p.append(m.read(self+0x144),localized);
   index=m.read(s(0x18))+1;m.write(s(0x18),index);
  }
 }
 return 1;
}
//00508E93 through005092E6. Leaves the original long-lived string scratch38 alive for later segments.
void area_properties_scripts(Memory&m,AreaPropertyPorts&p,Word self,Word root,Word scratch){
 auto s=[scratch](Word o){return scratch+o;};auto resource=[&](){return m.read(self+0x108);};auto server=[&](){return m.read(p.global(0x7a39fc)+8);};
 p.empty(s(0x38));const Word scripts[][2]={{0x747a4c,0x164},{0x747a3c,0x16c},{0x747a34,0x174},{0x747a2c,0x17c}};
 for(auto&row:scripts){p.ref_literal(s(0x48),0x73d71c);Word ref=p.ref(resource(),s(0x60),root,row[0],s(0x10),s(0x48));p.ref_string(ref,s(0x38));p.assign(self+row[1],s(0x38));}
 Word fallback=p.localized_construct(s(0x30));Word text=p.localized(resource(),s(0x24),root,0x745eec,s(0x10),fallback);p.localized_assign(self+0x150,text);p.localized_destroy(s(0x24));p.localized_destroy(s(0x30));
 p.literal(s(0x24),0x73d71c);text=p.string(resource(),s(0x40),root,0x747a28,s(0x10),s(0x24));Word lower=p.lower(text,s(0x30));p.assign(self+0x158,lower);p.destroy(s(0x30));p.destroy(s(0x40));p.destroy(s(0x24));
 m.write(self+4,p.scalar(0x411c20,resource(),root,0x747a20,s(0x10),0));m.write(self+0xb8,p.scalar(0x411c90,resource(),root,0x747a14,s(0x10),0));
 p.ref_literal(s(0x48),0x73d71c);Word ref=p.ref(resource(),s(0x60),root,0x747a04,s(0x10),s(0x48));p.ref_assign(self+0xbc,s(0xac),ref);m.write(self+0x224,1);
 if(m.read(p.party(server())+0xe8)==1)p.party_clear(p.party(server()),0);
 m.write(self+0x2ac,p.scalar(0x411a60,resource(),root,0x747654,s(0x10),m.read(self+0x2ac))&255);
 Word restriction=p.scalar(0x411a60,resource(),root,0x747644,s(0x10),m.read(self+0x2b0))&255;if(restriction&&restriction!=m.read(self+0x2b0))p.party_restrict(p.party(server()),1);m.write(self+0x2b0,restriction);
 const Word flags[][3]={{0x7479f8,0xa8,1},{0x7479ec,0xa9,1},{0x7479e0,0x184,4},{0x7479d0,0xaa,1},{0x7479c4,0xab,1}};
 for(auto&row:flags){Word value=p.scalar(0x411c90,resource(),root,row[0],s(0x10),0);if(row[2]==1)p.byte(self+row[1],value);else m.write(self+row[1],value);}
 if(m.read8(self+4)&1){for(Word off=0xa8;off<=0xab;++off)p.byte(self+off,0);m.write(self+0x184,0);}
 const Word colors[][2]={{0x7479b0,0x68},{0x74799c,0x6c},{0x74798c,0x70}};for(auto&row:colors)m.write(self+row[1],p.scalar(0x411c20,resource(),root,row[0],s(0x10),0));
}
//005092E7 through00509727. float_store must preserve the original x87 store/comparison behavior.
void area_properties_environment(Memory&m,AreaPropertyPorts&p,Word self,Word root,Word scratch){
 auto status=scratch+0x10;auto resource=[&](){return m.read(self+0x108);};
 auto scalar=[&](Word fn,Word label,Word fallback){return p.scalar(fn,resource(),root,label,status,fallback);};
 auto color=[&](Word label,Word off){m.write(self+off,scalar(0x411c20,label,0));};
 auto byte=[&](Word label,Word off){p.byte(self+off,scalar(0x411a60,label,0));};
 auto promoted=[&](Word label,Word off,Word fallback){m.write(self+off,scalar(0x411a60,label,fallback)&255);};
 auto floating=[&](Word label,Word off,Word fallback,bool clamp){p.float_store(resource(),root,label,status,fallback,self+off,clamp);};
 floating(0x747980,0x74,0x461c4000,true);floating(0x747974,0x78,0x461c4000,true);
 byte(0x747968,0x7c);promoted(0x74795c,0x80,0);color(0x74794c,0x84);color(0x74793c,0x88);color(0x7475c0,0x8c);
 floating(0x747930,0x90,0x461c4000,true);floating(0x747924,0x94,0x461c4000,true);
 byte(0x747918,0x98);promoted(0x74790c,0x9c,0);promoted(0x7478fc,0xa0,0);promoted(0x7478f4,0xa4,0);color(0x7478e4,0xac);
 promoted(0x7478dc,0xb0,m.read(self+0xb0));p.byte(scratch+0x1c,m.read8(self+0xb4));p.byte(self+0xb4,scalar(0x411a60,0x7478cc,m.read(scratch+0x1c)));
 byte(0x7478bc,0x222);m.write(self+0x188,scalar(0x411c90,0x7478ac,0));m.write(self+0x18c,scalar(0x411c90,0x74789c,0));
 //00411A10 has no fallback argument and writes its output to scratch14.
 m.write(self+0x234,p.structure(resource(),scratch+0x14,root,0x747890));m.write16(self+0x228,scalar(0x411b40,0x747880,0));color(0x747870,0xd0);color(0x747860,0xcc);
 floating(0x747850,0xd4,0,false);floating(0x747840,0xd8,0,false);
 p.ref_literal(scratch+0x48,0x73d71c);Word ref=p.ref(resource(),scratch+0x60,root,0x747830,status,scratch+0x48);p.ref_assign(self+0xec,scratch+0xac,ref);
 if(!p.ref_nonempty(self+0xec))p.ref_assign_literal(self+0xec,scratch+0x60,0x747828);
 floating(0x747818,0xdc,0,false);floating(0x747808,0xe0,0,false);floating(0x7477f8,0xe4,0,false);floating(0x7477e8,0xe8,0,false);floating(0x7477dc,0xfc,0x3e4ccccd,false);
}
//00509728 through00509B61. Scratch100 is a separate eight-byte by-value string carrier.
void area_properties_rooms(Memory&m,AreaPropertyPorts&p,Word self,Word root,Word scratch){
 auto s=[&](Word o){return scratch+o;};auto resource=[&](){return m.read(self+0x108);};
 p.list(resource(),s(0x98),root,0x7477d4);
 Word n=p.count(resource(),s(0x98));m.write(self+0x268,n);p.rooms_allocate(self,n);
 m.write(s(0x18),0);
 for(Word room=0;static_cast<int>(room)<static_cast<int>(m.read(self+0x268));){
  p.element(resource(),s(0x14),s(0x98),room);
  p.literal(s(0x30),0x73d71c);
  Word str=p.string(resource(),s(0xac),s(0x14),0x7477c8,s(0x10),s(0x30));
  p.assign(m.read(self+0x25c)+room*8,str);p.destroy(s(0xac));p.destroy(s(0x30));
  Word value=p.scalar(0x411c90,resource(),s(0x14),0x7477bc,s(0x10),0);m.write(m.read(self+0x260)+room*4,value);
  value=p.float_bits(resource(),s(0x14),0x7477ac,s(0x10),0);m.write(m.read(self+0x264)+room*4,value);
  if(p.list(resource(),s(0xbc),s(0x14),0x7477a0)){
   n=p.count(resource(),s(0xbc));m.write(s(0x58),n);m.write(self+0x26c,m.read(self+0x26c)+n);m.write(s(0x2c),0);
   while(static_cast<int>(m.read(s(0x2c)))<static_cast<int>(m.read(s(0x58)))){
    p.element(resource(),s(0x20),s(0xbc),m.read(s(0x2c)));
    p.copy_string(s(0x100),m.read(self+0x25c)+m.read(s(0x18))*8);p.append_owned_string(self+0x270,s(0x100));
    value=p.scalar(0x411a60,resource(),s(0x20),0x747798,s(0x10),0)&255;m.write(s(0x5c),value);
    Word count=m.read(self+0x280),capacity=m.read(self+0x284);
    if(count==capacity){
     capacity=capacity?capacity*2:16;m.write(s(0x1c),m.read(self+0x27c));m.write(self+0x284,capacity);
     Word data=p.allocate(capacity*4);m.write(self+0x27c,data);
     for(Word i=0;static_cast<int>(i)<static_cast<int>(m.read(self+0x280));++i)m.write(m.read(self+0x27c)+i*4,m.read(m.read(s(0x1c))+i*4));
     if(m.read(s(0x1c)))p.free(m.read(s(0x1c)));
    }
    m.write(m.read(self+0x27c)+m.read(self+0x280)*4,m.read(s(0x5c)));m.write(self+0x280,m.read(self+0x280)+1);
    p.literal(s(0x40),0x73d71c);p.string(resource(),s(0x100),s(0x20),0x74778c,s(0x10),s(0x40));p.append_owned_string(self+0x288,s(0x100));p.destroy(s(0x40));
    p.literal(s(0x78),0x73d71c);p.string(resource(),s(0x100),s(0x20),0x747780,s(0x10),s(0x78));p.append_owned_string(self+0x294,s(0x100));p.destroy(s(0x78));
    p.ref_literal(s(0x60),0x73d71c);Word ref=p.ref(resource(),s(0xd0),s(0x20),0x7442c8,s(0x10),s(0x60));
    Word a=m.read(ref),b=m.read(ref+4);m.write(s(0x48),a);Word c=m.read(ref+8),d=m.read(ref+12);m.write(s(0x4c),b);m.write(s(0x54),d);m.write(s(0x50),c);
    Word array=self+0x2a0;count=m.read(array+4);capacity=m.read(array+8);
    if(count==capacity)p.ref_array_grow(array,capacity?capacity*2:16);
    count=m.read(array+4);Word destination=m.read(array)+count*16;m.write(array+4,count+1);p.ref_assign(destination,s(0xe0),s(0x48));
    m.write(s(0x2c),m.read(s(0x2c))+1);
   }
   if(static_cast<int>(n)>0)room=m.read(s(0x18));
  }
  ++room;m.write(s(0x18),room);
 }
}
//00509B62 through00509F6C. Numeric conversion is an explicit x87 provider.
void area_properties_map(Memory&m,AreaPropertyPorts&p,Word self,Word root,Word scratch){
 auto s=[&](Word o){return scratch+o;};auto resource=[&](){return m.read(self+0x108);};
 auto module=[&](){return p.module(m.read(p.global(0x7a39fc)+8));};
 Word ref=module()+0x30;for(Word i=0;i<16;i+=4)m.write(s(0x88+i),m.read(ref+i));
 p.empty(s(0x80));p.empty(s(0x70));p.ref_string(s(0x88),s(0x80));p.format1(s(0x70),0x747774,p.cstr(s(0x80)));
 p.ref_from_string(s(0x60),s(0x70));Word found=p.exists(p.global(0x7a39e8),s(0x60),3,0);
 if(!found){p.ref_from_string(s(0x60),s(0x70));found=p.exists(p.global(0x7a39e8),s(0x60),0xbbf,0);}
 auto scalar=[&](Word label,Word fallback){return p.scalar(0x411c90,resource(),s(0x14),label,s(0x10),fallback);};
 bool enabled=found&&p.structure(resource(),s(0x14),root,0x747770);
 if(enabled){Word value=scalar(0x747768,0);m.write(s(0x24),value);enabled=value!=0;}
 if(enabled){
  m.write(s(0x30),scalar(0x74775c,0));m.write(s(0x58),scalar(0x747754,1));
  bool floating=p.field_type(resource(),s(0x14),0x74774c,0xffffffff)==8;
  auto number=[&](Word label,Word factor){return floating?p.map_float_integer(resource(),s(0x14),label,s(0x10),factor):scalar(label,0);};
  Word first=number(0x74774c,0x747748);m.write(s(0x20),number(0x747740,0x7455d4));m.write(s(0x2c),number(0x747738,0x747748));m.write(s(0x1c),number(0x747730,0x7455d4));
  m.write(s(0x40),p.float_bits(resource(),s(0x14),0x747724,s(0x10),0));m.write(s(0x78),p.float_bits(resource(),s(0x14),0x747718,s(0x10),0));
  m.write(s(0x18),p.float_bits(resource(),s(0x14),0x74770c,s(0x10),0));m.write(s(0x5c),p.float_bits(resource(),s(0x14),0x747700,s(0x10),0));
  Word mod=module();Word args[]={1,m.read(s(0x24)),m.read(s(0x30)),m.read(s(0x40)),m.read(s(0x78)),m.read(s(0x18)),m.read(s(0x5c)),first,m.read(s(0x20)),m.read(s(0x2c)),m.read(s(0x1c)),m.read(s(0x58))};p.map_setup(m.read(mod+0x218),args);
 }else{Word mod=module();Word args[]={0,88,0,0,0,0,0,0,0,0,0,1};p.map_setup(m.read(mod+0x218),args);}
}
//00509F6D through0050A0D6, including long-lived string cleanup.
Word area_properties_finish(Memory&m,AreaPropertyPorts&p,Word self,Word root,Word scratch){
 auto scalar=[&](Word fn,Word label,Word fallback){return p.scalar(fn,m.read(self+0x108),root,label,scratch+0x10,fallback);};
 Word value=scalar(0x411c20,0x747634,m.read(self+0x2b4));if(m.read(self+0x2b8)>value)m.write(self+0x2b8,value);m.write(self+0x2b4,value);
 value=scalar(0x411c20,0x747620,value);if(value>m.read(self+0x2b4))value=m.read(self+0x2b4);m.write(self+0x2b8,value);
 m.write(self+0x2bc,scalar(0x411c20,0x747610,m.read(self+0x2bc)));m.write(self+0x2c0,scalar(0x411a60,0x7475fc,m.read(self+0x2c0))&255);
 m.write(self+0x2c4,scalar(0x411a60,0x7475ec,m.read8(self+0x2c4))&255);p.byte(self+0x2c8,scalar(0x411a60,0x7475dc,m.read8(self+0x2c8)));p.byte(self+0x2c9,scalar(0x411a60,0x7475cc,m.read8(self+0x2c9)));
 p.destroy(scratch+0x70);p.destroy(scratch+0x80);p.destroy(scratch+0x38);return 1;
}
Word area_properties_00508c50(Memory&m,AreaPropertyPorts&p,Word self,Word root,Word scratch){
 if(!area_properties_header(m,p,self,root,scratch))return 0;
 area_properties_scripts(m,p,self,root,scratch);area_properties_environment(m,p,self,root,scratch);area_properties_rooms(m,p,self,root,scratch);area_properties_map(m,p,self,root,scratch);return area_properties_finish(m,p,self,root,scratch);
}
}
