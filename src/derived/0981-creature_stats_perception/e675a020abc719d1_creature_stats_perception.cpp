#include "03f8e22407416ca3_creature_stats_perception.hpp"
namespace kotor::server_mainloop {
Word creature_stats_perception_005b06b8(Memory&m,CreatureStatsPerceptionPorts&p,Word self,Word gff,Word root,Word out,Word s){
 auto table=[&](Word offset){return m.read(m.read(m.read(0x7a3a28)+0xb8)+offset);};
 auto failure=[&](){p.loc_destroy(s+0x4c);return Word(0x5f5);};
 if(m.read(self+0x6c)){
  p.string_literal(s+0x28,0x746f28);(void)p.table_float(table(0xc4),12,s+0x28,s+0x20);p.string_destroy(s+0x28);m.write(m.read(self+0x24)+0x914,m.read(s+0x20));
  p.string_literal(s+0x28,0x746f38);(void)p.table_float(table(0xc4),12,s+0x28,s+0x20);p.string_destroy(s+0x28);m.write(m.read(self+0x24)+0x918,m.read(s+0x20));
 }else{
  Word range=p.byte_read(gff,root,0x74ae20,s+0x10,11)&255;m.write(s+0x14,range);if(range==12)return failure();
  if(range==11){p.string_literal(s+0x28,0x74ae10);Word ok=p.table_integer(table(0x0c),m.read16(self+0x186),s+0x28,s+0x14);p.string_destroy(s+0x28);if(!ok)return failure();}
  p.string_literal(s+0x28,0x746f28);Word ok=p.table_float(table(0xc4),range==11?m.read(s+0x14):range,s+0x28,s+0x18);p.string_destroy(s+0x28);if(!ok)return failure();m.write(m.read(self+0x24)+0x914,m.read(s+0x18));
  p.string_literal(s+0x28,0x746f38);ok=p.table_float(table(0xc4),m.read(s+0x14),s+0x28,s+0x18);p.string_destroy(s+0x28);if(!ok)return failure();m.write(m.read(self+0x24)+0x918,m.read(s+0x18));
 }
 return p.continuation_005b098b(self,gff,root,out,s);
}
}
