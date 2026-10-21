#include "89d9117e4174a030_area_room_storage.hpp"
namespace kotor::server_mainloop {
// Ordinary array-cookie lifecycle; original exception unwinding remains separate.
void area_rooms_allocate_00506910(Memory&m,AreaRoomStoragePorts&p,Word self,Word count){
 Word strings=m.read(self+0x25c);
 if(strings){Word cookie=strings-4,n=m.read(cookie);while(n){--n;p.destroy_string(strings+n*8);}p.free(cookie);m.write(self+0x25c,0);}
 Word values=m.read(self+0x260);if(values){p.free(values);m.write(self+0x260,0);}
 values=m.read(self+0x264);if(values){p.free(values);m.write(self+0x264,0);}
 if(count){Word block=p.allocate(count*8+4),data=0;if(block){data=block+4;m.write(block,count);for(Word i=0;i<count;++i)p.construct_string(data+i*8);}m.write(self+0x25c,data);
  m.write(self+0x260,p.allocate(count*4));m.write(self+0x264,p.allocate(count*4));}
}
}
