#pragma once
#include "3c5e449ed2-door_methods.h"
#include "cca80f7c35-m15e.h"
#include "cc436f6baa-m15e_animbase.h"
#include "36a720fede-scene3d.h"
#include "07d4e464a9-vtables_gen.h"
#include "b0e1e980f2-m15_l0.h"
#include "916798d1e4-resources.h"
#include "c33c04e339-r8_copy_join.h"
#include "97e489d1ec-table_contract.h"
#include "0b7d27aef1-w3_animation.h"
#include "3ae3c3deb8-area_region_glue.h"
#include <cstring>
#include <cstddef>
namespace menu_continuation {struct AppManager;struct WorldTimer;extern AppManager* app_manager_007a39fc;}
namespace c2da_current {struct Table;}
extern "C" {
extern const void* const recovered_client_door_methods_00753960[82];
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
void __fastcall kotor_m15e_object_destruct_0063e800(void*,void*);
menu_continuation::WorldTimer* __fastcall frame_GetWorldTimer_005ed4f0(void*,void*);
void __fastcall frame_GetWorldTime_004ade40(menu_continuation::WorldTimer*,void*,std::uint32_t*,std::uint32_t*);
std::uint32_t __fastcall recovered_world_time_difference_004ae460(void*,void*,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t*,std::uint32_t*);
void* __fastcall client_get_player_creature_005ed540(void*,void*);
void* __fastcall kotor_fastcall_005ed560(void*,void*);
void* __fastcall kotor_fastcall_005ed570(void*,void*);
std::uint32_t __fastcall frame_R4_005ed690(void*,void*);
void* __fastcall kotor_m15e_client_get_client_ai_master_005ed6b0(void*,void*);
void __fastcall kotor_m15e_client_ai_list_remove_object_00604810(void*,void*,void*);
unsigned char __fastcall recovered_object_array_remove_004d81a0(void*,void*,unsigned,void**);
std::uint32_t __fastcall kotor_fastcall_0063df60(void*,void*);
void __fastcall kotor_fastcall_004015a0(void*,void*);
std::uint16_t __fastcall kotor_m15a_object_get_looping_animation_0063c100(void*,void*);
std::uint32_t __fastcall kotor_m15a_object_animation_looping_0063c300(void*,void*,std::uint32_t);
std::uint32_t __fastcall kotor_fastcall_0063c380(void*,void*,std::uint32_t);
std::uint32_t __fastcall kotor_fastcall_0063c610(void*,void*,std::uint16_t*,float*,std::uint32_t*,std::uint32_t*);
void __fastcall kotor_fastcall_0063c9c0(void*,void*);
std::int32_t __fastcall c2da_current_get_int_entry_00413510(c2da_current::Table*,void*,std::int32_t,std::int32_t,std::int32_t*);
std::int32_t __fastcall c2da_current_get_string_entry_00413190(c2da_current::Table*,void*,std::int32_t,std::int32_t,kotor_reconstruction::StartupString*);
menu_resources::Ref* __fastcall async_ref_construct_string_00406d60(menu_resources::Ref*,void*,kotor_reconstruction::StartupString*);
extern void* kotor_cell_rules_007a3a28;
extern std::uint32_t kotor_rules_cell32_007a224c,kotor_rules_cell32_007a2250;
extern std::uint32_t kotor_rules_cell32_007a2268,kotor_rules_cell32_007a226c,kotor_rules_cell32_007a2270;
extern void* kotor_cell_sound_007a39ec;
void* __fastcall frame_GetServerArea_00605640(void*,void*);
void __fastcall frame_R4_0062b020(void*,void*,unsigned,unsigned*);
float* __cdecl kotor_cdecl_00423bc0(float*,const float*,const float*);
kotor_scene3d::Vector* __fastcall quaternion_ydir_0044f420(const kotor_scene3d::Quaternion*,void*,kotor_scene3d::Vector*);
}
namespace door {
using U=std::uint32_t;using H=std::uint16_t;using B=std::uint8_t;using I=std::int32_t;
using V=kotor_scene3d::Vector;using Q=kotor_scene3d::Quaternion;
template<class T>T get(const void* p,U offset){T value;std::memcpy(&value,static_cast<const B*>(p)+offset,sizeof value);return value;}
template<class T>void put(void* p,U offset,T value){std::memcpy(static_cast<B*>(p)+offset,&value,sizeof value);}
inline void* at(void* p,U offset){return static_cast<B*>(p)+offset;}
inline void* ptr(const void* p,U offset){return get<void*>(p,offset);}
inline U word(const void* p){return static_cast<U>(reinterpret_cast<std::uintptr_t>(p));}
inline void* pointer(U value){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(value));}
inline void* client(){return ptr(menu_continuation::app_manager_007a39fc,4);}
inline const KotorM15ObjectMethods_007516c8* object_table(void* p){return get<const KotorM15ObjectMethods_007516c8*>(p,0);}
inline const KotorM15eAnimBaseVtbl* animation_table(void* p){return get<const KotorM15eAnimBaseVtbl*>(p,0);}
inline const aurora_model::GobVTable* gob_table(void* p){return get<const aurora_model::GobVTable*>(p,0);}
inline aurora_model::Gob* gob(void* p){return static_cast<aurora_model::Gob*>(p);}
inline V* vector(void* p,U offset=0){return reinterpret_cast<V*>(static_cast<B*>(p)+offset);}
inline void copy3(void* to,const void* from){for(U n=0;n!=12;n+=4)put<U>(to,n,get<U>(from,n));}
struct DeleteMethods {void*(__fastcall* deleting)(void*,void*,U);};
inline void deleting(void* p,U flags){(void)get<const DeleteMethods*>(p,0)->deleting(p,nullptr,flags);}
}

