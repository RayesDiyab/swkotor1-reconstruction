// Lane W4 t1b (review item 2): client creature vtable 0074F5F8 slots reached unconditionally from AIUpdate (+E8h, +C4h)
// and from SetCameraFollow in camera mode 3 (+D0h, +D4h). Currently trap entries kotor_m15e_trap_* in m15e_tables;
// the table owner rebinds the four slots to these symbols (INTEGRATION.md section 1).
#include "a6f1f2d434-creature.hpp"

using namespace w4;

extern "C" {
void* __fastcall kotor_fastcall_00697460(void* appearance, void*);                // CSWCCreatureAppearance::GetCameraFollowPart
U32 __fastcall kotor_fastcall_006974a0(void* appearance, void*);                  // CSWCCreatureAppearance::GetFollowingBody
}
namespace { const char camera_hook_0074f59c[] = "CAMERAHOOK"; }               // .rdata 0074F59C

// 0060EDC0 CSWCCreature::UpdatePopUpBubble() (vtable +E8h, thiscall RET 0). Both arms tail-jump to the empty
// void_return 004015A0, so the observable work is the module/area reads only.
extern "C" void __fastcall kotor_fastcall_0060edc0(void* creature, void*) {
    void* module = kotor_fastcall_005ed530(client_app(), nullptr);                  // 0060EDC0..0060EDCB
    if (module == nullptr) return;                                                // 0060EDD0..0060EDD2
    void* area = lp(module, 0x48);                                                // 0060EDD4
    if (area == nullptr) return;                                                  // 0060EDD7..0060EDD9
    (void)(lw(creature, 0x39C) == lw(area, 0x104));                              // 0060EDDB..0060EDF1 (both -> void_return)
}

// Legacy W4 entry delegates to the unique complete object VFX core body.
extern "C" void __fastcall object_vfx_0063f490(void*, void*);
extern "C" void __fastcall kotor_fastcall_0063f490(void* object, void*) {
    object_vfx_0063f490(object, nullptr);
}

// 00697460 CSWCCreatureAppearance::GetCameraFollowPart() (thiscall RET 0).
extern "C" void* __fastcall kotor_fastcall_00697460(void* appearance, void*) {
    void* model = vcall<void*, U32, U32>(lp(appearance, 0x3C), 0x98, 0xFF, 1);    // 00697460..0069746D
    void* hook = vcall<void*, const char*>(model, 0x10C, camera_hook_0074f59c);   // 00697475..0069747E
    if (hook == nullptr) return model;                                            // 00697484..00697493
    return vcall<void*>(hook, 0x18);                                              // 00697488..0069748D (tail jmp [edx+18h])
}

// 006974A0 CSWCCreatureAppearance::GetFollowingBody() (thiscall RET 0): 1 when the model has a CAMERAHOOK part.
extern "C" U32 __fastcall kotor_fastcall_006974a0(void* appearance, void*) {
    void* model = vcall<void*, U32, U32>(lp(appearance, 0x3C), 0x98, 0xFF, 1);    // 006974A0..006974AC
    return vcall<void*, const char*>(model, 0x10C, camera_hook_0074f59c) != nullptr ? 1u : 0u; // 006974B2..006974C7
}

// 0060EFE0 / 0060EFF0 CSWCCreature::GetCameraFollowPart / GetFollowingBody (vtable +D0h/+D4h): mov ecx,[ecx+21Ch]; jmp.
extern "C" void* __fastcall kotor_fastcall_0060efe0(void* creature, void*) {
    return kotor_fastcall_00697460(lp(creature, 0x21C), nullptr);                 // 0060EFE0..0060EFE6
}
extern "C" U32 __fastcall kotor_fastcall_0060eff0(void* creature, void*) {
    return kotor_fastcall_006974a0(lp(creature, 0x21C), nullptr);                 // 0060EFF0..0060EFF6
}
