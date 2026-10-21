// model_unlist.cpp - lane-3d-model rev2: Scene::UnlistSpecialParts 00453550 (D35 handoff, SpawnRoom path).
// The original body is the recursion with six levels unrolled; expressed as a depth-limited local recursion with
// the out-of-line recursive call kept for the children of level 6.
#include "a7e200e217-model_instance.hpp"

using namespace aurora_model;

extern "C" {
void __cdecl scene_bsp_remove_part_0046c510(void* scene, void* part);                     // lane-3d-bsp
void __fastcall unlist_special_parts_00453550(void* scene, void* unused_edx, void* part);  // scene3d.h spelling
}

namespace {
// Scene+80h object: vtbl+10h Remove(part, 0), thiscall RET 8 (00453572..0045357D).
using SceneListRemove = U (__fastcall*)(void* self, void* unused_edx, void* part, U zero);
struct SceneListVTable { U slots[4]; SceneListRemove remove; };
struct SceneListObject { const SceneListVTable* vtable; };
static_assert(offsetof(SceneListVTable, remove) == 0x10, "observed slot");

const PartVTable* part_vt(Part* p) { return get<const PartVTable*>(p, 0); }

void unlist_step(unsigned char* scene, Part* part) {
    void* const special = ptr<void>(part_vt(part)->query24(part, nullptr));        // 00453563..0045356C
    if (special != nullptr) {
        SceneListObject* const list = get<SceneListObject*>(scene, 0x80);           // 00453572
        list->vtable->remove(list, nullptr, special, 0);                            // 00453578..0045357D
        scene_bsp_remove_part_0046c510(scene, special);                             // 00453580..00453582
    }
    void* const other = ptr<void>(part_vt(part)->query34(part, nullptr));          // 0045358A..0045358F
    if (other != nullptr) {
        reinterpret_cast<List*>(at(scene, 0x94))->RemoveReverse32(addr(other));     // 00453598..0045359F (00445080)
        scene_bsp_remove_part_0046c510(scene, other);                               // 004535A4..004535A6
    }
}
void unlist_levels(unsigned char* scene, Part* part, I depth) {
    unlist_step(scene, part);
    for (I i = 0; i < get<I>(part, 0x34); ++i) {                                    // count re-read ([+34])
        Part* const child = ptr<Part>(ptr<U>(get<U>(part, 0x30))[i]);
        if (depth < 6) {
            if (child != nullptr) unlist_levels(scene, child, depth + 1);           // null children skipped inline
        } else {
            unlist_special_parts_00453550(scene, nullptr, child);                   // 00453850..00453859
        }
    }
}
}

// 00453550 Scene::UnlistSpecialParts(part), thiscall RET4: for part and every descendant, the vtbl+24 object
// is removed from scene+80 (vtbl+10, arg 0) and the BSP; the vtbl+34 object from the list at scene+94 and the BSP.
void __fastcall unlist_special_parts_00453550(void* scene, void*, void* part) {
    if (part == nullptr) return;                                                    // 00453555..0045355D
    unlist_levels(static_cast<unsigned char*>(scene), static_cast<Part*>(part), 0);
}
