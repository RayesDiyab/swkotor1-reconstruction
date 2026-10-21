#include "4847ea9d116db9c0_ai_loop.hpp"
#include <cstdint>
namespace kotor::server_mainloop {
static bool ai_signed_less(Word a,Word b){return static_cast<std::int32_t>(a)<static_cast<std::int32_t>(b);}
void ai_loop_004b0b70(Memory&m,AiLoopPorts&p,Word self,Word scratch){
 auto server=[&](){return m.read(m.read(0x7a39fc)+8);};auto clock=[&](){return m.read(m.read(0x7a39e0)+4);};
 p.world_time(m.read(m.read(self)+0x10048),scratch+0x28,scratch+0x24);
 Wide remaining=10000;Word level=4,list=self+0x40;
 do{
  Wide budget=static_cast<std::int32_t>(level)>0?(remaining*60)/100:remaining;
  Wide start=p.time(clock()),current=start;bool first=true,done=false;Word initial=0;
  for(;;){
   Word i=0;if(ai_signed_less(0,m.read(list+8)))do{
    if(m.read(list+8)){
     Word id=m.read(m.read(list+4)+i*4),o=p.object(server(),id);
     if(o&&m.read8(o+8)>m.read8(0x746633)&&m.read(o+0x1f8)&&p.creature(o)){
      Word node=m.read(m.read(p.creature(o)+0xfc));bool special=false;
      if(node){Word action=m.read(node+8);if(action){Word kind=m.read(action);special=kind==1||kind==0x3d;}}
      if(!special)special=m.read(p.creature(o)+0xa8c)==4;
      if(!special)special=m.read(p.creature(o)+0xa8c)==5;
      if(!special)special=m.read(p.creature(o)+0xa8c)==6;
      if(special){Word c=p.creature(o);if(p.action(c,0)==m.read(0x745dc0))m.write(p.creature(o)+0xa8c,1);}
     }
    }
    Word count=m.read(list+8);++i;if(!ai_signed_less(i,count))break;
   }while(true);
   for(;;){
    Word head=m.read(m.read(self+0x54));if(!head)break;Word e=m.read(head+8);
    Word tick=m.read(scratch+0x24),timer=m.read(m.read(self)+0x10048),day=m.read(scratch+0x28);
    Word etick=m.read(e+4),eday=m.read(e);
    if(static_cast<std::int32_t>(p.compare(timer,eday,etick,day,tick))>0)break;
    Word queue=m.read(self+0x54);if(!m.read(queue))continue;e=p.pop(queue);
    Word source=m.read(e+8),target=m.read(e+12),data=m.read(e+20),ed=m.read(e),et=m.read(e+4),kind=m.read(e+16);
    m.write(e+20,0);p.free(e);
    Word objects=m.read(m.read(self)+0x1005c);
    Word result=p.lookup(objects,target,scratch+0x34);
    if((result&255)==m.read8(0x74666c)){
     Word o=m.read(scratch+0x34),type=m.read8(o+8),areaType=m.read8(0x746633);
     if(type>areaType){p.object_event(o,kind,source,data,ed,et);continue;}
     if(type==areaType){p.area_event(p.area(o),kind,source,data,ed,et);continue;}
     if(type==m.read8(0x746632)){p.module_event(p.module(o),kind,source,data,ed,et);continue;}
    }
    p.cleanup(self,kind,data);
   }
   if(current-start<=budget){
    Word count=m.read(list+8),o=0;
    if(count){Word next=m.read(list+0x10)+1;m.write(list+0x10,next);if(!ai_signed_less(next,count))m.write(list+0x10,0);Word ix=m.read(list+0x10),data=m.read(list+4);Word id=m.read(data+ix*4);Word found=p.object(server(),id);if(found&&m.read8(found+8)>m.read8(0x746633))o=found;}
    if(first)initial=o;
    Wide before=p.time(clock());p.clear_string(self+0x60);for(Word off=0x68;off<=0x78;off+=4)m.write(self+off,0);m.write(self+0x7c,0xffffffff);
    if(o&&(o!=initial||first))p.update(o);else done=true;
    first=false;Wide after=p.time(clock());Word elapsed=static_cast<Word>(after)-static_cast<Word>(before);
    if(elapsed>=75000&&o)p.slow(o,0xa3,elapsed,75000,0);
    current=p.time(clock());
   }
   Word head=m.read(m.read(self+0x54));bool due=false;
   if(head){Word e=m.read(head+8),timer=m.read(m.read(self)+0x10048),tick=m.read(scratch+0x24),day=m.read(scratch+0x28),et=m.read(e+4),ed=m.read(e);due=static_cast<std::int32_t>(p.compare(timer,ed,et,day,tick))<=0;}
   if(due||done||current-start>budget)break;
  }
  Wide spent=current-start;remaining-=spent<budget?spent:budget;--level;list-=16;
 }while(static_cast<std::int32_t>(level)>=0);
 Word counts=self+0x48;
 for(Word levelCount=5;levelCount;--levelCount,counts-=16){Word i=0;if(ai_signed_less(0,m.read(counts)))do{
  if(m.read(counts)){Word data=m.read(counts-4),id=m.read(data+i*4);Word o=p.object(server(),id);
   if(o&&m.read8(o+8)>m.read8(0x746633)&&m.read(o+0x1f8)){
    Word render=p.render(o);if(render&&p.render_object(render)){
     if(p.dirty(o,0)){Word r=p.render_object(render),v=p.spill_float(m.read(o+0xd8)),id=m.read(o+0xd4);p.render_size(r,id,v);p.clear_dirty(o,0);}
     if(p.dirty(o,1)){Word r=p.render_object(render),v=p.spill_float(m.read(o+0xb8)),id=m.read(o+0xd4);p.render_position(r,o+0x90,id,v);p.clear_dirty(o,1);}
     if(p.dirty(o,2)){p.render_orientation(p.render_object(render),o+0x9c);p.clear_dirty(o,2);}
    }
   }
  }
  Word count=m.read(counts);++i;if(!ai_signed_less(i,count))break;
 }while(true);}
 Word inner=m.read(self);if(inner&&p.server_module(inner)){inner=m.read(self);p.module_update(p.server_module(inner));}
}
}
