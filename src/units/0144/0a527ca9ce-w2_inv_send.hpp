#pragma once
// W2 tier 6: server -> client inventory unequip messages (major 0Ch, framing via SendServerToPlayerMessage 00567E60).
//   0056FD10 SendServerToPlayerInventory_Unequip       (player, OBJECTID item, BOOL flag) RET 0Ch  minor 07: OBJECTID, BOOL
//   0056FD80 SendServerToPlayerInventory_UnequipCancel (player, OBJECTID item)            RET 8    minor 08: OBJECTID
// CreateWriteMessage(8 / 4, -1, 1). EAX: 0 when GetWriteMessage fails, else the send result.
#include "913ae857a3-w2_message.hpp"
namespace w2::inv_send {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;
// s: >= 8 bytes scratch (GetWriteMessage outputs)
Word unequip_0056fd10(Memory&, w2::msg::LocPorts&, Word msg, Word player, Word item, Word flag, Word s);
Word unequip_cancel_0056fd80(Memory&, w2::msg::LocPorts&, Word msg, Word player, Word item, Word s);
}
