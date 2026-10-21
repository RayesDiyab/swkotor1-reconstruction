#include "0850d293b87dd6c5_effect_array_copy.hpp"
namespace kotor::server_mainloop {
Word effect_array_copy_004fdd30(Memory& m, EffectInsertPorts& p, Word self, Word source) {
    m.write(self + 4, 0);
    m.write(self + 8, 0);
    m.write(self, 0);
    if (static_cast<int>(m.read(source + 4)) <= 0) return self;
    Word index = 0;
    do {
        const Word value = m.read(m.read(source) + index * 4);
        const Word capacity = m.read(self + 8);
        if (m.read(self + 4) == capacity) p.grow(self, capacity ? capacity * 2 : 16);
        const Word count = m.read(self + 4);
        const Word data = m.read(self);
        m.write(data + count * 4, value);
        m.write(self + 4, m.read(self + 4) + 1);
        const Word sourceCount = m.read(source + 4);
        ++index;
        if (static_cast<int>(index) >= static_cast<int>(sourceCount)) break;
    } while (true);
    return self;
}
}
