#pragma once
// 00692540 CSWCMapPin::CSWCMapPin -- private reconstruction contract.
//
// Original identity (read-only oracle swkotor.exe SHA-256
// 9C10E0450A6EECA417E036E3CDE7474FED1F0A92AAB018446D156944DEA91435):
//   00692540..006925AF, 0x6F body bytes, one RET 0 at 006925AE, 0x90 at 006925AF.
//   __thiscall: ECX = this, no stack arguments, EAX = this.
// Project ABI: __fastcall(self, unused EDX), decorated @8.  The original thiscall
// ECX receiver maps to the first fastcall argument and the unused EDX is the
// second; because the original function has zero stack arguments there is no
// extra dummy stack word.  The exported symbol must remain
// @recovered_client_map_pin_construct_00692540@8 so it replaces the selected W3
// trap (00543E40, W3-trap-time-restored.obj) instead of adding a second body.
//
// Object layout (Ghidra type /KotOR Types/Star Wars/Client CSWCMapPin, size 0x110;
// body size 0x6F is not the allocation size):
//   +0x000 CSWCObject base subobject, size 0xFC
//   +0x048 WORD animation = 0x2711 written by this constructor
//   +0x0FC DWORD field (MapNoteEnabled in the area-note caller)
//   +0x100 CExoString name (8 bytes; default-constructed by this constructor)
//   +0x108 int id (untouched by this constructor; caller writes it)
//   +0x10C DWORD field (script-pin flag in the area caller)
//   +0x110 end
//
// The constructor is a real C++ object construction for EH purposes: the base
// subobject's constructor can throw, and the original FuncInfo (handler 007301F8,
// FuncInfo 00780634, maxState 1) has exactly one state-0 unwind action at
// 007301F0: mov ecx,[ebp-10h]; jmp 0063E800 (~CSWCObject).  There is no separate
// CExoString destructor state because nothing after the member construct can
// throw; the original also never advances the state past 0.
#include <cstddef>
#include <cstdint>

namespace kotor_map_pin {

constexpr std::uint32_t kObjectSize = 0x110;
constexpr std::uint32_t kBaseObjectSize = 0xFC;
constexpr std::uint32_t kVtableOffset = 0x000;
constexpr std::uint32_t kAnimationWordOffset = 0x048;
constexpr std::uint32_t kFieldFcOffset = 0x0FC;
constexpr std::uint32_t kNameOffset = 0x100;
constexpr std::uint32_t kIdOffset = 0x108;
constexpr std::uint32_t kField10cOffset = 0x10C;
constexpr std::uint16_t kAnimationWord = 0x2711;
// .rdata 0074663B is the immutable CSWCObject type byte for a waypoint.  The
// original loads it with xor eax,eax / mov al,byte [0074663B] at 00692556..55C.
constexpr std::uint32_t kWaypointObjectType_0074663B = 0x0C;

// Private layout view used by the compile-time checks and the tests.  It is not
// a polymorphic C++ class: the original vptr is written explicitly at +0 because
// the canonical reconstructed CSWCMapPin table is a separate data object.
struct StorageView {
    const void* vtable;                 // +0x000
    unsigned char base_tail[0xF8];      // +0x004..+0x0FB (within the 0xFC base)
    std::uint32_t field_fc;             // +0x0FC
    unsigned char name[8];              // +0x100..+0x107 (CExoString storage)
    std::int32_t id;                    // +0x108
    std::uint32_t field_10c;            // +0x10C
};

static_assert(sizeof(void*) == 4, "the original 00692540 ABI is 32-bit x86");
static_assert(sizeof(StorageView) == kObjectSize, "CSWCMapPin must occupy 0x110 bytes");
static_assert(offsetof(StorageView, vtable) == kVtableOffset, "vptr offset");
static_assert(offsetof(StorageView, field_fc) == kFieldFcOffset, "field +FC offset");
static_assert(offsetof(StorageView, name) == kNameOffset, "CExoString offset");
static_assert(offsetof(StorageView, id) == kIdOffset, "id offset");
static_assert(offsetof(StorageView, field_10c) == kField10cOffset, "field +10C offset");
static_assert(offsetof(StorageView, base_tail) == sizeof(const void*),
              "base tail starts after the vptr");
static_assert(offsetof(StorageView, base_tail) + 0x44 == kAnimationWordOffset,
              "animation word +48h lies inside the base subobject");

}  // namespace kotor_map_pin

extern "C" {

// 0063E530 CSWCObject::CSWCObject(BYTE object_type), selected real provider
// @kotor_m15a_object_construct_0063e530@12 at 0047E8C0 [0817-m15a-object.obj].
// ECX = this, one stack dword (the type), RET 4, EAX = this.
void* __fastcall kotor_m15a_object_construct_0063e530(
    void* self, void* unused_edx, std::uint32_t object_type) noexcept(false);

// 005B3190 CExoString default constructor, selected real provider
// @g11_exostring_default@8 at 005FC2E0 [0012-startup-strings.obj].
// ECX = destination CExoString, no stack arguments, RET 0.
void* __fastcall g11_exostring_default(void* self, void* unused_edx) noexcept(false);

// 0063E800 CSWCObject::~CSWCObject, selected real provider
// @kotor_m15e_object_destruct_0063e800@8 at 00545DE0 [0831.obj].
// This is not a normal-path callee: it is the original state-0 unwind action
// reached from 007301F0, and the reconstruction uses the same receiver.
void __fastcall kotor_m15e_object_destruct_0063e800(void* self, void* unused_edx) noexcept(false);

// Canonical CSWCMapPin vtable, original 00754598..007546DC (0x148 bytes, 82
// slots).  The original constructor stores this address at object +0
// (00692578: mov dword [esi],00754598).  No selected data provider currently
// defines this table; see INTEGRATION.md "Unresolved canonical data edge".
extern const void* const recovered_client_map_pin_methods_00754598[82];

// 00692540 CSWCMapPin::CSWCMapPin, projected to the project __fastcall ABI.
void* __fastcall recovered_client_map_pin_construct_00692540(
    void* self, void* unused_edx) noexcept(false);

}  // extern "C"
