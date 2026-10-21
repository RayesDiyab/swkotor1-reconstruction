// Four bounded original bodies. All lower providers and global storage remain canonical.
#include "d790598862-combat_relation.hpp"
#include <cstdint>
#include <cstdlib>

#if !defined(_M_IX86)
#error "combat_relation.cpp requires x86"
#endif

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
extern void* kotor_cell_rules_007a3a28;
int __cdecl crt_rand_006fc447();                                  // 004EC97F call 006FC447 (game CRT rand state)
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
void* __fastcall kotor_fastcall_004ae770(void*, void*, unsigned);
void* __fastcall kotor_fastcall_004ae750(void*, void*, unsigned);
void* __fastcall kotor_fastcall_004ae910(void*, void*, unsigned);
unsigned __fastcall kotor_fastcall_004aea40(void*, void*);
void* __fastcall kotor_fastcall_00561080(void*, void*);
void* __fastcall recovered_object_area_004cb120(void*, void*);
unsigned __fastcall recovered_object_get_reputation_0057cb80(void*, void*, unsigned, unsigned);
void __fastcall recovered_creature_set_combat_state_004f2610(void*, void*, unsigned, unsigned);
float __fastcall recovered_creature_spot_distance_004efc70(void*, void*);
void __fastcall frame_LogStringCtor_005e5a90(void*, void*, const char*);
void __fastcall frame_LogStringDtor_005e5c20(void*, void*);
int __fastcall c2da_current_get_float_00413430(void*, void*, int, void*, float*);
int __fastcall c2da_current_get_int_00413660(void*, void*, int, void*, int*);
// Missing nearest-enemy body is independently owned by sol_dialogue. Exact first stack argument is float.
unsigned __fastcall kotor_fastcall_004f2de0(void*, void*, float, unsigned, unsigned);
void __fastcall kotor_fastcall_004d68a0(void*, void*, unsigned, unsigned, unsigned);
void __fastcall kotor_fastcall_00578570(void*, void*, unsigned);
void __fastcall kotor_fastcall_004d62c0(void*, void*, unsigned, unsigned);
unsigned __fastcall kotor_fastcall_004d6440(void*, void*, void**, unsigned*);
unsigned __fastcall kotor_fastcall_00567e60(void*, void*, unsigned, unsigned, unsigned, void*, unsigned);
}

namespace {
using U = unsigned;
template<class T> T& at(void* object, U offset) {
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset);
}
void* server() { return at<void*>(kotor_menu_window_cells::manager_007a39fc, 8); }
template<class R, class... A> R vc(void* object, U offset, A... args) {
    using F = R(__fastcall*)(void*, void*, A...);
    return reinterpret_cast<F>(at<void*>(at<void*>(object, 0), offset))(object, nullptr, args...);
}
struct String {
    alignas(4) unsigned char bytes[8];
    explicit String(const char* text) { frame_LogStringCtor_005e5a90(bytes, nullptr, text); }
    ~String() { frame_LogStringDtor_005e5c20(bytes, nullptr); }
};
static_assert(sizeof(String) == 8, "original CExoString frame");

// 004FBCD6..004FBD14: each subtraction rounds to a dword, in Z/Y/X order.
U relationship_difference(void* self, void* other, float* delta) {
    U control;
    __asm {
        mov eax, self
        mov edx, other
        mov ecx, delta
        fld dword ptr [eax + 098h]
        fsub dword ptr [edx + 098h]
        mov eax, dword ptr [eax + 0a88h]
        mov control, eax
        fstp dword ptr [ecx + 8]
        mov eax, self
        fld dword ptr [eax + 094h]
        fsub dword ptr [edx + 094h]
        fstp dword ptr [ecx + 4]
        fld dword ptr [eax + 090h]
        fsub dword ptr [edx + 090h]
        fstp dword ptr [ecx]
    }
    return control;
}
// 004FBD8F..004FBDBA: radius^2 < distance^2 OR unordered skips the member.
bool relationship_outside(const float* delta, const float* radius) {
    unsigned char outside;
    __asm {
        mov ecx, delta
        fld dword ptr [ecx]
        fmul dword ptr [ecx]
        fld dword ptr [ecx + 4]
        fmul dword ptr [ecx + 4]
        faddp st(1), st(0)
        fld dword ptr [ecx + 8]
        fmul dword ptr [ecx + 8]
        faddp st(1), st(0)
        mov ecx, radius
        fld dword ptr [ecx]
        fmul dword ptr [ecx]
        fcompp
        fnstsw ax
        test ah, 1
        setnz outside
    }
    return outside != 0;
}
// 004EC938..004EC97D: keep three unrounded x87 differences on the stack.
bool bark_in_range(void* player_object, void* self) {
    const U squared_limit = 0x44610000u; // original 007455BC = 900.0f
    unsigned char inside;
    __asm {
        mov eax, player_object
        mov edx, self
        fld dword ptr [eax + 090h]
        fsub dword ptr [edx + 090h]
        fld dword ptr [eax + 094h]
        fsub dword ptr [edx + 094h]
        fld dword ptr [eax + 098h]
        fsub dword ptr [edx + 098h]
        fld st(0)
        fmul st(0), st(1)
        fld st(2)
        fmul st(0), st(3)
        faddp st(1), st(0)
        fld st(3)
        fmul st(0), st(4)
        faddp st(1), st(0)
        fcomp dword ptr squared_limit
        fstp st(0)
        fnstsw ax
        fstp st(0)
        test ah, 041h
        fstp st(0)
        setnp inside
    }
    return inside != 0;
}
}

// [004FBBE0,004FBE36), RET4/native @12. Original argument is the hostile object ID.
extern "C" void __fastcall kotor_fastcall_004fbbe0(void* self, void*, U target) {
    void* hostile = kotor_fastcall_004ae770(server(), nullptr, target);
    U first = 0x7f000000u, second = 0x7f000000u;
    if (hostile != nullptr) { first = at<U>(hostile, 0x50c); second = at<U>(hostile, 0x524); }
    if (static_cast<std::int32_t>(recovered_object_get_reputation_0057cb80(self, nullptr, target, 0)) <= 10 ||
        target == at<U>(self, 4)) {
        const U self_id = at<U>(self, 4);
        const U flag = first == self_id || second == self_id ? 1u : 2u;
        recovered_creature_set_combat_state_004f2610(self, nullptr, 1, flag);
    }
    if (at<U>(self, 0xf8) != 0) return;
    void* members;
    (void)combat_reuse_members_005be780(combat_reuse_faction_00513fc0(self, nullptr), nullptr, &members);
    U index = 0;
    if (static_cast<std::int32_t>(at<U>(combat_reuse_faction_00513fc0(self, nullptr), 4)) <= 0) return;
    do {
        void* object = kotor_fastcall_004ae750(server(), nullptr, at<U>(members, index * 4));
        if (object != nullptr && vc<void*>(object, 0x10) != nullptr) {
            void* other = vc<void*>(object, 0x10); // original deliberately repeats the virtual conversion
            float delta[3];
            const U control = relationship_difference(self, other, delta);
            float radius = 0.0f;
            if (control != 0) radius = 30.0f;
            else if (vc<void*>(other, 0x30) != nullptr)
                radius = recovered_creature_spot_distance_004efc70(vc<void*>(other, 0x30), nullptr);
            else {
                String name("PrimaryRange"); // original literal 00746F28, unwind state0
                void* table = at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0xc4);
                (void)c2da_current_get_float_00413430(table, nullptr, 11, name.bytes, &radius);
            }
            if (!relationship_outside(delta, &radius) &&
                static_cast<std::int32_t>(recovered_object_get_reputation_0057cb80(vc<void*>(other, 0x30), nullptr, target, 0)) <= 10) {
                const U other_id = at<U>(other, 4);
                const U flag = first == other_id || second == other_id ? 1u : 2u;
                recovered_creature_set_combat_state_004f2610(vc<void*>(other, 0x30), nullptr, 1, flag);
                kotor_fastcall_004f3580(vc<void*>(other, 0x30), nullptr, 1);
            }
        }
        ++index;
    } while (static_cast<std::int32_t>(index) <
             static_cast<std::int32_t>(at<U>(combat_reuse_faction_00513fc0(self, nullptr), 4)));
}

// [004EC8E0,004EC9DA), RET0/native @8. Real player/area required; ordered distance <= 900.
extern "C" void __fastcall kotor_fastcall_004ec8e0(void* self, void*) {
    if (recovered_object_area_004cb120(self, nullptr) == nullptr) return;
    const U player_id = kotor_fastcall_004aea40(server(), nullptr);
    void* player = kotor_fastcall_004ae910(server(), nullptr, player_id);
    if (player == nullptr) return;
    void* object = kotor_fastcall_00561080(player, nullptr);
    if (object == nullptr || at<U>(self, 0x8c) != at<U>(object, 0x8c) || !bark_in_range(object, self)) return;
    const int remainder = crt_rand_006fc447() % 5; // 004EC97F rand (game CRT), signed CDQ/IDIV
    const U bark = remainder == 0 ? 1u : remainder == 1 ? 2u : remainder == 2 ? 3u : remainder == 3 ? 4u : 5u;
    const U id = at<U>(self, 4);
    void* message = combat_reuse_message_004aedb0(); // existing W1 sws_message helper, no new body
    (void)kotor_fastcall_00569ec0(message, nullptr, player, id, bark);
}

// [004F3580,004F3683), RET4/native @12. DURATION row uses the low argument byte.
extern "C" void __fastcall kotor_fastcall_004f3580(void* self, void*, U state) {
    int duration;
    int found;
    {
        String name("DURATION"); // literal007469D0; original unwind state0
        void* table = at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0xa0);
        found = c2da_current_get_int_00413660(table, nullptr, static_cast<int>(state & 0xffu), name.bytes, &duration);
    }
    if (found == 0 || static_cast<U>(duration) <= at<U>(self, 0x384)) return; // JBE unsigned
    at<U>(self, 0x384) = static_cast<U>(duration);
    const U id = at<U>(self, 4);
    void* player = kotor_fastcall_004ae910(server(), nullptr, id);
    if (recovered_object_area_004cb120(self, nullptr) == nullptr || player == nullptr) return;
    if (at<void*>(recovered_object_area_004cb120(self, nullptr), 0x208) == nullptr) return;
    if (kotor_fastcall_004f2de0(self, nullptr, 30.0f, 0x7f000000u, 0x7f000000u) == 0x7f000000u) return;
    vc<void>(at<void*>(recovered_object_area_004cb120(self, nullptr), 0x208), 0x10, 1u);
}

// [00569EC0,00569F33), RET0C/native @20. W2 owns all writers/buffer/finalize/send.
extern "C" U __fastcall kotor_fastcall_00569ec0(void* message, void*, void* player, U object_id, U bark) {
    kotor_fastcall_004d68a0(message, nullptr, 5, 0xffffffffu, 1);
    kotor_fastcall_00578570(message, nullptr, object_id);
    kotor_fastcall_004d62c0(message, nullptr, bark, 8);
    void* bytes;
    U size;
    if (kotor_fastcall_004d6440(message, nullptr, &bytes, &size) == 0) return 0;
    return kotor_fastcall_00567e60(message, nullptr, at<U>(player, 4), 0x1a, 1, bytes, size);
}
