// mdl_nodes.cpp - lane-m48-asciimdl (integrator part A): ASCII .mdl node parser ParseNode 004680E0 of the GOG
// swkotor.exe (image base 0x400000), the MdlNode* constructors it calls, the two node-type dispatchers
// (ParseFieldDispatch 0043FA10, PostProcessDispatch 0043FB90), the small post-process bodies, MdlNodeEmitter::Enable*,
// Vector::operator/=, MdlNodeTriMesh_PostProcess ctor and the two jmp thunks. Listings: lane d/<ADDR>.txt (local only).
// Reached only while parsing ASCII models (MaxTree +04 parse-fn cell -> 0046888C / 00468956 / 0046A56E); the menu path
// loads binary MDL. Conventions: asciimdl.hpp header.
//
// ParseNode body: donor lane-m47-3d source/m47_modelb_parse.cpp (sha256 ce3590d8..., excluded by D75) re-verified
// against d/004680E0.txt (both ranges 004680E0..00468595, 00468599..0046886F) and adopted with one change (D2 label on
// the sscanf buffers). ABI: consumer declaration `void __cdecl aurora_parse_node_004680e0(char*, MaxTree*)`
// (f3_mdl_node.cpp:33, lane-m48-a anim_parse.cpp:23 with void* tree - same symbol/ABI). The original returns the node
// in EAX (0046885F); no original caller (0046888C, 00468956, 0046A573) reads EAX, so void is caller-equivalent.
// EH: ParseNode states 0..0Bh (0046813B..0046848A) only free the new block if a node constructor throws; the
// constructors here make no throwing calls (00445210 none; 0044AD80 calls the vertex-pool wrapper 00423EE0 whose
// slot call does not throw), so the 00716C78 funclets are cited, not emitted (D30-FINAL /EHsc rule). The one original
// throw (00468826..00468838 _CxxThrowException(&int 0, ThrowInfo 0076388C)) is `throw 0` from a C++-linkage helper
// (malformed-file path only).
#include "d51e09c5f8-asciimdl.hpp"
#include "21ca817b3e-storage.h"   // historical_storage_007a8a70/7c/88 (owner model46-m46-model-storage)
#include <initializer_list>

using namespace aurora_model;
using asciimdl::list_at;

extern "C" {
// ---- cells (asciimdl.hpp) - constant-initialised, no dynamic initializer
volatile U8 mdl_parse_flag_007b927c = 0;                                                        // .bss (no writer)
volatile U trimesh_default_colors_0078e33c[6] = {0x3F4CCCCDu, 0x3F4CCCCDu, 0x3F4CCCCDu,          // .data 0078E33C 0.8f x3
                                                 0x3E4CCCCDu, 0x3E4CCCCDu, 0x3E4CCCCDu};         // 0078E348 0.2f x3

// ---- existing providers not in asciimdl.hpp (identical declarations to the units cited)
void __cdecl renderer_void_004015a0();                                                           // aurora-peer-void (ret)
void __fastcall behavior_void_return_4_0060e760(void* self, void* unused_edx, void* arg);        // 3d-construct-behavior-tables (ret 4)
}

namespace {
// .rdata strings (read from the pinned image, tools/pe.py)
constexpr char kFormat_00740c9c[] = "%s %s";
constexpr char kCamera_0073e458[] = "camera";
constexpr char kEmitter_00742a84[] = "emitter";
constexpr char kLight_00742a7c[] = "light";
constexpr char kTrimesh_00742a74[] = "trimesh";
constexpr char kAnimmesh_00742a68[] = "animmesh";
constexpr char kDangly_00742a5c[] = "danglymesh";
constexpr char kReference_00742a50[] = "reference";
constexpr char kAabb_00742a48[] = "aabb";
constexpr char kSkin_00742a40[] = "skin";
constexpr char kLightsaber_00741e74[] = "lightsaber";
constexpr char kEndnode_00742a38[] = "endnode";
constexpr float kForty_00740a0c = 40.0f;      // .rdata 00740A0C (42200000)
constexpr U kFortyBits = 0x42200000u;         // 00437C31 immediate
constexpr float kZero_0073d700 = 0.0f;        // .rdata 0073D700
constexpr float kOne_0073d764 = 1.0f;         // .rdata 0073D764

using Ctor = MdlNode* (__fastcall*)(void*, void*, const char*);
MdlNode* create(U size, Ctor ctor, const char* name) {   // push size; call 006FA7E6; test eax; je -> 0; else ctor(name)
    void* const block = live_operator_new(size);
    return block != nullptr ? ctor(block, nullptr, name) : nullptr;
}
// Inline CExoArrayList<DWORD>::Add (004684F4..0046851E, 004685E5..0046870F, 004687BE..004687F6):
// size == capacity -> Allocate(capacity ? 2*capacity : 8) (005E03C0); data[size] = value; ++size.
void list_add(List* l, U value) {
    if (l->size == l->capacity)
        menu_list_growth::allocate_005e03c0(l, nullptr, l->capacity != 0 ? static_cast<U>(l->capacity) * 2u : 8u);
    put<U>(ptr<void>(l->data), static_cast<U>(l->size) * 4u, value);
    l->size = l->size + 1;
}
// strlen + 1 / operator new / byte copy including the terminator (004685A9..004685D9, 0046877E..004687AE).
// The copy loop runs on the returned block without a NULL test (original: no test after 006FA7E6).
char* copy_name(const char* name) {
    U length = 0;
    while (name[length] != '\0') ++length;
    char* const copy = static_cast<char*>(live_operator_new(length + 1u));
    U i = 0;
    char c;
    do { c = name[i]; copy[i] = c; ++i; } while (c != '\0');
    return copy;
}
[[noreturn]] __declspec(noinline) void throw_int_zero() {   // 00468826..00468838 _CxxThrowException(&0, 0076388C)
    throw 0;
}
// `and al/ax,M; sub al/ax,M; neg; sbb eax,eax; not eax; and eax,ecx` (0043FA3E.., 0043FBB9..): node if all bits of
// the type mask are set in the low byte (8-bit masks) / low word (16-bit masks), else 0.
MdlNode* as_mask8(MdlNode* node, U8 mask) {
    const U8 low = static_cast<U8>(get<U16>(node, 0) & 0xFFu);
    return static_cast<U8>(low & mask) == mask ? node : nullptr;
}
MdlNode* as_mask16(MdlNode* node, U16 mask) {
    return static_cast<U16>(get<U16>(node, 0) & mask) == mask ? node : nullptr;
}
}

extern "C" {
// ================================================================ 0044A280 MdlNodeCamera::MdlNodeCamera (thiscall, RET 4)
MdlNode* __fastcall mdl_node_camera_construct_0044a280(void* self, void*, const char* name) {
    mdl_node_construct_00445210(self, nullptr, name);                                          // 0044A280..0044A288
    put<U16>(self, 0, kNodeCamera);                                                            // 0044A28D word [esi]=9
    return static_cast<MdlNode*>(self);                                                        // 0044A292
}

// ================================================================ 0044A300 MdlNodeEmitter::MdlNodeEmitter (thiscall, RET 4)
MdlNode* __fastcall mdl_node_emitter_construct_0044a300(void* self, void*, const char* name) {
    mdl_node_construct_00445210(self, nullptr, name);                                          // 0044A300..0044A30A
    put<U16>(self, 0, kNodeEmitter);                                                           // 0044A317
    put<U>(self, 0x104, 0);                                                                    // 0044A31C
    put<U>(self, 0xF0, 0); put<U>(self, 0xF4, 0); put<U>(self, 0xF8, 0); put<U>(self, 0xFC, 0);  // 0044A322..0044A32E
    for (U i = 0; i < 8; ++i) put<U>(self, 0x10B + i * 4u, 0);                                 // 0044A331..0044A33E rep stos (unaligned +10B..+12A)
    put<U>(self, 0x6C, 0); put<U>(self, 0x50, 0); put<U>(self, 0x100, 0);                      // 0044A341..0044A347
    put<U>(self, 0x54, 0); put<U>(self, 0x58, 0); put<U8>(self, 0x10A, 0);                     // 0044A34D..0044A353
    put<U>(self, 0x5C, 0); put<U>(self, 0x12C, 0); put<U16>(self, 0x108, 0);                   // 0044A359..0044A362
    put<U>(self, 0x60, 0); put<U>(self, 0x64, 0); put<U>(self, 0x68, 0);                       // 0044A369..0044A36F
    put<U8>(self, 0x70, 0); put<U8>(self, 0x90, 0); put<U8>(self, 0xB0, 0); put<U8>(self, 0xD0, 0); // 0044A372..0044A381
    put<U8>(self, 0xF0, 0);                                                                    // 0044A38A byte [ebx=esi+F0]
    return static_cast<MdlNode*>(self);                                                        // 0044A387 eax=esi
}

// ================================================================ 0044A3F0 MdlNodeLight::MdlNodeLight (thiscall, RET 4)
MdlNode* __fastcall mdl_node_light_construct_0044a3f0(void* self, void*, const char* name) {
    mdl_node_construct_00445210(self, nullptr, name);                                          // 0044A40A..0044A40D (EH state -1 only)
    for (U off : {0x58u, 0x5Cu, 0x54u, 0x64u, 0x68u, 0x60u, 0x70u, 0x74u, 0x6Cu, 0x7Cu, 0x80u, 0x78u,
                  0x88u, 0x8Cu, 0x84u})
        put<U>(self, off, 0);                                                                  // 0044A412..0044A447
    put<U>(self, 0x98, 1); put<U>(self, 0x9C, 1); put<U>(self, 0xA0, 1); put<U>(self, 0xA8, 1); // 0044A44D..0044A464
    put<U>(self, 0x94, 0); put<U>(self, 0xA4, 0); put<U>(self, 0x50, 0);                       // 0044A46E..0044A47A
    put<U16>(self, 0, kNodeLight);                                                             // 0044A47D
    put<U>(self, 0x90, 5);                                                                     // 0044A482
    return static_cast<MdlNode*>(self);                                                        // 0044A48C
}

// ================================================================ 0044A5C0 MdlNodeReference::MdlNodeReference (thiscall, RET 4)
MdlNode* __fastcall mdl_node_reference_construct_0044a5c0(void* self, void*, const char* name) {
    mdl_node_construct_00445210(self, nullptr, name);                                          // 0044A5C0..0044A5C8
    put<U16>(self, 0, kNodeReference);                                                         // 0044A5CD
    return static_cast<MdlNode*>(self);                                                        // 0044A5D2
}

// ================================================================ 0044A640 MdlNodeTriMesh::MdlNodeTriMesh (thiscall, RET 4)
MdlNode* __fastcall mdl_node_trimesh_construct_0044a640(void* self, void*, const char* name) {
    mdl_node_construct_00445210(self, nullptr, name);                                          // 0044A65A..0044A65D (EH state -1 only)
    for (U off : {0x5Cu, 0x60u, 0x58u, 0x64u, 0x68u, 0x6Cu, 0x70u, 0x74u, 0x78u, 0x80u, 0x84u, 0x88u, 0x8Cu,
                  0x90u, 0x94u, 0x98u, 0x9Cu, 0xA0u, 0xECu, 0xF0u, 0xE8u, 0xF8u, 0xFCu, 0xF4u, 0x104u, 0x108u,
                  0x100u, 0x110u, 0x114u, 0x10Cu, 0x11Cu, 0x120u, 0x118u})
        put<U>(self, off, 0);                                                                  // 0044A662..0044A709
    put<NodeMeshFn>(self, 0x50, aurora_trimesh_gen_vertices_0043e480);                         // 0044A714 (43E480h)
    put<NodeMeshFn>(self, 0x54, aurora_trimesh_remove_temporary_0043e490);                     // 0044A71B (43E490h)
    put<U>(self, 0x14C, 0xFFFFFFFFu);                                                          // 0044A722
    put<U>(self, 0x150, 0);                                                                    // 0044A728
    for (U off = 0x154; off <= 0x17C; off += 4) put<U>(self, off, 0xFFFFFFFFu);                // 0044A72E..0044A76A
    put<U>(self, 0x18C, 0); put<U>(self, 0x190, 0); put<U>(self, 0xA4, 0);                     // 0044A770..0044A77C
    put<U8>(self, 0x188, 0); put<U8>(self, 0x187, 1); put<U8>(self, 0x189, 1);                 // 0044A782..0044A78E
    put<U8>(self, 0x184, 0); put<U8>(self, 0x186, 0); put<U8>(self, 0x185, 0);                 // 0044A794..0044A7A0
    put<U>(self, 0x8C, trimesh_default_colors_0078e33c[0]);                                    // 0044A7A6..0044A7AC
    put<U>(self, 0x90, trimesh_default_colors_0078e33c[1]);                                    // 0044A7B2..0044A7B8
    put<U>(self, 0x94, trimesh_default_colors_0078e33c[2]);                                    // 0044A7BE..0044A7C4
    put<U>(self, 0x98, trimesh_default_colors_0078e33c[3]);                                    // 0044A7CA..0044A7D0
    put<U>(self, 0x9C, trimesh_default_colors_0078e33c[4]);                                    // 0044A7D6..0044A7DC
    const U color5 = trimesh_default_colors_0078e33c[5];                                       // 0044A7E2 (read before the stores below)
    put<U>(self, 0x128, 0xFFFFFFFFu);                                                          // 0044A7E8
    put<U8>(self, 0xA8, 0); put<U8>(self, 0xC8, 0);                                            // 0044A7F2..0044A7F8
    put<U16>(self, 0x180, 0); put<U16>(self, 0x182, 0);                                        // 0044A7FE..0044A805
    put<U>(self, 0x7C, 0); put<U>(self, 0x12C, 0); put<U>(self, 0x134, 0); put<U>(self, 0x138, 0); // 0044A80C..0044A81B
    put<U>(self, 0x194, 0); put<U>(self, 0x198, 0);                                            // 0044A821..0044A827
    put<U>(self, 0xA0, color5);                                                                // 0044A82D
    put<U16>(self, 0, kNodeTriMesh);                                                           // 0044A833
    return static_cast<MdlNode*>(self);                                                        // 0044A838
}

// ================================================================ 0044A8B0 MdlNodeAABB::MdlNodeAABB (thiscall, RET 4)
MdlNode* __fastcall mdl_node_aabb_construct_0044a8b0(void* self, void*, const char* name) {
    mdl_node_trimesh_construct_0044a640(self, nullptr, name);                                  // 0044A8B4..0044A8B8
    put<U>(self, 0x19C, 0);                                                                    // 0044A8BF
    put<U8>(self, 0x189, 0); put<U8>(self, 0x187, 0);                                          // 0044A8C5..0044A8CB
    put<U16>(self, 0, kNodeAABB);                                                              // 0044A8D1
    return static_cast<MdlNode*>(self);                                                        // 0044A8D6
}

// ================================================================ 0044ABC0 MdlNodeAnimMesh::MdlNodeAnimMesh (thiscall, RET 4)
MdlNode* __fastcall mdl_node_anim_mesh_construct_0044abc0(void* self, void*, const char* name) {
    mdl_node_trimesh_construct_0044a640(self, nullptr, name);                                  // 0044ABDA..0044ABDD (EH state -1 only)
    for (U off : {0x1A4u, 0x1A8u, 0x1A0u, 0x1B0u, 0x1B4u, 0x1ACu, 0x1BCu, 0x1C0u, 0x1B8u, 0x19Cu})
        put<U>(self, off, 0);                                                                  // 0044ABE2..0044AC1E
    put<U8>(self, 0xA8, 0); put<U8>(self, 0xC8, 0);                                            // 0044AC24..0044AC2A
    put<U>(self, 0x1CC, 0); put<U>(self, 0x1D0, 0); put<U>(self, 0x1C4, 0); put<U>(self, 0x1C8, 0); // 0044AC30..0044AC42
    put<NodeMeshFn>(self, 0x50, aurora_anim_mesh_gen_vertices_0043e420);                       // 0044AC48 (43E420h)
    put<NodeMeshFn>(self, 0x54, aurora_anim_mesh_remove_temporary_0043e430);                   // 0044AC4F (43E430h)
    put<U16>(self, 0, kNodeAnimMesh);                                                          // 0044AC56
    return static_cast<MdlNode*>(self);                                                        // 0044AC5B
}

// ================================================================ 0044AC70 MdlNodeSkin::MdlNodeSkin (thiscall, RET 4)
MdlNode* __fastcall mdl_node_skin_construct_0044ac70(void* self, void*, const char* name) {
    mdl_node_trimesh_construct_0044a640(self, nullptr, name);                                  // 0044AC8A..0044AC8D (EH state -1 only)
    for (U off : {0x1A0u, 0x1A4u, 0x19Cu, 0x1BCu, 0x1C0u, 0x1B8u, 0x1C8u, 0x1CCu, 0x1C4u, 0x1D4u, 0x1D8u,
                  0x1D0u, 0x1B0u})
        put<U>(self, off, 0);                                                                  // 0044AC92..0044ACE0
    put<U>(self, 0x1A8, 0xFFFFFFFFu); put<U>(self, 0x1AC, 0xFFFFFFFFu);                        // 0044ACE6..0044ACEF
    put<U16>(self, 0, kNodeSkin);                                                              // 0044ACF5
    put<NodeMeshFn>(self, 0x50, aurora_skin_gen_vertices_0043e440);                            // 0044ACFA (43E440h)
    put<NodeMeshFn>(self, 0x54, aurora_skin_remove_temporary_0043e450);                        // 0044AD01 (43E450h)
    return static_cast<MdlNode*>(self);                                                        // 0044AD08
}

// ================================================================ 0044AD80 MdlNodeLightsaber::MdlNodeLightsaber (thiscall, RET 4)
MdlNode* __fastcall mdl_node_lightsaber_construct_0044ad80(void* self, void*, const char* name) {
    mdl_node_trimesh_construct_0044a640(self, nullptr, name);                                  // 0044AD9D..0044ADA2 (then EH state 0)
    put<U16>(self, 0, kNodeSaber);                                                             // 0044ADC4
    aurora_pool_request_00423ee0(0, 0x100, 2, 0x1012, reinterpret_cast<U*>(at(self, 0x1A8))); // 0044ADA7..0044ADC9
    aurora_pool_request_00423ee0(0, 0x640, 2, 0x1010, reinterpret_cast<U*>(at(self, 0x1AC))); // 0044ADCE..0044ADE3
    return static_cast<MdlNode*>(self);                                                        // 0044ADEF
}

// ================================================================ 0044AE00 MdlNodeDanglyMesh::MdlNodeDanglyMesh (thiscall, RET 4)
MdlNode* __fastcall mdl_node_dangly_construct_0044ae00(void* self, void*, const char* name) {
    mdl_node_trimesh_construct_0044a640(self, nullptr, name);                                  // 0044AE1A..0044AE1D (EH state -1 only)
    put<U>(self, 0x1A0, 0); put<U>(self, 0x1A4, 0); put<U>(self, 0x19C, 0);                    // 0044AE28..0044AE34
    put<U16>(self, 0, kNodeDangly);                                                            // 0044AE3A
    put<NodeMeshFn>(self, 0x54, aurora_dangly_remove_temporary_0043e460);                      // 0044AE3F (43E460h)
    put<NodeMeshFn>(self, 0x50, aurora_dangly_gen_vertices_0043e470);                          // 0044AE46 (43E470h)
    return static_cast<MdlNode*>(self);                                                        // 0044AE4D
}

// ================================================================ 00445720 MdlNodeTriMesh_PostProcess ctor (thiscall, RET 0)
// 27 DWORDs +00..+68 = 0 (nine 12-byte list headers), EH state -1 only; EAX = this (0044572E).
void* __fastcall mdl_trimesh_postprocess_construct_00445720(void* self, void*) {
    for (U off : {0x04u, 0x08u, 0x00u, 0x10u, 0x14u, 0x0Cu, 0x1Cu, 0x20u, 0x18u, 0x28u, 0x2Cu, 0x24u, 0x34u,
                  0x38u, 0x30u, 0x40u, 0x44u, 0x3Cu, 0x4Cu, 0x50u, 0x48u, 0x58u, 0x5Cu, 0x54u, 0x64u, 0x68u, 0x60u})
        put<U>(self, off, 0);                                                                  // 00445739..00445786
    return self;
}

// ================================================================ 004A97D0 Vector::operator/= (thiscall, RET 4)
// fld d; fcomp 0.0 (0073D700); test ah,44h; jnp skip -> divide unless d == 0 (NaN divides). 1/d is the wide
// quotient (fdiv, kept on the stack), each component = wide * stored component, stored by fstp dword (D30-FINAL).
float* __fastcall vector_divide_assign_004a97d0(float* self, void*, float divisor) {
    if (!(divisor == kZero_0073d700)) {                                                        // 004A97D0..004A97E1
        const double inverse = static_cast<double>(kOne_0073d764) / static_cast<double>(divisor); // 004A97E3..004A97E9
        self[0] = static_cast<float>(inverse * static_cast<double>(self[0]));                 // 004A97ED..004A97F1
        self[1] = static_cast<float>(inverse * static_cast<double>(self[1]));                 // 004A97F3..004A97F8
        self[2] = static_cast<float>(inverse * static_cast<double>(self[2]));                 // 004A97FB..004A97FE
    }
    return self;                                                                               // 004A97DF eax=ecx
}

// ================================================================ 0048D790..0048D850 MdlNodeEmitter::Enable* (thiscall, RET 0)
// or dword ptr [ecx+12Ch], bit
void __fastcall mdl_emitter_enable_point_to_point_0048d790(MdlNode* s, void*) { put<U>(s, 0x12C, get<U>(s, 0x12C) | 0x1u); }       // 0048D790
void __fastcall mdl_emitter_enable_point_to_point_sel_0048d7a0(MdlNode* s, void*) { put<U>(s, 0x12C, get<U>(s, 0x12C) | 0x2u); }   // 0048D7A0
void __fastcall mdl_emitter_enable_affected_by_wind_0048d7b0(MdlNode* s, void*) { put<U>(s, 0x12C, get<U>(s, 0x12C) | 0x4u); }     // 0048D7B0
void __fastcall mdl_emitter_enable_tinted_0048d7c0(MdlNode* s, void*) { put<U>(s, 0x12C, get<U>(s, 0x12C) | 0x8u); }               // 0048D7C0
void __fastcall mdl_emitter_enable_bouncing_0048d7d0(MdlNode* s, void*) { put<U>(s, 0x12C, get<U>(s, 0x12C) | 0x10u); }            // 0048D7D0
void __fastcall mdl_emitter_enable_random_0048d7e0(MdlNode* s, void*) { put<U>(s, 0x12C, get<U>(s, 0x12C) | 0x20u); }              // 0048D7E0
void __fastcall mdl_emitter_enable_random_order_0048d7f0(MdlNode* s, void*) { put<U>(s, 0x12C, get<U>(s, 0x12C) | 0x1000u); }      // 0048D7F0
void __fastcall mdl_emitter_enable_inheriting_0048d800(MdlNode* s, void*) { put<U>(s, 0x12C, get<U>(s, 0x12C) | 0x40u); }          // 0048D800
void __fastcall mdl_emitter_enable_inheriting_velocity_0048d810(MdlNode* s, void*) { put<U>(s, 0x12C, get<U>(s, 0x12C) | 0x80u); } // 0048D810
void __fastcall mdl_emitter_enable_inheriting_local_0048d820(MdlNode* s, void*) { put<U>(s, 0x12C, get<U>(s, 0x12C) | 0x100u); }   // 0048D820
void __fastcall mdl_emitter_enable_splat_0048d830(MdlNode* s, void*) { put<U>(s, 0x12C, get<U>(s, 0x12C) | 0x200u); }              // 0048D830
void __fastcall mdl_emitter_enable_inheriting_part_0048d840(MdlNode* s, void*) { put<U>(s, 0x12C, get<U>(s, 0x12C) | 0x400u); }    // 0048D840
void __fastcall mdl_emitter_enable_depth_texture_0048d850(MdlNode* s, void*) { put<U>(s, 0x12C, get<U>(s, 0x12C) | 0x800u); }      // 0048D850

// ================================================================ 00468060 / 0046A150 jmp thunks
void __fastcall mdl_node_internal_parse_field_thunk_00468060(MdlNode* self, void*, char* line) {
    mdl_node_internal_parse_field_00465560(self, nullptr, line);                               // 00468060 jmp 00465560
}
void __fastcall mdl_trimesh_internal_parse_field_thunk_0046a150(MdlNode* self, void*, char* line) {
    mdl_trimesh_internal_parse_field_00469700(self, nullptr, line);                            // 0046A150 jmp 00469700
}

// ================================================================ 0043FA10 MdlNode::ParseFieldDispatch (thiscall, RET 4)
// switch on the signed type word (movsx): 1..21h through the byte/dword tables 0043FB68/0043FB4C (read from the image:
// index 0->type 1, 2->3, 4->5, 8->9, 10h->11h, 20h->21h, others -> RET), then 61h/A1h/121h/221h/401h/821h; each case
// tail-jumps with ECX = (node masked by its type bits) (see as_mask8/as_mask16). Unknown types return.
void __fastcall mdl_node_parse_field_dispatch_0043fa10(MdlNode* self, void*, char* line) {
    const I type = static_cast<I>(static_cast<I16>(get<U16>(self, 0)));                      // 0043FA10..0043FA13
    switch (type) {
    case 0x01: mdl_node_internal_parse_field_00465560(self, nullptr, line); return;            // 0043FA39 jmp (ecx = node unmasked)
    case 0x03: mdl_light_internal_parse_field_00469150(as_mask8(self, 0x03), nullptr, line); return;   // 0043FA3E..0043FA4C
    case 0x05: mdl_emitter_internal_parse_field_004658b0(as_mask8(self, 0x05), nullptr, line); return; // 0043FA51..0043FA5F
    case 0x09: mdl_node_internal_parse_field_thunk_00468060(as_mask8(self, 0x09), nullptr, line); return; // 0043FA64..0043FA72
    case 0x11: mdl_reference_internal_parse_field_00468070(as_mask8(self, 0x11), nullptr, line); return; // 0043FA77..0043FA85
    case 0x21: mdl_trimesh_internal_parse_field_00469700(as_mask8(self, 0x21), nullptr, line); return; // 0043FA8A..0043FA98
    case 0x61: mdl_skin_internal_parse_field_0046a2f0(as_mask8(self, 0x61), nullptr, line); return;    // 0043FA9D..0043FAAB
    case 0xA1: mdl_anim_mesh_internal_parse_field_0046a240(as_mask8(self, 0xA1), nullptr, line); return; // 0043FAE0..0043FAEE
    case 0x121: mdl_dangly_internal_parse_field_0046a160(as_mask16(self, 0x121), nullptr, line); return; // 0043FACA..0043FADB
    case 0x221: mdl_aabb_internal_parse_field_0046a3c0(as_mask16(self, 0x221), nullptr, line); return;  // 0043FAF3..0043FB06
    case 0x401: behavior_void_return_4_0060e760(as_mask16(self, 0x401), nullptr, line); return;         // 0043FB31..0043FB44 (ret 4)
    case 0x821: mdl_trimesh_internal_parse_field_thunk_0046a150(as_mask16(self, 0x821), nullptr, line); return; // 0043FB1B..0043FB2C
    default: return;                                                                           // 0043FB49 ret 4
    }
}

// ================================================================ 0043FB90 MdlNode::PostProcessDispatch (thiscall, RET 0)
// Same switch shape (tables 0043FCE4/0043FCC8). Type 1 -> table[0] = 0043FCBF (jmp 004015A0 with ECX = node).
void __fastcall mdl_node_post_process_dispatch_0043fb90(MdlNode* self, void*) {
    const I type = static_cast<I>(static_cast<I16>(get<U16>(self, 0)));                      // 0043FB90..0043FB93
    switch (type) {
    case 0x01: renderer_void_004015a0(); return;                                               // 0043FCBF jmp 004015A0 (ret)
    case 0x03: mdl_light_internal_post_process_0043dfc0(as_mask8(self, 0x03), nullptr); return;      // 0043FBB9..0043FBC7
    case 0x05: mdl_emitter_internal_post_process_00437c10(as_mask8(self, 0x05), nullptr); return;    // 0043FBCC..0043FBDA
    case 0x09: renderer_void_004015a0(); return;                                               // 0043FBDF..0043FBED (masked ECX unused)
    case 0x11: renderer_void_004015a0(); return;                                               // 0043FBF2..0043FC00
    case 0x21: mdl_trimesh_internal_post_process_0043cf00(as_mask8(self, 0x21), nullptr); return;    // 0043FC05..0043FC13
    case 0x61: mdl_skin_internal_post_process_0043b350(as_mask8(self, 0x61), nullptr); return;       // 0043FC18..0043FC26
    case 0xA1: renderer_void_004015a0(); return;                                               // 0043FC5B..0043FC69
    case 0x121: mdl_dangly_internal_post_process_0043d990(as_mask16(self, 0x121), nullptr); return;  // 0043FC45..0043FC56
    case 0x221: mdl_trimesh_internal_post_process_0043cf00(as_mask16(self, 0x221), nullptr); return; // 0043FC6E..0043FC81
    case 0x401: renderer_void_004015a0(); return;                                              // 0043FCAC..0043FCBF
    case 0x821: mdl_saber_internal_post_process_0043b380(as_mask16(self, 0x821), nullptr); return;   // 0043FC96..0043FCA7
    default: return;                                                                           // 0043FCC4 ret
    }
}

// ================================================================ 00437C10 MdlNodeEmitter::InternalPostProcess (thiscall, RET 0)
void __fastcall mdl_emitter_internal_post_process_00437c10(MdlNode* self, void*) {
    MaxTree* const model = aurora_tree_as_model_0043e1c0(ptr<MaxTree>(get<U>(self, 0x08)), nullptr); // 00437C10..00437C13
    if (model == nullptr) return;                                                              // 00437C1A..00437C1C
    // fld [model+80h]; fcomp 40.0; test ah,5; jp skip -> store only when ordered and below 40.0
    if (get<float>(model, 0x80) < kForty_00740a0c)                                             // 00437C1E..00437C2F
        put<U>(model, 0x80, kFortyBits);                                                       // 00437C31
}

// ================================================================ 0043B350 MdlNodeSkin::InternalPostProcess (thiscall, RET 0)
void __fastcall mdl_skin_internal_post_process_0043b350(MdlNode* self, void*) {
    if (static_cast<I16>(get<U16>(self, 0x180)) > 0) return;                                   // 0043B350..0043B358 (signed word)
    mdl_trimesh_process_verts_0043a120(self, nullptr);                                         // 0043B35A (ecx = self)
    put<U>(&historical_storage_007a8a88, 4, 0);                                                // 0043B35F (007A8A8C)
    put<U>(&historical_storage_007a8a7c, 4, 0);                                                // 0043B369 (007A8A80)
    put<U>(&historical_storage_007a8a70, 4, 0);                                                // 0043B373 (007A8A74)
}

// ================================================================ 0043DFC0 MdlNodeLight::InternalPostProcess (thiscall, RET 0)
// Texture-name list (+84 data, +88 size) -> SafePointer<CAurTexture> list +54: SetSize(count) (0043DF60), then from the
// last index down: AurTextureGetReference(name, 0) and an inlined SafePointer assignment: unregister the slot address
// from the old target's registration list (+04 data, +08 size, +0C capacity; search from the end; size decremented
// even when not found and the shift loop then starts at index -1 - original quirk, unreachable while the registration
// invariant holds), store the new target, register the slot (grow to 2*cap or 16 through 006FC0D2/006FC0CD).
void __fastcall mdl_light_internal_post_process_0043dfc0(MdlNode* self, void*) {
    aurora_texture_ptr_list_set_size_0043df60(at(self, 0x54), nullptr, get<I>(self, 0x88));  // 0043DFC5..0043DFD3
    I index = get<I>(self, 0x88);                                                              // 0043DFD8
    while (index != 0) {                                                                       // 0043DFDE..0043DFE0 / 0043E0BA..0043E0BC
        --index;                                                                               // 0043DFF9
        const char* const texture_name = ptr<const char>(get<U>(ptr<void>(get<U>(self, 0x84)), static_cast<U>(index) * 4u)); // 0043DFF0..0043DFFA
        const U slot = get<U>(self, 0x54) + static_cast<U>(index) * 4u;                       // 0043DFF6 / 0043DFFD / 0043E007
        font::Texture* const target = font::reference_00423a60(texture_name, nullptr);         // 0043E004..0043E009 (push 0; push name)
        const U old = get<U>(ptr<void>(slot), 0);                                              // 0043E00E
        if (old != 0) {                                                                        // 0043E013..0043E017
            const I last = get<I>(ptr<void>(old), 8) - 1;                                      // 0043E019..0043E01C ebx
            I found = last;                                                                    // 0043E01F
            if (found >= 0) {                                                                  // 0043E021..0043E023
                const U data = get<U>(ptr<void>(old), 4);                                      // 0043E025
                while (found >= 0 && get<U>(ptr<void>(data + static_cast<U>(found) * 4u), 0) != slot) --found; // 0043E030..0043E03A
            }
            put<I>(ptr<void>(old), 8, last);                                                   // 0043E03E (always)
            if (found < last) {                                                                // 0043E03C / 0043E041
                for (I k = found; k < get<I>(ptr<void>(old), 8); ++k) {                        // 0043E043..0043E055 (size re-read)
                    const U data = get<U>(ptr<void>(old), 4);
                    put<U>(ptr<void>(data + static_cast<U>(k) * 4u), 0, get<U>(ptr<void>(data + static_cast<U>(k) * 4u), 4));
                }
            }
        }
        put<U>(ptr<void>(slot), 0, addr(target));                                             // 0043E059
        if (target != nullptr) {                                                               // 0043E057 / 0043E05B
            List& reg = target->registrations;                                                 // +04/+08/+0C
            if (reg.size == reg.capacity) {                                                    // 0043E05D..0043E063
                const I capacity = reg.capacity != 0 ? reg.capacity + reg.capacity : 0x10;     // 0043E065..0043E06D
                const U previous = reg.data;                                                   // 0043E072
                reg.capacity = capacity;                                                       // 0043E075
                void* const grown = live_operator_new(static_cast<U>(capacity) << 2);          // 0043E078..0043E07C (006FC0D2)
                reg.data = addr(grown);                                                        // 0043E084
                for (I k = 0; k < reg.size; ++k)                                               // 0043E08A..0043E09F (no NULL test)
                    put<U>(ptr<void>(reg.data), static_cast<U>(k) * 4u, get<U>(ptr<void>(previous), static_cast<U>(k) * 4u));
                if (previous != 0) live_free(ptr<void>(previous));                             // 0043E0A1..0043E0A6 (006FC0CD)
            }
            put<U>(ptr<void>(reg.data), static_cast<U>(reg.size) * 4u, slot);                 // 0043E0AE..0043E0B4
            reg.size = reg.size + 1;                                                           // 0043E0B7
        }
    }
}

// ================================================================ 004680E0 ParseNode (cdecl, 2 args, RET 0)
void __cdecl aurora_parse_node_004680e0(char* text, MaxTree* tree) {
    // D2 (004680F9 sub esp,84h): the two 64-byte frame buffers have no initializing store; if sscanf converts nothing
    // (empty text -> EOF) the original compares stack garbage. Value-initialised here (D2 deviation, unreachable on
    // well-formed "node <type> <name>" lines).
    char name[0x40] = {};                                                                      // [esp+8]
    char type[0x40] = {};                                                                      // [esp+48h]
    kotor_sscanf_006fae08(text, kFormat_00740c9c, type, name);                                 // 00468100..00468110
    MdlNode* node;                                                                             // [esp+4] -> ebx
    if (kotor_stricmp_0070acaf(type, kCamera_0073e458) == 0)                                   // 00468115..00468129
        node = create(0x50, mdl_node_camera_construct_0044a280, name);                         // 0046812B..00468153 (state 0)
    else if (kotor_stricmp_0070acaf(type, kEmitter_00742a84) == 0)                             // 0046815D..00468171
        node = create(0x130, mdl_node_emitter_construct_0044a300, name);                       // 00468173..0046819E (state 1)
    else if (kotor_stricmp_0070acaf(type, kLight_00742a7c) == 0)                               // 004681A8..004681BC
        node = create(0xAC, mdl_node_light_construct_0044a3f0, name);                          // 004681BE..004681E9 (state 2)
    else if (kotor_stricmp_0070acaf(type, kTrimesh_00742a74) == 0)                             // 004681F3..00468207
        node = create(0x19C, mdl_node_trimesh_construct_0044a640, name);                       // 00468209..00468234 (state 3)
    else if (kotor_stricmp_0070acaf(type, kAnimmesh_00742a68) == 0)                            // 0046823E..00468252
        node = create(0x1D4, mdl_node_anim_mesh_construct_0044abc0, name);                     // 00468254..0046827F (state 4)
    else if (kotor_stricmp_0070acaf(type, kDangly_00742a5c) == 0)                              // 00468289..0046829D
        node = create(0x1B8, mdl_node_dangly_construct_0044ae00, name);                        // 0046829F..004682CA (state 5)
    else if (kotor_stricmp_0070acaf(type, kReference_00742a50) == 0)                           // 004682D4..004682E8
        node = create(0x74, mdl_node_reference_construct_0044a5c0, name);                      // 004682EA..00468312 (state 6)
    else if (kotor_stricmp_0070acaf(type, kAabb_00742a48) == 0)                                // 0046831C..00468330
        node = create(0x1A0, mdl_node_aabb_construct_0044a8b0, name);                          // 00468332..0046835D (state 7)
    else if (kotor_stricmp_0070acaf(type, kSkin_00742a40) == 0)                                // 00468367..0046837B
        node = create(0x200, mdl_node_skin_construct_0044ac70, name);                          // 0046837D..004683A4 (state 8)
    else if (kotor_stricmp_0070acaf(type, kLightsaber_00741e74) == 0)                          // 004683AB..004683BF
        node = create(0x1B0, mdl_node_lightsaber_construct_0044ad80, name);                    // 004683C1..004683E8 (state 9)
    else
        node = create(0x50, mdl_node_construct_00445210, name);                                // 004683EF..00468413 (state 0Ah)
    put<MaxTree*>(node, 0x08, tree);                                                           // 0046841E..00468432 [ebx+8] = tree
    for (;;) {                                                                                 // 00468435..00468469
        char* line = resource_next_0044bfa0();                                                 // 00468435
        if (line == nullptr) break;                                                            // 0046843C..0046843E
        while (*line == ' ' || *line == '\t') ++line;                                          // 00468440..0046844B
        if (kotor_strncmp_006faff0(line, kEndnode_00742a38, 7) == 0) break;                    // 0046844D..0046845F
        mdl_node_parse_field_dispatch_0043fa10(node, nullptr, line);                           // 00468461..00468464
    }
    if (aurora_tree_as_animation_0043e1d0(tree, nullptr) != nullptr) {                         // 0046846B..00468474
        // Animation tree: replace the parsed node with a plain MdlNode carrying its controllers.
        MdlNode* const copy = create(0x50, mdl_node_construct_00445210, name);                 // 0046847A..004684A7 (state 0Bh)
        float_list_assign_00438a50(list_at(copy, 0x44), nullptr, list_at(node, 0x44));        // 004684A9..004684BB
        const U16 word2 = get<U16>(node, 0x02);                                                // 004684C0
        put<U16>(copy, 0x02, word2);                                                           // 004684CB
        controller_list_assign_004642b0(list_at(copy, 0x38), nullptr, list_at(node, 0x38));   // 004684C4..004684CF
        put<MaxTree*>(copy, 0x08, tree);                                                       // 004684D4..004684DB
        void* const parent = ptr<void>(get<U>(node, 0x0C));                                    // 004684DE
        if (parent != nullptr) {                                                               // 004684E1..004684E3
            put<U>(copy, 0x0C, addr(parent));                                                  // 004684E9
            texture_list_remove_00445080(list_at(parent, 0x2C), nullptr, addr(node));          // 004684E5..004684EC
            list_add(list_at(ptr<void>(get<U>(copy, 0x0C)), 0x2C), addr(copy));               // 004684F1..0046851E
        } else if (get<U>(tree, 0x28) == addr(node)) {                                         // 00468523..00468526 (ecx = tree)
            put<U>(tree, 0x28, addr(copy));                                                    // 00468528
        }
        MdlNode* const old = node;                                                             // 0046852B esi
        node = copy;                                                                           // 0046852F ebx
        // Destroy the parsed node by its runtime type: each As* test is followed by a second As* call whose result
        // is destroyed (and freed, 006FA390) when non-null, exactly as the original.
        MdlNode* victim = nullptr;
        void (__fastcall* destroy)(MdlNode*, void*) = nullptr;
        if (aurora_node_as_aabb_0043e340(old, nullptr) != nullptr) {                           // 00468531..0046853A
            victim = aurora_node_as_aabb_0043e340(old, nullptr); destroy = aurora_aabb_destruct_00449170;          // 0046853C..00468549
        } else if (aurora_node_as_anim_mesh_0043e360(old, nullptr) != nullptr) {               // 00468550..00468559
            victim = aurora_node_as_anim_mesh_0043e360(old, nullptr); destroy = aurora_anim_mesh_destruct_00448da0; // 0046855B..00468568
        } else if (aurora_node_as_camera_0043e370(old, nullptr) != nullptr) {                  // 0046856F..00468578
            victim = aurora_node_as_camera_0043e370(old, nullptr); destroy = aurora_node_destruct_0044a040;        // 0046857E..0046858B
        } else if (aurora_node_as_dangly_0043e380(old, nullptr) != nullptr) {                  // 0046860E..00468617
            victim = aurora_node_as_dangly_0043e380(old, nullptr); destroy = aurora_dangly_destruct_004490f0;      // 00468619..0046862A
        } else if (aurora_node_as_emitter_0043e3c0(old, nullptr) != nullptr) {                 // 00468634..0046863D
            victim = aurora_node_as_emitter_0043e3c0(old, nullptr); destroy = aurora_node_destruct_0044a040;       // 0046863F..00468644 -> 00468583
        } else if (aurora_node_as_light_0043e3d0(old, nullptr) != nullptr) {                   // 00468649..00468652
            victim = aurora_node_as_light_0043e3d0(old, nullptr); destroy = aurora_light_node_destruct_00448f80;   // 00468654..00468665
        } else if (aurora_node_as_saber_0043e3a0(old, nullptr) != nullptr) {                   // 0046866F..00468678
            victim = aurora_node_as_saber_0043e3a0(old, nullptr); destroy = mdl_node_lightsaber_destruct_00449090; // 0046867A..0046868B
        } else if (aurora_node_as_reference_0043e3e0(old, nullptr) != nullptr) {               // 00468695..0046869E
            victim = aurora_node_as_reference_0043e3e0(old, nullptr); destroy = aurora_node_destruct_0044a040;     // 004686A0..004686A5 -> 00468583
        } else if (aurora_node_as_skin_0043e3f0(old, nullptr) != nullptr) {                    // 004686AA..004686B3
            victim = aurora_node_as_skin_0043e3f0(old, nullptr); destroy = aurora_skin_destruct_00448e60;          // 004686B5..004686C6
        } else if (aurora_node_as_trimesh_0043e400(old, nullptr) != nullptr) {                 // 004686D0..004686D7
            victim = aurora_node_as_trimesh_0043e400(old, nullptr); destroy = aurora_trimesh_destruct_00448bf0;    // 004686DD..004686F0
        } else {
            victim = old; destroy = aurora_node_destruct_0044a040;                             // 004686D7 je 00468585 (esi = old)
        }
        if (victim != nullptr) {                                                               // 00468543 / 00468562 / 00468585 ...
            destroy(victim, nullptr);
            live_free(victim);                                                                 // 00468590..00468591
        }
    }
    MaxTree* const owner = ptr<MaxTree>(get<U>(node, 0x08));                                   // 00468599 ecx = [ebx+8]
    if (aurora_tree_as_model_0043e1c0(owner, nullptr) != nullptr) {                            // 0046859C..004685A3
        char* const copy = copy_name(name);                                                    // 004685A9..004685D9
        MaxTree* const model = aurora_tree_as_model_0043e1c0(ptr<MaxTree>(get<U>(node, 0x08)), nullptr);   // 004685DB..004685E3
        list_add(list_at(model, 0xB8), addr(copy));                                            // 004685E5..0046870F
        MaxTree* const again = aurora_tree_as_model_0043e1c0(ptr<MaxTree>(get<U>(node, 0x08)), nullptr);   // 00468712..00468715
        put<I>(node, 0x04, get<I>(again, 0xBC) - 1);                                           // 0046871A..00468721
    } else {
        MaxTree* const anim = aurora_tree_as_animation_0043e1d0(ptr<MaxTree>(get<U>(node, 0x08)), nullptr); // 00468729..0046872C
        I index = get<I>(ptr<void>(get<U>(anim, 0x84)), 0xBC);                                 // 00468731..00468737
        bool found = false;
        while (index != 0) {                                                                   // 0046873D / 00468770..00468772
            --index;                                                                           // 00468744
            void* const base = ptr<void>(get<U>(aurora_tree_as_animation_0043e1d0(ptr<MaxTree>(get<U>(node, 0x08)), nullptr), 0x84)); // 00468741..0046874A
            const char* const entry = ptr<const char>(get<U>(ptr<void>(get<U>(base, 0xB8)), static_cast<U>(index) * 4u)); // 00468750..0046875B
            if (kotor_stricmp_0070acaf(entry, name) == 0) {                                    // 0046875E..0046876E
                put<I>(node, 0x04, index);                                                     // 004687DC (then 00468775: index != -1)
                found = true;
                break;
            }
        }
        if (!found) {                                                                          // 00468774..00468778 (dec esi; == -1)
            char* const copy = copy_name(name);                                                // 0046877E..004687AE
            void* const base = ptr<void>(get<U>(aurora_tree_as_animation_0043e1d0(ptr<MaxTree>(get<U>(node, 0x08)), nullptr), 0x84)); // 004687B0..004687B8
            list_add(list_at(base, 0xB8), addr(copy));                                         // 004687BE..004687F6
            void* const again = ptr<void>(get<U>(aurora_tree_as_animation_0043e1d0(ptr<MaxTree>(get<U>(node, 0x08)), nullptr), 0x84)); // 004687F9..00468801
            put<I>(node, 0x04, get<I>(again, 0xBC) - 1);                                       // 00468807..0046880E
        }
    }
    if (get<U>(node, 0x0C) == 0 && get<U>(tree, 0x28) != addr(node))                         // 00468811..00468824 (eax = arg tree)
        throw_int_zero();                                                                      // 00468826..00468838
    put<U16>(node, 0x02, static_cast<U16>(get<U>(tree, 0x2C)));                                // 0046883D..00468841
    put<U>(tree, 0x2C, get<U>(tree, 0x2C) + 1u);                                               // 00468845 inc dword [eax+2Ch]
    if (mdl_parse_flag_007b927c == 0)                                                          // 00468848..0046884F
        mdl_node_post_process_dispatch_0043fb90(node, nullptr);                                // 00468851..00468853
    // EAX = node (0046885F) - not returned: every consumer declaration is void (see header note).
}
}
