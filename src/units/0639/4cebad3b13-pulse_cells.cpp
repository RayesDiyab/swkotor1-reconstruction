// Sole storage + reference bindings for the six ports requested by the
// task-6 gui-pulse-00414a50-r1/r2 donor (pulse_owner_request.h). The output
// port binds to the packet-1 cell kotor_renderer_cells::pulse_0078d3d4, which
// is also what kotor_gui_canonical_required::pulse_0078d3d4 (text draw) reads:
// one object, one producer, one consumer view.
#include "9342737640-pulse_owner_request.h"
#include "4867c506f0-gui_cells.h"

namespace kotor_renderer_cells {
// 0078D3D0 alphaPulseTimer: file-backed .data, bytes 00 00 40 3F (0.75).
// Accessed only by UpdatePulsing (00414A50 FLD, 00414A5A FST, 00414A7A MOV
// 3F400000, 00414AB2 FSUB).
volatile float pulse_timer_0078d3d0 = 0x1.8p-1f;
// 007A4578 alphaPulsingUp: .data virtual tail, loader zero. Accessed only by
// UpdatePulsing (00414A6D read, 00414A84 store, 00414A89 read).
volatile std::uint32_t pulse_up_007a4578;
// Readonly .rdata literals (exact DWORDs).
extern const volatile float four_thirds_0073e318 = 0x1.555556p+0f;   // AB AA AA 3F
extern const volatile float three_quarters_0073e31c = 0x1.8p-1f;     // 00 00 40 3F
extern const volatile float two_fifths_0073e320 = 0x1.99999ap-2f;    // CD CC CC 3E
}

namespace kotor_pulse_canonical_required {
volatile float& timer_0078d3d0 = kotor_renderer_cells::pulse_timer_0078d3d0;
volatile unsigned int& producer_bits_0078d3d4 = kotor_renderer_cells::pulse_0078d3d4;
volatile unsigned int& direction_007a4578 = kotor_renderer_cells::pulse_up_007a4578;
const volatile float& factor_0073e318 = kotor_renderer_cells::four_thirds_0073e318;
const volatile float& period_0073e31c = kotor_renderer_cells::three_quarters_0073e31c;
const volatile float& low_0073e320 = kotor_renderer_cells::two_fifths_0073e320;
}
