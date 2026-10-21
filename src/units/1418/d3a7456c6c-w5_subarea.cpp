// W5 G16 movement side effects: subarea (trigger / door / area-of-effect / encounter) membership on movement.
// CSWSArea::InSubAreas 004BF470, CSWSArea::IntersectingLineSegment 004BF2C0, CSWSCreature::HandleSubAreaStateChanges
// 00516020, CSWSCreature::UpdateSubareasOnMoveTo 0051B7B0, CSWSTrigger::InTrigger 0058CE40,
// CSWSTrigger::LineSegmentIntersectTrigger 0058CD80, CSWArea::IntersectLineSegments 0058F840, CSWSDoor::InDoor 005897D0.
// Area-of-effect and encounter containment bodies are explicit traps (not reconstructed in this checkpoint).
#include "e5e3766b7c-w5_subarea.hpp"
#include "294ff121b7-w5_server_env.hpp"
#include <cmath>

using namespace w5;

namespace {
struct List { U* data; I size; I capacity; };
bool list_contains(const List* l, U id) {          // the original counts matches (inner loop) and tests != 0
    I hits = 0;
    for (I k = 0; k < l->size; ++k) if (l->data[k] == id) ++hits;
    return hits != 0;
}
bool creature_has(void* creature, U id) {
    const I n = at<I>(creature, 0x334);
    const U* d = at<U*>(creature, 0x330);
    I hits = 0;
    for (I k = 0; k < n; ++k) if (d[k] == id) ++hits;
    return hits != 0;
}
// x87 flag tests used below (fcomp a,b): "test ah,1; jne skip" runs the block iff a >= b; "test ah,41h; jne skip"
// iff a > b; "test ah,41h; jp skip" iff a <= b; "test ah,5; jp skip" iff a < b; "test ah,44h; jp skip" iff a == b.
// Every comparison below is written in that ordered form, so NaN operands take the original branch.

// Shared polygon crossing count (InTrigger 0058CEAE..0058D037 and InDoor 005897F6..00589956).
template<class Vertex> I polygon_crossings(I count, Vertex vertex, float px, float py) {
    I crossings = 0;
    for (I i = 0; i < count; ++i) {
        const I j = (i == count - 1) ? 0 : i + 1;
        const float* a = vertex(i);
        const float* b = vertex(j);
        if (!(b[0] == a[0])) {                                    // test ah,44h; jnp skip (equal x skips)
            const float dy = static_cast<float>(static_cast<double>(b[1]) - a[1]);
            const float dx = static_cast<float>(static_cast<double>(b[0]) - a[0]);
            const float yint = static_cast<float>(static_cast<double>(a[1]) -
                (static_cast<double>(a[0]) - px) * (static_cast<double>(dy) / dx));
            if (yint > py) {                                      // test ah,41h; jne skip (<=, unordered)
                if (std::fabs(static_cast<double>(dx)) < std::fabs(static_cast<double>(dy))) {   // test ah,5; jp
                    if (b[1] > a[1]) { if (yint >= a[1] && yint < b[1]) ++crossings; }           // A.y <= y < B.y
                    else if (a[1] > b[1]) { if (yint > b[1] && yint <= a[1]) ++crossings; }      // B.y < y <= A.y
                } else {
                    if (b[0] > a[0]) { if (px >= a[0] && px < b[0]) ++crossings; }              // A.x <= px < B.x
                    else if (a[0] > b[0]) { if (px > b[0] && px <= a[0]) ++crossings; }         // B.x < px <= A.x
                }
            }
        }
        if (b[0] == px) {                                         // vertex on the ray's x (test ah,44h; jp skip)
            if (b[1] > py) ++crossings;                           // test ah,41h; je count
            else if (a[1] >= py) ++crossings;                     // test ah,1; jne skip
        }
    }
    return crossings;
}
inline U odd(I n) { return (n % 2 == 1) ? 1u : 0u; }          // 0058D040..0058D055 (signed remainder == 1)
}  // namespace

extern "C" {

// 0058F840 CSWArea::IntersectLineSegments(p1, p2, q1, q2 as 8 floats, float* ox, float* oy): thiscall RET 28h.
// ECX unused. EAX 1 when the two segments intersect (ranges inclusive, as tested below).
U __fastcall w5_area_intersect_line_segments_0058f840(void*, void*, float p1x, float p1y, float p2x, float p2y,
    float q1x, float q1y, float q2x, float q2y, float* ox, float* oy) {
    const double d1x_w = static_cast<double>(p2x) - p1x;                  // 0058F843..0058F84B (kept wide on the stack)
    const float d1x = static_cast<float>(d1x_w);
    const double d1y_w = static_cast<double>(p2y) - p1y;                  // 0058F84F..0058F857
    const float d1y = static_cast<float>(d1y_w);
    const float d2x = static_cast<float>(static_cast<double>(q2x) - q1x); // 0058F85B..0058F863
    const float d2y = static_cast<float>(static_cast<double>(q2y) - q1y); // 0058F867..0058F877
    const double dot = static_cast<double>(d2y) * d1y + static_cast<double>(d2x) * d1x;   // 0058F87B..0058F893
    const double len1 = std::sqrt(d1y_w * d1y_w + d1x_w * d1x_w);         // 0058F895..0058F89F
    const double len2 = std::sqrt(static_cast<double>(d2y) * d2y + static_cast<double>(d2x) * d2x); // 0058F8A5..0058F8B7
    const double cosine = (dot / len1) / len2;                            // 0058F8A3 / 0058F8BD
    if (cosine == 1.0) return 0;                                          // 0058F8C1..0058F8CE (exactly parallel, same sense)
    U hit = 0;                                                            // ECX
    if (p1x == p2x) {                                                     // 0058F8D4..0058F8E3: first segment vertical
        const double m = static_cast<double>(d2y) / d2x;                  // 0058F8E9..0058F8F1
        *ox = p1x;                                                        // 0058F8F9..0058F8FD
        const double b = static_cast<double>(q1y) - static_cast<double>(q1x) * m;   // 0058F8FF..0058F905
        const float y = static_cast<float>(b + m * p1x);                  // 0058F90B..0058F911 (stored over p1x)
        *oy = y;
        if (q2y >= q1y && y >= q1y && y <= q2y) hit = 1;                  // 0058F917..0058F944
        if (q1y >= q2y && y >= q2y && y <= q1y) hit = 1;                  // 0058F949..0058F976
        else if (hit != 1) return hit;                                    // 0058F97D..0058F980
        if (q2x >= q1x) { if (!(*ox >= q1x) || !(*ox <= q2x)) hit = 0; }  // 0058F986..0058F9AF
        if (q1x >= q2x) { if (!(*ox >= q2x) || !(*ox <= q1x)) return 0; } // 0058F9B1..0058F9DC
        if (hit != 1) return hit;                                         // 0058F9E2..0058F9E5
        if (p2y >= p1y) { if (!(y >= p1y) || !(y <= p2y)) hit = 0; }      // 0058F9EB..0058FA16 (test cl,ah = C0)
        if (!(p1y >= p2y)) return hit;                                    // 0058FA18..0058FA25
        if (!(y >= p2y)) return 0;                                        // 0058FA2B..0058FA38
        return (y <= p1y) ? hit : 0;                                      // 0058FA3E..0058FCD4
    }
    const double m1 = static_cast<double>(d1y) / d1x;                     // 0058FA53..0058FA57
    if (q1x == q2x) {                                                     // 0058FA4B..0058FA60: second segment vertical
        *ox = q1x;                                                        // 0058FA66..0058FA72
        const double b = static_cast<double>(p1y) - static_cast<double>(p1x) * m1;  // 0058FA74..0058FA7A
        const float y = static_cast<float>(b + m1 * q1x);                 // 0058FA7E..0058FA86 (stored over q1x)
        *oy = y;
        if (p2y >= p1y && y >= p1y && y <= p2y) hit = 1;                  // 0058FA8C..0058FAB9
        if (p1y >= p2y && y >= p2y && y <= p1y) hit = 1;                  // 0058FABE..0058FAEB
        else if (hit != 1) return hit;                                    // 0058FAF2..0058FAF5
        if (p2x >= p1x) { if (!(*ox >= p1x) || !(*ox <= p2x)) hit = 0; }  // 0058FAFB..0058FB24
        if (p1x >= p2x) { if (!(*ox >= p2x) || !(*ox <= p1x)) return 0; } // 0058FB26..0058FB51
        if (hit != 1) return hit;                                         // 0058FB57..0058FB5A
        if (q2y >= q1y) { if (!(y >= q1y) || !(y <= q2y)) hit = 0; }      // 0058FB60..0058FB8B
        if (!(q1y >= q2y)) return hit;                                    // 0058FB8D..0058FB9A
        if (!(y >= q2y)) return 0;                                        // 0058FBA0..0058FBAD
        return (y <= q1y) ? hit : 0;                                      // 0058FBB3..0058FCD4
    }
    const float m1f = static_cast<float>(m1);                             // 0058FBC0 (stored over p2y)
    const double m2 = static_cast<double>(d2y) / d2x;                     // 0058FBC4..0058FBC8
    if (m2 - m1f == 0.0) return 0;                                        // 0058FBCC..0058FBE6
    const double b2 = static_cast<double>(q1y) - static_cast<double>(q1x) * m2;     // 0058FBE9..0058FBF7
    const double b1 = static_cast<double>(p1y) - static_cast<double>(m1f) * p1x;    // 0058FBFB..0058FC03
    const double x = (b2 - b1) / (static_cast<double>(m1f) - m2);         // 0058FC07..0058FC0F
    *ox = static_cast<float>(x);                                          // 0058FC11..0058FC13
    *oy = static_cast<float>(x * m2 + b2);                                // 0058FC15..0058FC19
    if (q2x >= q1x && *ox >= q1x && *ox <= q2x) hit = 1;                  // 0058FC1F..0058FC48
    if (q1x >= q2x && *ox >= q2x && *ox <= q1x) hit = 1;                  // 0058FC4D..0058FC76
    else if (hit != 1) return hit;                                        // 0058FC7D..0058FC80
    if (p2x >= p1x) { if (!(*ox >= p1x) || !(*ox <= p2x)) hit = 0; }      // 0058FC82..0058FCAB
    if (!(p1x >= p2x)) return hit;                                        // 0058FCAD..0058FCBA
    if (!(*ox >= p2x)) return 0;                                          // 0058FCBC..0058FCC7
    return (*ox <= p1x) ? hit : 0;                                        // 0058FCC9..0058FCD4
}

// 0058CD80 CSWSTrigger::LineSegmentIntersectTrigger(Vector* from, Vector* to): thiscall RET 8.
U __fastcall w5_trigger_line_segment_intersect_0058cd80(void* trigger, void*, const Vector* from, const Vector* to) {
    void* area = recovered_object_area_004cb120(trigger, nullptr);        // 0058CD89
    if (area == nullptr) return 0;
    for (I i = 0; i < at<I>(trigger, 0x294); ++i) {                       // count re-read per edge
        const I count = at<I>(trigger, 0x294);
        const U* index = at<U*>(trigger, 0x298);
        const float* verts = at<float*>(trigger, 0x288);
        const I j = (i == count - 1) ? 0 : i + 1;
        const float* a = verts + index[i] * 3;
        const float* b = verts + index[j] * 3;
        float ox, oy;                                                     // [esp+1C] [esp+18]
        if (w5_area_intersect_line_segments_0058f840(area, nullptr, from->x, from->y, to->x, to->y, a[0], a[1], b[0], b[1], &ox, &oy) == 1)
            return 1;
    }
    return 0;
}

// 0058CE40 CSWSTrigger::InTrigger(Vector* p): thiscall RET 4.
U __fastcall w5_trigger_in_trigger_0058ce40(void* trigger, void*, const Vector* p) {
    if (at<U>(trigger, 0x2bc) != 0) {                                     // 0058CE40..0058CE4E: circular trigger
        const double dx = static_cast<double>(p->x) - at<float>(trigger, 0x90);
        const double dy = static_cast<double>(p->y) - at<float>(trigger, 0x94);
        const double dz = static_cast<double>(p->z) - at<float>(trigger, 0x98);
        const double distance2 = (dz * dz + dx * dx) + dy * dy;           // 0058CE6E..0058CE7C
        const double radius = recovered_trigger_primary_range_007a206c;
        return (radius * radius >= distance2) ? 1u : 0u;                 // fcompp r*r vs d2; test ah,1; jne -> 0
    }
    const I count = at<I>(trigger, 0x294);
    const U* index = at<U*>(trigger, 0x298);
    const float* verts = at<float*>(trigger, 0x288);
    return odd(polygon_crossings(count, [&](I k) { return verts + index[k] * 3; }, p->x, p->y));
}

// 005897D0 CSWSDoor::InDoor(Vector p by value): thiscall RET 0Ch. Polygon at +350 (Vector stride 0Ch), count +380.
U __fastcall w5_door_in_door_005897d0(void* door, void*, Vector p) {
    const I count = at<I>(door, 0x380);
    const float* points = reinterpret_cast<const float*>(static_cast<unsigned char*>(door) + 0x350);
    return odd(polygon_crossings(count, [&](I k) { return points + k * 3; }, p.x, p.y));
}

// explicit traps (W5-owned addresses not reconstructed in this checkpoint)

// 004BF470 CSWSArea::InSubAreas(Vector* p, CExoArrayList<ulong>* out): thiscall RET 8.
U __fastcall w5_area_in_sub_areas_004bf470(void* area, void*, const Vector* p, void* out_list) {
    List* out = static_cast<List*>(out_list);
    U result = 0, missing = k_object_invalid;                             // [esp+10] [esp+14]
    for (I i = 0; i < at<I>(area, 0x1a4); ++i) {
        const U id = at<U*>(area, 0x1a0)[i];
        if (out->size > 0 && list_contains(out, id)) continue;            // 004BF4A0..004BF4C2
        void* object = game_object(id);                                  // 004BF4DB
        if (object == nullptr) { missing = at<U*>(area, 0x1a0)[i]; continue; }
        U inside = 0;
        if (vcall<void*>(object, 0x40) != nullptr) {
            inside = w5_trigger_in_trigger_0058ce40(vcall<void*>(object, 0x40), nullptr, p);           // 004BF511
        } else if (vcall<void*>(object, 0x50) != nullptr) {
            inside = recovered_aoe_in_area_of_effect_00595450(vcall<void*>(object, 0x50), nullptr, *p);       // 004BF543
        } else if (vcall<void*>(object, 0x5c) != nullptr) {
            inside = recovered_encounter_in_activate_area_00590090(vcall<void*>(object, 0x5c), nullptr, p);   // 004BF563
        }
        if (inside != 0) {                                                // 004BF568..004BF580
            gui_load_array_add_00671c00(static_cast<kotor_reconstruction::ArrayMutations*>(out_list), nullptr, at<U*>(area, 0x1a0)[i]);
            result = 1;
        }
        if (vcall<void*>(object, 0x18) != nullptr &&                       // 004BF585..004BF5B9
            w5_door_in_door_005897d0(vcall<void*>(object, 0x18), nullptr, *p) != 0) {
            gui_load_array_add_00671c00(static_cast<kotor_reconstruction::ArrayMutations*>(out_list), nullptr, at<U*>(area, 0x1a0)[i]);
            result = 1;
        }
    }
    if (missing != k_object_invalid) {                                    // 004BF5E3..004BF637: drop the stale id
        const I size = at<I>(area, 0x1a4);
        I k = 0;
        while (k < at<I>(area, 0x1a4) && at<U*>(area, 0x1a0)[k] != missing) ++k;
        at<I>(area, 0x1a4) = size - 1;                                    // decremented even when not found
        for (; k < at<I>(area, 0x1a4); ++k) at<U*>(area, 0x1a0)[k] = at<U*>(area, 0x1a0)[k + 1];
    }
    return result;
}

// 004BF2C0 CSWSArea::IntersectingLineSegment(Vector* a, Vector* b, CExoArrayList<ulong>* out): thiscall RET 0Ch.
U __fastcall w5_area_intersecting_line_segment_004bf2c0(void* area, void*, const Vector* a, const Vector* b, void* out_list) {
    List* out = static_cast<List*>(out_list);
    U result = 0;
    for (I i = 0; i < at<I>(area, 0x1a4); ++i) {
        void* object = game_object(at<U*>(area, 0x1a0)[i]);              // 004BF302
        if (object == nullptr) continue;
        if (out->size > 0 && list_contains(out, at<U*>(area, 0x1a0)[i])) continue;   // 004BF311..004BF34A
        if (vcall<void*>(object, 0x40) != nullptr) {                      // trigger: inline append with growth
            if (w5_trigger_line_segment_intersect_0058cd80(vcall<void*>(object, 0x40), nullptr, a, b) == 0) continue;
            const U id = at<U*>(area, 0x1a0)[i];
            result = 1;
            if (out->size == out->capacity)                               // 004BF384..004BF3A4
                menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(out_list), nullptr,
                    out->capacity ? static_cast<unsigned>(out->capacity) * 2u : 0x10u);
            out->data[out->size] = id;
            ++out->size;
            continue;
        }
        U crosses = 0;
        if (vcall<void*>(object, 0x50) != nullptr)
            crosses = recovered_aoe_segment_intersect_005950c0(vcall<void*>(object, 0x50), nullptr, *a, *b);   // 004BF3FF
        else if (vcall<void*>(object, 0x5c) != nullptr)
            crosses = recovered_encounter_segment_intersect_0058ffd0(vcall<void*>(object, 0x5c), nullptr, a, b); // 004BF424
        else continue;
        if (crosses != 0) {                                               // 004BF429..004BF441
            gui_load_array_add_00671c00(static_cast<kotor_reconstruction::ArrayMutations*>(out_list), nullptr, at<U*>(area, 0x1a0)[i]);
            result = 1;
        }
    }
    return result;
}

// 00516020 CSWSCreature::HandleSubAreaStateChanges(List* now, List* crossed, U triggerFlagCheck, U fireEvents):
// thiscall RET 10h. Script events: 0Ch enter, 0Dh exit, 1Eh (door / flagged trigger enter).
U __fastcall w5_creature_handle_sub_area_state_changes_00516020(void* creature, void*, void* now_list, void* crossed_list,
    U trigger_check, U fire) {
    const List* now = static_cast<const List*>(now_list);
    const List* crossed = static_cast<const List*>(crossed_list);
    void* array = object_array();                                         // 00516046
    U result = 0;                                                         // [esp+14]
    const U self_id = at<U>(creature, 4);
    auto lookup = [&](U id, void** out) { return static_cast<unsigned char>(kotor_fastcall_004d8230(array, nullptr, id, out)) == 0; };
    auto enter_type = [&](void* object) -> U16 {                         // 005163CD..00516411 / 00516575..005165B9
        if (trigger_check != 0 && vcall<void*>(object, 0x40) != nullptr && at<U>(vcall<void*>(object, 0x40), 0x2b4) != 0) return 0x1e;
        return vcall<void*>(object, 0x18) != nullptr ? 0x1e : 0x0c;
    };
    // (1) left: in the creature's list but not in `now`
    for (I k = 0; k < at<I>(creature, 0x334); ++k) {
        const U id = at<U*>(creature, 0x330)[k];
        if (now->size > 0 && list_contains(now, id)) continue;            // 00516067..0051608D
        void* object = nullptr;
        if (!lookup(id, &object)) continue;                               // 005160A6..005160BC (other array fetch is the same)
        if (vcall<void*>(object, 0x18) == nullptr && fire != 0)           // 005160C2..005160D5
            queue_script_event(0x0d, self_id, at<U*>(creature, 0x330)[k], self_id);
        result = 1;                                                       // 00516146
    }
    // (2) entered: in `now` but not in the creature's list
    for (I k = 0; k < now->size; ++k) {
        const U id = now->data[k];
        if (creature_has(creature, id)) continue;                         // 0051617D..0051619D
        U entered = 1;                                                    // EBX
        void* object = nullptr;
        if (lookup(id, &object) && fire != 0) {                           // 005161A3..005161CD
            void* memory = live_operator_new(0x34);
            void* event = memory ? recovered_event_construct_004d7540(memory, nullptr) : nullptr;
            bool special = trigger_check != 0 && vcall<void*>(object, 0x40) != nullptr && at<U>(vcall<void*>(object, 0x40), 0x2b4) != 0;
            if (!special && vcall<void*>(object, 0x18) == nullptr) {
                at<U16>(event, 0) = 0x0c;                                 // 0051638B
            } else if (at<U>(creature, 0xa88) != 0) {                     // 00516237
                void* client = client_app();
                const U client_id = kotor_fastcall_005eda50(client, nullptr, self_id);
                void* party = kotor_fastcall_005ed8b0(client, nullptr);
                if (recovered_party_index_006349c0(party, nullptr, client_id) == 0) {
                    at<U16>(event, 0) = 0x1e;                             // 0051626C
                } else if (at<U>(creature, 0xa88) != 0) {                 // 00516276..00516298: non-leader: no event
                    entered = 0;
                    if (event != nullptr) { recovered_event_destroy_004d7590(event, nullptr); live_free(event); }
                    result = entered;
                    continue;
                }
            }                                                             // (+A88 == 0: type left as constructed)
            recovered_event_object_004d7780(event, nullptr, 0, self_id);  // 00516390..005163BD
            recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, 0, self_id, id, k_event_queue_type_007441dc, event);
        }
        result = entered;                                                 // 0051629A
    }
    // (3) crossed during the step
    if (crossed != nullptr) {
        for (I k = 0; k < crossed->size; ++k) {
            const U id = crossed->data[k];
            const bool was = creature_has(creature, id);
            const bool is = now->size > 0 && list_contains(now, id);
            void* object = nullptr;
            if (was) {
                if (!is) continue;                                        // 00516491..005164AE
                if (!lookup(id, &object) || fire == 0) continue;
                queue_script_event(0x0d, self_id, id, self_id);           // 005164E7..00516549 exit
                queue_script_event(enter_type(object), self_id, id, self_id); // 0051654E..005165EC re-enter
            } else {
                if (is) continue;                                         // 00516313..0051632D
                if (!lookup(id, &object) || fire == 0) continue;
                queue_script_event(enter_type(object), self_id, id, self_id); // 00516366..00516443 enter
                queue_script_event(0x0d, self_id, id, self_id);           // 00516448..005165EC exit
            }
        }
    }
    return result;
}

// 0051B7B0 CSWSCreature::UpdateSubareasOnMoveTo(Vector* from, Vector* to, U useCrossed, List* crossed, U triggerCheck):
// thiscall RET 14h. EAX = 1 when the creature's subarea list (+330) was replaced.
U __fastcall w5_creature_update_subareas_on_move_to_0051b7b0(void* creature, void*, const Vector* from, const Vector* to,
    U use_crossed, void* crossed, U trigger_check) {
    List now{nullptr, 0, 0};                                              // [B+14]
    U changed = 0;                                                        // [B+C]
    (void)object_array();                                                 // 0051B7E7 (result unused)
    void* area = recovered_object_area_004cb120(creature, nullptr);
    if (area == nullptr) { live_free(now.data); return 0; }               // 0051B7F9..0051B821
    const U had_any = at<I>(creature, 0x334) > 0 ? 1u : 0u;               // 0051B829..0051B833 (setg)
    const U in_any = w5_area_in_sub_areas_004bf470(area, nullptr, to, &now); // 0051B844
    if (use_crossed == 0) {
        List swept{nullptr, 0, 0};                                        // [B+20]
        w5_area_intersecting_line_segment_004bf2c0(area, nullptr, from, to, &swept); // 0051B875
        if (had_any != 0 || in_any != 0) {
            if (w5_creature_handle_sub_area_state_changes_00516020(creature, nullptr, &now, &swept, trigger_check, 1) == 1) {
                kotor_fastcall_0051b720(static_cast<unsigned char*>(creature) + 0x330, nullptr, &now);
                changed = 1;
            }
        }
        live_free(swept.data);                                            // 0051B8B9..0051B8C2
    } else if (had_any != 0 || in_any != 0) {
        if (w5_creature_handle_sub_area_state_changes_00516020(creature, nullptr, &now, crossed, trigger_check, 1) == 1) {
            kotor_fastcall_0051b720(static_cast<unsigned char*>(creature) + 0x330, nullptr, &now);
            changed = 1;
        }
    }
    live_free(now.data);                                                  // 0051B90D..0051B91A
    return changed;
}
}
