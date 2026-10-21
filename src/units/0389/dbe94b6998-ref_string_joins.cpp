// Conditional canonical query carriers. No storage/table/allocator definitions.
#include "f3b6b6404a-ref_string_joins.h"
#include "60509abfa1-native_strings.h"
#include <type_traits>
using kotor_reconstruction::StartupString;
using LengthMember=std::int32_t (__thiscall StartupString::*)();
using CStrCarrier=char* (__fastcall*)(StartupString*,void*);
using LowerCarrier=std::int32_t (__cdecl*)(std::int32_t);
static_assert(sizeof(int)==4 && sizeof(StartupString)==8,"full x86 results and canonical String8");
static_assert(std::is_same<decltype(&StartupString::BorrowedLength005e5790),LengthMember>::value,"actual existing length member declaration required");
static_assert(std::is_same<decltype(&WindowCStr005e5670),CStrCarrier>::value,"actual char pointer CStr ABI");
static_assert(std::is_same<decltype(&kotor_lower_006fb766),LowerCarrier>::value,"genuine full int32 cdecl lowercase ABI");
extern "C" std::uint32_t __fastcall async_string_length_005e5790(StartupString* self,void*) {
    return static_cast<std::uint32_t>(self->BorrowedLength005e5790());
}
extern "C" char* __fastcall async_string_cstr_005e5670(StartupString* self,void* carrier) {
    return WindowCStr005e5670(self,carrier);
}
extern "C" int __fastcall async_string_equal_005e5200(StartupString* self,void*,StartupString* compare) {



    const char* left = self->data;
    const char* right = compare->data;
    if (!left) return !right || *right == 0;
    if (!right) return *left == 0;
    for (;;) {
        if (*left != *right) return 0;
        if (*left == 0) return 1;
        ++left;
        ++right;
    }

}
