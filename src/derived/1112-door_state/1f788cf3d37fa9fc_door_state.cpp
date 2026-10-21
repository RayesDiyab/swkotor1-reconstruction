#include "4c8bc0de61a21ac2_door_state.hpp"
namespace recovered_door_state{void set(U self,U value,U delayed,Ports&p){U state=p.read(self+0x308)?value&255:3;p.byte(self+0x2cd,state);if(!delayed)p.byte(self+0x2cc,state);if(state<=3){const U values[]={0x2726,0x2742,0x2743,0x2758};p.animation(self,values[state]);}}}
