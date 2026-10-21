#include "8c5a390f6f3039f4_vm_script_trace.hpp"
namespace recovered_vm_script_trace {
void append(Ports&p,Word ai,Word script,Word s){p.comma(s);const Word first=p.concat(ai+0x60,s+16,script);const Word second=p.concat(first,s+8,s);p.assign(ai+0x60,second);p.destroy(s+8);p.destroy(s+16);p.destroy(s);}
void command(Ports&p,Word script,Word s){const Word app=p.read(0x7a39fc);const Word server=p.read(app+8);const Word master=p.ai(server);if(p.not_empty(script))append(p,master,script,s);}
}
