// Lane worker "s2p-chatinv": CSWCMessage::HandleServerToPlayerInventory 0064DEE0 (client side of the inventory
// equip / unequip / drop / pickup confirmations) plus its missing callees CSWCItem::PlayDropSound 006466D0,
// CClientExoApp::GetItemDropSound 005ED920 and CSWCMessage::SendPlayerToServerInventory_Equip 00677F30.
// Transcribed from the original swkotor.exe (read-only); statement comments cite original instruction addresses.
// ABI: original BOOL __thiscall CSWCMessage::HandleServerToPlayerInventory(BYTE minor) RET 4; every exit verified
// (0 at 0064DF08/0064E390, 1 at 0064E0D3/0064E145/0064E19F/0064E1BA/0064E218/0064E328/0064E381; all ret 4) ->
//   extern "C" unsigned __fastcall kotor_fastcall_0064dee0(void* self, void*, unsigned minor)   (@12).
// Conventions as claude_dialogue_common.hpp. No SEH frame in any of these four functions.
// QUIRKs kept: minors 1..4 dereference the looked-up object without a null check (0064DF72 / 0064E084), the
// [esp+1C] minor slot is used as a full dword for the non-BOOL minors (0064E012 / 0064E234), SetLastInstant /
// EquipToInventorySlot receive the item pointer as their second argument for minor 1 (0064E0B9), and
// SendPlayerToServerInventory_Equip writes its first argument with WriteBOOL.
#include "6bd5403233-claude_dialogue_common.hpp"

using namespace dlg;

extern "C" {
extern void* kotor_cell_rules_007a3a28;                     // [007A3A28] CSWRules*
extern void* kotor_cell_sound_007a39ec;                     // [007A39EC] CExoSound*

// w2_message_native.obj / w2_g07_native.obj
U __fastcall kotor_fastcall_004d6230(void* msg, void*, U bits);                           // MessageReadOverflow RET 4
U __fastcall kotor_fastcall_004d6240(void* msg, void*, U bits);                           // MessageReadUnderflow RET 4
U __fastcall kotor_fastcall_004d6780(void* msg, void*, U bits);                           // ReadDWORD RET 4
U __fastcall kotor_fastcall_004d66f0(void* msg, void*);                                   // ReadBOOL RET 0
U __fastcall kotor_fastcall_00692630(void* msg, void*);                                   // ReadOBJECTIDClient RET 0
void __fastcall kotor_fastcall_004d68a0(void* msg, void*, U size, U a, U b);              // CreateWriteMessage RET 0Ch
void __fastcall kotor_fastcall_004d68e0(void* msg, void*, U value);                       // WriteBOOL RET 4
void __fastcall kotor_fastcall_004d6340(void* msg, void*, U value, U bits);               // WriteDWORD RET 8
U __fastcall kotor_fastcall_004d6440(void* msg, void*, void** data, U* size);             // GetWriteMessage RET 8
U __fastcall recovered_client_send_00677410(void* msg, void*, U major, U minor, void* data, U size); // SendPlayerToServerMessage
// client app / objects
void* __fastcall kotor_fastcall_005ed570(void* client, void*);                            // GetGameObjectArray
void* __fastcall client_get_player_creature_005ed540(void* client, void*);                // GetPlayerCreature
void* __fastcall kotor_fastcall_005ed580(void* client, void*, U id);                      // GetGameObject RET 4
void* __fastcall kotor_fastcall_005ed6f0(void* client, void*);                            // GetSWCMessage (1205-30)
void* __fastcall kotor_m15e_item_get_equipped_by_00646630(void* item, void*);             // CSWCItem::GetEquippedBy
void __cdecl kotor_m15a_switch_weapon_event_00610f40(U a, const char* b, void* creature); // SwitchWeaponEvent (cdecl)
U __fastcall kotor_m15a_creature_get_equipped_item_id_0060ca80(void* creature, void*, U slot); // GetEquippedItemID RET 4
void __fastcall recovered_client_creature_set_last_instant_0060cd50(void* creature, void*, U slot, U value); // RET 8
U __fastcall recovered_client_creature_equip_to_inventory_slot_00613ba0(void* creature, void*, U slot, U item); // RET 8
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);               // CSWItem::GetBaseItem
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, I id); // CSWBaseItemArray::GetBaseItem
void* __fastcall unresolved_ref_assign_00406290(void* r, void*, void* out, const char* text); // CResRef::operator=(char*)
U __fastcall w5_sound_play_one_shot_005d5e00(void* sound, void*, void* ref, U group, U delay, U volume, U fixed,
                                             U pitch);                                     // PlayOneShotSound RET 18h

// ---------------- exports ----------------
void* __fastcall kotor_fastcall_005ed920(void* client, void*, void* out, U index);
U __fastcall kotor_fastcall_006466d0(void* item, void*);
U __fastcall kotor_fastcall_00677f30(void* message, void*, U value, U slot);
U __fastcall kotor_fastcall_0064dee0(void* self, void*, U minor);
}

namespace {
constexpr U kNoSlot = 0xffffffffu;      // or edi,-1 (0064E16B / 0064E318)
// .rdata [00751BD4] "it_materialcloth", [00751BC4] "it_armorleather", [00751BB4] "it_armorchain", [00751BA4] "it_armorplate"
const char k_cloth[] = "it_materialcloth";
const char k_leather[] = "it_armorleather";
const char k_chain[] = "it_armorchain";
const char k_plate[] = "it_armorplate";

void* client_array() { return kotor_fastcall_005ed570(client(), nullptr); }
void* player_creature() { return client_get_player_creature_005ed540(client(), nullptr); }
// CGameObjectArray::GetGameObject(id, &out) compared with the status byte [0074666C] (= 0)
bool lookup(void* array, U id, void** out) { return kotor_fastcall_004d8230(array, nullptr, id, out) == kStatusOk; }
void* as_item(void* o) { return vcall<void*>(o, 0x34); }
U equipped_in_slot_of(void* item) { return kotor_m15a_creature_get_equipped_item_id_0060ca80(player_creature(), nullptr, rd(item, 0x154)); }
void set_last_instant(U slot, U value) { recovered_client_creature_set_last_instant_0060cd50(player_creature(), nullptr, slot, value); }
void equip_to_slot(U slot, U item) { (void)recovered_client_creature_equip_to_inventory_slot_00613ba0(player_creature(), nullptr, slot, item); }
}  // namespace

// =================================================================================================================
// 005ED920 CClientExoApp::GetItemDropSound(BYTE index) -> CResRef by value (hidden out), RET 8
// =================================================================================================================
extern "C" void* __fastcall kotor_fastcall_005ed920(void* client_app, void*, void* out, U index) {
    void* const internal = rp(client_app, 4);                                    // 005ED920
    void* const table = rp(internal, 0x1c8);                                     // 005ED923
    if (table != nullptr) {                                                      // 005ED929..005ED92C
        const void* const entry = fa(table, static_cast<U>(static_cast<U8>(index)) << 4); // 005ED92E..005ED936
        wr(out, 0x0, rd(entry, 0x0));                                            // 005ED938..005ED940
        wr(out, 0x4, rd(entry, 0x4));                                            // 005ED942..005ED945
        wr(out, 0x8, rd(entry, 0x8));                                            // 005ED948..005ED94B
        wr(out, 0xc, rd(entry, 0xc));                                            // 005ED94E..005ED951
        return out;                                                              // 005ED93A (eax = out)
    }
    resref_from(out, "");                                                        // 005ED958..005ED963 [0073D71C] ""
    return out;                                                                  // 005ED968
}

// =================================================================================================================
// 006466D0 CSWCItem::PlayDropSound() RET 0 (EAX = PlayOneShotSound result)
// =================================================================================================================
extern "C" U __fastcall kotor_fastcall_006466d0(void* item, void*) {
    ResRef sound; resref_default(&sound);                                        // [esp+8] 006466D6..006466DA
    const U8 type_low = rd8(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0xac); // 006466DF..006466E6
    bool armor = false;
    if (type_low >= 0x1f) {                                                      // 006466ED jb
        armor = rd8(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0xac) <= 0x26; // 006466EF..006466FD ja
    }
    if (armor) {
        ResRef t;                                                                // [esp+18/28/38/48]
        switch (rd8(item, 0x110)) {                                              // 006466FF..0064670F (table 006467C8)
        case 0: (void)unresolved_ref_assign_00406290(&sound, nullptr, &t, k_leather); break;  // 0064672B..00646739
        case 1: (void)unresolved_ref_assign_00406290(&sound, nullptr, &t, k_chain); break;    // 00646740..0064674E
        case 2: (void)unresolved_ref_assign_00406290(&sound, nullptr, &t, k_plate); break;    // 00646755..00646763
        case 3: (void)unresolved_ref_assign_00406290(&sound, nullptr, &t, k_cloth); break;    // 00646716..00646724
        default: break;                                                          // 00646709 ja 006467A9
        }
    } else {
        void* const rules = kotor_cell_rules_007a3a28;                           // 0064676D
        void* const base = kotor_m15b_base_item_array_get_base_item_005b31a0(rp(rules, 0x34), nullptr,
                                                                             rdi(item, 0xc)); // 0064676A..00646777
        const U index = rd8(base, 0x78);                                         // 0064677C..0064678B (low byte used)
        ResRef drop;                                                             // [esp+48]
        void* const got = kotor_fastcall_005ed920(client(), nullptr, &drop, index); // 0064677F..00646795
        resref_assign(&sound, got);                                              // 0064679A..006467A4 (tmp [esp+38])
    }
    return w5_sound_play_one_shot_005d5e00(kotor_cell_sound_007a39ec, nullptr, &sound, 0xb, 0, 0, 0, 0); // 006467A9..006467BE
}

// =================================================================================================================
// 00677F30 CSWCMessage::SendPlayerToServerInventory_Equip(value, slot) RET 8 -> major 0Ch minor 1
// =================================================================================================================
extern "C" U __fastcall kotor_fastcall_00677f30(void* message, void*, U value, U slot) {
    U size = 8;                                                                  // [esp+4] 00677F3C
    void* data;                                                                  // [esp+8]
    kotor_fastcall_004d68a0(message, nullptr, 8, 0xffffffffu, 1);                // 00677F34..00677F44
    kotor_fastcall_004d68e0(message, nullptr, value);                            // 00677F49..00677F50 WriteBOOL
    kotor_fastcall_004d6340(message, nullptr, slot, 0x20);                       // 00677F55..00677F5E WriteDWORD
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;  // 00677F63..00677F7C
    return recovered_client_send_00677410(message, nullptr, 0xc, 1, data, size); // 00677F7F..00677F98
}

// =================================================================================================================
// 0064DEE0 CSWCMessage::HandleServerToPlayerInventory(BYTE minor) RET 4
// =================================================================================================================
extern "C" U __fastcall kotor_fastcall_0064dee0(void* self, void*, U minor_arg) {
    void* object = nullptr;                                                      // [esp+10] 0064DEEA
    const U id = kotor_fastcall_00692630(self, nullptr);                         // 0064DEEE..0064DEF9 edi, [esp+14]
    if (kotor_fastcall_004d6230(self, nullptr, 1) != 0) return 0;                // 0064DEF5..0064DF0E
    void* const array = client_array();                                          // 0064DF11..0064DF1F ebx
    if (array == nullptr) return 0;                                              // 0064DF21..0064DF23
    const U8 minor = static_cast<U8>(minor_arg);                                 // 0064DF29 cl
    switch (minor) {                                                             // 0064DF2D..0064DF41 (table 0064E3B8)
    case 3: case 4: {                                                            // 0064DF48
        if (kotor_fastcall_004d6240(self, nullptr, 1) != 0) return 0;            // 0064DF48..0064DF53
        if (!lookup(array, id, &object)) return 1;                               // 0064DF59..0064DF6C
        void* const item = as_item(object);                                      // QUIRK 0064DF72..0064DF78 (no null check)
        if (item == nullptr) return 1;                                           // 0064DF7D..0064DF7F
        if (minor != 4) {                                                        // 0064DF85..0064DF8C
            (void)kotor_fastcall_006466d0(item, nullptr);                        // 0064DF8E..0064DF90
            if (void* const owner = kotor_m15e_item_get_equipped_by_00646630(item, nullptr)) // 0064DF95..0064DF9E
                kotor_m15a_switch_weapon_event_00610f40(0, nullptr, owner);      // 0064DFA0..0064DFA8
        }
        wr(item, 0x148, 0);                                                      // 0064DFAE
        if (minor != 3) return 1;                                                // 0064DFAB / 0064DFB4
        if (equipped_in_slot_of(item) != rd(item, 0x14)) return 1;               // 0064DFBA..0064DFD9
        const U slot = rd(item, 0x154);                                          // 0064DFDF
        if (slot == 1) return 1;                                                 // 0064DFE5..0064DFE8
        equip_to_slot(slot, 0);                                                  // 0064DFEE..0064DFF0 -> 0064E0BB..0064E0CB
        return 1;
    }
    case 1: case 2: {                                                            // 0064DFF5
        const U slot = kotor_fastcall_004d6780(self, nullptr, 0x20);             // 0064DFF5..0064DFFE edi
        U value;                                                                 // ebp
        if (minor == 1) value = kotor_fastcall_004d66f0(self, nullptr);          // 0064E000..0064E010
        else value = minor_arg;                                                  // QUIRK 0064E012 full dword
        if (kotor_fastcall_004d6230(self, nullptr, 1) != 0) return 0;            // 0064E016..0064E021
        if (kotor_fastcall_004d6240(self, nullptr, 1) != 0) return 0;            // 0064E027..0064E032
        if (minor != 1) return 1;                                                // 0064E038..0064E03D
        if (player_creature() == nullptr) return 1;                              // 0064E043..0064E052
        set_last_instant(slot, value);                                           // 0064E054..0064E066
        if (!lookup(array, id, &object)) return 1;                               // 0064E06B..0064E082
        void* const item = as_item(object);                                      // QUIRK 0064E084..0064E08A (no null check)
        if (item == nullptr) return 1;                                           // 0064E08D..0064E091
        if (equipped_in_slot_of(item) == rd(item, 0x14)) return 1;               // 0064E093..0064E0B2
        if (slot == 1) return 1;                                                 // 0064E0B4..0064E0B7
        equip_to_slot(slot, word(item));                                         // QUIRK 0064E0B9..0064E0CB item as arg 2
        return 1;
    }
    case 5: case 6: {                                                            // 0064E0DF
        if (kotor_fastcall_004d6230(self, nullptr, 1) != 0) return 0;            // 0064E0DF..0064E0EA
        if (kotor_fastcall_004d6240(self, nullptr, 1) != 0) return 0;            // 0064E0F0..0064E0FB
        if (!lookup(array, id, &object)) return 1;                               // 0064E101..0064E114
        if (object == nullptr) return 1;                                         // 0064E11A..0064E120
        void* const item = as_item(object);                                      // 0064E126..0064E128
        if (item == nullptr) return 1;                                           // 0064E12B..0064E12F
        if (minor == 6) {                                                        // 0064E135..0064E13A
            wr(item, 0x148, 0);                                                  // 0064E13E
            return 1;
        }
        (void)kotor_fastcall_006466d0(item, nullptr);                            // 0064E151..0064E153
        (void)vcall<U>(fa(item, 0x10), 0x94, 0u);                                // 0064E158..0064E15F
        if (rd(item, 0x14c) == kNoSlot) {                                        // 0064E165..0064E170
            wr(item, 0x148, 0);                                                  // 0064E1AB
            wr(item, 0x14c, kNoSlot);                                            // 0064E1B1
            return 1;
        }
        void* const message = kotor_fastcall_005ed6f0(client(), nullptr); // 0064E172..0064E17B
        if (message == nullptr) {                                                // 0064E180..0064E182
            wr(item, 0x14c, kNoSlot);                                            // 0064E1B1
            return 1;
        }
        (void)kotor_fastcall_00677f30(message, nullptr, rd(item, 0x14), rd(item, 0x14c)); // 0064E184..0064E191
        wr(item, 0x14c, kNoSlot);                                                // 0064E197
        return 1;
    }
    case 15: {                                                                   // 0064E1C6
        if (kotor_fastcall_004d6230(self, nullptr, 1) != 0) return 0;            // 0064E1C6..0064E1D1
        if (kotor_fastcall_004d6240(self, nullptr, 1) != 0) return 0;            // 0064E1D7..0064E1E2
        if (!lookup(array, id, &object)) return 1;                               // 0064E1E8..0064E1FB
        if (object == nullptr) return 1;                                         // 0064E1FD..0064E203
        if (void* const item = as_item(object)) (void)kotor_fastcall_006466d0(item, nullptr); // 0064E205..0064E210
        return 1;
    }
    case 7: case 8: {                                                            // 0064E224
        U value;                                                                 // ebp
        if (minor == 7) value = kotor_fastcall_004d66f0(self, nullptr);          // 0064E224..0064E232
        else value = minor_arg;                                                  // QUIRK 0064E234 full dword
        if (kotor_fastcall_004d6230(self, nullptr, 1) != 0) return 0;            // 0064E238..0064E243
        if (kotor_fastcall_004d6240(self, nullptr, 1) != 0) return 0;            // 0064E249..0064E254
        if (!lookup(array, id, &object)) return 1;                               // 0064E25A..0064E26D
        if (object == nullptr) return 1;                                         // 0064E273..0064E279
        void* const item = as_item(object);                                      // 0064E27F..0064E281
        if (item == nullptr) return 1;                                           // 0064E284..0064E288
        if (minor != 8 && value == 0) (void)kotor_fastcall_006466d0(item, nullptr); // 0064E28E..0064E29D
        wr(item, 0x148, 0);                                                      // 0064E2A5
        if (minor != 7) return 1;                                                // 0064E2A2 / 0064E2AF
        if (equipped_in_slot_of(item) != rd(item, 0x14)) return 1;               // 0064E2B1..0064E2D0
        if (rd(item, 0x154) != 1) {                                              // 0064E2D2..0064E2DB
            set_last_instant(rd(item, 0x154), value);                            // 0064E2DD..0064E2EF
            equip_to_slot(rd(item, 0x154), 0);                                   // 0064E2F4..0064E30D
        }
        if (rd(item, 0x154) != kNoSlot) wr(item, 0x154, kNoSlot);                // 0064E312..0064E31F
        return 1;
    }
    case 14: {                                                                   // 0064E334
        if (kotor_fastcall_004d6230(self, nullptr, 1) != 0) return 0;            // 0064E334..0064E33F
        if (kotor_fastcall_004d6240(self, nullptr, 1) != 0) return 0;            // 0064E341..0064E34C
        void* const o = kotor_fastcall_005ed580(client(), nullptr, id);          // 0064E34E..0064E35F [esp+10]
        if (o == nullptr) return 1;                                              // 0064E363
        void* const item = as_item(o);                                           // 0064E369..0064E36D
        if (item == nullptr) return 1;                                           // 0064E370..0064E372
        wr(item, 0x148, 0);                                                      // 0064E37A
        return 1;
    }
    default:                                                                     // 9..13 (table 6), 0 and >15
        return 0;                                                                // 0064E38D..0064E396
    }
}
