#include "fd0064c12cba6825_creature_tag.hpp"
namespace kotor::server_mainloop {void creature_tag_00553040(CreatureTagPorts&p,Word self,Word text,Word scratch){Word value=p.lower(text,scratch);p.assign(self+0x18,value);p.destroy(scratch);}}
