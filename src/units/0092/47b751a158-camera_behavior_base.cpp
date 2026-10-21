// Canonical camera-shared CAurBehavior constructor and original destructor tail alias.
#include "3ad6622b54-camera_behavior_base.h"
#include "289d120a9a-construct3d.h"
#include <cstdint>

extern "C" void __fastcall behavior_destruct_0049fa70(void*, void*);
extern "C" void* __fastcall kotor_fastcall_0049f110(void* self, void*, void* owner) {
    *static_cast<const kotor_scene3d_construct::BehaviorBaseSlots**>(self) =
        &kotor_scene3d_construct::behavior_vtable_00743864; // 0049F114
    auto* words = static_cast<std::uint32_t*>(self);
    words[2] = 0; // 0049F11A
    words[3] = 0; // 0049F11D
    words[1] = 0; // 0049F120
    words[4] = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(owner)); // 0049F127
    return self;
}
extern "C" void __fastcall kotor_fastcall_006db4e0(void* self, void*) {
    behavior_destruct_0049fa70(self, nullptr); // 006DB4E0 JMP 0049FA70
}
