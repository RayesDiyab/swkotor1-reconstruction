#pragma once
// W7 tier t1: NWScript engine-command (ACTION) bodies CSWVirtualMachineCommands::ExecuteCommand* at their
// original addresses, plus the unowned leaves they reach. Native x86 only (pointers are 32-bit guest words).
// Every ACTION export has the original command-table ABI: __thiscall(this, int routine, int argc), RET 8,
// modelled as __fastcall(ecx=this, edx unused, routine, argc). Return: 0 ok, -2001 pop failure, -2000 push failure.
#include <cstdint>
extern "C" {
// ---- ACTION bodies (command table entries) ----
int __fastcall recovered_vm_set_listening_00542fb0(void* self, void*, int routine, unsigned argc);            // 175 SetListening
int __fastcall recovered_vm_set_listen_string_00543030(void* self, void*, int routine, unsigned argc);        // 176 SetListenPattern
int __fastcall recovered_vm_set_local_value_00543130(void* self, void*, int routine, unsigned argc);         // 680 SetLocalBoolean, 682 SetLocalNumber
int __fastcall recovered_vm_get_local_value_0053b760(void* self, void*, int routine, unsigned argc);         // 679 GetLocalBoolean, 681 GetLocalNumber
int __fastcall recovered_vm_get_is_object_valid_0053c720(void* self, void*, int routine, unsigned argc);     // 42 GetIsObjectValid
int __fastcall recovered_vm_int_to_string_0053ebc0(void* self, void*, int routine, unsigned argc);          // 92 IntToString
int __fastcall recovered_vm_get_pc_0053c920(void* self, void*, int routine, unsigned argc);                 // 548 GetFirstPC, 549 GetNextPC
int __fastcall recovered_vm_get_user_defined_event_number_0053e220(void* self, void*, int routine, unsigned argc); // 247

// ---- leaves with no previous live owner ----
// CVirtualMachine outer forwarders (mov ecx,[ecx]; jmp internal) -- same family as 1196's 005D1010/10C0/10D0.
unsigned __fastcall recovered_vm_outer_pop_integer_005d1000(void* vm, void*, void* out);
unsigned __fastcall recovered_vm_outer_pop_string_005d1080(void* vm, void*, void* string);
unsigned __fastcall recovered_vm_outer_push_string_005d1090(void* vm, void*, void* string);
// CSWVarTable (3 DWORD boolean bitset + 8 byte numbers at +0C)
int __fastcall recovered_var_table_get_boolean_0059b000(void* table, void*, int index);
void __fastcall recovered_var_table_set_boolean_0059b040(void* table, void*, int index, int value);
unsigned char __fastcall recovered_var_table_get_number_0059b0b0(void* table, void*, int index);
void __fastcall recovered_var_table_set_number_0059b0d0(void* table, void*, int index, int value);
// CSWSObject listen expressions (CExoArrayList<CSWSExpression*> at +18C)
void* __fastcall recovered_object_get_listen_expression_004cd6c0(void* object, void*, int number);
void __fastcall recovered_object_set_listen_expression_004cf8f0(void* object, void*, void* string, int number);
// CSWSExpression / CSWSExpressionList / CSWSExpressionNode
struct W7ExoStringValue { char* data; std::uint32_t allocated; };   // CExoString passed by value (8 bytes)
void* __fastcall recovered_expression_construct_0059d990(void* expression, void*);
int __fastcall recovered_expression_parse_string_0059dab0(void* expression, void*, W7ExoStringValue string);
void* __fastcall recovered_expression_new_node_0059d860(void* expression, void*, int type);
void __fastcall recovered_expression_list_add_node_0059d820(void* list, void*, void* node);
void __fastcall recovered_expression_list_delete_list_0059d920(void* list, void*);
void __fastcall recovered_expression_list_delete_alternate_0059d7d0(void* list, void*, void* node);
// Player list (donor gog-dialogue first_pc_action.cpp @bb622021, re-verified against the original)
unsigned __fastcall recovered_server_first_pc_004aea10(void* server, void*);
unsigned __fastcall recovered_server_next_pc_004aea20(void* server, void*);
unsigned __fastcall recovered_server_internal_first_pc_004b4c50(void* internal, void*);
unsigned __fastcall recovered_server_internal_next_pc_004b4c80(void* internal, void*);
}
