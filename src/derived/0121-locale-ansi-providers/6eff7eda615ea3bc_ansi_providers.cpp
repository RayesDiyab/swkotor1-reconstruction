#include "749b42b31e93ee50_ansi_providers.h"
#include <malloc.h>
#include <string.h>
AnsiProviderBindings ansi_bindings{};
extern "C" int __cdecl ansi_locale_cp_00709419(uint32_t locale) {
    char text[7];text[6]=0;
    if(!ansi_bindings.locale_info(locale,0x1004,text,6))return -1;
    return ansi_bindings.atoi_006faf5f(text);
}
extern "C" char* __cdecl ansi_transcode_00709462(uint32_t source_cp,uint32_t target_cp,
    const char* source,int* length,char* output,int capacity) {
    const int initial_length=*length;
    if(source_cp==target_cp)return nullptr; // Deliberate original no-conversion result.
    CPINFO info;
    bool fixed_width=ansi_bindings.cp_info(source_cp,&info)!=0 && info.MaxCharSize==1;
    if(fixed_width)fixed_width=ansi_bindings.cp_info(target_cp,&info)!=0 && info.MaxCharSize==1;
    int wide_count;
    if(fixed_width)wide_count=initial_length==-1?(int)ansi_bindings.strlen_00701300(source)+1:initial_length;
    else {
        wide_count=ansi_bindings.to_wide(source_cp,1,source,initial_length,nullptr,0);
        if(!wide_count)return nullptr;
    }
    // Only bounded normal stack allocation; historical stack-failure fallback
    // to calloc(2,wide_count), original cookie and generic SEH stay excluded.
    if(wide_count<0 || wide_count>16384)ansi_outside_contract("wide scratch outside normal32768-byte domain");
    WCHAR* wide=(WCHAR*)_alloca(((unsigned)wide_count*2+3u)&~3u);
    memset(wide,0,(unsigned)wide_count*2);
    if(!ansi_bindings.to_wide(source_cp,1,source,initial_length,wide,wide_count))return nullptr;
    if(output) {
        int result=ansi_bindings.to_narrow(target_cp,0,wide,wide_count,output,capacity,nullptr,nullptr);
        return result?output:nullptr; // No caller length update on supplied output.
    }
    int bytes=wide_count;
    if(!fixed_width) {
        bytes=ansi_bindings.to_narrow(target_cp,0,wide,wide_count,nullptr,0,nullptr,nullptr);
        if(!bytes)return nullptr;
    }
    // Original reuses its byte-query result as final wide-input count. Preserve
    // the quirk only where all consumed elements are within initialized storage.
    if(bytes<0 || bytes>wide_count)ansi_outside_contract("original byte/wide count reuse would read outside initialized scratch");
    char* allocated=(char*)ansi_bindings.calloc_006fb823(1,(unsigned)bytes);
    if(!allocated)return nullptr;
    int result=ansi_bindings.to_narrow(target_cp,0,wide,bytes,allocated,bytes,nullptr,nullptr);
    if(!result) {ansi_bindings.free_006fb7b2(allocated);return nullptr;}
    if(initial_length!=-1)*length=result;
    return allocated;
}
