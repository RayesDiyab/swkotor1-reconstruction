#pragma once
// W2 tier 6 (fork E1): the object-state part of CSWSCreature::SaveCreature 00500610.
//   004CEC50 CSWSObject::SaveObjectState(gff, struct) RET 8
//   004CC9D0 CSWSObject::SaveEffectList(gff, struct) RET 8
//   00503790 CGameEffect::SaveGameEffect(gff, struct) RET 8
//   00503700 CGameEffect::CopyStringFromList(CExoString* out, index) RET 8
//   0059ADB0 CSWSScriptVarTable::SaveVarTable(gff, struct) RET 8
//   0059A650 CSWSScriptVarTable::GetLocation(CScriptLocation* out, CExoString* name) RET 8
//   004CA8E0 CScriptLocation::SaveLocation(gff, struct) RET 8
//   0059B250 CSWVarTable::SaveVarTable(gff, struct) RET 8
// GFF writes go through ports in the original argument order (struct, value, label); labels are passed as the
// original .rdata address and resolved by save_object_label() (texts transcribed from swkotor.exe).
#include "1e16cfcf3d-server_mainloop.hpp"
namespace w2::save_object {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;

const char* save_object_label(Word address);   // nullptr when not one of the transcribed labels

struct Ports {
    virtual ~Ports() = default;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    // CResGFF writers (thiscall on gff). Each returns the writer's EAX.
    virtual Word gff_byte_00412620(Word gff, Word st, Word value, Word label) = 0;
    virtual Word gff_word_004126c0(Word gff, Word st, Word value, Word label) = 0;
    virtual Word gff_dword_00412760(Word gff, Word st, Word value, Word label) = 0;
    virtual Word gff_int_004127b0(Word gff, Word st, Word value, Word label) = 0;
    virtual Word gff_float_00412870(Word gff, Word st, Word bits, Word label) = 0;
    virtual Word gff_dword64_00412800(Word gff, Word st, Word lo, Word hi, Word label) = 0;
    virtual Word gff_string_00412970(Word gff, Word st, Word str, Word label) = 0;
    virtual Word gff_add_list_00412450(Word gff, Word list_out, Word parent, Word label) = 0;
    virtual Word gff_add_list_element_004124e0(Word gff, Word st_out, Word list, Word type) = 0;
    virtual Word gff_add_struct_to_struct_004125b0(Word gff, Word st_out, Word parent, Word label, Word type) = 0;
    // other providers
    virtual Word var_match_0059a390(Word table, Word name, Word type, Word create) = 0;   // W2 (w2_entry)
    virtual Word var_string_0059a590(Word table, Word out, Word name) = 0;               // W2 (w2_entry)
    virtual void str_copy_005e5b00(Word out, Word src) = 0;                               // CExoString(const CExoString&)
    virtual void str_destroy_005e5c20(Word str) = 0;
    virtual void location_construct_004ca7a0(Word loc) = 0;                               // W6 vm_situation (zeroes 6 dwords)
    virtual Word save_action_queue_004cc7e0(Word object, Word gff, Word st) = 0;          // fork E2
};

// s: scratch of the given size (stands for the original stack frame locals).
Word save_object_state_004cec50(Memory&, Ports&, Word object, Word gff, Word st, Word s /*0x80*/);
Word save_effect_list_004cc9d0(Memory&, Ports&, Word object, Word gff, Word st, Word s /*0x60*/);
Word save_game_effect_00503790(Memory&, Ports&, Word effect, Word gff, Word st, Word s /*0x28*/);
Word effect_copy_string_00503700(Memory&, Ports&, Word effect, Word out, Word index);
Word save_var_table_0059adb0(Memory&, Ports&, Word table, Word gff, Word st, Word s /*0x78*/);
Word var_get_location_0059a650(Memory&, Ports&, Word table, Word out, Word name, Word s /*0x18*/);
Word save_location_004ca8e0(Memory&, Ports&, Word loc, Word gff, Word st);
Word sw_var_table_save_0059b250(Memory&, Ports&, Word table, Word gff, Word st, Word s /*0x40*/);
}
