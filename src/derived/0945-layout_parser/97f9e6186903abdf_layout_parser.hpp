#pragma once
#include <cstdint>
namespace recovered_layout {
using U=std::uint32_t;
struct Ports {
 virtual ~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;
 virtual void ref_from_string(U,U)=0;virtual void set_reference(U,U,U)=0;virtual U demand(U)=0;
 virtual U allocate(U)=0;virtual void zero(U,U)=0;virtual void copy(U,U,U)=0;virtual void free(U)=0;
 virtual void scan_line(U,U)=0;virtual U length(U)=0;virtual bool literal_equal(U,const char*,U)=0;
 virtual void scan_count(U,U)=0;virtual void ref_empty(U)=0;virtual void vector_empty(U)=0;virtual void string_empty(U)=0;
 virtual void scan_room(U,U,U)=0;virtual void ref_assign(U,U,U)=0;
 virtual void scan_door(U,U,U,U,U,U)=0;virtual void string_literal(U,U)=0;
};
U parse(U self,U name,U scratch,Ports&);
}
