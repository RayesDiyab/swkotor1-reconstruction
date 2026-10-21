// CSWCAnimBaseProjectile (projectile model, table 00759170): the six derived functions.
//   006EA470 ctor            -> object_projectile_model_006ea470   void* __fastcall(void*, void*)       (consumer contract:
//                                                                   sol-vfx-render object_projectile_foundation.cpp)
//   006EA490 complete dtor   -> object_projectile_model_006ea490   void  __fastcall(void*, void*)
//   006EA680 slot +000       -> object_projectile_model_006ea680   KotorM15eAnimBaseVtbl::s000 (scalar deleting dtor)
//   006EA4E0 slot +00C       -> object_projectile_model_006ea4e0   KotorM15eAnimBaseVtbl::s00c (LoadModel)
//   006EA550 slot +054       -> object_projectile_model_006ea550   KotorM15eAnimBaseVtbl::s054 (SetAnimation)
//   006EA6A0 slot +064       -> object_projectile_model_006ea6a0   KotorM15eAnimBaseVtbl::s064 (SetOrientation(Vector))
// Slot exports have exactly the canonical slot types (static_assert below) and the decorated names tier t7 imports.
// Callees are the existing providers (declarations from m15e_animbase_internal.h / scene3d.h; pitch copied verbatim
// from lane-m15-a m15a_creature.h line 9). Literal strings are the original bytes; float constants are the original
// IEEE bits (0073D700 = 0.0f, 00747504 = 0x3727C5AC).
#include "0c7ab49f76-m15e_animbase_internal.h"
#include "2988d44c0e-anim_base_projectile_vtable_00759170.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

using kotor_m15e::U;
using B = std::uint8_t;

extern "C" {
// lane-m15-a m15a_creature.h line 9 (provider: _kotor_m15a_pitch_004aa130, 0816-m15a-creature.obj)
float __cdecl kotor_m15a_pitch_004aa130(const void* vector);

void* __fastcall object_projectile_model_006ea470(void* self, void*);
void __fastcall object_projectile_model_006ea490(void* self, void*);
}

static_assert(std::is_same_v<decltype(&object_projectile_model_006ea680), decltype(KotorM15eAnimBaseVtbl::s000)>, "s000");
static_assert(std::is_same_v<decltype(&object_projectile_model_006ea4e0), decltype(KotorM15eAnimBaseVtbl::s00c)>, "s00c");
static_assert(std::is_same_v<decltype(&object_projectile_model_006ea550), decltype(KotorM15eAnimBaseVtbl::s054)>, "s054");
static_assert(std::is_same_v<decltype(&object_projectile_model_006ea6a0), decltype(KotorM15eAnimBaseVtbl::s064)>, "s064");

namespace {
using GobPlayAnimation = decltype(aurora_model::GobVTable::s018_gob_play_animation);
using GobSetOrientation = decltype(aurora_model::GobVTable::s060_gob_set_orientation);
static_assert(offsetof(aurora_model::GobVTable, s018_gob_play_animation) == 0x18, "Gob+18");
static_assert(offsetof(aurora_model::GobVTable, s060_gob_set_orientation) == 0x60, "Gob+60");

// Original literals (bytes read from the image; no original address is dereferenced).
#include <kotor_generated/projectile/projectile_model_literals.inc>






constexpr U kOneBits = 0x3F800000u;           // 1.0f (006EA561 / 006EA653 immediates)
constexpr float kZero_0073d700 = 0.0f;        // 0073D700 = 00000000
constexpr U kEpsilonBits_00747504 = 0x3727C5ACu;  // 00747504 = 1.0e-5f

template<class T> T rd(const void* p, std::size_t o) { T v; std::memcpy(&v, static_cast<const B*>(p) + o, sizeof v); return v; }
template<class T> void wr(void* p, std::size_t o, T v) { std::memcpy(static_cast<B*>(p) + o, &v, sizeof v); }
U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
aurora_model::Gob* model(const void* self) { return rd<aurora_model::Gob*>(self, 0xB8); }
template<class F> F gob_slot(const aurora_model::Gob* g, std::size_t off) { return rd<F>(rd<const B*>(g, 0), off); }

// 006EA6B5..006EA6DE: x*x + y*y + z*z in the original x87 order, FCOMP against 1.0e-5f, FSTP st(0),
// FNSTSW, FSTP, TEST AH,41h, FSTP, JP. Returns 1 when the original takes the JP (greater or unordered).
__declspec(noinline) U length_squared_exceeds(const float* v, const U* epsilon_bits) {
    U result = 0;
    __asm {
        mov ecx, v
        mov edx, epsilon_bits
        fld dword ptr [ecx+8]
        fld dword ptr [ecx+4]
        fld dword ptr [ecx]
        fld st(0)
        fmul st(0), st(1)
        fld st(2)
        fmul st(0), st(3)
        faddp st(1), st(0)
        fld st(3)
        fmul st(0), st(4)
        faddp st(1), st(0)
        fcomp dword ptr [edx]
        fstp st(0)
        fnstsw ax
        fstp st(0)
        test ah, 041h
        fstp st(0)
        jnp not_exceeding
        mov result, 1
    not_exceeding:
    }
    return result;
}

// 006EA490 unwind state 0: on any exit the CSWCAnimBase base destructor 0069E140 runs (FuncInfo 007882B8,
// unwind action 00738260 = mov ecx,[ebp-10h]; jmp 0069E140). noexcept(false): on the normal path the original
// calls 0069E140 directly (state -1), so an exception from it propagates instead of terminating.
struct BaseDestructOnExit {
    void* self;
    ~BaseDestructOnExit() noexcept(false) { kotor_m15e_anim_base_destruct_0069e140(self, nullptr); }
};
}  // namespace

extern "C" {

// 006EA470..006EA488: base ctor, vptr = 00759170, byte +C4 = 7, EAX = this. No EH frame in the original.
void* __fastcall object_projectile_model_006ea470(void* self, void*) {
    kotor_m15e_anim_base_construct_0069dfb0(self, nullptr);                       // 006EA473
    wr<const void*>(self, 0, &kotor_m15e_anim_base_projectile_vtable_00759170);    // 006EA478
    wr<B>(self, 0xC4, 7);                                                          // 006EA47E
    return self;                                                                   // 006EA485
}

// 006EA490..006EA4DE: vptr = 00759170; UnloadModel() 0069D740 inside unwind state 0; then base dtor 0069E140.
void __fastcall object_projectile_model_006ea490(void* self, void*) {
    wr<const void*>(self, 0, &kotor_m15e_anim_base_projectile_vtable_00759170);    // 006EA4AD
    BaseDestructOnExit base{self};                                                 // 006EA4B3 state 0
    (void)kotor_m15e_anim_base_unload_model_0069d740(self, nullptr);               // 006EA4BB
}                                                                                  // 006EA4C2..006EA4CA state -1, 0069E140

// 006EA680..006EA69B: complete dtor, then live_free when (BYTE)flags & 1; EAX = this; RET 4.
void* __fastcall object_projectile_model_006ea680(void* self, void*, U flags) {
    object_projectile_model_006ea490(self, nullptr);                               // 006EA683
    if (static_cast<B>(flags) & 1) live_free(self);                                // 006EA688..006EA695
    return self;                                                                   // 006EA698
}

// 006EA4E0..006EA53E: LoadModel(resref, part) RET 8; `part` is not read.
U __fastcall object_projectile_model_006ea4e0(void* self, void*, void* resref, U) {
    char name[20];                                                                 // [esp+18h], 14h bytes
    gui_load_resref_copy_to_string_00405fb0(resref, nullptr, name);                // 006EA4E9..006EA4F2
    aurora_model::Gob* const body = aurora_new_aur_object_00449cc0(name, kProjectile_00759268, nullptr, nullptr);  // 006EA4F7..006EA50A
    wr<aurora_model::Gob*>(self, 0xB8, body);                                      // 006EA50F
    if (body == nullptr) return 0;                                                 // 006EA50D..006EA51C
    alignas(4) B scratch[16];                                                      // [esp+8h], not initialised by the original
    c2da_copy_00406120(reinterpret_cast<menu_resources::Ref*>(static_cast<B*>(self) + 0x2C), nullptr,
                       reinterpret_cast<menu_resources::Ref*>(scratch), static_cast<menu_resources::Ref*>(resref));  // 006EA51F..006EA528
    wr<U>(self, 0x3C, 0);                                                          // 006EA52E
    return 1;                                                                      // 006EA535
}

// 006EA550..006EA665: SetAnimation(animation, speed, a3, a4) RET 10h; EAX = 1 on every path.
// A speed equal to 0.0f (ordered; +0/-0) is replaced by 1.0f bits (TEST AH,44h / JP); NaN keeps its bits.
// The animation index is the low WORD (MOVZX), unsigned compare against 4 (JA), jump table 006EA668.
// [self+B8] is not null-checked by the original: each case dereferences it (MOV ECX,[ECX+0B8h]; MOV EAX,[ECX])
// and faults on null at the actual Gob vtable load in the selected case.
U __fastcall object_projectile_model_006ea550(void* self, void*, U animation, U speed, U a3, U a4) {
    float speed_value;
    std::memcpy(&speed_value, &speed, 4);
    if (speed_value == kZero_0073d700) speed = kOneBits;                          // 006EA550..006EA561
    const U index = static_cast<std::uint16_t>(animation);                         // 006EA569
    const char* name = nullptr;
    U flags = a3;
    switch (index) {                                                               // 006EA56E..006EA577
    case 0: name = kDefault_0073df6c; break;                                       // 006EA57E (0073DF6C)
    case 1: name = kTravel01_00759298; break;                                      // 006EA5A5
    case 2: name = kImpact01_0075928c; flags = a3 | 1u; break;                     // 006EA5CC (OR EDX,1)
    case 3: name = kThrowout_00759280; break;                                      // 006EA5F6
    case 4: name = kThrowback_00759274; break;                                     // 006EA61D
    default: name = kEmpty_0073d71c; speed = kOneBits; flags = 0; break;           // 006EA644 (push 0; push 3F800000h)
    }
    aurora_model::Gob* const g = model(self);
    gob_slot<GobPlayAnimation>(g, 0x18)(g, nullptr, word(name), speed, flags, a4); // call [eax+18h], EAX discarded
    return 1;
}

// 006EA6A0..006EA756: SetOrientation(const Vector*) RET 4.
U __fastcall object_projectile_model_006ea6a0(void* self, void*, const float* vector3) {
    if (model(self) == nullptr) return 0;                                          // 006EA6A6..006EA6AF
    if (!length_squared_exceeds(vector3, &kEpsilonBits_00747504)) return 0;         // 006EA6B1..006EA6E7
    kotor_scene3d::Vector normalized;                                              // [esp+8h]
    normalize_004ab180(&normalized, reinterpret_cast<const kotor_scene3d::Vector*>(vector3));   // 006EA6EA..006EA6F5
    const float pitch = kotor_m15a_pitch_004aa130(&normalized);                    // 006EA6F8..006EA704 (evaluated first)
    const float yaw = kotor_m15a_yaw_004aa0f0(&normalized);                        // 006EA707..006EA711
    kotor_scene3d::Quaternion q;                                                   // [esp+14h]
    const kotor_scene3d::Quaternion* r = yaw_pitch_roll_004acac0(&q, yaw, pitch, 0.0f);   // 006EA714..006EA719 (roll pushed as 0)
    aurora_model::Gob* const g1 = model(self);                                     // 006EA71E (vtable source)
    const GobSetOrientation set = gob_slot<GobSetOrientation>(g1, 0x60);
    alignas(4) B out[16];                                                          // [esp+24h], written by Gob::SetOrientation
    aurora_model::Gob* const g2 = model(self);                                     // 006EA743 (ECX re-read)
    (void)set(g2, nullptr, word(out), rd<U>(r, 0), rd<U>(r, 4), rd<U>(r, 8), rd<U>(r, 12));   // 006EA726..006EA749
    return 1;
}

}  // extern "C"
