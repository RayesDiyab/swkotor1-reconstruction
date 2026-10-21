#include "6431ecf474-alias_add.h"
#include "1ce03b2961-alias_literals.h"
#include "6a0b8e8f70-warning_runtime.h"
#include <type_traits>

// This is a declaration of the existing modern-game-storage cell. It neither
// defines a second cell nor changes the canonical nonvolatile void* contract.
namespace kotor_menu_10_19::dependencies { extern void* base_007a39e0; }

namespace alias_add {
namespace {
int __fastcall ReadActualPublicIni(PublicIni* self, void*, String* value,
                                  String* file, String* category, String* key) {
    return self->read_005e67c0(value, file, category, key);
}
}
using ActualReadMember = int(PublicIni::*)(String*, String*, String*, String*);
static_assert(std::is_same<decltype(&PublicIni::read_005e67c0), ActualReadMember>::value,
              "Actual canonical PublicIni member and full int result");
static_assert(std::is_same<decltype(Dependencies::read_005e67c0),
                           decltype(&ReadActualPublicIni)>::value,
              "Exact ECX, unused EDX and four physical stack words");
static_assert(std::is_same<decltype(kotor_menu_10_19::dependencies::base_007a39e0),
                           void*>::value, "Borrow the sole existing Base cell");
static_assert(sizeof(Dependencies)==20 && offsetof(Dependencies,base_007a39e0)==0 &&
              offsetof(Dependencies,read_005e67c0)==4 &&
              offsetof(Dependencies,section_0074ddb0)==8 &&
              offsetof(Dependencies,empty_0073d71c)==12 &&
              offsetof(Dependencies,slash_0073d8f4)==16,
              "Five original dependency identities, no duplicated owners");

// Every member is an address constant. No ForActualOwners call, startup
// initializer, fixture provider, copied Base pointee, ready flag or fallback.
// The pointee is borrowed through volatile access; actual Base/PublicIni/Alias
// lifetimes and publication are required before AddAlias can be called.
const Dependencies dependencies {
    &kotor_menu_10_19::dependencies::base_007a39e0,
    &ReadActualPublicIni,
    alias_section_0074ddb0,
    base_ini_reconstruction::warning_empty_0073d71c,
    alias_slash_0073d8f4
};
}
