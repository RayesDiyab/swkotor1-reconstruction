#pragma once
#include "19b1371aa2-texture.hpp" // E1: the single texture::Basic epoch (lane-texture-epoch)

namespace texture_scheduler {
using U=texture::U;
using I=texture::I;
using Basic=texture::Basic;
using Ref=texture::Ref;
using List=texture::List;
using ReleaseComponents=void (__cdecl*)(void*,I,I);

// Borrow declarations only. Main supplies actual objects and one view; no storage here.
struct Cells {
 List* pending_007a4770;
 List* lookup_007a4798;
 List* ordered_007a47a4;
 List* ids_007a478c;
 const unsigned char volatile* skip_007a46d5;
 const I volatile* budget_0078d3f4;
 const U volatile* scale_0078d3f0; // E1 edit H3: raw cell owned by lane-menu-tables (U32); read only by x87 asm (fmul dword)
 U volatile* count_007a4724;
 U volatile* count_007a4730;
 U volatile* count_007a4728;
 U volatile* count_007a472c;
 const U volatile* current_007a46b0;
 U volatile* peak_007a4754;
 U volatile* peak_007a4758;
 U volatile* peak_007a475c;
 U volatile* peak_007a4760;
 const float volatile* delta_0078e574;
 float volatile* peak_delta_007a4750;
 U volatile* frame_007a46f4;
 U volatile* lookup_count_007a4748;
 U volatile* maximum_id_007a46bc;
 const unsigned char* const volatile* palette_007a4704;
 const I volatile* palette_parameter_007a4708;
 ReleaseComponents const volatile* release_components_007a46fc;
 const unsigned char volatile* fill_alpha_007a46d4;
 const float* fill_vector_007a4764;
 const char* norm_cube_map_0073ee34; // Exact twelve original bytes, including NUL.
};
extern const Cells& cells;

// Physical word transports requiring Main's explicit genuine-provider joins.
// These declarations do not claim C++ type identity with a foreign donor header.
extern "C" {
 void __cdecl scheduler_sort_words_0041e9c0(U*,I);
 void __cdecl scheduler_gamma_words_0045dd90(I,I,U,void*,U);
 void* __cdecl scheduler_decode_0045de10(void*,I,I,const unsigned char*,I,U*,I);
 void __cdecl scheduler_fill_alpha_0045f0d0(void*,I,I,U,const float*);
 float __cdecl scheduler_alpha_mean_0045e790(const void*,I,I,U); // Physical ST0 result.
 void __fastcall scheduler_load_plt_0041f9b0(Basic*,void*);
 // E1: LoadImageA 0041F6A0 (char* full EAX) and glImage 00420970 (U, RET4) use the shared
 // declarations acquisition::load_image_a_0041f6a0 and texture::basic_gl_image_00420970.
 // E1 edit H4: 00420C20 -> texture::basic_as_normalize_vector_cube_map_00420c20 (texture.hpp, this lane).

 U __stdcall texture_make_current_00421ee0();
 void __stdcall build_and_store_all_004217f0();
 void __fastcall build_and_store_texture_004216a0(Basic*,void*);
 void __fastcall construct_image_0041fa30(Basic*,void*);
}
static_assert(sizeof(Basic)==0xec && sizeof(Ref)==24 && sizeof(List)==12,"borrowed nominals");
static_assert(offsetof(Basic,prefix)==0 && offsetof(Basic,components)==0x3c && offsetof(Basic,f44)==0x44,"actual receiver fields");
static_assert(offsetof(Basic,flags)==0xd8 && offsetof(Basic,c2)==0xc2 && offsetof(Basic,c4)==0xc4,"flag and grid offsets");
}
