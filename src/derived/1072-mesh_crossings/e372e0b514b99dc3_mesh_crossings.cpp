#include "b066d4a60e85dd9f_mesh_crossings.hpp"
namespace recovered_mesh_crossings{
namespace{bool less(U s){return (s&0x4500)==0x0100;}bool equal(U s){return (s&0x4500)==0x4000;}bool greater(U s){return (s&0x4100)==0;}}
U count(U self,U from,U to,U edgeOut,U s,Ports&p){
 p.write(self+0xc0,0);p.write(self+0xcc,0);p.write(s+32,0);p.prepare(from,to,s);U direction=s,perpendicular=s+12,start=p.read(s+24),line=p.read(s+28),previousLine=line;U found=0,begin=0,group=0;
 if(static_cast<int>(p.read(self+0xa0))<=0)return 0;
 do{U end=p.read(p.read(self+0x9c)+group*4);U edge=begin;
  while(static_cast<int>(edge)<static_cast<int>(end)){
   U next=edge+1,prev=edge-1;if(static_cast<int>(prev)<static_cast<int>(begin))prev=end-1;
   U edges=p.read(self+0x8c),indices=p.read(self+0x60),vertices=p.read(self+0x54);
   U before=vertices+p.read(indices+p.read(edges+prev*8)*4)*12;
   U current=vertices+p.read(indices+p.read(edges+edge*8)*4)*12;
   U after=vertices+p.read(indices+p.read(edges+(next==end?begin:next)*8)*4)*12;
   U currentX=p.project(current,direction,0),currentY=p.project(current,perpendicular,1),nextY=p.project(after,perpendicular,2);
   bool crossing=(less(p.compare(currentY,line))&&less(p.compare(line,nextY)))||(less(p.compare(nextY,line))&&less(p.compare(line,currentY)));
   if(crossing){p.interpolate(current,after,line,currentY,nextY,s+36);if(p.forward(s+36,direction,start)){p.insert(self,from,direction,s+36,s+32,edge);if(!found){p.write(edgeOut,edge);found=1;}}}
   if(equal(p.compare(currentY,line))&&!(p.compare(currentX,start)&0x100)){
    Projection prevY=p.previous(before,perpendicular,currentY);
    if(!equal(prevY.status)||!equal(p.compare(currentY,nextY))){
     bool add=true;
     if(equal(p.compare(previousLine,line))){if(!equal(p.compare(prevY.bits,line)))previousLine=prevY.bits;}
     else{add=(less(p.compare(previousLine,line))&&less(p.compare(nextY,line)))||(greater(p.compare(previousLine,line))&&greater(p.compare(nextY,line)));previousLine=line;}
     if(add){p.insert(self,from,direction,current,s+32,edge);if(!found){p.write(edgeOut,edge);found=1;}
      if(equal(p.compare(currentX,start))&&((less(p.compare(prevY.bits,line))&&less(p.compare(nextY,line)))||(greater(p.compare(prevY.bits,line))&&greater(p.compare(nextY,line))))){p.insert(self,from,direction,current,s+32,edge);if(!found){p.write(edgeOut,edge);found=1;}}
     }
    }
   }
   edge=next;
  }
  begin=end;++group;
 }while(static_cast<int>(group)<static_cast<int>(p.read(self+0xa0)));
 U result=p.read(s+32);if(static_cast<int>(result)>1)p.write(edgeOut,p.read(p.read(self+0xc8)));return result;
}
}
