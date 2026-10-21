#include "433aa7d9880ae99d_area_map_load.hpp"
namespace kotor::server_mainloop{Word area_map_load_00505da0(Memory&m,AreaMapLoadPorts&p,Word self,Word gff,Word root,Word s){m.write(s+4,1);if(!p.structure(gff,s+0x14,root,0x747560))return 0;return p.present(self,gff,s+0x14,s);}}
