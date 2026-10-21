// lane-tlk: sole owner of the TLK cells and TLK-only original data.
//  007A3A08 .data CTlkTable* (Ghidra DEFINED_DATA "CTlkTable *"); file image 00000000;
//           written by StartServices at 005F89C0 (mov [007A3A08],eax); read at 005F4261,
//           005EA1AE, 005F8D3F and ~400 other sites. C linkage, plain void*: matches
//           lane-prepare game_prepare_producers.h:24 (words[Tlk] = &cell).
//  0078D3EC .data char* -> 0073ECD4 "V3.0"; read only by FetchInternal (0041E213, 0041E2B0).
//  0073ECD2 .rdata byte 07h; LIVE slot bound for 005F4180 (005F41BC, 005F4410).
#include "6db30bdc1c-tlk_core.h"

namespace {
const char tlk_version_0073ecd4[]="V3.0";                      // .rdata 56 33 2E 30 00
}
extern "C" {
void* kotor_cell_tlk_007a3a08=nullptr;
const char* kotor_cell_tlk_version_0078d3ec=tlk_version_0073ecd4;
const unsigned char kotor_rdata_live_count_0073ecd2=0x07;
}
