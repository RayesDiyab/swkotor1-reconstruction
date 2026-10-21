#include "a6f139d905fbf3e2_module_helpers.hpp"
namespace kotor::server_mainloop {
void module_identifier_0054eaa0(Memory&m,ModuleHelperPorts&p,Word self,Word name){
 p.assign_005e5c50(self+0xfc,name);
 Word special=p.equals_005e5290(self+0xfc,0x748778);
 if(!special)special=p.equals_005e5290(self+0xfc,0x74876c);
 if(!special)special=p.equals_005e5290(self+0xfc,0x748760);
 special=!!special;



 Word changed=(m.read(self+0x104)^special)&1;Word prior=m.read(self+0x104);m.write(self+0x104,prior^changed);

}
void timer_scale_004adba0(Memory&m,ModuleHelperPorts&p,Word self,Word scale){
 p.write_byte(self+0x38,scale);

 if(!(scale&255))p.write_byte(self+0x38,5);

 Word value=m.read8(self+0x38);Word day=value*0x15f900u;Word hour=value*0x5a0u;



 m.write(self+0x40,hour);m.write(self+0x3c,day);

}

void module_tag_004c3060(ModuleHelperPorts&p,Word self,Word name,Word scratch){Word result=p.lower_005e6080(name,scratch);p.assign_005e5c50(self+0x1f8,result);p.destroy_005e5c20(scratch);}
Word resource_type_00406720(Word type){switch(type&0xffff){case 0x7d9:case 0x7da:case 0x7dc:case 0x7e9:case 0x7eb:case 0x7ed:case 0x7ee:case 0x7f0:case 0x7f3:case 0x7f8:case 0x7fa:case 0x7fc:case 0x7fe:case 0x803:case 0x808:case 0x80a:case 0xbbb:return 1;default:return 0;}}
Word resource_count_0040d1a0(Memory&m,ModuleHelperPorts&p,Word self,Word all){
 Word result=0,index=0,offset=0;
 while(index<m.read(self+0xc)){Word entry=m.read(self+0x10)+offset;if(entry){Word type=m.read16(entry+0x1a);if(type!=0xffff){if(all||!p.resource_available_00406720(m.read(0x7a39e8),type))++result;}}++index;offset+=0x1c;}
 return result;
}
Word resource_path_00407780(Memory&m,ModuleHelperPorts&p,Word self,Word name,Word all){Word entry=p.lookup_004076e0(self,name);if(!entry)return 0xffffffff;return resource_count_0040d1a0(m,p,entry,all);}
}
