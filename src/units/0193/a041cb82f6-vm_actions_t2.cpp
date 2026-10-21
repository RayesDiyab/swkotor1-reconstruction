// W7 tier t2: ExecuteScript / GetRunScriptVar and the script-situation ACTIONs (AssignCommand, DelayCommand,
// ActionDoCommand), reconstructed from the original swkotor.exe (read-only, capstone 5.0.7).
// Native x86 translation unit (32-bit guest pointers). MEMORY: only live_operator_new / live_free.
//
// Ownership boundaries (see INTEGRATION.md):
//  * CVirtualMachine::StackPopCommand 005D10E0 is W6's (tiers/t2, unreviewed): imported as
//    @recovered_vm_stack_pop_command_005d10e0@12 with a named-trap fallback (/alternatename).
//  * CSWSObject::AddDoCommandAction 0057CB10 belongs to the server action queue (W1 domain; it ends in
//    CSWSObject::AddAction 004CEA20): imported as @recovered_object_add_do_command_action_0057cb10@12 with a
//    named-trap fallback.
//  * CServerAIMaster::AddEventDeltaTime 004B08D0 is the live 1017 export.
//  * CVirtualMachine::RunScript 005D0FC0 is called through its existing 0457 owner @frame_R4_005d0fc0@20, which
//    forwards (via the 0783 forwarder @kotor_fastcall_005d45d0@20) to the live 0886 CVirtualMachineInternal::RunScript
//    @recovered_vm_run_script_005d45d0@20. (t2-t4 inlined the forwarder and wrongly described 0783 as a trap.)
//  * CServerExoApp::GetServerAIMaster 004AED80 (two loads) is inlined like 004AED70 (no exported owner exists).
#include "73dbba025f-vm_actions_t1.hpp"
#include "6cdf8f62ff-vm_actions_t2.hpp"
#include <cstring>
#if !defined(_M_IX86)
#error W7 ACTION bodies are native x86 (32-bit guest pointers)
#endif
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }
namespace frame_control { extern unsigned message_manager_007a3a00; }
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
void __cdecl live_free(void*);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long address, const char* reason);
unsigned __fastcall recovered_vm_pop_float_005d2510(void*, void*, void*);              // 1196
unsigned __fastcall recovered_vm_outer_pop_object_005d10c0(void*, void*, void*);       // 1196
unsigned __fastcall recovered_vm_outer_push_integer_005d1010(void*, void*, unsigned);  // 1196
unsigned char __fastcall kotor_fastcall_004d8230(void*, void*, unsigned, void**);      // 0457 CGameObjectArray::GetGameObject
unsigned __fastcall recovered_event_enqueue_004b08d0(void*, void*, unsigned, unsigned, unsigned, unsigned, unsigned, void*); // 1017
unsigned __fastcall frame_R4_005d0fc0(void*, void*, void*, unsigned, unsigned);           // 0457 CVirtualMachine::RunScript
void __fastcall frame_LogStringDtor_005e5c20(void*, void*);                            // 0459
// imports owned by other lanes, with named-trap fallbacks (w7_missing_t2.cpp)
int __fastcall recovered_vm_stack_pop_command_005d10e0(void* vm, void*, void* out_script);
int __fastcall recovered_object_add_do_command_action_0057cb10(void* object, void*, void* script);
int __fastcall w7_missing_stack_pop_command_005d10e0(void*, void*, void*);
int __fastcall w7_missing_add_do_command_action_0057cb10(void*, void*, void*);
}
#pragma comment(linker, "/alternatename:@recovered_vm_stack_pop_command_005d10e0@12=@w7_missing_stack_pop_command_005d10e0@12")
#pragma comment(linker, "/alternatename:@recovered_object_add_do_command_action_0057cb10@12=@w7_missing_add_do_command_action_0057cb10@12")

extern "C" {
// .bss cell 00832828: the nScriptVar of the last ExecuteScript (only 00535C13 writes it, only 0053D020 reads it)
std::int32_t recovered_run_script_var_00832828 = 0;
}

namespace {
using U = std::uint32_t;
using I = std::int32_t;
constexpr U kInvalidObject = 0x7f000000u;   // [00746644]
constexpr U kEventScriptSituation = 1;      // [007441B8]
constexpr I kPopFailed = -2001, kPushFailed = -2000;
void* ptr(U a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
void* vm() { return ptr(frame_control::message_manager_007a3a00); }
U server() { return rd(word(kotor_menu_window_cells::manager_007a39fc) + 8); }
U object_array_004aed70(U s) { return rd(rd(s + 4) + 0x1005c); }       // inline, see vm_actions_t1.cpp
U ai_master_004aed80(U s) { return rd(rd(s + 4) + 0x10044); }          // mov eax,[ecx+4]; mov eax,[eax+10044]
bool lookup(U id, U& object) {                                          // CGameObjectArray::GetGameObject == 0 ([0074666C])
  void* out = nullptr;
  const bool found = kotor_fastcall_004d8230(ptr(object_array_004aed70(server())), nullptr, id, &out) == 0;
  object = word(out);
  return found;
}
// 0052E789 / 0052FEA5: the caller is the script owner when the implementer's owner is valid, else OBJECT_INVALID
U caller(U self) { return rd(self + 4) == 1 ? rd(self + 8) : kInvalidObject; }
// 006FAE8C __ftol2: FISTP m64 (current rounding), then a correction to truncation toward zero; the result is the low
// dword of the 64-bit integer. NaN, infinities and values outside the int64 range store the integer indefinite
// 8000000000000000h, whose low dword is 0.
U ftol2_low(double v) {
  if (!(v >= -9223372036854775808.0 && v < 9223372036854775808.0)) return 0;
  return static_cast<U>(static_cast<std::uint64_t>(static_cast<std::int64_t>(v)));   // C++ conversion truncates toward zero
}
// (float delay) * 1000.0f on the x87 stack: the float*float product is exact in double (24+24 significant bits)
U delay_ms(U bits) {
  float f; std::memcpy(&f, &bits, 4);
  return ftol2_low(static_cast<double>(f) * 1000.0);
}
}

extern "C" {
// 005D1020: mov ecx,[ecx]; jmp CVirtualMachineInternal::StackPopFloat
unsigned __fastcall recovered_vm_outer_pop_float_005d1020(void* v, void*, void* out) { return recovered_vm_pop_float_005d2510(ptr(rd(word(v))), nullptr, out); }

// 00535B70 ExecuteScript(string, object, int nScriptVar = -1)
int __fastcall recovered_vm_execute_script_00535b70(void* self, void*, int, unsigned argc) {
  W7ExoStringValue name;
  (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&name), nullptr);
  U target = 0;
  if (!recovered_vm_outer_pop_string_005d1080(vm(), nullptr, &name) || !recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &target)) {
    frame_LogStringDtor_005e5c20(&name, nullptr);
    return kPopFailed;
  }
  if (static_cast<I>(argc) >= 3) {                                   // 00535BC4 cmp [esp+24],3 / jl
    U var = 0xffffffffu;
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &var)) { frame_LogStringDtor_005e5c20(&name, nullptr); return kPopFailed; }
    recovered_run_script_var_00832828 = static_cast<I>(var);
  }
  if (rd(word(self) + 4) == 1) (void)frame_R4_005d0fc0(vm(), nullptr, &name, target, 1);
  else (void)frame_R4_005d0fc0(vm(), nullptr, &name, kInvalidObject, 0);
  frame_LogStringDtor_005e5c20(&name, nullptr);
  return 0;
}
// 0053D020 GetRunScriptVar
int __fastcall recovered_vm_get_run_script_var_0053d020(void*, void*, int, unsigned) {
  return recovered_vm_outer_push_integer_005d1010(vm(), nullptr, static_cast<U>(recovered_run_script_var_00832828)) ? 0 : kPushFailed;
}
// 0052E720 AssignCommand(object, action): an AI event of type 1 carrying the popped script situation, due now.
// When the target id is unknown the situation is released with a bare _free (original; its contents leak).
int __fastcall recovered_vm_assign_command_0052e720(void* self, void*, int, unsigned) {
  U target = 0, script = 0;
  if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &target)) return kPopFailed;
  if (!recovered_vm_stack_pop_command_005d10e0(vm(), nullptr, &script)) return kPopFailed;
  U object = 0;
  if (lookup(target, object)) {
    (void)recovered_event_enqueue_004b08d0(ptr(ai_master_004aed80(server())), nullptr, 0, 0, caller(word(self)), target, kEventScriptSituation, ptr(script));
    return 0;
  }
  if (script) live_free(ptr(script));
  return 0;
}
// 0052FE30 DelayCommand(float seconds, action): AI event for the script owner, due after ftol(seconds*1000) ms.
int __fastcall recovered_vm_delay_command_0052fe30(void* self, void*, int, unsigned) {
  U seconds = 0, script = 0;
  if (!recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &seconds)) return kPopFailed;
  if (!recovered_vm_stack_pop_command_005d10e0(vm(), nullptr, &script)) return kPopFailed;
  const U s = word(self);
  U object = 0;
  if (rd(s + 4) == 1 && lookup(rd(s + 8), object)) {
    (void)recovered_event_enqueue_004b08d0(ptr(ai_master_004aed80(server())), nullptr, 0, delay_ms(seconds), caller(s), rd(s + 8), kEventScriptSituation, ptr(script));
    return 0;
  }
  if (script) live_free(ptr(script));
  return 0;
}
// 0052C740 ActionDoCommand(action): CSWSObject::AddDoCommandAction on the script owner. When the owner id is
// unknown the situation is not released (original leak).
int __fastcall recovered_vm_action_do_command_0052c740(void* self, void*, int, unsigned) {
  U script = 0;
  if (!recovered_vm_stack_pop_command_005d10e0(vm(), nullptr, &script)) return kPopFailed;
  U object = 0;
  if (lookup(rd(word(self) + 8), object)) {
    if (!object) kotor_unreachable_trap(0x0052c79d, "ActionDoCommand: object array returned success with a null object (original calls through null)");
    (void)recovered_object_add_do_command_action_0057cb10(ptr(object), nullptr, ptr(script));
  }
  return 0;
}
}
