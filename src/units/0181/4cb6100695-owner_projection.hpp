#pragma once
#include "89766c7147-memory.hpp"
namespace kotor::repository_owner_projection {
using character_load::Memory;
using character_load::Word;
// These declarations transport the existing dialogue inventory owner. They
// do not create new owners for 0055D290 or 00563340.
struct Ports {
    virtual ~Ports() = default;
    virtual Word allocate_006fa7e6(Word bytes) = 0;
    virtual Word first_pc_004aea40(Word server) = 0;
    virtual Word construct_0055d290(Word storage,Word owner,Word shared) = 0;
};
Word party_repository(Memory&, Ports&, Word party);
}

