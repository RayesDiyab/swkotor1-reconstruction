// lane-m47-core: frontier names of lane-m46-startup's client_exo_app.cpp / pause_leaves.cpp:
//   005D4CB0 CNetLayer::SendMessageToPlayer (RET 10h)
//   005F7E20 CClientExoAppInternal::ClearPauseExclusionList, 005F7F10 ::AddToPauseExclusionList (RET 4),
//   + 005F56B0 ::IsOnExclusionList (RET 4), 005F7DC0 ::GetActiveTimer (RET 4).
// Link-required (pause / network paths; not on the main-menu first frame).
// Evidence: lane dis/005d4cb0.asm, 005f7e20.asm, 005f7f10.asm, 005f56b0.asm, 005f7dc0.asm; .rdata string 0074C550
// (tools/rd.py) reproduced as a literal of identical bytes.
// Game-object virtual slots (+08, +0C, +98h, +DCh, +E0h) have no published typed table in M46: D21 naked tail veneers in
// the style of lane-m46-startup pause_leaves.cpp (`mov eax,[ecx]; jmp [eax+N]`), declared with the slot's own stack words
// so the callee's RET N cleans exactly what the caller pushed.
// 005D4CB0 overflow path: CRT _isgraph 006FCA8F is the modern isgraph(int) (D35 runtime range); its only consumer here is
// a debug CExoString that is formatted and destroyed without being read, so no observable state depends on it.
#include "f9f7ab0530-startup_string.h"
#include <cctype>
#include <cstdint>
#include <cstring>

namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); }
namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction { __declspec(noinline) void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*); }
extern "C" {
std::uint32_t __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
std::uint32_t __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
void* __fastcall resource_append_005e9dc0(void*, void*, void*);                         // AddTail (resman-object-bodies)
unsigned char __fastcall kotor_fastcall_004d8230(void* array, void*, std::uint32_t id, void** out); // r4-lowers
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, unsigned* day, unsigned* ms);        // r4-lowers
}

namespace {
using U32 = std::uint32_t;
U32 w(const void* p, U32 off) { U32 v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
void* pw(const void* p, U32 off) { void* v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
unsigned char b8(const void* p, U32 off) { return static_cast<const unsigned char*>(p)[off]; }
const char k_overflow_format_0074c550[] =
    "CNetLayer::SendMessageToPlayer - Message Buffer Overflow! Message Header Follows:\n"
    "Size = %d! <%c %x, %c %x, %c %x, %c %x, %c %x, %c %x, %c %x, %c %x>\n";
// Game-object slots (D21 veneers).
__declspec(naked) void __fastcall object_slot_08(void*, void*, U32, U32) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax + 08h] } }
__declspec(naked) void* __fastcall object_slot_0c(void*, void*) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax + 0Ch] } }
__declspec(naked) void* __fastcall object_slot_98(void*, void*, U32, U32) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax + 98h] } }
__declspec(naked) void __fastcall object_slot_dc(void*, void*) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax + 0DCh] } }
__declspec(naked) void __fastcall object_slot_e0(void*, void*) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax + 0E0h] } }
void* list_internal(const void* self) { return pw(pw(self, 0x1D0), 0); }                  // [[this+1D0h]] (wrapper -> internal)
}

extern "C" {
// 005F56B0 IsOnExclusionList(id) RET 4: 1 when the list is absent or holds id.
__declspec(noinline) U32 __fastcall kotor_fastcall_005f56b0(void* self, void*, U32 id) {
    if (pw(self, 0x1D0) == nullptr) return 1;                                           // 005F56B4..C5
    void* position = pw(list_internal(self), 0);                                        // 005F56C8..CF
    while (position) {
        if (unresolved_list_at_pos_005e9b60(list_internal(self), nullptr, position) == id) return 1; // 005F56E0..F0
        (void)unresolved_list_next_005e9b90(list_internal(self), nullptr, &position);   // 005F56F2..0A
    }
    return 0;
}
// 005F7DC0 GetActiveTimer(id) RET 4: +28h/+2Ch for excluded objects per the +1CCh mode bits, else +24h (or 0).
__declspec(noinline) void* __fastcall kotor_fastcall_005f7dc0(void* self, void*, U32 id) {
    if (id == 0x7F000000u) return pw(self, 0x24);                                       // 005F7DC4..CD / 005F7E18
    const unsigned char mode = b8(self, 0x1CC);                                         // 005F7DCF
    if ((mode & 2u) == 0 && (mode & 1u) == 0) return pw(self, 0x24);                    // 005F7DD5..DE
    if ((mode & 2u) != 0)                                                               // 005F7DE0..E2
        return kotor_fastcall_005f56b0(self, nullptr, id) == 1 ? pw(self, 0x2C) : pw(self, 0x24); // 005F7DFE..0E
    if ((mode & 1u) == 0) return nullptr;                                               // 005F7DE4..E8 -> 005F7E12
    return kotor_fastcall_005f56b0(self, nullptr, id) == 1 ? pw(self, 0x28) : pw(self, 0x24);     // 005F7DEA..FB
}

// 005F7E20 ClearPauseExclusionList: drain the list; for each live object re-time it on its active timer and call
// [[obj+0C]()+98h](0FFh, 1) then [..+E0h]().
__declspec(noinline) void __fastcall client_internal_clear_pause_exclusion_list_005f7e20(void* self, void*) {
    for (;;) {
        void* const head = pw(list_internal(self), 0);                                  // 005F7E26..2E
        if (head == nullptr || w(head, 8) == 0) return;                                  // 005F7E30..3D
        const U32 id = static_cast<U32>(reinterpret_cast<std::uintptr_t>(
            base_ini_reconstruction::remove_head_005e9c80(static_cast<kotor_list::Internal*>(list_internal(self)), nullptr))); // 005F7E43..45
        void* object = nullptr;
        (void)kotor_fastcall_004d8230(pw(self, 0x14), nullptr, id, &object);            // 005F7E4A..53
        if (object == nullptr) continue;                                                 // 005F7E58..5E
        void* const timer = kotor_fastcall_005f7dc0(self, nullptr, w(object, 4));        // 005F7E60..AE (inlined 005F7DC0)
        unsigned day = 0, ms = 0;                                                        // D2: pure outputs of 004ADE40
        frame_GetWorldTime_004ade40(timer, nullptr, &day, &ms);                         // 005F7EB1..BB
        object_slot_08(object, nullptr, day, ms);                                       // 005F7EC0..D0
        void* const aur = object_slot_0c(object, nullptr);                              // 005F7ED3..D9
        void* const part = object_slot_98(aur, nullptr, 0xFFu, 1u);                     // 005F7EDC..E7
        object_slot_e0(part, nullptr);                                                  // 005F7EED..F1
    }
}

// 005F7F10 AddToPauseExclusionList(id) RET 4.
__declspec(noinline) void __fastcall client_internal_add_to_exclusion_list_005f7f10(void* self, void*, std::uint32_t id) {
    void* object = nullptr;                                                             // [esp+14h] (argument slot reused)
    (void)kotor_fastcall_004d8230(pw(self, 0x14), nullptr, id, &object);                // 005F7F19..24
    if (object == nullptr) return;                                                      // 005F7F29..2F
    void* const aur = object_slot_0c(object, nullptr);                                  // 005F7F31..33
    void* const part = object_slot_98(aur, nullptr, 0xFFu, 1u);                         // 005F7F36..41
    object_slot_dc(part, nullptr);                                                      // 005F7F47..4B
    (void)resource_append_005e9dc0(list_internal(self), nullptr,
                                   reinterpret_cast<void*>(static_cast<std::uintptr_t>(id))); // 005F7F51..5A AddTail(id)
    void* const timer = kotor_fastcall_005f7dc0(self, nullptr, w(object, 4));            // 005F7F5F..73
    unsigned day = 0, ms = 0;                                                           // D2: pure outputs of 004ADE40
    frame_GetWorldTime_004ade40(timer, nullptr, &day, &ms);                            // 005F7F78..7A
    object_slot_08(object, nullptr, day, ms);                                           // 005F7F7F..8F
}

// 005D4CB0 CNetLayer::SendMessageToPlayer(player, message, length, flags) RET 10h.
__declspec(noinline) std::int32_t __fastcall net_layer_send_message_to_player_005d4cb0(void* self, void*, std::uint32_t,
        const char* message, std::uint32_t length, std::uint32_t) {
    unsigned char* const buffer = static_cast<unsigned char*>(pw(self, 0x2000C));      // 005D4CCB
    if (buffer == nullptr) return 0;                                                    // 005D4CD4..EE
    U32 used = w(buffer, 0x20004);                                                      // 005D4CF1
    if (used >= 0x10000u) used = 0;                                                     // 005D4CF7..07 (unsigned JB)
    const U32 end = used - (length & 3u) + length + 8u;                                 // 005D4D11..1E
    if (end > w(buffer, 0x20000) + 0x10000u) {                                          // 005D4D09..2D (unsigned JBE)
        kotor_reconstruction::StartupString text;                                      // 005D4D33..37 (state 0)
        const unsigned char* const m = reinterpret_cast<const unsigned char*>(message);
        U32 raw[8]; int shown[8];
        for (int i = 7; i >= 0; --i) {                                                  // 005D4D3C..EDD (index 7 down to 0)
            const bool present = length >= static_cast<U32>(i + 1);                     // cmp edi,i+1; jb
            raw[i] = present ? m[i] : 0u;
            shown[i] = (present && std::isgraph(m[i])) ? m[i] : 0x5F;                   // 006FCA8F; '_' otherwise
        }
        kotor_reconstruction::StartupFormat005e5680(&text, k_overflow_format_0074c550, length,  // 005D4EDF..39
            shown[0], raw[0], shown[1], raw[1], shown[2], raw[2], shown[3], raw[3],
            shown[4], raw[4], shown[5], raw[5], shown[6], raw[6], shown[7], raw[7]);
    }                                                                                   // 005D4F41..4D ~text
    std::memcpy(buffer + used, &length, 4);                                             // 005D4F5E..68
    std::memcpy(buffer + used + 4, message, length);                                    // 005D4F6B..83 (rep movsd/movsb)
    std::memcpy(buffer + 0x20004, &end, 4);                                             // 005D4F85..90
    return 1;
}
}
