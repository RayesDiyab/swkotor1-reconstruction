#pragma once
#include "9082b031cccb6149_vm_scalar_live.hpp"
namespace recovered_vm_copy {
using U=recovered_vm_scalar::U;
struct Ports:recovered_vm_scalar::Ports{virtual void destroy_string(U)=0;virtual U construct_string(U)=0;virtual void assign_string(U,U)=0;virtual void destroy_engine(U function,U implementer,U type,U value)=0;virtual U copy_engine(U function,U implementer,U type,U value)=0;};
void copy(Ports&,U stack,U source,U destination);
}
