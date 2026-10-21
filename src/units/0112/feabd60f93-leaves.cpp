#include "4a8ca74ca0-leaves.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
namespace w1::leaves {
U message_data_construct_004d69f0(U self, Ports& p) {
  for (U o : {0x8u, 0xcu, 0x4u, 0x14u, 0x18u, 0x10u, 0x20u, 0x24u, 0x1cu, 0x2cu, 0x30u, 0x28u}) p.write(self + o, 0); // 004D6A09..004D6A2A
  p.write8(self, 0);                                                // 004D6A2D
  return self;
}
void message_data_destroy_004d6a40(U self, Ports& p) {
  p.call(0x6fa390, 0, {p.read(self + 0x28)});                       // 004D6A6A free(object ids)
  p.write(self + 0x28, 0);
  U strings = p.read(self + 0x1c);
  if (strings) {                                                    // 004D6A7A..004D6A9E delete[] CExoString array
    U header = strings - 4;
    p.call(0x6fa852, 0, {strings, 8, p.read(header), 0x5e5c20});    // eh vector destructor iterator(~CExoString)
    p.call(0x6fa390, 0, {header});
  }
  p.write(self + 0x1c, 0);
  p.call(0x6fa390, 0, {p.read(self + 0x10)});                       // 004D6AAD free(floats)
  p.write(self + 0x10, 0);
  p.call(0x6fa390, 0, {p.read(self + 4)});                          // 004D6AC1 free(integers)
  p.write(self + 4, 0);
}
void send_feedback_004ede10(U c, U id, U data, Ports& p) {
  U client = p.call(0x4ae910, server(p), {p.read(c + 4)});          // 004EDE34 GetClientObjectByObjectId
  if (client) {
    if (!data) {                                                    // 004EDE43..004EDE75
      U mem = p.call(0x6fa7e6, 0, {0x34});
      data = mem ? message_data_construct_004d69f0(mem, p) : 0;
    }
    p.call(0x4d76b0, data, {9, id & 0xffff});                       // 004EDE86 SetInteger(9, id)
    p.call(0x568570, sws_message(p), {p.read(client + 4), 0xb, data, 0}); // 004EDEA4 SendServerToPlayerCCMessage
  }
  if (data) {                                                       // 004EDEA9..004EDEB5
    message_data_destroy_004d6a40(data, p);
    p.call(0x6fa390, 0, {data});
  }
}
void set_invited_004ef520(U c, U value, Ports& p) {
  p.write(c + 0x930, value);                                        // 004EF526
  p.call(0x4ade40, p.call(0x4aede0, server(p), {}), {c + 0x934, c + 0x938}); // 004EF535..004EF549
}
void clean_invitations_00518660(U c, U f, Ports& p) {
  p.call(0x4ade40, p.call(0x4aede0, server(p), {}), {f + 0x18, f + 0x14}); // now: day f+18, ms f+14
  auto elapsed = [&](U entry) {                                     // SubtractWorldTimes(now, entry stamp) -> ms in f+10
    U wt = p.call(0x4aede0, server(p), {});
    p.call(0x4ae460, wt, {p.read(f + 0x18), p.read(f + 0x14), p.read(entry + 4), p.read(entry + 8), f + 0x24, f + 0x10});
    return p.read(f + 0x10);
  };
  if (U list = p.read(c + 0x93c)) {                                 // 0051869C..005186B6 invitations sent
    for (U i = p.read(list + 4) - 1; sge(i, 0); --i) {
      p.write(f + 0x1c, i);
      U offset = i * 12;
      if (elapsed(p.read(p.read(c + 0x93c)) + offset) > 0x7530) {   // 005186FD
        U other = p.call(0x4ae770, server(p), {p.read(p.read(p.read(c + 0x93c)) + offset)});
        if (other) set_invited_004ef520(other, 0, p);              // 00518727..0051872B
        p.call(0x48dde0, p.read(c + 0x93c), {i});                   // deleteAt
        U mem = p.call(0x6fa7e6, 0, {0x34});                        // 0051873E operator new
        p.write(f + 0x20, mem);
        U data = mem ? message_data_construct_004d69f0(mem, p) : 0;
        p.call(0x4d7780, data, {0, p.read(c + 4)});                 // 0051876D SetObjectID(0, self id)
        send_feedback_004ede10(other, 0x2b, data, p);               // 00518777 (ECX may be NULL as in the original)
        i = p.read(f + 0x1c);
      }
    }
  }
  if (U list = p.read(c + 0x940)) {                                 // 00518790..005187A6 invitations received
    for (U i = p.read(list + 4) - 1; sge(i, 0); --i) {
      p.write(f + 0x1c, i);
      U offset = i * 12;
      p.write(f + 0x20, offset);
      if (elapsed(p.read(p.read(c + 0x940)) + offset) > 0xea60) {   // 005187FB
        U arr = p.read(c + 0x940);
        U count = p.read(arr + 4) - 1;                              // 0051880B..00518815
        p.write(arr + 4, count);
        for (U k = i, off = offset; slt(k, count); ) {              // 00518820..00518840 shift down
          U e = p.read(arr) + off;
          p.write(e, p.read(e + 0xc)); p.write(e + 4, p.read(e + 0x10)); p.write(e + 8, p.read(e + 0x14));
          ++k; off += 12;
          if (!slt(k, p.read(arr + 4))) break;
        }
        i = p.read(f + 0x1c);
      }
    }
  }
  U wt = p.call(0x4aede0, server(p), {});                           // 0051885E..00518890 own invitation
  p.call(0x4ae460, wt, {p.read(f + 0x18), p.read(f + 0x14), p.read(c + 0x934), p.read(c + 0x938), f + 0x24, f + 0x10});
  if (p.read(f + 0x10) > 0x7530) set_invited_004ef520(c, 0, p);
}
U message_get_integer_004d6480(U self, U index, Ports& p) {
  if (!slt(index, p.read(self + 8))) return 0;                      // 004D6484 jge
  return p.read(p.read(self + 4) + index * 4);
}
U message_get_object_004d64c0(U self, U index, Ports& p) {
  if (!slt(index, p.read(self + 0x2c))) return kInvalidObject;      // 004D64C4 jge
  return p.read(p.read(self + 0x28) + index * 4);
}
}
