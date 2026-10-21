#include "7f5c11fd6fcf3759_room_base.hpp"
namespace recovered_room_base {
U construct(U s,Ports&p){p.write(s,p.table());p.write(s+4,0);p.write(s+8,0);p.write(s+12,0);p.write(s+28,0);p.write(s+24,0);p.write(s+20,0);p.write(s+16,0x3f800000);p.empty_ref(s+32);p.write(s+48,0);p.write(s+56,0);p.write(s+52,0x3f800000);return s;}
void assign(U s,U ref,U vector,U scratch,Ports&p){p.write(s+56,p.special_ref(ref));p.assign_ref(s+32,scratch,ref);p.write(s+4,p.read(vector));p.write(s+8,p.read(vector+4));p.write(s+12,p.read(vector+8));}
U destroy(U s,U flags,Ports&p){p.write(s,p.table());if(flags&1)p.release(s);return s;}
}
