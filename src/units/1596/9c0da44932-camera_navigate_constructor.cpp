// 00638670 CSWCameraNavigate::CSWCameraNavigate
// Original: ECX=self, EDX unused, [esp+4]=owner, RET 4, EAX=self.
// The caller allocates the 0x30 object and owns it; this constructor does not
// allocate, free, register or guard it. No original EH frame/cleanup exists.
#include "63931e62bc-camera_navigate_constructor.h"
#include "289d120a9a-construct3d.h"
#include "ac3ecc8ec9-camera_behavior_base.h"
#include <cstdint>

namespace w4cam {
// Single typed table contract, owned by the future 007515F4 table owner.
// This TU declares the symbol only; it does not define the table.
extern const kotor_scene3d_construct::BehaviorBaseSlots camera_navigate_vtable_007515f4;
}

namespace {
void store32(void* self, unsigned offset, std::uint32_t value) {
    *reinterpret_cast<volatile std::uint32_t*>(
        static_cast<unsigned char*>(self) + offset) = value;
}
}

extern "C" void* __fastcall kotor_fastcall_00638670(void* self, void*, void* owner) {
    (void)kotor_fastcall_0049f110(self, nullptr, owner);  // 00638678 base ctor

    store32(self, 0x14, 0);  // 0063867F
    store32(self, 0x18, 0);  // 00638682
    store32(self, 0x1c, 0);  // 00638685
    store32(self, 0x20, 0);  // 00638688
    store32(self, 0x24, 0);  // 0063868B
    store32(self, 0x28, 0);  // 0063868E
    store32(self, 0x2c, 0);  // 00638691

    *reinterpret_cast<const kotor_scene3d_construct::BehaviorBaseSlots* volatile*>(self) =
        &w4cam::camera_navigate_vtable_007515f4;  // 00638694

    return self;  // 0063869A
}
