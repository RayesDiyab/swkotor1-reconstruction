#include "1890cba853-providers.h"
// lane-m47-core ADAPTED COPY of the M46 stage unit rim-reviewed-body (rim/providers.cpp, base sha256
// dc8be3036c41ba5b3de77bd144def9ab37b07d2bd13976339abe16c7bb19f53e).
// FIX: kotor_missing_odd::rim_bindings is never published in the M46 stage (BindRim / GenuineRimBindings have no
// caller; tools/bindings_scan.py), so the AsyncRimLoadThreadProc body below dereferenced a null load_manager pointer as
// soon as the (CREATE_SUSPENDED, service-resource-3 construct.cpp:21) RIM thread is resumed. The six callees are now
// called directly through the genuine owners already listed by rim/genuine_bindings.cpp (same typed signatures):
//   00409B90 [007A39E8] -> rim_current_manager_007a39e8 (rim-manager-join)  00409BAA 005E9300 -> rim_enter_005e9300
//   00409BB5 004094A0  -> rim_load_004094a0 (next-rim-loader-existing)       00409BC3 005E9310 -> rim_leave_005e9310
//   00409B9A [0073D1C4] -> rim_suspend_thread_0073d1c4 (rim-win32-support)   00409BDF [0073D1E0] -> rim_exit_thread_0073d1e0
// lane-m48-b crt D81(2) REPLACEMENT COPY of the M47 stage unit rim-reviewed-body (rim/providers.cpp). Only change:
// FatalNormalBody (006FB344 _amsg_exit) no longer reads kotor_missing_odd::fatal_bindings (never filled: D76 null
// calls on the packed-file / purecall fatal paths); it calls the genuine owners directly (lane-m48-b crt/fatal):
//   [00833C34] crt_error_mode_00833c34   00701EC7 crt_ff_msgbanner_00701ec7   00701D50 report_runtime_error_00701d50
//   [007A2760] crt_aexit_rtn_007a2760 (image value 006FB223 _exit).  fatal_bindings / BindFatal stay defined (rim
//   providers.h ABI unchanged) but are no longer read by anything in the link.
#include "75948c323b-joins.h"
extern "C" {
extern volatile int crt_error_mode_00833c34;                     // 00833C34 owner (crt/fatal/crt_fatal.cpp)
void __cdecl crt_ff_msgbanner_00701ec7();                         // 00701EC7
extern void (__cdecl* volatile crt_aexit_rtn_007a2760)(int);      // 007A2760
}
namespace kotor_selected_termination_dependencies { void __cdecl report_runtime_error_00701d50(int); } // 00701D50
#include "a9e8159959-win32_bindings.h"
#include <intrin.h>



namespace kotor_missing_odd {
RimBindings rim_bindings{};
FatalBindings fatal_bindings{};
bool BindRim(const RimBindings& b) noexcept {
    if(!b.load_manager_007a39e8||!b.enter_005e9300||!b.load_004094a0||
       !b.leave_005e9310||!b.suspend_0073d1c4||!b.exit_thread_0073d1e0)return false;
    rim_bindings=b;return true;
}
bool BindFatal(const FatalBindings& b) noexcept {
    if(!b.error_mode_00833c34||!b.banner_00701ec7||!b.report_00701d50||
       !b.protected_exit_007a2760||!*b.protected_exit_007a2760)return false;
    fatal_bindings=b;return true;
}
}
using namespace kotor_missing_odd;
extern "C" __declspec(noinline) Word __stdcall unresolved_async_rim_thread_00409b90(void*) {
    auto* manager=rim_current_manager_007a39e8();                                   // 00409B90
    if(manager) {
        const SuspendGate suspend=&rim_suspend_thread_0073d1c4;                     // 00409B9A (hoisted IAT read)
        do {
            if(manager->word(0x48))break;                                           // 00409BA0..A5
            rim_enter_005e9300(manager->bytes+0x40,nullptr);                        // 00409BA7..AA



            rim_load_004094a0(rim_current_manager_007a39e8(),nullptr);              // 00409BAF..B5

            rim_leave_005e9310((rim_current_manager_007a39e8())->bytes+0x40,nullptr); // 00409BBA..C3
            const Word handle=rim_current_manager_007a39e8()->word(0x38);           // 00409BC8..CD
            (void)suspend(handle); // Original ignores all results, including FFFFFFFF. 00409BD0..D1
            manager=rim_current_manager_007a39e8();                                 // 00409BD3
        } while(manager);
    }



    rim_exit_thread_0073d1e0(1);                                                    // 00409BDD..DF

    // 00409BE5 int 3 follows the call; rim_exit_thread_0073d1e0 is declared noreturn (win32_bindings.h), so the
    // compiler already treats the path as terminated (C4702 if spelled here).
}
// Private C++ body. Its optimizer may tail-call exit and rewrite its own copy
// of the argument; the raw entry below preserves the original caller word.
extern "C" __declspec(noinline) void __cdecl FatalNormalBody(int error) {



    if(crt_error_mode_00833c34==1)crt_ff_msgbanner_00701ec7();                    // 006FB344..006FB34D

    kotor_selected_termination_dependencies::report_runtime_error_00701d50(error); // 006FB352..006FB356



    crt_aexit_rtn_007a2760(255);                                                   // 006FB35B..006FB360 call [007A2760]

    // The original POP/POP/RET path remains valid if this callback returns.
}
extern "C" __declspec(naked) void __cdecl unresolved_fatal_protected_006FB344(int) {
    __asm {
        push dword ptr [esp+4]
        call FatalNormalBody
        add esp,4
        mov ecx,dword ptr [esp+4]
        ret
    }
}
