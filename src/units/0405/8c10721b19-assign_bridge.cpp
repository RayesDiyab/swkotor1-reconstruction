#include "77c58f98ec-native_string_deps.h"
#include "04fadd4a60-string_priority.h"
#include <type_traits>
static_assert(std::is_same<menu_resources::String,kotor_reconstruction::StartupString>::value,"canonical String8 alias");
extern "C" menu_resources::String* __fastcall c2da_assign_text_005e5140(menu_resources::String* self,void* carrier,const char* input) {
    return GuiAssign005e5140(self,carrier,input);
}
