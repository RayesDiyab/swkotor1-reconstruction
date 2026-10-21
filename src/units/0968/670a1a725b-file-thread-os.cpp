// lane-audit-r6allow: link bindings for the task-30 file-thread bodies (005EBED0/005EBE10/005EB570) declared in
// task-30 file_thread.h namespace kotor_file_dependencies. Pure forwarders to the same Win32 imports the original
// calls through its IAT (resolved from the pinned swkotor.exe import directory, tools/iat.py):
//   0073D1F0 kernel32!CreateThread       (005EBF0D)      0073D1F4 kernel32!SetThreadPriority (005EBF18)
//   0073D05C kernel32!WaitForSingleObject (005EB58D)     0073D200 kernel32!CloseHandle       (005EB596)
//   0073D1C4 kernel32!SuspendThread      (005EBE21/005EBEB8)  0073D1E0 kernel32!ExitThread   (005EBEC9)
// resume (0073D1DC ResumeThread) already has its stage owner: resource-successor r31 resume_os.cpp (not redefined).
// read = original CRT fread 006FA9D4 (005EBE68) on the CExoFileInternal FILE*. In the stage every CExoFile FILE* is
// opened by the UCRT boundary (engine-file-epoch/file_boundary.cpp fopen_s; read_file = std::fread), so the same
// UCRT fread is the only consistent family (equivalence record as D84 CExoFile::Write 005EBC60 / D69 stdio).
#include "9f442eedcf-file_thread.h"
#include <cstdio>
#include <type_traits>
namespace kotor_file_dependencies {
HANDLE __stdcall create(LPSECURITY_ATTRIBUTES a, SIZE_T s, LPTHREAD_START_ROUTINE f, void* p, DWORD fl, DWORD* id) {
    return ::CreateThread(a, s, f, p, fl, id);
}
BOOL __stdcall priority(HANDLE h, int p) { return ::SetThreadPriority(h, p); }
DWORD __stdcall wait(HANDLE h, DWORD ms) { return ::WaitForSingleObject(h, ms); }
BOOL __stdcall close(HANDLE h) { return ::CloseHandle(h); }
DWORD __stdcall suspend(HANDLE h) { return ::SuspendThread(h); }
__declspec(noreturn) void __stdcall exit(DWORD code) { ::ExitThread(code); }
uint32_t __cdecl read(void* buffer, uint32_t size, uint32_t count, void* file) {
    return static_cast<uint32_t>(std::fread(buffer, size, count, static_cast<std::FILE*>(file)));
}
}
static_assert(std::is_same<decltype(&kotor_file_dependencies::create), decltype(&::CreateThread)>::value, "CreateThread contract");
static_assert(std::is_same<decltype(&kotor_file_dependencies::priority), decltype(&::SetThreadPriority)>::value, "SetThreadPriority contract");
static_assert(std::is_same<decltype(&kotor_file_dependencies::wait), decltype(&::WaitForSingleObject)>::value, "WaitForSingleObject contract");
static_assert(std::is_same<decltype(&kotor_file_dependencies::close), decltype(&::CloseHandle)>::value, "CloseHandle contract");
static_assert(std::is_same<decltype(&kotor_file_dependencies::suspend), decltype(&::SuspendThread)>::value, "SuspendThread contract");
