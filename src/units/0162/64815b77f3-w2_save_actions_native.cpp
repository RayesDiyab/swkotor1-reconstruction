// W2 t6 fork E2: native binding + exports for SaveActionQueue 004CC7E0, SaveScriptSituation 005D10F0 / 005D47A0 and
// CVirtualMachineStack::SaveStack 005D1D80. Providers: 0873 GFF writers (in 1122/1125), W2 t6 fork-A GFF siblings
// (w2_save_gff_native.cpp), list helpers (1122), W6 VM exports (in 1125), cell 007A3A00 (0456).
#if !defined(_M_IX86)
#error W2 native bindings require the Windows x86 route
#endif
#include "4b6543586e-w2_save_actions.hpp"
#include <cstdint>
#include <cstring>
namespace frame_control { extern unsigned message_manager_007a3a00; }   // 007A3A00 CVirtualMachine* (0456)
extern "C" {
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
unsigned __fastcall recovered_gff_00412450(void*, void*, void*, void*, const char*);           // AddList
unsigned __fastcall recovered_gff_004124e0(void*, void*, void*, void*, unsigned);              // AddListElement
unsigned __fastcall recovered_gff_00412670(void*, void*, void*, unsigned, const char*);        // CHAR
unsigned __fastcall recovered_gff_004126c0(void*, void*, void*, unsigned, const char*);        // WORD
unsigned __fastcall recovered_gff_00412760(void*, void*, void*, unsigned, const char*);        // DWORD
unsigned __fastcall recovered_gff_004127b0(void*, void*, void*, unsigned, const char*);        // INT
unsigned __fastcall recovered_gff_00412970(void*, void*, void*, void*, const char*);           // CExoString
unsigned __fastcall recovered_gff_00412870(void*, void*, void*, float, const char*);           // FLOAT (fork A)
unsigned __fastcall recovered_gff_00412c10(void*, void*, void*, void*, unsigned, const char*); // VOID (fork A)
unsigned __fastcall recovered_gff_004125b0(void*, void*, void*, void*, const char*, unsigned); // AddStructToStruct (fork A)
void* __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
void* __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
unsigned __fastcall recovered_vm_read_script_file_005d2260(void*, void*, void*);              // W6 (0886 in 1125)
void __fastcall recovered_vm_script_delete_005d44f0(void*, void*, void*);                      // W6 (1187)
}
namespace {
using w2::save_actions::Word;
inline void* ptr(Word a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline Word word(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
struct Mem final : kotor::server_mainloop::Memory {
    Word read(Word a) override { Word v; std::memcpy(&v, ptr(a), 4); return v; }
    void write(Word a, Word v) override { std::memcpy(ptr(a), &v, 4); }
    Word read16(Word a) override { std::uint16_t v; std::memcpy(&v, ptr(a), 2); return v; }
    void write16(Word a, Word v) override { auto x = static_cast<std::uint16_t>(v); std::memcpy(ptr(a), &x, 2); }
    Word read8(Word a) override { std::uint8_t v; std::memcpy(&v, ptr(a), 1); return v; }
};
// Labels padded to 16 bytes (the GFF writers copy a 16-byte label field), as save_chargen_native does.
struct Label { Word original; char text[16]; };
const Label kLabels[] = {
    {0x00745ea0u, "ActionList"}, {0x00745e94u, "ActionId"}, {0x00745e84u, "GroupActionId"}, {0x00745e78u, "NumParams"},
    {0x00745e6cu, "Paramaters"}, {0x00745e38u, "Type"}, {0x00744bfcu, "Value"}, {0x0074c4f0u, "CodeSize"},
    {0x0074c4e8u, "Code"}, {0x0074c4e4u, "CRC"}, {0x0074c4d4u, "InstructionPtr"}, {0x0074c4c4u, "SecondaryPtr"},
    {0x00745eecu, "Name"}, {0x00748fe0u, "StackSize"}, {0x0074c494u, "Stack"}, {0x0074c4b8u, "BasePointer"},
    {0x0074c4a8u, "StackPointer"}, {0x0074c49cu, "TotalSize"}};
const char* label(Word a) { return static_cast<const char*>(ptr(a)); }
struct Native final : w2::save_actions::Ports {
    [[noreturn]] void trap(Word a, const char* w) override { kotor_unreachable_trap(a, w); }
    Word literal(Word a) override {
        for (const Label& l : kLabels) if (l.original == a) return word(l.text);
        kotor_unreachable_trap(a, "W2 save_actions label not transcribed");
    }
    Word add_list_00412450(Word g, Word out, Word parent, Word l) override { return recovered_gff_00412450(ptr(g), nullptr, ptr(out), ptr(parent), label(l)); }
    Word add_list_element_004124e0(Word g, Word out, Word list, Word t) override { return recovered_gff_004124e0(ptr(g), nullptr, ptr(out), ptr(list), t); }
    Word write_char_00412670(Word g, Word st, Word v, Word l) override { return recovered_gff_00412670(ptr(g), nullptr, ptr(st), v, label(l)); }
    Word write_word_004126c0(Word g, Word st, Word v, Word l) override { return recovered_gff_004126c0(ptr(g), nullptr, ptr(st), v, label(l)); }
    Word write_dword_00412760(Word g, Word st, Word v, Word l) override { return recovered_gff_00412760(ptr(g), nullptr, ptr(st), v, label(l)); }
    Word write_int_004127b0(Word g, Word st, Word v, Word l) override { return recovered_gff_004127b0(ptr(g), nullptr, ptr(st), v, label(l)); }
    Word write_float_00412870(Word g, Word st, Word bits, Word l) override { float f; std::memcpy(&f, &bits, 4); return recovered_gff_00412870(ptr(g), nullptr, ptr(st), f, label(l)); }
    Word write_string_00412970(Word g, Word st, Word s, Word l) override { return recovered_gff_00412970(ptr(g), nullptr, ptr(st), ptr(s), label(l)); }
    Word write_void_00412c10(Word g, Word st, Word d, Word n, Word l) override { return recovered_gff_00412c10(ptr(g), nullptr, ptr(st), ptr(d), n, label(l)); }
    Word add_struct_to_struct_004125b0(Word g, Word out, Word parent, Word l, Word t) override { return recovered_gff_004125b0(ptr(g), nullptr, ptr(out), ptr(parent), label(l), t); }
    Word list_at_pos_005e9b60(Word l, Word pos) override { return word(unresolved_list_at_pos_005e9b60(ptr(l), nullptr, ptr(pos))); }
    void list_next_005e9b90(Word l, Word slot) override { (void)unresolved_list_next_005e9b90(ptr(l), nullptr, static_cast<void**>(ptr(slot))); }
    Word vm_007a3a00() override { return frame_control::message_manager_007a3a00; }
    Word read_script_file_005d2260(Word vmi, Word name) override { return recovered_vm_read_script_file_005d2260(ptr(vmi), nullptr, ptr(name)); }
    void delete_script_005d44f0(Word vmi, Word script) override { recovered_vm_script_delete_005d44f0(ptr(vmi), nullptr, ptr(script)); }
    void save_engine_structure(Word impl, Word type, Word value, Word gff, Word st) override {
        using F = int(__thiscall*)(void*, int, void*, void*, void*);              // [[impl]+24h](type, value, gff, struct*)
        Mem m; (void)reinterpret_cast<F>(m.read(m.read(impl) + 0x24))(ptr(impl), static_cast<int>(type), ptr(value), ptr(gff), ptr(st));
    }
};
}
namespace sa = w2::save_actions;
extern "C" {
void __fastcall recovered_save_action_queue_004cc7e0(void* object, void*, void* gff, void* parent) {
    Mem m; Native p; alignas(8) unsigned char s[0x60] = {}; sa::save_action_queue_004cc7e0(m, p, word(object), word(gff), word(parent), word(s));
}
unsigned __fastcall recovered_vm_save_situation_005d10f0(void* vm, void*, void* situation, void* gff, void* st) {
    Mem m; Native p; alignas(8) unsigned char s[0x20] = {}; return sa::vm_save_situation_005d10f0(m, p, word(vm), word(situation), word(gff), word(st), word(s));
}
unsigned __fastcall recovered_vm_internal_save_situation_005d47a0(void* vmi, void*, void* situation, void* gff, void* st) {
    Mem m; Native p; alignas(8) unsigned char s[0x20] = {}; return sa::vm_internal_save_situation_005d47a0(m, p, word(vmi), word(situation), word(gff), word(st), word(s));
}
unsigned __fastcall recovered_vm_stack_save_005d1d80(void* stack, void*, void* gff, void* st) {
    Mem m; Native p; alignas(8) unsigned char s[0x18] = {}; return sa::vm_stack_save_005d1d80(m, p, word(stack), word(gff), word(st), word(s));
}
}
