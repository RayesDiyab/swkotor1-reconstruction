#pragma once
// W2 lane: CSWMessage / CSWSMessage / CSWCMessage read/write primitives.
// Logic layer over original 32-bit address words (same convention as
// item_ai_recharge / module_message). All original offsets are kept:
//   +04 write buffer  +08 write capacity  +0C write index  +10 second buffer
//   +1C written-byte counter  +24 writing flag
//   +28 read buffer   +2C read size       +30 read index
//   +38/+3C reset by SetReadMessage  +44/+45 bytes  +48/+4C metadata
// KotOR's CSWMessage is byte-aligned: there is no bit packer. The "bits"
// stack arguments of the Write*/Read* family are accepted and ignored by the
// original (verified per function; see receipts).
//
// Owner reconciliation (single owner per address):
//   004D6260 ExtendWriteBuffer, 004D68A0 CreateWriteMessage, 004D6340
//   WriteDWORD, 004D6440 GetWriteMessage, 00567E60 SendServerToPlayerMessage
//     -> existing kotor::server_mainloop module_message owner (0898).
//   004D66B0 SetReadMessage, 004D6230 Overflow, 004D6240 Underflow
//     -> existing recovered_player_message owner (0917).
//   004D6780 ReadDWORD -> existing client_loadbar owner (0935).
// This header only declares the bodies W2 owns; callers that need the shared
// ones call the existing owners directly.
#include "4512f576a5-server_mainloop.hpp"
#include "3965539a67-module_message.hpp"
namespace w2::msg {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;
using kotor::server_mainloop::ModuleMessagePorts;

// Ports: ModuleMessagePorts supplies allocate(006FA7E6)/free(006FA390)/
// write_byte/network_getter/send(005D4CB0). Everything else is an existing
// original-address owner bound by the native layer.
struct Ports : ModuleMessagePorts {
    virtual void copy(Word dst, Word src, Word count) = 0;           // REP MOVSD/MOVSB forward copy
    virtual Word str_length_005e5790(Word str) = 0;
    virtual Word str_cstr_005e5670(Word str) = 0;
    virtual void str_literal_005e5a90(Word str, Word text) = 0;
    virtual void str_counted_005e5b70(Word str, Word text, Word count) = 0;
    virtual void str_copy_005e5b00(Word dst, Word src) = 0;
    virtual void str_destroy_005e5c20(Word str) = 0;
    virtual void ref_assign_text_00406290(Word self, Word result, Word text) = 0;
    virtual void ref_assign_raw_00406170(Word self, Word result, Word source) = 0;
    virtual Word empty_text_0073d71c() = 0;                         // original "" literal
};

// --- readers (CSWMessage) ---
void clear_read_004d6170(Memory&, Word self);
// ReadCResRef: hidden result + size (signed, clamped to 16). scratch: 32 bytes.
Word read_resref_004d6180(Memory&, Ports&, Word self, Word result, Word size, Word scratch);
Word read_voidptr_004d6200(Memory&, Word self, Word size);
Word more_data_004d6250(Memory&, Word self);
Word read_bool_004d66f0(Memory&, Word self);
Word read_byte_004d6720(Memory&, Word self, Word bits);
Word read_word16_004d6750(Memory&, Word self, Word bits);
// ReadFLOAT returns the IEEE single bits the original loads into ST0
// (overflow: +0.0 from 0073D700). The x87 load quiets an SNaN; the native
// export reproduces that by returning through ST0.
Word read_float_bits_004d67b0(Memory&, Word self, Word scale, Word bits);
// ReadCExoString: hidden result + bits. scratch: 8 bytes (temporary CExoString).
Word read_string_004d67e0(Memory&, Ports&, Word self, Word result, Word bits, Word scratch);
// --- writers (CSWMessage) ---
void write_byte_004d62c0(Memory&, Ports&, Word self, Word value, Word bits);
void write_word16_004d6300(Memory&, Ports&, Word self, Word value, Word bits);
// WriteCResRef(CResRef by value, size): `ref` is the address of the 16-byte
// by-value copy. Domain: size <= 16 (the original copies `size` bytes from the
// argument copy; larger values read the caller frame and are out of domain).
void write_resref_004d6380(Memory&, Ports&, Word self, Word ref, Word size);
void write_voidptr_004d63e0(Memory&, Ports&, Word self, Word source, Word size);
void write_bool_004d68e0(Memory&, Ports&, Word self, Word value);
// WriteFLOAT(value, scale, bits): stores fld/fstp of value (SNaN quieted).
void write_float_bits_004d6920(Memory&, Ports&, Word self, Word value_bits, Word scale, Word bits);
void write_string_004d6960(Memory&, Ports&, Word self, Word str, Word bits);
// --- CSWSMessage / CSWCMessage object id leaves ---
void write_object_id_00578570(Memory&, Ports&, Word self, Word id);
Word read_object_id_00692630(Memory&, Word self);
// 00578540 CSWSMessage::ReadOBJECTIDServer: client->server reader (W5 input minors 03h/0Bh)
Word read_object_id_server_00578540(Memory&, Word self);
// x87 fld/fstp single-precision round trip (SNaN -> QNaN).
Word x87_single_roundtrip(Word bits);

// --- localized strings ---
struct LocPorts : Ports {
    virtual Word list_at_005e9b60(Word list, Word position) = 0;
    virtual Word list_next_005e9b90(Word list, Word position_slot) = 0;
    virtual Word locinternal_get_005ec9c0(Word internal, Word index, Word out) = 0;
    virtual Word get_string_005e9eb0(Word loc, Word index, Word language_out, Word text_out, Word gender_out) = 0;
    virtual void str_empty_005b3190(Word str) = 0;
    virtual Word server() = 0;                                                // [[007A39FC]+8]
    // client side
    virtual Word tlk_fetch_0041e550(Word tlk, Word strref, Word out_entry, Word flag) = 0;
    virtual Word tlk() = 0;                                                   // [007A3A08]
    virtual void ref_default_00405ed0(Word ref) = 0;
    virtual void ref_assign_lower_00406120(Word self, Word result, Word source) = 0;
};
// 004B4050 CServerExoAppInternal::GetPlayerLanguage(player id) (004AE810 thunk: internal=[server+4]).
Word player_language_004b4050(Memory&, LocPorts&, Word internal, Word player_id, Word scratch4);
// 004AE820 CServerExoApp::GetModuleLanguage tail-jumps to 0063E7F0 return_zero.
inline Word module_language_004ae820(Word) { return 0; }
// 005EA240 CExoLocString::GetStringLoc(language, CExoString* out, gender byte)
Word get_string_loc_005ea240(Memory&, LocPorts&, Word loc, Word language, Word out, Word gender);
// WriteCExoLocStringServer(loc*, gender). scratch: 0x14 bytes.
void write_locstring_005785c0(Memory&, LocPorts&, Word self, Word loc, Word gender, Word scratch);
// ReadCExoLocStringClient(hidden result, CResRef* sound_out, DWORD* sound_length_out nullable).
// scratch: 0x44 bytes.
Word read_locstring_00692660(Memory&, LocPorts&, Word self, Word result, Word sound, Word strref_out, Word scratch);
}
