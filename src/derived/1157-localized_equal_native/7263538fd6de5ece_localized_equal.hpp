#pragma once
#include <cstdint>
namespace recovered_localized_equal{using U=std::uint32_t;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual U allocate(U)=0;virtual U string_construct(U)=0;virtual void string_destroy(U)=0;virtual void release(U)=0;virtual void assign(U,U)=0;virtual U different(U,U)=0;virtual U next(U,U)=0;virtual U raw(U,U,U)=0;virtual void ref_construct(U)=0;virtual U talk()=0;virtual U fetch(U,U,U,U)=0;};U uncached(U,U,U,U,U,Ports&);U internal_equal(U,U,U,Ports&);U equal(U,U,U,Ports&);U different(U,U,U,Ports&);}
