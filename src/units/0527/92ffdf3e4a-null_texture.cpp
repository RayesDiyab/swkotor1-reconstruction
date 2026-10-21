#include "aaf9b2970f-texture_join.h"
namespace texture_join {
Ref* __cdecl get_null_004230c0() {
    Ref* result=canonical_null_007a477c.target;
    if(!result) {
        void* const memory=live_operator_new(24);
        result=memory?texture::reference_ctor_00422e30(memory,nullptr):nullptr;
        font::assign_00447fa0(&canonical_null_007a477c,nullptr,result);
        result=canonical_null_007a477c.target; // Original fresh load, not ctor-result cache.
    }
    return result;
}
}
