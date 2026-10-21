#include "132ca1df0c439a40_module_start.hpp"
namespace kotor::server_mainloop {
ModulePrefixResult module_start_prefix_004c9050(Memory& m,ModuleStartPorts& p,Word self,Word name,Word scratch){
 auto s=[scratch](Word o){return scratch+o;};
 p.empty_005b3190(s(0x40));p.empty_005b3190(s(0x64));
 p.manager_begin_004070a0(m.read(0x7a39e8));
 Word objects=p.objects_004aed70(m.read(m.read(0x7a39fc)+8));m.write(s(0x5c),objects);
 Word current=p.literal_005e5a90(s(0x38),0x73d8f8);
 Word joined=p.concat_005e5d10(current,s(0x20),name);p.assign_005e5c50(s(0x40),joined);
 p.destroy_005e5c20(s(0x20));p.destroy_005e5c20(s(0x38));
 Word path=self+0x5c;
 if(p.compare_005e5310(path,s(0x40))){
  if(static_cast<std::int32_t>(p.length_005e5790(path))>0)p.release_path_004c44d0(self,path);
  p.assign_005e5c50(path,s(0x40));p.setup_004c4150(self,s(0x40));
 }
 if(!p.resource_ready_00409b10(m.read(self+8))){p.destroy_005e5c20(s(0x64));p.destroy_005e5c20(s(0x40));return ModulePrefixResult::OriginalReturn1;}
 return ModulePrefixResult::ContinueAt004c919b;
}
}
