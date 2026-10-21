// W5 t2 foreign imports (w5-foreign closure of the A11 patched TU; base sha d759bd25... A11 patched source).
// Every former /alternatename trap fallback is closed:
//  * Imports whose name is already defined by a real linked provider (W4 1205-xx objects, w4-creature_ai, W3 t2,
//    0886 cohort, w5_smooth_path, client_clear_all_actions, 1207-00 glue) no longer carry an /alternatename or a
//    trap body; callers bind to the provider directly. /include makes the linker REQUIRE each provider, so dropping
//    one is a link error instead of a silent trap.
//  * 0050CA10 / 004C0FA0 had no definition under the W5 import name (the alias resolved to the trap in c11): the W5
//    import names are now strong typed forwarders to the W1 tier-9 bodies in w1_native_b7_0056cbf0.obj
//    (recovered_area_*), which share the original stack contract (RET 28h / RET 18h, same argument order).
#include "d8371efa32-w5_t2.hpp"
#include <cstdint>
#include <cstring>

#define W5_REQUIRE(sym) __pragma(comment(linker, "/include:" sym))
// W4 (original RET verified: 0060F0F0/006D10B0/0061D5E0/0061D610/0061D640/00634DA0/00641DC0/0063A360 RET 0,
// 00634CC0/006345E0/0063FDB0 RET 4, 00634E00 RET 8; 00670830/006707A0 cdecl)
W5_REQUIRE("@kotor_fastcall_0060f0f0@8")       // w4-creature_ai.obj               CSWCCreature::IsDriveAnimationBlocked
W5_REQUIRE("@kotor_fastcall_006d10b0@8")       // 1205-04-w4t1b_rk4.obj            CSWRK4SplitAcceleration ctor
W5_REQUIRE("_kotor_cdecl_00670830")            // 1205-04-w4t1b_rk4.obj            CSWMGPhysicsState operator*
W5_REQUIRE("_kotor_cdecl_006707a0")            // 1205-04-w4t1b_rk4.obj            CSWMGPhysicsState operator+
W5_REQUIRE("@kotor_fastcall_0061d5e0@8")       // 1205-05-w4t1b_update_camera.obj  GetCameraKeyboardDPS (ST0 qword)
W5_REQUIRE("@kotor_fastcall_0061d610@8")       // 1205-05-w4t1b_update_camera.obj  GetCameraKeyboardAcceleration
W5_REQUIRE("@kotor_fastcall_0061d640@8")       // 1205-05-w4t1b_update_camera.obj  GetCameraKeyboardDeceleration
W5_REQUIRE("@kotor_fastcall_00634cc0@12")      // 1205-33-w4t1b_party_update.obj   CSWPartyFollowDataArray::GetPathData
W5_REQUIRE("@kotor_fastcall_00634da0@8")       // 1205-33-w4t1b_party_update.obj   FindFurthestPartyMemeber_Current
W5_REQUIRE("@kotor_fastcall_00634e00@16")      // 1205-33-w4t1b_party_update.obj   PointOnLineDistanceFromLeader
W5_REQUIRE("@kotor_fastcall_00641dc0@8")       // combat_restore-changed.obj       CSWCModule::RestoreCamera
W5_REQUIRE("@kotor_fastcall_0063a360@8")       // 1205-01-w4t1b_camera_stick.obj   CSWCameraOnAStick::ClearCombatCamera
W5_REQUIRE("@kotor_fastcall_006345e0@12")      // src_hud_main_interface_interaction_status_removed.obj SetCombatMessage
W5_REQUIRE("@kotor_fastcall_0063fdb0@12")      // module_camera.obj                CSWCModule::AcclTiltCamera
// W3 (RET 8)
W5_REQUIRE("@recovered_client_creature_set_way_points_0060c790@16")   // 1204-10-w3t2b_w3_client_t2.obj SetWayPoints
// 0886 TU wrappers (004BE5E0 RET 8, 005B4880 RET 0)
W5_REQUIRE("@w2x_area_safe_point_004be5e0@16")           // 0886-cohort-patched.obj CSWSArea::TestSafeLocationPoint
W5_REQUIRE("@w5x_creature_is_debilitated_005b4880@8")    // 0886-cohort-patched.obj CSWSCreature::IsDebilitated
W5_REQUIRE("@kotor_fastcall_005b4880@8")                 // 0886-cohort-patched.obj (same binding, W4 import name)
// W5 later tiers / other owners
W5_REQUIRE("@w5_t2_party_follow_smooth_path_00635af0@8")              // w5_smooth_path.obj SmoothFollowPath (RET 0)
W5_REQUIRE("@w5_t2_client_object_clear_all_actions_0063d470@8")       // client_clear_all_actions.obj (RET 0)
W5_REQUIRE("@w5_t2_module_find_object_by_tag_ordinal_004c5fe0@16")    // 1207-00 glue -> vm_actions_t3 (RET 8)

extern "C" {
// ---- 0050CA10 CSWSArea::NoNonWalkPolysInDoors: thiscall, 10 stack args, RET 28h ----
// W1 tier-9 body (w1::safe_location::no_nonwalk_polys_in_doors_0050ca10 via its C export). Argument 9 (f8) is the
// CExoArrayList* ignore list (W5 caller BumpFriends passes 0, as the original 004F6570..74 pushes 0,1,0).
unsigned __fastcall recovered_area_no_nonwalk_polys_in_doors_0050ca10(void* self, void*, void* a, void* b, float zlo,
    float zhi, float radius, unsigned object_id, unsigned* out_id, void* list, unsigned flag9, unsigned flag10);
U __fastcall w5_t2_area_no_nonwalk_polys_in_doors_0050ca10(void* area, void*, const Vector* a, const Vector* b,
    float lo, float hi, float radius, U object_id, U* out_id, U f8, U f9, U f10) {
    return recovered_area_no_nonwalk_polys_in_doors_0050ca10(area, nullptr, const_cast<Vector*>(a), const_cast<Vector*>(b),
        lo, hi, radius, object_id, out_id, reinterpret_cast<void*>(static_cast<std::uintptr_t>(f8)), f9, f10);
}
// ---- 004C0FA0 CSWSArea::ComputeSafeLocationInDirection: thiscall, 6 stack args, RET 18h ----
// Arg 4 is the creature's pathfind-size record (+4 step radius, +0x14 height), stored to area+0x1AC by the body.
unsigned __fastcall recovered_area_compute_safe_location_in_direction_004c0fa0(void* self, void*, void* start, void* dir,
    float max_dist, void* creature, unsigned check_doors, void* out);
U __fastcall w5_t2_area_compute_safe_location_in_direction_004c0fa0(void* area, void*, const Vector* p,
    const Vector* dir, float distance, void* pathfind, U flag, Vector* out) {
    return recovered_area_compute_safe_location_in_direction_004c0fa0(area, nullptr, const_cast<Vector*>(p),
        const_cast<Vector*>(dir), distance, pathfind, flag, out);
}
// 00610950: reuse the selected W1 real donor @0045DCB0 (RET 8).
void __fastcall recovered_client_creature_set_lock_orientation_00610950(void*, void*, U, U);
void __fastcall w5_t2_client_creature_set_lock_orientation_00610950(void* creature, void*, U target, U flag) {
    recovered_client_creature_set_lock_orientation_00610950(creature, nullptr, target, flag);
}
// 0060F140: exact W4 donor setter; original [cc+13Ch] = state, RET 4.
void __fastcall w5_t2_client_creature_set_engaged_state_0060f140(void* creature, void*, U state) {
    std::memcpy(static_cast<unsigned char*>(creature) + 0x13C, &state, 4);
}
}
