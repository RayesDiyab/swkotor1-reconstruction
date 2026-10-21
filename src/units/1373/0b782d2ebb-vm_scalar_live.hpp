#pragma once
#include <cstdint>
namespace recovered_vm_scalar {
using U=std::uint32_t;
struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual U byte(U)=0;virtual void write(U,U)=0;virtual void write_byte(U,U)=0;virtual U allocate(U)=0;virtual void release(U)=0;virtual void copy(U destination,U source,U bytes)=0;virtual void shrink(U stack,U depth)=0;};
// Live native adaptation of retained vm_push, vm_leaf and dialogue_float owners.
void add(Ports&,U embedded_stack,U type);
U push(Ports&,U vm,U type,U bits);
U pop(Ports&,U vm,U type,U output);
}
