#include "40141ae87f-ini_cleanup.h"
namespace base_ini_reconstruction {
Base* current_root_base() noexcept {
    return static_cast<Base*>(kotor_menu_10_19::dependencies::base_007a39e0);
}
// Original005E9400 reads ECX+8; null means RET12, otherwise the same three
// stack arguments go to the genuine internal Warning. That body stays explicit.
void __fastcall warning_005e9400(kotor_menu20::Debug* debug,void*,int line,
                               const char* source,const char* message) {
    void* internal=debug->internal;
    if(internal)warning_internal_005ec2b0(internal,nullptr,line,source,message);
}
}
