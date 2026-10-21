#pragma once
// texture_vtable.hpp - lane-texture-epoch PROPOSAL (epoch E1, integrator D1/D21): typed C++ vtables
// CAurTextureBasic 0073F160 and CAurTexture (reference) 0073F0A8, 46 slots each (+00..+B4; PE word +B8 is not a slot).
// Each slot member's type is decltype(&provider): heterogeneous typed members, no function-pointer casts.
// thiscall slots use the D1 fastcall(Self*, void* unused_edx, stack...) shape; decoration @N = 4*(2+stack words),
// stack words from the original `ret N` of each slot body (dumpbin).
// Slot return/parameter types marked (inferred) have no authored body yet; the full-EAX word is carried as U.
#include "9ebac881e9-resource.hpp"     // texture_owner (004233C0, 00641DB0), texture_resource (0041F570)
#include "63b483a8b0-texture_join.h"   // texture_join::referenced_004205c0 (task-17)
#include "a14ee97e1a-texture_cells.hpp" // c2da_resource_constant_word_0063e7f0 (return_zero, M41 constants.obj)

extern "C" {
// ---- providers authored by other lanes (identical declarations; a TU including both checks consistency) ----
texture::U   __fastcall texture_is_bump_map_0041eef0(void*,void*);            // lane-upload
texture::U   __fastcall texture_is_normal_map_0041ef50(void*,void*);          // lane-upload
texture::FontInfo* __fastcall texture_font_info_0041efa0(void*,void*);        // lane-font
void*        __fastcall texture_get_image_0041efc0(void*,void*);              // lane-upload
texture::I   __fastcall texture_get_width_0041f020(void*,void*);              // lane-upload
texture::I   __fastcall texture_get_height_0041f040(void*,void*);             // lane-upload
texture::I   __fastcall texture_get_bpp_0041f060(void*,void*);                // lane-upload
texture::I   __fastcall texture_get_num_mip_maps_0041f080(void*,void*);       // lane-upload
texture::I   __fastcall texture_get_num_x_0041f0a0(void*,void*);              // lane-upload
texture::I   __fastcall texture_get_num_y_0041f0c0(void*,void*);              // lane-upload
texture::I   __fastcall texture_get_clamp_0041f140(void*,void*);              // lane-upload
texture::I   __fastcall texture_get_compressed_size_0041f1a0(void*,void*);    // lane-upload
std::uint8_t __fastcall texture_get_mip_map_0041f0e0(void*,void*);            // lane-upload
std::uint8_t __fastcall texture_get_filter_0041f100(void*,void*);             // lane-upload
float        __fastcall texture_get_bump_map_scaling_0041f160(void*,void*);   // lane-upload
texture::I   __fastcall texture_get_compression_format_0041f1c0(void*,void*); // lane-upload
void         __fastcall texture_bind_004214b0(void*,void*);                   // lane-upload
// ---- slot bodies authored in texture/texture_slots.cpp (types from ret N / AL / ST0 / full EAX) ----
std::uint8_t __fastcall texture_is_cube_map_0041ebf0(void*,void*);             // AL (setne) 0041EC08
std::uint8_t __fastcall texture_is_invalid_004205d0(void*,void*);              // AL [+E1] / setne
texture::U   __fastcall texture_is_diffuse_bump_map_0041ee70(void*,void*);
texture::U   __fastcall texture_is_specular_bump_map_0041eeb0(void*,void*);
std::uint8_t __fastcall texture_is_using_global_alpha_0041ef80(void*,void*);   // AL-only (review E-D1)
std::uint8_t __fastcall texture_is_fully_loaded_0041ec10(void*,void*);        // AL [+E0]
std::uint8_t __fastcall texture_is_environment_mapped_0041ed40(void*,void*);   // AL [+DA]
std::uint8_t __fastcall texture_is_processed_0041ec30(void*,void*);           // AL [+E5]
std::uint8_t __fastcall texture_is_txi_loaded_0041ec50(void*,void*);          // AL [+E6]
char*        __fastcall texture_get_name_0041efe0(void*,void*);                // basic+78 | 0
texture::I   __fastcall texture_get_file_range_0041f000(void*,void*);
float        __fastcall texture_get_env_map_alpha_0041f180(void*,void*);      // ST0 (fld)
std::uint8_t __fastcall texture_get_map_texels_to_pixels_0041f120(void*,void*); // AL [+DF]
float        __fastcall texture_get_bump_intensity_0041edd0(void*,void*);     // ST0
float        __fastcall texture_get_specular_bump_intensity_0041edf0(void*,void*); // ST0
float        __fastcall texture_get_diffuse_bump_intensity_0041ee30(void*,void*);  // ST0
void*        __fastcall texture_get_specular_color_0041ed60(void*,void*,void*);   // ret 4 (hidden Vector result)
texture::U   __fastcall texture_get_txi_size_0041ed20(void*,void*);
void*        __fastcall texture_get_txi_data_0041ec90(void*,void*);
texture::U   __fastcall texture_get_texture_format_0041f1f0(void*,void*);
void         __fastcall texture_set_texture_format_0041f260(void*,void*,texture::U);        // ret 4
texture::U   __fastcall texture_set_txi_data_0041ecb0(void*,void*,void*,texture::I);        // ret 8 (texture.cpp V2)
texture::U   __fastcall texture_set_txi_loaded_0041ec70(void*,void*,texture::U);            // ret 4 (texture.cpp V1)
void         __fastcall texture_increment_tid_index_0041f3b0(void*,void*,texture::U);       // ret 4
texture::U   __fastcall texture_gl_image_00421430(void*,void*,texture::U,texture::U);       // ret 8
void*        __fastcall texture_reference_scalar_delete_00423130(void*,void*,texture::U);   // ret 4 (CAurTexture dtor)
// 0073F0AC slot +04 = 0063E7F0 (33 C0 C3). D24/E-D2: the slot's own type (same as Basic +04 00641DB0); ECX ignored.
font::Texture* __fastcall texture_reference_query_0063e7f0(font::Texture*,void*);
texture::U   __fastcall texture_free_image_0041f310(void*,void*);                           // CAurTexture::FreeImage
}

namespace texture_vtable {
// +08..+AC: identical in both tables.
struct CommonSlots {
 decltype(&texture_join::referenced_004205c0)            s08;
 decltype(&texture_is_cube_map_0041ebf0)                 s0c;
 decltype(&texture_is_invalid_004205d0)                  s10;
 decltype(&texture_is_diffuse_bump_map_0041ee70)         s14;
 decltype(&texture_is_specular_bump_map_0041eeb0)        s18;
 decltype(&texture_is_bump_map_0041eef0)                 s1c;
 decltype(&texture_is_normal_map_0041ef50)               s20;
 decltype(&texture_is_using_global_alpha_0041ef80)       s24;
 decltype(&texture_is_fully_loaded_0041ec10)             s28;
 decltype(&texture_is_environment_mapped_0041ed40)       s2c;
 decltype(&texture_is_processed_0041ec30)                s30;
 decltype(&texture_is_txi_loaded_0041ec50)               s34;
 decltype(&texture_font_info_0041efa0)                   s38;
 decltype(&texture_get_image_0041efc0)                   s3c;
 decltype(&texture_get_name_0041efe0)                    s40;
 decltype(&texture_get_file_range_0041f000)              s44;
 decltype(&texture_get_width_0041f020)                   s48;
 decltype(&texture_get_height_0041f040)                  s4c;
 decltype(&texture_get_bpp_0041f060)                     s50;
 decltype(&texture_get_num_mip_maps_0041f080)            s54;
 decltype(&texture_get_num_x_0041f0a0)                   s58;
 decltype(&texture_get_num_y_0041f0c0)                   s5c;
 decltype(&texture_get_clamp_0041f140)                   s60;
 decltype(&texture_get_compressed_size_0041f1a0)         s64;
 decltype(&texture_get_env_map_alpha_0041f180)           s68;
 decltype(&texture_get_mip_map_0041f0e0)                 s6c;
 decltype(&texture_get_filter_0041f100)                  s70;
 decltype(&texture_get_map_texels_to_pixels_0041f120)    s74;
 decltype(&texture_get_bump_map_scaling_0041f160)        s78;
 decltype(&texture_get_bump_intensity_0041edd0)          s7c;
 decltype(&texture_get_specular_bump_intensity_0041edf0) s80;
 decltype(&texture_get_diffuse_bump_intensity_0041ee30)  s84;
 decltype(&texture_get_specular_color_0041ed60)          s88;
 decltype(&texture_get_compression_format_0041f1c0)      s8c;
 decltype(&texture_get_txi_size_0041ed20)                s90;
 decltype(&texture_get_txi_data_0041ec90)                s94;
 decltype(&texture_get_texture_format_0041f1f0)          s98;
 decltype(&texture_set_texture_format_0041f260)          s9c;
 decltype(&texture_set_txi_data_0041ecb0)                sa0;
 decltype(&texture_set_txi_loaded_0041ec70)              sa4;
 decltype(&texture_increment_tid_index_0041f3b0)         sa8;
 decltype(&texture_gl_image_00421430)                    sac;
};
// +00..+AC have ONE type in both tables (E-D2): Head is the first member of each table (pointer-interconvertible).
using DeletingDtor=void* (__fastcall*)(void*,void*,texture::U);    // +00, thiscall RET4 flags (D1 shape)
struct Head {
 DeletingDtor                                            s00;
 decltype(&texture_owner::basic_query_00641db0)          s04;  // Ref*(Ref*,void*) in both tables
 CommonSlots                                             common;
};
using Bind=decltype(&texture_bind_004214b0);
struct BasicVTable {                                      // 0073F160
 Head                                                    head;
 decltype(&texture_resource::free_image_0041f570)        sb0;
 Bind                                                    sb4;
};
struct ReferenceVTable {                                  // 0073F0A8
 Head                                                    head;
 decltype(&texture_free_image_0041f310)                  sb0;
 Bind                                                    sb4;
};
struct BindTail {Bind sb4;};                              // +B4 view (same type in both tables)
static_assert(sizeof(CommonSlots)==0xa8&&sizeof(Head)==0xb0&&sizeof(BasicVTable)==0xb8&&sizeof(ReferenceVTable)==0xb8,"46 DWORD slots");
static_assert(offsetof(BasicVTable,sb0)==0xb0&&offsetof(BasicVTable,sb4)==0xb4&&offsetof(ReferenceVTable,sb4)==0xb4&&
              offsetof(Head,common)==8&&offsetof(CommonSlots,s38)==0x30&&offsetof(CommonSlots,s30)==0x28,"slot offsets (+38 font info, +30 IsProcessed)");
// Typed dispatch: reload the receiver's live table pointer at each call (original `mov eax,[ecx]; call [eax+n]`),
// then view it as the table's first member (object-pointer conversion only; D24).
inline const Head* head(const font::Texture* object){
 void** const table=object->vtable;
 return reinterpret_cast<const Head*>(table);
}
inline const BindTail* bind_tail(const font::Texture* object){
 void** const table=object->vtable;
 return reinterpret_cast<const BindTail*>(reinterpret_cast<const char*>(table)+offsetof(BasicVTable,sb4));
}
extern const BasicVTable basic_vtable_0073f160;
extern const ReferenceVTable reference_vtable_0073f0a8;
}
