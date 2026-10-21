#include "ba998d685b231ebc_server_clock.hpp"
namespace kotor::server_mainloop {
static Wide pair(Memory& m,Word a){const Word lo=m.read(a);const Word hi=m.read(a+4);return Wide(lo)|(Wide(hi)<<32);}
Word update_004b1ee0(Memory& m,ClockPorts& p,Word self){
 p.world_timer_reset_004adc70(m.read(self+0x10050));const Word now=p.low_time_005e9250(m.read(m.read(0x007a39e0)+4));
 if(m.read(self+0x1b938)){m.write(self+0x1b934,now);return 1;}
 if(m.read(self+0x1b934)==0)m.write(self+0x1b934,now);
 const Word old=m.read(self+0x1b934);m.write(self+0x1b934,now);const Word elapsed=m.read(self+0x1b930)+(now-old);m.write(self+0x1b930,elapsed);
 if(elapsed>0x000dbba0){m.write(self+0x1b930,0);m.write(self+0x1b938,1);return 1;}return 0;
}
Word tail_004b1e10(Memory& m,Word self,Wide now){
 const Word hi=m.read(self+0x100e8),lo=m.read(self+0x100e4);const Wide old=Wide(lo)|(Wide(hi)<<32);
 if(old==0){m.write(self+0x100e8,Word(now>>32));m.write(self+0x100e4,Word(now));m.write(self+0x100dc,10000000);m.write(self+0x100e0,0);return 0;}
 const Word dhi=m.read(self+0x100e0),dlo=m.read(self+0x100dc);const Wide remaining=Wide(dlo)|(Wide(dhi)<<32);
 if(now-old>=remaining){m.write(self+0x100e4,Word(now));m.write(self+0x100dc,10000000);m.write(self+0x100e0,0);m.write(self+0x100e8,Word(now>>32));return 1;}
 const Wide next=remaining-now+old;m.write(self+0x100e4,Word(now));m.write(self+0x100e8,Word(now>>32));m.write(self+0x100e0,Word(next>>32));m.write(self+0x100dc,Word(next));return 0;
}
Word timeout_004b4ab0(Memory& m,ClockPorts& p,Word self,Wide now){
 const Wide duration=pair(m,self+0x10028);if(duration==0)return 0;
 const Wide stamp=pair(m,self+0x10030);const Wide elapsed=now-stamp;
 if(elapsed>duration){m.write(self+0x10028,0);m.write(self+0x1002c,0);return 1;}
 const Wide remaining=stamp-now+duration;
 if((duration>60000000&&remaining<60000000)||(duration>30000000&&remaining<30000000)){
  for(Word i=0;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(m.read(self+0x10020));++i){
   const Wide current=pair(m,self+0x10030)-now+pair(m,self+0x10028);const Word seconds=Word(p.divide_006fb030(current,1000000));
   const Word ids=m.read(self+0x1001c),receiver=m.read(self+0x10010);const Word player=m.read(ids+i*4);p.notify_timeout_0056b2b0(receiver,player,seconds);
  }
 }
 const Wide next=pair(m,self+0x10030)-now+pair(m,self+0x10028);
 m.write(self+0x10034,Word(now>>32));m.write(self+0x10030,Word(now));m.write(self+0x10028,Word(next));m.write(self+0x1002c,Word(next>>32));return 0;
}
Word load_mode_005edfb0(Memory& m,Word self){return m.read(m.read(self+4)+0x33c);}
}
