#pragma once
#include "9082b031cccb6149_vm_scalar_live.hpp"
namespace recovered_vm_string {
using U=recovered_vm_scalar::U;
struct Ports:recovered_vm_scalar::Ports{virtual U chars(U)=0;virtual U construct(U,U)=0;virtual void assign(U,U)=0;};
U push(Ports&,U internal,U source);
U pop(Ports&,U internal,U destination);
}
