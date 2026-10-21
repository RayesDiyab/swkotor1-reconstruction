#pragma once
#include "9f8b86cc9d-startup_string.h"
#include "0d6cdf91cb-resources.h"
#include <cstdint>
#include <cstddef>
#include <type_traits>
namespace kotor_menu_10_19 {
using String=kotor_reconstruction::StartupString;
using Word=std::uint32_t;
// One existing nominal Ref type; actual output fields/layout remain unchanged.
using SoundRef=menu_resources::Ref;
struct StringResource {String text;SoundRef sound;Word duration_bits;};
static_assert(std::is_same<decltype(StringResource::text),String>::value,"one actual String declaration");
static_assert(sizeof(StringResource)==28&&alignof(StringResource)==4&&offsetof(StringResource,text)==0&&offsetof(StringResource,sound)==8&&offsetof(StringResource,duration_bits)==24,"authenticated TLK output layout");
}

namespace tlk_live {using String=kotor_reconstruction::StartupString;using SoundRef=menu_resources::Ref;using StringResource=kotor_menu_10_19::StringResource;}
