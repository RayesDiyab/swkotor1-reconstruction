#pragma once
#include "c43966139d-loading_lookup.hpp"
namespace recovered_module_query {
using String=recovered_loading_lookup::String;using Ref=recovered_loading_lookup::Ref;
struct Ports:recovered_loading_lookup::Ports{
virtual int table_named_int(void*,const String*,String*,unsigned*)=0;virtual void party_mount()=0;virtual void party_unmount()=0;virtual void directory(String*)=0;virtual void release(String*)=0;virtual unsigned live_count()=0;virtual void format_live(String*,unsigned)=0;virtual String*resolve_alias(String*,unsigned)=0;virtual void concat(String*,String*,String*)=0;virtual void assign_string(String*,String*)=0;virtual void reset_error()=0;
};
unsigned save_allowed_004b20e0(const String*,Ports&);
unsigned query_004b51a0(const String*,Ports&);
}
