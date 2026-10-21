#pragma once
// lane-m15-e (L15-E) private helpers shared by the m15e_*.cpp units (not a consumer header).
// Object views: CSWCObject / CSWCCreature / CSWCCreatureAppearance / anim bases are opaque byte blocks; only the
// offsets cited at each access are touched, through ld/st (memcpy, no type punning), one load per original load
// (extraderef helper spellings rp / rd32, D132).
#include <cstdint>
#include <cstring>
#include "5de39a25ea-m15e.h"
#include "112991fa2b-m15e_animbase.h"
#include "ed6f7a65b7-startup_string.h"      // kotor_reconstruction::StartupString (CExoString, 8 bytes)
#include "00a0946121-resources.h"           // menu_resources::Ref (CResRef, 16 bytes)
#include "13bda7cf95-kotor_unreachable_trap.h"

namespace kotor_m15e {
using I32 = std::int32_t;
using U16 = std::uint16_t;
using U8 = std::uint8_t;
using String = kotor_reconstruction::StartupString;
using Ref = menu_resources::Ref;

template <class T> inline T ld(const void* base, unsigned offset) noexcept {
    T value;
    std::memcpy(&value, static_cast<const unsigned char*>(base) + offset, sizeof value);
    return value;
}
template <class T> inline void st(void* base, unsigned offset, T value) noexcept {
    std::memcpy(static_cast<unsigned char*>(base) + offset, &value, sizeof value);
}
inline void* rp(const void* base, unsigned offset) noexcept { return ld<void*>(base, offset); }
inline U rd32(const void* base, unsigned offset) noexcept { return ld<U>(base, offset); }
inline void* at(void* base, unsigned offset) noexcept { return static_cast<unsigned char*>(base) + offset; }
inline const void* at(const void* base, unsigned offset) noexcept { return static_cast<const unsigned char*>(base) + offset; }
inline float fbits(U bits) noexcept { float f; std::memcpy(&f, &bits, 4); return f; }
inline U bitsf(float f) noexcept { U u; std::memcpy(&u, &f, 4); return u; }

// ---- typed table views (D41: the first word of every object holds the address of its typed table)
inline const KotorM15ObjectMethods_007516c8* object_methods(const void* object) noexcept {
    return *static_cast<const KotorM15ObjectMethods_007516c8* const*>(object);
}
inline const KotorM15CreatureMethods_0074f5f8* creature_methods(const void* creature) noexcept {
    return *static_cast<const KotorM15CreatureMethods_0074f5f8* const*>(creature);
}
inline const KotorM15eAnimBaseVtbl* anim_methods(const void* anim_base) noexcept {
    return *static_cast<const KotorM15eAnimBaseVtbl* const*>(anim_base);
}
// Slot-0 view of any original polymorphic object (every table of the image starts with the scalar deleting
// destructor, thiscall RET 4, EAX = this): used only where the original calls [vptr+0](1) on a member whose
// concrete class belongs to another owner (sound source, sound set, item model, stats ...).
struct Slot0View {
    void* (__fastcall* scalar_deleting_destructor)(void* self, void* edx, U flags);
};
inline void delete_object(void* object) noexcept(false) {
    (*static_cast<const Slot0View* const*>(object))->scalar_deleting_destructor(object, nullptr, 1u);
}
}  // namespace kotor_m15e

// ---------------------------------------------------------------------------------------- existing owners
extern "C" {
// allocator (canonical-inputs task-23 sbh-slice2 live allocator: operator new 006FA7E6 / free 006FA390)
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
// D83 shared trap (m48/t): kotor_unreachable_trap(original address, symbol) never returns.
// client app cell 007A39FC (AppManager*: +4 CClientExoApp*, +8 CServerExoApp*); owner lane-r4-lowers (D50 alias)
}
