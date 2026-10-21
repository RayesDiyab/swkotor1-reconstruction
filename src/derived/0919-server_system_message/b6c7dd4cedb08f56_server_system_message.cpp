#include "d3b7b8f180b0889c_server_system_message.hpp"
namespace kotor::server_mainloop {
Word system_token_00528130(Memory&m,ServerSystemPorts&p,Word token,Word cursor,Word remaining,Word dot){Word n=p.length(token);if(n>m.read(remaining))return 0;if(p.compare(m.read(cursor),token,n))return 0;if(dot){Word next=m.read(cursor)+n;if(m.read8(next)!=0x2e)return 0;m.write(cursor,next+1);m.write(remaining,m.read(remaining)-n-1);}else{m.write(cursor,m.read(cursor)+n);m.write(remaining,m.read(remaining)-n);}return 1;}
Word system_argument_00528090(Memory&m,ServerSystemPorts&p,Word data,Word length,Word out,Word maximum,Word scratch){if(m.read8(data)!=32)return 0;if(maximum&&(length<=1||length-1>maximum))return 0;Word temp=p.counted(scratch,data+1,length-1);p.assign(out,temp);p.destroy(scratch);return 1;}
Word server_system_00528380(Memory&m,ServerSystemPorts&p,Word self,Word id,Word packet,Word length,Word scratch){auto s=[&](Word n){return scratch+n;};auto server=[&](){return m.read(m.read(0x7a39fc)+8);};auto done=[&](Word result){p.destroy(s(0x2c));return result;};auto token=[&](Word global,Word dot){return system_token_00528130(m,p,m.read(global),s(0x14),s(0x0c),dot);};
 m.write(s(0x1c),self);p.empty(s(0x2c));if(m.read8(packet)!=0x73)return done(0);Word network=p.network(server());Word record=p.connection(network,id);if(!m.read(record+0x24))return done(0);
 Word echo=m.read(0x7a1c60);m.write(s(0x14),packet+1);m.write(s(0x0c),length-1);Word n=p.length(echo);
 if(n<=m.read(s(0x0c))&&!p.compare(m.read(s(0x14)),echo,n)){m.write(s(0x0c),m.read(s(0x0c))-n);m.write(s(0x14),m.read(s(0x14))+n);Word live=m.read(0x7a1c60),one=p.length(live),two=p.length(live);Word text=p.counted(s(0x4c),packet+two+2,length-one-2);p.status_text(self,id,text);p.destroy(s(0x4c));}
 if(token(0x7a1c64,1)){
  Word request=m.read(0x7a1c68),size=p.length(request),available=m.read(s(0x0c));if(size<=available&&!p.compare(m.read(s(0x14)),request,size)){if(available-size)return done(0);p.status(self,id);return done(1);}
  if(token(0x7a1c78,0)){if(m.read(s(0x0c)))return done(0);p.modules(self,id);return done(1);}
  if(token(0x7a1c88,0)){if(m.read(s(0x0c)))return done(0);p.players(self,id);return done(1);}
 }
 if(token(0x7a1c9c,1)){
  if(token(0x7a1ca0,0)){Word remaining=m.read(s(0x0c));if(!remaining)return done(0);p.empty(s(0x24));if(system_argument_00528090(m,p,m.read(s(0x14)),remaining,s(0x24),256,s(0x50))){Word player=p.player(server(),id,0);Word outer=server();Word playerObject=p.player_object(player);p.load_module(outer,s(0x24),playerObject);}p.destroy(s(0x24));return done(1);}
  if(token(0x7a1ca4,0)){if(m.read(s(0x0c)))return done(0);p.run_module(server());return done(1);}
  if(token(0x7a1ca8,0)){if(!m.read(s(0x0c)))return done(0);Word result=p.save_tail(self,id,s(0x14),s(0x0c),scratch);return done(result);}
 }
 return done(p.remaining_tail(self,id,s(0x14),s(0x0c),scratch));
}
}
