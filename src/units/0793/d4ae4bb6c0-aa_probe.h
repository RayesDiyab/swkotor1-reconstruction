#pragma once
#include "a39d5fa52e-renderer_initialize.h"
#include <windows.h>
namespace kotor_renderer_init {
using WindowProcedure = I32 (__stdcall*)(void*, U32, U32, I32);
using RawProcedure = void (__stdcall*)();
using LoadProcedure = RawProcedure (__stdcall*)(const char*);
using ExtensionString = const char* (__stdcall*)(void*);
using ChooseArb = I32 (__stdcall*)(void*, const I32*, const float*, U32, I32*, U32*);
struct WindowClass {
    U32 style; WindowProcedure procedure; I32 class_extra,window_extra;
    void* instance; void* icon; void* cursor; void* background;
    const char* menu; const char* class_name;
};
struct VersionInfo { U32 size,major,minor,build,platform; char service_pack[128]; };
static_assert(sizeof(WindowClass)==40 && offsetof(WindowClass,instance)==16 &&
    offsetof(WindowClass,class_name)==36,"WNDCLASSA x86 offsets");
static_assert(sizeof(VersionInfo)==148 && offsetof(VersionInfo,major)==4,
    "OSVERSIONINFOA x86 offsets");
// Every address below denotes borrowed canonical storage or immutable data.
struct AntiAliasingProbeBindings {
    HINSTANCE const volatile* instance_007a39c0;
    const WindowProcedure* def_window_proc_iat_0073d3b0;
    const char* class_name_0078e310;
    const char* window_title_0073f478;
    const char* extension_proc_name_0073f45c;
    const char* choose_proc_name_0073f444;
    const char* ati_extension_name_0073f42c;
    const char* multisample_name_0073f418;
    const LoadProcedure* load_proc_iat_0073d310;
    ExtensionString* extension_proc_007bb7ec;
    ChooseArb* choose_proc_007bb730;
    const char** extension_string_007a688c;
    U32* capabilities_007bb85c;
    const U32* multisample_mask_0078e4ec;
};
extern AntiAliasingProbeBindings& aa_probe;
extern void* __stdcall GetStockObject_0073d01c(I32);
extern U16 __stdcall RegisterClassA_0073d41c(const WindowClass*);
extern void* __stdcall CreateWindowExA_0073d410(U32,const char*,const char*,U32,
    I32,I32,I32,I32,void*,void*,void*,void*);
extern I32 __stdcall DestroyWindow_0073d3fc(void*);
extern const char* __stdcall glGetString_0073d290(U32);
extern I32 __stdcall GetVersionExA_0073d060(VersionInfo*);
// Original consumers test AL alone, including noncanonical nonzero byte values.
extern U8 __cdecl CheckExtension_0045f980(const char*,const char*);
}
