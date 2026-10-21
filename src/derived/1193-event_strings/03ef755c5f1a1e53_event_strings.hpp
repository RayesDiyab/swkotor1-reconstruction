#pragma once
#include <cstdint>
namespace recovered_event_strings {
using U=std::uint32_t;
struct Ports {virtual ~Ports()=default;virtual U read(U)=0;virtual void copy(U,U)=0;virtual void empty(U)=0;virtual void resize(U,U)=0;virtual void assign(U,U)=0;};
U get(U self,U out,U index,Ports&);
void set(U self,U index,U source,Ports&);
}
