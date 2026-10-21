// Original swkotor.exe 005A0940 CSWSDialog::ClearAllPlayerHostileActions.
//
// Entry-to-return reconstruction. Original thiscall shape: ECX = dialog (the
// body ignores it), no stack arguments, RET 0, no meaningful EAX. The selected
// composition symbol is @kotor_fastcall_005a0940@8.
//
// This TU defines only 005A0940. All callees are existing selected providers;
// none is implemented, stubbed, trapped or replaced here.

#include <cstdint>
#include <cstring>

using U32 = std::uint32_t;
using I32 = std::int32_t;

static_assert(sizeof(void*) == 4, "original x86 ABI");

// Selected data cell for [007A39FC].
namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
}

// Existing selected providers. Declarations are at global scope with external
// linkage; do not move them into the anonymous namespace.
extern "C" {
unsigned __fastcall kotor_fastcall_004aea40(void*, void*);
void* __fastcall kotor_fastcall_004ae910(void*, void*, U32);
void* __fastcall recovered_object_array_004aed70(void*, void*);
void* __fastcall server_get_module_004ae6b0(void*, void*);
void* __fastcall module_get_area_004c30f0(void*, void*);
unsigned char __fastcall kotor_fastcall_004d8230(void*, void*, U32, void**);
unsigned __fastcall recovered_object_get_reputation_0057cb80(void*, void*, unsigned, unsigned);
void __fastcall w5_object_clear_all_actions_004ccd80(void*, void*, unsigned);
void* __fastcall recovered_creature_client_creature_004efb20(void*, void*);
void __fastcall kotor_fastcall_0063d470(void*, void*);
}

namespace {

constexpr unsigned char k_object_found_0074666c = 0x00u;

inline U32 load_u32(const void* base, std::uintptr_t offset) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    return *reinterpret_cast<const U32*>(address);
}

inline void* load_ptr(const void* base, std::uintptr_t offset) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    return *reinterpret_cast<void* const*>(address);
}

// [ [007A39FC] + 8 ]: server application pointer, reloaded for each original use.
inline void* server_app() {
    return load_ptr(kotor_menu_window_cells::manager_007a39fc, 8);
}

// Original server-object vtable +0x30 (CSWSCreature self-return for the creature
// case; the selected runtime object supplies the actual slot).
inline void* server_creature(void* object) {
    void** const vtable = *reinterpret_cast<void***>(object);
    using Fn = void* (__fastcall*)(void*, void*);
    return reinterpret_cast<Fn>(vtable[0x30 / 4])(object, nullptr);
}

}  // namespace

extern "C" void __fastcall kotor_fastcall_005a0940(void* /*dialog*/, void* /*unused_edx*/) {
    // 005A0943..4E: manager/server reload, GetPlayerCreatureId.
    void* server = server_app();
    const U32 player_id = kotor_fastcall_004aea40(server, nullptr);

    // 005A0953..5D: manager/server reload, GetClientObjectByObjectId. The
    // selected 004B2A50 body returns a CSWSPlayer* player-list entry.
    server = server_app();
    void* const player_object = kotor_fastcall_004ae910(server, nullptr, player_id);

    // 005A0962..73: manager/server reload; read player+0x38; GetObjectArray.
    server = server_app();
    const U32 player_field = load_u32(player_object, 0x38);
    void* const object_array = recovered_object_array_004aed70(server, nullptr);

    // 005A0977..86: manager/server reload; GetModule; GetArea.
    server = server_app();
    void* const module = server_get_module_004ae6b0(server, nullptr);
    void* const area = module_get_area_004c30f0(module, nullptr);

    // 005A098B..9D: area +0x190 id base and +0x194 signed count. Both are
    // re-read each iteration by the original.
    const auto ids_slot = reinterpret_cast<const U32* const volatile*>(
        reinterpret_cast<std::uintptr_t>(area) + 0x190);
    const auto count_slot = reinterpret_cast<const volatile I32*>(
        reinterpret_cast<std::uintptr_t>(area) + 0x194);

    for (I32 index = 0; index < *count_slot; ++index) {
        // 005A09A4..B3: object id and CGameObjectArray::GetGameObject.
        const U32 id = (*ids_slot)[index];
        void* object;  // 004D8230 writes it on both found/absent; no source init (original).
        const unsigned char status =
            kotor_fastcall_004d8230(object_array, nullptr, id, &object);
        if (status != k_object_found_0074666c) continue;  // 005A09B8..BE
        if (!object) continue;                             // 005A09C0..C6

        // 005A09C8..E2: three server vtable+0x30 calls. The first is only
        // null-tested; the second is dereferenced at +0xA88 without a null
        // check; the third is used.
        if (!server_creature(object)) continue;
        void* const creature_field = server_creature(object);
        if (load_u32(creature_field, 0xA88) != 0) continue;
        void* const creature = server_creature(object);

        // 005A09EF..FC: reputation compared signed (jg 0Ah).
        const U32 reputation_bits =
            recovered_object_get_reputation_0057cb80(creature, nullptr, player_field, 0);
        I32 reputation;
        std::memcpy(&reputation, &reputation_bits, sizeof reputation);
        if (reputation > 10) continue;

        // 005A09FE..1B: clear server actions, then the client creature's actions.
        w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);
        void* client_creature =
            recovered_creature_client_creature_004efb20(creature, nullptr);
        if (!client_creature) continue;
        // Second call is not null-checked by the original before 0063D470.
        client_creature = recovered_creature_client_creature_004efb20(creature, nullptr);
        kotor_fastcall_0063d470(client_creature, nullptr);
    }
}
