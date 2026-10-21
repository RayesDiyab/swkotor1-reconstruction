#include "8e678dc65e-load_imports.h"

// The two actual Load sites preserve ECX=Table. This original callee never
// reads that register as a receiver and has no object/global/lower dependency.
extern "C" __declspec(noinline) c2da_current::Word __fastcall
c2da_current_line_end_004137b0(c2da_current::Table*,void*,char* data,c2da_current::Word size) {
    c2da_current::Word count=0;
#ifdef WRONG_SIGNED_LIMIT
    if(static_cast<std::int32_t>(size)<=0)return count;
#endif
    while(count<size) {
        const char byte=data[count];
        if(byte=='\n' || byte=='\r') {
#ifdef WRONG_CONSUME_TERMINATOR
            ++count;
#endif
            break;
        }
#ifdef WRONG_STOP_AT_NUL
        if(byte==0)break;
#endif
        ++count;
    }
#ifdef WRONG_TRUNCATE_RETURN
    return count&255u;
#else
    return count;
#endif
}
