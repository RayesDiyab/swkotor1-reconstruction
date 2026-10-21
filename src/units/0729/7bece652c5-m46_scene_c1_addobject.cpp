// lane-m46-3dcore fork C1: 00458BD0 Scene::AddObject (Scene vtable 00741708 +AC).
// Original (00458BD0..00458BD2): mov eax,[ecx]; jmp [eax+0B0h] -- a tail jump to Scene::ChildAdd (vtbl+B0,
// 00455670, RET 4) that forwards the caller's single stack argument. The real ABI is therefore
// __thiscall(Scene*, object) with callee cleanup of 4 bytes (@12), NOT the 0-argument contract that
// gen_vtables.py derived for this slot (no RET in a jmp body). This unit deliberately does not include
// scene3d.h, whose vtables_gen.h still declares the @8 contract; parts/C1.json carries the header diff that
// retypes the declaration and SceneVTable::s0ac_scene_add_object to (void*, void*, void*).
// D41: the +B0 slot is read from the receiver's vtable pointer at the call (reloaded), via a typed view of
// the one slot used, matching SceneVTable::s0b0_scene_child_add's type (no function-pointer casts).
#include <cstdint>

namespace {
using U = std::uint32_t;
using ChildAdd = U (__fastcall*)(void*, void*, void*);   // SceneVTable +B0 (00455670 Scene::ChildAdd, RET 4)
struct SceneVTablePrefix {                                  // typed view of SceneVTable up to +B0 (same layout)
    void* slots_000_0ac[0xB0 / 4];
    ChildAdd s0b0_scene_child_add;
};
static_assert(sizeof(SceneVTablePrefix) == 0xB4, "+B0 slot");
}

extern "C" U __fastcall scene_add_object_00458bd0(void* scene, void*, void* object) {
    const SceneVTablePrefix* table = *static_cast<const SceneVTablePrefix* const volatile*>(scene);   // 00458BD0 mov eax,[ecx]
    return table->s0b0_scene_child_add(scene, nullptr, object);                                     // 00458BD2 jmp [eax+0B0h]
}
