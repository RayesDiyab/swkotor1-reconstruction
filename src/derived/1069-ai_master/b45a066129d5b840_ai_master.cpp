#include "139f9fd1e91b7c00_ai_master.hpp"
namespace kotor::ai_master {
void add_unique_006449d0(Memory& memory, Ports& ports, Word list, Word id) {
    const Word count = memory.read(list + 4U);
    if (count != 0 && count <= 0x7fffffffU) {
        Word address = memory.read(list);
        Word matches = 0;
        for (Word left = count; left != 0; --left, address += 4U)
            if (memory.read(address) == id) ++matches;
        if (matches != 0) return;
    }
    const Word capacity = memory.read(list + 8U);
    if (count == capacity)
        ports.grow_005e03c0(list, capacity == 0 ? 16U : capacity * 2U);
    const Word live_count = memory.read(list + 4U);
    const Word data = memory.read(list);
    memory.write(data + live_count * 4U, id);
    const Word after_store_count = memory.read(list + 4U);
    memory.write(list + 4U, after_store_count + 1U);
}
Word remove_id_004af330(Memory& memory, Ports& ports, Word list, Word id) {
    const Word count = memory.read(list + 4U);
    if (count != 0 && count <= 0x7fffffffU) {
        Word address = memory.read(list);
        const Word first = address;
        Word matches = 0;
        for (Word left = count; left != 0; --left, address += 4U)
            if (memory.read(address) == id) ++matches;
        if (matches != 0) {
            address = first;
            for (Word index = 0; index < count; ++index, address += 4U) {
                if (memory.read(address) == id) {
                    ports.remove_index_006857a0(list, index);
                    break;
                }
            }
        }
    }
    return 1; // Original returns success even when no item matched.
}
Word remove_object_004af3d0(Memory& memory, Ports& ports, Word master, Word object) {
    const Word old_level = memory.read(object + 0x78U);
    if (old_level > 4U) return 0;
    const Word id = memory.read(object + 4U);
    const Word result = remove_id_004af330(memory, ports, master + 4U + old_level * 16U, id);
    if (result == 1) memory.write(object + 0x78U, 0xffffffffU);
    return result;
}
Word add_object_004b0850(Memory& memory, Ports& ports, Word master, Word object, Word level) {
    if (level > 4U || object == 0) return 0;
    if (memory.read(object + 0x78U) != 0xffffffffU)
        (void)remove_object_004af3d0(memory, ports, master, object);
    const Word id = memory.read(object + 4U); // Live after removal.
    add_unique_006449d0(memory, ports, master + 4U + level * 16U, id);
    memory.write(object + 0x78U, level);
    return 1;
}
Word set_level_004b08a0(Memory& memory, Ports& ports, Word master, Word object, Word level) {
    if (memory.read(object + 0x78U) == level) return 1;
    if (level > 4U) return 0;
    return add_object_004b0850(memory, ports, master, object, level);
}
}
