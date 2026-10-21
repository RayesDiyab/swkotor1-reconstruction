#include "11dd27c2b8-baseinternal.h"
#include <new>
namespace base_resource_table {
String* __fastcall ConstructString(String* self,void*) {
 return ::new(static_cast<void*>(self)) String;
}
void __fastcall DestroyString(String* self,void*) {self->~StartupString();}
void __stdcall ConstructStringVectorNormal(String* begin,std::uint32_t stride,std::int32_t count,
                                          Construct construct,Destroy) {
 // Genuine windows/renderer-lifetime ConstructVectorNormal normal loop.
 // Only callback/receiver transports change to begin actual canonical lifetime.
 auto address=reinterpret_cast<std::uint32_t>(begin);
 for(std::int32_t completed=0;completed<count;++completed) {
  (void)construct(reinterpret_cast<String*>(address),nullptr);
  address+=stride;
 }
}
}
