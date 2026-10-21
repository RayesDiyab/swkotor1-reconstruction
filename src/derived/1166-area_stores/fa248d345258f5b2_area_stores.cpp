#include "43243fe7daa44a71_area_stores.hpp"
namespace kotor::server_mainloop {
Word area_stores_005057a0(Memory&m,AreaStorePorts&p,Word area,Word gff,Word root,Word saved,Word templates,Word s){
 m.write(s+0x10,area);
 if(!p.list(gff,s+0x58,root,0x747510))return 1;
 m.write(s+0xc,0);
 if(!p.count(gff,s+0x58))return 1;
 return p.nonempty_005057fa(m.read(s+0x10),gff,root,saved,templates,s);
}
}
