#include "5c06c8e15d-source_substring.h"
#include "dcaf3f57f3-allocator_live.h"
#include <cstring>
#include <new>
namespace startup_rand {
using String=kotor_reconstruction::StartupString;
String* __fastcall SubString005e6270(String* self,void*,String* output,
                                    std::int32_t start,std::int32_t length) {
    // Actual temporary String lifetime; the original inlines its two zero stores.
    String temporary;
    const volatile unsigned char* first=
        reinterpret_cast<const volatile unsigned char*>(self->data);
    if(first) {
        const volatile unsigned char* end=first;
        while(*end)++end;
        const std::uint32_t measured=static_cast<std::uint32_t>(end-first);
        if(start>=0 && static_cast<std::uint32_t>(start)<measured && length!=0) {
            // JGE at005E62E0 keeps positive length unchanged. Negative means
            // remaining source, unlike a general clamp-to-available policy.
            const std::uint32_t count=length<0
                ? measured-static_cast<std::uint32_t>(start)
                : static_cast<std::uint32_t>(length);
            temporary.capacity=count+1u;
            char* allocation=static_cast<char*>(live_operator_new(count+1u));
            // Original reloads self->data before publishing the temporary data.
            const char* copied_source=self->data+start;
            temporary.data=allocation;
            // strncpy pads an overlong positive request, then the original
            // appends its separate terminator at the full requested count.
            std::strncpy(allocation,copied_source,count);
            allocation[count]='\0';
        }
    }
    // This existing owned-copy shell starts the fresh output only at the copy
    // phase, without default-zeroing it. It calls genuine WindowCopy005e5b00.
    // The genuine temporary destructor runs after output construction.
    return ::new(static_cast<void*>(output)) String(temporary);
}
}
