#pragma once
// W2 tier 4 (fork B): CSWSPlayerLastUpdateObject lifecycle for the per-frame object update stream.
//   00577000 CSWSMessage::CreateNewLastUpdateObject(player, object, flags*, appearance_flags*) RET 10h
//   00575C80 CSWSMessage::UpdateLastUpdateObject(player, object, last, flags)                 RET 10h
//   00568480 CSWSMessage::UpdateLastUpdateObjectAppearance(object, last, appearance_flags)   RET 0Ch
//   005CAA50 / 005CA6A0 CLastUpdateObject constructor / destructor
// Logic over original 32-bit address words. Float fields travel as IEEE single bits; an x87 FLD/FSTP
// pair is modelled with w2::msg::x87_single_roundtrip (SNaN -> QNaN). Every callee is a LastPorts entry.
// Object type bytes are the original .rdata cells 00746634.. (5 creature, 6 item, 7 trigger, 9 placeable,
// 0Ah door, 0Bh area of effect). Layout of the last-update object: see the table in w2_g07_last.cpp.
#include "913ae857a3-w2_message.hpp"
namespace w2::g07 {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;

struct LastPorts {
    virtual ~LastPorts() = default;
    virtual w2::msg::Ports& msg() = 0;                                       // CSWMessage primitives (W2 / 0898)
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    virtual Word app_007a39fc() = 0;
    virtual Word allocate_006fa7e6(Word bytes) = 0;
    virtual void free_006fa390(Word block) = 0;                              // live_free
    virtual void ref_default_00405ed0(Word ref) = 0;                          // CResRef::CResRef()
    virtual void loc_construct_005e9e10(Word loc) = 0;                       // CExoLocString::CExoLocString()
    virtual void loc_destroy_005ea000(Word loc) = 0;                         // CExoLocString::~CExoLocString
    virtual void str_empty_005b3190(Word str) = 0;                           // CExoString::CExoString()
    virtual void str_destroy_005e5c20(Word str) = 0;                         // CExoString::~CExoString
    virtual void update_appearance_equipped_004ed620(Word creature) = 0;     // CSWSCreature::UpdateAppearanceForEquippedItems
    virtual Word game_object_00561080(Word player) = 0;                      // CSWSPlayer::GetGameObject
    virtual Word vcall0(Word object, Word slot) = 0;                         // thiscall [[o]+slot](o)
    virtual Word vcall1(Word object, Word slot, Word a) = 0;                 // thiscall [[o]+slot](o, a)
    virtual void write_locstring_005785c0(Word self, Word loc, Word gender) = 0;   // W2 WriteCExoLocStringServer
    virtual void add_item_appearance_00567090(Word self, Word item) = 0;
    virtual void add_active_item_properties_00566f20(Word self, Word item, Word creature) = 0;
    virtual Word reputation_0057cb80(Word object, Word id, Word flag) = 0;   // CSWSObject::GetReputation
    virtual Word door_is_linked_005890d0(Word door) = 0;                     // W5
    virtual void list_add_head_005e9d70(Word list, Word node) = 0;           // CExoLinkedListInternal::AddHead
    virtual Word object_area_004cb120(Word object) = 0;
    virtual void resref_assign_00406120(Word self, Word result, Word source) = 0;
    virtual Word ai_state_reputation_0057cd90(Word creature, Word id) = 0;
    virtual Word animation_stationary_004cae60(Word object, Word animation) = 0;
    virtual Word walk_rate_bits_004f1b20(Word creature) = 0;                 // ST0 -> m32
    virtual Word run_rate_bits_004f1be0(Word creature) = 0;                  // ST0 -> m32
    virtual Word damage_level_004cb020(Word object) = 0;                     // AL
    virtual Word max_force_points_004fd490(Word creature) = 0;               // AX
    virtual Word faction_00513fc0(Word creature) = 0;
    virtual Word singleton_party_005bf690(Word faction) = 0;
    virtual Word faction_leader_005bf5c0(Word faction) = 0;
    virtual Word module_004ae6b0(Word server) = 0;
    virtual Word is_beam_004ca9d0(Word effect) = 0;                          // W2 w2_g07_object (main)
    virtual Word detect_mode_004edae0(Word creature) = 0;                    // AL; @recovered_creature_detect_mode_004edae0@8
    virtual w2::msg::LocPorts& loc_ports() = 0;                              // list ports for W2 player_language_004b4050
    virtual Word loc_get_string_005ea130(Word loc, Word language, Word out, Word gender) = 0;   // @exo_locstring_get_005ea130@20
    virtual void str_assign_text_005e5140(Word str, Word text) = 0;          // CExoString::operator=(const char*)
    virtual Word str_concat_005e5d10(Word self, Word result, Word rhs) = 0;  // CExoString::operator+ -> result
    virtual Word space_text_0073d5d8() = 0;                                  // original " " literal
    virtual Word array_count_004af240(Word list, Word value) = 0;            // CExoArrayList::Count
    virtual Word visible_list_element_00517a20(Word creature, Word id) = 0;
    virtual void str_assign_005e5c50(Word dst, Word src) = 0;                // CExoString::operator=
    virtual Word creature_class_005a4e90(Word stats, Word index) = 0;        // AL
    virtual Word class_level_005a5090(Word stats, Word index, Word flag) = 0;// AL
    virtual Word str_is_empty_005e58a0(Word str) = 0;                       // @frame_LogIsEmpty_005e58a0@8 (existing owner)
    virtual void loc_assign_005e9e70(Word dst, Word src) = 0;                // CExoLocString::operator=
};

// 005CAA50 CLastUpdateObject::CLastUpdateObject() RET -> self.
Word last_construct_005caa50(Memory&, LastPorts&, Word self);
// 005CA6A0 CLastUpdateObject::~CLastUpdateObject() RET (does not free the object itself).
void last_update_destroy_005ca6a0(Memory&, LastPorts&, Word self);
// 00566DA0 AddDoorAppearanceToMessage(door) RET 4.
void add_door_appearance_00566da0(Memory&, LastPorts&, Word self, Word door);
// 00567C20 AddTriggerGeometryToMessage(trigger) RET 4.
void add_trigger_geometry_00567c20(Memory&, LastPorts&, Word self, Word trigger);
// 00566DF0 AddPlaceableAppearanceToMessage(placeable) RET 4. scratch >= 0x40 (+0 CExoString, +8 CExoLocString, +10 for 00578870).
void add_placeable_appearance_00566df0(Memory&, LastPorts&, Word self, Word placeable, Word scratch);
// 00570530 AssignVisualEffectLists(dst list*, src list*) RET 8. Lists are CExoArrayList {data, count, capacity}.
void assign_visual_effect_lists_00570530(Memory&, LastPorts&, Word self, Word dst, Word src);
// 005706F0 AssignCreatureLists(src list*, dst list*) RET 8.
void assign_creature_lists_005706f0(Memory&, LastPorts&, Word self, Word src, Word dst);
// 005676B0 ComputeLastUpdate_AssociateState(creature) RET 4 -> AL.
Word associate_state_005676b0(Memory&, LastPorts&, Word self, Word creature);
// 00578870 CSWSMessage::GetLocStringServer(player, loc, loc2, CExoString* out, gender) cdecl -> 0 when no server, else 1.
// out = loc + " " + loc2 when both are non-empty, else loc + loc2 (player's language). scratch >= 0x2C.
Word get_locstring_server_00578870(Memory&, LastPorts&, Word player, Word loc, Word loc2, Word out, Word gender, Word scratch);
// 005E58A0 CExoString::IsEmpty.
// 00577000. Writes the 'A' (41h) record for the object into `self` and links the new last-update object at
// the head of [[player+0C]]. scratch >= 0x40 (handed to 00575C80 and 00566DF0). Returns the new last-update object.
Word create_new_last_00577000(Memory&, LastPorts&, Word self, Word player, Word object, Word flags_out, Word appearance_out, Word scratch);
// 00575C80. scratch >= 0x20 (+00 CResRef from vtable C8h, +10 CResRef operator= result).
void update_last_00575c80(Memory&, LastPorts&, Word self, Word player, Word object, Word last, Word flags, Word scratch);
// 00568480 (self unused by the original).
void update_last_appearance_00568480(Memory&, LastPorts&, Word object, Word last, Word appearance_flags);
}
