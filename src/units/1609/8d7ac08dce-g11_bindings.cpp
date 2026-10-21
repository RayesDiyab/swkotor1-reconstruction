// G11-core platform table and import bindings.
//
// This translation unit defines g11::platform with the real Win32 APIs and the
// selected providers, and owns every /alternatename join used by the quicksave
// and fatal lanes. The local C-linkage provider views below are declarations
// only; the fourteen one-call Win32 views above are the only definitions here.
// Every target is a strong selected symbol (v5 MAP for the 0727/0109/0720/0721
// and B-lane exports for the accepted 0062FBE0/005E6A20 bodies). No game body,
// no numeric address and no fallback provider is defined here.
#include "13bcb552ba-g11_crt_state.h"

#include <windows.h>

#include <cstring>

namespace {

// The frozen Platform spellings use std::size_t / unsigned long / void* while
// the Win32 imports use SIZE_T (= ULONG_PTR) / BOOL / FARPROC / HMODULE.
// HMODULE is a distinct struct pointer under STRICT. Every Win32 Platform entry
// therefore goes through a local one-call __stdcall view: taking a dllimport
// address in the aggregate would produce a dynamic .CRT$XCU initializer, but
// the existing heap XIB path runs g11_tls_init before XCU and needs the real
// TLS callbacks already stored. With only local function addresses (plus the
// three selected fatal providers) the aggregate is statically initialized.
// Each view forwards to the actual import and invents no value.
void* __stdcall g11_heap_alloc(void* heap, unsigned long flags, std::size_t bytes) {
    return ::HeapAlloc(static_cast<HANDLE>(heap), flags, static_cast<SIZE_T>(bytes));
}

unsigned long __stdcall g11_heap_free(void* heap, unsigned long flags, void* p) {
    return static_cast<unsigned long>(::HeapFree(static_cast<HANDLE>(heap), flags, p));
}

void* __stdcall g11_get_proc_address(void* module, const char* name) {
    FARPROC const proc = ::GetProcAddress(static_cast<HMODULE>(module), name);
    void* word = nullptr;
    static_assert(sizeof(word) == sizeof(proc), "x86 function/data pointer width");
    std::memcpy(&word, &proc, sizeof word);
    return word;
}

void* __stdcall g11_get_module_handle_a(const char* name) {
    return static_cast<void*>(::GetModuleHandleA(name));
}

int __stdcall g11_free_library(void* module) {
    return ::FreeLibrary(static_cast<HMODULE>(module));
}

unsigned long __stdcall g11_tls_alloc() {
    return ::TlsAlloc();
}

int __stdcall g11_tls_free(unsigned long index) {
    return ::TlsFree(index);
}

void* __stdcall g11_tls_get(unsigned long index) {
    return ::TlsGetValue(index);
}

int __stdcall g11_tls_set(unsigned long index, void* value) {
    return ::TlsSetValue(index, value);
}

unsigned long __stdcall g11_get_last_error() {
    return ::GetLastError();
}

void __stdcall g11_set_last_error(unsigned long e) {
    ::SetLastError(e);
}

unsigned long __stdcall g11_get_current_thread_id() {
    return ::GetCurrentThreadId();
}

int __stdcall g11_move_file_a_win32(const char* old_path, const char* new_path) {
    return ::MoveFileA(old_path, new_path);
}

void __stdcall g11_exit_process(unsigned code) {
    ::ExitProcess(code);
}

}  // namespace

extern "C" {

// ---- CExoString local views (BINDING-NAMES.md) ----
void* __fastcall g11_exostring_default(void* self, void* unused);
void* __fastcall g11_exostring_text(void* self, void* unused, const char* text);
void  __fastcall g11_exostring_destroy(void* self, void* unused);
void* __fastcall g11_exostring_assign(void* self, void* unused, void* rhs);
int   __fastcall g11_exostring_length(void* self, void* unused);
char* __fastcall g11_exostring_cstr(void* self, void* unused);
void* __fastcall g11_exostring_assign_text(void* self, void* unused, const char* text);
void  __cdecl   g11_exostring_format(void* out, const char* fmt, ...);
void* __fastcall g11_exostring_concat(void* a, void* unused, void* b, void* c);
void* __fastcall g11_exostring_left(void* src, void* unused, void* out, int count);
void* __fastcall g11_exostring_resolve(void* aliases, void* unused, void* out,
                                       void* name, unsigned flags);

// ---- CExoFile / selected game providers ----
void* __fastcall g11_exofile_construct(void* self, void* unused, void* path,
                                       unsigned flags, void* name);
void  __fastcall g11_exofile_destroy(void* self, void* unused);
int   __fastcall g11_exofile_write(void* file, void* unused, const char* text);
int   __fastcall g11_exofile_flush(void* file);
void* __fastcall g11_hide_load_screen(void* internal, void* unused, int a, int b);
void* __fastcall g11_nuke_directory(void* resman, void* unused, void* name,
                                    void* a, void* b);
int   __stdcall  g11_copy_quick_save(int save_id, void* suffix);

// ---- Fatal callbacks ----
void __cdecl g11_fatal_first(int code);
void __cdecl g11_banner();
void __cdecl g11_report(int code);

// ---- Data aliases (declarations only; storage is the selected owner) ----
extern void* g11_exo_base;
extern void* g11_resource_manager;
extern void* g11_app_manager;
extern void* g11_crtheap;
extern int   g11_error_mode;

}  // extern "C"

// CExoString: selected v5 symbols 00622DD0 / 00622D50 / 00622DE0 / 00632A30 /
// 0062BDD0 / 0062BDF0 / 0062CF10 / 00632B10 / 0062CD20 / 00632C60 / 00638000.
#pragma comment(linker, "/alternatename:@g11_exostring_default@8=??0StartupString@kotor_reconstruction@@QAE@XZ")
#pragma comment(linker, "/alternatename:@g11_exostring_text@12=??0StartupString@kotor_reconstruction@@QAE@RBD@Z")
#pragma comment(linker, "/alternatename:@g11_exostring_destroy@8=??1StartupString@kotor_reconstruction@@QAE@XZ")
#pragma comment(linker, "/alternatename:@g11_exostring_assign@12=?Assign005e5c50@StartupString@kotor_reconstruction@@QAEPAU12@PAU12@@Z")
#pragma comment(linker, "/alternatename:@g11_exostring_length@8=?BorrowedLength005e5790@StartupString@kotor_reconstruction@@QAEHXZ")
#pragma comment(linker, "/alternatename:@g11_exostring_cstr@8=@WindowCStr005e5670@8")
#pragma comment(linker, "/alternatename:@g11_exostring_assign_text@12=@GuiAssign005e5140@12")
#pragma comment(linker, "/alternatename:_g11_exostring_format=?StartupFormat005e5680@kotor_reconstruction@@YAXPAUStartupString@1@PBDZZ")
#pragma comment(linker, "/alternatename:@g11_exostring_concat@16=?GuiConcat005e5d10@@YIPAUStartupString@kotor_reconstruction@@PAU12@PAX0PBU12@@Z")
#pragma comment(linker, "/alternatename:@g11_exostring_left@16=?left_005e5f70@base_ini_reconstruction@@YIPAUStartupString@kotor_reconstruction@@PAU23@PAX0H@Z")
#pragma comment(linker, "/alternatename:@g11_exostring_resolve@20=?resolve_005e68a0@path_resolution@@YIPAUStartupString@kotor_reconstruction@@PAUAliasListView@alias_path@@PAXPAU23@2I@Z")

// CExoFile: construct is the selected real 005E68D0 body at 005D7C40 (not the
// frame-local @frame_LogFileCtor_005e68d0 alias). Flush is the accepted B export.
#pragma comment(linker, "/alternatename:@g11_exofile_construct@20=?exofile_construct_005e68d0@file_internal@@YIPAUFile@tlk_file@@PAU23@PAXPAUStartupString@kotor_reconstruction@@I2@Z")
#pragma comment(linker, "/alternatename:@g11_exofile_destroy@8=@file_destroy_005e69e0@8")
#pragma comment(linker, "/alternatename:@g11_exofile_write@12=@frame_LogWriteText_005e6a40@12")
#pragma comment(linker, "/alternatename:@g11_exofile_flush@4=@cerf_c_exofile_flush_005e6a20@4")

// Other selected game providers.
#pragma comment(linker, "/alternatename:@g11_hide_load_screen@16=@kotor_m15_client_hide_load_screen_005eda90@16")
#pragma comment(linker, "/alternatename:@g11_nuke_directory@20=@gui_load_resman_nuke_directory_00409480@20")
#pragma comment(linker, "/alternatename:_g11_copy_quick_save@8=_cerf_copy_quick_save_game_to_future_game_0062fbe0@8")

// Fatal providers: accepted 0109 first copy, selected banner and reporter.
#pragma comment(linker, "/alternatename:_g11_fatal_first=_unresolved_fatal_protected_006FB344")
#pragma comment(linker, "/alternatename:_g11_banner=_crt_ff_msgbanner_00701ec7")
#pragma comment(linker, "/alternatename:_g11_report=?report_runtime_error_00701d50@kotor_selected_termination_dependencies@@YAXH@Z")

// Data cells. exo_base/resource_manager/app_manager are the selected cells the
// quicksave body reads; resource_manager binds the 4-byte frame_control cell
// alias at 00967DA0 (the typed Manager view at the same VA is a reference
// symbol and would alias the cell address, not the contained pointer).
#pragma comment(linker, "/alternatename:_g11_exo_base=?exo_base_007a39e0@menu_continuation@@3PAUExoBase@1@A")
#pragma comment(linker, "/alternatename:_g11_resource_manager=?resource_manager_007a39e8@frame_control@@3IA")
#pragma comment(linker, "/alternatename:_g11_app_manager=?app_manager_007a39fc@menu_continuation@@3PAUAppManager@1@A")
#pragma comment(linker, "/alternatename:_g11_crtheap=?crtheap_00834338@kotor_crt_heap@@3RAXA")
#pragma comment(linker, "/alternatename:_g11_error_mode=_crt_error_mode_00833c34")

namespace g11 {

// Production defaults. Test-only code may replace entries before invoking
// production; production never checks for test mode.
Platform platform = {
    &g11_heap_alloc,
    &g11_heap_free,
    &g11_tls_alloc,
    &g11_tls_free,
    &g11_tls_get,
    &g11_tls_set,
    &g11_get_last_error,
    &g11_set_last_error,
    &g11_get_current_thread_id,
    &g11_move_file_a_win32,
    &g11_exit_process,
    &g11_get_module_handle_a,
    &g11_get_proc_address,
    &g11_free_library,
    &g11_banner,
    &g11_report,
    &g11_fatal_first
};

}  // namespace g11
