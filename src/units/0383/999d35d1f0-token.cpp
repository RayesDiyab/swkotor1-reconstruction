#include "8e678dc65e-load_imports.h"

// 004137E0..004138A1, normal serial writable-buffer domain. The real Load
// caller carries Table in ECX; this body never dereferences the receiver.
extern "C" __declspec(noinline) int __fastcall
c2da_current_token_004137e0(c2da_current::Table*,void*,char** data,
    c2da_current::Word* remaining,char* output,c2da_current::Word* leading) {
    using Word=c2da_current::Word;
    if(*remaining==0) {
        *output=0;
#ifdef WRONG_ZERO_LEADING
        *leading=0;
#endif
        return 0;
    }
    char* cursor=*data;
    Word consumed=0,spaces=0;
    if(*cursor==0) {
        ++cursor;
#ifdef WRONG_CHARGE_INITIAL_NUL
        ++consumed;
#endif
    }
    // The original's TAB comparison lies outside the unsigned size test.
#ifdef WRONG_BOUND_TABS
    while(spaces<*remaining && (*cursor==' ' || *cursor=='\t')) {
#else
    while((spaces<*remaining && *cursor==' ') || *cursor=='\t') {
#endif
        ++cursor;++spaces;
    }
    *leading=spaces;
    *remaining-=spaces;
    if(*cursor=='"') {
        ++cursor;
#ifdef WRONG_CHARGE_OPEN_QUOTE
        ++consumed;
#endif
        if(*remaining!=0) {
            do {
                const char byte=*cursor;
                if(byte==0 || byte=='"')break;
                *output=byte;++output;++cursor;++consumed;
            } while(consumed<*remaining);
        }
        if(*cursor=='"') {
            ++cursor;
#ifndef WRONG_IGNORE_CLOSE_QUOTE
            ++consumed;
#endif
        }
    } else if(*remaining!=0) {
        do {
            const char byte=*cursor;
            if(byte==0 || byte==' ' || byte=='\t' || byte=='\n' || byte=='\r')break;
            *output=byte;++output;++cursor;++consumed;
        } while(consumed<*remaining);
    }
    *data=cursor;
    *remaining-=consumed;
    *output=0;
#ifdef WRONG_LENGTH_RESULT
    return static_cast<int>(consumed);
#else
    return consumed!=0?1:0;
#endif
}
