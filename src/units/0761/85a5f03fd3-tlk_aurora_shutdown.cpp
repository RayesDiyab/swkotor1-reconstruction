// lane-m46-startup: shutdown callees 0041E100 CTlkTable::~CTlkTable and 0044ED80 CAurInternal::
// DestroyCAuroraInstance (jmp 0044D940), declared in routes-lane kotor_original_calls.h:150/152.
// Evidence: lane-m46-startup/dis/0041e100.asm (+tail 0041E19A..0041E19B), 0041d900.asm, 0044ed80.asm, 0044d940.asm.
#include "6bdce58525-winmain_routes.h"
#include "a0c44e02ca-tlk_core.h"                // lane-tlk typed CTlkTable table kotor_vtable_ctlktable_0073ecdc (D44)
#include "6a2ab66935-aurora_window_slots.h"     // lane-window typed Aurora table / table_of (D41)
#include "4691f07671-factory_contracts.h"       // kotor_aurora_factory::views (singleton 007B921C, counter 007B9218)
#include "4b90ecc798-startup_string.h"
#include "d08b97bcca-allocator_live.h"
#include <cstdint>
#include <cstring>
#include <new>

extern "C" void __fastcall file_destroy_005e69e0(void*, void*);   // CExoFile::~CExoFile owner (0041D900 = jmp 005E69E0)

namespace {
using Word = std::uint32_t;
using String = kotor_reconstruction::StartupString;
Word word_at(const void* o, Word off) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(o) + off, 4); return v; }
void put_word(void* o, Word off, Word v) { std::memcpy(static_cast<unsigned char*>(o) + off, &v, 4); }
void* pointer_at(const void* o, Word off) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word_at(o, off))); }
// 006FA852(a, size, [a-4], 0059EA20 = ~String(element+4)) then free(a-4): last-to-first.
void destroy_cookie_array_string4(unsigned char* a, Word size) {
    const Word count = word_at(a - 4, 0);
    for (Word n = count; n != 0; --n) std::launder(reinterpret_cast<String*>(a + (n - 1) * size + 4))->~String();
    live_free(a - 4);
}
}

// 0041E100..0041E19B CTlkTable::~CTlkTable (RET 0).
//  [this] = CTlkTable table 0073ECDC (lane-tlk typed constant); a=[+48]: 0Ch elements; a=[+40]: 24h elements
//  (both element dtor 0059EA20, no clear); then 7 file pairs (k=0..6): f=[+8+8k]: if f && f != [+4+8k]
//  { 0041D900(f) (= jmp 005E69E0); free(f) }; g=[+4+8k]: if g { 0041D900(g); free(g) }. No clears.
extern "C" void __fastcall kotor_fastcall_0041e100(void* self, void*) {
    const void* const table = &kotor_vtable_ctlktable_0073ecdc;
    std::memcpy(self, &table, 4);                                                            // 0041E10A
    if (unsigned char* a = static_cast<unsigned char*>(pointer_at(self, 0x48))) destroy_cookie_array_string4(a, 0x0c);   // 0041E104..0041E12C
    if (unsigned char* a = static_cast<unsigned char*>(pointer_at(self, 0x40))) destroy_cookie_array_string4(a, 0x24);   // 0041E12F..0041E150
    for (Word k = 0; k < 7; ++k) {                                                           // 0041E153..0041E196
        const Word second = 8 + 8 * k, first = 4 + 8 * k;
        void* const f = pointer_at(self, second);                                            // 0041E160
        if (f && f != pointer_at(self, first)) { file_destroy_005e69e0(f, nullptr); live_free(f); }   // 0041E162..0041E178
        if (void* g = pointer_at(self, first)) { file_destroy_005e69e0(g, nullptr); live_free(g); }   // 0041E17B..0041E18F
    }
}

// 0044ED80 jmp 0044D940 CAurInternal::DestroyCAuroraInstance (stdcall(void), RET 0):
//  p=[007B921C]; if p { if --[007B9218] == 0 { p->vtable[0](1) (typed lane-window table, D41); [007B921C]=0 } }.
extern "C" void __cdecl kotor_cdecl_0044ed80() {
    const kotor_aurora_factory::Views& v = *kotor_aurora_factory::views;
    void* const p = *v.singleton_007b921c;                                                   // 0044D940
    if (!p) return;                                                                          // 0044D946..0044D948
    const Word next = *v.counter_007b9218 - 1u;                                              // 0044D94A (dec [mem])
    *v.counter_007b9218 = next;
    if (next != 0) return;                                                                   // 0044D950
    kotor_renderer_init::Renderer* const r = static_cast<kotor_renderer_init::Renderer*>(p);
    (void)kotor_aurora_window::table_of(r)->destroy(r, nullptr, 1);                          // 0044D952..0044D956
    *v.singleton_007b921c = nullptr;                                                         // 0044D958
}
