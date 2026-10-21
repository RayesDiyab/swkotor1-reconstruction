// Sole definitions of options_video::{options,video,texture}_bindings (D1 epoch of the
// task28 options-video headers: lane-config d1/new + lane-menu-tables delta).
// Typed constant data; ports below are exact-ABI D1 functions over the canonical
// owners. options_video::String/Ini/ResourceRef are the task28 8/4/16-byte views of
// the same storage: the String/Ini/Ref ports below create the canonical object in
// that storage (placement) and later calls reach it through std::launder.
#include "67a3cbeb44-options.h"
#include "6b902ac777-video.h"
#include "866a226af7-texture.h"
#include "b01b98864e-video_mode_settings.h"   // lane-config bodies (005F0CE0, 0061B710, 0070D220, 0070D0A0, 005F1830)
#include "9ea344dff0-d1_bodies.h"
#include "c534281817-startup_string.h"
#include "9e32108632-base_ini.h"              // base_ini_reconstruction::PublicIni
#include "66a5366142-menu_targets.h"          // kotor_menu20::String::as_int_005e5630
#include "c30aaa70eb-resources.h"             // menu_resources::Ref
#include "689940810d-string_priority.h"       // GuiConcat005e5d10
#include "369787b919-window_shared_cells.h"
#include "7b8c81fcff-allocator_live.h"
#include <windows.h>
#include <new>
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*,const char*,...); }
extern "C" void* volatile kotor_cell_rules_007a3a28;                                  // lane-prepare cell owner
extern "C" menu_resources::Word __cdecl aurora_set_texture_pack_0070d800(std::uint8_t,menu_resources::String*); // lane-resman
namespace menu_tables_required {                                                      // REQUIRED (no M41 body)
options_video::Table* __fastcall C2daConstruct_00413cc0(options_video::Table*,void*,options_video::ResourceRef,options_video::I32);
options_video::I32 __fastcall C2daLoad_004143b0(options_video::Table*,void*);
void __fastcall C2daUnload_004139e0(options_video::Table*,void*);
options_video::I32 __fastcall C2daStringEntry_00413270(options_video::Table*,void*,options_video::I32,options_video::String*,options_video::String*);
options_video::I32 __fastcall C2daFloatEntry_00413430(options_video::Table*,void*,options_video::I32,options_video::String*,float*);
options_video::I32 __fastcall C2daIntEntry_00413660(options_video::Table*,void*,options_video::I32,options_video::String*,options_video::I32*);
void __stdcall TextureResetAll_00421d50();
options_video::U8 __cdecl TextureSetMem_00421d90(options_video::I32);
}
namespace options_video {
namespace {
using Canonical=kotor_reconstruction::StartupString;
inline Canonical* C(String* s){return std::launder(static_cast<Canonical*>(static_cast<void*>(s)));}
inline base_ini_reconstruction::PublicIni* P(Ini* i){return std::launder(static_cast<base_ini_reconstruction::PublicIni*>(static_cast<void*>(i)));}
static_assert(sizeof(String)==sizeof(Canonical)&&sizeof(Ini)==sizeof(base_ini_reconstruction::PublicIni)&&sizeof(ResourceRef)==sizeof(menu_resources::Ref),"same storage extents");
// 005B3190 CExoString() / 005E5A90 CExoString(const char*) / 005E5C20 ~CExoString: EAX=this.
String* __fastcall DefaultString_005b3190(String* s,void*){::new(static_cast<void*>(s)) Canonical();return s;}
String* __fastcall TextString_005e5a90(String* s,void*,const char* text){::new(static_cast<void*>(s)) Canonical(text);return s;}
void __fastcall DestroyString_005e5c20(String* s,void*){C(s)->~Canonical();}
// 005E6750 / 005E67E0 / 005E67C0 / 005E67D0 CExoIni public entries (task-37 PublicIni).
Ini* __fastcall ConstructIni_005e6750(Ini* i,void*){(void)(::new(static_cast<void*>(i)) base_ini_reconstruction::PublicIni)->construct_005e6750();return i;}
void __fastcall DestroyIni_005e67e0(Ini* i,void*){P(i)->destroy_005e67e0();}
I32 __fastcall ReadIni_005e67c0(Ini* i,void*,String* value,String* file,String* section,String* key){return P(i)->read_005e67c0(C(value),C(file),C(section),C(key));}
I32 __fastcall WriteIni_005e67d0(Ini* i,void*,String* value,String* file,String* section,String* key){return P(i)->write_005e67d0(C(value),C(file),C(section),C(key));}
// 005E5630 CExoString::AsINT (task19 owner receiver view kotor_menu20::String, same 8 bytes).
I32 __fastcall AsInt_005e5630(String* s,void*){return std::launder(static_cast<kotor_menu20::String*>(static_cast<void*>(s)))->as_int_005e5630();}
// 005E5680 CExoString::Format: identical cdecl variadic frame; tail jump keeps the caller's varargs.
__declspec(naked) void __cdecl Format_005e5680(String*,const char*,...){__asm jmp kotor_reconstruction::StartupFormat005e5680}
// 005E5D10 CExoString::operator+ (this, hidden out, rhs) -> EAX = out.
String* __fastcall Concat_005e5d10(String* self,void*,String* out,String* rhs){(void)GuiConcat005e5d10(C(self),nullptr,C(out),C(rhs));return out;}
// 00406D80 CResRef(const char*) on the 16-byte view.
ResourceRef* __fastcall ResourceRef_00406d80(ResourceRef* r,void*,const char* text){(void)(::new(static_cast<void*>(r)) menu_resources::Ref)->construct_00406d80(text);return r;}
// 0070D800 CAuroraInterface::SetTexturePack (lane-resman cdecl body) on the String view.
U32 __cdecl SetTexturePack_0070d800(U8 index,String* name){return aurora_set_texture_pack_0070d800(index,C(name));}
// 0073D3E4 EnumDisplaySettingsA (original IAT; 156-byte DEVMODEA view).
I32 __stdcall EnumDisplaySettings_0073d3e4(const char* device,U32 mode,DisplayMode* out){
    static_assert(sizeof(DisplayMode)==sizeof(DEVMODEA),"156-byte DEVMODEA");
    return ::EnumDisplaySettingsA(device,mode,std::launder(static_cast<DEVMODEA*>(static_cast<void*>(out))));
}
// Original literals (pushed as immediates by the consumers' original instructions).
const char tooltip_key_0074e880[]="TooltipDelay Sec";const char game_category_0073d658[]="Game Options";
const char ini_file_0073d648[]=".\\swkotor.ini";const char one_00741e1c[]="1";const char hardware_key_0074e86c[]="EnableHardwareMouse";
const char graphics_category_0073d6ac[]="Graphics Options";const char integer_format_0073d720[]="%d";
const char table_resource_0074e860[]="texpacks";const char texture_column_0074e858[]="Texture";const char gui_column_0074e854[]="Gui";
const char ratio_column_0074e848[]="DynMemRatio";const char memory_column_0074e844[]="Mem";const char pack_prefix_0074e834[]="TEXTUREPACKS:";
}
// 007A3C54 movie flag: .bss 0 (written only on the forMovie branch of 005F5AB0). REQUIRED canonical cell.
}
namespace kotor_menu_window_cells { extern std::uint32_t movie_flag_007a3c54; }
namespace options_video {
namespace {
OptionsBindings options_storage={
    &DefaultString_005b3190,&TextString_005e5a90,&DestroyString_005e5c20,
    &ConstructIni_005e6750,&DestroyIni_005e67e0,&ReadIni_005e67c0,&WriteIni_005e67d0,
    &AsInt_005e5630,&Format_005e5680,&menu_tables_m41::SetEnableHardwareMouseCursor_0061d670,
    tooltip_key_0074e880,game_category_0073d658,ini_file_0073d648,one_00741e1c,
    hardware_key_0074e86c,graphics_category_0073d6ac,integer_format_0073d720};
VideoBindings video_storage={
    &DefaultString_005b3190,&DestroyString_005e5c20,&ConstructIni_005e6750,&DestroyIni_005e67e0,
    &ReadVideoModeSettings_005f0ce0,&SetFullScreenEnabled_0061b710,          // lane-config
    &SetDisplayDevMode_0070d220,&GetNearestVideoMode_0070d0a0,               // lane-config
    &EnumDisplaySettings_0073d3e4,&SetVideoMode_005f1830,                     // lane-config 005F1830
    &kotor_menu_window_cells::movie_flag_007a3c54,
    nullptr,                                     // saved_width_0078d1d4: runtime attach (WinMain Views width_78d1d4)
    &kotor_menu_window_cells::height_0078d1d8,   // canonical U32 volatile cell
    nullptr};                                    // saved_depth_0078d1e0: runtime attach (WinMain Views depth_78d1e0)
TextureBindings texture_storage={
    &DefaultString_005b3190,&TextString_005e5a90,&DestroyString_005e5c20,
    &kotor_cell_rules_007a3a28,&live_operator_new,&ResourceRef_00406d80,
    &menu_tables_required::C2daConstruct_00413cc0,&menu_tables_required::C2daLoad_004143b0,&menu_tables_required::C2daUnload_004139e0,
    &menu_tables_required::C2daStringEntry_00413270,&menu_tables_required::C2daFloatEntry_00413430,&menu_tables_required::C2daIntEntry_00413660,
    &menu_tables_required::TextureResetAll_00421d50,&Concat_005e5d10,&SetTexturePack_0070d800,
    &menu_tables_m41::SetDynamicMemoryRatio_0041e9b0,&menu_tables_required::TextureSetMem_00421d90,
    table_resource_0074e860,texture_column_0074e858,gui_column_0074e854,ratio_column_0074e848,memory_column_0074e844,pack_prefix_0074e834};
}
OptionsBindings& options_bindings=options_storage;
VideoBindings& video_bindings=video_storage;
TextureBindings& texture_bindings=texture_storage;
// Runtime attach for the two words that live in the WinMain Views object.
// Returns false on a conflicting prior binding; detach restores null.
bool attach_saved_video_mode(U32* width_78d1d4,U8* depth_78d1e0) noexcept{
    if(!width_78d1d4||!depth_78d1e0)return false;
    if((video_storage.saved_width_0078d1d4&&video_storage.saved_width_0078d1d4!=width_78d1d4)||
       (video_storage.saved_depth_0078d1e0&&video_storage.saved_depth_0078d1e0!=depth_78d1e0))return false;
    video_storage.saved_width_0078d1d4=width_78d1d4;video_storage.saved_depth_0078d1e0=depth_78d1e0;return true;
}
void detach_saved_video_mode() noexcept{video_storage.saved_width_0078d1d4=nullptr;video_storage.saved_depth_0078d1e0=nullptr;}
}
