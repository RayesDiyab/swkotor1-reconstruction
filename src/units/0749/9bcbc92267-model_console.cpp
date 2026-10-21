// model_console.cpp - lane-3d-model rev2: the console function interpreter reached by the ASCII model loader
// (004A1576): FuncInterp 0044C1F0 and FindConCallBack 0044BB90, with the cells they read and write.
// The registry cells (007A8AB8 table, 007B8FB8 count, 0078E354 last hit) are also written by ConsoleFuncs_Add
// 0044BA80 (not in this lane's closure); this lane offers itself as their single owner (open gate G-CONSOLE).
#include "a7e200e217-model_instance.hpp"
#include <cstdio>
#include <cstring>

using namespace aurora_model;

namespace {
// ConCallBack: vtbl+4 Run(args), thiscall RET4 (0044C537..0044C541); return value not used.
using ConRun = U (__fastcall*)(void* self, void* unused_edx, char* args);
struct ConCallBackVTable { U slot00; ConRun run; };
struct ConCallBack { const ConCallBackVTable* vtable; };
static_assert(offsetof(ConCallBackVTable, run) == 4, "observed slot");
}

extern "C" {
// registry entry pointers: each entry starts with its name (strcmp operand) and holds the ConCallBack* at +50h.
U aurora_console_funcs_007a8ab8[0x4000] = {};                 // .bss 007A8AB8..007B8AB7 (span to 007B8AB8)
char aurora_console_result_007b8ab8[0x500] = {};              // .bss 007B8AB8..007B8FB7 (span to 007B8FB8)
volatile I aurora_console_func_count_007b8fb8 = 0;            // .bss
volatile I aurora_console_history_cursor_007b8fbc = 0;        // .bss
List aurora_console_history_007b8fcc = {0, 0, 0};             // .bss {data 007B8FCC, size 007B8FD0, capacity 007B8FD4}
volatile I aurora_console_last_func_0078e354 = -1;            // .data initial value -1
void* __cdecl aurora_find_con_callback_0044bb90(const char* name);
char* __cdecl aurres_func_interp_0044c1f0(char* line);
}

// 0044BB90 FindConCallBack(name), cdecl: the cached index 0078E354 first, then a linear search (cache updated);
// returns the entry's ConCallBack* (+50h) or 0 (cache = count after a failed search, 0 when the table is empty).
void* __cdecl aurora_find_con_callback_0044bb90(const char* name) {
    const I last = aurora_console_last_func_0078e354;                               // 0044BB90
    const I count = aurora_console_func_count_007b8fb8;                             // 0044BB97
    if (last >= 0 && last < count) {                                               // 0044BBA5..0044BBA9
        const char* const entry = ptr<const char>(aurora_console_funcs_007a8ab8[last]); // 0044BBAB
        if (std::strcmp(name, entry) == 0) return get<void*>(entry, 0x50);          // 0044BBB2..0044BBE3 (inline strcmp)
    }
    I i = 0;                                                                        // 0044BBEB
    aurora_console_last_func_0078e354 = 0;                                          // 0044BBEF
    if (count <= 0) return nullptr;                                                 // 0044BBED..0044BBF5
    for (; i < count; ++i) {                                                        // 0044BC00..0044BC40
        const char* const entry = ptr<const char>(aurora_console_funcs_007a8ab8[i]);
        if (std::strcmp(name, entry) == 0) {                                        // 0044BC07..0044BC3B
            aurora_console_last_func_0078e354 = i;                                  // 0044BC59
            return get<void*>(ptr<void>(aurora_console_funcs_007a8ab8[i]), 0x50);   // 0044BC4F..0044BC56
        }
    }
    aurora_console_last_func_0078e354 = i;                                          // 0044BC42
    return nullptr;
}

// 0044C1F0 FuncInterp(line), cdecl -> 007B8AB8: skip blanks; with the history gate 007B8FC0 set, a copy of the
// line is appended to the history list unless it repeats the last entry; then bracketed sub-expressions are
// evaluated recursively and spliced in (quoted spans skipped; missing ']' -> "missing ']'"); the first word names
// the console function, the rest (after '=' / ' ', trailing whitespace trimmed) is passed to its Run.
char* __cdecl aurres_func_interp_0044c1f0(char* line) {
    char name[0x80];                                                                // [esp+10h]
    char work[0x4000];                                                              // [esp+90h]
    char args[0x4000];                                                              // [esp+4090h]
    char splice[0x4000];                                                            // [esp+8090h] (0xC080 frame, _chkstk 006FC090)
    while (*line == ' ' || *line == '\t') ++line;                                   // 0044C205..0044C210
    if (aurora_model_gate_007b8fc0 != 0) {                                          // 0044C212..0044C219
        char* const copy = static_cast<char*>(live_operator_new(static_cast<U>(std::strlen(line)) + 1)); // 0044C21F..0044C22F
        std::strcpy(copy, line);                                                    // 0044C236..0044C248
        List* const h = &aurora_console_history_007b8fcc;
        const I n = h->size;                                                        // 0044C24A
        if (n == 0 || std::strcmp(copy, ptr<const char>(ptr<U>(h->data)[n - 1])) != 0) { // 0044C250..0044C28B (inline strcmp)
            if (h->size == h->capacity)                                             // 0044C28D..0044C294
                menu_list_growth::allocate_005e03c0(h, nullptr, h->capacity ? static_cast<U>(h->capacity) * 2u : 8u); // 0044C296..0044C2A9
            ptr<U>(h->data)[h->size] = addr(copy);                                  // 0044C2B4..0044C2B9
            h->size = h->size + 1;                                                  // 0044C2BC..0044C2C2
            aurora_console_history_cursor_007b8fbc = h->size;                       // 0044C2C7
        }
    }
    aurora_console_result_007b8ab8[0] = 0;                                          // 0044C2D3
    std::strcpy(work, line);                                                        // 0044C2CC..0044C2E8
    if (work[0] == '"') {                                                           // 0044C2EA..0044C300
        char* q = work;
        while (*q != '"') { if (q[1] == 0) { q = nullptr; break; } ++q; }           // 0044C302..0044C30F
        if (q != nullptr) *q = ' ';                                                 // 0044C311
    }
    char* p = work;                                                                 // 0044C31D
    if (*p != 0) {                                                                  // 0044C314..0044C324
        for (;;) {
            if (*p == '"') {                                                        // 0044C330..0044C333
                for (char c = p[1]; ++p, c != 0 && c != '"'; c = p[1]) {}           // 0044C335..0044C34B
            }
            if (*p == '[') {                                                        // 0044C34D..0044C350
                char* close = p;                                                    // 0044C356
                I depth = 1;                                                        // 0044C358
                for (;;) {                                                          // 0044C360..0044C37A
                    const char c = *++close;
                    if (c == 0) {                                                   // 0044C364..0044C366 -> 0044C433
                        std::memcpy(aurora_console_result_007b8ab8, "missing ']'", 12); // 0044C433..0044C452 (00741474, three dwords)
                        return aurora_console_result_007b8ab8;
                    }
                    if (c == '[') ++depth;                                          // 0044C36C..0044C371
                    if (c == ']') --depth;                                          // 0044C372..0044C377
                    if (depth == 0) break;
                }
                *close = 0;                                                         // 0044C37C
                *p = 0;                                                             // 0044C37F
                const char* const value = aurres_func_interp_0044c1f0(p + 1);       // 0044C381..0044C383
                std::strcpy(splice, work);                                          // 0044C38B..0044C3A1
                std::strcat(splice, value);                                         // 0044C3A3..0044C3CF
                std::strcat(splice, close + 1);                                     // 0044C3CC..0044C3FA
                std::strcpy(work, splice);                                          // 0044C3FC..0044C411
                p = work;                                                           // 0044C413
            }
            if (p[1] == 0) break;                                                   // 0044C41A..0044C420
            ++p;
        }
    }
    char* s = work;                                                                 // ebx
    while (*s == ' ' || *s == '\t') ++s;                                            // 0044C426..0044C431
    I k = 0;                                                                        // 0044C467
    for (char c = *s; c != 0 && c != ' ' && c != '='; c = *++s) name[k++] = c;      // 0044C465..0044C483
    name[k] = 0;                                                                    // 0044C485
    while (*s == '=' || *s == ' ') ++s;                                             // 0044C490..0044C49B
    ConCallBack* const cb = static_cast<ConCallBack*>(aurora_find_con_callback_0044bb90(name)); // 0044C49D..0044C4A2
    if (cb == nullptr) {                                                            // 0044C4AC..0044C4AE
        std::sprintf(aurora_console_result_007b8ab8, "Function %s not found", name); // 0044C4B0..0044C4BF (0074145C; 006FADB0)
        return aurora_console_result_007b8ab8;
    }
    std::strcpy(args, s);                                                           // 0044C4D7..0044C4EA
    char* end = args + std::strlen(args);                                           // 0044C4EC..0044C4FF
    while (end > args) {                                                            // 0044C506..0044C535
        const char c = end[-1];
        if (c != ' ' && c != '\t' && c != '\r' && c != '\n') break;                 // 0044C511..0044C526
        --end;
        *end = 0;                                                                   // 0044C528..0044C532
    }
    cb->vtable->run(cb, nullptr, args);                                             // 0044C537..0044C541
    return aurora_console_result_007b8ab8;                                          // 0044C547
}
