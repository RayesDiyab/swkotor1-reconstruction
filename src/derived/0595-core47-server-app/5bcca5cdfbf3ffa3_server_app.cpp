// Recovery: nine missing list-wrapper dereferences restored from original instructions.
// lane-m47-core: CServerExoApp wrappers and their CServerExoAppInternal bodies used by the R4 lowers
// (r4-lowers r4_frame_lowers.cpp) and the main-menu event handlers (gui-load mainmenu.cpp):
//   004AE910 -> 004B2A50 GetClientObjectByObjectId, 004AE960 -> 004BA610 TogglePauseState,
//   004AEA30 -> 004B1F70 ClientToServerObjectId, 004AEA40 -> 004B4CB0 GetPlayerCreatureId,
//   004AE920 -> 004B2AE0 GetClientObjectByPlayerId, 004AE770 -> 004B1790 GetCreatureByGameObjectID,
//   004B2820 GetPlayerCreature, 00561080 CSWSPlayer::GetGameObject.
// Link-required (not reached on the main-menu first frame: no player exists; R4 no-player path, D15).
// Evidence: lane dis/<addr>.asm; .rdata 00746633 = 04h, 00746634 = 05h, 0074666C = 00h, 0074665C = 80000000h.
// ABI: original __thiscall in the D1 form; the wrappers are `mov ecx,[ecx+4]; jmp internal` (004AEA40's target
// 004B4CB0 does not read ECX).
#include "4fc8ac33e31b3130_server_lookup_support.h"


using namespace r4_lowers;

namespace base_ini_reconstruction {}
extern "C" {
std::uint32_t __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
std::uint32_t __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
void __fastcall kotor_fastcall_004b8110(void* internal, void*, U32 type, U32 state);   // aurora-window server_pause.cpp
}

namespace {
constexpr unsigned char k_player_type_limit_00746633 = 0x04u;   // .rdata byte 00746633
constexpr unsigned char k_creature_type_00746634 = 0x05u;       // .rdata byte 00746634
constexpr unsigned char k_object_found_0074666c = 0x00u;        // .rdata byte 0074666C
constexpr U32 k_client_bit_0074665c = 0x80000000u;              // .rdata dword 0074665C
constexpr U32 k_invalid_object = 0x7F000000u;
void* list_at(void* internal, void* pos) { return at(unresolved_list_at_pos_005e9b60(internal, nullptr, pos)); }
void list_next(void* internal, void** pos) { (void)unresolved_list_next_005e9b90(internal, nullptr, pos); }
}

extern "C" {
void* __fastcall kotor_fastcall_00561080(void* player, void*);
void* __fastcall kotor_fastcall_004b2a50(void* internal, void*, U32 id);
void* __fastcall kotor_fastcall_004b2ae0(void* internal, void*, U32 id, U32 flags);
void* __fastcall kotor_fastcall_004ae920(void* server, void*, U32 id, U32 flags);
std::uint32_t __fastcall kotor_fastcall_004b4cb0(void* ignored, void*);
void* __fastcall kotor_fastcall_004b1790(void* internal, void*, U32 id);
void* __fastcall kotor_fastcall_004ae770(void* server, void*, U32 id);

// 00561080 CSWSPlayer::GetGameObject: server->GetGameObject([this+28h]) when its type byte > [00746633].
__declspec(noinline) void* __fastcall kotor_fastcall_00561080(void* player, void*) {
    void* const server = pw(kotor_menu_window_cells::manager_007a39fc, 8);       // 00561082..8B
    void* const object = kotor_fastcall_004ae750(server, nullptr, w(player, 0x28)); // 00561088..8F
    if (!object) return nullptr;                                                 // 00561094..96
    if (b8(object, 8) > k_player_type_limit_00746633) return object;             // 00561098..A1 (JA)
    return nullptr;                                                              // 005610A3
}
// 004B2A50 CServerExoAppInternal::GetClientObjectByObjectId(id) RET 4: player list [this+10064h].
__declspec(noinline) void* __fastcall kotor_fastcall_004b2a50(void* internal, void*, U32 id) {
    if (id == k_invalid_object) return nullptr;                                  // 004B2A55..64
    void* pos = pw(pw(pw(internal, 0x10064), 0), 0);                                    // 004B2A67..74 (wrapper -> internal -> head)
    while (pos) {
        void* const player = list_at(pw(pw(internal, 0x10064), 0), pos);                // 004B2A80..8E
        if (player && kotor_fastcall_00561080(player, nullptr) &&               // 004B2A90..9D
            w(kotor_fastcall_00561080(player, nullptr), 4) == id) return player; // 004B2A9F..A9
        list_next(pw(pw(internal, 0x10064), 0), &pos);                                  // 004B2AAB..C3
    }
    return nullptr;
}
// 004B2AE0 CServerExoAppInternal::GetClientObjectByPlayerId(id, byte which) RET 8: list +10064h when which is 0/1,
// then list +10068h when which is 0/2; compares [entry+4] (entry not null-checked).
__declspec(noinline) void* __fastcall kotor_fastcall_004b2ae0(void* internal, void*, U32 id, U32 flags) {
    const unsigned char which = static_cast<unsigned char>(flags);               // 004B2AE1 (BL)
    if (which == 1 || which == 0) {                                              // 004B2AE5..F4
        void* pos = pw(pw(pw(internal, 0x10064), 0), 0);                                // 004B2AF6..06
        while (pos) {
            void* const entry = list_at(pw(pw(internal, 0x10064), 0), pos);             // 004B2B10..19
            if (w(entry, 4) == id) return entry;                                 // 004B2B1E..21
            list_next(pw(pw(internal, 0x10064), 0), &pos);                              // 004B2B23..3B
        }
    }
    if (which == 2 || which == 0) {                                              // 004B2B3D..44
        void* pos = pw(pw(pw(internal, 0x10068), 0), 0);                                // 004B2B46..56
        while (pos) {
            void* const entry = list_at(pw(pw(internal, 0x10068), 0), pos);             // 004B2B60..69
            if (w(entry, 4) == id) return entry;                                 // 004B2B6E..71
            list_next(pw(pw(internal, 0x10068), 0), &pos);                              // 004B2B73..8B
        }
    }
    return nullptr;                                                              // 004B2B8D
}
// 004AE920: mov ecx,[ecx+4]; jmp 004B2AE0.
__declspec(noinline) void* __fastcall kotor_fastcall_004ae920(void* server, void*, U32 id, U32 flags) {
    return kotor_fastcall_004b2ae0(pw(server, 4), nullptr, id, flags);
}
// 004B4CB0 GetPlayerCreatureId (reached by JMP from 004AEA40; ECX unused).
__declspec(noinline) std::uint32_t __fastcall kotor_fastcall_004b4cb0(void*, void*) {
    void* const server = pw(kotor_menu_window_cells::manager_007a39fc, 8);       // 004B4CB0..B5
    void* const player = kotor_fastcall_004ae920(server, nullptr, 0u, 1u);       // 004B4CB8..BD (push 1; push 0)
    if (player && kotor_fastcall_00561080(player, nullptr))                      // 004B4CC4..D1
        return w(kotor_fastcall_00561080(player, nullptr), 4);                   // 004B4CD3..DA
    return k_invalid_object;                                                     // 004B4CDF
}
// 004B1790 CServerExoAppInternal::GetCreatureByGameObjectID(id) RET 4.
__declspec(noinline) void* __fastcall kotor_fastcall_004b1790(void* internal, void*, U32 id) {
    void* object = nullptr;                                                      // 004B17A1 (stored before the call)
    const unsigned char status = kotor_fastcall_004d8230(pw(internal, 0x1005c), nullptr, id, &object); // 004B1791..A9
    if (status != k_object_found_0074666c) return nullptr;                       // 004B17AE..B4
    if (!object) return nullptr;                                                 // 004B17B6..BC
    if (b8(object, 8) != k_creature_type_00746634) return nullptr;               // 004B17BE..C7
    return at(vcall0(object, 0x30));                                             // 004B17C9..CB [vtbl+30h]
}
// 004AE770: mov ecx,[ecx+4]; jmp 004B1790.
__declspec(noinline) void* __fastcall kotor_fastcall_004ae770(void* server, void*, U32 id) {
    return kotor_fastcall_004b1790(pw(server, 4), nullptr, id);
}
// 004AE910: mov ecx,[ecx+4]; jmp 004B2A50.
__declspec(noinline) void* __fastcall kotor_fastcall_004ae910(void* server, void*, std::uint32_t id) {
    return kotor_fastcall_004b2a50(pw(server, 4), nullptr, id);
}
// 004AEA40: mov ecx,[ecx+4]; jmp 004B4CB0.
__declspec(noinline) std::uint32_t __fastcall kotor_fastcall_004aea40(void* server, void*) {
    return kotor_fastcall_004b4cb0(pw(server, 4), nullptr);
}
// 004B2820 CServerExoApp::GetPlayerCreature: GetCreatureByGameObjectID(GetPlayerCreatureId()).
__declspec(noinline) void* __fastcall kotor_fastcall_004b2820(void* server, void*) {
    const U32 id = kotor_fastcall_004aea40(server, nullptr);                     // 004B2821..23
    return kotor_fastcall_004ae770(server, nullptr, id);                         // 004B2828..2B
}
// 004B1F70 CServerExoAppInternal::ClientToServerObjectId(id) RET 4 / 004AEA30 wrapper.
__declspec(noinline) std::uint32_t __fastcall kotor_fastcall_004b1f70(void*, void*, std::uint32_t id) {
    if (id == k_invalid_object) return k_invalid_object;                         // 004B1F74..7A / 004B1F88
    return ~k_client_bit_0074665c & id;                                          // 004B1F7C..83 (not; and)
}
__declspec(noinline) std::uint32_t __fastcall kotor_fastcall_004aea30(void* server, void*, std::uint32_t id) {
    return kotor_fastcall_004b1f70(pw(server, 4), nullptr, id);                  // 004AEA30..33
}
// 004BA610 CServerExoAppInternal::TogglePauseState(byte type) RET 4: SetPauseState(type, (pause & type) == 0).
__declspec(noinline) void __fastcall kotor_fastcall_004ba610(void* internal, void*, std::uint32_t type) {
    const unsigned char current = static_cast<unsigned char>(b8(internal, 0x10078) & static_cast<unsigned char>(type)); // 004BA614..1C
    const U32 state = current == 0 ? 1u : 0u;                                    // 004BA61F..25 (cmp/sbb/inc)
    kotor_fastcall_004b8110(internal, nullptr, type, state);                     // 004BA626..28 (push edx; push eax)
}
// 004AE960: mov ecx,[ecx+4]; jmp 004BA610.
__declspec(noinline) void __fastcall kotor_fastcall_004ae960(void* server, void*, std::uint32_t type) {
    kotor_fastcall_004ba610(pw(server, 4), nullptr, type);
}
}

