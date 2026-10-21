#pragma once
// Lane W1 (server AI pump) portable core interface.
// Every original memory access goes through read/write ports; every original call goes through call()/vcall()
// keyed by the ORIGINAL callee address (direct calls) or the vtable byte offset (virtual calls).
// The native binding (w1_native.cpp) maps each callee address to an existing provider, to a W1 body, or to the
// explicit kotor_unreachable_trap(address). Host tests (tests/) implement the same ports with a fake memory.
#include <cstdint>
#include <initializer_list>
namespace w1 {
using U = std::uint32_t;
using S = std::int32_t;
struct Ports {
  virtual ~Ports() = default;
  virtual U read(U address) = 0;
  virtual U read16(U address) = 0;
  virtual U read8(U address) = 0;
  virtual void write(U address, U value) = 0;
  virtual void write16(U address, U value) = 0;
  virtual void write8(U address, U value) = 0;
  // Direct call of the original function at `target`. `self` is ECX for __thiscall callees (0 otherwise);
  // `args` are the stack arguments in source order (first argument = lowest stack address).
  virtual U call(U target, U self, std::initializer_list<U> args) = 0;
  // Virtual call: `call dword ptr [[self]+slot]` with ECX=self.
  virtual U vcall(U self, U slot, std::initializer_list<U> args) = 0;
  // Direct call whose EDX:EAX result is used (e.g. 005E9260 GetHighResolutionTimer).
  virtual std::uint64_t call64(U target, U self, std::initializer_list<U> args) = 0;
  // Direct cdecl/thiscall call whose ST0 (80-bit) result is stored to the 10 bytes at `out10`
  // (e.g. 004AB100 vector_magnitude). Keeps the original extended precision across the port boundary.
  virtual void call_st0(U target, U self, std::initializer_list<U> args, U out10) = 0;
  // Original-image global cells referenced by constant address in the cores (manager cells, rules column
  // indices, debug cells). The native binding maps each known constant to its reconstructed cell and traps on any
  // other constant. Plain read/write never intercept addresses: they dereference data-derived pointers only.
  virtual U global(U address) = 0;
  virtual void set_global(U address, U value) = 0;
};
inline bool slt(U a, U b) { return static_cast<S>(a) < static_cast<S>(b); }
inline bool sle(U a, U b) { return static_cast<S>(a) <= static_cast<S>(b); }
inline bool sgt(U a, U b) { return static_cast<S>(a) > static_cast<S>(b); }
inline bool sge(U a, U b) { return static_cast<S>(a) >= static_cast<S>(b); }

// Global cells read through Ports::read (mapped by the native binding).
constexpr U kAppManager = 0x7a39fc;   // CAppManager*  (+4 client, +8 server)
constexpr U kVirtualMachine = 0x7a3a00; // CVirtualMachine*
constexpr U kRules = 0x7a3a28;        // CSWRules*     (+0x34 base item array)

// .rdata literals (values read from the original image; .rdata is read-only so these are constants).
constexpr U kInvalidObject = 0x7f000000; // [0x746644]
namespace event {             // AI master event codes, .rdata 0x7441b4..
constexpr U timed = 1;        // [0x7441b8]
constexpr U destroy = 11;     // [0x7441e0]
constexpr U item_charge = 16; // [0x7441f4]
}
namespace type {              // CGameObject::m_nObjectType byte codes, .rdata 0x746630..
constexpr U module = 3;       // [0x746632]
constexpr U area = 4;         // [0x746633]
constexpr U creature = 5;     // [0x746634]
constexpr U item = 6;         // [0x746635]
constexpr U placeable = 9;    // [0x746638]
constexpr U store = 14;       // [0x74663d]
}
// CServerExoApp inline accessors (0x10-byte getters; inlined, address ownership not claimed):
inline U server(Ports& p) { return p.read(p.global(kAppManager) + 8); }
inline U server_internal(Ports& p) { return p.read(server(p) + 4); }
inline U ai_master(Ports& p) { return p.read(server_internal(p) + 0x10044); }   // 004AED80
inline U sws_message(Ports& p) { return p.read(server_internal(p) + 0x10010); } // 004AEDB0
inline U player_list(Ports& p) { return p.read(server_internal(p) + 0x10064); } // 004AED90
inline U debug_mode(Ports& p) { return p.read(server_internal(p) + 0x1006c); }  // 004AEE20
}
