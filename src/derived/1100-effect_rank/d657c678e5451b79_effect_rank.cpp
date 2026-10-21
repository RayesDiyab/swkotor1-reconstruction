#include "529aacc58ff4bf85_effect_rank.hpp"
namespace kotor::server_mainloop {
Word effect_rank_00501649(Memory&m,EffectRankPorts&p,Word current,Word existing,Word currentOut,Word existingOut,Word strings){
 p.logarithm(p.integer(current,1));
 p.string(strings);
 Word table=m.read(m.read(m.read(0x7a3a28)+0xb8)+0xb0);
 Word row=p.integer(current,0);p.read_rank(table,row,strings,currentOut);p.destroy(strings);
 p.string(strings+8);
 table=m.read(m.read(m.read(0x7a3a28)+0xb8)+0xb0);
 row=p.integer(existing,0);p.read_rank(table,row,strings+8,existingOut);p.destroy(strings+8);
 return static_cast<int>(m.read(currentOut))>static_cast<int>(m.read(existingOut));
}
}
