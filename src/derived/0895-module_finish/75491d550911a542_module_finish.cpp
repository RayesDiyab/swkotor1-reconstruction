#include "b49d6d2dc87beff3_module_finish.hpp"
namespace kotor::server_mainloop {
Word module_finish_004c8c70(Memory&m,ModuleFinishPorts&p,Word self,Word resource,Word record,Word mode,Word scratch){
 auto s=[scratch](Word off){return scratch+off;};
 if(p.list_004118c0(resource,s(0x24),record,0x7458dc)){
  m.write(s(0xc),0);Word index=0;
  if(p.count_00411940(resource,s(0x24))){do{
   if(p.element_00411990(resource,s(0x4c),s(0x24),index)&&p.type_004111c0(resource,s(0x4c))==4){
    Word id=p.word_00411c20(resource,s(0x4c),0x744c24,s(0x48),0x7f000000);
    Word allocated=p.allocate(0xac8),creature=0;if(allocated)creature=p.creature_004f7a10(allocated,id,0);
    if(creature){if(!p.load_00500350(creature,resource,s(0x4c),mode)){p.destroy(creature,1);}else{
     if(mode)m.write(creature+0x34c,1);
     (void)p.float_bits_00411d00(resource,s(0x4c),0x745d80,s(0x48),0);
     (void)p.float_bits_00411d00(resource,s(0x4c),0x745d74,s(0x48),0);
     (void)p.float_bits_00411d00(resource,s(0x4c),0x745d68,s(0x48),0);
     m.write(s(0x14),p.float_bits_00411d00(resource,s(0x4c),0x745d58,s(0x48),0));
     m.write(s(0x10),p.float_bits_00411d00(resource,s(0x4c),0x745d48,s(0x48),0));
     m.write(s(0x18),p.float_bits_00411d00(resource,s(0x4c),0x745d38,s(0x48),0));
     Word first=m.read(s(0x14)),second=m.read(s(0x10));m.write(s(0x20),first);m.write(s(0x1c),second);
     p.position_004cd170(creature,s(0x18));p.attach_004c83b0(self,m.read(creature+4));
    }}
   }
   index=m.read(s(0xc))+1;m.write(s(0xc),index);
  }while(index<p.count_00411940(resource,s(0x24)));}
 }
 return 1;
}
}
