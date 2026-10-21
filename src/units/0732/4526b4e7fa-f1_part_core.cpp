// f1_part_core.cpp - lane-m46-3dmodel fork F1: Part-family class bodies reached through the lane-3d-model
// Part/emitter tables (model_vtables.cpp, D39) and the direct callers in lane-3d-emitter / lane-3d-anim.
// Static reading of GOG swkotor.exe (image base 00400000; lane-sound/full.asm). Every behaviour cites the
// original instruction addresses. Conventions:
//  * D1: original __thiscall entries are __fastcall(self, unused_edx, ...) with the original stack words and
//    callee cleanup; the table slot bodies keep the exact consumer declarations of model_vtables.cpp
//    (U __fastcall name(void*, void*, U...)), whose U return carries the original EAX.
//  * D30-FINAL: x87 wide intermediates are doubles; float only at the original fstp dword stores; values that
//    the original only moves with integer mov are copied as raw DWORDs (memcpy), never through the FPU.
//  * allocator family: 006FA390 free -> live_free (M41).
//  * Virtual calls reload the receiver's table at each call and dispatch through the published typed tables
//    (aurora_vt::VTable_00740ae8 for Part slots, aurora_model::GobVTable for Gob slots) - object-pointer views
//    only, no function-pointer casts (D21/D24/D41).
#include "a7e200e217-model_instance.hpp"
#include "b22a9fca42-material.h"
#include <cstring>

using aurora_model::U;
using aurora_model::I;
using aurora_model::U8;
using aurora_model::U16;
using aurora_model::I16;
using aurora_model::get;
using aurora_model::put;
using aurora_model::addr;
using aurora_model::ptr;
using kotor_scene3d::Vector;
using kotor_scene3d::Quaternion;

namespace {
inline unsigned char* bp(void* p, U off) { return static_cast<unsigned char*>(p) + off; }
inline void* obj(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
inline void copy3(void* dst, const void* src) { std::memcpy(dst, src, 12); }   // three integer mov pairs
inline void copy4(void* dst, const void* src) { std::memcpy(dst, src, 16); }
inline float f32(double v) { return static_cast<float>(v); }               // fstp dword (PC_53, RC nearest)
inline double W(float v) { return static_cast<double>(v); }                 // fld dword
inline const aurora_vt::VTable_00740ae8* part_table(void* part) {          // mov eax,[ecx]; call [eax+n]
    return static_cast<const aurora_vt::VTable_00740ae8*>(get<const void*>(part, 0));
}
inline const aurora_model::GobVTable* gob_table(void* gob) {
    return static_cast<const aurora_model::GobVTable*>(get<const void*>(gob, 0));
}
// sub dl,mask; neg dl; sbb; not; and: node if ((byte & mask) == mask) else 0 (00440209.. / 00441513.. / 0044334A..)
inline void* node_if(void* node, U8 mask) { return (get<U8>(node, 0) & mask) == mask ? node : nullptr; }
// type byte of a MaxTree: model if ((byte[+4C] & 7F) == 2) else 0 (00440206..00440218)
inline void* model_if(void* tree) { return (get<U8>(tree, 0x4C) & 0x7F) == 2 ? tree : nullptr; }
constexpr float kOne_0073d764 = 1.0f;          // .rdata 3F800000
constexpr float kZero_0073d700 = 0.0f;         // .rdata 00000000
constexpr float kMinusOne_00740cbc = -1.0f;    // .rdata BF800000
constexpr float kHalf_0073e9ac = 0.5f;         // .rdata 3F000000
constexpr float kTwo_00741be0 = 2.0f;          // .rdata 40000000
constexpr I16 kSaberSlots_00740a4c = 0x14;     // .rdata word 0014 (movsx/cmp word)
constexpr U kGlModelview_1700 = 0x1700, kGlTexture_1702 = 0x1702;
}  // namespace

// ---------------------------------------------------------------- borrowed / external symbols
namespace kotor_renderer_init {   // M40 aurora-windows-gl-imports (exact owner decorations)
void __stdcall glMatrixMode_0073d228(std::uint32_t);
void __stdcall glPushMatrix_0073d300();
void __stdcall glPopMatrix_0073d2fc();
void __stdcall glLoadIdentity_0073d230();
void __stdcall glRotatef_0073d288(float, float, float, float);
}
extern "C" {
// world-los owners (world_los.h / world_los_geometry.h exact decorations; Vector layout identical {x,y,z})
void __stdcall vector_constructor_iterator_00401130(void* base, U element_size, U count,
    void* (__fastcall* construct)(void*, void*));                                   // RET 10h
void* __fastcall vector_default_construct_00409e70(void* vector, void*);            // RET 0, EAX=this
Vector* __cdecl vector_component_mult_00423bf0(Vector* out, const Vector* v, float scale);
float* __cdecl kotor_cdecl_00423bc0(float* out, const float* a, const float* b);   // r4-lowers Vector::operator- (out=a-b)
// gui-writers owner of 004AA980 operator== (construct3d.h declaration)
std::uint32_t __cdecl aurora_vector_equal_004aa980(const void* left, const void* right);
// transitive callees NOT authored by F1 (hit-check internals; sized in frag/F1.json open_gates)
I __cdecl box_intersec_004aa1f0(const Vector* start, Vector* end, const Vector* box_min, const Vector* box_max, Vector* hit);
I __cdecl polyhit_004ab750(const Vector* points, I count, const void* face_normal, const void* face_distance,
    const Vector* start, Vector* end, Vector* hit);
U __cdecl hit_check_aabb_node_00441570(void* node, const Vector* start, Vector* end, U raw_scale);
U __cdecl anim_mesh_hit_check_00440cf0(void* part, void* node, Vector start, Vector end, Vector* hit, U raw_scale);
// 004AA980 / 004AAE70 / 004AAEE0 / 004A97A0 / 0043E2F0 authored below (new names)
Vector* __cdecl vector_min_004aae70(Vector* out, Vector a, Vector b);
Vector* __cdecl vector_max_004aaee0(Vector* out, Vector a, Vector b);
Vector* __fastcall vector_scale_assign_004a97a0(Vector* self, void* unused_edx, float factor);
}

// ---------------------------------------------------------------- cells owned here (only these functions
// reference them in the image, except where noted; BSS zero / .data values read from the pinned image)
extern "C" {
volatile U aurora_hit_part_name_007a8a20 = 0;            // 00442BA2 (only writer/reader)
volatile U aurora_aabb_direction_mask_007a8a24 = 0;      // 0044298A; read by 00441570 HitCheckAABBnode (not authored)
volatile U aurora_sphere_children_gate_007a8a38 = 0;     // 004430CC (BSS; no writer instruction in .text)
float aurora_hit_point_007a8aa4[3] = {0.0f, 0.0f, 0.0f}; // Part::HitCheck function-static (004429F9..00442A0B)
volatile U aurora_hit_point_guard_007a8ab0 = 0;          // its guard bit 0 (004429DF / 004429F9)
float aurora_trimesh_saved_end_007a8a94[3] = {0.0f, 0.0f, 0.0f}; // PartTriMesh::HitCheckGeom static (00441060..0044106C)
volatile U aurora_trimesh_saved_end_guard_007a8aa0 = 0;  // guard bit 0 (0044104B / 0044105A)
float aurora_hit_box_margin_007a8a4c[3] = {0.0f, 0.0f, 0.0f};    // BSS; also written by 005F610D..005F6119 (non-3D writer, not authored)
void* volatile aurora_aabb_node_007a69b0 = nullptr;      // 00442910; read by 00441570 (not authored)
volatile U aurora_aabb_hit_info_007a69b4 = 0;            // 00442919; read by 00441570 (not authored)
volatile U aurora_saber_min_step_00798b34 = 0x3C23D70Au; // .data 0.01f (00481AE7 / 00481AFA only)
volatile I aurora_saber_count_008302dc = 0;              // 00481D19 / 00481D2F / 00481D46 only
}

// ================================================================ small math lowers (transitive, new names)
// 004A97A0 Vector::operator*=(float), thiscall RET 4, EAX = this: each component fld f; fmul [c]; fstp [c].
Vector* __fastcall vector_scale_assign_004a97a0(Vector* self, void*, float factor) {
    self->x = f32(W(factor) * W(self->x));                                              // 004A97A0..004A97A8
    self->y = f32(W(factor) * W(self->y));                                              // 004A97AA..004A97B1
    self->z = f32(W(factor) * W(self->z));                                              // 004A97B4..004A97BB
    return self;                                                                        // 004A97A4 mov eax,ecx
}
// 004AAE70 VectorMin(out, a, b) cdecl by value: per component fld a.c; fcomp b.c; test ah,5; jp -> b.c else a.c
// (a.c < b.c ordered keeps a.c); stores z first then y,x order is irrelevant (three fstp).
Vector* __cdecl vector_min_004aae70(Vector* out, Vector a, Vector b) {
    const float z = (a.z < b.z) ? a.z : b.z;                                            // 004AAE70..004AAE85
    const float y = (a.y < b.y) ? a.y : b.y;                                            // 004AAE89..004AAE9E
    const float x = (a.x < b.x) ? a.x : b.x;                                            // 004AAEA2..004AAEC6
    out->x = x; out->y = y; out->z = z;                                                 // 004AAEB9..004AAEBE / 004AAECA..004AAECF
    return out;                                                                         // EAX = out
}
// 004AAEE0 VectorMax: test ah,41h; jne -> b.c (a.c <= b.c or unordered) else a.c.
Vector* __cdecl vector_max_004aaee0(Vector* out, Vector a, Vector b) {
    const float z = (a.z > b.z) ? a.z : b.z;                                            // 004AAEE0..004AAEF5
    const float y = (a.y > b.y) ? a.y : b.y;                                            // 004AAEF9..004AAF0E
    const float x = (a.x > b.x) ? a.x : b.x;                                            // 004AAF12..004AAF36
    out->x = x; out->y = y; out->z = z;                                                 // 004AAF29..004AAF2E / 004AAF3A..004AAF3F
    return out;
}

// ================================================================ base Part table 00740A74 (new, D39 family)
// .rdata 00740A74: [0] 0043E2F0 (deleting dtor), [1..7] 006FB8DE _purecall (read from the pinned image;
// the next table 00740A94 PathInfo follows). Written by Part::~Part 004401C2 as the last store.
extern "C" U __fastcall part_base_deleting_destructor_0043e2f0(void* self, void*, U flags);
namespace aurora_vt {
struct VTable_00740a74 {  // part_base (8 slots)
    Slot1 s000;  // 0043E2F0 Part(base)::~Part deleting
    Slot0 s004, s008, s00c, s010, s014, s018, s01c;   // 006FB8DE _purecall (pure virtual; argument count unknown,
                                                      // never returns: R6025 via _amsg_exit)
};
static_assert(sizeof(VTable_00740a74) == 0x20, "part_base slot count");
}
extern "C" int __cdecl _purecall(void);   // vcruntime R6025 (same mapping as model_vtables.cpp slot_purecall_006fb8de)
static U __fastcall slot_purecall_006fb8de_f1(void*, void*) { return static_cast<U>(_purecall()); }
extern "C" const aurora_vt::VTable_00740a74 aurora_vtable_part_base_00740a74 = {
    &part_base_deleting_destructor_0043e2f0,
    &slot_purecall_006fb8de_f1, &slot_purecall_006fb8de_f1, &slot_purecall_006fb8de_f1, &slot_purecall_006fb8de_f1,
    &slot_purecall_006fb8de_f1, &slot_purecall_006fb8de_f1, &slot_purecall_006fb8de_f1,
};
// 0043E2F0: test [esp+4],1; vtbl = 740A74; if (flags & 1) free(self); EAX = self; RET 4.
U __fastcall part_base_deleting_destructor_0043e2f0(void* self, void*, U flags) {
    put<const void*>(self, 0, &aurora_vtable_part_base_00740a74);                      // 0043E2F8
    if (flags & 1) live_free(self);                                                     // 0043E2F0 / 0043E2FE..0043E306
    return addr(self);                                                                  // 0043E309
}

// ================================================================ destructors
extern "C" {
void __fastcall part_destruct_00440140(void* part, void* unused_edx);
void __fastcall part_trimesh_destruct_00440230(void* part, void* unused_edx);
void __fastcall part_light_destruct_00440320(void* part, void* unused_edx);
void __fastcall light_container_destruct_00445600(void* self, void* unused_edx);
void __fastcall part_saber_destruct_00445670(void* part, void* unused_edx);
void __fastcall part_dangly_destruct_004456a0(void* part, void* unused_edx);
void __fastcall part_skin_destruct_00445710(void* part, void* unused_edx);
void __fastcall part_camera_destruct_004477f0(void* part, void* unused_edx);
void __fastcall part_reference_destruct_00447820(void* part, void* unused_edx);
void __fastcall part_aabb_destruct_004478c0(void* part, void* unused_edx);
}
// 00440140 Part::~Part (thiscall, RET 0). EH frame 00715B33 (state 1 over the child list); every callee on the
// path is non-throwing (deleting dtors, Material::~Material [noexcept(false) declaration of a non-throwing body,
// task-17], free) - D30-FINAL: no unwind emitted. EAX at return = the last free's EAX (not reproduced: void).
void __fastcall part_destruct_00440140(void* part, void*) {
    put<const void*>(part, 0, &aurora_vtable_part_00740ae8);                           // 0044015E
    I n = get<I>(part, 0x34);                                                           // 00440164
    while (n != 0) {                                                                    // 00440167 / 00440185..0044018A
        void* const child = obj(ptr<U>(get<U>(part, 0x30))[n - 1]);                     // 00440173..00440176 (data reloaded)
        --n;                                                                            // 0044017A
        if (child != nullptr) part_table(child)->s000(child, nullptr, 1);               // 0044017B..00440183 push 1; call [edx]
        put<I>(part, 0x34, n);                                                          // 00440187
    }
    void* const material = get<void*>(part, 0x44);                                      // 0044018C
    if (material != nullptr) {                                                          // 0044018F
        static_cast<material_owner::Material*>(material)->~Material();                  // 00440193..00440195 (0047B980)
        live_free(material);                                                            // 0044019A..0044019B
    }
    aurora_part_count_007a8a10 = aurora_part_count_007a8a10 - 1;                       // 004401A3 dec [007A8A10]
    void* const data = get<void*>(part, 0x30);                                          // 004401A9
    if (data != nullptr) live_free(data);                                               // 004401AE..004401B6
    put<const void*>(part, 0, &aurora_vtable_part_base_00740a74);                      // 004401C2
}
// 00440230 PartTriMesh::~PartTriMesh (EH frame 00715B48; non-throwing callees - see 00440140).
void __fastcall part_trimesh_destruct_00440230(void* part, void*) {
    put<const void*>(part, 0, &aurora_vtable_part_trimesh_00740b68);                   // 0044024D
    void* const primitive = get<void*>(part, 0x70);                                     // 00440253
    if (primitive != nullptr)                                                           // 00440256
        static_cast<const aurora_vt::VTable_00742f74*>(get<const void*>(primitive, 0))->s000(primitive, nullptr, 1); // 00440262..00440266
    if (get<U8>(part, 0x7C) == 1) {                                                     // 00440268
        const U handle = get<U>(part, 0x74);                                            // 0044026E
        put<U8>(part, 0x7C, 0);                                                         // 00440272 (before the call)
        aurora_pool_delete_00423f50(handle);                                            // 00440276
        put<U>(part, 0x74, 0xFFFFFFFFu);                                                // 0044027E
    }
    const U handle78 = get<U>(part, 0x78);                                              // 00440285
    if (handle78 != 0xFFFFFFFFu) {                                                      // 00440288
        aurora_pool_delete_00423f50(handle78);                                          // 0044028D..0044028E
        put<U>(part, 0x78, 0xFFFFFFFFu);                                                // 00440296
    }
    aurora_trimesh_part_count_007a8a14 = aurora_trimesh_part_count_007a8a14 - 1;       // 0044029D
    void* const block = get<void*>(part, 0x90);                                         // 004402A3
    if (block != nullptr) {                                                             // 004402A9
        live_free(block);                                                               // 004402AD..004402AE
        put<U>(part, 0x90, 0);                                                          // 004402B6
    }
    part_destruct_00440140(part, nullptr);                                              // 004402C0..004402CA
}
// 00440320 PartLight::~PartLight: removes this light from every Gob's light container (Gob+178), then
// tail-jumps to Part::~Part. Quirk (observed): a Gob with a null +178 container reaches 004403D9 with EAX = 0
// and reads [0+1Ch] (access violation in the original); reproduced as the same null-based read.
void __fastcall part_light_destruct_00440320(void* part, void*) {
    put<const void*>(part, 0, &aurora_vtable_part_light_00740c08);                     // 00440322
    const U self = addr(part);
    for (I g = 0; g < aurora_gobs_007a8a58.size; ++g) {                                 // 00440328..00440338 / 004403E4..004403F0
        void* const gob = obj(ptr<U>(aurora_gobs_007a8a58.data)[g]);                    // 00440340..00440345 (data reloaded)
        unsigned char* const lc = static_cast<unsigned char*>(get<void*>(gob, 0x178));  // 00440348
        if (lc != nullptr) {                                                            // 0044034E
            I size = get<I>(lc, 8);                                                     // 00440356
            I matches = 0;
            if (size > 0) {                                                             // 0044035D
                const U* p = ptr<U>(get<U>(lc, 4));                                     // 00440363
                do { if (*p == self) ++matches; ++p; } while (--size != 0);             // 00440366..0044036F
            }
            if (matches != 0) {                                                         // 00440371
                const I count = get<I>(lc, 8);                                          // 00440375
                I index = 0;
                bool found = false;
                if (count > 0) {                                                        // 0044037C
                    const U* p = ptr<U>(get<U>(lc, 4));                                 // 0044037E
                    for (; index < count; ++index, ++p) if (*p == self) { found = true; break; }   // 00440381..0044038B
                }
                if (found && index >= 0) {                                              // 0044038F
                    put<I>(lc, 8, get<I>(lc, 8) - 1);                                   // 00440393..0044039B
                    for (I i = index; i < get<I>(lc, 8); ++i) {                         // 0044039E..004403B2 (data/size reloaded)
                        U* const d = ptr<U>(get<U>(lc, 4));
                        d[i] = d[i + 1];
                    }
                    put<I>(lc, 0x14, get<I>(lc, 0x14) - 1);                             // 004403B8..004403C0
                    for (I i = index; i < get<I>(lc, 0x14); ++i) {                      // 004403C3..004403D7 (data/size reloaded)
                        U* const d = ptr<U>(get<U>(lc, 0x10));
                        d[i] = d[i + 1];
                    }
                }
            }
        }
        unsigned char* const c = lc != nullptr ? lc : reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(0)); // 0044034E je -> EAX = 0
        if (*reinterpret_cast<volatile U*>(c + 0x1C) == self) {                         // 004403D9 cmp [eax+1Ch],ecx
            *reinterpret_cast<volatile U*>(c + 0x1C) = 0;                               // 004403DE
            *reinterpret_cast<volatile U*>(c + 0x20) = 0;                               // 004403E1
        }
    }
    part_destruct_00440140(part, nullptr);                                              // 004403FD jmp 00440140
}
// 00445600 CAurObjectLightContainer::~CAurObjectLightContainer (EH 00715D5B; free only).
void __fastcall light_container_destruct_00445600(void* self, void*) {
    put<const void*>(self, 0, &aurora_vtable_light_container_00740cdc);                // 0044561D
    void* const second = get<void*>(self, 0x10);                                        // 00445623
    if (second != nullptr) live_free(second);                                           // 00445626..00445633
    void* const first = get<void*>(self, 4);                                            // 0044563B
    if (first != nullptr) live_free(first);                                             // 0044563E..0044564B
}
// 00445670 PartLightsaber::~PartLightsaber: vtbl 740CE0; words +3BC/+3BA/+3B8 = 0; jmp 00440230.
void __fastcall part_saber_destruct_00445670(void* part, void*) {
    put<const void*>(part, 0, &aurora_vtable_part_saber_00740ce0);                     // 00445672
    put<U16>(part, 0x3BC, 0);                                                           // 00445678
    put<U16>(part, 0x3BA, 0);                                                           // 0044567F
    put<U16>(part, 0x3B8, 0);                                                           // 00445686
    part_trimesh_destruct_00440230(part, nullptr);                                      // 0044568D
}
// 004456A0 PartDanglyMesh::~PartDanglyMesh (EH 00715D78): free(+B4) unconditionally; DeletePool(+B8); +B8 = -1.
void __fastcall part_dangly_destruct_004456a0(void* part, void*) {
    put<const void*>(part, 0, &aurora_vtable_part_dangly_00740d78);                    // 004456BD
    live_free(get<void*>(part, 0xB4));                                                  // 004456C3..004456D2
    aurora_pool_delete_00423f50(get<U>(part, 0xB8));                                    // 004456D7..004456DE
    put<U>(part, 0xB8, 0xFFFFFFFFu);                                                    // 004456E3..004456EB
    part_trimesh_destruct_00440230(part, nullptr);                                      // 004456E9..004456F5
}
// 00445710 PartSkin::~PartSkin: vtbl 740E10; jmp 00440230.
void __fastcall part_skin_destruct_00445710(void* part, void*) {
    put<const void*>(part, 0, &aurora_vtable_part_skin_00740e10);                      // 00445710
    part_trimesh_destruct_00440230(part, nullptr);                                      // 00445716
}
// 004477F0 PartCamera::~PartCamera = jmp 00440140 (no table store).
void __fastcall part_camera_destruct_004477f0(void* part, void*) { part_destruct_00440140(part, nullptr); }   // 004477F0
// 00447820 PartReference::~PartReference (EH 00715B48): deletes the referenced Gob (+4C) through Gob slot 0.
void __fastcall part_reference_destruct_00447820(void* part, void*) {
    put<const void*>(part, 0, &aurora_vtable_part_reference_00740f30);                 // 0044783D
    void* const gob = get<void*>(part, 0x4C);                                           // 00447843
    if (gob != nullptr) gob_table(gob)->s000_gob_destruct_gob(static_cast<aurora_model::Gob*>(gob), nullptr, 1); // 00447852..00447856
    part_destruct_00440140(part, nullptr);                                              // 00447858..00447862
}
// 004478C0 PartAABB::~PartAABB = jmp 00440230 (no table store).
void __fastcall part_aabb_destruct_004478c0(void* part, void*) { part_trimesh_destruct_00440230(part, nullptr); }   // 004478C0

// Deleting destructors (table slot 0, RET 4): call the destructor; if (flags & 1) free(self); EAX = self.
#define F1_DELETING(NAME, DTOR, A_CALL, A_TEST, A_FREE)                                                       \
    extern "C" U __fastcall NAME(void* self, void*, U flags) {                                                \
        DTOR(self, nullptr);                         /* A_CALL */                                             \
        if (flags & 1) live_free(self);              /* A_TEST / A_FREE */                                    \
        return addr(self);                           /* mov eax,esi */                                        \
    }
F1_DELETING(part_deleting_destructor_00445820, part_destruct_00440140, 00445823, 00445828, 00445830)
F1_DELETING(part_tri_mesh_deleting_destructor_00445a20, part_trimesh_destruct_00440230, 00445A23, 00445A28, 00445A30)
F1_DELETING(part_light_deleting_destructor_00445ac0, part_light_destruct_00440320, 00445AC3, 00445AC8, 00445AD0)
F1_DELETING(part_lightsaber_deleting_destructor_00447960, part_saber_destruct_00445670, 00447963, 00447968, 00447970)
F1_DELETING(part_dangly_mesh_deleting_destructor_00447b00, part_dangly_destruct_004456a0, 00447B03, 00447B08, 00447B10)
F1_DELETING(part_skin_deleting_destructor_00447880, part_skin_destruct_00445710, 00447883, 00447888, 00447890)
F1_DELETING(part_camera_deleting_destructor_004477d0, part_camera_destruct_004477f0, 004477D3, 004477D8, 004477E0)
F1_DELETING(part_reference_deleting_destructor_00447800, part_reference_destruct_00447820, 00447803, 00447808, 00447810)
F1_DELETING(part_aabb_deleting_destructor_004478a0, part_aabb_destruct_004478c0, 004478A3, 004478A8, 004478B0)
F1_DELETING(c_aur_object_light_container_deleting_destructor_004455e0, light_container_destruct_00445600, 004455E3, 004455E8, 004455F0)
#undef F1_DELETING

// ================================================================ trivial Part slots
extern "C" {
// 00440200 Part::Name: node = [this+4]; tree = [node+8]; model = (type&7F)==2 ? tree : 0;
// EAX = model->nameTable[node->index] ([[model+B8] + [node+4]*4]). A non-model tree reads [0+B8] (as original).
U __fastcall part_name_00440200(void* self, void*) {
    void* const node = get<void*>(self, 4);                                             // 00440200
    void* const tree = get<void*>(node, 8);                                             // 00440203
    const std::uintptr_t model = reinterpret_cast<std::uintptr_t>(model_if(tree));      // 00440206..00440218
    const U table = *reinterpret_cast<const volatile U*>(model + 0xB8);                 // 0044021A
    return ptr<U>(table)[get<U>(node, 4)];                                              // 00440209 / 00440220
}
// 004401E0 Part::GetChildren: EAX = &this->children (+30).
U __fastcall part_get_children_004401e0(void* self, void*) { return addr(bp(self, 0x30)); }   // 004401E0 lea eax,[ecx+30h]
// 004401F0 Part::GetAurObject: EAX = [this+40].
U __fastcall part_get_aur_object_004401f0(void* self, void*) { return get<U>(self, 0x40); }   // 004401F0
// 0043F5C0 Part::SetPartPosition(out, a1, a2, a3) RET 10h: *out = {0,0,0}; EAX = out.
U __fastcall part_set_part_position_0043f5c0(void*, void*, U out, U, U, U) {
    put<U>(obj(out), 0, 0); put<U>(obj(out), 4, 0); put<U>(obj(out), 8, 0);            // 0043F5C6..0043F5CB
    return out;                                                                         // 0043F5C0 mov eax,[esp+4]
}
// 0046EE30 PartTriMesh::RotateTexture(angle) RET 4: [this+94] = angle (raw DWORD); EAX = angle.
U __fastcall part_tri_mesh_rotate_texture_0046ee30(void* self, void*, U angle) {
    put<U>(self, 0x94, angle);                                                          // 0046EE34
    return angle;                                                                       // 0046EE30
}
// 0046F770 PartTriMesh::RestoreModelviewMatrix: if (!gob) jmp glPopMatrix; else EAX = gob.
U __fastcall part_tri_mesh_restore_modelview_matrix_0046f770(void* self, void*) {
    const U gob = get<U>(self, 0x40);                                                   // 0046F770
    if (gob == 0) { kotor_renderer_init::glPopMatrix_0073d2fc(); return 0; }            // 0046F777 jmp [0073D2FC] (EAX = glPopMatrix's, not reproduced)
    return gob;                                                                         // 0046F77D
}
// 0046F740 PartTriMesh::TransformModelviewMatrix: if (!gob) { glPushMatrix(); if ([0078E5F0]) Transform(pos, ori, 0); }
U __fastcall part_tri_mesh_transform_modelview_matrix_0046f740(void* self, void*) {
    const U gob = get<U>(self, 0x40);                                                   // 0046F743
    if (gob != 0) return gob;                                                           // 0046F748 (EAX = gob)
    kotor_renderer_init::glPushMatrix_0073d300();                                      // 0046F74A
    const U enabled = *kotor_scene3d::cells.transform_enabled_0078e5f0;                 // 0046F750
    if (enabled != 0)                                                                   // 0046F755
        gl_transform_0046f060(static_cast<const Vector*>(static_cast<void*>(bp(self, 8))),
                              static_cast<const Quaternion*>(static_cast<void*>(bp(self, 0x14))), 0);   // 0046F759..0046F768
    return enabled;   // EAX: 0 when the gate is clear (0046F750); after Transform it is Transform's (not reproduced)
}
// 0046F6C0 PartTriMesh::TransformTextureMatrix: angle = [this+94] (fst local); if (angle != 0.0f || NaN)
// { glMatrixMode(GL_TEXTURE); glPushMatrix(); glLoadIdentity(); glRotatef(angle,0,0,1); glMatrixMode(GL_MODELVIEW); }
U __fastcall part_tri_mesh_transform_texture_matrix_0046f6c0(void* self, void*) {
    const float angle = get<float>(self, 0x94);                                         // 0046F6C1..0046F6C7 (fst dword)
    if (!(angle == kZero_0073d700)) {                                                   // 0046F6CB..0046F6D6 test ah,44h; jnp (equal -> skip)
        kotor_renderer_init::glMatrixMode_0073d228(kGlTexture_1702);                    // 0046F6D9..0046F6E4
        kotor_renderer_init::glPushMatrix_0073d300();                                   // 0046F6E6
        kotor_renderer_init::glLoadIdentity_0073d230();                                 // 0046F6EC
        kotor_renderer_init::glRotatef_0073d288(angle, 0.0f, 0.0f, kOne_0073d764);      // 0046F6F2..0046F700 (angle raw DWORD)
        kotor_renderer_init::glMatrixMode_0073d228(kGlModelview_1700);                  // 0046F706..0046F70B
    }
    return 0;   // EAX = fnstsw/GL return (void method; callers ignore; not reproduced)
}
// 0046F710 PartTriMesh::RestoreTextureMatrix: if ([this+94] != 0) { glMatrixMode(TEXTURE); glPopMatrix(); glMatrixMode(MODELVIEW); }
U __fastcall part_tri_mesh_restore_texture_matrix_0046f710(void* self, void*) {
    if (!(get<float>(self, 0x94) == kZero_0073d700)) {                                  // 0046F710..0046F721
        kotor_renderer_init::glMatrixMode_0073d228(kGlTexture_1702);                    // 0046F724..0046F72F
        kotor_renderer_init::glPopMatrix_0073d2fc();                                    // 0046F731
        kotor_renderer_init::glMatrixMode_0073d228(kGlModelview_1700);                  // 0046F737..0046F73C
    }
    return 0;   // EAX not reproduced (void method)
}
// 00484260 Part::Animate_derived(dt) RET 4: for each child: child->slot68(dt) (count reloaded); EAX = last count read.
U __fastcall part_animate_derived_00484260(void* self, void*, U dt) {
    I count = get<I>(self, 0x34);                                                       // 00484264
    for (I i = 0; i < count; ) {                                                        // 00484269..0048426B
        void* const child = obj(ptr<U>(get<U>(self, 0x30))[i]);                         // 00484272..00484275
        part_table(child)->s068(child, nullptr, dt);                                    // 00484278..0048427B call [edx+68h]
        count = get<I>(self, 0x34);                                                     // 0048427E
        ++i;                                                                            // 00484281
    }
    return static_cast<U>(count);                                                       // EAX = [edi+34h]
}
}  // extern "C"

// Gob model flags (anim3d.h declarations: U8 __fastcall(void* gob, void*)).
extern "C" U8 __fastcall gob_model_flag2_00481d60(void* gob, void*) {
    void* const model = get<void*>(gob, 0x58);                                          // 00481D60
    if (model == nullptr) return 0;                                                     // 00481D63..00481D70 xor al,al
    return static_cast<U8>((get<U8>(model, 0x50) >> 2) & 1);                            // 00481D67..00481D6D
}
extern "C" U8 __fastcall gob_model_flag4_00481d80(void* gob, void*) {
    void* const model = get<void*>(gob, 0x58);                                          // 00481D80
    if (model == nullptr) return 0;                                                     // 00481D83..00481D90
    return static_cast<U8>((get<U8>(model, 0x50) >> 4) & 1);                            // 00481D87..00481D8D
}

// ================================================================ transforms
extern "C" {
// 0043E4A0 Part::GetWorld(pos_out, ori_out) RET 8: identity, then accumulate local transforms up the parent
// chain (+3C) and finally the Gob (+40) transform. EAX = ori_out->z DWORD (last mov eax,[..+0Ch]).
U __fastcall part_get_world_0043e4a0(void* self, void*, U pos_word, U ori_word) {
    Vector* const pos = static_cast<Vector*>(obj(pos_word));
    Quaternion* const ori = static_cast<Quaternion*>(obj(ori_word));
    put<U>(pos, 0, 0); put<U>(pos, 4, 0); put<U>(pos, 8, 0);                            // 0043E4B0..0043E4DB
    put<U>(ori, 0, 0x3F800000u); put<U>(ori, 4, 0); put<U>(ori, 8, 0); put<U>(ori, 0xC, 0);   // 0043E4DE..0043E518
    void* const gob = get<void*>(self, 0x40);                                           // 0043E51B
    if (gob == nullptr) {                                                               // 0043E51E..0043E524
        copy3(pos, bp(self, 8));                                                        // 0043E526..0043E536
        copy4(ori, bp(self, 0x14));                                                     // 0043E539..0043E551
        return get<U>(ori, 0xC);
    }
    void* p = self;
    Vector rv; Quaternion rq;
    do {                                                                                // 0043E560..0043E5E9
        vector_scale_assign_004a97a0(pos, nullptr, get<float>(p, 0x24));                // 0043E560..0043E566 (raw DWORD arg)
        copy3(pos, quaternion_rotate_004a9bb0(&rv, static_cast<const Quaternion*>(static_cast<void*>(bp(p, 0x14))), pos)); // 0043E56B..0043E589
        const float x = f32(W(get<float>(p, 8)) + W(pos->x));                           // 0043E58C..0043E593
        const float y = f32(W(get<float>(p, 0xC)) + W(pos->y));                         // 0043E59B..0043E5A1
        const float z = f32(W(get<float>(p, 0x10)) + W(pos->z));                        // 0043E5A5..0043E5B8
        pos->x = x; pos->y = y; pos->z = z;                                             // 0043E5AF / 0043E5B1 / 0043E5C1
        copy4(ori, quaternion_multiply_004a9ac0(&rq, static_cast<const Quaternion*>(static_cast<void*>(bp(p, 0x14))), ori)); // 0043E5C0..0043E5DE
        p = get<void*>(p, 0x3C);                                                        // 0043E5E1
    } while (p != nullptr);                                                             // 0043E5E7..0043E5E9
    if (get<U>(gob, 0x1B4) != 0x3F800000u)                                              // 0043E5F3..0043E5FE (integer compare)
        vector_scale_assign_004a97a0(pos, nullptr, get<float>(gob, 0x1B4));             // 0043E600..0043E605
    copy3(pos, quaternion_rotate_004a9bb0(&rv, static_cast<const Quaternion*>(static_cast<void*>(bp(gob, 0x84))), pos)); // 0043E60A..0043E62B
    const float x = f32(W(get<float>(gob, 0x78)) + W(pos->x));                          // 0043E62E..0043E635
    const float y = f32(W(get<float>(gob, 0x7C)) + W(pos->y));                          // 0043E63D..0043E643
    const float z = f32(W(get<float>(gob, 0x80)) + W(pos->z));                          // 0043E647..0043E65D
    pos->x = x; pos->y = y; pos->z = z;                                                 // 0043E654 / 0043E65A / 0043E666
    copy4(ori, quaternion_multiply_004a9ac0(&rq, static_cast<const Quaternion*>(static_cast<void*>(bp(gob, 0x84))), ori)); // 0043E665..0043E687
    return get<U>(ori, 0xC);                                                            // 0043E682
}
// 0043E6A0 Part::GetLocal(pos_io, ori_io) RET 8: transform a world pose into this part's local frame
// (parent's GetLocal first, else the Gob inverse), then this part's inverse; finally pos *= 1/scale.
// EAX = pos_io (004A97A0 returns this).
U __fastcall part_get_local_0043e6a0(void* self, void*, U pos_word, U ori_word) {
    Vector* const pos = static_cast<Vector*>(obj(pos_word));
    Quaternion* const ori = static_cast<Quaternion*>(obj(ori_word));
    U scale_bits = 0x3F800000u;                                                         // 0043E6B6 [esp+10h] = 1.0f
    Quaternion nq, rq; Vector dv, rv;
    void* const parent = get<void*>(self, 0x3C);                                        // 0043E6B1
    if (parent != nullptr) {                                                            // 0043E6B4
        part_table(parent)->s008(parent, nullptr, pos_word, ori_word);                  // 0043E6C0..0043E6C4 call [eax+8]
        scale_bits = get<U>(get<void*>(self, 0x3C), 0x24);                              // 0043E6C7..0043E6CA / 0043E77B
    } else {
        void* const gob = get<void*>(self, 0x40);                                       // 0043E6D2
        if (gob != nullptr) {                                                           // 0043E6D5..0043E6D7
            const Quaternion* const gq = static_cast<const Quaternion*>(static_cast<void*>(bp(gob, 0x84)));
            copy4(ori, quaternion_multiply_004a9ac0(&rq, quaternion_negate_w_004a9b50(&nq, gq), ori)); // 0043E6DD..0043E711
            void* const g2 = get<void*>(self, 0x40);                                    // 0043E717 (reloaded)
            const float z = f32(W(pos->z) - W(get<float>(g2, 0x80)));                   // 0043E714..0043E71A
            const float y = f32(W(pos->y) - W(get<float>(g2, 0x7C)));                   // 0043E723..0043E72A
            const float x = f32(W(pos->x) - W(get<float>(g2, 0x78)));                   // 0043E72E..0043E735
            dv.x = x; dv.y = y; dv.z = z;                                               // 0043E73E..0043E746
            copy3(pos, quaternion_rotate_004a9bb0(&rv,
                  quaternion_negate_w_004a9b50(&nq, static_cast<const Quaternion*>(static_cast<void*>(bp(g2, 0x84)))), &dv)); // 0043E74A..0043E76C
            scale_bits = get<U>(get<void*>(self, 0x40), 0x1B4);                         // 0043E76F..0043E77B
        }
    }
    const Quaternion* const sq = static_cast<const Quaternion*>(static_cast<void*>(bp(self, 0x14)));
    copy4(ori, quaternion_multiply_004a9ac0(&rq, quaternion_negate_w_004a9b50(&nq, sq), ori));   // 0043E77F..0043E7AF
    dv.x = f32(W(pos->x) - W(get<float>(self, 8)));                                     // 0043E7B2..0043E7BF
    dv.y = f32(W(pos->y) - W(get<float>(self, 0xC)));                                   // 0043E7C7..0043E7CF
    dv.z = f32(W(pos->z) - W(get<float>(self, 0x10)));                                  // 0043E7D3..0043E7D9
    copy3(pos, quaternion_rotate_004a9bb0(&rv, quaternion_negate_w_004a9b50(&nq, sq), &dv)); // 0043E7DD..0043E80C
    float scale; std::memcpy(&scale, &scale_bits, 4);
    const float inverse = f32(W(kOne_0073d764) / W(scale));                             // 0043E7F0..0043E80F fld 1.0; fdiv; fstp dword
    return addr(vector_scale_assign_004a97a0(pos, nullptr, inverse));                   // 0043E812
}
}  // extern "C"

// ================================================================ bounds
extern "C" {
// 00440960 Part::GetMinimumBoundingBox(min_io, max_io, scale) RET 0Ch. For a non-saber trimesh with vertices
// (+180 != 0) every face corner (word index at face+1A/+1C/+1E, stride 20h) is scaled, moved to world space,
// back into Gob-local space and folded into min/max; then each child's slot 74 recurses with the scaled factor.
// EAX = last child-count read ([edi+34h]).
U __fastcall part_get_minimum_bounding_box_00440960(void* self, void*, U min_word, U max_word, U scale_bits) {
    Vector* const vmin = static_cast<Vector*>(obj(min_word));                           // ebp
    Vector* const vmax = static_cast<Vector*>(obj(max_word));                           // ebx
    void* mesh = obj(part_table(self)->s02c(self, nullptr));                            // 0044096C..00440972 call [eax+2Ch]
    if (mesh != nullptr) {                                                              // 00440977..0044097D
        void* const n = get<void*>(mesh, 4);                                            // 0044097F
        if ((get<U16>(n, 0) & 0x821) == 0x821) mesh = nullptr;                          // 00440982..00440992 -> 004409AE
        else if (get<U16>(node_if(n, 0x21), 0x180) == 0) mesh = nullptr;                // 00440994..004409AC
    }
    float scale_arg; std::memcpy(&scale_arg, &scale_bits, 4);
    const float s = f32(W(scale_arg) * W(get<float>(self, 0x24)));                      // 004409BC..004409D4
    if (mesh != nullptr) {                                                              // 004409D8
        Vector pos; Quaternion ori;
        put<U>(&pos, 0, 0); put<U>(&pos, 4, 0); put<U>(&pos, 8, 0);                     // 004409EC..004409FC
        put<U>(&ori, 0xC, 0); put<U>(&ori, 8, 0); put<U>(&ori, 4, 0); put<U>(&ori, 0, 0x3F800000u);   // 00440A04..00440A1C
        part_table(mesh)->s00c(mesh, nullptr, addr(&pos), addr(&ori));                  // 00440A24 call [eax+0Ch] (GetWorld)
        void* const node = node_if(get<void*>(mesh, 4), 0x21);                          // 00440A27..00440A3C
        const U pool = get<U>(model_if(get<void*>(node, 8)), 0xAC);                     // 00440A3E..00440A52
        const U base = addr(aurora_pool_lock_00423f10(pool, 0, 0, 8));                  // 00440A32..00440A59 push 8,0,0,pool
        const I faces = get<I>(node, 0x5C);                                             // 00440A62
        U face_off = 0x1A;                                                              // 00440A78 [esp+24h] = 1Ah
        for (I f = 0; f < faces; ++f) {                                                 // 00440A68..00440A72 / 00440C77..00440C90
            U corner_off = face_off;                                                    // 00440A80
            for (int k = 3; k != 0; --k) {                                              // 00440A84 / 00440C62..00440C71
                const I16 index = get<I16>(obj(get<U>(node, 0x58)), corner_off);        // 00440A90..00440A93 movsx
                const U vaddr = static_cast<U>(static_cast<I>(get<U>(node, 0x14C)) * static_cast<I>(index)) +
                                get<U>(node, 0x194) + base;                             // 00440A97..00440AAA
                Vector v;
                v.x = f32(W(get<float>(obj(vaddr), 0)) * W(s));                         // 00440AC4..00440AE8
                v.y = f32(W(get<float>(obj(vaddr), 4)) * W(s));                         // 00440ACA..00440B01
                v.z = f32(W(get<float>(obj(vaddr), 8)) * W(s));                         // 00440AD1 / 00440AFD..00440B12
                Vector rv;
                const Vector* const r = quaternion_rotate_004a9bb0(&rv, &ori, &v);      // 00440AF0..00440B1E
                Vector w;
                w.x = f32(W(pos.x) + W(r->x));                                          // 00440B23..00440B2C
                w.y = f32(W(pos.y) + W(r->y));                                          // 00440B30..00440B37
                w.z = f32(W(pos.z) + W(r->z));                                          // 00440B3B..00440B55
                v = w;                                                                  // 00440B46..00440B65 (raw copies)
                void* const gob = get<void*>(mesh, 0x40);                               // 00440B4E..00440B52
                Vector d;
                d.z = f32(W(v.z) - W(get<float>(gob, 0x80)));                           // 00440B5D..00440B69 / 00440B98
                d.y = f32(W(v.y) - W(get<float>(gob, 0x7C)));                           // 00440B6F..00440B77 / 00440B94
                d.x = f32(W(v.x) - W(get<float>(gob, 0x78)));                           // 00440B7B..00440B84 / 00440B90
                Quaternion nq; Vector lv;
                const Vector* const l = quaternion_rotate_004a9bb0(&lv,
                    quaternion_negate_w_004a9b50(&nq, static_cast<const Quaternion*>(static_cast<void*>(bp(gob, 0x84)))), &d); // 00440B9F..00440BB0
                copy3(&v, l);                                                           // 00440BB5..00440BC7
                Vector a = *vmin, t;                                                    // 00440BDD..00440BF1 (by-value copies)
                copy3(vmin, vector_min_004aae70(&t, a, v));                             // 00440BF4..00440C14
                Vector b = *vmax;                                                       // 00440C2C..00440C40
                copy3(vmax, vector_max_004aaee0(&t, b, v));                             // 00440C43..00440C5F
                corner_off += 2;                                                        // 00440C69
            }
            face_off += 0x20;                                                           // 00440C83
        }
        aurora_pool_unlock_00423f30(get<U>(model_if(get<void*>(node, 8)), 0xAC));       // 00440C9A..00440CB5
    }
    I count = get<I>(self, 0x34);                                                       // 00440CBD
    for (I i = 0; i < count; ) {                                                        // 00440CC2..00440CDE
        void* const child = obj(ptr<U>(get<U>(self, 0x30))[i]);                         // 00440CC6..00440CC9
        part_table(child)->s074(child, nullptr, min_word, max_word, kotor_scene3d::float_to_bits(s)); // 00440CCC..00440CD5 call [edx+74h]
        count = get<I>(self, 0x34);                                                     // 00440CD8
        ++i;
    }
    return static_cast<U>(count);
}
}  // extern "C"

// ================================================================ spheres
extern "C" {
// 00443080 Part::GetMinimumSphere(center_out, radius_out) RET 8. Starts from {0,0,0}/0 (radius from the
// slot-34 query object +D0 when the Gob model flag bit 0 is set), then merges each child's sphere (slot 78)
// moved into this part's frame. Merge (0044316E..004432CA): big/small radii; if dist + small <= big or
// dist == 0 keep the bigger sphere, else the enclosing sphere. EAX = center.z DWORD (0044330C).
U __fastcall part_get_minimum_sphere_00443080(void* self, void*, U center_word, U radius_word) {
    float c[3]; put<U>(c, 0, 0); put<U>(c, 4, 0); put<U>(c, 8, 0);                      // 00443088..00443098 [esp+18h..20h]
    U r_bits = 0;                                                                       // 004430A0 [esp+4]
    const U q = part_table(self)->s034(self, nullptr);                                  // 00443086..004430A8 call [eax+34h]
    if (q != 0) {                                                                       // 004430AB
        void* const model = get<void*>(get<void*>(self, 0x40), 0x58);                   // 004430AF..004430B2
        if (get<U8>(model, 0x50) & 1) r_bits = get<U>(obj(q), 0xD0);                    // 004430B5..004430C1
    }
    if (get<U8>(self, 0x2C) != 0 || aurora_sphere_children_gate_007a8a38 != 0) {        // 004430C5..004430D3
        for (I i = 0; i < get<I>(self, 0x34); ++i) {                                    // 004430D9..004430E1 / 004432EC..004432F2
            float cc[3]; put<U>(cc, 0, 0); put<U>(cc, 4, 0); put<U>(cc, 8, 0);          // 004430F7..00443107
            U cr_bits = 0;                                                              // 0044310F
            void* const child = obj(ptr<U>(get<U>(self, 0x30))[i]);                     // 004430F0 / 00443117
            part_table(child)->s078(child, nullptr, addr(cc), addr(&cr_bits));          // 0044311A..00443122 call [edx+78h]
            void* const ch = obj(ptr<U>(get<U>(self, 0x30))[i]);                        // 00443125..00443128 (reloaded)
            Vector rv;
            const Vector* const rot = quaternion_rotate_004a9bb0(&rv,
                static_cast<const Quaternion*>(static_cast<void*>(bp(ch, 0x14))), reinterpret_cast<const Vector*>(cc)); // 0044312B..00443139
            float w[3];
            w[0] = f32(W(rot->x) + W(get<float>(ch, 8)));                               // 0044313E..00443146
            w[1] = f32(W(rot->y) + W(get<float>(ch, 0xC)));                             // 0044314E..00443154
            w[2] = f32(W(rot->z) + W(get<float>(ch, 0x10)));                            // 00443158..0044316A
            copy3(cc, w);                                                               // 00443162..0044317A (raw copies)
            float r, cr; std::memcpy(&r, &r_bits, 4); std::memcpy(&cr, &cr_bits, 4);
            const bool less = r < cr;                                                   // fcomp; test ah,5 (jp: >= or unordered)
            const U big_bits = less ? cr_bits : r_bits;                                 // 0044316E..00443193 [esp+18h]
            const U small_bits = less ? r_bits : cr_bits;                               // 00443197..004431B4 [esp+1Ch]
            const float* const big_center = less ? cc : c;                              // 004431B8..004431CB
            float big, small; std::memcpy(&big, &big_bits, 4); std::memcpy(&small, &small_bits, 4);
            Vector d;
            d.x = f32(W(c[0]) - W(w[0]));                                               // 004431CF..004431DC
            d.y = f32(W(c[1]) - W(w[1]));                                               // 004431E0..004431E8
            d.z = f32(W(c[2]) - W(w[2]));                                               // 004431EC..004431F4
            const double dist_wide = vector_magnitude_004ab100(&d);                     // 004431F8 (ST0)
            const float dist = f32(dist_wide);                                          // 004431FD fst dword [esp+18h]
            const double reach = dist_wide + W(small);                                  // 00443201 fadd (wide)
            if (!(reach <= W(big)) && !(dist == kZero_0073d700)) {                      // 00443208..00443211 jnp / 00443217..00443226 jnp
                const double a = W(r) / W(dist);                                        // 0044322C..00443230
                const double b = (W(kMinusOne_00740cbc) / W(dist)) * W(cr) + a;         // 00443234..00443242
                const double t = (b + W(kOne_0073d764)) * W(kHalf_0073e9ac);            // 00443244..0044324A
                const double rr = ((a + W(kOne_0073d764)) - t) * W(dist);               // 00443250..0044325A
                const float nr = f32(rr);                                               // 0044325E fstp [esp+0Ch]
                std::memcpy(&r_bits, &nr, 4);
                const double dx = W(c[0]) - W(cc[0]);                                   // 00443262..00443266 (wide)
                const double dy = W(c[1]) - W(cc[1]);                                   // 0044326A..0044326E (wide)
                const float dz = f32(W(c[2]) - W(cc[2]));                               // 00443272..0044327A fstp [esp+70h]
                const float sx = f32(dx * t);                                           // 0044327E..00443282
                const float sy = f32(dy * t);                                           // 00443286..00443288
                const double sz = W(dz) * t;                                            // 0044328C..00443294 (wide)
                c[0] = f32(W(sx) + W(cc[0]));                                           // 00443296..004432AA
                c[1] = f32(W(sy) + W(cc[1]));                                           // 004432A6..004432BE
                c[2] = f32(sz + W(cc[2]));                                              // 004432BA..004432CA
            } else {
                copy3(c, big_center);                                                   // 004432D0..004432E8
                r_bits = big_bits;                                                      // 004432D2..004432DD
            }
        }
    }
    copy3(obj(center_word), c);                                                         // 004432FA..0044331A
    float rf; std::memcpy(&rf, &r_bits, 4);
    *static_cast<float*>(obj(radius_word)) = rf;                                        // 004432FE / 0044331D fld; fstp dword
    return get<U>(c, 8);                                                                // 0044330C mov eax,[esp+20h]
}
// 00443330 PartTriMesh::GetMinimumSphere(center_io, radius_io) RET 8: Part::GetMinimumSphere, then merge the
// mesh's own sphere (node +7C radius, +80 center) the same way (0.5 factor; enclosing sphere).
// EAX = new center.y DWORD on the merge path; on the keep path EAX = the fnstsw word (not reproduced: 0).
U __fastcall part_tri_mesh_get_minimum_sphere_00443330(void* self, void*, U center_word, U radius_word) {
    part_get_minimum_sphere_00443080(self, nullptr, center_word, radius_word);          // 0044333E..00443342
    float* const center = static_cast<float*>(obj(center_word));                        // esi
    float* const radius = static_cast<float*>(obj(radius_word));                        // edi
    void* const node = node_if(get<void*>(self, 4), 0x21);                              // 00443347..00443356
    const float mr = get<float>(node, 0x7C);                                            // 00443358 fld
    float mc[3]; copy3(mc, bp(node, 0x80));                                             // 0044335B..00443378 (raw)
    const float r_now = *radius;                                                        // 0044337F
    float small, big;
    if (mr < r_now) { small = mr; big = r_now; }                                        // 00443362..00443387 (fcom; test ah,5; jp)
    else { small = r_now; big = mr; copy3(center, mc); }                                // 0044338D..0044339C
    Vector d;
    d.x = f32(W(mc[0]) - W(center[0]));                                                 // 0044339F..004433AA
    d.y = f32(W(mc[1]) - W(center[1]));                                                 // 004433AE..004433B5
    d.z = f32(W(mc[2]) - W(center[2]));                                                 // 004433B9..004433C0
    const double dist_wide = vector_magnitude_004ab100(&d);                             // 004433C4
    const float dist = f32(dist_wide);                                                  // 004433C9 fst
    if ((dist_wide + W(small)) <= W(big) || dist == kZero_0073d700) {                   // 004433CD..004433F2
        *radius = big;                                                                  // 00443487..0044348B (raw DWORD)
        return 0;
    }
    const double h = ((W(dist) - W(*radius)) + W(mr)) * W(kHalf_0073e9ac);              // 004433F8..00443402
    *radius = f32((W(dist) + W(*radius)) - h);                                          // 00443408..00443410
    const double dx = W(mc[0]) - W(center[0]);                                          // 00443413..00443417 (wide)
    const float dy = f32(W(mc[1]) - W(center[1]));                                      // 00443419..00443420
    const float dz = f32(W(mc[2]) - W(center[2]));                                      // 00443424..0044342B
    const float k = f32(h / W(dist));                                                   // 0044342F..00443435
    const double sx = dx * W(k);                                                        // 00443439 (wide)
    const double sy = W(dy) * W(k);                                                     // 0044343D..00443441 (wide)
    const float sz = f32(W(dz) * W(k));                                                 // 00443445..0044344D
    const float nx = f32(sx + W(center[0]));                                            // 00443451..00443455
    const float ny = f32(sy + W(center[1]));                                            // 0044345D..00443460
    const float nz = f32(W(sz) + W(center[2]));                                         // 00443464..00443474
    center[0] = nx; center[1] = ny; center[2] = nz;                                     // 0044346F / 00443471 / 0044347C
    U ny_bits; std::memcpy(&ny_bits, &ny, 4);
    return ny_bits;                                                                     // 00443468 mov eax,[esp+14h]
}
// 0046F1A0 PartTriMesh::WithinRange(x, y, z, range) RET 10h: EAX = (range + 2.0 > |pos - p|) ? 1 : 0.
U __fastcall part_tri_mesh_within_range_0046f1a0(void* self, void*, U x_bits, U y_bits, U z_bits, U range_bits) {
    float x, y, z, range;
    std::memcpy(&x, &x_bits, 4); std::memcpy(&y, &y_bits, 4); std::memcpy(&z, &z_bits, 4); std::memcpy(&range, &range_bits, 4);
    Vector d;
    d.x = f32(W(get<float>(self, 8)) - W(x));                                           // 0046F1A3..0046F1AF
    d.y = f32(W(get<float>(self, 0xC)) - W(y));                                         // 0046F1B3..0046F1BA
    d.z = f32(W(get<float>(self, 0x10)) - W(z));                                        // 0046F1BE..0046F1C5
    const double m = vector_magnitude_004ab100(&d);                                     // 0046F1C9
    const double limit = W(range) + W(kTwo_00741be0);                                   // 0046F1CE..0046F1D2
    return (limit > m) ? 1u : 0u;                                                       // 0046F1DB..0046F1EF fcompp; test ah,41h; jne -> 0
}
}  // extern "C"

// ================================================================ hit checks
extern "C" {
// 004429D0 Part::HitCheck(start(3), end(3), hit_out, info, scale) RET 24h. Moves the segment into this part's
// frame (function-static scratch 007A8AA4), tests the geometry (slot 20) and the children (slot 1C), and on a
// hit maps the hit point (and the info normal +40) back. EAX = the hit part (or 0).
U __fastcall part_hit_check_004429d0(void* self, void*, U sx, U sy, U sz, U ex, U ey, U ez, U hit_out, U info, U scale_bits) {
    float scale_arg; std::memcpy(&scale_arg, &scale_bits, 4);
    const float scale = f32(W(scale_arg) * W(get<float>(self, 0x24)));                  // 004429D3..004429EE (stored back to the arg slot)
    U result = 0;                                                                        // 004429F3
    if ((aurora_hit_point_guard_007a8ab0 & 1) == 0) {                                   // 004429DF..004429F7
        aurora_hit_point_guard_007a8ab0 = aurora_hit_point_guard_007a8ab0 | 1;          // 004429F9
        put<U>(aurora_hit_point_007a8aa4, 0, 0); put<U>(aurora_hit_point_007a8aa4, 4, 0); put<U>(aurora_hit_point_007a8aa4, 8, 0); // 004429FF..00442A0B
    }
    float* const hp = aurora_hit_point_007a8aa4;
    const Quaternion* const ori = static_cast<const Quaternion*>(static_cast<void*>(bp(self, 0x14)));   // ebx
    float s[3], e[3];
    std::memcpy(&s[0], &sx, 4); std::memcpy(&s[1], &sy, 4); std::memcpy(&s[2], &sz, 4);
    std::memcpy(&e[0], &ex, 4); std::memcpy(&e[1], &ey, 4); std::memcpy(&e[2], &ez, 4);
    const bool unit = (scale == kOne_0073d764);                                         // 00442A11..00442A2E test ah,44h; jp
    Quaternion nq; Vector rv;
    const float px = get<float>(self, 8), py = get<float>(self, 0xC), pz = get<float>(self, 0x10);
    if (unit) {
        hp[0] = f32(W(s[0]) - W(px)); hp[1] = f32(W(s[1]) - W(py)); hp[2] = f32(W(s[2]) - W(pz));   // 00442A30..00442A51
    } else {
        hp[0] = f32(W(s[0]) - W(scale) * W(px)); hp[1] = f32(W(s[1]) - W(scale) * W(py)); hp[2] = f32(W(s[2]) - W(scale) * W(pz)); // 00442AA0..00442ACD
    }
    copy3(s, quaternion_rotate_004a9bb0(&rv, quaternion_negate_w_004a9b50(&nq, ori), reinterpret_cast<const Vector*>(hp))); // 00442A57..00442A7E / 00442AD3..00442AFE / 00442B37
    if (unit) {
        hp[0] = f32(W(e[0]) - W(px)); hp[1] = f32(W(e[1]) - W(py)); hp[2] = f32(W(e[2]) - W(pz));   // 00442A6A..00442A98 / 00442B26
    } else {
        hp[0] = f32(W(e[0]) - W(scale) * W(px)); hp[1] = f32(W(e[1]) - W(scale) * W(py)); hp[2] = f32(W(e[2]) - W(scale) * W(pz)); // 00442AE6..00442B1F / 00442B26
    }
    copy3(e, quaternion_rotate_004a9bb0(&rv, quaternion_negate_w_004a9b50(&nq, ori), reinterpret_cast<const Vector*>(hp))); // 00442B2C..00442B6C
    void* const hit_info = obj(info);                                                   // edi
    const U geom = part_table(self)->s020(self, nullptr, addr(s), addr(e), addr(e), info, kotor_scene3d::float_to_bits(scale)); // 00442B66..00442B82 call [eax+20h]
    if (geom != 0 && (get<U>(hit_info, 0x58) == 0 || geom != 0xFFFFFFFFu)) {           // 00442B85..00442B93
        result = addr(self);                                                            // 00442B97..00442B9B
        aurora_hit_part_name_007a8a20 = part_table(self)->s004(self, nullptr);         // 00442B95..00442BA2 call [eax+4]
        put<U>(hit_info, 0x4C, part_table(self)->s004(self, nullptr));                  // 00442BA7..00442BAE
    }
    for (I i = 0; i < get<I>(self, 0x34); ++i) {                                        // 00442BB1..00442C26 (count reloaded)
        void* const child = obj(ptr<U>(get<U>(self, 0x30))[i]);                         // 00442BC0..00442BC3
        U a[6]; std::memcpy(a, s, 12); std::memcpy(a + 3, e, 12);                       // 00442BD7..00442C02 (by-value copies)
        const U r = part_table(child)->s01c(child, nullptr, a[0], a[1], a[2], a[3], a[4], a[5], addr(e), info,
                                            kotor_scene3d::float_to_bits(scale));       // 00442BC6..00442C05 call [edx+1Ch]
        if (r != 0) result = r;                                                         // 00442C08..00442C14
    }
    if (result != 0) {                                                                  // 00442C28
        copy3(hp, quaternion_rotate_004a9bb0(&rv, ori, reinterpret_cast<const Vector*>(e)));   // 00442C41..00442C6B / 00442C93..00442CBB
        if (unit) {
            hp[0] = f32(W(hp[0]) + W(px)); hp[1] = f32(W(hp[1]) + W(py)); hp[2] = f32(W(hp[2]) + W(pz)); // 00442C59..00442C91 / 00442CF2
        } else {
            hp[0] = f32(W(scale) * W(px) + W(hp[0]));                                   // 00442CA3..00442CC9
            hp[1] = f32(W(scale) * W(py) + W(hp[1]));                                   // 00442CCF..00442CDC
            hp[2] = f32(W(scale) * W(pz) + W(hp[2]));                                   // 00442CE2..00442CE9 / 00442CF2
        }
        if (hit_info != nullptr) {                                                      // 00442CF8
            Vector* const normal = static_cast<Vector*>(static_cast<void*>(bp(hit_info, 0x40)));
            copy3(normal, quaternion_rotate_004a9bb0(&rv, ori, normal));                // 00442CFC..00442D18
            copy3(bp(hit_info, 0x1C), hp);                                              // 00442D1B..00442D37
        }
        if (hit_out != 0) copy3(obj(hit_out), hp);                                      // 00442D3A..00442D59
    }
    return result;                                                                      // 00442D5E mov eax,ebp
}
// 004428F0 PartAABB::HitCheckGeom(start, end, hit, info, scale) RET 14h: publishes the AABB node and info
// (007A69B0/007A69B4) and the octant mask (007A8A24) for HitCheckAABBnode 00441570, walks the AABB tree root
// (node+19C); on a hit copies end into hit. EAX = HitCheckAABBnode's result.
U __fastcall part_aabb_hit_check_geom_004428f0(void* self, void*, U start_word, U end_word, U hit_word, U info, U scale_bits) {
    void* const node = get<void*>(self, 4);                                             // 004428F0
    const float* const s = static_cast<const float*>(obj(start_word));                  // ebx
    float* const e = static_cast<float*>(obj(end_word));                                // esi
    aurora_aabb_node_007a69b0 = node_if(node, 0x21);                                    // 004428F3..00442910
    aurora_aabb_hit_info_007a69b4 = info;                                               // 00442915..00442919
    U mask = 1u << ((s[0] < e[0]) ? 0 : 3);                                             // 00442900..00442936 (test ah,5; jp -> 3)
    mask |= 1u << ((s[1] < e[1]) ? 1 : 4);                                              // 0044292E..00442957
    mask |= 1u << ((s[2] < e[2]) ? 2 : 5);                                              // 0044294C..00442979
    aurora_aabb_direction_mask_007a8a24 = mask;                                         // 0044298A
    void* const aabb = (get<U16>(node, 0) & 0x221) == 0x221 ? node : nullptr;           // 0044297B..00442994
    const U r = hit_check_aabb_node_00441570(get<void*>(aabb, 0x19C),
                                             static_cast<const Vector*>(obj(start_word)), static_cast<Vector*>(obj(end_word)), scale_bits); // 00442973..0044299D
    if (r != 0 && hit_word != 0) copy3(obj(hit_word), e);                               // 004429A5..004429BE
    return r;                                                                           // EAX
}
// 00441510 PartAnimMesh::HitCheckGeom: forwards to AnimMeshHitCheck 00440CF0(part, anim-mesh node, start(v),
// end(v), hit, scale); the info argument is not used. EAX = its result.
U __fastcall part_anim_mesh_hit_check_geom_00441510(void* self, void*, U start_word, U end_word, U hit_word, U, U scale_bits) {
    void* const node = node_if(get<void*>(self, 4), 0xA1);                              // 00441510..00441523
    const Vector start = *static_cast<const Vector*>(obj(start_word));                  // 00441548..00441560 (by value)
    const Vector end = *static_cast<const Vector*>(obj(end_word));                      // 0044152E..00441545 (by value)
    return anim_mesh_hit_check_00440cf0(self, node, start, end, static_cast<Vector*>(obj(hit_word)), scale_bits); // 00441525..00441563
}
// 00440FF0 PartTriMesh::HitCheckGeom(start, end, hit, info, scale) RET 14h.
//  * slot 50 nonzero, saber node (word & 821h), or no vertices (+180) -> 0.
//  * info+14 != 0 (box mode): BoxIntersec 004AA1F0 against the node box (+64/+70, scaled) grown by the margin
//    vector 007A8A4C; face code r>0 -> info normal component +40[(r-1)/2] = (r-1) odd ? 1.0 : -1.0;
//    r == -1 -> restore end from the static 007A8A94 when info+58 != 0, else ClearOutput; info+1C = *hit.
//  * otherwise: BoxIntersect 004AA610 (scaled box); then every face (stride 20h, word corners +1A/+1C/+1E)
//    goes through polyhit 004AB750 (OR-accumulated); on a hit: ClearOutput, info+40 = the never-written
//    local {0,0,0} (observed: [esp+18h..20h] only zero-initialised), info+1C = info+28 = *hit = *end.
// EAX = BoxIntersec code / polyhit accumulator / 0.
U __fastcall part_tri_mesh_hit_check_geom_00440ff0(void* self, void*, U start_word, U end_word, U hit_word, U info, U scale_bits) {
    if (part_table(self)->s050(self, nullptr) != 0) return 0;                           // 00440FF5..00441007
    void* const node = get<void*>(get<void*>(self, 0x70), 4);                           // 0044100A..0044100F
    if ((get<U16>(node, 0) & 0x821) == 0x821) return 0;                                 // 00441012..00441022 -> 00441215
    if (get<U16>(node, 0x180) == 0) return 0;                                           // 00441028..00441031
    float scale; std::memcpy(&scale, &scale_bits, 4);
    const Vector* const start = static_cast<const Vector*>(obj(start_word));
    Vector* const end = static_cast<Vector*>(obj(end_word));                            // ebx
    Vector* const hit = static_cast<Vector*>(obj(hit_word));                            // ebp
    void* const hit_info = obj(info);
    const Vector* const box_min = static_cast<const Vector*>(static_cast<void*>(bp(node, 0x64)));
    const Vector* const box_max = static_cast<const Vector*>(static_cast<void*>(bp(node, 0x70)));
    const Vector* const margin = reinterpret_cast<const Vector*>(aurora_hit_box_margin_007a8a4c);
    const bool unit = (scale == kOne_0073d764);                                         // 00441079..004410B3 / 004411DD..004411F2 (test ah,44h; jp)
    if (get<U>(hit_info, 0x14) != 0) {                                                  // 00441037..00441045
        if ((aurora_trimesh_saved_end_guard_007a8aa0 & 1) == 0) {                       // 0044104B..00441058
            aurora_trimesh_saved_end_guard_007a8aa0 = aurora_trimesh_saved_end_guard_007a8aa0 | 1;   // 0044105A
            put<U>(aurora_trimesh_saved_end_007a8a94, 0, 0); put<U>(aurora_trimesh_saved_end_007a8a94, 4, 0); put<U>(aurora_trimesh_saved_end_007a8a94, 8, 0); // 00441060..0044106C
        }
        copy3(aurora_trimesh_saved_end_007a8a94, end);                                  // 0044108D..004410A8
        Vector t0, t1, t2, t3;
        const Vector* bmax; const Vector* bmin;
        if (unit) {
            bmax = vector_sum_00423b90(&t0, box_max, margin);                           // 004410B5..004410BE
            bmin = reinterpret_cast<const Vector*>(kotor_cdecl_00423bc0(&t1.x, &box_min->x, &margin->x)); // 004410C6..0044111B
        } else {
            bmax = vector_sum_00423b90(&t1, vector_component_mult_00423bf0(&t0, box_max, scale), margin);   // 004410D6..004410FA
            bmin = reinterpret_cast<const Vector*>(kotor_cdecl_00423bc0(&t3.x, &vector_component_mult_00423bf0(&t2, box_min, scale)->x, &margin->x)); // 004410FD..0044111B
        }
        const I r = box_intersec_004aa1f0(start, end, bmin, bmax, hit);                 // 00441120..0044112D
        if (r == 0) return 0;                                                           // 00441137..00441139 -> 004411D1 (EAX = esi = 0)
        if (r != -1) {                                                                  // 0044113F..00441142
            aurora_hit_info_clear_output_00456a60(hit_info, nullptr);                   // 00441144..0044114D
            const I i = r - 1;                                                          // 00441152
            const float sign = (i % 2 != 0) ? kOne_0073d764 : kMinusOne_00740cbc;       // 00441155..00441177 (and 80000001h; jns; dec/or/inc)
            put<float>(hit_info, static_cast<U>(0x40 + (i / 2) * 4), sign);             // 0044116C..00441182 cdq; sub; sar 1; fstp
        } else if (get<U>(hit_info, 0x58) != 0) {                                       // 00441188..00441194
            copy3(end, aurora_trimesh_saved_end_007a8a94);                              // 00441196..004411AC
        } else {
            aurora_hit_info_clear_output_00456a60(hit_info, nullptr);                   // 004411B1
        }
        copy3(bp(hit_info, 0x1C), hit);                                                 // 004411B6..004411CE
        return static_cast<U>(r);                                                       // 004411D3 mov eax,esi
    }
    I hits = 0;                                                                         // 00441041 [esp+10h] = 0
    {
        Vector t0, t1;
        const I inside = unit ? box_intersect_004aa610(start, end, box_min, box_max)    // 004411F4..00441209
                              : box_intersect_004aa610(start, end, vector_component_mult_00423bf0(&t1, box_min, scale),
                                                       vector_component_mult_00423bf0(&t0, box_max, scale));   // 00441221..0044125B
        if (inside == 0) return 0;                                                      // 00441211..00441213 / 00441263..00441265
    }
    const U verts = get<U>(node, 0x198);                                                // 0044126E esi
    float hit_normal[3]; put<U>(hit_normal, 0, 0); put<U>(hit_normal, 4, 0); put<U>(hit_normal, 8, 0);   // 0044127A..0044128A (never written after)
    const I faces = get<I>(node, 0x5C);                                                 // 00441297
    U face_off = 0;                                                                     // 004412A8 / 0044137A [esp+14h]
    if (faces > 0) for (I f = 0;;) {                                                    // 004412A0..004412A2 / 0044135D..00441367 (count reloaded)
        Vector tri[3];
        vector_constructor_iterator_00401130(tri, 0xC, 3, &vector_default_construct_00409e70);   // 004412B0..004412BE / 00441382..00441390
        unsigned char* const face = static_cast<unsigned char*>(obj(get<U>(node, 0x58))) + face_off; // 004412C3..004412CA
        for (int k = 0; k < 3; ++k) {
            const I16 index = get<I16>(face, static_cast<U>(0x1A + 2 * k));             // 004412CD / 004412EC / 0044130A (movsx)
            const float* const v = static_cast<const float*>(obj(verts + static_cast<U>(index) * 12u));
            if (unit) copy3(&tri[k], v);                                                // 004412D4..00441331 (raw copies)
            else {
                tri[k].x = f32(W(scale) * W(v[0]));                                     // 00441395..004413B4 / 004413D8.. / 0044141A..
                tri[k].y = f32(W(scale) * W(v[1]));
                tri[k].z = f32(W(scale) * W(v[2]));
            }
        }
        hits |= polyhit_004ab750(tri, 3, face, face + 0xC, start, end, end);           // 0044131D..00441344 / 004413B0..0044145A
        face_off += 0x20;                                                               // 0044135E / 00441474
        ++f;                                                                            // 0044135D / 00441473 inc ebp
        if (!(f < get<I>(node, 0x5C))) break;                                           // 0044134F / 00441465 (size reloaded)
    }
    if (hits != 0 && hit != nullptr) {                                                  // 00441483..00441494
        aurora_hit_info_clear_output_00456a60(hit_info, nullptr);                       // 00441496..0044149F
        copy3(bp(hit_info, 0x40), hit_normal);                                          // 004414A4..004414B8
        copy3(bp(hit_info, 0x1C), end);                                                 // 004414BB..004414CD
        copy3(bp(hit_info, 0x28), end);                                                 // 004414D0..004414E2
        copy3(hit, end);                                                                // 004414E5..004414F2
    }
    return static_cast<U>(hits);                                                        // 004414F5
}
}  // extern "C"

// ================================================================ PartLightsaber::Animate
// 00481AE0 (dt) RET 4. Ages the 20-slot blade trail (+98 floats, +FC Vector[20], +228 Quaternion[20];
// words +3B8 head, +3BA tail, +3BC count) and appends the current world pose when it moved. Publishes the
// count to 008302DC. EAX = movsx count (00481D0F) or the zero-extended count (00481D25/00481D3C paths).
extern "C" U __fastcall part_lightsaber_animate_00481ae0(void* self, void*, U dt_bits) {
    float dt; std::memcpy(&dt, &dt_bits, 4);
    float min_step; { const U b = aurora_saber_min_step_00798b34; std::memcpy(&min_step, &b, 4); }
    if (!(dt > min_step)) dt = min_step;                                               // 00481AE0..00481AFF test ah,41h; je; else mov [esp+3Ch],[00798B34] (raw DWORD)
    void* const scene = *kotor_scene3d::cells.active_scene_007b92a0;                    // 00481B03
    I16 di = get<I16>(self, 0x3BA);                                                     // 00481B09
    I16 removed = 0;                                                                    // 00481B12 ebx
    if (scene != nullptr &&
        static_cast<const kotor_scene3d::SceneVTable*>(get<const void*>(scene, 0))->s0a8_scene_is_animating(scene, nullptr) != 0) { // 00481B14..00481B26
        for (I16 i = 0; i < get<I16>(self, 0x3BC); ++i) {                               // 00481B2C..00481B35 / 00481BA0..00481BA8
            float* const age = static_cast<float*>(static_cast<void*>(bp(self, static_cast<U>(0x98 + static_cast<I>(di) * 4))));
            const double sum = W(dt) + W(*age);                                         // 00481B40..00481B47
            *age = f32(sum);                                                            // 00481B55 fst dword (ST keeps the wide sum)
            if (sum > W(*kotor_scene3d::cells.saber_max_accum_00798b30)) {              // 00481B57..00481B62 test ah,41h; jne
                if (static_cast<I>(get<I16>(self, 0x3BC)) - static_cast<I>(removed) > 1) {   // 00481B64..00481B73
                    ++removed;                                                          // 00481B75
                    put<I16>(self, 0x3BA, static_cast<I16>(get<I16>(self, 0x3BA) + 1)); // 00481B76
                    if (get<I16>(self, 0x3BA) == kSaberSlots_00740a4c) put<I16>(self, 0x3BA, 0);   // 00481B7D..00481B8D
                }
            }
            di = static_cast<I16>(di + 1);                                              // 00481B94
            if (di == kSaberSlots_00740a4c) di = 0;                                     // 00481B95..00481B9E
        }
        put<I16>(self, 0x3BC, static_cast<I16>(get<I16>(self, 0x3BC) - removed));       // 00481BAA
    }
    float pos[3]; put<U>(pos, 0, 0); put<U>(pos, 4, 0); put<U>(pos, 8, 0);              // 00481BBF..00481BCF
    U ori[4]; ori[3] = 0; ori[2] = 0; ori[1] = 0; ori[0] = 0x3F800000u;                 // 00481BD7..00481BEF
    part_table(self)->s00c(self, nullptr, addr(pos), addr(ori));                        // 00481BB1..00481BF7 call [edx+0Ch]
    float* const last = static_cast<float*>(static_cast<void*>(bp(self, 0x3C0)));       // 00481BFA
    if (aurora_vector_equal_004aa980(pos, last) != 0) {                                 // 00481C00..00481C10
        const I16 c = get<I16>(self, 0x3BC);
        aurora_saber_count_008302dc = c;                                                // 00481D0F..00481D19
        return static_cast<U>(static_cast<I>(c));
    }
    Vector d;
    d.x = f32(W(pos[0]) - W(last[0]));                                                  // 00481C16..00481C21
    d.y = f32(W(pos[1]) - W(last[1]));                                                  // 00481C25..00481C2C
    d.z = f32(W(pos[2]) - W(last[2]));                                                  // 00481C30..00481C37
    if (vector_magnitude_004ab100(&d) > W(kOne_0073d764)) {                             // 00481C3B..00481C4E test ah,41h; jne
        put<U16>(self, 0x3B8, 0); put<U16>(self, 0x3BA, 0); put<U16>(self, 0x3BC, 0);   // 00481C50..00481C5E
    }
    copy3(last, pos);                                                                   // 00481C65..00481C7D
    const I head = get<I16>(self, 0x3B8);                                               // 00481C73
    put<U>(self, static_cast<U>(0x98 + head * 4), 0);                                   // 00481C80
    copy3(bp(self, static_cast<U>((get<I16>(self, 0x3B8) + 0x15) * 12)), pos);          // 00481C87..00481CA8
    std::memcpy(bp(self, static_cast<U>(get<I16>(self, 0x3B8) * 16 + 0x228)), ori, 16); // 00481CAB..00481CD4
    const U count = get<U16>(self, 0x3BC);                                              // 00481CD7..00481CD9
    if (count == static_cast<U>(static_cast<U16>(kSaberSlots_00740a4c))) {              // 00481CE0..00481CE7
        aurora_saber_count_008302dc = get<I16>(self, 0x3BC);                            // 00481D3C..00481D46
        return count;                                                                   // EAX = 14h (zero-extended)
    }
    put<I16>(self, 0x3B8, static_cast<I16>(get<I16>(self, 0x3B8) + 1));                 // 00481CE9
    put<U16>(self, 0x3BC, static_cast<U16>(count + 1));                                 // 00481CF7..00481CF8
    if (get<I16>(self, 0x3B8) < kSaberSlots_00740a4c) {                                 // 00481CF0..00481D06
        aurora_saber_count_008302dc = get<I16>(self, 0x3BC);                            // 00481D25..00481D2F
        return count + 1;                                                               // EAX = zero-extended new count
    }
    put<U16>(self, 0x3B8, 0);                                                           // 00481D08
    const I16 c = get<I16>(self, 0x3BC);
    aurora_saber_count_008302dc = c;                                                    // 00481D0F..00481D19
    return static_cast<U>(static_cast<I>(c));
}
