#pragma once
#include "afb645df26-sound_mode.h"
// Live internal+0x40/+0x44 point to lists whose first pointer is a nullable
// cursor. Each nonnull cursor is a live canonical Node with nonnull payload at
// +8. Mode depth is 0..9 whenever the branch actually reads it. Lower callbacks
// must retain these live-object conditions. The flag write-view belongs to Main6.

