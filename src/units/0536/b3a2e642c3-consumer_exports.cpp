#include "6fe11fddcb-pbuffer.h"
#include "27b4f16387-framebuffer.h"
#include "49d96e5f57-soft_shadow.h"

namespace kotor_renderer_init {
void* __cdecl CreatePBuffer_00426560(FBWord width, FBWord height) {
    return kotor_pbuffer::CreatePBuffer_00426560(width, height);
}
}
namespace kotor_soft_shadow {
void* __cdecl CreatePBuffer_00426560(U32 width, U32 height) {
    return kotor_pbuffer::CreatePBuffer_00426560(width, height);
}
}
