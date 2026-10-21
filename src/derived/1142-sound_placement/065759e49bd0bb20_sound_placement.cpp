#include "3d1348bad257694b_sound_placement.hpp"
namespace recovered_sound_placement{void place(U s,U area,U flag,Ports&p){if(p.area(s)){U id=p.read(s+4);U old=p.area(s);p.remove(old,id);}p.set_area(s,0);if(!area)return;p.set_area(s,area);p.insert(area,p.read(s+4),flag);U id=p.read(s+4);U server=p.server();p.tag(p.module(server),s+0x18,id);}}
