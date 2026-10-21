// Stateless SDK transport proposal. Compile/static-audit only in this packet;
// no real ResumeThread, WaitForSingleObject or CloseHandle call is executed.
#include "40141ba4d9-manager_destructor.h"
#include <type_traits>
static_assert(sizeof(HANDLE)==4 && sizeof(DWORD)==4 && sizeof(BOOL)==4,"x86 IAT words");
using Resume=DWORD(WINAPI*)(HANDLE);
using Wait=DWORD(WINAPI*)(HANDLE,DWORD);
using Close=BOOL(WINAPI*)(HANDLE);
static_assert(std::is_same<decltype(&::ResumeThread),Resume>::value &&
              std::is_same<decltype(&::WaitForSingleObject),Wait>::value &&
              std::is_same<decltype(&::CloseHandle),Close>::value,"actual SDK import conventions");
extern "C" __declspec(naked) menu_resources::Word __stdcall
manager_resume_thread_0073d1dc(menu_resources::Word) {
    __asm jmp ResumeThread
}
extern "C" __declspec(naked) menu_resources::Word __stdcall
manager_wait_single_0073d05c(menu_resources::Word,menu_resources::Word) {
    __asm jmp WaitForSingleObject
}
extern "C" __declspec(naked) int __stdcall
manager_close_handle_0073d200(menu_resources::Word) {
    __asm jmp CloseHandle
}
