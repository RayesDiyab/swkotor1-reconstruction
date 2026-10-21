#include "262d6ba7d99e4373_server_copy.hpp"
namespace kotor::server_mainloop {
Word copy_module_004b1680(CopyPorts& p,Word source,Word destination){
 const Word to=p.cstr_005e5670(destination),from=p.cstr_005e5670(source);
 if(p.copy_file(from,to,0)==0)return 0;
 if(p.attributes(p.cstr_005e5670(destination))&1)
  (void)p.set_attributes(p.cstr_005e5670(destination),0x80);
 return 1;
}
}
