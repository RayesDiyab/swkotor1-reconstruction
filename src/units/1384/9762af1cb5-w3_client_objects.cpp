// lane-w3 unit w3-client-objects: the client GameObjectUpdate message (major 0x05) routers and the creature add path.
//   0066A4A0 HandleServerToPlayerGameObjectUpdate   00656BD0 Update_Add   0066A2A0 Update_Update
//   00656C90 Update_Delete   00656C60 Update_Appearance   0064C0A0 CreatureUpdate_Add
//   0064CAF0 CreatureUpdate_ControlObject   (00677410 SendPlayerToServerMessage: lane-w2)
//   004D7FF0 CGameObjectArray::AddExternalObject   00604A20 CClientAIMaster::AddObject   006047A0 CClientAIList::AddObject
//   0060EF50 CSWCCreature::SetInParty   0060B380 SetMovingOrientation   0060B3C0 SetStationaryOrientation
//   00614B20 CSWCCreature::SetArea   0063F570 CSWCObject::SetArea   0066DCC0 CExoArrayList<int>::remove
//   0063E350 CSWCObject::SetObjectVisibilityOnSetArea   0063E220 SetFadeState   0063CE60 SetDesiredFadeState
//   0063DEE0 GetFinalFadeAlpha   0063DE50 GetVisualEffectAlpha   00613A70 CSWCCreature::UpdateRelativePerception
//   00616FB0 CSWCObject::ShowShadowBlob   0060E490 CSWCCreature::TurnOnShadows   0060E520 CSWCCreature::TurnOffShadows
//   005EE1A0 SetCharacterChangeInProgress  005ED780 SetPlayerCreature  005ED590/005F5CB0 GetPlayerByGameObjectID
//   005ED720 AttachPlayerGameObjectToArea
#include "0bc18821ad-w3_client_objects.h"

using namespace kotor_w3;

// [007A2484] .data DWORD = 1 at load; no instruction of the image writes it (lane-w3 tools/xref.py: readers in
// ControlObject, AttachPlayerGameObjectToArea, CreatureUpdate_Appearance and CSWCCreature::AIUpdate). Single owner
// lane-w4 (kotor_cell_007a2484, frame/client_update.cpp); declared in w3_client_objects.h (coordinator ruling t2-4).

namespace {
constexpr U8 kCreature_00746634 = 5, kItem_00746635 = 6, kTrigger_00746636 = 7, kPlaceable_00746638 = 9,
             kDoor_00746639 = 10, kAreaOfEffect_0074663a = 11;        // .rdata 00746634..0074663A object types
constexpr U8 kGameObjectOk_0074666c = 0;                               // .rdata 0074666C
constexpr U kInvalidObject_00746644 = 0x7F000000u;                     // .rdata 00746644
U8 read_byte(void* m) noexcept { return kotor_fastcall_004d6720(m, nullptr, 8); }
float read_float(void* m) noexcept { return kotor_fastcall_004d67b0(m, nullptr, bitsf(1.0f), 0x20); }
using Slot0 = void (__fastcall*)(void*, void*);
using SlotU = U (__fastcall*)(void*, void*);
}  // namespace

extern "C" {
// ============================================================================================ client app leaves
// 005EE1A0 CClientExoApp::SetCharacterChangeInProgress(BOOL): [[this+4]+4B4h]. RET 4.
void __fastcall recovered_client_set_character_change_in_progress_005ee1a0(void* app, void*, U value) noexcept {
    st<U>(rp(app, 4), 0x4B4, value);                                   // 005EE1A0..005EE1A7
}
// 005ED780 CClientExoApp::SetPlayerCreature(id): tail call on [this+4]. RET 4.
void __fastcall recovered_client_set_player_creature_005ed780(void* app, void*, U id) noexcept(false) {
    kotor_fastcall_005f2bb0(rp(app, 4), nullptr, id);   // 005ED780..005ED783
}
// 005F5CB0 CClientExoAppInternal::GetPlayerByGameObjectID(id): walk the player list [this+1Ch] for player+28h == id.
void* __fastcall recovered_client_internal_get_player_by_id_005f5cb0(void* internal, void*, U id) noexcept(false) {
    void* const list = rp(internal, 0x1C);                             // 005F5CB4
    void* position = rp(list, 0);                                      // 005F5CB7..005F5CB9 {E-0x4} = head
    void* player = rp(list, 0);                                        // 005F5CBD
    if (player != nullptr) player = rp(player, 8);                     // 005F5CBF..005F5CC4 node->data
    while (player != nullptr) {                                        // 005F5CC7..005F5CE4
        if (rd32(player, 0x28) == id) return player;                   // 005F5CD0..005F5CD3
        player = unresolved_list_next_005e9b90(rp(internal, 0x1C), nullptr, &position);   // 005F5CD5..005F5CDD
    }
    return nullptr;                                                    // 005F5CE6
}
void* __fastcall recovered_client_get_player_by_id_005ed590(void* app, void*, U id) noexcept(false) {
    return recovered_client_internal_get_player_by_id_005f5cb0(rp(app, 4), nullptr, id);   // 005ED590..005ED593
}
// (t2, coordinator decision after OVERLAP-W4.md) 005ED720 CClientExoApp::AttachPlayerGameObjectToArea dropped: single owner lane-w4 `kotor_fastcall_005ed720`.

// 00677410 CSWCMessage::SendPlayerToServerMessage: single owner lane-w2 (@recovered_client_send_00677410@24,
// lane-w2/tiers/t4 w2_g07_native.cpp; coordinator ruling t3). The lane-w3 body was dropped.

// ============================================================================================ object array / AI list
// 004D7FF0 CGameObjectArray::AddExternalObject(DWORD* id, CGameObject* object, int) RET 0Ch -> AL (0 ok, 1 bad id,
// 4 null object). External ids live in buckets 1000h..1FFFh, each bucket sorted by descending id.
U8 __fastcall recovered_object_array_add_external_004d7ff0(void* array, void*, U* id, void* object, U) noexcept(false) {
    if (object == nullptr) return 4;                                   // 004D7FF2..004D8000
    const U key = *id & 0x7FFFFFFFu;                                   // 004D8003..004D800B
    const U bucket = (key & 0xFFF) + 0x1000;                           // 004D8011..004D8019
    if ((key & 0x7F000000u) != 0) {                                    // 004D801F..004D8025
        if (key < 0x7F000001u || key > 0x7FFFFFFFu) return 1;          // 004D8027..004D803E
        if (rd32(array, 0x10) >= key) st<U>(array, 0x10, key - 1);     // 004D8040..004D8048
    } else {
        if (key > 0x7EFFFFFFu) return 1;                               // 004D804D..004D8053
        if (rd32(array, 8) <= key) st<U>(array, 8, key + 1);           // 004D8055..004D805D
    }
    unsigned char* node = static_cast<unsigned char*>(live_operator_new(0xC));   // 004D8060..004D8062
    if (node != nullptr) { st<U>(node, 0, 0); st<U>(node, 4, 0); st<U>(node, 8, 0); }   // 004D806C..004D8075
    st<U>(node, 8, 0);                                                 // 004D807C (null node faults as in the original)
    st<U>(node, 0, key);                                               // 004D807F
    st<void*>(node, 4, object);                                        // 004D8081
    void** const slot = static_cast<void**>(at(rp(array, 0), bucket * 4));   // 004D8084..004D808C
    void* const head = *slot;
    if (head == nullptr) {                                             // 004D808A..004D808F
        *slot = node;                                                  // 004D8091
    } else {
        void* previous = nullptr;                                      // 004D8097
        void* current = head;                                          // 004D8095
        for (;;) {
            if (rd32(current, 0) <= key) {                             // 004D80A0..004D80A2
                if (previous == nullptr) {                             // 004D80AF..004D80B1
                    st<void*>(node, 8, head);                          // 004D80B3
                    st<void*>(rp(array, 0), bucket * 4, node);         // 004D80B6..004D80B9
                } else {
                    st<void*>(node, 8, rp(previous, 8));               // 004D80BE..004D80C1
                    st<void*>(previous, 8, node);                      // 004D80C4
                }
                break;
            }
            previous = current;                                        // 004D80A4
            current = rp(current, 8);                                  // 004D80A6
            if (current == nullptr) { st<void*>(previous, 8, node); break; }   // 004D80A9..004D80AD -> 004D80C4
        }
    }
    *id |= 0x80000000u;                                                // 004D80C7..004D80D6
    return 0;                                                          // 004D80D8
}

// 006047A0 CClientAIList::AddObject(CSWCObject*) RET 4 -> 0 when already listed, else 1 (appended, node in obj+1Ch).
U __fastcall recovered_client_ai_list_add_object_006047a0(void* list_owner, void*, void* object) noexcept(false) {
    void* position = rp(rp(list_owner, 4), 0);                         // 006047A4..006047B0 {E-0x4}
    while (position != nullptr) {                                      // 006047B4 / 006047D5..006047D7
        if (unresolved_list_at_pos_005e9b60(rp(list_owner, 4), nullptr, position) == rd32(object, 4)) return 0;   // 006047B6..006047C2
        unresolved_list_next_005e9b90(rp(list_owner, 4), nullptr, &position);   // 006047C4..006047CC
    }
    st<I32>(list_owner, 0, ld<I32>(list_owner, 0) + 1);                // 006047D9..006047DF count
    void* const node = resource_append_005e9dc0(rp(list_owner, 4), nullptr, word_ptr(rd32(object, 4)));   // 006047E1..006047E5 AddTail(id)
    st<void*>(object, 0x1C, node);                                     // 006047EA
    if (rp(list_owner, 8) == nullptr) st<void*>(list_owner, 8, node);  // 006047ED..006047F4
    return 1;                                                          // 006047F8
}
// 00604A20 CClientAIMaster::AddObject: ECX += 4; jmp CClientAIList::AddObject.
U __fastcall recovered_client_ai_master_add_object_00604a20(void* master, void*, void* object) noexcept(false) {
    return recovered_client_ai_list_add_object_006047a0(at(master, 4), nullptr, object);   // 00604A20..00604A23
}

// ============================================================================================ creature state
// 0060B380 CSWCCreature::SetMovingOrientation(Vector const*) RET 4: [+1C4h] = normalize(v).
void __fastcall recovered_client_creature_set_moving_orientation_0060b380(void* self, void*, const Vector3* v) noexcept(false) {
    Vector3 tmp;                                                       // {E-0xc}
    const Vector3* n = vector_normalized_004ab180(&tmp, v);            // 0060B380..0060B390
    st<Vector3>(self, 0x1C4, *n);                                      // 0060B395..0060B3AB
}
// (t2, coordinator decision after OVERLAP-W4.md) 0060B3C0 SetStationaryOrientation dropped: single owner lane-w4 `kotor_fastcall_0060b3c0`.
// (t2, coordinator decision after OVERLAP-W4.md) 0060EF50 SetInParty (and with it the W3 calls of 006364C0 / 00635480 / 006349F0) dropped: single owner lane-w4 `kotor_fastcall_0060ef50`.

// 0066DCC0 CExoArrayList<int>::remove: single owner lane-w1 (recovered_int_list_remove_0066dcc0, lane-w1/tiers/tier4,
// coordinator ownership ruling). The lane-w3 body was dropped; W1's body was compared and is equivalent.

// (t2, coordinator decision after OVERLAP-W4.md) 0063DE50 GetVisualEffectAlpha (and with it the W3 calls of 006A0920 / 005E9BD0) dropped: single owner lane-w4 `kotor_fastcall_0063de50`.
// (t2, coordinator decision after OVERLAP-W4.md) 0063DEE0 GetFinalFadeAlpha dropped: single owner lane-w4 `kotor_fastcall_0063dee0`.

// 0063E220 CSWCObject::SetFadeState(BYTE visible) RET 4.
void __fastcall recovered_client_object_set_fade_state_0063e220(void* self, void*, U visible_word) noexcept(false) {
    if (self != nullptr && rd32(self, 0xE4) == 1) return;              // 0063E223..0063E22E
    const U8 visible = static_cast<U8>(visible_word);                  // 0063E235
    st<U8>(self, 0x90, visible);                                       // 0063E239
    const U8 option = ld<U8>(kotor_fastcall_005ed700(client_app(), nullptr), 0x6C);   // 0063E23F..0063E250
    void* const anim = rp(self, 0x68);                                 // 0063E253
    if (visible == 1) {                                                // 0063E24D..0063E256
        if (anim != nullptr) {                                         // 0063E258..0063E25A
            if (option == 2) {                                         // 0063E25C..0063E264 -> 0063E283
                vslot<SlotU>(self, 0x110)(self, nullptr); vslot<SlotU>(self, 0x108)(self, nullptr);
            } else if (option == 1) {                                  // 0063E266..0063E281
                const U mine = rd32(self, 4);
                if (mine == static_cast<U>(kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr))) {
                    vslot<SlotU>(self, 0x110)(self, nullptr); vslot<SlotU>(self, 0x108)(self, nullptr);   // 0063E283..0063E28D
                } else {
                    vslot<SlotU>(self, 0x114)(self, nullptr); vslot<SlotU>(self, 0x104)(self, nullptr);   // 0063E299..0063E2A3
                }
            } else {
                vslot<SlotU>(self, 0x114)(self, nullptr); vslot<SlotU>(self, 0x104)(self, nullptr);       // 0063E295..0063E2A3
            }
        }
        st<U>(self, 0xD4, 1);                                          // 0063E2A9
        st<U8>(self, 0x91, 1);                                         // 0063E2B3
    } else {
        if (anim != nullptr) {                                         // 0063E2BC..0063E2BE
            if (option == 1)                                           // 0063E2C4..0063E2D4 (player id fetched, unused)
                (void)kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr);
            if (option == 1 || option == 2) {                          // 0063E2C0..0063E2E7
                vslot<SlotU>(self, 0x114)(self, nullptr); vslot<SlotU>(self, 0x108)(self, nullptr);
            } else {
                vslot<SlotU>(self, 0x114)(self, nullptr); vslot<SlotU>(self, 0x104)(self, nullptr);       // 0063E2EF..0063E2FD
            }
        }
        st<U>(self, 0xD4, 0);                                          // 0063E303
        st<U8>(self, 0x91, 0);                                         // 0063E30D
    }
    st<float>(self, 0x98, kotor_fastcall_0063dee0(self, nullptr));   // 0063E314..0063E31B
    void* const anim_now = rp(self, 0x68);                             // 0063E321
    if (anim_now != nullptr)                                           // 0063E324..0063E328
        vslot<void (__fastcall*)(void*, void*, U, U)>(anim_now, 0x94)(anim_now, nullptr, rd32(self, 0x98), 0xFF);   // 0063E32A..0063E338
}

// 0063CE60 CSWCObject::SetDesiredFadeState(BYTE visible, BOOL timed, DWORD a, DWORD b) RET 10h.
void __fastcall recovered_client_object_set_desired_fade_state_0063ce60(void* self, void*, U visible_word, U timed, U a,
                                                                        U b) noexcept(false) {
    if (self != nullptr && rd32(self, 0xE4) == 1) return;              // 0063CE63..0063CE6E
    const U8 visible = static_cast<U8>(visible_word);                  // 0063CE83
    st<U8>(self, 0x91, visible);                                       // 0063CE88
    st<U>(self, 0x94, timed);                                          // 0063CE8E
    st<U>(self, 0x88, a);                                              // 0063CE94
    st<U>(self, 0x8C, b);                                              // 0063CE9A
    if (timed != 0)                                                    // 0063CE78..0063CEA0
        frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, at(self, 0x80),
                                    at(self, 0x84));                   // 0063CEA2..0063CEBF
    const U8 option = ld<U8>(kotor_fastcall_005ed700(client_app(), nullptr), 0x6C);   // 0063CEC4..0063CED5
    void* const anim = rp(self, 0x68);                                 // 0063CED8
    if (visible == 1) {                                                // 0063CED2..0063CEDB
        if (anim != nullptr) {                                         // 0063CEDD..0063CEDF
            bool on = option == 2;                                     // 0063CEE1..0063CEE9
            if (option == 1) {                                         // 0063CEEB..0063CF06
                const U mine = rd32(self, 4);
                on = mine == static_cast<U>(kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr));
            }
            if (on) { vslot<SlotU>(self, 0x110)(self, nullptr); vslot<SlotU>(self, 0x108)(self, nullptr); }   // 0063CF08..0063CF12
            else { vslot<SlotU>(self, 0x114)(self, nullptr); vslot<SlotU>(self, 0x104)(self, nullptr); }      // 0063CF28..0063CF36
        }
        st<U>(self, 0xD4, 1);                                          // 0063CF1A / 0063CF3E
    } else {
        if (anim != nullptr) {                                         // 0063CF4C..0063CF4E
            if (option == 1) (void)kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr);   // 0063CF78..0063CF88
            if (option == 1 || option == 2) { vslot<SlotU>(self, 0x114)(self, nullptr); vslot<SlotU>(self, 0x108)(self, nullptr); }   // 0063CF54..0063CF62
            else { vslot<SlotU>(self, 0x114)(self, nullptr); vslot<SlotU>(self, 0x104)(self, nullptr); }   // 0063CF8F..0063CF9D
        }
        st<U>(self, 0xD4, 0);                                          // 0063CF6A / 0063CFA4
    }
}

// 0063E350 CSWCObject::SetObjectVisibilityOnSetArea(CSWCArea*) RET 4 (placeables, doors and items only).
void __fastcall recovered_client_object_set_visibility_on_set_area_0063e350(void* self, void*, void* area) noexcept(false) {
    const U8 type = ld<U8>(self, 8);                                   // 0063E356
    if (type != kPlaceable_00746638 && type != kDoor_00746639 && type != kItem_00746635) return;   // 0063E35F..0063E389
    if (type == kPlaceable_00746638) {                                 // 0063E38F..0063E391
        void* const placeable = vslot<void* (__fastcall*)(void*, void*)>(self, 0x4C)(self, nullptr);   // 0063E393..0063E397 AsSWCPlaceable
        void* const server = kotor_fastcall_00682e00(placeable, nullptr);   // 0063E39A..0063E39C
        if (server != nullptr && rp(server, 0x440) != nullptr && rp(server, 0x444) == nullptr) return;   // 0063E3A1..0063E3B7
    }
    Vector3 position = ld<Vector3>(self, 0x24);                        // 0063E3BD..0063E3D8 {E-0x14}
    const Vector3 origin = {0.0f, 0.0f, 0.0f};                         // 0063E3DC..0063E3EC {E-0x8}
    if (aurora_vector_equal_004aa980(&position, &origin) == 0 &&       // 0063E3D2..0063E3FE
        sound_volume29_required::predicate_006054e0(area, nullptr, &position) != 0) {   // 0063E400..0063E410
        recovered_client_object_set_fade_state_0063e220(self, nullptr, 1);   // 0063E412..0063E416
        recovered_client_object_set_desired_fade_state_0063ce60(self, nullptr, 1, 0, 0, 0);   // 0063E41B..0063E425
        return;
    }
    recovered_client_object_set_fade_state_0063e220(self, nullptr, 0);      // 0063E431..0063E435
    recovered_client_object_set_desired_fade_state_0063ce60(self, nullptr, 0, 0, 0, 0);   // 0063E43A..0063E444
}

// 0063F570 CSWCObject::SetArea(CSWCArea*) RET 4: maintain the area's object-id list [area+20Ch].
void __fastcall recovered_client_object_set_area_0063f570(void* self, void*, void* area) noexcept(false) {
    void* const current = rp(self, 0x20);                              // 0063F573
    if (current != area) {                                             // 0063F57B..0063F57D
        if (current != nullptr) recovered_int_list_remove_0066dcc0(at(current, 0x20C), nullptr, rd32(self, 4));   // 0063F57F..0063F58D
        if (area != nullptr) {                                         // 0063F592..0063F594
            recovered_bridge_dword_array_add_unique_006449d0(at(area, 0x20C), nullptr, rd32(self, 4));   // 0063F596..0063F5A0
            recovered_client_object_set_visibility_on_set_area_0063e350(self, nullptr, area);   // 0063F5A5..0063F5A8
        }
    }
    st<void*>(self, 0x20, area);                                       // 0063F5AD
}

// 00614B20 CSWCCreature::SetArea(CSWCArea*) RET 4 (creature table slot +84h).
void __fastcall recovered_client_creature_set_area_00614b20(void* self, void*, void* area) noexcept(false) {
    recovered_client_object_set_area_0063f570(self, nullptr, area);   // 00614B20..00614B28
    void* const now = rp(self, 0x20);                                  // 00614B2D
    st<U>(self, 0x39C, now != nullptr ? rd32(now, 0x104) : kInvalidObject_00746644);   // 00614B30..00614B4A
}

// 00616FB0 CSWCObject::ShowShadowBlob (slot +104h): anim base [+68h] vtable +ACh (0).
void __fastcall recovered_client_object_show_shadow_blob_00616fb0(void* self, void*) noexcept(false) {
    void* const anim = rp(self, 0x68);                                 // 00616FB0
    if (anim != nullptr) vslot<void (__fastcall*)(void*, void*, U)>(anim, 0xAC)(anim, nullptr, 0);   // 00616FB3..00616FBB
}

namespace {
// 0060E490 / 0060E520 shared: the three equipped item models ([+230h], [+234h], [+220h]) get their CSWCObject
// sub-object (+10h) slot `item_slot`, then the anim base [+68h] slot `anim_slot` (tail jump).
void creature_shadows(void* self, unsigned item_slot, unsigned anim_slot) noexcept(false) {
    static const unsigned kItems[3] = {0x230, 0x234, 0x220};
    for (unsigned k = 0; k < 3; ++k) {
        void* const item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, rd32(self, kItems[k]));
        if (item != nullptr) vslot<Slot0>(at(item, 0x10), item_slot)(at(item, 0x10), nullptr);
    }
    void* const anim = rp(self, 0x68);
    if (anim != nullptr) vslot<Slot0>(anim, anim_slot)(anim, nullptr);
}
}  // namespace
// 0060E490 CSWCCreature::TurnOnShadows (slot +110h).
void __fastcall recovered_client_creature_turn_on_shadows_0060e490(void* self, void*) noexcept(false) {
    creature_shadows(self, 0x110, 0xB8);                               // 0060E493..0060E50C
}
// 0060E520 CSWCCreature::TurnOffShadows (slot +114h).
void __fastcall recovered_client_creature_turn_off_shadows_0060e520(void* self, void*) noexcept(false) {
    creature_shadows(self, 0x114, 0xBC);                               // 0060E523..0060E59C
}

// 00613A70 CSWCCreature::UpdateRelativePerception(OBJECT_ID other) RET 4 (perception VFX 7D1h on the other creature).
void __fastcall recovered_client_creature_update_relative_perception_00613a70(void* self, void*, U other_id) noexcept(false) {
    const I32 count = ld<I32>(self, 0x364);                            // 00613A77
    U hostile = 0, friendly = 0;                                       // {E-0x14} / {E-0x10}
    I32 i = 0;
    void* entry = nullptr;
    for (; i < count; ++i) {                                           // 00613A81..00613AAC
        entry = rp(rp(self, 0x360), static_cast<U>(i) * 4);
        if (rd32(entry, 0) == other_id) break;                         // 00613AA2..00613AA4
    }
    if (i >= count) return;                                            // 00613AAE..00613AB5
    void* const other = frame_R4_005ed5b0(client_app(), nullptr, other_id);   // 00613AB8..00613AC1
    if (other == nullptr) return;                                      // 00613AC8..00613ACA
    const U8 flags = ld<U8>(entry, 4);                                 // 00613AD0
    const U8 seen = flags & 1;                                         // 00613AD3..00613ADA
    if ((seen == 0 && (flags & 2) != 0) || ((flags >> 4) & 1) != 0) { // 00613ADC..00613AE7
        hostile = 1;                                                   // 00613AE9
    } else if (seen == 1 && ((flags >> 4) & 1) == 0) {                 // 00613AEF..00613AF5
        friendly = 1;                                                  // 00613AF7
    }
    if (sound_volume29_required::predicate_006054e0(rp(self, 0x20), nullptr, at(other, 0x24)) != 1) return;   // 00613AFB..00613B09
    if (hostile != 1 || rd32(other, 0x1B0) != 0) {                     // 00613B0F..00613B1D
        if (friendly == 1 && rd32(other, 0x1B0) == 1) {                // 00613B6B..00613B77
            vslot<U (__fastcall*)(void*, void*, U)>(other, 0xB8)(other, nullptr, 0x7D1);   // 00613B79..00613B82 DeleteVisualEffect
            st<U>(other, 0x1B0, 0);                                    // 00613B88
        }
        return;
    }
    Vector3 zero = {0.0f, 0.0f, 0.0f};                                 // {E-0xc}
    using Add = U (__fastcall*)(void*, void*, U, U, U, U, U, U, Vector3*);
    vslot<Add>(other, 0xB4)(other, nullptr, 0x7D1, 0, kInvalidObject_00746644, kInvalidObject_00746644, 0, 0, &zero);   // 00613B1F..00613B55 AddVisualEffect
    st<U>(other, 0x1B0, 1);                                            // 00613B5C
}

// ============================================================================================ message handlers
// 0064C0A0 CSWCMessage::HandleServerToPlayerCreatureUpdate_Add. ECX this, RET. EAX = 1 / 0.
// Record: OBJECT_ID, BYTE in-party, 3 FLOAT position, 3 FLOAT orientation, WORD appearance.
U __fastcall recovered_client_handle_creature_add_0064c0a0(void* self, void*) noexcept(false) {
    Vector3 orientation = {0.0f, 0.0f, 0.0f};                          // 0064C0C1..0064C0D1 {E-0x24}
    void* const player = client_get_player_creature_005ed540(client_app(), nullptr);   // 0064C0B8..0064C0E4 (ebx)
    U existing = 0;                                                    // 0064C0E6 (edi)
    U id = kotor_fastcall_00692630(self, nullptr);   // 0064C0E8..0064C0F1 {E-0x3c}
    const U8 party = read_byte(self);                                  // 0064C0ED..0064C103 {E-0x3d}
    Vector3 position;                                                  // {E-0x38}
    position.x = read_float(self); position.y = read_float(self); position.z = read_float(self);   // 0064C0FA..0064C130
    orientation.x = read_float(self); orientation.y = read_float(self); orientation.z = read_float(self);   // 0064C134..0064C166
    const U16 appearance = kotor_fastcall_004d6750(self, nullptr, 0x10);   // 0064C16A..0064C177 {E-0x2c}
    if (recovered_player_message::overrun_004d6230(self) != 0) return 0;   // 0064C173..0064C197
    void* creature = frame_R4_005ed5b0(client_app(), nullptr, id);     // 0064C198..0064C1A6
    if (creature != nullptr) {                                         // 0064C1AB..0064C1AF already known on this client
        existing = 1;                                                  // 0064C1B3
        st<U>(creature, 0xE4, 0);                                      // 0064C1B8
        if (player != nullptr) recovered_client_creature_update_relative_perception_00613a70(player, nullptr, id);   // 0064C1B1..0064C1CB
        const U inside = sound_volume29_required::predicate_006054e0(rp(creature, 0x20), nullptr, at(creature, 0x24));   // 0064C1D0..0064C1D7
        recovered_client_object_set_desired_fade_state_0063ce60(creature, nullptr, inside == 1 ? inside : 0, 0, 0, 0);   // 0064C1DC..0064C1EE
        st<U>(creature, 0x9C, 1);                                      // 0064C1F3
    }
    void* const area = rp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48);   // 0064C1FD..0064C20D (ebp)
    if (existing == 0) {                                               // 0064C20B..0064C210
        creature = frame_R4_005ed5b0(client_app(), nullptr, id);       // 0064C216..0064C224
        if (creature == nullptr) {                                     // 0064C229..0064C22D
            void* const memory = live_operator_new(0x44C);             // 0064C233..0064C238
            creature = nullptr;                                        // 0064C257
            if (memory != nullptr) creature = kotor_m15a_creature_construct_00616a20(memory, nullptr);   // 0064C244..0064C24E
            if (creature == nullptr) return 0;                         // 0064C259..0064C279
            vslot<void (__fastcall*)(void*, void*, U)>(creature, 0x04)(creature, nullptr, id);   // 0064C27A..0064C283 SetId
            vslot<U (__fastcall*)(void*, void*, void*)>(creature, 0x84)(creature, nullptr, area);   // 0064C286..0064C28B SetArea
            kotor_fastcall_0060ef50(creature, nullptr, party == 1 ? 1u : 0u);   // 0064C291..0064C2A0
            Vector3 where = position;                                  // 0064C2A5..0064C2BD {E-0x18}
            vslot<void (__fastcall*)(void*, void*, const Vector3*)>(creature, 0x8C)(creature, nullptr, &where);   // 0064C2C1..0064C2C6 SetPosition
            vslot<void (__fastcall*)(void*, void*, const Vector3*)>(creature, 0x88)(creature, nullptr, &orientation);   // 0064C2CC..0064C2D5 SetOrientation
            recovered_client_creature_set_moving_orientation_0060b380(creature, nullptr, &orientation);       // 0064C2DB..0064C2E2
            kotor_fastcall_0060b3c0(creature, nullptr, &orientation);   // 0064C2E7..0064C2EE
            st<Vector3>(creature, 0x1DC, where);                       // 0064C2F3..0064C30A
            if (recovered_object_array_add_external_004d7ff0(kotor_fastcall_005ed570(client_app(), nullptr),
                    nullptr, &id, creature, 1) != kGameObjectOk_0074666c)   // 0064C30D..0064C330
                return 0;                                              // 0064C265 (the creature is not released, as in the original)
            recovered_client_ai_master_add_object_00604a20(kotor_m15e_client_get_client_ai_master_005ed6b0(client_app(), nullptr),
                                                           nullptr, creature);   // 0064C336..0064C347
        }
        if (player != nullptr) recovered_client_creature_update_relative_perception_00613a70(player, nullptr, id);   // 0064C34C..0064C357
        recovered_client_object_set_fade_state_0063e220(creature, nullptr, 1);   // 0064C35C..0064C360
        recovered_client_object_set_desired_fade_state_0063ce60(creature, nullptr, 1, 0, 0, 0);   // 0064C365..0064C36F
    }
    vslot<U (__fastcall*)(void*, void*, void*)>(creature, 0x94)(creature, nullptr, rp(area, 0x184));   // 0064C374..0064C37F AttachToScene
    st<U16>(creature, 0x3D4, appearance);                              // 0064C385..0064C390
    st<U>(creature, 0xE4, 0);                                          // 0064C397
    return 1;                                                          // 0064C3A2
}

// 0064CAF0 CSWCMessage::HandleServerToPlayerCreatureUpdate_ControlObject(OBJECT_ID) RET 4: the controlled creature.
U __fastcall recovered_client_handle_creature_control_object_0064caf0(void* self, void*, U id) noexcept(false) {
    void* const app = client_app();                                    // 0064CAF3..0064CAFA (esi)
    recovered_client_set_character_change_in_progress_005ee1a0(app, nullptr, 0);   // 0064CB02..0064CB06
    void* const previous = client_get_player_creature_005ed540(app, nullptr);   // 0064CB0B..0064CB19 (ebx)
    recovered_client_set_player_creature_005ed780(app, nullptr, id);   // 0064CB12..0064CB1B
    if (previous != nullptr) {                                         // 0064CB20..0064CB22
        void* const player = recovered_client_get_player_by_id_005ed590(app, nullptr, rd32(previous, 4));   // 0064CB24..0064CB2A
        if (player != nullptr) st<U>(player, 0x28, id);                // 0064CB2F..0064CB33
    }
    void* const creature = frame_R4_005ed5b0(app, nullptr, id);        // 0064CB36..0064CB3E (edi)
    if (creature == nullptr) return 0;                                 // 0064CB40..0064CB4A (EAX = 0 from the lookup)
    void* const module = kotor_fastcall_005ed530(app, nullptr);        // 0064CB4E..0064CB55 (ebp)
    if (module == nullptr || rp(module, 0x40) == nullptr) return 0;    // 0064CB57..0064CB6C
    if (previous == creature) {                                        // 0064CB6E..0064CB70
        if (kotor_cell_007a2484 != 0) {       // 0064CC4A..0064CC51
            st<U>(creature, 0x314, 1); st<U>(creature, 0x318, 1);      // 0064CC53..0064CC5F
        } else {
            kotor_fastcall_0063f9d0(module, nullptr, rd32(creature, 4), 1);   // 0064CC6E..0064CC76
        }
        return 1;
    }
    (void)recovered_client_get_player_by_id_005ed590(app, nullptr, id); // 0064CB76..0064CB7D (result unused)
    (void)recovered_save_stats::maximum_hp_00647a80(rp(creature, 0x2F8));   // 0064CB82..0064CB88 (result unused)
    kotor_fastcall_005ed720(app, nullptr);                             // 0064CB8D..0064CB8F (lane-w4)
    if (kotor_cell_007a2484 != 0) {           // 0064CB94..0064CB9B
        st<U>(creature, 0x314, 1); st<U>(creature, 0x318, 1);          // 0064CB9D..0064CBA8
    } else {
        kotor_fastcall_0063f9d0(module, nullptr, rd32(creature, 4), 1);   // 0064CBB0..0064CBB8
    }
    void* const area = rp(kotor_fastcall_005ed530(app, nullptr), 0x48); // 0064CBBD..0064CBC4
    if (area != nullptr && (ld<U8>(rp(kotor_fastcall_005ed530(app, nullptr), 0x48), 4) & 8) != 0) {   // 0064CBC7..0064CBD9
        Vector3 zero = {0.0f, 0.0f, 0.0f};                             // {E-0xc}
        using Add = U (__fastcall*)(void*, void*, U, U, U, U, U, U, Vector3*);
        vslot<Add>(creature, 0xB4)(creature, nullptr, 0x138B, 0, kInvalidObject_00746644, kInvalidObject_00746644, 0, 0, &zero);   // 0064CBDB..0064CC11
        if (previous != nullptr)                                       // 0064CC17..0064CC19
            vslot<U (__fastcall*)(void*, void*, U)>(previous, 0xB8)(previous, nullptr, 0x138B);   // 0064CC1B..0064CC24
    }
    recovered_client_send_00677410(self, nullptr, 0xE, 2, nullptr, 0);   // 0064CC2A..0064CC36
    return 1;                                                          // 0064CC3E
}

// 00656BD0 CSWCMessage::HandleServerToPlayerUpdate_Add: BYTE object type selects the per-type Add. RET.
U __fastcall recovered_client_handle_update_add_00656bd0(void* self, void*) noexcept(false) {
    const U8 type = read_byte(self);                                   // 00656BD2..00656BDF
    if (recovered_player_message::overrun_004d6230(self) != 0) return 0;   // 00656BDB..00656BE8
    if (type == kCreature_00746634) return recovered_client_handle_creature_add_0064c0a0(self, nullptr);         // 00656BEA..00656BF6
    if (type == kItem_00746635) return recovered_client_handle_item_add_0064e7b0(self, nullptr, 0, 0);          // 00656BFB..00656C09
    if (type == kTrigger_00746636) return recovered_client_handle_trigger_add_0064f4b0(self, nullptr);          // 00656C11..00656C1D
    if (type == kDoor_00746639) return recovered_client_handle_door_add_0064cf30(self, nullptr);                // 00656C22..00656C2E
    if (type == kAreaOfEffect_0074663a) return recovered_client_handle_aoe_add_00651680(self, nullptr);         // 00656C33..00656C3F
    if (type == kPlaceable_00746638) return recovered_client_handle_placeable_add_0064d500(self, nullptr);      // 00656C44..00656C50
    return 0;                                                          // 00656C55..00656C59
}

// 00656C90 CSWCMessage::HandleServerToPlayerUpdate_Delete. RET.
U __fastcall recovered_client_handle_update_delete_00656c90(void* self, void*) noexcept(false) {
    const U8 type = read_byte(self);                                   // 00656C92..00656C9F
    if (recovered_player_message::overrun_004d6230(self) != 0) return 0;   // 00656C9B..00656CA8
    if (type == kCreature_00746634) return recovered_client_handle_creature_delete_0064cc90(self, nullptr);      // 00656CAA..00656CB6
    if (type == kItem_00746635) return recovered_client_handle_item_delete_0064eac0(self, nullptr);             // 00656CBB..00656CC7
    if (type == kTrigger_00746636) return recovered_client_handle_trigger_delete_0064f960(self, nullptr);       // 00656CCC..00656CD8
    if (type == kDoor_00746639) return recovered_client_handle_door_delete_0064d3a0(self, nullptr);             // 00656CDD..00656CE9
    if (type == kAreaOfEffect_0074663a) return recovered_client_handle_aoe_delete_006517b0(self, nullptr);      // 00656CEE..00656CFA
    if (type == kPlaceable_00746638) return recovered_client_handle_placeable_delete_0064da20(self, nullptr);   // 00656CFF..00656D0B
    return 0;                                                          // 00656D10..00656D14
}

// 00656C60 CSWCMessage::HandleServerToPlayerUpdate_Appearance: creatures only. RET.
U __fastcall recovered_client_handle_update_appearance_00656c60(void* self, void*) noexcept(false) {
    const U8 type = read_byte(self);                                   // 00656C62..00656C6F
    if (recovered_player_message::overrun_004d6230(self) != 0) return 0;   // 00656C6B..00656C78
    if (type == kCreature_00746634) return recovered_client_handle_creature_appearance_00655e40(self, nullptr);   // 00656C7A..00656C86
    return 0;                                                          // 00656C8B..00656C8F
}

// 0066A2A0 CSWCMessage::HandleServerToPlayerUpdate_Update: BYTE type, OBJECT_ID, DWORD mask. RET.
U __fastcall recovered_client_handle_update_update_0066a2a0(void* self, void*) noexcept(false) {
    const U8 type = read_byte(self);                                   // 0066A2A4..0066A2AF (bl)
    const U id = kotor_fastcall_00692630(self, nullptr);   // 0066A2AD..0066A2BA (edi)
    const U mask = kotor_fastcall_004d6780(self, nullptr, 0x20);       // 0066A2B6..0066A2C5 (ebp)
    if (recovered_player_message::overrun_004d6230(self) != 0) return 0;   // 0066A2C1..0066A2CE
    if (type == kCreature_00746634) return recovered_client_handle_creature_update_006655e0(self, nullptr, id, mask);   // 0066A2D4..0066A2E9
    if (recovered_client_handle_generic_object_update_00651880(self, nullptr, id, mask) == 0) return 0;   // 0066A2EA..0066A2F1
    if (type == kItem_00746635) return recovered_client_handle_item_update_0064eb90(self, nullptr, id, mask);           // 0066A2F3..0066A308
    if (type == kTrigger_00746636) return recovered_client_handle_trigger_update_0064f430(self, nullptr, id, mask);     // 0066A309..0066A31E
    if (type == kDoor_00746639) return recovered_client_handle_door_update_0064d420(self, nullptr, id, mask);           // 0066A31F..0066A334
    if (type == kAreaOfEffect_0074663a) return recovered_client_handle_aoe_update_006517f0(self, nullptr, id, mask);    // 0066A335..0066A34A
    if (type == kPlaceable_00746638) return recovered_client_handle_placeable_update_0064dae0(self, nullptr, id, mask); // 0066A34B..0066A360
    return 0;                                                          // 0066A361..0066A367
}

// 0066A4A0 CSWCMessage::HandleServerToPlayerGameObjectUpdate(BYTE minor). ECX this, RET 4. EAX = handled.
U __fastcall recovered_client_handle_game_object_update_0066a4a0(void* self, void*, U minor) noexcept(false) {
    if (recovered_player_message::overrun_004d6230(self) != 0) return 0;   // 0066A4A3..0066A4AE
    switch (static_cast<U8>(minor)) {                                  // 0066A4B4..0066A4C3 (table 0066A5E0)
    case 1:                                                            // 0066A4CA object update stream
        while (kotor_fastcall_004d6250(self, nullptr) != 0) {   // 0066A4CA..0066A4D3 / 0066A556..0066A55F
            const U8 kind = read_byte(self);                           // 0066A4E0..0066A4ED
            if (recovered_player_message::overrun_004d6230(self) != 0) return 0;   // 0066A4E9..0066A4F6
            switch (kind) {                                            // 0066A4FC..0066A512 (byte table 0066A620)
            case 0x41: recovered_client_handle_update_add_00656bd0(self, nullptr); break;          // 'A' 0066A519
            case 0x44: recovered_client_handle_update_delete_00656c90(self, nullptr); break;       // 'D' 0066A52B
            case 0x47: recovered_client_handle_update_gui_elements_0066a1f0(self, nullptr); break; // 'G' 0066A546
            case 0x49: recovered_client_handle_update_player_info_00668bb0(self, nullptr); break;  // 'I' 0066A53D
            case 0x50: recovered_client_handle_update_appearance_00656c60(self, nullptr); break;   // 'P' 0066A534
            case 0x55: recovered_client_handle_update_update_0066a2a0(self, nullptr); break;       // 'U' 0066A522
            case 0x57: recovered_client_handle_update_work_remaining_0064dcf0(self, nullptr); break;   // 'W' 0066A54F
            default: return 0;                                         // 0066A5D5
            }
        }
        return 1;                                                      // 0066A565..0066A56D
    case 2: {                                                          // 0066A570 control object
        const U id = kotor_fastcall_00692630(self, nullptr);   // 0066A570..0066A57B
        if (recovered_player_message::overrun_004d6230(self) != 0) return 0;   // 0066A577..0066A584
        if (recovered_player_message::underrun_004d6240(self) != 0) return 0;  // 0066A586..0066A591
        return recovered_client_handle_creature_control_object_0064caf0(self, nullptr, id);   // 0066A593..0066A596
    }
    case 3: return recovered_client_handle_creature_visual_effect_0064c3c0(self, nullptr);         // 0066A5A1
    case 6: return recovered_client_handle_creature_strip_equipped_items_00651af0(self, nullptr);  // 0066A5AE
    case 7: return recovered_client_handle_item_destroy_00651b30(self, nullptr);                   // 0066A5BB
    case 8: return recovered_client_handle_item_script_add_00654b10(self, nullptr);                // 0066A5C8
    default: return 0;                                                 // 0066A5D5
    }
}
}  // extern "C"
