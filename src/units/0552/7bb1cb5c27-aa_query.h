#pragma once
#include <cstdint>
#include <windows.h>
namespace kotor_renderer_init {
using I32=std::int32_t;using U32=std::uint32_t;using U16=std::uint16_t;
using ChooseArb=I32(__stdcall*)(void*,const I32*,const float*,U32,I32*,U32*);
using PixelAttributes=I32(__stdcall*)(void*,I32,I32,U32,const I32*,I32*);
struct AntiAliasingQueryBindings {
    // Revision (same hunk shape as the owner-accepted aa_probe.h instance hunk): borrow
    // the M40 owner kotor_menu_window_cells::render_007a39d8 (HWND volatile) directly.
    HWND const volatile* window_007a39d8;const U16* color_007b9220;const U16* depth_007b9224;
    const ChooseArb* choose_007bb730;const PixelAttributes* attributes_007bb63c;
};
extern AntiAliasingQueryBindings& aa_query;
extern I32 __stdcall AntiAliasingAvailable_0046ed70();
extern void* __stdcall GetDC_0073d408(void*);
extern I32 __stdcall ReleaseDC_0073d404(void*,void*);
I32 __cdecl QueryAntiAliasingMode_0044f140(I32);
}
