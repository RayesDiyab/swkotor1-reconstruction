#pragma once
// W2 native binding layer shared by the W2 native translation units.
// Binds the logic ports to existing 1115 owners (exact symbols verified in
// windows-link/kotor-vm-cell1115.map) and to the game CRT allocator
// live_operator_new (006FA7E6) / live_free (006FA390). No host allocator.
#if !defined(_M_IX86)
#error W2 native bindings require the Windows x86 route
#endif
#include "913ae857a3-w2_message.hpp"
#include "2f0774e610-client_loadbar.hpp"
#include <cstdint>
#include <cstring>

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace recovered_player_message {
void initialize_read_004d66b0(void*, void*, unsigned, unsigned, unsigned);
unsigned overrun_004d6230(void*);
unsigned underrun_004d6240(void*);
}
namespace kotor::server_mainloop {
// Existing owner 0872-gff_write_localized_getters.obj (declaration copied from
// docs/kotor/recovery/server-runtime-source/source/gff_write_localized_getters.hpp).
struct LocalizedGetterPorts {virtual ~LocalizedGetterPorts()=default;virtual Word next(Word,Word)=0;virtual void assign(Word,Word)=0;virtual Word length(Word)=0;virtual void byte(Word,Word)=0;};
Word localized_get_index_005e9eb0(Memory&,LocalizedGetterPorts&,Word,Word,Word,Word,Word,Word);
}
extern "C" {
extern void* kotor_cell_tlk_007a3a08;
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
unsigned __fastcall async_string_length_005e5790(void*, void*);
const char* __fastcall frame_LogCStr_005e5670(void*, void*);
void __fastcall frame_LogStringCtor_005e5a90(void*, void*, const char*);
void __fastcall frame_LogStringDtor_005e5c20(void*, void*);
void* __fastcall resman_string_counted_005e5b70(void*, void*, const char*, unsigned);
void* __fastcall r8_copy_construct_005e5b00(void*, void*, const void*);
void __fastcall async_assign_string_005e5c50(void*, void*, const void*);
void __fastcall unresolved_ref_assign_00406290(void*, void*, void*, const char*);
void __fastcall SoundAssign00406170(void*, void*, void*, const void*);
void __fastcall c2da_copy_00406120(void*, void*, void*, const void*);
void* __fastcall c2da_default_ref_00405ed0(void*, void*);
void* __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
void* __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
unsigned __fastcall exo_locinternal_get_005ec9c0(void*, void*, unsigned, void*);
unsigned __fastcall tlk_fetch_0041e550(void*, void*, unsigned, void*, unsigned);
int __fastcall net_layer_send_message_to_player_005d4cb0(void*, void*, unsigned, const char*, unsigned, unsigned);
}

namespace w2::native {
using kotor::server_mainloop::Word;
static_assert(sizeof(void*) == 4, "original pointer width");
inline void* ptr(Word a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline Word word(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
[[noreturn]] inline void trap(Word address, const char* what) { kotor_unreachable_trap(address, what); }

// Plain live memory. The only original global cell read through Memory by a
// reused owner is 007A39FC (module_message SendServerToPlayerMessage).
struct Mem final : kotor::server_mainloop::Memory {
    Word read(Word a) override {
        if (a == 0x7a39fcu) return word(kotor_menu_window_cells::manager_007a39fc);
        Word v; std::memcpy(&v, ptr(a), 4); return v;
    }
    void write(Word a, Word v) override { std::memcpy(ptr(a), &v, 4); }
    Word read16(Word a) override { std::uint16_t v; std::memcpy(&v, ptr(a), 2); return v; }
    void write16(Word a, Word v) override { const auto x = static_cast<std::uint16_t>(v); std::memcpy(ptr(a), &x, 2); }
    Word read8(Word a) override { std::uint8_t v; std::memcpy(&v, ptr(a), 1); return v; }
};

struct LocalizedGetters final : kotor::server_mainloop::LocalizedGetterPorts {
    Word next(Word a, Word b) override { return word(unresolved_list_next_005e9b90(ptr(a), nullptr, static_cast<void**>(ptr(b)))); }
    void assign(Word a, Word b) override { async_assign_string_005e5c50(ptr(a), nullptr, ptr(b)); }
    Word length(Word a) override { return async_string_length_005e5790(ptr(a), nullptr); }
    void byte(Word a, Word b) override { const auto v = static_cast<std::uint8_t>(b); std::memcpy(ptr(a), &v, 1); }
};

// Message ports used by every W2 native TU.
struct MessagePorts : w2::msg::LocPorts {
    Mem& m;
    explicit MessagePorts(Mem& memory) : m(memory) {}
    // ModuleMessagePorts
    Word allocate(Word n) override { return word(live_operator_new(n)); }
    void free(Word a) override { live_free(ptr(a)); }
    void write_byte(Word a, Word v) override { const auto x = static_cast<std::uint8_t>(v); std::memcpy(ptr(a), &x, 1); }
    Word network_getter(Word a) override { using F = void*(__thiscall*)(void*); return word(reinterpret_cast<F>(m.read(m.read(a) + 0x10))(ptr(a))); }
    Word send_005d4cb0(Word a, Word b, Word c, Word d, Word e) override { return static_cast<Word>(net_layer_send_message_to_player_005d4cb0(ptr(a), nullptr, b, static_cast<const char*>(ptr(c)), d, e)); }
    // msg::Ports
    void copy(Word dst, Word src, Word n) override { if (n != 0) std::memmove(ptr(dst), ptr(src), n); }
    Word str_length_005e5790(Word s) override { return async_string_length_005e5790(ptr(s), nullptr); }
    Word str_cstr_005e5670(Word s) override { return word(frame_LogCStr_005e5670(ptr(s), nullptr)); }
    void str_literal_005e5a90(Word s, Word t) override { frame_LogStringCtor_005e5a90(ptr(s), nullptr, static_cast<const char*>(ptr(t))); }
    void str_counted_005e5b70(Word s, Word t, Word n) override { (void)resman_string_counted_005e5b70(ptr(s), nullptr, static_cast<const char*>(ptr(t)), n); }
    void str_copy_005e5b00(Word d, Word s) override { (void)r8_copy_construct_005e5b00(ptr(d), nullptr, ptr(s)); }
    void str_destroy_005e5c20(Word s) override { frame_LogStringDtor_005e5c20(ptr(s), nullptr); }
    void ref_assign_text_00406290(Word self, Word result, Word text) override { unresolved_ref_assign_00406290(ptr(self), nullptr, ptr(result), static_cast<const char*>(ptr(text))); }
    void ref_assign_raw_00406170(Word self, Word result, Word src) override { SoundAssign00406170(ptr(self), nullptr, ptr(result), ptr(src)); }
    Word empty_text_0073d71c() override { return word(""); }
    // msg::LocPorts
    Word list_at_005e9b60(Word list, Word position) override { return word(unresolved_list_at_pos_005e9b60(ptr(list), nullptr, ptr(position))); }
    Word list_next_005e9b90(Word list, Word slot) override { return word(unresolved_list_next_005e9b90(ptr(list), nullptr, static_cast<void**>(ptr(slot)))); }
    Word locinternal_get_005ec9c0(Word internal, Word index, Word out) override { return exo_locinternal_get_005ec9c0(ptr(internal), nullptr, index, ptr(out)); }
    Word get_string_005e9eb0(Word loc, Word index, Word language, Word text, Word gender) override {
        LocalizedGetters g; Word scratch = 0;
        return kotor::server_mainloop::localized_get_index_005e9eb0(m, g, loc, index, language, text, gender, word(&scratch));
    }
    void str_empty_005b3190(Word s) override { m.write(s, 0); m.write(s + 4, 0); }   // 005B3190 body: two zero stores
    Word server() override { return m.read(word(kotor_menu_window_cells::manager_007a39fc) + 8); }
    Word tlk_fetch_0041e550(Word tlk, Word strref, Word out, Word gender) override { return ::tlk_fetch_0041e550(ptr(tlk), nullptr, strref, ptr(out), gender); }
    Word tlk() override { return word(kotor_cell_tlk_007a3a08); }
    void ref_default_00405ed0(Word r) override { (void)c2da_default_ref_00405ed0(ptr(r), nullptr); }
    void ref_assign_lower_00406120(Word self, Word result, Word src) override { c2da_copy_00406120(ptr(self), nullptr, ptr(result), ptr(src)); }
};
}
