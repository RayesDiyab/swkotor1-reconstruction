#include "f021256dde66ef8b_area_effect_objects.hpp"
namespace kotor::server_mainloop {
Word area_effect_objects_00505af0(Memory&m,AreaEffectObjectPorts&p,Word area,Word gff,Word root,Word saved,Word templates,Word s){
 m.write(s+0x10,area);
 if(!p.list(gff,s+0x38,root,0x74751c))return 1;
 m.write(s+0xc,0);
 if(!p.count(gff,s+0x38))return 1;
 return p.nonempty_00505b4a(m.read(s+0x10),gff,root,saved,templates,s);
}
}
