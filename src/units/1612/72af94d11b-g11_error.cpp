// G11-quicksave lane: D-owned original CRT helpers.
//
//   006FE5C6  cdecl MoveFileA wrapper (2 stack args, caller cleans 8)
//   006FFE8A  cdecl OS-error -> errno mapper (1 stack arg, caller cleans 4)
//
// Both are exact original behavior; no UCRT _rename/MoveFileExA/modern errtable
// substitution and no numeric original address is read or aliased.
//
// 006FE5C6 contract:
//   MoveFileA succeeds                    -> 0, no GetLastError, no mapper
//   MoveFileA fails, GetLastError()==0    -> 0, no mapper
//   MoveFileA fails, GetLastError()!=0    -> 006FFE8A(error), then -1
//
// 006FFE8A contract:
//   ptd = 006FEEDF(); ptd->doserrno = os_error;
//   first match over the original 45 semantic {os_error, errno} pairs;
//   fallbacks 0x13..0x24 -> 0x0D, 0xBC..0xCA -> 8, otherwise 0x16;
//   return the PTD from the last 006FEEDF (callers ignore it).
//   The original duplicate os=0x0006 entry (index 30) is unreachable because
//   the first-match scan finds index 5 (os=6 -> errno 9) first.
#include "e848c4c6cb-g11_quicksave.h"

namespace {

struct OsErrorPair {
    unsigned os_error;
    int errno_value;
};

// Original 007A2C40: 0x2D (45) entries of {DWORD os_error, DWORD errno}.
// Order is the original table order; index 30 repeats os=0x0006 with errno
// 0x16 and is intentionally preserved but unreachable.
const OsErrorPair kOsErrorPairs[45] = {
    {0x001u, 0x16},  // 0
    {0x002u, 0x02},  // 1
    {0x003u, 0x02},  // 2
    {0x004u, 0x18},  // 3
    {0x005u, 0x0D},  // 4
    {0x006u, 0x09},  // 5  first os=6
    {0x007u, 0x0C},  // 6
    {0x008u, 0x0C},  // 7
    {0x009u, 0x0C},  // 8
    {0x00Au, 0x07},  // 9
    {0x00Bu, 0x08},  // 10
    {0x00Cu, 0x16},  // 11
    {0x00Du, 0x16},  // 12
    {0x00Fu, 0x02},  // 13
    {0x010u, 0x0D},  // 14
    {0x011u, 0x12},  // 15
    {0x012u, 0x02},  // 16
    {0x021u, 0x0D},  // 17
    {0x035u, 0x02},  // 18
    {0x041u, 0x0D},  // 19
    {0x043u, 0x02},  // 20
    {0x050u, 0x11},  // 21
    {0x052u, 0x0D},  // 22
    {0x053u, 0x0D},  // 23
    {0x057u, 0x16},  // 24
    {0x059u, 0x0B},  // 25
    {0x06Cu, 0x0D},  // 26
    {0x06Du, 0x20},  // 27
    {0x070u, 0x1C},  // 28
    {0x072u, 0x09},  // 29
    {0x006u, 0x16},  // 30 duplicate os=6, first match at 5 wins
    {0x080u, 0x0A},  // 31
    {0x081u, 0x0A},  // 32
    {0x082u, 0x09},  // 33
    {0x083u, 0x16},  // 34
    {0x084u, 0x0D},  // 35
    {0x091u, 0x29},  // 36
    {0x09Eu, 0x0D},  // 37
    {0x0A1u, 0x02},  // 38
    {0x0A4u, 0x0B},  // 39
    {0x0A7u, 0x0D},  // 40
    {0x0B7u, 0x11},  // 41
    {0x0CEu, 0x02},  // 42
    {0x0D7u, 0x0B},  // 43
    {0x718u, 0x0C},  // 44
};
static_assert(sizeof(kOsErrorPairs) / sizeof(kOsErrorPairs[0]) == 45,
              "original 007A2C40 table is 0x2D entries");

}  // namespace

extern "C" g11::Ptd* __cdecl g11_map_os_error(unsigned os_error) {
    // Original first 006FEEDF call: doserrno is written before the scan.
    g11::Ptd* ptd = g11_getptd();
    ptd->doserrno = os_error;

    // Original scan: for (i = 0; i < 0x2D; ++i) on the first match write +8 and
    // return. Every branch below calls 006FEEDF again, exactly like the
    // original's separate call sites; the same-thread PTD identity is unchanged.
    for (unsigned i = 0; i < 45u; ++i) {
        if (kOsErrorPairs[i].os_error == os_error) {
            ptd = g11_getptd();
            ptd->errno_value = kOsErrorPairs[i].errno_value;
            return ptd;
        }
    }

    if (os_error >= 0x13u && os_error <= 0x24u) {  // unsigned range
        ptd = g11_getptd();
        ptd->errno_value = 0x0D;
        return ptd;
    }
    if (os_error >= 0xBCu && os_error <= 0xCAu) {  // unsigned range
        ptd = g11_getptd();
        ptd->errno_value = 8;
        return ptd;
    }

    ptd = g11_getptd();
    ptd->errno_value = 0x16;
    return ptd;
}

extern "C" int __cdecl g11_move_file_a(const char* old_path,
                                       const char* new_path) {
    // Exact original 006FE5C6: MoveFileA success -> 0 without GetLastError.
    if (g11::platform.move_file_a(old_path, new_path) != 0) {
        return 0;
    }

    // Failure: GetLastError()==0 -> 0 without the mapper (the 0/0 quirk).
    const unsigned long last_error = g11::platform.get_last_error();
    if (last_error == 0) {
        return 0;
    }

    // Nonzero: 006FFE8A sets doserrno/errno as a side effect; the return value
    // is forced to -1 exactly as the original `or eax,-1` does.
    (void)g11_map_os_error(static_cast<unsigned>(last_error));
    return -1;
}
