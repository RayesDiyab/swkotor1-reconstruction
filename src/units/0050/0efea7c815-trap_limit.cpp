// Original area trap limit and exact trigger lookup; existing object-array storage is authoritative.
#include "eb237338e8-w5_server_env.hpp"
using namespace w5;
extern "C" {
// [004B1880,004B18C8),72bytes,RET4/native@12.
void* __fastcall kotor_fastcall_004b1880(void* internal, void*, U id) {
    void* object = nullptr;
    if (static_cast<U8>(kotor_fastcall_004d8230(at<void*>(internal, 0x1005c), nullptr, id, &object)) != 0) return nullptr;
    if (object == nullptr || at<U8>(object, 8) != 7) return nullptr;
    return vcall<void*>(object, 0x40);
}
// [004AE790,004AE798),8bytes,RET4 tail wrapper/native@12.
void* __fastcall kotor_fastcall_004ae790(void* server, void*, U id) {
    return kotor_fastcall_004b1880(at<void*>(server, 4), nullptr, id);
}
// [00508960,005089C2),98bytes,RET0/native@8, original result is AL only.
U8 __fastcall kotor_fastcall_00508960(void* area, void*) {
    U8 count = 0;
    for (I index = 0; index < at<I>(area, 0x130); ++index) {
        void* trigger = kotor_fastcall_004ae790(server_app(), nullptr,
            at<U*>(area, 0x12c)[index]);
        if (trigger != nullptr && at<U>(trigger, 0x2bc) != 0 && at<U>(trigger, 0x314) != 0)
            count = static_cast<U8>(count + 1);
    }
    return count;
}
// [005089D0,005089DC),12bytes,RET0/native@8; original unsigned AL compare15.
U __fastcall kotor_fastcall_005089d0(void* area, void*) {
    return kotor_fastcall_00508960(area, nullptr) < 15 ? 1u : 0u;
}
}
