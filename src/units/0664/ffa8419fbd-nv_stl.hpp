// nv_stl.hpp - lane-3d-model rev2: views and declarations for the VC7.1 (Dinkumware) standard-library
// instantiations and the NvTriStrip (NvStripifier / NvStripInfo) code reached from AurStripify 00481140
// (MdlNodeTriMesh::BuildVertexIndices 0043C53F). Never executed at runtime (cell 007A69A8 has no writer) but
// link-required. Layouts are the original 32-bit ones; every function keeps its original address in its name.
#pragma once
#include "5d11c14380-model_load.hpp"

namespace nv {
using aurora_model::U;
using aurora_model::I;
using aurora_model::U8;
using aurora_model::U16;
using aurora_model::get;
using aurora_model::put;
using aurora_model::ptr;
using aurora_model::addr;

// std::vector<T> (VC7.1): empty allocator (padded to 4) + _Myfirst / _Mylast / _Myend.
struct Vec { U alval; U first; U last; U end; };
static_assert(sizeof(Vec) == 16, "x86 vector layout");
// std::set<T*> red-black node and tree (VC7.1 _Tree): color 0 = red, 1 = black; _Isnil at +11h.
struct Node { Node* left; Node* parent; Node* right; U value; char color; char isnil; };
static_assert(sizeof(Node) == 0x14, "x86 tree node layout");
struct Tree { U alval; Node* head; U size; };
static_assert(sizeof(Tree) == 12, "x86 tree layout");
// pair<iterator, bool> returned by set::insert.
struct InsertResult { Node* where; U8 inserted; };
// 24-byte element of the AurStripify face vector (copied as six dwords: rep movsd, ecx = 6).
struct Elem24 { U w[6]; };
static_assert(sizeof(Elem24) == 24, "24-byte element");

constexpr char kRed = 0;
constexpr char kBlack = 1;
}

using aurora_model::U16;
using aurora_model::U8;

// C++ linkage: several of these throw (std::length_error / std::out_of_range), which /EHsc forbids for extern "C".
// ---------------------------------------------------------------- std::vector<unsigned short>
U16* __cdecl nv_ushort_uninit_copy_0047be70(const U16* first, const U16* last, U16* dest, void* alloc, aurora_model::U unused);
void __cdecl nv_ushort_uninit_fill_n_0047c1d0(U16* dest, aurora_model::U count, const U16* value, void* alloc, aurora_model::U unused);
U16* __cdecl nv_ushort_copy_0047cc30(const U16* first, const U16* last, U16* dest);
void __cdecl nv_ushort_fill_0047bde0(U16* first, U16* last, const U16* value);
U16** __cdecl nv_ushort_copy_backward_0047c0e0(U16** result, U16* first, U16* last, U16* dest);
U16* __fastcall nv_ushort_vec_ucopy_0047cc60(nv::Vec* self, void* unused_edx, const U16* first, const U16* last, U16* dest);
U16* __fastcall nv_ushort_vec_ucopy_0047cc80(nv::Vec* self, void* unused_edx, const U16* first, const U16* last, U16* dest);
U16* __fastcall nv_ushort_vec_ufill_0047ce40(nv::Vec* self, void* unused_edx, U16* dest, aurora_model::U count, const U16* value);
void __fastcall nv_ushort_vec_insert_n_0047d510(nv::Vec* self, void* unused_edx, U16* where, aurora_model::U count, const U16* value);
U16** __fastcall nv_ushort_vec_erase_0047cbc0(nv::Vec* self, void* unused_edx, U16** result, U16* first, U16* last);
void __fastcall nv_ushort_vec_reserve_0047db90(nv::Vec* self, void* unused_edx, aurora_model::U count);
void __fastcall nv_ushort_vec_resize_0047dd40(nv::Vec* self, void* unused_edx, aurora_model::U count, U16 value);
void __fastcall nv_ushort_vec_push_back_0047e5b0(nv::Vec* self, void* unused_edx, const U16* value);
// ---------------------------------------------------------------- std::vector<T*> (NvFaceInfo* / NvEdgeInfo* / NvStripInfo*)
aurora_model::U* __cdecl nv_ptr_uninit_copy_0047be40(const aurora_model::U* first, const aurora_model::U* last, aurora_model::U* dest, void* alloc, aurora_model::U unused);
void __cdecl nv_ptr_uninit_fill_n_0047c1a0(aurora_model::U* dest, aurora_model::U count, const aurora_model::U* value, void* alloc, aurora_model::U unused);
void __cdecl nv_ptr_fill_0047c110(aurora_model::U* first, aurora_model::U* last, const aurora_model::U* value);
aurora_model::U** __cdecl nv_ptr_copy_backward_0047c0b0(aurora_model::U** result, aurora_model::U* first, aurora_model::U* last, aurora_model::U* dest);
aurora_model::U* __fastcall nv_ptr_vec_ucopy_0047cc00(nv::Vec* self, void* unused_edx, const aurora_model::U* first, const aurora_model::U* last, aurora_model::U* dest);
aurora_model::U* __fastcall nv_ptr_vec_ufill_0047ce10(nv::Vec* self, void* unused_edx, aurora_model::U* dest, aurora_model::U count, const aurora_model::U* value);
void __fastcall nv_ptr_vec_insert_n_0047de10(nv::Vec* self, void* unused_edx, aurora_model::U* where, aurora_model::U count, const aurora_model::U* value);
aurora_model::U** __fastcall nv_ptr_vec_erase_0047d0b0(nv::Vec* self, void* unused_edx, aurora_model::U** result, aurora_model::U* first, aurora_model::U* last);
void __fastcall nv_ptr_vec_reserve_0047daa0(nv::Vec* self, void* unused_edx, aurora_model::U count);
void __fastcall nv_ptr_vec_resize_0047e630(nv::Vec* self, void* unused_edx, aurora_model::U count, aurora_model::U value);
void __fastcall nv_ptr_vec_push_back_0047ec60(nv::Vec* self, void* unused_edx, const aurora_model::U* value);
// ---------------------------------------------------------------- std::vector<24-byte element>
nv::Elem24* __cdecl nv_e24_uninit_copy_0047bea0(const nv::Elem24* first, const nv::Elem24* last, nv::Elem24* dest, void* alloc, aurora_model::U unused);
void __cdecl nv_e24_uninit_fill_n_0047c200(nv::Elem24* dest, aurora_model::U count, const nv::Elem24* value, void* alloc, aurora_model::U unused);
void __cdecl nv_e24_fill_0047c130(nv::Elem24* first, nv::Elem24* last, const nv::Elem24* value);
nv::Elem24** __cdecl nv_e24_copy_backward_0047be00(nv::Elem24** result, nv::Elem24* first, nv::Elem24* last, nv::Elem24* dest, aurora_model::U unused);
nv::Elem24** __cdecl nv_e24_copy_backward_0047c160(nv::Elem24** result, nv::Elem24* first, nv::Elem24* last, nv::Elem24* dest);
nv::Elem24* __fastcall nv_e24_vec_ucopy_0047ccb0(nv::Vec* self, void* unused_edx, const nv::Elem24* first, const nv::Elem24* last, nv::Elem24* dest);
nv::Elem24* __fastcall nv_e24_vec_ufill_0047ce70(nv::Vec* self, void* unused_edx, nv::Elem24* dest, aurora_model::U count, const nv::Elem24* value);
aurora_model::U __fastcall nv_e24_vec_size_0047bab0(nv::Vec* self, void* unused_edx);
void __fastcall nv_e24_vec_insert_n_0047e170(nv::Vec* self, void* unused_edx, nv::Elem24* where, aurora_model::U count, const nv::Elem24* value);
nv::Elem24** __fastcall nv_e24_vec_insert_0047e6b0(nv::Vec* self, void* unused_edx, nv::Elem24** result, nv::Elem24* where, const nv::Elem24* value);
void __fastcall nv_e24_vec_push_back_0047ecd0(nv::Vec* self, void* unused_edx, const nv::Elem24* value);
// ---------------------------------------------------------------- vector length error (_Xlen)
[[noreturn]] void __fastcall nv_vector_xlen_0047d2b0(void* self, void* unused_edx);
// ---------------------------------------------------------------- std::set<T*>
nv::Node* __cdecl nv_tree_buy_head_0047c030();
nv::Node* __fastcall nv_tree_buy_node_0047bed0(nv::Tree* self, void* unused_edx, nv::Node* left, nv::Node* parent, nv::Node* right, const aurora_model::U* value, char color);
void __fastcall nv_tree_erase_subtree_0047c070(nv::Tree* self, void* unused_edx, nv::Node* root);
void __fastcall nv_tree_iter_inc_0047bb20(nv::Node** self, void* unused_edx);
void __fastcall nv_tree_iter_dec_0047bd80(nv::Node** self, void* unused_edx);
nv::Node* __cdecl nv_tree_min_0047bb00(nv::Node* node);
nv::Node* __cdecl nv_tree_max_0047bae0(nv::Node* node);
void __fastcall nv_tree_lrotate_0047bcc0(nv::Tree* self, void* unused_edx, nv::Node* where);
void __fastcall nv_tree_rrotate_0047bd20(nv::Tree* self, void* unused_edx, nv::Node* where);
nv::Node** __fastcall nv_tree_find_0047ca80(nv::Tree* self, void* unused_edx, nv::Node** result, const aurora_model::U* key);
nv::Node** __fastcall nv_tree_insert_at_0047d330(nv::Tree* self, void* unused_edx, nv::Node** result, U8 add_left, nv::Node* where, const aurora_model::U* value);
nv::InsertResult* __fastcall nv_tree_insert_0047dc80(nv::Tree* self, void* unused_edx, nv::InsertResult* result, const aurora_model::U* value);
nv::Node** __fastcall nv_tree_erase_0047d7b0(nv::Tree* self, void* unused_edx, nv::Node** result, nv::Node* where);
nv::Node** __fastcall nv_tree_erase_range_0047e0b0(nv::Tree* self, void* unused_edx, nv::Node** result, nv::Node* first, nv::Node* last);
