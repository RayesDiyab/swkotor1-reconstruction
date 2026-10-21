#include "521c87c368-action_queue.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// CSWSObject action-queue group helpers (list at object+0xFC, node data: +0x00 action type, +0x6C WORD group id,
// +0x6E WORD parameter count, +0x04.. parameter types, +0x38.. parameter values).
namespace w1::action_queue {
// Ten 0x1C-byte action-queue slots at creature+0x3A8. 004F6F30 writes:
//   +0x00 WORD group, +0x02 WORD AQ action id, +0x04 WORD 0xFFFF,
//   +0x06 untouched by this function, +0x08 DWORD 0xFFFFFFFF,
//   +0x0C/+0x10/+0x14 Vector, +0x18 DWORD 0x7F000000.
// The +0x06 omission and the null-payload re-scan are original behaviour.
static void reset_action_slot_004f6f30(U creature, U index, Ports& p) {
  U slot = creature + 0x3a8 + index * 0x1c;
  p.write16(slot + 0x00, 0xffff);                                   // 004F6F9D / 004F74A5
  p.write16(slot + 0x02, 0xffff);                                   // 004F6FA7 / 004F74A9
  p.write16(slot + 0x04, 0xffff);                                   // 004F6FAE / 004F74AD
  p.write(slot + 0x08, 0xffffffffu);                                // 004F6FBB / 004F74B7
  p.write(slot + 0x0c, 0);                                          // 004F6F51 / 004F74A3
  p.write(slot + 0x10, 0);                                          // 004F6F53 / 004F74BE
  p.write(slot + 0x14, 0);                                          // 004F6F56 / 004F74C5
  p.write(slot + 0x18, 0x7f000000u);                                // 004F6FB5 / 004F74B0
  // +0x06 is deliberately not written: 004F6F4B..004F74C8 never touch it.
}
static void reset_action_slots_from_004f6f30(U creature, U index, Ports& p) {
  // 004F745B..004F74C8: reset the current and remaining slots, no call.
  for (U i = index; i < 10u; ++i) reset_action_slot_004f6f30(creature, i, p);
}
// 004F6F30 CSWSCreature::UpdateActionQueue, __thiscall void(void), RET 0, frame 0x58.
// `scratch` is 8 bytes: [0..3] CExoLinkedList position scratch, [4..7] action-node out pointer.
// Original quirks preserved: only 0xFFFF increments `skipped`; a non-null position with a null
// payload re-scans the SAME index+skipped with no increment; a type-46 item/property failure
// consumes a slot without writing it; slot+0x06 is never written.
void update_action_queue_004f6f30(U creature, U scratch, Ports& p) {
  if (creature + 0xfc == 0) return;                                 // 004F6F36..004F6F40 (lea/test)
  for (U i = 0; i < 10u; ++i) reset_action_slot_004f6f30(creature, i, p); // 004F6F46..004F719C
  U index = 0;                                                      // [esp+0x20]
  U skipped = 0;                                                    // [esp+0x24]
  while (index < 10u) {                                             // 004F71B6..004F744D
    U node = 0;
    for (;;) {
      U position = position_by_group_index_004cc5c0(creature, index + skipped, scratch, p); // 004F71B6..71C8
      if (!position) { reset_action_slots_from_004f6f30(creature, index, p); return; }      // 004F71CA..71E6 -> 004F745B
      node = p.call(0x5e9b60, p.read(creature + 0xfc), {position});                          // 004F71CE..71DA
      if (node) break;                                                                       // 004F71EC..7212: same-index re-scan
    }
    U group = p.read16(node + 0x6c);                               // 004F71F0..71F2
    U action_id = action_by_group_id_004cc530(creature, group, scratch + 4, scratch, p);     // 004F71F6..7208
    if ((action_id & 0xffff) == 0xffff) { ++skipped; continue; }    // 004F7205..720E: only 0xFFFF skips
    U action = p.read(scratch + 4);                                 // [esp+0x1C] action-node out
    if (!action) { reset_action_slots_from_004f6f30(creature, index, p); return; }            // 004F721F..7225 -> 004F745B
    U slot = creature + 0x3a8 + index * 0x1c;
    bool consumed = false;                                          // type-46 failure still advances the slot
    switch (static_cast<std::uint16_t>(action_id)) {                // 004F722B..723F, tables 004F74D4/004F74F0
    case 1: {                                                       // 004F7246..004F72A0
      U x = p.read(action + 0x38);
      U y = p.read(action + 0x3c);
      U z = p.read(action + 0x40);
      U extra = p.read(action + 0x44);
      U flags = p.read(action + 0x4c);
      if (((flags >> 6) & 3) == 0) {                                // 004F7276..727F (shrl 6; test bl,3)
        p.write(slot + 0x0c, x);
        p.write(slot + 0x10, y);
        p.write(slot + 0x14, z);
        p.write(slot + 0x18, extra);                                // 004F72A0 -> 004F741F
      }
      break;
    }
    case 7: case 12: case 20: case 21: case 24: case 25: case 26: case 27: case 28:
    case 38: case 39: case 40: case 41: case 43: case 50: case 54: case 55: case 56: case 62:
      p.write(slot + 0x18, p.read(action + 0x38));                  // 004F741C..004F741F
      break;
    case 9: {                                                       // 004F738B..004F73CF
      U x = p.read(action + 0x3c);                                  // cached before the slot stores
      U y = p.read(action + 0x40);
      U z = p.read(action + 0x44);
      p.write(slot + 0x0c, x);
      p.write(slot + 0x10, y);
      p.write(slot + 0x14, z);
      break;
    }
    case 15: {                                                      // 004F72A5..004F72F2
      U x = p.read(action + 0x50);                                  // cached before the slot stores
      U y = p.read(action + 0x54);
      U z = p.read(action + 0x58);
      U field18 = p.read(action + 0x4c);
      U field8 = p.read(action + 0x38);
      p.write(slot + 0x18, field18);
      p.write(slot + 0x08, field8);
      p.write(slot + 0x0c, x);
      p.write(slot + 0x10, y);
      p.write(slot + 0x14, z);
      break;
    }
    case 29: {                                                      // 004F73D1..004F741A
      U x = p.read(action + 0x40);                                  // cached before the slot stores
      U y = p.read(action + 0x44);
      U z = p.read(action + 0x48);
      U field18 = p.read(action + 0x3c);
      p.write(slot + 0x18, field18);
      p.write(slot + 0x0c, x);
      p.write(slot + 0x10, y);
      p.write(slot + 0x14, z);
      break;
    }
    case 46: {                                                      // 004F72F7..004F7386
      U p48 = p.read(action + 0x48);                                // cached before the two calls
      U id = p.read(action + 0x38);
      U prop_index = p.read(action + 0x3c);
      U p44 = p.read(action + 0x44);
      U p4c = p.read(action + 0x4c);
      U p50 = p.read(action + 0x50);
      U manager = p.global(kAppManager);                            // 004F7311: canonical [007A39FC]
      U server_app = p.read(manager + 8);                           // 004F7317: [[007A39FC]+8]
      U item = p.call(0x4ae760, server_app, {id});                  // 004F731A..004F7326
      if (!item) { consumed = true; break; }                        // 004F7326 -> 004F7439
      U prop = p.call(0x553960, item, {prop_index});                // 004F732C..004F7336
      if (!prop) { consumed = true; break; }                        // 004F7336 -> 004F7439
      U prop_word = p.read16(prop + 2);                             // 004F733C (read before the slot stores)
      p.write(slot + 0x0c, p48);                                    // 004F7340..004F7373
      p.write(slot + 0x10, p4c);
      p.write(slot + 0x14, p50);
      p.write(slot + 0x18, p44);                                    // 004F7371
      p.write(slot + 0x08, prop_word);                              // 004F736E/737A (movzwl, dword store)
      break;
    }
    default:
      break;                                                        // 004F7421 common tail only
    }
    if (!consumed) {
      p.write16(slot + 0x00, p.read16(action + 0x6c));              // 004F7421/742A
      U aq = p.call(0x57a2b0, creature, {action_id, 0});            // 004F7425..004F742E
      p.write16(slot + 0x02, aq & 0xffff);                          // 004F7433
    }
    ++index;
  }
}
U acceptable_action_0057a240(U type) {
  switch (type) {                                                   // 0057A240..0057A262 (byte table 0057A270)
  case 1: case 7: case 9: case 12: case 15: case 20: case 21: case 24: case 25: case 26: case 27: case 28: case 29:
  case 30: case 38: case 39: case 40: case 41: case 42: case 43: case 46: case 50: case 54: case 55: case 56: case 61: case 63:
    return 1;
  default: return 0;
  }
}
U num_groups_004cc690(U object, U scratch, Ports& p) {
  U list = p.read(object + 0xfc);
  U position = p.read(list);
  if (!position) return position;                                   // 004CC69C..004CC6A2 (EAX = 0)
  p.write(scratch, position);
  U data = p.call(0x5e9b60, list, {position});
  U group = p.read16(data + 0x6c);                                  // read before the null test (original order)
  U count = data ? 1 : 0;
  while (p.read(scratch)) {                                         // 004CC6BC..004CC6E6
    U g = p.read16(data + 0x6c);
    if (g != group) { group = g; ++count; }
    data = p.call(0x5e9b90, p.read(object + 0xfc), {scratch});
  }
  return count;
}
U position_by_group_index_004cc5c0(U object, U index, U scratch, Ports& p) {
  U list = p.read(object + 0xfc);
  U position = p.read(list);
  if (!position) return position;                                   // 004CC5D1..004CC5D6
  p.write(scratch, position);
  U data = p.call(0x5e9b60, list, {position});
  U group = p.read16(data + 0x6c);
  if (!index) return p.read(scratch);                               // 004CC5E9..004CC5FA
  U seen = 0;
  for (;;) {                                                        // 004CC600..004CC622
    U g = p.read16(data + 0x6c);
    if (g != group) { group = g; ++seen; }
    if (seen == index) return p.read(scratch);
    data = p.call(0x5e9b90, p.read(object + 0xfc), {scratch});
    if (!data) return 0;
  }
}
U action_by_group_id_004cc530(U object, U group, U out, U scratch, Ports& p) {
  p.write(out, 0);                                                  // 004CC539
  U list = p.read(object + 0xfc);
  U position = p.read(list);
  U result = 0xffff;
  if (!position) return position | 0xffff;                          // 004CC550..004CC556 (or ax,bp)
  p.write(scratch, position);
  U data = p.call(0x5e9b60, list, {position});
  bool in_group = false;
  while (data) {                                                    // 004CC570..004CC5B2
    if (p.read16(data + 0x6c) == (group & 0xffff)) {
      in_group = true;
      if (acceptable_action_0057a240(p.read(data)) == 1) {          // GetAcceptableAction([data])
        result = p.read16(data);
        p.write(out, data);
      }
    } else if (in_group) {
      break;
    }
    data = p.call(0x5e9b90, p.read(object + 0xfc), {scratch});
  }
  return result;                                                    // mov ax, bp (upper EAX bits from the last call)
}
void action_node_destroy_004cc4d0(U node, Ports& p) {
  for (U i = 0; i < p.read16(node + 0x6e); ++i) {                   // 004CC4D6..004CC526
    U slot = node + 0x38 + i * 4;
    U value = p.read(slot);
    if (!value) continue;
    U kind = p.read(slot - 0x34);
    if (kind == 4) {                                                // CExoString parameter
      p.call(0x5e5c20, value, {});
      p.call(0x6fa390, 0, {value});
      p.write(slot, 0);
    } else if (kind == 5) {                                         // script situation parameter
      U vm = p.global(kVirtualMachine);
      if (vm) { p.call(0x5d1110, vm, {value}); p.write(slot, 0); }  // CVirtualMachine::DeleteScriptSituation
    }
  }
}
void remove_group_004cc6f0(U object, U group, U scratch, Ports& p) {
  U list = p.read(object + 0xfc);
  U position = p.read(list);
  if (!position) return;                                            // 004CC6FC
  p.write(scratch, position);
  U data = p.call(0x5e9b60, list, {position});
  U current = p.read(scratch);
  if (!current) return;
  do {                                                              // 004CC720..004CC779
    if (p.read16(data + 0x6c) == (group & 0xffff)) {
      U node = data;
      data = p.call(0x5e9b90, p.read(object + 0xfc), {scratch});   // advance first
      p.call(0x5e9c10, p.read(object + 0xfc), {current});           // CExoLinkedListInternal::Remove(old position)
      if (node) {
        action_node_destroy_004cc4d0(node, p);
        p.call(0x6fa390, 0, {node});
      }
    } else {
      data = p.call(0x5e9b90, p.read(object + 0xfc), {scratch});
    }
    current = p.read(scratch);
  } while (current);
}
void clear_hostile_004f61d0(U c, U target, U scratch, Ports& p) {
  // `lea eax,[esi+0xfc]; test eax,eax` can never be zero for a valid object.
  U n = num_groups_004cc690(c, scratch, p);
  if (!sgt(n, 0)) return;
  for (U i = 0; slt(i, n); ++i) {                                   // 004F6200..004F6283
    U position = position_by_group_index_004cc5c0(c, i, scratch, p);
    if (!position) continue;
    U data = p.call(0x5e9b60, p.read(c + 0xfc), {position});
    if (!data) continue;
    U group = p.read16(data + 0x6c);
    U id = action_by_group_id_004cc530(c, group, scratch + 4, scratch + 8, p);
    if ((id & 0xffff) == 0xffff) continue;
    U action = p.read(scratch + 4);
    if (!action) continue;
    U kind = p.read(action);
    U field;
    if (kind == 0xc) field = 0x38;                                   // attack
    else if (kind == 0xf) field = 0x4c;                              // cast spell
    else if (kind == 0x2e) field = 0x44;
    else continue;
    if (target && p.read(action + field) != p.read(target + 4)) continue;
    remove_group_004cc6f0(c, group, scratch + 8, p);                // 004F6272..004F627A
    i = 0;                                                          // xor ebx,ebx; then inc -> restarts at index 1
  }
}
}
