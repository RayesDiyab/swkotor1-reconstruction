#pragma once
// G11 shared original-CRT-state contract. Core-owned; frozen 2026-10-18.
//
// Exactly one original per-thread data object (0x88 bytes) and one TLS index
// cell exist in the composition. The quicksave/fatal lanes include this header
// and call these declarations; they never define a second copy.
//
// Evidence: original swkotor.exe PTD 006FEEDF/006FEF50/006FEEC1, TLS index
// 007A29B0, PTD+0x54 = kotor_reporter_action_data::Tables + 0x90 (the typed
// 120-byte action table), and the selected v5 image identity aliases. No
// numeric original address is dereferenced by this contract.
#include <cstddef>
#include <cstdint>

namespace g11 {

// Original 0x88-byte per-thread data. Fields not written by the original
// creation path stay zero (HeapAlloc HEAP_ZERO_MEMORY) and are reserved here.
struct Ptd {
    std::uint32_t tid;              // +0x00 current thread id
    std::int32_t  thread_sentinel;  // +0x04 = -1 at creation
    std::int32_t  errno_value;      // +0x08 original errno (== 006FFE78 +8)
    std::uint32_t doserrno;         // +0x0C original _doserrno
    std::uint32_t reserved_10;      // +0x10 zero
    std::uint32_t holdrand;         // +0x14 = 1 only at PTD creation
    std::uint8_t  reserved_18[0x3C];// +0x18..+0x53 zero
    void*         actions;          // +0x54 typed Tables::actions (10 x 12 B)
    std::uint8_t  reserved_58[0x30];// +0x58..+0x87 zero
};

static_assert(sizeof(Ptd) == 0x88, "original 006FB823(1,0x88) PTD size");
static_assert(offsetof(Ptd, tid) == 0x00, "PTD +0 tid");
static_assert(offsetof(Ptd, thread_sentinel) == 0x04, "PTD +4 = -1");
static_assert(offsetof(Ptd, errno_value) == 0x08, "PTD +8 errno");
static_assert(offsetof(Ptd, doserrno) == 0x0C, "PTD +C doserrno");
static_assert(offsetof(Ptd, holdrand) == 0x14, "PTD +14 holdrand");
static_assert(offsetof(Ptd, actions) == 0x54, "PTD +54 Tables::actions");
static_assert(offsetof(Ptd, reserved_58) + sizeof(Ptd::reserved_58) == 0x88,
    "PTD ends at 0x88; no +0x7C field");

// Test seam: production defaults are the real Win32 APIs plus the selected
// providers. Test-only code may replace entries before invoking production;
// production never checks for test mode.
struct Platform {
    void*         (__stdcall* heap_alloc)(void* heap, unsigned long flags, std::size_t bytes);
    unsigned long (__stdcall* heap_free)(void* heap, unsigned long flags, void* p);
    unsigned long (__stdcall* tls_alloc)();
    int           (__stdcall* tls_free)(unsigned long index);
    void*         (__stdcall* tls_get)(unsigned long index);
    int           (__stdcall* tls_set)(unsigned long index, void* value);
    unsigned long (__stdcall* get_last_error)();
    void          (__stdcall* set_last_error)(unsigned long e);
    unsigned long (__stdcall* get_current_thread_id)();
    int           (__stdcall* move_file_a)(const char* old_path, const char* new_path);
    void          (__stdcall* exit_process)(unsigned code);
    void*         (__stdcall* get_module_handle_a)(const char* name);
    void*         (__stdcall* get_proc_address)(void* module, const char* name);
    int           (__stdcall* free_library)(void* module);
    void          (__cdecl* banner)();
    void          (__cdecl* report)(int code);
    void          (__cdecl* fatal_first)(int code);
};

extern Platform platform;  // defined only by G11-core/source/g11_bindings.cpp

}  // namespace g11

extern "C" {

// Core-owned original state (006FEEDF getter, 007A29B0 index, 006FEF50 init,
// 006FEEC1 cleanup). The getter returns the possibly-null PTD; no guard is
// added, matching the original fatal-return path.
g11::Ptd* __cdecl g11_getptd();
int*      __cdecl g11_errno();
int       __cdecl g11_tls_init();
void      __cdecl g11_tls_cleanup();

// Fatal lane (accepted 0109 first-copy wrapper and the bounded second-copy
// init-fatal route). g11_fatal_getter is not noreturn.
void __cdecl g11_fatal_getter(unsigned code);
void __cdecl g11_fatal_init(unsigned code);

// Quicksave lane (original 006FFE8A mapper and 006FE5C6 MoveFileA wrapper).
g11::Ptd* __cdecl g11_map_os_error(unsigned os_error);
int       __cdecl g11_move_file_a(const char* old_path, const char* new_path);

}  // extern "C"
