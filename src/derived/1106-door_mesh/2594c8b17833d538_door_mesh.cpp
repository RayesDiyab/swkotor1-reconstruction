#include "b1c5fd22d7500fc3_door_mesh.hpp"
namespace recovered_door_mesh {
U construct(U self,Ports&p){p.base(self,0x7d2,0x804);p.write(self,p.table());return self;}
void state(U self,U value,Ports&p){p.write(self+0x88,value);U a;if(value==0)a=0x74c3cc;else if(value==1)a=0x74c3d4;else if(value==2)a=0x74c3dc;else{p.half(self+0x8c,0x20);return;}p.write(self+0x8c,p.constant(a));p.half(self+0x90,p.constant(a+4));if(value==0)p.byte(self+0x92,p.constant(a+6));}
}
