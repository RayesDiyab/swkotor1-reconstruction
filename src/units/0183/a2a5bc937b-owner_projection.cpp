// Generated from exact dialogue inventory owner 5393dcf4f5d1b6a9470247fd879fe2721bbd30966762137ef812216d2dae6c0b
#include "4cb6100695-owner_projection.hpp"
namespace kotor::repository_owner_projection {
Word party_repository(Memory& memory, Ports& ports, Word party) {
    if(!memory.read(party+0x118U)) {
        const Word allocation=ports.allocate_006fa7e6(24);
        Word result=0;
        if(allocation) {
            const Word app=memory.read(0x007a39fcU);
            const Word server=memory.read(app+8U);
            const auto owner=ports.first_pc_004aea40(server);
            result=ports.construct_0055d290(allocation,owner,1);
        }
        memory.write(party+0x118U,result);
    }
    return memory.read(party+0x118U);
}
}

