#pragma once
#include <cstdint>
namespace recovered_save_stats {
using Pointer=void*(*)(void*,unsigned);
std::uint16_t maximum_hp_00647a80(void*);
std::uint16_t spell_count_006487d0(void*,unsigned,unsigned);
std::uint32_t spell_006495a0(void*,unsigned,unsigned,unsigned,Pointer);
}
