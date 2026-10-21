#pragma once
#include <cstdint>
namespace recovered_placeable_constructor{using U=std::uint32_t;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual void byte(U,U)=0;virtual void half(U,U)=0;virtual void base(U,U,U,U)=0;virtual U table()=0;virtual void localized(U)=0;virtual void ref(U)=0;virtual void string(U)=0;virtual void mesh(U)=0;virtual U ai()=0;virtual void register_ai(U,U,U)=0;virtual U allocate(U)=0;virtual U inventory(U,U,U)=0;virtual U invalid()=0;virtual void empty_ref(U,U)=0;};U construct(U,U,U,Ports&);}
