#pragma once
// W2 tier 6 (fork E2): action-queue and VM script-situation save (CSWSCreature::SaveCreature chain).
//   004CC7E0 CSWSObject::SaveActionQueue(CResGFF*, CResStruct*) RET 8
//   005D10F0 CVirtualMachine::SaveScriptSituation(situation, CResGFF*, CResStruct*) RET 0Ch  (ecx = [ecx]; jmp 005D47A0)
//   005D47A0 CVirtualMachineInternal::SaveScriptSituation(situation, CResGFF*, CResStruct*) RET 0Ch, EAX = 1
//   005D1D80 CVirtualMachineStack::SaveStack(CResGFF*, CResStruct*) RET 8, EAX = 1
// GFF labels are passed as their original .rdata addresses; the ports map them to live text.
// Layouts (as read by the original):
//   action node (CExoLinkedList at object+FCh): +0 ActionId, +4+4k param type k, +38h+4k param value k,
//     +6Ch WORD GroupActionId, +6Eh WORD NumParams. Param types: 1 INT, 2 FLOAT, 3 OBJECT (DWORD), 4 STRING
//     (value = CExoString*), 5 script situation (value = situation*). Other types write only "Type".
//   situation: +0 stack*, +4 StackSize, +8 InstructionPtr, +0Ch SecondaryPtr, +10h code*, +14h code size,
//     +18h CExoString name, +24h CRC (zeroed by the save).
//   VM internal: +28h script index, script entry k at +2Ch+28h*k (+10h code*, +14h code size).
//   VM stack: +0 StackPointer, +4 BasePointer, +8 TotalSize, +0Ch BYTE types[], +10h DWORD values[],
//     +14h VM (its +3D4h is the engine-structure command implementer; vtbl+24h saves an engine structure).
//     Stack types: 3 INT, 4 FLOAT, 5 STRING (CExoString*), 6 OBJECT (DWORD); any other type (signed BYTE)
//     goes to the engine-structure saver with type-10h.
#include "1e16cfcf3d-server_mainloop.hpp"
namespace w2::save_actions {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;

struct Ports {
    virtual ~Ports() = default;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    virtual Word literal(Word original_address) = 0;                                 // .rdata label -> live text
    // 0873 CResGFF writers (thiscall on gff) and the t6 fork-A siblings
    virtual Word add_list_00412450(Word gff, Word out_list, Word parent, Word label) = 0;
    virtual Word add_list_element_004124e0(Word gff, Word out_struct, Word list, Word type) = 0;
    virtual Word write_char_00412670(Word gff, Word st, Word value, Word label) = 0;
    virtual Word write_word_004126c0(Word gff, Word st, Word value, Word label) = 0;
    virtual Word write_dword_00412760(Word gff, Word st, Word value, Word label) = 0;
    virtual Word write_int_004127b0(Word gff, Word st, Word value, Word label) = 0;
    virtual Word write_float_00412870(Word gff, Word st, Word bits, Word label) = 0;
    virtual Word write_string_00412970(Word gff, Word st, Word str, Word label) = 0;
    virtual Word write_void_00412c10(Word gff, Word st, Word data, Word size, Word label) = 0;
    virtual Word add_struct_to_struct_004125b0(Word gff, Word out_struct, Word parent, Word label, Word type) = 0;
    // CExoLinkedList
    virtual Word list_at_pos_005e9b60(Word list, Word pos) = 0;
    virtual void list_next_005e9b90(Word list, Word pos_slot) = 0;
    // VM
    virtual Word vm_007a3a00() = 0;                                                    // value of the CVirtualMachine* cell
    virtual Word read_script_file_005d2260(Word vm_internal, Word name_str) = 0;       // W6
    virtual void delete_script_005d44f0(Word vm_internal, Word script) = 0;            // W6
    virtual void save_engine_structure(Word implementer, Word type, Word value, Word gff, Word st) = 0;   // [[impl]+24h]
};

// s: >= 0x60 bytes of scratch (the original 34h locals plus its two argument slots, which it reuses as structs;
// s + 40h is the nested situation-save scratch).
void save_action_queue_004cc7e0(Memory&, Ports&, Word object, Word gff, Word parent, Word s);
// s: >= 0x20 bytes of scratch.
Word vm_save_situation_005d10f0(Memory&, Ports&, Word vm, Word situation, Word gff, Word st, Word s);
Word vm_internal_save_situation_005d47a0(Memory&, Ports&, Word vm_internal, Word situation, Word gff, Word st, Word s);
// s: >= 0x18 bytes of scratch (list at s, element struct at s + 14h = the original's argument slot).
Word vm_stack_save_005d1d80(Memory&, Ports&, Word stack, Word gff, Word st, Word s);
}
