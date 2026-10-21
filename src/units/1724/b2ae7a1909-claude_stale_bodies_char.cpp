// stale-stubs lane G4 "char": original bodies the server character ingress reached through named traps.
//   004B3070 CServerExoAppInternal::SendCharacterQuery(CSWSPlayer*)                         RET 4,  EAX 1
//   00560D00 CSWSPlayer::GetCharacterInfoFromIFO(BYTE* count, int* classes, BYTE* levels, DWORD* xp) RET 10h
//   0056C730 CSWSMessage::SendServerToPlayerLogin_CharacterQuery(player, count*, classes*, levels*, xp*) RET 14h
//   00562380 CSWSPlayer::LoadServerCharacter(CResRef name)                                  RET 10h
// Every callee is an existing linked provider (listed beside each declaration). No traps, no invented values.
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
using U8 = std::uint8_t;
using U16 = std::uint16_t;
template <class T> T rd(const void* p, unsigned off) { T v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof v); return v; }
template <class T> void wr(void* p, unsigned off, T v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, sizeof v); }
struct Ref16 { unsigned char bytes[16]; };   // CResRef, passed by value in 16 bytes
}

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }        // 0053 window cells (g_pAppManager)
namespace kotor_reconstruction { struct StartupString; }
namespace menu_resources {
struct Manager { unsigned directory_00408800(kotor_reconstruction::StartupString*); };   // 0027 CExoResMan::AddResourceDirectory
struct Ref { unsigned char bytes[16]; Ref* construct_00406d80(const char*); };           // 0030 CResRef::CResRef(char*)
Manager* __cdecl BorrowCanonicalManager007a39e8();                                       // 0150 [007A39E8] resman
}

extern "C" {
void* __cdecl live_operator_new(unsigned);
void __cdecl live_free(void*);
// GFF / resources
void* __fastcall c2da_lookup_004074d0(void* resman, void*, void* resref, unsigned type);       // 0264 GetResObject RET 8
U __fastcall c2da_current_resource_demand_00409b10(void* res, void*);                         // 0264 CRes::Demand
U __fastcall c2da_release_demand_00409b80(void* res, void*);                                  // 0264 CRes::Release
U __fastcall c2da_release_00409cf0(void* resman, void*, void* res);                           // 0264 ReleaseResObject RET 4
U __fastcall kotor_fastcall_00411240(void* gff, void*, void* out_struct);                     // claude_dialogue_start GetTopLevelStruct
unsigned __fastcall gff_list_004118c0(void* gff, void*, void* out_list, void* s, const char* label);   // 0291 GetList RET 0Ch
unsigned __fastcall gff_list_count_00411940(void* gff, void*, void* list);                    // 0291 GetListCount RET 4
unsigned __fastcall gff_list_element_00411990(void* gff, void*, void* out_struct, void* list, unsigned i); // 0291 GetListElement
unsigned __fastcall gui_load_gff_dword_00411c20(void* gff, void*, void* s, const char* label, unsigned* ok, unsigned fallback); // 0292
int __fastcall gui_load_gff_int_00411c90(void* gff, void*, void* s, const char* label, unsigned* ok, int fallback);            // 0292
U16 __fastcall kotor_fastcall_00411bb0(void* gff, void*, void* s, const char* label, U* ok, U fallback);  // claude_p2s_b_charlist ReadFieldSHORT
// messages
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                  // creature_destructor_r2 GetSWSMessage
void __fastcall kotor_fastcall_004d68a0(void* msg, void*, unsigned size, unsigned player, unsigned flag); // w2_message_native CreateWriteMessage
void __fastcall kotor_fastcall_004d62c0(void* msg, void*, unsigned value, unsigned bits);     // w2_message_native WriteBYTE
void __fastcall kotor_fastcall_004d6340(void* msg, void*, unsigned value, unsigned bits);     // w2_message_native WriteDWORD
unsigned __fastcall kotor_fastcall_004d6440(void* msg, void*, void** data, unsigned* size);    // w2_message_native GetWriteMessage
unsigned __fastcall kotor_fastcall_00567e60(void* msg, void*, unsigned player, unsigned major, unsigned minor,
                                            void* data, unsigned size);                       // w2_message_native SendServerToPlayerMessage
// strings / objects
void __fastcall frame_LogStringCtor_005e5a90(void* s, void*, const char* text);               // 0459 CExoString(char*)
void __fastcall frame_LogStringDtor_005e5c20(void* s, void*);                                 // 0459 ~CExoString
void __fastcall kotor_fastcall_004088d0(void* resman, void*, void* directory);                // 0576 RemoveResourceDirectory
void* __fastcall recovered_server_creature_construct_004f7a10(void* mem, void*, unsigned id, unsigned flag); // 0963 CSWSCreature ctor
void __fastcall recovered_set_game_object_00561c40(void* player, void*, void* creature);      // w2_entry_native SetGameObject
unsigned __fastcall recovered_player_deserialize_bic_00560e60(void* player, void*, Ref16 name, void* creature); // character_materializer

unsigned __fastcall kotor_fastcall_00560d00(void* player, void*, U8* count, U* classes, U8* levels, U* experience);
unsigned __fastcall kotor_fastcall_0056c730(void* msg, void*, void* player, U8* count, U* classes, U8* levels, U* experience);
unsigned __fastcall kotor_fastcall_004b3070(void* internal, void*, void* player);
unsigned __fastcall kotor_fastcall_00562380(void* player, void*, Ref16 name);
}

namespace {
void* server_app() { return rd<void*>(kotor_menu_window_cells::manager_007a39fc, 8); }   // [[007A39FC]+8]
// Original .rdata literals.
const char kModule_007442a8[] = "Module";
const char kPlayerList_00745320[] = "Mod_PlayerList";
const char kExperience_00745378[] = "Experience";
const char kClassList_0074535c[] = "ClassList";
const char kClass_00749068[] = "Class";
const char kClassLevel_00745350[] = "ClassLevel";
const char kServerVault_00745514[] = "SERVERVAULT:";
}

extern "C" {
// 00560D00 CSWSPlayer::GetCharacterInfoFromIFO. Frame mirrors the original locals (ESP+8..4Fh) so the GFF
// struct/list handles keep their original placement: elem0 [08h], ok [0Ch], elem1 [10h], top [14h], resref [18h],
// class list [28h], player list [3Ch]. Original quirks kept: after a failed list lookup the IFO is released without
// CRes::Release (Demand stays counted); the class loop writes classes[i]/levels[i] for every list entry unchecked.
unsigned __fastcall kotor_fastcall_00560d00(void* player, void*, U8* count, U* classes, U8* levels, U* experience) {
    alignas(4) unsigned char f[0x50];                                                   // f[x] == [esp+x]
    reinterpret_cast<menu_resources::Ref*>(f + 0x18)->construct_00406d80(kModule_007442a8);           // 00560D07..10
    void* const res = c2da_lookup_004074d0(menu_resources::BorrowCanonicalManager007a39e8(), nullptr, f + 0x18, 0x7de); // 00560D15..25
    if (res == nullptr) return 0;                                                       // 00560D2C..2E
    (void)c2da_current_resource_demand_00409b10(res, nullptr);                          // 00560D30..32
    (void)kotor_fastcall_00411240(res, nullptr, f + 0x14);                              // 00560D37..3E
    if (gff_list_004118c0(res, nullptr, f + 0x3c, f + 0x14, kPlayerList_00745320) == 0 ||          // 00560D43..5B
        gff_list_element_00411990(res, nullptr, f + 0x08, f + 0x3c, rd<U>(player, 0x44)) == 0) {  // 00560D5D..74
        (void)c2da_release_00409cf0(menu_resources::BorrowCanonicalManager007a39e8(), nullptr, res); // 00560D76..7D
        return 0;                                                                       // 00560D82..89
    }
    *experience = gui_load_gff_dword_00411c20(res, nullptr, f + 0x08, kExperience_00745378,
                                              reinterpret_cast<unsigned*>(f + 0x0c), 0);           // 00560D8C..A8
    if (gff_list_004118c0(res, nullptr, f + 0x28, f + 0x08, kClassList_0074535c) == 1) {          // 00560DAA..C3
        *count = static_cast<U8>(gff_list_count_00411940(res, nullptr, f + 0x28));     // 00560DC5..DA (AL)
        for (U i = 0; i < *count; ++i) {                                                // 00560DD6..35 (count re-read)
            (void)gff_list_element_00411990(res, nullptr, f + 0x10, f + 0x28, i);       // 00560DE3..F0
            classes[i] = static_cast<U>(gui_load_gff_int_00411c90(res, nullptr, f + 0x10, kClass_00749068,
                                                                  reinterpret_cast<unsigned*>(f + 0x0c), -1)); // 00560DF5..18
            levels[i] = static_cast<U8>(kotor_fastcall_00411bb0(res, nullptr, f + 0x10, kClassLevel_00745350,
                                                                reinterpret_cast<U*>(f + 0x0c), 0));         // 00560E11..2C
        }
    }
    (void)c2da_release_demand_00409b80(res, nullptr);                                   // 00560E39..3B
    (void)c2da_release_00409cf0(menu_resources::BorrowCanonicalManager007a39e8(), nullptr, res);    // 00560E40..47
    return 1;                                                                           // 00560E4D
}

// 0056C730 CSWSMessage::SendServerToPlayerLogin_CharacterQuery. Major 2 / minor 0Ah; returns the send result,
// 0 when no write message is produced. The class count byte is re-read every iteration (BL counter).
unsigned __fastcall kotor_fastcall_0056c730(void* msg, void*, void* player, U8* count, U* classes, U8* levels, U* experience) {
    kotor_fastcall_004d68a0(msg, nullptr, 0x80, 0xffffffffu, 1);                        // 0056C736..41
    kotor_fastcall_004d62c0(msg, nullptr, *count, 8);                                   // 0056C746..54
    for (U8 i = 0; i < *count; ++i) {                                                   // 0056C759..91
        kotor_fastcall_004d6340(msg, nullptr, classes[i], 0x20);                        // 0056C763..72
        kotor_fastcall_004d62c0(msg, nullptr, levels[i], 8);                            // 0056C777..85
    }
    kotor_fastcall_004d6340(msg, nullptr, *experience, 0x20);                           // 0056C794..9F
    void* data;                                                                         // [esp+10h]
    unsigned size;                                                                      // [esp+0Ch]
    if (kotor_fastcall_004d6440(msg, nullptr, &data, &size) == 0) return 0;             // 0056C7A4..B9
    return kotor_fastcall_00567e60(msg, nullptr, rd<U>(player, 4), 2, 0x0a, data, size); // 0056C7C2..DA
}

// 004B3070 CServerExoAppInternal::SendCharacterQuery(CSWSPlayer*). Original quirks: GetCharacterInfoFromIFO's
// status is ignored, so on failure the uninitialised locals are sent; the locals hold two classes (8 bytes) and
// four level bytes with no bound on the IFO class count.
unsigned __fastcall kotor_fastcall_004b3070(void*, void*, void* player) {
    void* const msg = recovered_server_sws_message_004aedb0(server_app(), nullptr);     // 004B3070..7D
    alignas(4) unsigned char f[0x14];                                                   // [esp+8..1Bh] -> f[x-8]
    U8* const count = f + 0x03;                                                         // [esp+0Bh]
    U8* const levels = f + 0x04;                                                        // [esp+0Ch]
    U* const experience = reinterpret_cast<U*>(f + 0x08);                               // [esp+10h]
    U* const classes = reinterpret_cast<U*>(f + 0x0c);                                  // [esp+14h]
    (void)kotor_fastcall_00560d00(player, nullptr, count, classes, levels, experience);  // 004B3082..9E
    (void)kotor_fastcall_0056c730(msg, nullptr, player, count, classes, levels, experience); // 004B30A3..BA
    return 1;                                                                           // 004B30C0
}

// 00562380 CSWSPlayer::LoadServerCharacter(CResRef). EH: state 0 frees the creature allocation if its ctor throws,
// states 1/2 destroy the "SERVERVAULT:" temporaries. Original quirk: a null allocation is dereferenced at [edi+4].
unsigned __fastcall kotor_fastcall_00562380(void* player, void*, Ref16 name) {
    void* const raw = live_operator_new(0xac8);                                         // 0056239B..A2
    void* creature = nullptr;                                                           // 005623CC
    if (raw != nullptr) {
        try { creature = recovered_server_creature_construct_004f7a10(raw, nullptr, 0x7f000000u, 1); } // 005623BA..C3
        catch (...) { live_free(raw); throw; }
    }
    recovered_set_game_object_00561c40(player, nullptr, creature);                      // 005623D1..D8
    wr<U>(player, 0x38, rd<U>(creature, 4));                                            // 005623DD..E9
    wr<U8>(player, 0x64, 3);                                                            // 005623EC
    {
        alignas(4) unsigned char text[8];                                               // [esp+0Ch]
        frame_LogStringCtor_005e5a90(text, nullptr, kServerVault_00745514);             // 005623E0..F0
        try { kotor_fastcall_004088d0(menu_resources::BorrowCanonicalManager007a39e8(), nullptr, text); } // 005623F5..08
        catch (...) { frame_LogStringDtor_005e5c20(text, nullptr); throw; }
        frame_LogStringDtor_005e5c20(text, nullptr);                                    // 0056240D..15
    }
    {
        alignas(4) unsigned char text[8];                                               // [esp+14h]
        frame_LogStringCtor_005e5a90(text, nullptr, kServerVault_00745514);             // 0056241A..23
        try { (void)menu_resources::BorrowCanonicalManager007a39e8()->directory_00408800(
                  reinterpret_cast<kotor_reconstruction::StartupString*>(text)); }      // 00562428..3B
        catch (...) { frame_LogStringDtor_005e5c20(text, nullptr); throw; }
        frame_LogStringDtor_005e5c20(text, nullptr);                                    // 00562440..48
    }
    const unsigned status = recovered_player_deserialize_bic_00560e60(player, nullptr, name, creature); // 0056244D..70
    if (status == 0) return 0;                                                          // 00562477..79 -> 005624A9
    recovered_set_game_object_00561c40(player, nullptr, nullptr);                       // 0056247B..7F
    wr<U>(player, 0x38, 0x7f000000u);                                                   // 00562484
    using DeletingDtor = void(__fastcall*)(void*, void*, unsigned);
    rd<DeletingDtor>(rd<void*>(creature, 0), 0)(creature, nullptr, 1);                 // 0056248B..91 vtbl+0 (1)
    return status;                                                                      // 00562493
}
}  // extern "C"
