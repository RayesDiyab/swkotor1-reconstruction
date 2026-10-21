#pragma once
#include "1420d47ecc-fetch_value_providers.h"
#include "cd5676f4ba-f990.h"
#include <type_traits>
static_assert(std::is_same<tlk_values::Ref,menu_resources::Ref>::value,"actual canonical Ref alias chain");
static_assert(std::is_same<decltype(&SoundAssign00406170),decltype(&f990_ports::RefCopy00406170)>::value,"direct exact Ref receiver/output/source ABI");
static_assert(std::is_trivially_default_constructible<menu_resources::Ref>::value && std::is_trivially_destructible<menu_resources::Ref>::value,"assignment uses live trivial Refs without construction or cleanup");
