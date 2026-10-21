// Lane W4 (party fork): client per-frame selection / combat-mode bodies that replace the menu-premise traps of
// frame_menu_path_client.cpp (same exported symbols), plus the client leaves they reach.
// Transcribed from dis/005F3AD0.asm (UpdateCombatMode), dis/005F9C60.asm (ShowObject), dis/005FA5A0.asm
// (DoPassiveSelection), dis/006146E0.asm (LookAt) and the leaves cited per function.
#include "7b616bc567-party.hpp"

using w4::U32;
namespace options_video { struct String; }
namespace client_movies { int __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*); }  // CExoString::operator!=
namespace directory_list { struct TimeList; void __fastcall AllocateTimes004c4f50(TimeList*, void*, std::int32_t); } // CExoArrayList<8-byte>::Allocate
namespace w4cam { extern std::uint32_t dialog_camera_debug_008327ec; }   // camera fork owner (src/camera/update_camera.cpp)
extern "C" {
unsigned __fastcall kotor_fastcall_004d8230(void* array, void*, unsigned id, void** out);           // CGameObjectArray::GetGameObject
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);          // CSWCCreature::GetServerCreature
void* __fastcall kotor_m15a_object_get_server_object_0063d4b0(void* object, void*);                // CSWCObject::GetServerObject
unsigned __fastcall kotor_m15_client_get_player_creature_id_005ed550(void* app, void*);            // CClientExoApp::GetPlayerCreatureId
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, int index);                       // CSWParty::GetCharacter
void* __fastcall kotor_fastcall_00635460(void* party, void*);                                      // CSWParty::GetPlayerCharacter (frame_menu_path_client)
void __fastcall kotor_fastcall_0062b000(void* in_game, void*, unsigned id, unsigned flag);         // CGuiInGame::SetMainInterfaceTarget
void __fastcall frame_RePopulate_0062b050(void* in_game, void*);                                   // CGuiInGame::RePopulateMainInterface
void* __fastcall kotor_fastcall_005ed580(void* app, void*, unsigned id);                           // CClientExoApp::GetGameObject
void __fastcall show_tutorial_window_005edf40(void* app, void*, int, unsigned, unsigned, int);      // CClientExoApp::ShowTutorialWindow
unsigned __fastcall frame_GetPauseState_004ae980(void* server, void*, unsigned);                    // CServerExoApp::GetPauseState
void __fastcall frame_SetAutoPaused_005f3f10(void* client, void*, unsigned, unsigned);              // CClientExoAppInternal::SetAutoPaused
void __fastcall gui_load_event_list_allocate_004186e0(void* list, void*, int count);                // CExoArrayList<12-byte>::Allocate
int __fastcall recovered_creature_incapacitated_004ef890(void* creature, void*);                   // CSWSCreature::GetDeadTemp
int __fastcall c2da_current_get_int_entry_00413510(void* table, void*, int row, int col, int* out); // C2DA::GetINTEntry
int __fastcall c2da_current_get_string_entry_00413190(void* table, void*, int row, int col, void* out); // C2DA::GetCExoStringEntry
int __fastcall kotor_m15b_c2da_get_float_entry_00413350(void* table, void*, int row, int col, float* out); // C2DA::GetFLOATEntry
void* __fastcall async_assign_string_005e5c50(void* self, void*, void* source);                    // CExoString::operator=(CExoString const&)
void* __fastcall frame_LogAssign_005e5140(void* self, void*, const char* text);                    // CExoString::operator=(char const*)
const char* __fastcall frame_LogCStr_005e5670(void* self, void*);                                  // CExoString::CStr
void __cdecl kotor_cdecl_00423bc0(W4Vector* out, const void* a, const void* b);                    // operator-(Vector, Vector)
W4Vector* __fastcall vector_add_assign_004a9740(W4Vector* self, void*, const W4Vector* other);     // Vector::operator+=
extern const char c2da_empty_0073d71c[];                                                            // "" (.rdata 0073D71C)
extern const float kotor_m15d_color_007a23d8[3];                                                    // [007A23D8] hilite colour
extern std::uint32_t kotor_rules_cell32_007a2100, kotor_rules_cell32_007a20fc;                      // appearance.2da column ids
extern std::uint32_t kotor_rules_cell32_007a20f4, kotor_rules_cell32_007a20f8;
std::int32_t __fastcall recovered_party_index_006349c0(void* party, void*, std::uint32_t id);              // CSWParty::GetIndex (W1 pending)
void __fastcall kotor_fastcall_0062b110(void* in_game, void*, std::uint32_t message);              // CGuiInGame::SetCombatMessage (W4 HUD)
// Cross-lane pending providers (see integration/party.json).
void __fastcall kotor_fastcall_004fc4c0(void* creature, void*, float, float, void* list_a, void* list_b); // CSWSCreature::GetNearestObjects (W1/W5)
void* __fastcall recovered_creature_client_creature_004efb20(void* creature, void*);                                   // CSWSCreature::GetClientCreature (W1/W5)
std::uint32_t __fastcall kotor_fastcall_00617ad0(void* creature, void*, void* object);             // CSWCCreature::CanSee (needs SceneHitCheck, W5 G17)
void __fastcall kotor_fastcall_00641540(void* module, void*);                                      // CSWCModule::SetCombatCamera (camera)
void __fastcall kotor_fastcall_00641dc0(void* module, void*);                                      // CSWCModule::RestoreCamera (camera)

// Definitions in this unit.
void __fastcall frame_DialogCheck_005f3ad0(void* client, void*);                                   // UpdateCombatMode
void __fastcall frame_DialogUpdate_005fa5a0(void* client, void*, float delta);                     // DoPassiveSelection
void __fastcall kotor_fastcall_005f9c60(void* client, void*, void* object, U32 look, U32 lock, U32 unused); // ShowObject
void __fastcall kotor_fastcall_005f3a80(void* client, void*, U32 mode);                            // CClientExoAppInternal::SetCombatMode
void __fastcall kotor_fastcall_005ede80(void* app, void*, U32 mode);                               // CClientExoApp::SetCombatMode
void __fastcall kotor_fastcall_00610a10(void* creature, void*, U32 mode);                          // CSWCCreature::SetCombatMode
void* __fastcall kotor_fastcall_00683ee0(void* door, void*);                                       // CSWCDoor::GetServerDoor
void* __fastcall kotor_fastcall_00682e00(void* placeable, void*);                                  // CSWCPlaceable::GetServerPlaceable
U32 __fastcall kotor_fastcall_006146e0(void* creature, void*, U32 target, float radius);           // CSWCCreature::LookAt
double __fastcall kotor_fastcall_0060e250(void* creature, void*);                                  // GetHeadTurnHorizontal
double __fastcall kotor_fastcall_0060e2a0(void* creature, void*);                                  // GetHeadTurnVertical
void __fastcall kotor_fastcall_00639c30(void* stick, void*, void* object);                         // CSWCameraOnAStick::SetLookAtTarget
void __fastcall kotor_fastcall_005f7c90(void* list, void*, int size);                              // CExoArrayList<CHostileHilite>::SetSize
}

using namespace w4;

namespace w4 {
volatile U32 selection_debug_008327f0 = 0;      // [008327F0] .data, read only (ShowObject, CSWSCreature::AIUpdate)
U32 lookat_override_008338d0[2] = {0, 0};       // [008338D0] static CExoString, never assigned by the original
}

namespace {
const U8 k_object_ok_0074666c = 0x00;      // byte [0074666C]
const U8 k_type_creature_00746634 = 0x05;  // byte [00746634]
const float k_zero = 0.0f, k_one = 1.0f;
void* client_objects(void* client) { return lp(client, 0x14); }       // CGameObjectArray
bool lookup(void* client, U32 id, void** out) {                        // GetGameObject + `cmp al,[0074666C]`
    *out = nullptr;
    return static_cast<U8>(kotor_fastcall_004d8230(client_objects(client), nullptr, id, out)) == k_object_ok_0074666c;
}



U32 to_client_id(U32 id) { return id == 0x7F000000u ? 0x7F000000u : (id | 0x80000000u); }   // 005FA6D3..005FA6ED

void* appearance_table() { return lp(lp(kotor_cell_rules_007a3a28, 0xB8), 0x0C); }     // [[[007A3A28]+B8h]+0Ch]
int appearance_row(void* creature) { return lh(lp(creature, 0x21C), 0x18); }            // movzx [[c+21Ch]+18h]
// Server-object "still a live combat target" test used three times by UpdateCombatMode.
bool server_alive(void* server, U32 id) {
    void* o = kotor_fastcall_004ae750(server, nullptr, id);
    return o != nullptr && vcall<void*>(o, 0x10) != nullptr && vcall<U32>(vcall<void*>(o, 0x10), 0x94) == 0;
}
}

// 00683EE0 CSWCDoor::GetServerDoor() / 00682E00 CSWCPlaceable::GetServerPlaceable() (thiscall, RET 0).
extern "C" void* __fastcall kotor_fastcall_00683ee0(void* door, void*) {
    if (kotor_m15a_object_get_server_object_0063d4b0(door, nullptr) == nullptr) return nullptr;    // 00683EE3..00683EEA
    return vcall<void*>(kotor_m15a_object_get_server_object_0063d4b0(door, nullptr), 0x18);       // 00683EEC..00683EF8
}
extern "C" void* __fastcall kotor_fastcall_00682e00(void* placeable, void*) {
    if (kotor_m15a_object_get_server_object_0063d4b0(placeable, nullptr) == nullptr) return nullptr; // 00682E03..00682E0A
    return vcall<void*>(kotor_m15a_object_get_server_object_0063d4b0(placeable, nullptr), 0x48);  // 00682E0C..00682E18
}

// 005F3A80 CClientExoAppInternal::SetCombatMode(int) (unlabelled; thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_005f3a80(void* client, void*, U32 mode) {
    if (mode == 0) sw(client, 0x3E0, 0);                                                     // 005F3A84..005F3A8B
    if (lw(client, 0x320) == mode) return;                                                   // 005F3A91..005F3A97
    sw(client, 0x320, mode);                                                                 // 005F3A9C
    frame_RePopulate_0062b050(lp(client, 0x40), nullptr);                                    // 005F3A99..005F3AA2
    void* module = lp(client, 0x18);                                                         // 005F3AA7
    if (module == nullptr) return;                                                           // 005F3AAA..005F3AAC
    if (lw(client, 0x320) != 0) kotor_fastcall_00641540(module, nullptr);                    // 005F3AAE..005F3AB8
    else kotor_fastcall_00641dc0(module, nullptr);                                           // 005F3AC1
}
// 005EDE80 CClientExoApp::SetCombatMode(int): mov ecx,[ecx+4]; jmp 005F3A80.
extern "C" void __fastcall kotor_fastcall_005ede80(void* app, void*, U32 mode) {
    kotor_fastcall_005f3a80(lp(app, 4), nullptr, mode);
}
// 00610A10 CSWCCreature::SetCombatMode(int) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_00610a10(void* creature, void*, U32 mode) {
    const U32 flags = lw(creature, 0x440);
    sw(creature, 0x440, flags ^ ((flags ^ mode) & 1u));                                      // 00610A13..00610A2A
    if (lw(creature, 4) != kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr)) return; // 00610A30..00610A45
    kotor_fastcall_005ede80(client_app(), nullptr, lw(creature, 0x440) & 1u);                // 00610A47..00610A5D (tail jmp)
}

// 005F3AD0 CClientExoAppInternal::UpdateCombatMode() (thiscall, RET 0). Replaces the frame_menu_path_client trap body.
extern "C" void __fastcall frame_DialogCheck_005f3ad0(void* client, void*) {
    void* const server = server_app();                                                       // 005F3AD3..005F3AF2 [esp+10h]
    void* object_target = nullptr;                                                           // ebx / [esp+0Ch] (persists across members)
    if (li(lp(client, 0x270), 0) > 0) {                                                      // 005F3AE0..005F3AFA
        for (I32 i = 0;;) {                                                                  // [esp+18h]
            void* member = frame_GetCharacter_006346c0(lp(client, 0x270), nullptr, i);       // 005F3B01..005F3B08 [esp+14h]
            if (member == nullptr) goto next;                                                // 005F3B0F..005F3B15
            if ((lw(member, 0x440) & 1u) != 0) {                                             // 005F3B1B..005F3B22
                void* target_creature = nullptr;                                             // ebp
                void* found = nullptr;
                if (lookup(client, lw(lp(client, 0x270), static_cast<U32>(i) * 0x88u + 0xA4), &found) && // 005F3B28..005F3B53
                    found != nullptr && lb(found, 8) == k_type_creature_00746634)            // 005F3B55..005F3B66
                    target_creature = vcall<void*>(found, 0x2C);                             // 005F3B68..005F3B6D
                if (target_creature == nullptr) {                                            // 005F3B6F..005F3B71
                    void* other = nullptr;
                    if (lookup(client, lw(lp(client, 0x270), static_cast<U32>(i) * 0x88u + 0xA4), &other) && other != nullptr) // 005F3B73..005F3BA4
                        object_target = vcall<void*>(other, 0x0C);                           // 005F3BA6..005F3BAF
                }

                if (i == 0 && jcc_je(lf(client, 0x378), k_zero, 0x41)) goto next;            // 005F3BB1..005F3BCA

                if (target_creature != nullptr && vcall<U32>(target_creature, 0x138) != 0) goto next; // 005F3BD0..005F3BE1
                if (object_target != nullptr) {                                              // 005F3BE7..005F3BE9
                    if (vcall<void*>(object_target, 0x14) != nullptr &&                      // 005F3BEB..005F3BF4
                        kotor_fastcall_00683ee0(vcall<void*>(object_target, 0x14), nullptr) != nullptr &&     // 005F3BF6..005F3C06
                        lw(kotor_fastcall_00683ee0(vcall<void*>(object_target, 0x14), nullptr), 0x2C4) != 0) // 005F3C08..005F3C1E
                        goto next;
                    if (vcall<void*>(object_target, 0x4C) != nullptr &&                      // 005F3C24..005F3C2D
                        kotor_fastcall_00682e00(vcall<void*>(object_target, 0x4C), nullptr) != nullptr &&     // 005F3C2F..005F3C3F
                        lw(kotor_fastcall_00682e00(vcall<void*>(object_target, 0x4C), nullptr), 0x260) != 0) // 005F3C41..005F3C57
                        goto next;
                }
                void* sc = kotor_m15a_creature_get_server_creature_0060fb20(member, nullptr); // 005F3C5D..005F3C68
                if (sc != nullptr) {
                    const U32 t0 = lw(sc, 0x50C), t2 = lw(sc, 0x524), t1 = lw(sc, 0x520);   // 005F3C6E..005F3C86
                    if (t0 != 0x7F000000u && server_alive(server, t0)) goto next;            // 005F3C74..005F3CBA
                    if (t2 != 0x7F000000u && t2 != t0 && server_alive(server, t2)) goto next; // 005F3CC4..005F3CFE
                    if (t1 != 0x7F000000u && t1 != t0 && t1 != t2) {                         // 005F3D08..005F3D15
                        void* o = kotor_fastcall_004ae750(server, nullptr, t1);              // 005F3D17..005F3D21
                        if (o != nullptr && vcall<void*>(o, 0x10) != nullptr &&               // 005F3D23..005F3D30
                            vcall<U32>(vcall<void*>(o, 0x10), 0x94) == 0) {                   // 005F3D32..005F3D45
                            if (vcall<void*>(o, 0x30) == nullptr) goto next;                 // 005F3D47..005F3D50
                            void* cc = recovered_creature_client_creature_004efb20(vcall<void*>(o, 0x30), nullptr); // 005F3D56..005F3D5F
                            if (cc == nullptr) goto next;                                    // 005F3D64..005F3D66
                            if (vcall<U32>(cc, 0x138) != 0) goto next;                       // 005F3D6C..005F3D78
                        }
                    }
                }
                sw(client, 0x394, 0xBF800000u);                                              // 005F3D88 (-1.0f)
                kotor_fastcall_00610a10(member, nullptr, 0);                                 // 005F3D82..005F3D92
            } else {
                void* sc = kotor_m15a_creature_get_server_creature_0060fb20(member, nullptr); // 005F3D9C..005F3DA7
                if (sc == nullptr) goto next;
                const U32 t0 = lw(sc, 0x50C), t2 = lw(sc, 0x524);                            // 005F3DAD..005F3DBF
                if (t0 != 0x7F000000u && server_alive(server, t0)) {                         // 005F3DB3..005F3DF3
                    kotor_fastcall_00610a10(member, nullptr, 1);                             // 005F3E39..005F3E3F
                    goto next;
                }
                if (t2 != 0x7F000000u && t2 != lw(sc, 4) && t2 != t0 && server_alive(server, t2)) // 005F3DF9..005F3E37
                    kotor_fastcall_00610a10(member, nullptr, 1);                             // 005F3E39..005F3E3F
            }
        next:
            if (++i >= li(lp(client, 0x270), 0)) break;                                      // 005F3E48..005F3E5B
        }
    }
    (void)object_target;
    // 005F3E62..005F3F03: combat message of the controlled character.
    U32 id = lw(client, 0x20);
    void* player;
    if (lw(client, 0x3BC) == id) player = lp(client, 0x3C0);                                 // 005F3E62..005F3E73
    else {
        player = pt(frame_R4_005ed5b0(client_app(), nullptr, id));                          // 005F3E75..005F3E7F
        id = lw(client, 0x20); sw(client, 0x3C0, ad(player)); sw(client, 0x3BC, id);         // 005F3E84..005F3E8D
    }
    if (player == nullptr) return;                                                           // 005F3E93..005F3E95
    if (lw(client, 0x3BC) == id) player = lp(client, 0x3C0);                                 // 005F3E97..005F3EA5
    else {
        player = pt(frame_R4_005ed5b0(client_app(), nullptr, id));                          // 005F3EA7..005F3EB0
        id = lw(client, 0x20); sw(client, 0x3C0, ad(player)); sw(client, 0x3BC, id);         // 005F3EB5..005F3EBE
    }
    if ((lw(player, 0x440) & 1u) == 0) return;                                               // 005F3EC4..005F3ECB
    const I32 index = recovered_party_index_006349c0(lp(client, 0x270), nullptr, id);               // 005F3ECD..005F3ED4
    if (index < 0) return;                                                                   // 005F3ED9..005F3EDB
    const U32 message = lw(lp(client, 0x270), static_cast<U32>(index) * 0x88u + 0xA8);       // 005F3EDD..005F3EED
    if (message == 0xBC50u) return;                                                          // 005F3EF3..005F3EF8
    kotor_fastcall_0062b110(lp(client, 0x40), nullptr, message);                             // 005F3EFA..005F3EFE
}

// 0060E250 / 0060E2A0 CSWCCreature::GetHeadTurnHorizontal / Vertical() (thiscall, RET 0; float in ST0).
extern "C" double __fastcall kotor_fastcall_0060e250(void* creature, void*) {
    float value;
    if (kotor_m15b_c2da_get_float_entry_00413350(appearance_table(), nullptr, appearance_row(creature),
                                                 static_cast<int>(kotor_rules_cell32_007a20f4), &value) == 0)
        return 40.0f;                                                                        // 0060E283 [00740A0C]
    return value;                                                                            // 0060E28B
}
extern "C" double __fastcall kotor_fastcall_0060e2a0(void* creature, void*) {
    float value;
    if (kotor_m15b_c2da_get_float_entry_00413350(appearance_table(), nullptr, appearance_row(creature),
                                                 static_cast<int>(kotor_rules_cell32_007a20f8), &value) == 0)
        return 30.0f;                                                                        // 0060E2D3 [007455DC]
    return value;                                                                            // 0060E2DB
}

// 006146E0 CSWCCreature::LookAt(id, float radius) (thiscall, RET 8; returns 0/1).
extern "C" U32 __fastcall kotor_fastcall_006146e0(void* creature, void*, U32 target, float radius) {
    U32 hook[2], bone[2];                                                                    // [esp+24h], [esp+1Ch] CExoString
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(hook), nullptr); // 00614703..00614707
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(bone), nullptr); // 0061470C..00614718
    U32 result = 0;
    if (lw(creature, 0x140) == 0 && lw(creature, 0x3E4) != target && lw(creature, 0x54) == 0 &&  // 0061471D..00614744
        lw(creature, 0x21C) != 0) {                                                          // 0061474A..00614752
        void* head = vcall<void*, U32, U32>(creature, 0x98, 0xFE, 0);                        // 00614758..00614763 ebx
        void* body = vcall<void*, U32, U32>(creature, 0x98, 0xFF, 0);                        // 00614769..00614776 [esp+18h]
        if (head != nullptr || body != nullptr) {                                            // 0061477C..00614788
            int tracks = 0;                                                                  // [esp+0Ch]
            const int ok = c2da_current_get_int_entry_00413510(appearance_table(), nullptr, appearance_row(creature),
                                                               static_cast<int>(kotor_rules_cell32_007a2100), &tracks); // 0061478A..006147B3
            bool stop = true;
            void* other = nullptr;
            if (ok != 0 && tracks != 0 && target != 0x7F000000u) {                           // 0061481B..0061482B
                stop = false;
                void* found = kotor_fastcall_005ed580(client_app(), nullptr, target);       // 0061482D..00614837
                other = found ? vcall<void*>(found, 0x0C) : nullptr;                         // 0061483C..00614847
                if (found == nullptr || other == nullptr ||                                  // 0061483E / 00614849..0061484B
                    vcall<void*, U32, U32>(other, 0x98, 0xFF, 0) == nullptr) goto done;      // 0061484D..00614860
                W4Vector d;                                                                  // [esp+2Ch]
                kotor_cdecl_00423bc0(&d, static_cast<U8*>(other) + 0x24, static_cast<U8*>(creature) + 0x24); // 00614862..0061486F
                const double d2 = (static_cast<double>(d.z) * d.z + static_cast<double>(d.y) * d.y) +
                                  static_cast<double>(d.x) * d.x;                            // 00614874..00614891
                const double r2 = static_cast<double>(radius) * radius;                      // 00614893..00614896



                if (jcc_jnp(r2, d2, 0x05)) goto done;                                        // 00614899..006148A0

                if (client_movies::StringNotEqual_005e5390(reinterpret_cast<options_video::String*>(lookat_override_008338d0), nullptr,
                                                           c2da_empty_0073d71c) != 0) {      // 006148A6..006148B7
                    async_assign_string_005e5c50(bone, nullptr, lookat_override_008338d0);   // 006148B9..006148C2
                } else if (c2da_current_get_string_entry_00413190(appearance_table(), nullptr, appearance_row(creature),
                                                                  static_cast<int>(kotor_rules_cell32_007a20fc), bone) == 0) { // 006148C9..006148F9
                    frame_LogAssign_005e5140(bone, nullptr, "hturn_g");                      // 006148FB..00614904 [0074F5A8]
                }
                const float horizontal = static_cast<float>(kotor_fastcall_0060e250(creature, nullptr)); // 00614909..00614910
                const float vertical = static_cast<float>(kotor_fastcall_0060e2a0(creature, nullptr));   // 00614914..0061491B
                if (lb(other, 8) == 5) frame_LogAssign_005e5140(hook, nullptr, "CAMERAHOOK");          // 0061491F..00614923, 00614940 [0074F59C]
                else frame_LogAssign_005e5140(hook, nullptr,
                                              vcall<const char*>(vcall<void*, U32, U32>(other, 0x98, 0xFF, 1), 0x0C)); // 00614925..0061493D
                sw(creature, 0x3E4, target);                                                 // 00614950..00614953
                if (head != nullptr)                                                         // 0061494E..00614959
                    vcall<void, void*, const char*, const char*, float, float, float, float>(
                        head, 0x118, vcall<void*, U32, U32>(other, 0x98, 0xFF, 1), frame_LogCStr_005e5670(hook, nullptr),
                        frame_LogCStr_005e5670(bone, nullptr), horizontal, vertical, 0.25f, radius); // 0061495B..00614998
                if (body != nullptr)                                                         // 0061499E..006149A4
                    vcall<void, void*, const char*, const char*, float, float, float, float>(
                        body, 0x118, vcall<void*, U32, U32>(other, 0x98, 0xFF, 1), frame_LogCStr_005e5670(hook, nullptr),
                        frame_LogCStr_005e5670(bone, nullptr), horizontal, vertical, 0.25f, radius); // 006149A6..006149E3
                result = 1;
                goto done;
            }
            if (stop) {                                                                      // 006147BC..006147E0
                sw(creature, 0x3E4, 0x7F000000u);
                if (head != nullptr) vcall<void>(head, 0x11C);
                if (body != nullptr) vcall<void>(body, 0x11C);
            }
        }
    }
done:
    frame_LogStringDtor_005e5c20(bone, nullptr);                                              // 006147E6..006147EF / 006149E9..006149F2
    frame_LogStringDtor_005e5c20(hook, nullptr);                                              // 006147F4..00614800 / 006149F7..00614A03
    return result;
}

// 00639C30 CSWCameraOnAStick::SetLookAtTarget(CSWCObject*) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_00639c30(void* stick, void*, void* object) {
    void* gob = lp(stick, 0x14);                                                             // 00639C36
    if (gob == nullptr || object == nullptr) return;                                         // 00639C39..00639C48
    sw(stick, 0x80, lw(object, 4));                                                          // 00639C4E..00639C51
    W4Vector position;                                                                       // [esp+20h]
    const W4Vector* p = vcall<W4Vector*, W4Vector*>(gob, 0x64, &position);                   // 00639C57..00639C5E Gob::GetPosition
    const float range = lf(stick, 0x110);                                                    // 00639C67 -> [esp+3Ch] (arg slot)
    W4Vector delta;                                                                          // [esp+10h] (fstp at esp-8)
    delta.x = static_cast<float>(static_cast<double>(p->x) - lf(object, 0x24));              // 00639C78..00639C86
    delta.y = static_cast<float>(static_cast<double>(p->y) - lf(object, 0x28));              // 00639C6D..00639C8A
    delta.z = static_cast<float>(static_cast<double>(p->z) - lf(object, 0x2C));              // 00639C61..00639C8E
    W4Vector unit;                                                                           // [esp+28h]
    const W4Vector* n = vector_normalized_004ab180(&unit, &delta);                           // 00639C92
    W4Vector offset;
    offset.x = static_cast<float>(static_cast<double>(range) * n->x);                        // 00639C97..00639CA5
    offset.y = static_cast<float>(static_cast<double>(range) * n->y);                        // 00639CAD..00639CB4
    offset.z = static_cast<float>(static_cast<double>(range) * n->z);                        // 00639CB8..00639CC8
    std::memcpy(static_cast<U8*>(stick) + 0x74, &offset, 12);                                // 00639CC3..00639CD0
    W4Vector again;                                                                          // [esp+2Ch]
    vector_add_assign_004a9740(reinterpret_cast<W4Vector*>(static_cast<U8*>(stick) + 0x74), nullptr,
                               vcall<W4Vector*, W4Vector*>(lp(stick, 0x14), 0x64, &again));  // 00639CD3..00639CE3
    sf(stick, 0x7C, static_cast<float>((static_cast<double>(lf(stick, 0x120)) + lf(stick, 0x6C)) + lf(stick, 0x7C))); // 00639CE8..00639CF4
}

// 005F7C90 CExoArrayList<CHostileHilite>::SetSize(int) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_005f7c90(void* list, void*, int size) {
    if (size == 0) {                                                                         // 005F7C96..005F7C9A
        if (lw(list, 0) == 0) { sw(list, 4, 0); return; }                                    // 005F7C9C..005F7CA0 -> 005F7CC0
        live_free(lp(list, 0));                                                              // 005F7CA2..005F7CA3
        sw(list, 0, 0); sw(list, 8, 0); sw(list, 4, 0);                                      // 005F7CAB..005F7CB0
        return;
    }
    directory_list::AllocateTimes004c4f50(static_cast<directory_list::TimeList*>(list), nullptr, size); // 005F7CB8..005F7CBB
    sw(list, 4, static_cast<U32>(size));                                                     // 005F7CC0
}

// 005F9C60 CClientExoAppInternal::ShowObject(CSWCObject*, int, int, int) (thiscall, RET 10h).
extern "C" void __fastcall kotor_fastcall_005f9c60(void* client, void*, void* object, U32 look, U32 lock, U32) {
    const U32 id = object ? lw(object, 4) : 0x7F000000u;                                     // 005F9C7D..005F9C89
    kotor_fastcall_0062b000(lp(client, 0x40), nullptr, id, 0);                               // 005F9C8E..005F9C94
    if (object == nullptr) {                                                                 // 005F9C99..005F9C9B
        void* character = frame_GetCharacter_006346c0(lp(client, 0x270), nullptr, 0);        // 005F9C9D..005F9CA4
        if (character != nullptr) kotor_fastcall_006146e0(character, nullptr, 0x7F000000u, 10.0f); // 005F9CB1..005F9CBD
        return;
    }
    if (look != 0) {                                                                         // 005F9CD5..005F9CE0
        void* character = frame_GetCharacter_006346c0(lp(client, 0x270), nullptr, 0);        // 005F9CE6..005F9CEE
        if (character != nullptr) kotor_fastcall_006146e0(character, nullptr, lw(object, 4), 10.0f); // 005F9CF3..005F9D02
        if (lock != 0 || lw(client, 0x320) != 0) {                                           // 005F9D07..005F9D1A
            void* stick = vcall<void*, U32>(lp(lp(client, 0x18), 0x40), 0x80, 0x106A);       // 005F9D20..005F9D2D GetBehavior
            if (stick != nullptr) kotor_fastcall_00639c30(stick, nullptr, object);           // 005F9D33..005F9D3A
            if (lw(client, 0x320) != 0 && vcall<void*>(object, 0x2C) != nullptr &&           // 005F9D3F..005F9D56
                vcall<U32>(vcall<void*>(object, 0x2C), 0x138) != 0) {                        // 005F9D5C..005F9D6F
                vcall<void, const float*, U32, U32, U32>(object, 0xA4, kotor_m15d_color_007a23d8, 0, 0, 1); // 005F9D71..005F9D80 Hilite
                const U32 quarter = 0x3E800000u;                                             // 005F9D90 (0.25f)
                U32 found = 0;
                const I32 count = li(client, 0x318);
                if (count > 0) {                                                             // 005F9D86..005F9D95
                    for (I32 i = 0; i < li(client, 0x318); ++i) {                            // 005F9D97..005F9DBD
                        if (found != 0) goto debug;                                          // 005F9D97..005F9D99 -> 005F9DE8
                        U8* entry = static_cast<U8*>(lp(client, 0x314)) + static_cast<U32>(i) * 8u;
                        if (lw(entry, 0) == lw(object, 4)) { sw(entry, 4, quarter); found = 1; } // 005F9DA1..005F9DAF
                    }
                    if (found != 0) goto debug;                                              // 005F9DBF..005F9DC1
                }
                const I32 n = li(client, 0x318);                                             // 005F9DC3
                kotor_fastcall_005f7c90(static_cast<U8*>(client) + 0x314, nullptr, n + 1);   // 005F9DC9..005F9DD5
                sw(static_cast<U8*>(lp(client, 0x314)) + static_cast<U32>(n) * 8u, 0, lw(object, 4)); // 005F9DDA..005F9DDF
                sw(static_cast<U8*>(lp(client, 0x314)) + static_cast<U32>(n) * 8u, 4, quarter);       // 005F9DE2..005F9DE4
            }
        }
    }
debug:
    // 005F9DE8..005FA2F3 / 005FA2F3..005FA589: debug overlays gated by [008327F0] / [008327EC]; neither cell has a
    // writer in the original image (MAINLOOP-PATHS.md method), so both arms are explicit traps.
    if (selection_debug_008327f0 != 0)
        kotor_unreachable_trap(0x005F9DF5u, "CClientExoAppInternal::ShowObject@005F9DF5 debug overlay [008327F0]");
    if (w4cam::dialog_camera_debug_008327ec != 0)
        kotor_unreachable_trap(0x005FA300u, "CClientExoAppInternal::ShowObject@005FA300 debug dump [008327EC]");
}

// 005FA5A0 CClientExoAppInternal::DoPassiveSelection(float) (thiscall, RET 4). Replaces the frame_menu_path_client trap.
extern "C" void __fastcall frame_DialogUpdate_005fa5a0(void* client, void*, float delta) {
    struct List { U32* data; I32 count; I32 capacity; };                                     // CExoArrayList<OBJECT_ID>
    U32 shown = 0;                                                                           // [esp+10h] -> [E-4Ch]
    void* character = kotor_fastcall_00635460(lp(client, 0x270), nullptr);                   // 005FA5BC..005FA5C2 [E-48h]
    void* sc = character ? kotor_m15a_creature_get_server_creature_0060fb20(character, nullptr) : nullptr; // 005FA5D9..005FA5DB
    if (character != nullptr && sc != nullptr) {                                             // 005FA5C9..005FA5E2
        List near_a{nullptr, 0, 0}, near_b{nullptr, 0, 0};                                   // [E-24h], [E-18h]
        U32 fallback_slot = f_bits(delta);                                                   // the delta argument slot [esp+68h], reused below
        kotor_fastcall_004fc4c0(sc, nullptr, 30.0f, 30.0f, &near_a, &near_b);               // 005FA5E8..005FA621
        const I32 count_b = near_b.count;                                                    // 005FA626..005FA62E [E-28h]
        sw(client, 0x2AC, 0);                                                                // 005FA632
        U8* entries_cell = static_cast<U8*>(client) + 0x2A8;                                 // ebx
        if (count_b == 0) {                                                                  // 005FA63E
            if (lw(entries_cell, 0) != 0) {                                                  // 005FA640..005FA644
                live_free(lp(entries_cell, 0));                                              // 005FA646..005FA647
                sw(entries_cell, 0, 0); sw(entries_cell, 8, 0);                              // 005FA64F..005FA651
            }
        } else {
            gui_load_event_list_allocate_004186e0(entries_cell, nullptr, count_b);           // 005FA656..005FA659
        }
        sw(entries_cell, 4, static_cast<U32>(count_b));                                      // 005FA65E..005FA660
        if (count_b == 0) goto release;                                                      // 005FA663
        {
            U32 found_current = 0, armed = 0, hostile = 0;                                   // [E-50h], [E-2Ch], [E-38h]
            U32 want_creature = 1, want_placeable = 1;                                       // [E-44h], [E-40h]
            // Integer address arithmetic: index -1 (not found) addresses entry[-1] exactly like the original.
            auto entry = [&](I32 k) { return static_cast<U8*>(pt(lw(client, 0x2A8) + static_cast<U32>(k) * 12u)); };
            for (I32 i = 0; i < count_b; ++i) {                                              // 005FA682..005FA7C9
                I32 matches = 0;
                for (I32 j = 0; j < near_a.count; ++j)                                       // 005FA690..005FA6B2
                    if (near_a.data[j] == near_b.data[i]) ++matches;
                U8* e = entry(i);
                sw(e, 8, lw(e, 8) ^ ((lw(e, 8) ^ static_cast<U32>(matches)) & 1u));          // 005FA6B4..005FA6D1
                near_b.data[i] = to_client_id(near_b.data[i]);                               // 005FA6D3..005FA6ED
                sw(entry(i), 0, near_b.data[i]);                                             // 005FA6F0..005FA6F9
                sb(entry(i), 4, 0xFF);                                                       // 005FA6FC..005FA6FE
                if (hostile == 0 || want_creature != 0 || want_placeable != 0) {             // 005FA703..005FA719
                    void* o;
                    if (lookup(client, lw(entry(i), 0), &o) && o != nullptr) {               // 005FA71F..005FA748
                        if (vcall<void*>(o, 0x2C) != nullptr && vcall<U32>(vcall<void*>(o, 0x2C), 0x138) != 0) { // 005FA74A..005FA76A
                            want_creature = 0; hostile = 1;                                  // 005FA76C..005FA774
                        } else if (vcall<void*>(o, 0x3C) != nullptr && vcall<U32>(vcall<void*>(o, 0x3C), 0x138) != 0) { // 005FA77E..005FA7A0
                            want_placeable = 0;                                              // 005FA7A2
                        }
                    }
                }
                if (lw(entry(i), 0) == lw(client, 0x2B4)) found_current = 1;                 // 005FA7AA..005FA7B7
            }
            // ---- 005FA7CF..005FA8C6: keep a hostile combat target for 1.5 s ----
            bool clear_timer = true;
            if (hostile != 0 && lw(client, 0x320) != 0 && found_current == 0) {              // 005FA7CF..005FA7EF
                const double timer = lf(client, 0x378);
                if (!jcc_jp(timer, k_zero, 0x41)) {                                          // 005FA7F5..005FA806
                    void* target = kotor_fastcall_004ae770(server_app(), nullptr,
                                                           kotor_fastcall_004aea30(server_app(), nullptr, lw(client, 0x2B4))); // 005FA808..005FA822
                    clear_timer = false;
                    if (target != nullptr && (vcall<U32>(target, 0x94) != 0 ||                // 005FA827..005FA83D
                                              recovered_creature_incapacitated_004ef890(target, nullptr) != 0)) { // 005FA83F..005FA848
                        sw(client, 0x378, 0x3FC00000u);                                      // 005FA84E (1.5f)
                        found_current = 1;
                    }
                } else {
                    const float left = static_cast<float>(timer - delta);                    // 005FA862..005FA86C
                    sf(client, 0x378, left);
                    clear_timer = false;
                    if (!jcc_jne(left, k_zero, 0x41)) {                                      // 005FA872..005FA87D
                        void* target = kotor_fastcall_004ae770(server_app(), nullptr,
                                                               kotor_fastcall_004aea30(server_app(), nullptr, lw(client, 0x2B4))); // 005FA87F..005FA898
                        if (target != nullptr && (vcall<U32>(target, 0x94) != 0 ||            // 005FA89F..005FA8AF
                                                  recovered_creature_incapacitated_004ef890(target, nullptr) != 0)) // 005FA8B1..005FA8BA
                            found_current = 1;                                               // 005FA8BC
                        else
                            clear_timer = true;                                              // 005FA8C6
                    }
                }
            }
            if (clear_timer) sw(client, 0x378, 0);                                           // 005FA8C6
            // ---- 005FA8D0..005FAAAD: pick a usable placeable / creature in sight ----
            U32 saw_creature = 0, saw_placeable = 0;                                         // [E-34h], [E-30h]
            if (want_creature == 0 || want_placeable == 0) {                                 // 005FA8D0..005FA8E6
                for (I32 k = 0; k < li(client, 0x2AC); ++k) {                                // 005FA8EC..005FAA43
                    void* o;
                    if (!lookup(client, lw(entry(k), 0), &o) || o == nullptr) continue;      // 005FA900..005FA931
                    if (vcall<void*>(o, 0x0C) == nullptr) continue;                          // 005FA937..005FA940
                    if (vcall<U32>(vcall<void*>(o, 0x0C), 0x138) == 0) continue;             // 005FA946..005FA959
                    if (lb(entry(k), 4) == 0xFF)                                             // 005FA95F..005FA96C
                        sb(entry(k), 4, static_cast<U8>(kotor_fastcall_00617ad0(character, nullptr, vcall<void*>(o, 0x0C)))); // 005FA96E..005FA97F
                    if (lb(entry(k), 4) == 0) continue;                                      // 005FA982..005FA98E
                    const bool usable = vcall<void*>(o, 0x3C) != nullptr && lw(vcall<void*>(o, 0x3C), 0x108) != 0; // 005FA994..005FA9AE
                    if (usable) saw_placeable = 1; else saw_creature = 1;                    // 005FA9B0..005FA9C0
                    void* server = server_app();                                             // 005FA9C4..005FA9CA
                    if (!((server != nullptr && frame_GetPauseState_004ae980(server, nullptr, 2) != 0) ||  // 005FA9CD..005FA9DA
                          lb(client, 0x398) != 0xFF)) {                                      // 005FA9DC..005FA9E3
                        if (vcall<void*>(o, 0x3C) != nullptr && lw(vcall<void*>(o, 0x3C), 0x108) != 0) { // 005FA9E5..005FA9FF
                            want_placeable = 1;                                              // 005FAA09
                            if (lw(client, 0x328) == 0) {                                    // 005FAA01..005FAA0D
                                if ((lw(lp(client, 4), 0x14) & 0x2000u) != 0 && lw(client, 0x320) == 0) // 005FAADF..005FAAF2
                                    frame_SetAutoPaused_005f3f10(client, nullptr, 1, 0x0B);  // 005FAAF4..005FAAF9
                                sw(client, 0x2B4, lw(entry(k), 0));                          // 005FAAFE..005FAB0A
                                found_current = 1; armed = 1;                                // 005FAAA5..005FAAA9
                                break;
                            }
                        } else {
                            want_creature = 1;                                               // 005FAA53
                            if (lw(client, 0x324) == 0) {                                    // 005FAA4B..005FAA57
                                show_tutorial_window_005edf40(client_app(), nullptr, 0x15, 0x7F000000u, 0x7F000000u, 0); // 005FAA59..005FAA70
                                if ((lw(lp(client, 4), 0x14) & 0x1000u) != 0 && lw(client, 0x320) == 0) // 005FAA75..005FAA88
                                    frame_SetAutoPaused_005f3f10(client, nullptr, 1, 1);     // 005FAA8A..005FAA8E
                                sw(client, 0x2B4, lw(entry(k), 0));                          // 005FAA93..005FAA9F
                                found_current = 1; armed = 1;                                // 005FAAA5..005FAAA9
                                break;
                            }
                            if (want_placeable != 0) break;                                  // 005FAA1B..005FAA21
                            continue;
                        }
                    }
                    if (want_creature != 0 && want_placeable != 0) break;                    // 005FAA13..005FAA21
                }
            }
            // ---- 005FAAAD..005FAB7F: 10 s memory of a seen creature / placeable ----
            if (lw(client, 0x324) != 0 && saw_creature == 0) {                               // 005FAAAD..005FAAC2
                if (jcc_jp(lf(client, 0x394), k_zero, 0x05))                                 // 005FAAC4..005FAAD5
                    sf(client, 0x394, static_cast<float>(static_cast<double>(lf(client, 0x394)) - delta)); // 005FAB12..005FAB1C
                else
                    sw(client, 0x324, 0);                                                    // 005FAAD7
            } else {
                sw(client, 0x324, saw_creature); sw(client, 0x394, 0x41200000u);             // 005FAB24..005FAB2E (10.0f)
            }
            if (lw(client, 0x328) != 0 && saw_placeable == 0) {                              // 005FAB34..005FAB40
                if (jcc_jp(lf(client, 0x3A0), k_zero, 0x05))                                 // 005FAB42..005FAB53
                    sf(client, 0x3A0, static_cast<float>(static_cast<double>(lf(client, 0x3A0)) - delta)); // 005FAB5D..005FAB67
                else
                    sw(client, 0x328, 0);                                                    // 005FAB55
            } else {
                sw(client, 0x328, saw_placeable); sw(client, 0x3A0, 0x41200000u);            // 005FAB6F..005FAB79
            }
            // ---- 005FAB7F..005FAC98: keep showing the current selection while visible ----
            if (found_current != 0) {                                                        // 005FAB7F..005FAB83
                void* o;
                if (lookup(client, lw(client, 0x2B4), &o) && o != nullptr) {                 // 005FAB89..005FABB3
                    I32 idx = -1;
                    for (I32 j = 0; j < near_b.count; ++j)                                   // 005FABB9..005FABDC
                        if (near_b.data[j] == lw(client, 0x2B4)) { idx = j; break; }
                    I32 visible;
                    if (idx == -1) {                                                         // 005FABF1..005FABF4 / 005FABDE
                        visible = static_cast<I32>(kotor_fastcall_00617ad0(character, nullptr, vcall<void*>(o, 0x0C))); // 005FABDE..005FABEA (full EAX)
                    } else {
                        U8* e = entry(idx);                                                  // 005FABF6..005FAC02
                        if (lb(e, 4) == 0xFF) sb(e, 4, static_cast<U8>(kotor_fastcall_00617ad0(character, nullptr, vcall<void*>(o, 0x0C)))); // 005FAC05..005FAC1C
                        visible = static_cast<I32>(static_cast<std::int8_t>(lb(entry(idx), 4))); // 005FAC1F..005FAC25 movsx
                    }
                    if (visible != 0) {                                                      // 005FAC2A..005FAC2E
                        sw(client, 0x368, 0);                                                // 005FAC36
                        const U32 flag = (armed == 0 && (lb(client, 0x37C) & 1u) == 0) ? 0u : 1u; // 005FAC30..005FAC59
                        kotor_fastcall_005f9c60(client, nullptr, vcall<void*>(o, 0x0C), flag, armed, 0); // 005FAC47..005FAFB5
                        shown = 1;                                                           // 005FAFBA
                        goto release;
                    }
                    const float lost = static_cast<float>(static_cast<double>(delta) + lf(client, 0x368)); // 005FAC66..005FAC70
                    sf(client, 0x368, lost);
                    if (!jcc_jp(lost, k_one, 0x05)) {                                        // 005FAC76..005FAC81
                        kotor_fastcall_005f9c60(client, nullptr, vcall<void*>(o, 0x0C), 0, 0, 0); // 005FAC83..005FAFB5
                        shown = 1;
                        goto release;
                    }
                    sw(client, 0x368, 0);                                                    // 005FAC92
                }
            }
            // ---- 005FAC98..005FAFC2: choose a new selection ----
            if (lw(client, 0x320) != 0 && hostile != 0) {                                    // 005FAC98..005FACA8
                fallback_slot = 0;                                                           // 005FACB6 [esp+68h]
                for (I32 k = 0; k < near_a.count; ++k) {                                     // 005FACBA..005FADCF
                    near_a.data[k] = to_client_id(near_a.data[k]);                           // 005FACC4..005FACDE
                    void* o;
                    if (!lookup(client, near_a.data[k], &o) || o == nullptr) continue;      // 005FACE1..005FAD10
                    if (vcall<void*>(o, 0x0C) == nullptr) continue;                          // 005FAD16..005FAD1F
                    if (vcall<U32>(vcall<void*>(o, 0x0C), 0x138) != 0) {                     // 005FAD25..005FAD38
                        I32 idx = -1;
                        for (I32 j = 0; j < near_b.count; ++j)                               // 005FAD3A..005FAD5E
                            if (near_b.data[j] == near_a.data[k]) { idx = j; break; }
                        U8* e = entry(idx);                                                  // 005FAD61..005FAD73 (idx -1 reads entry[-1] as the original)
                        if (lb(e, 4) == 0xFF) sb(e, 4, static_cast<U8>(kotor_fastcall_00617ad0(character, nullptr, vcall<void*>(o, 0x0C)))); // 005FAD76..005FAD89
                        if (lb(entry(idx), 4) == 0) continue;                                // 005FAD8C..005FAD98
                        sw(client, 0x2B4, near_a.data[k]);                                   // 005FAD9A..005FADA5
                        kotor_fastcall_005f9c60(client, nullptr, vcall<void*>(o, 0x0C), 1, 0, 0); // 005FADAB..005FAFB5
                        shown = 1;
                        goto release;
                    }
                    if (fallback_slot == 0) fallback_slot = ad(o);                           // 005FADB4..005FADBC
                }
                void* fallback = pt(fallback_slot);                                          // 005FADD5 ebx
                for (I32 k = 0; k < li(client, 0x2AC); ++k) {                                // 005FADD9..005FAE89
                    void* o;
                    if (!lookup(client, lw(entry(k), 0), &o) || o == nullptr) continue;      // 005FADF1..005FAE1E
                    if (vcall<void*>(o, 0x0C) == nullptr) continue;                          // 005FAE20..005FAE29
                    if (vcall<U32>(vcall<void*>(o, 0x0C), 0x138) == 0) continue;             // 005FAE2B..005FAE3E
                    if (lb(entry(k), 4) == 0xFF) sb(entry(k), 4, static_cast<U8>(kotor_fastcall_00617ad0(character, nullptr, vcall<void*>(o, 0x0C)))); // 005FAE40..005FAE60
                    if (lb(entry(k), 4) != 0) {                                              // 005FAE63..005FAE6F
                        sw(client, 0x2B4, lw(entry(k), 0));                                  // 005FAEB2..005FAEC2
                        kotor_fastcall_005f9c60(client, nullptr, vcall<void*>(o, 0x0C), 1, 0, 0); // 005FAEC8..005FAFB5
                        shown = 1;
                        goto release;
                    }
                    fallback = pt(fallback_slot);                                            // 005FAE71
                }
                if (fallback != nullptr) {                                                   // 005FAE8F..005FAE91
                    sw(client, 0x2B4, lw(fallback, 4));                                      // 005FAE97..005FAE9E
                    kotor_fastcall_005f9c60(client, nullptr, vcall<void*>(fallback, 0x0C), 1, 0, 0); // 005FAEA4..005FAFB5
                    shown = 1;
                }
                goto release;
            }
            for (I32 k = 0; k < near_a.count; ++k) {                                         // 005FAED1..005FAF92
                near_a.data[k] = to_client_id(near_a.data[k]);                               // 005FAEE0..005FAEFA
                void* o;
                if (!lookup(client, near_a.data[k], &o) || o == nullptr) continue;          // 005FAEFD..005FAF28 (out = [esp+68h])
                fallback_slot = ad(o);
                I32 idx = -1;
                for (I32 j = 0; j < near_b.count; ++j)                                       // 005FAF2A..005FAF4F
                    if (near_b.data[j] == near_a.data[k]) { idx = j; break; }
                U8* e = entry(idx);                                                          // 005FAF52..005FAF5E
                if (lb(e, 4) == 0xFF) sb(e, 4, static_cast<U8>(kotor_fastcall_00617ad0(character, nullptr, vcall<void*>(o, 0x0C)))); // 005FAF61..005FAF7A
                if (lb(entry(idx), 4) == 0) continue;                                        // 005FAF7D..005FAF89
                sw(client, 0x2B4, near_a.data[k]);                                           // 005FAF9A..005FAFA1
                kotor_fastcall_005f9c60(client, nullptr, vcall<void*>(o, 0x0C), 1, 0, 0);   // 005FAFA7..005FAFB5
                shown = 1;
                break;
            }
        }
    release:
        live_free(near_b.data);                                                              // 005FAFC2..005FAFCC
        live_free(near_a.data);                                                              // 005FAFD1..005FAFE4
    }
    if (shown == 0) {                                                                        // 005FAFE9..005FAFF8
        sw(client, 0x2B4, 0x7F000000u);                                                      // 005FB000
        kotor_fastcall_005f9c60(client, nullptr, nullptr, 0, 0, 0);                          // 005FAFFA..005FB00A
    }
    void* party = lp(client, 0x270);                                                         // 005FB00F
    if (party != nullptr) sw(party, 0xA4, lw(client, 0x2B4));                                // 005FB015..005FB01F
    sw(client, 0x37C, lw(client, 0x37C) & 0xFFFFFFFEu);                                      // 005FB025..005FB032
}
