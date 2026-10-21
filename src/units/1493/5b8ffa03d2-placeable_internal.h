#pragma once
#include "b596b10d93-placeable_methods.h"
#include "983aa0b277-startup_string.h"
#include <cstring>
using U=std::uint32_t;
using I=std::int32_t;
using B=std::uint8_t;
using String=kotor_reconstruction::StartupString;
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;}
namespace placeable_private {
inline void* at(const void* p,U n){return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(p)+n);}
template<class T>T rd(const void* p,U n){T result;std::memcpy(&result,at(p,n),sizeof result);return result;}
template<class T>void wr(void* p,U n,T value){std::memcpy(at(p,n),&value,sizeof value);}
inline void* ptr(const void* p,U n){return rd<void*>(p,n);}
template<class F>F slot(void* p,U n){return rd<F>(ptr(p,0),n);}
inline void* client(){return ptr(kotor_menu_window_cells::manager_007a39fc,4);}
using Void=void(__fastcall*)(void*,void*);
using Ptr=void*(__fastcall*)(void*,void*);
using One=void(__fastcall*)(void*,void*,U);
using Three=void(__fastcall*)(void*,void*,U,U,U);
using Delete=void*(__fastcall*)(void*,void*,U);
using Pointer=void(__fastcall*)(void*,void*,const void*);
}
extern "C" {
extern void* kotor_cell_rules_007a3a28;
extern void* kotor_cell_sound_007a39ec;
extern const void* const recovered_client_placeable_methods_007537d0[82];
void* __fastcall kotor_m15a_object_get_server_object_0063d4b0(void*,void*);
void* __fastcall kotor_fastcall_005ed530(void*,void*);
void __fastcall recovered_client_area_remove_placeable_object_light_00605b50(void*,void*,U,U);
void __fastcall kotor_m15e_object_destruct_0063e800(void*,void*);
void __fastcall frame_LogStringDtor_005e5c20(void*,void*);
String* __fastcall GuiAssign005e5140(String*,void*,const char*);
I __fastcall c2da_current_get_int_00413660(void*,void*,I,void*,I*);
I __fastcall c2da_current_get_float_00413430(void*,void*,I,void*,float*);
I __fastcall c2da_current_get_string_00413270(void*,void*,I,void*,void*);
void __cdecl live_free(void*);
}
