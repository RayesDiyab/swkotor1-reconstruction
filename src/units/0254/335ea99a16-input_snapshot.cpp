#include "dac3197bde-input_snapshot.h"
namespace kotor_reconstruction {
void InputInternalView::GetMouseDelta(unsigned long* x, unsigned long* y) {
    *x = delta_x;
    // The first output may alias delta_y. Preserve the original second load order.
    *y = delta_y;
}
void InputView::GetMouseDelta(unsigned long* x, unsigned long* y) {
    internal->GetMouseDelta(x, y);
}
unsigned char InputView::GetMouseMode() {
    return internal->mouse_mode;
}
void InputView::SetCaptureKeyboardKeys(unsigned long value) {
    internal->capture_keyboard_keys = value;
}
unsigned long InputView::GetLastSetMouseDownState() {
    return internal->last_mouse_down;
}
}
