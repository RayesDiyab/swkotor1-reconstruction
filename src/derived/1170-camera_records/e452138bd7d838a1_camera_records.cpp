#include "4dec4b1bc7d4dc7e_camera_records.hpp"
namespace recovered_camera_records{
U count(U self,U n,Ports&p){if(static_cast<std::int32_t>(n)>50)return 0;p.write(self+0x1b0,n);return 1;}
void insert(U self,U index,U id,U position,U quaternion,U a,U b,U c,U value,Ports&p){if(static_cast<std::int32_t>(index)>=static_cast<std::int32_t>(p.read(self+0x1b0)))return;U final=p.float_spill(value),base=self+index*48;p.write(base+0x1b4,id);for(U i=0;i<3;++i)p.write(base+0x1b8+4*i,p.read(position+4*i));for(U i=0;i<4;++i)p.write(base+0x1c4+4*i,p.read(quaternion+4*i));p.write(base+0x1d4,a);p.write(base+0x1d8,b);p.write(base+0x1dc,c);p.write(base+0x1e0,final);}
}
