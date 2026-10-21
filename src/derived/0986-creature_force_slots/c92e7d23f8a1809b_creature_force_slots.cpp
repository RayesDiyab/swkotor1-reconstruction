#include "44585985383092d1_creature_force_slots.hpp"
#include "9ae7aaa6bd4b3aa1_creature_class_leaves.hpp"
namespace kotor::server_mainloop {
Word creature_class_slots_005bcd30(Memory&m,Word self,Word level,Word spell){level&=255;spell&=255;if(level<1||level>20||spell>=m.read8(self+level+0x113))return 255;return m.read8(self+level+0x127);}
Word creature_force_slots_005a5ee0(Memory&m,Word self,Word index,Word spell){Word cls=creature_get_class_005a4e90(m,self,index)&255;Word rules=m.read(0x7a3a28),record=m.read(rules+0xac)+cls*0x1a0;if(!record)return 0;int ability=creature_force_class_005be4a0(cls)?static_cast<signed char>(m.read8(self+0xf4)):0;Word bonus=0,rank=spell&255;if(rank&&ability>0&&ability>=static_cast<int>(rank))bonus=static_cast<Word>((ability-static_cast<int>(rank)+4)/4);Word level=(index&255)<m.read8(self+0x89)?m.read8(self+(index&255)*40+0xa8):0;Word result=creature_class_slots_005bcd30(m,record,level,spell)&255;return result==255?0:(result+bonus)&255;}
}
