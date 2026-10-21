// A7 source — original swkotor.exe 004CD580 CSWSObject::UpdateDialog.
// Independent full-plan review: ACCEPT-WITH-FIXES; fix closure: ACCEPT.
//
// Original: swkotor.exe 004CD580 CSWSObject::UpdateDialog (0xD6 bytes,
// original thiscall-shaped, ECX=self, no stack args, RET 0, returns int EAX).
// Selected composition: candidate 1566, symbol @kotor_fastcall_004cd580@8.
//
// This TU is intentionally dependency-complete and link-incomplete: the seven
// callees marked MISSING are unassigned original functions. They are declared
// with their exact expected contracts and are NOT implemented, stubbed, trapped
// or replaced here. The final integrator must not link this object until those
// providers have independent source/native acceptance.

#include <cstdint>

using U32 = std::uint32_t;
using I32 = std::int32_t;

constexpr U32 k_invalid_object_id = 0x7F000000u;

// Providers and the data cell are declared at global/named-namespace scope with
// external linkage. Do not move these declarations into the anonymous namespace
// below; that would give them internal linkage and break exact symbol binding.

// Existing selected real provider (0457.obj, original 005ED690). Its canonical
// declaration returns a 32-bit pointer value as `unsigned`; keep that exact type.
extern "C" unsigned __fastcall frame_R4_005ed690(void*, void*);

// Missing original providers. Expected selected-composition symbols; each is a
// separate ownership gate. The original 004CD460 pops one stack argument
// (RET 4); the selected 3-argument fastcall is also callee-clean RET 4, so the
// caller pushes entry and the callee pops it (no add esp,4 at the call site).
extern "C" void __fastcall kotor_fastcall_0062fa30(void*, void*);            // 0062FA30 ECX=gui
extern "C" void __fastcall kotor_fastcall_005a0940(void*, void*);            // 005A0940 ECX=dialog (ignored)
extern "C" int __fastcall kotor_fastcall_004cd2d0(void*, void*);             // 004CD2D0 ECX=self
extern "C" int __fastcall kotor_fastcall_004cd460(void*, void*, U32);        // 004CD460 ECX=self, entry on stack
extern "C" int __fastcall kotor_fastcall_004cd500(void*, void*);             // 004CD500 ECX=self
extern "C" int __fastcall kotor_fastcall_004cb4f0(void*, void*);             // 004CB4F0 ECX=self
extern "C" int __fastcall kotor_fastcall_005edbc0(void*, void*);             // 005EDBC0 ECX=client-internal

// Existing selected data cell for [007A39FC]. The declaration matches the
// canonical window-shared-cells TU used by the selected composition.
namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
}

namespace {

// Bounded raw accessors. The original accesses are 4-byte dwords; these helpers
// preserve the exact width and fault at the original address when a pointer is
// null. They do not add checks or fallback values.
inline U32 load_u32(const void* base, std::uintptr_t offset) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    return *reinterpret_cast<const U32*>(address);
}

inline I32 load_i32(const void* base, std::uintptr_t offset) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    return *reinterpret_cast<const I32*>(address);
}

inline void* load_ptr(const void* base, std::uintptr_t offset) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    return *reinterpret_cast<void* const*>(address);
}

inline void store_u32(void* base, std::uintptr_t offset, U32 value) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    *reinterpret_cast<U32*>(address) = value;
}

// [ [007A39FC] + 4 ]: the client-internal pointer used by 005ED690/005EDBC0.
inline void* client_internal() {
    return load_ptr(kotor_menu_window_cells::manager_007a39fc, 4);
}

}  // namespace

extern "C" int __fastcall kotor_fastcall_004cd580(void* self, void* /*unused_edx*/) {
    // 004CD583..004CD595: dialog-owner gate.
    const U32 owner = load_u32(self, 0x54);
    if (owner == k_invalid_object_id) return 0;
    if (owner != load_u32(self, 0x04)) return 0;

    // 004CD596..004CD5B7: lipsync, hostile-action clear, delay gate.
    const unsigned gui_for_lipsync = frame_R4_005ed690(client_internal(), nullptr);
    kotor_fastcall_0062fa30(reinterpret_cast<void*>(gui_for_lipsync), nullptr);
    kotor_fastcall_005a0940(load_ptr(self, 0x34), nullptr);
    if (load_u32(self, 0x50) != 0) return 1;

    // 004CD5BD..004CD5CD: dialog-delay predicate and active-flag gate.
    if (kotor_fastcall_004cd2d0(self, nullptr) == 0 && load_u32(self, 0x48) != 0) {
        // 004CD5CF..004CD601: second GetInGameGui; optional sound wait.
        const unsigned gui_bits = frame_R4_005ed690(client_internal(), nullptr);
        const void* const gui = reinterpret_cast<const void*>(gui_bits);
        if (load_u32(gui, 0xC0) == 0) {
            if (load_u32(self, 0x220) != 0) {
                if (kotor_fastcall_005edbc0(client_internal(), nullptr) != 0) {
                    return 1;
                }
            }
        }

        // 004CD603..004CD627: clear state, then entry or replies dispatch.
        const I32 entry = load_i32(self, 0x4C);
        store_u32(self, 0x48, 0);
        store_u32(self, 0x220, 0);
        if (entry != -1) {
            return kotor_fastcall_004cd460(self, nullptr, static_cast<U32>(entry));
        }
        return kotor_fastcall_004cd500(self, nullptr);
    }

    // 004CD62C..004CD655: end path. StopDialog is conditional on the dialog
    // field and the sound predicate; the body always returns 1.
    void* const dialog = load_ptr(self, 0x34);
    if (load_u32(dialog, 0x60) != 0) {
        if (kotor_fastcall_005edbc0(client_internal(), nullptr) == 0) {
            (void)kotor_fastcall_004cb4f0(self, nullptr);
        }
    }
    return 1;
}
