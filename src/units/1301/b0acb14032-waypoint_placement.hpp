#pragma once
#include <cstdint>
namespace recovered_waypoint_placement{using U=std::uint32_t;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual U area(U)=0;virtual void remove(U,U)=0;virtual void set_area(U,U)=0;virtual void position(U,U,U,U,U)=0;virtual void string_zero(U)=0;virtual void string_destroy(U)=0;virtual void insert(U,U,U)=0;};void detach(U,U,Ports&);void place(U,U,U,U,U,U,U,Ports&);}
