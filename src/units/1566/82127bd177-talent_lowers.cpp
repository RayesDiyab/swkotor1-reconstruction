// Lane A sub-tier 2a: the exactly reconstructed lower talent bodies.
#include "49380ea791-talent_cohort.hpp"

namespace sound_pause29_required { unsigned __stdcall zero_0063e7f0(); }

namespace kotor_talent {
namespace {
using sound_pause29_required::zero_0063e7f0;

inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void* owner_of(void* stats) { return pv(u32(stats, 0x24)); }

int has_spell(void* stats, U classIndex, U spell, U arg) {
    return static_cast<int>(recovered_stats_has_spell_005a6e70(stats, nullptr, classIndex, spell, arg));
}

}  // namespace

// ------------------------------------------------------------------ 005B23A0
extern "C" int __fastcall recovered_stats_talent_random_005b23a0(void* self, void*, I a1, I a2, U a3, U a4,
                                                                U a5, U a6, U a7, U a8, I a9, I a10, I a11) {
    I* const type = reinterpret_cast<I*>(static_cast<std::uintptr_t>(a3));
    I* const id = reinterpret_cast<I*>(static_cast<std::uintptr_t>(a4));
    unsigned char* const b8 = reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(a5));
    U* const field0c = reinterpret_cast<U*>(static_cast<std::uintptr_t>(a6));
    I* const field10 = reinterpret_cast<I*>(static_cast<std::uintptr_t>(a7));
    unsigned char* const b14 = reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(a8));
    *type = -1;
    *id = -1;
    *b8 = 0xff;
    *field0c = 0x7f000000u;
    *field10 = -1;
    *b14 = 0xff;

    int tried_special = 0;
    int tried_feat = 0;
    for (;;) {
        if (tried_special != 0 && tried_feat != 0) {
            *type = -1;
            *id = -1;
            return 0;
        }
        const int draw = crt_rand_006fc447();
        if ((draw & 1) == 0) {
            if (tried_special != 0) continue;
            const I last = (a10 == 0) ? a11 : -1;
            if (recovered_talent_special_005ab9e0(self, nullptr, a1, a2, static_cast<I>(a3), static_cast<I>(a4),
                                                  static_cast<I>(a5), static_cast<I>(a8), a9, last) != 0) return 1;
            if (recovered_talent_power_005afa30(self, nullptr, a1, a2, static_cast<I>(a3), static_cast<I>(a4),
                                                static_cast<I>(a5), a9, last) != 0) return 1;
            if (recovered_talent_power_item_005abd00(self, nullptr, a1, a2, static_cast<I>(a3), static_cast<I>(a4),
                                                     static_cast<I>(a5), static_cast<I>(a6), static_cast<I>(a7), a9,
                                                     last) != 0) return 1;
            tried_special = 1;
        } else {
            if (tried_feat != 0) continue;
            I last = -1;
            if (a10 == 1) {
                last = a11;
                if (a11 == -1) tried_special = 1;
            }
            if (recovered_talent_feat_005ac370(self, nullptr, a1, a2, static_cast<I>(a3), static_cast<I>(a4),
                                               static_cast<I>(a5), a9, last) != 0) return 1;
            tried_feat = 1;
        }
    }
}

// ------------------------------------------------------------------ 005AAA30
extern "C" int __fastcall recovered_stats_has_talent_005aaa30(void* self, void*, int type, unsigned id) {
    if (type == -1 || id == 0xffffffffu) return 0;
    void* owner = owner_of(self);

    auto dead = [&]() -> int {
        if (type == 0) {
            (void)kotor_m15b_spell_array_get_spell_0059b6d0(pv(u32(rules(), 0x8c)), nullptr, static_cast<I>(id));
        }
        return 0;
    };

    if ((u8(owner, 0x9f0) & 6u) != 6u) {
        if (vcall0(owner, 0x94) != nullptr) return dead();
    }
    if (zero_0063e7f0() != 0) return dead();
    if (recovered_creature_incapacitated_004ef890(owner, nullptr) != 0) return dead();

    int found = 0;
    if (type == 0) {
        void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(pv(u32(rules(), 0x8c)), nullptr,
                                                                    static_cast<I>(id));
        if (!spell) return 0;
        const unsigned classCount = u8(self, 0x89);
        if (classCount == 0) return 0;
        unsigned char index = 0;
        do {
            const unsigned char cls = static_cast<unsigned char>(kt_get_class_005a4e90(self, nullptr, index));
            if (kt_is_jedi_005be4a0(nullptr, nullptr, cls) != 0) {
                if (has_spell(self, 0, id, 1)) {
                    if (recovered_stats_spell_uses_left_005a5550(self, nullptr, id, index, 0, 0) != 0) {
                        found = 1;
                        break;
                    }
                }
            } else {
                if (recovered_stats_in_spell_like_005a68e0(self, nullptr, id) != 0) { found = 1; break; }
                if (has_spell(self, 0, id, 1)) {
                    if (recovered_stats_spell_uses_left_005a5550(self, nullptr, id, index, 0, 0) != 0) {
                        found = 1;
                        break;
                    }
                }
                if (recovered_stats_power_from_item_005a72e0(self, nullptr, id) != 0) { found = 1; break; }
            }
            ++index;
        } while (index < classCount);
        if (!found) return 0;
        if ((u8(owner, 0x9f0) & 8u) == 0) return 0;
        return 1;
    }
    if (type == 1) {
        if (recovered_stats_has_feat_005a6630(self, nullptr, id) == 0) return 0;
        if ((kt_feat_remaining_uses_005a6680(self, nullptr, id) & 0xffu) == 0) return 0;
        return 1;
    }
    if (type == 2) {
        const int rank = static_cast<signed char>(
            static_cast<unsigned char>(w1x_creature_skill_rank_005aa570(self, nullptr, static_cast<int>(id), 0, 0)));
        if (rank > 0) return 1;
        void* table = pv(u32(rules(), 0xb4));
        const U at = (id & 0xffffu) << 5;
        if ((u8(table, at + 0x1c) & 2u) != 0) return 1;
        return 0;
    }
    return 0;
}

// ------------------------------------------------------------------ 005A5550
extern "C" int __fastcall recovered_stats_spell_uses_left_005a5550(void* self, void*, unsigned spell,
                                                                  unsigned classIndex, unsigned, unsigned) {
    if (static_cast<unsigned char>(classIndex) >= u8(self, 0x89)) return 0;
    void* sp = kotor_m15b_spell_array_get_spell_0059b6d0(pv(u32(rules(), 0x8c)), nullptr,
                                                             static_cast<I>(spell));
    if (!sp) return 0;
    void* owner = owner_of(self);
    const U flag = u32(owner, 0xa88);                                   // 005A5582, pushed first (arg2)
    const I level_word = static_cast<std::int16_t>(u16(self, 0x17e));   // 005A5591 movsx, pushed last (arg1)
    const U cost = recovered_spell_adjusted_force_points_0059b8a0(sp, nullptr, level_word, flag);
    const I fp = static_cast<std::int16_t>(static_cast<unsigned short>(u16(self, 0x126) + u16(self, 0x124)));
    // 005A55AE cmp edx,eax / sbb al,al / inc al: unsigned fp >= cost.
    return static_cast<U>(fp) >= cost;
}

// ------------------------------------------------------------------ 005A68E0
extern "C" int __fastcall recovered_stats_in_spell_like_005a68e0(void* self, void*, unsigned id) {
    void* list = pv(u32(self, 0x128));
    const I count = static_cast<I>(u32(list, 4));
    if (count <= 0) return 0;
    const U* data = reinterpret_cast<const U*>(pv(u32(list, 0)));
    for (I i = 0; i < count; ++i) {
        if (data[i * 3 + 0] == id && data[i * 3 + 1] == 1u) return 1;
    }
    return 0;
}

// ------------------------------------------------------------------ 005A72E0
extern "C" int __fastcall recovered_stats_power_from_item_005a72e0(void* self, void*, unsigned id) {
    void* owner = owner_of(self);
    void* inventory = pv(u32(owner, 0xa2c));
    if (inventory) {
        for (int slot = 0; slot < 0x12; ++slot) {
            void* item = recovered_equipment_item_005a4c20(inventory, nullptr, 1u << slot);
            if (!item) continue;
            if (recovered_item_property_has_00555c90(item, nullptr, 10, 0) == 0) continue;
            const I count = static_cast<I>(u32(item, 0x248));
            for (I i = 0; i < count; ++i) {
                void* prop = kotor_m15_menu_active_property_00553960(item, nullptr, i);
                if (u16(prop, 0) != 10) continue;
                if (u8(prop, 0xc) == 0) continue;
                const unsigned char bit = u8(prop, 0x11);
                if (bit != 0xffu && (u32(item, 0x294) & (1u << (bit & 31u))) == 0) continue;
                if (static_cast<U>(u16(prop, 2)) == id) return 1;
            }
        }
    }
    void* repository = recovered_creature_inventory_004ef770(owner, nullptr, 1);
    if (!repository) return 0;
    const U n = u32(repository, 0x10);
    for (U i = 0; i < n; ++i) {
        void* item = w2x_inventory_item_005560b0(repository, nullptr, static_cast<I>(i));
        if (!item) continue;
        void* base = kotor_m15b_base_item_array_get_base_item_005b31a0(pv(u32(rules(), 0x34)), nullptr,
                                                                       static_cast<I>(u32(item, 0xc)));
        if (u32(base, 4) != 0) continue;
        if (recovered_item_property_has_00555c90(item, nullptr, 10, 0) == 0) continue;
        const I count = static_cast<I>(u32(item, 0x248));
        for (I j = 0; j < count; ++j) {
            void* prop = kotor_m15_menu_active_property_00553960(item, nullptr, j);
            if (u16(prop, 0) != 10) continue;
            if (u8(prop, 0xc) == 0 && static_cast<I>(u32(item, 0x258)) <= 0) continue;
            const unsigned char bit = u8(prop, 0x11);
            if (bit != 0xffu && (u32(item, 0x294) & (1u << (bit & 31u))) == 0) continue;
            if (static_cast<U>(u16(prop, 2)) == id) return 1;
        }
    }
    return 0;
}

// ------------------------------------------------------------------ 005A5D70
extern "C" int __fastcall recovered_talent_compatible_005a5d70(void*, void*, unsigned a, unsigned b) {
    int ok = 1;
    const unsigned masks[4] = {0xf000u, 0x0f00u, 0x00f0u, 0x000fu};
    // Original loads b into edx and a into esi: b is the wildcard side.
    for (int i = 0; i < 4; ++i) {
        const unsigned wild = b & masks[i];
        if (wild != masks[i] && wild != (a & masks[i])) ok = 0;
    }
    return ok;
}

// ------------------------------------------------------------------ 005AB980
extern "C" int __fastcall recovered_talent_power_suitable_005ab980(void* self, void*, I a1, I a2, I a3, I a4,
                                                                   I a5, I a6, I a7, I a8) {
    (void)a4; (void)a5; (void)a6; (void)a7;
    if (a1 == a8) return 0;
    if (recovered_stats_has_talent_005aaa30(self, nullptr, 0, static_cast<unsigned>(a1)) == 0) return 0;
    if (recovered_object_area_004cb120(owner_of(self), nullptr) == nullptr) return 1;
    void* spell = pv(static_cast<U>(a2));
    return recovered_talent_compatible_005a5d70(self, nullptr, u32(spell, 0x124), static_cast<unsigned>(a3)) != 0;
}

// ------------------------------------------------------------------ 005A5DF0
extern "C" int __fastcall recovered_talent_inclusion_005a5df0(void* self, void*, I a1, I a2, I a3) {
    (void)self;
    int result = 1;
    if (a1 == 1) {
        char column[8];
        frame_LogStringCtor_005e5a90(column, nullptr, "Exclusion");   // 0074A1CC
        void* table = pv(u32(pv(u32(rules(), 0xb8)), 0xf8));
        int out = 0;
        const int status = static_cast<int>(c2da_current_get_int_00413660(table, nullptr, a2, column, &out));
        frame_LogStringDtor_005e5c20(column, nullptr);
        if (status == 0 || out == 0 || (out | a3) == 0) result = 1;
        else result = 0;
    } else if (a1 == 0) {
        char column[8];
        frame_LogStringCtor_005e5a90(column, nullptr, "Exclusion");   // 0074A1CC
        void* table = pv(u32(pv(u32(rules(), 0xb8)), 0xf4));
        int out = 0;
        const int status = static_cast<int>(c2da_current_get_int_00413660(table, nullptr, a2, column, &out));
        frame_LogStringDtor_005e5c20(column, nullptr);
        if (status == 0 || out == 0 || (out | a3) == 0) result = 1;
        else result = 0;
    }
    return result;
}

// ------------------------------------------------------------------ 005AFA30
extern "C" int __fastcall recovered_talent_power_005afa30(void* self, void*, I a1, I a2, I a3, I a4,
                                                          I a5, I a6, I a7) {
    const unsigned char classCount = u8(self, 0x89);
    if (classCount == 0) return 0;
    I last = 0;
    unsigned char index = 0;
    do {
        unsigned char cls;
        if (index < classCount) cls = u8(self, static_cast<U>(index) * 40u + 0xa7);
        else cls = 0xff;
        if (kt_is_jedi_005be4a0(nullptr, nullptr, cls) != 0) {
            last = recovered_talent_known_power_005ac5d0(self, nullptr, a1, a2, a3, a4, a5,
                                                         static_cast<I>(index), a6, a7);
            if (last == 1) return 1;
        }
        ++index;
    } while (index < classCount);
    return last;
}

// ------------------------------------------------------------------ 0056B7E0
extern "C" void __fastcall recovered_byte_list_allocate_owned_0056b7e0(void* self, void*, int capacity) {
    void* old = *reinterpret_cast<void**>(self);
    set_u32(self, 8, static_cast<U>(capacity));
    void* fresh = live_operator_new(static_cast<U>(capacity));
    *reinterpret_cast<void**>(self) = fresh;
    const I count = static_cast<I>(u32(self, 4));
    for (I i = 0; i < count; ++i) {
        static_cast<unsigned char*>(fresh)[i] = static_cast<unsigned char*>(old)[i];
    }
    if (old) live_free(old);
}

// ------------------------------------------------------------------ 005704F0
extern "C" void __fastcall recovered_byte_list_add_owned_005704f0(void* self, void*, int value) {
    if (u32(self, 4) == u32(self, 8)) {
        U capacity = u32(self, 8);
        capacity = capacity ? capacity * 2u : 0x10u;
        recovered_byte_list_allocate_owned_0056b7e0(self, nullptr, static_cast<int>(capacity));
    }
    const U index = u32(self, 4);
    static_cast<unsigned char*>(*reinterpret_cast<void**>(self))[index] = static_cast<unsigned char>(value);
    set_u32(self, 4, index + 1);
}

}  // namespace kotor_talent


