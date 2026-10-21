// Compiled/import-audited only. Never linked into a native fixture.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <gl/GL.h>
#include "5e01185142-render_system_gl.h"
namespace aurora_context_adoption {
static_assert(sizeof(void*)==4,"original x86 handle width");
std::uint32_t __stdcall current_context_0073d2e0(){return reinterpret_cast<std::uint32_t>(::wglGetCurrentContext());}
std::uint32_t __stdcall current_dc_0073d2dc(){return reinterpret_cast<std::uint32_t>(::wglGetCurrentDC());}
renderer_system::Views make_live_system_view(
    volatile std::uint32_t& context,volatile std::uint32_t& dc,
    volatile std::uint32_t (&cache)[6],volatile std::uint32_t& zero) noexcept {
    return {&current_context_0073d2e0,&current_dc_0073d2dc,&context,&dc,cache,&zero};
}
}
