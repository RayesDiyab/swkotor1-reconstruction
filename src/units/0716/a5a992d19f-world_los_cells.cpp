// lane-world-los: original .data cells owned by this lane (values read from swkotor.exe .data).
#include "dc46124e28-world_los.h"

namespace kotor_world_los {
// 007A693C CExoArrayList<ulong> {data 7A693C, size 7A6940, capacity 7A6944}; original .data bytes are all zero.
// Writers: 00435B40 / 00435B90 (here); reader 00432720 (lane-3d-render cells.drawable_squares_007a693c view).
List drawable_squares_007a693c = {0, 0, 0};
// 007A19B0 .data Vector = 00000000 00000000 0000803F -> (0, 0, 1). No writer found in the image (all references
// are reads/pushes: 004ACB38.., 005989D3.., 00599342.., 0061539D); kept non-const as original .data.
Vector up_007a19b0 = {0.0f, 0.0f, 1.0f};
}
