#include "b6a6f59d48-startup_views.h"
namespace kotor_menu_10_19 {
WorldTimer* WorldTimer::construct_004ae4c0(){
    write32(this,0,0); write8(this,0x38,5); write32(this,0x40,0x1c20);
    write32(this,0x3c,0x6ddd00); write32(this,8,100);
    auto timer=reinterpret_cast<TimerDependency*>(read32(dependencies::base_007a39e0,4));
    Wide first=timer->high_resolution_005e9260();
    write32(this,0x0c,static_cast<Word>(first)); write32(this,0x10,static_cast<Word>(first>>32));
    // Re-read the shared base and nested timer after the first dependency call.
    timer=reinterpret_cast<TimerDependency*>(read32(dependencies::base_007a39e0,4));
    Wide second=timer->high_resolution_005e9260();
    Word first_high=read32(this,0x10);
    write32(this,0x1c,static_cast<Word>(second));
    Word first_low=read32(this,0x0c);
    write32(this,0x20,static_cast<Word>(second>>32));
    write32(this,0x14,first_low); write32(this,0x18,first_high);
    Wide quotient=dependencies::unsigned_divide_006fb030((Wide(first_high)<<32)|first_low,1000);
    Word divisor=read32(this,0x3c); Divmod result;
    dependencies::divmod_adapter(quotient,divisor,&result);



    Word remainder=result.remainder_low;

    Word day=0x6df4cU-result.quotient_low;
    Word ms=Word(read8(this,0x38))*0x150ea0U-remainder+0x36dee0U;
    write32(this,0x24,0); write32(this,0x34,ms); write32(this,0x30,day);
    return this;
}
}
