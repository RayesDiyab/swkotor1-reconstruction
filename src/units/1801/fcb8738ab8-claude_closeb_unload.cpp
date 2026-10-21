// claude_closeB_unload.cpp (lane worker closeB): CServerExoAppInternal::UnloadModule 004B9240 behind the 0886
// NativeLoadRequest::unload_004b9240 gate (LoadModule 004B95B0 -> UnloadModule when a module is already loaded, i.e.
// the Endar Spire -> Taris transition and every later module change), with its missing callee cascade, transcribed
// from the original swkotor.exe (read-only):
//   004B9240 CServerExoAppInternal::UnloadModule()                         RET 0, EAX 1  closeB_server_unload_module_004b9240
//   005EE2D0 CClientExoApp::GetSoundPausedByCombat()                       RET 0        kotor_fastcall_005ee2d0
//   0056CCB0 CSWSMessage::SendServerToPlayerModule_DumpPlayer(CSWSPlayer*) RET 4        kotor_fastcall_0056ccb0
//   00562E40 CSWSPlayer::ClearPlayerOnDestroyGame()                        RET 0        kotor_fastcall_00562e40
//   005CA4A0 CSWSPlayerLUOInventory::ClearContainer()                      RET 0        kotor_fastcall_005ca4a0
//   005CA900 CSWSPlayerLUOInventory::~CSWSPlayerLUOInventory()             RET 0        kotor_fastcall_005ca900
//   005CA7F0 CSWSPlayerLUOInventory::CSWSPlayerLUOInventory()              RET 0        kotor_fastcall_005ca7f0
//   005CA400 CExoLinkedList<CSWSPlayerLUOInventoryItem>::CExoLinkedList()   RET 0        kotor_fastcall_005ca400
//   005611F0 CExoLinkedList<CLastUpdateObject>::CExoLinkedList()           RET 0        kotor_fastcall_005611f0
//   00560B30 CSWSPlayerCharSheetGUI::CSWSPlayerCharSheetGUI()              RET 0        kotor_fastcall_00560b30
//   00560C20 CSWSPlayerInventoryGUI::CSWSPlayerInventoryGUI()              RET 0        kotor_fastcall_00560c20
//   00563600 CSWPartyTable::ClearInventory()                               RET 0        kotor_fastcall_00563600
//   0041DCF0 CTlkTable::ClearCustomTokens()                                RET 0        kotor_fastcall_0041dcf0
//   0052B950 CFactionManager::~CFactionManager()                           RET 0        kotor_fastcall_0052b950
//   0052B8E0 CFactionManager::CFactionManager()                            RET 0        kotor_fastcall_0052b8e0
//   005BE8A0 CSWSFaction::~CSWSFaction()                                   RET 0        kotor_fastcall_005be8a0
//   004B11C0 CServerAIMaster::ClearEventQueue()                            RET 0        kotor_fastcall_004b11c0
// UnloadModule is exported under a lane-unique name: the census-listed trap @kotor_fastcall_004b9240@8 in
// 0783-populate-removed.obj (lane c, imported by 0457-successor.obj) still defines the canonical name; once that trap is
// removed, `/alternatename:@kotor_fastcall_004b9240@8=@closeB_server_unload_module_004b9240@8` (or a rename) binds it.
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, stack args...); game heap only
// (live_operator_new / live_free). EH: the original unwind states only guard operator new results while a
// constructor runs (no bound callee throws), so they are not reproduced; quirks (unchecked allocation results that
// the original dereferences or stores) are kept.
#include <cstdint>
#include <cstring>
#include <initializer_list>

namespace {
using U = std::uint32_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(a); }
inline U word(const void* p) { return reinterpret_cast<U>(p); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
inline void wr8(U a, U v) { *static_cast<unsigned char*>(ptr(a)) = static_cast<unsigned char>(v); }
template <class R, class... A> R vcall(U object, U slot, A... a) {          // thiscall virtual through slot offset
    using F = R(__thiscall*)(void*, A...);
    return reinterpret_cast<F>(rd(rd(object) + slot))(ptr(object), a...);
}
constexpr U k_invalid_00746644 = 0x7f000000u;                              // [00746644] OBJECT_INVALID
constexpr U k_sound_mode_0074c5ec = 0;                                     // [0074C5EC] = 0
}

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }   // [007A39FC] CAppManager*
namespace frame_control { extern unsigned resource_manager_007a39e8; }        // [007A39E8] CExoResMan*
namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction {
void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*);           // 005E9C80 CExoLinkedListInternal::RemoveHead [0091]
void __fastcall destroy_list_005e9ab0(kotor_list::Internal*, void*);           // 005E9AB0 ~CExoLinkedList<uint> [0091]
}
namespace recovered_server_create { void* construct_list_internal_005e9aa0(void*); } // 005E9AA0 CExoLinkedListInternal() [0842]
namespace menu_tables_m41 { void* __fastcall ClientListConstruct_004b25d0(void*, void*); } // 004B25D0 CExoLinkedList<CSWSClient>() [d1_bodies]
namespace kotor_menu_10_19 { struct ObjectArray { ObjectArray* __thiscall construct_004d7d80(int); }; } // 004D7D80 CGameObjectArray(int) [0019]

extern "C" {
// ---------------- existing providers (c35 map, exact decorated arity) ----------------
void* __cdecl live_operator_new(unsigned);                                                  // 006FA7E6
void __cdecl live_free(void*);                                                              // 006FA390
extern void* kotor_cell_sound_007a39ec;                                                     // [007A39EC] CExoSound* [0673]
extern void* kotor_cell_tlk_007a3a08;                                                       // [007A3A08] CTlkTable* [0384]
void* __fastcall server_internal_get_module_004b14f0(void* internal, void*);                // 004B14F0 GetModule
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                // 004AEDB0 GetSWSMessage
void __fastcall frame_R4_005d5e80(void* sound, void*, U mode);                              // 005D5E80 SetSoundMode [0709]
void __fastcall kotor_fastcall_004b8110(void* internal, void*, U type, U state);           // 004B8110 SetPauseState RET 8 [0453]
U __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* pos);                 // 005E9B60 GetAtPos
U __fastcall unresolved_list_next_005e9b90(void* list, void*, void** pos);                  // 005E9B90 GetNext
void* __fastcall kotor_fastcall_00401060(void* app, void*, U which);                        // 00401060 GetObjectTableManager RET 4 [0718]
U __fastcall kotor_fastcall_004ae650(void* table, void*, U id);                             // 004AE650 CObjectTableManager::RemovePlayer [w3_traps]
void* __fastcall kotor_fastcall_00561080(void* player, void*);                              // 00561080 CSWSPlayer::GetGameObject [0595]
void* __fastcall recovered_object_area_004cb120(void* object, void*);                       // 004CB120 CSWSObject::GetArea [1060]
void __fastcall kotor_fastcall_004f0fd0(void* creature, void*, U flag);                     // 004F0FD0 CSWSCreature::RemoveFromArea [route_bodies]
void __fastcall recovered_set_game_object_00561c40(void* player, void*, void* object);      // 00561C40 CSWSPlayer::SetGameObject [w2_entry]
U __fastcall recovered_object_control_0056f430(void* message, void*, U player, U object);   // 0056F430 GameObjUpdate_ObjControl [w2_update]
void* __fastcall kotor_fastcall_005ed8b0(void* client, void*);                              // 005ED8B0 CClientExoApp::GetSWParty [0707]
void __fastcall kotor_fastcall_00634a40(void* party, void*);                                // 00634A40 CSWParty::ClearParty [vmC_party]
void __fastcall recovered_clear_items_0055f330(void* repository, void*);                    // 0055F330 CItemRepository::ClearItems [w2_party]
void __fastcall kotor_fastcall_004d7e10(void* array, void*);                                // 004D7E10 ~CGameObjectArray [0577]
void __fastcall recovered_ai_event_cleanup_004b0ab0(void* ai, void*, U type, U data);       // 004B0AB0 CServerAIMaster::DeleteEventData (0886)
void* __fastcall frame_LogStringCtor_005e5a90(void* string, void*, const char* text);      // 005E5A90 CExoString(char*) [0459]
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);                         // 005E5C20 ~CExoString [0459]
U __fastcall gui_load_resman_nuke_directory_00409480(void* resman, void*, void* path, U a, U b); // 00409480 CExoResMan::NukeDirectory RET 0Ch [0395]
int __fastcall net_layer_send_message_to_player_005d4cb0(void* net, void*, U player, const char* data, U size, U flags); // 005D4CB0 [0599]
void __fastcall recovered_last_update_destroy_005ca6a0(void* object, void*);                // 005CA6A0 ~CLastUpdateObject [w2_g07]
void __fastcall kotor_fastcall_005cb040(void* update, void*);                               // 005CB040 ~CSWCreatureStatsUpdate [p2s_a_gui]
void* __fastcall kotor_fastcall_005caf10(void* update, void*);                              // 005CAF10 CSWCreatureStatsUpdate() [p2s_a_gui]
void __fastcall tlk_token_destroy_0059ea20(void* token, void*);                             // 0059EA20 ~CSWSPlayerJournalQuestUpdates [0583]
void __stdcall c2da_destroy_array_006fa852(void* first, U size, int count, void* destroy);   // 006FA852 eh_vector_destructor_iterator [0186]

// ---------------- bodies defined here ----------------
U __fastcall closeB_server_unload_module_004b9240(void* internal, void*);
U __fastcall kotor_fastcall_005ee2d0(void* client, void*);
U __fastcall kotor_fastcall_0056ccb0(void* message, void*, void* player);
void __fastcall kotor_fastcall_00562e40(void* player, void*);
void __fastcall kotor_fastcall_005ca4a0(void* inventory, void*);
void __fastcall kotor_fastcall_005ca900(void* inventory, void*);
void* __fastcall kotor_fastcall_005ca7f0(void* inventory, void*);
void* __fastcall kotor_fastcall_005ca400(void* list, void*);
void* __fastcall kotor_fastcall_005611f0(void* list, void*);
void* __fastcall kotor_fastcall_00560b30(void* gui, void*);
void* __fastcall kotor_fastcall_00560c20(void* gui, void*);
void __fastcall kotor_fastcall_00563600(void* party, void*);
void __fastcall kotor_fastcall_0041dcf0(void* tlk, void*);
void __fastcall kotor_fastcall_0052b950(void* factions, void*);
void* __fastcall kotor_fastcall_0052b8e0(void* factions, void*);
void __fastcall kotor_fastcall_005be8a0(void* faction, void*);
void __fastcall kotor_fastcall_004b11c0(void* ai, void*);
}

namespace {
U app() { return word(const_cast<void*>(kotor_menu_window_cells::manager_007a39fc)); }
// CExoLinkedList<T>::~CExoLinkedList inline pattern: destroy the internal list, free it.
void destroy_list_member(U internal) {
    if (internal != 0) {
        base_ini_reconstruction::destroy_list_005e9ab0(static_cast<kotor_list::Internal*>(ptr(internal)), nullptr);
        live_free(ptr(internal));
    }
}
// RemoveHead/free until empty (inline loop of the LUO inventory list members)
void drain_list(U list_member) {
    if (rd(rd(list_member)) != 0) {
        do {
            live_free(base_ini_reconstruction::remove_head_005e9c80(static_cast<kotor_list::Internal*>(ptr(rd(list_member))), nullptr));
        } while (rd(rd(list_member)) != 0);
    }
}
}

// ===================================================================================== 005EE2D0
U __fastcall kotor_fastcall_005ee2d0(void* client, void*) {
    return rd(rd(word(client) + 4) + 0x4c4);                                         // 005EE2D0..005EE2D9
}

// ===================================================================================== 0056CCB0
// SendServerToPlayerModule_DumpPlayer: 3-byte message 'P' 03 08 to the player's id via the server net layer.
U __fastcall kotor_fastcall_0056ccb0(void* message, void*, void* player) {
    const U id = rd(word(player) + 4);                                               // 0056CCB0..0056CCB7
    unsigned char* const data = static_cast<unsigned char*>(live_operator_new(3));   // 0056CCBA..0056CCC5
    wr(word(message) + 0x4c, 0xffffffffu);                                           // 0056CCCA
    if (data == nullptr) return 0;                                                   // 0056CCC8..0056CCD6 (EAX = NULL result)
    data[0] = 0x50; data[1] = 3; data[2] = 8;                                        // 0056CCDB..0056CCE2
    const U server = rd(app() + 8);                                                  // 0056CCE6..0056CCEC
    void* const net = vcall<void*>(server, 0x10);                                    // 0056CCEF..0056CCF5 CServerExoApp::GetNetLayer
    const int sent = net_layer_send_message_to_player_005d4cb0(net, nullptr, id,
        reinterpret_cast<const char*>(data), 3, 0);                                  // 0056CCD9..0056CCFA
    live_free(data);                                                                 // 0056CCFF..0056CD07
    return static_cast<U>(sent);                                                     // 0056CD0A
}

// ===================================================================================== 005CA400 / 005611F0
// CExoLinkedList<T>(): allocate a 0Ch CExoLinkedListInternal; NULL internal when operator new fails.
void* __fastcall kotor_fastcall_005ca400(void* list, void*) {
    void* const memory = live_operator_new(0xc);
    wr(word(list), memory != nullptr ? word(recovered_server_create::construct_list_internal_005e9aa0(memory)) : 0);
    return list;
}
void* __fastcall kotor_fastcall_005611f0(void* list, void*) {
    void* const memory = live_operator_new(0xc);                                     // 00561207..00561210
    wr(word(list), memory != nullptr ? word(recovered_server_create::construct_list_internal_005e9aa0(memory)) : 0); // 00561217..00561244
    return list;                                                                     // 0056122C / 00561246
}

// ===================================================================================== 005CA4A0
void __fastcall kotor_fastcall_005ca4a0(void* inventory, void*) {
    drain_list(word(inventory) + 0x98);                                              // 005CA4A3..005CA4C8
}

// ===================================================================================== 005CA7F0
void* __fastcall kotor_fastcall_005ca7f0(void* inventory, void*) {
    const U s = word(inventory);
    kotor_fastcall_005ca400(ptr(s + 0x90), nullptr);                                 // 005CA80C..005CA818
    kotor_fastcall_005ca400(ptr(s + 0x98), nullptr);                                 // 005CA81D..005CA82D
    kotor_fastcall_005ca400(ptr(s + 0x9c), nullptr);                                 // 005CA832..005CA83F
    for (U i = 0; i < 0x12; ++i) {                                                   // 005CA844..005CA861
        wr(s + i * 4, k_invalid_00746644);
        wr(s + 0x48 + i * 4, 0xffffffffu);
    }
    drain_list(s + 0x90);                                                            // 005CA863..005CA886
    drain_list(s + 0x98);                                                            // 005CA888..005CA8A6
    drain_list(s + 0x9c);                                                            // 005CA8A8..005CA8C8
    wr(s + 0x94, k_invalid_00746644);                                                // 005CA8CA..005CA8DD (both arms)
    return inventory;                                                                // 005CA8E8
}

// ===================================================================================== 005CA900
void __fastcall kotor_fastcall_005ca900(void* inventory, void*) {
    const U s = word(inventory);
    for (U i = 0; i < 0x12; ++i) {                                                   // 005CA925..005CA93C
        wr(s + i * 4, k_invalid_00746644);
        wr(s + 0x48 + i * 4, 0xffffffffu);
    }
    drain_list(s + 0x90);                                                            // 005CA93E..005CA96E
    drain_list(s + 0x98);                                                            // 005CA970..005CA99E
    drain_list(s + 0x9c);                                                            // 005CA9A0..005CA9CE
    wr(s + 0x94, k_invalid_00746644);                                                // 005CA9D0..005CA9DF
    destroy_list_member(rd(s + 0x9c));                                               // 005CA9D7..005CA9F9
    destroy_list_member(rd(s + 0x98));                                               // 005CA9FC..005CAA18
    destroy_list_member(rd(s + 0x90));                                               // 005CAA1B..005CAA3B
}

// ===================================================================================== 00560B30 / 00560C20
void* __fastcall kotor_fastcall_00560b30(void* gui, void*) {
    const U s = word(gui);
    wr(s, k_invalid_00746644);                                                       // 00560B4B
    void* const memory = live_operator_new(0x5c);                                    // 00560B49..00560B56
    wr(s + 8, memory != nullptr ? word(kotor_fastcall_005caf10(memory, nullptr)) : 0); // 00560B5D..00560B78
    wr8(s + 4, 0xff);                                                                // 00560B7B
    wr(s + 0xc, 1);                                                                  // 00560B7F
    return gui;
}
void* __fastcall kotor_fastcall_00560c20(void* gui, void*) {
    const U s = word(gui);
    wr(s + 4, 0);                                                                    // 00560C3E
    void* const memory = live_operator_new(0xa0);                                    // 00560C39..00560C4A
    wr(s, memory != nullptr ? word(kotor_fastcall_005ca7f0(memory, nullptr)) : 0);   // 00560C51..00560C7E
    return gui;
}

// ===================================================================================== 00562E40
void __fastcall kotor_fastcall_00562e40(void* player, void*) {
    const U s = word(player);
    const U inventory_gui = rd(s + 0x54);
    if (rd(inventory_gui) != 0) kotor_fastcall_005ca4a0(ptr(rd(inventory_gui)), nullptr); // 00562E5B..00562E67 (+54 dereferenced unchecked)
    if (const U list = rd(s + 0x10)) {                                               // 00562E6C..00562E71
        destroy_list_member(rd(list));                                               // 00562E73..00562E86
        live_free(ptr(list));                                                        // 00562E89..00562E8A
        void* const memory = live_operator_new(4);                                   // 00562E8F..00562E96
        wr(s + 0x10, memory != nullptr ? word(kotor_fastcall_005611f0(memory, nullptr)) : 0); // 00562E9D..00562EB8
    }
    if (rd(s + 0xc) != 0 && rd(rd(rd(s + 0xc))) != 0) {                              // 00562EBB..00562EC6
        do {
            void* const object = base_ini_reconstruction::remove_head_005e9c80(
                static_cast<kotor_list::Internal*>(ptr(rd(rd(s + 0xc)))), nullptr);  // 00562ED0..00562ED5
            if (object != nullptr) {                                                 // 00562EDC..00562EDE
                recovered_last_update_destroy_005ca6a0(object, nullptr);             // 00562EE0..00562EE2
                live_free(object);                                                   // 00562EE7..00562EE8
            }
        } while (rd(rd(rd(s + 0xc))) != 0);                                          // 00562EF0..00562EF7
    }
    if (const U sheet = rd(s + 0x50)) {                                              // 00562EF9..00562EFE
        if (const U update = rd(sheet + 8)) {                                        // 00562F00..00562F05
            kotor_fastcall_005cb040(ptr(update), nullptr);                           // 00562F07..00562F09
            live_free(ptr(update));                                                  // 00562F0E..00562F0F
        }
        live_free(ptr(sheet));                                                       // 00562F17..00562F18
        void* const memory = live_operator_new(0x10);                                // 00562F1D..00562F24
        wr(s + 0x50, memory != nullptr ? word(kotor_fastcall_00560b30(memory, nullptr)) : 0); // 00562F2B..00562F4A
    }
    if (const U ids = rd(s + 0x58)) {                                                // 00562F4D..00562F52
        live_free(ptr(ids));                                                         // 00562F54..00562F55
        const U memory = word(live_operator_new(8));                                 // 00562F5A..00562F61
        if (memory != 0) { wr(memory + 4, 0); wr(memory, k_invalid_00746644); }      // 00562F64..00562F71
        wr(s + 0x58, memory);                                                        // 00562F75..00562F77
    }
    if (const U quests = rd(s + 0x4c)) {                                             // 00562F7A..00562F7F
        const U array = rd(quests + 4);                                              // 00562F81
        if (array != 0) {                                                            // 00562F84..00562F86
            c2da_destroy_array_006fa852(ptr(array), 0xc, static_cast<int>(rd(array - 4)),
                reinterpret_cast<void*>(&tlk_token_destroy_0059ea20));               // 00562F88..00562F97
            live_free(ptr(array - 4));                                               // 00562F9C..00562F9D
        }
        wr(quests + 4, 0);                                                           // 00562FA6
        live_free(ptr(quests));                                                      // 00562FA5..00562FA9
        const U memory = word(live_operator_new(0x14));                              // 00562FAE..00562FB5
        if (memory != 0) {                                                           // 00562FB8..00562FBA
            wr(memory + 8, 0); wr(memory + 0xc, 0); wr(memory + 4, 0);               // 00562FBC..00562FC2
            wr(memory + 0x10, 1); wr(memory, 0);                                     // 00562FC5..00562FCC
        }
        wr(s + 0x4c, memory);                                                        // 00562FD0..00562FDA
    }
    if (const U gui = rd(s + 0x54)) {                                                // 00562FDD..00562FE2
        if (const U inventory = rd(gui)) {                                           // 00562FE4..00562FE8
            kotor_fastcall_005ca900(ptr(inventory), nullptr);                        // 00562FEA..00562FEC
            live_free(ptr(inventory));                                               // 00562FF1..00562FF2
        }
        live_free(ptr(gui));                                                         // 00562FFA..00562FFB
        void* const memory = live_operator_new(8);                                   // 00563000..00563007
        wr(s + 0x54, memory != nullptr ? word(kotor_fastcall_00560c20(memory, nullptr)) : 0); // 0056300A..00563025
    }
}

// ===================================================================================== 00563600
void __fastcall kotor_fastcall_00563600(void* party, void*) {
    const U s = word(party);
    if (rd(s + 0x118) != 0) recovered_clear_items_0055f330(ptr(rd(s + 0x118)), nullptr); // 00563603..0056360D
    wr(s + 0x110, 0);                                                                // 00563612
}

// ===================================================================================== 0041DCF0
void __fastcall kotor_fastcall_0041dcf0(void* tlk, void*) {
    const U s = word(tlk);
    const U tokens = rd(s + 0x48);                                                   // 0041DCF4
    if (tokens != 0) {                                                               // 0041DCF9..0041DCFB
        c2da_destroy_array_006fa852(ptr(tokens), 0xc, static_cast<int>(rd(tokens - 4)),
            reinterpret_cast<void*>(&tlk_token_destroy_0059ea20));                   // 0041DCFD..0041DD0D
        live_free(ptr(tokens - 4));                                                  // 0041DD12..0041DD13
    }
    wr(s + 0x48, 0);                                                                 // 0041DD1C
    wr(s + 0x4c, 0);                                                                 // 0041DD1F
}

// ===================================================================================== 005BE8A0
void __fastcall kotor_fastcall_005be8a0(void* faction, void*) {
    const U s = word(faction);
    frame_LogStringDtor_005e5c20(ptr(s + 0x18), nullptr);                            // 005BE8BD..005BE8C8
    live_free(ptr(rd(s)));                                                           // 005BE8CD..005BE8D8
    wr(s, 0);                                                                        // 005BE8E4
}

// ===================================================================================== 0052B950 / 0052B8E0
void __fastcall kotor_fastcall_0052b950(void* factions, void*) {
    const U s = word(factions);
    if (rd(s + 0x24) != 0) {                                                         // 0052B953..0052B959
        for (int i = 0; i < static_cast<int>(rd(rd(s + 0x24) + 4)); ++i) {           // 0052B95B..0052B98A
            const U faction = rd(rd(rd(s + 0x24)) + static_cast<U>(i) * 4);          // 0052B965..0052B96A
            if (faction != 0) {                                                      // 0052B96D..0052B96F
                kotor_fastcall_005be8a0(ptr(faction), nullptr);                      // 0052B971..0052B973
                live_free(ptr(faction));                                             // 0052B978..0052B979
            }
        }
        const U array = rd(s + 0x24);                                                // 0052B98C
        if (array != 0) {                                                            // 0052B991..0052B992
            live_free(ptr(rd(array)));                                               // 0052B994..0052B997
            wr(array, 0);                                                            // 0052B99D
            live_free(ptr(array));                                                   // 0052B99C..0052B9A3
        }
    }
    const U list = rd(s + 0x20);                                                     // 0052B9AB
    if (list != 0) {                                                                 // 0052B9AE..0052B9B0
        live_free(ptr(rd(list)));                                                    // 0052B9B2..0052B9B5
        wr(list, 0);                                                                 // 0052B9BB
        live_free(ptr(list));                                                        // 0052B9BA..0052B9C1
        wr(s + 0x20, 0);                                                             // 0052B9C9
    }
}
void* __fastcall kotor_fastcall_0052b8e0(void* factions, void*) {
    const U s = word(factions);
    wr(s + 0x18, 0); wr(s + 0x10, 0); wr(s + 0x14, 0); wr(s + 0xc, 0);              // 0052B8FD..0052B906
    wr(s + 8, 0); wr(s + 4, 0); wr(s, 0);                                            // 0052B909..0052B90F
    const U array = word(live_operator_new(0xc));                                    // 0052B8FB..0052B916
    if (array != 0) { wr(array + 4, 0); wr(array + 8, 0); wr(array, 0); }            // 0052B919..0052B923
    wr(s + 0x1c, 0);                                                                 // 0052B92D
    wr(s + 0x20, 0);                                                                 // 0052B930
    wr(s + 0x24, array);                                                             // 0052B933
    return factions;                                                                 // 0052B937
}

// ===================================================================================== 004B11C0
void __fastcall kotor_fastcall_004b11c0(void* ai, void*) {
    const U s = word(ai);
    if (rd(rd(s + 0x54)) != 0) {                                                     // 004B11C3..004B11C9
        U node;
        do {
            node = word(base_ini_reconstruction::remove_head_005e9c80(
                static_cast<kotor_list::Internal*>(ptr(rd(s + 0x54))), nullptr));    // 004B11D0..004B11D5
            recovered_ai_event_cleanup_004b0ab0(ai, nullptr, rd(node + 0x10), rd(node + 0x14)); // 004B11D7..004B11E1
            live_free(ptr(node));                                                    // 004B11E6..004B11E7
        } while (rd(rd(s + 0x54)) != 0);                                             // 004B11EC..004B11F6
    }
}

// ===================================================================================== 004B9240
U __fastcall closeB_server_unload_module_004b9240(void* internal, void*) {
    const U s = word(internal);
    const U objects = rd(s + 0x1005c);                                               // 004B925D..004B9263 [esp+18]
    const U module = word(server_internal_get_module_004b14f0(internal, nullptr));   // 004B9267..004B9275 [esp+14]
    const U message = word(recovered_server_sws_message_004aedb0(ptr(rd(app() + 8)), nullptr)); // 004B926C..004B928B [esp+10]
    const U client = rd(app() + 4);                                                  // 004B927E..004B9284
    if (client != 0 && kotor_fastcall_005ee2d0(ptr(client), nullptr) != 0)          // 004B9289..004B9298
        frame_R4_005d5e80(kotor_cell_sound_007a39ec, nullptr, k_sound_mode_0074c5ec); // 004B929A..004B92A6
    kotor_fastcall_004b8110(internal, nullptr, 2, 0);                                // 004B92AC..004B92B1
    kotor_fastcall_004b8110(internal, nullptr, 1, 0);                                // 004B92B6..004B92BB
    for (const U offset : {0x10070u, 0x10074u}) {                                    // two client lists, same sequence
        if (const U list = rd(s + offset)) {                                         // 004B92C0..004B92C8 / 004B930A..004B9320
            destroy_list_member(rd(list));                                           // 004B92CA..004B92DD / 004B9322..004B9335
            live_free(ptr(list));                                                    // 004B92E0..004B92E1 / 004B9338..004B9339
        }
        void* const memory = live_operator_new(4);                                   // 004B92E9..004B92F0 / 004B9341..004B9348
        wr(s + offset, memory != nullptr ? word(menu_tables_m41::ClientListConstruct_004b25d0(memory, nullptr)) : 0); // 004B92F3..004B931A / 004B934B..004B936C
    }
    void* pos = ptr(rd(rd(rd(s + 0x10064))));                                        // 004B9366..004B937F [esp+10] (message slot reused)
    if (pos != nullptr) {                                                            // 004B9379..004B9383
        do {
            const U client_object = unresolved_list_at_pos_005e9b60(ptr(rd(rd(s + 0x10064))), nullptr, pos); // 004B9390..004B9399
            const U player = vcall<U>(client_object, 4);                             // 004B939E..004B93A2 AsSWSPlayer
            if (player != 0) {                                                       // 004B93A5..004B93A9
                void* const table = kotor_fastcall_00401060(ptr(app()), nullptr, 1); // 004B93AF..004B93BB
                (void)kotor_fastcall_004ae650(table, nullptr, rd(player + 4));       // 004B93C0..004B93C2
                (void)kotor_fastcall_0056ccb0(ptr(message), nullptr, ptr(player));   // 004B93C7..004B93CC
                const U object = word(kotor_fastcall_00561080(ptr(player), nullptr)); // 004B93D1..004B93D3
                if (object != 0) {                                                   // 004B93D8..004B93DA
                    const U creature = vcall<U>(object, 0x30);                       // 004B93DC..004B93E0 AsSWSCreature
                    if (creature != 0) {                                             // 004B93E3..004B93E7
                        if (recovered_object_area_004cb120(ptr(creature), nullptr) != nullptr) // 004B93E9..004B93F2
                            kotor_fastcall_004f0fd0(ptr(creature), nullptr, 0);      // 004B93F4..004B93F7
                        (void)vcall<void*>(creature, 0, 1u);                         // 004B93FC..004B9403 deleting destructor
                        recovered_set_game_object_00561c40(ptr(player), nullptr, nullptr); // 004B9405..004B9408
                    }
                }
                kotor_fastcall_00562e40(ptr(player), nullptr);                       // 004B940D..004B940F
                wr8(player + 0x24, 0);                                               // 004B9414
                wr(player + 0x7c, 0);                                                // 004B9417
                wr(player + 0x34, k_invalid_00746644);                               // 004B941A
                (void)recovered_object_control_0056f430(ptr(rd(s + 0x10010)), nullptr,
                                                         rd(player + 4), k_invalid_00746644); // 004B9421..004B9430
            }
            (void)unresolved_list_next_005e9b90(ptr(rd(rd(s + 0x10064))), nullptr, &pos); // 004B9438..004B9445
        } while (pos != nullptr);                                                    // 004B944A..004B9450
    }
    if (rd(app() + 4) != 0 && kotor_fastcall_005ed8b0(ptr(rd(app() + 4)), nullptr) != nullptr) // 004B9456..004B946A
        kotor_fastcall_00634a40(kotor_fastcall_005ed8b0(ptr(rd(app() + 4)), nullptr), nullptr); // 004B946C..004B947B
    kotor_fastcall_00563600(ptr(s + 0x1b770), nullptr);                              // 004B9480..004B948A (non-NULL member)
    kotor_fastcall_0041dcf0(kotor_cell_tlk_007a3a08, nullptr);                       // 004B948F..004B9495
    if (module != 0) (void)vcall<void*>(module, 0, 1u);                              // 004B949A..004B94A6 deleting destructor
    wr(s + 0x10060, k_invalid_00746644);                                             // 004B94B0
    if (const U factions = rd(s + 0x10054)) {                                        // 004B94A8..004B94BA
        kotor_fastcall_0052b950(ptr(factions), nullptr);                             // 004B94BC..004B94BE
        live_free(ptr(factions));                                                    // 004B94C3..004B94C4
    }
    wr(s + 0x10054, 0);                                                              // 004B94CE
    void* const factions = live_operator_new(0x28);                                  // 004B94CC..004B94D9
    wr(s + 0x10054, factions != nullptr ? word(kotor_fastcall_0052b8e0(factions, nullptr)) : 0); // 004B94DC..004B9501
    if (objects != 0) {                                                              // 004B94F7..004B9507
        kotor_fastcall_004d7e10(ptr(objects), nullptr);                              // 004B9509..004B950B
        live_free(ptr(objects));                                                     // 004B9510..004B9511
    }
    void* const array = live_operator_new(0x18);                                     // 004B9519..004B9520
    wr(s + 0x1005c, array != nullptr
        ? word(static_cast<kotor_menu_10_19::ObjectArray*>(array)->construct_004d7d80(1)) : 0); // 004B9523..004B954B
    kotor_fastcall_004b11c0(ptr(rd(s + 0x10044)), nullptr);                          // 004B9541..004B9551
    alignas(4) unsigned char path[8];                                                // [esp+1C] CExoString
    frame_LogStringCtor_005e5a90(path, nullptr, "CURRENTGAME:");                     // 004B9556..004B955F ([0073D8F8])
    (void)gui_load_resman_nuke_directory_00409480(ptr(frame_control::resource_manager_007a39e8), nullptr, path, 1, 1); // 004B9564..004B957B
    frame_LogStringDtor_005e5c20(path, nullptr);                                     // 004B9580..004B9588
    return 1;                                                                        // 004B9593
}
