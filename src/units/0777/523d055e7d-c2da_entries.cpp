// lane-m47-core: C2DA::GetINTEntry(CExoString row, CExoString column, int*) 00414110 and
// C2DA::GetFLOATEntry(CExoString row, CExoString column, float*) 00413FA0 (RET 0Ch), consumed by
// SetEventDescriptions 005EEB10 (r4-lowers adopted/event_descriptions.h; first frame per D29).
// Evidence: lane dis/00414110.asm, dis/00413fa0.asm; literals 0073E2EC "%i", 0073E2E8 "%x", 0073E2E4 "%f",
// 0073D71C "" (tools/rd.py). Table fields: +1C default CExoString, +28 column count, +38 row CExoString arrays,
// +3C binary flag, +40 binary data, +44 16-bit cell offsets (as read by these bodies).
// Callees (M46 providers): GetRowIndex 004138B0 (lane-config), GetColumnIndex 00413100 (startup-misc),
// CExoString ==/GetLength/CStr/AsINT (005E5290/005E5790/005E5670/005E5630), sscanf 006FAE08 (font-txi),
// AsFLOAT 005E5650 (lane-sound StringAsFloat005e5650; listed in the M46 prediction, defined by lane-sound).
// D30-FINAL: AsFLOAT's x87 ST0 result is stored with fstp dword (00414057 / 00414070): the float return value is
// stored as float here.
#include "f9f7ab0530-startup_string.h"
#include <cstdint>
#include <cstring>

using kotor_reconstruction::StartupString;
namespace options_video { struct String; }
namespace client_movies { std::int32_t __fastcall C2daGetRowIndex_004138b0(void* table, void* unused_edx, options_video::String* label); }
#include "a0f716d4dd-menu_targets.h"   // kotor_menu20::String::as_int_005e5630 (service-menu20-4 owner)
namespace kotor_sound_path { float __fastcall StringAsFloat005e5650(StartupString*, void*); }
extern "C" {
std::int32_t __fastcall c2da_current_column_index_00413100(void* table, void*, StartupString* column);
char* __fastcall async_string_cstr_005e5670(StartupString*, void*);
int __cdecl kotor_sscanf_006fae08(const char*, const char*, ...);
const char* __cdecl c2da_empty_0073d71c();
}

namespace {
using Word = std::uint32_t;
template <class T> T load(const void* p, unsigned off) { T v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof(v)); return v; }
std::int32_t row_index(void* table, StartupString* row) {
    return client_movies::C2daGetRowIndex_004138b0(table, nullptr, reinterpret_cast<options_video::String*>(row));
}
// Text cell (row r, column c): [[+38h] + r*4] + c*8 (00414186..98).
StartupString* cell(void* table, std::int32_t r, std::int32_t c) {
    return reinterpret_cast<StartupString*>(load<unsigned char*>(load<unsigned char*>(table, 0x38), static_cast<unsigned>(r) * 4u)
                                            + static_cast<unsigned>(c) * 8u);
}
// Binary cell text: [+40h] + u16 [[+44h] + (cols*r + c)*2] (00414146..51).
std::uint32_t binary_offset(void* table, std::int32_t r, std::int32_t c) {
    const Word index = load<Word>(table, 0x28) * static_cast<Word>(r) + static_cast<Word>(c);
    return load<std::uint16_t>(load<unsigned char*>(table, 0x44), index * 2u);
}
StartupString* default_string(void* table) { return reinterpret_cast<StartupString*>(static_cast<unsigned char*>(table) + 0x1c); }
std::int32_t as_int(StartupString* s) { return reinterpret_cast<kotor_menu20::String*>(s)->as_int_005e5630(); }
}

extern "C" {
// 00414110 C2DA::GetINTEntry(row, column, out) RET 0Ch.
__declspec(noinline) int __fastcall kotor_fastcall_00414110(void* table, void*, StartupString* row, StartupString* column,
                                                            std::int32_t* out) {
    const std::int32_t r = row_index(table, row);                                // 00414110..1A
    const std::int32_t c = c2da_current_column_index_00413100(table, nullptr, column); // 0041411F..28
    if (r == -1 || c == -1) {                                                    // 0041412D..39
        *out = as_int(default_string(table));                                    // 00414248..56
        return 0;
    }
    if (load<Word>(table, 0x3c) != 0) {                                          // 0041413F..44
        const std::uint32_t offset = binary_offset(table, r, c);                 // 00414146..51
        *out = 0;                                                                // 00414155..59
        (void)kotor_sscanf_006fae08(load<char*>(table, 0x40) + offset, "%i", out); // 0041415F..6B ("%i" 0073E2EC)
        return load<char*>(table, 0x40)[offset] != 0 ? 1 : 0;                    // 00414170..7F (setne)
    }
    if (cell(table, r, c)->BorrowedEquals005e5290(c2da_empty_0073d71c())) {      // 00414186..A1
        *out = 0;                                                                // 004141A3..A9
        return 0;
    }
    if (cell(table, r, c)->BorrowedLength005e5790() > 2 &&                       // 004141B5..C5 (signed JLE)
        async_string_cstr_005e5670(cell(table, r, c), nullptr)[0] == '0' &&      // 004141C7..D7
        (async_string_cstr_005e5670(cell(table, r, c), nullptr)[1] == 'x' ||     // 004141D9..EA
         async_string_cstr_005e5670(cell(table, r, c), nullptr)[1] == 'X')) {    // 004141EC..FD
        (void)kotor_sscanf_006fae08(async_string_cstr_005e5670(cell(table, r, c), nullptr), "%x", out); // 004141FF..17 ("%x" 0073E2E8)
        return 1;
    }
    *out = as_int(cell(table, r, c));                                            // 0041422A..3D
    return 1;
}

// 00413FA0 C2DA::GetFLOATEntry(row, column, out) RET 0Ch.
__declspec(noinline) int __fastcall kotor_fastcall_00413fa0(void* table, void*, StartupString* row, StartupString* column,
                                                            float* out) {
    const std::int32_t r = row_index(table, row);                                // 00413FA0..AA
    const std::int32_t c = c2da_current_column_index_00413100(table, nullptr, column); // 00413FAF..B8
    if (r == -1 || c == -1) {                                                    // 00413FBD..C9
        *out = kotor_sound_path::StringAsFloat005e5650(default_string(table), nullptr); // 00414063..70
        return 0;
    }
    if (load<Word>(table, 0x3c) != 0) {                                          // 00413FCF..D4
        const std::uint32_t offset = binary_offset(table, r, c);                 // 00413FD6..E1
        std::memset(out, 0, 4);                                                  // 00413FE5..E9 (dword 0)
        (void)kotor_sscanf_006fae08(load<char*>(table, 0x40) + offset, "%f", out); // 00413FEF..FB ("%f" 0073E2E4)
        return load<char*>(table, 0x40)[offset] != 0 ? 1 : 0;                    // 00414000..0F
    }
    if (cell(table, r, c)->BorrowedEquals005e5290(c2da_empty_0073d71c())) {      // 00414016..31
        std::memset(out, 0, 4);                                                  // 00414033..39 (dword 0)
        return 0;
    }
    *out = kotor_sound_path::StringAsFloat005e5650(cell(table, r, c), nullptr);  // 00414045..57
    return 1;
}
}
