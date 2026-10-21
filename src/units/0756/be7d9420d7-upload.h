#pragma once
// lane-upload: texture upload/bind chain (swkotor.exe GOG K1 9c10e045..1435, image base 00400000).
// Basic layout: lane-texture-epoch E1 texture.hpp (single texture::Basic epoch).
#include "a303632628-texture.hpp"             // lane-texture-epoch E1: Basic, basic_gl_image_00420970 decl, slot carriers
#include "87447400ff-texture_vtable.hpp"      // lane-texture-epoch typed vtables 0073F160/0073F0A8 (U-D1)
#include "fce1f86b35-renderer_initialize.h"   // M41 canonical renderer interface (glEnable/glDisable IAT adapters)
#include "4ba38b6813-renderer_gl_imports.h"     // single GL IAT adapter owner (lane-renderer-bodies): glGenTextures/glBindTexture/glTexParameteri
#include "2c36ec5a18-renderer_gl_imports_gen.h" // same owner, generated: glFinish/glTexImage2D/glTexSubImage2D/gluBuild2DMipmaps
#include "9d6729921f-live_extensions.h"       // LiveExtensions descriptor (D11 delta: 007BB774)
#include "8764550a9d-peer-system.h"           // renderer_system::bindings(): glCurrentTID 007A683C[6], uiCurrentStage 007A6898

namespace texture_upload {
using U=texture::U;
using I=texture::I;
using U8=std::uint8_t;
using Basic=texture::Basic;

// Borrowed view over the original globals this chain reads/writes. Defined once in upload_cells.cpp.
struct Cells {
 const U volatile* gl_texture_cube_map_0078d478;  // .data 00008513 (no writer in image)
 const U volatile* gl_texture_2d_0078d458;        // lane-gui-draw kotor_renderer_cells::enable_0078d458 (0DE1)
 const I volatile* anisotropy_007a685c;           // BSS; writer 0044F2DE
 U volatile* texture_binds_count_007a4710;        // BSS; writers 00420572/004205AA/0046EE02
 const I volatile* bind_gate_007a470c;            // BSS; no writer in image; `jl` at 00420532 (signed)
 const U volatile* bind_zero_007a4714;            // BSS; no writer in image (CAurTexture::bindZero)
 const font::Safe* null_texture_007a477c;         // lane-texture-epoch texture_cells::null_texture_007a477c (owner object)
 U volatile* current_tid_007a47b8;                // BSS; writer 004214FF (CAurTexture::curretnTID)
 const U volatile* enable_texture_controllers_0078d430; // .data 00000001 (no writer in image)
 const U* gl_format_table_0073f36c;               // .rdata U[24] 0073F36C..0073F3C8
 const U* cube_face_target_0078db50;              // .data U[6] {1,0,2,3,4,5} (+8515h)
 const U* cube_face_rotation_0078db38;            // .data U[6] {3,1,0,2,2,0}
};
extern const Cells& cells;

// U-D1: Basic/Reference slots are called through lane-texture-epoch's typed tables
// (texture_vtable::CommonSlots, decltype(&provider) members); no untyped slot reinterpretation.
}

namespace kotor_renderer_init {
// Renderer capability leaves (cache -1 cold sentinel; masks vs 007BB85C). Authored in capabilities_0045f.cpp.
bool __cdecl ATIValidate_0045f640();               // AL (sete al, 0045F676)
U32 __cdecl CubeMapAvailable_0045f8b0();           // full EAX = cache 0078E530
U32 __cdecl SGIMipmapAvailable_0045f910();         // full EAX = cache 0078E538
U32 __cdecl BumpMapAvailable_0045fb10();           // full EAX 0/1
}

extern "C" {
// ---- U-D1 naked carriers (upload_carriers.cpp): reload [ecx] and tail-jump the slot; ECX receiver, zero stack.
// +04 differs per table (Basic 00641DB0 fastcall Ref*; Reference 0063E7F0 stdcall no-arg), so it has no single
// C++ slot type: the carrier forwards the original `call [eax+4]` (00421504-style) and returns full EAX.
font::Texture* __fastcall upload_call_query_04(font::Texture*);
// Controller object at Basic+34 (no published typed table): `call [[+34]+4]` 0042153E, `call [[+34]+10h]` 00420A70.
void __fastcall upload_controller_call_04(texture_upload::U);
void __fastcall upload_controller_call_10(texture_upload::U);
// ---- authored (upload.cpp). 00420970 is declared once in texture.hpp (texture::basic_gl_image_00420970).
void __cdecl aur_texture_bind_tid_004204f0(texture_upload::U,texture_upload::U8);     // AurTextureBindTID
texture_upload::U __cdecl aur_texture_get_tid_00420bf0(void*);                         // AurTextureGetTID
void __fastcall texture_bind_004214b0(void*,void*);                                    // CAurTexture::Bind, slot+B4
void __cdecl gl_create_texture_00424980(texture_upload::Basic*,texture_upload::U,texture_upload::I);
void __cdecl gl_create_processed_texture_00424dd0(texture_upload::Basic*,texture_upload::U,texture_upload::I);
void __cdecl gl_create_cube_texture_00423f90(texture_upload::Basic*,texture_upload::U,texture_upload::I);
void __cdecl gl_create_processed_cube_texture_00424230(texture_upload::Basic*,texture_upload::U,texture_upload::I);
texture_upload::I __cdecl get_compressed_contact_sheet_offset_00424d70(texture_upload::I,texture_upload::I,texture_upload::I);
void* __cdecl image_get_sub_image_0045f000(const void*,texture_upload::I,texture_upload::I,texture_upload::I,
                                           texture_upload::I,texture_upload::I,texture_upload::I,texture_upload::I);
texture_upload::U __cdecl build_texture_normal_map_00420ea0(void*,float,texture_upload::I*,texture_upload::I*,
                                                            texture_upload::I,texture_upload::I,texture_upload::U);
void __cdecl aur_build_2d_mip_maps_00424580(texture_upload::I,texture_upload::I,texture_upload::I,const void*,texture_upload::U);

// CAurTexture getters (Basic vtable 0073F160). ECX this, EDX ignored, zero stack, RET0.
texture_upload::U __fastcall texture_is_bump_map_0041eef0(void*,void*);       // slot+1C (full EAX 0/1)
texture_upload::U __fastcall texture_is_normal_map_0041ef50(void*,void*);     // slot+20 (full EAX 0/1)
void* __fastcall texture_get_image_0041efc0(void*,void*);                     // slot+3C
texture_upload::I __fastcall texture_get_width_0041f020(void*,void*);         // slot+48
texture_upload::I __fastcall texture_get_height_0041f040(void*,void*);        // slot+4C
texture_upload::I __fastcall texture_get_bpp_0041f060(void*,void*);           // slot+50
texture_upload::I __fastcall texture_get_num_mip_maps_0041f080(void*,void*);  // slot+54
texture_upload::I __fastcall texture_get_num_x_0041f0a0(void*,void*);         // slot+58
texture_upload::I __fastcall texture_get_num_y_0041f0c0(void*,void*);         // slot+5C
texture_upload::I __fastcall texture_get_clamp_0041f140(void*,void*);         // slot+60
texture_upload::I __fastcall texture_get_compressed_size_0041f1a0(void*,void*); // slot+64
texture_upload::U8 __fastcall texture_get_mip_map_0041f0e0(void*,void*);      // slot+6C
texture_upload::U8 __fastcall texture_get_filter_0041f100(void*,void*);       // slot+70
float __fastcall texture_get_bump_map_scaling_0041f160(void*,void*);          // slot+78
texture_upload::I __fastcall texture_get_compression_format_0041f1c0(void*,void*); // slot+8C

// ---- lowers outside this packet (undefined externals)
void* __cdecl image_scale_0045dad0(const void*,texture_upload::I,texture_upload::I,texture_upload::I,texture_upload::I,texture_upload::I);
void* __cdecl image_quarter_rotated_0041fbd0(const void*,texture_upload::I,texture_upload::I,texture_upload::I,texture_upload::U);
texture_upload::U __cdecl convert_height_field_and_load_normal_map_004260a0(void*,texture_upload::I,texture_upload::I,
                     texture_upload::I,texture_upload::I,float,texture_upload::I,texture_upload::U);
}
