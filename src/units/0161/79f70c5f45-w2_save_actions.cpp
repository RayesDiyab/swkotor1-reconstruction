#include "4b6543586e-w2_save_actions.hpp"
#include <cstdint>
namespace w2::save_actions {
namespace {
using I = std::int32_t;
// .rdata labels
constexpr Word kActionList = 0x00745ea0, kActionId = 0x00745e94, kGroupActionId = 0x00745e84, kNumParams = 0x00745e78,
               kParamaters = 0x00745e6c, kType = 0x00745e38, kValue = 0x00744bfc;
constexpr Word kCodeSize = 0x0074c4f0, kCode = 0x0074c4e8, kCrc = 0x0074c4e4, kInstructionPtr = 0x0074c4d4,
               kSecondaryPtr = 0x0074c4c4, kName = 0x00745eec, kStackSize = 0x00748fe0, kStack = 0x0074c494,
               kBasePointer = 0x0074c4b8, kStackPointer = 0x0074c4a8, kTotalSize = 0x0074c49c;
}

// 004CC7E0. Original frame F (34h locals): +0 list position, +4 situation struct, +8 saved this, +0Ch ActionList
// (20 bytes), +20h Paramaters list (20 bytes); the action struct and param struct are written into the gff and
// parent argument slots (F+38h / F+3Ch) once their values are held in registers.
void save_action_queue_004cc7e0(Memory& m, Ports& p, Word object, Word gff, Word parent, Word s) {
    const Word pos = s, sit_struct = s + 4, actions = s + 0x0c, params = s + 0x20, action = s + 0x38, param = s + 0x3c;
    (void)p.add_list_00412450(gff, actions, parent, p.literal(kActionList));        // 004CC800
    m.write(pos, m.read(m.read(object + 0xfc)));                                      // head position
    while (m.read(pos) != 0) {
        const Word node = p.list_at_pos_005e9b60(m.read(object + 0xfc), m.read(pos));
        (void)p.add_list_element_004124e0(gff, action, actions, 0);
        (void)p.write_dword_00412760(gff, action, m.read(node), p.literal(kActionId));
        (void)p.write_word_004126c0(gff, action, m.read16(node + 0x6c), p.literal(kGroupActionId));
        (void)p.write_word_004126c0(gff, action, m.read16(node + 0x6e), p.literal(kNumParams));
        if (m.read16(node + 0x6e) != 0) {                                             // 004CC88A (unsigned)
            (void)p.add_list_00412450(gff, params, action, p.literal(kParamaters));
            for (Word k = 0; k < m.read16(node + 0x6e); ++k) {                        // count re-read each pass (004CC975)
                (void)p.add_list_element_004124e0(gff, param, params, 1);
                const Word type = m.read(node + 4 + 4 * k), value = m.read(node + 0x38 + 4 * k);
                (void)p.write_dword_00412760(gff, param, type, p.literal(kType));
                switch (type) {                                                       // jump table 004CC9B0
                case 1: (void)p.write_int_004127b0(gff, param, value, p.literal(kValue)); break;
                case 2: (void)p.write_float_00412870(gff, param, value, p.literal(kValue)); break;
                case 3: (void)p.write_dword_00412760(gff, param, value, p.literal(kValue)); break;
                case 4: (void)p.write_string_00412970(gff, param, value, p.literal(kValue)); break;
                case 5:
                    (void)p.add_struct_to_struct_004125b0(gff, sit_struct, param, p.literal(kValue), 2);
                    (void)vm_save_situation_005d10f0(m, p, p.vm_007a3a00(), value, gff, sit_struct, s + 0x40);
                    break;
                default: break;                                                       // 004CC8E4 ja: Type only
                }
            }
        }
        p.list_next_005e9b90(m.read(object + 0xfc), pos);                             // 004CC994
    }
}

// 005D10F0: ecx = [ecx] (CVirtualMachine -> CVirtualMachineInternal), tail jump.
Word vm_save_situation_005d10f0(Memory& m, Ports& p, Word vm, Word situation, Word gff, Word st, Word s) {
    return vm_internal_save_situation_005d47a0(m, p, m.read(vm), situation, gff, st, s);
}

// 005D47A0. The Stack struct is written into the situation argument slot (s here).
Word vm_internal_save_situation_005d47a0(Memory& m, Ports& p, Word vmi, Word sit, Word gff, Word st, Word s) {
    if (m.read(sit + 0x10) != 0 && m.read(sit + 0x14) != 0) {                         // code already loaded
        (void)p.write_int_004127b0(gff, st, m.read(sit + 0x14), p.literal(kCodeSize));
        (void)p.write_void_00412c10(gff, st, m.read(sit + 0x10), m.read(sit + 0x14), p.literal(kCode));
    } else if (p.read_script_file_005d2260(vmi, sit + 0x18) != 0) {                  // 005D47FF: load failed
        (void)p.write_int_004127b0(gff, st, 0, p.literal(kCodeSize));
    } else {
        const Word entry = vmi + 0x2c + 0x28 * m.read(vmi + 0x28);
        (void)p.write_int_004127b0(gff, st, m.read(entry + 0x14), p.literal(kCodeSize));
        const Word entry2 = vmi + 0x2c + 0x28 * m.read(vmi + 0x28);                  // re-read (005D4814)
        (void)p.write_void_00412c10(gff, st, m.read(entry2 + 0x10), m.read(entry2 + 0x14), p.literal(kCode));
        p.delete_script_005d44f0(vmi, vmi + 0x2c + 0x28 * m.read(vmi + 0x28));
        m.write(vmi + 0x28, m.read(vmi + 0x28) - 1);                                  // 005D4845
    }
    m.write(sit + 0x24, 0);                                                           // 005D485E (before the CRC write)
    (void)p.write_dword_00412760(gff, st, 0, p.literal(kCrc));
    (void)p.write_int_004127b0(gff, st, m.read(sit + 8), p.literal(kInstructionPtr));
    (void)p.write_int_004127b0(gff, st, m.read(sit + 0x0c), p.literal(kSecondaryPtr));
    (void)p.write_string_00412970(gff, st, sit + 0x18, p.literal(kName));
    (void)p.write_int_004127b0(gff, st, m.read(sit + 4), p.literal(kStackSize));
    (void)p.add_struct_to_struct_004125b0(gff, s, st, p.literal(kStack), 0);          // 005D48BD
    (void)vm_stack_save_005d1d80(m, p, m.read(sit), gff, s, s + 4);
    return 1;
}

// 005D1D80. Frame: list at F+0 (s); element struct in the CResStruct* argument slot (s + 14h).
Word vm_stack_save_005d1d80(Memory& m, Ports& p, Word stack, Word gff, Word st, Word s) {
    (void)p.write_int_004127b0(gff, st, m.read(stack + 4), p.literal(kBasePointer));
    (void)p.write_int_004127b0(gff, st, m.read(stack), p.literal(kStackPointer));
    (void)p.write_int_004127b0(gff, st, m.read(stack + 8), p.literal(kTotalSize));
    if (static_cast<I>(m.read(stack + 8)) <= 0) return 1;                             // 005D1DC7 jle
    const Word list = s, elem = s + 0x14;
    (void)p.add_list_00412450(gff, list, st, p.literal(kStack));
    if (static_cast<I>(m.read(stack)) <= 0) return 1;                                 // 005D1DE5 jle
    for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(stack)); ++k) {      // 005D1EBF (count re-read)
        (void)p.add_list_element_004124e0(gff, elem, list, k);                        // struct type = index
        const Word raw = m.read8(m.read(stack + 0x0c) + k);
        (void)p.write_char_00412670(gff, elem, raw, p.literal(kType));                // MOVZX byte
        const I type = static_cast<I>(static_cast<std::int8_t>(m.read8(m.read(stack + 0x0c) + k)));   // MOVSX
        const Word value = m.read(m.read(stack + 0x10) + 4 * k);
        switch (type) {                                                               // jump table 005D1ED4 (type-3)
        case 3: (void)p.write_int_004127b0(gff, elem, value, p.literal(kValue)); break;
        case 4: (void)p.write_float_00412870(gff, elem, value, p.literal(kValue)); break;
        case 5: (void)p.write_string_00412970(gff, elem, value, p.literal(kValue)); break;
        case 6: (void)p.write_dword_00412760(gff, elem, value, p.literal(kValue)); break;
        default: {                                                                    // 005D1E9A engine structure
            const Word impl = m.read(m.read(stack + 0x14) + 0x3d4);
            p.save_engine_structure(impl, static_cast<Word>(type - 0x10), value, gff, elem);
            break;
        }
        }
    }
    return 1;
}
}
