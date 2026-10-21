// W5 lane "pathfind-plan" part 2/3: the walkmesh grid search (CSWSArea::PlotGridPath 004C1A00, GridDFSearch,
// GridDFSGenerateSuccessors) and the path-point graph search (PlotPathPointPath 004C2200, PathPointDFSearch,
// PathPointDFSGenerateSuccessors, ClosestPathPoint). Transcribed from the original swkotor.exe (read-only);
// addresses in the comments are the original instructions.
#include "eb5e5e70a0-claude_w5_pathfind_plan.hpp"

using namespace w5;
using namespace w5pp;

namespace {
// cdq / xor / sub absolute value (|INT_MIN| stays INT_MIN)
inline I iabs(I v) { const U s = static_cast<U>(v >> 31); return static_cast<I>((static_cast<U>(v) ^ s) - s); }
inline I imul(I a, I b) { return static_cast<I>(static_cast<U>(a) * static_cast<U>(b)); }
// CPathfindInformation grid frame: +12C origin (goal), +138 perpendicular step, +144 step along goal->start,
// +150 up. The three transcribed grid->world variants keep the original rounding points (fstp dword).
struct GridFrame {
    const float* o; const float* p; const float* d; const float* u;
    explicit GridFrame(void* pf)
        : o(&at<float>(pf, 0x12c)), p(&at<float>(pf, 0x138)), d(&at<float>(pf, 0x144)), u(&at<float>(pf, 0x150)) {}
};
// x*p + y*d + z*u + o (004BCF68..D04D, 004BD4FC..D620)
inline Vector grid_point_full(void* pf, I x, I y, I z) {
    const GridFrame f(pf);
    const double zz = z, yy = y, xx = x;
    const double zux = zz * f.u[0];
    const float zuy = static_cast<float>(zz * f.u[1]);
    const float zuz = static_cast<float>(zz * f.u[2]);
    const double ydx = yy * f.d[0];
    const float ydy = static_cast<float>(yy * f.d[1]);
    const float ydz = static_cast<float>(yy * f.d[2]);
    const double xpx = xx * f.p[0];
    const float xpy = static_cast<float>(xx * f.p[1]);
    const float xpz = static_cast<float>(xx * f.p[2]);
    double sx = xpx + f.o[0];
    const float ay = static_cast<float>(static_cast<double>(xpy) + f.o[1]);
    const float az = static_cast<float>(static_cast<double>(xpz) + f.o[2]);
    sx = ydx + sx;                                                                  // faddp st(1)
    const float by = static_cast<float>(static_cast<double>(ay) + ydy);
    const float bz = static_cast<float>(static_cast<double>(az) + ydz);
    Vector r;
    r.x = static_cast<float>(sx + zux);                                             // fadd st(1)
    r.y = static_cast<float>(static_cast<double>(by) + zuy);
    r.z = static_cast<float>(static_cast<double>(bz) + zuz);
    return r;
}
// x*p + y*d + 0.0f*u + o (004BD1AB..D2A7): the up term multiplies the .rdata 0.0f constant; x and y keep the
// unrounded x87 products of it, z the stored dword
inline Vector grid_point_zero_up(void* pf, I x, I y) {
    const GridFrame f(pf);
    const double ux0 = static_cast<double>(f.u[0]) * 0.0f;
    const double uy0 = static_cast<double>(f.u[1]) * 0.0f;
    const float uz0 = static_cast<float>(static_cast<double>(f.u[2]) * 0.0f);
    const double yy = y, xx = x;
    const double ydx = yy * f.d[0];
    const float ydy = static_cast<float>(yy * f.d[1]);
    const float ydz = static_cast<float>(yy * f.d[2]);
    const double xpx = xx * f.p[0];
    const float xpy = static_cast<float>(xx * f.p[1]);
    const float xpz = static_cast<float>(xx * f.p[2]);
    double sx = xpx + f.o[0];
    const float ay = static_cast<float>(static_cast<double>(xpy) + f.o[1]);
    const float az = static_cast<float>(static_cast<double>(xpz) + f.o[2]);
    sx = ydx + sx;
    const float by = static_cast<float>(static_cast<double>(ay) + ydy);
    const float bz = static_cast<float>(static_cast<double>(az) + ydz);
    Vector r;
    r.x = static_cast<float>(sx + ux0);
    r.y = static_cast<float>(static_cast<double>(by) + uy0);
    r.z = static_cast<float>(static_cast<double>(bz) + uz0);
    return r;
}
// x*p + y*d + o (004BD381..D435)
inline Vector grid_point_flat(void* pf, I x, I y) {
    const GridFrame f(pf);
    const double yy = y, xx = x;
    const double ydx = yy * f.d[0];
    const float ydy = static_cast<float>(yy * f.d[1]);
    const float ydz = static_cast<float>(yy * f.d[2]);
    const double xpx = xx * f.p[0];
    const float xpy = static_cast<float>(xx * f.p[1]);
    const float xpz = static_cast<float>(xx * f.p[2]);
    const double sx = xpx + f.o[0];
    const float ay = static_cast<float>(static_cast<double>(xpy) + f.o[1]);
    const float az = static_cast<float>(static_cast<double>(xpz) + f.o[2]);
    Vector r;
    r.x = static_cast<float>(sx + ydx);                                             // fadd st(1)
    r.y = static_cast<float>(static_cast<double>(ay) + ydy);
    r.z = static_cast<float>(static_cast<double>(az) + ydz);
    return r;
}
inline void set_entry(I* e, I a, I b, I c, I d) { e[0] = a; e[1] = b; e[2] = c; e[3] = d; }
const char k_reversed_0074564c[] = "we can not return a closest result if points are reversed.";
const char k_depth_00745628[] = "Exceeded limits:Max search deapth";
const char k_time_0074560c[] = "Exceeded limits:Time limit";
const char k_distance_007455e8[] = "Exceeded limits:Distance to goal";
const char k_no_closest_00745764[] = "Error could not find closest path node to start or end";
const char k_levels_0074573c[] = "we exceeded 4096 levels of searching.. ";
const char k_start_grid_007456e0[] = "    failed to grid based pathfind from the creatures position to the starting path point.";
const char k_end_grid_00745688[] = "    failed to grid based pathfind from the ending path point ot the destiantion.";
}  // namespace

extern "C" {

// =========================================================================================================
// 004C1A00 CSWSArea::PlotGridPath(pf, budget lo, budget hi): RET 0Ch. Iterative-deepening DFS on a grid of
// step max(radius, 0.1) laid out from the goal towards the start; time-sliced by the 64-bit timer budget.
U __fastcall kotor_fastcall_004c1a00(void* area, void*, void* pf, U budget_lo, U budget_hi) {
    at<U>(recovered_server_ai_master_004aed80(server_app(), nullptr), 0x6c) = 1;    // 004C1A03..19
    I iterations = 0;                                                               // [esp+18] E-70
    const std::uint64_t t0 = hr_timer();                                            // 004C1A20..2F E-3C/E-38
    const std::uint64_t budget = (static_cast<std::uint64_t>(budget_hi) << 32) | budget_lo;
    at<void*>(area, 0x1ac) = pf;                                                    // 004C1A34..3B
    float step = at<float>(pf, 4);                                                  // 004C1A41..4E fst E-78
    if (step <= 0.1f) step = 0.1f;                                                  // 004C1A52..89 (test ah,41h; jp keeps)
    const float goal_x = at<float>(pf, 0x5c), goal_y = at<float>(pf, 0x60);         // E-6C / E-68
    const float start_x = at<float>(pf, 0x74), start_y = at<float>(pf, 0x78);       // E-64 / E-60
    const Vector origin = {goal_x, goal_y, 0.0f};                                   // E-48
    const float dx = static_cast<float>(static_cast<double>(start_x) - goal_x);     // 004C1A91..AB (E-50, E-24)
    const float dy = static_cast<float>(static_cast<double>(start_y) - goal_y);     // 004C1AB7..E1 (E-54, E-20)
    const Vector toward = {dx, dy, 0.0f};                                           // E-24
    const Vector up = {0.0f, 0.0f, 1.0f};                                           // E-18
    Vector unit;                                                                    // E-C
    normalize_004ab180(&unit, &toward);                                             // 004C1AE5
    const float sx = static_cast<float>(static_cast<double>(step) * unit.x);        // 004C1AEA..F8 E-30
    const float sy = static_cast<float>(static_cast<double>(step) * unit.y);        // 004C1AFC..03 E-2C (fst; same value fstp E-24)
    const float sz = static_cast<float>(static_cast<double>(step) * unit.z);        // 004C1B07..16 E-28
    const Vector perpendicular = {sy, -sx, 0.0f};                                   // 004C1B1A..45 E-24
    const Vector delta = {dx, dy, 0.0f};                                            // 004C1B26..3A E-C
    const I cells = ftol2(kotor_cdecl_004ab100(&delta) / step + 0.5);               // 004C1B49..58 (EDI)
    copy_vector(bytes(pf) + 0x12c, &origin);                                        // 004C1B5D..76
    copy_vector(bytes(pf) + 0x138, &perpendicular);                                 // 004C1B79..90
    const Vector along = {sx, sy, sz};
    copy_vector(bytes(pf) + 0x144, &along);                                         // 004C1B93..AA
    copy_vector(bytes(pf) + 0x150, &up);                                            // 004C1BAD..E3
    at<I>(pf, 0x120) = 0; at<I>(pf, 0x124) = cells; at<I>(pf, 0x128) = 0;          // 004C1BD1..DD
    at<U>(pf, 0x24) = 1;                                                            // 004C1BE6
    I span = cells + 0x24;                                                          // 004C1BED E-5C
    const U size = static_cast<U>(span) * 8 + 0x10;                                 // 004C1BF0 (ESI)
    void* path = live_operator_new(size);                                           // 004C1BF7..FC E-4C
    at<void*>(pf, 0x15c) = path;                                                    // 004C1C05
    void* best = at<void*>(pf, 0x18c);                                              // 004C1C0B E-50
    if (best == nullptr) {                                                          // 004C1C14..1A
        best = live_operator_new(size);                                             // 004C1C1C..1D
        at<U>(pf, 0x188) = 0;                                                       // 004C1C29
        at<void*>(pf, 0x18c) = best;                                                // 004C1C33
    }
    // 004C1C39..CE: resume the previous slice when start and goal match the saved +170/+17C
    const double a = static_cast<double>(start_x) - at<float>(pf, 0x170);
    const double b = static_cast<double>(start_y) - at<float>(pf, 0x174);
    const double c = -static_cast<double>(at<float>(pf, 0x178));
    const float ga = static_cast<float>(static_cast<double>(goal_x) - at<float>(pf, 0x17c)); // E-48
    const float gb = static_cast<float>(static_cast<double>(goal_y) - at<float>(pf, 0x180)); // E-44
    const float gc = -at<float>(pf, 0x184);                                         // E-40
    I depth;                                                                        // E-78 (ESI)
    if (!((c * c + b * b) + a * a >= 0.0001f) &&                                        // 004C1C7D..9E (test ah,1; je)
        !((static_cast<double>(gc) * gc + static_cast<double>(gb) * gb) + static_cast<double>(ga) * ga >= 0.0001f)) { // 004C1CA0..C7
        at<void*>(area, 0x1b4) = at<void*>(pf, 0x16c);                              // 004C1CC9..CF
        depth = at<I>(pf, 0x168) + 2;                                               // 004C1CD5..DE
    } else {
        if (at<void*>(pf, 0x16c) != nullptr) at<void*>(area, 0x1b4) = at<void*>(pf, 0x16c); // 004C1CE4..EE
        kotor_fastcall_004bbc50(area, nullptr);                                     // 004C1CF4..F6
        depth = cells + 2;                                                          // 004C1CFB..02
    }
    at<U>(pf, 0x164) = 0x41200000u;                                                 // 004C1D04 (10.0f)
    std::uint64_t elapsed = hr_timer() - t0;                                        // 004C1D0E..3B (E-48, EBP)
    U result = k_in_progress_00745dbc;                                              // 004C1D27..32 E-7C
    if (at<U>(pf, 0x24) == 1 && at<U>(pf, 0x84) == 1) {                            // 004C1D3D..47
        debug_print(pf, 2, k_reversed_0074564c);                                    // 004C1D49..52
        result = k_failed_00745dc4;                                                 // 004C1D57..5D
    }
    at<U>(area, 0x1d0) = 0; at<U>(area, 0x1cc) = 0;                                 // 004C1D61..6D
    if (at<I>(pf, 0x190) > cells + 1) at<I>(pf, 0x190) = cells + 1;                 // 004C1D73..7E
    const I closest = at<I>(pf, 0x190);                                             // 004C1D88..8E E-58
    if (depth < span) {                                                             // 004C1D84..92
        do {
            if (result != k_in_progress_00745dbc) break;                            // 004C1DA0..AA
            if (elapsed >= budget) break;                                           // 004C1DB0..CC (unsigned 64)
            ++iterations;                                                           // 004C1DD2..E3
            depth += 2;                                                             // 004C1DE7..F2
            at<U>(area, 0x1d0) = 0; at<U>(area, 0x1cc) = 0;                         // 004C1DF6..FC
            if (kotor_fastcall_004c0930(area, nullptr, 0, 0, depth, 0, cells, 0) == 1) result = k_complete_00745dc0; // 004C1DDE..12
            elapsed = hr_timer() - t0;                                              // 004C1E16..3E
            if (at<U>(pf, 0x24) == 1) {                                             // 004C1E30..42
                const I kept = at<I>(pf, 0x188);                                    // 004C1E48..50 (E-5C)
                if (kept > 1 && result != k_complete_00745dc0 && iterations == 1) { // 004C1E54..65
                    const std::uint64_t four = budget * 4;                          // 004C1E67..74 __allmul
                    bool take = elapsed >= four;                                    // 004C1E79..81
                    if (!take) take = elapsed >= budget && closest == at<I>(pf, 0x190); // 004C1E83..9C
                    if (take) {
                        if (at<U>(area, 0x1cc) != 1) result = k_complete_00745dc0;  // 004C1E9E..B1
                        at<I>(pf, 0x160) = kept;                                    // 004C1EC8
                        std::memcpy(at<void*>(pf, 0x15c), at<void*>(pf, 0x18c), static_cast<U>(kept) * 8); // 004C1EB9..CE rep movsd
                    }
                }
            }
            span = cells + 0x24;                                                    // 004C1ED0..DD
        } while (depth < span);                                                     // 004C1EDB..E1
    }
    at<U>(pf, 0x164) = 0x442f0000u;                                                 // 004C1EF9 (700.0f)
    if (result != k_complete_00745dc0) {                                            // 004C1F03..09
        bool report = false;                                                        // reaches 004C1F7D
        bool fail = true;                                                           // reaches 004C1FD3
        if (depth < span) {                                                         // 004C1F0F..17
            if (iterations != 1) { fail = false; }                                  // 004C1F19..1E -> 004C1FDC
            else if (elapsed >= budget * 40) { report = true; }                     // 004C1F24..39
            else if (elapsed < budget * 10) { fail = false; }                       // 004C1F3B..54
            else if (closest != at<I>(pf, 0x190)) { fail = false; }                 // 004C1F5A..64
            else { report = true; }
        } else {
            debug_print(pf, 2, k_depth_00745628);                                   // 004C1F68..71
            report = iterations == 1;                                               // 004C1F76..7B
        }
        if (report) {
            if (elapsed >= budget * 40) debug_print(pf, 2, k_time_0074560c);        // 004C1F7D..9D
            if (elapsed >= budget * 10 && closest == at<I>(pf, 0x190)) debug_print(pf, 2, k_distance_007455e8); // 004C1FA2..CE
        }
        if (fail) result = k_failed_00745dc4;                                       // 004C1FD3..D8
    }
    if (result == k_in_progress_00745dbc) {                                         // 004C1FDC..E6: save the slice
        at<float>(pf, 0x170) = start_x; at<float>(pf, 0x174) = start_y; at<U>(pf, 0x178) = 0; // 004C1FEC..20
        at<float>(pf, 0x17c) = goal_x; at<float>(pf, 0x180) = goal_y; at<U>(pf, 0x184) = 0;  // 004C2026..58
        at<I>(pf, 0x168) = depth;                                                   // 004C2038
        at<void*>(pf, 0x16c) = at<void*>(area, 0x1b4);                              // 004C205E..64
        at<void*>(area, 0x1b4) = nullptr;                                           // 004C206A
    } else {
        live_free(at<void*>(area, 0x1b4));                                          // 004C2076..81 (_free, no null test)
        at<void*>(area, 0x1b4) = nullptr;                                           // 004C208B
        for (U off = 0x170; off <= 0x184; off += 4) at<U>(pf, off) = 0;             // 004C2095..CB
        at<U>(pf, 0x168) = 0; at<U>(pf, 0x16c) = 0;                                 // 004C20D1..D7
    }
    if (result == k_complete_00745dc0) {                                            // 004C20DD..E7: grid cells -> world xy
        const I n = at<I>(pf, 0x160);                                               // 004C20ED
        float* out = static_cast<float*>(live_operator_new(static_cast<U>(n) * 8)); // 004C20F3..FB
        const I* cell = at<const I*>(pf, 0x15c);                                    // 004C2104 (EBP)
        out[0] = start_x; out[1] = start_y;                                         // 004C2100..1A
        for (I k = 1; k < n; ++k) {                                                 // 004C2118..8F
            const double along_k = cell[k * 2 + 1];                                 // 004C2126 fild
            const float ax = static_cast<float>(static_cast<double>(sx) * along_k); // 004C212A..35 E-C
            const float ay = static_cast<float>(static_cast<double>(sy) * along_k); // 004C213C..42 E-8
            const double across = cell[k * 2];                                      // 004C214B fild
            const double px = static_cast<double>(sy) * across;                     // 004C214F..53
            const float py = static_cast<float>(static_cast<double>(-sx) * across); // 004C2155..5B E-14
            const double gx = px + goal_x;                                          // 004C215F
            const float gy = static_cast<float>(static_cast<double>(py) + goal_y);  // 004C2165..6D E-38
            out[k * 2] = static_cast<float>(gx + ax);                               // 004C2171 / 89
            out[k * 2 + 1] = static_cast<float>(static_cast<double>(gy) + ay);      // 004C2178..83 / 8C
        }
        at<I>(pf, 0x94) = n + n;                                                    // 004C2191..94
        at<float*>(pf, 0x98) = out;                                                 // 004C219A
    }
    if (result != k_in_progress_00745dbc) {                                         // 004C21A0..AB
        live_free(best);                                                            // 004C21AD..B2
        at<void*>(pf, 0x18c) = nullptr;                                             // 004C21BA
    }
    live_free(path);                                                                // 004C21C4..C9
    at<void*>(pf, 0x15c) = nullptr;                                                 // 004C21D2
    if (result == k_complete_00745dc0) kotor_fastcall_004c1670(area, nullptr);      // 004C21E4..EC
    return result;
}

// 004C0930 CSWSArea::GridDFSearch(depth, g, limit, x, y, z): RET 18h, EAX 1 when the goal cell (or a cell inside
// the step tolerance +6C) is reached
U __fastcall kotor_fastcall_004c0930(void* area, void*, I depth, I g, I limit, I x, I y, I z) {
    void* pf = pf_of(area);                                                         // 004C0937
    I* path = at<I*>(pf, 0x15c);                                                    // 004C093D
    at<I>(pf, 0x160) = depth + 1;                                                   // 004C0948..4B
    path[depth * 2 + 1] = y;                                                        // 004C0956
    path[depth * 2] = x;                                                            // 004C0962
    const I ax = iabs(x);                                                           // 004C0965..6C (EBP)
    const I ay = iabs(y);                                                           // 004C096E..79
    const I manhattan = ay + ax;                                                    // 004C097B
    if (at<I>(pf_of(area), 0x190) > manhattan) {                                    // 004C097E..84 (jle)
        at<I>(pf_of(area), 0x190) = manhattan;                                      // 004C0986
        std::memcpy(at<void*>(pf_of(area), 0x18c), at<void*>(pf_of(area), 0x15c), static_cast<U>(depth) * 8 + 8); // 004C098C..B5
        at<I>(pf_of(area), 0x188) = depth + 1;                                      // 004C09B7..C1
    }
    if (x == 0 && y == 0 && z == 0) return 1;                                       // 004C09C7..D9
    const I tolerance = at<I>(pf_of(area), 0x6c);                                   // 004C09DB..E4 E-4
    const I squared = static_cast<I>(static_cast<U>(imul(x, x)) + static_cast<U>(imul(y, y)) + static_cast<U>(imul(z, z))); // 004C09E8..01
    if (!(squared > imul(tolerance, tolerance)) && depth > 0) return 1;             // 004C0A08..12
    if (limit < static_cast<I>(static_cast<U>(ay) - static_cast<U>(tolerance) + static_cast<U>(ax))) return 0; // 004C0A23..2D
    I stored_g = 0, stored_limit = 0;                                               // [esp+1C] (depth slot) / E-4
    if (kotor_fastcall_004bbcc0(area, nullptr, x, y, &stored_g, &stored_limit) != 0) { // 004C0A2F..48
        if (g > stored_g) return 0;                                                 // 004C0A4A..52
        if (!(stored_limit < limit)) return 0;                                      // 004C0A54..58
    }
    const I visits = at<I>(area, 0x1d0) + 1;                                        // 004C0A66..6C
    at<I>(area, 0x1d0) = visits;                                                    // 004C0A72
    if (visits >= 0x40) at<U>(area, 0x1cc) = 1;                                     // 004C0A6F..7A
    I* successors = nullptr;                                                        // [esp+30] (the z slot)
    const I count = kotor_fastcall_004bcee0(area, nullptr, g, limit, x, y, z, &successors); // 004C0A84..98
    U result = 0;                                                                   // EDI
    for (I i = 0; i < count; ++i) {                                                 // 004C0AA1..F2
        if (result != 0) break;                                                     // 004C0AB0..B2
        if (at<U>(area, 0x1cc) != 0) break;                                         // 004C0AB4..BC
        const I* e = successors + i * 4;                                            // ESI
        result = kotor_fastcall_004c0930(area, nullptr, depth + 1, g + e[0], limit - e[0], e[1], e[2], e[3]); // 004C0ABE..E6
    }
    live_free(successors);                                                          // 004C0AF8..FD
    kotor_fastcall_004bbd80(area, nullptr, x, y, g, limit);                         // 004C0B02..17
    return result;
}

// 004BCEE0 CSWSArea::GridDFSGenerateSuccessors(g, limit, x, y, z, out): RET 18h, EAX successor count. Entries are
// {cost, x, y, z} (16 bytes, 9 allocated). Every 6th depth limit (4, 10, 16, ...) first tries a long jump.
I __fastcall kotor_fastcall_004bcee0(void* area, void*, I g, I limit, I x, I y, I z, I** out) {
    void* pf = pf_of(area);                                                         // 004BCEEA (EDI from 004BCF30)
    const float radius = at<float>(pf, 4);                                          // 004BCEFE..02 E-38
    const float height = at<float>(pf, 0x14);                                       // 004BCF06..17 E-34
    I count = 0;                                                                    // E-54
    *out = static_cast<I*>(live_operator_new(0x90));                                // 004BCF0A..27
    Vector here;                                                                    // E-30
    if (g == 0) {                                                                   // 004BCF39..3B
        here.x = at<float>(pf, 0x74);                                               // 004BCF3D..51 (dword move)
        here.y = at<float>(pf, 0x78);                                               // 004BCF40..45 (fld/fstp)
        here.z = 0.0f;                                                              // 004BCF5F
    } else {
        here = grid_point_full(pf, x, y, z);                                        // 004BCF68..4D
    }
    U jumped = 0;                                                                   // E-78
    const bool try_jump = limit == 4 || (limit > 4 && g % 6 == 4);                 // 004BD051..72 (idiv divides EAX = g, review P2)
    if (try_jump) {
        const Vector goal = {at<float>(pf, 0x12c), at<float>(pf, 0x130), at<float>(pf, 0x134)}; // 004BD078..92 E-60
        const I tolerance = at<I>(pf, 0x6c);                                        // 004BD096..A0 E-88
        I jump_x = 0, jump_y = 0;                                                   // EBX / E-8C
        bool bisect = true;
        if (tolerance <= 1) {                                                       // 004BD099..A4 (jg)
            if (test_line(area, &here, &goal, &radius, height, 1, nullptr) == 1) {  // 004BD0A6..CA
                set_entry(*out, iabs(x) + iabs(y), 0, 0, 0);                        // 004BD0D0..FF
                return 1;
            }
        } else {
            const I squared = static_cast<I>(static_cast<U>(imul(x, x)) + static_cast<U>(imul(y, y)) + static_cast<U>(imul(z, z))); // 004BD111..39
            const double dsq = squared;                                             // 004BD13D fild
            const double inverse = 1.0f / x87_fsqrt(dsq);                           // 004BD141..45 (fdivr [1.0f])
            const double tol = tolerance;                                           // 004BD14B fild
            jump_x = ftol2(static_cast<double>(x) * inverse * tol * 0.833333313465118408203125f); // 004BD14F..60
            jump_y = ftol2(static_cast<double>(y) * inverse * tol * 0.833333313465118408203125f); // 004BD165..7C
            const double tol_sq = imul(tolerance, tolerance);                       // 004BD187..98
            if (dsq >= tol_sq) {                                                    // 004BD19C..A5 (fcompp; test ah,1)
                const Vector target = grid_point_zero_up(pf, jump_x, jump_y);       // 004BD1AB..2A7
                if (test_line(area, &here, &target, &radius, height, 1, nullptr) == 1) { // 004BD2B1..BD
                    set_entry(*out, iabs(y - jump_y) + iabs(x - jump_x), jump_x, jump_y, 0); // 004BD2BF..ED
                    return 1;
                }
            }
        }
        if (bisect) {
            // 004BD30E..4BE: bisect between here and the jump target
            const float span_x = static_cast<float>(static_cast<double>(jump_x - x));  // 004BD315..2D E-20
            const float span_y = static_cast<float>(static_cast<double>(jump_y - y));  // 004BD31F..3D E-70
            float t = 0.5f, step = 0.25f;                                           // E-8C / E-88
            double next;
            do {
                const I nx = ftol2(static_cast<double>(t) * span_x) + x;            // 004BD341..5A (EDI, E-1C)
                const I ny = ftol2(static_cast<double>(t) * span_y) + y;            // 004BD34E..73 (EBX, E-74)
                if (!(nx == x && ny == y)) {                                        // 004BD371..7B
                    const Vector target = grid_point_flat(pf_of(area), nx, ny);     // 004BD381..435
                    if (test_line(area, &here, &target, &radius, height, 1, nullptr) == 1) { // 004BD44B..53
                        t = static_cast<float>(static_cast<double>(step) + t);      // 004BD45C..62
                        set_entry(*out, iabs(y - ny) + iabs(x - nx), nx, ny, 0);    // 004BD455..88
                        jumped = 1;                                                 // 004BD48F
                    } else {
                        t = static_cast<float>(static_cast<double>(t) - step);      // 004BD499..A1
                    }
                }
                next = static_cast<double>(step) * 0.5f;                            // 004BD4A5..AF
                step = static_cast<float>(next);
            } while (next > 0.005f);                                                // 004BD4B3..BE
            if (jumped == 1) return 1;                                              // 004BD4C4..CF
        }
    }
    // 004BD4D5..75B: the 8 neighbour cells, kept sorted by |x| + |y| (one bubble pass per insertion)
    for (I dj = -1; dj <= 1; ++dj) {                                                // EBP / E-74
        for (I di = -1; di <= 1; ++di) {                                            // EBX / E-8C
            if (dj == 0 && di == 0) continue;                                       // 004BD4F0..F6
            const I ny = y + dj;                                                    // 004BD50D..1E E-88
            const I nx = x + di;                                                    // 004BD524..33 E-78
            const Vector target = grid_point_full(pf_of(area), nx, ny, z);          // 004BD4FC..620
            if (test_line(area, &here, &target, &radius, height, 1, nullptr) != 1) continue; // 004BD636..3E
            set_entry(*out + count * 4, iabs(di) + iabs(dj), nx, ny, 0);            // 004BD644..76
            ++count;                                                                // 004BD66F..84
            if (count > 1) {                                                        // 004BD681..8F
                for (I j = count - 1; j != 0; --j) {                                // 004BD695..72A
                    I* cur = *out + j * 4;
                    I* prev = cur - 4;
                    const I prev_key = iabs(prev[1]) + iabs(prev[2]);               // 004BD6A5..B7
                    const I cur_key = iabs(cur[2]) + iabs(cur[1]);                  // 004BD6B9..CB
                    if (cur_key > prev_key) continue;                               // 004BD6CD..CF (jg)
                    for (I k = 0; k < 4; ++k) { const I v = cur[k]; cur[k] = prev[k]; prev[k] = v; } // 004BD6D1..1A
                }
            }
        }
    }
    return count;
}

// =========================================================================================================
// 004BD770 CSWSArea::ClosestPathPoint(point, out index): RET 8, EAX 1 found (or no path points / no pf, index 0)
U __fastcall kotor_fastcall_004bd770(void* area, void*, const Vector* point, I* out) {
    const float* points = at<const float*>(area, 0x23c);                            // 004BD779
    if (points == nullptr || at<void*>(area, 0x1ac) == nullptr) { *out = 0; return 1; } // 004BD77F..8F / 004BDADA
    void* pf = at<void*>(area, 0x1ac);
    const float first_x = points[0];                                                // 004BD7A1 E-94 (dword)
    U n = at<U>(area, 0x238);                                                       // 004BD7AD (EDX)
    float lo_x = static_cast<float>(static_cast<double>(point->x) - 10.0f);         // 004BD79F..B5 E-A4
    I best = -1;                                                                    // E-78
    float best_d2 = flt(0x7f7fffffu);                                               // E-74
    U start = 0;                                                                    // EBP
    I rounds = 0x32;                                                                // E-70
    float hi_x = static_cast<float>(static_cast<double>(point->x) + 10.0f);         // E-98
    float lo_y = static_cast<float>(static_cast<double>(point->y) - 10.0f);         // E-84
    float hi_y = static_cast<float>(static_cast<double>(point->y) + 10.0f);         // E-7C
    float skip_hi_x = static_cast<float>(static_cast<double>(point->x) + 0.01f);    // E-A0
    float skip_lo_x = static_cast<float>(static_cast<double>(point->x) - 0.01f);    // E-9C
    float skip_hi_y = static_cast<float>(static_cast<double>(point->y) + 0.01f);    // E-8C
    float skip_lo_y = static_cast<float>(static_cast<double>(point->y) - 0.01f);    // E-80
    const float last_x = points[n * 4 - 4];                                         // 004BD82E..35 E-90 (fld/fstp)
    const float radius_value = at<float>(pf, 4);                                    // 004BD832..39 E-68
    for (;;) {
        const I c = rounds--;                                                       // 004BD840..49
        if (c <= 0) break;                                                          // 004BD84D
        if (start < n) {                                                            // 004BD853..57 (jae)
            U i = start;                                                            // EBX
            do {
                const float* e = at<const float*>(area, 0x23c) + i * 4;             // 004BD870..79
                if (e[0] < lo_x) {                                                  // 004BD876..84 (test ah,5; jp)
                    start = i;                                                      // 004BD886 (last index left of the box)
                } else {
                    // 004BD88D..C1: skip the box searched in the previous round
                    const bool inside_old = e[0] > skip_hi_x && e[0] < skip_lo_x && e[1] > skip_hi_y && e[1] < skip_lo_y;
                    if (!inside_old) {
                        if (e[0] > hi_x) break;                                     // 004BD8C7..D2 (sorted by x)
                        if (!(e[1] < lo_y) && !(e[1] > hi_y)) {                     // 004BD8D8..F6
                            const double ex = static_cast<double>(e[0]) - point->x; // 004BD8FC..FE
                            const double ey = static_cast<double>(e[1]) - point->y; // 004BD900..03
                            const double d2 = ey * ey + ex * ex;                    // 004BD906..0E
                            const float d2f = static_cast<float>(d2);               // 004BD910 fst E-6C
                            if (d2 < best_d2) {                                     // 004BD918..21 (test ah,5; jp)
                                const Vector from = {point->x, point->y, 0.0f};     // E-60
                                const Vector to = {e[0], e[1], 0.0f};               // E-54
                                unsigned char hit[0x48];                            // E-48 CWalkHitInfo
                                w5_walk_hit_construct_004b14b0(hit, nullptr);       // 004BD942..5A
                                const float height = at<float>(at<void*>(area, 0x1ac), 0x14); // 004BD95F..65
                                const float radius = radius_value;                  // E-68
                                const I r = test_line(area, &from, &to, &radius, height, 1, hit); // 004BD968..87
                                if (r == 1 || r == -3) { best_d2 = d2f; best = static_cast<I>(i); } // 004BD98C..9E
                            }
                        }
                    }
                }
                ++i;                                                                // 004BD9A2..AC
                n = at<U>(area, 0x238);                                             // 004BD9A6 (re-read)
            } while (i < n);                                                        // 004BD9B0..B6
            if (best != -1) break;                                                  // 004BD9BC..C1
        }
        // 004BD9C7..AB1: grow the box by 10 in every direction (clamped to the first/last x)
        double grown = static_cast<double>(lo_x) - 10.0f;
        lo_x = static_cast<float>(grown);
        if (!(grown > first_x)) lo_x = first_x;                                     // 004BD9D5..E4 (test ah,41h; je)
        grown = static_cast<double>(hi_x) + 10.0f;
        hi_x = static_cast<float>(grown);
        if (!(grown < last_x)) hi_x = last_x;                                       // 004BD9F6..05 (test ah,5; jnp)
        lo_y = static_cast<float>(static_cast<double>(lo_y) - 10.0f);               // 004BDA09..13
        hi_y = static_cast<float>(static_cast<double>(hi_y) + 10.0f);               // 004BDA17..21
        grown = static_cast<double>(skip_hi_x) - 10.0f;
        skip_hi_x = static_cast<float>(grown);
        if (!(grown > first_x)) skip_hi_x = first_x;                                // 004BDA33..42
        grown = static_cast<double>(skip_lo_x) + 10.0f;
        skip_lo_x = static_cast<float>(grown);
        if (!(grown < last_x)) skip_lo_x = last_x;                                  // 004BDA54..63
        skip_hi_y = static_cast<float>(static_cast<double>(skip_hi_y) - 10.0f);     // 004BDA69..73
        skip_lo_y = static_cast<float>(static_cast<double>(skip_lo_y) + 10.0f);     // 004BDA77..81
        if (static_cast<I>(start) > 0) {                                            // 004BDA67 / 85 (jle)
            I s = static_cast<I>(start);
            const float* e = at<const float*>(area, 0x23c) + s * 4;                 // 004BDA8B..96
            do {
                if (!(e[0] > lo_x)) break;                                          // 004BDA98..A3 (test ah,41h; jne)
                --s; e -= 4;                                                        // 004BDAA9..AA
            } while (s > 0);                                                        // 004BDAAD..AF
            start = static_cast<U>(s);
        }
    }
    *out = best;                                                                    // 004BDAB6..CC
    return best != -1 ? 1u : 0u;                                                    // 004BDAC1..C6 setne
}

// 004BDB00 CSWSArea::PathPointDFSGenerateSuccessors(g, limit, node, out): RET 10h, EAX count. Entries {node,
// cost, heuristic} (12 bytes), sorted by heuristic; g and limit are not read.
I __fastcall kotor_fastcall_004bdb00(void* area, void*, float g, I limit, I node, void** out) {
    (void)g; (void)limit;
    const float radius = at<float>(at<void*>(area, 0x1ac), 4);                      // 004BDB13..19 E-58
    const U* entry = at<const U*>(area, 0x23c) + node * 4;                          // 004BDB1C..29
    const U first = entry[3];                                                       // 004BDB25 (EBP)
    const U links = entry[2];                                                       // 004BDB2C E-9C
    I count = 0;                                                                    // E-80
    *out = live_operator_new(links * 12);                                           // 004BDB33..52
    const float* here_entry = at<const float*>(area, 0x23c) + node * 4;
    Vector here;                                                                    // E-8C
    here.x = here_entry[0];                                                         // 004BDB5E (dword)
    here.y = here_entry[1];                                                         // 004BDB5A..63 (fld/fstp)
    here.z = 0.0f;                                                                  // 004BDB78
    Vector next = {0.0f, 0.0f, 0.0f};                                               // E-98..E-90
    const U end = links + first;                                                    // 004BDB67..6B E-4C
    U offset = 0;                                                                   // E-7C (EBP)
    for (U j = first; j < end; ++j) {                                               // 004BDB70..A0 / 004BDF54..63
        const U neighbour = at<const U*>(area, 0x244)[j];                           // 004BDBB0..BF
        const float* np = at<const float*>(area, 0x23c) + neighbour * 4;
        next.x = np[0];                                                             // 004BDBC4..CD (fld/fstp)
        next.y = np[1];                                                             // 004BDBDA..E3
        float detour = 0.0f;                                                        // E-78
        U blocker = 0;                                                              // E-54
        const I clear = w5x_area_no_creatures_on_line_004bc590(area, nullptr, &here, &next,
            ptr<void>(at<U>(area, 0x2cc)), 1, 0, static_cast<I>(addr(&blocker)), 0, nullptr); // 004BDBD3..C00
        if (clear == 0) {
            // 004BDC0D..D2A: route around the blocking creature, sideways by both radii
            const Vector side_in = {static_cast<float>(-(static_cast<double>(next.y) - here.y)),
                                    static_cast<float>(static_cast<double>(next.x) - here.x),
                                    static_cast<float>(static_cast<double>(next.z) - here.z)}; // 004BDC0D..3E
            Vector side;                                                            // E-C -> E-70
            normalize_004ab180(&side, &side_in);                                    // 004BDC42..64
            void* creature = creature_by_id(blocker);                               // 004BDC50..6C
            if (creature == nullptr) continue;                                      // 004BDC71..73 (no successor)
            const double offset_len = static_cast<double>(at<float>(at<void*>(creature, 0x340), 4)) + radius; // 004BDC79..87
            Vector around = {static_cast<float>(static_cast<double>(side.x) * offset_len),
                             static_cast<float>(static_cast<double>(side.y) * offset_len),
                             static_cast<float>(static_cast<double>(side.z) * offset_len)}; // 004BDC8B..AA E-64
            vector_add_assign_004a9740(&around, nullptr, reinterpret_cast<const Vector*>(bytes(creature) + 0x90)); // 004BDC82..B0
            around.z = 0.0f;                                                        // 004BDCC2
            const Vector first_leg = {static_cast<float>(static_cast<double>(around.x) - here.x),
                                      static_cast<float>(static_cast<double>(around.y) - here.y),
                                      -here.z};                                     // 004BDCB5..E6 E-30
            const Vector second_leg = {static_cast<float>(static_cast<double>(next.x) - around.x),
                                       static_cast<float>(static_cast<double>(next.y) - around.y),
                                       next.z};                                     // 004BDCED..09 E-48
            const float second = static_cast<float>(kotor_cdecl_004ab100(&second_leg)); // 004BDD0D..12 E-9C
            detour = static_cast<float>(kotor_cdecl_004ab100(&first_leg) + second); // 004BDD1E..2A
        }
        // 004BDD2E..DA1: walkmesh clearance in a +-1000 height band
        const double low = here.z < next.z ? static_cast<double>(here.z) : static_cast<double>(next.z); // 004BDD2E..43 (test ah,5; jp)
        const float band_low = static_cast<float>(low - 1000.0f);                   // 004BDD47..4D E-9C
        const double high = here.z > next.z ? static_cast<double>(here.z) : static_cast<double>(next.z); // 004BDD51..66 (test ah,41h)
        const float band_high = static_cast<float>(high + 1000.0f);                 // 004BDD6E / 8E
        U wall = 0;                                                                 // E-50
        const U walkable = area_no_nonwalk_polys_detailed_00506650(area, nullptr, &here, &next, band_low, band_high,
            radius, k_object_invalid, &wall, 1, nullptr, 0, nullptr, 0);            // 004BDD78..9E
        if (walkable == 0) {                                                        // 004BDDA3..A5
            void* object = game_object(wall);                                       // 004BDDA7..B4
            if (object != nullptr && vcall<void*>(object, 0x10) != nullptr) {       // 004BDDBB..C8
                void* placeable = vcall<void*>(object, 0x48);                       // 004BDDCA..D1
                void* door = vcall<void*>(object, 0x18);                            // 004BDDD3..D7
                const U is_placeable = placeable != nullptr ? 1u : 0u;              // 004BDDDA..E0
                if (door != nullptr && at<U>(door, 0x3c0) != 0) continue;           // 004BDDE5..F1
                if (is_placeable != 0) continue;                                    // 004BDDF7..F9
            }
        }
        const float* goal = at<const float*>(area, 0x23c) + at<U>(at<void*>(area, 0x1ac), 0x1a8) * 4; // 004BDE03..1F
        const float goal_x = goal[0];                                               // E-3C (dword)
        const float goal_y = goal[1];                                               // E-38 (fld/fstp)
        unsigned char* list = static_cast<unsigned char*>(*out);
        at<U>(list, offset) = neighbour;                                            // 004BDE21..42
        if (!(detour == 0.0f)) {                                                    // 004BDE2D..4A (test ah,44h; jnp)
            at<float>(*out, offset + 4) = detour;                                   // 004BDE4C..52
        } else {
            const Vector leg = {static_cast<float>(static_cast<double>(next.x) - here.x),
                                static_cast<float>(static_cast<double>(next.y) - here.y),
                                static_cast<float>(static_cast<double>(next.z) - here.z)}; // 004BDE58..86 E-18
            at<float>(*out, offset + 4) = static_cast<float>(kotor_cdecl_004ab100(&leg)); // 004BDE8D..94
        }
        const Vector to_goal = {static_cast<float>(static_cast<double>(next.x) - goal_x),
                                static_cast<float>(static_cast<double>(next.y) - goal_y),
                                next.z};                                            // 004BDE9B..C5 E-24
        at<float>(*out, offset + 8) = static_cast<float>(kotor_cdecl_004ab100(&to_goal)); // 004BDECC..DA
        ++count;                                                                    // 004BDED1..E5
        offset += 12;                                                               // 004BDEDF..E9
        if (count > 1) {                                                            // 004BDEE2..ED
            U at_off = offset - 12;                                                 // ECX
            for (I k = count - 1; k != 0; --k) {                                    // 004BDEEF..4C
                unsigned char* base = static_cast<unsigned char*>(*out);
                float* cur = reinterpret_cast<float*>(base + at_off);
                float* prev = cur - 3;
                if (cur[2] <= prev[2]) {                                            // 004BDEF9..07 (test ah,41h; jp keeps)
                    const U idx = reinterpret_cast<U*>(cur)[0];
                    reinterpret_cast<U*>(cur)[0] = reinterpret_cast<U*>(prev)[0];   // 004BDF09..12
                    reinterpret_cast<U*>(prev)[0] = idx;
                    const float c1 = cur[1]; cur[1] = prev[1]; prev[1] = c1;        // 004BDF16..27
                    const float c2 = cur[2]; cur[2] = prev[2]; prev[2] = c2;        // 004BDF2B..3C
                }
                at_off -= 12;                                                       // 004BDF44
            }
        }
    }
    return count;
}

// 004C0B30 CSWSArea::PathPointDFSearch(depth, g, limit, node, goal): RET 14h, EAX 0/1. Iterative-deepening DFS over
// the area path points (+23C {x, y, link count, first link}, links +244); +1AC of the pf holds the node path.
U __fastcall kotor_fastcall_004c0b30(void* area, void*, I depth, float g, float limit, I node, I goal) {
    if (w5_path_debug_draw_008327cc != 0 && w5_path_debug_reset_007a1b20 != 0 &&
        at<U>(pf_of(area), 0x2c) == w5_path_debug_creature_00832924) {              // 004C0B30..5B
        for (U k = 0; k < 500; ++k) w5_path_debug_squares_00831ff0[k] = 0xffffffffu; // 004C0B5D..6A
        w5_path_debug_reset_007a1b20 = 0;                                           // 004C0B6C
        w5_path_debug_square_count_008327d0 = 0;                                    // 004C0B72
    }
    void* pf = pf_of(area);                                                         // 004C0B78
    at<I>(pf, 0x1b0) = depth + 1;                                                   // 004C0B8C..8F
    at<I*>(pf, 0x1ac)[depth] = node;                                                // 004C0B7E / 95
    const I next_depth = depth + 1;                                                 // E-58
    const float* points = at<const float*>(area, 0x23c);                            // 004C0B98
    float here_x = points[node * 4];                                                // 004C0BAE E-48 (dword)
    float here_y = points[node * 4 + 1];                                            // 004C0BA7..BC E-44 (fld/fstp)
    const float here_z = 0.0f;                                                      // E-40
    const Vector to_goal = {static_cast<float>(static_cast<double>(points[goal * 4]) - here_x),
                            static_cast<float>(static_cast<double>(points[goal * 4 + 1]) - here_y), 0.0f}; // 004C0BC0..EC E-3C
    const double h = kotor_cdecl_004ab100(&to_goal);                                // 004C0BF0
    if (at<float>(pf_of(area), 0x1d4) > h) {                                        // 004C0BF5..0B (test ah,41h; jne)
        // +1D4 is compared but +20C is written (+1D4 keeps FLT_MAX after a reset, so every visit snapshots)
        at<float>(pf_of(area), 0x20c) = static_cast<float>(h);                      // 004C0C0D
        std::memcpy(at<void*>(pf_of(area), 0x1d8), at<void*>(pf_of(area), 0x1ac), static_cast<U>(depth) * 4 + 4); // 004C0C13..3C
        at<I>(pf_of(area), 0x1dc) = next_depth;                                     // 004C0C3E..44
    }
    if (node == goal) return 1;                                                     // 004C0C4A..61
    if (limit < h) return 0;                                                        // 004C0C64..71 (test ah,5; jnp)
    float stored_g = 0.0f, stored_limit = 0.0f;                                     // depth slot / node slot
    if (kotor_fastcall_004bbf20(area, nullptr, here_x, here_y, &stored_g, &stored_limit) != 0) { // 004C0C73..90
        if (g > stored_g) return 0;                                                 // 004C0C92..9F (test ah,41h; je)
        if (stored_limit >= limit) return 0;                                         // 004C0CA1..AE (test ah,1; jne continues)
    }
    const I visits = at<I>(area, 0x254) + 1;                                        // 004C0CBC..C2
    at<I>(area, 0x254) = visits;                                                    // 004C0CCA
    if (visits >= 0x1000) at<U>(area, 0x248) = 1;                                   // 004C0CC5..D2
    void* successors = nullptr;                                                     // E-4C
    const I count = kotor_fastcall_004bdb00(area, nullptr, g, ftol2(limit), node, &successors); // 004C0CDC..F8 E-50
    kotor_fastcall_004bc010(area, nullptr, here_x, here_y, g, limit);               // 004C0CFC..11
    U result = 0;                                                                   // node slot, zeroed 004C0D06
    const unsigned char* cur = static_cast<const unsigned char*>(successors);       // depth slot
    for (I i = 0; i < count; ++i, cur += 12) {                                      // 004C0D16..24 / 004C0F5F..79
        if (result != 0) break;                                                     // 004C0D40..46
        if (at<U>(area, 0x248) != 0) break;                                         // 004C0D4C..54
        const float cost = *reinterpret_cast<const float*>(cur + 4);                // 004C0D5A..5D E-5C
        const I to = *reinterpret_cast<const I*>(cur);
        const bool draw = w5_path_debug_draw_008327cc != 0 &&
                          at<U>(pf_of(area), 0x2c) == w5_path_debug_creature_00832924; // 004C0D61..7E
        if (draw) {
            // 004C0D84..EDC: debug square from this node to the successor (four corners snapped to the walkmesh)
            const float* np = at<const float*>(area, 0x23c) + to * 4;
            const float succ_x = np[0];                                             // E-48 (overwritten)
            const float succ_y = np[1];                                             // E-44
            float corners[12] = {here_x, here_y, here_z, here_x, here_y, here_z,
                                 succ_x, succ_y, 0.0f, succ_x, succ_y, 0.0f};       // E-30..E-4
            here_x = succ_x; here_y = succ_y;                                       // 004C0D9A..AE (E-48/E-44 reused)
            for (U k = 0; k < 4; ++k) {                                             // 004C0DE8..EB7
                void* module_area = module_get_area_004c30f0(server_get_module_004ae6b0(server_app(), nullptr), nullptr);
                corners[k * 3 + 2] = static_cast<float>(w5_server_compute_height_004aea50(server_app(), nullptr,
                    bytes(module_area) + 0x10c, reinterpret_cast<Vector*>(&corners[k * 3]), 1));
            }
            const U handle = aur_add_drawable_square_0044efb0(corners, 0xffffffffu); // 004C0EBB..C2
            w5_path_debug_squares_00831ff0[w5_path_debug_square_count_008327d0] = handle; // 004C0EC7..CD
            ++w5_path_debug_square_count_008327d0;                                  // 004C0EDB..DC
        }
        result = kotor_fastcall_004c0b30(area, nullptr, next_depth, static_cast<float>(static_cast<double>(cost) + g),
                                         static_cast<float>(static_cast<double>(limit) - cost), to, goal); // 004C0EE2..16
        if (w5_path_debug_draw_008327cc != 0 && at<U>(pf_of(area), 0x2c) == w5_path_debug_creature_00832924) { // 004C0F1A..32
            // removes the slot after the last added square (an unused -1 handle), then steps the count back
            const U idx = w5_path_debug_square_count_008327d0;                      // 004C0F34
            aur_remove_drawable_square_0044f050(w5_path_debug_squares_00831ff0[idx]); // 004C0F39..41
            w5_path_debug_squares_00831ff0[idx] = 0xffffffffu;                      // 004C0F4E
            w5_path_debug_square_count_008327d0 = idx - 1;                          // 004C0F59..5A
        }
    }
    live_free(successors);                                                          // 004C0F7F..84
    return result;
}

// 004C2200 CSWSArea::PlotPathPointPath(pf, budget lo, budget hi): RET 0Ch. State machine on pf +1E0:
// 1 locate start/goal path points, 4 time-sliced graph search, 2 grid leg to the first point, 3 grid leg from the
// last point, 5 assemble the polyline and run the restartable smoothing.
U __fastcall kotor_fastcall_004c2200(void* area, void*, void* pf, U budget_lo, U budget_hi) {
    Vector start = {at<float>(pf, 0x74), at<float>(pf, 0x78), at<float>(pf, 0x7c)}; // 004C2209..23 E-24 (dwords)
    Vector goal = {at<float>(pf, 0x5c), at<float>(pf, 0x60), at<float>(pf, 0x64)};  // 004C2219..37 E-18
    I iterations = 0;                                                               // E-3C
    std::uint64_t t0 = hr_timer();                                                  // 004C222E..5C E-2C/E-28
    const std::uint64_t budget = (static_cast<std::uint64_t>(budget_hi) << 32) | budget_lo;
    U search_result = 0;                                                            // E-44: only written by state 4
                                                                                    // (states 1..5 always pass it; any other
                                                                                    // state returns the original's stale slot)
    I start_node, goal_node;                                                        // E-38 (EBX) / the pf argument slot
    if (at<U>(pf, 0x1e0) == k_pp_init_0074c42c) {                                   // 004C224C..60
        const U start_safe = w2x_area_safe_point_004be5e0(area, nullptr, &start, addr(pf)); // 004C2266..73
        const U goal_safe = w2x_area_safe_point_004be5e0(area, nullptr, &goal, addr(pf));   // 004C2275..84
        if (start_safe == 0) {                                                      // 004C2282..86
            const Vector before = start;                                            // E-C
            w1x_area_safe_location_004be860(area, nullptr, addr(&start), 0x40000000u, addr(pf), 1, addr(&start)); // 004C229C..B8
            if (aurora_vector_equal_004aa980(&start, &before) == 0) {               // 004C22BD..D1
                void* creature = creature_by_id(at<U>(pf, 0x2c));                   // 004C22D3..E0
                if (creature != nullptr) recovered_object_position_004cd1a0(creature, nullptr, &start, 1, 1, start_safe); // 004C22E7..F5
            }
            copy_vector(bytes(pf) + 0x74, &start);                                  // 004C22FA..13
            (void)w5_area_position_walkable_00506400(area, nullptr, start);         // 004C230C..2A
        }
        if (goal_safe == 0) {                                                       // 004C232F..31
            w1x_area_safe_location_004be860(area, nullptr, addr(&goal), 0x40000000u, addr(pf), 1, addr(&goal)); // 004C2333..47
            copy_vector(bytes(pf) + 0x5c, &goal);                                   // 004C234C..6D
            (void)w5_area_position_walkable_00506400(area, nullptr, goal);          // 004C235E..7C
        }
        if (kotor_fastcall_004bd770(area, nullptr, &start, &start_node) == 0 ||     // 004C2381..94
            kotor_fastcall_004bd770(area, nullptr, &goal, &goal_node) == 0) {       // 004C2396..A9
            debug_print(pf, 2, k_no_closest_00745764);                              // 004C2404..0D
            at<U>(pf, 0x1e0) = k_pp_init_0074c42c;                                  // 004C2412..17
            return k_failed_00745dc4;                                               // 004C241D
        }
        at<I>(pf, 0x1a4) = start_node; at<I>(pf, 0x1a8) = goal_node;                // 004C23AB..B9
        at<U>(pf, 0x1e0) = k_pp_search_0074c438;                                    // 004C23BF..C5
        if (hr_timer() - t0 > budget) return k_in_progress_00745dbc;                // 004C23CB..F5 (strictly above)
    } else {
        start_node = at<I>(pf, 0x1a4);                                              // 004C242C..38
        goal_node = at<I>(pf, 0x1a8);                                               // 004C2432..3C
    }
    if (start_node == goal_node && at<U>(pf, 0x1e0) == k_pp_search_0074c438) {      // 004C2440..52
        at<I>(pf, 0x1b0) = 1;                                                       // 004C2456
        I* path = static_cast<I*>(live_operator_new(4));                            // 004C2454..60
        path[0] = start_node;                                                       // 004C246B
        kotor_fastcall_004bb340(pf, nullptr, path);                                 // 004C2468..6D
        at<U>(pf, 0x1e0) = k_pp_verify_start_0074c430;                              // 004C2472..77
    }
    if (at<U>(pf, 0x1e0) == k_pp_search_0074c438) {                                 // 004C247D..89
        const Vector span = {static_cast<float>(static_cast<double>(goal.x) - start.x),
                             static_cast<float>(static_cast<double>(goal.y) - start.y),
                             static_cast<float>(static_cast<double>(goal.z) - start.z)}; // 004C248F..C4 E-C
        at<I>(pf, 0x1a4) = start_node; at<I>(pf, 0x1a8) = goal_node;                // 004C24A0..AA
        const float distance = static_cast<float>(kotor_cdecl_004ab100(&span));     // 004C24C8..CD E-40
        at<U>(area, 0x258) = at<U>(area, 0x238);                                    // 004C24D1..DF
        kotor_fastcall_004bb340(pf, nullptr, live_operator_new(at<U>(area, 0x238) * 4)); // 004C24D7..F0
        void* best = at<void*>(pf, 0x1d8);                                          // 004C24F5 E-30
        if (best == nullptr) {                                                      // 004C24FD..03
            best = live_operator_new(at<U>(area, 0x238) << 2);                      // 004C2505..0F
            at<void*>(pf, 0x1d8) = best;                                            // 004C251B
        }
        // 004C2521..60E: resume the previous slice when start and goal match the saved +1B8/+1C4
        const double a = static_cast<double>(start.x) - at<float>(pf, 0x1b8);
        const double b = static_cast<double>(start.y) - at<float>(pf, 0x1bc);
        const double c = static_cast<double>(start.z) - at<float>(pf, 0x1c0);
        const float ga = static_cast<float>(static_cast<double>(goal.x) - at<float>(pf, 0x1c4)); // E-C
        const float gb = static_cast<float>(static_cast<double>(goal.y) - at<float>(pf, 0x1c8)); // E-8
        const float gc = static_cast<float>(static_cast<double>(goal.z) - at<float>(pf, 0x1cc)); // E-4
        const float quarter = static_cast<float>(static_cast<double>(distance) * 0.25f); // 004C2569..77 E-44 / E-34
        float limit;                                                                // E-48
        if (!((c * c + b * b) + a * a >= 0.0001f) &&                                    // 004C257B..9C
            !((static_cast<double>(gc) * gc + static_cast<double>(gb) * gb) + static_cast<double>(ga) * ga >= 0.0001f)) { // 004C259E..C5
            at<void*>(area, 0x250) = at<void*>(pf, 0x1b4);                          // 004C25C7..CD
            limit = static_cast<float>(static_cast<double>(at<float>(pf, 0x1d0)) + quarter); // 004C25D3..DD
        } else {
            if (at<void*>(pf, 0x1b4) != nullptr) at<void*>(area, 0x250) = at<void*>(pf, 0x1b4); // 004C25E3..ED
            kotor_fastcall_004bbeb0(area, nullptr);                                 // 004C25F3..F5
            limit = static_cast<float>(static_cast<double>(quarter) + 10.0f);       // 004C25FA..04
            at<float>(pf, 0x20c) = limit;                                           // 004C2608
        }
        std::uint64_t elapsed = hr_timer() - t0;                                    // 004C260E..22
        search_result = k_in_progress_00745dbc;                                     // 004C2626..41
        at<U>(area, 0x254) = 0; at<U>(area, 0x248) = 0;                             // 004C262B..31
        if (at<float>(pf, 0x1d4) > distance) at<float>(pf, 0x1d4) = distance;       // 004C2637..50
        bool timed_out = false;
        for (;;) {
            if (iterations != 0) {                                                  // 004C2656..5A
                if (search_result != k_in_progress_00745dbc) break;                 // 004C265C..66
                if (elapsed >= budget) { timed_out = true; break; }                 // 004C266C..7C
            }
            ++iterations;                                                           // 004C267E..8B
            limit = static_cast<float>(static_cast<double>(limit) + quarter);       // 004C2682..94
            at<U>(area, 0x254) = 0; at<U>(area, 0x248) = 0;                         // 004C26A2..A8
            at<void*>(area, 0x2cc) = pf;                                            // 004C26AE
            if (kotor_fastcall_004c0b30(area, nullptr, 0, 0.0f, limit, start_node, goal_node) == 1) // 004C269E..BC
                search_result = k_complete_00745dc0;                                // 004C26BE..C3
            if (at<U>(area, 0x248) != 0) {                                          // 004C26C7..CD
                debug_print(at<void*>(area, 0x2cc), 0, k_levels_0074573c);          // 004C2826..32
                return k_failed_00745dc4;                                           // 004C2837
            }
            at<U>(area, 0x2cc) = 0;                                                 // 004C26D3
            elapsed = hr_timer() - t0;                                              // 004C26D9..ED
        }
        if (timed_out) {                                                            // 004C26F6..74E: save the slice
            copy_vector(bytes(pf) + 0x1b8, &start);
            copy_vector(bytes(pf) + 0x1c4, &goal);
            at<float>(pf, 0x1d0) = limit;                                           // 004C2736
            at<void*>(pf, 0x1b4) = at<void*>(area, 0x250);                          // 004C273C..42
            at<void*>(area, 0x250) = nullptr;                                       // 004C2748
        } else {
            live_free(at<void*>(area, 0x250));                                      // 004C2753..5A
            at<void*>(area, 0x250) = nullptr;                                       // 004C275F
            for (U off = 0x1b8; off <= 0x1cc; off += 4) at<U>(pf, off) = 0;         // 004C2765..CE
            at<U>(pf, 0x1d0) = 0; at<U>(pf, 0x1b4) = 0;                             // 004C27D4..DA
        }
        if (search_result != k_in_progress_00745dbc) {                              // 004C27E0..EA
            live_free(best);                                                        // 004C27EC..F1
            at<void*>(pf, 0x1d8) = nullptr;                                         // 004C27F9
        }
        if (search_result == k_complete_00745dc0) {                                 // 004C27FF..09
            at<U>(pf, 0x1e0) = k_pp_verify_start_0074c430;                          // 004C280B..11
            return k_in_progress_00745dbc;                                          // 004C2817
        }
    }
    if (at<U>(pf, 0x1e0) == k_pp_verify_start_0074c430) {                           // 004C2846..52
        const float* first = at<const float*>(area, 0x23c) + start_node * 4;        // 004C2858..63 (EDI = node*16)
        const Vector node = {first[0], first[1], 0.0f};                             // 004C2863..B3 E-C
        U leg = k_complete_00745dc0;                                                // E-3C -> EDI
        at<U>(pf, 0x210) = start_node != goal_node ? 1u : 0u;                       // 004C2876..99
        const float radius = at<float>(pf, 4);                                      // E-34
        const float height = at<float>(pf, 0x14);                                   // E-30
        if (test_line(area, &start, &node, &radius, height, 1, nullptr) == 1) {     // 004C28A6..C3
            float* seg = static_cast<float*>(live_operator_new(0x10));              // 004C296F..71
            seg[0] = start.x; seg[1] = start.y;                                     // 004C2976..80
            seg[2] = (at<const float*>(area, 0x23c) + start_node * 4)[0];           // 004C2983..8C
            seg[3] = (at<const float*>(area, 0x23c) + start_node * 4)[1];           // 004C298F..9D
            kotor_fastcall_004bb400(pf, nullptr, 4, seg);                           // 004C299C..A4
            leg = k_complete_00745dc0;                                              // 004C29A9 (EDI = [E-3C])
        } else {
            kotor_fastcall_004bb3c0(pf, nullptr, &start, &goal);                    // 004C28C9..D5
            at<float>(pf, 0x5c) = (at<const float*>(area, 0x23c) + start_node * 4)[0]; // 004C28DA..EB
            at<float>(pf, 0x60) = (at<const float*>(area, 0x23c) + start_node * 4)[1]; // 004C28EE..FA
            at<U>(pf, 0x1c) = 1;                                                    // 004C2900
            leg = kotor_fastcall_004c1a00(area, nullptr, pf, budget_lo, budget_hi); // 004C28F8..07
            at<U>(pf, 0x1c) = 0;                                                    // 004C291C
            kotor_fastcall_004bb380(pf, nullptr, &start, &goal);                    // 004C290C..1F
            at<float>(pf, 0x5c) = goal.x; at<float>(pf, 0x60) = goal.y;             // 004C2924..2F
            if (leg == k_complete_00745dc0) {                                       // 004C2932..38
                void* grid_pf = at<void*>(area, 0x1ac);                             // 004C293A
                kotor_fastcall_004bb400(pf, nullptr, at<I>(grid_pf, 0x94), at<void*>(grid_pf, 0x98)); // 004C2940..50
                at<U>(at<void*>(area, 0x1ac), 0x94) = 0;                            // 004C2955..5B
                at<U>(at<void*>(area, 0x1ac), 0x98) = 0;                            // 004C2961..67
            }
        }
        at<U>(pf, 0x210) = 0;                                                       // 004C29AF
        if (leg == k_complete_00745dc0) {                                           // 004C29B5..BB
            const std::uint64_t now = hr_timer();                                   // 004C29BD..C6
            at<U>(pf, 0x1e0) = k_pp_verify_end_0074c434;                            // 004C29CB..D1
            if (now - t0 > budget) return k_in_progress_00745dbc;                   // 004C29D7..EF
        }
        if (leg == k_failed_00745dc4) {                                             // 004C29F5..FB: drop the first path node
            const I n = at<I>(pf, 0x1b0);                                           // 004C29FD
            if (n > 1) {                                                            // 004C2A03..06
                I* path = at<I*>(pf, 0x1ac);                                        // 004C2A08
                std::memmove(path, path + 1, static_cast<U>(n - 1) * 4);            // 004C2A0E..16 rep movsd (forward)
                at<I>(pf, 0x1b0) = n - 1;                                           // 004C2A18..19
                at<I>(pf, 0x1a4) = path[0];                                         // 004C2A1F..21
                at<U>(pf, 0x1e0) = k_pp_verify_start_0074c430;                      // 004C2A27..2C
                return k_in_progress_00745dbc;                                      // 004C2A32
            }
            at<U>(pf, 0x1e0) = k_pp_init_0074c42c;                                  // 004C2A41..4C
            debug_print(pf, 0, k_start_grid_007456e0);                              // 004C2A47..55
            return k_failed_00745dc4;                                               // 004C2A5A
        }
        if (leg == k_in_progress_00745dbc) return leg;                              // 004C2A69..70
    }
    if (at<U>(pf, 0x1e0) == k_pp_verify_end_0074c434) {                            // 004C2A76..82
        const float* last = at<const float*>(area, 0x23c) + goal_node * 4;          // 004C2A88..A0
        const Vector node = {last[0], last[1], 0.0f};                               // 004C2A9D..CF E-C
        U leg = k_complete_00745dc0;                                                // EBX
        at<U>(pf, 0x210) = start_node != goal_node ? 1u : 0u;                       // 004C2AAE..D7
        const float radius = at<float>(pf, 4);                                      // E-30
        const float height = at<float>(pf, 0x14);                                   // the goal-node slot
        const I r = test_line(area, &goal, &node, &radius, height, 1, nullptr);    // 004C2ADF..ED
        if (r == 1 || r == -3) {                                                    // 004C2AF2..FE
            float* seg = static_cast<float*>(live_operator_new(0x10));              // 004C2BAE..B0
            seg[0] = (at<const float*>(area, 0x23c) + goal_node * 4)[0];            // 004C2BB5..BE
            seg[1] = (at<const float*>(area, 0x23c) + goal_node * 4)[1];            // 004C2BC0..CA
            seg[2] = goal.x; seg[3] = goal.y;                                       // 004C2BCD..E0
            kotor_fastcall_004bb440(pf, nullptr, 4, seg);                           // 004C2BDB..E3
        } else {
            kotor_fastcall_004bb3c0(pf, nullptr, &start, &goal);                    // 004C2B04..10
            at<float>(pf, 0x74) = (at<const float*>(area, 0x23c) + goal_node * 4)[0]; // 004C2B15..26
            at<float>(pf, 0x78) = (at<const float*>(area, 0x23c) + goal_node * 4)[1]; // 004C2B29..35
            at<U>(pf, 0x1c) = 1;                                                    // 004C2B3B
            leg = kotor_fastcall_004c1a00(area, nullptr, pf, budget_lo, budget_hi); // 004C2B33..42
            at<U>(pf, 0x1c) = 0;                                                    // 004C2B55
            kotor_fastcall_004bb380(pf, nullptr, &start, &goal);                    // 004C2B47..5C
            at<float>(pf, 0x74) = start.x; at<float>(pf, 0x78) = start.y;           // 004C2B61..6C
            if (leg == k_complete_00745dc0) {                                       // 004C2B6F..75
                void* grid_pf = at<void*>(area, 0x1ac);                             // 004C2B77
                kotor_fastcall_004bb440(pf, nullptr, at<I>(grid_pf, 0x94), at<void*>(grid_pf, 0x98)); // 004C2B7D..8D
                at<U>(at<void*>(area, 0x1ac), 0x94) = 0;                            // 004C2B92..9A
                at<U>(at<void*>(area, 0x1ac), 0x98) = 0;                            // 004C2BA0..A6
            }
        }
        at<U>(pf, 0x210) = 0;                                                       // 004C2BE8
        if (leg == k_complete_00745dc0) {                                           // 004C2BF2..F8
            const std::uint64_t now = hr_timer();                                   // 004C2BFA..02
            at<U>(pf, 0x1e0) = k_pp_assemble_0074c43c;                              // 004C2C07..11
            at<U>(pf, 0x214) = k_assemble_pending_0074c440;                         // 004C2C17..1D
            if (now - t0 > budget) return k_in_progress_00745dbc;                   // 004C2C23..37
        }
        if (leg == k_failed_00745dc4) {                                             // 004C2C39..3F: drop the last path node
            I n = at<I>(pf, 0x1b0);                                                 // 004C2C41
            if (n > 1) {                                                            // 004C2C47..4A
                --n;                                                                // 004C2C52
                at<I>(pf, 0x1b0) = n;                                               // 004C2C53
                at<I>(pf, 0x1a8) = at<I*>(pf, 0x1ac)[n - 1];                        // 004C2C4C / 59..5D
                at<U>(pf, 0x1e0) = k_pp_verify_end_0074c434;                        // 004C2C63..69
                return k_in_progress_00745dbc;                                      // 004C2C6F
            }
            at<U>(pf, 0x1e0) = k_pp_init_0074c42c;                                  // 004C2C7E..8D
            debug_print(pf, 0, k_end_grid_00745688);                                // 004C2C84..93
            return k_failed_00745dc4;                                               // 004C2C98
        }
        if (leg == k_in_progress_00745dbc) return leg;                              // 004C2CA7..AE
    }
    if (at<U>(pf, 0x1e0) != k_pp_assemble_0074c43c) return search_result;           // 004C2CB4..C0 / 004C301C
    if (at<U>(pf, 0x214) == k_assemble_pending_0074c440) {                          // 004C2CC6..D2
        t0 = hr_timer();                                                            // 004C2CD8..F2 / 0E (restart the clock)
        const I* nodes = at<const I*>(pf, 0x1ac);                                   // 004C2CE6 E-3C
        const I n = at<I>(pf, 0x1b0);                                               // 004C2CF6 (arg1 slot)
        const I head = at<I>(pf, 0x1fc);                                            // 004C2D00 E-30
        const I tail = at<I>(pf, 0x204);                                            // 004C2CEC (arg2 slot)
        const float* head_points = at<const float*>(pf, 0x200);                     // E-38
        const float* tail_points = at<const float*>(pf, 0x208);                     // E-34
        const I total = tail + n * 2 - 4 + head;                                    // 004C2D0A..18 E-40
        U* out = static_cast<U*>(live_operator_new(static_cast<U>(total) * 4));     // 004C2D28..3C (EDI)
        out[0] = (static_cast<U>(total) & 0xffu) * 0x01010101u;                     // 004C2D41..56 (inlined byte fill)
        I w = 0;                                                                    // EAX
        const I head_copy = head - 2;                                               // 004C2D58..5F
        if (head_copy > 0) {                                                        // 004C2D61
            for (I k = 0; k < head_copy; ++k) out[k] = reinterpret_cast<const U*>(head_points)[k]; // 004C2D63..8A
            w = head_copy;                                                          // 004C2D73
        }
        const float* pts = at<const float*>(area, 0x23c);
        for (I i = 0; i < n; ++i) {                                                 // 004C2D8C..EB9 (unrolled by 4)
            out[w++] = reinterpret_cast<const U*>(at<const float*>(area, 0x23c) + nodes[i] * 4)[0];
            out[w++] = reinterpret_cast<const U*>(at<const float*>(area, 0x23c) + nodes[i] * 4)[1];
        }
        (void)pts;
        for (I k = 2; k < tail; ++k) out[w++] = reinterpret_cast<const U*>(tail_points)[k]; // 004C2EBF..F33 (unrolled by 4)
        at<U>(pf, 0x1fc) = 0;                                                       // 004C2F3F
        if (at<void*>(pf, 0x200) != nullptr) live_free(at<void*>(pf, 0x200));      // 004C2F35..4D
        at<U>(pf, 0x200) = 0; at<U>(pf, 0x204) = 0;                                 // 004C2F5A..60
        if (at<void*>(pf, 0x208) != nullptr) live_free(at<void*>(pf, 0x208));      // 004C2F52..6E
        at<U>(pf, 0x208) = 0;                                                       // 004C2F7B
        at<I>(pf, 0x94) = total;                                                    // 004C2F81
        at<U*>(pf, 0x98) = out;                                                     // 004C2F87
        if (at<void*>(pf, 0x1ac) != nullptr) { live_free(at<void*>(pf, 0x1ac)); at<U>(pf, 0x1ac) = 0; } // 004C2F73..98
        at<U>(pf, 0x1ac) = 0;                                                       // 004C2FA2
        at<U>(pf, 0x214) = k_assemble_done_0074c444;                                // 004C2FAC..B2
        const std::uint64_t slice = static_cast<std::uint64_t>(static_cast<std::int64_t>(w5_path_smooth_budget_007a1b1c)); // cdq
        if (hr_timer() - t0 > slice) return 1;                                      // 004C2FB8..EF (EAX = 1 immediate)
    }
    if (kotor_fastcall_004c17a0(area, nullptr) != 0) return k_in_progress_00745dbc; // 004C2FF2..FB / 004C2C6F
    at<U>(pf, 0x1e0) = k_pp_init_0074c42c;                                          // 004C3001..07
    return k_complete_00745dc0;                                                     // 004C300D
}

}  // extern "C"
