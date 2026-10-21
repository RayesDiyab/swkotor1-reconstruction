#include "53402ec549855d25_creature_stats_saves.hpp"
namespace kotor::server_mainloop {
Word creature_stats_saves_005b0cdb(Memory&m,CreatureStatsSavePorts&p,Word self,Word gff,Word root,Word out,Word s){
 const Word fields[]={0x1a1,0x1a0,0x1a2},labels[]={0x74ae04,0x74adf8,0x74adec};
 Word fallback=static_cast<Word>(static_cast<short>(static_cast<signed char>(m.read8(self+fields[0]))));
 for(unsigned i=0;i<3;++i){Word v=p.short_read(gff,root,labels[i],s+0x10,fallback);if(i<2)fallback=static_cast<Word>(static_cast<short>(static_cast<signed char>(m.read8(self+fields[i+1]))));p.byte_write(self+fields[i],v);}
 if(m.read(self+0x6c))p.level_history_005b0d49(self,gff,root,s);
 return p.continuation_005b1254(self,gff,root,out,s);
}
}
