#include "a73d34dc7b6860bc_module_load.hpp"
namespace kotor::server_mainloop {
void module_time_004c9597(Memory& m,ModuleLoadPorts& p,Word self,Word use_saved,Word scratch){
 auto s=[scratch](Word x){return scratch+x;};auto res=[&](){return m.read(self+8);};auto server=[&](){return m.read(m.read(0x7a39fc)+8);};
 Word day;
 if(use_saved){
  m.write(self+0x1a8,p.word_00411c20(res(),s(0x10),0x745ba8,s(0xc),m.read(0x74410c)));
  const Word labels[]={0x745b98,0x745b88,0x745b78},globals[]={0x74411c,0x744128,0x744134};
  for(Word i=0;i<3;i++)p.write_byte(self+0x1a3+i,p.byte_00411a60(res(),s(0x10),labels[i],s(0xc),m.read8(globals[i])));
  m.write(self+0x1b8,p.word_00411c20(res(),s(0x10),0x745b68,s(0xc),0));
  m.write(s(0x88),p.short_00411b40(res(),s(0x10),0x745b58,s(0xc),0));
  m.write(s(0x84),p.short_00411b40(res(),s(0x10),0x745b48,s(0xc),0));
  m.write(s(0x80),p.short_00411b40(res(),s(0x10),0x745b34,s(0xc),0));
  m.write(s(0x20),p.word_00411c20(res(),s(0x10),0x745b14,s(0xc),0));
  day=p.word_00411c20(res(),s(0x10),0x745b24,s(0xc),0);
 }else{
  p.date_004aebd0(server(),self+0x1a8,self+0x1a3,self+0x1a4,self+0x1a5,s(0x88),s(0x84),s(0x80));
  m.write(s(0x20),p.tick_004af0d0(server()));day=p.day_004af0b0(server());
 }
 m.write(s(0x38),day);
 Word month=m.read8(self+0x1a3),hour=m.read8(self+0x1a5),daypart=m.read8(self+0x1a4);
 m.write(self+0x1b0,daypart);m.write(self+0x1ac,month);m.write(self+0x1b4,hour);
 Word scale=m.read8(self+0x1a0);p.timer_scale_004adba0(p.timer_004aede0(server()),scale);
 Word year=m.read(self+0x1a8),mo=m.read8(self+0x1a3),d=m.read8(self+0x1a4),h=m.read8(self+0x1a5),t0=m.read16(s(0x88)),t1=m.read16(s(0x84)),t2=m.read16(s(0x80));
 p.timer_date_004ae3e0(p.timer_004aede0(server()),year,mo,d,h,t0,t1,t2,1);
 Word timer=p.timer_004aede0(server());m.write(timer+0x28,m.read(s(0x38)));
 timer=p.timer_004aede0(server());m.write(timer+0x2c,m.read(s(0x20)));
 Word app=server(),source=p.timer_004aede0(app);p.timer_copy_004adef0(p.timer_004aedf0(app),source);
 app=server();source=p.timer_004aede0(app);p.timer_copy_004adef0(p.timer_004aee00(app),source);
 p.timer_fields_004ade40(p.timer_004aede0(server()),self+0x1c0,self+0x1c4);
 Word start=m.read8(self+0x1a1),end=m.read8(self+0x1a2),phase=1;
 if(start!=end){Word current=m.read(self+0x1b4);if(current==start)phase=3;else if(current==end)phase=4;else if(start<end){if(current<start||current>end)phase=2;}else if(current>=end&&current<=start)phase=2;}
 p.write_byte(self+0x1bc,phase);
 p.write_byte(self+0x1a6,p.byte_00411a60(res(),s(0x10),0x745b08,s(0xc),10));
}
}
