#pragma once
#include "0a481446a4-module.hpp"
namespace kotor::character_load {
using Word=kotor::module::Word;
// Guest addresses are opaque uint32 values. Existing module::Memory owns the
// DWORD contract; each call is a live access, including repeated same addresses.
// Implementations define valid mapped storage and fault behavior. No host cast,
// globals, allocator, layout, implicit alignment or provider is supplied here.
struct Memory: kotor::module::Memory {
 virtual std::uint8_t read_byte(Word address) const=0;
 virtual void write_byte(Word address,std::uint8_t value)=0;
 virtual void write_short(Word address,std::uint16_t value)=0;
};
}
