#include "26dc53c389b79f5e_server_outer.hpp"
#include <cstring>
namespace recovered_server_create {
namespace {void* get(void* p,unsigned off){std::uint32_t x;std::memcpy(&x,static_cast<unsigned char*>(p)+off,4);return reinterpret_cast<void*>(static_cast<std::uintptr_t>(x));}void put(void*p,unsigned off,const void*v){std::uint32_t x=static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(v));std::memcpy(static_cast<unsigned char*>(p)+off,&x,4);}}
void* network_getter_004aeda0(void* self){return get(get(self,4),0x10040);}
void* construct_outer_004aec00(void* self,OuterPorts& p){
 put(self,0,p.vtable());void* storage=p.allocate(0x1b944);
 void* internal=storage?p.internal_construct_004b55e0(storage):nullptr;
 put(self,4,internal);return self;
}
std::uint32_t create_server_00401380(void* manager,std::uint32_t options,OuterPorts& p){
 if(get(manager,8)){
  p.shutdown_005ed310(get(manager,8));p.stop_004ae900(get(manager,8));
  void* old=get(manager,8);if(old){p.destroy_004aec70(old);p.free(old);}
  put(manager,8,nullptr);p.cleanup_00401440();
 }
 void* storage=p.allocate(8);void* server=storage?construct_outer_004aec00(storage,p):nullptr;
 put(manager,8,server);p.set_options_004ae8f0(server,options);
 return p.initialize_004ae850(get(manager,8));
}
}
