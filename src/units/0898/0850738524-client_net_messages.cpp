// lane-m48-b fork CLIENT, unit client-net-messages: CNetLayer::NonWindowMessages 005D5690 (r4-lowers consumer at
// 005D5834, CNetLayer::ProcessReceivedFrames) with HandleBNCSMessage 005D5180 and HandleBNCRMessage 005D53F0.
// Evidence: dumpbin /disasm swkotor.exe 005D5690..005D5713, 005D5180..005D539E (full contiguous body; Ghidra ranges
// omit 005D5367..005D53A1), 005D53A1..005D53E3, 005D53F0..005D5430. .rdata 0073D71C "".
// CExoString locals use the canonical kotor_reconstruction::StartupString lifetime (ctor 005B3190 / dtor 005E5C20), so the
// original EH states 0..2 (005D51A8..005D5268) are emitted as C++ destructors.
#include "84a5500766-startup_string.h"
#include <cstdint>
#include <cstring>

using kotor_reconstruction::StartupString;
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
StartupString* __fastcall resman_string_counted_005e5b70(StartupString*, void*, const char*, int);   // CExoString(const char*, int)
std::int32_t __fastcall net_layer_send_message_to_player_005d4cb0(void* self, void*, std::uint32_t, const char* message,
                                                                  std::uint32_t length, std::uint32_t);  // core47 net_pause_leaves
std::uint32_t __fastcall kotor_fastcall_005d5180(void* net, void*, std::uint32_t player, const char* message, std::uint32_t length);
std::uint32_t __fastcall kotor_fastcall_005d53f0(void* net, void*, std::uint32_t player, const char* message, std::uint32_t length);
void __fastcall kotor_fastcall_005d5690(void* net, void*, std::uint32_t player, const char* message, std::uint32_t length);
}

namespace {
using U32 = std::uint32_t;
U32 w(const void* p, U32 o) { U32 v; std::memcpy(&v, static_cast<const unsigned char*>(p) + o, 4); return v; }
void sw(void* p, U32 o, U32 v) { std::memcpy(static_cast<unsigned char*>(p) + o, &v, 4); }
void* pw(const void* p, U32 o) { void* v; std::memcpy(&v, static_cast<const unsigned char*>(p) + o, 4); return v; }
StartupString* str(void* p, U32 o) { return reinterpret_cast<StartupString*>(static_cast<unsigned char*>(p) + o); }
const char k_empty_0073d71c[] = "";
// Session object [net+20008h] slots, called through its own table word (r4_lowers vcall precedent).
using SessionSlot0 = U32(__thiscall*)(void*, U32, U32, U32);
using SessionSlot4 = U32(__thiscall*)(void*, U32, const char*, U32, U32);
}

// 005D5180 CNetLayer::HandleBNCSMessage(player, message, length), RET 0Ch, EAX 1.
extern "C" U32 __fastcall kotor_fastcall_005d5180(void* net, void*, U32, const char* message, U32 length) {
    StartupString name;                                                 // [esp+4]  005D519B..005D519F (EH state 0)
    StartupString unused;                                               // [esp+0C] 005D51A4..005D51B0 (EH state 1)
    if (length < 9) return 1;                                           // 005D51B5..005D51C1 / 005D51C3..005D51E2
    const U32 name_length = static_cast<unsigned char>(message[8]);     // 005D51FD
    const unsigned char kind = static_cast<unsigned char>(message[7]);  // 005D5202
    if (length != name_length + 9) return 1;                            // 005D5207..005D520C / 005D520E..005D522F
    alignas(4) unsigned char counted[8];                                // [esp+14h] temporary (EH state 2)
    resman_string_counted_005e5b70(reinterpret_cast<StartupString*>(counted), nullptr, message + 9, static_cast<int>(name_length)); // 005D5246..005D5250
    name.Assign005e5c50(reinterpret_cast<StartupString*>(counted));     // 005D5255..005D525F
    reinterpret_cast<StartupString*>(counted)->~StartupString();       // 005D5264..005D526D
    unsigned char* reply = static_cast<unsigned char*>(live_operator_new(0x0a)); // 005D5272..005D5279
    reply[0] = 'B'; reply[1] = 'N'; reply[2] = 'C'; reply[3] = 'R';     // 005D5289..005D5294
    str(net, 0x20018)->Assign005e5c50(&name);                           // 005D527E..005D5298
    const U32 one = 1;                                                  // 005D52A0 (ebp)
    sw(net, 0x20014, one);                                              // 005D52A5
    if (kind == 0x40) {                                                 // 005D529D..005D52B0
        if (str(net, 0x20058)->BorrowedEquals005e5290(k_empty_0073d71c) != 0) { // 005D52AB..005D52BF
            sw(net, 0x20034, 0); sw(net, 0x20030, 0); sw(net, 0x20038, one); // 005D52C1..005D52C9 / 005D531A
        }
    } else if (kind == 0x20) {                                          // 005D52D1..005D52D4
        if (str(net, 0x20050)->BorrowedEquals005e5290(k_empty_0073d71c) != 0) { // 005D52D6..005D52E3
            sw(net, 0x20038, 0); sw(net, 0x20034, one); sw(net, 0x20030, 0); // 005D52E5..005D52F3
        }
    } else if (str(net, 0x20048)->BorrowedEquals005e5290(k_empty_0073d71c) != 0) { // 005D52FB..005D5308
        sw(net, 0x20034, 0); sw(net, 0x20030, one); sw(net, 0x20038, one); // 005D530A..005D531A
    }
    reply[4] = 'A';                                                     // 005D5323
    bool accept;
    if (kind == 0x10) accept = str(net, 0x20048)->BorrowedEquals005e5290(k_empty_0073d71c) != 0; // 005D5320..005D533B
    else if (kind == 0x20) accept = str(net, 0x20050)->BorrowedEquals005e5290(k_empty_0073d71c) != 0; // 005D53A1..005D53B1 -> 005D5334
    else if (kind == 0x40) accept = str(net, 0x20058)->BorrowedEquals005e5290(k_empty_0073d71c) != 0; // 005D53B3..005D53CA
    else accept = false;                                                // 005D53B6
    if (accept) {
        reply[5] = 'C';                                                 // 005D53D2
        void* session = pw(net, 0x20008);                               // 005D53D6
        reinterpret_cast<SessionSlot0>(w(pw(session, 0), 0))(session, 0, one, 0); // 005D53D0..005D53E1
    } else {
        reply[5] = 'P';                                                 // 005D5341
    }
    reply[6] = kind; reply[7] = 'U'; reply[8] = 0; reply[9] = 0;        // 005D534E..005D5359
    net_layer_send_message_to_player_005d4cb0(net, nullptr, 0, reinterpret_cast<const char*>(reply), 0x0a, 0); // 005D5345..005D535D
    live_free(reply);                                                   // 005D5362..005D5368
    return one;                                                         // 005D538E
}

// 005D53F0 CNetLayer::HandleBNCRMessage(player, message, length), RET 0Ch.
extern "C" U32 __fastcall kotor_fastcall_005d53f0(void* net, void*, U32, const char* message, U32 length) {
    if (length != 0x0a) return 0;                                       // 005D53F0..005D53F9
    if (message[4] != 'A') { sw(net, 0x20078, 0); return 1; }           // 005D53FC..005D5415
    if (message[5] == 'C') {                                            // 005D5418..005D541C
        sw(net, 0x20070, 1); sw(net, 0x20074, 1); sw(net, 0x20078, 1);  // 005D541E..005D542A
    }
    return 1;                                                           // 005D5404 / 005D5430
}

// 005D5690 CNetLayer::NonWindowMessages(player, message, length), RET 0Ch, EAX 0 (consumer declaration is void).
extern "C" void __fastcall kotor_fastcall_005d5690(void* net, void*, U32 player, const char* message, U32 length) {
    if (message[0] == 'B' && message[1] == 'N') {                       // 005D5696..005D56A1
        if (w(net, 0x2003c) == 1 && message[2] == 'C' && message[3] == 'S' && length >= 7) // 005D56A3..005D56C5
            kotor_fastcall_005d5180(net, nullptr, player, message, length); // 005D56C7..005D56CA
        if (w(net, 0x20060) == 1 && message[2] == 'C' && message[3] == 'R') // 005D56CF..005D56E2
            kotor_fastcall_005d53f0(net, nullptr, player, message, length); // 005D56E4..005D56E9
        return;                                                         // 005D56F1
    }
    void* session = pw(net, 0x20008);                                   // 005D56FB
    reinterpret_cast<SessionSlot4>(w(pw(session, 0), 4))(session, player, message, length, 1); // 005D56F7..005D570C
}
