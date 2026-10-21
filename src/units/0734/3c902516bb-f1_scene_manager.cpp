// f1_scene_manager.cpp - lane-m46-3dmodel fork F1: Scene::RemoveFromSceneManager 004554B0 (cdecl(part)), the
// callee of lane-3d-emitter emitter_core.cpp (emitter.h:203 declaration). Root set = the lane-3d-emitter
// consumer roots. Lists: emitter bucket 007FBF78 (EmitterCells view) and trigger list 0080AA74 (kotor_scene3d
// Cells view); CExoArrayList::Remove 00445080 = kotor_reconstruction::ArrayMutations::RemoveReverse32 (task-14,
// aurres-array-mutations provider; same binding as emitter_core.cpp:152).
#include "4ac9e30747-emitter.h"

using kotor_scene3d::U;
using kotor_scene3d::I;
using kotor_scene3d::field;

extern "C" void __cdecl scene_remove_from_scene_manager_004554b0(void* part) {
    const auto* const table = static_cast<const aurora_vt::VTable_00740ae8*>(field<const void*>(part, 0));
    const U emitter = table->s034(part, nullptr);                                      // 004554B6..004554BB call [eax+34h]
    const U trigger = static_cast<const aurora_vt::VTable_00740ae8*>(field<const void*>(part, 0))->s054(part, nullptr); // 004554BE..004554C4 call [edx+54h]
    if (emitter != 0) {                                                                 // 004554C7..004554CB
        kotor_scene3d::PointerList* const list = kotor_emitter::emitter_cells.emitter_parts_007fbf78;
        const I n = list->size;                                                         // 004554CD
        I matches = 0;
        for (I i = 0; i < n; ++i)                                                       // 004554D3..004554EA
            if (reinterpret_cast<const U*>(static_cast<std::uintptr_t>(list->data))[i] == emitter) ++matches;   // 004554DB..004554E6 (data reloaded)
        if (matches != 0) list->RemoveReverse32(emitter);                              // 004554EC..004554F6 (ecx = 7FBF78h)
    }
    if (trigger != 0) {                                                                 // 004554FB..004554FD
        kotor_scene3d::PointerList* const list = kotor_scene3d::cells.trigger_list_0080aa74;
        const I n = list->size;                                                         // 004554FF
        I matches = 0;
        for (I i = 0; i < n; ++i)                                                       // 00455505..0045551C
            if (reinterpret_cast<const U*>(static_cast<std::uintptr_t>(list->data))[i] == trigger) ++matches;   // 0045550D..00455518
        if (matches != 0) list->RemoveReverse32(trigger);                              // 0045551E..00455528 (ecx = 80AA74h)
    }
    // EAX at return: the last instruction's value (void in the consumer declaration; not reproduced).
}
