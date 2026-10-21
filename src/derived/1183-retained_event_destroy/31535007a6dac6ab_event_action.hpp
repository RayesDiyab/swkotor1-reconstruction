#pragma once
#include "f1c22f5a49c0731f_memory16.hpp"
#include "3991cb1894612357_events.hpp"
#include "f2cbd6ed8f6d9259_event_integer.hpp"
#include "aab0ebebb5cf71a5_engine_push.hpp"
namespace kotor::scene_event {
using module::Word;
using character_load::Memory16;
// Existing allocation, list growth, string and non-event owners remain ports.
// No defaults, fabricated copy or fallback success is supplied.
struct Services:module::EventStorage {
    virtual Word allocate_006fa7e6(Word size)=0;
    virtual void destruct_string_array_006fa852(Word data,Word stride,Word count,Word destructor)=0;
    virtual void allocate_string_list_00405080(Word list,Word count)=0;
    virtual void assign_string_005e5c50(Word destination,Word source)=0;
    virtual Word construct_effect_00503e40(Word self,Word flag)=0;
    virtual void copy_effect_00504090(Word self,Word source,Word flag)=0;
    virtual Word construct_location_004ca7a0(Word self)=0;
    virtual void copy_location_004ca7c0(Word self,Word source)=0;
    virtual Word construct_talent_004ca9e0(Word self)=0;
    virtual void copy_talent_004caa10(Word self,Word source)=0;
};
// Normal-flow donors. Mapped live memory and original compatible allocation
// services are required. No native SEH, original ABI or allocation-fault parity.
void copy_event_004d77f0(Memory16&,Services&,Word destination,Word source);
void destroy_event_004d7590(Memory16&,Services&,Word self);
Word copy_engine_structure_0052c140(Memory16&,Services&,Word type,Word source);
struct Binding {
    Memory16& memory;
    Services& services;
    vm_leaf::Machine** live_vm;
    vm_push::Allocator stack_allocator;
    vm_engine_push::CopyEngineStructure copy_slot20;
};
std::int32_t event_user_defined_00535a90(const Binding&,Word routine,Word count);
}
