// lane-w3 unit w3-client-appearance: the client creature appearance message (GameObjectUpdate 'P' / creature) and the
// creature / item leaves it reaches.
//   00655E40 CSWCMessage::HandleServerToPlayerCreatureUpdate_Appearance
//   0064E730 HandleServerToPlayerItemAppearance   0064E3D0 HandleServerToPlayerActiveItemProperties
//   0064B540 CSWCMessage::CreateItem   005B3050 CSWBaseItem::GetIconResRef   005B30E0 CSWBaseItem::GetModelResRef
//   00646660 CSWCItem::SetNumActiveProperties   006466A0 GetActiveProperty   00646810 SetStackSize
//   006469B0 CSWCItem::SetItemPropertyUsesLeft
//   0060C910 CSWCCreature::RequestAppearance   0060D540 GetArmourClass   0060D510 DeleteLoopingCreatureSound
//   0060D460 SetNewLoopingCreatureSound   0060CD50 SetLastInstant   0060CBD0 SetEquippedItemID
//   0060D020 SetItemUnEquippedByMe   00613BA0 EquipToInventorySlot   00613850 UpdateVisibleWeapons
//   005ED640 CClientExoApp / 005EE8F0 CClientExoAppInternal::GetPauseState
// Reconstructed in w3_client_player_model.cpp: 00646230 CSWCItem::CSWCItem (+ tables 00751BA0/00751A58) and
// 00698A40 / 00696F60 CSWCCreatureAppearance::RequestBody/RequestHeadAppearance. Bound to lane-w4
// (coordinator rulings t2-3/t2-5): 00610A70 PlayDrawAnimation -> kotor_fastcall_00610a70 (which itself calls
// lane-w4 kotor_fastcall_0063c760 InsertFireForgetAnimation); [007A2484] -> kotor_cell_007a2484.
//
// Exception unwinding of the original frames (CExoString temporaries) is not reproduced, as in the other lane-w3
// units: every temporary is destroyed explicitly on each normal path.
#include "8fdb3c6e33-w3_client_appearance.h"

using namespace kotor_w3;

namespace {
constexpr U kInvalidObject_00746644 = 0x7F000000u;                      // .rdata 00746644
constexpr U8 kGameObjectOk_0074666c = 0;                                // .rdata 0074666C
const char kSoundAppType_00751ea8[] = "soundapptype";                   // .rdata 00751EA8
const char kLooping_00747798[] = "Looping";                             // .rdata 00747798
const char kDefault_0073df6c[] = "default";                             // .rdata 0073DF6C
const char kLoading_0074f4cc[] = "Loading";                             // .rdata 0074F4CC
const char kIconGender_0074b000[] = "i%s_%c_%03d";                      // .rdata 0074B000
const char kIcon_0074aff4[] = "i%s_%03d";                               // .rdata 0074AFF4
const char kModelGender_0074b014[] = "%s_%c_%03d";                      // .rdata 0074B014
const char kModel_0074b00c[] = "%s_%03d";                               // .rdata 0074B00C
const char kIconMissing_00751ca4[] = "Icon %s nor the default icon %s could be loaded.";     // .rdata 00751CA4
const char kModelMissing_00751c70[] = "Model %s nor the default model %s could be loaded.";  // .rdata 00751C70
constexpr U kResTga_3 = 3, kResTpc_bbf = 0xBBF, kResMdl_7d2 = 0x7D2;    // resource type immediates

// CSWCCreatureAppearance request block (3Ch bytes): the original copies it with rep movsd ecx=0Fh.
constexpr unsigned kAppearanceSize = 0x3C;
using SlotU = U (__fastcall*)(void*, void*);
using SlotU1 = U (__fastcall*)(void*, void*, U);
using SlotU2 = U (__fastcall*)(void*, void*, U, U);
using SlotU3 = U (__fastcall*)(void*, void*, U, U, U);

void* rules() noexcept { return kotor_cell_rules_007a3a28; }                        // [007A3A28]
void* base_item_array() noexcept { return rp(rules(), 0x34); }                    // [[007A3A28]+34h]
menu_resources::Manager* resman() noexcept {
    return static_cast<menu_resources::Manager*>(word_ptr(frame_control::resource_manager_007a39e8));   // [007A39E8]
}
U exists(ResRef* ref, U type) noexcept(false) {
    return resman()->exists_00408bc0(reinterpret_cast<menu_resources::Ref*>(ref), type, nullptr);
}
void make_empty_string(ExoString* s) noexcept(false) {
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(s), nullptr);
}
// GetModel-style slot +98h(part, flag) on a creature or on an item's CSWCObject sub-object.
U model_of(void* object, U part, U flag) noexcept(false) { return vslot<SlotU2>(object, 0x98)(object, nullptr, part, flag); }
}  // namespace

extern "C" {
// ============================================================================================ client app leaves
// 005EE8F0 CClientExoAppInternal::GetPauseState(BYTE mask): BYTE [this+1CCh] & mask. RET 4.
U __fastcall recovered_client_internal_get_pause_state_005ee8f0(void* internal, void*, U mask) noexcept {
    return static_cast<U>(ld<U8>(internal, 0x1CC)) & static_cast<U>(static_cast<U8>(mask));   // 005EE8F0..005EE8FC
}
// 005ED640 CClientExoApp::GetPauseState(BYTE): tail jump on [this+4]. RET 4.
U __fastcall recovered_client_get_pause_state_005ed640(void* app, void*, U mask) noexcept {
    return recovered_client_internal_get_pause_state_005ee8f0(rp(app, 4), nullptr, mask);   // 005ED640..005ED643
}

// ============================================================================================ creature leaves
// 0060D540 CSWCCreature::GetArmourClass: [this+21Ch] ? return_zero(0063E7F0) : 0.
U __fastcall recovered_client_creature_get_armour_class_0060d540(void* self, void*) noexcept(false) {
    void* const appearance = rp(self, 0x21C);                           // 0060D540
    if (appearance == nullptr) return 0;                                // 0060D546..0060D551
    return slot_return_zero_0063e7f0_a0(appearance, nullptr);           // 0060D54A jmp return_zero
}

// 0060C910 CSWCCreature::RequestAppearance(CSWCCreatureAppearance const* request, BYTE mask) RET 8 -> 1.
U __fastcall recovered_client_creature_request_appearance_0060c910(void* self, void*, const void* request,
                                                                    U mask_word) noexcept(false) {
    const U8 mask = static_cast<U8>(mask_word);                         // 0060C911 bl
    if ((mask & 1) != 0) recovered_client_appearance_request_body_00698a40(rp(self, 0x21C), nullptr, request);   // 0060C915..0060C929
    if ((mask & 2) != 0) recovered_client_appearance_request_head_00696f60(rp(self, 0x21C), nullptr, request);   // 0060C92E..0060C93A
    if ((mask & 4) != 0)                                                // 0060C93F..0060C94B return_true_4
        sound_volume29_required::predicate_006054e0(rp(self, 0x21C), nullptr, const_cast<void*>(request));
    if ((mask & 8) != 0)                                                // 0060C950..0060C95C return_true_4
        sound_volume29_required::predicate_006054e0(rp(self, 0x21C), nullptr, const_cast<void*>(request));
    std::memcpy(at(self, 0x31C), request, kAppearanceSize);             // 0060C961..0060C96C rep movsd (0Fh dwords)
    return 1;                                                           // 0060C970
}

// 0060D510 CSWCCreature::DeleteLoopingCreatureSound: Stop, delete and clear [this+17Ch].
void __fastcall recovered_client_creature_delete_looping_sound_0060d510(void* self, void*) noexcept(false) {
    kotor_m15_sound_source_stop_005d5a20(rp(self, 0x17C), nullptr);     // 0060D513..0060D519
    void* const source = rp(self, 0x17C);                               // 0060D51E
    if (source != nullptr) delete_object(source);                       // 0060D524..0060D52C [vtbl+0](1)
    st<void*>(self, 0x17C, nullptr);                                    // 0060D52E
}

// 0060D460 CSWCCreature::SetNewLoopingCreatureSound(CExoString const& name) RET 4.
void __fastcall recovered_client_creature_set_new_looping_sound_0060d460(void* self, void*, const void* name) noexcept(false) {
    void* const memory = live_operator_new(8);                          // 0060D47A..0060D47E
    void* source = nullptr;                                             // 0060D48C
    if (memory != nullptr) {                                            // 0060D48E..0060D494
        ResRef ref;                                                     // {E-0x1c}
        async_ref_construct_string_00406d60(&ref, nullptr, name);       // 0060D496..0060D49F
        source = kotor_sound_rev3::SourceConstruct005d60e0(memory, nullptr, &ref);   // 0060D4A4..0060D4AB
    }
    st<void*>(self, 0x17C, source);                                     // 0060D4B0
    void* const player = client_get_player_creature_005ed540(client_app(), nullptr);   // 0060D4B6..0060D4C7
    kotor_sound_rev3::SourceSetPriorityGroup005d5900(rp(self, 0x17C), nullptr, player == self ? 6u : 7u);   // 0060D4CC..0060D4DC
    kotor_m15_sound_source_set_looping_005d59d0(rp(self, 0x17C), nullptr, 1);   // 0060D4E1..0060D4E9
}

namespace {
// 0060CD50 / 0060CBD0 share one slot decoder (byte table + jump table for 1..40h, compare chain above 80h).
// Returns the field offset relative to `base` (0 when the slot has no field: the original just returns).
unsigned slot_field(U slot, unsigned base) noexcept {
    switch (slot) {
    case 0x1: return base + 0x00;      case 0x2: return base + 0x04;      case 0x4: return base + 0x08;
    case 0x8: return base + 0x0C;      case 0x10: return base + 0x10;     case 0x20: return base + 0x14;
    case 0x40: return base + 0x18;     case 0x80: return base + 0x1C;     case 0x100: return base + 0x20;
    case 0x200: return base + 0x24;    case 0x400: return base + 0x28;    case 0x4000: return base + 0x2C;
    case 0x8000: return base + 0x30;   case 0x10000: return base + 0x34;  case 0x20000: return base + 0x38;
    default: return 0;
    }
}
}  // namespace

// 0060CD50 CSWCCreature::SetLastInstant(DWORD slot, BOOL value) RET 8: [this+270h..2A8h] by slot bit.
void __fastcall recovered_client_creature_set_last_instant_0060cd50(void* self, void*, U slot, U value) noexcept {
    const unsigned field = slot_field(slot, 0x270);                     // 0060CD50..0060CE46 (table 0060CE70/0060CE90)
    if (field != 0) st<U>(self, field, value);                          // 0060CD79..0060CE66
}
// 0060CBD0 CSWCCreature::SetEquippedItemID(DWORD slot, OBJECT_ID id) RET 8: [this+220h..258h] by slot bit.
void __fastcall recovered_client_creature_set_equipped_item_id_0060cbd0(void* self, void*, U slot, U id) noexcept {
    const unsigned field = slot_field(slot, 0x220);                     // 0060CBD0..0060CCC6 (table 0060CCF0/0060CD10)
    if (field != 0) st<U>(self, field, id);                             // 0060CBF9..0060CCE6
}

// 0060D020 CSWCCreature::SetItemUnEquippedByMe(OBJECT_ID) RET 4.
void __fastcall recovered_client_creature_set_item_unequipped_by_me_0060d020(void* /*self*/, void*, U id) noexcept(false) {
    if (id == kInvalidObject_00746644) return;                          // 0060D020..0060D02C
    void* object = nullptr;                                             // {E+0x4} (the argument slot)
    void* const array = kotor_fastcall_005ed570(client_app(), nullptr);   // 0060D02E..0060D036
    if (kotor_fastcall_004d8230(array, nullptr, id, &object) != kGameObjectOk_0074666c) return;   // 0060D03B..0060D04E
    void* const item = vslot<void* (__fastcall*)(void*, void*)>(object, 0x34)(object, nullptr);   // 0060D050..0060D056 AsSWCItem
    kotor_m15a_item_set_equipped_by_00646600(item, nullptr, nullptr);   // 0060D059..0060D05D
}

// 00610A70 CSWCCreature::PlayDrawAnimation: single owner lane-w4 (kotor_fastcall_00610a70, creature/creature_ai.cpp;
// coordinator ruling t2-5, OVERLAP-W4.md: bodies identical). The lane-w3 copy was dropped.

// 00613850 CSWCCreature::UpdateVisibleWeapons(DWORD slot, CSWCItem* item, BOOL instant) RET 0Ch.
void __fastcall recovered_client_creature_update_visible_weapons_00613850(void* self, void*, U slot, void* item,
                                                                         U instant) noexcept(false) {
    U part;
    if (slot == 0x10) part = 0xF7;                                      // 00613854..0061385B / 00613921
    else if (slot == 0x20) part = 0xFB;                                 // 00613861..0061386A
    else goto event;
    if (instant != 0) goto event;                                       // 0061386F..00613875
    {
        void* const anim = gob_get_property_0x68_0063c070(self, nullptr);   // 0061387B..0061387D
        if (vslot<SlotU1>(anim, 0x50)(anim, nullptr, 0x2806) != 1) goto event;   // 00613882..00613891
        const U pending = rd32(self, 0x14C);                            // 00613896 ebx
        void* powered = nullptr;                                        // 006138A2 ebp
        void* const model = word_ptr(model_of(self, part, 1));          // 0061389D..006138A4 GetModel
        if (model != nullptr &&                                         // 006138AC..006138AE
            vslot<SlotU>(model, 0x78)(model, nullptr) == 6)             // 006138B0..006138BA
            powered = vslot<void* (__fastcall*)(void*, void*)>(model, 0x70)(model, nullptr);   // 006138BC..006138C3
        if (item != powered && pending == 0) {                          // 006138C5..006138CD
            (void)gob_get_property_0x68_0063c070(self, nullptr);        // 006138CF..006138D1 (result unused)
            st<U>(self, 0x390, 0);                                      // 006138E0
            st<U>(self, 0x398, 0);                                      // 006138E6
            if (ld<U8>(self, 0x90) != 0)                                // 006138D6..006138EC
                kotor_fastcall_00610a70(self, nullptr, 0x2806);   // 006138EE..006138F5
            if (powered != nullptr)                                     // 006138FA..006138FC
                kotor_m15a_item_power_item_00647130(powered, nullptr, 0, 0, 1, 0);   // 006138FE..00613908
        }
    }
event:
    kotor_m15a_switch_weapon_event_00610f40(0, nullptr, self);          // 0061390F..00613919
}

// 00613BA0 CSWCCreature::EquipToInventorySlot(DWORD slot, CSWCItem* item) RET 8.
void __fastcall recovered_client_creature_equip_to_inventory_slot_00613ba0(void* self, void*, U slot, void* item) noexcept(false) {
    void* model;                                                        // ebp
    U item_id;                                                          // ebx
    if (item != nullptr) {                                              // 00613BA4..00613BAC
        model = word_ptr(model_of(at(item, 0x10), 0xFF, 1));            // 00613BAE..00613BBB
        item_id = rd32(item, 0x14);                                     // 00613BC1
        kotor_m15a_item_set_equipped_by_00646600(item, nullptr, self);  // 00613BC4..00613BC9
    } else {
        model = nullptr;                                                // 00613BD0
        item_id = kInvalidObject_00746644;                              // 00613BD2
    }
    void* const previous = kotor_m15_client_get_item_by_id_005ed5a0(   // 00613BDB..00613BF5 kept in {E+0x4}
        client_app(), nullptr, kotor_m15a_creature_get_equipped_item_id_0060ca80(self, nullptr, slot));
    if (slot == 0x10 || slot == 0x20) {                                 // 00613BF2..00613C02 -> 00613CAB
        if (kotor_m15a_creature_get_equipped_item_id_0060ca80(self, nullptr, slot) != item_id) {   // 00613CAB..00613CB5
            const U other = slot == 0x10 ? 0x20u : 0x10u;               // 00613CB7..00613CBA
            bool release = true;
            if (kotor_m15a_creature_get_equipped_item_id_0060ca80(self, nullptr, other) != kInvalidObject_00746644 &&   // 00613CBC..00613CC9 / 00613CDC..00613CE9
                kotor_m15a_creature_get_equipped_item_id_0060ca80(self, nullptr, other) == item_id)   // 00613CCB..00613CD6 / 00613CEB..00613CF6
                release = false;                                        // the new item is leaving the other hand
            if (release)                                                // 00613CD8 / 00613CF8..00613D01
                recovered_client_creature_set_item_unequipped_by_me_0060d020(
                    self, nullptr, kotor_m15a_creature_get_equipped_item_id_0060ca80(self, nullptr, slot));
        }
        recovered_client_creature_set_equipped_item_id_0060cbd0(self, nullptr, slot, item_id);   // 00613D06..00613D0A
        kotor_m15a_creature_update_weapon_class_00613710(self, nullptr);   // 00613D0F
        const U instant = kotor_m15a_creature_get_last_instant_0060ced0(self, nullptr, slot);   // 00613D14..00613D17
        recovered_client_creature_update_visible_weapons_00613850(self, nullptr, slot, item, instant);   // 00613D1C..00613D23
        if (kotor_m15a_object_get_looping_animation_0063c100(self, nullptr) == 0x2711)   // 00613D28..00613D33
            vslot<SlotU3>(self, 0x9C)(self, nullptr, 0x2711, 0, 0);     // 00613D35..00613D42 SetAnimation
    } else if (slot == 2) {                                             // 00613C08..00613C0B
        if (model != nullptr) vslot<SlotU1>(model, 0x4C)(model, nullptr, 0);   // 00613C0D..00613C18
        if (kotor_m15a_creature_get_equipped_item_id_0060ca80(self, nullptr, 2) != item_id)   // 00613C1B..00613C26
            recovered_client_creature_set_item_unequipped_by_me_0060d020(
                self, nullptr, kotor_m15a_creature_get_equipped_item_id_0060ca80(self, nullptr, 2));   // 00613C28..00613C30
        recovered_client_creature_set_equipped_item_id_0060cbd0(self, nullptr, 2, item_id);   // 00613C35..00613C3A
    } else {
        if (kotor_m15a_creature_get_equipped_item_id_0060ca80(self, nullptr, slot) != item_id)   // 00613C4B..00613C53 / 00613C85..00613C8D
            recovered_client_creature_set_item_unequipped_by_me_0060d020(
                self, nullptr, kotor_m15a_creature_get_equipped_item_id_0060ca80(self, nullptr, slot));   // 00613C55..00613C5C / 00613C8F..00613C98
        recovered_client_creature_set_equipped_item_id_0060cbd0(self, nullptr, slot, item_id);   // 00613C61..00613C66 / 00613C9D..00613CA1
        if (slot == 1 && item != nullptr) st<U>(item, 0x164, rd32(self, 4));   // 00613C6B..00613C7A (head slot only)
    }
    if (previous != nullptr && previous != item) {                      // 00613D48..00613D56
        void* const object = at(previous, 0x10);                        // 00613D58..00613D5B
        if (model_of(object, 0xFF, 1) != 0)                             // 00613D5E..00613D6F
            vslot<SlotU>(object, 0x80)(object, nullptr);                // 00613D71..00613D75 UnloadModel
    }
    if (item != nullptr)                                                // 00613D7B..00613D7D
        vslot<SlotU1>(at(item, 0x10), 0x84)(at(item, 0x10), nullptr, 0);   // 00613D7F..00613D87 SetArea(0)
}

// ============================================================================================ item leaves
// 00646660 CSWCItem::SetNumActiveProperties(int count) RET 4: reallocate the 1Ch-byte property array [+124h].
void __fastcall recovered_client_item_set_num_active_properties_00646660(void* item, void*, U count) noexcept(false) {
    st<U>(item, 0x120, count);                                          // 00646667
    void* const old = rp(item, 0x124);                                  // 0064666D
    if (old != nullptr) live_free(old);                                 // 00646673..00646678
    st<void*>(item, 0x124, live_operator_new(rd32(item, 0x120) * 0x1C));   // 00646680..00646692
}
// 006466A0 CSWCItem::GetActiveProperty(int index) RET 4 -> &props[index] or 0 (signed bounds).
void* __fastcall recovered_client_item_get_active_property_006466a0(void* item, void*, I32 index) noexcept {
    if (index < 0 || index >= ld<I32>(item, 0x120)) return nullptr;     // 006466A4..006466AE / 006466BE
    return word_ptr(rd32(item, 0x124) + static_cast<U>(index) * 0x1C);  // 006466B0..006466B9
}
// 00646810 CSWCItem::SetStackSize(int) RET 4: [+184h] = size; tail call GetBaseItem([+0Ch]) (EAX passes through).
void* __fastcall recovered_client_item_set_stack_size_00646810(void* item, void*, U size) noexcept(false) {
    st<U>(item, 0x184, size);                                           // 00646814
    return kotor_m15b_base_item_array_get_base_item_005b31a0(base_item_array(), nullptr, rd32(item, 0xC));   // 0064681A..0064682A
}
// 006469B0 CSWCItem::SetItemPropertyUsesLeft(int index, WORD uses) RET 8.
void __fastcall recovered_client_item_set_property_uses_left_006469b0(void* item, void*, I32 index, U uses_word) noexcept {
    if (index >= 8) return;                                             // 006469B4..006469B7 (signed)
    const U16 uses = static_cast<U16>(uses_word);                       // 006469B9
    st<U16>(item, 0x16C + static_cast<unsigned>(index) * 2, uses);      // 006469BF
    if (index != static_cast<I32>(ld<U8>(item, 0x17C)) || uses != 0) return;   // 006469C7..006469D5
    I32 j = 0;                                                          // 006469D8 ebx
    for (; j < 8; ++j) {                                                // 006469E2..00646A13 (edx = j*1Ch, esi = &uses[j])
        if (j >= ld<I32>(item, 0x120)) continue;                        // 006469E6..006469EC
        const U prop = rd32(item, 0x124) + static_cast<U>(j) * 0x1C;   // 006469EE..006469F4 (address arithmetic)
        if (prop == 0) continue;                                        // 006469F6..006469F8
        if (ld<U16>(word_ptr(prop), 0) != 0xA) continue;                // 006469FA..006469FE
        if (ld<U16>(item, 0x16C + static_cast<unsigned>(j) * 2) != 0) { // 00646A00..00646A04 (unsigned >0)
            st<U8>(item, 0x17C, static_cast<U8>(j));                    // 00646A17
            break;
        }
    }
    if (j == 8) st<U8>(item, 0x17C, 0xFF);                              // 00646A1D..00646A23
}

// 005B3050 CSWBaseItem::GetIconResRef(SHORT variation, CHAR gender) -> CResRef by hidden return. RET 0Ch.
// Original: 20-byte sprintf buffer {E-0x14}; the formats are fixed .rdata strings.
void* __fastcall recovered_base_item_get_icon_resref_005b3050(void* base, void*, void* out, U variation,
                                                              U gender) noexcept(false) {
    char text[0x14];                                                    // {E-0x14}
    const int number = static_cast<short>(static_cast<U16>(variation)); // movsx word
    if (rd32(base, 0x74) != 0 && static_cast<U8>(gender) != 0)          // 005B3050..005B3060
        format_006fadb0(text, kIconGender_0074b000, static_cast<const char*>(at(base, 0x3C)),
                        static_cast<int>(static_cast<signed char>(gender)), number);   // 005B3062..005B307A
    else
        format_006fadb0(text, kIcon_0074aff4, static_cast<const char*>(at(base, 0x3C)), number);   // 005B3084..005B3098
    static_cast<menu_resources::Ref*>(out)->construct_00406d80(text);  // 005B30A0..005B30AC CResRef(char*)
    return out;                                                         // 005B30B1
}
// 005B30E0 CSWBaseItem::GetModelResRef(SHORT variation, CHAR gender) -> CResRef by hidden return. RET 0Ch.
void* __fastcall recovered_base_item_get_model_resref_005b30e0(void* base, void*, void* out, U variation,
                                                               U gender) noexcept(false) {
    char text[0x14];                                                    // {E-0x14}
    const int number = static_cast<short>(static_cast<U16>(variation));
    if (rd32(base, 0x74) != 0 && static_cast<U8>(gender) != 0)          // 005B30E0..005B30F0
        format_006fadb0(text, kModelGender_0074b014, static_cast<const char*>(at(base, 0x3C)),
                        static_cast<int>(static_cast<signed char>(gender)), number);   // 005B30F2..005B310A
    else
        format_006fadb0(text, kModel_0074b00c, static_cast<const char*>(at(base, 0x3C)), number);   // 005B3114..005B3128
    static_cast<menu_resources::Ref*>(out)->construct_00406d80(text);  // 005B3130..005B313C
    return out;                                                         // 005B3141
}

// ============================================================================================ message handlers
// 0064E730 CSWCMessage::HandleServerToPlayerItemAppearance(DWORD* base_item, BYTE* variation, BYTE* a, BYTE* b)
// RET 10h -> 0 on overflow after the base item, else 1. The two extra bytes are read only for base items whose
// [+18h] model type is 1.
U __fastcall recovered_client_handle_item_appearance_0064e730(void* self, void*, U* base_item, U8* variation, U8* extra_a,
                                                              U8* extra_b) noexcept(false) {
    *base_item = kotor_fastcall_004d6780(self, nullptr, 0x20);   // 0064E732..0064E743
    if (recovered_player_message::overrun_004d6230(self) != 0) return 0;   // 0064E73F..0064E752
    *variation = kotor_fastcall_004d6720(self, nullptr, 8);   // 0064E755..0064E762
    void* const base = kotor_m15b_base_item_array_get_base_item_005b31a0(base_item_array(), nullptr, *base_item);   // 0064E764..0064E770
    if (ld<U8>(base, 0x18) == 1) {                                      // 0064E775..0064E779
        *extra_a = kotor_fastcall_004d6720(self, nullptr, 8);   // 0064E77B..0064E788
        *extra_b = kotor_fastcall_004d6720(self, nullptr, 8);   // 0064E78A..0064E797
    }
    return 1;                                                           // 0064E79A
}

// 0064E3D0 CSWCMessage::HandleServerToPlayerActiveItemProperties(OBJECT_ID item) RET 4 -> 1 / 0 (overflow).
// The record is consumed in full even when the item is not known on this client.
U __fastcall recovered_client_handle_active_item_properties_0064e3d0(void* self, void*, U id) noexcept(false) {
    ExoString description;                                              // {E-0x2c}
    make_empty_string(&description);                                    // 0064E3ED..0064E3F1
    void* const item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, id);   // 0064E3F6..0064E40B (esi)
    if (item != nullptr &&                                              // 0064E412..0064E414
        ld<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0xAC) >= 0x1F &&   // 0064E416..0064E424
        ld<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0xAC) <= 0x26) {   // 0064E426..0064E434
        const U16 charges = kotor_fastcall_004d6750(self, nullptr, 0x10);   // 0064E436..0064E443 (bx)
        if (recovered_player_message::overrun_004d6230(self) != 0) {    // 0064E43F..0064E44C
            frame_LogStringDtor_005e5c20(&description, nullptr);        // 0064E4E7..0064E4F3
            return 0;
        }
        st<I32>(item, 0x10C, static_cast<short>(charges));              // 0064E452..0064E455 movsx
    }
    {
        ResRef sound;                                                   // {E-0x1c}
        kotor_fastcall_00405ed0(&sound, nullptr);                       // 0064E45B..0064E461 (push 0 = no length out)
        ExoString text;                                                 // {E-0x24}
        kotor_fastcall_00692660(self, nullptr, &text, &sound, nullptr);   // 0064E466..0064E46E
        async_assign_string_005e5c50(&description, nullptr, &text);     // 0064E473..0064E47D
        frame_LogStringDtor_005e5c20(&text, nullptr);                   // 0064E482..0064E48B
    }
    const U identified = kotor_fastcall_004d66f0(self, nullptr);   // 0064E490..0064E49B {E-0x40}
    const U field_180 = kotor_fastcall_004d6780(self, nullptr, 0x20);   // 0064E497..0064E4A8 {E-0x3c}
    const U stack_size = kotor_fastcall_004d6780(self, nullptr, 0x20);   // 0064E4A4..0064E4B3 {E-0x38}
    const U field_188 = kotor_fastcall_004d66f0(self, nullptr);   // 0064E4B1..0064E4BE {E-0x34}
    const U field_118 = kotor_fastcall_004d66f0(self, nullptr);   // 0064E4BC..0064E4CB {E-0x30}
    const U8 count = kotor_fastcall_004d6720(self, nullptr, 8);   // 0064E4C7..0064E4DA (bl, {E+0x4})
    if (recovered_player_message::overrun_004d6230(self) != 0) {        // 0064E4D6..0064E4E5
        frame_LogStringDtor_005e5c20(&description, nullptr);            // 0064E4E7..0064E4F3
        return 0;                                                       // 0064E4FA
    }
    if (count != 0) {                                                   // 0064E50E..0064E511
        if (item != nullptr)                                            // 0064E517..0064E519
            recovered_client_item_set_num_active_properties_00646660(item, nullptr, count);   // 0064E51B..0064E521
        for (I32 i = 0; i < static_cast<I32>(count); ++i) {             // 0064E526..0064E603
            const U16 property = kotor_fastcall_004d6750(self, nullptr, 0x10);   // 0064E535..0064E542 (bp)
            const U16 subtype = kotor_fastcall_004d6750(self, nullptr, 0x10);    // 0064E53E..0064E54D {E-0x48}
            const U16 value = kotor_fastcall_004d6750(self, nullptr, 0x10);      // 0064E549..0064E55A {E-0x44}
            const U8 param = kotor_fastcall_004d6720(self, nullptr, 8);          // 0064E556..0064E567 {E-0x49}
            if (recovered_player_message::overrun_004d6230(self) != 0) {   // 0064E563..0064E572
                frame_LogStringDtor_005e5c20(&description, nullptr);    // 0064E71A..0064E726
                return 0;                                               // 0064E72B
            }
            if (item == nullptr) continue;                              // 0064E578..0064E57A
            st<U16>(recovered_client_item_get_active_property_006466a0(item, nullptr, i), 0, property);   // 0064E580..0064E58B
            st<U16>(recovered_client_item_get_active_property_006466a0(item, nullptr, i), 2, subtype);    // 0064E588..0064E598
            st<U16>(recovered_client_item_get_active_property_006466a0(item, nullptr, i), 6, value);      // 0064E59C..0064E5AC
            st<U8>(recovered_client_item_get_active_property_006466a0(item, nullptr, i), 8, param);       // 0064E5A9..0064E5B9
            st<U>(recovered_client_item_get_active_property_006466a0(item, nullptr, i), 0xC, 1);          // 0064E5BC..0064E5C7
            st<U8>(recovered_client_item_get_active_property_006466a0(item, nullptr, i), 0x10, 0);        // 0064E5C4..0064E5D6
            st<U8>(recovered_client_item_get_active_property_006466a0(item, nullptr, i), 0x11, 0xFF);     // 0064E5D3..0064E5E3
            // 0064E5DF..0064E5F5: property 0Ah selects the charge property index, but the original compares the
            // zero-extended byte [+17Ch] with -1 (movzx edx / cmp edx,-1), which never matches: the store is dead.
            const I32 current = static_cast<I32>(ld<U8>(item, 0x17C));
            if (property == 0xA && current == -1) st<U8>(item, 0x17C, static_cast<U8>(i));
        }
    }
    if (item != nullptr) {                                              // 0064E609..0064E60B
        st<U>(item, 0x180, field_180);                                  // 0064E619
        st<U>(item, 0x114, identified);                                 // 0064E622
        recovered_client_item_set_stack_size_00646810(item, nullptr, stack_size);   // 0064E61F..0064E628
        async_assign_string_005e5c50(at(item, 0x18C), nullptr, &description);   // 0064E62D..0064E638
        st<U>(item, 0x188, field_188);                                  // 0064E645
        st<U>(item, 0x118, field_118);                                  // 0064E64B
    }
    (void)frame_R4_005ed690(client_app(), nullptr);                     // 0064E651..0064E659 GetInGameGui (unused)
    const U8 enabled = kotor_fastcall_004d6720(self, nullptr, 8);   // 0064E65E..0064E66B {E-0x49}
    const U8 present = kotor_fastcall_004d6720(self, nullptr, 8);   // 0064E667..0064E678 (bl)
    if (recovered_player_message::overrun_004d6230(self) != 0) {        // 0064E674..0064E681
        frame_LogStringDtor_005e5c20(&description, nullptr);            // 0064E71A..0064E726
        return 0;
    }
    for (I32 j = 0; j < 8; ++j) {                                       // 0064E687..0064E6ED
        const U bit = 1u << j;                                          // 0064E694..0064E69B
        if ((bit & present) == 0) continue;                             // 0064E69D..0064E69F
        const U8 uses = kotor_fastcall_004d6720(self, nullptr, 8);   // 0064E6A1..0064E6AE {E+0x4}
        if (recovered_player_message::overrun_004d6230(self) != 0) {    // 0064E6AA..0064E6B9
            frame_LogStringDtor_005e5c20(&description, nullptr);        // 0064E71A..0064E726
            return 0;
        }
        if (item == nullptr) continue;                                  // 0064E6BB..0064E6BD
        recovered_client_item_set_property_uses_left_006469b0(item, nullptr, j, uses);   // 0064E6BF..0064E6C9
        void* const prop = recovered_client_item_get_active_property_006466a0(item, nullptr, j);   // 0064E6CE..0064E6D1
        if (prop != nullptr) st<U>(prop, 0xC, (enabled & static_cast<U8>(bit)) != 0 ? 1u : 0u);   // 0064E6D6..0064E6E6
    }
    frame_LogStringDtor_005e5c20(&description, nullptr);                // 0064E6EF..0064E6FB
    return 1;                                                           // 0064E700
}

// 0064B540 CSWCMessage::CreateItem(DWORD base_item, BYTE variation, CHAR gender) RET 0Ch -> new CSWCItem or 0.
// The icon (TGA/TPC) and model (MDL) resrefs fall back to the base item defaults; a missing default only formats a
// diagnostic string that is discarded (the original's log sink is absent from the retail image).
void* __fastcall recovered_client_create_item_0064b540(void* /*self*/, void*, U base_item, U variation_word,
                                                       U gender_word) noexcept(false) {
    void* const memory = live_operator_new(0x1A8);                      // 0064B561..0064B566
    void* item = nullptr;                                               // 0064B58D
    if (memory != nullptr) item = recovered_client_item_construct_00646230(memory, nullptr, base_item);   // 0064B57D..0064B582
    if (item == nullptr) return nullptr;                                // 0064B591..0064B59D -> 0064B5B3
    void* const base = kotor_m15b_base_item_array_get_base_item_005b31a0(base_item_array(), nullptr, base_item);   // 0064B59F..0064B5A8
    if (base == nullptr) return nullptr;                                // 0064B5AD..0064B5B1 (item not released, as in the original)
    const U variation = static_cast<U8>(variation_word);                // 0064B5D2 movzx di, byte
    const U gender = gender_word;                                       // 0064B5D7 ebx (callees use BL)
    ResRef ref;                                                         // {E-0x44}
    kotor_fastcall_00405ed0(&ref, nullptr);                             // 0064B5C9..0064B5CD
    {
        ResRef made, copied;                                            // {E-0x54} / {E-0x64}
        c2da_copy_00406120(&ref, nullptr, &copied,
                           recovered_base_item_get_icon_resref_005b3050(base, nullptr, &made, variation, gender));   // 0064B5DA..0064B5F2
    }
    if (exists(&ref, kResTga_3) == 0 && exists(&ref, kResTpc_bbf) == 0) {   // 0064B5F7..0064B62B
        ResRef fallback = ld<ResRef>(base, 0x4C), copied;               // 0064B631..0064B654 {E-0x54}
        c2da_copy_00406120(&ref, nullptr, &copied, &fallback);          // 0064B658..0064B65D
        if (exists(&ref, kResTga_3) == 0 || exists(&ref, kResTpc_bbf) == 0) {   // 0064B662..0064B693
            ExoString message, icon_text, default_text;                 // {E-0x54} / {E-0x64} / {E-0x6c}
            make_empty_string(&message);                                // 0064B699..0064B69D
            make_empty_string(&icon_text);                              // 0064B6A2..0064B6AE
            make_empty_string(&default_text);                           // 0064B6B3..0064B6BC
            ResRef made;                                                // {E-0x34}
            c2da_ref_to_string_00405f70(recovered_base_item_get_icon_resref_005b3050(base, nullptr, &made, variation, gender),
                                        nullptr, &icon_text);           // 0064B6C1..0064B6DE
            c2da_ref_to_string_00405f70(&ref, nullptr, &default_text);  // 0064B6E3..0064B6EC
            kotor_reconstruction::StartupFormat005e5680(reinterpret_cast<kotor_reconstruction::StartupString*>(&message),
                                                        kIconMissing_00751ca4,
                                                        async_string_cstr_005e5670(&icon_text, nullptr),
                                                        async_string_cstr_005e5670(&default_text, nullptr));   // 0064B6F1..0064B714
            frame_LogStringDtor_005e5c20(&default_text, nullptr);       // 0064B717..0064B720
            frame_LogStringDtor_005e5c20(&icon_text, nullptr);          // 0064B725..0064B72E
            frame_LogStringDtor_005e5c20(&message, nullptr);            // 0064B733..0064B73F
        }
    }
    {
        ResRef copied;                                                  // {E-0x34}
        c2da_copy_00406120(at(item, 0x138), nullptr, &copied, &ref);    // 0064B744..0064B758 icon
    }
    st<U8>(item, 0x11C, static_cast<U8>(variation));                    // 0064B761..0064B76A
    st<U8>(item, 0x11D, static_cast<U8>(gender));                       // 0064B773
    {
        ResRef made, copied;                                            // {E-0x34} / {E-0x54}
        c2da_copy_00406120(&ref, nullptr, &copied,
                           recovered_base_item_get_model_resref_005b30e0(base, nullptr, &made, variation, gender));   // 0064B764..0064B788
    }
    if (exists(&ref, kResMdl_7d2) == 0) {                               // 0064B78D..0064B7A6
        ResRef fallback = ld<ResRef>(base, 0x5C), copied;               // 0064B7AC..0064B7CF {E-0x54}
        c2da_copy_00406120(&ref, nullptr, &copied, &fallback);          // 0064B7D3..0064B7D8
        if (exists(&ref, kResMdl_7d2) == 0) {                           // 0064B7DD..0064B7F6
            ExoString message, model_text, default_text;                // {E-0x54} / {E-0x6c} / {E-0x64}
            make_empty_string(&message);                                // 0064B7FC..0064B800
            make_empty_string(&model_text);                             // 0064B805..0064B811
            make_empty_string(&default_text);                           // 0064B816..0064B81F
            ResRef made;                                                // {E-0x34}
            c2da_ref_to_string_00405f70(recovered_base_item_get_model_resref_005b30e0(base, nullptr, &made, variation, gender),
                                        nullptr, &model_text);          // 0064B824..0064B841
            c2da_ref_to_string_00405f70(&ref, nullptr, &default_text);  // 0064B846..0064B84F
            kotor_reconstruction::StartupFormat005e5680(reinterpret_cast<kotor_reconstruction::StartupString*>(&message),
                                                        kModelMissing_00751c70,
                                                        async_string_cstr_005e5670(&model_text, nullptr),
                                                        async_string_cstr_005e5670(&default_text, nullptr));   // 0064B854..0064B877
            frame_LogStringDtor_005e5c20(&default_text, nullptr);       // 0064B87A..0064B883
            frame_LogStringDtor_005e5c20(&model_text, nullptr);         // 0064B888..0064B891
            frame_LogStringDtor_005e5c20(&message, nullptr);            // 0064B896..0064B8A2
        }
    }
    {
        ResRef copied;                                                  // {E-0x24}
        c2da_copy_00406120(at(item, 0x128), nullptr, &copied, &ref);    // 0064B8A7..0064B8BB model
    }
    return item;                                                        // 0064B8C5
}

// 00655E40 CSWCMessage::HandleServerToPlayerCreatureUpdate_Appearance. ECX this, RET. EAX = 1 / 0.
// Record: OBJECT_ID creature, WORD mask, then per mask bit: 400h two CExoLocStrings (first / last name),
// 1 WORD appearance type, 2/4/80h BYTEs, 4000h BOOL, 8000h BYTE, 800h/1000h BYTEs, 8/10h/20h/40h BYTEs,
// 2000h WORD sound set, 200h BYTE count of equipment records {BYTE 'A'|'D', OBJECT_ID item, DWORD slot,
// [BOOL instant for slots 10h/20h], 'A': ItemAppearance + ActiveItemProperties records}.
// The creature's 3Ch-byte appearance block (+21Ch) is copied, edited and handed to CSWCCreature::SetAppearance
// (or RequestAppearance while the area is not ready to display).
U __fastcall recovered_client_handle_creature_appearance_00655e40(void* self, void*) noexcept(false) {
    ExoString first_name, last_name;                                    // {E-0xb0} / {E-0xb8}
    make_empty_string(&first_name);                                     // 00655E67..00655E6B
    make_empty_string(&last_name);                                      // 00655E70..00655E7F
    U result = 0;
    U object_id = kotor_fastcall_00692630(self, nullptr);   // 00655E84..00655E97 {E-0xa8}
    const U mask = kotor_fastcall_004d6750(self, nullptr, 0x10);        // 00655E93..00655EA4 {E-0xc4}
    void* creature = nullptr;                                           // ebp
    void* module = nullptr;                                             // {E-0x2c}
    void* area = nullptr;                                               // {E-0x70}
    void* objects = nullptr;                                            // {E-0x28}
    U8 flags = 0;                                                       // {E-0xbe}
    U reattach = 0;                                                     // {E-0x6c}
    alignas(4) unsigned char appearance[kAppearanceSize];               // {E-0x68}..{E-0x2c}
    U has_model = 0;                                                    // {E-0x78}
    U right_instant = 0, left_instant = 0;                              // {E-0x7c} / {E-0x74}
    void* right_item = nullptr; U right_changed = 0;                    // {E-0x9c} / {E-0x98}
    void* left_item = nullptr; U left_changed = 0;                      // {E-0x94} / {E-0x90}
    // Locals the original leaves indeterminate unless the message writes them (zeroed here, see receipt).
    U sound_row = 0;                                                    // {E-0xa0} (C2DA::GetINTEntry out)
    U8 model_variation = 0, model_extra_a = 0, model_extra_b = 0;       // {E-0x84} / {E-0xa1} / {E-0xa2}

    if (recovered_player_message::overrun_004d6230(self) != 0) goto done;   // 00655EA0..00655EAF
    objects = kotor_fastcall_005ed570(client_app(), nullptr);   // 00655EB5..00655ED0
    creature = frame_R4_005ed5b0(client_app(), nullptr, object_id);     // 00655EC2..00655EDC
    if (creature == nullptr) goto done;                                 // 00655EDE..00655EE0
    module = kotor_fastcall_005ed530(client_app(), nullptr);            // 00655EE6..00655EF5
    if (module == nullptr) goto done;                                   // 00655EF3..00655EFC
    area = rp(module, 0x48);                                            // 00655F02..00655F07
    if (area == nullptr) goto done;                                     // 00655F05..00655F0B

    if ((mask & 0x400) != 0) {                                          // 00655F11..00655F18
        ExoString* const names[2] = {&first_name, &last_name};
        for (ExoString* name : names) {                                 // 00655F1E..00655F95 (two identical reads)
            ResRef sound;                                               // {E-0x24}
            kotor_fastcall_00405ed0(&sound, nullptr);                   // CResRef() (push 0 = no length out)
            ExoString text;                                             // {E-0x8c}
            kotor_fastcall_00692660(self, nullptr, &text, &sound, nullptr);
            async_assign_string_005e5c50(name, nullptr, &text);
            frame_LogStringDtor_005e5c20(&text, nullptr);
        }
        if (recovered_player_message::overrun_004d6230(self) != 0) goto done;   // 00655F9A..00655FA5
        kotor_m15a_creature_set_name_0060d600(creature, nullptr, &first_name, &last_name);   // 00655FAB..00655FB7
    }
    if (recovered_player_message::overrun_004d6230(self) != 0) goto done;   // 00655FBC..00655FC7
    reattach = 0;                                                       // 00655FD3
    flags = 0;                                                          // 00655FD7
    std::memcpy(appearance, rp(creature, 0x21C), kAppearanceSize);      // 00655FCD..00655FE9 rep movsd

    if ((mask & 1) != 0) {                                              // 00655FDB..00655FEB
        const U16 type = kotor_fastcall_004d6750(self, nullptr, 0x10);   // 00655FF1..00655FFF (di)
        if (recovered_player_message::overrun_004d6230(self) != 0) goto done;   // 00655FFA..00656007
        if (type != ld<U16>(appearance, 0x18)) {                        // 0065600D..00656015
            if (rp(creature, 0x17C) != nullptr)                         // 0065601B..00656023
                recovered_client_creature_delete_looping_sound_0060d510(creature, nullptr);   // 00656025..00656027
            {
                ExoString column;                                       // {E-0x84}
                frame_LogStringCtor_005e5a90(&column, nullptr, kSoundAppType_00751ea8);   // 0065602C..00656035
                void* const table = rp(rp(rules(), 0xB8), 0xC);         // 0065603A..00656046
                c2da_current_get_int_00413660(table, nullptr, type, &column, &sound_row);   // 00656049..00656061
                frame_LogStringDtor_005e5c20(&column, nullptr);         // 00656066..00656072
            }
            ExoString sound;                                            // {E-0x8c}
            make_empty_string(&sound);                                  // 00656077..0065607B
            {
                ExoString column;                                       // {E-0x84}
                frame_LogStringCtor_005e5a90(&column, nullptr, kLooping_00747798);   // 00656080..00656091
                void* const table = rp(rp(rules(), 0xB8), 0x5C);        // 00656096..006560A2
                const int found = c2da_current_get_string_00413270(table, nullptr, static_cast<int>(sound_row), &column,
                                                                   &sound);   // 006560A5..006560BE (esi)
                frame_LogStringDtor_005e5c20(&column, nullptr);         // 006560C3..006560D1
                if (found != 0)                                         // 006560D6..006560D8
                    recovered_client_creature_set_new_looping_sound_0060d460(creature, nullptr, &sound);   // 006560DA..006560E1
            }
            frame_LogStringDtor_005e5c20(&sound, nullptr);              // 006560E6..006560F2
        }
        st<U16>(appearance, 0x18, type);                                // 006560F7
    }
    if ((mask & 2) != 0) st<U8>(appearance, 0x1C, kotor_fastcall_004d6720(self, nullptr, 8));   // 006560FF..0065610F
    if ((mask & 4) != 0) st<U8>(appearance, 0x1B, kotor_fastcall_004d6720(self, nullptr, 8));   // 00656116..00656126
    if ((mask & 0x80) != 0) st<U8>(appearance, 0x1A, kotor_fastcall_004d6720(self, nullptr, 8));   // 0065612D..0065613E
    if ((mask & 0x4000) != 0) st<U>(appearance, 0x30, kotor_fastcall_004d66f0(self, nullptr));     // 00656145..00656155
    if ((mask & 0x8000) != 0) st<U8>(appearance, 0x34, kotor_fastcall_004d6720(self, nullptr, 8));  // 0065615C..0065616D
    if ((mask & 0x800) != 0) st<U8>(appearance, 0x21, kotor_fastcall_004d6720(self, nullptr, 8));   // 00656174..00656186
    if ((mask & 0x1000) != 0) st<U8>(appearance, 0x22, kotor_fastcall_004d6720(self, nullptr, 8));  // 0065618D..0065619F
    if ((mask & 8) != 0) st<U8>(appearance, 0x1D, kotor_fastcall_004d6720(self, nullptr, 8));       // 006561A6..006561B6
    if ((mask & 0x10) != 0) st<U8>(appearance, 0x1E, kotor_fastcall_004d6720(self, nullptr, 8));    // 006561BD..006561CD
    if ((mask & 0x20) != 0) st<U8>(appearance, 0x1F, kotor_fastcall_004d6720(self, nullptr, 8));    // 006561D4..006561E4
    if ((mask & 0x40) != 0) st<U8>(appearance, 0x20, kotor_fastcall_004d6720(self, nullptr, 8));    // 006561EB..006561FB
    if ((mask & 0x2000) != 0)                                           // 00656202..00656209
        kotor_m15a_creature_set_sound_set_0060b7c0(creature, nullptr, kotor_fastcall_004d6750(self, nullptr, 0x10));   // 0065620B..00656217

    {
        const void* const current = rp(creature, 0x21C);                // 0065621C
        if (ld<U16>(appearance, 0x18) != ld<U16>(current, 0x18) ||      // 00656222..0065622E
            ld<U8>(appearance, 0x1C) != ld<U8>(current, 0x1C) ||        // 00656230..0065623A
            ld<U8>(appearance, 0x1B) != ld<U8>(current, 0x1B)) {        // 0065623C..00656246
            flags = 0xEF;                                               // 00656248
            reattach = 1;                                               // 0065624D
        }
        if (ld<U8>(appearance, 0x1A) != ld<U8>(current, 0x1A) ||        // 00656255..0065625F
            ld<U8>(appearance, 0x34) != ld<U8>(current, 0x34) ||        // 00656261..0065626B
            rd32(appearance, 0x30) != rd32(current, 0x30))              // 0065626D..00656277
            flags |= 2;                                                 // 00656279
        if (ld<U8>(appearance, 0x21) != ld<U8>(current, 0x21)) flags |= 4;   // 0065627E..0065628A
        if (ld<U8>(appearance, 0x22) != ld<U8>(current, 0x22)) flags |= 8;   // 0065628F..0065629B
    }
    has_model = model_of(creature, 0xFF, 0) != 0 ? 1u : 0u;             // 006562A0..006562C7 neg/sbb/neg
    right_instant = 0; left_instant = 0;                                // 006562BB / 006562BF
    right_item = nullptr; left_item = nullptr; right_changed = 0; left_changed = 0;   // 006562D0..006562DC

    if ((mask & 0x200) != 0) {                                          // 006562C3..006562E0
        const U8 count = kotor_fastcall_004d6720(self, nullptr, 8);   // 006562E6..006562F3 {E-0xba}
        if (recovered_player_message::overrun_004d6230(self) != 0) goto done;   // 006562EF..006562FE
        for (U8 index = 0; index < count; ++index) {                    // 00656304..0065661A {E-0xb9}
            const U8 kind = kotor_fastcall_004d6720(self, nullptr, 8);   // 00656314..00656324 {E-0xa3}
            object_id = kotor_fastcall_00692630(self, nullptr);   // 00656326..00656331 {E-0xa8}
            const U slot = kotor_fastcall_004d6780(self, nullptr, 0x20);    // 0065632D..0065633F {E-0xc4}
            if (slot == 0x10) left_instant = kotor_fastcall_004d66f0(self, nullptr);         // 0065633C..0065634C
            else if (slot == 0x20) right_instant = kotor_fastcall_004d66f0(self, nullptr);   // 00656352..0065635E
            if (recovered_player_message::overrun_004d6230(self) != 0) goto done;   // 00656362..0065636C
            if (kind == 0x41) {                                         // 00656372..00656378 'A' equip
                U base_item_id = 0;                                     // {E-0xa0}
                recovered_client_handle_item_appearance_0064e730(self, nullptr, &base_item_id, &model_variation,
                                                                 &model_extra_a, &model_extra_b);   // 0065637E..00656394 (result unused)
                void* item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, object_id);   // 00656399..006563AC
                if (item == nullptr) {                                  // 006563AE..006563B0
                    const U gender = ld<U8>(appearance, 0x1B) == 0 ? 0x4Du : 0x46u;   // 006563B2..006563D5 'M' / 'F'
                    item = recovered_client_create_item_0064b540(self, nullptr, base_item_id, model_variation, gender);   // 006563D7..006563D9
                    if (item == nullptr) continue;                      // 006563DE..006563E2
                    vslot<void (__fastcall*)(void*, void*, U)>(at(item, 0x10), 0x04)(at(item, 0x10), nullptr, object_id);   // 006563E8..006563F5 SetId
                    if (recovered_object_array_add_external_004d7ff0(objects, nullptr, &object_id, at(item, 0x10), 1) !=
                        kGameObjectOk_0074666c)                         // 006563F8..00656412
                        continue;
                } else if (kotor_m15b_base_item_array_get_base_item_005b31a0(base_item_array(), nullptr, rd32(item, 0xC)) ==
                           nullptr) {                                   // 0065641A..0065642E
                    continue;
                }
                recovered_client_handle_active_item_properties_0064e3d0(self, nullptr, object_id);   // 00656434..0065643B (result unused)
                void* const base = kotor_m15b_base_item_array_get_base_item_005b31a0(base_item_array(), nullptr,
                                                                                     rd32(item, 0xC));   // 00656440..0065644D {E-0x8c}
                if (ld<U8>(base, 0x18) == 1) {                          // 00656452..0065645A
                    st<U8>(appearance, 0x00, model_extra_a);            // 0065645C..00656464
                    st<U8>(appearance, 0x01, model_extra_b);            // 00656460..00656468
                }
                void* const object = at(item, 0x10);                    // 0065647F..00656482 (edi)
                if (slot == 1 || slot == 0x10 || slot == 0x20) {        // 0065646C..0065647D
                    if (model_of(object, 0xFF, 1) == 0) {               // 0065647F..00656496
                        vslot<void (__fastcall*)(void*, void*, void*, U, U)>(object, 0x78)(object, nullptr, at(item, 0x128),
                                                                                           0xFF, 0);   // 00656498..006564AA LoadModel
                        if (rd32(base, 0x48) == 1) {                    // 006564AD..006564B5
                            ResRef env;                                 // {E-0x24}
                            reinterpret_cast<menu_resources::Ref*>(&env)->construct_00406d80(kDefault_0073df6c);   // 006564B7..006564C3
                            vslot<void (__fastcall*)(void*, void*, U, U, void*)>(object, 0xB0)(object, nullptr, 0xFF, 1, &env);   // 006564C8..006564DB SetEnvMap
                        }
                    }
                }
                vslot<SlotU1>(object, 0x94)(object, nullptr, rd32(area, 0x184));   // 006564E1..006564F4 AttachToScene
                if (slot == 2) {                                        // 006564FA..006564FF body
                    vslot<SlotU1>(object, 0x94)(object, nullptr, 0);    // 00656501..00656507
                    (void)recovered_client_creature_get_armour_class_0060d540(creature, nullptr);   // 0065650D..0065650F (unused)
                    st<U>(appearance, 0x14, 1);                         // 00656514
                    for (unsigned k = 0; k < 6; ++k)                    // 0065651F..0065655E words from BYTE item+4..+9
                        st<U16>(appearance, 0x02 + 2 * k, ld<U8>(item, 4 + k));
                    flags |= 1;                                         // 00656551..00656563
                    recovered_client_creature_equip_to_inventory_slot_00613ba0(creature, nullptr, 2, item);   // 0065655D..00656605
                } else if (slot == 1) {                                 // 0065656E..00656575 head
                    vslot<SlotU1>(object, 0x94)(object, nullptr, 0);    // 00656577..0065657D
                    flags |= 2;                                         // 00656583..00656596
                    st<U>(appearance, 0x10, object_id);                 // 00656587..0065658F
                    recovered_client_creature_equip_to_inventory_slot_00613ba0(creature, nullptr, 1, item);   // 0065658E..00656605
                } else if (slot == 0x20) {                              // 0065659E..006565A1
                    right_item = item; right_changed = 1;               // 006565B6..006565BA
                } else if (slot == 0x10) {                              // 006565A3..006565A6
                    left_item = item; left_changed = 1;                 // 006565A8..006565AC
                }
            } else if (kind == 0x44) {                                  // 006565C4..006565C6 'D' unequip
                if (slot == 2) {                                        // 006565C8..006565CB
                    for (unsigned o = 0x02; o < 0x0E; o += 4) st<U>(appearance, o, 0);   // 006565D1..006565DB
                    (void)recovered_client_creature_get_armour_class_0060d540(creature, nullptr);   // 006565DF..006565E1 (unused)
                    st<U>(appearance, 0x14, 0);                         // 006565EA
                    st<U8>(appearance, 0x00, 0);                        // 006565F5
                    flags |= 1;                                         // 006565FA..006565FC
                    recovered_client_creature_equip_to_inventory_slot_00613ba0(creature, nullptr, 2, nullptr);   // 00656600..00656605
                } else if (slot == 1) {                                 // 00656665..00656667
                    st<U>(appearance, 0x10, kInvalidObject_00746644);   // 00656669..00656673
                    flags |= 2;                                         // 0065666F..006565FC
                    recovered_client_creature_equip_to_inventory_slot_00613ba0(creature, nullptr, 1, nullptr);   // 00656600..00656605
                } else if (slot == 0x20) {                              // 0065667B..0065667E
                    right_item = nullptr; right_changed = 1;            // 00656680..00656688
                } else if (slot == 0x10) {                              // 00656691..00656694
                    left_item = nullptr; left_changed = 1;              // 0065669A..006566A2
                } else {
                    recovered_client_creature_equip_to_inventory_slot_00613ba0(creature, nullptr, slot, nullptr);   // 00656600..00656605
                }
            }
        }
    }

    if (flags != 0) {                                                   // 00656628..0065662E
        const U whole = (right_changed == 0 && left_changed == 0) ? 1u : 0u;   // 00656652..006566AD / 00656725..0065673A
        if (kotor_cell_007a2484 == 0) {        // 00656634..0065663B
            // 0065673B: the original pushes the DWORD at {E-0xbe} (flags plus the loop counters above it); the callee
            // reads only the low byte.
            if (kotor_m15a_creature_set_appearance_006134c0(creature, nullptr, appearance, flags, whole) == 0)   // 0065673A..00656747
                goto done;                                              // 0065674C..0065674E
        } else if (kotor_m15_client_get_area_not_ready_005edad0(client_app(), nullptr) != 0) {   // 00656641..00656650
            if (kotor_m15a_creature_set_appearance_006134c0(creature, nullptr, appearance, 0xEF, whole) == 0)   // 006566AD..006566BA
                goto done;                                              // 006566BF..006566C7
        } else {
            if (recovered_client_creature_request_appearance_0060c910(creature, nullptr, appearance, flags) == 0)   // 006566CC..006566D8
                goto done;                                              // 006566DD..006566DF
            if (recovered_client_get_pause_state_005ed640(client_app(), nullptr, 1) != 0 ||   // 006566E5..006566F7
                recovered_client_get_pause_state_005ed640(client_app(), nullptr, 2) != 0)     // 006566F9..0065670B
                st<U>(creature, 0x310, 1);                              // 00656719
            else
                st<U>(creature, 0x310, 0x1F4);                          // 0065670D
        }
    }
    if (kotor_cell_007a2484 != 0) {            // 00656754..0065675B
        void* const current = rp(creature, 0x21C);                      // 00656767
        if (rd32(creature, 0x310) == 0 || ld<U16>(appearance, 0x18) != ld<U16>(current, 0x18)) {   // 0065675D..00656779
            st<U16>(creature, 0x38C, ld<U16>(rp(creature, 0x21C), 0x18));   // 0065677B..00656785 previous type
            st<U16>(rp(creature, 0x21C), 0x18, ld<U16>(appearance, 0x18));   // 0065678C..00656794
        }
    }

    // 00656798..0065699B right (20h) then left (10h) hand: same sequence, parts FBh then F7h on the creature.
    for (unsigned hand = 0; hand < 2; ++hand) {
        const U slot = hand == 0 ? 0x20u : 0x10u;
        if ((hand == 0 ? right_changed : left_changed) == 0) continue;  // 00656798..0065679E / 0065688F..00656895
        void* const item = hand == 0 ? right_item : left_item;          // 006567A4 / 0065689B
        const U instant = hand == 0 ? right_instant : left_instant;     // {E-0x7c} / {E-0x74}
        if (item == nullptr) {                                          // 006567A8..006567AC / 0065689F..006568A3
            recovered_client_creature_set_last_instant_0060cd50(creature, nullptr, slot, instant);   // 00656878..0065687F / 00656984..0065698B
            recovered_client_creature_equip_to_inventory_slot_00613ba0(creature, nullptr, slot, nullptr);   // 00656884..0065688A / 00656990..00656996
            continue;
        }
        U same_part = 0;                                                // {E-0xa0} / edi
        void* const object = at(item, 0x10);                            // 006567CA..006567D1 / 006568BB..006568BF
        U creature_part = model_of(creature, 0xFB, 1);                  // 006567B2..006567C4 / 006568A9..006568B5 (FBh both hands)
        if (creature_part == model_of(object, 0xFF, 1)) same_part = 1;  // 006567CE..006567E9 / 006568C2..006568D9
        creature_part = model_of(creature, 0xF7, 1);                    // 006567F1..006567FD / 006568DE..006568EA
        if (creature_part != model_of(object, 0xFF, 1) && same_part == 0)   // 00656803..00656820 / 006568F0..00656909
            vslot<SlotU1>(object, 0x94)(object, nullptr, 0);            // 00656822..00656828 / 0065690B..00656910
        recovered_client_creature_set_last_instant_0060cd50(creature, nullptr, slot, instant);   // 0065682E..00656837 / 00656916..0065691F
        if (has_model == 0) {                                           // 0065683C..0065683E / 00656924..0065692A
            kotor_m15a_item_set_equipped_by_00646600(item, nullptr, creature);   // 00656840..00656847 / 0065692C..00656933
            recovered_client_creature_set_equipped_item_id_0060cbd0(creature, nullptr, slot, rd32(item, 0x14));   // 0065684C..00656854 / 00656938..00656940
            kotor_m15a_creature_update_weapon_class_00613710(creature, nullptr);   // 00656859..0065685B / 00656945..00656947
            kotor_m15a_switch_weapon_event_00610f40(0, kLoading_0074f4cc, creature);   // 00656860..0065686C / 0065694C..00656959
            if (hand == 1) recovered_client_creature_set_last_instant_0060cd50(creature, nullptr, 0x10, 0);   // 0065695C..00656962
        } else {
            recovered_client_creature_equip_to_inventory_slot_00613ba0(creature, nullptr, slot, item);   // 00656871..0065688A / 00656969..00656972
            if (hand == 1) recovered_client_creature_set_last_instant_0060cd50(creature, nullptr, 0x10, 0);   // 00656977..0065697D
        }
    }
    if (right_changed != 0 || left_changed != 0) {                      // 0065699B..006569A9
        vslot<SlotU>(creature, 0xE0)(creature, nullptr);                // 006569AB..006569B0 DetachVisualEffects
        vslot<SlotU>(creature, 0xE4)(creature, nullptr);                // 006569B6..006569BB ReAttachVisualEffects
    }
    kotor_m15a_creature_update_shadow_blob_0060e5b0(creature, nullptr); // 006569C1..006569C3
    U attached;                                                         // esi
    if (reattach != 0) {                                                // 006569C8..006569CE
        vslot<SlotU1>(creature, 0x94)(creature, nullptr, rd32(area, 0x184));   // 006569D0..006569E0 AttachToScene
        attached = 1;                                                   // 006569E6
    } else {
        attached = 0;                                                   // 006569ED
    }
    if (client_get_player_creature_005ed540(client_app(), nullptr) == creature) {   // 006569EF..006569FE
        if (kotor_cell_007a2484 != 0) {        // 00656A00..00656A07
            if (rd32(creature, 0x310) != 0) st<U>(creature, 0x314, 1);  // 00656A09..00656A17
        } else if (attached != 0) {                                     // 00656A26..00656A28
            if (rp(module, 0x40) == nullptr) goto done;                 // 00656A2E..00656A3A -> 00656A40 (EAX = 0)
            kotor_fastcall_0063f9d0(module, nullptr, rd32(creature, 4), 0);   // 00656B1B..00656B21
        }
    } else {
        const U mine = rd32(creature, 4);                               // 00656A86
        if (kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr) == mine) {   // 00656A7D..00656A90
            void* const app = client_app();                             // 00656A96..00656A9C (esi)
            void* const player = recovered_client_get_player_by_id_005ed590(
                app, nullptr, kotor_m15_client_get_player_creature_id_005ed550(app, nullptr));   // 00656A9F..00656AA9
            if (player != nullptr) st<U>(player, 0x28, rd32(creature, 4));   // 00656AAE..00656AB5
            recovered_client_set_player_creature_005ed780(client_app(), nullptr, rd32(creature, 4));   // 00656AB8..00656AC5
            void* const module_now = kotor_fastcall_005ed530(client_app(), nullptr);   // 00656ACA..00656AD8
            if (module_now == nullptr || rp(module_now, 0x40) == nullptr) goto done;   // 00656ADA..00656AE7
            (void)recovered_client_get_player_by_id_005ed590(client_app(), nullptr, object_id);   // 00656AED..00656AFA (unused)
            kotor_fastcall_005ed720(client_app(), nullptr);   // 00656AFF..00656B07
            if (kotor_cell_007a2484 != 0) {    // 00656B0C..00656B13 -> 00656A09
                if (rd32(creature, 0x310) != 0) st<U>(creature, 0x314, 1);
            } else {
                kotor_fastcall_0063f9d0(module_now, nullptr, rd32(creature, 4), 0);   // 00656B19..00656B21
            }
        }
    }
    if (has_model == 0) {                                               // 00656B26..00656B2C
        const U anim = ld<U16>(creature, 0x3D4);                        // 00656B2E..00656B30
        if (kotor_m15a_object_animation_looping_0063c300(creature, nullptr, anim) != 0)   // 00656B37..00656B41
            kotor_m15a_object_set_looping_animation_0063d830(creature, nullptr, anim);   // 00656B43..00656B46
        vslot<SlotU3>(creature, 0x9C)(creature, nullptr, anim, 0, 0);  // 00656B4B..00656B55 SetAnimation
    }
    result = 1;                                                         // 00656B88
done:
    frame_LogStringDtor_005e5c20(&last_name, nullptr);                  // 00656A40..00656A4C / 00656B5B..00656B67
    frame_LogStringDtor_005e5c20(&first_name, nullptr);                 // 00656A51..00656A60 / 00656B6C..00656B7B
    return result;                                                      // 00656A65 / 00656B88
}
}  // extern "C"
