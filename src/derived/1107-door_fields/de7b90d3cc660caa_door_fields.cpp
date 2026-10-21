#include "9e3f32cd7e2756fb_door_fields.hpp"
namespace kotor::server_mainloop {
Word door_fields_0058a1f0(Memory&m,DoorFieldPorts&p,Word self,Word gff,Word root,Word templ,Word s){
 Word status=s+0xc,temp=s+0x10;
 Word value=p.dword(gff,root,0x746efc,status,m.read8(self+0x2a0));Word next=m.read8(self+0x2a1);p.write_byte(self+0x2a0,value);p.write_byte(temp,next);
 value=p.byte(gff,root,0x74988c,status,m.read(temp));next=m.read8(self+0x2cc);p.write_byte(self+0x2a1,value);p.write_byte(temp,next);
 value=p.byte(gff,root,0x749880,status,m.read(temp));if(!m.read(self+0x308))value=3;value&=255;p.write_byte(self+0x2cd,value);p.write_byte(self+0x2cc,value);
 if(value<=3){const Word states[]={0x2726,0x2742,0x2743,0x2758};p.state(self,states[value]);}
 p.write_byte(temp,m.read8(self+0x2f0));value=p.byte(gff,root,0x7497d0,status,m.read(temp));Word angle=m.read(self+0x2b4);m.write(self+0x2f0,value&255);m.write(temp,angle);
 value=p.floating(gff,root,0x747a8c,status,angle);Word fallback=m.read(self+0x2b8);m.write(self+0x2b4,value);m.write(self+0x2b8,p.dword(gff,root,0x7497c8,status,fallback));
 const Word offsets[]={0x2bc,0x2be,0x2bd};const Word labels[]={0x749710,0x749708,0x749704};
 for(unsigned i=0;i<3;++i){p.write_byte(temp,m.read8(self+offsets[i]));p.write_byte(self+offsets[i],p.byte(gff,root,labels[i],status,m.read(temp)));}
 value=p.short_value(gff,root,0x749730,status,m.read16(self+0xe0));m.write(self+0xe0,static_cast<Word>(static_cast<int>(static_cast<short>(value))));
 fallback=p.current_hp(self);value=p.short_value(gff,root,0x749724,status,fallback);int hp=static_cast<short>(value);if(templ){int maximum=static_cast<short>(m.read16(self+0xe0));if(hp>maximum)hp=maximum;}p.hitpoints(self,static_cast<Word>(hp));
 return p.continuation_0058a424(self,gff,root,templ,s);
}
}
