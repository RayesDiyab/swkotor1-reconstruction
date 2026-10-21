// f3_mdl_node.cpp - lane-m46-3dmodel (fork F3): MdlNode vertex generation / temporary-array removal and the
// MaxTree/Model ASCII field parsers of the GOG swkotor.exe (image base 0x400000), plus the NodeVertex family and
// list helpers reached only from them. Static reading of the original instructions (lane-sound/full.asm dumpbin);
// every behaviour cites its instruction addresses. Objects are raw observed-offset storage (model_load.hpp get/put).
//
// Conventions: original __thiscall bodies exported as __fastcall(self, unused_edx, ...) (D1; callees never read
// EDX). Allocator family: 006FA7E6 operator new / 006FC0D2 jmp thunk -> live_operator_new; 006FA390 free /
// 006FC0CD jmp thunk -> live_free (M41). Original operator new returns NULL (D30-FINAL); the `block ? ctor : 0`
// shape below reproduces the original null tests. EH states (00439DF0 state 0, 0043BD40 state 1, 0043D450 states
// 2/3, 004885E0 state 1, 004791E0 states 0..2) only delete the new block / destroy local lists when a callee
// throws; no callee here throws (NodeVertex ctors, list helpers, live_* do not throw), so the unwind is cited, not
// emitted (D30-FINAL /EHsc extern "C" rule: no throwing callee).
// Floats: every vertex/vector component is moved as a raw DWORD (mov), except the dangly constraint copy
// 0043BF81/0043BF86 + 0043BFDE/0043BFE3 (fld/fstp dword, float->float: value-preserving; an SNaN would be quieted
// by x87 - reproduced as a float copy, recorded) and the dangly equality fld/fcomp 00478952..0047895D (float
// compare, NaN unequal).
#include "134d543bb3-model_load.hpp"
#include <initializer_list>

using namespace aurora_model;

extern "C" {
// ---- existing providers (M45 objects; declarations repeated with identical extern "C" decoration)
std::int32_t __cdecl kotor_strncmp_006faff0(const char* first, const char* second, U count);     // tlk-parse (M45)
char* __cdecl firstword_00463530(char* line);                                                    // font-txi-readers (M45)
int __cdecl kotor_sscanf_006fae08(const char* buffer, const char* format, ...);                  // font-txi-readers (M45; UCRT-backed per its packet)
U __cdecl aurora_vector_equal_004aa980(const void* left, const void* right);                     // gui-writers-vector-equal (M44/M45)
void __fastcall vector_list_allocate_00421310(List* list, void* unused_edx, I size);             // font-info-parse (M45; RET 4)
// 3D-core / construct owner (unresolved in M45 as @float_list_assign_00438a50@12; not authored here):
List* __fastcall float_list_assign_00438a50(List* self, void* unused_edx, const List* source);
// ParseNode 004680E0 (ASCII MDL node parser, 563 instructions, 44 callees; not on the binary-MDL menu path):
// NOT authored here - new name, open gate (callees_new_names).
void __cdecl aurora_parse_node_004680e0(char* text, MaxTree* tree);

// ---- authored here (reached only from the bodies in this unit; exe-wide call scan in frag/F3.json)
// BSS cell 00830238: NodeVertex construction statistic; only 004787C4/004787CE read/write it.
volatile U aurora_node_vertex_count_00830238 = 0;
void* __fastcall aurora_node_vertex_construct_00478680(void* self, void* unused_edx, const void* position, const void* normal,
                                                       const void* tex0, const void* tex1, const void* tex2,
                                                       const void* vector3c, const void* vector48);
void* __fastcall aurora_node_vertex_dangly_construct_004787e0(void* self, void* unused_edx, const void* position, const void* normal,
                                                              const void* tex0, const void* tex1, const void* tex2,
                                                              const void* vector3c, const void* vector48, U constraint_bits);
void* __fastcall aurora_node_vertex_anim_construct_004791e0(void* self, void* unused_edx, const void* position, const void* normal,
                                                            const void* tex0, const void* tex1, const void* tex2,
                                                            const void* vector3c, const void* vector48,
                                                            const List* list58, const List* list64, const List* list70);
void __fastcall aurora_node_vertex_anim_destroy_0043c1b0(void* self, void* unused_edx);
void* __fastcall aurora_node_vertex_as_anim_00478650(void* self, void* unused_edx);
void* __fastcall aurora_node_vertex_as_dangly_00478660(void* self, void* unused_edx);
U8 __fastcall aurora_node_vertex_equal_00478820(void* self, void* unused_edx, const void* other);
U8 __fastcall aurora_node_vertex_dangly_equal_004788c0(void* self, void* unused_edx, const void* other);
List* __fastcall aurora_vector_list_assign_00479150(List* self, void* unused_edx, const List* source);
List* __fastcall aurora_skin_weight_list_assign_004880d0(List* self, void* unused_edx, const List* source);
void __fastcall aurora_skin_weight_list_allocate_0043f6b0(List* self, void* unused_edx, I capacity);
}

namespace {
constexpr char kNode_00742a8c[] = "node";     // .rdata 00742A8C (read from the pinned image)
constexpr char kRadius_00742aa4[] = "radius"; // .rdata 00742AA4
constexpr char kBmin_00742a9c[] = "bmin";     // .rdata 00742A9C
constexpr char kBmax_00742a94[] = "bmax";     // .rdata 00742A94
constexpr char kF_0073e2e4[] = "%f";          // .rdata 0073E2E4
constexpr char kF3_0073ee40[] = "%f %f %f";   // .rdata 0073EE40

void* vp(U a) { return ptr<void>(a); }
U post_of(MdlNode* n) { return get<U>(n, 0x134); }                    // node+134: PostProcess temporaries object
U face_record(U post, I f) { return get<U>(vp(post), 0x48) + static_cast<U>(f) * 0x14; }   // post+48, 0x14 stride
I index_at(U array, I i) { return get<I>(vp(array), static_cast<U>(i) * 4); }
U vec12(U base, I index) { return base + static_cast<U>(index) * 12; }   // lea r,[i+i*2]; lea r,[base+r*4]
List* list_at(void* p, U off) { return reinterpret_cast<List*>(at(p, off)); }

// Vertex attribute pointers shared by the TriMesh / Dangly / Skin bodies (00439E50..00439F1D, 0043BDB0..0043BE4C,
// 00488642..004886DF): position post[0]; normal post[0C] if post[10]; tex0 post[18] if post[1C]; tex1 post[24] if
// post[28]; tex2 post[30] if post[34] (index arrays: face+00 -> position idx[k]; face+04 -> normal idx[k],
// tex0 idx[3+k], tex1 idx[6+k], tex2 idx[9+k]; face+10 -> post[54] idx[k]); post[54] vector always.
struct VertexPointers { U position, normal, tex0, tex1, tex2, vector3c; I position_index; };
VertexPointers vertex_pointers(U post, U face, I k) {
    VertexPointers v{};
    const U attr = get<U>(vp(face), 0x04);
    v.position_index = index_at(get<U>(vp(face), 0x00), k);
    v.position = vec12(get<U>(vp(post), 0x00), v.position_index);
    v.normal = get<U>(vp(post), 0x10) != 0 ? vec12(get<U>(vp(post), 0x0C), index_at(attr, k)) : 0;
    v.tex0 = get<U>(vp(post), 0x1C) != 0 ? vec12(get<U>(vp(post), 0x18), index_at(attr, 3 + k)) : 0;
    v.tex1 = get<U>(vp(post), 0x28) != 0 ? vec12(get<U>(vp(post), 0x24), index_at(attr, 6 + k)) : 0;
    v.tex2 = get<U>(vp(post), 0x34) != 0 ? vec12(get<U>(vp(post), 0x30), index_at(attr, 9 + k)) : 0;
    v.vector3c = vec12(get<U>(vp(post), 0x54), index_at(get<U>(vp(face), 0x10), k));
    return v;
}

// post+60 vertex pointer list append (00439FB6..0043A02D; dangly 0043BEEA..0043BF70; anim 0043D860..0043D8E5):
// count == capacity -> capacity = capacity ? 2*capacity : initial (stored before the allocation), new block,
// copy count entries (count re-read per entry), free the old block when non-null; then data[count] = v; ++count.
void append_word(List* l, U value, I initial) {
    if (l->size == l->capacity) {
        const I capacity = l->capacity != 0 ? l->capacity * 2 : initial;
        const U old = l->data;
        l->capacity = capacity;
        l->data = addr(live_operator_new(static_cast<U>(capacity) * 4));
        for (I i = 0; i < l->size; ++i) put<U>(vp(l->data), static_cast<U>(i) * 4, get<U>(vp(old), static_cast<U>(i) * 4));
        if (old != 0) live_free(vp(old));
    }
    put<U>(vp(l->data), static_cast<U>(l->size) * 4, value);
    l->size = l->size + 1;
}
// face corner word store: node[58] + 1A + face*20 + k*2 (u16) = value
void store_corner(MdlNode* node, I f, I k, U value) {
    put<U16>(vp(get<U>(node, 0x58)), 0x1A + static_cast<U>(f) * 0x20 + static_cast<U>(k) * 2, static_cast<U16>(value));
}
// CExoArrayList<Vector> append with 00421310 growth (0043D596..0043D5E2 / 0043D602..0043D64D / 0043D6A0..0043D6EA):
// the source vector is loaded before the capacity test.
void append_vector(List* l, U source) {
    const U x = get<U>(vp(source), 0), y = get<U>(vp(source), 4), z = get<U>(vp(source), 8);
    if (l->size == l->capacity) vector_list_allocate_00421310(l, nullptr, l->capacity != 0 ? l->capacity * 2 : 0x10);
    const U dst = vec12(l->data, l->size);
    put<U>(vp(dst), 0, x); put<U>(vp(dst), 4, y); put<U>(vp(dst), 8, z);
    l->size = l->size + 1;
}
void free_list_storage(List* l) {   // "if (data) { free(data); data = 0; } capacity = count = 0" (006FC0CD)
    if (l->data != 0) { live_free(vp(l->data)); l->data = 0; }
}
} // namespace

extern "C" {

// ================================================================ NodeVertex family (only callers: this unit)
// 00478680 NodeVertex::NodeVertex(pos, normal, tex0, tex1, tex2, v3c, v48), RET 1Ch, EAX = this. Layout (0x58):
// +00 position, +0C normal, +18 tex0, +24 tex1, +30 tex2, +3C v3c, +48 v48 (default 1,1,1), +54 u8 present
// mask, +55 u8 kind (0 flat, 1 anim, 2 dangly).
void* __fastcall aurora_node_vertex_construct_00478680(void* self, void*, const void* position, const void* normal,
                                                       const void* tex0, const void* tex1, const void* tex2,
                                                       const void* vector3c, const void* vector48) {
    for (U off = 0; off <= 0x50; off += 4) put<U>(self, off, 0);                   // 00478687..004786C2
    put<U>(self, 0x48, 0x3F800000u);                                                // 004786CB..004786D9 (1.0f)
    put<U>(self, 0x4C, 0x3F800000u);                                                // 004786DB..004786E7
    put<U8>(self, 0x55, 0);                                                         // 004786EA
    put<U8>(self, 0x54, 0);                                                         // 004786ED
    put<U>(self, 0x50, 0x3F800000u);                                                // 004786F0..004786FC
    for (U i = 0; i < 12; i += 4) put<U>(self, 0x00 + i, get<U>(position, i));      // 004786FF..00478712
    for (U i = 0; i < 12; i += 4) put<U>(self, 0x3C + i, get<U>(vector3c, i));      // 00478715..00478727 (unconditional)
    if (normal != nullptr) {                                                        // 0047872A..00478730
        for (U i = 0; i < 12; i += 4) put<U>(self, 0x0C + i, get<U>(normal, i));    // 00478732..00478740
        put<U8>(self, 0x54, 1);                                                     // 00478743 (store, not or)
    }
    if (tex0 != nullptr) {                                                          // 00478747..0047874D
        for (U i = 0; i < 12; i += 4) put<U>(self, 0x18 + i, get<U>(tex0, i));      // 0047874F..00478763
        put<U8>(self, 0x54, static_cast<U8>(get<U8>(self, 0x54) | 2));              // 00478751/00478760/00478766
    }
    if (tex1 != nullptr) {                                                          // 00478769..0047876F
        for (U i = 0; i < 12; i += 4) put<U>(self, 0x24 + i, get<U>(tex1, i));      // 00478771..00478785
        put<U8>(self, 0x54, static_cast<U8>(get<U8>(self, 0x54) | 4));              // 00478773/00478782/00478788
    }
    if (tex2 != nullptr) {                                                          // 0047878B..00478796
        for (U i = 0; i < 12; i += 4) put<U>(self, 0x30 + i, get<U>(tex2, i));      // 00478798..004787A9
        put<U8>(self, 0x54, static_cast<U8>(get<U8>(self, 0x54) | 8));              // 004787A6
    }
    if (vector48 != nullptr)                                                        // 004787AC..004787B2
        for (U i = 0; i < 12; i += 4) put<U>(self, 0x48 + i, get<U>(vector48, i));  // 004787B4..004787C1
    aurora_node_vertex_count_00830238 = aurora_node_vertex_count_00830238 + 8;     // 004787C4..004787CE (ebx = 8 set
    return self;                                                                    //  unconditionally at 00478791)
}
// 004787E0 NodeVertexDangly::NodeVertexDangly(7 NodeVertex args, constraint), RET 20h: base ctor; +55 = 2;
// +58 = constraint DWORD; EAX = this.
void* __fastcall aurora_node_vertex_dangly_construct_004787e0(void* self, void*, const void* position, const void* normal,
                                                              const void* tex0, const void* tex1, const void* tex2,
                                                              const void* vector3c, const void* vector48, U constraint_bits) {
    aurora_node_vertex_construct_00478680(self, nullptr, position, normal, tex0, tex1, tex2, vector3c, vector48); // 004787E0..00478808
    put<U8>(self, 0x55, 2);                                                         // 00478811
    put<U>(self, 0x58, constraint_bits);                                            // 0047880D/00478815
    return self;                                                                    // 00478818
}
// 00479150 CExoArrayList<Vector>::operator=(source), RET 4, EAX = this: count = 0; append every source vector
// (loaded first), growth capacity ? 2*capacity : 8 through 00421310.
List* __fastcall aurora_vector_list_assign_00479150(List* self, void*, const List* source) {
    self->size = 0;                                                                 // 0047915E
    for (I i = 0; i < source->size; ++i) {                                          // 00479161..004791D0 (size re-read)
        const U src = vec12(source->data, i);
        const U x = get<U>(vp(src), 0), y = get<U>(vp(src), 4), z = get<U>(vp(src), 8);   // 00479170..00479186
        if (self->size == self->capacity)                                           // 0047917C..0047918A
            vector_list_allocate_00421310(self, nullptr, self->capacity != 0 ? self->capacity * 2 : 8); // 0047918C..0047919C
        const U dst = vec12(self->data, self->size);                                // 004791A1..004791A9
        put<U>(vp(dst), 0, x); put<U>(vp(dst), 4, y); put<U>(vp(dst), 8, z);       // 004791B4..004791B9
        self->size = self->size + 1;                                                // 004791BC
    }
    return self;                                                                    // 004791D4
}
// 004791E0 NodeVertexAnim::NodeVertexAnim(7 NodeVertex args, list58, list64, list70), RET 28h: base ctor; three
// empty Vector lists at +58/+64/+70 ({data,count,cap} zeroed, data last); +55 = 1; each list = its source (00479150).
void* __fastcall aurora_node_vertex_anim_construct_004791e0(void* self, void*, const void* position, const void* normal,
                                                            const void* tex0, const void* tex1, const void* tex2,
                                                            const void* vector3c, const void* vector48,
                                                            const List* list58, const List* list64, const List* list70) {
    aurora_node_vertex_construct_00478680(self, nullptr, position, normal, tex0, tex1, tex2, vector3c, vector48); // 004791F6..00479224
    for (U base : {0x58u, 0x64u, 0x70u}) { put<U>(self, base + 4, 0); put<U>(self, base + 8, 0); put<U>(self, base, 0); } // 00479229..0047924E
    put<U8>(self, 0x55, 1);                                                         // 0047925A
    aurora_vector_list_assign_00479150(list_at(self, 0x58), nullptr, list58);       // 00479250..0047925E
    aurora_vector_list_assign_00479150(list_at(self, 0x64), nullptr, list64);       // 00479263..0047926A
    aurora_vector_list_assign_00479150(list_at(self, 0x70), nullptr, list70);       // 0047926F..00479276
    return self;                                                                    // 00479280
}
// 0043C1B0 NodeVertexAnim::~NodeVertexAnim (non-deleting): clear lists +58/+64/+70 (free data via 006FC0CD and
// zero it, then cap = count = 0); then the three member destructors (+70, +64, +58: free data when non-null -
// re-read, now null).
void __fastcall aurora_node_vertex_anim_destroy_0043c1b0(void* self, void*) {
    for (U base : {0x58u, 0x64u, 0x70u}) {                                          // 0043C1C9..0043C213
        if (get<U>(self, base) != 0) { live_free(vp(get<U>(self, base))); put<U>(self, base, 0); }
        put<U>(self, base + 8, 0); put<U>(self, base + 4, 0);
    }
    for (U base : {0x70u, 0x64u, 0x58u})                                            // 0043C216..0043C243
        if (get<U>(self, base) != 0) live_free(vp(get<U>(self, base)));
}
// 00478650 NodeVertex::AsNodeVertexAnim: kind byte +55 == 1 ? this : 0 (setne/dec/and).
void* __fastcall aurora_node_vertex_as_anim_00478650(void* self, void*) {
    return get<U8>(self, 0x55) == 1 ? self : nullptr;
}
// 00478660 NodeVertex::AsNodeVertexDangly: +55 == 2 ? this : 0.
void* __fastcall aurora_node_vertex_as_dangly_00478660(void* self, void*) {
    return get<U8>(self, 0x55) == 2 ? self : nullptr;
}
// 00478820 NodeVertex::operator==(other), RET 4, AL: Vector equality (004AA980(other+o, this+o)) for o = 00, 0C,
// 18, 24, 30, 3C, 48 in that order, first failure -> 0.
U8 __fastcall aurora_node_vertex_equal_00478820(void* self, void*, const void* other) {
    for (U off = 0; off <= 0x48; off += 0x0C)                                        // 00478822..004788AC
        if (aurora_vector_equal_004aa980(static_cast<const unsigned char*>(other) + off, at(self, off)) == 0) return 0;  // 004788B5
    return 1;                                                                       // 004788AF
}
// 004788C0 NodeVertexDangly::operator==(other), RET 4: the seven vector tests, then float other+58 == this+58
// (fld/fcomp/fnstsw; test ah,44h; jp -> unequal unless only C3: ordered equality).
U8 __fastcall aurora_node_vertex_dangly_equal_004788c0(void* self, void*, const void* other) {
    for (U off = 0; off <= 0x48; off += 0x0C)                                        // 004788C2..00478950
        if (aurora_vector_equal_004aa980(static_cast<const unsigned char*>(other) + off, at(self, off)) == 0) return 0;  // 00478966
    return get<float>(other, 0x58) == get<float>(self, 0x58) ? 1 : 0;               // 00478952..0047895F
}
// 0043F6B0 CExoArrayList<SkinVertexWeight>::allocate(capacity), RET 4 (0x34-byte elements): capacity stored
// first; new block (006FA7E6); copy count records; free the old block (006FA390) when non-null.
void __fastcall aurora_skin_weight_list_allocate_0043f6b0(List* self, void*, I capacity) {
    self->capacity = capacity;                                                      // 0043F6B7
    const U old = self->data;                                                       // 0043F6BE
    self->data = addr(live_operator_new(static_cast<U>(capacity) * 0x34));          // 0043F6BA..0043F6C6
    for (I i = 0; i < self->size; ++i)                                              // 0043F6C8..0043F6EF (count re-read)
        std::memcpy(at(vp(self->data), static_cast<U>(i) * 0x34), at(vp(old), static_cast<U>(i) * 0x34), 0x34);
    if (old != 0) live_free(vp(old));                                               // 0043F6F3..0043F6F8
}
// 004880D0 CExoArrayList<SkinVertexWeight>::operator=(source), RET 4, EAX = this: count = 0; per source record
// (copied to a local first): grow when count == capacity (capacity ? 2*capacity : 8 via 0043F6B0), store, ++count.
List* __fastcall aurora_skin_weight_list_assign_004880d0(List* self, void*, const List* source) {
    self->size = 0;                                                                 // 004880DD
    for (I i = 0; i < source->size; ++i) {                                          // 004880E0..00488152 (size re-read)
        unsigned char record[0x34];
        std::memcpy(record, at(vp(source->data), static_cast<U>(i) * 0x34), 0x34);   // 004880F0..00488105
        if (self->size == self->capacity)                                           // 00488107..0048810A
            aurora_skin_weight_list_allocate_0043f6b0(self, nullptr, self->capacity != 0 ? self->capacity * 2 : 8); // 0048810C..0048811C
        std::memcpy(at(vp(self->data), static_cast<U>(self->size) * 0x34), record, 0x34);   // 00488121..0048813C
        self->size = self->size + 1;                                                // 0048813E
    }
    return self;                                                                    // 00488157
}

// ================================================================ MdlNode bodies (consumers: model_binary.cpp)
// 00439DF0 MdlNodeTriMesh::InternalGenVertices. For each PostProcess face f < post[4C] (re-read) and corner
// k < 3: build a 0x58 NodeVertex (position, normal, tex0..2, post[54] vector, post[3C] vector when post[40] > 0);
// if an equal vertex exists in post+60 (NodeVertex::operator==, first match i) free the new one and store i in the
// node face corner word, else store the current count and append. Then, unless the node is an AABB (0043E340),
// remove degenerate faces (two equal corner indices) from node[58]/node[5C], scanning from the last face down.
void __fastcall aurora_trimesh_gen_vertices_body_00439df0(MdlNode* self, void*) {
    for (I f = 0; f < get<I>(vp(post_of(self)), 0x4C); ++f) {                       // 00439E0B..00439E1F / 0043A062..0043A079
        for (I k = 0; k < 3; ++k) {                                                 // 0043A030..0043A049
            const U post = post_of(self);                                           // 00439E50
            const U face = face_record(post, f);                                    // 00439E56..00439E62
            const VertexPointers v = vertex_pointers(post, face, k);                // 00439E5D..00439F1D
            const U v48 = get<I>(vp(post), 0x40) > 0 ? vec12(get<U>(vp(post), 0x3C), v.position_index) : 0; // 00439F20..00439F2C
            void* const block = live_operator_new(0x58);                            // 00439F2E..00439F35
            void* const vertex = block != nullptr                                   // 00439F3C..00439F6A
                ? aurora_node_vertex_construct_00478680(block, nullptr, vp(v.position), vp(v.normal), vp(v.tex0),
                                                        vp(v.tex1), vp(v.tex2), vp(v.vector3c), vp(v48))   // 00439F48..00439F61
                : nullptr;
            List* const verts = list_at(vp(post_of(self)), 0x60);
            I found = -1;
            for (I i = 0; i < verts->size; ++i) {                                   // 00439F6C..00439F9F (count re-read)
                if (aurora_node_vertex_equal_00478820(vp(get<U>(vp(list_at(vp(post_of(self)), 0x60)->data), static_cast<U>(i) * 4)),
                                                      nullptr, vertex) != 0) { found = i; break; }   // 00439F83..00439F91
            }
            if (found >= 0) {                                                       // 00439FD1..00439FE5
                live_free(vertex);                                                  // 00439FD1..00439FD2 (006FA390)
                store_corner(self, f, k, static_cast<U>(found));                    // 00439FD7..00439FE1
            } else {
                store_corner(self, f, k, static_cast<U>(list_at(vp(post_of(self)), 0x60)->size));   // 00439FA1..00439FB2
                append_word(list_at(vp(post_of(self)), 0x60), addr(vertex), 0x10);  // 00439FB6..0043A02D (thunks 006FC0D2/006FC0CD)
            }
        }
    }
    if (aurora_node_as_aabb_0043e340(self, nullptr) != nullptr) return;             // 0043A07F..0043A088
    I n = get<I>(self, 0x5C);                                                       // 0043A08A..0043A08F
    while (n != 0) {                                                                // 0043A0FF..0043A101
        --n;                                                                        // 0043A0A3/0043A0B2
        const U face = get<U>(self, 0x58) + static_cast<U>(n) * 0x20;               // 0043A0A0..0043A0B0
        const U16 a = get<U16>(vp(face), 0x1A), b = get<U16>(vp(face), 0x1C);       // 0043A0A6..0043A0AB
        if (a == b || a == get<U16>(vp(face), 0x1E) || b == get<U16>(vp(face), 0x1E)) {   // 0043A0B3..0043A0C8
            put<I>(self, 0x5C, get<I>(self, 0x5C) - 1);                             // 0043A0CA..0043A0D4
            for (I j = n; j < get<I>(self, 0x5C); ++j) {                            // 0043A0D0..0043A0F9 (count re-read)
                const U dst = get<U>(self, 0x58) + static_cast<U>(j) * 0x20;
                std::memmove(vp(dst), vp(dst + 0x20), 0x20);                        // 0043A0E0..0043A0EE (rep movs 8 dwords)
            }
        }
    }
}

// 0043C130 MdlNodeDanglyMesh::InternalRemoveTemporaryArrays: free (006FA390) every post+60 entry (count re-read;
// node[134] is NOT null-tested here), free post+60 storage (006FC0CD) and zero it, [+68] = [+64] = 0, then tail
// jump 0043C090.
void __fastcall aurora_dangly_remove_temporary_body_0043c130(MdlNode* self, void*) {
    for (I i = 0; i < get<I>(vp(post_of(self)), 0x64); ++i)                         // 0043C134..0043C15E
        live_free(vp(get<U>(vp(get<U>(vp(post_of(self)), 0x60)), static_cast<U>(i) * 4)));   // 0043C143..0043C14A
    List* const verts = list_at(vp(post_of(self)), 0x60);                          // 0043C160..0043C169
    free_list_storage(verts);                                                       // 0043C16C..0043C179
    verts->capacity = 0; verts->size = 0;                                           // 0043C182/0043C189
    aurora_trimesh_remove_temporary_body_0043c090(self, nullptr);                   // 0043C17F/0043C191 (jmp)
}

// 0043C320 MdlNodeAnimMesh::InternalRemoveTemporaryArrays: each non-null post+60 entry is a NodeVertexAnim:
// ~NodeVertexAnim (0043C1B0) then free (006FA390); clear post+60; clear node lists +1A0, +1AC, +1B8 (free data via
// 006FC0CD when non-null and zero it, then capacity = count = 0); tail jump 0043C090.
void __fastcall aurora_anim_mesh_remove_temporary_body_0043c320(MdlNode* self, void*) {
    for (I i = 0; i < get<I>(vp(post_of(self)), 0x64); ++i) {                       // 0043C324..0043C35D
        void* const vertex = vp(get<U>(vp(get<U>(vp(post_of(self)), 0x60)), static_cast<U>(i) * 4));   // 0043C337..0043C33A
        if (vertex != nullptr) {                                                    // 0043C33D..0043C33F
            aurora_node_vertex_anim_destroy_0043c1b0(vertex, nullptr);              // 0043C341..0043C343
            live_free(vertex);                                                      // 0043C348..0043C349
        }
    }
    List* const verts = list_at(vp(post_of(self)), 0x60);                          // 0043C360..0043C369
    free_list_storage(verts); verts->capacity = 0; verts->size = 0;                 // 0043C36C..0043C37E
    for (U base : {0x1A0u, 0x1ACu, 0x1B8u}) {                                       // 0043C381..0043C3EB
        List* const l = list_at(self, base);
        free_list_storage(l); l->capacity = 0; l->size = 0;
    }
    aurora_trimesh_remove_temporary_body_0043c090(self, nullptr);                   // 0043C3F1/0043C3F5 (jmp)
}

// 0043BD40 MdlNodeDanglyMesh::InternalGenVertices: as TriMesh over node faces f < node[5C] (re-read) with a 0x5C
// NodeVertexDangly (7th arg 0, constraint = node[19C][position index] DWORD); matching through
// AsNodeVertexDangly + NodeVertexDangly::operator==; each appended vertex also appends its constraint (float copy)
// to a local float list (growth capacity ? 2*capacity : 16). Afterwards node+19C is cleared (free 006FC0CD, zero,
// capacity = count = 0) and assigned from the local list (00438A50); the local storage is freed. No degenerate
// face pass.
void __fastcall aurora_dangly_gen_vertices_body_0043bd40(MdlNode* self, void*) {
    U local_data = 0; I local_count = 0, local_capacity = 0;                        // 0043BD63..0043BD6B / [esp+38h..40h]
    for (I f = 0; f < get<I>(self, 0x5C); ++f) {                                    // 0043BD6F..0043BD7A / 0043C016..0043C034
        for (I k = 0; k < 3; ++k) {                                                 // 0043BFEB..0043C004
            const U post = post_of(self);                                           // 0043BDB0..0043BDB4
            const U face = face_record(post, f);
            const VertexPointers v = vertex_pointers(post, face, k);                // 0043BDBA..0043BE4C
            void* const block = live_operator_new(0x5C);                            // 0043BE4A..0043BE4F
            void* const vertex = block != nullptr                                   // 0043BE57..0043BE91
                ? aurora_node_vertex_dangly_construct_004787e0(block, nullptr, vp(v.position), vp(v.normal), vp(v.tex0),
                      vp(v.tex1), vp(v.tex2), vp(v.vector3c), nullptr,
                      get<U>(vp(get<U>(self, 0x19C)), static_cast<U>(v.position_index) * 4))   // 0043BE64..0043BE88
                : nullptr;
            I found = -1;
            for (I i = 0; i < list_at(vp(post_of(self)), 0x60)->size; ++i) {       // 0043BE93..0043BED3 (count re-read)
                void* const entry = vp(get<U>(vp(list_at(vp(post_of(self)), 0x60)->data), static_cast<U>(i) * 4));
                void* const dangly = aurora_node_vertex_as_dangly_00478660(entry, nullptr);   // 0043BEB0..0043BEB6
                if (aurora_node_vertex_dangly_equal_004788c0(dangly, nullptr, vertex) != 0) { found = i; break; }   // 0043BEBB..0043BEC5
            }
            if (found >= 0) {
                live_free(vertex);                                                  // 0043BF05..0043BF06 (006FA390)
                store_corner(self, f, k, static_cast<U>(found));                    // 0043BF0B..0043BF15
                continue;
            }
            store_corner(self, f, k, static_cast<U>(list_at(vp(post_of(self)), 0x60)->size));   // 0043BED5..0043BEE6
            append_word(list_at(vp(post_of(self)), 0x60), addr(vertex), 0x10);      // 0043BEEA..0043BF70
            const float constraint = get<float>(vp(get<U>(self, 0x19C)), static_cast<U>(v.position_index) * 4);   // 0043BF73..0043BF86 (fld/fstp dword)
            if (local_count == local_capacity) {                                    // 0043BF7D..0043BF8A
                const U old = local_data;
                local_capacity = local_capacity != 0 ? local_capacity * 2 : 0x10;   // 0043BF8C..0043BF9D
                local_data = addr(live_operator_new(static_cast<U>(local_capacity) * 4));   // 0043BFA1..0043BFB1 (006FC0D2)
                for (I i = 0; i < local_count; ++i) put<U>(vp(local_data), static_cast<U>(i) * 4, get<U>(vp(old), static_cast<U>(i) * 4));   // 0043BFAD..0043BFC9
                if (old != 0) live_free(vp(old));                                   // 0043BFCB..0043BFD0 (006FC0CD)
            }
            ++local_count;                                                          // 0043BFE2/0043BFE7
            put<float>(vp(local_data), static_cast<U>(local_count - 1) * 4, constraint);   // 0043BFDE..0043BFE3
        }
    }
    List* const constraints = list_at(self, 0x19C);                                 // 0043C03F..0043C049
    free_list_storage(constraints);                                                 // 0043C04F..0043C05C
    constraints->capacity = 0; constraints->size = 0;                               // 0043C065/0043C068
    List local{};                                                                   // [esp+34h..3Ch] at 0043C05E
    local.data = local_data; local.size = local_count; local.capacity = local_capacity;
    float_list_assign_00438a50(constraints, nullptr, &local);                       // 0043C05E..0043C06B
    if (local_data != 0) live_free(vp(local_data));                                 // 0043C070..0043C075 (006FC0CD)
}

// 0043D450 MdlNodeAnimMesh::InternalGenVertices: node[1CC] = node[1A4] / post[4] and node[1D0] = node[1B0] /
// post[10] (cdq/idiv). Per node face f < node[5C] and corner k: three local Vector lists (A positions from node+1A0
// stride post[4]; B from node+1B8 stride post[58] with the post[54] index; C normals from node+1AC stride post[10])
// collect the per-frame vectors; a 0x7C NodeVertexAnim(position, normal, tex0..2, post[54] vector, post[3C] vector
// when post[40] > 0, A, C, B) is matched through AsNodeVertexAnim + NodeVertex::operator== (base 00478820); a
// match destroys (0043C1B0) and frees the new vertex; the locals are freed (B, C, A) per corner.
void __fastcall aurora_anim_mesh_gen_vertices_body_0043d450(MdlNode* self, void*) {
    put<I>(self, 0x1CC, get<I>(self, 0x1A4) / get<I>(vp(post_of(self)), 0x04));    // 0043D46B..0043D47B
    put<I>(self, 0x1D0, get<I>(self, 0x1B0) / get<I>(vp(post_of(self)), 0x10));    // 0043D481..0043D491
    for (I f = 0; f < get<I>(self, 0x5C); ++f) {                                    // 0043D497..0043D49A / 0043D945..0043D969
        for (I k = 0; k < 3; ++k) {                                                 // 0043D926..0043D93F
            List a{}, b{}, c{};                                                     // 0043D4E0..0043D509 (A [50h], B [44h], C [38h])
            U post = post_of(self);                                                 // 0043D50D
            const U face = face_record(post, f);                                    // 0043D513..0043D51A
            const I position_index = index_at(get<U>(vp(face), 0x00), k);          // 0043D51C..0043D521
            const I normal_index = get<U>(vp(post), 0x10) != 0 ? index_at(get<U>(vp(face), 0x04), k) : 0;   // 0043D525..0043D540
            const I vector_index = index_at(get<U>(vp(face), 0x10), k);            // 0043D544..0043D54A
            for (I j = 0; j < get<I>(self, 0x1CC); ++j) {                           // 0043D54E..0043D654 (bound re-read)
                append_vector(&a, vec12(get<U>(self, 0x1A0), get<I>(vp(post_of(self)), 0x04) * j + position_index));   // 0043D570..0043D5E2
                append_vector(&b, vec12(get<U>(self, 0x1B8), get<I>(vp(post_of(self)), 0x58) * j + vector_index));     // 0043D5DB..0043D64C
            }
            for (I j = 0; j < get<I>(self, 0x1D0); ++j)                             // 0043D664..0043D6F1 (bound re-read)
                append_vector(&c, vec12(get<U>(self, 0x1AC), get<I>(vp(post_of(self)), 0x10) * j + normal_index));    // 0043D680..0043D6EA
            post = post_of(self);                                                   // 0043D6FD
            const VertexPointers v = vertex_pointers(post, face_record(post, f), k);  // 0043D6F9..0043D798
            const U v48 = get<I>(vp(post), 0x40) > 0                                // 0043D79B..0043D7B7
                ? vec12(get<U>(vp(post), 0x3C), index_at(get<U>(vp(face_record(post, f)), 0x00), k)) : 0;
            void* const block = live_operator_new(0x7C);                            // 0043D7BA..0043D7C1
            void* const vertex = block != nullptr                                   // 0043D7C4..0043D805
                ? aurora_node_vertex_anim_construct_004791e0(block, nullptr, vp(v.position), vp(v.normal), vp(v.tex0),
                      vp(v.tex1), vp(v.tex2), vp(v.vector3c), vp(v48), &a, &c, &b)   // 0043D7D4..0043D7FC
                : nullptr;
            I found = -1;
            for (I i = 0; i < list_at(vp(post_of(self)), 0x60)->size; ++i) {       // 0043D807..0043D849 (count re-read)
                void* const entry = vp(get<U>(vp(list_at(vp(post_of(self)), 0x60)->data), static_cast<U>(i) * 4));
                void* const anim = aurora_node_vertex_as_anim_00478650(entry, nullptr);   // 0043D820..0043D82C
                if (aurora_node_vertex_equal_00478820(anim, nullptr, vertex) != 0) { found = i; break; }   // 0043D831..0043D83B
            }
            if (found >= 0) {
                if (vertex != nullptr) {                                            // 0043D87B..0043D87D
                    aurora_node_vertex_anim_destroy_0043c1b0(vertex, nullptr);      // 0043D87F..0043D881
                    live_free(vertex);                                              // 0043D886..0043D887 (006FA390)
                }
                store_corner(self, f, k, static_cast<U>(found));                    // 0043D88F..0043D896
            } else {
                store_corner(self, f, k, static_cast<U>(list_at(vp(post_of(self)), 0x60)->size));   // 0043D84B..0043D85C
                append_word(list_at(vp(post_of(self)), 0x60), addr(vertex), 0x10);  // 0043D860..0043D8E5
            }
            if (b.data != 0) live_free(vp(b.data));                                 // 0043D8E8..0043D8F1 (006FC0CD)
            if (c.data != 0) live_free(vp(c.data));                                 // 0043D8F9..0043D902
            if (a.data != 0) live_free(vp(a.data));                                 // 0043D90A..0043D91E
        }
    }
}

// 004885E0 MdlNodeSkin::InternalGenVertices: as TriMesh over node faces f < node[5C] with a 0x58 NodeVertex (7th
// arg 0), base operator== matching (match -> free 006FA390); post+60 growth capacity ? 2*capacity : 8 through
// 006FA7E6/006FA390; each appended vertex also appends its 0x34-byte weight record node[19C][position index] to a
// local list (growth ? 2*capacity : 8, 006FA7E6/006FA390). Then node+19C = local (004880D0); local storage freed.
void __fastcall aurora_skin_gen_vertices_body_004885e0(MdlNode* self, void*) {
    U local_data = 0; I local_count = 0, local_capacity = 0;                        // 00488602..0048860A / [esp+38h..40h]
    for (I f = 0; f < get<I>(self, 0x5C); ++f) {                                    // 0048860E..00488619 / 004888BD..004888DB
        for (I k = 0; k < 3; ++k) {                                                 // 00488896..004888AB
            const U post = post_of(self);                                           // 00488642..00488646
            const U face = face_record(post, f);
            const VertexPointers v = vertex_pointers(post, face, k);                // 0048864C..004886DF
            void* const block = live_operator_new(0x58);                            // 004886DD..004886E2
            void* const vertex = block != nullptr                                   // 004886EA..00488719
                ? aurora_node_vertex_construct_00478680(block, nullptr, vp(v.position), vp(v.normal), vp(v.tex0),
                                                        vp(v.tex1), vp(v.tex2), vp(v.vector3c), nullptr)   // 004886FA..00488710
                : nullptr;
            I found = -1;
            for (I i = 0; i < list_at(vp(post_of(self)), 0x60)->size; ++i)          // 0048871B..00488752 (count re-read)
                if (aurora_node_vertex_equal_00478820(vp(get<U>(vp(list_at(vp(post_of(self)), 0x60)->data), static_cast<U>(i) * 4)),
                                                      nullptr, vertex) != 0) { found = i; break; }   // 00488736..00488744
            if (found >= 0) {
                live_free(vertex);                                                  // 00488784..00488785
                store_corner(self, f, k, static_cast<U>(found));                    // 0048878A..00488794
                continue;
            }
            store_corner(self, f, k, static_cast<U>(list_at(vp(post_of(self)), 0x60)->size));   // 00488754..00488765
            append_word(list_at(vp(post_of(self)), 0x60), addr(vertex), 8);         // 00488769..004887F1 (006FA7E6/006FA390)
            unsigned char record[0x34];                                             // 004887F4..0048881B ([esp+44h])
            std::memcpy(record, at(vp(get<U>(self, 0x19C)), static_cast<U>(index_at(get<U>(vp(face_record(post_of(self), f)), 0x00), k)) * 0x34), 0x34);
            if (local_count == local_capacity) {                                    // 0048881D..00488821
                const U old = local_data;
                local_capacity = local_capacity != 0 ? local_capacity * 2 : 8;      // 00488823..00488834
                local_data = addr(live_operator_new(static_cast<U>(local_capacity) * 0x34));   // 00488838..0048884A
                for (I i = 0; i < local_count; ++i)                                 // 00488848..00488864
                    std::memcpy(at(vp(local_data), static_cast<U>(i) * 0x34), at(vp(old), static_cast<U>(i) * 0x34), 0x34);
                if (old != 0) live_free(vp(old));                                   // 00488866..0048886B
            }
            std::memcpy(at(vp(local_data), static_cast<U>(local_count) * 0x34), record, 0x34);   // 00488877..00488890
            ++local_count;                                                          // 0048888F/00488892
        }
    }
    List local{};                                                                   // [esp+30h..38h] at 004888EB
    local.data = local_data; local.size = local_count; local.capacity = local_capacity;
    aurora_skin_weight_list_assign_004880d0(list_at(self, 0x19C), nullptr, &local); // 004888E7..004888F6
    if (local_data != 0) live_free(vp(local_data));                                 // 004888FB..00488908
}

// ================================================================ ASCII MDL field parsers
// 00468870 MaxTree::InternalParseField(line), RET 4: strncmp(line, "node", 4) == 0 -> ParseNode(line + 4, this).
void __fastcall aurora_maxtree_parse_field_body_00468870(MaxTree* self, void*, char* line) {
    if (kotor_strncmp_006faff0(line, kNode_00742a8c, 4) == 0)                       // 00468872..0046888A
        aurora_parse_node_004680e0(line + 4, self);                                 // 0046888C..00468891
}
// 004688A0 Model::InternalParseField(line), RET 4: firstword(line) (00463530) compared with _stricmp (0070ACAF)
// against "radius" -> sscanf(line+6, "%f", this+80); "bmin" -> sscanf(line+4, "%f %f %f", this+68/6C/70);
// "bmax" -> sscanf(line+4, "%f %f %f", this+74/78/7C); then the "node" test as 00468870. All three tests run
// (firstword re-evaluated each time).
void __fastcall aurora_model_parse_field_body_004688a0(MaxTree* self, void*, char* line) {
    if (kotor_stricmp_0070acaf(kRadius_00742aa4, firstword_00463530(line)) == 0)    // 004688A6..004688BE
        kotor_sscanf_006fae08(line + 6, kF_0073e2e4, at(self, 0x80));               // 004688C0..004688D0
    if (kotor_stricmp_0070acaf(kBmin_00742a9c, firstword_00463530(line)) == 0)      // 004688D8..004688EE
        kotor_sscanf_006fae08(line + 4, kF3_0073ee40, at(self, 0x68), at(self, 0x6C), at(self, 0x70));   // 004688F0..00468905
    if (kotor_stricmp_0070acaf(kBmax_00742a94, firstword_00463530(line)) == 0)      // 0046890D..00468923
        kotor_sscanf_006fae08(line + 4, kF3_0073ee40, at(self, 0x74), at(self, 0x78), at(self, 0x7C));   // 00468925..0046893A
    if (kotor_strncmp_006faff0(line, kNode_00742a8c, 4) == 0)                       // 00468942..00468954
        aurora_parse_node_004680e0(line + 4, self);                                 // 00468956..0046895B
}

} // extern "C"
