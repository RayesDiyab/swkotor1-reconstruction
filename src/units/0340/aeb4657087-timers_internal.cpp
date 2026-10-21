#include "cdd98abf6f-timers_constructor.h"
#include <new>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This source preserves the original x87 operations with MSVC x86.
#endif
namespace kotor_startup {
// Source-private constants retain the original double bit values, independently
// pinned from0074E5A0 and0073EDE8. They own no original-address storage.
static const double scale=1.0e-6;
static const double one=1.0;
static void StorePeriod(TimersInternal* self,const LARGE_INTEGER* frequency,int small_mode) {
    // FILD is signed64 and exact. FMUL/FDIVR obey the caller's current x87 CW;
    // FSTP rounds into the double field. Do not replace this with SSE arithmetic.
    __asm {
        mov eax,frequency
        fild qword ptr [eax]
        cmp small_mode,0
        je arithmetic
        mov eax,self
        mov dword ptr [eax+4],1
    arithmetic:
        fmul qword ptr scale
        fdivr qword ptr one
        mov eax,self
        fstp qword ptr [eax+24]
    }
}
TimersInternal* __fastcall ConstructTimersInternal005ec160(TimersInternal* storage,void*) {
    auto* self=::new(static_cast<void*>(storage)) TimersInternal;
    LARGE_INTEGER frequency;
    const BOOL available=timer_imports.query_performance_frequency(&frequency);
    self->supported=available;
    if(available==1) {
        self->frequency_low=frequency.LowPart;
        self->frequency_high=static_cast<std::uint32_t>(frequency.HighPart);
        self->mode=0;
        // Source compiler division primitives implement these operations; this
        // does not recover or claim original CRT006FB030/006FC660 bodies.
        const auto unsigned_quotient=static_cast<std::uint64_t>(frequency.QuadPart)/1000000u;
        if(unsigned_quotient<200u) {
            StorePeriod(self,&frequency,1);
        } else {
            StorePeriod(self,&frequency,0);
            const auto signed_quotient=frequency.QuadPart/1000000;
            self->quotient_low=static_cast<std::uint32_t>(signed_quotient);
            self->quotient_high=static_cast<std::uint32_t>(static_cast<std::uint64_t>(signed_quotient)>>32);
        }
    }
    return self;
}
}
