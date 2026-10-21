// W5 G16: CSWSArea::TestDirectLine (004BCB70) - straight-line walkability across rooms.
// Reconstructed from swkotor.exe (GOG), read-only disassembly. Native x86 export (see w5_types.hpp).
#include "4f99fc5850-w5_walkmesh.hpp"
#include "4f622be2f2-w5_area_direct_line.hpp"

using namespace w5;

extern "C" {
U w5_direct_line_debug_008327c4 = 0;     // .data 008327C4: cleared by TestDirectLine when nonzero (no other writer seen)
U w5_direct_line_heights_008327c8 = 0;   // .data 008327C8: nonzero -> debug room-height probes at both endpoints
}

namespace {
constexpr float k_band_0073e798 = 0.1f;        // .rdata 0073E798
constexpr float k_far_0073d6fc = 1000.0f;      // .rdata 0073D6FC
constexpr float k_reach_00749b30 = 0.0001f;    // .rdata 00749B30 (9.9999997e-05)
constexpr float k_zero_0073d700 = 0.0f;

// x87 axis test 004BCD38..004BCD84: fail when !(target - eps <= hit) (ordered) or (target + eps < hit / unordered).
bool reached_axis(float target, float hit) {
    const double lo = static_cast<double>(target) - k_reach_00749b30;
    if (!(lo <= hit)) return false;                    // test ah,41h; jp -> fail (greater or unordered)
    const double hi = static_cast<double>(k_reach_00749b30) + target;
    if (!(hi >= hit)) return false;                    // test ah,1; jne -> fail (less or unordered)
    return true;
}
}  // namespace

extern "C" I __fastcall w5_area_test_direct_line_004bcb70(void* area, void*, const Vector* from, const Vector* to,
    const float* radius, float height_band, U unused5, void* walk_hit) {
    (void)unused5;                                                         // [esp+7C] never read
    I result = 1;                                                          // [E+10]
    U done = 0;                                                            // [E+1C]
    Vector cur; copy_vector(&cur, from);                                   // [E+44]
    Vector clip_from{0, 0, 0};                                             // [E+38]
    Vector clip_to{0, 0, 0};                                               // [E+2C]
    void* room = area_get_room(area, &cur, nullptr); // 004BCBBF (out = 0)
    if (w5_direct_line_debug_008327c4 != 0) w5_direct_line_debug_008327c4 = 0;  // 004BCBC6..004BCBCE
    float z_low = 0.0f, z_high = 0.0f;                                     // [E+14] [E+18]
    U edge;                                                                // [E+24] (written by the clip walk)
    U blocker;                                                             // [E+28]
    for (;;) {
        if (done != 0) break;                                              // 004BCBF0..004BCBF6 -> 004BCDF6
        if (room == nullptr) return 0;                                     // 004BCBFC..004BCBFE -> 004BCED0 (no debug probes)
        if (w5_room_clipped_segment_walkable_00579670(room, nullptr, &cur, to, &clip_from, &clip_to, &z_low, &z_high, &edge) != 1) { // 004BCC29..004BCC31
            result = -1; break;                                            // 004BCEA9
        }
        z_low = static_cast<float>(static_cast<double>(z_low) - k_band_0073e798);                    // 004BCC37..004BCC4B
        z_high = static_cast<float>((static_cast<double>(z_high) + height_band) + k_band_0073e798);  // 004BCC50..004BCC72
        if (w5_adapt_room_no_nonwalk_polys_00579680(room, nullptr, &clip_from, &clip_to, &z_low, &z_high, radius, walk_hit) == 0) { // 004BCC76
            result = -1; break;
        }
        void* const mover = at<void*>(area, 0x1ac);                        // 004BCC87
        const float low_far = static_cast<float>(static_cast<double>(z_low) - k_far_0073d6fc);   // 004BCC8D..004BCC9E
        const float high_far = static_cast<float>(static_cast<double>(z_high) + k_far_0073d6fc); // 004BCCA2..004BCCB9 (stored in the arg-6 slot)
        if (area_no_nonwalk_polys_detailed_00506650(area, nullptr, &clip_from, &clip_to, low_far, high_far, *radius,
                at<U>(mover, 0xbc), &blocker, 0, nullptr, 0, walk_hit, 0) == 0) {  // 004BCCD9
            result = -2; break;                                            // 004BCEB6
        }
        void* const creature = at<void*>(area, 0x1ac);                     // 004BCCE6 (re-read)
        if (creature == nullptr) { result = -3; break; }                   // 004BCCEC..004BCCEE
        if (at<U>(creature, 0x210) == 0) {                                 // 004BCCF4..004BCCFC
            if (!(*radius > k_zero_0073d700)) { result = -3; break; }      // 004BCCFE..004BCD0B (test ah,41h; jne: fail when <= 0 or unordered)
            if (w5x_area_no_creatures_on_line_004bc590(area, nullptr, &clip_from, &clip_to, creature, 1, 0, 0, 1, walk_hit) == 0) { // 004BCD27
                result = -3; break;
            }
        }
        if (reached_axis(to->x, clip_to.x) && reached_axis(to->y, clip_to.y)) { // 004BCD34..004BCD84
            done = 1; continue;                                            // 004BCD86
        }
        const I next = w5_adapt_room_adjacent_00579420(room, nullptr, edge); // 004BCD9A
        if (next == -1) { result = 0; break; }                             // 004BCD9F..004BCDA2 -> 004BCDEE
        unsigned char* const rooms = at<unsigned char*>(area, 0x230);
        if (at<U>(rooms + next * 0x4c, 0x38) != 0) { result = 0; break; }  // 004BCDA4..004BCDB5
        (void)aurora_vector_equal_004aa980(&clip_from, &clip_to);         // 004BCDC1 (EAX discarded)
        room = rooms + next * 0x4c;                                        // 004BCDDB
        copy_vector(&cur, &clip_to);                                       // 004BCDDD..004BCDE5
    }
    // 004BCDF6..004BCE99: optional debug height probes at both endpoints (results discarded).
    if (w5_direct_line_heights_008327c8 != 0) {
        Vector f; copy_vector(&f, from);                                   // [E+5C]
        Vector t; copy_vector(&t, to);                                     // [E+50]
        U scratch_out;                                                     // arg-1 stack slot reused as GetRoom out
        if (void* r = area_get_room(area, &f, &scratch_out))
            (void)w5_room_compute_height_00579430(r, nullptr, f, 0);       // 004BCE5B (fstp st(0))
        if (void* r = area_get_room(area, &t, &scratch_out))
            (void)w5_room_compute_height_00579430(r, nullptr, t, 0);       // 004BCE94
    }
    return result;                                                         // 004BCE9B
}
