#pragma once
namespace kotor_scene3d {struct Vector;struct Quaternion;}
extern "C" void __fastcall vector_normalize_004ab130(kotor_scene3d::Vector*,void*);
extern "C" float __cdecl kotor_m15a_yaw_004aa0f0(const void*);
extern "C" kotor_scene3d::Quaternion* __cdecl yaw_pitch_roll_004acac0(kotor_scene3d::Quaternion*,float,float,float);
extern "C" kotor_scene3d::Quaternion* __cdecl quaternion_negate_w_004a9b50(kotor_scene3d::Quaternion*,const kotor_scene3d::Quaternion*);
extern "C" kotor_scene3d::Vector* __cdecl quaternion_rotate_004a9bb0(kotor_scene3d::Vector*,const kotor_scene3d::Quaternion*,const kotor_scene3d::Vector*);
extern "C" void __fastcall recovered_trigger_creator_0058d700(void*,void*,unsigned);
extern "C" void* __fastcall server_get_module_004ae6b0(void*,void*);
namespace kotor_reconstruction {struct ArrayMutations;}
extern "C" void __fastcall gui_load_array_add_00671c00(kotor_reconstruction::ArrayMutations*,void*,unsigned);
