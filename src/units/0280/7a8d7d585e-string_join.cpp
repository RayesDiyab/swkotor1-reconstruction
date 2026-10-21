#include "886dc8751a-string_join.h"
#include "4f178974e9-native_strings.h"
#include "171def4b62-pilot.h"
#include <type_traits>
using String=base_ini_reconstruction::String;
using Table=base_ini_reconstruction::Bindings;
static_assert(!std::is_same<CExoString,String>::value,"distinct nominal String owners");
static_assert(std::is_same<decltype(Table::c_str_005e5670),decltype(&WindowCStr005e5670)>::value,"exact fastcall receiver/unused EDX/char pointer return");
static_assert(std::is_same<decltype(Table::equals_005e5290),decltype(&String::BorrowedEquals005e5290)>::value,"actual String equality member");
static_assert(std::is_same<decltype(Table::length_005e5790),decltype(&String::BorrowedLength005e5790)>::value,"actual String length member");
static_assert(sizeof(Table)==112 && offsetof(Table,c_str_005e5670)==36 && offsetof(Table,equals_005e5290)==96 && offsetof(Table,length_005e5790)==100,"three transports retain the inspected x86 table offsets");











void ini_string_join::bind(Table& target) noexcept {
 target.c_str_005e5670=&WindowCStr005e5670;
 target.equals_005e5290=&String::BorrowedEquals005e5290;
 target.length_005e5790=&String::BorrowedLength005e5790;
}
