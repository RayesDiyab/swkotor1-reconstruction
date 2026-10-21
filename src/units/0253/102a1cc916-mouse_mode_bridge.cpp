#include "5ad031ea49-mouse_mode_bridge.h"
#include "aa2c4bcb49-input_snapshot.h"
extern "C" __declspec(noreturn) void __cdecl kotor_menu_service_contract_violation(const mouse_mode_join::Call*);
extern "C" mouse_mode_join::Word __cdecl kotor_menu_service_005DF6E0(const mouse_mode_join::Call* c) {
    if (!mouse_mode_join::matches(c)) kotor_menu_service_contract_violation(c);
    // Call.self is the wrapper value, not the address of the singleton cell.
    // The native result contract is unsigned AL8; EAX's upper bits are undefined.
    return static_cast<mouse_mode_join::Word>(
        reinterpret_cast<kotor_reconstruction::InputView*>(c->self)->GetMouseMode());
}
