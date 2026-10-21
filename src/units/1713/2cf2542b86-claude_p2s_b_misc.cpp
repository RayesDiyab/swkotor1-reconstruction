// Lane worker "p2s-b": CSWSMessage::HandlePlayerToServer* handlers routed from the 0918 player-message dispatcher
// (Chat, QuickChat, ShutDownServer, PlayerDeath, PlayModuleCharacterList, MapPin) and their missing callees.
// Transcribed from the original swkotor.exe (read-only); statement comments cite original instruction addresses.
// Handler ABI: thiscall(CSWSMessage* this; CSWSPlayer* player, BYTE minor) RET 8, exported as
//   extern "C" unsigned __fastcall recovered_player_to_server_<name>_<addr>(message, edx, player, minor) @16.
#include "6bd5403233-claude_dialogue_common.hpp"

using namespace dlg;

namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString* out, const char* format, ...); }
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }       // [007A39E0] CExoBase*
namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned int); }

namespace p2s_b {
struct ExoStringArg { char* text; U length; };   // CExoString passed by value (two dwords; callee destroys)
}

extern "C" {
// ---- providers (c16 map; none in the c16 trap census) ----
U __fastcall kotor_fastcall_004d6230(void* msg, void*, U bits);           // CSWMessage::MessageReadOverflow RET 4
U __fastcall kotor_fastcall_004d6240(void* msg, void*, U bits);           // CSWMessage::MessageReadUnderflow RET 4
U __fastcall kotor_fastcall_004d66f0(void* msg, void*);                   // CSWMessage::ReadBOOL RET 0
U16 __fastcall kotor_fastcall_004d6750(void* msg, void*, U bits);         // CSWMessage::ReadWORD RET 4
U __fastcall kotor_fastcall_004d6780(void* msg, void*, U bits);           // CSWMessage::ReadDWORD RET 4
float __fastcall kotor_fastcall_004d67b0(void* msg, void*, U scale, U bits); // CSWMessage::ReadFLOAT RET 8
void* __fastcall kotor_fastcall_004d67e0(void* msg, void*, void* result, U bits); // CSWMessage::ReadCExoString RET 8
void __fastcall kotor_fastcall_004d68a0(void* msg, void*, U size, U a, U b);       // CSWMessage::CreateWriteMessage
void __fastcall kotor_fastcall_004d62c0(void* msg, void*, U value, U bits);        // CSWMessage::WriteBYTE
void __fastcall kotor_fastcall_004d6300(void* msg, void*, U value, U bits);        // CSWMessage::WriteWORD
void __fastcall kotor_fastcall_004d6340(void* msg, void*, U value, U bits);        // CSWMessage::WriteDWORD
void __fastcall kotor_fastcall_004d68e0(void* msg, void*, U value);                // CSWMessage::WriteBOOL
void __fastcall kotor_fastcall_004d6920(void* msg, void*, U value_bits, U scale, U bits); // CSWMessage::WriteFLOAT
void __fastcall kotor_fastcall_004d6960(void* msg, void*, const void* s, U bits);  // CSWMessage::WriteCExoString
void __fastcall kotor_fastcall_00578570(void* msg, void*, U id);                   // CSWSMessage::WriteOBJECTIDServer
U __fastcall kotor_fastcall_004d6440(void* msg, void*, void** data, U* size);      // CSWMessage::GetWriteMessage
U __fastcall kotor_fastcall_00567e60(void* msg, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage
void __fastcall kotor_fastcall_005785c0(void* msg, void*, void* loc, U gender);    // WriteCExoLocStringServer
struct P2sResRef16 { unsigned char bytes[16]; };
void __fastcall kotor_fastcall_004d6380(void* msg, void*, P2sResRef16 ref, U size); // CSWMessage::WriteCResRef (by value)
void* __fastcall kotor_fastcall_00561080(void* player, void*);                     // CSWSPlayer::GetGameObject
void* __fastcall recovered_server_player_list_004aed90(void* server, void*);       // CServerExoApp::GetPlayerList
void* __fastcall recovered_object_area_004cb120(void* object, void*);              // CSWSObject::GetArea
void* __fastcall kotor_fastcall_004ae770(void* server, void*, U id);               // GetCreatureByGameObjectID
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);       // CServerExoApp::GetSWSMessage
void* __fastcall server_get_module_004ae6b0(void* server, void*);                  // CServerExoApp::GetModule
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);         // CServerExoApp::GetServerAIMaster
void* __fastcall recovered_event_construct_004d7540(void* event, void*);           // CScriptEvent::CScriptEvent
U __fastcall recovered_event_enqueue_004b08d0(void* ai, void*, U day, U ms, U source, U target, U kind, void* data); // AddEventDeltaTime
U __fastcall kt_get_class_005a4e90(void* stats, void*, U index);                   // CSWSCreatureStats::GetClass (AL)
U8 __fastcall recovered_stats_class_level_005a5090(void* stats, void*, U index, U flag); // GetClassLevel (AL)
U __fastcall recovered_send_chat_00571960(void* msg, void*, U mode, U speaker, p2s_b::ExoStringArg text, U target,
                                          void* last);                             // SendServerToPlayerChatMessage RET 18h
int __fastcall async_string_equal_005e5200(void* s, void*, const void* other);    // CExoString::operator==(CExoString&)
U __fastcall kotor_fastcall_0056b2b0(void* message, void*, U player, U reason);    // SendServerToPlayerShutDownServer
std::uint64_t __fastcall recovered_bridge_exo_timers_high_resolution_005e9260(void* timers, void*); // GetHighResolutionTimer
U __fastcall recovered_var_int_0059a530(void* table, void*, void* name);           // CSWSScriptVarTable::GetInt
void __fastcall kotor_fastcall_0059a6f0(void* table, void*, void* name, U value, U flag); // CSWSScriptVarTable::SetInt
void* __fastcall recovered_var_match_0059a390(void* table, void*, void* name, U type, U create); // MatchIndex
U __cdecl recovered_get_locstring_server_00578870(U player, void* loc, void* loc2, void* out, U gender); // GetLocStringServer

// ---- exports of this file ----
U __fastcall recovered_player_to_server_quick_chat_005209b0(void* message, void*, void* player, U minor);
U __fastcall recovered_player_to_server_chat_00520a10(void* message, void*, void* player, U minor);
U __fastcall recovered_player_to_server_shut_down_server_00525390(void* message, void*, void* player, U minor);
U __fastcall recovered_player_to_server_player_death_005252a0(void* message, void*, void* player, U minor);
U __fastcall recovered_player_to_server_play_module_character_list_00527890(void* message, void*, void* player, U minor);
U __fastcall recovered_player_to_server_map_pin_00527380(void* message, void*, void* player, U minor);
U __fastcall kotor_fastcall_00571ec0(void* message, void*, U speaker, U value);
U __fastcall kotor_fastcall_0056d850(void* message, void*, U player, U speaker, U value);
U __fastcall kotor_fastcall_004ae9e0(void* server, void*, void* name);
U __fastcall kotor_fastcall_004b4960(void* internal, void*, void* name);
void __fastcall kotor_fastcall_004ae890(void* server, void*, U a1, U a2);
void __fastcall kotor_fastcall_004b69c0(void* internal, void*, U a1, U a2);
U __fastcall kotor_fastcall_005273d0(void* message, void*, void* player);
U __fastcall kotor_fastcall_005253f0(void* message, void*, void* player);
U __fastcall kotor_fastcall_0056b400(void* message, void*, U player, U creature, U flag);
U __fastcall kotor_fastcall_00524c70(void* message, void*, void* player);
U __fastcall kotor_fastcall_00524f10(void* message, void*, void* player);
U __fastcall kotor_fastcall_00525080(void* message, void*, void* player);
U __fastcall kotor_fastcall_0056a740(void* message, void*, void* player, U x, U y, U z, void* name, U index);
void __fastcall kotor_fastcall_0059a8c0(void* table, void*, void* name, U value_bits);
void __fastcall kotor_fastcall_0059a8e0(void* table, void*, void* name, const void* value);
void __fastcall kotor_fastcall_0059a900(void* table, void*, void* name, U value);
void __fastcall kotor_fastcall_0059aa50(void* table, void*, void* name);
void __fastcall kotor_fastcall_0059aa60(void* table, void*, void* name);
void __fastcall kotor_fastcall_0059aa70(void* table, void*, void* name);
void __fastcall kotor_fastcall_0059a4e0(void* table, void*, void* name, U type);
void __fastcall kotor_fastcall_0059a330(void* table, void*, I index);
}

namespace {
constexpr U kObjectInvalid_00746644 = 0x7f000000u;   // .rdata [00746644]
constexpr U kOne_3f800000 = 0x3f800000u;             // 1.0f pushed as the ReadFLOAT/WriteFLOAT scale
constexpr U kEventDelay_007441dc = 0x0au;            // .rdata [007441DC] (dword 0000000A)
constexpr float kQuickChatRange_00747444 = 1000.0f;  // .rdata [00747444] (447A0000)

// A CExoString returned by value into caller storage (the callee constructs it; caller destroys it).
struct ReturnedString {
    U d[2];
    ReturnedString() = default;
    ReturnedString(const ReturnedString&) = delete;
    ReturnedString& operator=(const ReturnedString&) = delete;
    ~ReturnedString() { frame_LogStringDtor_005e5c20(d, nullptr); }
};
// CExoString::operator=(ReadCExoString(bits)) with the temporary destroyed right after the assignment.
inline void read_string_into(void* message, void* dst) {
    ReturnedString tmp;
    str_assign(dst, kotor_fastcall_004d67e0(message, nullptr, tmp.d, 0x20));
}
inline void format(ExoString& s, const char* fmt, I i) {
    kotor_reconstruction::StartupFormat005e5680(reinterpret_cast<kotor_reconstruction::StartupString*>(&s), fmt, i);
}
inline void* list_internal(void* list) { return rp(list, 0); }
inline U fl_bits(float f) { return fbits(f); }
}  // namespace

// =================================================================================================================
// 005209B0 CSWSMessage::HandlePlayerToServerQuickChatMessage(player, minor) RET 8 (minor unused)
// =================================================================================================================
extern "C" U __fastcall recovered_player_to_server_quick_chat_005209b0(void* message, void*, void* player, U) {
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                     // 005209B1..005209C1
    const U value = kotor_fastcall_004d6750(message, nullptr, 0x10);                     // 005209C5..005209D2 edi
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                     // 005209CE..005209DB
    if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                     // 005209DD..005209E8
    void* go = kotor_fastcall_00561080(player, nullptr);                                  // 005209EA..005209EE
    if (go == nullptr) return 0;                                                          // 005209F3..005209F5
    return kotor_fastcall_00571ec0(message, nullptr, rd(go, 4), value);                   // 005209F7..00520A05
}

// 00571EC0 CSWSMessage::SendServerToPlayerQuickChatMessage(speaker_id, value) RET 8
extern "C" U __fastcall kotor_fastcall_00571ec0(void* message, void*, U speaker, U value) {
    U esi = 1;                                                                            // 00571ED1
    void* area = nullptr;                                                                 // 00571ED6 [esp+0Ch]
    float pos[3] = {0.0f, 0.0f, 0.0f};                                                    // 00571EDE..00571EEE
    void* players = recovered_server_player_list_004aed90(server(), nullptr);            // 00571EC3..00571EF6
    if (players == nullptr) return 0;                                                     // 00571EFB..00571F06 (eax)
    void* object;                                                                         // [esp+8]
    if (get_object(speaker, &object) && vcall<void*>(object, 0x30) != nullptr) {          // 00571F09..00571F3B
        area = recovered_object_area_004cb120(vcall<void*>(object, 0x30), nullptr);       // 00571F3D..00571F53
        void* c = vcall<void*>(object, 0x30);                                             // 00571F4D..00571F57
        std::memcpy(pos, fa(c, 0x90), 12);                                                // 00571F5A..00571F6F
    } else {
        esi = 0;                                                                          // 00571F75
    }
    void* cursor = rp(list_internal(players), 0);                                         // 00571F77..00571F7D
    if (cursor == nullptr) return esi;                                                    // 00571F81
    while (esi == 1) {                                                                    // 00571F90..00571F93
        void* pl = unresolved_list_at_pos_005e9b60(list_internal(players), nullptr, cursor); // 00571F99..00571FA1
        if (kotor_fastcall_00561080(pl, nullptr) != nullptr) {                            // 00571FA3..00571FAC
            void* go = kotor_fastcall_00561080(pl, nullptr);                              // 00571FB2..00571FB4
            const float dx = rdf(go, 0x90) - pos[0];                                      // 00571FB9..00571FCA (fstp dword)
            const float dy = rdf(go, 0x94) - pos[1];                                      // 00571FCE..00571FD5
            const float dz = rdf(go, 0x98) - pos[2];                                      // 00571FD9..00571FE0
            if (area == recovered_object_area_004cb120(kotor_fastcall_00561080(pl, nullptr), nullptr)) { // 00571FE4..00571FF4
                const double d2 = (static_cast<double>(dz) * dz + static_cast<double>(dy) * dy) +
                                  static_cast<double>(dx) * dx;                           // 00571FF6..00572010
                const double r2 = static_cast<double>(kQuickChatRange_00747444) * kQuickChatRange_00747444; // 00572012..00572018
                if (!(r2 < d2) && r2 == r2 && d2 == d2) {                                 // 0057201E..00572025 FCOMPP C0 (NaN skips)
                    esi = kotor_fastcall_0056d850(message, nullptr, rd(pl, 4), speaker, value) & 1u; // 00572027..0057203C
                }
            }
        }
        (void)unresolved_list_next_005e9b90(list_internal(players), nullptr, &cursor);   // 0057203F..00572046
        if (cursor == nullptr) break;                                                     // 0057204B..00572051
    }
    return esi;                                                                           // 00572057..00572060
}

// 0056D850 CSWSMessage::SendServerToPlayerQuickChat(player_id, speaker_id, value) RET 0Ch
extern "C" U __fastcall kotor_fastcall_0056d850(void* message, void*, U player, U speaker, U value) {
    kotor_fastcall_004d68a0(message, nullptr, 0x0a, 0xffffffffu, 1);                     // 0056D854..0056D85C
    kotor_fastcall_00578570(message, nullptr, speaker);                                   // 0056D861..0056D868
    kotor_fastcall_004d6300(message, nullptr, value, 0x10);                               // 0056D86D..0056D876
    void* data; U size;                                                                   // [esp+8] / [esp+4]
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;          // 0056D87B..0056D894
    return kotor_fastcall_00567e60(message, nullptr, player, 0x16, 0, data, size);        // 0056D897..0056D8B5
}

// =================================================================================================================
// 00520A10 CSWSMessage::HandlePlayerToServerChatMessage(player, minor) RET 8
// =================================================================================================================
extern "C" U __fastcall recovered_player_to_server_chat_00520a10(void* message, void*, void* player, U minor) {
    ExoString text;                                                                       // 00520A33..00520A37 [esp+20h]
    ExoString name;                                                                       // 00520A3C..00520A48 [esp+18h]
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                     // 00520A4D..00520A5D -> 00520B96
    U target;                                                                             // ebx
    if (static_cast<U8>(minor) == 4) {                                                    // 00520A63..00520A67
        if (kotor_fastcall_004d66f0(message, nullptr) == 0) {                             // 00520A69..00520A76
            target = kotor_fastcall_004d6780(message, nullptr, 0x20);                     // 00520A78..00520A7D
        } else {
            read_string_into(message, &name);                                             // 00520A81..00520AA3
            target = kotor_fastcall_004ae9e0(server(), nullptr, &name);                   // 00520AA8..00520ABB
        }
    } else {
        target = 0xffffffffu;                                                             // 00520ABF
    }
    read_string_into(message, &text);                                                     // 00520AC2..00520AE8
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                     // 00520AED..00520AF8
    if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                     // 00520AFE..00520B09
    void* go = kotor_fastcall_00561080(player, nullptr);                                  // 00520B0F..00520B12
    if (go == nullptr) return 0;                                                          // 00520B17..00520B19
    const U speaker = rd(go, 4);                                                          // 00520B1B edi
    U result;                                                                             // esi
    switch (static_cast<U8>(minor)) {                                                     // 00520B1E..00520B2F (index bytes 00520BD4)
    case 1: case 2: case 3: case 4: case 6: case 14: {                                    // table entry 0 -> 00520B36
        p2s_b::ExoStringArg copy;                                                         // 00520B3C sub esp,8
        (void)WindowCopy005e5b00(&copy, nullptr, &text);                                  // 00520B3F..00520B4A
        result = recovered_send_chat_00571960(message, nullptr, minor, speaker, copy, target, &name); // 00520B4F..00520B5B
        break;
    }
    default:                                                                              // table entry 1 / ja -> 00520B5F
        result = 0;
        break;
    }
    return result;                                                                        // 00520B61..00520B93 (name, text dtors)
}

// 004AE9E0 CServerExoApp::ResolvePlayerByFirstName(CExoString&) RET 4: thunk to the internal object
extern "C" U __fastcall kotor_fastcall_004ae9e0(void* server_app, void*, void* name) {
    return kotor_fastcall_004b4960(rp(server_app, 4), nullptr, name);                    // 004AE9E0..004AE9E3
}

// 004B4960 CServerExoAppInternal::ResolvePlayerByFirstName(CExoString&) RET 4
extern "C" U __fastcall kotor_fastcall_004b4960(void* self, void*, void* name) {
    ExoString first;                                                                      // 004B4983..004B498B [esp+18h]
    void* cursor = rp(rp(rp(self, 0x10064), 0), 0);                                       // 004B4990..004B49A4
    while (cursor != nullptr) {                                                           // 004B49A8 / 004B4A44
        void* pl = unresolved_list_at_pos_005e9b60(rp(rp(self, 0x10064), 0), nullptr, cursor); // 004B49B0..004B49BE
        if (pl != nullptr) {                                                              // 004B49C0..004B49C2
            void* go = kotor_fastcall_00561080(pl, nullptr);                              // 004B49C4..004B49C6
            if (go != nullptr) {                                                          // 004B49CB..004B49CD
                void* creature = vcall<void*>(go, 0x30);                                  // 004B49CF..004B49D3 AsSWSCreature
                if (creature != nullptr) {                                                // 004B49D8..004B49DA
                    {
                        ExoLocString last;                                                // 004B49DC..004B49E0 [esp+20h]
                        const U id = rd(pl, 4);                                           // 004B49E7
                        void* first_name = vcall<void*>(creature, 0x8c);                  // 004B49EA..004B49F9
                        (void)recovered_get_locstring_server_00578870(id, first_name, &last, &first, 0); // 004B49FF..004B4A06
                    }                                                                     // 004B4A09..004B4A12 ~CExoLocString
                    if (async_string_equal_005e5200(&first, nullptr, name) != 0)          // 004B4A17..004B4A26
                        return rd(pl, 4);                                                 // 004B4A77..004B4A9E
                }
            }
        }
        (void)unresolved_list_next_005e9b90(rp(rp(self, 0x10064), 0), nullptr, &cursor); // 004B4A2C..004B4A39
    }
    return kObjectInvalid_00746644;                                                       // 004B4A4A..004B4A74
}

// =================================================================================================================
// 00525390 CSWSMessage::HandlePlayerToServerShutDownServer(player, minor) RET 8 (player/minor unused)
// =================================================================================================================
extern "C" U __fastcall recovered_player_to_server_shut_down_server_00525390(void* message, void*, void*, U) {
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                     // 00525391..005253A1
    const U flag = kotor_fastcall_004d66f0(message, nullptr);                             // 005253A5..005253B0 edi
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                     // 005253AC..005253B9
    const U underrun = kotor_fastcall_004d6240(message, nullptr, 1);                      // 005253BB..005253BF
    if (underrun != 0) return 0;                                                          // 005253C4..005253C6
    // 005253D1..005253D3: sete al keeps EAX's upper bytes from the (zero) underflow result
    kotor_fastcall_004ae890(server(), nullptr, (underrun & 0xffffff00u) | (flag == 0 ? 1u : 0u), 1); // 005253C8..005253D9
    return 1;                                                                             // 005253DF..005253E5
}

// 004AE890 CServerExoApp::Shutdown(a1, a2) RET 8: thunk to the internal object
extern "C" void __fastcall kotor_fastcall_004ae890(void* server_app, void*, U a1, U a2) {
    kotor_fastcall_004b69c0(rp(server_app, 4), nullptr, a1, a2);                         // 004AE890..004AE893
}

namespace {
// Inlined CExoArrayList<unsigned long>::Add (data +0, size +4, allocated +8): grow to 2x (or 10h) when full.
void ulong_list_add(void* list, U value) {
    const U alloc = rd(list, 8);
    if (rd(list, 4) == alloc)                                                              // 004B6A62..004B6A6B
        menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                            alloc != 0 ? alloc + alloc : 0x10u);          // 004B6A6D..004B6A7D
    wr(rp(list, 0), rd(list, 4) * 4, value);                                              // 004B6A82..004B6A87
    wr(list, 4, rd(list, 4) + 1);                                                         // 004B6A8A
}
}  // namespace

// 004B69C0 CServerExoAppInternal::Shutdown(a1, a2) RET 8
extern "C" void __fastcall kotor_fastcall_004b69c0(void* self, void*, U a1, U a2) {
    const U mode = rd(rp(self, 0x10004), 0x98);                                           // 004B69C8..004B69CE ebx / [esp+14h]
    void* ids = fa(self, 0x1001c);                                                        // 004B69DB edi (CExoArrayList<ulong>)
    if (rd(ids, 0) != 0) {                                                                // 004B69D4..004B69E9
        live_free(rp(ids, 0));                                                            // 004B69EB..004B69F1 _free
        wr(ids, 0, 0);                                                                    // 004B69F4
        wr(ids, 8, 0);                                                                    // 004B69F6
    }
    wr(ids, 4, 0);                                                                        // 004B69FB
    void* cursor = rp(rp(rp(self, 0x10064), 0), 0);                                       // 004B69FE..004B6A08
    if (mode != 0 && a1 != 0) {                                                           // 004B69F9 / 004B6A0C..004B6A16
        while (cursor != nullptr) {                                                       // 004B6A1C / 004B6AA5
            void* pl = unresolved_list_at_pos_005e9b60(rp(rp(self, 0x10064), 0), nullptr, cursor); // 004B6A24..004B6A2D
            if (pl != nullptr) {                                                          // 004B6A32..004B6A36
                const std::uint64_t t = (static_cast<std::uint64_t>(rd(self, 0x1003c)) << 32) | rd(self, 0x10038);
                const U seconds = static_cast<U>(t / 100000u);                            // 004B6A38..004B6A4D __aulldiv(.., 186A0h)
                (void)kotor_fastcall_0056b2b0(rp(self, 0x10010), nullptr, rd(pl, 4), seconds); // 004B6A52..004B6A5D
                ulong_list_add(ids, rd(pl, 4));                                           // 004B6A62..004B6A8A
            }
            (void)unresolved_list_next_005e9b90(rp(rp(self, 0x10064), 0), nullptr, &cursor); // 004B6A8D..004B6A9A
        }
    } else if (cursor != nullptr) {                                                       // 004B6AB0..004B6AB7
        I budget = 0x100;                                                                 // 004B6AB2
        for (;;) {
            --budget;                                                                     // 004B6AC0
            if (budget <= 0) break;                                                       // 004B6AC1..004B6AC3
            void* pl = unresolved_list_at_pos_005e9b60(rp(rp(self, 0x10064), 0), nullptr, cursor); // 004B6AC9..004B6AD2
            if (pl == nullptr) {
                (void)unresolved_list_next_005e9b90(rp(rp(self, 0x10064), 0), nullptr, &cursor); // 004B6B2D..004B6B3A
            } else if (a2 != 0) {                                                         // 004B6ADB..004B6AE1
                ulong_list_add(ids, rd(pl, 4));                                           // 004B6AE3..004B6B0B
                (void)unresolved_list_next_005e9b90(rp(rp(self, 0x10064), 0), nullptr, &cursor); // 004B6B0E..004B6B3A
            } else {
                cursor = rp(rp(rp(self, 0x10064), 0), 0);                                 // 004B6B1D..004B6B27 restart at head
            }
            if (cursor == nullptr) break;                                                 // 004B6B3F..004B6B45
        }
    }
    // 004B6B4B: second list (+10068h): walk until every entry's id is in the collected list (bounded by 100h)
    cursor = rp(rp(rp(self, 0x10068), 0), 0);                                             // 004B6B4B..004B6B57
    I budget = 0x100;                                                                     // 004B6B5B
    if (cursor != nullptr) {                                                              // 004B6B60
        for (;;) {
            --budget;                                                                     // 004B6B62
            if (budget <= 0) break;                                                       // 004B6B63..004B6B65
            void* entry = unresolved_list_at_pos_005e9b60(rp(rp(self, 0x10068), 0), nullptr, cursor); // 004B6B67..004B6B70
            if (entry == nullptr) {
                (void)unresolved_list_next_005e9b90(rp(rp(self, 0x10068), 0), nullptr, &cursor); // 004B6BB5..004B6BC2
            } else {
                const U id = rd(entry, 4);                                                // 004B6B79
                const I n = static_cast<I>(rd(ids, 4));                                   // 004B6B7C
                U found = 0;                                                              // 004B6B7F
                for (I i = 0; i < n; ++i)                                                 // 004B6B81..004B6B90
                    if (rd(rp(ids, 0), static_cast<U>(i) * 4) == id) ++found;
                if (found != 0)                                                           // 004B6B92..004B6B94
                    (void)unresolved_list_next_005e9b90(rp(rp(self, 0x10068), 0), nullptr, &cursor); // 004B6BA6..004B6BC2
                else
                    cursor = rp(rp(rp(self, 0x10068), 0), 0);                             // 004B6B96..004B6BA0 restart at head
            }
            if (cursor == nullptr) break;                                                 // 004B6BC7..004B6BCD
        }
    }
    void* timers = rp(kotor_menu_10_19::dependencies::base_007a39e0, 4);                  // [007A39E0]+4 CExoTimers*
    std::uint64_t now;
    if (mode != 0 && a1 != 0 && rd(self, 0x10020) != 0) {                                 // 004B6BCF..004B6BE7
        now = recovered_bridge_exo_timers_high_resolution_005e9260(timers, nullptr);     // 004B6BE9..004B6BF2
        wr(self, 0x10028, 0x0aba9500u);                                                   // 004B6BF7 (180000000)
    } else {
        now = recovered_bridge_exo_timers_high_resolution_005e9260(timers, nullptr);     // 004B6C03..004B6C0B
        wr(self, 0x10028, 0x000f4240u);                                                   // 004B6C10 (1000000)
    }
    wr(self, 0x10030, static_cast<U>(now));                                               // 004B6C1B
    wr(self, 0x10034, static_cast<U>(now >> 32));                                         // 004B6C21
    wr(self, 0x1002c, 0);                                                                 // 004B6C27
}

// =================================================================================================================
// 005252A0 CSWSMessage::HandlePlayerToServerPlayerDeath(player, minor) RET 8
// =================================================================================================================
extern "C" U __fastcall recovered_player_to_server_player_death_005252a0(void*, void*, void* player, U minor) {
    if (static_cast<U8>(minor) == 1) {                                                    // 005252AE..005252BD
        void* module = server_get_module_004ae6b0(server(), nullptr);                     // 005252BF..005252C7
        const U target = rd(module, 0x20);                                                // 005252CC edi (005252CF add eax,1Ch is dead)
        void* memory = live_operator_new(0x34);                                           // 005252D2..005252D9 operator new
        void* event = memory != nullptr ? recovered_event_construct_004d7540(memory, nullptr) : nullptr; // 005252DC..005252F7
        wr16(event, 0, 0x21);                                                             // 00525305 (unchecked, as original)
        void* go = kotor_fastcall_00561080(player, nullptr);                              // 005252F9..0052530A
        (void)recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server(), nullptr), nullptr, 0, 0,
                                               rd(go, 4), target, kEventDelay_007441dc, event); // 0052530F..00525330
    }
    return 1;                                                                             // 00525335..0052534A
}

// =================================================================================================================
// 00527890 CSWSMessage::HandlePlayerToServerPlayModuleCharacterList(player, minor) RET 8
// =================================================================================================================
extern "C" U __fastcall recovered_player_to_server_play_module_character_list_00527890(void* message, void*, void* player,
                                                                                      U minor) {
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                     // 00527891..005278A1
    switch (static_cast<U8>(minor)) {                                                     // 005278A4..005278AD
    case 1: return kotor_fastcall_005273d0(message, nullptr, player);                     // 005278BF..005278CC
    case 2: return kotor_fastcall_005253f0(message, nullptr, player);                     // 005278AF..005278BC
    default: return 0;                                                                    // 0052789E
    }
}

// 005273D0 CSWSMessage::HandlePlayerToServerPlayModuleCharacterList_Start(player) RET 4
extern "C" U __fastcall kotor_fastcall_005273d0(void* message, void*, void* player) {
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                     // 005273D2..005273DD
    if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                     // 005273E3..005273EE
    void* players = recovered_server_player_list_004aed90(server(), nullptr);            // 005273F4..005273FE
    void* cursor = rp(list_internal(players), 0);                                         // 00527409..0052740F
    while (cursor != nullptr) {                                                           // 00527413 / 00527472
        void* pl = unresolved_list_at_pos_005e9b60(list_internal(players), nullptr, cursor); // 00527415..00527418
        if (pl != nullptr && kotor_fastcall_00561080(pl, nullptr) != nullptr) {           // 0052741D..0052742C
            const U id = rd(kotor_fastcall_00561080(pl, nullptr), 4);                     // 0052742E..00527435
            if (id != kObjectInvalid_00746644) {                                          // 00527438..0052743E
                void* msg = recovered_server_sws_message_004aedb0(server(), nullptr);    // 00527440..00527449
                if (msg != nullptr)                                                       // 0052744E..00527450
                    (void)kotor_fastcall_0056b400(msg, nullptr, rd(player, 4), id, 1);    // 00527452..0052745B
            }
        }
        (void)unresolved_list_next_005e9b90(list_internal(players), nullptr, &cursor);   // 00527460..00527467
    }
    wr(player, 0x48, 1);                                                                  // 00527475
    return 1;                                                                             // 0052747D
}

// 005253F0 CSWSMessage::HandlePlayerToServerPlayModuleCharacterList_Stop(player) RET 4
extern "C" U __fastcall kotor_fastcall_005253f0(void* message, void*, void* player) {
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                     // 005253F1..005253FC
    if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                     // 005253FE..00525409
    wr(player, 0x48, 0);                                                                  // 0052540B..0052540F
    return 1;                                                                             // 00525416
}

// 0056B400 CSWSMessage::SendServerToPlayerPlayModuleCharacterListResponse(player_id, creature_id, flag) RET 0Ch
extern "C" U __fastcall kotor_fastcall_0056b400(void* message, void*, U player, U creature_id, U flag) {
    U slot_size = 4;                                                                      // 0056B424 [esp+10h] (later the size out)
    void* data;                                                                           // [esp+14h]
    kotor_fastcall_004d68a0(message, nullptr, 4, 0xffffffffu, 1);                        // 0056B41C..0056B42C
    kotor_fastcall_004d68e0(message, nullptr, flag);                                      // 0056B431..0056B438
    kotor_fastcall_004d6340(message, nullptr, creature_id, 0x20);                         // 0056B43D..0056B446
    if (flag != 0) {                                                                      // 0056B44B..0056B44F
        void* c = kotor_fastcall_004ae770(server(), nullptr, creature_id);                // 0056B455..0056B45E
        if (c == nullptr) return 0;                                                       // 0056B463..0056B467 -> 0056B5D7
        U slot50 = flag;                                                                  // [esp+50h] argument slot reused
        U slot4c = creature_id;                                                           // [esp+4Ch] argument slot reused
        {
            ExoLocString first;                                                           // 0056B46D..0056B471 [esp+20h]
            ExoLocString last;                                                            // 0056B476..0056B47E [esp+18h]
            slot50 = (slot50 & 0xffffff00u) | (vcall<U>(c, 0x88) & 0xffu);               // 0056B483..0056B492 gender byte
            (void)recovered_localized_assign_005e9e70(&first, nullptr, vcall<void*>(c, 0x8c)); // 0056B496..0056B4A5
            (void)recovered_localized_assign_005e9e70(&last, nullptr, vcall<void*>(c, 0x90));  // 0056B4AA..0056B4B9
            kotor_fastcall_005785c0(message, nullptr, &first, slot50);                    // 0056B4BE..0056B4CA
            kotor_fastcall_005785c0(message, nullptr, &last, slot50);                     // 0056B4CF..0056B4D7
            const U portrait = vcall<U>(c, 0xd0);                                         // 0056B4DC..0056B4E0
            kotor_fastcall_004d6300(message, nullptr, portrait, 0x10);                    // 0056B4E6..0056B4ED
            const U16 p16 = static_cast<U16>(portrait);
            if (p16 == 0xffff || p16 == 0xfffe) {                                         // 0056B4F2..0056B4FE
                ResRef tmp;                                                               // [esp+28h]
                const void* ref = vcall<void*>(c, 0xc8, static_cast<void*>(&tmp));        // 0056B500..0056B509
                P2sResRef16 arg;
                std::memcpy(arg.bytes, ref, 16);                                          // 0056B50F..0056B52B
                kotor_fastcall_004d6380(message, nullptr, arg, 0x10);                     // 0056B511..0056B52E
            }
            const U8 classes = rd8(rp(c, 0xa74), 0x89);                                   // 0056B533..0056B539
            slot50 = (slot50 & 0xffffff00u) | classes;                                    // 0056B53F
            kotor_fastcall_004d62c0(message, nullptr, slot50, 8);                         // 0056B543..0056B54C
            for (I i = 0; i < static_cast<I>(classes); ++i) {                             // 0056B551..0056B5A1
                const U klass = kt_get_class_005a4e90(rp(c, 0xa74), nullptr, static_cast<U>(i)); // 0056B560..0056B567
                slot50 = (slot50 & 0xffffff00u) | (klass & 0xffu);                        // 0056B575
                const U8 level = recovered_stats_class_level_005a5090(rp(c, 0xa74), nullptr, static_cast<U>(i), 0); // 0056B56C..0056B579
                slot4c = (slot4c & 0xffffff00u) | level;                                  // 0056B587
                kotor_fastcall_004d62c0(message, nullptr, slot50, 8);                     // 0056B57E..0056B58B
                kotor_fastcall_004d62c0(message, nullptr, slot4c, 8);                     // 0056B590..0056B599
            }
        }                                                                                 // 0056B5A3..0056B5BD ~last, ~first
    }
    if (kotor_fastcall_004d6440(message, nullptr, &data, &slot_size) == 0) return 0;     // 0056B5C2..0056B5D7
    return kotor_fastcall_00567e60(message, nullptr, player, 0x31, 3, data, slot_size);   // 0056B5DB..0056B5F0
}

// =================================================================================================================
// 00527380 CSWSMessage::HandlePlayerToServerMapPinMessage(player, minor) RET 8 (no overflow check)
// =================================================================================================================
extern "C" U __fastcall recovered_player_to_server_map_pin_00527380(void* message, void*, void* player, U minor) {
    switch (static_cast<U8>(minor)) {                                                     // 00527380..00527390
    case 2: return kotor_fastcall_00524c70(message, nullptr, player);                     // 005273B4..005273BE
    case 3: return kotor_fastcall_00524f10(message, nullptr, player);                     // 005273A7..005273B1
    case 5: return kotor_fastcall_00525080(message, nullptr, player);                     // 0052739A..005273A4
    default: return 1;                                                                    // 00527392..00527397
    }
}

// 00524C70 CSWSMessage::HandlePlayerToServerMapPinSetMapPinAt(player) RET 4
extern "C" U __fastcall kotor_fastcall_00524c70(void* message, void*, void* player) {
    float pos[3] = {0.0f, 0.0f, 0.0f};                                                    // 00524C97..00524CA7 [esp+34h]
    ExoString text;                                                                       // 00524C93..00524CAF [esp+1Ch]
    ExoString key;                                                                        // 00524CB4..00524CC0 [esp+14h]
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                     // 00524CC5..00524CD5
    pos[0] = kotor_fastcall_004d67b0(message, nullptr, kOne_3f800000, 0x20);             // 00524CDB..00524CE9
    pos[1] = kotor_fastcall_004d67b0(message, nullptr, kOne_3f800000, 0x20);             // 00524CED..00524CFB
    pos[2] = kotor_fastcall_004d67b0(message, nullptr, kOne_3f800000, 0x20);             // 00524CFF..00524D0D
    read_string_into(message, &text);                                                     // 00524D11..00524D37
    if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                     // 00524D3C..00524D47
    void* go = kotor_fastcall_00561080(player, nullptr);                                  // 00524D49..00524D4C
    if (go == nullptr) return 0;                                                          // 00524D51..00524D53
    void* creature = vcall<void*>(go, 0x30);                                              // 00524D55..00524D59
    if (creature == nullptr) return 0;                                                    // 00524D5C..00524D62
    void* vars = fa(creature, 0x100);                                                     // 00524D64 CSWSScriptVarTable
    if (vars == nullptr) return 0;                                                        // 00524D6A..00524D6C
    ExoString total("NW_TOTAL_MAP_PINS");                                                 // 00524DA3..00524DAC [esp+24h]
    const I index = static_cast<I>(recovered_var_int_0059a530(vars, nullptr, &total)) + 1; // 00524DB1..00524DC6
    kotor_fastcall_0059a6f0(vars, nullptr, &total, static_cast<U>(index), 0);             // 00524DC4..00524DCF
    format(key, "NW_MAP_PIN_NTRY_%i", index);                                             // 00524DD4..00524DE4
    kotor_fastcall_0059a8e0(vars, nullptr, &key, &text);                                  // 00524DE7..00524DF3
    format(key, "NW_MAP_PIN_XPOS_%i", index);                                             // 00524DF8..00524E08
    kotor_fastcall_0059a8c0(vars, nullptr, &key, fl_bits(pos[0]));                        // 00524E08..00524E17
    format(key, "NW_MAP_PIN_YPOS_%i", index);                                             // 00524E1C..00524E2C
    kotor_fastcall_0059a8c0(vars, nullptr, &key, fl_bits(pos[1]));                        // 00524E2C..00524E3B
    format(key, "NW_MAP_PIN_ZPOS_%i", index);                                             // 00524E40..00524E50
    kotor_fastcall_0059a8c0(vars, nullptr, &key, fl_bits(pos[2]));                        // 00524E50..00524E5F
    format(key, "NW_MAP_PIN_AREA_%i", index);                                             // 00524E64..00524E74
    void* area = recovered_object_area_004cb120(creature, nullptr);                       // 00524E74..00524E7B
    kotor_fastcall_0059a900(vars, nullptr, &key, rd(fa(area, 0x11c), 4));                 // 00524E80..00524E90 (unchecked)
    (void)kotor_fastcall_0056a740(message, nullptr, player, fl_bits(pos[0]), fl_bits(pos[1]), fl_bits(pos[2]), &text,
                                  static_cast<U>(index));                                 // 00524E95..00524EBA
    return 1;                                                                             // 00524EBF..00524F02 (dtors)
}

// 00524F10 CSWSMessage::HandlePlayerToServerMapPinDestroyMapPin(player) RET 4
extern "C" U __fastcall kotor_fastcall_00524f10(void* message, void*, void* player) {
    ExoString key;                                                                        // 00524F32..00524F36 [esp+8]
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                     // 00524F3B..00524F4E
    const U index = kotor_fastcall_004d6780(message, nullptr, 0x20);                      // 00524F50..00524F5D edi
    if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                     // 00524F59..00524F66
    void* go = kotor_fastcall_00561080(player, nullptr);                                  // 00524F68..00524F6B
    if (go == nullptr) return 0;                                                          // 00524F70..00524F72
    void* creature = vcall<void*>(go, 0x30);                                              // 00524F74..00524F78
    if (creature == nullptr) return 0;                                                    // 00524F7B..00524F7D
    void* vars = fa(creature, 0x100);                                                     // 00524F7F
    if (vars == nullptr) return 0;                                                        // 00524F85..00524F87
    format(key, "NW_MAP_PIN_NTRY_%i", static_cast<I>(index));                             // 00524FAF..00524FBF
    kotor_fastcall_0059aa60(vars, nullptr, &key);                                         // 00524FC2..00524FC9 DestroyString
    format(key, "NW_MAP_PIN_XPOS_%i", static_cast<I>(index));                             // 00524FCE..00524FDE
    kotor_fastcall_0059aa50(vars, nullptr, &key);                                         // 00524FE1..00524FE8 DestroyFloat
    format(key, "NW_MAP_PIN_YPOS_%i", static_cast<I>(index));                             // 00524FED..00524FFD
    kotor_fastcall_0059aa50(vars, nullptr, &key);                                         // 00525000..00525007
    format(key, "NW_MAP_PIN_ZPOS_%i", static_cast<I>(index));                             // 0052500C..0052501C
    kotor_fastcall_0059aa50(vars, nullptr, &key);                                         // 0052501F..00525026
    format(key, "NW_MAP_PIN_AREA_%i", static_cast<I>(index));                             // 0052502B..0052503B
    kotor_fastcall_0059aa70(vars, nullptr, &key);                                         // 0052503E..00525045 DestroyObject
    return 1;                                                                             // 0052504A..00525070
}

// 00525080 CSWSMessage::HandlePlayerToServerMapPinChangePin(player) RET 4
extern "C" U __fastcall kotor_fastcall_00525080(void* message, void*, void* player) {
    ExoString text;                                                                       // 005250A3..005250A7 [esp+14h]
    ExoString key;                                                                        // 005250AC..005250B8 [esp+0Ch]
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                     // 005250BD..005250CD
    float pos[3];                                                                         // [esp+24h]
    pos[0] = kotor_fastcall_004d67b0(message, nullptr, kOne_3f800000, 0x20);             // 005250D3..005250E1
    pos[1] = kotor_fastcall_004d67b0(message, nullptr, kOne_3f800000, 0x20);             // 005250E5..005250F3
    pos[2] = kotor_fastcall_004d67b0(message, nullptr, kOne_3f800000, 0x20);             // 005250F7..00525105
    read_string_into(message, &text);                                                     // 00525109..0052512F
    const U index = kotor_fastcall_004d6780(message, nullptr, 0x20);                      // 00525134..00525141 edi
    if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                     // 0052513D..0052514A
    void* go = kotor_fastcall_00561080(player, nullptr);                                  // 0052514C..0052514F
    if (go == nullptr) return 0;                                                          // 00525154..00525156
    void* creature = vcall<void*>(go, 0x30);                                              // 00525158..0052515F ebx
    if (creature == nullptr) return 0;                                                    // 00525161..00525163
    void* vars = fa(creature, 0x100);                                                     // 00525165
    if (vars == nullptr) return 0;                                                        // 0052516B..0052516D
    format(key, "NW_MAP_PIN_NTRY_%i", static_cast<I>(index));                             // 005251A4..005251B4
    kotor_fastcall_0059a8e0(vars, nullptr, &key, &text);                                  // 005251B7..005251C3
    format(key, "NW_MAP_PIN_XPOS_%i", static_cast<I>(index));                             // 005251C8..005251D8
    kotor_fastcall_0059a8c0(vars, nullptr, &key, fl_bits(pos[0]));                        // 005251D8..005251E7
    format(key, "NW_MAP_PIN_YPOS_%i", static_cast<I>(index));                             // 005251EC..005251FC
    kotor_fastcall_0059a8c0(vars, nullptr, &key, fl_bits(pos[1]));                        // 005251FC..0052520B
    format(key, "NW_MAP_PIN_ZPOS_%i", static_cast<I>(index));                             // 00525210..00525220
    kotor_fastcall_0059a8c0(vars, nullptr, &key, fl_bits(pos[2]));                        // 00525220..0052522F
    format(key, "NW_MAP_PIN_AREA_%i", static_cast<I>(index));                             // 00525234..00525244
    void* area = recovered_object_area_004cb120(creature, nullptr);                       // 00525247..00525249
    kotor_fastcall_0059a900(vars, nullptr, &key, rd(fa(area, 0x11c), 4));                 // 0052524E..0052525E (unchecked)
    return 1;                                                                             // 00525263..00525298 (dtors)
}

// 0056A740 CSWSMessage::SendServerToPlayerMapPinAdded(CSWSPlayer*, Vector, CExoString*, index) RET 18h
extern "C" U __fastcall kotor_fastcall_0056a740(void* message, void*, void* player, U x, U y, U z, void* name, U index) {
    kotor_fastcall_004d68a0(message, nullptr, async_string_length_005e5790(name, nullptr) + 0x10, 0xffffffffu, 1); // 0056A743..0056A75A
    kotor_fastcall_004d6920(message, nullptr, x, kOne_3f800000, 0x20);                   // 0056A75F..0056A76D
    kotor_fastcall_004d6920(message, nullptr, y, kOne_3f800000, 0x20);                   // 0056A772..0056A780
    kotor_fastcall_004d6920(message, nullptr, z, kOne_3f800000, 0x20);                   // 0056A785..0056A793
    kotor_fastcall_004d6960(message, nullptr, name, 0x20);                                // 0056A798..0056A79D
    kotor_fastcall_004d6340(message, nullptr, index, 0x20);                               // 0056A7A2..0056A7AB
    void* data; U size;                                                                   // [esp+8] / name slot [esp+20h]
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;          // 0056A7B0..0056A7C8
    return kotor_fastcall_00567e60(message, nullptr, rd(player, 4), 0x20, 4, data, size); // 0056A7CB..0056A7EB
}

// ---- CSWSScriptVarTable (entries: {CExoString name; U type @+8; U value @+0Ch}, 10h bytes; data +0, count +4) ----
// 0059A8C0 SetFloat(name, value) RET 8
extern "C" void __fastcall kotor_fastcall_0059a8c0(void* table, void*, void* name, U value_bits) {
    wr(recovered_var_match_0059a390(table, nullptr, name, 2, 1), 0xc, value_bits);       // 0059A8C0..0059A8D5
}
// 0059A8E0 SetString(name, value) RET 8
extern "C" void __fastcall kotor_fastcall_0059a8e0(void* table, void*, void* name, const void* value) {
    str_assign(rp(recovered_var_match_0059a390(table, nullptr, name, 3, 1), 0xc), value); // 0059A8E0..0059A8FB
}
// 0059A900 SetObject(name, id) RET 8
extern "C" void __fastcall kotor_fastcall_0059a900(void* table, void*, void* name, U value) {
    wr(recovered_var_match_0059a390(table, nullptr, name, 4, 1), 0xc, value);            // 0059A900..0059A915
}
// 0059AA50 / 0059AA60 / 0059AA70 Destroy{Float,String,Object}(name) RET 4
extern "C" void __fastcall kotor_fastcall_0059aa50(void* table, void*, void* name) {
    kotor_fastcall_0059a4e0(table, nullptr, name, 2);                                     // 0059AA50..0059AA5C
}
extern "C" void __fastcall kotor_fastcall_0059aa60(void* table, void*, void* name) {
    kotor_fastcall_0059a4e0(table, nullptr, name, 3);                                     // 0059AA60..0059AA6C
}
extern "C" void __fastcall kotor_fastcall_0059aa70(void* table, void*, void* name) {
    kotor_fastcall_0059a4e0(table, nullptr, name, 4);                                     // 0059AA70..0059AA7C
}

// 0059A4E0 DeleteIndex(name, type) RET 8 (first match only; the entry's value is not released - original leak)
extern "C" void __fastcall kotor_fastcall_0059a4e0(void* table, void*, void* name, U type) {
    if (rdi(table, 4) <= 0) return;                                                       // 0059A4E4..0059A4EB
    I i = 0;                                                                              // 0059A4E7 ebx
    U off = 0;                                                                            // 0059A4F3 edi
    do {
        void* entry = fa(rp(table, 0), off);                                              // 0059A4F5..0059A4F7
        if (rd(entry, 8) == type && async_string_equal_005e5200(entry, nullptr, name) != 0) { // 0059A4FA..0059A50B
            kotor_fastcall_0059a330(table, nullptr, i);                                   // 0059A51F..0059A522
            return;
        }
        ++i; off += 0x10;                                                                 // 0059A510..0059A511
    } while (i < rdi(table, 4));                                                          // 0059A50D / 0059A514..0059A516
}

// 0059A330 internal_deleteIndex(index) RET 4: shift later entries down by one (name by CExoString::operator=)
extern "C" void __fastcall kotor_fastcall_0059a330(void* table, void*, I index) {
    const I count = rdi(table, 4) - 1;                                                    // 0059A333..0059A336
    wr(table, 4, static_cast<U>(count));                                                  // 0059A340
    if (index >= count) return;                                                           // 0059A33E..0059A343
    U off = static_cast<U>(index) << 4;                                                   // 0059A346
    do {
        void* dst = fa(rp(table, 0), off);                                                // 0059A350..0059A352
        void* src = fa(dst, 0x10);                                                        // 0059A355
        str_assign(dst, src);                                                             // 0059A358..0059A35B
        wr(dst, 8, rd(src, 8));                                                           // 0059A360..0059A367
        wr(dst, 0xc, rd(src, 0xc));                                                       // 0059A36A..0059A36D
        ++index; off += 0x10;                                                             // 0059A373..0059A374
    } while (index < rdi(table, 4));                                                      // 0059A370 / 0059A377..0059A37D
}
