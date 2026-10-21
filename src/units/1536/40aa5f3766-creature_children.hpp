#pragma once
#include <cstdint>
namespace recovered_creature_children{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual U byte(U)=0;virtual void write(U,U)=0;virtual void put_byte(U,U)=0;virtual U table()=0;virtual U allocate(U)=0;virtual void grow(U,U)=0;virtual void release(U)=0;};U equipment(U,Ports&);U equipment_delete(U,U,Ports&);U inventory(U,U,U,Ports&);U contains(U,U,Ports&);void perception(U,U,U,U,U,U,Ports&);}
