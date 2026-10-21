// W2 t6 native binding + export: CSWSMessage::SendServerToPlayerChatMessage 00571960 (@32, RET 18h).
// The CExoString argument is passed by value (two dwords) and destroyed by the callee, as in the original.
#if !defined(_M_IX86)
#error W2 native bindings require the Windows x86 route
#endif
#include "386aac6f5b-w2_chat.hpp"
#include "1291820eb6-faction_member_helpers.hpp"
#include <cstdint>
#include <cstring>
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
void* __fastcall recovered_server_player_list_004aed90(void*, void*);                         // W1 tier6
void* __fastcall recovered_object_array_004aed70(void*, void*);                               // W2
unsigned char __fastcall kotor_fastcall_004d8230(void*, void*, unsigned, void*);
void* __fastcall recovered_object_area_004cb120(void*, void*);
void* __fastcall kotor_fastcall_004ae910(void*, void*, unsigned);
void* __fastcall kotor_fastcall_00561080(void*, void*);
void* __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
void* __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
unsigned __fastcall recovered_chat_talk_0056d7e0(void*, void*, unsigned, unsigned, void*);       // W2 t6 (fork F)
unsigned __fastcall recovered_chat_whisper_0056dbf0(void*, void*, unsigned, unsigned, void*);    // W2 t6 (fork F)
unsigned __fastcall recovered_chat_tell_0056d8c0(void*, void*, unsigned, unsigned, void*);       // W2 t6 (fork F)
unsigned __fastcall recovered_chat_server_tell_0056d9c0(void*, void*, unsigned, void*);          // W2 t6 (fork F)
unsigned __fastcall recovered_chat_shout_0056da30(void*, void*, unsigned, unsigned, void*);      // W2 t6 (fork F)
unsigned __fastcall recovered_faction_send_chat_005be900(void*, void*, unsigned, void*);         // W2 t6 (fork F)
unsigned __fastcall recovered_object_broadcast_dialog_004cd760(void*, void*, void*, float, unsigned);   // W2 t6 (fork G)
void __fastcall frame_LogStringDtor_005e5c20(void*, void*);
}
namespace {
using w2::chat::Word;
inline void* ptr(Word a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline Word word(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
float f32(Word b) { float v; std::memcpy(&v, &b, 4); return v; }
struct Mem final : kotor::server_mainloop::Memory {
    Word read(Word a) override { if (a == 0x7a39fcu) return word(kotor_menu_window_cells::manager_007a39fc); Word v; std::memcpy(&v, ptr(a), 4); return v; }
    void write(Word a, Word v) override { std::memcpy(ptr(a), &v, 4); }
    Word read16(Word a) override { std::uint16_t v; std::memcpy(&v, ptr(a), 2); return v; }
    void write16(Word a, Word v) override { auto x = static_cast<std::uint16_t>(v); std::memcpy(ptr(a), &x, 2); }
    Word read8(Word a) override { std::uint8_t v; std::memcpy(&v, ptr(a), 1); return v; }
};
struct Native final : w2::chat::Ports {
    [[noreturn]] void trap(Word a, const char* w) override { kotor_unreachable_trap(a, w); }
    Word app_007a39fc() override { return word(kotor_menu_window_cells::manager_007a39fc); }
    Word player_list_004aed90(Word s) override { return word(recovered_server_player_list_004aed90(ptr(s), nullptr)); }
    Word object_array_004aed70(Word s) override { return word(recovered_object_array_004aed70(ptr(s), nullptr)); }
    Word object_lookup_004d8230(Word a, Word id, Word out) override { return kotor_fastcall_004d8230(ptr(a), nullptr, id, ptr(out)); }
    Word object_area_004cb120(Word o) override { return word(recovered_object_area_004cb120(ptr(o), nullptr)); }
    Word vcall0(Word o, Word slot) override { Mem m; using F = void*(__thiscall*)(void*); return word(reinterpret_cast<F>(m.read(m.read(o) + slot))(ptr(o))); }
    Word client_object_by_id_004ae910(Word s, Word id) override { return word(kotor_fastcall_004ae910(ptr(s), nullptr, id)); }
    Word player_game_object_00561080(Word pl) override { return word(kotor_fastcall_00561080(ptr(pl), nullptr)); }
    Word list_at_pos_005e9b60(Word l, Word pos) override { return word(unresolved_list_at_pos_005e9b60(ptr(l), nullptr, ptr(pos))); }
    void list_next_005e9b90(Word l, Word slot) override { (void)unresolved_list_next_005e9b90(ptr(l), nullptr, static_cast<void**>(ptr(slot))); }
    Word creature_faction_00513fc0(Word c) override { Mem m; return kotor::server_mainloop::creature_faction_00513fc0(m, c); }
    Word chat_talk_0056d7e0(Word msg, Word pl, Word sp, Word t) override { return recovered_chat_talk_0056d7e0(ptr(msg), nullptr, pl, sp, ptr(t)); }
    Word chat_whisper_0056dbf0(Word msg, Word pl, Word sp, Word t) override { return recovered_chat_whisper_0056dbf0(ptr(msg), nullptr, pl, sp, ptr(t)); }
    Word chat_tell_0056d8c0(Word msg, Word pl, Word sp, Word t) override { return recovered_chat_tell_0056d8c0(ptr(msg), nullptr, pl, sp, ptr(t)); }
    Word chat_server_tell_0056d9c0(Word msg, Word pl, Word t) override { return recovered_chat_server_tell_0056d9c0(ptr(msg), nullptr, pl, ptr(t)); }
    Word chat_shout_0056da30(Word msg, Word pl, Word sp, Word t) override { return recovered_chat_shout_0056da30(ptr(msg), nullptr, pl, sp, ptr(t)); }
    Word faction_send_chat_005be900(Word f, Word sp, Word t) override { return recovered_faction_send_chat_005be900(ptr(f), nullptr, sp, ptr(t)); }
    Word broadcast_dialog_004cd760(Word o, Word t, Word r, Word fl) override { return recovered_object_broadcast_dialog_004cd760(ptr(o), nullptr, ptr(t), f32(r), fl); }
    void str_destroy_005e5c20(Word s) override { frame_LogStringDtor_005e5c20(ptr(s), nullptr); }
};
}
struct W2ExoStringArg { char* text; unsigned length; };   // CExoString by value (8 bytes on the stack)
extern "C" unsigned __fastcall recovered_send_chat_00571960(void* msg, void*, unsigned mode, unsigned speaker, W2ExoStringArg text,
                                                           unsigned target, unsigned unused) {
    (void)unused;
    Mem m; Native p; alignas(8) unsigned char s[0x20] = {};
    return w2::chat::send_chat_00571960(m, p, word(msg), mode, speaker, word(&text), target, word(s));
}
