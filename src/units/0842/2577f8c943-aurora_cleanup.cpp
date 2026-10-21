// lane-m47-core rev2 ADAPTED COPY of the M47 stage unit aurora-window-aurora-cleanup (aurora-window/aurora_cleanup.cpp = lane-window
// src/aurora_cleanup.cpp, base sha256 99ef04a609103e3800a0edf2269c0540cca93391343937d8e4ef10a413157f8a). Only change: the
// posted list 007B90FC/007B9100 is read through its single owner r4_lowers_cells::posted_list_007b90fc (.data/.count).
// lane-window: ~CAurInternalGL (0044EC60) cleanup callees.
//   0044D250 untrace(name)       cdecl, returns char* 007B9118     (0044D250..0044D316)
//   0044D1E0 AurStringsFree      cdecl                             (0044D1E0..0044D249)
//   0044CC60 AurFreeFont         cdecl                             (0044CC60..0044CC9E)
//   004A22E0 AurDeleteAllFonts   cdecl                             (004A22E0..004A2311)
// Cells: posted list 007B90FC/007B9100 (lane-r4-lowers r4_cells posted_list_007b90fc, rev2), trace list 007B9108/007B910C
// (lane-r4-lowers r4_cells.cpp) borrowed; 007B9118 message buffer, 007B90EC font object,
// 007B90F0 font flag, 007A47C0 display-list base and font list 0083052C (atexit 0073BE50 frees
// data) have no other owner and are owned here.
#include "64c8bc673c-window_lists.h"
#include "39c670fc66-allocator_live.h"
#include <windows.h>
#include <GL/gl.h>
#include <cstdint>
#include <cstdio>
#include <cstring>

using U32 = std::uint32_t; using I32 = std::int32_t;
namespace r4_lowers_cells {                                           // lane-r4-lowers r4_cells.cpp
// rev2 (D50/D59): 007B90FC is ONE 12-byte CExoArrayList<PostedString*> {data 007B90FC, count 007B9100, alloc 007B9104},
// sole owner lane-r4-lowers r4_cells.cpp (definition identical to r4_lowers.h:52-54). The former extern "C" spellings
// _kotor_cell_posted_table_007b90fc / _kotor_cell_posted_count_007b9100 had no definer (m47-compose INIT-1).
struct PostedList { std::uint32_t volatile data, count, alloc; };
static_assert(sizeof(PostedList) == 12, "original CExoArrayList extent");
extern PostedList posted_list_007b90fc;
extern std::uint32_t volatile trace_table_007b9108;
extern std::int32_t volatile trace_count_007b910c;
}
extern "C" {
// 004A21D0 CAurFont::~CAurFont (__thiscall, RET 0): font domain, still missing.
void __fastcall aurora_font_destroy_004a21d0(void* font, void*);
const char* __cdecl aurora_untrace_0044d250(const char* name);
void __cdecl aurora_strings_free_0044d1e0();
void __cdecl aurora_free_font_0044cc60();
void __cdecl aurora_delete_all_fonts_004a22e0();
}

namespace kotor_aurora_window {
// Font object at 007B90EC: first word is its table; slot 0 = scalar deleting destructor (RET 4).
struct FontObject;
struct FontTable { void* (__fastcall* destroy)(FontObject*, void*, U32 flags); };
struct FontObject { const FontTable* table; };
FontObject* font_007b90ec = nullptr;      // .bss
U32 font_loaded_007b90f0 = 0;             // .bss (borrowed by renderer_texture::Views per epoch notes)
U32 font_lists_007a47c0 = 0;              // .bss; glDeleteLists base
char trace_message_007b9118[0x100] = {};  // 007B9118..007B9217 (next referenced cell 007B9218)
kotor_reconstruction::ArrayMutations fonts_0083052c{};   // 0083052C .bss zero (constant-initialised; window_lists.h)
}

namespace {
U32* words(U32 table) { return reinterpret_cast<U32*>(static_cast<std::uintptr_t>(table)); }
void* as_ptr(U32 w) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(w)); }
}

// 0044D1E0 AurStringsFree.
extern "C" void __cdecl aurora_strings_free_0044d1e0() {
    U32 remaining = r4_lowers_cells::posted_list_007b90fc.count;                // 0044D1E1
    while (remaining != 0) {                                          // 0044D1E7 / 0044D243..0044D245
        const U32 entry = words(r4_lowers_cells::posted_list_007b90fc.data)[remaining - 1]; // 0044D1F2
        --remaining;                                                  // 0044D1F6
        if (entry == 0) continue;                                     // 0044D1F7..0044D1F9
        const I32 last = static_cast<I32>(r4_lowers_cells::posted_list_007b90fc.count) - 1; // 0044D1FB..0044D201
        I32 at = last;
        while (at >= 0 && words(r4_lowers_cells::posted_list_007b90fc.data)[at] != entry) --at; // 0044D204..0044D20E
        r4_lowers_cells::posted_list_007b90fc.count = static_cast<U32>(last);    // 0044D212
        for (; at < last && at < static_cast<I32>(r4_lowers_cells::posted_list_007b90fc.count); ++at) // 0044D210..0044D232
            words(r4_lowers_cells::posted_list_007b90fc.data)[at] = words(r4_lowers_cells::posted_list_007b90fc.data)[at + 1];
        live_free(as_ptr(entry));                                     // 0044D234..0044D235
    }
}

// 0044D250 untrace(name): delete every trace entry (all, or those whose prefix matches name).
extern "C" const char* __cdecl aurora_untrace_0044d250(const char* name) {
    using namespace r4_lowers_cells;
    I32 index = trace_count_007b910c;                                 // 0044D251 ebx
    I32 deleted = 0;                                                  // 0044D258 ebp
    while (index != 0) {                                              // 0044D25A / 0044D2F2..0044D2F4
        const U32 entry = words(trace_table_007b9108)[index - 1];    // 0044D270
        --index;                                                      // 0044D274
        if (name && *name) {                                          // 0044D275..0044D27C
            const std::size_t length = std::strlen(name);             // 0044D27E..0044D28A
            if (std::strncmp(static_cast<const char*>(as_ptr(entry)), name, length) != 0) continue; // 0044D28C..0044D29F (006FAFF0, tested only for zero)
        }
        if (entry) {                                                  // 0044D2A1..0044D2A3
            const I32 last = trace_count_007b910c - 1;                // 0044D2A5..0044D2AB
            I32 at = last;
            while (at >= 0 && words(trace_table_007b9108)[at] != entry) --at; // 0044D2AE..0044D2B8
            trace_count_007b910c = last;                              // 0044D2BC
            for (; at < last && at < trace_count_007b910c; ++at)      // 0044D2BA..0044D2E0
                words(trace_table_007b9108)[at] = words(trace_table_007b9108)[at + 1];
            live_free(as_ptr(entry));                                 // 0044D2E2..0044D2E3
        }
        ++deleted;                                                    // 0044D2F1 (also for a null entry)
    }
    std::sprintf(kotor_aurora_window::trace_message_007b9118, "deleted %d trace commands", deleted); // 0044D2FC..0044D307 (fmt 007414C0)
    return kotor_aurora_window::trace_message_007b9118;              // 0044D310
}

// 0044CC60 AurFreeFont.
extern "C" void __cdecl aurora_free_font_0044cc60() {
    using namespace kotor_aurora_window;
    if (font_loaded_007b90f0 == 0) return;                            // 0044CC60..0044CC67
    const U32 base = font_lists_007a47c0;                             // 0044CC69
    font_loaded_007b90f0 = 0;                                         // 0044CC74
    ::glDeleteLists(base, 0x80);                                      // 0044CC6E..0044CC7E [0073D33C]
    if (FontObject* font = font_007b90ec) (void)font->table->destroy(font, nullptr, 1); // 0044CC84..0044CC92
    font_007b90ec = nullptr;                                          // 0044CC94
}

// 004A22E0 AurDeleteAllFonts.
extern "C" void __cdecl aurora_delete_all_fonts_004a22e0() {
    auto& l = kotor_aurora_window::fonts_0083052c;
    for (I32 i = l.size - 1; i >= 0; --i) {                           // 004A22E1..004A22EA / 004A230C..004A230D
        void* font = as_ptr(words(l.data)[i]);                        // 004A22F0..004A22F5 (data reloaded)
        if (!font) continue;                                          // 004A22F8..004A22FA
        aurora_font_destroy_004a21d0(font, nullptr);                  // 004A22FC..004A22FE
        live_free(font);                                              // 004A2303..004A2304
    }
}

// 0073BE50: atexit callback registered by __xc 0078D0C8 -> 0073B110 (not registered here; see window_lists.h).
extern "C" void __cdecl kotor_atexit_0073be50() {
    if (const std::uint32_t data = kotor_aurora_window::fonts_0083052c.data)   // 0073BE50..0073BE77
        live_free(as_ptr(data));                                                   // 0073BE79..0073BE7A
}
