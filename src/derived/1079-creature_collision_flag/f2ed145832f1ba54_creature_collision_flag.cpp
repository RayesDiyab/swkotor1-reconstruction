#include "044578d62d6fa1d7_creature_collision_flag.hpp"
namespace recovered_creature_collision_flag{
U query(U self,U other,Ports&p){
 if(p.disabled())return 0;
 U result=0;
 if(p.relation(self,p.read(other+4),0)<10)return result;
 if(p.read(other+0xa88))return result;
 U server=p.server(), action=other+0xfc;
 U selected=p.creature(server,p.selected_id(server));
 if(selected!=self){
  result=0;
  if(selected==other||!action)return result;
  if(p.read(p.read(action)))return result;
  if(p.read(other+0x4e0))return result;
  if(p.read(p.read(other+0x9c8)+0x958))return result;
  return 1;
 }
 if(selected==other)return result;
 U first=p.read(p.read(action));
 result=p.read(other+0xa88);
 if(!first)return result;
 U data=p.read(first+8);if(!data)return result;
 U kind=p.read(data);if(kind==12||kind==63||kind==15||kind==46)result=0;
 return result;
}
}
