#pragma once
#include "717b2a06f6-growth.h"
namespace gui_viewport_list {
using Word=menu_list_growth::Word;
using List=menu_list_growth::List;
// Declaration-only views. The integrator publishes its existing canonical cells.
struct ViewportBindings {
 const volatile short* index_007b9460;
 const volatile float* alpha_007b9248;
 const volatile float* one_0073d764;
};
extern const ViewportBindings& viewport;
__declspec(noinline) void __fastcall add_00444fe0(List*,void*,int);
// Non-C numeric transport: no arguments, one unrounded result in ST0.
// Call only through the assembly boundary already used by Draw.
void __cdecl product_00458ef0();
}
