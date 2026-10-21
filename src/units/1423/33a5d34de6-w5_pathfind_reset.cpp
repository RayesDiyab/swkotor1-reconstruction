// W5: CPathfindInformation reset family (reached from CSWSObject::ClearAllActions on the drive-control path).
// ResetAtEndOfPath 005D0EC0, ResetInterAreaPathSearchData 005D0E00, ResetGridSearchData 005CEFA0,
// ResetWayPointData 005CF060, ResetInterTileSearchData 005D00A0, DeleteFirstTileFValueAlternatives 005CEB70,
// and the unnamed tail 005CF130 (search-result reset). Pure field resets + live_free.
#include "73b9db80fe-w5_pathfind_reset.hpp"
#include <initializer_list>

using namespace w5;

namespace {
inline void free_field(void* pf, U off) { void* p = at<void*>(pf, off); if (p != nullptr) live_free(p); }
inline void zero3(void* pf, U off) { at<U>(pf, off) = 0; at<U>(pf, off + 4) = 0; at<U>(pf, off + 8) = 0; }
}

extern "C" {
// 005CEFA0
void __fastcall w5_pathfind_reset_grid_search_005cefa0(void* pf, void*) {
    zero3(pf, 0x170); zero3(pf, 0x17c);                                   // 005CEFA6..005CF007
    at<U>(pf, 0x164) = 0x442f0000u;                                       // 700.0f
    at<U>(pf, 0x168) = 0;
    at<U>(pf, 0x188) = 0;
    free_field(pf, 0x18c);
    at<U>(pf, 0x18c) = 0;
    at<U>(pf, 0x190) = 0x2bc;                                             // 700
    at<U>(pf, 0x1d4) = 0x47c35000u;                                       // 100000.0f
}
// 005CF060
void __fastcall w5_pathfind_reset_way_point_data_005cf060(void* pf, void*) {
    free_field(pf, 0x90);                                                 // the drive/walk target points
    free_field(pf, 0x98);
    at<U>(pf, 0x108) = 0;
    at<U>(pf, 0x8c) = 0; at<U>(pf, 0x90) = 0; at<U>(pf, 0x94) = 0; at<U>(pf, 0x98) = 0; at<U>(pf, 0x9c) = 0;
    at<U>(pf, 0xf8) = 0; at<U>(pf, 0x104) = 0;
    at<U>(pf, 0x10c) = 0; at<U>(pf, 0x110) = 0;
    at<U>(pf, 0x114) = 0x3f800000u; at<U>(pf, 0x118) = 0; at<U>(pf, 0x11c) = 0;   // (1, 0, 0)
}
// 005CEB70
void __fastcall w5_pathfind_delete_first_tile_alternatives_005ceb70(void* pf, void*) {
    if (at<void*>(pf, 0xf0) == nullptr) return;
    for (I k = 0; k < at<I>(pf, 0xe4); ++k) {
        void* p = at<void**>(pf, 0xf0)[k];
        if (p != nullptr) { live_free(p); at<void**>(pf, 0xf0)[k] = nullptr; }
    }
    if (at<void*>(pf, 0xe8) != nullptr) { live_free(at<void*>(pf, 0xe8)); at<U>(pf, 0xe8) = 0; }
    if (at<void*>(pf, 0xec) != nullptr) { live_free(at<void*>(pf, 0xec)); at<U>(pf, 0xec) = 0; }
    at<U>(pf, 0xe4) = 0;
    live_free(at<void*>(pf, 0xf0));                                       // free(+F0) without a null test (non-null here)
    at<U>(pf, 0xf0) = 0;
}
// 005D00A0
void __fastcall w5_pathfind_reset_inter_tile_search_005d00a0(void* pf, void*) {
    free_field(pf, 0xc4); free_field(pf, 0xc8); free_field(pf, 0xd0); free_field(pf, 0xd4);
    const bool alternatives = at<U>(pf, 0xf0) != 0;                       // 005D00F2 (tested before the clears)
    at<U>(pf, 0xc0) = 0; at<U>(pf, 0xc4) = 0; at<U>(pf, 0xc8) = 0; at<U>(pf, 0xcc) = 0; at<U>(pf, 0xd0) = 0; at<U>(pf, 0xd4) = 0;
    if (alternatives) w5_pathfind_delete_first_tile_alternatives_005ceb70(pf, nullptr);
    at<U>(pf, 0xd8) = 0; at<U>(pf, 0xdc) = 0; at<U>(pf, 0xe0) = 0;
    free_field(pf, 0xf4);
    at<U>(pf, 0xf4) = 0;
}
// 005CF130 (unlabelled; search-result reset)
void __fastcall w5_pathfind_reset_search_result_005cf130(void* pf, void*) {
    zero3(pf, 0x1b8); zero3(pf, 0x1c4);
    at<U>(pf, 0x1d0) = 0; at<U>(pf, 0x1dc) = 0;
    free_field(pf, 0x1d8);
    at<U>(pf, 0x1d8) = 0;
    at<U>(pf, 0x20c) = 0x442f0000u;                                       // 700.0f
    at<U>(pf, 0x1d4) = 0x7f7fffffu;                                       // FLT_MAX
}
// 005D0E00
void __fastcall w5_pathfind_reset_inter_area_search_005d0e00(void* pf, void*) {
    free_field(pf, 0xac); free_field(pf, 0xb0);
    at<U>(pf, 0xa0) = 0; at<U>(pf, 0xa4) = 0; at<U>(pf, 0xa8) = 0; at<U>(pf, 0xac) = 0; at<U>(pf, 0xb0) = 0;
    w5_pathfind_reset_grid_search_005cefa0(pf, nullptr);
    w5_pathfind_reset_way_point_data_005cf060(pf, nullptr);
    w5_pathfind_reset_inter_tile_search_005d00a0(pf, nullptr);
    w5_pathfind_reset_search_result_005cf130(pf, nullptr);               // tail jump 005D0E63
}
// 005D0EC0
void __fastcall w5_pathfind_reset_at_end_of_path_005d0ec0(void* pf, void*) {
    const void* buffer = at<void*>(pf, 0xb8);                             // tested before the clears
    at<U>(pf, 0x194) = 0;
    for (U off : {0x40u, 0x44u, 0x48u, 0x4cu, 0x54u, 0x58u, 0x5cu, 0x60u, 0x64u, 0x68u, 0x70u, 0x74u, 0x78u, 0x7cu, 0x80u}) at<U>(pf, off) = 0;
    if (buffer != nullptr) live_free(const_cast<void*>(buffer));
    at<U>(pf, 0x84) = 0;
    at<U>(pf, 0x1c) = 0; at<U>(pf, 0x20) = 0; at<U>(pf, 0x24) = 0;
    at<U>(pf, 0xb4) = 0; at<U>(pf, 0xb8) = 0;
    at<U>(pf, 0x88) = 1;
    w5_pathfind_reset_inter_area_search_005d0e00(pf, nullptr);            // tail jump 005D0F38
}
}
