#include "b6a6f59d48-startup_views.h"
namespace kotor_menu_10_19 {
ObjectArray* ObjectArray::construct_004d7d80(int mode){
    write32(this,0x14,static_cast<Word>(mode));



    Word extent=mode?0x8000U:0x4000U;

    write32(this,0,reinterpret_cast<Word>(dependencies::allocate_006fa7e6(extent)));
    // The original performs no allocation-null test and reloads its slot in the loop.
    for(Word off=0;off<extent;off+=4)write32(reinterpret_cast<void*>(read32(this,0)),off,0);
    for(Word off=0x0c;off<0x14;off+=4){write32(this,off-8,0);write32(this,off,0x7fffffffU);}
    return this;
}
}
