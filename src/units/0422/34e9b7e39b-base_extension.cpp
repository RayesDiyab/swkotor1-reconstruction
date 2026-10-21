#include "41a6af6a8a-base_extension.h"
#include "552d7598f8-baseinternal_storage.h"
#include <cstddef>
#include <type_traits>

namespace {
using Base = base_ini_reconstruction::Base;
using Internal = base_ini_reconstruction::BaseInternal;
using String = kotor_reconstruction::StartupString;
static_assert(offsetof(Base, internal) == 0x14, "actual Base receiver");
static_assert(sizeof(Internal) == 12 && offsetof(Internal, extension_ids) == 4 &&
              offsetof(Internal, extension_strings) == 8, "actual table owner");

// Nominal-type projection of the already recovered Task32 algorithm. This is
// not a cast to Task32's bounded fixture views, and supplies no object lifetime.
String* FindExtension(Internal* internal, std::uint16_t type) {
    // Original 005E7A01 captures the signed count once. The current nominal
    // field is unsigned, so test its sign bit without an out-of-range cast.
    const std::uint32_t count = internal->extension_count;
    if (count != 0 && count <= 0x7fffffffu) {
        const std::uint16_t* const ids = internal->extension_ids;
        for (std::uint32_t index = 0; index < count; ++index) {
            if (ids[index] == type) {
                return base_resource_table::LiveStringElement(
                    internal->extension_strings, index);
            }
        }
    }
    // Original fixed fallback is record 87, independent of the captured count.
    return base_resource_table::LiveStringElement(internal->extension_strings, 87);
}
}

extern "C" kotor_reconstruction::StartupString* __fastcall c2da_base_005e6660(
    void* receiver, void*, std::uint32_t type_word) {
    auto* const base = static_cast<Base*>(receiver);
    auto* const internal = static_cast<Internal*>(base->internal);
    return FindExtension(internal, static_cast<std::uint16_t>(type_word));
}

using Contract = String* (__fastcall*)(void*, void*, std::uint32_t);
static_assert(std::is_same<decltype(&c2da_base_005e6660), Contract>::value,
              "ECX receiver, unused EDX, one DWORD stack argument, EAX pointer");
