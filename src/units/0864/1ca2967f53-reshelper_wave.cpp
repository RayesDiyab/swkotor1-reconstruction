// lane-resman rev3: CResHelper<CResWave,4> SetResRef 005D5E90 and ~CResHelper 005DAED0.
// Consumer: lane-sound rev3 sound_source_object.cpp:35-38 (exact names/types below).
// Same CResHelper template algorithm as the staged M46 owners tpc-setresref.obj
// (0070F800, CResTPC 0x0BBF) and the Task10 C2DA donor; instantiated with type 4,
// a 0x3C-byte CResWave (ctor 005DF1B0) and helper vtable 0074D2E4. Static evidence
// only (rev3\evidence\reshelper_wave.txt); never executed.
//
// Helper layout (observed): +00 vtable, +04 requested flag, +08 CRes*, +0C CResRef (16).
#include "15d7bcda8b-resources.h"   // menu_resources::{Manager,Ref,Word}
#include <cstdint>
#include <cstring>

using Word = std::uint32_t;
using Ref = menu_resources::Ref;
using Manager = menu_resources::Manager;

// ---- M46-defined lowers (exact existing symbols; rev3\build\m46-defined.json)
extern "C" {
Manager* __cdecl c2da_manager_007a39e8();                                       // [007A39E8]
const char* __cdecl c2da_empty_0073d71c();                                      // 0073D71C ""
int __fastcall resource_equal_004060b0(Ref*, void*, Ref*);                      // CResRef::operator==(CResRef const&)
int __fastcall c2da_not_equal_text_00406060(Ref*, void*, const char*);          // CResRef::operator!=(char const*)
int __fastcall c2da_equal_text_004060d0(Ref*, void*, const char*);              // CResRef::operator==(char const*)
Ref* __fastcall unresolved_ref_assign_00406290(Ref*, void*, Ref*, const char*); // CResRef::operator=(char const*) (hidden out)
Ref* __fastcall c2da_copy_00406120(Ref*, void*, Ref*, Ref*);                    // CResRef::operator=(CResRef const&) (hidden out)
void* __fastcall c2da_lookup_004074d0(Manager*, void*, Ref*, Word);             // CExoResMan::GetResObject
void __fastcall c2da_register_00407680(Manager*, void*, Ref*, Word, void*);     // CExoResMan::SetResObject
void __fastcall c2da_request_00408620(void*, void*);                            // CRes::Request
void __fastcall c2da_cancel_00409b00(void*, void*);                             // CRes::CancelRequest
int __fastcall c2da_release_00409cf0(Manager*, void*, void*);                   // CExoResMan::ReleaseResObject
void* __cdecl c2da_directory_new_006fa7e6(std::uint32_t) noexcept(false);       // 006FA7E6 operator new
void __cdecl live_free(void*);                                                  // 006FA390
// MISSING in M46 (CResWave cohort owner: lane-sound / resource cohort): 005DF1B0
// CResWave::CResWave = CRes::CRes 00406460, zero +28..+38, vtable 0074D398
// (005DF380, 00406490, 0063E7F0, 005DF1F0, 005DF230, 005DF360). RET0, EAX=this.
void* __fastcall res_wave_construct_005df1b0(void* self, void* unused_edx) noexcept(false);
}
// lane-sound rev3 owner of helper vtable 0074D2E4 (sound_source_object.cpp:55-65).
namespace kotor_sound_rev3 {
using Deleting = void* (__fastcall*)(void*, void*, std::uint32_t);
extern const Deleting vtable_0074d2e4[1];
}

// ---- consumer-required names (lane-sound rev3 sound_source_object.cpp:35-38)
namespace kotor_sound_rev3_required {
void __fastcall res_helper_set_ref_005d5e90(void* helper, void*, const void* ref, std::uint32_t mode); // RET 8
void __fastcall res_helper_destroy_005daed0(void* helper, void*);                                   // RET 0
}

namespace {
unsigned char* at(void* p, unsigned off) { return static_cast<unsigned char*>(p) + off; }
Word rd(void* p, unsigned off) { Word v; std::memcpy(&v, at(p, off), 4); return v; }
void wr(void* p, unsigned off, Word v) { std::memcpy(at(p, off), &v, 4); }
void* rp(void* p, unsigned off) { void* v; std::memcpy(&v, at(p, off), 4); return v; }
void wp(void* p, unsigned off, const void* v) { std::memcpy(at(p, off), &v, 4); }
Ref* reference(void* helper) { return reinterpret_cast<Ref*>(at(helper, 0x0c)); }
// CALL [[res]+0] with flags 1 (CRes scalar deleting dtor; D1 fastcall slot type).
void delete_resource(void* resource) {
    kotor_sound_rev3::Deleting destroy;
    std::memcpy(&destroy, rp(resource, 0), sizeof(destroy));
    (void)destroy(resource, nullptr, 1u);
}
struct ResourceAllocation {                                                     // EH 0072401B state 0 -> free
    void* raw;
    ~ResourceAllocation() { if (raw) live_free(raw); }
};
}

// 005D5E90 CResHelper<CResWave,4>::SetResRef(CResRef const&, int request) RET8.
void __fastcall kotor_sound_rev3_required::res_helper_set_ref_005d5e90(
    void* helper, void*, const void* ref, std::uint32_t mode) {
    Ref* const incoming = static_cast<Ref*>(const_cast<void*>(ref));
    if (resource_equal_004060b0(incoming, nullptr, reference(helper))) return;          // 005D5EB1-005D5EC2
    if (rp(helper, 8) &&                                                                 // 005D5EC8-005D5ECD
        c2da_not_equal_text_00406060(reference(helper), nullptr, c2da_empty_0073d71c())) { // 005D5ECF-005D5EDD
        if (rd(helper, 4)) c2da_cancel_00409b00(rp(helper, 8), nullptr);                 // 005D5EDF-005D5EE9 (flag kept)
        if (!c2da_release_00409cf0(c2da_manager_007a39e8(), nullptr, rp(helper, 8))) {   // 005D5EEE-005D5EFF
            if (void* resource = rp(helper, 8)) delete_resource(resource);               // 005D5F01-005D5F0C
            wp(helper, 8, nullptr);                                                      // 005D5F0E
        }
    }
    if (c2da_equal_text_004060d0(incoming, nullptr, c2da_empty_0073d71c())) {            // 005D5F15-005D5F23
        wp(helper, 8, nullptr);                                                          // 005D5F31
        Ref discarded;
        (void)unresolved_ref_assign_00406290(reference(helper), nullptr, &discarded, c2da_empty_0073d71c()); // 005D5F25-005D5F38
        return;
    }
    void* selected = c2da_lookup_004074d0(c2da_manager_007a39e8(), nullptr, incoming, 4u); // 005D5F51-005D5F5A
    if (!selected) {                                                                     // 005D5F61-005D5F63
        void* const raw = c2da_directory_new_006fa7e6(0x3c);                             // 005D5F65-005D5F67
        {
            ResourceAllocation guard{raw};                                               // state 0 (005D5F75)
            selected = raw ? res_wave_construct_005df1b0(raw, nullptr) : nullptr;        // 005D5F73-005D5F86
            guard.raw = nullptr;                                                         // state -1 (005D5F92)
        }
        c2da_register_00407680(c2da_manager_007a39e8(), nullptr, incoming, 4u, selected); // 005D5F88-005D5F9A
        if (!selected) return;                                                           // 005D5F9F-005D5FA1
    }
    wp(helper, 8, selected);                                                             // 005D5FA9
    if (mode) {                                                                          // 005D5FA3-005D5FAC
        wr(helper, 4, 1);                                                                // 005D5FB0
        c2da_request_00408620(selected, nullptr);                                        // 005D5FAE-005D5FB7
    }
    Ref discarded;
    (void)c2da_copy_00406120(reference(helper), nullptr, &discarded, incoming);          // 005D5FBC-005D5FC6
}

// 005DAED0 CResHelper<CResWave,4>::~CResHelper RET0.
void __fastcall kotor_sound_rev3_required::res_helper_destroy_005daed0(void* helper, void*) {
    void* const resource = rp(helper, 8);                                                // 005DAED3
    wp(helper, 0, kotor_sound_rev3::vtable_0074d2e4);                                     // 005DAED8
    if (!resource) return;                                                               // 005DAED6/005DAEDE
    if (!c2da_not_equal_text_00406060(reference(helper), nullptr, c2da_empty_0073d71c())) return; // 005DAEE0-005DAEEF
    if (rd(helper, 4)) {                                                                 // 005DAEF1-005DAEF6
        c2da_cancel_00409b00(rp(helper, 8), nullptr);                                    // 005DAEF8-005DAEFB
        wr(helper, 4, 0);                                                                // 005DAF00 (cleared here, unlike SetResRef)
    }
    if (!c2da_release_00409cf0(c2da_manager_007a39e8(), nullptr, rp(helper, 8))) {       // 005DAF07-005DAF18
        if (void* live = rp(helper, 8)) delete_resource(live);                           // 005DAF1A-005DAF25 (reloaded)
        wp(helper, 8, nullptr);                                                          // 005DAF27
    }
}
