#include "988aa59a9a-alias_path.h"
#include "e87f8473e7-warning_runtime.h"
#include <type_traits>
namespace alias_path {
using S=kotor_reconstruction::StartupString;
static_assert(std::is_same<decltype(Providers::equals_005e5290),decltype(&S::BorrowedEquals005e5290)>::value,"nominal equality");
static_assert(std::is_same<decltype(Providers::length_005e5790),decltype(&S::BorrowedLength005e5790)>::value,"nominal length");
static_assert(std::is_same<decltype(Providers::prefix_005e5900),decltype(&S::ComparePrefix005e5900)>::value,"nominal prefix");
Providers providers{&S::BorrowedEquals005e5290,&S::BorrowedLength005e5790,
 &S::ComparePrefix005e5900,base_ini_reconstruction::warning_empty_0073d71c};
}
