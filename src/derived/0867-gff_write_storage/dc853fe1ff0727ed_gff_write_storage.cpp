#include "9bdb5a18c1300200_gff_write_storage.hpp"
#include <initializer_list>
namespace kotor::server_mainloop {
void gff_owned_storage_00410aa0(Memory&m,GffStoragePorts&p,Word self){
 Word header=p.allocate(0x38);
 for(Word off:{8u,0x10u,0x18u,0x20u,0x28u,0x30u})m.write(header+off,0);
 if(m.read(self+0x40)){
  m.write(header,m.read(m.read(self+0x40)));m.write(header+4,m.read(m.read(self+0x40)+4));
  const Word count_offsets[]={0xcu,0x14u,0x1cu,0x24u,0x2cu,0x34u};
  const Word capacities[]={0x48u,0x50u,0x58u,0x60u,0x68u,0x74u};
  const Word growth[]={0x28u,0x2cu,0x30u,0x34u,0x38u,0x3cu};
  for(unsigned i=0;i<6;i++){m.write(header+count_offsets[i],m.read(m.read(self+0x40)+count_offsets[i]));m.write(self+capacities[i],m.read(m.read(self+0x40)+count_offsets[i])+m.read(self+growth[i]));}
 }else{
  m.write(header,0);m.write(header+4,0);
  const Word counts[]={0xcu,0x14u,0x1cu,0x24u,0x2cu,0x34u};
  const Word capacities[]={0x48u,0x50u,0x58u,0x60u,0x68u,0x74u};
  const Word sizes[]={0x6eu,0x286u,0x62u,0x72cu,0xfd4u,0xfd4u};
  for(unsigned i=0;i<6;i++){m.write(header+counts[i],0);m.write(self+capacities[i],sizes[i]);}
 }
 const Word slots[]={0x44u,0x4cu,0x54u,0x5cu,0x64u,0x70u};
 const Word capacities[]={0x48u,0x50u,0x58u,0x60u,0x68u,0x74u};
 const Word counts[]={0xcu,0x14u,0x1cu,0x24u,0x2cu,0x34u};
 const Word strides[]={12,12,16,1,1,1};Word data[6];
 for(unsigned i=0;i<6;i++){
  data[i]=p.allocate(m.read(self+capacities[i])*strides[i]);Word old=m.read(self+slots[i]);
  if(old){Word bytes=m.read(m.read(self+0x40)+counts[i])*strides[i];Word words=bytes>>2;for(Word j=0;j<words;j++)m.write(data[i]+j*4,m.read(old+j*4));for(Word j=words*4;j<bytes;j++)p.byte(data[i]+j,m.read8(old+j));}
 }
 if(m.read(self+0x98))p.release_004108c0(self);
 else for(Word off:{0x70u,0x64u,0x5cu,0x54u,0x4cu,0x44u,0x40u})p.free(m.read(self+off));
 m.write(self+0x70,data[5]);m.write(self+0x64,data[4]);m.write(self+0x5c,data[3]);m.write(self+0x40,header);m.write(self+0x54,data[2]);m.write(self+0x4c,data[1]);m.write(self+0x44,data[0]);
}
}
