// lane-w3 unit w3-message-read (tier t2). Original: swkotor.exe GOG K1; every statement cites the original instruction.
//
// Coordinator ruling t2-1: the CSWMessage / CSWCMessage readers 004D6170, 004D6180, 004D6250, 004D66F0, 004D6720,
// 004D6750, 004D67B0, 004D67E0, 00692630, 00692660 are owned by lane-w2 tier t2 (src/w2_message.cpp via the native
// exports kotor_fastcall_<addr> in src/w2_message_native.cpp); 004D6780 belongs to the existing 0935 owner through
// W2's veneer kotor_fastcall_004d6780. The former lane-w3 bodies were deleted here and every W3 call site binds to
// the W2 symbols (declarations in w3_client.h). W2 does NOT export 00406DA0 CResRef::CResRef(char const(&)[16])
// (its ReadCResRef inlines the call as a port), so lane-w3 keeps that single leaf below.
// 004D66B0 SetReadMessage, 004D6230 MessageReadOverflow, 004D6240 MessageReadUnderflow stay with
// runtime1115 0917-player_message.obj (recovered_player_message::*).
#include "dd06b37d22-w3_client.h"

using namespace kotor_w3;

extern "C" {
// 00406DA0 CResRef::CResRef(char const (&)[16]): RET 4. Builds through operator=(char(&)[16]) 00406170 into a
// 16-byte temporary (result discarded), exactly as the original.
void* __fastcall recovered_resref_from_bytes_00406da0(void* self, void*, const void* bytes16) noexcept {
    ResRef temporary;                                                 // [esp+4] 16-byte hidden-return slot
    SoundAssign00406170(self, nullptr, &temporary, bytes16);          // 00406DA0..00406DB2
    return self;                                                      // 00406DB7
}
}  // extern "C"
