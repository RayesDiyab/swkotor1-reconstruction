#include "d403f1225a-r8_copy_join.h"
#include "0ec67db7c4-native_strings.h"
#include <new>
namespace kotor_reconstruction {
// Start the canonical String lifetime without default-constructor preclearing.
// The recovered operation remains owned by task20; no copy body is repeated here.
StartupString::StartupString(const StartupString& input) {
    (void)WindowCopy005e5b00(this,nullptr,&input);
}
}
extern "C" kotor_reconstruction::StartupString* __fastcall
r8_copy_construct_005e5b00(kotor_reconstruction::StartupString* self,void*,const kotor_reconstruction::StartupString* input) {
    return ::new(self) kotor_reconstruction::StartupString(*input);
}
