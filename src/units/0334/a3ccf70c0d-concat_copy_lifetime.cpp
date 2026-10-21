#include "1095ffa0e5-concat_copy_lifetime.h"
#include <new>
namespace base_ini_string_joins {
kotor_reconstruction::StartupString* __fastcall ConstructFreshConcatOutput(
    kotor_reconstruction::StartupString* output, void*,
    const kotor_reconstruction::StartupString* input) {
    // No default construction or pre-copy zero stores. The existing explicit
    // owned-copy lifetime shell alone delegates to genuine WindowCopy005e5b00.
    return ::new (static_cast<void*>(output))
        kotor_reconstruction::StartupString(*input);
}
}
