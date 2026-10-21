// model_skin.cpp - lane-3d-model: skin-seam normal welding at instance creation.
// MdlNode::ProcessSkinSeams 00438FA0, MdlNode::CreateSkinNodeList 00438C40, CExoArrayList<MdlNodeTriMesh*>::Add
// 00438B40, Vector::operator+= 004A9740, Vector::Normalize 004AB130. x87 intermediates as double with every
// original fstp dword kept as a float rounding (D30). Unwind states cited, not emitted.
// NOTE (path): ProcessSkinSeams returns at 00438FC3 when the owner tree's type byte has bit 7 set, which
// InputBinary::Reset 004A104F sets for every binary MDL; the welding body runs only for ASCII-loaded models.
#include "1321619b61-model_instance.hpp"
#include <cmath>

using namespace aurora_model;
using kotor_scene3d::Vector;
using kotor_scene3d::Quaternion;

extern "C" {
Quaternion* __cdecl quaternion_multiply_004a9ac0(Quaternion* out, const Quaternion* a, const Quaternion* b); // core sub-lane (not authored here)
void __fastcall quaternion_list_allocate_00437cc0(List* self, void* unused_edx, I capacity);                 // core sub-lane (CExoArrayList<Quaternion>::Allocate)
void __fastcall vector_list_allocate_00421310(List* self, void* unused_edx, I capacity);                     // lane-font (CExoArrayList<Vector>::allocate)
void __fastcall node_list_add_00438b40(List* self, void*, U value);
I __fastcall aurora_node_create_skin_node_list_00438c40(MdlNode* self, void*, List* list);
Vector* __fastcall vector_add_assign_004a9740(Vector* self, void*, const Vector* v);
void __fastcall vector_normalize_004ab130(Vector* self, void*);
}

// 004A9740 Vector::operator+=(v), RET4: each component this = v + this, rounded to float; EAX = this (004A9740).
Vector* __fastcall vector_add_assign_004a9740(Vector* self, void*, const Vector* v) {
    // Unlike the out-of-place helpers, the original stores each component
    // before loading the next. Keep that order even when the ranges overlap.
    __asm {
        mov eax, self
        mov ecx, v
        fld dword ptr [ecx]
        fadd dword ptr [eax]
        fstp dword ptr [eax]
        fld dword ptr [ecx+4]
        fadd dword ptr [eax+4]
        fstp dword ptr [eax+4]
        fld dword ptr [ecx+8]
        fadd dword ptr [eax+8]
        fstp dword ptr [eax+8]
    }
    return self;
}
// 004AB130 Vector::Normalize: m = |v| (004AB100, ST0); m < 1e-9 (qword 007440F0, x87 compare) -> (1,0,0);
// else inv = 1.0f / m; each component *= inv, rounded to float.
void __fastcall vector_normalize_004ab130(Vector* self, void*) {
    const double m = vector_magnitude_004ab100(self);                               // 004AB130..004AB131
    if (!(m >= 1e-9)) {                                                             // 004AB136..004AB144 (jp taken iff m >= eps)
        self->x = 1.0f; self->y = 0.0f; self->z = 0.0f;                             // 004AB146..004AB153
        return;
    }
    const double inv = 1.0 / m;                                                     // 004AB157 (0073D764)
    self->x = static_cast<float>(inv * self->x);                                    // 004AB15D..004AB161
    self->y = static_cast<float>(inv * self->y);                                    // 004AB163..004AB168
    self->z = static_cast<float>(inv * self->z);                                    // 004AB16B..004AB16E
}
// 00438B40 CExoArrayList<MdlNodeTriMesh*>::Add(value), RET4: growth cap ? 2*cap : 16 via 00437D60.
void __fastcall node_list_add_00438b40(List* self, void*, U value) {
    if (self->size == self->capacity)                                               // 00438B43..00438B49
        kotor_renderer_gl::allocate_00437d60(self, nullptr, static_cast<U>(self->capacity ? self->capacity * 2 : 0x10)); // 00438B4B..00438B5B
    ptr<U>(self->data)[self->size] = value;                                         // 00438B60..00438B69
    self->size = self->size + 1;                                                    // 00438B6C
}
// 00438C40 MdlNode::CreateSkinNodeList(list), RET4 -> number of skin nodes added below this node: for each child
// (count +30 re-read): lightsaber subtrees are skipped whole; a trimesh child with byte [+189] != 0 is appended
// (and counted when it is a skin); then the child's own subtree is added (the original unrolls four levels; the
// first level inlines the Add).
I __fastcall aurora_node_create_skin_node_list_00438c40(MdlNode* self, void*, List* list) {
    I count = 0;                                                                    // 00438C54 ([esp+20h])
    for (I i = 0; i < get<I>(self, 0x30); ++i) {                                    // 00438C46..00438F83
        MdlNode* const child = ptr<MdlNode>(ptr<U>(get<U>(self, 0x2C))[i]);         // 00438C64..00438C67
        if (aurora_node_as_saber_0043e3a0(child, nullptr) != nullptr) continue;     // 00438C6A..00438C71
        MdlNode* const mesh = aurora_node_as_trimesh_0043e400(child, nullptr);      // 00438C7D
        if (mesh != nullptr && get<U8>(aurora_node_as_trimesh_0043e400(child, nullptr), 0x189) != 0) { // 00438C82..00438C99
            node_list_add_00438b40(list, nullptr, addr(aurora_node_as_trimesh_0043e400(child, nullptr))); // 00438CA1..00438CD3
            if (aurora_node_as_skin_0043e3f0(child, nullptr) != nullptr) ++count;   // 00438CDC..00438CE5
        }
        count += aurora_node_create_skin_node_list_00438c40(child, nullptr, list); // inlined levels / 00438EE8
    }
    return count;                                                                   // 00438F89
}

namespace {
// Locally owned DWORD lists of 00438FA0 (growth cap ? 2*cap : 16; new via 006FC0D2, free via 006FC0CD).
void grow_and_append(List& l, U value) {
    if (l.size == l.capacity) {                                                     // 004396B7 / 0043971C
        const I cap = l.capacity ? l.capacity * 2 : 0x10;                           // 004396BB..004396C3
        l.capacity = cap;                                                           // 004396CC / 0043973B
        const U old = l.data;
        U* const fresh = static_cast<U*>(live_operator_new(static_cast<U>(cap) << 2)); // 004396D0..004396D4
        for (I k = 0; k < l.size; ++k) fresh[k] = ptr<U>(old)[k];                   // 004396DC..004396F9
        l.data = addr(fresh);
        if (old != 0) live_free(ptr<void>(old));                                    // 004396FF..00439704
    }
    ptr<U>(l.data)[l.size] = value;                                                 // 0043970C..00439714
    l.size = l.size + 1;                                                            // 0043971B..0043971E
}
// 004393F0..004394D9: both per-vertex lists are released first, then reallocated (capacity 16) with one entry each.
void reset_pair(List& normals, U first_normal, List& owners, U first_owner) {
    if (normals.data != 0) { live_free(ptr<void>(normals.data)); normals.data = 0; } // 004393F0..00439401
    normals.size = 0;                                                               // 0043940B
    if (owners.data != 0) { live_free(ptr<void>(owners.data)); owners.data = 0; }   // 00439405..0043941A
    owners.capacity = 0; owners.size = 0; normals.capacity = 0x10;                  // 0043944C..00439454
    U* const a = static_cast<U*>(live_operator_new(0x40));                          // 0043944A..0043945C
    normals.data = addr(a);                                                         // 00439466
    a[0] = first_normal; normals.size = 1;                                          // 00439484..00439486
    owners.capacity = 0x10;                                                         // 0043948A
    U* const b = static_cast<U*>(live_operator_new(0x40));                          // 00439482..00439492
    owners.data = addr(b);                                                          // 0043949E
    b[0] = first_owner; owners.size = 1;                                            // 004394B1 / 004394D9
}
Vector vertex_at(unsigned char* pool, MdlNode* mesh, I index, U field) {            // stride +14C, base +194, field +154/+158
    const U off = get<U>(mesh, 0x14C) * static_cast<U>(index) + get<U>(mesh, 0x194) + get<U>(mesh, field);
    Vector v;
    std::memcpy(&v, pool + off, sizeof v);
    return v;
}
unsigned char* vertex_ptr(unsigned char* pool, MdlNode* mesh, I index, U field) {
    return pool + get<U>(mesh, 0x14C) * static_cast<U>(index) + get<U>(mesh, 0x194) + get<U>(mesh, field);
}
Vector world_point(const Quaternion* q, const Vector& local, const Vector& origin) {
    Vector rotated;
    const Vector* r = quaternion_rotate_004a9bb0(&rotated, q, &local);
    Vector out;
    out.x = static_cast<float>(static_cast<double>(r->x) + origin.x);
    out.y = static_cast<float>(static_cast<double>(r->y) + origin.y);
    out.z = static_cast<float>(static_cast<double>(r->z) + origin.z);
    return out;
}
// x87 "fcom 0; jp; fchs; fcomp 0.001f; test ah,5; jp skip": |d| >= 0.001f skips (ordered compares).
bool within(double d) { return std::fabs(d) < static_cast<double>(0.001f); }
}

// 00438FA0 MdlNode::ProcessSkinSeams (thiscall, RET0).
extern "C" void __fastcall aurora_node_process_skin_seams_00438fa0(MdlNode* self, void*) {
    if (static_cast<signed char>(get<U8>(get<MaxTree*>(self, 8), 0x4C)) < 0) return; // 00438FB5..00438FC3
    List nodes = {};                                                                // 00438FCC..00438FD4
    if (aurora_node_create_skin_node_list_00438c40(self, nullptr, &nodes) == 0) {  // 00438FD8..00438FEB
        if (nodes.data != 0) live_free(ptr<void>(nodes.data));                      // 00438FED..004399EF
        return;
    }
    unsigned char* pool = nullptr;                                                  // S54
    List normals = {}, owners = {}, positions = {}, orientations = {};              // S20.., S3C.., SB4.., S88..
    Vector sum = {0.0f, 0.0f, 0.0f};                                                // S48..S50
    for (I i = 0; i < nodes.size; ++i) {                                            // 0043908D..004392A2
        MdlNode* n = ptr<MdlNode>(ptr<U>(nodes.data)[i]);                           // 004390A0..004390A8
        Vector p = {0.0f, 0.0f, 0.0f};                                              // S14..S1C
        Quaternion r = {1.0f, 0.0f, 0.0f, 0.0f};                                    // S98..SA4 (w,x,y,z)
        for (; n != nullptr; n = get<MdlNode*>(n, 0x0C)) {                          // 004390AB / 0043919D..004391A5
            Vector rotated;
            const Quaternion* const orient = reinterpret_cast<const Quaternion*>(at(n, 0x1C));
            const Vector* t = quaternion_rotate_004a9bb0(&rotated, orient, &p);    // 004390F7..00439108
            p = *t;                                                                 // 0043910D..00439121
            p.x = static_cast<float>(static_cast<double>(p.x) + get<float>(n, 0x10)); // 00439113..00439128
            p.y = static_cast<float>(static_cast<double>(p.y) + get<float>(n, 0x14)); // 00439130..00439137
            p.z = static_cast<float>(static_cast<double>(p.z) + get<float>(n, 0x18)); // 0043913B..00439156
            Quaternion product;
            const Quaternion* const q = quaternion_multiply_004a9ac0(&product, orient, &r); // 0043914A..00439171
            r = *q;                                                                 // 00439176..00439196
        }
        if (positions.size == positions.capacity)                                   // 004391BE..004391C7
            vector_list_allocate_00421310(&positions, nullptr, positions.capacity ? positions.capacity * 2 : 0x10); // 004391C9..004391DE
        ptr<Vector>(positions.data)[positions.size] = p;                            // 004391EA..00439203
        positions.size = positions.size + 1;                                        // 00439230..00439233
        if (orientations.size == orientations.capacity)                             // 00439229..00439241
            quaternion_list_allocate_00437cc0(&orientations, nullptr, orientations.capacity ? orientations.capacity * 2 : 0x10); // 00439243..00439258
        ptr<Quaternion>(orientations.data)[orientations.size] = r;                  // 00439264..00439288
        orientations.size = orientations.size + 1;                                  // 00439293..00439297
    }
    if (nodes.size >= 1) {                                                          // 004392A8..004392AB
        MaxTree* const model = aurora_tree_as_model_0043e1c0(get<MaxTree*>(ptr<MdlNode>(ptr<U>(nodes.data)[0]), 8), nullptr); // 004392AD..004392B6
        pool = static_cast<unsigned char*>(aurora_pool_lock_00423f10(get<U>(model, 0xAC), 0, 0, 0x10)); // 004392BB..004392D0
    }
    for (I i = 0; i < nodes.size - 1; ++i) {                                        // 004392D4..004392E5 / 00439945..0043996F
        const Quaternion* const qi = ptr<Quaternion>(orientations.data) + i;        // S68
        const Vector* const vi = ptr<Vector>(positions.data) + i;                   // S94 (-8)
        for (I v = 0; v < static_cast<I>(get<I16>(ptr<MdlNode>(ptr<U>(nodes.data)[i]), 0x180)); ++v) { // 00439310..0043932E / 00439918..0043993B
            MdlNode* const mi = ptr<MdlNode>(ptr<U>(nodes.data)[i]);
            const Vector world = world_point(qi, vertex_at(pool, mi, v, 0x154), *vi); // 00439340..004393E9 (S78..S80)
            reset_pair(normals, addr(vertex_ptr(pool, mi, v, 0x158)), owners, static_cast<U>(i)); // 004393F0..004394D9
            {
                const Vector local_normal = vertex_at(pool, mi, v, 0x158);          // 004394B3..004394C0 (S58)
                Vector rotated;
                sum = *quaternion_rotate_004a9bb0(&rotated, qi, &local_normal);     // 004394C3..004394F6
            }
            for (I j = 0; j < nodes.size; ++j) {                                    // 004394FA..0043950B / 004397F5..00439819
                if (i == j) continue;                                               // 00439530..00439538
                const Quaternion* const qj = ptr<Quaternion>(orientations.data) + j; // S64
                const Vector* const vj = ptr<Vector>(positions.data) + j;           // SC4 (-8)
                for (I w = 0; w < static_cast<I>(get<I16>(ptr<MdlNode>(ptr<U>(nodes.data)[j]), 0x180)) + 1; ++w) { // 00439547..0043955A / 004397D2..004397EF (one past the count, original bound)
                    MdlNode* const mj = ptr<MdlNode>(ptr<U>(nodes.data)[j]);
                    const Vector other = world_point(qj, vertex_at(pool, mj, w, 0x154), *vj); // 00439560..004395E7 (S14..S1C)
                    const double dx = static_cast<double>(other.x) - world.x;       // 004395EB..004395F3 (kept in ST0)
                    const float dy = static_cast<float>(static_cast<double>(other.y) - world.y); // 004395FE..00439606 (S70)
                    const float dz = static_cast<float>(static_cast<double>(other.z) - world.z); // 0043960A..00439615 (S74)
                    if (!within(dx) || !within(dy) || !within(dz)) continue;        // 00439619..00439683
                    unsigned char* const normal = vertex_ptr(pool, mj, w, 0x158);   // 00439689..004396B5
                    grow_and_append(normals, addr(normal));                         // 004396B7..0043971E
                    grow_and_append(owners, static_cast<U>(j));                     // 0043971C..0043978E
                    Vector local_normal;
                    std::memcpy(&local_normal, normal, sizeof local_normal);        // 00439787..004397A5 (S58)
                    Vector rotated;
                    const Vector* const r = quaternion_rotate_004a9bb0(&rotated, qj, &local_normal); // 0043979C..004397B2
                    vector_add_assign_004a9740(&sum, nullptr, r);                   // 004397BA..004397BF
                }
            }
            if (normals.size == 1) continue;                                        // 0043981F..00439822
            const double count = static_cast<double>(normals.size);                 // 00439828 (fild)
            if (count != 0.0) {                                                     // 0043982C..00439837
                const double inv = 1.0 / count;                                     // 00439839 (0073D764)
                sum.x = static_cast<float>(sum.x * inv);                            // 0043983F..00439845
                sum.y = static_cast<float>(sum.y * inv);                            // 00439853..00439859
                sum.z = static_cast<float>(sum.z * inv);                            // 00439860..0043986D
            }
            vector_normalize_004ab130(&sum, nullptr);                               // 0043988F..0043989B
            for (I k = 0; k < normals.size; ++k) {                                  // 004398A0..00439916
                const I owner = static_cast<I>(ptr<U>(owners.data)[k]);             // 004398B5
                Quaternion conjugate;
                const Quaternion* const c = quaternion_negate_w_004a9b50(&conjugate, ptr<Quaternion>(orientations.data) + owner); // 004398B8..004398CB
                Vector local;
                const Vector* const r = quaternion_rotate_004a9bb0(&local, c, &sum);  // 004398D3..004398DC
                std::memcpy(ptr<void>(ptr<U>(normals.data)[k]), r, sizeof(Vector)); // 004398E1..00439913
            }
        }
    }
    if (nodes.size >= 1) {                                                          // 00439975..0043997B
        MaxTree* const model = aurora_tree_as_model_0043e1c0(get<MaxTree*>(ptr<MdlNode>(ptr<U>(nodes.data)[0]), 8), nullptr); // 0043997D..00439986
        aurora_pool_unlock_00423f30(get<U>(model, 0xAC));                           // 0043998B..00439992
    }
    if (owners.data != 0) live_free(ptr<void>(owners.data));                        // 0043999A..004399A3
    if (orientations.data != 0) live_free(ptr<void>(orientations.data));            // 004399AB..004399B4
    if (positions.data != 0) live_free(ptr<void>(positions.data));                  // 004399BC..004399C8
    if (normals.data != 0) live_free(ptr<void>(normals.data));                      // 004399D0..004399D9
    if (nodes.data != 0) live_free(ptr<void>(nodes.data));                          // 004399E1..004399EA
}
