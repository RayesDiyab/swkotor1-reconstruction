#include "5072eb8c01-directory_data.h"
#include "90c0a5b35a-allocator_live.h"
#include <type_traits>
namespace {
using Allocate=void*(__cdecl*)(std::uint32_t);
static_assert(std::is_same<decltype(&live_operator_new),Allocate>::value,"Existing exact allocator signature");
}
extern "C" void* __cdecl c2da_directory_new_006fa7e6(std::uint32_t size) noexcept(false) {
 Allocate volatile target=&live_operator_new;
 return target(size);
}
