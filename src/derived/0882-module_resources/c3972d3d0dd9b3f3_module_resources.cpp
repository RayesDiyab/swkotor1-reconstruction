#include "ffeb4d8d8c3c3314_module_resources.hpp"
namespace kotor::server_mainloop {
void module_resources_004c4150(Memory&m,ModuleResourcePorts&p,Word self,Word name,Word scratch){
 const auto s=[&](Word x){return scratch+x;};
 const auto client=[&](){return m.read(m.read(0x7a39fc)+4);};
 const auto manager=[&](){return m.read(0x7a39e8);};
 p.empty_005b3190(s(0x20));
 Word progress=p.progress_part_005edff0(client(),0)&255;
 const Word span=p.progress_part_005edff0(client(),1)&255,end=progress+span,step=span/7;
 p.notify_name_0063e7c0(manager(),name);
 p.literal_005e5a90(s(0x18),0x7458b8);p.add_004087a0(manager(),s(0x18),2);p.destroy_005e5c20(s(0x18));
 progress+=step;p.progress_005edd40(client(),progress,1);p.pump_00401c10(0x3d088889,0,0);
 const Word colon=p.find_005e5f20(name,0x3a,0);m.write(s(0x18),colon);
 if(colon==0xffffffff){const Word v=p.concat_005e5d10(s(0x20),s(0x30),name);p.assign_005e5c50(s(0x20),v);p.destroy_005e5c20(s(0x30));}
 else{const Word n=p.length_005e5790(name)-m.read(s(0x18))-1;const Word part=p.right_005e6170(name,s(0x38),n);const Word v=p.concat_005e5d10(s(0x20),s(0x30),part);p.assign_005e5c50(s(0x20),v);p.destroy_005e5c20(s(0x30));p.destroy_005e5c20(s(0x38));}
 p.literal_005e5a90(s(0x18),0x73d8f8);p.prepare_00408800(manager(),s(0x18));p.destroy_005e5c20(s(0x18));
 p.resref_00406d60(s(0x38),s(0x20));if(p.exists_00408bc0(manager(),s(0x38),0x809,0))m.write(self+0x1c8,1);
 p.literal_005e5a90(s(0x18),0x73d8f8);p.finalize_004088d0(manager(),s(0x18));p.destroy_005e5c20(s(0x18));
 p.empty_005b3190(s(0x28));p.format_005e5680(s(0x28),0x7458ac,p.cstr_005e5670(s(0x20)));p.add_004087a0(manager(),s(0x28),2);
 p.literal_005e5a90(s(0x30),0x745898);p.add_004087a0(manager(),s(0x30),2);p.destroy_005e5c20(s(0x30));
 p.mount_004064f0(manager(),s(0x20),1,m.read(self+0x1c8));
 Word count=0,active=manager();while(m.read8(active+0x44)==1){
  p.sleep_0073d1e4(10);
  if(static_cast<std::int32_t>(progress)<static_cast<std::int32_t>(end)&&++count>100){
   progress+=step;if(static_cast<std::int32_t>(progress)>=static_cast<std::int32_t>(end))progress=end;
   p.progress_005edd40(client(),progress,1);p.pump_00401c10(0x3d088889,0,0);count=0;
  }
  active=manager();
 }
 p.write_byte(s(0x18),m.read8(active+0x5c));p.result_005ee0a0(client(),m.read(s(0x18)));
 p.progress_005edd40(client(),end,1);p.pump_00401c10(0x3d088889,0,0);p.destroy_005e5c20(s(0x28));p.destroy_005e5c20(s(0x20));
}
}
