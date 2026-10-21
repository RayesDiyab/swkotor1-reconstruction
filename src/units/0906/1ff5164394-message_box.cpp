#include "14a0146a69-message_box.h"
// lane-m48-b crt ADAPTED COPY of task-25 reporter-messagebox/source/message_box.cpp (accepted task-35 reporter-review,
// new credit 007092DE ___crtMessageBoxA). Control flow, slot publication order, the stored-flag write into the third
// argument slot and the final MessageBoxA reread are unchanged. Two adaptations, both recorded in crt/FORK-PACKET.json:
//  (1) the donor resolved user32 exports with LoadLibraryA/GetProcAddress and reinterpret_cast<FARPROC> (a function-
//      pointer cast, forbidden by WORKER-BRIEF). user32 is a static import of both the original (MessageBoxA etc. are
//      also reached via its own IAT elsewhere) and this build, so the cache slots are published with the typed address of
//      the same exports (&::MessageBoxA ...). Equivalence: LoadLibraryA("user32.dll") (007092F6) of an already-loaded
//      static import returns its module and GetProcAddress (0070930C..00709357) returns those same export addresses; the
//      null-module (00709300) and null-MessageBoxA (0070931B) failure exits are therefore unreachable in this build.
//  (2) _osplatform 00833BE8 / _winmajor 00833BF4 are written by the original WinMainCRTStartup from GetVersionExA
//      (006FB3B6/006FB3BF). This build starts through the modern CRT, so no original writer runs; the two words are
//      obtained from the same GetVersionExA source at the original read points (00709332, 0070938A).
#include <cstring>
namespace {
OSVERSIONINFOA crt_version() {
    OSVERSIONINFOA info;
    std::memset(&info, 0, sizeof info);
    info.dwOSVersionInfoSize = sizeof info;
#pragma warning(push)
#pragma warning(disable:4996) // GetVersionExA: the original CRT's own source of these words (006FB3A0..006FB3BF)
    ::GetVersionExA(&info);
#pragma warning(pop)
    return info;
}
}
namespace kotor_runtime_reporter_dependencies {
int __cdecl message_box_007092de(const char* text,const char* caption,unsigned flags) {
    namespace d=kotor_message_box_dependencies;
    HWND owner=nullptr;
    if(d::cache.message_box==nullptr) {                                              // 007092E7
        d::cache.message_box=&::MessageBoxA;                                           // 00709316
        d::cache.active_window=&::GetActiveWindow;                                     // 0070932B
        const d::LastPopupProc resolved_popup=&::GetLastActivePopup;                   // 00709330
        const bool nt_platform=crt_version().dwPlatformId==2u;                         // 00709332 [00833BE8]==2
        d::cache.last_popup=resolved_popup;                                            // 00709339
        if(nt_platform) {
            d::cache.user_information=&::GetUserObjectInformationA;                    // 0070934A
            d::cache.process_station=&::GetProcessWindowStation;                       // 00709359
        }
    }
    bool service=false;
    const auto station_proc=d::cache.process_station;                                  // 0070935E
    if(station_proc!=nullptr) {
        const HWINSTA station=station_proc();                                          // 00709367
        USEROBJECTFLAGS station_flags;
        DWORD required;
        if(station==nullptr || !d::cache.user_information(station,1,&station_flags,12,&required) ||
           (station_flags.dwFlags&1u)==0)service=true;                                // 00709369..00709388
    }
    if(service) {
        volatile unsigned* const flag_slot=&flags;
        *flag_slot|=crt_version().dwMajorVersion<4u?0x00040000u:0x00200000u;           // 0070938A..007093A1 [00833BF4]
    } else {
        const auto active_proc=d::cache.active_window;                                 // 007093A3
        if(active_proc!=nullptr) {
            owner=active_proc();
            if(owner!=nullptr) {
                const auto popup_proc=d::cache.last_popup;
                if(popup_proc!=nullptr)owner=popup_proc(owner);                        // 007093B4..007093C0
            }
        }
    }
    return d::cache.message_box(owner,text,caption,flags);                             // 007093C2..007093CC
}
}
