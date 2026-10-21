#pragma once
#include <cstdint>
#include <cstring>
namespace gui_mouse {
using Word=std::uint32_t;
struct Rect{std::int32_t x,y,w,h;};struct Vec{Word x,y,z;};
struct Gui{alignas(4) unsigned char bytes[0xa8];Word __thiscall activate_0040b060();};
struct Bindings{
 void* (__cdecl* allocate_006fa7e6)(Word);
 Word (__thiscall* construct_004174b0)(void*);
 Word (__thiscall* text_005e5a90)(void*,const char*);
 Word (__thiscall* load_00417620)(void*,void*,Word);
 void (__thiscall* destroy_005e5c20)(void*);
 Word (__thiscall* reference_00405fe0)(void*);
 void (__thiscall* alpha_0043e150)(void*,Word);
 const char* cursor_0073dff8;const char* empty_0073d71c;const void* color_0073df74;
};extern Bindings bindings;
}
