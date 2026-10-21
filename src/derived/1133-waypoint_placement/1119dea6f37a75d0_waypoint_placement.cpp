#include "90b00b7e047c6648_waypoint_placement.hpp"
namespace recovered_waypoint_placement{
void detach(U s,U q,Ports&p){if(p.area(s)){U id=p.read(s+4);U a=p.area(s);p.remove(a,id);}p.set_area(s,0);p.write(q,0);p.write(q+4,0);p.write(q+8,0);p.position(s,q,1,1,0);}
void place(U s,U area,U x,U y,U z,U flag,U q,Ports&p){detach(s,q,p);if(!area)return;p.string_zero(q);p.set_area(s,area);p.write(q+8,x);p.write(q+12,y);p.write(q+16,z);p.position(s,q+8,0,1,0);U id=p.read(s+4);U live=p.area(s);p.insert(live,id,flag);p.string_destroy(q);}
}
