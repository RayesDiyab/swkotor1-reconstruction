#include "6dab7adcb5-menu_cohort.h"
#include "ffee89c921-populate_imports.h"
#include <cstddef>
#include <cstring>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
using B = unsigned char;
void* at(void* p, U n) { return static_cast<B*>(p) + n; }
const void* at(const void* p, U n) { return static_cast<const B*>(p) + n; }
template<class T> T load(const void* p, U n) {
    T value; std::memcpy(&value, at(p, n), sizeof value); return value;
}
template<class T> void store(void* p, U n, T value) {
    std::memcpy(at(p, n), &value, sizeof value);
}
void* client_app() { return load<void*>(menu_continuation::app_manager_007a39fc, 4); }
const menu_resources::Ref* ref(const void* p) {
    return static_cast<const menu_resources::Ref*>(p);
}
static_assert(offsetof(KotorM15ObjectMethods_007516c8, s00c_as_creature_or_self) == 0xc,
              "canonical client object cast slot");
struct StringCleanup {
    frame_log::String* string;
    ~StringCleanup() noexcept(false) { frame_LogStringDtor_005e5c20(string, nullptr); }
};

// Shared instruction sequence in 00685960 and the inlined selected-action paths.
void set_icon(void* button, const menu_resources::Ref* icon, U dimmed) {
    if (gui_load_resref_not_equal_00406dc0(icon, nullptr, ref(at(button, 0x284))) != 0) {
        gui_load_border_params_set_fill_00414c00(at(button, 0x244), nullptr, icon, 0);
        gui_load_border_params_set_fill_00414c00(at(button, 0x2b8), nullptr, icon, 0);
        store<B>(button, 0x714, 0);
    }
    // Original 0073D764=1.0f, 0073EDF0=0.25f; FLD/FSTP preserves these bits.
    const U alpha = dimmed != 0 ? 0x3f800000u : 0x3e800000u;
    store(button, 0x250, alpha);
    store(button, 0x2c4, alpha);
}
void clear_list(void* list) {
    U index = 0;
    while (static_cast<I>(index) < load<I>(list, 4)) {
        kotor_fastcall_0063c000(at(load<void*>(list, 0), index * 0x38u), nullptr);
        ++index; // Original reloads both data and count after each clear callback.
    }
    store<U>(list, 4, 0);
}
}

extern "C" void __fastcall kotor_fastcall_00685960(
    void* self, void*, void* icon, void*, U dimmed, U) {
    set_icon(self, ref(icon), dimmed);
}

// 00689280..00689396 RET8. The string alone owns an unwind state.
extern "C" void __fastcall kotor_fastcall_00689280(
    void* self, void*, U empty, U multiple) {
    if (empty != 0) {
        alignas(kotor_reconstruction::StartupString)
            B string_storage[sizeof(kotor_reconstruction::StartupString)];
        auto* string = reinterpret_cast<frame_log::String*>(string_storage);
        frame_LogStringCtor_005e5a90(string, nullptr, "");
        {
            StringCleanup cleanup{string};
            menu_resources::Ref icon;
            icon.construct_00406d80("");
            if (gui_load_resref_not_equal_00406dc0(&icon, nullptr, ref(at(self, 0x284))) != 0) {
                gui_load_border_params_set_fill_00414c00(at(self, 0x244), nullptr, &icon, 0);
                gui_load_border_params_set_fill_00414c00(at(self, 0x2b8), nullptr, &icon, 0);
                store<B>(self, 0x714, 0);
            }
            store<U>(self, 0x250, 0x3f800000u);
            store<U>(self, 0x2c4, 0x3f800000u);
        }
    }
    const U alpha = empty != 0 ? 0x3f000000u : 0x3f800000u;
    store(self, 0x8c, alpha);
    store(self, 0x100, alpha);
    const U bit = (multiple & 1u) << 1;
    store(self, 0x3cc, (load<U>(self, 0x3cc) & ~2u) | bit);
    store(self, 0x590, (load<U>(self, 0x590) & ~2u) | bit);
}

// 00619DB0..00619DF9 RET8; four table entries, including selector2 -> mask4.
extern "C" void __fastcall kotor_fastcall_00619db0(
    void* self, void*, U category, void* output) {
    switch (category) {
    case 0: kotor_fastcall_00616230(self, nullptr, output); break;
    case 1: kotor_fastcall_006197d0(self, nullptr, output, 1); break;
    case 2: kotor_fastcall_006197d0(self, nullptr, output, 4); break;
    case 3: kotor_fastcall_006197d0(self, nullptr, output, 2); break;
    default: break; // The original unsigned selector >3 returns without a call.
    }
}

// 00689410..00689608 RET0Ch. Target menu owns three action lists/buttons.
extern "C" void __fastcall kotor_fastcall_00689410(
    void* self, void*, void* creature, U disabled, void* target) {
    const std::uint8_t kind = kotor_fastcall_0060fc00(creature, nullptr, target);
    store(self, 0x1aea, kind);
    for (U row = 0; row < 3; ++row) {
        void* list = at(self, row * 0xcu);
        clear_list(list);
        kotor_fastcall_00619c20(creature, nullptr, target, row, list);
    }
    for (U row = 0; row < 3; ++row) {
        void* list = at(self, row * 0xcu);
        void* button = at(self, 0x54u + row * 0x71cu);
        const I count = load<I>(list, 4);
        if (count <= 0) {
            kotor_fastcall_00689280(button, nullptr, 1, 0);
            continue;
        }
        const auto signed_kind = static_cast<std::int8_t>(load<std::uint8_t>(self, 0x1aea));
        const U bank = static_cast<U>(static_cast<I>(signed_kind)) + 3u;
        const U selected = load<U>(self, (bank * 3u + row) * 4u);
        U index = 0;
        void* data = load<void*>(list, 0);
        while (static_cast<I>(index) < count && load<U>(data, index * 0x38u + 8u) != selected)
            ++index;
        const bool found = static_cast<I>(index) < count;
        kotor_fastcall_00689280(button, nullptr, 0, count > 1 ? 1u : 0u);
        data = load<void*>(list, 0);
        void* action = at(data, found ? index * 0x38u : 0u);
        const U dimmed = (load<B>(action, 0x30) & 1u) != 0 && disabled == 0 ? 1u : 0u;
        set_icon(button, ref(at(action, 0x20)), dimmed);
    }
    kotor_fastcall_00685cb0(self, nullptr, target);
    kotor_fastcall_00686090(self, nullptr, creature, target);
}

// 00689D80..0068A00B RET0. Lookup still occurs before the null-player exit.
extern "C" void __fastcall kotor_fastcall_00689d80(void* self, void*) {
    void* player = kotor_fastcall_00635460(
        kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr);
    void* server = player == nullptr ? nullptr :
        kotor_m15a_creature_get_server_creature_0060fb20(player, nullptr);
    auto* lookup_manager = menu_continuation::app_manager_007a39fc;
    const U target_id = load<U>(self, 0x64);
    void* lookup_app = load<void*>(lookup_manager, 4);
    void* target = kotor_fastcall_005ed580(lookup_app, nullptr, target_id);
    U disabled = 1;
    if (player == nullptr) return;
    if (server != nullptr) {
        auto* table = load<const menu_server::Slots*>(server, 0);
        if (table->dead_094(server, nullptr) == 0 &&
            recovered_creature_incapacitated_004ef890(server, nullptr) == 0)
            disabled = 0;
    }
    const U player_id = load<U>(player, 4);
    if (player_id != load<U>(self, 0x68)) store(self, 0x68, player_id);
    void* target_creature = nullptr;
    if (target != nullptr) {
        auto* table = load<const KotorM15ObjectMethods_007516c8*>(target, 0);
        target_creature = table->s00c_as_creature_or_self(target, nullptr);
    }
    for (U row = 0; row < 6; ++row) {
        void* list = at(self, 0x74u + row * 0xcu);
        clear_list(list);
        kotor_fastcall_00619db0(player, nullptr, row, list);
    }
    kotor_fastcall_00689410(at(self, 0xbc), nullptr, player, disabled, target_creature);
    for (U row = 0; row < 4; ++row) {
        void* list = at(self, 0x74u + row * 0xcu);
        void* button = at(self, 0x772cu + row * 0x71cu);
        const I count = load<I>(list, 4);
        if (count <= 0) {
            kotor_fastcall_00689280(button, nullptr, 1, 0);
            continue;
        }
        const U selected = load<U>(self, 0x1bacu + row * 4u);
        U index = 0;
        void* data = load<void*>(list, 0);
        while (static_cast<I>(index) < count && load<U>(data, index * 0x38u + 8u) != selected)
            ++index;
        const bool found = static_cast<I>(index) < count;
        kotor_fastcall_00689280(button, nullptr, 0, count > 1 ? 1u : 0u);
        data = load<void*>(list, 0);
        void* action = at(data, found ? index * 0x38u : 0u);
        const U dimmed = (load<B>(action, 0x30) & 1u) != 0 && disabled == 0 ? 1u : 0u;
        if (found) set_icon(button, ref(at(action, 0x20)), dimmed);
        else kotor_fastcall_00685960(button, nullptr, at(action, 0x20), action, dimmed, 0);
    }
    kotor_fastcall_00686e20(self, nullptr);
}
