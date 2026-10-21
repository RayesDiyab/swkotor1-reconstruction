// Lane worker "p2s-b": CSWSMessage::HandlePlayerToServer* handlers routed from the 0918 player-message dispatcher.
// This file: 00522810 CSWSMessage::HandlePlayerToServerDialogMessage (player's reply selection in a conversation).
// Transcribed from the original swkotor.exe (read-only); statement comments cite original instruction addresses.
// Original ABI: thiscall(CSWSMessage* this; CSWSPlayer* player, BYTE minor) RET 8.
// Export: extern "C" unsigned __fastcall recovered_player_to_server_dialog_00522810(message, edx, player, minor) @16.
#include "6bd5403233-claude_dialogue_common.hpp"

using namespace dlg;

extern "C" {
// w2_message_native.obj (real bodies; none in the c16 trap census)
U __fastcall kotor_fastcall_004d6230(void* msg, void*, U bits);           // CSWMessage::MessageReadOverflow RET 4
U __fastcall kotor_fastcall_004d6240(void* msg, void*, U bits);           // CSWMessage::MessageReadUnderflow RET 4
U __fastcall kotor_fastcall_00578540(void* msg, void*);                   // CSWSMessage::ReadOBJECTIDServer RET 0
U __fastcall kotor_fastcall_004d6780(void* msg, void*, U bits);           // CSWMessage::ReadDWORD RET 4
U8 __fastcall kotor_fastcall_004d6720(void* msg, void*, U bits);          // CSWMessage::ReadBYTE RET 4
// claude_dialogue_server.cpp: kotor_fastcall_004cb480 CSWSObject::ReplyDialog (declared in that file's export list)
U __fastcall kotor_fastcall_004cb480(void* object, void*, U player, U reply, U flag, U entry, U skip_delay);

U __fastcall recovered_player_to_server_dialog_00522810(void* message, void*, void* player, U minor);
}

// 00522810 CSWSMessage::HandlePlayerToServerDialogMessage(CSWSPlayer* player, BYTE minor) RET 8
extern "C" U __fastcall recovered_player_to_server_dialog_00522810(void* message, void*, void* player, U minor) {
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                   // 00522815..00522820
    if (static_cast<U8>(minor) != 3) return 0;                                          // 00522826..0052282B (byte compare)
    const U object_id = kotor_fastcall_00578540(message, nullptr);                      // 00522831..0052283C edi
    const U reply = kotor_fastcall_004d6780(message, nullptr, 0x20);                    // 00522838..00522847 ebx
    const U flag = kotor_fastcall_004d6720(message, nullptr, 8) != 0 ? 1u : 0u;         // 00522843..00522858 neg/sbb/neg -> ebp
    const U entry = kotor_fastcall_004d6780(message, nullptr, 0x20);                    // 00522850..00522863 (stored over the
                                                                                        //   minor argument slot [esp+1Ch])
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                   // 0052285F..0052286E
    if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                   // 00522870..0052287B
    void* object;                                                                       // [esp+10h] (uninitialised local)
    if (!get_object(object_id, &object)) return 0;                                      // 0052287D..0052289D (cmp [0074666C])
    return kotor_fastcall_004cb480(object, nullptr, rd(player, 4), reply, flag, entry, 0); // 0052289F..005228BE
}
