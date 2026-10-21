#include "b13f99bca3e22c1b_gff_write_fields.hpp"
namespace kotor::server_mainloop {
static void copy(Memory&m,GffStoragePorts&p,Word dst,Word src,Word bytes){
 for(Word i=0;i<(bytes>>2);++i)m.write(dst+i*4,m.read(src+i*4));
 for(Word i=(bytes>>2)*4;i<bytes;++i)p.byte(dst+i,m.read8(src+i));
}
Word gff_grow_buffer(Memory&m,GffStoragePorts&p,Word self,Word offset,Word oldSize,Word newSize,unsigned section){
 const Word ptrs[]={0x5c,0x64,0x70},caps[]={0x60,0x68,0x74},growths[]={0x34,0x38,0x3c},useds[]={0x24,0x2c,0x34},wastes[]={0,0x6c,0x78};
 Word slot=ptrs[section],cap=caps[section],growth=growths[section],used=useds[section];
 Word header=m.read(self+0x40);if(!header||!m.read(self+slot))return 0;
 Word base=m.read(header+used);if(base==offset+oldSize)base=offset;
 Word total=base+newSize;
 if(total>=m.read(self+cap)){
  Word increment=m.read(self+growth),capacity=total+increment;
  m.write(self+cap,capacity);m.write(self+growth,increment*2);
  Word data=p.allocate(capacity);copy(m,p,data,m.read(self+slot),m.read(m.read(self+0x40)+used));
  p.free(m.read(self+slot));m.write(self+slot,data);
 }
 if(base!=offset){
  Word buffer=m.read(self+slot);copy(m,p,buffer+base,buffer+offset,oldSize);
  if(wastes[section])m.write(self+wastes[section],m.read(self+wastes[section])+oldSize);
 }
 m.write(m.read(self+0x40)+used,total);return m.read(self+slot)+base;
}
Word gff_add_label_00410e20(Memory&m,GffStoragePorts&p,Word self,Word label){
 Word header=m.read(self+0x40);if(!header||!m.read(self+0x54))return 0xffffffff;
 Word count=m.read(header+0x1c),text[4]={0,0,0,0};bool ended=false;
 for(unsigned i=0;i<16;++i){Word c=ended?0:m.read8(label+i);if(!c)ended=true;text[i/4]|=c<<((i%4)*8);}
 for(Word i=count;i!=0;){--i;Word address=m.read(self+0x54)+i*16;bool match=true;for(unsigned k=0;k<4;++k)if(m.read(address+k*4)!=text[k])match=false;if(match)return i;}
 if(count>=m.read(self+0x58)){
  Word growth=m.read(self+0x30),capacity=count+growth;m.write(self+0x58,capacity);m.write(self+0x30,growth*2);
  Word data=p.allocate(capacity*16);copy(m,p,data,m.read(self+0x54),count*16);p.free(m.read(self+0x54));m.write(self+0x54,data);
 }
 m.write(m.read(self+0x40)+0x1c,count+1);Word address=m.read(self+0x54)+count*16;for(unsigned i=0;i<4;++i)m.write(address+i*4,text[i]);return count;
}
Word gff_add_field_00411730(Memory&m,GffStoragePorts&p,Word self,Word root,Word label,Word type){
 if(!label||!root)return 0;
 Word header=m.read(self+0x40);if(!header)return 0;Word records=m.read(self+0x44);if(!records)return 0;
 Word index=m.read(root);if(index>=m.read(header+12))return 0;Word record=records+index*12;if(!record)return 0;
 Word count=m.read(header+0x14);
 if(count>=m.read(self+0x50)){
  Word growth=m.read(self+0x2c),capacity=count+growth;m.write(self+0x50,capacity);m.write(self+0x2c,growth*2);
  Word data=p.allocate(capacity*12);copy(m,p,data,m.read(self+0x4c),count*12);p.free(m.read(self+0x4c));m.write(self+0x4c,data);
 }
 m.write(m.read(self+0x40)+0x14,count+1);Word fields=m.read(record+8);
 if(!fields)m.write(record+4,count);
 else {
  Word indices;
  if(fields==1){indices=gff_grow_buffer(m,p,self,0xffffffff,0,8,1);if(!indices)return 0;m.write(indices,m.read(record+4));}
  else {indices=gff_grow_buffer(m,p,self,m.read(record+4),fields*4,fields*4+4,1);if(!indices)return 0;}
  m.write(indices+fields*4,count);m.write(record+4,indices-m.read(self+0x64));
 }
 m.write(record+8,fields+1);Word labelIndex=gff_add_label_00410e20(m,p,self,label),offset=count*12;
 m.write(m.read(self+0x4c)+offset+4,labelIndex);m.write(m.read(self+0x4c)+offset,type);m.write(m.read(self+0x4c)+offset+8,0xffffffff);return m.read(self+0x4c)+offset;
}
}
