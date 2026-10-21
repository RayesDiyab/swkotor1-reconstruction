#include "5f1ba2e9f5-vm_commands_t6.hpp"
namespace kotor::vm_commands_t6 {
I32 get_solo_mode_00546af0(Memory& m,Ports& p){
 const Word party=m.read(p.server_app()+4)+0x1b770;   // CServerExoApp::GetPartyTable 004AEE70 (inlined)
 return p.push_integer(m.read(party+0x190))?0:kPushFailed;
}
}
