// Lane-sound batch 1: CExoSoundInternal list maintenance, source/stream small
// bodies, sound-type helpers, and the vector math leaves they call.
// Receivers: sys = CExoSoundInternal (0x14C), src = CExoSoundSourceInternal,
// st = CExoStreamingSoundSourceInternal (0x84). Offsets are observed.
#include "3a10bcdf21-sound_internal.h"
#include <cmath>
#include <cstring>

namespace add_key_lower { void __fastcall Remove005e9c10(kotor_list::Internal*, void*, void*); } // remove_position.obj
namespace sound_mode29_required {
extern const volatile std::uint32_t* const word_0074c5f4;
extern const volatile std::uint32_t* const word_0074c5fc;
}
namespace kotor_resource_required { std::uint32_t __fastcall res_release_00409b80(void*, void*); } // CRes::Release (resman lane)
extern "C" {
__declspec(dllimport) void __stdcall AIL_pause_stream(void*, std::int32_t);
__declspec(dllimport) std::uint32_t __stdcall AIL_3D_sample_offset(void*);
__declspec(dllimport) std::int32_t __stdcall AIL_sample_position(void*);
}

namespace sound_stream_play29_required { void __fastcall close_existing_005dc380(void*, void*); }
namespace kotor_sound_path_required { void __fastcall interrupt_stream_005d80a0(void*, void*, std::uint32_t); }
namespace {
using U = std::uint32_t;
unsigned char* at(void* p, unsigned off) { return static_cast<unsigned char*>(p) + off; }
U rd(void* p, unsigned off) { U v; std::memcpy(&v, at(p, off), 4); return v; }
void wr(void* p, unsigned off, U v) { std::memcpy(at(p, off), &v, 4); }
void* rp(void* p, unsigned off) { void* v; std::memcpy(&v, at(p, off), 4); return v; }
void wp(void* p, unsigned off, void* v) { std::memcpy(at(p, off), &v, 4); }
float rf(void* p, unsigned off) { float v; std::memcpy(&v, at(p, off), 4); return v; }
unsigned char rb(void* p, unsigned off) { return *at(p, off); }
void wb(void* p, unsigned off, unsigned char v) { *at(p, off) = v; }
U pbits(const void* p) { U v; std::memcpy(&v, &p, 4); return v; }
void* bp(U v) { void* p; std::memcpy(&p, &v, 4); return p; }
kotor_list::Internal* L(void* sys, unsigned off) { return static_cast<kotor_list::Internal*>(rp(sys, off)); }
void* next(kotor_list::Internal* l, void** pos) { return bp(unresolved_list_next_005e9b90(l, nullptr, pos)); }
// Priority group record: [sys+4C] + index*24 (005D6080 shape).
unsigned char* group(void* sys, unsigned char index) { return static_cast<unsigned char*>(bp(rd(sys, 0x4c) + static_cast<U>(index) * 24u)); }
}

namespace kotor_sound_math {
// 004AB100 vector_magnitude(const Vector*), cdecl, result ST0 = sqrt(x*x+y*y+z*z).
double __cdecl Magnitude004ab100(const float* v) {
    const double x = v[0], y = v[1], z = v[2];                       // 004AB104-004AB10A
    return std::sqrt(x * x + y * y + z * z);                         // 004AB10C-004AB11C
}
// 004AA980 operator==(const Vector&, const Vector&), cdecl: each component FCOMP, TEST AH,44h, JP.
// [M47 compose] 004AA980 vector ==: single body = existing provider _aurora_vector_equal_004aa980 (gui-writers vector_equal.cpp, M44) (definition removed; declaration kept)
U __cdecl Equal004aa980(const float* a, const float* b);
// 006054E0 return_true_4: EAX=1, RET 4 (shared leaf).
}
namespace sound_volume29_required {
std::uint32_t __fastcall predicate_006054e0(void*, void*, void*) { return 1; } // 006054E0 return_true_4 (shared leaf), RET 4
}

namespace kotor_sound_path_required {
// 005D78F0 RemoveFromPlayingList(src), RET 4, EAX = next position.
std::uint32_t __fastcall remove_playing_005d78f0(void* sys, void*, void* src) {
    kotor_list::Internal* playing = L(sys, 0x40);
    void* pos = rp(playing, 0);                                      // 005D78F5-005D78FA
    void* data = pos ? rp(pos, 8) : nullptr;                         // 005D78FE-005D7904
    while (pos) {                                                    // 005D7907 / 005D7925
        if (data == src) {                                           // 005D7910
            if (pos == rp(sys, 0xa4)) {                              // 005D792B-005D793A
                void* cursor = rp(sys, 0xa4);
                (void)unresolved_list_next_005e9b90(L(sys, 0x40), nullptr, &cursor); // 005D793C-005D7940 (writes [A4])
                wp(sys, 0xa4, cursor);
                if (!rp(sys, 0xa4)) {                                // 005D7945
                    wp(sys, 0xa4, rp(L(sys, 0x44), 0));              // 005D794A-005D7953
                    wr(sys, 0xa8, 2);                                // 005D7955
                    if (!rp(sys, 0xa4)) { wr(sys, 0xa8, 1); wp(sys, 0xa4, nullptr); } // 005D795F-005D796B
                }
            }
            break;
        }
        data = next(L(sys, 0x40), &pos);                             // 005D7914-005D791C
    }
    pos = rp(src, 0x40);                                             // 005D7972-005D797D
    (void)next(L(sys, 0x40), &pos);                                  // 005D7981
    add_key_lower::Remove005e9c10(L(sys, 0x40), nullptr, rp(src, 0x40)); // 005D7986-005D798D
    const unsigned char index = rb(src, 0x35);                       // 005D7992
    wb(src, 0x34, 0); wr(src, 0x40, 0);                              // 005D7996-005D799A
    unsigned char* g = group(sys, index);                            // 005D79A1-005D79A7
    g[5] = static_cast<unsigned char>(g[5] - 1);                     // 005D79AA-005D79B0
    return pbits(pos);                                               // 005D79B3
}
// 005D79C0 RemoveFromNonPlayingLoopingList(src) [+38] / 005D7A70 RemoveFromDelayedOneShotList(src) [+34], RET 4.
static std::uint32_t remove_by_position(void* sys, unsigned list, void* src) {
    void* pos = rp(src, 0x40);
    (void)next(L(sys, list), &pos);
    add_key_lower::Remove005e9c10(L(sys, list), nullptr, rp(src, 0x40));
    wb(src, 0x34, 0); wr(src, 0x40, 0);
    return pbits(pos);
}
std::uint32_t __fastcall remove_nonplaying_looping_005d79c0(void* sys, void*, void* src) { return remove_by_position(sys, 0x38, src); }
std::uint32_t __fastcall remove_delayed_005d7a70(void* sys, void*, void* src) { return remove_by_position(sys, 0x34, src); }
// 005D7A00 RemoveFromNonPlayingStreamingLoopingList(st), RET 4: search [+3C], 0 if absent.
std::uint32_t __fastcall remove_nonplaying_streaming_005d7a00(void* sys, void*, void* st) {
    void* pos = rp(L(sys, 0x3c), 0);                                 // 005D7A04-005D7A09
    void* data = pos ? rp(pos, 8) : nullptr;
    while (pos) {
        if (data == st) {                                            // 005D7A20
            void* found = pos;                                       // 005D7A41
            (void)next(L(sys, 0x3c), &pos);                          // 005D7A43-005D7A4B
            add_key_lower::Remove005e9c10(L(sys, 0x3c), nullptr, found); // 005D7A50-005D7A54
            return pbits(pos);                                       // 005D7A59
        }
        data = next(L(sys, 0x3c), &pos);                             // 005D7A24-005D7A2C
    }
    return 0;                                                        // 005D7A3A
}
// 005D7AB0 RemoveFromStreamingList(st), RET 4.
std::uint32_t __fastcall remove_streaming_005d7ab0(void* sys, void*, void* st) {
    void* pos = rp(st, 0x1c);                                        // 005D7AB5
    if (!pos) return 0;                                              // 005D7AC1
    if (pos == rp(sys, 0xa4)) {                                      // 005D7AC3-005D7AD2
        void* cursor = rp(sys, 0xa4);
        (void)unresolved_list_next_005e9b90(L(sys, 0x44), nullptr, &cursor); // 005D7AD4-005D7AD8
        wp(sys, 0xa4, cursor);
        if (!rp(sys, 0xa4)) {                                        // 005D7ADD
            wp(sys, 0xa4, rp(L(sys, 0x40), 0));                      // 005D7AE2-005D7AEE
            wr(sys, 0xa8, 1);                                        // 005D7AF0
            if (!rp(sys, 0xa4)) { wr(sys, 0xa8, 1); wp(sys, 0xa4, nullptr); } // 005D7AF8-005D7AFE
        }
    }
    (void)next(L(sys, 0x44), &pos);                                  // 005D7B04-005D7B0C
    add_key_lower::Remove005e9c10(L(sys, 0x44), nullptr, rp(st, 0x1c)); // 005D7B11-005D7B18
    const unsigned char index = rb(st, 8);                           // 005D7B1D
    wr(st, 0x1c, 0);                                                 // 005D7B21
    unsigned char* g = group(sys, index);
    g[5] = static_cast<unsigned char>(g[5] - 1);                     // 005D7B31
    return pbits(pos);                                               // 005D7B34
}
// 005D7B40 AddToPlayingList(src), RET 4.
void __fastcall add_playing_005d7b40(void* sys, void*, void* src) {
    wb(src, 0x34, 1);                                                // 005D7B48
    wp(src, 0x40, ini_addhead::AddHead005e9d70(L(sys, 0x40), nullptr, src)); // 005D7B4C-005D7B55
    unsigned char* g = group(sys, rb(src, 0x35));
    g[5] = static_cast<unsigned char>(g[5] + 1);                     // 005D7B65-005D7B6B
}
// 005D7B80 AddToNonPlayingLoopingList(src), RET 4.
void __fastcall add_nonplaying_looping_005d7b80(void* sys, void*, void* src) {
    wb(src, 0x34, 2);                                                // 005D7B85
    wp(src, 0x40, ini_addhead::AddHead005e9d70(L(sys, 0x38), nullptr, src)); // 005D7B89-005D7B92
}
// 005D80A0 InterruptPlayingStream(group byte), RET 4.
static void interrupt_playing_stream(void* sys, unsigned char group_index) {
    void* pos = rp(L(sys, 0x44), 0);
    void* st = pos ? rp(pos, 8) : nullptr;
    if (!pos) return;                                                // 005D80BB
    for (;;) {
        if (rb(st, 8) == group_index) break;                         // 005D80C2
        st = next(L(sys, 0x44), &pos);                               // 005D80C7-005D80D4
        if (!pos) return;                                            // 005D80DA
    }
    sound_stream_play29_required::close_existing_005dc380(st, nullptr); // 005D80E7
    if (rd(st, 4)) ini_addhead::AddHead005e9d70(L(sys, 0x3c), nullptr, st); // 005D80EC-005D80F7
}
}

// Stream Stop (005DC380) and the remaining stream/source small bodies.
namespace sound_stream_play29_required {
// 005DC380 CExoStreamingSoundSourceInternal::Stop, RET 0 (EAX of the tail remove).
void __fastcall close_existing_005dc380(void* st, void*) {
    if (void* h = rp(st, 0)) AIL_pause_stream(h, 1);                  // 005DC383-005DC38C
    const U attached = rd(st, 0x1c);                                 // 005DC392
    void* sys = rp(st, 0x20);                                        // 005DC397
    wr(st, 0x2c, 0); wr(st, 0x34, 0); wr(st, 0x40, 0);               // 005DC39A-005DC3A8
    if (attached) (void)kotor_sound_path_required::remove_streaming_005d7ab0(sys, nullptr, st); // 005DC3B2
    else (void)kotor_sound_path_required::remove_nonplaying_streaming_005d7a00(sys, nullptr, st); // 005DC3B9
}
// 005D7BA0 AddToNonPlayingStreamingLoopingList(st): tail JMP AddHead([+3C]).
void __fastcall attach_005d7ba0(void* sys, void*, void* st) {
    (void)ini_addhead::AddHead005e9d70(L(sys, 0x3c), nullptr, st);
}
// 005D80A0 InterruptPlayingStream (donor-declared void carrier).
void __fastcall remove_group_005d80a0(void* sys, void*, std::uint32_t group_word) {
    kotor_sound_path_required::interrupt_stream_005d80a0(sys, nullptr, group_word);
}
// 005D8470 AddToStreamingList(st), RET 4, EAX 0/1.
std::uint32_t __fastcall start_005d8470(void* sys, void*, void* st) {
    const U n2 = rb(sys, 0x28);                                      // 005D8476
    const U limit = n2 - (n2 >> 1);                                  // 005D847D-005D8481
    if (static_cast<U>(L(sys, 0x44)->count) >= limit) {              // 005D8483-005D8487 (JB -> add)
        unsigned char* mine = group(sys, rb(st, 8));                 // 005D848D-005D849B
        void* best = nullptr; unsigned char best_rank = 0;           // 005D849E-005D84A0
        if (!mine) return 0;                                         // 005D84A2-005D84A4
        void* pos = rp(L(sys, 0x44), 0);                             // 005D84AA
        void* data = pos ? rp(pos, 8) : nullptr;
        if (!pos) return 0;                                          // 005D84BD-005D84BF
        do {
            if (rd(data, 4) != 0) {                                  // 005D84C5-005D84CA
                unsigned char* other = group(sys, rb(data, 8));      // 005D84D3-005D84DD
                if (other) {
                    const unsigned char rank = other[6];             // 005D84E4
                    if (rank > mine[6] && rank > best_rank) { best = data; best_rank = rank; } // 005D84E7-005D84F2
                }
            }
            data = next(L(sys, 0x44), &pos);                         // 005D84F4-005D84FC
        } while (pos);                                               // 005D8501-005D8507
        if (!best) return 0;                                         // 005D8509-005D850B
        close_existing_005dc380(best, nullptr);                      // 005D850F
        if (rd(best, 4)) ini_addhead::AddHead005e9d70(L(sys, 0x3c), nullptr, best); // 005D8514-005D851F
    }
    wp(st, 0x1c, ini_addhead::AddHead005e9d70(L(sys, 0x44), nullptr, st)); // 005D8524-005D8531
    unsigned char* g = group(sys, rb(st, 8));
    g[5] = static_cast<unsigned char>(g[5] + 1);                     // 005D8541-005D8549
    return 1;
}
// 005DC4D0 CExoStreamingSoundSourceInternal::GetWithinRangeOfListener, RET 0.
std::uint32_t __fastcall attachment_query_005dc4d0(void* st, void*) {
    if (rd(st, 0x58) == 0) return 1;                                 // 005DC4D6-005DC4DB
    float d[3];
    for (int i = 0; i < 3; ++i)                                      // listener [sys+98] - source [st+5C], FSTP DWORD
        d[i] = static_cast<float>(static_cast<double>(rf(rp(st, 0x20), 0x98 + 4 * i)) - rf(st, 0x5c + 4 * i));
    const double m = kotor_sound_math::Magnitude004ab100(d);         // 005DC539
    const double r = rf(st, 0x4c);                                   // 005DC53E (FCOMP [st+4C])
    return (m < r || m == r) ? 1u : 0u;                              // TEST AH,41h / JP (greater or unordered -> 0)
}
}

namespace kotor_sound_path_required {
void __fastcall interrupt_stream_005d80a0(void* sys, void*, std::uint32_t group_word) {
    interrupt_playing_stream(sys, static_cast<unsigned char>(group_word)); // 005D80BE (BL = low byte)
}
// 005DB210 CExoSoundSourceInternal::ShutDownSource, RET 0.
void __fastcall shutdown_source_005db210(void* src, void*) {
    const unsigned char state = rb(src, 0x34);                       // 005DB214
    if (state == 1) (void)remove_playing_005d78f0(rp(src, 0x44), nullptr, src);         // 005DB217-005DB21F
    else if (state == 2) (void)remove_nonplaying_looping_005d79c0(rp(src, 0x44), nullptr, src); // 005DB226-005DB22E
    else if (state == 3) (void)remove_delayed_005d7a70(rp(src, 0x44), nullptr, src);    // 005DB235-005DB23D
    if (rd(rp(src, 0x44), 8) != 0) {                                 // 005DB242-005DB24C
        void* v3 = rp(src, 0x3c);
        if (v3 && rp(v3, 4)) {                                       // 005DB24E-005DB25A
            AIL_end_3D_sample(rp(v3, 4));                            // 005DB25C
            wr(rp(src, 0x3c), 0, 0); wp(src, 0x3c, nullptr);         // 005DB263-005DB268
        } else {
            void* v2 = rp(src, 0x38);
            if (v2 && rp(v2, 4)) {                                   // 005DB26D-005DB279
                AIL_end_sample(rp(v2, 4));                           // 005DB27B
                wr(rp(src, 0x38), 0, 0); wp(src, 0x38, nullptr);     // 005DB282-005DB287
            }
        }
    }
    if (rd(src, 0x30) != 0) {                                        // 005DB28A
        wr(src, 0x30, 0);                                            // 005DB292
        kotor_resource_required::res_release_00409b80(rp(src, 8), nullptr); // 005DB28F/005DB295
    }
    wb(src, 0x34, 0); wr(src, 0x40, 0);                              // 005DB29A-005DB29D
}
// 005DB380 CExoSoundSourceInternal::StoreRestartSoundPosition, RET 0.
void __fastcall store_restart_005db380(void* src, void*) {
    if (rd(rp(src, 0x44), 8) == 0) { wr(src, 0x88, 0); return; }    // 005DB383-005DB38B / 005DB3BE
    if (rd(src, 0x1c)) wr(src, 0x88, AIL_3D_sample_offset(rp(rp(src, 0x3c), 4)));  // 005DB38D-005DB3A1
    else wr(src, 0x88, static_cast<U>(AIL_sample_position(rp(rp(src, 0x38), 4))));   // 005DB3A9-005DB3B6
}
// 005DB050 CExoSoundSourceInternal::GetWithinRangeOfListener, RET 0.
std::uint32_t __fastcall source_in_range_005db050(void* src, void*) {
    if (rd(src, 0x1c) == 0) return 1;                                // 005DB056-005DB05B
    float d[3];
    for (int i = 0; i < 3; ++i)                                      // listener [sys+98] - source [src+78]
        d[i] = static_cast<float>(static_cast<double>(rf(rp(src, 0x44), 0x98 + 4 * i)) - rf(src, 0x78 + 4 * i));
    const double m = kotor_sound_math::Magnitude004ab100(d);
    const double r = rf(src, 0x6c);                                  // 005DB0BE
    return (m < r || m == r) ? 1u : 0u;
}
}

namespace kotor_sound_internal {
// 005D6B70 / 005D6BC0 GetPauseable(src|st): type byte at +35 / +8, RET 4.
U pauseable_shared(void* sys, unsigned char type) {
    const U mode = rd(sys, 0x118 + rd(sys, 0x140) * 4);              // 005D6B70-005D6B76
    if (mode == *sound_mode29_required::word_0074c5f4) {             // 2
        if (type == 1 || type == 2 || type == 4 || type == 0x0b) return 0; // 005D6B8C-005D6BB1
        return 1;
    }
    if (mode == *sound_mode29_required::word_0074c5fc) {             // 4
        if (type == 1 || type == 2 || type == 0x0b) return 0;        // 005D6BA7-005D6BB1
        return 1;
    }
    return 1;                                                        // 005D6BB8
}
U __fastcall SourcePauseable005d6b70(void* sys, void*, void* src) { return pauseable_shared(sys, rb(src, 0x35)); }
}
namespace sound_stream_play29_required {
std::uint32_t __fastcall pause_query_005d6bc0(void* sys, void*, void* st) { return kotor_sound_internal::pauseable_shared(sys, rb(st, 8)); }
}
namespace kotor_sound_internal {
// 005D6A90 / 005D6AD0 GetSliderType: 8,9,10h,11h,1Ah -> 1; 1,2 -> 3; else 2. RET 4.
U slider_shared(unsigned char type) {
    if (type == 8 || type == 9 || type == 0x10 || type == 0x11 || type == 0x1a) return 1;
    if (type == 1 || type == 2) return 3;
    return 2;
}
U __fastcall SourceSliderType005d6a90(void*, void*, void* src) { return slider_shared(rb(src, 0x35)); }
}
namespace sound_volume29_required {
std::uint32_t __fastcall group_005d6ad0(void*, void*, void* st) { return kotor_sound_internal::slider_shared(rb(st, 8)); }
}
namespace kotor_sound_internal {
// 005D6B10 GetSliderVolume(type, current), RET 8, ST0.
}
namespace sound_volume29_required {
float __fastcall gain_005d6b10(void* sys, void*, std::uint32_t type, std::uint32_t current) {
    switch (type) {                                                  // 005D6B14-005D6B1B (DEC chain)
    case 1: return current ? rf(sys, 0x74) : rf(sys, 0x64);
    case 2: return current ? rf(sys, 0x70) : rf(sys, 0x7c);
    case 3: return current ? rf(sys, 0x6c) : rf(sys, 0x78);
    default: return 1.0f;                                            // 005D6B1D ([0073D764])
    }
}
}
