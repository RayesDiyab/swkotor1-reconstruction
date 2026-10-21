// lane-m47-core rev2 ADAPTED COPY of the M46 stage unit texture-epoch-texture_cells (texture-epoch/texture/texture_cells.cpp,
// base sha256 d61134bc11a87ce8690f209ac85af8521c09ba03ce1708280c2e7625d38fbf53).
// D76 FIX (FIRST FRAME: GUI font textures run 00423150 / 00422AF0 / 00420AC0): the 8 null members of texture::providers
// are bound statically to the genuine M46 providers, each with the exact slot type (no casts):
//   compare        00420AED/00420B8E call 0070ACAF -> kotor_stricmp_0070acaf (string-compare-existing)
//   read           00422B72 call 0044C740 -> aurres_get_0044c740 (aurres-stream)
//   bytes          00422BCA call 0044BD10 -> aurres_data_bytes_0044bd10 (aurres-stream)
//   next_line      00422BDB/00422BFB call 0044BFA0 -> aurres_next_line_const_0044bfa0 (aurres-stream typed view)
//   resource_release 00422C0A call 0044C8F0 -> aurres_release_0044c8f0 (aurres-stream)
//   cube_available 00422CB2 call 0045F8B0 -> kotor_renderer_init::CubeMapAvailable_0045f8b0 (renderer-capability-leaves-0045f)
//   find_cache     00420ACB/00420AE0 [007A479C]/[007A4798] -> texture_cells::ordered_texture_list_007a4798 (this unit)
//   default_flag   00423268 mov al,[0078D400] -> aur_texture_mode_dynamic_0078d400 (core46-gl-fb)
// texture_cells.cpp - lane-texture-epoch PROPOSAL (epoch E1): genuine storage for the texture-system globals and
// the single definitions of the borrowed views/bindings consumed by acquisition (0041F6A0/0041EA20), the
// scheduler (00421EE0/004217F0/004216A0/0041FA30), FreeImage/RemoveOrdered (0041F570/00421650) and the
// destructor (00422F40). Cold values: pinned GOG swkotor.exe (.data raw bytes; BSS = 0), read by
// tools/read_va.py. All initializers are constant (no dynamic initialization). Integrator may relocate.
#include "708974864c-texture_cells.hpp"
#include "1ad9984726-resource.hpp"
#include "49236907d8-texture_join.h"
#include "283ae0c931-texture_vtable.hpp"
#include "e1a7e44581-font_info.hpp"
// glDeleteTextures goes through the single GL IAT adapter owner (lane-renderer-bodies windows_gl_imports.cpp:70).
namespace kotor_renderer_init { void __stdcall glDeleteTextures_0073d248(std::int32_t,const std::uint32_t*); }

namespace texture_cells {
List aur_refresh_textures_007a4770{};       // CExoArrayList<CAurTextureBasic*> aurRefreshTextures (BSS)
List aur_dynamic_texture_list_007a4780{};   // aurDyanmicTextureList (BSS)
List texture_indices_007a478c{};            // CExoArrayList<int> textureIndices (BSS)
List ordered_texture_list_007a4798{};       // orderdTextureList (BSS)
List aur_static_texture_list_007a47a4{};    // aurStaticTextureList (BSS)
U volatile no_compression_007a4700=0;       // noCompression (BSS)
U volatile texture_count_007a4718=0;        // count (BSS)
U volatile accounting_007a46d8=0,accounting_007a46e4=0,accounting_007a46e8=0,accounting_007a46ec=0,accounting_007a46f0=0; // BSS
U volatile compression_cache_0078e53c=0xffffffffu;               // aurTextureCompressionAvailable (.data 0xFFFFFFFF)
font::Safe null_texture_007a477c{};          // CAurTexture* nullTexture (BSS); read 00420C00/004214C1
// Scheduler cells (texture-system owned; external linkage - lane-startup-misc SetMem 00421D90 writes 0078D3F4).
unsigned char volatile is_suspended_007a46d5=0;  // isSuspended (BSS)
I volatile max_tex_memory_0078d3f4=0x01000000;   // maxTexMemory (.data 0x01000000); writer SetMem 00421DA6
U volatile construct_image_time_007a4724=0,max_decode_image_time_007a4730=0,max_gl_image_time_007a4728=0,
           max_texture_load_time_007a472c=0,max_texture_time_007a46b0=0;                       // BSS
U volatile current_texture_time_007a4754=0,load_image_time_007a4758=0,decode_image_time_007a475c=0,
           gl_image_time_007a4760=0;                                                             // BSS
float volatile delta_t_0078e574=0.1f;            // deltaT (.data 0x3DCCCCCD) - frame-loop cell; see PACKET open gate
float volatile max_delta_t_007a4750=0.0f;        // maxDeltaT (BSS)
U volatile texture_current_frame_007a46f4=0,aur_basic_texture_count_007a4748=0,max_tex_id_007a46bc=0; // BSS
const unsigned char* palette_image_007a4704=nullptr; // paletteImage (BSS)
I volatile palette_height_007a4708=0;            // paletteHeight (BSS)
unsigned char volatile fill_alpha_007a46d4=0;    // (BSS) 0041FB97
float fill_vector_007a4764[3]={0.0f,0.0f,0.0f};  // (BSS) pushed as 7A4764h at 0041FBA9
extern const char norm_cube_map_0073ee34[12]; // .rdata 12 bytes incl. NUL (REPE CMPS count 0Ch, 004216B7)
}

// ---- the eleven installer-written cells owned here: typed C++ data, original cold values (.data) ----
extern "C" {
acquisition::Raster volatile                 aurora_cell_0078d404=&image_read_tga_0045e430;          // .data 0045E430
aurora_raster::ReadRasterHeader volatile     aurora_cell_0078d408=&image_read_header_0045e440;       // .data 0045E440
acquisition::ReleaseRaster volatile          aurora_cell_007a46f8=nullptr;                           // BSS
aurora_raster::GetLayeredRaster volatile     aurora_cell_0078d41c=&image_read_plt_0045e510;          // .data 0045E510
aurora_raster::ReleaseLayeredRaster volatile aurora_cell_007a46fc=nullptr;                           // BSS
acquisition::CompressedRaster volatile       aurora_cell_0078d40c=&image_read_s3tc_0045f4a0;         // .data 0045F4A0
aurora_raster::CompressedHeader volatile     aurora_cell_0078d410=&image_read_s3tc_header_0045e180;  // .data 0045E180
acquisition::Raster16 volatile               aurora_cell_0078d414=&image_read_16bit_0045df10;        // .data 0045DF10
aurora_raster::Raster16Header volatile       aurora_cell_0078d418=&image_read_16bit_header_0045e010; // .data 0045E010
// D24: cold value 0063E7F0 return_zero (33 C0 C3: xor eax,eax; ret - no argument read, no stack cleanup) expressed
// as a function of each cell's own type (C2440 proof for the direct form: checks/minimal_case_0078d420.cpp).
acquisition::ProcessedRaster volatile        aurora_cell_0078d420=&return_zero_processed_raster_0063e7f0; // .data 0063E7F0
aurora_raster::ProcessedHeader volatile      aurora_cell_0078d424=&return_zero_processed_header_0063e7f0; // .data 0063E7F0
// 0063E7F0 bytes 33 C0 C3: EAX=0, RET0. Parameters intentionally unread.
char* __cdecl return_zero_processed_raster_0063e7f0(const char*,acquisition::Signed*,acquisition::Signed*,char*,acquisition::Signed*,
                                                    acquisition::Signed*,acquisition::Signed*,float*,char*){return nullptr;}
acquisition::Word __cdecl return_zero_processed_header_0063e7f0(const char*,acquisition::Signed*,acquisition::Signed*,char*,
                                                    acquisition::Signed*,acquisition::Signed*,float*,char*){return 0;}
}

// Texture ctor/Init providers (definition moved from texture.cpp, edit T4). Vtables via the D24-accepted
// object-pointer conversion (font::Texture::vtable is void**). rev2: every member is bound (see file header).
// rev2: genuine providers of the formerly-null texture::providers members (declarations identical to their owners').
extern "C" {
std::int32_t __cdecl kotor_stricmp_0070acaf(const char*, const char*);
void* __cdecl aurres_get_0044c740(const char* name,const char* ext,std::int32_t volatile* size,std::uint32_t memory_only);
void* __cdecl aurres_data_bytes_0044bd10(std::uint32_t count,void* descriptor);
const char* __cdecl aurres_next_line_const_0044bfa0();
std::uint32_t __cdecl aurres_release_0044c8f0(void* descriptor,std::int32_t size);
extern volatile std::uint8_t aur_texture_mode_dynamic_0078d400;
}
namespace kotor_renderer_init { std::uint32_t __cdecl CubeMapAvailable_0045f8b0(); }

namespace texture {
Providers providers={
 &kotor_stricmp_0070acaf,                        // compare 0070ACAF (rev2)
 &texture_hash_0041f4d0,                         // hash 0041F4D0 (lane-font)
 &texture_add_ordered_00421560,                  // add_ordered 00421560 (lane-font)
 &font_info_producer::construct_004221c0,        // info_ctor 004221C0
 &font_info_destroy_00420610,                    // info_dtor 00420610 (lane-font)
 &aurres_get_0044c740,&aurres_data_bytes_0044bd10,&aurres_next_line_const_0044bfa0, // read / bytes / next_line (rev2)
 &texture_parse_field_00422390,                  // parse_line 00422390 (lane-font)
 &aurres_release_0044c8f0,&kotor_renderer_init::CubeMapAvailable_0045f8b0, // resource_release / cube_available (rev2)
 &texture_cells::ordered_texture_list_007a4798,  // find_cache 007A4798 (rev2)
 &texture_cells::texture_count_007a4718,         // object_count 007A4718
 &aur_texture_mode_dynamic_0078d400,             // default_flag 0078D400 (rev2)
 reinterpret_cast<void**>(const_cast<texture_vtable::ReferenceVTable*>(&texture_vtable::reference_vtable_0073f0a8)), // 00422E30 -> 0073F0A8
 reinterpret_cast<void**>(const_cast<texture_vtable::BasicVTable*>(&texture_vtable::basic_vtable_0073f160))};       // 00423150 -> 0073F160
}

namespace acquisition {
namespace {
const Cells cell_view={&aurora_cell_0078d420,&aurora_cell_0078d40c,&aurora_cell_0078d414,&aurora_cell_0078d404,
                       &aurora_cell_007a46f8,&texture_cells::no_compression_007a4700};
}
const Cells& cells=cell_view;
}

namespace texture_scheduler {
namespace {
using namespace texture_cells;
const Cells cell_view={
 &aur_refresh_textures_007a4770,&ordered_texture_list_007a4798,&aur_static_texture_list_007a47a4,&texture_indices_007a478c,
 &is_suspended_007a46d5,&max_tex_memory_0078d3f4,&menu_tables_m41::cells::dynamic_memory_ratio_0078d3f0,
 &construct_image_time_007a4724,&max_decode_image_time_007a4730,&max_gl_image_time_007a4728,&max_texture_load_time_007a472c,
 &max_texture_time_007a46b0,&current_texture_time_007a4754,&load_image_time_007a4758,&decode_image_time_007a475c,
 &gl_image_time_007a4760,&delta_t_0078e574,&max_delta_t_007a4750,&texture_current_frame_007a46f4,
 &aur_basic_texture_count_007a4748,&max_tex_id_007a46bc,&palette_image_007a4704,&palette_height_007a4708,
 &aurora_cell_007a46fc,&fill_alpha_007a46d4,fill_vector_007a4764,norm_cube_map_0073ee34};
}
const Cells& cells=cell_view;
}

// FreeImage 0041F570 / RemoveOrdered 00421650 (definition moved here from resource.cpp, edit F7).
namespace texture_resource {
Bindings bindings={&aurora_cell_007a46f8,                         // 0041F662 call [007A46F8]
                   &texture_cells::ordered_texture_list_007a4798, // 00421653/00421679
                   &texture_cells::aur_dynamic_texture_list_007a4780, // 0042168C (D3 != 0)
                   &texture_cells::aur_static_texture_list_007a47a4}; // 00421693 (D3 == 0)
}

// Destructor 00422F40 (definition moved here from destruction.cpp, edit D3).
extern "C" {
 void __fastcall texture_list_remove_00445080(texture::List*,void*,texture::U); // texture_x86.asm -> ArrayMutations::RemoveReverse32
}
namespace texture_destruction {
Bindings bindings={
 // MINIMAL CASE (D21): font::Texture::vtable is `void** volatile` (frozen font.hpp), so publishing the typed
 // struct vtable needs one OBJECT-pointer conversion (no function-pointer cast); readers then index void* slots.
 reinterpret_cast<void**>(const_cast<texture_vtable::BasicVTable*>(&texture_vtable::basic_vtable_0073f160)),                // 00422F61 mov [esi],73F160h
 &font_info_destroy_00420610,                                     // 00422F76
 &texture_memory_usage_00420710,                                  // 00422FAC
 &kotor_renderer_init::glDeleteTextures_0073d248,                 // 00423023 call [0073D248] (IAT adapter)
 &texture_resource::free_image_0041f570,                          // 00423050
 &texture_resource::remove_ordered_00421650,                      // 00423062
 &texture_list_remove_00445080,                                   // 00423095
 &texture_owner::base_destroy_00422e90,                           // 004230A4
 {&texture_cells::accounting_007a46d8,&texture_cells::accounting_007a46e4,&texture_cells::accounting_007a46e8,
  &texture_cells::accounting_007a46ec,&texture_cells::accounting_007a46f0}, // see PACKET evidence: acc[0]=D8,[1]=E4,[2]=E8,[3]=EC,[4]=F0
 &texture_cells::texture_count_007a4718,                          // 00423055..0042305D
 &texture_cells::aur_refresh_textures_007a4770};                  // 00423067..00423095
}

// task-17 texture-cleanup: the same live list object (texture_join.h "Main6 must bind the SAME live nominal cells").
namespace texture_join {
List& canonical_environment_007a4770=texture_cells::aur_refresh_textures_007a4770;
font::Safe& canonical_null_007a477c=texture_cells::null_texture_007a477c;  // D19: lane-texture-epoch owns 007A477C
}
