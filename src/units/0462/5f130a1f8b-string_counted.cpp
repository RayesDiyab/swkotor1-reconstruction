// lane-resman: CExoString::CExoString(char const*, int) 005E5B70 -- DECISIONS D18 single
// provider. Consumers: lane-gui-load gff_readers.cpp (00411EC0 -> 005E5B70),
// lane-packed-file packed_file.cpp (0040E509). Body verified vs 005E5B70..005E5BB7.
#include "9f8b86cc9d-startup_string.h"
#include <cstdint>
#include <cstring>

using Word = std::uint32_t;
using String = kotor_reconstruction::StartupString;

extern "C" {
void* __cdecl c2da_directory_new_006fa7e6(std::uint32_t) noexcept(false); // 006FA7E6 operator new (M40)
String* __fastcall resman_string_counted_005e5b70(String* self, void* unused_edx, const char* text, int length);
}
namespace {
template<class T> T get(const void* p, Word off) { T v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof(v)); return v; }
template<class T> void put(void* p, Word off, T v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, sizeof(v)); }
}

// 005E5B70 CExoString::CExoString(char const*, int) RET8.
extern "C" __declspec(noinline) String* __fastcall resman_string_counted_005e5b70(String* self, void*, const char* text, int length) {
    if (length > 0) {                                          // 005E5B76-005E5B7A jle
        const Word capacity = static_cast<Word>(length) + 1u;
        put<Word>(self, 4, capacity);                          // 005E5B80
        char* data = static_cast<char*>(c2da_directory_new_006fa7e6(capacity)); // 005E5B83
        put<char*>(self, 0, data);                             // 005E5B8F
        std::strncpy(data, text, static_cast<std::size_t>(length)); // 005E5B91
        get<char*>(self, 0)[length] = '\0';                    // 005E5B96-005E5B9B (reloaded data)
        return self;
    }
    put<char*>(self, 0, nullptr);
    put<Word>(self, 4, 0);
    return self;
}
