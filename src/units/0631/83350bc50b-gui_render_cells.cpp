// lane-gui-render: sole storage for the GUI render cells listed in gui_render.h.
// Cold values are the original file bytes (GOG swkotor.exe; .data raw-backed to
// 007A3FFF, 007A4000+ is loader zero-fill). Xrefs: full .text disassembly scan
// (lane-gui-draw/exe-text.asm); no other lane or M40 object defines these.
#include "1277741e89-gui_render.h"

namespace kotor_gui_render_cells {
// 0078E43C file bytes 00 00 80 3F.
volatile std::uint32_t gui_image_alpha_0078e43c = 0x3F800000u;
}
