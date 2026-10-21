#pragma once
#include <cstdint>
namespace recovered_vm_construct {
struct Ports {virtual ~Ports()=default;virtual void* allocate(unsigned)=0;virtual const void* base_table()=0;virtual const void* derived_table()=0;virtual void reference_default(void*)=0;virtual void reference_empty(void*,void*)=0;virtual void string_default(void*)=0;virtual void string_empty(void*)=0;virtual void initialize_implementer(void*)=0;};
void* frame_005d1740(void*,Ports&);
void* internal_005d4900(void*,Ports&);
void* outer_005d0f40(void*,Ports&);
void set_implementer_005d1a40(void*,void*,Ports&);
void outer_set_005d0ff0(void*,void*,Ports&);
}
