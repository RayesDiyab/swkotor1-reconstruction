#pragma once
// Lane vmC: shared helpers for the vmC NWScript engine-command handlers
// (CSWVirtualMachineCommands::ExecuteCommand*), on top of claude_vm_actions_common.hpp (not modified).
//  * Original handler: int __thiscall ExecuteCommandX(int routine, int argc) RET 8 ->
//    extern "C" int __fastcall kotor_fastcall_<addr>(void* self, void*, int routine, unsigned argc)  (@16).
//  * CSWVirtualMachineCommands: +4 run-script object valid flag (==1), +8 OBJECT_SELF id.
//  * .rdata literal facts: [0074666C] byte 0 = CGameObjectArray "found"; [00746644] 7F000000h OBJECT_INVALID.
#include "061872cb05-claude_vm_actions_common.hpp"

namespace vmc {
using namespace vmx;
inline U self_id(void* self) { return at<U>(self, 8); }
inline bool self_valid(void* self) { return at<U>(self, 4) == 1; }
// neg eax; sbb eax,eax; and eax,7D0h; add eax,FFFFF830h
inline I push_status(U pushed) { return pushed ? 0 : k_push_failed; }
inline U pop_int(I* v) { return recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, v); }
inline U pop_float(float* v) { return recovered_vm_outer_pop_float_005d1020(vm(), nullptr, v); }
inline U pop_object(U* id) { return recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, id); }
inline U pop_vector(Vector* v) { return recovered_vm_outer_pop_vector_005d1040(vm(), nullptr, v); }
inline U pop_string(ExoString* s) { return recovered_vm_outer_pop_string_005d1080(vm(), nullptr, s); }
inline U pop_engine(I type, void** out) { return recovered_vm_outer_pop_engine_structure_005d10a0(vm(), nullptr, type, out); }
inline I push_int(I v) { return push_status(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, v)); }
inline I push_float(float v) { return push_status(recovered_vm_outer_push_float_005d1030(vm(), nullptr, v)); }
inline I push_object(U id) { return push_status(recovered_vm_outer_push_object_005d10d0(vm(), nullptr, id)); }
inline I push_vector(Vector v) { return push_status(recovered_vm_outer_push_vector_005d1050(vm(), nullptr, v)); }
inline I push_string(ExoString* s) { return push_status(recovered_vm_outer_push_string_005d1090(vm(), nullptr, s)); }
inline U push_engine_raw(I type, void* value) { return recovered_vm_outer_push_engine_structure_005d10b0(vm(), nullptr, type, value); }
inline void* creature_by_id(U id) { return kotor_fastcall_004ae770(server_app(), nullptr, id); }
// CGameObject vtable casts (thiscall, no args)
inline U8 type_of(void* o) { return at<U8>(o, 8); }
inline void* as_object(void* o) { return vcall<void*>(o, 0x10); }      // AsSWSObject
inline void* as_door(void* o) { return vcall<void*>(o, 0x18); }        // AsSWSDoor
inline void* as_module(void* o) { return vcall<void*>(o, 0x20); }      // AsSWSModule
inline void* as_area(void* o) { return vcall<void*>(o, 0x28); }        // AsSWSArea
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }    // AsSWSCreature
inline void* as_trigger(void* o) { return vcall<void*>(o, 0x40); }     // AsSWSTrigger
inline void* as_placeable(void* o) { return vcall<void*>(o, 0x48); }   // AsSWSPlaceable
}  // namespace vmc
