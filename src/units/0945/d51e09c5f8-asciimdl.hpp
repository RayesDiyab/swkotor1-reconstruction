// asciimdl.hpp - lane-m48-asciimdl: shared declarations of the ASCII .mdl node parser family of the GOG swkotor.exe
// (image base 0x400000): ParseNode 004680E0 and its static closure (tools/closure.json, all Ghidra ADDRESS_RANGEs).
// Off the menu path (the menu loads binary MDL); required for a clean static link.
// Conventions (all units of this lane):
//  * original __thiscall bodies -> __fastcall(self, void* unused_edx, ...) (D1; identical ECX receiver, stack layout,
//    callee cleanup; callees never read EDX; no struct-by-value return through D1);
//  * objects are raw observed-offset storage through model_load.hpp get/put/at/ptr (no invented layout);
//  * allocator family: 006FA7E6 operator new / 006FC0D2 thunk -> live_operator_new, 006FA390 free / 006FC0CD thunk ->
//    live_free (M41); original operator new returns NULL (D30-FINAL);
//  * floats: double intermediates, float only at original fstp dword stores (D30-FINAL);
//  * sscanf 006FAE08 -> existing provider kotor_sscanf_006fae08 (font-txi-readers; UCRT-backed with the lane-font
//    equivalence record for %s/%d/%c/%f; formats used here: tools/formats.txt);
//  * no dynamic initializers: every cell is constant-initialised.
#pragma once
#include "5fb819dbd7-model_load.hpp"

namespace asciimdl {
using namespace aurora_model;
inline List* list_at(void* p, U off) { return reinterpret_cast<List*>(at(p, off)); }
}

extern "C" {
// ===================================================================== existing providers (M47 objects; identical
// extern "C" declarations to the units cited)
char* __cdecl firstword_00463530(char* line);                                                    // font-txi-readers
void __cdecl parse_cstr_00463590(char* text, char* out);                                         // font-txi-readers
void __cdecl parse_float_004635c0(char* text, volatile float* out);                              // font-txi-readers
void __cdecl parse_int_004635e0(char* text, volatile std::int32_t* out);                         // font-txi-readers
void __cdecl parse_bool_00463680(char* text, volatile bool* out);                                // font-txi-readers
char* __cdecl skip_blanks_00463510(char* text);                                                  // font-txi-readers
int __cdecl kotor_sscanf_006fae08(const char* buffer, const char* format, ...);                  // font-txi-readers
std::int32_t __cdecl kotor_strncmp_006faff0(const char* first, const char* second, aurora_model::U count); // tlk-parse
char* __cdecl resource_next_0044bfa0();                                                          // aurres-stream
void* __cdecl resource_read_0044bfd0(aurora_model::U count);                                     // aurres-stream (AurResGetData)
aurora_model::List* __fastcall float_list_assign_00438a50(aurora_model::List* self, void* unused_edx, const aurora_model::List* source); // core46-math
void __fastcall texture_list_remove_00445080(aurora_model::List* list, void* unused_edx, aurora_model::U value); // texture-epoch-texture_x86

// 3d-model-model-destroy / 3d-construct-light-nodes (destructors used by ParseNode)
void __fastcall aurora_node_destruct_0044a040(aurora_model::MdlNode* self, void*);
void __fastcall mdl_node_lightsaber_destruct_00449090(aurora_model::MdlNode* self, void*);
void __fastcall aurora_trimesh_destruct_00448bf0(aurora_model::MdlNode* self, void*);
void __fastcall aurora_anim_mesh_destruct_00448da0(aurora_model::MdlNode* self, void*);
void __fastcall aurora_skin_destruct_00448e60(aurora_model::MdlNode* self, void*);
void __fastcall aurora_dangly_destruct_004490f0(aurora_model::MdlNode* self, void*);
void __fastcall aurora_aabb_destruct_00449170(aurora_model::MdlNode* self, void*);
void __fastcall aurora_light_node_destruct_00448f80(aurora_model::MdlNode* self, void*);
// m47-anim-lookat (single owner of 00445210)
aurora_model::MdlNode* __fastcall mdl_node_construct_00445210(void* self, void* unused_edx, const char* name);

// ===================================================================== cells owned by this lane (unit mdl_nodes.cpp)
// 007B927C BSS byte: read by ParseNode (00468848) and the InternalParseField bodies (00465561, 004658B1, 00468071,
// 00469151, 0046980F, 0046A161, 0046A241, 0046A2F1, 0046A3C1); no writer instruction in the image -> 0. Meaning unknown.
extern volatile aurora_model::U8 mdl_parse_flag_007b927c;
// 0078E33C..0078E353 .data: six DWORDs (0.8f x3, 0.2f x3) copied by MdlNodeTriMesh::MdlNodeTriMesh 0044A7A6..0044A82D;
// no other reference in the image.
extern volatile aurora_model::U trimesh_default_colors_0078e33c[6];

// ===================================================================== unit mdl_nodes.cpp (integrator lane)
void __cdecl aurora_parse_node_004680e0(char* text, aurora_model::MaxTree* tree);   // consumer decl f3_mdl_node.cpp:33
aurora_model::MdlNode* __fastcall mdl_node_camera_construct_0044a280(void* self, void* unused_edx, const char* name);
aurora_model::MdlNode* __fastcall mdl_node_emitter_construct_0044a300(void* self, void* unused_edx, const char* name);
aurora_model::MdlNode* __fastcall mdl_node_light_construct_0044a3f0(void* self, void* unused_edx, const char* name);
aurora_model::MdlNode* __fastcall mdl_node_reference_construct_0044a5c0(void* self, void* unused_edx, const char* name);
aurora_model::MdlNode* __fastcall mdl_node_trimesh_construct_0044a640(void* self, void* unused_edx, const char* name);
aurora_model::MdlNode* __fastcall mdl_node_aabb_construct_0044a8b0(void* self, void* unused_edx, const char* name);
aurora_model::MdlNode* __fastcall mdl_node_anim_mesh_construct_0044abc0(void* self, void* unused_edx, const char* name);
aurora_model::MdlNode* __fastcall mdl_node_skin_construct_0044ac70(void* self, void* unused_edx, const char* name);
aurora_model::MdlNode* __fastcall mdl_node_lightsaber_construct_0044ad80(void* self, void* unused_edx, const char* name);
aurora_model::MdlNode* __fastcall mdl_node_dangly_construct_0044ae00(void* self, void* unused_edx, const char* name);
void __fastcall mdl_node_parse_field_dispatch_0043fa10(aurora_model::MdlNode* self, void* unused_edx, char* line); // RET 4
void __fastcall mdl_node_post_process_dispatch_0043fb90(aurora_model::MdlNode* self, void* unused_edx);            // RET 0
void* __fastcall mdl_trimesh_postprocess_construct_00445720(void* self, void* unused_edx);                          // RET 0, EAX=this
float* __fastcall vector_divide_assign_004a97d0(float* self, void* unused_edx, float divisor);                      // RET 4, EAX=this
void __fastcall mdl_node_internal_parse_field_thunk_00468060(aurora_model::MdlNode* self, void* unused_edx, char* line);    // jmp 00465560
void __fastcall mdl_trimesh_internal_parse_field_thunk_0046a150(aurora_model::MdlNode* self, void* unused_edx, char* line); // jmp 00469700
void __fastcall mdl_emitter_internal_post_process_00437c10(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall mdl_skin_internal_post_process_0043b350(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall mdl_light_internal_post_process_0043dfc0(aurora_model::MdlNode* self, void* unused_edx);
// MdlNodeEmitter::Enable* (thiscall RET 0: or dword [ecx+12Ch], bit)
void __fastcall mdl_emitter_enable_point_to_point_0048d790(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall mdl_emitter_enable_point_to_point_sel_0048d7a0(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall mdl_emitter_enable_affected_by_wind_0048d7b0(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall mdl_emitter_enable_tinted_0048d7c0(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall mdl_emitter_enable_bouncing_0048d7d0(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall mdl_emitter_enable_random_0048d7e0(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall mdl_emitter_enable_random_order_0048d7f0(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall mdl_emitter_enable_inheriting_0048d800(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall mdl_emitter_enable_inheriting_velocity_0048d810(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall mdl_emitter_enable_inheriting_local_0048d820(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall mdl_emitter_enable_splat_0048d830(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall mdl_emitter_enable_inheriting_part_0048d840(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall mdl_emitter_enable_depth_texture_0048d850(aurora_model::MdlNode* self, void* unused_edx);

// ===================================================================== unit mdl_parse_helpers.cpp (fork B)
// cdecl Parse<T>(text, list): list header "%d %c" / per-line or binary payload (all RET 0, no used EAX)
void __cdecl mdl_parse_float_list_004628c0(char* text, aurora_model::List* list);
void __cdecl mdl_parse_int_list_00464630(char* text, aurora_model::List* list);
void __cdecl mdl_parse_cstr_list_00464880(char* text, aurora_model::List* list);
void __cdecl mdl_parse_uint_ptr_list_00464a40(char* text, aurora_model::List* list);
void __cdecl mdl_parse_maxface_list_00468a00(char* text, aurora_model::List* list);
void __cdecl mdl_parse_uint_list_00468b90(char* text, aurora_model::List* list);
void __cdecl mdl_parse_skin_weight_list_00468d70(char* text, aurora_model::List* list);
void __cdecl mdl_parse_quaternion_list_00468f60(char* text, aurora_model::List* list);
// cdecl element parsers
void __cdecl mdl_parse_quaternion_00463a30(char* text, void* out);
void __cdecl mdl_parse_aabb_00463c20(char* text, void* out_slot);
aurora_model::MdlNode* __cdecl mdl_find_node_00464af0(aurora_model::MdlNode* root, const char* name);
// cdecl controller builders (6 args, RET 0, EAX unused by every caller)
void __cdecl mdl_add_generic_controller_single_00464e40(char* keyword, char* text, aurora_model::List* controllers,
                                                       aurora_model::List* data, aurora_model::I type, aurora_model::I index);
void __cdecl mdl_add_generic_controller_00465090(char* keyword, char* text, aurora_model::List* controllers,
                                                aurora_model::List* data, aurora_model::I type, aurora_model::I index);
void __cdecl mdl_add_bezier_controller_00465320(char* keyword, char* text, aurora_model::List* controllers,
                                               aurora_model::List* data, aurora_model::I type, aurora_model::I index);
// thiscall list helpers used across units
aurora_model::List* __fastcall controller_list_assign_004642b0(aurora_model::List* self, void* unused_edx, const aurora_model::List* source); // RET 4
void __fastcall mdl_maxface_list_set_size_00464350(aurora_model::List* self, void* unused_edx, aurora_model::I size);                       // RET 4

// ===================================================================== unit mdl_parse_fields.cpp (fork C)
void __fastcall mdl_node_internal_parse_field_00465560(aurora_model::MdlNode* self, void* unused_edx, char* line);       // RET 4
void __fastcall mdl_reference_internal_parse_field_00468070(aurora_model::MdlNode* self, void* unused_edx, char* line);  // RET 4
void __fastcall mdl_light_internal_parse_field_00469150(aurora_model::MdlNode* self, void* unused_edx, char* line);      // RET 4
void __fastcall mdl_trimesh_internal_parse_field_00469700(aurora_model::MdlNode* self, void* unused_edx, char* line);   // RET 4
void __fastcall mdl_dangly_internal_parse_field_0046a160(aurora_model::MdlNode* self, void* unused_edx, char* line);    // RET 4
void __fastcall mdl_anim_mesh_internal_parse_field_0046a240(aurora_model::MdlNode* self, void* unused_edx, char* line); // RET 4
void __fastcall mdl_skin_internal_parse_field_0046a2f0(aurora_model::MdlNode* self, void* unused_edx, char* line);      // RET 4
void __fastcall mdl_aabb_internal_parse_field_0046a3c0(aurora_model::MdlNode* self, void* unused_edx, char* line);      // RET 4

// ===================================================================== unit mdl_parse_emitter.cpp (fork E)
void __fastcall mdl_emitter_internal_parse_field_004658b0(aurora_model::MdlNode* self, void* unused_edx, char* line);   // RET 4

// ===================================================================== unit mdl_postprocess.cpp (fork D)
void __fastcall mdl_trimesh_process_verts_0043a120(aurora_model::MdlNode* self, void* unused_edx);                      // RET 0
void __fastcall mdl_saber_internal_post_process_0043b380(aurora_model::MdlNode* self, void* unused_edx);                // RET 0
void __fastcall mdl_trimesh_internal_post_process_0043cf00(aurora_model::MdlNode* self, void* unused_edx);              // RET 0
void __fastcall mdl_dangly_internal_post_process_0043d990(aurora_model::MdlNode* self, void* unused_edx);               // RET 0
}
