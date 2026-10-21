#include "81ab9ce981-c2_priority.hpp"
#include <cstdint>

namespace c2 {
void increase_ai_priority_00507220(Memory& memory, PriorityPorts& ports, Word area) {
    const Word objects = ports.object_array();
    const Word master = ports.ai_master();

    // The signed condition is the original initial and per-tail count read.
    for (std::int32_t index = 0;
         static_cast<std::int32_t>(memory.read(area + 0x194U)) > index;
         ++index) {
        const Word list = memory.read(area + 0x190U);
        const Word id = memory.read(list + static_cast<Word>(index) * 4U);
        Word object = 0;
        // 004D8230 returns success in AL; the upper EAX bits are not status.
        if (ports.lookup(objects, id, object) != 0)
            continue;
        if (memory.read_byte(object + 8U) <= 4U)
            continue;
        if (memory.read(object + 0x78U) != 0)
            continue;

        bool raise = false;
        if (ports.as_creature(object) != 0) {
            raise = true;
        } else if (ports.as_area_of_effect(object) != 0) {
            raise = true;
        } else if (ports.as_trigger(object) != 0) {
            raise = true;
        } else {
            // The original tests +0x18 once, then calls it a second live time.
            const Word door = ports.as_door(object);
            if (door != 0) {
                const Word live_door = ports.as_door(object);
                if (ports.string_length(live_door + 0x250U) > 0 &&
                    memory.read(live_door + 0x3C0U) == 0)
                    raise = true;
            } else {
                // Same two-live-call shape for the +0x48 placeable slot.
                const Word placeable = ports.as_placeable(object);
                if (placeable != 0) {
                    const Word live_placeable = ports.as_placeable(object);
                    if (memory.read(live_placeable + 0x398U) == 0)
                        raise = true;
                }
            }
        }

        if (raise)
            ports.set_ai_level(master, object, 1);
    }
}
}
