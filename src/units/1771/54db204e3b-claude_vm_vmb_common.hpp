#pragma once
// Lane vmB: shared helpers for the vmB NWScript engine-command handlers
// (CSWVirtualMachineCommands::ExecuteCommand*), reconstructed from the original swkotor.exe (read-only).
//  * Original: int __thiscall ExecuteCommandX(int routine, int argc), RET 8.
//    Export:   extern "C" int __fastcall kotor_fastcall_<addr>(void* self, void*, int routine, unsigned argc)  (@16)
//  * CSWVirtualMachineCommands: +4 "OBJECT_SELF valid" flag (1), +8 OBJECT_SELF id.
//  * Builds on claude_vm_actions_common.hpp (not modified).  Return literals: 0 / -2001 / -2000.
//  * .rdata facts used as literals: [0074666C] byte 0 = CGameObjectArray "found"; [00746644] 7F000000h OBJECT_INVALID.
#include "061872cb05-claude_vm_actions_common.hpp"

extern "C" {
// ---- CGameEffect providers (decorated arity checked against the original RET) ----
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);        // CGameEffect::CGameEffect RET 4 [0886-cohort-patched]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);  // SetInteger RET 8 [0886-cohort-patched]
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);             // SetCreator RET 4 [0886-cohort-patched]
void __fastcall recovered_effect_destroy_00503590(void* effect, void*);                   // ~CGameEffect [w1_native_b7_0056cbf0]
void __fastcall recovered_effect_resize_00503630(void* effect, void*, U count);           // SetNumIntegers RET 4 [1091]
}

namespace vmb {
using namespace vmx;
// neg/sbb/and 7D0h/add F830h
inline I push_status(U pushed) { return pushed ? 0 : k_push_failed; }
inline U pop_object(U* id) { return recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, id); }
inline U pop_int(I* v) { return recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, v); }
inline U pop_float(float* v) { return recovered_vm_outer_pop_float_005d1020(vm(), nullptr, v); }
inline U pop_vector(Vector* v) { return recovered_vm_outer_pop_vector_005d1040(vm(), nullptr, v); }
inline U pop_string(ExoString* s) { return recovered_vm_outer_pop_string_005d1080(vm(), nullptr, s); }
inline U pop_engine(I type, void** out) { return recovered_vm_outer_pop_engine_structure_005d10a0(vm(), nullptr, type, out); }
inline U push_object_raw(U id) { return recovered_vm_outer_push_object_005d10d0(vm(), nullptr, id); }
inline U push_int_raw(I v) { return recovered_vm_outer_push_integer_005d1010(vm(), nullptr, v); }
inline U push_float_raw(float v) { return recovered_vm_outer_push_float_005d1030(vm(), nullptr, v); }
inline U push_string_raw(ExoString* s) { return recovered_vm_outer_push_string_005d1090(vm(), nullptr, s); }
inline U push_vector_raw(Vector v) { return recovered_vm_outer_push_vector_005d1050(vm(), nullptr, v); }
inline U push_engine_raw(I type, void* value) { return recovered_vm_outer_push_engine_structure_005d10b0(vm(), nullptr, type, value); }
inline I push_object(U id) { return push_status(push_object_raw(id)); }
inline I push_int(I v) { return push_status(push_int_raw(v)); }
inline I push_float(float v) { return push_status(push_float_raw(v)); }

inline U self_id(void* self) { return at<U>(self, 8); }
inline U self_valid(void* self) { return at<U>(self, 4); }
inline U type_of(void* o) { return at<U8>(o, 8); }
inline void* as_object(void* o) { return vcall<void*>(o, 0x10); }      // vtbl+10 AsSWSObject
inline void* as_door(void* o) { return vcall<void*>(o, 0x18); }        // vtbl+18 AsSWSDoor
inline void* as_module(void* o) { return vcall<void*>(o, 0x20); }      // vtbl+20 AsSWSModule
inline void* as_area(void* o) { return vcall<void*>(o, 0x28); }        // vtbl+28 AsSWSArea
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }    // vtbl+30 AsSWSCreature
inline void* as_item(void* o) { return vcall<void*>(o, 0x38); }        // vtbl+38 AsSWSItem
inline void* as_trigger(void* o) { return vcall<void*>(o, 0x40); }     // vtbl+40 AsSWSTrigger
inline void* as_placeable(void* o) { return vcall<void*>(o, 0x48); }   // vtbl+48 AsSWSPlaceable
inline void* creature_by_id(U id) { return kotor_fastcall_004ae770(server_app(), nullptr, id); } // 004AE770

// operator new(8Ch) + CGameEffect(1); a null allocation yields a null effect the caller then dereferences (quirk)
inline void* new_effect() {
    void* memory = live_operator_new(0x8c);
    return memory ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr;
}
inline void set_effect_type(void* e, U type) { at<U16>(e, 8) = static_cast<U16>(type); }
// xor r,r; mov r16,[e+0A]; and r,FFEF; or r,8; mov [e+0A],r16  (magical sub-type)
inline void set_subtype_magical(void* e) { at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xffefu) | 8u); }
inline void delete_effect(void* e) { recovered_effect_destroy_00503590(e, nullptr); live_free(e); }
// StackPushEngineStructure(0, e): failure -> -2000 (effect leaked); success -> ~CGameEffect + free, 0
inline I push_effect(void* e) {
    if (!push_engine_raw(0, e)) return k_push_failed;
    delete_effect(e);
    return 0;
}
// CScriptLocation dtor 004015A0 is a bare RET (no-op); callers then free 006FA390.
inline void location_dtor_004015a0(void*) {}
}  // namespace vmb
