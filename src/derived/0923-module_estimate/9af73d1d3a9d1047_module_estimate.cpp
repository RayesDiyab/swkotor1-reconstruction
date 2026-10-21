#include "38d7531e77702d7e_module_estimate.hpp"
#include <cstring>
namespace recovered_module_estimate {
void estimate_004b5f90(void*self,const String*,unsigned type,Ports&p){
 Array entries{};String unused;p.empty(&unused);unsigned total=0;String directory;p.literal(&directory,"GAMEINPROGRESS:");p.enumerate(&entries,&directory,0xffff,0,0);p.destroy(&directory);const unsigned count=entries.count;
 if(count==0){unsigned size=0x393870;std::memcpy(static_cast<char*>(self)+0x100c8,&size,4);}else{
  String name,path,extension;p.empty(&name);p.empty(&path);p.empty(&extension);
  for(unsigned i=0;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(count);++i){
   p.assign(&name,p.element(&entries,i));String stem;String*left=p.left(&name,&stem,p.length(&name)-4);String prefix;p.literal(&prefix,"GAMEINPROGRESS:");String joined;p.assign(&path,p.concat(&prefix,&joined,left));p.destroy(&joined);p.destroy(&prefix);p.destroy(&stem);
   String suffix;p.assign(&extension,p.right(&name,&suffix,3));p.destroy(&suffix);String mode;p.literal(&mode,"rb");void*file;unsigned resourceType=p.extension_type(&extension);p.file_construct(&file,&path,resourceType,&mode);p.destroy(&mode);if(!p.file_opened(&file))total+=0x2625a0;else total+=p.file_size(&file);p.file_destroy(&file);
  }
  if(static_cast<std::uint16_t>(type)!=0xbc1&&static_cast<std::uint16_t>(type)!=0x809)total+=0x2625a0;
  const unsigned ten=total*10u;const unsigned size=static_cast<unsigned>((static_cast<std::uint64_t>(ten)*0x38e38e39u)>>33);std::memcpy(static_cast<char*>(self)+0x100c8,&size,4);p.destroy(&extension);p.destroy(&path);p.destroy(&name);
 }
 p.destroy(&unused);if(entries.data)p.destroy_array(&entries);
}
}
