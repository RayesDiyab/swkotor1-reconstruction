// W5 lane "pathfind-plan" part 3/3: path smoothing (CSWSArea::SmoothPointsOnPath 004C1670 and the restartable
// variant 004C17A0, SmoothSelection / SmoothSelectionRestartable, SmoothSelectNodes / SmoothSelectNodesRestartable,
// SmoothCornerOptimize, ComputeBestCorner). Point lists are xy float pairs; counts are float counts.
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are the original instructions.
#include "eb5e5e70a0-claude_w5_pathfind_plan.hpp"

using namespace w5;
using namespace w5pp;

namespace {
inline std::uint64_t smooth_slice() {                                               // cdq of [007A1B1C]
    return static_cast<std::uint64_t>(static_cast<std::int64_t>(w5_path_smooth_budget_007a1b1c));
}
// inlined "free buffer, zero buffer and count" pattern of 004C18A0.. / 004C1589..
inline void drop(void* pf, U count_off, U buffer_off) {
    void* p = at<void*>(pf, buffer_off);
    if (p != nullptr) { at<U>(pf, count_off) = 0; live_free(p); at<U>(pf, buffer_off) = 0; }
    at<U>(pf, buffer_off) = 0;
}
}  // namespace

extern "C" {

// 004C1670 CSWSArea::SmoothPointsOnPath: +94/+98 raw points -> +8C/+90 walk points. RET 0, EAX 1
U __fastcall kotor_fastcall_004c1670(void* area, void*) {
    if (at<void*>(pf_of(area), 0x90) != nullptr) {                                  // 004C1678..89
        live_free(at<void*>(pf_of(area), 0x90));                                    // 004C168B..8C
        at<U>(pf_of(area), 0x90) = 0; at<U>(pf_of(area), 0x8c) = 0;                 // 004C1691..A6
    }
    void* pf = pf_of(area);
    const I count = at<I>(pf, 0x94);                                                // 004C16B2 (EDI)
    const float* raw = at<const float*>(pf, 0x98);                                  // 004C16BA (EBX)
    if (count == 0) {                                                               // 004C16B8..C0
        at<U>(pf, 0x8c) = 0; at<U>(pf_of(area), 0x90) = 0;                          // 004C16C2..D0
        return 1;
    }
    if (at<U>(pf, 0x1c) == 1) {                                                     // 004C16E1..E5: copy unsmoothed
        U* copy = static_cast<U*>(live_operator_new(static_cast<U>(count) * 4));    // 004C16E7..EF
        for (I k = 0; k < count; ++k) copy[k] = reinterpret_cast<const U*>(raw)[k]; // 004C16F7..0C
        at<U*>(pf_of(area), 0x90) = copy;                                           // 004C170E..14
        at<I>(pf_of(area), 0x8c) = count;                                           // 004C171A..20
        return 1;
    }
    I selected_count = 0;                                                           // E-4
    float* selected = nullptr;                                                      // E-8
    kotor_fastcall_004c15f0(area, nullptr, count, const_cast<float*>(raw), &selected_count, &selected); // 004C1733..41
    I out_count = 0; float* out = nullptr;                                          // E-8 / E-4 (reused)
    kotor_fastcall_004bfe20(area, nullptr, selected_count, selected, &out_count, &out); // 004C1746..5C
    at<float*>(pf_of(area), 0x90) = out;                                            // 004C1761..6B
    at<I>(pf_of(area), 0x8c) = out_count;                                           // 004C1771..7C
    live_free(selected);                                                            // 004C177B..82
    return 1;
}

// 004C17A0 CSWSArea::SmoothPointsOnPathRestartable: RET 0, EAX 1 while time-sliced work remains, else 0
U __fastcall kotor_fastcall_004c17a0(void* area, void*) {
    void* pf = pf_of(area);
    const I count = at<I>(pf, 0x94);                                                // 004C17AE (EBP)
    float* raw = at<float*>(pf, 0x98);                                              // 004C17B4 (EBX, E-4)
    if (count == 0) {                                                               // 004C17BD..C3
        at<U>(pf, 0x8c) = 0; at<U>(pf_of(area), 0x90) = 0;                          // 004C17C5..D1
        return 0;
    }
    if (at<U>(pf, 0x214) == k_smooth_start_0074c444) {                              // 004C17E1..ED
        if (at<void*>(pf, 0x90) != nullptr) {                                       // 004C17F3..FB
            live_free(at<void*>(pf, 0x90));                                         // 004C17FD..FE
            at<U>(pf_of(area), 0x90) = 0; at<U>(pf_of(area), 0x8c) = 0;             // 004C1803..18
        }
        if (at<U>(pf_of(area), 0x1c) == 1) {                                        // 004C181E..28: copy unsmoothed
            U* copy = static_cast<U*>(live_operator_new(static_cast<U>(count) * 4)); // 004C182A..32
            for (I k = 0; k < count; ++k) copy[k] = reinterpret_cast<const U*>(raw)[k]; // 004C183A..4F
            at<U*>(pf_of(area), 0x90) = copy;                                       // 004C1851..57
            at<I>(pf_of(area), 0x8c) = count;                                       // 004C185D..65
            return 0;
        }
        at<I>(pf_of(area), 0x234) = -1;                                             // 004C1873
        at<U>(pf_of(area), 0x230) = 0;                                              // 004C187D..83
        at<U>(pf_of(area), 0x214) = k_smooth_pass1_0074c448;                        // 004C1889..94
        drop(pf_of(area), 0x218, 0x21c);                                            // 004C189A..BF
        drop(pf_of(area), 0x220, 0x228);                                            // 004C18C5..EA
        drop(pf_of(area), 0x224, 0x22c);                                            // 004C18F0..15
    }
    I out_count = at<I>(pf_of(area), 0x224);                                        // 004C191F..31 E-8
    float* out = at<float*>(pf_of(area), 0x22c);                                    // E-C
    float* corners = nullptr;                                                       // E-4
    const U pending = kotor_fastcall_004c13e0(area, nullptr, count, raw, &out_count, &out); // 004C1935..4B
    at<I>(pf_of(area), 0x224) = out_count;                                          // 004C1950..60
    kotor_fastcall_004bb520(pf_of(area), nullptr, out);                             // 004C1966..6D
    if (pending == 1) return pending;                                               // 004C1972..80
    I corner_count = out_count;                                                     // E-8 (in/out)
    kotor_fastcall_004bfe20(area, nullptr, out_count, out, &corner_count, &corners); // 004C1981..93
    at<float*>(pf_of(area), 0x90) = corners;                                        // 004C1998..A2
    at<I>(pf_of(area), 0x8c) = corner_count;                                        // 004C19A8..B2
    at<U>(pf_of(area), 0x224) = 0;                                                  // 004C19B8..BE
    drop(pf_of(area), 0x224, 0x22c);                                                // 004C19C4..E9
    return 0;
}

// 004C15F0 CSWSArea::SmoothSelection(count, points, out_count, out_points): RET 10h. Three SmoothSelectNodes passes
// (subdivide/look-ahead 1/0, 1/1, 0/1); the two intermediate buffers are freed.
void __fastcall kotor_fastcall_004c15f0(void* area, void*, I count, float* points, I* out_count, float** out_points) {
    kotor_fastcall_004c0180(area, nullptr, count, points, &count, &points, 1, 0);   // 004C15F0..0F (arg slots reused)
    float* first = points;                                                          // 004C1614 (EDI)
    kotor_fastcall_004c0180(area, nullptr, count, first, &count, &points, 1, 1);    // 004C1618..2E
    float* second = points;                                                         // 004C163B (EBX)
    kotor_fastcall_004c0180(area, nullptr, count, second, out_count, out_points, 0, 1); // 004C1633..4D
    live_free(first);                                                               // 004C1652..53
    live_free(second);                                                              // 004C1658..59
}

// 004C13E0 CSWSArea::SmoothSelectionRestartable(count, points, out_count, out_points): RET 10h, EAX 1 = suspended.
// Passes keyed by +214 (2, 3, 4) with intermediate buffers +218/+21C and +220/+228.
U __fastcall kotor_fastcall_004c13e0(void* area, void*, I count, float* points, I* out_count, float** out_points) {
    void* pf = pf_of(area);
    I count1 = at<I>(pf, 0x218);                                                    // 004C13F4 E-C
    float* first = at<float*>(pf, 0x21c);                                           // 004C1407 E-10 (EDI)
    I count2 = at<I>(pf, 0x220);                                                    // 004C13FA E-4 (EBP)
    float* second = at<float*>(pf, 0x228);                                          // 004C1400 E-8 (EBX)
    if (at<U>(pf, 0x214) == k_smooth_pass1_0074c448) {                              // 004C140D..25
        const U pending = kotor_fastcall_004c04e0(area, nullptr, count, points, &count1, &first, 1, 0, 5.0f); // 004C142B..4A
        kotor_fastcall_004bb480(pf_of(area), nullptr, first);                       // 004C144F..5E
        at<I>(pf_of(area), 0x218) = count1;                                         // 004C1463..6D
        if (pending == 1) return 1;                                                 // 004C1473..7E
        at<I>(pf_of(area), 0x234) = -1;                                             // 004C1484..8A
        at<U>(pf_of(area), 0x230) = 0;                                              // 004C1494..9A
        at<U>(pf_of(area), 0x214) = k_smooth_pass2_0074c44c;                        // 004C14A4..B0
    }
    if (at<U>(pf_of(area), 0x214) == k_smooth_pass2_0074c44c) {                     // 004C14B6..C8
        const U pending = kotor_fastcall_004c04e0(area, nullptr, count1, first, &count2, &second, 1, 1, 1.1f); // 004C14CA..E1
        kotor_fastcall_004bb4d0(pf_of(area), nullptr, second);                      // 004C14E6..F3
        at<I>(pf_of(area), 0x220) = count2;                                         // 004C14FB..05
        if (pending == 1) return 1;                                                 // 004C14F8 / 0B
        at<I>(pf_of(area), 0x234) = -1;                                             // 004C150D..13
        at<U>(pf_of(area), 0x230) = 0;                                              // 004C151D..23
        at<U>(pf_of(area), 0x214) = k_smooth_pass3_0074c450;                        // 004C152D..39
    }
    if (at<U>(pf_of(area), 0x214) == k_smooth_pass3_0074c450) {                     // 004C153F..51
        if (kotor_fastcall_004c04e0(area, nullptr, count2, second, out_count, out_points, 0, 1, 1.1f) == 1) return 1; // 004C1553..72
    }
    drop(pf_of(area), 0x218, 0x21c);                                                // 004C1583..AA
    drop(pf_of(area), 0x220, 0x228);                                                // 004C15B0..D5
    return 0;
}

// 004C0180 CSWSArea::SmoothSelectNodes(count, points, out_count, out_points, subdivide, look_ahead): RET 18h.
// Greedy line-of-sight node selection; with subdivide, long legs are re-sampled at 1-unit spacing.
void __fastcall kotor_fastcall_004c0180(void* area, void*, I count, const float* points, I* out_count, float** out_points,
                                        I subdivide, I look_ahead) {
    void* pf = pf_of(area);
    const float radius = at<float>(pf, 4);                                          // 004C0189..94 E-38
    const float height = at<float>(pf, 0x14);                                       // 004C019C..A5 E-3C
    const I extra = ftol2(kotor_fastcall_004bb860(area, nullptr, count, points));   // 004C019F..AE
    I sub_count = -1;                                                               // E-54
    float* sub = nullptr;                                                           // E-50 (ESI)
    *out_count = 0;                                                                 // 004C01CC
    float* out = static_cast<float*>(live_operator_new(static_cast<U>(count + extra * 2) << 2)); // 004C01B3..CE
    *out_points = out;                                                              // 004C01DB
    reinterpret_cast<U*>(*out_points)[*out_count] = reinterpret_cast<const U*>(points)[0];      // 004C01DE..E2
    reinterpret_cast<U*>(*out_points)[*out_count + 1] = reinterpret_cast<const U*>(points)[1];  // 004C01E5..F1
    *out_count += 2;                                                                // 004C01F5..06
    I i = 0;                                                                        // EBX
    const I last = (count - 1) >> 1;                                                // 004C0203..08 E-30
    while (i < last || sub_count > 0) {                                             // 004C0210..1A
        I advance = 0;                                                              // the out_count argument slot
        if (look_ahead == 1) {                                                      // 004C0220..33
            I k = (count >> 1) - i - 1;                                             // 004C0239..41
            while (k >= 1) {                                                        // 004C0242..44 / B3..B6
                const Vector to = {points[(k + i) * 2], points[(k + i) * 2 + 1], 0.0f};  // 004C0250..74 E-24
                const Vector from = {(*out_points)[*out_count - 2], (*out_points)[*out_count - 1], 0.0f}; // E-18
                if (test_line(area, &from, &to, &radius, height, 1, nullptr) == 1) { // 004C0272..AA
                    advance += k;                                                   // 004C02AC
                    k = -1;                                                         // 004C02B0..B2
                } else if (k <= 4) {                                                // 004C02F8..FB
                    --k;
                } else {
                    k >>= 1;                                                        // 004C02FD (sar)
                }
            }
        } else if (!(sub_count > 0)) {                                              // 004C0301..05
            advance = 1;
        }
        i += advance;                                                               // 004C02C8
        float px = points[i * 2], py = points[i * 2 + 1];                           // 004C02CC..D7 (arg slots)
        if (advance != 0) {                                                         // 004C02DB
            if (sub != nullptr) { live_free(sub); sub = nullptr; }                  // 004C031E..34
            sub_count = 0;                                                          // 004C0337
        } else if (sub_count > 0) {                                                 // 004C02DD..DF
            --sub_count;                                                            // 004C02E5
            px = sub[sub_count * 2];                                                // 004C02E1..E6
            py = sub[sub_count * 2 + 1];                                            // 004C02EE..F2
        } else {
            px = points[i * 2 + 2];                                                 // 004C030B..10
            ++i;
            py = points[i * 2 + 1];                                                 // 004C0314..18
        }
        if (subdivide != 0) {                                                       // 004C033F..45
            const float* o = *out_points;
            const I n = *out_count;
            const Vector leg = {static_cast<float>(static_cast<double>(o[n - 2]) - px),
                                static_cast<float>(static_cast<double>(o[n - 1]) - py), 0.0f}; // 004C034B..71
            const double length = kotor_cdecl_004ab100(&leg);                       // 004C0375
            const float length_f = static_cast<float>(length);                      // 004C037A fst E-4C
            if (length >= 1.1) {                                                    // 004C0385..90 (qword 1.1)
                const float fx = o[n - 2], fy = o[n - 1];                           // 004C039D..A9 E-44/E-40
                if (sub != nullptr) live_free(sub);                                 // 004C03AD..B5
                sub = static_cast<float*>(live_operator_new(static_cast<U>(ftol2(length_f)) * 8 + 8)); // 004C03B8..D8
                sub_count = 0;                                                      // 004C03E9
                if (length > 0.0) {                                                 // 004C03CE..F4 (test ah,41h)
                    const double inverse = 1.0f / length_f;                         // 004C03FA..04
                    const float ux = static_cast<float>((static_cast<double>(px) - fx) * inverse); // 004C040A..14 E-4C
                    const double uy = (static_cast<double>(py) - fy) * inverse;     // 004C0418..20 (ST)
                    double t = length;                                              // 004C0422 fld qword
                    float tf = length_f;                                            // E-48
                    float* w = sub;
                    do {                                                            // 004C0430..6D
                        w[0] = static_cast<float>(static_cast<double>(ux) * tf + fx);
                        ++sub_count;
                        w[1] = static_cast<float>(uy * tf + fy);
                        w += 2;
                        t = t - 1.0;
                        tf = static_cast<float>(t);
                    } while (t > 0.0);
                    if (sub_count > 0) {                                            // 004C0475..7D
                        px = sub[(sub_count - 1) * 2];                              // 004C047F
                        --sub_count;
                        py = sub[sub_count * 2 + 1];                                // 004C048C
                    }
                }
            }
        }
        (*out_points)[*out_count] = px;                                             // 004C049A..A3
        (*out_points)[*out_count + 1] = py;                                         // 004C04A6..AF
        *out_count += 2;                                                            // 004C04B3..B8
    }
    if (sub != nullptr) live_free(sub);                                             // 004C04C3..C8
}

// 004C04E0 CSWSArea::SmoothSelectNodesRestartable(count, points, out_count, out_points, subdivide, look_ahead,
// min_length): RET 1Ch, EAX 1 = suspended (state in +230 index, +234 subdivision count, +238 subdivision buffer)
U __fastcall kotor_fastcall_004c04e0(void* area, void*, I count, const float* points, I* out_count, float** out_points,
                                     I subdivide, I look_ahead, float min_length) {
    const std::uint64_t t0 = hr_timer();                                            // 004C04E3..11 E-2C/E-28
    void* pf = pf_of(area);                                                         // 004C04FA
    const float radius = static_cast<float>(static_cast<double>(at<float>(pf, 4)) + 0.001f); // 004C0500..19 E-34
    const float height = at<float>(pf, 0x14);                                       // 004C051D..21 E-38
    const I extra = ftol2(kotor_fastcall_004bb860(area, nullptr, count, points));   // 004C0520..2D
    float* sub = at<float*>(pf, 0x238);                                             // 004C0532 E-50
    I sub_count = at<I>(pf, 0x234);                                                 // 004C0538 E-54
    I i = at<I>(pf, 0x230);                                                         // 004C0542 (EBP)
    if (*out_points == nullptr) {                                                   // 004C0550..5A
        *out_count = 0;                                                             // 004C0567
        float* out = static_cast<float*>(live_operator_new(static_cast<U>(count + extra * 2) << 2)); // 004C055C..69
        *out_points = out;                                                          // 004C0572
        reinterpret_cast<U*>(*out_points)[*out_count] = reinterpret_cast<const U*>(points)[i * 2];         // 004C0574..79
        reinterpret_cast<U*>(*out_points)[*out_count + 1] = reinterpret_cast<const U*>(points)[i * 2 + 1]; // 004C057C..84
        *out_count += 2;                                                            // 004C0588..94
    }
    const I last = (count - 1) >> 1;                                                // 004C059E..A5 E-30
    while (i < last || sub_count > 0) {                                             // 004C05B0..B6
        I advance = 0;                                                              // the out_points argument slot
        if (look_ahead == 1) {                                                      // 004C05BC..C9
            I k = (count >> 1) - i - 1;                                             // 004C05CF..D7
            while (k >= 1) {                                                        // 004C05D8..DA / 4C0653..5A
                const Vector from = {(*out_points)[*out_count - 2], (*out_points)[*out_count - 1], 0.0f}; // E-18
                const Vector to = {points[(k + i) * 2], points[(k + i) * 2 + 1], 0.0f};  // E-24
                const I r = test_line(area, &from, &to, &radius, height, 1, nullptr); // 004C05E8..34
                w5_path_smooth_stat_00832870 = 0;                                   // 004C063E
                w5_path_smooth_stat_00832840 = 0;                                   // 004C0644
                if (r == 1) {                                                       // 004C063B..4A
                    advance += k;                                                   // 004C064C
                    k = -1;                                                         // 004C0650..52
                } else if (k <= 4) {                                                // 004C069A..9D
                    --k;
                } else {
                    k >>= 1;                                                        // 004C069F
                }
            }
        } else if (!(sub_count > 0)) {                                              // 004C06A3..A7
            advance = 1;
        }
        i += advance;                                                               // 004C0666
        float px = points[i * 2], py = points[i * 2 + 1];                           // 004C066A..75 (arg4 / arg3 slots)
        if (advance != 0) {                                                         // 004C0679
            if (sub != nullptr) {                                                   // 004C06C0..C6
                live_free(sub);                                                     // 004C06C8..C9
                sub = nullptr;
                at<U>(pf_of(area), 0x238) = 0;                                      // 004C06CE..DF
            }
            sub_count = 0;                                                          // 004C06E5
        } else if (sub_count > 0) {                                                 // 004C067B..7D
            px = sub[(sub_count - 1) * 2];                                          // 004C067F..83
            --sub_count;                                                            // 004C0687
            py = sub[sub_count * 2 + 1];                                            // 004C0690
        } else {
            px = points[i * 2 + 2];                                                 // 004C06AD
            ++i;
            py = points[i * 2 + 1];                                                 // 004C06B6
        }
        if (subdivide != 0) {                                                       // 004C06E9..ED
            const float* o = *out_points;
            const I n = *out_count;
            const Vector leg = {static_cast<float>(static_cast<double>(o[n - 2]) - px),
                                static_cast<float>(static_cast<double>(o[n - 1]) - py), 0.0f}; // 004C06F3..18
            const double length = kotor_cdecl_004ab100(&leg);                       // 004C071C
            const float length_f = static_cast<float>(length);                      // 004C0721 fst E-48
            if (length >= min_length) {                                             // 004C0728..34 (test ah,1)
                const float fx = o[n - 2], fy = o[n - 1];                           // 004C073A..50 E-40/E-3C
                if (sub != nullptr) {                                               // 004C074A..54
                    live_free(sub);                                                 // 004C0756..57
                    at<U>(pf_of(area), 0x238) = 0;                                  // 004C075C..69
                }
                sub = static_cast<float*>(live_operator_new(static_cast<U>(ftol2(length_f)) * 8 + 8)); // 004C076F..80
                at<float*>(pf_of(area), 0x238) = sub;                               // 004C07A5
                sub_count = 0;                                                      // 004C07B7
                double t = length_f;                                                // 004C0785 fld dword
                if (t > 0.0) {                                                      // 004C0789..BB (test ah,41h)
                    const double inverse = 1.0f / length_f;                         // 004C07C1..C7
                    const float ux = static_cast<float>((static_cast<double>(px) - fx) * inverse); // 004C07CB..D5 E-48
                    const float uy = static_cast<float>((static_cast<double>(py) - fy) * inverse); // 004C07D9..E3 E-54
                    float tf = length_f;                                            // E-44
                    float* w = sub;
                    do {                                                            // 004C07F0..27
                        w[0] = static_cast<float>(static_cast<double>(ux) * tf + fx);
                        ++sub_count;
                        w[1] = static_cast<float>(static_cast<double>(uy) * tf + fy);
                        w += 2;
                        t = t - 1.0;
                        tf = static_cast<float>(t);
                    } while (t > 0.0);
                    if (sub_count > 0) {                                            // 004C0829..31
                        px = sub[(sub_count - 1) * 2];                              // 004C0839
                        --sub_count;
                        py = sub[sub_count * 2 + 1];                                // 004C0846
                    }
                }
            }
        }
        (*out_points)[*out_count] = px;                                             // 004C0852..5A
        (*out_points)[*out_count + 1] = py;                                         // 004C085D..65
        *out_count += 2;                                                            // 004C0869..6E
        if (hr_timer() - t0 > smooth_slice()) {                                     // 004C0870..9A: suspend
            at<I>(pf_of(area), 0x234) = sub_count;                                  // 004C08FE..0D
            at<I>(pf_of(area), 0x230) = i;                                          // 004C0913..1A
            return 1;
        }
    }
    if (sub != nullptr) { live_free(sub); at<U>(pf_of(area), 0x238) = 0; }          // 004C08AF..CA
    at<I>(pf_of(area), 0x234) = -1;                                                 // 004C08D6..DC
    at<U>(pf_of(area), 0x230) = 0;                                                  // 004C08E6..ED
    return 0;
}

// 004BFE20 CSWSArea::SmoothCornerOptimize(count, points, out_count, out_points): RET 10h. Cuts each interior corner
// with up to three ComputeBestCorner refinements.
void __fastcall kotor_fastcall_004bfe20(void* area, void*, I count, const float* points, I* out_count, float** out_points) {
    const float radius = at<float>(pf_of(area), 4);                                 // 004BFE36 E-28
    const float height = at<float>(pf_of(area), 0x14);                              // 004BFE29 E-2C
    *out_points = static_cast<float*>(live_operator_new(static_cast<U>(count) << 4)); // 004BFE3A..5A
    *out_count = 0;                                                                 // 004BFE5C
    reinterpret_cast<U*>(*out_points)[0] = reinterpret_cast<const U*>(points)[0];   // 004BFE62..66
    reinterpret_cast<U*>(*out_points)[*out_count + 1] = reinterpret_cast<const U*>(points)[1]; // 004BFE68..6F
    *out_count += 2;                                                                // 004BFE73..85
    auto push = [&](float x, float y) {                                             // buf[n] = x; buf[n+1] = y; n += 2
        (*out_points)[*out_count] = x;
        (*out_points)[*out_count + 1] = y;
        *out_count += 2;
    };
    I i = 2;                                                                        // EBX
    if (count - 2 > 2) {                                                            // 004BFE7D..87
        for (; i < count - 2; i += 2) {                                             // 004C011B..3E
            const float px = (*out_points)[*out_count - 2], py = (*out_points)[*out_count - 1];
            const float cx = points[i], cy = points[i + 1];
            const float nx = points[i + 2], ny = points[i + 3];
            float ax, ay, bx, by;                                                   // E-18 E-14 E-34 E-30
            if (kotor_fastcall_004be3a0(area, nullptr, px, py, cx, cy, nx, ny, radius, height, &ax, &ay, &bx, &by) != 1) { // 004BFE97..E9
                reinterpret_cast<U*>(*out_points)[*out_count] = reinterpret_cast<const U*>(points)[i];         // 004C00FD..08
                reinterpret_cast<U*>(*out_points)[*out_count + 1] = reinterpret_cast<const U*>(points)[i + 1]; // 004C010B..17
                *out_count += 2;                                                    // 004C011F..3A
                continue;
            }
            float cx2, cy2, dx2, dy2;                                               // E-20 E-1C arg3-slot E-38
            const float qx = (*out_points)[*out_count - 2], qy = (*out_points)[*out_count - 1];
            if (kotor_fastcall_004be3a0(area, nullptr, qx, qy, ax, ay, bx, by, radius, height, &cx2, &cy2, &dx2, &dy2) == 1) { // 004BFEEF..3B
                float ex, ey, fx, fy;                                               // E-10 E-C E-8 E-4
                if (kotor_fastcall_004be3a0(area, nullptr, dx2, dy2, bx, by, nx, ny, radius, height, &ex, &ey, &fx, &fy) == 1) { // 004BFF41..8E
                    push(cx2, cy2); push(dx2, dy2); push(ex, ey); push(fx, fy);     // 004BFF90..FF
                } else {
                    push(cx2, cy2); push(dx2, dy2); push(bx, by);                   // 004C0000..00FB
                }
            } else if (kotor_fastcall_004be3a0(area, nullptr, ax, ay, bx, by, nx, ny, radius, height, &cx2, &cy2, &dx2, &dy2) == 1) { // 004C002E..7B
                push(ax, ay); push(cx2, cy2); push(dx2, dy2);                       // 004C007D..CA
            } else {
                push(ax, ay); push(bx, by);                                         // 004C00CC..FB
            }
        }
    }
    if (count > 2) {                                                                // 004C014C..4F
        reinterpret_cast<U*>(*out_points)[*out_count] = reinterpret_cast<const U*>(points)[i];         // 004C0151..56
        reinterpret_cast<U*>(*out_points)[*out_count + 1] = reinterpret_cast<const U*>(points)[i + 1]; // 004C0159..61
        *out_count += 2;                                                            // 004C0165
    }
}

// 004BE3A0 CSWSArea::ComputeBestCorner(p0, p1 (corner), p2, radius, height, &a, &b): RET 30h, EAX 1 when a cut
// a (on p1->p0) .. b (on p1->p2) is walkable; bisects the cut distance from the corner.
U __fastcall kotor_fastcall_004be3a0(void* area, void*, float x0, float y0, float x1, float y1, float x2, float y2,
                                     float radius, float height, float* p_x, float* p_y, float* q_x, float* q_y) {
    const double d2x = static_cast<double>(x2) - x1;                                // 004BE3A3..A8
    const float d2x_f = static_cast<float>(d2x);                                    // 004BE3B2 fst E-28
    const double d2y = static_cast<double>(y2) - y1;                                // 004BE3B6..BA
    const float d2y_f = static_cast<float>(d2y);                                    // 004BE3BE fst E-24
    const float len2 = static_cast<float>(x87_fsqrt(d2y * d2y + d2x * d2x));        // 004BE3C2..CE
    const double d1x = static_cast<double>(x1) - x0;                                // 004BE3D6..DA
    const float d1x_f = static_cast<float>(d1x);                                    // E-30
    const double d1y = static_cast<double>(y1) - y0;                                // 004BE3E2..E6
    const float d1y_f = static_cast<float>(d1y);                                    // E-2C
    const float len1 = static_cast<float>(x87_fsqrt(d1y * d1y + d1x * d1x));        // 004BE3EE..FA
    if (len1 < 0.0001f) return 0;                                                   // 004BE402..11 (test ah,5; jnp)
    if (len2 < 0.0001f) return 0;                                                   // 004BE417..26
    const double cosine = (static_cast<double>(d1y_f) * d2y_f + static_cast<double>(d1x_f) * d2x_f) /
                          (static_cast<double>(len1) * len2);                       // 004BE42C..46
    if (cosine >= 0.9f) return 0;                                                 // 004BE448..53 (test ah,1; je)
    float shortest = len1;                                                          // 004BE459..65 E-2C
    if (len2 < len1) shortest = len2;                                               // 004BE46B..74
    float t = 0.5f, step = 0.25f;                                                   // E-34 / E-30
    U found = 0;                                                                    // ESI
    bool first = true;                                                              // EAX = 1
    for (;;) {
        if (!first && !(static_cast<double>(step) * shortest > 0.1f)) break;        // 004BE4A0..B8
        const double tm = static_cast<double>(t) * shortest;                        // 004BE4BE..C6
        const double inv1 = 1.0f / len1;                                            // 004BE4CC..D4
        const float ax = static_cast<float>((static_cast<double>(x0) - x1) * inv1 * tm + x1); // 004BE4ED..FD
        const double ay = (static_cast<double>(y0) - y1) * inv1 * tm + y1;          // 004BE504..1F
        const float ay_f = static_cast<float>(ay);                                  // 004BE523 fst E-1C
        const double inv2 = 1.0f / len2;                                            // 004BE527..2D
        const double bx = inv2 * tm * d2x_f + x1;                                   // 004BE531..39
        const float bx_f = static_cast<float>(bx);                                  // 004BE53D fst
        const float by_f = static_cast<float>(inv2 * tm * d2y_f + y1);              // 004BE541..4D
        const Vector from = {ax, ay_f, 0.0f};                                       // E-C
        const Vector to = {bx_f, by_f, 0.0f};                                       // E-18
        const float r = radius;                                                     // the radius argument slot
        if (test_line(area, &from, &to, &r, height, 1, nullptr) == 1) {             // 004BE55D..76
            t = static_cast<float>(static_cast<double>(step) + t);                  // 004BE57C..84
            *p_x = ax; *q_x = bx_f; *p_y = ay_f; *q_y = by_f;                       // 004BE578..9B
            found = 1;                                                              // 004BE59D
        } else {
            t = static_cast<float>(static_cast<double>(t) - step);                  // 004BE5A4..AC
        }
        first = false;                                                              // 004BE5B0
        step = static_cast<float>(static_cast<double>(step) * 0.5f);                // 004BE5B2..BC
    }
    return found;
}

}  // extern "C"
