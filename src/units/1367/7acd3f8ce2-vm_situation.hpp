#pragma once
// W6 tier t2. These are the VM-side pieces that sit outside the ACTION bodies.
//
// Command implementer slots (CSWVirtualMachineCommands, table 007450E4):
//   slot 4 (+10) 005B5E90 ReportError: live 0296 provider @gui_load_slot_ret8_005b5e90@16 (t3b; no W6 body)
//   slot 6 (+18) 00548360 DestroyGameDefinedStructure
//   slot 7 (+1C) 0052C510 GetEqualGameDefinedStructure
//   slot 8 (+20) 0052C140 CopyGameDefinedStructure
// and the engine-structure leaves they need that have no live owner:
//   CScriptLocation 004CA7A0 ctor / 004CA7C0 Copy / 0052BF80 Equals
//   CScriptTalent   004CA9E0 ctor / 004CAA10 Copy / 004CABB0 Equals
//   CScriptEvent    004D7270 operator==, 004D77F0 CopyScriptEvent (t2b; adapted from the reviewed
//                   retained-event-cleanup donor copy_event_004d77f0, re-checked against the disassembly)
//
// Script situations (CVirtualMachineScript, 0x28 bytes):
//   005D4380 StackPopCommand, 005D28F0 CVirtualMachineStack::CopyFromStack,
//   005D4550 ~CVirtualMachineScript, 005D48E0 DeleteScriptSituation (internal),
//   005D23E0 SetUpScriptSituation, 005D4AD0 RunScriptSituation (internal),
//   plus the CVirtualMachine wrappers 005D10E0 / 005D1110 / 005D0FD0.
//
// Existing live owners stay ports. These include ReadScriptFile 005D2260, DeleteScript 005D44F0,
// ClearStack 005D2140, RunScriptFile 005D4270 (W6 t1), CVirtualMachineScript ctor 005D1740 (0851),
// the CGameEffect/CScriptEvent ctor/copy/dtor, the CExoString primitives and Vector operator== 004AA980.
#include "8f2a5e8ea0-server_mainloop.hpp"
namespace kotor::vm_situation {
using server_mainloop::Word;using server_mainloop::Memory;using I32=std::int32_t;
struct Ports{
 virtual ~Ports()=default;
 virtual void write8(Word,Word)=0;
 virtual Word allocate_006fa7e6(Word)=0;
 virtual void free_006fa390(Word)=0;
 virtual void stack_clear_005d2140(Word stack)=0;
 virtual void string_default_005b3190(Word)=0;
 virtual void string_assign_005e5c50(Word dst,Word src)=0;
 virtual void string_destroy_005e5c20(Word)=0;
 virtual Word string_not_equal_005e5310(Word self,Word other)=0;
 virtual void script_construct_005d1740(Word)=0;
 virtual Word implementer_copy_slot8(Word implementer,Word type,Word value)=0;
 virtual I32 read_script_file_005d2260(Word vm,Word name)=0;
 virtual void delete_script_005d44f0(Word vm,Word descriptor)=0;
 virtual I32 run_script_file_005d4270(Word vm,Word pc)=0;
 // engine-structure providers
 virtual Word effect_construct_00503e40(Word self,Word flag)=0;
 virtual void effect_copy_00504090(Word self,Word source,Word flag)=0;
 virtual void effect_destroy_00503590(Word self)=0;
 virtual Word event_construct_004d7540(Word self)=0;
 // providers used by CopyScriptEvent 004D77F0 (W6-owned since t2b)
 virtual void list_allocate_005e03c0(Word list,Word count)=0;          // CExoArrayList<int/float>::Allocate
 virtual void string_list_allocate_00405080(Word list,Word count)=0;   // CExoArrayList<CExoString>::Allocate
 virtual void string_array_destroy_006fa852(Word data,Word count)=0;   // eh_vector_destructor_iterator(data,8,count,~CExoString)
 virtual void event_destroy_004d7590(Word self)=0;
 virtual Word vector_equal_004aa980(Word a,Word b)=0;
};
// ---- implementer slots ----
Word copy_game_defined_structure_0052c140(Memory&,Ports&,Word type,Word source);
Word get_equal_game_defined_structure_0052c510(Memory&,Ports&,Word type,Word a,Word b);
void destroy_game_defined_structure_00548360(Memory&,Ports&,Word type,Word value);
Word location_construct_004ca7a0(Memory&,Word self);
void location_copy_004ca7c0(Memory&,Word self,Word source);
Word location_equals_0052bf80(Memory&,Ports&,Word self,Word other);
Word talent_construct_004ca9e0(Memory&,Ports&,Word self);
void talent_copy_004caa10(Memory&,Ports&,Word self,Word source);
Word talent_equals_004cabb0(Memory&,Word self,Word other);
Word event_equal_004d7270(Memory&,Ports&,Word self,Word other);
void event_copy_004d77f0(Memory&,Ports&,Word self,Word source);
// ---- script situations ----
void copy_from_stack_005d28f0(Memory&,Ports&,Word dst,Word src,Word top_bytes,Word base_bytes);
void script_destroy_005d4550(Memory&,Ports&,Word script);
void delete_script_situation_005d48e0(Memory&,Ports&,Word script);
Word stack_pop_command_005d4380(Memory&,Ports&,Word vm,Word out);
I32 setup_script_situation_005d23e0(Memory&,Ports&,Word vm,Word script);
Word run_script_situation_005d4ad0(Memory&,Ports&,Word vm,Word script,Word object_self,Word flag);
}
