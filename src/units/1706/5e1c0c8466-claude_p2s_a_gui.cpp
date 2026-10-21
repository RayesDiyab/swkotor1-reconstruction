// Lane p2s-a: server player-to-server message handlers (CSWSMessage::HandlePlayerToServer*) routed by the 0918
// player-message dispatcher (player_message_native.cpp handler()), plus the callees that have no implementation
// in the candidate (c16 map). Transcribed from the original swkotor.exe (read-only); addresses in comments are
// the original instructions.
// Handler ABI (every handler): original BOOL __thiscall CSWSMessage::HandlePlayerToServer<X>(CSWSPlayer* player,
// BYTE minor), RET 8, EAX 1 handled / 0 overflow-underflow. Exported as the dispatcher calls handlers:
//   extern "C" unsigned __fastcall recovered_player_to_server_<name>_<addr>(void* message, void*, void* player, unsigned minor)
// (@16). The dispatcher stores only the low byte of minor; every handler reads only that byte.
// Conventions as W5: thiscall -> extern "C" __fastcall(self, unused_edx, ...); live_operator_new / live_free only.
#include "58f963adf7-w5_types.hpp"
#include "294ff121b7-w5_server_env.hpp"
#include <initializer_list>

using namespace w5;

namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction { void* __fastcall remove_head_005e9c80(kotor_list::Internal* list, void*); } // [0091]

extern "C" {
// ---------------- imports: existing providers in the c16 candidate (exact names) ----------------
unsigned char __fastcall kotor_fastcall_004d6720(void* message, void*, unsigned bits);   // CSWMessage::ReadBYTE (w2_message_native)
unsigned __fastcall kotor_fastcall_004d6230(void* message, void*, unsigned);              // MessageReadOverflow (-> 0917 overrun)
unsigned __fastcall kotor_fastcall_004d6240(void* message, void*, unsigned);              // MessageReadUnderflow (-> 0917 underrun)

// ---------------- exports ----------------
void __fastcall kotor_fastcall_005ca030(void* luo, void*);
void __fastcall kotor_fastcall_005ca470(void* luo, void*);
void __fastcall kotor_fastcall_00560c90(void* gui, void*, U open);
U __fastcall recovered_player_to_server_gui_inventory_00523240(void* message, void*, void* player, U minor);
void __fastcall kotor_fastcall_005caeb0(void* update, void*);
void* __fastcall kotor_fastcall_005caf10(void* update, void*);
void __fastcall kotor_fastcall_005cb040(void* update, void*);
void __fastcall kotor_fastcall_00560ba0(void* sheet, void*, U id);
U __fastcall kotor_fastcall_00561190(void* player, void*, U id);
U __fastcall kotor_fastcall_0056b610(void* message, void*, U player_id, U id);
U __fastcall recovered_player_to_server_gui_container_005232a0(void* message, void*, void* player, U minor);
U __fastcall recovered_player_to_server_character_sheet_00523380(void* message, void*, void* player, U minor);

// more imports (c16 candidate)
unsigned __fastcall kotor_fastcall_00578540(void* message, void*);                        // CSWSMessage::ReadOBJECTIDServer
unsigned __fastcall kotor_fastcall_004d66f0(void* message, void*);                        // CSWMessage::ReadBOOL
void* __fastcall recovered_object_array_004aed70(void* server, void*);                    // CServerExoApp::GetObjectArray
void* __fastcall kotor_fastcall_00561080(void* player, void*);                            // CSWSPlayer::GetGameObject
void __fastcall kotor_fastcall_00587560(void* placeable, void*, U closer, U take);        // CSWSPlaceable::CloseInventory
void __fastcall kotor_fastcall_0055d800(void* item, void*, U id, U take);                 // CSWSItem::CloseInventory
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U a, U b);          // CreateWriteMessage
void __fastcall kotor_fastcall_00578570(void* message, void*, U id);                      // WriteOBJECTIDServer
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);         // GetWriteMessage
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage
void* __fastcall kotor_m15_combat_information_construct_00552280(void* self, void*);      // CCombatInformation::CCombatInformation [0811]
void __fastcall kotor_m15b_combat_information_destruct_00550e00(void* self, void*);       // ~CCombatInformation [0821]
extern void* kotor_cell_rules_007a3a28;                                                   // [007A3A28] CSWRules*

// ---------------------------------------------------------------------------------------------------------
// 005CA030 CSWSPlayerLUOInventory::ClearSlots(): 18 slot ids := [00746644] (7F000000 OBJECT_INVALID),
// 18 slot words at +48 := -1. RET 0.
void __fastcall kotor_fastcall_005ca030(void* luo, void*) {
    for (U i = 0; i < 0x12; ++i) {                                                   // 005CA030..51
        at<U>(luo, i * 4) = 0x7f000000u;                                             // 005CA040..46 ([00746644])
        at<U>(luo, 0x48 + i * 4) = 0xffffffffu;                                      // 005CA049
    }
}

// 005CA470 CSWSPlayerLUOInventory::ClearRepository(): pop and free every node of the list at +90. RET 0.
void __fastcall kotor_fastcall_005ca470(void* luo, void*) {
    while (*at<void**>(luo, 0x90) != nullptr) {                                      // 005CA473..7C / 005CA48B..98 (head)
        void* data = base_ini_reconstruction::remove_head_005e9c80(
            at<kotor_list::Internal*>(luo, 0x90), nullptr);                         // 005CA480
        live_free(data);                                                             // 005CA485..86 (_free)
    }
}

// 00560C90 CSWSPlayerInventoryGUI::SetOpen(BOOL open): closing an open GUI clears the LUO inventory (+0) slots
// and repository; +4 := open. RET 4.
void __fastcall kotor_fastcall_00560c90(void* gui, void*, U open) {
    if (open == 0 && at<U>(gui, 4) != 0) {                                           // 00560C96..A1
        kotor_fastcall_005ca030(at<void*>(gui, 0), nullptr);                         // 00560CA3..A5
        kotor_fastcall_005ca470(at<void*>(gui, 0), nullptr);                         // 00560CAA..AC
    }
    at<U>(gui, 4) = open;                                                            // 00560CB1
}

// ---------------------------------------------------------------------------------------------------------
// 00523240 CSWSMessage::HandlePlayerToServerGuiInventoryMessage(CSWSPlayer* player, BYTE minor). RET 8.
// Reads BYTE open (8 bits) before the minor test; minor 1 = set the player's inventory GUI (+54) open state.
U __fastcall recovered_player_to_server_gui_inventory_00523240(void* message, void*, void* player, U minor) {
    const unsigned char open = kotor_fastcall_004d6720(message, nullptr, 8);         // 00523242..4F
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                 // 0052324B..58 -> 0052328C
    if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                 // 0052325A..65 -> 0052328C
    if (static_cast<unsigned char>(minor) == 1)                                      // 00523267..6C
        kotor_fastcall_00560c90(at<void*>(player, 0x54), nullptr, open != 0 ? 1u : 0u); // 0052326E..7D
    return 1;                                                                        // 00523282..89
}

// ---------------------------------------------------------------------------------------------------------
// 005232A0 CSWSMessage::HandlePlayerToServerGuiContainerMessage(CSWSPlayer* player, BYTE minor). RET 8.
// Only minor 2 (close container) reads anything; every other minor returns 1 without reading. The object lookup's
// out slot is the minor argument slot ([esp+14]).
U __fastcall recovered_player_to_server_gui_container_005232a0(void* message, void*, void* player, U minor) {
    if (static_cast<unsigned char>(minor) != 2) return 1;                            // 005232A0..AA -> 00523331
    const U id = kotor_fastcall_00578540(message, nullptr);                          // 005232B0..B7 (ebx)
    const U take = kotor_fastcall_004d66f0(message, nullptr);                        // 005232B5..C2 (edi)
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                 // 005232BE..CB -> 0052336D
    if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                 // 005232D1..DC
    void* object = nullptr;                                                          // [esp+14]
    if (static_cast<unsigned char>(kotor_fastcall_004d8230(recovered_object_array_004aed70(server_app(), nullptr),
                                                           nullptr, id, &object)) != 0) // 005232E2..FC ([0074666C] = 0)
        return 1;                                                                    // 00523302
    const unsigned char type = at<unsigned char>(object, 8);                         // 00523304..08
    if (type == 9) {                                                                 // 0052330B ([00746638] = 9 placeable)
        const U closer = at<U>(kotor_fastcall_00561080(player, nullptr), 4);         // 00523313..1C
        kotor_fastcall_00587560(vcall<void*>(object, 0x48), nullptr, closer, take);  // 0052331F..2C (AsSWSPlaceable)
    } else if (type == 6) {                                                          // 0052333C ([00746635] = 6 item)
        const U closer = at<U>(kotor_fastcall_00561080(player, nullptr), 4);         // 00523344..4D
        kotor_fastcall_0055d800(vcall<void*>(object, 0x38), nullptr, closer, take);  // 00523350..5D (AsSWSItem)
    }
    return 1;                                                                        // 00523331 / 00523362
}

// ---------------------------------------------------------------------------------------------------------
// 005CAEB0 CSWCreatureStatsUpdate::ClearEffectIcons(): from the last element down, free +20[i], zero it, drop the
// count (+24) and shift the tail down by one. RET 0.
void __fastcall kotor_fastcall_005caeb0(void* update, void*) {
    for (I i = at<I>(update, 0x24) - 1; i >= 0; --i) {                               // 005CAEB4..B8 / 005CAF03..04 (js)
        void* icon = at<void**>(update, 0x20)[i];                                    // 005CAEC0..C3
        if (icon != nullptr) {                                                       // 005CAEC6..C8
            live_free(icon);                                                         // 005CAECA..CB (_free)
            at<void**>(update, 0x20)[i] = nullptr;                                   // 005CAED0..D6
        }
        const I count = at<I>(update, 0x24) - 1;                                     // 005CAEDD..E0
        at<I>(update, 0x24) = count;                                                 // 005CAEE7
        for (I k = i; k < at<I>(update, 0x24); ++k)                                  // 005CAEE3..01 (count re-read)
            at<U*>(update, 0x20)[k] = at<U*>(update, 0x20)[k + 1];                   // 005CAEF0..F9
    }
}

// 005CAF10 CSWCreatureStatsUpdate::CSWCreatureStatsUpdate(): 0x5C bytes; +54 = new CCombatInformation (0x48),
// +58 = rules+AB bytes of 0xEC when that count is nonzero. RET 0, EAX this.
void* __fastcall kotor_fastcall_005caf10(void* s, void*) {
    at<U>(s, 4) = 0; at<U>(s, 8) = 0; at<U>(s, 0) = 0;                               // 005CAF32..38
    at<U>(s, 0x10) = 0; at<U>(s, 0x14) = 0; at<U>(s, 0xc) = 0;                       // 005CAF3E..44
    at<U>(s, 0x24) = 0; at<U>(s, 0x28) = 0; at<U>(s, 0x20) = 0;                      // 005CAF47..4D
    at<U>(s, 0x48) = 0xffffffecu;                                                    // 005CAF5E
    at<U16>(s, 0x42) = 0xffec; at<U16>(s, 0x44) = 0xffec;                            // 005CAF61..65
    for (U o : {0x32u, 0x33u, 0x34u, 0x35u, 0x36u, 0x37u, 0x2cu, 0x2du, 0x2eu, 0x2fu, 0x30u, 0x31u, 0x3eu})
        at<U8>(s, o) = 0xec;                                                         // 005CAF69..8D
    at<U16>(s, 0x52) = 0xffec;                                                       // 005CAF90
    for (U o : {0x38u, 0x39u, 0x3au, 0x3bu, 0x3cu, 0x3du, 0x3fu, 0x40u, 0x41u})
        at<U8>(s, o) = 0xec;                                                         // 005CAF94..A9
    at<U>(s, 0x58) = 0; at<U>(s, 0x4c) = 0;                                          // 005CAFAF..B2
    void* memory = live_operator_new(0x48);                                          // 005CAF57 / 005CAFB5
    at<void*>(s, 0x54) = memory != nullptr ? kotor_m15_combat_information_construct_00552280(memory, nullptr) : nullptr; // 005CAFC1..D5
    const U8 count = at<U8>(kotor_cell_rules_007a3a28, 0xab);                        // 005CAFD8..DD
    if (count != 0) {                                                                // 005CAFE3..EA
        void* bytes = live_operator_new(count);                                      // 005CAFEC..F1
        at<void*>(s, 0x58) = bytes;                                                  // 005CAFF8
        std::memset(bytes, 0xec, at<U8>(kotor_cell_rules_007a3a28, 0xab));           // 005CAFFB..1C (count re-read)
    }
    return s;                                                                        // 005CB023
}

// 005CB040 CSWCreatureStatsUpdate::~CSWCreatureStatsUpdate(). RET 0. (+58 is freed but not zeroed.)
void __fastcall kotor_fastcall_005cb040(void* s, void*) {
    at<U>(s, 4) = 0; at<U>(s, 0x10) = 0;                                             // 005CB06E..71
    if (at<void*>(s, 0x58) != nullptr) live_free(at<void*>(s, 0x58));                // 005CB05F..7C
    void* combat = at<void*>(s, 0x54);                                               // 005CB07F
    if (combat != nullptr) {                                                         // 005CB082..84
        kotor_m15b_combat_information_destruct_00550e00(combat, nullptr);            // 005CB086..88
        live_free(combat);                                                           // 005CB08D..8E
        at<void*>(s, 0x54) = nullptr;                                                // 005CB096
    }
    kotor_fastcall_005caeb0(s, nullptr);                                             // 005CB099..9B
    live_free(at<void*>(s, 0x20)); at<void*>(s, 0x20) = nullptr;                     // 005CB0A0..AE (CExoArrayList dtor)
    live_free(at<void*>(s, 0xc)); at<void*>(s, 0xc) = nullptr;                       // 005CB0B1..BE
    live_free(at<void*>(s, 0)); at<void*>(s, 0) = nullptr;                           // 005CB0C1..D8
}

// 00560BA0 CSWSPlayerCharSheetGUI::SetCreatureDisplayed(id): on a change of id with a stats update present (+8),
// replace it with a fresh one; +0 := id. RET 4.
void __fastcall kotor_fastcall_00560ba0(void* sheet, void*, U id) {
    if (at<U>(sheet, 0) != id) {                                                     // 00560BBD..C0
        void* update = at<void*>(sheet, 8);                                          // 00560BC2
        if (update != nullptr) {                                                     // 00560BC5..C7
            kotor_fastcall_005cb040(update, nullptr);                                // 00560BC9..CB
            live_free(update);                                                       // 00560BD0..D1
            void* memory = live_operator_new(0x5c);                                  // 00560BD6..D8
            at<void*>(sheet, 8) = memory != nullptr ? kotor_fastcall_005caf10(memory, nullptr) : nullptr; // 00560BE4..FB
        }
    }
    at<U>(sheet, 0) = id;                                                            // 00560C02
}

// 00561190 CSWSPlayer::PermittedToDisplayCharacterSheet(id): both the requested creature and the player's
// creature (+28) exist and are the same object. RET 4, EAX 0/1.
U __fastcall kotor_fastcall_00561190(void* player, void*, U id) {
    if (server_app() == nullptr) return 0;                                           // 00561190..A2
    void* requested = kotor_fastcall_004ae770(server_app(), nullptr, id);            // 005611A5..B0
    if (requested == nullptr) return 0;                                              // 005611B2..B4
    void* own = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(player, 0x28)); // 005611BD..C9
    if (own == nullptr) return 0;                                                    // 005611CE..D0
    return at<U>(own, 4) == at<U>(requested, 4) ? 1u : 0u;                           // 005611D2..DD
}

// 0056B610 CSWSMessage::SendServerToPlayerGUICharacterSheet_NotPermitted(player id, object id). RET 8,
// EAX = SendServerToPlayerMessage result (0 when GetWriteMessage fails).
U __fastcall kotor_fastcall_0056b610(void* message, void*, U player_id, U id) {
    kotor_fastcall_004d68a0(message, nullptr, 0x80, 0xffffffffu, 1);                 // 0056B614..1F
    kotor_fastcall_00578570(message, nullptr, id);                                   // 0056B624..2B
    void* data = nullptr; U size = 0;                                                // [esp+8] / [esp+4]
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;      // 0056B630..49
    return kotor_fastcall_00567e60(message, nullptr, player_id, 0x15, 2, data, size); // 0056B64C..61
}

// 00523380 CSWSMessage::HandlePlayerToServerCharacterSheetMessage(CSWSPlayer* player, BYTE minor). RET 8.
// Reads BYTE mode and OBJECTID before the minor test. minor 1: a refused id is replaced by OBJECT_INVALID and mode
// 0xFF (after the NotPermitted reply); the viewer counts (+348) of the old and new displayed creature move by one.
U __fastcall recovered_player_to_server_character_sheet_00523380(void* message, void*, void* player, U minor) {
    unsigned char mode = kotor_fastcall_004d6720(message, nullptr, 8);               // 00523383..8E (bl)
    U id = kotor_fastcall_00578540(message, nullptr);                                // 0052338C..99 (edi)
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                 // 00523395..A2 -> 00523446
    if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                 // 005233A8..B3
    if (static_cast<unsigned char>(minor) != 1) return 1;                            // 005233B9..BE -> 0052343B
    if (kotor_fastcall_00561190(player, nullptr, id) == 0) {                         // 005233C1..CF
        kotor_fastcall_0056b610(message, nullptr, at<U>(player, 4), id);             // 005233D1..DB
        mode = 0xff;                                                                 // 005233D8
        id = 0x7f000000u;                                                            // 005233E0
    }
    void* old_shown = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(at<void*>(player, 0x50), 0)); // 005233E5..F4
    if (old_shown != nullptr) --at<I>(old_shown, 0x348);                             // 005233F9..FD
    void* new_shown = kotor_fastcall_004ae770(server_app(), nullptr, id);            // 00523403..0C
    if (new_shown != nullptr) ++at<I>(new_shown, 0x348);                             // 00523411..15
    kotor_fastcall_00560ba0(at<void*>(player, 0x50), nullptr, id);                   // 0052341B..1F
    at<unsigned char>(at<void*>(player, 0x50), 4) = mode;                            // 00523427..2A
    if (mode == 0xff) kotor_fastcall_005caeb0(at<void*>(at<void*>(player, 0x50), 8), nullptr); // 00523424..35
    return 1;                                                                        // 0052343A..43
}
}  // extern "C"
