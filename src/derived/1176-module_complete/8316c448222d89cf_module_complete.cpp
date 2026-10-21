#include "5662bd95d120fd19_module_complete.hpp"
namespace kotor::server_mainloop {
Word module_complete_004c5880(Memory&m,ModuleCompletePorts&p,Word self,Word scratch){
 p.root(m.read(self+8),scratch);
 Word loading=m.read(m.read(0x7a39fc)+0x14),count=m.read(loading+0xc);m.write(loading+8,count);
 Word server=m.read(m.read(0x7a39fc)+8);p.date(p.messages(server),count,0xffffffff);
 Word allocation=p.allocate(0x34),event=allocation?p.event(allocation):0;m.write16(event,0x11);
 Word kind=m.read(0x7441dc),id=m.read(self+0x20);server=m.read(m.read(0x7a39fc)+8);
 p.enqueue(p.ai(server),0,0,id,id,kind,event);
 p.release(m.read(self+8));m.write(self+0x1d8,1);
 if(m.read(0x832904))p.debug(0x7458d8);
 return 0;
}
}
