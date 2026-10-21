#include "a2fb69b91934c90c_creature_effect_index.hpp"
namespace kotor::server_mainloop {
void creature_effect_index_004f14f0(Memory& m, Word self) {
    static constexpr Word cells[] = {0x74673c,0x7466a4,0x7466ac,0x746684,0x7466b0,0x7466d8,0x746788,0x746738,0x746778,0x74670c,0x746700,0x746794,0x746798,0x7467a8,0x7467ac,0x7467a0,0x746764,0x746780,0x746744,0x746770,0x746758,0x7466e4,0x746680,0x7466d4,0x746768,0x7466f0};
    static constexpr Word offsets[] = {0x12e,0x130,0x132,0x134,0x136,0x138,0x13c,0x140,0x13e,0x144,0x142,0x146,0x148,0x14a,0x14c,0x14e,0x150,0x152,0x154,0x156,0x158,0x15a,0x15c,0x15e,0x160,0x162};
    Word previous=m.read(0x746830), count=m.read(self+0x128), index=0;
    if(static_cast<int>(count)<=0) return;
    do {
        Word type=m.read16(m.read(m.read(self+0x124)+index*4)+8);
        if(type!=previous) {
            for(unsigned j=0;j<26;++j) {
                if(type==m.read(cells[j])) {
                    m.write16(m.read(self+0xa74)+offsets[j],index&65535);
                    break;
                }
            }
        }
        previous=type;
        count=m.read(self+0x128);
        ++index;
    } while(static_cast<int>(index)<static_cast<int>(count));
}
}
