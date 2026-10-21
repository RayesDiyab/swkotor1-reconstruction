#pragma once
#include <cstdint>
namespace recovered_room_base {
using U=std::uint32_t;
struct Ports {virtual ~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual U table()=0;virtual void empty_ref(U)=0;virtual U special_ref(U)=0;virtual void assign_ref(U,U,U)=0;virtual void release(U)=0;};
U construct(U,Ports&);void assign(U,U,U,U,Ports&);U destroy(U,U,Ports&);
}
