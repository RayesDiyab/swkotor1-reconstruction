#pragma once
// D1/D25 re-typing of M40 task-33 audio.hpp (4caf8676...a292): every slot uses an
// existing canonical producer type, so all slots are fillable without casts.
//  - INI slots: PublicIni member pointers (005E6750/005E67C0/005E67D0/005E67E0).
//  - String default/text ctor and dtor (005B3190/005E5A90/005E5C20) are the
//    canonical kotor_reconstruction::StartupString special members; a special
//    member has no address, so the three former slots are replaced by direct
//    construction/destruction of that canonical type (same callees).
//  - AsINT 005E5630: kotor_menu20::String member (layout-identical receiver).
//  - AsFLOAT 005E5650 / Get2D3DBias 005D6380: float-returning fastcall bodies
//    (replace the former ST0 void* hooks and their inline asm).
//  - Format 005E5680: kotor_reconstruction::StartupFormat005e5680 (cdecl, void).
//  - GetClientOptions 005ED700: kotor_menu20::ClientApp member pointer.
//  - Manager cell 007A39FC: kotor_menu_window_cells::manager_007a39fc.
#include <stdint.h>
#include "f9f7ab0530-startup_string.h"
#include "b2b642e2f3-base_ini.h"
#include "a0f716d4dd-menu_targets.h"
namespace audio {
using U = uint32_t;
using String = kotor_reconstruction::StartupString;
using Ini = base_ini_reconstruction::PublicIni;
struct Providers {
    Ini* (Ini::*ini_ctor)();
    int (Ini::*ini_read)(String*, String*, String*, String*);
    int (Ini::*ini_write)(String*, String*, String*, String*);
    void (Ini::*ini_dtor)();
    int32_t (kotor_menu20::String::*as_int)();
    float (__fastcall* as_float)(String*, void*);
    void (__cdecl* format)(String*, const char*, ...);
    void* (__cdecl* allocate)(U);
    void* (__fastcall* sound_ctor)(void*, void*, U, U, U, U);
    U (__fastcall* set_nonstream)(void*, void*, U);
    U (__fastcall* set_stream)(void*, void*, U);
    U (__fastcall* set_bias)(void*, void*, float);
    U (__fastcall* get_eax)(void*, void*);
    float (__fastcall* get_bias)(void*, void*);
    U (__fastcall* get_2d)(void*, void*);
    U (__fastcall* get_3d)(void*, void*);
    void* (kotor_menu20::ClientApp::*settings)();
    void* volatile* manager_slot;
    void** sound_slot;
};
extern Providers providers;
static_assert(sizeof(void*) == 4 && sizeof(String) == 8 && sizeof(Ini) == 4 &&
              sizeof(kotor_menu20::String) == 8, "x86 layout");
// EAX is the final INI destructor's raw result in the original; destroy_005e67e0
// is void, so EAX is not reproduced (StartServices 005F8CBF ignores it).
extern "C" U __stdcall audio_005f1f70();
}
// 005E5650 CExoString::AsFLOAT (authored in audio_retyped.cpp).
namespace kotor_sound_path { float __fastcall StringAsFloat005e5650(kotor_reconstruction::StartupString*, void*); }
