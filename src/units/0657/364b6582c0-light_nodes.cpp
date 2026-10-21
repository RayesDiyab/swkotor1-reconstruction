// lane-3d-construct unit light_nodes: MdlNodeLight destruction/control and static tile lighting
// (LightNodeTile 0046DB60, LightPartTriMesh 0046A9E0). Receivers use lane-3d-model's aurora_model types
// (model_load.hpp) and the canonical scene3d.h math providers.
// Evidence: dumpbin /disasm swkotor.exe (GOG K1); lane-local listings d/<ADDR>.txt (0046A9E0 also d/0046A9E0.s).
// Float policy (D30-FINAL): x87 PC_53 => double intermediates; float only where the original stores with `fst/fstp dword`.
// Values that stay on the x87 stack across a `fst dword` (004AEC2 k, 0046ADC7 distance, 0046ADF0 attenuation) are
// kept as the unrounded double for the following x87 operations and as the rounded float for the later memory reads.
#include "6c81d8874d-scene3d.h"            // proposed canonical copy (source/proposed)
#include "5d11c14380-model_load.hpp"       // lane-3d-model: MdlNode, pools, AsModel/AsTriMesh, SafePointer dtor, eh vector iterator
#include "516b075ab1-construct3d.h"
#include <cstdint>

extern "C" {
// lane-3d-model cell (proposed storage owner lane-3d-model: its other writers 0043FD5D/0043FDF8 are in that lane)
extern volatile aurora_model::I aurora_node_count_007a8a08;
// lane-startup-misc texture_memory.cpp (extern "C"): CExoArrayList<T*>::operator= 0046BC80 (RET 4, EAX = this)
kotor_reconstruction::ArrayMutations* __fastcall texture_list_assign_0046bc80(kotor_reconstruction::ArrayMutations* self, void*,
                                                                              const kotor_reconstruction::ArrayMutations* source);
}

namespace {
using kotor_scene3d::Vector; using kotor_scene3d::Quaternion; using kotor_scene3d::field;
using kotor_scene3d::bits_to_float; using kotor_scene3d::float_to_bits;
using U = std::uint32_t; using I = std::int32_t; using U8 = std::uint8_t; using U16 = std::uint16_t; using I16 = std::int16_t;
namespace C3 = kotor_scene3d_construct;
using C3::List; using C3::PartSlots; using C3::slots_of;
using aurora_model::MdlNode;

inline void* element(U data, I index) {
    return *reinterpret_cast<void* const volatile*>(static_cast<std::uintptr_t>(data) + static_cast<U>(index) * 4u);
}
inline void set_element(U data, I index, void* value) {
    *reinterpret_cast<void* volatile*>(static_cast<std::uintptr_t>(data) + static_cast<U>(index) * 4u) = value;
}
inline void* address(U value) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(value)); }
inline U word_of(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline float f(U bits) { return bits_to_float(bits); }
// byte-offset view (kotor_scene3d::at; named locally because aurora_model::at is found by ADL on MdlNode*)
inline void* atp(void* object, U offset) { return kotor_scene3d::at(object, offset); }
inline List* list_at(void* object, U offset) { return static_cast<List*>(atp(object, offset)); }

// Inlined CExoArrayList<SafePointer*>::RemoveReverse (00448022..00448062 / 0044A522..0044A562) on the registration list
// of the texture held by `cell`: search from the end for `cell`, then size = size-1 UNCONDITIONALLY, then shift the
// tail down from the found index (index -1 when absent: the original then shifts from element -1, i.e. it writes the
// word before the buffer - reproduced, not corrected).
inline void texture_unregister(font::Texture* texture, void* cell) {
    List& registrations = texture->registrations;                    // texture+4 data, +8 size
    const I last = registrations.size - 1;                            // 00448022..00448025 lea edi,[eax-1]
    I k = last;
    while (k >= 0) {                                                  // 0044802A..0044803E
        if (element(registrations.data, k) == cell) break;            // 00448034 cmp [edx],esi
        --k;
    }
    registrations.size = last;                                        // 00448042 mov [ecx+8],edi
    if (k < last) {                                                   // 00448040..00448045 cmp eax,edi; jge
        do {
            const U data = registrations.data;                        // 00448050 (re-read)
            set_element(data, k, element(data, k + 1));               // 00448053..0044805A
            ++k;
        } while (k < registrations.size);                             // 0044805C..00448062 (size re-read)
    }
}

// The lens-flare texture SafePointer array of an MdlNodeLight: +54 data (new[] block, count at data-4), +58 count.
inline void release_light_textures(MdlNode* node) {
    I n = field<I>(node, 0x58);                                       // 00448005 / 0044A506
    while (n != 0) {                                                  // 0044800B / 0044A50E
        --n;                                                          // 00448013
        void* const cell = static_cast<unsigned char*>(address(field<U>(node, 0x54))) + static_cast<U>(n) * 4u; // 00448010..00448019
        font::Texture* const texture = *static_cast<font::Texture* volatile*>(cell); // 00448014 / 0044801C spill
        if (texture != nullptr) texture_unregister(texture, cell);    // 00448017..00448062
        *static_cast<font::Texture* volatile*>(cell) = nullptr;      // 00448069 mov dword [esi],0
        (void)font::release_00420d70(texture);                        // 00448064..0044806F AurTextureRelease(old target)
        field<I>(node, 0x58) = field<I>(node, 0x58) - 1;              // 00448074..0044807D
    }
}
// `eh vector destructor iterator`(data, 4, count, SafePointer<CAurTexture>::~SafePointer) then free(data-4)
// (00449048..0044905D / 0044A587..0044A59C) via the strict-link provider c2da_destroy_array_006fa852 (lane-3d-model spelling).
inline void destroy_texture_array(U data) {
    const I count = *static_cast<const I*>(address(data - 4u));       // 00449048 mov ecx,[eax-4]
    c2da_destroy_array_006fa852(address(data), 4u, count, &aurora_texture_safe_pointer_destroy_00422380);
    live_free(address(data - 4u));                                    // 0044905C..0044905D lea edi,[eax-4]; free
}
} // namespace

extern "C" {
// 00448000 MdlNodeLight::InternalDestructor (thiscall, RET 0)
void __fastcall aurora_light_node_internal_destructor_00448000(MdlNode* self, void*) {
    release_light_textures(self);                                     // 00448005..00448080
    I children = field<I>(self, 0x30);                                // 00448083 mov esi,[ebx+30h]
    while (children != 0) {                                           // 00448086..0044809F
        MdlNode* const child = static_cast<MdlNode*>(element(field<U>(self, 0x2C), children - 1)); // 00448090..00448093 (data re-read)
        --children;                                                   // 00448097
        aurora_node_destructor_dispatch_004488f0(child, nullptr);     // 00448098 MdlNode::DestructorDispatch
    }
    aurora_node_count_007a8a08 = aurora_node_count_007a8a08 - 1;      // 004480A1..004480A9 dec [007A8A08]
}

// 00448F80 MdlNodeLight::~MdlNodeLight (thiscall, RET 0). EH FuncInfo 00716092 states 5..0 (member list dtors; the
// original only frees, nothing throws - not emitted, D30-FINAL).
void __fastcall aurora_light_node_destruct_00448f80(MdlNode* self, void*) {
    if (field<U16>(self, 0) != 0)                                     // 00448F9E cmp word [esi],0
        aurora_node_destructor_dispatch_004488f0(self, nullptr);     // 00448FAC
    I blocks = field<I>(self, 0x88);                                  // 00448FB1
    field<U16>(self, 0) = 0;                                          // 00448FB9 mov word [esi],0
    while (blocks != 0) {                                             // 00448FBE..00448FE3
        void* const block = element(field<U>(self, 0x84), blocks - 1); // 00448FC0..00448FC6
        --blocks;                                                     // 00448FCA
        live_free(block);                                             // 00448FCB..00448FCC
        field<I>(self, 0x88) = field<I>(self, 0x88) - 1;              // 00448FD1..00448FDD
    }
    if (field<U>(self, 0x84) != 0) live_free(address(field<U>(self, 0x84))); // 00448FE5..00448FF5
    if (field<U>(self, 0x78) != 0) live_free(address(field<U>(self, 0x78))); // 00448FFD..0044900A
    if (field<U>(self, 0x6C) != 0) live_free(address(field<U>(self, 0x6C))); // 00449012..0044901F
    if (field<U>(self, 0x60) != 0) live_free(address(field<U>(self, 0x60))); // 00449027..00449034
    if (field<U>(self, 0x54) != 0) destroy_texture_array(field<U>(self, 0x54)); // 0044903C..0044905D (+54 not cleared)
    aurora_node_destruct_0044a040(self, nullptr);                     // 00449065..0044906F MdlNode::~MdlNode
}

// 0044A500 MdlNodeLight::BinaryInternalDestructor (thiscall, RET 0)
void __fastcall aurora_light_node_binary_internal_destructor_0044a500(MdlNode* self, void*) {
    release_light_textures(self);                                     // 0044A506..0044A57E
    if (field<U>(self, 0x54) != 0) {                                  // 0044A580..0044A585
        destroy_texture_array(field<U>(self, 0x54));                  // 0044A587..0044A59C
        field<U>(self, 0x54) = 0;                                     // 0044A5A4
    }
    field<U>(self, 0x5C) = 0;                                         // 0044A5A8
    field<U>(self, 0x58) = 0;                                         // 0044A5AB
}

// 00485560 MdlNodeLight::InternalControl(part, t0, t1, weight) (thiscall, RET 10h): part slot +24 (result unused),
// then MdlNode::InternalControl 004854D0 with the same three stack words (0048556F..00485581; EAX passed through,
// unused by the dispatcher 0043F890 - lane-3d-anim declares the family void).
void __fastcall node_light_internal_control_00485560(void* node, void*, void* part, float t0, float t1, float weight) {
    (void)slots_of<PartSlots>(part)->as_light(static_cast<aurora_model::Part*>(part), nullptr); // 00485561..0048556C call [eax+24h]
    node_internal_control_004854d0(node, nullptr, part, t0, t1, weight);                      // 0048556F..00485581
}

// 0046DB60 LightNodeTile(room, scene) (cdecl). For every trimesh part of the room model: collect the scene lights
// (scene+80 -> +20 list, copied with operator=) whose sphere reaches the part's bounding sphere, then light its
// vertices (0046A9E0, list passed by value as a fresh copy constructed by 0046D250). EH FuncInfo 00716E60 (two lists).
void __cdecl light_node_tile_0046db60(void* room, void* scene) {
    void* const room_object = field<void*>(room, 0x58);              // 0046DB78..0046DB7E
    if (field<void*>(room_object, 0x70) == nullptr) return;          // 0046DB81..0046DB8E
    List tile_lights; tile_lights.size = 0; tile_lights.capacity = 0; tile_lights.data = 0; // 0046DB94..0046DB9E ([esp+20h])
    List scene_lights; scene_lights.size = 0; scene_lights.capacity = 0; scene_lights.data = 0; // 0046DBA6..0046DBAE ([esp+2Ch])
    (void)texture_list_assign_0046bc80(&scene_lights, nullptr,
        static_cast<const List*>(atp(field<void*>(scene, 0x80), 0x20)));   // 0046DBB2..0046DBC9
    I part_index = 0;                                                 // 0046DBD4 (spilled to the room argument cell)
    if (field<I>(field<void*>(room_object, 0x70), 0x1C) > 0) {        // 0046DBCE..0046DBDC
        do {
            void* const holder = field<void*>(room_object, 0x70);
            void* const part = element(field<U>(holder, 0x18), part_index); // 0046DBE2..0046DBE5
            void* const trimesh = static_cast<void*>(slots_of<PartSlots>(part)->as_trimesh(static_cast<aurora_model::Part*>(part), nullptr)); // 0046DBE8..0046DBEA
            if (trimesh != nullptr) {                                 // 0046DBEF..0046DBF1
                I count = 0;                                          // 0046DBFA..0046DBFC tile_lights.size = 0
                tile_lights.size = 0;
                MdlNode* const mesh = aurora_node_as_trimesh_0043e400(field<MdlNode*>(trimesh, 0x04), nullptr); // 0046DBF7..0046DC05
                Vector rotated;
                const Vector* const offset = quaternion_rotate_004a9bb0(&rotated,
                    static_cast<const Quaternion*>(atp(trimesh, 0x14)), static_cast<const Vector*>(atp(mesh, 0x80))); // 0046DC09..0046DC18
                Vector centre;                                        // 0046DC1D..0046DC3F (three fadd/fstp dword)
                centre.z = static_cast<float>(static_cast<double>(field<float>(trimesh, 0x10)) + static_cast<double>(offset->z));
                centre.y = static_cast<float>(static_cast<double>(field<float>(trimesh, 0x0C)) + static_cast<double>(offset->y));
                centre.x = static_cast<float>(static_cast<double>(field<float>(trimesh, 0x08)) + static_cast<double>(offset->x));
                for (I j = 0; j < scene_lights.size; ++j) {           // 0046DC39..0046DC43 / 0046DD0A..0046DD11
                    void* const light = element(scene_lights.data, j); // 0046DC50..0046DC54
                    if (field<U>(light, 0x70) != 0) continue;         // 0046DC57..0046DC5C
                    Vector zero; zero.x = 0.0f; zero.y = 0.0f; zero.z = 0.0f; // 0046DC6B..0046DC7B
                    if (aurora_vector_equal_004aa980(atp(light, 0x4C), &zero) != 0) continue; // 0046DC62..0046DC8D
                    Vector delta;                                     // 0046DC8F..0046DCBA
                    delta.x = static_cast<float>(static_cast<double>(field<float>(light, 0x94)) - static_cast<double>(centre.x));
                    delta.y = static_cast<float>(static_cast<double>(field<float>(light, 0x98)) - static_cast<double>(centre.y));
                    delta.z = static_cast<float>(static_cast<double>(field<float>(light, 0x9C)) - static_cast<double>(centre.z));
                    const double distance = vector_magnitude_004ab100(&delta);  // 0046DCBE
                    const double reach = static_cast<double>(field<float>(light, 0x58)) +
                                         static_cast<double>(field<float>(mesh, 0x7C)); // 0046DCC3..0046DCCA
                    // 0046DCD0..0046DCD9 fxch; fcompp; test ah,5; jp: keep only when distance < reach (C0 alone).
                    if (!(distance < reach)) continue;
                    const I capacity = tile_lights.capacity;          // 0046DCDB
                    if (count == capacity) {                          // 0046DCDF..0046DCE1
                        menu_list_growth::allocate_005e03c0(&tile_lights, nullptr,
                            static_cast<U>(capacity != 0 ? capacity + capacity : 8)); // 0046DCE3..0046DCF5
                        count = tile_lights.size;                     // 0046DCFA
                    }
                    set_element(tile_lights.data, count, light);      // 0046DCFE..0046DD02
                    ++count; tile_lights.size = count;                // 0046DD05..0046DD06
                }
                // 0046DD1D..0046DD42: push scene; construct the by-value list in the argument area from tile_lights
                // (0046D250 deep copy); push trimesh; call LightPartTriMesh (the callee owns and frees the copy).
                List argument;
                (void)int_list_copy_construct_0046d250(&argument, nullptr, &tile_lights);
                light_part_trimesh_0046a9e0(trimesh, argument, scene);
            }
            ++part_index;                                             // 0046DD4B
        } while (part_index < field<I>(field<void*>(room_object, 0x70), 0x1C)); // 0046DD45..0046DD52 (re-read)
    }
    if (scene_lights.data != 0) live_free(address(scene_lights.data)); // 0046DD5C..0046DD6A
    if (tile_lights.data != 0) live_free(address(tile_lights.data));   // 0046DD72..0046DD7F
}

// 0046A9E0 LightPartTriMesh(trimesh, lights (CExoArrayList by value, owned), scene) (cdecl): computes a static
// per-vertex colour (ARGB words) into the part's vertex pool. EH FuncInfo 00716CC8 frees the by-value list on unwind.
// The local Matrix constructed at 0046AAA6 (Matrix::Matrix 00437AF0 on [esp+FCh]) is never read afterwards and has no
// effect outside the frame; it is not reproduced.
void __cdecl light_part_trimesh_0046a9e0(void* trimesh, List lights, void* scene) {
    (void)scene;                                                      // stack word 5 is never read (0046A9E0..0046B1D5)
    static Vector accumulator_00827f58;                               // function-local static (guard 00827F64 bit 0)
    static U guard_00827f64;
    const U list_data = lights.data;                                  // [esp+134h]
    auto finish = [&]() { if (list_data != 0) live_free(address(list_data)); }; // 0046AA20..0046AA3B / 0046B1A8..0046B1B6
    if (trimesh == nullptr) { finish(); return; }                     // 0046AA06..0046AA0F
    {
        void* const cache = field<void*>(trimesh, 0x80);              // 0046AA11..0046AA19
        if (cache != nullptr && field<U>(cache, 0x34) != 0) { finish(); return; } // 0046AA1B..0046AA1E
    }
    void* const vertices = field<void*>(trimesh, 0x70);               // 0046AAAB
    MdlNode* const node = field<MdlNode*>(vertices, 0x04);            // 0046AAAE
    if (field<U16>(node, 0x180) == 0) { finish(); return; }           // 0046AAB1..0046AAC3 (vertex count word)
    if (lights.size == 0) {                                           // 0046AAC8..0046AACF
        Vector zero; zero.x = 0.0f; zero.y = 0.0f; zero.z = 0.0f;    // 0046AADA..0046AAEA
        if (aurora_vector_equal_004aa980(atp(trimesh, 0x64), &zero) != 0) {   // 0046AAD1..0046AAFC
            if (field<U8>(trimesh, 0x7C) == 1) {                      // 0046AAFE..0046AB02
                (void)aurora_pool_delete_00423f50(field<U>(trimesh, 0x74)); // 0046AB04..0046AB08
                field<U>(trimesh, 0x74) = 0xFFFFFFFFu;                // 0046AB0D
                field<U8>(trimesh, 0x7C) = 0;                         // 0046AB14
                void* const owner = field<void*>(trimesh, 0x70);      // 0046AB18
                slots_of<C3::VertexSlots>(owner)->invalidate(owner, nullptr); // 0046AB1B..0046AB23 call [eax+28h]
            }
            finish(); return;                                         // 0046AB26..0046AB2F
        }
    }
    if (field<U8>(trimesh, 0x7C) == 0) {                              // 0046AB34..0046AB39
        const U16 vertex_count = field<U16>(node, 0x180);             // 0046AB3B
        if (vertex_count == 0) { finish(); return; }                  // 0046AB42..0046AB45
        (void)aurora_pool_request_00423ee0(0u, static_cast<U>(static_cast<I>(static_cast<I16>(vertex_count))) * 4u + 4u,
                                           0x42u, 0u, static_cast<U*>(atp(trimesh, 0x74))); // 0046AB47..0046AB5A (movsx; lea [eax*4+4])
        field<U8>(trimesh, 0x7C) = 1;                                 // 0046AB62
        void* const owner = field<void*>(trimesh, 0x70);              // 0046AB5F
        slots_of<C3::VertexSlots>(owner)->invalidate(owner, nullptr); // 0046AB66..0046AB6B
    }
    Vector position; position.x = 0.0f; position.y = 0.0f; position.z = 0.0f;            // 0046AA62..0046AA72 ([esp+54h])
    Quaternion orientation; orientation.z = 0.0f; orientation.y = 0.0f; orientation.x = 0.0f; orientation.w = 1.0f; // 0046AA7A..0046AA9B
    slots_of<PartSlots>(trimesh)->get_transform(static_cast<aurora_model::Part*>(trimesh), nullptr, &position, &orientation); // 0046AB6E..0046AB81
    U* const colours = static_cast<U*>(aurora_pool_lock_00423f10(field<U>(trimesh, 0x74), 0u, 0u, 1u)); // 0046AB84..0046AB97
    aurora_model::MaxTree* const model = aurora_tree_as_model_0043e1c0(field<aurora_model::MaxTree*>(node, 0x08), nullptr); // 0046AB91..0046AB9E
    const U mdx = word_of(aurora_pool_lock_00423f10(field<U>(model, 0xAC), 0u, 0u, 0x10u)); // 0046ABA3..0046ABC4
    const U mdx_offset = field<U>(node, 0x194);                       // 0046ABB3 / [esp+34h]
    if (!(field<I16>(node, 0x180) > 0)) goto unlock;                  // 0046ABBC..0046ABD2 cmp word,0; jle
    {
        Vector zero_init; zero_init.x = 0.0f; zero_init.y = 0.0f; zero_init.z = 0.0f; // 0046ABD8..0046ABEB ([esp+7Ch])
        I i = 0;
        do {
            const U row = field<U>(node, 0x14C) * static_cast<U>(i);  // 0046AC00..0046AC0C imul (stride)
            const U vertex_at = field<U>(node, 0x154) + row + mdx_offset + mdx;           // 0046AC06..0046AC13
            Vector vertex;                                            // 0046AC15..0046AC33 ([esp+C4h])
            vertex.x = *static_cast<const float*>(address(vertex_at));
            vertex.y = *static_cast<const float*>(address(vertex_at + 4u));
            vertex.z = *static_cast<const float*>(address(vertex_at + 8u));
            const U normal_at = field<U>(node, 0x158) + row + mdx_offset + mdx;           // 0046AC2B..0046AC47
            Vector normal;                                            // 0046AC3C..0046AC66 ([esp+A4h])
            normal.x = *static_cast<const float*>(address(normal_at));
            normal.y = *static_cast<const float*>(address(normal_at + 4u));
            normal.z = *static_cast<const float*>(address(normal_at + 8u));
            Vector rotated_vertex;
            const Vector* const rv = quaternion_rotate_004a9bb0(&rotated_vertex, &orientation, &vertex); // 0046AC57..0046AC76
            Vector world;                                             // 0046AC7B..0046ACAF (fadd; fstp dword)
            world.z = static_cast<float>(static_cast<double>(position.z) + static_cast<double>(rv->z));
            world.y = static_cast<float>(static_cast<double>(position.y) + static_cast<double>(rv->y));
            world.x = static_cast<float>(static_cast<double>(position.x) + static_cast<double>(rv->x));
            Vector rotated_normal;
            const Vector* const rn = quaternion_rotate_004a9bb0(&rotated_normal, &orientation, &normal); // 0046AC82..0046ACB6
            if ((guard_00827f64 & 1u) == 0) guard_00827f64 |= 1u;     // 0046ACBB..0046ACC7 (trivial static init)
            accumulator_00827f58 = zero_init;                         // 0046ACCE..0046ACEB (three DWORD copies)
            if (field<U8>(field<void*>(trimesh, 0x44), 0x40) != 0) {  // 0046ACF1..0046ACF9
                accumulator_00827f58.x = f(0x3F800000u);              // 0046ACFB..0046AD2A (1,1,1 through a temporary)
                accumulator_00827f58.y = f(0x3F800000u);
                accumulator_00827f58.z = f(0x3F800000u);
            } else {
                accumulator_00827f58 = field<Vector>(trimesh, 0x64);  // 0046AD32..0046AD49 (three DWORD copies)
            }
            for (I j = 0; j < lights.size; ++j) {                     // 0046AD4E..0046AD5D / 0046AF7A..0046AF8C (size re-read)
                void* const light = element(lights.data, j);          // 0046AD63..0046AD6A
                void* const light_node = field<void*>(light, 0x04);   // 0046AD70 mov ebx,[esi+4]
                // 0046AD6D..0046AD7E: fcomp [0073D700] 0.0; test ah,44h; jnp -> skip only when radius == 0.
                if (field<float>(light, 0x58) == 0.0f) continue;
                Vector delta;                                         // 0046AD84..0046ADBB
                delta.x = static_cast<float>(static_cast<double>(field<float>(light, 0x94)) - static_cast<double>(world.x));
                delta.y = static_cast<float>(static_cast<double>(field<float>(light, 0x98)) - static_cast<double>(world.y));
                delta.z = static_cast<float>(static_cast<double>(field<float>(light, 0x9C)) - static_cast<double>(world.z));
                const double magnitude = vector_magnitude_004ab100(&delta);                 // 0046ADC2
                float distance = static_cast<float>(magnitude);       // 0046ADC7 fst dword [esp+14h]
                // 0046ADCE..0046ADDB: fcomp 0.0 on the UNROUNDED value; equal -> distance = 1e-6f (358637BDh).
                if (magnitude == 0.0) distance = f(0x358637BDu);
                // 0046ADE3..0046ADFF: attenuation = 1.0f - distance / radius (double); `test ah,5; jnp` skips when < 0.
                const double attenuation = static_cast<double>(f(0x3F800000u)) -
                                           static_cast<double>(distance) / static_cast<double>(field<float>(light, 0x58));
                const float attenuation_f = static_cast<float>(attenuation);                // 0046ADF0 fst dword [esp+48h]
                if (attenuation < 0.0) continue;
                const U light_flag = field<U>(light_node, 0x94);      // 0046AE0B mov ecx,[ebx+94h]
                const double dz = static_cast<double>(field<float>(light, 0x9C)) - static_cast<double>(world.z); // 0046AE05..0046AE13
                const double dy = static_cast<double>(field<float>(light, 0x98)) - static_cast<double>(world.y); // 0046AE17..0046AE1D
                const float dx = static_cast<float>(static_cast<double>(field<float>(light, 0x94)) - static_cast<double>(world.x)); // 0046AE21..0046AE2E fstp
                const float inverse = static_cast<float>(static_cast<double>(f(0x3F800000u)) / static_cast<double>(distance)); // 0046AE35..0046AE3F
                const float lz = static_cast<float>(dz * static_cast<double>(inverse));     // 0046AE43..0046AE49
                const float ly = static_cast<float>(dy * static_cast<double>(inverse));     // 0046AE4D..0046AE51
                const float lx = static_cast<float>(static_cast<double>(dx) * static_cast<double>(inverse)); // 0046AE58..0046AE63
                double factor = static_cast<double>(f(0x3F800000u));  // 0046AE6A fld 1.0
                const void* material;
                if (light_flag != 0) {                                // 0046AE70 jne 0046AEB4
                    material = atp(node, 0x98);                        // 0046AEB4
                } else {
                    // 0046AE72..0046AE9D: dot = (rn.y*ly + rn.z*lz) + lx*rn.x; `fcom 0; test ah,5; jnp` drops it when < 0.
                    const double dot = (static_cast<double>(rn->y) * static_cast<double>(ly) +
                                        static_cast<double>(rn->z) * static_cast<double>(lz)) +
                                       static_cast<double>(lx) * static_cast<double>(rn->x);
                    if (dot < 0.0) continue;                          // 0046AEAA -> 0046AF78 fstp st(0)
                    factor = dot;
                    material = atp(node, 0x8C);                        // 0046AEBC
                }
                const double k = factor * static_cast<double>(attenuation_f); // 0046AEC2 fmul [esp+48h]
                const float k_f = static_cast<float>(k);              // 0046AECE fst dword [esp+10h]
                const Vector m = *static_cast<const Vector*>(material); // 0046AEC6..0046AEE4 (three DWORD copies)
                Vector c;                                             // 0046AED2..0046AF0B
                c.z = static_cast<float>(k * static_cast<double>(field<float>(light, 0x54)));          // ST0 k (unrounded)
                c.y = static_cast<float>(static_cast<double>(k_f) * static_cast<double>(field<float>(light, 0x50)));
                c.x = static_cast<float>(static_cast<double>(k_f) * static_cast<double>(field<float>(light, 0x4C)));
                const float tx = static_cast<float>(static_cast<double>(c.x) * static_cast<double>(m.x)); // 0046AF0F..0046AF1B
                const float ty = static_cast<float>(static_cast<double>(c.y) * static_cast<double>(m.y)); // 0046AF1F..0046AF27
                const double tz = static_cast<double>(c.z) * static_cast<double>(m.z);                    // 0046AF2B..0046AF2F (not stored)
                const float sz = static_cast<float>(tz + static_cast<double>(accumulator_00827f58.z));    // 0046AF33
                const float sy = static_cast<float>(static_cast<double>(ty) + static_cast<double>(accumulator_00827f58.y)); // 0046AF39..0046AF3D
                const float sx = static_cast<float>(static_cast<double>(tx) + static_cast<double>(accumulator_00827f58.x)); // 0046AF43..0046AF47
                accumulator_00827f58.x = sx;                          // 0046AF4D..0046AF55
                accumulator_00827f58.y = sy;                          // 0046AF5B..0046AF63
                accumulator_00827f58.z = sz;                          // 0046AF69..0046AF71
            }
            // 0046AF9A..0046B00C: modulate by the vertex colour bytes when present (offset +15C != -1).
            if (field<U>(node, 0x15C) != 0xFFFFFFFFu) {
                const U colour_at = field<U>(node, 0x14C) * static_cast<U>(i) + field<U>(node, 0x15C) + mdx_offset + mdx;
                const float k255 = f(0x3B808081u);                    // [0073EDE0] 1/255
                const U8 b0 = *static_cast<const U8*>(address(colour_at));
                accumulator_00827f58.x = static_cast<float>(static_cast<double>(static_cast<I>(b0)) * static_cast<double>(accumulator_00827f58.x) * static_cast<double>(k255));
                const U8 b1 = *static_cast<const U8*>(address(colour_at + 1u));
                accumulator_00827f58.y = static_cast<float>(static_cast<double>(static_cast<I>(b1)) * static_cast<double>(accumulator_00827f58.y) * static_cast<double>(k255));
                const U8 b2 = *static_cast<const U8*>(address(colour_at + 2u));
                accumulator_00827f58.z = static_cast<float>(static_cast<double>(static_cast<I>(b2)) * static_cast<double>(accumulator_00827f58.z) * static_cast<double>(k255));
            }
            // 0046B012..0046B0F5: clamp each component: > 0 ? (< 1 ? keep : 1.0f) : 0 (unordered -> 0).
            auto clamp = [](float& v) {
                if (v > 0.0f) { if (!(v < 1.0f)) v = f(0x3F800000u); }
                else v = 0.0f;
            };
            clamp(accumulator_00827f58.x);
            clamp(accumulator_00827f58.y);
            clamp(accumulator_00827f58.z);
            // 0046B0FF..0046B164: ARGB = FF | z | y | x, each byte = low byte of _ftol2(c * 255.0f + 0.5f) (truncation;
            // values lie in [0.5, 255.5] so the C++ truncating conversion equals _ftol2's 64-bit truncation here).
            const float k255f = f(0x437F0000u);                       // [0073EDE4] 255.0
            const double half = static_cast<double>(f(0x3F000000u));  // [0073E9AC] 0.5
            const U bz = static_cast<U>(static_cast<I>(static_cast<double>(accumulator_00827f58.z) * static_cast<double>(k255f) + half)) & 0xFFu;
            const U by = static_cast<U>(static_cast<I>(static_cast<double>(accumulator_00827f58.y) * static_cast<double>(k255f) + half)) & 0xFFu;
            const U bx = static_cast<U>(static_cast<I>(static_cast<double>(accumulator_00827f58.x) * static_cast<double>(k255f) + half)) & 0xFFu;
            colours[i] = ((((0xFFFFFF00u | bz) << 8) | by) << 8) | bx; // 0046B122..0046B164
            ++i;                                                      // 0046B16E
        } while (i < static_cast<I>(field<I16>(node, 0x180)));        // 0046B167..0046B175 (movsx re-read)
    }
unlock:
    (void)aurora_pool_unlock_00423f30(field<U>(trimesh, 0x74));       // 0046B17B..0046B17F
    (void)aurora_pool_unlock_00423f30(field<U>(aurora_tree_as_model_0043e1c0(field<aurora_model::MaxTree*>(node, 0x08), nullptr), 0xAC)); // 0046B184..0046B196
    finish();                                                         // 0046B19B..0046B1B6
}
} // extern "C"
