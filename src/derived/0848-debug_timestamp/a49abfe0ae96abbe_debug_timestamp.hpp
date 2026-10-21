#pragma once
#include <cstdint>
namespace recovered_debug_timestamp {
struct Timeb32 {std::int32_t time;std::uint16_t millitm;std::int16_t timezone,dstflag;};
struct Ports {virtual ~Ports()=default;virtual void ftime32(Timeb32*)=0;virtual const char* ctime32(const std::int32_t*)=0;virtual void render19(char*,const char*)=0;virtual void format(void*,const char*)=0;};
void stamp_internal_005ec560(void*,void*,Ports&);
void stamp_005e9410(void*,void*,Ports&);
}
