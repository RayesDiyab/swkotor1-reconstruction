#pragma once
// W2 tier 4 (fork C): CSWSMessage::WriteGameObjUpdate_UpdateAppearance 00572070 and the small unowned
// callees it needs: AddItemAppearanceToMessage 00567090, WriteGameObjUpdate_WriteInventorySlotAdd 00567710,
// AddActiveItemPropertiesToMessage 00566F20, CSWSItem::GetCost 00554000, CSWSCreature::CanUseItem 0051B440 and its
// restriction checks 005145A0 / 00518210 / 005182F0 / 00514650, GetMinEquipLevel 00554080, CheckProficiencies 00510E30,
// GetPassiveProperty 00553990, GetFeatRemainingUses 005A6680, GetSimpleAlignmentGoodEvil 005A5110, GetRequiredFeat 005B30C0, CSWSItem::GetActiveProperty 00553960,
// CSWSCreature::GetLastInstantEquip 004EFDD0 / SetLastInstantEquip 004EFE00.
// Logic over original 32-bit address words. Message primitives are the W2 writers (w2_message.hpp) and the
// existing module_message DWORD writer (kotor::server_mainloop::message_word_004d6340).
#include "58eca878c7-w2_entry.hpp"
namespace w2::g07 {
using w2::msg::Word;
using w2::msg::Memory;

struct AppearancePorts {
    virtual ~AppearancePorts() = default;
    // CSWInventory::GetItemInSlot(slot) RET 4 -> item* or 0. Provider @recovered_equipment_item_005a4c20@12.
    virtual Word item_in_slot_005a4c20(Word inventory, Word slot) = 0;
    // CServerExoApp::GetServerInfo (0051B4C1 call). W1 tier6 @recovered_server_info_004aec90@8.
    virtual Word server_info_004aec90(Word server) = 0;
    // CSWBaseItemArray::GetBaseItem(type) RET 4. Provider @kotor_m15b_base_item_array_get_base_item_005b31a0@12.
    virtual Word base_item_005b31a0(Word array, Word type) = 0;
    // [007A3A28] (rules); +34h is the CSWBaseItemArray*.
    virtual Word rules_007a3a28() = 0;
    // CSWItem::GetBaseItem() thiscall RET 0 -> base item row or 0. Provider @kotor_m15b_item_get_base_item_005b4790@8.
    virtual Word item_base_item_005b4790(Word item) = 0;
    // CSWSCreatureStats::GetLevel(flag) RET 4 -> AL. Provider @recovered_stats_level_005a5fd0@12.
    virtual Word stats_level_005a5fd0(Word stats, Word flag) = 0;
    // CSWSItem::GetPropertyByTypeExists(type, subtype) RET 8. Provider @recovered_item_property_has_00555c90@16.
    virtual Word item_property_exists_00555c90(Word item, Word type, Word subtype) = 0;
    // CSWSItem::GetPropertyByType(CSWItemProperty** out, type, subtype) RET 0Ch. Provider @recovered_item_property_query_005539c0@20.
    virtual Word item_property_by_type_005539c0(Word item, Word out, Word type, Word subtype) = 0;
    // CSWSCreatureStats::HasFeat(WORD feat) RET 4. Provider @recovered_stats_has_feat_005a6630@12.
    virtual Word stats_has_feat_005a6630(Word stats, Word feat) = 0;
    // CSWRules::GetFeat(WORD feat) thiscall RET 4 on [007A3A28]. Provider kotor_rules::kotor_fastcall_00550c00 (1122, C++ fastcall).
    virtual Word rules_feat_00550c00(Word rules, Word feat) = 0;
    // CSWSCreatureStats::GetClass(BYTE index) RET 4 -> AL. Provider kotor::server_mainloop::creature_get_class_005a4e90 (0982 TU).
    virtual Word stats_class_005a4e90(Word stats, Word index) = 0;
    // CExoArrayList<int>::Add(value) RET 4. Provider @gui_load_array_add_00671c00@12.
    virtual void list_add_00671c00(Word list, Word value) = 0;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
};

// 004EFDD0 GetLastInstantEquip(slot) RET 4: slot 10h -> [c+AACh], 20h -> [c+AA8h], else 0.
Word last_instant_equip_004efdd0(Memory&, Word creature, Word slot);
// 004EFE00 SetLastInstantEquip(slot, value) RET 8: slot 10h -> [c+AACh], 20h -> [c+AA8h], else no store.
void set_last_instant_equip_004efe00(Memory&, Word creature, Word slot, Word value);
// 006FAE8C __ftol2 of an x87 value: truncating 64-bit conversion, low dword (indefinite -> 0).
Word ftol2_low_006fae8c(double value);
// 00554000 CSWSItem::GetCost() RET 0: 0 when [item+108h] == 1; else ftol(base+C4h (float) * (int32)[item+260h]), min 1 (unsigned).
Word item_cost_00554000(Memory&, AppearancePorts&, Word item);
// 0051B440 CSWSCreature::CanUseItem(item, unused) RET 8. scratch: 0x10 bytes.
Word can_use_item_0051b440(Memory&, w2::entry::Ports&, AppearancePorts&, Word creature, Word item, Word unused, Word scratch);
// 00553990 CSWSItem::GetPassiveProperty(index) RET 4: [item+254h] + index*1Ch when 0 <= index < [item+24Ch], else 0.
Word passive_property_00553990(Memory&, Word item, Word index);
// 005A5110 CSWSCreatureStats::GetSimpleAlignmentGoodEvil() -> AL: (int16)[s+17Eh] <= 40 -> 3; >= 60 -> 2; else 1.
Word simple_alignment_good_evil_005a5110(Memory&, Word stats);
// 005B30C0 CSWBaseItem::GetRequiredFeat(BYTE index) RET 4 -> AX: index < [b+80h] ? [[b+7Ch] + 2*index] : FFFFh.
Word required_feat_005b30c0(Memory&, Word base, Word index);
// CSWSCreature item restriction checks, thiscall (item) RET 4.
Word check_item_alignment_005145a0(Memory&, AppearancePorts&, Word creature, Word item);
Word check_item_class_00518210(Memory&, AppearancePorts&, Word creature, Word item);
// scratch: 4 bytes (the original reuses its item argument slot as the GetPropertyByType out pointer).
Word check_item_race_005182f0(Memory&, AppearancePorts&, Word creature, Word item, Word scratch);
Word check_item_feat_00514650(Memory&, AppearancePorts&, Word creature, Word item);
// 00554080 CSWSItem::GetMinEquipLevel() RET 0 -> AL. scratch: 0x0C bytes (+0 CExoString, +8 GetINTEntry out:
// not initialised, like the original's stack slot).
Word item_min_equip_level_00554080(Memory&, w2::entry::Ports&, AppearancePorts&, Word item, Word scratch);
// 005A6680 CSWSCreatureStats::GetFeatRemainingUses(WORD feat) RET 4 -> EAX (callers use AL).
Word feat_remaining_uses_005a6680(Memory&, AppearancePorts&, Word stats, Word feat);
// 00510E30 CSWSCreature::CheckProficiencies(item, unused mask) RET 8.
Word check_proficiencies_00510e30(Memory&, AppearancePorts&, Word creature, Word item, Word unused);
// 00553960 CSWSItem::GetActiveProperty(index) RET 4: [item+250h] + index*1Ch when 0 <= index < [item+248h], else 0.
Word active_property_00553960(Memory&, Word item, Word index);
// 00566F20 AddActiveItemPropertiesToMessage(item, creature) RET 8. scratch: 0x14 bytes (loc-string writer).
void active_item_properties_00566f20(Memory&, w2::entry::Ports&, AppearancePorts&, Word self, Word item, Word creature, Word scratch);
// 00567090 AddItemAppearanceToMessage(item) RET 4.
void item_appearance_00567090(Memory&, w2::entry::Ports&, AppearancePorts&, Word self, Word item);
// 00567710 WriteGameObjUpdate_WriteInventorySlotAdd(creature, item, slot) RET 0Ch.
// scratch: 0x14 bytes.
void inventory_slot_add_00567710(Memory&, w2::entry::Ports&, AppearancePorts&, Word self, Word creature, Word item, Word slot, Word scratch);
// 00572070 WriteGameObjUpdate_UpdateAppearance(object, last_update, flags) RET 0Ch.
// scratch >= 0x20: +00 CExoArrayList<int> {data,count,capacity} (original ESP+10h), +0Ch loc-string scratch (0x14).
void update_appearance_00572070(Memory&, w2::entry::Ports&, AppearancePorts&, Word self, Word object, Word last, Word flags, Word scratch);
}
