#include "7102fe53f2-startup_string.h"
#include "7deb550b61-allocator_live.h" // Unchanged task-23 dependency; integrator supplies it.
namespace kotor_reconstruction {
StartupString::StartupString() { data=nullptr;capacity=0; }
StartupString::StartupString(const char* volatile input) {
    const volatile unsigned char* source=
        reinterpret_cast<const volatile unsigned char*>(input);
    if(source) {
        const volatile unsigned char* cursor=source;
        while(*cursor)++cursor;
        const std::uint32_t length=static_cast<std::uint32_t>(cursor-source);
        // Original overwrites its argument slot with the first measured length.
        input=reinterpret_cast<const char*>(length);
        if(length) {
            cursor=source;
            while(*cursor)++cursor;
            const std::uint32_t count=static_cast<std::uint32_t>(cursor-source)+1u;
            capacity=count;
            data=static_cast<char*>(live_operator_new(count));
            volatile unsigned char* output=
                reinterpret_cast<volatile unsigned char*>(data);
            unsigned char value;
            do { value=*source++;*output++=value; } while(value);
            return;
        }
    }
    data=nullptr;capacity=0;
}
StartupString::~StartupString() {
    char* pointer=data;
    if(pointer) { capacity=0;live_free(pointer);data=nullptr; }
}
}
