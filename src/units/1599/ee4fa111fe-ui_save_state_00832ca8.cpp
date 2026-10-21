// 00832CA8 — single canonical 4-byte zero-initialized BSS cell owned by this lane.
//
// Original read: `cmp dword ptr [0x832ca8], ebp` at 00633E5C (00633C50).  The
// original has no direct writer; the cell is zero from load.  This storage-only unit
// provides exactly one definition.  It contains:
//   * no initializer function (static zero initialization only),
//   * no writer,
//   * no trap, no stub, no alias.
// Do not add a second definition or a load-screen fallback.
//
// C symbol produced: _kotor_cell_00832ca8 (4 bytes, .bss)

extern "C" unsigned int kotor_cell_00832ca8 = 0;
