// Original swkotor.exe 005F2D00 CClientExoAppInternal::IsSoundPlayingInDialog.
//
// Entry-to-return reconstruction. Original thiscall shape: ECX =
// CClientExoAppInternal*, no stack arguments, RET 0, int EAX. The selected
// composition symbol is @kotor_fastcall_005f2d00@8. This is the tail body of
// the 005EDBC0 wrapper.
//
// This TU defines only 005F2D00. All callees are existing selected providers;
// none is implemented, stubbed, trapped or replaced here.

#include <cstdint>

using U32 = std::uint32_t;

static_assert(sizeof(void*) == 4, "original x86 ABI");

namespace table29_required {
unsigned __fastcall gui_playing_005d5af0(void*, void*);  // original 005D5AF0
}

extern "C" {
unsigned char __fastcall kotor_fastcall_004d8230(void*, void*, U32, void**);  // original 004D8230
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

inline void* load_ptr_volatile(const void* base, std::uintptr_t offset) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    return *reinterpret_cast<void* const volatile*>(address);
}

inline void store_u32(void* base, std::uintptr_t offset, U32 value) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    *reinterpret_cast<U32*>(address) = value;
}

// Original client-object vtable +0x0C. Only the null/non-null result is used.
inline void* client_self(void* object) {
    void** const vtable = *reinterpret_cast<void***>(object);
    using Fn = void* (__fastcall*)(void*, void*);
    return reinterpret_cast<Fn>(vtable[0x0C / 4])(object, nullptr);
}

}  // namespace

extern "C" int __fastcall kotor_fastcall_005f2d00(void* internal, void* /*unused_edx*/) {
    // 005F2D04..12: gui and the first-call latch.
    void* gui = load_ptr_volatile(internal, 0x40);
    if (load_u32(gui, 0x16C) == 0) {
        // 005F2D5B..6D: write 1, return 1.
        store_u32(gui, 0x16C, 1);
        return 1;
    }

    // 005F2D14..27: dialog id, object array, zeroed out slot, lookup.
    const U32 id = load_u32(gui, 0x180);
    void* const object_array = load_ptr(internal, 0x14);
    void* object = nullptr;
    const unsigned char status =
        kotor_fastcall_004d8230(object_array, nullptr, id, &object);

    // 005F2D2C..3A: absent or null result.
    if (status != k_object_found_0074666c) return 0;
    if (!object) return 0;

    // 005F2D3C..43: client vtable+0x0C; null result fails.
    if (!client_self(object)) return 0;

    // 005F2D45..50: reload the gui via [internal+0x40], then the sound source.
    gui = load_ptr_volatile(internal, 0x40);
    void* const sound = load_ptr_volatile(gui, 0xB40);
    if (!sound) return 0;

    // 005F2D52..5A: return the real streaming predicate directly.
    return static_cast<int>(table29_required::gui_playing_005d5af0(sound, nullptr));
}
