#include "c8b4fb84c7e53d2e_player_message.hpp"
#include <cstring>
namespace recovered_player_message {
Route route(unsigned char op){switch(op){
case 1:return {0x00524c40u,0x00745094u,false}; // ServerStatus
case 2:return {0x005244e0u,0x00748258u,false}; // Login
case 3:return {0x00524800u,0x007442a8u,false}; // Module
case 4:return {0x00524b80u,0x007442b0u,false}; // Area
case 5:return {0x005228d0u,0x0074819cu,false}; // GameObjUpdate
case 6:return {0x005254c0u,0x00748250u,false}; // Input
case 8:return {0x00523b20u,0x00748248u,false}; // Gold
case 9:return {0x00520a10u,0x00748240u,false}; // Chat
case 11:return {0x00000000u,0x00748240u,true}; // Chat
case 12:return {0x00523c20u,0x00748234u,false}; // Inventory
case 13:return {0x00523240u,0x00748224u,false}; // GuiInventory
case 14:return {0x00526910u,0x0074820cu,false}; // Party
case 15:return {0x00520bf0u,0x00748204u,false}; // Cheat
case 17:return {0x005246f0u,0x007481f8u,false}; // CharList
case 20:return {0x00522810u,0x007481f0u,false}; // Dialog
case 21:return {0x00523380u,0x007481dcu,false}; // GuiCharacterSheet
case 22:return {0x005209b0u,0x007481d0u,false}; // QuickChat
case 25:return {0x005232a0u,0x00748214u,false}; // GuiContainer
case 28:return {0x005278d0u,0x007481c0u,false}; // Journal
case 29:return {0x00527560u,0x007481c8u,false}; // LevelUp
case 30:return {0x00000000u,0x007481b4u,true}; // GuiQuickbar
case 32:return {0x00527380u,0x007481acu,false}; // MapPin
case 37:return {0x005252a0u,0x00748194u,false}; // Death
case 43:return {0x00525350u,0x00748180u,false}; // Character_Download
case 47:return {0x00525390u,0x00748170u,false}; // ShutDownServer
case 49:return {0x00527890u,0x00748158u,false}; // PlayModuleCharacterList
default:return {0,0,false};}}
namespace{unsigned rd(void*p,unsigned o){unsigned v;std::memcpy(&v,static_cast<unsigned char*>(p)+o,4);return v;}void wr(void*p,unsigned o,unsigned v){std::memcpy(static_cast<unsigned char*>(p)+o,&v,4);}}
void initialize_read_004d66b0(void*self,void*data,unsigned size,unsigned a,unsigned b){wr(self,0x4c,a);wr(self,0x28,static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(data)));wr(self,0x48,b);wr(self,0x2c,size);wr(self,0x30,0);wr(self,0x38,0);wr(self,0x3c,0);if(size==0)static_cast<unsigned char*>(self)[0x44]=static_cast<unsigned char*>(self)[0x45];}
unsigned overrun_004d6230(void*self){return rd(self,0x2c)<rd(self,0x30);}
unsigned underrun_004d6240(void*self){return rd(self,0x30)<rd(self,0x2c);}
unsigned handle_00527b20(void*self,unsigned id,void*packet,unsigned size,Ports&p){
 String name;p.string_construct(&name);auto*bytes=static_cast<unsigned char*>(packet);const unsigned char major=bytes[1],minor=bytes[2],tag=bytes[0];
 p.initialize_read(self,bytes+3,size-3,0xffffffffu,1);
 if(p.overrun(self,1)!=0||tag!=0x70){p.string_destroy(&name);return 0;}
 void*player=p.player(id,1);if(!player){p.string_destroy(&name);return 1;}
 p.format_unknown(&name,major);unsigned result=0;const Route r=route(major);
 if(r.label){p.assign_label(&name,r.label);if(r.constant_one)result=1;else result=p.handler(r.target,self,player,minor);}
 String error;p.string_construct(&error);
 if(p.overrun(self,0)!=0)p.format_problem(&error,true,p.text(&name),minor);
 if(p.underrun(self,0)!=0)p.format_problem(&error,false,p.text(&name),minor);
 p.string_destroy(&error);p.string_destroy(&name);return result;
}
}
