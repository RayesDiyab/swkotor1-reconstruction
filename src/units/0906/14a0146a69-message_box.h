#pragma once
#include "2513c38919-runtime_reporter.h"
#include <cstdint>

namespace kotor_message_box_dependencies {
using MessageBoxProc=int (WINAPI*)(HWND,LPCSTR,LPCSTR,UINT);
using ActiveWindowProc=HWND (WINAPI*)();
using LastPopupProc=HWND (WINAPI*)(HWND);
using StationProc=HWINSTA (WINAPI*)();
using UserInformationProc=BOOL (WINAPI*)(HANDLE,int,void*,DWORD,DWORD*);
struct Cache {
    MessageBoxProc volatile message_box;        //00834064
    ActiveWindowProc volatile active_window;   //00834068
    LastPopupProc volatile last_popup;          //0083406C
    StationProc volatile process_station;      //00834070
    UserInformationProc volatile user_information; //00834074
};
static_assert(sizeof(Cache)==20 && sizeof(void*)==4,"Original five x86 cache words");
static_assert(offsetof(Cache,process_station)==12 && offsetof(Cache,user_information)==16,"Original cache order");
static_assert(sizeof(USEROBJECTFLAGS)==12 && offsetof(USEROBJECTFLAGS,dwFlags)==8,"Original UOI_FLAGS data");
extern Cache cache;
// Source aliases must come from the existing task36 entry-state owner.
extern const volatile std::uint32_t* platform_00833be8;
extern const volatile std::uint32_t* major_00833bf4;
HMODULE WINAPI load_library_0073d07c(LPCSTR);
FARPROC WINAPI resolve_0073d1d4(HMODULE,LPCSTR);
}

