#pragma once
// W6 tier t3: ACTION bodies assigned by the coordinator (critical path).
//   005313F0 ExecuteCommandEffectComplex: routines 51 EffectAssuredHit (first), 130,133,134,148,154,157,158,161,164,
//            252,270,402,420,460,463,467,473,754,755,756 (one shared handler; full original dispatch)
//   0052F570 ExecuteCommandCreateItemOnObject (31)
//   00547240 ExecuteCommandRevealMap (515)
//   0053ED70 ExecuteCommandIsAvailableNPC (696 IsAvailableCreature; 709 shares the handler)
//   0053F220 ExecuteCommandItemActivated (440 GetItemActivator; 439/441/442 share the handler)
// plus the leaf CGameEffect::SetObjectID 005036F0 (no live owner).
// Every other callee is an existing owner reached through Ports. In the native object, a callee without a live
// provider is an import whose /alternatename fallback is a named trap at its original address.
#include "8f2a5e8ea0-server_mainloop.hpp"
namespace kotor::vm_commands_t3 {
using server_mainloop::Word;using server_mainloop::Memory;using I32=std::int32_t;
constexpr I32 kPushFailed=-2000,kPopFailed=-2001;
constexpr Word kInvalid=0x7f000000;
struct Ports{
 virtual ~Ports()=default;
 virtual void write8(Word,Word)=0;
 virtual void write16(Word,Word)=0;
 // VM (CVirtualMachine at [007A3A00])
 virtual Word pop_integer(Word* out)=0;              // 005D1000
 virtual Word pop_float(Word* out)=0;                // 005D1020
 virtual Word pop_object(Word* out)=0;               // 005D10C0
 virtual Word pop_string(Word dst)=0;                // 005D1080
 virtual Word pop_vector(Word out12)=0;              // 005D1040
 virtual Word push_integer(Word)=0;                  // 005D1010
 virtual Word push_object(Word)=0;                   // 005D10D0
 virtual Word push_engine_structure(Word type,Word value)=0; // 005D10B0
 // server
 virtual Word server_app()=0;                        // [[007A39FC]+8] CServerExoApp
 virtual Word object_array_004aed70(Word server)=0;   // CServerExoApp::GetObjectArray (W2 export)
 virtual Word client_app()=0;                        // [[007A39FC]+4] CClientExoApp
 virtual Word get_module_004ae6b0(Word server)=0;
 virtual Word get_game_object_004d8230(Word array,Word id,Word* out)=0; // returns status byte (0 = found)
 virtual Word get_creature_by_id_004ae770(Word server,Word id)=0;
 virtual Word party_table_004aee70(Word server)=0;
 virtual Word npc_available_005636b0(Word party,Word npc)=0;
 virtual Word npc_selectable_005637c0(Word party,Word npc)=0;  // low byte significant (MOVZX AL)
 virtual void map_world_point_explored_005792d0(Word map,Word x,Word y,Word z,Word value)=0;
 virtual void map_entire_explored_00578fa0(Word map)=0;
 virtual Word vector_equal_004aa980(Word a,Word b)=0;
 virtual void vector_normalize_004ab180(Word out,Word in)=0;
 // effects
 virtual Word allocate_006fa7e6(Word)=0;
 virtual void free_006fa390(Word)=0;
 virtual Word effect_construct_00503e40(Word self,Word flag)=0;
 virtual void effect_set_integer_005036a0(Word effect,Word index,Word value)=0;
 virtual void effect_set_creator_00503a00(Word effect,Word id)=0;
 virtual void effect_destroy_00503590(Word effect)=0;
 virtual Word ftol_mul_1000_x87(Word float_bits)=0;  // FLD; FMUL [0073D6FC]=1000.0f; __ftol2
 // items
 virtual void string_default_005b3190(Word)=0;
 virtual void string_destroy_005e5c20(Word)=0;
 virtual Word string_cstr_005e5670(Word)=0;
 virtual void resref_from_chars_00406d80(Word ref,Word chars)=0;
 virtual Word item_construct_005530a0(Word self,Word id)=0;
 virtual Word item_load_template_005608b0(Word item,Word ref)=0;
 virtual void item_delete_virtual(Word item)=0;      // vtbl+00 (scalar deleting dtor, flag 1)
 virtual Word base_item_005b31a0(Word base_item_array,Word base)=0;
 virtual Word object_virtual(Word object,Word offset)=0; // vtbl+30 AsSWSCreature / +48 AsSWSPlaceable / +64 AsSWSStore
 virtual void creature_acquire_item_005158e0(Word creature,Word item_cell,Word a,Word b,Word flag)=0;
 virtual void placeable_acquire_item_00584b10(Word placeable,Word item_cell,Word a,Word flag)=0;
 virtual void store_acquire_item_005c7610(Word store,Word item)=0;
 virtual Word in_game_gui_005ed690(Word client)=0;
 virtual void gui_update_status_0062eeb0(Word gui,Word a,Word b)=0;
 virtual Word rules()=0;                             // [007A3A28]
};
// Scratch: guest storage the original keeps on its stack frame (0x40 bytes is enough for every handler).
struct Frame{Word base;};
void effect_set_object_id_005036f0(Memory&,Word effect,Word index,Word id);
I32 effect_complex_005313f0(Memory&,Ports&,Word implementer,I32 routine,Word argc);
I32 create_item_on_object_0052f570(Memory&,Ports&,Word argc,Frame);
I32 reveal_map_00547240(Memory&,Ports&,Frame);
I32 is_available_npc_0053ed70(Memory&,Ports&,I32 routine);
I32 item_activated_0053f220(Memory&,Ports&,I32 routine,Frame);
}
