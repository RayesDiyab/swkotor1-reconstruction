#include "a49abfe0ae96abbe_debug_timestamp.hpp"
#include <cstring>
namespace recovered_debug_timestamp {
void stamp_internal_005ec560(void* internal,void* output,Ports&p){(void)internal;Timeb32 now;p.ftime32(&now);char buffer[20];p.render19(buffer,p.ctime32(&now.time));p.format(output,buffer);}
void stamp_005e9410(void* debug,void* output,Ports&p){std::uint32_t value;std::memcpy(&value,(char*)debug+8,4);if(value)stamp_internal_005ec560(reinterpret_cast<void*>(static_cast<std::uintptr_t>(value)),output,p);}
}
