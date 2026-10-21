// action_clear_providers.cpp
// Owned action-input nested providers:
//   006880C0  clear-actions tail helper
//   004D37B0  remove/free tail action
// Static reconstruction only; no native product was built in this lane.
#include "7d9ceabe3d-nested_providers.h"

#include <cstdint>
#include <cstring>

static_assert(sizeof(void*) == 4, "nested-providers recipe is x86 only");

namespace {

template <class T>
T ld(const void* p, U32 off) {
    T v;
    std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof(T));
    return v;
}

void* app_manager() {
    return kotor_menu_window_cells::manager_007a39fc;
}

// Deliberate unguarded original load.  004D37B0 does not null-check the node
// returned by [list]; a null node faults at the original cmp [node],0.
template <class T>
T ldu(const void* p, U32 off) {
    return *reinterpret_cast<const volatile T*>(
        reinterpret_cast<const volatile unsigned char*>(p) + off);
}

}  // namespace

// 006880C0: if the server subobject's tail action cannot be removed, clear all actions.
extern "C" void __fastcall kotor_nested_006880c0(void*, void*) {
    void* party = kotor_fastcall_005ed8b0(ld<void*>(app_manager(), 4), nullptr);
    void* pc = kotor_fastcall_00635460(party, nullptr);
    void* server = kotor_m15a_creature_get_server_creature_0060fb20(pc, nullptr);
    if (!server) {
        return;
    }
    void* sub = ld<void*>(server, 0x9C8);
    if (!sub) {
        return;
    }
    if (kotor_nested_004d37b0(sub, nullptr) == 0) {
        w5_object_clear_all_actions_004ccd80(server, nullptr, 1u);
    }
}

// 004D37B0: remove the list tail and free its payload through the live-policy free.
// The original checks [list] but not [list]'s node; a null node faults at cmp [node],0.
extern "C" U32 __fastcall kotor_nested_004d37b0(void* self, void*) {
    void* list = ld<void*>(self, 0x9B0);
    if (!list) {
        return 0;
    }
    void* node = ld<void*>(list, 0);
    if (ldu<U32>(node, 0) == 0) {  // no node guard: null node faults, as original
        return 0;
    }
    void* payload = kotor_input_hover::ListRemoveTail005e9ce0(
        reinterpret_cast<kotor_list::Internal*>(node), nullptr);
    if (payload) {
        live_free(payload);
    }
    return 1;
}
