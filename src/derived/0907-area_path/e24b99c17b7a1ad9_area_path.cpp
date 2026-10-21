#include "e13b84fe00ab603c_area_path.hpp"
namespace kotor::server_mainloop {
Word area_path_00508400(Memory&m,AreaPathPorts&p,Word self,Word,Word scratch){
 Word result=1,ref=self+0x10c;
 if(!p.exists_00408bc0(m.read(0x7a39e8),ref,0xbbb,0))return result;
 Word allocation=p.allocate(0xa0),resource=allocation?p.construct_00410630(allocation,0xbbb,0x7476f8,ref):0;
 if(!m.read(resource+0x94)){p.destroy(resource,1);return 0;}
 Word root=scratch+0x1c,list=scratch+0x20,element=scratch+0x14,status=scratch+0x10;
 p.root_00411240(resource,root);
 if(p.list_004118c0(resource,list,root,0x7476ec)){
  Word count=p.count_00411940(resource,list);m.write(self+0x238,count);
  if(!count){p.destroy(resource,1);return 0;}
  Word data=p.allocate(count<<4);m.write(self+0x23c,data);
  Word index=0,offset=0;
  if(m.read(self+0x238)>0)do{
   if(p.element_00411990(resource,element,list,index)&&p.type_004111c0(resource,element)==2){
    Word value=p.float_bits_00411d00(resource,element,0x7476e8,status,0);m.write(m.read(self+0x23c)+offset,value);result&=m.read(status);
    value=p.float_bits_00411d00(resource,element,0x74699c,status,0);m.write(m.read(self+0x23c)+offset+4,value);result&=m.read(status);
    value=p.dword_00411c20(resource,element,0x7476dc,status,0);m.write(m.read(self+0x23c)+offset+8,value);result&=m.read(status);
    value=p.dword_00411c20(resource,element,0x7476cc,status,0);m.write(m.read(self+0x23c)+offset+12,value);result&=m.read(status);
   }
   Word live=m.read(self+0x238);++index;offset+=16;if(index>=live)break;
  }while(true);
 }
 if(p.list_004118c0(resource,list,root,0x7476bc)){
  Word count=p.count_00411940(resource,list);m.write(self+0x240,count);
  Word data=p.allocate(count*4);m.write(self+0x244,data);
  Word index=0;if(m.read(self+0x240)>0)do{
   if(p.element_00411990(resource,element,list,index)&&p.type_004111c0(resource,element)==3){Word value=p.dword_00411c20(resource,element,0x7476b0,status,0);m.write(m.read(self+0x244)+index*4,value);result&=m.read(status);}
   Word live=m.read(self+0x240);++index;if(index>=live)break;
  }while(true);
 }
 p.destroy(resource,1);return result;
}
}
