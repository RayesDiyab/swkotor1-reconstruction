// camera_nested_providers.cpp
// Eight of the ten owned original-address bodies:
//   005F4460, 00674090, 005ED670, 005EEA90, 006774E0, 006C24A0,
//   00687FD0, 005F4D60.
// The 006880C0/004D37B0 pair is in action_clear_providers.cpp; the exact
// consumer-name joins are in consumer_joins.cpp.
//
// Static reconstruction only; no native product was built in this lane.
// Selected callees are bound by their exact v5-MAP C symbols; no trap,
// stub, placeholder, numeric-address alias or host allocator is used.
#include "7d9ceabe3d-nested_providers.h"

#include <cstdint>
#include <cstring>

static_assert(sizeof(void*) == 4, "nested-providers recipe is x86 only");

namespace {

template <class T>
T ld(const void* p, U32 off) {
    T v;
    std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof(T));
    return v;
}

template <class T>
void st(void* p, U32 off, T v) {
    std::memcpy(static_cast<unsigned char*>(p) + off, &v, sizeof(T));
}

// Deliberate unguarded original load.  The original bodies below dereference
// these pointers without a null check (for example 004D37B0's [list] node and
// 005F4460's unguarded 0063C070 result).  A volatile access keeps the fault at
// the original load instead of letting the optimizer delete a null access.
template <class T>
T ldu(const void* p, U32 off) {
    return *reinterpret_cast<const volatile T*>(
        reinterpret_cast<const volatile unsigned char*>(p) + off);
}

void* app_manager() {
    return kotor_menu_window_cells::manager_007a39fc;
}

void* client_app() {
    return ld<void*>(app_manager(), 4);
}

template <class Fn>
Fn vtslot(void* obj, U32 off) {
    // The original does not null-check the object before loading its vtable.
    void* vtbl = ldu<void*>(obj, 0);
    void* entry = ldu<void*>(vtbl, off);
    Fn fn;
    std::memcpy(&fn, &entry, sizeof(fn));
    return fn;
}

// 005F4460 shared lazy player-object cache (five identical lookups).
// The original re-reads [self+0x20] after the 005ED5B0 call and publishes that
// fresh id as the cache tag; the call argument is the id read before the call.
void* cached_object(void* self) {
    const U32 id = ld<U32>(self, 0x20);
    if (ld<U32>(self, 0x3BC) == id) {
        return ld<void*>(self, 0x3C0);
    }
    void* obj = frame_R4_005ed5b0(client_app(), nullptr, id);
    // Force the post-call re-read rather than allowing it to be CSE'd with id.
    const U32 fresh_id = ldu<U32>(self, 0x20);
    st<void*>(self, 0x3C0, obj);
    st<U32>(self, 0x3BC, fresh_id);
    return obj;
}

using Fn0  = void  (__fastcall*)(void*, void*);
using Fn1  = void  (__fastcall*)(void*, void*, U32);
using FnV1 = void  (__fastcall*)(void*, void*, void*);
using FnI0 = int   (__fastcall*)(void*, void*);
using FnP0 = void* (__fastcall*)(void*, void*);
using FnP1 = void* (__fastcall*)(void*, void*, U32);
using FnP2 = void* (__fastcall*)(void*, void*, U32, U32);
using FnF0 = float (__fastcall*)(void*, void*);

// Exact x87 sequence and predicate for 005F45E0/005F45F8:
//   ecx = object; (call vtable getter returning float in ST0)
//   fcomp dword ptr [0.0f]; fnstsw ax; test ah,44h; jp non_equal; jnp equal
// The original's jp is taken for greater, less or unordered (NaN) and not
// taken only for exact equality.  The helper touches only EAX/ECX/EDX/flags;
// the callee preserves EBX/ESI/EDI/EBP, and fcomp pops ST0.
__declspec(noinline) bool __fastcall ne_or_unordered(void* self, void*, FnF0 fn) {
    volatile unsigned char result = 0;
    volatile float zero = 0.0f;
    FnF0 volatile f = fn;
    __asm {
        mov ecx, self
        mov eax, dword ptr [f]
        xor edx, edx
        call eax
        fcomp dword ptr [zero]
        fnstsw ax
        test ah, 44h
        jnp is_equal
        mov byte ptr [result], 1
        jmp done
    is_equal:
        mov byte ptr [result], 0
    done:
    }
    return result != 0;
}

}  // namespace

// 006774E0: build and send the two-string module message.
// Original arg order (first stack argument last pushed):
//   message_begin(M, 0x80, -1, 1)
//   message_word(M, arg1, 0x20)
//   write_string(M, arg2, 0x20)
//   write_string(M, arg3, 0x20)
//   message_finish(M, data, size)
//   client_send(M, 3, 4, data, size)
extern "C" U32 __fastcall kotor_nested_006774e0(void* self, void*, U32 arg1, void* arg2, void* arg3) {
    void* data = nullptr;
    U32 size = 0;

    kotor_fastcall_004d68a0(self, nullptr, 0x80u, 0xFFFFFFFFu, 1u);
    kotor_fastcall_004d6340(self, nullptr, arg1, 0x20u);
    kotor_fastcall_004d6960(self, nullptr, static_cast<U32>(reinterpret_cast<std::uintptr_t>(arg2)), 0x20u);
    kotor_fastcall_004d6960(self, nullptr, static_cast<U32>(reinterpret_cast<std::uintptr_t>(arg3)), 0x20u);

    if (kotor_fastcall_004d6440(self, nullptr, &data, &size) == 0) {
        return 0;
    }
    return recovered_client_send_00677410(self, nullptr, 3u, 4u, data, size);
}

// 005EEA90: guard [C+0x14C] then run the message builder.
extern "C" U32 __fastcall kotor_nested_005eea90(void* self, void*, U32 arg1, void* arg2, void* arg3) {
    void* message = ld<void*>(self, 0x14C);
    if (!message) {
        return 0;
    }
    return kotor_nested_006774e0(message, nullptr, arg1, arg2, arg3);
}

// 005ED670: do_quick_save helper thunk; adjust client -> internal and tail the guard.
extern "C" U32 __fastcall kotor_nested_005ed670(void* self, void*, U32 arg1, void* arg2, void* arg3) {
    void* internal = ld<void*>(self, 4);
    return kotor_nested_005eea90(internal, nullptr, arg1, arg2, arg3);
}

// 005F4460: player flourish weapons body.
extern "C" void __fastcall kotor_nested_005f4460(void* self, void*) {
    void* obj = cached_object(self);
    kotor_fastcall_00611700(obj, nullptr, 0u, 1u);

    obj = cached_object(self);
    void* server = kotor_m15a_creature_get_server_creature_0060fb20(obj, nullptr);
    if (server) {
        const int dead = vtslot<FnI0>(server, 0x94)(server, nullptr);
        if (dead == 0 && recovered_creature_incapacitated_004ef890(server, nullptr) == 0) {
            void* client_creature = recovered_creature_client_creature_004efb20(server, nullptr);
            void* prop = gob_get_property_0x68_0063c070(client_creature, nullptr);
            // No null guard: the original faults at 005F4512 if prop is null.
            const unsigned char state = ldu<unsigned char>(prop, 0xC5);
            if (state < 2 || state > 4) {
                return;  // original early RET at 005F46E7
            }

            // Weapon slot 0x10.
            void* player = kotor_fastcall_005f1420(self, nullptr);
            U32 item_id = kotor_m15a_creature_get_equipped_item_id_0060ca80(player, nullptr, 0x10u);
            if (item_id != 0x7F000000u) {
                void* item = kotor_fastcall_005ed580(client_app(), nullptr, item_id);
                if (item) {
                    void* item_first = vtslot<FnP0>(item, 0x34)(item, nullptr);
                    if (item_first) {
                        void* item_second = vtslot<FnP0>(item, 0x34)(item, nullptr);
                        kotor_m15a_item_power_item_00647130(item_second, nullptr, 1u, 0u, 1u, 0u);
                    }
                }
            }

            // Weapon slot 0x20.
            player = kotor_fastcall_005f1420(self, nullptr);
            item_id = kotor_m15a_creature_get_equipped_item_id_0060ca80(player, nullptr, 0x20u);
            if (item_id != 0x7F000000u) {
                void* item = kotor_fastcall_005ed580(client_app(), nullptr, item_id);
                if (item) {
                    void* item_first = vtslot<FnP0>(item, 0x34)(item, nullptr);
                    if (item_first) {
                        void* item_second = vtslot<FnP0>(item, 0x34)(item, nullptr);
                        kotor_m15a_item_power_item_00647130(item_second, nullptr, 1u, 0u, 1u, 0u);
                    }
                }
            }

            recovered_creature_set_combat_state_004f2610(server, nullptr, 1u, 2u);
        }
    }

    // 005F45CD: float gate. take = ne(v1) || ne(v2); final only if both == 0.0f.
    bool take = false;
    void* ctrl = ld<void*>(self, 0x2A0);
    if (ctrl) {
        take = ne_or_unordered(ctrl, nullptr, vtslot<FnF0>(ctrl, 0x0C));
        if (!take) {
            // Original re-reads [self+0x2A0] at 005F45ED for the second call;
            // there is no second null guard (a null re-read faults).  The
            // volatile load prevents CSE with the first load.
            ctrl = ldu<void*>(self, 0x2A0);
            take = ne_or_unordered(ctrl, nullptr, vtslot<FnF0>(ctrl, 0x04));
        }
    }

    if (take) {
        // 005F4609 flourish path.
        void* a = gob_get_property_0x68_0063c070(cached_object(self), nullptr);
        void* b = gob_get_property_0x68_0063c070(cached_object(self), nullptr);
        void* r = vtslot<FnP2>(a, 0xE0)(a, nullptr, 0x27ADu, 1u);
        vtslot<FnV1>(b, 0x44)(b, nullptr, r);
        return;  // 005F469B
    }

    // 005F469C final animation path.
    void* final_player = cached_object(self);
    kotor_fastcall_0063c760(final_player, nullptr, 0x27AEu, 0x5DCu, 1.0f, 0u, 0u);
}

// 00674090: fire per-element cleaner.
extern "C" void __fastcall kotor_nested_00674090(void* self, void*) {
    const int count = ld<int>(self, 0x38);
    if (count <= 0) {
        return;
    }
    for (int i = 0; i < count; ++i) {
        // Original re-reads [this+0x34] each iteration; volatile forces it.
        void** array = ldu<void**>(self, 0x34);
        void* obj = array[i];
        void* r = vtslot<FnP1>(obj, 0x108)(obj, nullptr, 0xAAADu);
        if (r) {
            vtslot<Fn0>(r, 0x24)(r, nullptr);
        }
    }
}

// 006C24A0: solo-mode query helper.
// Four independent original exits, each writing Q+0xFC4 after its vtable call:
//   006C24BE (arg != 0, id 0x9402)
//   006C2504 (party pc != 0 and bit 0 set, id 0x9404)
//   006C2519 (party pc != 0 and bit 0 clear, id 0x9403)
//   006C2530 (party pc == 0, id 0x9401)
extern "C" void __fastcall kotor_nested_006c24a0(void* self, void*, U32 arg) {
    if (arg != 0) {
        vtslot<Fn1>(self, 0x6C)(self, nullptr, 0x9402u);
        st<U32>(self, 0xFC4, arg);
        return;
    }

    void* party = lane_config_required::GetPartyTable_004aee70(ld<void*>(app_manager(), 8), nullptr);
    // Unguarded original loads: a null party or player faults at [party+0x190]
    // or [player+0x2D4]; no guard is added.
    void* pc = ldu<void*>(party, 0x190);
    if (pc) {
        void* player = client_get_player_creature_005ed540(ld<void*>(app_manager(), 4), nullptr);
        if (ldu<unsigned char>(player, 0x2D4) & 1u) {
            vtslot<Fn1>(self, 0x6C)(self, nullptr, 0x9404u);
            st<U32>(self, 0xFC4, arg);
            return;
        }
        vtslot<Fn1>(self, 0x6C)(self, nullptr, 0x9403u);
        st<U32>(self, 0xFC4, arg);
        return;
    }
    vtslot<Fn1>(self, 0x6C)(self, nullptr, 0x9401u);
    st<U32>(self, 0xFC4, arg);
}

// 00687FD0: genuine 13-byte A->B thunk into accepted get_sw_party.
extern "C" void* __fastcall kotor_thunk_00687fd0(void*, void*) {
    return kotor_fastcall_005ed8b0(ld<void*>(app_manager(), 4), nullptr);
}

// 005F4D60: random creature sound helper.
// Signed modulo: the original is call rand; cdq; mov ecx,3; idiv ecx and then
// tests the EDX remainder.  crt_rand returns 0..0x7FFF, so the three branches
// are 0/1/2 -> sound 7/8/9.  The original's three RET 4 exits collapse to the
// single __fastcall RET 4 emitted for this one-stack-argument body.
extern "C" void __fastcall kotor_nested_005f4d60(void*, void*, void* creature) {
    const int rem = crt_rand_006fc447() % 3;
    if (rem == 0) {
        recovered_client_creature_play_sound_set_sound_00611470(creature, nullptr, 7u, 1u);
        return;
    }
    if (rem == 1) {
        recovered_client_creature_play_sound_set_sound_00611470(creature, nullptr, 8u, 1u);
        return;
    }
    recovered_client_creature_play_sound_set_sound_00611470(creature, nullptr, 9u, 1u);
}
