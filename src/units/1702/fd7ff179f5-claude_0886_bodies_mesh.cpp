// 0886-gates (Claude): faithful bodies for former MESH_GATE / required(0x...) gates of the 0886 cohort unit on the
// walkmesh paths (room_walkmesh_native_mask.inc CSWCollisionMesh / CSWRoomSurfaceMesh vtables and ports).
// Transcribed from the original swkotor.exe (read-only); addresses in comments are original instructions.
// ABI: original __thiscall members are exported as extern "C" __fastcall(self, unused_edx, stack args...):
//   00599ED0 CSWCollisionMesh::TransformToWorld(Vector const* position)                    RET 4   (vtbl +08)
//   00580720 CSWRoomSurfaceMeshAABBNode::CSWRoomSurfaceMeshAABBNode()                       RET 0
//   00581170 CExoArrayList<CSWRoomSurfaceMeshAABBNode>::Allocate(int capacity)              RET 4
//   00581290 CExoArrayList<CSWRoomSurfaceMeshAABBNode>::Add(CSWRoomSurfaceMeshAABBNode)     RET 2Ch
//   00581BD0 CExoArrayList<CSWRoomSurfaceMeshAABBNode>::operator=(CExoArrayList const&)     RET 4
// x87 at 53-bit precision: double intermediates, float exactly where the original stores a dword.
// Allocation: live_operator_new / live_free only; new[]/delete[] go through the existing CRT iterator owners.
// EH: 00581170 registers an unwind frame that only frees the new block when an element constructor throws; the
// AABB node constructor (00580720) only stores constants, so that cleanup is not reproduced.
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(a); }
inline U word(const void* p) { return reinterpret_cast<U>(p); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
inline float rf(U a) { float v; std::memcpy(&v, ptr(a), 4); return v; }
inline void wf(U a, float v) { std::memcpy(ptr(a), &v, 4); }
struct Node { U d[11]; };                                                           // CSWRoomSurfaceMeshAABBNode, 2Ch
static_assert(sizeof(Node) == 0x2c, "AABB node is 2Ch bytes");
}

extern "C" {
void* __cdecl live_operator_new(unsigned);                                          // 006FA7E6
void __cdecl live_free(void*);                                                      // 006FA390
void __stdcall crt_eh_vector_constructor_006fb55f(void* first, U size, I count, void* construct, void* destroy); // 006FB55F (d1)
void __stdcall c2da_destroy_array_006fa852(void* first, U size, I count, void* destroy); // 006FA852 eh_vector_destructor_iterator (0186)
void __fastcall kotor_m15e_slot090_void0_004015a0(void*, void*);                    // 004015A0 empty destructor (0832)

void __fastcall kotor_fastcall_00599ed0(void* mesh, void*, const float* position);
void* __fastcall kotor_fastcall_00580720(void* node, void*);
void __fastcall kotor_fastcall_00581170(void* list, void*, I capacity);
void __fastcall kotor_fastcall_00581290(void* list, void*, Node node);
void* __fastcall kotor_fastcall_00581bd0(void* list, void*, const void* other);
}

// ===================================================================================== 00599ED0
// CSWCollisionMesh::TransformToWorld: once (+4Ch == 0), stores the position at +20h, sets +4 = 1 and moves every
// vertex (+50h count re-read, +54h data re-read) by the local offset (+2Ch) and then the stored position (+20h).
// x and y keep the unrounded (v + offset) in ST before adding the position; z is rounded to float first.
void __fastcall kotor_fastcall_00599ed0(void* mesh, void*, const float* position) {
    const U self = word(mesh);
    if (rd(self + 0x4c) != 0) return;                                               // 00599ED0..00599ED8
    const U from = word(position);
    wr(self + 0x20, rd(from));                                                      // 00599EDE..00599EEB
    wr(self + 0x24, rd(from + 4));                                                  // 00599EED..00599EF0
    wr(self + 0x28, rd(from + 8));                                                  // 00599EF3..00599EF6
    wr(self + 4, 1);                                                                // 00599F00
    for (I i = 0; i < static_cast<I>(rd(self + 0x50)); ++i) {                       // 00599EF9..00599F07, 00599F5D..00599F60
        const U v = rd(self + 0x54) + 12u * static_cast<U>(i);                      // 00599F10..00599F16
        const double x = static_cast<double>(rf(v)) + rf(self + 0x2c);              // 00599F13..00599F18
        const double y = static_cast<double>(rf(v + 4)) + rf(self + 0x30);          // 00599F1C..00599F22
        const float z = static_cast<float>(static_cast<double>(rf(v + 8)) + rf(self + 0x34)); // 00599F25..00599F2B
        const float wx = static_cast<float>(x + rf(self + 0x20));                   // 00599F2F..00599F33
        const float wy = static_cast<float>(y + rf(self + 0x24));                   // 00599F3B..00599F3E
        const float wz = static_cast<float>(static_cast<double>(z) + rf(self + 0x28)); // 00599F42..00599F52
        wf(v, wx);                                                                  // 00599F49
        wf(v + 4, wy);                                                              // 00599F4F
        wf(v + 8, wz);                                                              // 00599F5A
    }
}

// ===================================================================================== 00580720
// CSWRoomSurfaceMeshAABBNode ctor: box/+0..+14 = 0, +18 = -1, +1C = 4, +20 = 0, +24/+28 (children) = -1.
void* __fastcall kotor_fastcall_00580720(void* node, void*) {
    const U self = word(node);
    for (U off = 0; off <= 0x14; off += 4) wr(self + off, 0);                       // 00580722..00580732
    wr(self + 0x24, 0xffffffffu);                                                   // 00580735..00580738
    wr(self + 0x28, 0xffffffffu);                                                   // 0058073B
    wr(self + 0x18, 0xffffffffu);                                                   // 0058073E
    wr(self + 0x1c, 4);                                                             // 00580741
    wr(self + 0x20, 0);                                                             // 00580748
    return node;                                                                    // EAX = this
}

// ===================================================================================== 00581170
// Allocate: new[] (count cookie at -4) of `capacity` constructed nodes, element-wise copy of the `size` (+4,
// re-read) old nodes, delete[] of the old block (empty destructor iterator, then free of the cookie address).
void __fastcall kotor_fastcall_00581170(void* list, void*, I capacity) {
    const U self = word(list);
    const U old_data = rd(self);                                                    // 00581195 / 0058119B
    wr(self + 8, static_cast<U>(capacity));                                         // 0058119F
    void* const block = live_operator_new(static_cast<U>(capacity) * 0x2cu + 4u);   // 0058118E..005811A2
    U data = 0;                                                                     // 005811AE
    if (block != nullptr) {                                                         // 005811B0..005811B6
        wr(word(block), static_cast<U>(capacity));                                  // 005811C9 cookie
        data = word(block) + 4;                                                     // 005811C3
        crt_eh_vector_constructor_006fb55f(ptr(data), 0x2c, capacity,
            reinterpret_cast<void*>(&kotor_fastcall_00580720),
            reinterpret_cast<void*>(&kotor_m15e_slot090_void0_004015a0));          // 005811B8..005811CB
    }
    wr(self, data);                                                                 // 005811DF
    for (I i = 0; i < static_cast<I>(rd(self + 4)); ++i) {                          // 005811D0..005811E1, 00581245..0058124E
        const U from = old_data + 0x2cu * static_cast<U>(i);
        const U to = rd(self) + 0x2cu * static_cast<U>(i);                          // 005811F7..005811FB ([esi] re-read)
        for (U k = 0; k < 0x2c; k += 4) wr(to + k, rd(from + k));                   // 005811FD..00581242
    }
    if (old_data != 0) {                                                            // 00581250..00581256
        c2da_destroy_array_006fa852(ptr(old_data), 0x2c, static_cast<I>(rd(old_data - 4)),
            reinterpret_cast<void*>(&kotor_m15e_slot090_void0_004015a0));          // 00581258..00581267
        live_free(ptr(old_data - 4));                                               // 0058126C..0058126D
    }
}

// ===================================================================================== 00581290
// Add: grows to capacity*2 (16 when empty) when full, then appends the by-value node.
void __fastcall kotor_fastcall_00581290(void* list, void*, Node node) {
    const U self = word(list);
    const U capacity = rd(self + 8);                                                // 00581293
    if (rd(self + 4) == capacity)                                                   // 00581296..00581299
        kotor_fastcall_00581170(list, nullptr, capacity != 0 ? static_cast<I>(capacity + capacity) : 0x10); // 0058129B..005812AB
    const U size = rd(self + 4);                                                    // 005812B0
    const U slot = rd(self) + size * 0x2cu;                                         // 005812B3..005812BA
    wr(self + 4, size + 1);                                                         // 005812C0..005812C1
    for (U k = 0; k < 11; ++k) wr(slot + 4 * k, node.d[k]);                        // 005812C6..0058130D
}

// ===================================================================================== 00581BD0
// operator=: size = 0, then Add of every element of `other` (+4 size and +0 data re-read each time); returns this.
void* __fastcall kotor_fastcall_00581bd0(void* list, void*, const void* other) {
    wr(word(list) + 4, 0);                                                          // 00581BD9..00581BDB
    const U source = word(other);
    for (I i = 0; i < static_cast<I>(rd(source + 4)); ++i) {                        // 00581BDE..00581BE7, 00581C0E..00581C1F
        Node copy;
        std::memcpy(&copy, ptr(rd(source) + 0x2cu * static_cast<U>(i)), sizeof copy); // 00581BF0..00581C01 rep movsd
        kotor_fastcall_00581290(list, nullptr, copy);                               // 00581C03..00581C05
    }
    return list;                                                                    // EAX = this
}

// =====================================================================================================
//   00581FE0 CSWRoomSurfaceMesh::~CSWRoomSurfaceMesh()  RET 0 (layout room release on module unload)
//   005965D0 CSWCollisionMesh::~CSWCollisionMesh()      RET 0
// Vtable stores (0074944C / 0074A068) bind to the 0886 unit's rebuilt mesh tables through
// claude_0886_mesh_table(original address) (room_walkmesh_native_mask.inc).
// EH: ~CSWRoomSurfaceMesh registers member-destruction unwind states (5..0); the bound member destructors do not
// throw in normal operation, so the unwind replay is not reproduced (normal-path order is kept exactly).
namespace frame_control { extern unsigned resource_manager_007a39e8; }           // [007A39E8] CExoResMan*
extern "C" {
void* __cdecl claude_0886_mesh_table(unsigned original);                         // 0886 unit: rebuilt mesh vtable
void __fastcall c2da_release_demand_00409b80(void* res, void*);                  // 00409B80 CRes::Release (0264)
unsigned __fastcall c2da_release_00409cf0(void* manager, void*, void* res);      // 00409CF0 CExoResMan::ReleaseResObject (0264)
void __fastcall kotor_fastcall_00581fe0(void* mesh, void*);
void __fastcall kotor_fastcall_005965d0(void* mesh, void*);
}

void __fastcall kotor_fastcall_005965d0(void* mesh, void*) {
    const U s = word(mesh);
    wr(s, word(claude_0886_mesh_table(0x74a068)));                               // 005965DB
    if (rd(s + 0x4c) != 0) {                                                     // 005965D3..005965E1 (loaded from a resource)
        const U res = rd(s + 0x1c);                                              // 005965E3
        wr(s + 0x54, 0); wr(s + 0x60, 0); wr(s + 0x64, 0); wr(s + 0x6c, 0); wr(s + 0x68, 0); // 005965E6..005965F2
        c2da_release_demand_00409b80(ptr(res), nullptr);                        // 005965F5
        if (c2da_release_00409cf0(ptr(frame_control::resource_manager_007a39e8), nullptr, ptr(rd(s + 0x1c))) == 0) { // 005965FA..0059660B
            const U r = rd(s + 0x1c);                                            // 0059660D
            if (r != 0) {                                                        // 00596610..00596612
                using Destroy = void*(__thiscall*)(void*, U);
                (void)reinterpret_cast<Destroy>(rd(rd(r)))(ptr(r), 1);          // 00596614..00596618
            }
        }
        wr(s + 0x1c, 0);                                                         // 0059661A
        return;
    }
    static const U owned[] = {0x54, 0x60, 0x64, 0x68, 0x6c};                     // 00596620..0059665C
    for (const U offset : owned) { live_free(ptr(rd(s + offset))); wr(s + offset, 0); }
}

void __fastcall kotor_fastcall_00581fe0(void* mesh, void*) {
    const U s = word(mesh);
    wr(s, word(claude_0886_mesh_table(0x74944c)));                               // 00581FFE
    if (rd(s + 0x4c) != 0) {                                                     // 00582004..00582013
        static const U cleared[] = {0x88, 0xac, 0xb0, 0xb4, 0xb8, 0x8c, 0x90, 0x94, 0x98, 0xa8}; // 00582015..0058204B
        for (const U offset : cleared) wr(s + offset, 0);
        wr(s + 0x9c, 0);                                                         // 00582051 -> 005820B2
        wr(s + 0xa4, 0);                                                         // 005820B8
        wr(s + 0xa0, 0);                                                         // 005820BE
    } else {
        live_free(ptr(rd(s + 0x88)));                                            // 00582053..0058205A
        wr(s + 0x88, 0);                                                         // 0058205F
        if (rd(s + 0x98) == 0) {                                                 // 00582065..00582070
            if (rd(s + 0x8c) != 0) {                                             // 00582072..0058207A
                live_free(ptr(rd(s + 0x8c)));                                    // 0058207C..0058207D
                wr(s + 0x8c, 0);                                                 // 00582085
                wr(s + 0x94, 0);                                                 // 0058208B
            }
            wr(s + 0x90, 0);                                                     // 00582091
        }
        if (rd(s + 0xa8) == 0) {                                                 // 00582097..0058209D
            if (rd(s + 0x9c) != 0) {                                             // 0058209F..005820A7
                live_free(ptr(rd(s + 0x9c)));                                    // 005820A9..005820AA
                wr(s + 0x9c, 0);                                                 // 005820B2
                wr(s + 0xa4, 0);                                                 // 005820B8
            }
            wr(s + 0xa0, 0);                                                     // 005820BE
        }
    }
    live_free(ptr(rd(s + 0xc8)));                                                // 005820C4..005820D0
    wr(s + 0xc8, 0);                                                             // 005820D5
    live_free(ptr(rd(s + 0xbc)));                                                // 005820DB..005820E7
    wr(s + 0xbc, 0);                                                             // 005820EC
    const U nodes = rd(s + 0xac);                                                // 005820F2
    if (nodes != 0) {                                                            // 005820FB..00582102
        c2da_destroy_array_006fa852(ptr(nodes), 0x2c, static_cast<I>(rd(nodes - 4)),
            reinterpret_cast<void*>(&kotor_m15e_slot090_void0_004015a0));       // 00582104..00582114 (AABB node[] delete)
        live_free(ptr(nodes - 4));                                               // 00582119..0058211A
    }
    wr(s + 0xac, 0);                                                             // 00582123
    live_free(ptr(rd(s + 0x9c)));                                                // 00582129..00582135
    wr(s + 0x9c, 0);                                                             // 0058213A
    live_free(ptr(rd(s + 0x8c)));                                                // 00582140..0058214B
    wr(s + 0x8c, 0);                                                             // 00582155
    kotor_fastcall_005965d0(mesh, nullptr);                                      // 00582153..00582163
}
