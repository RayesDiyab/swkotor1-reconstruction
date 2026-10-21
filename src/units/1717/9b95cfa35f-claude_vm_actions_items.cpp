// Lane endar-actions, group "items": NWScript engine-command bodies (CSWVirtualMachineCommands::ExecuteCommand*)
// for item/inventory routines used by the Endar Spire scripts, plus the two CSWSCreature equip-most-damaging
// helpers they need (no provider existed). Transcribed from the original swkotor.exe (read-only, capstone);
// the addresses in the comments are the original instructions.
//  * handlers: int __thiscall ExecuteCommandX(int routine, int argc), RET 8 -> extern "C" __fastcall @16
//  * returns 0 / -2001 (pop failure, 0xFFFFF82F) / -2000 (push failure, 0xFFFFF830)
//  * CSWVirtualMachineCommands: +4 "object-run-script valid" dword (== 1), +8 object-run-script id
#include "061872cb05-claude_vm_actions_common.hpp"


extern "C" {
extern void* kotor_cell_rules_007a3a28;                                                        // g_pRules 007A3A28 [0421]
void* __fastcall lower_construct_005e6080(void* s, void*, void* out);                          // CExoString::LowerCase [0310]
void* __fastcall frame_R4_005ed690(void* client, void*);                                       // CClientExoApp::GetInGameGui [0457]
void __fastcall kotor_fastcall_0062eeb0(void* gui, void*, U kind, U value);                    // CGuiInGame::UpdateStatus [src_hud...]
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, U id);  // CSWBaseItemArray::GetBaseItem [0820]
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);                 // CSWCCMessageData() [w1_native_b7]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);         // CSWCCMessageData::SetInteger [1017]
U __fastcall recovered_find_item_with_tag_00555ed0(void* repository, void*, void* tag);        // CItemRepository::FindItemWithTag [w2_party_native]
U __fastcall recovered_remove_item_00555fd0(void* repository, void*, void* item);              // CItemRepository::RemoveItem [w2_party_native]
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, U index);                // CItemRepository::ItemListGetItem [0886]
U __fastcall recovered_inventory_insert_0055d330(void* repository, void*, void** item, U a, U b, U c); // CItemRepository::AddItem [1088]
U __fastcall recovered_object_equip_004f0420(void* creature, void*, void* item, U slot, U a3, U a4, U instant, U a6, U a7); // AddEquipItemActions [clear9]
U __fastcall recovered_object_unequip_004f06d0(void* creature, void*, void* item, U target, U a3, U a4, U instant);         // AddUnequipActions [clear9]
U __fastcall recovered_creature_unequip_item_004faa70(void* creature, void*, void* item);      // CSWSCreature::UnequipItem [w1_native_b7]
U __fastcall recovered_creature_equip_004feb90(void* creature, void*, U slot, void* item, U a3, U a4, U a5); // CSWSCreature::EquipItem [pf_item_equip]
unsigned char __fastcall recovered_item_can_equip_0051aa60(void* creature, void*, void* item, U* slot, U a3, U a4, U a5); // CanEquipItem [1025]
U __fastcall kotor_fastcall_004d3da0(void* round, void*);                                      // CSWSCombatRound::GetWeaponAttackType [claude_combat_attack_round]
I __fastcall kotor_fastcall_005a9050(void* stats, void*, void* target, U offhand, U a3, U a4, U a5, U a6); // GetDamageRoll [claude_combat_attack_damagestats]
I __fastcall kotor_fastcall_005b5640(void* creature, void*, void* target, U offhand);          // CalculateMaxElementalDamage [claude_combat_attack_damage]
I __fastcall kotor_fastcall_005a7ea0(void* stats, void*, void* target);                        // GetAttackModifierVersus [claude_combat_attack_attackstats]
void __cdecl genuine_x87_pow_006fc1f0();                                                       // CRT _CIpow (ST1^ST0 -> ST0) [0365]
void __cdecl genuine_x87_ftol_006fae8c();                                                      // CRT __ftol2 (ST0 -> EDX:EAX) [0206]
// reconstructed below
U __fastcall kotor_fastcall_00500e50(void* creature, void*, U target, U offhand, U recursed);
U __fastcall kotor_fastcall_00501250(void* creature, void*, U target);
}

namespace {
using namespace vmx;
constexpr U k_first_item_in_inventory = 0x153;   // routine 339 GetFirstItemInInventory (340 = GetNextItemInInventory)

inline U rd(void* p, U off) { return at<U>(p, off); }
inline U rd(U p, U off) { return at<U>(ptr(p), off); }
inline U16 rd16(void* p, U off) { return at<U16>(p, off); }
inline U8 rd8(void* p, U off) { return at<U8>(p, off); }
inline void* rp(void* p, U off) { return at<void*>(p, off); }

inline void* as_object(void* o) { return vcall<void*>(o, 0x10); }       // vtbl+10 AsSWSObject
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }     // vtbl+30 AsSWSCreature
inline void* as_item(void* o) { return vcall<void*>(o, 0x38); }         // vtbl+38 AsSWSItem
inline void* as_placeable(void* o) { return vcall<void*>(o, 0x48); }    // vtbl+48 AsSWSPlaceable
inline void* as_store(void* o) { return vcall<void*>(o, 0x64); }        // vtbl+64 AsSWSStore

inline void* item_by_id(U id) { return recovered_item_lookup_004ae760(server_app(), nullptr, id); }
inline void* creature_by_id(U id) { return kotor_fastcall_004ae770(server_app(), nullptr, id); }
inline void* base_item(U base_id) {                                     // [[007A3A28]+34]->GetBaseItem(id)
    return kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(kotor_cell_rules_007a3a28, 0x34), nullptr, base_id);
}
inline void* repository(void* creature) { return recovered_creature_inventory_004ef770(creature, nullptr, 1); }
inline I push_int(U v) { return recovered_vm_outer_push_integer_005d1010(vm(), nullptr, static_cast<I>(v)) ? 0 : k_push_failed; }
inline I push_object(U v) { return recovered_vm_outer_push_object_005d10d0(vm(), nullptr, v) ? 0 : k_push_failed; }
inline bool pop_object(U* v) { return recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, v) != 0; }
inline bool pop_int(I* v) { return recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, v) != 0; }

// fld qword [00741DF8] (2.0); fild slot; call _CIpow; call __ftol2  (005356 67..76)
U x87_pow2_ftol(I slot) {
    static const double two = 2.0;                                       // .rdata 00741DF8
    U result;
    __asm {
        fld qword ptr two
        fild dword ptr slot
        call genuine_x87_pow_006fc1f0
        call genuine_x87_ftol_006fae8c
        mov result, eax
    }
    return result;
}
}  // namespace

extern "C" {

// =================================================================================================== 005369B0
// GetBaseItemType(object oItem): item +0C base item id; 0x100 (BASE_ITEM_INVALID) when not an item.
int __fastcall recovered_vm_get_base_item_type_005369b0(void*, void*, int, unsigned) {
    U id;
    if (!pop_object(&id)) return k_pop_failed;                           // 005369B1..005369CB
    U result = 0x100;                                                    // 005369DD
    void* item = item_by_id(id);                                         // 005369CE..005369E2
    if (item) result = rd(item, 0xc);                                    // 005369E7..005369EB
    return push_int(result);                                             // 005369EE..00536A0A
}

// =================================================================================================== 005465D0
// GetItemStackSize(object oItem): item +28C; 0 when not an item.
int __fastcall recovered_vm_get_item_stack_size_005465d0(void*, void*, int, unsigned) {
    U id;
    U result = 0;                                                        // 005465DD
    if (!pop_object(&id)) return k_pop_failed;                           // 005465D1..005465EF
    void* item = item_by_id(id);                                         // 005465F2..00546600
    if (item) result = rd(item, 0x28c);                                  // 00546605..00546609
    return push_int(result);                                             // 0054660F..0054662B
}

// =================================================================================================== 0053C220
// GetNumStackedItems(object oItem): item +28C; -1 when not an item.
int __fastcall recovered_vm_get_num_stacked_items_0053c220(void*, void*, int, unsigned) {
    U id;
    if (!pop_object(&id)) return k_pop_failed;                           // 0053C221..0053C23B
    void* item = item_by_id(id);                                         // 0053C23E..0053C24C
    const U result = item ? rd(item, 0x28c) : 0xffffffffu;               // 0053C251..0053C25D
    return push_int(result);                                             // 0053C260..0053C27B
}

// =================================================================================================== 00546630
// SetItemStackSize(object oItem, int nSize): clamp to [1, base item +24 (word, max stack)] (signed compares);
// on change store +28C, send feedback 0x32 (grew) / 0x33 (shrank) to the possessor (+268) with object id
// (item +14) and integer 1, then CGuiInGame::UpdateStatus(7|8, 0). Quirks kept: the possessor creature is not
// null-checked before SendFeedbackMessage; a failed operator new leaves the message pointer null (SetObjectID
// on null), exactly like the original.
int __fastcall recovered_vm_set_item_stack_size_00546630(void*, void*, int, unsigned) {
    U id = 0; I size;
    if (!pop_object(&id) || !pop_int(&size)) return k_pop_failed;        // 00546648..0054668B
    void* item = item_by_id(id);                                         // 0054668E..005466A2
    if (!item) return 0;                                                 // 005466A4..005466A6 -> 0054678E
    const U16 max = rd16(base_item(rd(item, 0xc)), 0x24);                // 005466AC..005466BE
    if (size > static_cast<I>(max)) size = max;                          // 005466C2..005466D0 (jle, signed)
    if (size < 1) size = 1;                                              // 005466D4..005466DE (jge, signed)
    const I old = static_cast<I>(rd(item, 0x28c));                       // 005466E4
    if (size == old) return 0;                                           // 005466EA..005466EC
    at<U>(item, 0x28c) = static_cast<U>(size);                           // 005466F2
    void* possessor = creature_by_id(rd(item, 0x268));                   // 005466F8..00546708
    void* memory = live_operator_new(0x34);                              // 0054670D..00546716
    void* data = nullptr;                                                // 0054671F (edi = 0)
    if (memory) {
        try { data = recovered_message_data_construct_004d69f0(memory, nullptr); }   // 00546725..0054672C (EH state 0)
        catch (...) { live_free(memory); throw; }
    }
    recovered_event_object_004d7780(data, nullptr, 0, rd(item, 0x14));   // 0054672E..0054673E
    recovered_event_integer_004d76b0(data, nullptr, 0, 1);               // 00546743..00546749
    void* gui;
    if (size > old) {                                                    // 0054674E..00546755 (jle, signed)
        recovered_creature_feedback_004ede10(possessor, nullptr, 0x32, data);  // 00546757..00546759
        gui = frame_R4_005ed690(client_app(), nullptr);                  // 0054675E..00546780
        kotor_fastcall_0062eeb0(gui, nullptr, 7, 0);                     // 00546766..00546787
    } else {
        recovered_creature_feedback_004ede10(possessor, nullptr, 0x33, data);  // 0054676C..0054676E
        gui = frame_R4_005ed690(client_app(), nullptr);                  // 00546773..00546780
        kotor_fastcall_0062eeb0(gui, nullptr, 8, 0);                     // 0054677C..00546787
    }
    return 0;                                                            // 0054678C..005467A0
}

// =================================================================================================== 0053A390
// GetItemPossessedBy(object oCreature, string sItemTag): lowercased tag; creature: repository(1) search, then
// the 18 equipment slots (1 << i) comparing item +28 tag; placeable with +324 set: repository +36C; item with
// a container repository (+26C): that repository. Pushes the item id (+14) or OBJECT_INVALID.
int __fastcall recovered_vm_get_item_possessed_by_0053a390(void*, void*, int, unsigned) {
    ExoString tag; exo_default(&tag);                                    // 0053A3B2..0053A3BB
    U result = k_invalid;                                                // 0053A3B6 (ebp)
    U id = 0;
    if (!pop_object(&id)) { exo_destroy(&tag); return k_pop_failed; }    // 0053A3C0..0053A3F0
    if (!recovered_vm_outer_pop_string_005d1080(vm(), nullptr, &tag)) { exo_destroy(&tag); return k_pop_failed; } // 0053A3F5..0053A41F
    ExoString lower;                                                     // [esp+20]
    lower_construct_005e6080(&tag, nullptr, &lower);                     // 0053A424..0053A429
    void* object = nullptr;
    try {
        if (lookup(id, &object)) {                                       // 0053A42E..0053A457
            if (as_creature(object)) {                                   // 0053A45D..0053A46C
                void* creature = as_creature(object);                    // 0053A46E..0053A478
                result = recovered_find_item_with_tag_00555ed0(repository(creature), nullptr, &lower); // 0053A47A..0053A490
                void* inventory = rp(creature, 0xa2c);                   // 0053A48A..0053A49D
                U found = result != k_invalid ? 1u : 0u;                 // 0053A492..0053A49A
                for (U i = 0; found == 0 && i < 0x12; ++i) {             // 0053A4A1..0053A4DF
                    void* slot_item = recovered_equipment_item_005a4c20(inventory, nullptr, 1u << i);   // 0053A4A9..0053A4BC
                    if (slot_item && async_string_equal_005e5200(static_cast<char*>(slot_item) + 0x28, nullptr, &lower)) { // 0053A4BE..0053A4D1
                        result = rd(slot_item, 0x14);                    // 0053A4D3
                        found = 1;                                       // 0053A4D6
                    }
                }
            } else if (as_placeable(object)) {                           // 0053A4E3..0053A4EE
                if (rd(as_placeable(object), 0x324) != 0 && rd(as_placeable(object), 0x36c) != 0) // 0053A4F0..0053A50C
                    result = recovered_find_item_with_tag_00555ed0(rp(as_placeable(object), 0x36c), nullptr, &lower); // 0053A50E..0053A554
            } else if (as_item(object)) {                                // 0053A51F..0053A526
                if (rd(as_item(object), 0x26c) != 0)                     // 0053A528..0053A537
                    result = recovered_find_item_with_tag_00555ed0(rp(as_item(object), 0x26c), nullptr, &lower); // 0053A539..0053A554
            }
        }
    } catch (...) { exo_destroy(reinterpret_cast<ExoString*>(&lower)); exo_destroy(&tag); throw; }
    const U pushed = recovered_vm_outer_push_object_005d10d0(vm(), nullptr, result);   // 0053A556..0053A55D
    exo_destroy(reinterpret_cast<ExoString*>(&lower));                   // 0053A56F / 0053A58C
    exo_destroy(&tag);                                                   // 0053A574..0053A580 / 0053A591..0053A59D
    return pushed ? 0 : k_push_failed;                                   // 0053A585 / 0053A5A2
}

// =================================================================================================== 00549B50
// GetFirstItemInInventory (339) / GetNextItemInInventory (340). Iteration state: creature +A34 (word, next
// top-level index), +A36 (word, next index inside the open container), +A38 (open container item id);
// placeable +370/+372/+374 likewise; item +298 (word, next index); store +254 (word, counter only).
// The result defaults to OBJECT_INVALID (7F000000) when nothing is found or the object is not a
// creature/placeable/item (REVIEW-ENDAR-ACTIONS finding 1); a store only advances its counter. On the item path the original
// compares against an uninitialised local ([esp+10]); both outcomes of that compare lead to the same state
// (the top-level fallback re-selects the same repository and neither the creature nor the placeable branch
// applies to an item), so the compare is modelled against the item's own repository.
int __fastcall recovered_vm_get_item_in_inventory_00549b50(void*, void*, int routine, unsigned) {
    U id = k_invalid;                                                    // 00549B61
    U result = k_invalid;                                                // 00549B61 / 00549EF6..EFC (review fix 1: OBJECT_INVALID when not found)
    if (!pop_object(&id)) return k_pop_failed;                           // 00549B53..00549B7F
    void* object = game_object(id);                                      // 00549B82..00549B96
    if (!object) return push_object(result);                             // 00549B98..00549B9A -> 00549EFC
    const bool first = routine == static_cast<int>(k_first_item_in_inventory);
    U16 index = 0;                                                       // di (edi = 0)
    void* repo = nullptr;                                                // ebp
    void* top = nullptr;                                                 // [esp+10]
    if (as_creature(object)) {                                           // 00549BA0..00549BAD
        void* c = as_creature(object);                                   // 00549BB3..00549BB8
        repo = repository(c);                                            // 00549BBA..00549BC3
        top = repo;                                                      // 00549BCD
        if (first) {                                                     // 00549BC5..00549BD1
            at<U>(c, 0xa38) = k_invalid;                                 // 00549BD7..00549BDC
            at<U16>(c, 0xa36) = 0;                                       // 00549BE2
            at<U16>(c, 0xa34) = 1;                                       // 00549BEB
        } else {
            void* open = item_by_id(rd(c, 0xa38));                       // 00549C70..00549C80
            if (open && rd(open, 0x268) == rd(c, 4)) {                   // 00549C85..00549C92
                repo = rp(open, 0x26c);                                  // 00549C94
                index = rd16(c, 0xa36);                                  // 00549C9A..00549C9C
                at<U16>(c, 0xa36) = static_cast<U16>(index + 1);         // 00549CA3..00549CA6
            } else {
                at<U16>(c, 0xa36) = 0;                                   // 00549CB8 (di = 0)
                index = rd16(c, 0xa34);                                  // 00549CBF..00549CC1
                at<U>(c, 0xa38) = k_invalid;                             // 00549CB2, 00549CC8
                at<U16>(c, 0xa34) = static_cast<U16>(index + 1);         // 00549CCE..00549CD1
            }
        }
    } else if (as_placeable(object)) {                                   // 00549CDD..00549CE6
        void* p = as_placeable(object);                                  // 00549CEC..00549CF1
        repo = rp(p, 0x36c); top = repo;                                 // 00549CFB..00549D01
        if (first) {                                                     // 00549CF3..00549D05
            at<U>(p, 0x374) = k_invalid;                                 // 00549D07..00549D0C
            at<U16>(p, 0x372) = 0;                                       // 00549D12
            at<U16>(p, 0x370) = 1;                                       // 00549D1B
        } else {
            void* open = item_by_id(rd(p, 0x374));                       // 00549D29..00549D39
            if (open && rd(open, 0x268) == rd(p, 4)) {                   // 00549D3E..00549D4B
                repo = rp(open, 0x26c);                                  // 00549D4D
                index = rd16(p, 0x372);                                  // 00549D53..00549D55
                at<U16>(p, 0x372) = static_cast<U16>(index + 1);         // 00549D5C..00549D5F
            } else {
                at<U16>(p, 0x372) = 0;                                   // 00549D71
                index = rd16(p, 0x370);                                  // 00549D78..00549D7A
                at<U>(p, 0x374) = k_invalid;                             // 00549D6B, 00549D81
                at<U16>(p, 0x370) = static_cast<U16>(index + 1);         // 00549D87..00549D8A
            }
        }
    } else if (as_item(object)) {                                        // 00549D96..00549D9F
        repo = rp(as_item(object), 0x26c);                               // 00549DA1..00549DA6
        top = repo;                                                      // (uninitialised in the original; see above)
        if (!first) index = rd16(as_item(object), 0x298);                // 00549DAC..00549DBF
        at<U16>(as_item(object), 0x298) = static_cast<U16>(index + 1);   // 00549DC6..00549DD0
    } else {
        void* s = as_store(object);                                      // 00549DDC..00549DE3
        if (s) {
            void* store = as_store(object);                              // 00549DE9..00549DED
            if (first) at<U16>(store, 0x254) = 1;                        // 00549DF0..00549DFD
            else at<U16>(store, 0x254) = static_cast<U16>(rd16(store, 0x254) + 1);   // 00549E09..00549E13
        }
        return push_object(result);                                      // 00549EFA -> 00549EFC
    }
    // 00549BF4: shared continuation (ebp = repository, di = index)
    if (!repo) return push_object(result);                               // 00549BF4..00549BF6
    if (!(rd(repo, 0x10) > static_cast<U>(index)) && repo != top) {      // 00549BFC..00549C10 (ja, unsigned)
        repo = top;                                                      // 00549C1A: back to the top level
        if (as_creature(object)) {                                       // 00549C16..00549C25
            index = rd16(as_creature(object), 0xa34);                    // 00549C2B..00549C32
            at<U16>(as_creature(object), 0xa34) = static_cast<U16>(index + 1);   // 00549C3B..00549C41
            at<U>(as_creature(object), 0xa38) = k_invalid;               // 00549C48..00549C55
            at<U16>(as_creature(object), 0xa36) = 0;                     // 00549C5B..00549C62
        } else if (as_placeable(object)) {                               // 00549E1F..00549E24
            index = rd16(as_placeable(object), 0x370);                   // 00549E26..00549E2F
            at<U16>(as_placeable(object), 0x370) = static_cast<U16>(index + 1);  // 00549E36..00549E40
            at<U>(as_placeable(object), 0x374) = k_invalid;              // 00549E47..00549E54
            at<U16>(as_placeable(object), 0x372) = 0;                    // 00549E5A..00549E61
        }
    }
    // 00549E6A
    if (rd(repo, 0x10) <= static_cast<U>(index)) return push_object(result);   // 00549E6A..00549E72 (jbe)
    void* item = w2x_inventory_item_005560b0(repo, nullptr, index);      // 00549E78..00549E7B
    if (!item) return push_object(result);                               // 00549E80..00549E86
    void* base = base_item(rd(item, 0xc));                               // 00549E88..00549E95
    if (base && rd(base, 0x70) != 0) {                                   // 00549E9A..00549EA1 (container item)
        if (as_creature(object)) {                                       // 00549EA3..00549EB0
            const U item_id = rd(item, 0x14);                            // 00549EB2
            at<U>(as_creature(object), 0xa38) = item_id;                 // 00549EB5..00549EB8
            at<U16>(as_creature(object), 0xa36) = 0;                     // 00549EBE..00549EC5
        } else if (as_placeable(object)) {                               // 00549ECE..00549ED3
            const U item_id = rd(item, 0x14);                            // 00549ED7
            at<U>(as_placeable(object), 0x374) = item_id;                // 00549EDC..00549EDF
            at<U16>(as_placeable(object), 0x372) = 0;                    // 00549EE5..00549EEC
        }
    }
    result = rd(item, 0x14);                                             // 00549EF3..00549EF6
    return push_object(result);                                          // 00549EFC..00549F1F
}

// =================================================================================================== 005355F0
// ActionEquipItem(object oItem, int nInventorySlot, int bInstant = FALSE): slot index in [0,18) is turned into
// the slot mask pow(2.0, n) through _CIpow + __ftol2, then the running creature queues AddEquipItemActions.
int __fastcall recovered_vm_action_equip_item_005355f0(void* self, void*, int, unsigned argc) {
    const U runner = rd(self, 8);                                        // 005355FD
    U item_id; I slot; I instant = 0;                                    // 00535605
    if (!pop_object(&item_id) || !pop_int(&slot)) return k_pop_failed;   // 00535600..00535628
    if (static_cast<I>(argc) >= 3 && !pop_int(&instant)) return k_pop_failed;   // 0053562A..00535643 (jl, signed)
    if (slot < 0 || slot >= 0x12) return 0;                              // 00535652..00535661
    const U mask = x87_pow2_ftol(slot);                                  // 00535667..0053567B
    if (rd(self, 4) != 1) return 0;                                      // 0053567F..00535683
    void* object = nullptr;
    if (!lookup(runner, &object)) return 0;                              // 00535685..005356A5
    void* creature = as_creature(object);                                // 005356A7..005356B0
    if (!creature) return 0;                                             // 005356B2..005356B4
    void* item_object = nullptr;
    if (!lookup(item_id, &item_object)) return 0;                        // 005356B6..005356DB
    void* item = as_item(item_object);                                   // 005356DD..005356E3
    if (!item) return 0;                                                 // 005356E6..005356E8
    recovered_object_equip_004f0420(creature, nullptr, item, mask, 0, 0, static_cast<U>(instant), 0, 0);  // 005356EA..005356FF
    return 0;                                                            // 00535704..0053570B
}

// =================================================================================================== 00545050
// ActionUnequipItem(object oItem, int bInstant = FALSE). Quirk kept: neither AsSWSItem() nor AsSWSCreature()
// is null-checked before AddUnequipActions (AsSWSItem is evaluated first, as in the original).
int __fastcall recovered_vm_action_unequip_item_00545050(void* self, void*, int, unsigned argc) {
    U item_id; I instant = 0;                                            // 00545061
    if (!pop_object(&item_id)) return k_pop_failed;                      // 00545054..00545070
    if (static_cast<I>(argc) >= 2 && !pop_int(&instant)) return k_pop_failed;   // 00545072..0054508B
    if (rd(self, 4) != 1) return 0;                                      // 00545099..0054509D
    void* runner = nullptr;
    if (!lookup(rd(self, 8), &runner)) return 0;                         // 0054509F..005450C3
    void* item_object = nullptr;
    if (!lookup(item_id, &item_object)) return 0;                        // 005450C5..005450E9
    void* item = as_item(item_object);                                   // 005450EB..00545101
    void* creature = as_creature(runner);                                // 00545104..0054510B
    recovered_object_unequip_004f06d0(creature, nullptr, item, k_invalid, 0, 0, static_cast<U>(instant));  // 0054510E..00545110
    return 0;                                                            // 00545115..0054511B
}

// =================================================================================================== 0052C7B0
// ActionEquipMostDamagingMelee(object oVersus = OBJECT_INVALID, int bOffHand = FALSE) on the running creature.
int __fastcall recovered_vm_action_equip_most_damaging_melee_0052c7b0(void* self, void*, int, unsigned) {
    U versus; I offhand;
    if (!pop_object(&versus) || !pop_int(&offhand)) return k_pop_failed; // 0052C7B4..0052C7E7
    void* creature = creature_by_id(rd(self, 8));                        // 0052C7EA..0052C7F7
    if (creature) kotor_fastcall_00500e50(creature, nullptr, versus, static_cast<U>(offhand), 0);   // 0052C7FC..0052C80E
    return 0;                                                            // 0052C813..0052C819
}

// =================================================================================================== 0052C820
// ActionEquipMostDamagingRanged(object oVersus = OBJECT_INVALID); falls back to the melee helper (offhand 0)
// when no ranged weapon was chosen.
int __fastcall recovered_vm_action_equip_most_damaging_ranged_0052c820(void* self, void*, int, unsigned) {
    U versus;
    if (!pop_object(&versus)) return k_pop_failed;                       // 0052C821..0052C83F
    void* creature = creature_by_id(rd(self, 8));                        // 0052C842..0052C854
    if (creature && kotor_fastcall_00501250(creature, nullptr, versus) == 0)       // 0052C856..0052C868
        kotor_fastcall_00500e50(creature, nullptr, versus, 0, 0);        // 0052C86A..0052C873
    return 0;                                                            // 0052C878..0052C87C
}

// =================================================================================================== callees
// 00500E50 CSWSCreature::EquipMostDamagingMeleeWeapon(object versus, int offhand, int recursed): RET 0Ch, EAX 0/1.
// Locals as the original frame: L08 current weapon (re-equipped at the end), L0C chosen slot, L10 versus
// AsSWSObject, L14 slot copy passed to CanEquipItem, L18 versus creature, L1C best damage, L20 loop index,
// L24 best item. GetDamageRoll / CalculateMaxElementalDamage are evaluated once before the loop with their
// results discarded (original).
U __fastcall kotor_fastcall_00500e50(void* self, void*, U versus, U offhand, U recursed) {
    void* l08 = nullptr; U l0c = 0; void* l10 = nullptr; U l14 = 0; void* l18 = nullptr;  // 00500E5B..00500E5F
    I l1c = 0; U l20 = 0; void* l24 = nullptr;
    if (!repository(self)) return 0;                                     // 00500E57..00500E71 (EAX = 0)
    void* target = game_object(versus);                                  // 00500E74..00500E88
    if (target && as_object(target)) {                                   // 00500E8A..00500E97
        l10 = as_object(target);                                         // 00500E99..00500EA4
        if (as_creature(l10)) l18 = as_creature(l10);                    // 00500EA0..00500EB8
    }
    if (offhand == 0) {                                                  // 00500EBC..00500EC2
        const U type = kotor_fastcall_004d3da0(rp(self, 0x9c8), nullptr);  // 00500EC4..00500ECA
        if (type == 3) l0c = 0x4000;                                     // 00500ECF..00500ED4
        else if (type == 4) l0c = 0x8000;                                // 00500EDE..00500EE3
        else l0c = type == 5 ? 0x10000u : 0x10u;                         // 00500EED..00500EFF (setne/dec/and/add)
    } else {
        l0c = 0x20;                                                      // 00500F05
    }
    const U slot = l0c;                                                  // 00500F0D (edi)
    void* current = recovered_equipment_item_005a4c20(rp(self, 0xa2c), nullptr, slot);   // 00500F11..00500F18
    l08 = current;                                                       // 00500F1F
    if (current) {                                                       // 00500F1D..00500F23
        if (rd8(base_item(rd(current, 0xc)), 0x1a) != 0) {               // 00500F29..00500F40 (jbe)
            recovered_creature_unequip_item_004faa70(self, nullptr, l08);   // 00500F42..00500F49
            recovered_inventory_insert_0055d330(repository(self), nullptr, &l08, 0, 0, 1);  // 00500F4E..00500F62
            l08 = nullptr;                                               // 00500F67..00500F6C
            if (slot == 0x10) {                                          // 00500F69..00500F70
                void* second = recovered_equipment_item_005a4c20(rp(self, 0xa2c), nullptr, 0x20);  // 00500F72..00500F7A
                void* l24_slot = second;                                 // 00500F81 ([esp+2C] -> L24)
                if (second) {
                    recovered_creature_unequip_item_004faa70(self, nullptr, second);   // 00500F87..00500F8A
                    recovered_inventory_insert_0055d330(repository(self), nullptr, &l24_slot, 0, 0, 1);  // 00500F8F..00500FA3
                }
                l24 = l24_slot;
            }
        }
    }
    void* chosen = l08;                                                  // 00500FA8 / 00500FAC (eax)
    if (offhand == 1) {                                                  // 00500FAC..00500FAF
        if (chosen == nullptr) {                                         // 00500FB1..00500FB3
            void* main = recovered_equipment_item_005a4c20(rp(self, 0xa2c), nullptr, 0x10);   // 00500FB5..00500FBD
            if (main && rd8(base_item(rd(main, 0xc)), 8) == 3) {         // 00500FC2..00500FDD
                chosen = main; l08 = main; l0c = 0x10;                   // 00500FDF..00500FE5
            } else {
                chosen = l08;                                            // 00500FEF
            }
        }
    }
    if (chosen) {                                                        // 00500FF3..00500FF5
        recovered_creature_unequip_item_004faa70(self, nullptr, chosen); // 00500FF7..00500FFA
        recovered_inventory_insert_0055d330(repository(self), nullptr, &l08, 0, 0, 1);  // 00500FFF..00501013
    }
    l24 = nullptr;                                                       // 0050102C
    (void)kotor_fastcall_005a9050(rp(self, 0xa74), nullptr, l10, 0, 0, 0, 1, 1);   // 00501018..00501030
    (void)kotor_fastcall_005b5640(self, nullptr, l18, 0);                // 00501035..0050103D
    l1c = 0; l20 = 0;                                                    // 00501046..0050104A
    void* best = nullptr;                                                // ebx (0 when the repository is empty)
    if (rd(repository(self), 0x10) != 0) {                               // 00501042..00501058 (jbe)
        do {
            void* item = w2x_inventory_item_005560b0(repository(self), nullptr, l20);   // 00501060..0050106C
            if (!item) goto next;                                        // 00501071..00501075
            {
                void* base = base_item(rd(item, 0xc));                   // 0050107B..0050108D
                if (!base || rd8(base, 9) == 0) goto next;               // 0050108F..0050109C
                l14 = l0c;                                               // 005010A2..005010AC
                I diff = static_cast<I>(rd8(base_item(rd(item, 0xc)), 0x1b)) - static_cast<I>(rd(self, 0x4f8));   // 005010A6..005010C5
                const I fit = (diff < -2 || diff > 1) ? -10 : diff;      // 005010C7..005010D3 (signed)
                const unsigned char can = recovered_item_can_equip_0051aa60(self, nullptr, item, &l14, 0, 0, 1);   // 005010D8..00501104
                if (offhand == 0 ? can == 0 : can != 1) goto next;       // 005010F3 / 00501109
                if (rd8(base, 0x1a) != 0) goto next;                     // 0050110D..00501112 (ja)
                if (rd8(base, 9) == 0) goto next;                        // 00501114..00501119
                if (offhand != 0 && (fit >= 1 || fit == -10)) goto next; // 0050111B..0050112B
                recovered_creature_equip_004feb90(self, nullptr, l14, item, 1, 0, 0);   // 0050112D..0050113B
                I damage = kotor_fastcall_005a9050(rp(self, 0xa74), nullptr, l10, offhand, 0, 0, 1, 1);   // 00501140..00501159
                damage += kotor_fastcall_005b5640(self, nullptr, l18, offhand);   // 0050115B..00501168
                if (damage >= l1c) { l1c = damage; l24 = item; }         // 0050116A..00501174 (jl, signed)
                recovered_creature_unequip_item_004faa70(self, nullptr, item);   // 00501178..0050117B
            }
        next:
            ++l20;                                                       // 00501180..00501189
        } while (l20 < rd(repository(self), 0x10));                      // 0050118D..00501195 (jb)
        best = l24;                                                      // 0050119B
    }
    if (l08) {                                                           // 005011A3..005011A9
        recovered_remove_item_00555fd0(repository(self), nullptr, l08);  // 005011AB..005011B7
        recovered_creature_equip_004feb90(self, nullptr, l0c, l08, 1, 0, 0);   // 005011BC..005011CE
    }
    if (!best) {                                                         // 005011D7..005011D9
        if (l08 && versus != k_invalid && recursed == 0) {               // 005011DB..005011EF
            kotor_fastcall_00500e50(self, nullptr, k_invalid, offhand, 1);   // 005011F1..005011FB
            return 1;                                                    // 00501200..0050120C
        }
        return 1;                                                        // 00501232..0050123E
    }
    if (l08 && rd(l08, 0x14) == rd(best, 0x14)) return 1;                // 0050120F..00501219
    recovered_object_equip_004f0420(self, nullptr, best, l0c, 0, 0, 0, 0, 0);   // 0050121B..0050122D
    return 1;                                                            // 00501232..0050123E
}

// 00501250 CSWSCreature::EquipMostDamagingRangedWeapon(object versus): RET 4, EAX 0/1.
// Locals: L08 current main-hand weapon, L0C versus creature, L10 best attack modifier (starts -30), L14 best item.
// Quirk kept: the argument slot is reused as scratch (second weapon pointer for AddItem, then the CanEquipItem
// slot set to 0x10 before every call).
U __fastcall kotor_fastcall_00501250(void* self, void*, U versus) {
    void* l08 = nullptr; void* l0c = nullptr; I l10 = -30; void* l14 = nullptr;   // 0050125B..0050126B
    if (!repository(self)) return 0;                                     // 00501257..00501276 -> 0050148A
    void* target = game_object(versus);                                  // 0050127C..0050128F
    if (target && as_object(target)) {                                   // 00501291..0050129E
        void* object = as_object(target);                                // 005012A0..005012A7
        if (as_creature(object)) l0c = as_creature(object);              // 005012A9..005012BB
    }
    U arg_slot = versus;                                                 // [esp+20] reused below
    l08 = recovered_equipment_item_005a4c20(rp(self, 0xa2c), nullptr, 0x10);   // 005012BF..005012CE
    if (l08) {                                                           // 005012CC..005012D2
        void* base = base_item(rd(l08, 0xc));                            // 005012D8..005012E4
        if (base && rd8(base, 0x1a) != 0) {                              // 005012E9..005012F2 (jbe)
            l14 = l08;                                                   // 005012F4..005012FC
            l10 = kotor_fastcall_005a7ea0(rp(self, 0xa74), nullptr, l0c);   // 005012F8..0050130C
        }
        if (l08) {                                                       // 00501310..00501316
            recovered_creature_unequip_item_004faa70(self, nullptr, l08);   // 00501318..0050131B
            recovered_inventory_insert_0055d330(repository(self), nullptr, &l08, 0, 0, 1);   // 00501320..00501334
            void* second = recovered_equipment_item_005a4c20(rp(self, 0xa2c), nullptr, 0x20);   // 00501339..00501341
            arg_slot = word(second);                                     // 00501348
            if (second) {                                                // 00501346..0050134C
                recovered_creature_unequip_item_004faa70(self, nullptr, second);   // 0050134E..00501351
                void* scratch = ptr(arg_slot);
                recovered_inventory_insert_0055d330(repository(self), nullptr, &scratch, 0, 0, 1);   // 00501356..0050136A
                arg_slot = word(scratch);
            }
        }
    }
    if (rd(repository(self), 0x10) != 0) {                               // 0050136F..0050137B (jbe)
        U index = 0;                                                     // ebp
        do {
            void* item = w2x_inventory_item_005560b0(repository(self), nullptr, index);   // 00501382..0050138E
            if (item) {                                                  // 00501393..00501397
                void* base = base_item(rd(item, 0xc));                   // 00501399..005013AB
                if (base && rd8(base, 9) != 0) {                         // 005013AD..005013B6
                    arg_slot = 0x10;                                     // 005013C6
                    if (recovered_item_can_equip_0051aa60(self, nullptr, item, &arg_slot, 0, 0, 1) != 0 &&   // 005013B8..005013D5
                        rd8(base, 0x1a) != 0) {                          // 005013D7..005013DC (jbe)
                        recovered_creature_equip_004feb90(self, nullptr, arg_slot, item, 1, 0, 0);   // 005013DE..005013EC
                        const I modifier = kotor_fastcall_005a7ea0(rp(self, 0xa74), nullptr, l0c);   // 005013F1..005013FC
                        if (modifier > l10) { l10 = modifier; l14 = item; }   // 00501401..0050140B (jle, signed)
                        recovered_creature_unequip_item_004faa70(self, nullptr, item);   // 0050140F..00501412
                    }
                }
            }
            ++index;                                                     // 00501417..0050141B
        } while (index < rd(repository(self), 0x10));                    // 0050141C..00501424 (jb)
    }
    if (l08) {                                                           // 0050142B..00501432
        recovered_remove_item_00555fd0(repository(self), nullptr, l08);  // 00501434..00501440
        recovered_creature_equip_004feb90(self, nullptr, 0x10, l08, 1, 0, 0);   // 00501445..00501454
    }
    if (!l14 || l08 == l14) return 0;                                    // 0050145D..00501467 -> 0050148A
    recovered_object_equip_004f0420(self, nullptr, l14, 0x10, 0, 0, 0, 0, 0);   // 00501469..00501478
    return 1;                                                            // 0050147D..00501487
}

}  // extern "C"
