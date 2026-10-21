#include "c92269d982-read_callback.hpp"
#include "896b5ec2dc-assignment.h"
// Name joins only (no original-address body credit). Each target is an
// existing stage / sibling-lane symbol with the identical physical ABI.
namespace directory_list {
// lane-resource-lowers: ?LowerCase005e6080@directory_list@@YIPAUStartupString@kotor_reconstruction@@PAU23@PAX0@Z
kotor_reconstruction::StartupString* __fastcall LowerCase005e6080(
    kotor_reconstruction::StartupString*,void*,kotor_reconstruction::StartupString*);
}
namespace kotor_functional {
// M41: ?modern_ini_sprintf@kotor_functional@@YAHPADPBDZZ
int __cdecl modern_ini_sprintf(char*,const char*,...);
}
extern "C" {
menu_resources::String* __fastcall async_assign_string_005e5c50(menu_resources::String*,void*,menu_resources::String*); // M41
menu_resources::String* __fastcall c2da_assign_text_005e5140(menu_resources::String*,void*,const char*);             // M41
char* __cdecl c2da_current_strupr_007136f3(char*);                                        // M41
}

// resource_ref_assignment::bindings is owned by lane-resource-lowers
// (assignment_bindings.cpp); not defined here (review T-D3).

// 00406120 (consumers 0070F800 F93C, 0070FB90 FCCC).
extern "C" menu_resources::Ref* __fastcall c2da_copy_00406120(
    menu_resources::Ref* self,void*,menu_resources::Ref* out,menu_resources::Ref* rhs) {
    return resource_ref_assignment::assign_00406120(self,nullptr,out,rhs);
}

namespace font_callback_read {
// 0070C920 donor (read_callback.cpp) call names -> stage providers.
extern "C" String* __fastcall assign_text_005e5140(String* self,void*,const char* text) {         // 0070C999
    return c2da_assign_text_005e5140(self,nullptr,text);
}
extern "C" String* __fastcall lower_construct_005e6080(String* self,void*,void* destination) {    // 0070C9A7
    return directory_list::LowerCase005e6080(self,nullptr,static_cast<String*>(destination));
}
extern "C" String* __fastcall assign_string_005e5c50(String* self,void*,String* rhs) {             // 0070C9B9
    return async_assign_string_005e5c50(self,nullptr,rhs);
}
// Variadic/stack-preserving routes, same as Oct4 font-import-joins-r2 routes.cpp.
extern "C" __declspec(naked) int __cdecl format_006fadb0(char*,const char*,...) {                  // 0070C97F
    __asm { jmp kotor_functional::modern_ini_sprintf }
}
extern "C" __declspec(naked) char* __cdecl uppercase_007136f3(char*) {                              // 0070C989
    __asm { jmp c2da_current_strupr_007136f3 }
}
}
