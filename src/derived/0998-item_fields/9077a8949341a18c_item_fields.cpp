#include "8bbb96aee817709f_item_fields.hpp"
namespace recovered_item_fields{
void prefix(U self,U g,U r,U s,Ports&p){
 const U status=s+32;auto scalar=[&](U type,const char*l,U def){return p.scalar(type,g,r,l,status,def);};auto bit=[&](unsigned n,U value){U f=p.read(self+0x288);p.write(self+0x288,(f&~(1u<<n))|((value&1)<<n));};
 p.write(self+12,scalar(5,"BaseItem",p.read(self+12)));p.identity(self,p.read(self+20));U str=p.string(g,s,r,"Tag",status,self+0x28);p.tag(self+16,str);p.string_destroy(s);
 bit(0,scalar(0,"Identified",1));U loc=p.localized(g,s,r,"Description",status,self+0x270);p.localized_assign(self+0x278,loc);p.localized_destroy(s);loc=p.localized(g,s,r,"DescIdentified",status,self+0x278);p.localized_assign(self+0x270,loc);p.localized_destroy(s);loc=p.localized(g,s+8,r,"LocalizedName",status,self+0x280);p.localized_assign(self+0x280,loc);p.localized_destroy(s+8);
 U stack=scalar(2,"StackSize",p.read(self+0x28c))&65535;U fallback=(p.read(self+0x288)>>5)&0xffffff01;p.write(self+0x28c,stack);bit(5,scalar(0,"Stolen",fallback));p.write(self+0x294,scalar(4,"Upgrades",p.read(self+0x294)));bit(3,scalar(0,"Dropable",0));bit(4,scalar(0,"Pickpocketable",0));bit(6,scalar(0,"NonEquippable",(p.read(self+0x288)>>6)&0xffffff01));
 U model=scalar(0,"ModelVariation",p.byte(self+0x264))&255;if(!p.read(status)){model=scalar(0,"ModelPart1",model)&255;if(!model)model=1;}p.put_byte(self+0x264,model);
 if(p.byte(p.rule(p.read(self+12))+0x18)==1){p.put_byte(self+0x265,p.byte(p.rule(p.read(self+12))+0xaf));p.put_byte(self+0x266,scalar(0,"TextureVar",1));}
 U charges=scalar(0,"Charges",50)&255;p.write(self+0x258,charges);U maxcharges=scalar(0,"MaxCharges",charges)&255;fallback=(p.read(self+0x288)>>7)&0xffffff01;p.write(self+0x25c,maxcharges);bit(7,scalar(0,"NewItem",fallback));bit(8,scalar(0,"DELETING",(p.read(self+0x288)>>8)&0xffffff01));p.write(self+0x260,scalar(4,"AddCost",p.read(self+0x260)));if(p.read(p.rule(p.read(self+12))+0x70))p.container(self,g,r);p.write(self+0x108,scalar(0,"Plot",p.read(self+0x108))&255);p.continuation(self,g,r);
}}
