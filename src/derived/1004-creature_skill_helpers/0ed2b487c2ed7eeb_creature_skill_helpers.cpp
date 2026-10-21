#include "6d454fe80390de2d_creature_skill_helpers.hpp"
namespace kotor::server_mainloop {
Word creature_skill_penalty_004ee210(Memory&m,Word self){Word flags=m.read8(self+0x8ec);return (flags&16)||((flags&8)&&!(flags&2));}
Word creature_dex_restricted_005b4880(Memory&m,CreatureSkillHelperPorts&p,Word self){return m.read8(self+0x8ed)||p.incapacitated(self)?1:0;}
Word server_area_004b18d0(Memory&m,CreatureSkillHelperPorts&p,Word self,Word id,Word out){Word manager=m.read(self+0x1005c);m.write(out,0);Word status=p.lookup(manager,id,out);if((status&255)!=m.read8(0x74666c))return 0;Word object=m.read(out);if(!object||m.read8(object+8)!=m.read8(0x746638))return 0;return p.area_virtual(m.read(m.read(object)+0x48),object);}
}
