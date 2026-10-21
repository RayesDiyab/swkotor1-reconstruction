#pragma once
// Private ACTION9 clear9 cleanup cluster (original 004FAB00/004F3690/004F7530/
// 004D3BA0/004EF4D0/004F8280/004D3580/004F0420/004F06D0/0056B380/004F0310/
// 004F05D0/0056FB70/0056FBD0/0056FCB0 plus the two arg4==0 combat helpers
// 004D3440/004D3C30/004D3CE0).  Source-only: this lane compiles and links nothing.
//
// Conventions (same as the accepted W1/W2/W5 providers):
//  * original __thiscall entries are exported as extern "C" __fastcall(self, unused_edx, ...)
//  * all object access is by the original byte offsets; no original address is dereferenced
//  * memory comes only from live_operator_new / live_free
//  * every callee is an accepted selected provider symbol (or a body in this file)
#include <cstdint>
#include <cstring>

namespace clear9 {
using U = std::uint32_t;
using I = std::int32_t;
using U16 = std::uint16_t;
using U8 = std::uint8_t;

template <class T> inline T& at(void* object, U offset) {
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset);
}
template <class T> inline T vcall_at(void* self, U slot) {
    return reinterpret_cast<T>(at<void*>(*static_cast<void**>(self), slot));
}
// vtable call through byte offset `slot`; thiscall, EDX unused.
template <class R, class... A> inline R vcall(void* self, U slot, A... args) {
    using F = R(__fastcall*)(void*, void*, A...);
    return reinterpret_cast<F>(at<void*>(*static_cast<void**>(self), slot))(self, nullptr, args...);
}
inline void* word(U value) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(value)); }
inline U uword(void* value) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(value)); }
// __ftol2: truncate to signed 64 bits, keep the low dword; NaN/Inf/out-of-range give 0.
inline I ftol2_low(double value) {
    if (!(value >= -9223372036854775808.0 && value < 9223372036854775808.0)) return 0;
    const long long truncated = static_cast<long long>(value);
    return static_cast<I>(static_cast<U>(truncated));
}
}  // namespace clear9

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace menu_tables_m41 { namespace cells { extern const unsigned invalid_object_00746644; } }
namespace kotor_game_prepare {
void __fastcall ResetPartyMemberData004eae80(void*, void*);   // 004EAE80 thiscall RET
}

extern "C" {
using clear9::U; using clear9::I;
using clear9::uword; using clear9::word;

void* __cdecl live_operator_new(std::uint32_t size);  // 006FA7E6
void __cdecl live_free(void* block);                  // 006FA390

// selected W1/W2/W5/0530/0595 providers
void* __fastcall recovered_item_lookup_004ae760(void*, void*, U);                 // 004AE760 @12
void* __fastcall recovered_placeable_lookup_004ae7a0(void*, void*, U);            // 004AE7A0 @12
void* __fastcall server_get_module_004ae6b0(void*, void*);                         // 004AE6B0 @8
void* __fastcall kotor_fastcall_004ae910(void*, void*, U);                         // 004AE910 @12
void* __fastcall kotor_fastcall_004ae920(void*, void*, U, U);                      // 004AE920 @16
void* __fastcall recovered_server_sws_message_004aedb0(void*, void*);              // 004AEDB0 @8
void* __fastcall kotor_fastcall_004b2a50(void*, void*, U);                         // 004B2A50 @12

void __fastcall recovered_creature_feedback_004ede10(void*, void*, U, void*);      // 004EDE10 @16
U    __fastcall recovered_inventory_has_item_005a4950(void*, void*, void*);        // 005A4950 @12
void* __fastcall recovered_equipment_item_005a4c20(void*, void*, U);               // 005A4C20 @12
U    __fastcall recovered_inventory_unequip_cancel_message_0056fd80(void*, void*, U, U); // 0056FD80 @16
I    __fastcall recovered_party_index_006349c0(void*, void*, void*);               // 006349C0 @12
U    __fastcall recovered_creature_can_unequip_weapon_00513e30(void*, void*, void*); // 00513E30 @12

U    __fastcall kotor_fastcall_005eda50(void*, void*, U);                          // 005EDA50 @12
void* __fastcall kotor_fastcall_005ed8b0(void*, void*);                            // 005ED8B0 @8
void* __fastcall frame_R4_005ed690(void*, void*);                                  // 005ED690 @8
void __fastcall kotor_fastcall_0062ec60(void*, void*, U);                          // 0062EC60 @12

void __fastcall w5_creature_set_combat_mode_0050ee80(void*, void*, U, U);          // 0050EE80 @16
void __fastcall w5_creature_set_lock_orientation_to_object_004f34a0(void*, void*, U, U); // 004F34A0 @16

// 004CCB00 / 004CEA20: 26 action parameter pairs, RET 0x78 (120 bytes)
U __fastcall recovered_object_add_action_to_front_004ccb00(void*, void*, U type, U group,
    U t0, void* v0, U t1, void* v1, U t2, void* v2, U t3, void* v3, U t4, void* v4,
    U t5, void* v5, U t6, void* v6, U t7, void* v7, U t8, void* v8, U t9, void* v9,
    U t10, void* v10, U t11, void* v11, U t12, void* v12);
U __fastcall recovered_object_add_action_004cea20(void*, void*, U type, U group,
    U t0, void* v0, U t1, void* v1, U t2, void* v2, U t3, void* v3, U t4, void* v4,
    U t5, void* v5, U t6, void* v6, U t7, void* v7, U t8, void* v8, U t9, void* v9,
    U t10, void* v10, U t11, void* v11, U t12, void* v12);

// selected list / action-node helpers
void* __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);             // 005E9B60 @12
void* __fastcall unresolved_list_next_005e9b90(void*, void*, void**);              // 005E9B90 @12
void __fastcall c2da_005e9c10(void*, void*, void*);                                // 005E9C10 @12
void* __fastcall resource_append_005e9dc0(void*, void*, void*);                    // 005E9DC0 @12
void __fastcall kotor_m15e_slot090_void0_004015a0(void*, void*);                   // 004015A0 @8
void* __fastcall kotor_fastcall_004d1d50(void*, void*);                            // 004D1D50 @8

// message writers (selected W2 providers)
void __fastcall kotor_fastcall_004d68a0(void*, void*, U, U, U);                    // 004D68A0 @20
void __fastcall kotor_fastcall_004d68e0(void*, void*, U);                          // 004D68E0 @12
void __fastcall kotor_fastcall_004d6340(void*, void*, U, U);                       // 004D6340 @16
void __fastcall kotor_fastcall_004d62c0(void*, void*, U, U);                       // 004D62C0 @16
U    __fastcall kotor_fastcall_004d6440(void*, void*, void**, U*);                 // 004D6440 @16
U    __fastcall kotor_fastcall_00567e60(void*, void*, U, U, U, void*, U);          // 00567E60 @28
void __fastcall kotor_fastcall_00578570(void*, void*, U);                          // 00578570 @12

void* __fastcall recovered_event_construct_004d7540(void*, void*);                // 004D7540 @8
void __fastcall recovered_event_integer_004d76b0(void*, void*, U, U);              // 004D76B0 @16

// thin 1017-Native facades (patched into the accepted 1017 owner_events source)
U    __fastcall recovered_event_insert_004afdb0(void*, void*, U day, U tick, U source, U target, U kind, void* data);
void __fastcall recovered_event_list_before_005e9ad0(void*, void*, void* data, void* position);

// bodies in this file
U    __fastcall recovered_creature_field_004f8280(void*, void*);
I    __fastcall recovered_combat_round_has_current_action_004d3580(void*, void*);
void __fastcall recovered_combat_round_clear_current_004d3ba0(void*, void*);
void __fastcall recovered_creature_cancel_action_004ef4d0(void*, void*);
U    __fastcall recovered_action_node_clear_004f0310(void*, void*, void* node, U arg1, U arg2, U arg3);
U    __fastcall recovered_action_node_clear_unequip_004f05d0(void*, void*, void* node, U arg1, U arg2, U arg3);
U    __fastcall recovered_object_equip_004f0420(void*, void*, void* node, U arg1, U arg2, U arg3, U arg4, U arg5, U arg6);
U    __fastcall recovered_object_unequip_004f06d0(void*, void*, void* node, U arg1, U arg2, U arg3, U arg4);
void __fastcall recovered_combat_round_cleanup_004f7530(void*, void*);
void __fastcall recovered_creature_feedback_cleanup_004f3690(void*, void*, U);
U    __fastcall recovered_object_action_queue_callback_004fab00(void*, void*, void* action, U mode);
U    __fastcall recovered_action_cancel_message_0056b380(void*, void*, void* object, U flag, U byte_value, U word_value);
U    __fastcall recovered_inventory_item_message_0056fb70(void*, void*, U recipient, U object_id);
U    __fastcall recovered_inventory_slot_message_0056fbd0(void*, void*, U recipient, U object_id, U slot);
U    __fastcall recovered_inventory_equip_message_0056fcb0(void*, void*, U recipient, U object_id);
void __fastcall recovered_combat_round_advance_004d3440(void*, void*, U tick, U flag);
void __fastcall recovered_combat_round_equip_004d3c30(void*, void*, U item_id, U arg1, U arg2, U arg3);
void __fastcall recovered_combat_round_unequip_004d3ce0(void*, void*, U item_id, U arg1, U arg2);
}
