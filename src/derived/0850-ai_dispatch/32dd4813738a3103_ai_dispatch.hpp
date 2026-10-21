#pragma once
#include <cstdint>
namespace recovered_ai_dispatch {
struct Ports {virtual ~Ports()=default;virtual void* allocate(unsigned)=0;virtual const void* resolve(unsigned)=0;};
void initialize_004e4a10(void*,Ports&);
void initialize_004ea9f0(void*,Ports&);
}
