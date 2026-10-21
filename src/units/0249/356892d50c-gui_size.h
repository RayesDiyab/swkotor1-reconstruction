#pragma once
#include <cstdint>
#include <cstring>
namespace gui_size {
using Word=std::uint32_t;
struct Gui {
    alignas(4) unsigned char bytes[0xa8];
    __declspec(noinline) void __thiscall set_size_0040be70(std::int32_t width,std::int32_t height);
};
using Method=void (__thiscall*)(void*);
using ScalarDelete=void (__thiscall*)(void*,Word);
struct Bindings {Method changed_0040b420;Method refresh_0040acc0;};
extern Bindings bindings;
// Only a valid live0xA8 receiver and complete typed normal bindings are supported.
// Popup+7C, when nonnull, owns a live vtable with a thiscall slot0/oneWord delete.
static_assert(sizeof(void*)==4&&sizeof(Gui)==0xa8,"original x86 GUI image");
}
