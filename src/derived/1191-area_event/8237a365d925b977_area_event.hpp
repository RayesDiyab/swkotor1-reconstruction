#pragma once
#include <cstdint>
namespace recovered_area_event {
using U=std::uint32_t;
struct Ports {virtual~Ports()=default;virtual U read(U)=0;virtual U read16(U)=0;virtual U read8(U)=0;virtual void write(U,U)=0;virtual void byte(U,U)=0;virtual void saved(U,U,U,U)=0;virtual void script(U,U,U,U)=0;virtual U objects(U)=0;virtual U lookup(U,U,U)=0;virtual U placeable(U)=0;virtual void place(U,U,U,U)=0;virtual void bounds(U,U,U)=0;virtual void position_update(U,U,U,U,U,U)=0;virtual U effect_real(U,U)=0;virtual void effect(U,U,U,U)=0;virtual U integer(U,U)=0;virtual U mode(U)=0;virtual void set_mode(U,U)=0;virtual void set_transition(U,U)=0;virtual U string(U,U,U)=0;virtual void set_module(U,U)=0;virtual void set_waypoint(U,U)=0;virtual void movie(U,U)=0;virtual void string_destroy(U)=0;virtual U allocate(U)=0;virtual U construct(U)=0;virtual U next(U)=0;virtual void integer_set(U,U,U)=0;virtual void string_set(U,U,U)=0;virtual U ai(U)=0;virtual void enqueue(U,U,U,U,U,U,U)=0;virtual U gui(U)=0;virtual void fade(U,U,U,U,U)=0;virtual void input(U,U,U)=0;virtual void destroy(U)=0;virtual void release(U)=0;};
void dispatch(U self,U kind,U source,U data,U day,U tick,U scratch,Ports&);
}
