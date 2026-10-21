#pragma once
#include <cstdint>
namespace recovered_ai_event_cleanup{
using U=std::uint32_t;
struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual void destroy(U address,U payload)=0;virtual void release(U)=0;virtual void script_release(U vm,U payload)=0;virtual void strings(U data,U count)=0;};
void cleanup(U kind,U payload,Ports&);
void event_destroy(U self,Ports&);
}
