#pragma once
#include <cstdint>
namespace recovered_vm_entering_commands {
using U=std::uint32_t;
struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual U read8(U)=0;virtual U object(U,U)=0;virtual U cast(U,U)=0;virtual U player(U,U)=0;virtual U push_object(U,U)=0;virtual U push_integer(U,U)=0;virtual U pop_object(U,U)=0;};
U entering(U,Ports&);
U is_pc(U scratch,Ports&);
}
