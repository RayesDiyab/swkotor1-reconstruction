#pragma once
// Fork F3 bodies: ATI fragment-shader route of the renderer.
#include <cstdint>
#include "ccf5cb551c-render_shared_lowers.h"  // texture::Basic forward declaration (fork F2)
namespace kotor_renderer_init {
// 004794E0 GLRender::InitializeATIFragmentShaders: no args, plain RET (0047A27C). Declared
// __stdcall by the consumer extension_loader.h (00437BD call 004794E0); RET 0 == plain ret.
void __stdcall InitializeATIFragmentShaders_004794e0();
}
namespace kotor_renderer_init {
// 0045F8B0 AurCubeMapAvailable: single owner lane-upload (source/capabilities_0045f.cpp; upload.h:47),
// full-EAX cache 0078E530 owned there. Declaration only.
std::uint32_t __cdecl CubeMapAvailable_0045f8b0();
}
namespace kotor_renderer_gl {
// LoadExtensions ATI dispatch targets (cells 007A47E0 / 007A6820 / 007A47DC, 004372A5..004372B9).
// 0047A2F0: cdecl; [esp+4] mode compared unsigned (0047A2F4 cmp 25h / ja); [esp+8..+14] four
// CAurTexture receivers (0047A407/0047A423/0047A440/0047A45D); EAX not meaningful.
// Cell 007A47E0 type shared with F2 EnableCombiners_0042e600.
void __cdecl EnableFragmentShaders_0047a2f0(std::uint32_t mode, texture::Basic* texture0, texture::Basic* texture1,
    texture::Basic* texture2, texture::Basic* texture3);
// 0047A280/0047A2A0: cdecl; [esp+4] const float* xyz (0047A280), address of the incoming
// [esp+8] float word passed as w (0047A284 lea eax,[esp+8]).
void __cdecl SetFragmentShaderConstant0_0047a280(const float* xyz, float w);
void __cdecl SetFragmentShaderConstant1_0047a2a0(const float* xyz, float w);
// 0044EEE0 AurGetGLError: returns 0 (glGetError result) or gluErrorString's pointer.
const std::uint8_t* __cdecl AurGetGLError_0044eee0();
// 00426550 GLRender::SetCurrentStageTID: [esp+4] stage, [esp+8] texture id.
void __cdecl SetCurrentStageTID_00426550(std::uint32_t stage, std::uint32_t texture_id);
// 004233E0 AurTextureGetNormalizationCubeMap: returns a CAurTexture receiver or 0.
texture::Basic* __cdecl TextureGetNormalizationCubeMap_004233e0();
}
