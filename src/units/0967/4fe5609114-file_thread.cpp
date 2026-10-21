#include "9f442eedcf-file_thread.h"
namespace kotor_file {
FileThread* FileThread::Construct() {
    critical_section.Construct();
    complete = 0;
    terminate = 0;
    file = 0;
    result = 0;
    element_count = 0;
    element_size = 0;
    buffer = 0;
#ifdef NEGATIVE_THIS_PARAMETER
    void* parameter = this;
#else
    FileThread* local = 0;
    void* parameter = &local;
#endif
    handle = kotor_file_dependencies::create(0, 0x10000, Callback, parameter, 4, &thread_id);
    kotor_file_dependencies::priority(handle, -2);
    return this;
}
void FileThread::Destroy() {
    terminate = 1;
    kotor_file_dependencies::resume(handle);
#ifdef NEGATIVE_NULL_GUARD
    if (handle && handle != INVALID_HANDLE_VALUE) {
#else
    if (handle != INVALID_HANDLE_VALUE) {
#endif
        kotor_file_dependencies::wait(handle, INFINITE);
        kotor_file_dependencies::close(handle);
    }
    critical_section.Destroy();
}
DWORD __stdcall Callback(void*) {
    FileThread& t = global_file_thread;
    while (t.terminate == 0) {
        t.critical_section.Enter();
#ifndef NEGATIVE_SINGLE_ENTER
        t.critical_section.Enter();
#endif
        t.result = 0;
        if (t.file) {
            uint32_t elements = kotor_file_dependencies::read(t.buffer, t.element_size,
                                                            t.element_count, t.file);
            t.result = elements * t.element_size; // low 32 bits, like IMUL
        }
        t.complete = 1;
        t.file = 0;
        t.element_count = 0;
        t.element_size = 0;
#ifndef NEGATIVE_KEEP_BUFFER
        t.buffer = 0;
#endif
        t.critical_section.Leave();
#ifndef NEGATIVE_SINGLE_ENTER
        t.critical_section.Leave();
#endif
        kotor_file_dependencies::suspend(t.handle);
    }
    kotor_file_dependencies::exit(1);
}
}
