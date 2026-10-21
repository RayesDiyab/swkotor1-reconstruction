// 0064A870: collect class spell variants and append original 38h action rows.
#include "56f214f303-menu_projection.h"
#include "98d2400bef-menu_rows.h"
#include "6bff443bba-array_iterator.h"
#include "63176f6f15-startup_string.h"
#include "10aad36f4a-resources.h"
#include "8e5b891251-allocator_live.h"
#include "67a5d47ab7-m15b_stats.h"
#include "21be0cd701-m15_l0.h"
#include <cstring>

namespace resource_ref_assignment {
menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*, void*,
    menu_resources::Ref*, const menu_resources::Ref*);
}
namespace menu_tables_m41 { void __fastcall LiveContentDestroy_0059ea30(void*, void*); }
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
extern void* kotor_cell_rules_007a3a28;
void* __fastcall server_get_module_004ae6b0(void*, void*);
void* __fastcall module_get_area_004c30f0(void*, void*);
void __fastcall kotor_fastcall_004f7920(void*, void*, std::int32_t);
std::int32_t __fastcall recovered_spell_adjusted_force_points_0059b8a0(
    void*, void*, std::uint32_t, std::uint32_t);
void __fastcall w1x_pointer_list_insert_00616940(void*, void*, unsigned, unsigned);
kotor_reconstruction::StartupString* __fastcall kotor_fastcall_005edeb0(
    void*, void*, kotor_reconstruction::StartupString*, std::uint32_t) noexcept(false);
}

namespace {
using U = std::uint32_t;
using I = std::int32_t;
using H = std::uint16_t;
using String = kotor_reconstruction::StartupString;
void* pointer(U value) { return reinterpret_cast<void*>(value); }
U word(const void* value) { return reinterpret_cast<U>(value); }
template<class T> T load(const void* base, U offset) {
    T value;
    std::memcpy(&value, pointer(word(base) + offset), sizeof value);
    return value;
}
template<class T> void store(void* base, U offset, T value) {
    std::memcpy(pointer(word(base) + offset), &value, sizeof value);
}
struct Scratch {
    U data = 0;
    I count = 0;
    I capacity = 0;
    ~Scratch() noexcept(false) { live_free(pointer(data)); }
};
static_assert(sizeof(Scratch) == 12, "original three-word scratch lists");
template<class T> void append(Scratch& list, T value) {
    if (list.count == list.capacity) {
        const U old = list.data;
        const U capacity = list.capacity != 0 ? static_cast<U>(list.capacity) * 2u : 16u;
        list.capacity = static_cast<I>(capacity); // Original stores capacity before allocation.
        list.data = word(live_operator_new(capacity * static_cast<U>(sizeof(T))));
        for (I index = 0; index < list.count; ++index) {
            const U offset = static_cast<U>(index) * static_cast<U>(sizeof(T));
            store<T>(pointer(list.data), offset, load<T>(pointer(old), offset));
        }
        if (old != 0) live_free(pointer(old));
    }
    store<T>(pointer(list.data), static_cast<U>(list.count) * static_cast<U>(sizeof(T)), value);
    list.count = static_cast<I>(static_cast<U>(list.count) + 1u);
}
struct StringSlot {
    alignas(String) unsigned char raw[sizeof(String)];
    bool live = false;
    String* get() { return reinterpret_cast<String*>(raw); }
    ~StringSlot() noexcept(false) { if (live) get()->~String(); }
};

bool square_at_least_range(void* self, const String* tag, float range) {
    unsigned char result;
    __asm {
        push tag
        mov ecx, self
        xor edx, edx
        call kotor_fastcall_00647dd0
        fld st(0)
        fmul st(0), st(1)
        fcomp dword ptr range
        fnstsw ax
        fstp st(0)
        test ah, 1
        setz result
    }
    return result != 0;
}
}

extern "C" void __fastcall kotor_fastcall_0064a870(void* self, void*, void* output,
    U kind, I hit_points, I signed_word80, U target_filter, U equipment_mask, float range) {
    Scratch spells;
    Scratch ranks;
    Scratch families; // 0064A870 [esp+4Ch] (16-bit): (subradial % 1000) / 10, the dedupe key
    Scratch ids;      // 0064A870 [esp+40h] (32-bit): the creature's spell id, stored in the row (+8)
    void* const server = load<void*>(kotor_menu_window_cells::manager_007a39fc, 8);
    void* const module = server_get_module_004ae6b0(server, nullptr);
    if (load<U>(module_get_area_004c30f0(module, nullptr), 0x2B0) != 0) return;

    for (I input = 0; input < load<I>(self, 4); ++input) {
        const U id = load<U>(load<void*>(self, 0), static_cast<U>(input) * 4u);
        void* const spell = kotor_m15b_spell_array_get_spell_0059b6d0(
            load<void*>(kotor_cell_rules_007a3a28, 0x8C), nullptr, static_cast<I>(id));
        if (spell == nullptr || (load<U>(spell, 0x188) & target_filter) != 0) continue;
        const I variants = load<std::uint8_t>(spell, 0x130);
        for (I variant = 0; variant < variants; ++variant) {
            const auto index = static_cast<std::uint8_t>(variant);
            const U ten_base = (kotor_fastcall_0059b6a0(spell, nullptr, index) / 10u) * 10u;
            const U rank = kotor_fastcall_0059b6a0(spell, nullptr, index) - ten_base;
            const U thousand_base = (kotor_fastcall_0059b6a0(spell, nullptr, index) / 1000u) * 1000u;
            const I action_kind = static_cast<I>(kotor_fastcall_0059b6a0(spell, nullptr, index) - thousand_base) / 10;
            const I class_kind = static_cast<I>(kotor_fastcall_0059b6a0(spell, nullptr, index)) / 1000;
            if (static_cast<U>(class_kind) != kind) continue;
            bool found = false;
            for (I position = 0; position < spells.count; ++position) {
                if (found) break;
                const U half_offset = static_cast<U>(position) * 2u;
                // 0064A9FA..0064AA09: the key compared is the family ([esp+10h]), not the spell id.
                const H old_family = load<H>(pointer(families.data), half_offset);
                if (old_family == static_cast<H>(action_kind)) {
                    found = true;
                    if (static_cast<std::int16_t>(load<H>(pointer(ranks.data), half_offset)) <
                        static_cast<std::int16_t>(rank)) {
                        store<H>(pointer(ranks.data), half_offset, static_cast<H>(rank));
                        store<U>(pointer(spells.data), static_cast<U>(position) * 4u, word(spell));
                        store<U>(pointer(ids.data), static_cast<U>(position) * 4u, id); // 0064AA1A..2D: [esp+14h]
                    }
                } else if (static_cast<std::int16_t>(old_family) > static_cast<std::int16_t>(action_kind)) {
                    found = true;
                    kotor_m15_array_i16_insert_006169a0(&ranks, nullptr, rank, position);
                    kotor_m15_array_i16_insert_006169a0(&families, nullptr, static_cast<U>(action_kind), position); // 0064AA44..4E
                    w1x_pointer_list_insert_00616940(&spells, nullptr, word(spell), static_cast<U>(position));
                    w1x_pointer_list_insert_00616940(&ids, nullptr, id, static_cast<U>(position)); // 0064AA5E..68
                }
            }
            if (!found) {
                append<U>(spells, word(spell));
                append<H>(ranks, static_cast<H>(rank));
                append<H>(families, static_cast<H>(action_kind)); // 0064ABD9..E2
                append<U>(ids, id);                                // 0064AC52..56
            }
        }
    }

    U next = load<U>(output, 4) + 1u;
    U offset = load<U>(output, 4) * 0x38u;
    const I rows = spells.count;
    for (I index = 0; index < rows; ++index, ++next, offset += 0x38u) {
        void* const spell = load<void*>(pointer(spells.data), static_cast<U>(index) * 4u);
        bool enabled = true;
        if (next == 0) {
            void* const data = load<void*>(output, 0);
            if (data != nullptr) {
                c2da_destroy_array_006fa852(data, 0x38, load<U>(data, 0xFFFFFFFCu),
                    &menu_tables_m41::LiveContentDestroy_0059ea30);
                live_free(pointer(word(data) - 4u));
                store<U>(output, 0, 0);
                store<U>(output, 8, 0);
            }
        } else {
            kotor_fastcall_004f7920(output, nullptr, static_cast<I>(next));
        }
        store<U>(output, 4, next);
        {
            StringSlot text;
            String* const value = kotor_fastcall_005edeb0(
                load<void*>(kotor_menu_window_cells::manager_007a39fc, 4), nullptr,
                text.get(), load<U>(spell, 8));
            text.live = true;
            reinterpret_cast<String*>(pointer(load<U>(output, 0) + offset))->Assign005e5c50(value);
        }
        store<U>(pointer(load<U>(output, 0)), offset + 8u,
            load<U>(pointer(ids.data), static_cast<U>(index) * 4u) | 0x20000000u); // 0064AD6F..80
        menu_resources::Ref scratch;
        resource_ref_assignment::assign_00406120(
            reinterpret_cast<menu_resources::Ref*>(pointer(load<U>(output, 0) + offset + 0x20u)), nullptr,
            &scratch, reinterpret_cast<const menu_resources::Ref*>(pointer(word(spell) + 0x10u)));
        void* flags = pointer(load<U>(output, 0) + offset + 0x30u);
        store<U>(flags, 0, load<U>(flags, 0) & 0xFFFFFFE1u);
        const I cost = recovered_spell_adjusted_force_points_0059b8a0(
            spell, nullptr, static_cast<U>(signed_word80), 1);
        if (static_cast<U>(cost) > static_cast<U>(hit_points)) {
            flags = pointer(load<U>(output, 0) + offset + 0x30u);
            store<U>(flags, 0, (load<U>(flags, 0) & 0xFFFFFFE3u) | 2u);
            enabled = false;
        }
        if (square_at_least_range(self,
            reinterpret_cast<const String*>(pointer(word(spell) + 0x20u)), range)) {
            flags = pointer(load<U>(output, 0) + offset + 0x30u);
            store<U>(flags, 0, (load<U>(flags, 0) & 0xFFFFFFE9u) | 8u);
            enabled = false;
        }
        if ((load<U>(spell, 0x18C) & equipment_mask) != 0) {
            flags = pointer(load<U>(output, 0) + offset + 0x30u);
            store<U>(flags, 0, (load<U>(flags, 0) & 0xFFFFFFE5u) | 4u);
            enabled = false;
        } else if (enabled) {
            const U required = load<U>(spell, 0x190);
            if ((required & equipment_mask) != required) {
                flags = pointer(load<U>(output, 0) + offset + 0x30u);
                store<U>(flags, 0, (load<U>(flags, 0) & 0xFFFFFFE7u) | 6u);
                enabled = false;
            }
        }
        flags = pointer(load<U>(output, 0) + offset + 0x30u);
        const U previous = load<U>(flags, 0);
        store<U>(flags, 0, previous ^ ((previous ^ static_cast<U>(enabled)) & 1u));
    }
}
