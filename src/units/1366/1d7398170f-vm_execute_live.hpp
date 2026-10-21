#pragma once
// W6 tier t1: owner 1194 (vm_execute_live), extended from the authenticated
// 25-pair subset to the complete CVirtualMachineInternal::ExecuteCode 005D2BD0
// dispatch (44 jump-table entries 005D41B4, every opcode/type branch), plus the
// VM stack helpers it calls that had no live owner:
//   005D18B0 StackPushVector, 005D2560 StackPopVector,
//   005D19C0 StackPushEngineStructure, 005D16C0 modifyStackInteger.
// Existing live owners stay ports: 005D1850/1880/1930/1A10 pushes,
// 005D24C0/2510/25E0 pops, 005D1C40 copy, 005D21F0 SetStackPointer,
// 005D15A0 AddToTopOfStack, CExoString 005E5A90/5B3190/5E5C20/5E5D10/5E5680/
// 5E5670/5E6450, vector helpers 00423BF0/00437A90, implementer vtable slots.
#include "8f2a5e8ea0-server_mainloop.hpp"
#include <stdexcept>
namespace kotor::vm_execute_live {
using server_mainloop::Word;using server_mainloop::Memory;using I32=std::int32_t;
// Not an original VM status. Thrown only where the ORIGINAL executable faults or
// corrupts memory (integer #DE, negative CONST-string length, modifyStackInteger
// beyond the stack allocation). `address` is the original instruction address.
struct CoverageStop:std::runtime_error{Word address;explicit CoverageStop(const char*m,Word a=0):std::runtime_error(m),address(a){}};
struct Ports {
 virtual ~Ports()=default;
 // kind 3 int (005D1850/005D24C0), 4 float (005D1880/005D2510), 6 object push (005D1A10)
 virtual Word push(Word vm,Word value,Word kind)=0;
 virtual Word pop(Word vm,Word* output,Word kind)=0;
 virtual void trim(Word stack,I32 depth)=0;                 // 005D21F0 SetStackPointer
 virtual void copy(Word vm,I32 source,I32 destination)=0;   // 005D1C40 on vm+0x1AC
 virtual Word push_string(Word vm,Word string)=0;           // 005D1930
 virtual Word pop_string(Word vm,Word string)=0;            // 005D25E0
 virtual void byte(Word,Word)=0;
 virtual void string_default_005b3190(Word)=0;
 virtual void string_chars_005e5a90(Word,Word)=0;
 virtual Word string_cstr_005e5670(Word)=0;
 virtual void string_destroy_005e5c20(Word)=0;
 virtual Word string_concat_005e5d10(Word lhs,Word result,Word rhs)=0;
 virtual void string_format_005e5680(Word,Word,Word)=0;
 virtual Word allocate_006fa7e6(Word)=0;
 virtual void free_006fa390(Word)=0;
 // x87: FLD m32 / FCOMP m64 0.0 / FNSTSW; true when exactly one of C3,C2 is set.
 virtual bool divisor_is_zero_x87(Word)=0;
 virtual I32 action_slot2(Word implementer,I32 routine,Word count)=0;
 virtual void error_slot4(Word implementer,Word name,Word negated)=0;
 // ---- added for full coverage (tier t1) ----
 virtual void add_to_top_005d15a0(Word stack,Word type)=0;
 virtual Word compare_no_case_005e6450(Word self,Word other)=0;
 virtual Word implementer_equal_slot7(Word implementer,Word type,Word top,Word lower)=0;  // vtbl+1C
 virtual Word implementer_copy_slot8(Word implementer,Word type,Word value)=0;             // vtbl+20
 // FLD m32 lhs; F<op> m32 rhs; FSTP m32 (op 0x14 add,0x15 sub,0x16 mul,0x17 div)
 virtual Word float_binary_x87(Word op,Word lhs,Word rhs)=0;
 // FLD lhs; FCOMP rhs; FNSTSW; op 0x0D GEQ / 0x0E GT / 0x0F LT / 0x10 LEQ exactly as 005D356A..
 virtual Word float_compare_x87(Word op,Word lhs,Word rhs)=0;
 virtual Word float_negate_x87(Word)=0;      // FLD; FCHS; FSTP
 virtual Word int_to_float_x87(Word)=0;      // FILD m32; FSTP m32
 virtual Word float_reload_x87(Word)=0;      // FLD m32; FSTP m32 (StackPopVector z)
 virtual void vector_component_mult_00423bf0(Word out,Word vector,Word scalar)=0;
 virtual void vector_divide_00437a90(Word out,Word vector,Word scalar)=0;
 // Notified with the decoded ACTION before slot2 dispatch (diagnostic only; no state effect).
 // Per-instruction observation hook (default no-op; host differential tests only).
};
// Guest scratch: three 8-byte CExoStrings, three 12-byte vectors.
struct Scratch{Word string_a,string_b,string_result;Word vector_a=0,vector_b=0,vector_result=0;};
struct Runtime{Memory&memory;Word internal;Ports&ports;Scratch scratch;};
Word push_return_005d17f0(Runtime&,Word);
Word pop_return_005d17b0(Runtime&,Word&);
Word push_vector_005d18b0(Runtime&,Word x,Word y,Word z);
Word pop_vector_005d2560(Runtime&,Word out);
Word push_engine_structure_005d19c0(Runtime&,Word type,Word value);
// t3: 005D2630 StackPopEngineStructure (out receives an implementer slot-8 copy; the stack copy is destroyed)
Word pop_engine_structure_005d2630(Runtime&,Word type,Word& out);
void modify_stack_integer_005d16c0(Runtime&,I32 index,Word delta);
I32 execute_code_005d2bd0(Runtime&,Word& pc,Word code,Word size);
I32 run_script_file_005d4270(Runtime&,Word);
struct DispatchPorts{virtual ~DispatchPorts()=default;virtual I32 invoke(Word,Word,I32,Word)=0;};
I32 execute_command_0052c0d0(Memory&,DispatchPorts&,Word,I32,Word);
// ACTION metadata (names from the audited command-table census; not game bytes).
const char* action_name(I32 routine);
Word action_handler(I32 routine); // original handler address stored by 0054C960, 0 if none
}
