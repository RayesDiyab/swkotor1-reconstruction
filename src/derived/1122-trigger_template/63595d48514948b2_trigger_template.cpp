#include "7e848cbd52f62673_trigger_template.hpp"
namespace kotor::server_mainloop {
Word trigger_template_0058ed70(Memory&m,TriggerTemplatePorts&p,Word self,Word ref,Word rootScratch){Word storage=p.allocate(0xa0);Word gff=storage?p.construct(storage,0x7f0,0x749b18,ref):0;if(!m.read(gff+0x94)){p.destroy(gff);return 0;}p.root(gff,rootScratch);Word result=p.fields(self,gff,rootScratch);p.destroy(gff);return result;}
}
