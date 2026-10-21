#include "8e678dc65e-load_imports.h"

extern "C" __declspec(noinline) c2da_current::Word __fastcall
c2da_current_skip_lines_00413940(c2da_current::Table*,void*,char** data,
    c2da_current::Word* remaining) {
    using Word=c2da_current::Word;
    char* const start=*data;
    char* cursor=start;
    const Word limit=*remaining;
    Word committed=0,scanned=0;
    if(limit!=0) {
        for(;;) {
            const char byte=*cursor;
            if(byte!='\n' && byte!='\r' && byte!=' ' && byte!='\t')break;
            committed=scanned;
            if(scanned<limit) {
                while(*cursor=='\n' || *cursor=='\r') {
                    ++cursor;++committed;
                    if(committed>=*remaining)break;
                }
            }
            scanned=committed;
            if(committed<limit) {
                while(*cursor==' ' || *cursor=='\t') {
                    ++cursor;++scanned;
                    if(scanned>=*remaining)break;
                }
            }
            if(scanned==limit) {
                committed=scanned;break;
            }
            if(committed>=*remaining)break;
        }
    }
#ifdef WRONG_COMMIT_INDENT
    committed=scanned;
#endif
#ifdef WRONG_DROP_CR
    // Deliberate semantic negative: fail the first observed CR case.
    if(limit && *start=='\r')committed=0;
#endif
    // Even zero size writes the incoming pointer and remaining word. Avoid
    // forming a null-plus-zero C++ expression for the valid empty-buffer case.
    *data=committed?start+committed:start;
    *remaining-=committed;
#ifdef WRONG_COUNT_RESULT
    return committed;
#else
    return committed!=0?1u:0u;
#endif
}
