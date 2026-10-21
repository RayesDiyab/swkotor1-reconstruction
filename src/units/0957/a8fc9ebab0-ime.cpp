// lane-m48-traps / f-keys: CExoImeUI (IME composition / candidate handling), original IMM32 imports (D84 imm32.lib).
//   005DFB00 CExoImeUI::handleIme(hwnd, wparam, lparam)          __thiscall RET 0Ch  (consumer @kotor_fastcall_005dfb00@20)
//   005DFC40 CExoImeUI::HandleNotification(hwnd, wparam, lparam) __thiscall RET 0Ch  (consumer @kotor_fastcall_005dfc40@20)
//   005DF800 handleCompositionResult(hwnd, with_reading) RET 8, 005DF740 handleCompositionString(hwnd, lparam) RET 8,
//   005DFB40 UpdateCandidateList(hwnd, lparam) RET 8, 005DF980 HandleImeStatusChange(hwnd) RET 4,
//   005DF9D0 FormatCandidateList(result, list) RET 8 (ECX loaded by the caller, unused by the body).
// Reached on the menu from WinMessageHandler: WM_IME_COMPOSITION 00402E03 / WM_IME_NOTIFY 004032EF when
// CClientExoApp::IsClientImeUI, and from HandleWMKeyDownMessage VK_PROCESSKEY 004026FF.
// CExoImeUI observed fields: +000 flags (bit 1 = candidate list shown), +108 enabled, +110 result CExoString,
// +118 composition CExoString, +120 candidate CExoString.
// Thunks 0070C150/0070C14A/0070C144/0070C15C/0070C156 jump to IMM32 ImmGetContext / ImmGetCompositionStringA /
// ImmReleaseContext / ImmGetCandidateListA / ImmGetOpenStatus: called directly through imm32.lib.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <imm.h>
#include <cstdint>
#include <cstring>
#include <new>
#include "84a5500766-startup_string.h"
#include "a6391dda4a-string_priority.h"   // GuiAssign005e5140 (005E5140), GuiConcat005e5d10 (005E5D10)

using kotor_reconstruction::StartupString;
using U32 = std::uint32_t;

extern "C" {
StartupString* __fastcall WindowCopy005e5b00(StartupString*, void*, const StartupString* volatile);  // 005E5B00 copy ctor
void* __cdecl c2da_directory_new_006fa7e6(std::uint32_t) noexcept(false);                          // 006FA7E6 operator new
void __cdecl live_free(void*);                                                                      // 006FA390 free
int __cdecl format_006fadb0(char*, const char*, ...);                                               // 006FADB0 sprintf
}

namespace {
const char empty_0073d71c[] = "";                                       // 0073D71C
unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
StartupString* string_at(void* ime, U32 off) { return reinterpret_cast<StartupString*>(bytes(ime) + off); }
U32 flags(void* ime) { U32 v; std::memcpy(&v, ime, 4); return v; }
void set_flags(void* ime, U32 v) { std::memcpy(ime, &v, 4); }
// buf[len] = 0 with the original unsigned 32-bit index (a negative IMM error count wraps exactly as in the original).
void terminate_at(char* buf, LONG len) { *reinterpret_cast<char*>(reinterpret_cast<std::uintptr_t>(buf) + static_cast<U32>(len)) = 0; }
}

extern "C" {
// ---- 005DF800 handleCompositionResult(hwnd, with_reading) ----
void __fastcall kotor_fastcall_005df800(void* ime, void*, HWND hwnd, U32 with_reading) {
    HIMC himc = ImmGetContext(hwnd);                                    // 005DF821..26
    if (!himc) return;                                                  // 005DF82D..2F -> 005DF964
    const LONG length = ImmGetCompositionStringA(himc, GCS_RESULTSTR, nullptr, 0);  // 005DF836..40 (800h)
    if (length == 0) {                                                  // 005DF847..49 (unsigned JA)
        GuiAssign005e5140(string_at(ime, 0x110), nullptr, empty_0073d71c);  // 005DF84B..56
        ImmReleaseContext(hwnd, himc);                                  // 005DF85B..5E
        return;
    }
    char* text = static_cast<char*>(c2da_directory_new_006fa7e6(static_cast<U32>(length) + 1));  // 005DF862..66
    ImmGetCompositionStringA(himc, GCS_RESULTSTR, text, static_cast<DWORD>(length));  // 005DF86E..78
    terminate_at(text, length);                                         // 005DF886
    GuiAssign005e5140(string_at(ime, 0x110), nullptr, text);            // 005DF87D..8A
    live_free(text);                                                    // 005DF88F..90
    if (with_reading) {                                                 // 005DF895..9E
        const LONG reading = ImmGetCompositionStringA(himc, GCS_RESULTREADSTR, nullptr, 0);  // 005DF8A4..AE (1000h)
        if (reading != 0) {                                             // 005DF8B5..B7 (unsigned JBE)
            char* read_text = static_cast<char*>(c2da_directory_new_006fa7e6(static_cast<U32>(reading) + 1));  // 005DF8BD..C1
            ImmGetCompositionStringA(himc, GCS_RESULTREADSTR, read_text, static_cast<DWORD>(reading));  // 005DF8C9..D3
            terminate_at(read_text, reading);                           // 005DF8DD
            {
                StartupString addition(read_text);                      // 005DF8D8..E1 [esp+14h]
                alignas(StartupString) unsigned char slot[sizeof(StartupString)];  // [esp+20h] operator+ result slot
                StartupString* sum = GuiConcat005e5d10(string_at(ime, 0x110), nullptr, reinterpret_cast<StartupString*>(slot), &addition);  // 005DF8E6..FA
                string_at(ime, 0x110)->Assign005e5c50(sum);             // 005DF8FF..907
                sum->~StartupString();                                  // 005DF90C..15
            }                                                           // 005DF91A..26 ~addition
            live_free(read_text);                                       // 005DF92B..2C
            GuiAssign005e5140(string_at(ime, 0x118), nullptr, empty_0073d71c);  // 005DF938..43
            GuiAssign005e5140(string_at(ime, 0x120), nullptr, empty_0073d71c);  // 005DF948..53
        }
    }
    ImmReleaseContext(hwnd, himc);                                      // 005DF958..5E
}

// ---- 005DF740 handleCompositionString(hwnd, lparam) ----
void __fastcall kotor_fastcall_005df740(void* ime, void*, HWND hwnd, U32 lparam) {
    HIMC himc = ImmGetContext(hwnd);                                    // 005DF747..4A
    if (!himc) return;                                                  // 005DF751..53
    const LONG length = ImmGetCompositionStringA(himc, GCS_COMPSTR, nullptr, 0);  // 005DF759..61 (8)
    if (length == 0) { ImmReleaseContext(hwnd, himc); return; }         // 005DF766..77
    char* text = static_cast<char*>(c2da_directory_new_006fa7e6(static_cast<U32>(length) + 1));  // 005DF77A..7E
    ImmGetCompositionStringA(himc, GCS_COMPSTR, text, static_cast<DWORD>(length));  // 005DF786..8D
    terminate_at(text, length);                                         // 005DF799
    GuiAssign005e5140(string_at(ime, 0x118), nullptr, text);            // 005DF792..9D
    live_free(text);                                                    // 005DF7A2..A3
    if (lparam & GCS_COMPATTR) {                                        // 005DF7A8..B1 (10h)
        const LONG attributes = ImmGetCompositionStringA(himc, GCS_COMPATTR, nullptr, 0);  // 005DF7B3..BA
        if (attributes != 0) {                                          // 005DF7C1..C3 (unsigned JBE)
            char* attr = static_cast<char*>(c2da_directory_new_006fa7e6(static_cast<U32>(attributes) + 1));  // 005DF7C5..C9
            ImmGetCompositionStringA(himc, GCS_COMPATTR, attr, static_cast<DWORD>(attributes));  // 005DF7D1..D8
            terminate_at(attr, attributes);                             // 005DF7DE
            live_free(attr);                                            // 005DF7DD..E2 (read and discarded)
        }
    }
    ImmReleaseContext(hwnd, himc);                                      // 005DF7EA..F0
}

// ---- 005DFB00 handleIme(hwnd, wparam, lparam) ----
void __fastcall kotor_fastcall_005dfb00(void* ime, void*, U32 hwnd, U32 wparam, U32 lparam) {
    (void)wparam;                                                       // not read by the original
    HWND window = reinterpret_cast<HWND>(static_cast<std::uintptr_t>(hwnd));
    if (lparam & GCS_RESULTSTR) {                                       // 005DFB00..07 (test ah,8)
        kotor_fastcall_005df800(ime, nullptr, window, (lparam & GCS_RESULTREADSTR) ? 1u : 0u);  // 005DFB09..24 (test ah,10h)
        return;
    }
    if (lparam & GCS_COMPSTR) kotor_fastcall_005df740(ime, nullptr, window, lparam);  // 005DFB2C..36 (test al,8)
}

// ---- 005DF9D0 FormatCandidateList(result, list) ----
StartupString* __fastcall kotor_fastcall_005df9d0(void* unused_ecx, void*, StartupString* result, const CANDIDATELIST* list) {
    (void)unused_ecx;                                                   // ECX = CExoImeUI at 005DFBDD, never read
    StartupString text;                                                 // 005DF9EB..FC [esp+10h]
    int number = 1;                                                     // 005DF9F7 ebp = 1
    U32 end = list->dwPageSize;                                         // 005DFA05
    if (end == 0) end = 5;                                              // 005DFA08..10
    end += list->dwPageStart;                                           // 005DFA15..1B
    if (end >= list->dwCount) end = list->dwCount;                      // 005DFA1D..21 (JB keeps)
    for (U32 i = list->dwPageStart; i < end; ++i) {                     // 005DFA23..25 / 005DFAB0..B8
        const char* candidate = reinterpret_cast<const char*>(list) + list->dwOffset[i];  // 005DFA2E..38
        char line[0x34];                                                // [esp+2Ch] (0x34-byte local)
        const int length = lstrlenA(candidate);                         // 005DFA3A..3B (IAT 0073D190)
        if (static_cast<U32>(length) < sizeof(line)) line[length] = 0;  // 005DFA43 (original stores unconditionally)
        format_006fadb0(line, "%d:%s  ", number, candidate);            // 005DFA41..52 (0074D5F4)
        ++number;                                                       // 005DFA63
        StartupString piece(line);                                      // 005DFA5A..64 [esp+1Ch]
        {
            alignas(StartupString) unsigned char slot[sizeof(StartupString)];  // [esp+24h] operator+ result slot
            StartupString* sum = GuiConcat005e5d10(&text, nullptr, reinterpret_cast<StartupString*>(slot), &piece);  // 005DFA69..7C
            text.Assign005e5c50(sum);                                   // 005DFA81..8B
            sum->~StartupString();                                      // 005DFA90..99
        }
    }                                                                   // 005DFA9E..A7 ~piece
    WindowCopy005e5b00(result, nullptr, &text);                         // 005DFABF..CA (copy-construct the result)
    return result;                                                      // 005DFAE9 (EAX = result), ~text 005DFAE0
}

// ---- 005DFB40 UpdateCandidateList(hwnd, lparam) ----
void __fastcall kotor_fastcall_005dfb40(void* ime, void*, HWND hwnd, U32 lparam) {
    set_flags(ime, flags(ime) & 0xFFFFFFFDu);                           // 005DFB5D..6D
    GuiAssign005e5140(string_at(ime, 0x120), nullptr, empty_0073d71c);  // 005DFB62..6F
    HIMC himc = ImmGetContext(hwnd);                                    // 005DFB74..79
    if (!himc) return;                                                  // 005DFB80..82
    U32 index = 0;                                                      // 005DFB8D
    while (index < 0x20u && !(lparam & (1u << index))) ++index;         // 005DFB90..A1
    if (index != 0x20u) {                                               // 005DFBA3..A6
        const DWORD size = ImmGetCandidateListA(himc, index, nullptr, 0);  // 005DFBA8..AE
        if (size != 0) {                                                // 005DFBB3..B9
            auto* list = static_cast<CANDIDATELIST*>(c2da_directory_new_006fa7e6(size));  // 005DFBBF..C8
            ImmGetCandidateListA(himc, index, list, size);              // 005DFBCA..D2
            {
                alignas(StartupString) unsigned char storage[sizeof(StartupString)];  // [esp+10h] result slot
                StartupString* formatted = kotor_fastcall_005df9d0(ime, nullptr, reinterpret_cast<StartupString*>(storage), list);  // 005DFBD7..DF
                string_at(ime, 0x120)->Assign005e5c50(formatted);       // 005DFBE4..F3
                formatted->~StartupString();                            // 005DFBF8..C04
            }
            live_free(list);                                            // 005DFC09..0A
            set_flags(ime, flags(ime) | 2u);                            // 005DFC0F..17
        }
    }
    ImmReleaseContext(hwnd, himc);                                      // 005DFBBB..BD / 005DFC19..1F
}

// ---- 005DF980 HandleImeStatusChange(hwnd) ----
void __fastcall kotor_fastcall_005df980(void* ime, void*, HWND hwnd) {
    HIMC himc = ImmGetContext(hwnd);                                    // 005DF987..8A
    if (!himc) return;                                                  // 005DF991..93
    if (!ImmGetOpenStatus(himc)) {                                      // 005DF995..9D
        GuiAssign005e5140(string_at(ime, 0x120), nullptr, empty_0073d71c);  // 005DF99F..AA
        set_flags(ime, 0);                                              // 005DF9AF (whole flags word = 0)
    }
    ImmReleaseContext(hwnd, himc);                                      // 005DF9B5..B7
}

// ---- 005DFC40 HandleNotification(hwnd, wparam, lparam): returns 1 when handled ----
U32 __fastcall kotor_fastcall_005dfc40(void* ime, void*, U32 hwnd, U32 wparam, U32 lparam) {
    HWND window = reinterpret_cast<HWND>(static_cast<std::uintptr_t>(hwnd));
    switch (wparam) {                                                   // 005DFC40..4F table 005DFCA8 (wparam-3, 0..5)
    case IMN_OPENCANDIDATE:                                             // 3 -> 005DFC56
    case IMN_CHANGECANDIDATE:                                           // 5 -> 005DFC56
        kotor_fastcall_005dfb40(ime, nullptr, window, lparam);          // 005DFC56..62
        return 1;
    case IMN_CLOSECANDIDATE:                                            // 4 -> 005DFC70
        GuiAssign005e5140(string_at(ime, 0x120), nullptr, empty_0073d71c);  // 005DFC70..7B
        set_flags(ime, flags(ime) & 0xFFFFFFFDu);                       // 005DFC80
        return 1;
    case IMN_SETOPENSTATUS:                                             // 8 -> 005DFC8C
        kotor_fastcall_005df980(ime, nullptr, window);                  // 005DFC8C..93
        return 1;
    default:                                                            // 6, 7, out of range -> 005DFCA1
        return 0;
    }
}
}
