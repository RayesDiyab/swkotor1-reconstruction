#include "7ec6946fd22d535e_creature_area_access.hpp"
namespace recovered_creature_area_access{void set(U self,U area,Ports&p){p.write(self+0x8c,area?p.read(area+0x120):p.invalid());}}
