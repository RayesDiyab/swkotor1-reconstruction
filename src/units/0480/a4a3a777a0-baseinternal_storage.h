#pragma once
#include "a6dc227f17-baseinternal.h"
#include <cstddef>
#include <new>
namespace base_resource_table {
// C++17 support ownership only; no original class/address credit. Default
// initialization starts the real88-element ID array without value clearing,
// placement-array overhead, a cookie, another allocation or a callback.
struct IdStorage {std::uint16_t elements[88];};
static_assert(sizeof(IdStorage)==176 && alignof(IdStorage)==2 &&
              offsetof(IdStorage,elements)==0,"exact ID array storage");
static_assert(sizeof(std::uintptr_t)==4 && sizeof(String)==8,"x86 live cells");
inline std::uint16_t* BeginIdStorage(void* storage) {
 return (::new(storage) IdStorage)->elements;
}
// Individually constructed live String objects are addressed in the supported
// x86 allocation domain. This does not assert a containing String array.
inline String* LiveStringElement(String* first,std::uint32_t index) {
 return std::launder(reinterpret_cast<String*>(
     reinterpret_cast<std::uintptr_t>(first)+index*8u));
}
inline String* StringElementsStorage(void* cookie_storage) {
 return reinterpret_cast<String*>(reinterpret_cast<std::uintptr_t>(cookie_storage)+4u);
}
}
