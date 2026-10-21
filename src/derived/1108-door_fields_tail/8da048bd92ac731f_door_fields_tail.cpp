#include "df335c9e8522ee3b_door_fields_tail.hpp"
namespace kotor::server_mainloop {
Word door_fields_tail_0058a424(Memory&m,DoorFieldTailPorts&p,Word self,Word gff,Word root,Word,Word s){
 const Word status=s+0xc,temp=s+0x10;Word value=p.byte(gff,root,0x745368,status,m.read(self+0xf8))&255;m.write(temp,value);
 if(!m.read(status))m.write(temp,p.byte(gff,root,0x748f64,status,m.read(self+0xf8))&255);
 value=p.byte(gff,root,0x749734,status,0)&255;Word flag=value?1:m.read(temp);m.write(self+0x3c0,value);Word fallback=m.read(self+0x200);m.write(self+0xf8,flag);
 value=p.byte(gff,root,0x7497c0,status,fallback)&255;m.write(self+0x200,value);
 Word out=p.string_read(gff,s+0x28,root,0x74979c,status,self+0x2d0);p.assign(self+0x2d0,out);p.string_destroy(s+0x28);
 value=p.byte(gff,root,0x7496d4,status,m.read(self+0x2d8))&255;p.write_byte(temp,m.read8(self+0x2bf));fallback=m.read(temp);m.write(self+0x2d8,value);p.write_byte(self+0x2bf,p.byte(gff,root,0x7497a4,status,fallback));
 const Word offsets[]={0x2c0,0x2c1};const Word labels[]={0x7496c8,0x749870};for(unsigned i=0;i<2;++i){p.write_byte(temp,m.read8(self+offsets[i]));p.write_byte(self+offsets[i],p.byte(gff,root,labels[i],status,m.read(temp)));}
 out=p.string_read(gff,s+0x28,root,0x747a28,status,self+0x18);p.tag(self,out);p.string_destroy(s+0x28);
 p.ref_empty(temp);out=p.ref_read(gff,s+0x48,root,0x7496b8,status,temp);
 Word a=m.read(out),b=m.read(out+4);m.write(s+0x28,a);Word c=m.read(out+8);m.write(s+0x2c,b);Word d=m.read(out+12);m.write(s+0x30,c);m.write(s+0x34,d);p.ref_assign(self+0x2a2,s+0x58,s+0x28);
 value=p.word_value(gff,root,0x74937c,status,65535)&65535;
 if(value>=65534){p.ref_default(s+0x28);p.ref_empty(temp);out=p.ref_read(gff,s+0x58,root,0x749370,status,temp);p.ref_assign(s+0x28,s+0x48,out);Word raw=p.ref_raw(s+0x28);if(m.read8(raw))p.portrait_ref(self,s+0x28);else p.portrait_id(self,0x22e);}
 else p.portrait_id(self,value?value:0x22e);
 p.write_byte(self+0x2c2,p.byte(gff,root,0x749718,status,0));
 const Word locLabels[]={0x7497e0,0x7488c0};const Word locOffsets[]={0x39c,0x3a4};for(unsigned i=0;i<2;++i){Word def=p.localized_construct(temp);out=p.localized_read(gff,s+0x28,root,locLabels[i],status,def);p.localized_assign(self+locOffsets[i],out);p.localized_destroy(s+0x28);p.localized_destroy(temp);}
 struct Script{Word fallback,destination,label;};const Script scripts[]={
 {0x230,0x230,0x749684},{0x230,0x238,0x749678},{0x240,0x240,0x744d0c},{0x248,0x248,0x74966c},{0x250,0x250,0x747a4c},{0x258,0x258,0x749654},{0x260,0x260,0x749644},{0x228,0x228,0x74963c},{0x268,0x268,0x74962c},{0x270,0x270,0x7495ec},{0x278,0x278,0x749620},{0x280,0x280,0x747a3c},{0x288,0x288,0x749868},{0x298,0x298,0x749858},{0x290,0x290,0x74960c}};
 for(const auto&v:scripts){p.ref_from_string(s+0x28,self+v.fallback);out=p.ref_read(gff,s+0x58,root,v.label,status,s+0x28);Word text=p.ref_text(out);p.string_text(temp,text);p.assign(self+v.destination,temp);p.string_destroy(temp);}
 p.write_byte(temp,m.read8(self+0x2f8));value=p.byte(gff,root,0x74973c,status,m.read(temp));Word script=self+0x270;p.write_byte(self+0x2f8,value);
 if(!script||p.string_empty(script)||p.string_equal_literal(script,0x73df6c)){
  p.string_zero(temp);p.string_literal(s+0x28,0x7495e0);Word row=m.read8(self+0x2f8),table=m.read(m.read(m.read(0x7a3a28)+0xb8)+0x8c);p.table_string(table,row,s+0x28,temp);p.string_destroy(s+0x28);p.assign(self+0x270,temp);p.string_destroy(temp);
 }
 m.write(self+0x2fc,p.byte(gff,root,0x74978c,status,m.read(self+0x2fc))&255);
 value=p.byte(gff,root,0x74977c,status,m.read(self+0x300))&255;p.write_byte(temp,m.read8(self+0x2ec));fallback=m.read(temp);m.write(self+0x300,value);p.write_byte(self+0x2ec,p.byte(gff,root,0x749770,status,fallback));
 p.write_byte(temp,m.read8(self+0x2ed));p.write_byte(self+0x2ed,p.byte(gff,root,0x749760,status,m.read(temp)));
 value=p.byte(gff,root,0x749754,status,m.read(self+0x2e8))&255;fallback=m.read(self+0x304);m.write(self+0x2e8,value);m.write(self+0x304,p.byte(gff,root,0x749748,status,fallback)&255);
 value=p.byte(gff,root,0x7496f0,status,m.read(self+0x2c4))&255;fallback=m.read(self+0x2c8);m.write(self+0x2c4,value);m.write(self+0x2c8,p.byte(gff,root,0x7496f8,status,fallback)&255);
 p.write_byte(self+0x384,p.byte(gff,root,0x747a94,status,0));p.string_literal(s+0x28,0x73d71c);out=p.string_read(gff,temp,root,0x747aa4,status,s+0x28);p.assign(self+0x388,out);p.string_destroy(temp);p.string_destroy(s+0x28);
 out=p.localized_read(gff,s+0x28,root,0x747ab0,status,self+0x3c8);p.localized_assign(self+0x3c8,out);p.localized_destroy(s+0x28);
 p.ref_default(s+0x38);p.ref_empty(s+0x28);out=p.ref_read(gff,s+0x58,root,0x747ac8,status,s+0x28);p.ref_assign(s+0x38,s+0x48,out);if(m.read(status)==1)p.assign_literal(self+0x390,p.ref_text(s+0x38));
 m.write16(self+0x3ac,p.word_value(gff,root,0x747880,status,0));p.string_zero(s+0x20);
 if(m.read8(self+0x2a0)){
  p.string_literal(s+0x28,0x7499a0);Word row=m.read8(self+0x2a0),table=m.read(m.read(m.read(0x7a3a28)+0xb8)+0x24);p.table_string(table,row,s+0x28,s+0x20);p.string_destroy(s+0x28);
  m.write(temp,1);p.string_literal(s+0x28,0x749990);row=m.read8(self+0x2a0);table=m.read(m.read(m.read(0x7a3a28)+0xb8)+0x24);p.table_integer(table,row,s+0x28,temp);p.string_destroy(s+0x28);m.write(self+0x308,m.read(temp));m.write(self+0x3c4,0);
 }else{
  p.string_literal(s+0x28,0x749814);Word table=m.read(m.read(m.read(0x7a3a28)+0xb8)+0x28),row=m.read8(self+0x2a1);p.table_string(table,row,s+0x28,s+0x20);p.string_destroy(s+0x28);
  m.write(temp,0);p.string_literal(s+0x28,0x749820);row=m.read8(self+0x2a1);table=m.read(m.read(m.read(0x7a3a28)+0xb8)+0x28);p.table_integer(table,row,s+0x28,temp);p.string_destroy(s+0x28);m.write(self+0x3c4,m.read(temp)!=0);
 }
 Word index=m.read(0x74c394);if(static_cast<int>(index)<=static_cast<int>(m.read(0x74c39c))){Word slot=self+0x32c+index*4;do{Word allocation=p.allocate(0x94);Word mesh=allocation?p.mesh_construct(allocation):0;m.write(slot,mesh);p.mesh_state(mesh,index);out=p.ref_from_string(s+0x68,s+0x20);p.ref_assign(m.read(slot)+0xc,s+0x78,out);p.mesh_load(m.read(slot),0);p.mesh_finish(m.read(slot));Word last=m.read(0x74c39c);++index;slot+=4;if(static_cast<int>(index)>static_cast<int>(last))break;}while(true);}
 p.finish(self);p.string_destroy(s+0x20);return 1;
}
}
