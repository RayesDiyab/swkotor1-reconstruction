#include "a065f393f3728a51_door_template.hpp"
namespace kotor::server_mainloop {
Word door_template_0058b3d0(Memory&m,DoorTemplatePorts&p,Word self,Word ref,Word root){Word allocation=p.allocate(0xa0);Word gff=allocation?p.construct_gff(allocation,0x7fa,0x7499a8,ref):0;if(!m.read(gff+0x94)){p.destroy(gff);return 0;}p.root(gff,root);Word result=p.fields(self,gff,root,1);p.destroy(gff);return result;}
}
