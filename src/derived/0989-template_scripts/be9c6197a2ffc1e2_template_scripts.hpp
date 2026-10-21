#pragma once
#include <cstdint>
namespace recovered_template_scripts{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual void empty(U)=0;virtual void ref_string(U,U)=0;virtual U read_ref(U,U,U,const char*,U,U)=0;virtual void to_string(U,U)=0;virtual void assign(U,U)=0;virtual void destroy(U)=0;};void load(U,U,U,U,Ports&);}
