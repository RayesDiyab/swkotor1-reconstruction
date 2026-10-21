// Lane W4 GUI successor (worker "gui-char"): CSWGuiPartySelection (0x3A80, table 00756D28) slot functions, its
// trapped event handler and the party-table / party helpers they reach that had no callable provider in c24.
//   006BE490 OnPanelRemoved        thiscall() RET            (slot)
//   006BEDE0 HandleInputEvent      thiscall(int, int) RET 8  (slot)
//   006BEEB0 OnPanelAdded          thiscall() RET            (slot)
//   006BF3B0 OnDone(CSWGuiControl*)          thiscall RET 4  (event handler, W4_GUI_TRAP_HANDLER in panels_menus.cpp)
// helpers reconstructed here:
//   006BE560 AcceptParty                     thiscall() RET
//   006BEC90 ConfirmPartySelection(int)      thiscall RET 4 (message-box callback)
//   00563710 CSWPartyTable::GetIsMember(int)            thiscall RET 4
//   00563810 CSWPartyTable::KillNPCObject(int, int)     thiscall RET 8
//   00565620 CSWPartyTable::AddMember(int, ulong)       thiscall RET 8
//   005EDD90 CClientExoApp::SetRunScript(const CExoString&)  thiscall RET 4 (tail call CExoString::operator=)
//   006348C0 CSWParty::GetWorldFormationPosition(Vector* out, Vector const&, Vector const&, int)  thiscall RET 10h
//   004B6950 CServerExoAppInternal::UpdateClientGameObjects(int)  thiscall RET 4
//            (the only existing body is the 0879 port function behind a file-local 0886 adapter; see report)
// Transcribed from the original swkotor.exe linear sweep; each statement carries its original PCs.
#include "6030a9ee65-claude_gui_char.hpp"

namespace options_video { struct String; }
namespace client_movies { int __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*); } // CExoString::operator!=
namespace frame_control { extern std::uint32_t message_manager_007a3a00; }                       // [007A3A00] CVirtualMachine*

namespace gchar_party { struct V3 { float x, y, z; }; }
extern "C" {
// ---- party table / party
void __fastcall kotor_fastcall_00565560(void* table, void*, w4::I32 npc);                        // CSWPartyTable::RemoveMember
void __fastcall w2x_party_mount_005638d0(void* table, void*);                                    // CSWPartyTable::AddGameInProgress
void __fastcall w2x_party_unmount_00563950(void* table, void*);                                  // CSWPartyTable::RemoveGameInProgress
void __fastcall kotor_fastcall_00563e80(void* table, void*, int index, int clear_actions);       // CSWPartyTable::SaveMember
unsigned __fastcall recovered_party_npc_selectable_005637c0(void* table, void*, unsigned npc);   // GetNPCSelectability (AL)
unsigned __fastcall recovered_spawn_npc_00565130(void* table, void*, unsigned index, unsigned use_given, void* pos, void* ori, unsigned flag); // SpawnNPC
void __fastcall recovered_transfer_inventory_005641e0(void* table, void*, void* creature);       // CSWPartyTable::TransferInventory
void __fastcall kotor_fastcall_00635480(void* party, void*, w4::I32 index);                      // CSWParty::SetLeader
void __fastcall kotor_fastcall_006359c0(void* party, void*);                                     // CSWParty::RecaulateFollowPoint
void* __fastcall kotor_fastcall_00634cc0(void* follow, void*, int index);                        // CSWPartyFollowDataArray::GetPathData
w4::I32 __fastcall recovered_party_index_006349c0(void* party, void*, w4::U32 id);               // CSWParty::GetIndex
// ---- creatures / objects
void __fastcall kotor_fastcall_0060ef50(void* creature, void*, w4::U32 in_party);                // CSWCCreature::SetInParty
void __fastcall recovered_set_in_party_004fdb20(void* creature, void*, w4::U32 in_party, w4::U32 flag); // CSWSCreature::SetInParty
void* __fastcall recovered_creature_client_creature_004efb20(void* creature, void*);             // CSWSCreature::GetClientCreature
void __fastcall recovered_force_equip_clothing_00501c40(void* creature, void*);                  // CSWSCreature::ForceEquipClothing
void* __fastcall recovered_creature_visible_element_00517a20(void* creature, void*, w4::U32 id); // GetVisibleListElement
void __fastcall recovered_creature_add_visible_0051b370(void* creature, void*, w4::U32 id, w4::U32, w4::U32, w4::U32, w4::U32); // AddToVisibleList
void __fastcall recovered_object_position_004cd1a0(void* object, void*, void* position, w4::U32, w4::U32, w4::U32); // CSWSObject::SetPosition
void __fastcall w1x_object_set_orientation_004cd170(void* object, void*, void* orientation);    // CSWSObject::SetOrientation
void __fastcall w5_object_clear_all_actions_004ccd80(void* object, void*, w4::U32 flag);        // CSWSObject::ClearAllActions
void __fastcall w5_pathfind_reset_way_point_data_005cf060(void* pathfind, void*);               // CPathfindInformation::ResetWayPointData
void __fastcall recovered_client_object_set_fade_state_0063e220(void* object, void*, w4::U32 state); // CSWCObject::SetFadeState
void __fastcall recovered_client_object_set_desired_fade_state_0063ce60(void* object, void*, w4::U32, w4::U32, w4::U32, w4::U32); // SetDesiredFadeState
// ---- server area / module
w4::U32 __fastcall recovered_compute_safe_location_004aeb60(void* server, void*, void* position, float radius, w4::U32 id, w4::U32, void* out); // ComputeSafeLocation
void* __fastcall server_get_module_004ae6b0(void* server, void*);                                // CServerExoApp::GetModule
void* __fastcall module_get_area_004c30f0(void* module, void*);                                  // CSWSModule::GetArea
w4::I32 __fastcall w5_area_test_direct_line_004bcb70(void* area, void*, const void* start, const void* end, const float* radius, float height, w4::U32, void*); // TestDirectLine
void __fastcall recovered_update_client_objects_004b3ec0(void* internal, void*, void* player, w4::U32 flag, w4::U32 lo, w4::U32 hi); // UpdateClientGameObjectsForPlayer
// ---- math
double __cdecl deg_to_rad_004a96d0(float degrees);                                               // DegToRad (ST0)
void* __fastcall quaternion_construct_004ac960(void* q, void*, gchar_party::V3 axis, float angle); // Quaternion::Quaternion(Vector, float)
void* __cdecl quaternion_rotate_004a9bb0(void* out, const void* quaternion, const void* vector); // operator*(Quaternion, Vector)
void* __cdecl rotation_arc_004ab630(void* out, gchar_party::V3 from, gchar_party::V3 to);        // RotationArc
// ---- resources / strings
void* __fastcall c2da_default_ref_00405ed0(void* self, void*);                                   // CResRef::CResRef()
void* __fastcall ref_assign_copy_00406120(void* ref, void*, void* out, const void* other);       // CResRef::operator= (by-value result)
void __fastcall frame_R4_005d0fc0(void* vm, void*, unsigned* script, unsigned object, unsigned flag); // CVirtualMachine::RunScript
// ---- party selection helpers (claude_menu_handlers_*.cpp / panels_menus.cpp)
void __fastcall kotor_fastcall_006be370(void* button, void*, w4::U32 selected);                  // CSWGuiPartySelectionButton::SetSelected
void __fastcall kotor_fastcall_006be4e0(void* self, void*);                                      // CSWGuiPartySelection::UpdateCount
// ---- defined here
w4::U32 __fastcall kotor_fastcall_00563710(void* table, void*, w4::I32 npc);
w4::U32 __fastcall kotor_fastcall_00563810(void* table, void*, w4::I32 npc, w4::U32 fade);
w4::U32 __fastcall kotor_fastcall_00565620(void* table, void*, w4::I32 npc, w4::U32 id);
void* __fastcall kotor_fastcall_005edd90(void* app, void*, void* script);
void* __fastcall kotor_fastcall_006348c0(void* party, void*, void* out, const void* position, const void* orientation, w4::I32 index);
void __fastcall kotor_fastcall_004b6950(void* internal, void*, w4::U32 flag);
void __fastcall kotor_fastcall_006be560(void* self, void*);
void __fastcall kotor_fastcall_006bec90(void* self, void*, w4::U32 arg);
void __fastcall kotor_fastcall_006be490(void* self, void*);
void __fastcall kotor_fastcall_006bede0(void* self, void*, std::uint32_t event, std::uint32_t down);
void __fastcall kotor_fastcall_006beeb0(void* self, void*);
void __fastcall kotor_fastcall_006bf3b0(void* self, void*, void* control);
}

using namespace gchar;
using gchar_party::V3;

namespace {
inline float fadd(float a, float b) { return static_cast<float>(static_cast<double>(a) + static_cast<double>(b)); } // fld/fadd/fstp dword
inline void copy12(void* dst, const void* src) { std::memcpy(dst, src, 12); }
// x87 `fld x; fcomp 0.0 [0073D700]; fnstsw; test ah,44h; jp` -> TRUE when x is not equal to 0 (or unordered).
inline bool not_zero(float x) { return jcc_jp(static_cast<double>(x), 0.0, 0x44); }
}  // namespace

extern "C" {
// 00563710 CSWPartyTable::GetIsMember(int npc) RET 4.
w4::U32 __fastcall kotor_fastcall_00563710(void* table, void*, w4::I32 npc) {
    const I32 count = li(table, 0);                                                            // 00563710
    for (I32 i = 0; i < count; ++i)                                                            // 00563715..0056372A
        if (npc == li(table, 4 + static_cast<U32>(i) * 4u)) return 1;                          // 00563720..00563722, 00563732
    return 0;                                                                                  // 0056372C
}

// 00563810 CSWPartyTable::KillNPCObject(int npc, int fade) RET 8.
w4::U32 __fastcall kotor_fastcall_00563810(void* table, void*, w4::I32 npc, w4::U32 fade) {
    if (npc < 0 || npc >= 9) return 0;                                                         // 00563816..00563820
    if (kotor_fastcall_00563710(table, nullptr, npc) != 0) return 0;                           // 00563822..0056382A
    const U32 slot = static_cast<U32>(npc) * 4u + 0xCu;
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, lw(table, slot));          // 0056382C..0056383F
    sw(table, slot, 0x7F000000u);                                                              // 00563843
    if (creature == nullptr) return 0;                                                         // 00563841..0056384B
    if (fade != 0) {                                                                           // 0056384D..00563853
        void* client = recovered_creature_client_creature_004efb20(creature, nullptr);         // 00563855..00563857
        if (client != nullptr) {                                                               // 0056385C..00563860
            recovered_client_object_set_fade_state_0063e220(client, nullptr, 0);               // 00563862..00563866
            recovered_client_object_set_desired_fade_state_0063ce60(client, nullptr, 0, 1, 0, 0); // 0056386B..00563875
            sw(client, 0xE4, 1);                                                               // 0056387A
        }
    }
    vcall<void*, U32>(creature, 0x00, 1);                                                      // 00563884..0056388A deleting destructor
    return 1;                                                                                  // 0056388E
}

// 00565620 CSWPartyTable::AddMember(int npc, ulong id) RET 8.
w4::U32 __fastcall kotor_fastcall_00565620(void* table, void*, w4::I32 npc, w4::U32 id) {
    if (li(table, 0xF0) != -1) return 0;                                                       // 00565625..0056562D
    if (li(table, 0) == 2) return 0;                                                           // 00565633..00565636
    if (npc < 0 || npc >= 9) return 0;                                                         // 0056563C..0056564B
    if (lw(table, static_cast<U32>(npc) * 4u + 0x30u) == 0) return 0;                          // 00565651..00565657
    if (kotor_fastcall_004ae770(server_app(), nullptr, id) == nullptr) return 0;               // 0056565D..00565671
    const I32 count = li(table, 0);                                                            // 00565677 edx
    for (I32 i = 0; i < li(table, 0); ++i)                                                     // 0056567B..00565692 (bound re-read)
        if (npc == li(table, 4 + static_cast<U32>(i) * 4u)) return 0;
    for (I32 i = 0; i < 9; ++i)                                                                // 00565694..005656B3
        if (i != npc && lw(table, 0xC + static_cast<U32>(i) * 4u) == id) return 0;
    sw(table, static_cast<U32>(count) * 4u + 4u, static_cast<U32>(npc));                       // 005656B5
    sw(table, static_cast<U32>(npc) * 4u + 0xCu, id);                                          // 005656B9
    void* client = client_app();                                                               // 005656BD..005656C3 edi
    void* creature = pt(frame_R4_005ed5b0(client, nullptr, kotor_fastcall_005eda50(client, nullptr, id))); // 005656C6..005656D1
    void* server_creature = kotor_fastcall_004ae770(server_app(), nullptr, id);                // 005656D6..005656E2
    if (creature != nullptr) kotor_fastcall_0060ef50(creature, nullptr, 1);                    // 005656E7..005656F5
    if (server_creature != nullptr) {                                                          // 005656FA..005656FC
        recovered_set_in_party_004fdb20(server_creature, nullptr, 1, 1);                       // 005656FE..00565702
        recovered_transfer_inventory_005641e0(table, nullptr, server_creature);                // 00565707..0056570A
        recovered_force_equip_clothing_00501c40(server_creature, nullptr);                     // 0056570F..00565711
    }
    if (li(table, 0) == 0) {                                                                   // 00565716..00565719
        void* server = server_app();                                                           // 0056571B..00565720 edi
        void* player = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 00565723..0056572D
        if (player != nullptr && lb(player, 0x4D1) == 1) sw(table, 0x190, 1);                  // 00565732..0056573E
    }
    sw(table, 0, lw(table, 0) + 1u);                                                           // 00565744..00565748
    return 1;                                                                                  // 0056574C
}

// 005EDD90 CClientExoApp::SetRunScript(const CExoString& script) RET 4.
void* __fastcall kotor_fastcall_005edd90(void* app, void*, void* script) {
    void* internal = lp(app, 4);                                                               // 005EDD90
    sw(internal, 0x32C, 1);                                                                    // 005EDD93
    return async_assign_string_005e5c50(at(internal, 0x330), nullptr, script);                 // 005EDD9D..005EDDA3 jmp operator=
}

// 006348C0 CSWParty::GetWorldFormationPosition(Vector* out, const Vector& position, const Vector& orientation, int index) RET 10h.
void* __fastcall kotor_fastcall_006348c0(void* party, void*, void* out, const void* position, const void* orientation, w4::I32 index) {
    V3 result = {0.0f, 0.0f, 0.0f};                                                            // 006348CD..006348DD [esp+8]
    if (index == 0) {                                                                          // 006348C8..006348E5
        std::memcpy(&result, position, 12);                                                    // 006348E7..006348F3
    } else if (index > 0 && index < 3) {                                                       // 006348F8..00634901
        V3 to; std::memcpy(&to, orientation, 12);                                              // 00634907..0063491D (by value)
        const V3 from = {0.0f, 1.0f, 0.0f};                                                    // 00634920..00634953
        unsigned char arc[16], rotated[12];
        void* q = rotation_arc_004ab630(arc, from, to);                                        // 00634942..00634956
        void* v = quaternion_rotate_004a9bb0(rotated, q, at(party, static_cast<U32>(index) * 0x88u + 0x5Cu)); // 0063495B..00634970
        const V3* p = static_cast<const V3*>(position);                                        // 00634975 [esp+60]
        const V3* r = static_cast<const V3*>(v);
        result.x = fadd(p->x, r->x);                                                           // 0063497C..00634980
        result.y = fadd(r->y, p->y);                                                           // 00634984..0063498A
        result.z = fadd(r->z, p->z);                                                           // 0063498E..00634994
    }
    std::memcpy(out, &result, 12);                                                             // 00634998..006349AF
    return out;                                                                                // 006349A4
}

// 004B6950 CServerExoAppInternal::UpdateClientGameObjects(int flag) RET 4: update every connected player.
void __fastcall kotor_fastcall_004b6950(void* internal, void*, w4::U32 flag) {
    const U64 now = high_resolution_timer();                                                   // 004B6951..004B695E [[007A39E0]+4]
    void* position = lp(lp(lp(internal, 0x10064), 0), 0);                                      // 004B6963..004B696F head
    if (position == nullptr) return;                                                           // 004B6971..004B6977
    do {
        void* list = lp(lp(internal, 0x10064), 0);                                             // 004B6980..004B6986 (re-read)
        void* player = pt(unresolved_list_at_pos_005e9b60(list, nullptr, position));           // 004B6988..004B6989
        recovered_update_client_objects_004b3ec0(internal, nullptr, player, flag,
                                                 static_cast<U32>(now), static_cast<U32>(now >> 32)); // 004B698E..004B6994
        unresolved_list_next_005e9b90(lp(lp(internal, 0x10064), 0), nullptr, &position);       // 004B6999..004B69A6
    } while (position != nullptr);                                                             // 004B69AB..004B69B1
}

// 006BE560 CSWGuiPartySelection::AcceptParty() RET 0.
void __fastcall kotor_fastcall_006be560(void* self, void*) {
    V3 position = {0.0f, 0.0f, 0.0f};                                                          // 006BE56E..006BE57E [esp+10]
    void* server = server_app();                                                               // 006BE586 [esp+1C]
    void* client = client_app();                                                               // 006BE589 [esp+44]
    U32 joined = 1;                                                                            // 006BE591 [esp+20]
    void* player = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 006BE5A1..006BE5B0 [esp+30]
    void* party = kotor_fastcall_005ed8b0(client, nullptr);                                    // 006BE5B4..006BE5BD ebx [esp+24]
    void* table = lane_config_required::GetPartyTable_004aee70(server, nullptr);               // 006BE5BB..006BE5C6 [esp+34]
    for (I32 npc = 0; npc < 9; ++npc) {                                                        // 006BE5CA..006BE6FE
        U8* entry = static_cast<U8*>(at(self, 0x4C8 + static_cast<U32>(npc) * 0x454u));        // esi = button+44Ch
        const I32 index = li(entry, 0);
        if (index == -1) continue;                                                             // 006BE5D2..006BE5D7
        if (lw(entry - 0x288, 0) != 0) continue;                                               // 006BE5DD..006BE5E5 (still selected)
        void* character = frame_GetCharacter_006346c0(party, nullptr, static_cast<U32>(index)); // 006BE5EB..006BE5EE [esp+2C]
        if (character == nullptr) continue;                                                    // 006BE5F5..006BE5FB
        for (U32 j = 0; j < 9; ++j) {                                                          // 006BE601..006BE6C1 (unrolled)
            const U32 other = 0x4C8 + j * 0x454u;
            const I32 value = li(self, other);
            if (value != -1 && value > li(entry, 0)) sw(self, other, static_cast<U32>(value - 1));
        }
        kotor_fastcall_0060ef50(character, nullptr, 0);                                        // 006BE6C7..006BE6C9
        void* server_creature = kotor_m15a_creature_get_server_creature_0060fb20(character, nullptr); // 006BE6CE..006BE6D2
        if (server_creature == nullptr) continue;                                              // 006BE6D7..006BE6D9
        recovered_set_in_party_004fdb20(server_creature, nullptr, 0, 1);                       // 006BE6DB..006BE6E1
        kotor_fastcall_00565560(table, nullptr, npc);                                          // 006BE6E6..006BE6EB RemoveMember
        sw(entry - 4, 0, lw(entry - 4, 0) & 0xFFFFFFFDu);                                      // 006BE6F0
    }
    V3 facing; copy12(&facing, at(player, 0x9C));                                              // 006BE704..006BE71F [esp+70]
    kotor_fastcall_006359c0(party, nullptr);                                                   // 006BE71D..006BE723 RecaulateFollowPoint
    for (I32 npc = 0; npc < 9; ++npc) {                                                        // 006BE728..006BEAA8
        U8* flags = static_cast<U8*>(at(self, 0x4C4 + static_cast<U32>(npc) * 0x454u));        // [esp+28] = button+448h
        if (li(flags, 4) != -1) continue;                                                      // 006BE734..006BE738
        if (lw(flags - 0x284, 0) == 0) continue;                                               // 006BE73E..006BE746 (not selected)
        U8* follow = static_cast<U8*>(party) + lw(party, 0) * 0x88u;                           // 006BE74C..006BE754 [esp+60]
        V3 offset; copy12(&offset, follow + 0x5C);                                             // 006BE757..006BE770 [esp+88]
        V3 local; copy12(&local, follow + 0x4C);                                               // 006BE777..006BE798 [esp+38]
        if (not_zero(local.x) || not_zero(local.y) || not_zero(local.z)) {                     // 006BE783..006BE7C3
            void* path = kotor_fastcall_00634cc0(at(party, 0x2DC), nullptr, li(follow, 0x58)); // 006BE7C9..006BE7D3
            const float degrees = lf(path, 0xC);                                               // 006BE7D8
            const V3 axis = {0.0f, 0.0f, 1.0f};                                                // 006BE7EA..006BE7FA [esp+54]
            const float radians = static_cast<float>(deg_to_rad_004a96d0(degrees));            // 006BE802..006BE807 fstp dword
            unsigned char quaternion[16], rotated[12];
            void* q = quaternion_construct_004ac960(quaternion, nullptr, axis, radians);       // 006BE80A..006BE82A
            const V3* r = static_cast<const V3*>(quaternion_rotate_004a9bb0(rotated, q, &offset)); // 006BE82F..006BE838
            position.x = fadd(local.x, r->x);                                                  // 006BE83D..006BE864
            position.y = fadd(local.y, r->y);
            position.z = fadd(local.z, r->z);                                                  // 006BE859..006BE874
        } else {
            unsigned char formation[12];
            void* p = kotor_fastcall_006348c0(party, nullptr, formation, at(player, 0x90), &facing, static_cast<I32>(joined)); // 006BE87A..006BE898
            copy12(&position, p);                                                              // 006BE89D..006BE8AD
        }
        V3 spawn_position = {0.0f, 0.0f, 0.0f}, spawn_orientation = {0.0f, 0.0f, 0.0f};        // 006BE8CA..006BE901 [esp+94] [esp+A0]
        const U32 id = recovered_spawn_npc_00565130(table, nullptr, static_cast<U32>(npc), 1, &spawn_position, &spawn_orientation, 1); // 006BE8B1..006BE90C
        recovered_compute_safe_location_004aeb60(server, nullptr, &position, bits_f(0x40A00000u), id, 1, &position); // 006BE911..006BE92B (5.0)
        void* creature = kotor_fastcall_004ae770(server, nullptr, id);                         // 006BE930..006BE933 esi
        void* client_creature = recovered_creature_client_creature_004efb20(creature, nullptr); // 006BE93A..006BE942 [esp+2C]
        void* leader = frame_GetCharacter_006346c0(party, nullptr, 0);                         // 006BE941..006BE94B
        V3 target; copy12(&target, at(leader, 0x24));                                          // 006BE950..006BE966 [esp+7C]
        void* pathfind = lp(creature, 0x340);                                                  // 006BE96D
        U32 scratch[2] = {lw(pathfind, 0x14), lw(pathfind, 4)};                                // 006BE973..006BE98D [esp+48] [esp+4C]
        void* area = module_get_area_004c30f0(server_get_module_004ae6b0(server, nullptr), nullptr); // 006BE99E..006BE9A5
        const I32 line = w5_area_test_direct_line_004bcb70(area, nullptr, &position, &target,
            reinterpret_cast<const float*>(&scratch[1]), bits_f(lw(pathfind, 0x14)), 1, nullptr); // 006BE976..006BE9AC
        if (line != 1 && line != -3) {                                                         // 006BE9B1..006BE9B9
            void* path = kotor_fastcall_00634cc0(at(party, 0x2DC), nullptr, li(follow, 0x58)); // 006BE9BB..006BE9CD
            copy12(&position, path);                                                           // 006BE9D2..006BE9F8
            recovered_compute_safe_location_004aeb60(server, nullptr, &position, bits_f(0x40A00000u), id, 1, &position); // 006BE9E2..006BE9FC
        }
        recovered_object_position_004cd1a0(creature, nullptr, &position, 1, 1, 0);             // 006BEA01..006BEA0E
        w1x_object_set_orientation_004cd170(creature, nullptr, &facing);                       // 006BEA13..006BEA1A
        kotor_fastcall_00565620(table, nullptr, npc, id);                                      // 006BEA1F..006BEA25 AddMember
        w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);                            // 006BEA2A..006BEA2E
        w5_pathfind_reset_way_point_data_005cf060(lp(creature, 0x340), nullptr);               // 006BEA33..006BEA39
        recovered_set_in_party_004fdb20(creature, nullptr, 1, 1);                              // 006BEA3E..006BEA44
        vcall<void>(creature, 0x70);                                                           // 006BEA49..006BEA4D
        if (recovered_creature_visible_element_00517a20(player, nullptr, lw(creature, 4)) == nullptr) // 006BEA50..006BEA61
            recovered_creature_add_visible_0051b370(player, nullptr, lw(creature, 4), 1, 1, 0, 0); // 006BEA63..006BEA6F
        if (client_creature != nullptr) kotor_fastcall_0060ef50(client_creature, nullptr, 1);  // 006BEA74..006BEA7E
        sw(flags, 0, lw(flags, 0) | 2u);                                                       // 006BEA83..006BEA99
        ++joined;                                                                              // 006BEA87..006BEA95
    }
    for (U32 i = 0; i < 9; ++i) {                                                              // 006BEAAE..006BEB2F (unrolled)
        const U32 off = 0x4C4 + i * 0x454u;
        sw(self, off, lw(self, off) & 0xFFFFFFFBu);
    }
    kotor_fastcall_004b6950(lp(server, 4), nullptr, 1);                                        // 006BEB11..006BEB3A UpdateClientGameObjects(1)
    void* script = at(self, 0x74);                                                             // 006BEB43 esi
    zero(self, 0x6C);                                                                          // 006BEB49
    kotor_fastcall_005edd90(client, nullptr, script);                                          // 006BEB3F..006BEB4C SetRunScript
    if (w4::ingame_gui_single_panel_007a2370 != 0) {                                           // 006BEB51..006BEB57
        U32 f = lw(self, 0x44) & 0xFFFFFEFFu;                                                  // 006BEB59..006BEB5C
        sw(self, 0x44, f);                                                                     // 006BEB6F
        if ((f & 0x600u) != 0x400u) sw(self, 0x44, (f & 0xFFFFFBFFu) | 0x200u);                // 006BEB61..006BEB7E
    } else {
        sw(ingame_gui(), 0x78, 0);                                                             // 006BEB83..006BEB90
        sw(self, 0x44, (lw(self, 0x44) & 0xFFFFFCFFu) | 0x400u);                               // 006BEB93..006BEBA2
    }
    kotor_fastcall_0040be00(manager_of(self), nullptr);                                        // 006BEBA5..006BEBA8 PopModalPanel
    if (client_movies::StringNotEqual_005e5390(static_cast<options_video::String*>(script), nullptr, "") != 0) { // 006BEBAD..006BEBBB
        sw(ingame_gui(), 0xC04, 1);                                                            // 006BEBBD..006BEBCD
        frame_R4_005d0fc0(pt(frame_control::message_manager_007a3a00), nullptr, static_cast<unsigned*>(script), 0, 0); // 006BEBCB..006BEBDE RunScript
        GuiAssign005e5140(script, nullptr, "");                                                // 006BEBE3..006BEBEA operator=("")
        sw(ingame_gui(), 0xC04, 0);                                                            // 006BEBEF..006BEBFC
    }
    if (lw(self, 0x64) == 0 && kotor_fastcall_005edc10(client_app(), nullptr) != 0)            // 006BEC02..006BEC17
        kotor_fastcall_005edc20(client_app(), nullptr, 0, 0, 0);                               // 006BEC19..006BEC25 SetPausedByCombat
    if (lw(self, 0x70) != 0) {                                                                 // 006BEC2A..006BEC2D
        play_sound(self, 5);                                                                   // 006BEC2F..006BEC34
        frame_R4_005d5e80(pt(frame_control::sound_manager_007a39ec), nullptr, kotor_rdata_sound_mode_0074c5ec); // 006BEC39..006BEC45
    }
    gui_m48_client_set_input_class_005eda60(client_app(), nullptr, lw(self, 0x64), 1);         // 006BEC4A..006BEC59
    sw(lp(ingame_gui(), 0x98), 0x97C, lw(self, 0x64));                                         // 006BEC5E..006BEC77
}

// 006BEC90 CSWGuiPartySelection::ConfirmPartySelection(int) RET 4 (CSWGuiMessageBox callback).
void __fastcall kotor_fastcall_006bec90(void* self, void*, w4::U32) {
    if ((lb(lp(ingame_gui(), 0x98), 0x64) & 1) != 0) kotor_fastcall_006be560(self, nullptr);   // 006BEC90..006BECAE
}

// 006BE490 OnPanelRemoved: kill the NPC objects that are not members (fade out), then the base.
void __fastcall kotor_fastcall_006be490(void* self, void*) {
    void* table = party_table();                                                               // 006BE490..006BE4A3 ebx
    for (I32 npc = 0; npc < 9; ++npc)                                                          // 006BE4A5..006BE4C9
        if ((lb(self, 0x4C4 + static_cast<U32>(npc) * 0x454u) & 2) == 0)                       // 006BE4B0..006BE4B3
            kotor_fastcall_00563810(table, nullptr, npc, 1);                                   // 006BE4B5..006BE4BA
    gui_load_panel_on_removed_0040c170(self, nullptr);                                         // 006BE4D1 jmp
}

// 006BEDE0 HandleInputEvent(int event, int down) RET 8.
void __fastcall kotor_fastcall_006bede0(void* self, void*, std::uint32_t event, std::uint32_t down) {
    if (down != 0 && (event == 0x28 || event == 0x2E)) {                                       // 006BEDE0..006BEDFB
        play_sound(self, 0);                                                                   // 006BEE01..006BEE07
        if (lw(self, 0x70) == 0 && lw(self, 0x6C) == 0) {                                      // 006BEE0C..006BEE18
            for (U32 i = 0; i < 9; ++i) {                                                      // 006BEE1A..006BEE40
                void* button = at(self, 0x7C + i * 0x454u);
                kotor_fastcall_006be370(button, nullptr, li(button, 0x44C) != -1 ? 1u : 0u);   // 006BEE23..006BEE34 SetSelected
            }
            kotor_fastcall_006be560(self, nullptr);                                            // 006BEE42..006BEE44 AcceptParty
        } else {
            void* box = lp(ingame_gui(), 0x98);                                                // 006BEE4C..006BEE5A
            kotor_fastcall_00627130(box, nullptr, 0);                                          // 006BEE60..006BEE64 SetAllowCancel(0)
            gui_load_messagebox_set_callback_00624a40(box, nullptr, nullptr, 0, 0);            // 006BEE69..006BEE71
            vcall<void, U32>(box, 0x6C, 0xA5A5);                                               // 006BEE76..006BEE7F
            kotor_fastcall_0040bc70(manager_of(self), nullptr, box, 1, 1);                     // 006BEE82..006BEE8A AddPanel
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, down);                          // 006BEE90..006BEE9C
}

// 006BEEB0 OnPanelAdded: rebuild the nine NPC buttons from the party table.
void __fastcall kotor_fastcall_006beeb0(void* self, void*) {
    void* server = server_app();                                                               // 006BEEB3..006BEEBB [esp+18]
    void* client = client_app();                                                               // 006BEEBE [esp+20]
    void* party = kotor_fastcall_005ed8b0(client, nullptr);                                    // 006BEEC4..006BEED7 edi [esp+24]
    void* table = lane_config_required::GetPartyTable_004aee70(server, nullptr);               // 006BEED9..006BEEE4 esi [esp+10]
    if (lw(self, 0x6C) == 0 && lw(self, 0x70) == 0) {                                          // 006BEEE6..006BEEF6
        gui_load_text_params_set_color_00414e10(at(self, 0x3860), nullptr, borrowed_exit_color_007a23b4); // 006BEEF8..006BEF03
        copy12(at(self, 0x3784), kotor_m1_color_007a2378);                                     // 006BEF08..006BEF25
    } else {
        gui_load_text_params_set_color_00414e10(at(self, 0x3860), nullptr, kotor_m1_color_007a2408); // 006BEF2A..006BEF35
        copy12(at(self, 0x3784), kotor_m1_color_007a2384);                                     // 006BEF3A..006BEF55
    }
    sw(self, 0x68, lw(self, 0x6C) != 0 ? 0u : lw(table, 0));                                   // 006BEF58..006BEF64
    kotor_fastcall_00635480(party, nullptr, -3);                                               // 006BEF67..006BEF6B SetLeader(-3)
    w2x_party_mount_005638d0(table, nullptr);                                                  // 006BEF70..006BEF72 AddGameInProgress
    for (I32 npc = 0; npc < 9; ++npc) {                                                        // 006BEF77..006BF240
        U8* e = static_cast<U8*>(at(self, 0x108 + static_cast<U32>(npc) * 0x454u));            // esi = button+8Ch
        unsigned char portrait[16];
        c2da_default_ref_00405ed0(portrait, nullptr);                                          // 006BEF81..006BEF85 CResRef() [esp+28]
        sw(e, 0x138, 0); sw(e, 0, 0);                                                          // 006BEF8A..006BEF94
        copy12(e + 0x78, borrowed_enter_color_007a23c0);                                       // 006BEF9A..006BEFBD
        gui_load_border_params_set_pulsing_00414d00(e + 0x68, nullptr, 1, 1, 0);               // 006BEFAC..006BEFC0
        if (kotor_fastcall_005636b0(table, nullptr, npc) == 0) {                               // 006BEFC5..006BEFD1
            sw(e, 0x3BC, (lw(e, 0x3BC) & 0xFFFFFFFEu) | 2u);                                   // 006BF208..006BF214
            sw(e, 0x3C0, 0xFFFFFFFFu);                                                         // 006BF21A
            sw(e, 0x180, lw(e, 0x180) | 2u);                                                   // 006BF224
            sw(e, 0x2C0, lw(e, 0x2C0) & 0xFFFFFFFDu);                                          // 006BF22B
            continue;
        }
        U32 object = recovered_npc_object_00564700(table, nullptr, npc, 0, 1);                 // 006BEFD7..006BEFE9 ebp
        void* creature = kotor_fastcall_004ae770(server, nullptr, object);                     // 006BEFEB..006BEFF1
        U32 flags;
        if (creature != nullptr) {                                                             // 006BEFF3..006BEFF5
            kotor_fastcall_00563e80(table, nullptr, npc, 0);                                   // 006BEFF7..006BF002 SaveMember
            unsigned char ref[16], result[16];
            void* r = vcall<void*, void*>(creature, 0xC8, ref);                                // 006BF007..006BF010
            ref_assign_copy_00406120(portrait, nullptr, result, r);                            // 006BF016..006BF020
            flags = lw(e, 0x3BC) | 2u;                                                         // 006BF025..006BF02B
        } else {
            object = recovered_npc_object_00564700(table, nullptr, npc, 1, 1);                 // 006BF035..006BF04E
            void* other = kotor_fastcall_004ae770(server, nullptr, object);                    // 006BF050..006BF051
            if (other != nullptr) {                                                            // 006BF056..006BF058
                unsigned char ref[16], result[16];
                void* r = vcall<void*, void*>(other, 0xC8, ref);                               // 006BF05A..006BF063
                ref_assign_copy_00406120(portrait, nullptr, result, r);                        // 006BF069..006BF073
            }
            flags = lw(e, 0x3BC) & 0xFFFFFFFDu;                                                // 006BF078..006BF07E
        }
        sw(e, 0x3BC, flags);                                                                   // 006BF085
        sw(e, 0x3BC, flags | 1u);                                                              // 006BF08B..006BF08D
        if (kotor_fastcall_00563710(table, nullptr, npc) != 0)                                 // 006BF093..006BF09F GetIsMember
            sw(e, 0x3C0, static_cast<U32>(recovered_party_index_006349c0(party, nullptr,
                kotor_fastcall_005eda50(client, nullptr, object))));                           // 006BF0A1..006BF0B5
        else
            sw(e, 0x3C0, 0xFFFFFFFFu);                                                         // 006BF0BD
        if (lw(self, 0x6C) != 0) {                                                             // 006BF0C7..006BF0D0
            if ((lb(e, 0x3BC) & 4) != 0) {                                                     // 006BF0D2..006BF0D9
                sw(e, 0x138, 1); sw(e, 0, 0x3F800000u);                                        // 006BF0DF..006BF0E5
                copy12(e + 4, kotor_m15d_color_007a23e4);                                      // 006BF0EB..006BF104
                copy12(e + 0x78, kotor_m15d_color_007a23e4);                                   // 006BF107..006BF124
                gui_load_border_params_set_pulsing_00414d00(e + 0x68, nullptr, 1, 1, 0);       // 006BF114..006BF127
                sw(self, 0x68, lw(self, 0x68) + 1u);                                           // 006BF12C
                sw(e, 0x3BC, lw(e, 0x3BC) & 0xFFFFFFFEu);                                      // 006BF12F
            }
        } else if (kotor_fastcall_00563710(table, nullptr, npc) != 0) {                        // 006BF138..006BF148
            sw(e, 0x138, 1); sw(e, 0, 0x3F800000u);                                            // 006BF14A..006BF150
            copy12(e + 4, kotor_m15d_color_007a23e4);                                          // 006BF156..006BF16F
            copy12(e + 0x78, kotor_m15d_color_007a23e4);                                       // 006BF172..006BF18F
            gui_load_border_params_set_pulsing_00414d00(e + 0x68, nullptr, 1, 1, 0);           // 006BF17F..006BF192
        }
        if (static_cast<U8>(recovered_party_npc_selectable_005637c0(table, nullptr, static_cast<U32>(npc))) != 0) { // 006BF197..006BF1A7
            sw(e, 0x2F8, 0x3F800000u);                                                         // 006BF1A9 (1.0)
        } else {
            const U32 f = lw(e, 0x3BC) & 0xFFFFFFFEu;                                          // 006BF1B5..006BF1BB
            sw(e, 0x2F8, 0x3E800000u);                                                         // 006BF1BE (0.25)
            sw(e, 0x3BC, f);                                                                   // 006BF1C8
        }
        sw(e, 0x2C0, lw(e, 0x2C0) | 2u);                                                       // 006BF1CE..006BF1E4
        gui_load_border_params_set_fill_00414c00(e + 0x2EC, nullptr, portrait, 0);             // 006BF1D4..006BF1EA
        sw(e, 0x180, lw(e, 0x180) & 0xFFFFFFFDu);                                              // 006BF1EF..006BF200
    }
    kotor_fastcall_006be4e0(self, nullptr);                                                    // 006BF246..006BF248 UpdateCount
    w2x_party_unmount_00563950(table, nullptr);                                                // 006BF24D..006BF251 RemoveGameInProgress
    vcall<void, void*, U32>(self, 0x08, at(self, 0x38B8), 0);                                  // 006BF256..006BF263 [vtbl+8](BTN_ACCEPT, 0)
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 006BF266..006BF268
    show_tutorial_window_005edf40(client_app(), nullptr, 0x29, 0x7F000000u, 0x7F000000u, 0);   // 006BF26D..006BF284
}

// 006BF3B0 CSWGuiPartySelection::OnDone(CSWGuiControl* control) RET 4.
void __fastcall kotor_fastcall_006bf3b0(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                        // 006BF3B0..006BF3BD
    if (lw(control, 0x48) == 0x2D) play_sound(self, 0);                                        // 006BF3C3..006BF3CE
    if (lw(self, 0x70) == 0) { kotor_fastcall_006be560(self, nullptr); return; }               // 006BF3D3..006BF3D8, 006BF59F
    U8 able = 0, forced = 0, selected = 0;                                                     // bl, cl, [esp+0Ch] (argument slot)
    for (U32 i = 0; i < 9; ++i) {                                                              // 006BF3DE..006BF504 (unrolled)
        const U32 flags = lw(self, 0x4C4 + i * 0x454u);
        if ((flags & 1u) != 0) ++able;
        if ((flags & 4u) != 0) ++forced;
        if (lw(self, 0x240 + i * 0x454u) != 0) ++selected;
    }
    if (able == 0 || forced == 2) {                                                            // 006BF508..006BF50F
        kotor_fastcall_006be560(self, nullptr);                                                // 006BF57E..006BF580 AcceptParty
        sw(ingame_gui(), 0x30, 0);                                                             // 006BF585..006BF594
        return;
    }
    if (able == 0) return;                                                                     // 006BF511..006BF513 (jbe)
    void* box = lp(ingame_gui(), 0x98);                                                        // 006BF519..006BF527
    kotor_fastcall_00627130(box, nullptr, 1);                                                  // 006BF52D..006BF531 SetAllowCancel(1)
    if (able > 1 && selected < 2) vcall<void, U32>(box, 0x6C, 0x95B9);                         // 006BF536..006BF54B
    else vcall<void, U32>(box, 0x6C, 0x95B8);                                                  // 006BF550..006BF559
    gui_load_messagebox_set_callback_00624a40(box, nullptr, self, ad(reinterpret_cast<const void*>(&kotor_fastcall_006bec90)), 0); // 006BF55C..006BF566
    kotor_fastcall_0040bc70(manager_of(self), nullptr, box, 1, 1);                             // 006BF56B..006BF573 AddPanel
}
}  // extern "C"
