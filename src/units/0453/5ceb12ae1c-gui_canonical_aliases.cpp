// The seven Task17 state aliases (the "7 state aliases" left open by Task17
// gui-immutable-data-aliases-r1), each a reference bound to its sole owner
// cell in gui_cells.cpp. The other fifteen come unchanged from Task17
// immutable_aliases.cpp (8 readonly) and opengl_iat_aliases.cpp (7 IAT).
#include "15d4c2890e-canonical_views.h"
#include "33a7cc3451-gui_cells.h"

namespace kotor_gui_canonical_required {
const volatile unsigned int& pulse_0078d3d4 = kotor_renderer_cells::pulse_0078d3d4;
const volatile unsigned int& busy_007bb4d0 = kotor_renderer_cells::busy_007bb4d0;
const volatile unsigned int& disable_0078d45c = kotor_renderer_cells::disable_0078d45c;
volatile unsigned int& reset_007a689c = kotor_renderer_cells::reset_007a689c;
const volatile short& index_007b9460 = kotor_renderer_cells::viewport_index_007b9460;
// First byte of record 0's width WORD (record base 007B9468 + 4 == 007B946C).
// Consumers read signed WORDs at +index*10+{0,2} from here (draw.cpp:4).
// Character-type view of the same U16[50] object; no second object or copy.
const unsigned char& dimensions_first_007b946c =
    reinterpret_cast<const unsigned char*>(kotor_renderer_cells::viewport_frame_words_007b9468)[4];
const volatile float& alpha_first_007b9248 = kotor_renderer_cells::viewport_alphas_007b9248[0];
}
