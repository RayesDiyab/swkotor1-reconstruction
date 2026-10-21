// Original swkotor.exe 0069D490 and 0069D4C0: forward a flag word to the Gob's
// virtual AddAnimationFlag (+128) / RemoveAnimationFlag (+12C).
// Slot types come from the canonical aurora_model::GobVTable (vtables_gen.h):
// U(__fastcall*)(Gob*, void*, U), canonical bodies 00484C40 / 00484C70 (RET 4).
#include "309ecfaddb-scene3d.h"
#include "3f0de2542c-anim_base_animation_flag_internal.h"
#include <cstddef>
#include <cstdint>
#include <cstring>

using U = std::uint32_t;

namespace {
using GobAddAnimationFlag = decltype(aurora_model::GobVTable::s128_gob_add_animation_flag);
using GobRemoveAnimationFlag = decltype(aurora_model::GobVTable::s12c_gob_remove_animation_flag);
static_assert(offsetof(aurora_model::GobVTable, s128_gob_add_animation_flag) == 0x128, "Gob+128");
static_assert(offsetof(aurora_model::GobVTable, s12c_gob_remove_animation_flag) == 0x12C, "Gob+12C");

template<class F> F slot(const void* object, U offset) {
    const unsigned char* table;
    std::memcpy(&table, object, sizeof table);
    F f;
    std::memcpy(&f, table + offset, sizeof f);
    return f;
}
}  // namespace

// 0069D490..0069D4AF
extern "C" U __stdcall anim_base_add_animation_flag_internal_0069d490(aurora_model::Gob* gob, U flag) {
    if (!gob) return 0;                                                        // 0069D494..0069D49A
    slot<GobAddAnimationFlag>(gob, 0x128)(gob, nullptr, flag);                 // 0069D49D..0069D4A4, EAX discarded
    return 1;                                                                  // 0069D4AA
}

// 0069D4C0..0069D4DF
extern "C" U __stdcall anim_base_remove_animation_flag_internal_0069d4c0(aurora_model::Gob* gob, U flag) {
    if (!gob) return 0;                                                        // 0069D4C4..0069D4CA
    slot<GobRemoveAnimationFlag>(gob, 0x12C)(gob, nullptr, flag);              // 0069D4CD..0069D4D4, EAX discarded
    return 1;                                                                  // 0069D4DA
}
