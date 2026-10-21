#include "4f727ab50b1551ae_server_mainloop.hpp"
#include <initializer_list>
namespace kotor::server_mainloop {
Word mainloop_004babb0(Memory& m,Ports& p,Word self,Word scratch){
 const auto s=[&](Word off){return scratch+off;};
 const auto app=[&](){return m.read(0x007a39fc);};
 const auto client=[&](){return m.read(app()+4);};
 const auto server=[&](){return m.read(app()+8);};
 const auto loading=[&](){return m.read(app()+0x14);};
 const auto creature=[&](){const Word v=server();return p.creature_004ae770(v,p.player_id_004aea40(v));};
 p.update_004b1ee0(self);
 if(m.read(loading())==0){
  const Word pause=m.read8(self+0x10078);
  if((pause&3)==0)p.tick_004adbd0(m.read(self+0x10048));
  if((m.read8(self+0x10078)&2)==0)p.tick_004adbd0(m.read(self+0x1004c));
  p.tick_004adbd0(m.read(self+0x10050));
  p.ai_mode_005d57d0(m.read(self+0x10040),1);
  p.ai_update_005d5650(m.read(self+0x10040),m.read(0x0074c53c));
  p.party_update_004b6da0(self);p.party_positions_004b4e80(self);
  if(m.read(loading())==0&&m.read16(self+0x10008)==2){
   p.net_update_004b0b70(m.read(self+0x10044));p.update_state_004b6950(self,m.read(self+0x10014));m.write(self+0x10014,0);
  }
  if(m.read(self+0x10080)==1&&m.read(p.gui_005ed690(client())+0xdc)==0){
   p.timer_fields_004ade40(p.timer_004aede0(server()),s(0x44),s(0x1c));
   p.timer_split_004add90(p.timer_004aede0(server()),m.read(s(0x1c)),s(0x18),s(0x2c),s(0x28),s(0x24));
   p.module_date_004c4100(p.module_004b14f0(self),s(0x3c),s(0x34),s(0x30),s(0x18),s(0x17),s(0x4c));
   p.set_date_004aebc0(server(),m.read(s(0x3c)),m.read(s(0x34)),m.read(s(0x30)),m.read(s(0x18)),m.read(s(0x2c)),m.read(s(0x28)),m.read(s(0x24)));
   p.set_tick_004af0c0(server(),m.read(s(0x1c)));p.set_day_004af0a0(server(),m.read(s(0x44)));
   p.party_flag_00565530(self+0x1b770,0);p.transition_004ba920(self,self+0x10084);m.write(self+0x10080,0);
  }
  if(m.read(loading())==0&&creature()!=0&&p.creature_area_004cb120(creature())!=0&&p.client_busy_005edad0(client())==0){
   if(m.read(self+0x100b8)==1){
    if(p.can_save_004b2520(self)!=0&&m.read16(self+0x10008)==2){
     p.empty_005b3190(s(0x4c));p.literal_005e5140(s(0x4c),0x007455a0);
     p.save_004b58a0(self,1,s(0x4c),0,1);p.destroy_005e5c20(s(0x4c));
    }else{
     if(m.read(p.gui_005ed690(client())+0xb4)!=0)m.write(p.gui_005ed690(client())+0xb98,1);
     else{m.write(s(0x4c),0);m.write(s(0x50),0);m.write(s(0x54),0);p.fade_0062abf0(p.gui_005ed690(client()),1,0x3f000000,0x3f800000,s(0x4c));}
     p.client_mode_005edc80(client(),1);p.input_005ed310(m.read(0x007a39e4));p.gui_mode_0062b410(p.gui_005ed690(client()),0);p.reset_00604d70(m.read(p.client_internal_005ed530(client())+0x48));
    }
   }
   m.write(self+0x100b8,0);
  }
  if(m.read(self+0x100bc)==1){p.save_done_004b3000(self);m.write(self+0x100bc,0);}
  if(m.read(self+0x100c0)==1){
   Word node=m.read(m.read(m.read(self+0x10064)));Word player=node?m.read(node+8):0;
   p.player_notice_0056c7f0(m.read(self+0x10010),m.read(player+4));m.write(self+0x100c0,0);
  }
 }else{
  p.ai_mode_005d57d0(m.read(self+0x10040),0);p.ai_update_005d5650(m.read(self+0x10040),m.read(0x0074c53c));
  const Word state=loading();
  if(m.read(state+0x38)==0){
   const Word mode=m.read(state+4);
   if(mode==2){p.unload_phase_004b3110(self);p.log_00401430(0x007455b8);}
   else if(mode==1||mode==3){
    p.copy_string_005e5b00(s(0x58),state+0x18);
    const Word module=p.module_004b14f0(self);const Word loadMode=p.load_mode_005edfb0(client())==1?1u:0u;
    if(m.read(state+0xc)==0){
     p.copy_string_005e5b00(s(0x44),state+0x18);p.copy_string_005e5b00(s(0x34),state+0x20);p.copy_string_005e5b00(s(0x3c),state+0x28);
     const Word type=m.read(state+0x30);
     if(p.copy_module_004b1680(self,s(0x34),s(0x3c),type)==0)p.error_004ba5b0(self,5);
     else{
      const Word raw=p.allocate_006fa7e6(0x224);const Word instance=raw?p.construct_module_004c84a0(raw,s(0x44),0,((type&0xffff)==0x809||(type&0xffff)==0xbc1)?1u:0u):0;
      m.write(self+0x10060,m.read(instance+0x20)); // Original intentionally dereferences failed allocation.
      if((type&0xffff)==0xbc1){m.write(instance+0x200,1);p.assign_005e5c50(instance+0x204,s(0x44));}
      const Word error=p.start_module_004c9050(instance,s(0x44),loadMode);if(error)p.error_004ba5b0(self,error);
     }
     p.destroy_005e5c20(s(0x3c));p.destroy_005e5c20(s(0x34));p.destroy_005e5c20(s(0x44));
    }else if(m.read(state+0xc)!=m.read(state+8)){
     p.party_reset_005639c0(self+0x1b770);const Word error=p.area_step_004c5720(module,m.read(state+8),m.read(state+0xc));if(error)p.error_004ba5b0(self,error);
    }else if(m.read(state+0x14)!=1){
     const Word error=p.finish_module_004c5880(module);if(error)p.error_004ba5b0(self,error);
     m.write(state+0x14,1);if(m.read8(0x007a39dc)==2){m.write(state+0x14,0);m.write(state,0);m.write(state+4,0);}
     m.write16(self+0x10008,1);m.write(self+0x100b4,0);p.status_0056c980(p.message_004aedb0(server()),1,0);
     p.notify_00568310(m.read(self+0x10010),m.read(0x0074c52c));p.notify_00579920(m.read(self+0x10010),m.read(0x0074c534));
     if(m.read(self+0x100f0)!=0){
      for(Word i=0;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(m.read(self+0x100f0));++i){
       const Word id=m.read(m.read(self+0x100ec)+i*4);Word found=0;
       for(Word offset: {0x10064u,0x10068u}){
        const Word pos=s(offset==0x10064u?0x1c:0x20);m.write(pos,m.read(m.read(m.read(self+offset))));
        while(m.read(pos)){Word entry=p.list_at_005e9b60(m.read(m.read(self+offset)),m.read(pos));if(m.read(entry+4)==id){found=entry;break;}p.list_next_005e9b90(m.read(m.read(self+offset)),pos);}
        if(found)break;
       }
       if(found)p.player_ready_00576360(m.read(self+0x10010),found);
      }
      if(m.read(self+0x100ec)){p.free_006fa390(m.read(self+0x100ec));m.write(self+0x100ec,0);m.write(self+0x100f4,0);}m.write(self+0x100f0,0);
     }
    }
    p.destroy_005e5c20(s(0x58));
   }
  }
 }
 if(p.timeout_004b4ab0(self,p.time_005e9260(m.read(m.read(0x007a39e0)+4)))!=0){
  if(m.read(self+0x10020))p.shutdown_004b69c0(self,0,0);else p.quit_00401280(app());
 }
 p.tail_004b1e10(self,p.time_005e9260(m.read(m.read(0x007a39e0)+4)));return 0;
}
}
