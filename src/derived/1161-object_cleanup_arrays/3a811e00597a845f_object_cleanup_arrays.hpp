#pragma once
#include <cstdint>
namespace recovered_object_cleanup_arrays{using U=std::uint32_t;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual void string_destroy(U)=0;virtual void release(U)=0;virtual void records(U,U)=0;};void strings(U,Ports&);void locals(U,Ports&);}
