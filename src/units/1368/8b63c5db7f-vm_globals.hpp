#pragma once
// W6 tier t3: Global* ACTION bodies and the CSWGlobalVariableTable value accessors they reach.
//   ACTION 194     GetGlobalString         -> 00538ED0      ACTION 160     SetGlobalString         -> 00542C60
//   ACTION 692     GetGlobalLocation       -> 00538CD0      ACTION 693     SetGlobalLocation       -> 00542A50
//   CSWGlobalVariableTable (server internal +100FC, via CServerExoApp::GetGlobalVariableTable 004AEE60):
//   GetValueLocation 00529350, GetValueString 00529460, SetValueLocation 00529790, SetValueString 005298A0.
// Coordinator ruling (OWNERSHIP.md): 578-581 (00538DC0/00542B60) and SetBit/Get/SetValueBoolean/Number are W7's.
// Existing owners stay ports: FindIdentifierIndex 00529030 (0855 recovered_global_catalog), the table constructor
// 0052B170 and ReadCatalogue 0052AE50 (server-create), the VM stack (1196/1200/W6 1194), CExoString.
// Table layout (all original offsets from the table base T):
//   T+0      1775 x 24-byte identifier slots: char name[22], uint16 code = (category<<14) | index
//   T+A8D0   locations (100 x 24 bytes)                     T+B230  strings (5 x CExoString)
// The original also Formats a diagnostic CExoString on a missing or mistyped identifier and then destroys it.
// That has no observable effect and is not reproduced (no output sink exists in the original either).
#include "8f2a5e8ea0-server_mainloop.hpp"
namespace kotor::vm_globals {
using server_mainloop::Word;using server_mainloop::Memory;using I32=std::int32_t;
struct Ports{
 virtual ~Ports()=default;
 virtual Word vm_internal()=0;                       // [[007A3A00]]
 virtual Word global_table()=0;                      // [[[007A39FC]+8]+4]+100FC
 virtual I32 find_identifier_00529030(Word table,Word name)=0;
 virtual Word pop_string_005d25e0(Word vm,Word dst)=0;
 virtual Word push_string_005d1930(Word vm,Word src)=0;
 virtual Word push_engine_structure_005d19c0(Word vm,Word type,Word value)=0;
 virtual Word pop_engine_structure_005d2630(Word vm,Word type,Word* out)=0;
 virtual void string_default_005b3190(Word)=0;
 virtual void string_destroy_005e5c20(Word)=0;
 virtual void string_assign_005e5c50(Word dst,Word src)=0;
 virtual void free_006fa390(Word)=0;
};
// caller-provided guest scratch: two 8-byte CExoStrings, one 24-byte location, one pointer cell
struct Scratch{Word name,value,location,cell;};
constexpr I32 kPushFailed=-2000,kPopFailed=-2001;
void get_value_location_00529350(Memory&,Ports&,Word table,Word name,Word out_location);
void get_value_string_00529460(Memory&,Ports&,Word table,Word name,Word out_string);
void set_value_location_00529790(Memory&,Ports&,Word table,Word name,Word location);
void set_value_string_005298a0(Memory&,Ports&,Word table,Word name,Word value);
I32 get_global_string_00538ed0(Memory&,Ports&,const Scratch&);
I32 set_global_string_00542c60(Memory&,Ports&,const Scratch&);
I32 get_global_location_00538cd0(Memory&,Ports&,const Scratch&);
I32 set_global_location_00542a50(Memory&,Ports&,const Scratch&);
}
