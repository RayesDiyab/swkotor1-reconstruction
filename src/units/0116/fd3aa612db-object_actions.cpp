#include "c51588067b-object_actions.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// Server action queue: 004CEA20 CSWSObject::AddAction, 004CAC30 AddActionNodeParameter, 0057CB10 AddDoCommandAction;
// 004CE8A0 CSWSObject::SetFadeOutTime.
namespace w1::object_actions {
void add_action_node_parameter_004cac30(U o, U node, U index, U type, U value, Ports& p) {
  (void)o;
  p.write16(node + 0x6e, index + 1);                                // 004CAC52..004CAC55
  if (index >= 0xd) return;                                         // jae (unsigned)
  p.write(node + 4 + index * 4, type);
  U slot = node + 0x38 + index * 4;
  switch (type) {
  case 1: case 2: case 3: p.write(slot, p.read(value)); break;      // int / float / object id: copied from *value
  case 4: {                                                         // string: new CExoString, assigned from *value
    U mem = p.call(0x6fa7e6, 0, {8});
    U s = mem ? p.call(0x5b3190, mem, {}) : 0;                      // CExoString::CExoString
    p.write(slot, s);
    p.call(0x5e5c50, s, {value});                                   // CExoString::operator=
    break;
  }
  case 5: p.write(slot, value); break;                              // script situation: the pointer itself
  default: break;
  }
}
namespace {
U add_action_impl(U o, U type, U group, const U (&params)[26], U scratch, bool front, Ports& p) {
  U mem = p.call(0x6fa7e6, 0, {0x74});                              // 004CEA23..004CEA51
  U node = 0;
  if (mem) {
    for (U i = 0; i < 0xd; ++i) { p.write(mem + 4 + i * 4, 0); p.write(mem + 0x38 + i * 4, 0); }
    p.write16(mem + 0x6e, 0);
    node = mem;
  }
  p.write(node, type);
  p.write(node + 0x70, 1);
  U g = group & 0xffff;
  if (g == 0xffff) {                                                // 004CEA71..004CEA8D: new group id
    U cur = p.read16(o + 0x14);
    U next = (cur + 1) & 0xffff;
    p.write16(o + 0x14, next == 0xffff ? 0 : next);
    p.write16(o + 0x16, cur);
    p.write16(node + 0x6c, cur);
  } else if (g == 0xfffe) {                                         // 004CEC2A..004CEC38: current group
    p.write16(node + 0x6c, p.read16(o + 0x16));
  } else {
    p.write16(node + 0x6c, group);
  }
  for (U k = 0; k < 13; ++k) {                                      // 004CEA91..004CEBE3
    if (!params[k * 2]) break;
    add_action_node_parameter_004cac30(o, node, k, params[k * 2], params[k * 2 + 1], p);
  }
  if (front) p.call(0x5e9d70, p.read(o + 0xfc), {node});            // 004CCCCF CExoLinkedListInternal::AddHead
  else p.call(0x5e9dc0, p.read(o + 0xfc), {node});                  // 004CEBEF CExoLinkedListInternal::AddTail
  U head = p.read(p.read(o + 0xfc));                                // 004CEBF4..004CEC22: walk to the tail
  p.write(scratch, head);
  U r = head;
  if (head && p.read(head + 8)) {
    do r = p.call(0x5e9b90, p.read(o + 0xfc), {scratch});
    while (r);
  }
  return r;
}
}
U add_action_004cea20(U o, U type, U group, const U (&params)[26], U scratch, Ports& p) {
  return add_action_impl(o, type, group, params, scratch, false, p);
}
U add_action_to_front_004ccb00(U o, U type, U group, const U (&params)[26], U scratch, Ports& p) {
  return add_action_impl(o, type, group, params, scratch, true, p);   // 004CCB00: same body, AddHead at 004CCCCF
}
U add_do_command_action_0057cb10(U o, U situation, U scratch, Ports& p) {
  if (!p.read(o + 0xe8)) {                                          // 0057CB10..0057CB2C
    p.call(0x5d1110, p.global(kVirtualMachine), {situation});       // DeleteScriptSituation
    return 0;
  }
  U params[26] = {5, situation};                                    // 0057CB2F..0057CB6D: type 0x25, new group
  add_action_004cea20(o, 0x25, 0xffff, params, scratch, p);
  return 1;
}
void set_fade_out_time_004ce8a0(U o, U value, U scratch, Ports& p) {
  U srv = server(p);
  U player = p.call(0x4ae910, srv, {p.call(0x4aea40, srv, {})});   // GetClientObjectByObjectId(GetPlayerCreatureId())
  if (player) {                                                     // 004CE8C2..004CE8F9
    U head = p.read(p.read(p.read(player + 0xc)));
    p.write(scratch, head);
    for (U pos = head; pos; pos = p.read(scratch)) {
      U entry = p.call(0x5e9b60, p.read(p.read(player + 0xc)), {pos});
      if (p.read(entry + 0x2c) == p.read(o + 4)) { p.write(entry + 0x150, value); return; }   // 004CE90B
      p.call(0x5e9b90, p.read(p.read(player + 0xc)), {scratch});
    }
  }
  p.write(o + 0x20c, value);                                        // 004CE8FB
}
}
