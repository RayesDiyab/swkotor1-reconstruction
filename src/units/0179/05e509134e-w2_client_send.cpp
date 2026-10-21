#include "788ec0865b-w2_client_send.hpp"
#include "74be47154e-module_message.hpp"
namespace w2::client_send {
namespace sm = kotor::server_mainloop;
namespace { constexpr Word kServerPlayer = 0xfffffffdu; }   // [0074C528]
Word send_00677410(Memory& m, w2::msg::Ports& p, Word, Word client_app, Word major, Word minor, Word data, Word size) {
    Word buffer = data, length = size;
    if (size == 0) { length = 3; buffer = p.allocate(3); }
    if (buffer == 0) return 0;
    p.write_byte(buffer, 0x70); p.write_byte(buffer + 1, major); p.write_byte(buffer + 2, minor);
    const Word result = p.send_005d4cb0(p.network_getter(client_app), kServerPlayer, buffer, length, 0);
    if (size == 0) p.free(buffer);
    (void)m;
    return result;
}
Word gui_inventory_status_00677a20(Memory& m, w2::msg::Ports& p, Word self, Word client_app, Word status, Word s) {
    sm::message_begin_004d68a0(m, p, self, 1, 0xffffffffu, 1);
    w2::msg::write_byte_004d62c0(m, p, self, status == 1 ? 1u : 0u, 8);
    if (sm::message_finish_004d6440(m, self, s + 4, s) == 0) return 0;
    return send_00677410(m, p, self, client_app, 0x0d, 1, m.read(s + 4), m.read(s));
}
Word journal_quest_screen_open_00677820(Memory& m, w2::msg::Ports& p, Word client_app) {
    const Word buffer = p.allocate(3);
    if (buffer == 0) return 0;
    p.write_byte(buffer, 0x70); p.write_byte(buffer + 1, 0x1c); p.write_byte(buffer + 2, 0x0a);
    const Word result = p.send_005d4cb0(p.network_getter(client_app), kServerPlayer, buffer, 3, 0);
    p.free(buffer);
    (void)m;
    return result;
}
}
