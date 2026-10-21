#include "56b5abcb0e72bf52_area_encounters.hpp"
namespace kotor::server_mainloop {
Word area_encounters_00505060(Memory&m,AreaEncounterPorts&p,Word area,Word gff,Word root,Word saved,Word templates,Word s){
 m.write(s+0x10,area);
 if(!p.list(gff,s+0x54,root,0x7474c8))return 1;
 m.write(s+8,0);
 if(!p.count(gff,s+0x54))return 1;
 return p.nonempty_005050ba(m.read(s+0x10),gff,root,saved,templates,s);
}
}
