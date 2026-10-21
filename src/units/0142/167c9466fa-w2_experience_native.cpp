// W2 t6 (fork H) native binding + exports: DistributeExperience 005653A0, ReceiveExperience 004EF930,
// AddExperience 005AF6A0, GetNPCID 005638A0.
#if !defined(_M_IX86)
#error W2 native bindings require the Windows x86 route
#endif
#include "e657249d9e-w2_experience.hpp"
#include <cstdint>
#include <cstring>
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void*, void*); }
extern "C" {
void __fastcall kotor_fastcall_005b27e0(void*, void*, unsigned);   // CSWSCreatureStats::AutoLevelup thiscall(int) RET 4 (srcw5 claude_levelup_rules.cpp)
unsigned __fastcall recovered_party_table_npc_id_005638a0(void*, void*, unsigned);   // W1 (t6b ruling)
extern void* kotor_cell_rules_007a3a28;
void* __cdecl live_operator_new(std::uint32_t);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
void __fastcall frame_LogStringCtor_005e5a90(void*, void*, const char*);
void __fastcall frame_LogStringDtor_005e5c20(void*, void*);
char* __fastcall async_string_cstr_005e5670(void*, void*);
int __fastcall c2da_current_get_int_00413660(void*, void*, unsigned, void*, unsigned*);
void __fastcall w2x_stats_set_experience_005af480(void*, void*, unsigned, unsigned);         // W2 0886 wrapper (t6)
void* __fastcall kotor_fastcall_004ae770(void*, void*, unsigned);
void* __fastcall kotor_fastcall_005ed700(void*, void*);
unsigned __fastcall recovered_can_level_up_005a6810(void*, void*);                         // W2 t4
void* __fastcall recovered_server_player_list_004aed90(void*, void*);                       // W1 tier6
void* __fastcall recovered_message_data_construct_004d69f0(void*, void*);                   // W1
void __fastcall recovered_event_integer_004d76b0(void*, void*, unsigned, unsigned);
void __fastcall recovered_creature_feedback_004ede10(void*, void*, unsigned, void*);        // W1
}
namespace {
using w2::experience::Word;
inline void* ptr(Word a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline Word word(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
struct Mem final : kotor::server_mainloop::Memory {
    Word read(Word a) override { Word v; std::memcpy(&v, ptr(a), 4); return v; }
    void write(Word a, Word v) override { std::memcpy(ptr(a), &v, 4); }
    Word read16(Word a) override { std::uint16_t v; std::memcpy(&v, ptr(a), 2); return v; }
    void write16(Word a, Word v) override { auto x = static_cast<std::uint16_t>(v); std::memcpy(ptr(a), &x, 2); }
    Word read8(Word a) override { std::uint8_t v; std::memcpy(&v, ptr(a), 1); return v; }
};
struct Native final : w2::experience::Ports {
    [[noreturn]] void trap(Word a, const char* w) override { kotor_unreachable_trap(a, w); }
    Word app_007a39fc() override { return word(kotor_menu_window_cells::manager_007a39fc); }
    Word rules_007a3a28() override { return word(kotor_cell_rules_007a3a28); }
    Word party_npc_id_005638a0(Word pt, Word id) override { return recovered_party_table_npc_id_005638a0(ptr(pt), nullptr, id); }   // W1
    Word party_table_004aee70(Word s) override { return word(lane_config_required::GetPartyTable_004aee70(ptr(s), nullptr)); }
    void str_literal_005e5a90(Word s, const char* t) override { frame_LogStringCtor_005e5a90(ptr(s), nullptr, t); }
    void str_destroy_005e5c20(Word s) override { frame_LogStringDtor_005e5c20(ptr(s), nullptr); }
    Word str_cstr_005e5670(Word s) override { return word(async_string_cstr_005e5670(ptr(s), nullptr)); }
    Word c2da_int_00413660(Word t, Word row, Word label, Word out) override {
        return static_cast<Word>(c2da_current_get_int_00413660(ptr(t), nullptr, row, ptr(label), static_cast<unsigned*>(ptr(out))));
    }
    void stats_set_experience_005af480(Word s, Word xp, Word f) override { w2x_stats_set_experience_005af480(ptr(s), nullptr, xp, f); }
    Word creature_by_id_004ae770(Word s, Word id) override { return word(kotor_fastcall_004ae770(ptr(s), nullptr, id)); }
    Word client_options_005ed700(Word c) override { return word(kotor_fastcall_005ed700(ptr(c), nullptr)); }
    Word can_level_up_005a6810(Word s) override { return recovered_can_level_up_005a6810(ptr(s), nullptr); }
    void auto_levelup_005b27e0(Word s, Word flag) override { kotor_fastcall_005b27e0(ptr(s), nullptr, flag); }   // levelup worker: was a trap
    Word player_list_004aed90(Word s) override { return word(recovered_server_player_list_004aed90(ptr(s), nullptr)); }
    Word vcall0(Word o, Word slot) override { Mem m; using F = void*(__thiscall*)(void*); return word(reinterpret_cast<F>(m.read(m.read(o) + slot))(ptr(o))); }
    Word allocate_006fa7e6(Word n) override { return word(live_operator_new(n)); }
    Word message_data_construct_004d69f0(Word st) override { return word(recovered_message_data_construct_004d69f0(ptr(st), nullptr)); }
    void message_data_integer_004d76b0(Word d, Word i, Word v) override { recovered_event_integer_004d76b0(ptr(d), nullptr, i, v); }
    void send_feedback_004ede10(Word c, Word id, Word d) override { recovered_creature_feedback_004ede10(ptr(c), nullptr, id, ptr(d)); }
};
}
namespace ex = w2::experience;
extern "C" {
void __fastcall recovered_party_distribute_experience_005653a0(void* party, void*, unsigned xp, unsigned feedback) {
    Mem m; Native p; alignas(8) unsigned char s[0x20] = {}; ex::distribute_experience_005653a0(m, p, word(party), xp, feedback, word(s));
}
void __fastcall recovered_creature_receive_experience_004ef930(void* c, void*, unsigned xp) {
    Mem m; Native p; alignas(8) unsigned char s[0x20] = {}; ex::receive_experience_004ef930(m, p, word(c), xp, word(s));
}
void __fastcall recovered_stats_add_experience_005af6a0(void* stats, void*, unsigned xp) { Mem m; Native p; ex::add_experience_005af6a0(m, p, word(stats), xp); }
}
