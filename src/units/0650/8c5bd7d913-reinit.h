#pragma once
#include "2b112e8e36-global_aliases.h"
#include <cstdint>
#include <cstddef>
namespace kotor_reinit {
using Word=std::uint32_t;
using Result=std::uint64_t;
struct String {Word data,length;};
struct Globals {kotor_menu_aliases::Cell32 aurora_7a39f8;
    kotor_menu_aliases::Cell32 manager_7a39fc;
    kotor_menu_aliases::Cell32 input_7a39e4;
    kotor_menu_aliases::Cell32 top_7a39d4;
    kotor_menu_aliases::Cell32 render_7a39d8;
    kotor_menu_aliases::Cell32 instance_7a39c0;
    Word guard_7a3a3c;
    kotor_menu_aliases::Cell32 reinit_7a3a2c;
    Word mode_78d1dc;};
// Provider-owned literals and callable thread entry. No historical VA is invoked.
struct Constants {Word ini_file,section,disable_vbo,disable_write_only,thread_class,thread_procedure;};
// Offsets are relative to ESP after the original three nonvolatile pushes.
// The upper three bytes of mouse_mode are deliberately supplied, not invented.
struct Frame {
    Word untouched[4],mouse_mode,nested_or_exit,backdrop,ini,mode,thread,event,thread_id;
    union {String string_30;Word allocation;};
    String value,string_40,string_48;
    Word clear_color[4];
};
enum class Kind:Word {direct,import_slot,virtual_slot};
struct Call {Word site,target,self,count,args[12];Kind kind;Word slot;};
struct Bindings {void* context;Result (__cdecl* invoke)(void*,const Call&);Word (__cdecl* read_word)(void*,Word);};
struct Context {Globals* globals;Constants* constants;Frame* frame;Bindings dependencies;};
extern Context* bound_context;
// Five full stack words; cdecl caller cleanup20. No EAX return contract.
// Saved byte annotation for depth does not cover the full DWORD read at 403a73.
void __cdecl reinit_00403800(Word width,Word height,Word depth,Word fullscreen,Word check_movies);
static_assert(sizeof(void*)==4 && sizeof(String)==8,"x86 observed views");
static_assert(offsetof(Frame,mouse_mode)==0x10 && offsetof(Frame,nested_or_exit)==0x14 && offsetof(Frame,ini)==0x1c,"original local offsets");
static_assert(offsetof(Frame,string_30)==0x30 && offsetof(Frame,value)==0x38 && offsetof(Frame,clear_color)==0x50,"original local aliases");
}
