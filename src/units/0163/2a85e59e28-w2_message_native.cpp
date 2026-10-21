// W2 native exports for the CSWMessage primitive family.
// Original ABI: thiscall (ECX=this) with callee stack cleanup; exported as
// __fastcall with an unused EDX word, the project's existing convention.
// "veneer" = new ABI entry that delegates to the existing single owner of the
// address (no duplicate body, no reconstructed-address credit).
#include "2346527c32-w2_native_ports.hpp"
using namespace w2::native;
namespace sm = kotor::server_mainloop;
namespace {
struct ResRef16 { unsigned char bytes[16]; };
static_assert(sizeof(ResRef16) == 16, "CResRef by-value size");
}
extern "C" {
// ---- readers ----
void __fastcall kotor_fastcall_004d6170(void* self, void*) { Mem m; w2::msg::clear_read_004d6170(m, word(self)); }
void* __fastcall kotor_fastcall_004d6180(void* self, void*, void* result, unsigned size) {
    Mem m; MessagePorts p(m);
    alignas(4) unsigned char scratch[0x20] = {};   // original local is not initialized; bytes past `size` are out of domain
    return ptr(w2::msg::read_resref_004d6180(m, p, word(self), word(result), size, word(scratch)));
}
void* __fastcall kotor_fastcall_004d6200(void* self, void*, unsigned size) { Mem m; return ptr(w2::msg::read_voidptr_004d6200(m, word(self), size)); }
unsigned __fastcall kotor_fastcall_004d6230(void* self, void*, unsigned) { return recovered_player_message::overrun_004d6230(self); }   // veneer -> 0917
unsigned __fastcall kotor_fastcall_004d6240(void* self, void*, unsigned) { return recovered_player_message::underrun_004d6240(self); }  // veneer -> 0917
unsigned __fastcall kotor_fastcall_004d6250(void* self, void*) { Mem m; return w2::msg::more_data_004d6250(m, word(self)); }
void __fastcall kotor_fastcall_004d66b0(void* self, void*, void* data, unsigned size, unsigned field4c, unsigned field48) {
    recovered_player_message::initialize_read_004d66b0(self, data, size, field4c, field48);                           // veneer -> 0917
}
unsigned __fastcall kotor_fastcall_004d66f0(void* self, void*) { Mem m; return w2::msg::read_bool_004d66f0(m, word(self)); }
unsigned char __fastcall kotor_fastcall_004d6720(void* self, void*, unsigned bits) { Mem m; return static_cast<unsigned char>(w2::msg::read_byte_004d6720(m, word(self), bits)); }
unsigned short __fastcall kotor_fastcall_004d6750(void* self, void*, unsigned bits) { Mem m; return static_cast<unsigned short>(w2::msg::read_word16_004d6750(m, word(self), bits)); }
unsigned __fastcall kotor_fastcall_004d6780(void* self, void*, unsigned bits) { Mem m; return sm::message_read_word_004d6780(m, word(self), bits); } // veneer -> 0935
float __fastcall kotor_fastcall_004d67b0(void* self, void*, unsigned scale, unsigned bits) {
    Mem m; const Word value = w2::msg::read_float_bits_004d67b0(m, word(self), scale, bits);
    float f; std::memcpy(&f, &value, 4); return f;                 // returned in ST0 like the original fld
}
void* __fastcall kotor_fastcall_004d67e0(void* self, void*, void* result, unsigned bits) {
    Mem m; MessagePorts p(m); alignas(4) unsigned char scratch[8] = {};
    return ptr(w2::msg::read_string_004d67e0(m, p, word(self), word(result), bits, word(scratch)));
}
// ---- writers ----
void __fastcall kotor_fastcall_004d6260(void* self, void*, unsigned extra) { Mem m; MessagePorts p(m); sm::message_grow_004d6260(m, p, word(self), extra); } // veneer -> 0898
void __fastcall kotor_fastcall_004d62c0(void* self, void*, unsigned value, unsigned bits) { Mem m; MessagePorts p(m); w2::msg::write_byte_004d62c0(m, p, word(self), value, bits); }
void __fastcall kotor_fastcall_004d6300(void* self, void*, unsigned value, unsigned bits) { Mem m; MessagePorts p(m); w2::msg::write_word16_004d6300(m, p, word(self), value, bits); }
void __fastcall kotor_fastcall_004d6340(void* self, void*, unsigned value, unsigned bits) { Mem m; MessagePorts p(m); sm::message_word_004d6340(m, p, word(self), value, bits); } // veneer -> 0898
void __fastcall kotor_fastcall_004d6380(void* self, void*, ResRef16 ref, unsigned size) {
    if (size > 16) trap(0x004d6380, "WriteCResRef size above 16 reads the caller frame (out of domain)");
    Mem m; MessagePorts p(m); w2::msg::write_resref_004d6380(m, p, word(self), word(&ref), size);
}
void __fastcall kotor_fastcall_004d63e0(void* self, void*, const void* source, unsigned size) { Mem m; MessagePorts p(m); w2::msg::write_voidptr_004d63e0(m, p, word(self), word(source), size); }
unsigned __fastcall kotor_fastcall_004d6440(void* self, void*, void** output, unsigned* size) { Mem m; return sm::message_finish_004d6440(m, word(self), word(output), word(size)); } // veneer -> 0898
void __fastcall kotor_fastcall_004d68a0(void* self, void*, unsigned size, unsigned field4c, unsigned field48) { Mem m; MessagePorts p(m); sm::message_begin_004d68a0(m, p, word(self), size, field4c, field48); } // veneer -> 0898
void __fastcall kotor_fastcall_004d68e0(void* self, void*, unsigned value) { Mem m; MessagePorts p(m); w2::msg::write_bool_004d68e0(m, p, word(self), value); }
void __fastcall kotor_fastcall_004d6920(void* self, void*, unsigned value_bits, unsigned scale, unsigned bits) { Mem m; MessagePorts p(m); w2::msg::write_float_bits_004d6920(m, p, word(self), value_bits, scale, bits); }
void __fastcall kotor_fastcall_004d6960(void* self, void*, const void* str, unsigned bits) { Mem m; MessagePorts p(m); w2::msg::write_string_004d6960(m, p, word(self), word(str), bits); }
// ---- CSWSMessage / CSWCMessage leaves ----
void __fastcall kotor_fastcall_00578570(void* self, void*, unsigned id) { Mem m; MessagePorts p(m); w2::msg::write_object_id_00578570(m, p, word(self), id); }
unsigned __fastcall kotor_fastcall_00692630(void* self, void*) { Mem m; return w2::msg::read_object_id_00692630(m, word(self)); }
unsigned __fastcall kotor_fastcall_00578540(void* self, void*) { Mem m; return w2::msg::read_object_id_server_00578540(m, word(self)); }
void __fastcall kotor_fastcall_005785c0(void* self, void*, void* loc, unsigned gender) {
    Mem m; MessagePorts p(m); alignas(4) unsigned char scratch[0x14] = {};
    w2::msg::write_locstring_005785c0(m, p, word(self), word(loc), gender, word(scratch));
}
void* __fastcall kotor_fastcall_00692660(void* self, void*, void* result, void* sound, unsigned* length_out) {
    Mem m; MessagePorts p(m); alignas(4) unsigned char scratch[0x44] = {};
    return ptr(w2::msg::read_locstring_00692660(m, p, word(self), word(result), word(sound), word(length_out), word(scratch)));
}
unsigned __fastcall kotor_fastcall_00567e60(void* self, void*, unsigned recipient, unsigned major, unsigned minor, void* data, unsigned size) {
    Mem m; MessagePorts p(m); return sm::message_send_00567e60(m, p, word(self), recipient, major, minor, word(data), size); // veneer -> 0898
}
unsigned __fastcall kotor_fastcall_004b4050(void* internal, void*, unsigned player_id) {
    Mem m; MessagePorts p(m); Word slot = 0; return w2::msg::player_language_004b4050(m, p, word(internal), player_id, word(&slot));
}
unsigned __fastcall kotor_fastcall_005ea240(void* loc, void*, unsigned language, void* out, unsigned gender) {
    Mem m; MessagePorts p(m); return w2::msg::get_string_loc_005ea240(m, p, word(loc), language, word(out), gender);
}
}
