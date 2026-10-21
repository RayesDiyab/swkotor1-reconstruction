#pragma once
#include "ece0afe1ac-memory.hpp"
namespace kotor::area_attachment {
using character_load::Memory;
using character_load::Word;
struct Ports {
    virtual ~Ports() = default;
    // Existing 004D8230 owner; status is deliberately ignored by GetArea.
    virtual std::uint8_t get_game_object_004d8230(Word array, Word id, Word& output) = 0;
    // Resolve/call the live guest vtable target, never cast it to a host pointer.
    virtual Word as_area_slot_28(Word function, Word object) = 0;
    virtual void increase_ai_priority_00507220(Word area) = 0;
    virtual void decrease_ai_priority_00507340(Word area) = 0;
};
Word get_area_004cb120(Memory&, Ports&, Word object);
void increment_players_00508c20(Memory&, Ports&, Word area);
void decrement_players_00508c40(Memory&, Ports&, Word area);
}
