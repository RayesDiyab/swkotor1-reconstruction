#include "cb2a31e7e3a6cb21_nls_helpers.h"
#include <malloc.h>
#include <string.h>
NlsGlobals nls_globals{};
NlsBindings nls_bindings{};
static const WCHAR probe_empty_0075a8e0=0;
static unsigned scratch_bytes(int count,unsigned width) {
    // Bounded source candidate: normal small allocations, no stack exhaustion,
    // original exception fallback, or size arithmetic wraparound claim.
    if(count<=0 || (unsigned)count>32768/width)
        nls_outside_contract("scratch extent outside bounded normal domain");
    return ((unsigned)count*width+3u)&~3u;
}
// These layouts alias physical incoming x86 caller slots, not copied parameters.
struct ClassSlots { uint32_t type; const char* input; int count; uint16_t* output; uint32_t codepage,locale; int strict; };
struct MapSlots { uint32_t locale,flags; const char* input; int count; char* output; int capacity; uint32_t codepage; int strict; };
static_assert(sizeof(ClassSlots)==28 && sizeof(MapSlots)==32 && sizeof(void*)==4,"x86 caller-slot layout");
extern "C" int __cdecl nls_classify_slot_core(ClassSlots* slots) {
    uint32_t type=slots->type,codepage=slots->codepage,locale=slots->locale;
    const char* input=slots->input;
    int& count=slots->count;
    uint16_t* output=slots->output;
    int strict=slots->strict;
    if(*nls_globals.classification_mode_00834034==0) {
        WORD temp;
        if(nls_bindings.type_w(1,&probe_empty_0075a8e0,1,&temp))
            *nls_globals.classification_mode_00834034=1;
        else if(nls_bindings.last_error()==120)
            *nls_globals.classification_mode_00834034=2;
    }
    uint32_t mode=*nls_globals.classification_mode_00834034;
    if(mode==1) {
        if(!codepage) {
            codepage=*nls_globals.locale_codepage_0083402c;
            slots->codepage=codepage; //00707472: Unicode branch only.
        }
        int needed=nls_bindings.to_wide(codepage,strict?9:1,input,count,nullptr,0);
        if(!needed)return 0;
        WCHAR* wide=(WCHAR*)_alloca(scratch_bytes(needed,2));
        memset(wide,0,(unsigned)needed*2);
        int actual=nls_bindings.to_wide(codepage,1,input,count,wide,needed);
        return actual?nls_bindings.type_w(type,wide,actual,output):0;
    }
    if(mode==0 || mode==2) {
        if(!locale)locale=*nls_globals.locale_id_0083401c;
        if(!codepage)codepage=*nls_globals.locale_codepage_0083402c;
        int target=nls_bindings.locale_cp_00709419(locale);
        if(target==-1)return 0;
        char* converted=nullptr;
        if((uint32_t)target!=codepage) {
            converted=nls_bindings.transcode_00709462(codepage,(uint32_t)target,input,&count,nullptr,0);
            if(!converted)return 0;
            input=converted;
            slots->input=converted; //00707584: published before ANSI API, retained after free.
        }
        int result=nls_bindings.type_a(locale,type,input,count,output);
        if(converted)nls_bindings.free_006fb7b2(converted);
        return result;
    }
    return 0;
}

extern "C" int __cdecl nls_map_slot_core(MapSlots* slots) {
    uint32_t locale=slots->locale,flags=slots->flags,codepage=slots->codepage;
    const char* input=slots->input;
    int& count=slots->count;
    char* output=slots->output;
    int capacity=slots->capacity,strict=slots->strict;
    if(*nls_globals.mapping_mode_00833ec4==0) {
        if(nls_bindings.map_w(0,0x100,&probe_empty_0075a8e0,1,nullptr,0))
            *nls_globals.mapping_mode_00833ec4=1;
        else if(nls_bindings.last_error()==120)
            *nls_globals.mapping_mode_00833ec4=2;
    }
    if(count>0) {
        int used=0;
        while(used<count && input[used])++used;
        count=used; // Excludes NUL; negative counts are passed through unchanged.
    }
    uint32_t mode=*nls_globals.mapping_mode_00833ec4;
    if(mode==1) {
        if(!codepage) {
            codepage=*nls_globals.locale_codepage_0083402c;
            slots->codepage=codepage; //007027CF or007029A4, before downstream calls.
        }
        int wide_count=nls_bindings.to_wide(codepage,strict?9:1,input,count,nullptr,0);
        if(!wide_count)return 0;
        WCHAR* wide=(WCHAR*)_alloca(scratch_bytes(wide_count,2));
        int actual=nls_bindings.to_wide(codepage,1,input,count,wide,wide_count);
        if(!actual)return 0;
        // Original passes the query count, not the second conversion's result.
        int mapped_count=nls_bindings.map_w(locale,flags,wide,wide_count,nullptr,0);
        if(!mapped_count)return 0;
        if(flags&0x400) {
            if(capacity!=0 && mapped_count<=capacity)
                nls_bindings.map_w(locale,flags,wide,wide_count,(WCHAR*)output,capacity);
            return mapped_count; // Original keeps query result even on fill failure.
        }
        WCHAR* mapped=(WCHAR*)_alloca(scratch_bytes(mapped_count,2));
        if(nls_bindings.map_w(locale,flags,wide,wide_count,mapped,mapped_count)==0)
            return mapped_count; // Deliberate observed retained-positive-result quirk.
        return nls_bindings.to_narrow(codepage,0,mapped,mapped_count,
            capacity?output:nullptr,capacity?capacity:0,nullptr,nullptr);
    }
    if(mode==0 || mode==2) {
        if(!locale) {
            locale=*nls_globals.locale_id_0083401c;
            slots->locale=locale; //00702997: ANSI branch only.
        }
        if(!codepage) {
            codepage=*nls_globals.locale_codepage_0083402c;
            slots->codepage=codepage; //007027CF or007029A4, before downstream calls.
        }
        int target=nls_bindings.locale_cp_00709419(locale);
        if(target==-1)return 0;
        if((uint32_t)target==codepage)
            return nls_bindings.map_a(locale,flags,input,count,output,capacity);
        char* converted=nls_bindings.transcode_00709462(codepage,(uint32_t)target,input,&count,nullptr,0);
        if(!converted)return 0;
        int required=nls_bindings.map_a(locale,flags,converted,count,nullptr,0);
        if(!required) {
            // Original00702AA5 reads EBP-48 without a write in this envelope.
            // Do not invent a zero/error/success result or claim this branch.
            nls_bindings.free_006fb7b2(converted);
            nls_outside_contract("00702AA5 indeterminate prior stack word");
        }
        char* mapped=(char*)_alloca(scratch_bytes(required,1));
        memset(mapped,0,(unsigned)required);
        int actual=nls_bindings.map_a(locale,flags,converted,count,mapped,required);
        int result=0;
        if(actual)result=nls_bindings.transcode_00709462((uint32_t)target,codepage,mapped,&actual,output,capacity)!=nullptr;
        nls_bindings.free_006fb7b2(converted);
        return result; // Original reduces reverse-transcode pointer to Boolean.
    }
    return 0;
}

// Both cdecl bridges preserve caller cleanup and pass the incoming array itself.
extern "C" __declspec(naked) int __cdecl nls_classify_007073f8(uint32_t,const char*,int,uint16_t*,uint32_t,uint32_t,int) {
    __asm { lea eax,[esp+4] }
    __asm { push eax }
    __asm { call nls_classify_slot_core }
    __asm { add esp,4 }
    __asm { ret }
}
extern "C" __declspec(naked) int __cdecl nls_map_0070272d(uint32_t,uint32_t,const char*,int,char*,int,uint32_t,int) {
    __asm { lea eax,[esp+4] }
    __asm { push eax }
    __asm { call nls_map_slot_core }
    __asm { add esp,4 }
    __asm { ret }
}
