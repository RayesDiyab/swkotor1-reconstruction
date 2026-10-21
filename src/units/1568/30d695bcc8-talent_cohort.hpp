// Shared header for the 25-address talent/VM cohort (lane A).
// Exact C-callable contracts, fixed original field offsets and selected provider
// declarations.  Every provider symbol below is a selected real provider in
// candidate87 (server_harness_1127.map).  The two portable facades
// (kt_get_class_005a4e90 / kt_feat_remaining_uses_005a6680) call the selected
// portable bodies through their designed Memory / AppearancePorts interfaces;
// they add no second semantic body.
#pragma once
#include <cstdint>
#include <cstring>

// Selected reconstruction globals (not original addresses).  These live at
// global scope because their mangled names are part of the selected providers.
namespace frame_control { extern unsigned message_manager_007a3a00; }
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" void* kotor_cell_rules_007a3a28;

namespace kotor_talent {

using U = std::uint32_t;
using I = std::int32_t;

inline U u32(const void* p, U off) { U v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
inline void set_u32(void* p, U off, U v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4); }
inline unsigned short u16(const void* p, U off) { unsigned short v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 2); return v; }
inline void set_u16(void* p, U off, unsigned short v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 2); }
inline unsigned char u8(const void* p, U off) { return static_cast<const unsigned char*>(p)[off]; }
inline void set_u8(void* p, U off, unsigned char v) { static_cast<unsigned char*>(p)[off] = v; }
inline void* vcall0(void* self, U slot) {
    using F = void* (__thiscall*)(void*);
    void** vtable = *reinterpret_cast<void***>(self);
    return reinterpret_cast<F>(vtable[slot / 4])(self);
}
inline void* pv(U v) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(v)); }

// ------------------------------------------------------------------ selected provider contracts
extern "C" {
void* __cdecl live_operator_new(U size);
void __cdecl live_free(void* p);

// CVirtualMachine outer wrappers: self is the outer CVirtualMachine* whose [self] is the VM.
unsigned __fastcall recovered_vm_outer_pop_integer_005d1000(void*, void*, int* out);
unsigned __fastcall recovered_vm_outer_push_integer_005d1010(void*, void*, int value);
unsigned __fastcall recovered_vm_outer_push_float_005d1030(void*, void*, U bits);
unsigned __fastcall recovered_vm_outer_pop_object_005d10c0(void*, void*, U* out);
unsigned __fastcall recovered_vm_outer_push_object_005d10d0(void*, void*, U value);
unsigned __fastcall recovered_vm_outer_pop_engine_structure_005d10a0(void*, void*, int type, void** out);
unsigned __fastcall recovered_vm_outer_push_engine_structure_005d10b0(void*, void*, int type, const void* data);

// object lookup
void* __fastcall recovered_object_array_004aed70(void* server, void*);
void* __fastcall kotor_fastcall_004ae750(void* server, void*, U id);
void* __fastcall kotor_fastcall_004ae770(void* server, void*, U id);
unsigned __fastcall kotor_fastcall_004d8230(void* array, void*, U id, void** out);

// effects
unsigned __fastcall recovered_effect_construct_00503e40(void* effect, void*, U arg);
void __fastcall recovered_effect_destroy_00503590(void* effect, void*);
void __fastcall kotor_fastcall_004015a0(void*, void*);   // trivial destructor / void return

// rules / spells / feats / items
unsigned __fastcall recovered_rules_roll_dice_00550b60(void* rules, void*, int count, int sides);
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* array, void*, I index);
unsigned __fastcall recovered_spell_adjusted_force_points_0059b8a0(void* spell, void*, U player, I caster);
void* __fastcall recovered_equipment_item_005a4c20(void* inventory, void*, U mask);
void* __fastcall recovered_item_property_query_005539c0(void* item, void*, void** out, U type, U subtype);
unsigned __fastcall recovered_item_property_has_00555c90(void* item, void*, U type, U subtype);
void* __fastcall kotor_m15_menu_active_property_00553960(void* item, void*, I index);
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, I index);
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, I id);
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);
unsigned __fastcall recovered_stats_has_feat_005a6630(void* stats, void*, U feat);
unsigned __fastcall recovered_stats_level_005a5fd0(void* stats, void*, U flag);
unsigned __fastcall recovered_stats_has_spell_005a6e70(void* stats, void*, U classIndex, U spell, U arg);
unsigned __fastcall w1x_creature_skill_rank_005aa570(void* stats, void*, int skill, int a2, int a3);
unsigned __fastcall recovered_creature_incapacitated_004ef890(void* creature, void*);
void* __fastcall recovered_creature_inventory_004ef770(void* creature, void*, U flag);
void* __fastcall recovered_object_area_004cb120(void* object, void*);
void* __fastcall recovered_server_info_004aec90(void* app, void*);
void __fastcall recovered_creature_set_last_instant_equip_004efe00(void* creature, void*, U slot, U value);
unsigned __fastcall c2da_current_get_int_00413660(void* table, void*, I row, void* column, int* out);
int __cdecl crt_rand_006fc447();

// CExoArrayList::{Allocate,Add} for the <byte> specialisation (12-byte header).
void __fastcall recovered_byte_list_allocate_0056b7e0(void* list, void*, int capacity);
void __fastcall recovered_byte_list_add_005704f0(void* list, void*, int value);
// generic CExoArrayList<U> Add / Allocate
void __fastcall gui_load_array_add_00671c00(void* list, void*, U value);
void __fastcall recovered_generic_list_allocate_005e03c0(void* mutations, void* list, U capacity);

// CExoString
void __fastcall frame_LogStringCtor_005e5a90(void* str, void*, const char* text);
void __fastcall frame_LogStringDtor_005e5c20(void* str, void*);
const char* __fastcall WindowCStr005e5670(void* str, void*);

// x87 CRT transports (custom ABI: consume the FPU stack).
void __cdecl genuine_x87_pow_006fc1f0();
void __cdecl genuine_x87_ftol_006fae8c();
}

// Selected reconstruction globals are declared at global scope above.

// CScriptTalent (0x18 bytes; the ctor initialises all fields).
struct ScriptTalent {
    I type;          // +00
    I id;            // +04
    unsigned char b8;// +08
    I field0c;       // +0c
    I field10;       // +10
    unsigned char b14;
    unsigned char b15;
};
static_assert(sizeof(ScriptTalent) == 0x18, "CScriptTalent must be 0x18 bytes");
extern "C" unsigned __fastcall recovered_talent_construct_004ca9e0(void* talent, void*);

// CExoArrayList<U>/<byte> layout shared by the owned bodies.
struct ArrayList {
    void* data;      // +00
    I count;         // +04
    I capacity;      // +08
};

// ------------------------------------------------------------------ engine structures
// CGameEffect fixed offsets used by the owned bodies.
struct GameEffect {
    unsigned short type;    // +08
    unsigned short flags;   // +0a
    U field20;              // +20
};

// Engine structure kinds.
enum : int { kEngineEffect = 0, kEngineTalent = 3 };

}  // namespace kotor_talent

// ------------------------------------------------------------------ class / feat portable facades
// These types are global-scope by design: their names are part of the selected
// providers' mangled symbols (kotor::server_mainloop::Memory, w2::g07::AppearancePorts).
namespace kotor {
namespace server_mainloop {
using Word = unsigned;
struct Memory {
    virtual ~Memory() = default;
    virtual Word read(Word) = 0;
    virtual void write(Word, Word) = 0;
    virtual Word read16(Word) = 0;
    virtual void write16(Word, Word) = 0;
    virtual Word read8(Word) = 0;
};
Word __cdecl creature_get_class_005a4e90(Memory&, Word, Word);
Word __cdecl creature_force_class_005be4a0(Word);
}  // namespace server_mainloop
}  // namespace kotor

namespace w2 {
namespace g07 {
using kotor::server_mainloop::Memory;
using kotor::server_mainloop::Word;
struct AppearancePorts {
    virtual ~AppearancePorts() = default;
    virtual Word item_in_slot_005a4c20(Word inventory, Word slot) = 0;
    virtual Word server_info_004aec90(Word app) = 0;
    virtual void set_last_instant_equip_004efe00(Word creature, Word slot, Word value) = 0;
    virtual Word base_item_005b31a0(Word array, Word type) = 0;
    virtual Word rules_007a3a28() = 0;
    virtual Word item_base_item_005b4790(Word item) = 0;
    virtual Word stats_level_005a5fd0(Word stats, Word flag) = 0;
    virtual Word item_property_exists_00555c90(Word item, Word type, Word subtype) = 0;
    virtual Word item_property_by_type_005539c0(Word item, Word out, Word type, Word subtype) = 0;
    virtual Word stats_has_feat_005a6630(Word stats, Word feat) = 0;
    virtual Word rules_feat_00550c00(Word rules, Word feat) = 0;
    virtual Word stats_class_005a4e90(Word stats, Word index) = 0;
    virtual void list_add_00671c00(Word list, Word value) = 0;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
};
Word __cdecl feat_remaining_uses_005a6680(Memory&, AppearancePorts&, Word stats, Word feat);
}  // namespace g07
}  // namespace w2

namespace kotor_rules { void* __fastcall kotor_fastcall_00550c00(void*, void*, unsigned short); }

// Native facades supplied by this tier (provider_adapters.cpp).
extern "C" {
unsigned __fastcall kt_get_class_005a4e90(void* stats, void*, unsigned index);
unsigned __fastcall kt_is_jedi_005be4a0(void*, void*, unsigned classId);
unsigned __fastcall kt_feat_remaining_uses_005a6680(void* stats, void*, unsigned feat);
}

namespace kotor_talent {
// ------------------------------------------------------------------ owned C-callable contracts
// VM handlers: the fixture ABI.  All ten are int __fastcall(self, edx, routine, argc) ret 8.
extern "C" {
int __fastcall recovered_vm_get_hitpoints_00539610(void*, void*, int routine, unsigned argc);
int __fastcall recovered_vm_get_effect_00549180(void*, void*, int routine, unsigned argc);
int __fastcall recovered_vm_get_is_effect_valid_0053ee00(void*, void*, int routine, unsigned argc);
int __fastcall recovered_vm_roll_dice_005420b0(void*, void*, int routine, unsigned argc);
int __fastcall recovered_vm_get_item_in_slot_0053a2c0(void*, void*, int routine, unsigned argc);
int __fastcall recovered_vm_int_to_float_0053eaa0(void*, void*, int routine, unsigned argc);
int __fastcall recovered_vm_get_creature_has_talent_00536ec0(void*, void*, int routine, unsigned argc);
int __fastcall recovered_vm_get_creature_talent_00536fe0(void*, void*, int routine, unsigned argc);
int __fastcall recovered_vm_get_is_talent_valid_00539f90(void*, void*, int routine, unsigned argc);
int __fastcall recovered_vm_talent_access_00544d70(void*, void*, int routine, unsigned argc);
}

// Lower talent bodies.  Exact contracts mirror the original ret-immediates.
extern "C" {
// 005AAA30: CSWSCreatureStats::GetCreatureHasTalent(int type, DWORD id) ret 8.  ECX=stats.
int __fastcall recovered_stats_has_talent_005aaa30(void*, void*, int type, unsigned id);
// 005B23A0: CSWSCreatureStats::GetCreatureTalentRandom(11 stack args) ret 0x2c.  ECX=stats.
int __fastcall recovered_stats_talent_random_005b23a0(void*, void*, I a1, I a2, U a3, U a4, U a5, U a6,
                                                     U a7, U a8, I a9, I a10, I a11);
// 005A5550: GetSpellUsesLeft(spell, classIndex, unused, unused) ret 0x10.  ECX=stats.
int __fastcall recovered_stats_spell_uses_left_005a5550(void*, void*, unsigned spell, unsigned classIndex,
                                                        unsigned a3, unsigned a4);
// 005A68E0: GetIsInSpellLikeAbilityList(id) ret 4.  ECX=stats.
int __fastcall recovered_stats_in_spell_like_005a68e0(void*, void*, unsigned id);
// 005A72E0: GetHasPowerFromItem(id) ret 4.  ECX=stats.
int __fastcall recovered_stats_power_from_item_005a72e0(void*, void*, unsigned id);
// 005AB9E0: random special ability (8 stack args) ret 0x20.
int __fastcall recovered_talent_special_005ab9e0(void*, void*, I a1, I a2, I a3, I a4, I a5, I a6, I a7, I a8);
// 005ABD00: random power from item (9 stack args) ret 0x24.
int __fastcall recovered_talent_power_item_005abd00(void*, void*, I a1, I a2, I a3, I a4, I a5, I a6, I a7, I a8, I a9);
// 005AC370: random feat (7 stack args) ret 0x1c.
int __fastcall recovered_talent_feat_005ac370(void*, void*, I a1, I a2, I a3, I a4, I a5, I a6, I a7);
// 005AFA30: random power (7 stack args) ret 0x1c.
int __fastcall recovered_talent_power_005afa30(void*, void*, I a1, I a2, I a3, I a4, I a5, I a6, I a7);
// 005A5D70: compatible category (a,b) ret 8.
int __fastcall recovered_talent_compatible_005a5d70(void*, void*, unsigned a, unsigned b);
// 005A5DF0: inclusion match (3 stack args) ret 0xc.
int __fastcall recovered_talent_inclusion_005a5df0(void*, void*, I a1, I a2, I a3);
// 005AB980: power suitability (8 stack args) ret 0x20.
int __fastcall recovered_talent_power_suitable_005ab980(void*, void*, I a1, I a2, I a3, I a4, I a5, I a6, I a7, I a8);
// 005AC5D0: random known power (8 stack args) ret 0x20.
int __fastcall recovered_talent_known_power_005ac5d0(void*, void*, I a1, I a2, I a3, I a4, I a5, I a6, I a7, I a8);
// 0056B7E0: CExoArrayList<byte>::Allocate(capacity) ret 4.
void __fastcall recovered_byte_list_allocate_owned_0056b7e0(void*, void*, int capacity);
// 005704F0: CExoArrayList<byte>::Add(value) ret 4.
void __fastcall recovered_byte_list_add_owned_005704f0(void*, void*, int value);
}

}  // namespace kotor_talent

