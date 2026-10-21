#pragma once
#include <cstdint>
namespace recovered_vm_commands {
struct Ports {virtual ~Ports()=default;virtual void* allocate(unsigned)=0;virtual const void* resolve(unsigned)=0;};
void initialize_0054c960(void*,Ports&);
}
