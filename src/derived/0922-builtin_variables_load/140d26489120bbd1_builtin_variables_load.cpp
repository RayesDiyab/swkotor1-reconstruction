#include "317d006f64fee8cd_builtin_variables_load.hpp"
namespace kotor::server_mainloop {
void builtin_variables_load_0059b0f0(Memory&m,BuiltinVariablesPorts&p,Word self,Word gff,Word root,Word scratch){
 if(!p.structure(gff,scratch+0x2c,root,0x74a14c))return;
 if(p.list(gff,scratch+0x14,scratch+0x2c,0x74a140)){
  (void)p.count(gff,scratch+0x14);Word observed=p.count(gff,scratch+0x14),limit=observed<3?p.count(gff,scratch+0x14):3;
  for(Word i=0;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(limit);++i)if(p.element(gff,scratch+0x30,scratch+0x14,i)){Word value=p.dword(gff,scratch+0x30,0x74a134,scratch+0x10,0);m.write(self+4*i,value);}
 }
 if(p.list(gff,scratch+0x14,scratch+0x2c,0x74a128)){
  (void)p.count(gff,scratch+0x14);Word observed=p.count(gff,scratch+0x14),limit=observed<8?p.count(gff,scratch+0x14):8;
  for(Word i=0;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(limit);++i)if(p.element(gff,scratch+0x30,scratch+0x14,i)){Word value=p.byte(gff,scratch+0x30,0x74a134,scratch+0x10,0);p.write_byte(self+12+i,value&255);}
 }
}
}
