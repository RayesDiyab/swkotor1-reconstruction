#include "f4ea642378c110e0_trigger_fields.hpp"
namespace kotor::server_mainloop {
Word trigger_fields_0058da80(Memory&m,TriggerFieldPorts&p,Word self,Word g,Word root,Word s){
 const Word status=s+0xc;Word value=p.word_value(g,root,0x74937c,status,0xffff);
 if((value&0xffff)>=0xfffe){p.ref_empty(s+0x74);Word vt=m.read(self);Word r=p.ref_read(g,s+0x24,root,0x749370,status,s+0x74);p.portrait_ref(self,vt,r);}
 else p.portrait_id(self,m.read(self),value);
 p.string_zero(s+0x18);value=p.dword(g,root,0x7473f0,status,0x7f000000);p.owner(self,value);
 const Word offsets[]={0x244,0x24c,0x254,0x25c,0x264,0x26c,0x274};
 const Word labels[]={0x746c94,0x749a28,0x749a18,0x746be8,0x7495ec,0x74966c,0x749868};
 for(unsigned i=0;i<7;++i){p.ref_from_string(s+0x44,self+(i==6?0x24c:offsets[i]));Word r=p.ref_read(g,s+0x64,root,labels[i],status,s+0x44);p.ref_to_string(r,s+0x18);p.assign(self+offsets[i],s+0x18);}
 p.write_byte(s+0x10,m.read8(self+0x2d4));value=p.byte(g,root,0x74973c,status,m.read(s+0x10));p.write_byte(self+0x2d4,value);
 if(self+0x264==0||p.empty(self+0x264)||p.equal_text(self+0x264,0x73df6c)){
  p.string_zero(s+0x10);p.string_construct(s+0x24,0x747d50);
  Word table=m.read(m.read(m.read(0x7a3a28)+0xb8)+0x8c);
  (void)p.table_string(table,m.read8(self+0x2d4),s+0x24,s+0x10);p.string_destroy(s+0x24);p.assign(self+0x264,s+0x10);p.string_destroy(s+0x10);
 }
 m.write(self+0x2d0,p.byte(g,root,0x749748,status,m.read(self+0x2d0))&255);
 p.string_zero(s+0x8c);p.string_construct(s+0x10,0x73d71c);
 Word r=p.string_read(g,s+0x24,root,0x747aa4,status,s+0x10);p.assign(s+0x8c,r);p.string_destroy(s+0x24);p.string_destroy(s+0x10);
 if(m.read(status)==1)p.assign(self+0x230,s+0x8c);
 p.write_byte(self+0x240,p.byte(g,root,0x747a94,status,0));
 p.ref_default(s+0xb0);p.ref_empty(s+0x44);r=p.ref_read(g,s+0x64,root,0x747ac8,status,s+0x44);p.ref_assign(s+0xb0,s+0xa0,r);
 if(m.read(status)==1)p.assign_text(self+0x238,p.ref_text(s+0xb0));
 value=p.byte(g,root,0x7497d0,status,0)&255;if(m.read(status)==1)m.write(self+0x2cc,value);
 p.string_zero(s+0x84);p.string_construct(s+0x10,0x73d71c);r=p.string_read(g,s+0x24,root,0x747a28,status,s+0x10);p.assign(s+0x84,r);p.string_destroy(s+0x24);p.string_destroy(s+0x10);
 if(m.read(status)==1)p.tag(self,s+0x84);
 r=p.localized_construct(s+0x24);r=p.localized_read(g,s+0x10,root,0x748fec,status,r);p.localized_assign(self+0x228,r);p.localized_destroy(s+0x10);p.localized_destroy(s+0x24);
 value=p.dword(g,root,0x7497c8,status,0);if(m.read(status))m.write(self+0x2b8,value);
 p.string_construct(s+0x10,0x73d71c);r=p.string_read(g,s+0x24,root,0x74979c,status,s+0x10);p.assign(self+0x27c,r);p.string_destroy(s+0x24);p.string_destroy(s+0x10);
 m.write(self+0x2c4,p.byte(g,root,0x74978c,status,0)&255);m.write(self+0x2c0,p.byte(g,root,0x74977c,status,0)&255);
 const Word columns[]={0x747d5c,0x747d68},destinations[]={0x2dc,0x2d8};
 for(unsigned i=0;i<2;++i){p.string_construct(s+0x24,columns[i]);Word table=m.read(m.read(m.read(0x7a3a28)+0xb8)+0x8c);m.write(s+0x10,p.table_integer(table,m.read8(self+0x2d4),s+0x24,s+0x54));p.string_destroy(s+0x24);if(m.read(s+0x10))m.write(self+destinations[i],m.read(s+0x54));}
 p.write_byte(self+0x2fc,p.byte(g,root,0x749a10,status,0));p.ref_empty(s+0x44);Word vt=m.read(self);r=p.ref_read(g,s+0x64,root,0x749370,status,s+0x44);p.portrait_ref(self,vt,r);
 r=p.localized_read(g,s+0x10,root,0x747ab0,status,self+0x30c);p.localized_assign(self+0x30c,r);p.localized_destroy(s+0x10);
 value=p.integer(g,root,0x745e38,status,0);if(m.read(status)==1){if(value==1){m.write(self+0x2b4,1);p.write_byte(self+0x2fc,1);}else if(value==2){m.write(self+0x2bc,1);p.write_byte(self+0x2fc,0);}}
 value=p.floating(g,root,0x749a00,status,0x3dcccccd);if(p.positive(value))m.write(self+0x2e0,value);
 m.write(s+0x58,0);m.write(s+0x5c,0);m.write(s+0x60,0);m.write(s+0x40,0);
 m.write(s+0x58,p.floating(g,root,0x745d80,s+0x40,0));m.write(s+0x5c,p.floating(g,root,0x745d74,s+0x40,0));m.write(s+0x60,p.floating(g,root,0x745d68,s+0x40,0));
 m.write(s+0x34,0);m.write(s+0x38,0);m.write(s+0x3c,0);m.write(s+0x20,0);
 m.write(s+0x34,p.floating(g,root,0x745d38,s+0x20,0));m.write(s+0x38,p.floating(g,root,0x745d48,s+0x20,0));m.write(s+0x3c,p.floating(g,root,0x745d58,s+0x20,0));p.normalize_if_needed(s+0x34);
 if(m.read(s+0x20)==1){p.quaternion(s+0xa0,p.angle(self+0x9c));p.quaternion(s+0x64,p.angle(s+0x34));if(static_cast<std::int32_t>(m.read(self+0x284))>0){m.write(s+0x10,0);m.write(s+0x24,0);do{
  Word vertex=m.read(self+0x288)+m.read(s+0x24);p.subtract_position(s+0x94,vertex,self+0x90);r=p.inverse(s+0xe0,s+0xa0);r=p.rotate(s+0xd4,r,s+0x94);r=p.rotate(s+0x40,s+0x64,r);
  Word target=m.read(self+0x288)+m.read(s+0x24);Word next=m.read(s+0x24)+12;m.write(s+0x24,next);p.translated_store(target,s+0x58,r,s+0x74);
  Word index=m.read(s+0x10)+1;Word count=m.read(self+0x284);m.write(s+0x10,index);if(static_cast<std::int32_t>(index)>=static_cast<std::int32_t>(count))break;
 }while(true);}}
 if(m.read(s+0x40))p.position(self,s+0x58);
 if(m.read(s+0x20))p.orientation(self,s+0x34);
 if(p.list(g,s+0xc0,root,0x7474bc))p.geometry(self,g,s+0xc0);
 m.write16(self+0x308,p.word_value(g,root,0x747880,status,0));value=p.byte(g,root,0x7499ec,status,m.read(self+0x314))&255;Word id=m.read(self+4);m.write(self+0x314,value);Word server=m.read(m.read(0x7a39fc)+8);p.register_tag(p.module(server),self+0x18,id);
 p.string_destroy(s+0x84);p.string_destroy(s+0x8c);p.string_destroy(s+0x18);return 1;
}
}
