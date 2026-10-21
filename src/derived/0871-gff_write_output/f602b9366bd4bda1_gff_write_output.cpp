#include "464891f8ed268e61_gff_write_output.hpp"
namespace kotor::server_mainloop {
static void copy(Memory&m,GffStoragePorts&p,Word dst,Word src,Word bytes){for(Word i=0;i<(bytes>>2);++i)m.write(dst+i*4,m.read(src+i*4));for(Word i=(bytes>>2)*4;i<bytes;++i)p.byte(dst+i,m.read8(src+i));}
void gff_pack_00412db0(Memory&m,GffStoragePorts&p,Word self,Word percentage,Word threshold){
 Word fieldWaste=m.read(self+0x6c);if(!fieldWaste&&!m.read(self+0x78))return;
 Word header=m.read(self+0x40);if(!header||!m.read(self+0x64)||!m.read(self+0x70)||!m.read(self+0x44)||!m.read(self+0x4c))return;
 Word wasted=fieldWaste+m.read(self+0x78);if(wasted<=threshold)return;
 Word bytes=(m.read(header+12)+m.read(header+20))*12+m.read(header+28)*16+m.read(header+52)+m.read(header+44)+m.read(header+36)+56;
 if((wasted*100)/bytes<=(percentage&255))return;
 if(fieldWaste){
  Word capacity=m.read(header+44)-fieldWaste;
  if(capacity){
   Word data=p.allocate(capacity),used=0;
   for(Word i=0;i<m.read(m.read(self+0x40)+12);++i){Word off=i*12,record=m.read(self+0x44)+off,count=m.read(record+8);if(count>1){Word start=m.read(record+4),size=m.read(m.read(self+0x40)+44);if(start<size&&size-start>=count*4){copy(m,p,data+used,m.read(self+0x64)+start,count*4);m.write(m.read(self+0x44)+off+4,used);used+=m.read(m.read(self+0x44)+off+8)*4;}else{m.write(record+4,0xffffffff);m.write(m.read(self+0x44)+off+8,0);}}}
   p.free(m.read(self+0x64));m.write(self+0x64,data);m.write(m.read(self+0x40)+44,used);
  }else m.write(header+44,0);
  m.write(self+0x6c,0);
 }
 Word listWaste=m.read(self+0x78);if(!listWaste)return;
 Word current=m.read(self+0x40),capacity=m.read(current+52)-listWaste;
 if(capacity){
  Word data=p.allocate(capacity),used=0;
  for(Word i=0;i<m.read(m.read(self+0x40)+20);++i){Word off=i*12,field=m.read(self+0x4c)+off;if(m.read(field)==15){Word start=m.read(field+8),size=m.read(m.read(self+0x40)+52);bool valid=start<size&&size-start>=4;Word count=0;if(valid){count=m.read(m.read(self+0x70)+start);valid=size-start>=count*4+4;}if(valid){copy(m,p,data+used,m.read(self+0x70)+start,count*4+4);m.write(m.read(self+0x4c)+off+8,used);used+=count*4+4;}else m.write(field+8,0xffffffff);}}
  p.free(m.read(self+0x70));m.write(self+0x70,data);m.write(m.read(self+0x40)+52,used);
 }else m.write(current+52,0);
 m.write(self+0x78,0);
}
Word gff_write_stream_004113d0(Memory&m,GffOutputPorts&p,Word self,Word file,Word sizeOut){
 if(!m.read(self+0x40))return 0;
 const Word counts[]={12,20,28,36,44,52},strides[]={12,12,16,1,1,1},slots[]={0x44,0x4c,0x54,0x5c,0x64,0x70};
 m.write(sizeOut,56);
 for(unsigned i=0;i<6;++i){m.write(m.read(self+0x40)+counts[i]-4,m.read(sizeOut));m.write(sizeOut,m.read(sizeOut)+m.read(m.read(self+0x40)+counts[i])*strides[i]);}
 if(p.write(file,m.read(self+0x40),56,1)!=56)return 0;
 for(unsigned i=0;i<6;++i){Word size=m.read(m.read(self+0x40)+counts[i])*strides[i];if(p.write(file,m.read(self+slots[i]),size,1)!=size)return 0;}return 1;
}
Word gff_write_file_00413030(Memory&m,GffOutputPorts&p,Word self,Word path,Word type,Word scratch){
 Word mode=scratch+8,file=scratch+4;p.string_literal(mode,0x73d8a4);p.file_construct(file,path,type,mode);p.string_destroy(mode);
 if(!p.file_open(file)){p.file_destroy(file);return 0;}
 gff_pack_00412db0(m,p,self,0,0);Word result=gff_write_stream_004113d0(m,p,self,file,scratch+0x24);p.file_destroy(file);return result;
}
}
