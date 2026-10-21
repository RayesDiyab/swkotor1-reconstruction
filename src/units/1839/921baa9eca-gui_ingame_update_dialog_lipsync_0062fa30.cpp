// Original swkotor.exe 0062FA30 CGuiInGame::UpdateDialogLipSync.
//
// Entry-to-return reconstruction. Original thiscall shape: ECX = CGuiInGame*,
// no stack arguments, RET 0, no meaningful EAX. The selected composition symbol
// is @kotor_fastcall_0062fa30@8.
//
// This TU defines only 0062FA30. All callees are existing selected providers;
// none is implemented, stubbed, trapped or replaced here.

#include <cstdint>

using U32 = std::uint32_t;

static_assert(sizeof(void*) == 4, "original x86 ABI");

// Selected data cells.
namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
}
namespace frame_log {
extern const char clear_0073d71c[];  // original 0073D71C empty string data cell
}

// Existing selected providers.
namespace table29_required {
unsigned __fastcall gui_playing_005d5af0(void*, void*);  // original 005D5AF0
}
extern "C" {
void* __fastcall kotor_fastcall_005ed580(void*, void*, U32);
void* __fastcall unresolved_ref_assign_00406290(void*, void*, void*, const char*);
int __fastcall input_desc_return_true_0063e7a0(void*, void*);  // original 0063E7A0
}

namespace {

constexpr U32 k_invalid_object_id = 0x7F000000u;

// The original has a 16-byte discarded CResRef out slot. This is a plain
// trivially-copyable aggregate, not a CResRef object and not a char[16]; the
// original body has no /GS cookie or EH funclet.
struct ResRefOut {
    U32 w0;
    U32 w1;
    U32 w2;
    U32 w3;
};
static_assert(sizeof(ResRefOut) == 16, "original 16-byte CResRef out slot");

inline U32 load_u32_volatile(const void* base, std::uintptr_t offset) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    return *reinterpret_cast<const volatile U32*>(address);
}

inline void* load_ptr_volatile(const void* base, std::uintptr_t offset) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    return *reinterpret_cast<void* const volatile*>(address);
}

inline void* load_ptr(const void* base, std::uintptr_t offset) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    return *reinterpret_cast<void* const*>(address);
}

inline void store_u32(void* base, std::uintptr_t offset, U32 value) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    *reinterpret_cast<U32*>(address) = value;
}

// Original client-object vtable +0x0C (CSWCCreature self-return in the normal
// case; the selected runtime object supplies the actual slot).
inline void* client_self(void* object) {
    void** const vtable = *reinterpret_cast<void***>(object);
    using Fn = void* (__fastcall*)(void*, void*);
    return reinterpret_cast<Fn>(vtable[0x0C / 4])(object, nullptr);
}

// Original client CSWCCreature vtable +0x128 LipSync(CResRef*) RET 4.
inline unsigned lipsync(void* creature, void* resref) {
    void** const vtable = *reinterpret_cast<void***>(creature);
    using Fn = unsigned (__fastcall*)(void*, void*, void*);
    return reinterpret_cast<Fn>(vtable[0x128 / 4])(creature, nullptr, resref);
}

}  // namespace

extern "C" void __fastcall kotor_fastcall_0062fa30(void* gui, void* /*unused_edx*/) {
    // 0062FA36..40: invalid current lipsync dialog id.
    if (load_u32_volatile(gui, 0xBDC) == k_invalid_object_id) return;

    // 0062FA42..4C: dialog sound pointer, then the real streaming predicate.
    void* sound = load_ptr_volatile(gui, 0xB40);
    if (!sound) return;
    if (!table29_required::gui_playing_005d5af0(sound, nullptr)) return;

    // 0062FA55..62: reload the sound pointer; the return-one predicate is
    // preserved even though the selected leaf always returns 1.
    sound = load_ptr_volatile(gui, 0xB40);
    if (!input_desc_return_true_0063e7a0(sound, nullptr)) return;

    // 0062FA64..79: manager, id, client, GetGameObject.
    void* const manager = kotor_menu_window_cells::manager_007a39fc;
    const U32 dialog_id = load_u32_volatile(gui, 0xBDC);
    void* const client = load_ptr(manager, 4);
    void* const object = kotor_fastcall_005ed580(client, nullptr, dialog_id);
    if (!object) return;

    // 0062FA7F..8E: two client vtable+0x0C calls on the same client object.
    if (!client_self(object)) return;
    void* const creature = client_self(object);

    // 0062FA91..9C: CSWCCreature::LipSync(&gui->CResRef).
    void* const gui_resref = reinterpret_cast<void*>(
        reinterpret_cast<std::uintptr_t>(gui) + 0xBE0);
    (void)lipsync(creature, gui_resref);

    // 0062FAA2..AE: reassign gui->CResRef from the canonical empty literal.
    ResRefOut discarded;
    (void)unresolved_ref_assign_00406290(gui_resref, nullptr, &discarded,
                                         frame_log::clear_0073d71c);

    // 0062FAB3: invalidate the lipsync dialog id after the CResRef clear.
    store_u32(gui, 0xBDC, k_invalid_object_id);
}
