// W2 t6 native binding + exports: server -> client chat senders (0056D6E0..0056DBF0) and CSWSFaction::SendChatMessage
// 005BE900. Message writers / framing: W2 w2_message bodies + 0898 module_message owners through w2::native::MessagePorts.
#include "2346527c32-w2_native_ports.hpp"
#include "0a2c4c7b5e-w2_chat_send.hpp"
using namespace w2::native;
namespace cs = w2::chat_send;
extern "C" {
void* __fastcall kotor_fastcall_004ae770(void*, void*, unsigned);              // CServerExoApp::GetCreatureByGameObjectID
void* __fastcall kotor_fastcall_004ae910(void*, void*, unsigned);              // CServerExoApp::GetClientObjectByObjectId
void* __fastcall exo_locstring_construct_005e9e10(void*, void*);
void __fastcall exo_locstring_destroy_005ea000(void*, void*);
}
namespace {
struct ChatNative final : cs::Ports {
    Mem& m; explicit ChatNative(Mem& v) : m(v) {}
    [[noreturn]] void trap(Word a, const char* w) override { kotor_unreachable_trap(a, w); }
    Word app_007a39fc() override { return word(kotor_menu_window_cells::manager_007a39fc); }
    Word creature_by_id_004ae770(Word s, Word id) override { return word(kotor_fastcall_004ae770(ptr(s), nullptr, id)); }
    Word client_object_by_id_004ae910(Word s, Word id) override { return word(kotor_fastcall_004ae910(ptr(s), nullptr, id)); }
    // 004AEDB0 GetSWSMessage: mov eax,[ecx+4]; mov eax,[eax+10010h]; ret (inlined, as w2_entry.cpp sws_message)
    Word sws_message_004aedb0(Word s) override { return m.read(m.read(s + 4) + 0x10010); }
    Word vcall0(Word o, Word slot) override { using F = void*(__thiscall*)(void*); return word(reinterpret_cast<F>(m.read(m.read(o) + slot))(ptr(o))); }
    void loc_construct_005e9e10(Word l) override { (void)exo_locstring_construct_005e9e10(ptr(l), nullptr); }
    void loc_destroy_005ea000(Word l) override { exo_locstring_destroy_005ea000(ptr(l), nullptr); }
};
}
#define W2_CHAT3(name, fn) \
unsigned __fastcall name(void* self, void*, unsigned player, unsigned speaker, void* text) { \
    Mem m; MessagePorts p(m); ChatNative x(m); alignas(8) unsigned char s[0x30] = {}; \
    return cs::fn(m, p, x, word(self), player, speaker, word(text), word(s)); }
extern "C" {
W2_CHAT3(recovered_chat_talk_0056d7e0, talk_0056d7e0)
W2_CHAT3(recovered_chat_whisper_0056dbf0, whisper_0056dbf0)
W2_CHAT3(recovered_chat_tell_0056d8c0, tell_0056d8c0)
W2_CHAT3(recovered_chat_shout_0056da30, shout_0056da30)
W2_CHAT3(recovered_chat_party_0056d6e0, party_0056d6e0)
unsigned __fastcall recovered_chat_server_tell_0056d9c0(void* self, void*, unsigned player, void* text) {
    Mem m; MessagePorts p(m); ChatNative x(m); alignas(8) unsigned char s[0x30] = {};
    return cs::server_tell_0056d9c0(m, p, x, word(self), player, word(text), word(s));
}
unsigned __fastcall recovered_faction_send_chat_005be900(void* faction, void*, unsigned speaker, void* text) {
    Mem m; MessagePorts p(m); ChatNative x(m); alignas(8) unsigned char s[0x30] = {};
    return cs::faction_send_chat_005be900(m, p, x, word(faction), speaker, word(text), word(s));
}
}
#undef W2_CHAT3
