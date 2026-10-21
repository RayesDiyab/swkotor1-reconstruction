#include "572d069072-base_cancel_service.h"
#include "30b63a61bb-files.h"
#include <type_traits>
using Port=void(__fastcall*)(void*,void*);
using Donor=void(__fastcall*)(tlk_file::File*,void*);
static_assert(std::is_same_v<decltype(&c2da_005e69e0),Port>);
static_assert(std::is_same_v<decltype(&tlk_file::file_destroy_005e69e0),Donor>);



// Existing selected CExoFile owner; original destructor reads only internal at +0.
// Resource/type3 CExoFile construction and lifetime must satisfy that owner.
extern "C" void __fastcall c2da_005e69e0(void* wrapper,void*) {
    tlk_file::file_destroy_005e69e0(static_cast<tlk_file::File*>(wrapper),nullptr);
}
