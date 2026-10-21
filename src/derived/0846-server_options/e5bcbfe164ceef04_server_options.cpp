#include "8e991176ea2afe70_server_options.hpp"
#include <cstring>
namespace recovered_server_options {
namespace {
void* get(void*p,unsigned off){std::uint32_t x;std::memcpy(&x,static_cast<unsigned char*>(p)+off,4);return reinterpret_cast<void*>(static_cast<std::uintptr_t>(x));}
void word(void*p,unsigned off,std::uint32_t x){std::memcpy(static_cast<unsigned char*>(p)+off,&x,4);}
void put(void*p,unsigned off,const void*x){word(p,off,static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(x)));}
template<class F> void* build(Ports&p,unsigned size,F f){void*s=p.allocate(size);return s?f(s):nullptr;}
}
void set_options_004b63e0(void*self,unsigned options,Ports&p){
 // The original RET 4 consumes options but its ordinary body never reads it.
 (void)options;String name;p.string_default(&name);p.debug_copy_name(&name);p.string_data(&name);
 put(self,0x10054,build(p,0x28,[&](void*s){return p.construct_0052b8e0(s);}));
 put(self,0x10048,build(p,0x44,[&](void*s){return p.construct_004ae4c0(s);}));
 put(self,0x1004c,build(p,0x44,[&](void*s){return p.construct_004ae4c0(s);}));
 put(self,0x10050,build(p,0x44,[&](void*s){return p.construct_004ae4c0(s);}));
 put(self,0x1005c,build(p,0x18,[&](void*s){return p.construct_004d7d80(s,1);}));
 if(p.mode()==2)p.publish_resource_manager(build(p,0x60,[&](void*s){return p.construct_00409bf0(s);}));
 put(self,0x10040,build(p,0x2007c,[&](void*s){return p.construct_005d54a0(s);}));
 if(!get(self,0x10004))put(self,0x10004,build(p,0x42c,[&](void*s){return p.construct_0054eb10(s);}));
 put(self,0x10010,build(p,0x50,[&](void*s){return p.construct_00520980(s);}));
 void* ai=build(p,0x90,[&](void*s){return p.construct_004b0780(s);});put(self,0x10044,ai);p.set_app_00604790(ai,self);
 if(p.mode()==2){
  String s;p.string_literal(&s,"OVERRIDE:");p.directory_00408800(p.resource_manager(),&s);p.string_destroy(&s);
  p.string_literal(&s,"ERRORTEX:");p.directory_00408800(p.resource_manager(),&s);p.string_destroy(&s);
  p.string_literal(&s,"HD0:chitin");p.fixed_004087e0(p.resource_manager(),&s);p.string_destroy(&s);
  p.string_literal(&s,"SERVERVAULT:");p.directory_00408800(p.resource_manager(),&s);p.string_destroy(&s);
 }
 String s;p.string_literal(&s,"HD0:TEMP");p.remove_file_00406b20(p.resource_manager(),&s,0x56);p.string_destroy(&s);
 p.string_literal(&s,"TEMP:");p.create_directory_004068c0(p.resource_manager(),&s);p.string_destroy(&s);
 p.string_literal(&s,"GAMEINPROGRESS:");p.create_directory_004068c0(p.resource_manager(),&s);p.string_destroy(&s);
 if(!p.talk_table()){
  p.publish_talk_table(build(p,0x58,[&](void*x){return p.construct_0041d8d0(x);}));
  p.string_literal(&s,"HD0:dialog");p.open_talk_0041e5a0(p.talk_table(),&s);p.string_destroy(&s);
 }
 p.publish_event_manager(build(p,4,[&](void*x){return p.construct_005d0f40(x);}));
 void* event=p.allocate(0x10);if(event)put(event,0,p.event_vtable_007450e4());put(self,0x1000c,event);
 p.register_event_005d0ff0(p.event_manager(),event);
 if(p.mode()!=2){String log;p.string_literal(&log,"swserver");p.open_logs_005e93f0(&log);p.string_destroy(&log);}
 word(self,0x10038,0x0aba9500);word(self,0x1003c,0);p.initialize_net_005d5550(get(self,0x10040),p.application_server());p.initialize_004b1b00(self);
 if(p.mode()==2)p.publish_007a3a28(build(p,0xdc,[&](void*x){return p.construct_0057a1f0(x);}));else p.initialize_004b1c40(self);
 word(self,0x100e4,0);word(self,0x100e8,0);word(self,0x100dc,0);word(self,0x100e0,0);
 word(self,0x100cc,static_cast<std::uint32_t>(p.high_resolution_005e9260()/1000));
 word(self,0x100d0,0);word(self,0x100d4,0);word(self,0x1b930,0);word(self,0x1b938,0);p.string_destroy(&name);
}
}
