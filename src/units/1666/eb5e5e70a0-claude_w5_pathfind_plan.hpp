#pragma once
// W5 lane "pathfind-plan": server path PLANNING half of CSWSCreature movement (CSWSModule::PlotPath 004C6F70 and
// everything under it). Shared declarations for claude_w5_pathfind_plan.cpp / _plan2.cpp / _plan3.cpp.
// Conventions (w5_types.hpp): original thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit
// precision -> double intermediates, float exactly where the original stores a dword; ST0 float returns are
// exported as double; memory only through live_operator_new / live_free (006FA7E6 / 006FA390).
#include "842d6b1618-w5_t3.hpp"
#include "4f622be2f2-w5_area_direct_line.hpp"
#include "ab3fc15780-w5_drive.hpp"
#include "73b9db80fe-w5_pathfind_reset.hpp"
#include "e5e3766b7c-w5_subarea.hpp"

namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }  // [0068] CExoBase*

extern "C" {
using w5::U; using w5::I; using w5::Vector;

// ---------------- existing providers not declared by the W5 headers (exact C-linkage names, c05 map) ----------
void* __fastcall kotor_fastcall_004ae780(void* server, void*, U id);                     // GetAreaByGameObjectID [0530]
void* __fastcall recovered_object_array_004aed70(void* server, void*);                    // GetObjectArray [w2_party_native]
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                // GetServerAIMaster [creature_destructor_r2]
std::uint64_t __fastcall recovered_bridge_exo_timers_high_resolution_005e9260(void* timers, void*); // CExoTimers::GetHighResolutionTimer [1204-21 w3]
unsigned __fastcall recovered_module_get_waypoint_004c6e00(void* module, void*, const void* tag); // CSWSModule::GetWaypoint [vm_actions_t4]
void* __fastcall w5_door_get_linked_object_00589580(void* door, void*);                   // CSWSDoor::GetLinkedObject [1206-14 w5 t2]
Vector* __cdecl float_times_vector_00437a60(Vector* out, float scale, const Vector* v);   // operator*(float, Vector) [0555]
void __fastcall kotor_m15e_slot008_void8_005b5e90(void* self, void*, unsigned a, unsigned b); // 005B5E90 `ret 8` (debug print sink) [0832]
U __cdecl aur_add_drawable_square_0044efb0(const float* corners12, U color);              // AurAddDrawableSquare [0531]
void __cdecl aur_remove_drawable_square_0044f050(U handle);                               // AurRemoveDrawableSquare [0531]
void __cdecl unresolved_ftol2_006fae8c();                                                  // __ftol2 ST0 -> EDX:EAX [0064]
extern unsigned recovered_debug_cell_00832838;                                             // W1 .bss cell (path debug flag) [w1_native_b7]
void __fastcall kotor_fastcall_005d0e70(void* pf, void*);                                // ResetInterAreaSearchData (w5-movement helper)
void __fastcall kotor_fastcall_004c3ea0(void* module, void*, void* pf);                   // SetIntraAreaGoal (w5-movement helper)

// ---------------- storage owned by this lane (no other owner in the c05 map; original initial values) ---------
extern U w5_path_debug_draw_008327cc;          // .bss 0   : path-point search debug squares enabled
extern U w5_path_debug_reset_007a1b20;         // .data 1  : clear the square table on the next debug search
extern U w5_path_debug_creature_00832924;      // .bss 0   : creature id whose search is drawn (written by 005F9Dxx)
extern U w5_path_debug_squares_00831ff0[500];  // .bss 0   : AurAddDrawableSquare handles (filled with -1)
extern U w5_path_debug_square_count_008327d0;  // .bss 0
extern I w5_path_smooth_budget_007a1b1c;       // .data 10000 : restartable smoothing time slice (timer units)
extern U w5_path_smooth_stat_00832870;         // .bss 0   : cleared after every restartable smoothing line test
extern U w5_path_smooth_stat_00832840;         // .bss 0

// ---------------- exports of this lane ----------------
// module layer (claude_w5_pathfind_plan.cpp)
U __fastcall kotor_fastcall_004c6f70(void* module, void*, void* pf, U budget);            // CSWSModule::PlotPath RET 8
U __fastcall kotor_fastcall_004c6e20(void* module, void*, void* pf, U budget);            // PlotInterAreaPath RET 8
U __fastcall kotor_fastcall_004c6bd0(void* module, void*, void* pf);                      // ComputeInterAreaPath RET 4
U __fastcall kotor_fastcall_004c5510(void* module, void*, I depth, I max_depth, void* pf);// InterAreaDFS RET 0Ch
I __fastcall kotor_fastcall_004c54a0(void* module, void*, U unused, void* pf, U** out);   // GenerateInterAreaDFSSuccessors RET 0Ch
I __fastcall kotor_fastcall_004bc400(void* area, void*, U* out, I count, const U* list);  // CountAreaTransitionTriggers RET 0Ch
U __fastcall kotor_fastcall_0058c830(void* trigger, void*);                               // CSWSTrigger::GetTargetArea RET 0 (IMPORT: pathfind-walk owns it)
U __fastcall kotor_fastcall_004c3260(void* module, void*, void* pf, U budget);            // PlotPathInArea RET 8
double __fastcall kotor_fastcall_005111c0(void* creature, void*);                         // ComputeTotalWalkDistance, ST0
void __fastcall kotor_fastcall_005cec10(void* pf, void*);                                 // FlipStartEndPoints
void __fastcall kotor_fastcall_005cec50(void* pf, void*);                                 // FlipTempEndPoints
void __fastcall kotor_fastcall_004bb340(void* pf, void*, void* path);                     // set +1AC path index buffer RET 4
void __fastcall kotor_fastcall_004bb380(void* pf, void*, Vector* start, Vector* goal);    // load +1E4/+1F0 RET 8
void __fastcall kotor_fastcall_004bb3c0(void* pf, void*, const Vector* start, const Vector* goal); // save +1E4/+1F0 RET 8
void __fastcall kotor_fastcall_004bb400(void* pf, void*, I count, void* points);          // start segment +1FC/+200 RET 8
void __fastcall kotor_fastcall_004bb440(void* pf, void*, I count, void* points);          // end segment +204/+208 RET 8
void __fastcall kotor_fastcall_004bb480(void* pf, void*, void* points);                   // +21C (count +218) RET 4
void __fastcall kotor_fastcall_004bb4d0(void* pf, void*, void* points);                   // +228 (count +220) RET 4
void __fastcall kotor_fastcall_004bb520(void* pf, void*, void* points);                   // +22C (count +224) RET 4
double __fastcall kotor_fastcall_004bb860(void* area, void*, I count, const float* points); // ComputePathDistance RET 8, ST0
void __fastcall kotor_fastcall_004bbc50(void* area, void*);                               // GridDFSTransTableInitialize
U __fastcall kotor_fastcall_004bbcc0(void* area, void*, I x, I y, I* g, I* limit);        // GridDFSTransTableGet RET 10h
void __fastcall kotor_fastcall_004bbd80(void* area, void*, I x, I y, I g, I limit);       // GridDFSTransTablePut RET 10h
void __fastcall kotor_fastcall_004bbeb0(void* area, void*);                               // PathPointDFSTransTableInitialize
U __fastcall kotor_fastcall_004bbf20(void* area, void*, float x, float y, float* g, float* limit); // PathPointDFSTransTableGet RET 10h
void __fastcall kotor_fastcall_004bc010(void* area, void*, float x, float y, float g, float limit); // PathPointDFSTransTablePut RET 10h
// grid and path-point search (claude_w5_pathfind_plan2.cpp)
U __fastcall kotor_fastcall_004c1a00(void* area, void*, void* pf, U budget_lo, U budget_hi); // PlotGridPath RET 0Ch
U __fastcall kotor_fastcall_004c0930(void* area, void*, I depth, I g, I limit, I x, I y, I z); // GridDFSearch RET 18h
I __fastcall kotor_fastcall_004bcee0(void* area, void*, I g, I limit, I x, I y, I z, I** out); // GridDFSGenerateSuccessors RET 18h
U __fastcall kotor_fastcall_004c2200(void* area, void*, void* pf, U budget_lo, U budget_hi); // PlotPathPointPath RET 0Ch
U __fastcall kotor_fastcall_004c0b30(void* area, void*, I depth, float g, float limit, I node, I goal); // PathPointDFSearch RET 14h
I __fastcall kotor_fastcall_004bdb00(void* area, void*, float g, I limit, I node, void** out); // PathPointDFSGenerateSuccessors RET 10h
U __fastcall kotor_fastcall_004bd770(void* area, void*, const Vector* point, I* out);    // ClosestPathPoint RET 8
// smoothing (claude_w5_pathfind_plan3.cpp)
U __fastcall kotor_fastcall_004c1670(void* area, void*);                                  // SmoothPointsOnPath RET 0
U __fastcall kotor_fastcall_004c17a0(void* area, void*);                                  // SmoothPointsOnPathRestartable RET 0
void __fastcall kotor_fastcall_004c15f0(void* area, void*, I count, float* points, I* out_count, float** out_points); // SmoothSelection RET 10h
U __fastcall kotor_fastcall_004c13e0(void* area, void*, I count, float* points, I* out_count, float** out_points); // SmoothSelectionRestartable RET 10h
void __fastcall kotor_fastcall_004c0180(void* area, void*, I count, const float* points, I* out_count, float** out_points,
                                        I subdivide, I look_ahead);                       // SmoothSelectNodes RET 18h
U __fastcall kotor_fastcall_004c04e0(void* area, void*, I count, const float* points, I* out_count, float** out_points,
                                     I subdivide, I look_ahead, float min_length);        // SmoothSelectNodesRestartable RET 1Ch
void __fastcall kotor_fastcall_004bfe20(void* area, void*, I count, const float* points, I* out_count, float** out_points); // SmoothCornerOptimize RET 10h
U __fastcall kotor_fastcall_004be3a0(void* area, void*, float x0, float y0, float x1, float y1, float x2, float y2,
                                     float radius, float height, float* p_x, float* p_y, float* q_x, float* q_y); // ComputeBestCorner RET 30h
}

namespace w5pp {
using namespace w5;
constexpr U k_in_progress_00745dbc = 1;   // .rdata 00745DBC
constexpr U k_complete_00745dc0 = 2;      // .rdata 00745DC0
constexpr U k_failed_00745dc4 = 3;        // .rdata 00745DC4
constexpr unsigned char k_found_0074666c = 0;     // .rdata 0074666C (byte): GetGameObject status "found"
constexpr unsigned char k_type_trigger_00746636 = 7;  // .rdata 00746636 (byte)
constexpr unsigned char k_type_door_00746639 = 10;    // .rdata 00746639 (byte)
constexpr unsigned char k_type_waypoint_0074663b = 12;// .rdata 0074663B (byte)
// path-point search states (CPathfindInformation +1E0) .rdata 0074C42C..0074C43C, smoothing states +214
constexpr U k_pp_init_0074c42c = 1, k_pp_verify_start_0074c430 = 2, k_pp_verify_end_0074c434 = 3,
            k_pp_search_0074c438 = 4, k_pp_assemble_0074c43c = 5;
constexpr U k_assemble_pending_0074c440 = 0, k_assemble_done_0074c444 = 1;
constexpr U k_smooth_start_0074c444 = 1, k_smooth_pass1_0074c448 = 2, k_smooth_pass2_0074c44c = 3, k_smooth_pass3_0074c450 = 4;

inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
template<class T> inline T* ptr(U v) { return reinterpret_cast<T*>(static_cast<std::uintptr_t>(v)); }
inline void* pf_of(void* area) { return at<void*>(area, 0x1ac); }
// x87 sqrt and __ftol2 exactly as the original executes them (ST0 in/out)
inline double x87_fsqrt(double v) { double r; __asm { fld qword ptr v } __asm { fsqrt } __asm { fstp qword ptr r } return r; }
inline I ftol2(double v) { U r; __asm { fld qword ptr v } __asm { call unresolved_ftol2_006fae8c } __asm { mov r, eax } return static_cast<I>(r); }
// CGameObjectArray::GetGameObject(id, &out) on the array returned by CServerExoApp::GetObjectArray; true = found
inline bool get_object(void* array, U id, void** out) {
    return static_cast<unsigned char>(kotor_fastcall_004d8230(array, nullptr, id, out)) == k_found_0074666c;
}
inline void* object_array_real() { return recovered_object_array_004aed70(server_app(), nullptr); }
// CExoTimers::GetHighResolutionTimer on [[007A39E0]+4]
inline std::uint64_t hr_timer() {
    return recovered_bridge_exo_timers_high_resolution_005e9260(at<void*>(kotor_menu_10_19::dependencies::base_007a39e0, 4), nullptr);
}
inline void* creature_by_id(U id) { return kotor_fastcall_004ae770(server_app(), nullptr, id); }
inline void debug_print(void* self, U level, const char* text) {  // 005B5E90 (`ret 8`): the original logging sink
    kotor_m15e_slot008_void8_005b5e90(self, nullptr, level, addr(text));
}
inline I test_line(void* area, const Vector* from, const Vector* to, const float* radius, float height, U flag5, void* hit) {
    return w5_area_test_direct_line_004bcb70(area, nullptr, from, to, radius, height, flag5, hit);
}
}  // namespace w5pp
