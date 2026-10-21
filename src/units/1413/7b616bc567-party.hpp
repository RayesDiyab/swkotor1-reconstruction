#pragma once
// Lane W4 party/selection fork: provider declarations (exact runtime1115 map symbols) and W4 definitions.
#include "27e3c46f56-w4_frame.hpp"

namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }   // CExoString::CExoString()
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); }       // CServerExoApp::GetPartyTable
namespace kotor_scene3d { struct Vector; }
namespace kotor_world_los { extern kotor_scene3d::Vector up_007a19b0; }                                 // [007A19B0] (0,0,1)

extern "C" {
extern void* kotor_cell_rules_007a3a28;                                                                  // [007A3A28] CSWRules*
void __fastcall frame_LogStringCtor_005e5a90(void* self, void*, const char* text);                     // CExoString::CExoString(char const*)
void __cdecl frame_LogFormat_005e5680(void* self, const char* format, ...);                            // CExoString::Format
int __fastcall c2da_current_get_float_00413430(void* table, void*, int row, void* column, float* out);  // C2DA::GetFLOATEntry
void __fastcall frame_R4_005d0fc0(void* vm, void*, unsigned* script, unsigned object, unsigned flag);  // CVirtualMachine::RunScript
void* __fastcall kotor_fastcall_004ae770(void* server, void*, unsigned id);                             // CServerExoApp::GetCreatureByGameObjectID
W4Vector* __cdecl vector_normalized_004ab180(W4Vector* out, const W4Vector* v);                         // normalize
double __cdecl kotor_m15a_yaw_004aa0f0(const W4Vector* v);                                              // Yaw (naked; ST0 kept at PC53)
double __cdecl deg_to_rad_004a96d0(float degrees);                                                     // DegToRad (ST0 read as double)
void* __fastcall quaternion_from_axis_angle_004ac960(void* self, void*, W4Vector axis, float angle);   // Quaternion::Quaternion
W4Vector* __cdecl quaternion_rotate_004a9bb0(W4Vector* out, const void* q, const W4Vector* v);         // operator*(Quaternion, Vector)

// W4 party fork definitions.
void __fastcall frame_R4_00637050(void* party, void*, float delta);                                    // CSWParty::Update
int __fastcall kotor_fastcall_00634da0(void* follow, void*);                                           // FindFurthestPartyMemeber_Current
int __fastcall kotor_fastcall_00634d40(void* follow, void*);                                           // FindClosestPartyMemeber_Current
double __fastcall kotor_fastcall_00634e00(void* follow, void*, int index, int squared);                // PointOnLineDistanceFromLeader
void* __fastcall kotor_fastcall_00634cc0(void* follow, void*, int index);                              // GetPathData
int __fastcall kotor_fastcall_006350f0(void* follow, void*, float distance, W4Vector* out, std::uint32_t* out_index, int member); // GetPointBackFromStart
double __fastcall kotor_fastcall_00634a70(void* party, void*);                                         // CSWParty::GetFollowRange
void __fastcall kotor_fastcall_006359c0(void* party, void*);                                           // CSWParty::RecaulateFollowPoint
int __fastcall kotor_fastcall_00636840(void* party, void*);                                            // CSWParty::TestAndResetPath
void __fastcall kotor_fastcall_006346f0(void* party, void*);                                           // CSWParty::InvertFormation
}
namespace w4 { extern volatile U32 follow_formation_override_007a2414; }   // .data cell, never written by the original
