// Adapted from retained condition_branch::run_script_005d45d0. Every VM access
// targets the canonical native carrier directly; no host Internal mirror exists.
#include "a761402505f6f88b_vm_run_live.hpp"
namespace kotor::server_mainloop{
Word vm_run_live_005d45d0(Memory&m,VmRunLivePorts&p,Word self,Word script,Word context,Word flag){
 if(!script||p.empty(script)==1)return 0;
 if(m.read(self+0x28)==0xffffffff){p.clear(self+0x1ac);m.write(self+0x1c0,self);m.write(self+0x24,0);m.write(self+0x1c4,0);}
 if(p.load(self,script)!=0){if(m.read(self+0x28)!=0xffffffff)p.cstr(script);return 0;}
 m.write(self+40*(m.read(self+0x28)+2),0);
 m.write(self+0x16c+4*m.read(self+0x28),flag);m.write(self+0x18c+4*m.read(self+0x28),context);
 Word implementer=m.read(self+0x3d4);
 if(implementer){p.script_slot(implementer,script);Word f=m.read(self+0x16c+4*m.read(self+0x28));implementer=m.read(self+0x3d4);m.write(implementer+4,f);Word index=m.read(self+0x28);implementer=m.read(self+0x3d4);Word c=m.read(self+0x18c+4*index);m.write(implementer+8,c);}
 Word saved=m.read(self+0x1ac);Word status=p.execute(self,0);
 Word descriptor=self+0x2c+40*m.read(self+0x28);p.delete_script(self,descriptor);
 Word index=m.read(self+0x28)-1;m.write(self+0x28,index);
 if(index!=0xffffffff){implementer=m.read(self+0x3d4);if(implementer){Word f=m.read(self+0x16c+4*index);m.write(implementer+4,f);Word outer=m.read(self+0x28);implementer=m.read(self+0x3d4);Word c=m.read(self+0x18c+4*outer);m.write(implementer+8,c);}}
 Word live=m.read(self+0x28);if(status){if(live!=0xffffffff)p.cstr(script);return 0;}
 if(live!=0xffffffff)p.cstr(script);
 Word depth=m.read(self+0x1ac);
 if(depth==saved+1){Word tags=m.read(self+0x1b8);if(m.read8(tags+saved)==3){Word words=m.read(self+0x1bc);m.write(self+0x1c,3);Word value=m.read(words+saved*4);m.write(self+0x20,value);}p.cleanup(self+0x1ac,saved);}
 else if(depth!=saved){p.cleanup(self+0x1ac,saved);if(m.read(self+0x28)==0xffffffff)p.clear(self+0x1ac);return 0;}
 if(m.read(self+0x28)==0xffffffff){p.clear(self+0x1ac);}return 1;
}
}
