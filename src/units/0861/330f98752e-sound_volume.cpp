// Lane-sound batch 2: ApplyNewVolume, listener updates, source SetVolume,
// stream fade begin, CExoStreamingSoundSourceInternal ctor/dtor, small getters.
// lane-m48-b D81(3) REPLACEMENT COPY of the M47 stage unit sound-volume (sound/sound_volume.cpp). Only change: the two
// modern UCRT pow calls are replaced by the original CRT _pow port genuine_x87_pow_006fc1f0 (x87-pow-entry,
// D30-FINAL), called exactly like the original: ST1 = base (the x87 product), ST0 = qword [0074D300] exponent,
// call 006FC1F0, result in ST0 (005DB90F..005DB91B, 005DC868..005DC873).
#include "9e2ca73358-sound_internal.h"
#include <cmath>
#include <cstdio>
#include <cstring>

namespace kotor_sound_math { std::uint32_t __cdecl Equal004aa980(const float*, const float*); }
namespace kotor_sound_internal { std::uint32_t __fastcall SourceSliderType005d6a90(void*, void*, void*); }
namespace sound_volume29_required {
std::uint32_t __fastcall predicate_006054e0(void*, void*, void*);
float __fastcall gain_005d6b10(void*, void*, std::uint32_t, std::uint32_t);
}
namespace sound_lower29_required { void __fastcall volume_005dc930(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t); }
namespace kotor_sound_path_required {
std::uint32_t __fastcall remove_streaming_005d7ab0(void*, void*, void*);
std::uint32_t __fastcall remove_nonplaying_streaming_005d7a00(void*, void*, void*);
}
extern "C" std::uint32_t __stdcall unresolved_GetTickCount_005ec200();
// Original x87 transport (owner: x87-pow-entry _genuine_x87_pow_006fc1f0); consumed only from inline asm below (same
// declaration as the other stage consumers, e.g. 3d/model46/f5_p2p.cpp).
extern "C" void __cdecl genuine_x87_pow_006fc1f0();
extern "C" {
__declspec(dllimport) void* __stdcall AIL_open_3D_listener(void*);
__declspec(dllimport) void __stdcall AIL_close_3D_listener(void*);
__declspec(dllimport) void __stdcall AIL_set_3D_orientation(void*, float, float, float, float, float, float);
__declspec(dllimport) void __stdcall AIL_set_3D_position(void*, float, float, float);
__declspec(dllimport) std::int32_t __stdcall AIL_3D_room_type(void*);
__declspec(dllimport) void __stdcall AIL_set_3D_sample_effects_level(void*, float);
__declspec(dllimport) void __stdcall AIL_set_3D_sample_volume(void*, float);
__declspec(dllimport) void __stdcall AIL_set_sample_volume_levels(void*, float, float);
__declspec(dllimport) std::int32_t __stdcall AIL_stream_status(void*);
}

namespace {
using U = std::uint32_t;
unsigned char* at(void* p, unsigned off) { return static_cast<unsigned char*>(p) + off; }
U rd(void* p, unsigned off) { U v; std::memcpy(&v, at(p, off), 4); return v; }
void wr(void* p, unsigned off, U v) { std::memcpy(at(p, off), &v, 4); }
void* rp(void* p, unsigned off) { void* v; std::memcpy(&v, at(p, off), 4); return v; }
float rf(void* p, unsigned off) { float v; std::memcpy(&v, at(p, off), 4); return v; }
unsigned char rb(void* p, unsigned off) { return *at(p, off); }
void* bp(U v) { void* p; std::memcpy(&p, &v, 4); return p; }
// call 006FC1F0 (_pow) with ST1 = base, ST0 = exponent; ST0 = result (x87 transport, D30-FINAL).
__declspec(noinline) double x87_pow_006fc1f0(double base, double exponent) {
    double result;
    __asm {
        fld qword ptr [base]
        fld qword ptr [exponent]
        call genuine_x87_pow_006fc1f0
        fstp qword ptr [result]
    }
    return result;
}
U fb(float f) { U v; std::memcpy(&v, &f, 4); return v; }
kotor_list::Internal* L(void* sys, unsigned off) { return static_cast<kotor_list::Internal*>(rp(sys, off)); }
void* next(kotor_list::Internal* l, void** pos) { return bp(unresolved_list_next_005e9b90(l, nullptr, pos)); }
const float minus_one = -1.0f;                                       // [00740CBC]
}

namespace sound_voice_required {
// 005DB800 CExoSoundSourceInternal::SetVolume(volume, keep, factor), RET 0Ch.
void __fastcall actor_volume_005db800(void* src, void*, std::uint32_t volume, std::uint32_t keep, std::uint32_t factor_bits) {
    void* sys = rp(src, 0x44);
    const U type = kotor_sound_internal::SourceSliderType005d6a90(sys, nullptr, src); // 005DB808-005DB80D
    if (keep == 0) *at(src, 0x85) = static_cast<unsigned char>(volume); // 005DB812-005DB81C
    U current = keep;                                                // EBP
    if (!sound_volume29_required::predicate_006054e0(sys, nullptr, src) && keep == 0) { // 005DB822-005DB831
        const float with = sound_volume29_required::gain_005d6b10(sys, nullptr, type, 1); // 005DB833-005DB83E
        const float without = sound_volume29_required::gain_005d6b10(sys, nullptr, type, current); // 005DB842-005DB847
        current = (without == with) ? 0u : 1u;                       // 005DB84C-005DB85E (TEST AH,44h / JNP)
    }
    float vol = sound_volume29_required::gain_005d6b10(sys, nullptr, type, current); // 005DB860-005DB86A
    float factor_arg; std::memcpy(&factor_arg, &factor_bits, 4);
    const unsigned char kind = rb(src, 0x35);                        // 005DB86E
    const double factor = (kind == 4 || (kind == 0x15 && rd(src, 0x38) != 0)) ? static_cast<double>(factor_arg) : 1.0; // 005DB871-005DB888
    const std::int32_t product = static_cast<std::int32_t>(rb(src, 0x84)) * static_cast<std::int32_t>(volume & 0xffu); // 005DB88C-005DB8A4
    const std::int32_t level = static_cast<std::int32_t>(static_cast<long long>(product * factor * static_cast<double>(0.007874015718698502f))); // 005DB8A8-005DB8BA (__ftol2, [0073EE00]=1/127)
    if (rd(sys, 8) == 0) return;                                     // 005DB8BE-005DB8C3
    if (rd(src, 0x1c)) {                                             // 3D
        void* voice = rp(src, 0x3c);
        if (!voice || !rp(voice, 4)) return;                         // 005DB8D0-005DB8E0
        if (rf(sys, 0x60) < 1.0f) vol = static_cast<float>(static_cast<double>(vol) * rf(sys, 0x60)); // 005DB8E6-005DB8FD
        const double x = static_cast<double>(level) * vol * static_cast<double>(0.007874015718698502f); // 005DB901-005DB909
        AIL_set_3D_sample_volume(rp(voice, 4), static_cast<float>(x87_pow_006fc1f0(x, 0.6000000238418579))); // 005DB90F-005DB91F ([0074D300] qword, _pow)
        return;
    }
    void* voice = rp(src, 0x38);
    if (!voice || !rp(voice, 4)) return;                             // 005DB92B-005DB937
    if (rf(sys, 0x60) > 1.0f) vol = static_cast<float>((2.0 - rf(sys, 0x60)) * vol); // 005DB939-005DB956 ([00741BE0]=2.0)
    const float f = static_cast<float>(static_cast<double>(level) * vol * static_cast<double>(0.007874015718698502f)); // 005DB95A-005DB968
    AIL_set_sample_volume_levels(rp(voice, 4), f, f);                 // 005DB96C-005DB973
}
}

namespace kotor_sound_path_required {
// Render's carrier name for 005DB800 (same body).
void __fastcall source_set_volume_005db800(void* src, void*, std::uint32_t v, std::uint32_t k, std::uint32_t f) {
    sound_voice_required::actor_volume_005db800(src, nullptr, v, k, f);
}
// 005D8110 CExoSoundInternal::ApplyNewVolume(keep), RET 4.
void __fastcall apply_new_volume_005d8110(void* sys, void*, std::uint32_t keep) {
    void* pos = rp(L(sys, 0x44), 0);                                 // 005D8115-005D811A
    void* st = pos ? rp(pos, 8) : nullptr;
    while (pos) {
        if (st) sound_lower29_required::volume_005dc930(st, nullptr, rb(st, 0x46), keep, 0x3f800000u); // 005D8130-005D8146
        st = next(L(sys, 0x44), &pos);                               // 005D814B-005D8153
    }
    pos = rp(L(sys, 0x40), 0);                                       // 005D8160-005D8165
    void* src = pos ? rp(pos, 8) : nullptr;
    while (pos) {
        if (src) {
            sound_voice_required::actor_volume_005db800(src, nullptr, rb(src, 0x85), keep, 0x3f800000u); // 005D8194-005D81A9
            if (rd(src, 0x1c)) {                                     // 005D81AE-005D81B3
                void* h = rp(rp(src, 0x3c), 4);
                if (rd(sys, 4) && AIL_3D_room_type(rp(sys, 0xf8)) != 0 && AIL_3D_room_type(rp(sys, 0xf8)) != -1) // 005D81B5-005D81D5
                    AIL_set_3D_sample_effects_level(h, rf(sys, 0x68)); // 005D81D7-005D81F0
                else
                    AIL_set_3D_sample_effects_level(h, 0.0f);         // 005D81E7-005D81F0
            }
        }
        src = next(L(sys, 0x40), &pos);                              // 005D81F2-005D81FF
    }
}
// 005D6540 SetListenerOrientation(a, b), RET 8.
void __fastcall listener_orientation_005d6540(void* sys, void*, void* a, void* b) {
    if (kotor_sound_math::Equal004aa980(reinterpret_cast<float*>(at(sys, 0x80)), static_cast<float*>(a)) &&
        kotor_sound_math::Equal004aa980(reinterpret_cast<float*>(at(sys, 0x8c)), static_cast<float*>(b))) return; // 005D6550-005D6574
    std::memcpy(at(sys, 0x80), a, 12);                               // 005D6576-005D6585
    void* provider = rp(sys, 0xf8);                                  // 005D658E
    std::memcpy(at(sys, 0x8c), b, 12);                               // 005D6596-005D65A5
    if (!provider) return;                                           // 005D65A8
    void* listener = AIL_open_3D_listener(provider);                 // 005D65AA-005D65AB (no null test, as observed)
    const float* fa = static_cast<float*>(a); const float* fbv = static_cast<float*>(b);
    AIL_set_3D_orientation(listener, fa[0], fa[1], static_cast<float>(static_cast<double>(fa[2]) * minus_one),
                           fbv[0], fbv[1], static_cast<float>(static_cast<double>(fbv[2]) * minus_one)); // 005D65B1-005D65DD
    AIL_close_3D_listener(listener);                                 // 005D65E3-005D65E4
}
// 005D6600 SetListenerPosition(p), RET 4.
void __fastcall listener_position_005d6600(void* sys, void*, void* p) {
    if (kotor_sound_math::Equal004aa980(reinterpret_cast<float*>(at(sys, 0x98)), static_cast<float*>(p))) return; // 005D660F-005D661B
    std::memcpy(at(sys, 0x98), p, 12);                               // 005D661D-005D662C
    void* provider = rp(sys, 0xf8);
    if (!provider) return;                                           // 005D662F-005D6637
    void* listener = AIL_open_3D_listener(provider);                 // 005D663A
    if (!listener) return;                                           // 005D6642-005D6644
    const float* fp = static_cast<float*>(p);
    AIL_set_3D_position(listener, fp[0], fp[1], static_cast<float>(static_cast<double>(fp[2]) * minus_one)); // 005D6646-005D665B
    AIL_close_3D_listener(listener);                                 // 005D6662
}
}

namespace sound_fade29_required {
// 005DC480 CExoStreamingSoundSourceInternal fade begin(span), RET 4.
void __fastcall begin_internal_005dc480(void* st, void*, std::uint32_t span) {
    if (rd(rp(st, 0x20), 8) == 0) return;                            // 005DC483-005DC48B
    void* h = rp(st, 0);
    if (!h) return;                                                  // 005DC48D-005DC491
    if (AIL_stream_status(h) != 4) return;                           // 005DC493-005DC4A4
    wr(st, 0x34, unresolved_GetTickCount_005ec200());                // 005DC4A6-005DC4B8 (005E9250)
    wr(st, 0x2c, span);                                              // 005DC4BB
}
}

namespace sound_lifecycle29_required {
// 005DBBE0 CExoStreamingSoundSourceInternal ctor(system), RET 4, EAX=this.
void* __fastcall construct_005dbbe0(void* st, void*, void* sys) {
    std::memset(at(st, 9), 0, 16);                                   // 005DBBE7-005DBBEA (00405ED0: four zero DWORDs)
    ::new (static_cast<void*>(at(st, 0x24))) kotor_reconstruction::StartupString(); // 005DBBEF-005DBBF2 (005B3190)
    wr(st, 0x5c, 0); wr(st, 0x60, 0); wr(st, 0x64, 0);               // 005DBBFD-005DBC03
    wr(st, 0x70, 0); wr(st, 0x74, 0); wr(st, 0x78, 0);               // 005DBC06-005DBC0C
    std::memcpy(at(st, 0x20), &sys, 4);                                     // 005DBC0F
    wr(st, 4, 0); wr(st, 0, 0); wr(st, 0x40, 0); wr(st, 0x2c, 0);    // 005DBC12-005DBC1A
    wr(st, 0x34, 0); wr(st, 0x1c, 0); wr(st, 0x38, 0);               // 005DBC1D-005DBC23
    *at(st, 0x46) = 0x7f;                                            // 005DBC26-005DBC2C ([0074C5E8]=7Fh)
    const U counter = rd(sys, 0x58);                                 // 005DBC2F
    wr(sys, 0x58, counter + 1);                                      // 005DBC32-005DBC35
    wr(st, 0x68, 1); wr(st, 0x6c, 1);                                // 005DBC38-005DBC40
    wr(st, 0x30, counter);                                           // 005DBC46
    wr(st, 0x58, 0); wr(st, 0x48, 0); wr(st, 0x4c, 0);               // 005DBC49-005DBC4F
    wr(st, 0x54, 0xbf800000u);                                       // 005DBC52 (-1.0f)
    *at(st, 8) = 2;                                                  // 005DBC59
    *at(st, 0x50) = static_cast<unsigned char>(rb(rp(rp(st, 0x20), 0x4c), 0x37)); // 005DBC5D-005DBC65
    wr(st, 0x70, 0); wr(st, 0x74, 0); wr(st, 0x7c, 0); wr(st, 0x80, 0); wr(st, 0x78, 0); // 005DBC68-005DBC86
    return st;                                                       // 005DBC89
}
// 005DBCA0 CExoStreamingSoundSourceInternal dtor, RET 0. EH 0072428B state 0 -> String +24.
void __fastcall destroy_005dbca0(void* st, void*) {
    struct Unwind { void* st; ~Unwind() { reinterpret_cast<kotor_reconstruction::StartupString*>(at(st, 0x24))->~StartupString(); } } unwind{st}; // 005DBD09-005DBD14
    void* sys = rp(st, 0x20);
    if (rd(st, 0x1c)) (void)kotor_sound_path_required::remove_streaming_005d7ab0(sys, nullptr, st);     // 005DBCBD-005DBCD0
    else (void)kotor_sound_path_required::remove_nonplaying_streaming_005d7a00(sys, nullptr, st);      // 005DBCD7
    if (rd(rp(st, 0x20), 8) == 0) return;                            // 005DBCDC-005DBCE4
    void* h = rp(st, 0);
    if (!h) return;                                                  // 005DBCE6-005DBCEA
    AIL_close_stream(h);                                             // 005DBCEC-005DBCED
    if (void* file = rp(st, 0x38)) std::fclose(static_cast<FILE*>(file)); // 005DBCF3-005DBCFB (006FACF0; FILE from InitializeSource's fopen family)
    wr(st, 0, 0);                                                    // 005DBD03
}
}

namespace kotor_sound_internal {
// 005DBBB0 CExoSoundInternal::GetForwardOrientation(out): copy [sys+80..88], RET 4.
void* __fastcall ForwardOrientation005dbbb0(void* sys, void*, void* out) { std::memcpy(out, at(sys, 0x80), 12); return out; }
// 005D6490 CExoSound::GetSoundMode: internal ? mode[depth] : 0.
std::uint32_t __fastcall SoundGetMode005d6490(void* outer, void*) {
    void* internal = rp(outer, 0);
    return internal ? rd(internal, 0x118 + rd(internal, 0x140) * 4) : 0u;
}
}

namespace kotor_sound_math {
double __cdecl Magnitude004ab100(const float*);
// 004AB130 Vector::Normalize (thiscall): m<1e-9 ([007440F0] qword) -> (1,0,0), else scale by 1/m.
void __fastcall Normalize004ab130(float* v, void*) {
    const double m = Magnitude004ab100(v);                          // 004AB131
    if (!(m >= 1e-09 || m != m)) { v[0] = 1.0f; v[1] = 0.0f; v[2] = 0.0f; return; } // 004AB136-004AB156
    const double inv = 1.0 / m;                                     // 004AB157 (FDIVR [0073D764])
    v[0] = static_cast<float>(inv * v[0]);                          // 004AB15D-004AB161
    v[1] = static_cast<float>(inv * v[1]);                          // 004AB163-004AB168
    v[2] = static_cast<float>(inv * v[2]);                          // 004AB16B-004AB16E
}
}
extern "C" void* kotor_cell_sound_007a39ec;                         // lane-sound owner (007A39EC)
namespace sound_mode29_required {
extern const volatile std::uint32_t* const word_0074c5f8;
extern const volatile std::uint32_t* const word_0074c5fc;
}
extern "C" {
__declspec(dllimport) void __stdcall AIL_set_stream_volume_pan(void*, float, float);
__declspec(dllimport) void __stdcall AIL_set_stream_reverb_levels(void*, float, float);
__declspec(dllimport) void __stdcall AIL_set_stream_volume_levels(void*, float, float);
}
namespace {
// 005DC77F-005DC817 pan side selection. Returns true for the 005DC7BB arm (1 - |c|/2).
bool front_arm(float fx, float fy, float rx, float ry) {
    if (fy > 0.0f) {                                                // status1: C0|C3 clear
        if (ry > 0.0f) return !(fx >= rx || fx != fx || rx != rx);  // 005DC79D-005DC7A6 (JP -> 819)
        return fx > -rx;                                            // 005DC7AA-005DC7B9 (C0|C3 set or NaN -> 819)
    }
    if (ry < 0.0f) return fx > rx;                                  // 005DC7FD-005DC802 then 005DC7B4
    return fx < -rx;                                                // 005DC808-005DC817 (JNP: C0 only)
}
}
namespace sound_stream_play29_required {
// 005DC560 CExoStreamingSoundSourceInternal::Update3DStream, RET 0.
void __fastcall refresh_005dc560(void* st, void*) {
    const float px = rf(st, 0x5c), py = rf(st, 0x60), pz = rf(st, 0x64);   // 005DC567-005DC586
    if (px == 0.0f && py == 0.0f && pz == 0.0f) return;              // 005DC578-005DC5B1
    if (rd(st, 0x58) == 0) return;                                   // 005DC5B7-005DC5BC
    if (!(rf(st, 0x54) > 0.0f)) return;                              // 005DC5C2-005DC5D0
    if (!(rf(st, 0x4c) > 0.0f)) return;                              // 005DC5D6-005DC5E4
    if (rf(st, 0x48) >= rf(st, 0x4c) || rf(st, 0x48) != rf(st, 0x48) || rf(st, 0x4c) != rf(st, 0x4c)) return; // 005DC5EA-005DC5F5
    const U mode = kotor_sound_internal::SoundGetMode005d6490(kotor_cell_sound_007a39ec, nullptr); // 005DC5FB-005DC601
    if (mode == *sound_mode29_required::word_0074c5f8 ||
        kotor_sound_internal::SoundGetMode005d6490(kotor_cell_sound_007a39ec, nullptr) == *sound_mode29_required::word_0074c5fc) { // 005DC606-005DC623
        AIL_set_stream_volume_levels(rp(st, 0), 0.0f, 0.0f);         // 005DC913-005DC91A
        return;
    }
    float vol = rf(st, 0x54);                                        // 005DC629
    void* sys = rp(st, 0x20);
    float listener[3], forward[3], rel[3] = {0.0f, 0.0f, 0.0f};
    std::memcpy(listener, at(sys, 0x98), 12);                        // 005DC663-005DC67E
    kotor_sound_internal::ForwardOrientation005dbbb0(sys, nullptr, forward); // 005DC679-005DC694
    float d[3] = {static_cast<float>(static_cast<double>(px) - listener[0]),
                  static_cast<float>(static_cast<double>(py) - listener[1]),
                  static_cast<float>(static_cast<double>(pz) - listener[2])}; // 005DC697-005DC6D6
    const double m = kotor_sound_math::Magnitude004ab100(d);         // 005DC6DA
    if (m > rf(st, 0x48))                                            // 005DC6DF-005DC6EA (FCOM; C0|C3 or NaN skip)
        vol = static_cast<float>((static_cast<double>(rf(st, 0x4c)) - m) / (static_cast<double>(rf(st, 0x4c)) - rf(st, 0x48)) * vol); // 005DC6EC-005DC6FD
    rel[0] = static_cast<float>(static_cast<double>(rf(st, 0x5c)) - listener[0]); // 005DC701-005DC72B
    rel[1] = static_cast<float>(static_cast<double>(rf(st, 0x60)) - listener[1]); // 005DC727-005DC743
    rel[2] = static_cast<float>(static_cast<double>(rf(st, 0x64)) - listener[2]); // 005DC737-005DC753
    kotor_sound_math::Normalize004ab130(rel, nullptr);               // 005DC757
    kotor_sound_math::Normalize004ab130(forward, nullptr);           // 005DC760
    const float c = static_cast<float>(static_cast<double>(forward[1]) * rel[1] + static_cast<double>(rel[0]) * forward[0]); // 005DC765-005DC77B
    float pan = front_arm(forward[0], forward[1], rel[0], rel[1])
        ? static_cast<float>(1.0 - std::fabs(static_cast<double>(c)) * 0.5)   // 005DC7BB-005DC7CD
        : static_cast<float>(std::fabs(static_cast<double>(c)) * 0.5);        // 005DC819-005DC825
    if (0.05000000074505806f > pan) pan = 0.05000000074505806f;      // 005DC7D1-005DC7E0 / 005DC827 ([007415BC])
    else if (!(0.949999988079071f >= pan || pan != pan)) pan = 0.949999988079071f; // 005DC7E2-005DC7F3 ([00745E50])
    if (c < 0.0f) vol = static_cast<float>((static_cast<double>(c) * 0.125 + 1.0) * vol); // 005DC82F-005DC854 ([0073F400]=0.125)
    float v = static_cast<float>(x87_pow_006fc1f0((1.0 - rf(st, 0x80)) * vol, 0.6000000238418579)); // 005DC858-005DC873
    if (0.0f > v) v = 0.0f;                                          // 005DC877-005DC886 / 005DC8A3
    else if (!(1.0f >= v || v != v)) v = 1.0f;                      // 005DC888-005DC899
    AIL_set_stream_volume_pan(rp(st, 0), v, pan);                    // 005DC8AB-005DC8B8
    if (rd(st, 0x6c) == 0) return;                                   // 005DC8BE-005DC8C3
    if (rd(st, 0x68) && rd(rp(st, 0x20), 0) != 0)                    // 005DC8C5-005DC8D2
        AIL_set_stream_reverb_levels(rp(st, 0), 1.0f, rf(rp(st, 0x20), 0x68)); // 005DC8D4-005DC8E3
    else
        AIL_set_stream_reverb_levels(rp(st, 0), 1.0f, 0.0f);         // 005DC8F6-005DC900
    wr(st, 0x6c, 0);                                                 // 005DC8EA / 005DC907
}
}
namespace sound_volume29_required {
// Same body under the volume donor's carrier name.
void __fastcall refresh_005dc560(void* st, void*) { sound_stream_play29_required::refresh_005dc560(st, nullptr); }
}
