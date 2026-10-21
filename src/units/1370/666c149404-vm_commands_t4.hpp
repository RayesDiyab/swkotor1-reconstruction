#pragma once
// W6 tier t4: ACTION bodies and item-acquisition leaves (claims recorded in lane-w7/OWNERSHIP.md).
//   0052E570 ExecuteCommandApplyEffectToObject (220)     0052DCD0 ExecuteCommandAddAvailableNPCByTemplate (697)
//   005424E0 ExecuteCommandSetAreaUnescapable (14)       0053F160 ExecuteCommandIsObjectPartyMember (576)
//   005158E0 CSWSCreature::AcquireItem   00584B10 CSWSPlaceable::AcquireItem   005C7610 CSWSStore::AcquireItem
// Existing owners stay ports: ApplyEffect 004D14F0 (0886/1031), the CGameEffect destructor 00503590 (W1),
// GetGameObject/GetItemByGameObjectID/GetCreatureByGameObjectID, CItemRepository::AddItem 0055D330 (1088),
// CSWSItem::SetPossessor 00553210 (1013) / RemoveFromArea 005538A0 (1134), GetItemRepository 004EF770 (1086),
// GetBaseItem 005B31A0, CSWSModule::GetArea 004C30F0. Callees with no callable live provider are named-trap imports.
#include "8f2a5e8ea0-server_mainloop.hpp"
namespace kotor::vm_commands_t4 {
using server_mainloop::Word;using server_mainloop::Memory;using I32=std::int32_t;
constexpr I32 kPushFailed=-2000,kPopFailed=-2001;
constexpr Word kInvalid=0x7f000000;
struct Ports{
 virtual ~Ports()=default;
 virtual void write16(Word,Word)=0;
 // VM
 virtual Word pop_integer(Word* out)=0;
 virtual Word pop_float(Word* out)=0;
 virtual Word pop_object(Word* out)=0;
 virtual Word pop_string(Word dst)=0;
 virtual Word pop_engine_structure(Word type,Word* out)=0;  // 005D10A0
 virtual Word push_integer(Word)=0;
 // server
 virtual Word server_app()=0;
 virtual Word object_array_004aed70(Word server)=0;   // CServerExoApp::GetObjectArray (W2 export)
 virtual Word get_game_object_004d8230(Word array,Word id,Word* out)=0;   // status byte (0 = found)
 virtual Word get_game_object_004ae750(Word server,Word id)=0;
 virtual Word get_item_004ae760(Word server,Word id)=0;
 virtual Word get_creature_by_id_004ae770(Word server,Word id)=0;
 virtual Word get_module_004ae6b0(Word server)=0;
 virtual Word module_get_area_004c30f0(Word module)=0;
 virtual Word party_add_npc_005645f0(Word party,Word npc,Word template_string)=0;
 virtual Word object_virtual(Word object,Word offset)=0;      // AsX() thiscall, no arguments
 virtual void effect_apply_004d14f0(Word object,Word effect,Word a,Word b)=0;
 virtual void effect_destroy_00503590(Word effect)=0;
 virtual void free_006fa390(Word)=0;
 virtual void string_default_005b3190(Word)=0;
 virtual void string_destroy_005e5c20(Word)=0;
 // items
 virtual Word rules()=0;
 virtual Word base_item_005b31a0(Word array,Word base)=0;
 virtual Word creature_item_repository_004ef770(Word creature,Word shared)=0;
 virtual Word repository_add_item_0055d330(Word repository,Word item_cell,Word a,Word b,Word c)=0;
 virtual void repository_remove_item_00555fd0(Word repository,Word item)=0;
 virtual Word repository_find_item_with_tag_00555ed0(Word repository,Word tag)=0;
 virtual void creature_remove_item_00510ef0(Word creature,Word item,Word a,Word b,Word c)=0;
 virtual void placeable_remove_item_00584ac0(Word placeable,Word item)=0;
 virtual void item_remove_from_area_005538a0(Word item)=0;
 virtual void item_set_possessor_00553210(Word item,Word owner,Word a,Word b,Word previous)=0;
 virtual void item_set_identified_00554170(Word item,Word value)=0;
 virtual Word item_compare_00553cf0(Word item,Word other)=0;
 virtual void item_delete_virtual(Word item)=0;
 virtual Word store_add_item_to_inventory_005c70c0(Word store,Word item_cell)=0;
};
I32 apply_effect_to_object_0052e570(Memory&,Ports&);
I32 add_available_npc_by_template_0052dcd0(Memory&,Ports&,Word string_scratch);
I32 set_area_unescapable_005424e0(Memory&,Ports&);
I32 is_object_party_member_0053f160(Memory&,Ports&);
Word creature_acquire_item_005158e0(Memory&,Ports&,Word self,Word item_cell,Word container,Word repository_owner,Word flag);
Word placeable_acquire_item_00584b10(Memory&,Ports&,Word self,Word item_cell,Word container,Word flag);
Word store_acquire_item_005c7610(Memory&,Ports&,Word self,Word item_cell);
}
