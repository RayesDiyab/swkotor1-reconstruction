#include "db58c87c3deeee77_script_delete.h"
namespace kotor::vm_delete {
U32 delete_script_005d44f0(const Descriptor& s,const Dependencies& d) {
 const U32 stack=s.read(s.context,0);
 if(stack!=0) {
  vm_stack_clear::clear_stack_005d2140(d.resolve_stack(d.context,stack),d.stack);
  d.free_006fa390(d.context,stack);
  s.write(s.context,0,0);
 }
 const U32 code=s.read(s.context,0x10);
 if(code!=0) {
  d.free_006fa390(d.context,code);
  s.write(s.context,0x10,0);
 }
 s.write(s.context,4,0);
 s.write(s.context,0x0c,0);
 s.write(s.context,0x14,0);
 s.write(s.context,0x24,0);
 s.write(s.context,8,13);
 return 1;
}
}
