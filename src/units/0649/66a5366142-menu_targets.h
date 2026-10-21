#pragma once
#include "d4b9a99015-ini_write.h"
#include "ef6d3b4b64-debug_constructor.h"
#include <cstdint>
#include <cstddef>
#include <cstdarg>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error MSVC x86 required
#endif
namespace kotor_menu20 {
using Word=std::uint32_t;
struct String {char* volatile data;Word volatile capacity;
 __declspec(noinline) std::int32_t __thiscall as_int_005e5630();
};
struct Input {Word unknown;void* volatile internal;__declspec(noinline) void __thiscall set_mouse_005df5d0(int x,int y);};
// Sole complete Debug declaration is supplied by debug_constructor.h.
using Ini=base_ini_reconstruction::PublicIni; // One canonical C++ receiver lifetime.
struct ClientInternal {Word unknown;void* volatile options;};
struct ClientApp {Word unknown;ClientInternal* volatile internal;__declspec(noinline) void* __thiscall get_options_005ed700();};
// Borrowed memory views, not additional production owners or allocating C++
// constructors. Unknown regions remain untouched on these observed paths.
struct NetLayer {
 unsigned char prefix[0x20000];Word volatile field0,field4;void* volatile app;NetLayer* volatile peer;Word volatile initialized,field14;
 String string18;unsigned char unknown20[16];Word volatile field30,field34,field38,field3c;String strings40[4];Word volatile field60;String string64;Word volatile field6c,field70,field74,field78;
 __declspec(noinline) NetLayer* __thiscall construct_normal_005d54a0();
 __declspec(noinline) Word __thiscall initialize_005d5550(void* application);
};
struct Ime {unsigned char prefix[0x104];Word volatile field104,field108,unknown10c;String strings110[3];__declspec(noinline) Ime* __thiscall construct_normal_005ee400();};
static_assert(sizeof(void*)==4&&sizeof(String)==8&&offsetof(String,capacity)==4,"historical width");
static_assert(offsetof(NetLayer,field0)==0x20000&&offsetof(NetLayer,peer)==0x2000c&&offsetof(NetLayer,string18)==0x20018&&offsetof(NetLayer,strings40)==0x20040&&offsetof(NetLayer,string64)==0x20064&&sizeof(NetLayer)==0x2007c,"observed network fields");
static_assert(offsetof(Ime,strings110)==0x110&&sizeof(Ime)==0x128,"observed IME fields");
struct Bindings {
 std::int32_t(__cdecl* atol_006fafe7)(const char*);
 void*(__cdecl* allocate_006fa7e6)(Word);
 void(__cdecl* free_006fa390)(void*);
 // Native thiscall-compatible projections: ECX receiver, ignored EDX, stack words, callee cleanup.
 void(__fastcall* set_mouse_005e0960)(void*,void*,int,int);
 void(__fastcall* open_logs_0060e760)(void*,void*,Word);
 void(__fastcall* ini_destroy_005eb500)(base_ini_reconstruction::Ini*,void*);
 void*(__fastcall* player_construct_005d5440)(void*,void*);
 String*(__fastcall* string_construct_005b3190)(String*,void*);
 String*(__fastcall* string_assign_005e5140)(String*,void*,const char*);
 std::int32_t(__cdecl* vsnprintf_006fd5a8)(char*,Word,const char*,va_list);
 char*(__cdecl* strncpy_006fab80)(char*,const char*,Word);
 NetLayer* volatile* network_head_0083288c;
 char* volatile* format_buffer_008328a8;
 Word volatile* format_capacity_008328ac;
 const char* initial_text_0073d71c;
};
extern const Bindings bindings;
}
