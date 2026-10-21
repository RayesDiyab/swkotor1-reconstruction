// claude_closeB_mesh.cpp (lane worker closeB, mesh fork): text-walkmesh loaders and the text-mesh (+4Ch == 0) finish
// passes behind the 0886 walkmesh vtable / guard gates, transcribed from the original swkotor.exe (read-only):
//   005CDAD0 CSWPlaceableSurfaceMesh::LoadMeshText(char* data, ulong size)     RET 8,  EAX 0/1  kotor_fastcall_005cdad0
//   005CE160 CSWDoorSurfaceMesh::LoadMeshText(char* data, ulong size)          RET 8,  EAX 1    kotor_fastcall_005ce160
//   005968A0 CSWCollisionMesh::LoadMeshString(char**, ulong*, char*, ulong)    RET 10h, EAX 0/1 kotor_fastcall_005968a0
//   00596900 CSWCollisionMesh::SetTriangleCount(int)                           RET 4            kotor_fastcall_00596900
//   005969A0 CSWCollisionMesh::SetVertexCount(int)                             RET 4            kotor_fastcall_005969a0
//   005CDA60 CSWPlaceableSurfaceMesh::<set use point>(int index, Vector*)      RET 8            kotor_fastcall_005cda60
//   005CE070 CSWDoorSurfaceMesh::<set use point>(uchar index, Vector by value) RET 10h          kotor_fastcall_005ce070
//   00596C10 CSWCollisionMesh::ComputeMeshNormals()                            RET 0            kotor_fastcall_00596c10
//   00580BE0 CSWRoomSurfaceMesh::GetEdgeVertex(int, Vector*)                   RET 8,  EAX 0/1  kotor_fastcall_00580be0
//   00580C60 CSWRoomSurfaceMesh::GetEdgeVertices(int, Vector*, Vector*)        RET 0Ch, EAX 0/1 kotor_fastcall_00580c60
//   00580D50 CSWRoomSurfaceMesh::WeldEdgeVertices(mesh*, float, int, int)      RET 10h, EAX n   kotor_fastcall_00580d50
//   00580E90 CSWRoomSurfaceMesh::SetRoomAdjacency(mesh*, int, int, int, int)   RET 14h, EAX n   kotor_fastcall_00580e90
// __strnicmp 0070AD18: the game never changes the CRT locale, so the original always takes its C-locale branch
// (__ascii_strnicmp 0070B920); it is bound to the linked UCRT _strnicmp, which takes the same C-locale branch.
// x87: PC_53 assumed; every x87 intermediate is a double, every `fstp dword` an explicit float.
// Quirks kept (see the per-function notes): the parsed "orientation" quaternion is constructed into a dead local; the
// placeable loader leaks its line buffer when a vertex line is missing; LoadMeshString overwrites the last copied
// character with NUL; WeldEdgeVertices reuses the Y of the probe vertex loaded before its inner loop; SetRoomAdjacency
// only records the first loop start at edge 0 (the per-loop start is carried by the swap at each loop end).
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
using U8 = std::uint8_t;
struct Vec { float x, y, z; };
static_assert(sizeof(Vec) == 12, "Vector ABI");
inline void* ptr(U a) { return reinterpret_cast<void*>(a); }
inline U word(const void* p) { return reinterpret_cast<U>(p); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline float rf(U a) { float v; std::memcpy(&v, ptr(a), 4); return v; }
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
inline void wf(U a, float v) { std::memcpy(ptr(a), &v, 4); }
inline U fbits(float f) { U v; std::memcpy(&v, &f, 4); return v; }
inline Vec rv(U a) { Vec v; std::memcpy(&v, ptr(a), 12); return v; }
inline void wv(U a, const Vec& v) { std::memcpy(ptr(a), &v, 12); }
}

extern "C" {
// ---------------- existing providers (c35 map, exact decorated arity) ----------------
void* __cdecl live_operator_new(unsigned);                                                      // 006FA7E6
void __cdecl live_free(void*);                                                                  // 006FA390
int __cdecl kotor_strncmp_006faff0(const char* a, const char* b, U n);                          // 006FAFF0 _strncmp [0389]
int __cdecl kotor_sscanf_006fae08(const char* buffer, const char* format, ...);                 // 006FAE08 _sscanf [0360]
int __cdecl _strnicmp(const char* a, const char* b, std::size_t n);                             // 0070AD18 __strnicmp (C locale) [libucrt]
void* __fastcall kotor_m15e_slot00c_self_00641db0(void* self, void*);                           // 00641DB0 self_return [0832]
void* __fastcall quaternion_construct_004ac960(void* out, void*, Vec axis, float angle);       // 004AC960 Quaternion(Vector, float) [model46]
void __stdcall vector_constructor_iterator_00401130(void* first, U size, U count, void* ctor);  // 00401130 vector ctor iterator [0526]
void* __fastcall vector_default_construct_00409e70(void* v, void*);                             // 00409E70 Vector::Vector (zero) [0526]
Vec* __fastcall collision_mesh_world_to_local_00596a00(void* mesh, void*, Vec* out, const Vec* world); // 00596A00 WorldToLocal [0527]
Vec* __cdecl normal_of_004aca00(Vec* out, const Vec* a, const Vec* b, const Vec* c);            // 004ACA00 NormalOf [0464]

// ---------------- bodies defined here ----------------
U __fastcall kotor_fastcall_005968a0(void* mesh, void*, char** cursor, U* remaining, char* out, U size);
void __fastcall kotor_fastcall_00596900(void* mesh, void*, U count);
void __fastcall kotor_fastcall_005969a0(void* mesh, void*, U count);
void __fastcall kotor_fastcall_005cda60(void* mesh, void*, U index, const Vec* point);
void __fastcall kotor_fastcall_005ce070(void* mesh, void*, U index, Vec point);
U __fastcall kotor_fastcall_005cdad0(void* mesh, void*, char* data, U size);
U __fastcall kotor_fastcall_005ce160(void* mesh, void*, char* data, U size);
void __fastcall kotor_fastcall_00596c10(void* mesh, void*);
U __fastcall kotor_fastcall_00580be0(void* mesh, void*, U edge, Vec* out);
U __fastcall kotor_fastcall_00580c60(void* mesh, void*, U edge, Vec* first, Vec* second);
U __fastcall kotor_fastcall_00580d50(void* mesh, void*, void* other, U tolerance_bits, U, U);
U __fastcall kotor_fastcall_00580e90(void* mesh, void*, void* other, U other_room, U this_room, U, U);
}

namespace {
// strncmp(...) == 0 helper with the original lengths
bool eq(const char* s, const char* lit, U n) { return kotor_strncmp_006faff0(s, lit, n) == 0; }
const char* skip_blanks(const char* s) { while (*s == ' ' || *s == '\t') ++s; return s; }   // 005CDB81..005CDB8C
// "orientation" handling shared by both text loaders: sscanf four floats, construct the (unused) quaternion when the
// axis is not (0,0,0); x87 `fcomp 0.0 / test ah,44h / jp` = "not equal or unordered".
void orientation_line(const char* s) {
    float x = 0.0f, y = 0.0f, z = 0.0f, angle;
    (void)kotor_sscanf_006fae08(s, "%f %f %f %f", &x, &y, &z, &angle);
    if (x != 0.0f || y != 0.0f || z != 0.0f) {
        alignas(4) unsigned char quaternion[16];
        (void)quaternion_construct_004ac960(quaternion, nullptr, Vec{x, y, z}, angle);
    }
}
}

// ===================================================================================== 005968A0
// Copies bytes from *cursor until a newline is copied, the buffer is full (size-1) or the input ends, then stores
// NUL over the last copied byte (newline or not). EAX 0 when the input was already empty.
U __fastcall kotor_fastcall_005968a0(void*, void*, char** cursor, U* remaining, char* out, U size) {
    U i = 0xffffffffu;                                                               // 005968A6
    if (*remaining == 0) return 0;                                                   // 005968A4..005968AF
    const U last = size - 1;                                                         // 005968C2
    U left;
    do {
        ++i;                                                                         // 005968C9
        out[i] = **cursor;                                                           // 005968C5..005968CA
        ++*cursor;                                                                   // 005968CD
        left = *remaining - 1;                                                       // 005968CF..005968D1
        *remaining = left;                                                           // 005968D4
        if (i >= last) break;                                                        // 005968D2..005968D6
        if (out[i] == '\n') break;                                                   // 005968D8..005968DC
    } while (left != 0);                                                             // 005968DE..005968E0
    if (i >= size) return 0;                                                         // 005968E2..005968EC
    out[i] = 0;                                                                      // 005968EF
    return 1;
}

// ===================================================================================== 00596900 / 005969A0
void __fastcall kotor_fastcall_00596900(void* mesh, void*, U count) {
    const U s = word(mesh);
    wr(s + 0x58, count);                                                             // 00596924
    if (rd(s + 0x4c) != 0) return;                                                   // 0059691F..00596927
    wr(s + 0x60, word(live_operator_new(count * 12)));                               // 00596929..0059693D
    wr(s + 0x64, word(live_operator_new(count * 4)));                                // 00596935..00596946
    wr(s + 0x6c, word(live_operator_new(count * 4)));                                // 00596945..0059694F
    void* const normals = live_operator_new(count * 12);                             // 0059694E..00596957
    if (normals != nullptr)                                                          // 00596960..0059696A
        vector_constructor_iterator_00401130(normals, 0xc, count,
            reinterpret_cast<void*>(&vector_default_construct_00409e70));            // 0059696C..00596975
    wr(s + 0x68, word(normals));                                                     // 0059697C..0059697E
}
void __fastcall kotor_fastcall_005969a0(void* mesh, void*, U count) {
    const U s = word(mesh);
    wr(s + 0x50, count);                                                             // 005969B0
    const U p = word(live_operator_new(count * 12));                                 // 005969A6..005969B3
    if (p != 0 && static_cast<int>(count - 1) >= 0)                                  // 005969BD..005969C6
        for (U i = 0; i < count; ++i) { wr(p + i * 12, 0); wr(p + i * 12 + 4, 0); wr(p + i * 12 + 8, 0); } // 005969C8..005969DC
    wr(s + 0x54, p);                                                                 // 005969E0 / 005969EB
}

// ===================================================================================== 005CDA60 / 005CE070
void __fastcall kotor_fastcall_005cda60(void* mesh, void*, U index, const Vec* point) {
    Vec local;
    const Vec* const r = collision_mesh_world_to_local_00596a00(mesh, nullptr, &local, point); // 005CDA60..005CDA72
    wv(word(mesh) + index * 12 + 0x70, *r);                                          // 005CDA77..005CDA8F
}
void __fastcall kotor_fastcall_005ce070(void* mesh, void*, U index, Vec point) {
    Vec local;
    const Vec* const r = collision_mesh_world_to_local_00596a00(mesh, nullptr, &local, &point); // 005CE076..005CE082
    wv(word(mesh) + static_cast<U>(static_cast<U8>(index)) * 12 + 0x70, *r);       // 005CE087..005CE0A0
}

// ===================================================================================== 005CDAD0
U __fastcall kotor_fastcall_005cdad0(void* mesh, void*, char* data, U size) {
    const U s = word(mesh);
    char* const line = static_cast<char*>(live_operator_new(0x100));                 // 005CDADA..005CDAE6 EBX
    if (line == nullptr) return 0;                                                   // 005CDAED..005CDAEF -> 005CDFF9
    char* cursor = data;                                                             // [esp+24]
    U remaining = size;                                                              // [esp+18]
    U in_node = 0;                                                                   // [esp+14] (EDI at the loop top)
    U trimesh = 0;                                                                   // [esp+48]
    U8 use_point = 0;                                                                // [esp+13]
    float px = 0.0f, py = 0.0f, pz = 0.0f;                                           // [esp+28] [esp+1C] [esp+30]
    float vx = 0.0f, vy = 0.0f, vz = 0.0f;                                           // [esp+4C] [esp+50] [esp+54]
    int faces = 0, verts = 0;                                                        // [esp+20] [esp+2C]
    char tag;                                                                        // [esp+47] / [esp+37]
    while (remaining != 0) {                                                         // 005CDB07..005CDB32 / 005CDFDF..005CDFE5
        const U node_flag = in_node;                                                 // 005CDB5F EDI
        if (kotor_fastcall_005968a0(mesh, nullptr, &cursor, &remaining, line, 0x100) != 1) break; // 005CDB63..005CDB7D
        const char* p = skip_blanks(line);                                           // 005CDB7F..005CDB8C
        if (eq(p, "node", 4)) {                                                      // 005CDB8E..005CDBA0
            p += 4; in_node = 0;                                                     // 005CDBA6..005CDBB1
            if (eq(p, " trimesh ", 9)) { in_node = 1; trimesh = 1; continue; }       // 005CDBB5..005CDBCE
            if (!eq(p, " dummy ", 7)) continue;                                      // 005CDBD3..005CDBE5
            p += 0xb;                                                                // 005CDBED
            if (_strnicmp(p, "pwk_use", 7) == 0) p += 7;                             // 005CDBEB..005CDC02
            else if (_strnicmp(p, "pwk_dp_use_", 0xb) == 0) p += 0xb;                // 005CDC07..005CDC1F
            else continue;
            if (eq(p, "01", 2)) use_point = 1;                                       // 005CDC22..005CDC36
            else if (eq(p, "02", 2)) use_point = 2;                                  // 005CDC40..005CDC58
            continue;
        }
        if (eq(p, "position", 8)) {                                                  // 005CDC62..005CDC74
            (void)kotor_sscanf_006fae08(p + 8, "%f %f %f", &px, &py, &pz);           // 005CDC7A..005CDC92
            if (use_point == 0) {                                                    // 005CDC97..005CDCA0
                if (node_flag != 1) { live_free(line); return 0; }                   // 005CDCA2..005CDCA5 -> 005CDFF0
                wv(s + 0x2c, Vec{px, py, pz});                                       // 005CDCAB..005CDCD7
                continue;
            }
            if (use_point > 2) { live_free(line); return 0; }                        // 005CDCDF..005CDCE9 -> 005CDFF0
            const Vec point{px, py, pz};                                             // 005CDCEF..005CDD03 [esp+74]
            kotor_fastcall_005cda60(mesh, nullptr, static_cast<U>(use_point) - 1, &point); // 005CDD07..005CDD13
            use_point = 0;                                                           // 005CDD18
            continue;
        }
        if (node_flag != 1) continue;                                                // 005CDD22..005CDD25
        if (eq(p, "endnode", 7)) { in_node = 0; use_point = 0; continue; }           // 005CDD2B..005CDD47
        if (eq(p, "orientation", 0xb)) { orientation_line(p + 0xb); continue; }      // 005CDD4C..005CDDF6
        if (eq(p, "verts", 5) || eq(p, "vertices", 8)) {                             // 005CDDFB..005CDE25
            if (eq(p, "verts", 5)) p += 5;                                           // 005CDF24..005CDF38
            if (eq(p, "vertices", 8)) p += 8;                                        // 005CDF3B..005CDF4F
            (void)kotor_sscanf_006fae08(p, "%d %c", &verts, &tag);                   // 005CDF52..005CDF62
            kotor_fastcall_005969a0(mesh, nullptr, static_cast<U>(verts));           // 005CDF67..005CDF71
            for (int i = 0; i < verts; ++i) {                                        // 005CDF76..005CDFDD
                if (kotor_fastcall_005968a0(mesh, nullptr, &cursor, &remaining, line, 0x100) == 0)
                    return 0;                                                        // 005CDF82..005CDF9B -> 005CDFF9 (buffer leaked)
                (void)kotor_sscanf_006fae08(line, "%f %f %f", &vx, &vy, &vz);        // 005CDF9D..005CDFB2
                wv(rd(s + 0x54) + static_cast<U>(i) * 12, Vec{vx, vy, vz});          // 005CDFB7..005CDFD0
            }
            continue;
        }
        if (!eq(p, "faces", 5)) continue;                                            // 005CDE2B..005CDE3D
        (void)kotor_sscanf_006fae08(p + 5, "%d %c", &faces, &tag);                   // 005CDE43..005CDE56
        kotor_fastcall_00596900(mesh, nullptr, static_cast<U>(faces));               // 005CDE5B..005CDE65
        for (int i = 0; i < faces; ++i) {                                            // 005CDE6A..005CDF19
            if (kotor_fastcall_005968a0(mesh, nullptr, &cursor, &remaining, line, 0x100) == 0) {
                live_free(line); return 0;                                           // 005CDE80..005CDE99 -> 005CDFF0
            }
            int v0, v1, v2, s0, s1, s2, t0;                                          // [esp+5C] [esp+70] [esp+60] [esp+80] [esp+8C] [esp+88] [esp+84]
            (void)kotor_sscanf_006fae08(line, "%d %d %d %d %d %d %d %d", &v0, &v1, &v2, &s0, &s1, &s2, &t0,
                                        reinterpret_cast<int*>(ptr(rd(s + 0x64) + static_cast<U>(i) * 4))); // 005CDE9F..005CDEDE
            const U tri = rd(s + 0x60) + static_cast<U>(i) * 12;                     // 005CDEE3..005CDF0B
            wr(tri, static_cast<U>(v0)); wr(tri + 4, static_cast<U>(v1)); wr(tri + 8, static_cast<U>(v2));
        }
    }
    live_free(line);                                                                 // 005CDB34..005CDB35
    if (trimesh == 0) (void)kotor_m15e_slot00c_self_00641db0(ptr(s + 0xc), nullptr); // 005CDB3A..005CDB48
    return 1;                                                                        // 005CDB4D
}

// ===================================================================================== 005CE160
U __fastcall kotor_fastcall_005ce160(void* mesh, void*, char* data, U size) {
    const U s = word(mesh);
    char* const line = static_cast<char*>(live_operator_new(0x100));                 // 005CE16A..005CE176 EBP
    if (line == nullptr) return 0;                                                   // 005CE17D..005CE18D
    char* cursor = data;                                                             // [esp+2C]
    U remaining = size;                                                              // [esp+18]
    float px = 0.0f, py = 0.0f, pz = 0.0f;                                           // [esp+38] [esp+30] [esp+4C]
    float vx = 0.0f, vy = 0.0f, vz = 0.0f;                                           // [esp+54] [esp+58] [esp+5C]
    U hook = 0xffffffffu;                                                            // [esp+14]
    U selector = 0xffffffffu;                                                        // [esp+20]
    U in_node = 0;                                                                   // [esp+10]
    U trimesh = 0;                                                                   // [esp+1C]
    U walk = 0;                                                                      // [esp+28] (EBX at the loop top)
    int faces = 0, verts = 0;                                                        // [esp+24] [esp+34]
    char tag;                                                                        // [esp+4B] / [esp+53]
    const char* const name = reinterpret_cast<const char*>(s + 0x8c);                // door model name
    while (remaining != 0) {                                                         // 005CE1A5..005CE1EF / 005CE73E..005CE744
        const U walk_flag = walk;                                                    // 005CE21A EBX
        if (kotor_fastcall_005968a0(mesh, nullptr, &cursor, &remaining, line, 0x100) != 1) break; // 005CE220..005CE23A
        const char* p = skip_blanks(line);                                           // 005CE23C..005CE24B
        if (eq(p, "node", 4)) {                                                      // 005CE24D..005CE25F
            p += 4; in_node = 0;                                                     // 005CE265..005CE270
            if (eq(p, " trimesh ", 9)) {                                             // 005CE274..005CE27E
                p += 0xb;                                                            // 005CE282
                if (_strnicmp(p, "_DWK_wg_", 8) == 0) {                              // 005CE280..005CE295
                    p += 8;                                                          // 005CE2A0
                    hook = eq(p, name, 5) ? rd(s + 0x88) : 0xffffffffu;              // 005CE297..005CE2BC
                    in_node = 1; trimesh = 1; walk = 1;                              // 005CE2C4..005CE2D1
                    continue;
                }
            }
            if (eq(p, " dummy ", 7)) {                                               // 005CE2DA..005CE2EC
                p += 9;                                                              // 005CE2F4
                if (_strnicmp(p, "_DWK_dp_", 8) == 0) {                              // 005CE2F2..005CE307
                    walk = 0; in_node = 1; trimesh = 1;                              // 005CE30D..005CE327
                    p += 8;                                                          // 005CE31E
                    if (eq(p, name, 5)) {                                            // 005CE321..005CE335
                        hook = rd(s + 0x88);                                         // 005CE337..005CE33F
                        p += std::strlen(name);                                      // 005CE33D..005CE34F
                    }
                    if (eq(p, "_01", 3)) selector = 0;                               // 005CE351..005CE365
                    else if (eq(p, "_02", 3)) selector = 1;                          // 005CE36E..005CE386
                    continue;
                }
            }
        }
        if (eq(p, "endnode", 7)) { in_node = 0; hook = 0xffffffffu; continue; }      // 005CE393..005CE3B3
        if (in_node != 1) continue;                                                  // 005CE3B8..005CE3BD
        if (hook == 0xffffffffu) continue;                                           // 005CE3C3..005CE3C8
        if (eq(p, "position", 8)) {                                                  // 005CE3CE..005CE3E0
            (void)kotor_sscanf_006fae08(p + 8, "%f %f %f", &px, &py, &pz);           // 005CE3E6..005CE3FE
            if (walk_flag != 0) wv(s + 0x2c, Vec{px, py, pz});                       // 005CE406..005CE448
            else kotor_fastcall_005ce070(mesh, nullptr, selector, Vec{px, py, pz});  // 005CE450..005CE491
            continue;
        }
        if (walk_flag == 0) continue;                                                // 005CE49B..005CE49D
        if (eq(p, "orientation", 0xb)) { orientation_line(p + 0xb); continue; }      // 005CE4A3..005CE54D
        if (eq(p, "verts", 5) || eq(p, "vertices", 8)) {                             // 005CE552..005CE57C
            if (eq(p, "verts", 5)) p += 5;                                           // 005CE683..005CE697
            if (eq(p, "vertices", 8)) p += 8;                                        // 005CE69A..005CE6AE
            (void)kotor_sscanf_006fae08(p, "%d %c", &verts, &tag);                   // 005CE6B1..005CE6C1
            kotor_fastcall_005969a0(mesh, nullptr, static_cast<U>(verts));           // 005CE6C6..005CE6D0
            for (int i = 0; i < verts; ++i) {                                        // 005CE6D5..005CE73C
                if (kotor_fastcall_005968a0(mesh, nullptr, &cursor, &remaining, line, 0x100) == 0) {
                    (void)reinterpret_cast<U(__thiscall*)(void*)>(rd(rd(s)))(mesh);  // 005CE74F..005CE753 slot 0
                    live_free(line); return 1;                                       // 005CE755..005CE766
                }
                (void)kotor_sscanf_006fae08(line, "%f %f %f", &vx, &vy, &vz);        // 005CE6FC..005CE711
                wv(rd(s + 0x54) + static_cast<U>(i) * 12, Vec{vx, vy, vz});          // 005CE716..005CE72F
            }
            continue;
        }
        if (!eq(p, "faces", 5)) continue;                                            // 005CE582..005CE594
        (void)kotor_sscanf_006fae08(p + 5, "%d %c", &faces, &tag);                   // 005CE59A..005CE5AD
        kotor_fastcall_00596900(mesh, nullptr, static_cast<U>(faces));               // 005CE5B2..005CE5BC
        for (int i = 0; i < faces; ++i) {                                            // 005CE5C1..005CE678
            if (kotor_fastcall_005968a0(mesh, nullptr, &cursor, &remaining, line, 0x100) == 0) {
                live_free(line); return 1;                                           // 005CE5D1..005CE5EA -> 005CE755
            }
            int v0, v1, v2, s0, s1, s2, t0, material;                                // [esp+70] [esp+64] [esp+6C] [esp+8C] [esp+98] [esp+94] [esp+90] [esp+60]
            (void)kotor_sscanf_006fae08(line, "%d %d %d %d %d %d %d %d", &v0, &v1, &v2, &s0, &s1, &s2, &t0, &material); // 005CE5F0..005CE630
            wr(rd(s + 0x64) + static_cast<U>(i) * 4, static_cast<U>(material));      // 005CE635..005CE63F
            const U tri = rd(s + 0x60) + static_cast<U>(i) * 12;                     // 005CE642..005CE66A
            wr(tri, static_cast<U>(v0)); wr(tri + 4, static_cast<U>(v1)); wr(tri + 8, static_cast<U>(v2));
        }
    }
    live_free(line);                                                                 // 005CE1F1..005CE1F2
    if (trimesh == 0) (void)reinterpret_cast<U(__thiscall*)(void*)>(rd(rd(s)))(mesh); // 005CE1F7..005CE206 slot 0
    return 1;                                                                        // 005CE208
}

// ===================================================================================== 00596C10
void __fastcall kotor_fastcall_00596c10(void* mesh, void*) {
    const U s = word(mesh);
    if (rd(s + 0x4c) != 0) return;                                                   // 00596C16..00596C1B
    for (int t = 0; t < static_cast<int>(rd(s + 0x58)); ++t) {                       // 00596C21..00596CF5
        const U tri = rd(s + 0x60) + static_cast<U>(t) * 12;                         // 00596C33..00596C44
        const U verts = rd(s + 0x54);
        const Vec a = rv(verts + rd(tri) * 12);                                      // 00596C36..00596C54 [esp+10]
        const Vec b = rv(verts + rd(tri + 4) * 12);                                  // 00596C58..00596C71 [esp+34]
        const Vec c = rv(verts + rd(tri + 8) * 12);                                  // 00596C75..00596C97 [esp+28]
        Vec n;                                                                       // [esp+1C]
        (void)normal_of_004aca00(&n, &a, &b, &c);                                    // 00596C8E..00596CA6
        wv(rd(s + 0x68) + static_cast<U>(t) * 12, n);                                // 00596CAB..00596CC1
        const double d = (static_cast<double>(a.z) * n.z + static_cast<double>(a.y) * n.y)
                         + static_cast<double>(a.x) * n.x;                           // 00596CC4..00596CE8
        wf(rd(s + 0x6c) + static_cast<U>(t) * 4, static_cast<float>(-d));            // 00596CEA..00596CEC
    }
}

// ===================================================================================== 00580BE0 / 00580C60
namespace {
U edge_vertex(U s, U edge) {                                                         // vertex of perimeter edge e
    const U e = rd(rd(s + 0x8c) + edge * 8);
    return rd(s + 0x54) + rd(rd(s + 0x60) + e * 4) * 12;
}
}
U __fastcall kotor_fastcall_00580be0(void* mesh, void*, U edge, Vec* out) {
    const U s = word(mesh);
    if (static_cast<int>(edge) >= static_cast<int>(rd(s + 0x90))) {                  // 00580BE0..00580BEF
        *out = Vec{0.0f, 0.0f, 0.0f}; return 0;                                      // 00580C28..00580C58
    }
    *out = rv(edge_vertex(s, edge)); return 1;                                       // 00580BF1..00580C1D
}
U __fastcall kotor_fastcall_00580c60(void* mesh, void*, U edge, Vec* first, Vec* second) {
    const U s = word(mesh);
    const U count = rd(s + 0x90);
    if (static_cast<int>(edge) >= static_cast<int>(count)) {                         // 00580C60..00580C70
        *first = Vec{0.0f, 0.0f, 0.0f}; *second = Vec{0.0f, 0.0f, 0.0f}; return 0;  // 00580CE7..00580D47
    }
    *first = rv(edge_vertex(s, edge));                                               // 00580C72..00580C9D
    const U next = edge + 1 == count ? 0 : edge + 1;                                 // 00580CA0..00580CAE
    *second = rv(edge_vertex(s, next));                                              // 00580CB0..00580CD9
    return 1;
}

// ===================================================================================== 00580D50
// Snaps every perimeter vertex of `other` within sqrt(tolerance) of a perimeter vertex of this mesh onto it.
U __fastcall kotor_fastcall_00580d50(void* mesh, void*, void* other, U tolerance_bits, U, U) {
    const U s = word(mesh), o = word(other);
    float tolerance; std::memcpy(&tolerance, &tolerance_bits, 4);
    if (rd(s + 0x4c) != 0) return 0;                                                 // 00580D53..00580D67
    U welded = 0;                                                                    // [esp+4]
    for (int i = 0; i < static_cast<int>(rd(s + 0x90)); ++i) {                       // 00580D6A..00580E7A
        Vec a{0.0f, 0.0f, 0.0f};                                                     // [esp+1C]
        (void)kotor_fastcall_00580be0(mesh, nullptr, static_cast<U>(i), &a);          // 00580DC0..00580DC6
        int count = static_cast<int>(rd(o + 0x90));                                  // 00580DCB EBX
        if (count <= 0) continue;                                                    // 00580DD3..00580DD5
        const U ay = fbits(a.y);                                                     // 00580DDB EBP
        for (int j = 0; j < count; ) {                                               // 00580DE0..00580E5F
            Vec b{0.0f, 0.0f, 0.0f};                                                 // [esp+28]
            (void)kotor_fastcall_00580be0(other, nullptr, static_cast<U>(j), &b);    // 00580DE0..00580DE8
            const double dx = static_cast<double>(a.x) - b.x;                        // 00580DED..00580DF1
            const double dy = static_cast<double>(a.y) - b.y;                        // 00580DF5..00580DF9
            const double dz = static_cast<double>(a.z) - b.z;                        // 00580DFD..00580E01
            const double d2 = (dz * dz + dy * dy) + dx * dx;                         // 00580E05..00580E13
            if (d2 < static_cast<double>(tolerance)) {                               // 00580E15..00580E24 (C0 only)
                if (j < count) {                                                     // 00580E26..00580E28
                    const U v = edge_vertex(o, static_cast<U>(j));                   // 00580E2A..00580E3F
                    wf(v, a.x); wr(v + 4, ay); wf(v + 8, a.z);                       // 00580E42..00580E4F
                }
                ++welded;                                                            // 00580E52
            }
            count = static_cast<int>(rd(o + 0x90));                                  // 00580E56
            ++j;                                                                     // 00580E5C
        }
    }
    return welded;                                                                   // 00580E83
}

// ===================================================================================== 00580E90
// Links coincident perimeter edges of two room meshes (both endpoints within 0.1): this edge -> this_room,
// other edge -> other_room. Loop ends are taken from the +9Ch loop-length table (cumulative counts).
U __fastcall kotor_fastcall_00580e90(void* mesh, void*, void* other, U other_room, U this_room, U, U) {
    const U s = word(mesh), o = word(other);
    constexpr double k_limit_00743310 = 0.01;                                        // [00743310] qword 0.01
    if (rd(s + 0x4c) != 0) return 0;                                                 // 00580E96..00580EAB
    U linked = 0;                                                                    // [esp+1C]
    Vec a{0, 0, 0}, b{0, 0, 0}, c{0, 0, 0}, d{0, 0, 0}, f{0, 0, 0};                  // [54] [3C] [60] [30] [48]
    U loop_off = 0;                                                                  // [esp+10]
    for (int i = 0; i < static_cast<int>(rd(s + 0x90)); ++i) {                       // 00580EAE..0058113B
        (void)kotor_fastcall_00580c60(mesh, nullptr, static_cast<U>(i), &a, &b);      // 00580F43..00580F51
        if (i == 0) f = a;                                                           // 00580F56..00580F6E
        if (i == static_cast<int>(rd(rd(s + 0x9c) + loop_off)) - 1) {                // 00580F72..00580F82
            loop_off += 4;                                                           // 00580F90..00580F93
            const Vec old = b; b = f; f = old;                                       // 00580F84..00580FB7
        }
        Vec e{0, 0, 0};                                                              // [esp+6C] (EDI EBX EBP)
        U other_off = 0;                                                             // [esp+14]
        for (int j = 0; j < static_cast<int>(rd(o + 0x90)); ++j) {                   // 00580FBB..0058111E
            (void)kotor_fastcall_00580c60(other, nullptr, static_cast<U>(j), &c, &d); // 00580FF3..00581005
            if (j == 0) e = c;                                                       // 0058100A..00581016
            if (j == static_cast<int>(rd(rd(o + 0x9c) + other_off)) - 1) {           // 0058101A..0058102E
                other_off += 4;                                                      // 0058103A..0058103D
                const Vec old = d; d = e; e = old;                                   // 00581030..00581055
            }
            const double dx = static_cast<double>(a.x) - d.x;                        // 00581057..0058105B
            const double dy = static_cast<double>(a.y) - d.y;                        // 0058105F..00581063
            const double dz = static_cast<double>(a.z) - d.z;                        // 00581067..0058106B
            const float ex = static_cast<float>(static_cast<double>(b.x) - c.x);     // 0058106F..00581077 [esp+28]
            const float ey = static_cast<float>(static_cast<double>(b.y) - c.y);     // 0058107B..00581083 [esp+24]
            const float ez = static_cast<float>(static_cast<double>(b.z) - c.z);     // 00581087..0058108F [esp+20]
            if (!((dz * dz + dy * dy) + dx * dx < k_limit_00743310)) continue;       // 00581093..005810B4
            const double e2 = (static_cast<double>(ez) * ez + static_cast<double>(ey) * ey)
                              + static_cast<double>(ex) * ex;                        // 005810B6..005810D0
            if (!(e2 < k_limit_00743310)) continue;                                  // 005810D2..005810DD
            wr(rd(s + 0x8c) + static_cast<U>(i) * 8 + 4, this_room);                 // 005810DF..005810F4
            wr(rd(o + 0x8c) + static_cast<U>(j) * 8 + 4, other_room);                // 005810F8..00581109
            ++linked;                                                                // 0058110D
        }
    }
    return linked;                                                                   // 00581142
}
