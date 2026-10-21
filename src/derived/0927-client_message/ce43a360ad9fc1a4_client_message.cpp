#include "deb84e0ff43ac358_client_message.hpp"
namespace recovered_client_message {
unsigned handle_005ee700(void*self,unsigned id,unsigned char*message,unsigned length,unsigned ignore,Ports&p){
 if(p.logging(p.client_app())&&p.log_file(p.client_app())){String text;p.empty(&text);const auto millis=p.ticks(p.timer(p.client_app()))/1000u;p.header(&text,static_cast<unsigned>(millis),0x4d,id,ignore,length);p.write_string(p.log_file(p.client_app()),&text);p.write_bytes(p.log_file(p.client_app()),message,length,1);p.newline(&text);p.write_string(p.log_file(p.client_app()),&text);p.destroy(&text);}
 if(ignore==0){if(message[0]==0x50)p.player_message(p.pointer(self,0x14c),message,length);if(message[0]==0x53)p.system_message(p.pointer(self,0x1c0),message,length);}return 1;
}
}
