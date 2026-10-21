#include "9d2df4cef05c408e_gff_write_complex.hpp"
namespace kotor::server_mainloop {
static void copy(Memory&m,GffWriteComplexPorts&p,Word dst,Word src,Word bytes){for(Word i=0;i<(bytes>>2);++i)m.write(dst+i*4,m.read(src+i*4));for(Word i=(bytes>>2)*4;i<bytes;++i)p.byte(dst+i,m.read8(src+i));}
Word gff_write_resref_004128c0(Memory&m,GffWriteComplexPorts&p,Word self,Word root,Word value,Word label){
 if(!root||!label)return 0;
 if(m.read(self+0x98))p.owned(self);
 Word field=p.add_field(self,root,label,11);if(!field)return 0;
 Word length=p.ref_length(value)&255,data=p.grow_data(self,0xffffffff,0,length+1);if(!data)return 0;
 m.write(field+8,data-m.read(self+0x5c));p.byte(data,length);Word text=p.ref_text(value);
 for(Word i=0;i<length;++i){Word c=m.read8(text+i);if(c>='A'&&c<='Z')c+=32;p.byte(data+1+i,c);}return 1;
}
Word gff_write_string_00412970(Memory&m,GffWriteComplexPorts&p,Word self,Word root,Word value,Word label){
 if(!root||!label)return 0;
 if(m.read(self+0x98))p.owned(self);
 Word field=p.add_field(self,root,label,10);if(!field)return 0;
 Word length=p.string_length(value),data=p.grow_data(self,0xffffffff,0,length+4);if(!data)return 0;
 m.write(field+8,data-m.read(self+0x5c));m.write(data,length);copy(m,p,data+4,p.string_text(value),length);return 1;
}
Word gff_write_localized_00412a10(Memory&m,GffWriteComplexPorts&p,Word self,Word root,Word value,Word label,Word scratch){
 Word text=scratch+0x20,language=scratch+0x18,gender=scratch+0xf;p.string_construct(text);
 auto fail=[&](){p.string_destroy(text);return Word(0);};
 if(!root||!label)return fail();
 if(m.read(self+0x98))p.owned(self);
 Word field=p.add_field(self,root,label,12);if(!field)return fail();
 Word length=8;for(Word i=0;i<p.localized_count(value);++i)length+=p.localized_length(value,i)+8;
 Word data=p.grow_data(self,0xffffffff,0,length+4);if(!data)return fail();
 m.write(field+8,data-m.read(self+0x5c));m.write(data,length);m.write(data+4,m.read(value+4));m.write(data+8,p.localized_count(value));Word cursor=data+12,result=1;
 for(Word i=0;i<p.localized_count(value);++i){
  Word bytes;if(!p.localized_get(value,i,language,text,gender)){result=0;bytes=0;m.write(language,0xffffffff);p.byte(gender,0);}else bytes=p.string_length(text);
  m.write(cursor,m.read8(gender)+m.read(language)*2);m.write(cursor+4,bytes);copy(m,p,cursor+8,p.string_text(text),bytes);cursor+=bytes+8;
 }
 p.string_destroy(text);return result;
}
}
