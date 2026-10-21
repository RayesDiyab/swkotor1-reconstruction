// claude_snapshot_crt.cpp (lane worker snapshot): the original statically linked VC CRT sscanf of swkotor.exe,
// transcribed from the original (read-only), replacing the UCRT vsscanf provider of _kotor_sscanf_006fae08 (0360).
//   006FAE08 sscanf(char const* input, char const* format, ...)        cdecl   kotor_sscanf_006fae08
//   00700858 _input(FILE* stream, uchar const* format, va_list args)     cdecl   kotor_cdecl_00700858
//   00700842 _inc(FILE*) (edx)                                           -       inc_00700842 (file-local)
//   007089F8 _ungetc(int ch, FILE*) (string-stream state)               cdecl   kotor_cdecl_007089f8
//   006FF058 _filbuf (only the _IOSTRG exit: returns EOF)                -       (inlined in inc_00700842)
//   0070142C __fassign(int is_double, void* out, char* text)             cdecl   kotor_cdecl_0070142c ([007A2DB8])
//   00708F44 _atodbl(double*, char*)                                     cdecl   kotor_cdecl_00708f44
//   00708F87 _atoflt(float*, char*)                                      cdecl   kotor_cdecl_00708f87
//   00708F2E __ld12tof(ld12*, float*) = _ld12cvt(.., FloatFormat 007A34E8) cdecl  kotor_cdecl_00708f2e
//   006FCA16 isxdigit                                                    cdecl   kotor_cdecl_006fca16
// Existing reviewed ports bound (claude_vm_actions_strings, REVIEW-ENDAR-ACTIONS "atof chain"):
//   006FCA55 isspace _kotor_cdecl_006fca55, 006FC9DC isdigit _kotor_cdecl_006fc9dc, 0070A54E __strgtold12
//   _kotor_cdecl_0070a54e, 00708DC0 _ld12cvt _kotor_cdecl_00708dc0, 00708F18 __ld12tod _kotor_cdecl_00708f18;
//   00701300 strlen _reconstructed_strlen_00701300 (0010-strlen).
// FILE layout used by the string stream (sscanf's local FILE at [ebp-20h]): +0 _ptr, +4 _cnt, +8 _base, +C _flag
// (49h = _IOREAD|_IOSTRG|_IOMYBUF); _filbuf returns EOF for _IOSTRG (006FF068..006FF06A), so _cnt keeps falling
// below zero on every read past the end, as in the original.
// Locale: isspace/isdigit/isxdigit, the decimal point [007A32A4], the lead-byte test (_pctype[c] & 8000h,
// [007A3250]) and mbtowc with MB_CUR_MAX [007A32A0] read the rebuilt process's CRT locale, as the atof chain does;
// that is the "C" locale unless the game's own setlocale call (005F932D, Polish TLK) ran.
// The scanset table: the original _alloca(32)s it on the first %[ (with an SEH fallback to malloc + free at exit,
// malloc_flag [ebp-24h], taken only on a stack-overflow exception); here it is a 32-byte frame array (the fallback is
// not reproducible without that exception).
// Quirks kept: a %[ scanset without ']' scans past the format's NUL; a range whose byte count wraps to 0 (hi-lo+1 =
// 256) loops 2^32 times as the original; %I64n stores num64 (not the char count); comchr 'F' (never produced by the
// lower-cased comchr) clears started; the wide-char path leaves temp[1] as the stack had it in the original (here 0).
#include <cctype>
#include <clocale>
#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <cwchar>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
using U64 = std::uint64_t;
using I64 = std::int64_t;
constexpr int k_eof = -1;
struct FpFormat { I max_exp, min_exp, precision, exp_width, format_width, bias; };
const FpFormat k_float_format_007a34e8 = {128, -127, 24, 8, 32, 127};   // .data 007A34E8 FloatFormat
struct StrFile {                                                         // FILE (first 16 bytes)
    const unsigned char* ptr;                                            // +0
    I cnt;                                                               // +4
    const unsigned char* base;                                           // +8
    U flag;                                                              // +C
};
constexpr U k_ioread = 0x1, k_ioeof = 0x10, k_iostrg = 0x40;
}

extern "C" {
int __cdecl kotor_cdecl_006fca55(int c);                                       // isspace [claude_vm_actions_strings]
int __cdecl kotor_cdecl_006fc9dc(int c);                                       // isdigit [claude_vm_actions_strings]
U __cdecl kotor_cdecl_0070a54e(void* out, const char** end, const char* text, U mult12, I scale, I decpt, I implicit_e);
int __cdecl kotor_cdecl_00708dc0(const void* value, void* out, const void* format);   // _ld12cvt
int __cdecl kotor_cdecl_00708f18(const void* value, void* out);                // __ld12tod
std::size_t __cdecl reconstructed_strlen_00701300(const char*);                // strlen [0010]

int __cdecl kotor_cdecl_006fca16(int c);
int __cdecl kotor_cdecl_007089f8(int ch, StrFile* stream);
int __cdecl kotor_cdecl_00708f2e(const void* value, void* out);
int __cdecl kotor_cdecl_00708f44(void* out, const char* text);
int __cdecl kotor_cdecl_00708f87(void* out, const char* text);
void __cdecl kotor_cdecl_0070142c(int is_double, void* out, const char* text);
int __cdecl kotor_cdecl_00700858(StrFile* stream, const unsigned char* format, va_list args);
int __cdecl kotor_sscanf_006fae08(const char* input, const char* format, ...);
}

// ===================================================================================== small CRT leaves
// 006FCA16 isxdigit: current CRT locale classification (_HEX = 80h)
extern "C" int __cdecl kotor_cdecl_006fca16(int c) { return std::isxdigit(c) ? 0x80 : 0; }

namespace {
// 00700842 _inc(stream in EDX): --_cnt >= 0 ? (uchar)*_ptr++ : _filbuf(stream) (EOF for _IOSTRG)
int inc_00700842(StrFile* s) {
    if (--s->cnt >= 0) return *s->ptr++;                               // 00700842..0070084F
    return k_eof;                                                      // 00700850..00700857 -> 006FF068 _IOSTRG
}
// isleadbyte: _pctype[c] & _LEADBYTE ([007A3250], test byte [+c*2+1], 80h)
bool lead_byte(int c) { return ::isleadbyte(c) != 0; }
}

// 007089F8 _ungetc(ch, stream)
extern "C" int __cdecl kotor_cdecl_007089f8(int ch, StrFile* s) {
    if (ch == k_eof) return k_eof;                                     // 007089FD..00708A01
    const U f = s->flag;
    if (!(f & k_ioread) && (!(f & 0x80) || (f & 0x2))) return k_eof;   // 00708A07..00708A14
    // 00708A16..00708A22: _base == NULL -> _getbuf (a string stream always has its base)
    if (s->ptr == s->base) {                                           // 00708A23..00708A28
        if (s->cnt != 0) return k_eof;                                 // 00708A2A..00708A2E
        ++s->ptr;                                                      // 00708A30..00708A31
    }
    --s->ptr;                                                          // 00708A33
    if (s->flag & k_iostrg) {                                          // 00708A35..00708A3B
        if (*s->ptr != static_cast<unsigned char>(ch)) { ++s->ptr; return k_eof; }   // 00708A3D..00708A44
    } else {
        *const_cast<unsigned char*>(s->ptr) = static_cast<unsigned char>(ch);       // 00708A4A (non-string FILE)
    }
    ++s->cnt;                                                          // 00708A4F
    s->flag = (s->flag & ~k_ioeof) | k_ioread;                         // 00708A4C..00708A58
    return ch & 0xff;                                                  // 00708A5B..00708A5D
}

// 00708F2E __ld12tof(pld12, float*)
extern "C" int __cdecl kotor_cdecl_00708f2e(const void* value, void* out) {
    return kotor_cdecl_00708dc0(value, out, &k_float_format_007a34e8);   // 00708F2E..00708F40
}
// 00708F44 _atodbl(double* out, char* text)
extern "C" int __cdecl kotor_cdecl_00708f44(void* out, const char* text) {
    unsigned char ld12[12];                                            // [ebp-14h]
    const char* end = nullptr;                                         // [ebp-8]
    (void)kotor_cdecl_0070a54e(ld12, &end, text, 0, 0, 0, 0);          // 00708F55..00708F66
    return kotor_cdecl_00708f18(ld12, out);                            // 00708F6B..00708F72
}
// 00708F87 _atoflt(float* out, char* text)
extern "C" int __cdecl kotor_cdecl_00708f87(void* out, const char* text) {
    unsigned char ld12[12];                                            // [ebp-14h]
    const char* end = nullptr;                                         // [ebp-8]
    (void)kotor_cdecl_0070a54e(ld12, &end, text, 0, 0, 0, 0);          // 00708F98..00708FA9
    return kotor_cdecl_00708f2e(ld12, out);                            // 00708FAE..00708FB5
}
// 0070142C __fassign(flag, argptr, number): flag ? *(double*)argptr = _atodbl : *(float*)argptr = _atoflt
extern "C" void __cdecl kotor_cdecl_0070142c(int is_double, void* out, const char* text) {
    unsigned char* const o = static_cast<unsigned char*>(out);
    if (is_double != 0) {                                              // 00701431..00701438
        unsigned char d[8];                                            // [ebp-8]
        (void)kotor_cdecl_00708f44(d, text);                           // 0070143A..0070143E
        for (int k = 0; k < 8; ++k) o[k] = d[k];                       // 00701443..00701450 two dwords
        return;
    }
    unsigned char f[4];                                                // [ebp+8] (the flag slot)
    (void)kotor_cdecl_00708f87(f, text);                               // 00701455..00701459
    for (int k = 0; k < 4; ++k) o[k] = f[k];                           // 0070145E..00701466
}

// ===================================================================================== 00700858 _input
extern "C" int __cdecl kotor_cdecl_00700858(StrFile* stream, const unsigned char* format, va_list arglist) {
    unsigned char table_storage[32];                                   // _alloca(32) [ebp-20h] (see header)
    unsigned char* table = nullptr;                                    // [ebp-20h]
    int ch = 0;                                                        // [ebp-28h]
    signed char match = 0;                                             // [ebp-29h]
    I charcount = 0;                                                   // [ebp-30h]
    I count = 0;                                                       // [ebp-34h]
    U number = 0;                                                      // [ebp-38h]
    U64 num64 = 0;                                                     // [ebp-5Ch]/[ebp-58h]
    void* pointer = nullptr;                                           // [ebp-64h]
    va_list arglistsave = arglist;                                     // [ebp-60h]
    char floatstring[0x160];                                           // [ebp-1C8h] .. [ebp-68h]
    auto INC = [&]() { ++charcount; return inc_00700842(stream); };
    auto UN_INC = [&](int c) { --charcount; if (c != k_eof) (void)kotor_cdecl_007089f8(c, stream); };

    for (;;) {
        if (*format == 0) goto error_return;                           // 00700886..0070088D
        if (kotor_cdecl_006fca55(*format)) {                           // 00700893..0070089F
            --charcount;                                               // 007008A1 UN_INC(EAT_WHITE())
            int c;
            do { c = INC(); } while (kotor_cdecl_006fca55(c));         // 007008A4..007008BA
            if (c != k_eof) (void)kotor_cdecl_007089f8(c, stream);     // 007008BC..007008CB
            do { ++format; } while (kotor_cdecl_006fca55(*format));    // 007008CC..007008DE
            continue;                                                  // 007008E0 -> 00700886
        }
        if (*format == '%') {                                          // 007008E2..007008E8
            I width = 0;                                               // edi -> [ebp-48h]
            I started = 0;                                             // [ebp-40h]
            I widthset = 0;                                            // [ebp-44h]
            unsigned char prevchar = 0;                                // [ebp-39h]
            signed char reject = 0, negative = 0, suppress = 0, done_flag = 0;   // [ebp-49h..-4Ch]
            signed char fl_wchar_arg = 0, widechar = 0, longone = 1;  // [ebp-4Dh], [ebp-4Eh], [ebp-4Fh]
            I integer64 = 0;                                           // [ebp-54h]
            number = 0;                                                // 007008F0
            const unsigned char* f = format;                           // esi
            while (!done_flag) {                                       // 0070091F..007009DE
                ++f;
                const int comchr = *f;
                if (kotor_cdecl_006fc9dc(comchr)) {                    // 00700923..0070092F
                    ++widthset;
                    width = static_cast<I>(static_cast<U>(width) * 10u + static_cast<U>(comchr) - 0x30u);
                    continue;
                }
                switch (comchr) {
                case 'N': case 'F': break;                             // 00700945 / 00700950
                case '*': ++suppress; break;                           // 007009B3
                case 'L': ++longone; break;                            // 00700963
                case 'I':                                              // 00700968
                    if (f[1] == '6' && f[2] == '4') { f += 2; ++integer64; num64 = 0; }
                    else if (f[1] == '3' && f[2] == '2') f += 2;
                    else if (f[1] == 'd' || f[1] == 'i' || f[1] == 'o' || f[1] == 'x' || f[1] == 'X') {}
                    else ++done_flag;
                    break;
                case 'h': --longone; --widechar; break;                // 007009D4..007009D7
                case 'l': ++longone; ++widechar; break;                // 007009CC..007009CF
                case 'w': ++widechar; break;                           // 007009CF
                default: ++done_flag; break;                           // 007009C7
                }
            }
            format = f;                                                // 007009E4..007009E7
            if (!suppress) {                                           // 007009EA..007009FF
                arglistsave = arglist;
                pointer = va_arg(arglist, void*);
            }
            done_flag = 0;                                             // 00700A07
            if (widechar == 0) {                                       // 00700A0B..00700A21
                if (*format == 'S' || *format == 'C') widechar = 1; else widechar = -1;
            }
            int comchr = *format | 0x20;                               // 00700A25..00700A2B [ebp-68h]
            if (comchr != 'n') {                                       // 00700A2E
                if (comchr != 'c' && comchr != '{') {                  // 00700A33..00700A3F EAT_WHITE
                    do { ch = INC(); } while (kotor_cdecl_006fca55(ch));   // 00700A41..00700A59
                } else {
                    ch = INC();                                        // 00700ABC..00700AC9
                }
            }
            if (widthset != 0 && width == 0) {                         // 00700A5F..00700A6A -> 00700CC5
                if (ch == k_eof) goto error_return;
                (void)kotor_cdecl_007089f8(ch, stream);                // 00701297..0070129D
                goto error_return;
            }
            switch (comchr) {
            case 'c':                                                  // 00700C73
                if (widthset == 0) { widthset = 1; ++width; }          // 00700C73..00700C7E
                [[fallthrough]];
            case 's':                                                  // 00700C81
                if (widechar > 0) fl_wchar_arg = 1;                    // 00700C81..00700C8B
                goto scanit;
            case '{': {                                                // 00700CE6 '[' | 20h
                if (widechar > 0) fl_wchar_arg = 1;                    // 00700CE6..00700CEC
                const unsigned char* scanptr = ++format;               // 00700CF0..00700CF7 [ebp-1CCh]
                if (*scanptr == '^') { ++scanptr; reject = -1; }       // 00700CFD..00700D09
                if (table == nullptr) table = table_storage;           // 00700D0D..00700D27
                for (int k = 0; k < 32; ++k) table[k] = 0;             // 00700D65..00700D6F memset(table, 0, 32)
                unsigned char dl = prevchar;                           // prevchar register
                if (comchr == '{' && *scanptr == ']') {                // 00700D72..00700D7B
                    dl = ']'; ++scanptr; table[0xb] = 0x20;            // 00700D7D..00700D80 (']' bit)
                } else {
                    dl = prevchar;                                     // 00700DEC
                }
                unsigned char al = *scanptr;                           // 00700DEF
                while (al != ']') {                                    // 00700DF1..00700DF3
                    ++scanptr;                                         // 00700D86
                    if (al == '-' && dl != 0 && *scanptr != ']') {     // 00700D87..00700D94 range
                        unsigned char cl = *scanptr++;                 // 00700D8F..00700D96
                        unsigned char hi, lo;
                        if (dl < cl) { hi = cl; lo = dl; } else { hi = dl; lo = cl; }   // 00700D97..00700DA1
                        if (lo <= hi) {                                // 00700DA3..00700DA5
                            U c = lo;
                            U n = static_cast<unsigned char>(static_cast<unsigned char>(hi - lo) + 1);   // 00700DAA..00700DB1
                            do {                                       // 00700DB7..00700DD0
                                table[c >> 3] |= static_cast<unsigned char>(1u << (c & 7));
                                ++c;
                            } while (--n != 0);
                        }
                        dl = 0;                                        // 00700DD2
                    } else {
                        prevchar = al;                                 // 00700DD6
                        table[al >> 3] |= static_cast<unsigned char>(1u << (al & 7));   // 00700DD9..00700DEA
                        dl = prevchar;                                 // 00700DEC
                    }
                    al = *scanptr;                                     // 00700DEF
                }
                if (al == 0) goto error_return;                        // 00700DF5..00700DF7 (never: al is ']')
                if (comchr == '{') format = scanptr;                   // 00700E00..00700E06
                goto scanit;
            }
            case 'i':                                                  // 00700ACE
                comchr = 'd';
                [[fallthrough]];
            case 'x':                                                  // 00700AD1
                if (ch == '-') { negative = 1; goto x_incwidth; }      // 00700AD4..00700AE1
                if (ch == '+') {                                       // 00700F00..00700F03
                x_incwidth:
                    if (--width == 0 && widthset != 0) done_flag = 1;  // 00700F05..00700F12
                    else ch = INC();                                   // 00700F14..00700F20
                }
                if (ch == '0') {                                       // 00700F23..00700F26
                    ch = INC();                                        // 00700F2C..00700F38
                    if (static_cast<char>(ch) == 'x' || static_cast<char>(ch) == 'X') {   // 00700F3B..00700F43
                        ch = INC();                                    // 00700F80..00700F8C
                        if (widthset != 0) {                           // 00700F8F..00700F9F
                            width -= 2;
                            if (width < 1) ++done_flag;
                        }
                        comchr = 'x';                                  // 00700FA2
                    } else {
                        started = 1;                                   // 00700F45
                        if (comchr != 'x') {                           // 00700F4C..00700F5F
                            if (widthset != 0 && --width == 0) ++done_flag;
                            comchr = 'o';
                        } else {
                            UN_INC(ch);                                // 00700F67..00700F76
                            ch = '0';                                  // 00700F78..00701035
                        }
                    }
                }
                goto getnum;
            case 'p':                                                  // 00701003
                longone = 1;
                [[fallthrough]];
            case 'o': case 'u': case 'd':                              // 00701007
                if (ch == '-') { negative = 1; goto d_incwidth; }      // 0070100A..00701013
                if (ch == '+') {                                       // 00701015..00701018
                d_incwidth:
                    if (--width == 0 && widthset != 0) done_flag = 1;  // 0070101A..00701027
                    else ch = INC();                                   // 00701029..00701035
                }
            getnum:
                if (integer64 != 0) {                                  // 00701038..0070103C
                    while (!done_flag) {                               // 00701042..00701112
                        if (comchr == 'x' || comchr == 'p') {          // 0070104C..00701054
                            if (kotor_cdecl_006fca16(ch)) {            // 00701097..007010A0
                                num64 <<= 4;                           // 007010A2..007010B2
                                if (!kotor_cdecl_006fc9dc(ch)) ch = (ch & ~0x20) - 7;   // 007010B5..007010C3
                            } else ++done_flag;
                        } else if (kotor_cdecl_006fc9dc(ch)) {         // 00701056..0070105F
                            if (comchr == 'o') {                       // 00701061..00701069
                                if (ch < '8') num64 <<= 3; else ++done_flag;   // 0070106B..0070107B
                            } else {
                                num64 = num64 * 10u;                   // 00701080..00701092 __allmul
                            }
                        } else ++done_flag;                            // 007010C8
                        if (!done_flag) {                              // 007010CB..007010CF
                            ++started;                                 // 007010D1
                            num64 += static_cast<U64>(static_cast<I64>(ch - '0'));   // 007010D4..007010DB cdq/adc
                            if (widthset != 0 && --width == 0) done_flag = 1;        // 007010DE..007010ED
                            else ch = INC();                           // 007010EF..007010F9
                        } else {
                            UN_INC(ch);                                // 007010FD..0070110C
                        }
                    }
                    if (negative) num64 = static_cast<U64>(-static_cast<I64>(num64));   // 0070111B..00701135
                } else {
                    while (!done_flag) {                               // 0070113D..007011E5
                        if (comchr == 'x' || comchr == 'p') {          // 00701147..0070114F
                            if (kotor_cdecl_006fca16(ch)) {            // 00701179..00701182
                                number <<= 4;                          // 00701184
                                if (!kotor_cdecl_006fc9dc(ch)) ch = (ch & ~0x20) - 7;   // 00701188..00701196
                            } else ++done_flag;
                        } else if (kotor_cdecl_006fc9dc(ch)) {         // 00701151..0070115A
                            if (comchr == 'o') {                       // 0070115C..00701164
                                if (ch < '8') number <<= 3; else ++done_flag;   // 00701166
                            } else {
                                number = number * 10u;                 // 0070116C..00701174
                            }
                        } else ++done_flag;                            // 0070119B
                        if (!done_flag) {                              // 0070119E..007011A2
                            ++started;                                 // 007011A4
                            number = number + static_cast<U>(ch) - 0x30u;   // 007011A7..007011AE
                            if (widthset != 0 && --width == 0) done_flag = 1;   // 007011B1..007011C0
                            else ch = INC();                           // 007011C2..007011CC
                        } else {
                            UN_INC(ch);                                // 007011D0..007011DF
                        }
                    }
                    if (negative) number = static_cast<U>(-static_cast<I>(number));   // 007011EE..007011F4
                }
                if (comchr == 'F') started = 0;                        // 007011F7..007011FC (unreachable: lowered)
                if (started == 0) goto error_return;                   // 00701200..00701204
                if (!suppress) {                                       // 0070120A..0070120E
                    ++count;                                           // 00701210
                assign_num:
                    if (integer64 != 0) {                              // 00701219..0070122A
                        U64 v = num64; unsigned char* p = static_cast<unsigned char*>(pointer);
                        for (int k = 0; k < 8; ++k) { p[k] = static_cast<unsigned char>(v); v >>= 8; }
                    } else if (longone != 0) {                         // 0070122C..00701234
                        *static_cast<U*>(pointer) = number;
                    } else {
                        *static_cast<std::uint16_t*>(pointer) = static_cast<std::uint16_t>(number);   // 00701236
                    }
                }
                break;
            case 'n':                                                  // 00700AAA
                number = static_cast<U>(charcount);
                if (!suppress) goto assign_num;                        // 00700AAD..00700AB7
                break;
            case 'e': case 'f': case 'g': {                            // 00700AE6
                char* scanptr = floatstring;                           // esi = [ebp-1C8h]
                if (ch == '-') { *scanptr++ = '-'; goto f_incwidth; }  // 00700AEC..00700B00
                if (ch == '+') {                                       // 00700B02..00700B05
                f_incwidth:
                    --width;                                           // 00700B07
                    ch = INC();                                        // 00700B0A..00700B19
                }
                if (widthset == 0 || width > 0x15d) width = 0x15d;     // 00700B21..00700B30
                const char decimal_point = *std::localeconv()->decimal_point;   // [007A32A4]
                while (kotor_cdecl_006fc9dc(ch) && width-- != 0) {     // 00700B39..00700B61
                    ++started;
                    *scanptr++ = static_cast<char>(ch);
                    ch = INC();
                }
                if (decimal_point == static_cast<char>(ch) && width-- != 0) {   // 00700B63..00700B73
                    ch = INC();                                        // 00700B75..00700B7F
                    *scanptr++ = decimal_point;                        // 00700B81..00700B88
                    while (kotor_cdecl_006fc9dc(ch) && width-- != 0) { // 00700B8B..00700BB3
                        ++started;
                        *scanptr++ = static_cast<char>(ch);
                        ch = INC();
                    }
                }
                if (started != 0 && (ch == 'e' || ch == 'E') && width-- != 0) {   // 00700BB5..00700BCD
                    *scanptr++ = 'e';                                  // 00700BCF..00700BD2
                    ch = INC();                                        // 00700BD3..00700BDF
                    if (ch == '-') { *scanptr++ = '-'; goto f_incwidth2; }   // 00700BE2..00700BEA
                    if (ch == '+') {                                   // 00700BEC..00700BEF
                    f_incwidth2:
                        if (width-- == 0) ++width;                     // 00700BF1..00700BFE
                        else ch = INC();                               // 00700C10..00700C1C
                    }
                    while (kotor_cdecl_006fc9dc(ch) && width-- != 0) { // 00700C00..00700C28
                        ++started;
                        *scanptr++ = static_cast<char>(ch);
                        ch = INC();
                    }
                }
                UN_INC(ch);                                            // 00700C2A..00700C39
                if (started == 0) goto error_return;                   // 00700C3B..00700C3F
                if (!suppress) {                                       // 00700C45..00700C49
                    ++count;                                           // 00700C4F
                    *scanptr = 0;                                      // 00700C52
                    kotor_cdecl_0070142c(longone - 1, pointer, floatstring);   // 00700C55..00700C65 [007A2DB8]
                }
                break;
            }
            default:                                                   // 00700CBA
                if (static_cast<int>(*format) != ch) {                 // 00700CBA..00700CC3
                    if (ch == k_eof) goto error_return;                // 00700CC5 / 00701295
                    (void)kotor_cdecl_007089f8(ch, stream);            // 00701297..0070129D
                    goto error_return;
                }
                --match;                                               // 00700CCE
                if (!suppress) arglist = arglistsave;                  // 00700CD1..00700CDE
                break;
            }
            goto matched;
        scanit: {                                                      // 00700E0C (c, s, [)
                std::uintptr_t start = reinterpret_cast<std::uintptr_t>(pointer);   // esi
                unsigned char* p = static_cast<unsigned char*>(pointer);       // ebx
                UN_INC(ch);                                            // 00700E0E..00700E22
                for (;;) {
                    if (widthset != 0 && width-- == 0) break;          // 00700E24..00700E32 -> 00700FCA
                    ch = INC();                                        // 00700E38..00700E43
                    bool take;
                    if (ch == k_eof) take = false;                     // 00700E46..00700E49
                    else if (comchr == 'c') take = true;               // 00700E4F..00700E52
                    else if (comchr == 's' && (ch < 9 || ch > 0xd) && ch != ' ') take = true;   // 00700E54..00700E66
                    else if (comchr == '{')                            // 00700E68..00700E8F
                        take = ((static_cast<signed char>(table[ch >> 3]) ^ static_cast<int>(reject)) &
                                (1 << (ch & 7))) != 0;
                    else take = false;
                    if (!take) { UN_INC(ch); goto scan_end; }          // 00700FB7..00700FC9
                    if (suppress) { ++start; continue; }               // 00700E98..00700E9C -> 00700FB1 (inc esi)
                    if (fl_wchar_arg) {                                // 00700EA2..00700EA6
                        char temp[2] = {static_cast<char>(ch), 0};     // [ebp-1D4h]
                        if (lead_byte(static_cast<unsigned char>(ch))) {   // 00700EAC..00700EC0
                            ++charcount;
                            temp[1] = static_cast<char>(inc_00700842(stream));   // 00700EC2..00700ECD
                        }
                        wchar_t wctemp = 0;                            // [ebp-1D8h]
                        (void)std::mbtowc(&wctemp, temp, MB_CUR_MAX);  // 00700ED3..00700EE7 [007A32A0]
                        *reinterpret_cast<std::uint16_t*>(p) = static_cast<std::uint16_t>(wctemp);   // 00700EEF..00700EF6
                        p += 2;
                    } else {
                        *p++ = static_cast<unsigned char>(ch);         // 00700FA6..00700FA8
                    }
                    pointer = p;                                       // 00700FA9
                }
            scan_end:
                if (start == reinterpret_cast<std::uintptr_t>(p)) goto error_return;   // 00700FCA..00700FCC
                if (!suppress) {                                       // 00700FD2..00700FD6
                    ++count;                                           // 00700FDC
                    if (comchr != 'c') {                               // 00700FDF..00700FE3
                        if (fl_wchar_arg) *static_cast<std::uint16_t*>(pointer) = 0;   // 00700FF2
                        else *static_cast<unsigned char*>(pointer) = 0;                // 00700FFB
                    }
                }
            }
        matched:
            ++match;                                                   // 00701239
            ++format;                                                  // 0070123C
        } else {
            ch = INC();                                                // 00701241..0070124E
            const int fc = *format++;                                  // 00701251..00701255
            if (fc != ch) {                                            // 00701258..0070125A
                if (ch == k_eof) goto error_return;                    // 00701292..00701295
                (void)kotor_cdecl_007089f8(ch, stream);                // 00701297..0070129D
                goto error_return;
            }
            if (lead_byte(static_cast<unsigned char>(ch))) {           // 0070125C..0070126A
                const int ch2 = INC();                                 // 0070126C..00701277
                const int fc2 = *format++;                             // 00701277..0070127B
                if (fc2 != ch2) {                                      // 0070127E..00701280
                    if (ch2 != k_eof) (void)kotor_cdecl_007089f8(ch2, stream);   // 00701282..0070128B
                    if (ch == k_eof) goto error_return;                // 00701292..00701295
                    (void)kotor_cdecl_007089f8(ch, stream);            // 00701297..0070129D
                    goto error_return;
                }
                --charcount;                                           // 007012A6
            }
        }
        if (ch == k_eof && (format[0] != '%' || format[1] != 'n')) goto error_return;   // 007012A9..007012BF
    }
error_return:                                                          // 007012C5
    if (ch == k_eof && count == 0 && match == 0) return -1;            // 007012D4..007012E6
    return count;
}

// ===================================================================================== 006FAE08 sscanf
extern "C" int __cdecl kotor_sscanf_006fae08(const char* input, const char* format, ...) {
    StrFile str;                                                       // [ebp-20h]
    str.flag = 0x49;                                                   // 006FAE12 _IOREAD|_IOSTRG|_IOMYBUF
    str.base = reinterpret_cast<const unsigned char*>(input);         // 006FAE19 [ebp-18h]
    str.ptr = reinterpret_cast<const unsigned char*>(input);          // 006FAE1C [ebp-20h]
    str.cnt = static_cast<I>(reconstructed_strlen_00701300(input));    // 006FAE1F..006FAE24
    va_list args;
    va_start(args, format);                                            // 006FAE27 lea eax, [ebp+10h]
    const int result = kotor_cdecl_00700858(&str, reinterpret_cast<const unsigned char*>(format), args);   // 006FAE2B..006FAE32
    va_end(args);
    return result;                                                     // 006FAE37..006FAE3B
}
