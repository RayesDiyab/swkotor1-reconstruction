#include "3732422c487cdca4_area_properties_load.hpp"
namespace recovered_area_properties_load{U load(U self,U gff,U root,U scratch,Ports&p){const U nested=scratch+4,status=scratch;if(!p.structure(gff,nested,root,0x747660))return 0;
 U v=p.scalar(0x411a60,gff,nested,0x747654,status,p.read(self+0x2ac))&255;p.write(self+0x2ac,v);
 U mode=p.scalar(0x411a60,p.read(self+0x108),root,0x747644,status,p.read(self+0x2b0))&255;if(mode&&mode!=p.read(self+0x2b0))p.restrict_party(p.party(),1);
 U fallback=p.read(self+0x2b4);p.write(self+0x2b0,mode);U maximum=p.scalar(0x411c20,gff,nested,0x747634,status,fallback);if(p.read(self+0x2b8)>maximum)p.write(self+0x2b8,maximum);p.write(self+0x2b4,maximum);
 U current=p.scalar(0x411c20,gff,nested,0x747620,status,maximum);U live_max=p.read(self+0x2b4);if(current>live_max)current=live_max;p.write(self+0x2b8,current);
 v=p.scalar(0x411c20,gff,nested,0x747610,status,p.read(self+0x2bc));p.write(self+0x2bc,v);
 v=p.scalar(0x411a60,gff,nested,0x7475fc,status,p.read(self+0x2c0))&255;U next=p.byte(self+0x2c4);p.write(self+0x2c0,v);
 v=p.scalar(0x411a60,gff,nested,0x7475ec,status,next)&255;next=p.byte(self+0x2c8);p.write(self+0x2c4,v);
 v=p.scalar(0x411a60,gff,nested,0x7475dc,status,next);next=p.byte(self+0x2c9);p.byte(self+0x2c8,v);
 v=p.scalar(0x411a60,gff,nested,0x7475cc,status,next);U fog=p.read(self+0x8c);p.byte(self+0x2c9,v);
 v=p.scalar(0x411c20,gff,nested,0x7475c0,status,fog);U child=p.read(self+0x208);p.write(self+0x8c,v);return p.child(child,gff,nested);
}}
