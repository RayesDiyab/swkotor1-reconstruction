#pragma once
#include "4b2b3e80d1-renderer_initialize.h"
namespace aurora_context_adoption {
using ChooseArb=kotor_renderer_init::I32 (__stdcall*)(void*,const kotor_renderer_init::I32*,const float*,kotor_renderer_init::U32,kotor_renderer_init::I32*,kotor_renderer_init::U32*);
using SwapInterval=kotor_renderer_init::I32 (__stdcall*)(kotor_renderer_init::I32);
// D11: glCompressedTexImage2DARB (APIENTRY): target, level, internalformat, width, height, border, imageSize, data.
using CompressedTexImage2D=void (__stdcall*)(kotor_renderer_init::U32,kotor_renderer_init::I32,kotor_renderer_init::U32,kotor_renderer_init::I32,kotor_renderer_init::I32,kotor_renderer_init::I32,kotor_renderer_init::I32,const void*);
struct LiveExtensions {
    ChooseArb volatile* choose_007bb730;
    SwapInterval volatile* swap_007bb538;
    CompressedTexImage2D volatile* compressed_tex_image_2d_007bb774; // D11: read at 00424AF2/00424B85/00424F33/00424FC5/004242EB/00424377
};
// Proposed borrowed descriptor only; Main6 defines and publishes it once over
// real extension cells, whose values are produced by the real GL initialization.
extern const LiveExtensions& live_extensions;
static_assert(sizeof(LiveExtensions)==12,"three native x86 cell addresses");
}
