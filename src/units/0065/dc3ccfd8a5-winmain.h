#pragma once
#include "a093173e29-global_aliases.h"
#include <cstdint>
#include <initializer_list>
namespace kotor_winmain {
using Word=std::uint32_t;
using Tick=std::uint64_t;
struct String {Word data,length;};
struct Ini {Word opaque;};
struct Message {Word hwnd,message,wparam,lparam,time;std::int32_t x,y;};
struct Security {Word length,descriptor,inherit;};
struct Frame {
    // Caller-supplied bits model native entry storage, including MSG before any
    // successful Peek/GetMessage write. No invented zero initialization.
    Ini ini; String s14,value,s28,s30; Message message;
};
struct Globals {
    kotor_menu_aliases::Cell32 base_7a39e0;
    kotor_menu_aliases::Cell32 manager_7a39fc;
    Word quit_7a39b8;
    Word argc_833bfc,argv_833c00,callback_78e39c;
    // Borrowed sole owner kotor_cell_disable_sound_00832894 (lane-sound): the
    // INI value written at 00404382 must reach CExoSound (005DA8AE etc.).
    kotor_menu_aliases::Cell32 disable_sound_832894;
    Word priority_changed_7a3ca4;
    Word priority_mode_7a3ca0;
    kotor_menu_aliases::Cell32 reinit_7a3a2c;
    Word width_78d1d4;
    kotor_menu_aliases::Cell32 height_78d1d8; std::uint8_t depth_78d1e0;
    Word headlight_7a3c50,frame_word_7a3c5c;
    float bias_7b9274,client_ms_7a3c7c,server_ms_7a3c80,cap_7a3c64;
    Tick start_7a39c8,end_7a39b0,cap_start_7a3c68,cap_end_7a3c70;
    Word debug_7a3a40,deferred_7a3a48,position_7a3a44;
    char text_7a3a4c[1]; // Only WinMain's observed first-byte write is represented.
    kotor_menu_aliases::Cell32 hwnd_7a39d8;
    Word swapping_7a3c9c;
    Word sleep_enabled_7a3c58;
    Word sleep_ms_78d1e8;
    std::int32_t mouse_trails_7a3ca8,screen_saver_7a3cac;
};
struct Call {
    Word site,target,self,count,args[5];
    bool has_x87_operand;
    unsigned char x87_operand[10];
};
struct Bindings {
    void* context;
    Tick (__cdecl* invoke)(void*,const Call&);
    Word (__cdecl* read_word)(void*,Word);
};
struct Context { Globals* globals; Frame* frame; Bindings dependencies; };
// Test binding only. A production resolver/state/frame mapping is outstanding.
extern Context* bound_context;
std::int32_t __stdcall winmain_004041f0(Word instance,Word previous,char* command,std::int32_t show);
float delta_milliseconds(Tick);
bool delta_less_than(Tick,float);
static_assert(sizeof(void*)==4 && sizeof(String)==8 && sizeof(Ini)==4 && sizeof(Message)==28 && sizeof(Security)==12,"Observed x86 views");
}
