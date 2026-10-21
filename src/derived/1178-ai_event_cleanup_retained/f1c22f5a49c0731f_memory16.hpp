#pragma once
#include "d4a3ce94afdcb05a_memory.hpp"
namespace kotor::character_load {
// Optional exact-width transport shared by level records and GFF WORD readers.
// A read is one live 16-bit access, not two observable byte accesses. Storage,
// fault behavior and original runtime binding remain the implementing owner's.
struct Memory16:Memory {
    virtual std::uint16_t read_short(Word address) const=0;
};
}
