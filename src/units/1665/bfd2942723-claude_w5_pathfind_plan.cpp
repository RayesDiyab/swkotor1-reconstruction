// W5 lane "pathfind-plan" part 1/3: CSWSModule::PlotPath 004C6F70, the inter-area layer, PlotPathInArea, the
// CPathfindInformation helpers (FlipStart/TempEndPoints, the FUN_004BB3xx buffer setters), ComputeTotalWalkDistance,
// ComputePathDistance and the two DFS transposition tables. Transcribed from the original swkotor.exe (read-only);
// addresses in the comments are the original instructions.
#include "eb5e5e70a0-claude_w5_pathfind_plan.hpp"

using namespace w5;
using namespace w5pp;

extern "C" {
// ---------------- lane-owned storage (original initial values) ----------------
U w5_path_debug_draw_008327cc = 0;
U w5_path_debug_reset_007a1b20 = 1;
U w5_path_debug_creature_00832924 = 0;
U w5_path_debug_squares_00831ff0[500] = {};
U w5_path_debug_square_count_008327d0 = 0;
I w5_path_smooth_budget_007a1b1c = 10000;
U w5_path_smooth_stat_00832870 = 0;
U w5_path_smooth_stat_00832840 = 0;

// =========================================================================================================
// 004C6F70 CSWSModule::PlotPath(pf, budget): RET 8, EAX disposition
U __fastcall kotor_fastcall_004c6f70(void* module, void*, void* pf, U budget) {
    if (pf == nullptr) return k_failed_00745dc4;                                    // 004C6F72..87
    const U restart = at<U>(pf, 0x58);                                              // 004C6F8A
    if (restart == 0 && at<U>(pf, 0x88) == 1) return k_complete_00745dc0;           // 004C6F8D..A2
    if (restart == 1) {                                                             // 004C6FA5..A8
        if (at<U>(pf, 0xa4) == 1) {                                                 // 004C6FAA..B3
            w5_pathfind_reset_inter_tile_search_005d00a0(pf, nullptr);              // 004C6FB5
            w5_pathfind_reset_grid_search_005cefa0(pf, nullptr);                    // 004C6FBC
            at<U>(pf, 0xa4) = 0;                                                    // 004C6FC1
        } else {
            const U keep = at<U>(pf, 0x84);                                         // 004C6FC9
            kotor_fastcall_005d0e70(pf, nullptr);                                   // 004C6FCF
            at<U>(pf, 0x84) = keep;                                                 // 004C6FD4
        }
        at<U>(pf, 0x3c) = 0; at<U>(pf, 0x88) = 0; at<U>(pf, 0x58) = 0;              // 004C6FDC..E5
    }
    at<U>(recovered_server_ai_master_004aed80(server_app(), nullptr), 0x68) = 1;    // 004C6FE8..FD
    const U result = kotor_fastcall_004c6e20(module, nullptr, pf, budget);          // 004C6FF5..09
    if (recovered_debug_cell_00832838 != 0) {                                       // 004C700B..12
        void* creature = creature_by_id(at<U>(pf, 0x2c));                           // 004C7014..26
        if (result == k_complete_00745dc0) {                                        // 004C7028..2E
            (void)kotor_fastcall_005111c0(creature, nullptr);                       // 004C7030..37 (fstp st0)
            (void)kotor_fastcall_005111c0(creature, nullptr);                       // 004C7039..40
        }
    }
    return result;
}

// 004C6E20 CSWSModule::PlotInterAreaPath(pf, budget): RET 8
U __fastcall kotor_fastcall_004c6e20(void* module, void*, void* pf, U budget) {
    if (at<U>(pf, 0x70) == at<U>(pf, 0x80)) {                                       // 004C6E27..3D (same area)
        if (at<void*>(pf, 0xac) == nullptr) {                                       // 004C6E3F..41
            at<U>(pf, 0xa0) = 1; at<U>(pf, 0xa8) = 1;                               // 004C6E45..4B
            U* list = static_cast<U*>(live_operator_new(4));                        // 004C6E43..51
            list[0] = at<U>(pf, 0x70);                                              // 004C6E56..59
            at<U*>(pf, 0xac) = list;                                                // 004C6E5E
            at<U>(pf, 0xb0) = 0;                                                    // 004C6E64
        }
    } else if (at<void*>(pf, 0xac) == nullptr) {                                    // 004C6E70..72
        if (kotor_fastcall_004c6bd0(module, nullptr, pf) == 0) return k_failed_00745dc4; // 004C6E74..88
    } else {
        kotor_fastcall_005cec50(pf, nullptr);                                       // 004C6E8B..8D
    }
    U result = kotor_fastcall_004c3260(module, nullptr, pf, budget);                // 004C6E92..A0
    const I areas = at<I>(pf, 0xa8);                                                // 004C6EA8
    if (result == k_failed_00745dc4) {                                              // 004C6EA2..AE
        if (areas <= 1) return result;                                              // 004C6EB4..B6 (jle)
        const I count = at<I>(pf, 0xb4);                                            // 004C6EBC
        U* blocked = static_cast<U*>(live_operator_new(static_cast<U>(count) * 4 + 4)); // 004C6EC2..CA
        for (I i = 0; i < count; ++i) blocked[i] = at<U*>(pf, 0xb8)[i];             // 004C6ED4..EF
        blocked[count] = *at<U*>(pf, 0xb0);                                         // 004C6EF1..F9 (first transition object)
        at<I>(pf, 0xb4) = count + 1;                                                // 004C6F02..05
        if (at<void*>(pf, 0xb8) != nullptr) live_free(at<void*>(pf, 0xb8));         // 004C6EFC..13
        at<U*>(pf, 0xb8) = blocked;                                                 // 004C6F18
        w5_pathfind_reset_inter_area_search_005d0e00(pf, nullptr);                  // 004C6F16..1E
        kotor_fastcall_005cec50(pf, nullptr);                                       // 004C6F23..25
        if (kotor_fastcall_004c6bd0(module, nullptr, pf) == 0) return k_failed_00745dc4; // 004C6F2A..3F
        result = k_in_progress_00745dbc;                                            // 004C6F42
        kotor_fastcall_005cec50(pf, nullptr);                                       // 004C6F48..4A
        return result;
    }
    if (areas > 1) kotor_fastcall_005cec50(pf, nullptr);                            // 004C6F58..5E
    return result;
}

// 004C6BD0 CSWSModule::ComputeInterAreaPath(pf): RET 4, EAX 0/1 (iterative deepening up to 9 transitions)
U __fastcall kotor_fastcall_004c6bd0(void* module, void*, void* pf) {
    at<I>(pf, 0xa8) = 10;                                                           // 004C6BDB
    at<void*>(pf, 0xac) = live_operator_new(0x28);                                  // 004C6BD7..EC (old buffers not freed)
    at<void*>(pf, 0xb0) = live_operator_new(0x24);                                  // 004C6BEA..FA
    at<U>(pf, 0xa0) = 0;                                                            // 004C6C00
    I limit = 1;                                                                    // 004C6C0A
    do {
        if (at<U>(pf, 0xa0) != 0) break;                                            // 004C6C10..18
        at<U*>(pf, 0xac)[0] = at<U>(pf, 0x80);                                      // 004C6C1A..26
        void* area = kotor_fastcall_004ae780(server_app(), nullptr, at<U>(module, 0x40)); // 004C6C28..35
        if (area != nullptr) at<U>(area, 0x1e0) = 0;                                // 004C6C3A..3E (only the start area is unmarked)
        const U found = kotor_fastcall_004c5510(module, nullptr, 0, limit, pf);     // 004C6C48..4E
        if (found == 1) at<U>(pf, 0xa0) = found;                                    // 004C6C53..58
        ++limit;                                                                    // 004C6C5E
    } while (limit < 10);                                                           // 004C6C5F..62
    if (at<U>(pf, 0xa0) == 0) return 0;                                             // 004C6C64..73
    kotor_fastcall_004c3ea0(module, nullptr, pf);                                   // 004C6C76..79
    return 1;
}

// 004C5510 CSWSModule::InterAreaDFS(depth, max_depth, pf): RET 0Ch, EAX 0/1
U __fastcall kotor_fastcall_004c5510(void* module, void*, I depth, I max_depth, void* pf) {
    if (at<U*>(pf, 0xac)[depth] == at<U>(pf, 0x70)) {                               // 004C551D..2D
        at<I>(pf, 0xa8) = depth + 1;                                                // 004C552F..30
        return 1;
    }
    if (depth >= max_depth) return 0;                                               // 004C5543..50
    // 004C5557..63: an empty counting loop `for (i = 0; i < depth; ++i) ;` (no observable effect)
    U* list = nullptr;                                                              // [esp+28] (the pf argument slot), zeroed 004C556C
    const I count = kotor_fastcall_004c54a0(module, nullptr, static_cast<U>(depth), pf, &list); // 004C5565..70
    void* array = object_array_real();                                              // 004C5575..84
    for (I i = 0; i < count; ++i) {                                                 // 004C5589..95 / 004C56CB..D2
        at<U*>(pf, 0xb0)[depth] = list[i];                                          // 004C55A0..B1
        void* object = nullptr;                                                     // [esp+20] (the depth argument slot)
        if (!get_object(array, list[i], &object)) continue;                         // 004C55B4..C8
        const unsigned char type = at<unsigned char>(object, 8);                    // 004C55D2
        void* target_area = nullptr;
        if (type == k_type_trigger_00746636) {                                      // 004C55D5..DB
            void* trigger = vcall<void*>(object, 0x40);                             // 004C55E1..E3 AsSWSTrigger
            if (at<U>(trigger, 0x2b4) == 0) continue;                               // 004C55E6..EE (test: any link)
            const U area_id = kotor_fastcall_0058c830(trigger, nullptr);            // 004C55F4..FB
            void* area_object = nullptr;                                            // reuses [esp+20]
            if (!get_object(array, area_id, &area_object)) continue;                // 004C5600..10
            target_area = vcall<void*>(area_object, 0x28);                          // 004C5616..1C AsSWSArea
            if (target_area == nullptr) continue;                                   // 004C561F..21
        } else if (type == k_type_door_00746639) {                                  // 004C566B..71
            void* door = vcall<void*>(object, 0x18);                                // 004C5673..75 AsSWSDoor
            if (door == nullptr) continue;                                          // 004C567A..7C
            if (w5_door_get_is_linked_005890d0(door, nullptr) == 0) continue;       // 004C567E..87
            void* linked = w5_door_get_linked_object_00589580(door, nullptr);       // 004C5689..90
            if (linked == nullptr) continue;
            target_area = recovered_object_area_004cb120(linked, nullptr);          // 004C5694..9D
            if (target_area == nullptr) continue;
        } else {
            continue;                                                               // 004C5671
        }
        at<U*>(pf, 0xac)[depth + 1] = at<U>(target_area, 0x120);                    // 004C5627..38 / 004C569F..B0
        if (kotor_fastcall_004c5510(module, nullptr, depth + 1, max_depth, pf) == 1) { // 004C5637..4D / 004C56AF..C5
            if (list != nullptr) live_free(list);                                   // 004C564F..59 / 004C56DA..EC
            return 1;
        }
    }
    if (list != nullptr) live_free(list);                                           // 004C5702..0C
    return 0;
}

// 004C54A0 CSWSModule::GenerateInterAreaDFSSuccessors(unused, pf, out): RET 0Ch. Uses the module's area (004C30F0),
// not the area of the DFS node (KotOR modules hold one area).
I __fastcall kotor_fastcall_004c54a0(void* module, void*, U unused, void* pf, U** out) {
    (void)unused;
    void* area = module_get_area_004c30f0(module, nullptr);                         // 004C54A1
    if (area == nullptr) return 0;                                                  // 004C54A8..AD
    const I count = kotor_fastcall_004bc400(area, nullptr, nullptr, at<I>(pf, 0xb4), at<const U*>(pf, 0xb8)); // 004C54B5..C7
    if (count == 0) return 0;                                                       // 004C54CC..D2
    U* list = static_cast<U*>(live_operator_new(static_cast<U>(count) << 2));       // 004C54D5..D9
    *out = list;                                                                    // 004C54DE..E2
    return kotor_fastcall_004bc400(area, nullptr, list, at<I>(pf, 0xb4), at<const U*>(pf, 0xb8)); // 004C54E4..F8
}

// 004BC400 CSWSArea::CountAreaTransitionTriggers(out, blocked_count, blocked): RET 0Ch. Counts (and with out != 0
// lists, marking the target area +1E0) the area's linked triggers and doors that lead to an unmarked area.
I __fastcall kotor_fastcall_004bc400(void* area, void*, U* out, I count, const U* list) {
    void* array = object_array_real();                                              // 004BC403..16
    const I objects = at<I>(area, 0x194);                                           // 004BC41B (read once)
    I found = 0;                                                                    // EBP
    for (I i = 0; i < objects; ++i) {                                               // 004BC433 / 004BC560..6F
        const U id = at<U*>(area, 0x190)[i];                                        // 004BC450..56 (EDI)
        void* object = nullptr;                                                     // [esp+1C]
        if (!get_object(array, id, &object)) continue;                              // 004BC459..6C
        const unsigned char type = at<unsigned char>(object, 8);                    // 004BC476
        void* target = nullptr;                                                     // EAX
        if (type == k_type_trigger_00746636) {                                      // 004BC479..7F
            void* trigger = vcall<void*>(object, 0x40);                             // 004BC481..83
            if (at<U>(trigger, 0x2b4) != 1) continue;                               // 004BC486..8D
            void* area_object = nullptr;                                            // [esp+20]
            const U area_id = kotor_fastcall_0058c830(trigger, nullptr);            // 004BC493..9A
            if (!get_object(array, area_id, &area_object)) continue;                // 004BC49F..AD
            target = vcall<void*>(area_object, 0x28);                               // 004BC4B3..B9
            if (target == nullptr) continue;                                        // 004BC4BC..BE
        } else if (type == 0x0a) {                                                  // 004BC4F5..F7 (immediate)
            void* door = vcall<void*>(object, 0x18);                                // 004BC4F9..FE (no null test)
            if (w5_door_get_is_linked_005890d0(door, nullptr) == 0) continue;       // 004BC500..09
            void* linked = w5_door_get_linked_object_00589580(door, nullptr);       // 004BC50B..14
            if (linked == nullptr) continue;
            target = recovered_object_area_004cb120(linked, nullptr);               // 004BC516..1F
            if (target == nullptr) continue;
        } else {
            continue;
        }
        if (at<U>(target, 0x1e0) != 0) continue;                                    // 004BC4C4..CC / 004BC521..29
        U blocked = 0;                                                              // EDX
        for (I k = 0; k < count; ++k) if (list[k] == id) blocked = 1;               // 004BC4D2..F3 / 004BC52B..44
        if (blocked != 0) continue;                                                 // 004BC546..48
        if (out != nullptr) {                                                       // 004BC54A..50
            out[found] = id;                                                        // 004BC552
            at<U>(target, 0x1e0) = 1;                                               // 004BC555
        }
        ++found;                                                                    // 004BC55F
    }
    return found;
}

// 0058C830 CSWSTrigger::GetTargetArea is owned by claude_w5_pathfind_walk.cpp (coordinator ruling); imported.

// =========================================================================================================
// 004C3260 CSWSModule::PlotPathInArea(pf, budget): RET 8
U __fastcall kotor_fastcall_004c3260(void* module, void*, void* pf, U budget) {
    float start_x = at<float>(pf, 0x74), start_y = at<float>(pf, 0x78);             // [esp+8]/[C]  (dword moves)
    float goal_x = at<float>(pf, 0x5c), goal_y = at<float>(pf, 0x60);               // [esp+10]/[14]
    const float radius = at<float>(pf, 4);                                          // [esp+24]
    const float height = at<float>(pf, 0x14);                                       // [esp+20]
    void* area = module_get_area_004c30f0(module, nullptr);                         // 004C3299 (EBP)
    if (area == nullptr) return k_failed_00745dc4;                                  // 004C32A0..A2
    if (at<U>(area, 0x120) != at<U>(pf, 0x80)) return k_failed_00745dc4;            // 004C32A8..B4
    ++at<I>(pf, 0x3c);                                                              // 004C32BA
    at<void*>(area, 0x1ac) = pf;                                                    // 004C32BD
    // 004C32C3..33ED: stop short of the goal by the approach range +68 (unless +240 is set)
    if (at<float>(pf, 0x68) > 0.001f && at<U>(pf, 0x240) == 0) {                   // 004C32C3..DF (test ah,41h)
        const double dx = static_cast<double>(goal_x) - start_x;                    // 004C32E5..ED
        const double dy = static_cast<double>(goal_y) - start_y;                    // 004C32F5..FB
        Vector result = {goal_x, goal_y, 0.0f};                                     // [esp+40] (E-48)
        const float approach = at<float>(pf, 0x68);                                 // [esp+1C]
        const Vector goal = {goal_x, goal_y, 0.0f};                                 // [esp+4C] (E-3C)
        const float dist2 = static_cast<float>(dy * dy + dx * dx);                  // 004C3306..32 [esp+18]
        if (dist2 >= 0x1.5798ecp-27f) {   // [00745824] = 0x322BCC76 (review P1)                                                      // 004C333A..49 (test ah,1; jne skips)
            const bool shorter = static_cast<double>(approach) * approach < dist2;  // 004C334F..5F, 004C337B (test ah,5)
            const Vector away = {static_cast<float>(static_cast<double>(start_x) - goal_x),
                                 static_cast<float>(static_cast<double>(start_y) - goal_y), 0.0f}; // [esp+64] E-24
            Vector unit;                                                            // [esp+7C] E-C
            normalize_004ab180(&unit, &away);                                       // 004C338C / 004C339D
            const double scale = shorter ? static_cast<double>(approach) - 0.001f   // 004C3391..95
                                         : x87_fsqrt(dist2);                        // 004C33A2..A6
            Vector step;                                                            // [esp+70] E-18
            float_times_vector_00437a60(&step, static_cast<float>(scale), &unit);   // 004C33AB..B5 (fstp dword arg)
            Vector sum;                                                             // [esp+58] E-30
            vector_sum_00423b90(&sum, &goal, &step);                                // 004C33BA..C5
            result = sum;                                                           // 004C33CA..DD
        }
        goal_x = result.x; goal_y = result.y;                                       // 004C33E1..ED
    }
    I line;                                                                         // EDI / [esp+20] (E-70)
    if (at<I>(pf, 0x3c) == 1) {                                                     // 004C33F1..FD
        const Vector from = {start_x, start_y, 0.0f};                               // [esp+58] E-30
        const Vector to = {goal_x, goal_y, 0.0f};                                   // [esp+64] E-24
        line = test_line(area, &from, &to, &radius, height, 1, nullptr);            // 004C33FF..4E
        at<I>(pf, 0x19c) = line;                                                    // 004C3459
    } else {
        line = at<I>(pf, 0x19c);                                                    // 004C3461..6B
    }
    if (at<U>(pf, 0x198) == 1) {                                                    // 004C346D..73
        if (line != 1 && at<U>(pf, 0x84) == 0) {                                    // 004C3479..89
            // 004C348F..357E: bisect the start->goal segment for the farthest directly reachable point
            float t = 0.5f, step = 0.25f;                                           // [esp+20] / [esp+24]
            U found = 0;                                                            // EDI
            const float dx = static_cast<float>(static_cast<double>(goal_x) - start_x); // [esp+30]
            const float dy = static_cast<float>(static_cast<double>(goal_y) - start_y); // [esp+34]
            float best_x = 0.0f, best_y = 0.0f;                                     // [esp+40]/[44] (written before use)
            double next;
            do {
                const double x = static_cast<double>(dx) * t + start_x;             // 004C34C0..D6
                const float xf = static_cast<float>(x);                             // 004C34E1 fst [E-58]
                const double y = static_cast<double>(dy) * t + start_y;             // 004C34E9..FB
                const float yf = static_cast<float>(y);                             // 004C3506 fst [E-54]
                const Vector from = {start_x, start_y, 0.0f};                       // [esp+58] E-30
                const Vector to = {static_cast<float>(x), static_cast<float>(y), 0.0f}; // [esp+64] E-24 (fstp)
                if (test_line(area, &from, &to, &radius, height, 1, nullptr) == 1) { // 004C3530..38
                    t = static_cast<float>(static_cast<double>(step) + t);          // 004C353A..46
                    best_x = xf; best_y = yf; found = 1;                            // 004C353E..52
                } else {
                    t = static_cast<float>(static_cast<double>(t) - step);          // 004C3559..61
                }
                next = static_cast<double>(step) * 0.5f;                            // 004C3565..69
                step = static_cast<float>(next);                                    // 004C356F fst
            } while (next > 0.005f);                                                // 004C3573..7E (test ah,41h; je)
            if (found == 1) { goal_x = best_x; goal_y = best_y; line = 1; }         // 004C3584..99
            else line = -4;                                                         // 004C359F
        }
        at<U>(pf, 0x194) = 0; at<U>(pf, 0x198) = 0;                                 // 004C35AB..B3
    }
    U result;                                                                       // EBX
    if (line == 1) {                                                                // 004C35B9..BC
        at<I>(pf, 0x8c) = 4;                                                        // 004C35C0
        float* points = static_cast<float*>(live_operator_new(0x10));               // 004C35BE..CA
        points[0] = start_x; points[1] = start_y; points[2] = goal_x; points[3] = goal_y; // 004C35CF..E7
        at<float*>(pf, 0x90) = points;                                              // 004C35EA
        result = k_complete_00745dc0;                                               // 004C35F0
    } else {
        void* creature = creature_by_id(at<U>(pf, 0x2c));                           // 004C35FE..0B
        if (creature != nullptr && at<U>(creature, 0x78) == 0) {                    // 004C3610..19
            result = k_failed_00745dc4;                                             // 004C3703
        } else {
            if (at<U>(pf, 0x194) != 0) {                                            // 004C361F..27
                result = kotor_fastcall_004c1a00(area, nullptr, pf, budget, 0);     // 004C3629..3B
                if (recovered_debug_cell_00832838 != 0) (void)creature_by_id(at<U>(pf, 0x2c)); // 004C363D..57
            } else if (line != -2 && line != -1 && line != -3 && line != 0) {       // 004C365E..6F
                result = k_failed_00745dc4;                                         // 004C3671
            } else if (at<U>(area, 0x238) != 0) {                                   // 004C3679..86 (path points)
                if (recovered_debug_cell_00832838 != 0) (void)creature_by_id(at<U>(pf, 0x2c)); // 004C3688..98
                result = kotor_fastcall_004c2200(area, nullptr, pf, budget, 0);     // 004C369D..AA
            } else {
                if (recovered_debug_cell_00832838 != 0) (void)creature_by_id(at<U>(pf, 0x2c)); // 004C36B1..C1
                result = kotor_fastcall_004c1a00(area, nullptr, pf, budget, 0);     // 004C36C6..D3
            }
            if (recovered_debug_cell_00832838 != 0) (void)creature_by_id(at<U>(pf, 0x2c)); // 004C36DA..F0
            if (at<I>(pf, 0x3c) > 100 && result == k_in_progress_00745dbc) result = k_failed_00745dc4; // 004C36F5..03
        }
    }
    if (result == k_failed_00745dc4 && at<U>(pf, 0) == 0) {                         // 004C3709..1B
        // 004C3721..38A0: fall back to the farthest reachable point towards the (unmodified) pf goal
        const float sx = at<float>(pf, 0x74), sy = at<float>(pf, 0x78);             // [esp+10]/[14] (dword moves)
        const float vx = static_cast<float>(static_cast<double>(at<float>(pf, 0x5c)) - sx); // [esp+60]
        const float vy = static_cast<float>(static_cast<double>(at<float>(pf, 0x60)) - sy); // [esp+64]
        const float vz = 0.0f;                                                      // [esp+68]
        at<U>(pf, 0x84) = 0;                                                        // 004C3746
        float best_x = 0.0f, best_y = 0.0f;                                         // [esp+48]/[4C]
        float t = 0.5f, step = 0.25f;                                               // [esp+1C] / [esp+24]
        U found = 0;                                                                // EDI
        if (line != -1) {                                                           // 004C3737..70 ([esp+20])
            double next;
            do {
                const double a = static_cast<double>(vx) * t;                       // 004C3776..7C
                const double b = static_cast<double>(vy) * t;                       // 004C3782..91
                const float c = static_cast<float>(static_cast<double>(vz) * t);    // 004C3795..9D
                const float x = static_cast<float>(a + sx);                         // 004C37A6..AA
                const float y = static_cast<float>(b + sy);                         // 004C37B6..C9
                const Vector from = {sx, sy, 0.0f};                                 // [esp+80] E-18
                const Vector to = {x, y, c};                                        // [esp+5C] E-3C
                if (test_line(area, &from, &to, &radius, height, 0, nullptr) == 1) { // 004C37E8..10
                    best_x = to.x; best_y = to.y;                                   // 004C3812..2E
                    t = static_cast<float>(static_cast<double>(step) + t);          // 004C3816..1E
                    found = 1;
                } else {
                    t = static_cast<float>(static_cast<double>(t) - step);          // 004C3839..3D
                }
                next = static_cast<double>(step) * 0.5f;                            // 004C3845..49
                step = static_cast<float>(next);
            } while (next > 0.005f);                                                // 004C3853..5E
            if (found != 0) {                                                       // 004C3864..66
                at<I>(pf, 0x8c) = 4;                                                // 004C386A
                float* points = static_cast<float*>(live_operator_new(0x10));       // 004C3868..74
                points[0] = sx; points[1] = sy; points[2] = best_x; points[3] = best_y; // 004C3879..91
                at<float*>(pf, 0x90) = points;                                      // 004C3894
                result = k_complete_00745dc0;                                       // 004C389A
            }
        }
    }
    if (result == k_complete_00745dc0 && at<U>(pf, 0x84) == 1) {                    // 004C38A3..B2 (planned goal->start)
        const I count = at<I>(pf, 0x8c);                                            // EDI
        float* old = at<float*>(pf, 0x90);                                          // EBP
        float* reversed = static_cast<float*>(live_operator_new(static_cast<U>(count) * 4)); // 004C38C0..CC
        for (I i = 0; i < count; i += 2) {                                          // 004C38D4..F4
            reversed[i] = old[count - 2 - i];
            reversed[i + 1] = old[count - 1 - i];
        }
        at<float*>(pf, 0x90) = reversed;                                            // 004C38FC
        if (old != nullptr) live_free(old);                                         // 004C38FA..0A
        at<U>(pf, 0x84) = 0;                                                        // 004C390F
        kotor_fastcall_005cec10(pf, nullptr);                                       // 004C3919
    }
    if (result == k_failed_00745dc4 || result == k_complete_00745dc0) at<U>(pf, 0x88) = 1; // 004C391E..2E
    return result;
}

// =========================================================================================================
// 005111C0 CSWSCreature::ComputeTotalWalkDistance: ST0 = length of the remaining planned walk (+9C..+8C)
double __fastcall kotor_fastcall_005111c0(void* creature, void*) {
    void* pf = at<void*>(creature, 0x340);                                          // 005111C9
    const I first = at<I>(pf, 0x9c);                                                // 005111CF (EBX)
    const I count = at<I>(pf, 0x8c);                                                // 005111D5 (EBP)
    float total = 0.0f;                                                             // [esp+10]
    for (I i = first; i < count; i += 2) {                                          // 005111E3..E7 / 0051126A..71
        const float* points = at<const float*>(at<void*>(creature, 0x340), 0x90);   // 005111EF..F5 (re-read)
        const double x = points[i];                                                 // 005111FB
        const float y = points[i + 1];                                              // 005111FE..02 [esp+30]
        Vector segment;
        if (i == first) {                                                           // 005111ED / 00511206
            segment.x = static_cast<float>(x - at<float>(creature, 0x90));          // 00511208..13
            segment.y = static_cast<float>(static_cast<double>(y) - at<float>(creature, 0x94)); // 00511217..21
            segment.z = -at<float>(creature, 0x98);                                 // 00511225..2D (fchs)
        } else {
            segment.z = 0.0f;                                                       // 0051123F
            segment.x = static_cast<float>(x - points[i - 2]);                      // 00511233..4C
            segment.y = static_cast<float>(static_cast<double>(y) - points[i - 1]); // 00511250..56
        }
        total = static_cast<float>(kotor_cdecl_004ab100(&segment) + total);         // 0051125E..6D
    }
    return total;                                                                   // 00511276 fld dword
}

// 005CEC10 CPathfindInformation::FlipStartEndPoints: swap +5C/+60/+64 with +74/+78/+7C and +80 with +70
void __fastcall kotor_fastcall_005cec10(void* pf, void*) {
    const U sx = at<U>(pf, 0x74), sy = at<U>(pf, 0x78), sz = at<U>(pf, 0x7c);       // 005CEC13..22
    at<float>(pf, 0x74) = at<float>(pf, 0x5c);                                      // 005CEC10..19 (fld/fstp)
    at<float>(pf, 0x78) = at<float>(pf, 0x60);                                      // 005CEC1C..25
    at<float>(pf, 0x7c) = at<float>(pf, 0x64);                                      // 005CEC28..34
    at<U>(pf, 0x5c) = sx; at<U>(pf, 0x60) = sy; at<U>(pf, 0x64) = sz;               // 005CEC1F..37
    const U area = at<U>(pf, 0x70);                                                 // 005CEC3A
    at<U>(pf, 0x70) = at<U>(pf, 0x80);                                              // 005CEC2E / 3D
    at<U>(pf, 0x80) = area;                                                         // 005CEC40
}

// 005CEC50 CPathfindInformation::FlipTempEndPoints: swap +5C..+68 with +44..+50 and +70 with +54
void __fastcall kotor_fastcall_005cec50(void* pf, void*) {
    const U tx = at<U>(pf, 0x44), ty = at<U>(pf, 0x48), tz = at<U>(pf, 0x4c), tr = at<U>(pf, 0x50); // 005CEC53..6E
    at<float>(pf, 0x44) = at<float>(pf, 0x5c);                                      // 005CEC50..59 (fld/fstp)
    at<float>(pf, 0x48) = at<float>(pf, 0x60);                                      // 005CEC5C..65
    at<float>(pf, 0x4c) = at<float>(pf, 0x64);                                      // 005CEC68..71
    at<float>(pf, 0x50) = at<float>(pf, 0x68);                                      // 005CEC74..7D
    at<U>(pf, 0x5c) = tx; at<U>(pf, 0x60) = ty; at<U>(pf, 0x64) = tz; at<U>(pf, 0x68) = tr; // 005CEC5F..80
    const U area = at<U>(pf, 0x70);                                                 // 005CEC7A
    at<U>(pf, 0x70) = at<U>(pf, 0x54);                                              // 005CEC83..86
    at<U>(pf, 0x54) = area;                                                         // 005CEC89
}

// 004BB340: +1AC = path (old buffer freed; +1AC zeroed first)
void __fastcall kotor_fastcall_004bb340(void* pf, void*, void* path) {
    void* old = at<void*>(pf, 0x1ac);                                               // 004BB343
    if (old != nullptr) { live_free(old); at<U>(pf, 0x1ac) = 0; }                   // 004BB349..57
    at<void*>(pf, 0x1ac) = path;                                                    // 004BB364 / 72
}
// 004BB380: *start = +1E4, *goal = +1F0 (dword moves)
void __fastcall kotor_fastcall_004bb380(void* pf, void*, Vector* start, Vector* goal) {
    copy_vector(start, bytes(pf) + 0x1e4);                                          // 004BB384..98
    copy_vector(goal, bytes(pf) + 0x1f0);                                           // 004BB39B..B2
}
// 004BB3C0: +1E4 = *start, +1F0 = *goal
void __fastcall kotor_fastcall_004bb3c0(void* pf, void*, const Vector* start, const Vector* goal) {
    copy_vector(bytes(pf) + 0x1e4, start);                                          // 004BB3C4..D8
    copy_vector(bytes(pf) + 0x1f0, goal);                                           // 004BB3DB..F2
}
// 004BB400: +1FC = count, +200 = points (old freed)
void __fastcall kotor_fastcall_004bb400(void* pf, void*, I count, void* points) {
    at<I>(pf, 0x1fc) = count;                                                       // 004BB407
    if (at<void*>(pf, 0x200) != nullptr) live_free(at<void*>(pf, 0x200));          // 004BB40D..18
    at<void*>(pf, 0x200) = points;                                                  // 004BB424 / 32
}
// 004BB440: +204 = count, +208 = points (old freed)
void __fastcall kotor_fastcall_004bb440(void* pf, void*, I count, void* points) {
    at<I>(pf, 0x204) = count;                                                       // 004BB447
    if (at<void*>(pf, 0x208) != nullptr) live_free(at<void*>(pf, 0x208));          // 004BB44D..58
    at<void*>(pf, 0x208) = points;                                                  // 004BB464 / 72
}
// 004BB480 / 004BB4D0 / 004BB520: replace a smoothing buffer; a different old buffer is freed and its count zeroed
void __fastcall kotor_fastcall_004bb480(void* pf, void*, void* points) {
    void* old = at<void*>(pf, 0x21c);                                               // 004BB483
    if (old != nullptr && old != points) {                                          // 004BB489..9C
        at<U>(pf, 0x218) = 0; live_free(old); at<U>(pf, 0x21c) = 0;                // 004BB49E..B1
    }
    at<void*>(pf, 0x21c) = points;                                                  // 004BB4BB
}
void __fastcall kotor_fastcall_004bb4d0(void* pf, void*, void* points) {
    void* old = at<void*>(pf, 0x228);                                               // 004BB4D3
    if (old != nullptr && old != points) {                                          // 004BB4D9..EC
        at<U>(pf, 0x220) = 0; live_free(old); at<U>(pf, 0x228) = 0;                // 004BB4EE..01
    }
    at<void*>(pf, 0x228) = points;                                                  // 004BB50B
}
void __fastcall kotor_fastcall_004bb520(void* pf, void*, void* points) {
    void* old = at<void*>(pf, 0x22c);                                               // 004BB523
    if (old != nullptr && old != points) {                                          // 004BB529..3C
        at<U>(pf, 0x224) = 0; live_free(old); at<U>(pf, 0x22c) = 0;                // 004BB53E..51
    }
    at<void*>(pf, 0x22c) = points;                                                  // 004BB55B
}

// 004BB860 CSWSArea::ComputePathDistance(count floats, points xy pairs): RET 8, ST0 (thiscall, ECX unused)
double __fastcall kotor_fastcall_004bb860(void* area, void*, I count, const float* points) {
    (void)area;
    double total = 0.0f;                                                            // 004BB864 fld [0073D700]
    const I n = count - 2;                                                          // 004BB86A
    if (n <= 0) return total;                                                       // 004BB86D..6F
    U iterations = (static_cast<U>(n - 1) >> 1) + 1;                                // 004BB878..7B
    const float* p = points + 1;                                                    // 004BB875 (eax = points + 4)
    do {
        const double dy = static_cast<double>(p[2]) - p[0];                         // 004BB880..87
        const double dx = static_cast<double>(p[1]) - p[-1];                        // 004BB88A..8D
        p += 2;                                                                     // 004BB883
        total = total + x87_fsqrt(dx * dx + dy * dy);                               // 004BB890..A0
    } while (--iterations != 0);                                                    // 004BB886 / A2
    return total;
}

// ---------------- DFS transposition tables (256 + 256 slots of 16 bytes, two-way) ----------------
namespace {
inline I table_hash(I a, I b) {                                                     // (a*17 + b*23 + 6000) % 256, signed
    const I h = static_cast<I>(static_cast<U>(a) * 0x11u + static_cast<U>(b) * 0x17u + 0x1770u);
    return h % 256;                                                                 // and 800000FFh / jns / dec,or,inc
}
}
// 004BBC50 CSWSArea::GridDFSTransTableInitialize: +1B4 (allocated once, 2000h) slots = {0, 0, 80000000h, 0}
void __fastcall kotor_fastcall_004bbc50(void* area, void*) {
    if (at<void*>(area, 0x1b4) == nullptr) at<void*>(area, 0x1b4) = live_operator_new(0x2000); // 004BBC53..6D
    for (U off = 0; off < 0x2000; off += 0x10) {                                    // 004BBC73..AF (base re-read)
        at<U>(at<void*>(area, 0x1b4), off) = 0;
        at<U>(at<void*>(area, 0x1b4), off + 4) = 0;
        at<U>(at<void*>(area, 0x1b4), off + 8) = 0x80000000u;
        at<U>(at<void*>(area, 0x1b4), off + 0xc) = 0;
    }
}
// 004BBCC0 CSWSArea::GridDFSTransTableGet(x, y, &g, &limit): RET 10h, EAX 1 found / 0 (outputs zeroed)
U __fastcall kotor_fastcall_004bbcc0(void* area, void*, I x, I y, I* g, I* limit) {
    const I slot = table_hash(x, y);                                                // 004BBCC7..E9
    unsigned char* table = at<unsigned char*>(area, 0x1b4);                         // 004BBCEA
    I* e = reinterpret_cast<I*>(table + slot * 16);
    if (e[0] == x && e[1] == y) {                                                   // 004BBCF5..FE
        *g = e[2];                                                                  // 004BBD00..08
        *limit = reinterpret_cast<I*>(at<unsigned char*>(area, 0x1b4) + slot * 16)[3]; // 004BBD0A..1A
        return 1;
    }
    e = reinterpret_cast<I*>(table + (slot + 0x100) * 16);                          // 004BBD25..2A
    if (e[0] == x && e[1] == y) {                                                   // 004BBD2D..36
        *g = e[2];                                                                  // 004BBD38..40
        *limit = reinterpret_cast<I*>(at<unsigned char*>(area, 0x1b4) + (slot + 0x100) * 16)[3]; // 004BBD42..52
        return 1;
    }
    *g = 0; *limit = 0;                                                             // 004BBD5D..6D
    return 0;
}
// 004BBD80 CSWSArea::GridDFSTransTablePut(x, y, g, limit): RET 10h. Same key or a worse primary entry
// (greater g, or equal g and greater limit) is overwritten in the primary slot, otherwise the secondary slot.
void __fastcall kotor_fastcall_004bbd80(void* area, void*, I x, I y, I g, I limit) {
    I slot = table_hash(x, y);                                                      // 004BBD87..AA
    unsigned char* table = at<unsigned char*>(area, 0x1b4);                         // 004BBDAB
    const I* e = reinterpret_cast<const I*>(table + slot * 16);
    if (!(e[0] == x && e[1] == y)) {                                                // 004BBDB6..C6
        const I stored = e[2];                                                      // 004BBDC8
        if (!(stored > g)) {                                                        // 004BBDCB..CD (jg)
            if (stored != g || !(e[3] > limit)) slot += 0x100;                      // 004BBDCF..DA
        }
    }
    I* out = reinterpret_cast<I*>(table + slot * 16);                               // 004BBDDF
    out[0] = x;                                                                     // 004BBDE2
    reinterpret_cast<I*>(at<unsigned char*>(area, 0x1b4) + slot * 16)[1] = y;       // 004BBDE5..EF
    reinterpret_cast<I*>(at<unsigned char*>(area, 0x1b4) + slot * 16)[2] = g;       // 004BBDF3..FA
    reinterpret_cast<I*>(at<unsigned char*>(area, 0x1b4) + slot * 16)[3] = limit;   // 004BBDFE..0A
}
// 004BBEB0 CSWSArea::PathPointDFSTransTableInitialize: +250 slots = {0, 0, FLT_MAX, 0}
void __fastcall kotor_fastcall_004bbeb0(void* area, void*) {
    if (at<void*>(area, 0x250) == nullptr) at<void*>(area, 0x250) = live_operator_new(0x2000); // 004BBEB3..CD
    for (U off = 0; off < 0x2000; off += 0x10) {                                    // 004BBED3..0F
        at<U>(at<void*>(area, 0x250), off) = 0;
        at<U>(at<void*>(area, 0x250), off + 4) = 0;
        at<U>(at<void*>(area, 0x250), off + 8) = 0x7f7fffffu;
        at<U>(at<void*>(area, 0x250), off + 0xc) = 0;
    }
}
// 004BBF20 CSWSArea::PathPointDFSTransTableGet(x, y, &g, &limit): RET 10h (key = __ftol2 of the coordinates)
U __fastcall kotor_fastcall_004bbf20(void* area, void*, float x, float y, float* g, float* limit) {
    const I kx = ftol2(x);                                                          // 004BBF20..28
    const I ky = ftol2(y);                                                          // 004BBF2D..36
    const I slot = table_hash(kx, ky);                                              // 004BBF33..54
    unsigned char* table = at<unsigned char*>(area, 0x250);                         // 004BBF55
    const float* e = reinterpret_cast<const float*>(table + slot * 16);
    if (e[0] == x && e[1] == y) {                                                   // 004BBF60..7B (test ah,44h; jp)
        *reinterpret_cast<U*>(g) = reinterpret_cast<const U*>(e)[2];                // 004BBF7D..85
        *reinterpret_cast<U*>(limit) = reinterpret_cast<const U*>(at<unsigned char*>(area, 0x250) + slot * 16)[3]; // 004BBF87..96
        return 1;
    }
    e = reinterpret_cast<const float*>(table + (slot + 0x100) * 16);                // 004BBFA1..A7
    if (e[0] == x && e[1] == y) {                                                   // 004BBFAA..C5
        *reinterpret_cast<U*>(g) = reinterpret_cast<const U*>(e)[2];                // 004BBFC7..CF
        *reinterpret_cast<U*>(limit) = reinterpret_cast<const U*>(at<unsigned char*>(area, 0x250) + (slot + 0x100) * 16)[3]; // 004BBFD1..E0
        return 1;
    }
    *reinterpret_cast<U*>(g) = 0; *reinterpret_cast<U*>(limit) = 0;                 // 004BBFEB..FA
    return 0;
}
// 004BC010 CSWSArea::PathPointDFSTransTablePut(x, y, g, limit): RET 10h (fld/fstp stores)
void __fastcall kotor_fastcall_004bc010(void* area, void*, float x, float y, float g, float limit) {
    const I kx = ftol2(x);                                                          // 004BC010..18
    const I ky = ftol2(y);                                                          // 004BC01D..26
    I slot = table_hash(kx, ky);                                                    // 004BC023..44
    unsigned char* table = at<unsigned char*>(area, 0x250);                         // 004BC045
    const float* e = reinterpret_cast<const float*>(table + slot * 16);
    if (!(e[0] == x && e[1] == y)) {                                                // 004BC050..6D
        if (!(e[2] > g)) {                                                          // 004BC06F..7B (test ah,41h; je primary)
            if (!(e[2] == g) || !(e[3] > limit)) slot += 0x100;                     // 004BC07D..99
        }
    }
    float* out = reinterpret_cast<float*>(table + slot * 16);                       // 004BC09F..A3
    out[0] = x;                                                                     // 004BC0A6
    reinterpret_cast<float*>(at<unsigned char*>(area, 0x250) + slot * 16)[1] = y;   // 004BC0A9..B4
    reinterpret_cast<float*>(at<unsigned char*>(area, 0x250) + slot * 16)[2] = g;   // 004BC0B8..C2
    reinterpret_cast<float*>(at<unsigned char*>(area, 0x250) + slot * 16)[3] = limit; // 004BC0C6..D1
}

}  // extern "C"
