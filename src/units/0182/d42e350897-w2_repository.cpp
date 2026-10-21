#include "7ba7948e7c-w2_repository.hpp"
namespace w2::repository {
using kotor::repository_owner_projection::Word;
// 0055D290 (RET 8): +10/+14/+0C/+00/+04 stores in original order, then bit 0 of +08 := shared bit 0.
Word construct_0055d290(kotor::character_load::Memory& m, Word self, Word owner, Word shared) {
    m.write(self + 0x10, 0); m.write(self + 0x14, 0); m.write(self + 0x0c, 0);
    m.write(self, owner); m.write(self + 4, 0);
    const Word changed = (m.read(self + 8) ^ shared) & 1u;
    m.write(self + 8, m.read(self + 8) ^ changed);
    return self;
}
}
namespace w2::repository {
void set_parent_id_00556150(kotor::character_load::Memory& m, ParentPorts& p, Word self, Word id) {
    m.write(self, id);
    for (Word i = 0; static_cast<int>(i) < static_cast<int>(m.read(self + 0x10)); ++i) {
        const Word server = m.read(m.read(0x7a39fcu) + 8);
        const Word item = p.item_by_id_004ae760(server, m.read(m.read(self + 0x0c) + 4 * i));
        if (item != 0) p.set_possessor_00553210(item, m.read(self), 0, 0, 0x7f000000u);
    }
}
}
