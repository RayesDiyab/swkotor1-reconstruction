// lane-sound rev3: CExoSoundSource object family reached on the first frame via
// CSWGuiManager::LoadGuiSounds 00409F00 (StartServices 005F9451).
//   005D60E0 CExoSoundSource::CExoSoundSource(const CResRef*)   RET 4
//   005D60A0 CExoSoundSource scalar deleting dtor (vtable 0074C608 slot 0)
//   005D5900 CExoSoundSource::SetPriorityGroup thunk -> 005DBB20  RET 4
//   005DBB20 CExoSoundSourceInternal::SetPriorityGroup             RET 4
//   005DBA40 CExoSoundSourceInternal::SetPitchVariance             RET 4
//   005DAF50 CExoSoundSourceInternal ctor (vtables 0074D2E4 -> 0074D2E8) RET 4
//   005DAF30 / 005DBB00 / 005DB480 CResHelper and Internal deleting dtors / dtor
// With Disable Sound ([00832894] != 0) the original stores vtable 0074C608 and a
// null internal and returns (005D60F9-005D6120); SetPriorityGroup then returns at
// 005D590C. This unit reproduces exactly that, with no other effect.
#include <cstdint>
#include <cstring>

namespace sound_lifecycle29_required { extern volatile std::uint32_t* const word_00832894; } // lane-sound owner
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);                      // 006FA7E6 (live-policy.obj)
void __cdecl live_free(void*);                                       // 006FA390
extern void* kotor_cell_sound_007a39ec;                              // lane-sound owner (007A39EC)
// 00406290 CResRef::operator=(const char*) carrier (resource-ref-assignment.obj): (this, edx, out, text), RET 8.
void* __fastcall unresolved_ref_assign_00406290(void*, void*, void*, const char*);
__declspec(dllimport) void __stdcall AIL_set_3D_sample_playback_rate(void*, std::int32_t);
__declspec(dllimport) void __stdcall AIL_set_sample_playback_rate(void*, std::int32_t);
}
namespace base_ini_reconstruction { extern const char warning_empty_0073d71c[]; }        // 0073D71C ""
namespace sound_voice_required { void __fastcall actor_volume_005db800(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t); } // lane-sound sound_volume.cpp
namespace sound_pause29_required {
std::uint32_t __fastcall actor_pause_005db150(void*, void*);        // donor sound-lower-r6r3
std::uint32_t __fastcall actor_resume_005db1c0(void*, void*);       // donor sound-lower-r6r3
}
namespace kotor_sound_internal { void __fastcall PitchVariance005db3d0(void*, void*); } // lane-sound sound_source.cpp
namespace kotor_sound_path_required { void __fastcall shutdown_source_005db210(void*, void*); } // lane-sound sound_lists.cpp
// Still-missing resource-side lowers (resman owner): declared, not implemented here.
namespace kotor_sound_rev3_required {
void __fastcall res_helper_set_ref_005d5e90(void* helper, void*, const void* ref, std::uint32_t mode); // 005D5E90, RET 8
void __fastcall res_helper_destroy_005daed0(void* helper, void*);                                   // 005DAED0, RET 0
}

namespace {
using U = std::uint32_t;
unsigned char* at(void* p, unsigned off) { return static_cast<unsigned char*>(p) + off; }
U rd(void* p, unsigned off) { U v; std::memcpy(&v, at(p, off), 4); return v; }
void wr(void* p, unsigned off, U v) { std::memcpy(at(p, off), &v, 4); }
void* rp(void* p, unsigned off) { void* v; std::memcpy(&v, at(p, off), 4); return v; }
void wp(void* p, unsigned off, const void* v) { std::memcpy(at(p, off), &v, 4); }
float rf(void* p, unsigned off) { float v; std::memcpy(&v, at(p, off), 4); return v; }
void wf(void* p, unsigned off, float v) { std::memcpy(at(p, off), &v, 4); }
using Deleting = void* (__fastcall*)(void*, void*, U);
void* virtual_delete(void* object, U flags) {                         // CALL [[obj]+0], push flags (005D60B0-005D60B4)
    // lane-m58-exitsound: the slot word is read FROM the table (rp(object,0)); the former
    // rp(rp(object,0),0) source read 4 code bytes of the dtor itself (EIP 56EC8B55 at exit).
    Deleting d; std::memcpy(&d, rp(object, 0), 4); return d(object, nullptr, flags);
}
}

namespace kotor_sound_rev3 {
void* __fastcall SourceDelete005d60a0(void* self, void*, U flags);
void* __fastcall InternalDelete005dbb00(void* self, void*, U flags);
void* __fastcall HelperDelete005daf30(void* self, void*, U flags);
// Single-slot vtables, as in the image (0074C608 -> 005D60A0; 0074D2E8 -> 005DBB00; 0074D2E4 -> 005DAF30).
extern const Deleting vtable_0074c608[1];
extern const Deleting vtable_0074d2e8[1];
extern const Deleting vtable_0074d2e4[1];
const Deleting vtable_0074c608[1] = {&SourceDelete005d60a0};
const Deleting vtable_0074d2e8[1] = {&InternalDelete005dbb00};
const Deleting vtable_0074d2e4[1] = {&HelperDelete005daf30};

// 005DAF50 CExoSoundSourceInternal::CExoSoundSourceInternal(system), RET 4, EAX=this.
void* __fastcall InternalConstruct005daf50(void* self, void*, void* system) {
    wp(self, 0, vtable_0074d2e4);                                    // 005DAF5D
    std::memset(at(self, 0x0c), 0, 16);                              // 005DAF63 (00405ED0)
    wr(self, 8, 0);                                                  // 005DAF76
    unsigned char discarded[16];                                     // [esp+0Ch] scratch out-param
    (void)unresolved_ref_assign_00406290(at(self, 0x0c), nullptr, discarded, base_ini_reconstruction::warning_empty_0073d71c); // 005DAF68-005DAF79
    wr(self, 4, 0);                                                  // 005DAF82
    wp(self, 0, vtable_0074d2e8);                                    // 005DAF85
    wr(self, 0x78, 0); wr(self, 0x7c, 0); wr(self, 0x80, 0);         // 005DAF8B-005DAF91
    wr(self, 0x8c, 0); wr(self, 0x90, 0); wr(self, 0x94, 0);         // 005DAF97-005DAFA3
    wp(self, 0x44, system);                                          // 005DAFA9
    wr(self, 0x40, 0); wr(self, 0x1c, 0); wr(self, 0x24, 0);         // 005DAFAC-005DAFB2
    *at(self, 0x85) = 0x7f;                                          // 005DAFB5-005DAFBB ([0074C5E8]=7Fh)
    wr(self, 0x28, 0); wr(self, 0x3c, 0); wr(self, 0x38, 0); wr(self, 0x88, 0); // 005DAFC1-005DAFCA
    *at(self, 0x34) = 0;                                             // 005DAFD0
    wr(self, 0x70, 0x41200000u); wr(self, 0x6c, 0x41a00000u);        // 005DAFD3-005DAFDA (10.0f / 20.0f)
    *at(self, 0x35) = 0x17;                                          // 005DAFE1
    *at(self, 0x84) = *at(rp(system, 0x4c), 0x22f);                  // 005DAFE5-005DAFEE (group 0x17 volume byte)
    wr(self, 0x74, 0); wr(self, 0x68, 0); wr(self, 0x30, 0);         // 005DAFF4-005DAFFA
    wr(self, 0x5c, 0); wr(self, 0x64, 0); wr(self, 0x58, 0);         // 005DAFFD-005DB003
    const U counter = rd(system, 0x58);                              // 005DB006
    wr(system, 0x58, counter + 1);                                   // 005DB009-005DB00C
    wr(self, 0x60, counter);                                         // 005DB011
    wr(self, 0x8c, 0); wr(self, 0x90, 0); wr(self, 0x98, 0); wr(self, 0x9c, 0); wr(self, 0x94, 0); // 005DB014-005DB031
    return self;                                                     // 005DB037
}
// 005DB480 CExoSoundSourceInternal::~CExoSoundSourceInternal, RET 0 (EH 00724248 state 0 -> ~CResHelper).
void __fastcall InternalDestroy005db480(void* self, void*) {
    wp(self, 0, vtable_0074d2e8);                                    // 005DB49D
    struct Base { void* s; ~Base() { kotor_sound_rev3_required::res_helper_destroy_005daed0(s, nullptr); } } base{self}; // 005DB4BA
    kotor_sound_path_required::shutdown_source_005db210(self, nullptr); // 005DB4AB
}
// 005DBB00 / 005DAF30 / 005D60A0 scalar deleting dtors (TEST [esp+8],1 -> _free), RET 4, EAX=this.
void* __fastcall InternalDelete005dbb00(void* self, void*, U flags) {
    InternalDestroy005db480(self, nullptr);                          // 005DBB03
    if (flags & 1) live_free(self);                                  // 005DBB08-005DBB10
    return self;
}
void* __fastcall HelperDelete005daf30(void* self, void*, U flags) {
    kotor_sound_rev3_required::res_helper_destroy_005daed0(self, nullptr); // 005DAF33
    if (flags & 1) live_free(self);                                  // 005DAF38-005DAF40
    return self;
}
void* __fastcall SourceDelete005d60a0(void* self, void*, U flags) {
    void* internal = rp(self, 4);                                    // 005D60A3
    wp(self, 0, vtable_0074c608);                                    // 005D60A8
    if (internal) { (void)virtual_delete(internal, 1); wr(self, 4, 0); } // 005D60AE-005D60B6
    if (flags & 1) live_free(self);                                  // 005D60BD-005D60C5
    return self;
}
// 005D60E0 CExoSoundSource::CExoSoundSource(const CResRef*), RET 4, EAX=this.
void* __fastcall SourceConstruct005d60e0(void* self, void*, const void* ref) {
    wp(self, 0, vtable_0074c608);                                    // 005D60F9
    if (*sound_lifecycle29_required::word_00832894 != 0) {           // 005D60FF-005D6106 (Disable Sound)
        wr(self, 4, 0);                                              // 005D6108
        return self;                                                 // 005D610F-005D6120
    }
    void* raw = live_operator_new(0xa0);                             // 005D6123-005D6128
    struct Guard { void* p; ~Guard() { if (p) live_free(p); } } guard{raw}; // EH 00723FFB state 0 -> 00723FF0 _free
    void* internal = raw ? InternalConstruct005daf50(raw, nullptr, rp(kotor_cell_sound_007a39ec, 0)) : nullptr; // 005D6140-005D614B (([007A39EC])+0 = CExoSoundInternal*)
    guard.p = nullptr;                                               // 005D6156
    wp(self, 4, internal);                                           // 005D615E
    if (internal) kotor_sound_rev3_required::res_helper_set_ref_005d5e90(internal, nullptr, ref, 0); // 005D6161-005D616C
    return self;                                                     // 005D6175
}
// 005DBA40 CExoSoundSourceInternal::SetPitchVariance(float), RET 4.
void __fastcall InternalSetPitchVariance005dba40(void* self, void*, float variance) {
    float v;
    if (0.0f > variance) v = 0.0f;                                   // 005DBA40-005DBA52 / 005DBA6D
    else if (1.0f >= variance || variance != variance) v = variance; // 005DBA54-005DBA63 / 005DBA75
    else v = 1.0f;                                                   // 005DBA65
    wf(self, 0x74, v);                                               // 005DBA7C
    wr(self, 0x68, 0);                                               // 005DBA7F
    if (rd(rp(self, 0x44), 8) == 0) return;                          // 005DBA79-005DBA8B
    (void)sound_pause29_required::actor_pause_005db150(self, nullptr); // 005DBA8F
    void* v3 = rp(self, 0x3c);
    if (v3 && rp(v3, 4)) {                                           // 005DBA94-005DBAA0
        kotor_sound_internal::PitchVariance005db3d0(self, nullptr);  // 005DBAA4
        AIL_set_3D_sample_playback_rate(rp(rp(self, 0x3c), 4), static_cast<std::int32_t>(rd(self, 0x54))); // 005DBAA9-005DBAB4
        (void)sound_pause29_required::actor_resume_005db1c0(self, nullptr); // 005DBABC
        return;
    }
    void* v2 = rp(self, 0x38);
    if (v2 && rp(v2, 4)) {                                           // 005DBAC5-005DBAD1
        kotor_sound_internal::PitchVariance005db3d0(self, nullptr);  // 005DBAD5
        AIL_set_sample_playback_rate(rp(rp(self, 0x38), 4), static_cast<std::int32_t>(rd(self, 0x54))); // 005DBADA-005DBAE5
    }
    (void)sound_pause29_required::actor_resume_005db1c0(self, nullptr); // 005DBAEB-005DBAED
}
// 005DBB20 CExoSoundSourceInternal::SetPriorityGroup(group), RET 4.
void __fastcall InternalSetPriorityGroup005dbb20(void* self, void*, U group_word) {
    void* system = rp(self, 0x44);                                   // 005DBB23
    if (rd(system, 8) == 0) return;                                  // 005DBB26-005DBB2B
    const unsigned char g = static_cast<unsigned char>(group_word);  // 005DBB2D
    *at(self, 0x35) = g;                                             // 005DBB31
    unsigned char* rec = at(rp(system, 0x4c), static_cast<U>(g) * 24u); // 005DBB34-005DBB42
    *at(self, 0x84) = rec[7];                                        // 005DBB3E / 005DBB56
    sound_voice_required::actor_volume_005db800(self, nullptr, *at(self, 0x85), 0, 0x3f800000u); // 005DBB45-005DBB5D
    InternalSetPitchVariance005dba40(self, nullptr, rf(rec, 0x10));  // 005DBB62-005DBB6B
    wf(self, 0x70, rf(rec, 0x0c));                                   // 005DBB73-005DBB77
    wf(self, 0x6c, rf(rec, 8));                                      // 005DBB70 / 005DBB7A
}
// 005D5900 CExoSoundSource::SetPriorityGroup: internal ? tail JMP 005DBB20 : RET 4.
void __fastcall SourceSetPriorityGroup005d5900(void* self, void*, U group_word) {
    if (void* internal = rp(self, 4)) InternalSetPriorityGroup005dbb20(internal, nullptr, group_word);
}
}
