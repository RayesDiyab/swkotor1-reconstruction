#pragma once
#include <cstdint>
namespace recovered_door_placement {
using U=std::uint32_t;
struct Vector {U x,y,z;};
struct Ports {
 virtual ~Ports()=default;
 virtual U read(U)=0; virtual void write(U,U)=0;
 virtual U area(U)=0; virtual void detach(U,U)=0; virtual void erase_id(U,U)=0; virtual void remove_bounds(U,U)=0;
 virtual void set_area(U,U)=0; virtual void position(U,U,U,U,U)=0;
 virtual void insert(U,U,U)=0; virtual void grow(U,U)=0;
 virtual U bounds(U,Vector,Vector,U,U)=0; virtual void add_bounds(U,U,U,U)=0;
 virtual void geometry(U,Vector,Vector,U,U,U,U)=0;
};
void remove(U,U,Ports&);
void place(U,U,U,U,U,U,U,Ports&);
}
