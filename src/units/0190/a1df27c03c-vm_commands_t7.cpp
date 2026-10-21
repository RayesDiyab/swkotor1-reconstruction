#include "19a60b5b52-vm_commands_t7.hpp"
namespace kotor::vm_commands_t7 {
I32 get_last_hostile_target_0053a900(Memory& m,Ports& p){
 Word id=kInvalid;
 if(!p.pop_object(&id))return kPopFailed;
 const Word c=p.get_creature_by_id_004ae770(p.server_app(),id);
 return p.push_object(c?m.read(c+0x53c):kInvalid)?0:kPushFailed;
}
}
