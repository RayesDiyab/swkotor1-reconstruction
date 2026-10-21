#pragma once
#include <cstdint>
namespace recovered_mesh_resource {using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual void base_construct(U)=0;virtual void base_destroy(U)=0;virtual U table()=0;virtual bool header(U)=0;virtual void release(U)=0;};U construct(U,Ports&);U service(U,Ports&);U freed(U,Ports&);U binary(U,Ports&);U destroy(U,U,Ports&);}
