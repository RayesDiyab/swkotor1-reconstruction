#pragma once
// texture_controller.hpp - lane-texture-epoch PROPOSAL (owner of TextureController, the Basic +34 object).
// Classes (Ghidra labels): TextureController 00741C08 (base), Life 0073ED54, Perlin 0073ED70, Random 0073ED90,
// RingDistort 0073EDAC, Water 00741D2C, Wave 00741D80, Arturo 00741D9C, CycleTID 00741DD8.
// Shared slots +00..+18 (D1 fastcall shapes; stack words from each body's `ret N`):
//   +00 scalar deleting dtor (RET4 flags, EAX=this)   +04 Control (RET0)   +08 Init (RET0)
//   +0C ParseField(line) (RET4; Ghidra void)          +10 IsLinear (AL)    +14 Distort(height map) (RET4, EAX=0x7BB870 buffer)
//   +18 SetImage (RET0).  Perlin +1C BuildPerlinImage, Arturo +1C BuildArturoImage (RET0).
// No menu or fnt_console texture instantiates a controller: the 12 main-menu GUI TPCs and the textures named in
// mainmenu/mainmenu_model MDLs carry no 'proceduretype' (tools/txi_procedure_scan.py, tools/mainmenu_procedure_scan.py).
#include "19b1371aa2-texture.hpp"
#include "283ae0c931-texture_vtable.hpp"

namespace texture_controller {
using texture::U; using texture::I;
struct TextureControllerVTable {
 void*         (__fastcall* s00)(void*,void*,U);
 void          (__fastcall* s04)(void*,void*);
 void          (__fastcall* s08)(void*,void*);
 void          (__fastcall* s0c)(void*,void*,const char*);
 std::uint8_t  (__fastcall* s10)(void*,void*);
 std::uint8_t* (__fastcall* s14)(void*,void*,const std::uint8_t*);
 void          (__fastcall* s18)(void*,void*);
};
static_assert(sizeof(TextureControllerVTable)==0x1c,"shared controller slots +00..+18");
struct PerlinVTable {TextureControllerVTable base; void (__fastcall* s1c)(void*,void*,std::uint8_t* image,I size);}; // +1C BuildPerlinImage RET8
struct ArturoVTable {TextureControllerVTable base; void (__fastcall* s1c)(void*,void*);};                          // +1C BuildArturoImage RET0
static_assert(offsetof(PerlinVTable,s1c)==0x1c&&offsetof(ArturoVTable,s1c)==0x1c,"+1C Build*Image");

// TextureController (0x38 bytes; ctor 00462A70, dtor 00460CF0, ParseField 00462B00).
struct TextureController {
 const TextureControllerVTable* volatile vtable; // +00 (Perlin/Arturo publish &table.base of their extended table)
 font::Texture* volatile owner;                  // +04 texture (ctor arg); SetImage/Distort call its +48/+4C/+50/+3C
 I volatile f08;                                 // +08 0 (base ctor 00462AA8); Arturo ctor -1 (0046334C)
 I f0c;                                          // +0C "distort"       (00462B7B parse_int target)
 I f10;                                          // +10 "distortangle"  (00462BA3); ctor 1
 float f14;                                      // +14 "distortionamplitude" (00462BCB); ctor 5.0f
 float f18;                                      // +18 "speed" (00462BF3); ctor 1.0f
 texture::List channel_scale;                    // +1C CExoArrayList<float> "channelscale*"
 texture::List channel_translate;                // +28 CExoArrayList<float> "channeltranslate*"
 U volatile image;                               // +34 copy of the owner image (SetImage 0045FDA5), freed by dtor
};
static_assert(sizeof(TextureController)==0x38&&offsetof(TextureController,channel_scale)==0x1c&&
              offsetof(TextureController,channel_translate)==0x28&&offsetof(TextureController,image)==0x34,"base layout");
// Basic +34 holds the controller address as a raw DWORD (U volatile f34); object-pointer conversion only.
inline TextureController* from_word(U word){return reinterpret_cast<TextureController*>(word);}
// Live table reload at each call (original mov edx,[ecx]; call [edx+n]).
inline const TextureControllerVTable* table(const TextureController* c){return c->vtable;}
// Perlin/Arturo objects publish their extended table; its first member is the shared table (pointer-interconvertible).
inline const PerlinVTable* perlin_table(const TextureController* c){return reinterpret_cast<const PerlinVTable*>(c->vtable);}
inline const ArturoVTable* arturo_table(const TextureController* c){return reinterpret_cast<const ArturoVTable*>(c->vtable);}
}

extern "C" {
// ---- base TextureController (controller/controller_base.cpp) ----
void*         __fastcall texture_controller_ctor_00462a70(void* self,void*,font::Texture* owner);   // RET4
void          __fastcall texture_controller_dtor_00460cf0(void* self,void*);
void*         __fastcall texture_controller_delete_00462ae0(void* self,void*,texture::U flags);
std::uint8_t  __fastcall texture_controller_is_linear_0041f250(void*,void*);
void          __fastcall texture_controller_set_image_0045fd70(void* self,void*);
std::uint8_t* __fastcall texture_controller_distort_00460d70(void* self,void*,const std::uint8_t* height);
std::uint8_t* __fastcall texture_controller_height2channel_00461060(void* self,void*,const std::uint8_t* height,std::uint8_t* out); // RET8
void          __fastcall texture_controller_parse_field_00462b00(void* self,void*,const char* line);
void          __fastcall texture_controller_purecall_006fb8de(void*,void*);
std::uint8_t  __fastcall texture_controller_return_true_005a4cf0(void*,void*);
void          __fastcall texture_controller_void_return_004015a0(void*,void*);
float __cdecl cosine_lut_004aadf0(float);
double __cdecl texture_controller_grid_sample_004608a0(const void* grid,float x,float y,texture::I w,texture::I h); // ST0
void __cdecl image_lowpass_low_quality_blur_0045e8b0(std::uint8_t* out,const std::uint8_t* image,texture::I w,texture::I h,texture::I phase);
void __cdecl image_lowpass_low_quality_blur2_0045ea70(std::uint8_t* image,texture::I w,texture::I h);
void __cdecl image_lowpass_blur_0045ecb0(std::uint8_t* image,texture::I w,texture::I h,texture::I bpp);
float __cdecl sine_lut_004aae30(float);
extern std::uint8_t texture_controller_new_image_007bb870[0x10000];
// ---- derived classes ----
void* __fastcall life_controller_delete_0041f290(void*,void*,texture::U);
void  __fastcall life_controller_dtor_0041f2b0(void*,void*);
void  __fastcall life_controller_control_00460250(void*,void*);
void  __fastcall life_controller_init_00460120(void*,void*);
void* __fastcall perlin_controller_delete_0041f330(void*,void*,texture::U);
void  __fastcall perlin_controller_dtor_0041f350(void*,void*);
void  __fastcall perlin_controller_control_004615a0(void*,void*);
void  __fastcall perlin_controller_init_0045fdd0(void*,void*);
void  __fastcall perlin_controller_build_0045fe50(void*,void*,std::uint8_t*,texture::I); // RET8
void* __fastcall random_controller_delete_0041f3d0(void*,void*,texture::U);
void  __fastcall random_controller_dtor_0041f3f0(void*,void*);
void  __fastcall random_controller_control_00462800(void*,void*);
void  __fastcall random_controller_init_004609f0(void*,void*);
void* __fastcall ring_controller_delete_0041f450(void*,void*,texture::U);
void  __fastcall ring_controller_dtor_0041f470(void*,void*);
void  __fastcall ring_controller_control_00460a90(void*,void*);
void  __fastcall ring_controller_init_00460a50(void*,void*);
void* __fastcall water_controller_ctor_004630b0(void*,void*,font::Texture*);
void* __fastcall water_controller_delete_00463100(void*,void*,texture::U);
void  __fastcall water_controller_dtor_00463120(void*,void*);
void  __fastcall water_controller_control_00461820(void*,void*);
void  __fastcall water_controller_init_0045ff40(void*,void*);
void  __fastcall water_controller_parse_field_004631a0(void*,void*,const char*);
void* __fastcall wave_controller_ctor_00463260(void*,void*,font::Texture*);
void* __fastcall wave_controller_delete_00463290(void*,void*,texture::U);
void  __fastcall wave_controller_dtor_004632b0(void*,void*);
void  __fastcall wave_controller_control_00462030(void*,void*);
void  __fastcall wave_controller_init_00460000(void*,void*);
void* __fastcall arturo_controller_ctor_00463320(void*,void*,font::Texture*);
void* __fastcall arturo_controller_delete_00463360(void*,void*,texture::U);
void  __fastcall arturo_controller_dtor_00463380(void*,void*);
void  __fastcall arturo_controller_control_00462520(void*,void*);
void  __fastcall arturo_controller_init_00460560(void*,void*);
void  __fastcall arturo_controller_parse_field_00463400(void*,void*,const char*);
void  __fastcall arturo_controller_build_00460690(void*,void*);
void* __fastcall cycle_controller_ctor_00463470(void*,void*,font::Texture*);
void* __fastcall cycle_controller_delete_004634a0(void*,void*,texture::U);
void  __fastcall cycle_controller_dtor_004634c0(void*,void*);
void  __fastcall cycle_controller_control_00460970(void*,void*);
void  __fastcall cycle_controller_parse_field_004634d0(void*,void*,const char*);
}

namespace texture_controller {
extern const TextureControllerVTable base_vtable_00741c08;
extern const TextureControllerVTable life_vtable_0073ed54;
extern const PerlinVTable perlin_vtable_0073ed70;
extern const TextureControllerVTable random_vtable_0073ed90;
extern const TextureControllerVTable ring_distort_vtable_0073edac;
extern const TextureControllerVTable water_vtable_00741d2c;
extern const TextureControllerVTable wave_vtable_00741d80;
extern const ArturoVTable arturo_vtable_00741d9c;
extern const TextureControllerVTable cycle_tid_vtable_00741dd8;
}
