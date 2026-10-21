#pragma once
// Exact Task29 dispatcher declarations, supplied by the pinned contract epoch.
#include "1430fdca85-sound_mode.h"
// Inputs borrow an internal with words at+0x0c/+0x50/+0x54/+0x5c. The base cell
// and base+4 TimerDependency must be live only when the flag causes a transition.
// No-op Mute returns the existing flag bits; no-op Unmute returns zero. Changed
// paths return the low product bits left by the original unsigned MUL.
