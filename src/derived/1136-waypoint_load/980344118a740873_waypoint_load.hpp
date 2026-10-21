#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct WaypointLoadPorts{virtual~WaypointLoadPorts()=default;virtual Word localized_construct(Word)=0;virtual void localized_destroy(Word)=0;virtual Word localized_read(Word,Word,Word,Word,Word,Word)=0;virtual void localized_assign(Word,Word)=0;virtual void string_empty(Word)=0;virtual Word string_read(Word,Word,Word,Word,Word,Word)=0;virtual void string_destroy(Word)=0;virtual void tag(Word,Word)=0;virtual Word floating(Word,Word,Word,Word,Word)=0;virtual void normalize(Word)=0;virtual void position(Word,Word)=0;virtual void orientation(Word,Word)=0;virtual Word byte(Word,Word,Word,Word,Word)=0;virtual Word module(Word)=0;virtual void register_tag(Word,Word,Word)=0;};
Word waypoint_load_005c7f30(Memory&,WaypointLoadPorts&,Word self,Word gff,Word root,Word scratch);
}
