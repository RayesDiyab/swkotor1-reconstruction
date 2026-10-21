// 0062B1E0 CGuiInGame::UpdateCreatedInGameGUI — full phase-2 body (re-authored).
//
// Source-only; no compiler/linker/native action. Faithful structure:
//   gate const -> old==new -> detach old panel/flags -> panel[new] check ->
//   faithful new>7 OOB store of old_index -> eight real new expressions.
// Under /EHs each new expression must generate operator new (selected ??2), the
// class constructor (which calls the selected free @kotor_fastcall_006b...@12
// ctor with ECX=this and manager on the stack), and compiler delete-on-throw
// (selected unsized ??3) when that constructor throws.
//
// The base /EHsc object emitted no such cleanup: /EHsc assumes a C-linkage callee
// such as the selected ctor cannot throw, so the new-expression cleanup was
// dropped. This successor compiles with /EHs, which preserves C++ cleanup for
// potentially-throwing C-linkage callees. Admission is semantic, not original EH
// state numbering: a throw from any of the eight ctors must free the raw block
// exactly once through the selected unsized ??3 and continue propagating the same
// exception. There are no try/catch/rethrow statements. The compiler chooses the
// state count/table; reproducing the original 8 states is not required.
// No adapters, caller add-esp patches, traps or extra cells.
#include "e2d7cacab1-load_screen_0062b1e0.h"

#include <cstdint>

// Accepted extern const, defined once by the accepted 1205-30 object. Exact accepted
// declaration (ui_save_state_types.h:97): const volatile std::uint32_t in namespace w4.
// This unit must not define or initialize it.
namespace w4 {
extern const volatile std::uint32_t ingame_gui_single_panel_007a2370;
}

// Selected game-heap allocation functions (G9: ??2 -> _live_operator_new,
// ??3 -> _live_free). These are declarations only. operator new has no exception
// specification (matching the compiler's built-in declaration; the previous
// noexcept declaration was a C5043 compile error). operator delete is the standard
// noexcept unsized deallocator; it must be the unsized form so the emitted EH
// cleanup imports exactly ??3@YAXPAX@Z and never the sized ??3@YAXPAXI@Z. The
// selected game-heap ??2 returns null on failure; each real new expression below
// is followed by the original null check, which stores null and skips the ctor.
// /Zc:throwingNew- is the compiler default and must stay in effect.
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* block) noexcept;

// Selected panel ctors; each main exit is RET 4. Declarations only.
extern "C" void* __fastcall kotor_fastcall_006ba980(void*, void*, void*);
extern "C" void* __fastcall kotor_fastcall_006b34c0(void*, void*, void*);
extern "C" void* __fastcall kotor_fastcall_006b0e40(void*, void*, void*);
extern "C" void* __fastcall kotor_fastcall_006adda0(void*, void*, void*);
extern "C" void* __fastcall kotor_fastcall_00626400(void*, void*, void*);
extern "C" void* __fastcall kotor_fastcall_00644a40(void*, void*, void*);
extern "C" void* __fastcall kotor_fastcall_00694d50(void*, void*, void*);
extern "C" void* __fastcall kotor_fastcall_006ab2f0(void*, void*, void*);

namespace {

// Eight exact-size panel types. Each constructor is marked __forceinline for
// source intent, but /Ob0 still emits one local PanelN::PanelN helper per type.
// Those helpers are allowed when their ABI/cleanup is equivalent; inlining is not
// required. Under /EHs each helper is potentially throwing because it calls a
// potentially throwing C-linkage ctor; the new-expression cleanup is owned by the
// enclosing function, not by the helper.
struct Panel0 {
    unsigned char storage[0x42BC];
    __forceinline Panel0(void* manager) {
        (void)kotor_fastcall_006ba980(this, nullptr, manager);
    }
};
static_assert(sizeof(Panel0) == 0x42BC, "Panel0 size");

struct Panel1 {
    unsigned char storage[0x1DE8];
    __forceinline Panel1(void* manager) {
        (void)kotor_fastcall_006b34c0(this, nullptr, manager);
    }
};
static_assert(sizeof(Panel1) == 0x1DE8, "Panel1 size");

struct Panel2 {
    unsigned char storage[0x59F8];
    __forceinline Panel2(void* manager) {
        (void)kotor_fastcall_006b0e40(this, nullptr, manager);
    }
};
static_assert(sizeof(Panel2) == 0x59F8, "Panel2 size");

struct Panel3 {
    unsigned char storage[0x3F98];
    __forceinline Panel3(void* manager) {
        (void)kotor_fastcall_006adda0(this, nullptr, manager);
    }
};
static_assert(sizeof(Panel3) == 0x3F98, "Panel3 size");

struct Panel4 {
    unsigned char storage[0x0AF4];
    __forceinline Panel4(void* manager) {
        (void)kotor_fastcall_00626400(this, nullptr, manager);
    }
};
static_assert(sizeof(Panel4) == 0x0AF4, "Panel4 size");

struct Panel5 {
    unsigned char storage[0x0FC8];
    __forceinline Panel5(void* manager) {
        (void)kotor_fastcall_00644a40(this, nullptr, manager);
    }
};
static_assert(sizeof(Panel5) == 0x0FC8, "Panel5 size");

struct Panel6 {
    unsigned char storage[0x10DC];
    __forceinline Panel6(void* manager) {
        (void)kotor_fastcall_00694d50(this, nullptr, manager);
    }
};
static_assert(sizeof(Panel6) == 0x10DC, "Panel6 size");

struct Panel7 {
    unsigned char storage[0x1DAC];
    __forceinline Panel7(void* manager) {
        (void)kotor_fastcall_006ab2f0(this, nullptr, manager);
    }
};
static_assert(sizeof(Panel7) == 0x1DAC, "Panel7 size");

}  // namespace

extern "C" void __fastcall load_screen_update_created_in_game_gui_0062b1e0(
    void* self, void*, unsigned old_index, unsigned new_index) {
    if (w4::ingame_gui_single_panel_007a2370 != 0u) return;
    if (old_index == new_index) return;

    void** const panels =
        reinterpret_cast<void**>(static_cast<char*>(self) + 0xC);

    if (old_index != 0xFFFFFFFFu) {
        void* const old_panel = panels[old_index];
        if (old_panel != nullptr) {
            unsigned int* const flags = reinterpret_cast<unsigned int*>(
                static_cast<char*>(old_panel) + 0x44);
            *flags = (*flags & 0xFFFFFCFFu) | 0x400u;
            panels[old_index] = nullptr;
        }
    }

    if (panels[new_index] != nullptr) return;

    if (new_index > 7u) {
        // Faithful original quirk: OOB store of the old index value, not the old
        // panel pointer, with no bounds guard.
        panels[new_index] =
            reinterpret_cast<void*>(static_cast<unsigned long>(old_index));
        return;
    }

    switch (new_index) {
    case 0u: {
        // Allocation is sequenced before the manager initializer in C++17; the
        // manager load therefore occurs after operator new and before the ctor.
        Panel0* const allocated = new Panel0(
            *reinterpret_cast<void**>(static_cast<char*>(self) + 0x38));
        if (allocated == nullptr) {
            panels[new_index] = nullptr;
            return;
        }
        panels[new_index] = allocated;
        return;
    }
    case 1u: {
        Panel1* const allocated = new Panel1(
            *reinterpret_cast<void**>(static_cast<char*>(self) + 0x38));
        if (allocated == nullptr) {
            panels[new_index] = nullptr;
            return;
        }
        panels[new_index] = allocated;
        return;
    }
    case 2u: {
        Panel2* const allocated = new Panel2(
            *reinterpret_cast<void**>(static_cast<char*>(self) + 0x38));
        if (allocated == nullptr) {
            panels[new_index] = nullptr;
            return;
        }
        panels[new_index] = allocated;
        return;
    }
    case 3u: {
        Panel3* const allocated = new Panel3(
            *reinterpret_cast<void**>(static_cast<char*>(self) + 0x38));
        if (allocated == nullptr) {
            panels[new_index] = nullptr;
            return;
        }
        panels[new_index] = allocated;
        return;
    }
    case 4u: {
        Panel4* const allocated = new Panel4(
            *reinterpret_cast<void**>(static_cast<char*>(self) + 0x38));
        if (allocated == nullptr) {
            panels[new_index] = nullptr;
            return;
        }
        panels[new_index] = allocated;
        return;
    }
    case 5u: {
        Panel5* const allocated = new Panel5(
            *reinterpret_cast<void**>(static_cast<char*>(self) + 0x38));
        if (allocated == nullptr) {
            panels[new_index] = nullptr;
            return;
        }
        panels[new_index] = allocated;
        return;
    }
    case 6u: {
        Panel6* const allocated = new Panel6(
            *reinterpret_cast<void**>(static_cast<char*>(self) + 0x38));
        if (allocated == nullptr) {
            panels[new_index] = nullptr;
            return;
        }
        panels[new_index] = allocated;
        return;
    }
    case 7u: {
        Panel7* const allocated = new Panel7(
            *reinterpret_cast<void**>(static_cast<char*>(self) + 0x38));
        if (allocated == nullptr) {
            panels[new_index] = nullptr;
            return;
        }
        panels[new_index] = allocated;
        return;
    }
    }
}
