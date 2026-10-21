#pragma once
#include "d4f4f521cc-global_aliases.h"
#include <cstddef>
#include <cstdint>
namespace kotor_message {
using Word=std::uint32_t;using Result=std::uint64_t;
struct Globals {kotor_menu_aliases::Cell32 render_7a39d8;
    kotor_menu_aliases::Cell32 input_7a39e4;
    kotor_menu_aliases::Cell32 manager_7a39fc;
    kotor_menu_aliases::Cell32 aurora_7a39f8;
    kotor_menu_aliases::Cell32 reinit_7a3a2c;
    Word movie_fullscreen_7a3a10;
    Word movie_inactive_7a3a18;
    Word focus_78d1d0;
    Word last_tick_7a3c4c;
    Word movie_window_7a3cf8;
    kotor_menu_aliases::Cell32 height_78d1d8;
    kotor_menu_aliases::Cell32 flag_8338f0;};   // D86(a): borrowed view of the f-input link cell 008338F0 (no storage here)
struct Frame {Word untouched[3],point_x,point_y,message[7];};
enum class Kind:Word {direct,import_slot,virtual_slot};
struct Call {Word site,target,self,count,args[4];Kind kind;Word slot;};
struct Bindings {void* context;Result (__cdecl* invoke)(void*,const Call&);Word (__cdecl* read_word)(void*,Word);void (__cdecl* write_word)(void*,Word,Word);};
struct Context {Globals* globals;Frame* frame;Bindings dependencies;};
extern Context* bound_context;
Word __stdcall message_00402800(Word window,Word message,Word wparam,Word lparam);
static_assert(sizeof(void*)==4&&offsetof(Frame,point_x)==0xc&&offsetof(Frame,message)==0x14&&sizeof(Frame)==0x30,"original x86 locals after saves");
}
