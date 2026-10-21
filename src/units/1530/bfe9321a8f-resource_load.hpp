#pragma once
#include "5aaf728098-memory.hpp" // Existing character-load-common owner, pinned by binder.
namespace kotor::vm_executor {
using Word=character_load::Word;
using Memory=character_load::Memory;
// Guest handles only. Existing CResRef, CRes, resource manager and string bodies
// remain sole providers. Scratch addresses designate caller-owned mapped storage.
struct ResourcePorts {
 virtual ~ResourcePorts()=default;
 virtual void resref_from_chars_00406d80(Word dst,Word chars)=0;
 virtual void resref_from_string_00406d60(Word dst,Word string)=0;
 virtual Word resref_equal_004060b0(Word lhs,Word rhs)=0;
 virtual Word resref_not_chars_00406060(Word lhs,Word chars)=0;
 virtual Word resref_equal_chars_004060d0(Word lhs,Word chars)=0;
 virtual void resref_assign_chars_00406290(Word dst,Word result,Word chars)=0;
 virtual void resref_assign_00406120(Word dst,Word result,Word rhs)=0;
 virtual void resource_cancel_00409b00(Word resource)=0;
 virtual Word resource_remove_00409cf0(Word manager,Word resource)=0;
 virtual void resource_virtual_destroy(Word resource,Word flags)=0;
 virtual Word resource_find_004074d0(Word manager,Word ref,Word type)=0;
 virtual void resource_register_00407680(Word manager,Word ref,Word type,Word resource)=0;
 virtual void resource_service_00408620(Word resource)=0;
 virtual Word allocate_006fa7e6(Word bytes)=0;
 virtual void resource_construct_00406460(Word self)=0;
 virtual Word resource_demand_00409b10(Word resource)=0;
 virtual Word resource_data_00404dd0(Word resource)=0;
 virtual Word resource_size_00710e50(Word resource)=0;
 virtual void resource_release_00409b80(Word resource)=0;
 virtual void string_assign_005e5c50(Word dst,Word src)=0;
};
struct ResourceScratch { Word resref, assignment_result; };
Word construct_ncs_005d4c30(Memory&,ResourcePorts&,Word self);
void select_resource_005d1ac0(Memory&,ResourcePorts&,Word internal,Word ref,Word flag,ResourceScratch);
void copy_script_005d16f0(Memory&,ResourcePorts&,Word descriptor,Word data,Word size);
Word load_script_005d2260(Memory&,ResourcePorts&,Word internal,Word script,ResourceScratch);
}
