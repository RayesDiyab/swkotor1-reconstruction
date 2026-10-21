// W2 tier 6 fork A: native exports for the CResGFF write primitives (thiscall as __fastcall(self, edx, args...)).
// Ports bind the EXISTING Memory-model writer exactly as 0873 gff_writer_native_live.cpp does: game CRT heap,
// gff_resource_release_004108c0, gff_owned_storage_00410aa0, gff_add_struct_00410d60, gff_add_field_00411730,
// gff_grow_buffer (sections 0 / 2). Ports the six primitives never reach trap explicitly.
#if !defined(_M_IX86)
#error W2 native bindings require the Windows x86 route
#endif
#include "88c689318a-w2_save_gff.hpp"
#include "e509e74b2b-gff_write_fields.hpp"
#include <cstdint>
#include <cstring>
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
void __fastcall gff_resource_release_004108c0(void*, void*);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
}
namespace {
using kotor::server_mainloop::Word;
using namespace kotor::server_mainloop;
void* ptr(Word v) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(v)); }
Word word(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
struct M final : Memory {
    Word read(Word a) override { Word v; std::memcpy(&v, ptr(a), 4); return v; }
    Word read8(Word a) override { return *static_cast<unsigned char*>(ptr(a)); }
    Word read16(Word a) override { unsigned short v; std::memcpy(&v, ptr(a), 2); return v; }
    void write(Word a, Word v) override { std::memcpy(ptr(a), &v, 4); }
    void write16(Word a, Word v) override { const auto s = static_cast<unsigned short>(v); std::memcpy(ptr(a), &s, 2); }
};
[[noreturn]] void unused(const char* what) { kotor_unreachable_trap(0x00412870, what); }
struct P final : GffWriteComplexPorts {
    M& m; explicit P(M& v) : m(v) {}
    Word allocate(Word a) override { return word(live_operator_new(a)); }
    void free(Word a) override { live_free(ptr(a)); }
    void release_004108c0(Word a) override { gff_resource_release_004108c0(ptr(a), nullptr); }
    void byte(Word a, Word b) override { *static_cast<unsigned char*>(ptr(a)) = static_cast<unsigned char>(b); }
    void owned(Word a) override { gff_owned_storage_00410aa0(m, *this, a); }
    Word add_struct(Word a, Word b) override { return gff_add_struct_00410d60(m, *this, a, b); }
    Word add_field(Word a, Word b, Word c, Word d) override { return gff_add_field_00411730(m, *this, a, b, c, d); }
    Word grow_data(Word a, Word b, Word c, Word d) override { return gff_grow_buffer(m, *this, a, b, c, d, 0); }
    Word grow_list(Word a, Word b, Word c, Word d) override { return gff_grow_buffer(m, *this, a, b, c, d, 2); }
    // not reached by the W2 primitives (string / list-read / localized helpers of the other writers)
    Word string_literal(Word, Word) override { unused("W2 GFF primitive port string_literal not used"); }
    void string_destroy(Word) override { unused("W2 GFF primitive port string_destroy not used"); }
    Word string_length(Word) override { unused("W2 GFF primitive port string_length not used"); }
    Word string_char(Word, Word) override { unused("W2 GFF primitive port string_char not used"); }
    Word field_index(Word, Word, Word) override { unused("W2 GFF primitive port field_index not used"); }
    Word field(Word, Word, Word) override { unused("W2 GFF primitive port field not used"); }
    Word data(Word, Word, Word) override { unused("W2 GFF primitive port data not used"); }
    Word ref_length(Word) override { unused("W2 GFF primitive port ref_length not used"); }
    Word ref_text(Word) override { unused("W2 GFF primitive port ref_text not used"); }
    Word string_text(Word) override { unused("W2 GFF primitive port string_text not used"); }
    void string_construct(Word) override { unused("W2 GFF primitive port string_construct not used"); }
    Word localized_count(Word) override { unused("W2 GFF primitive port localized_count not used"); }
    Word localized_length(Word, Word) override { unused("W2 GFF primitive port localized_length not used"); }
    Word localized_get(Word, Word, Word, Word, Word) override { unused("W2 GFF primitive port localized_get not used"); }
};
}
namespace g = w2::save_gff;
extern "C" {
// 00412870 CResGFF::WriteFieldFLOAT(CResStruct*, float, char const*) RET 0Ch
Word __fastcall recovered_gff_00412870(void* self, void*, void* root, float value, const char* label) {
    M m; P p(m); Word bits; std::memcpy(&bits, &value, 4); return g::write_float_00412870(m, p, word(self), word(root), bits, word(label));
}
// 00412800 CResGFF::WriteFieldDWORD64(CResStruct*, unsigned __int64, char const*) RET 10h
Word __fastcall recovered_gff_00412800(void* self, void*, void* root, unsigned long long value, const char* label) {
    M m; P p(m);
    return g::write_dword64_00412800(m, p, word(self), word(root), static_cast<Word>(value), static_cast<Word>(value >> 32), word(label));
}
// 00412C10 CResGFF::WriteFieldVOID(CResStruct*, void*, unsigned long, char const*) RET 10h
Word __fastcall recovered_gff_00412c10(void* self, void*, void* root, void* data, Word size, const char* label) {
    M m; P p(m); return g::write_void_00412c10(m, p, word(self), word(root), word(data), size, word(label));
}
// 00412D30 CResGFF::WriteFieldVector(CResStruct*, Vector, char const*) RET 0Ch: the Vector is passed by reference
// (the original reads [arg2], [arg2+4], [arg2+8]).
Word __fastcall recovered_gff_00412d30(void* self, void*, void* root, const void* vector, const char* label) {
    M m; P p(m); return g::write_vector_00412d30(m, p, word(self), word(root), word(vector), word(label));
}
// 00410F50 CResGFF::AddDataField(unsigned long offset, unsigned long old_size, unsigned long new_size) RET 0Ch
void* __fastcall recovered_gff_00410f50(void* self, void*, Word offset, Word old_size, Word new_size) {
    M m; P p(m); return ptr(g::add_data_field_00410f50(m, p, word(self), offset, old_size, new_size));
}
// 004125B0 CResGFF::AddStructToStruct(CResStruct* out, CResStruct* parent, char const*, unsigned long) RET 10h
Word __fastcall recovered_gff_004125b0(void* self, void*, void* out, void* parent, const char* label, Word type) {
    M m; P p(m); return g::add_struct_to_struct_004125b0(m, p, word(self), word(out), word(parent), word(label), type);
}
}
