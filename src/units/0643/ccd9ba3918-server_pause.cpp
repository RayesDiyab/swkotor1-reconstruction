// lane-window: CServerExoApp::SetPauseState 004AE9A0 -> CServerExoAppInternal::SetPauseState 004B8110
// and the CWorldTimer leaves it reaches that no other lane owns: 004ADFF0 PauseWorldTimer,
// 004ADEF0 SetMinutesPerHour-style copy (Ghidra: CWorldTimer::operator= copy body), 004ADC70
// GetSnapshotTime, 004AE030 UnpauseWorldTimer. 004AE980/004B1D80 GetPauseState, 004ADBD0 and
// 004ADE40 are lane-r4-lowers' (referenced, not duplicated). Evidence: dumpbin /disasm swkotor.exe
// 004AE9A0..004AE9A3, 004B8110..004B82F3, 004ADFF0..004AE027, 004ADEF0..004ADF4D, 004ADC70..004ADC76,
// 004AE030..004AE080; CRT 006FB030 = _aulldiv, 006FD0C0 = _aulldvrm (quotient EDX:EAX, remainder EBX:ECX).
#include "07738bacbe-ini_section.h"   // M41 ini_section::AddTail005e9dc0 (kotor_list::Internal)
#include "7b8c81fcff-allocator_live.h"
#include <cstdint>
#include <cstring>

using U32 = std::uint32_t; using U64 = std::uint64_t;
namespace menu_continuation { struct WorldTimer; }
extern "C" {
// lane-r4-lowers (r4_frame_lowers.cpp)
void __fastcall frame_R4_004adbd0(void* timer, void*);
void __fastcall frame_GetWorldTime_004ade40(menu_continuation::WorldTimer* t, void*, unsigned* day, unsigned* ms);
// lane-r4-lowers r4_frame_lowers.cpp: CGameObjectArray::GetGameObject 004D8230
unsigned char __fastcall kotor_fastcall_004d8230(void* array, void*, std::uint32_t id, void** out);
// M41 resource-list-accessor-exports.obj: CExoLinkedList GetAtPos 005E9B60 / GetNext 005E9B90
std::uint32_t __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
std::uint32_t __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
// still missing (client domain): 005F7FA0 (behind 005ED650, RET 0Ch) and 005F2AC0 (behind 005EDA50, RET 4)
void __fastcall kotor_fastcall_005f7fa0(void* internal, void*, U32 type, U32 state, void* list);
std::uint32_t __fastcall kotor_fastcall_005f2ac0(void* internal, void*, std::uint32_t id);
// authored here (004B8020 RET 4; 0056C9F0 RET 8, EAX=1)
void __fastcall kotor_fastcall_004b8020(void* internal, void*, U32 type);
U32 __fastcall kotor_fastcall_0056c9f0(void* ignored_this, void*, U32 type, U32 state);
void* __fastcall kotor_fastcall_004ae9d0(void* server, void*);
void* __fastcall kotor_fastcall_004b1dc0(void* internal, void*);
U32 __fastcall kotor_fastcall_004b48e0(void* internal, void*, std::uint32_t id);
void __fastcall kotor_fastcall_005ed650(void* client, void*, U32 type, U32 state, void* list);
std::uint32_t __fastcall kotor_fastcall_005eda50(void* client, void*, std::uint32_t id);
// authored here
void __fastcall kotor_fastcall_004ae9a0(void* server, void*, U32 type, U32 state);
void __fastcall kotor_fastcall_004b8110(void* internal, void*, U32 type, U32 state);
void __fastcall kotor_fastcall_004adff0(void* timer, void*);
void __fastcall kotor_fastcall_004adef0(void* timer, void*, const void* source);
U64 __fastcall kotor_fastcall_004adc70(const void* timer, void*);
void __fastcall kotor_fastcall_004ae030(void* timer, void*);
}

namespace {
unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
U32 rd(const void* p, unsigned o) { U32 v; std::memcpy(&v, static_cast<const unsigned char*>(p) + o, 4); return v; }
void wr(void* p, unsigned o, U32 v) { std::memcpy(bytes(p) + o, &v, 4); }
void* ptr(const void* p, unsigned o) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(rd(p, o))); }
}

// 004ADC70: mov eax,[ecx+0Ch]; mov edx,[ecx+10h]; ret.
extern "C" U64 __fastcall kotor_fastcall_004adc70(const void* t, void*) {
    return (static_cast<U64>(rd(t, 0x10)) << 32) | rd(t, 0x0c);
}
// 004ADEF0 (RET 4): field copy in the original order, then [+8] = 64h.
extern "C" void __fastcall kotor_fastcall_004adef0(void* t, void*, const void* s) {
    static const unsigned words[] = {0x00, 0x04, 0x0c, 0x10, 0x14, 0x18, 0x24, 0x28, 0x2c, 0x30, 0x34};
    for (unsigned o : words) wr(t, o, rd(s, o));                     // 004ADEF4..004ADF31
    bytes(t)[0x38] = static_cast<const unsigned char*>(s)[0x38];     // 004ADF34..004ADF37
    wr(t, 0x3c, rd(s, 0x3c));                                        // 004ADF3A..004ADF3D
    wr(t, 0x40, rd(s, 0x40));                                        // 004ADF40..004ADF43
    wr(t, 0x08, 0x64);                                               // 004ADF46
}
// 004ADFF0 PauseWorldTimer (RET 0).
extern "C" void __fastcall kotor_fastcall_004adff0(void* t, void*) {
    if (rd(t, 0x24) != 0) return;                                    // 004ADFF6..004ADFFB
    unsigned day = 0, ms = 0;                                        // locals [esp+4]/[esp+8] (written by callee)
    frame_GetWorldTime_004ade40(static_cast<menu_continuation::WorldTimer*>(t), nullptr, &day, &ms); // 004ADFFD..004AE009
    wr(t, 0x28, day);                                                // 004AE016
    wr(t, 0x2c, ms);                                                 // 004AE019
    wr(t, 0x24, 1);                                                  // 004AE01C
}
// 004AE030 UnpauseWorldTimer (RET 0).
extern "C" void __fastcall kotor_fastcall_004ae030(void* t, void*) {
    if (rd(t, 0x24) != 1) return;                                    // 004AE036..004AE03A
    const U64 snapshot = (static_cast<U64>(rd(t, 0x10)) << 32) | rd(t, 0x0c);
    const U64 seconds = snapshot / 1000u;                            // 004AE046..004AE04E _aulldiv
    const U64 period = rd(t, 0x3c);                                  // 004AE053..004AE05A _aulldvrm(.., [+3C], 0)
    const U64 quotient = seconds / period, remainder = seconds % period;
    wr(t, 0x24, 0);                                                  // 004AE06F
    wr(t, 0x30, rd(t, 0x28) - static_cast<U32>(quotient));          // 004AE067..004AE06A, 004AE075 (EAX = quotient low)
    wr(t, 0x34, rd(t, 0x2c) - static_cast<U32>(remainder));         // 004AE06C..004AE072, 004AE078 (ECX = remainder low)
}
// 004AE9A0: mov ecx,[ecx+4]; jmp 004B8110.
extern "C" void __fastcall kotor_fastcall_004ae9a0(void* server, void*, U32 type, U32 state) {
    kotor_fastcall_004b8110(ptr(server, 4), nullptr, type, state);
}
// 004B8110 CServerExoAppInternal::SetPauseState(type, state) (RET 8; EAX incidental).
extern "C" void __fastcall kotor_fastcall_004b8110(void* s, void*, U32 type, U32 state) {
    const std::uint8_t bl = static_cast<std::uint8_t>(type);
    std::uint8_t flags = bytes(s)[0x10078];                          // 004B811E
    const U32 current = (flags & bl) != 0 ? 1u : 0u;                 // 004B8124..004B813B
    void* const module = ptr(s, 0x10010);                            // 004B812F..004B8135 (saved before changes)
    if (current == state) return;                                    // 004B813D..004B813F -> 004B82EE
    auto timer = [s](unsigned o) { return ptr(s, o); };
    auto snapshot = [&](unsigned o) {                                // 004ADC70 then [t+14h]=eax, [t+18h]=edx
        void* t = timer(o); const U64 v = kotor_fastcall_004adc70(t, nullptr);
        wr(t, 0x14, static_cast<U32>(v)); wr(t, 0x18, static_cast<U32>(v >> 32));
    };
    if (state == 1) {                                                // 004B8145..004B8148
        void* t48 = timer(0x10048);                                  // 004B814A (loaded before the flag store)
        bytes(s)[0x10078] = static_cast<std::uint8_t>(flags | bl);   // 004B8150..004B8152
        kotor_fastcall_004adff0(t48, nullptr);                       // 004B8158
        if (bytes(s)[0x10078] & 2) kotor_fastcall_004adff0(timer(0x1004c), nullptr); // 004B815D..004B816C
        (void)kotor_fastcall_0056c9f0(module, nullptr, type, 1);     // 004B8171..004B8176
        return;
    }
    flags = static_cast<std::uint8_t>(~bl & flags);                  // 004B8183..004B8187
    bytes(s)[0x10078] = flags;                                       // 004B818C
    if (bl == 2) {                                                   // 004B8189 / 004B8192
        kotor_fastcall_004adff0(timer(0x10050), nullptr);            // 004B8198..004B819E
        kotor_fastcall_004b8020(s, nullptr, 2);                      // 004B81A3..004B81A7
        kotor_fastcall_004adef0(timer(0x10050), nullptr, timer(0x1004c)); // 004B81AC..004B81B9
        frame_R4_004adbd0(timer(0x10050), nullptr);                  // 004B81BE..004B81C4
        frame_R4_004adbd0(timer(0x1004c), nullptr);                  // 004B81C9..004B81CF
        snapshot(0x10050);                                           // 004B81D4..004B81E4
        snapshot(0x1004c);                                           // 004B81E7..004B81F7
        const std::uint8_t now = bytes(s)[0x10078];                  // 004B81FA
        if ((bl & now) == 0 && (now & 1) != 0) {                     // 004B8200..004B8206 -> 004B82CD
            kotor_fastcall_004ae030(timer(0x1004c), nullptr);        // 004B82CD..004B82D3
            kotor_fastcall_004ae030(timer(0x10050), nullptr);        // 004B82D8..004B82DE
            (void)kotor_fastcall_0056c9f0(module, nullptr, type, state);// 004B82E3..004B82E9
            return;
        }
        frame_R4_004adbd0(timer(0x10048), nullptr);                  // 004B820C..004B8212
    } else if (bl == 1) {                                            // 004B821C..004B821F
        kotor_fastcall_004adff0(timer(0x1004c), nullptr);            // 004B8225..004B822B
        kotor_fastcall_004adff0(timer(0x10050), nullptr);            // 004B8230..004B8236
        kotor_fastcall_004b8020(s, nullptr, 1);                      // 004B823B..004B823F
        kotor_fastcall_004adef0(timer(0x1004c), nullptr, timer(0x10048)); // 004B8244..004B8251
        kotor_fastcall_004adef0(timer(0x10050), nullptr, timer(0x10048)); // 004B8256..004B8263
        frame_R4_004adbd0(timer(0x10048), nullptr);                  // 004B8268..004B826E
        frame_R4_004adbd0(timer(0x1004c), nullptr);                  // 004B8273..004B8279
        frame_R4_004adbd0(timer(0x10050), nullptr);                  // 004B827E..004B8284
        snapshot(0x10050);                                           // 004B8289..004B8299
        snapshot(0x1004c);                                           // 004B829C..004B82AC
    } else {
        (void)kotor_fastcall_0056c9f0(module, nullptr, type, state);    // 004B821F -> 004B82E3
        return;
    }
    snapshot(0x10048);                                               // 004B82AF..004B82BF (store order +18 then +14: no observable difference)
    kotor_fastcall_004ae030(timer(0x10048), nullptr);                // 004B82C2..004B82C8
    kotor_fastcall_004ae030(timer(0x1004c), nullptr);                // 004B82CD..004B82D3
    kotor_fastcall_004ae030(timer(0x10050), nullptr);                // 004B82D8..004B82DE
    (void)kotor_fastcall_0056c9f0(module, nullptr, type, state);         // 004B82E3..004B82E9
}

// ---------------- 004B8020 / 0056C9F0 and their leaves ----------------
namespace kotor_input_hover {      // lane-input input_system.h (identical declaration)
struct ListWrapper {
    kotor_list::Internal* internal;
    ListWrapper* __thiscall Construct005e0350();
    ListWrapper* __thiscall Construct004b25d0();
};
}
namespace kotor_window_pause {
// Typed view of slot +08 of the table of an object returned by 004D8230, as called at
// 004B80DD..004B80E5 (ECX object, two stack words, callee cleanup). Bodies belong to the
// game-object classes (open gate).
struct GameObject;
struct GameObjectTable { void* s00; void* s04; void (__fastcall* s08)(GameObject*, void*, U32 day, U32 ms); };
struct GameObject { const GameObjectTable* table; };
}
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }   // M41 window-shared-cells.obj
namespace {
kotor_list::Internal* list_of(void* wrapper) { return static_cast<kotor_list::Internal*>(ptr(wrapper, 0)); }
void* head_of(void* wrapper) { return list_of(wrapper)->head; }   // [[wrapper]]
// 0056CA2A..0056CA49: new(4) (006FA7E6); EH state 0 frees it if 004B25D0 throws; null stays null.
kotor_input_hover::ListWrapper* construct_client_list() {
    void* memory = live_operator_new(4);
    if (!memory) return nullptr;
    try { return static_cast<kotor_input_hover::ListWrapper*>(memory)->Construct004b25d0(); }
    catch (...) { live_free(memory); throw; }
}
}
// 004B1DC0 (RET 0): (flags&2) ? [+10070] : (flags&1) ? [+10074] : 0.
extern "C" void* __fastcall kotor_fastcall_004b1dc0(void* s, void*) {
    const std::uint8_t f = bytes(s)[0x10078];
    if (f & 2) return ptr(s, 0x10070);                               // 004B1DC6..004B1DC8 / 004B1DD7
    if (f & 1) return ptr(s, 0x10074);                               // 004B1DCA..004B1DD0
    return nullptr;                                                   // 004B1DDE
}
// 004AE9D0: mov ecx,[ecx+4]; jmp 004B1DC0.
extern "C" void* __fastcall kotor_fastcall_004ae9d0(void* server, void*) { return kotor_fastcall_004b1dc0(ptr(server, 4), nullptr); }
// 004B48E0 (RET 4): 1 unless the active pause list exists and does not contain id.
extern "C" U32 __fastcall kotor_fastcall_004b48e0(void* s, void*, std::uint32_t id) {
    const std::uint8_t f = bytes(s)[0x10078];                        // 004B48E1
    void* wrapper;
    if (f & 2) wrapper = ptr(s, 0x10070);                             // 004B48E7..004B48EA / 004B4906
    else if ((f & 1) == 0) return 1;                                  // 004B48EC..004B48F0 -> 004B48FC
    else wrapper = ptr(s, 0x10074);                                   // 004B48F2
    if (!wrapper) return 1;                                           // 004B48F8..004B48FA
    void* pos = head_of(wrapper);                                     // 004B490E..004B4915
    while (pos) {                                                     // 004B4919 / 004B493C..004B493E
        if (unresolved_list_at_pos_005e9b60(list_of(wrapper), nullptr, pos) == id) return 1; // 004B4920..004B492A -> 004B4948
        (void)unresolved_list_next_005e9b90(list_of(wrapper), nullptr, &pos); // 004B492C..004B4933
    }
    return 0;                                                         // 004B4940..004B4941
}
// 005ED650: mov ecx,[ecx+4]; jmp 005F7FA0.
extern "C" void __fastcall kotor_fastcall_005ed650(void* client, void*, U32 type, U32 state, void* list) {
    kotor_fastcall_005f7fa0(ptr(client, 4), nullptr, type, state, list);
}
// 005EDA50: mov ecx,[ecx+4]; jmp 005F2AC0 (ServerToClientObjectId).
extern "C" std::uint32_t __fastcall kotor_fastcall_005eda50(void* client, void*, std::uint32_t id) {
    return kotor_fastcall_005f2ac0(ptr(client, 4), nullptr, id);
}
// 004B8020 (RET 4): pass each listed object its timer's world time.
extern "C" void __fastcall kotor_fastcall_004b8020(void* s, void*, U32 type) {
    void* const wrapper = static_cast<std::uint8_t>(type) == 2 ? ptr(s, 0x10070) : ptr(s, 0x10074); // 004B8020..004B8037
    void* pos = head_of(wrapper);                                     // 004B803D..004B8043 (stored over the arg cell)
    while (pos) {                                                     // 004B8047 / 004B80F8..004B80FA
        const std::uint32_t id = unresolved_list_at_pos_005e9b60(list_of(wrapper), nullptr, pos); // 004B8050..004B8058
        void* timer;
        if (id == 0x7f000000u) timer = ptr(s, 0x10048);               // 004B805A..004B8060 -> 004B80AB
        else {
            const std::uint8_t f = bytes(s)[0x10078];                // 004B8062
            const std::uint8_t two = static_cast<std::uint8_t>(f & 2);
            if (two == 0 && (f & 1) == 0) timer = ptr(s, 0x10048);    // 004B806D..004B8071 -> 004B80AB
            else if (two != 0)                                        // 004B8073..004B8075 -> 004B8092
                timer = kotor_fastcall_004b48e0(s, nullptr, id) == 1 ? ptr(s, 0x10050) : ptr(s, 0x10048); // 004B8092..004B80A5
            else if ((f & 1) == 0) timer = nullptr;                   // 004B8077..004B807B -> 004B80A7 (unreachable form kept)
            else timer = kotor_fastcall_004b48e0(s, nullptr, id) == 1 ? ptr(s, 0x1004c) : ptr(s, 0x10048); // 004B807D..004B8090
        }
        unsigned day = 0, ms = 0;                                     // [esp+14h]/[esp+10h], written by the callee
        frame_GetWorldTime_004ade40(static_cast<menu_continuation::WorldTimer*>(timer), nullptr, &day, &ms); // 004B80B1..004B80BB
        void* object = nullptr;                                       // [esp+0Ch], written by 004D8230
        (void)kotor_fastcall_004d8230(ptr(s, 0x1005c), nullptr, id, &object); // 004B80C0..004B80CC
        if (auto* o = static_cast<kotor_window_pause::GameObject*>(object)) // 004B80D1..004B80D7
            o->table->s08(o, nullptr, day, ms);                       // 004B80D9..004B80E5 call [edx+8](day, ms)
        (void)unresolved_list_next_005e9b90(list_of(wrapper), nullptr, &pos); // 004B80E8..004B80EF
    }
}
// 0056C9F0 (RET 8, EAX=1): ECX unused; client/server read from 007A39FC.
extern "C" U32 __fastcall kotor_fastcall_0056c9f0(void*, void*, U32 type, U32 state) {
    void* const app = kotor_menu_window_cells::manager_007a39fc;      // 0056C9FE
    void* const client = ptr(app, 4);                                 // 0056CA11 ebx
    kotor_input_hover::ListWrapper* list = nullptr;                   // 0056CA16 edi=0
    void* const wrapper = kotor_fastcall_004ae9d0(ptr(app, 8), nullptr); // 0056CA0A..0056CA18
    if (wrapper && head_of(wrapper)) {                                // 0056CA1F..0056CA28
        list = construct_client_list();                               // 0056CA2A..0056CA49
        void* pos = head_of(wrapper);                                 // 0056CA4D..0056CA5B
        while (pos) {                                                 // 0056CA5F / 0056CA89..0056CA8B
            const std::uint32_t id = unresolved_list_at_pos_005e9b60(list_of(wrapper), nullptr, pos); // 0056CA61..0056CA64
            const std::uint32_t client_id = kotor_fastcall_005eda50(client, nullptr, id); // 0056CA69..0056CA6C
            (void)ini_section::AddTail005e9dc0(list->internal, nullptr,
                reinterpret_cast<void*>(static_cast<std::uintptr_t>(client_id))); // 0056CA71..0056CA74 (ecx=[edi])
            (void)unresolved_list_next_005e9b90(list_of(wrapper), nullptr, &pos); // 0056CA79..0056CA80
        }
    }
    kotor_fastcall_005ed650(client, nullptr, type, state, list);      // 0056CA8D..0056CA9A
    return 1;                                                         // 0056CAA5
}
