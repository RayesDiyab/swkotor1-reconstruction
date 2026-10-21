// claude_walkmesh.cpp (lane worker "walkmesh", M-1): the room text-walkmesh loader behind the 0886 room-mesh vtable
// slot 10 (+28h, was a required(0x582D70) gate) and its callees, transcribed from the original swkotor.exe (read-only):
//   00582D70 CSWRoomSurfaceMesh::LoadMeshText(char* data, ulong size)        RET 8, EAX 0/1   kotor_fastcall_00582d70
//   00580700 CSWRoomSurfaceMesh::SetOrientation(Quaternion const*)           RET 4             kotor_fastcall_00580700
//   005803C0 CSWRoomSurfaceMesh::ComputeTriangleAdjacency()                  RET 0             kotor_fastcall_005803c0
//   005821E0 CSWRoomSurfaceMesh::ComputeRoomBorder()                         RET 0             kotor_fastcall_005821e0
// File-local helpers (bodies of two tiny list growers that already have several file-local ports in the image):
//   005E03C0 CExoArrayList<int>::Allocate(int)   and   00581CD0 CExoArrayList<8-byte pair>::Allocate(int)
//
// Why this is reached: CSWCollisionMesh::LoadMesh 00596670 asks for the room's WOK (type [+48h] = 7E0h); when the
// room has none it asks for type [+4Ah] = 7D2h (MDL, set by CSWRoomSurfaceMesh::CSWRoomSurfaceMesh 00582B02/00582B09)
// and hands that buffer to this text loader. Model-only rooms (STUNT_* cutscene modules: stuntroom12aa4,
// stuntroom40d) therefore always come here with a binary MDL: no line starts with "node aabb", so nothing is parsed
// and the loader ends in the +0A4h == 0 branch (virtual slot 0 = 0063E7A0 return_true) and returns 1.
//
// x87: PC_53 assumed; every x87 intermediate is a double, every `fstp dword` an explicit float, every `fstp qword`
// an explicit double. __ftol2 is reproduced as a 64-bit truncation keeping the low dword.
// Quirks kept: LoadMeshText returns 1 on every exit except the line-buffer allocation failure (the original jumps to
// the common EAX=1 epilogue after its failure cleanups); a missing vertex/face line leaks the per-section arrays and
// the half-built AABB nodes; the "faces" arrays of an earlier "faces" line are overwritten (leaked) by a later one;
// the AABB face index and the orientation angle are not reset per line (kept function-scope; the original stack
// slots start uninitialized, here 0); after "verts"/"faces" the remaining keyword tests run on the same pointer into
// the reused line buffer; ComputeRoomBorder retries forever when a border loop does not close (malformed mesh);
// ComputeTriangleAdjacency records only every second matching edge pair (single found flag across all loops).
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
struct Vec { float x, y, z; };
static_assert(sizeof(Vec) == 12, "Vector ABI");
struct Node { U d[11]; };                                                           // CSWRoomSurfaceMeshAABBNode, 2Ch
static_assert(sizeof(Node) == 0x2c, "AABB node is 2Ch bytes");
inline void* ptr(U a) { return reinterpret_cast<void*>(a); }
inline U word(const void* p) { return reinterpret_cast<U>(p); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline I ri(U a) { return static_cast<I>(rd(a)); }
inline float rf(U a) { float v; std::memcpy(&v, ptr(a), 4); return v; }
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
inline void wf(U a, float v) { std::memcpy(ptr(a), &v, 4); }
inline U fbits(float f) { U v; std::memcpy(&v, &f, 4); return v; }
inline float bitsf(U v) { float f; std::memcpy(&f, &v, 4); return f; }
// __ftol2 006FAE8C: truncate to a 64-bit integer, keep the low dword (NaN / out of range -> 0x8000000000000000 -> 0).
inline I ftol2(double v) {
    if (!(v > -9223372036854775808.0 && v < 9223372036854775808.0)) return 0;
    return static_cast<I>(static_cast<U>(static_cast<std::uint64_t>(static_cast<std::int64_t>(v))));
}
}

extern "C" {
// ---------------- existing providers (c57 map, exact decorated arity) ----------------
void* __cdecl live_operator_new(unsigned);                                                      // 006FA7E6
void __cdecl live_free(void*);                                                                  // 006FA390 (null-safe)
int __cdecl kotor_strncmp_006faff0(const char* a, const char* b, U n);                          // 006FAFF0 _strncmp [0389]
int __cdecl kotor_sscanf_006fae08(const char* buffer, const char* format, ...);                 // 006FAE08 _sscanf [claude_snapshot_crt]
void* __fastcall quaternion_construct_004ac960(void* out, void*, Vec axis, float angle);       // 004AC960 Quaternion(Vector, float) [model46]
void* __fastcall frame_LogStringCtor_005e5a90(void* string, void*, const char* text);           // 005E5A90 CExoString(char const*) [0459]
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);                             // 005E5C20 ~CExoString [0459]
int __fastcall c2da_current_get_int_00413660(void* table, void*, int row, void* column, int* out); // 00413660 C2DA::GetINTEntry [0376]
U __fastcall kotor_fastcall_005968a0(void* mesh, void*, char** cursor, U* remaining, char* out, U size); // 005968A0 LoadMeshString [closeB_mesh]
void __fastcall kotor_fastcall_00596900(void* mesh, void*, U count);                           // 00596900 SetTriangleCount [closeB_mesh]
void __fastcall kotor_fastcall_005969a0(void* mesh, void*, U count);                           // 005969A0 SetVertexCount [closeB_mesh]
void __fastcall kotor_fastcall_00581290(void* list, void*, Node node);                         // 00581290 CExoArrayList<AABBNode>::Add [0886_bodies_mesh]
extern void* kotor_cell_rules_007a3a28;                                                         // [007A3A28] CSWRules*

// ---------------- bodies defined here ----------------
U __fastcall kotor_fastcall_00582d70(void* mesh, void*, char* data, U size);
void __fastcall kotor_fastcall_00580700(void* mesh, void*, const void* quaternion);
void __fastcall kotor_fastcall_005803c0(void* mesh, void*);
void __fastcall kotor_fastcall_005821e0(void* mesh, void*);
}

namespace {
bool eq(const char* s, const char* lit, U n) { return kotor_strncmp_006faff0(s, lit, n) == 0; }
// virtual slot 0 of the mesh (room table 0074944C[0] = 0063E7A0 return_true): `mov edx,[esi] / call [edx]`
void vcall0(U s) { using F = U(__fastcall*)(void*, void*); (void)reinterpret_cast<F>(rd(rd(s)))(ptr(s), nullptr); }

// CExoArrayList<T> {data, count, capacity} of dwords as the inlined growth sequences use it:
// if (count == cap) { cap = cap ? cap*2 : 16; old = data; data = new(cap*4); copy count; if (old) free(old); }
struct DList { U data, count, cap; };
void dlist_push(DList& l, U value) {
    if (l.count == l.cap) {
        const U cap = l.cap != 0 ? l.cap + l.cap : 0x10;
        l.cap = cap;
        const U old = l.data;
        l.data = word(live_operator_new(cap << 2));
        if (static_cast<I>(l.count) > 0)
            for (U i = 0; i < l.count; ++i) wr(l.data + i * 4, rd(old + i * 4));
        if (old != 0) live_free(ptr(old));
    }
    wr(l.data + l.count * 4, value);
    ++l.count;
}
// CExoArrayList::Allocate 005E03C0 (thiscall list, int capacity): cap = n; data = new(n*4); copy count; free old.
void list_allocate_005e03c0(U list, U capacity) {
    wr(list + 8, capacity);                                                          // 005E03C7
    const U old = rd(list);                                                          // 005E03CB
    wr(list, word(live_operator_new(capacity << 2)));                                // 005E03CD..005E03D9
    for (I i = 0; i < ri(list + 4); ++i) wr(rd(list) + static_cast<U>(i) * 4, rd(old + static_cast<U>(i) * 4)); // 005E03DE..005E03F2
    if (old != 0) live_free(ptr(old));                                               // 005E03F4..005E03FE
}
// FUN_00581CD0 (thiscall list of 8-byte {int,int}, int capacity): cap = n; data = new(n*8) filled with {-1,-1};
// copy count pairs; free old.
void list8_allocate_00581cd0(U list, U capacity) {
    const U old = rd(list);                                                          // 00581CD9
    wr(list + 8, capacity);                                                          // 00581CE3
    U data = word(live_operator_new(capacity << 3));                                 // 00581CDB..00581CE6
    if (data != 0) {
        if (static_cast<I>(capacity - 1) >= 0)                                       // 00581CF2..00581CF9
            for (U i = 0; i < capacity; ++i) { wr(data + i * 8, 0xffffffffu); wr(data + i * 8 + 4, 0xffffffffu); }
    } else data = 0;                                                                 // 00581D0D
    wr(list, data);                                                                  // 00581D0F
    for (I i = 0; i < ri(list + 4); ++i) {                                           // 00581D11..00581D3D
        const U d = rd(list) + static_cast<U>(i) * 8, o = old + static_cast<U>(i) * 8;
        wr(d, rd(o)); wr(d + 4, rd(o + 4));
    }
    if (old != 0) live_free(ptr(old));                                               // 00581D3F..00581D44
}
// vertex coordinate snap of 005830?? (x at 00583128, y at 0058316A): q = ftol2(v*512 + 0.5); if q % 512 == 0 the
// coordinate becomes q >> 9 (exact integer).
void snap(U a) {
    const I q = ftol2(static_cast<double>(rf(a)) * static_cast<double>(512.0f) + static_cast<double>(0.5f)); // fmul [007494B0] / fadd [0073E9AC]
    if (q % 512 == 0) wf(a, static_cast<float>(q >> 9));                             // and 800001FFh / sar 9 / fild / fstp
}
}

// ===================================================================================== 00580700
void __fastcall kotor_fastcall_00580700(void* mesh, void*, const void* quaternion) {
    std::memcpy(static_cast<unsigned char*>(mesh) + 0x38, quaternion, 16);           // 00580700..0058071D
}

// ===================================================================================== 00582D70
U __fastcall kotor_fastcall_00582d70(void* mesh, void*, char* data, U size) {
    const U s = word(mesh);
    if (rd(s + 0x60) != 0) return 1;                                                 // 00582D95..00582DA1 -> 00583E08
    char* const buf = static_cast<char*>(live_operator_new(0x100));                  // 00582DA7..00582DB8 [esp+1C]
    if (buf == nullptr) return 0;                                                    // 00582DBC..00582DD4
    char* cursor = data;                                                             // [esp+50]
    U remaining = size;                                                              // [esp+48]
    U in_aabb = 0;                                                                   // [esp+80]
    U have_walkmesh = 0;                                                             // [esp+A4]
    float px = 0.0f, py = 0.0f, pz = 0.0f;                                           // [esp+A0] [esp+A8] [esp+AC]
    U faceverts = 0, map_walk = 0, map_nonwalk = 0;                                  // [esp+2C] [esp+40] [esp+44]
    float angle = 0.0f;                                                              // [esp+E0] (never reset)
    I aabb_face = 0;                                                                 // [esp+84] (never reset)
    if (remaining != 0) {                                                            // 00582DE1..00582E22
        do {
            if (kotor_fastcall_005968a0(mesh, nullptr, &cursor, &remaining, buf, 0x100) != 1) break; // 00582E38..00582E52
            char* p = buf;
            while (*p == ' ') ++p;                                                   // 00582E58..00582E66
            if (eq(p, "node", 4)) {                                                  // 00582E68..00582E7A
                p += 4; in_aabb = 0;                                                 // 00582E7E / 00582E87
                if (eq(p, " aabb", 5)) { in_aabb = 1; have_walkmesh = 1; }           // 00582E7C..00582EA6
            }
            if (eq(p, "endnode", 7)) { in_aabb = 0; continue; }                      // 00582EAD..00582EC8 -> 00583D0B
            if (in_aabb != 1) continue;                                              // 00582ECD..00582ED5
            if (eq(p, "position", 8)) {                                              // 00582EDB..00582EED
                p += 8;                                                              // 00582F07
                (void)kotor_sscanf_006fae08(p, "%f %f %f", &px, &py, &pz);           // 00582EEF..00582F10
                wf(s + 0x2c, px); wf(s + 0x30, py); wf(s + 0x34, pz);                // 00582F15..00582F63
            }
            if (eq(p, "orientation", 0xb)) {                                         // 00582F6C..00582F7E
                float ox = 0.0f, oy = 0.0f, oz = 0.0f;                               // 00582F8C..00582F9A [esp+94..9C]
                p += 0xb;                                                            // 00582FB9
                (void)kotor_sscanf_006fae08(p, "%f %f %f %f", &ox, &oy, &oz, &angle); // 00582F84..00582FC2
                // fcomp 0.0 / test ah,44h / jp: taken when not equal or unordered
                if (!(ox != 0.0f) && !(oy != 0.0f) && !(oz != 0.0f)) {               // 00582FC7..00583004
                    const U q[4] = {0x3f800000u, 0, 0, 0};                           // 00583010..00583031 [esp+EC..F8]
                    kotor_fastcall_00580700(mesh, nullptr, q);                       // 00583006..0058303C
                } else {
                    alignas(4) unsigned char quaternion[16];                         // [esp+130]
                    const void* r = quaternion_construct_004ac960(quaternion, nullptr, Vec{ox, oy, oz}, angle); // 00583043..00583074
                    std::memcpy(ptr(s + 0x38), r, 16);                               // 00583079..0058308F
                }
            }
            if (eq(p, "verts", 5)) {                                                 // 00583092..005830A4
                p += 5;                                                              // 005830B2
                (void)kotor_sscanf_006fae08(p, "%d", ptr(s + 0x50));                 // 005830AA..005830BB
                kotor_fastcall_005969a0(mesh, nullptr, rd(s + 0x50));                // 005830C0..005830C8
                for (I i = 0; i < ri(s + 0x50); ++i) {                               // 005830CD..005830D8 / 005831A4..005831BC
                    if (kotor_fastcall_005968a0(mesh, nullptr, &cursor, &remaining, buf, 0x100) == 0) { // 005830E0..00583101
                        vcall0(s); live_free(buf); return 1;                         // 00583D62..00583D75 -> 00583E08
                    }
                    const U v = rd(s + 0x54) + static_cast<U>(i) * 12;               // 00583107..0058310A
                    (void)kotor_sscanf_006fae08(buf, "%f %f %f", ptr(v), ptr(v + 4), ptr(v + 8)); // 0058310D..00583120
                    snap(rd(s + 0x54) + static_cast<U>(i) * 12);                     // 00583125..00583161
                    snap(rd(s + 0x54) + static_cast<U>(i) * 12 + 4);                 // 00583163..005831A2
                }
            }
            if (eq(p, "faces", 5)) {                                                 // 005831C2..005831D4
                p += 5;                                                              // 005831E2
                char* const saved = p;                                               // 005831EB [esp+88]
                (void)kotor_sscanf_006fae08(p, "%d", ptr(s + 0x58));                 // 005831DA..005831F2
                kotor_fastcall_00596900(mesh, nullptr, rd(s + 0x58));                // 005831F7..005831FF
                faceverts = word(live_operator_new(rd(s + 0x58) * 12));              // 00583204..0058320D [esp+2C]
                const U surf = word(live_operator_new(rd(s + 0x58) << 2));           // 00583212..0058321E [esp+14]
                for (I k = 0; k < ri(s + 0x58); ++k) {                               // 00583228..00583230 / 005832AB..005832B8
                    if (kotor_fastcall_005968a0(mesh, nullptr, &cursor, &remaining, buf, 0x100) == 0) { // 00583240..0058325F
                        vcall0(s); live_free(buf); return 1;                         // 00583D7A..00583D8F -> 00583E08
                    }
                    const U f = faceverts + static_cast<U>(k) * 12;
                    I d0, d1, d2, d3;                                                // [esp+10C] [esp+114] [esp+FC] [esp+110]
                    (void)kotor_sscanf_006fae08(buf, "%d %d %d %d %d %d %d %d", ptr(f), ptr(f + 4), ptr(f + 8),
                                                &d0, &d1, &d2, &d3, ptr(surf + static_cast<U>(k) * 4)); // 00583265..0058329D
                }
                I walk_count = 0, nonwalk_count = 0;                                 // [esp+4C] [esp+30]
                const U walk_index = word(live_operator_new(rd(s + 0x58) << 2));     // 005832BA..005832D7 EBX / [esp+CC]
                const U nonwalk_index = word(live_operator_new(rd(s + 0x58) << 2));  // 005832CF..005832E3 EDI
                map_walk = word(live_operator_new(rd(s + 0x58) << 2));               // 005832E5..005832F6 [esp+40]
                map_nonwalk = word(live_operator_new(rd(s + 0x58) << 2));            // 005832F0..005832FF [esp+44]
                for (I k = 0; k < ri(s + 0x58); ++k) {                               // 00583303..00583312 / 005833EF..00583407
                    alignas(4) unsigned char column[8];                              // [esp+E4] CExoString
                    frame_LogStringCtor_005e5a90(column, nullptr, "Walk");           // 00583340..0058334C ([00747670])
                    void* const surfacemat = ptr(rd(rd(word(kotor_cell_rules_007a3a28) + 0xb8) + 0x14)); // 00583351..0058335D
                    int walkable = 0;                                                // [esp+B0]
                    (void)c2da_current_get_int_00413660(surfacemat, nullptr, ri(surf + static_cast<U>(k) * 4),
                                                        column, &walkable);          // 00583360..0058338C
                    frame_LogStringDtor_005e5c20(column, nullptr);                   // 00583391..005833A3
                    if (walkable != 0) {                                             // 005833A8..005833B9
                        wr(map_walk + static_cast<U>(k) * 4, static_cast<U>(walk_count));   // 005833BF
                        wr(map_nonwalk + static_cast<U>(k) * 4, 0xffffffffu);             // 005833C1
                        wr(walk_index + static_cast<U>(walk_count) * 4, static_cast<U>(k)); // 005833C8..005833CC
                        ++walk_count;                                                // 005833CF..005833D0
                    } else {
                        wr(map_walk + static_cast<U>(k) * 4, 0xffffffffu);                // 005833DA
                        wr(map_nonwalk + static_cast<U>(k) * 4, static_cast<U>(nonwalk_count)); // 005833E0
                        wr(nonwalk_index + static_cast<U>(nonwalk_count) * 4, static_cast<U>(k)); // 005833E3..005833E7
                        ++nonwalk_count;                                             // 005833EA..005833EB
                    }
                }
                for (I t = 0; t < walk_count; ++t) {                                 // 0058340D..0058349B
                    const U src = faceverts + rd(walk_index + static_cast<U>(t) * 4) * 12;
                    const U dst = rd(s + 0x60) + static_cast<U>(t) * 12;
                    wr(dst, rd(src));                                                // 00583420..00583438
                    wr(rd(s + 0x60) + static_cast<U>(t) * 12 + 4, rd(src + 4));      // 0058343B..00583454
                    wr(rd(s + 0x60) + static_cast<U>(t) * 12 + 8, rd(src + 8));      // 00583458..00583471
                    wr(rd(s + 0x64) + static_cast<U>(t) * 4, rd(surf + rd(walk_index + static_cast<U>(t) * 4) * 4)); // 00583475..00583486
                }
                for (I t = 0; t < nonwalk_count; ++t) {                              // 0058349D..0058352A
                    const U at = static_cast<U>(walk_count + t);
                    const U src = faceverts + rd(nonwalk_index + static_cast<U>(t) * 4) * 12;
                    wr(rd(s + 0x60) + at * 12, rd(src));                             // 005834C0..005834D4
                    wr(rd(s + 0x60) + at * 12 + 4, rd(src + 4));                     // 005834D7..005834E8
                    wr(rd(s + 0x60) + at * 12 + 8, rd(src + 8));                     // 005834EC..00583501
                    wr(rd(s + 0x64) + at * 4, rd(surf + rd(nonwalk_index + static_cast<U>(t) * 4) * 4)); // 00583505..00583516
                }
                wr(s + 0x5c, static_cast<U>(walk_count));                            // 00583533..0058353C
                live_free(ptr(nonwalk_index));                                       // 0058353B..0058353F
                live_free(ptr(walk_index));                                          // 00583544..00583545
                live_free(ptr(surf));                                                // 0058354A..0058354F
                p = saved;                                                           // 00583554
            }
            if (!eq(p, "aabb", 4)) continue;                                         // 0058355E..00583570 -> 00583D0B
            p += 4;                                                                  // 00583578
            DList stack_nodes{0, 0, 0};                                              // [esp+34] [esp+38] [esp+3C]
            DList stack_index{0, 0, 0};                                              // [esp+20] [esp+24] [esp+28]
            DList nodes{0, 0, 0};                                                    // [esp+6C] [esp+70] [esp+74]
            U current = 0;                                                           // [esp+10]
            for (;;) {                                                               // 005835C0
                float a[3] = {0.0f, 0.0f, 0.0f}, b[3] = {0.0f, 0.0f, 0.0f};          // [esp+60..68] [esp+54..5C]
                (void)kotor_sscanf_006fae08(p, "%f %f %f %f %f %f %d", &a[0], &a[1], &a[2], &b[0], &b[1], &b[2],
                                            &aabb_face);                             // 005835C0..00583628
                if (aabb_face != -1) {                                               // 0058362D..0058363A
                    const I c = ri(map_walk + static_cast<U>(aabb_face) * 4);        // 0058363C..00583640
                    if (c != -1) aabb_face = c;                                      // 00583643..00583648
                    else aabb_face = ri(map_nonwalk + static_cast<U>(aabb_face) * 4) + ri(s + 0x5c); // 00583651..0058365F
                }
                for (int c = 0; c < 3; ++c) {                                        // 00583666..005836BF
                    // fcomp / test ah,41h / jne: swap only when a > b (ordered)
                    if (a[c] > b[c]) { const float t = b[c]; b[c] = a[c]; a[c] = t; }
                }
                const U node = word(live_operator_new(0x2c));                        // 005836C3..005836CA
                if (node != 0) {                                                     // 005836CD..005836FA
                    for (U o = 0; o < 0x18; o += 4) wr(node + o, 0);
                    wr(node + 0x24, 0xffffffffu); wr(node + 0x28, 0xffffffffu); wr(node + 0x18, 0xffffffffu);
                    wr(node + 0x1c, 4); wr(node + 0x20, 0);
                }
                dlist_push(nodes, node);                                             // 00583700..0058376B
                wr(node + 0x18, static_cast<U>(aabb_face));                          // 0058376E..00583775
                current = nodes.count - 1;                                           // 00583788..0058379E
                const float eps = bitsf(0x3c23d70au);                                // [0073DFEC] 0.01f
                wf(node + 0, static_cast<float>(static_cast<double>(a[0]) - eps));   // 00583793..005837B0
                wf(node + 4, static_cast<float>(static_cast<double>(a[1]) - eps));   // 00583784..005837C0
                wf(node + 8, static_cast<float>(static_cast<double>(a[2]) - eps));   // 00583778..005837D1
                wf(node + 0xc, static_cast<float>(static_cast<double>(b[0]) + eps)); // 005837EB..00583803
                wf(node + 0x10, static_cast<float>(static_cast<double>(b[1]) + eps)); // 005837E1..00583813
                wf(node + 0x14, static_cast<float>(static_cast<double>(b[2]) + eps)); // 005837D4..00583824
                bool read_next;
                if (ri(node + 0x18) == -1) {                                         // 00583827..0058382B: internal node
                    dlist_push(stack_nodes, node);                                   // 00583831..0058389E
                    dlist_push(stack_index, current);                                // 00583897..00583912
                    read_next = true;                                                // 00583913..00583930
                } else {
                    read_next = false;                                               // leaf: walk back up
                    while (static_cast<I>(stack_nodes.count) > 0) {                  // 005839A1..005839A3
                        const U parent_index = rd(stack_index.data + (stack_index.count - 1) * 4); // 005839A9..005839BD
                        const U parent = rd(stack_nodes.data + (stack_nodes.count - 1) * 4);       // 005839B5..005839B9
                        wr(stack_nodes.data + (stack_nodes.count - 1) * 4, 0);       // 005839C4
                        --stack_nodes.count;                                         // 005839C1..005839E0 (DelIndex(last))
                        --stack_index.count;                                         // 005839E2..005839FB
                        if (ri(parent + 0x24) != -1) {                               // 005839FD..00583A03: right child
                            wr(parent + 0x28, current);                              // 00583A09..00583A11
                            const U l = rd(nodes.data + rd(parent + 0x24) * 4);      // 00583A14
                            const U r = rd(nodes.data + current * 4);                // 00583A28
                            const double lz = static_cast<double>(rf(l + 8)) + static_cast<double>(rf(l + 0x14)); // 00583A17..00583A1A
                            const double ly = static_cast<double>(rf(l + 4)) + static_cast<double>(rf(l + 0x10)); // 00583A1D..00583A20
                            const float lx = static_cast<float>(static_cast<double>(rf(l)) + static_cast<double>(rf(l + 0xc))); // 00583A23..00583A2B
                            const double rz = static_cast<double>(rf(r + 8)) + static_cast<double>(rf(r + 0x14)); // 00583A32..00583A35
                            const float ry = static_cast<float>(static_cast<double>(rf(r + 4)) + static_cast<double>(rf(r + 0x10))); // 00583A38..00583A3E
                            const float rx = static_cast<float>(static_cast<double>(rf(r)) + static_cast<double>(rf(r + 0xc))); // 00583A45..00583A4A
                            const float dz = static_cast<float>(rz - lz);            // 00583A51..00583A53 [esp+14]
                            const double dy = static_cast<double>(ry) - ly;          // 00583A57..00583A62 (kept on the stack)
                            const double dxd = static_cast<double>(rx) - static_cast<double>(lx); // 00583A64..00583A6B
                            const float dx = static_cast<float>(dxd);                // 00583A72 [esp+100]
                            const double adx = dxd < 0.0 ? -dxd : dxd;               // 00583A79 fabs (unrounded)
                            const double ady = dy < 0.0 ? -dy : dy;                  // 00583A7B..00583A7F [esp+88] qword
                            const double adz = dz < 0.0f ? -static_cast<double>(dz) : static_cast<double>(dz);
                            U axis;
                            if (adx > ady && adx > adz) {                            // 00583A86..00583AA3
                                axis = dx > 0.0f ? 1u : 8u;                          // 00583AA7..00583AC4
                            } else if (adz < ady) {                                  // 00583AD7..00583AE9 (test ah,5 / jp)
                                axis = dy > 0.0 ? 2u : 0x10u;                        // 00583AEB..00583B01
                            } else {
                                axis = dz > 0.0f ? 4u : 0x20u;                       // 00583B12..00583B2A
                            }
                            wr(parent + 0x20, axis);                                 // 00583AC9 / 00583B06 / 00583B33
                            current = parent_index;                                  // 00583ACC / 00583B09 / 00583B36
                            continue;                                                // -> 005839A1
                        }
                        wr(parent + 0x24, current);                                  // 00583B3F..00583B49: left child
                        dlist_push(stack_nodes, parent);                             // 00583B4C..00583BB5
                        dlist_push(stack_index, parent_index);                       // 00583BB9..00583C1F
                        read_next = true;                                            // 00583C20..00583C3D
                        break;
                    }
                }
                if (read_next) {
                    if (kotor_fastcall_005968a0(mesh, nullptr, &cursor, &remaining, buf, 0x100) == 0) {
                        vcall0(s);                                                   // 00583936..0058393A / 00583D94..00583D98
                        live_free(buf); live_free(ptr(map_walk)); live_free(ptr(map_nonwalk)); live_free(ptr(faceverts));
                        live_free(ptr(nodes.data)); live_free(ptr(stack_index.data)); live_free(ptr(stack_nodes.data));
                        return 1;                                                    // 0058393C..0058399C / 00583D9A..00583DFA
                    }
                    if (static_cast<I>(stack_nodes.count) > 0) { p = buf; continue; } // 00583C43..00583C45 -> 005835BA
                }
                break;                                                               // -> 00583C4B
            }
            wr(s + 0xd4, current);                                                   // 00583C4B..00583C57
            for (I t = 0; t < static_cast<I>(nodes.count); ++t) {                    // 00583C5D..00583CC3
                const U n = rd(nodes.data + static_cast<U>(t) * 4);
                if (rd(s + 0xb8) == 0) {                                             // 00583C70..00583C84
                    Node copy; std::memcpy(&copy, ptr(n), sizeof copy);              // 00583C86..00583C90
                    kotor_fastcall_00581290(ptr(s + 0xac), nullptr, copy);           // 00583C92..00583C9C
                }
                if (n != 0) live_free(ptr(n));                                       // 00583CA5..00583CAD
                wr(nodes.data + static_cast<U>(t) * 4, 0);                           // 00583CB9
            }
            live_free(ptr(nodes.data));                                              // 00583CC5..00583CCE
            live_free(ptr(stack_index.data));                                        // 00583CD3..00583CE6
            live_free(ptr(stack_nodes.data));                                        // 00583CEB..00583CFF
        } while (remaining != 0);                                                    // 00583D0B..00583D11 (ja)
    }
    live_free(buf);                                                                  // 00583D21..00583D22
    live_free(ptr(map_walk));                                                        // 00583D27..00583D2C
    live_free(ptr(map_nonwalk));                                                     // 00583D31..00583D36
    live_free(ptr(faceverts));                                                       // 00583D3B..00583D40
    if (have_walkmesh != 0) {                                                        // 00583D45..00583D53
        kotor_fastcall_005803c0(mesh, nullptr);                                      // 00583DFC
        kotor_fastcall_005821e0(mesh, nullptr);                                      // 00583E01..00583E03
    } else {
        vcall0(s);                                                                   // 00583D59..00583D5B
    }
    return 1;                                                                        // 00583E08..00583E21
}

// ===================================================================================== 005803C0
// Edge adjacency of the walkable faces (+5Ch of them): per face the vertex cycle v0 v1 v2 v0 v2 v1 v0 (7 ints), every
// consecutive pair of face i compared with every consecutive pair of each later face j. One `found` flag spans all
// loops: the first match only sets it, the next match records the pair (edge = position<3 ? pos : 6-pos) and clears it.
void __fastcall kotor_fastcall_005803c0(void* mesh, void*) {
    const U s = word(mesh);
    if (rd(s + 0x5c) == 0) return;                                                   // 005803C6..005803CB
    wr(s + 0x88, word(live_operator_new(rd(s + 0x5c) * 12)));                        // 005803D3..005803E7
    const U tmp = word(live_operator_new(rd(s + 0x5c) * 0x1c));                      // 005803E0..005803ED [esp+34]
    for (I i = 0; i < ri(s + 0x5c); ++i) {                                           // 005803F8..00580464
        const U ui = static_cast<U>(i);
        wr(rd(s + 0x88) + ui * 12 + 8, 0xffffffffu);                                 // 00580410..00580419
        wr(rd(s + 0x88) + ui * 12 + 4, 0xffffffffu);                                 // 0058041D..00580423
        wr(rd(s + 0x88) + ui * 12, 0xffffffffu);                                     // 00580427..0058042D
        const U v0 = rd(rd(s + 0x60) + ui * 12);                                     // 00580430..00580433
        wr(tmp + (ui * 7 + 6) * 4, v0); wr(tmp + (ui * 7 + 3) * 4, v0); wr(tmp + ui * 7 * 4, v0); // 00580436..0058043B
        const U v1 = rd(rd(s + 0x60) + ui * 12 + 4);                                 // 0058043E..00580441
        wr(tmp + (ui * 7 + 5) * 4, v1); wr(tmp + (ui * 7 + 1) * 4, v1);              // 00580445..00580448
        const U v2 = rd(rd(s + 0x60) + ui * 12 + 8);                                 // 0058044B..0058044E
        wr(tmp + (ui * 7 + 4) * 4, v2); wr(tmp + (ui * 7 + 2) * 4, v2);              // 00580452..00580455
    }
    I found = 0;                                                                     // EDX 0058046B
    if (ri(s + 0x5c) > 0) {                                                          // 00580466..0058046F
        I i = 0;
        do {
            for (I a = 1, ra = 5; ra > -1; ++a, --ra) {                              // [esp+10] / [esp+18] 00580481..005805A3
                for (I j = i + 1; j < ri(s + 0x5c); ++j) {                           // 005804A0..005804A7 / 00580571..00580585
                    for (I b = 1, rb = 5; rb > -1; ++b, --rb) {                      // [esp+14] / EDI 005804D0..00580567
                        const U ia = static_cast<U>(i * 7 + a), jb = static_cast<U>(j * 7 + b);
                        if (rd(tmp + (ia - 1) * 4) != rd(tmp + (jb - 1) * 4)) continue; // 005804E0..005804EA
                        if (rd(tmp + ia * 4) != rd(tmp + jb * 4)) continue;          // 005804EC..005804F4
                        if (found == 0) { found = 1; continue; }                     // 005804F6..005804FF
                        const I ea = ra > 2 ? (a % 4) - 1 : ra;                      // 00580501..0058051B
                        const I eb = rb > 2 ? (b % 4) - 1 : rb;                      // 00580522..0058053B
                        const I e1 = i * 3 + ea, e2 = j * 3 + eb;                    // 0058051C..00580543
                        wr(rd(s + 0x88) + static_cast<U>(e1) * 4, static_cast<U>(e2)); // 0058053D..00580545
                        wr(rd(s + 0x88) + static_cast<U>(e2) * 4, static_cast<U>(e1)); // 00580548..0058054E
                        found = 0;                                                   // 00580555
                    }
                }
            }
            ++i;
        } while (i < ri(s + 0x5c));                                                  // 005805A9..005805C4
    }
    live_free(ptr(tmp));                                                             // 005805CA..005805CB
}

// ===================================================================================== 005821E0
// Border loops: every edge without a neighbour (adjacency -1) becomes a {edge, -1} pair; the pairs are chained into
// closed loops (next edge starts at the current edge's end vertex). The loop ends (running pair count) go to the int
// list at +9Ch (unless +0A8h), the ordered pairs to the pair list at +8Ch (unless +98h).
void __fastcall kotor_fastcall_005821e0(void* mesh, void*) {
    static volatile U spin_sink;
    const U s = word(mesh);
    if (rd(s + 0x5c) == 0) return;                                                   // 005821F8..00582204
    alignas(4) U edges[3] = {0, 0, 0};                                               // [esp+30] CExoArrayList<pair*>
    const U el = word(edges);
    DList order{0, 0, 0};                                                            // [esp+3C] [esp+40] [esp+44]
    for (I e = 0; e < ri(s + 0x5c) * 3; ++e) {                                       // 0058222D..005822B1
        if (ri(rd(s + 0x88) + static_cast<U>(e) * 4) != -1) continue;                // 00582240..0058224A
        U pair = word(live_operator_new(8));                                         // 0058224C..0058224E
        if (pair != 0) { wr(pair, static_cast<U>(e)); wr(pair + 4, 0xffffffffu); }   // 00582256..00582263
        else pair = 0;                                                               // 00582269
        if (edges[1] == edges[2]) list_allocate_005e03c0(el, edges[2] != 0 ? edges[2] + edges[2] : 0x10); // 00582271..0058228B
        wr(edges[0] + edges[1] * 4, pair);                                           // 00582294..0058229C
        ++edges[1];                                                                  // 005822A3..005822A4
    }
    const U copy = word(live_operator_new(edges[1] << 2));                           // 005822B3..005822C7 EDI / [esp+14]
    if (static_cast<I>(edges[1]) > 0)                                                // 005822C3..005822CB
        for (U i = 0; i < edges[1]; ++i) wr(copy + i * 4, rd(rd(edges[0] + i * 4))); // 005822CD..005822EA
    I processed = 0;                                                                 // [esp+10]
    if (static_cast<I>(edges[1]) > 0) {                                              // 005822EC..005822F6
        do {
            U i = 0;
            while (ri(copy + i * 4) == -1) ++i;                                      // 00582308..00582319
            dlist_push(order, rd(edges[0] + i * 4));                                 // 0058231B..0058238D
            ++processed;                                                             // 00582391..00582396
            wr(copy + i * 4, 0xffffffffu);                                           // 00582397
            I edge = ri(rd(edges[0] + i * 4));                                       // 0058239E..005823A1
            U faces = rd(s + 0x60);                                                  // 005823A7..005823AB
            const U start = rd(faces + static_cast<U>(edge) * 4);                    // 005823AE..005823B1 [esp+28]
            I rem = edge % 3;                                                        // 005823B5..005823C1
            for (;;) {                                                               // 005823C3
                const I next = rem == 2 ? edge - 2 : edge + 1;                       // 005823C3..005823CB
                const U end = rd(faces + static_cast<U>(next) * 4);                  // 005823CE
                U k = 0;
                for (; static_cast<I>(k) < static_cast<I>(edges[1]); ++k) {          // 005823E0..005823FE
                    if (ri(copy + k * 4) == -1) continue;                            // 005823E0..005823E8
                    if (end == rd(faces + rd(rd(edges[0] + k * 4)) * 4)) break;      // 005823EA..005823F2
                }
                if (static_cast<I>(k) >= static_cast<I>(edges[1])) { spin_sink = spin_sink + k; continue; } // 00582400 jmp 005823C3 (no state change)
                dlist_push(order, rd(edges[0] + k * 4));                             // 00582402..0058247C
                edge = ri(rd(edges[0] + k * 4));                                     // 00582480..00582483
                wr(copy + k * 4, 0xffffffffu);                                       // 0058248D
                ++processed;                                                         // 00582495..00582496
                rem = edge % 3;                                                      // 0058249A..005824A2
                const I after = rem == 2 ? edge - 2 : edge + 1;                      // 005824A8..005824B0
                faces = rd(s + 0x60);                                                // 005824B3..005824B7
                if (start == rd(faces + static_cast<U>(after) * 4)) break;           // 005824BA..005824C1
            }
            if (rd(s + 0xa8) == 0) {                                                 // 005824C7..005824D3
                if (rd(s + 0xa0) == rd(s + 0xa4)) {                                  // 005824D9..005824E5
                    const U cap = rd(s + 0xa4) != 0 ? rd(s + 0xa4) + rd(s + 0xa4) : 0x10; // 005824E7..005824EF
                    const U old = rd(s + 0x9c);                                      // 005824F4
                    wr(s + 0xa4, cap);                                               // 00582502
                    wr(s + 0x9c, word(live_operator_new(cap << 2)));                 // 00582501..00582513
                    for (I t = 0; t < ri(s + 0xa0); ++t)                             // 0058251C..00582537
                        wr(rd(s + 0x9c) + static_cast<U>(t) * 4, rd(old + static_cast<U>(t) * 4));
                    if (old != 0) live_free(ptr(old));                               // 00582539..0058253E
                }
                wr(rd(s + 0x9c) + rd(s + 0xa0) * 4, static_cast<U>(processed));      // 00582546..00582556
                wr(s + 0xa0, rd(s + 0xa0) + 1);                                      // 00582559
            }
        } while (processed < static_cast<I>(edges[1]));                              // 0058255F..0058256B
    }
    for (I t = 0; t < static_cast<I>(order.count); ++t) {                            // 00582571..005825F5
        const U pair = rd(order.data + static_cast<U>(t) * 4);                       // 0058258C..00582590
        if (rd(s + 0x98) == 0) {                                                     // 00582587..00582594
            const U lst = s + 0x8c;
            const U v0 = rd(pair), v1 = rd(pair + 4);                                // 00582596..005825A5
            if (rd(lst + 4) == rd(lst + 8))                                          // 0058259B..005825A9
                list8_allocate_00581cd0(lst, rd(lst + 8) != 0 ? rd(lst + 8) + rd(lst + 8) : 0x10); // 005825AB..005825BB
            const U at = rd(lst) + rd(lst + 4) * 8;                                  // 005825C0..005825C9
            wr(lst + 4, rd(lst + 4) + 1);                                            // 005825CC..005825CD
            wr(at, v0); wr(at + 4, v1);                                              // 005825D0..005825D6
        }
        live_free(ptr(rd(order.data + static_cast<U>(t) * 4)));                      // 005825D9..005825E2
    }
    live_free(ptr(copy));                                                            // 005825F7..005825F8
    live_free(ptr(order.data));                                                      // 005825FD..00582603
    live_free(ptr(edges[0]));                                                        // 00582608..00582615
}
