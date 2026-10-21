// W2 t6 native exports: inventory unequip messages 0056FD10 (@20) / 0056FD80 (@16). Writers / framing as w2_chat_send_native.
#include "2346527c32-w2_native_ports.hpp"
#include "0a527ca9ce-w2_inv_send.hpp"
using namespace w2::native;
extern "C" {
unsigned __fastcall recovered_inventory_unequip_message_0056fd10(void* self, void*, unsigned player, unsigned item, unsigned flag) {
    Mem m; MessagePorts p(m); alignas(8) unsigned char s[8] = {};
    return w2::inv_send::unequip_0056fd10(m, p, word(self), player, item, flag, word(s));
}
unsigned __fastcall recovered_inventory_unequip_cancel_message_0056fd80(void* self, void*, unsigned player, unsigned item) {
    Mem m; MessagePorts p(m); alignas(8) unsigned char s[8] = {};
    return w2::inv_send::unequip_cancel_0056fd80(m, p, word(self), player, item, word(s));
}
}
