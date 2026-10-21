#pragma once
// lane-m15-a (L15-A, Milestone 1.5): shared helpers for the L15-A units.
//   m15a-classsel   (m15a_classsel.cpp)   OnNewGamePicked post-guard body (lifts #71), CSWGuiClassSelection,
//                                         CSWGuiClassSelChar, method table 00758020   [header m15a_gui.h]
//   m15a-creature   (m15a_creature.cpp)   CSWCCreature members (ctor/SetAppearance/Setup3DScene/CGPauseCycle ...),
//                                         CSWCCombatAttackData, CSWCSpellCastVisual, Yaw/Pitch [header m15a_creature.h]
//   m15a-object     (m15a_object.cpp)     CSWCObject members, CSWCItem powered anims, SwitchWeaponEvent
//                                                                                   [header m15a_object.h]
//   m15a-appearance (m15a_appearance.cpp) CSWCCreatureAppearance                     [header m15a_appearance.h]
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only; every body cites its
// instruction addresses.
//
// Conventions (same as lane-m15-0 / lane-m15-b; DECISIONS D1/D21/D24): every original __thiscall entry is an
// extern "C" __fastcall(Self*, void* unused_edx, <original stack words>) veneer named
// kotor_m15a_<class>_<method>_<addr> with identical callee cleanup (RET N noted per entry); callers pass nullptr
// for EDX; no callee reads EDX. Object pointers are void* (C linkage: @name@<bytes>). By-value struct returns
// use the original hidden-return pointer as the first stack argument and return it in EAX.
// Object views: the client objects (CSWCObject 0x?, CSWCCreature 0x44C, CSWCCreatureAppearance 0x6C) are opaque
// byte blocks; only offsets cited at the access site are touched, through ld/st (memcpy; no type punning,
// exactly one load/store per original access -- D132).
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace kotor_m15a {
using Word = std::uint32_t;
using I32 = std::int32_t;
using U16 = std::uint16_t;
using U8 = std::uint8_t;

template <class T> inline T ld(const void* base, unsigned offset) {
    T value;
    std::memcpy(&value, static_cast<const unsigned char*>(base) + offset, sizeof value);
    return value;
}
template <class T> inline void st(void* base, unsigned offset, T value) {
    std::memcpy(static_cast<unsigned char*>(base) + offset, &value, sizeof value);
}
// Pointer / dword loads under the composer EXTRA-DEREF-SCAN helper spellings (m60-compose/tools/extraderef.py
// HELPERS: rp, rd32): every pointer load uses rp() so nested chains rp(rp(x, a), b) are counted against the
// original load count of the address cited in the line comment.
inline void* rp(const void* base, unsigned offset) { return ld<void*>(base, offset); }
inline Word rd32(const void* base, unsigned offset) { return ld<Word>(base, offset); }
// Address of an embedded member (lea reg,[base+offset]).
inline void* at(void* base, unsigned offset) { return static_cast<unsigned char*>(base) + offset; }
inline const void* at(const void* base, unsigned offset) { return static_cast<const unsigned char*>(base) + offset; }

struct Vector { float x, y, z; };                  // Vector (12 bytes)
struct Quaternion { float x, y, z, w; };           // Quaternion (16 bytes)
static_assert(sizeof(Vector) == 12 && sizeof(Quaternion) == 16, "vector sizes");

// Opaque sizes established by operator new at the construction sites.
constexpr unsigned kCreatureSize = 0x44C;          // 006DC965 push 44Ch (CSWGuiClassSelection ctor)
constexpr unsigned kAppearanceSize = 0x6C;         // 00616BF7 push 6Ch (CSWCCreature ctor)
constexpr unsigned kLevelUpStatsSize = 0x134;      // 00616ACC push 134h
constexpr unsigned kCombatAttackDataSize = 0x48;   // 00616B1B / 00616B86 push 48h
constexpr unsigned kSpellCastVisualSize = 0x30;    // 00616CCD push 30h
}

// ---------------------------------------------------------------------------------------------------------
// Method tables stored by the L15-A constructors but owned by lane L15-E (slot closure owner; SLOT-AUDIT.json
// of lane-m15-f): declared here with an incomplete tag so only their ADDRESS is taken (never a raw original
// address, never a load through this declaration). L15-E defines them with these exact names and tags as
// complete typed constant tables (D21):
//   extern "C" const KotorM15CreatureMethods_0074f5f8 kotor_m15e_creature_methods_0074f5f8;  (88 slots)
//   extern "C" const KotorM15ObjectMethods_007516c8   kotor_m15e_object_methods_007516c8;
// Slot calls read the receiver's vptr through a typed slot view at the call site (D24 object view).
struct KotorM15CreatureMethods_0074f5f8;
#include "5de39a25ea-m15e.h"   // canonical per-table type; this header takes its address only
extern "C" const KotorM15CreatureMethods_0074f5f8 kotor_m15e_creature_methods_0074f5f8;   // stored 00616A51
extern "C" const KotorM15ObjectMethods_007516c8 kotor_m15e_object_methods_007516c8;       // stored 0063E562
