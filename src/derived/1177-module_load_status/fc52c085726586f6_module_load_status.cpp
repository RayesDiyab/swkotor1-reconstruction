#include "21db7640f4422ba5_module_load_status.hpp"
namespace kotor::server_mainloop {
Word module_load_status_0056c980(Memory&m,ModuleMessagePorts&p,Word self,Word status,Word flag,Word scratch){
 message_begin_004d68a0(m,p,self,4,0xffffffff,1);
 message_word_004d6340(m,p,self,status,32);
 message_word_004d6340(m,p,self,flag,3);
 const Word ready=message_finish_004d6440(m,self,scratch+4,scratch);
 if(!ready)return ready;
 const Word size=m.read(scratch),data=m.read(scratch+4),recipient=m.read(0x74c52c);
 return message_send_00567e60(m,p,self,recipient,0x2c,3,data,size);
}
}
