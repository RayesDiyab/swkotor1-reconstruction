#pragma once
// Private C2 increment-provider contract. No original-address calls are exposed here.
#include "1e4deaabc8-area_attachment.hpp"
#include <cstdint>

namespace c2 {
using Word = kotor::character_load::Word;
using Memory = kotor::character_load::Memory;

// Typed live operations used by original CSWSArea::IncreaseAILevelPriority.
// The adapter supplying these operations owns the exact guest ABI binding.
struct PriorityPorts {
    virtual ~PriorityPorts() = default;
    virtual Word object_array() = 0;
    virtual Word ai_master() = 0;
    virtual std::uint8_t lookup(Word array, Word id, Word& object) = 0;
    virtual Word as_creature(Word object) = 0;          // vtable +0x30
    virtual Word as_door(Word object) = 0;              // vtable +0x18
    virtual Word as_trigger(Word object) = 0;           // vtable +0x40
    virtual Word as_placeable(Word object) = 0;         // vtable +0x48
    virtual Word as_area_of_effect(Word object) = 0;    // vtable +0x50
    virtual int string_length(Word string) = 0;          // CExoString::GetLength
    virtual void set_ai_level(Word master, Word object, Word level) = 0;
};

// Original 00507220 is thiscall on the area. This private source form takes the
// live area explicitly and preserves its entry-to-exit state effects.
void increase_ai_priority_00507220(Memory& memory, PriorityPorts& ports, Word area);
}
