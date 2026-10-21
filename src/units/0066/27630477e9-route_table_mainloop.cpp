// fullscreen lane 2026-10-20: reinit_bind admits the fullscreen ReInitAurora routes: the 00403A86 initializer with a
// non-zero fullscreen word (Renderer::Initialize 0044DAB0 fullscreen branch: EnumDisplaySettingsA / ChangeDisplaySettingsA
// CDS_TEST+CDS_FULLSCREEN, popup style, HWND_TOPMOST), the cinematic backdrop worker CreateThread 00403885 (start 00401A30,
// reached when FullScreen()==1 && fullscreen==1, 00403850..0040385F), and the two Shell_TrayWnd ShowWindow calls 004038BA
// (SW_HIDE) / 00403C59 (SW_SHOWNOACTIVATE) on the FindWindowA("Shell_TrayWnd") result, which is passed to user32 unchanged.
// gfxapply 2026-10-20: reinit_bind admits the windowed in-game ReInitAurora tail (SetMouseMode 005DF5A0, SetEventDescriptions
// 005ED870) to their real bodies; the fullscreen-only stops (backdrop thread 00401A30, 00403A86 fullscreen initializer) stay.
// lane-routes: stage-ready kotor_dispatch::route_table (header owned by lane-prepare:
// stage/game-prepare/kotor_dispatch_routes.h). One row per exact original call site of the
// four consumers (winmain.cpp ea6ccb9d, window_shell.cpp b5c3d140, reinit.cpp, message.cpp).
// Rows exist only where a genuine body (existing M41, authored here, platform import, or a
// declared genuine provider still to be linked) receives the call; sites already consumed
// by the M40 bridges (winmain_window_route, shell_import, message_import) have no row.
// No row returns a value without executing the original callee's body.
#include "3473d08fe8-kotor_dispatch_routes.h"
extern "C" unsigned __fastcall kotor_fastcall_004ae860(void*, void*);
#include "c5c43d370a-winmain_routes.h"
#include "6b711608f5-services.h"
#include "1e4e568650-base_ini.h"
#include "3df78882af-startup_string.h"
#include "b7650b9cab-warning_runtime.h"
#include "5cc7361c75-menu_targets.h"
#include "be5af23e96-native_strings.h"
#include "7dff9333b0-string_priority.h"
#include "6542d604d6-allocator_live.h"
#include "9d61fbd0c6-initializer_slot.h"
#include "c117bb715c-aurora_window_slots.h"        // lane-window: typed CAurInternalGL table + table_of() (D41/D45)
#include "77fb10ec4f-startup_views.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <objbase.h>
#include <cstring>
#include "03e67ad358-window_shared_cells.h"
#include "85474cc56b-renderer_initialize.h"

#include <new>
#include <intrin.h>

extern "C" unsigned __cdecl lane_routes_mainloop_005ed7d0(unsigned outer);   // route_mainloop.cpp
extern "C" DWORD __stdcall kotor_stdcall_00401a30(void*);                     // lane-prepare backdrop.cpp (game_prepare_producers.h:83)
namespace client_intro { void __fastcall BeginIntro_005ed300(void* app, void* unused_edx); }  // lane-config intro_queue.h:41 (D36)


namespace {
using kotor_winmain::Word;
using kotor_winmain::Tick;
using kotor_winmain::Call;
using String = kotor_reconstruction::StartupString;
// A row whose genuine callee cannot accept this exact Call (impossible key mismatch,
// unknown literal, non-native vtable word) terminates like the window bridges (86).
[[noreturn]] void not_admitted(const char* check_id, Word w0, Word w1) { (void)check_id; (void)w0; (void)w1; TerminateProcess(GetCurrentProcess(), 86u); __fastfail(7); }  // R-6: same non-unwinding termination as the dispatcher
template<class T> T* as(Word w) { return reinterpret_cast<T*>(w); }
Word word(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
float as_float(Word bits) { float f; std::memcpy(&f, &bits, 4); return f; }

// ============================== WinMain ==============================
// Exact .rdata literals referenced by WinMain (bytes verified against the PE).
const char literal_0073d75c[] = "swkotor";
const char literal_0073d750[] = "config.txt";
const char literal_0073d744[] = "swKotor.ini";
const char literal_0073d734[] = "Disable Sound";
const char literal_0073d724[] = "Sound Options";
const char literal_0073d720[] = "%d";
const char literal_0073d71c[] = "";
const char literal_0073d710[] = "startup.txt";
const char literal_0073d648[] = ".\\swkotor.ini";
const char* literal(Word original) {
    switch (original) {
    case 0x0073d75c: return literal_0073d75c;
    case 0x0073d750: return literal_0073d750;
    case 0x0073d744: return literal_0073d744;
    case 0x0073d734: return literal_0073d734;
    case 0x0073d724: return literal_0073d724;
    case 0x0073d720: return literal_0073d720;
    case 0x0073d71c: return literal_0073d71c;
    case 0x0073d710: return literal_0073d710;
    case 0x0073d648: return literal_0073d648;
    default: not_admitted("route.literal.unknown", original, 0);   // debug/console literals (0073D640/0073D704/007A3A4C) are not admitted
    }
}

// 0040457C: WinMain stores (262144.0 - bias) as an 80-bit operand; the original FTOL body
// consumes ST(0) and returns EDX:EAX. Existing provider, x87 operand loaded exactly.
// Inline-asm CALL hazard (clang MS asm does not treat a CALL as clobbering unnamed caller-saved registers): the
// call lives in this never-inlined helper and the block names EAX, ECX and EDX (no-op `mov ecx, ecx`) so the
// compiler treats all three caller-saved registers as clobbered; no value, flag or x87 state changes.
__declspec(noinline) Tick ftol_operand(const unsigned char* operand) {
    Word result_low, result_high;
    __asm {
        mov eax, operand
        mov ecx, ecx
        fld tbyte ptr [eax]
        call unresolved_ftol2_006fae8c
        mov result_low, eax
        mov result_high, edx
    }
    return (static_cast<Tick>(result_high) << 32) | result_low;
}

Tick __cdecl winmain_bind(const Call& c) {
    const Word* a = c.args;
    void* self = reinterpret_cast<void*>(c.self);
    switch (c.target) {
    // ---- platform imports (IAT 0073Dxxx) ----
    case 0x0073d050: return word(CreateMutexA(as<SECURITY_ATTRIBUTES>(a[0]), static_cast<BOOL>(a[1]), literal(a[2])));
    case 0x0073d05c: return WaitForSingleObject(as<void>(a[0]), a[1]);
    case 0x0073d200: return static_cast<Word>(CloseHandle(as<void>(a[0])));
    case 0x0073d598: return static_cast<Word>(CoInitialize(as<void>(a[0])));
    case 0x0073d59c: CoUninitialize(); return 0;
    case 0x0073d3f4: return static_cast<Word>(PeekMessageA(as<MSG>(a[0]), as<HWND__>(a[1]), a[2], a[3], a[4]));
    case 0x0073d3b4: return static_cast<Word>(GetMessageA(as<MSG>(a[0]), as<HWND__>(a[1]), a[2], a[3]));
    case 0x0073d3a0: return static_cast<Word>(TranslateMessage(as<MSG>(a[0])));
    case 0x0073d3b8: return static_cast<Word>(DispatchMessageA(as<MSG>(a[0])));
    case 0x0073d0bc: return word(GetCurrentProcess());
    case 0x0073d110: return static_cast<Word>(SetPriorityClass(as<void>(a[0]), a[1]));
    case 0x0073d21c: glClear(a[0]); return 0;
    case 0x0073d224: glColor4f(as_float(a[0]), as_float(a[1]), as_float(a[2]), as_float(a[3])); return 0;
    case 0x0073d408: return word(GetDC(as<HWND__>(a[0])));
    case 0x0073d00c: return static_cast<Word>(SwapBuffers(as<HDC__>(a[0])));
    case 0x0073d404: return static_cast<Word>(ReleaseDC(as<HWND__>(a[0]), as<HDC__>(a[1])));
    case 0x0073d1e4: Sleep(a[0]); return 0;
    case 0x0073d3f8: return static_cast<Word>(SystemParametersInfoA(a[0], a[1], as<void>(a[2]), a[3]));
    // ---- existing M41 bodies (bind only) ----
    case 0x006fa7e6: return word(live_operator_new(a[0]));
    case 0x006fa390: live_free(as<void>(a[0])); return 0;
    case 0x005e6500: return word(static_cast<base_ini_reconstruction::Base*>(self)->construct_005e6500());
    case 0x005e6680:   // 005E6680: mov ecx,[ecx+14h]; jmp 005E7A90
        static_cast<base_ini_reconstruction::BaseInternal*>(static_cast<base_ini_reconstruction::Base*>(self)->internal)->load_aliases_005e7a90(as<String>(a[0]));
        return 0;
    case 0x005e5a90: return word(new (self) String(literal(a[0])));
    case 0x005b3190: return word(new (self) String());
    case 0x005e5c20: static_cast<String*>(self)->~String(); return 0;
    case 0x005e5630: return static_cast<Word>(static_cast<kotor_menu20::String*>(self)->as_int_005e5630());
    case 0x005e5680: kotor_reconstruction::StartupFormat005e5680(as<String>(a[0]), literal(a[1]), a[2]); return 0;
    case 0x005e6750: return word(static_cast<base_ini_reconstruction::PublicIni*>(self)->construct_005e6750());
    case 0x005e67c0: return static_cast<Word>(static_cast<base_ini_reconstruction::PublicIni*>(self)->read_005e67c0(as<String>(a[0]), as<String>(a[1]), as<String>(a[2]), as<String>(a[3])));
    case 0x005e67d0: return static_cast<Word>(static_cast<base_ini_reconstruction::PublicIni*>(self)->write_005e67d0(as<String>(a[0]), as<String>(a[1]), as<String>(a[2]), as<String>(a[3])));
    case 0x005e67e0: static_cast<base_ini_reconstruction::PublicIni*>(self)->destroy_005e67e0(); return 0;
    case 0x00401160: return word(static_cast<client_services::Manager*>(self)->construct_00401160());
    case 0x005ed860: return static_cast<client_services::Client*>(self)->start_services_005ed860(a[0], a[1], a[2], a[3], a[4]);
    case 0x005ed7b0: return static_cast<client_services::Client*>(self)->initialize_005ed7b0();
    case 0x005eda10: static_cast<client_services::Client*>(self)->input_005eda10(static_cast<int>(a[0]), static_cast<int>(a[1])); return 0;
    case 0x005e9260: return static_cast<kotor_menu_10_19::TimerDependency*>(self)->high_resolution_005e9260();
    case 0x006fae8c: return ftol_operand(c.x87_operand);
    // ---- authored bodies (lane-routes) ----
    case 0x0044c980: return word(kotor_cdecl_0044c980(literal(a[0])));
    case 0x004015b0: kotor_cdecl_004015b0(); return 0;
    case 0x00401610: kotor_cdecl_00401610(); return 0;
    case 0x00401700: kotor_cdecl_00401700(static_cast<int>(a[0]), as<char*>(a[1])); return 0;
    case 0x0044ef00: kotor_cdecl_0044ef00(as_float(a[0]), as_float(a[1]), as_float(a[2]), as_float(a[3])); return 0;
    case 0x005edfd0: kotor_fastcall_005edfd0(self, nullptr, a[0], a[1]); return 0;
    case 0x005ed300: client_intro::BeginIntro_005ed300(self, nullptr); return 0;   // lane-config body (D36)
    case 0x005ed420: kotor_fastcall_005ed420(self, nullptr, a[0]); return 0;
    case 0x005edb40: return kotor_fastcall_005edb40(self, nullptr);
    case 0x0044da60: kotor_cdecl_0044da60(); return 0;
    case 0x0044da80: kotor_cdecl_0044da80(); return 0;
    case 0x005ed7d0: return lane_routes_mainloop_005ed7d0(c.self);
    case 0x004ae860: return kotor_fastcall_004ae860(self, nullptr);
    case 0x0044d6a0: kotor_cdecl_0044d6a0(); return 0;
    case 0x005ee1f0: kotor_fastcall_005ee1f0(self, nullptr); return 0;
    // ---- graceful shutdown 00404A21..00404A95 (authored, lifecycle_leaves.cpp) ----
    case 0x00401280: kotor_fastcall_00401280(self, nullptr); return 0;
    case 0x005ed810: kotor_fastcall_005ed810(self, nullptr); return 0;
    case 0x005ed880: kotor_fastcall_005ed880(self, nullptr); return 0;
    case 0x004012c0: kotor_fastcall_004012c0(self, nullptr); return 0;
    case 0x005e66a0: kotor_fastcall_005e66a0(self, nullptr); return 0;
    // ---- not admitted: no genuine body yet (exit/debug/server paths; none before frame 1) ----
    default: not_admitted("route.winmain.target", c.site, c.target);
    }
}


// ============================== Shell (InitGameApp 00403F20, InitRenderWindow 004036F0,
// setup_opengl 00401BB0) ==============================
const char literal_0073d6f0[] = " Graphics ";
const char literal_0073d5d8[] = " ";
kotor_window_shell::Result __cdecl shell_bind(const kotor_window_shell::Call& c) {
    const Word* a = c.args;
    void* self = reinterpret_cast<void*>(c.self);
    switch (c.target) {
    case 0x005e6b10: kotor_cdecl_005e6b10(); return 0;                                    // 00403F44
    case 0x0044eec0: return word(kotor_cdecl_0044eec0());                                 // 00403F49
    case 0x005e5a90: {                                                                    // 00403F53/F6A/404006
        const char* text = c.site == 0x00403f6a ? literal_0073d6f0 : c.site == 0x00404006 ? literal_0073d5d8 : as<const char>(a[0]);
        return word(new (self) String(text));
    }
    case 0x005e5c20: static_cast<String*>(self)->~String(); return 0;
    case 0x005e5d10: return word(GuiConcat005e5d10(static_cast<String*>(self), nullptr, as<String>(a[0]), as<const String>(a[1])));
    case 0x005e5c50: return word(kotor_reconstruction::StartupAssign005e5c50(static_cast<String*>(self), nullptr, as<String>(a[0])));
    case 0x005e5b00: return word(WindowCopy005e5b00(static_cast<String*>(self), nullptr, as<const String>(a[0])));
    case 0x005e5670: return word(WindowCStr005e5670(static_cast<String*>(self), nullptr));
    case 0x0073d220: glClearColor(as_float(a[0]), as_float(a[1]), as_float(a[2]), as_float(a[3])); return 0;
    case 0x0073d390: glEnable(a[0]); return 0;
    case 0x0073d38c: glBlendFunc(a[0], a[1]); return 0;
    default: not_admitted("route.shell.target", c.site, c.target);
    }
}

// ============================== ReInitAurora 00403800 ==============================
using DeletingDestructor = void* (__fastcall*)(void*, void*, Word);
kotor_reinit::Result __cdecl reinit_bind(const kotor_reinit::Call& c) {
    const Word* a = c.args;
    void* self = reinterpret_cast<void*>(c.self);
    if (c.kind == kotor_reinit::Kind::virtual_slot) {
        // Runtime vtable word (Aurora table 00741518 published by the factory views owner;
        // CExoInput table for slot 0). Native fastcall-form entries only.
        if (c.site == 0x00403a86) { kotor_reinit::Result r = 0;   // fullscreen lane: a[6] (fullscreen) admitted; count/slot/target checked by dispatch_initializer
            if (!aurora_context_adoption::dispatch_initializer(c, r)) not_admitted("route.reinit.initializer_00403a86", c.site, c.slot); return r; }
        auto* const r = static_cast<kotor_renderer_init::Renderer*>(self);
        switch (c.slot) {   // Aurora rows read the typed table published by lane-window (D41/D45)
        case 0x04: return kotor_aurora_window::table_of(r)->fullscreen(r, nullptr);                                 // FullScreen 0044D6D0
        case 0x20: kotor_aurora_window::table_of(r)->shutdown(r, nullptr, a[0], a[1]); return 0;                   // ShutDown 0044EA30 (EAX incidental)
        case 0x0c: return kotor_aurora_window::table_of(r)->mode(as<kotor_renderer_init::U32>(a[0]), as<kotor_renderer_init::U32>(a[1]),
                                                                  as<kotor_renderer_init::U8>(a[2]), as<kotor_renderer_init::U32>(a[3]), a[4]); // GetValidMode 0044D6E0 (stdcall, ECX unused)
        case 0x00: return word(reinterpret_cast<DeletingDestructor>(c.target)(self, nullptr, a[0]));                  // CExoInput deleting dtor: GATED (D41, lane-input typed table pending)
        default: not_admitted("route.reinit.virtual_slot", c.site, c.slot);
        }
    }
    if (c.kind == kotor_reinit::Kind::import_slot) {
        switch (c.target) {
        case 0x0073d1ec: return word(CreateEventA(as<SECURITY_ATTRIBUTES>(a[0]), static_cast<BOOL>(a[1]), static_cast<BOOL>(a[2]), as<const char>(a[3])));
        case 0x0073d1f0: {
            // fullscreen lane: 00403885 CreateThread(0, 8000h, 00401A30 initCinematicBackdrop, event, 0, &id) admitted unchanged.
            if (a[2] != word(reinterpret_cast<const void*>(&kotor_stdcall_00401a30))) not_admitted("route.reinit.create_thread_start", c.site, a[2]);
            return word(CreateThread(as<SECURITY_ATTRIBUTES>(a[0]), a[1], &kotor_stdcall_00401a30, as<void>(a[3]), a[4], as<DWORD>(a[5])));
        }
        case 0x0073d1f4: return static_cast<Word>(SetThreadPriority(as<void>(a[0]), static_cast<int>(a[1])));
        case 0x0073d0f8: return GetTickCount();
        case 0x0073d3a8: return word(FindWindowA(as<const char>(a[0]), as<const char>(a[1])));
        case 0x0073d3ac: return static_cast<Word>(EnableWindow(as<HWND__>(a[0]), static_cast<BOOL>(a[1])));
        case 0x0073d400:   // 004038BA / 00403C59: ShowWindow(FindWindowA("Shell_TrayWnd") 0073D6E0, SW_HIDE / SW_SHOWNOACTIVATE)
            if (c.site == 0x004038ba || c.site == 0x00403c59) return static_cast<Word>(ShowWindow(as<HWND__>(a[0]), static_cast<int>(a[1])));
            return static_cast<Word>(::ShowWindow(as<HWND__>(a[0]), static_cast<int>(a[1])));
        case 0x0073d218: glGetFloatv(a[0], as<GLfloat>(a[1])); return 0;
        case 0x0073d430: return word(::SetCapture(as<HWND__>(a[0])));
        case 0x0073d3fc: return static_cast<Word>(DestroyWindow(as<HWND__>(a[0])));
        case 0x0073d220: glClearColor(as_float(a[0]), as_float(a[1]), as_float(a[2]), as_float(a[3])); return 0;
        case 0x0073d1f8: return static_cast<Word>(SetEvent(as<void>(a[0])));
        case 0x0073d1fc: return static_cast<Word>(GetExitCodeThread(as<void>(a[0]), as<DWORD>(a[1])));
        case 0x0073d200: return static_cast<Word>(CloseHandle(as<void>(a[0])));
        default: not_admitted("route.reinit.import", c.site, c.target);
        }
    }
    switch (c.target) {
    case 0x005ee170: kotor_fastcall_005ee170(self, nullptr, a[0]); return 0;
    case 0x005e6750: return word(static_cast<base_ini_reconstruction::PublicIni*>(self)->construct_005e6750());
    case 0x005b3190: return word(new (self) String());
    case 0x005e5a90: return word(new (self) String(as<const char>(a[0])));   // provider-owned literal pointers (Constants)
    case 0x005e67c0: return static_cast<Word>(static_cast<base_ini_reconstruction::PublicIni*>(self)->read_005e67c0(as<String>(a[0]), as<String>(a[1]), as<String>(a[2]), as<String>(a[3])));
    case 0x005e5c20: static_cast<String*>(self)->~String(); return 0;
    case 0x005e5630: return static_cast<Word>(static_cast<kotor_menu20::String*>(self)->as_int_005e5630());
    case 0x0044ef60: kotor_cdecl_0044ef60(); return 0;
    case 0x0044ef70: kotor_cdecl_0044ef70(); return 0;
    case 0x0044ef80: kotor_cdecl_0044ef80(); return 0;
    case 0x005eda10: static_cast<client_services::Client*>(self)->input_005eda10(static_cast<int>(a[0]), static_cast<int>(a[1])); return 0;
    case 0x004036f0: kotor_window_shell::init_render_window_004036f0(a[0], static_cast<std::int32_t>(a[1])); return 0;
    case 0x00401bb0: kotor_window_shell::setup_opengl_00401bb0(); return 0;
    case 0x005df6e0: return kotor_fastcall_005df6e0(self, nullptr);
    case 0x006fa7e6: return word(live_operator_new(a[0]));
    case 0x005df3f0: return word(kotor_fastcall_005df3f0(self, nullptr));
    case 0x005df5a0: kotor_fastcall_005df5a0(self, nullptr, a[0]); return 0;   // 00403BD5 CExoInput::SetMouseMode -> 005E1740 (mouse_mode.cpp, reviewed gfxapply vs 005E1740..17D1; also live via 005F49C6); EAX unused at 00403BDA
    case 0x00401d90: kotor_cdecl_00401d90(); return 0;
    case 0x005ed7c0: return kotor_fastcall_005ed7c0(self, nullptr);
    case 0x005ed870: kotor_fastcall_005ed870(self, nullptr); return 0;          // 00403BF9 CClientExoApp::SetEventDescriptions -> 005EEB10 (0460 body, the startup provider of 005F5318)
    case 0x00401e00: kotor_cdecl_00401e00(); return 0; // Original void route; actual activation effects retained.
    case 0x005ed980: kotor_fastcall_005ed980(self, nullptr); return 0;
    case 0x005ee180: kotor_fastcall_005ee180(self, nullptr, a[0]); return 0;
    case 0x005e67e0: static_cast<base_ini_reconstruction::PublicIni*>(self)->destroy_005e67e0(); return 0;
    default: not_admitted("route.reinit.target", c.site, c.target);
    }
}

// ============================== WinMessageHandler 00402800 ==============================
kotor_message::Result __cdecl message_bind(const kotor_message::Call& c) {
    const Word* a = c.args;
    void* self = reinterpret_cast<void*>(c.self);
    if (c.kind == kotor_message::Kind::virtual_slot) {
        auto* const r = static_cast<kotor_renderer_init::Renderer*>(self);
        switch (c.slot) {
        case 0x04: return kotor_aurora_window::table_of(r)->fullscreen(r, nullptr);   // FullScreen 0044D6D0
        case 0x18: return kotor_aurora_window::table_of(r)->desktop(r, nullptr);      // RestoreDesktopDisplayMode 0044D8D0
        case 0x1c: return kotor_aurora_window::table_of(r)->display(r, nullptr);      // RestoreAuroraDisplayMode 0044D900
        default: not_admitted("route.message.virtual_slot", c.site, c.slot);
        }
    }
    switch (c.target) {
    case 0x005eda10: static_cast<client_services::Client*>(self)->input_005eda10(static_cast<int>(a[0]), static_cast<int>(a[1])); return 0;
    case 0x005edb40: return kotor_fastcall_005edb40(self, nullptr);
    case 0x005eda00: return kotor_fastcall_005eda00(self, nullptr);
    case 0x005df5b0: kotor_fastcall_005df5b0(self, nullptr, a[0]); return 0;
    case 0x006fa7e6: return word(live_operator_new(a[0]));
    case 0x00401d90: kotor_cdecl_00401d90(); return 0;
    case 0x00401e00: kotor_cdecl_00401e00(); return 0;
    case 0x004015f0: kotor_cdecl_004015f0(); return 0;
    case 0x00401650: kotor_cdecl_00401650(); return 0;
    case 0x004015b0: kotor_cdecl_004015b0(); return 0;
    case 0x00401610: kotor_cdecl_00401610(); return 0;
    case 0x005ed980: kotor_fastcall_005ed980(self, nullptr); return 0;
    case 0x00401e90: kotor_cdecl_00401e90(a[0], a[1]); return 0;
    case 0x005ee160: kotor_fastcall_005ee160(self, nullptr); return 0;
    case 0x005ed480: return frame_R4_005ed480(self, nullptr);
    case 0x004024f0: return kotor_cdecl_004024f0(a[0], a[1], a[2]);
    case 0x00402240: return kotor_cdecl_00402240(a[0]);
    case 0x005ed4d0: return kotor_fastcall_005ed4d0(self, nullptr);
    case 0x005ed4e0: return kotor_fastcall_005ed4e0(self, nullptr);
    case 0x005dfb00: kotor_fastcall_005dfb00(self, nullptr, a[0], a[1], a[2]); return 0;
    case 0x005dfc40: kotor_fastcall_005dfc40(self, nullptr, a[0], a[1], a[2]); return 0;
    case 0x00403de0: kotor_cdecl_00403de0(); return 0;
    case 0x005df510: return kotor_fastcall_005df510(self, nullptr);
    case 0x005ed700: return word(static_cast<kotor_menu20::ClientApp*>(self)->get_options_005ed700());
    case 0x005df590: kotor_fastcall_005df590(self, nullptr, as<void>(a[0])); return 0;
    default: not_admitted("route.message.target", c.site, c.target);
    }
}

const kotor_dispatch::WinMainRoute winmain_rows[] = {
 {0x00404222,0x0073d050,3,0,0x0,&winmain_bind},
 {0x00404234,0x0073d05c,2,0,0x0,&winmain_bind},
 {0x0040423f,0x0073d200,1,0,0x0,&winmain_bind},
 {0x00404252,0x0073d598,1,0,0x0,&winmain_bind},
 {0x00404264,0x0073d3f4,5,0,0x0,&winmain_bind},
 {0x0040426f,0x0044c980,1,0,0x0,&winmain_bind},
 {0x00404276,0x006fa7e6,1,0,0x0,&winmain_bind},
 {0x00404284,0x005e6500,0,0,0x0,&winmain_bind},
 {0x0040429b,0x005e5a90,1,0,0x0,&winmain_bind},
 {0x004042ab,0x005e6680,1,0,0x0,&winmain_bind},
 {0x004042b4,0x005e5c20,0,0,0x0,&winmain_bind},
 {0x004042bb,0x006fa7e6,1,0,0x0,&winmain_bind},
 {0x004042c9,0x00401160,0,0,0x0,&winmain_bind},
 {0x004042ef,0x004015b0,0,0,0x0,&winmain_bind},
 {0x004042f4,0x00401610,0,0,0x0,&winmain_bind},
 {0x004042fd,0x005e6750,0,0,0x0,&winmain_bind},
 {0x00404306,0x005b3190,0,0,0x0,&winmain_bind},
 {0x00404314,0x005e5a90,1,0,0x0,&winmain_bind},
 {0x00404322,0x005e5a90,1,0,0x0,&winmain_bind},
 {0x00404330,0x005e5a90,1,0,0x0,&winmain_bind},
 {0x0040434d,0x005e67c0,4,0,0x0,&winmain_bind},
 {0x00404358,0x005e5c20,0,0,0x0,&winmain_bind},
 {0x00404361,0x005e5c20,0,0,0x0,&winmain_bind},
 {0x0040436a,0x005e5c20,0,0,0x0,&winmain_bind},
 {0x00404377,0x005e5630,0,0,0x0,&winmain_bind},
 {0x00404390,0x005e5680,3,0,0x0,&winmain_bind},
 {0x004043a1,0x005e5a90,1,0,0x0,&winmain_bind},
 {0x004043af,0x005e5a90,1,0,0x0,&winmain_bind},
 {0x004043bd,0x005e5a90,1,0,0x0,&winmain_bind},
 {0x004043da,0x005e67d0,4,0,0x0,&winmain_bind},
 {0x004043e3,0x005e5c20,0,0,0x0,&winmain_bind},
 {0x004043ec,0x005e5c20,0,0,0x0,&winmain_bind},
 {0x004043f5,0x005e5c20,0,0,0x0,&winmain_bind},
 {0x00404407,0x00401700,2,0,0x0,&winmain_bind},
 {0x00404422,0x005e5a90,1,0,0x0,&winmain_bind},
 {0x00404430,0x005e5a90,1,0,0x0,&winmain_bind},
 {0x0040443e,0x005e5a90,1,0,0x0,&winmain_bind},
 {0x0040445d,0x005ed860,5,0,0x0,&winmain_bind},
 {0x00404468,0x005e5c20,0,0,0x0,&winmain_bind},
 {0x00404471,0x005e5c20,0,0,0x0,&winmain_bind},
 {0x0040447a,0x005e5c20,0,0,0x0,&winmain_bind},
 {0x00404484,0x0073d200,1,0,0x0,&winmain_bind},
 {0x0040448e,0x005e5c20,0,0,0x0,&winmain_bind},
 {0x00404497,0x005e67e0,0,0,0x0,&winmain_bind},
 {0x004044b0,0x005ed7b0,0,0,0x0,&winmain_bind},
 {0x004044c7,0x005eda10,2,0,0x0,&winmain_bind},
 {0x004044d1,0x0044c980,1,0,0x0,&winmain_bind},
 {0x004044da,0x0044ef00,4,0,0x0,&winmain_bind},
 {0x004044ed,0x005edfd0,2,0,0x0,&winmain_bind},
 {0x004044ff,0x005edfd0,2,0,0x0,&winmain_bind},
 {0x00404511,0x005edfd0,2,0,0x0,&winmain_bind},
 {0x00404522,0x005edfd0,2,0,0x0,&winmain_bind},
 {0x00404534,0x005edfd0,2,0,0x0,&winmain_bind},
 {0x00404542,0x005ed300,0,0,0x0,&winmain_bind},
 {0x00404551,0x005ed420,1,0,0x0,&winmain_bind},
 {0x0040457c,0x006fae8c,0,0,0x0,&winmain_bind},
 {0x00404592,0x0073d3f4,5,0,0x0,&winmain_bind},
 {0x004045a7,0x0073d3b4,4,0,0x0,&winmain_bind},
 {0x004045ba,0x0073d3a0,1,0,0x0,&winmain_bind},
 {0x004045c5,0x0073d3b8,1,0,0x0,&winmain_bind},
 {0x004045f2,0x0073d0bc,0,0,0x0,&winmain_bind},
 {0x004045f5,0x0073d110,2,0,0x0,&winmain_bind},
 {0x00404609,0x0073d0bc,0,0,0x0,&winmain_bind},
 {0x0040460c,0x0073d110,2,0,0x0,&winmain_bind},
 {0x00404673,0x005edb40,0,0,0x0,&winmain_bind},
 {0x00404681,0x0073d21c,1,0,0x0,&winmain_bind},
 {0x0040468f,0x0044da60,0,0,0x0,&winmain_bind},
 {0x00404696,0x0044da80,0,0,0x0,&winmain_bind},
 {0x004046a4,0x005e9260,0,0,0x0,&winmain_bind},
 {0x004046bd,0x005ed7d0,0,0,0x0,&winmain_bind},
 {0x004046d8,0x005e9260,0,0,0x0,&winmain_bind},
 {0x00404736,0x005e9260,0,0,0x0,&winmain_bind},
 {0x00404757,0x004ae860,0,0,0x0,&winmain_bind},
 {0x00404785,0x005e9260,0,0,0x0,&winmain_bind},
 {0x00404846,0x005edb40,0,0,0x0,&winmain_bind},
 {0x0040484f,0x0044d6a0,0,0,0x0,&winmain_bind},
 {0x00404870,0x0073d224,4,0,0x0,&winmain_bind},
 {0x00404896,0x005edb40,0,0,0x0,&winmain_bind},
 {0x004048b8,0x0073d408,1,0,0x0,&winmain_bind},
 {0x004048c1,0x0073d00c,1,0,0x0,&winmain_bind},
 {0x004048cf,0x0073d404,2,0,0x0,&winmain_bind},
 {0x004048e3,0x005ee1f0,0,0,0x0,&winmain_bind},
 {0x004048f7,0x0073d1e4,1,0,0x0,&winmain_bind},
 {0x0040492d,0x005e9260,0,0,0x0,&winmain_bind},
 {0x00404998,0x005e9260,0,0,0x0,&winmain_bind},
 {0x004049fb,0x005e9260,0,0,0x0,&winmain_bind},
 {0x00404a21,0x00401280,0,0,0x0,&winmain_bind},
 {0x00404a2f,0x005ed810,0,0,0x0,&winmain_bind},
 {0x00404a3c,0x005ed880,0,0,0x0,&winmain_bind},
 {0x00404a56,0x0073d3f8,4,0,0x0,&winmain_bind},
 {0x00404a67,0x0073d3f8,4,0,0x0,&winmain_bind},
 {0x00404a75,0x004012c0,0,0,0x0,&winmain_bind},
 {0x00404a7b,0x006fa390,1,0,0x0,&winmain_bind},
 {0x00404a95,0x005e66a0,0,0,0x0,&winmain_bind},
 {0x00404a9b,0x006fa390,1,0,0x0,&winmain_bind},
 {0x00404aa9,0x0073d59c,0,0,0x0,&winmain_bind},
 {0x00404ab0,0x0073d200,1,0,0x0,&winmain_bind},
 {0x00404abe,0x005e5c20,0,0,0x0,&winmain_bind},
 {0x00404ac7,0x005e67e0,0,0,0x0,&winmain_bind},
};
const kotor_dispatch::ShellRoute shell_rows[] = {
 {0x00401bb9,0x0073d220,4,0,0x0,&shell_bind},
 {0x00401bcb,0x0073d390,1,0,0x0,&shell_bind},
 {0x00401bd4,0x0073d390,1,0,0x0,&shell_bind},
 {0x00401bdd,0x0073d390,1,0,0x0,&shell_bind},
 {0x00401be5,0x0073d390,1,0,0x0,&shell_bind},
 {0x00401bee,0x0073d390,1,0,0x0,&shell_bind},
 {0x00401bfd,0x0073d38c,2,0,0x0,&shell_bind},
 {0x00401c0a,0x0073d390,1,0,0x0,&shell_bind},
 {0x0040373b,0x005e5b00,1,0,0x0,&shell_bind},
 {0x004037bb,0x005e5670,0,0,0x0,&shell_bind},
 {0x004037e8,0x005e5c20,0,0,0x0,&shell_bind},
 {0x00403f44,0x005e6b10,0,0,0x0,&shell_bind},
 {0x00403f49,0x0044eec0,0,0,0x0,&shell_bind},
 {0x00403f53,0x005e5a90,1,0,0x0,&shell_bind},
 {0x00403f6a,0x005e5a90,1,0,0x0,&shell_bind},
 {0x00403f86,0x005e5d10,2,0,0x0,&shell_bind},
 {0x00403f9f,0x005e5d10,2,0,0x0,&shell_bind},
 {0x00403fb2,0x005e5c50,1,0,0x0,&shell_bind},
 {0x00403fc3,0x005e5c20,0,0,0x0,&shell_bind},
 {0x00403fd4,0x005e5c20,0,0,0x0,&shell_bind},
 {0x00403fe4,0x005e5c20,0,0,0x0,&shell_bind},
 {0x00403ff8,0x005e5c20,0,0,0x0,&shell_bind},
 {0x00404006,0x005e5a90,1,0,0x0,&shell_bind},
 {0x00404025,0x005e5d10,2,0,0x0,&shell_bind},
 {0x0040403e,0x005e5d10,2,0,0x0,&shell_bind},
 {0x0040404f,0x005e5c20,0,0,0x0,&shell_bind},
 {0x00404060,0x005e5c20,0,0,0x0,&shell_bind},
 {0x0040406e,0x005e5b00,1,0,0x0,&shell_bind},
 {0x004040ef,0x005e5670,0,0,0x0,&shell_bind},
 {0x00404158,0x005e5c20,0,0,0x0,&shell_bind},
 {0x0040416c,0x005e5c20,0,0,0x0,&shell_bind},
 {0x004041b1,0x005e5c20,0,0,0x0,&shell_bind},
 {0x004041c5,0x005e5c20,0,0,0x0,&shell_bind},
};
const kotor_dispatch::ReinitRoute reinit_rows[] = {
 {0x0040383f,0x005ee170,1,0,0x0,&reinit_bind},
 {0x00403850,0x00000000,0,2,0x4,&reinit_bind},
 {0x00403869,0x0073d1ec,4,1,0x0,&reinit_bind},
 {0x00403885,0x0073d1f0,6,1,0x0,&reinit_bind},
 {0x00403892,0x0073d1f4,2,1,0x0,&reinit_bind},
 {0x00403898,0x0073d0f8,0,1,0x0,&reinit_bind},
 {0x004038a4,0x0073d3a8,2,1,0x0,&reinit_bind},
 {0x004038b2,0x0073d3ac,2,1,0x0,&reinit_bind},
 {0x004038ba,0x0073d400,2,1,0x0,&reinit_bind},
 {0x00403902,0x0073d218,2,1,0x0,&reinit_bind},
 {0x00403923,0x00000000,2,2,0x20,&reinit_bind},
 {0x00403930,0x005e6750,0,0,0x0,&reinit_bind},
 {0x0040393d,0x005b3190,0,0,0x0,&reinit_bind},
 {0x00403950,0x005e5a90,1,0,0x0,&reinit_bind},
 {0x00403963,0x005e5a90,1,0,0x0,&reinit_bind},
 {0x00403977,0x005e5a90,1,0,0x0,&reinit_bind},
 {0x00403999,0x005e67c0,4,0,0x0,&reinit_bind},
 {0x004039a8,0x005e5c20,0,0,0x0,&reinit_bind},
 {0x004039b6,0x005e5c20,0,0,0x0,&reinit_bind},
 {0x004039c4,0x005e5c20,0,0,0x0,&reinit_bind},
 {0x004039d1,0x005e5630,0,0,0x0,&reinit_bind},
 {0x004039db,0x0044ef60,0,0,0x0,&reinit_bind},
 {0x004039e0,0x0073d0f8,0,1,0x0,&reinit_bind},
 {0x004039fc,0x005eda10,2,0,0x0,&reinit_bind},
 {0x00403a0f,0x00000000,1,2,0x0,&reinit_bind},
 {0x00403a23,0x004036f0,2,0,0x0,&reinit_bind},
 {0x00403a57,0x00000000,5,2,0xc,&reinit_bind},
 {0x00403a86,0x00000000,9,2,0x8,&reinit_bind},
 {0x00403a92,0x005e5a90,1,0,0x0,&reinit_bind},
 {0x00403aa5,0x005e5a90,1,0,0x0,&reinit_bind},
 {0x00403ab9,0x005e5a90,1,0,0x0,&reinit_bind},
 {0x00403adb,0x005e67c0,4,0,0x0,&reinit_bind},
 {0x00403aea,0x005e5c20,0,0,0x0,&reinit_bind},
 {0x00403af8,0x005e5c20,0,0,0x0,&reinit_bind},
 {0x00403b06,0x005e5c20,0,0,0x0,&reinit_bind},
 {0x00403b15,0x005e5630,0,0,0x0,&reinit_bind},
 {0x00403b1f,0x0044ef70,0,0,0x0,&reinit_bind},
 {0x00403b26,0x0044ef80,0,0,0x0,&reinit_bind},
 {0x00403b32,0x0073d430,1,1,0x0,&reinit_bind},
 {0x00403b3d,0x0073d3fc,1,1,0x0,&reinit_bind},
 {0x00403b49,0x0073d0f8,0,1,0x0,&reinit_bind},
 {0x00403b4b,0x00401bb0,0,0,0x0,&reinit_bind},
 {0x00403b72,0x0073d220,4,1,0x0,&reinit_bind},
 {0x00403b82,0x005df6e0,0,0,0x0,&reinit_bind},
 {0x00403b99,0x00000000,1,2,0x0,&reinit_bind},
 {0x00403b9d,0x006fa7e6,1,0,0x0,&reinit_bind},
 {0x00403bb4,0x005df3f0,0,0,0x0,&reinit_bind},
 {0x00403bd5,0x005df5a0,1,0,0x0,&reinit_bind},
 {0x00403bda,0x00401d90,0,0,0x0,&reinit_bind},
 {0x00403be8,0x005ed7c0,0,0,0x0,&reinit_bind},
 {0x00403bf9,0x005ed870,0,0,0x0,&reinit_bind},
 {0x00403bfe,0x00401e00,0,0,0x0,&reinit_bind},
 {0x00403c03,0x0073d0f8,0,1,0x0,&reinit_bind},
 {0x00403c21,0x005ed980,0,0,0x0,&reinit_bind},
 {0x00403c41,0x0073d3a8,2,1,0x0,&reinit_bind},
 {0x00403c50,0x0073d3ac,2,1,0x0,&reinit_bind},
 {0x00403c59,0x0073d400,2,1,0x0,&reinit_bind},
 {0x00403c64,0x0073d1f8,1,1,0x0,&reinit_bind},
 {0x00403c6a,0x0073d0f8,0,1,0x0,&reinit_bind},
 {0x00403c8a,0x0073d1fc,2,1,0x0,&reinit_bind},
 {0x00403c98,0x0073d0f8,0,1,0x0,&reinit_bind},
 {0x00403ca5,0x0073d200,1,1,0x0,&reinit_bind},
 {0x00403ca8,0x0073d200,1,1,0x0,&reinit_bind},
 {0x00403caa,0x0073d0f8,0,1,0x0,&reinit_bind},
 {0x00403cb7,0x005ee180,1,0,0x0,&reinit_bind},
 {0x00403cc5,0x005e5c20,0,0,0x0,&reinit_bind},
 {0x00403cd6,0x005e67e0,0,0,0x0,&reinit_bind},
};
const kotor_dispatch::MessageRoute message_rows[] = {
 {0x00402895,0x005edb40,0,0,0x0,&message_bind},
 {0x004028a9,0x005eda10,2,0,0x0,&message_bind},
 {0x004028dc,0x005eda10,2,0,0x0,&message_bind},
 {0x00402900,0x005edb40,0,0,0x0,&message_bind},
 {0x0040290c,0x006fa7e6,1,0,0x0,&message_bind},
 {0x00402988,0x005edb40,0,0,0x0,&message_bind},
 {0x0040299a,0x00000000,0,2,0x4,&message_bind},
 {0x004029ca,0x00000000,0,2,0x4,&message_bind},
 {0x004029e9,0x00000000,0,2,0x18,&message_bind},
 {0x004029f8,0x00401d90,0,0,0x0,&message_bind},
 {0x00402a15,0x004015f0,0,0,0x0,&message_bind},
 {0x00402a1a,0x00401650,0,0,0x0,&message_bind},
 {0x00402a3f,0x00000000,0,2,0x4,&message_bind},
 {0x00402a63,0x00000000,0,2,0x1c,&message_bind},
 {0x00402a83,0x005edb40,0,0,0x0,&message_bind},
 {0x00402aa2,0x005ed980,0,0,0x0,&message_bind},
 {0x00402aca,0x00000000,0,2,0x4,&message_bind},
 {0x00402ad1,0x00401e00,0,0,0x0,&message_bind},
 {0x00402ae4,0x004015b0,0,0,0x0,&message_bind},
 {0x00402ae9,0x00401610,0,0,0x0,&message_bind},
 {0x00402b19,0x005df5b0,1,0,0x0,&message_bind},
 {0x00402b51,0x005eda00,0,0,0x0,&message_bind},
 {0x00402b6a,0x005edb40,0,0,0x0,&message_bind},
 {0x00402b7a,0x006fa7e6,1,0,0x0,&message_bind},
 {0x00402c04,0x005eda10,2,0,0x0,&message_bind},
 {0x00402c29,0x005eda10,2,0,0x0,&message_bind},
 {0x00402c7f,0x005eda10,2,0,0x0,&message_bind},
 {0x00402c9f,0x00401e90,2,0,0x0,&message_bind},
 {0x00402ce1,0x005edb40,0,0,0x0,&message_bind},
 {0x00402cf4,0x005ee160,0,0,0x0,&message_bind},
 {0x00402d01,0x005ed480,0,0,0x0,&message_bind},
 {0x00402d15,0x004024f0,3,0,0x0,&message_bind},
 {0x00402d3a,0x005ed480,0,0,0x0,&message_bind},
 {0x00402d48,0x00402240,1,0,0x0,&message_bind},
 {0x00402d6c,0x005ed4d0,0,0,0x0,&message_bind},
 {0x00402d82,0x005ed4e0,0,0,0x0,&message_bind},
 {0x00402da2,0x005ed4d0,0,0,0x0,&message_bind},
 {0x00402db7,0x005ed4e0,0,0,0x0,&message_bind},
 {0x00402dd7,0x005ed4d0,0,0,0x0,&message_bind},
 {0x00402dfc,0x005ed4e0,0,0,0x0,&message_bind},
 {0x00402e03,0x005dfb00,3,0,0x0,&message_bind},
 {0x00402e52,0x005df510,0,0,0x0,&message_bind},
 {0x00402e8e,0x005df590,1,0,0x0,&message_bind},
 {0x00402ea6,0x005df510,0,0,0x0,&message_bind},
 {0x00402ec0,0x005ed700,0,0,0x0,&message_bind},
 {0x00402ed6,0x005ed700,0,0,0x0,&message_bind},
 {0x00402efe,0x005df510,0,0,0x0,&message_bind},
 {0x00402f17,0x005ed700,0,0,0x0,&message_bind},
 {0x00402f29,0x005ed700,0,0,0x0,&message_bind},
 {0x00402f69,0x005df590,1,0,0x0,&message_bind},
 {0x00402f81,0x005df510,0,0,0x0,&message_bind},
 {0x00402f9b,0x005ed700,0,0,0x0,&message_bind},
 {0x00402fad,0x005ed700,0,0,0x0,&message_bind},
 {0x00402fe9,0x005df510,0,0,0x0,&message_bind},
 {0x00403003,0x005ed700,0,0,0x0,&message_bind},
 {0x00403014,0x005ed700,0,0,0x0,&message_bind},
 {0x00403055,0x005df590,1,0,0x0,&message_bind},
 {0x00403078,0x005df510,0,0,0x0,&message_bind},
 {0x00403092,0x005ed700,0,0,0x0,&message_bind},
 {0x004030a4,0x005ed700,0,0,0x0,&message_bind},
 {0x004030e5,0x005df590,1,0,0x0,&message_bind},
 {0x00403108,0x005df510,0,0,0x0,&message_bind},
 {0x00403121,0x005ed700,0,0,0x0,&message_bind},
 {0x00403133,0x005ed700,0,0,0x0,&message_bind},
 {0x00403173,0x005df590,1,0,0x0,&message_bind},
 {0x004031ba,0x005df510,0,0,0x0,&message_bind},
 {0x004031fa,0x005df590,1,0,0x0,&message_bind},
 {0x00403252,0x005df510,0,0,0x0,&message_bind},
 {0x004032b0,0x005df590,1,0,0x0,&message_bind},
 {0x004032c3,0x005ed4d0,0,0,0x0,&message_bind},
 {0x004032e8,0x005ed4e0,0,0,0x0,&message_bind},
 {0x004032ef,0x005dfc40,3,0,0x0,&message_bind},
 {0x00403316,0x00403de0,0,0,0x0,&message_bind},
 {0x00403334,0x00401e90,2,0,0x0,&message_bind},
 {0x00403350,0x005ed4d0,0,0,0x0,&message_bind},
};
}

namespace kotor_dispatch {
extern const RouteTable route_table;
const RouteTable route_table = {
    winmain_rows, sizeof(winmain_rows) / sizeof(winmain_rows[0]),
    shell_rows, sizeof(shell_rows) / sizeof(shell_rows[0]),
    reinit_rows, sizeof(reinit_rows) / sizeof(reinit_rows[0]),
    message_rows, sizeof(message_rows) / sizeof(message_rows[0]),
};
}
