#include "42bcb3815d-directory_ref_boundary.hpp"
#include "d91679e4d4-native_string_deps.h"
#include "90c0a5b35a-allocator_live.h"
#include <cstddef>
#include <type_traits>
static_assert(sizeof(menu_resources::Ref)==16 && alignof(menu_resources::Ref)==1 &&
              offsetof(menu_resources::Ref,bytes)==0,"current nominal Ref");
static_assert(std::is_same<menu_resources::String,kotor_reconstruction::StartupString>::value,
              "existing constructed String8");
extern "C" void __fastcall c2da_directory_ref_string_00405f70(
    menu_resources::Ref* reference,void* edx,menu_resources::String* output) noexcept(false) {
    using Copy=void(__fastcall*)(menu_resources::Ref*,void*,menu_resources::String*);
    Copy volatile provider=&c2da_ref_to_string_00405f70;
    provider(reference,edx,output);
}
