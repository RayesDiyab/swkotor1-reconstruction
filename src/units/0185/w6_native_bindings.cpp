// Native W6 import bindings to reconstructed chat and creature-stat providers.
// Forwarders preserve the selected x86 calling conventions and argument layouts.
struct W2ExoStringArg { char* text; unsigned length; };
extern "C" {
unsigned __fastcall recovered_send_chat_00571960(void*, void*, unsigned, unsigned, W2ExoStringArg, unsigned, unsigned);
unsigned char __fastcall recovered_creature_stats_str_005a6190(void*, void*);
unsigned char __fastcall recovered_creature_stats_dex_005a6550(void*, void*);
unsigned char __fastcall recovered_creature_stats_con_005a6250(void*, void*);
unsigned char __fastcall recovered_creature_stats_int_005a6310(void*, void*);
unsigned char __fastcall recovered_creature_stats_wis_005a63d0(void*, void*);
unsigned char __fastcall recovered_creature_stats_cha_005a6490(void*, void*);
void __fastcall w6_import_send_chat_00571960(void* m, void*, unsigned mode, unsigned id, unsigned text, unsigned len, unsigned pl, void* extra) {
    W2ExoStringArg s{reinterpret_cast<char*>(static_cast<unsigned long long>(text)), len};
    (void)recovered_send_chat_00571960(m, nullptr, mode, id, s, pl, static_cast<unsigned>(reinterpret_cast<unsigned long long>(extra)));
}
unsigned char __fastcall w6_import_creature_stats_str_005a6190(void* s, void*) { return recovered_creature_stats_str_005a6190(s, nullptr); }
unsigned char __fastcall w6_import_creature_stats_dex_005a6550(void* s, void*) { return recovered_creature_stats_dex_005a6550(s, nullptr); }
unsigned char __fastcall w6_import_creature_stats_con_005a6250(void* s, void*) { return recovered_creature_stats_con_005a6250(s, nullptr); }
unsigned char __fastcall w6_import_creature_stats_int_005a6310(void* s, void*) { return recovered_creature_stats_int_005a6310(s, nullptr); }
unsigned char __fastcall w6_import_creature_stats_wis_005a63d0(void* s, void*) { return recovered_creature_stats_wis_005a63d0(s, nullptr); }
unsigned char __fastcall w6_import_creature_stats_cha_005a6490(void* s, void*) { return recovered_creature_stats_cha_005a6490(s, nullptr); }
}
