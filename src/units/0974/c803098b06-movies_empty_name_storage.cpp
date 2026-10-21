// lane-m1-movies: D69-style INTERIM form of .data 008338D8 (stage this unit only while movies_empty_name_init.cpp is
// not yet admitted by the initializer allowlist; exactly one of the two units is staged).
// The original initializer 0073B630 runs CExoString::CExoString() 005B3190 (writes data = 0 and length = 0 over
// .bss zero-fill: no observable change) and registers 0073C150 (~CExoString on an empty string: free(NULL), no
// effect). Recorded deviation (D69 pattern): the constructor call and the exit-time disposer are not reproduced;
// the cell content is identical for the whole process lifetime.
extern "C" {
alignas(4) unsigned char kotor_cell_movie_name_008338d8[8] = {};   // 008338D8: {data 0, length 0}
}
