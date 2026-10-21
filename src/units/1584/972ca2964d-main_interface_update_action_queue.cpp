// Original 0068A010 — CSWGuiMainInterface::UpdateActionQueue
// Full ordinary-path reconstruction.
//
// Span 0068A010..0068A304; thiscall ECX=this, RET 0.
//
// 2026-10-20 actionqueue fix (live c39 AV): CClientExoApp is loaded from
// [app+4] (was &app[+4]); list positions advance in place through 005E9B90
// (its returned data was being stored as the position); an empty queue skips
// to the clear loop; 00686FB0 arg0 is the combat round ESI (callee-unused).
// Export @kotor_fastcall_0068a010@8.
//
// Normal path: party/player/creature -> combat round -> icon queue walk with
// 004D2B30/00686FB0/00685610, second list and clear loop. E+0x10 is incremented
// by 00685610, not here; B8b intentionally does not set the shown flag.
//
// SEH trace correction (ASTRA-QUEUE-UNWIND-RULING.md): pRN=E+0x54;
// __CallSettingFrame computes EAX=pRN+0xC, spills that POINTER VALUE into the
// helper local at [H-4], then `mov EBP,[H-4]` loads that saved pointer value.
// EBP=pRN+0xC=E+0x60, not *(pRN+0xC). Funclets [ebp-0x34]/[ebp-0x3c] therefore
// address E+0x2C/E+0x24 exactly. The prior "original EH defect" claim and the
// intentional-divergence narrative are withdrawn; they were a symbolic
// double-dereference error, not an original bug. Normal and unwind cleanup
// order E+0x24 then E+0x2C is correct. Source keeps ordinary /EHs (required
// for the extern-"C" RAII body) and two real CExoString locals; no hand
// funclets, traps or behavior rewrite.

#include <cstdint>

namespace kotor_menu_10_19 {

struct String {
    unsigned char storage[8];
    String* __thiscall default_005b3190();
    void __thiscall destroy_005e5c20();
};

}  // namespace kotor_menu_10_19

namespace client_services {
struct String;
}

// Exact selected C++ provider import; the prior C-linkage alternatename was
// keyed for a leading-underscore name and left @frame_LogStringCtor_...@12
// unresolved. No stub or fake provider is introduced.
namespace kotor_game_prepare {
client_services::String* __fastcall ConstructStringText005e5a90(
    client_services::String* out, void*, const char* text);
}

namespace {

struct Ref {
    unsigned char storage[16];
};

std::uint32_t load_u32(const void* base, std::uintptr_t offset) {
    return *reinterpret_cast<const std::uint32_t*>(
        reinterpret_cast<std::uintptr_t>(base) + offset);
}

void* load_ptr(const void* base, std::uintptr_t offset) {
    return *reinterpret_cast<void* const*>(
        reinterpret_cast<std::uintptr_t>(base) + offset);
}

struct ScopedString {
    kotor_menu_10_19::String value;

    ScopedString();
    explicit ScopedString(const char* text);
    ~ScopedString();
    ScopedString(const ScopedString&) = delete;
    ScopedString& operator=(const ScopedString&) = delete;
};

__declspec(noinline) ScopedString::ScopedString() {
    value.default_005b3190();
}

__declspec(noinline) ScopedString::ScopedString(const char* text) {
    kotor_game_prepare::ConstructStringText005e5a90(
        reinterpret_cast<client_services::String*>(&value), nullptr, text);
}

__declspec(noinline) ScopedString::~ScopedString() {
    value.destroy_005e5c20();
}

extern "C" void* __fastcall c2da_default_ref_00405ed0(void*, void*);

}  // namespace

namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
}

extern "C" void* __fastcall kotor_fastcall_005ed8b0(void*, void*);
extern "C" void* __fastcall kotor_fastcall_00635460(void*, void*);
extern "C" void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(
    void*, void*);
extern "C" unsigned char __fastcall kotor_fastcall_004d2b30(void*, void*);
extern "C" void __fastcall kotor_fastcall_00686fb0(
    void* self, void*, void* creature, const void* node, const void* action,
    void* unused4, void* outSpecific, void* outGeneric);
extern "C" void __fastcall kotor_fastcall_00685610(
    void* self, void*, int* count, const void* primary, const void* fallback,
    unsigned active, unsigned force);
extern "C" void* __fastcall unresolved_list_at_pos_005e9b60(
    void* list, void*, void* pos);
extern "C" void* __fastcall unresolved_list_next_005e9b90(
    void* list, void*, void** pos);

extern "C" void __fastcall kotor_fastcall_0068a010(void* self, void*) {
    unsigned char* const base = static_cast<unsigned char*>(self);
    void* const app = kotor_menu_window_cells::manager_007a39fc;

    // Original 0068A028..0068A036: mov eax,[007A39FC]; mov ecx,[eax+4];
    // call 005ED8B0. ECX is the loaded CClientExoApp pointer, not &app[+4].
    void* const party = kotor_fastcall_005ed8b0(load_ptr(app, 0x04), nullptr);
    void* const player = kotor_fastcall_00635460(party, nullptr);
    void* const creature =
        kotor_m15a_creature_get_server_creature_0060fb20(player, nullptr);
    void* const round = load_ptr(creature, 0x9C8);

    unsigned active = 0;
    if (load_u32(round, 0x934) != 0 &&
        kotor_fastcall_004d2b30(round, nullptr) != 0) {
        active = 1;
    }

    ScopedString default_text;
    ScopedString dash_text("-");
    Ref specific;
    Ref fallback;
    c2da_default_ref_00405ed0(&specific, nullptr);
    c2da_default_ref_00405ed0(&fallback, nullptr);

    void* const list = load_ptr(creature, 0xFC);
    void* pos_cell = load_ptr(list, 0);
    int icon_index = 0;
    unsigned shown = 0;

    // Original 0068A0C1: an empty list jumps straight to the clear loop.
    for (; pos_cell != nullptr;) {
        if (icon_index >= 4) {
            break;
        }

        // Original 0068A0DA reloads [creature+0xFC] for each lookup.
        void* const node = unresolved_list_at_pos_005e9b60(
            load_ptr(creature, 0xFC), nullptr, pos_cell);
        if (node == nullptr) {
            goto outer_next;
        }

        const int id = static_cast<int>(load_u32(node, 0));
        const bool eligible =
            (id == 0x3F && active != 0) || id == 0x0C || id == 0x0F ||
            id == 0x2E;

        if (eligible && shown == 0) {
            void* const sub = load_ptr(round, 0x9C8);
            if (sub != nullptr) {
                shown = 1;
                kotor_fastcall_00686fb0(
                    self, nullptr, round, node, sub, &pos_cell,
                    &specific, &fallback);
                kotor_fastcall_00685610(
                    self, nullptr, &icon_index, &specific, &fallback,
                    active, 0);
            } else if (id == 0x3F) {
                goto second_list;
            } else {
                kotor_fastcall_00686fb0(
                    self, nullptr, round, node, nullptr, &pos_cell,
                    &specific, &fallback);
                kotor_fastcall_00685610(
                    self, nullptr, &icon_index, &specific, &fallback,
                    active, 0);
                shown = 1;
            }
        }

        if (id != 0x3F) {
            goto outer_next;
        }

    second_list: {
        void* const second = load_ptr(round, 0x9B0);
        if (second != nullptr) {
            void* const inner_list = load_ptr(second, 0);
            void* inner_cell = load_ptr(inner_list, 0);
            if (inner_cell == nullptr) {
                if (shown == 0) {
                    kotor_fastcall_00686fb0(
                        self, nullptr, round, node,
                        load_ptr(round, 0x9C8), &pos_cell,
                        &specific, &fallback);
                    kotor_fastcall_00685610(
                        self, nullptr, &icon_index, &specific, &fallback,
                        active, 0);
                }
            } else if (icon_index < 4) {
                for (;;) {
                    if (inner_cell == nullptr) {
                        break;
                    }
                    // Original re-reads [[round+0x9B0]] before each call.
                    void* const inner_node =
                        unresolved_list_at_pos_005e9b60(
                            load_ptr(load_ptr(round, 0x9B0), 0), nullptr,
                            inner_cell);
                    kotor_fastcall_00686fb0(
                        self, nullptr, round, node, inner_node,
                        &pos_cell, &specific, &fallback);
                    kotor_fastcall_00685610(
                        self, nullptr, &icon_index, &specific, &fallback,
                        active, 0);
                    if (icon_index >= 4) {
                        break;
                    }
                    // 005E9B90 advances the position in place and returns
                    // the next element's data; the original discards the
                    // return and reloads the position local ([esp+0x18]).
                    unresolved_list_next_005e9b90(
                        load_ptr(load_ptr(round, 0x9B0), 0), nullptr,
                        &inner_cell);
                }
            }
        }
    }

    outer_next:
        // Original 0068A279..0068A293: reload [creature+0xFC], advance the
        // position in place, discard the returned data, test the position.
        unresolved_list_next_005e9b90(
            load_ptr(creature, 0xFC), nullptr, &pos_cell);
        if (pos_cell == nullptr) {
            break;
        }
    }

    if (icon_index < 4) {
        for (int i = icon_index; i < 4; ++i) {
            *reinterpret_cast<std::uint32_t*>(
                base + 0x625C + static_cast<std::uintptr_t>(i) * 0x140 +
                0x44) &= ~2u;
        }
    }
}
