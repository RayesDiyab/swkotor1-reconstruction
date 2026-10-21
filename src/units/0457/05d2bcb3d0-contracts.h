#pragma once
#include "16b9992a5a-storage.h"
#include "f205cbc926-providers.h"
#include "c41186a1ec-modern_alpha.h"
#include "da2168170f-ftol2.h"
#include <windows.h>
#include <GL/gl.h>
#include <cstring>
namespace gui_wrap_draw {
using U8=unsigned char;using U16=unsigned short;using U32=unsigned int;using I32=int;using I16=short;
static_assert(sizeof(void*)==4 && sizeof(I32)==4 && sizeof(I16)==2,"Windows x86");
inline void* PointerAt(const void* p,unsigned n){void* v;std::memcpy(&v,static_cast<const U8*>(p)+n,4);return v;}
inline U32 WordAt(const void* p,unsigned n){U32 v;std::memcpy(&v,static_cast<const U8*>(p)+n,4);return v;}
inline I32 Signed(U32 v){I32 s;std::memcpy(&s,&v,4);return s;}
inline I32 ShortSigned(U32 v){U16 low=static_cast<U16>(v);I16 s;std::memcpy(&s,&low,2);return s;}
inline float FloatAt(const void* p,unsigned n){float v;std::memcpy(&v,static_cast<const U8*>(p)+n,4);return v;}
inline void StoreWord(void* p,unsigned n,U32 v){std::memcpy(static_cast<U8*>(p)+n,&v,4);}
inline void StorePointer(void* p,unsigned n,void* v){std::memcpy(static_cast<U8*>(p)+n,&v,4);}
struct Bindings {
 const volatile U32* pulse_0078d3d4;
 const volatile U32* busy_007bb4d0;
 const volatile U32* disable_0078d45c;
 volatile U32* reset_007a689c;
 const volatile I16* viewport_index_007b9460;
 const U8* viewport_dimensions_007b946c;
 const volatile float *hundred_00741838,*quarter_0073edf0,*negative_hundred_0074186c,*epsilon_00741870,*zero_0073d700,*one_0073d764,*channel_0073ede4;
 const volatile U32* quads_0073f34c;
 decltype(&::unresolved_ftol2_006fae8c) ftol_006fae8c; // Original non-C ABI: argument ST0, low result EAX; no stack words.
 int(__cdecl* alpha_006fc99d)(int);
 void*(__cdecl* allocate_006fa7e6)(U32);
 void(__cdecl* free_006fa390)(void*);
 void(__fastcall* array_allocate_005e03c0)(menu_list_growth::List*,void*,U32);
 void(__fastcall* array_add_00444fe0)(menu_list_growth::List*,void*,I32);
 void(__cdecl* viewport_alpha_00458ef0)(); // Original result ST0; no arguments.
 decltype(&::glDisable) const volatile* disable_0073d22c;
 decltype(&::glBegin) const volatile* begin_0073d2d0;
 decltype(&::glColor4ubv) const volatile* color_0073d2e8;
 decltype(&::glTexCoord2f) const volatile* texcoord_0073d2cc;
 decltype(&::glVertex4f) const volatile* vertex_0073d324;
 decltype(&::glEnd) const volatile* end_0073d2c4;
 decltype(&::glEnable) const volatile* enable_0073d390;
};
extern const Bindings& bindings;
void BindGenuineNumericAndLists(Bindings&);
// Existing task1 Text type imported. GUI-string object/type/vtable is owned by
// task35; void* is a bounded original memory receiver, not a second class.
__declspec(noinline) void __fastcall text_draw_00416240(menu_storage::Text*,void*,U32);
__declspec(noinline) void __fastcall wrap_0045a2f0(void*,void*,U32);
__declspec(noinline) void __fastcall draw_0045a850(void*,void*,U32);
}
