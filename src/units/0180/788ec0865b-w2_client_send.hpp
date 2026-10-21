#pragma once
// W2 tier 4: client -> server senders reached at first area entry (W4 review assignment):
// 00677410 CSWCMessage::SendPlayerToServerMessage, 00677A20 SendPlayerToServerGuiInventory_Status,
// 00677820 SendPlayerToServerJournalMessage_QuestScreenOpen. Framing 'p' (70h), major, minor.
#include "913ae857a3-w2_message.hpp"
namespace w2::client_send {
using w2::msg::Word;
using w2::msg::Memory;
// The client app object ([[007A39FC]+4]) supplies the net layer through vtbl+10h (msg::Ports::network_getter).
// 00677410 (major, minor, data, size) RET 10h: size == 0 sends a fresh 3-byte header-only buffer (freed after).
Word send_00677410(Memory&, w2::msg::Ports&, Word self, Word client_app, Word major, Word minor, Word data, Word size);
// 00677A20 (status) RET 4: p/0D/01, BYTE (status == 1). scratch >= 8.
Word gui_inventory_status_00677a20(Memory&, w2::msg::Ports&, Word self, Word client_app, Word status, Word scratch);
// 00677820 () RET: p/1C/0A with no payload, sent from its own 3-byte buffer.
Word journal_quest_screen_open_00677820(Memory&, w2::msg::Ports&, Word client_app);
}
