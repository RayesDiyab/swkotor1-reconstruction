#include "d2e9f84747-base_ini.h"
namespace base_ini_reconstruction {
ListWrapper* ListWrapper::construct_005ea4d0() {
    void* allocation=bindings.allocate_006fa7e6(12);
    List* result=nullptr;
    if(allocation) {
        // Trivial placement default-initialization starts the exact shared
        // task30 object's lifetime without initializing its bytes.
        result=(::new(allocation) List)->Construct(); // accepted005E9AA0
    }
    internal=result; // publish only after the original constructor returns
    return this;
}
}
