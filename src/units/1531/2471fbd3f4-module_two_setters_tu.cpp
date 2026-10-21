// Separate leaf TU for the two reconstructed setters.
// This is compiled as its own object with the recorded 0886 binder except
// /Oy- -> /Oy, because the originals are frame-less leaves and /Oy- inserts a
// push ebp/mov ebp,esp/pop ebp frame. It is not included into the 0886 TU.
#include "1f716f38f5-module_two_setters_exports.h"
#include "680dd4e4c2-module_two_setters.inc"
