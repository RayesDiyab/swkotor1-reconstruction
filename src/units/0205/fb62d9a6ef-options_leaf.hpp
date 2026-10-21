#pragma once
#include <cstdint>
namespace recovered_options_leaf {
struct Ports {virtual ~Ports()=default;virtual void* allocate(unsigned)=0;};
void* construct_00520980(void*,void*(*)(void*),const void*);
void* construct_0052b8e0(void*,Ports&);
}
