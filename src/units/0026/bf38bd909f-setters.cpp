#include "ab77de67ab-level.hpp"
namespace kotor::character_level_stats {
namespace {
std::int32_t signed_bits(Word n){return n<0x80000000u?static_cast<std::int32_t>(n):-1-static_cast<std::int32_t>(~n);}
}
void set_skill_005cb120(LevelMemory& m,Dependencies& d,Word self,Word skill_bits,Word change_bits) {
    const Word rules=d.rules_007a3a28();
    const Word count=m.read_byte(rules+0xab);
    const Word skill=static_cast<std::uint16_t>(skill_bits);
    if(skill<count)m.write_byte(m.read(self+0x24)+skill,static_cast<std::uint8_t>(change_bits));
}
void add_feat_005cb150(LevelMemory& m,Dependencies& d,Word self,Word feat_bits) {
    const Word list=self+0x18;
    const Word count=m.read(list+4);
    const auto feat=static_cast<std::uint16_t>(feat_bits);
    Word matches=0;
    if(signed_bits(count)>0) {
        Word p=m.read(list);
        for(Word remaining=count;remaining!=0;--remaining,p+=2)if(m.read_short(p)==feat)++matches;
        if(matches!=0)return;
    }
    const Word capacity=m.read(list+8);
    if(m.read(list+4)==capacity)d.grow_u16_004d2fa0(list,capacity!=0?capacity+capacity:16u);
    const Word index=m.read(list+4);
    const Word data=m.read(list);
    m.write_short(data+index*2u,feat);
    m.write(list+4,m.read(list+4)+1u);
}
}
