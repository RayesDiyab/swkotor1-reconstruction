#include "5456d596c08da383_area_update.hpp"
namespace kotor::server_mainloop {
namespace {bool even_two(Word value){return value==0||(value&(value-1))!=0;}}
Word area_update_00506fe0(Memory&m,AreaUpdatePorts&p,Word area,Word object,Word s){
 Word count=m.read(area+0x194),id=m.read(object+4),index=0;
 if(static_cast<std::int32_t>(count)>0){Word cursor=m.read(area+0x190);while(m.read(cursor)!=id){++index;cursor+=4;if(static_cast<std::int32_t>(index)>=static_cast<std::int32_t>(count)){index=0xffffffff;break;}}}else index=0xffffffff;
 Word server=m.read(m.read(0x7a39fc)+8);count=m.read(area+0x194);Word array=p.objects(server);Word x=m.read(object+0x90);m.write(s+0x20,object);Word next=index+1;m.write(s+0x14,array);m.write(s+0x10,x);
 if(next!=count){Word offset=index*4;m.write(s+0x18,offset);Word nextId=m.read(m.read(area+0x190)+offset+4);Word status=p.lookup(array,nextId,s+0x20);
  if((status&255)!=m.read8(0x74666c)||(p.compare_status(m.read(m.read(s+0x20)+0x90),m.read(s+0x10))&1)){
   index=next;offset=m.read(s+0x18);do{
    nextId=m.read(m.read(area+0x190)+offset+4);status=p.lookup(m.read(s+0x14),nextId,s+0x20);if((status&255)!=m.read8(0x74666c))return 1;
    if(even_two(p.compare_status(m.read(m.read(s+0x20)+0x90),m.read(s+0x10))&5))return 1;
    Word data=m.read(area+0x190),current=m.read(data+offset),following=m.read(data+offset+4);m.write(data+offset+4,current);m.write(m.read(area+0x190)+offset,following);offset+=4;++index;
   }while(index!=count);return 1;
  }
 }
 if(!index)return 1;
 Word previous=m.read(m.read(area+0x190)+index*4-4);Word status=p.lookup(m.read(s+0x14),previous,s+0x20);
 if((status&255)==m.read8(0x74666c)&&!even_two(p.compare_status(m.read(m.read(s+0x20)+0x90),m.read(s+0x10))&0x41))return 1;
 do{
  previous=m.read(m.read(area+0x190)+index*4-4);status=p.lookup(m.read(s+0x14),previous,s+0x20);if((status&255)!=m.read8(0x74666c))return 1;
  if(p.compare_status(m.read(m.read(s+0x20)+0x90),m.read(s+0x10))&0x41)return 1;
  Word data=m.read(area+0x190),before=m.read(data+index*4-4),address=data+index*4;--index;Word current=m.read(address);m.write(address-4,current);m.write(m.read(area+0x190)+index*4+4,before);
 }while(index);return 1;
}
}
