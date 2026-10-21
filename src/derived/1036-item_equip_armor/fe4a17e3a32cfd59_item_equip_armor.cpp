#include "41dc684441f7f4d1_item_equip_armor.hpp"
namespace recovered_item_equip_armor{void update(U self,Ports&p){U item=p.equipment(p.read(self+0xa2c),2);if(item){U rule=p.rule(item);if(rule){U stats=p.read(self+0xa74);U value=p.byte(rule+0x8a);p.write_byte(stats+0xf6,value);return;}}p.write_byte(p.read(self+0xa74)+0xf6,0);}}
