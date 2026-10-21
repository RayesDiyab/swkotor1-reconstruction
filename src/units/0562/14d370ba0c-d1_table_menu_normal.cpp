// Sole definitions of menu_normal_40_49::providers (CGuiInGame 0062FED0 consumer) and
// menu_normal_retained_40_49::providers (music 005F6480 / module 00643F40 consumers),
// D1 header epoch. Typed constant data, no function-pointer casts.
// Runtime-attached slots keep null statics (their owners write them later):
//   string_construct/string_destroy/assign/cstr/not_equal -> task22_strings::attach (string_joins.cpp)
//   resource_007a39e8 -> task10_resource_cells::attach; manager_007a39fc -> kotor_base_gui_join::attach
// allocate/free are live_operator_new/live_free (string_joins.cpp AllocatorNotAttached check).
#include "065742b12f-normal.h"
#include "d9a418ba34-normal.h"
#include "9ea344dff0-d1_bodies.h"
#include "7b8c81fcff-allocator_live.h"
#include "c30aaa70eb-resources.h"
#include "604fee70b4-reference.h"             // unresolved_ref_assign_00406290 (M41 resource-ref-assignment)
#include "7d1d224ac9-async_load.h"            // async_ref_construct_string_00406d60 (M41 next-rim)
#include "66a5366142-menu_targets.h"          // kotor_menu20::ClientApp::get_options_005ed700
#include <new>
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*,const char*,...); }
extern "C" void* volatile kotor_cell_sound_007a39ec;                   // lane-prepare cell owner
extern "C" const std::uint32_t kotor_rdata_sound_mode_0074c5f8;        // lane-prepare .rdata owner (value 3)
extern "C" menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*,void*); // lane-m42-menu-joins adopter (D9)
namespace table29_required { void* __fastcall gui_game_object_004d7d60(void*,void*,std::uint32_t,std::uint32_t); } // task-24 reviewed donor
namespace menu_tables_required {   // REQUIRED, off first-frame path: music 005F6480 only (StartServices arg1=0 -> 005F8CC9 skips it)
void __fastcall FadeAndStop_005f1b20(void*,void*,void*,std::uint32_t);
void __fastcall StreamDestroy_005d6200(void*,void*);
void* __fastcall StreamConstruct_005d5a50(void*,void*);
void __fastcall StreamSetLooping_005d5b40(void*,void*,std::uint32_t);
void __fastcall StreamSetPriority_005d5ba0(void*,void*,std::uint32_t);
std::uint32_t __fastcall StreamPlay_005d5b10(void*,void*,menu_resources::Ref*,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t);
void __fastcall SoundSetMode_005d5e80(void*,void*,std::uint32_t);
std::uint32_t __fastcall StreamIsPlaying_005d5af0(void*,void*);
void __fastcall StreamResume_005d5b30(void*,void*);
}
namespace {
using Ref=menu_resources::Ref;using String=kotor_reconstruction::StartupString;
namespace R=menu_tables_required;namespace B=menu_tables_m41;namespace C=menu_tables_m41::cells;
// 00405ED0 CResRef(): adopter returns the receiver (EAX=this); starts the trivial Ref lifetime.
void* __fastcall RefDefault_00405ed0(void* p,void*){return c2da_default_ref_00405ed0(::new(p) Ref,nullptr);}
// 004085F0 CExoResMan cache (existing Manager member owner).
void* __fastcall Cache_004085f0(void* manager,void*,String* out,String* name,std::uint32_t type){
    return static_cast<menu_resources::Manager*>(manager)->cache_004085f0(out,name,type);}
// 00406D60 CResRef(CExoString const&) (existing next-rim transport).
void* __fastcall RefString_00406d60(Ref* r,void*,void* name){return async_ref_construct_string_00406d60(r,nullptr,static_cast<menu_resources::String*>(name));}
// 005ED700 CClientExoApp::GetClientOptions (existing task19 member owner).
void* __fastcall GetOptions_005ed700(void* app,void*){return static_cast<kotor_menu20::ClientApp*>(app)->get_options_005ed700();}
const char empty_0073d71c[]="";                           // 0073D71C
const char music_format_0074c304[]="HD0:STREAMMUSIC\\%s"; // 0074C304
}
namespace menu_normal_40_49 {
static_assert(sizeof(Providers)==160,"40 DWORD slots");
Providers providers={
 &R::FadeAndStop_005f1b20,&R::StreamDestroy_005d6200,&live_free,
 nullptr,nullptr,nullptr,                                  // not_equal / string_construct / cstr: task22 attach
 &kotor_reconstruction::StartupFormat005e5680,&Cache_004085f0,
 nullptr,                                                  // string_destroy: task22 attach
 &live_operator_new,&R::StreamConstruct_005d5a50,&R::StreamSetLooping_005d5b40,&R::StreamSetPriority_005d5ba0,
 &RefString_00406d60,&R::StreamPlay_005d5b10,&R::SoundSetMode_005d5e80,&R::StreamIsPlaying_005d5af0,&R::StreamResume_005d5b30,
 &table29_required::gui_game_object_004d7d60,&RefDefault_00405ed0,
 nullptr,                                                  // assign: task22 attach
 &B::ClientListConstruct_004b25d0,&B::StringListConstruct_0062b510,&B::VectorConstruct_006fb55f,
 &B::LiveContentConstruct_0059ea90,&B::DialogMessageConstruct_006285b0,&B::LiveContentDestroy_0059ea30,&B::DialogMessageDestroy_00628600,
 &GetOptions_005ed700,&B::SetCameraMode_0061b6f0,&unresolved_ref_assign_00406290,
 &kotor_cell_sound_007a39ec,nullptr,&kotor_rdata_sound_mode_0074c5f8, // resource_007a39e8: task10 attach
 nullptr,                                                  // manager_007a39fc: base_gui_join attach
 &C::module_type_00746632,&C::invalid_object_00746644,&C::module_word_0074410c,empty_0073d71c,music_format_0074c304};
}
namespace menu_normal_retained_40_49 {
static_assert(sizeof(Providers)==116,"29 DWORD slots");
Providers providers={
 &R::FadeAndStop_005f1b20,&R::StreamDestroy_005d6200,&live_free,
 nullptr,nullptr,nullptr,
 &kotor_reconstruction::StartupFormat005e5680,&Cache_004085f0,
 nullptr,
 &live_operator_new,&R::StreamConstruct_005d5a50,&R::StreamSetLooping_005d5b40,&R::StreamSetPriority_005d5ba0,
 &RefString_00406d60,&R::StreamPlay_005d5b10,&R::SoundSetMode_005d5e80,&R::StreamIsPlaying_005d5af0,&R::StreamResume_005d5b30,
 &table29_required::gui_game_object_004d7d60,&RefDefault_00405ed0,
 nullptr,
 &kotor_cell_sound_007a39ec,nullptr,&kotor_rdata_sound_mode_0074c5f8,
 &C::module_type_00746632,&C::invalid_object_00746644,&C::module_word_0074410c,empty_0073d71c,music_format_0074c304};
}
