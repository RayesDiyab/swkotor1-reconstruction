// Lane A sub-tier 1: the ten unbound VM ACTION handler bodies.
// Reconstructed from swkotor.exe 1.0 (SHA-256 9c10e045...) at the exact original
// extents.  Every callee is a selected candidate87 provider or one of the two
// shared facade providers declared in talent_cohort.hpp.
#include "30d695bcc8-talent_cohort.hpp"

namespace kotor_talent {
namespace {

void* outer_vm() { return pv(frame_control::message_manager_007a3a00); }
void* server_app() { return pv(u32(reinterpret_cast<void*>(&frame_control::message_manager_007a3a00), 0)); }
inline void* app_manager() { return kotor_menu_window_cells::manager_007a39fc; }
inline void* server() { return app_manager() ? *reinterpret_cast<void**>(static_cast<unsigned char*>(app_manager()) + 8) : nullptr; }
inline void* rules() { return kotor_cell_rules_007a3a28; }

int push_integer(int v) { return recovered_vm_outer_push_integer_005d1010(outer_vm(), nullptr, v); }
int pop_integer(int* out) { return recovered_vm_outer_pop_integer_005d1000(outer_vm(), nullptr, out); }
int pop_object(U* out) { return recovered_vm_outer_pop_object_005d10c0(outer_vm(), nullptr, out); }
int push_object(U v) { return recovered_vm_outer_push_object_005d10d0(outer_vm(), nullptr, v); }
int pop_engine(int type, void** out) { return recovered_vm_outer_pop_engine_structure_005d10a0(outer_vm(), nullptr, type, out); }
int push_engine(int type, const void* data) { return recovered_vm_outer_push_engine_structure_005d10b0(outer_vm(), nullptr, type, data); }

int vcall_int1(void* self, U slot, int arg) {
    using F = int (__thiscall*)(void*, int);
    void** vtable = *reinterpret_cast<void***>(self);
    return reinterpret_cast<F>(vtable[slot / 4])(self, arg);
}
void* as_creature(void* obj) { return vcall0(obj, 0x30); }
void* as_swsobject(void* obj) { return vcall0(obj, 0x10); }

// 0053A2C0's x87 transport: fld qword [00741DF8] (=2.0); fild slot; call 006FC1F0; call 006FAE8C.
__declspec(noinline) double x87_pow_006fc1f0(double base, double exponent) {
    double result;
    __asm {
        fld qword ptr [base]
        fld qword ptr [exponent]
        call genuine_x87_pow_006fc1f0
        fstp qword ptr [result]
    }
    return result;
}
__declspec(noinline) int x87_ftol_006fae8c(double value) {
    int result;
    __asm {
        fld qword ptr [value]
        call genuine_x87_ftol_006fae8c
        mov result, eax
    }
    return result;
}

}  // namespace

// ------------------------------------------------------------------ 00539610
extern "C" int __fastcall recovered_vm_get_hitpoints_00539610(void*, void*, int routine, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return -2001;
    int value = 0;
    void* obj = nullptr;
    void* array = recovered_object_array_004aed70(server(), nullptr);
    const unsigned status = kotor_fastcall_004d8230(array, nullptr, id, &obj);
    if (static_cast<unsigned char>(status) == 0 && u8(obj, 8) > 4) {
        if (routine == 0x31) {
            if (void* c = as_creature(obj)) value = vcall_int1(c, 0x9c, 0);
            else value = vcall_int1(as_swsobject(obj), 0x9c, 0);
        } else {
            if (void* c = as_creature(obj)) value = vcall_int1(c, 0x98, 1);
            else value = vcall_int1(as_swsobject(obj), 0x98, 1);
        }
        value = static_cast<std::int16_t>(value);   // movsx ax
    }
    return push_integer(value) ? 0 : -2000;
}

// ------------------------------------------------------------------ 00549180
extern "C" int __fastcall recovered_vm_get_effect_00549180(void*, void*, int routine, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return -2001;

    void* effect = nullptr;
    bool destroy_after = false;
    void* obj = kotor_fastcall_004ae750(server(), nullptr, id);
    void* holder = obj ? as_swsobject(obj) : nullptr;
    if (holder) {
        int index;
        if (routine == 0x55) {
            set_u16(holder, 0x1e8, 0);
            index = 0;
        } else {
            index = static_cast<int>(u16(holder, 0x1e8));
        }
        const int count = static_cast<int>(u32(holder, 0x128));
        void* const* list = reinterpret_cast<void* const*>(pv(u32(holder, 0x124)));
        while (index < count) {
            void* candidate = list[index];
            if (u32(candidate, 0x20) != 0 && u16(candidate, 8) != 67 &&
                ((u16(candidate, 0xa) & 7u) == 2u || (u16(candidate, 0xa) & 7u) == 1u)) {
                effect = candidate;
                set_u16(holder, 0x1e8, static_cast<unsigned short>(index + 1));
                break;
            }
            ++index;
        }
        if (index >= count) {
            effect = nullptr;                    // fall through to the invalid-effect allocation
            set_u16(holder, 0x1e8, 0);           // original resets the cursor on scan exhaustion
        }
    }

    if (!effect) {
        // Recheck of 00549326..2F: the push-success path falls through 0054932F,
        // so both the no-holder and holder/no-effect allocations are destroyed;
        // only the found-effect path (ebx=1) leaves the effect alone.
        destroy_after = true;
        void* alloc = live_operator_new(0x8c);
        effect = alloc;
        if (alloc) recovered_effect_construct_00503e40(alloc, nullptr, 1);
        // The original stores type 0 without an allocation check (null deref preserved).
        *reinterpret_cast<volatile unsigned short*>(static_cast<unsigned char*>(effect) + 8) = 0;
    }

    if (!push_engine(kEngineEffect, effect)) return -2000;
    if (destroy_after) {
        if (effect) {
            recovered_effect_destroy_00503590(effect, nullptr);
            live_free(effect);
        }
    }
    return 0;
}

// ------------------------------------------------------------------ 0053EE00
extern "C" int __fastcall recovered_vm_get_is_effect_valid_0053ee00(void*, void*, int, unsigned) {
    void* effect = nullptr;
    if (!pop_engine(kEngineEffect, &effect)) return -2001;

    int valid = 1;
    if (effect) {
        if (u16(effect, 8) == 0) valid = 0;
        else if (u32(effect, 0x20) == 0) valid = 0;
    } else {
        valid = 0;
    }
    // Original reads the flags through the pointer unconditionally (null deref preserved).
    const unsigned short low = static_cast<unsigned short>(
        *reinterpret_cast<volatile unsigned short*>(static_cast<unsigned char*>(effect) + 0xa) & 7u);
    if (low != 0 && low != 1 && low != 2 && low != 3 && low != 4) valid = 0;

    if (!push_integer(valid)) return -2000;
    if (effect) {
        recovered_effect_destroy_00503590(effect, nullptr);
        live_free(effect);
    }
    return 0;
}

// ------------------------------------------------------------------ 005420B0
extern "C" int __fastcall recovered_vm_roll_dice_005420b0(void*, void*, int routine, unsigned argc) {
    int sides;
    switch (routine - 0x5f) {
        case 0: sides = 2; break;
        case 1: sides = 3; break;
        case 2: sides = 4; break;
        case 3: sides = 6; break;
        case 4: sides = 8; break;
        case 5: sides = 10; break;
        case 6: sides = 12; break;
        case 7: sides = 20; break;
        case 8: sides = 100; break;
        default: sides = routine; break;   // original uses the raw routine value
    }
    int count = 1;
    if (argc == 1) {
        if (!pop_integer(&count)) return -2001;
    }
    if (count <= 1) count = 1;
    const unsigned rolled = recovered_rules_roll_dice_00550b60(rules(), nullptr, count, sides);
    return push_integer(static_cast<unsigned short>(rolled)) ? 0 : -2000;
}

// ------------------------------------------------------------------ 0053A2C0
extern "C" int __fastcall recovered_vm_get_item_in_slot_0053a2c0(void*, void*, int, unsigned) {
    int slot = 0;
    if (!pop_integer(&slot)) return -2001;
    const int mask = x87_ftol_006fae8c(x87_pow_006fc1f0(2.0, static_cast<double>(slot)));
    U id = 0;
    if (!pop_object(&id)) return -2001;

    U result = 0x7f000000u;
    void* obj = nullptr;
    void* array = recovered_object_array_004aed70(server(), nullptr);
    if (static_cast<unsigned char>(kotor_fastcall_004d8230(array, nullptr, id, &obj)) == 0) {
        if (void* creature = as_creature(obj)) {
            void* inventory = pv(u32(creature, 0xa2c));
            if (inventory) {
                void* item = recovered_equipment_item_005a4c20(inventory, nullptr, static_cast<U>(mask));
                if (item) result = u32(item, 0x14);
            }
        }
    }
    return push_object(result) ? 0 : -2000;
}

// ------------------------------------------------------------------ 0053EAA0
extern "C" int __fastcall recovered_vm_int_to_float_0053eaa0(void*, void*, int, unsigned) {
    int value = 0;
    if (!pop_integer(&value)) return -2001;
    float converted = static_cast<float>(value);
    U bits;
    std::memcpy(&bits, &converted, 4);
    return recovered_vm_outer_push_float_005d1030(outer_vm(), nullptr, bits) ? 0 : -2000;
}

// ------------------------------------------------------------------ 00536EC0
extern "C" int __fastcall recovered_vm_get_creature_has_talent_00536ec0(void*, void*, int, unsigned argc) {
    void* talent = nullptr;
    if (!pop_engine(kEngineTalent, &talent)) return -2001;
    U id = 0x7f000000u;
    if (static_cast<int>(argc) > 1) {
        if (!pop_object(&id)) return -2001;
    }
    int found = 0;
    void* creature = kotor_fastcall_004ae770(server(), nullptr, id);
    if (creature) {
        void* stats = pv(u32(creature, 0xa74));
        found = recovered_stats_has_talent_005aaa30(stats, nullptr, static_cast<int>(u32(talent, 0)), u32(talent, 4));
    }
    if (!push_integer(found)) return -2000;   // original 00536F4A..55 leaks the talent on push failure
    if (talent) { kotor_fastcall_004015a0(talent, nullptr); live_free(talent); }
    return 0;
}

// ------------------------------------------------------------------ 00536FE0
extern "C" int __fastcall recovered_vm_get_creature_talent_00536fe0(void*, void*, int routine, unsigned argc) {
    int first = 0;
    if (!pop_integer(&first)) return -2001;

    U objectId = 0x7f000000u;
    int arg2 = -1, arg3 = 0, arg4 = -1, arg5 = -1;

    if (routine == 0x134) {
        if (!pop_integer(&arg2)) return -2001;
        if (static_cast<int>(argc) > 2) { if (!pop_object(&objectId)) return -2001; }
        if (static_cast<int>(argc) > 3) { if (!pop_integer(&arg3)) return -2001; }
        if (static_cast<int>(argc) > 4) { if (!pop_integer(&arg4)) return -2001; }
        if (static_cast<int>(argc) > 5) { if (!pop_integer(&arg5)) return -2001; }
    } else if (routine == 0x133) {
        if (static_cast<int>(argc) > 1) { if (!pop_object(&objectId)) return -2001; }
        if (static_cast<int>(argc) > 2) { if (!pop_integer(&arg3)) return -2001; }
    }

    ScriptTalent talent;
    recovered_talent_construct_004ca9e0(&talent, nullptr);

    void* creature = kotor_fastcall_004ae770(server(), nullptr, objectId);
    if (creature) {
        void* stats = pv(u32(creature, 0xa74));
        I outType = -1, outId = -1, outField10 = -1;
        unsigned char outB8 = 0xff, outB14 = 0xff;
        U outField0c = 0x7f000000u;
        int a1, a2, a9, a10, a11;
        if (routine == 0x134) {
            a1 = first; a2 = arg2; a9 = arg3; a10 = arg4; a11 = arg5;
        } else {
            a1 = first; a2 = -1; a9 = arg3; a10 = -1; a11 = -1;
        }
        recovered_stats_talent_random_005b23a0(stats, nullptr, a1, a2,
                                               static_cast<U>(reinterpret_cast<std::uintptr_t>(&outType)),
                                               static_cast<U>(reinterpret_cast<std::uintptr_t>(&outId)),
                                               static_cast<U>(reinterpret_cast<std::uintptr_t>(&outB8)),
                                               static_cast<U>(reinterpret_cast<std::uintptr_t>(&outField0c)),
                                               static_cast<U>(reinterpret_cast<std::uintptr_t>(&outField10)),
                                               static_cast<U>(reinterpret_cast<std::uintptr_t>(&outB14)),
                                               a9, a10, a11);
        talent.type = outType;
        talent.id = outId;
        talent.b8 = outB8;
        talent.field0c = static_cast<I>(outField0c);
        talent.field10 = outField10;
        talent.b14 = outB14;
    } else {
        talent.id = -1;
    }

    const int pushed = push_engine(kEngineTalent, &talent);
    kotor_fastcall_004015a0(&talent, nullptr);
    return pushed ? 0 : -2000;
}

// ------------------------------------------------------------------ 00539F90
extern "C" int __fastcall recovered_vm_get_is_talent_valid_00539f90(void*, void*, int, unsigned) {
    void* talent = nullptr;
    if (!pop_engine(kEngineTalent, &talent)) return -2001;
    int valid = 1;
    if (u32(talent, 0) == 0xffffffffu) valid = 0;
    if (talent) { kotor_fastcall_004015a0(talent, nullptr); live_free(talent); }
    return push_integer(valid) ? 0 : -2000;
}

// ------------------------------------------------------------------ 00544D70
extern "C" int __fastcall recovered_vm_talent_access_00544d70(void*, void*, int routine, unsigned) {
    void* talent = nullptr;
    if (!pop_engine(kEngineTalent, &talent)) return -2001;

    int result = 0;
    int pushed = 1;
    const int kind = routine - 0x16a;
    if (kind == 0) {                                   // 362 GetTypeFromTalent
        pushed = push_integer(static_cast<int>(u32(talent, 0)));
    } else if (kind == 1) {                            // 363 GetIdFromTalent
        pushed = push_integer(static_cast<int>(u32(talent, 4)));
    } else if (kind == 0x175) {                        // 735 spell/feat lookup
        const int type = static_cast<int>(u32(talent, 0));
        if (type == 0) {
            void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(pv(u32(rules(), 0x8c)), nullptr,
                                                                        static_cast<I>(u32(talent, 4)));
            result = spell ? static_cast<int>(u32(spell, 0x124)) : 0;
        } else if (type == 1) {
            void* feat = kotor_rules::kotor_fastcall_00550c00(rules(), nullptr,
                                                             static_cast<unsigned short>(u32(talent, 4)));
            result = feat ? static_cast<int>(u32(feat, 0x10)) : 0;
        }
        pushed = push_integer(result);
    }
    if (talent) { kotor_fastcall_004015a0(talent, nullptr); live_free(talent); }
    return pushed ? 0 : -2000;
}

}  // namespace kotor_talent

