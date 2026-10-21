#include "66f829b3cbd72280_stats_read.hpp"
#include <cstring>
namespace recovered_save_stats {
namespace{unsigned rd(void*p,unsigned o){unsigned x;std::memcpy(&x,(char*)p+o,4);return x;}std::uint16_t half(void*p,unsigned o){std::uint16_t x;std::memcpy(&x,(char*)p+o,2);return x;}}
std::uint16_t maximum_hp_00647a80(void*self){return half(self,0x4e);}
std::uint16_t spell_count_006487d0(void*self,unsigned cls,unsigned level){cls=static_cast<unsigned char>(cls);level=static_cast<unsigned char>(level);if(cls>=2)return 0;return half(self,cls*32+(level+18)*12);}
std::uint32_t spell_006495a0(void*self,unsigned cls,unsigned level,unsigned index,Pointer pointer){cls=static_cast<unsigned char>(cls);level=static_cast<unsigned char>(level);index=static_cast<unsigned char>(index);if(cls>=2||level>=1)return 0xffffffffu;void*list=(char*)self+0xd4+cls*32+level*12;if(static_cast<int>(index)>=static_cast<std::int32_t>(rd(list,4)))return 0xffffffffu;return rd(pointer(list,0),index*4);}
}
