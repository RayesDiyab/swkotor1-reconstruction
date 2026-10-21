// Original 0052CBE0..0052CC64: ACTION730 ActionFollowLeader (thiscall RET8).
// Read-only original evidence; private reconstruction source only.
#include <cstdint>
#include <cstring>

extern "C" {
void* __fastcall kotor_fastcall_004ae770(void*, void*, unsigned);
// Original 0083281C: writable .data virtual zero-fill; one canonical mutable
// owner shared by all reconstructed readers. Initial zero is faithful, not a
// permanently-false invariant. Never substitute a raw rebuilt address/constant.
unsigned recovered_debug_0083281c = 0;
// Selected 0078-window-native-strings.obj @WindowCStr005e5670@8: CExoString
// CStr getter, receiver ECX, unused EDX, RET0, pointer return. Reuse existing.
const char* __fastcall WindowCStr005e5670(void*,void*);
unsigned __fastcall recovered_object_add_action_004cea20(
    void* self, void*, unsigned type, unsigned group,
    unsigned t0, void* v0, unsigned t1, void* v1, unsigned t2, void* v2,
    unsigned t3, void* v3, unsigned t4, void* v4, unsigned t5, void* v5,
    unsigned t6, void* v6, unsigned t7, void* v7, unsigned t8, void* v8,
    unsigned t9, void* v9, unsigned t10, void* v10, unsigned t11, void* v11,
    unsigned t12, void* v12);
}

namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
}

namespace {
template <typename T>
T read_at(const void* base, unsigned offset) {
    T value;
    std::memcpy(&value, static_cast<const unsigned char*>(base) + offset, sizeof(value));
    return value;
}
}

extern "C" int __fastcall recovered_vm_action_follow_leader_0052cbe0(
    void* self, void*, int /*argc*/, unsigned /*arg*/) {
    // 0052CBE2..0052CBF5: manager->server + GetGameObject(self+8).
    // Original unconditionally dereferences [007A39FC] and [manager+8]; do not
    // add null guards that turn an original fault into a fabricated success.
    void* manager = kotor_menu_window_cells::manager_007a39fc;
    void* server = read_at<void*>(manager, 8);
    const unsigned object_id = read_at<unsigned>(self, 8);
    void* object = kotor_fastcall_004ae770(server, nullptr, object_id);

    // 0052CBF7..0052CC0C: null, +A88 zero, or +E8 != 1 => return 0.
    if (object == nullptr) return 0;
    if (read_at<unsigned>(object, 0xA88) == 0) return 0;
    if (read_at<unsigned>(object, 0xE8) != 1) return 0;

    // 0052CC0E..0052CC1A: read the canonical mutable [0083281C] owner and,
    // when nonzero, call the existing CExoString CStr getter with ecx=object+18.
    // The getter returns a char pointer and has no logging/state side effect; its
    // result is discarded. This preserves the original conditional call/faults.
    if (recovered_debug_0083281c != 0) {
        (void)WindowCStr005e5670(static_cast<unsigned char*>(object) + 0x18, nullptr);
    }

    // 0052CC1F..0052CC5C: original pushes 26 zero params, then group 0xFFFF,
    // then type 0x3D (28 stack arguments = type+group+26 params). Pass all 13
    // parameter pairs; the old 12 pairs omitted two params. No blind padding.
    (void)recovered_object_add_action_004cea20(
        object, nullptr, 0x3D, 0xFFFF,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
        0, nullptr, 0, nullptr, 0, nullptr);
    return 0;
}
