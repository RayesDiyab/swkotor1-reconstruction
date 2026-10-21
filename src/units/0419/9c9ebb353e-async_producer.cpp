#include "06b9b092fa-async_read.h"
#include "f0fff50cf7-file_thread.h"
#include "42951039a5-joins.h"
#include <cstring>
#include <type_traits>

// Existing Task30 singleton and critical owners. No storage or lifetime is
// created here. The caller supplies four readable bytes containing FILE*.
extern "C" void __fastcall c2da_file_internal_005ebbf0(
    void* internal,void*,void* buffer,std::uint32_t size,std::uint32_t count) {
    void* stream;
    std::memcpy(&stream,internal,sizeof(stream));
    auto& worker=kotor_file::global_file_thread;
    volatile auto& published=worker;
    rim_enter_005e9300(&worker.critical_section,nullptr);
    published.buffer=buffer;
    const HANDLE handle=published.handle;
    published.complete=0;
    published.file=stream;
    published.element_size=size;
    published.element_count=count;
    (void)kotor_file_dependencies::resume(handle);
    rim_leave_005e9310(&worker.critical_section,nullptr);
}
using Producer=void(__fastcall*)(void*,void*,void*,std::uint32_t,std::uint32_t);
static_assert(std::is_same<decltype(&c2da_file_internal_005ebbf0),Producer>::value,
              "original ECX receiver, three stack words, consumed-void result");
static_assert(offsetof(kotor_file::FileThread,handle)==0,"008328B0");
static_assert(offsetof(kotor_file::FileThread,buffer)==0x24,"008328D4");
