#include "f11283c9e96b5282_sound_load.hpp"
namespace kotor::server_mainloop {
Word sound_load_005c9040(Memory&m,SoundLoadPorts&p,Word self,Word gff,Word root,Word s){
 m.write(s+0x1c,0);m.write(s+0x20,0);m.write(s+0x24,0);p.ref_construct(s+0x2c);
 Word text=p.string(gff,s+0x14,root,0x747a28,s+0x10,self+0x18);p.tag(self,text);p.string_destroy(s+0x14);
 Word value=p.byte_read(gff,root,0x749d58,s+0x10,m.read(self+0x228))&255;
 Word next=m.read(self+0x22c);m.write(self+0x228,value);m.write(self+0x22c,p.byte_read(gff,root,0x74c250,s+0x10,next)&255);
 value=p.byte_read(gff,root,0x747798,s+0x10,m.read(self+0x230))&255;
 next=m.read(self+0x234);m.write(self+0x230,value);value=p.byte_read(gff,root,0x74c248,s+0x10,next)&255;
 next=m.read(self+0x238);m.write(self+0x234,value);m.write(self+0x238,p.byte_read(gff,root,0x74c23c,s+0x10,next)&255);
 value=p.byte_read(gff,root,0x74c234,s+0x10,m.read(self+0x23c))&255;
 next=m.read(self+0x240);m.write(self+0x23c,value);m.write(s+0x14,next);value=p.floating(gff,root,0x74c224,s+0x10,next);
 next=m.read(self+0x244);m.write(self+0x240,value);value=p.dword(gff,root,0x74c21c,s+0x10,next);
 next=p.byte(self+0x24c);m.write(self+0x244,value);p.byte_write(self+0x24c,p.dword(gff,root,0x7474e8,s+0x10,next));
 m.write(self+0x268,p.dword(gff,root,0x744cb8,s+0x10,m.read(self+0x268)));
 value=p.dword(gff,root,0x74c20c,s+0x10,m.read(self+0x26c));next=m.read(self+0x270);m.write(self+0x26c,value);m.write(s+0x14,next);value=p.floating(gff,root,0x74c200,s+0x10,next);
 next=m.read(self+0x274);m.write(self+0x270,value);m.write(s+0x14,next);value=p.floating(gff,root,0x74c1f4,s+0x10,next);
 next=m.read(self+0x278);m.write(self+0x274,value);m.write(self+0x278,p.byte_read(gff,root,0x74c1e8,s+0x10,next)&255);
 value=p.byte_read(gff,root,0x74c1e0,s+0x10,m.read(self+0x27c))&255;next=m.read(self+0x248);m.write(self+0x27c,value);m.write(s+0x14,next);value=p.floating(gff,root,0x74c1d0,s+0x10,next);
 next=m.read(self+0x250);m.write(self+0x248,value);value=p.byte_read(gff,root,0x74c1c0,s+0x10,next)&255;
 next=m.read(self+0x254);m.write(self+0x250,value);m.write(s+0x14,next);value=p.floating(gff,root,0x74c1b0,s+0x10,next);
 next=m.read(self+0x258);m.write(self+0x254,value);m.write(s+0x14,next);m.write(self+0x258,p.floating(gff,root,0x74c1a0,s+0x10,next));
 m.write(s+0x1c,p.floating(gff,root,0x745d80,s+0x10,0));m.write(s+0x20,p.floating(gff,root,0x745d74,s+0x10,0));m.write(s+0x24,p.floating(gff,root,0x745d68,s+0x10,0));p.position(self,s+0x1c,1,1,0);
 if(p.list(gff,s+0x4c,root,0x74c198)){
  Word count=p.count(gff,s+0x4c);m.write(s+0x14,count);if(static_cast<std::int32_t>(count)>0){Word index=0;do{
   if(p.element(gff,s+0x28,s+0x4c,index)){
    p.ref_empty(s+0x60);Word ref=p.ref_read(gff,s+0x90,s+0x28,0x7442c8,s+0x10,s+0x60);p.ref_assign(s+0x2c,s+0x70,ref);
    if(m.read(s+0x10)){
     Word a=m.read(s+0x2c),b=m.read(s+0x30),c=m.read(s+0x34);m.write(s+0x3c,a);Word d=m.read(s+0x38);m.write(s+0x40,b);Word used=m.read(self+0x260);m.write(s+0x48,d);Word capacity=m.read(self+0x264);m.write(s+0x44,c);
     if(used==capacity)p.grow(self+0x25c,capacity?capacity*2:16);
     used=m.read(self+0x260);Word array=m.read(self+0x25c);m.write(self+0x260,used+1);p.ref_assign(array+used*16,s+0x80,s+0x3c);count=m.read(s+0x14);
    }
   }
   ++index;
  }while(static_cast<std::int32_t>(index)<static_cast<std::int32_t>(count));}
 }
 return m.read(s+0x10);
}
}
