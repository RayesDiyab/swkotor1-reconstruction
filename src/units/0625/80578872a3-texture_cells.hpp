#pragma once
// texture_cells.hpp - lane-texture-epoch PROPOSAL (epoch E1): storage owner declarations for the texture-system
// globals and the Aurora raster callback cells installed by CAuroraInterface ctor 0070D9C0.
// Every object below is one original .data cell (name = original VA; cold value = pinned PE image, BSS = 0).
// Types follow the cell's readers/writers; Ghidra names in comments are labels only.
#include "5fcd820543-texture.hpp"
#include "56e9a90acd-scheduler.h"

namespace aurora_raster {
using Signed=acquisition::Signed; using Word=acquisition::Word;
// Cell types not already named in acquisition:: (Ghidra TYPEINFO of the installed targets; "undefined4" returns
// are carried as Word = full EAX, inferred).
using GetResource          =char* (__cdecl*)(void*,const char*,Signed*);                                       // 7B8FC4 -> 0070C920
using ReleaseResource      =void  (__cdecl*)(void*,Signed);                                                    // 7B8FC8 -> 0070CBD0
using ReadRasterHeader     =Word  (__cdecl*)(const char*,Signed*,Signed*,Signed*);                             // 78D408 -> 0070CC60
using GetLayeredRaster     =char* (__cdecl*)(const char*,Signed*,Signed*,Signed*);                             // 78D41C -> 0070C6E0
using ReleaseLayeredRaster =texture_scheduler::ReleaseComponents;                                              // 7A46FC -> 0070C850
using CompressedHeader     =Word  (__cdecl*)(const char*,Signed*,Signed*,Signed*,Signed*,float*);              // 78D410 -> 0070CDD0
using Raster16Header       =Word  (__cdecl*)(const char*,Signed*,Signed*,float*,Signed*);                      // 78D418 -> 0070CE80
using ProcessedHeader      =Word  (__cdecl*)(const char*,Signed*,Signed*,char*,Signed*,Signed*,float*,char*);  // 78D424 -> 0070CD10
}

extern "C" {
// ---- installer-written cells. 007B8FC4/007B8FC8 are owned by lane-aurres (aurres.hpp, identical declarations,
// D19-style single owner); the other eleven are owned here (texture_cells.cpp), typed C++ data with original cold
// values. Only writer of all thirteen in the original: 0070D9C0 (0070D9C7..0070DA3F; full-image scan).
extern void* (__cdecl* volatile aurres_get_callback_007b8fc4)(const char*,const char*,std::int32_t*);   // lane-aurres
extern void (__cdecl* volatile aurres_release_callback_007b8fc8)(void*,std::int32_t);                   // lane-aurres
extern acquisition::Raster volatile                      aurora_cell_0078d404; // cold 0045E430 ImageReadTGA
extern aurora_raster::ReadRasterHeader volatile          aurora_cell_0078d408; // cold 0045E440 ImageReadHeader
extern acquisition::ReleaseRaster volatile               aurora_cell_007a46f8; // BSS 0
extern aurora_raster::GetLayeredRaster volatile          aurora_cell_0078d41c; // cold 0045E510 ImageReadPLT
extern aurora_raster::ReleaseLayeredRaster volatile      aurora_cell_007a46fc; // BSS 0
extern acquisition::CompressedRaster volatile            aurora_cell_0078d40c; // cold 0045F4A0 ImageReadS3TC
extern aurora_raster::CompressedHeader volatile          aurora_cell_0078d410; // cold 0045E180 ImageReadS3TCHeader
extern acquisition::Raster16 volatile                    aurora_cell_0078d414; // cold 0045DF10 ImageRead16bit
extern aurora_raster::Raster16Header volatile            aurora_cell_0078d418; // cold 0045E010 ImageRead16bitHeader
extern acquisition::ProcessedRaster volatile             aurora_cell_0078d420; // cold 0063E7F0 return_zero: see PACKET minimal case
extern aurora_raster::ProcessedHeader volatile           aurora_cell_0078d424; // cold 0063E7F0 return_zero: see PACKET minimal case

// ---- D24: original cold value of 0078D420/0078D424 is 0063E7F0 return_zero = `33 C0 / C3` (xor eax,eax; ret):
// reads no argument, cleans no stack, returns EAX=0 - valid for any cdecl signature. Implemented once per cell type.
char* __cdecl return_zero_processed_raster_0063e7f0(const char*,acquisition::Signed*,acquisition::Signed*,char*,acquisition::Signed*,
                                                    acquisition::Signed*,acquisition::Signed*,float*,char*);
acquisition::Word __cdecl return_zero_processed_header_0063e7f0(const char*,acquisition::Signed*,acquisition::Signed*,char*,
                                                    acquisition::Signed*,acquisition::Signed*,float*,char*);
// ---- texture Providers members authored by lane-font / task-33 (identical declarations) ----
// 00420710 CAurTextureBasic::GetMemoryUsage: authored by lane-startup-misc (texture_memory.cpp:74); shared C declaration.
void __fastcall texture_memory_usage_00420710(texture::Basic*,void*,texture::U*);
texture::U __cdecl texture_hash_0041f4d0(const char*,texture::I,const texture::U*);
texture::U __fastcall texture_add_ordered_00421560(texture::Basic*,void*);
texture::U __fastcall texture_parse_field_00422390(texture::Basic*,void*,const char*);
texture::U __fastcall font_info_destroy_00420610(texture::FontInfo*,void*);
// ---- cold-value defaults (no authored body; declared with their cell's type) ----
char* __cdecl image_read_tga_0045e430(const char*,acquisition::Word*,acquisition::Word*,acquisition::Word*);
acquisition::Word __cdecl image_read_header_0045e440(const char*,acquisition::Signed*,acquisition::Signed*,acquisition::Signed*);
char* __cdecl image_read_plt_0045e510(const char*,acquisition::Signed*,acquisition::Signed*,acquisition::Signed*);
char* __cdecl image_read_s3tc_0045f4a0(const char*,acquisition::Signed*,acquisition::Signed*,acquisition::Signed*,acquisition::Signed*,float*);
acquisition::Word __cdecl image_read_s3tc_header_0045e180(const char*,acquisition::Signed*,acquisition::Signed*,acquisition::Signed*,
                                                          acquisition::Signed*,float*);
char* __cdecl image_read_16bit_0045df10(const char*,acquisition::Signed*,acquisition::Signed*,float*,acquisition::Signed*);
acquisition::Word __cdecl image_read_16bit_header_0045e010(const char*,acquisition::Signed*,acquisition::Signed*,float*,acquisition::Signed*);

// ---- the thirteen installed targets (0070D9C0 stores their addresses) ----
// 0070C920/0070CBD0 typed to lane-aurres cell types (lane-tpc authors the bodies; the older 2026-10-04 donor
// read_0070c920 has a different C type char*(void*,const char*,Signed*) and is not used here).
void* __cdecl aurora_get_resource_0070c920(const char*,const char*,std::int32_t*) noexcept(false); // E-D4
void  __cdecl aurora_release_resource_0070cbd0(void*,std::int32_t);
char* __cdecl aurora_get_raster_0070c790(const char*,acquisition::Word*,acquisition::Word*,acquisition::Word*);
acquisition::Word __cdecl aurora_read_raster_header_0070cc60(const char*,acquisition::Signed*,acquisition::Signed*,acquisition::Signed*);
void  __cdecl aurora_release_raster_0070c870(char*,acquisition::Signed,acquisition::Signed,acquisition::Signed,acquisition::Word,
                                             acquisition::Signed,acquisition::Signed,acquisition::Word);
char* __cdecl aurora_get_layered_raster_0070c6e0(const char*,acquisition::Signed*,acquisition::Signed*,acquisition::Signed*);
void  __cdecl aurora_release_layered_raster_0070c850(void*,texture::I,texture::I);
char* __cdecl aurora_get_compressed_raster_0070d380(const char*,acquisition::Signed*,acquisition::Signed*,acquisition::Signed*,
                                                    acquisition::Signed*,float*);
acquisition::Word __cdecl aurora_read_compressed_raster_header_0070cdd0(const char*,acquisition::Signed*,acquisition::Signed*,
                                                    acquisition::Signed*,acquisition::Signed*,float*);
char* __cdecl aurora_get_16bit_raster_0070d6b0(const char*,acquisition::Signed*,acquisition::Signed*,float*,acquisition::Signed*);
acquisition::Word __cdecl aurora_get_16bit_raster_header_0070ce80(const char*,acquisition::Signed*,acquisition::Signed*,float*,
                                                    acquisition::Signed*);
char* __cdecl processed_raster_0070d510(const char*,acquisition::Signed*,acquisition::Signed*,char*,acquisition::Signed*,
                                        acquisition::Signed*,acquisition::Signed*,float*,char*) noexcept(false); // task-4 processed-raster-r1 (E-D4)
acquisition::Word __cdecl aurora_read_processed_raster_header_0070cd10(const char*,acquisition::Signed*,acquisition::Signed*,char*,
                                                    acquisition::Signed*,acquisition::Signed*,float*,char*);
// 0063E7F0 return_zero: existing M40/M41 definition (constants.obj, task-6 c2da-resource-callees-r1).
acquisition::Word __stdcall c2da_resource_constant_word_0063e7f0();
}

namespace texture_cells {
using texture::U; using texture::I; using texture::List;
// CExoArrayList<CAurTextureBasic*> / <int> (BSS; static ctors only zero them).
extern List aur_refresh_textures_007a4770;   // pending list; also texture_join::canonical_environment_007a4770
extern List aur_dynamic_texture_list_007a4780;
extern List texture_indices_007a478c;
extern List ordered_texture_list_007a4798;
extern List aur_static_texture_list_007a47a4;
extern U volatile no_compression_007a4700;   // [007A4700] read 0041EA20 (writer not in this cohort)
extern U volatile texture_count_007a4718;    // object count: ctor 00423150, dtor 00423055..0042305D
extern U volatile accounting_007a46d8, accounting_007a46e4, accounting_007a46e8, accounting_007a46ec, accounting_007a46f0;
extern U volatile compression_cache_0078e53c;           // cold 0xFFFFFFFF (0045F950 cache)
extern font::Safe null_texture_007a477c;                // CAurTexture* nullTexture (BSS); texture_join::canonical_null_007a477c
// Scheduler cells (external linkage: lane-startup-misc SetMem 00421D90 writes 0078D3F4).
extern unsigned char volatile is_suspended_007a46d5;
extern I volatile max_tex_memory_0078d3f4;
extern U volatile construct_image_time_007a4724,max_decode_image_time_007a4730,max_gl_image_time_007a4728,
                  max_texture_load_time_007a472c,max_texture_time_007a46b0;
extern U volatile current_texture_time_007a4754,load_image_time_007a4758,decode_image_time_007a475c,gl_image_time_007a4760;
extern float volatile delta_t_0078e574;
extern float volatile max_delta_t_007a4750;
extern U volatile texture_current_frame_007a46f4,aur_basic_texture_count_007a4748,max_tex_id_007a46bc;
extern const unsigned char* palette_image_007a4704;
extern I volatile palette_height_007a4708;
extern unsigned char volatile fill_alpha_007a46d4;
extern float fill_vector_007a4764[3];
extern const char norm_cube_map_0073ee34[12];
}
// D19: capability masks owned by lane-renderer-bodies (renderer_gl_procs.h:427-428, cold 0x4000/0x8000).
namespace kotor_renderer_gl_procs { extern std::uint32_t mask_0078e4b8; extern std::uint32_t mask_0078e4bc; }
namespace kotor_renderer_gl_cells { extern std::uint32_t capabilities_007bb85c; }       // lane-renderer-bodies owner
namespace menu_tables_m41 { namespace cells { extern std::uint32_t dynamic_memory_ratio_0078d3f0; } } // lane-menu-tables owner
