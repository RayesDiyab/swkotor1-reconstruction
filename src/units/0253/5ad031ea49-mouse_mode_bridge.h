#pragma once
#include "61f04a8cb1-start_body.h"
namespace mouse_mode_join {
using Call = client_services_start::Call;
using Word = client_services_start::Word;
static inline bool matches(const Call* c) noexcept {
    return c && c->site == 0x005F942A && c->target == 0x005DF6E0 &&
           c->abi == client_services_start::Abi::Thiscall &&
           c->count == 0 && c->self != 0;
}
}
extern "C" mouse_mode_join::Word __cdecl kotor_menu_service_005DF6E0(const mouse_mode_join::Call*);
