#pragma once
// lane-m47-core: shared template bodies of the CResHelper<CResX,T> members (see aurora_helpers.cpp header).
#include "03c44705a44b5536_aurora_family.h"
namespace aurora_family_impl {
using aurora_family::Word;
using aurora_family::Signed;
using aurora_family::String;
using aurora_family::HelperMethods;
using aurora_family::ResourceCtor;
using aurora_family::load;
using aurora_family::store;
using aurora_family::call_slot0;
using font_callback_read::aurora_get_demands_004064b0;
using font_callback_read::aurora_processed_pointer_00712f90;
struct Allocation {
    void* raw;
    ~Allocation() noexcept { if (raw) live_free(raw); }
};
template <class H> using Dtor = void (__fastcall*)(H*, void*) noexcept;
template <class H> using SetRef = void (__fastcall*)(H*, void*, menu_resources::Ref*, Signed) noexcept(false);
template <class H> using Ctor = H* (__fastcall*)(H*, void*, menu_resources::Ref, Signed) noexcept(false);
template <class H> using LoadFn = Signed (__fastcall*)(H*, void*) noexcept(false);
template <class H> struct BaseCleanup {
    H* object; Dtor<H> destroy;
    ~BaseCleanup() noexcept { if (object) destroy(object, nullptr); }
};

// Base destructor template (0070FB30 shape).
template <class H> inline void base_destroy(H* self, const HelperMethods* base) noexcept {
    void* const initial = self->resource;                                        // +03
    self->methods = base;                                                        // +08
    if (!initial) return;                                                        // +0E
    if (!c2da_not_equal_text_00406060(&self->reference, nullptr, c2da_empty_0073d71c())) return; // +10..+1F
    if (self->requested) {                                                       // +21
        c2da_cancel_00409b00(self->resource, nullptr);                           // +28..+2B
        self->requested = 0;                                                     // +30
    }
    if (c2da_release_00409cf0(c2da_manager_007a39e8(), nullptr, self->resource)) return; // +37..+48
    if (void* resource = self->resource) call_slot0(resource, 1u);               // +4A..+55
    self->resource = nullptr;                                                    // +57
}
template <class H> inline void* deleting(void* self, Word flags, Dtor<H> destroy) noexcept {
    destroy(static_cast<H*>(self), nullptr);                                     // +03
    if ((flags & 1u) != 0u) live_free(self);                                     // +08..+10 006FA390
    return self;                                                                 // RET 4
}
// SetResRef template (0070FB90 shape, RET 8).
template <class H> inline void set_ref(H* self, menu_resources::Ref* incoming, Signed flag, Word tag, Word size, ResourceCtor ctor) {
    if (resource_equal_004060b0(incoming, nullptr, &self->reference)) return;   // +24..+32
    if (self->resource && c2da_not_equal_text_00406060(&self->reference, nullptr, c2da_empty_0073d71c())) { // +38..+4D
        if (self->requested) c2da_cancel_00409b00(self->resource, nullptr);     // +4F..+59 (flag kept)
        if (!c2da_release_00409cf0(c2da_manager_007a39e8(), nullptr, self->resource)) { // +5E..+6F
            if (void* resource = self->resource) call_slot0(resource, 1u);       // +71..+7C
            self->resource = nullptr;                                            // +7E
        }
    }
    if (c2da_equal_text_004060d0(incoming, nullptr, c2da_empty_0073d71c())) {   // +85..+93
        self->resource = nullptr;
        menu_resources::Ref discarded;
        (void)unresolved_ref_assign_00406290(&self->reference, nullptr, &discarded, c2da_empty_0073d71c());
        return;
    }
    void* selected = c2da_lookup_004074d0(c2da_manager_007a39e8(), nullptr, incoming, tag); // +C1..+CD
    if (!selected) {
        void* const raw = live_operator_new(size);                               // +D5..+D7
        {
            Allocation guard{raw};                                               // state 0
            selected = raw ? ctor(raw, nullptr) : nullptr;                       // +E3..+F6
            guard.raw = nullptr;                                                 // state -1
        }
        c2da_register_00407680(c2da_manager_007a39e8(), nullptr, incoming, tag, selected); // +F8..+10A
        if (!selected) return;                                                   // +10F..+111
    }
    self->resource = selected;                                                   // +119
    if (flag) {
        self->requested = 1;                                                     // +120
        c2da_request_00408620(selected, nullptr);                                // +127
    }
    menu_resources::Ref discarded;
    (void)c2da_copy_00406120(&self->reference, nullptr, &discarded, incoming);   // +12C..+136
}
// Constructor template (0070FD10 shape, RET 14h): base table, default Ref, resource 0, requested 0,
// derived table, SetResRef(&incoming arg slot, mode).
template <class H> inline H* construct(H* self, menu_resources::Ref* incoming, Signed mode, const HelperMethods* base,
                                const HelperMethods* derived, SetRef<H> set, Dtor<H> base_dtor) {
    self->methods = base;                                                        // +25
    (void)c2da_default_ref_00405ed0(&self->reference, nullptr);                  // +2B
    self->resource = nullptr;                                                    // +3C
    menu_resources::Ref discarded;
    (void)unresolved_ref_assign_00406290(&self->reference, nullptr, &discarded, c2da_empty_0073d71c()); // +30..+43
    self->requested = 0;                                                         // +48
    BaseCleanup<H> cleanup{self, base_dtor};                                     // state 0
    self->methods = derived;                                                     // +63
    set(self, nullptr, incoming, mode);                                          // +54/+69
    cleanup.object = nullptr;
    return self;
}
// Allocating getter template (0070E5E0 shape, cdecl one pointer).
template <class H, class Info> inline H* getter(String* name, Ctor<H> ctor, Info info, LoadFn<H> load_fn, std::uint16_t tag) {
    H* object = nullptr;
    void* const raw = live_operator_new(0x1c);                                   // +1A..+1C
    if (raw) {
        Allocation guard{raw};                                                   // state 0
        menu_resources::Ref outgoing;
        (void)async_ref_construct_string_00406d60(&outgoing, nullptr, name);     // +36..+46 (outgoing arg area)
        object = ctor(static_cast<H*>(raw), nullptr, outgoing, 0);              // +4B..+4D
        guard.raw = nullptr;                                                     // state -1
    }
    if (!object) return nullptr;                                                 // +58..+62
    if (void* resource = object->resource) {                                     // +64..+69
        if (aurora_get_demands_004064b0(resource, nullptr) > 0) {                // +6B..+72 (JLE)
            const void* tagged = info(object, nullptr);                          // +74..+76
            if (tagged && load<std::uint16_t>(tagged, 0) == tag) {               // +7B..+83
                void* const owner = load<void*>(tagged, 2);                      // +85
                if (owner) {                                                     // +88..+8A
                    call_slot0(object, 1u);                                      // +8C..+92
                    object = static_cast<H*>(owner);                             // +94
                }
            }
        }
    }
    if (!object) return nullptr;                                                 // +96..+98
    if (load_fn(object, nullptr)) return object;                                 // +9A..+A3
    void* const resource = object->resource;                                     // +A5
    if (resource && aurora_get_demands_004064b0(resource, nullptr) >= 1) return nullptr; // +A8..+B4
    call_slot0(object, 1u);                                                      // +B6..+BC
    return nullptr;
}
// Simple Load template (0070FAC0 shape): Demand; WORD tag + owner DWORD at data - displacement.
template <class H> inline Signed load_tagged(H* self, Word displacement, std::uint16_t tag) {
    if (!c2da_current_resource_demand_00409b10(self->resource, nullptr)) return 0; // +03..+10
    unsigned char* const at = static_cast<unsigned char*>(c2da_current_resource_data_00404dd0(self->resource, nullptr))
                              - displacement;                                    // +11..+19
    store<H*>(at, 2, self);                                                      // +1C
    store<std::uint16_t>(at, 0, tag);                                            // +1F
    return 1;
}
// GetPtrToResHelperInfo shape of 00710490 / 00710870: data - displacement, 0, or literal -displacement.
template <class H> inline void* info_displaced(H* self, Word displacement) {
    void* resource = self->resource;                                             // +03
    if (!resource || aurora_get_demands_004064b0(resource, nullptr) < 1) return nullptr; // +06..+12, +3B
    resource = self->resource;                                                   // +14
    if (!resource || aurora_get_demands_004064b0(resource, nullptr) < 1)        // +17..+23
        return reinterpret_cast<void*>(static_cast<std::uintptr_t>(0u - displacement));   // +32..+34
    const Word data = static_cast<Word>(reinterpret_cast<std::uintptr_t>(
        c2da_current_resource_data_00404dd0(self->resource, nullptr)));         // +25..+28
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(data - displacement)); // +2D
}
inline bool demanded(void* resource) { return resource && aurora_get_demands_004064b0(resource, nullptr) >= 1; }
}

