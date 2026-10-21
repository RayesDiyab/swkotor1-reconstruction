// Lane A sub-tier 2b: the four random talent selectors.
//
// All four are reconstructed from the original disassembly with an explicit
// per-instruction frame map (push/pop/call cleanup included).  The original
// heap list headers are allocated with live_operator_new(0xC) and freed with
// live_free; the byte list uses the owned 0056B7E0/005704F0 bodies.
//
// Output mapping used by 00536FE0's 0x18-byte CScriptTalent:
//   +00 *a3, +04 *a4, +08 *a5(byte), +0c *a6, +10 *a7, +14 *a8(byte).
#include "49380ea791-talent_cohort.hpp"

namespace sound_pause29_required { unsigned __stdcall zero_0063e7f0(); }

namespace kotor_talent {
namespace {

inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void* owner_of(void* stats) { return pv(u32(stats, 0x24)); }

struct IntList {
    void* data;
    I count;
    I capacity;
};

struct ByteList {
    void* data;
    I count;
    I capacity;
};

IntList* list_new() {
    IntList* list = static_cast<IntList*>(live_operator_new(0x0c));
    if (list) { list->data = nullptr; list->count = 0; list->capacity = 0; }
    return list;
}
void list_add(IntList* list, U value) { gui_load_array_add_00671c00(list, nullptr, value); }
void list_free(IntList* list) {
    if (!list) return;
    if (list->data) live_free(list->data);
    list->data = nullptr;
    live_free(list);
}

ByteList* byte_list_new() {
    ByteList* list = static_cast<ByteList*>(live_operator_new(0x0c));
    if (list) { list->data = nullptr; list->count = 0; list->capacity = 0; }
    return list;
}
void byte_list_add(ByteList* list, unsigned char value) { recovered_byte_list_add_owned_005704f0(list, nullptr, value); }
void byte_list_free(ByteList* list) {
    if (!list) return;
    if (list->data) live_free(list->data);
    list->data = nullptr;
    live_free(list);
}

int rand_index(int count) { return count > 0 ? crt_rand_006fc447() % count : -1; }

bool dead_like(void* owner) {
    if ((u8(owner, 0x9f0) & 6u) != 6u) {
        if (vcall0(owner, 0x94) != nullptr) return true;
    }
    if (sound_pause29_required::zero_0063e7f0() != 0) return true;
    if (recovered_creature_incapacitated_004ef890(owner, nullptr) != 0) return true;
    return false;
}

// C2DA lookup on the rules table with the original 0x74A1CC "Exclusion" column.
int c2da_rule_table(U row, U which) {
    char column[8];
    frame_LogStringCtor_005e5a90(column, nullptr, "Exclusion");   // 0074A1CC
    void* table = pv(u32(rules(), 0xb8));
    table = pv(u32(table, which));
    int out = 0;
    const int status = static_cast<int>(c2da_current_get_int_00413660(table, nullptr, static_cast<I>(row), column, &out));
    frame_LogStringDtor_005e5c20(column, nullptr);
    if (status != 0 && out != 0) return out;
    return 0;
}

}  // namespace

// ------------------------------------------------------------------ 005AB9E0
extern "C" int __fastcall recovered_talent_special_005ab9e0(void* self, void*, I a1, I a2, I a3, I a4,
                                                            I a5, I a6, I a7, I a8) {
    (void)a8;
    IntList* spells = list_new();
    IntList* values = list_new();
    I weight = -100;
    void* list = pv(u32(self, 0x128));
    const I count = static_cast<I>(u32(list, 4));
    if (count > 0) {
        const unsigned char* data = static_cast<const unsigned char*>(pv(u32(list, 0)));
        for (I i = 0; i < count; ++i) {
            const unsigned char* entry = data + i * 0xc;
            const U spellId = u32(entry, 0);
            const U flag = u32(entry, 4);
            const unsigned char byteValue = entry[8];
            if (flag == 0) continue;
            void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(pv(u32(rules(), 0x8c)), nullptr,
                                                                    static_cast<I>(spellId));
            if (!spell || spellId == 0xffffffffu) continue;
            if (recovered_stats_has_talent_005aaa30(self, nullptr, 0, spellId) == 0) continue;
            if (recovered_object_area_004cb120(owner_of(self), nullptr) != nullptr) {
                if (recovered_talent_compatible_005a5d70(self, nullptr, u32(spell, 0x124),
                                                         static_cast<unsigned>(a1)) == 0) continue;
            }
            const int rule = c2da_rule_table(spellId, 0xf4);
            if (rule != 0 && (static_cast<unsigned>(rule) | static_cast<unsigned>(a7)) != 0) continue;
            if (a2 != -1) {
                const I candidate = static_cast<I>(u32(spell, 0x128));
                if (candidate > weight) {
                    list_free(spells);
                    spells = list_new();
                    list_free(values);
                    values = list_new();
                    weight = candidate;
                } else if (candidate != weight) {
                    continue;
                }
            }
            list_add(spells, spellId);
            list_add(values, byteValue);
            WindowCStr005e5670(spell, nullptr);
        }
    }
    int result = 0;
    if (spells->count > 0) {
        const int index = rand_index(spells->count);
        *reinterpret_cast<I*>(static_cast<std::uintptr_t>(a3)) = 0;
        *reinterpret_cast<I*>(static_cast<std::uintptr_t>(a4)) =
            static_cast<I>(static_cast<U*>(spells->data)[index]);
        *reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(a5)) = 0xfe;
        *reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(a6)) =
            static_cast<unsigned char>(static_cast<U*>(values->data)[index]);
        result = 1;
    }
    list_free(spells);
    list_free(values);
    return result;
}

// ------------------------------------------------------------------ 005AC5D0
extern "C" int __fastcall recovered_talent_known_power_005ac5d0(void* self, void*, I a1, I a2, I a3, I a4,
                                                                I a5, I a6, I a7, I a8) {
    (void)a7;
    IntList* spells = list_new();
    ByteList* classes = byte_list_new();
    const unsigned char classIndex = static_cast<unsigned char>(a6);
    const unsigned classCount = u8(self, 0x89);
    U spellCount = 0;
    if (classIndex < classCount) spellCount = u32(self, static_cast<U>(classIndex) * 40u + 0x90);
    for (unsigned char i = 0;
         static_cast<unsigned short>(i) < static_cast<unsigned short>(spellCount);
         ++i) {
        U spellId = 0xffffffffu;
        if (classIndex < classCount) {
            void* data = pv(u32(self, static_cast<U>(classIndex) * 40u + 0x8c));
            if (static_cast<U>(i) < u32(self, static_cast<U>(classIndex) * 40u + 0x90)) spellId = u32(data, static_cast<U>(i) * 4);
        }
        void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(pv(u32(rules(), 0x8c)), nullptr,
                                                                static_cast<I>(spellId));
        if (!spell) continue;
        void* owner = owner_of(self);
        const U flag = u32(owner, 0xa88);                                   // 005AC6C4, pushed first (arg2)
        const I level_word = static_cast<std::int16_t>(u16(self, 0x17e));   // 005AC6D2 movsx, pushed last (arg1)
        // 005AC6F2 cmp eax,edx / jg: signed cost > fp (unlike 005A5550).
        const int cost = static_cast<int>(recovered_spell_adjusted_force_points_0059b8a0(spell, nullptr, level_word, flag));
        const int fp = static_cast<std::int16_t>(static_cast<unsigned short>(u16(self, 0x126) + u16(self, 0x124)));
        if (cost > fp) continue;
        if (spellId == static_cast<U>(a8)) continue;
        if (recovered_stats_has_talent_005aaa30(self, nullptr, 0, spellId) == 0) continue;
        if (recovered_object_area_004cb120(owner, nullptr) != nullptr) {
            if (recovered_talent_compatible_005a5d70(self, nullptr, u32(spell, 0x124),
                                                     static_cast<unsigned>(a1)) == 0) continue;
        }
        if (a2 != -1) {
            const I candidate = static_cast<I>(u32(spell, 0x128));
            if (candidate > -100) {
                list_free(spells);
                spells = list_new();
                byte_list_free(classes);
                classes = byte_list_new();
            } else if (candidate != -100) {
                continue;
            }
        }
        list_add(spells, spellId);
        byte_list_add(classes, classIndex);
        WindowCStr005e5670(spell, nullptr);
    }
    int result = 0;
    if (spells->count > 0) {
        const int index = rand_index(spells->count);
        *reinterpret_cast<I*>(static_cast<std::uintptr_t>(a3)) = 0;
        *reinterpret_cast<I*>(static_cast<std::uintptr_t>(a4)) =
            static_cast<I>(static_cast<U*>(spells->data)[index]);
        *reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(a5)) =
            static_cast<unsigned char*>(classes->data)[index];
        result = 1;
    }
    list_free(spells);
    byte_list_free(classes);
    return result;
}

// ------------------------------------------------------------------ 005ABD00
extern "C" int __fastcall recovered_talent_power_item_005abd00(void* self, void*, I a1, I a2, I a3, I a4,
                                                               I a5, I a6, I a7, I a8, I a9) {
    (void)a5;
    IntList* spells = list_new();
    IntList* values = list_new();
    IntList* properties = list_new();
    I weight = -100;
    void* owner = owner_of(self);
    void* inventory = pv(u32(owner, 0xa2c));

    auto add_weighted = [&](void* item, void* spell, U spellId, int propertyIndex) {
        if (a2 != -1) {
            const I candidate = static_cast<I>(u32(spell, 0x128));
            if (candidate > weight) {
                list_free(spells); spells = list_new();
                list_free(values); values = list_new();
                list_free(properties); properties = list_new();
                weight = candidate;
            } else if (candidate != weight) {
                return;
            }
        }
        list_add(spells, spellId);
        list_add(values, u32(item, 0x14));
        list_add(properties, static_cast<U>(propertyIndex));
    };

    // Inventory branch: 005AB980 suitability + 005A5DF0 inclusion.
    auto add_inventory = [&](void* item, void* spell, U spellId, int propertyIndex) {
        if (recovered_talent_power_suitable_005ab980(
                self, nullptr, static_cast<I>(spellId), static_cast<I>(reinterpret_cast<std::uintptr_t>(spell)),
                a1, a2, 0, 0, a9, -1) != 1) return;
        if (recovered_talent_inclusion_005a5df0(self, nullptr, 0, static_cast<I>(spellId), a8) != 1) return;
        add_weighted(item, spell, spellId, propertyIndex);
    };

    // Repository branch: only 005A5D70 compatible(spell+0x124, a1), then weight/add.
    auto add_repository = [&](void* item, void* spell, U spellId, int propertyIndex) {
        if (recovered_talent_compatible_005a5d70(self, nullptr, u32(spell, 0x124),
                                                 static_cast<unsigned>(a1)) == 0) return;
        add_weighted(item, spell, spellId, propertyIndex);
    };

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
                const U spellId = u16(prop, 2);
                void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(pv(u32(rules(), 0x8c)), nullptr,
                                                                        static_cast<I>(spellId));
                if (!spell) continue;
                add_inventory(item, spell, spellId, static_cast<int>(i));
            }
        }
    }
    void* repository = recovered_creature_inventory_004ef770(owner, nullptr, 1);
    if (repository) {
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
                const U spellId = u16(prop, 2);
                void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(pv(u32(rules(), 0x8c)), nullptr,
                                                                        static_cast<I>(spellId));
                if (!spell) continue;
                add_repository(item, spell, spellId, static_cast<int>(j));
            }
        }
    }
    int result = 0;
    if (spells->count > 0) {
        const int index = rand_index(spells->count);
        *reinterpret_cast<I*>(static_cast<std::uintptr_t>(a3)) = 0;
        *reinterpret_cast<I*>(static_cast<std::uintptr_t>(a4)) =
            static_cast<I>(static_cast<U*>(spells->data)[index]);
        *reinterpret_cast<I*>(static_cast<std::uintptr_t>(a6)) =
            static_cast<I>(static_cast<U*>(values->data)[index]);
        *reinterpret_cast<I*>(static_cast<std::uintptr_t>(a7)) =
            static_cast<I>(static_cast<U*>(properties->data)[index]);
        result = 1;
    }
    list_free(spells);
    list_free(values);
    list_free(properties);
    return result;
}

// ------------------------------------------------------------------ 005AC370
extern "C" int __fastcall recovered_talent_feat_005ac370(void* self, void*, I a1, I a2, I a3, I a4,
                                                         I a5, I a6, I a7) {
    (void)a5;
    IntList* chosen = list_new();
    int weight = -100;
    const I n = static_cast<I>(u32(self, 4));
    const U* feats = reinterpret_cast<const U*>(pv(u32(self, 0)));
    unsigned short index = 0;
    if (n > 0) {
    for (;;) {
        const U feat = u16(feats, static_cast<U>(index) * 2);
        void* info = kotor_rules::kotor_fastcall_00550c00(rules(), nullptr, static_cast<unsigned short>(feat));
        if (info) {
            if (recovered_talent_compatible_005a5d70(self, nullptr, u32(info, 0x10), static_cast<unsigned>(a1)) != 0) {
                const int rule = c2da_rule_table(feat, 0xf8);
                if (!(rule != 0 && (static_cast<unsigned>(rule) | static_cast<unsigned>(a6)) != 0) &&
                    static_cast<U>(a7) != feat &&
                    !dead_like(owner_of(self)) &&
                    recovered_stats_has_feat_005a6630(self, nullptr, feat) != 0 &&
                    (kt_feat_remaining_uses_005a6680(self, nullptr, feat) & 0xffu) != 0) {
                    bool add = true;
                    if (a2 != -1) {
                        const int candidate_weight = static_cast<I>(u32(info, 0x14));
                        if (candidate_weight > weight) {
                            list_free(chosen);
                            chosen = list_new();
                            weight = candidate_weight;
                        } else if (candidate_weight != weight) {
                            add = false;
                        }
                    }
                    if (add) list_add(chosen, feat);
                }
            }
        }
        ++index;
        if (index >= n) break;   // original 16-bit counter: n > 0xFFFF loops forever
    }
    }
    int result = 0;
    if (chosen->count > 0) {
        *reinterpret_cast<I*>(static_cast<std::uintptr_t>(a3)) = 1;   // original 005AC57C outType
        const int pick = rand_index(chosen->count);
        const U picked = static_cast<U*>(chosen->data)[pick];
        *reinterpret_cast<I*>(static_cast<std::uintptr_t>(a4)) = static_cast<I>(picked);
        result = 1;
    }
    list_free(chosen);
    return result;
}

}  // namespace kotor_talent

