#pragma once
// lane-m47-core: CAuroraInterface resource-helper family (MDL 07D2, MDX 0BC0, VIS 0BB9, TGA 0003, PLT 0006,
// DDS 07F1, 4PC 080B) - the remaining CResHelper<CResX,T> instances of the template whose TPC (0BBF) and
// TXI (07E6) instances are owned by lane-tpc (tpc-lane/tpc/tpc_helper.cpp, tpc_txi.cpp).
// Instruction-shape equivalence of every member with the reviewed TXI instance was checked by
// tools/norm.py (lane family-shapes notes in NOTES.md): only the tag word, CRes size, table and callee
// addresses differ; per-instance values are cited at each entry in aurora_helpers.cpp / aurora_cres.cpp.
// Helper layout (all seven): 0x1C bytes, operator new(1Ch) at the getters (e.g. 0070E14A), fields per
// ctor 0070EFA0 (methods +0, requested +4, resource +8, CResRef +C) - identical to tpc.hpp Txi.
#include "95ef7f858108789a_tpc.hpp"
#include "adbb58d5cea769fb_read_callback.hpp"

namespace font_callback_read {
#define KOTOR_M47_HELPER_LAYOUT                                                                  \
    const void* volatile methods;     /* +0  base / derived one-slot table             */       \
    volatile Word requested;          /* +4                                             */       \
    void* volatile resource;          /* +8  CResX*                                     */       \
    menu_resources::Ref reference;    /* +C  CResRef[16]                                */
struct Model { KOTOR_M47_HELPER_LAYOUT };              // CResHelper<CResMDL,0x07D2> (00710300)
struct ModelX { KOTOR_M47_HELPER_LAYOUT };             // CResHelper<CResMDX,0x0BC0> (0070FFE0)
struct Visibility { KOTOR_M47_HELPER_LAYOUT };         // CResHelper<CResVIS,0x0BB9> (0070F270)
struct Texture { KOTOR_M47_HELPER_LAYOUT };            // CResHelper<CResTGA,0x0003> (0070EFA0)
struct LayeredTexture { KOTOR_M47_HELPER_LAYOUT };     // CResHelper<CResPLT,0x0006> (0070DD60)
struct CompressedTexture { KOTOR_M47_HELPER_LAYOUT };  // CResHelper<CResDDS,0x07F1> (007106B0)
struct Texture16 { KOTOR_M47_HELPER_LAYOUT };          // CResHelper<CRes4PC,0x080B> (00710A90)
#undef KOTOR_M47_HELPER_LAYOUT
static_assert(sizeof(Model) == 0x1c && offsetof(Model, resource) == 8 && offsetof(Model, reference) == 12 &&
              sizeof(Texture16) == 0x1c, "observed CResHelper ABI (same as tpc.hpp Txi)");
}

namespace aurora_family {
using font_callback_read::Word;
using font_callback_read::Signed;
using font_callback_read::String;
using tpc_lower::HelperMethods;
using tpc_lower::ResourceMethods;
using tpc_lower::DeletingDestroy;
using tpc_lower::load;
using tpc_lower::store;
using tpc_lower::call_slot0;
using ResourceCtor = void* (__fastcall*)(void*, void*);
}

extern "C" {
// ---------------------------------------------------------------- helper tables (one slot each)
extern const tpc_lower::HelperMethods aurora_layered_helper_methods_0075fb4c, aurora_layered_methods_0075fb50;
extern const tpc_lower::HelperMethods aurora_texture_helper_methods_0075fb54, aurora_texture_methods_0075fb58;
extern const tpc_lower::HelperMethods aurora_visibility_helper_methods_0075fb5c, aurora_visibility_methods_0075fb60;
extern const tpc_lower::HelperMethods aurora_modelx_helper_methods_0075fb74, aurora_modelx_methods_0075fb78;
extern const tpc_lower::HelperMethods aurora_model_helper_methods_0075fb7c, aurora_model_methods_0075fb80;
extern const tpc_lower::HelperMethods aurora_compressed_helper_methods_0075fb84, aurora_compressed_methods_0075fb88;
extern const tpc_lower::HelperMethods aurora_texture16_helper_methods_0075fb8c, aurora_texture16_methods_0075fb90;
// ---------------------------------------------------------------- CRes tables (five slots each)
extern const tpc_lower::ResourceMethods aurora_mdl_resource_methods_0074c404;
extern const tpc_lower::ResourceMethods aurora_plt_resource_methods_0075fb94;
extern const tpc_lower::ResourceMethods aurora_vis_resource_methods_0075fba8;
extern const tpc_lower::ResourceMethods aurora_mdx_resource_methods_0075fbd0;
extern const tpc_lower::ResourceMethods aurora_dds_resource_methods_0075fbe4;
extern const tpc_lower::ResourceMethods aurora_4pc_resource_methods_0075fbf8;
extern const tpc_lower::ResourceMethods aurora_tga_resource_methods_0075fc48;

// ---------------------------------------------------------------- CRes subclasses (aurora_cres.cpp)
void* __fastcall aurora_mdl_resource_ctor_005cea50(void*, void*);
void __fastcall aurora_mdl_resource_destroy_005cea80(void*, void*) noexcept;
void* __fastcall aurora_mdl_resource_deleting_destroy_005cea90(void*, void*, tpc_lower::Word) noexcept;
void* __fastcall aurora_vis_resource_ctor_00710d50(void*, void*);
void __fastcall aurora_vis_resource_destroy_00710d80(void*, void*) noexcept;
void* __fastcall aurora_vis_resource_deleting_destroy_00710d90(void*, void*, tpc_lower::Word) noexcept;
void* __fastcall aurora_mdx_resource_ctor_00710e10(void*, void*);
void __fastcall aurora_mdx_resource_destroy_00710e40(void*, void*) noexcept;
void* __fastcall aurora_mdx_resource_deleting_destroy_00710e80(void*, void*, tpc_lower::Word) noexcept;
void* __fastcall aurora_plt_resource_ctor_00710b50(void*, void*);
void __fastcall aurora_plt_resource_destroy_00710b80(void*, void*) noexcept;
void* __fastcall aurora_plt_resource_deleting_destroy_00710ca0(void*, void*, tpc_lower::Word) noexcept;
tpc_lower::Word __fastcall aurora_plt_resource_freed_00710bd0(void*, void*);
tpc_lower::Word __fastcall aurora_plt_resource_serviced_00710cc0(void*, void*) noexcept(false);
tpc_lower::Word __fastcall aurora_plt_read_rle_00710c10(void*, void*) noexcept(false);
tpc_lower::Word __fastcall aurora_plt_attrib_00710b90(void*, void*, tpc_lower::Word*, tpc_lower::Word*, tpc_lower::Word*);
void* __fastcall aurora_dds_resource_ctor_00710ea0(void*, void*);
void __fastcall aurora_dds_resource_destroy_00710ed0(void*, void*) noexcept;
void* __fastcall aurora_dds_resource_deleting_destroy_00710f70(void*, void*, tpc_lower::Word) noexcept;
tpc_lower::Word __fastcall aurora_dds_resource_freed_00711010(void*, void*);
tpc_lower::Word __fastcall aurora_dds_resource_serviced_00710f30(void*, void*);
tpc_lower::Word __fastcall aurora_dds_attrib_00710ee0(void*, void*, tpc_lower::Word*, tpc_lower::Word*, unsigned char*,
                                                      tpc_lower::Word*, float*);
void* __fastcall aurora_4pc_resource_ctor_00710f90(void*, void*);
void __fastcall aurora_4pc_resource_destroy_00710fc0(void*, void*) noexcept;
void* __fastcall aurora_4pc_resource_deleting_destroy_00711070(void*, void*, tpc_lower::Word) noexcept;
tpc_lower::Word __fastcall aurora_4pc_resource_serviced_00711030(void*, void*);
tpc_lower::Word __fastcall aurora_4pc_attrib_00710fd0(void*, void*, tpc_lower::Word*, tpc_lower::Word*, float*,
                                                      tpc_lower::Signed*);
void* __fastcall aurora_tga_resource_ctor_007129a0(void*, void*);
void __fastcall aurora_tga_resource_destroy_007129e0(void*, void*) noexcept;
void* __fastcall aurora_tga_resource_deleting_destroy_00712c90(void*, void*, tpc_lower::Word) noexcept;
tpc_lower::Word __fastcall aurora_tga_resource_freed_00712a30(void*, void*);
tpc_lower::Word __fastcall aurora_tga_resource_serviced_00712cb0(void*, void*) noexcept(false);
tpc_lower::Word __fastcall aurora_tga_read_color_mapped_rle_00712a70(void*, void*) noexcept(false);
tpc_lower::Word __fastcall aurora_tga_read_unmapped_rle_00712b00(void*, void*) noexcept(false);
tpc_lower::Word __fastcall aurora_tga_attrib_007129f0(void*, void*, tpc_lower::Word*, tpc_lower::Word*, unsigned char*);

// ---------------------------------------------------------------- helpers (aurora_helpers.cpp)
// MDL
font_callback_read::Model* __fastcall aurora_model_construct_00710300(font_callback_read::Model*, void*,
    menu_resources::Ref, font_callback_read::Signed) noexcept(false);
void __fastcall aurora_model_set_ref_00710180(font_callback_read::Model*, void*, menu_resources::Ref*,
    font_callback_read::Signed) noexcept(false);
void __fastcall aurora_model_base_destroy_00710120(font_callback_read::Model*, void*) noexcept;
void __fastcall aurora_model_destroy_00710390(font_callback_read::Model*, void*) noexcept;
void* __fastcall aurora_model_base_deleting_destroy_007102e0(void*, void*, tpc_lower::Word) noexcept;
void* __fastcall aurora_model_deleting_destroy_007103a0(void*, void*, tpc_lower::Word) noexcept;
font_callback_read::Signed __fastcall aurora_model_load_007100d0(font_callback_read::Model*, void*) noexcept(false);
font_callback_read::Model* __cdecl aurora_model_helper_0070de20(font_callback_read::String*, font_callback_read::Signed,
    void*, void*) noexcept(false);
// MDX
font_callback_read::ModelX* __cdecl aurora_modelx_target_0070e7c0(font_callback_read::String*) noexcept(false);
font_callback_read::ModelX* __fastcall aurora_modelx_construct_0070ffe0(font_callback_read::ModelX*, void*,
    menu_resources::Ref, font_callback_read::Signed) noexcept(false);
void __fastcall aurora_modelx_set_ref_0070fe60(font_callback_read::ModelX*, void*, menu_resources::Ref*,
    font_callback_read::Signed) noexcept(false);
void __fastcall aurora_modelx_base_destroy_0070fe00(font_callback_read::ModelX*, void*) noexcept;
void __fastcall aurora_modelx_destroy_00710070(font_callback_read::ModelX*, void*) noexcept;
void* __fastcall aurora_modelx_base_deleting_destroy_0070ffc0(void*, void*, tpc_lower::Word) noexcept;
void* __fastcall aurora_modelx_deleting_destroy_00710080(void*, void*, tpc_lower::Word) noexcept;
font_callback_read::Signed __fastcall aurora_modelx_load_0070fdd0(font_callback_read::ModelX*, void*) noexcept(false);
// VIS
font_callback_read::Visibility* __cdecl aurora_visibility_target_0070e6d0(font_callback_read::String*) noexcept(false);
font_callback_read::Visibility* __fastcall aurora_visibility_construct_0070f270(font_callback_read::Visibility*, void*,
    menu_resources::Ref, font_callback_read::Signed) noexcept(false);
void __fastcall aurora_visibility_set_ref_0070f0f0(font_callback_read::Visibility*, void*, menu_resources::Ref*,
    font_callback_read::Signed) noexcept(false);
void __fastcall aurora_visibility_base_destroy_0070f090(font_callback_read::Visibility*, void*) noexcept;
void __fastcall aurora_visibility_destroy_0070f300(font_callback_read::Visibility*, void*) noexcept;
void* __fastcall aurora_visibility_base_deleting_destroy_0070f250(void*, void*, tpc_lower::Word) noexcept;
void* __fastcall aurora_visibility_deleting_destroy_0070f310(void*, void*, tpc_lower::Word) noexcept;
font_callback_read::Signed __fastcall aurora_visibility_load_0070f060(font_callback_read::Visibility*, void*) noexcept(false);
// TGA
font_callback_read::Texture* __cdecl aurora_texture_target_0070e130(font_callback_read::String*) noexcept(false);
font_callback_read::Texture* __fastcall aurora_texture_construct_0070efa0(font_callback_read::Texture*, void*,
    menu_resources::Ref, font_callback_read::Signed) noexcept(false);
void __fastcall aurora_texture_set_ref_0070ee30(font_callback_read::Texture*, void*, menu_resources::Ref*,
    font_callback_read::Signed) noexcept(false);
void __fastcall aurora_texture_base_destroy_0070edd0(font_callback_read::Texture*, void*) noexcept;
void __fastcall aurora_texture_destroy_0070f030(font_callback_read::Texture*, void*) noexcept;
void* __fastcall aurora_texture_base_deleting_destroy_0070ef80(void*, void*, tpc_lower::Word) noexcept;
void* __fastcall aurora_texture_deleting_destroy_0070f040(void*, void*, tpc_lower::Word) noexcept;
font_callback_read::Signed __fastcall aurora_texture_load_0070ebf0(font_callback_read::Texture*, void*) noexcept(false);
void __fastcall aurora_texture_unload_0070ec70(font_callback_read::Texture*, void*) noexcept(false);
font_callback_read::Signed __fastcall aurora_texture_read_header_0070ece0(font_callback_read::Texture*, void*,
    font_callback_read::Signed*, font_callback_read::Signed*, font_callback_read::Signed*) noexcept(false);
void* __fastcall aurora_texture_info_0070ed40(font_callback_read::Texture*, void*);
font_callback_read::Signed __fastcall aurora_texture_attrib_0070ebb0(font_callback_read::Texture*, void*,
    tpc_lower::Word*, tpc_lower::Word*, unsigned char*);
// PLT
font_callback_read::LayeredTexture* __cdecl aurora_layered_target_0070e310(font_callback_read::String*) noexcept(false);
font_callback_read::LayeredTexture* __fastcall aurora_layered_construct_0070dd60(font_callback_read::LayeredTexture*,
    void*, menu_resources::Ref, font_callback_read::Signed) noexcept(false);
void __fastcall aurora_layered_set_ref_0070dbf0(font_callback_read::LayeredTexture*, void*, menu_resources::Ref*,
    font_callback_read::Signed) noexcept(false);
void __fastcall aurora_layered_base_destroy_0070db90(font_callback_read::LayeredTexture*, void*) noexcept;
void __fastcall aurora_layered_destroy_0070ddf0(font_callback_read::LayeredTexture*, void*) noexcept;
void* __fastcall aurora_layered_base_deleting_destroy_0070dd40(void*, void*, tpc_lower::Word) noexcept;
void* __fastcall aurora_layered_deleting_destroy_0070de00(void*, void*, tpc_lower::Word) noexcept;
font_callback_read::Signed __fastcall aurora_layered_load_0070daa0(font_callback_read::LayeredTexture*, void*) noexcept(false);
void* __fastcall aurora_layered_info_0070db10(font_callback_read::LayeredTexture*, void*);
font_callback_read::Signed __fastcall aurora_layered_attrib_0070da60(font_callback_read::LayeredTexture*, void*,
    tpc_lower::Word*, tpc_lower::Word*, tpc_lower::Word*);
// DDS
font_callback_read::CompressedTexture* __cdecl aurora_compressed_target_0070e400(font_callback_read::String*) noexcept(false);
font_callback_read::CompressedTexture* __fastcall aurora_compressed_construct_007106b0(font_callback_read::CompressedTexture*,
    void*, menu_resources::Ref, font_callback_read::Signed) noexcept(false);
void __fastcall aurora_compressed_set_ref_00710530(font_callback_read::CompressedTexture*, void*, menu_resources::Ref*,
    font_callback_read::Signed) noexcept(false);
void __fastcall aurora_compressed_base_destroy_007104d0(font_callback_read::CompressedTexture*, void*) noexcept;
void __fastcall aurora_compressed_destroy_00710740(font_callback_read::CompressedTexture*, void*) noexcept;
void* __fastcall aurora_compressed_base_deleting_destroy_00710690(void*, void*, tpc_lower::Word) noexcept;
void* __fastcall aurora_compressed_deleting_destroy_00710750(void*, void*, tpc_lower::Word) noexcept;
font_callback_read::Signed __fastcall aurora_compressed_load_00710400(font_callback_read::CompressedTexture*, void*) noexcept(false);
font_callback_read::Signed __fastcall aurora_compressed_read_header_00710430(font_callback_read::CompressedTexture*, void*,
    font_callback_read::Signed*, font_callback_read::Signed*, font_callback_read::Signed*, font_callback_read::Signed*,
    float*) noexcept(false);
void* __fastcall aurora_compressed_info_00710490(font_callback_read::CompressedTexture*, void*);
font_callback_read::Signed __fastcall aurora_compressed_attrib_007103c0(font_callback_read::CompressedTexture*, void*,
    tpc_lower::Word*, tpc_lower::Word*, unsigned char*, tpc_lower::Word*, float*);
// 4PC
font_callback_read::Texture16* __cdecl aurora_texture16_target_0070e220(font_callback_read::String*) noexcept(false);
font_callback_read::Texture16* __fastcall aurora_texture16_construct_00710a90(font_callback_read::Texture16*, void*,
    menu_resources::Ref, font_callback_read::Signed) noexcept(false);
void __fastcall aurora_texture16_set_ref_00710910(font_callback_read::Texture16*, void*, menu_resources::Ref*,
    font_callback_read::Signed) noexcept(false);
void __fastcall aurora_texture16_base_destroy_007108b0(font_callback_read::Texture16*, void*) noexcept;
void __fastcall aurora_texture16_destroy_00710b20(font_callback_read::Texture16*, void*) noexcept;
void* __fastcall aurora_texture16_base_deleting_destroy_00710a70(void*, void*, tpc_lower::Word) noexcept;
void* __fastcall aurora_texture16_deleting_destroy_00710b30(void*, void*, tpc_lower::Word) noexcept;
font_callback_read::Signed __fastcall aurora_texture16_load_007107e0(font_callback_read::Texture16*, void*) noexcept(false);
font_callback_read::Signed __fastcall aurora_texture16_read_header_00710810(font_callback_read::Texture16*, void*,
    font_callback_read::Signed*, font_callback_read::Signed*, float*, font_callback_read::Signed*) noexcept(false);
void* __fastcall aurora_texture16_info_00710870(font_callback_read::Texture16*, void*);
font_callback_read::Signed __fastcall aurora_texture16_attrib_00710770(font_callback_read::Texture16*, void*,
    tpc_lower::Word*, tpc_lower::Word*, float*, font_callback_read::Signed*);
// Processed (TPC) header reader (instance member of the lane-tpc helper type)
font_callback_read::Signed __fastcall aurora_processed_read_header_0070f590(font_callback_read::ProcessedTexture*, void*,
    font_callback_read::Signed*, font_callback_read::Signed*, char*, font_callback_read::Signed*, font_callback_read::Signed*,
    float*, char*) noexcept(false);

// ---------------------------------------------------------------- resman raw read (resman_raw.cpp)
tpc_lower::Word __fastcall resman_res_read_raw_00409b20(void* resource, void*, font_callback_read::Signed bytes, char* out) noexcept(false);

// ---------------------------------------------------------------- existing M46 providers used here
// tpc-txi: CResTXI OnResourceFreed (also slot +0C of the MDL/VIS/MDX tables) and generic OnResourceServiced.
tpc_lower::Word __fastcall aurora_txi_resource_freed_00710e60(void*, void*);
tpc_lower::Word __fastcall aurora_generic_resource_serviced_005d4c60(void*, void*);
}
