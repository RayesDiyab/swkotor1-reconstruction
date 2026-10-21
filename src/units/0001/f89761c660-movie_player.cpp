// lane-m47-core rev2: CExoMoviePlayer / CExoMoviePlayerInternal (the movie player created on the first frame by
// CClientExoAppInternal::InitializeMoviePlayer 005F2BF0, called from StartServices 005F9488).
//   0070C170 CExoMoviePlayer::CExoMoviePlayer            00405D70 CExoMoviePlayerInternal::CExoMoviePlayerInternal
//   004058F0 CExoArrayList<CExoString>::CExoArrayList    00405920 ~CExoArrayList   00405080 ::Allocate   00405950 ::SetSize
//   00405B30 MoviePlaybackWorker (CreateThread start, created running; waits on the +04 event)
//   004053E0 InitializeMovie   00404C80 PlayMovieLoop   00404DE0 StopMovie   00404BB0 ShutDown   00404C20 PrepareCancel
//   00405190 MovieWinMessageHandler (class "SWMovieWindow")   00404F40 UpdateWindowPosition   00404F80 RefreshWindowLayout
// 00404C40 CancelMovie is lane-m47-core client_ingame_teardown.cpp (kotor_fastcall_00404c40).
// Route note: with the documented first-frame route (Disable Movies=1, lane-firstframe PATH.md:107) only the constructor
// chain and the waiting worker run; everything after the worker's first wait is link-required.
// Evidence: rev2/dis/*.asm (tools/iat.py annotates import slots), EH maps 007141 7A/007140DB/0071410B decoded from FuncInfo
// (tools rd.py), .rdata strings 0073D7FC/0073D7E4/0073D62C/0073D7D8/0073D790/0073D774 and float 0073D770 (= 2^32) read
// with tools/rd.py and reproduced as literals of identical bytes.
// Imports: Win32 through the existing kotor_renderer_init IAT adapters where one exists, otherwise the Win32 API;
// binkw32.dll and mss32.dll entries are declared as the original imports (__imp__Bink*@N) - binkw32 needs a library
// rule like D77 (open gate in PACKET-REV2.json).
// lane-m1-movies-play player rev3 (replacement of unit lane-m47-core-rev2-movie-player; only these changes):
//  PL-1 00404BB0 / PL-2 00404C80: BinkGetSummary writes a 7Ch-byte BINKSUMMARY. The original reserves exactly 7Ch
//       bytes for it (00404BB0: sub esp,7Ch, summary at [esp+8] after 2 pushes = the whole 7Ch area; 00404C80:
//       summary at [esp+20h] = offset 1Ch of the 98h area -> 7Ch bytes to its end). rev2 sized the locals 74h/78h,
//       so the call overwrote the /GS cookie (and saved EBP in 00404BB0) -> __report_gsfailure at the end of every
//       movie (StopMovie 00404DE0 -> ShutDown) and at the end of PlayMovieLoop.
// lane-m60-binkaudio rev4 (replacement of this unit; only this change):
//  BA-1 00405779: the CClientExoAppInternal +4BCh flag that zeroes the movie volume is read from [[[7A39FC]+4]+4]+4BCh
//       (one deref fewer than rev3). rev3 read [[[[7A39FC]+4]+4]+0]+4BCh -> nonzero -> BinkSetVolume(bink,0,0): no Bink audio.
//  PL-3 004057D5/004057F3: the two scaled sizes are converted by the original __ftol2 port (006FAE8C,
//       genuine_x87_ftol_006fae8c) straight from ST(0) as the original does, instead of fstp qword + modern __dtol3
//       (D30-FINAL: original CRT ports for _ftol2).
#include "b720f821ed-renderer_adoption.h"       // kotor_renderer_init::Renderer + renderer_adoption::VTable (slot +04)
#include "b1e10a9ac7-aa_probe.h"                // kotor_renderer_init::WindowClass, RegisterClassA/CreateWindowExA/DestroyWindow adapters
#include "2ff8e789bb-window_shared_cells.h"
#include "0e7edb1140-window_live_views.h"       // live Views: message.movie_window_7a3cf8, winmain.swapping_7a3c9c
#include "d3feb50159-live_publication.h"        // kotor_menu_window_attachment (sole live Views publication)
#include "63176f6f15-startup_string.h"
#include <cstdint>
#include <cstring>

namespace { using U32 = std::uint32_t; using I32 = std::int32_t; using kotor_reconstruction::StartupString; }

// ---- binkw32.dll / mss32.dll (original imports, see header) ----
extern "C" {
__declspec(dllimport) void* __stdcall BinkOpen(const char* name, U32 flags);
__declspec(dllimport) void __stdcall BinkClose(void* bink);
__declspec(dllimport) I32 __stdcall BinkSetSoundSystem(void* open, U32 param);
__declspec(dllimport) void* __stdcall BinkOpenMiles(U32 param);
__declspec(dllimport) void __stdcall BinkSetVolume(void* bink, U32 track, I32 volume);
__declspec(dllimport) I32 __stdcall BinkDoFrame(void* bink);
__declspec(dllimport) void __stdcall BinkNextFrame(void* bink);
__declspec(dllimport) I32 __stdcall BinkWait(void* bink);
__declspec(dllimport) I32 __stdcall BinkCopyToBuffer(void* bink, void* dest, I32 pitch, U32 height, U32 x, U32 y, U32 flags);
__declspec(dllimport) I32 __stdcall BinkGetRects(void* bink, U32 flags);
__declspec(dllimport) void __stdcall BinkGetSummary(void* bink, void* summary);
__declspec(dllimport) I32 __stdcall BinkPause(void* bink, I32 pause);
__declspec(dllimport) void* __stdcall BinkBufferOpen(void* wnd, U32 width, U32 height, U32 flags);
__declspec(dllimport) void __stdcall BinkBufferClose(void* buffer);
__declspec(dllimport) I32 __stdcall BinkBufferLock(void* buffer);
__declspec(dllimport) I32 __stdcall BinkBufferUnlock(void* buffer);
__declspec(dllimport) void __stdcall BinkBufferBlit(void* buffer, void* rects, U32 count);
__declspec(dllimport) I32 __stdcall BinkBufferSetScale(void* buffer, U32 width, U32 height);
__declspec(dllimport) I32 __stdcall BinkBufferSetOffset(void* buffer, I32 x, I32 y);
__declspec(dllimport) I32 __stdcall BinkBufferCheckWinPos(void* buffer, I32* x, I32* y);
__declspec(dllimport) void __stdcall AIL_quick_handles(void* dig, void* mdi, void* dls);
}

// ---- genuine providers (M46 stage) ----
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*);
                         struct ClientApp { __declspec(noinline) void* __thiscall get_options_005ed700(); };
                         struct Debug; }
namespace options_video { struct String; }
namespace client_movies { int __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*); }
namespace base_ini_reconstruction { void __fastcall warning_005e9400(kotor_menu20::Debug*, void*, int, const char*, const char*); }
namespace alias_path { struct AliasListView; }
namespace path_resolution { StartupString* __fastcall resolve_005e68a0(alias_path::AliasListView*, void*, StartupString*, StartupString*, U32); }
namespace menu_tables_m41 {
using Constructor = void* (__fastcall*)(void*, void*);
using Destructor = void (__fastcall*)(void*, void*);
void __stdcall VectorConstruct_006fb55f(void* destination, U32 size, U32 count, Constructor ctor, Destructor dtor);
}
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }    // CExoBase (007A39E0)
namespace kotor_renderer_gl_cells { extern std::uint32_t width_0078d1d4; }
namespace kotor_startup { struct CriticalSection { CriticalSection* __thiscall Construct(); void __thiscall Destroy(); }; }
namespace kotor_renderer_init { I32 __stdcall DefWindowProcA_0073d3b0(void*, U32, U32, I32); }
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
StartupString* __fastcall GuiAssign005e5140(StartupString*, void*, const char*);
char* __fastcall WindowCStr005e5670(StartupString*, void*);
void* __fastcall exo_array_list_construct_00416770(void*, void*, std::uint32_t);
void __fastcall exo_array_list_destruct_0056b6f0(void*, void*);
void __stdcall c2da_destroy_array_006fa852(void* first, std::uint32_t size, std::int32_t count, void (__fastcall* destroy)(void*, void*));
std::uint32_t __stdcall unresolved_create_thread_0073d1f0(void*, std::uint32_t, std::uint32_t (__stdcall*)(void*), void*, std::uint32_t, std::uint32_t*);
std::uint32_t __stdcall manager_wait_single_0073d05c(std::uint32_t, std::uint32_t);
__declspec(noreturn) void __stdcall rim_exit_thread_0073d1e0(std::uint32_t);
void __stdcall c2da_sleep_0073d1e4(std::uint32_t);
void __fastcall kotor_fastcall_00404c40(void* self, void*, U32 a, U32 b);         // CancelMovie (lane-m47-core)
void __cdecl genuine_x87_ftol_006fae8c();                                          // __ftol2 006FAE8C port (renderer-x87-ftol-alias)
}
StartupString* __fastcall GuiConcat005e5d10(StartupString*, void*, StartupString*, const StartupString*);
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); }

namespace kotor_movie {
// .data/.bss cells with no other owner (full-image xref: tools/xref.py).
void* volatile current_internal_007a3cf4 = nullptr;   // written 00405B3A; read by 00405190 only
U32 volatile class_registered_007a3cfc = 0;           // 004053EF / 004054BB
U32 volatile movie_restore_007a3cc0 = 0;              // 00405C70 / 00405CE6 (also read by 00403E1F RestoreResolutionFromMovie)
const char class_name_0073d7fc[] = "SWMovieWindow";
const char window_name_0073d7e4[] = "SW Movie Player Window";
const char error_caption_0073d62c[] = "Error";
const char movies_format_0073d7d8[] = "MOVIES:%s";
const char cant_open_0073d790[] = "CExoMoviePlayerInternal::CExoMoviePlayerInternal()  Can't open movie: ";
const char source_file_0073d774[] = "exomovieplayerinternal.cpp";
const char empty_0073d71c[] = "";
const U32 two_pow_32_0073d770 = 0x4F800000u;           // 4294967296.0f (unsigned fild fix-up)

struct MovieList { StartupString* data; I32 size; I32 alloc; };          // CExoArrayList<CExoString>
struct IntList { I32* data; I32 size; I32 alloc; };                      // CExoArrayList<int> (+90)
struct Internal {                                                        // 0xA0 (0070C187 push 0A0h)
    U32 quit;                    // +00 worker exits when nonzero (00405B55, 00405CFE)
    U32 event;                   // +04 CreateEventA (00405DF3)
    MovieList movies;            // +08
    U32 cancel_requested;        // +14
    U32 cancel_arg;              // +18
    U32 keep_list;               // +1C (00404C29)
    U32 restore_mode;            // +20
    U32 shutting_down;           // +24 (00404BBE)
    unsigned char name28[8];     // +28 CExoString
    U32 playing;                 // +30
    U32 u34;                     // +34
    U32 thread;                  // +38
    U32 thread_id;               // +3C
    kotor_startup::CriticalSection lock; // +40
    U32 u44;                     // +44
    void* bink;                  // +48
    void* buffer;                // +4C
    void* window;                // +50
    unsigned char u54[0x0C];     // +54..+5F
    U32 u60, u64;                // +60/+64
    unsigned char file68[8];     // +68 CExoString
    U32 u70;                     // +70
    unsigned char u74[0x10];     // +74..+83
    I32 offset_x, offset_y;      // +84/+88
    U32 u8c;                     // +8C
    IntList skip;                // +90
    U32 skip_current;            // +9C
};
static_assert(sizeof(Internal) == 0xA0 && offsetof(Internal, lock) == 0x40 && offsetof(Internal, bink) == 0x48 &&
              offsetof(Internal, file68) == 0x68 && offsetof(Internal, offset_x) == 0x84 && offsetof(Internal, skip) == 0x90 &&
              offsetof(Internal, skip_current) == 0x9C, "observed offsets");
StartupString* str(unsigned char* p) { return reinterpret_cast<StartupString*>(p); }
U32 word(const void* p, U32 off) { U32 v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
void* ptr(const void* p, U32 off) { void* v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
HWND render_window() { return kotor_menu_window_cells::render_007a39d8; }
kotor_menu_live_views::Views* views() { return kotor_menu_window_attachment->views; }   // 007A3CF8 / 007A3C9C owner
// D24 typed element leaves for the vector iterators (forward to the single 005B3190 / 005E5C20 owners).
void* __fastcall string_element_construct_005b3190(void* element, void*) {
    return kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(element), nullptr);
}
void __fastcall string_element_destroy_005e5c20(void* element, void*) { static_cast<StartupString*>(element)->~StartupString(); }
void destroy_elements(StartupString* data) {                               // count word at data[-4]
    I32 count; std::memcpy(&count, reinterpret_cast<unsigned char*>(data) - 4, 4);
    c2da_destroy_array_006fa852(data, 8, count, &string_element_destroy_005e5c20);
    live_free(reinterpret_cast<unsigned char*>(data) - 4);
}
kotor_renderer_init::Renderer* aurora() { return static_cast<kotor_renderer_init::Renderer*>(kotor_menu_window_cells::aurora_007a39f8); }
const renderer_adoption::VTable* aurora_table(kotor_renderer_init::Renderer* r) {
    return reinterpret_cast<const renderer_adoption::VTable*>(static_cast<std::uintptr_t>(r->vtable_word));
}
void* exo_base_word(U32 off) { return ptr(kotor_menu_10_19::dependencies::base_007a39e0, off); }
}
using namespace kotor_movie;

extern "C" {
U32 __stdcall kotor_stdcall_00405b30(void* parameter);
I32 __stdcall kotor_stdcall_00405190(void* window, U32 message, U32 wparam, I32 lparam);

// 00405080 CExoArrayList<CExoString>::Allocate(n) RET 4.
__declspec(noinline) void __fastcall kotor_fastcall_00405080(MovieList* self, void*, I32 count) noexcept(false) {
    StartupString* const old = self->data;                                          // 0040509F
    self->alloc = count;                                                            // 004050A9
    unsigned char* const memory = static_cast<unsigned char*>(live_operator_new(static_cast<U32>(count) * 8u + 4u)); // 004050A1..AC
    StartupString* fresh = nullptr;
    if (memory) {                                                                   // 004050BA..C0 (EH state 0)
        std::memcpy(memory, &count, 4);                                             // 004050D3
        try {
            menu_tables_m41::VectorConstruct_006fb55f(memory + 4, 8, static_cast<U32>(count),
                &string_element_construct_005b3190, &string_element_destroy_005e5c20);   // 004050C2..D5
        } catch (...) { live_free(memory); throw; }                                  // 007140D0
        fresh = reinterpret_cast<StartupString*>(memory + 4);
    }
    self->data = fresh;                                                             // 004050E9
    for (I32 i = 0; i < self->size; ++i) fresh[i].Assign005e5c50(&old[i]);          // 004050F0..0A (data reloaded: [esi])
    if (old) destroy_elements(old);                                                 // 0040510C..2A
}
// 004058F0 CExoArrayList<CExoString>::CExoArrayList(n) RET 4.
__declspec(noinline) MovieList* __fastcall kotor_fastcall_004058f0(MovieList* self, void*, I32 count) noexcept(false) {
    self->size = 0; self->alloc = 0; self->data = nullptr;                          // 004058FB..01
    if (count != 0) kotor_fastcall_00405080(self, nullptr, count);                  // 00405903..08
    return self;
}
// 00405920 ~CExoArrayList<CExoString>.
__declspec(noinline) void __fastcall kotor_fastcall_00405920(MovieList* self, void*) {
    if (self->data) destroy_elements(self->data);                                   // 00405923..44
    self->data = nullptr;                                                           // 00405948
}
// 00405950 CExoArrayList<CExoString>::SetSize(n) RET 4.
__declspec(noinline) void __fastcall kotor_fastcall_00405950(MovieList* self, void*, I32 count) noexcept(false) {
    if (count == 0) {
        if (self->data) {                                                           // 0040595C..60
            destroy_elements(self->data);                                           // 00405962..7D
            self->data = nullptr; self->alloc = 0; self->size = 0;                  // 00405981..86
            return;
        }
    } else {
        kotor_fastcall_00405080(self, nullptr, count);                              // 0040598E..91
    }
    self->size = count;                                                             // 00405996
}

// 00405D70 CExoMoviePlayerInternal::CExoMoviePlayerInternal.
__declspec(noinline) Internal* __fastcall kotor_fastcall_00405d70(Internal* self, void*) noexcept(false) {
    kotor_fastcall_004058f0(&self->movies, nullptr, 0);                             // 00405D8D..95 (state 0)
    try {
        kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(self->name28), nullptr); // 00405DA3 (state 1)
        try {
            self->lock.Construct();                                                 // 00405DB0 (state 2)
            try {
                kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(self->file68), nullptr); // 00405DBD (state 3)
                try {
                    exo_array_list_construct_00416770(&self->skip, nullptr, 0);     // 00405DCE (state 4)
                    try {
                        GuiAssign005e5140(str(self->name28), nullptr, empty_0073d71c);   // 00405DD3..DF
                        self->playing = 0; self->quit = 0;                          // 00405DE8..EB
                        self->event = static_cast<U32>(reinterpret_cast<std::uintptr_t>(
                            ::CreateEventA(nullptr, FALSE, FALSE, nullptr)));       // 00405DE4..F3
                        const U32 thread = unresolved_create_thread_0073d1f0(nullptr, 0x8000, &kotor_stdcall_00405b30,
                                                                             self, 0, &self->thread_id); // 00405DF6..0D
                        self->u34 = 0; self->cancel_requested = 0; self->cancel_arg = 0; self->keep_list = 0;  // 00405E11..1A
                        self->restore_mode = 0; self->shutting_down = 0; self->u44 = 0; self->buffer = nullptr; // 00405E1D..26
                        self->bink = nullptr; self->u70 = 0; self->window = nullptr;   // 00405E29..2F
                        self->thread = thread;                                      // 00405E32
                        return self;
                    } catch (...) { exo_array_list_destruct_0056b6f0(&self->skip, nullptr); throw; } // 0071416C
                } catch (...) { str(self->file68)->~StartupString(); throw; }      // 00714161
            } catch (...) { self->lock.Destroy(); throw; }                         // 00714156 (005E9320)
        } catch (...) { str(self->name28)->~StartupString(); throw; }              // 0071414B
    } catch (...) { kotor_fastcall_00405920(&self->movies, nullptr); throw; }      // 00714140
}
// 0070C170 CExoMoviePlayer::CExoMoviePlayer: one word, the internal (0xA0).
__declspec(noinline) void* __fastcall kotor_fastcall_0070c170(void* self, void*) noexcept(false) {
    void* const memory = live_operator_new(0xA0);                                   // 0070C187..8E
    Internal* internal = nullptr;
    if (memory) {                                                                   // EH state 0 (0073A730 frees)
        try { internal = kotor_fastcall_00405d70(static_cast<Internal*>(memory), nullptr); }   // 0070C1A6..A8
        catch (...) { live_free(memory); throw; }
    }
    std::memcpy(self, &internal, 4);                                                // 0070C1AD / 0070C1C7
    return self;
}

// 00404C20 PrepareMovieForCancellation(mode) RET 4.
__declspec(noinline) void __fastcall kotor_fastcall_00404c20(Internal* self, void*, U32 mode) {
    const U32 current = self->restore_mode;                                         // 00404C20
    self->keep_list = 1;                                                            // 00404C29
    if (current == 0 || mode == 1) self->restore_mode = mode;                       // 00404C30..37
}
// 00404BB0 ShutDown.
__declspec(noinline) void __fastcall kotor_fastcall_00404bb0(Internal* self, void*) noexcept(false) {
    U32 summary[0x7C / 4];                                                          // PL-1: [esp+8] = the whole 7Ch frame (BINKSUMMARY)
    self->shutting_down = 1;                                                        // 00404BBE
    if (self->bink != nullptr) {                                                    // 00404BB6..C5
        BinkGetSummary(self->bink, summary);                                        // 00404BC7..CD
        (void)BinkPause(self->bink, 1);                                             // 00404BD3..D9
        BinkClose(self->bink);                                                      // 00404BDF..E3
        self->bink = nullptr;                                                       // 00404BEE
        if (self->buffer != nullptr) { BinkBufferClose(self->buffer); self->buffer = nullptr; } // 00404BE9..FA
        (void)kotor_renderer_init::DestroyWindow_0073d3fc(self->window);           // 00404BFD..01
        self->window = nullptr;                                                     // 00404C07
        views()->message.movie_window_7a3cf8 = 0;                                   // 00404C0A [007A3CF8]
    }
    self->shutting_down = 0;                                                        // 00404C10
}
// 00404DE0 StopMovie.
__declspec(noinline) void __fastcall kotor_fastcall_00404de0(Internal* self, void*) noexcept(false) {
    kotor_fastcall_00404bb0(self, nullptr);                                         // 00404DE3
    self->playing = 0;                                                              // 00404DE8
}
// 00404C80 PlayMovieLoop(unused) RET 4: returns 0.
__declspec(noinline) U32 __fastcall kotor_fastcall_00404c80(Internal* self, void*, U32) {
    MSG message;                                                                    // [esp+10h] (0x1C bytes)
    U32 summary[0x7C / 4];                                                          // PL-2: [esp+20h] = 98h frame offset 1Ch -> 7Ch bytes (BINKSUMMARY)
    void* bink = self->bink;
    if (bink == nullptr) return 0;                                                  // 00404C89..8E
    if (word(bink, 8) != word(bink, 0x0C)) {                                        // 00404C94..9A
        do {
            (void)BinkDoFrame(self->bink);                                          // 00404CB5..B6 (bink reloaded)
            if (BinkBufferLock(self->buffer) != 0) {                                // 00404CBC..C8
                void* const b = self->buffer;
                (void)BinkCopyToBuffer(self->bink, ptr(b, 0x14), static_cast<I32>(word(b, 0x18)), word(b, 4), 0, 0,
                                       word(b, 0x10));                              // 00404CCA..E5
                (void)BinkBufferUnlock(self->buffer);                               // 00404CEB..EF
            }
            const I32 rects = BinkGetRects(self->bink, word(self->buffer, 0x10));   // 00404CF5..00
            BinkBufferBlit(self->buffer, static_cast<unsigned char*>(self->bink) + 0x34, static_cast<U32>(rects)); // 00404D06..12
            BinkNextFrame(self->bink);                                              // 00404D18..1C
            if (BinkWait(self->bink) == 1) {                                        // 00404D22..2B
                do { c2da_sleep_0073d1e4(1); } while (BinkWait(self->bink) == 1);   // 00404D30..3D
            }
            if (self->cancel_requested == 1) break;                                 // 00404D3F..43
            if (::PeekMessageA(&message, nullptr, 0, 0, PM_NOREMOVE)) {             // 00404D45..56
                do {
                    if (!::GetMessageA(&message, nullptr, 0, 0)) break;             // 00404D58..6B
                    ::TranslateMessage(&message);                                   // 00404D6D..72
                    ::DispatchMessageA(&message);                                   // 00404D78..7D
                } while (::PeekMessageA(&message, nullptr, 0, 0, PM_NOREMOVE));     // 00404D83..94
            }
        } while (word(self->bink, 8) != word(self->bink, 0x0C));                    // 00404D96..9F
    }
    BinkGetSummary(self->bink, summary);                                            // 00404DA8..B1
    return 0;                                                                       // 00404DB7
}
// 00404F40 UpdateWindowPosition(WINDOWPOS*) RET 4.
__declspec(noinline) void __fastcall kotor_fastcall_00404f40(Internal* self, void*, void* position) {
    if (self->buffer == nullptr) return;                                            // 00404F41..46
    I32 x = static_cast<I32>(word(position, 8)), y = static_cast<I32>(word(position, 0x0C));  // 00404F4D..5B
    (void)BinkBufferCheckWinPos(self->buffer, &x, &y);                              // 00404F5F..66
    std::memcpy(static_cast<unsigned char*>(position) + 8, &x, 4);                  // 00404F74
    std::memcpy(static_cast<unsigned char*>(position) + 0x0C, &y, 4);               // 00404F77
}
// 00404F80 RefreshWindowLayout(unused) RET 4.
__declspec(noinline) void __fastcall kotor_fastcall_00404f80(Internal* self, void*, U32) {
    if (self->buffer != nullptr) (void)BinkBufferSetOffset(self->buffer, self->offset_x, self->offset_y); // 00404F86..9C
    WINDOWINFO info; info.cbSize = 0x3C;                                            // 00404FA8..AE
    if (!::GetWindowInfo(render_window(), &info)) return;                           // 00404FB6..BE
    const RECT& c = info.rcClient;
    if (!::MoveWindow(static_cast<HWND>(self->window), c.left, c.top, c.right - c.left, c.bottom - c.top, TRUE)) // 00404FC0..E6
        (void)::GetLastError();                                                     // 00404FE8
    void* const dc = kotor_renderer_init::GetDC_0073d408(render_window());          // 00404FEE..F6
    (void)::PatBlt(static_cast<HDC>(dc), 0, 0, static_cast<int>(kotor_renderer_gl_cells::width_0078d1d4),
                   static_cast<int>(kotor_menu_window_cells::height_0078d1d8), BLACKNESS);  // 00404FFC..12
    (void)kotor_renderer_init::ReleaseDC_0073d404(render_window(), dc);             // 00405018..20
    (void)kotor_renderer_init::ShowWindow_0073d400(self->window, SW_SHOWNA);        // 00405026..2C
}

// 004053E0 InitializeMovie(CExoString* name) RET 4: 1 on success, 0 on failure.
__declspec(noinline) U32 __fastcall kotor_fastcall_004053e0(Internal* self, void*, StartupString* name) noexcept(false) {
    self->window = nullptr;                                                         // 00405409
    views()->message.movie_window_7a3cf8 = 0;                                       // 0040540C
    if (class_registered_007a3cfc == 0) {                                           // 004053EE..17
        kotor_renderer_init::WindowClass wc;
        wc.style = 0;                                                               // 00405425
        wc.procedure = &kotor_stdcall_00405190;                                     // 00405429
        wc.class_extra = 0; wc.window_extra = 0;                                    // 00405431..35
        wc.instance = kotor_menu_window_cells::instance_007a39c0;                   // 00405439
        wc.icon = ::LoadIconA(kotor_menu_window_cells::instance_007a39c0, MAKEINTRESOURCEA(0x65)); // 0040541D..49
        wc.cursor = ::LoadCursorA(nullptr, MAKEINTRESOURCEA(0x7F00));               // 00405443..53
        wc.background = nullptr; wc.menu = nullptr;                                 // 0040545C..60
        wc.class_name = class_name_0073d7fc;                                        // 00405464
        if (kotor_renderer_init::RegisterClassA_0073d41c(&wc) == 0) {               // 0040546C..75
            char* text = nullptr;
            ::FormatMessageA(0x1300, nullptr, ::GetLastError(), 0x400, reinterpret_cast<char*>(&text), 0, nullptr); // 00405477..90
            ::MessageBoxA(nullptr, text, error_caption_0073d62c, 0x40);             // 00405496..A3
            ::LocalFree(text);                                                      // 004054A9..AE
            return 0;
        }
        class_registered_007a3cfc = 1;                                              // 004054BB
    }
    self->playing = 1;                                                              // 004054C5
    WINDOWINFO info; info.cbSize = 0x3C;                                            // 004054C1..D0
    if (!::GetWindowInfo(render_window(), &info)) return 0;                         // 004054D8..E0
    const I32 left = info.rcClient.left, top = info.rcClient.top;                  // 004054E2..ED (saved [T+20]/[T+24])
    const I32 right = info.rcClient.right, bottom = info.rcClient.bottom;          // edi / ebp
    self->window = kotor_renderer_init::CreateWindowExA_0073d410(0, class_name_0073d7fc, window_name_0073d7e4, 0x90000000u,
        left, top, right - left, bottom - top, render_window(), nullptr, kotor_menu_window_cells::instance_007a39c0, nullptr); // 004054F1..36
    views()->message.movie_window_7a3cf8 = static_cast<U32>(reinterpret_cast<std::uintptr_t>(self->window)); // 00405540
    ::SetCapture(static_cast<HWND>(self->window));                                  // 0040553F..45
    kotor_renderer_init::Renderer* const r = aurora();                              // 0040554B..53 [vtbl+4]
    (void)aurora_table(r)->fullscreen(r, nullptr);
    (void)kotor_renderer_init::SetWindowLongA_0073d3ec(self->window, -16, 0x82000000u);   // 00405556..61
    I32 width = right - left;                                                       // 00405579..7B (ebx; [T+10])
    (void)kotor_renderer_init::SetWindowPos_0073d40c(self->window, HWND_TOPMOST, left, top, static_cast<U32>(width),
                                                     static_cast<U32>(bottom - top), 0x121); // 00405567..8A
    self->bink = nullptr; self->u60 = 0; self->u64 = 0;                             // 0040559B..A1
    if (std::strchr(WindowCStr005e5670(name, nullptr), '.') != nullptr) {           // 00405599..BF
        str(self->file68)->Assign005e5c50(name);                                    // 0040563D
    } else {
        str(self->file68)->Assign005e5c50(name);                                    // 004055C1
        if (std::strchr(WindowCStr005e5670(str(self->file68), nullptr), ':') == nullptr)   // 004055C6..DB
            kotor_reconstruction::StartupFormat005e5680(str(self->file68), movies_format_0073d7d8,
                                                       WindowCStr005e5670(str(self->file68), nullptr));  // 004055DD..F4
        alignas(4) unsigned char resolved[8];                                       // [T+58]
        StartupString* const out = path_resolution::resolve_005e68a0(
            static_cast<alias_path::AliasListView*>(exo_base_word(0x0C)), nullptr,
            reinterpret_cast<StartupString*>(resolved), str(self->file68), 0x80F); // 004055F7..0E
        try { str(self->file68)->Assign005e5c50(out); }                             // 00405613..22 (state 0)
        catch (...) { reinterpret_cast<StartupString*>(resolved)->~StartupString(); throw; }  // 007140F0
        reinterpret_cast<StartupString*>(resolved)->~StartupString();               // 00405627..36
    }
    void* dig = nullptr;                                                            // [T+1C]
    AIL_quick_handles(&dig, nullptr, nullptr);                                      // 00405642..4B
    (void)BinkSetSoundSystem(reinterpret_cast<void*>(&BinkOpenMiles), static_cast<U32>(reinterpret_cast<std::uintptr_t>(dig))); // 00405651..5D
    self->bink = BinkOpen(WindowCStr005e5670(str(self->file68), nullptr), 0x8000000u);   // 00405663..79
    if (self->bink == nullptr) {
        alignas(4) unsigned char message_bytes[8];                                  // [T+14]
        StartupString* const message = reinterpret_cast<StartupString*>(message_bytes);
        kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(message_bytes), nullptr);  // 00405682..86 (state 1)
        try {
            GuiAssign005e5140(message, nullptr, cant_open_0073d790);                // 00405690..A0
            alignas(4) unsigned char sum_bytes[8];                                  // [T+60]
            StartupString* const sum = GuiConcat005e5d10(message, nullptr, reinterpret_cast<StartupString*>(sum_bytes),
                                                         str(self->file68));        // 004056A5..B2 (state 2)
            try { message->Assign005e5c50(sum); }                                   // 004056B7..C4
            catch (...) { reinterpret_cast<StartupString*>(sum_bytes)->~StartupString(); throw; }   // 00714103
            reinterpret_cast<StartupString*>(sum_bytes)->~StartupString();          // 004056C9..D4
            base_ini_reconstruction::warning_005e9400(static_cast<kotor_menu20::Debug*>(exo_base_word(8)), nullptr, 0x43B,
                source_file_0073d774, WindowCStr005e5670(message, nullptr));        // 004056D9..F6
            kotor_fastcall_00404bb0(self, nullptr);                                 // 004056FB..FD
            self->playing = 0;                                                      // 00405706
        } catch (...) { message->~StartupString(); throw; }                         // 007140F8
        message->~StartupString();                                                  // 00405702..18
        return 0;
    }
    self->buffer = nullptr;                                                         // 00405727
    self->buffer = BinkBufferOpen(self->window, word(self->bink, 0), word(self->bink, 4), 0x5D000000u); // 00405724..43
    if (self->buffer == nullptr) {                                                  // 00405746
        BinkClose(self->bink);                                                      // 00405748..4C
        self->playing = 0;                                                          // 00405752
        return 0;
    }
    void* const client = ptr(kotor_menu_window_cells::manager_007a39fc, 4);         // 00405760..65
    const void* const options = static_cast<kotor_menu20::ClientApp*>(client)->get_options_005ed700();   // 00405768
    // BA-1 (lane-m60-binkaudio): 0040576D mov ecx,[7A39FC]; 00405773 mov edx,[ecx+4] (client); 00405776 mov ecx,[edx+4]
    // (CClientExoAppInternal); 00405779 mov edx,[ecx+4BC]. rev3 had a third ptr(...,0) deref -> read [[internal]+4BC]
    // (garbage past the 400h object at internal+0) -> level forced 0 -> BinkSetVolume(...,0) = silent movies.
    const void* const internal = ptr(ptr(kotor_menu_window_cells::manager_007a39fc, 4), 4);              // 0040576D..76
    I32 level = static_cast<const unsigned char*>(options)[0x48];                   // 00405781
    if (word(internal, 0x4BC) != 0) level = 0;                                      // 00405779..87
    BinkSetVolume(self->bink, 0, (level << 15) / 100);                              // 00405789..A6 (imul 51EB851F, sar 5)
    const U32 movie_width = word(self->bink, 0);                                    // 004057AC..AF
    if (static_cast<U32>(width) != movie_width) {                                   // 004057B1..B3
        // x87 sequence of 004057B5..004057F3 (CRT 53-bit precision control: each ST value is exactly a double);
        // __ftol2 (006FAE8C) truncates toward zero and the callers use EAX = low 32 bits of the 64-bit result.
        const I32 movie_width_i = static_cast<I32>(movie_width);
        const I32 movie_height_i = static_cast<I32>(word(self->bink, 4));          // 004057DC..DF
        const U32 fix = two_pow_32_0073d770;
        float scale = 0.0f; I32 scaled_w = 0, scaled_h = 0;
        __asm {
            fild    dword ptr movie_width_i        ; 004057B7..BB ([T+14] = width word)
            cmp     movie_width_i, 0               ; 004057B5 test eax,eax / 004057BF jge
            jge     width_unsigned
            fadd    dword ptr fix                  ; 004057C1 (0073D770 = 2^32)
        width_unsigned:
            fild    dword ptr width                ; 004057C7 [T+10] client width
            fdiv    st, st(1)                      ; 004057CB
            fstp    dword ptr scale                ; 004057CD -> [T+10] (float)
            fmul    dword ptr scale                ; 004057D1
            call    genuine_x87_ftol_006fae8c      ; 004057D5 __ftol2 (pops ST(0); EAX = low word)
            mov     scaled_w, eax                  ; 004057DA (ebx)
            fild    dword ptr movie_height_i       ; 004057E2
            cmp     movie_height_i, 0              ; 004057E5..E7
            jge     height_unsigned
            fadd    dword ptr fix                  ; 004057E9
        height_unsigned:
            fmul    dword ptr scale                ; 004057EF
            call    genuine_x87_ftol_006fae8c      ; 004057F3 __ftol2
            mov     scaled_h, eax                  ; 004057F9 ([T+10])
        }
        (void)BinkBufferSetScale(self->buffer, static_cast<U32>(scaled_w), static_cast<U32>(scaled_h)); // 004057F8..802
        self->offset_x = (right - scaled_w - left) / 2;                             // 00405808..19 (cdq/sub/sar: toward 0)
        self->offset_y = (bottom - scaled_h - top) / 2;                             // 0040581F..2C
    } else {
        self->offset_x = static_cast<I32>((static_cast<U32>(right) - movie_width - static_cast<U32>(left)) >> 1);      // 00405834..3C
        self->offset_y = static_cast<I32>((static_cast<U32>(bottom) - word(self->bink, 4) - static_cast<U32>(top)) >> 1); // 00405842..4F
    }
    (void)BinkBufferSetOffset(self->buffer, self->offset_x, self->offset_y);       // 00405855..67
    self->playing = 1;                                                              // 00405872
    while (views()->winmain.swapping_7a3c9c == 1) c2da_sleep_0073d1e4(1);           // 00405875..8C
    void* const dc = kotor_renderer_init::GetDC_0073d408(render_window());          // 0040588E..95
    (void)::PatBlt(static_cast<HDC>(dc), 0, 0, static_cast<int>(kotor_renderer_gl_cells::width_0078d1d4),
                   static_cast<int>(kotor_menu_window_cells::height_0078d1d8), BLACKNESS);  // 0040589B..B1
    (void)kotor_renderer_init::ReleaseDC_0073d404(render_window(), dc);             // 004058B7..BF
    (void)kotor_renderer_init::ShowWindow_0073d400(self->window, SW_SHOWNA);        // 004058C5..CB
    return 1;
}
// 00405190 MovieWinMessageHandler (WNDPROC, RET 10h).
I32 __stdcall kotor_stdcall_00405190(void* window, U32 message, U32 wparam, I32 lparam) {
    Internal* const current = static_cast<Internal*>(current_internal_007a3cf4);
    auto options_word54 = [] {                                                      // 004052F5..302 / 0040534F..35D
        void* const client = ptr(kotor_menu_window_cells::manager_007a39fc, 4);
        return word(static_cast<kotor_menu20::ClientApp*>(client)->get_options_005ed700(), 0x54);
    };
    switch (message) {
    case 0x407: {                                                                   // 0040537D..82 -> 004053BC
        void* const position = reinterpret_cast<void*>(static_cast<std::intptr_t>(lparam));
        kotor_fastcall_00404f40(static_cast<Internal*>(current_internal_007a3cf4), nullptr, position);   // 004053C0..C7
        live_free(position);                                                        // 004053CC..D2
        return 0;
    }
    case 0x408: {                                                                   // 00405384..85 -> 0040539B
        void* const block = reinterpret_cast<void*>(static_cast<std::intptr_t>(lparam));
        kotor_fastcall_00404f80(static_cast<Internal*>(current_internal_007a3cf4), nullptr,
                                static_cast<U32>(lparam));                          // 0040539F..A6
        live_free(block);                                                           // 004053AB..B1
        return 0;
    }
    case 0x203: case 0x204: case 0x206: case 0x201:                                 // -> 004053D5
        return 0;
    case 0x205:                                                                     // 0040533F..78 WM_RBUTTONUP
        if (current_internal_007a3cf4 == nullptr || options_word54() == 0) return 0;
        kotor_fastcall_00404c40(current_internal_007a3cf4, nullptr, 0, 0);          // 00405364..6E
        return 0;
    case 0x202:                                                                     // 004052E1..321 WM_LBUTTONUP
        if (current_internal_007a3cf4 == nullptr || options_word54() != 0) return 0;
        kotor_fastcall_00404c40(current_internal_007a3cf4, nullptr, 0, 0);          // 0040530D..17
        return 0;
    case 0x105:                                                                     // 00405299..D1 WM_SYSKEYUP
        if (wparam == 0x0D && (lparam & 0x20000000) != 0 && current != nullptr) {
            kotor_fastcall_00404c20(current, nullptr, 1);                           // 004052BC..BE
            kotor_fastcall_00404c40(current, nullptr, 1, 1);                        // 004052C3..C7 (ECX unchanged)
            return 0;
        }
        break;
    case 0x1C:                                                                      // 00405255..96 WM_ACTIVATEAPP
        if (wparam == 0 && current != nullptr && current->shutting_down == 0) {
            kotor_fastcall_00404c20(current, nullptr, wparam);                      // 00405276..77 (push edi = 0)
            kotor_fastcall_00404c40(current, nullptr, 1, 1);                        // 0040527C..80
        }
        break;
    case 0x100:                                                                     // 00405221..52 WM_KEYDOWN
        if (wparam == 0x1B && current != nullptr) kotor_fastcall_00404c40(current, nullptr, 0, 0);   // 0040522A..3C
        break;
    case 0x104:                                                                     // 004051DE..21E WM_SYSKEYDOWN
        if (wparam == 0x73 && (lparam & 0x20000000) != 0 && kotor_menu_window_cells::top_007a39d4 != nullptr)
            ::PostMessageA(kotor_menu_window_cells::top_007a39d4, 0x10, 0, 0);       // 004051F3..207
        break;
    default:
        break;
    }
    return kotor_renderer_init::DefWindowProcA_0073d3b0(window, message, wparam, lparam);   // 00405387..398 etc.
}

// 00405B30 MoviePlaybackWorker (thread start, stdcall).
U32 __stdcall kotor_stdcall_00405b30(void* parameter) {
    Internal* const self = static_cast<Internal*>(parameter);
    current_internal_007a3cf4 = self;                                               // 00405B3A
    if (self == nullptr) rim_exit_thread_0073d1e0(0);                               // 00405B38..43
    (void)manager_wait_single_0073d05c(self->event, 0xFFFFFFFFu);                    // 00405B49..4F
    while (self->quit == 0) {                                                       // 00405B55..57 / 00405CFE..00
        for (I32 i = 0; i < self->movies.size; ++i)                                 // 00405B60..86 (results unused)
            (void)client_movies::StringNotEqual_005e5390(reinterpret_cast<options_video::String*>(&self->movies.data[i]),
                                                         nullptr, empty_0073d71c);
        self->skip_current = 1;                                                     // 00405B88..90
        for (I32 i = 0; i < self->movies.size; ++i) {                               // 00405B96..C47
            StartupString* const entry = &self->movies.data[i];                     // 00405BA0..AA
            if (client_movies::StringNotEqual_005e5390(reinterpret_cast<options_video::String*>(entry), nullptr,
                                                       empty_0073d71c) == 0) continue;   // 00405BAC..BA
            const I32 skips = self->skip.size;                                      // 00405BC0
            if (skips > 0) self->skip_current = i < skips ? static_cast<U32>(self->skip.data[i]) : 1u;   // 00405BC6..DF
            const U32 started = kotor_fastcall_004053e0(self, nullptr, entry);      // 00405BE9..EC
            if (started == 1) {                                                     // 00405BF1..F4
                (void)kotor_fastcall_00404c80(self, nullptr, started);              // 00405BF6..F9 (push eax)
                c2da_sleep_0073d1e4(5);                                             // 00405BFE..C00
                if (self->keep_list == 0)                                           // 00405C06..0B
                    GuiAssign005e5140(&self->movies.data[i], nullptr, empty_0073d71c);   // 00405C0D..17 (data reloaded)
                kotor_fastcall_00404de0(self, nullptr);                             // 00405C1C..1E
            }
            if (self->skip_current != 0) {                                          // 00405C23..2B
                if (self->cancel_requested != 1) continue;                          // 00405C2D..31
                if (self->cancel_arg != 0) break;                                   // 00405C33..38 -> 00405C4D
            }
            self->cancel_requested = 0;                                             // 00405C3A
        }
        if (self->keep_list == 0) kotor_fastcall_00405950(&self->movies, nullptr, 0);    // 00405C54..5D
        const U32 keep = self->keep_list;                                           // 00405C62
        self->cancel_requested = 0; self->cancel_arg = 0; self->playing = 0;        // 00405C65..6B
        if (keep == 1) {                                                            // 00405C6E
            movie_restore_007a3cc0 = 1;                                             // 00405C70
            if (self->restore_mode == 1) {                                          // 00405C76..79
                kotor_renderer_init::Renderer* const r = aurora();                  // 00405C7B..83 [vtbl+4]
                if (aurora_table(r)->fullscreen(r, nullptr) == 1)
                    ::PostMessageA(render_window(), 0x409, 0, 0);                   // 00405C8B..97
                else
                    ::PostMessageA(render_window(), 0x409, 1, 0);                   // 00405CA5..B2
            } else {
                ::PostMessageA(render_window(), 0x1C, 0, 0);                        // 00405CC0..CB
            }
            self->restore_mode = 0; self->keep_list = 0;                            // 00405C9D..CD4
        } else {
            movie_restore_007a3cc0 = 0;                                             // 00405CE6
            ::PostMessageA(render_window(), 0x40A, 0, 0);                           // 00405CD9..EC
        }
        (void)manager_wait_single_0073d05c(self->event, 0xFFFFFFFFu);                // 00405CF2..F8
    }
    rim_exit_thread_0073d1e0(0);                                                    // 00405D08..09 (int 3 follows)
}
}
