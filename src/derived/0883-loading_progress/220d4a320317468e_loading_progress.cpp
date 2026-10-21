#include "00539c10f9e35c81_loading_progress.hpp"
namespace kotor::server_mainloop {
void loading_hint_005f7b20(Memory& m,LoadingPorts& p,Word self,Word scratch){
 Word server=m.read(m.read(0x7a39fc)+8);
 if(!server||!m.read(server+4))return;
 Word panel=m.read(self+0x278);
 if(!panel||!m.read(panel+0x574))return;
 Word now=p.timer_005e9250(m.read(m.read(0x7a39e0)+4));
 Word old=m.read(self+0x48c);m.write(self+0x48c,now);
 Word total=m.read(self+0x488)+(now-old);m.write(self+0x488,total);
 if(total<10000)return;
 m.write(self+0x488,0);
 p.literal_005e5a90(scratch,0x73d71c);
 Word hint=p.choose_005f4760(self,scratch,2);
 panel=m.read(self+0x278);
 if(panel)p.panel_0067a9c0(panel,hint);
 p.destroy_005e5c20(scratch);
}
void loading_progress_005fb3f0(Memory& m,LoadingPorts& p,Word self,Word value,Word flag,Word scratch){
 p.input_005ed310(m.read(0x7a39e4));
 if(!m.read(self+0x278))return;
 if(flag&&static_cast<std::int32_t>(value)<80)loading_hint_005f7b20(m,p,self,scratch+8);
 p.progress_00417fb0(m.read(self+0x278)+0x64,value);
 p.empty_005b3190(scratch);
 p.format_005e5680(scratch,0x74ec0c,value);
 p.destroy_005e5c20(scratch);
}
}
