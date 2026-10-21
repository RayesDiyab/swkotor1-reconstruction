#pragma once
#include "98ca969b25-f200.h"
namespace eb80_private {
using W=menu_resources::Word;
constexpr W AllocationBytes(W capacity){return capacity*28u;}
constexpr bool FirstPass(W capacity){return ((capacity-1u)&0x80000000u)==0;}
}
