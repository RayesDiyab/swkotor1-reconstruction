// Lane-sound: 005DBD30 CExoStreamingSoundSourceInternal::InitializeSource.
// File I/O binds the original's statically linked CRT calls (fopen 006FAD9D,
// fread 006FA9D4, strncmp 006FAFF0, fseek 006FBB19, ftell 006FBA49,
// fclose 006FACF0) to the linked CRT, and the original FILE->_file read at
// [FILE+10h] + __get_osfhandle 006FD385 to _fileno + _get_osfhandle (UCRT FILE
// is opaque) -- D16-style binding, same semantics on reachable inputs.
#include "e2581310e3-sound_internal.h"
#include "315fa1bc4a-warning_runtime.h"   // StartupFormat005e5680
#include <cstdio>
#include <cstring>
#include <io.h>
#include <new>

using String = kotor_reconstruction::StartupString;
extern "C" {
char* __fastcall async_string_cstr_005e5670(String*, void*);
std::uint32_t __fastcall async_string_length_005e5790(String*, void*);
__declspec(dllimport) void* __stdcall AIL_open_stream(void*, const char*, std::int32_t);
extern volatile std::uint32_t kotor_cell_stream_mode_007a2374;      // lane-sound owner (image value 1)
}
namespace base_ini_reconstruction { String* __fastcall left_005e5f70(String*, void*, String*, std::int32_t); } // ini-string-transforms.obj
String* __fastcall GuiConcat005e5d10(String*, void*, String*, const String*);                               // ini-string-priority.obj

namespace {
using U = std::uint32_t;
unsigned char* at(void* p, unsigned off) { return static_cast<unsigned char*>(p) + off; }
U rd(void* p, unsigned off) { U v; std::memcpy(&v, at(p, off), 4); return v; }
void wr(void* p, unsigned off, U v) { std::memcpy(at(p, off), &v, 4); }
void* rp(void* p, unsigned off) { void* v; std::memcpy(&v, at(p, off), 4); return v; }
void wp(void* p, unsigned off, void* v) { std::memcpy(at(p, off), &v, 4); }
FILE* file(void* st) { return static_cast<FILE*>(rp(st, 0x38)); }
std::uint16_t format(void* st) { std::uint16_t v; std::memcpy(&v, at(st, 0x44), 2); return v; }
String* name(void* st) { return reinterpret_cast<String*>(at(st, 0x24)); }
// Hidden-return storage: the callee (005E5F70 Left / 005E5D10 operator+) constructs it.
struct Raw8 {
    alignas(4) unsigned char b[8]; bool live = false;
    String* raw() { return reinterpret_cast<String*>(b); }
    String* adopt(String* s) { live = true; return s; }
    ~Raw8() { if (live) std::launder(raw())->~StartupString(); }
};
std::int32_t os_handle(FILE* f) { return static_cast<std::int32_t>(_get_osfhandle(_fileno(f))); }
// Open the Miles stream on the 0074D318 format (OS handle, byte count), optional suffix.
// lane-m1-sound C1: 0074D318 is the .rdata string 5C 5C 5C 5C 25 64 2C 25 64 00 (FOUR backslashes, "%d,%d").
// Mss32.dll (sha 858d6066...) default file-open callback 2110A430 compares the first 4 chars with its own .rdata
// four-backslash string (21144150; compare 21109DE0, length 4) and only then parses "handle,size" (2110A290);
// any other name goes to CreateFileA (2110A3A0). M47..M54 emitted TWO backslashes (C literal with 4 escapes), so
// every stream open fell through to CreateFileA on a UNC-shaped name and failed: no menu music.
void open_by_handle(void* st, std::int32_t size, const char* suffix) {
    const std::int32_t handle = os_handle(file(st));                 // 005DBEE9-005DBEFC / 005DC0BC-005DC0D2 / 005DC1EC-005DC1FF
    String path;                                                     // 005B3190
    void* dig = nullptr;
    AIL_quick_handles(&dig, nullptr, nullptr);                        // 005DBF03-005DBF12
    kotor_reconstruction::StartupFormat005e5680(&path, "\\\\\\\\%d,%d", handle, size); // 005DBF18-005DBF24 ([0074D318]: 4 x 5Ch)
    if (suffix) {                                                    // 005DBF2C-005DBF7E
        String ext(suffix);                                          // 005DBF33-005DBF3C
        Raw8 joined;
        String* sum = joined.adopt(GuiConcat005e5d10(&path, nullptr, joined.raw(), &ext)); // 005DBF41-005DBF54
        path.Assign005e5c50(sum);                                    // 005DBF59-005DBF63
    }                                                                // ~joined 005DBF71, ~ext 005DBF7E
    wp(st, 0, AIL_open_stream(dig, async_string_cstr_005e5670(&path, nullptr), 0)); // 005DBF83-005DBF9E
}
}

namespace sound_stream_play29_required {
// 005DBD30 InitializeSource, RET 0.
void __fastcall open_005dbd30(void* st, void*) {
    void* sys = rp(st, 0x20);                                        // 005DBD5C
    const U ready = rd(sys, 8);                                      // 005DBD5F
    if (kotor_cell_stream_mode_007a2374 == 0) {                      // 005DBD4E-005DBD63 -> 005DC196
        if (!ready) return;                                          // 005DC196-005DC198
        wp(st, 0x38, nullptr);                                       // 005DC1A3 (no fclose on this path)
        if (format(st) != 8) {                                       // 005DC19E-005DC1A6 -> 005DC2C6
            void* dig = nullptr; AIL_quick_handles(&dig, nullptr, nullptr);
            wp(st, 0, AIL_open_stream(dig, async_string_cstr_005e5670(name(st), nullptr), 0)); // 005DC2C6-005DC2E8
            return;
        }
        FILE* f = std::fopen(async_string_cstr_005e5670(name(st), nullptr), "rb"); // 005DC1AC-005DC1C4
        wp(st, 0x38, f);
        if (!f) return;
        std::fseek(f, 0, SEEK_END); const std::int32_t size = std::ftell(f); std::fseek(f, 0, SEEK_SET); // 005DC1CD-005DC1E7
        open_by_handle(st, size, ".mp3");                            // 005DC1EC-005DC2A4 (format is 8 here)
        return;
    }
    if (!ready) return;                                              // 005DBD69-005DBD6B
    if (file(st)) std::fclose(file(st));                             // 005DBD71-005DBD7E
    const bool mp3 = format(st) == 8;                                // 005DBD81
    wp(st, 0x38, nullptr);                                           // 005DBD86
    if (mp3) {
        Raw8 combined_slot;
        String* combined_p;
        {
            String wav("wav");                                       // 005DBD8F-005DBD98
            Raw8 left;
            String* l = left.adopt(base_ini_reconstruction::left_005e5f70(name(st), nullptr, left.raw(),
                static_cast<std::int32_t>(async_string_length_005e5790(name(st), nullptr)) - 3)); // 005DBD9D-005DBDB6
            combined_p = combined_slot.adopt(GuiConcat005e5d10(l, nullptr, combined_slot.raw(), &wav)); // 005DBDBB-005DBDCC
        }                                                            // ~left 005DBDDA, ~wav 005DBDE8
        String& combined = *combined_p;
        std::int32_t size = 0; bool have = false;
        FILE* f = std::fopen(async_string_cstr_005e5670(&combined, nullptr), "rb"); // 005DBDED-005DBE06
        wp(st, 0x38, f);
        if (f) {
            char riff[4]; (void)std::fread(riff, 1, 4, f);           // 005DBE0F-005DBE19
            if (std::strncmp(riff, "RIFF", 4) != 0) { std::fclose(f); wp(st, 0x38, nullptr); } // 005DBE1E-005DBE45
            else {
                std::uint32_t chunk = 0; (void)std::fread(&chunk, 4, 1, f); // 005DBE47-005DBE54
                const std::int32_t start = static_cast<std::int32_t>(chunk + 8); // 005DBE5C-005DBE65
                std::fseek(f, 0, SEEK_END); const std::int32_t end = std::ftell(f); // 005DBE59-005DBE76
                if (start != end) { std::fseek(f, start, SEEK_SET); size = end - start; have = true; } // 005DBE7B-005DBE90
                else { std::fclose(f); wp(st, 0x38, nullptr); }      // 005DBE92-005DBEA5
            }
        }
        if (!have) {                                                 // 005DBEA7
            f = std::fopen(async_string_cstr_005e5670(name(st), nullptr), "rb"); // 005DBEA7-005DBEBE
            wp(st, 0x38, f);
            if (!f) return;                                          // 005DBEC1 -> 005DBFAA (~combined)
            std::fseek(f, 0, SEEK_END); size = std::ftell(f); std::fseek(f, 0, SEEK_SET); // 005DBEC7-005DBEE1
        }
        open_by_handle(st, size, format(st) == 8 ? ".mp3" : nullptr); // 005DBEE9-005DBFA5 (re-tests [44]==8)
        return;
    }
    FILE* f = std::fopen(async_string_cstr_005e5670(name(st), nullptr), "rb"); // 005DBFCE-005DBFE6
    wp(st, 0x38, f);
    if (!f) return;
    char riff[4]; (void)std::fread(riff, 1, 4, f);                   // 005DBFEF-005DBFF9
    std::int32_t size, start;
    if (std::strncmp(riff, "RIFF", 4) == 0) {                        // 005DC00A-005DC014 -> 005DC09A
        std::fseek(f, 0, SEEK_END); size = std::ftell(f); start = 0; // 005DC09A-005DC0B0
    } else {
        std::fseek(f, 0x1d6, SEEK_SET);                              // 005DC01A-005DC024
        (void)std::fread(riff, 1, 4, f);                             // 005DC029-005DC036
        if (std::strncmp(riff, "RIFF", 4) != 0) { std::fclose(f); wp(st, 0x38, nullptr); return; } // 005DC03B-005DC074
        std::fseek(f, 0, SEEK_END); size = std::ftell(f) - 0x1d6; start = 0x1d6; // 005DC075-005DC093
    }
    std::fseek(f, start, SEEK_SET);                                  // 005DC0B3-005DC0B7
    open_by_handle(st, size, format(st) == 4 ? ".wav" : nullptr);     // 005DC0BC-005DC174
}
}
