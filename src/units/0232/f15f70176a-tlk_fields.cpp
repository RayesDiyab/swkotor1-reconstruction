#include "e347588a95-startup_views.h"
#include "b593572262-m50_vtables.h"   // lane-m50-vtables W-4 scan: lane-tlk typed CTlkTable table address
namespace kotor_menu_10_19 {
Tlk* Tlk::construct_0041d8d0(){
    write8(this,0x3c,0);
    // 0041D8E3 mov [esi],0073ECDCh: lane-tlk typed CTlkTable table (D44) via the lane-m50-vtables address constant.
    write32(this,0,kotor_vtable_word(kotor_vtable_address_ctlktable_0073ecdc));
    for(Word off=4;off<0x3c;off+=4)write32(this,off,0);
    write32(this,0x40,0);write32(this,0x44,0);write32(this,0x48,0);write32(this,0x4c,0);



    write32(this,0x54,0);return this;
}
Word Tlk::language_0041e0f0(){
    Word file=read32(this,4);



    return file?read32(reinterpret_cast<void*>(file),0x0c):0;

}
}
