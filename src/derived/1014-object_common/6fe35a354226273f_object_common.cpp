#include "7e578231e4a96403_object_common.hpp"
namespace kotor::server_mainloop {
// Scratch offsets are relative to each original frame after its initial saved registers.
void object_effects_004d1be0(ObjectCommonPorts&p,Word self,Word gff,Word root,Word s){
 if(p.list(gff,s+8,root,0x745eac)&&p.count(gff,s+8)!=0)p.effects_nonempty(self,gff,root,s);
}
void object_actions_004cecb0(ObjectCommonPorts&p,Word self,Word gff,Word root,Word s){
 if(p.list(gff,s+0xb4,root,0x745ea0)&&p.count(gff,s+0xb4)!=0)p.actions_nonempty(self,gff,root,s);
}
void object_common_004d1cf0(Memory&m,ObjectCommonPorts&p,Word self,Word gff,Word root,Word s){
 object_effects_004d1be0(p,self,gff,root,s);
 p.variables(self+0x100,gff,root);
 p.builtins(self+0x110,gff,root);
 object_actions_004cecb0(p,self,gff,root,s);
 m.write(self+0xe8,p.byte_read(gff,root,0x745f08,s+0x14,1)&255);
}
}
