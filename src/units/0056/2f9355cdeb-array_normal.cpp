#include "6bff443bba-array_iterator.h"

// Existing 0366 lower owner and ABI. This declaration adds no provider or storage.
namespace menu_tables_m41 {
using Destructor=kotor_array_iterator::Destroy;
void __stdcall ArrayUnwind_006fa7f4(void*,std::uint32_t,std::uint32_t,Destructor);
}

// Original 006FA852 reverse DEC/JS iterator, including 006FA89A finally cleanup.
// The generic element callback and all existing normal-path mutant controls remain.
extern "C" __declspec(noinline) void __stdcall c2da_destroy_array_006fa852(
    void* begin,std::uint32_t stride,std::uint32_t count,
    kotor_array_iterator::Destroy destroy) {





    volatile std::uint32_t remaining=count;
    volatile std::uint32_t address=reinterpret_cast<std::uint32_t>(begin)+stride*remaining;
    volatile int completed=0;
    __try {




        for (;;) {
            --remaining; // Original DEC/JS wraps, including count==80000000; no signed UB.
            if (remaining&0x80000000u)break;
            address-=stride;
            destroy(reinterpret_cast<void*>(address),nullptr);
        }

        completed=1; // 006FA882: normal completion suppresses the finally cleanup.
    } __finally {
        if(completed==0) // 006FA89A: failed element is excluded; lower starts below current address.
            menu_tables_m41::ArrayUnwind_006fa7f4(
                reinterpret_cast<void*>(address),stride,remaining,
                destroy);
    }
}
