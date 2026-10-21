#pragma once
#include <cstdint>
namespace recovered_mesh_position{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual void shift(U self,U vertex,U translation)=0;};void base(U,U,Ports&);void tree(U,U,U,Ports&);void position(U,U,Ports&);}
