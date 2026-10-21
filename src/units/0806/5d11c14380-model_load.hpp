// model_load.hpp - lane-3d-model: Aurora model load chain (FindModel -> IODispatcher -> Input::Read ->
// InputBinary::Read -> Reset* relocation -> model cache) of the GOG swkotor.exe (image base 0x400000).
// Every body in the .cpp files cites the original instructions that establish it. Objects are raw
// observed-offset storage (binary MDL images relocated in place); no class layout beyond cited offsets.
// Calling-convention transport (integrator decision D1): original __thiscall bodies are exported as
// __fastcall(self, unused_edx, ...) - identical ECX receiver, stack layout and callee cleanup.
#pragma once
#include "459924a746-array_mutations.h"
#include "69638710e8-font.hpp"  // font::Texture/Safe, reference_00423a60, assign_00447fa0
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace aurora_model {
using U = std::uint32_t;
using I = std::int32_t;
using U16 = std::uint16_t;
using I16 = std::int16_t;
using U8 = std::uint8_t;
using List = kotor_reconstruction::ArrayMutations;   // {U32 data, I32 size, I32 capacity} (task-14)

// Raw observed-offset accessors (memcpy: no aliasing assumption; precedent lane-resman get/put).
template <class T> inline T get(const void* p, U off) { T v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof(v)); return v; }
template <class T> inline void put(void* p, U off, T v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, sizeof(v)); }
inline unsigned char* at(void* p, U off) { return static_cast<unsigned char*>(p) + off; }
inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
template <class T> inline T* ptr(U a) { return reinterpret_cast<T*>(static_cast<std::uintptr_t>(a)); }

// Opaque runtime objects (all relocated in place inside the loaded MDL image or heap blocks).
struct MaxTree;      // Model (type 2) / Animation (type 5) header: +00 destroy fn, +04 parse fn, +08 name[32],
                     // +28 root node, +48 res handle, +4C type byte (bit7 = binary), +54 ref count (Model),
                     // +58/+5C animation list, +64 supermodel, +88 supermodel name, +A8 offset, +AC/+B0 mdx
                     // pool handle/size, +B4 mdx res handle, +B8/+BC name table (Model) - cited per use.
struct MdlNode;      // +00 u16 type, +08 owner tree, +0C parent, +2C/+30 children, +38/+3C controllers,
                     // +44/+48 controller data; mesh header from +50 (cited per use).
struct Resource;     // lane-aurres 28-byte descriptor (0044C6A8/0044C7DB/0044C871)
struct IODispatcher; // 12-byte CExoArrayList static at 00830510 (004A05AA..004A05BB, freed by 0047ABC0)
struct InputObject;  // ReadSync stack object, 16 bytes (004A15B0/004A15D0 sub esp,10h)

// Node type words (.rdata 00740A18..00740A44, read-only; values read from the image).
constexpr U16 kNodeHeader = 0x0001;      // 00740A18
constexpr U16 kNodeLight = 0x0003;       // 00740A1C
constexpr U16 kNodeEmitter = 0x0005;     // 00740A20
constexpr U16 kNodeCamera = 0x0009;      // 00740A24
constexpr U16 kNodeReference = 0x0011;   // 00740A28
constexpr U16 kNodeTriMesh = 0x0021;     // 00740A2C
constexpr U16 kNodeSkin = 0x0061;        // 00740A30
constexpr U16 kNodeAnimMesh = 0x00A1;    // 00740A34
constexpr U16 kNodeDangly = 0x0121;      // 00740A38
constexpr U16 kNodeAABB = 0x0221;        // 00740A3C
constexpr U16 kNodeTrigger = 0x0401;     // 00740A40
constexpr U16 kNodeSaber = 0x0821;       // 00740A44
constexpr I16 kSaberWord4C = 0x14;       // 00740A4C (movsx)
constexpr I16 kSaberWord50 = 0x08;       // 00740A50 (movsx)
constexpr I16 kSaberWord54 = 0x04;       // 00740A54 (movsx)

// GLRender vertex pool object (cell 007A68A4, owned by the renderer epoch: InitVertexBufferPool/InitSystemPool).
struct PoolObject;
using PoolRequest = U (__fastcall*)(PoolObject* self, void* unused_edx, U a1, U a2, U a3);   // vtbl+04 (00423EF7)
using PoolLock = void* (__fastcall*)(PoolObject* self, void* unused_edx, U handle, U a4);   // vtbl+08 (00423F22)
using PoolUnlock = U (__fastcall*)(PoolObject* self, void* unused_edx, U handle);           // vtbl+0C (00423F3D)
using PoolDelete = U (__fastcall*)(PoolObject* self, void* unused_edx, U handle);           // vtbl+18 (00423F62)
using PoolResize = U (__fastcall*)(PoolObject* self, void* unused_edx, U handle, U bytes);  // vtbl+1C (00423F82)
struct PoolVTable {
    U slot00;            // not referenced by this lane
    PoolRequest request; // +04
    PoolLock lock;       // +08
    PoolUnlock unlock;   // +0C
    U slot10, slot14;    // not referenced by this lane
    PoolDelete destroy;  // +18
    PoolResize resize;   // +1C
};
struct PoolObject { const PoolVTable* vtable; };
static_assert(offsetof(PoolVTable, request) == 4 && offsetof(PoolVTable, lock) == 8 &&
              offsetof(PoolVTable, unlock) == 0xC && offsetof(PoolVTable, destroy) == 0x18 && offsetof(PoolVTable, resize) == 0x1C, "observed pool slots");

// Function-pointer cells written into relocated trees/nodes (0x0044AA60 etc.); cdecl(object[, line]).
using TreeDestroyFn = void (__cdecl*)(MaxTree*);
using TreeParseFn = void (__cdecl*)(MaxTree*, char*);
using NodeMeshFn = void (__cdecl*)(MdlNode*);
}

extern "C" {
// ---------------- cells defined by this lane (original .data/.bss, zero-initialized like the image) --------
extern aurora_model::MaxTree* volatile aurora_current_model_007fbae4;  // ASCII loader "current model"
extern volatile aurora_model::U aurora_model_no_delete_007a8a04;       // read 0043E210; no writer in image
extern volatile aurora_model::U aurora_model_variants_00828068;        // read 00449D0A; no writer in image
extern volatile aurora_model::U aurora_anim_footprint_007a6990;        // 00437DB0 / 004A0FF2 / 004A1212
extern volatile aurora_model::I aurora_mesh_vertices_00832c80;         // 004A0DEC..004A0E00
extern volatile aurora_model::I aurora_mesh_vertex_bytes_00832c88;     // 004A0DFB..004A0E1B
extern aurora_model::List aurora_io_dispatcher_00830510;               // 004A05AA..004A05BB
extern volatile aurora_model::U8 aurora_io_dispatcher_guard_0083051c;  // 004A0586 / 004A05A4
extern volatile aurora_model::I aurora_input_read_counter_007b932c;    // 004A13AF / 004A13C5

// ---------------- providers authored by this lane ----------------
// cache / dispatch
aurora_model::MaxTree* __cdecl aurora_find_model_00464110(const char* name);
aurora_model::MaxTree* __cdecl aurora_binary_search_model_00464090(const char* name, aurora_model::I lo, aurora_model::I hi);
aurora_model::MaxTree* __cdecl aurora_load_model_00464200(std::FILE* mdl, std::FILE* mdx);
void __cdecl aurora_insert_model_0046a5f0(aurora_model::MaxTree* tree);
void __cdecl aurora_binary_insert_model_0046a590(aurora_model::MaxTree* tree, aurora_model::I lo, aurora_model::I hi);
void __fastcall aurora_list_insert_00445020(aurora_model::List* self, void* unused_edx, aurora_model::U value, aurora_model::I index);
aurora_model::IODispatcher* __cdecl aurora_io_dispatcher_get_004a0580();
void __cdecl aurora_free_io_dispatch_0073be40();
aurora_model::MaxTree* __fastcall aurora_io_read_sync_name_004a15b0(aurora_model::IODispatcher* self, void* unused_edx, const char* name);
aurora_model::MaxTree* __fastcall aurora_io_read_sync_file_004a15d0(aurora_model::IODispatcher* self, void* unused_edx, std::FILE* mdl, std::FILE* mdx);
aurora_model::MaxTree* __fastcall aurora_input_read_name_004a13a0(aurora_model::InputObject* self, void* unused_edx, const char* name);
aurora_model::MaxTree* __fastcall aurora_input_read_file_004a14b0(aurora_model::InputObject* self, void* unused_edx, std::FILE* mdl, std::FILE* mdx);
aurora_model::MaxTree* __fastcall aurora_tree_as_model_0043e1c0(aurora_model::MaxTree* self, void* unused_edx);
aurora_model::MaxTree* __fastcall aurora_tree_as_animation_0043e1d0(aurora_model::MaxTree* self, void* unused_edx);
aurora_model::U __fastcall aurora_tree_get_type_0043e130(aurora_model::MaxTree* self, void* unused_edx);
aurora_model::I __fastcall model_add_ref_0043e240(aurora_model::MaxTree* self, void* unused_edx, void* unused);   // symbol expected by lane-3d-render
aurora_model::MaxTree* __fastcall maxtree_construct_0044a900(aurora_model::MaxTree* self, void* unused_edx, const char* name);
aurora_model::MaxTree* __fastcall model_construct_0044aa70(aurora_model::MaxTree* self, void* unused_edx, const char* name);   // symbol expected by lane-3d-render
aurora_model::I __fastcall aurora_model_del_ref_0043e210(aurora_model::MaxTree* self, void* unused_edx, void* unused);

// binary relocation (InputBinary)
aurora_model::MaxTree* __fastcall aurora_input_binary_read_004a1260(aurora_model::InputObject* self, void* unused_edx, aurora_model::Resource* mdl, aurora_model::Resource* mdx, aurora_model::U8 lite);
aurora_model::MaxTree* __fastcall aurora_input_binary_reset_004a1030(aurora_model::InputObject* self, void* unused_edx, aurora_model::MaxTree* tree, unsigned char* mdx, aurora_model::Resource* res);
aurora_model::MaxTree* __fastcall aurora_input_binary_reset_lite_004a11b0(aurora_model::InputObject* self, void* unused_edx, aurora_model::MaxTree* tree, unsigned char* mdx, aurora_model::Resource* res);
void __fastcall aurora_input_binary_reset_animation_004a0fb0(aurora_model::InputObject* self, void* unused_edx, aurora_model::MaxTree* anim, aurora_model::MaxTree* base, unsigned char* mdx);
void __fastcall aurora_input_binary_reset_node_004a0900(aurora_model::InputObject* self, void* unused_edx, aurora_model::MdlNode* node, aurora_model::MaxTree* base, unsigned char* mdx);
void __fastcall aurora_input_binary_reset_node_lite_004a0a90(aurora_model::InputObject* self, void* unused_edx, aurora_model::MdlNode* node, aurora_model::MaxTree* base, unsigned char* mdx);
void __fastcall aurora_input_binary_reset_node_parts_004a0b60(aurora_model::InputObject* self, void* unused_edx, aurora_model::MdlNode* node, aurora_model::MaxTree* base, unsigned char* mdx);
void __fastcall aurora_input_binary_reset_node_parts_lite_004a0160(aurora_model::InputObject* self, void* unused_edx, aurora_model::MdlNode* node, aurora_model::MaxTree* base, unsigned char* mdx);
void __fastcall aurora_input_binary_reset_trimesh_parts_004a0c00(aurora_model::InputObject* self, void* unused_edx, aurora_model::MdlNode* node, aurora_model::MaxTree* base, unsigned char* mdx);
void __fastcall aurora_input_binary_reset_trimesh_parts_lite_004a0e30(aurora_model::InputObject* self, void* unused_edx, aurora_model::MdlNode* node, aurora_model::MaxTree* base, unsigned char* mdx);
void __fastcall aurora_input_binary_reset_anim_004a0060(aurora_model::InputObject* self, void* unused_edx, aurora_model::MdlNode* node, aurora_model::MaxTree* base, unsigned char* mdx);
void __fastcall aurora_input_binary_reset_dangly_004a0100(aurora_model::InputObject* self, void* unused_edx, aurora_model::MdlNode* node, aurora_model::MaxTree* base, unsigned char* mdx);
void __fastcall aurora_input_binary_reset_skin_004a01b0(aurora_model::InputObject* self, void* unused_edx, aurora_model::MdlNode* node, aurora_model::MaxTree* base, unsigned char* mdx);
void __fastcall aurora_input_binary_reset_trimesh_004a0240(aurora_model::InputObject* self, void* unused_edx, aurora_model::MdlNode* node, aurora_model::MaxTree* base, unsigned char* mdx);
void __fastcall aurora_input_binary_reset_aabb_tree_004a0260(aurora_model::InputObject* self, void* unused_edx, unsigned char* aabb, aurora_model::MaxTree* base, unsigned char* mdx);
void __fastcall aurora_input_binary_reset_saber_004a0460(aurora_model::InputObject* self, void* unused_edx, aurora_model::MdlNode* node, aurora_model::MaxTree* base, unsigned char* mdx);
void __fastcall aurora_input_binary_reset_saber_lite_004a0510(aurora_model::InputObject* self, void* unused_edx, aurora_model::MdlNode* node, aurora_model::MaxTree* base, unsigned char* mdx);
void __fastcall aurora_input_binary_reset_light_004a05e0(aurora_model::InputObject* self, void* unused_edx, aurora_model::MdlNode* node, aurora_model::MaxTree* base, unsigned char* mdx);
void __fastcall aurora_input_binary_reset_light_lite_004a07b0(aurora_model::InputObject* self, void* unused_edx, aurora_model::MdlNode* node, aurora_model::MaxTree* base, unsigned char* mdx);

// tree / node bookkeeping
void __fastcall aurora_tree_reset_free_lists_004451d0(aurora_model::MaxTree* self, void* unused_edx);
void __fastcall aurora_tree_add_free_token_00445190(aurora_model::MaxTree* self, void* unused_edx, aurora_model::U token);
void __fastcall aurora_node_add_free_token_00445310(aurora_model::MdlNode* self, void* unused_edx, aurora_model::U token);
void __fastcall aurora_node_add_free_list_004452d0(aurora_model::MdlNode* self, void* unused_edx, void* item);
const char* __fastcall aurora_node_translate_name_00438a00(aurora_model::MdlNode* self, void* unused_edx);
void __cdecl aurora_update_anim_footprint_00437db0(aurora_model::MdlNode* node, aurora_model::I scale);
aurora_model::MdlNode* __fastcall aurora_node_as_aabb_0043e340(aurora_model::MdlNode* self, void* unused_edx);
aurora_model::MdlNode* __fastcall aurora_node_as_anim_mesh_0043e360(aurora_model::MdlNode* self, void* unused_edx);
aurora_model::MdlNode* __fastcall aurora_node_as_camera_0043e370(aurora_model::MdlNode* self, void* unused_edx);
aurora_model::MdlNode* __fastcall aurora_node_as_dangly_0043e380(aurora_model::MdlNode* self, void* unused_edx);
aurora_model::MdlNode* __fastcall aurora_node_as_saber_0043e3a0(aurora_model::MdlNode* self, void* unused_edx);
aurora_model::MdlNode* __fastcall aurora_node_as_emitter_0043e3c0(aurora_model::MdlNode* self, void* unused_edx);
aurora_model::MdlNode* __fastcall aurora_node_as_light_0043e3d0(aurora_model::MdlNode* self, void* unused_edx);
aurora_model::MdlNode* __fastcall aurora_node_as_reference_0043e3e0(aurora_model::MdlNode* self, void* unused_edx);
aurora_model::MdlNode* __fastcall aurora_node_as_skin_0043e3f0(aurora_model::MdlNode* self, void* unused_edx);
aurora_model::MdlNode* __fastcall aurora_node_as_trimesh_0043e400(aurora_model::MdlNode* self, void* unused_edx);

// GLRender vertex pool cdecl wrappers
aurora_model::U __cdecl aurora_pool_request_00423ee0(aurora_model::U a1, aurora_model::U a2, aurora_model::U a3, aurora_model::U unused4, aurora_model::U* out);
void* __cdecl aurora_pool_lock_00423f10(aurora_model::U handle, aurora_model::U unused2, aurora_model::U unused3, aurora_model::U a4);
aurora_model::U __cdecl aurora_pool_unlock_00423f30(aurora_model::U handle);
aurora_model::U __cdecl aurora_pool_delete_00423f50(aurora_model::U handle);
aurora_model::U __cdecl gl_render_resize_pool_00423f70(aurora_model::U handle, aurora_model::U bytes);

// function-pointer thunks stored into relocated objects (cdecl(object) -> thiscall worker)
void __cdecl aurora_model_destructor_0044aa60(aurora_model::MaxTree* tree);
void __cdecl aurora_model_parse_field_0043e1e0(aurora_model::MaxTree* tree, char* line);
void __cdecl aurora_maxtree_destructor_0044a8e0(aurora_model::MaxTree* tree);
void __cdecl aurora_maxtree_parse_field_0043e190(aurora_model::MaxTree* tree, char* line);
void __cdecl aurora_trimesh_gen_vertices_0043e480(aurora_model::MdlNode* node);
void __cdecl aurora_trimesh_remove_temporary_0043e490(aurora_model::MdlNode* node);
void __cdecl aurora_anim_mesh_gen_vertices_0043e420(aurora_model::MdlNode* node);
void __cdecl aurora_anim_mesh_remove_temporary_0043e430(aurora_model::MdlNode* node);
void __cdecl aurora_skin_gen_vertices_0043e440(aurora_model::MdlNode* node);
void __cdecl aurora_skin_remove_temporary_0043e450(aurora_model::MdlNode* node);
void __cdecl aurora_dangly_remove_temporary_0043e460(aurora_model::MdlNode* node);
void __cdecl aurora_dangly_gen_vertices_0043e470(aurora_model::MdlNode* node);

// ---------------- imports (other lanes / M41 / donors) ----------------
// lane-aurres (in progress; symbol spellings from lane-aurres/source/aurres.hpp)
void* __cdecl aurres_data_bytes_0044bd10(aurora_model::U count, void* descriptor);
void __cdecl aurres_free_data_bytes_0044bd50(void* descriptor, void* bytes);
void __cdecl aurres_remove_0044bdc0(void* descriptor);
void* __cdecl aurres_get_0044c740(const char* name, const char* ext, aurora_model::I volatile* size, aurora_model::U memory_only);
void* __cdecl aurres_get_file_0044c870(std::FILE* file);
aurora_model::U __cdecl aurres_release_0044c8f0(void* descriptor, aurora_model::I size);
char* __cdecl aurres_dofile_0044c980(const char* file_name);
// lane-3d-render (authored there): CExoArrayList::~CExoArrayList (null-checked) 0047ABC0
void __fastcall exo_array_list_free_0047abc0(void* list, void* unused_edx);
// M41-defined
void* __cdecl live_operator_new(std::uint32_t size);        // 006FA7E6 operator new (and 006FC0D2 jmp thunk)
void __cdecl live_free(void* block);                        // 006FA390 free (and 006FC0CD jmp thunk)
std::int32_t __cdecl kotor_stricmp_0070acaf(const char*, const char*);
// task-7 cpp-range-storage donor (single owner of the 12-byte historical extent 007A8A64; not in M41)
// declared through the task-7 header type in model_cache.cpp.
// renderer epoch (not in M41): GLRender vertex pool object pointer cell
extern aurora_model::PoolObject* volatile kotor_vertex_pool_007a68a4;
// texture lane (not in M41)
// AurTextureGetReference 00423A60: font::reference_00423a60 (task-33 font.hpp declaration; texture lane)
void __fastcall aurora_texture_ptr_list_allocate_0043de00(void* self, void* unused_edx, aurora_model::I count);   // CExoArrayList<SafePointer<CAurTexture>>::allocate
void __fastcall aurora_texture_ptr_list_set_size_0043df60(void* self, void* unused_edx, aurora_model::I count);   // ::SetSize
void __fastcall aurora_texture_safe_pointer_destroy_00422380(void* self, void* unused_edx);                       // SafePointer<CAurTexture>::~SafePointer
void __stdcall c2da_destroy_array_006fa852(void* first, aurora_model::U size, aurora_model::I count, void (__fastcall* destroy)(void*, void*));
// Model destruction family (Tier D, not authored here)
void __fastcall aurora_model_destroy_0043f790(aurora_model::MaxTree* self, void* unused_edx);       // Model::~Model
void __cdecl aurora_maxtree_delete_0044aec0(aurora_model::MaxTree* tree);                          // MaxTree::operator delete
void __fastcall aurora_model_destroy_body_00449f60(aurora_model::MaxTree* self, void* unused_edx);  // ModelDestructor worker
void __fastcall aurora_node_destructor_dispatch_004488f0(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall aurora_model_parse_field_body_004688a0(aurora_model::MaxTree* self, void* unused_edx, char* line);
void __fastcall aurora_maxtree_parse_field_body_00468870(aurora_model::MaxTree* self, void* unused_edx, char* line);
// mesh vertex preparation workers (renderer/mesh lane, not authored here)
void __fastcall aurora_trimesh_gen_vertices_body_00439df0(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall aurora_trimesh_remove_temporary_body_0043c090(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall aurora_anim_mesh_gen_vertices_body_0043d450(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall aurora_anim_mesh_remove_temporary_body_0043c320(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall aurora_skin_gen_vertices_body_004885e0(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall aurora_skin_remove_temporary_body_0043c1a0(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall aurora_dangly_remove_temporary_body_0043c130(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall aurora_dangly_gen_vertices_body_0043bd40(aurora_model::MdlNode* self, void* unused_edx);
}

namespace kotor_renderer_gl {
// D32: CExoArrayList::allocate 00437D60 owner = lane-renderer-bodies (src/cohort/render_pool.h).
void __fastcall allocate_00437d60(kotor_reconstruction::ArrayMutations* self, void* unused_edx, std::uint32_t capacity);
}

namespace menu_list_growth {
// M41-defined (viewport-integration list-growth unit): CExoArrayList<DWORD>::Allocate 005E03C0.
__declspec(noinline) void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, std::uint32_t);
}
