// lane-3d-bsp: BSP tree (004ACEA0..004AD7E0) and scene BSP bookkeeping (0046A680..0046ED00) of swkotor.exe
// (GOG K1, image base 00400000). Evidence: dumpbin /disasm of the pinned image (lane-3d-bsp/d/<ADDR>.txt, lane-local);
// the instruction addresses reproduced by each statement are cited inline.
// Floating point (D30-FINAL): every fld/fmul/fadd chain is evaluated in double in the original operand order;
// values the original stores with `fstp dword` are rounded to float at that point and only there; compare idioms
// follow the predicate table in bsp.h (NaN takes the same branch as on the x87).
// Unwind: the original EH frames of 004AD630 (handler 00718346), 004AD6D0 (00718358), 0046E5C0 (00716E18) and
// 0046BFB0 (00716DF9) are cited, not emitted: every callee is extern "C" (non-throwing under /EHsc) and the M41
// operator new returns NULL instead of throwing (D30-FINAL unwind clause).
#include "11b317539a-bsp.h"
#include <cmath>

namespace {
using namespace kotor_scene3d_bsp;

constexpr U kZero_0073d700 = 0x00000000u;     // .rdata 0.0f (fcomp operand at 004AD027, 004AD1F2, ...)
constexpr U kBoxEpsilon_00744104 = 0xBCA3D70Au; // .rdata -0.02f (004AD137 / 004AD164)
constexpr U kHalfDivisor = 0x40000000u;       // push 40000000h (2.0f) at 0046CEB0
constexpr U kOne = 0x3F800000u;               // immediate 1.0f (0046B52C, 0046E2ED, 0046C56A, 0046E6AF)
constexpr char kFocusGob_007416fc[11] = {'c', '_', 'F', 'o', 'c', 'u', 's', 'G', 'o', 'b', '\0'}; // repe cmpsb, ecx=0Bh

inline double zero() { return Wb(kZero_0073d700); }
inline void* child_front(void* node) { return field<void*>(node, 0x10); }   // BSPNode+10 (right_child)
inline void* child_back(void* node) { return field<void*>(node, 0x14); }    // BSPNode+14 (left_child)
inline U leaf(void* node) { return field<U>(node, 0x18); }                  // BSPNode+18
inline void* node_data(void* node) { return field<void*>(node, 0x58); }     // BSPNode+58 nodedata*
inline float nf(void* node, U offset) { return field<float>(node, offset); } // plane a,b,c,d at +0/+4/+8/+C
inline Vector vector_at(void* object, U offset) { Vector v; std::memcpy(&v, at(object, offset), sizeof v); return v; }
inline void* scene_bsp(void* scene) { return field<void*>(scene, 0xA0); }   // Scene+A0 BSPNode* bsp
inline void* light_manager(void* scene) { return field<void*>(scene, 0x80); } // Scene+80 LightManager*
inline PointerList* scene_rooms(void* scene) { return list_at(scene, 0xC8); } // Scene+C8 CExoArrayList<CAurRoom*>
inline PointerList* scene_objects(void* scene) { return list_at(scene, 0x54); } // Scene+54 CExoArrayList<Gob*>
inline void delete_object(void* object) { slots<DeletingSlot>(object).destroy(object, nullptr, 1u); } // [vtbl+0](1)
inline void* camera() { return *bsp_cells.current_camera_007bb4e8; }       // [007BB4E8]
} // namespace

extern "C" {

// ================================================================ BSP tree

// 004ACEA0 BSPTraverse(node, fn, context): pre-order walk, recursion on +10, iteration on +14; callback EAX ignored.
void __cdecl bsp_traverse_004acea0(void* node, void (__cdecl* fn)(void* node, void* context), void* context) {
    if (node == nullptr) return;                                         // 004ACEA5..004ACEA7
    do {
        fn(node, context);                                               // 004ACEB3..004ACEB5
        bsp_traverse_004acea0(child_front(node), fn, context);          // 004ACEB7..004ACEBD ([esi+10h] after fn)
        node = child_back(node);                                         // 004ACEC2
    } while (node != nullptr);                                           // 004ACEC8..004ACECA
}

// 004ACF60 BSPTraverse(node, center, radius_sq, fn): sphere-limited walk (r = fsqrt(radius_sq) rounded to float).
void __cdecl bsp_traverse_sphere_004acf60(void* node, const Vector* center, float radius_sq, BspVisitSphere fn) {
    if (node == nullptr) return;                                         // 004ACF68..004ACF6A
    const float radius = S(std::sqrt(W(radius_sq)));                     // 004ACF6C fld; 004ACF71 fsqrt; 004ACF81 fstp
    const float negative_radius = -radius;                               // 004ACF85..004ACF8B fld; fchs; fstp
    do {
        fn(node, center, radius_sq);                                     // 004ACF90..004ACF93 (raw [esp+18h] DWORD)
        const double distance = ((W(nf(node, 8)) * W(center->z) + W(nf(node, 4)) * W(center->y)) +
                                 W(nf(node, 0)) * W(center->x)) + W(nf(node, 0x0C));  // 004ACF95..004ACFAC
        const float stored = S(distance);                                // 004ACFAF fst dword [esp+1Ch]
        if (distance > W(negative_radius))                               // 004ACFB3..004ACFBC test ah,41h; jne
            bsp_traverse_sphere_004acf60(child_back(node), center, radius_sq, fn); // 004ACFBE..004ACFC5
        if (!(W(stored) < W(radius))) return;                            // 004ACFCD..004ACFDA fld [esp+1Ch]; test ah,5; jp
        node = child_front(node);                                        // 004ACFDC
    } while (node != nullptr);                                           // 004ACFDF..004ACFE1
}

// 004ACFF0 RFaceSplitTest(CExoArrayList<Vector>*, Plane*): bit 1 = some vertex strictly behind, bit 2 = some vertex
// on/in front (or unordered); stops as soon as both bits are set.
U __cdecl rface_split_test_004acff0(const PointerList* face, const Plane* plane) {
    U sides = 0;                                                         // 004ACFFA xor esi,esi
    const I count = face->size;                                          // 004ACFF7
    const Vector* vertex = reinterpret_cast<const Vector*>(static_cast<std::uintptr_t>(face->data)); // 004AD006
    for (I index = 0; index < count; ++index, ++vertex) {               // 004ACFFC..004AD04B
        const double distance = ((W(vertex->z) * W(plane->c) + W(vertex->y) * W(plane->b)) +
                                 W(vertex->x) * W(plane->a)) + W(plane->d);   // 004AD010..004AD024
        sides |= (distance < zero()) ? 1u : 2u;                          // 004AD027..004AD03E test ah,5; jnp
        if (sides == 3) break;                                           // 004AD040..004AD043
    }
    return sides;                                                        // 004AD04E
}

// 004AD060 BSPFitSphere(node, center, radius): descend while the sphere lies entirely on one side.
void* __cdecl bsp_fit_sphere_004ad060(void* node, const Vector* center, float radius) {
    if (leaf(node) != 0) return node;                                    // 004AD064..004AD069
    const float negative_radius = -radius;                               // 004AD06B..004AD075 fld; fchs; fstp [esp+4]
    do {
        const double distance = ((W(nf(node, 8)) * W(center->z) + W(nf(node, 4)) * W(center->y)) +
                                 W(nf(node, 0)) * W(center->x)) + W(nf(node, 0x0C));  // 004AD080..004AD094
        if (distance <= W(negative_radius)) {                            // 004AD097..004AD0A0 test ah,41h; jp
            node = child_front(node);                                    // 004AD0A2
        } else {
            if (!(distance >= W(radius))) return node;                   // 004AD0A9..004AD0B2 test ah,1; jne
            node = child_back(node);                                     // 004AD0B4
        }
    } while (leaf(node) == 0);                                           // 004AD0B7..004AD0BC
    return node;                                                         // 004AD0BE
}

// 004AD0D0 BSPFitBox(node, min, max): descend while BoxAbovePlane(-plane) or BoxAbovePlane(plane) >= -0.02.
void* __cdecl bsp_fit_box_004ad0d0(void* node, const Vector* minimum, const Vector* maximum) {
    if (leaf(node) != 0) return node;                                    // 004AD0D8..004AD0E5
    Plane flipped;                                                       // [esp+18h] (four fchs/fstp, 004AD0E6..004AD12E)
    flipped.a = -nf(node, 0);
    flipped.b = -nf(node, 4);
    flipped.c = -nf(node, 8);
    flipped.d = -nf(node, 0x0C);
    if (box_above_plane_004aaf50(minimum, maximum, &flipped) >= Wb(kBoxEpsilon_00744104))  // 004AD132..004AD145
        return bsp_fit_box_004ad0d0(child_front(node), minimum, maximum); // 004AD147..004AD15B
    if (box_above_plane_004aaf50(minimum, maximum, static_cast<const Plane*>(node)) >= Wb(kBoxEpsilon_00744104)) // 004AD15C..004AD172
        return bsp_fit_box_004ad0d0(child_back(node), minimum, maximum);  // 004AD174..004AD188
    return node;                                                         // 004AD189..004AD191
}

// 004AD1A0 BSPSearch(node, point, fn, context): first node for which fn != 0, nearer side first.
void* __cdecl bsp_search_004ad1a0(void* node, Vector point, BspSearch fn, void* context) {
    if (fn(node, context) != 0) return node;                             // 004AD1AC..004AD1B7 -> 004AD24E
    for (;;) {
        if (leaf(node) != 0) return nullptr;                             // 004AD1D0..004AD1D5 -> 004AD255
        const double distance = ((W(point.y) * W(nf(node, 4)) + W(point.z) * W(nf(node, 8))) +
                                 W(point.x) * W(nf(node, 0))) + W(nf(node, 0x0C));  // 004AD1D7..004AD1EF
        void* nearer;
        void* other;
        if (distance < zero()) {                                         // 004AD1F2..004AD1FD test ah,5; jp
            nearer = child_front(node); other = child_back(node);        // 004AD1FF..004AD202
        } else {
            nearer = child_back(node); other = child_front(node);        // 004AD207..004AD20A
        }
        void* found = bsp_search_004ad1a0(nearer, point, fn, context);   // 004AD20D..004AD225
        if (found != nullptr) return found;                              // 004AD22D..004AD22F -> 004AD257
        node = other;
        if (fn(node, context) != 0) return node;                         // 004AD231..004AD24C
    }
}

// 004AD260 BSPHitCheck(node, start, end, hit, fn): segment walk; fn may shorten `end` through its out pointer.
U __cdecl bsp_hit_check_004ad260(void* node, Vector start, Vector end, Vector* hit, BspHit fn) {
    U result = 0;                                                        // 004AD26A xor ebp,ebp
    if (node == nullptr) return 0;                                       // 004AD26C..004AD27A
    if (fn != nullptr) {                                                 // 004AD27F..004AD285
        result = fn(node, start, end, &end);                             // 004AD287..004AD2C1 (&[esp+34h] = end)
        if (result != 0) {                                               // 004AD2C6..004AD2C8
            *hit = end;                                                  // 004AD2CA..004AD2DD (no null test)
            result = 1;                                                  // 004AD2E0
        }
    }
    const U leaf_flag = leaf(node);                                      // 004AD2EF
    if (leaf_flag != 0) {                                                // 004AD2F2..004AD2F4
        if (fn == nullptr && leaf_flag == 1) {                           // 004AD2F6..004AD2FD
            if (hit != nullptr) *hit = start;                            // 004AD2FF..004AD314
            return 1;                                                    // 004AD317
        }
        return result;                                                   // 004AD324
    }
    const double start_distance = ((W(start.y) * W(nf(node, 4)) + W(start.z) * W(nf(node, 8))) +
                                   W(start.x) * W(nf(node, 0))) + W(nf(node, 0x0C));   // 004AD32E..004AD34E
    const U start_front = (start_distance > zero()) ? 1u : 0u;          // 004AD332 / 004AD351..004AD35E
    const double end_distance = ((W(end.y) * W(nf(node, 4)) + W(end.z) * W(nf(node, 8))) +
                                 W(end.x) * W(nf(node, 0))) + W(nf(node, 0x0C));       // 004AD366..004AD386
    const U end_front = (end_distance > zero()) ? 1u : 0u;              // 004AD36A / 004AD389..004AD396
    if (start_front == 0 && end_front == 0)                              // 004AD39E..004AD3AC
        return bsp_hit_check_004ad260(child_front(node), start, end, hit, fn) | result; // 004AD3AE..004AD3D4, 004AD541..004AD555
    if (start_front == 1 && end_front == 1)                              // 004AD3D9..004AD3E3
        return bsp_hit_check_004ad260(child_back(node), start, end, hit, fn) | result;  // 004AD3E5..004AD411, 004AD547..004AD555
    Vector split;                                                        // [esp+14h]
    plane_line_intersection_004ab250(&split, static_cast<const Vector*>(node), nf(node, 0x0C), &start, &end); // 004AD416..004AD42A
    if (start_front == 0) {                                              // 004AD436..004AD43D
        if (bsp_hit_check_004ad260(child_front(node), start, split, hit, fn) != 0) return 1; // 004AD443..004AD480 -> 004AD317
        return bsp_hit_check_004ad260(child_back(node), split, end, hit, fn) | result;       // 004AD486..004AD4C6
    }
    if (bsp_hit_check_004ad260(child_back(node), start, split, hit, fn) != 0) return 1;      // 004AD4D0..004AD50D -> 004AD317
    return bsp_hit_check_004ad260(child_front(node), split, end, hit, fn) | result;          // 004AD513..004AD555
}

// 004AD560 BSPSphereCheck(bsp, center, radius, out): collect every leaf the sphere touches into out.
void __cdecl bsp_sphere_check_004ad560(void* bsp, const void* center, float radius, kotor_reconstruction::ArrayMutations* out) {
    void* node = bsp;                                                    // 004AD566
    if (leaf(node) == 0) {                                               // 004AD56A..004AD56F
        const Vector* c = static_cast<const Vector*>(center);            // 004AD581
        const float negative_radius = -radius;                           // 004AD571..004AD57D fld; fchs; fstp [esp+14h]
        do {
            const double distance = ((W(nf(node, 8)) * W(c->z) + W(nf(node, 4)) * W(c->y)) +
                                     W(nf(node, 0)) * W(c->x)) + W(nf(node, 0x0C));   // 004AD585..004AD599
            if (distance <= W(negative_radius)) {                        // 004AD59C..004AD5A5 test ah,41h; jp
                node = child_front(node);                                // 004AD5A7
            } else {
                if (!(distance >= W(radius)))                            // 004AD5AE..004AD5B7 test ah,1; je
                    bsp_sphere_check_004ad560(child_front(node), center, radius, out); // 004AD5B9..004AD5C0
                node = child_back(node);                                 // 004AD5C8
            }
        } while (leaf(node) == 0);                                       // 004AD5CB..004AD5D0
    }
    int_list_add_00444fe0(out, nullptr, static_cast<U>(reinterpret_cast<std::uintptr_t>(node))); // 004AD5D4..004AD5D7
}

// 004AD630 BSPNode::BSPNode(Vector normal, int distance_bits), RET 10h, EAX = this.
void* __fastcall bsp_node_construct_004ad630(void* self, void*, Vector v, I flag) {
    std::memcpy(at(self, 0x00), &v, sizeof v);                           // 004AD64D..004AD660 (raw DWORDs)
    field<I>(self, 0x0C) = flag;                                         // 004AD663
    field<U>(self, 0x1C) = 0; field<U>(self, 0x20) = 0; field<U>(self, 0x24) = 0;   // 004AD669..004AD66F bbox_min
    field<U>(self, 0x28) = 0; field<U>(self, 0x2C) = 0; field<U>(self, 0x30) = 0;   // 004AD67A..004AD680 bbox_max
    plane_list_construct_0045d790(static_cast<PlaneList*>(at(self, 0x34)), nullptr, 0); // 004AD672..004AD683 (EH state 0)
    plane_list_construct_0045d790(static_cast<PlaneList*>(at(self, 0x40)), nullptr, 0); // 004AD688..004AD690 (state 1)
    vector_list_construct_00421fc0(list_at(self, 0x4C), nullptr, 0);    // 004AD695..004AD69E (state 2)
    field<U>(self, 0x10) = 0; field<U>(self, 0x14) = 0; field<U>(self, 0x18) = 0;   // 004AD6A7..004AD6AD
    field<U>(self, 0x58) = 0;                                            // 004AD6B0
    return self;                                                         // 004AD6B4
}

// 004AD6D0 BSPTraverse(node, point, fn, context): front-to-back from the point, each node visited with an empty
// local CExoArrayList<Plane> (constructed at 004AD6FE, destroyed by 0047ABC0 at 004AD7BC).
void __cdecl bsp_traverse_point_004ad6d0(void* node, Vector point, BspVisitPlanes fn, void* context) {
    if (node == nullptr) return;                                         // 004AD6EF..004AD6F2
    PlaneList planes;                                                    // [esp+10h]
    plane_list_construct_0045d790(&planes, nullptr, 0);                  // 004AD6F8..004AD6FE
    fn(node, context, &planes);                                          // 004AD70B..004AD71A
    const double distance = ((W(point.y) * W(nf(node, 4)) + W(point.z) * W(nf(node, 8))) +
                             W(point.x) * W(nf(node, 0))) + W(nf(node, 0x0C));   // 004AD71C..004AD737
    void* second = (distance > zero()) ? child_back(node) : child_front(node);   // 004AD73A..004AD753 ([esp+2Ch])
    void* first = (distance > zero()) ? child_front(node) : child_back(node);    // 004AD757..004AD769
    bsp_traverse_point_004ad6d0(first, point, fn, context);              // 004AD76C..004AD788
    bsp_traverse_point_004ad6d0(second, point, fn, context);             // 004AD78D..004AD7A8
    exo_array_list_free_0047abc0(&planes, nullptr);                      // 004AD7B0..004AD7BC
}

// 004AD7E0 BSPTraverseVolumeApprox(node, point, planes, fn, context): frustum walk; clips the incoming plane set
// against each node's face (+4C) into the node's two plane lists (+34 / +40) and tail-iterates on the far child.
void __cdecl bsp_traverse_volume_approx_004ad7e0(void* node, const Vector* point, PlaneList* planes,
                                                 BspVisitPlanes fn, void* context) {
    if (node == nullptr) return;                                         // 004AD7EC..004AD7EF
    for (;;) {
        fn(node, context, planes);                                       // 004AD800..004AD80B
        if (leaf(node) != 0) return;                                     // 004AD80F..004AD817
        PlaneList* front_list = static_cast<PlaneList*>(at(node, 0x34));
        PlaneList* back_list = static_cast<PlaneList*>(at(node, 0x40));
        front_list->count = 0;                                           // 004AD81D [esi+38h]
        back_list->count = 0;                                            // 004AD820 [esi+44h]
        U8 back_hit = 0;                                                 // 004AD826 dl -> [esp+13h]
        U8 front_hit = 0;                                                // 004AD828 bl -> [esp+24h]
        I remaining = planes->count;                                     // 004AD823
        if (remaining != 0) {                                            // 004AD82A..004AD834
            U offset = static_cast<U>(remaining) << 4;                   // 004AD83A..004AD83C
            do {
                --remaining;                                             // 004AD852
                offset -= 0x10;                                          // 004AD853
                const Plane* plane = reinterpret_cast<const Plane*>(reinterpret_cast<unsigned char*>(planes->data) + offset); // 004AD856
                const U split = rface_split_test_004acff0(list_at(node, 0x4C), plane); // 004AD85D..004AD862
                if (split == 3) {                                        // 004AD86C..004AD86F
                    plane_list_add_004ad5e0(back_list, nullptr, *plane); // 004AD871..004AD88F
                    plane = reinterpret_cast<const Plane*>(reinterpret_cast<unsigned char*>(planes->data) + offset); // 004AD894..004AD89A
                    plane_list_add_004ad5e0(front_list, nullptr, *plane); // 004AD89C..004AD8BA
                }
                plane = reinterpret_cast<const Plane*>(reinterpret_cast<unsigned char*>(planes->data) + offset);     // 004AD8BF..004AD8C9
                const double facing = (W(plane->b) * W(nf(node, 4)) + W(plane->c) * W(nf(node, 8))) +
                                      W(plane->a) * W(nf(node, 0));      // 004AD8C5..004AD8DB
                const bool front = facing > zero();                      // 004AD8DD..004AD8EE
                if (split == 1) {                                        // 004AD8F0..004AD8F3
                    if (front) {                                         // 004AD8F9..004AD8FB
                        if (front_hit == 0) {                            // 004AD8FD..004AD903
                            plane_list_add_004ad5e0(back_list, nullptr, *plane);   // 004AD905..004AD923
                            continue;                                    // 004AD928 -> 004AD969
                        }
                        if (back_hit != 0) return;                       // 004AD95D..004AD963
                        continue;
                    }
                    if (back_hit == 0) plane_list_add_004ad5e0(front_list, nullptr, *plane); // 004AD92A..004AD950
                } else if (split == 2) {                                 // 004AD9B9..004AD9BC
                    if (front) {                                         // 004AD9BE..004AD9C0
                        front_hit = 1;                                   // 004AD9C8
                        if (back_hit != 0) return;                       // 004AD9C2..004AD9CD
                        plane_list_add_004ad5e0(front_list, nullptr, *plane);      // 004AD9D3..004AD9F1
                        continue;                                        // 004AD9F6
                    }
                    back_hit = 1;                                        // 004ADA01
                    if (front_hit != 0) return;                          // 004AD9FB..004ADA06
                    plane_list_add_004ad5e0(back_list, nullptr, *plane);           // 004ADA0C..004ADA2A
                    continue;                                            // 004ADA2F
                }
                if (front_hit != 0 && back_hit != 0) return;             // 004AD955..004AD963
            } while (remaining != 0);                                    // 004AD969..004AD96F
        }
        const double distance = ((W(nf(node, 8)) * W(point->z) + W(nf(node, 4)) * W(point->y)) +
                                 W(nf(node, 0)) * W(point->x)) + W(nf(node, 0x0C));   // 004AD97D..004AD995
        const float stored = S(distance);                                // 004AD998 fst dword [esp+24h]
        void* near_child = (distance > zero()) ? child_back(node) : child_front(node); // 004AD99C..004ADA37 [esp+14h]
        const bool front = W(stored) > zero();                           // 004ADA3B.. (every later test reloads [esp+24h])
        void* far_child = front ? child_front(node) : child_back(node);  // 004ADA3B..004ADA58 [esp+18h]
        const I near_count = front ? back_list->count : front_list->count;   // 004ADA5C..004ADA72 ebp
        const I far_count = front ? front_list->count : back_list->count;    // 004ADA75..004ADA92 [esp+1Ch]
        const U8 near_done = front ? front_hit : back_hit;               // 004ADA96..004ADAA9 cl
        const U8 far_done = front ? back_hit : front_hit;                // 004ADAAB..004ADABC bl
        if (near_done == 0) {                                            // 004ADABE..004ADAC0
            if (near_count != 0) {                                       // 004ADAC2..004ADAC4
                PlaneList* near_planes = front ? back_list : front_list; // 004ADAC6..004ADADA
                bsp_traverse_volume_approx_004ad7e0(near_child, point, near_planes, fn, context); // 004ADADD..004ADAEE
            } else {
                bsp_traverse_point_004ad6d0(near_child, *point, fn, context);  // 004ADAF8..004ADB1E
            }
        }
        if (far_done != 0) return;                                       // 004ADB26..004ADB28
        if (far_count == 0) {                                            // 004ADB2A..004ADB30
            bsp_traverse_point_004ad6d0(far_child, *point, fn, context); // 004ADB63..004ADB87
            return;
        }
        planes = front ? front_list : back_list;                         // 004ADB32..004ADB4B ([esp+2Ch] = esi+34h/40h)
        node = far_child;                                                // 004ADB4F
        if (node == nullptr) return;                                     // 004ADB53..004ADB55
    }
}

// ================================================================ scene BSP

// 0046A680 SceneBSPGetTileData(scene, point): nodedata(+58)->tile_data(+70) of the leaf containing the point.
void* __cdecl scene_bsp_get_tile_data_0046a680(void* scene, const Vector* point) {
    void* bsp = scene_bsp(scene);                                        // 0046A680..0046A684
    if (bsp == nullptr) return bsp;                                      // 0046A68A..0046A68E (EAX = 0)
    void* node = bsp_fit_sphere_004ad060(bsp, point, bits_to_float(0));  // 0046A68F..0046A697 (push 0)
    return field<void*>(node_data(node), 0x70);                          // 0046A69C..0046A69F
}

// 0046B4C0 HitPartOnBSPNode(node, start, end, hit): BSPHitCheck callback over triggers / trimeshes / AABB.
U __cdecl hit_part_on_bsp_node_0046b4c0(void* node, Vector start, Vector end, Vector* hit) {
    bsp_hit_calls_007fbf58 = bsp_hit_calls_007fbf58 + 1;                 // 0046B4C3..0046B4D2
    void* data = node_data(node);                                        // 0046B4D8
    U result = 0;                                                        // 0046B4DF [esp+10h]
    if (data == nullptr) return 0;                                       // 0046B4DD..0046B4EE
    for (I index = 0; index < list_at(data, 0x54)->size; ++index) {      // 0046B4EF..0046B59A (triggers, +54)
        void* trigger = elements(list_at(data, 0x54))[index];            // 0046B500..0046B503
        if (box_intersect_004aa610(&start, &end, static_cast<const Vector*>(at(trigger, 0x5C)),
                                   static_cast<const Vector*>(at(trigger, 0x68))) == 0) continue; // 0046B506..0046B522
        if (slots<PartSlots>(trigger).hit_check(trigger, nullptr, start, end, &end, *bsp_cells.hit_mask_007fbf48,
                                                bits_to_float(kOne)) == 0) continue;               // 0046B524..0046B570
        result = 1;                                                      // 0046B574
        if (hit != nullptr) *hit = end;                                  // 0046B572..0046B591
    }
    if (*bsp_cells.hit_trimeshes_007fbf4c != 0) {                        // 0046B5A0..0046B5A7
        for (I index = 0; index < list_at(data, 0x18)->size; ++index) {  // 0046B5AD..0046B65D (trimeshes, +18)
            void* mesh = elements(list_at(data, 0x18))[index];           // 0046B5C0..0046B5C3
            if (box_intersect_004aa610(&start, &end, static_cast<const Vector*>(at(mesh, 0x4C)),
                                       static_cast<const Vector*>(at(mesh, 0x58))) == 0) continue; // 0046B5C6..0046B5E2
            mesh = elements(list_at(data, 0x18))[index];                 // 0046B5ED..0046B5F0 (element reloaded)
            if (slots<PartSlots>(mesh).hit_check(mesh, nullptr, start, end, &end, *bsp_cells.hit_mask_007fbf48,
                                                 bits_to_float(kOne)) == 0) continue;              // 0046B5E4..0046B633
            result = 1;                                                  // 0046B637
            if (hit != nullptr) *hit = end;                              // 0046B635..0046B654
        }
    }
    const U ground = *bsp_cells.gate_0078e5a8;                           // 0046B663
    bsp_hit_plane_007fbf50 = 0;                                          // 0046B66A
    if (ground == 0) return result;                                      // 0046B668 / 0046B674 -> 0046B77A
    void* aabb = field<void*>(data, 0x6C);                               // 0046B67A
    if (aabb != nullptr) {                                               // 0046B67D..0046B67F
        const I hit_aabb = slots<PartSlots>(aabb).hit_check(aabb, nullptr, start, end, hit, *bsp_cells.hit_mask_007fbf48,
                                                            bits_to_float(kOne));                  // 0046B681..0046B6C2
        return (hit_aabb != 0 ? 1u : 0u) | result;                       // 0046B6C5..0046B6CF neg/sbb/neg/or
    }
    if (bsp_hit_ground_007fbf54 == 0) return result;                     // 0046B6D9..0046B6E0
    if (child_back(node) != nullptr) return result;                      // 0046B6E6..0046B6EF
    if (child_front(node) != nullptr) return result;                     // 0046B6F5..0046B6FA
    if (!(W(end.z) < zero())) return result;                             // 0046B6FC..0046B70B test ah,5; jp
    if (!(W(start.z) > zero())) return result;                           // 0046B70D..0046B71C test ah,41h; jne
    if (hit != nullptr) {                                                // 0046B71E..0046B720
        const Vector up = {bits_to_float(0), bits_to_float(0), bits_to_float(kOne)}; // 0046B738..0046B748
        Vector crossing;                                                 // [esp+20h]
        const Vector* point = plane_line_intersection_004ab250(&crossing, &up, bits_to_float(0), &start, &end); // 0046B722..0046B750
        *hit = *point;                                                   // 0046B755..0046B765
    }
    bsp_hit_plane_007fbf50 = 1;                                          // 0046B768..0046B76D
    return 1;
}

// 0046BBC0 AurProcessActiveBins: build grass polys of the active bins, destroy those of bins no longer active.
void __cdecl aur_process_active_bins_0046bbc0() {
    PointerList* active = bsp_cells.active_bins_007fbf9c;
    PointerList* previous = bsp_cells.old_active_bins_007fbffc;
    I active_size = active->size;                                        // 0046BBC0
    for (I index = 0; index < active_size; ++index) {                    // 0046BBC7..0046BBE6
        triangle_bin_build_grass_polys_004a9630(elements(active)[index], nullptr); // 0046BBD0..0046BBD8
        active_size = active->size;                                      // 0046BBDD
    }
    void** active_data = nullptr;
    if (previous->size > 0) active_data = elements(active);              // 0046BBF4 (read once before the loop)
    for (I index = 0; index < previous->size; ++index) {                 // 0046BBE8..0046BC33
        void* bin = elements(previous)[index];                           // 0046BC00..0046BC06
        bool found = false;
        for (I search = 0; search < active_size; ++search)               // 0046BC09..0046BC18
            if (active_data[search] == bin) { found = true; break; }     // 0046BC10..0046BC13
        if (found) continue;                                             // -> 0046BC2B
        triangle_bin_destroy_grass_polys_004a8460(bin, nullptr);         // 0046BC1A
        active_data = elements(active);                                  // 0046BC1F
        active_size = active->size;                                      // 0046BC25
    }
}

// 0046BC40 ClearSceneRooms(scene): reset the per-frame room lists (+74 gobs, +80 lights, +8C emitters) of every room.
void __cdecl clear_scene_rooms_0046bc40(void* scene) {
    for (I index = 0; index < scene_rooms(scene)->size; ++index) {       // 0046BC44..0046BC77
        void* data = node_data(elements(scene_rooms(scene))[index]);     // 0046BC53..0046BC5C
        field<U>(data, 0x84) = 0;                                        // 0046BC5F
        field<U>(data, 0x78) = 0;                                        // 0046BC65
        field<U>(data, 0x90) = 0;                                        // 0046BC68
    }
}

// 0046BD70 nodedata::AddObject(Gob*): CExoArrayList<Gob*> at +74, M41 growth.
void __fastcall nodedata_add_object_0046bd70(void* nodedata, void*, void* gob) {
    add_with_growth(list_at(nodedata, 0x74), gob);                       // 0046BD70..0046BD9D
}

// 0046BEF0 ClearBuckets(scene): empty every render bucket (sizes only) and roll the active-bin list.
void __cdecl clear_buckets_0046bef0(void*) {
    const I max_texture = aur_texture_get_max_tex_id_0041feb0();         // 0046BEF1
    bsp_cells.gob_bucket_007fbfcc->size = 0;                             // 0046BEFA
    bsp_cells.distortion_bucket_007fbfc0->size = 0;                      // 0046BF00
    bsp_cells.light_saber_bucket_007fbff0->size = 0;                     // 0046BF06
    bsp_cells.blur_bucket_007fbf90->size = 0;                            // 0046BF0C
    bsp_cells.emitter_bucket_007fbf78->size = 0;                         // 0046BF12
    if (max_texture >= 0) {                                              // 0046BF18 jl
        for (I index = 0; index <= max_texture; ++index)                 // 0046BF1A..0046BF28
            bsp_cells.cap_buckets_007fc008[index].size = 0;              // 0046BF22 [7FC00C + 0Ch*i]
        for (I index = 0; index <= max_texture; ++index) {               // 0046BF2E..0046BF41
            bsp_cells.mesh_buckets_008194e0[index].size = 0;             // 0046BF31
            bsp_cells.fade_buckets_0080aa80[index].size = 0;             // 0046BF37
        }
    }
    bsp_cells.part_list_00827f40->size = 0;                              // 0046BF4D
    bsp_cells.static_shadow_bucket_007fbf84->size = 0;                   // 0046BF53
    bsp_cells.beam_bucket_007fbf6c->size = 0;                            // 0046BF59
    bsp_cells.mesh_shadow_bucket_007fbfb4->size = 0;                     // 0046BF5F
    bsp_cells.trigger_bucket_0080aa74->size = 0;                         // 0046BF65
    bsp_cells.static_light_bucket_007fbfe4->size = 0;                    // 0046BF6B
    bsp_cells.bin_bucket_0080aa68->size = 0;                             // 0046BF71
    bsp_cells.fading_out_bucket_007fbfa8->size = 0;                      // 0046BF77
    bsp_cells.fading_in_bucket_007fbfd8->size = 0;                       // 0046BF7D
    exo_array_list_assign_0046bc80(bsp_cells.old_active_bins_007fbffc, nullptr, bsp_cells.active_bins_007fbf9c); // 0046BF43..0046BF83
    const U models = *bsp_cells.models_007a8a68;                         // 0046BF88
    bsp_cells.active_bins_007fbf9c->size = 0;                            // 0046BF8D
    *bsp_cells.bins_collected_007fbf28 = 0;                              // 0046BF93
    *bsp_cells.bins_visited_007fbf2c = 0;                                // 0046BF99
    *bsp_cells.model_7a8a68_copy_007fbf30 = models;                      // 0046BF9F
}

// 0046BFB0 nodedata::nodedata: thirteen empty CExoArrayLists + aabb(+6C) / tile_data(+70) = 0, store order kept.
void* __fastcall nodedata_construct_0046bfb0(void* self, void*) {
    static const U kOrder[38] = {0x04, 0x08, 0x00, 0x10, 0x14, 0x0C, 0x1C, 0x20, 0x18, 0x28, 0x2C, 0x24, 0x34, 0x38,
        0x30, 0x40, 0x44, 0x3C, 0x4C, 0x50, 0x48, 0x58, 0x5C, 0x54, 0x64, 0x68, 0x60, 0x78, 0x7C, 0x74, 0x84, 0x88,
        0x80, 0x90, 0x94, 0x8C, 0x6C, 0x70};                             // 0046BFC9..0046C049
    for (U offset : kOrder) field<U>(self, offset) = 0;
    return self;                                                         // 0046BFBE mov eax,ecx
}

// 0046C420 SceneBSPAddGob(scene, gob): add the gob to the gobs list (+0C) of the node fitting its bounding sphere.
void __cdecl scene_bsp_add_gob_0046c420(void* scene, void* gob) {
    void* bsp = scene_bsp(scene);                                        // 0046C420..0046C424
    if (bsp == nullptr) return;                                          // 0046C42A..0046C42C
    const float radius = field<float>(field<void*>(gob, 0x58), 0x80);   // 0046C434..0046C437 (raw push)
    void* node = bsp_fit_sphere_004ad060(bsp, static_cast<const Vector*>(at(gob, 0x78)), radius); // 0046C43D..0046C443
    PointerList* gobs = list_at(node_data(node), 0x0C);                 // 0046C448..0046C44B
    if (count_of(gobs, gob) != 0) return;                                // 0046C44E..0046C46D
    add_with_growth(gobs, gob);                                          // 0046C46F..0046C494
}

// 0046C4A0 SceneBSPRemoveGob(scene, gob)
void __cdecl scene_bsp_remove_gob_0046c4a0(void* scene, void* gob) {
    void* bsp = scene_bsp(scene);                                        // 0046C4A0..0046C4A4
    if (bsp == nullptr) return;                                          // 0046C4AA..0046C4AE
    void* node = bsp_search_004ad1a0(bsp, vector_at(gob, 0x78), node_has_gob_0046b490, gob); // 0046C4B0..0046C4D3
    if (node == nullptr) return;                                         // 0046C4DB..0046C4DD
    PointerList* gobs = list_at(node_data(node), 0x0C);                 // 0046C4DF..0046C4E2
    if (count_of(gobs, gob) == 0) return;                                // 0046C4E5..0046C4FD
    exo_array_list_remove_00445080(gobs, nullptr, gob);                  // 0046C4FF..0046C500
}

// 0046C510 SceneBSPRemovePart(scene, part): locate the node holding the part (by world position) and unlist it.
void __cdecl scene_bsp_remove_part_0046c510(void* scene, void* part) {
    if (scene_bsp(scene) == nullptr) return;                             // 0046C518..0046C522
    Vector position = {bits_to_float(0), bits_to_float(0), bits_to_float(0)};          // 0046C53A..0046C54A
    Quaternion orientation = {bits_to_float(kOne), bits_to_float(0), bits_to_float(0), bits_to_float(0)}; // 0046C552..0046C56A
    slots<PartSlots>(part).get_world(part, nullptr, &position, &orientation);           // 0046C52C..0046C572
    if (void* light = slots<PartSlots>(part).as_light(part, nullptr)) {                 // 0046C575..0046C580
        void* node = bsp_search_004ad1a0(scene_bsp(scene), position, node_has_light_0046b340, light); // 0046C582..0046C5A8
        PointerList* lights = list_at(node_data(node), 0x00);           // 0046C5AD
        if (count_of(lights, light) != 0)                                // 0046C5B0..0046C5CD
            exo_array_list_remove_00445080(lights, nullptr, light);      // 0046C5CF..0046C5D0
        void* manager = light_manager(scene);                            // 0046C5D5
        slots<LightManagerSlots>(manager).remove_light(manager, nullptr, light, 0); // 0046C5DB..0046C5E0
        return;
    }
    if (void* emitter = slots<PartSlots>(part).as_emitter(part, nullptr)) {            // 0046C5EA..0046C5F5
        void* node = bsp_search_004ad1a0(scene_bsp(scene), position, node_has_emitter_0046b370, emitter); // 0046C5F7..0046C61D
        PointerList* emitters = list_at(node_data(node), 0x24);         // 0046C622..0046C628
        if (exo_array_list_count_004af240(emitters, nullptr, emitter) == 0) return;     // 0046C62B..0046C635
        exo_array_list_remove_00445080(emitters, nullptr, emitter);      // 0046C63B..0046C63E
        return;
    }
    if (void* projector = slots<PartSlots>(part).query_3c(part, nullptr)) {            // 0046C64A..0046C655
        void* node = bsp_search_004ad1a0(scene_bsp(scene), position, node_has_projection_0046b3a0, projector); // 0046C65B..0046C681
        void* data = node_data(node);                                    // 0046C686
        if (void* item = slots<PartSlots>(projector).query_40(projector, nullptr)) {    // 0046C689..0046C697
            PointerList* list = list_at(data, 0x30);                     // 0046C699
            if (exo_array_list_count_004af240(list, nullptr, item) == 0) return;        // 0046C69C..0046C6A6
            exo_array_list_remove_00445080(list, nullptr, item);         // 0046C6AC..0046C6AF
            return;
        }
        void* item = slots<PartSlots>(projector).query_44(projector, nullptr);          // 0046C6BB..0046C6C2
        if (item == nullptr) return;                                     // 0046C6C4..0046C6C6
        PointerList* list = list_at(data, 0x3C);                         // 0046C6CC
        if (exo_array_list_count_004af240(list, nullptr, item) == 0) return;            // 0046C6CF..0046C6D9
        exo_array_list_remove_00445080(list, nullptr, item);             // 0046C6DF..0046C6E2
        return;
    }
    if (void* walk = slots<PartSlots>(part).query_48(part, nullptr)) {                 // 0046C6EE..0046C6F9
        void* node = bsp_search_004ad1a0(scene_bsp(scene), position, node_has_aabb_0046a660, walk); // 0046C6FB..0046C721
        void* data = node_data(node);                                    // 0046C726
        if (field<void*>(data, 0x6C) != walk) return;                    // 0046C729..0046C731
        field<void*>(data, 0x6C) = nullptr;                              // 0046C737
        return;
    }
    if (void* mesh = slots<PartSlots>(part).as_trimesh(part, nullptr)) {               // 0046C745..0046C750
        void* node = bsp_search_004ad1a0(scene_bsp(scene), position, node_has_trimesh_0046b400, mesh); // 0046C752..0046C778
        PointerList* list = list_at(node_data(node), 0x18);             // 0046C77D..0046C783
        if (exo_array_list_count_004af240(list, nullptr, mesh) == 0) return;            // 0046C786..0046C790
        exo_array_list_remove_00445080(list, nullptr, mesh);             // 0046C796..0046C799
        return;
    }
    if (void* trigger = slots<PartSlots>(part).query_54(part, nullptr)) {              // 0046C7A5..0046C7B8
        void* node = bsp_search_004ad1a0(scene_bsp(scene), position, node_has_trigger_0046b430, trigger); // 0046C7BA..0046C7D8
        PointerList* list = list_at(node_data(node), 0x54);             // 0046C7DD..0046C7E3
        if (exo_array_list_count_004af240(list, nullptr, trigger) == 0) return;         // 0046C7E6..0046C7F0
        exo_array_list_remove_00445080(list, nullptr, trigger);          // 0046C7F2..0046C7F5
        return;
    }
    void* node = bsp_search_004ad1a0(scene_bsp(scene), position, node_has_other_0046b460, part); // 0046C801..0046C81F
    PointerList* list = list_at(node_data(node), 0x48);                 // 0046C824..0046C82A
    if (exo_array_list_count_004af240(list, nullptr, part) == 0) return; // 0046C82D..0046C837
    exo_array_list_remove_00445080(list, nullptr, part);                 // 0046C839..0046C83C
}

// 0046CE70 AddObjectToRooms(scene, gob): add the gob to every room whose XY box contains its centre, or to all
// rooms when none does / the model asks for it.
void __cdecl add_object_to_rooms_0046ce70(void* scene, void* gob) {
    I added = 0;                                                         // 0046CE7D xor ebx,ebx
    Vector centre;                                                       // [esp+10h]
    void* model = field<void*>(gob, 0x58);                               // 0046CE79
    if (model != nullptr && ((field<U8>(model, 0x50) >> 2) & 1) != 0) {  // 0046CE7F..0046CE8C
        centre = vector_at(gob, 0x78);                                   // 0046CE8E..0046CEA1
    } else {
        Vector sum;                                                      // [esp+20h]
        sum.x = S(W(field<float>(gob, 0x120)) + W(field<float>(gob, 0x12C)));   // 0046CEAA..0046CEC4
        sum.y = S(W(field<float>(gob, 0x124)) + W(field<float>(gob, 0x130)));   // 0046CEC9..0046CED5
        sum.z = S(W(field<float>(gob, 0x128)) + W(field<float>(gob, 0x134)));   // 0046CED9..0046CEE5
        const Vector* half = vector_divide_00437a90(&centre, &sum, bits_to_float(kHalfDivisor)); // 0046CEB0..0046CEE9
        Vector rotated;                                                  // [esp+2Ch]
        const Vector* turned = quaternion_rotate_004a9bb0(&rotated, static_cast<const Quaternion*>(at(gob, 0x84)), half); // 0046CEEE..0046CEFB
        Vector world;
        world.x = S(W(field<float>(gob, 0x78)) + W(turned->x));          // 0046CF00..0046CF08
        world.y = S(W(field<float>(gob, 0x7C)) + W(turned->y));          // 0046CF0C..0046CF12
        world.z = S(W(field<float>(gob, 0x80)) + W(turned->z));          // 0046CF16..0046CF2F
        centre = world;                                                  // 0046CF1C..0046CF37
    }
    model = field<void*>(gob, 0x58);                                     // 0046CF3B
    if (model == nullptr || (field<U8>(model, 0x51) & 1) == 0) {         // 0046CF3E..0046CF49
        for (I index = 0; index < scene_rooms(scene)->size; ++index) {   // 0046CF4F..0046CFBA
            void* room = elements(scene_rooms(scene))[index];            // 0046CF60..0046CF6A
            if (!(W(centre.x) >= W(field<float>(room, 0x1C)))) continue; // 0046CF66..0046CF75 test ah,1; jne
            if (!(W(centre.y) >= W(field<float>(room, 0x20)))) continue; // 0046CF77..0046CF83
            if (!(W(centre.x) <= W(field<float>(room, 0x28)))) continue; // 0046CF85..0046CF91 test ah,41h; jp
            if (!(W(centre.y) <= W(field<float>(room, 0x2C)))) continue; // 0046CF93..0046CF9F
            nodedata_add_object_0046bd70(node_data(room), nullptr, gob); // 0046CFA1..0046CFA5
            ++added;                                                     // 0046CFAA
            bsp_room_index_007fbf68 = index;                             // 0046CFAB
        }
        if (added != 0) return;                                          // 0046CFBC..0046CFBE
    }
    for (I index = 0; index < scene_rooms(scene)->size; ++index)         // 0046CFCA..0046D057
        add_inline_growth(list_at(node_data(elements(scene_rooms(scene))[index]), 0x74), gob); // 0046CFE0..0046D04B
    bsp_room_index_007fbf68 = 999;                                       // 0046D059 (3E7h)
}

// 0046D070 CollectActiveRooms(scene, rooms): current room + its visible rooms, or every room.
void __cdecl collect_active_rooms_0046d070(void* scene, PointerList* rooms) {
    if (field<void*>(scene, 0xD4) == nullptr) {                          // 0046D075..0046D080
        exo_array_list_assign_0046bc80(rooms, nullptr, scene_rooms(scene)); // 0046D082..0046D08D
        return;
    }
    rooms->size = 0;                                                     // 0046D09A
    if (field<U16>(scene, 0xD8) == 0 && bsp_use_current_room_0078e5c8 != 0) { // 0046D09D..0046D0AC
        int_list_add_00444fe0(rooms, nullptr, field<U>(scene, 0xD4));    // 0046D0AE..0046D0B7
        for (I index = 0; index < list_at(field<void*>(scene, 0xD4), 0x5C)->size; ++index) // 0046D0BC..0046D10A
            add_with_growth(rooms, elements(list_at(field<void*>(scene, 0xD4), 0x5C))[index]); // 0046D0D0..0046D0FB
        return;
    }
    exo_array_list_assign_0046bc80(rooms, nullptr, scene_rooms(scene)); // 0046D111..0046D11A
}

// 0046D2F0 SceneBSPRemoveTiledata(scene, point): unlist and delete the tile's static parts, then the tile.
void __cdecl scene_bsp_remove_tiledata_0046d2f0(void* scene, const Vector* point) {
    void* bsp = scene_bsp(scene);                                        // 0046D2F5
    if (bsp == nullptr) return;                                          // 0046D2FB..0046D2FD
    void* node = bsp_fit_sphere_004ad060(bsp, point, bits_to_float(0));  // 0046D303..0046D30C
    void* data = node_data(node);                                        // 0046D311
    void* tile = field<void*>(data, 0x70);                               // 0046D314
    if (tile == nullptr) return;                                         // 0046D31A..0046D31C
    I remaining = field<I>(tile, 0x1C);                                  // 0046D324
    while (remaining != 0) {                                             // 0046D327..0046D329 / 0046D36B..0046D370
        void* part = elements(list_at(field<void*>(data, 0x70), 0x18))[remaining - 1]; // 0046D330..0046D336
        --remaining;                                                     // 0046D33A
        scene_bsp_remove_part_0046c510(scene, part);                     // 0046D33B..0046D33D
        aurora_model::MaxTree* model = aurora_tree_as_model_0043e1c0(
            field<aurora_model::MaxTree*>(field<void*>(part, 4), 8), nullptr);            // 0046D342..0046D34B
        if (model != nullptr) aurora_model_del_ref_0043e210(model, nullptr, model);      // 0046D350..0046D357
        delete_object(part);                                             // 0046D35C..0046D362
        void* current = field<void*>(data, 0x70);                        // 0046D364
        field<I>(current, 0x1C) = field<I>(current, 0x1C) - 1;           // 0046D367..0046D36D
    }
    void* object = field<void*>(field<void*>(data, 0x70), 0x24);         // 0046D372..0046D375
    if (object != nullptr) {                                             // 0046D378..0046D37A
        delete_object(object);                                           // 0046D37C..0046D380
        field<void*>(field<void*>(data, 0x70), 0x24) = nullptr;          // 0046D382..0046D385
    }
    void* tile_data = field<void*>(data, 0x70);                          // 0046D38C
    if (tile_data != nullptr) {                                          // 0046D38F..0046D391
        tile_data_destruct_00444e20(tile_data, nullptr);                 // 0046D393..0046D395
        live_free(tile_data);                                            // 0046D39A..0046D39B
    }
    field<void*>(data, 0x70) = nullptr;                                  // 0046D3A4
}

// 0046E290 SceneBSPAddPart(scene, part, model): file the part in the node fitting it, by part kind; EAX 0/1.
I __cdecl scene_bsp_add_part_0046e290(void* scene, void* part, aurora_model::MaxTree* model) {
    if (scene_bsp(scene) == nullptr) return 0;                           // 0046E298..0046E2A8
    Vector position = {bits_to_float(0), bits_to_float(0), bits_to_float(0)};          // 0046E2BD..0046E2CD
    Quaternion orientation = {bits_to_float(kOne), bits_to_float(0), bits_to_float(0), bits_to_float(0)}; // 0046E2D5..0046E2ED
    slots<PartSlots>(part).get_world(part, nullptr, &position, &orientation);           // 0046E2AE..0046E2F5
    if (void* light = slots<PartSlots>(part).as_light(part, nullptr)) {                 // 0046E2F8..0046E303
        void* node = bsp_fit_sphere_004ad060(scene_bsp(scene), &position, field<float>(light, 0x58)); // 0046E305..0046E315
        PointerList* lights = list_at(node_data(node), 0x00);           // 0046E31A
        if (count_of(lights, light) == 0)                                // 0046E31D..0046E33D
            int_list_add_00444fe0(lights, nullptr, static_cast<U>(reinterpret_cast<std::uintptr_t>(light))); // 0046E33F..0046E340
        void* manager = light_manager(scene);                            // 0046E345
        slots<LightManagerSlots>(manager).add_light(manager, nullptr, light);          // 0046E34B..0046E34E
    } else if (void* walk = slots<PartSlots>(part).query_48(part, nullptr)) {          // 0046E356..0046E361
        void* node = bsp_fit_sphere_004ad060(scene_bsp(scene), &position, bits_to_float(0)); // 0046E363..0046E371
        field<void*>(node_data(node), 0x6C) = walk;                      // 0046E376..0046E37C
    } else if (void* mesh = slots<PartSlots>(part).as_trimesh(part, nullptr)) {        // 0046E384..0046E38F
        const Vector minimum = vector_at(mesh, 0x4C);                    // 0046E391..0046E3A9 [esp+24h]
        const Vector maximum = vector_at(mesh, 0x58);                    // 0046E3A4..0046E3C4 [esp+18h]
        void* node = bsp_fit_box_004ad0d0(scene_bsp(scene), &minimum, &maximum);       // 0046E3B7..0046E3D0
        int_list_add_00444fe0(list_at(node_data(node), 0x18), nullptr,
                              static_cast<U>(reinterpret_cast<std::uintptr_t>(mesh)));  // 0046E3D5..0046E3DF
    } else if (void* projector = slots<PartSlots>(part).query_3c(part, nullptr)) {     // 0046E3E9..0046E3F4
        const Vector minimum = vector_at(projector, 0x54);               // 0046E3F6..0046E40E [esp+18h]
        const Vector maximum = vector_at(projector, 0x60);               // 0046E409..0046E429 [esp+24h]
        void* node = bsp_fit_box_004ad0d0(scene_bsp(scene), &minimum, &maximum);       // 0046E41C..0046E435
        void* data = node_data(node);                                    // 0046E43C
        if (slots<PartSlots>(projector).query_40(projector, nullptr) != nullptr) {     // 0046E43A..0046E44B
            void* item = slots<PartSlots>(projector).query_40(projector, nullptr);     // 0046E44D..0046E44F (called again)
            int_list_add_00444fe0(list_at(data, 0x30), nullptr, static_cast<U>(reinterpret_cast<std::uintptr_t>(item))); // 0046E452..0046E456
        } else {
            void* item = slots<PartSlots>(projector).query_44(projector, nullptr);     // 0046E460..0046E462
            int_list_add_00444fe0(list_at(data, 0x3C), nullptr, static_cast<U>(reinterpret_cast<std::uintptr_t>(item))); // 0046E465..0046E469
        }
    } else if (void* emitter = slots<PartSlots>(part).as_emitter(part, nullptr)) {     // 0046E473..0046E47E
        void* node = bsp_fit_sphere_004ad060(scene_bsp(scene), &position, field<float>(emitter, 0xD0)); // 0046E480..0046E493
        PointerList* emitters = list_at(node_data(node), 0x24);         // 0046E498..0046E49E
        if (exo_array_list_count_004af240(emitters, nullptr, emitter) == 0)              // 0046E4A1..0046E4AB
            int_list_add_00444fe0(emitters, nullptr, static_cast<U>(reinterpret_cast<std::uintptr_t>(emitter))); // 0046E4AD..0046E4B0
        aurora_model::MdlNode* emitter_node = aurora_node_as_emitter_0043e3c0(
            field<aurora_model::MdlNode*>(emitter, 4), nullptr);         // 0046E4B5..0046E4B8
        if (emitter_node_is_affected_by_wind_0048d780(emitter_node, nullptr)) {        // 0046E4BD..0046E4C6
            if (field<void*>(scene, 0x84) != nullptr) {                  // 0046E4CC..0046E4D4
                wind_safe_pointer_assign_0046e240(at(emitter, 0x1C4), nullptr, field<void*>(scene, 0x84)); // 0046E4DA..0046E4E1
                wind_manager_register_windy_emitter_004a6470(field<void*>(scene, 0x84), nullptr, emitter); // 0046E4E6..0046E4ED
            }
        }
    } else if (void* trigger = slots<PartSlots>(part).query_54(part, nullptr)) {       // 0046E4F7..0046E502
        const Vector minimum = vector_at(trigger, 0x5C);                 // 0046E504..0046E51C [esp+18h]
        const Vector maximum = vector_at(trigger, 0x68);                 // 0046E517..0046E537 [esp+24h]
        void* node = bsp_fit_box_004ad0d0(scene_bsp(scene), &minimum, &maximum);       // 0046E52A..0046E543
        PointerList* triggers = list_at(node_data(node), 0x54);         // 0046E548..0046E54E
        if (exo_array_list_count_004af240(triggers, nullptr, trigger) == 0)              // 0046E551..0046E55B
            int_list_add_00444fe0(triggers, nullptr, static_cast<U>(reinterpret_cast<std::uintptr_t>(trigger))); // 0046E55D..0046E560
    } else {
        void* node = bsp_fit_sphere_004ad060(scene_bsp(scene), &position, bits_to_float(0)); // 0046E567..0046E575
        PointerList* others = list_at(node_data(node), 0x48);           // 0046E57A..0046E580
        if (exo_array_list_count_004af240(others, nullptr, part) == 0)   // 0046E583..0046E58D
            int_list_add_00444fe0(others, nullptr, static_cast<U>(reinterpret_cast<std::uintptr_t>(part))); // 0046E58F..0046E592
    }
    if (model != nullptr) model_add_ref_0043e240(model, nullptr, model); // 0046E597..0046E5A2 (push ecx; this = arg = model)
    return 1;                                                            // 0046E5A7
}

// 0046E5C0 ManageSceneBSP(scene): per-frame room / light / emitter / gob bookkeeping and visibility walk.
void __cdecl manage_scene_bsp_0046e5c0(void* scene) {
    const U enabled = bsp_manage_enabled_0078e5cc;                       // 0046E5CE
    *bsp_cells.vertex_programs_reset_007a193c = 1;                       // 0046E5E8
    if (enabled == 0) return;                                            // 0046E5EE -> 0046ECEE
    const I forced = bsp_forced_room_0078e5d0;                           // 0046E5F4
    if (forced >= 0) {                                                   // 0046E5F9..0046E600 jl
        field<void*>(scene, 0xD4) = elements(scene_rooms(scene))[forced]; // 0046E602..0046E60B
        field<U16>(scene, 0xD8) = 0;                                     // 0046E611
        bsp_use_current_room_0078e5c8 = 1;                               // 0046E618
    }
    if (scene_bsp(scene) == nullptr && scene_rooms(scene)->size == 0) return; // 0046E61E..0046E62C
    for (I index = 0; index < scene_objects(scene)->size; ++index) {     // 0046E632..0046E64B
        aurora_model::Gob* gob = as_gob(elements(scene_objects(scene))[index]);        // 0046E637..0046E63A
        gob_slots(gob).s17c_gob_disable_in_view_volume(gob, nullptr);    // 0046E63D..0046E63F
    }
    PointerList* scene_lights = list_at(light_manager(scene), 0x14);    // 0046E653..0046E659 (kept in edi)
    U guard = bsp_camera_guard_00827f9c;                                 // 0046E64D
    if ((guard & 1) == 0) {                                              // 0046E65C..0046E65E
        guard |= 1; bsp_camera_guard_00827f9c = guard;                   // 0046E660..0046E662
        bsp_camera_position_00827f90[0] = 0; bsp_camera_position_00827f90[1] = 0; bsp_camera_position_00827f90[2] = 0; // 0046E667..0046E67B
    }
    if ((guard & 2) == 0) {                                              // 0046E685..0046E687
        guard |= 2; bsp_camera_guard_00827f9c = guard;                   // 0046E689..0046E68C
        bsp_camera_orientation_00827f80[3] = 0; bsp_camera_orientation_00827f80[2] = 0;  // 0046E691..0046E69B
        bsp_camera_orientation_00827f80[1] = 0; bsp_camera_orientation_00827f80[0] = kOne; // 0046E6A5..0046E6AF
    }
    construct_camera_fade_plane_0046b7c0(scene);                         // 0046E6B9..0046E6BB (EAX = scene in the original)
    do_static_light_buckets_0046dda0(scene);                             // 0046E6C0..0046E6C1
    clear_buckets_0046bef0(scene);                                       // 0046E6C6..0046E6C7
    clear_scene_rooms_0046bc40(scene);                                   // 0046E6CC..0046E6CD
    { void* manager = light_manager(scene); slots<LightManagerSlots>(manager).reset(manager, nullptr); }          // 0046E6D2..0046E6DD
    { void* manager = light_manager(scene); slots<LightManagerSlots>(manager).update_lights(manager, nullptr); }  // 0046E6E0..0046E6E8
    for (I index = 0; index < scene_lights->size; ++index) {             // 0046E6EB..0046E726
        void* light = elements(scene_lights)[index];                     // 0046E6F4..0046E6F6
        add_light_to_rooms_0046cc00(scene, light);                       // 0046E6F9..0046E6FB
        if (scene_rooms(scene)->size == 0) {                             // 0046E700..0046E70B
            scene_bsp_remove_part_0046c510(scene, light);                // 0046E70D..0046E70F
            scene_bsp_add_part_0046e290(scene, light, nullptr);          // 0046E714..0046E718
        }
    }
    bool collect = false;
    void* collect_room = nullptr;
    if (field<U16>(scene, 0xD8) == 0 && bsp_use_current_room_0078e5c8 != 0) {         // 0046E728..0046E739
        collect = true; collect_room = field<void*>(scene, 0xD4);        // 0046E73B..0046E741
    } else if (scene_rooms(scene)->size != 0) {                          // 0046E744..0046E74C
        collect = true; collect_room = nullptr;                          // 0046E74E push 0
    }
    bool room_lights = false;
    if (collect) {
        void* manager = light_manager(scene);                            // 0046E750
        slots<LightManagerSlots>(manager).collect_lights_for_given_room(manager, nullptr, collect_room); // 0046E756..0046E758
        room_lights = scene_rooms(scene)->size != 0;                     // 0046E75B..0046E763
    }
    {
        void* view = camera();                                           // 0046E765 / 0046E786
        const U far_bits = field<U>(view, 0x1E0);                        // 0046E76B / 0046E78C
        void* manager = light_manager(scene);                            // 0046E771 / 0046E792
        const Vector* focus = static_cast<const Vector*>(at(scene, 0xA8)); // 0046E779 / 0046E79A
        if (room_lights) slots<LightManagerSlots>(manager).setup_lights_for_this_room(manager, nullptr, focus, far_bits); // 0046E77F..0046E781
        else slots<LightManagerSlots>(manager).setup_lights_for_position(manager, nullptr, focus, far_bits);            // 0046E7A0..0046E7A2
    }
    for (I index = 0; index < list_at(scene, 0x94)->size; ++index) {    // 0046E7A5..0046E7F1 (emitters, Scene+94)
        void* emitter = elements(list_at(scene, 0x94))[index];           // 0046E7B1..0046E7B7
        add_emitter_to_rooms_0046cd70(scene, emitter);                   // 0046E7BA..0046E7BC
        if (scene_rooms(scene)->size == 0 && field<U>(emitter, 0x40) != 0) {           // 0046E7C1..0046E7D3
            scene_bsp_remove_part_0046c510(scene, emitter);              // 0046E7D5..0046E7D7
            scene_bsp_add_part_0046e290(scene, emitter, nullptr);        // 0046E7DC..0046E7E0
        }
    }
    for (I index = 0; index < scene_objects(scene)->size; ++index) {     // 0046E7F3..0046E857
        void* gob = elements(scene_objects(scene))[index];               // 0046E800..0046E803
        if (field<void*>(gob, 0x58) == nullptr) continue;                // 0046E806..0046E809
        add_object_to_rooms_0046ce70(scene, gob);                        // 0046E80B..0046E80D
        if (scene_rooms(scene)->size != 0) continue;                     // 0046E812..0046E81D
        gob = elements(scene_objects(scene))[index];                     // 0046E81F..0046E822
        const unsigned char* name = static_cast<const unsigned char*>(at(gob, 0x38)); // 0046E82A lea esi,[eax+38h]
        bool equal = true;
        for (U byte = 0; byte < 11; ++byte) {                            // 0046E825..0046E834 repe cmpsb (ecx = 0Bh)
            if (name[byte] != static_cast<unsigned char>(kFocusGob_007416fc[byte])) { equal = false; break; }
        }
        if (equal) continue;                                             // 0046E836
        scene_bsp_remove_gob_0046c4a0(scene, gob);                       // 0046E838..0046E83A
        scene_bsp_add_gob_0046c420(scene, elements(scene_objects(scene))[index]);      // 0046E83F..0046E847
    }
    bsp_stat_007fbf34 = 0;                                               // 0046E859
    *bsp_cells.stat_007fbf38 = 0;                                        // 0046E85F
    *bsp_cells.stat_007fbf3c = 0;                                        // 0046E865
    bsp_stat_007fbf40 = 0;                                               // 0046E86B
    *bsp_cells.stat_007fbf44 = 0;                                        // 0046E871
    PointerList rooms;                                                   // [esp+10h] (EH state 0 from 0046E889)
    rooms.size = 0; rooms.capacity = 0; rooms.data = 0;                  // 0046E877..0046E87F
    collect_active_rooms_0046d070(scene, &rooms);                        // 0046E883..0046E88D
    if (scene_rooms(scene)->size != 0) {                                 // 0046E892..0046E89D
        const I count = rooms.size;                                      // 0046E89F
        for (I index = 0; index < count; ++index) {                      // 0046E8A3..0046E8D8
            const float radius_sq = bsp_bin_radius_sq_007a1968;          // 0046E8B0 (raw DWORD)
            void* view = camera();                                       // 0046E8B5
            bsp_traverse_sphere_004acf60(elements(&rooms)[index], static_cast<const Vector*>(at(view, 0x7C)),
                                         radius_sq, collect_bins_0046cae0);              // 0046E8BB..0046E8CD
        }
        aur_process_active_bins_0046bbc0();                              // 0046E8DA
        for (I index = 0; index < count; ++index) {                      // 0046E8DF..0046E90E
            void* view = camera();                                       // 0046E8E5
            bsp_traverse_volume_approx_004ad7e0(elements(&rooms)[index], static_cast<const Vector*>(at(view, 0x7C)),
                                                static_cast<PlaneList*>(at(view, 0x1F8)), process_parts_0046d3b0, scene); // 0046E8EA..0046E903
        }
    } else {
        void* view = camera();                                           // 0046E912
        bsp_traverse_volume_approx_004ad7e0(scene_bsp(scene), static_cast<const Vector*>(at(view, 0x7C)),
                                            static_cast<PlaneList*>(at(view, 0x1F8)), process_parts_0046d3b0, scene); // 0046E917..0046E92F
    }
    {
        void* manager = light_manager(scene);                            // 0046E937
        slots<LightManagerSlots>(manager).prioritize_lights(manager, nullptr, static_cast<const Vector*>(at(scene, 0xA8)), 1); // 0046E93D..0046E948
    }
    bsp_unlit_lights_007fbf18 = 0;                                       // 0046E94C
    for (I index = 0; index < list_at(light_manager(scene), 0x2C)->size; ++index) {    // 0046E952..0046E9CA (LightManager+2C/+30)
        void* light = elements(list_at(light_manager(scene), 0x2C))[index];            // 0046E961..0046E96A
        const bool black = field<float>(light, 0x4C) == bits_to_float(kZero_0073d700) && // 0046E96D..0046E97E test ah,44h; jp
                           field<float>(light, 0x50) == bits_to_float(kZero_0073d700) && // 0046E980..0046E98E
                           field<float>(light, 0x54) == bits_to_float(kZero_0073d700);   // 0046E990..0046E99E
        bsp_unlit_lights_007fbf18 = bsp_unlit_lights_007fbf18 + (black ? 0 : 1);       // 0046E9A0..0046E9B8 sete cl
    }
    PointerList* distortion = bsp_cells.distortion_bucket_007fbfc0;
    PointerList* saber = bsp_cells.light_saber_bucket_007fbff0;
    PointerList* gobs = bsp_cells.gob_bucket_007fbfcc;
    for (I index = 0; index < scene_objects(scene)->size; ++index) {     // 0046E9CC..0046EC4E
        void* object = elements(scene_objects(scene))[index];            // 0046E9D5..0046E9D8
        void* model = field<void*>(object, 0x58);                        // 0046E9DB
        if (model == nullptr) continue;                                  // 0046E9DE..0046E9E0
        if (scene_rooms(scene)->size == 0) { add_inline_growth(gobs, object); continue; } // 0046E9E6..0046E9EE -> 0046EBC9..0046EC42
        aurora_model::Gob* gob = as_gob(object);
        if (gob_slots(gob).s180_gob_is_in_view_volume(gob, nullptr) == 0 &&             // 0046E9F4..0046EA00
            ((field<U8>(field<void*>(object, 0x58), 0x50) >> 6) & 1) == 0 &&            // 0046EA02..0046EA0E
            gob_slots(gob).s190_gob_is_always_rendering(gob, nullptr) == 0) {           // 0046EA10..0046EA1C
            field<U8>(object, 0x18) = 1;                                 // 0046EA1E
            continue;
        }
        model = field<void*>(object, 0x58);                              // 0046EA27 (non-null in practice: tested at
                                                                         // 0046E9DE, only gob virtual queries ran since)
        if (model != nullptr) {                                          // 0046EA2A..0046EA2C (null -> 0046EACD)
            if (field<U8>(object, 0x17C) != 0 || ((field<U8>(model, 0x50) >> 1) & 1) != 0) { // 0046EA32..0046EA44
                add_inline_growth(distortion, object);                   // 0046EA4A..0046EAC2
                continue;
            }
        }
        if (((field<U8>(model, 0x50) >> 4) & 1) != 0) {                  // 0046EACD..0046EAD6
            add_inline_growth(saber, object);                            // 0046EADC..0046EB52
            continue;
        }
        add_inline_growth(gobs, object);                                 // 0046EB5D..0046EC42
    }
    {
        void* view = camera();                                           // 0046EC54
        aurora_model::Gob* camera_gob = as_gob(at(view, 4));             // 0046EC5A..0046EC5D (Gob subobject at +4)
        gob_slots(camera_gob).s178_gob_enable_in_view_volume(camera_gob, nullptr);    // 0046EC60
    }
    void* view = camera();                                               // 0046EC66
    for (U word = 0; word < 3; ++word) bsp_camera_position_00827f90[word] = field<U>(view, 0x7C + 4 * word);    // 0046EC6B..0046EC82
    for (U word = 0; word < 4; ++word) bsp_camera_orientation_00827f80[word] = field<U>(view, 0x88 + 4 * word); // 0046EC88..0046ECAB
    const U near_bits = field<U>(view, 0x1DC);                           // 0046ECB1
    const U far_bits = field<U>(view, 0x1E0);                            // 0046ECB7
    bsp_camera_near_00827f7c = near_bits;                                // 0046ECC3
    bsp_camera_far_00827f78 = far_bits;                                  // 0046ECC9
    bsp_first_frame_0078e5d4 = 0;                                        // 0046ECCF
    if (rooms.data != 0) live_free(elements(&rooms));                    // 0046ECBD..0046ECE5 (EH state -1)
}

// 0046ED00 SceneBSPAddTileData(scene, tile, point): attach the tile to the leaf at point and file its static parts.
void __cdecl scene_bsp_add_tile_data_0046ed00(void* scene, void* tile, Vector point) {
    void* bsp = scene_bsp(scene);                                        // 0046ED00..0046ED04
    if (bsp == nullptr) return;                                          // 0046ED0A..0046ED0C
    void* node = bsp_fit_sphere_004ad060(bsp, &point, bits_to_float(0)); // 0046ED11..0046ED19
    void* data = node_data(node);                                        // 0046ED1E
    field<void*>(data, 0x70) = tile;                                     // 0046ED25
    for (I index = 0; index < field<I>(tile, 0x1C); ++index) {           // 0046ED28..0046ED5E (bound re-read from tile)
        void* part = elements(list_at(field<void*>(data, 0x70), 0x18))[index];        // 0046ED35..0046ED3B
        aurora_model::MaxTree* model = aurora_tree_as_model_0043e1c0(
            field<aurora_model::MaxTree*>(field<void*>(part, 4), 8), nullptr);        // 0046ED3E..0046ED44
        scene_bsp_add_part_0046e290(scene, part, model);                 // 0046ED49..0046ED50
    }
}

} // extern "C"
