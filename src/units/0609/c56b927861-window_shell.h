#pragma once
#include "e4915bc513-global_aliases.h"
#include <cstddef>
#include <cstdint>
#include <initializer_list>
namespace kotor_window_shell {
using Word=std::uint32_t;
using Result=std::uint64_t;
// Observed storage only. Providers own the actual CExoString lifetime.
struct String {Word data,length;};
struct Class {Word style,procedure,class_extra,window_extra,instance,icon,cursor,brush,menu,class_name;};
struct Monitor {Word size;std::int32_t monitor[4],work[4];Word flags;};
struct Globals {
    String graphics_version_7a3cc4,title_7a3ccc,caption_prefix_7a3cdc;
    kotor_menu_aliases::Cell32 top_window_7a39d4;
    kotor_menu_aliases::Cell32 render_window_7a39d8;
    kotor_menu_aliases::Cell32 instance_7a39c0;
    Word enable_78d454,enable_78d474,enable_78d45c,enable_78d458,enable_78d448,enable_78d44c;
};
struct Constants {const char* top_class;char render_class[14];Word blend_source_73f270,blend_destination_73f274;};
struct Frame {
    String graphics,version,unused_caption,title,concat_graphics,concat_version;
    String render_title;
    Class top_class,render_class;
    Monitor monitor;
    char render_class_name[14];
};
struct Call {Word site,target,self,count,args[12];};
struct Bindings {void* context;Result (__cdecl* invoke)(void*,const Call&);};
struct Context {Globals* globals;Constants* constants;Frame* frame;Bindings dependencies;};
extern Context* bound_context;
Word __cdecl init_game_app_00403f20(Word instance,std::int32_t show);
// Original caller passes show too, but this body does not read it. Its EAX is
// incidental after the destructor; no defined return-value contract is added.
void __cdecl init_render_window_004036f0(Word instance,std::int32_t show);
void __stdcall setup_opengl_00401bb0();
static_assert(sizeof(void*)==4 && sizeof(String)==8 && sizeof(Class)==40 && sizeof(Monitor)==40,"Observed x86 views");
static_assert(offsetof(Class,procedure)==4 && offsetof(Class,class_name)==36 && offsetof(Monitor,flags)==36,"Observed native offsets");
}
