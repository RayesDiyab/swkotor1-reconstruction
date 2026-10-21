#include "f56b43bb86-source.h"
extern "C" __declspec(noinline) std::uint32_t __cdecl renderer_system_00425c30(){
    auto& v=renderer_system::bindings();



    *v.context_007a6854=v.get_context_0073d2e0();*v.dc_007a47e4=v.get_dc_0073d2dc();




    for(unsigned i=0;i<6;++i)v.cache6_007a683c[i]=0;

    *v.zero_007a6898=0;



    return 0; // Observed EAX0, not an invented success/failure interpretation.

}
