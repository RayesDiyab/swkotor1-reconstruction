#include "ffd821e460ba85cb_gff_write_core.hpp"
namespace kotor::server_mainloop {
Word gff_add_struct_00410d60(Memory&m,GffStoragePorts&p,Word self,Word type){
 Word header=m.read(self+0x40);if(!header||!m.read(self+0x44))return 0xffffffff;
 Word count=m.read(header+12);
 if(count>=m.read(self+0x48)){
  Word growth=m.read(self+0x28),capacity=count+growth;m.write(self+0x48,capacity);m.write(self+0x28,growth*2);
  Word data=p.allocate(capacity*12),old=m.read(self+0x44),bytes=count*12;
  for(Word i=0;i<(bytes>>2);i++)m.write(data+i*4,m.read(old+i*4));
  for(Word i=(bytes>>2)*4;i<bytes;i++)p.byte(data+i,m.read8(old+i));
  p.free(m.read(self+0x44));m.write(self+0x44,data);
 }
 m.write(m.read(self+0x40)+12,count+1);Word offset=count*12;
 m.write(m.read(self+0x44)+offset,type);m.write(m.read(self+0x44)+offset+4,0xffffffff);m.write(m.read(self+0x44)+offset+8,0);return count;
}
Word gff_initialize_00411260(Memory&m,GffWriteCorePorts&p,Word self,Word root,Word fileType,Word,Word scratch){
 Word version=scratch+12;p.string_literal(version,m.read(0x78d3cc));
 if(!root||p.string_length(fileType)<4||p.string_length(version)<4){p.string_destroy(version);return 0;}
 if(m.read(self+0x98)){p.release_004108c0(self);}
 p.owned(self);
 auto four=[&](Word text){Word v=p.string_char(text,3)&255;v=(v<<8)|(p.string_char(text,2)&255);v=(v<<8)|(p.string_char(text,1)&255);return (v<<8)|(p.string_char(text,0)&255);};
 Word type=four(fileType);m.write(m.read(self+0x40),type);Word ver=four(version);m.write(m.read(self+0x40)+4,ver);
 Word index=p.add_struct(self,0xffffffff);m.write(root,index);p.string_destroy(version);return index!=0xffffffff?1:0;
}
Word gff_scalar_write(Memory&m,GffWriteCorePorts&p,Word self,Word root,Word value,Word label,Word type){
 if(!root||!label)return 0;
 if(m.read(self+0x98))p.owned(self);
 Word field=p.add_field(self,root,label,type);if(!field)return 0;
 switch(type){case 0:value&=255;break;case 1:value=(value&255)|((value&128)?0xffffff00:0);break;case 2:value&=65535;break;case 3:value=(value&65535)|((value&32768)?0xffff0000:0);break;default:break;}
 m.write(field+8,value);return 1;
}
Word gff_add_list_00412450(Memory&m,GffWriteCorePorts&p,Word self,Word out,Word root,Word label){
 if(!out||!root||!label)return 0;
 if(m.read(self+0x98))p.owned(self);
 Word field=p.add_field(self,root,label,15);if(!field)return 0;
 Word data=p.grow_list(self,m.read(field+8),0,4);if(!data)return 0;
 m.write(field+8,data-m.read(self+0x70));m.write(data,0);m.write(out,m.read(root));
 bool ended=false;for(Word i=0;i<16;++i){Word c=ended?0:m.read8(label+i);if(!c)ended=true;p.byte(out+4+i,c);}
 return 1;
}
Word gff_add_list_struct_004124e0(Memory&m,GffWriteCorePorts&p,Word self,Word out,Word list,Word type,Word scratch){
 if(!list||!out)return 0;
 if(m.read(self+0x98))p.owned(self);
 Word index=p.field_index(self,list,list+4);Word field=p.field(self,list,index);if(!field||m.read(field)!=15)return 0;
 Word data=p.data(self,field,scratch);if(!data)return 0;Word size=m.read(scratch);if(size<4)return 0;
 Word count=m.read(data),oldSize=count*4+4;if(size<oldSize)return 0;
 Word grown=p.grow_list(self,m.read(field+8),oldSize,count*4+8);if(!grown)return 0;
 m.write(field+8,grown-m.read(self+0x70));Word child=p.add_struct(self,type);m.write(out,child);if(child==0xffffffff)return 0;
 m.write(grown,count+1);m.write(grown+count*4+4,m.read(out));return 1;
}
}
