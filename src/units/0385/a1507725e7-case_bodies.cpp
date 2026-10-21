#include "ccfda63995-case_bodies.h"
#include <malloc.h>
using kotor_lowercase::Locale;
using kotor_lowercase::dependencies;
extern "C" char* __cdecl c2da_current_strlwr_007135df(char* text) {
    Locale* locale=dependencies.getptd_006feedf()->locale;

    if(locale!=*dependencies.global_locale_007a2824)locale=dependencies.update_006fdd2a();

    const kotor_lowercase::Word query_ctype=locale->ctype_locale;
    if(query_ctype==0) {
        char* cursor=text;
        while(*cursor) {
            const signed char byte=static_cast<signed char>(*cursor);
            if(byte>=65 && byte<=90)*cursor=static_cast<char>(byte+32);
            ++cursor;
        }
        return text;
    }



    const kotor_lowercase::Word flags=0x100;

    const int required=dependencies.map_0070272d(query_ctype,flags,text,-1,nullptr,0,locale->codepage,1);
    if(!required)return text;
    char* buffer;
    __try {
        const kotor_lowercase::Word rounded=(static_cast<kotor_lowercase::Word>(required)+3u)&~3u;
        buffer=static_cast<char*>(_alloca(rounded));
    } __except(1) {
        c2da_case_stack_recover_00708b4f();
        buffer=nullptr;
    }
    bool heap=false;
    if(!buffer) {
        buffer=static_cast<char*>(live_malloc(static_cast<kotor_lowercase::Word>(required)));
        heap=true;
        if(!buffer) {
            live_free(buffer);
            return text;
        }
    }
    const kotor_lowercase::Word fill_codepage=locale->codepage;
    const kotor_lowercase::Word fill_ctype=locale->ctype_locale;
    const int mapped=dependencies.map_0070272d(fill_ctype,flags,text,-1,buffer,required,fill_codepage,1);



    if(mapped)reconstructed_strcpy_00705910(text,buffer);

    if(heap)live_free(buffer);
    return text;
}
extern "C" char* __cdecl c2da_current_strupr_007136f3(char* text) {
    Locale* locale=dependencies.getptd_006feedf()->locale;

    if(locale!=*dependencies.global_locale_007a2824)locale=dependencies.update_006fdd2a();

    const kotor_lowercase::Word query_ctype=locale->ctype_locale;
    if(query_ctype==0) {
        char* cursor=text;
        while(*cursor) {
            const signed char byte=static_cast<signed char>(*cursor);
            if(byte>=97 && byte<=122)*cursor=static_cast<char>(byte-32);
            ++cursor;
        }
        return text;
    }



    const kotor_lowercase::Word flags=0x200;

    const int required=dependencies.map_0070272d(query_ctype,flags,text,-1,nullptr,0,locale->codepage,1);
    if(!required)return text;
    char* buffer;
    __try {
        const kotor_lowercase::Word rounded=(static_cast<kotor_lowercase::Word>(required)+3u)&~3u;
        buffer=static_cast<char*>(_alloca(rounded));
    } __except(1) {
        c2da_case_stack_recover_00708b4f();
        buffer=nullptr;
    }
    bool heap=false;
    if(!buffer) {
        buffer=static_cast<char*>(live_malloc(static_cast<kotor_lowercase::Word>(required)));
        heap=true;
        if(!buffer) {
            live_free(buffer);
            return text;
        }
    }
    const kotor_lowercase::Word fill_codepage=locale->codepage;
    const kotor_lowercase::Word fill_ctype=locale->ctype_locale;
    const int mapped=dependencies.map_0070272d(fill_ctype,flags,text,-1,buffer,required,fill_codepage,1);



    if(mapped)reconstructed_strcpy_00705910(text,buffer);

    if(heap)live_free(buffer);
    return text;
}
