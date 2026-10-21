#include "8b63c5db7f-vm_globals.hpp"
#include "7acd3f8ce2-vm_situation.hpp"
#include <cstring>
namespace kotor::vm_globals {
namespace {
// returns the identifier code for a found name, or -1
I32 lookup(Memory& m,Ports& p,Word table,Word name,Word category,Word& index){
 const I32 slot=p.find_identifier_00529030(table,name);
 if(slot<0)return -1;
 const Word code=m.read16(table+static_cast<Word>(slot)*24+0x16);
 if((code&0xc000)!=(category<<14))return -1;
 index=code&0x3fff;return 1;
}
void copy24(Memory& m,Word dst,Word src){for(Word o=0;o<24;o+=4)m.write(dst+o,m.read(src+o));}
}
void get_value_location_00529350(Memory& m,Ports& p,Word t,Word name,Word out){Word i=0;if(lookup(m,p,t,name,2,i)>0)copy24(m,out,t+0xa8d0+i*24);}
void get_value_string_00529460(Memory& m,Ports& p,Word t,Word name,Word out){Word i=0;if(lookup(m,p,t,name,3,i)>0)p.string_assign_005e5c50(out,t+0xb230+i*8);}
void set_value_location_00529790(Memory& m,Ports& p,Word t,Word name,Word loc){Word i=0;if(lookup(m,p,t,name,2,i)>0)copy24(m,t+0xa8d0+i*24,loc);}
void set_value_string_005298a0(Memory& m,Ports& p,Word t,Word name,Word value){Word i=0;if(lookup(m,p,t,name,3,i)>0)p.string_assign_005e5c50(t+0xb230+i*8,value);}
I32 get_global_string_00538ed0(Memory& m,Ports& p,const Scratch& s){
 p.string_default_005b3190(s.name);p.string_default_005b3190(s.value);
 if(!p.pop_string_005d25e0(p.vm_internal(),s.name)){p.string_destroy_005e5c20(s.value);p.string_destroy_005e5c20(s.name);return kPushFailed;}
 get_value_string_00529460(m,p,p.global_table(),s.name,s.value);
 const Word ok=p.push_string_005d1930(p.vm_internal(),s.value);
 p.string_destroy_005e5c20(s.value);p.string_destroy_005e5c20(s.name);
 return ok?0:kPushFailed;
}
I32 set_global_string_00542c60(Memory& m,Ports& p,const Scratch& s){
 p.string_default_005b3190(s.name);p.string_default_005b3190(s.value);
 if(!p.pop_string_005d25e0(p.vm_internal(),s.name)||!p.pop_string_005d25e0(p.vm_internal(),s.value)){
  p.string_destroy_005e5c20(s.value);p.string_destroy_005e5c20(s.name);return kPushFailed;}
 set_value_string_005298a0(m,p,p.global_table(),s.name,s.value);
 p.string_destroy_005e5c20(s.value);p.string_destroy_005e5c20(s.name);
 return 0;
}
I32 get_global_location_00538cd0(Memory& m,Ports& p,const Scratch& s){
 p.string_default_005b3190(s.name);
 vm_situation::location_construct_004ca7a0(m,s.location);
 if(!p.pop_string_005d25e0(p.vm_internal(),s.name)){p.string_destroy_005e5c20(s.name);return kPushFailed;}
 get_value_location_00529350(m,p,p.global_table(),s.name,s.location);
 const Word ok=p.push_engine_structure_005d19c0(p.vm_internal(),2,s.location);
 p.string_destroy_005e5c20(s.name);
 return ok?0:kPushFailed;
}
I32 set_global_location_00542a50(Memory& m,Ports& p,const Scratch& s){
 p.string_default_005b3190(s.name);
 if(!p.pop_string_005d25e0(p.vm_internal(),s.name)){p.string_destroy_005e5c20(s.name);return kPushFailed;}
 Word location=0;
 if(!p.pop_engine_structure_005d2630(p.vm_internal(),2,&location)){p.string_destroy_005e5c20(s.name);return kPopFailed;}
 if(location){set_value_location_00529790(m,p,p.global_table(),s.name,location);p.free_006fa390(location);}
 p.string_destroy_005e5c20(s.name);
 return 0;
}
}
