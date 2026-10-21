#pragma once
#include <cstdint>
namespace recovered_object_state {
template<class Memory> std::uint32_t set(std::uint32_t self,std::uint32_t value,Memory& m){m.write(self+0x1fc,m.read(self+0x1fc)|1u);m.write(self+0xd4,value);return value;}
}
