#pragma once
// Fork F2 (lane-renderer-bodies): NV register-combiner dispatch targets stored by
// LoadExtensions_00436490 (004373CC..004373E0) into 007A47E0/007A6820/007A47DC.
#include <cstdint>
#include "ccf5cb551c-render_shared_lowers.h"
namespace kotor_renderer_gl {
// 007A47E0 value on the NV route (004373CC). Cell type (shared with F3 0047A2F0): void __cdecl(U32 mode, CAurTexture* x4); mode compared unsigned (cmp 25h/ja 0042E605/0042E610),
// plain RET (caller pops 5 dwords).
void __cdecl EnableCombiners_0042e600(std::uint32_t mode, texture::Basic* texture1, texture::Basic* texture2,
                                      texture::Basic* texture3, texture::Basic* texture4);
// 007A6820 value on the NV route (004373D6): void __cdecl(Vector*, float); constant color 0 (852Ah).
void __cdecl SetFragmentShaderConstant0_00424940(const float* vector, float w);
// 007A47DC value on the NV route (004373E0): void __cdecl(Vector*, float); constant color 1 (852Bh).
void __cdecl SetCombinerConstant1_00424960(const float* vector, float w);
// 00423CD0: cdecl, plain RET; callers push one CAurTexture* that the body never reads.
void __cdecl DisableCombiners_00423cd0(texture::Basic* unused);
}
