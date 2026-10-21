#include "245bb0f355eeafeb_client_system_message.hpp"
namespace kotor::server_mainloop {
Word client_system_00675710(Memory&m,ClientSystemPorts&p,Word self,Word packet,Word length,Word scratch){auto s=[&](Word n){return scratch+n;};auto info=[&](){return p.info(m.read(m.read(0x7a39fc)+4));};auto eq=[&](Word text,Word cell){return p.equal(text,m.read(cell))!=0;};auto done=[&](Word result){for(Word off=24;off<=48;off+=8)p.destroy(s(off));return result;};auto flag=[&](Word bits){Word a=info();m.write(a,m.read(a)|bits);};p.empty(s(48));if(m.read8(packet)!=0x53){p.destroy(s(48));return 0;}Word data=packet+1,size=length-1;Word temp=p.counted(s(16),data,size);p.assign(s(48),temp);p.destroy(s(16));p.empty(s(40));p.empty(s(32));p.empty(s(24));Word count=p.parse(self,data,size,s(40),s(32),s(24));
 if(static_cast<int>(count)>=1&&eq(s(40),0x7a249c)){
  if(static_cast<int>(count)>=2){
   if(eq(s(32),0x7a24a8)){m.write16(info()+4,0);flag(1);p.log0(0x752bf8);return done(1);}
   if(eq(s(32),0x7a24a4)){m.write16(info()+4,1);flag(1);p.log0(0x752bd8);p.module_name(info(),s(24));return done(1);}
   if(eq(s(32),0x7a24ac)){m.write16(info()+4,2);flag(1);p.log0(0x752bb4);p.module_name(info(),s(24));return done(1);}
   if(eq(s(32),0x7a24b4)){p.assign(info()+8,s(24));flag(16);p.log0(0x752b9c);Word text=p.cstr(info()+8);p.log0(text);p.log0(0x73d5cc);return done(1);}
   if(p.equal(s(32),0x752b8c))return done(1);
   if(eq(s(32),0x7a24bc)){p.assign(info()+32,s(24));flag(256);Word text=p.cstr(info()+32);p.log1(0x752b64,text);return done(1);}
   if(eq(s(32),0x7a24c4)){p.assign(info()+16,s(24));flag(32);Word text=p.cstr(info()+16);p.log1(0x752b44,text);return done(1);}
  }
  if(eq(s(32),0x7a24a4)){p.module_name(info(),s(24));flag(16);p.empty(s(16));Word run=m.read(0x7a24dc),module=m.read(0x7a24d4);p.format2(s(16),0x7493b4,module,run);p.send(self,s(16));p.destroy(s(16));return done(1);}
 }
 if(static_cast<int>(count)>=2&&eq(s(40),0x7a24d4)&&eq(s(32),0x7a24e8)){p.assign(info()+24,s(24));flag(128);Word text=p.cstr(info()+24);p.log1(0x752b24,text);return done(1);}
 Word original_info=info(),result=0;
 if(count==3)for(;;){Word index=p.option(info(),s(40),s(32));if(!index)break;flag(2);Word entry=original_info+(index<<4)+0x10c,type=m.read(entry+12);if(type==2){Word text=p.cstr(s(24)),value=p.integer(text);m.write(m.read(entry+8),value);}else if(type==1){Word value=(p.character(s(24),0)&255)==0x31;m.write(m.read(entry+8),value);}result=1;Word at=0;if(p.character(s(24),0)&255)for(;;){if((p.character(s(24),at)&255)==10)break;++at;if(!(p.character(s(24),at)&255))break;}Word part=p.left(s(24),s(56),at);Word value=p.cstr(part),key=p.cstr(s(32)),section=p.cstr(s(40));p.log3(0x752b14,section,key,value);p.destroy(s(56));if((p.character(s(24),at)&255)!=10)break;Word size2=p.length(s(24))-at-1;Word bytes=p.cstr(s(24))+at+1;if(p.parse(self,bytes,size2,s(40),s(32),s(24))!=3)break;}
 return done(result);
}
}
