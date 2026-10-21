#include "25d825b048-live_extensions.h"
namespace {
using namespace kotor_renderer_init;
I32 __stdcall ChooseLive(void* dc,const I32* ints,const float* floats,U32 maximum,I32* format,U32* count){




    return (*aurora_context_adoption::live_extensions.choose_007bb730)(dc,ints,floats,maximum,format,count);

}
I32 __stdcall SwapLive(I32 value){return (*aurora_context_adoption::live_extensions.swap_007bb538)(value);}
}
namespace kotor_renderer_init {
// Preserve the sealed donor declarations while forwarding each invocation to
// the then-current extension value. No historical address is called directly.
const decltype(choose_arb_007bb730) choose_arb_007bb730=&ChooseLive;
const decltype(swap_interval_007bb538) swap_interval_007bb538=&SwapLive;
}
