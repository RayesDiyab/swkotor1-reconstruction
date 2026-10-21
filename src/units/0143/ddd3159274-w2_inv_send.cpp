#include "0a527ca9ce-w2_inv_send.hpp"
#include "74be47154e-module_message.hpp"
namespace w2::inv_send {
namespace sm = kotor::server_mainloop;
namespace msg = w2::msg;
namespace {
constexpr Word kMajorInventory = 0x0c;
Word finish_and_send(Memory& m, msg::LocPorts& p, Word self, Word player, Word minor, Word s) {
    const Word data = s, size = s + 4;
    if (sm::message_finish_004d6440(m, self, data, size) == 0) return 0;
    return sm::message_send_00567e60(m, p, self, player, kMajorInventory, minor, m.read(data), m.read(size));
}
}
Word unequip_0056fd10(Memory& m, msg::LocPorts& p, Word self, Word player, Word item, Word flag, Word s) {
    sm::message_begin_004d68a0(m, p, self, 8, 0xffffffffu, 1);             // 0056FD1C
    msg::write_object_id_00578570(m, p, self, item);
    msg::write_bool_004d68e0(m, p, self, flag);
    return finish_and_send(m, p, self, player, 7, s);
}
Word unequip_cancel_0056fd80(Memory& m, msg::LocPorts& p, Word self, Word player, Word item, Word s) {
    sm::message_begin_004d68a0(m, p, self, 4, 0xffffffffu, 1);             // 0056FD8C
    msg::write_object_id_00578570(m, p, self, item);
    return finish_and_send(m, p, self, player, 8, s);
}
}
