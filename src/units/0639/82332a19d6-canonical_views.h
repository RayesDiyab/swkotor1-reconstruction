#pragma once
#include <windows.h>
#include <GL/gl.h>
// Proposed owner aliases, never definitions or replacement storage. Main6 must
// bind each reference by constant initialization to its sole durable live cell.
// Array-first aliases require the original contiguous range and producer lifetime.
namespace kotor_gui_canonical_required {
extern const volatile unsigned int& pulse_0078d3d4;
extern const volatile unsigned int& busy_007bb4d0;
extern const volatile unsigned int& disable_0078d45c;
extern volatile unsigned int& reset_007a689c;



extern const volatile short& index_007b9460;

extern const unsigned char& dimensions_first_007b946c;
extern const volatile float& alpha_first_007b9248;
extern const volatile float& hundred_00741838;
extern const volatile float& quarter_0073edf0;
extern const volatile float& negative_hundred_0074186c;
extern const volatile float& epsilon_00741870;
extern const volatile float& zero_0073d700;
extern const volatile float& one_0073d764;
extern const volatile float& channel_0073ede4;
extern const volatile unsigned int& quads_0073f34c;
extern decltype(&::glDisable) const volatile& disable_0073d22c;
extern decltype(&::glBegin) const volatile& begin_0073d2d0;
extern decltype(&::glColor4ubv) const volatile& color_0073d2e8;
extern decltype(&::glTexCoord2f) const volatile& texcoord_0073d2cc;
extern decltype(&::glVertex4f) const volatile& vertex_0073d324;
extern decltype(&::glEnd) const volatile& end_0073d2c4;
extern decltype(&::glEnable) const volatile& enable_0073d390;
}
