#pragma once
// Lane vmA: shared helpers for the vmA NWScript engine-command package (CSWVirtualMachineCommands::ExecuteCommand*).
// Builds on claude_vm_actions_common.hpp (unchanged).  Every handler: original int __thiscall (int routine, int argc),
// RET 8 -> extern "C" int __fastcall kotor_fastcall_<addr>(void* self, void* edx, int routine, unsigned argc) (@16).
// CSWVirtualMachineCommands: +4 "run-script caller valid" flag, +8 OBJECT_SELF id of the running script.
// .rdata facts used as literals: [0074666C] byte 0 = CGameObjectArray "found"; [00746633] byte 4 = object type area;
// [00746644] 7F000000h = OBJECT_INVALID.
#include "061872cb05-claude_vm_actions_common.hpp"

extern "C" {
// ---- CGameEffect providers (decorated arity checked against the original RET) ----
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);        // CGameEffect::CGameEffect RET 4 [0886-cohort-patched]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);  // SetInteger RET 8 [0886-cohort-patched]
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);             // SetCreator RET 4 [0886-cohort-patched]
void __fastcall recovered_effect_destroy_00503590(void* effect, void*);                   // ~CGameEffect [w1_native_b7_0056cbf0]
void __fastcall recovered_effect_resize_00503630(void* effect, void*, U count);           // SetNumIntegers RET 4 [1091-effect_copy_native]
void __fastcall recovered_effect_set_object_id_005036f0(void* effect, void*, U index, U id); // SetObjectID RET 8 [1194u]
void __fastcall kotor_fastcall_005036d0(void* effect, void*, U index, U value_bits);      // SetFloat RET 8 [claude_w1_chains_damage]
}

namespace vma {
using namespace vmx;
constexpr U k_type_area_00746633 = 4;
inline U self_id(void* self) { return at<U>(self, 8); }
inline U type_of(void* o) { return at<U8>(o, 8); }
inline void* as_object(void* o) { return vcall<void*>(o, 0x10); }      // vtbl+10 AsSWSObject
inline void* as_door(void* o) { return vcall<void*>(o, 0x18); }        // vtbl+18 AsSWSDoor
inline void* as_module(void* o) { return vcall<void*>(o, 0x20); }      // vtbl+20 AsSWSModule
inline void* as_area(void* o) { return vcall<void*>(o, 0x28); }        // vtbl+28 AsSWSArea
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }    // vtbl+30 AsSWSCreature
inline void* as_trigger(void* o) { return vcall<void*>(o, 0x40); }     // vtbl+40 AsSWSTrigger
inline void* as_placeable(void* o) { return vcall<void*>(o, 0x48); }   // vtbl+48 AsSWSPlaceable
// neg/sbb/and 7D0h/add F830h
inline I push_status(U pushed) { return pushed ? 0 : k_push_failed; }
inline U pop_object(U* id) { return recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, id); }
inline U pop_int(I* v) { return recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, v); }
inline U pop_float(float* v) { return recovered_vm_outer_pop_float_005d1020(vm(), nullptr, v); }
inline U pop_string(ExoString* s) { return recovered_vm_outer_pop_string_005d1080(vm(), nullptr, s); }
inline U pop_struct(I type, void** out) { return recovered_vm_outer_pop_engine_structure_005d10a0(vm(), nullptr, type, out); }
inline I push_object(U id) { return push_status(recovered_vm_outer_push_object_005d10d0(vm(), nullptr, id)); }
inline I push_int(I v) { return push_status(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, v)); }
inline I push_float(float v) { return push_status(recovered_vm_outer_push_float_005d1030(vm(), nullptr, v)); }
inline I push_string(ExoString* s) { return push_status(recovered_vm_outer_push_string_005d1090(vm(), nullptr, s)); }
// CGameEffect helpers (operator new 8Ch + CGameEffect(1); a null allocation is not checked by the originals)
inline void* new_effect() {
    void* memory = live_operator_new(0x8c);
    return memory ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr;
}
inline void set_effect_type(void* e, U type) { at<U16>(e, 8) = static_cast<U16>(type); }
// xor r,r; mov r16,[e+0A]; and r,FFEF; or r,8; mov [e+0A],r16  (SetSubType magical)
inline void set_subtype_magical(void* e) { at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xffefu) | 8u); }
// StackPushEngineStructure(0, e): failure -> -2000 (effect leaked, original quirk); success -> ~CGameEffect + free, 0
inline int push_effect(void* e) {
    if (!recovered_vm_outer_push_engine_structure_005d10b0(vm(), nullptr, 0, e)) return k_push_failed;
    recovered_effect_destroy_00503590(e, nullptr);
    live_free(e);
    return 0;
}
}  // namespace vma
