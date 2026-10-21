// texture_normalize_cubemap.cpp - lane-texture-epoch (D32/D48 takeover from lane-renderer-bodies renderer_gl_cubemap.cpp).
// 00420C20 CAurTextureBasic::AsNormalizeVectorCubeMap (thiscall, no stack words, RET). Re-verified against the
// original (build/full.disasm.txt 00420C20..00420D61). Corrections vs the lane-renderer-bodies text:
//  * glGenTextures writes Basic+58 directly (00420C83 lea edi,[esi+58h]; push edi) - no local + delayed store;
//  * 006FA7E6 maps to live_operator_new (epoch allocator mapping);
//  * Signed/float fields use the E1 accessors.
// MakeNormalizationCubeMap 00425EA0, its helper and the cells 0078D47C/0073F23C stay with lane-renderer-bodies.
#include "f31f0dbce3-texture.hpp"
#include <cstdint>

namespace kotor_renderer_init {
 extern void __stdcall glGenTextures_0073d24c(std::int32_t,std::uint32_t*);
 extern void __stdcall glBindTexture_0073d240(std::uint32_t,std::uint32_t);
 extern void __stdcall glTexParameteri_0073d23c(std::uint32_t,std::uint32_t,std::int32_t);
 extern void __stdcall glEnable_0073d390(std::uint32_t);
 extern void __stdcall glDisable_0073d22c(std::uint32_t);
}
namespace kotor_renderer_gl {
 std::uint32_t __cdecl MultitextureAvailable_0045f710();
 void __cdecl MakeNormalizationCubeMap_00425ea0(std::int32_t size);
}
namespace kotor_renderer_gl_procs { extern void (__stdcall* glActiveTextureARB_007bb80c)(std::uint32_t); }
namespace kotor_renderer_gl_cells {
 extern std::uint32_t current_stage_007a6898;           // uiCurrentStage
 extern std::uint32_t current_tid_007a683c[6];          // glCurrentTID[6]
 extern std::uint32_t normalize_enum_0078d47c;          // .data 0x0BA1
 extern const std::uint32_t texture_target_table_0073f23c[8];
}
namespace texture_upload_cells {
 extern std::uint32_t gl_texture_cube_map_0078d478;     // lane-upload owner (0x8513)
 extern std::int32_t anisotropy_007a685c;               // lane-upload owner
}

namespace {
template<class T> T load(const T& cell){return *static_cast<const volatile T*>(&cell);}
template<class T> void store(T& cell,T value){*static_cast<volatile T*>(&cell)=value;}
void select_stage(){
 if(kotor_renderer_gl::MultitextureAvailable_0045f710()!=0)                     // 00420CA8 / 00420D41
  kotor_renderer_gl_procs::glActiveTextureARB_007bb80c(load(kotor_renderer_gl_cells::current_stage_007a6898)+0x84c0u); // 00420CB1..00420CBC
}
}

extern "C" void __fastcall basic_as_normalize_vector_cube_map_00420c20(void* self_word,void*){
 using namespace kotor_renderer_init;
 texture::Basic* const self=static_cast<texture::Basic*>(self_word);
 const texture::U ids=self->f40;                                                // 00420C25
 self->bc=0;self->b8=0;self->ba=0;                                              // 00420C32..00420C40
 self->flags[10]=1;                                                             // 00420C47 +E2
 texture::field_store(self->f70,3);                                             // 00420C4D
 self->flags[4]=1;self->flags[5]=1;self->flags[3]=1;                            // 00420C54..00420C60 +DC,+DD,+DB
 texture::field_store(self->f68,0x100);                                         // 00420C66
 texture::field_store(self->f6c,0x600);                                         // 00420C6D
 if(ids==0)self->f40=reinterpret_cast<texture::U>(live_operator_new(4));        // 00420C74..00420C80
 glGenTextures_0073d24c(1,&self->f58);                                          // 00420C83..00420C88 (writes +58)
 const texture::U array=self->f40;                                              // 00420C8E reload
 *reinterpret_cast<texture::U volatile*>(array)=static_cast<const volatile texture::U&>(self->f58); // 00420C91..00420C93
 glEnable_0073d390(load(kotor_renderer_gl_cells::normalize_enum_0078d47c));     // 00420C95..00420C9C
 store(kotor_renderer_gl_cells::current_stage_007a6898,1u);                     // 00420CA2
 select_stage();
 const texture::U name=static_cast<const volatile texture::U&>(self->f58);      // 00420CC2
 glBindTexture_0073d240(load(texture_upload_cells::gl_texture_cube_map_0078d478),name); // 00420CC4..00420CCC
 const std::int32_t anisotropy=load(texture_upload_cells::anisotropy_007a685c); // 00420CD2
 const std::uint32_t cube=load(texture_upload_cells::gl_texture_cube_map_0078d478); // 00420CD8
 glTexParameteri_0073d23c(cube,0x84fe,anisotropy);                              // 00420CE3..00420CEA
 const std::uint32_t stage=load(kotor_renderer_gl_cells::current_stage_007a6898); // 00420CEC
 store(kotor_renderer_gl_cells::current_tid_007a683c[stage],name);              // 00420CF7
 kotor_renderer_gl::MakeNormalizationCubeMap_00425ea0(0x100);                  // 00420CF2..00420CFE
 // 00420D03..00420D2D: target [0073F23C] with [0073F364]=2800h/[0073F368]=2801h (.rdata constants).
 glTexParameteri_0073d23c(kotor_renderer_gl_cells::texture_target_table_0073f23c[0],0x2800,0x2601);
 glTexParameteri_0073d23c(kotor_renderer_gl_cells::texture_target_table_0073f23c[0],0x2801,0x2703);
 glDisable_0073d22c(load(texture_upload_cells::gl_texture_cube_map_0078d478));  // 00420D2F..00420D35
 store(kotor_renderer_gl_cells::current_stage_007a6898,0u);                     // 00420D3B
 select_stage();                                                                // 00420D41..00420D5B
}
