#include "e42bc01111-dependencies.h"
#include "00277387d3-server_option_data.h"
#include <initializer_list>
namespace kotor_menu_10_19 {
static void Option(ServerInfo* p,Word slot,Word family,Word label,Word offset,Word type){
    Word start=0x10c+slot*16;
    write32(p,start,family);write32(p,start+4,label);



    Word pointer=offset?reinterpret_cast<Word>(p)+offset:0;

    write32(p,start+8,pointer);write32(p,start+12,type);
}
ServerInfo* ServerInfo::construct_0054eb10(){
    for(Word offset:{8U,0x10U,0x18U,0x20U})reinterpret_cast<String*>(bytes+offset)->default_005b3190();
    write32(this,0xf0,0);write32(this,0xf4,1);write32(this,0xf8,0);
    reinterpret_cast<String*>(bytes+0xfc)->default_005b3190();
    Word flags=read32(this,0x104)&0xfffffff8U;
    (void)flags;
    write32(this,0,0);write16(this,4,0);write32(this,0x90,1);write32(this,0xc4,1);write32(this,0x98,1);



    write32(this,0x104,flags);

    Option(this,0,reinterpret_cast<Word>(dependencies::server_undefined_00748794),reinterpret_cast<Word>(dependencies::server_undefined_option_00748784),0,0);
    Word family=dependencies::server_option_globals_007a1e10[0];
    for(Word slot=1;slot<=28;++slot){
        if(slot==4){write32(this,0x94,20);write32(this,0x108,2);}
        Word offset=0x28+(slot-1)*4;
        Option(this,slot,family,dependencies::server_option_globals_007a1e10[slot],offset,slot<23?1U:2U);
    }
    Word family2=dependencies::server_option_globals_007a1e10[29];
    for(Word slot=29;slot<=38;++slot)Option(this,slot,family2,dependencies::server_option_globals_007a1e10[slot+1],0x9c+(slot-29)*4,slot<36?1U:2U);
    family=dependencies::server_option_globals_007a1e10[46];
    for(Word slot=39;slot<=43;++slot)Option(this,slot,family,dependencies::server_option_globals_007a1e10[slot+8],0xcc+(slot-39)*4,1);
    family=dependencies::server_option_globals_007a1e10[52];
    for(Word slot=44;slot<=47;++slot)Option(this,slot,family,dependencies::server_option_globals_007a1e10[slot+9],0xe0+(slot-44)*4,slot==47?2U:1U);
    Option(this,48,reinterpret_cast<Word>(dependencies::server_undefined_00748794),reinterpret_cast<Word>(dependencies::server_undefined_option_00748784),0,0);
    Option(this,49,family2,dependencies::server_option_globals_007a1e10[45],0xc4,1);
    return this;
}
}
