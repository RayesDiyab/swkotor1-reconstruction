// CSWCProjectile foundation (vtable 00757CC8): constructor, destructors, list holder and the per-object control
// functions, finished from the paused sol-vfx-render draft (source/object_projectile_foundation.cpp).
//   006D57B0 ctor                      object_projectile_006d57b0  void*  fastcall @8   RET 0
//   006D5A40 dtor                      object_projectile_006d5a40  void   fastcall @8   RET 0
//   006D5AF0 scalar deleting dtor      object_projectile_006d5af0  void*  fastcall @12  RET 4   (table +000)
//   006D44B0 list holder ctor          object_projectile_006d44b0  void*  fastcall @8   RET 0
//   006D2C30 SetDuration               object_projectile_006d2c30  U      fastcall @12  RET 4
//   006D41B0 SetEffect                 object_projectile_006d41b0  void   fastcall @12  RET 4
//   006D4250 PlayAnimation             object_projectile_006d4250  U      fastcall @12  RET 4
//   006D42D0 SetTarget                 object_projectile_006d42d0  void   fastcall @16  RET 8
//   006D6EE0 SetMode                   object_projectile_006d6ee0  void   fastcall @12  RET 4
//   006D2C50 LoadModel                 object_projectile_006d2c50  U      fastcall @20  RET 0Ch (table +078)
//   006D2DB0 UnloadModel(part)         object_projectile_006d2db0  U      fastcall @12  RET 4   (table +07C)
//   006D2D30 UnloadModel()             object_projectile_006d2d30  void   fastcall @8   RET 0   (table +080)
// Declarations: shared/object_projectile*.h (unchanged copies of the foundation-owned shared headers that the
// accepted modes/motion/updates sources pin). Every callee is a selected provider or another projectile export
// under its exact name; table 00757CC8 is the tier t10 symbol object_projectile::table_00757cc8.
// Changes from the draft: direct calls (the draft's opaque() volatile-pointer wrapper is dropped); 006D42D0's
// second branch concatenates [object+130h] (the draft used +144h); 006D2D30/006D2DB0 rewritten from the draft's
// naked transcription; original unchecked pointer accesses retain their native failure behavior.
// EH: /EHs (no /EHc). Unwind states follow FuncInfo 00786188 (57B0), 007861BC (5A40), 00786130 (42D0),
// 007860F4 (44B0 and 2C50); see INTEGRATION.md.
#include "addd5fe471-object_projectile_internal.h"
#include "47b8e43b06-object_projectile_providers.h"
#include "282e36397a-object_projectile_literals.h"
#include "4150447bd0-m15e.h"
#include "9646e8111f-m15e_animbase.h"
#include "f58c269829-scene3d.h"
#include "9963eab06a-vtables_gen.h"
#include "91a206e668-startup_string.h"
#include "06dd5f0fff-resources.h"
#include "54fe282baa-allocator_live.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>
#include <type_traits>

using U = std::uint32_t;
using B = std::uint8_t;
using S = kotor_reconstruction::StartupString;

namespace menu_continuation { struct WorldTimer; }
namespace recovered_server_create { void* construct_list_internal_005e9aa0(void*); }
namespace kotor_sound_internal { void __fastcall ListUnwind005d86c0(void*, void*); }
namespace menu_tables_m41 { namespace cells { extern const unsigned int invalid_object_00746644; } }
namespace base_ini_reconstruction { extern const char warning_empty_0073d71c[]; }
S* __fastcall GuiConcat005e5d10(S*, void*, S*, const S*);
extern "C" {
extern void* kotor_cell_rules_007a3a28;
void* __fastcall kotor_m15a_object_construct_0063e530(void*, void*, U);
menu_continuation::WorldTimer* __fastcall frame_GetWorldTimer_005ed4f0(void*, void*);
void __fastcall frame_GetWorldTime_004ade40(menu_continuation::WorldTimer*, void*, U*, U*);
U __fastcall area_vfx_register_existing_004d80e0(void*, void*, U*, void*, U);
void* __fastcall kotor_m15e_client_get_client_ai_master_005ed6b0(void*, void*);
void __fastcall recovered_client_ai_master_add_object_00604a20(void*, void*, void*);
void __fastcall recovered_object_array_erase_004d8290(void*, void*, U);
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void*, void*, std::int32_t);
menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*, void*);
menu_resources::Ref* __fastcall c2da_copy_00406120(menu_resources::Ref*, void*, menu_resources::Ref*, menu_resources::Ref*);
S* __fastcall GuiAssign005e5140(S*, void*, const char*);
void* __fastcall object_projectile_model_006ea470(void*, void*);
}

// Exact contracts of the accepted consumers (cheat, lightsaber: decorated names) and of the table slots.
static_assert(std::is_same_v<decltype(&object_projectile_006d5af0), decltype(KotorM15ObjectMethods_007516c8::s000_scalar_deleting_destructor)>, "+000");
static_assert(std::is_same_v<decltype(&object_projectile_006d2c50), decltype(KotorM15ObjectMethods_007516c8::s078_load_model)>, "+078");
static_assert(std::is_same_v<decltype(&object_projectile_006d2db0), decltype(KotorM15ObjectMethods_007516c8::s07c_unload_model)>, "+07C");
static_assert(std::is_same_v<decltype(&object_projectile_006d2d30), decltype(KotorM15ObjectMethods_007516c8::s080_unload_model)>, "+080");

namespace {
template<class T> T rd(const void* p, U n) { T v; std::memcpy(&v, static_cast<const B*>(p) + n, sizeof v); return v; }
template<class T> void wr(void* p, U n, T v) { std::memcpy(static_cast<B*>(p) + n, &v, sizeof v); }
void* at(void* p, U n) { return static_cast<B*>(p) + n; }
void* ptr(const void* p, U n) { return rd<void*>(p, n); }
void* client() { return ptr(kotor_menu_window_cells::manager_007a39fc, 4); }   // [[007A39FC]+4]
S* string_at(void* p, U n) { return static_cast<S*>(at(p, n)); }
menu_resources::Ref* ref_at(void* p, U n) { return static_cast<menu_resources::Ref*>(at(p, n)); }
const KotorM15eAnimBaseVtbl* model_vtable(const void* model) { return rd<const KotorM15eAnimBaseVtbl*>(model, 0); }
const aurora_model::GobVTable* gob_vtable(const void* gob) { return rd<const aurora_model::GobVTable*>(gob, 0); }
const KotorM15ObjectMethods_007516c8* object_vtable(const void* object) { return rd<const KotorM15ObjectMethods_007516c8*>(object, 0); }
// [self+1ACh] holds a polymorphic object that is only released here (scalar deleting destructor, slot +0).
using ScalarDeleting = void* (__fastcall*)(void* self, void* edx, U flags);

// 006D57B0 / 006D5A40 unwind map (00786188 / 007861BC): state 0 -> CSWCObject::~CSWCObject 0063E800,
// state 1 -> ~CExoString [this+128h] then state 0, state 2 -> list holder unwind 005D86C0 [this+194h] then 1.
// `state` here is the original state + 1 (0 = original -1, nothing to undo).
struct ObjectUnwind {
    void* self;
    int state;
    ~ObjectUnwind() noexcept(false) {
        if (state >= 3) kotor_sound_internal::ListUnwind005d86c0(at(self, 0x194), nullptr);
        if (state >= 2) string_at(self, 0x128)->~StartupString();
        if (state >= 1) kotor_m15e_object_destruct_0063e800(self, nullptr);
    }
};
// 006D42D0 temporaries returned through a hidden pointer by GuiConcat 005E5D10 (unwind states 1 / 3).
struct ReturnedString {
    alignas(S) B bytes[sizeof(S)];
    bool active = false;
    S* get() { return reinterpret_cast<S*>(bytes); }
    ~ReturnedString() noexcept(false) { if (active) get()->~StartupString(); }
};
// 006D42D0 suffix concatenation, branch A (state 0/1) or B (state 2/3): S suffix(literal); [self+128] = base+field
// concatenated with suffix; both temporaries destroyed in the original order (concat result, then suffix).
void assign_concatenation(void* self, void* base, U field, const char* literal) {
    S suffix(literal);                                                             // 006D435E / 006D43C3 005E5A90
    ReturnedString combined;
    S* const result = GuiConcat005e5d10(string_at(base, field), nullptr, combined.get(), &suffix);  // 006D437B / 006D43E1
    combined.active = true;                                                        // state 1 / 3
    (void)string_at(self, 0x128)->Assign005e5c50(result);                          // 006D438C / 006D43F2
    combined.active = false;                                                       // state 0 / 2
    combined.get()->~StartupString();                                              // 006D439A / 006D43FF 005E5C20
}                                                                                  // suffix: 006D43AB / 006D4410
void assign_name_or_impact(void* self, S* name) {
    if (name->BorrowedEquals005e5290(base_ini_reconstruction::warning_empty_0073d71c) != 0)   // 006D4316..006D432A
        (void)GuiAssign005e5140(string_at(self, 0x128), nullptr, object_projectile_literal_00743318);  // 006D4335
    else
        (void)string_at(self, 0x128)->Assign005e5c50(name);                        // 006D4418
}
// Gob at [self+19Ch] (created by 006D33C0): detach, AttachToScene(null), then delete. Shared by 2D30 / 2DB0.
// Returns the EAX the original holds afterwards when the Gob path ran (Gob destructor result), else `none`.
U release_attached_gob(void* self, U none) {
    void* gob = ptr(self, 0x19c);
    if (gob == nullptr) return none;                                               // 006D2D39 / 006D2DB9
    (void)gob_vtable(gob)->s050_gob_attach_to_object(gob, nullptr, nullptr, nullptr, 0);   // call [eax+50h] (0,0,0)
    gob = ptr(self, 0x19c);                                                        // re-read
    U eax = static_cast<U>(gob_vtable(gob)->attach_to_scene(static_cast<aurora_model::Gob*>(gob), nullptr, nullptr));  // call [edx+4Ch] (0)
    gob = ptr(self, 0x19c);                                                        // re-read, null-checked
    if (gob != nullptr) eax = gob_vtable(gob)->s000_gob_destruct_gob(static_cast<aurora_model::Gob*>(gob), nullptr, 1);  // call [eax] (1)
    wr<U>(self, 0x19c, 0);
    return eax;
}
}  // namespace

extern "C" {

// 006D2C30..006D2C40: [this+120h] = [this+190h] = duration; EAX = duration; RET 4.
U __fastcall object_projectile_006d2c30(void* self, void*, U duration) {
    wr(self, 0x120, duration);
    wr(self, 0x190, duration);
    return duration;
}

// 006D44B0..006D4513: list holder: [this] = new(0Ch) list (005E9AA0) or 0 when the allocation is null; EAX = this.
// Unwind state 0 (FuncInfo 007860F4, action 00735EC0) frees the allocation if the list constructor throws.
void* __fastcall object_projectile_006d44b0(void* self, void*) {
    void* const allocation = live_operator_new(12u);                               // 006D44C7..006D44CB
    void* value = nullptr;
    if (allocation != nullptr) {                                                   // 006D44D7..006D44E1
        try { value = recovered_server_create::construct_list_internal_005e9aa0(allocation); }   // 006D44E5
        catch (...) { live_free(allocation); throw; }
    }
    wr(self, 0, value);                                                            // 006D44EA / 006D4504
    return self;
}

// 006D57B0..006D5A3B: CSWCProjectile::CSWCProjectile. RET 0, EAX = this.
void* __fastcall object_projectile_006d57b0(void* self, void*) {
    (void)kotor_m15a_object_construct_0063e530(self, nullptr, static_cast<U>(object_projectile_literal_00746637[0]));  // 006D57CA..006D57D9
    ObjectUnwind unwind{self, 1};                                                  // state 0
    wr(self, 0, &object_projectile::table_00757cc8);                               // 006D57E0
    wr<U>(self, 0x100, 0); wr<U>(self, 0x104, 0); wr<U>(self, 0x108, 0);
    wr<U>(self, 0x114, 0); wr<U>(self, 0x118, 0); wr<U>(self, 0x11c, 0);
    ::new (string_at(self, 0x128)) S();                                            // 006D5816 005B3190
    unwind.state = 2;                                                              // state 1 (006D5821)
    (void)c2da_default_ref_00405ed0(ref_at(self, 0x130), nullptr);                 // 006D5826
    (void)c2da_default_ref_00405ed0(ref_at(self, 0x140), nullptr);                 // 006D5831
    wr<U>(self, 0x178, 0); wr<U>(self, 0x17c, 0); wr<U>(self, 0x180, 0);
    wr<U>(self, 0x184, 0); wr<U>(self, 0x188, 0); wr<U>(self, 0x18c, 0);
    (void)object_projectile_006d44b0(at(self, 0x194), nullptr);                    // 006D5860
    wr<U>(self, 0x1b0, 0); wr<U>(self, 0x1b4, 0); wr<U>(self, 0x1b8, 0);
    unwind.state = 3;                                                              // state 2 (006D588A)
    U day, tick;
    frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client(), nullptr), nullptr, &day, &tick);  // 006D5877..006D5896
    wr(self, 0x3c, day);                                                           // 006D58A3
    wr<U>(self, 0x24, 0);
    wr(self, 0x40, tick);                                                          // 006D58AD
    wr<U>(self, 0x28, 0); wr<U>(self, 0x2c, 0);
    wr<U>(self, 0x114, 0); wr<U>(self, 0x118, 0);
    wr<U>(self, 0x178, 0); wr<U>(self, 0x11c, 0); wr<U>(self, 0x15c, 1);
    wr<U>(self, 0x160, 0); wr<U>(self, 0x164, 0); wr<U>(self, 0x168, 0); wr<U>(self, 0x16c, 0);
    wr<U>(self, 0x17c, 0); wr<U>(self, 0x184, 0); wr<U>(self, 0x188, 0);
    wr<U>(self, 0x154, 1); wr<U>(self, 0x170, 1); wr<U>(self, 0x180, 0); wr<U>(self, 0x18c, 0);
    wr<U>(self, 0x150, 0); wr<U>(self, 0x120, 0); wr<U>(self, 0x190, 0);
    (void)area_vfx_register_existing_004d80e0(kotor_fastcall_005ed570(client(), nullptr), nullptr,
                                              static_cast<U*>(at(self, 4)), self, 0u);   // 006D5956..006D5977
    recovered_client_ai_master_add_object_00604a20(kotor_m15e_client_get_client_ai_master_005ed6b0(client(), nullptr), nullptr, self);  // 006D597C..006D598D
    wr<U>(self, 0x100, 0); wr<U>(self, 0x104, 0); wr<U>(self, 0x108, 0);
    wr<U>(self, 0x198, 0); wr<U>(self, 0x174, 0); wr<U>(self, 0x19c, 0);
    wr<B>(self, 0x1a0, 0); wr<B>(self, 0x1a1, 0); wr<B>(self, 0xfc, 0);
    wr<std::uint16_t>(self, 0x10c, 0); wr<U>(self, 0x110, 0); wr<B>(self, 0x158, 0);
    (void)GuiAssign005e5140(string_at(self, 0x128), nullptr, object_projectile_literal_00743318);  // 006D59F4 "impact"
    wr<U>(self, 0x124, 0x7f000000u); wr<U>(self, 0x1c4, 0x7f000000u);             // 006D59FD..006D5A08
    wr<U>(self, 0x1a4, 0); wr<U>(self, 0x1a8, 0); wr<U>(self, 0x1ac, 0); wr<U>(self, 0x1bc, 0); wr<U>(self, 0x1c0, 0);
    unwind.state = 0;
    return self;                                                                   // 006D5A2D
}

// 006D5A40..006D5AE1: CSWCProjectile::~CSWCProjectile. Starts in unwind state 2.
void __fastcall object_projectile_006d5a40(void* self, void*) {
    ObjectUnwind unwind{self, 3};
    wr(self, 0, &object_projectile::table_00757cc8);                               // 006D5A5E
    const U id = rd<U>(self, 4);                                                   // 006D5A64
    recovered_object_array_erase_004d8290(kotor_fastcall_005ed570(client(), nullptr), nullptr, id);  // 006D5A67..006D5A80
    object_projectile_006d4520(self, nullptr);                                     // 006D5A87
    (void)object_projectile_006d2d30(self, nullptr);                               // 006D5A8E
    void* const list = ptr(self, 0x194);                                           // 006D5A93
    unwind.state = 2;                                                              // state 1 (006D5A9B)
    if (list != nullptr) {
        base_ini_reconstruction::destroy_list_005e9ab0(static_cast<kotor_list::Internal*>(list), nullptr);  // 006D5AA4
        live_free(list);                                                           // 006D5AAA
    }
    unwind.state = 1;                                                              // state 0 (006D5AB8)
    string_at(self, 0x128)->~StartupString();                                      // 006D5ABD 005E5C20
    unwind.state = 0;                                                              // state -1 (006D5AC4)
    kotor_m15e_object_destruct_0063e800(self, nullptr);                            // 006D5ACC
}

// 006D5AF0..006D5B0B: scalar deleting destructor: dtor, then live_free when (BYTE)flags & 1; EAX = this; RET 4.
void* __fastcall object_projectile_006d5af0(void* self, void*, U flags) {
    object_projectile_006d5a40(self, nullptr);                                     // 006D5AF3
    if (static_cast<B>(flags) & 1u) live_free(self);                               // 006D5AF8..006D5B00
    return self;
}

// 006D41B0..006D424B: SetEffect(effect) RET 4. Record = spell array [[007A3A28]+8Ch].GetSpell(effect).
void __fastcall object_projectile_006d41b0(void* self, void*, U effect) {
    std::int32_t id;
    std::memcpy(&id, &effect, sizeof id);
    void* const record = kotor_m15b_spell_array_get_spell_0059b6d0(ptr(kotor_cell_rules_007a3a28, 0x8c), nullptr, id);  // 006D41B0..006D41C8
    if (record == nullptr && effect != 0x2af8u) return;                            // 006D41CF..006D41D9
    wr(self, 0x174, effect);                                                       // 006D41E1
    menu_resources::Ref out;                                                       // [esp+0Ch] scratch, not initialised
    if (effect == 0x2af8u) {                                                       // 006D41DB / 006D41E7
        (void)unresolved_ref_assign_00406290(ref_at(self, 0x140), nullptr, &out, object_projectile_literal_00757cac);  // 006D41E9..006D41F9 "c_cow_dead"
        return;
    }
    menu_resources::Ref* const source = ref_at(record, 0x10d);                     // 006D4207
    (void)c2da_copy_00406120(ref_at(self, 0x130), nullptr, &out, source);           // 006D4219
    (void)c2da_copy_00406120(ref_at(self, 0x140), nullptr, &out, source);           // 006D422A
    wr<U>(self, 0x168, 0);                                                         // 006D422F
    wr(self, 0x1a1, rd<B>(record, 0x11d));                                         // 006D4239..006D423F
}

// 006D4250..006D428E: PlayAnimation(animation) RET 4: model [this+68h] SetAnimation(animation, 1.0f, 0, 0);
// on non-zero, WORD [this+48h] = animation and EAX 1; otherwise (or no model) EAX 0.
U __fastcall object_projectile_006d4250(void* self, void*, U animation) {
    void* const model = ptr(self, 0x68);
    if (model == nullptr) return 0;                                                // 006D4259..006D425B
    if (model_vtable(model)->s054(model, nullptr, animation, 0x3f800000u, 0u, 0u) == 0) return 0;   // 006D4264..006D4271
    wr(self, 0x48, static_cast<std::uint16_t>(animation));                         // 006D4273
    return 1;
}

// 006D42D0..006D4435: SetTarget(target, name) RET 8.
void __fastcall object_projectile_006d42d0(void* self, void*, U target, S* name) {
    void* const object = kotor_fastcall_005ed580(client(), nullptr, target);       // 006D42DE..006D42FB
    if (object == nullptr) {                                                       // 006D4302..006D4304
        wr(self, 0x124, menu_tables_m41::cells::invalid_object_00746644);           // 006D4306..006D430C
        assign_name_or_impact(self, name);                                         // 006D4312..
    } else {
        const U first = object_vtable(object)->s04c(object, nullptr);              // 006D433F..006D4343
        const U second = object_vtable(object)->s014(object, nullptr);             // 006D4348..006D434C
        if (first != 0)                                                            // 006D434F..006D4353
            assign_concatenation(self, reinterpret_cast<void*>(static_cast<std::uintptr_t>(first)), 0x144, object_projectile_literal_007554b4);   // "_impact"
        else if (second != 0)                                                      // 006D43B2..006D43B4
            assign_concatenation(self, reinterpret_cast<void*>(static_cast<std::uintptr_t>(second)), 0x130, object_projectile_literal_007554bc);  // "impc"
        else
            assign_name_or_impact(self, name);                                     // jumps to 006D4312
    }
    wr(self, 0x124, target);                                                       // 006D4422
}

// 006D6EE0..006D704A: SetMode(mode) RET 4. Jump table 006D7050 / index bytes 006D7098 (modes 1..50; 17..49 and
// out-of-range return after 006D4520).
void __fastcall object_projectile_006d6ee0(void* self, void*, U mode) {
    const U target = rd<U>(self, 0x124);                                           // 006D6EE3
    wr(self, 0x16c, mode);                                                         // 006D6EEE
    if (target != menu_tables_m41::cells::invalid_object_00746644) {               // 006D6EF4..006D6EFA
        void* const array = kotor_fastcall_005ed570(client(), nullptr);            // 006D6EFC..006D6F05
        void* object;                                                              // the original reuses its argument slot
        if (kotor_fastcall_004d8230(array, nullptr, rd<U>(self, 0x124), &object) == object_projectile_literal_0074666c[0]) {  // 006D6F0A..006D6F23
            wr(self, 0x114, rd<U>(object, 0x24));                                  // 006D6F25..006D6F3F
            wr(self, 0x118, rd<U>(object, 0x28));
            wr(self, 0x11c, rd<U>(object, 0x2c));
        }
    }
    object_projectile_006d4520(self, nullptr);                                     // 006D6F44
    switch (mode) {                                                                // 006D6F49..006D6F5C
    case 1: object_projectile_006d4550(self, nullptr); break;
    case 2: object_projectile_006d4640(self, nullptr); break;
    case 3: object_projectile_006d4730(self, nullptr); break;
    case 4: object_projectile_006d4930(self, nullptr); break;
    case 5: object_projectile_006d4b10(self, nullptr); break;
    case 6: object_projectile_006d4c00(self, nullptr); break;
    case 7: object_projectile_006d4fe0(self, nullptr); break;
    case 8: object_projectile_006d64f0(self, nullptr); break;
    case 9: object_projectile_006d4a20(self, nullptr); break;
    case 10: object_projectile_006d5500(self, nullptr); break;
    case 11: object_projectile_006d5b10(self, nullptr, 1u); break;
    case 12: object_projectile_006d5b10(self, nullptr, 0u); break;
    case 13: object_projectile_006d6a90(self, nullptr); break;
    case 14: object_projectile_006d50f0(self, nullptr); break;
    case 15: object_projectile_006d5250(self, nullptr); break;
    case 16: object_projectile_006d53a0(self, nullptr); break;
    case 50: object_projectile_006d55f0(self, nullptr); break;
    default: return;                                                               // 006D7048
    }
    void* const record = base_ini_reconstruction::remove_head_005e9c80(static_cast<kotor_list::Internal*>(ptr(self, 0x194)), nullptr);  // 006D7007..006D700D
    wr(self, 0x198, record);                                                       // 006D7012
    const U duration = rd<U>(record, 0x20);                                        // 006D7018
    wr(self, 0x120, duration);
    wr(self, 0x190, duration);
    wr(self, 0x114, rd<U>(record, 0));                                             // 006D7027..006D703A
    wr(self, 0x118, rd<U>(record, 4));
    wr(self, 0x11c, rd<U>(record, 8));
    object_projectile_006d33c0(self, nullptr, static_cast<kotor_scene3d::Vector*>(at(self, 0x30)));   // 006D703D..006D7043
}

// 006D2C50..006D2D25: LoadModel(resref, part, type) RET 0Ch; `type` is not read.
U __fastcall object_projectile_006d2c50(void* self, void*, void* resref, U part, U) {
    if (ptr(self, 0x68) == nullptr) {                                              // 006D2C69..006D2C6E
        void* const allocation = live_operator_new(0xf0u);                         // 006D2C70..006D2C75
        void* model = nullptr;
        if (allocation != nullptr) {                                               // 006D2C81..006D2C8B
            try { model = object_projectile_model_006ea470(allocation, nullptr); }   // 006D2C8F (state 0: free on throw)
            catch (...) { live_free(allocation); throw; }
        }
        wr(self, 0x68, model);                                                     // 006D2CA2
        if (model == nullptr) return 0;                                            // 006D2CA5..006D2CB6
    }
    void* model = ptr(self, 0x68);                                                 // 006D2CB9
    if (model_vtable(model)->s00c(model, nullptr, resref, part) == 0) return 0;    // 006D2CC2..006D2CE0 LoadModel
    model = ptr(self, 0x68);                                                       // 006D2CE3
    void* const gob = model_vtable(model)->s008(model, nullptr, part);             // 006D2CE8..006D2CE9 GetModel(part)
    auto* const g = static_cast<aurora_model::Gob*>(gob);
    (void)gob_vtable(g)->s074_gob_set_external_data(g, nullptr, static_cast<U>(reinterpret_cast<std::uintptr_t>(self)));   // 006D2CF0..006D2CF3
    (void)gob_vtable(g)->s07c_gob_set_external_data_type(g, nullptr, static_cast<U>(object_projectile_literal_00746637[0]));  // 006D2CF6..006D2D02
    model = ptr(self, 0x68);                                                       // 006D2D05
    (void)model_vtable(model)->s0bc(model, nullptr);                               // 006D2D0A
    return 1;
}

// 006D2D30..006D2DAD: UnloadModel() RET 0. Releases the attached Gob [this+19Ch], the object [this+1ACh] and the
// model [this+68h]. The canonical +080 callable is void; callee results are discarded.
void __fastcall object_projectile_006d2d30(void* self, void*) {
    (void)release_attached_gob(self, 0);                             // 006D2D33..006D2D65
    if (ptr(self, 0x68) == nullptr) return;                                      // 006D2D6F..006D2D74 (EAX = [this+68h] = 0)
    void* const extra = ptr(self, 0x1ac);                                          // 006D2D76
    if (extra != nullptr) (void)rd<ScalarDeleting>(rd<const void*>(extra, 0), 0)(extra, nullptr, 1);   // 006D2D80..006D2D84
    void* model = ptr(self, 0x68);                                                 // 006D2D86
    wr<U>(self, 0x1ac, 0);                                                         // 006D2D89
    (void)model_vtable(model)->s078(model, nullptr);                             // 006D2D93..006D2D95 UnloadModel()
    model = ptr(self, 0x68);                                                       // 006D2D98
    if (model != nullptr)
        (void)model_vtable(model)->s000(model, nullptr, 1);   // 006D2D9F..006D2DA3
    wr<U>(self, 0x68, 0);                                                          // 006D2DA5
}

// 006D2DB0..006D2DFC: UnloadModel(part) RET 4. Releases the attached Gob [this+19Ch], then tail-calls the model's
// UnloadModel(part) (JMP [edx+74h]) and returns its EAX. With no model the original returns whatever EAX holds:
// the Gob destructor result when the Gob path ran, otherwise the caller's EAX (not representable; 0 is returned
// on that path only - documented deviation, see INTEGRATION.md).
U __fastcall object_projectile_006d2db0(void* self, void*, U part) {
    const U eax = release_attached_gob(self, 0);                     // 006D2DB3..006D2DE5
    void* const model = ptr(self, 0x68);                                           // 006D2DEF
    if (model == nullptr) return eax;                                              // 006D2DF2..006D2DFC
    return model_vtable(model)->s074(model, nullptr, part);                        // 006D2DF7..006D2DF9
}

}  // extern "C"
