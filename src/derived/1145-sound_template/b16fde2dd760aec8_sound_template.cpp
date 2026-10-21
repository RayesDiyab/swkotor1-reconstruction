#include "ff5f3aeea3b7716b_sound_template.hpp"
namespace kotor::server_mainloop {
Word sound_template_005c94e0(Memory&m,SoundTemplatePorts&p,Word self,Word ref,Word rootScratch){Word storage=p.allocate(0xa0);Word gff=storage?p.construct(storage,0x7f3,0x74c25c,ref):0;if(!m.read(gff+0x94)){p.destroy(gff);return 0;}p.root(gff,rootScratch);Word result=p.fields(self,gff,rootScratch);p.destroy(gff);return result;}
}
