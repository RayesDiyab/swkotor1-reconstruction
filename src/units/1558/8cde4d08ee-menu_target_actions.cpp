#include "10f5f58e1a-menu_target_imports.h"
#include "ba3b8e7083-menu_action_record.h"
#include <cstddef>
#include <cstring>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
using H = std::int16_t;
using S = kotor_reconstruction::StartupString;
using Ref = menu_resources::Ref;
using Callback = menu_action::Callback;
using Getter = void* (__fastcall*)(void*, void*);
using Predicate = U (__fastcall*)(void*, void*);
void* at(void* p, U off) { return static_cast<unsigned char*>(p) + off; }
const void* at(const void* p, U off) { return static_cast<const unsigned char*>(p) + off; }
template<class T> T load(const void* p, U off) {
    T value; std::memcpy(&value, at(p, off), sizeof value); return value;
}
template<class T> void store(void* p, U off, T value) {
    std::memcpy(at(p, off), &value, sizeof value);
}
template<class F> F slot(void* p, U off) { return load<F>(load<void*>(p, 0), off); }
template<class F, class... A> auto opaque(F fn, A... args) {
    F volatile target = fn; return target(args...);
}
I signed_bits(U value) { I result; std::memcpy(&result, &value, 4); return result; }
U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
void* client() { return load<void*>(kotor_menu_window_cells::manager_007a39fc, 4); }
void* server_app() { return load<void*>(kotor_menu_window_cells::manager_007a39fc, 8); }
void* row(void* list, U byte_offset) { return at(load<void*>(list, 0), byte_offset); }
void callback(void* list, U offset, Callback fn) {
    void* record = at(row(list, offset), 0x0C);
    store<Callback>(record, 0, fn);
    store<U>(record, 4, 0); store<U>(record, 8, 0); store<U>(record, 12, 0);
}
struct TemporaryString {
    alignas(4) unsigned char storage[sizeof(S)];
    bool active = false;
    S* get() { return reinterpret_cast<S*>(storage); }
    ~TemporaryString() noexcept(false) { destroy(); }
    void destroy() { if (active) { active = false; get()->~StartupString(); } }
};
void label(void* list, U offset, U strref) {
    TemporaryString temporary;
    S* value = opaque(kotor_fastcall_005edeb0, client(), nullptr, temporary.get(), strref);
    temporary.active = true;
    static_cast<S*>(row(list, offset))->Assign005e5c50(value);
    temporary.destroy();
}
void icon(void* list, U offset, const char* text) {
    Ref discarded;
    (void)opaque(unresolved_ref_assign_00406290,
        static_cast<Ref*>(at(row(list, offset), 0x20)), nullptr, &discarded, text);
}
void rules_icon(void* list, U offset) {
    S* text = static_cast<S*>(at(load<void*>(kotor_cell_rules_007a3a28, 0xB4), 0xD0));
    Ref discarded;
    (void)opaque(async_ref_assign_string_004061f0,
        static_cast<Ref*>(at(row(list, offset), 0x20)), nullptr, &discarded, text);
}
void* server_cast(void* object, U cast_slot) {
    if (!opaque(kotor_m15a_object_get_server_object_0063d4b0, object, nullptr)) return nullptr;
    void* fresh = opaque(kotor_m15a_object_get_server_object_0063d4b0, object, nullptr);
    return opaque(&slot<Getter>, fresh, cast_slot)(fresh, nullptr);
}

// These are the three original 12-byte local array records. Their reverse
// destruction order also applies when an allocation or row construction throws.
struct LocalArray {
    void* data;
    U count;
    U capacity;
    LocalArray() { count = 0; capacity = 0; data = nullptr; }
    ~LocalArray() noexcept(false) { opaque(live_free, data); }
};
static_assert(sizeof(LocalArray) == 12, "original array record");
template<class T> void append(LocalArray& array, T value) {
    if (array.count == array.capacity) {
        const U capacity = array.capacity ? array.capacity * 2u : 16u;
        void* old = array.data;
        array.capacity = capacity;
        void* fresh = opaque(live_operator_new, capacity * static_cast<U>(sizeof(T)));
        array.data = fresh;
        for (I i = 0; i < signed_bits(array.count); ++i)
            store<T>(fresh, static_cast<U>(i) * sizeof(T), load<T>(old, static_cast<U>(i) * sizeof(T)));
        if (old) opaque(live_free, old);
    }
    store<T>(array.data, array.count * sizeof(T), value);
    ++array.count;
}

// The original rounds once directly into the final float argument. Keep that
// x87 sequence and integer argument transport together, without a float reload.
void rows_at_distance(void* self_value, void* target_value, void* stats_value,
                      void* list_value, U hp_word, U filter_word, U mask_word) {
    __asm {
        mov eax, target_value
        mov ecx, self_value
        fld dword ptr [eax+24h]
        fsub dword ptr [ecx+24h]
        push ecx
        fld dword ptr [eax+28h]
        fsub dword ptr [ecx+28h]
        fld dword ptr [eax+2Ch]
        fsub dword ptr [ecx+2Ch]
        fld st(0)
        fmul st(0), st(1)
        fld st(2)
        fmul st(0), st(3)
        faddp st(1), st(0)
        fld st(3)
        fmul st(0), st(4)
        faddp st(1), st(0)
        fstp dword ptr [esp]
        push mask_word
        push filter_word
        fstp st(0)
        push hp_word
        fstp st(0)
        push 1
        fstp st(0)
        push list_value
        mov ecx, stats_value
        call kotor_fastcall_0064af10
    }
}
}

extern "C" void __fastcall kotor_fastcall_006191f0(void* self, void*, void* target, void* output) {
    U filter = 0;
    void* creature = server_cast(self, 0x30);
    if (!creature) return;
    if (load<U>(opaque(recovered_object_area_004cb120, creature, nullptr), 0x2B0)) return;
    if (opaque(&slot<Getter>, target, U{0x2C})(target, nullptr)) {
        void* cast = opaque(&slot<Getter>, target, U{0x2C})(target, nullptr);
        if (load<std::uint16_t>(load<void*>(cast, 0x2F8), 0x24) == 5) filter = 2;
        else {
            cast = opaque(&slot<Getter>, target, U{0x2C})(target, nullptr);
            if (load<std::uint16_t>(load<void*>(cast, 0x2F8), 0x24) == 6) filter = 1;
        }
    } else filter = 4;
    const U equipment = opaque(kotor_fastcall_004efa80, creature, nullptr);
    U hp;
    if (load<U>(creature, 0xA9C)) {
        using CurrentHP = decltype(menu_server::Slots::current_hp_09c);
        hp = opaque(&slot<CurrentHP>, creature, U{0x9C})(creature, nullptr, 0);
    } else {
        void* state = load<void*>(creature, 0xA74);
        const std::uint16_t bonus_hp = load<std::uint16_t>(state, 0x126);
        const std::uint16_t base_hp = load<std::uint16_t>(state, 0x124);
        hp = static_cast<std::uint16_t>(bonus_hp + base_hp);
    }
    void* stats = load<void*>(self, 0x2F8);
    const bool native_items = load<std::uint16_t>(stats, 0x24) == 5;
    const I first = load<I>(output, 4);
    const U hp_signed = static_cast<U>(static_cast<I>(static_cast<H>(hp)));
    if (native_items) opaque(kotor_fastcall_00618c20, self, nullptr, output, U{1}, filter, equipment);
    else opaque(&rows_at_distance, self, target, stats, output, hp_signed, filter, equipment);
    if (first >= load<I>(output, 4)) return;
    U offset = static_cast<U>(first) * 0x38u;
    I index = first;
    do {
        callback(output, offset, &kotor_fastcall_00616200);
        void* destination = row(output, offset);
        const U target_id = load<U>(target, 4);
        store<U>(destination, 0x1C, target_id);
        index = signed_bits(static_cast<U>(index) + 1u);
        offset += 0x38u;
    } while (index < load<I>(output, 4));
}

extern "C" void __fastcall kotor_fastcall_00619b10(void* self, void*, void*, U, void* output) {
    void* creature = server_cast(self, 0x30);
    if (!creature) return;
    if (load<U>(opaque(recovered_object_area_004cb120, creature, nullptr), 0x2B0)) return;
    const U index = load<U>(output, 4);
    opaque(kotor_fastcall_004fddd0, output, nullptr, signed_bits(index + 1u));
    const U offset = index * 0x38u;
    label(output, offset, 0x177);
    callback(output, offset, &kotor_fastcall_00616800);
    const U target_id = load<U>(self, 4);
    store<U>(row(output, offset), 0x1C, target_id);
    store<U>(row(output, offset), 8, 0x3EB);
    icon(output, offset, "i_attack");
}

extern "C" void __fastcall kotor_fastcall_006837d0(void* self, void*, void* actor, U category, void* output) {
    void* placeable = server_cast(self, 0x48);
    const U restricted = load<U>(opaque(frame_GetServerArea_00605640, load<void*>(self, 0x20), nullptr), 0x2B0);
    const U index = load<U>(output, 4);
    S scratch;
    if (category == 1) {
        if (!placeable || !load<U>(placeable, 0x324) || !load<U>(self, 0x118)) return;
        if (!opaque(kotor_m15b_creature_stats_get_can_use_skill_006477e0,
                    load<void*>(actor, 0x2F8), nullptr, std::uint16_t{6})) return;
        opaque(kotor_fastcall_004fddd0, output, nullptr, signed_bits(index + 1u));
        const U offset = index * 0x38u;
        label(output, offset, 0x149);
        callback(output, offset, &kotor_fastcall_00682dd0);
        void* destination = row(output, offset);
        const U target_id = load<U>(self, 4);
        store<U>(destination, 0x1C, target_id);
        store<U>(row(output, offset), 8, 0x3F3);
        rules_icon(output, offset);
        return;
    }
    if (category != 0) return;
    if (placeable && load<U>(placeable, 0x324)) {
        if (load<U>(self, 0x110) || restricted || !load<U>(self, 0x118)) return;
        opaque(kotor_fastcall_004fddd0, output, nullptr, signed_bits(index + 1u));
        const U offset = index * 0x38u;
        label(output, offset, 0x170);
        store<U>(row(output, offset), 8, 0x3F5);
        callback(output, offset, &kotor_fastcall_006826a0);
        const U target_id = load<U>(self, 4);
        store<U>(row(output, offset), 0x1C, target_id);
        icon(output, offset, "i_attack");
        const U player = opaque(kotor_fastcall_004aea40, server_app(), nullptr);
        void* player_object = opaque(kotor_fastcall_004ae910, server_app(), nullptr, player);
        const U character_id = opaque(kotor_fastcall_005eda50, client(), nullptr, load<U>(player_object, 0x38));
        void* object = opaque(kotor_fastcall_005ed580, client(), nullptr, character_id);
        if (!object || !opaque(&slot<Getter>, object, U{0x2C})(object, nullptr)) return;
        void* character = opaque(&slot<Getter>, object, U{0x2C})(object, nullptr);
        if (load<U>(actor, 4) == character_id || !load<U>(character, 0x3A4)) return;
        if (!opaque(kotor_m15a_creature_get_server_creature_0060fb20, character, nullptr)) return;
        void* server = opaque(kotor_m15a_creature_get_server_creature_0060fb20, character, nullptr);
        if (!opaque(recovered_creature_incapacitated_004ef890, server, nullptr)) return;
        if (!opaque(kotor_fastcall_00682e00, self, nullptr)) return;
        if (load<U>(opaque(kotor_fastcall_00682e00, self, nullptr), 0x218)) return;
        void* destination = row(output, offset);
        store<U>(destination, 0x30, load<U>(destination, 0x30) & ~1u);
        destination = row(output, offset);
        store<U>(destination, 0x30, (load<U>(destination, 0x30) & ~0x12u) | 0x0Cu);
        return;
    }
    if (load<U>(self, 0x110) || restricted || !load<U>(self, 0x118)) return;
    opaque(kotor_fastcall_004fddd0, output, nullptr, signed_bits(index + 1u));
    const U offset = index * 0x38u;
    label(output, offset, 0x170);
    store<U>(row(output, offset), 8, 0x3F5);
    callback(output, offset, &kotor_fastcall_006826a0);
    const U target_id = load<U>(self, 4);
    store<U>(row(output, offset), 0x1C, target_id);
    icon(output, offset, "i_attack");
}

extern "C" void __fastcall kotor_fastcall_00684410(void* self, void*, void* actor, U category, void* output) {
    void* door = server_cast(self, 0x18);
    const U restricted = load<U>(opaque(frame_GetServerArea_00605640, load<void*>(self, 0x20), nullptr), 0x2B0);
    const U index = load<U>(output, 4);
    S scratch;
    if (category == 1) {
        if (!load<U>(self, 0x108) || !door || load<U>(door, 0x2D8)) return;
        if (!opaque(kotor_m15b_creature_stats_get_can_use_skill_006477e0,
                    load<void*>(actor, 0x2F8), nullptr, std::uint16_t{6})) return;
        opaque(kotor_fastcall_004fddd0, output, nullptr, signed_bits(index + 1u));
        const U offset = index * 0x38u;
        label(output, offset, 0x149);
        callback(output, offset, &kotor_fastcall_00683e50);
        void* destination = row(output, offset);
        const U target_id = load<U>(self, 4);
        store<U>(destination, 0x1C, target_id);
        store<U>(row(output, offset), 8, 0x3F3);
        rules_icon(output, offset);
        return;
    }
    if (category != 0) return;
    if (opaque(kotor_m15a_object_get_current_animation_0063c060, self, nullptr) == 0x2742) return;
    if (opaque(kotor_m15a_object_get_current_animation_0063c060, self, nullptr) == 0x2743) return;
    if (load<U>(self, 0x138) || load<U>(self, 0x104) || restricted || !load<U>(self, 0x108)) return;
    opaque(kotor_fastcall_004fddd0, output, nullptr, signed_bits(index + 1u));
    const U offset = index * 0x38u;
    label(output, offset, 0x170);
    store<U>(row(output, offset), 8, 0x3F5);
    callback(output, offset, &kotor_fastcall_00683e90);
    const U target_id = load<U>(self, 4);
    store<U>(row(output, offset), 0x1C, target_id);
    icon(output, offset, "i_attack");
}

extern "C" void __fastcall kotor_fastcall_00691f00(void* self, void*, void* actor, U category, void* output) {
    const U index = load<U>(output, 4);
    if (category != 0 && category != 1) return;
    if (!opaque(kotor_m15b_creature_stats_get_can_use_skill_006477e0,
                load<void*>(actor, 0x2F8), nullptr, std::uint16_t{1})) return;
    if (category == 0 && !opaque(&slot<Predicate>, self, U{0x138})(self, nullptr)) return;
    opaque(kotor_fastcall_004fddd0, output, nullptr, signed_bits(index + 1u));
    const U offset = index * 0x38u;
    label(output, offset, category == 1 ? 0x5FBu : 0x172u);
    callback(output, offset, category == 1 ? &kotor_fastcall_00691950 : &kotor_fastcall_00691900);
    void* destination = row(output, offset);
    const U target_id = load<U>(self, 4);
    store<U>(destination, 0x1C, target_id);
    store<U>(row(output, offset), 8, category == 1 ? 0x402u : 0x3F4u);
    icon(output, offset, category == 1 ? "i_recovermine" : "i_disablemine");
}

extern "C" void __fastcall kotor_fastcall_00682dd0(void* self, void*, U, void*) {
    void* message = opaque(kotor_fastcall_005ed6f0, client(), nullptr);
    if (message) (void)opaque(kotor_fastcall_00677de0, message, nullptr, load<U>(self, 4), U{0x7F000000});
}
extern "C" void __fastcall kotor_fastcall_00683e50(void* self, void*, U, void* actor) {
    if (actor) opaque(kotor_fastcall_0063d470, actor, nullptr);
    void* message = opaque(kotor_fastcall_005ed6f0, client(), nullptr);
    (void)opaque(kotor_fastcall_00677de0, message, nullptr, load<U>(self, 4), U{0x7F000000});
    opaque(kotor_fastcall_005edf30, client(), nullptr);
}

extern "C" void __fastcall kotor_fastcall_00618c20(
    void* self, void*, void* output, U kind, U target_filter, U equipment_mask) {
    LocalArray items;
    LocalArray levels;
    LocalArray groups;
    for (U equipment_slot = 0; equipment_slot != 15; ++equipment_slot) {
        void* application = client();
        const U item_id = load<U>(self, 0x220u + equipment_slot * 4u);
        void* item = opaque(kotor_m15_client_get_item_by_id_005ed5a0, application, nullptr, item_id);
        if (!item) continue;
        const I spell_id = opaque(kotor_fastcall_006467e0, item, nullptr, I{0});
        void* spell = opaque(kotor_m15b_spell_array_get_spell_0059b6d0,
            load<void*>(kotor_cell_rules_007a3a28, 0x8C), nullptr, spell_id);
        if (!spell || (load<U>(spell, 0x188) & target_filter)) continue;
        const U property_count = load<std::uint8_t>(spell, 0x130);
        for (U property_index = 0; property_index < property_count; ++property_index) {
            const I encoded = static_cast<H>(opaque(kotor_fastcall_0059b6a0,
                spell, nullptr, static_cast<std::uint8_t>(property_index)));
            const I quotient = encoded / 10;
            const I level = encoded - quotient * 10;
            const I property_kind = quotient / 100;
            const I group = quotient - static_cast<H>(property_kind) * 100;
            if (static_cast<U>(property_kind) != kind) continue;
            bool found = false;
            for (I i = 0; i < signed_bits(items.count); ++i) {
                if (found) break;
                const I current_group = load<H>(groups.data, static_cast<U>(i) * 2u);
                if (current_group == group) {
                    const I current_level = load<H>(levels.data, static_cast<U>(i) * 2u);
                    found = true;
                    if (current_level < level) {
                        store<H>(levels.data, static_cast<U>(i) * 2u, static_cast<H>(level));
                        store<void*>(items.data, static_cast<U>(i) * 4u, item);
                    }
                } else if (current_group > group) {
                    found = true;
                    opaque(kotor_m15_array_i16_insert_006169a0, &levels, nullptr, static_cast<U>(level), i);
                    opaque(kotor_m15_array_i16_insert_006169a0, &groups, nullptr, static_cast<U>(group), i);
                    opaque(w1x_pointer_list_insert_00616940, &items, nullptr, word(item), static_cast<U>(i));
                }
            }
            if (!found) {
                append(items, item);
                append(levels, static_cast<H>(level));
                append(groups, static_cast<H>(group));
            }
        }
    }
    const U old_count = opaque(&load<U>, output, U{4});
    if (signed_bits(items.count) <= 0) return;
    U offset = old_count * 0x38u;
    U next_count = old_count + 1u;
    for (I index = 0; index < signed_bits(items.count); ++index) {
        void* item = load<void*>(items.data, static_cast<U>(index) * 4u);
        const I spell_id = opaque(kotor_fastcall_006467e0, item, nullptr, I{0});
        void* spell = opaque(kotor_m15b_spell_array_get_spell_0059b6d0,
            load<void*>(kotor_cell_rules_007a3a28, 0x8C), nullptr, spell_id);
        opaque(kotor_fastcall_004fddd0, output, nullptr, signed_bits(next_count));
        TemporaryString temporary;
        S* name = opaque(feedback_spell_name_0059b940, spell, nullptr, temporary.get());
        temporary.active = true;
        static_cast<S*>(row(output, offset))->Assign005e5c50(name);
        temporary.destroy();
        const U action_id = load<U>(item, 0x14);
        void* destination = row(output, offset);
        store<U>(destination, 8, action_id | 0x20000000u);
        Ref* spell_icon = static_cast<Ref*>(at(spell, 0x10));
        const bool empty_icon = opaque(c2da_equal_text_004060d0, spell_icon, nullptr, "") != 0;
        destination = row(output, offset);
        Ref* source = empty_icon ? static_cast<Ref*>(at(item, 0x138)) : spell_icon;
        Ref discarded;
        (void)opaque(c2da_copy_00406120, static_cast<Ref*>(at(destination, 0x20)), nullptr, &discarded, source);
        destination = row(output, offset);
        store<U>(destination, 0x30, load<U>(destination, 0x30) & 0xFFFFFFE1u);
        void* server_item = opaque(kotor_m15closure_00646c90, item, nullptr);
        void* property = nullptr;
        if (server_item)
            property = opaque(kotor_m15_menu_active_property_00553960, server_item, nullptr, U{0});
        U enabled = 1;
        if (server_item && property &&
            static_cast<std::uint16_t>(opaque(kotor_fastcall_00555a60, server_item, nullptr, std::uint8_t{0})) < 1u) {
            const std::uint16_t type = load<std::uint16_t>(property, 6);
            if (type != 13 && type != 7) {
                destination = row(output, offset);
                store<U>(destination, 0x30, (load<U>(destination, 0x30) & 0xFFFFFFE3u) | 2u);
                enabled = 0;
            }
        }
        if (load<U>(spell, 0x18C) & equipment_mask) {
            destination = row(output, offset);
            store<U>(destination, 0x30, (load<U>(destination, 0x30) & 0xFFFFFFE5u) | 4u);
            enabled = 0;
        }
        const U required_equipment = load<U>(spell, 0x190);
        if ((required_equipment & equipment_mask) != required_equipment) {
            destination = row(output, offset);
            store<U>(destination, 0x30, (load<U>(destination, 0x30) & 0xFFFFFFE7u) | 6u);
            enabled = 0;
        }
        destination = row(output, offset);
        const U first_flags = load<U>(destination, 0x30);
        const U second_flags = load<U>(destination, 0x30);
        store<U>(destination, 0x30, second_flags ^ ((first_flags ^ enabled) & 1u));
        ++next_count;
        offset += 0x38u;
    }
}
