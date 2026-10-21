// W7 tier t7: GetLastAttacker (36), reconstructed from the original swkotor.exe (read-only, capstone 5.0.7).
// Live providers: CVirtualMachine::StackPopObject 005D10C0 / StackPushObject 005D10D0 (1196), CServerExoApp::
// GetGameObject 004AE750 (0530).
#include "a51007811a-vm_actions_t7.hpp"
#include <cstdint>
#include <cstring>
#if !defined(_M_IX86)
#error W7 ACTION bodies are native x86 (32-bit guest pointers)
#endif
namespace frame_control { extern unsigned message_manager_007a3a00; }
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
unsigned __fastcall recovered_vm_outer_pop_object_005d10c0(void*, void*, void*);
unsigned __fastcall recovered_vm_outer_push_object_005d10d0(void*, void*, unsigned);
void* __fastcall kotor_fastcall_004ae750(void*, void*, unsigned);                            // CServerExoApp::GetGameObject
}
namespace {
using U = std::uint32_t;
void* ptr(U a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
U vcall0(U obj, U slot) { using F = U(__fastcall*)(void*, void*); return reinterpret_cast<F>(ptr(rd(rd(obj) + slot)))(ptr(obj), nullptr); }
}
extern "C" {
// 0053A880 GetLastAttacker(object oAttackee): AsSWSObject()+15C of the object, else OBJECT_INVALID. Pop failure
// -2001; push failure -2000. (argc is not consulted: the object is always popped.)
int __fastcall recovered_vm_get_last_attacker_0053a880(void*, void*, int, unsigned) {
  void* vm = ptr(frame_control::message_manager_007a3a00);
  U id = 0x7f000000u, result = 0x7f000000u;
  if (!recovered_vm_outer_pop_object_005d10c0(vm, nullptr, &id)) return -2001;
  const U o = word(kotor_fastcall_004ae750(ptr(rd(word(kotor_menu_window_cells::manager_007a39fc) + 8)), nullptr, id));
  if (o && vcall0(o, 0x10)) result = rd(vcall0(o, 0x10) + 0x15c);
  return recovered_vm_outer_push_object_005d10d0(vm, nullptr, result) ? 0 : -2000;
}
}
