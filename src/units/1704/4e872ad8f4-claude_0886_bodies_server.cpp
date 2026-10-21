// 0886-gates (Claude): faithful bodies for former required(0x...) gates of the 0886 cohort unit on the server main
// loop ports (server_mainloop_native_vm_canonical_cell.cpp NativePorts / NativeClock).
// Transcribed from the original swkotor.exe (read-only); addresses in comments are original instructions.
// ABI: original __thiscall members are exported as extern "C" __fastcall(self, unused_edx, stack args...):
//   004AEBC0 CServerExoApp::SetMoveToWorldTime(...)         -> [ecx+4]; jmp 004B22D0            RET 1Ch
//   004B22D0 CServerExoAppInternal::SetMoveToWorldTime(ulong day, uchar, uchar, uchar, ushort, ushort, ushort) RET 1Ch
//   005EDC80 CClientExoApp::SetCanSendDialog(int)            -> [ecx+4]; jmp 005F3030            RET 4
//   (005F3030 CClientExoAppInternal::SetCanSendDialog(int) RET 4 is the existing client_update body)
//   004B2520 CServerExoAppInternal::HasEnoughDiskSpaceForSaveGame()                                 RET 0
//   0056B2B0 CSWSMessage::SendServerToPlayerShutDownServer(ulong player, ulong reason)              RET 8
//   0056C7F0 CSWSMessage::SendServerToPlayerLogin_NeedCharacter(ulong player)                       RET 4
// .rdata bound to proven values: [00745174] = "SAVES:" (C string literal).
// EH: 004B2520 registers an unwind frame for its CExoString local only; the bound callees do not throw in normal
// operation, so that cleanup is not reproduced (the normal path destroys it).
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(a); }
inline U word(const void* p) { return reinterpret_cast<U>(p); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
inline void wr8(U a, U v) { *static_cast<unsigned char*>(ptr(a)) = static_cast<unsigned char>(v); }
inline void wr16(U a, U v) { const std::uint16_t x = static_cast<std::uint16_t>(v); std::memcpy(ptr(a), &x, 2); }
}

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }    // [007A39FC] CAppManager*
namespace frame_control { extern unsigned resource_manager_007a39e8; }           // [007A39E8] CExoResMan*

extern "C" {
// ---------------- existing providers (c14 map, exact decorated arity) ----------------
void* __cdecl live_operator_new(unsigned);                                       // 006FA7E6
void __cdecl live_free(void*);                                                   // 006FA390
void* __fastcall frame_LogStringCtor_005e5a90(void* string, void*, const char* text); // 005E5A90 CExoString(char const*) (0459)
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);              // 005E5C20 ~CExoString (0459)
U __fastcall gui_load_resman_free_disk_space_004067b0(void* manager, void*, void* path, std::uint64_t* out); // 004067B0 (0296)
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U a, U b); // 004D68A0 CreateWriteMessage (w2_message_native)
void __fastcall kotor_fastcall_004d6340(void* message, void*, U value, U bits);  // 004D6340 WriteDWORD
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size); // 004D6440 GetWriteMessage
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // 00567E60
int __fastcall net_layer_send_message_to_player_005d4cb0(void* net, void*, U player, const char* data, U size, U flags); // 005D4CB0 (0599)

// ---------------- bodies defined here ----------------
void __fastcall kotor_fastcall_004b22d0(void* internal, void*, U day, U a, U b, U c, U d, U e, U f);
void __fastcall kotor_fastcall_004aebc0(void* server, void*, U day, U a, U b, U c, U d, U e, U f);
void __fastcall kotor_fastcall_005f3030(void* internal, void*, U value);              // 005F3030 CClientExoAppInternal::SetCanSendDialog (client_update)
void __fastcall kotor_fastcall_005edc80(void* client, void*, U value);
U __fastcall kotor_fastcall_004b2520(void* internal, void*);
U __fastcall kotor_fastcall_0056b2b0(void* message, void*, U player, U reason);
U __fastcall kotor_fastcall_0056c7f0(void* message, void*, U player);
}

// ===================================================================================== 004B22D0 / 004AEBC0
// SetMoveToWorldTime: +1009C day, +100A0..A2 three bytes, +100A4..A8 three words.
void __fastcall kotor_fastcall_004b22d0(void* internal, void*, U day, U a, U b, U c, U d, U e, U f) {
    const U s = word(internal);
    wr(s + 0x1009c, day);                                                        // 004B22D0..004B22D8
    wr8(s + 0x100a0, a);                                                         // 004B22D4..004B22E2
    wr8(s + 0x100a1, b);                                                         // 004B22DE..004B22EC
    wr8(s + 0x100a2, c);                                                         // 004B22E8..004B22F7
    wr16(s + 0x100a4, d);                                                        // 004B22F2..004B2302
    wr16(s + 0x100a6, e);                                                        // 004B22FD..004B230E
    wr16(s + 0x100a8, f);                                                        // 004B2309..004B2315
}
void __fastcall kotor_fastcall_004aebc0(void* server, void*, U day, U a, U b, U c, U d, U e, U f) {
    kotor_fastcall_004b22d0(ptr(rd(word(server) + 4)), nullptr, day, a, b, c, d, e, f); // 004AEBC0..004AEBC3
}

// ===================================================================================== 005EDC80
// CClientExoApp::SetCanSendDialog: [ecx+4]; jmp 005F3030 (existing body @kotor_fastcall_005f3030@12, client_update).
void __fastcall kotor_fastcall_005edc80(void* client, void*, U value) {
    kotor_fastcall_005f3030(ptr(rd(word(client) + 4)), nullptr, value);          // 005EDC80..005EDC83
}

// ===================================================================================== 004B2520
// HasEnoughDiskSpaceForSaveGame: 1 when (free bytes of "SAVES:" >> 14) > 1600 (signed 64-bit compare).
U __fastcall kotor_fastcall_004b2520(void*, void*) {
    std::uint64_t free_bytes = 0;                                                // 004B2541..004B2549
    unsigned char path[8];                                                       // CExoString
    frame_LogStringCtor_005e5a90(path, nullptr, "SAVES:");                       // 004B2538..004B2551 ([00745174])
    (void)gui_load_resman_free_disk_space_004067b0(ptr(frame_control::resource_manager_007a39e8), nullptr, path,
                                                   &free_bytes);                 // 004B2556..004B256E
    frame_LogStringDtor_005e5c20(path, nullptr);                                 // 004B2573..004B257F
    const long long blocks = static_cast<long long>(free_bytes >> 14);           // 004B2584..004B2590 (shrd/shr)
    return blocks > 0x640 ? 1u : 0u;                                             // 004B2593..004B25C5
}

// ===================================================================================== 0056B2B0
// SendServerToPlayerShutDownServer: message 2Fh/0 carrying one DWORD; 1 when sent, 0 when GetWriteMessage fails.
U __fastcall kotor_fastcall_0056b2b0(void* message, void*, U player, U reason) {
    kotor_fastcall_004d68a0(message, nullptr, 4, 0xffffffffu, 1);                // 0056B2B4..0056B2BC
    kotor_fastcall_004d6340(message, nullptr, reason, 0x20);                     // 0056B2C1..0056B2CA
    void* data = nullptr;                                                        // [esp+8]
    U size = 0;                                                                  // [esp+4]
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;  // 0056B2CF..0056B2E8
    (void)kotor_fastcall_00567e60(message, nullptr, player, 0x2f, 0, data, size); // 0056B2EB..0056B300
    return 1;                                                                    // 0056B305..0056B30E
}

// ===================================================================================== 0056C7F0
// SendServerToPlayerLogin_NeedCharacter: +4C = -1; 3-byte packet {50h, 02h, 10h} through the server's net layer
// (CServerExoApp vtbl +10h). Returns the send result, 0 when the packet allocation fails.
U __fastcall kotor_fastcall_0056c7f0(void* message, void*, U player) {
    unsigned char* const packet = static_cast<unsigned char*>(live_operator_new(3)); // 0056C7F2..0056C7FB
    wr(word(message) + 0x4c, 0xffffffffu);                                       // 0056C802
    if (packet == nullptr) return 0;                                             // 0056C800..0056C80D (EAX = 0)
    packet[0] = 0x50;                                                            // 0056C816
    packet[1] = 2;                                                               // 0056C819
    packet[2] = 0x10;                                                            // 0056C81D
    void* const server = *reinterpret_cast<void* const*>(
        static_cast<unsigned char*>(kotor_menu_window_cells::manager_007a39fc) + 8); // 0056C821..0056C826
    using GetNetLayer = void*(__thiscall*)(void*);
    void* const net = reinterpret_cast<GetNetLayer>(rd(rd(word(server)) + 0x10))(server); // 0056C829..0056C82F
    const U result = static_cast<U>(net_layer_send_message_to_player_005d4cb0(net, nullptr, player,
        reinterpret_cast<const char*>(packet), 3, 0));                           // 0056C810..0056C834
    live_free(packet);                                                           // 0056C839..0056C83C
    return result;                                                               // 0056C844..0056C848
}

// =====================================================================================================
// Second group (module load / client system / debug console ports):
//   004C83B0 CSWSModule::AddObjectToLimbo(ulong id)                                         RET 4
//   0056CB80 CSWSMessage::SendServerToPlayerSaveLoad_Status(CSWSPlayer*, uchar, ulong)     RET 0Ch
//   0054E9D0 CServerInfo::FindOptionIndex(CExoString*, CExoString*)                        RET 8
//   00675690 CClientMsgSysAdmin::SendSysAdminToServerMessage(CExoString*)                  RET 4
//   005EE4A0 SetFramesPerSecond(char const*) (cdecl)                                       RET 0
// .rdata: [0074C528] = 0FFFFFFFDh (server player id), [0074E71C] = "Frame rate set.",
//   [0074E704] = "Fixed frame rate off." (returned C string literals).
namespace kotor_menu20 { int __cdecl atoi_006fafe7(const char* text); }          // 006FAFE7 _atol (CRT owner)
extern "C" {
void __fastcall kotor_fastcall_004d62c0(void* message, void*, U value, U bits);  // 004D62C0 WriteBYTE (w2_message_native)
void* __fastcall async_string_cstr_005e5670(void* string, void*);                // 005E5670 CExoString::CStr (0188)
int __fastcall async_string_length_005e5790(void* string, void*);               // 005E5790 CExoString::GetLength (0188)
void* __fastcall frame_GetWorldTimer_005ed4f0(void* client, void*);             // 005ED4F0 CClientExoApp::GetWorldTimer
void* __fastcall kotor_fastcall_005ed500(void* client, void*);                  // 005ED500 CClientExoApp::GetPauseTimer
void* __fastcall kotor_fastcall_005ed510(void* client, void*);                  // 005ED510 CClientExoApp::GetAnimationTimer
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                  // 004AEDE0 CServerExoApp::GetWorldTimer
void* __fastcall kotor_fastcall_004aee00(void* server, void*);                  // 004AEE00 CServerExoApp::GetPauseTimer
void __fastcall kotor_fastcall_004ae170(void* timer, void*, U fixed, U rate);    // 004AE170 CWorldTimer::SetFixedSnapshotRate

void __fastcall kotor_fastcall_004c83b0(void* module, void*, U id);
U __fastcall kotor_fastcall_0056cb80(void* message, void*, void* player, U status, U value);
U __fastcall kotor_fastcall_0054e9d0(void* info, void*, void* name, void* value);
U __fastcall kotor_fastcall_00675690(void* sysadmin, void*, void* text);
const char* __cdecl kotor_cdecl_005ee4a0(const char* text);
}

// ===================================================================================== 004C83B0
// AddObjectToLimbo: unique append of the id to the limbo list (+1DC data, +1E0 size, +1E4 capacity; grows to
// capacity*2, 16 when empty; old block copied and freed).
void __fastcall kotor_fastcall_004c83b0(void* module, void*, U id) {
    const U s = word(module);
    const int size = static_cast<int>(rd(s + 0x1e0));                            // 004C83B8 (cached for the search)
    for (int i = 0; i < size; ++i)                                               // 004C83BE..004C83DE
        if (rd(rd(s + 0x1dc) + 4u * static_cast<U>(i)) == id) return;            // 004C83D0..004C83D2 -> 004C8469
    if (rd(s + 0x1e0) == rd(s + 0x1e4)) {                                        // 004C83E0..004C83EC
        const U capacity = rd(s + 0x1e4) != 0 ? rd(s + 0x1e4) * 2 : 0x10;        // 004C83EE..004C83F6
        const U old_data = rd(s + 0x1dc);                                        // 004C83FB
        wr(s + 0x1e4, capacity);                                                 // 004C8401
        wr(s + 0x1dc, word(live_operator_new(capacity * 4)));                    // 004C8407..004C8416
        for (int i = 0; i < static_cast<int>(rd(s + 0x1e0)); ++i)                // 004C841F..004C8445 (size re-read)
            wr(rd(s + 0x1dc) + 4u * static_cast<U>(i), rd(old_data + 4u * static_cast<U>(i))); // 004C8430..004C8439
        if (old_data != 0) live_free(ptr(old_data));                             // 004C8447..004C844C
    }
    wr(rd(s + 0x1dc) + 4u * rd(s + 0x1e0), id);                                  // 004C8454..004C8460
    wr(s + 0x1e0, rd(s + 0x1e0) + 1);                                            // 004C8463
}

// ===================================================================================== 0056CB80
// SendServerToPlayerSaveLoad_Status: message 2Dh/1 {BYTE status (8 bits), DWORD value (4 bits)} to player +4.
// Quirk kept: the size out-parameter is the player argument slot (player stays in EDI).
U __fastcall kotor_fastcall_0056cb80(void* message, void*, void* player, U status, U value) {
    if (player == nullptr) return 0;                                             // 0056CB83..0056CB8B -> 0056CBC9
    kotor_fastcall_004d68a0(message, nullptr, 4, 0xffffffffu, 1);                // 0056CB8D..0056CB93
    kotor_fastcall_004d62c0(message, nullptr, status, 8);                        // 0056CB98..0056CBA1
    kotor_fastcall_004d6340(message, nullptr, value, 4);                         // 0056CBA6..0056CBAF
    void* data = nullptr;                                                        // [esp+8]
    U size = 0;                                                                  // [esp+10h] (player argument slot)
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;  // 0056CBB4..0056CBCE
    return kotor_fastcall_00567e60(message, nullptr, rd(word(player) + 4), 0x2d, 1, data, size); // 0056CBD1..0056CBE5
}

// ===================================================================================== 0054E9D0
// FindOptionIndex: first index 1..49 whose option name (+10Ch+10h*i) and value (+110h+10h*i) C strings equal the
// two CExoStrings (inline strcmp); 0 when none.
U __fastcall kotor_fastcall_0054e9d0(void* info, void*, void* name, void* value) {
    U option = word(info) + 0x120;                                               // 0054E9DD edi
    for (U index = 1; index < 0x32; ++index, option += 0x10) {                   // 0054E9D8, 0054EA53..0054EA5A
        if (std::strcmp(static_cast<const char*>(async_string_cstr_005e5670(name, nullptr)),
                        reinterpret_cast<const char*>(rd(option - 4))) != 0) continue;   // 0054E9E3..0054EA1B
        if (std::strcmp(static_cast<const char*>(async_string_cstr_005e5670(value, nullptr)),
                        reinterpret_cast<const char*>(rd(option))) == 0) return index;   // 0054EA1D..0054EA51, 0054EA65
    }
    return 0;                                                                    // 0054EA5C..0054EA62
}

// ===================================================================================== 00675690
// SendSysAdminToServerMessage: packet 's' + text (length+1 bytes) to player [0074C528] through the client's net
// layer (CClientExoApp vtbl +10h). Quirk kept: the packet is never freed (original leak).
U __fastcall kotor_fastcall_00675690(void*, void*, void* text) {
    const int length = async_string_length_005e5790(text, nullptr) + 1;         // 00675697..006756A0
    unsigned char* const packet = static_cast<unsigned char*>(live_operator_new(static_cast<U>(length))); // 006756A1..006756A7
    if (packet == nullptr) return 0;                                             // 006756AC..006756B3 (EAX = 0)
    packet[0] = 0x73;                                                            // 006756B9
    const int count = async_string_length_005e5790(text, nullptr);              // 006756BC..006756C3
    std::memcpy(packet + 1, async_string_cstr_005e5670(text, nullptr), static_cast<std::size_t>(count)); // 006756C7..006756E1
    void* const client = *reinterpret_cast<void* const*>(
        static_cast<unsigned char*>(kotor_menu_window_cells::manager_007a39fc) + 4); // 006756E3..006756E9
    using GetNetLayer = void*(__thiscall*)(void*);
    void* const net = reinterpret_cast<GetNetLayer>(rd(rd(word(client)) + 0x10))(client); // 006756F2..006756F9
    return static_cast<U>(net_layer_send_message_to_player_005d4cb0(net, nullptr, 0xfffffffdu,
        reinterpret_cast<const char*>(packet), static_cast<U>(length), 0));      // 006756EC..006756FE ([0074C528])
}

// ===================================================================================== 005EE4A0
// SetFramesPerSecond: fixed snapshot rate n (atol) on the client world/pause/animation and server world/pause
// timers, or fixed rate off when n == 0; returns the status text.
const char* __cdecl kotor_cdecl_005ee4a0(const char* text) {
    const int rate = kotor_menu20::atoi_006fafe7(text);                          // 005EE4A0..005EE4AD
    void* const app = kotor_menu_window_cells::manager_007a39fc;
    auto client = [&]() { return *reinterpret_cast<void* const*>(static_cast<unsigned char*>(app) + 4); };
    auto server = [&]() { return *reinterpret_cast<void* const*>(static_cast<unsigned char*>(app) + 8); };
    const U fixed = rate != 0 ? 1u : 0u;                                         // push 1 / push 0
    const U value = rate != 0 ? static_cast<U>(rate) : 0u;                       // push esi / push 0
    kotor_fastcall_004ae170(frame_GetWorldTimer_005ed4f0(client(), nullptr), nullptr, fixed, value); // 005EE4B4..005EE4C7 / 005EE532..005EE545
    kotor_fastcall_004ae170(kotor_fastcall_005ed500(client(), nullptr), nullptr, fixed, value);      // 005EE4CC..005EE4DF / 005EE54A..005EE55E
    kotor_fastcall_004ae170(kotor_fastcall_005ed510(client(), nullptr), nullptr, fixed, value);      // 005EE4E4..005EE4F6 / 005EE563..005EE577
    kotor_fastcall_004ae170(kotor_fastcall_004aede0(server(), nullptr), nullptr, fixed, value);      // 005EE4FB..005EE50E / 005EE57C..005EE58F
    kotor_fastcall_004ae170(kotor_fastcall_004aee00(server(), nullptr), nullptr, fixed, value);      // 005EE513..005EE526 / 005EE594..005EE5A8
    return rate != 0 ? "Frame rate set." : "Fixed frame rate off.";              // 005EE52B / 005EE5AD ([0074E71C] / [0074E704])
}
