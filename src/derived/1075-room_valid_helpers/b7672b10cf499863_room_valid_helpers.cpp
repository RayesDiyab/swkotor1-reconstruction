#include "1b37688a8a98dc67_room_valid_helpers.hpp"
namespace kotor::server_mainloop {
Word room_sweep_00579680(Memory&m,RoomValidHelperPorts&p,Word room,Word from,Word to,Word low,Word high,Word radius,Word hit){Word mesh=m.read(room+0x3c);Word r=m.read(radius),h=m.read(high),l=m.read(low);return p.collision(mesh,from,to,l,h,r,hit);}
Word room_adjacent_00579420(Memory&m,Word room,Word edge){Word mesh=m.read(room+0x3c),array=m.read(mesh+0x8c);return m.read(array+edge*8+4);}
}
