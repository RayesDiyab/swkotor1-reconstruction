#include "eb023f732e15a15d_module_mount.hpp"
namespace kotor::server_mainloop {
void party_mount_005638d0(Memory&m,ModuleMountPorts&p,Word self,Word scratch){if(!m.read(self+0xf4)){p.literal_005e5a90(scratch,0x74514c);p.directory_00408800(m.read(0x7a39e8),scratch);p.destroy_005e5c20(scratch);}m.write(self+0xf4,m.read(self+0xf4)+1);}
void party_unmount_00563950(Memory&m,ModuleMountPorts&p,Word self,Word scratch){Word count=m.read(self+0xf4)-1;m.write(self+0xf4,count);if(!count){p.literal_005e5a90(scratch,0x74514c);p.release_004088d0(m.read(0x7a39e8),scratch);p.destroy_005e5c20(scratch);}}
Word resource_scope_00408c00(Memory&m,ModuleMountPorts&p,Word self,Word ref,Word type,Word scope,Word scratch){if(scope-1>3)return 0;Word table=self+0x10+4*(scope-1);if(!table)return 0;m.write(scratch,type);m.write(scratch+4,scope);return p.lookup_004071a0(self,table,ref,type,scratch,scratch+4,0);}
}
