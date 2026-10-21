#include "0edcad0cd864ffc9_area_attachment.hpp"
namespace kotor::area_attachment {
Word get_area_004cb120(Memory& memory, Ports& ports, Word object) {
    Word found = 0; // original stack local starts at zero
    const Word app = memory.read(0x007a39fcU);
    const Word server = memory.read(app + 8U);
    // Exact complete two-load projection of 004AED70; no new exported owner.
    const Word internal = memory.read(server + 4U);
    const Word array = memory.read(internal + 0x1005cU);
    const Word id = memory.read(object + 0x8cU); // AFTER the getter, 004CB139
    (void)ports.get_game_object_004d8230(array, id, found);
    if (found == 0) return 0;
    const Word methods = memory.read(found);
    const Word function = memory.read(methods + 0x28U);
    return ports.as_area_slot_28(function, found);
}
void increment_players_00508c20(Memory& memory, Ports& ports, Word area) {
    if (memory.read(area + 0x128U) == 0)
        ports.increase_ai_priority_00507220(area);
    // INC reads the live counter after a provider may have modified it.
    const Word count = memory.read(area + 0x128U);
    memory.write(area + 0x128U, count + 1U);
}
void decrement_players_00508c40(Memory& memory, Ports& ports, Word area) {
    const Word count = memory.read(area + 0x128U) - 1U;
    memory.write(area + 0x128U, count);
    if (count == 0) ports.decrease_ai_priority_00507340(area);
    // No write after the callback; preserve any callback mutation.
}
}
